#!/usr/bin/env python3
"""Run reproducible coverage-guided parser checks seeded by acquired ROS shapes."""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import struct
import subprocess
import time
from platform_agilex_checks import ROOT,GROUP,guard,sha

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--corpus',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();guard();out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    corpus=args.corpus.resolve();proof=json.loads((corpus/'results.json').read_text());assert proof['complete'] and sha(corpus/'cases.bin')==proof['bundle_sha256']
    source=Path('test/0084.ros-runtime/bounded_message_fuzz.cpp');snapshot=out/'source';compiler='/usr/bin/clang++-22'
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-fsanitize=fuzzer,address,undefined','-fno-omit-frame-pointer']
    report=dict(complete=False,scope=__doc__,commands=[],inputs={str(corpus/'results.json'):sha(corpus/'results.json'),str(corpus/'cases.bin'):sha(corpus/'cases.bin'),compiler:sha(compiler)},
                snapshots={},cgroup=str(GROUP),memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();done=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=240,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))
        (out/(label+'.stdout')).write_text(done.stdout);(out/(label+'.stderr')).write_text(done.stderr)
        report['commands'].append(dict(label=label,argv=argv,exit=done.returncode,seconds=time.monotonic()-start));save();print(label,done.returncode,flush=True)
        assert done.returncode==0,(label,done.stderr[-7000:]);return done.stdout
    success=False
    try:
        text=run('dependencies',[compiler,*flags,'-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include'),'-MM','-MT','fuzz',str(ROOT/source)])
        parsed=shlex.split(text.replace('\\\n',' '));assert parsed[0]=='fuzz:'
        for path in sorted({Path(p).resolve()for p in parsed[1:]}|{Path(__file__).resolve()}):
            assert path.is_relative_to(ROOT);name=path.relative_to(ROOT);target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
            report['inputs'][str(path)]=sha(path);report['snapshots'][str(name)]=sha(target)
        seeds=out/'seeds';seeds.mkdir();count=0
        with (corpus/'cases.bin').open('rb') as stream:
            def blob():
                header=stream.read(8)
                if not header:return None
                assert len(header)==8;size=struct.unpack('<Q',header)[0];assert size<=16*1024*1024;data=stream.read(size);assert len(data)==size;return data
            index=0
            while blob() is not None:
                description=blob();message=blob();blob()
                seed=struct.pack('<I',len(description))+description+message
                if index%100==0 and len(seed)<=16384:
                    (seeds/str(index)).write_bytes(seed);count+=1
                index+=1
        assert count;report['initial_seeds']={path.name:sha(path)for path in seeds.iterdir()}
        binary=out/'fuzz';run('compile',[compiler,*flags,'-I'+str(snapshot/'src'),'-I'+str(snapshot/'third-parties/fast_io/include'),str(snapshot/source),'-o',str(binary)])
        report['binary_sha256']=sha(binary)
        run('fuzz',[str(binary),str(seeds),'-seed=1','-runs=100000','-max_len=16384','-timeout=10','-rss_limit_mb=2048','-artifact_prefix='+str(out)+'/'])
        success=True
    finally:
        report['inputs_unchanged']=all(sha(path)==digest for path,digest in report['inputs'].items())
        report['snapshots_unchanged']=all(sha(snapshot/path)==digest for path,digest in report['snapshots'].items())
        report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['inputs_unchanged'] and report['snapshots_unchanged'];save()
    assert report['complete']

if __name__=='__main__':main()
