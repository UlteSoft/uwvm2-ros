#!/usr/bin/env python3
"""Exercise every registered ROSv2 import's explicit-deny and hidden boundary.

Run only through tools/ros_scene/in_existing_cgroup.py. The catalogue supplies
signatures, not expected runtime results: every explicitly denied call must
return EACCES before interpreting either ordinary or poisoned carrier fields.
Guest assertions check request, result and result-length canaries after each
call, in both memories when a nonzero memory index is selected. A separately
authorized main module also invokes the same denied code through a preload;
its authority must not replace the immediate caller's identity.

This tests registration, call admission and nonpublication. It does not qualify
successful provider semantics, physical effects or industrial certification.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import sys
import time

from run_scene_vm import GATE, ROOT, sha, verify_gate


SPECIAL = {
    'abi': [],
    'capability.open': ['i32', 'i64', 'i64', 'i64'],
    'capability.close': ['i64'],
}
CARRIER = ['i64', 'i32', 'i64', 'i64', 'i64', 'i64', 'i64']
CANARIES = ((128, 256, 0x3c), (4096, 512, 0xa5), (8192, 8, 0xc6))


def import_text(row):
    signature = row['wasm_signature']
    parameters = signature['parameters']
    assert parameters == SPECIAL.get(row['name'], CARRIER), row['name']
    assert signature['results'] == ['i32'], row['name']
    params = (' (param ' + ' '.join(parameters) + ')') if parameters else ''
    return f'(import "uwvm_ros_v2" "{row["name"]}" (func ${row["name"]}{params} (result i32)))'


def denied_guest(rows, width, memory, export):
    """Assert the closed denial contract independently for each import."""
    definitions = [import_text(row) for row in rows]
    for index in range(memory + 1):
        bits = width if index == memory else 32
        definitions.append(f'(memory $m{index} ' + ('i64 ' if bits == 64 else '') + '1)')
        for address, size, value in CANARIES:
            definitions.append(f'(data (memory $m{index}) (i{bits}.const {address}) "' + f'\\{value:02x}' * size + '")')
        cast = 'i64.extend_i32_u' if bits == 64 else ''
        definitions.append(f'''(func $check_memory_{index}
          (param $start i32) (param $count i32) (param $expected i32)
          (local $offset i32)
          loop $next
            local.get $start local.get $offset i32.add {cast}
            i32.load8_u $m{index} local.get $expected i32.ne if unreachable end
            local.get $offset i32.const 1 i32.add local.tee $offset
            local.get $count i32.lt_u br_if $next
          end)''')
    checks = '\n'.join(f'i32.const {address} i32.const {size} i32.const {value} call $check_memory_{index}'
                       for index in range(memory + 1) for address, size, value in CANARIES)
    body = []
    for row in rows:
        name = row['name']
        if name == 'abi':
            vectors = [('ordinary', [])]
        elif name == 'capability.open':
            vectors = [('ordinary', [memory, 128, 16, 4096]), ('poisoned', [-1] * 4)]
        elif name == 'capability.close':
            vectors = [('ordinary', [1]), ('poisoned', [-1])]
        else:
            vectors = [('ordinary', [1, memory, 128, 16, 4096, 512, 8192]), ('poisoned', [-1] * 7)]
        for variant, values in vectors:
            parameters = row['wasm_signature']['parameters']
            expression = ' '.join(f'{kind}.const {value}' for kind, value in zip(parameters, values, strict=True))
            label = f'assert_{name}_{variant}'
            definitions.append(f'(func ${label} {expression} call ${name} i32.const 13 i32.ne if unreachable end\n{checks})')
            body.append('call $' + label)
    return '(module\n' + '\n'.join(definitions) + f'\n(func (export "{export}")\n' + '\n'.join(body) + '\n))\n'


def exposure(rows, state, subject):
    flags = ['--ros-scene-create', 'denied-group', '--ros-scene-bind', 'denied-group', subject]
    for row in rows:
        flags += ['--ros-scene-interface', 'denied-group', row['name'], state]
    return flags


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--engines', nargs='+', choices=['int', 'aot'], default=['int', 'aot'])
    args = parser.parse_args()
    verify_gate()
    runtime = args.runtime.resolve()
    build_path = runtime.parent / 'full-build-results.json'
    build = json.loads(build_path.read_text())
    assert build['complete'] and build['binary_sha256'] == sha(runtime)
    source = Path(build['source_root'])
    manifest_path = source / 'source-manifest.json'
    manifest = json.loads(manifest_path.read_text())
    catalogue = ROOT / 'tools/ros_scene/interface-catalogue.json'
    data = json.loads(catalogue.read_text())
    rows = [row for row in data['operations'] if row['implementation'] == 'registered_local']
    assert rows and len({row['name'] for row in rows}) == len(rows)
    for row in rows:
        import_text(row)
    inputs = [Path(__file__).resolve(), Path(__file__).with_name('run_scene_vm.py'), catalogue,
              runtime, build_path, manifest_path]
    # Pin the contracts compiled into this cohort, independently of later
    # changes in the shared checkout. Actual imports verify catalogue signatures.
    for relative in data['implementation_authority']:
        assert manifest[relative] == sha(source / relative), relative
        inputs.append(source / relative)
    pins = {str(path): sha(path) for path in inputs}
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent / 'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    for path in inputs:
        if path.is_relative_to(ROOT):
            retained = out / 'retained-inputs' / path.relative_to(ROOT)
            retained.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(path, retained)
    report = dict(complete=False, scope=__doc__, cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino,
                  memory_max=68719476736, swap_max=0, runtime_sha256=pins[str(runtime)],
                  boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                  inputs=pins, operations=[row['name'] for row in rows], invocation=sys.argv,
                  memory_events_before=(GATE / 'memory.events').read_text(), cases=[], compiles=[])

    def save():
        (out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')

    def compile_guest(name, wat):
        verify_gate()
        text, binary = out / (name + '.wat'), out / (name + '.wasm')
        text.write_text(wat)
        argv = ['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', str(text), '-o', str(binary)]
        result = subprocess.run(argv, capture_output=True, timeout=120)
        report['compiles'].append(dict(name=name, argv=argv, returncode=result.returncode))
        save()
        assert result.returncode == 0, result.stderr.decode(errors='replace')
        return binary

    def run(name, flags, binary, hidden=False):
        verify_gate()
        argv = [str(runtime), '--wasm-set-main-module-name', 'main', *flags, '--run', str(binary)]
        start = time.monotonic()
        result = subprocess.run(argv, capture_output=True, timeout=120)
        (out / (name + '.stdout')).write_bytes(result.stdout)
        (out / (name + '.stderr')).write_bytes(result.stderr)
        passed = (result.returncode != 0 and b'scene interface is hidden' in result.stderr) if hidden else result.returncode == 0
        report['cases'].append(dict(name=name, argv=argv, returncode=result.returncode, passed=passed,
                                  seconds=time.monotonic() - start, guest_sha256=sha(binary)))
        save()
        assert passed, (name, result.returncode, result.stderr.decode(errors='replace')[-4000:])
        print('PASS ' + name, flush=True)

    success = False
    try:
        parent = compile_guest('authorized-parent', '''(module
          (import "uwvm_ros_v2" "abi" (func $abi (result i32)))
          (import "denied-worker" "verify_denied" (func $verify))
          (func (export "_start") call $abi i32.const 2 i32.ne if unreachable end
            call $verify call $abi i32.const 2 i32.ne if unreachable end))''')
        parent_policy = ['--ros-scene-create', 'parent', '--ros-scene-bind', 'parent', 'main',
                         '--ros-scene-interface', 'parent', 'abi', 'allow']
        for width, memory in ((32, 0), (64, 0), (32, 1), (64, 1)):
            for preload in (False, True):
                label = f'{"preload" if preload else "main"}-w{width}-m{memory}'
                guest = compile_guest(label, denied_guest(rows, width, memory, 'verify_denied' if preload else '_start'))
                subject = 'denied-worker' if preload else 'main'
                flags = exposure(rows, 'deny', subject)
                # Two members share the denied group; neither group membership
                # nor the parent's separate allow grant changes this decision.
                if preload:
                    flags += ['--ros-scene-bind', 'denied-group', 'same-group-peer', *parent_policy,
                              '--wasm-preload-library', str(guest), subject]
                for engine in args.engines:
                    run(label + '-' + engine, ['--runtime-' + engine, *flags], parent if preload else guest)
        # Import each name separately: one early hidden import must not mask a
        # later incorrectly exposed entry. Hidden imports are never invoked.
        for row in rows:
            guest = compile_guest('hidden-' + row['name'], '(module\n' + import_text(row) + '\n(func (export "_start")))\n')
            for engine in args.engines:
                run('hidden-' + row['name'] + '-' + engine,
                    ['--runtime-' + engine, *exposure([row], 'hidden', 'main')], guest, hidden=True)
        success = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(Path(path)) == digest for path, digest in pins.items())
        report['memory_events_after'] = (GATE / 'memory.events').read_text()
        report['complete'] = success and report['inputs_unchanged']
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
