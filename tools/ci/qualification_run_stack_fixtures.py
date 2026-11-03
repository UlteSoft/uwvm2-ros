#!/usr/bin/env python3
"""Build three migrated consumers of a successful paired native VM, then run real oracles.

Never configures Xmake, rebuilds a dependency, edits the product tree, or infers
native correctness from source parsing. Run only through the centralized Linux
keeper after the product build. Test sources live in a separate frozen packet.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import sys
from qualification_stack_support import guard, run, save, sha, verify_manifest

RUNTIME = 'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp'
MAIN = 'src/uwvm2/uwvm/main.default.cpp'
FIXTURES = {
    'llvm_debug_observer': {
        'wat': '''(module
 (import "observer-host" "marker" (func $marker (result i32)))
 (tag $e (param i32))
 (func $warm call $marker drop)
 (func $leaf (result i32) i32.const 73 throw $e)
 (func $work (result i32)
  (block $caught (result i32)
   (try_table (catch $e $caught) call $leaf drop) unreachable)
  i32.const 1 i32.add))\n''',
        'runs': [([], ('PASS LLVM full observer:', 'mandatory-native'))]},
    'llvm_debug_lifecycle': {
        'wat': '''(module
 (import "lifecycle-host" "old_tick" (func $old_tick (result i32)))
 (import "lifecycle-host" "new_entry" (func $new_entry (result i32)))
 (import "lifecycle-host" "reenter" (func $reenter (result i32)))
 (import "lifecycle-host" "nested_tick" (func $nested_tick (result i32)))
 (tag $e (param i32))
 (func $warm)
 (func $old (result i32) (loop $again call $old_tick i32.eqz br_if $again) i32.const 41)
 (func $new (result i32) call $new_entry)
 (func $thrower (result i32) i32.const 73 throw $e)
 (func $nested (result i32)
  (block $caught (result i32)
   (try_table (catch $e $caught)
    (loop $again call $nested_tick i32.eqz br_if $again)
    call $thrower drop) unreachable))
 (func $outer (result i32) (local i32)
  call $reenter local.set 0
  (block $caught (result i32)
   (try_table (catch $e $caught) call $thrower drop) unreachable)
  local.get 0 i32.add))\n''',
        'runs': [(['reset'], ('PASS paused new entry:', 'results 41/43')),
                 (['reentry'], ('PASS callback raw reentry:', 'results 146'))]},
    'native_step_llvm_full': {
        'wat': '(module (func (result i32) i32.const 40 i32.const 2 i32.add))\n',
        'runs': [([], ('PASS actual LLVM full native step:', 'two precise SIGTRAP stops', 'mandatory-native'))]},
}


def absolute(source: Path, token: str) -> Path:
    path = Path(token)
    return (path if path.is_absolute() else source / path).resolve(strict=True)


def expand(argv: list[str], source: Path, pins: dict, depth=0) -> list[str]:
    if depth > 4:
        raise RuntimeError('nested response file limit')
    result = []
    for token in argv:
        if token.startswith('@'):
            path = absolute(source, token[1:])
            if path.stat().st_size > 4 * 1024 * 1024:
                raise RuntimeError('response file too large')
            pins[str(path)] = sha(path)
            result.extend(expand(shlex.split(path.read_text()), source, pins, depth + 1))
        else:
            result.append(token)
    return result


def recipes(log: Path, source: Path, product: Path, pins: dict) -> tuple[list[str], list[str], str]:
    candidates = {'runtime': {}, 'main': {}, 'link': {}}
    for line in log.read_text(errors='strict').splitlines():
        line = re.sub(r'\x1b\[[0-?]*[ -/]*[@-~]', '', line).strip()
        try:
            tokens = shlex.split(line)
        except ValueError:
            continue
        if not tokens or Path(tokens[0]).name not in ('clang++', 'clang'):
            continue
        tokens = expand(tokens, source, pins)
        if tokens.count('-o') != 1 or tokens.index('-o') + 1 == len(tokens):
            continue
        if any(token == RUNTIME or token.endswith('/' + RUNTIME) for token in tokens) and '-c' in tokens:
            candidates['runtime'][tuple(tokens)] = tokens
        if any(token == MAIN or token.endswith('/' + MAIN) for token in tokens) and '-c' in tokens:
            candidates['main'][tuple(tokens)] = tokens
        output = Path(tokens[tokens.index('-o') + 1])
        if '-c' not in tokens and (output if output.is_absolute() else source / output).resolve() == product:
            candidates['link'][tuple(tokens)] = tokens
    if any(len(candidates[key]) != 1 for key in ('runtime', 'link')):
        raise RuntimeError('require unique actual runtime/link commands from the successful verbose build log')
    compile_cmd, link_cmd = (next(iter(candidates[key].values())) for key in ('runtime', 'link'))
    if len(candidates['main']) == 1:
        main_cmd = next(iter(candidates['main'].values()))
        main_object = str(absolute(source, main_cmd[main_cmd.index('-o') + 1]))
    else:
        # A source-identical main object may be cached. Its unique actual link
        # input is still pinned by the successful product receipt and its hash.
        matches = [token for token in link_cmd if token.endswith('/' + MAIN + '.o')]
        if len(matches) != 1:
            raise RuntimeError('cannot identify the one actual main object')
        main_object = str(absolute(source, matches[0]))
    return compile_cmd, link_cmd, main_object


def consumer_compile(command: list[str], fixture: Path, obj: Path) -> list[str]:
    result = []
    index = 0
    while index < len(command):
        token = command[index]
        if token in ('-MF', '-MT', '-MQ', '-MJ'):
            index += 2
            continue
        if token in ('-MMD', '-MD', '-MP') or token.startswith(('-ftime-trace', '-fprofile-', '-fmodules-cache-path')):
            index += 1
            continue
        if token == '-o':
            result.extend(('-o', str(obj)))
            index += 2
            continue
        result.append(str(fixture) if token == RUNTIME or token.endswith('/' + RUNTIME) else token)
        index += 1
    for define in ('UWVM2TEST_RUNNER_USE_LLVM_JIT=1', 'UWVM2TEST_STRICT_NO_INTERPRETER=1'):
        if '-D' + define not in result:
            result.append('-D' + define)
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True, help='untouched successful product source tree')
    parser.add_argument('--tests-root', type=Path, required=True, help='separately extracted full migration tests archive')
    parser.add_argument('--manifest', type=Path, required=True)
    parser.add_argument('--manifest-sha256', required=True)
    parser.add_argument('--source-manifest', type=Path, required=True, help='parent r13 full product manifest')
    parser.add_argument('--source-identity', required=True)
    parser.add_argument('--product-receipt', type=Path, required=True, help='parent successful build inside.json')
    parser.add_argument('--product', type=Path, required=True)
    parser.add_argument('--build-log', type=Path, required=True, help='successful verbose build stdout containing actual recipes')
    parser.add_argument('--toolchain-root', type=Path, required=True, help='same frozen portable LLVM 23 closure used by the parent native build')
    parser.add_argument('--wasm-tools', type=Path, required=True)
    parser.add_argument('--lease', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    admission = guard(args.lease)
    args.out.mkdir(parents=True, exist_ok=False)
    source = args.source.resolve(strict=True)
    tests = args.tests_root.resolve(strict=True)
    out = args.out.resolve(strict=True)
    if source == tests or source in out.parents or out == source or tests in source.parents:
        raise RuntimeError('consumer outputs and migration sources must be separate from the product tree')
    report = {'implementation': 'SOURCE_AUTHORED', 'fixture_build': 'UNTESTED',
              'wasm_validation': 'UNTESTED', 'wasm_execution': 'UNTESTED',
              'native_ros_endpoint': 'UNTESTED', 'qemu': 'UNTESTED', 'performance': 'UNTESTED',
              'scope': 'three independent migrated native consumers; four real debugger/EH/lifecycle/PC executions; no full regression claim',
              'admission': admission, 'builds': [], 'executions': []}
    pins = {}
    try:
        verify_manifest(tests, args.manifest, args.manifest_sha256)
        receipt = json.loads(args.product_receipt.read_text())
        product = args.product.resolve(strict=True)
        if (receipt.get('build_passed') is not True or receipt.get('source', {}).get('source_identity') != args.source_identity
                or Path(receipt['runtime']['path']).resolve(strict=True) != product or sha(product) != receipt['runtime']['sha256']):
            raise RuntimeError('new successful source-bound paired product receipt is required')
        manifest = json.loads(args.source_manifest.read_text())
        identity = 'sha256:' + hashlib.sha256(json.dumps(manifest, sort_keys=True, separators=(',', ':')).encode()).hexdigest()
        if identity != args.source_identity:
            raise RuntimeError('parent product source identity differs')
        for row in manifest:
            if sha(source / row['path']) != row['sha256']:
                raise RuntimeError('parent product source changed: ' + row['path'])
        for path in (product, args.product_receipt, args.build_log, args.wasm_tools, args.source_manifest):
            pins[str(path.resolve(strict=True))] = sha(path)
        compile_cmd, link_cmd, main_object = recipes(args.build_log, source, product, pins)
        toolchain = args.toolchain_root.resolve(strict=True)
        if not (toolchain / 'bin/clang++').is_file() or not (toolchain / 'lib/clang/23/include').is_dir():
            raise RuntimeError('frozen LLVM 23 compiler/resource closure is required')
        # The actual recorded parent wrapper requires this task-specific variable.
        os.environ['UWVM_PARENT_LLVM23_ROOT'] = str(toolchain)
        os.environ['PATH'] = str(absolute(source, compile_cmd[0]).parent) + ':' + str(toolchain / 'bin') + ':' + os.environ.get('PATH', '')
        for path in toolchain.rglob('*'):
            if path.is_file():
                pins[str(path)] = sha(path)
        run([compile_cmd[0], '--version'], source, out / 'compiler-version.log', 30, args.lease)
        if not re.search(r'clang version 23(?:\.|\s)', (out / 'compiler-version.log').read_text()):
            raise RuntimeError('actual frozen consumer compiler is not LLVM 23')
        if (not any('UWVM_ROS_PAIRED_RUNTIME_HEADERS=1' in token for token in compile_cmd)
                or not any(args.source_identity in token for token in compile_cmd)
                or any(token == '-fno-exceptions' for token in compile_cmd)):
            raise RuntimeError('captured compile command does not retain the exact paired ROS EH/source contract')
        if any(token.startswith(('-Wl,-Map', '-Wl,--Map', '--out-implib', '-Wl,--out-implib')) for token in link_cmd):
            raise RuntimeError('captured link command has a side-output option; require a reviewed isolated consumer recipe')
        object_tokens = [token for token in link_cmd if token.endswith('.o') and not token.startswith('-')]
        objects = {token: absolute(source, token) for token in object_tokens}
        if sum(str(path) == main_object for path in objects.values()) != 1:
            raise RuntimeError('cannot uniquely exclude the actual product main object')
        archives = [absolute(source, token) for token in link_cmd if token.endswith('.a') and not token.startswith('-')]
        if not {'libc++.a', 'libc++abi.a', 'libunwind.a'} <= {path.name for path in archives}:
            raise RuntimeError('actual link command lacks the three paired runtime archives')
        for path in (*objects.values(), *archives, absolute(source, compile_cmd[0]), absolute(source, link_cmd[0])):
            pins[str(path)] = sha(path)
        save(out / 'immutable-inputs-before.json', pins)
        # Require actual checked generated-chain admission from this new product.
        admission_wat = out / 'admission.wat'
        admission_wat.write_text('(module (func $leaf) (func (export "_start") call $leaf))\n')
        admission_wasm = admission_wat.with_suffix('.wasm')
        run([str(args.wasm_tools), 'parse', str(admission_wat), '-o', str(admission_wasm)], source, out / 'admission.parse.log', 60, args.lease)
        run([str(args.wasm_tools), 'validate', str(admission_wasm)], source, out / 'admission.validate.log', 60, args.lease)
        run([str(product), '-Raot', '-Rct', '0', '-Rllvm-cache-path', 'disable', '--log-verbose',
             '--runtime-compiler-log', 'err', '--run', str(admission_wasm)], source, out / 'admission.run.log', 90, args.lease)
        if '[unwind-check] status=ok' not in (out / 'admission.run.log').read_text(errors='replace'):
            raise RuntimeError('new product did not prove live checked generated-chain native unwind')
        report['product_native_admission'] = 'PASS'
        for name, fixture in FIXTURES.items():
            folder = out / name
            folder.mkdir()
            cc = tests / 'test/0017.runtime' / (name + '.cc')
            obj, binary = folder / 'fixture.o', folder / 'fixture'
            report['builds'].append(run(consumer_compile(compile_cmd, cc, obj), source, folder / 'compile.log', 1200, args.lease))
            link = []
            index = 0
            while index < len(link_cmd):
                token = link_cmd[index]
                if token == '-o':
                    link.extend(('-o', str(binary)))
                    index += 2
                    continue
                if token in objects and str(objects[token]) == main_object:
                    link.append(str(obj))
                else:
                    link.append(token)
                index += 1
            report['builds'].append(run(link, source, folder / 'link.log', 300, args.lease))
            wat, wasm = folder / 'fixture.wat', folder / 'fixture.wasm'
            wat.write_text(fixture['wat'])
            run([str(args.wasm_tools), 'parse', str(wat), '-o', str(wasm)], source, folder / 'parse.log', 60, args.lease)
            run([str(args.wasm_tools), 'validate', str(wasm)], source, folder / 'validate.log', 60, args.lease)
            for number, (arguments, markers) in enumerate(fixture['runs']):
                row = run([str(binary), str(wasm), *arguments], source, folder / f'run-{number}.log', 120, args.lease)
                text = (folder / f'run-{number}.log').read_text(errors='replace')
                if any(marker not in text for marker in markers) or 'FAIL' in text or 'SKIP' in text or 'BLOCKED' in text:
                    raise RuntimeError('actual enabled semantic oracle missing: ' + name)
                row.update(fixture=name, semantic_oracles=list(markers), source_sha256=sha(cc),
                           executable_sha256=sha(binary), wasm_sha256=sha(wasm), status='PASS')
                report['executions'].append(row)
            # The migrated fixture itself must reject legacy positional selectors.
            for token in ('instruction', 'unwind', 'unwind-uncheck', '-Rllvm-call-stack'):
                run([str(binary), str(wasm), token], source, folder / ('reject-' + token.lstrip('-') + '.log'), 30, args.lease, expected=64)
            save(out / 'fixture-results.json', report)
        if len(report['executions']) != 4:
            raise RuntimeError('focused native execution matrix incomplete')
        if any(sha(Path(path)) != expected for path, expected in pins.items()):
            raise RuntimeError('product/object/archive/compiler/receipt inputs changed')
        verify_manifest(tests, args.manifest, args.manifest_sha256)
        report.update(focused_status='PASS', fixture_build='PASS', wasm_validation='PASS', wasm_execution='PASS',
                      actual_native_component_executions=4, untouched_product_sha256=sha(product))
    except BaseException as error:
        report['error'] = repr(error)
        report['focused_status'] = 'FAIL'
        raise
    finally:
        report['final_admission'] = guard(args.lease)
        save(out / 'fixture-results.json', report)


if __name__ == '__main__':
    main()
