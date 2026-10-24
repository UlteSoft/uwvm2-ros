#!/usr/bin/env python3
"""Serial trusted Doosan source preparation and selected SDK qualification.

Run only beneath owner_doosan.py. Source preparation executes no original
vendor controller or installer. Actual test lanes have separate retained
attempts, finite owners and explicit source/tool/Runtime input pins.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,required=True)
    parser.add_argument('--phase',required=True)
    parser.add_argument('--attempt',required=True)
    args=parser.parse_args()
    root=args.root.resolve(strict=True)
    inputs=json.loads((root/'inputs.json').read_bytes())
    source=Path(inputs['source_root']);kit=source/'sdk/origin/robots/arms/doosan'
    profiles=json.loads((kit/'profiles.json').read_bytes())['profiles'] if args.phase!='discover' else []
    python=[sys.executable,'-I','-S','-B']
    if args.phase in ('qualify','managed','cross','finish'):
        required=['sdk/origin/tools/check_wasm_imports.py']
        if args.phase=='managed':
            required+=['sdk/origin/javascript/guest/'+n for n in (
                'prepare.py','engine.c','engine.h','stdio.c','utf8.c','command.c',
                'scalars.c','native_ros.c','native_rmw_service.c','upstream-lock.json','bootstrap.mjs')]
        for name in required:assert (source/name).is_file(),name
        for name in ('runtime','validator'):assert Path(inputs[name]).is_file(),name
    def run(argv,deadline=1800):
        subprocess.run(list(map(str,argv)),cwd=source,check=True,timeout=deadline)
    acquired=Path(inputs.get('capture_output',root/'out/acquired'))
    if args.phase=='discover':
        run([*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/discover_doosan.py','--out',root/'out'/('discover-'+args.attempt)])
    elif args.phase=='capture':
        argv=[*python,kit/'tools/capture_sources.py','--source',source,'--mirror',root/'mirror','--out',acquired]
        if inputs.get('capture_blob_cache'):argv+=['--blob-cache',inputs['capture_blob_cache']]
        if inputs.get('capture_archive_download'):argv+=['--archive-download']
        run(argv)
    elif args.phase=='prepare':
        # Admit the entire fixed preparation closure before generating any
        # profile. An incomplete transport is an operator failure, not a
        # vendor parser or SDK compatibility result.
        for relative in ['test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_identity.cc',
                         *['sdk/origin/custom-types/interface_templates/bounded_interfaces.'+ext+'.in'
                           for ext in ('rs','mjs','go','ts')]]:
            assert (source/relative).is_file(),relative
        for name in ('upstream','catalogs'):
            if (kit/name).exists():
                prior={str(p.relative_to(acquired/name)):p for p in (acquired/name).rglob('*')if p.is_file()}
                current={str(p.relative_to(kit/name)):p for p in (kit/name).rglob('*')if p.is_file()}
                assert prior.keys()==current.keys()
                assert all(prior[n].read_bytes()==current[n].read_bytes()for n in prior)
            else:shutil.copytree(acquired/name,kit/name)
        for row in profiles:
            profile=row['id'];out=root/'out'/('prepared-'+profile+'-'+args.attempt)
            generated=source/'sdk/origin/custom-types'/('doosan-'+profile.replace('_','-')+'-complete')
            if generated.exists():
                # Earlier closed preparation may have completed this exact
                # domain before a later domain refused. Reuse only verified
                # original inputs and every exact generated body, never a test
                # status. Subsequent execution lanes still compile all domains.
                prior=json.loads((generated/'preparation.vendor.json').read_bytes())
                catalog=kit/'catalogs'/profile/'catalog.json'
                assert prior['mode']=='prepare'and prior['original_catalog']['sha256']==hashlib.sha256(catalog.read_bytes()).hexdigest()
                for name,pin in prior['original_definitions'].items():
                    assert pin['sha256']==hashlib.sha256((catalog.parent/name).read_bytes()).hexdigest()
                receipt=json.loads((generated/'generation.vendor.json').read_bytes())
                assert not receipt['native_registry_emitted']and receipt['default_authority']==[]
                for name,pin in receipt['outputs'].items():
                    body=(generated/name).read_bytes()
                    assert len(body)==pin['bytes']and hashlib.sha256(body).hexdigest()==pin['sha256']
                print('REUSED_EXACT_PREPARED_SOURCE_NOT_EXECUTION',profile,flush=True)
                continue
            argv=[*python,source/'sdk/origin/robots/industrial/prepare.py','--source-root',source,
                  '--catalog',kit/'catalogs'/profile/'catalog.json','--out',out,'--mode','prepare']
            for role,path in [('parent','sdk/origin/tools/custom_types.py'),('bounded','sdk/origin/custom-types/bounded_profile.py'),('adapter','sdk/origin/robots/industrial/vendor_profile.py')]:
                argv+=['--'+role+'-sha256',hashlib.sha256((source/path).read_bytes()).hexdigest()]
            run(argv)
            shutil.copytree(out/'generated',generated)
            if not (generated/'preparation.vendor.json').exists():
                shutil.copyfile(out/'preparation.vendor.json',generated/'preparation.vendor.json')
    elif args.phase=='assemble':
        for script,name,target in [('emit_examples.py','example-sources',kit),('emit_contracts.py','contract-sources',kit/'tests')]:
            out=root/'out'/(name+'-'+args.attempt)
            run([*python,kit/'tools'/script,'--source',source,'--out',out])
            for path in sorted(out.rglob('*')):
                if path.is_file():
                    destination=target/path.relative_to(out)
                    destination.parent.mkdir(parents=True,exist_ok=True)
                    with destination.open('xb')as stream:stream.write(path.read_bytes())
    elif args.phase=='verify':
        out=root/'out'/('verify-'+args.attempt);out.mkdir(mode=0o700)
        argv=[*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/verify_doosan.py','--root',root]
        for name in ('direct','managed','finish','cross'):
            argv+=['--'+name,inputs['qualification_attempts'][name]]
        with (out/'verification.json').open('xb')as stdout,(out/'stderr').open('xb')as stderr:
            subprocess.run(list(map(str,argv)),cwd=source,stdout=stdout,stderr=stderr,check=True,timeout=180)
        result=json.loads((out/'verification.json').read_bytes())
        assert result['status']=='INDEPENDENT_DOOSAN_SOURCE_PRODUCTS_REFUSALS_AND_FOUR_ARCH_DATA_PASS'
        run([*python,kit/'tests/validate_sources.py','--source',source])
        print('INDEPENDENT_DOOSAN_VERIFICATION_PASS',flush=True)
    else:
        argv=[*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk'/(args.phase+'_doosan.py'),
              '--root',root,'--out',root/'out'/(args.phase+'-'+args.attempt)]
        if args.phase=='cross':argv+=['--vectors',inputs['vectors']]
        run(argv)


if __name__=='__main__':main()
