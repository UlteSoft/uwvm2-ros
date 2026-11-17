#!/usr/bin/env python3
"""Additional native scene-policy diagnostic for the exact XML output benchmark.

Keeps the original vector-copy reference, XML bytes, adapter roundtrip checks,
O3 compiler and 3x31x1000 timing shape. Only the separate broker bridge needs
C++23/exceptions; the serializer and reused guest adapter retain C++17/noexcept
build flags. No VM host wrapper or Wasm guest-memory copy is included.
The broker is compiled against the exact manifest-verified frozen VM source.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import statistics
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode',choices=['prepare','run'])
    parser.add_argument('--prepared',type=Path,required=True,help='Existing lifecycle-xml-batched benchmark directory')
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--source-root',type=Path,required=True,help='Frozen source directory used by the qualified VM build')
    parser.add_argument('--vm-build',type=Path,required=True,help='VM full-build-results.json pinning the frozen source manifest')
    parser.add_argument('--cpu',type=int,default=1)
    parser.add_argument('--rounds',type=int,default=3)
    args=parser.parse_args();guard();assert args.cpu in os.sched_getaffinity(0) and 1<=args.rounds<=9
    prepared=args.prepared.resolve();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    source_root=args.source_root.resolve();build_path=args.vm_build.resolve()
    build=json.loads(build_path.read_text())
    assert build['complete'] and build['sources_unchanged'] and build['link_returncode']==0
    assert source_root==Path(build['source_root']).resolve()
    manifest=Path(build['full_source_manifest']).resolve()
    assert manifest.is_relative_to(source_root) and sha(manifest)==build['full_source_manifest_sha256']
    source_manifest=json.loads(manifest.read_text())
    for name,digest in source_manifest.items():
        assert (source_root/name).resolve().is_relative_to(source_root),'Source manifest escapes frozen directory: '+name
        assert sha(source_root/name)==digest,'Changed frozen build source: '+name
    base=json.loads((prepared/'prepare.json').read_text());assert base['complete']
    for name,digest in base['inputs'].items():assert sha(name)==digest
    for name,digest in base['toolchain'].items():assert sha(name)==digest
    fixture=base['fixtures']['baseline'];assert (fixture['iterations'],fixture['batches'])==(1000,31)
    record=dict(complete=False,mode=args.mode,scope=__doc__,cgroup=str(GATE),memory_max=68719476736,swap_max=0,
                events_before=(GATE/'memory.events').read_text(),commands=[],observations=[],cpu=args.cpu,
                paired_native_reference=False,native_parity_qualified=False,hostcall_cost_isolated=False,
                configuration=dict(bytes=65536,objects=1,object_bytes=65536,transfer=fixture['chunk'],recovery_bytes=fixture['chunk']+96,
                                   max_handles=1,rate_attenuation=None,timed_retries=False),
                baseline_prepare_sha256=sha(prepared/'prepare.json'))
    record.update(source_root=str(source_root),source_manifest=str(manifest),source_manifest_sha256=sha(manifest),
                  vm_build=str(build_path),vm_build_sha256=sha(build_path),vm_binary_sha256=build['binary_sha256'])
    def save():(out/(args.mode+'.json')).write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();result=subprocess.run([str(x) for x in argv],cwd=ROOT,capture_output=True,text=True,timeout=240)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=[str(x) for x in argv],exit=result.returncode,seconds=time.monotonic()-start));save()
        print(label+': '+str(result.returncode),flush=True)
        if result.returncode:raise RuntimeError(result.stderr[-6000:])
        return result.stdout
    def observe(name,number,data):
        header=re.search(r'iterations=(\d+) batches=(\d+) outputs=(\d+) chunk=(\d+) expected=(\d+) checksum=(\d+)',data);assert header
        fields=tuple(map(int,header.groups()));assert fields[:4]==(1000,31,14,fixture['chunk'])
        samples=list(map(int,re.findall(r'elapsed_ns=(\d+)',data)));assert len(samples)==31 and min(samples)>0
        if record['observations']:assert fields[4:]==tuple(record['observations'][0]['header'][4:])
        record['observations'].append(dict(provider=name,round=number,header=fields,samples_ns=samples,
                                           batch_p50_ns_per_cycle=statistics.median(samples)/1000));save()
    passed=False
    try:
        if args.mode=='prepare':
            assert not (out/'prepare.json').exists(),'Use a fresh preparation directory'
            names=['upstream_artifact_output.cpp','upstream_artifact_output_benchmark.cpp','artifact_output_fixtures.h','artifact_output_expected.h']
            for name in names:shutil.copyfile(prepared/name,out/name)
            guest=(prepared/'artifact_output_guest.h').read_text()
            adapted,count=re.subn(r'__attribute__\(\(import_module\("uwvm_ros_v2"\),import_name\([^)]*\)\)\)','',guest)
            assert count==2
            (out/'artifact_output_native.h').write_text(adapted)
            shim=ROOT/'test/0084.ros-runtime/artifact_output_native_scene.cpp';shutil.copyfile(shim,out/shim.name)
            shutil.copyfile(Path(__file__),out/Path(__file__).name)
            native=next(row['argv'] for row in base['commands'] if row['name']=='compile-native-baseline')
            compile_flags=[];skip=False
            for flag in native:
                if skip:skip=False;continue
                if flag=='-o':skip=True;continue
                if flag.endswith('.cpp') or flag.startswith('-Wl,'):continue
                compile_flags.append(flag.replace('-I'+str(prepared),'-I'+str(out)))
            xml=next(Path(flag) for flag in native if flag.endswith('/tinyxml2.cpp'))
            for label,source in [('xml',xml),('fixture',out/'upstream_artifact_output_benchmark.cpp')]:
                run('compile-'+label,[*compile_flags,'-c',source,'-o',out/(label+'.o')])
            run('compile-scene',[native[0],'-std=c++23','-O3','-g','-ffunction-sections','-fdata-sections','-fno-rtti','-pthread',
                '-I'+str(source_root/'src'),'-DOUTPUT_CHUNK='+str(fixture['chunk']),'-c',out/shim.name,'-o',out/'scene.o'])
            run('link',[native[0],'-O3','-pthread',out/'xml.o',out/'fixture.o',out/'scene.o','-Wl,--gc-sections','-o',out/'native-scene'])
            observe('native-reference',0,run('verify-native',['taskset','-c',args.cpu,prepared/'native-baseline']))
            observe('native-scene',0,run('verify-scene',['taskset','-c',args.cpu,out/'native-scene']))
            record['prepared']=str(prepared);record['binary_sha256']=sha(out/'native-scene');record['compiler_sha256']=sha(native[0])
            record['adapter_transformation']='Only removed the two Wasm import attribute spellings; guest roundtrip body unchanged'
            record['inputs']={str(path):sha(path) for path in [Path(__file__),out/Path(__file__).name,shim,*[out/name for name in names],out/'artifact_output_native.h',out/shim.name,xml,
                prepared/'artifact_output_guest.h',prepared/'native-baseline',manifest,build_path]}
        else:
            config=json.loads((out/'prepare.json').read_text());assert config['complete'] and config['prepared']==str(prepared)
            for name in ['source_root','source_manifest_sha256','vm_build_sha256','vm_binary_sha256']:assert record[name]==config[name]
            assert config['baseline_prepare_sha256']==sha(prepared/'prepare.json')
            for name,digest in config['inputs'].items():assert sha(name)==digest
            assert sha(out/'native-scene')==config['binary_sha256']
            record['prepare_sha256']=sha(out/'prepare.json');record['inputs']=config['inputs'];record['binary_sha256']=config['binary_sha256']
            for number in range(args.rounds):
                for name in (['native-reference','native-scene'] if number%2==0 else ['native-scene','native-reference']):
                    binary=prepared/'native-baseline' if name=='native-reference' else out/'native-scene'
                    observe(name,number,run(name+'-'+str(number),['taskset','-c',args.cpu,binary]))
            medians={name:statistics.median(row['batch_p50_ns_per_cycle'] for row in record['observations'] if row['provider']==name)
                     for name in ['native-reference','native-scene']}
            record['comparison']=dict(median_of_round_p50_ns_per_cycle=medians,scene_reference_ratio=medians['native-scene']/medians['native-reference'])
            record['paired_native_reference']=True
        for name,digest in record['inputs'].items():assert sha(name)==digest
        assert sha(manifest)==record['source_manifest_sha256'] and sha(build_path)==record['vm_build_sha256']
        for name,digest in source_manifest.items():assert sha(source_root/name)==digest,'Changed frozen build source: '+name
        for name,digest in base['toolchain'].items():assert sha(name)==digest
        passed=True
    finally:
        record['complete']=passed;record['events_after']=(GATE/'memory.events').read_text();save()


if __name__=='__main__':main()
