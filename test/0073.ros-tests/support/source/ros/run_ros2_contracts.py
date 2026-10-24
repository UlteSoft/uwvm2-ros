#!/usr/bin/env python3
"""Build four Worker profiles and 18 native targets, then run complete A15.

Runs as root in the existing Linux container's /host chroot. Frozen baseline
sources and binary snapshots are read only. Every attempt requires a new output
directory and an unused short mount alias; failures remain in their own reports.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import shutil
import signal
import subprocess
import sys
import time
import traceback


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1048576), b''):
            h.update(block)
    return h.hexdigest()


def inventory(root):
    return {str(p.relative_to(root)): digest(p) for p in root.rglob('*') if p.is_file()}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--baseline', type=Path, required=True)
    p.add_argument('--rootfs-workspace', type=Path, required=True)
    p.add_argument('--overlay', type=Path, required=True)
    p.add_argument('--workspace', type=Path, required=True)
    p.add_argument('--alias', type=Path, required=True)
    p.add_argument('--compiler', required=True)
    p.add_argument('--llvm-libs', required=True)
    p.add_argument('--reuse-worker', type=Path, help='Reuse four successful fresh builds only after exact compiled-input and ELF checks')
    p.add_argument('--reuse-native', type=Path, help='Reuse 18 successful fresh native builds after exact compiled-input, dependency and ELF checks')
    a = p.parse_args()
    base, original, overlay = a.baseline.resolve(), a.rootfs_workspace.resolve(), a.overlay.resolve()
    out, alias = a.workspace.absolute(), a.alias.absolute()
    if os.geteuid() != 0 or sys.platform != 'linux' or out.exists() or alias.exists():
        p.error('root Linux, a fresh workspace and unused alias required')
    if not re.fullmatch(r'/[a-z][0-9]+', str(alias)):
        p.error('short absolute alias required')
    if any(out == x or x in out.parents for x in (base, original, overlay)):
        p.error('output overlaps frozen inputs')
    sys.path.insert(0, str(base/'source/test/0073.ros-tests/support/source/ros_cgroup'))
    from probe import probe
    before = probe()
    if before['status'] != 'PASS':
        raise RuntimeError('provided 64 GiB cgroup not proven')
    source, reports = out/'source', out/'reports'
    reports.mkdir(parents=True)
    protected_source = inventory(base/'source')
    protected_binary = inventory(base/'native-binaries')
    protected_report = digest(base/'reports/report.json')
    assert len(protected_binary) == 18
    patches = json.loads((overlay/'manifest.json').read_text())
    children, mounts, commands = [], [], []
    report = {'status':'FAIL', 'scope':'Four fresh Worker profiles, 18 fresh native targets and complete A15; deletion-only ROS2 parameter events, service response admission deadlines, bounded audit I/O and borrowed String/SDK event encoding',
              'executed_driver_sha256':digest(Path(__file__)), 'cgroup_before':before,
              'overlay_inputs':patches, 'baseline_source_inputs':protected_source,
              'baseline_binary_inputs':protected_binary, 'baseline_report_sha256':protected_report,
              'commands':commands, 'actual_main_live_PIDs':[],
              'main_sampling_order':'Pinned ELF FD and live PID/cgroup/PPid/device/inode captured before FD hashing'}
    env = os.environ.copy()
    env.update(CXX=a.compiler, LD_LIBRARY_PATH=a.llvm_libs, PYTHONDONTWRITEBYTECODE='1',
               ROS_PRODUCT_CXXFLAGS='-MMD', ROS_TEST_CXXFLAGS='-std=c++26 -O1 -g0 -MMD')
    wrapper = reports/'exec_in_cgroup.py'
    wrapper.write_text('''import json,os,sys
from pathlib import Path
proof=Path(sys.argv[1]);expected=json.loads(sys.argv[2]);args=sys.argv[3:]
membership=Path('/proc/self/cgroup').read_text().splitlines();assert membership==expected
cg=Path('/sys/fs/cgroup')/membership[0].split(':',2)[2].lstrip('/')
assert str(os.getpid()) in (cg/'cgroup.procs').read_text().split()
proof.write_text(json.dumps({'pid':os.getpid(),'membership':membership,'live_cgroup_procs':True,'command':args},indent=2)+'\\n')
os.execvpe(args[0],args,os.environ)
''')
    seen = set()
    cg = Path(before['cgroup_ancestors'][0]['path'])

    def observe(parent):
        pending = [(parent, [parent])]
        while pending:
            pid, ancestry = pending.pop()
            try:
                pending.extend((int(v), ancestry+[int(v)]) for v in Path(f'/proc/{pid}/task/{pid}/children').read_text().split())
                exe = Path(f'/proc/{pid}/exe')
                path = os.readlink(exe)
                if pid in seen or not (path.startswith(str(out)+'/') or path.startswith(str(alias)+'/')):
                    continue
                # A compiler PID can exec another program while being sampled.
                # Keep one open file description and discard a changing sample.
                with exe.open('rb') as actual_stream:
                    stat = os.fstat(actual_stream.fileno())
                    if actual_stream.read(4) != b'\x7fELF':continue
                    # Pin the ELF and capture live identity before hashing it.
                    # Short-lived guests can finish while the pinned FD is read.
                    membership = Path(f'/proc/{pid}/cgroup').read_text().splitlines()
                    parent_pid = int(next(line.split()[1] for line in Path(f'/proc/{pid}/status').read_text().splitlines() if line.startswith('PPid:')))
                    live = str(pid) in (cg/'cgroup.procs').read_text().split()
                    current = exe.stat()
                    if (current.st_dev,current.st_ino)!=(stat.st_dev,stat.st_ino) or os.readlink(exe)!=path:continue
                    assert membership == before['membership'] and live
                    if len(ancestry)>1 and parent_pid!=ancestry[-2]:continue
                    wanted = Path(path)
                    with wanted.open('rb') as wanted_stream:
                        same = os.fstat(wanted_stream.fileno())
                        if (stat.st_dev,stat.st_ino)!=(same.st_dev,same.st_ino):continue
                        actual_stream.seek(0); value = hashlib.file_digest(actual_stream,'sha256').hexdigest()
                        expected_value = hashlib.file_digest(wanted_stream,'sha256').hexdigest()
                assert value == expected_value, 'stable executable content mismatch: '+path
                report['actual_main_live_PIDs'].append({'pid':pid,'parent_pid':parent_pid,'ancestry':ancestry,
                    'exe_path':path,'exe_device':stat.st_dev,'exe_inode':stat.st_ino,'exe_sha256':value,
                    'membership':membership,'live_cgroup_procs':live})
                seen.add(pid)
            except (FileNotFoundError, ProcessLookupError):
                pass
            except PermissionError as error:
                # Unavailable /proc samples are never accepted as ELF proofs.
                report.setdefault('unavailable_process_samples',[]).append({'pid':pid,'errno':error.errno,
                    'operation':'live descendant sampling','reason':str(error)})

    def run(name, args, timeout):
        log, proof_file = reports/(name+'.log'), reports/(name+'-exec.json')
        row = {'name':name,'command':list(map(str,args)),'status':'FAIL'}
        commands.append(row)
        full = [sys.executable,str(wrapper),str(proof_file),json.dumps(before['membership']),*map(str,args)]
        start = time.monotonic()
        with log.open('wb') as output:
            child = subprocess.Popen(full,stdout=output,stderr=subprocess.STDOUT,env=env,start_new_session=True)
            children.append(child); row['wrapper_pid'] = child.pid
            try:
                while child.poll() is None:
                    observe(child.pid)
                    if time.monotonic()-start > timeout:
                        raise subprocess.TimeoutExpired(args,timeout)
                    time.sleep(.02)
                code = child.wait()
            except BaseException:
                if child.poll() is None:
                    os.killpg(child.pid,signal.SIGTERM)
                    try: child.wait(timeout=20)
                    except subprocess.TimeoutExpired: os.killpg(child.pid,signal.SIGKILL);child.wait(timeout=5)
                raise
        row.update(returncode=code,status='PASS' if code==0 else 'FAIL',seconds=round(time.monotonic()-start,3),
                   log_sha256=digest(log),pre_exec_live_PID=json.loads(proof_file.read_text()))
        assert row['pre_exec_live_PID']['pid'] == child.pid
        assert row['pre_exec_live_PID']['membership'] == before['membership']
        (reports/'progress.json').write_text(json.dumps(report,indent=2)+'\n')
        print(row['status'],name,flush=True)
        if code: raise RuntimeError(name+' actual exit '+str(code))

    def mount(args):
        subprocess.run(['mount',*map(str,args)],check=True,timeout=30)

    def interrupted(signum, frame):
        raise RuntimeError('interrupted; bounded owned process and mount cleanup follows')
    signal.signal(signal.SIGINT,interrupted);signal.signal(signal.SIGTERM,interrupted)
    try:
        shutil.copytree(base/'source',source,symlinks=True)
        for relative, value in patches.items():
            rel = Path(relative)
            if rel.is_absolute() or '..' in rel.parts or not relative.startswith(('src/uwvm2/','src/ros/gateway/','sdk/','test/0073.ros-tests/support/source/ros','examples/ros2/')):
                raise RuntimeError('unsafe overlay '+relative)
            assert digest(overlay/rel) == value
            dest = source/rel;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(overlay/rel,dest)
        frozen = inventory(source)
        report['candidate_source_inputs'] = frozen
        (reports/'source-manifest.json').write_text(json.dumps(frozen,sort_keys=True,indent=2)+'\n')
        for version, distro in ((1,'noetic'),(2,'jazzy')):
            root = out/'gateway'/(distro+'-rootfs');root.mkdir(parents=True)
            mount(['--rbind',original/'gateway'/(distro+'-rootfs'),root]);mounts.append(root)
            mount(['--make-rprivate',root])
            mount(['--bind',source,root/'work']);mount(['-o','remount,bind,ro',root/'work'])
            temp = out/'native-temp'/distro;temp.mkdir(parents=True)
            mount(['--bind',temp,root/'tmp'])
            (temp/f'ros{version}-build').symlink_to(f'ros{version}-seed-build')
        reuse = a.reuse_worker.resolve() if a.reuse_worker else None
        if reuse:
            old_report = json.loads((reuse/'reports/report.json').read_text())
            built = json.loads((reuse/'product/results.json').read_text())
            assert built == old_report['worker_build'] and built['status']=='PASS' and len(built['profiles'])==4
            worker_inputs = json.loads((reuse/'product/source-manifest.json').read_text())
            assert all(digest(reuse/'product/source'/rel)==value==frozen[rel] for rel,value in worker_inputs.items())
            assert inventory(reuse/'product/source')==worker_inputs
            for entry in built['profiles']:
                assert digest(reuse/'product'/entry['profile']/'uwvm2-ros')==entry['binary_sha256']
                assert entry['compile_returncodes']==[0,0,0] and entry['link_returncode']==entry['version_returncode']==0
            shutil.copytree(reuse/'product',out/'product')
            report['worker_reuse']={'workspace':str(reuse),'original_aggregate_status':old_report['status'],
                'original_report_sha256':digest(reuse/'reports/report.json'),'worker_build_status':'PASS',
                'source_manifest_sha256':digest(reuse/'product/source-manifest.json'),
                'source_root':str(reuse/'product/source'),'scope':'Four fresh builds from this continuation; unchanged source inputs and ELF bytes verified'}
            print('PASS exact-input reuse of four fresh Worker builds',flush=True)
        else:
            run('four-worker-build',[sys.executable,source/'test/0073.ros-tests/support/source/ros/build_product.py','--profile','all','--out',out/'product'],1500)
        product = json.loads((out/'product/results.json').read_text())
        assert product['status']=='PASS' and len(product['profiles'])==4
        report['worker_build'] = product
        native_dir = out/'gateway/final-next-r1-provenance'
        native_reuse = a.reuse_native.resolve() if a.reuse_native else None
        if native_reuse:
            old_dir=native_reuse/'gateway/final-next-r1-provenance'
            original_native=json.loads((old_dir/'gateway-frozen-manifest.json').read_text())
            old_native_build=json.loads((old_dir/'build-report.json').read_text())
            old_aggregate=json.loads((native_reuse/'reports/report.json').read_text())
            assert original_native['status']=='FROZEN' and old_native_build['status']=='PASS'
            assert len(original_native['binaries'])==18 and all(row['status']=='PASS' and row['returncode']==0 for row in old_native_build['builds'])
            inputs=original_native['source_inventory']
            assert all(digest(source/rel)==entry['sha256']==digest(native_reuse/'source'/rel) for rel,entry in inputs.items())
            for version,distro in ((1,'noetic'),(2,'jazzy')):
                old_build=native_reuse/'native-temp'/distro/f'ros{version}-next-r1-build'
                for dep in [*old_build.rglob('*.o.d'),*old_build.rglob('depend.make')]:
                    names={posixpath.normpath(x.rstrip(':'))[6:] for x in re.findall(r'/work/[^\s\\]+',dep.read_text()) if not x.rstrip(':').endswith('.o')}
                    assert all(rel in inputs and digest(source/rel)==inputs[rel]['sha256'] for rel in names)
                root=out/'gateway'/(distro+'-rootfs')
                package_data=subprocess.check_output(['chroot',str(root),'/usr/bin/dpkg-query','-W','-f=${Package}\t${Version}\n'],timeout=30)
                assert hashlib.sha256(package_data).hexdigest()==original_native['toolchains'][str(version)]['packages_sha256']
                shutil.copytree(old_build,out/'native-temp'/distro/old_build.name,symlinks=True)
                canonical=out/'native-temp'/distro/f'ros{version}-build';canonical.unlink();canonical.symlink_to(old_build.name)
            shutil.copytree(old_dir,native_dir)
            shutil.copy2(old_dir/'gateway-frozen-manifest.json',native_dir/'gateway-reused-original-manifest.json')
            native_copy=json.loads(json.dumps(original_native));native_copy['source_root']=str(source)
            for key,entry in native_copy['binaries'].items():
                version,name=key.split(':');number=int(version[3:]);distro='noetic' if number==1 else 'jazzy'
                build=out/'gateway'/(distro+'-rootfs')/f'tmp/ros{number}-next-r1-build'
                entry['path']=str(build/name);entry['canonical_path']=str(build.parent/f'ros{number}-build'/name)
                assert digest(Path(entry['path']))==entry['sha256']==digest(native_reuse/'native-binaries'/version/name)
            native_copy['reused_from']={'workspace':str(native_reuse),'original_manifest_sha256':digest(old_dir/'gateway-frozen-manifest.json')}
            (native_dir/'gateway-frozen-manifest.json').write_text(json.dumps(native_copy,indent=2)+'\n')
            report['native_reuse']={'workspace':str(native_reuse),'original_aggregate_status':old_aggregate['status'],
                'original_report_sha256':digest(native_reuse/'reports/report.json'),'native_build_status':'PASS',
                'original_build_report_sha256':digest(old_dir/'build-report.json'),
                'original_manifest_sha256':digest(old_dir/'gateway-frozen-manifest.json'),
                'scope':'18 fresh native builds from this continuation; unchanged inputs, original dependencies, installed package versions and ELF bytes verified'}
            print('PASS exact-input reuse of 18 fresh native builds',flush=True)
        else:
            run('all-native-build',[sys.executable,source/'test/0073.ros-tests/support/source/ros/build_gateways.py','--workspace',out,'--tag','next-r1'],1500)
        native = json.loads((native_dir/'gateway-frozen-manifest.json').read_text())
        assert native['status']=='FROZEN' and len(native['binaries'])==18
        report['native_binaries'] = native['binaries']
        for key, entry in native['binaries'].items():
            version, name = key.split(':')
            dest = out/'native-binaries'/version/name;dest.parent.mkdir(parents=True,exist_ok=True)
            shutil.copy2(entry['path'],dest);assert digest(dest)==entry['sha256']
        dependencies = {}
        for dep in (out/'product').glob('*/*.d'):
            text = dep.read_text().replace('\\\n',' ')
            values = {}
            for token in text.partition(':')[2].split():
                candidate = Path(token)
                roots = [out/'product/source'] + ([reuse/'product/source'] if reuse else [])
                relative = next((str(candidate.relative_to(root)) for root in roots if candidate.is_relative_to(root)),None)
                if relative is None:continue
                rel = posixpath.normpath(relative)
                assert '..' not in Path(rel).parts and digest(candidate)==frozen[rel]
                assert digest(out/'product/source'/rel)==frozen[rel]
                values[rel]=frozen[rel]
            assert values
            dependencies[str(dep.relative_to(out))]={'sha256':digest(dep),'inputs':values}
        assert len(dependencies)==12
        report['worker_compiled_dependencies']=dependencies
        alias.mkdir();mount(['--rbind',out,alias]);mounts.append(alias);mount(['--make-rprivate',alias])
        run('complete-a15',[sys.executable,alias/'source/test/0073.ros-tests/support/source/ros/run_all.py','--uwvm',alias/'product/dual/uwvm2-ros',
            '--out',alias/'acceptance-next','--workspace',alias,'--llvm-libs',a.llvm_libs],6000)
        acceptance = json.loads((out/'acceptance-next/report.json').read_text())
        assert acceptance['status']=='PASS' and len(acceptance['suites'])==15
        report['acceptance']=acceptance
        # Generated Wasm fixtures may change during A15. All compiler inputs are
        # still checked against the frozen bytes actually used for these builds.
        consumed = {name:value for dep in dependencies.values() for name,value in dep['inputs'].items()}
        consumed.update({name:entry['sha256'] for name,entry in native['source_inventory'].items()})
        assert all(digest(source/name)==value for name,value in consumed.items())
        report['compiled_inputs_unchanged'] = True
        assert any(x['exe_sha256']==acceptance['product_sha256'] for x in report['actual_main_live_PIDs'])
        report['status']='PASS'
    except BaseException as error:
        report.update(status='FAIL',error=str(error),traceback=traceback.format_exc())
    finally:
        cleanup = []
        for child in children:
            if child.poll() is None:
                os.killpg(child.pid,signal.SIGTERM)
                try: child.wait(timeout=20)
                except subprocess.TimeoutExpired: os.killpg(child.pid,signal.SIGKILL);child.wait(timeout=5)
        # Native builders and middleware may create their own process groups.
        # Stop only live executables inside this attempt's private mount trees.
        stopped = []
        for sig in (signal.SIGTERM,signal.SIGKILL):
            for proc in Path('/proc').iterdir():
                if not proc.name.isdigit():continue
                try:
                    path=os.readlink(proc/'exe')
                    if path.startswith((str(out)+'/',str(alias)+'/')):
                        pid=int(proc.name);os.kill(pid,sig);stopped.append({'pid':pid,'exe':path,'signal':int(sig)})
                except (FileNotFoundError,ProcessLookupError):pass
                except PermissionError as error:
                    report.setdefault('unavailable_cleanup_samples',[]).append({'pid':int(proc.name),'errno':error.errno})
            time.sleep(.2)
        report['owned_process_cleanup']=stopped
        for root in reversed(mounts):
            attempts = []
            for attempt in range(3):
                result = subprocess.run(['umount','-R',str(root)],capture_output=True,text=True,timeout=30)
                attempts.append({'returncode':result.returncode,'stderr':result.stderr})
                if result.returncode==0:break
                if 'target is busy' not in result.stderr:break
                time.sleep(.25)
            cleanup.append({'root':str(root),'attempts':attempts})
        report['mount_cleanup']=cleanup
        report['remaining_mounts']=[line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4]==str(alias) or line.split()[4].startswith(str(alias)+'/')
            or line.split()[4].startswith(str(out)+'/')]
        remaining = []
        for proc in Path('/proc').iterdir():
            if not proc.name.isdigit():continue
            try:
                path=os.readlink(proc/'exe')
                if path.startswith((str(out)+'/',str(alias)+'/')):remaining.append({'pid':int(proc.name),'exe':path})
            except OSError as error:
                if isinstance(error,PermissionError):report.setdefault('unavailable_postflight_samples',[]).append({'pid':int(proc.name),'errno':error.errno})
        report['remaining_owned_processes']=remaining
        report['baseline_preservation']={'source':inventory(base/'source')==protected_source,
            '18_native_binaries':inventory(base/'native-binaries')==protected_binary,
            'report':digest(base/'reports/report.json')==protected_report}
        after=probe();report['cgroup_after']=after
        if before['membership']!=after.get('membership') or len(before['cgroup_ancestors'])!=len(after.get('cgroup_ancestors',[])):
            report['status']='FAIL'
        deltas=[]
        for first,last in zip(before['cgroup_ancestors'],after.get('cgroup_ancestors',[])):
            if first['path']!=last['path'] or first['memory.max']!=last['memory.max']:
                report['status']='FAIL'
            if first['memory.events'] is None:
                assert last['memory.events'] is None;continue
            old=dict(line.split() for line in first['memory.events'].splitlines())
            new=dict(line.split() for line in last['memory.events'].splitlines())
            deltas.append({'path':first['path'],**{k:int(new[k])-int(old[k]) for k in ('oom','oom_kill','oom_group_kill')}})
        report['oom_deltas']=deltas
        if after['status']!='PASS' or report['remaining_mounts'] or remaining or not all(report['baseline_preservation'].values()) \
            or any(row['attempts'][-1]['returncode'] for row in cleanup) or any(x[k] for x in deltas for k in ('oom','oom_kill','oom_group_kill')):
            report['status']='FAIL'
        if alias.exists() and not report['remaining_mounts']:alias.rmdir()
        (reports/'report.json').write_text(json.dumps(report,indent=2)+'\n')
        print(report['status'],'integration',reports/'report.json',flush=True)
    return 0 if report['status']=='PASS' else 1


if __name__ == '__main__':raise SystemExit(main())
