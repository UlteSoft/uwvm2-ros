#!/usr/bin/env python3
"""Selected complete source facade regression, never native ROS qualification."""

import importlib.util
import json
import os
from pathlib import Path
import re
import subprocess
import sys

original, include, emitter, output = map(Path, sys.argv[1:5])
output.mkdir()
baseline = Path(sys.argv[5]) if len(sys.argv) > 5 else include
canonical_c = original / 'test/0073.ros-tests/support/source/sdk-tests/c'
fixture_root = Path(sys.argv[6]) if len(sys.argv) > 6 else canonical_c
if not (fixture_root / 'mock_backend.h').is_file():
    raise ValueError(
        'Canonical C fixtures are required; pass an explicit fixture root for a frozen historical archive'
    )
assert os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') == '1'
spec = importlib.util.spec_from_file_location('facade_cpp', emitter)
cpp = importlib.util.module_from_spec(spec)
spec.loader.exec_module(cpp)
rows = []
commands = []
headers = []
for file in sorted((original / 'sdk/include/platform').rglob('*.hpp')):
    if file.name in ('bounded_records.hpp', 'bounded_interfaces.hpp'):
        continue
    relative = file.relative_to(original / 'sdk/include')
    body, note = cpp.transform(file, 'include/' + relative.as_posix(), file.read_bytes())
    if note is None:
        continue
    target = include / relative
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_bytes(body)
    headers.append(relative)
# Consume the actual canonical C prerequisites. A failed old-standard compile
# must report a real SDK gap rather than quietly patch another owner's body.
for relative in (
    'platform/unitree/c/roswasm_unitree/crc.h',
    'platform/unitree/c/roswasm_unitree/sport.h',
    'platform/ros/c/custom-types/generated-r3/fixed_records.h',
):
    source = original / 'sdk/include' / relative
    if not source.is_file():
        raise ValueError('Missing canonical C prerequisite: ' + str(source))
    destination = include / relative
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(source.read_bytes())


def run(label, argv):
    stdout = output / (label + '.stdout')
    stderr = output / (label + '.stderr')
    with stdout.open('wb') as out, stderr.open('wb') as err:
        r = subprocess.run(list(map(str, argv)), stdout=out, stderr=err, timeout=120, check=False)
    commands.append(
        dict(
            label=label,
            argv=list(map(str, argv)),
            exit_code=r.returncode,
            stdout=stdout.name,
            stderr=stderr.name,
        )
    )
    return r.returncode == 0


def record(layer, label, standard, good, **facts):
    row = dict(
        layer=layer, label=label, standard=standard, status='PASS' if good else 'FAIL', **facts
    )
    rows.append(row)
    print(json.dumps(row), flush=True)


inc = ['-I' + str(include), '-I' + str(baseline)] + [
    '-I' + str(root / 'platform' / p / l)
    for root in (include, baseline)
    for p in ('ros', 'ros1', 'ros2')
    for l in ('cpp', 'c')
]


def selection(header):
    platform = header.parts[1]
    extra = []
    if header.name not in ('state.hpp', 'io.hpp', 'scout.hpp'):
        return extra
    cfile = baseline / 'platform' / platform / 'c' / header.with_suffix('.h').name
    if not cfile.is_file():
        return extra
    text = cfile.read_text()
    branch = re.search(r'^#(?:if|elif)\s+(UWVM_\w+)\s*==\s*(\d+)', text, re.M)
    defined = False
    if branch is None:
        branch = re.search(r'^#(?:if|elif)\s+defined\((UWVM_\w+)\)', text, re.M)
        defined = True
    assert branch, 'explicit selected source profile required: ' + str(header)
    macro = branch.group(1)
    value = '1' if defined else branch.group(2)
    extra += ['-D' + macro + '=' + value]
    end = re.search(r'^#(?:elif|else|endif)\b', text[branch.end() :], re.M)
    body = text[branch.end() : branch.end() + end.start()] if end else text[branch.end() :]
    names = set(re.findall(r'\broswasmc_bounded_[12]_\w+', body))
    names = {re.sub(r'_(?:valid|decode|encode|default)$', '', n) for n in names}
    all_required = bool(names)
    version = re.search(r'^#define\s+\w*_ROS_VERSION\s+([12])\b', body, re.M)
    if not names:
        names = set(re.findall(r'\broswasmc_bounded_[12]_\w+', text))
        names = {re.sub(r'_(?:valid|decode|encode|default)$', '', n) for n in names}
    families = []
    for family in sorted((baseline / 'platform' / platform / 'c').rglob('bounded_records.h')):
        records = set(re.findall(r'typedef struct (roswasmc_bounded_\w+)', family.read_text()))
        if version and ('ros' + version.group(1)) not in family.parts:
            continue
        if names and (names <= records if all_required else bool(names & records)):
            families.append(family)
        elif header.name == 'scout.hpp' and ('ros' + value) in family.parts:
            families.append(family)
    assert families, 'no complete selected field family for ' + str(header)
    family = families[0]
    relative = family.parent.relative_to(baseline)
    parts = list(relative.parts)
    parts[2] = 'cpp'
    extra += ['-I' + str(family.parent), '-I' + str(baseline / Path(*parts))]
    return extra


modes = [
    (98, 'c++98'),
    (3, 'c++03'),
    (11, 'c++11'),
    (14, 'c++14'),
    (17, 'c++17'),
    (20, 'c++20'),
    (23, 'c++23'),
    (26, 'c++2c'),
]
for selector, mode in modes:
    flags = [
        '/usr/bin/clang++-22',
        '-std=' + mode,
        '-DROSWASM_CPP_STANDARD=' + str(selector),
        '-pedantic-errors',
    ] + inc
    for i, header in enumerate(headers):
        name = str(selector) + '-header-' + str(i)
        file = output / (name + '.cpp')
        prefix = ''
        if header.name == 'binding_ros2.hpp':
            platform = header.parts[1]
            family = next(
                (baseline / 'platform' / platform / 'cpp/ros2').glob('*/bounded_records.hpp')
            ).relative_to(baseline)
            prefix = (
                '#define UWVM_INDUSTRIAL_GENERATED_RECORDS <'
                + family.as_posix()
                + '>\n#define UWVM_INDUSTRIAL_GENERATED_INTERFACES <'
                + family.with_name('bounded_interfaces.hpp').as_posix()
                + '>\n'
            )
        file.write_text(prefix + '#include <' + header.as_posix() + '>\nint main(){return 0;}\n')
        good = run(name, flags + selection(header) + ['-fsyntax-only', file])
        record(
            'native-header',
            header.as_posix(),
            selector,
            good,
            scope='one explicit complete selected source family per facade',
        )
    # Actual generated complete Go/HG fields instantiate every CRC adapter copy.
    for family in ('go', 'hg'):
        name = str(selector) + '-crc-' + family
        file = output / (name + '.cpp')
        exe = output / name
        type_name = 'roswasmc_bounded_2_unitree_' + family + '_msg_LowCmd'
        text = (
            '#include <platform/unitree/cpp/ros2/unitree-'
            + family
            + '-complete/bounded_records.hpp>\n#include <platform/unitree/cpp/roswasm_unitree/crc.hpp>\n#include <cassert>\n'
        )
        text += (
            'int main(){'
            + type_name
            + ' value={0};value.crc=UINT32_C(0x12345678);value.motor_cmd[0].q=1.25F;'
        )
        text += (
            'assert(!roswasm_unitree::set_'
            + family
            + '_lowcmd_crc(value));uint32_t crc=value.crc;assert(crc!=UINT32_C(0x12345678));'
        )
        text += 'value.motor_cmd[0].reserve[0]=1;' if family == 'go' else 'value.reserve[1]=1;'
        text += (
            'assert(roswasm_unitree::set_'
            + family
            + '_lowcmd_crc(value)==22&&value.crc==crc);return 0;}\n'
        )
        file.write_text(text)
        good = run(name + '-compile', flags + [file, '-o', exe])
        good = run(name + '-run', [exe]) if good else False
        record('native-crc-model', family, selector, good, physical=False)
    for platform in ('fanuc', 'kuka', 'yaskawa', 'abb'):
        name = str(selector) + '-trajectory-' + platform
        file = output / (name + '.cpp')
        exe = output / name
        family = next(
            (baseline / 'platform' / platform / 'cpp/ros2').glob('*/bounded_records.hpp')
        ).relative_to(baseline)
        lifecycle = (
            'platform/'
            + platform
            + '/cpp/adapter/robots/industrial/include/trajectory_lifecycle.hpp'
        )
        text = (
            '#include <'
            + family.with_name('bounded_interfaces.hpp').as_posix()
            + '>\n#include <'
            + lifecycle
            + '>\n'
        )
        text += '#include "' + str(fixture_root / 'mock_backend.h') + '"\n#include <cassert>\n'
        client = 'uwvm_custom::Ros2Control_msgsActionFollowJointTrajectoryClient'
        text += (
            'template class uwvm_industrial::Trajectory<'
            + client
            + '>;\nint main(){roswasmc_context c={model_backend()};roswasmc_action_aliases a={{"s",1},{"o",1},{"f",1},{"r",1},{0,0}};'
        )
        text += (
            'uwvm_industrial::Trajectory<'
            + client
            + '> value(c,a);assert(value.error()==13);value.close();return 0;}\n'
        )
        file.write_text(text)
        good = run(name + '-compile', flags + [file, '-o', exe])
        good = run(name + '-run', [exe]) if good else False
        record('native-trajectory-model', platform, selector, good, actual_guest=False)
    name = str(selector) + '-topic-rpc'
    file = output / (name + '.cpp')
    exe = output / name
    file.write_text(
        '#include <platform/unitree/cpp/roswasm_unitree/topic_rpc.hpp>\n#include <cassert>\nint main(){roswasm_unitree::TopicRequests value;int64_t id=0;assert(!value.begin_next(1001,2,20,id)&&id==1);assert(!value.accept(id,1001,3));assert(value.begin(id,1001,4,30)==17);value.close();assert(value.begin_next(1001,4,30,id)==9);return 0;}\n'
    )
    good = run(name + '-compile', flags + [file, '-o', exe])
    good = run(name + '-run', [exe]) if good else False
    record(
        'native-topic-correlation-model', 'once-only-local-ids', selector, good, actual_guest=False
    )
result = dict(
    status='PASS' if all(r['status'] == 'PASS' for r in rows) else 'FAIL',
    headers=len(headers),
    cases=rows,
    commands=commands,
    native_ros=0,
    actual_guest=0,
    upstream_original=0,
)
(output / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
sys.exit(result['status'] != 'PASS')
