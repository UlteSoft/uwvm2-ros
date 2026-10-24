#!/usr/bin/env python3
"""Fresh linear-Wasm MoonBit producers and exact two-backend UWVM refusal results.

Prepare a fresh bounded root with explicit source/tool/Runtime pins. This owner
uses the recorded cache-aware profile inside the original hard64GiB/swap-zero
group. Native ROS delivery, AOT and performance remain separate acceptance.
"""
import argparse
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
    source_root = Path(inputs.get('source_root', root / 'source')).resolve(strict=True)
    controls = root / 'source/test/0073.ros-tests/support/source/ros_language_integration'
    helper = module('reviewed_moonbit_owner', controls / 'native_owner.py')
    budget = module('reviewed_moonbit_budget', controls / 'cgroup_budget.py')

    class CachedOwner(helper.Owner):
        def guard(self):
            super().guard()
            current = budget.sample(self.group)
            accounting = self.report.setdefault('memory_accounting', {})
            accounting.setdefault('first', current)
            accounting['last'] = current
            for name, key in [('highest_admission', 'admission_bytes'), ('highest_charged', 'charged_bytes')]:
                if current[key] > accounting.get(name, {}).get(key, -1):
                    accounting[name] = current
            return current['admission_bytes']

        def admit(self):
            end = time.monotonic() + 60
            while self.guard() >= 60 << 30:
                assert time.monotonic() < end, '4GiB conservative admission margin unavailable'
                time.sleep(0.5)

    report = {'status': 'RUNNING', 'scope': 'Fresh MoonBit linear-Wasm ROS1/ROS2 Wasms, complete validation/all-import checks and exact default refusals; argv is not read, no native/performance credit',
              'inputs': inputs, 'artifacts': [], 'default_authority': []}
    owner = CachedOwner(root, inputs, report, 'moonbit-producer-results.json')

    def finish(actor, expected=0):
        while actor['process'].poll() is None:
            owner.observe()
            time.sleep(0.02)
        actor['row']['exit'] = actor['process'].wait()
        actor['row']['expected_exit'] = expected
        actor['row']['finished_monotonic_ns'] = time.monotonic_ns()
        owner.save()
        assert actor['row']['exit'] == expected, (actor['row']['label'], actor['row']['exit'], owner.text(actor, 'stderr')[-2200:])

    try:
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256'], str(path)
        for link in inputs.get('tool_links', []):
            assert Path(link['path']).readlink().as_posix() == link['target']
        for backend in (1, 2):
            owner.admit()
            label = 'moonbit-ros' + str(backend)
            output = root / 'out' / (label + '-producer')
            argv = [sys.executable, '-I', '-S', '-B', source_root / 'sdk/origin/examples/moonbit_telemetry/build.py',
                '--backend', str(backend), '--moon-home', inputs['moon_home'], '--validator', inputs['validator'], '--out', output]
            actor = owner.launch(label + '-build', argv, deadline=240)
            finish(actor)
            built = json.loads((output / 'build.json').read_bytes())
            assert built['status'] == 'BUILT_AND_VALIDATED_NOT_ROS_EXECUTED'
            assert built['source_and_tool_inputs_preserved']
            assert built['runtime_model'] == 'Native MoonBit linear Wasm; no Host language runtime'
            artifact = dict(built['artifact'], language='moonbit', backend=backend, fresh_compiler_product=True)
            path = Path(artifact['path'])
            assert path.stat().st_size == artifact['bytes'] and helper.sha(path) == artifact['sha256']
            report['artifacts'].append(artifact)
            for suffix, arguments, expected, stdout in [
                ('default-denial', [], 0, 'PASS_ROS_DEFAULT_DENIED\n')]:
                owner.admit()
                actor = owner.launch(label + '-' + suffix,
                    [inputs['runtime'], '-Rint', '--wasip1-global-noinherit-system-environment', '--run', path, *arguments])
                finish(actor, expected)
                assert owner.text(actor) == stdout and owner.text(actor, 'stderr') == ''
            owner.save()
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256'], str(path)
        for link in inputs.get('tool_links', []):
            assert Path(link['path']).readlink().as_posix() == link['target']
        report['source_and_tool_pins_preserved'] = True
        report['status'] = 'ACTUAL_MOONBIT_TWO_BACKEND_PRODUCER_AND_REFUSAL_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        owner.close()


if __name__ == '__main__':
    main()
