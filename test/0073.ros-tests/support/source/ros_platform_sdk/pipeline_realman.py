#!/usr/bin/env python3
"""Serial trusted RealMan source preparation and selected SDK qualification.

Run only beneath owner_realman.py. Source preparation executes no original
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
    source=Path(inputs['source_root']);kit=source/'sdk/origin/robots/arms/realman'
    profiles=json.loads((kit/'profiles.json').read_bytes())['profiles']
    python=[sys.executable,'-I','-S','-B']
    def run(argv,deadline=1200):
        subprocess.run(list(map(str,argv)),cwd=source,check=True,timeout=deadline)
    acquired=Path(inputs.get('capture_output',root/'out/acquired'))
    if args.phase=='capture':
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
            argv=[*python,source/'sdk/origin/robots/industrial/prepare.py','--source-root',source,
                  '--catalog',kit/'catalogs'/profile/'catalog.json','--out',out,'--mode','prepare']
            for role,path in [('parent','sdk/origin/tools/custom_types.py'),('bounded','sdk/origin/custom-types/bounded_profile.py'),('adapter','sdk/origin/robots/industrial/vendor_profile.py')]:
                argv+=['--'+role+'-sha256',hashlib.sha256((source/path).read_bytes()).hexdigest()]
            run(argv)
            generated=source/'sdk/origin/custom-types'/('realman-'+profile.replace('_','-')+'-complete')
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
        argv=[*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/verify_realman.py','--root',root]
        for name in ('direct','managed','finish','cross'):
            argv+=['--'+name,inputs['qualification_attempts'][name]]
        with (out/'verification.json').open('xb')as stdout,(out/'stderr').open('xb')as stderr:
            subprocess.run(list(map(str,argv)),cwd=source,stdout=stdout,stderr=stderr,check=True,timeout=180)
        result=json.loads((out/'verification.json').read_bytes())
        assert result['status']=='INDEPENDENT_REALMAN_SOURCE_PRODUCTS_REFUSALS_AND_FOUR_ARCH_DATA_PASS'
        run([*python,kit/'tests/validate_sources.py','--source',source])
        print('INDEPENDENT_REALMAN_VERIFICATION_PASS',flush=True)
    else:
        argv=[*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk'/(args.phase+'_realman.py'),
              '--root',root,'--out',root/'out'/(args.phase+'-'+args.attempt)]
        if args.phase=='cross':argv+=['--vectors',inputs['vectors']]
        run(argv)


if __name__=='__main__':main()
