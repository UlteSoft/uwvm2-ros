#!/usr/bin/env python3
"""Run independent native Action response budget modes in a fresh ROS workspace.

Execute as root inside the existing SSH container's /host chroot. The completed
--baseline is read-only. --rootfs supplies the existing mounted Jazzy SDK. A
new probe is copied into the fresh workspace, with an optional reviewed Action
header overlay. Expected old-code failures keep their actual FAIL rows and rc1.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import posixpath
import shlex
import shutil
import signal
import subprocess
import sys
import time
import traceback


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--baseline',type=Path,required=True)
    parser.add_argument('--rootfs',type=Path,required=True)
    parser.add_argument('--workspace',type=Path,required=True)
    parser.add_argument('--probe',type=Path,required=True)
    parser.add_argument('--action-header',type=Path)
    parser.add_argument('--expected',type=int,choices=(0,1),required=True)
    parser.add_argument('--failure',help='Required exact semantic failure marker for old-code modes')
    parser.add_argument('--modes',nargs='+',default=[f'{budget}-{kind}' for budget in ('zero','call','cap','goal') for kind in ('feedback','success','abort','canceled')])
    parser.add_argument('--domain',type=int,default=188)
    args=parser.parse_args()
    allowed={f'{budget}-{kind}' for budget in ('zero','call','cap','goal') for kind in ('feedback','success','abort','canceled')}
    if os.geteuid()!=0 or sys.platform!='linux' or args.workspace.exists():
        parser.error('root in the existing Linux container and a fresh workspace are required')
    if len(set(args.modes))!=len(args.modes) or any(mode not in allowed for mode in args.modes):
        parser.error('provide distinct documented independent modes; all is deliberately run as separate modes')
    if not 0<=args.domain<=232 or args.expected and not args.failure:
        parser.error('valid isolated domain and an exact marker for expected old-code failures are required')
    base=args.baseline.resolve();sdk=args.rootfs.resolve();out=args.workspace.absolute()
    if any(out==p or p in out.parents or out in p.parents for p in (base,sdk,args.probe.resolve())):
        parser.error('workspace overlaps a frozen input')
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    def inventory(path):
        return {str(p.relative_to(path)):sha(p) for p in path.rglob('*') if p.is_file()}
    def cgroup():
        membership=Path('/proc/self/cgroup').read_text().splitlines()
        if len(membership)!=1 or not membership[0].startswith('0::'):
            raise RuntimeError('unified supplied cgroup required')
        path=Path('/sys/fs/cgroup')/membership[0].split(':',2)[2].lstrip('/')
        events={k:int(v) for k,v in (line.split() for line in (path/'memory.events').read_text().splitlines())}
        result={'membership':membership,'path':str(path),'memory.max':(path/'memory.max').read_text().strip(),
                'memory.events':events,'self_live':str(os.getpid()) in (path/'cgroup.procs').read_text().split()}
        if result['memory.max']!='68719476736' or not result['self_live']:
            raise RuntimeError('supplied live 64 GiB cgroup not proven')
        return result
    before=cgroup();frozen=inventory(base/'source');binaries=inventory(base/'native-binaries')
    if len(binaries)!=18 or json.loads((base/'reports/report.json').read_text())['status']!='PASS':
        raise RuntimeError('completed baseline source, 18 binaries and PASS report required')
    protected_report=sha(base/'reports/report.json')
    out.mkdir();reports=out/'reports';reports.mkdir()
    source=out/'source';scratch=out/'native-temp/jazzy';root=out/'gateway/jazzy-rootfs'
    rows=[];mounts=[];children=[]
    report={'status':'FAIL','scope':'Independent native Action response operation/capability/goal budgets; not a full A15 rerun',
            'workspace':str(out),'domain':args.domain,'cgroup_before':before,'commands':rows,'executed_driver_sha256':sha(Path(__file__)),
            'baseline_source_before':frozen,'baseline_binaries_before':binaries,'baseline_report_before':protected_report,
            'expected_exit':args.expected,'expected_semantic_marker':args.failure,'probe_input_sha256':sha(args.probe)}
    wrapper=reports/'exec_in_cgroup.py'
    wrapper.write_text('''import json,os,sys
from pathlib import Path
proof=Path(sys.argv[1]);expected=json.loads(sys.argv[2]);command=sys.argv[3:]
membership=Path('/proc/self/cgroup').read_text().splitlines();assert membership==expected
path=Path('/sys/fs/cgroup')/membership[0].split(':',2)[2].lstrip('/')
assert str(os.getpid()) in (path/'cgroup.procs').read_text().split()
proof.write_text(json.dumps({'pid':os.getpid(),'membership':membership,'live_cgroup_procs':True,'exec_command':command},indent=2)+'\\n')
os.execvpe(command[0],command,os.environ)
''')
    def chroot(arguments):
        return ['chroot',str(root),'/usr/bin/env','-u','CC','-u','CXX','-u','LD_LIBRARY_PATH',
                f'ROS_DOMAIN_ID={args.domain}','RMW_IMPLEMENTATION=rmw_fastrtps_cpp','ROS_SECURITY_ENABLE=false','ROS_LOG_DIR=/tmp/action-budget-logs',
                '/bin/bash','-lc','source /opt/ros/jazzy/setup.bash && exec "$@"','action-budget',*arguments]
    def run(command,name,expected=0,artifact=None,timeout=60):
        log=reports/(name+'.log');proof=reports/(name+'-exec.json')
        row={'name':name,'command':command,'status':'FAIL'};rows.append(row)
        with log.open('wb') as output:
            child=subprocess.Popen([sys.executable,str(wrapper),str(proof),json.dumps(before['membership']),*command],
                                   stdout=output,stderr=subprocess.STDOUT,start_new_session=True)
            children.append(child);row['pid']=child.pid;until=time.monotonic()+timeout
            try:
                while child.poll() is None:
                    if artifact is not None and 'actual_main_live_PID' not in row:
                        pending=[(child.pid,[child.pid])];seen=set()
                        while pending:
                            pid,ancestry=pending.pop()
                            if pid in seen:continue
                            seen.add(pid)
                            try:
                                pending.extend((int(n),ancestry+[int(n)]) for n in Path(f'/proc/{pid}/task/{pid}/children').read_text().split())
                                proc=Path(f'/proc/{pid}');actual=(proc/'exe').stat();wanted=artifact.stat()
                                if (actual.st_dev,actual.st_ino)!=(wanted.st_dev,wanted.st_ino):continue
                                membership=(proc/'cgroup').read_text().splitlines()
                                live=str(pid) in (Path(before['path'])/'cgroup.procs').read_text().split()
                                parent=int(next(line.split()[1] for line in (proc/'status').read_text().splitlines() if line.startswith('PPid:')))
                                digest=sha(proc/'exe')
                                if membership!=before['membership'] or not live or digest!=sha(artifact) or len(ancestry)<2 or parent!=ancestry[-2]:
                                    raise RuntimeError('real native ELF PID/artifact/parent/cgroup mismatch')
                                row['actual_main_live_PID']={'pid':pid,'parent_pid':parent,'ancestry':ancestry,'exe_path':os.readlink(proc/'exe'),
                                    'exe_device':actual.st_dev,'exe_inode':actual.st_ino,'exe_sha256':digest,'membership':membership,'live_cgroup_procs':live}
                            except (FileNotFoundError,ProcessLookupError):pass
                    if time.monotonic()>=until:raise TimeoutError(name+' bounded execution expired')
                    time.sleep(.01)
                code=child.wait()
                row.update(returncode=code,log_path=str(log),log_sha256=sha(log),status='PASS' if code==0 else 'FAIL')
                row['pre_exec_live_PID']=json.loads(proof.read_text())
                if row['pre_exec_live_PID']['pid']!=child.pid or row['pre_exec_live_PID']['membership']!=before['membership']:
                    raise RuntimeError('pre-exec cgroup proof mismatch')
                if code!=expected or artifact is not None and 'actual_main_live_PID' not in row:
                    raise RuntimeError('unexpected exit or missing real native main proof: '+name)
                if expected:
                    row['expected_failure']=True
                    if args.failure not in log.read_text():raise RuntimeError('missing exact semantic failure marker: '+name)
                    row['actual_failure_marker']=args.failure
                print(('EXPECTED FAIL ' if expected else 'PASS ')+name,flush=True)
            finally:
                if child.poll() is None:os.killpg(child.pid,signal.SIGKILL);child.wait(timeout=5)
    def interrupted(signum,frame):raise RuntimeError('interrupted; bounded owned cleanup follows')
    signal.signal(signal.SIGINT,interrupted);signal.signal(signal.SIGTERM,interrupted)
    try:
        shutil.copytree(base/'source',source,symlinks=True)
        probe=source/'test/0073.ros-tests/support/source/ros_action/respond_budget_ros2.cc';shutil.copy2(args.probe,probe)
        if args.action_header:
            header=source/'src/ros/gateway/ros2/action.h';shutil.copy2(args.action_header,header)
            report['action_overlay_sha256']=sha(header)
        report['production_action_sha256']=sha(source/'src/ros/gateway/ros2/action.h')
        report['candidate_source_inputs']=inventory(source)
        root.mkdir(parents=True);scratch.mkdir(parents=True)
        subprocess.run(['mount','--rbind',str(sdk),str(root)],check=True);mounts.append(root)
        subprocess.run(['mount','--make-rprivate',str(root)],check=True)
        subprocess.run(['mount','--bind',str(source),str(root/'work')],check=True)
        subprocess.run(['mount','--bind',str(scratch),str(root/'tmp')],check=True)
        metadata=base/'native-temp/jazzy/ros2-continuation-build/CMakeFiles/uwvm-ros2-action-probe.dir'
        report['compile_metadata_sha256']=sha(metadata/'flags.make');report['link_metadata_sha256']=sha(metadata/'link.txt')
        flags={}
        for line in (metadata/'flags.make').read_text().splitlines():
            if ' = ' in line:
                key,value=line.split(' = ',1);flags[key]=shlex.split(value)
        compile=['/usr/bin/c++',*flags['CXX_DEFINES'],*flags['CXX_INCLUDES'],'-std=gnu++17','-Wall','-Wextra','-Wpedantic',
                 '-O1','-g0','-MMD','-MF','/tmp/respond-budget.d','-c','/work/test/0073.ros-tests/support/source/ros_action/respond_budget_ros2.cc','-o','/tmp/respond-budget.o']
        run(chroot(compile),'native-compile',timeout=120)
        dependency=(scratch/'respond-budget.d').read_text().replace('\\\n',' ')
        project={}
        for token in shlex.split(dependency.split(':',1)[1]):
            canonical=posixpath.normpath(token)
            if canonical.startswith('/work/'):
                relative=canonical[len('/work/'):];project[relative]=sha(source/relative)
        if project.get('src/ros/gateway/ros2/action.h')!=report['production_action_sha256'] or project.get('test/0073.ros-tests/support/source/ros_action/respond_budget_ros2.cc')!=report['probe_input_sha256']:
            raise RuntimeError('MMD does not prove exact production/probe compilation')
        report['compiled_project_inputs']=project;report['dependency_sha256']=sha(scratch/'respond-budget.d')
        link=shlex.split((metadata/'link.txt').read_text());objects=[i for i,token in enumerate(link) if token.endswith('.o')]
        if len(objects)!=1:raise RuntimeError('one native Action translation unit link template required')
        link[objects[0]]='/tmp/respond-budget.o';link[link.index('-o')+1]='/tmp/respond-budget';link.extend(['-rdynamic','-ldl'])
        run(chroot(link),'native-link',timeout=60)
        artifact=scratch/'respond-budget';report['native_binary_sha256']=sha(artifact)
        for mode in args.modes:run(chroot(['/tmp/respond-budget',mode]),'native-'+mode,args.expected,artifact,30)
        report['status']='PASS'
    except BaseException:
        report['exception']=traceback.format_exc()
    finally:
        for child in children:
            if child.poll() is None:os.killpg(child.pid,signal.SIGKILL);child.wait(timeout=5)
        clean=[]
        for mount in reversed(mounts):
            result=subprocess.run(['umount','-R',str(mount)],capture_output=True,text=True,timeout=15)
            clean.append({'mount':str(mount),'returncode':result.returncode,'output':result.stdout+result.stderr})
        report['cleanup']=clean
        report['remaining_owned_mounts']=[line for line in Path('/proc/self/mountinfo').read_text().splitlines() if str(out) in line]
        live=[];unreadable=[]
        for proc in Path('/proc').iterdir():
            if not proc.name.isdecimal():continue
            try:
                paths={name:os.readlink(proc/name) for name in ('exe','cwd','root')}
                if any(value==str(out) or value.startswith(str(out)+'/') for value in paths.values()):live.append({'pid':int(proc.name),**paths})
            except (FileNotFoundError,ProcessLookupError):pass
            except PermissionError:unreadable.append(int(proc.name))
        report['remaining_owned_live_processes']=live;report['postflight_unreadable_pids']=unreadable
        report['baseline_source_after']=inventory(base/'source');report['baseline_binaries_after']=inventory(base/'native-binaries')
        report['baseline_report_after']=sha(base/'reports/report.json')
        report['baseline_unchanged']=report['baseline_source_after']==frozen and report['baseline_binaries_after']==binaries and report['baseline_report_after']==protected_report
        after=cgroup();report['cgroup_after']=after
        report['oom_delta']={key:after['memory.events'][key]-before['memory.events'][key] for key in ('oom','oom_kill','oom_group_kill')}
        if not report['baseline_unchanged'] or any(report['oom_delta'].values()) or live or unreadable or report['remaining_owned_mounts'] or any(row['returncode'] for row in clean):
            report['status']='FAIL'
        (reports/'report.json').write_text(json.dumps(report,indent=2)+'\n')
        print(json.dumps({'status':report['status'],'report':str(reports/'report.json'),'report_sha256':sha(reports/'report.json'),
                          'commands':len(rows),'compiled_project_inputs':len(report.get('compiled_project_inputs',{})),
                          'oom_delta':report['oom_delta'],'baseline_unchanged':report['baseline_unchanged'],
                          'remaining_owned_live_processes':live,'remaining_owned_mounts':report['remaining_owned_mounts']},indent=2),flush=True)
    return 0 if report['status']=='PASS' else 1


if __name__=='__main__':
    raise SystemExit(main())
