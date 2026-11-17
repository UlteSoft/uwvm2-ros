#!/usr/bin/env python3
"""Compare bounded message conversion with independently compiled ROS1 and Fast-CDR.

Freeze upstream source, build Fast-CDR 2.2.x with -O3 -g, and retain all local
header dependencies. This is a structural serialization oracle, not a native
ROS endpoint or physical device qualification. All execution uses the existing
64 GiB, zero-swap cgroup.
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
from bounded_message_definition import compile_schema

def fixtures(out):
    nodes=[]
    def add(kind,bound=0,flags=(),members=()):
        nodes.append(dict(kind=kind,bound=bound,flags=list(flags),members=[dict(name=name,type=child)for name,child in members]));return len(nodes)-1
    boolean=add('bool');counter=add('int16');ticks=add('uint64');scalar=add('float64',flags=['finite'])
    point=add('record',members=[(name,scalar)for name in ('x','y','z')]);points=add('sequence',4,members=[('element',point)])
    label=add('string',32,['unicode','no-nul']);byte=add('uint8');data=add('sequence',4096,members=[('element',byte)])
    add('record',members=[('flag',boolean),('counter',counter),('ticks',ticks),('points',points),('label',label),('data',data),('tail',boolean)])
    for layout in ('cdr','ros1'):
        document=dict(layout=layout,message_bytes=65536,work=8192,depth=4,nodes=nodes)
        (out/(layout+'.bin')).write_bytes(compile_schema(document));(out/(layout+'.json')).write_text(json.dumps(document,indent=2)+'\n')
    document=dict(layout='cdr',message_bytes=65536,work=64,depth=2,nodes=[dict(kind='wstring',bound=16,flags=['unicode','no-nul'],members=[]),dict(kind='record',bound=0,flags=[],members=[dict(name='value',type=0)])])
    (out/'wide.bin').write_bytes(compile_schema(document))

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args();guard()
    temporary=ROOT.parent/'uwvm2-ros-sdk/.tmp';out=args.output.resolve();assert out.is_relative_to(temporary.resolve());out.mkdir(parents=True,exist_ok=False)
    ros=temporary/'ros-ecosystem/sources/35e08e35aed10833c3cf82d2/roscpp_core-noetic-devel'
    vendor=temporary/'ros-ecosystem/sources/8ca5449151a74e6dfa28df93/Fast-CDR-2.2.x'
    boost=temporary/'ros-scene-security/recording-boost-dependency/extracted/usr/include'
    source=out/'source';native=source/'vendor/fastcdr';build=out/'fastcdr-build'
    local=Path('test/0084.ros-runtime/bounded_message_native.cpp');serializer=ros/'roscpp_serialization/src/serialization.cpp'
    report=dict(complete=False,scope=__doc__,commands=[],inputs={},snapshots={},generated={},binaries={},
                cgroup=str(GROUP),memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,env=None):
        guard();start=time.monotonic();done=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=300)
        (out/(label+'.stdout')).write_text(done.stdout);(out/(label+'.stderr')).write_text(done.stderr)
        report['commands'].append(dict(label=label,argv=argv,exit=done.returncode,seconds=time.monotonic()-start));save();print(label,done.returncode,flush=True)
        assert done.returncode==0,(label,done.stderr[-7000:]);return done.stdout
    def copy(original,target):
        target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(original,target)
        report['inputs'][str(original)]=sha(original);report['snapshots'][str(target.relative_to(source))]=sha(target)
    success=False
    try:
        receipt=temporary/'ros-ecosystem/receipts/8ca5449151a74e6dfa28df93.json';report['inputs'][str(receipt)]=sha(receipt)
        for path in sorted(vendor.rglob('*')):
            assert not path.is_symlink(),path
            if path.is_file():copy(path,native/path.relative_to(vendor))
        compiler='/usr/bin/clang++-22';report['inputs'][compiler]=sha(compiler)
        run('configure-native',['/usr/bin/cmake','-S',str(native),'-B',str(build),'-DCMAKE_CXX_COMPILER='+compiler,
            '-DCMAKE_BUILD_TYPE=RelWithDebInfo','-DCMAKE_CXX_FLAGS_RELWITHDEBINFO=-O3 -g','-DBUILD_TESTING=OFF','-DBUILD_DOCUMENTATION=OFF',
            '-DBUILD_SHARED_LIBS=OFF','-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'])
        run('build-native',['/usr/bin/cmake','--build',str(build),'--parallel','2'])
        recipes=json.loads((build/'compile_commands.json').read_text())
        assert recipes and all('-O3' in row['command'] and '-g' in row['command'] for row in recipes)
        report['native_recipes']=recipes
        includes=[ROOT/'src',ROOT/'third-parties/fast_io/include',native/'include',build/'include',boost,
                  *[ros/name/'include'for name in ('roscpp_serialization','cpp_common','rostime','roscpp_traits')]]
        mappings=[(ROOT,source),(ros,source/'vendor/ros1'),(boost,source/'vendor/boost')]
        def mapped(path):
            if path.is_relative_to(source) or path.is_relative_to(build):return path
            for root,target in mappings:
                if path.is_relative_to(root):return target/path.relative_to(root)
            raise AssertionError(path)
        flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
        dependencies={ROOT/local,serializer,Path(__file__).resolve(),ROOT/'tools/ros_scene/bounded_message_definition.py'}
        for index,file in enumerate((ROOT/local,serializer)):
            text=run('dependencies-'+str(index),[compiler,*flags,*['-I'+str(path)for path in includes],'-M','-MT','oracle',str(file)])
            parsed=shlex.split(text.replace('\\\n',' '));assert parsed[0]=='oracle:'
            for item in parsed[1:]:
                path=Path(item).resolve()
                if any(path.is_relative_to(root)for root,_ in mappings):dependencies.add(path)
                elif path.is_relative_to(build):report['generated'][str(path)]=sha(path)
                elif path.is_relative_to(source):pass
                else:assert path.is_relative_to('/usr');report['inputs'][str(path)]=sha(path)
        for original in sorted(dependencies):copy(original,mapped(original))
        fixture=out/'fixtures';fixture.mkdir();fixtures(fixture)
        report['fixtures']={str(path):sha(path)for path in fixture.iterdir()}
        libraries=list(build.rglob('libfastcdr.a'));assert len(libraries)==1;report['binaries']['fastcdr']=sha(libraries[0])
        for label,extra in [('sanitized',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('optimized',[])]:
            binary=out/label;run(label+'-compile',[compiler,*flags,*extra,*['-I'+str(mapped(path))for path in includes],str(source/local),str(mapped(serializer)),str(libraries[0]),'-o',str(binary)])
            report['binaries'][label]=sha(binary)
            result=run(label,[str(binary),str(fixture)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1'));assert 'PASS native bounded-message oracle: 516' in result
        success=True
    finally:
        report['inputs_unchanged']=all(sha(path)==digest for path,digest in report['inputs'].items())
        report['snapshots_unchanged']=all(sha(source/path)==digest for path,digest in report['snapshots'].items())
        report['generated_unchanged']=all(sha(path)==digest for path,digest in report['generated'].items())
        report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['inputs_unchanged'] and report['snapshots_unchanged'] and report['generated_unchanged'];save()
    assert report['complete']

if __name__=='__main__':main()
