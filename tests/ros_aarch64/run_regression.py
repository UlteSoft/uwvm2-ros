#!/usr/bin/env python3
"""Run the original nine regression commands per ARM Worker profile (four profiles total 36)."""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import shlex
import shutil
import signal
import subprocess
import sys
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def expected_commands(snapshot, output, source, wat2wasm, compile_prefix):
    """Describe the unchanged nine-command contract without executing it.

    These are the original commands recorded before the cross-run adapter
    redirects the separately frozen Trusted-DL C source or prefixes QEMU.
    The caller also binds the original recipe bytes, so a recipe change needs
    a new explicit qualification rather than reusing a nine-row PASS label.
    """
    snapshot, output, source = map(Path, (snapshot, output, source))
    commands = []
    for name in ('wasi_preview1', 'wasi_memory64', 'runtime_tail'):
        features = (['--wasm-feature-enable-memory64'] if name == 'wasi_memory64' else
                    ['--wasm-feature-enable-tail-call'] if name == 'runtime_tail' else [])
        commands += [[str(wat2wasm), '--enable-all', str(output/(name+'.wat')), '-o', str(output/(name+'.wasm'))],
                     [str(snapshot), *features, '--run', str(output/(name+'.wasm')), 'hello', 'world']]
    plugin = output/'libregdl.so'
    commands += [[*compile_prefix, '-std=c17', '-shared', '-fPIC',
                  str(source/'examples/0002.dl/regdl.c'), '-o', str(plugin)],
                 [str(wat2wasm), '--enable-all', str(source/'examples/0002.dl/main.wat'), '-o', str(output/'trusted_dl.wasm')],
                 [str(snapshot), '--wasip1-global-expose-host-api', '--wasm-register-dl', str(plugin), 'dl.example',
                  '--wasm-set-preload-module-attribute', 'dl.example', 'copy', 'all',
                  '--run', str(output/'trusted_dl.wasm'), 'hello', 'world']]
    return commands


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--manifest', type=Path)
    p.add_argument('--source-addon', type=Path, required=True)
    p.add_argument('--source-addon-manifest', type=Path, required=True)
    p.add_argument('--uwvm', type=Path, required=True)
    p.add_argument('--config', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--wat2wasm', default='wat2wasm')
    p.add_argument('--profile', choices=('none', 'ros1', 'ros2', 'dual'), required=True)
    args = p.parse_args()
    source = args.source.resolve(strict=True)
    worker = args.uwvm.resolve(strict=True)
    manifest_path = args.manifest or source.parent/'source-manifest.json'
    manifest = json.loads(manifest_path.read_text())
    addon = args.source_addon.resolve(strict=True)
    addon_manifest = json.loads(args.source_addon_manifest.read_text())
    config = json.loads(args.config.read_text())
    probe = load('arm_regression_cgroup', source/'tests/ros_cgroup/probe.py')
    evidence = Evidence(args.out.resolve(), probe.probe)
    evidence.emulator(config['qemu_argv'][0])
    snapshot = evidence.out/'uwvm'
    shutil.copy2(worker, snapshot)
    if digest(snapshot) != digest(worker):
        raise RuntimeError('ARM Worker changed while making the regression snapshot')
    evidence.target('worker', snapshot)
    original = load('arm_original_regression', source/'tests/ros/regression.py')
    original_popen, saved_argv = subprocess.Popen, sys.argv
    original_out = evidence.out/'original'
    plugin = original_out/'libregdl.so'
    dependency = evidence.out/'trusted-dl.d'
    compile_prefix = [*config['compiler'], *config['compile_flags'], '-x', 'c',
                      '-MMD', '-MF', str(dependency)]
    environment = dict(os.environ)
    environment.pop('LD_PRELOAD', None)
    environment.pop('QEMU_LD_PREFIX', None)
    environment.update(config.get('environment', {}))
    launches, children, compiler_commands, errors, input_redirects = [], [], [], [], []
    frozen = {'manifest': digest(manifest_path), 'config': digest(args.config),
              'compiler': digest(config['compiler'][0]), 'qemu': digest(config['qemu_argv'][0]),
              'addon_manifest': digest(args.source_addon_manifest)}
    command_binding = None

    def checked(relative):
        expected = manifest[relative] if relative in manifest else addon_manifest[relative]
        expected_sha = expected['sha256'] if isinstance(expected, dict) else expected
        path = (source if relative in manifest else addon)/relative
        if digest(path) != expected_sha or isinstance(expected, dict) and path.stat().st_size != expected['bytes']:
            raise RuntimeError('frozen source changed: '+relative)
        return expected_sha

    def dependency_name(path):
        for base in (source, addon):
            try:
                return str(path.relative_to(base))
            except ValueError:
                pass
        raise ValueError('unexpected non-project MMD input: '+str(path))

    def factory(command, *positional, **keyword):
        if not isinstance(command, (list, tuple)) or not command:
            return original_popen(command, *positional, **keyword)
        command = [str(item) for item in command]
        original_command = list(command)
        for index, token in enumerate(command):
            for relative in addon_manifest:
                if token == str(source/relative):
                    checked(relative)
                    command[index] = str(addon/relative)
        if command != original_command:
            input_redirects.append({'original_command': original_command, 'actual_command': list(command),
                                    'source_addon_manifest_sha256': frozen['addon_manifest']})
        if Path(command[0]).resolve() == snapshot:
            required = ['worker']
            if '--wasm-register-dl' in command:
                passed = Path(command[command.index('--wasm-register-dl')+1]).resolve()
                if passed != plugin:
                    raise RuntimeError('unexpected Trusted-DL plugin path')
                evidence.target('trusted-dl-plugin', plugin)
                required.append('trusted-dl-plugin')
            argv = [*config['qemu_argv'], *command]
            keyword['env'] = environment
            keyword.setdefault('start_new_session', True)
            process = original_popen(argv, *positional, **keyword)
            name = 'regression-launch-'+str(len(launches))+'/'
            launches.append({'name': name, 'pid': process.pid, 'required': required,
                             'original_command': command, 'qemu_command': argv})
            children.append(process)
            evidence.start(process, name, argv)
            return process
        if str(source/'examples/0002.dl/regdl.c') in original_command:
            keyword['env'] = environment
            process = original_popen(command, *positional, **keyword)
            compiler_commands.append({'pid': process.pid, 'command': command, 'original_command': original_command})
            children.append(process)
            evidence.start(process, 'regression-trusted-dl-compile/', command)
            return process
        return original_popen(command, *positional, **keyword)

    code, status, error, actual_inputs = 1, 'FAIL', None, {}
    try:
        for relative in manifest:
            checked(relative)
        for relative in addon_manifest:
            checked(relative)
        subprocess.Popen = factory
        sys.argv = [str(source/'tests/ros/regression.py'), '--uwvm', str(snapshot),
                    '--out', str(original_out), '--wat2wasm', args.wat2wasm,
                    '--cc', shlex.join(compile_prefix)]
        code = original.main()
        status = 'PASS' if code == 0 else 'BLOCKED' if code == 77 else 'FAIL'
        for launch in launches:
            try:
                evidence.check(launch['required'], launch['name'])
            except RuntimeError as caught:
                errors.append(str(caught))
                status, code = 'FAIL', 1
        result = json.loads((original_out/'results.json').read_text())
        command_path = original_out/'commands.json'
        recorded_commands = json.loads(command_path.read_text())
        if recorded_commands != expected_commands(snapshot, original_out, source, args.wat2wasm, compile_prefix):
            raise RuntimeError('original regression command recipes changed or were omitted')
        command_binding = {'path':str(command_path), 'sha256':digest(command_path), 'commands':recorded_commands,
                           'wat2wasm':args.wat2wasm, 'compile_prefix':compile_prefix}
        expected = {'wasi_preview1_compile', 'wasi_preview1', 'wasi_memory64_compile', 'wasi_memory64',
                    'runtime_tail_compile', 'runtime_tail', 'trusted_dl_compile',
                    'trusted_dl_guest_compile', 'trusted_dl'}
        names = {row['name'] for row in result['tests']}
        if status == 'PASS' and (names != expected or len(launches) != 4):
            raise RuntimeError('original regression coverage differs from nine commands/four ARM launches')
        if dependency.is_file():
            for token in shlex.split(dependency.read_text().replace('\\\n', ' ').split(':', 1)[1]):
                relative = dependency_name(Path(token).resolve(strict=True))
                actual_inputs[relative] = checked(relative)
            if 'examples/0002.dl/regdl.c' not in actual_inputs:
                raise RuntimeError('Trusted-DL MMD missing original C source')
        elif status == 'PASS':
            raise RuntimeError('missing Trusted-DL MMD')
        for relative in manifest:
            checked(relative)
        for relative in addon_manifest:
            checked(relative)
        if digest(manifest_path) != frozen['manifest'] or digest(args.config) != frozen['config']:
            raise RuntimeError('source manifest or cross config changed')
        if digest(args.source_addon_manifest) != frozen['addon_manifest']:
            raise RuntimeError('source addon manifest changed')
        if digest(config['compiler'][0]) != frozen['compiler'] or digest(config['qemu_argv'][0]) != frozen['qemu']:
            raise RuntimeError('compiler or QEMU changed')
    except BaseException:
        error = traceback.format_exc()
        code, status = 1, 'FAIL'
    finally:
        subprocess.Popen, sys.argv = original_popen, saved_argv
        for process in children:
            if process.poll() is None:
                if process in [row['process'] for row in evidence.processes
                               if row['name'].startswith('regression-launch-')]:
                    try:
                        os.killpg(process.pid, signal.SIGKILL)
                    except ProcessLookupError:
                        pass
                else:
                    process.kill()
                process.wait(timeout=5)
        report_path = original_out/'results.json'
        finished = evidence.finish(status, {'suite': 'complete-original-regression', 'profile': args.profile,
            'driver_sha256': digest(__file__), 'runtime_sha256': digest(Path(__file__).with_name('runtime.py')),
            'original_driver_sha256': checked('tests/ros/regression.py'),
            'source_manifest_sha256': frozen['manifest'], 'config_sha256': frozen['config'],
            'source_addon': str(addon), 'source_addon_manifest_sha256': frozen['addon_manifest'],
            'compiler_sha256': frozen['compiler'], 'qemu_sha256': frozen['qemu'],
            'input_worker': str(worker), 'input_worker_sha256': digest(worker),
            'launches': launches, 'mapping_failures': errors, 'trusted_dl_compiler_commands': compiler_commands,
            'original_commands':command_binding,
            'source_input_redirects': input_redirects,
            'trusted_dl_mmd': {'path': str(dependency), 'sha256': digest(dependency) if dependency.is_file() else None,
                               'inputs': actual_inputs}, 'error': error,
            'original_results': {'path': str(report_path), 'sha256': digest(report_path),
                                 'result': json.loads(report_path.read_text())} if report_path.is_file() else None,
            'coverage': 'Nine original commands per profile; none/ros1/ros2/dual together total 36'})
        if finished['status'] != 'PASS':
            code = 1
    print(finished['status'], evidence.out/'architecture-evidence.json', flush=True)
    return code


if __name__ == '__main__':
    raise SystemExit(main())
