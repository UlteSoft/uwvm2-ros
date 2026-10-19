#!/usr/bin/env python3
"""Inventory the exact optional owned Service imports without execution.

The twelve-function service namespace is required. Original ROS2 business
imports are admitted. Subscription, publisher and client extension namespaces require their
explicit flags, and every non-ROS module requires an explicit allowance. This
checks binary signatures only, not Wasm validity, Runtime execution, permission,
native ownership, metadata completeness or ROS delivery. The separate full
validator and real Runtime/native fixtures remain mandatory.
"""
import argparse
import hashlib
import json
from pathlib import Path
from check_wasm_imports import inspect
from check_rmw_publisher_imports import load_wasm

SDK=Path(__file__).resolve().parents[1]

def check(data,allowed_modules=(),require_all=False,with_publisher=False,with_client=False,with_subscription=False):
    original=json.loads((SDK/'abi/abi-v1.json').read_text())
    expected={}
    for row in original['modules']['uwvm_ros2_v1']['functions']:
        expected['uwvm_ros2_v1',row['name']]=([p['type']for p in row['params']],row['results'])
    selected=[('rmw-service-v1.json','uwvm_ros2_rmw_srv_v1')]
    if with_subscription:selected.append(('rmw-subscription-v1.json','uwvm_ros2_rmw_sub_v1'))
    if with_publisher:selected.append(('rmw-publisher-v1.json','uwvm_ros2_rmw_v1'))
    if with_client:selected.append(('service-client-v1.json','uwvm_ros2_client_v1'))
    required=set()
    for catalog,module in selected:
        rows=json.loads((SDK/'abi'/catalog).read_text())['modules'][module]['functions']
        for row in rows:
            key=module,row['name'];expected[key]=(row['params'],row['results'])
            if module=='uwvm_ros2_rmw_srv_v1':required.add(key)
    found=set();rows=inspect(data)
    for row in rows:
        module=row['module'];key=module,row['name']
        if module.startswith('uwvm_ros'):
            if row['kind']!=0 or key not in expected:raise ValueError('unknown/unselected/wrong-backend ROS import: '+'.'.join(key))
            if(row['params'],row['results'])!=expected[key]:raise ValueError('wrong actual Wasm signature: '+'.'.join(key))
            if key in found:raise ValueError('duplicate backend-qualified ROS import')
            found.add(key)
        elif module not in allowed_modules:raise ValueError('non-ROS module requires explicit allowance: '+module)
    actual=found&required
    if not actual:raise ValueError('no selected owned-RMW service import')
    if require_all and actual!=required:raise ValueError('incomplete owned-RMW service import inventory')
    return dict(status='OWNED_RMW_SERVICE_BINARY_IMPORT_CONTRACT_PASS',
        scope='Binary signatures only; not full validation, execution, authority or native ROS evidence',
        sha256=hashlib.sha256(data).hexdigest(),optional_functions_checked=len(actual),
        other_selected_functions_checked=len(found-required),required_complete_inventory=require_all,
        publisher_selected=with_publisher,client_selected=with_client,subscription_selected=with_subscription,imports=rows)

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('wasm',type=Path)
    parser.add_argument('--allow-module',action='append',default=[]);parser.add_argument('--require-all',action='store_true')
    parser.add_argument('--with-publisher',action='store_true');parser.add_argument('--with-client',action='store_true')
    parser.add_argument('--with-subscription',action='store_true')
    parser.add_argument('--report',type=Path);args=parser.parse_args()
    try:
        data=load_wasm(args.wasm)
        result=check(data,args.allow_module,args.require_all,args.with_publisher,args.with_client,args.with_subscription)
    except (OSError,ValueError)as error:parser.error(str(error))
    if args.report:
        with args.report.open('x')as output:output.write(json.dumps(result,indent=2)+'\n')
    print('PASS owned-RMW service binary signatures:',result['optional_functions_checked'])
if __name__=='__main__':main()
