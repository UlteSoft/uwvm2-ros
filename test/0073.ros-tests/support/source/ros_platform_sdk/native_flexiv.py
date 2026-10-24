#!/usr/bin/env python3
"""Build an unchanged Flexiv interface package in an actual original ROS SDK.

This operator phase requires owner_flexiv containment. It runs no RDK, controller
or robot connection. A private mount namespace makes the existing native SDK
read-only and exposes only this task's writable tmpfs at /tmp. Genuine upstream
CTest outcomes remain distinct from portable codecs and native robot delivery.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import xml.etree.ElementTree as ET

ROOTS = {
    'ros1_noetic':('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source58-noetic-native3-r1/sdk-rootfs','noetic'),
    'ros2_humble_v1':('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source39-native-gateways-physics-peer-r1/rootfs','humble'),
    'ros2_humble_v2':('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source39-native-gateways-physics-peer-r1/rootfs','humble'),
    'ros2_humble_v2_2':('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source39-native-gateways-physics-peer-r1/rootfs','humble'),
}


def pin(path):
    with path.open('rb') as stream:
        digest = hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=digest)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a = p.parse_args()
    root,out = a.root.resolve(strict=True),a.out
    inputs = json.loads((root/'inputs.json').read_bytes())
    profile = inputs['native_profile']
    sdk,distro = ROOTS[profile]
    sdk = Path(inputs.get('native_sdk_roots',{}).get(profile,sdk))
    source = Path(inputs['source_root'])
    original = source/'sdk/origin/robots/arms/flexiv/upstream'/profile/'files/flexiv_msgs'
    assert out.is_absolute() and out.parent==root/'out' and not out.exists()
    assert all((sdk/p).exists() for p in ['usr/bin/cmake','usr/bin/make','usr/bin/g++','usr/bin/python3'])
    assert (sdk/'tmp').is_dir() and (sdk/'dev').is_dir()
    for name in ['usr/bin/cmake','usr/bin/make','usr/bin/g++','usr/bin/python3','tmp','dev']:
        assert (sdk/name).resolve(strict=True).is_relative_to(sdk.resolve(strict=True)),name
    out.mkdir(mode=0o700)
    (out/'commands').mkdir()
    (out/'tmp').mkdir()
    report = dict(status='RUNNING',profile=profile,distribution=distro,native_sdk_root=str(sdk),
        scope='Unchanged original native interface package build and actual package tests only; no RDK/controller or Wasm native delivery',
        default_authority=[],commands=[],original_files=[pin(p) for p in sorted(original.rglob('*'))if p.is_file()],
        sdk_tools=[pin(sdk/p) for p in ['usr/bin/cmake','usr/bin/make','usr/bin/g++','usr/bin/python3']])
    report['sdk_identity_files'] = [pin(sdk/p) for p in ['etc/os-release','var/lib/dpkg/status','opt/ros/'+distro+'/setup.sh'] if (sdk/p).is_file()]
    def save():
        (out/'native.json').write_text(json.dumps(report,indent=2)+'\n')
    mount_script = out/'chroot-exec.py'
    mount_script.write_text('''import os,pathlib,stat,subprocess,sys
sdk,root=sys.argv[1:3]
def run(args):subprocess.run(args,check=True,timeout=10)
assert pathlib.Path(root).is_mount()
run(['/usr/bin/mount','--bind',sdk,sdk])
run(['/usr/bin/mount','-o','remount,bind,ro',sdk])
run(['/usr/bin/mount','--bind',root,sdk+'/tmp'])
run(['/usr/bin/mount','-t','tmpfs','-o','size=256k,nr_inodes=16,mode=755,nosuid,noexec','flexiv-native-null',sdk+'/dev'])
os.mknod(sdk+'/dev/null',stat.S_IFCHR|0o666,os.makedev(1,3))
os.chroot(sdk);os.chdir('/tmp')
os.execvpe(sys.argv[3],sys.argv[3:],os.environ)
''')
    inside_source = '/tmp/'+str(original.relative_to(root))
    inside_out = '/tmp/'+str(out.relative_to(root))
    def run(label,argv,expected=0,deadline=300):
        directory = out/'commands'/label
        directory.mkdir()
        # Setup is original native ROS configuration in the read-only SDK.
        # All requested commands are explicit argv after that setup, not shell
        # substitutions from upstream declarations, file names or Guest input.
        shell = ['bash','--noprofile','--norc','-c',
                 'set -e; source /opt/ros/'+distro+'/setup.sh; exec "$@"','flexiv-native',*map(str,argv)]
        actual = ['/usr/bin/unshare','--mount','--net','--propagation','private',sys.executable,
            '-I','-S','-B',mount_script,sdk,root,*shell]
        env = dict(os.environ,PYTHONDONTWRITEBYTECODE='1',HOME=inside_out+'/tmp',TMPDIR=inside_out+'/tmp',
            ROS_DISTRO=distro,ROS_LOCALHOST_ONLY='1',CMAKE_BUILD_PARALLEL_LEVEL='1',MAKEFLAGS='-j1',
            CATKIN_SHELL='sh',OMP_NUM_THREADS='1',OPENBLAS_NUM_THREADS='1')
        with (directory/'stdout').open('xb') as stdout,(directory/'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str,actual)),env=env,stdout=stdout,stderr=stderr,timeout=deadline,check=False)
        row = dict(label=label,argv=list(map(str,actual)),exit=result.returncode,expected_exit=expected,
                   stdout=pin(directory/'stdout'),stderr=pin(directory/'stderr'))
        report['commands'].append(row)
        save()
        if expected is not None:
            assert result.returncode==expected,(label,result.returncode,(directory/'stderr').read_text()[-2500:])
        return result.returncode
    try:
        configure = ['cmake','-S',inside_source,'-B',inside_out+'/build',
            '-DCMAKE_BUILD_TYPE=Release','-DCMAKE_C_COMPILER=/usr/bin/gcc','-DCMAKE_CXX_COMPILER=/usr/bin/g++',
            '-DCMAKE_INSTALL_PREFIX='+inside_out+'/install','-DCMAKE_EXPORT_COMPILE_COMMANDS=ON','-DBUILD_TESTING=ON']
        if distro=='noetic':
            configure += ['-DCATKIN_ENABLE_TESTING=ON','-DPYTHON_EXECUTABLE=/usr/bin/python3']
        else:
            configure += ['-DPython3_EXECUTABLE=/usr/bin/python3']
        run('configure-original-package',configure)
        run('build-original-package',['cmake','--build',inside_out+'/build','--parallel','1'],deadline=900)
        run('install-original-package',['cmake','--install',inside_out+'/build'])
        # List actual original package cases before executing; never invent a
        # case count, call an authored codec contract an official case, or
        # suppress original failures by editing locked upstream bytes.
        ctest = ['cmake','-E','chdir',inside_out+'/build','ctest']
        run('list-original-tests',[*ctest,'--show-only=json-v1'])
        listed = json.loads((out/'commands/list-original-tests/stdout').read_bytes())
        report['original_test_cases_discovered'] = len(listed['tests'])
        if distro=='noetic' and not listed['tests']:
            # Ubuntu20's original CTest predates --output-junit. No registered
            # case is invented to compensate; the genuine zero-case invocation
            # and native package compilation remain separate observations.
            result = run('run-original-tests',[*ctest,'-j1','--timeout','60','--output-on-failure'],expected=None)
            report.update(original_cases_run=0,original_case_failures=0,original_case_skips=0)
        else:
            result = run('run-original-tests',[*ctest,'-j1','--timeout','60',
                '--output-on-failure','--output-junit',inside_out+'/upstream-junit.xml'],expected=None,deadline=600)
            junit = ET.fromstring((out/'upstream-junit.xml').read_bytes())
            cases = junit.findall('.//testcase')
            report['original_cases_run'] = len(cases)
            report['original_case_failures'] = sum(c.find('failure')is not None or c.find('error')is not None for c in cases)
            report['original_case_skips'] = sum(c.find('skipped')is not None for c in cases)
        report['installed_products'] = [pin(p)for p in sorted((out/'install').rglob('*'))if p.is_file()and not p.is_symlink()]
        assert report['installed_products']
        for row in report['original_files']+report['sdk_tools']+report['sdk_identity_files']:
            assert pin(Path(row['path']))==row
        report['status'] = 'ORIGINAL_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS' if result==0 else 'ORIGINAL_NATIVE_INTERFACE_BUILD_PASS_UPSTREAM_TEST_FAILURES_RETAINED'
        print(report['status'],profile,report['original_cases_run'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error))
        raise
    finally:
        save()


if __name__=='__main__':
    main()
