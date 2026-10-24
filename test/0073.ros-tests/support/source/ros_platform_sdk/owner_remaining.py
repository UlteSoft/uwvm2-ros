#!/usr/bin/env python3
"""Own finite Remaining-platform source-review work in the original hard64GiB/swap0 group.

Metadata review has a 512MiB virtual-address/RSS allowance. Original-source
capture has a 1GiB virtual-address and 768MiB sampled RSS allowance. Compiler
lanes have a 16GiB virtual-address and 1536MiB process-tree RSS allowance;
the explicitly selected current Runtime compiler lane allows 3072MiB RSS.
Every lane has finite CPU/output/file/wall budgets and independently checked
original cgroup membership. No cache/limit mutation, robot connection or
Guest authority is introduced; source capture executes no upstream code.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import re
import resource
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
    parser.add_argument('--phase', choices=['discover', 'capture', 'review', 'catalog', 'prepare', 'assemble', 'extras', 'repair', 'agibot', 'normalize', 'fourier', 'validate', 'boundary', 'restore', 'qualify', 'managed', 'values', 'cross', 'finish', 'audit', 'endian', 'reproduce', 'build', 'perf', 'runtimecheck', 'sizes', 'configuration', 'layout', 'pythonprepare', 'pythonverify', 'pythonfetch', 'pythonbuild', 'pythonwasm', 'pythonpublish', 'pythonmulti', 'unitreecapture', 'unitreesport', 'unitreesportbuild', 'unitreepublish', 'unitreedelivery', 'unitreepython', 'unitreepythonwasm', 'unitreepythondelivery'], required=True)
    parser.add_argument('--attempt', default='r1')
    args = parser.parse_args()
    assert re.fullmatch(r'[a-z][a-z0-9-]{0,31}', args.attempt)
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    helper = module('remaining_owner_base', source / 'test/0073.ros-tests/support/source/ros_language_integration/native_owner.py')
    budget = module('remaining_cache_admission', source / 'test/0073.ros-tests/support/source/ros_language_integration/cgroup_budget.py')
    execution = args.phase in ('registry', 'qualify', 'managed', 'cross', 'finish', 'values', 'endian', 'build', 'perf', 'runtimecheck', 'configuration', 'layout', 'pythonverify', 'pythonbuild', 'pythonwasm', 'pythonpublish', 'pythonmulti', 'unitreesport', 'unitreesportbuild', 'unitreepublish', 'unitreepython', 'unitreepythonwasm', 'unitreepythondelivery')
    owned_scope = None
    owned_scope_identity = None

    class Owner(helper.Owner):
        def guard(self):
            super().guard()
            sample = budget.sample(self.group)
            self.report.setdefault('first_accounting', sample)
            self.report['last_accounting'] = sample
            return sample['admission_bytes']

        @staticmethod
        def limits():
            resource.setrlimit(resource.RLIMIT_AS, ((16384 if execution else 512 if args.phase == 'discover' else 1024) << 20,) * 2)
            resource.setrlimit(resource.RLIMIT_CPU, (180 if args.phase == 'discover' else 1800 if execution else 600,) * 2)
            resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
            resource.setrlimit(resource.RLIMIT_FSIZE, ((32 if args.phase == 'discover' else 384 if args.phase == 'agibot' else 256) << 20,) * 2)
            resource.setrlimit(resource.RLIMIT_NOFILE, (1024 if execution else 256,) * 2)
            if owned_scope is not None:
                # A fresh descendant cgroup changes no parent resource limit.
                # Profiler/compiler helpers stay inside the original 64GiB
                # accounting tree even if they create a different session.
                (owned_scope / 'cgroup.procs').write_text(str(__import__('os').getpid()))
                assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership'] + '/' + owned_scope.name

        def observe(self):
            assert self.guard() < 63 << 30, 'shared 1GiB running margin'
            total = 0
            for actor in self.actors:
                if actor['process'].poll() is not None:
                    continue
                used = 0
                for pid in self.descendants(actor['process'].pid):
                    used += self.rss(pid)
                    try:
                        expected = inputs['cgroup']['membership'] + ('/' + owned_scope.name if owned_scope is not None else '')
                        assert (Path('/proc') / str(pid) / 'cgroup').read_text().strip() == '0::' + expected
                    except (FileNotFoundError, ProcessLookupError):
                        pass
                total += used
                actor['row']['max_sampled_owned_rss'] = max(actor['row']['max_sampled_owned_rss'], used)
                assert time.monotonic() - actor['started'] < actor['row']['deadline_seconds']
                assert sum((actor['directory'] / n).stat().st_size for n in ('stdout', 'stderr')) <= 8 << 20
            self.report['owned_aggregate_rss_limit_bytes'] = (3072 if args.phase == 'build' else 1536 if execution else 512 if args.phase == 'discover' else 768) << 20
            assert total <= self.report['owned_aggregate_rss_limit_bytes']

        def close(self):
            super().close()
            if owned_scope is None:
                return
            info = owned_scope.stat()
            assert [info.st_dev, info.st_ino] == owned_scope_identity
            deadline = time.monotonic() + 5
            while 'populated 1' in (owned_scope / 'cgroup.events').read_text() and time.monotonic() < deadline:
                time.sleep(.05)
            forced = 'populated 1' in (owned_scope / 'cgroup.events').read_text()
            if forced:
                # This exact fresh group belongs only to this phase. Atomic
                # cgroup cleanup also catches session-detached descendants;
                # no unrelated process ID or sibling group is signalled.
                (owned_scope / 'cgroup.kill').write_text('1')
                deadline = time.monotonic() + 5
                while 'populated 1' in (owned_scope / 'cgroup.events').read_text():
                    assert time.monotonic() < deadline, 'owned subgroup failed to retire'
                    time.sleep(.05)
            owned_scope.rmdir()
            self.report['owned_subgroup_closed'] = True
            self.report['owned_subgroup_forced_cleanup'] = forced
            self.report['events_after'] = (self.group / 'memory.events').read_text()
            if forced and self.report['status'] == 'PASS':
                self.report.update(status='FAILED_RETAINED', error='Owned descendants survived normal phase completion')
                self.save()
                raise RuntimeError(self.report['error'])

    report = dict(status='RUNNING', phase=args.phase, attempt=args.attempt,
                  inputs=inputs, default_authority=[], original_official_cases_run=0,
                  scope='Bounded source preparation or explicitly selected authored SDK execution; no original official-suite credit')
    owner = Owner(root, inputs, report, 'remaining-' + args.phase + '-' + args.attempt + '.json')
    try:
        for row in inputs['pins']:
            path = Path(row['path'])
            assert path.stat().st_size == row['bytes'] and helper.sha(path) == row['sha256']
        owner.admit()
        if args.phase in ('build', 'perf'):
            candidate = owner.group / ('uwvm-ros-r60-' + args.phase + '-' + args.attempt)
            assert not candidate.exists()
            candidate.mkdir(mode=0o700)
            owned_scope = candidate
            info = owned_scope.stat()
            owned_scope_identity = [info.st_dev, info.st_ino]
            report['owned_subgroup'] = dict(path=str(owned_scope), identity=owned_scope_identity,
                parent_identity=inputs['cgroup']['identity'], parent_hard_bytes=64 << 30,
                parent_swap_bytes=0, parent_limits_modified=False)
            owner.save()
        argv = [sys.executable, '-I', '-S', '-B', source / 'test/0073.ros-tests/support/source/ros_platform_sdk' / (args.phase + '_remaining.py'),
                '--out', root / 'out' / (args.phase + '-' + args.attempt)]
        argv += ['--root', root]
        if args.phase == 'cross':
            argv += ['--vectors', inputs['selected_direct_result']]
        actor = owner.launch(args.phase + '-' + args.attempt, argv,
                             deadline=900 if args.phase == 'discover' else 3600 if execution else 1800)
        owner.wait(actor)
        for row in inputs['pins']:
            path = Path(row['path'])
            assert path.stat().st_size == row['bytes'] and helper.sha(path) == row['sha256']
        report.update(status='PASS', all_admitted_source_and_tool_pins_preserved=True)
        print('REMAINING_PLATFORM_PHASE_PASS', args.phase, args.attempt, flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        owner.close()
        owner.save()


if __name__ == '__main__':
    main()
