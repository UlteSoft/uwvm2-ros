#!/usr/bin/env python3
"""Review imports of an already validated, local value-contract Wasm program.

The SDK's binary checker deliberately requires a ROS import for ABI coverage.
These local contracts may dead-strip every ROS import. That case receives a
separate value-only result and never ABI or transport coverage. If a ROS import
is present, the unmodified genuine checker must verify its backend/signature.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('wasm',type=Path)
    parser.add_argument('--checker',type=Path,required=True)
    parser.add_argument('--report',type=Path,required=True)
    args=parser.parse_args()
    assert args.wasm.stat().st_size<=64*1024*1024
    data=args.wasm.read_bytes()
    spec=importlib.util.spec_from_file_location('genuine_sdk_import_checker',args.checker)
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    rows=module.inspect(data)
    ros=any(row['module'].startswith('uwvm_ros') for row in rows)
    if ros:
        result=module.check(data,'2',('wasi_snapshot_preview1',))
    else:
        assert all(row['module']=='wasi_snapshot_preview1' and row['kind']==0 for row in rows),rows
        result=dict(status='BINARY_LOCAL_VALUE_IMPORTS_PASS',
            scope='Pure value-contract imports only; zero ROS ABI coverage; no transport qualification',
            sha256=hashlib.sha256(data).hexdigest(),ROS_functions_checked=0,imports=rows)
    result['full_validator_required_separately']=True
    args.report.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status']+' ROS_functions_checked='+str(result['ROS_functions_checked']))


if __name__=='__main__':main()
