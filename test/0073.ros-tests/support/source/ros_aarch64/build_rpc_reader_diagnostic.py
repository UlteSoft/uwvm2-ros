#!/usr/bin/env python3
"""Compile a diagnostic-only copy of the original typed-event witness on ARM.

Its rejected reader dump precedes the original assertion; all observation and
publication budgets remain unchanged. This driver uses the parent's binfmt
lease and does not execute DDS behavior during preparation.
"""
import argparse
import importlib.util
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

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm
from rebuild_affected import rooted, sdk_inventory, sdk_root_aliases, source_state
from run_resources import bind_frozen_work


def derived_header(original):
    replacements = [
        ('#include "typed_event_common_ros2.h"', '#include "/work/test/0073.ros-tests/support/source/ros_resources/typed_event_common_ros2.h"'),
        ('#include "../../src/ros/gateway/common/native_io.h"', '#include "/work/src/ros/gateway/common/native_io.h"'),
        ('                require(original!=baseline_.end()&&original->second==reader.second,"denied typed subscription created a new actual DDS reader GID");',
         '                if(original==baseline_.end()||original->second!=reader.second)\n'
         '                {\n'
         '                    snapshot(true,"denied-unexpected");\n'
         '                    std::cout<<"DENIED_READER_REJECTED gid="<<reader.first<<" node="<<reader.second\n'
         '                        <<" baseline_gid_present="<<(original!=baseline_.end())\n'
         '                        <<" actual_subscriptions="<<publisher_->get_subscription_count()\n'
         '                        <<" baseline_readers="<<baseline_.size()<<"\\n"<<std::flush;\n'
         '                    for(auto const& known:baseline_)\n'
         '                    { std::cout<<"DENIED_READER_BASELINE gid="<<known.first<<" node="<<known.second<<"\\n"<<std::flush; }\n'
         '                }\n'
         '                require(original!=baseline_.end()&&original->second==reader.second,"denied typed subscription created a new actual DDS reader GID");')]
    result = original
    for before, after in replacements:
        if result.count(before) != 1:
            raise RuntimeError('frozen original diagnostic insertion anchor differs')
        result = result.replace(before, after)
    # Invert every exact substitution, including the original require retained
    # in the insertion; this guards every other byte and every original budget.
    recovered = result
    for before, after in reversed(replacements):
        if recovered.count(after) != 1:
            raise RuntimeError('ambiguous diagnostic derivation')
        recovered = recovered.replace(after, before)
    if recovered != original:
        raise RuntimeError('diagnostic modified original behavior beyond graph logging')
    return result


def cmake():
    return '''cmake_minimum_required(VERSION 3.16)
project(arm_rpc_reader_diagnostic LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rcl_interfaces REQUIRED)
add_executable(uwvm-ros2-rpc-reader-diagnostic /tmp/addon/rpc_reader_diagnostic.cc)
target_include_directories(uwvm-ros2-rpc-reader-diagnostic PRIVATE /work/src)
target_compile_options(uwvm-ros2-rpc-reader-diagnostic PRIVATE -O1 -g0 -UNDEBUG -std=gnu++17 -MD)
ament_target_dependencies(uwvm-ros2-rpc-reader-diagnostic rclcpp std_msgs rcl_interfaces)
target_link_libraries(uwvm-ros2-rpc-reader-diagnostic pthread)
'''


def closure(mmd, source, sdk, temporary):
    body = mmd.read_text().replace('\\\n', ' ')
    rows = {}
    if ':' not in body:
        raise RuntimeError('invalid actual compiler dependency file')
    for token in shlex.split(body.split(':', 1)[1]):
        logical = posixpath.normpath(token)
        if logical.startswith('/work/'):
            path = source/logical[6:]
        elif logical.startswith('/tmp/'):
            path = temporary/logical[5:]
        elif logical.startswith(('/usr/', '/opt/')):
            path = rooted(sdk, logical)
        else:
            raise RuntimeError('unclassified actual compiler dependency: '+token)
        if not path.is_file():
            raise RuntimeError('missing compiler dependency: '+str(path))
        rows[logical] = {'path': str(path), 'sha256': digest(path), 'bytes': path.stat().st_size}
    return rows


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'rootfs', 'out', 'qemu', 'binfmt-evidence'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    source, sdk = args.source.resolve(strict=True), args.rootfs.resolve(strict=True)
    manifest = source.parent/'source-manifest.json'
    frozen = json.loads(manifest.read_text())
    original = source/'test/0073.ros-tests/support/source/ros_resources/typed_event_witness_ros2.h'
    out = args.out.resolve();out.mkdir(parents=True, mode=0o700, exist_ok=False)
    root, temporary = out/'rootfs', out/'native-temp'
    root.mkdir();temporary.mkdir();addon = temporary/'addon';addon.mkdir()
    mounts, children, evidence, initial, sdk_before, aliases_before = [], [], None, None, None, None
    report = {'status': 'RUNNING', 'scope': 'Diagnostic ARM witness compilation only; no DDS behavior verdict',
        'source': str(source), 'source_manifest': str(manifest), 'source_manifest_sha256': digest(manifest),
        'original_witness': {'path': str(original), 'sha256': digest(original)}, 'commands': [],
        'driver_sha256': digest(__file__), 'runtime_sha256': digest(Path(__file__).with_name('runtime.py'))}
    status = 'FAIL'

    def save():
        (out/'report.json').write_text(json.dumps(report, indent=2)+'\n')

    def checked(command):
        subprocess.run([str(value) for value in command], check=True, capture_output=True, timeout=30)

    def stop():
        for child in reversed(children):
            if child.poll() is None:
                try:
                    os.killpg(child.pid, signal.SIGTERM);child.wait(timeout=5)
                except (ProcessLookupError, subprocess.TimeoutExpired):
                    try:os.killpg(child.pid, signal.SIGKILL)
                    except ProcessLookupError:pass
                    child.wait(timeout=5)

    def interrupted(signum, frame):
        raise RuntimeError('diagnostic build interrupted by signal '+str(signum))

    signal.signal(signal.SIGTERM, interrupted);signal.signal(signal.SIGINT, interrupted)
    try:
        if sys.platform != 'linux' or os.geteuid() != 0:
            raise RuntimeError('root Linux private mount namespace required')
        if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
            raise RuntimeError('launch diagnostic preparation with unshare -m inside the parent lease')
        lease = json.loads(args.binfmt_evidence.read_text())
        registered = (Path('/proc/sys/fs/binfmt_misc')/lease['name']).read_text()
        if registered != lease['entry'] or digest(args.qemu) != lease['qemu_sha256']:
            raise RuntimeError('active parent fixed QEMU lease changed')
        fields = dict(line.split(None, 1) for line in registered.splitlines()[1:] if ' ' in line)
        magic, mask = bytes.fromhex(fields['magic']), bytes.fromhex(fields['mask'])
        if (not registered.startswith('enabled\n') or 'F' not in fields['flags:'] or
                fields['interpreter'] != str(args.qemu.resolve()) or magic[18:20] != b'\xb7\x00' or mask[18:20] != b'\xff\xff'):
            raise RuntimeError('exact fixed-interpreter ELF183 lease required')
        report['binfmt'] = {'path': str(args.binfmt_evidence), 'sha256': digest(args.binfmt_evidence), 'entry': registered}
        initial = source_state(source, frozen)
        sdk_before, aliases_before = sdk_inventory(sdk), sdk_root_aliases(sdk)
        (out/'sdk-before.json').write_text(json.dumps(sdk_before, indent=2)+'\n')
        report['sdk_before'] = {'path': str(out/'sdk-before.json'), 'sha256': digest(out/'sdk-before.json')}
        report['sdk_root_aliases'] = aliases_before
        spec = importlib.util.spec_from_file_location('diagnostic_cgroup', source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
        probe = importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
        evidence = Evidence(out/'execution', probe.probe);evidence.emulator(args.qemu)
        compiler = rooted(sdk, '/usr/bin/aarch64-linux-gnu-g++-13')
        frontend = next(rooted(sdk, '/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus')
            for base in ('usr/lib', 'usr/libexec') if rooted(sdk, '/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus').is_file())
        evidence.target('native-cmake', rooted(sdk, '/usr/bin/cmake'))
        report['compiler'] = evidence.target('native-gcc', compiler)
        report['frontend'] = evidence.target('native-frontend', frontend)
        (addon/'typed_event_witness_diagnostic.h').write_text(derived_header(original.read_text()))
        shutil.copy2(Path(__file__).with_name('rpc_reader_diagnostic.cc'), addon/'rpc_reader_diagnostic.cc')
        generated = temporary/'cmake';generated.mkdir();(generated/'CMakeLists.txt').write_text(cmake())
        report['diagnostic_header'] = {'path': str(addon/'typed_event_witness_diagnostic.h'),
            'sha256': digest(addon/'typed_event_witness_diagnostic.h'), 'exact_derivation': 'two include path relocations and logging before unchanged rejected-GID assertion'}
        checked(['mount', '--make-rprivate', '/'])
        checked(['mount', '--rbind', sdk, root]);mounts.append(root);checked(['mount', '--make-rprivate', root])
        report['source_mount'] = bind_frozen_work(root, source, sdk, out, checked)
        checked(['mount', '--bind', temporary, root/'tmp']);checked(['mount', '--make-rprivate', root])
        home = out/'owned-home';home.mkdir();checked(['mount', '--bind', home, root/'root'])
        for name in ('usr', 'opt', 'etc', 'var/lib/dpkg'):
            checked(['mount', '--bind', sdk/name, root/name])
            checked(['mount', '-o', 'remount,bind,ro', root/name])
        for name in ('proc', 'sys', 'dev'):
            checked(['mount', '--rbind', Path('/')/name, root/name])
            checked(['mount', '--make-rprivate', root/name])
        environment = dict(os.environ)
        for name in ('CC', 'CXX', 'LD_LIBRARY_PATH', 'LD_PRELOAD', 'QEMU_LD_PREFIX', 'CFLAGS', 'CXXFLAGS', 'LDFLAGS'):
            environment.pop(name, None)
        environment['PYTHONDONTWRITEBYTECODE'] = '1'

        def run(name, arguments, required, timeout):
            command = ['/usr/bin/chroot', str(root), '/bin/bash', '-lc',
                'source /opt/ros/jazzy/setup.bash && exec "$@"', 'arm-rpc-diagnostic-build', *arguments]
            row = {'name': name, 'command': command, 'status': 'FAIL'};report['commands'].append(row);save()
            with (out/(name+'.log')).open('xb') as log:
                child = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=environment, start_new_session=True)
            children.append(child);row['pid'] = child.pid;evidence.start(child, name+'/', command)
            row['returncode'] = child.wait(timeout=timeout)
            row['log_sha256'] = digest(out/(name+'.log'))
            if row['returncode']:
                raise RuntimeError('ARM diagnostic compiler returned failure: '+name)
            evidence.check(required, name+'/');row['status'] = 'PASS';save()

        run('configure', ['/usr/bin/cmake', '-S', '/tmp/cmake', '-B', '/tmp/build',
            '-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++-13', '-DCMAKE_BUILD_TYPE=Release',
            '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'], ['native-cmake'], 120)
        run('compile', ['/usr/bin/cmake', '--build', '/tmp/build', '--parallel', '1'],
            ['native-cmake', 'native-gcc', 'native-frontend'], 600)
        report['binary'] = evidence.target('diagnostic-witness', temporary/'build/uwvm-ros2-rpc-reader-diagnostic')
        meta = temporary/'build/CMakeFiles/uwvm-ros2-rpc-reader-diagnostic.dir'
        found = list(meta.rglob('*.o.d'))
        if len(found) != 1:
            raise RuntimeError('exact original compiler dependency file required')
        copied = out/'compiler.d';shutil.copy2(found[0], copied)
        report['raw_dependencies'] = {'path': str(copied), 'sha256': digest(copied), 'actual_compile_path': str(found[0]),
            'inputs': closure(copied, source, sdk, temporary)}
        for name, path in [('flags', meta/'flags.make'), ('link', meta/'link.txt'),
                ('compile_commands', temporary/'build/compile_commands.json'), ('cmake', generated/'CMakeLists.txt')]:
            report[name] = {'path': str(path), 'sha256': digest(path)}
        status = 'PASS'
    except BaseException:
        report['error'] = traceback.format_exc()
    finally:
        stop()
        try:
            report['source_preserved'] = initial is not None and source_state(source, frozen) == initial
            report['sdk_preserved'] = sdk_before is not None and sdk_inventory(sdk) == sdk_before and sdk_root_aliases(sdk) == aliases_before
            if not report['source_preserved'] or not report['sdk_preserved']:status = 'FAIL'
        except BaseException:
            report['preservation_error'] = traceback.format_exc();status = 'FAIL'
        if evidence:
            result = evidence.finish(status);status = result['status']
            report['architecture_evidence'] = {'path': str(evidence.out/'architecture-evidence.json'),
                'sha256': digest(evidence.out/'architecture-evidence.json')}
        report['mount_cleanup'] = []
        for path in reversed(mounts):
            result = subprocess.run(['umount', '-R', str(path)], capture_output=True, text=True, timeout=30)
            report['mount_cleanup'].append({'path': str(path), 'returncode': result.returncode, 'stderr': result.stderr})
            if result.returncode:status = 'FAIL'
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')]
        if report['remaining_owned_mounts']:status = 'FAIL'
        report['status'] = status;save()
    print(report['status'], out/'report.json', flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
