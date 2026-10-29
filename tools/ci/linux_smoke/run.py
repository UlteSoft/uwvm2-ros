"""Bounded Linux compiler acquisition and four real ROS unit suites.

This is a bootstrap and unit-test lane. It cannot qualify a complete VM,
generated unwind, ROS interoperability, SDK execution or a release artifact.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import platform
import re
import resource
import select
import shutil
import signal
import subprocess
import sys
import time
import traceback

from acquire import digest, download, require, save, unpack
from process_evidence import process_resources


def resources(work):
    status = dict(line.split(':', 1) for line in Path('/proc/self/status').read_text().splitlines() if ':' in line)
    return {'route': os.environ.get('RUNNER_ENVIRONMENT', 'manual'), 'machine': platform.machine(),
        'kernel': platform.release(), 'runner_image': os.environ.get('ImageOS'),
        'runner_image_version': os.environ.get('ImageVersion'), 'os_release': Path('/etc/os-release').read_text(),
        'cpus': len(os.sched_getaffinity(0)), 'uid': os.getuid(), 'gid': os.getgid(),
        'membership': Path('/proc/self/cgroup').read_text(), 'free_bytes': shutil.disk_usage(work).free,
        'process_resources': process_resources(os.getpid()),
        'actual_security': {key: status[key].strip() for key in
            ('CapInh', 'CapPrm', 'CapEff', 'CapBnd', 'CapAmb', 'NoNewPrivs', 'Seccomp')},
        'build_jobs': 1, 'hosted_has_manual_64GiB_admission': False}


class Commands:
    def __init__(self, source, out, work, env, frozen=None):
        self.source, self.out, self.work, self.env = source, out, work, env
        self.frozen = frozen
        self.name = None
        self.images = {}
        self.rows = []

    def sample(self, leader, known, handles):
        processes = {}
        for proc in Path('/proc').iterdir():
            if not proc.name.isdecimal():
                continue
            try:
                row = (proc/'stat').read_text().rsplit(') ', 1)[1].split()
                processes[int(proc.name)] = {'parent': int(row[1]), 'session': int(row[3]), 'birth': row[19], 'state': row[0]}
            except (FileNotFoundError, ProcessLookupError, PermissionError):
                pass
        selected = {pid for pid, birth in handles if pid in processes and
                    processes[pid]['birth'] == birth and processes[pid]['state'] not in ('Z', 'X')}
        # A live recorded member pins the command session's ownership. Never
        # rediscover a reused session after every recorded member has retired.
        if selected:
            selected |= {pid for pid, row in processes.items() if row['session'] == leader}
        while True:
            expanded = selected | {pid for pid, row in processes.items() if row['parent'] in selected}
            if expanded == selected:
                break
            selected = expanded
        require(len(selected) <= 256, 'command descendant bound exceeded')
        for pid in selected:
            proc = Path('/proc')/str(pid)
            try:
                identity_key = (pid, processes[pid]['birth'])
                if identity_key not in handles:
                    require(len(handles) < 256, 'total command actor bound exceeded')
                    descriptor = os.pidfd_open(pid)
                    try:
                        actual = (proc/'stat').read_text().rsplit(') ', 1)[1].split()
                        require(actual[19] == identity_key[1], 'actor birth changed while opening pidfd')
                    except BaseException:
                        os.close(descriptor)
                        raise
                    handles[identity_key] = descriptor
                image = proc/'exe'
                with image.open('rb') as stream:
                    info = os.fstat(stream.fileno())
                    key = (info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns)
                    if key not in self.images:
                        self.images[key] = hashlib.file_digest(stream, 'sha256').hexdigest()
                    argv = (proc/'cmdline').read_bytes().decode(errors='replace').split('\0')
                    executable = os.readlink(image)
                    membership = (proc/'cgroup').read_text()
                    maps_text = (proc/'maps').read_text() if self.frozen is not None else None
                    mapped = self.frozen.mapped_compiler(maps_text) if maps_text is not None else None
                    after = image.stat()
                    if key != (after.st_dev, after.st_ino, after.st_size, after.st_mtime_ns, after.st_ctime_ns):
                        continue
                    if (proc/'stat').read_text().rsplit(') ', 1)[1].split()[19] != processes[pid]['birth']:
                        continue
                identity = (pid, processes[pid]['birth'], self.images[key])
                if identity not in known:
                    known[identity] = {'pid': pid, **processes[pid], 'executable_sha256': self.images[key],
                        'executable': executable, 'argv': argv, 'cgroup': membership,
                        'executable_identity': {'device': info.st_dev, 'inode': info.st_ino,
                                                'bytes': info.st_size, 'mtime_ns': info.st_mtime_ns,
                                                'ctime_ns': info.st_ctime_ns},
                        'observed_ns': time.monotonic_ns(), 'executable_open_fd_verified': True}
                if mapped is not None and 'compiler_image' not in known[identity]:
                    path = self.out/(self.name+'-'+str(pid)+'-'+processes[pid]['birth']+'.maps')
                    path.write_text(maps_text)
                    known[identity].update(compiler_image=mapped, compiler_image_observed_ns=time.monotonic_ns(),
                        compiler_image_argv=argv, maps_original={'path': str(path), 'sha256': digest(path)})
            except (FileNotFoundError, ProcessLookupError, PermissionError):
                pass

    def run(self, name, argv, timeout, expected=0):
        self.name = name
        stdout, stderr = self.out/(name+'.stdout'), self.out/(name+'.stderr')
        row = {'name': name, 'argv': list(map(str, argv)), 'cwd': str(self.source), 'status': 'FAIL',
               'before': resources(self.work)}
        child, started, actors, handles = None, time.monotonic(), {}, {}
        try:
            with stdout.open('xb') as output, stderr.open('xb') as error:
                child = subprocess.Popen(row['argv'], cwd=self.source, env=self.env, stdin=subprocess.DEVNULL,
                    stdout=output, stderr=error, start_new_session=True)
                row['pid'] = child.pid
                descriptor = os.pidfd_open(child.pid)
                child_birth = (Path('/proc')/str(child.pid)/'stat').read_text().rsplit(') ', 1)[1].split()[19]
                handles[(child.pid, child_birth)] = descriptor
                row['birth'] = child_birth
                while child.poll() is None:
                    self.sample(child.pid, actors, handles)
                    require(time.monotonic() - started < timeout, 'bounded command timeout')
                    require(stdout.stat().st_size + stderr.stat().st_size < 16 * 1024**2, 'bounded command logs exceeded')
                    require(shutil.disk_usage(self.work).free >= 2 * 1024**3, '2GiB owned work reserve reached')
                    time.sleep(0.02)
                row['exit'] = child.wait(timeout=5)
                require(time.monotonic() - started < timeout, 'final command deadline exceeded')
                require(stdout.stat().st_size + stderr.stat().st_size < 16 * 1024**2, 'final command logs exceeded')
                self.sample(child.pid, actors, handles)
                require(row['exit'] == expected, 'actual command failed: '+name)
                row['status'] = 'PASS'
        except BaseException as error:
            row['error'] = repr(error)
            raise
        finally:
            retirement_error = None
            try:
                for descriptor in handles.values():
                    if not select.select([descriptor], [], [], 0)[0]:
                        try:
                            signal.pidfd_send_signal(descriptor, signal.SIGKILL)
                        except ProcessLookupError:
                            pass
                if child is not None:
                    child.wait(timeout=10)
                deadline = time.monotonic() + 10
                for descriptor in handles.values():
                    require(select.select([descriptor], [], [], max(0, deadline-time.monotonic()))[0],
                            'recorded command actor did not retire')
                row['recorded_actors_retired'] = True
            except BaseException as error:
                retirement_error = error
                row.update(status='FAIL', retirement_error=repr(error), recorded_actors_retired=False)
            finally:
                for descriptor in handles.values():
                    os.close(descriptor)
            row.update(seconds=time.monotonic()-started, actors=list(actors.values()), after=resources(self.work))
            for key, path in (('stdout', stdout), ('stderr', stderr)):
                if path.exists():
                    row[key] = {'path': str(path), 'bytes': path.stat().st_size, 'sha256': digest(path)}
            self.rows.append(row)
            save(self.out/(name+'.command.json'), row)
            if retirement_error is not None:
                raise retirement_error
        return row


def source_census(source):
    roots = ('src/uwvm2/imported/ros', 'third-parties/fast_io/include',
             'test/0073.ros-tests/support/source/ros', 'test/0073.ros-tests/support/source/ros_security',
             'test/0073.ros-tests/support/source/ros_ipc', 'test/0073.ros-tests/support/source/ros_ipc_client_deadline',
             'tools/ci/linux_smoke', '.github/workflows')
    rows = []
    for name in roots:
        root = source/name
        require(root.is_dir() and not root.is_symlink(), 'selected source root absent: '+name)
        for path in sorted(root.rglob('*')):
            require(not path.is_symlink(), 'unexpected source alias: '+str(path))
            if path.is_file():
                require(path.stat().st_size < 16*1024**2, 'bounded source input exceeded')
                rows.append({'path': str(path.relative_to(source)), 'bytes': path.stat().st_size,
                             'mode': path.stat().st_mode & 0o7777, 'sha256': digest(path)})
    return rows


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--work', type=Path, required=True)
    parser.add_argument('--route', choices=('github-hosted', 'guarded-manual'), default='github-hosted')
    parser.add_argument('--frozen-compiler', type=Path)
    parser.add_argument('--frozen-archive', type=Path)
    parser.add_argument('--frozen-sdk', type=Path)
    args = parser.parse_args()
    source, out, work = args.source.resolve(), args.out.resolve(), args.work.resolve()
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    report = {'format': 'uwvm-linux-compiler-ros-units-v1', 'status': 'FAIL', 'route': args.route,
        'required_suites': ['security', 'ipc', 'ipc-handshake', 'ipc-client-deadline'], 'executed_suites': [],
        'product_qualified': False, 'actual_hosted_qualification': False,
        'targets': {name: {'status': 'BLOCKED', 'reason': 'Full product, EH/unwind, ABI, ROS and SDK runtime gates remain unqualified'}
            for name in ('x86_64', 'i686', 'aarch64', 'arm', 'armhf')}}
    commands = None
    frozen = None
    try:
        require(sys.platform == 'linux' and platform.machine() == 'x86_64', 'Linux x86_64 bootstrap required')
        if args.route == 'github-hosted':
            require(not any((args.frozen_compiler, args.frozen_archive, args.frozen_sdk)),
                    'the existing frozen cache route requires own guarded manual admission')
            require(os.environ.get('GITHUB_ACTIONS') == 'true' and
                os.environ.get('RUNNER_ENVIRONMENT') == 'github-hosted', 'official hosted runner required')
            report['workflow_identity'] = {name: os.environ.get(name) for name in
                ('GITHUB_SHA', 'GITHUB_RUN_ID', 'GITHUB_RUN_ATTEMPT', 'GITHUB_EVENT_NAME', 'GITHUB_REPOSITORY')}
        else:
            context = json.loads(Path(os.environ['UWVM_R18_CONTEXT']).read_text())
            require(context['mode'] == 'ci-smoke' and context['nonce'] == os.environ['UWVM_R18_NONCE'],
                    'own exact guarded manual operation required')
            require(context['guardian_proof'] and context['benign_proof'], 'manual own proof and benign required')
            report['manual_context_sha256'] = digest(os.environ['UWVM_R18_CONTEXT'])
        work.mkdir(mode=0o700, parents=True, exist_ok=False)
        require(len(str(work/'units/ipc/ipc-abcdefgh/ros-client-XXXXXX/socket').encode()) < 108,
                'select a short owned work path for the real Unix socket fixture')
        report['source_inputs'] = source_census(source)
        save(out/'source-before.json', report['source_inputs'])
        report['resources'] = resources(work)
        require(report['resources']['free_bytes'] >= 3 * 1024**3, '3GiB real owned work space required')
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        resource.setrlimit(resource.RLIMIT_AS, (14 * 1024**3, 14 * 1024**3))
        lock_path = Path(__file__).with_name('inputs.lock.json')
        lock = json.loads(lock_path.read_text())
        require(lock['format'] == 'uwvm-linux-smoke-inputs-v1', 'unknown sealed input contract')
        shutil.copy2(lock_path, out/'inputs.lock.json')
        if any((args.frozen_compiler, args.frozen_archive, args.frozen_sdk)):
            require(args.route == 'guarded-manual' and
                    all((args.frozen_compiler, args.frozen_archive, args.frozen_sdk)),
                    'all three frozen input mounts and guarded admission are required')
            from frozen_inputs import FrozenInputs
            frozen = FrozenInputs(args.frozen_compiler, args.frozen_archive, args.frozen_sdk)
            before = frozen.verify()
            save(out/'frozen-inputs-before.json', before)
            report['frozen_inputs'] = {'before_sha256': digest(out/'frozen-inputs-before.json'),
                'archive_sha256': before['archive']['sha256'], 'complete_entries': len(before['files']),
                'compiler_image': before['compiler_image'], 'network_used': False}
            compiler = args.frozen_compiler/frozen.lock['compiler']
            sdk = args.frozen_sdk
        else:
            compiler_archive = work/'llvm.tar.xz'
            report['compiler_download'] = download(lock['compiler'], compiler_archive, out/'compiler-download.json')
            report['compiler_archive'] = unpack(compiler_archive, work/'llvm')
            candidates = sorted((work/'llvm').rglob('bin/clang++'))
            require(len(candidates) == 1 and candidates[0].is_file(), 'one real compiler required')
            compiler = candidates[0]
            require(compiler.resolve().is_relative_to(work/'llvm'), 'compiler alias leaves verified archive')
            sdk = work/'sdk-fixtures'
            for index, row in enumerate(lock['sdk_fixtures']):
                target = sdk/row['path']
                target.parent.mkdir(parents=True, exist_ok=True)
                download(row, target, out/('sdk-fixture-'+str(index)+'.json'))
        report['compiler'] = {'path': str(compiler), 'sha256': digest(compiler)}
        flags = '-std=c++26 -O1 -g0'
        if frozen is not None:
            flags += ' ' + ' '.join(frozen.lock['compiler_flags'])
        env = dict(os.environ, CXX=str(compiler), ROS_TEST_CXXFLAGS=flags,
            PATH=str(compiler.parent)+':/usr/bin:/bin', PYTHONDONTWRITEBYTECODE='1',
            HOME=str(work/'home'), TMPDIR=str(work/'tmp'), LC_ALL='C')
        for key in ('HOME', 'TMPDIR'):
            Path(env[key]).mkdir()
        commands = Commands(source, out, work, env, frozen)
        commands.run('compiler-version', [compiler, '--version'], 30)
        require(re.search(r'clang version 23\.1\.4\b', (out/'compiler-version.stdout').read_text()),
                'acquired compiler version differs')
        unit = source/'test/0073.ros-tests/support/source/ros/run_unit.py'
        commands.run('missing-sdk-refusal', [sys.executable, '-B', unit, '--suite', 'security',
            '--sdk-root', work/'absent-sdk', '--out', work/'refusal'], 15, expected=77)
        refused = json.loads((work/'refusal/results.json').read_text())
        require(refused['status'] == 'BLOCKED' and len(refused['suites']) == 1 and
                refused['suites'][0]['executed'] is False and refused['suites'][0]['compiled'] is False,
                'missing public fixture must block before compilation')
        report['missing_sdk_refusal'] = refused
        observed = []
        for suite in ('security', 'ipc'):
            selected = work/'units'/suite
            row = commands.run(suite+'-units', [sys.executable, '-B', unit, '--suite', suite,
                '--sdk-root', sdk, '--out', selected, '--require-exec-observation'], 720)
            result = json.loads((selected/'results.json').read_text())
            require(result['status'] == 'PASS' and result['suites'], 'actual nonempty unit results required')
            require(all(item.get('compiled') and item.get('executed') and item['status'] == 'PASS'
                for item in result['suites']), 'unit suite silently skipped')
            for item in result['suites']:
                evidence = item.get('actual_exec') or {}
                actual = evidence.get('actual_exec') or {}
                require(evidence.get('status') == 'OBSERVED_AND_RETIRED' and evidence.get('retired') is True
                    and evidence.get('exit') == item['returncode'] == 0,
                    'every unit requires actual exec identity and a reaped successful exit')
                require(actual.get('before_first_user_instruction') is True
                    and actual.get('executable_sha256') == digest(selected/item['suite'])
                    and actual.get('pid') == evidence.get('pid') and actual.get('birth') == evidence.get('birth')
                    and actual['observed_ns'] < evidence['released_ns'] < evidence['exit_observed_ns'],
                    'unit exec-stop identity or execution ordering differs')
                require(actual.get('resources', {}).get('cgroup_resolution'),
                    'unit actual resources or explicit unreadable reason missing')
            if frozen is None:
                require(any(actor['executable_sha256'] == report['compiler']['sha256'] for actor in row['actors']),
                        'actual acquired compiler process was not observed')
            else:
                require(any(actor.get('compiler_image', {}).get('actual_mapped_image_verified') is True
                            for actor in row['actors']), 'actual frozen Clang mapping and original FD were not observed')
            observed.extend(result['suites'])
        require([row['suite'] for row in observed] == lock['required_suites'], 'selected unit census differs')
        report.update(status='PASS_COMPILER_AND_FOUR_ROS_UNIT_SUITES_ONLY', executed_suites=observed,
            actual_hosted_qualification=args.route == 'github-hosted')
    except BaseException as error:
        report.update(status='FAIL', error=repr(error), traceback=traceback.format_exc())
    finally:
        if commands is not None:
            report['commands'] = commands.rows
        try:
            for suite in ('security', 'ipc'):
                selected = work/'units'/suite
                if selected.exists():
                    shutil.copytree(selected, out/(suite+'-units'))
            if (work/'refusal').exists():
                shutil.copytree(work/'refusal', out/'missing-sdk-refusal')
            if 'source_inputs' in report:
                after = source_census(source)
                save(out/'source-after.json', after)
                require(after == report['source_inputs'], 'unit source or workflow bytes changed during execution')
            if frozen is not None:
                after = frozen.verify()
                save(out/'frozen-inputs-after.json', after)
                report['frozen_inputs_after_sha256'] = digest(out/'frozen-inputs-after.json')
        except BaseException as error:
            report.update(status='FAIL', evidence_preservation_error=repr(error))
        finally:
            if frozen is not None:
                frozen.close()
        report['finished_ns'] = time.monotonic_ns()
        save(out/'result.json', report)
        print(json.dumps({key: report[key] for key in ('status', 'product_qualified', 'actual_hosted_qualification')}))
    return 0 if report['status'] == 'PASS_COMPILER_AND_FOUR_ROS_UNIT_SUITES_ONLY' else 1


if __name__ == '__main__':
    raise SystemExit(main())
