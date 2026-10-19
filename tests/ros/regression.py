#!/usr/bin/env python3
"""Execute existing WASI/Trusted-DL and interpreter regressions against a product."""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--uwvm', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--wat2wasm', default='wat2wasm')
    parser.add_argument('--cc', default=os.environ.get('CC', 'cc'))
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[2]
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    rows = []
    commands = []

    def run(name, command, expected_stdout=None):
        commands.append(command)
        try:
            result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30)
            (out/(name+'.stdout.log')).write_bytes(result.stdout)
            (out/(name+'.stderr.log')).write_bytes(result.stderr)
            ok = result.returncode == 0 and (expected_stdout is None or result.stdout == expected_stdout)
            rows.append({'name':name, 'status':'PASS' if ok else 'FAIL', 'returncode':result.returncode})
            return ok
        except FileNotFoundError as error:
            rows.append({'name':name, 'status':'BLOCKED', 'reason':str(error)})
        except subprocess.TimeoutExpired:
            rows.append({'name':name, 'status':'FAIL', 'reason':'bounded test timeout'})
        return False

    wasi = '''(module
      (import "wasi_snapshot_preview1" "args_sizes_get" (func $args (param i32 i32) (result i32)))
      (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
      (memory 1) (data (i32.const 32) "WASI\\0a")
      (func (export "_start")
       (if (call $args (i32.const 0) (i32.const 4)) (then unreachable))
       (if (i32.lt_u (i32.load (i32.const 0)) (i32.const 1)) (then unreachable))
       (i32.store (i32.const 8) (i32.const 32)) (i32.store (i32.const 12) (i32.const 5))
       (if (call $write (i32.const 1) (i32.const 8) (i32.const 1) (i32.const 16)) (then unreachable))
       (if (i32.ne (i32.load (i32.const 16)) (i32.const 5)) (then unreachable))))'''
    memory64 = '''(module
      (import "wasi_snapshot_preview1" "args_sizes_get_wasm64" (func $args (param i64 i64) (result i32)))
      (memory i64 1)
      (func (export "_start")
       (if (call $args (i64.const 0) (i64.const 8)) (then unreachable))
       (if (i64.lt_u (i64.load (i64.const 0)) (i64.const 1)) (then unreachable))))'''
    tail = '''(module
      (func $factorial (param $n i32) (param $acc i32) (result i32)
       (if (result i32) (i32.eqz (local.get $n)) (then (local.get $acc))
        (else (return_call $factorial (i32.sub (local.get $n) (i32.const 1)) (i32.mul (local.get $acc) (local.get $n))))))
      (func (export "_start")
       (if (i32.ne (call $factorial (i32.const 5) (i32.const 1)) (i32.const 120)) (then unreachable))))'''
    for name, wat, expected in [('wasi_preview1',wasi,b'WASI\n'),('wasi_memory64',memory64,None),('runtime_tail',tail,None)]:
        source = out/(name+'.wat'); binary = out/(name+'.wasm')
        source.write_text(wat)
        if run(name+'_compile',[args.wat2wasm,'--enable-all',str(source),'-o',str(binary)]):
            features = ['--wasm-feature-enable-memory64'] if name == 'wasi_memory64' else ['--wasm-feature-enable-tail-call'] if name == 'runtime_tail' else []
            run(name,[str(args.uwvm.resolve()),*features,'--run',str(binary),'hello','world'],expected)
    plugin = out/'libregdl.so'
    if run('trusted_dl_compile',[*shlex.split(args.cc),'-std=c17','-shared','-fPIC',str(root/'examples/0002.dl/regdl.c'),'-o',str(plugin)]):
        guest = out/'trusted_dl.wasm'
        if run('trusted_dl_guest_compile',[args.wat2wasm,'--enable-all',str(root/'examples/0002.dl/main.wat'),'-o',str(guest)]):
            run('trusted_dl',[str(args.uwvm.resolve()),'--wasip1-global-expose-host-api','--wasm-register-dl',str(plugin),'dl.example','--wasm-set-preload-module-attribute','dl.example','copy','all','--run',str(guest),'hello','world'])
    status='PASS' if rows and all(row['status']=='PASS' for row in rows) else 'FAIL' if any(row['status']=='FAIL' for row in rows) else 'BLOCKED'
    (out/'commands.json').write_text(json.dumps(commands,indent=2)+'\n')
    (out/'results.json').write_text(json.dumps({'status':status,'tests':rows},indent=2)+'\n')
    for row in rows: print(row['status'],row['name'],flush=True)
    return 0 if status=='PASS' else 77 if status=='BLOCKED' else 1


if __name__=='__main__': sys.exit(main())
