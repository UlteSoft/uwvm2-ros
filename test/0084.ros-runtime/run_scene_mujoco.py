#!/usr/bin/env python3
"""Real ROSv2 Wasm/device/sensor calls against an isolated official MuJoCo binary.

All processes inherit the existing 64 GiB zero-swap gate. The native fixture
uses the production frame/receiver gates and both production seccomp filters,
inside fresh PID/user/network namespaces and a sealed read-only image closure.
This qualifies the scalar binding and concrete fault/permission cases only;
it is not a vendor driver, balance controller, physical robot or safety rating.
"""
import argparse
import fcntl
import hashlib
import json
import os
from pathlib import Path
import pty
import re
import resource
import select
import shlex
import signal
import subprocess
import sys
import threading
import time
import tty

from run_scene_vm import ROOT, GATE, sha, verify_gate, policy
from run_scene_robot_local import (OPERATIONS, DEVICE_PROFILE, CONTROL_PROFILE, SCALAR,
                                  base_guest, device_flags, profile, u64, text, blob, receipt_done)
import run_scene_sensors as sensors
sys.path.insert(0, str(ROOT/'tools/ros_scene'))
from performance_build_policy import compile_policy, vm_policy

def guest(width, memory, scenario):
    g=base_guest(width,memory)
    if scenario=='no-grants':
        g.open('drive',13)
        g.raw('device.open',1,DEVICE_PROFILE,expected=13)
        return g
    sensor=g.open('sensor');frames=g.open('frames');drive=g.open('drive');control=g.open('controller')
    g.raw('sensor.open',sensor,sensors.PROFILE_WIRE);source=g.save('sensor.resource')
    g.polling('sensor.take',sensor,bytes(8),[(0,source)])
    g.raw('device.open',drive,DEVICE_PROFILE,expected=11 if scenario=='rate-zero' else 0)
    if scenario=='rate-zero':return g
    device=g.save('device')
    opening=CONTROL_PROFILE+bytes(16)+u64(1)+u64(1)+bytes(24);at=len(CONTROL_PROFILE)
    patches=[(at,drive),(at+8,device),(at+32,sensor),(at+40,source),(at+48,frames)]
    denied=scenario in ('no-export','no-read','no-device-submit')
    g.raw('control.open',control,opening,patches,expected=13 if denied else 0)
    if denied:return g
    cycle=g.save('cycle');epoch=g.save('controller.epoch',520)
    if scenario=='submit-denied':
        # Removing submission exposure prevents native controller activation.
        # Do not wait for a ready state that this policy deliberately withholds.
        g.raw('control.submit',control,bytes(8),[(0,cycle)],expected=13)
        stop=bytes(8)+u64(1)+bytes(8)+profile('drive.stop')
        receipt_done(g,'control.stop',control,stop,[(0,cycle),(16,cycle)])
        return g
    state=512+len(DEVICE_PROFILE)+len(profile('drive.schema'))+len(profile('drive.stop'))
    g.body.append('loop $ready');g.polling('device.describe',drive,bytes(8),[(0,device)])
    g.body.append(g.word(state)+' i64.const 5 i64.ne if');g.bound();g.body.append('br $ready end end')
    g.exact(state+8,b'ready')
    if scenario=='cycle':
        g.globals.add('iteration');g.body.append('i64.const 1 global.set $iteration loop $cycles')
        g.polling('control.sample',control,bytes(8),[(0,cycle)]);sequence=g.save('cycle.sequence',520)
        g.check(sequence+' global.get $iteration i64.ne')
        body=bytes(8)+u64(1)+bytes(24)+profile('drive.schema')+text('robot.scalar.command')+text('le64')+blob(SCALAR)
        target='global.get $iteration i64.const 1 i64.and i64.eqz if (result i64) i64.const -4620693217682128896 else i64.const 4602678819172646912 end'
        patches=[(0,cycle),(8,'global.get $iteration'),(16,cycle),(24,sequence),(32,epoch),(len(body)-8,target)]
        g.raw('control.submit',control,body,patches)
        g.raw('control.submit',control,body,patches)
        receipt_done(g,'control.result',control,bytes(16),[(0,cycle),(8,sequence)])
        g.polling('sensor.take',sensor,bytes(8),[(0,source)])
        g.check(g.word(528)+' global.get $iteration i64.const 1 i64.add i64.ne')
        g.body.append('global.get $iteration i64.const 1 i64.add global.set $iteration global.get $iteration i64.const 128 i64.le_u br_if $cycles end')
        stop=bytes(8)+u64(1)+bytes(8)+profile('drive.stop')
        receipt_done(g,'control.stop',control,stop,[(0,cycle),(16,cycle)])
        return g
    g.raw('control.sample',control,bytes(8),[(0,cycle)]);sequence=g.save('cycle.sequence',520)
    value=SCALAR if scenario!='out-of-range' else SCALAR[:-8]+__import__('struct').pack('<d',2.0)
    body=bytes(8)+u64(1)+bytes(24)+profile('drive.schema')+text('robot.scalar.command')+text('le64')+blob(value)
    patches=[(0,cycle),(16,cycle),(24,sequence),(32,epoch)]
    expected=13 if scenario in ('submit-denied','out-of-range') else 0
    g.raw('control.submit',control,body,patches,expected=expected);request=g.save('request')
    if expected:
        stop=bytes(8)+u64(1)+bytes(8)+profile('drive.stop')
        receipt_done(g,'control.stop',control,stop,[(0,cycle),(16,cycle)])
        return g
    if scenario=='success':
        # Idempotent replay must not cause a second native effect.
        g.raw('control.submit',control,body,patches)
        receipt_done(g,'control.result',control,bytes(16),[(0,cycle),(8,sequence)])
    # A second actual framed sensor record is emitted only after mj_step.
    g.polling('sensor.take',sensor,bytes(8),[(0,source)])
    g.equal(g.word(520),1);g.equal(g.word(528),2)
    stop=bytes(8)+u64(1)+bytes(8)+profile('drive.stop')
    if scenario in ('success','stop-inflight'):
        receipt_done(g,'control.stop',control,stop,[(0,cycle),(16,cycle)])
        if scenario=='stop-inflight':
            g.raw('control.result',control,bytes(16),[(0,cycle),(8,sequence)])
            g.exact(520,text('outcome_unknown')+text('unknown'))
            g.call('resource.close',drive,[request],expected=11)
            g.polling('device.describe',drive,bytes(8),[(0,device)])
            g.exact(state,text('outcome_unknown'))
    else:
        # The real supervisor must revoke only the failed device/control cohort.
        g.body.append('loop $fault')
        g.dynamic('control.result',control,bytes(16),[(0,cycle),(8,sequence)])
        g.body.append('global.get $status i32.const 13 i32.ne if')
        g.check('global.get $status');g.bound();g.body.append('br $fault end end')
        g.open('drive',13);g.open('controller',13)
        g.raw('device.open',drive,DEVICE_PROFILE,expected=13)
        g.raw('sensor.take',sensor,bytes(8),[(0,source)],expected=11)
    return g

def flags_for(scenario, sensor_fds, device_fds, poll_ns):
    if scenario=='no-grants':
        from run_scene_robot_local import steady
        return steady(policy(OPERATIONS))
    flags=device_flags(sensor_fds,device_fds)
    flags[flags.index('--ros-scene-device-budget')+6]=str(poll_ns)
    if scenario in ('no-export','no-read','no-device-submit'):
        alias,action={'no-export':('frames','data.export'),'no-read':('frames','buffer.read'),
                      'no-device-submit':('drive','device.submit')}[scenario]
        pos=next(i for i in range(len(flags)-6) if flags[i:i+3]==['--ros-scene-grant','robot',alias])
        actions=flags[pos+6].split(',');actions.remove(action);flags[pos+6]=','.join(actions)
    if scenario=='submit-denied':
        pos=next(i for i in range(len(flags)-3) if flags[i:i+3]==['--ros-scene-interface','robot','control.submit'])
        flags[pos+3]='deny'
    # Explicit may-reject attenuation is independently denied at admission.
    if scenario=='rate-zero':
        flags+=['--ros-scene-limit','robot','drive','delivery','may-reject',
                '--ros-scene-limit','robot','drive','calls-per-window','0']
    return flags

class Native:
    def __init__(self, closure, filter_bytes, mode, fds, log, core):
        self.memfds=[];self.prefix=[];self.log=log;self.pidfd=None;self.closed=False;self.reader=None;self.overflow=False
        def sealed(data):
            fd=os.memfd_create('ros-simulation-image',os.MFD_CLOEXEC|os.MFD_ALLOW_SEALING)
            self.memfds.append(fd)
            with os.fdopen(os.dup(fd),'wb') as f:f.write(data)
            os.lseek(fd,0,0)
            fcntl.fcntl(fd,fcntl.F_ADD_SEALS,fcntl.F_SEAL_WRITE|fcntl.F_SEAL_GROW|fcntl.F_SEAL_SHRINK|fcntl.F_SEAL_SEAL)
            return fd
        argv=['/usr/bin/bwrap','--unshare-user','--unshare-ipc','--unshare-pid','--unshare-net','--unshare-uts',
              '--disable-userns','--assert-userns-disabled','--clearenv','--new-session','--die-with-parent','--as-pid-1',
              '--cap-drop','ALL']
        directories=sorted({str(p) for _,dest in closure for p in Path(dest).parents if str(p)!='/'},
                           key=lambda p:(len(Path(p).parts),p))
        for path in directories:argv+=['--dir',path]
        for path,dest in closure:argv+=['--perms','0500','--ro-bind-data',str(sealed(path.read_bytes())),dest]
        info=os.memfd_create('ros-simulation-pid',os.MFD_CLOEXEC)
        release_read,release_write=os.pipe2(os.O_CLOEXEC)
        argv+=['--info-fd',str(info),'--block-fd',str(release_read),'--remount-ro','/','--chdir','/worker','--setenv','LD_LIBRARY_PATH','/lib',
               '--seccomp',str(sealed(filter_bytes)),'--','/worker/receiver',mode,*map(str,fds),'1','official-humanoid']
        def limits():
            os.sched_setaffinity(0,{core})
            resource.setrlimit(resource.RLIMIT_CORE,(0,0))
            resource.setrlimit(resource.RLIMIT_AS,(1073741824,1073741824))
            resource.setrlimit(resource.RLIMIT_CPU,(8,8))
            resource.setrlimit(resource.RLIMIT_NOFILE,(128,128))
            resource.setrlimit(resource.RLIMIT_FSIZE,(134217728,134217728))
        self.argv=argv
        try:
            self.process=subprocess.Popen(argv,stdin=subprocess.DEVNULL,stdout=subprocess.DEVNULL,stderr=subprocess.PIPE,bufsize=0,
                pass_fds=tuple(self.memfds)+tuple(set(fds))+(info,release_read),preexec_fn=limits)
        finally:
            for fd in self.memfds:os.close(fd)
        until=time.monotonic()+10
        while True:
            try:identity=json.loads(os.pread(info,4096,0));break
            except json.JSONDecodeError:
                assert time.monotonic()<until,'Namespace identity timeout'
                time.sleep(.01)
        os.close(info);self.actual_pid=identity['child-pid'];self.pidfd=os.pidfd_open(self.actual_pid)
        self.membership=Path('/proc',str(self.actual_pid),'cgroup').read_text().strip()
        assert self.membership=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
        assert int(Path('/proc',str(self.actual_pid),'status').read_text().split('PPid:')[1].split()[0])==self.process.pid
        os.write(release_write,b'x');os.close(release_write);os.close(release_read)
        assert select.select([self.process.stderr],[],[],10)[0], 'Native startup timeout'
        line=self.process.stderr.readline();self.prefix.append(line)
        try:self.ready=json.loads(line)
        except Exception:
            self.close()
            raise RuntimeError(log.read_text()[-3000:])
        assert self.ready['kind']=='ready' and self.ready['escape_checks']==7
        self.tail=bytearray();self.effect_seen=threading.Event();self.watchdog_seen=threading.Event()
        def drain():
            while data:=os.read(self.process.stderr.fileno(),4096):
                if len(self.tail)+len(data)>262144:
                    self.overflow=True
                    if self.pidfd is not None:
                        try:signal.pidfd_send_signal(self.pidfd,signal.SIGKILL)
                        except ProcessLookupError:pass
                    return
                self.tail.extend(data)
                if b'"kind":"effect"' in self.tail:self.effect_seen.set()
                if b'"cause":"watchdog"' in self.tail:self.watchdog_seen.set()
        self.reader=threading.Thread(target=drain);self.reader.start()
    def close(self):
        if self.closed:return self.process.returncode
        if self.pidfd is not None:
            try:signal.pidfd_send_signal(self.pidfd,signal.SIGKILL)
            except ProcessLookupError:pass
            os.close(self.pidfd);self.pidfd=None
        if self.reader:
            self.process.wait(timeout=10);self.reader.join(timeout=10);assert not self.reader.is_alive()
            tail=self.tail
        else:_,tail=self.process.communicate(timeout=10)
        self.closed=True
        self.log.write_bytes(b''.join(self.prefix)+tail)
        assert not self.overflow,'Native diagnostic byte limit'
        return self.process.returncode

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--runtime',required=True,type=Path);p.add_argument('--source-root',required=True,type=Path)
    p.add_argument('--mujoco',required=True,type=Path);p.add_argument('--output',required=True,type=Path)
    p.add_argument('--smoke',action='store_true');p.add_argument('--cycles-only',action='store_true')
    p.add_argument('--poll-ns',type=int,default=1000000)
    p.add_argument('--scenarios',nargs='+',choices=['success','cycle','stop-inflight','crash','hang','corrupt','stale','vm-crash','vm-pause',
                    'no-grants','no-export','no-read','no-device-submit','submit-denied','out-of-range','rate-zero'])
    a=p.parse_args();assert 10000<=a.poll_ns<=100000000
    verify_gate();assert os.sched_getaffinity(0).issuperset({4,5,6,7})
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(exist_ok=False)
    source=a.source_root.resolve();runtime=a.runtime.resolve();mujoco=a.mujoco.resolve()
    build=json.loads((runtime.parent/'full-build-results.json').read_text())
    assert build['complete'] and build['source_root']==str(source) and build['binary_sha256']==sha(runtime)
    report=dict(complete=False,scope=__doc__,runtime_sha256=sha(runtime),vm_compile_policy=vm_policy(runtime),
                cgroup=str(GATE),memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),
                placement=dict(vm_cpus=[4,5,6],native_cpu=7,device_poll_ns=a.poll_ns,native_poll_ns=100000,
                               numa_nodes=Path('/sys/devices/system/node/online').read_text().strip(),realtime_scheduler=False),
                inputs={},retained_inputs={},commands=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def pin(path):report['inputs'][str(path.resolve())]=sha(path)
    for path in [*Path(__file__).parent.glob('run_scene_*.py'),Path(__file__).with_name('mujoco_device_receiver.cpp'),
                 ROOT/'tools/ros_scene/performance_build_policy.py',
                 source/'source-manifest.json',runtime.parent/'full-build-results.json',mujoco.parent/'provenance.json']:
        pin(path)
        if path.resolve().is_relative_to(ROOT):
            retained=out/'fixture-source'/path.resolve().relative_to(ROOT)
            retained.parent.mkdir(parents=True,exist_ok=True);retained.write_bytes(path.read_bytes())
            assert sha(retained)==report['inputs'][str(path.resolve())]
            report['retained_inputs'][str(path.resolve())]=str(retained);pin(retained)
    def run(name,argv,env=None):
        verify_gate();t=time.monotonic();r=subprocess.run(argv,capture_output=True,env=env,timeout=240)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        report['commands'].append(dict(name=name,argv=argv,exit=r.returncode,seconds=time.monotonic()-t));save()
        assert r.returncode==0,(name,r.stderr.decode(errors='replace')[-2500:]);return r
    success=False
    try:
        fixture=out/'mujoco_device_receiver.cpp';fixture.write_bytes(Path(__file__).with_name(fixture.name).read_bytes());pin(fixture)
        binary=out/'receiver'
        flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(source/'src'),
               '-I'+str(source/'third-parties/fast_io/include'),'-I'+str(source/'test/0084.ros-runtime'),
               '-I'+str(mujoco/'include')]
        compile=['/usr/bin/clang++-22',*flags,str(fixture),'-L'+str(mujoco/'lib'),'-Wl,-rpath,/lib','-lmujoco','-o',str(binary)]
        report['native_compile_policy']=compile_policy(compile)
        dependencies=run('dependencies',['/usr/bin/clang++-22',*flags,'-MM','-MT','fixture',str(fixture)])
        for path in shlex.split(dependencies.stdout.decode().replace('\\\n',' '))[1:]:pin(Path(path))
        run('compile',compile);pin(binary)
        env=dict(os.environ,LD_LIBRARY_PATH=str(mujoco/'lib'))
        libraries=run('libraries',['/usr/bin/ldd',str(binary)],env).stdout.decode()
        closure=[(binary,'/worker/receiver'),(mujoco/'model/humanoid/humanoid.xml','/fixture/model.xml')]
        for line in libraries.splitlines():
            match=re.search(r'(?:=> )?(/[^ ]+)',line)
            if match:
                path=Path(match.group(1));dest='/lib/'+path.name if 'mujoco' in path.name else str(path)
                closure.append((path,dest))
        assert any('libmujoco' in dest for _,dest in closure)
        for path,_ in closure:pin(path)
        pin(Path('/usr/bin/bwrap'))
        filter_bytes=run('filter',[str(binary),'--filter'],env).stdout
        report['closure']=[dict(path=str(path),destination=dest,sha256=sha(path)) for path,dest in closure]
        report['startup_filter_sha256']=hashlib.sha256(filter_bytes).hexdigest()
        # An equivalent direct native physics step, in the same sandbox.
        dummy=os.open('/dev/null',os.O_RDWR);direct=Native(closure,filter_bytes,'direct',[dummy]*5,out/'direct.log',7)
        direct.process.wait(timeout=10);assert direct.close()==0;os.close(dummy)
        direct_rows=[json.loads(line) for line in (out/'direct.log').read_text().splitlines() if line.startswith('{')]
        reference=next(row for row in direct_rows if row['kind']=='effect');report['native_reference']=reference
        dummy=os.open('/dev/null',os.O_RDWR);direct=Native(closure,filter_bytes,'direct-cycle',[dummy]*5,out/'direct-cycle.log',7)
        direct.process.wait(timeout=10);assert direct.close()==0;os.close(dummy)
        cycle_reference=[json.loads(line) for line in (out/'direct-cycle.log').read_text().splitlines() if line.startswith('{"kind":"effect"')]
        assert len(cycle_reference)==128
        report['native_cycle_reference']=cycle_reference
        scenarios=a.scenarios or (['cycle'] if a.cycles_only else ['success'] if a.smoke else ['success','cycle','stop-inflight','crash','hang','corrupt','stale','vm-crash','vm-pause',
                    'no-grants','no-export','no-read','no-device-submit','submit-denied','out-of-range','rate-zero'])
        layouts=[(32,0)] if a.smoke else [(32,0),(64,0),(32,1),(64,1)]
        for width,memory in layouts:
            for scenario in scenarios:
                name=f'{scenario}-{width}-{memory}';g=guest(width,memory,scenario)
                wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                assert g.cursor<45000
                run(name+'-compile',['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]);pin(wat);pin(wasm)
                for engine in ('int','aot'):
                    label=name+'-'+engine;pairs=[];pipes=[];peer=None;record=dict(name=label,passed=False)
                    try:
                        pairs=[pty.openpty() for _ in range(4)]
                        for master,slave in pairs:tty.setraw(slave);os.set_blocking(master,False);os.set_blocking(slave,False)
                        pipes=[os.pipe2(os.O_NONBLOCK|os.O_CLOEXEC) for _ in sensors.ALIASES]
                        for rd,wr in pipes:assert os.write(wr,sensors.PACKET)==len(sensors.PACKET)
                        mode={'stop-inflight':'withhold','vm-crash':'withhold','vm-pause':'withhold'}.get(scenario,scenario)
                        if mode not in ('cycle','crash','hang','corrupt','stale','withhold'):mode='normal'
                        peer=Native(closure,filter_bytes,mode,[slave for _,slave in pairs]+[pipes[0][1]],out/(label+'.native.log'),7)
                        # No parent copy may hide EOF when the actual peer crashes.
                        for _,slave in pairs:os.close(slave)
                        sensor_fds=[rd for rd,wr in pipes];device_fds=[master for master,_ in pairs]
                        flags=flags_for(scenario,sensor_fds,device_fds,a.poll_ns)
                        argv=['taskset','-c','4-6',str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0',
                              '--runtime-llvm-jit-cache-path','disable','--log-color','disable','--runtime-'+engine,*flags,'--run',str(wasm)]
                        t=time.monotonic()
                        with subprocess.Popen(argv,pass_fds=tuple(sensor_fds+device_fds),stdout=subprocess.PIPE,stderr=subprocess.PIPE) as vm:
                            try:
                                if scenario in ('vm-crash','vm-pause'):
                                    assert peer.effect_seen.wait(10),'No physical effect before VM fault injection'
                                    vm.send_signal(signal.SIGKILL if scenario=='vm-crash' else signal.SIGSTOP)
                                    assert peer.watchdog_seen.wait(4),'External driver watchdog did not expire without VM execution'
                                    if scenario=='vm-pause':vm.send_signal(signal.SIGCONT)
                                stdout,stderr=vm.communicate(timeout=45)
                            except BaseException:
                                vm.kill();vm.wait(timeout=10);raise
                        record.update(argv=argv,vm_exit=vm.returncode,wall_seconds=time.monotonic()-t,native_cgroup=peer.membership,assertions=len(g.checks))
                        (out/(label+'.stdout')).write_bytes(stdout);(out/(label+'.stderr')).write_bytes(stderr)
                        assert vm.returncode==(-signal.SIGKILL if scenario=='vm-crash' else 0),(label,stderr.decode(errors='replace')[-4500:])
                        if scenario=='crash':peer.process.wait(timeout=3)
                        record['native_exit']=peer.close();peer=None
                        log=(out/(label+'.native.log')).read_text()
                        rows=[json.loads(line) for line in log.splitlines() if line.startswith('{')]
                        effects=[row for row in rows if row['kind']=='effect'];stops=[row for row in rows if row['kind']=='stop']
                        active=scenario in ('success','stop-inflight','crash','hang','corrupt','stale','vm-crash','vm-pause')
                        assert len(effects)==(128 if scenario=='cycle' else int(active)),(label,log)
                        for index,row in enumerate(effects):
                            selected=cycle_reference[index] if scenario=='cycle' else reference
                            for key in ('step','time','position','velocity'):assert row[key]==selected[key],(label,key,row,selected)
                        if scenario in ('success','cycle','stop-inflight','submit-denied','out-of-range'):
                            assert any(row['cause']=='host' for row in stops),(label,log)
                        if scenario in ('vm-crash','vm-pause'):assert any(row['cause']=='watchdog' for row in stops)
                        assert 'fixture failure:' not in log,(label,log)
                        if scenario=='crash':assert record['native_exit'] in (-signal.SIGSEGV,128+signal.SIGSEGV)
                        record.update(passed=True,effects=effects,stops=stops,escape_checks=7)
                        print('PASS '+label,flush=True)
                    finally:
                        if peer:record['native_exit']=peer.close()
                        for master,_ in pairs:os.close(master)
                        for pair in pipes:
                            for fd in pair:os.close(fd)
                        report['cases'].append(record);save()
        success=True
    finally:
        report['inputs_unchanged']=all(sha(Path(path))==digest for path,digest in report['inputs'].items())
        report['events_after']=(GATE/'memory.events').read_text();report['complete']=success and report['inputs_unchanged'];save()
    assert report['complete']

if __name__=='__main__':main()
