#!/usr/bin/env python3
"""Build an immutable VM cohort with mandatory -O3 -g in the existing 64 GiB gate.

Reuse recorded compiler/link recipes, replacing only their source/output roots
and optimization/debug levels. All source manifest hashes are verified before
and after compilation. This does not qualify unrelated live workspace changes.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--recipes',required=True,type=Path)
    parser.add_argument('--source',required=True,type=Path)
    parser.add_argument('--output',required=True,type=Path)
    parser.add_argument('--host-profile',choices=('recipe','default','legacy-enabled'),default='recipe',
        help='recipe preserves recorded defines; default selects only ROSv2; legacy-enabled additionally compiles both deprecated ROSv1 hosts and native preloading')
    parser.add_argument('--intel-jit-sdk',type=Path,help='Trusted symbol-only qualification variant; official Intel SDK root. Never inferred from environment.')
    parser.add_argument('--integer-leaf-qualification',action='store_true',help='Explicit closed integer-leaf experiment; requires the qualified frozen receipt and Intel observer variant.')
    args=parser.parse_args()
    inode=GROUP.stat().st_ino
    def gate():
        assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
        assert GROUP.stat().st_ino==inode
        assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    gate()
    source=args.source.resolve();recipes=args.recipes.resolve();out=args.output.resolve()
    scratch=[(ROOT/'.tmp').resolve(),(ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve()]
    assert all(any(path.is_relative_to(root) for root in scratch) for path in (source,out))
    out.mkdir(parents=True,exist_ok=False)
    baseline=json.loads((recipes/'full-build-results.json').read_text())
    assert baseline['complete'] and baseline['sources_unchanged']
    old_source=Path(baseline['source_root'])
    manifest=source/'source-manifest.json';files=json.loads(manifest.read_text())
    assert all(sha(source/name)==digest for name,digest in files.items())
    profiling=None
    if args.intel_jit_sdk:
        sdk=args.intel_jit_sdk.resolve()
        sdk_header=sdk/'include/jitprofiling.h';sdk_library=sdk/'lib64/libjitprofiling.a'
        qualification=source/'intel-jit-qualification.json'
        info=json.loads(qualification.read_text())
        assert info['complete'] and info['ordinary_full_only'] and not info['production_integration']
        assert info['qualified_manifest_sha256']==sha(manifest)
        assert info['trusted_compile_time_opt_in']=='UWVM_SCENE_INTEL_JIT_QUALIFICATION'
        assert sdk_header.is_file() and sdk_library.is_file()
        profiling=dict(sdk=str(sdk),inputs={str(p):sha(p) for p in (sdk_header,sdk_library,qualification)},
            guest_source_mapping=False,production_integration=False)
    integer_leaf=None
    if args.integer_leaf_qualification:
        assert profiling is not None
        qualification=source/'integer-leaf-qualification.json';info=json.loads(qualification.read_text())
        assert info['complete'] and not info['production_integration'] and info['max_instructions']==32
        assert info['qualified_manifest_sha256']==sha(manifest)
        assert info['trusted_compile_time_opt_in']=='UWVM_SCENE_INTEGER_LEAF_QUALIFICATION'
        integer_leaf={str(qualification):sha(qualification)}
    report=dict(complete=False,optimization='O3',debug='full -g',cgroup=str(GROUP),cgroup_inode=inode,
        boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),memory_max=68719476736,swap_max=0,
        events_before=(GROUP/'memory.events').read_text(),affinity=sorted(os.sched_getaffinity(0)),
        source_root=str(source),full_source_manifest=str(manifest),full_source_manifest_sha256=sha(manifest),
        full_source_files=len(files),sources=files,units=[],commands=[],recipes={},runner_sha256=sha(__file__),intel_jit_qualification=profiling,
        integer_leaf_qualification=integer_leaf,host_profile=args.host_profile)
    def save():(out/'full-build-results.json').write_text(json.dumps(report,indent=2)+'\n')
    def command(name):
        path=recipes/(name+'.argv.json');report['recipes'][str(path)]=sha(path)
        argv=json.loads(path.read_text())
        # Older recipes retained workspace vendor include paths even when the
        # VM headers were frozen. Prefer the snapshot's copied fast_io and its
        # explicit vendor links; never silently compile against a newer live
        # fast_io tree while reporting the frozen manifest as build input.
        return [item.replace(str(old_source),str(source)).replace(str(recipes),str(out)).replace(
            str(ROOT/'third-parties'),str(source/'third-parties')) for item in argv]
    def run(name,argv):
        gate();(out/(name+'.argv.json')).write_text(json.dumps(argv,indent=2)+'\n');save()
        print('Building '+name,flush=True);started=time.monotonic()
        with (out/(name+'.log')).open('w') as log:
            result=subprocess.run(argv,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT,timeout=1800)
        report['commands'].append(dict(name=name,argv=argv,exit=result.returncode,seconds=time.monotonic()-started));save()
        if result.returncode:raise RuntimeError(name+': '+(out/(name+'.log')).read_text()[-8000:])
        return result.returncode
    success=False
    try:
        for unit in ('main.default.cpp.o','uwvm_runtime.default.cpp.o','host_api.default.cpp.o'):
            argv=command(unit)
            argv=[item for item in argv if not re.fullmatch(r'-O(?:[0-3szg]|fast)',item) and item not in ('-g','-g0','-g1','-g2','-g3','-gline-tables-only','-gline-directives-only')]
            argv[1:1]=['-O3','-g']
            if args.host_profile!='recipe':
                host_macros=('UWVM_IMPORT_ROS1','UWVM_IMPORT_ROS2','UWVM_IMPORT_ROSV2','UWVM_ENABLE_PRELOAD_DL','UWVM_SUPPORT_PRELOAD_DL')
                argv=[item for item in argv if not (item.startswith(('-D','-U')) and item[2:].split('=',1)[0] in host_macros)]
                argv[1:1]=['-DUWVM_IMPORT_ROSV2','-DUWVM_ROS_ATTACHMENT_OPENSSL']
                if args.host_profile=='legacy-enabled':
                    argv[1:1]=['-DUWVM_IMPORT_ROS1','-DUWVM_IMPORT_ROS2','-DUWVM_ENABLE_PRELOAD_DL=1']
            if profiling:
                assert not any('UWVM_RUNTIME_INTEL_JIT_PROFILE' in x or 'UWVM_SCENE_INTEL_JIT_QUALIFICATION' in x for x in argv)
                argv[1:1]=['-DUWVM_RUNTIME_INTEL_JIT_PROFILE=1','-DUWVM_SCENE_INTEL_JIT_QUALIFICATION=1','-I'+str(sdk/'include')]
            if integer_leaf:argv[1:1]=['-DUWVM_SCENE_INTEGER_LEAF_QUALIFICATION=1']
            assert [x for x in argv if re.fullmatch(r'-O(?:[0-3szg]|fast)',x)]==['-O3']
            assert '-g' in argv and not any(x in argv for x in ('-g0','-gline-tables-only','-gline-directives-only','-s'))
            status=run(unit,argv);report['units'].append(dict(unit=unit,returncode=status));save()
        link=command('link')
        if profiling:link.append(str(sdk_library))
        assert not any(x in ('-s','--strip-all','-Wl,-s','-Wl,--strip-all') for x in link)
        report['link_returncode']=run('link',link)
        binary=out/'uwvm2-ros';report['binary_sha256']=sha(binary)
        sections=subprocess.run(['/usr/bin/readelf','--sections','--wide',str(binary)],capture_output=True,text=True,check=True,timeout=120)
        (out/'debug-sections.txt').write_text(sections.stdout)
        assert '.debug_info' in sections.stdout and '.debug_line' in sections.stdout
        report['debug_sections_sha256']=sha(out/'debug-sections.txt');success=True
    finally:
        gate();report['sources_unchanged']=report['full_source_unchanged']=all(sha(source/name)==digest for name,digest in files.items())
        report['events_after']=(GROUP/'memory.events').read_text()
        report['recipes_unchanged']=all(sha(path)==digest for path,digest in report['recipes'].items())
        if profiling:
            report['profiling_inputs_unchanged']=all(sha(path)==digest for path,digest in profiling['inputs'].items())
            assert report['profiling_inputs_unchanged']
        if integer_leaf:
            report['integer_leaf_inputs_unchanged']=all(sha(path)==digest for path,digest in integer_leaf.items())
            assert report['integer_leaf_inputs_unchanged']
        report['complete']=success and report['sources_unchanged'] and report['recipes_unchanged'];save()
    assert report['complete']
    print(str(out/'uwvm2-ros')+' '+report['binary_sha256'],flush=True)

if __name__=='__main__':main()
