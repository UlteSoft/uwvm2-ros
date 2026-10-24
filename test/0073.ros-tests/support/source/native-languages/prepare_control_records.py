#!/usr/bin/env python3
"""Generate one public Rust control-record module under the finite Linux owner.

The immutable source capsule contains the genuine original catalog/operator.
SDK consumers use the one canonical generated module. This job supplies no
Rust typechecking, installation, Wasm/Guest or positive Native ROS credit.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import subprocess
import sys
import tarfile


def digest(path):
    with path.open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--sha256',required=True)
    args=parser.parse_args();work=Path.cwd();source=work/'inputs-source'
    if digest(args.source)!=args.sha256:raise ValueError('Original frozen source changed')
    source.mkdir()
    with tarfile.open(args.source) as archive:
        members=archive.getmembers()
        if sum(row.size for row in members)>12*1024**2 or any(not row.isfile() or
                Path(row.name).is_absolute() or '..' in Path(row.name).parts for row in members):
            raise ValueError('Bounded ordinary retained input files required')
        archive.extractall(source,filter='data')
    before={str(p.relative_to(source)):digest(p) for p in source.rglob('*') if p.is_file()}
    origin=source/'sdk/origin';parent=origin/'tools/custom_types.py'
    bounded=origin/'custom-types/bounded_profile.py'
    operator=origin/'rmw_wasm/default_control/managed/generate_managed.py'
    catalog=origin/'rmw_wasm/default_control/catalog.preparation.json'
    canonical=work/'canonical-c-cpp';managed=work/'complete-managed';rows=[]
    def run(name,command,env=None):
        log=work/(name+'.log')
        with log.open('wb') as stream:
            result=subprocess.run([str(v) for v in command],env=env,
                stdout=stream,stderr=subprocess.STDOUT,timeout=180)
        rows.append({'name':name,'argv':[str(v) for v in command],
                     'exit':result.returncode,'log_sha256':digest(log)})
        if result.returncode:raise ValueError('Retained actual '+name+' failure')
    run('genuine-c-cpp-control',[sys.executable,'-I','-S','-B',parent,catalog,'--guest-only','--out',canonical])
    run('genuine-full-managed-control',[sys.executable,'-I','-S','-B',operator,
        '--source-root',source,'--catalog',catalog,'--canonical',canonical,'--out',managed,
        '--parent-sha256',digest(parent),'--bounded-sha256',digest(bounded),'--records-only'])
    proof_path=managed/'generation.control-managed.json';proof=json.loads(proof_path.read_bytes())
    records=managed/'bounded_records.rs';original=proof['outputs']['bounded_records.rs']
    if len(proof['record_names'])!=32 or len(proof['services'])!=7 or proof['native_registry_emitted'] is not False:
        raise ValueError('Complete original32-record/seven-service proof required')
    if records.stat().st_size!=original['bytes'] or digest(records)!=original['sha256']:
        raise ValueError('Actual generated source differs from its proof')
    agents=Path('/home/macromodel/Documents/uwvm-test-261018/agents/03')
    rust=agents/'20261021-toolchains-r1/work/toolchains/rust'
    formatter=agents/'20261021-round03-rustfmt-r1/work/tools/bin/rustfmt'
    if digest(formatter)!='5d90e4b57cf7ae89a83c35428a245c0de9fa050fd13c5ad7f19b2ee4b561ae6f':
        raise ValueError('Matching actual Rust1.85 formatter required')
    environment=dict(os.environ);environment['LD_LIBRARY_PATH']=str(rust/'lib')
    for number in (1,2):
        run('actual-rust-format-'+str(number),[formatter,'--edition','2021',records],environment)
        if number==1:first=digest(records)
    if digest(records)!=first:raise ValueError('Actual Rust formatter is not idempotent')
    if any(digest(source/name)!=value for name,value in before.items()):
        raise ValueError('Retained original source changed during generation')
    public={'format':'uwvm-original-control-records-canonical-source-v1',
        'status':'GENERATED_AND_FORMATTED_TYPECHECK_AND_GUEST_PENDING',
        'canonical_profile':proof['canonical_profile'],'record_names':proof['record_names'],
        'services':proof['services'],'native_registry_emitted':False,
        'record_source':'include/platform/ros/rs/owned_service/src/records.rs',
        'record_product':{'bytes':records.stat().st_size,'sha256':digest(records)},
        'original_generated_product':original,'source_inputs':proof['source_inputs'],
        'canonical_proof_sha256':digest(canonical/'generation.json'),
        'managed_proof_sha256':digest(proof_path),'original_source_capsule_sha256':args.sha256,
        'generator_sha256':digest(parent),'bounded_generator_sha256':digest(bounded),
        'operator_sha256':digest(operator),'formatter_sha256':digest(formatter),
        'actual_formatter_passes':2,'commands':rows,'default_authority':[],
        'Rust_compiler_runs':0,'Guest_runs':0,'native_ROS_runs':0}
    (work/'owned_control_records.json').write_text(json.dumps(public,indent=2)+'\n')
    return 0


if __name__=='__main__':raise SystemExit(main())
