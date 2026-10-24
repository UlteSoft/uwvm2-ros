#!/usr/bin/env python3
"""Qualify the owned numeric model, JS declarations and genuine Rust formatting.

This small finite-owner workload grants no language build/Guest/ROS credit.
It does not load the application in a host ROS stub or change input sources.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import struct
import subprocess


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'rust-source', 'rustfmt', 'rust-libs', 'inspector', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('common finite Linux owner required')
    args.out.mkdir()
    pins = []
    paths = [Path(__file__), args.inspector, args.rust_source, args.rustfmt,
             Path('/usr/bin/node'), Path('/usr/share/nodejs/acorn/dist/acorn.mjs')]
    paths += sorted(path for path in args.source.rglob('*') if path.is_file())
    paths += sorted(path for path in args.rust_libs.iterdir() if path.is_file())
    for path in paths:
        pins.append(dict(path=str(path), sha256=sha(path), bytes=path.stat().st_size))
    (args.out / 'source-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
    cases = []

    def run(name, command, data=None, environment=None):
        result = subprocess.run(list(map(str, command)), input=data, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, timeout=90, check=False, env=environment)
        (args.out / (name + '.stdout')).write_bytes(result.stdout)
        (args.out / (name + '.stderr')).write_bytes(result.stderr)
        cases.append(dict(name=name, argv=list(map(str, command)), exit=result.returncode,
                          status='PASS' if result.returncode == 0 else 'FAIL',
                          stdout_sha256=hashlib.sha256(result.stdout).hexdigest(),
                          stderr_sha256=hashlib.sha256(result.stderr).hexdigest()))
        if result.returncode != 0:
            raise RuntimeError('actual source qualification failed: ' + name)
        return result.stdout

    run('node-version', ['/usr/bin/node', '--version'])
    formatter_environment = dict(os.environ, LD_LIBRARY_PATH=str(args.rust_libs))
    run('rustfmt-version', [args.rustfmt, '--version'], environment=formatter_environment)
    files = [args.source / 'include/decimal.mjs', args.source / 'include/monitor.mjs',
             args.source / 'example/official/robot_states_monitor.mjs']
    for index, path in enumerate(files):
        run('js-syntax-' + str(index), ['/usr/bin/node', '--check', path])
    run('js-public-api-inventory', ['/usr/bin/node', args.inspector, *files])

    # Both signs and every finite binary64 exponent; the fraction varies by
    # a fixed bit recurrence. Explicit ties/adjacent values/negative zero follow.
    bits = []
    state = 0x8A7D3B9E1542F601
    for exponent in range(2047):
        state = (state * 6364136223846793005 + 1442695040888963407) & ((1 << 64) - 1)
        fraction = state & ((1 << 52) - 1)
        for sign in (0, 1):
            bits.append((sign << 63) | (exponent << 52) | fraction)
    for value in (0.0, -0.0, 0.0625, 0.1875, -0.0625, -0.1875, 1e21, 1e30,
                  1e308, math.ulp(0.0), float.fromhex('0x1.fffffffffffffp+1023')):
        for candidate in (value, math.nextafter(value, math.inf), math.nextafter(value, -math.inf)):
            if math.isfinite(candidate):
                bits.append(struct.unpack('<Q', struct.pack('<d', candidate))[0])
    vectors = [dict(bits=f'{word:016x}', expected=format(struct.unpack('<d', struct.pack('<Q', word))[0], '.3f'))
               for word in bits]
    payload = json.dumps(vectors, separators=(',', ':')).encode()
    (args.out / 'numeric-oracles.json').write_bytes(payload)
    program = '''import {readFileSync} from 'node:fs';
import {pathToFileURL} from 'node:url';
const {fixedThree}=await import(pathToFileURL(process.argv[1]).href);
const vectors=JSON.parse(readFileSync(0,'utf8'));
let tested=0;
for (const vector of vectors) {
  const buffer=new ArrayBuffer(8); const view=new DataView(buffer);
  view.setBigUint64(0,BigInt('0x'+vector.bits),true);
  const actual=fixedThree(view.getFloat64(0,true));
  if (actual!==vector.expected) throw new Error(JSON.stringify({vector,actual}));
  tested++;
}
for (const invalid of [NaN,Infinity,-Infinity,'1',null,undefined]) {
  let code; try {fixedThree(invalid);} catch(error) {code=error.code;}
  if (code!==22) throw new Error('nonfinite/type rejection was not22');
}
process.stdout.write(JSON.stringify({status:'PASS',finite_cases:tested,invalid_cases:6,
scope:'HOST_NUMERIC_MODEL_ONLY',wasm_builds:0,guest:0,native_ros:0})+'\\n');'''
    run('numeric-model', ['/usr/bin/node', '--input-type=module', '-e', program, files[0]], payload)

    target = args.out / 'main.rs'
    target.write_bytes(args.rust_source.read_bytes())
    command = [args.rustfmt, '--edition', '2021', '--config', 'skip_children=true', target]
    run('rust-format-first', command, environment=formatter_environment)
    formatted = target.read_bytes()
    run('rust-format-second', command, environment=formatter_environment)
    if formatted != target.read_bytes():
        raise RuntimeError('genuine Rust formatter is not byte idempotent')
    for pin in pins:
        if sha(Path(pin['path'])) != pin['sha256']:
            raise RuntimeError('immutable source changed during qualification')
    (args.out / 'cases.json').write_text(json.dumps(cases, indent=2) + '\n')
    summary = dict(status='PASS', numeric_model_finite_cases=len(vectors), invalid_cases=6,
                   js_exported_function_inventory=4, js_documented_typedefs=1,
                   rust_source_format_idempotent=True, rust_formatted_sha256=sha(target),
                   scope='HOST_NUMERIC_MODEL_AND_SOURCE_QUALIFICATION_ONLY',
                   wasm_builds=0, guest=0, native_ros=0)
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
