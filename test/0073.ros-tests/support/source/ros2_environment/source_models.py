#!/usr/bin/env python3
"""Check ROS2 startup source and synthetic enclave selection without native IO.

This reads source files and changes only Python dictionaries. It never compiles
or runs C++, calls setenv/unsetenv, initializes ROS, or starts a Guest/QEMU.
Use a fresh --report path to retain the checked source hashes and model scope.
"""
import sys
sys.dont_write_bytecode = True
import argparse
import hashlib
import itertools
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[5]
REFERENCE = 'https://github.com/ros2/rcl/blob/jazzy/rcl/src/rcl/security.c'


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    if args.report.exists():
        parser.error('report must be fresh')
    checks = 0

    def require(condition, message):
        nonlocal checks
        checks += 1
        if not condition:
            raise AssertionError(message)

    files = ('src/ros/gateway/ros2/environment.h', 'src/ros/gateway/ros2/main.cc',
             'test/0073.ros-tests/support/source/ros2_environment/unit.cc', 'test/0073.ros-tests/support/source/ros2_environment/README.md',
             'test/0073.ros-tests/support/source/ros2_environment/source_models.py')
    original = {name: (ROOT / name).read_bytes() for name in files}
    text = {name: value.decode() for name, value in original.items()}
    helper = text[files[0]]
    source = text[files[1]]
    unit = text[files[2]]
    startup = source[source.index('int main(int argc,char** argv)'):]
    require('#include "environment.h"' in source, 'actual Gateway must include helper')
    require('if(::setenv(name,value,1)!=0)' in helper and
            'if(::unsetenv(name)!=0)' in helper and helper.count('throw std::runtime_error') == 2,
            'each native edit must fail closed')
    require('clear("ROS_SECURITY_ENCLAVE_OVERRIDE");' in helper,
            'clear the actual rcl override name')
    require('::setenv(' not in startup and '::unsetenv(' not in startup,
            'startup must use only checked edits')
    clear = startup.index('gateway::ros2_environment::clear_enclave_override();')
    require(startup.index('RMW has no qualified security adapter') < clear <
            startup.index('auto context{std::make_shared<rclcpp::Context>()}'),
            'validate connection and remove override before context initialization')
    require(startup.count('clear_enclave_override();') == 1, 'one startup removal')
    calls = re.findall(r'gateway::ros2_environment::(set|clear)\("([A-Z_]+)"', startup)
    require(calls == [('set', 'RMW_IMPLEMENTATION'), ('set', 'ROS_DOMAIN_ID'),
                      ('set', 'ROS_SECURITY_ENABLE'), ('set', 'ROS_SECURITY_STRATEGY'),
                      ('set', 'ROS_SECURITY_KEYSTORE'), ('set', 'ROS_SECURITY_ENABLE'),
                      ('clear', 'ROS_SECURITY_STRATEGY'), ('clear', 'ROS_SECURITY_KEYSTORE')],
            'retain exact Enforce and disabled-test environment edits')
    require('args.insert(args.end(),{"--ros-args","--enclave",enclave});' in startup,
            'explicit enclave argument must remain')
    require('rmw!=rmw_get_implementation_identifier()' in startup,
            'retain actual loaded RMW comparison')
    require(startup.index('context->init(') < startup.index('gateway::run(c,'),
            'all startup edits precede IPC serving')
    require(len(re.findall(r'if\(!require\(', unit)) == 13 and 'checks!=13u' in unit,
            'native unit keeps all13 unconditional checks')
    require('PASS C++17 ROS2 checked environment contract checks=' in unit,
            'native unit exposes its own completion marker')

    # Model the documented rcl precedence. This compares synthetic directory
    # names only; it does not assert file existence, signatures or DDS delivery.
    def selected_root(keystore, enclave, override):
        name = override if override else enclave
        return keystore.rstrip('/') + '/enclaves' + ('' if name == '/' else name)

    selection_cases = 0
    for keys, enclave, override in itertools.product(
            ('/test/keys', '/other/test/keys'),
            ('/gateway', '/', '/nested/test', '/gateway2'),
            (None, '', '/witness', '/', '/other')):
        before = selected_root(keys, enclave, override)
        cleared = selected_root(keys, enclave, None)
        expected = keys + '/enclaves' + ('' if enclave == '/' else enclave)
        require(cleared == expected, 'cleared selection must match configured enclave')
        require(bool(override) or before == expected, 'empty override preserves explicit selection')
        if override and override != enclave:
            require(before != cleared, 'nonempty different override changes native root')
        selection_cases += 1

    # Model checked startup sequencing with an injected edit failure at every
    # position. Failures stop admission; rollback of prior edits is not needed
    # because no context or server has been created at that point.
    sequencing_cases = 0
    for mode, override, fail_at in itertools.product(
            ('enforce', 'disabled-test'), (None, '', '/wrong-test-identity'),
            range(8)):
        values = {'ROS_SECURITY_ENCLAVE_OVERRIDE': override,
                  'ROS_SECURITY_ENABLE': 'inherited-test-value',
                  'ROS_SECURITY_STRATEGY': 'inherited-test-value',
                  'ROS_SECURITY_KEYSTORE': '/inherited-test-keys'}
        edits = [('clear', 'ROS_SECURITY_ENCLAVE_OVERRIDE', None),
                 ('set', 'RMW_IMPLEMENTATION', 'rmw_fastrtps_cpp'),
                 ('set', 'ROS_DOMAIN_ID', '74')]
        edits += ([('set', 'ROS_SECURITY_ENABLE', 'true'),
                   ('set', 'ROS_SECURITY_STRATEGY', 'Enforce'),
                   ('set', 'ROS_SECURITY_KEYSTORE', '/test/keys')]
                  if mode == 'enforce' else
                  [('set', 'ROS_SECURITY_ENABLE', 'false'),
                   ('clear', 'ROS_SECURITY_STRATEGY', None),
                   ('clear', 'ROS_SECURITY_KEYSTORE', None)])
        admitted = True
        for index, (operation, name, value) in enumerate(edits):
            if index == fail_at:
                admitted = False
                break
            if operation == 'clear':
                values.pop(name, None)
            else:
                values[name] = value
        require(admitted == (fail_at >= len(edits)), 'failed edit must stop initialization')
        if admitted:
            require('ROS_SECURITY_ENCLAVE_OVERRIDE' not in values, 'no hidden identity override')
            require(values['RMW_IMPLEMENTATION'] == 'rmw_fastrtps_cpp' and
                    values['ROS_DOMAIN_ID'] == '74', 'explicit native connection values')
            require(values['ROS_SECURITY_ENABLE'] == ('true' if mode == 'enforce' else 'false'),
                    'explicit security enable value')
            require((values.get('ROS_SECURITY_STRATEGY'), values.get('ROS_SECURITY_KEYSTORE')) ==
                    (('Enforce', '/test/keys') if mode == 'enforce' else (None, None)),
                    'mode-specific security values')
        sequencing_cases += 1
    require(all((ROOT / name).read_bytes() == value for name, value in original.items()),
            'checked source must remain unchanged')
    report = {'status': 'SOURCE_AND_SYNTHETIC_MODEL_PASS', 'checks': checks,
              'selection_cases': selection_cases, 'sequencing_cases': sequencing_cases,
              'source_files': {name: {'bytes': len(value), 'sha256': hashlib.sha256(value).hexdigest()}
                               for name, value in original.items()},
              'primary_reference': REFERENCE,
              'scope': 'Source call ordering and synthetic dictionary/directory-selection models only',
              'actual_CPP17_compilation': 'NOT_EXECUTED', 'actual_native_environment_calls': 'NOT_EXECUTED',
              'actual_ROS_DDS_Guest_QEMU': 'NOT_EXECUTED', 'original_task_complete': False}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    with args.report.open('x') as output:
        output.write(json.dumps(report, indent=2) + '\n')
    print(json.dumps({key: report[key] for key in ('status', 'checks', 'selection_cases', 'sequencing_cases')}))


if __name__ == '__main__':
    main()
