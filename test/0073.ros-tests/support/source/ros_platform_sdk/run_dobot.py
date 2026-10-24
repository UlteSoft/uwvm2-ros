#!/usr/bin/env python3
"""Own finite serial DOBOT source/SDK qualification inside the original cgroup.

This is an operator test harness, never a Guest entry or native robot driver.
Each child has independent CPU, output, descriptor, wall and aggregate RSS
limits. Cache accounting changes admission only; hard64GiB/swap0 is invariant.
"""
import argparse
import importlib.util
import json
import re
from pathlib import Path
import sys
import time


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--phase', choices=['capture', 'prepare', 'assemble', 'contracts', 'qualify', 'managed', 'finish', 'cross', 'kernel'], required=True)
    parser.add_argument('--attempt', default='r1', help='Fresh finite ledger name; earlier attempts remain immutable')
    args = parser.parse_args()
    assert re.fullmatch(r'[a-z][a-z0-9-]{0,31}', args.attempt)
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    helper = module('dobot_process_owner', source / 'test/0073.ros-tests/support/source/ros_language_integration/native_owner.py')
    budget = module('dobot_cache_admission', source / 'test/0073.ros-tests/support/source/ros_language_integration/cgroup_budget.py')

    class Owner(helper.Owner):
        def guard(self):
            super().guard()
            sample = budget.sample(self.group)
            self.report.setdefault('first_accounting', sample)
            self.report['last_accounting'] = sample
            return sample['admission_bytes']

        def observe(self):
            # The complete C++ DTO translation units exceed the older common
            # language fixture's 768MiB compiler ceiling. Keep this explicit,
            # serial 1.5GiB build lane separate; the kernel hard64GiB remains.
            assert self.guard() < 63 << 30, '1GiB conservative running margin'
            total = 0
            for actor in self.actors:
                if actor['process'].poll() is not None:
                    continue
                used = 0
                for pid in self.descendants(actor['process'].pid):
                    used += self.rss(pid)
                    try:
                        assert (Path('/proc') / str(pid) / 'cgroup').read_text().strip() == '0::' + self.inputs['cgroup']['membership']
                    except (FileNotFoundError, ProcessLookupError):
                        pass
                total += used
                actor['row']['max_sampled_owned_rss'] = max(actor['row']['max_sampled_owned_rss'], used)
                assert time.monotonic() - actor['started'] < actor['row']['deadline_seconds'], 'actor deadline'
                assert sum((actor['directory'] / n).stat().st_size for n in ('stdout', 'stderr')) <= 8 << 20, 'output bound'
            maximum = (1536 if args.phase in ('qualify', 'managed', 'finish', 'cross') else 768) << 20
            self.report['owned_aggregate_rss_limit_bytes'] = maximum
            assert total <= maximum, 'finite aggregate sampled owned RSS bound'

    report = dict(status='RUNNING', phase=args.phase, inputs=inputs,
                  scope='Selected original source and SDK/data contracts; native robot/performance remain separate',
                  default_authority=[])
    owner = Owner(root, inputs, report, 'dobot-' + args.phase + '-' + args.attempt + '.json')

    def run(label, argv, deadline=180, env=None):
        owner.admit()
        actor = owner.launch(args.phase + '-' + args.attempt + '-' + label, argv, extra=env, deadline=deadline)
        while actor['process'].poll() is None:
            owner.observe()
            time.sleep(0.03)
        actor['row']['exit'] = actor['process'].wait()
        owner.save()
        assert actor['row']['exit'] == 0, (label, actor['row']['exit'], owner.text(actor, 'stderr')[-2400:])
        return actor

    try:
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256'], path
        if args.phase == 'capture':
            argv = [sys.executable, '-I', '-S', '-B',
                source / 'sdk/origin/robots/arms/dobot/tools/capture_sources.py',
                '--out', inputs.get('capture_output', root / 'out/acquired')]
            if inputs.get('archives'):
                argv += ['--archives', inputs['archives']]
            if inputs.get('upstream_input'):
                argv += ['--upstream-input', inputs['upstream_input']]
            run('original-source', argv, deadline=600)
        elif args.phase == 'prepare':
            for profile in ('ros1_v3', 'ros1_v4', 'ros2_v4'):
                argv = [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/robots/industrial/prepare.py',
                    '--source-root', source, '--catalog', Path(inputs.get('capture_output', root / 'out/acquired')) / 'catalogs' / profile / 'catalog.json',
                    '--out', root / 'out' / ('prepared-' + profile + '-' + args.attempt), '--mode', 'prepare']
                for role, relative in [('parent', 'sdk/origin/tools/custom_types.py'),
                    ('bounded', 'sdk/origin/custom-types/bounded_profile.py'),
                    ('adapter', 'sdk/origin/robots/industrial/vendor_profile.py')]:
                    argv += ['--' + role + '-sha256', helper.sha(source / relative)]
                run(profile, argv)
        elif args.phase == 'contracts':
            run('contract-sources', [sys.executable, '-I', '-S', '-B',
                source / 'sdk/origin/robots/arms/dobot/tools/emit_contracts.py', '--source', source,
                '--out', root / 'out' / ('contract-sources-' + args.attempt)])
        elif args.phase == 'assemble':
            kit = source / 'sdk/origin/robots/arms/dobot'
            run('example-sources', [sys.executable, '-I', '-S', '-B', kit / 'tools/emit_examples.py',
                '--source', source, '--out', root / 'out/example-sources'])
            run('contract-sources', [sys.executable, '-I', '-S', '-B', kit / 'tools/emit_contracts.py',
                '--source', source, '--out', root / 'out/contract-sources'])
            report['published_sources'] = []
            for base, destination in [(root / 'out/example-sources', kit), (root / 'out/contract-sources', kit / 'tests')]:
                for path in sorted(base.rglob('*')):
                    if not path.is_file():
                        continue
                    assert not path.is_symlink()
                    target = destination / path.relative_to(base)
                    target.parent.mkdir(parents=True, exist_ok=True)
                    with target.open('xb') as stream:
                        stream.write(path.read_bytes())
                    report['published_sources'].append(dict(path=str(target), bytes=target.stat().st_size, sha256=helper.sha(target)))
        elif args.phase == 'kernel':
            run('matching-kernel-data', [sys.executable, '-I', '-S', '-B',
                source / 'test/0073.ros-tests/support/source/ros_platform_sdk/acquire_dobot_kernel.py', '--metadata', inputs['kernel_metadata'],
                '--out', inputs.get('kernel_output', root / 'tools/kernel-uapi-7.2.10')], deadline=240)
        elif args.phase == 'cross':
            run('four-architectures', [sys.executable, '-I', '-S', '-B',
                source / 'test/0073.ros-tests/support/source/ros_platform_sdk/cross_dobot.py', '--root', root,
                '--out', root / 'out' / ('cross-' + args.attempt),
                '--vectors', inputs['qualified_vectors']], deadline=2400)
        elif args.phase == 'finish':
            run('profile-source-cmake', [sys.executable, '-I', '-S', '-B',
                source / 'test/0073.ros-tests/support/source/ros_platform_sdk/finish_dobot.py', '--root', root,
                '--out', root / 'out' / ('finish-' + args.attempt)], deadline=1200)
        elif args.phase == 'managed':
            run('four-languages', [sys.executable, '-I', '-S', '-B',
                source / 'test/0073.ros-tests/support/source/ros_platform_sdk/managed_dobot.py', '--root', root,
                '--out', root / 'out' / ('managed-qualification-' + args.attempt)], deadline=2400)
        else:
            run('c-cpp-data', [sys.executable, '-I', '-S', '-B',
                source / 'test/0073.ros-tests/support/source/ros_platform_sdk/qualify_dobot.py', '--root', root,
                '--out', root / 'out' / ('qualification-' + args.attempt)], deadline=2400)
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256'], path
        report['all_admitted_source_and_tool_pins_preserved'] = True
        report['status'] = 'DOBOT_' + args.phase.upper() + '_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        owner.close()


if __name__ == '__main__':
    main()
