#!/usr/bin/env python3
"""Render explicit caller DATA into a fresh MoonBit entry selection, no grants.

This operator never compiles, links, runs a Guest, loads ROS or edits a policy.
Admit this entry/interpreter/stdlib and the input under the real Linux keeper.
The output is Source requiring independent review and actual compilation.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys


def capture(path, maximum):
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise ValueError('resolved ordinary caller input required')
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= maximum:
            raise ValueError('bounded regular input required')
        blocks, size = [], 0
        while True:
            block = os.read(fd, min(65536, maximum - size + 1))
            if not block:
                break
            size += len(block)
            if size > maximum:
                raise ValueError('caller input exceeded bound')
            blocks.append(block)
        end, named = os.fstat(fd), os.lstat(path)
        for key in ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_nlink'):
            if getattr(first, key) != getattr(end, key) or getattr(first, key) != getattr(named, key):
                raise ValueError('caller input changed')
        if size != first.st_size:
            raise ValueError('short caller input')
        return b''.join(blocks)
    finally:
        os.close(fd)


def pairs(rows):
    output = {}
    for key, value in rows:
        if key in output:
            raise ValueError('duplicate caller JSON key')
        output[key] = value
    return output


def text(value, minimum=0, maximum=255):
    if type(value) is not str or not minimum <= len(value) <= maximum or any(not 32 <= ord(c) <= 126 for c in value):
        raise ValueError('bounded printable ASCII caller text required')
    return json.dumps(value)


def integer(value, minimum, maximum):
    if type(value) is not int or not minimum <= value <= maximum:
        raise ValueError('finite bounded caller integer required')
    return str(value)


def render(row):
    common = {'role', 'source_alias', 'peer_alias', 'availability_alias', 'expected_qos_hex',
              'maximum_body', 'history_budget', 'poll_limit'}
    fields = {'description-client': {'type_name', 'type_hash', 'include_type_sources', 'availability_wait_ns'},
              'parameter-server': {'parameters'}}
    if type(row) is not dict or row.get('role') not in fields or set(row) != common | fields[row['role']]:
        raise ValueError('closed complete caller selection required')
    aliases = [row['source_alias'], row['peer_alias']]
    if row['availability_alias'] is not None:
        aliases.append(row['availability_alias'])
    for alias in aliases:
        text(alias, 1, 255)
    if len(set(aliases)) != len(aliases):
        raise ValueError('independent unique alias names required')
    available = 'None' if row['availability_alias'] is None else 'Some(' + text(row['availability_alias'], 1, 255) + ')'
    qos = row['expected_qos_hex']
    if type(qos) is not list or len(qos) != 2 or any(type(item) is not str or len(item) != 160 or any(c not in '0123456789abcdef' for c in item) for item in qos):
        raise ValueError('two exact caller-observed lowercase QoS80 byte rows required')
    qos_literals = ['b"' + ''.join('\\x' + item[i:i+2] for i in range(0, 160, 2)) + '"' for item in qos]
    lines = ['// Explicit caller DATA; neither a grant nor Native identity discovery.',
             'fn selected() -> @app.Selection { {',
             '  source_alias: ' + text(row['source_alias'], 1, 255) + ',',
             '  peer_alias: ' + text(row['peer_alias'], 1, 255) + ',',
             '  availability_alias: ' + available + ',',
             '  expected_qos: [' + ', '.join(qos_literals) + '],',
             '  maximum_body: ' + integer(row['maximum_body'], 0, 4193626) + ',',
             '  history_budget: ' + integer(row['history_budget'], 1, 65536), ' } }',
             'fn selected_poll_limit() -> Int { ' + integer(row['poll_limit'], 1, 100000) + ' }']
    if row['role'] == 'description-client':
        if type(row['include_type_sources']) is not bool:
            raise ValueError('explicit original request boolean required')
        if row['availability_alias'] is None and row['availability_wait_ns'] != 0:
            raise ValueError('availability wait needs its independent owner')
        lines += ['fn selected_type_name() -> String { ' + text(row['type_name'], 1, 1024) + ' }',
                  'fn selected_type_hash() -> String { ' + text(row['type_hash'], 0, 1024) + ' }',
                  'fn selected_include_sources() -> Bool { ' + str(row['include_type_sources']).lower() + ' }',
                  'fn selected_wait_ns() -> UInt64 { ' + integer(row['availability_wait_ns'], 0, 60000000000) + 'UL }']
    else:
        if row['availability_alias'] is not None or type(row['parameters']) is not list or len(row['parameters']) > 4096:
            raise ValueError('explicit finite local server parameter table required')
        seen = set()
        lines += ['fn resolve_selected_type(name : String) -> Result[Int, Int] {']
        for parameter in row['parameters']:
            if type(parameter) is not dict or set(parameter) != {'name', 'type'}:
                raise ValueError('complete name/type parameter entry required')
            name = text(parameter['name'], 1, 1024)
            if name in seen:
                raise ValueError('duplicate local parameter name')
            seen.add(name)
            lines += ['  if name == ' + name + ' { return Ok(' + integer(parameter['type'], 0, 9) + ') }']
        lines += ['  // Original PARAMETER_NOT_SET, not an implicit Native lookup.', '  Ok(0)', '}']
    return '\n'.join(lines) + '\n'


def main():
    if sys.platform != 'linux' or not sys.flags.isolated or not sys.flags.no_site or not sys.flags.safe_path:
        raise ValueError('externally admitted Linux Python -I -S entry required')
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--selection', type=Path, required=True)
    parser.add_argument('--selection-sha256', required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    raw = capture(args.selection, 2 * 1024 * 1024)
    if hashlib.sha256(raw).hexdigest() != args.selection_sha256:
        raise ValueError('caller selection input SHA differs')
    def nonfinite(_value):
        raise ValueError('finite JSON input required')
    row = json.loads(raw, object_pairs_hook=pairs, parse_constant=nonfinite)
    source = render(row).encode('utf-8')
    if not args.out.is_absolute() or args.out.resolve() != args.out or args.out.exists() or args.out.is_symlink():
        raise ValueError('fresh separate output directory required')
    if capture(args.selection, 2 * 1024 * 1024) != raw:
        raise ValueError('caller selection changed')
    args.out.mkdir(parents=False)
    (args.out / 'selection.mbt').write_bytes(source)
    report = {'status': 'CALLER_SELECTION_SOURCE_ONLY_NOT_EXECUTED', 'role': row['role'],
              'source_input': {'path': str(args.selection), 'bytes': len(raw), 'sha256': args.selection_sha256},
              'output': {'path': 'selection.mbt', 'bytes': len(source), 'sha256': hashlib.sha256(source).hexdigest()},
              'implicit_permissions': [], 'compiler_execution': 'NOT_RUN', 'guest_execution': 'NOT_RUN'}
    (args.out / 'selection-proof.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
