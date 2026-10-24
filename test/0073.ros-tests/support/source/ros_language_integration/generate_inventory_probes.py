#!/usr/bin/env python3
"""Author inspection-only Go/Zig programs retaining every numeric ROS import.

Compile and fully validate these programs, then require --require-all with the
SDK's binary import checker. Never execute a selected placeholder call. These
probes establish compiler/linker retention and exact signatures, not operations,
permission grants, native delivery or complete client-library compatibility.
"""
import argparse
import hashlib
import json
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    sdk = args.sdk.resolve(strict=True)
    raw = (sdk/'abi/abi-v1.json').read_bytes()
    schema = json.loads(raw)
    args.out.mkdir(parents=True, exist_ok=False)
    products = []
    for backend, count in ((1, 38), (2, 43)):
        functions = schema['modules'][f'uwvm_ros{backend}_v1']['functions']
        if len(functions) != count:
            raise ValueError('complete ABI inventory required')
        go = ['// Inspection only. Never execute a valid placeholder selector.',
              'package main', 'import ("os"; ros "uwvm2.local/roswasmgo")',
              'func main(){ var b [1]byte; if n,e:=os.Stdin.Read(b[:]);e!=nil||n!=1{return};var result uint32;switch b[0]{']
        zig = ['//! Inspection only. Never execute a valid placeholder selector.',
               'const raw=@import("ros");', 'pub fn main() void {}',
               'export fn abi_inventory(index:u32)u32{return switch(index){']
        for index, function in enumerate(functions):
            public = ''.join(word.title() for word in function['name'].split('_'))
            zeros = ','.join('0' for _ in function['params'])
            go.append(f'case {index}:result=ros.{public}({zeros})')
            zig.append(f'{index}=>raw.{function["name"]}({zeros}),')
        go += ['default:return};os.Exit(int(result&255))}']
        zig += ['else=>22,};}']
        root = args.out/('ros'+str(backend))
        root.mkdir()
        (root/'main.go').write_text('\n'.join(go)+'\n')
        (root/'main.zig').write_text('\n'.join(zig)+'\n')
        # Quote the actual absolute replacement as a Go string, preserving spaces.
        (root/'go.mod').write_text('module uwvm2.local/ros-inventory\n\ngo 1.24\n\n'
            'require uwvm2.local/roswasmgo v0.0.0\n\nreplace uwvm2.local/roswasmgo => '+json.dumps(str(sdk/'go'))+'\n')
        for path in sorted(root.iterdir()):
            body = path.read_bytes()
            products.append({'path':str(path.relative_to(args.out)), 'bytes':len(body),
                             'sha256':hashlib.sha256(body).hexdigest()})
    (args.out/'generation.json').write_text(json.dumps({
        'status':'INSPECTION_PROBES_NOT_EXECUTED', 'default_authority':[],
        'schema_sha256':hashlib.sha256(raw).hexdigest(), 'files':products
    }, indent=2)+'\n')


if __name__ == '__main__':
    main()
