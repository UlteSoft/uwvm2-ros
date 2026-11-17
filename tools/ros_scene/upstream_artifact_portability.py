#!/usr/bin/env python3
"""Run an unmodified upstream XML parser over exact immutable scene artifacts.

The pinned BehaviorTree.CPP release archive supplies TinyXML2 and five real XML
fixtures. Native, sanitized native and Wasm compare canonical parse summaries
for four chunk sizes; two explicit malformed/empty fixtures test refusal.
Only input loading is adapted. No BehaviorTree execution, plugin loading,
filesystem include resolution, model inference or whole-package claim follows.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tarfile
import time

ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
IDENTITY='fa917e4ea95e0210a67d0fd6'
RELEASE='behaviortree_cpp_v4-release-upstream-4.11.1'


def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset(set(range(8,16)))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode',choices=['prepare','execute','all'],default='all')
    parser.add_argument('--runtime',type=Path)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--corpus',type=Path,default=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-ecosystem/supplemental')
    parser.add_argument('--sysroot',type=Path,default=ROOT.parent/'uwvm2-ros-sdk/.tmp/wasmmte-sdk-integration/stable-sysroots-tls-nomte/output/noeh/wasm32-wasip1')
    parser.add_argument('--builtins',type=Path,default=ROOT.parent/'uwvm2-ros-sdk/.tmp/wasmmte-sdk-integration/stable-sysroots-tls-nomte/output/clang/23/lib/wasm32-unknown-wasip1/libclang_rt.builtins.a')
    parser.add_argument('--clang',default='/usr/bin/clang++-22')
    parser.add_argument('--wasm-ld',default='wasm-ld')
    args=parser.parse_args();guard()
    if args.mode!='prepare' and args.runtime is None:parser.error('--runtime is required for execution')
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    if args.mode=='execute':record=json.loads((out/'results.json').read_text());assert record['preparation_complete']
    else:
        assert not (out/'sources').exists(),'Use a fresh preparation directory'
        record=dict(complete=False,preparation_complete=False,scope=__doc__,cgroup=str(GATE),inode=GATE.stat().st_ino,
                    memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),commands=[],fixtures=[])
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(name,command,env=None):
        guard();start=time.monotonic();result=subprocess.run([str(x) for x in command],cwd=ROOT,capture_output=True,timeout=180,env=env)
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        record['commands'].append(dict(name=name,argv=[str(x) for x in command],exit=result.returncode,seconds=time.monotonic()-start));save()
        assert result.returncode==0,(name,result.stderr.decode(errors='replace')[-5000:]);print('PASS',name,flush=True)
        return result.stdout
    def unchanged():
        for name,digest in record['input_sha256'].items():assert sha(out/name)==digest,'Changed qualification input: '+name
        for name,digest in record['toolchain_sha256'].items():assert sha(name)==digest,'Changed toolchain: '+name
    if args.mode!='execute':
        receipt=args.corpus/'receipts'/(IDENTITY+'.json');received=json.loads(receipt.read_text())
        archive=args.corpus/'archives'/(IDENTITY+'.tar.gz');assert sha(archive)==received['archive']['sha256']
        (out/'sources').mkdir()
        with tarfile.open(archive) as bundle:bundle.extractall(out/'sources',filter='data')
        upstream=out/'sources'/RELEASE;parser_dir=upstream/'3rdparty/tinyxml2'
        shutil.copyfile(receipt,out/'upstream.acquisition.json')
        record['upstream']=dict(source=received['source'],archive=received['archive'],receipt_sha256=sha(receipt))
        paths=[('parent','tests/trees/parent_no_include.xml'),('log','examples/test_files/log_test.xml'),
               ('includes','examples/test_files/subtree_test.xml'),('check','examples/test_files/Check.xml'),
               ('talk','examples/test_files/subtrees/Talk.xml')]
        for alias,path in paths:
            source=upstream/path;record['fixtures'].append(dict(alias=alias,path=str(source),size=source.stat().st_size,
                generation=23,digest=sha(source),valid=True,origin=path))
        invalid=out/'malformed.xml';invalid.write_bytes(b'<root><BehaviorTree ID="truncated">')
        empty=out/'empty.xml';empty.write_bytes(b'')
        for alias,path in [('malformed',invalid),('empty',empty)]:
            record['fixtures'].append(dict(alias=alias,path=str(path),size=path.stat().st_size,generation=23,
                                          digest=sha(path),valid=False,origin='explicit negative fixture'))
        header=['#pragma once','#include <cstddef>','#include <cstdint>',
                'struct artifact_fixture { char const* alias;char const* path;std::uint64_t generation,size;unsigned char digest[32];bool valid; };',
                'inline constexpr artifact_fixture artifact_fixtures[]{']
        for fixture in record['fixtures']:
            digest=','.join('0x'+fixture['digest'][n:n+2] for n in range(0,64,2))
            header.append('{'+','.join([json.dumps(fixture['alias']),json.dumps(fixture['path']),str(fixture['generation']),
                str(fixture['size']),'{'+digest+'}','true' if fixture['valid'] else 'false'])+'},')
        header+=['};'];(out/'artifact_fixtures.h').write_text('\n'.join(header)+'\n')
        shutil.copyfile(ROOT/'test/0084.ros-runtime/upstream_artifact_xml.cc',out/'upstream_artifact_xml.cc')
        compiler=shutil.which(args.clang);linker=shutil.which(args.wasm_ld);assert compiler and linker
        libraries=[args.sysroot/'lib/wasm32-wasip1'/name for name in ['libc++.a','libc++abi.a','libc.a']]+[args.builtins]
        record['toolchain_sha256']={str(path.resolve()):sha(path) for path in [Path(compiler),Path(linker),*libraries]}
        common=[compiler,'-std=c++17','-O1','-g','-ffunction-sections','-fdata-sections','-fno-exceptions','-fno-rtti',
                '-I'+str(parser_dir),'-I'+str(out),parser_dir/'tinyxml2.cpp',out/'upstream_artifact_xml.cc']
        run('compiler-version',[compiler,'--version'])
        run('compile-native',[*common,'-Wl,--gc-sections','-o',out/'native'])
        baseline=run('native',[out/'native']);assert baseline.endswith(b'checks:28\n')
        run('compile-native-sanitized',[*common,'-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wl,--gc-sections','-o',out/'native-sanitized'])
        assert run('native-sanitized',[out/'native-sanitized'],dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))==baseline
        guest=[*common,'--target=wasm32-wasip1','--sysroot='+str(args.sysroot.resolve()),'-nostartfiles',
               '-fuse-ld='+linker,'-Wl,--no-entry','-Wl,--export=_start','-Wl,--max-memory=16777216',
               '-Wl,--gc-sections','-nodefaultlibs',*libraries]
        run('compile-wasm',[*guest,'-o',out/'guest.wasm'])
        run('compile-wasm-denied',[*guest,'-DEXPECT_ARTIFACT_DENIAL','-o',out/'denied.wasm'])
        record['imports']={}
        for name in ['guest','denied']:
            run(name+'-disassemble',['wasm2wat','--enable-all',out/(name+'.wasm'),'-o',out/(name+'.wat')])
            imports=re.findall(r'\(import "([^"]+)" "([^"]+)"',(out/(name+'.wat')).read_text())
            assert all(module=='uwvm_ros_v2' and function in ['capability.open','artifact.open','artifact.read','artifact.describe','resource.close'] or
                       module=='wasi_snapshot_preview1' and function in ['fd_write','fd_close','fd_seek','fd_fdstat_get',
                           'clock_time_get','fd_prestat_get','fd_prestat_dir_name','proc_exit']
                       for module,function in imports),imports
            record['imports'][name]=imports
        inputs=[out/'upstream_artifact_xml.cc',out/'artifact_fixtures.h',out/'upstream.acquisition.json',
                parser_dir/'tinyxml2.cpp',parser_dir/'tinyxml2.h',out/'guest.wasm',out/'denied.wasm',out/'native',out/'native-sanitized',
                *[Path(fixture['path']) for fixture in record['fixtures']]]
        record['input_sha256']={str(path.relative_to(out)):sha(path) for path in inputs}
        record['runner_sha256']=sha(__file__);record['fixture_source_sha256']=sha(ROOT/'test/0084.ros-runtime/upstream_artifact_xml.cc')
        record['preparation_complete']=True;save()
    unchanged()
    if args.mode!='prepare':
        runtime=args.runtime.resolve();record['runtime']=dict(path=str(runtime),sha256=sha(runtime))
        def policy(deny=False):
            flags=['--ros-scene-create','xml','--ros-scene-bind','xml','main',
                   '--ros-scene-input-budget','xml',str(sum(f['size'] for f in record['fixtures'])),str(len(record['fixtures']))]
            for name in ['capability.open','artifact.open','artifact.read','artifact.describe','resource.close']:
                flags+=['--ros-scene-interface','xml',name,'deny' if deny and name=='artifact.open' else 'allow']
            for fixture in record['fixtures']:
                alias=fixture['alias']
                flags+=['--ros-scene-grant','xml',alias,'configuration',alias,'sealed-input','artifact.open,artifact.read,artifact.describe,resource.close',
                        '--ros-scene-input','xml',alias,fixture['path'],fixture['digest'],str(fixture['generation']),str(fixture['size']),'64',
                        '--ros-scene-limit','xml',alias,'max-handles','4','--ros-scene-limit','xml',alias,'max-resources','1']
            # libc's stdout plumbing imports no path_open and receives no file
            # or directory capability. Only stdout write is granted explicitly.
            flags+=['--wasip1-single-create','main','--wasip1-single-enable','main']
            for module,function in record['imports']['guest']:
                if module=='wasi_snapshot_preview1':flags+=['--wasip1-single-interface','main',function,'allow' if function=='fd_write' else 'deny']
            flags+=['--wasip1-single-grant','main','stdout.write']
            return flags
        baseline=(out/'native.stdout').read_bytes()
        for engine in ['int','aot']:
            flags=[str(runtime),'--wasm-set-main-module-name','main','--runtime-'+engine]
            assert run('vm-'+engine,[*flags,*policy(),'--run',out/'guest.wasm'])==baseline
            assert run('vm-denied-'+engine,[*flags,*policy(True),'--run',out/'denied.wasm'])==b'checks:28\n'
        assert sha(runtime)==record['runtime']['sha256'];record['complete']=True
    unchanged();assert sha(__file__)==record['runner_sha256']
    assert sha(ROOT/'test/0084.ros-runtime/upstream_artifact_xml.cc')==record['fixture_source_sha256']
    record['events_after']=(GATE/'memory.events').read_text();save()


if __name__=='__main__':main()
