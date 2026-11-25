#!/usr/bin/env python3
"""Measure the qualified upstream XML slice with bounded artifact reads.

Native uses preloaded immutable byte snapshots and identical bounded copies;
Wasm uses opened scene artifact handles. Both parse the same seven inputs with
unmodified TinyXML2. Batch timing excludes initialization/compilation/reporting;
official VTune collection includes the whole process. No native parity claim is
made merely because compilation, correctness or hardware collection succeeds.
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

def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset(set(range(8,16)))

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode',choices=['prepare','baseline','profile'])
    parser.add_argument('--prepared',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--runtime',type=Path)
    parser.add_argument('--iterations',type=int,default=2000)
    parser.add_argument('--profile-iterations',type=int,default=10000)
    parser.add_argument('--batches',type=int,default=31)
    parser.add_argument('--rounds',type=int,default=3)
    parser.add_argument('--chunk',type=int,choices=[64,4096],default=4096)
    parser.add_argument('--cpu',type=int,default=8)
    parser.add_argument('--vtune',default='/opt/intel/oneapi/vtune/latest/bin64/vtune')
    args=parser.parse_args();guard()
    assert args.cpu in os.sched_getaffinity(0)
    assert 1<=args.iterations<=100000 and 1<=args.profile_iterations<=100000 and 3<=args.batches<=101 and 1<=args.rounds<=9
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True);prepared=args.prepared.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    qualification=json.loads((prepared/'results.json').read_text());assert qualification['preparation_complete']
    for name,digest in qualification['input_sha256'].items():assert sha(prepared/name)==digest
    for name,digest in qualification['toolchain_sha256'].items():assert sha(name)==digest
    record=dict(complete=False,mode=args.mode,scope=__doc__,cgroup=str(GATE),inode=GATE.stat().st_ino,
        memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),cpu=args.cpu,
        thread_siblings=Path(f'/sys/devices/system/cpu/cpu{args.cpu}/topology/thread_siblings_list').read_text().strip(),
        loadavg_before=Path('/proc/loadavg').read_text(),commands=[],observations=[],native_parity_qualified=False,
        guest_jit_source_mapping_qualified=False)
    def save():(out/(args.mode+'.json')).write_text(json.dumps(record,indent=2)+'\n')
    def run(name,argv,timeout=240):
        guard();start=time.monotonic();result=subprocess.run([str(x) for x in argv],cwd=ROOT,capture_output=True,timeout=timeout)
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        record['commands'].append(dict(name=name,argv=[str(x) for x in argv],exit=result.returncode,seconds=time.monotonic()-start));save()
        assert result.returncode==0,(name,result.stderr.decode(errors='replace')[-5000:]);print('PASS',name,flush=True)
        return result.stdout
    if args.mode=='prepare':
        assert not (out/'prepare.json').exists(),'Use a fresh output directory'
        shutil.copyfile(__file__,out/'benchmark-runner.py')
        for name in ['upstream_artifact_xml.cc','artifact_fixtures.h']:shutil.copyfile(prepared/name,out/name)
        source=ROOT/'test/0084.ros-runtime/upstream_artifact_benchmark.cc';shutil.copyfile(source,out/source.name)
        parser_dir=next((prepared/'sources').glob('*/3rdparty/tinyxml2'))
        original=next(row['argv'] for row in qualification['commands'] if row['name']=='compile-native')
        guest_original=next(row['argv'] for row in qualification['commands'] if row['name']=='compile-wasm')
        common=[original[0],'-std=c++17','-O3','-g','-ffunction-sections','-fdata-sections','-fno-exceptions','-fno-rtti',
                '-I'+str(parser_dir),'-I'+str(out),parser_dir/'tinyxml2.cpp',out/source.name]
        target=guest_original[guest_original.index('--target=wasm32-wasip1'):guest_original.index('-o')]
        record['fixtures']={}
        for mode,iterations in [('baseline',args.iterations),('profile',args.profile_iterations)]:
            defines=['-DARTIFACT_ITERATIONS='+str(iterations),'-DARTIFACT_BATCHES='+str(args.batches),'-DARTIFACT_CHUNK='+str(args.chunk)]
            run('compile-native-'+mode,[*common,*defines,'-Wl,--gc-sections','-o',out/('native-'+mode)])
            run('compile-wasm-'+mode,[*common,*defines,*target,'-o',out/(mode+'.wasm')])
            run('disassemble-'+mode,['wasm2wat','--enable-all',out/(mode+'.wasm'),'-o',out/(mode+'.wat')])
            imports=re.findall(r'\(import "([^"]+)" "([^"]+)"',(out/(mode+'.wat')).read_text())
            assert all(module=='uwvm_ros_v2' and function in ['capability.open','artifact.open','artifact.read','resource.close'] or
                       module=='wasi_snapshot_preview1' and function in ['fd_write','fd_close','fd_seek','fd_fdstat_get',
                           'clock_time_get','fd_prestat_get','fd_prestat_dir_name','proc_exit'] for module,function in imports),imports
            record['fixtures'][mode]=dict(iterations=iterations,batches=args.batches,chunk=args.chunk,imports=imports)
        inputs=[out/name for name in ['benchmark-runner.py','upstream_artifact_xml.cc','artifact_fixtures.h',source.name,'native-baseline','native-profile','baseline.wasm','profile.wasm']]
        record['inputs']={str(path):sha(path) for path in inputs}
        record['inputs'].update({str(prepared/name):digest for name,digest in qualification['input_sha256'].items()})
        record['toolchain']=qualification['toolchain_sha256'];record['runner_sha256']=sha(__file__)
        record['source_sha256']=sha(source);record['prepared']=str(prepared)
    else:
        assert args.runtime and qualification['complete'],'Complete real-VM correctness before performance qualification'
        runtime=args.runtime.resolve();build=json.loads((runtime.parent/'full-build-results.json').read_text())
        assert build['complete'] and build['optimization']=='O3' and build['sources_unchanged'] and build['link_returncode']==0
        assert sha(runtime)==build['binary_sha256']==qualification['runtime']['sha256'],'Performance requires the exact correctness-qualified VM'
        assert {unit['unit'] for unit in build['units'] if unit['returncode']==0}=={'main.default.cpp.o','uwvm_runtime.default.cpp.o','host_api.default.cpp.o'}
        for unit in ['main.default.cpp','uwvm_runtime.default.cpp','host_api.default.cpp']:
            argv=json.loads((runtime.parent/(unit+'.o.argv.json')).read_text())
            assert [x for x in argv if re.fullmatch(r'-O(?:[0-3szg]|fast)',x)][-1]=='-O3'
        config=json.loads((out/'prepare.json').read_text());assert config['complete'] and config['prepared']==str(prepared)
        assert sha(__file__)==config['runner_sha256']
        for name,digest in {**config['inputs'],**config['toolchain']}.items():assert sha(name)==digest
        record['runtime_sha256']=sha(runtime);record['build_sha256']=sha(runtime.parent/'full-build-results.json')
        record['prepare_sha256']=sha(out/'prepare.json');record['qualification_sha256']=sha(prepared/'results.json')
        record['vm_source_snapshots']={}
        for name,digest in build['sources'].items():
            assert sha(ROOT/name)==digest,'Source differs from VM build: '+name
            snapshot=out/'vm-sources'/name;snapshot.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/name,snapshot)
            record['vm_source_snapshots'][name]=digest
        mode=args.mode if args.mode=='profile' else 'baseline';fixture=config['fixtures'][mode]
        def policy():
            flags=['--ros-scene-create','xml','--ros-scene-bind','xml','main',
                '--ros-scene-input-budget','xml',str(sum(f['size'] for f in qualification['fixtures'])),str(len(qualification['fixtures']))]
            for name in ['capability.open','artifact.open','artifact.read','resource.close']:flags+=['--ros-scene-interface','xml',name,'allow']
            for value in qualification['fixtures']:
                alias=value['alias'];flags+=['--ros-scene-grant','xml',alias,'configuration',alias,'sealed-input','artifact.open,artifact.read,resource.close',
                    '--ros-scene-input','xml',alias,value['path'],value['digest'],str(value['generation']),str(value['size']),str(fixture['chunk']),
                    '--ros-scene-limit','xml',alias,'max-handles','1','--ros-scene-limit','xml',alias,'max-resources','1']
            flags+=['--wasip1-single-create','main','--wasip1-single-enable','main']
            for module,function in fixture['imports']:
                if module=='wasi_snapshot_preview1':flags+=['--wasip1-single-interface','main',function,'allow' if function in ['fd_write','clock_time_get'] else 'deny']
            flags+=['--wasip1-single-grant','main','stdout.write','--wasip1-single-grant','main','monotonic-clock']
            return flags
        def command(provider):
            if provider=='native':return ['taskset','-c',str(args.cpu),str(out/('native-'+mode))]
            return ['taskset','-c',str(args.cpu),str(runtime),'--wasm-set-main-module-name','main','--runtime-aot',
                '--wasm-policy-global', 'runtime.llvm.policy','max','--runtime-compile-threads','1','--runtime-llvm-jit-cache-path','disable',*policy(),'--run',str(out/(mode+'.wasm'))]
        expected=None
        def observe(provider,data,label):
            nonlocal expected
            text=data.decode(errors='replace');header=re.search(r'iterations=(\d+) batches=(\d+) fixtures=(\d+) chunk=(\d+) expected=(\d+) checksum=(\d+)',text)
            assert header,text[-2000:];values=tuple(map(int,header.groups()))
            assert values[:4]==(fixture['iterations'],fixture['batches'],7,fixture['chunk'])
            if expected is None:expected=values[4:]
            else:assert values[4:]==expected
            durations=list(map(int,re.findall(r'elapsed_ns=(\d+)',text)));assert len(durations)==fixture['batches'] and min(durations)>0
            record['observations'].append(dict(provider=provider,label=label,chunk=fixture['chunk'],checksum=values[5],samples_ns=durations,
                batch_p50_ns_per_cycle=statistics.median(durations)/fixture['iterations']));save()
        if args.mode=='baseline':
            for number in range(args.rounds):
                for provider in (['native','wasm'] if number%2==0 else ['wasm','native']):observe(provider,run(f'{provider}-{number}',command(provider)),number)
            medians={provider:statistics.median(row['batch_p50_ns_per_cycle'] for row in record['observations'] if row['provider']==provider) for provider in ['native','wasm']}
            record['comparison']=dict(median_of_round_p50_ns=medians,wasm_native_ratio=medians['wasm']/medians['native'])
        else:
            run('vtune-version',[args.vtune,'-version']);record['whole_process_profile']=True
            for provider in ['wasm','native']:
                for analysis in ['hotspots','uarch-exploration']:
                    result=out/('vtune-'+provider+'-'+analysis);assert not result.exists()
                    knobs=['-knob','sampling-mode=hw','-knob','enable-stack-collection=true'] if analysis=='hotspots' else ['-knob','pmu-collection-mode=detailed','-knob','dram-bandwidth-limits=false']
                    data=run('collect-'+provider+'-'+analysis,[args.vtune,'-collect',analysis,*knobs,'-knob','sampling-interval=1',
                        '-no-analyze-system','-data-limit=300','-return-app-exitcode','-result-dir',result,'-source-search-dir',ROOT,'--',*command(provider)],timeout=360)
                    observe(provider,data,analysis)
                    for kind in ['summary','hotspots']:run(f'report-{provider}-{analysis}-{kind}',[args.vtune,'-report',kind,'-result-dir',result,'-format','csv'])
                    run(f'lines-{provider}-{analysis}',[args.vtune,'-report','hotspots','-result-dir',result,
                        '-group-by','function,source-file,source-line','-format','csv'])
        assert sha(runtime)==record['runtime_sha256']
        for name,digest in record['vm_source_snapshots'].items():assert sha(ROOT/name)==digest
        for name,digest in {**config['inputs'],**config['toolchain']}.items():assert sha(name)==digest
    record['events_after']=(GATE/'memory.events').read_text();record['loadavg_after']=Path('/proc/loadavg').read_text()
    record['complete']=True;save()

if __name__=='__main__':main()
