#!/usr/bin/env python3
"""Check the actual delivered platform-source audit and its incomplete-CI exit.

This is source coverage only. A matching six-language generated family does
not qualify its native driver, robot, release, operation or upstream suite.
The expected missing vendors are visible, and --require-all must fail while
they remain missing. Run in the original bounded Linux owner.
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
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args()
    root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes())
    source=Path(inputs['audit_source_root'])
    manifest=json.loads((source/'source-io.json').read_bytes())
    def stable():
        for row in manifest:
            relative=Path(row['path'])
            assert not relative.is_absolute() and '..'not in relative.parts
            path=source/relative
            assert not path.is_symlink() and path.stat().st_size==row['bytes']
            with path.open('rb')as stream:
                assert hashlib.file_digest(stream,'sha256').hexdigest()==row['sha256']
    stable()
    assert out.is_absolute() and out.parent==root/'out' and not out.exists()
    out.mkdir(mode=0o700)
    result=None
    commands=[]
    for label,flags,expected in [('source-coverage',[],0),('incomplete-ci',['--require-all'],1)]:
        argv=[sys.executable,'-I','-S','-B',str(source/'sdk/origin/tools/audit_ros_platform_sdks.py'),
              '--source',str(source),*flags]
        with (out/(label+'.json')).open('xb')as stdout,(out/(label+'.stderr')).open('xb')as stderr:
            process=subprocess.run(argv,stdout=stdout,stderr=stderr,check=False,timeout=120)
        assert process.returncode==expected
        actual=json.loads((out/(label+'.json')).read_bytes())
        if result is not None:assert actual==result
        result=actual
        commands.append(dict(argv=argv,exit=process.returncode,expected_exit=expected))
    stable()
    assert result['vendor_count']==28 and len(result['generated_kits'])==65
    assert result['vendors_with_selected_generated_source']==20
    assert not result['all_vendors_have_selected_generated_source']
    assert all(k['status']=='GENERATED_SOURCE_HASHES_MATCH'for k in result['generated_kits'].values())
    missing=[r['id']for r in result['vendors']if r['source_status']=='MISSING_OR_PARTIAL_VENDOR_SDK']
    assert len(missing)==8
    report=dict(status='DELIVERED_SOURCE_AUDIT_AND_EXPECTED_INCOMPLETE_CI_REFUSAL_PASS',
        commands=commands,checked_files=len(manifest),generated_families=65,
        vendors_with_selected_source=20,total_vendors=28,missing_or_partial=missing,
        actual_compiler_guest_native_runs=0,complete_vendor_support_claimed=False,default_authority=[])
    (out/'audit.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],flush=True)


if __name__=='__main__':main()
