#!/usr/bin/env python3
"""Actual generator composition/default-bit regressions; synthetic IR, no native ROS claim."""

import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys

source, include, output = map(Path, sys.argv[1:4])
output.mkdir()
literal_source = Path(sys.argv[4]) if len(sys.argv) > 4 else source
canonical_c = source / 'test/0073.ros-tests/support/source/sdk-tests/c'
fixture_root = Path(sys.argv[5]) if len(sys.argv) > 5 else canonical_c
if not (fixture_root / 'mock_backend.h').is_file():
    raise ValueError(
        'Canonical C fixtures are required; pass an explicit fixture root for a frozen historical archive'
    )
formatter = Path(sys.argv[6]) if len(sys.argv) > 6 else None
formatter_config = Path(sys.argv[7]) if len(sys.argv) > 7 else None
formatted = []
assert os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') == '1'


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    value = importlib.util.module_from_spec(spec)
    sys.modules[name] = value
    spec.loader.exec_module(value)
    return value


cpp = module('qa_cpp', source / 'sdk/tools/cpp_profiles.py')
bounded = module('qa_bounded', source / 'sdk/tools/custom-types/bounded_profile.py')
parent = module('qa_parent', source / 'sdk/tools/custom_types.py')
rows = []
commands = []


def run(label, argv, reject=False):
    stdout = output / (label + '.stdout')
    stderr = output / (label + '.stderr')
    with stdout.open('wb') as out, stderr.open('wb') as err:
        result = subprocess.run(
            list(map(str, argv)), stdout=out, stderr=err, timeout=120, check=False
        )
    good = result.returncode != 0 if reject else result.returncode == 0
    if reject:
        good = good and 'Conflicting same-name bounded' in stderr.read_text()
    commands.append(
        dict(
            label=label,
            argv=list(map(str, argv)),
            exit_code=result.returncode,
            matched=good,
            stdout=stdout.name,
            stderr=stderr.name,
        )
    )
    return good


def record(name, value='1.0'):
    # Deliberately non-native IR. These products never enter a Gateway registry.
    return bounded.Record(
        2,
        'qa/msg/' + name,
        [bounded.Field('value', 'float64', native_default=value)],
        None,
        dict(test_fixture=True),
        minimum=8,
        maximum=8,
        codec_identity='TEST_ONLY_NO_NATIVE_AUTHORITY_' + name,
        has_native_defaults=True,
    )


shared = record('Shared')
a = record('A')
b = record('B')
conflicting = record('Shared', '2.0')
constant_conflict = record('Shared')
constant_conflict.constants = [dict(name='MODE', type='uint32', value='7', literal='7')]
request = record('Request')
response = record('Response')
service = bounded.Interface(
    2,
    'qa/srv/Service',
    'service',
    None,
    dict(request=request, response=response),
    codec_identity='TEST_ONLY_NO_NATIVE_SERVICE',
)
for name, records in [
    ('A', [shared, a, request, response]),
    ('B', [shared, b, request, response]),
    ('Conflict', [conflicting]),
    ('ConstantConflict', [constant_conflict]),
]:
    directory = output / name
    directory.mkdir()
    mapping = {(r.version, r.name): r for r in records}
    (directory / 'bounded_records.h').write_text(bounded.emit_c(parent, mapping, records))
    (directory / 'bounded_records.hpp').write_text(bounded.emit_cpp(records))
    interfaces = [] if 'Conflict' in name else [service]
    (directory / 'bounded_interfaces.h').write_text(bounded.emit_c_interfaces(interfaces))
    (directory / 'bounded_interfaces.hpp').write_text(bounded.emit_cpp_interfaces(interfaces))
    if formatter:
        if not formatter_config or not formatter.is_file() or not formatter_config.is_file():
            raise ValueError('An exact formatter and configuration are both required')
        for header in sorted(directory.glob('*.hpp')):
            argv = [str(formatter), '--style=file:' + str(formatter_config), str(header)]
            first = subprocess.run(argv, capture_output=True, timeout=30, check=True).stdout
            header.write_bytes(first)
            second = subprocess.run(argv, capture_output=True, timeout=30, check=True).stdout
            if first != second:
                raise ValueError('Generated C++ formatting is not idempotent')
            unchanged, note = cpp.transform(header, 'include/platform/qa/cpp/' + header.name, first)
            if unchanged != first or note is not None:
                raise ValueError('Documented C++ facade is not transform-idempotent')
            previous = first.replace(b'// roswasm-cpp-documented-facade-v3\n', b'', 1)
            refreshed, note = cpp.transform(
                header, 'include/platform/qa/cpp/' + header.name, previous
            )
            probe = header.with_suffix('.refresh.hpp')
            probe.write_bytes(refreshed)
            refreshed_formatted = subprocess.run(
                [str(formatter), '--style=file:' + str(formatter_config), str(probe)],
                capture_output=True,
                timeout=30,
                check=True,
            ).stdout
            if refreshed_formatted != first:
                raise ValueError(
                    'Previous C++ facade refresh changed its selected profile contract'
                )
            formatted.append(dict(path=str(header), sha256=hashlib.sha256(first).hexdigest()))
inc = [
    '-I' + str(include),
    '-I' + str(include / 'platform/ros/cpp'),
    '-I' + str(include / 'platform/ros/c'),
]
for standard, mode in [
    (98, 'c++98'),
    (3, 'c++03'),
    (11, 'c++11'),
    (14, 'c++14'),
    (17, 'c++17'),
    (20, 'c++20'),
    (23, 'c++23'),
    (26, 'c++2c'),
]:
    flags = [
        '/usr/bin/clang++-22',
        '-std=' + mode,
        '-DROSWASM_CPP_STANDARD=' + str(standard),
        '-pedantic-errors',
    ] + inc
    for combination in [('A',), ('B',), ('A', 'B'), ('A', 'Conflict'), ('A', 'ConstantConflict')]:
        name = str(standard) + '-' + '-'.join(combination)
        program = output / (name + '.cpp')
        executable = output / name
        text = '\n'.join(
            '#include "' + str(output / c / 'bounded_interfaces.hpp') + '"' for c in combination
        )
        text += (
            '\n#include "'
            + str(fixture_root / 'mock_backend.h')
            + '"\n#include <cassert>\nint main(){'
        )
        text += 'uwvm_custom::Ros2QaMsgShared value=roswasmcpp::Codec<uwvm_custom::Ros2QaMsgShared>::default_value();assert(value.value==1.0);'
        text += 'roswasmc_context context={model_backend()};uwvm_custom::Ros2QaSrvServiceClient client(context,"denied");assert(client.error()==13);return 0;}\n'
        program.write_text(text)
        reject = any('Conflict' in value for value in combination)
        good = run(name + '-compile', flags + [program, '-o', executable], reject)
        if good and not reject:
            good = run(name + '-run', [executable])
        rows.append(
            dict(
                kind='synthetic-generator-composition',
                standard=standard,
                selection=list(combination),
                status='PASS' if good else 'FAIL',
                native_identity=False,
            )
        )
# Independent original hexadecimal vs portable constant compilation. Preserve
# original binary32/64 bytes including -0/subnormals; no decimal tolerance.
literals = set()
for header in (literal_source / 'sdk/include/platform').glob('*/c/**/bounded_records.h'):
    for number, suffix in re.findall(
        r'(-?0x[0-9a-fA-F]+(?:\.[0-9a-fA-F]*)?[pP][+-]?[0-9]+)([fF]?)', header.read_text()
    ):
        literals.add((number, 32 if suffix else 64))
assert literals, 'actual retained nonzero floating-default literal denominator required'
for variant in ('original', 'portable'):
    text = '#include <stdio.h>\nint main(){\n'
    for literal, width in sorted(literals):
        value = (
            (literal + ('F' if width == 32 else ''))
            if variant == 'original'
            else cpp.c_float_literal(width, float.fromhex(literal))
        )
        text += (
            '{'
            + ('float' if width == 32 else 'double')
            + ' value='
            + value
            + ';if(fwrite(&value,sizeof value,1,stdout)!=1)return 1;}\n'
        )
    text += 'return 0;}\n'
    program = output / (variant + '.cpp')
    program.write_text(text)
    mode = 'c++20' if variant == 'original' else 'c++98'
    exe = output / (variant + '-constants')
    good = run(
        variant + '-constants-compile',
        ['/usr/bin/clang++-22', '-std=' + mode, '-pedantic-errors', program, '-o', exe],
    )
    if good:
        good = run(variant + '-constants-run', [exe])
    rows.append(
        dict(
            kind='float-constant-compile',
            variant=variant,
            unique_literals=len(literals),
            status='PASS' if good else 'FAIL',
        )
    )
original = output / 'original-constants-run.stdout'
portable = output / 'portable-constants-run.stdout'
equal = original.is_file() and portable.is_file() and original.read_bytes() == portable.read_bytes()
rows.append(
    dict(
        kind='exact-ieee-defaults',
        unique_literals=len(literals),
        status='PASS' if equal else 'FAIL',
    )
)
result = dict(
    status='PASS' if all(r['status'] == 'PASS' for r in rows) else 'FAIL',
    cases=rows,
    commands=commands,
    formatted=formatted,
    native_ros=0,
    actual_guest=0,
    original_upstream_cases=0,
)
(output / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result['cases']), flush=True)
raise SystemExit(0 if result['status'] == 'PASS' else 1)
