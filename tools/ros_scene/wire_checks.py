#!/usr/bin/env python3
"""Qualify canonical provider records from immutable sources in the existing gate."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[2]
GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    inode = GATE.stat().st_ino
    def gate():
        assert Path('/proc/self/cgroup').read_text().strip() == '0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
        assert GATE.stat().st_ino == inode
        assert (GATE/'memory.max').read_text().strip() == '68719476736'
        assert (GATE/'memory.swap.max').read_text().strip() == '0'
    gate()
    output = args.output.resolve()
    assert output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    output.mkdir(parents=True, exist_ok=True)
    assert not (output/'results.json').exists(), 'Use a fresh evidence directory'
    names = ['src/uwvm2/imported/ros/scene/wire.h', 'src/uwvm2/imported/ros/scene/schema_contract.h',
             'test/0084.ros-runtime/wire_security.cpp', 'tools/ros_scene/wire_checks.py',
             'tools/ros_scene/generate_schema_contract.py', 'tools/ros_scene/interface-catalogue.json']
    sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
    pins = {name: sha(ROOT/name) for name in names}
    snapshot = output/'source'
    for name in names:
        path = snapshot/name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes((ROOT/name).read_bytes())
    catalogue = json.loads((snapshot/names[-1]).read_text())
    schemas = catalogue['schemas']
    u64 = lambda value: struct.pack('<Q', value)
    text = lambda value: u64(len(value.encode()))+value.encode()
    def encode(kind):
        if kind in ('u64', 'u32'): return struct.pack('<'+('Q' if kind=='u64' else 'I'), 7)
        if kind == 'i64': return struct.pack('<q', -13)
        if kind == 'f64': return struct.pack('<d', 1.25)
        if kind == 'bool': return u64(1)
        if kind == 'text': return text('robot/café')
        if kind == 'bytes': return u64(3)+b'\x00\x7f\x80'
        if kind.startswith('array<'): return u64(2)+encode(kind[6:-1])*2
        if kind.startswith('optional<'): return u64(1)+encode(kind[9:-1])
        schema = schemas[kind]
        if 'variant' in schema:
            selected = 'not_set' if 'not_set' in schema['variant'] else next(iter(schema['variant']))
            return u64(schema['variant_tags'][selected])+b''.join(encode(child) for _, child in schema['variant'][selected])
        return b''.join(text(schema['enums'][name][0]) if name in schema.get('enums', {}) else encode(child)
                        for name, child in schema['fields'])
    cases = []
    for op in catalogue['operations']:
        if op['wire'] not in ('proposed_record', 'canonical_record'): continue
        for direction in ('request', 'result'):
            data = b''.join(encode(field['type']) for field in op[direction])
            cases.append((op['name'].replace('.', '_')+'__'+direction, data))
    lines = ['#pragma once', '#include <array>', '#include <span>', '#include <cstdint>',
             'namespace wire_fixture {', 'using shape_type=uwvm2::imported::ros::scene::schema_contract::type;',
             'struct record {shape_type shape;std::span<std::uint8_t const> data;};']
    # Avoid relying on the generator implementation for encoded fixture bytes.
    for index, (_, data) in enumerate(cases):
        lines.append('inline constexpr std::array<std::uint8_t,%d> bytes_%d{%s};' %
                     (len(data), index, ','.join(map(str, data))))
    lines += ['inline constexpr std::array<record,%d> cases{{' % len(cases)]
    lines += ['{shape_type::%s,bytes_%d},' % (name, index) for index, (name, _) in enumerate(cases)]
    lines += ['}};', '}', 'inline constexpr auto const& wire_cases=wire_fixture::cases;']
    header = snapshot/'test/0084.ros-runtime/wire_cases.h'
    header.write_text('\n'.join(lines)+'\n')
    report = dict(complete=False, scope='canonical structural codecs only; no provider/authority/VM registration',
                  cgroup=str(GATE), cgroup_inode=inode, boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                  memory_max=68719476736, swap_max=0, events_before=(GATE/'memory.events').read_text(),
                  sources=pins, snapshot_root=str(snapshot), fixture_sha256=sha(header),
                  catalogue_records=len(cases), commands=[], affinity=sorted(os.sched_getaffinity(0)))
    def save(): (output/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    def run(name, argv):
        gate()
        done = subprocess.run(argv, capture_output=True, text=True, timeout=240, cwd=ROOT,
                              env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1'))
        (output/(name+'.stdout')).write_text(done.stdout)
        (output/(name+'.stderr')).write_text(done.stderr)
        report['commands'].append(dict(name=name, argv=argv, exit=done.returncode));save()
        assert done.returncode == 0, done.stderr[-6000:]
        print('PASS', name, flush=True)
        return done.stdout
    try:
        for name, compiler, flags in [('asan', '/usr/bin/clang++-22', ['-fsanitize=address,undefined']),
                                     ('gcc', '/usr/bin/g++-13', [])]:
            executable = output/name
            run(name+'-compile', [compiler, '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
                                  '-fno-omit-frame-pointer', *flags, '-I'+str(snapshot/'src'),
                                  str(snapshot/'test/0084.ros-runtime/wire_security.cpp'), '-o', str(executable)])
            assert 'passed canonical wire safety:' in run(name+'-run', [str(executable)])
        report['complete'] = True
    finally:
        gate()
        report['sources_unchanged'] = all(sha(ROOT/name)==digest for name, digest in pins.items())
        report['snapshot_unchanged'] = all(sha(snapshot/name)==digest for name, digest in pins.items()) and sha(header)==report['fixture_sha256']
        report['events_after'] = (GATE/'memory.events').read_text()
        report['complete'] &= report['sources_unchanged'] and report['snapshot_unchanged']
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
