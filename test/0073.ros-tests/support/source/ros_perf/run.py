#!/usr/bin/env python3
"""Bounded actual native-process versus authorized Wasm ROS topic measurements."""
import argparse
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import threading
import time
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'ros'))
import run_network as network

COUNT=100

def wat(version,size,count=COUNT,pace_ns=10000000):
    warm='w'*size
    return f"""(module
      (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
      (import "uwvm_ros{version}_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))
      (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))
      (memory 1) (data (i32.const 0) "pub") (data (i32.const 200) "0123456789abcdef")
      (data (i32.const 1028) "{warm}")
      (func $check (param $e i32) (if (local.get $e) (then unreachable)))
      (func $delay (param $n i64) (local $start i64)
        (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64)))
        (local.set $start (i64.load (i32.const 64)))
        (loop $wait
          (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
          (br_if $wait (i64.lt_u (i64.sub (i64.load (i32.const 72)) (local.get $start)) (local.get $n)))))
      (func (export "_start") (local $count i32) (local $i i32) (local $stamp i64)
        (call $check (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)))
        (i32.store (i32.const 1024) (i32.const {size}))
        (call $check (call $publish (i64.load (i32.const 8)) (i32.const 0) (i64.const 1024) (i64.const {size+4})))
        (call $delay (i64.const 1500000000))
        (memory.fill (i32.const 1044) (i32.const 120) (i32.const {size-16}))
        (loop $samples
          (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 80)))
          (local.set $stamp (i64.load (i32.const 80))) (local.set $i (i32.const 0))
          (loop $hex
            (i32.store8 (i32.add (i32.const 1028) (local.get $i))
              (i32.load8_u (i32.add (i32.const 200) (i32.wrap_i64 (i64.and (i64.shr_u (local.get $stamp) (i64.extend_i32_u (i32.sub (i32.const 60) (i32.mul (local.get $i) (i32.const 4))))) (i64.const 15))))))
            (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $hex (i32.lt_u (local.get $i) (i32.const 16))))
          (call $check (call $publish (i64.load (i32.const 8)) (i32.const 0) (i64.const 1024) (i64.const {size+4})))
          (call $delay (i64.const {pace_ns}))
          (local.set $count (i32.add (local.get $count) (i32.const 1))) (br_if $samples (i32.lt_u (local.get $count) (i32.const {count}))))
        (call $delay (i64.const 500000000))))
"""

def dual_wat(size,count=COUNT,pace_ns=10000000):
    first=wat(1,size,count,pace_ns).replace('$open','$open1').replace('$publish','$publish1')
    second=wat(2,size,count,pace_ns).replace('$open','$open2').replace('$publish','$publish2')
    imports='\n'.join(line for line in second.splitlines() if 'uwvm_ros2_v1' in line)
    first=first.replace('      (memory 1)',imports+'\n      (memory 1)')
    opening='(call $check (call $open1 (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)))'
    first=first.replace(opening,opening+'\n(call $check (call $open2 (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 16)))')
    publishing=f'(call $check (call $publish1 (i64.load (i32.const 8)) (i32.const 0) (i64.const 1024) (i64.const {size+4})))'
    return first.replace(publishing,publishing+'\n'+publishing.replace('$publish1','$publish2').replace('(i32.const 8)','(i32.const 16)'))

class Runner(network.Runner):
    def __init__(self,args):
        super().__init__(args);self.metrics={};self.threads=[]
    def start(self,command,name,env=None):
        proc=super().start(command,name,env);row={'pid':proc.pid,'max_rss_bytes':0,'cpu_seconds':0,'samples':0};self.metrics[name]=row
        def monitor():
            observed={}
            while proc.poll() is None:
                pending=[proc.pid];rss_total=0;seen=set()
                while pending:
                    pid=pending.pop()
                    if pid in seen:continue
                    seen.add(pid)
                    try:
                        pending.extend(map(int,Path(f'/proc/{pid}/task/{pid}/children').read_text().split()))
                        stat=Path(f'/proc/{pid}/stat').read_text().split(') ',1)[1].split()
                        status=dict(line.split(':',1) for line in Path(f'/proc/{pid}/status').read_text().splitlines())
                        rss=int(status.get('VmRSS','0 kB').split()[0])*1024;rss_total+=rss
                        item=observed.setdefault(pid,{'pid':pid,'cpu_seconds':0,'max_rss_bytes':0})
                        item.update(cpu_seconds=(int(stat[11])+int(stat[12]))/os.sysconf('SC_CLK_TCK'),max_rss_bytes=max(item['max_rss_bytes'],rss),voluntary_context_switches=int(status.get('voluntary_ctxt_switches',0)),involuntary_context_switches=int(status.get('nonvoluntary_ctxt_switches',0)),membership=Path(f'/proc/{pid}/cgroup').read_text().splitlines(),command=Path(f'/proc/{pid}/cmdline').read_bytes().replace(b'\0',b' ').decode(errors='replace'))
                        if rss>512*1024*1024:row['budget_exceeded']=True;os.killpg(proc.pid,signal.SIGTERM)
                    except (OSError,ValueError):pass
                row.update(max_rss_bytes=max(row['max_rss_bytes'],rss_total),cpu_seconds=sum(i['cpu_seconds'] for i in observed.values()),voluntary_context_switches=sum(i.get('voluntary_context_switches',0) for i in observed.values()),involuntary_context_switches=sum(i.get('involuntary_context_switches',0) for i in observed.values()),processes=list(observed.values()))
                row['samples']+=1
                time.sleep(.01)
        thread=threading.Thread(target=monitor,daemon=True);thread.start();self.threads.append(thread);return proc
    def ready(self,proc,name):
        for _ in range(200):
            if proc.poll() is not None:raise RuntimeError('FAIL native observer startup')
            if b'READY' in (self.run/(name+'.log')).read_bytes():return
            time.sleep(.01)
        raise RuntimeError('FAIL observer readiness timeout')
    def result(self,name):
        lines=(self.run/(name+'.log')).read_text().splitlines()
        return json.loads(next(line for line in reversed(lines) if line.startswith('{')))
    def cleanup(self):
        super().cleanup()
        for thread in self.threads:thread.join(timeout=1)

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--workspace',required=True);p.add_argument('--source',default=str(Path(__file__).resolve().parents[5]));p.add_argument('--uwvm',required=True);p.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''));p.add_argument('--wat2wasm',default='wat2wasm');a=p.parse_args()
    runner=Runner(a);report={'status':'FAIL','workload':{'measured_count':COUNT,'paced_hz':100,'payload_bytes':[64,4096],'observer_timeout_seconds':10,'per_process_rss_budget_bytes':512*1024*1024,'compiler_concurrency':1,'native_processes_are_separate':True,'warmup':'one all-w String, excluded from latency/count'},'results':[]};original=network.policy
    report['workload'].update(multimodule_count_per_instance=COUNT,dual_count_per_backend=COUNT,dual_schedule='interleaved publish calls in one Guest',high_frequency_count=200,high_frequency_paced_hz=1000,ipc_roundtrips=1000,service_calls=100,service_concurrency=4)
    def terminated(signum,frame):
        raise RuntimeError('FAIL bounded performance runner interrupted')
    signal.signal(signal.SIGTERM,terminated)
    try:
        report['cgroup_before']=network.cgroup_snapshot();runner.prepare({1,2})
        for version in (1,2):
            for size in (64,4096):
                topic='/uwvm_ros_perf/output';base=f'ros{version}-{size}'
                binary=f'/tmp/ros{version}-build/uwvm-ros{version}-perf-probe'
                if not (runner.roots[version]/binary.lstrip('/')).exists():raise RuntimeError('BLOCKED missing actual perf probe '+binary)
                for source in (('native','wasm','multimodule') if size==64 else ('native','wasm')):
                    name=base+'-'+source;measured=COUNT*2 if source=='multimodule' else COUNT
                    observer=runner.start(runner.native_command(version,[binary,'--observe',str(measured),str(size),topic]),name+'-observer');runner.ready(observer,name+'-observer')
                    emitter=None
                    try:
                        if source=='native':
                            emitter=runner.start(runner.native_command(version,[binary,'--emit',str(COUNT),str(size),topic]),name+'-emitter')
                            if emitter.wait(timeout=15):raise RuntimeError('FAIL native emitter')
                        else:
                            fixture=runner.run/(name+'.wat');fixture.write_text(wat(version,size));wasm=fixture.with_suffix('.wasm')
                            subprocess.run([a.wat2wasm,'--enable-all',str(fixture),'-o',str(wasm)],check=True,timeout=15)
                            def configured(v,role,message_type):
                                connection,grants=original(v,['publish']);cap=grants[0];cap['resource']=topic
                                cap['limits'].update({'max-message-bytes':str(size+4),'max-rate-hz':'500','burst':'32','max-bytes-per-second':'2097152','max-queue-bytes':'131200','overflow-policy':'reject','audit-max-rate-hz':'500','audit-burst':'32'})
                                if v==1:cap['limits']['queue-depth']='32'
                                else:cap['limits']['qos.depth']='32'
                                return connection,grants
                            network.policy=configured
                            if source=='multimodule':
                                provider=runner.run/(name+'-provider.wat')
                                content=wat(version,size).replace('(func (export "_start")','(func $bench')
                                provider.write_text(content.rsplit(')',1)[0]+'\n(start $bench))\n')
                                preload=provider.with_suffix('.wasm');subprocess.run([a.wat2wasm,'--enable-all',str(provider),'-o',str(preload)],check=True,timeout=15)
                                runner.case(name,str(wasm),{version:['publish']},[],group='perf',preload=str(preload),extra=(f'--ros{version}-group-add-module','perf','main'))
                            else:runner.case(name,str(wasm),{version:['publish']},[])
                            network.policy=original
                        if observer.wait(timeout=15):raise RuntimeError('FAIL native exact-content observer')
                        row={'name':name,'source':source,'version':version,'native_observer':runner.result(name+'-observer'),'cgroup':network.cgroup_snapshot()};report['results'].append(row);print('PASS',name,flush=True)
                    finally:
                        runner.stop(observer)
                        if emitter:runner.stop(emitter)
        for size,measured,pace_ns in ((64,COUNT,10000000),(4096,COUNT,10000000),(64,200,1000000),(4096,200,1000000)):
            high=pace_ns==1000000
            topic='/uwvm_ros_perf/output';name=f'dual-{size}'+('-high-frequency' if high else '');observers=[]
            try:
                for version in (1,2):
                    binary=f'/tmp/ros{version}-build/uwvm-ros{version}-perf-probe';label=name+f'-ros{version}-observer'
                    observer=runner.start(runner.native_command(version,[binary,'--observe',str(measured),str(size),topic]),label);runner.ready(observer,label);observers.append((version,label,observer))
                fixture=runner.run/(name+'.wat');fixture.write_text(dual_wat(size,measured,pace_ns));wasm=fixture.with_suffix('.wasm')
                subprocess.run([a.wat2wasm,'--enable-all',str(fixture),'-o',str(wasm)],check=True,timeout=15)
                def configured(v,role,message_type):
                    connection,grants=original(v,['publish']);cap=grants[0];cap['resource']=topic
                    cap['limits'].update({'max-message-bytes':str(size+4),'max-rate-hz':'2000' if high else '500','burst':'32','max-bytes-per-second':'8388608','max-queue-bytes':'131200','overflow-policy':'reject','audit-max-rate-hz':'500','audit-burst':'32'})
                    cap['limits']['queue-depth' if v==1 else 'qos.depth']='32'
                    return connection,grants
                network.policy=configured;runner.case(name,str(wasm),{1:['publish'],2:['publish']},[]);network.policy=original
                for version,label,observer in observers:
                    if observer.wait(timeout=15):raise RuntimeError('FAIL dual native exact-content observer')
                    report['results'].append({'name':name+f'-ros{version}','source':'dual-wasm','version':version,'configured_pace_hz':1000 if high else 100,'native_observer':runner.result(label),'cgroup':network.cgroup_snapshot()})
                print('PASS',name,flush=True)
            finally:
                for _,_,observer in observers:runner.stop(observer)
        for version in (1,2):
            name=f'ros{version}-high-frequency';size=64;topic='/uwvm_ros_perf/output';binary=f'/tmp/ros{version}-build/uwvm-ros{version}-perf-probe'
            observer=runner.start(runner.native_command(version,[binary,'--observe','200',str(size),topic]),name+'-observer');runner.ready(observer,name+'-observer')
            try:
                fixture=runner.run/(name+'.wat');fixture.write_text(wat(version,size,200,1000000));wasm=fixture.with_suffix('.wasm')
                subprocess.run([a.wat2wasm,'--enable-all',str(fixture),'-o',str(wasm)],check=True,timeout=15)
                def configured(v,role,message_type):
                    connection,grants=original(v,['publish']);cap=grants[0];cap['resource']=topic
                    cap['limits'].update({'max-message-bytes':'68','max-rate-hz':'2000','burst':'32','max-bytes-per-second':'2097152','max-queue-bytes':'2176','overflow-policy':'reject','audit-max-rate-hz':'500','audit-burst':'32'})
                    cap['limits']['queue-depth' if v==1 else 'qos.depth']='32'
                    return connection,grants
                network.policy=configured;runner.case(name,str(wasm),{version:['publish']},[]);network.policy=original
                if observer.wait(timeout=15):raise RuntimeError('FAIL high frequency exact native observer')
                report['results'].append({'name':name,'source':'wasm','version':version,'configured_pace_hz':1000,'native_observer':runner.result(name+'-observer'),'cgroup':network.cgroup_snapshot()});print('PASS',name,flush=True)
            finally:runner.stop(observer)
        for version in (1,2):
            name=f'ros{version}-ipc-service';binary=f'/tmp/ros{version}-build/uwvm-ros{version}-ipc-service-probe';gateway=f'/tmp/ros{version}-build/uwvm-ros{version}-gateway'
            if binary not in runner.frozen:raise RuntimeError('BLOCKED missing actual IPC/service benchmark '+binary)
            process=runner.start(runner.native_command(version,[binary,runner.frozen[gateway]]),name)
            try:
                if process.wait(timeout=30):raise RuntimeError('FAIL actual IPC/service measurement')
                metrics=[json.loads(line) for line in (runner.run/(name+'.log')).read_text().splitlines() if line.startswith('{')]
                if len(metrics)!=2:raise RuntimeError('FAIL missing IPC/service measurements')
                for metric in metrics:report['results'].append({'name':name+'-'+metric['workload'],'source':'trusted-worker-native-gateway','version':version,'measurement':metric,'cgroup':network.cgroup_snapshot()})
                print('PASS',name,flush=True)
            finally:runner.stop(process)
        report['status']='PASS'
    except (RuntimeError,OSError,subprocess.SubprocessError,StopIteration,ValueError) as error:
        report.update(status='BLOCKED' if str(error).startswith('BLOCKED') else 'FAIL',error=str(error));print(report['status'],error,flush=True)
    finally:
        network.policy=original;runner.cleanup();report['process_metrics']=runner.metrics;report['cgroup_after']=network.cgroup_snapshot()
        if any(row.get('budget_exceeded') for row in runner.metrics.values()):report['status']='FAIL';report['error']='bounded process RSS exceeded'
        def events(s):return dict(line.split() for line in s['memory.events'].splitlines())
        if 'cgroup_before' in report and any(events(report['cgroup_before']).get(k)!=events(report['cgroup_after']).get(k) for k in ('oom','oom_kill')):report['status']='FAIL';report['error']='shared target OOM counter increased'
        (runner.run/'performance.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],runner.run/'performance.json',flush=True)
    return 0 if report['status']=='PASS' else 77 if report['status']=='BLOCKED' else 1
if __name__=='__main__':sys.exit(main())
