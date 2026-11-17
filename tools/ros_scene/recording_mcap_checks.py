#!/usr/bin/env python3
"""Qualify the closed ROS MCAP codec with exact retained native source.

All four variants use -O3 -g. Core checks exercise actual zero-allocation paths,
reserved finalization and malformed/truncated input. Native variants use the
retained upstream MCAP reader/writer in both directions, without filesystem I/O.
Its external allocation behavior is not claimed to use the VM allocator. This
does not qualify recording/replay imports, persistence or compressed containers.
"""
import argparse,hashlib,json,os,shlex,shutil,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
UPSTREAM=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-ecosystem/sources/dede5da75d08ee49a231d180/depthai-core-ros-old-devel/include/3rdparty'
def sha(p):
    with Path(p).open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest()
def gate():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',required=True,type=Path);args=parser.parse_args();gate()
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    retained=out/'source';source=Path('test/0084.ros-runtime/recording_mcap.cpp')
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
    variants=[(kind+'-'+name,compiler,extra+(['-DRECORDING_NATIVE_MCAP']if kind=='native' else []))
              for kind in ['core','native']for name,compiler,extra in [('asan','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc','/usr/bin/g++-13',[])]]
    report=dict(complete=False,scope=__doc__,cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,
                events_before=(GATE/'memory.events').read_text(),affinity=sorted(os.sched_getaffinity(0)),commands=[],inputs={},retained={},toolchain={})
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,argv):
        gate();start=time.monotonic();r=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=240,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))
        (out/(name+'.stdout')).write_text(r.stdout);(out/(name+'.stderr')).write_text(r.stderr)
        report['commands'].append(dict(name=name,argv=argv,exit=r.returncode,seconds=time.monotonic()-start));save()
        assert not r.returncode,(name,r.stderr[-6000:]);print('PASS '+name,flush=True);return r.stdout
    success=False
    try:
        dependencies=set()
        for name,compiler,extra in variants:
            report['toolchain'][compiler]=sha(compiler)
            data=run(name+'-dependencies',[compiler,*flags,*extra,'-I'+str(ROOT/'src'),'-I'+str(UPSTREAM),'-MM','-MT','codec',str(ROOT/source)])
            words=shlex.split(data.replace('\\\n',' '));assert words[0]=='codec:'
            dependencies.update(Path(p).resolve()for p in words[1:])
        for path in sorted(dependencies):
            if path.is_relative_to(ROOT):name=path.relative_to(ROOT)
            else:assert path.is_relative_to(UPSTREAM);name=Path('vendor')/path.relative_to(UPSTREAM)
            target=retained/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
            report['inputs'][str(path)]=sha(path);report['retained'][str(name)]=sha(target)
        runner=retained/'tools/ros_scene/recording_mcap_checks.py';runner.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(__file__,runner)
        report['inputs'][str(Path(__file__).resolve())]=sha(__file__);report['retained'][str(runner.relative_to(retained))]=sha(runner);save()
        for name,compiler,extra in variants:
            binary=out/name
            run(name+'-compile',[compiler,*flags,*extra,'-I'+str(retained/'src'),'-I'+str(retained/'vendor'),str(retained/source),
                '-Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc','-o',str(binary)])
            output=run(name,[str(binary)])
            assert ('passed 4 recording MCAP groups including actual upstream reader/writer'if name.startswith('native')else'passed 3 recording MCAP security groups')in output
        success=True
    finally:
        report['inputs_unchanged']=all(sha(p)==digest for p,digest in report['inputs'].items())
        report['retained_unchanged']=all(sha(retained/p)==digest for p,digest in report['retained'].items())
        report['events_after']=(GATE/'memory.events').read_text();report['complete']=success and report['inputs_unchanged'] and report['retained_unchanged'];save()
    assert report['complete']
if __name__=='__main__':main()
