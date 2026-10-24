#!/usr/bin/env python3
"""Review actual package ownership and unresolved original interface references.

This source inspection does not infer a native type identity or approve a
transport. Every candidate remains tied to its own repository and branch;
conflicting or missing types are reported rather than renamed or guessed.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import xml.etree.ElementTree as ET

SCALARS = {'bool', 'byte', 'char', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32',
           'int64', 'uint64', 'float32', 'float64', 'string', 'wstring', 'time', 'duration'}


def fields(body, kind, version=None):
    roles = {'msg': [''], 'srv': ['request.', 'response.'], 'action': ['goal.', 'result.', 'feedback.']}[kind]
    sections = re.split(r'^\s*---\s*(?:#[^\n]*)?$', body.decode(), flags=re.MULTILINE)
    if version == 1 and kind == 'srv':
        request, response = [], []
        target = request
        for line in body.decode().split('\n'):
            if line.startswith('---'):
                target = response
            else:
                target.append(line + '\n')
        sections = [''.join(request), ''.join(response)]
    elif version == 1 and kind == 'action':
        pieces = [[]]
        for line in body.decode().split('\n'):
            if line.startswith('---'):
                pieces.append([])
            else:
                pieces[-1].append(line + '\n')
        sections = [''.join(piece) for piece in pieces]
    assert len(sections) == len(roles), (kind, body[:200])
    for role, section in zip(roles, sections):
        for original in section.splitlines():
            line = original.split('#', 1)[0].strip()
            if not line or re.match(r'^\w+\s+\w+\s*=', line):
                continue
            match = re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:<=([0-9]+))?(?:\[(<=)?([0-9]*)\])?\s+([A-Za-z][A-Za-z0-9_]*)(?:\s+.*)?', line)
            assert match, original
            native, string_bound, bounded, extent, member = match.groups()
            array = extent if '[' in line.split()[0] else None
            yield role, native, array, member, string_bound, bool(bounded)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    args = parser.parse_args()
    inputs = json.loads((args.root / 'inputs.json').read_bytes())
    capture = Path(inputs['remaining_capture'])
    report = dict(status='RUNNING', profiles=[], default_authority=[], original_official_cases_run=0)
    args.out.mkdir(mode=0o700)
    try:
        for lock_path in sorted((capture / 'upstream').glob('*/*/source-lock.json')):
            lock = json.loads(lock_path.read_bytes())
            base = lock_path.parent / 'files'
            packages = {}
            for row in lock['files']:
                body = (base / row['path']).read_bytes()
                assert len(body) == row['bytes'] and hashlib.sha256(body).hexdigest() == row['sha256']
                if lock.get('source_kind') == 'ORIGINAL_VENDOR_ZIP_MEMBERS':
                    assert row['source_kind'] == 'ORIGINAL_VENDOR_ZIP_MEMBER' and type(row['crc32']) is int
                elif lock.get('source_kind') == 'AUTHORED_OPERATOR_NATIVE_BRIDGE_V1':
                    assert row['source_kind'] == 'AUTHORED_OPERATOR_NATIVE_BRIDGE_V1' and lock['default_authority'] == []
                else:
                    assert hashlib.sha1(b'blob ' + str(len(body)).encode() + b'\0' + body).hexdigest() == row['git_blob']
                if PurePosixPath(row['path']).name != 'package.xml':
                    continue
                package = ET.fromstring(body)
                tools = [p.text for p in package.findall('buildtool_depend')]
                versions = {1 if p == 'catkin' else 2 for p in tools if p in ('catkin', 'ament_cmake', 'ament_python')}
                assert len(versions) <= 1
                packages[str(PurePosixPath(row['path']).parent)] = dict(name=package.findtext('name'),
                    xml_version=package.findtext('version'), ros_version=next(iter(versions), None),
                    package_xml=row['path'], buildtools=tools)
            declarations, other = [], []
            for row in lock['files']:
                p = PurePosixPath(row['path'])
                if p.suffix not in ('.msg', '.srv', '.action', '.idl', '.proto'):
                    continue
                if p.suffix in ('.idl', '.proto'):
                    other.append(row)
                    continue
                parents = [str(v) for v in p.parents if str(v) in packages]
                assert parents, ('No original package owner', lock['repository'], row['path'])
                package_root = parents[0]
                package = packages[package_root]
                assert package['ros_version'] in (1, 2), (row['path'], package)
                kind = p.suffix[1:]
                typ = package['name'] + ('/' + kind if package['ros_version'] == 2 else '') + '/' + p.stem
                refs = []
                error = None
                try:
                    for role, native, array, member, string_bound, bounded in fields((base / p).read_bytes(), kind, package['ros_version']):
                        if native not in SCALARS:
                            refs.append('std_msgs/Header' if native == 'Header' else native if '/' in native else package['name'] + '/' + native)
                except AssertionError as failure:
                    error = repr(failure)
                declarations.append(dict(row, type=typ, kind=kind, package=package['name'],
                    package_root=package_root, ros_version=package['ros_version'], dependencies=sorted(set(refs)),
                    original_parse_error=error))
            key = lock_path.parent.parent.name + '_' + lock['id']
            row = dict(id=key, source_key=lock_path.parent.parent.name, branch=lock['branch'],
                       commit=lock['commit'], tree=lock['tree'], repository=lock['repository'],
                       packages=packages, declarations=declarations, other_interface_sources=other,
                       source_proof=lock.get('source_proof'))
            report['profiles'].append(row)
            print(json.dumps(dict(profile=key, packages=len(packages), ros_interfaces=len(declarations),
                                  other_interface_sources=len(other))), flush=True)
        report['status'] = 'EXACT_SOURCE_PACKAGE_OWNERSHIP_REVIEWED_NOT_NATIVE_OR_GENERATION_ACCEPTANCE'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (args.out / 'review.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
