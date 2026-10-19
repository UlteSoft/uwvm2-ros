#!/usr/bin/env python3
"""Inspect owned-RMW publisher imports without executing or validating Wasm.

Only the selected five-function namespace, original ROS2 business imports, and
explicitly allowed non-ROS modules are admitted. This binary inventory proves
neither native ownership nor policy/ROS/performance behavior. Use the independent
full Wasm validator and real Runtime/native witnesses as separate checks.
"""
import argparse
import hashlib
import json
import os
import stat
from pathlib import Path
from check_wasm_imports import inspect

SDK=Path(__file__).resolve().parents[1]

def check(data,allowed_modules=(),require_all=False):
    original=json.loads((SDK/'abi/abi-v1.json').read_text())
    extension=json.loads((SDK/'abi/rmw-publisher-v1.json').read_text())
    expected={}
    for row in original['modules']['uwvm_ros2_v1']['functions']:
        expected['uwvm_ros2_v1',row['name']]=([p['type']for p in row['params']],row['results'])
    optional=set()
    for row in extension['modules']['uwvm_ros2_rmw_v1']['functions']:
        key='uwvm_ros2_rmw_v1',row['name'];optional.add(key);expected[key]=(row['params'],row['results'])
    found=set();rows=inspect(data)
    for row in rows:
        module=row['module'];key=module,row['name']
        if module.startswith('uwvm_ros'):
            if row['kind']!=0 or key not in expected:raise ValueError('unknown/wrong-backend ROS import: '+'.'.join(key))
            if(row['params'],row['results'])!=expected[key]:raise ValueError('wrong actual Wasm signature: '+'.'.join(key))
            if key in found:raise ValueError('duplicate backend-qualified ROS import')
            found.add(key)
        elif module not in allowed_modules:raise ValueError('non-ROS module requires explicit allowance: '+module)
    actual=found&optional
    if not actual:raise ValueError('no selected owned-RMW publisher import')
    if require_all and actual!=optional:raise ValueError('incomplete owned-RMW publisher import inventory')
    return dict(status='OWNED_RMW_PUBLISHER_BINARY_IMPORT_CONTRACT_PASS',
        scope='Actual binary signatures only; not full validation, execution, authority or native ROS evidence',
        sha256=hashlib.sha256(data).hexdigest(),optional_functions_checked=len(actual),
        original_functions_checked=len(found-optional),required_complete_inventory=require_all,imports=rows)

def load_wasm(path,limit=64*1024*1024):
    """Read one regular file through one owned descriptor under an exact bound.

    The path may change after open: it never selects another file for this read.
    Reject growth/shrinkage or metadata changes instead of accepting a torn
    artifact. This is an input snapshot check, not signature authentication.
    """
    flags=os.O_RDONLY|os.O_NONBLOCK|getattr(os,'O_CLOEXEC',0)|getattr(os,'O_NOFOLLOW',0)
    descriptor=os.open(path,flags)
    try:
        before=os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode):raise ValueError('Wasm input must be a regular file')
        if before.st_size>limit:raise ValueError('inspection limit is64MiB')
        chunks=[];total=0
        while True:
            # At most one extra byte distinguishes EOF from growth. Never use
            # an unbounded read even if the opened file changes concurrently.
            chunk=os.read(descriptor,min(1024*1024,before.st_size-total+1))
            if not chunk:break
            total+=len(chunk)
            if total>before.st_size:raise ValueError('Wasm input extent changed during read')
            chunks.append(chunk)
        after=os.fstat(descriptor)
        identity=lambda value:(value.st_dev,value.st_ino,value.st_mode,value.st_size,value.st_mtime_ns,value.st_ctime_ns)
        if total!=before.st_size or identity(before)!=identity(after):
            raise ValueError('Wasm input changed during bounded read')
        return b''.join(chunks)
    finally:
        os.close(descriptor)

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('wasm',type=Path)
    parser.add_argument('--allow-module',action='append',default=[]);parser.add_argument('--require-all',action='store_true')
    parser.add_argument('--report',type=Path);args=parser.parse_args()
    try:data=load_wasm(args.wasm)
    except (OSError,ValueError)as error:parser.error(str(error))
    result=check(data,args.allow_module,args.require_all)
    if args.report:
        with args.report.open('x')as output:output.write(json.dumps(result,indent=2)+'\n')
    print('PASS owned-RMW publisher binary signatures:',result['optional_functions_checked'])
if __name__=='__main__':main()
