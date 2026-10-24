#!/usr/bin/env python3
"""Bound a fresh trusted Doosan operator phase in the original hard64GiB group.

All compiler/Guest/QEMU descendants share the kernel limit and a separate finite
sampled RSS, CPU, output and wall allowance. No cgroup/cache/kernel setting is
changed. This harness is not callable through the Guest ROS import surface.
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
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True)
    p.add_argument('--phase', choices=['discover','capture','prepare','assemble','qualify','managed','cross','finish','verify','audit','native','nativecheck','nativeformat','registry'], required=True)
    p.add_argument('--attempt', default='r1')
    a = p.parse_args()
    assert re.fullmatch(r'[a-z][a-z0-9-]{0,31}', a.attempt)
    root = a.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    helper = module('doosan_owner_base', source / 'test/0073.ros-tests/support/source/ros_language_integration/native_owner.py')
    budget = module('doosan_cache_admission', source / 'test/0073.ros-tests/support/source/ros_language_integration/cgroup_budget.py')

    class Owner(helper.Owner):
        def guard(self):
            super().guard()
            sample = budget.sample(self.group)
            self.report.setdefault('first_accounting', sample)
            self.report['last_accounting'] = sample
            return sample['admission_bytes']

        @staticmethod
        def limits():
            # Each complete five-domain serial phase has a finite allowance;
            # its own child command deadlines are checked independently.
            resource.setrlimit(resource.RLIMIT_CPU, (900, 900))
            resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
            resource.setrlimit(resource.RLIMIT_AS, (16 << 30, 16 << 30))
            file_limit = 64 << 20
            resource.setrlimit(resource.RLIMIT_FSIZE, (file_limit, file_limit))
            resource.setrlimit(resource.RLIMIT_NOFILE, (1024, 1024))

        def observe(self):
            assert self.guard() < 63 << 30
            total = 0
            for actor in self.actors:
                if actor['process'].poll() is not None:
                    continue
                used = 0
                for pid in self.descendants(actor['process'].pid):
                    used += self.rss(pid)
                    try:
                        assert (Path('/proc') / str(pid) / 'cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
                    except (FileNotFoundError, ProcessLookupError):
                        pass
                total += used
                actor['row']['max_sampled_owned_rss'] = max(actor['row']['max_sampled_owned_rss'], used)
                assert time.monotonic() - actor['started'] < actor['row']['deadline_seconds']
                assert sum((actor['directory'] / n).stat().st_size for n in ('stdout','stderr')) <= 8 << 20
            self.report['owned_aggregate_rss_limit_bytes'] = 1536 << 20
            assert total <= 1536 << 20

    report = dict(status='RUNNING', phase=a.phase, attempt=a.attempt,
        default_authority=[], inputs=inputs, original_official_cases_run=0,
        scope='Selected original SDK/data/example contracts only')
    owner = Owner(root, inputs, report, 'doosan-' + a.phase + '-' + a.attempt + '.json')
    try:
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256'], path
        owner.admit()
        actor = owner.launch(a.phase + '-' + a.attempt,
            [sys.executable, '-I','-S','-B', source / 'test/0073.ros-tests/support/source/ros_platform_sdk/pipeline_doosan.py',
             '--root', root, '--phase', a.phase, '--attempt', a.attempt],
             extra={'XML_CATALOG_FILES':'/tmp/tools/ros-schema-xsd/catalog.xml'} if inputs.get('offline_ros_schema') else None,
             deadline=2100)
        while actor['process'].poll() is None:
            owner.observe()
            time.sleep(.05)
        actor['row']['exit'] = actor['process'].wait()
        assert actor['row']['exit'] == 0, owner.text(actor, 'stderr')[-4000:]
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256'], path
        report['all_admitted_source_and_tool_pins_preserved'] = True
        report['status'] = 'PASS'
        print('DOOSAN_PHASE_PASS', a.phase, a.attempt, flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        owner.close()
        owner.save()


if __name__ == '__main__':
    main()
