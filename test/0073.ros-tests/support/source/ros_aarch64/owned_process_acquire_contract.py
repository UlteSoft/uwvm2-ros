#!/usr/bin/env python3
"""Exercise the actual tracker class at a controlled Linux reparent boundary.

Root runs this standalone HOST fixture serially inside its verified cgroup. It
does not load ROS, compile a product or claim ARM execution. The production
class alone is extracted from its pinned source; importing the large controller
would introduce unrelated drivers. A real child exits after pidfd_open while
its child is adopted by this fixture's temporary Linux subreaper. Additional
synthetic procfs observations cover refusal/cleanup paths that must not signal
an unrelated numeric PID. No scenario may signal an unproven candidate.
"""
import argparse
import ast
import ctypes
import hashlib
import json
import os
from pathlib import Path
import signal
import time
import types


def identity(pid):
    fields=Path(f'/proc/{pid}/stat').read_text().rsplit(')',1)[1].split()
    return {'pid':pid,'state':fields[0],'parent_pid':int(fields[1]),'starttime_ticks':int(fields[19])}


def pin(path):
    path=Path(path);raw=path.read_bytes();state=path.stat()
    return {'path':str(path.resolve()),'device':state.st_dev,'inode':state.st_ino,
            'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}


def wait_for(predicate,seconds=3):
    until=time.monotonic()+seconds
    while time.monotonic()<until:
        value=predicate()
        if value:return value
        time.sleep(.001)
    raise TimeoutError('bounded fixture synchronization expired')


def reap(pid):
    """Reap only one fixture-created child, without an unbounded cleanup wait."""
    until=time.monotonic()+3
    while time.monotonic()<until:
        try:waited,status=os.waitpid(pid,os.WNOHANG)
        except ChildProcessError:return None
        if waited==pid:return status
        time.sleep(.001)
    raise TimeoutError('bounded owned fixture reap expired')


def load_class(path):
    body=path.read_text();tree=ast.parse(body,filename=str(path))
    classes=[node for node in tree.body if isinstance(node,ast.ClassDef) and node.name=='OwnedProcesses']
    if len(classes)!=1:raise RuntimeError('exact production tracker class required')
    scope={'os':os,'signal':signal,'Path':Path,'time':time,'process_identity':identity}
    exec(compile(ast.Module(body=classes,type_ignores=[]),str(path),'exec'),scope)
    return scope


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--expected-cgroup',type=Path,required=True)
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args()
    if args.out.exists():raise RuntimeError('fresh fixture report required')
    path=args.source.resolve()/'test/0073.ros-tests/support/source/ros_aarch64/run_ros1_search.py'
    inputs=[pin(path),pin(Path(__file__)),pin(args.expected_cgroup)]
    expected=json.loads(args.expected_cgroup.read_text())
    if set(expected)!={'path','device','inode','boot_id'}:raise RuntimeError('closed cgroup incarnation required')
    leaf=Path(expected['path'])
    def current_cgroup():
        state=leaf.stat()
        result={'path':str(leaf),'device':state.st_dev,'inode':state.st_ino,
                'boot_id':Path('/proc/sys/kernel/random/boot_id').read_text().strip()}
        if (result!=expected or (leaf/'memory.max').read_text().strip()!='68719476736'
                or (leaf/'memory.swap.max').read_text().strip()!='0'):
            raise RuntimeError('fixture requires current exact64GiB/swap0 cgroup')
        membership=Path('/proc/self/cgroup').read_text().splitlines()
        if str(os.getpid()) not in (leaf/'cgroup.procs').read_text().split():raise RuntimeError('fixture outside selected leaf')
        events=dict(line.split() for line in (leaf/'memory.events').read_text().splitlines())
        return {'incarnation':result,'membership':membership,
                'oom':{key:int(events[key]) for key in ('oom','oom_kill','oom_group_kill')}}
    before=current_cgroup();scope=load_class(path);tracker=scope['OwnedProcesses']
    evidence=types.SimpleNamespace(leaf=leaf,before={'membership':before['membership']})
    report={'status':'FAIL','scope':'HOST tracker only; actual Linux reparent plus synthetic refusal observations',
            'tests':[],'input_pins':inputs,'before':before,'ARM_execution':False,'ROS_execution':False}
    def check(name,value,**facts):
        if not value:raise AssertionError(name)
        report['tests'].append({'name':name,'status':'PASS',**facts})
    libc=ctypes.CDLL(None,use_errno=True);old=ctypes.c_int()
    if libc.prctl(37,ctypes.byref(old),0,0,0)!=0:raise OSError(ctypes.get_errno(),'PR_GET_CHILD_SUBREAPER')
    if libc.prctl(36,1,0,0,0)!=0:raise OSError(ctypes.get_errno(),'PR_SET_CHILD_SUBREAPER')
    descriptors=[];births=[];owner=None
    try:
        exit_r,exit_w=os.pipe();descriptors.extend((exit_r,exit_w))
        info_r,info_w=os.pipe();descriptors.extend((info_r,info_w))
        hold_r,hold_w=os.pipe();descriptors.extend((hold_r,hold_w))
        parent=os.fork()
        if parent==0:
            try:
                os.close(exit_w);os.close(info_r);os.close(hold_w)
                child=os.fork()
                if child==0:
                    os.close(exit_r);os.close(info_w)
                    while os.read(hold_r,1):pass
                    os._exit(0)
                os.close(hold_r)
                os.write(info_w,(str(child)+'\n').encode('ascii'));os.close(info_w)
                if os.read(exit_r,1)!=b'E':os._exit(125)
                os._exit(0)
            except BaseException:os._exit(126)
        parent_fd=os.pidfd_open(parent);births.append((parent,parent_fd))
        os.close(info_w);descriptors.remove(info_w)
        raw=b''
        while not raw.endswith(b'\n'):
            part=os.read(info_r,32)
            if not part or len(raw)+len(part)>32:raise RuntimeError('bounded owned child PID record required')
            raw+=part
        child=int(raw);parent_identity=identity(parent)
        child_fd=os.pidfd_open(child);births.append((child,child_fd))
        first=identity(child)
        if first['parent_pid']!=parent:raise RuntimeError('fixture child ancestry was not established')
        real_open=os.pidfd_open;opened=[]
        def reparenting_open(pid):
            if pid!=child:raise AssertionError('unexpected fixture acquisition')
            fd=real_open(pid);opened.append(fd)
            try:
                os.write(exit_w,b'E')
                if reap(parent)!=0:raise AssertionError('owned parent did not exit successfully')
                wait_for(lambda:identity(child)['parent_pid']==os.getpid())
                return fd
            except BaseException:
                os.close(fd);raise
        scope['os']=types.SimpleNamespace(pidfd_open=reparenting_open,close=os.close,getpid=os.getpid)
        # Construct with the real module first, then substitute only the exact
        # pidfd boundary. All identity/cgroup reads and eventual signals are real.
        scope['os']=os;owner=tracker(evidence)
        scope['os']=types.SimpleNamespace(pidfd_open=reparenting_open,close=os.close,getpid=os.getpid)
        row=owner.acquire(child,'controlled-child',[parent,child],parent_identity)
        check('real same-birth reparent is acquired',row is not None and row['acquisition']['parent_changed'],acquisition=row['acquisition'])
        check('original observed ancestry is retained',row['identity']['parent_pid']==parent and row['ancestry']==[parent,child])
        check('reparented owned child passes current cgroup check',owner.check(row))
        repeated=owner.acquire(child,'controlled-child',[parent,child],parent_identity)
        check('known orphan lookup opens no second pidfd',repeated is row and len(opened)==1)
        scope['os']=os;cleanup=owner.stop([]);owner=None
        reap(child)
        check('actual orphan is removed through owned pidfd cleanup',cleanup['status']=='PASS' and not cleanup['remaining_owned_processes'],cleanup=cleanup)

        def modeled(first,second=None,checked=None):
            values=iter([first,second]);closed=[];calls=[]
            def read(pid):
                value=next(values)
                if isinstance(value,BaseException):raise value
                return value
            scope['process_identity']=read
            scope['os']=types.SimpleNamespace(pidfd_open=lambda pid:(calls.append(pid) or 701),close=closed.append,getpid=lambda:100)
            instance=tracker.__new__(tracker);instance.rows={};instance.errors=[];instance.unproven={};instance.check=checked or (lambda row:True)
            return instance,closed,calls
        original={'pid':201,'state':'S','parent_pid':100,'starttime_ticks':20}
        parent_fact={'pid':100,'state':'S','parent_pid':1,'starttime_ticks':10}
        for name,second in (('replacement birth is refused',dict(original,starttime_ticks=21)),
                            ('vanished second observation closes the fd',FileNotFoundError('controlled disappearance'))):
            instance,closed,calls=modeled(original,second)
            try:instance.acquire(201,'model',[100,201],parent_fact)
            except (RuntimeError,FileNotFoundError):pass
            else:raise AssertionError(name)
            check(name,closed==[701] and not instance.rows)
        instance,closed,calls=modeled(dict(original,parent_pid=1))
        check('unproven descendant is recorded without a signal or fd',instance.acquire(201,'model',[100,201],parent_fact) is None
              and not calls and (201,20) in instance.unproven)
        scope['process_identity']=lambda pid:dict(original,parent_pid=1)
        result=instance.stop([])
        check('live unproven candidate prevents a clean result',result['status']=='CLEANUP_INCOMPLETE' and len(result['unproven_live_candidates'])==1)
        scope['process_identity']=lambda pid:dict(original,parent_pid=1,state='Z')
        check('known exited unproven candidate discharges unavailable ancestry',instance.stop([])['status']=='PASS')
        instance,closed,calls=modeled(dict(original,parent_pid=1))
        try:instance.acquire(201,'model',[201])
        except RuntimeError:pass
        else:raise AssertionError('foreign direct root was accepted')
        check('foreign direct root is refused before pidfd_open',not calls and (201,20) in instance.unproven)
        def escaped(row):raise RuntimeError('controlled cgroup rejection')
        instance,closed,calls=modeled(original,dict(original,parent_pid=1),escaped)
        try:instance.acquire(201,'model',[100,201],parent_fact)
        except RuntimeError:pass
        else:raise AssertionError('cgroup rejection was not propagated')
        check('cgroup rejection retains the acquired cleanup responsibility',(201,20) in instance.rows and closed==[])
        scope['os'].close(instance.rows[(201,20)]['pidfd'])
        report['after']=current_cgroup()
        check('input pins and cgroup incarnation/OOM remain unchanged',inputs==[pin(path),pin(Path(__file__)),pin(args.expected_cgroup)]
              and report['after']==before and all(value==0 for value in before['oom'].values()))
        report['status']='PASS';report['checks']=len(report['tests'])
    except BaseException as error:
        report['error']=repr(error)
        raise
    finally:
        scope['os']=os;scope['process_identity']=identity
        if owner is not None:
            try:report['failure_cleanup']=owner.stop([])
            except BaseException as error:
                report['status']='FAIL';report['failure_cleanup_error']=repr(error)
        # These pidfds were opened only for this fixture's explicitly forked
        # births; they cover early exceptions before tracker registration.
        def cleanup_error(error):
            report['status']='FAIL';report.setdefault('fixture_cleanup_errors',[]).append(repr(error))
        for pid,fd in reversed(births):
            try:signal.pidfd_send_signal(fd,signal.SIGKILL)
            except ProcessLookupError:pass
            except BaseException as error:cleanup_error(error)
            try:reap(pid)
            except BaseException as error:cleanup_error(error)
            try:os.close(fd)
            except BaseException as error:cleanup_error(error)
        for fd in descriptors:
            try:os.close(fd)
            except BaseException as error:cleanup_error(error)
        try:
            if libc.prctl(36,old.value,0,0,0)!=0:raise OSError(ctypes.get_errno(),'restore child subreaper')
        except BaseException as error:cleanup_error(error)
        args.out.parent.mkdir(parents=True,exist_ok=True)
        with args.out.open('x') as output:json.dump(report,output,indent=2);output.write('\n')
    if report['status']!='PASS':raise RuntimeError('fixture restoration failed')
    print('PASS_OWNED_PROCESS_ACQUIRE checks='+str(report['checks']))


if __name__=='__main__':main()
