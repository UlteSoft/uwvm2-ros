#!/usr/bin/env python3
"""Check relocated metadata and archived bytes under the actual finite owner.

This checks the supplied source capsule, not a complete SDK installation, a
compiler product, a Guest, or an official robot suite.
"""
import ast
import gzip
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tarfile

ROOT=next(p for p in Path(__file__).resolve().parents if(p/'sdk/origin').is_dir())
SUPPORT=ROOT/'test/0073.ros-tests/support'
sys.path.insert(0,str(SUPPORT))
from resource_contract import validate_resource_owner
from source_paths import sdk_path,sdk_test_source,historical_record,_historical_rows


def require(condition,message):
    if not condition:raise AssertionError(message)


def main():
    proof=validate_resource_owner();cases=[]
    def passed(name,denominator=1):cases.append(dict(name=name,passed=denominator,denominator=denominator))
    files=sorted(ROOT.rglob('*.py'))
    for path in files:
        tree=ast.parse(path.read_bytes(),filename=str(path))
        for node in ast.walk(tree):
            if isinstance(node,ast.Attribute)and isinstance(node.value,ast.Constant)and isinstance(node.value.value,str):
                require(node.attr not in('read_bytes','read_text','with_suffix','with_name','resolve','is_file','is_dir'),
                    'Path operation applied to a string literal: '+str(path)+':'+str(node.lineno))
    passed('current-capsule-python-syntax-and-path-expression-check',len(files))
    mapping=json.loads((SUPPORT/'sdk-test-source-map.json').read_bytes())
    rows=_historical_rows(ROOT/'test/0073.ros-tests',mapping)
    archive=SUPPORT/'records/sdk-test-migration/sdk-test-records.tar.gz'
    require(hashlib.sha256(archive.read_bytes()).hexdigest()==mapping['historical_record_storage']['sha256'],
        'historical archive hash differs')
    with tarfile.open(archive,'r:gz')as source:
        members=source.getmembers()
        require({row['storage']['member']for row in rows.values()}=={m.name for m in members},'exact historical inventory required')
        require(len(members)==len(rows)==1407,'all original historical records required')
        by_member={row['storage']['member']:row for row in rows.values()}
        for item in members:
            row=by_member[item.name]
            require(item.isfile()and item.size==row['original_bytes'],'historical member size/type changed')
            require(hashlib.sha256(source.extractfile(item).read()).hexdigest()==row['original_sha256'],
                'historical member bytes changed')
    passed('exact-preservation-of-historical-record-bytes',len(rows))
    for name in (next(n for n in rows if n.startswith('sdk/origin/')),
            next(n for n in rows if n.startswith('sdk/test/'))):
        raw=historical_record(name,test_root=ROOT/'test/0073.ros-tests')
        require(hashlib.sha256(raw).hexdigest()==rows[name]['original_sha256'],'historical record lookup changed bytes')
    passed('origin-and-delivered-historical-lookup',2)
    origin=ROOT/'sdk/origin'
    require(sdk_path(origin,'abi/abi-v1.json')==origin/'abi/abi-v1.json','production lookup changed')
    selected=sdk_test_source(origin,'tests/c/wrapper_contract.c')
    require(selected==SUPPORT/'source/sdk-tests/c/wrapper_contract.c','canonical moved test source required')
    require(sdk_path(origin,'tests/c/wrapper_contract.c')==selected,'general SDK input lookup disagrees')
    passed('real-production-and-canonical-source-lookup',3)
    for invalid in ('../escape','/absolute'):
        for function in (sdk_path,sdk_test_source,historical_record):
            try:
                if function is historical_record:function(invalid,test_root=ROOT/'test/0073.ros-tests')
                else:function(origin,invalid)
            except ValueError:continue
            raise AssertionError('path confinement accepted '+invalid)
    passed('source-and-record-traversal-refusal',6)
    damaged=dict(mapping);damaged['historical_record_index']=dict(mapping['historical_record_index'],sha256='0'*64)
    try:_historical_rows(ROOT/'test/0073.ros-tests',damaged)
    except ValueError:pass
    else:raise AssertionError('changed historical index accepted')
    passed('historical-index-tamper-refusal')
    require(len(mapping['existing_case_ids'])==len(set(mapping['existing_case_ids']))==431,'existing case ID set changed')
    passed('431-existing-identities-recorded',431)
    command=[sys.executable,'-I','-S','-B',str(SUPPORT/'source/ros_platform_sdk/test_numbered_catalog.py')]
    outcome=subprocess.run(command,stdin=subprocess.DEVNULL,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=30)
    Path('catalog-contracts.log').write_bytes(outcome.stdout)
    require(outcome.returncode==0,'numbered catalog contracts failed; see retained log')
    passed('numbered-catalog-contract-tests',12)
    pins=[dict(path=p.relative_to(ROOT).as_posix(),bytes=p.stat().st_size,
        sha256=hashlib.sha256(p.read_bytes()).hexdigest())for p in sorted(ROOT.rglob('*'))if p.is_file()]
    result=dict(kind='sdk-test-cutover-source-contract-v1',status='PASS',resource=proof,cases=cases,source_pins=pins,
        source_capsule_only=True,numbered_case_workloads_executed=0,compiler_cases=0,guest_cases=0,
        native_ros_cases=0,official_upstream_cases=0,default_authority=[])
    Path('cutover-contract.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    print(json.dumps(result),flush=True)


if __name__=='__main__':main()
