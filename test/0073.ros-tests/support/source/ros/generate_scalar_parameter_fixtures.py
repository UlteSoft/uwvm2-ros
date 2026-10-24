#!/usr/bin/env python3
"""Write scalar Wasm sources/profiles; compile only with an explicit --compile."""
import argparse
import json
from pathlib import Path
import subprocess
import run_rpc as rpc
import scalar_parameters as scalar


def literal(value):
    return ''.join('\\'+format(byte,'02x') for byte in value)


def signed(bits, width):
    return bits if bits < (1 << (width-1)) else bits-(1 << width)


def guest(version, description):
    kind, scenario = description['kind'], description['scenario']
    grants = rpc.scalar_grants(version, kind, scenario)
    aliases = {cap['alias']: (1024+index*16, index*8) for index,cap in enumerate(grants)}
    imports = {'get', 'set'} | ({'declare', 'undeclare'} if version == 2 and scenario.startswith(('local_', 'undeclare_')) else set())
    source = ['(module', f'  (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))']
    source += [f'  (import "uwvm_ros{version}_v1" "parameter_{op}" (func ${op} (param i64 i32 i64 i64 i64 i64 i64) (result i32)))' for op in sorted(imports)]
    source += ['  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))',
               '  (memory (export "memory") 1)']
    source += [f'  (data (i32.const {offset}) "{alias}")' for alias,(offset,_) in aliases.items()]
    source += ['  (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))',
               '  (func $sentinels', '    (i64.store (i32.const 64) (i64.const 1234605616436508552))']
    source += [f'    (i64.store (i32.const {offset}) (i64.const 6510615555426900570))' for offset in range(256,320,8)]
    source += ['  )', '  (func $buffer_unchanged']
    source += [f'    (if (i64.ne (i64.load (i32.const {offset})) (i64.const 6510615555426900570)) (then (call $exit (i32.const 201))))' for offset in range(256,320,8)]
    source += ['  )', '  (func $unchanged',
               '    (if (i64.ne (i64.load (i32.const 64)) (i64.const 1234605616436508552)) (then (call $exit (i32.const 202))))',
               '    (call $buffer_unchanged))']
    body = []
    assertions = []
    data = []

    def invoke(operation, alias, request=b'', error=0, response=b''):
        offset = 4096+len(data)*64
        data.append((offset, request))
        body.append('    (call $sentinels)')
        call = f'(call ${operation} (i64.load (i32.const {aliases[alias][1]})) (i32.const 0) (i64.const {offset}) (i64.const {len(request)}) (i64.const 256) (i64.const 64) (i64.const 64))'
        if error:
            body.extend([f'    (if (i32.ne {call} (i32.const {error})) (then (call $exit (i32.const 206))))',
                         '    (call $unchanged)'])
        else:
            body.extend([f'    (call $check {call})',
                         f'    (if (i64.ne (i64.load (i32.const 64)) (i64.const {len(response)})) (then (call $exit (i32.const 203))))'])
            if not response:
                body.append('    (call $buffer_unchanged)')
            elif len(response)==1:
                body.append(f'    (if (i32.ne (i32.load8_u (i32.const 256)) (i32.const {response[0]})) (then (call $exit (i32.const 204))))')
            elif len(response)==4:
                body.append(f'    (if (i32.ne (i32.load (i32.const 256)) (i32.const {signed(int.from_bytes(response,"little"),32)})) (then (call $exit (i32.const 204))))')
            elif len(response)==8:
                body.append(f'    (if (i64.ne (i64.load (i32.const 256)) (i64.const {signed(int.from_bytes(response,"little"),64)})) (then (call $exit (i32.const 204))))')
            else:
                raise ValueError('unexpected scalar response width')
        assertions.append({'operation':'param.'+operation,'alias':alias,'request_hex':request.hex(),
                           'expected_error':error,'expected_response_hex':None if error else response.hex(),
                           'preserves_output_buffer_and_length':bool(error)})

    old = scalar.encode(kind,scalar.VALUES[kind]['old'])
    new = scalar.encode(kind,scalar.VALUES[kind]['new'])
    if scenario=='roundtrip':
        invoke('get','get',response=old); invoke('set','set',new); invoke('get','get',response=new)
    elif scenario=='create':
        invoke('set','set',new); invoke('get','get',response=new)
    elif scenario in ('malformed','range'):
        for payload,error,_ in scalar.malformed(kind) if scenario=='malformed' else scalar.outside(kind):
            invoke('set','set',payload,error)
        invoke('get','get',response=old)
    elif scenario=='get_bounds':
        invoke('get','get',error=13)
    elif scenario=='wrong_kind':
        invoke('get','get',error=95); invoke('set','set',new,95)
    elif scenario in ('operation_denied','dynamic_denied'):
        invoke('set','get' if scenario=='operation_denied' else 'set',new,13 if scenario=='operation_denied' else 95)
        invoke('get','get',response=old)
    elif scenario=='absent_denied':
        invoke('set','set',new,95); invoke('get','get',error=2)
    elif scenario.startswith(('local_','undeclare_')):
        invoke('declare','declare',old); invoke('get','get',response=old)
        if scenario=='local_roundtrip':
            invoke('set','set',new); invoke('get','get',response=new); invoke('undeclare','undeclare')
        else:
            invoke('undeclare','undeclare',error=95 if scenario=='undeclare_wrong_kind' else 13)
            invoke('get','get',response=old)
            if scenario=='undeclare_bounds':
                invoke('set','set',new);invoke('get','get',response=new);invoke('undeclare','undeclare')
            else:invoke('undeclare','cleanup')
        invoke('get','get',error=2)
    else:
        raise ValueError(scenario)
    source += [f'  (data (i32.const {offset}) "{literal(payload)}")' for offset,payload in data]
    source += ['  (func (export "_start")']
    source += [f'    (call $check (call $open (i32.const 0) (i64.const {offset}) (i64.const {len(alias)}) (i64.const {handle})))' for alias,(offset,handle) in aliases.items()]
    source += body+['  ))', '']
    return '\n'.join(source),grants,assertions


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,default=Path(__file__).resolve().parents[5])
    parser.add_argument('--compile',action='store_true')
    parser.add_argument('--wat2wasm',default='wat2wasm')
    args=parser.parse_args()
    for version in (1,2):
        profiles=[]
        for description in scalar.cases(version):
            contents,grants,assertions=guest(version,description)
            path=args.source/f'examples/ros{version}'/(description['name'].removeprefix(f'ros{version}-')+'.wat')
            path.write_text(contents)
            profiles.append({**description,'grants':grants,'guest_assertions':assertions})
            if args.compile:
                subprocess.run([args.wat2wasm,'--enable-all',str(path),'-o',str(path.with_suffix('.wasm'))],check=True,timeout=30)
        (args.source/f'examples/ros{version}/scalar_parameter_profiles.json').write_text(json.dumps(profiles,indent=2)+'\n')
        print(f'WROTE ROS{version} scalar sources/profiles: {len(profiles)}; compiled={args.compile}',flush=True)


if __name__=='__main__':main()
