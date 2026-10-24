#!/usr/bin/env python3
"""Execute the original complete CLI matrix through the frozen ARM64 Worker with live QEMU evidence."""
import argparse
import importlib.util
import json
from pathlib import Path
import runpy
import shutil
import subprocess
import sys
import traceback

from runtime import Evidence, digest, elf

sys.dont_write_bytecode = True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--uwvm', type=Path, required=True)
    parser.add_argument('--config', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--wat2wasm', default='wat2wasm')
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    worker = args.uwvm.resolve(strict=True)
    config = json.loads(args.config.read_text())
    prefix = config['qemu_argv']
    compiler_path = shutil.which(args.wat2wasm)
    if compiler_path is None:
        raise RuntimeError('BLOCKED configured WAT compiler is unavailable')
    compiler = Path(compiler_path).resolve(strict=True)
    compiler_before = elf(compiler)
    if compiler_before['e_machine'] != 62:
        raise RuntimeError('BLOCKED this host requires an explicitly bound x86_64 WAT compiler')
    spec = importlib.util.spec_from_file_location('ros_cgroup', source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    evidence = Evidence(args.out.resolve(), module.probe)
    evidence.emulator(prefix[0])
    snapshot = evidence.out/'uwvm2-ros'
    shutil.copy2(worker, snapshot)
    if digest(snapshot) != digest(worker):
        raise RuntimeError('ARM Worker changed during CLI snapshot')
    evidence.target('worker', snapshot)
    original = subprocess.run
    launches = []

    def execute(command, *positional, **keyword):
        if not isinstance(command, (list, tuple)) or not command or Path(command[0]).resolve() != snapshot:
            return original(command, *positional, **keyword)
        if positional:
            raise RuntimeError('unsupported positional subprocess.run arguments in original CLI runner')
        timeout = keyword.pop('timeout', None)
        check = keyword.pop('check', False)
        input_data = keyword.pop('input', None)
        if keyword.pop('capture_output', False):
            keyword['stdout'] = keyword['stderr'] = subprocess.PIPE
        if input_data is not None:
            keyword['stdin'] = subprocess.PIPE
        argv = [*prefix, *command]
        name = 'cli-'+str(len(launches))+'/'
        process = subprocess.Popen(argv, **keyword)
        launches.append({'name': name, 'pid': process.pid, 'original_argv': list(command), 'qemu_argv': argv})
        evidence.start(process, name, argv)
        try:
            stdout, stderr = process.communicate(input=input_data, timeout=timeout)
        except BaseException:
            process.kill()
            process.communicate(timeout=5)
            raise
        evidence.check({'worker'}, name)
        result = subprocess.CompletedProcess(command, process.returncode, stdout, stderr)
        if check:
            result.check_returncode()
        return result

    code, status, error = 1, 'FAIL', None
    try:
        subprocess.run = execute
        sys.argv = [str(source/'test/0073.ros-tests/support/source/ros_cli/run.py'), '--uwvm', str(snapshot),
                    '--output', str(evidence.out/'cli-results.json'), '--wat2wasm', str(compiler)]
        runpy.run_path(str(source/'test/0073.ros-tests/support/source/ros_cli/run.py'), run_name='__main__')
        code, status = 0, 'PASS'
    except SystemExit as exit_code:
        code = exit_code.code or 0
        status = 'PASS' if code == 0 else 'FAIL'
    except BaseException:
        error = traceback.format_exc()
    finally:
        subprocess.run = original
        compiler_after = None
        try:
            compiler_after = elf(compiler)
            if compiler_after != compiler_before:
                raise RuntimeError('WAT compiler bytes or identity changed during CLI tests')
        except (OSError, RuntimeError) as compiler_error:
            status, code = 'FAIL', 1
            error = (error or '') + '\n' + str(compiler_error)
        result = evidence.finish(status, {'suite': 'complete-original-cli', 'error': error,
            'driver_sha256': digest(__file__), 'original_driver_sha256': digest(source/'test/0073.ros-tests/support/source/ros_cli/run.py'),
            'input_worker': str(worker), 'input_worker_sha256': digest(worker),
            'config_sha256': digest(args.config), 'qemu_sha256': digest(prefix[0]),
            'wat_compiler_before':compiler_before, 'wat_compiler_after':compiler_after,
            'launches': launches})
        if result['status'] != 'PASS':
            code = 1
    print(result['status'], evidence.out/'architecture-evidence.json', flush=True)
    return code


if __name__ == '__main__':
    raise SystemExit(main())
