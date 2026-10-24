#!/usr/bin/env python3
"""Verify native ROS deadlines, Image encoding and real regressions in the supplied cgroup.

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
import traceback


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--workspace', type=Path, required=True)
    p.add_argument('--baseline', type=Path, required=True)
    p.add_argument('--overlay', type=Path, required=True)
    p.add_argument('--previous', type=Path, required=True, help='Frozen completed ROS2 follow-up with pre-fix native headers')
    p.add_argument('--reuse-native', type=Path, help='Completed attempt with all 18 native builds; only unchanged compiled inputs may be reused')
    p.add_argument('--alias', type=Path, default=Path('/u10'))
    p.add_argument('--llvm-libs', required=True)
    a = p.parse_args()
    base, out, overlay, previous = a.baseline.resolve(), a.workspace.absolute(), a.overlay.resolve(), a.previous.resolve()
    if out.exists() or a.alias.exists() or os.geteuid() != 0 or sys.platform != 'linux':
        p.error('root, a fresh workspace and unused short alias are required')
    if any(out==protected or protected in out.parents for protected in (base,previous,overlay)):
        p.error('fresh workspace must be outside all frozen input trees')
    if not re.fullmatch(r'/[a-z][0-9]+', str(a.alias)):
        p.error('alias must be a short absolute path such as /u9')
    source, reports = out/'source', out/'reports'
    reports.mkdir(parents=True)
    sys.path.insert(0, str(base/'source/test/0073.ros-tests/support/source/ros_cgroup'))
    from probe import probe
    before = probe()
    if before['status'] != 'PASS':
        raise RuntimeError('BLOCKED supplied 64 GiB cgroup not proven')
    sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    inventory = lambda root: {str(x.relative_to(root)):sha(x) for x in root.rglob('*') if x.is_file()}
    baseline = inventory(base/'source')
    previous_inputs = inventory(previous/'source')
    assert json.loads((previous/'reports/report.json').read_text())['status']=='PASS'
    previous_snapshots={str(x.relative_to(previous)):sha(x) for x in (previous/'native-binaries').rglob('*') if x.is_file()}
    assert len(previous_snapshots)==18
    previous_binaries={str(x.relative_to(base)):sha(x)
        for distribution,version in (('noetic',1),('jazzy',2))
        for x in (base/'gateway'/(distribution+'-rootfs')/f'tmp/ros{version}-build').glob(f'uwvm-ros{version}-*') if x.is_file()}
    if len(previous_binaries) != 18:
        raise RuntimeError('expected all 18 frozen native baseline binaries')
    patches = json.loads((overlay/'manifest.json').read_text())
    reuse=a.reuse_native.resolve() if a.reuse_native else None
    if reuse:
        if out==reuse or reuse in out.parents:p.error('workspace overlaps native reuse input')
        reuse_document=json.loads((reuse/'reports/report.json').read_text())
        reuse_report_digest=sha(reuse/'reports/report.json')
        reuse_binaries={str(x.relative_to(reuse)):sha(x) for x in (reuse/'native-binaries').rglob('*') if x.is_file()}
        assert len(reuse_binaries)==18
    mount_roots, children, rows, binaries = [], [], [], {}
    report = {'status':'FAIL', 'scope':'Native ROS deadline and Image continuation, not a new full A15 acceptance',
              'cgroup_before':before, 'commands':rows, 'baseline_source_inputs':baseline,
              'overlay_inputs':patches, 'native_binaries':binaries,
              'previous_native_binary_inputs':previous_binaries,
              'executed_driver_sha256':sha(Path(__file__)), 'previous_followup_source_inputs':previous_inputs,
              'previous_followup_binaries':previous_snapshots, 'before_variants':[],
              'sanitizers':'Standalone bounded-text, Image encoder and IPC admission checks use ASan/UBSan; native middleware libraries are not instrumented.'}
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

    def run(command, name, timeout=30, env=None, expected=0, live_artifact=None, failure=None):
        log_path, proof_path = reports/(name+'.log'), reports/(name+'-exec.json')
        row = {'name': name, 'command': list(map(str, command)), 'status': 'FAIL'}
        rows.append(row)
        full = [sys.executable, str(wrapper), str(proof_path), json.dumps(before['membership']), *map(str, command)]
        with log_path.open('wb') as log:
            child = subprocess.Popen(full, stdout=log, stderr=subprocess.STDOUT, start_new_session=True, env=env)
            children.append(child);row['pid'] = child.pid
            until = time.monotonic()+timeout
            try:
                while child.poll() is None:
                    if live_artifact is not None and 'actual_main_live_PID' not in row:
                        # uutils chroot forks the requested program. Keep the
                        # wrapper proof and inspect its real live descendants.
                        pending, seen = [(child.pid, [child.pid])], set()
                        while pending:
                            pid, ancestry = pending.pop()
                            if pid in seen:
                                continue
                            seen.add(pid)
                            try:
                                descendants=Path(f'/proc/{pid}/task/{pid}/children').read_text().split()
                                pending.extend((int(value),ancestry+[int(value)]) for value in descendants)
                                proc_exe=Path(f'/proc/{pid}/exe')
                                actual,wanted=proc_exe.stat(),live_artifact.stat()
                                parent=int(next(line.split()[1] for line in Path(f'/proc/{pid}/status').read_text().splitlines() if line.startswith('PPid:')))
                                observed={'pid':pid,'parent_pid':parent,'ancestry':ancestry,'exe_path':os.readlink(proc_exe),
                                          'exe_device':actual.st_dev,'exe_inode':actual.st_ino}
                                samples=row.setdefault('observed_execs', [])
                                if observed not in samples and len(samples)<32:
                                    samples.append(observed)
                                if (actual.st_dev,actual.st_ino)!=(wanted.st_dev,wanted.st_ino):
                                    continue
                                membership=Path(f'/proc/{pid}/cgroup').read_text().splitlines()
                                cgroup_path=Path(before['cgroup_ancestors'][0]['path'])
                                live=str(pid) in (cgroup_path/'cgroup.procs').read_text().splitlines()
                                if membership!=before['membership'] or not live or sha(proc_exe)!=sha(live_artifact) or (len(ancestry)>1 and parent!=ancestry[-2]):
                                    raise RuntimeError('native main PID/artifact/cgroup/ancestry mismatch')
                                row['actual_main_live_PID']={**observed,'membership':membership,'live_cgroup_procs':live,'exe_sha256':sha(proc_exe)}
                            except (FileNotFoundError,ProcessLookupError):
                                pass
                    if time.monotonic() >= until:
                        raise subprocess.TimeoutExpired(command, timeout)
                    time.sleep(0.01)
                code = child.wait()
            except BaseException:
                if child.poll() is None:
                    os.killpg(child.pid, signal.SIGKILL);child.wait(timeout=5)
                raise
        row.update(returncode=code, log_sha256=sha(log_path), log_path=str(log_path))
        if not proof_path.is_file():
            raise RuntimeError('missing pre-exec cgroup/PID proof for '+name)
        row['pre_exec_live_PID'] = json.loads(proof_path.read_text())
        if row['pre_exec_live_PID']['membership'] != before['membership'] or row['pre_exec_live_PID']['pid'] != child.pid:
            raise RuntimeError('pre-exec proof mismatch for '+name)
        row['status'] = 'PASS' if code == 0 else 'FAIL'
        if code != expected or live_artifact is not None and 'actual_main_live_PID' not in row:
            raise RuntimeError('unexpected exit or missing actual native main proof for '+name)
        if expected:
            row['expected_failure']=True
            markers=failure if isinstance(failure,list) else [failure]
            matched=[marker for marker in markers if marker and marker in log_path.read_text()]
            if code!=1 or not matched:raise RuntimeError('old-code failure marker mismatch '+name)
            row['actual_failure_marker']=matched[0]
        print(('EXPECTED FAIL ' if expected else 'PASS ')+name,flush=True)
        return row

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
            if path.is_absolute() or '..' in path.parts or not relative.startswith(('src/ros/gateway/','test/0073.ros-tests/support/source/ros','src/uwvm2/imported/ros/ipc/')):
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
            build=root/f'tmp/ros{version}-continuation-build'
            command=chroot(root,distribution,['/bin/bash','-lc',
                f'source /opt/ros/{distribution}/setup.bash && cmake -S /work/src/ros/gateway/ros{version} -B /tmp/{build.name} -DCMAKE_BUILD_TYPE=RelWithDebInfo && cmake --build /tmp/{build.name} --parallel 1'])
            if reuse:
                build_row=next(row for row in reuse_document['commands'] if row['name']==f'ros{version}-native-build')
                assert build_row['status']=='PASS' and build_row['returncode']==0
                build_log=reuse/'reports'/f'ros{version}-native-build.log'
                assert sha(build_log)==build_row['log_sha256']
                old_build=reuse/'native-temp'/distribution/build.name
                native_inputs={}
                dependencies=[*(old_build/'CMakeFiles').rglob('*.d'),*(old_build/'CMakeFiles').rglob('depend.make')]
                for dep in dependencies:
                    prerequisites='\n'.join(line.partition(':')[2] for line in dep.read_text().replace('\\\n',' ').splitlines() if ':' in line)
                    for token in re.findall(r'/work/[^\s\\]+',prerequisites):
                        rel=posixpath.normpath(token.rstrip(':'))[6:]
                        assert rel in frozen and frozen[rel]==reuse_document['candidate_source_inputs'][rel] and sha(source/rel)==frozen[rel]
                        native_inputs[rel]=frozen[rel]
                assert f'src/ros/gateway/ros{version}/main.cc' in native_inputs,'native CMake dependency metadata is missing gateway source'
                shutil.copytree(old_build,build,symlinks=True)
                for binary in build.glob(f'uwvm-ros{version}-*'):
                    assert sha(binary)==reuse_binaries[str((reuse/'native-binaries'/f'ros{version}'/binary.name).relative_to(reuse))]
                record={'source_workspace':str(reuse),'source_report_sha256':reuse_report_digest,
                        'source_overall_status':reuse_document['status'],'build_status':'PASS','source_build_command':build_row,
                        'compiled_inputs':native_inputs,'source_binary_snapshots':reuse_binaries}
                report.setdefault('native_reuse',{})[distribution]=record
                proof_dir=reports/'native-reuse'/distribution;proof_dir.mkdir(parents=True)
                shutil.copy2(build_log,proof_dir/'build.log')
                print('REUSED native '+distribution+' after exact compiled-input and binary SHA verification',flush=True)
            else:
                build.mkdir();run(command,f'ros{version}-native-build',900)
            run(chroot(root,distribution,['/usr/bin/g++','--version']),distribution+'-compiler-version',30)
            run(chroot(root,distribution,['/usr/bin/dpkg-query','-W','-f=${Package}\\t${Version}\\n']),distribution+'-packages',30)
            if reuse:
                for name in (distribution+'-compiler-version',distribution+'-packages'):
                    assert sha(reports/(name+'.log'))==sha(reuse/'reports'/(name+'.log'))
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
            # Keep standalone sanitizer probes at a fixed executable address.
            # Both sanitizer instruments remain enabled; this does not alter
            # the production native targets or their reused build flags.
            command=chroot(root,distribution,['/usr/bin/g++','-std=c++17','-O1','-g0','-fno-pie','-no-pie','-fsanitize=address,undefined',
                '-fno-omit-frame-pointer','-MMD','-MF',f'/tmp/{build.name}/bounded-text.d','-I/work/src',
                '/work/test/0073.ros-tests/support/source/ros_resources/bounded_text_test.cc','-o',f'/tmp/{build.name}/bounded-text'])
            run(command,f'{distribution}-bounded-text-build',120)
            run(chroot(root,distribution,[f'/tmp/{build.name}/bounded-text']),f'{distribution}-bounded-text-run',45)
            for name,relative in [('image-encode','test/0073.ros-tests/support/source/ros_ipc/image_encode_test.cc'),
                                  ('admission-deadline','test/0073.ros-tests/support/source/ros_ipc/admission_deadline_test.cc')]:
                inside=f'/tmp/{build.name}/{name}'
                run(chroot(root,distribution,['/usr/bin/g++','-std=c++17','-O1','-g0','-pthread','-fno-pie','-no-pie',
                    '-fsanitize=address,undefined','-fno-omit-frame-pointer','-MMD','-MF',inside+'.d',
                    '-I/work/src','/work/'+relative,'-o',inside]),distribution+'-'+name+'-build',120)
                run(chroot(root,distribution,[inside]),distribution+'-'+name+'-run',45,
                    live_artifact=build/name if name=='admission-deadline' else None)
        assert len(binaries)==18
        build=roots[2]/'tmp/ros2-continuation-build'
        flags=(build/'CMakeFiles/uwvm-ros2-gateway.dir/flags.make').read_text()
        values={line.split(' = ',1)[0]:shlex.split(line.split(' = ',1)[1]) for line in flags.splitlines() if ' = ' in line}
        link=shlex.split((build/'CMakeFiles/uwvm-ros2-gateway.dir/link.txt').read_text())
        for name,relative in [('bounded-native','test/0073.ros-tests/support/source/ros_resources/bounded_native_ros2.cc'),
                              ('timer-publication','test/0073.ros-tests/support/source/ros_action/timer_publication_ros2.cc'),
                              ('action-deadline','test/0073.ros-tests/support/source/ros_action/deadline_native_ros2.cc'),
                              ('query-deadline','test/0073.ros-tests/support/source/ros_action/query_deadline_ros2.cc'),
                              ('parameter-deadline','test/0073.ros-tests/support/source/ros_resources/parameter_deadline_ros2.cc'),
                              ('tf-deadline','test/0073.ros-tests/support/source/ros_tf/deadline_native_ros2.cc'),
                              ('image-native','test/0073.ros-tests/support/source/ros_ipc/image_native_ros2.cc'),
                              ('response-deadline','test/0073.ros-tests/support/source/ros_ipc/response_deadline_ros2.cc')]:
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
            native_env.update(ROS_DOMAIN_ID='91',RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_SECURITY_ENABLE='false',ROS_LOG_DIR='/tmp/continuation-roslogs')
            run(chroot(roots[2],'jazzy',args),name+'-run',60,env=native_env,live_artifact=build/name)
        # Each old-code variant differs in exactly one production file from
        # this candidate; the new probe and every other compiled input match.
        native_env=os.environ.copy()
        native_env.update(ROS_DOMAIN_ID='91',RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_SECURITY_ENABLE='false',ROS_LOG_DIR='/tmp/continuation-roslogs')
        variants=[
            ('admission','src/ros/gateway/common/server.h','test/0073.ros-tests/support/source/ros_ipc/admission_deadline_test.cc',
             [('expired','expired IPC request reached adapter side effect'),('remaining','adapter budget reset time spent on admission'),
              ('cap','adapter budget reset time spent on admission'),
              ('cap-expired','expired capability result failed to return timeout on live IPC session')]),
            ('query','src/ros/gateway/ros2/action.h','test/0073.ros-tests/support/source/ros_action/query_deadline_ros2.cc',
             [('feedback','expired feedback query delivered queued data'),('result','expired result query delivered queued data')]),
            ('parameter','src/ros/gateway/ros2/resources.h','test/0073.ros-tests/support/source/ros_resources/parameter_deadline_ros2.cc',
             [('set','expired local parameter set accepted success'),('declare','expired local parameter declare accepted success'),
              ('undeclare','expired undeclare preflight removed the actual native parameter'),
              ('get','expired local parameter encoding accepted success'),('describe','expired local parameter encoding accepted success'),
              ('list','expired local parameter encoding accepted success'),('remote','expired remote parameter inspector accepted success'),
              ('graph','expired actual Graph inspect-node encoding accepted success')]),
            ('tf','src/ros/gateway/ros2/tf.h','test/0073.ros-tests/support/source/ros_tf/deadline_native_ros2.cc',
             [('', ['zero-budget lookup must create no native subscription','cached zero-budget lookup must not deliver a successful result'])]),
            ('image','src/ros/gateway/ros2/main.cc','test/0073.ros-tests/support/source/ros_ipc/image_native_ros2.cc',
             [('', 'malformed native Image endian was normalized or valid barrier changed')]),
        ]
        # The response probe also supports selective old Service/Lifecycle
        # checks, with the same generated native endpoints and wire barrier.
        variants.extend([
            ('service','src/ros/gateway/ros2/main.cc','test/0073.ros-tests/support/source/ros_ipc/response_deadline_ros2.cc',
             [('service','expired native Service response accepted success')]),
            ('lifecycle','src/ros/gateway/ros2/lifecycle.h','test/0073.ros-tests/support/source/ros_ipc/response_deadline_ros2.cc',
             [('get','expired native Lifecycle state response accepted success'),
              ('change','expired native Lifecycle change response accepted success')]),
        ])
        for label,changed,relative,cases in variants:
            variant=roots[2]/'tmp/variants'/label
            shutil.copytree(source,variant,symlinks=True)
            old=previous/'source'/changed;assert sha(old)!=frozen[changed]
            shutil.copy2(old,variant/changed)
            assert inventory(variant)=={**frozen,changed:sha(old)}
            inside=f'/tmp/ros2-continuation-build/before-{label}'
            variant_inside=f'/tmp/variants/{label}'
            includes=[token.replace('/work/',variant_inside+'/') for token in values['CXX_INCLUDES']]
            if label=='admission':
                compile_command=['/usr/bin/g++','-std=c++17','-O1','-g0','-pthread','-MMD','-MF',inside+'.d',
                                 '-I'+variant_inside+'/src',variant_inside+'/'+relative,'-o',inside]
                run(chroot(roots[2],'jazzy',compile_command),'before-'+label+'-build',180)
            else:
                run(chroot(roots[2],'jazzy',['/usr/bin/g++',*values['CXX_DEFINES'],*includes,
                    '-std=c++17','-O1','-g0','-MMD','-MF',inside+'.d','-c',variant_inside+'/'+relative,'-o',inside+'.o']),
                    'before-'+label+'-build',180)
                command=[inside+'.o' if token.endswith('main.cc.o') else token for token in link]
                command[command.index('-o')+1]=inside
                run(chroot(roots[2],'jazzy',command),'before-'+label+'-link',90)
            prerequisites=(build/('before-'+label+'.d')).read_text().replace('\\\n',' ').partition(':')[2]
            before_deps={}
            for token in re.findall(re.escape(variant_inside)+r'/[^\s\\]+',prerequisites):
                rel=posixpath.normpath(token[len(variant_inside)+1:])
                assert rel in frozen and sha(variant/rel)==(sha(old) if rel==changed else frozen[rel])
                before_deps[rel]=sha(variant/rel)
                dest=reports/'before-inputs'/label/rel;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(variant/rel,dest)
            assert changed in before_deps and relative in before_deps
            record={'label':label,'changed_file':changed,'old_source':str(old),'old_sha256':sha(old),
                    'candidate_sha256':frozen[changed],'probe_sha256':frozen[relative],
                    'compiled_inputs':before_deps,'binary_sha256':sha(build/('before-'+label)),'runs':[]}
            report['before_variants'].append(record)
            for mode,failure in cases:
                name='before-'+label+'-'+(mode or 'default')
                row=run(chroot(roots[2],'jazzy',[inside,*([mode] if mode else [])]),name,60,
                        env=native_env,expected=1,live_artifact=build/('before-'+label),failure=failure)
                record['runs'].append({'name':name,'status':row['status'],'returncode':row['returncode'],
                                       'expected_failure':True,'failure_marker':row['actual_failure_marker'],'allowed_failure_markers':failure,
                                       'log_sha256':row['log_sha256']})
        assert inventory(source)==frozen,'candidate source changed during native build/tests'
        deps={}
        for root in roots.values():
            dependencies=[*(root/'tmp').glob('ros*-continuation-build/**/*.d'),
                          *(root/'tmp').glob('ros*-continuation-build/CMakeFiles/**/depend.make')]
            for dep in dependencies:
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
        worker=base/'product-final-v8-io/dual/uwvm2-ros';report['worker_sha256']=sha(worker)
        run([sys.executable,str(alias_source/'test/0073.ros-tests/support/source/ros/run_native.py'),'--workspace',str(a.alias),'--suite','all','--out',str(reports/'native-regression')],
            'native-regression',900)
        run([sys.executable,str(alias_source/'test/0073.ros-tests/support/source/ros/run_rpc.py'),'--workspace',str(a.alias),'--source',str(alias_source),'--uwvm',str(worker),
             '--llvm-libs',a.llvm_libs,'--version','2','--mode','resources'], 'wasm-resources',900)
        run([sys.executable,str(alias_source/'test/0073.ros-tests/support/source/ros/run_network.py'),'--workspace',str(a.alias),'--source',str(alias_source),'--uwvm',str(worker),
             '--llvm-libs',a.llvm_libs,'--mode','actions','--only','ros2-wasm-action-client','ros2-wasm-action-server'], 'wasm-actions',180)
        topics=[f'ros{v}-wasm-{operation}'+suffix for v in (1,2) for operation in ('publish','subscribe')
                for suffix in ('','-twist','-image')]
        run([sys.executable,str(alias_source/'test/0073.ros-tests/support/source/ros/run_network.py'),'--workspace',str(a.alias),'--source',str(alias_source),'--uwvm',str(worker),
             '--llvm-libs',a.llvm_libs,'--mode','all','--only',*topics], 'wasm-topics',600)
        assert sha(worker)==report['worker_sha256']
        # RPC recompiles reviewed WAT fixtures. Native compiled inputs must
        # remain exact; the pre-build complete inventory is retained separately.
        assert all(sha(source/relative)==digest for relative,digest in deps.items())
        assert inventory(base/'source')==baseline,'original frozen source was modified'
        assert all(sha(base/relative)==digest for relative,digest in previous_binaries.items()),'original native binaries were modified'
        report['status']='PASS'
    except Exception as error:
        report['error']=type(error).__name__+': '+str(error);report['traceback']=traceback.format_exc();print('FAIL '+report['error'],flush=True)
    finally:
        for child in reversed(children):
            if child.poll() is None:
                os.killpg(child.pid,signal.SIGTERM)
                try:child.wait(timeout=5)
                except subprocess.TimeoutExpired:os.killpg(child.pid,signal.SIGKILL);child.wait()
        cleanup=[]
        for root in reversed(mount_roots):
            attempts=[]
            for attempt in range(3):
                result=subprocess.run(['umount','-R',str(root)],stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=15)
                attempts.append({'returncode':result.returncode,'stderr':result.stderr.decode(errors='replace')})
                if not result.returncode or b'target is busy' not in result.stderr:break
                if attempt<2:time.sleep(0.25)
            cleanup.append({'mount':str(root),**attempts[-1],'attempts':attempts})
            if result.returncode:report['status']='FAIL'
        report['mount_cleanup']=cleanup
        report['baseline_preservation']={
            'previous_followup_source_unchanged':inventory(previous/'source')==previous_inputs,
            'previous_followup_binaries_unchanged':all(sha(previous/rel)==digest for rel,digest in previous_snapshots.items()),
            'source_unchanged':inventory(base/'source')==baseline,
            'native_binaries_unchanged':all(sha(base/relative)==digest for relative,digest in previous_binaries.items()),
            'native_binary_count':len(previous_binaries)}
        if reuse:
            report['native_reuse_preservation']={'source_unchanged':inventory(reuse/'source')==reuse_document['candidate_source_inputs'],
                'binaries_unchanged':all(sha(reuse/rel)==digest for rel,digest in reuse_binaries.items()),
                'report_unchanged':sha(reuse/'reports/report.json')==reuse_report_digest,'report_sha256':sha(reuse/'reports/report.json')}
            if not all(report['native_reuse_preservation'][key] for key in ('source_unchanged','binaries_unchanged','report_unchanged')):report['status']='FAIL'
        if not all(report['baseline_preservation'][key] for key in ('source_unchanged','native_binaries_unchanged','previous_followup_source_unchanged','previous_followup_binaries_unchanged')):
            report['status']='FAIL'
        after=probe();report['cgroup_after']=after
        events=lambda value:dict(line.split() for line in value['cgroup_ancestors'][0]['memory.events'].splitlines())
        if after['status']!='PASS' or any(events(before).get(k)!=events(after).get(k) for k in ('oom','oom_kill','oom_group_kill')):
            report['status']='FAIL';report['cgroup_error']='limit mismatch or OOM counter increased'
        (reports/'report.json').write_text(json.dumps(report,indent=2)+'\n')
        print(report['status']+' '+str(reports/'report.json'),flush=True)
    return 0 if report['status']=='PASS' else 1


if __name__=='__main__':sys.exit(main())
