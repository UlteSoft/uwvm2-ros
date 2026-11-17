#!/usr/bin/env python3
"""Check the unregistered complete-publication prototype in the existing gate.

The baseline broker headers come from the unchanged qualified VM snapshot.
Actual frozen XML output bytes drive both native adapters. Call counts are
algorithm topology evidence, not new VM imports, timing or safety certification.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'

def main():
    global ROOT
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repository-root',type=Path,help='Required when replaying the retained runner snapshot outside tools/ros_scene')
    parser.add_argument('--prototype-source-root',type=Path,help='Replay a retained source/ directory instead of current prototype files')
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--source-root',type=Path,required=True)
    parser.add_argument('--vm-build',type=Path,required=True)
    parser.add_argument('--xml-prepared',type=Path,required=True)
    parser.add_argument('--thread',action='store_true')
    args=parser.parse_args();guard()
    if args.repository_root:ROOT=args.repository_root.resolve()
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    frozen=args.source_root.resolve();build_path=args.vm_build.resolve();build=json.loads(build_path.read_text())
    assert build['complete'] and build['sources_unchanged'] and frozen==Path(build['source_root']).resolve()
    manifest=Path(build['full_source_manifest']);assert sha(manifest)==build['full_source_manifest_sha256']
    frozen_inputs=json.loads(manifest.read_text())
    for name,digest in frozen_inputs.items():assert sha(frozen/name)==digest
    prepared=args.xml_prepared.resolve();qualified=json.loads((prepared/'results.json').read_text());assert qualified['complete']
    assert qualified['runtime']['sha256']==build['binary_sha256']
    reference=prepared/'native-reference.stdout';parsed=[]
    for line in reference.read_text().splitlines():
        if not line.startswith('OUTPUT '):continue
        _,alias,compact,size,value,*hexadecimal=line.split();data=bytes.fromhex(hexadecimal[0]) if hexadecimal else b''
        expected=qualified['outputs'][len(parsed)]
        assert (alias,int(compact),int(size),int(value))==(expected['alias'],expected['compact'],expected['size'],expected['hash'])
        fnv=1469598103934665603
        for byte in data:fnv=((fnv^byte)*1099511628211)&((1<<64)-1)
        assert len(data)==int(size) and fnv==int(value) and hashlib.sha256(data).hexdigest()==expected['sha256'];parsed.append(data)
    assert len(parsed)==14 and sum(map(len,parsed))==3005
    fixture=out/'publication_fixture_bytes.h';lines=['#pragma once','#include <array>','#include <span>','#include <cstdint>']
    for n,data in enumerate(parsed):lines.append('inline constexpr std::array<std::uint8_t,'+str(len(data))+'> publication_bytes_'+str(n)+'{'+','.join(map(str,data))+'};')
    lines.append('inline constexpr std::array<std::span<std::uint8_t const>,14> publication_fixture_bytes{'+','.join('std::span<std::uint8_t const>{publication_bytes_'+str(n)+'}' for n in range(14))+'};')
    fixture.write_text('\n'.join(lines)+'\n')
    prototype_root=args.prototype_source_root.resolve() if args.prototype_source_root else ROOT/'test/0084.ros-runtime'
    source=prototype_root/'complete_publication.cpp';header=prototype_root/'prototype/complete_publication.h'
    snapshots=out/'source';(snapshots/'prototype').mkdir(parents=True)
    snapshot_sources={source:snapshots/source.name,header:snapshots/'prototype'/header.name,Path(__file__).resolve():snapshots/Path(__file__).name}
    for original,target in snapshot_sources.items():shutil.copyfile(original,target);assert sha(original)==sha(target)
    workspace_paths=[ROOT/'test/0084.ros-runtime/complete_publication.cpp',ROOT/'test/0084.ros-runtime/prototype/complete_publication.h',ROOT/'tools/ros_scene/complete_publication_checks.py']
    workspace_inputs={str(path):sha(path) for path in workspace_paths}
    snapshot_inputs={str(path):sha(path) for path in snapshot_sources.values()}
    inputs={str(path):sha(path) for path in [source,header,Path(__file__).resolve(),fixture,reference,prepared/'results.json',manifest,build_path,*snapshot_sources.values()]}
    record=dict(complete=False,scope=__doc__,cgroup=str(GROUP),inode=GROUP.stat().st_ino,boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text(),cpu_affinity=sorted(os.sched_getaffinity(0)),
                source_root=str(frozen),baseline_vm_binary_sha256=build['binary_sha256'],production_vm_changed=False,new_import_registered=False,
                native_parity_qualified=False,new_vm_hostcall_count_qualified=False,inputs=inputs,commands=[],toolchain={},
                prototype_source_root=str(prototype_root),compiled_source_root=str(snapshots),prototype_snapshots=snapshot_inputs,workspace_inputs=workspace_inputs,
                prototype_matches_workspace=all(sha(original)==sha(workspace) for original,workspace in zip([source,header,Path(__file__).resolve()],workspace_paths)))
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,env=None,allow_failure=False):
        guard();start=time.monotonic();result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=240)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-start));save()
        print(label+': '+str(result.returncode),flush=True)
        if result.returncode and not allow_failure:raise RuntimeError(result.stderr[-6000:])
        return result
    def passed(result):
        assert 'passed 10 complete-publication prototype groups' in result.stdout
        assert 'XML native adapter topology legacy=66 proposed=40 outputs=14; no new VM import executed' in result.stdout
    success=False
    try:
        for name,compiler,flags in [('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc13','/usr/bin/g++-13',[])]:
            record['toolchain'][compiler]=sha(compiler);binary=out/name
            run(name+'-compile',[compiler,'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(frozen/'src'),'-I'+str(out),*flags,str(snapshots/source.name),'-o',str(binary)])
            passed(run(name,[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1')))
        if args.thread:
            compiler='/usr/bin/clang++-22';binary=out/'thread-sanitized'
            run('thread-compile',[compiler,'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(frozen/'src'),'-I'+str(out),
                '-fsanitize=thread','-fno-omit-frame-pointer',str(snapshots/source.name),'-o',str(binary)])
            initialized=False
            for attempt in range(1,9):
                result=run('thread-run-'+str(attempt),[str(binary)],dict(os.environ,TSAN_OPTIONS='halt_on_error=1'),True)
                if not result.returncode:passed(result);assert 'WARNING: ThreadSanitizer:' not in result.stderr;initialized=True;break
                if not (result.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not result.stdout):raise RuntimeError(result.stderr[-6000:])
            assert initialized,'TSan did not initialize in eight recorded attempts'
        record['native_adapter_topology']=dict(outputs=14,bytes=3005,legacy=66,proposed=40,includes_vm=False,once_only_open_excluded=True)
        success=True
    finally:
        record['inputs_unchanged']=all(sha(path)==digest for path,digest in inputs.items())
        record['prototype_snapshots_unchanged']=all(sha(path)==digest for path,digest in snapshot_inputs.items())
        record['workspace_inputs_unchanged']=all(sha(path)==digest for path,digest in workspace_inputs.items())
        record['frozen_baseline_unchanged']=all(sha(frozen/name)==digest for name,digest in frozen_inputs.items())
        record['events_after']=(GROUP/'memory.events').read_text();record['complete']=success and record['inputs_unchanged'] and record['frozen_baseline_unchanged'] and record['workspace_inputs_unchanged'] and record['prototype_snapshots_unchanged'];save()
        if success:assert record['complete']

if __name__=='__main__':main()
