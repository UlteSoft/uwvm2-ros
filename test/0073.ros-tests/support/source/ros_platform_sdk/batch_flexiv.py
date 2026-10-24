#!/usr/bin/env python3
"""Close and immediately replicate the four selected native-package phases.

The independent owner bounds every compiler/test tree. This serial operator
dispatcher never runs an archived controller, changes Guest policy or treats
the owner's zero exit as proof that upstream package cases passed. Each native
report must independently state its actual package-test outcome. A later
nativecheck phase verifies original sources, installed products and all cases.
"""
import argparse
import json
from pathlib import Path
import subprocess
import sys


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--destination',type=Path,required=True)
    p.add_argument('--replica',type=Path,required=True)
    a=p.parse_args()
    root=a.root.resolve(strict=True)
    inputs=json.loads((root/'inputs.json').read_bytes())
    source=Path(inputs['source_root'])
    python=[sys.executable,'-I','-S','-B']
    def run(phase,attempt):
        subprocess.run([*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/owner_flexiv.py',
                        '--root',root,'--phase',phase,'--attempt',attempt],check=True,timeout=1900)
    def retain(phase,attempt):
        label=phase+'-'+attempt
        subprocess.run([*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/retain_platform_evidence.py',
            '--root',root,'--label',label,'--destination',a.destination,'--replica',a.replica,
            '--include','out/'+label,'--include','out/flexiv-'+label+'.json',
            '--include','out/commands/'+label,'--include','inputs.json','--include','control'],
            check=True,timeout=900)
    for profile,selection in inputs['native_attempts'].items():
        attempt=selection['build']
        assert not selection.get('retest'),'This dispatcher accepts fresh direct builds only'
        before=root/'control'/('inputs-before-native-'+attempt+'.json')
        assert not before.exists()
        before.write_text(json.dumps(inputs,indent=2)+'\n')
        inputs['native_profile']=profile
        (root/'inputs.json').write_text(json.dumps(inputs,indent=2)+'\n')
        run('native',attempt)
        report=json.loads((root/'out'/('native-'+attempt)/'native.json').read_bytes())
        # Retain failures too; checking the result is never permission to
        # discard its original logs or promote an upstream failure to PASS.
        retain('native',attempt)
        assert report['status']=='ORIGINAL_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS',profile
    for phase in ('nativecheck','audit'):
        run(phase,'r1')
        retain(phase,'r1')
    print('FLEXIV_NATIVE_BATCH_AND_INDEPENDENT_PACKAGE_SOURCE_CHECKS_CLOSED',flush=True)


if __name__=='__main__':main()
