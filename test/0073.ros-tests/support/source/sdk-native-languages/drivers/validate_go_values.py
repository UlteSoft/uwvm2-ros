#!/usr/bin/env python3
"""Real official-Go Wasm value/denial checks inside the common finite owner.

The ABI golden input is embedded at compile time. No file preopen, native ROS,
substituted import, extra ROS permission or Host codec is used at runtime.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
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
    ap.add_argument('--go',type=Path,required=True)
    ap.add_argument('--runtime',type=Path,required=True)
    args=ap.parse_args();root=Path.cwd();source=root/'source';source.mkdir()
    with tarfile.open(args.archive) as archive:archive.extractall(source,filter='data')
    origin=source/'sdk/origin';project=root/'application';project.mkdir()
    staging=stage_inputs(origin,args.test_artifact_root)
    shutil.copy2(origin/'tools/native_languages/fixtures/go-values/main.go',project/'main.go')
    vectors=origin/'abi/wire-vectors-v1.json'
    shutil.copy2(vectors,project/vectors.name)
    (project/'go.mod').write_text('module uwvm-native.local/govalues\n\ngo 1.24\n\nrequire uwvm2.local/roswasmgo v0.0.0\nreplace uwvm2.local/roswasmgo => '+str(origin/'go')+'\n')
    runtime_sha=hashlib.sha256(args.runtime.read_bytes()).hexdigest()
    if runtime_sha!='0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556':
        raise ValueError('frozen current Task10 Runtime pin changed')
    env=os.environ.copy();env.update(GOROOT=str(args.go.parent.parent),GOOS='wasip1',GOARCH='wasm',CGO_ENABLED='0',
        GOTOOLCHAIN='local',GOWORK='off',GOFLAGS='',GOPROXY='off',GOSUMDB='off',GOMAXPROCS='1',
        GOCACHE=str(root/'go-cache'),GOMODCACHE=str(root/'go-mod-cache'))
    rows=[];logs=root/'logs';logs.mkdir();fixtures=json.loads(vectors.read_bytes())
    def save():
        (root/'go-values03.json').write_text(json.dumps({'source_archive_sha256':hashlib.sha256(args.archive.read_bytes()).hexdigest(),
            'test_source_staging':staging,
            'ABI_golden_sha256':hashlib.sha256(vectors.read_bytes()).hexdigest(),
            'specification_denominator':{key:len(fixtures[key]) for key in ('positive','negative')},
            'runtime_sha256':runtime_sha,'rows':rows,'default_authority':[],
            'native_ROS_runs':0,'Gateway_integration_runs':0,'upstream_runs':0,'QEMU_runs':0,'VTune_runs':0},indent=2)+'\n')
    def run(name,layer,command,timeout=240):
        row={'name':name,'layer':layer,'argv':[str(x) for x in command]};log=logs/(name+'.log');row['log']=str(log.relative_to(root))
        try:
            with log.open('wb') as output:r=subprocess.run(row['argv'],cwd=project,env=env,stdout=output,stderr=subprocess.STDOUT,timeout=timeout)
            row.update(exit=r.returncode,status='PASS' if r.returncode==0 else 'FAIL')
        except Exception as error:row.update(status='FAIL',error=str(error))
        rows.append(row);save();print(name,row['status'],flush=True);return row
    run('official-go-version','inventory',[args.go,'version'])
    for backend in ('ros1','ros2'):
        output=root/('go-values-'+backend+'.wasm')
        if run('compile-'+backend,'compiler',[args.go,'build','-p=1','-tags',backend,'-trimpath','-o',output,'.'])['status']!='PASS':continue
        if run('validate-'+backend,'wasm_validation',['/usr/bin/wasm-validate',output])['status']!='PASS':continue
        row=run('Guest-baseline-'+backend,'actual_guest_default_denial',[args.runtime,'--run',output],timeout=30)
        if row['status']=='FAIL' and row.get('exit',0)!=0 and 'clock_time_get failed' in (root/row['log']).read_text(errors='replace'):
            row.update(status='UNAVAILABLE',layer='actual_guest_startup_refusal',reason='clock denied before main; no codec or ROS-denial credit');save()
        row=run('Guest-explicit-monotonic-'+backend,'actual_guest_value_codec_and_ROS_denial_explicit_clock',
            [args.runtime,'--wasip1-global-grant','monotonic-clock','--run',output],timeout=30)
        if row['status']=='PASS':
            output_text=(root/row['log']).read_text(errors='replace')
            match=re.search(r'PASS_TASK03_GO_VALUES positive=(\d+) negative=(\d+) truncations=(\d+) omitted=(.*)',output_text)
            if match:
                row['actual_codec_counts']={key:int(match[i+1]) for i,key in enumerate(('positive','negative','truncations'))}
                row['omitted_names']=match[4]
                expected={'positive':len(fixtures['positive']),'negative':len(fixtures['negative']),
                    'truncations':sum(len(bytes.fromhex(v['hex'])) for v in fixtures['positive'])}
                if row['actual_codec_counts']!=expected or match[4].strip()!='[]':
                    row.update(status='FAIL',reason='actual codec coverage differs from the complete frozen ABI denominator',expected_codec_counts=expected)
            else:row.update(status='FAIL',reason='actual value/denial application completion marker missing')
            save()
    return 1 if any(row['status']=='FAIL' for row in rows) else 0


if __name__=='__main__':sys.exit(main())
