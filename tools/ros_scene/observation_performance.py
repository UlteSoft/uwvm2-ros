#!/usr/bin/env python3
"""Compare native policy/projection and actual Wasm observation with -O3 -g.

Run in the existing 64 GiB gate, without concurrent builds or timing workloads.
Both references authorize, encode, copy and erase the same kind of metadata;
the native trusted registry fixture excludes Wasm identity/memory/dispatch.
Timings are distributions of batch means, not single-call tail guarantees.
VTune hardware collection includes startup; unprofiled timings exclude startup.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import statistics
import struct
import subprocess
import sys
import time

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'test/0084.ros-runtime'))
from run_module_security_vm import Fixture, record, QUERY
from run_scene_vm import verify_gate, GATE, sha
from performance_build_policy import compile_policy, vm_policy
from hardware_sampling import audit_samples, reference_frequency, vtune_periods
VTUNE='/opt/intel/oneapi/vtune/latest/bin64/vtune'
COUNTS=[1,3,0,1,2,4,1,17]


def guest(operation,iterations,batches):
    name=QUERY[operation];g=Fixture([name])
    g.extra_imports=['(import "worker" "ping" (func $ping))',
        '(import "wasi_snapshot_preview1" "clock_time_get" (func $now (param i32 i64 i32) (result i32)))',
        '(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))']
    query=record('rosv2','permission.report',0) if operation==0 else record('worker-view','rosv2','permission.report',0) if operation==5 else record('runtime-view' if operation==7 else 'worker-view',0)
    g.request(name,query);g.body+=['global.get $checksum '+g.word(16416)+' i64.add global.set $checksum']
    step=g.body;g.body=[]
    g.globals.update(['checksum','batch','iteration','begin','started','elapsed','total'])
    g.extra_functions.append('(func $step '+'\n'.join(step)+')')
    g.extra_functions.append('(func $clock (result i64) i32.const 1 i64.const 0 i32.const 26000 call $now if unreachable end '+g.word(26000)+')')
    g.body+=['i64.const 1000 global.set $iteration','loop $warm call $step global.get $iteration i64.const 1 i64.sub global.set $iteration global.get $iteration i64.eqz i32.eqz br_if $warm end',
        'call $clock global.set $begin','loop $batches','call $clock global.set $started',f'i64.const {iterations} global.set $iteration',
        'loop $iterations call $step global.get $iteration i64.const 1 i64.sub global.set $iteration global.get $iteration i64.eqz i32.eqz br_if $iterations end',
        'call $clock global.get $started i64.sub global.set $elapsed',
        'i32.const 50000 global.get $batch i32.wrap_i64 i32.const 8 i32.mul i32.add global.get $elapsed i64.store $work',
        'global.get $batch i64.const 1 i64.add global.set $batch',f'global.get $batch i64.const {batches} i64.lt_u br_if $batches end',
        'call $clock global.get $begin i64.sub global.set $total']
    g.write(50000+8*batches,'global.get $total');g.write(50008+8*batches,'global.get $checksum')
    g.body+=['i32.const 30000 i32.const 50000 i32.store $work',f'i32.const 30004 i32.const {8*(batches+2)} i32.store $work',
        'i32.const 1 i32.const 30000 i32.const 1 i32.const 30008 call $write if unreachable end']
    return g


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode',choices=['baseline','profile','perf-import'])
    parser.add_argument('--runtime',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--operations',nargs='+',choices=QUERY,default=QUERY)
    parser.add_argument('--iterations',type=int,default=10000);parser.add_argument('--batches',type=int,default=51)
    parser.add_argument('--rounds',type=int,default=3);parser.add_argument('--cpu',type=int,default=2)
    parser.add_argument('--sampling-interval',type=float,default=.1)
    parser.add_argument('--reference-context',type=Path);parser.add_argument('--reference-period',type=int,default=24000)
    parser.add_argument('--assembly-function',action='append',default=[])
    a=parser.parse_args();verify_gate();assert 0<a.batches<=1000 and 0<a.iterations<=10000000 and a.rounds>0
    assert a.cpu in os.sched_getaffinity(0)
    def sampler_settings():
        return {name:(Path('/proc/sys/kernel')/name).read_text().strip()
                for name in ('perf_event_max_sample_rate','perf_cpu_time_max_percent','perf_event_paranoid')}
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';built=json.loads(build.read_text())
    assert built['complete'] and built['binary_sha256']==sha(runtime)
    source=Path(built['source_root']);manifest=source/'source-manifest.json'
    files=json.loads(manifest.read_text());assert all(sha(source/n)==v for n,v in files.items())
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    fixture=ROOT/'test/0084.ros-runtime/observation_performance.cpp';shutil.copyfile(fixture,out/fixture.name)
    runners=[Path(__file__).resolve(),ROOT/'test/0084.ros-runtime/run_module_security_vm.py',
        ROOT/'test/0084.ros-runtime/run_scene_vm.py',fixture,
        Path(__file__).with_name('hardware_sampling.py'),Path(__file__).with_name('performance_build_policy.py')]
    for runner in runners:
        retained=out/'retained-inputs'/runner.relative_to(ROOT)
        retained.parent.mkdir(parents=True,exist_ok=True)
        shutil.copyfile(runner,retained)
    inputs=[*runners,build,manifest,runtime]
    if a.reference_context:
        inputs.append(a.reference_context.resolve())
        shutil.copyfile(a.reference_context,out/'reference-context.cfg')
    report=dict(complete=False,scope=__doc__,cgroup=str(GATE),cgroup_inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,
        events_before=(GATE/'memory.events').read_text(),runtime_sha256=sha(runtime),vm_compile_policy=vm_policy(runtime),source_root=str(source),
        inputs={str(p):sha(p) for p in inputs},invocation=sys.argv,commands=[],observations=[],profiles=[],cpu=a.cpu,summary={},
        boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),host=os.uname().nodename,
        sampling_settings_before=sampler_settings())
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,timeout=300):
        verify_gate();argv=list(map(str,argv));start=time.monotonic();p=subprocess.run(argv,capture_output=True,timeout=timeout)
        (out/(label+'.stdout')).write_bytes(p.stdout);(out/(label+'.stderr')).write_bytes(p.stderr)
        report['commands'].append(dict(label=label,argv=argv,exit=p.returncode,seconds=time.monotonic()-start));save()
        assert p.returncode==0,(label,p.stderr.decode(errors='replace')[-5000:]);return p
    def observe(label,operation,p,profile=None):
        assert len(p.stdout)==8*(a.batches+2),(label,len(p.stdout),p.stdout[:100])
        values=struct.unpack('<'+'Q'*(a.batches+2),p.stdout)
        assert values[-1]==COUNTS[operation]*(1000+a.batches*a.iterations),(label,values[-1])
        samples=sorted(x/a.iterations for x in values[:-2])
        report['observations'].append(dict(label=label,operation=QUERY[operation],profile=profile,iterations=a.iterations,batches=a.batches,
            batch_mean_ns_p50=statistics.median(samples),batch_mean_ns_p95=samples[min(len(samples)-1,int(.95*len(samples)))],checksum=values[-1]));save()
    success=False
    try:
        compiler='/usr/bin/clang++-22'
        argv=[compiler,'--gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13','-std=c++26','-O3','-g','-Wall','-Wextra','-Werror','-pthread',
            '-DUWVM_USE_UWVM_INT','-DUWVM_USE_LLVM_JIT','-DUWVM_IMPORT_ROSV2','-DUWVM_ROS_ZERO_HOST_DEFAULTS=1',
            '-I'+str(source/'src'),'-I'+str(source/'third-parties/fast_io/include'),'-I'+str(source/'third-parties/bizwen/include'),
            '-I'+str(source/'third-parties/boost_unordered/include'),str(out/fixture.name),'-o',str(out/'native')]
        if 'class owned_page' in (source/'src/uwvm2/imported/ros/scene/permission_report.h').read_text():argv.insert(1,'-DUWVM_SCENE_OBSERVATION_OWNED_PAGE=1')
        report['native_compile_policy']=compile_policy(argv);run('compile-native',argv)
        report['native_sha256']=sha(out/'native');report['compiler_sha256']=sha(compiler)
        worker=out/'worker.wat';worker.write_text('(module (func (export "ping")))')
        run('compile-worker',['wat2wasm',worker,'-o',out/'worker.wasm'])
        flags=['--ros-scene-create','benchmark','--ros-scene-bind','benchmark','main','--ros-scene-create','worker-group','--ros-scene-bind','worker-group','worker']
        for op in QUERY[1:]:flags+=['--ros-scene-interface','benchmark',op,'allow']
        flags+=['--ros-scene-grant','benchmark','worker-view','module','worker','metadata',','.join(x for x in QUERY[1:] if x!='vm.describe'),
            '--ros-scene-grant','benchmark','runtime-view','vm','runtime','metadata','vm.describe']
        flags+=['--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-interface','main','clock_time_get','allow',
            '--wasip1-single-interface','main','fd_write','allow','--wasip1-single-grant','main','monotonic-clock','--wasip1-single-grant','main','stdout.write']
        for name in a.operations:
            operation=QUERY.index(name);wat=out/(name+'.wat');wat.write_text(guest(operation,a.iterations,a.batches).render())
            wasm=out/(name+'.wasm');run('compile-'+name,['wat2wasm','--enable-multi-memory','--debug-names',wat,'-o',wasm])
            commands={'native':['taskset','-c',a.cpu,out/'native',operation,a.iterations,a.batches],
                'wasm':['taskset','-c',a.cpu,runtime,'--wasm-set-main-module-name','main','--runtime-aot','--wasm-policy-global', 'runtime.llvm.policy','max',
                    '--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,'--wasm-preload-library',out/'worker.wasm','worker','--run',wasm]}
            report.setdefault('workloads',{})[name]={k:list(map(str,v)) for k,v in commands.items()};save()
            if a.mode=='baseline':
                for repeat in range(a.rounds):
                    for variant in (['native','wasm'] if repeat%2==0 else ['wasm','native']):
                        label=name+'-'+variant;observe(label,operation,run(label+'-'+str(repeat),commands[variant]))
                medians={variant:statistics.median(x['batch_mean_ns_p50'] for x in report['observations'] if x['label']==name+'-'+variant) for variant in commands}
                report['summary'][name]=dict(native_ns=medians['native'],wasm_ns=medians['wasm'],wasm_over_native=medians['wasm']/medians['native']);save()
                print(name+' native=%.2f ns wasm=%.2f ns ratio=%.2f'%(medians['native'],medians['wasm'],medians['wasm']/medians['native']),flush=True)
            else:
                for variant,command in commands.items():
                    label=name+'-'+variant
                    if a.mode=='perf-import':
                        assert a.reference_context
                        hz=reference_frequency(a.reference_context,os.uname().nodename);trace=out/(label+'.perf');target=out/(label+'-hardware')
                        events={'CPU_CLK_UNHALTED.REF_TSC':('umask=0x3',a.reference_period),'CPU_CLK_UNHALTED.THREAD':('event=0x3c',240000),
                            'INST_RETIRED.ANY':('event=0xc0',240000),'BR_INST_RETIRED.ALL_BRANCHES':('event=0xc4',240000),'BR_MISP_RETIRED.ALL_BRANCHES':('event=0xc5',10000)}
                        config=','.join(f'cpu_core/{encoding},period={period},name={key}/u' for key,(encoding,period) in events.items())
                        observe(label,operation,run('collect-'+label,['/usr/bin/perf','record','-o',trace,'-N','-B','-T','--sample-cpu','-e',config,'--',*command]),'perf-import')
                        attrs=run('attributes-'+label,['/usr/bin/perf','evlist','-v','-i',trace]).stdout.decode()
                        samples=run('samples-'+label,['/usr/bin/perf','script','--ns','-F','comm,pid,tid,time,event,period,ip','-i',trace]).stdout.decode()
                        raw=run('records-'+label,['/usr/bin/perf','script','-D','-i',trace]).stdout.decode()
                        audit=audit_samples(attrs,samples,raw,{k:v[1] for k,v in events.items()},hz,'native' if variant=='native' else 'uwvm2-ros')
                        report['profiles'].append(dict(label=label,analysis='explicit-hardware-perf-import',audit=audit,result=str(target)));save()
                        assert audit['periods_verified']
                        run('import-'+label,[VTUNE,'-import',trace,'-result-dir',target,'-search-dir',str(runtime.parent),'-source-search-dir',source,'-finalization-mode','full'])
                        for kind,group in [('functions','function'),('lines','function,source-file,source-line')]:
                            run(kind+'-'+label,[VTUNE,'-report','hw-events','-result-dir',target,'-group-by',group,'-format','csv'])
                        for i,function in enumerate(a.assembly_function):
                            run('assembly-'+label+'-'+str(i),[VTUNE,'-report','hw-events','-result-dir',target,'-source-object','function='+function,
                                '-group-by','address','-column','Assembly,Source Line,Hardware Event Count','-format','csv'])
                    else:
                        for analysis in ('hotspots','uarch-exploration'):
                            target=out/(label+'-'+analysis)
                            knobs=['-knob','sampling-mode=hw','-knob','enable-stack-collection=true'] if analysis=='hotspots' else ['-knob','pmu-collection-mode=detailed','-knob','dram-bandwidth-limits=false']
                            observe(label,operation,run('collect-'+label+'-'+analysis,[VTUNE,'-collect',analysis,'-no-summary','-no-analyze-system','-return-app-exitcode','-data-limit=400','-result-dir',target,'-knob','sampling-interval='+str(a.sampling_interval),
                                *knobs,'-source-search-dir',source,'--',*command]),analysis)
                            report['profiles'].append(dict(label=label,analysis=analysis,requested_interval_ms=a.sampling_interval,
                                period_evidence=vtune_periods(target),result=str(target)));save()
                            for kind in ('summary','hotspots'):
                                run(kind+'-'+label+'-'+analysis,[VTUNE,'-report',kind,'-result-dir',target,'-format','csv'])
                            run('lines-'+label+'-'+analysis,[VTUNE,'-report','hotspots','-result-dir',target,'-group-by','function,source-file,source-line','-format','csv'])
                            for i,function in enumerate(a.assembly_function):
                                run('assembly-'+label+'-'+analysis+'-'+str(i),[VTUNE,'-report','hw-events','-result-dir',target,'-source-object','function='+function,
                                    '-group-by','address','-column','Assembly,Source Line,Hardware Event Count','-format','csv'])
        success=True
    finally:
        report['inputs_unchanged']=all(sha(Path(p))==v for p,v in report['inputs'].items())
        report['sources_unchanged']=all(sha(source/n)==v for n,v in files.items())
        report['events_after']=(GATE/'memory.events').read_text();report['complete']=success and report['inputs_unchanged'] and report['sources_unchanged'];save()
        report['sampling_settings_after']=sampler_settings();save()
    assert report['complete']


if __name__=='__main__':main()
