#!/usr/bin/env python3
"""Closed Java linear-WASI binary inventory. SOURCE-only until Linux execution.
External keeper owns exact entry/interpreter/stdlib/startup and resource guard.
No --allow-module. Captured inspector bytes, not pyc/sys.modules, are executed.
No full code validation, Guest execution, authority or native success is implied.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys
import types
import secrets


def require(condition, message):
    if not condition:
        raise ValueError(message)


def capture(path, maximum, expected=None):
    """Read one bounded resolved regular descriptor and check exact captured bytes."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'resolved absolute file required')
    selected = path.stat(follow_symlinks=False)
    require(stat.S_ISREG(selected.st_mode) and 0 <= selected.st_size <= maximum, 'bounded regular file required')
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    with os.fdopen(fd, 'rb') as stream:
        first = os.fstat(stream.fileno())
        require(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum and
                (first.st_dev, first.st_ino) == (selected.st_dev, selected.st_ino), 'file changed at open')
        data = stream.read(maximum + 1)
        last = os.fstat(stream.fileno())
    final = path.stat(follow_symlinks=False)
    fields = ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
    require(len(data) == first.st_size and len(data) <= maximum and
            all(getattr(first, k) == getattr(last, k) == getattr(final, k) for k in fields), 'file changed during capture')
    digest = hashlib.sha256(data).hexdigest()
    require(expected is None or digest == expected, 'captured source/contract hash differs')
    return data, {'path': str(path), 'bytes': len(data), 'sha256': digest,
                  'stat': {k: str(getattr(first, k)) for k in fields}}


RMW_MODULE = 'uwvm_ros2_rmw_srv_v1'
RMW_SIGNATURES = [
    ('abi_version', []),
    ('client_activate', ['i64', 'i64', 'i32', 'i64']),
    ('service_activate', ['i64', 'i64', 'i32', 'i64']),
    ('endpoint_inspect', ['i64', 'i64', 'i32', 'i64']),
    ('client_submit', ['i64', 'i64', 'i32', 'i64', 'i64', 'i64']),
    ('client_abandon', ['i64', 'i64', 'i64']),
    ('service_respond', ['i64', 'i64', 'i64', 'i64', 'i32', 'i64', 'i64']),
    ('endpoint_peek', ['i64', 'i64', 'i32', 'i32', 'i64', 'i64']),
    ('endpoint_commit', ['i64', 'i64', 'i64', 'i64']),
    ('endpoint_cancel', ['i64', 'i64', 'i64', 'i64']),
    ('client_available', ['i64', 'i64', 'i64', 'i64', 'i32', 'i64']),
    ('endpoint_retire', ['i64', 'i64'])]
RMW_OUTPUT = {
    'activation_inspection_bytes': 256, 'submit_ticket_bytes': 8,
    'availability_bytes': 1,
    'peek': 'one complete UMRC160+URMI192+canonical body checked copy, no separate length write',
    'minimum_peek_capacity': 352, 'maximum_peek_capacity': 4194304,
    'nonzero_error_preserves_output': True, 'ack_abandon_retire_no_output': True}
LEGACY_CATALOG_SHA256 = '0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0'


def rmw_catalog_contract(document):
    """Validate the complete optional12 numeric/output contract as DATA only."""
    fields = {'schema_version', 'extension_version', 'kind', 'source_status',
              'authority_source', 'implementation_source', 'raw_header',
              'original_business_modules_unchanged', 'authority_contract', 'modules',
              'output_contract', 'quota_contract', 'pair_history',
              'transaction_contract', 'legacy_preservation', 'actual_compilers_models_Guest_native_ROS'}
    require(type(document) is dict and set(document) == fields, 'closed owned-RMW service catalog')
    require(type(document['schema_version']) is int and document['schema_version'] == 1 and
            type(document['extension_version']) is int and document['extension_version'] == 1 and
            document['kind'] == 'optional-owned-rmw-service-v1' and
            document['source_status'] == 'SOURCE_ONLY_HELD_UNEXECUTED' and
            document['original_business_modules_unchanged'] is True and
            type(document['actual_compilers_models_Guest_native_ROS']) is int and
            document['actual_compilers_models_Guest_native_ROS'] == 0, 'owned-RMW source-only catalog version')
    paths = {'authority_source': 'src/uwvm2/uwvm/imported/ros/host.h',
             'implementation_source': 'src/uwvm2/uwvm/imported/ros/rmw_service_host.h',
             'raw_header': 'sdk/include/roswasm/rmw_service_v1.h'}
    for key, path in paths.items():
        pin = document[key]
        require(type(pin) is dict and set(pin) == {'path', 'sha256'} and pin['path'] == path and
                isinstance(pin['sha256'], str) and re.fullmatch('[0-9a-f]{64}', pin['sha256']), 'fixed owned-RMW source binding')
    functions = [{'name': name, 'params': params, 'results': ['i32']} for name, params in RMW_SIGNATURES]
    require(document['modules'] == {RMW_MODULE: {'backend': 2, 'function_count': 12, 'functions': functions}},
            'complete ordered12 exact owned-RMW signatures required')
    require(type(document['modules'][RMW_MODULE]['backend']) is int and
            type(document['modules'][RMW_MODULE]['function_count']) is int, 'exact numeric catalog fields')
    require(document['output_contract'] == RMW_OUTPUT and
            all(type(document['output_contract'][k]) is int for k in
                ('activation_inspection_bytes', 'submit_ticket_bytes', 'availability_bytes', 'minimum_peek_capacity', 'maximum_peek_capacity')) and
            document['output_contract']['nonzero_error_preserves_output'] is True and
            document['output_contract']['ack_abandon_retire_no_output'] is True, 'exact owned-RMW output contract')
    require(document['legacy_preservation'] == {'catalog_path': 'sdk/abi/abi-v1.json',
            'catalog_sha256': LEGACY_CATALOG_SHA256}, 'unchanged original business81 catalog required')
    require(isinstance(document['authority_contract'], str) and document['authority_contract'] and
            isinstance(document['pair_history'], str) and document['pair_history'] and
            type(document['quota_contract']) is dict and type(document['transaction_contract']) is dict,
            'catalog authority/lifetime scope required')
    return functions


def strict(raw):
    def pairs(rows):
        out={}
        for k,v in rows:
            require(k not in out,'duplicate JSON key');out[k]=v
        return out
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:(_ for _ in()).throw(ValueError('nonfinite JSON')))


def check(root,wasm,policy_path,policy_sha,optional_path,optional_sha,inspector_sha,client_contract=None):
    """The entire caller-reviewed policy is pinned, never inferred from output.

    Actual business/optional signatures additionally must match fixed registries.
    WASI rows are explicit full name/kind/params/results policy, not a blanket
    namespace allowance. Full validation/linking checks the actual WASI provider.
    This inspector does not claim to authenticate a source binary or deployment.
    """
    require(sys.platform=='linux','future execution only under admitted Linux keeper')
    sources={}
    def admit(path,maximum,expected=None):
        raw,pin=capture(path,maximum,expected);sources[str(path)]=(raw,pin);return raw
    optional_raw=admit(optional_path,2*1024*1024,optional_sha);optional=strict(optional_raw);functions=rmw_catalog_contract(optional)
    for role in ('authority_source','implementation_source','raw_header'):
        row=optional[role];admit(root/row['path'],2*1024*1024,row['sha256'])
    original_raw=admit(root/optional['legacy_preservation']['catalog_path'],2*1024*1024,LEGACY_CATALOG_SHA256);original=strict(original_raw)
    policy_raw=admit(policy_path,2*1024*1024,policy_sha);policy=strict(policy_raw)
    require(type(policy)is dict and set(policy)=={'kind','imports'} and policy['kind']=='java-linear-wasi-exact-imports-v1' and type(policy['imports'])is list and policy['imports'],'nonempty explicit full binary policy required')
    allowed={}
    def add(module,name,params,results):
        key=module,name;require(key not in allowed,'duplicate registry key');allowed[key]={'module':module,'name':name,'kind':0,'params':params,'results':results}
    require(set(original['modules'])=={'uwvm_ros1_v1','uwvm_ros2_v1'} and original['modules']['uwvm_ros1_v1']['function_count']==38 and original['modules']['uwvm_ros2_v1']['function_count']==43,'preserved business81 registry required')
    for f in original['modules']['uwvm_ros2_v1']['functions']:add('uwvm_ros2_v1',f['name'],[p['type']for p in f['params']],f['results'])
    for f in functions:add(RMW_MODULE,f['name'],f['params'],f['results'])
    if client_contract is not None:
        expected_client={'module':'uwvm_ros2_client_v1','functions':[
            {'name':'abi_version','params':[],'results':['i32']},
            {'name':'features','params':['i64','i64','i32','i64'],'results':['i32']},
            {'name':'service_available','params':['i64','i64','i64','i32','i64'],'results':['i32']}]}
        require(client_contract==expected_client,'exact preserved optional3 backend2 contract required')
        for f in client_contract['functions']:add(client_contract['module'],f['name'],f['params'],f['results'])
    wanted={}
    for row in policy['imports']:
        require(type(row)is dict and set(row)=={'module','name','kind','params','results'} and type(row['kind'])is int and row['kind']==0 and type(row['module'])is str and type(row['name'])is str
                and row['module'] and row['name'] and type(row['params'])is list and type(row['results'])is list and all(v in ('i32','i64','f32','f64')for v in row['params']+row['results']),'exact numeric function policy row')
        key=row['module'],row['name'];require(key not in wanted,'duplicate policy import')
        if row['module']=='wasi_snapshot_preview1':
            require(row['name'].isascii() and re.fullmatch('[a-z][a-z0-9_]*',row['name']),'explicit WASI field spelling required')
        else:require(key in allowed and row==allowed[key],'unexpected/wrong-backend ROS module/name/signature')
        wanted[key]=row
    require({k for k in wanted if k[0]==RMW_MODULE}=={(RMW_MODULE,f['name'])for f in functions},'actual owned12 complete inventory required')
    inspector_path=root/'sdk/tools/check_wasm_imports.py';inspector_raw=admit(inspector_path,2*1024*1024,inspector_sha)
    binary=admit(wasm,64*1024*1024)
    # All transitive local sources and policy/data captured before helper exec.
    # inspect() imports only trusted stdlib and does not call its file-reading
    # check()/CLI entry. No ambient local module is accepted or reused.
    name='_uwvm_java_import_inspector_'+secrets.token_hex(16);require(name not in sys.modules,'fresh inspector owner')
    module=types.ModuleType(name);module.__file__=str(inspector_path);module.__package__='';module.__loader__=module.__spec__=module.__cached__=None;sys.modules[name]=module
    try:
        exec(compile(inspector_raw,str(inspector_path),'exec',dont_inherit=True),module.__dict__)
        require(sys.modules.get(name)is module,'inspector module owner changed');rows=module.inspect(binary)
    finally:
        if sys.modules.get(name)is module:del sys.modules[name]
    found={}
    for row in rows:
        require(type(row)is dict and set(row)=={'module','name','kind','params','results'},'nonfunction actual import refused')
        key=row['module'],row['name'];require(key not in found and key in wanted and row==wanted[key],'duplicate/unexpected/wrong actual full signature');found[key]=row
    require(found==wanted,'missing full expected actual import')
    for path,(raw,pin)in sources.items():require(capture(Path(path),64*1024*1024,pin['sha256'])[0]==raw,'admitted source/input changed')
    return {'status':'BINARY_IMPORT_CONTRACT_PASS','imports':rows,'expected_imports':policy['imports'],'sources':{p:pin for p,(_,pin)in sources.items()},
            'full_validation_performed':False,'Guest_or_native_execution_performed':False,'authority_granted':False}
