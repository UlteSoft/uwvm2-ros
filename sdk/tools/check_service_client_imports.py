#!/usr/bin/env python3
"""Inspect the explicitly selected optional client extension; never execute Wasm.

The existing check_wasm_imports.check contract and original ABI inventories are
unchanged. This separate entry admits exactly its selected optional namespace
and any original business-v1 imports, plus explicit non-ROS module allowances.
Full Wasm validation, actual Runtime/Guest/native ROS tests remain separate.
"""
import argparse
import hashlib
import json
from pathlib import Path
from check_wasm_imports import inspect

SDK=Path(__file__).resolve().parents[1]

def check(data,backend,allowed_modules=(),require_all=False):
    """Check actual binary numeric tuples; refuse unknown/wrong-backend ROS."""
    if backend not in ('1','2','dual'):raise ValueError('explicit backend required')
    selected=(1,2) if backend=='dual' else (int(backend),)
    original=json.loads((SDK/'abi/abi-v1.json').read_text())
    extension=json.loads((SDK/'abi/service-client-v1.json').read_text())
    expected={}
    optional=set()
    for number in selected:
        old=f'uwvm_ros{number}_v1';new=f'uwvm_ros{number}_client_v1'
        for row in original['modules'][old]['functions']:
            expected[old,row['name']]=([p['type'] for p in row['params']],row['results'])
        for row in extension['modules'][new]['functions']:
            key=new,row['name'];optional.add(key);expected[key]=(row['params'],row['results'])
    found=set();rows=inspect(data)
    for row in rows:
        module=row['module'];key=module,row['name']
        if module.startswith('uwvm_ros'):
            if row['kind']!=0 or key not in expected:raise ValueError('unknown/wrong-backend ROS import: '+'.'.join(key))
            if (row['params'],row['results'])!=expected[key]:raise ValueError('wrong actual Wasm signature: '+'.'.join(key))
            if key in found:raise ValueError('duplicate backend-qualified ROS import')
            found.add(key)
        elif module not in allowed_modules:raise ValueError('non-ROS module requires explicit allowance: '+module)
    actual=found&optional
    if not actual:raise ValueError('no selected optional client import')
    if require_all and actual!=optional:raise ValueError('incomplete optional client inventory')
    return {'status':'OPTIONAL_SERVICE_CLIENT_BINARY_IMPORT_CONTRACT_PASS',
            'scope':'Actual binary signatures only; no full validation, Guest execution, ROS or authority credit',
            'backend':backend,'sha256':hashlib.sha256(data).hexdigest(),'optional_functions_checked':len(actual),
            'original_functions_checked':len(found-optional),'required_complete_inventory':require_all,'imports':rows}

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('wasm',type=Path)
    parser.add_argument('--backend',choices=('1','2','dual'),required=True)
    parser.add_argument('--allow-module',action='append',default=[]);parser.add_argument('--require-all',action='store_true')
    parser.add_argument('--report',type=Path);args=parser.parse_args()
    if args.wasm.stat().st_size>64*1024*1024:parser.error('inspection limit is64MiB')
    result=check(args.wasm.read_bytes(),args.backend,args.allow_module,args.require_all)
    if args.report:
        with args.report.open('x') as output:output.write(json.dumps(result,indent=2)+'\n')
    print('PASS optional client binary signatures:',result['optional_functions_checked'])
if __name__=='__main__':main()
