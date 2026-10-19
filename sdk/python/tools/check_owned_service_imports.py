#!/usr/bin/env python3
"""Closed final-Wasm full-signature import check, without instantiation.

The entry/interpreter/stdlib require keeper admission before startup. Only the
inspector helper is captured here. No module wildcard, local import or pyc is
used. Binary validity and Native/Guest execution remain separate requirements.
"""
import argparse
from collections import Counter
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types

RAW = 'uwvm_ros2_rmw_srv_v1'
RAW_SHA = '9e743ad0077cfab6d96bd9ca37d4d23a8804d9c2593af70678b6406222b9ac18'
LEGACY_SHA = '0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0'
INSPECTOR_SHA = '327fccbd6bdcfae0ae8da593014f4fb4451747b1ee1c9991996242b798c9bfa4'
SIGNATURES = [('abi_version', []), ('client_activate', ['i64','i64','i32','i64']),
 ('service_activate', ['i64','i64','i32','i64']), ('endpoint_inspect', ['i64','i64','i32','i64']),
 ('client_submit', ['i64','i64','i32','i64','i64','i64']), ('client_abandon', ['i64','i64','i64']),
 ('service_respond', ['i64','i64','i64','i64','i32','i64','i64']),
 ('endpoint_peek', ['i64','i64','i32','i32','i64','i64']),
 ('endpoint_commit', ['i64','i64','i64','i64']), ('endpoint_cancel', ['i64','i64','i64','i64']),
 ('client_available', ['i64','i64','i64','i64','i32','i64']), ('endpoint_retire', ['i64','i64'])]


def capture(path, limit, expected=None):
    path = Path(path)
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise ValueError('resolved absolute source/binary required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= limit:
            raise ValueError('bounded ordinary source/binary required')
        parts, size = [], 0
        while True:
            part = os.read(fd, min(65536, limit - size + 1))
            if not part:
                break
            parts.append(part)
            size += len(part)
            if size > limit:
                raise ValueError('file exceeded bound')
        last, named = os.fstat(fd), os.lstat(path)
        keys = ('st_dev','st_ino','st_mode','st_size','st_mtime_ns','st_ctime_ns','st_nlink')
        if size != first.st_size or any(getattr(first,k) != getattr(last,k) or getattr(first,k) != getattr(named,k) for k in keys):
            raise ValueError('captured file changed')
        body = b''.join(parts)
        if expected is not None and hashlib.sha256(body).hexdigest() != expected:
            raise ValueError('captured file SHA differs')
        return body
    finally:
        os.close(fd)


def strict(raw):
    def pairs(rows):
        out = {}
        for key, value in rows:
            if key in out:
                raise ValueError('duplicate JSON key')
            out[key] = value
        return out
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: (_ for _ in ()).throw(ValueError('nonfinite JSON')))


def signature(row):
    if (type(row) is not dict or set(row) != {'module','name','kind','params','results'}
            or type(row['module']) is not str or not row['module'] or type(row['name']) is not str or not row['name']
            or type(row['kind']) is not int or row['kind'] != 0
            or type(row['params']) is not list or type(row['results']) is not list
            or any(value not in ('i32','i64','f32','f64') for value in row['params'] + row['results'])):
        raise ValueError('complete exact function signature required')
    return row['module'], row['name'], row['kind'], tuple(row['params']), tuple(row['results'])


def compare(actual, expected):
    left, right = Counter(signature(row) for row in actual), Counter(signature(row) for row in expected)
    if any(value != 1 for value in left.values()) or any(value != 1 for value in right.values()):
        raise ValueError('duplicate actual/expected import')
    if len({(row['module'],row['name']) for row in actual}) != len(actual) or left != right:
        raise ValueError('missing, unexpected, wrong-kind or wrong-full-signature import')


def run(root, binary, backend, enabled, startup, startup_sha):
    if type(enabled) is not bool or type(backend) is not int or backend not in (1,2) or enabled and backend != 2:
        raise ValueError('explicit default-off/backend2 optional selection required')
    legacy = strict(capture(root/'sdk/abi/abi-v1.json', 2*1024*1024, LEGACY_SHA))
    rows = [{'module':'uwvm_ros'+str(backend)+'_v1','name':row['name'],'kind':0,
             'params':[p['type'] for p in row['params']], 'results':row['results']}
            for row in legacy['modules']['uwvm_ros'+str(backend)+'_v1']['functions']]
    if len(rows) != (38 if backend == 1 else 43):
        raise ValueError('complete original backend inventory required')
    if enabled:
        catalog = strict(capture(root/'sdk/abi/rmw-service-v1.json', 65536, RAW_SHA))
        functions = [{'name':name,'params':params,'results':['i32']} for name, params in SIGNATURES]
        if catalog['modules'] != {RAW:{'backend':2,'function_count':12,'functions':functions}}:
            raise ValueError('complete exact raw12 catalog required')
        for key in ('authority_source','implementation_source','raw_header'):
            pin = catalog[key]
            capture(root/pin['path'], 2*1024*1024, pin['sha256'])
        rows += [{'module':RAW,'name':row['name'],'kind':0,'params':row['params'],'results':row['results']} for row in functions]
    closure = strict(capture(startup, 65536, startup_sha))
    if type(closure) is not dict or set(closure) != {'format','imports'} or closure['format'] != 'python-wasi-startup-full-imports-v1' or type(closure['imports']) is not list or len(closure['imports']) > 128:
        raise ValueError('separately reviewed SHA-paired finite startup/generic signature manifest required')
    for row in closure['imports']:
        signature(row)
        if row['module'].startswith('uwvm_ros'):
            raise ValueError('startup/generic declaration cannot bypass reserved ROS namespaces')
    rows += closure['imports']
    helper_path = root/'sdk/tools/check_wasm_imports.py'
    helper_raw = capture(helper_path, 1024*1024, INSPECTOR_SHA)
    module = types.ModuleType('_roswasm_python_exact_inspector')
    module.__file__, module.__package__, module.__cached__ = str(helper_path), '', None
    # Only stdlib imports exist in this exact pinned helper. No ordinary module
    # alias is consulted and no candidate path is reopened by SourceFileLoader.
    exec(compile(helper_raw,str(helper_path),'exec',dont_inherit=True), module.__dict__)
    binary_raw = capture(binary, 64*1024*1024)
    actual = module.inspect(binary_raw)
    compare(actual, rows)
    # Reopen to reject source/binary drift; this is not external interpreter or
    # entry admission, a full validator, grant proof or execution evidence.
    capture(helper_path,1024*1024,INSPECTOR_SHA)
    capture(startup,65536,startup_sha)
    capture(binary,64*1024*1024,hashlib.sha256(binary_raw).hexdigest())
    return {'status':'BINARY_FULL_IMPORT_MULTISET_PASS','backend':backend,'optional_owned_service':enabled,
            'binary_sha256':hashlib.sha256(binary_raw).hexdigest(),'imports':actual,
            'startup_manifest_sha256':startup_sha,'inspector_sha256':INSPECTOR_SHA,
            'scope':'import numeric contract only; independent full validation/Guest/native/DBG/performance pending'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--wasm',type=Path,required=True)
    p.add_argument('--backend',type=int,choices=(1,2),required=True)
    p.add_argument('--owned-service',type=int,choices=(0,1),default=0)
    p.add_argument('--startup-imports',type=Path,required=True);p.add_argument('--startup-sha256',required=True)
    a=p.parse_args()
    print(json.dumps(run(a.source_root,a.wasm,a.backend,bool(a.owned_service),a.startup_imports,a.startup_sha256),indent=2))


if __name__ == '__main__':
    main()
