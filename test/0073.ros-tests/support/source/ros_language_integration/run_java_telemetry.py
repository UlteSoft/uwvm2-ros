#!/usr/bin/env python3
"""Fresh two-backend Java producers and exact UWVM refusal outcomes on Linux.

The owner requires an explicitly prepared bounded root and actual surviving
inputs. Its new cache-aware admission profile is recorded in every report;
strict-current historical tests are unchanged. Native ROS delivery is separate.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import time


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    controls = root / 'source/test/0073.ros-tests/support/source/ros_language_integration'
    helper = module('reviewed_java_owner', controls / 'native_owner.py')
    budget = module('reviewed_java_budget', controls / 'cgroup_budget.py')

    class JavaOwner(helper.Owner):
        def guard(self):
            super().guard()  # Exact boot/group/hard/swap identity and OOM check.
            current = budget.sample(self.group)
            accounting = self.report.setdefault('memory_accounting', {})
            accounting.setdefault('first', current)
            accounting['last'] = current
            for name, key in [('highest_admission', 'admission_bytes'), ('highest_charged', 'charged_bytes')]:
                if name not in accounting or current[key] > accounting[name][key]:
                    accounting[name] = current
            return current['admission_bytes']

    report = {'status': 'RUNNING', 'scope': 'Fresh Java ROS1/ROS2 producer, complete validation/import checks and exact default/argument Runtime refusals; no native/performance credit',
              'inputs': inputs, 'artifacts': [], 'default_authority': []}
    owner = JavaOwner(root, inputs, report, 'java-producer-results.json')

    def finish(actor, expected=0):
        while actor['process'].poll() is None:
            owner.observe()
            time.sleep(0.02)
        actor['row']['exit'] = actor['process'].wait()
        actor['row']['expected_exit'] = expected
        actor['row']['finished_monotonic_ns'] = time.monotonic_ns()
        owner.save()
        assert actor['row']['exit'] == expected, (actor['row']['label'], actor['row']['exit'], owner.text(actor, 'stderr')[-2000:])

    try:
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256'], str(path)
        for backend in (1, 2):
            owner.admit()
            label = 'java-ros' + str(backend)
            output = root / 'out' / (label + '-producer')
            argv = [sys.executable, '-I', '-S', '-B', root / 'source/sdk/origin/examples/java_telemetry/build.py',
                '--backend', str(backend), '--java-home', inputs['java_home'], '--maven', inputs['maven'],
                '--m2', inputs['m2'], '--toolchain-receipt', inputs['toolchain_receipt'],
                '--toolchain-receipt-sha256', inputs['toolchain_receipt_sha256'],
                '--wasm-tools', inputs['validator'], '--out', output]
            actor = owner.launch(label + '-build', argv, deadline=240)
            finish(actor)
            built = json.loads((output / 'build.json').read_bytes())
            assert built['status'] == 'BUILT_AND_VALIDATED_NOT_ROS_EXECUTED'
            assert built['tools_unchanged'] and built['sources_unchanged'] and built['m2_inputs_unchanged']
            artifact = dict(built['artifact'], language='java', backend=backend, fresh_compiler_product=True)
            path = Path(artifact['path'])
            assert path.stat().st_size == artifact['bytes'] and helper.sha(path) == artifact['sha256']
            report['artifacts'].append(artifact)
            for suffix, arguments, expected, stdout in [
                ('default-denial', ['deny'], 0, 'PASS_ROS_DEFAULT_DENIED\n'),
                ('argument-unknown', ['invalid'], 2, ''),
                ('argument-extra', ['deny', 'extra'], 2, '')]:
                owner.admit()
                actor = owner.launch(label + '-' + suffix,
                    [inputs['runtime'], '-Rint', '--wasip1-global-noinherit-system-environment', '--run', path, *arguments])
                finish(actor, expected)
                assert owner.text(actor) == stdout and owner.text(actor, 'stderr') == ''
            owner.save()
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256'], str(path)
        report['source_and_tool_pins_preserved'] = True
        report['status'] = 'ACTUAL_JAVA_TWO_BACKEND_PRODUCER_AND_REFUSAL_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report['status'] = 'FAILED_RETAINED'
        report['error'] = repr(error)
        raise
    finally:
        owner.close()


if __name__ == '__main__':
    main()
