#!/usr/bin/env python3
"""One serialized finite-owner workload for current regression/additional tools."""

from pathlib import Path
import argparse
import json
import subprocess
import sys


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--archive',type=Path,required=True)
    ap.add_argument('--test-artifact-root',type=Path,help='Separate unified test artifact when absent from the source archive')
    ap.add_argument('--base-tools',type=Path,required=True)
    ap.add_argument('--additional-tools',type=Path,required=True)
    ap.add_argument('--ldc-tools',type=Path)
    ap.add_argument('--wasi-sdk',type=Path,required=True)
    ap.add_argument('--runtime',type=Path,required=True)
    args=ap.parse_args();root=Path.cwd()
    # The canonical base driver stages checks in its owned production copy.
    command=[sys.executable,'-I','-S','-B',str(Path(__file__).resolve().with_name('validate.py')),
        '--source-archive',str(args.archive),'--tools',str(args.base_tools),'--runtime',str(args.runtime),
        '--skip-rust','--classify-go-startup-refusal']
    if args.test_artifact_root:command+=['--test-artifact-root',str(args.test_artifact_root)]
    first=subprocess.run(command,check=False)
    origin=root/'source/sdk/origin';generated=root/'generated'
    second=127
    if generated.is_dir():
        command=[sys.executable,'-I','-S','-B',str(origin/'tools/native_languages/validate_additional.py'),
            '--origin',str(origin),'--generated',str(generated),'--tools',str(args.additional_tools),
            '--go',str(args.base_tools/'go-input/go'),'--wasi-sdk',str(args.wasi_sdk),'--runtime',str(args.runtime)]
        if args.ldc_tools:command+=['--ldc-tools',str(args.ldc_tools)]
        second=subprocess.run(command,check=False).returncode
    (root/'extended-status03.json').write_text(json.dumps({'base_exit':first.returncode,'additional_exit':second,
        'native_ROS_runs':0,'Gateway_integration_runs':0,'default_authority':[],
        'clock_grants':'only explicitly named Go control cases; never counted as strict baseline'},indent=2)+'\n')
    return 0 if first.returncode==0 and second==0 else 1


if __name__=='__main__':sys.exit(main())
