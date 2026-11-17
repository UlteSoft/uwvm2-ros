#!/usr/bin/env python3
"""Compare compiled recording payload validators to retained ROS1 serialization and Fast-CDR.

All native variants use -O3 -g. Copy compiler-discovered repository/upstream
headers, the exact upstream serialization implementation, and the Fast-CDR
shared library before compiling. No ROS node, network or device is started.
The external native libraries use their own allocators; forced allocation
failure is scoped only to the VM validator calls. This is a finite four-schema
qualification, not a generic ROS type or whole-VM portability claim.
"""
import argparse,hashlib,json,os,shlex,shutil,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736'and (GROUP/'memory.swap.max').read_text().strip()=='0'
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--ros1',type=Path,default=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-ecosystem/sources/35e08e35aed10833c3cf82d2/roscpp_core-noetic-devel')
    parser.add_argument('--ros2',type=Path,default=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security/communication-ros-sysroot/opt/ros/jazzy')
    parser.add_argument('--boost',type=Path,default=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security/recording-boost-dependency/extracted/usr/include')
    args=parser.parse_args();guard();out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    ros1=args.ros1.resolve();ros2=args.ros2.resolve();fastcdr=(ros2/'include/fastcdr').resolve();boost=args.boost.resolve()
    local=Path('test/0084.ros-runtime/recording_codecs.cpp');upstream=ros1/'roscpp_serialization/src/serialization.cpp';library=(ros2/'lib/libfastcdr.so').resolve()
    retained=out/'source';mappings=[(ROOT,retained),(ros1,retained/'vendor/ros1'),(fastcdr,retained/'vendor/fastcdr'),(boost,retained/'vendor/boost')]
    def mapped(path):
        for original,copy in mappings:
            if path.is_relative_to(original):return copy/path.relative_to(original)
        raise AssertionError('Unexpected non-system dependency '+str(path))
    includes=[ROOT/'src',ROOT/'third-parties/fast_io/include',fastcdr,boost,*[ros1/name/'include'for name in ('roscpp_serialization','cpp_common','rostime','roscpp_traits')]]
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
    variants=[('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc13','/usr/bin/g++-13',[])]
    report=dict(complete=False,scope=__doc__,cgroup=str(GROUP),inode=GROUP.stat().st_ino,boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),memory_max=68719476736,swap_max=0,
        affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text(),commands=[],inputs={},snapshots={},system_headers={},toolchain={})
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,argv,env=None):
        guard();start=time.monotonic();p=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=240)
        (out/(name+'.stdout')).write_text(p.stdout);(out/(name+'.stderr')).write_text(p.stderr)
        report['commands'].append(dict(name=name,argv=argv,exit=p.returncode,seconds=time.monotonic()-start));save();print(name,p.returncode,flush=True)
        if p.returncode:raise RuntimeError(p.stderr[-7000:])
        return p.stdout
    success=False
    try:
        dependencies={Path(__file__).resolve(),upstream,ROOT/local}
        for name,compiler,extra in variants:
            report['toolchain'][compiler]=sha(compiler)
            for index,source in enumerate((ROOT/local,upstream)):
                # Boost marks some of its own headers as system headers. -MM
                # omits their transitive includes; collect -M and distinguish
                # actual compiler/libc headers from the copied vendor roots.
                result=run(name+'-dependencies-'+str(index),[compiler,*flags,*extra,*['-I'+str(p)for p in includes],'-M','-MT','codec',str(source)])
                words=shlex.split(result.replace('\\\n',' '));assert words[0]=='codec:'
                for word in words[1:]:
                    path=Path(word).resolve()
                    if any(path.is_relative_to(root)for root,_ in mappings):dependencies.add(path)
                    else:
                        assert path.is_relative_to('/usr'),path
                        report['system_headers'][str(path)]=sha(path)
        for original in sorted(dependencies):
            target=mapped(original);target.parent.mkdir(parents=True,exist_ok=True);digest=sha(original);shutil.copyfile(original,target);assert sha(target)==digest
            report['inputs'][str(original)]=digest;report['snapshots'][str(target.relative_to(retained))]=digest
        target=out/'libfastcdr.so.2';shutil.copyfile(library,target);report['inputs'][str(library)]=sha(library);report['library_sha256']=sha(target)
        report['source_files']=len(report['snapshots']);save()
        for name,compiler,extra in variants:
            binary=out/name
            run(name+'-compile',[compiler,*flags,*extra,*['-I'+str(mapped(p))for p in includes],str(retained/local),str(mapped(upstream)),'-o',str(binary),str(target),'-Wl,-rpath,$ORIGIN','-Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc'])
            result=run(name,[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1'));assert 'passed 3 native recording codec groups'in result
        success=True
    finally:
        report['inputs_unchanged']=all(sha(p)==v for p,v in report['inputs'].items())
        report['snapshots_unchanged']=all(sha(retained/p)==v for p,v in report['snapshots'].items())
        report['system_headers_unchanged']=all(sha(p)==v for p,v in report['system_headers'].items())
        report['library_unchanged']=not(out/'libfastcdr.so.2').exists()or sha(out/'libfastcdr.so.2')==report.get('library_sha256')
        report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['inputs_unchanged']and report['snapshots_unchanged']and report['library_unchanged']and report['system_headers_unchanged'];save()
    assert report['complete']
if __name__=='__main__':main()
