"""Verify default ABI discovery, per-caller visibility and independent self reporting."""
import argparse
import json
from pathlib import Path
import subprocess

from run_module_security_vm import Fixture, record
from run_scene_all_import_denials import import_text
from run_scene_vm import GATE, ROOT, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    out = args.output.resolve()
    if '.tmp' not in out.parts:
        parser.error('use a project .tmp directory for fixtures')
    out.mkdir(parents=True, exist_ok=False)
    vm = args.runtime.resolve()
    cases = []
    bound = ['--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', 'main']

    def prepare(name, wat):
        source = out / (name + '.wat')
        binary = source.with_suffix('.wasm')
        source.write_text(wat)
        subprocess.run(['wat2wasm', '--enable-all', source, '-o', binary], check=True)
        return binary

    def run(name, wasm, flags, hidden=False):
        for engine in ('int', 'aot'):
            argv = [vm, '--runtime-' + engine, '--runtime-compile-threads', '0',
                    '--runtime-llvm-jit-cache-path', 'disable', '--wasm-set-main-module-name', 'main',
                    *flags, '--run', wasm]
            result = subprocess.run(argv, capture_output=True, timeout=45)
            ok = ((result.returncode > 0 and b'ROS scene interface is hidden' in result.stderr)
                  if hidden else result.returncode == 0)
            cases.append(dict(case=name, engine=engine, passed=ok, code=result.returncode,
                              wasm_sha256=sha(wasm)))
            if not ok:
                raise AssertionError((name, engine, result.returncode, result.stderr[-2000:]))

    # Instantiate one module per declaration so an earlier hidden dependency
    # cannot mask a later default grant. Compare the full registries first.
    policy = subprocess.run([vm, '--ros-scene-dump-effective-policy'], capture_output=True, check=True)
    names = json.loads(policy.stdout)['registeredOperations']
    catalogue = json.loads((ROOT / 'tools/ros_scene/interface-catalogue.json').read_bytes())
    registered = [row for row in catalogue['operations'] if row['implementation'] == 'registered_local']
    assert sorted(row['name'] for row in registered) == names
    for row in registered:
        name = row['name']
        wasm = prepare('import-' + name, '(module ' + import_text(row) + ' (func (export "_start")))')
        run('bound-default-' + name, wasm, bound, hidden=name not in ('abi', 'permission.report'))

    for width in (32, 64):
        for index in (0, 1):
            for exposure, expected, mode in [('default', 2, 2), ('deny', 13, 1)]:
                fixture = Fixture(['permission.report'], width, index)
                fixture.extra_imports.append('(import "uwvm_ros_v2" "abi" (func $abi (result i32)))')
                query = record('rosv2', 'abi', 0)
                fixture.request('permission.report', query)
                # The row encodes three length-prefixed strings followed by its value.
                row_value = 16384 + 40 + len(record('interface', 'abi', ''))
                fixture.equal(fixture.word(row_value), mode)
                fixture.check('call $abi', expected)
                fixture.request('permission.report', query)
                fixture.equal(fixture.word(row_value), mode)
                name = f'memory{width}-{index}-{exposure}'
                wasm = prepare(name, fixture.render())
                flags = bound.copy()
                if exposure == 'deny':
                    flags.extend(['--ros-scene-interface', 'robot', 'abi', 'deny'])
                run(name, wasm, flags)
            hidden = Fixture(['permission.report'], width, index)
            hidden.request('permission.report', record('rosv2', 'abi', 0))
            hidden.equal(hidden.word(row_value), 0)
            name = f'memory{width}-{index}-abi-hidden-report-readable'
            run(name, prepare(name, hidden.render()), [*bound, '--ros-scene-interface', 'robot', 'abi', 'hidden'])

    version = prepare('version', '(module (import "uwvm_ros_v2" "abi" (func $abi (result i32)))'
                      ' (func (export "_start") call $abi i32.const 2 i32.ne if unreachable end))')
    run('unbound', version, [], hidden=True)
    run('explicitly-hidden', version, [*bound, '--ros-scene-interface', 'robot', 'abi', 'hidden'], hidden=True)
    for mode in ('deny', 'hidden'):
        run('independent-report-' + mode, version, [*bound, '--ros-scene-interface', 'robot', 'permission.report', mode])

    library = prepare('library', '(module (import "uwvm_ros_v2" "abi" (func $abi (result i32)))'
                      ' (func (export "version") (result i32) call $abi))')
    for lib_mode, expected in [('default', 2), ('deny', 13)]:
        caller = prepare('caller-' + lib_mode, '(module (import "lib" "version" (func $version (result i32)))'
                         f' (func (export "_start") call $version i32.const {expected} i32.ne if unreachable end))')
        flags = [*bound, '--ros-scene-interface', 'robot', 'abi', 'hidden',
                 '--ros-scene-create', 'library', '--ros-scene-bind', 'library', 'lib']
        if lib_mode == 'deny':
            flags.extend(['--ros-scene-interface', 'library', 'abi', 'deny'])
        run('library-' + lib_mode, caller, [*flags, '--wasm-preload-library', library, 'lib'])

    verify_gate()
    (out / 'results.json').write_text(json.dumps(dict(complete=True, cgroup=str(GATE),
        vm_sha256=sha(vm), cases=cases), indent=2) + '\n')
    print(f'{len(cases)} real VM cases passed')


if __name__ == '__main__':
    main()
