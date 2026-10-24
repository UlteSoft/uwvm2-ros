#!/usr/bin/env python3
"""Own original package builds and an explicit downstream format-only fix.

The optional --format-only-rebuild retains the original Humble lint failure,
copies the source into a private overlay, removes the reviewed two spaces,
and rebuilds/runs every registered case. Original results are never rewritten.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--destination',type=Path,required=True)
    p.add_argument('--replica',type=Path,required=True)
    p.add_argument('--format-only-rebuild',action='store_true')
    args=p.parse_args();root=args.root.resolve(strict=True)
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    python=[sys.executable,'-I','-S','-B']
    def save():(root/'inputs.json').write_text(json.dumps(inputs,indent=2)+'\n')
    def run(phase,attempt):
        result=subprocess.run([*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/owner_realman.py',
            '--root',root,'--phase',phase,'--attempt',attempt],check=False,timeout=1900)
        owner=json.loads((root/'out'/('realman-'+phase+'-'+attempt+'.json')).read_bytes())
        assert owner['status']!='RUNNING'and all(c['exit']is not None for c in owner['commands'])
        label=phase+'-'+attempt+(''if owner['status']=='PASS'else'-failed')
        argv=[*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/retain_platform_evidence.py',
            '--root',root,'--label',label,'--destination',args.destination,'--replica',args.replica]
        for relative in ('out/'+phase+'-'+attempt,'out/realman-'+phase+'-'+attempt+'.json',
                         'out/commands/'+phase+'-'+attempt,'inputs.json','control'):
            if (root/relative).exists():argv+=['--include',relative]
        subprocess.run(argv,check=True,timeout=900)
        assert owner['status']=='PASS'and result.returncode==0,label
    for profile,selection in inputs['native_attempts'].items():
        inputs['native_profile']=profile;save()
        attempt=selection['build'];run('native',attempt)
        report=json.loads((root/'out'/('native-'+attempt)/'native.json').read_bytes())
        if report['status']=='ORIGINAL_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS':
            continue
        assert args.format_only_rebuild and profile=='ros2_humble',profile
        assert attempt=='humble-r1','The reviewed overlay preserves that exact original attempt'
        run('nativeformat','r1')
        proof=json.loads((root/'out/nativeformat-r1/overlay.json').read_bytes())
        assert proof['status']=='EXPLICIT_CMAKE_WHITESPACE_ONLY_OVERLAY_CREATED'
        for row in proof['files']:
            path=Path(proof['overlay'])/row['path']
            assert hashlib.sha256(path.read_bytes()).hexdigest()==row['after_sha256']
            inputs['pins'].append(dict(path=str(path),bytes=path.stat().st_size,sha256=row['after_sha256']))
        inputs['native_source_overrides']={'ros2_humble':proof['overlay']}
        selection['format_rebuild']='humble-format-r1';save()
        run('native',selection['format_rebuild'])
        fixed=json.loads((root/'out/native-humble-format-r1/native.json').read_bytes())
        assert fixed['status']=='DOWNSTREAM_FORMAT_ONLY_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS'
    run('nativecheck','r1')
    print('REALMAN_ORIGINAL_RESULTS_AND_EXPLICIT_DOWNSTREAM_FORMAT_REBUILD_VERIFIED',flush=True)


if __name__=='__main__':main()
