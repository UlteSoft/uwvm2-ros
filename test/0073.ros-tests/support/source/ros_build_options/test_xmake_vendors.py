#!/usr/bin/env python3
"""Run the actual xmake vendor selector serially, without compiling ROS.

This is a real xmake configuration contract. Its two empty header files are
explicit input fixtures, never generated Native type identities or adapters.
Run only in the admitted Linux 64 GiB / zero-swap group. Compiler, Guest,
middleware, hardware and complete upstream-suite qualification are separate.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import resource
import shutil
import signal
import subprocess
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--xmake', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--physical-storage', type=Path,
                        help='Actual physical filesystem whose 8GiB free floor must be preserved; output may be a bounded tmpfs')
    args = parser.parse_args()
    root = args.source_root.resolve(strict=True)
    xmake = args.xmake.resolve(strict=True)
    out = args.out.absolute()
    leaf = Path('/proc/self/cgroup').read_text().strip().split('0::', 1)[1]
    group = Path('/sys/fs/cgroup') / leaf.lstrip('/')
    spec = importlib.util.spec_from_file_location('xmake_configuration_cache_admission',
        root / 'test/0073.ros-tests/support/source/ros_language_integration/cgroup_budget.py')
    budget = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(budget)
    admission_samples = []
    def admission():
        if (not leaf.startswith('/system.slice/docker-') or
                (group / 'memory.max').read_text().strip() != '68719476736' or
                (group / 'memory.swap.max').read_text().strip() != '0'):
            raise RuntimeError('the original hard64GiB/swap0 Linux group is required')
        # Each serial child retains its 512MiB address-space ceiling. Admit
        # only with a conservative 3GiB margin, crediting inactive ordinary
        # clean/unmapped file pages. Shared/tmpfs pages are never credited.
        # The independent outer owner also checks group identity and bounds.
        sample = budget.sample(group)
        admission_samples.append(sample)
        if sample['admission_bytes'] > 61 * 1024**3:
            raise RuntimeError('lightweight configuration budget unavailable; do not compete with a large build')
        space = os.statvfs(args.physical_storage or out.parent)
        if space.f_bfree * space.f_frsize < 8 * 1024**3:
            raise RuntimeError('preserve the actual 8GiB physical filesystem floor')
        scratch = os.statvfs(out.parent)
        if scratch.f_bavail * scratch.f_frsize < 64 * 1024**2:
            raise RuntimeError('bounded configuration scratch margin unavailable')
    admission()
    out.mkdir(mode=0o700)
    module = root / 'xmake/ros_vendors.lua'
    source = module.read_bytes()
    inventory = json.loads((root / 'sdk/origin/robots/vendors/inventory-v2.json').read_bytes())
    names = ['ros-vendor-' + row['id'].replace('_', '-') for row in inventory['vendors']]
    names += ['ros-platform-isaac-ros', 'ros-platform-isaac-sim', 'ros-platform-groot']
    if len(set(names)) != 31:
        raise RuntimeError('the exact28 vendors and3 data platforms are required')
    first, second = out / 'fixture-a.hpp', out / 'fixture-b.hpp'
    first.write_text('// Configuration fixture only; not Native schema proof.\n')
    second.write_text('// A different physical configuration fixture.\n')
    # Each tuple names an actual independent xmake invocation and expectation.
    cases = [('all-disabled', [], True, '')]
    for name in names:
        cases.append((name + '-selected', ['--import-ros2=y', '--' + name + '=y',
                      '--' + name + '-registry-header=' + str(first)], True, str(first)))
        cases.append((name + '-missing-registry', ['--import-ros2=y', '--' + name + '=y'], False, None))
        cases.append((name + '-disabled-injection', ['--import-ros2=y',
                      '--' + name + '-registry-header=' + str(first)], False, None))
    a, b = 'ros-vendor-unitree', 'ros-vendor-fanuc'
    shared = ['--import-ros2=y', '--' + a + '=y', '--' + b + '=y',
              '--' + a + '-registry-header=' + str(first)]
    cases += [
        ('two-vendors-one-bundle', shared + ['--' + b + '-registry-header=' + str(first)], True, str(first)),
        ('two-independent-bundles-refused', shared + ['--' + b + '-registry-header=' + str(second)], False, None),
        ('legacy-registry-conflict', ['--import-ros2=y', '--' + a + '=y',
            '--' + a + '-registry-header=' + str(first), '--ros-custom-registry-header=' + str(second)], False, None),
        ('no-implicit-backend', ['--' + a + '=y', '--' + a + '-registry-header=' + str(first)], False, None),
        ('ros1-vendor-independent', ['--import-ros1=y', '--' + a + '=y',
            '--' + a + '-registry-header=' + str(first)], True, str(first)),
        ('isaac-needs-ros2', ['--import-ros1=y', '--ros-platform-isaac-ros=y',
            '--ros-platform-isaac-ros-registry-header=' + str(first)], False, None),
        ('jetson-rejects-x86', ['--import-ros2=y', '--ros-platform-jetson=y'], False, None),
        ('jetson-arm64-deployment', ['--import-ros2=y', '--ros-platform-jetson=y'], True, ''),
        ('legacy-explicit-registry', ['--import-ros2=y',
            '--ros-custom-registry-header=' + str(first)], True, str(first)),
        ('all-explicit-one-bundle', ['--import-ros2=y'] + [flag for name in names
            for flag in ('--' + name + '=y', '--' + name + '-registry-header=' + str(first))], True, str(first)),
    ]
    results = []
    events_before = (group / 'memory.events').read_text()
    for index, (label, flags, accepted, expected) in enumerate(cases):
        admission()
        case = out / ('case-%03d' % index)
        case.mkdir()
        (case / 'ros_vendors.lua').write_bytes(source)
        # Execute the complete actual production Lua module through the real
        # xmake project interpreter, rather than a Lua/Python mock parser.
        (case / 'xmake.lua').write_text('''set_xmakever("2.9.8")
set_project("ros_vendor_configuration_contract")
option("import-ros1", function() set_default(false) end)
option("import-ros2", function() set_default(false) end)
option("ros-custom-registry-header", function() set_default("none") end)
includes("ros_vendors.lua")
target("selection")
    set_kind("phony")
    local registry = uwvm_ros_selected_vendor_registry()
    on_load(function(target)
        io.writefile(path.join(os.projectdir(), "actual-selected.txt"), registry or "")
    end)
target_end()
''')
        architecture = 'arm64' if label == 'jetson-arm64-deployment' else 'x86_64'
        argv = [str(xmake), 'f', '-q', '-c', '-P', str(case), '-p', 'linux', '-a', architecture] + flags
        stdout, stderr = case / 'stdout.log', case / 'stderr.log'
        def limits():
            resource.setrlimit(resource.RLIMIT_AS, (512 * 1024**2, 512 * 1024**2))
            resource.setrlimit(resource.RLIMIT_FSIZE, (1024**2, 1024**2))
            resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        started = time.monotonic()
        with stdout.open('xb') as so, stderr.open('xb') as se:
            process = subprocess.Popen(argv, stdin=subprocess.DEVNULL, stdout=so, stderr=se,
                cwd=case, start_new_session=True, preexec_fn=limits,
                env={**os.environ, 'XMAKE_ROOT': 'y', 'LC_ALL': 'C',
                     'XMAKE_GLOBALDIR': str(out / '.xmake-global')})
            try:
                status = process.wait(timeout=20)
            except BaseException:
                try: os.killpg(process.pid, signal.SIGKILL)
                except ProcessLookupError: pass
                process.wait()
                raise
        observed_path = case / 'actual-selected.txt'
        observed = observed_path.read_text() if observed_path.exists() else None
        passed = (status == 0 and observed == expected) if accepted else status != 0
        results.append(dict(label=label, argv=argv, returncode=status,
            expected_configuration_acceptance=accepted, actual_selected=observed,
            passed=passed, seconds=time.monotonic() - started,
            stdout=str(stdout), stderr=str(stderr)))
        if not passed:
            break
    report = dict(kind='actual-xmake-vendor-configuration-contract',
        passed=len(results) == len(cases) and all(row['passed'] for row in results),
        planned_cases=len(cases), executed_cases=len(results), cases=results,
        production_source=dict(path=str(module), bytes=len(source), sha256=hashlib.sha256(source).hexdigest()),
        actual_xmake=dict(path=str(xmake), sha256=hashlib.sha256(xmake.read_bytes()).hexdigest()),
        cgroup=dict(path=str(group), device=group.stat().st_dev, inode=group.stat().st_ino,
                    events_before=events_before, events_after=(group / 'memory.events').read_text(),
                    admission_samples=admission_samples, limits_or_cache_modified=False),
        physical_storage_floor_path=str(args.physical_storage or out.parent),
        observed_children_peak_RSS_bytes=resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss * 1024,
        compiler_Guest_Native_ROS_QEMU_runs=0, default_authority=[],
        qualification='Actual xmake configuration only; no vendor type/layout/Native middleware or binary-size/performance acceptance.')
    if module.read_bytes() != source:
        raise RuntimeError('production Lua changed during the contract')
    (out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({key: report[key] for key in ('passed', 'planned_cases', 'executed_cases', 'observed_children_peak_RSS_bytes')}))
    return 0 if report['passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
