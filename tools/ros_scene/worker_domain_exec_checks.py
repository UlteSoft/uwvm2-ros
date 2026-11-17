#!/usr/bin/env python3
"""Qualify namespace ownership, immediate exec and stale identity-map refusal.

Real Linux processes exercise nested/daemonized descendant death, owner death,
rejected launch, inherited SIGCHLD-ignore, removal of the VM address space after
exec and mapping with an already-dead birth identity. Clang and GCC compile with
-O3 -g. The prior mapping helper is an expected-failure negative control. These
are native lifecycle checks, not vendor ABI or physical-stop qualification.
"""
import argparse
import json
import os
from pathlib import Path
import resource
import shutil
import subprocess
import time

from module_exchange_performance import ROOT, GATE, sha, verify_gate


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root',type=Path,required=True)
    parser.add_argument('--reference-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True,exist_ok=False)
    roots=dict(corrected=args.source_root.resolve(),reference=args.reference_root.resolve())
    manifests={kind:json.loads((root/'source-manifest.json').read_text()) for kind,root in roots.items()}
    assert all(sha(roots[kind]/name)==value for kind,rows in manifests.items() for name,value in rows.items())
    pins={str(Path(__file__).resolve()):sha(__file__)}
    for name in ('linux_process_domain_exec.cpp','linux_process_domain_mapping.cpp'):
        source=ROOT/'test/0084.ros-runtime'/name;shutil.copyfile(source,out/name)
        pins[str(source)]=pins[str(out/name)]=sha(source)
    report=dict(complete=False,scope=__doc__,source_roots={k:str(v) for k,v in roots.items()},inputs=pins,
                cgroup=str(GATE),memory_max=68719476736,swap_max=0,
                events_before=(GATE/'memory.events').read_text(),commands=[])

    def save(): (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')

    def run(label,command,expected=0):
        verify_gate();command=list(map(str,command));started=time.monotonic()
        done=subprocess.run(command,capture_output=True,timeout=90,env=dict(os.environ))
        (out/(label+'.stdout')).write_bytes(done.stdout);(out/(label+'.stderr')).write_bytes(done.stderr)
        report['commands'].append(dict(label=label,argv=command,exit=done.returncode,expected_exit=expected,seconds=time.monotonic()-started))
        save();assert done.returncode==expected,(label,done.stderr[-4000:]);print(label,done.returncode,flush=True)

    success=False
    try:
        for label,compiler,root,fixture,defines,expected in [
            ('reference-mapping','/usr/bin/clang++-22',roots['reference'],'linux_process_domain_mapping.cpp',['-DUWVM_TEST_REFERENCE_MAPPING_WITHOUT_IDENTITY'],-6),
            *[(compiler_label+'-'+name,compiler,roots['corrected'],'linux_process_domain_'+name+'.cpp',[],0)
              for compiler_label,compiler in [('clang','/usr/bin/clang++-22'),('gcc','/usr/bin/g++-13')]
              for name in ('exec','mapping')]]:
            binary=out/label
            run(label+'-compile',[compiler,'-std=c++23','-O3','-g','-UNDEBUG','-Wall','-Wextra','-Werror',
                                  '-I'+str(root/'src'),*defines,out/fixture,'-o',binary])
            pins[str(binary)]=sha(binary);run(label+'-run',[binary],expected)
        success=True
    finally:
        verify_gate()
        report['inputs_unchanged']=all(sha(name)==digest for name,digest in pins.items())
        report['sources_unchanged']=all(sha(roots[kind]/name)==value for kind,rows in manifests.items() for name,value in rows.items())
        report['events_after']=(GATE/'memory.events').read_text()
        report['complete']=success and report['inputs_unchanged'] and report['sources_unchanged'];save()
    assert report['complete']


if __name__=='__main__':main()
