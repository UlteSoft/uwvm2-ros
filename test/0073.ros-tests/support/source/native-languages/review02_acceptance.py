#!/usr/bin/env python3
"""Real installed native-example xmake acceptance inside the shared owner.

This new unnumbered support source performs no physical migration or catalog
publication. It requires a genuine assembled SDK and actual tool programs.
"""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess
import sys


def sha(path):
    with path.open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk',type=Path,required=True)
    parser.add_argument('--xmake',type=Path,required=True)
    parser.add_argument('--xmake-sha256',required=True)
    parser.add_argument('--toolchain',type=Path,required=True)
    parser.add_argument('--profile-manifest',type=Path,required=True)
    parser.add_argument('--language',choices=('rs','go','tinygo','zig','swift','haskell','nim','d','odin'),required=True)
    parser.add_argument('--example',required=True)
    parser.add_argument('--backend',choices=('ros1','ros2'),required=True)
    parser.add_argument('--configure',action='append',default=[])
    parser.add_argument('--validator',type=Path,required=True)
    parser.add_argument('--run-empty',action='store_true')
    parser.add_argument('--runtime',type=Path)
    parser.add_argument('--runtime-sha256')
    args=parser.parse_args();root=Path.cwd();sdk=args.sdk.resolve(strict=True)
    if (sdk/'origin').exists():raise ValueError('actual standalone assembled SDK required, not the maintenance checkout')
    if sha(args.xmake)!=args.xmake_sha256:raise ValueError('actual xmake program pin differs')
    manifest=args.profile_manifest.resolve(strict=True)
    if not manifest.is_relative_to(sdk):raise ValueError('manifest must be SDK-local for the relocation check')
    if args.run_empty and (args.runtime is None or args.runtime_sha256 is None or sha(args.runtime)!=args.runtime_sha256):
        raise ValueError('actual Runtime and matching explicit pin required for execution')
    for path in sdk.rglob('*'):
        if path.is_symlink() and (path.readlink().is_absolute() or not path.resolve(strict=True).is_relative_to(sdk)):
            raise ValueError('SDK has a nonrelocatable link: '+str(path))
    staged=root/'relocated-sdk';shutil.copytree(sdk,staged,symlinks=True)
    logdir=root/'logs';logdir.mkdir();output=root/'example-output';rows=[]
    receipt={'kind':'task03-native-example-review02-v1','language':args.language,'example':args.example,
        'backend':args.backend,'xmake_sha256':args.xmake_sha256,'profile_manifest_sha256':sha(manifest),
        'validator_sha256':sha(args.validator),'rows':rows,'default_authority':[],
        'Guest_attempts':0,'Guest_passes':0,'native_ROS_runs':0,'Gateway_runs':0,'upstream_runs':0,'QEMU_runs':0,'VTune_runs':0}
    def save():
        (root/'review02-acceptance.json').write_text(json.dumps(receipt,indent=2)+'\n')
    def run(name,layer,command,timeout=300):
        row={'name':name,'layer':layer,'argv':[str(x) for x in command]};rows.append(row)
        log=logdir/(name+'.log');row['log']=str(log.relative_to(root));save()
        try:
            with log.open('wb') as stream:result=subprocess.run(row['argv'],cwd=staged,stdout=stream,stderr=subprocess.STDOUT,timeout=timeout)
            row.update(exit=result.returncode,status='PASS' if result.returncode==0 else 'FAIL')
        except Exception as error:row.update(status='FAIL',error=str(error))
        save();print(name,row['status'],flush=True);return row['status']=='PASS'
    configuration=['f','--sdk-example-platform=ros','--sdk-example-language='+args.language,'--sdk-example='+args.example,
        '--sdk-ros-backend='+args.backend,'--sdk-target=wasm32-wasip1','--sdk-startup=command',
        '--sdk-exceptions=n','--sdk-threads=n','--sdk-offline=y','--sdk-toolchain='+str(args.toolchain),
        '--sdk-profile-manifest='+str(staged/manifest.relative_to(sdk)),*args.configure]
    if not run('configure','actual_xmake_configuration',[args.xmake,*configuration]):return 1
    if not run('catalog','actual_xmake_source_catalog',[args.xmake,'sdk-example-list']):return 1
    if not run('build','actual_xmake_build',[args.xmake,'sdk-example-build','--output='+str(output)],timeout=900):return 1
    actual=json.loads((output/'build-receipt.json').read_bytes())
    for index,artifact in enumerate(actual['artifacts']):
        path=Path(artifact['path']).resolve(strict=True)
        if not path.is_relative_to(output) or artifact['kind']!='wasm':raise ValueError('expected owned actual Wasm command')
        if not run('validate-'+str(index),'independent_wasm_validation',[args.validator,path]):return 1
    if args.run_empty:
        receipt['Guest_attempts']+=1;save()
        if run('Guest-empty','actual_Guest_empty_authority',[args.xmake,'sdk-example-run',
            '--receipt='+str(output/'build-receipt.json'),'--runtime='+str(args.runtime)],timeout=60):
            receipt['Guest_passes']+=1;save()
        else:return 1
    return 0


if __name__=='__main__':sys.exit(main())
