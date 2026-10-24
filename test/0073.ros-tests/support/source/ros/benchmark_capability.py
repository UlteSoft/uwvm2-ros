#!/usr/bin/env python3
"""Bounded authorization cost; this is explicitly not a ROS network benchmark."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import shlex
import subprocess
import sys
import time


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args()
    source=Path(__file__).resolve().parents[5];out=args.out.resolve();out.mkdir()
    sys.path.insert(0,str(source/'test/0073.ros-tests/support/source/ros_cgroup'))
    from probe import probe
    before=probe()
    if before['status']!='PASS': print('BLOCKED provided cgroup not proven');return 77
    sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    entry=source/'test/0073.ros-tests/support/source/ros_security/capability_benchmark.cc'
    initial={str(p.relative_to(source)):sha(p) for p in [entry,*sorted((source/'src/uwvm2/imported/ros').rglob('*.h'))]}
    initial.update({str(p.relative_to(source)):sha(p) for p in sorted((source/'third-parties/fast_io/include').rglob('*')) if p.is_file()})
    binary=out/'capability-benchmark';dep=out/'capability-benchmark.d'
    command=[*shlex.split(os.environ.get('CXX','clang++')),'-std=c++26','-stdlib=libc++','-O2','-g0','-pthread','-MMD','-MF',str(dep),'-I',str(source/'src'),'-I',str(source/'third-parties/fast_io/include'),str(entry),'-o',str(binary)]
    report={'status':'FAIL','network_test':False,'command':command,'cgroup_before':before,
            'runtime_address_space_budget_bytes':64*1024*1024,'runtime_timeout_seconds':12,'compile_timeout_seconds':180}
    try:
        with (out/'compile.log').open('wb') as log:
            compiled=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,timeout=180)
        if compiled.returncode: raise RuntimeError('benchmark build failed '+str(compiled.returncode))
        dependencies=shlex.split(dep.read_text().split(':',1)[1].replace('\\\n',''))
        inputs=[]
        for value in sorted(set(dependencies)):
            p=Path(value).resolve()
            if not p.is_relative_to(source): raise RuntimeError('project dependency outside source')
            name=str(p.relative_to(source))
            if name not in initial or sha(p)!=initial[name]:raise RuntimeError('compiler input changed: '+name)
            dest=out/'inputs'/name;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(p.read_bytes())
            inputs.append({'path':name,'sha256':initial[name],'bytes':p.stat().st_size})
        report.update(source_inputs=inputs,source_sha256=hashlib.sha256(json.dumps(inputs,sort_keys=True).encode()).hexdigest(),binary_sha256=sha(binary))
        def budget():
            resource.setrlimit(resource.RLIMIT_AS,(64*1024*1024,64*1024*1024))
            resource.setrlimit(resource.RLIMIT_CPU,(10,11))
        process=subprocess.Popen([str(binary)],stdout=subprocess.PIPE,stderr=subprocess.PIPE,preexec_fn=budget)
        report['pid']=process.pid;report['membership']=Path(f'/proc/{process.pid}/cgroup').read_text().splitlines()
        report['live_pid_membership_verified']=str(process.pid) in Path(before['cgroup_ancestors'][0]['path'],'cgroup.procs').read_text().splitlines()
        if report['membership']!=before['membership'] or not report['live_pid_membership_verified']:
            process.kill();process.wait();raise RuntimeError('benchmark process cgroup mismatch')
        started=time.monotonic();max_rss=0
        while True:
            pid,status,usage=os.wait4(process.pid,os.WNOHANG)
            if pid:break
            try:
                for line in Path(f'/proc/{process.pid}/status').read_text().splitlines():
                    if line.startswith('VmRSS:'):max_rss=max(max_rss,int(line.split()[1])*1024)
            except FileNotFoundError:pass
            if time.monotonic()-started>12:
                process.kill();os.wait4(process.pid,0);process.returncode=-9;raise RuntimeError('benchmark bounded timeout')
            time.sleep(.002)
        process.returncode=os.waitstatus_to_exitcode(status)
        stdout=process.stdout.read();stderr=process.stderr.read();(out/'stdout.log').write_bytes(stdout);(out/'stderr.log').write_bytes(stderr)
        report.update(returncode=process.returncode,wall_seconds=time.monotonic()-started,cpu_user_seconds=usage.ru_utime,
                      cpu_system_seconds=usage.ru_stime,peak_rss_bytes=usage.ru_maxrss*1024,sampled_rss_bytes=max_rss,
                      voluntary_context_switches=usage.ru_nvcsw,involuntary_context_switches=usage.ru_nivcsw)
        if process.returncode: raise RuntimeError('benchmark execution failed')
        report['benchmark']=json.loads(stdout)
        if report['benchmark']['status']!='PASS' or report['benchmark']['iterations']!=100000: raise RuntimeError('benchmark incomplete')
        after=probe();report['cgroup_after']=after
        if after['status']!='PASS':raise RuntimeError('final cgroup verification failed')
        events=lambda snap:dict(line.split() for line in snap['cgroup_ancestors'][0]['memory.events'].splitlines())
        if any(events(before).get(key)!=events(after).get(key) for key in ('oom','oom_kill')):raise RuntimeError('OOM counter increased')
        if any(sha(source/row['path'])!=row['sha256'] for row in inputs):raise RuntimeError('input changed during runtime')
        report['status']='PASS'
    except Exception as error:report['error']=str(error)
    (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],out/'report.json',flush=True)
    return 0 if report['status']=='PASS' else 1


if __name__=='__main__':raise SystemExit(main())
