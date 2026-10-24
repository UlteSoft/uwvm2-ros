#!/usr/bin/env python3
"""Deliver public SDK documents and relative links as inert, checked file I/O.

Export requires the original Linux cgroup. Receive does not import the SDK,
compile code, run tests or execute transported content. Existing foreign bytes
and links are refused. All intermediate ordinary files stay under /tmp.
"""
import argparse
import base64
import hashlib
import json
import os
from pathlib import Path
import shutil
import sys
import tempfile


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        while data := stream.read(1 << 20):
            h.update(data)
    return h.hexdigest()


def name_ok(name):
    p = Path(name)
    return (not p.is_absolute() and '..' not in p.parts and '\\' not in name
            and len(p.parts) >= 3 and p.parts[0] == 'sdk'
            and p.parts[1] in ('language', 'platform')
            and all(ord(c) >= 32 for c in name))


def parents_ok(path, source):
    for parent in path.parents:
        if parent == source:
            return
        assert not parent.is_symlink(), parent
    raise AssertionError('Destination escaped source root')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode', choices=('export', 'receive'))
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--receipt', type=Path, required=True)
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    assert not args.receipt.exists()
    if args.mode == 'export':
        inputs = json.loads((source.parent / 'inputs.json').read_bytes())
        group = Path(inputs['cgroup']['path'])
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
        assert [group.stat().st_dev, group.stat().st_ino] == inputs['cgroup']['identity']
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
        assert (group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (group / 'memory.swap.max').read_text().strip() == '0'
        acceptance = inputs.get('public_layout_acceptance','out/layout-r4/layout.json')
        accepted = json.loads((source.parent / acceptance).read_bytes())
        assert accepted['status'] in ('PUBLIC_SDK_LAYOUT_AND_TEN_REAL_WASM_PRODUCTS_PASS',
            'PYTHON_VENDOR_SDK_SOURCE_VIEWS_AND_NATIVE_VALUE_ACCEPTANCE_PASS',
            'UNITREE_OFFICIAL_ENTRY_REVIEW_AND_COMPLETE_SPORT_PAYLOAD_SOURCE_VIEWS_PASS',
            'COMPLETE_UNITREE_API_PYTHON_RECORDS_SPORT_AND_ASYNC_SOURCE_VIEWS_PASS')
        rows = []
        for top in ('language', 'platform'):
            for base, dirs, files in os.walk(source/'sdk/origin' / top, followlinks=False):
                names = files + [n for n in dirs if (Path(base) / n).is_symlink()]
                for name in sorted(names):
                    path = Path(base) / name
                    row = dict(path=path.relative_to(source).as_posix())
                    assert name_ok(row['path'])
                    if path.is_symlink():
                        row.update(kind='relative-link', target=os.readlink(path))
                        assert not Path(row['target']).is_absolute()
                        assert path.resolve(strict=True).is_relative_to(source/'sdk/origin')
                    else:
                        assert path.is_file() and path.stat().st_size <= 16 << 20
                        row.update(kind='ordinary-source', bytes=path.stat().st_size, sha256=sha(path))
                    rows.append(row)
        rows.sort(key=lambda row: row['path'])
        previous = None
        if inputs.get('python_sdk_layout_previous'):
            previous=json.loads((source.parent/inputs['python_sdk_layout_previous']).read_bytes())
            assert previous['kind']=='checked-public-sdk-view-source-state-v1'and previous['default_authority']==[]
        header = dict(kind='public-sdk-layout-inert-source-delivery-v1', default_authority=[], rows=rows,
                      previous=previous)
        encoded = json.dumps(header, sort_keys=True).encode()
        sys.stdout.buffer.write(encoded + b'\n')
        for row in rows:
            if row['kind'] != 'ordinary-source':
                continue
            data = (source / row['path']).read_bytes()
            assert len(data) == row['bytes'] and hashlib.sha256(data).hexdigest() == row['sha256']
            sys.stdout.buffer.write(base64.b64encode(data) + b'\n')
        sys.stdout.buffer.flush()
    else:
        encoded = sys.stdin.buffer.readline((4 << 20) + 1).rstrip(b'\n')
        assert len(encoded) <= 4 << 20
        header = json.loads(encoded)
        assert header['kind'] == 'public-sdk-layout-inert-source-delivery-v1' and header['default_authority'] == []
        rows = header['rows']
        assert 0 < len(rows) <= 10000 and len({row['path'] for row in rows}) == len(rows)
        total = sum(row.get('bytes', 0) for row in rows)
        assert total <= 32 << 20
        changed=set(); pending=set(); initial={}
        previous=header.get('previous')or dict(files={})
        if header.get('previous')is not None:
            assert previous['kind']=='checked-public-sdk-view-source-state-v1'and previous['default_authority']==[]
        for row in rows:
            assert name_ok(row['path'])
            path = source / row['path']
            parents_ok(path, source)
            if row['kind'] == 'relative-link':
                assert not Path(row['target']).is_absolute() and all(ord(c) >= 32 for c in row['target'])
                assert (path.parent / row['target']).resolve().is_relative_to(source/'sdk/origin')
                assert not os.path.lexists(path) or path.is_symlink() and os.readlink(path) == row['target']
            else:
                assert row['kind'] == 'ordinary-source' and 0 <= row['bytes'] <= 16 << 20
                assert not path.is_symlink()
                if path.exists():
                    assert path.is_file()
                    if path.stat().st_size!=row['bytes']or sha(path)!=row['sha256']:
                        pin=previous['files'].get(row['path'][4:])
                        assert pin and path.stat().st_size==pin['bytes']and sha(path)==pin['sha256']
                        changed.add(row['path'])
                        pending.add(row['path'])
                    initial[row['path']]=dict(bytes=path.stat().st_size,sha256=sha(path))
                else:
                    pending.add(row['path']); initial[row['path']]=None
        # Every body is still streamed and hashed. Only missing/changed source
        # needs temporary disk storage; identical implementation files are
        # never copied again merely to refresh a few generated documents.
        additional=sum(row.get('bytes',0)for row in rows if row['path']in pending)
        assert shutil.disk_usage(source).free >= additional*2+(128<<20), 'incremental source-delivery disk margin'
        with tempfile.TemporaryDirectory(prefix='uwvm-ros-sdk-layout-', dir='/tmp') as temp:
            staged = Path(temp)
            for index, row in enumerate(rows):
                if row['kind'] != 'ordinary-source':
                    continue
                line = sys.stdin.buffer.readline((24 << 20) + 1)
                assert line.endswith(b'\n') and len(line) <= 24 << 20
                data = base64.b64decode(line[:-1], validate=True)
                assert len(data) == row['bytes'] and hashlib.sha256(data).hexdigest() == row['sha256']
                if row['path']in pending:
                    (staged / str(index)).write_bytes(data)
            assert sys.stdin.buffer.read(1) == b''
            # Refuse a destination changed during transport before the first
            # install. Prior byte ownership never authorizes a new foreign edit.
            for row in rows:
                path=source/row['path']; parents_ok(path,source)
                if row['kind']=='relative-link':
                    assert not os.path.lexists(path)or path.is_symlink()and os.readlink(path)==row['target']
                else:
                    before=initial[row['path']]
                    assert not path.is_symlink()
                    if before is None: assert not path.exists()
                    else: assert path.is_file()and path.stat().st_size==before['bytes']and sha(path)==before['sha256']
            for index, row in enumerate(rows):
                path = source / row['path']
                parents_ok(path, source)
                path.parent.mkdir(parents=True, exist_ok=True)
                if row['kind'] == 'relative-link':
                    if not path.is_symlink():
                        path.symlink_to(row['target'], target_is_directory=True)
                elif not path.exists():
                    with path.open('xb') as stream:
                        stream.write((staged / str(index)).read_bytes())
                elif row['path']in changed:
                    pin=previous['files'][row['path'][4:]]
                    assert not path.is_symlink()and path.stat().st_size==pin['bytes']and sha(path)==pin['sha256']
                    path.write_bytes((staged/str(index)).read_bytes())
        for row in rows:
            path = source / row['path']
            if row['kind'] == 'relative-link':
                assert path.is_symlink() and os.readlink(path) == row['target']
                assert path.resolve(strict=True).is_relative_to(source/'sdk/origin')
            else:
                assert sha(path) == row['sha256']
    receipt = dict(status='INERT_PUBLIC_SDK_SOURCE_DELIVERY_PASS', mode=args.mode,
                   manifest_sha256=hashlib.sha256(encoded).hexdigest(), rows=len(rows),
                   ordinary_files=sum(row['kind'] == 'ordinary-source' for row in rows),
                   relative_links=sum(row['kind'] == 'relative-link' for row in rows),
                   ordinary_bytes=sum(row.get('bytes', 0) for row in rows),
                   default_authority=[], local_compiler_guest_or_sdk_tests_run=0)
    args.receipt.parent.mkdir(parents=True, exist_ok=True)
    with args.receipt.open('x') as stream:
        json.dump(receipt, stream, indent=2)
        stream.write('\n')


if __name__ == '__main__':
    main()
