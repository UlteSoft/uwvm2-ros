#!/usr/bin/env python3
"""Six actual dual-ROS default-deny configurations; no ROS SDK is required."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import signal
import socket
import subprocess
import sys
import tempfile
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest


def fixture(source):
    header = (source/'src/uwvm2/uwvm/imported/ros/host.h').read_text()
    signatures = {}
    for name, parameters in re.findall(r'template <core::version (?:V|Version)>\s+inline ::std::uint32_t (\w+)\(([^)]*)\) noexcept', header):
        signatures[name] = re.findall(r'::std::uint(32|64)_t', parameters)
    imports, calls, counts = [], [], {}
    for version in (1, 2):
        names = list(dict.fromkeys(re.findall(r'&host::(\w+)<core::version::ros'+str(version)+'>', header)))
        resources = [name for name in names if name not in ('abi_version', 'capability_open')]
        counts[str(version)] = len(resources)
        for name in ['capability_open', *resources]:
            types = signatures[name]
            imports.append(f'(import "uwvm_ros{version}_v1" "{name}" (func $r{version}_{name} (param '+
                           ' '.join('i'+kind for kind in types)+') (result i32)))')
        calls.append(f'(call $expect (call $r{version}_capability_open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (i32.const 13))')
        calls.append('(if (i64.ne (i64.load (i32.const 8)) (i64.const -1)) (then (call $exit (i32.const 225))))')
        for name in resources:
            types = signatures[name]
            offsets = [-1, 0, 128, 0, 256, 64, 320]
            values = ' '.join(f'(i{kind}.const {offsets[index]})' for index, kind in enumerate(types))
            calls.append(f'(call $expect (call $r{version}_{name} {values}) (i32.const 9))')
    sentinels = '\n'.join(f'(i64.store (i32.const {offset}) (i64.const 6510615555426900570))' for offset in range(256, 328, 8))
    checks = '\n'.join(f'(if (i64.ne (i64.load (i32.const {offset})) (i64.const 6510615555426900570)) (then (call $exit (i32.const 226))))' for offset in range(256, 328, 8))
    text = '(module\n'+'\n'.join(imports)+r'''
      (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
      (import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32) (result i32)))
      (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
      (memory (export "memory") 1) (data (i32.const 0) "pub")
      (data (i32.const 512) "READY ARM DEFAULT\0a")
      (func $expect (param $actual i32) (param $wanted i32)
        (if (i32.ne (local.get $actual) (local.get $wanted)) (then (call $exit (i32.const 224)))))
      (func (export "_start")
        (i32.store (i32.const 64) (i32.const 512)) (i32.store (i32.const 68) (i32.const 18))
        (call $expect (call $write (i32.const 1) (i32.const 64) (i32.const 1) (i32.const 80)) (i32.const 0))
        (i32.store (i32.const 64) (i32.const 96)) (i32.store (i32.const 68) (i32.const 1))
        (call $expect (call $read (i32.const 0) (i32.const 64) (i32.const 1) (i32.const 80)) (i32.const 0))
        (call $expect (i32.load (i32.const 80)) (i32.const 1))
        (i64.store (i32.const 8) (i64.const -1))
    '''+sentinels+'\n'+'\n'.join(calls)+'\n'+checks+'))\n'
    return text, counts


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for flag in ('source', 'uwvm', 'out'):
        p.add_argument('--'+flag, type=Path, required=True)
    p.add_argument('--worker-prefix-json', required=True)
    p.add_argument('--wat2wasm', default='wat2wasm')
    p.add_argument('--llvm-libs', default='')
    args = p.parse_args()
    source = args.source.resolve()
    prefix = json.loads(args.worker_prefix_json)
    if not isinstance(prefix, list) or not prefix or not all(isinstance(x, str) for x in prefix):
        p.error('--worker-prefix-json requires a nonempty argv array beginning with the verified host QEMU')
    sys.path.insert(0, str(source/'tests/ros_cgroup'))
    from probe import probe
    sys.path.insert(0, str(source/'tests/ros'))
    import run_network as network
    evidence = Evidence(args.out.resolve(), probe)
    evidence.emulator(prefix[0])
    worker = evidence.out/'worker'
    shutil.copy2(args.uwvm, worker)
    evidence.target('worker', worker)
    rows, children, listeners = [], [], []
    private = Path(tempfile.mkdtemp(prefix='uwvm-arm-deny-', dir='/tmp'))
    status, error = 'FAIL', None
    try:
        guest_text, counts = fixture(source)
        wat, wasm = evidence.out/'defaults.wat', evidence.out/'defaults.wasm'
        wat.write_text(guest_text)
        compile_command = [args.wat2wasm, '--enable-all', str(wat), '-o', str(wasm)]
        compiled = subprocess.run(compile_command, capture_output=True, timeout=30)
        (evidence.out/'fixture-compile.log').write_bytes(compiled.stdout+compiled.stderr)
        if compiled.returncode:
            raise RuntimeError('FAIL default fixture compile: '+compiled.stderr.decode(errors='replace'))
        for version in (1, 2):
            listener = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            listener.bind(str(private/f'ros{version}.sock'))
            listener.listen(32)
            listener.setblocking(False)
            listeners.append(listener)
        scopes = ['unconfigured', 'global-disabled', 'single-empty', 'group-empty', 'grants-do-not-enable', 'connections-do-not-enable']
        for scope in scopes:
            name = 'default-'+scope
            cli = ['--wasm-set-main-module-name', 'arm_default']
            for version in (1, 2):
                pre = f'--ros{version}-'
                if scope == 'global-disabled':
                    cli += [pre+'global-disable']
                elif scope == 'single-empty':
                    cli += [pre+'single-create', 'arm_default']
                elif scope == 'group-empty':
                    cli += [pre+'group-create', 'empty', pre+'group-add-module', 'empty', 'arm_default']
                elif scope == 'grants-do-not-enable':
                    for cap in network.policy(version, ['publish'])[1]:
                        cli += [pre+'global-grant', *[cap[k] for k in ('alias', 'operation', 'resource', 'type', 'identity')]]
                        for key, value in cap['limits'].items():
                            cli += [pre+'global-set', cap['alias'], key, value]
                elif scope == 'connections-do-not-enable':
                    connection = network.policy(version, [])[0]
                    connection.update({'gateway-socket': str(private/f'ros{version}.sock'), 'gateway-uid': str(os.getuid()), 'connect-timeout-ms': '1000'})
                    for key, value in connection.items():
                        cli += [pre+'global-'+key, value]
            command = [*prefix, str(worker), *cli, '--run', str(wasm)]
            log = evidence.out/(name+'.log')
            env = {'PATH': os.defpath, 'LD_LIBRARY_PATH': args.llvm_libs,
                   'ROS_MASTER_URI': 'http://127.0.0.1:11339', 'ROS_DOMAIN_ID': '73',
                   'ROS_ARM_SYNTHETIC_TEST': 'default-environment-does-not-grant-ROS'}
            with log.open('wb') as output:
                child = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=output,
                                         stderr=subprocess.STDOUT, env=env, start_new_session=True)
            children.append(child)
            evidence.start(child, name, command)
            deadline = time.monotonic()+20
            while b'READY ARM DEFAULT\n' not in log.read_bytes():
                if child.poll() is not None or time.monotonic() >= deadline:
                    raise RuntimeError('FAIL default stdin readiness: '+name)
                evidence.observe(child.pid, name)
                time.sleep(.005)
            evidence.check(['worker'], name)
            child.stdin.write(b'x')
            child.stdin.close()
            code = child.wait(timeout=20)
            accepts = []
            for listener in listeners:
                count = 0
                while True:
                    try:
                        connection, _ = listener.accept()
                        connection.close()
                        count += 1
                    except BlockingIOError:
                        break
                accepts.append(count)
            row = {'name': name, 'status': 'PASS' if code == 0 and accepts == [0, 0] else 'FAIL',
                   'returncode': code, 'pid': child.pid, 'private_listener_accepts': accepts,
                   'capability_open_expected': 13, 'forged_handle_expected': 9,
                   'forged_calls_by_version': counts, 'output_sentinel_bytes': 72,
                   'command': command, 'log_sha256': digest(log)}
            rows.append(row)
            if row['status'] != 'PASS':
                raise RuntimeError('FAIL actual dual ROS defaults: '+name)
        status = 'PASS'
    except BaseException as caught:
        error = traceback.format_exc()
        status = 'BLOCKED' if 'BLOCKED' in str(caught) else 'FAIL'
    finally:
        for child in reversed(children):
            if child.poll() is None:
                os.killpg(child.pid, signal.SIGTERM)
                try:
                    child.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    os.killpg(child.pid, signal.SIGKILL)
                    child.wait(timeout=3)
        for listener in listeners:
            listener.close()
        shutil.rmtree(private)
        result = evidence.finish(status, {'suite': 'default-ROS-deny',
            'limits': 'ROS default rights only; WASIp1 stdio-only defaults are a separate requirement',
            'executed_driver_sha256': digest(__file__), 'source': str(source),
            'results': rows, 'error': error,
            'fixture_sha256': {'wat': digest(wat), 'wasm': digest(wasm)} if 'wasm' in locals() and wasm.exists() else {}})
    print(result['status'], evidence.out/'architecture-evidence.json', flush=True)
    return 0 if result['status'] == 'PASS' else 77 if result['status'] == 'BLOCKED' else 1


if __name__ == '__main__':
    sys.exit(main())
