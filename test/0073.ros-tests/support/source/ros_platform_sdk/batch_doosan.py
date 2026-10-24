#!/usr/bin/env python3
"""Close and retain serial Doosan SDK/data qualification lanes.

The caller first joins a completed direct qualification owner. Every later
compiler/Guest/QEMU phase has its own finite independent owner. Failed phases
are retained before the dispatcher stops. No driver, robot, policy or archived
controller executes, and no data-program result becomes native acceptance.
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
    args=p.parse_args()
    root=args.root.resolve(strict=True)
    inputs=json.loads((root/'inputs.json').read_bytes())
    source=Path(inputs['source_root']);python=[sys.executable,'-I','-S','-B']
    for phase in ('managed','finish','cross'):
        report=root/'out'/('doosan-'+phase+'-r1.json')
        if phase!='qualify':
            result=subprocess.run([*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/owner_doosan.py',
                '--root',root,'--phase',phase,'--attempt','r1'],check=False,timeout=1900)
        else:
            # A completed command owner is a precondition; never poll an old
            # RUNNING status into a success or adopt a failed prior attempt.
            assert json.loads(report.read_bytes())['status']=='PASS'
            result=None
        row=json.loads(report.read_bytes());assert row['status']!='RUNNING'
        assert all(c['exit']is not None for c in row['commands'])
        label=phase+'-r1'+(''if row['status']=='PASS'else'-failed')
        argv=[*python,source/'test/0073.ros-tests/support/source/ros_platform_sdk/retain_platform_evidence.py',
            '--root',root,'--label',label,'--destination',args.destination,'--replica',args.replica]
        for relative in ('out/'+phase+'-r1','out/doosan-'+phase+'-r1.json',
                         'out/commands/'+phase+'-r1','inputs.json','control',
                         'source/test/0073.ros-tests/support/source/ros_platform_sdk',
                         'source/sdk/origin/tools/check_javascript_guest_imports.py',
                         'source/sdk/origin/robots/arms/doosan/README.md',
                         'source/sdk/origin/robots/arms/doosan/examples/README.md'):
            if (root/relative).exists():argv+=['--include',relative]
        subprocess.run(argv,check=True,timeout=900)
        assert row['status']=='PASS'and(result is None or result.returncode==0),label
    print('DOOSAN_SERIAL_DATA_PHASES_CLOSED_AND_REPLICATED',flush=True)


if __name__=='__main__':main()
