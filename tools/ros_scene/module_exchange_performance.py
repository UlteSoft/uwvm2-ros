#!/usr/bin/env python3
"""Measure real cross-Wasm messages; profile hardware at requested 0.01 ms.

Guest timestamps exclude loading/JIT. Batch distributions are means per complete
round trip, not individual-request tail latencies. The direct Wasm reference
passes one i64 and does not provide the registered-message isolation contract.
VTune profiling includes startup/JIT and is separate from unprofiled timing.
All tools inherit the existing 64 GiB, zero-swap cgroup; no limits are changed.
"""
import argparse
import json
import os
from pathlib import Path
import statistics
import struct
import subprocess
import sys
import time
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'test/0084.ros-runtime'))
from run_scene_vm import verify_gate, GATE, sha
from run_module_security_vm import Fixture, CALLS
from hardware_sampling import audit_samples, reference_frequency, vtune_periods
from performance_build_policy import vm_policy
VTUNE='/opt/intel/oneapi/vtune/latest/bin64/vtune'


def guests(payload,iterations,batches,direct=False):
    w=Fixture(CALLS,shared=True);w.globals.add('exchange')
    w.call('module.call.take','global.get $exchange',[0,0],expected=11,output=8192,capacity=payload+16,length=24000)
    w.extra_functions.append('(func (export "init") (param $c i64) local.get $c global.set $exchange '+'\n'.join(w.body)+')');w.body=[]
    w.call('module.call.take','global.get $exchange',[0,0],output=8192,capacity=payload+16,length=24000)
    w.write(128,w.word(8192));w.write(136,payload)
    w.body.append(f'i32.const 144 i32.const 8208 i32.const {payload} memory.copy $work $work')
    w.write(144,w.word(144)+' i64.const 1 i64.add')
    w.call('module.call.respond','global.get $exchange',size=payload+16,output=20000,capacity=16,length=25000)
    w.extra_functions.append('(func (export "pump") '+'\n'.join(w.body)+')');w.body=[]
    w.extra_functions.append('(func (export "direct") (param i64) (result i64) local.get 0 i64.const 1 i64.add)')
    g=Fixture([] if direct else CALLS,shared=True)
    g.extra_imports=['(import "worker" "init" (func $init (param i64)))','(import "worker" "pump" (func $pump))',
        '(import "worker" "direct" (func $direct (param i64) (result i64)))',
        '(import "wasi_snapshot_preview1" "clock_time_get" (func $now (param i32 i64 i32) (result i32)))',
        '(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))']
    g.globals.update(['elapsed','started','batch','iteration','checksum','begin','total'])
    if not direct:
        cap=g.open('exchange');g.body.append(cap+' call $init')
        setup=g.body;g.body=[]
        g.write(128,0);g.write(136,payload);g.body.append(f'i32.const 144 i32.const 165 i32.const {payload} memory.fill $work')
        g.write(144,21)
        g.call('module.call.submit',cap,size=payload+16,output=20000,capacity=16,length=25000)
        token=g.save('token',20000)
        g.body.append('call $pump')
        g.call('module.call.result',cap,[token,0],output=8192,capacity=payload+16,length=24000)
        g.equal(g.word(8208),22)
        g.body.append('global.get $checksum '+g.word(8208)+' i64.add global.set $checksum')
        g.call('module.call.acknowledge',cap,[token,0],output=20000,capacity=0,length=25000)
        step=g.body;g.body=setup
    else:
        step=['i64.const 21 call $direct global.get $checksum i64.add global.set $checksum']
    g.extra_functions.append('(func $step '+'\n'.join(step)+')')
    g.extra_functions.append('(func $clock (result i64) i32.const 1 i64.const 0 i32.const 26000 call $now if unreachable end '+g.word(26000)+')')
    # A thousand unmeasured round trips exercise all functions before timing.
    g.body+=['i64.const 1000 global.set $iteration','loop $warm call $step global.get $iteration i64.const 1 i64.sub global.set $iteration global.get $iteration i64.eqz i32.eqz br_if $warm end',
        'call $clock global.set $begin','loop $batches','call $clock global.set $started',f'i64.const {iterations} global.set $iteration',
        'loop $iterations call $step global.get $iteration i64.const 1 i64.sub global.set $iteration global.get $iteration i64.eqz i32.eqz br_if $iterations end',
        'call $clock global.get $started i64.sub global.set $elapsed',
        'i32.const 40000 global.get $batch i32.wrap_i64 i32.const 8 i32.mul i32.add global.get $elapsed i64.store $work',
        'global.get $batch i64.const 1 i64.add global.set $batch',f'global.get $batch i64.const {batches} i64.lt_u br_if $batches end',
        'call $clock global.get $begin i64.sub global.set $total']
    g.write(40000+8*batches,'global.get $total');g.write(40008+8*batches,'global.get $checksum')
    g.body+=['i32.const 30000 i32.const 40000 i32.store $work',f'i32.const 30004 i32.const {8*(batches+2)} i32.store $work',
        'i32.const 1 i32.const 30000 i32.const 1 i32.const 30008 call $write if unreachable end']
    return g,w


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode',choices=['baseline','profile','perf-import']);parser.add_argument('--runtime',required=True,type=Path)
    parser.add_argument('--output',required=True,type=Path);parser.add_argument('--cpu',type=int,default=2)
    parser.add_argument('--iterations',type=int,default=500);parser.add_argument('--batches',type=int,default=51)
    parser.add_argument('--payloads',type=int,nargs='+',default=[8,256,4096]);parser.add_argument('--direct',action='store_true')
    parser.add_argument('--sampling-interval',type=float,default=0.01,
                        help='Requested hardware interval in ms; never silently falls back')
    parser.add_argument('--analyses',nargs='+',choices=['hotspots','uarch-exploration'],default=['hotspots','uarch-exploration'])
    parser.add_argument('--reference-context',type=Path,help='Same-host VTune context containing the measured referenceFrequency; required for perf-import')
    parser.add_argument('--pmu',choices=['cpu_core','cpu_atom'],default='cpu_core')
    parser.add_argument('--reference-period',type=int,default=24000)
    parser.add_argument('--assembly-function',action='append',help='Exact VTune function selector; repeat for the optimized call path. Defaults retain the original baseline selectors.')
    args=parser.parse_args();verify_gate();assert args.cpu in os.sched_getaffinity(0)
    assert 1<=args.iterations<=1000000 and 1<=args.batches<=501 and all(8<=p<=4096 for p in args.payloads)
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=False);runtime=args.runtime.resolve()
    build=json.loads((runtime.parent/'full-build-results.json').read_text());assert build['complete'] and build['binary_sha256']==sha(runtime)
    compile_evidence=vm_policy(runtime)
    assert 0.01<=args.sampling_interval<=1000
    if args.mode=='perf-import':
        assert args.reference_context and 1<=args.reference_period<=1000000000
        reference_hz=reference_frequency(args.reference_context,os.uname().nodename)
        assert args.cpu in {int(cpu) for part in (Path('/sys/bus/event_source/devices')/args.pmu/'cpus').read_text().strip().split(',')
                           for cpu in (range(int(part.split('-')[0]),int(part.split('-')[1])+1) if '-' in part else [int(part)])}
    receipt=dict(complete=False,scope=__doc__,requested_sampling_interval_ms=args.sampling_interval,
        requested_10us_target_met=False,perf_event_max_sample_rate=Path('/proc/sys/kernel/perf_event_max_sample_rate').read_text().strip(),
        cgroup=str(GATE),memory_max=68719476736,swap_max=0,
        events_before=(GATE/'memory.events').read_text(),runtime_sha256=sha(runtime),build_sha256=sha(runtime.parent/'full-build-results.json'),
        runner_sha256=sha(__file__),helper_sha256=sha(ROOT/'test/0084.ros-runtime/run_module_security_vm.py'),
        sampling_audit_sha256=sha(Path(__file__).with_name('hardware_sampling.py')),
        compile_policy_sha256=sha(Path(__file__).with_name('performance_build_policy.py')),
        compile_policy=compile_evidence,commands=[],observations=[],profiles=[])
    if args.mode=='perf-import':receipt['reference_context']=dict(path=str(args.reference_context.resolve()),sha256=sha(args.reference_context),hz=reference_hz)
    def save():(out/'results.json').write_text(json.dumps(receipt,indent=2)+'\n')
    def run(label,argv,timeout=180,required=True):
        verify_gate();start=time.monotonic();p=subprocess.run(argv,capture_output=True,timeout=timeout)
        (out/(label+'.stdout')).write_bytes(p.stdout);(out/(label+'.stderr')).write_bytes(p.stderr)
        receipt['commands'].append(dict(label=label,argv=argv,exit=p.returncode,seconds=time.monotonic()-start));save()
        if required and p.returncode:raise RuntimeError(label+': '+p.stderr.decode(errors='replace')[-5000:])
        print(label,p.returncode,flush=True);return p
    # Baseline/profile receipts also pin the audit helper and runtime so later
    # edits cannot silently qualify a different experiment.
    pinned={runtime:receipt['runtime_sha256'],Path(__file__):receipt['runner_sha256'],
            Path(__file__).with_name('hardware_sampling.py'):receipt['sampling_audit_sha256'],
            Path(__file__).with_name('performance_build_policy.py'):receipt['compile_policy_sha256'],
            ROOT/'test/0084.ros-runtime/run_module_security_vm.py':receipt['helper_sha256'],
            runtime.parent/'full-build-results.json':receipt['build_sha256']}
    pinned.update({Path(item['argv_file']):item['argv_sha256'] for item in compile_evidence.values()})
    if args.mode=='perf-import':pinned[args.reference_context]=receipt['reference_context']['sha256']
    try:
        for payload,direct in [(p,False) for p in args.payloads]+([(8,True)] if args.direct else []):
            label=('direct' if direct else 'broker')+'-'+str(payload)
            main,worker=guests(payload,args.iterations,args.batches,direct)
            for name,g,export in [(label,main,'_start'),(label+'-worker',worker,None)]:
                (out/(name+'.wat')).write_text(g.render(export))
                run('compile-'+name,['wat2wasm','--enable-threads','--enable-multi-memory','--debug-names',str(out/(name+'.wat')),'-o',str(out/(name+'.wasm'))])
            flags=['--ros-scene-create','benchmark','--ros-scene-bind','benchmark','main','--ros-scene-bind','benchmark','worker']
            for op in ['capability.open',*CALLS]:flags+=['--ros-scene-interface','benchmark',op,'allow']
            flags+=['--ros-scene-call-budget','benchmark','65536','1','--ros-scene-call','benchmark','echo','main','worker','echo',
                'request-octets','response-octets',str(payload),str(payload),'1','60000000000',
                '--ros-scene-grant','benchmark','exchange','module.call','echo','registered-message',','.join(CALLS)]
            flags+=['--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-interface','main','clock_time_get','allow',
                '--wasip1-single-interface','main','fd_write','allow','--wasip1-single-grant','main','monotonic-clock','--wasip1-single-grant','main','stdout.write']
            command=['taskset','-c',str(args.cpu),str(runtime),'--wasm-set-main-module-name','main','--runtime-aot','--runtime-llvm-jit-policy','max',
                '--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,
                '--wasm-preload-library',str(out/(label+'-worker.wasm')),'worker','--run',str(out/(label+'.wasm'))]
            def observation(proc,profile=None):
                assert len(proc.stdout)==8*(args.batches+2),(len(proc.stdout),proc.stdout[:300])
                values=struct.unpack('<'+'Q'*(args.batches+2),proc.stdout)
                assert values[-1]==22*(args.iterations*args.batches+1000)
                samples=sorted(v/args.iterations for v in values[:-2]);receipt['observations'].append(dict(label=label,payload_bytes=payload,
                    iterations=args.iterations,batches=args.batches,profile=profile,elapsed_ns=values[-2],
                    batch_mean_ns_p50=statistics.median(samples),batch_mean_ns_p95=samples[min(len(samples)-1,int(.95*len(samples)))],
                    batch_mean_ns_p99=samples[min(len(samples)-1,int(.99*len(samples)))],checksum=values[-1],guest_sha256=sha(out/(label+'.wasm'))));save()
            if args.mode=='baseline':
                for round_number in range(3):observation(run(label+'-'+str(round_number),command))
            elif args.mode=='perf-import':
                # Explicit periods avoid VTune's SEP minimum-SAV conversion.
                # Only the target process is sampled, on the selected hybrid PMU.
                events={'CPU_CLK_UNHALTED.REF_TSC':('umask=0x3',args.reference_period),
                        'CPU_CLK_UNHALTED.THREAD':('event=0x3c',240000),
                        'INST_RETIRED.ANY':('event=0xc0',240000),
                        'BR_INST_RETIRED.ALL_BRANCHES':('event=0xc4',240000),
                        'BR_MISP_RETIRED.ALL_BRANCHES':('event=0xc5',10000)}
                config=','.join(f'{args.pmu}/{encoding},period={period},name={name}/u' for name,(encoding,period) in events.items())
                trace=out/(label+'.perf');target=out/(label+'-hardware')
                argv=['/usr/bin/perf','record','-o',str(trace),'-N','-B','-T','--sample-cpu','-e',config,'--',*command]
                observation(run('collect-'+label,argv,300),'perf-import')
                attrs=run('attributes-'+label,['/usr/bin/perf','evlist','-v','-i',str(trace)]).stdout.decode()
                samples=run('samples-'+label,['/usr/bin/perf','script','--ns','-F','comm,pid,tid,time,event,period,ip','-i',str(trace)]).stdout.decode()
                raw=run('records-'+label,['/usr/bin/perf','script','-D','-i',str(trace)]).stdout.decode()
                audit=audit_samples(attrs,samples,raw,{name:period for name,(_,period) in events.items()},reference_hz)
                receipt['profiles'].append(dict(result=str(target),analysis='explicit-hardware-perf-import',trace_sha256=sha(trace),audit=audit));save()
                assert audit['periods_verified'],'Missing, clamped, lost or throttled hardware samples'
                run('import-'+label,[VTUNE,'-import',str(trace),'-result-dir',str(target),'-source-search-dir',build['source_root'],'-finalization-mode','full'],300)
                for kind,group in [('functions','function'),('lines','function,source-file,source-line')]:
                    run(kind+'-'+label,[VTUNE,'-report','hw-events','-result-dir',str(target),'-group-by',group,'-format','csv'],180)
                functions=args.assembly_function or ['uwvm2::runtime::lib::ros_observed_caller_identity_host_api',
                           'uwvm2::runtime::lib::exchange_memory_details::current',
                           'uwvm2::runtime::lib::ros_exchange_memory_erase_host_api',
                           'uwvm2::uwvm::imported::scene::invoke_module_call<(uwvm2::imported::ros::scene::module_exchange::operation)2>']
                receipt['profiles'][-1]['assembly_functions']=functions;save()
                for index,function in enumerate(functions):
                    run('assembly-'+label+'-'+str(index),[VTUNE,'-report','hw-events','-result-dir',str(target),'-source-object','function='+function,
                        '-group-by','address','-column','Assembly,Source Line,Hardware Event Count','-format','csv'],180)
                receipt['requested_10us_target_met']=all(p['audit']['ten_us_reference_period_verified'] for p in receipt['profiles']);save()
            else:
                for analysis in args.analyses:
                    target=out/(label+'-'+analysis)
                    knobs=['-knob','sampling-mode=hw','-knob','enable-stack-collection=true'] if analysis=='hotspots' else ['-knob','pmu-collection-mode=detailed','-knob','dram-bandwidth-limits=false']
                    argv=[VTUNE,'-collect',analysis,*knobs,'-knob','sampling-interval='+str(args.sampling_interval),'-no-analyze-system','-no-summary','-data-limit=400','-return-app-exitcode',
                        '-result-dir',str(target),'-source-search-dir',build['source_root'],'--',*command]
                    p=run('collect-'+label+'-'+analysis,argv,300);observation(p,analysis)
                    for kind in ('summary','hotspots'):
                        run('report-'+label+'-'+analysis+'-'+kind,[VTUNE,'-report',kind,'-result-dir',str(target),'-format','csv'],120)
                    run('lines-'+label+'-'+analysis,[VTUNE,'-report','hotspots','-result-dir',str(target),'-group-by','function,source-file,source-line','-format','csv'],120)
                    receipt['profiles'].append(dict(result=str(target),analysis=analysis,requested_sampling_interval_ms=args.sampling_interval,
                                                   period_evidence=vtune_periods(target)));save()
                    if args.assembly_function:
                        receipt['profiles'][-1]['assembly_functions']=args.assembly_function;save()
                        for index,function in enumerate(args.assembly_function):
                            run('assembly-'+label+'-'+analysis+'-'+str(index),[VTUNE,'-report','hw-events','-result-dir',str(target),
                                '-source-object','function='+function,'-group-by','address','-column',
                                'Assembly,Source Line,Hardware Event Count','-format','csv'],180)
        receipt['complete']=True
    finally:
        verify_gate();receipt['inputs_unchanged']=all(sha(path)==value for path,value in pinned.items())
        receipt['complete']=receipt['complete'] and receipt['inputs_unchanged']
        receipt['events_after']=(GATE/'memory.events').read_text();save()
    assert receipt['complete'],'Performance inputs changed during qualification'

if __name__=='__main__':main()
