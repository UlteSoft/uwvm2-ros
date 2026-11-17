#!/usr/bin/env python3
"""Execute independently generated acquired ROS interface fixtures through the C++ validator.

Success qualifies only structural messages under explicit test bounds. It does
not grant capabilities or establish package/native/hardware semantic coverage.
"""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import time
from platform_agilex_checks import ROOT,GROUP,guard,sha

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--corpus',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();guard();corpus=args.corpus.resolve();proof=json.loads((corpus/'results.json').read_text());assert proof['complete']
    assert sha(corpus/'cases.bin')==proof['bundle_sha256']
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    snapshot=out/'source';source=Path('test/0084.ros-runtime/bounded_message_corpus.cpp');compiler='/usr/bin/clang++-22'
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer']
    record=dict(complete=False,scope=__doc__,cases=proof['cases'],commands=[],inputs={},snapshots={},cgroup=str(GROUP),
                memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=300,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-start));save();print(label,result.returncode,flush=True)
        assert result.returncode==0,(label,result.stderr[-7000:]);return result.stdout
    success=False
    try:
        text=run('dependencies',[compiler,*flags,'-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include'),'-MM','-MT','corpus',str(ROOT/source)])
        parsed=shlex.split(text.replace('\\\n',' '));assert parsed[0]=='corpus:'
        for path in sorted({Path(p).resolve()for p in parsed[1:]}|{Path(__file__).resolve()}):
            assert path.is_relative_to(ROOT);name=path.relative_to(ROOT);target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
            record['inputs'][str(path)]=sha(path);record['snapshots'][str(name)]=sha(target)
        for path in [Path(compiler),corpus/'results.json',corpus/'cases.bin',corpus/'source-pins.json']:record['inputs'][str(path)]=sha(path)
        record['corpus_sources_unchanged']=all(sha(path)==digest for path,digest in json.loads((corpus/'source-pins.json').read_text()).items());assert record['corpus_sources_unchanged']
        binary=out/'corpus';run('compile',[compiler,*flags,'-I'+str(snapshot/'src'),'-I'+str(snapshot/'third-parties/fast_io/include'),str(snapshot/source),'-o',str(binary)])
        record['binary_sha256']=sha(binary);text=run('corpus',[str(binary),str(corpus/'cases.bin')]);assert f"PASS acquired ROS shape corpus: {proof['cases']} cases" in text
        success=True
    finally:
        record['inputs_unchanged']=all(sha(path)==digest for path,digest in record['inputs'].items())
        record['snapshots_unchanged']=all(sha(snapshot/path)==digest for path,digest in record['snapshots'].items())
        record['events_after']=(GROUP/'memory.events').read_text();record['complete']=success and record['inputs_unchanged'] and record['snapshots_unchanged'];save()
    assert record['complete']

if __name__=='__main__':main()
