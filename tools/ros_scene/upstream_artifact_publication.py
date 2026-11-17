#!/usr/bin/env python3
"""Qualify real upstream XML through complete bounded artifact.publish.

The pinned ROS release archive supplies unmodified TinyXML2 and five XML files.
The native reference emits exact serialized bytes for pretty and compact modes;
explicit UTF-8/escaping and empty-document fixtures extend the boundary cases.
Each guest output uses only the explicitly granted volatile cache operations.
These fixtures are embedded, never resolved as host paths by the guest. This is
an XML serialization slice, not BehaviorTree execution or disk persistence.
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
FUNCTIONS=['capability.open','artifact.begin','artifact.append','artifact.commit','artifact.remove','artifact.status','artifact.receipt','artifact.read','artifact.publish']
SOURCES=['upstream_artifact_output.cpp','artifact_output_native.h','artifact_publication_guest.h']


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
    if args.mode=='execute':
        record=json.loads((out/'results.json').read_text());assert record['preparation_complete']
        assert not record['complete'],'Preserve completed receipts; use a fresh output directory'
    else:
        assert not (out/'results.json').exists(),'Use a fresh preparation directory'
        record=dict(complete=False,preparation_complete=False,scope=__doc__,cgroup=str(GATE),inode=GATE.stat().st_ino,
            memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),commands=[],fixtures=[],outputs=[])
    # Preparation and execution can be separated by a reboot. Keep their
    # cgroup identities and event counters separate; the preparation inode
    # must never be presented as the later execution's current identity.
    invocation=dict(mode=args.mode,cgroup=str(GATE),inode=GATE.stat().st_ino,
        boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
        memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text())
    record.setdefault('invocations',[]).append(invocation)
    invocation_index=len(record['invocations'])-1
    def invocation_guard():
        guard()
        assert GATE.stat().st_ino==invocation['inode'],'Cgroup identity changed during invocation'
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==invocation['boot_id']
    def save():
        invocation_guard();invocation['events_after']=(GATE/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(name,command,env=None):
        invocation_guard();start=time.monotonic();result=subprocess.run([str(x) for x in command],cwd=ROOT,capture_output=True,timeout=180,env=env)
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        record['commands'].append(dict(name=name,argv=[str(x) for x in command],exit=result.returncode,seconds=time.monotonic()-start,
            invocation=invocation_index));save()
        assert result.returncode==0,(name,result.stderr.decode(errors='replace')[-5000:]);print('PASS',name,flush=True)
        return result.stdout
    def unchanged():
        for name,digest in record['input_sha256'].items():assert sha(out/name)==digest,'Changed preparation input: '+name
        for name,digest in record['toolchain_sha256'].items():assert sha(name)==digest,'Changed toolchain: '+name
        for name,digest in record['source_sha256'].items():assert sha(ROOT/name)==digest,'Changed fixture source: '+name
    if args.mode!='execute':
        receipt=args.corpus/'receipts'/(IDENTITY+'.json');received=json.loads(receipt.read_text())
        archive=args.corpus/'archives'/(IDENTITY+'.tar.gz');assert sha(archive)==received['archive']['sha256']
        (out/'sources').mkdir()
        with tarfile.open(archive) as bundle:bundle.extractall(out/'sources',filter='data')
        upstream=out/'sources'/RELEASE;parser_dir=upstream/'3rdparty/tinyxml2'
        shutil.copyfile(receipt,out/'upstream.acquisition.json')
        record['upstream']=dict(source=received['source'],archive=received['archive'],receipt_sha256=sha(receipt))
        files=[('parent','tests/trees/parent_no_include.xml'),('log','examples/test_files/log_test.xml'),
            ('includes','examples/test_files/subtree_test.xml'),('check','examples/test_files/Check.xml'),
            ('talk','examples/test_files/subtrees/Talk.xml')]
        content=[]
        for alias,path in files:
            source=upstream/path;data=source.read_bytes();content.append((alias,data,False))
            record['fixtures'].append(dict(alias=alias,origin=path,size=len(data),sha256=sha(source)))
        escaped='<?xml version="1.0"?><root name="机器人 &amp; &quot;camera&quot;"><value>&lt;goal&gt; &amp; π</value><!-- keep --></root>'.encode()
        content += [('escaping',escaped,False),('empty',b'',True)]
        for alias,data,empty in content[len(files):]:record['fixtures'].append(dict(alias=alias,origin='explicit boundary fixture',size=len(data),sha256=hashlib.sha256(data).hexdigest(),empty_document=empty))
        header=['#pragma once','#include <cstddef>','struct output_fixture { char const* alias;unsigned char const* bytes;std::size_t size;bool empty_document; };']
        for index,(_,data,_) in enumerate(content):header.append('inline constexpr unsigned char output_data_'+str(index)+'[]{'+(','.join(map(str,data)) if data else '0')+'};')
        header+=['inline constexpr output_fixture output_fixtures[]{']
        for index,(alias,data,empty) in enumerate(content):header.append('{'+json.dumps(alias)+',output_data_'+str(index)+','+str(len(data))+','+('true' if empty else 'false')+'},')
        header+=['};'];(out/'artifact_output_fixtures.h').write_text('\n'.join(header)+'\n')
        for name in SOURCES:shutil.copyfile(ROOT/'test/0084.ros-runtime'/name,out/name)
        source=out/'upstream_artifact_output.cpp';source.write_text(source.read_text().replace('artifact_output_guest.h','artifact_publication_guest.h'))
        shutil.copyfile(__file__,out/'qualification-runner.py')
        compiler=shutil.which(args.clang);linker=shutil.which(args.wasm_ld);assert compiler and linker
        libraries=[args.sysroot/'lib/wasm32-wasip1'/name for name in ['libc++.a','libc++abi.a','libc.a']]+[args.builtins]
        record['toolchain_sha256']={str(path.resolve()):sha(path) for path in [Path(compiler),Path(linker),*libraries]}
        common=[compiler,'-std=c++17','-O1','-g','-ffunction-sections','-fdata-sections','-fno-exceptions','-fno-rtti',
            '-I'+str(parser_dir),'-I'+str(out),parser_dir/'tinyxml2.cpp',out/'upstream_artifact_output.cpp']
        run('compiler-version',[compiler,'--version'])
        run('compile-native-reference',[*common,'-DARTIFACT_OUTPUT_REFERENCE','-Wl,--gc-sections','-o',out/'native-reference'])
        reference=run('native-reference',[out/'native-reference']).decode();assert reference.endswith('checks:14\n')
        expected=['#pragma once','#include <cstdint>','struct output_expectation {std::uint64_t size,hash;unsigned char sha256[32];};','inline constexpr output_expectation output_expectations[]{']
        for line in reference.splitlines():
            if not line.startswith('OUTPUT '):continue
            fields=line.split(' ',5);alias,compact,size,hashvalue=fields[1:5];data=bytes.fromhex(fields[5]);assert len(data)==int(size)
            digest=hashlib.sha256(data).digest();expected.append('{'+size+'ULL,'+hashvalue+'ULL,{'+','.join(map(str,digest))+'}},')
            record['outputs'].append(dict(alias=alias,compact=int(compact),size=int(size),hash=int(hashvalue),sha256=digest.hex()))
        assert len(record['outputs'])==14
        expected+=['};'];(out/'artifact_output_expected.h').write_text('\n'.join(expected)+'\n')
        run('compile-native',[*common,'-Wl,--gc-sections','-o',out/'native'])
        baseline=run('native',[out/'native']);assert baseline.endswith(b'checks:56\n')
        run('compile-native-sanitized',[*common,'-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wl,--gc-sections','-o',out/'native-sanitized'])
        assert run('native-sanitized',[out/'native-sanitized'],dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))==baseline
        guest=[*common,'--target=wasm32-wasip1','--sysroot='+str(args.sysroot.resolve()),'-nostartfiles',
            '-fuse-ld='+linker,'-Wl,--no-entry','-Wl,--export=_start','-Wl,--max-memory=16777216','-Wl,--gc-sections','-nodefaultlibs',*libraries]
        run('compile-wasm',[*guest,'-o',out/'guest.wasm'])
        run('compile-wasm-denied',[*guest,'-DEXPECT_ARTIFACT_OUTPUT_DENIAL','-o',out/'denied.wasm'])
        record['imports']={}
        for name in ['guest','denied']:
            run(name+'-disassemble',['wasm2wat','--enable-all',out/(name+'.wasm'),'-o',out/(name+'.wat')])
            imports=re.findall(r'\(import "([^"]+)" "([^"]+)"',(out/(name+'.wat')).read_text())
            assert all(module=='uwvm_ros_v2' and function in FUNCTIONS or module=='wasi_snapshot_preview1' and function in
                ['fd_write','fd_close','fd_seek','fd_fdstat_get','clock_time_get','fd_prestat_get','fd_prestat_dir_name','proc_exit'] for module,function in imports),imports
            record['imports'][name]=imports
        inputs=[out/name for name in [*SOURCES,'qualification-runner.py','artifact_output_fixtures.h','artifact_output_expected.h','upstream.acquisition.json',
            'native-reference','native','native-sanitized','guest.wasm','denied.wasm']]
        inputs += [parser_dir/'tinyxml2.cpp',parser_dir/'tinyxml2.h',*[upstream/path for _,path in files]]
        record['input_sha256']={str(path.relative_to(out)):sha(path) for path in inputs}
        record['source_sha256']={str(p.relative_to(ROOT)):sha(p) for p in [*[ROOT/'test/0084.ros-runtime'/name for name in SOURCES],Path(__file__)]}
        record['preparation_complete']=True;save()
    unchanged()
    if args.mode!='prepare':
        runtime=args.runtime.resolve();record['runtime']=dict(path=str(runtime),sha256=sha(runtime))
        def policy(deny=False):
            flags=['--ros-scene-create','xml','--ros-scene-bind','xml','main']
            for name in FUNCTIONS:flags+=['--ros-scene-interface','xml',name,'deny' if deny and name=='artifact.publish' else 'allow']
            flags+=['--ros-scene-grant','xml','xmlout','cache','serialized-xml','volatile-output',','.join(FUNCTIONS[1:]),
                '--ros-scene-output-budget','xml','65536','1','4192',
                '--ros-scene-output','xml','xmlout','65536','1','65536','4096',
                '--ros-scene-publication-budget','xml','8192','1','--ros-scene-publication','xml','xmlout','8192','1',
                '--ros-scene-limit','xml','xmlout','max-handles','1']
            flags+=['--wasip1-single-create','main','--wasip1-single-enable','main']
            for module,function in record['imports']['guest']:
                if module=='wasi_snapshot_preview1':flags+=['--wasip1-single-interface','main',function,'allow' if function=='fd_write' else 'deny']
            flags+=['--wasip1-single-grant','main','stdout.write'];return flags
        baseline=(out/'native.stdout').read_bytes()
        for engine in ['int','aot']:
            flags=[str(runtime),'--wasm-set-main-module-name','main','--runtime-'+engine]
            assert run('vm-'+engine,[*flags,*policy(),'--run',out/'guest.wasm'])==baseline
            assert run('vm-denied-'+engine,[*flags,*policy(True),'--run',out/'denied.wasm'])==b'checks:56\n'
        assert sha(runtime)==record['runtime']['sha256'];record['complete']=True
    unchanged();record['events_after']=(GATE/'memory.events').read_text();save()


if __name__=='__main__':main()
