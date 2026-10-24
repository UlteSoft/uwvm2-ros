#!/usr/bin/env python3
"""Fresh CPython/WASI Python producers and exact two-backend UWVM refusal results.

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
    controls = Path(__file__).resolve().parent
    helper = module('reviewed_python_owner', controls / 'native_owner.py')
    budget = module('reviewed_python_budget', controls / 'cgroup_budget.py')

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

        def observe(self):
            # Clang's representation of the complete frozen byte arrays is
            # larger than the small-app compiler lane. Keep this explicit
            # allowance exclusive to the serial producer, never ROS actors.
            assert self.guard() < 63 << 30, 'shared 1GiB running margin'
            total, building = 0, False
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
                building |= actor['row']['label'].endswith('-build')
                actor['row']['max_sampled_owned_rss'] = max(actor['row']['max_sampled_owned_rss'], used)
                assert time.monotonic() - actor['started'] < actor['row']['deadline_seconds'], 'actor deadline'
                assert sum((actor['directory'] / name).stat().st_size for name in ('stdout', 'stderr')) <= 8 << 20, 'output bound'
            assert total <= (2 << 30 if building else 768 << 20), 'explicit producer/runtime owned RSS bound'

    report = {'status': 'RUNNING', 'scope': 'Fresh Python CPython ROS1/ROS2 Wasms, complete validation/all-import checks and exact default/argument refusals; no AOT/native/performance credit',
              'inputs': inputs, 'artifacts': [], 'default_authority': [],
              'producer_owned_rss_limit_bytes': 2 << 30, 'runtime_owned_rss_limit_bytes': 768 << 20}
    owner = CachedOwner(root, inputs, report, 'python-producer-results.json')

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
            label = 'python-ros' + str(backend)
            output = root / 'out' / (label + '-producer')
            argv = [sys.executable, '-I', '-S', '-B', root / 'source/sdk/origin/examples/python_telemetry/build.py',
                '--backend', str(backend), '--products', inputs['products'],
                '--toolchain-receipt', inputs['toolchain_receipt'],
                '--toolchain-receipt-sha256', inputs['toolchain_receipt_sha256'],
                '--validator', inputs['validator'], '--out', output]
            actor = owner.launch(label + '-build', argv, deadline=240)
            finish(actor)
            built = json.loads((output / 'build.json').read_bytes())
            assert built['status'] == 'BUILT_AND_VALIDATED_NOT_ROS_EXECUTED'
            assert built['source_and_tool_inputs_preserved']
            assert built['runtime_model'] == 'CPython interpreter inside Wasm; no host Python application'
            artifact = dict(built['artifact'], language='python', backend=backend, fresh_compiler_product=True)
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
        for link in inputs.get('tool_links', []):
            assert Path(link['path']).readlink().as_posix() == link['target']
        report['source_and_tool_pins_preserved'] = True
        report['status'] = 'ACTUAL_PYTHON_TWO_BACKEND_PRODUCER_AND_REFUSAL_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        owner.close()


if __name__ == '__main__':
    main()
