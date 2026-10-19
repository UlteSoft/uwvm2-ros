#!/usr/bin/env python3
"""Build and verify bounded ROS2 results and Action timer retirement in the supplied cgroup.

Run as root inside the existing SSH container's /host chroot. --baseline is the
previous frozen delivery workspace; --overlay is a directory with manifest.json
mapping reviewed project-relative source paths to SHA256. A fresh --workspace
keeps the previous sources, native builds and reports intact. The supplied ROS
rootfs trees are recursively bound into this workspace and detached on exit.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import shutil
import signal
import subprocess
import sys
import time


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--workspace', type=Path, required=True)
    p.add_argument('--baseline', type=Path, required=True)
    p.add_argument('--overlay', type=Path, required=True)
    p.add_argument('--alias', type=Path, default=Path('/u9'))
    p.add_argument('--llvm-libs', required=True)
    a = p.parse_args()
    base, out, overlay = a.baseline.resolve(), a.workspace.absolute(), a.overlay.resolve()
    if out.exists() or a.alias.exists() or os.geteuid() != 0:
        p.error('root, a fresh workspace and unused short alias are required')
    if not re.fullmatch(r'/[a-z][0-9]+', str(a.alias)):
        p.error('alias must be a short absolute path such as /u9')
    source, reports = out/'source', out/'reports'
    reports.mkdir(parents=True)
    sys.path.insert(0, str(base/'source/tests/ros_cgroup'))
    from probe import probe
    before = probe()
    if before['status'] != 'PASS':
        raise RuntimeError('BLOCKED supplied 64 GiB cgroup not proven')
    sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    inventory = lambda root: {str(x.relative_to(root)):sha(x) for x in root.rglob('*') if x.is_file()}
    baseline = inventory(base/'source')
    previous_binaries={str(x.relative_to(base)):sha(x)
        for distribution,version in (('noetic',1),('jazzy',2))
        for x in (base/'gateway'/(distribution+'-rootfs')/f'tmp/ros{version}-build').glob(f'uwvm-ros{version}-*') if x.is_file()}
    if len(previous_binaries) != 18:
        raise RuntimeError('expected all 18 frozen native baseline binaries')
    patches = json.loads((overlay/'manifest.json').read_text())
    mount_roots, children, rows, binaries = [], [], [], {}
    report = {'status':'FAIL', 'scope':'ROS2 follow-up, not a new full A15 acceptance',
              'cgroup_before':before, 'commands':rows, 'baseline_source_inputs':baseline,
              'overlay_inputs':patches, 'native_binaries':binaries,
              'previous_native_binary_inputs':previous_binaries,
              'executed_driver_sha256':sha(Path(__file__)),
              'sanitizers':'Standalone bounded-text checks use ASan/UBSan; native middleware libraries are not instrumented.'}
    wrapper = reports/'exec_in_cgroup.py'
    wrapper.write_text('''import json,os,sys
from pathlib import Path
proof=Path(sys.argv[1]); expected=sys.argv[2]; command=sys.argv[3:]
membership=Path('/proc/self/cgroup').read_text().splitlines()
assert membership==json.loads(expected)
unified=membership[0].split(':',2)[2]
visible=Path('/sys/fs/cgroup')/unified.lstrip('/')
assert str(os.getpid()) in (visible/'cgroup.procs').read_text().split()
proof.write_text(json.dumps({'pid':os.getpid(),'membership':membership,'live_cgroup_procs':True,'exec_command':command,
    'environment':{key:os.environ.get(key) for key in ('ROS_DOMAIN_ID','RMW_IMPLEMENTATION','ROS_SECURITY_ENABLE')}},indent=2)+'\\n')
os.execvpe(command[0],command,os.environ)
''')

    def run(command, name, timeout=120, env=None):
        proof=reports/(name+'-exec.json')
        full=[sys.executable,str(wrapper),str(proof),json.dumps(before['membership']),*map(str,command)]
        row={'name':name,'command':list(map(str,command)),'status':'FAIL'};rows.append(row)
        with (reports/(name+'.log')).open('wb') as log:
            child=subprocess.Popen(full,stdout=log,stderr=subprocess.STDOUT,start_new_session=True,env=env)
            children.append(child);row['pid']=child.pid
            try:code=child.wait(timeout=timeout)
            except subprocess.TimeoutExpired:
                os.killpg(child.pid,signal.SIGKILL);child.wait();raise
        row['returncode']=code;row['log_sha256']=sha(reports/(name+'.log'))
        if proof.exists():row['pre_exec_live_PID']=json.loads(proof.read_text())
        if code or not proof.exists():raise RuntimeError('FAIL '+name+'; see '+str(reports/(name+'.log')))
        row['status']='PASS';print('PASS '+name,flush=True)

    def chroot(root, distribution, arguments):
        return ['chroot',str(root),'/usr/bin/env','-u','CC','-u','CXX','-u','LD_LIBRARY_PATH',
                '/bin/bash','-lc',f'source /opt/ros/{distribution}/setup.bash && exec "$@"','followup',*arguments]

    def interrupted(signum, frame):
        raise RuntimeError('FAIL interrupted; bounded process/mount cleanup follows')
    signal.signal(signal.SIGTERM,interrupted);signal.signal(signal.SIGINT,interrupted)
    try:
        shutil.copytree(base/'source',source,symlinks=True)
        for relative,digest in patches.items():
            path=Path(relative)
            if path.is_absolute() or '..' in path.parts or not relative.startswith(('gateways/','tests/ros')):
                raise RuntimeError('unsafe overlay path '+relative)
            candidate=overlay/path;assert sha(candidate)==digest
            destination=source/path;destination.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(candidate,destination)
        frozen=inventory(source);report['candidate_source_inputs']=frozen
        roots={}
        for version,distribution in ((1,'noetic'),(2,'jazzy')):
            root=out/'gateway'/(distribution+'-rootfs');root.mkdir(parents=True);roots[version]=root
            subprocess.run(['mount','--rbind',str(base/'gateway'/(distribution+'-rootfs')),str(root)],check=True)
            mount_roots.append(root)
            subprocess.run(['mount','--make-rprivate',str(root)],check=True)
            subprocess.run(['mount','--bind',str(source),str(root/'work')],check=True)
            scratch=out/'native-temp'/distribution;scratch.mkdir(parents=True)
            subprocess.run(['mount','--bind',str(scratch),str(root/'tmp')],check=True)
            (root/f'tmp/ros{version}-build').mkdir()
            build=root/f'tmp/ros{version}-followup-build';build.mkdir()
            command=chroot(root,distribution,['/bin/bash','-lc',
                f'source /opt/ros/{distribution}/setup.bash && cmake -S /work/gateways/ros{version} -B /tmp/{build.name} -DCMAKE_BUILD_TYPE=RelWithDebInfo && cmake --build /tmp/{build.name} --parallel 1'])
            run(command,f'ros{version}-native-build',900)
            run(chroot(root,distribution,['/usr/bin/g++','--version']),distribution+'-compiler-version',30)
            run(chroot(root,distribution,['/usr/bin/dpkg-query','-W','-f=${Package}\\t${Version}\\n']),distribution+'-packages',30)
            for metadata in [build/'CMakeCache.txt',*build.rglob('flags.make'),*build.rglob('link.txt')]:
                destination=reports/'build-metadata'/f'ros{version}'/metadata.relative_to(build)
                destination.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(metadata,destination)
            for binary in build.glob(f'uwvm-ros{version}-*'):
                if binary.is_file():
                    destination=out/'native-binaries'/f'ros{version}'/binary.name
                    destination.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(binary,destination)
                    binaries[f'ros{version}:'+binary.name]={'sha256':sha(binary),'bytes':binary.stat().st_size,'snapshot':str(destination)}
            # Bind only the cloned rootfs view. Do not replace the original
            # rootfs's canonical symlink or touch its previous build files.
            subprocess.run(['mount','--bind',str(build),str((root/f'tmp/ros{version}-build').resolve())],check=True)
            command=chroot(root,distribution,['/usr/bin/g++','-std=c++17','-O1','-g0','-fsanitize=address,undefined',
                '-fno-omit-frame-pointer','-MMD','-MF',f'/tmp/{build.name}/bounded-text.d','-I/work/src',
                '/work/tests/ros_resources/bounded_text_test.cc','-o',f'/tmp/{build.name}/bounded-text'])
            run(command,f'{distribution}-bounded-text-build',120)
            run(chroot(root,distribution,[f'/tmp/{build.name}/bounded-text']),f'{distribution}-bounded-text-run',45)
        assert len(binaries)==18
        build=roots[2]/'tmp/ros2-followup-build'
        flags=(build/'CMakeFiles/uwvm-ros2-gateway.dir/flags.make').read_text()
        values={line.split(' = ',1)[0]:shlex.split(line.split(' = ',1)[1]) for line in flags.splitlines() if ' = ' in line}
        link=shlex.split((build/'CMakeFiles/uwvm-ros2-gateway.dir/link.txt').read_text())
        for name,relative in [('bounded-native','tests/ros_resources/bounded_native_ros2.cc'),
                              ('timer-publication','tests/ros_action/timer_publication_ros2.cc'),
                              ('action-deadline','tests/ros_action/deadline_native_ros2.cc')]:
            inside=f'/tmp/{build.name}/{name}'
            run(chroot(roots[2],'jazzy',['/usr/bin/g++',*values['CXX_DEFINES'],*values['CXX_INCLUDES'],
                '-std=c++17','-O1','-g0','-MMD','-MF',inside+'.d','-c','/work/'+relative,'-o',inside+'.o']),name+'-build',180)
            command=[token for token in link]
            command=[inside+'.o' if token.endswith('main.cc.o') else token for token in command]
            command[command.index('-o')+1]=inside
            run(chroot(roots[2],'jazzy',command),name+'-link',90)
            args=[inside]
            if name=='bounded-native':args.append('/tmp/ros2-build/uwvm-ros2-gateway')
            native_env=os.environ.copy()
            native_env.update(ROS_DOMAIN_ID='89',RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_SECURITY_ENABLE='false')
            run(chroot(roots[2],'jazzy',args),name+'-run',60,env=native_env)
        assert inventory(source)==frozen,'candidate source changed during native build/tests'
        deps={}
        for root in roots.values():
            for dep in (root/'tmp').glob('ros*-followup-build/**/*.d'):
                # GCC's dependency file starts with a make target. Some CMake
                # targets use an absolute /work/...cc.o path; it is an output,
                # not a source input. Scan each rule's prerequisites only.
                prerequisites='\n'.join(line.partition(':')[2]
                    for line in dep.read_text().replace('\\\n',' ').splitlines() if ':' in line)
                for token in re.findall(r'/work/[^\s\\]+',prerequisites):
                    relative=posixpath.normpath(token.rstrip(':'))[6:]
                    if relative not in frozen:raise RuntimeError('unfrozen compiler input '+relative)
                    assert sha(source/relative)==frozen[relative]
                    deps[relative]=frozen[relative]
        for relative,digest in deps.items():
            destination=reports/'compiled-inputs'/relative;destination.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source/relative,destination)
        report['compiled_project_inputs']=deps
        a.alias.mkdir();subprocess.run(['mount','--rbind',str(out),str(a.alias)],check=True);mount_roots.append(a.alias)
        subprocess.run(['mount','--make-rprivate',str(a.alias)],check=True)
        alias_source=a.alias/'source'
        worker=base/'product-final-v8-io/dual/uwvm';report['worker_sha256']=sha(worker)
        run([sys.executable,str(alias_source/'tests/ros/run_native.py'),'--workspace',str(a.alias),'--suite','all','--out',str(reports/'native-regression')],
            'native-regression',900)
        run([sys.executable,str(alias_source/'tests/ros/run_rpc.py'),'--workspace',str(a.alias),'--source',str(alias_source),'--uwvm',str(worker),
             '--llvm-libs',a.llvm_libs,'--version','2','--mode','resources'], 'wasm-resources',900)
        run([sys.executable,str(alias_source/'tests/ros/run_network.py'),'--workspace',str(a.alias),'--source',str(alias_source),'--uwvm',str(worker),
             '--llvm-libs',a.llvm_libs,'--mode','actions','--only','ros2-wasm-action-client','ros2-wasm-action-server'], 'wasm-actions',180)
        assert sha(worker)==report['worker_sha256']
        # RPC recompiles reviewed WAT fixtures. Native compiled inputs must
        # remain exact; the pre-build complete inventory is retained separately.
        assert all(sha(source/relative)==digest for relative,digest in deps.items())
        assert inventory(base/'source')==baseline,'original frozen source was modified'
        assert all(sha(base/relative)==digest for relative,digest in previous_binaries.items()),'original native binaries were modified'
        report['status']='PASS'
    except Exception as error:
        report['error']=str(error);print('FAIL '+str(error),flush=True)
    finally:
        for child in reversed(children):
            if child.poll() is None:
                os.killpg(child.pid,signal.SIGTERM)
                try:child.wait(timeout=5)
                except subprocess.TimeoutExpired:os.killpg(child.pid,signal.SIGKILL);child.wait()
        cleanup=[]
        for root in reversed(mount_roots):
            result=subprocess.run(['umount','-R',str(root)],stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=15)
            cleanup.append({'mount':str(root),'returncode':result.returncode,'stderr':result.stderr.decode(errors='replace')})
            if result.returncode:report['status']='FAIL'
        report['mount_cleanup']=cleanup
        report['baseline_preservation']={
            'source_unchanged':inventory(base/'source')==baseline,
            'native_binaries_unchanged':all(sha(base/relative)==digest for relative,digest in previous_binaries.items()),
            'native_binary_count':len(previous_binaries)}
        if not all(report['baseline_preservation'][key] for key in ('source_unchanged','native_binaries_unchanged')):
            report['status']='FAIL'
        after=probe();report['cgroup_after']=after
        events=lambda value:dict(line.split() for line in value['cgroup_ancestors'][0]['memory.events'].splitlines())
        if after['status']!='PASS' or any(events(before).get(k)!=events(after).get(k) for k in ('oom','oom_kill','oom_group_kill')):
            report['status']='FAIL';report['cgroup_error']='limit mismatch or OOM counter increased'
        (reports/'report.json').write_text(json.dumps(report,indent=2)+'\n')
        print(report['status']+' '+str(reports/'report.json'),flush=True)
    return 0 if report['status']=='PASS' else 1


if __name__=='__main__':sys.exit(main())
