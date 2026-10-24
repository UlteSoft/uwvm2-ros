#!/usr/bin/env python3
"""Independently read actual DOBOT command/product/refusal/QEMU evidence.

This verifier executes no compiler, Wasm, QEMU or ROS operation. It checks raw
closed run data and file contents; native controller/official/performance credit
remains zero. Run it under the same finite Linux test owner before retention.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def evidence(path):
    return json.loads(path.read_bytes())


def check_pin(pin):
    p = Path(pin['path'])
    assert p.is_file() and p.stat().st_size == pin['bytes'] and sha(p) == pin['sha256'], p


def check_memory(path):
    """Independently inspect the actual core memory declaration, not a receipt."""
    raw = Path(path).read_bytes()
    assert raw[:8] == b'\0asm\1\0\0\0'
    def number(at):
        value = 0
        for n in range(5):
            assert at < len(raw)
            b = raw[at]
            at += 1
            assert n != 4 or b < 16
            value |= (b & 127) << (7*n)
            if b < 128:
                return value, at
        raise AssertionError('overlong core length')
    at, memories = 8, []
    while at < len(raw):
        section = raw[at]
        extent, begin = number(at + 1)
        end = begin + extent
        assert end <= len(raw)
        if section == 5:
            count, cursor = number(begin)
            assert count == 1
            flags, cursor = number(cursor)
            assert flags == 1  # One nonshared 32-bit defined memory with max.
            minimum, cursor = number(cursor)
            maximum, cursor = number(cursor)
            assert cursor == end and minimum == 256 and maximum == 1024
            memories.append((minimum, maximum))
        at = end
    assert memories == [(256, 1024)]


def check_commands(report):
    names = set()
    for row in report['commands']:
        assert row['label'] not in names
        names.add(row['label'])
        assert row['exit'] == row.get('expected_exit', 0), row['label']
        for name in ('stdout', 'stderr'):
            if name in row:
                check_pin(row[name])
    return len(names)


def verify(root, managed, cross):
    root = Path(root).resolve(strict=True)
    inputs = evidence(root / 'inputs.json')
    source = Path(inputs['source_root'])
    owners = [('qualify', 'r9'), ('finish', 'r11'), ('managed', managed), ('cross', cross)]
    rss = {}
    for phase, attempt in owners:
        owner = evidence(root / 'out' / ('dobot-' + phase + '-' + attempt + '.json'))
        assert owner['status'] == 'DOBOT_' + phase.upper() + '_PASS'
        assert owner['all_admitted_source_and_tool_pins_preserved']
        for key in ('events_before', 'events_after'):
            events = dict(line.split() for line in owner[key].splitlines())
            assert all(events[k] == '0' for k in ('oom', 'oom_kill', 'oom_group_kill'))
        assert all(r['exit'] == 0 and r['forced_signal'] is None for r in owner['commands'])
        rss[phase] = max(r['max_sampled_owned_rss'] for r in owner['commands'])
        assert rss[phase] <= owner['owned_aggregate_rss_limit_bytes'] == 1536 << 20
    direct = evidence(root / 'out/qualification-r9/qualification.json')
    assert direct['status'] == 'DOBOT_C_CPP_COMPLETE_DATA_AND_DEFAULT_REFUSAL_PASS'
    assert check_commands(direct) == 279
    assert sum(r['complete_records'] for r in direct['profiles']) == 638
    assert sum(r['scalar_or_string_field_instances'] for r in direct['profiles']) == 1272
    assert len(direct['artifacts']) == 54
    for pin in direct['artifacts']:
        check_pin(pin)
        check_memory(pin['path'])
    finish = evidence(root / 'out/finish-r11/finish.json')
    assert finish['status'] == 'DOBOT_EXACT_SOURCE_PROFILE_CMAKE_AND_REPRODUCTION_PASS'
    assert check_commands(finish) == 74
    assert finish['accepted_profiles'] == 3 and finish['wrong_domain_refusals'] == 6 and finish['default_off_confirmed']
    managed_report = evidence(root / 'out' / ('managed-qualification-' + managed) / 'managed.json')
    assert managed_report['status'] == 'DOBOT_FOUR_MANAGED_LANGUAGE_PRODUCTS_AND_EXACT_REFUSALS_PASS'
    check_commands(managed_report)
    assert len(managed_report['artifacts']) == 12
    for pin in managed_report['artifacts']:
        check_pin(pin)
        check_memory(pin['path'])
    denied = [r for r in managed_report['commands'] if '-denied-' in r['label']]
    assert len(denied) == 24
    alias = b'dobot.observation'
    for row in denied:
        if 'javascript' in row['label']:
            assert row['exit'] == 0
            assert Path(row['stdout']['path']).read_bytes() == struct.pack('<II', 13, 0)
        else:
            assert row['exit'] == 13 and Path(row['stdout']['path']).read_bytes() == b''
        assert Path(row['stderr']['path']).read_bytes() == b''
        if 'javascript' in row['label'] or 'assemblyscript' in row['label']:
            mode = int(row['label'].rsplit('-', 1)[1])
            data = bytes([mode, len(alias)]) + alias
            assert row['stdin_bytes'] == len(data) and row['stdin_sha256'] == hashlib.sha256(data).hexdigest()
    cross_report = evidence(root / 'out' / ('cross-' + cross) / 'cross.json')
    assert cross_report['status'] == 'DOBOT_FOUR_ARCH_USER_QEMU_COMPLETE_DATA_CONTRACTS_PASS'
    assert check_commands(cross_report) == 216
    machines = {'x86_64': (2, 62), 'i686': (1, 3), 'aarch64': (2, 183), 'armhf': (1, 40)}
    assert {r['architecture'] for r in cross_report['targets']} == set(machines)
    for row in cross_report['targets']:
        assert row['contracts'] == 6 and row['independent_vector_programs'] == 21 and len(row['artifacts']) == 27
        check_pin(row['emulator'])
        for pin in row['artifacts']:
            check_pin(pin)
            with Path(pin['path']).open('rb') as f:
                header = f.read(64)
            assert header[:4] == b'\x7fELF' and header[5] == 1
            assert (header[4], int.from_bytes(header[18:20], 'little')) == machines[row['architecture']]
    # Every production C/C++ vendor body must correspond to both earlier data
    # lanes. Test function framing and later compiler recipes are separate.
    for name in ['include/state.h', 'include/state.hpp', 'examples/entry.h', 'examples/guest.c', 'examples/guest.cc']:
        p = source / 'sdk/origin/robots/arms/dobot' / name
        for prior in ('source-r9', 'source-r11'):
            assert p.read_bytes() == (root / prior / 'sdk/origin/robots/arms/dobot' / name).read_bytes()
    for profile in ('ros1_v3', 'ros1_v4', 'ros2_v4'):
        family = 'dobot-' + profile.replace('_', '-') + '-complete'
        p = source / 'sdk/origin/custom-types' / family
        gen = evidence(p / 'generation.vendor.json')
        assert gen['default_authority'] == gen['implicit_permissions'] == [] and not gen['native_registry_emitted']
        for name, pin in gen['outputs'].items():
            assert (p / name).stat().st_size == pin['bytes'] and sha(p / name) == pin['sha256']
            for prior in ('source-r9', 'source-r11'):
                assert (p / name).read_bytes() == (root / prior / 'sdk/origin/custom-types' / family / name).read_bytes()
    return dict(status='INDEPENDENT_DOBOT_SOURCE_PRODUCTS_REFUSALS_AND_FOUR_ARCH_DATA_PASS',
        original_definitions=322, complete_records=638, nonzero_scalar_or_string_instances=1272,
        direct_commands=279, configuration_commands=74, managed_commands=len(managed_report['commands']),
        managed_products=12, managed_refusals=24, qemu_program_runs=108,
        qemu_architectures=list(machines), sampled_owned_peak_rss_bytes=rss,
        runtime=dict(bytes=Path(inputs['runtime']).stat().st_size, sha256=sha(inputs['runtime'])),
        default_authority=[], original_official_cases_run=0, native_robot_delivery=False,
        scope='Closed authored source/SDK/data/default-refusal qualification only; no native driver/controller, original official-suite, cross-host Runtime or hardware-counter performance result')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--managed', required=True)
    parser.add_argument('--cross', required=True)
    args = parser.parse_args()
    print(json.dumps(verify(args.root, args.managed, args.cross), indent=2))
