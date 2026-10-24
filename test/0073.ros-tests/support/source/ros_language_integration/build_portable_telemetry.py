#!/usr/bin/env python3
"""Serial Linux producer/validator/refusal matrix for five real language apps.

Prepare an owned bounded tmpfs with explicit inputs.json. Compiler/toolchain
prefixes are trusted, pinned Host inputs; the source SDK is a frozen copy. This
owner never installs packages, changes cgroup limits, starts native ROS or adds
Guest grants. Each raw command/output/status is retained, including failures.
Use separate native acceptance to establish actual ROS message delivery.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import signal
import subprocess
import time
import traceback


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--resume', action='store_true',
                        help='Reuse only complete earlier six-command product groups after raw file/status/hash checks')
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    group = Path(inputs['cgroup']['path'])
    source = root / 'source'
    output = root / 'out'
    output.mkdir(exist_ok=True)
    for name in ('commands', 'home', 'tmp'):
        (output / name).mkdir(exist_ok=True)
    events_before = (group / 'memory.events').read_text()
    oom_before = {key: value for key, value in map(str.split, events_before.splitlines()) if key.startswith('oom')}
    report = {'status': 'RUNNING', 'scope': 'Fresh C/C++/Rust/TinyGo/AssemblyScript '
              'ROS1/ROS2 Wasms, complete validation/import inspection and exact '
              'default-denial/argument outcomes; native/performance separate',
              'inputs': inputs, 'commands': [], 'artifacts': [],
              'events_before': events_before}
    if args.resume:
        previous_path = output / 'portable-results.json'
        previous_bytes = previous_path.read_bytes()
        previous = json.loads(previous_bytes)
        assert previous['status'] == 'FAILED_RETAINED'
        accepted = []
        for product in previous['artifacts']:
            path = Path(product['path'])
            assert path.stat().st_size == product['bytes'] and sha(path) == product['sha256']
            prefix = product['language'] + '-ros' + str(product['backend'])
            rows = [row for row in previous['commands'] if row['label'].startswith(prefix + '-')]
            assert {row['label'] for row in rows} == {prefix + suffix for suffix in (
                '-build', '-validate', '-imports', '-default-denial', '-argument-unknown', '-argument-extra')}
            for row in rows:
                assert row['exit'] == row['expected_exit'] and row['forced_signal'] is None
                for stream in ('stdout', 'stderr'):
                    assert sha(output / 'commands' / row['label'] / stream) == row[stream + '_sha256']
            accepted.extend(rows)
        report['commands'] = accepted
        report['artifacts'] = previous['artifacts']
        report['resumed_from'] = {'previous_report_sha256': hashlib.sha256(previous_bytes).hexdigest(),
                                  'completed_products_reused': len(previous['artifacts'])}

    def save():
        (output / 'portable-results.json').write_text(json.dumps(report, indent=2) + '\n')

    def guard():
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
        info = group.stat()
        assert [info.st_dev, info.st_ino] == inputs['cgroup']['identity']
        assert (group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (group / 'memory.swap.max').read_text().strip() == '0'
        current_oom = {key: value for key, value in map(str.split, (group / 'memory.events').read_text().splitlines()) if key.startswith('oom')}
        assert current_oom == oom_before, 'shared OOM/kill counter changed'
        return int((group / 'memory.current').read_text())

    def descendants(pid):
        found, pending = {pid}, [pid]
        while pending:
            parent = pending.pop()
            try:
                children = (Path('/proc') / str(parent) / 'task' / str(parent) / 'children').read_text().split()
            except (FileNotFoundError, ProcessLookupError):
                continue
            for child in map(int, children):
                if child not in found:
                    found.add(child)
                    pending.append(child)
        return found

    def memory(pid):
        try:
            return int((Path('/proc') / str(pid) / 'statm').read_text().split()[1]) * os.sysconf('SC_PAGE_SIZE')
        except (FileNotFoundError, ProcessLookupError):
            return 0

    def limits():
        resource.setrlimit(resource.RLIMIT_AS, (16 << 30, 16 << 30))
        resource.setrlimit(resource.RLIMIT_CPU, (480, 480))
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        resource.setrlimit(resource.RLIMIT_FSIZE, (128 << 20, 128 << 20))
        resource.setrlimit(resource.RLIMIT_NOFILE, (4096, 4096))

    def run(label, argv, cwd=source, extra=None, expected=0, timeout=180):
        deadline = time.monotonic() + 60
        while guard() >= 61 << 30:
            assert time.monotonic() < deadline, 'shared cgroup lacks 3GiB admission margin'
            time.sleep(0.5)
        directory = output / 'commands' / label
        directory.mkdir()
        env = {'PATH': '/usr/bin:/bin', 'HOME': str(output / 'home'),
               'TMPDIR': str(output / 'tmp'), 'LANG': 'C.UTF-8', 'LC_ALL': 'C.UTF-8',
               'OMP_NUM_THREADS': '1', 'OPENBLAS_NUM_THREADS': '1'}
        env.update(extra or {})
        argv = list(map(str, argv))
        row = {'label': label, 'argv': argv, 'cwd': str(cwd), 'env': env,
               'expected_exit': expected, 'exit': None, 'forced_signal': None,
               'max_sampled_owned_rss': 0, 'started_monotonic_ns': time.monotonic_ns()}
        report['commands'].append(row)
        save()
        process = None
        try:
            with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
                process = subprocess.Popen(argv, cwd=cwd, env=env, stdin=subprocess.DEVNULL,
                                           stdout=stdout, stderr=stderr, start_new_session=True,
                                           preexec_fn=limits)
            row['pid'] = process.pid
            until = time.monotonic() + timeout
            while process.poll() is None:
                assert guard() < 63 << 30, 'shared running margin'
                children = descendants(process.pid)
                used = sum(memory(pid) for pid in children)
                row['max_sampled_owned_rss'] = max(row['max_sampled_owned_rss'], used)
                assert used <= 768 << 20, '768MiB sampled compiler/process-tree RSS bound'
                for pid in children:
                    try:
                        assert (Path('/proc') / str(pid) / 'cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
                    except (FileNotFoundError, ProcessLookupError):
                        pass
                assert time.monotonic() < until, 'command deadline'
                assert sum((directory / name).stat().st_size for name in ('stdout', 'stderr')) <= 8 << 20, 'output bound'
                time.sleep(0.05)
            row['exit'] = process.wait()
        finally:
            if process is not None:
                if process.poll() is None:
                    os.killpg(process.pid, signal.SIGKILL)
                    row['forced_signal'] = 'SIGKILL owned process group after failed guard/deadline'
                row['exit'] = process.wait()
            row['finished_monotonic_ns'] = time.monotonic_ns()
            for name in ('stdout', 'stderr'):
                if (directory / name).exists():
                    row[name + '_sha256'] = sha(directory / name)
            save()
        assert row['exit'] == expected, (label, row['exit'], (directory / 'stderr').read_text()[-2400:])
        print(label, 'exit', row['exit'], flush=True)
        return directory

    save()
    try:
        os.sched_setaffinity(0, {20})
        guard()
        assert root.is_mount() and root.stat().st_dev != root.parent.stat().st_dev
        for pin in inputs['pins']:
            assert Path(pin['path']).stat().st_size == pin['bytes'] and sha(pin['path']) == pin['sha256'], pin['path']
        tools = {name: Path(value) for name, value in inputs['tools'].items()}
        kit = source / 'sdk/origin/examples/portable_telemetry'
        for backend in (1, 2):
            for language in ('c', 'cpp', 'rust', 'tinygo', 'assemblyscript'):
                name = language + '-ros' + str(backend)
                if any(row['language'] == language and row['backend'] == backend for row in report['artifacts']):
                    print(name, 'reused exact completed product and six raw outcomes', flush=True)
                    continue
                artifact = output / (name + '.wasm')
                if language in ('c', 'cpp'):
                    compiler = tools['wasi'] / 'bin' / ('clang++' if language == 'cpp' else 'clang')
                    argv = [compiler, '-std=c++20' if language == 'cpp' else '-std=c11', '-Os', '-g0',
                            '-DROSWASM_EXAMPLE_ROS_VERSION=' + str(backend), '-I', source / 'sdk/origin/include',
                            '-MD', '-MF', output / (name + '.d'), kit / language / ('main.cpp' if language == 'cpp' else 'main.c'),
                            '-Wl,--threads=1,--initial-memory=8388608,--max-memory=33554432,-z,stack-size=1048576', '-o', artifact]
                    if language == 'cpp':
                        argv[1:1] = ['-fno-exceptions', '-fno-rtti', '-I', source / 'third-parties/fast_io/include']
                    run(name + '-build', argv)
                elif language == 'rust':
                    env = {'CARGO_HOME': str(output / 'cargo-home'), 'CARGO_TARGET_DIR': str(output / 'cargo-target'),
                           'CARGO_BUILD_JOBS': '1', 'CARGO_NET_OFFLINE': 'true',
                           'RUSTC': str(tools['rust'] / 'bin/rustc'), 'RUSTDOC': str(tools['rust'] / 'bin/rustdoc'),
                           'RUSTFLAGS': '-C opt-level=z -C codegen-units=1 -C debuginfo=0 -C link-arg=--threads=1 -C link-arg=--initial-memory=8388608 -C link-arg=--max-memory=33554432 -C link-arg=-z -C link-arg=stack-size=1048576'}
                    run(name + '-build', [tools['rust'] / 'bin/cargo', 'build', '--offline', '--locked', '--release',
                        '--target', 'wasm32-wasip1', '--no-default-features', '--features', 'ros' + str(backend), '-j', '1'], cwd=kit / 'rust', extra=env)
                    artifact.write_bytes((output / 'cargo-target/wasm32-wasip1/release/roswasm-portable-telemetry.wasm').read_bytes())
                elif language == 'tinygo':
                    env = {'PATH': str(tools['go'] / 'bin') + ':/usr/bin:/bin', 'GOROOT': str(tools['go']),
                           'TINYGOROOT': str(tools['tinygo']), 'GOCACHE': str(output / 'go-cache'),
                           'GOPATH': str(output / 'go-path'), 'GOPROXY': 'off', 'GOSUMDB': 'off',
                           'GOTOOLCHAIN': 'local', 'GOMAXPROCS': '1', 'GOMEMLIMIT': '512MiB'}
                    run(name + '-build', [tools['tinygo'] / 'bin/tinygo', 'build', '-target=wasi',
                        '-tags=ros' + str(backend), '-scheduler=none', '-gc=leaking', '-panic=trap', '-p=1', '-o', artifact, '.'], cwd=kit / 'tinygo', extra=env, timeout=480)
                else:
                    run(name + '-build', ['/usr/bin/node', '--max-old-space-size=256', '--disable-wasm-trap-handler',
                        tools['asc'], kit / 'assemblyscript' / ('ros' + str(backend) + '.ts'), '--outFile', artifact,
                        '--runtime', 'incremental', '--initialMemory', '128', '--maximumMemory', '512',
                        '--optimizeLevel', '2', '--shrinkLevel', '1', '--use', 'abort=sdk/origin/assemblyscript/api/abortGuest'])
                run(name + '-validate', [tools['validator'], 'validate', artifact])
                run(name + '-imports', ['/usr/bin/python3', '-I', '-B', source / 'sdk/origin/tools/check_wasm_imports.py',
                    artifact, '--backend', str(backend), '--allow-module', 'wasi_snapshot_preview1', '--report', output / (name + '-imports.json')])
                worker = [inputs['runtime'], '-Rint', '--wasip1-global-noinherit-system-environment', '--run', artifact]
                denied = run(name + '-default-denial', [*worker, 'deny'])
                assert (denied / 'stdout').read_bytes() == b'PASS_ROS_DEFAULT_DENIED\n' and (denied / 'stderr').read_bytes() == b''
                for suffix, values in (('unknown', ['unknown']), ('extra', ['deny', 'extra'])):
                    refused = run(name + '-argument-' + suffix, [*worker, *values], expected=2)
                    assert (refused / 'stdout').read_bytes() == (refused / 'stderr').read_bytes() == b''
                report['artifacts'].append({'language': language, 'backend': backend, 'path': str(artifact),
                    'bytes': artifact.stat().st_size, 'sha256': sha(artifact), 'fresh_compiler_product': True})
                save()
        for pin in inputs['pins']:
            assert Path(pin['path']).stat().st_size == pin['bytes'] and sha(pin['path']) == pin['sha256'], pin['path']
        report['source_and_tool_pins_preserved'] = True
        report['status'] = 'ACTUAL_FIVE_PORTABLE_LANGUAGE_TEN_WASM_MATRIX_PASS'
    except BaseException as error:
        report['status'] = 'FAILED_RETAINED'
        report['error'] = repr(error)
        report['traceback'] = traceback.format_exc()
        print('PORTABLE MATRIX FAILURE', repr(error), flush=True)
    finally:
        report['events_after'] = (group / 'memory.events').read_text()
        save()
    return 0 if report['status'].endswith('_PASS') else 1


if __name__ == '__main__':
    raise SystemExit(main())
