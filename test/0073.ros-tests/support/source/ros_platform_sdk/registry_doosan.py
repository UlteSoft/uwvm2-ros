#!/usr/bin/env python3
"""Exercise the actual compiled ROS registry ceiling and xmake configuration.

No synthetic native identity or accepted robot adapter is supplied. Native C++
checks use empty production registries and explicit count boundaries. Actual
xmake interprets the unchanged production option module; the build printer
produces argv data only. No full Runtime binary or ROS deployment is inferred.
"""
import argparse
import hashlib
import json
from pathlib import Path
import os
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', commands=[], default_authority=[],
                  original_official_cases_run=0, native_robot_delivery=False)
    def save():
        (out / 'registry.json').write_text(json.dumps(report, indent=2) + '\n')
    def run(label, argv, expected=0, env=None):
        directory = out / label
        directory.mkdir()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=source,
                env=dict(os.environ, **(env or {})), stdout=stdout, stderr=stderr,
                timeout=120, check=False)
        streams = {}
        for name in ('stdout', 'stderr'):
            body = (directory / name).read_bytes()
            assert len(body) <= 8 << 20
            streams[name] = dict(bytes=len(body), sha256=hashlib.sha256(body).hexdigest())
        report['commands'].append(dict(label=label, argv=list(map(str, argv)),
            exit=result.returncode, expected_exit=expected, **streams))
        save()
        assert (result.returncode != 0 if expected is None else result.returncode == expected), (label, result.returncode, (directory / 'stderr').read_text()[-2000:])
        return (directory / 'stdout').read_bytes(), (directory / 'stderr').read_bytes()
    try:
        fixture = source / 'test/0073.ros-tests/support/source/ros_platform_sdk/registry_capacity.cc'
        for label, value in [('default', None), ('minimum', 1), ('doosan', 160), ('paired', 256), ('maximum', 512)]:
            artifact = out / ('capacity-' + label)
            flags = [] if value is None else ['-DUWVM_ROS_MAX_CUSTOM_INTERFACES=' + str(value)]
            run(label + '-compile', ['/usr/bin/clang++-21', '-std=c++2c', '-O1',
                '-fconstexpr-steps=8388608', '-I' + str(source / 'src'),
                '-DUWVM_TEST_EXPECTED_REGISTRY_CAPACITY=' + str(value or 128),
                *flags, fixture, '-o', artifact])
            run(label + '-execute', [artifact])
        for label, value in [('zero', 0), ('negative', -1), ('oversize', 513)]:
            _, error = run(label + '-compile-refusal', ['/usr/bin/clang++-21', '-std=c++2c',
                '-I' + str(source / 'src'), '-DUWVM_ROS_MAX_CUSTOM_INTERFACES=' + str(value),
                '-DUWVM_TEST_EXPECTED_REGISTRY_CAPACITY=' + str(value), fixture,
                '-o', out / ('refused-' + label)], expected=1)
            assert b'explicit compiled ROS interface capacity must be 1..512' in error
        xmake = Path(inputs['xmake'])
        assert xmake.is_file()
        production = (source / 'xmake/ros_vendors.lua').read_bytes()
        cases = [('default', None, 128), ('one', '1', 1), ('doosan', '160', 160),
                 ('paired', '256', 256), ('maximum', '512', 512),
                 ('zero', '0', None), ('negative', '-1', None), ('oversize', '513', None),
                 ('fraction', '128.5', None), ('text', 'NaN', None), ('boolean', 'true', None)]
        for label, value, expected in cases:
            case = out / ('xmake-project-' + label)
            case.mkdir()
            (case / 'ros_vendors.lua').write_bytes(production)
            (case / 'xmake.lua').write_text('''set_xmakever("2.9.8")
set_project("explicit_ros_registry_capacity_contract")
option("import-ros1", function() set_default(false) end)
option("import-ros2", function() set_default(false) end)
option("ros-custom-registry-header", function() set_default("none") end)
includes("ros_vendors.lua")
target("selection")
  set_kind("phony")
  local registry = uwvm_ros_selected_vendor_registry()
  local capacity, reason = uwvm_ros_registry_max_interfaces()
  on_load(function(target)
    if reason then raise(reason) end
    assert(not registry and not get_config("import-ros1") and not get_config("import-ros2"))
    io.writefile(path.join(os.projectdir(), "observed-limit.txt"), tostring(capacity))
  end)
target_end()
''')
            flags = [] if value is None else ['--ros-registry-max-interfaces=' + value]
            output, error = run('xmake-' + label, [xmake, 'f', '-c', '-P', case, '-p', 'linux', '-a', 'x86_64', *flags],
                expected=0 if expected is not None else None,
                env=dict(XMAKE_ROOT='y', XMAKE_GLOBALDIR=str(out / 'xmake-global')))
            if expected is None:
                assert b'requires an explicit integer 1..512' in output + error
                assert not (case / 'observed-limit.txt').exists()
            else:
                assert (case / 'observed-limit.txt').read_text() == str(expected)
        for label, value in [('default', None), ('paired', '512')]:
            flags = [] if value is None else ['--registry-max-interfaces', value]
            body, _ = run('printer-' + label, [sys.executable, '-I', '-S', '-B',
                source / 'sdk/origin/tools/ros_build_config.py', '--source', source,
                '--build', out / ('printed-build-' + label), '--backend', 'none',
                '--format', 'json', *flags])
            result = json.loads(body)
            assert result['registry_max_interfaces'] == int(value or '128')
            assert '--ros-registry-max-interfaces=' + str(result['registry_max_interfaces']) in result['configure_argv']
            assert result['default_authority'] == [] and result['selected_vendors'] == []
        for label, value in [('zero', '0'), ('oversize', '513'), ('fraction', '128.0'), ('boolean', 'true')]:
            run('printer-refuse-' + label, [sys.executable, '-I', '-S', '-B',
                source / 'sdk/origin/tools/ros_build_config.py', '--source', source,
                '--build', out / ('refused-print-' + label), '--registry-max-interfaces', value], expected=2)
        assert (source / 'xmake/ros_vendors.lua').read_bytes() == production
        run('adjacent-platform-delivery-tools', [sys.executable, '-I', '-S', '-B',
            source / 'test/0073.ros-tests/support/source/ros_build_options/test_platform_delivery_tools.py',
            '--source', source, '--out', out / 'adjacent-source-regression'])
        report.update(status='EXPLICIT_COMPILED_REGISTRY_CAPACITY_AND_REAL_XMAKE_BOUNDARIES_PASS',
                      default_capacity=128, maximum_capacity=512,
                      source_schema_counts_only=True, byte_node_identity_and_permission_checks_unchanged=True,
                      full_runtime_or_native_adapter_qualified=False)
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
