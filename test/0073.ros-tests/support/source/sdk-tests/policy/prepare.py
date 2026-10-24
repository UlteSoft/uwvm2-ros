#!/usr/bin/env python3
"""Exercise the policy generator CLI without applying its output permissions.

All principal numbers and connection names here are mathematical test inputs.
No Worker, Gateway, ROS process or socket is started. The native companion
checks these produced files with the actual Worker/Gateway policy validators.
Use only the serial /tmp developer runner; outputs must be fresh.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

import json
from pathlib import Path
import subprocess
import sys

SDK = _CHECKOUT_ROOT / 'sdk/origin'
COMMON = ('pub', 'sub', 'trigger-call', 'trigger-advertise', 'trigger-respond',
          'set-bool-call', 'set-bool-advertise', 'set-bool-respond', 'goal-send',
          'goal-feedback', 'goal-result', 'goal-cancel-own', 'goal-cancel-any',
          'fib-advertise', 'fib-respond', 'parameter-get', 'parameter-set',
          'parameter-list', 'graph-topic', 'graph-node', 'graph-service',
          'graph-topics', 'graph-nodes', 'graph-services', 'ros-time', 'log-info',
          'tf-lookup', 'tf-broadcast')
ONLY1 = ('parameter-delete', 'parameter-exists', 'parameter-search', 'wall-time')
ONLY2 = ('parameter-describe', 'parameter-declare', 'parameter-undeclare',
         'parameter-events', 'graph-action', 'system-time', 'steady-time',
         'lifecycle-get', 'lifecycle-change')
out = Path(sys.argv[1]).resolve()
assert out.is_relative_to(Path('/tmp').resolve()) and not out.exists()
out.mkdir()
rows, configs = [], []


def generate(backend, aliases, name, kind='string', refused=False):
    """Run one real CLI, then check selected aliases and the fresh output state."""
    directory = out / name
    argv = [sys.executable, '-B', str(_sdk_path(SDK, 'examples/c/advanced_policy.py')),
            '--backend', str(backend), '--socket', '/tmp/sdk-model-unused.sock',
            '--worker-uid', '1000', '--worker-pid', '12345', '--gateway-uid', '1000',
            '--node-name', 'sdk_gateway', '--namespace', '/uwvm_sdk_relay',
            '--parameter-node', '/uwvm_sdk_relay/sdk_gateway', '--parameter-kind', kind,
            '--out', str(directory)]
    argv += (['--master-uri', 'http://127.0.0.1:11384', '--advertise-host', '127.0.0.1']
             if backend == 1 else ['--domain-id', '205', '--disabled-security-tutorial'])
    for alias in aliases:
        argv += ['--alias', alias]
    result = subprocess.run(argv, capture_output=True, text=True, timeout=15)
    (out / (name + '.log')).write_text(result.stdout + result.stderr)
    assert result.returncode == (2 if refused else 0), (name, result.stderr)
    if refused:
        assert not directory.exists()
    else:
        policy = json.loads((directory / 'policy.json').read_text())
        assert [item['alias'] for item in policy['grants']] == list(aliases)
        assert policy['implicit_rights'] == []
        assert policy['default_authority'] == 'empty except basic stdin/stdout/stderr'
        assert policy['Worker_policy_check_arguments'][-1] == f'--ros{backend}-check-policy'
        configs.append(str(directory / 'gateway.conf'))
    rows.append({'name': name, 'returncode': result.returncode, 'expected_refusal': refused})


for backend, extra, absent in ((1, ONLY1, ONLY2), (2, ONLY2, ONLY1)):
    supported = COMMON + extra
    assert len(supported) == (32 if backend == 1 else 37)
    for alias in supported:
        generate(backend, (alias,), f'ros{backend}-{alias}')
    generate(backend, supported, f'ros{backend}-all-string')
    parameters = tuple(alias for alias in supported if alias.startswith('parameter-'))
    generate(backend, parameters, f'ros{backend}-parameters-float64', 'float64')
    for alias in absent:
        generate(backend, (alias,), f'refuse-ros{backend}-{alias}', refused=True)
generate(2, ('fib-advertise', 'fib-respond'), 'ros2-action-pair')
generate(2, ('lifecycle-get', 'lifecycle-change'), 'ros2-lifecycle-pair')
assert len(configs) == 75 and len(rows) == 88
(out / 'configs.json').write_text(json.dumps(configs, indent=2) + '\n')
(out / 'generator-receipt.json').write_text(json.dumps({
    'status': 'GENERATOR_CLI_MODELS_PASS', 'positive_profiles': 75,
    'individual_aliases': 69, 'version_refusals': 13,
    'actual_ROS_delivery': 'NOT_EXECUTED', 'authority_applied': False,
    'rows': rows}, indent=2) + '\n')
print(json.dumps({'status': 'GENERATOR_CLI_MODELS_PASS', 'profiles': 75, 'refusals': 13}))
