#!/usr/bin/env python3
"""Prepare one immutable source snapshot and selected real compiler checks.

Execute under the shared finite owner; this is no independent scheduler, test
substitute or resource harness. Generators and every compiler run serially.
"""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess
import sys
import tarfile


def stage_inputs(origin,explicit):
    root=explicit or origin.parents[1]/'test/0073.ros-tests'
    if not(root/'support/stage_sdk_tests.py').is_file():
        root=next((p for p in Path(__file__).resolve().parents
            if(p/'support/stage_sdk_tests.py').is_file()),None)if explicit is None else None
    if root is None:raise ValueError('separate test artifact required; supply --test-artifact-root')
    sys.path.insert(0,str(root/'support'))
    from stage_sdk_tests import native_languages_tools
    return native_languages_tools(origin,test_root=root)


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--archive',type=Path,required=True)
    ap.add_argument('--test-artifact-root',type=Path,help='Separate unified test artifact when absent from the source archive')
    ap.add_argument('--tools',type=Path,required=True)
    ap.add_argument('--go',type=Path,required=True)
    ap.add_argument('--wasi-sdk',type=Path,required=True)
    ap.add_argument('--runtime',type=Path,required=True)
    ap.add_argument('--ldc-tools',type=Path)
    ap.add_argument('--language',action='append',choices=('nim','tinygo','odin','d'),required=True)
    args=ap.parse_args();root=Path.cwd();source=root/'source';source.mkdir()
    with tarfile.open(args.archive) as archive:archive.extractall(source,filter='data')
    origin=source/'sdk/origin';helper=origin/'tools/native_languages'
    staging=stage_inputs(origin,args.test_artifact_root)
    preparation=[]
    for name,command in (
        ('numeric',[sys.executable,'-I','-S','-B',str(helper/'numeric.py'),'--schema',str(origin/'abi/abi-v1.json'),'--out',str(root/'numeric')]),
        ('records',[sys.executable,'-I','-S','-B',str(helper/'emit.py'),'--origin',str(origin),'--catalog',str(helper/'fixtures/catalog.json'),'--guest-only','--out',str(root/'generated')])):
        log=root/(name+'.log')
        with log.open('wb') as output:r=subprocess.run(command,stdout=output,stderr=subprocess.STDOUT)
        preparation.append({'name':name,'exit':r.returncode,'log':log.name})
        if r.returncode:break
    (root/'subset-input03.json').write_text(json.dumps({'source_sha256':hashlib.sha256(args.archive.read_bytes()).hexdigest(),
        'test_source_staging':staging,'selected_languages':args.language,'preparation':preparation,'default_authority':[]},indent=2)+'\n')
    if any(row['exit'] for row in preparation):return 1
    for language in ('swift','haskell','nim','d','odin'):
        shutil.copytree(root/'numeric'/language,origin/language,dirs_exist_ok=True)
    command=[sys.executable,'-I','-S','-B',str(helper/'validate_additional.py'),'--origin',str(origin),
        '--generated',str(root/'generated'),'--tools',str(args.tools),'--go',str(args.go),
        '--wasi-sdk',str(args.wasi_sdk),'--runtime',str(args.runtime)]
    for language in args.language:command+=['--language',language]
    if args.ldc_tools:command+=['--ldc-tools',str(args.ldc_tools)]
    return subprocess.run(command,check=False).returncode


if __name__=='__main__':sys.exit(main())
