#!/usr/bin/env python3
"""Check the ROS native call ABI with real multivalue, tail and reference guests.

The fixtures contain their own value assertions. Trap cases additionally check
retired frames against the actual checked native unwinder. This uses the shared
upstream fixture definitions and intentionally exposes no instruction-stack mode.
"""

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess


def load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--uwvm', type=Path, required=True)
    assembler = parser.add_mutually_exclusive_group(required=True)
    assembler.add_argument('--wat2wasm', type=Path)
    assembler.add_argument('--wasm-tools', type=Path)
    parser.add_argument('--source-root', type=Path)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if args.source_root is None:
        args.source_root = next((p for p in Path(__file__).resolve().parents
                                 if (p / 'test/0014.llvm_jit/run_wasm3_tail_transfer.py').is_file()), None)
        if args.source_root is None:
            parser.error('select --source-root when running outside the VM checkout')
    args.out.mkdir(parents=True, exist_ok=False)
    runtime = args.uwvm.resolve()
    digest = hashlib.sha256(runtime.read_bytes()).hexdigest()
    tail_source = args.source_root / 'test/0014.llvm_jit/run_wasm3_tail_transfer.py'
    ref_source = args.source_root / 'test/0013.uwvm_int/run_call_ref_typed_core3.py'
    tail = load(tail_source, 'tail_cases')
    refs = load(ref_source, 'reference_cases')
    cases = [(name, source, trap[1] if trap else None, trap[2] if trap else None)
             for name, source, trap in tail.fixtures('jit')]
    selected = ('typed-param', 'typed-result', 'typed-local-nullable', 'typed-local-nonnull',
                'typed-local-preinit-survives', 'typed-local-tee', 'typed-global-null',
                'typed-select-call-ref', 'typed-block-multivalue', 'tail-typed-local-250k')
    for name in selected:
        outcome, source = refs.CASES[name]
        assert outcome in ('ok', 'null reference'), (name, outcome)
        cases.append((name, source, None if outcome == 'ok' else outcome, None))
    report = {'passed': False, 'runtime_sha256': digest, 'checks': [], 'fixture_sources': {
        str(p.relative_to(args.source_root)): hashlib.sha256(p.read_bytes()).hexdigest()
        for p in (tail_source, ref_source)}}
    ansi = re.compile(r'\x1b\[[0-9;?]*[ -/]*[@-~]')
    try:
        for name, source, diagnostic, expected_stack in cases:
            wat = args.out / (name + '.wat')
            wasm = wat.with_suffix('.wasm')
            wat.write_text(source + '\n')
            assembly_command = ([str(args.wasm_tools), 'parse', str(wat), '-o', str(wasm)] if args.wasm_tools else
                                [str(args.wat2wasm), '--enable-function-references', str(wat), '-o', str(wasm)])
            assembled = subprocess.run(assembly_command, capture_output=True, timeout=30)
            (args.out / (name + '.assemble.log')).write_bytes(assembled.stdout + assembled.stderr)
            assert assembled.returncode == 0, name
            command = [str(runtime), '-Raot', '-Rllvm-cache-path', 'disable', '-Rllvm-call-stack', 'unwind',
                       '-WFE-tail-call', '-WFE-function-references', '--run', str(wasm)]
            child = subprocess.run(command, capture_output=True, timeout=120)
            text = ansi.sub('', (child.stdout + child.stderr).decode(errors='replace'))
            (args.out / (name + '.log')).write_text(text)
            stack = [int(value) for value in re.findall(r'func_idx=(\d+)', text)]
            passed = child.returncode == 0 if diagnostic is None else child.returncode != 0 and diagnostic in text
            if expected_stack is not None:
                passed = passed and stack == expected_stack
            report['checks'].append({'case': name, 'passed': passed, 'exit': child.returncode,
                                     'stack': stack, 'command': command, 'wasm_sha256': hashlib.sha256(wasm.read_bytes()).hexdigest()})
        assert hashlib.sha256(runtime.read_bytes()).hexdigest() == digest
        report['passed'] = all(row['passed'] for row in report['checks'])
    finally:
        (args.out / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    if not report['passed']:
        raise RuntimeError([row['case'] for row in report['checks'] if not row['passed']])
    print(f"PASS checked native tail ABI: {len(report['checks'])} guests")


if __name__ == '__main__':
    main()
