#!/usr/bin/env python3
"""Check VTune duplicate-column handling using rows from a real source report.

This is a source-attribution parser regression, not a runtime qualification.
Two different source files deliberately contain valid but different lines at
the reported line number. Ambiguous basenames and unknown JIT locations must
remain unattributed. Every fixture and subprocess stays in the shared cgroup.
"""
import argparse
import csv
import hashlib
import json
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sample',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True,exist_ok=False)
    with args.sample.open() as stream:
        reader=csv.reader(stream,delimiter='\t');header=next(reader);rows=list(reader)
    assert header.count('Source File')==2
    first=header.index('Source File');second=header.index('Source File',first+1);line=header.index('Source Line')
    inline=next(row for row in rows if row[first]=='shared_count.h' and row[second]=='deployment.h' and row[line].isdecimal())
    ambiguous=next(row for row in rows if row[first]=='uwvm_runtime.default.cpp' and row[line].isdecimal())
    unknown=next(row for row in rows if row[first]=='[Unknown source file]')
    source=out/'source';source.mkdir();manifest={}
    def create(relative,number,text):
        path=source/relative;path.parent.mkdir(parents=True,exist_ok=True)
        path.write_text('\n'*(number-1)+text+'\n');manifest[relative]=sha(path)
    create('shared_count.h',int(inline[line]),'correct sampled inline source')
    create('deployment.h',int(inline[line]),'WRONG containing-function source')
    create('a/uwvm_runtime.default.cpp',int(ambiguous[line]),'ambiguous first source')
    create('b/uwvm_runtime.default.cpp',int(ambiguous[line]),'ambiguous second source')
    manifest_path=out/'manifest.json';manifest_path.write_text(json.dumps(manifest))
    runtime=out/'parser-test-runtime';runtime.write_bytes(b'parser fixture only; not an executable')
    (out/'full-build-results.json').write_text(json.dumps(dict(binary_sha256=sha(runtime),full_source_manifest=str(manifest_path),
        full_source_manifest_sha256=sha(manifest_path),source_root=str(source))))
    (out/'profile.json').write_text(json.dumps(dict(complete=True,runtime_sha256=sha(runtime),scope=__doc__)))
    (out/'prepare.json').write_text(json.dumps(dict(inputs={str(source/name):digest for name,digest in manifest.items()})))
    for provider in ['wasm','native']:
        for analysis in ['hotspots','uarch-exploration']:
            with (out/f'lines-{provider}-{analysis}.stdout').open('w') as stream:
                writer=csv.writer(stream,delimiter='\t');writer.writerows([header,inline,ambiguous,unknown])
    runner=ROOT/'tools/ros_scene/profile_artifact_sources.py'
    result=subprocess.run(['/usr/bin/python3',str(runner),'--profile',str(out),'--runtime',str(runtime)],capture_output=True,text=True)
    (out/'runner.stdout').write_text(result.stdout);(out/'runner.stderr').write_text(result.stderr)
    assert result.returncode==0,result.stderr
    report=json.loads((out/'source-line-attribution.json').read_text())
    for entry in report['reports']:
        joined=json.loads(Path(entry['joined_path']).read_text())
        assert len(joined)==3 and entry['preserved_rows']==1
        assert joined[0]['Source File']=='shared_count.h' and joined[0]['Function Source File']=='deployment.h'
        assert joined[0]['source_text']=='correct sampled inline source'
        assert int(joined[0]['Source Line'])==int(inline[line])
        assert 'preserved_source' not in joined[1] and 'preserved_source' not in joined[2]
    receipt=dict(complete=True,scope=__doc__,sample=str(args.sample.resolve()),sample_sha256=sha(args.sample),
        runner_sha256=sha(runner),test_sha256=sha(__file__),checks=['sampled source wins over function source',
            'line number retained','ambiguous basename unattributed','unknown JIT location unattributed'],
        reports_checked=len(report['reports']),cgroup=str(GATE),memory_events=(GATE/'memory.events').read_text())
    (out/'regression-results.json').write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps(receipt,indent=2))

if __name__=='__main__':main()
