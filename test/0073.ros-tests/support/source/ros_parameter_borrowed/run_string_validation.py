#!/usr/bin/env python3
"""Measure frozen scalar String validation with Clang and both standard libraries."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import time
import traceback


def digest(path):
    h = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(1048576), b''):
            h.update(block)
    return h.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--compiler', required=True)
    parser.add_argument('--llvm-libs', required=True)
    parser.add_argument('--optimized', action='store_true')
    args = parser.parse_args()
    source, out = args.source.resolve(), args.out.absolute()
    if sys.platform != 'linux' or out.exists():
        parser.error('Linux and a fresh output directory required')
    sys.path.insert(0, str(source / 'test/0073.ros-tests/support/source/ros_cgroup'))
    from probe import probe
    out.mkdir(parents=True)
    before = probe()
    report = {'status': 'FAIL', 'scope': 'Borrowed scalar String validation, six sanitizer profiles',
              'executed_driver_sha256': digest(__file__), 'source': str(source),
              'cgroup_before': before, 'commands': [], 'profiles': []}
    env = os.environ.copy()
    env.update(LD_LIBRARY_PATH=args.llvm_libs, ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',
               UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
    children = []
    project_inputs = {}

    def run(command, label, elf=None, dependency=None):
        log = out / (label + '.log')
        row = {'command': command, 'log': str(log), 'main_proof': None}
        report['commands'].append(row)
        started = time.monotonic()
        with log.open('wb') as stream:
            process = subprocess.Popen(command, env=env, stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
            children.append(process)
            row['wrapper_pid'] = process.pid
            if elf:
                expected = Path(elf).stat()
                until = time.monotonic() + 5
                while process.poll() is None and time.monotonic() < until:
                    p = Path('/proc') / str(process.pid)
                    try:
                        fd = os.open(p / 'exe', os.O_RDONLY)
                        with os.fdopen(fd, 'rb') as native:
                            identity = os.fstat(native.fileno())
                            if (identity.st_dev, identity.st_ino) == (expected.st_dev, expected.st_ino):
                                # Capture the process while it is live, then hash the pinned FD.
                                membership = (p / 'cgroup').read_text().splitlines()
                                parent = int((p / 'stat').read_text().rsplit(')', 1)[1].split()[1])
                                leaf = Path(before['cgroup_ancestors'][0]['path'])
                                exe_path = os.readlink(p / 'exe')
                                current = (p / 'exe').stat()
                                live = str(process.pid) in (leaf / 'cgroup.procs').read_text().split()
                                captured = time.monotonic_ns()
                                assert (current.st_dev, current.st_ino) == (identity.st_dev, identity.st_ino)
                                assert parent == os.getpid() and membership == before['membership'] and live
                                h = hashlib.sha256()
                                for block in iter(lambda: native.read(1048576), b''):
                                    h.update(block)
                                row['main_proof'] = {'pid': process.pid, 'parent_pid': parent,
                                    'ancestry': [parent, process.pid], 'exe_path': exe_path,
                                    'exe_device': identity.st_dev, 'exe_inode': identity.st_ino,
                                    'exe_sha256': h.hexdigest(), 'membership': membership, 'live_cgroup_procs': True,
                                    'live_identity_captured_before_fd_hash': True, 'capture_monotonic_ns': captured}
                                break
                    except (FileNotFoundError, ProcessLookupError):
                        pass
                    time.sleep(0.005)
            try:
                row['returncode'] = process.wait(timeout=180)
            except subprocess.TimeoutExpired:
                os.killpg(process.pid, signal.SIGKILL)
                process.wait(timeout=5)
                raise
        row.update(seconds=time.monotonic()-started, log_sha256=digest(log))
        if dependency and Path(dependency).exists():
            raw = Path(dependency).read_text().replace('\\\n', ' ')
            paths = sorted(set(re.findall(re.escape(str(source)) + r'/[^\s\\]+', raw)))
            row['dependency_sha256'] = digest(dependency)
            row['compiled_project_inputs'] = {str(Path(p).relative_to(source)): digest(p) for p in paths}
            project_inputs.update(row['compiled_project_inputs'])
        assert row['returncode'] == 0, label + ' failed'
        if elf:
            assert row['main_proof'] and row['main_proof']['exe_sha256'] == digest(elf), 'missing actual main ELF proof'
        print('PASS', label, flush=True)
        return row

    try:
        assert before['status'] == 'PASS' and before['effective_memory_max'] == 68719476736
        run([args.compiler, '--version'], 'compiler-version')
        for standard in ('c++17', 'c++20', 'c++26'):
            for library in ('libc++', 'libstdc++'):
                label = standard.replace('+', 'p') + '-' + library.replace('+', 'p')
                binary, dep = out / label, out / (label + '.d')
                command = [args.compiler, '-std=' + standard, '-stdlib=' + library, '-O1', '-g',
                           '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie',
                           '-MMD', '-MF', str(dep), '-I', str(source / 'src'),
                           '-I', str(source / 'third-parties/fast_io/include'),
                           str(source / 'test/0073.ros-tests/support/source/ros_parameter_borrowed/string_validation_test.cc'), '-pthread', '-o', str(binary)]
                if args.optimized:
                    command.insert(1, '-DUWVM_ROS_BORROWED_STRING_OPTIMIZED')
                build = run(command, label + '-build', dependency=dep)
                result = run([str(binary)], label + '-run', elf=binary)
                assert binary.read_bytes()[16:18] == b'\x02\x00', 'sanitizer target is not ET_EXEC'
                report['profiles'].append({'standard': standard, 'library': library,
                    'binary_sha256': digest(binary), 'build_log_sha256': build['log_sha256'],
                    'run_log_sha256': result['log_sha256'], 'checks': int(re.search(r'PASS borrowed scalar String checks calls=(\d+)', Path(result['log']).read_text())[1])})
        assert all(digest(source / name) == sha for name, sha in project_inputs.items()), 'compiled input drift'
        after = probe()
        assert after['status'] == 'PASS' and after['membership'] == before['membership']
        deltas = []
        for a, b in zip(before['cgroup_ancestors'], after['cgroup_ancestors']):
            assert a['path'] == b['path'] and a['memory.max'] == b['memory.max']
            assert (a['memory.events'] is None) == (b['memory.events'] is None)
            if a['memory.events'] is None:
                continue
            old, new = dict(x.split() for x in a['memory.events'].splitlines()), dict(x.split() for x in b['memory.events'].splitlines())
            delta = {k: int(new[k])-int(old[k]) for k in ('oom', 'oom_kill', 'oom_group_kill')}
            assert not any(delta.values()), 'OOM increment'
            deltas.append({'path': a['path'], **delta})
        report.update(status='PASS', cgroup_after=after, oom_deltas=deltas, compiled_project_inputs=project_inputs)
    except Exception as error:
        report.update(error=str(error), traceback=traceback.format_exc())
    finally:
        for process in children:
            if process.poll() is None:
                os.killpg(process.pid, signal.SIGKILL)
                process.wait(timeout=5)
        (out / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
        print(report['status'], str(out / 'report.json'), flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
