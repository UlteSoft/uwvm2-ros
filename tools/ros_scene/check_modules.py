#!/usr/bin/env python3
"""Compile real scene modules and link mixed named/header hook identity checks.

Run through in_existing_cgroup.py. --compile-argv and --link-argv are JSON
argument arrays containing the compiler and flags only (no source or output).
--graph additionally compiles the real frontend/host/loader dependency graph;
the focused default does not claim a complete VM module build or runtime link.
--isolated-host checks the new host exports with textual existing infrastructure,
and must use production ROS feature flags. It cannot qualify the full graph.
Use --host-fp-mode llvm and interpreter in separate --output directories to
verify both callback-policy contracts, with every PCM built for that mode.
Completion requires the tracked scene sources and this driver to stay unchanged
through the final check; changed or unreadable inputs invalidate the receipt.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess


ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compile-argv', type=Path, required=True)
    parser.add_argument('--link-argv', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Frozen production headers/module graph; qualification fixtures remain in checkout')
    parser.add_argument('--graph', action='store_true')
    parser.add_argument('--isolated-host', action='store_true')
    parser.add_argument('--host-fp-mode', choices=('llvm', 'interpreter'),
                        help='Explicit isolated-host backend policy; use a distinct output directory for each mode')
    parser.add_argument('--root', action='append', default=[], help='Additional exact named-module graph root')
    args = parser.parse_args()
    production = args.source_root.resolve(); assert (production/"src").is_dir()
    assert not (args.graph and args.isolated_host), 'Keep isolated fixtures separate from production graph evidence'
    assert not args.host_fp_mode or args.isolated_host, '--host-fp-mode requires --isolated-host'
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736'
    assert (GROUP/'memory.swap.max').read_text().strip() == '0'
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    compile_flags = json.loads(args.compile_argv.read_text())
    link_flags = json.loads(args.link_argv.read_text())
    compile_flags = [item for item in compile_flags if not item.startswith('-fprebuilt-module-path=')]
    # Qualification fixtures perform their setup and checks in assert(). A
    # production release recipe must never silently compile those checks out.
    compile_flags += ['-UNDEBUG']
    for prefix in ('src', 'third-parties/fast_io'):
        compile_flags = [item.replace(str(ROOT/prefix), str(production/prefix)) for item in compile_flags]
    compile_flags = [('-I'+str(production/'src')) if item=='-Isrc' else
                     ('-I'+str(production/'third-parties/fast_io/include')) if item=='-Ithird-parties/fast_io/include' else item for item in compile_flags]
    if args.host_fp_mode:
        # push_macros.h recomputes the runtime marker from these selectors, so
        # merely undefining UWVM_RUNTIME_LLVM_JIT would not disable LLVM.
        compile_flags += ['-UUWVM_RUNTIME_LLVM_JIT', '-UUWVM_USE_DEFAULT_JIT', '-UUWVM_USE_LLVM_JIT',
                          '-UUWVM_DISABLE_JIT', '-UUWVM_SCENE_TEST_EXPECT_LLVM']
        compile_flags += (['-DUWVM_USE_LLVM_JIT', '-DUWVM_SCENE_TEST_EXPECT_LLVM=1']
                          if args.host_fp_mode == 'llvm' else
                          ['-DUWVM_DISABLE_JIT', '-DUWVM_SCENE_TEST_EXPECT_LLVM=0'])
    compile_flags += ['-fprebuilt-module-path=' + str(output)]
    result = {'complete': False, 'full_graph_requested': args.graph, 'isolated_host_export': args.isolated_host,
              'host_fp_mode': args.host_fp_mode, 'production_source_root': str(production), 'fixture_source_root': str(ROOT),
              'cgroup_inode': GROUP.stat().st_ino,
              'commands': [], 'modules': {},
              'cgroup': str(GROUP), 'memory_max': 68719476736, 'memory_swap_max': 0,
              'events_before': (GROUP/'memory.events').read_text()}
    tracked = [*production.glob('src/uwvm2/imported/ros/scene/*'), *production.glob('src/uwvm2/imported/ros/runtime/*'),
               *production.glob('src/uwvm2/uwvm/imported/scene/*'), *ROOT.glob('test/0084.ros-runtime/module_*.cpp'),
               ROOT/'test/0084.ros-runtime/host_fp_policy.cpp', ROOT/'test/0084.ros-runtime/host_module_export.cppm',
               Path(__file__).resolve()]
    hashes = {str(path): hashlib.sha256(path.read_bytes()).hexdigest() for path in tracked if path.is_file()}
    for path in (args.compile_argv.resolve(), args.link_argv.resolve()):hashes[str(path)]=hashlib.sha256(path.read_bytes()).hexdigest()
    manifest=production/'source-manifest.json'
    if manifest.is_file():
        hashes[str(manifest)]=hashlib.sha256(manifest.read_bytes()).hexdigest()
        for name,digest in json.loads(manifest.read_text()).items():
            path=production/name;assert hashlib.sha256(path.read_bytes()).hexdigest()==digest,name
            hashes[str(path)]=digest
        result['full_source_manifest']=str(manifest)
        result['full_source_manifest_sha256']=hashes[str(manifest)]

    def save():
        (output/'results.json').write_text(json.dumps(result, indent=2) + '\n')

    def command(label, argv, expected_failure=None):
        log = output/(label + '.log')
        (output/(label + '.argv.json')).write_text(json.dumps(argv, indent=2) + '\n')
        process = subprocess.run(argv, cwd=ROOT, capture_output=True, text=True, timeout=300)
        log.write_text(process.stderr if label.endswith('.dependencies') else process.stdout + process.stderr)
        passed = process.returncode == 0 if expected_failure is None else process.returncode != 0 and expected_failure in process.stderr
        result['commands'].append({'name': label, 'exit': process.returncode, 'passed': passed})
        save()
        if not passed:
            raise RuntimeError(label + ': ' + process.stderr[-6000:])
        return process.stdout

    sources = {}
    for path in [*production.glob('src/**/*.cppm'), *production.glob('third-parties/fast_io/share/fast_io/*.cppm')]:
        match = re.search(r'^\s*export\s+module\s+([\w.:]+)\s*;', path.read_text(encoding='utf-8-sig'), re.M)
        if match:
            assert match[1] not in sources, 'duplicate module owner: ' + match[1]
            sources[match[1]] = path
    built = {}
    failed = {}
    active = []

    def build(name):
        if name in failed:
            raise RuntimeError(failed[name])
        try:
            return build_impl(name)
        except Exception as error:
            failed[name] = str(error)
            if active and active[-1] == name:
                active.pop()
            raise

    def build_impl(name):
        if name in built:
            return built[name]
        assert name not in active, 'module dependency cycle: ' + ' -> '.join([*active, name])
        active.append(name)
        source = sources[name]
        label = name.replace(':', '-')
        preprocessed = command(label + '.dependencies', [*compile_flags, '-E', '-P', str(source)])
        dependencies = []
        for imported in re.findall(r'^\s*(?:export\s+)?(?:__preprocessed_)?import\s+([\w.:]+)\s*;', preprocessed, re.M):
            if imported.startswith(':'):
                imported = name.split(':', 1)[0] + imported
            dependencies.append(imported)
            build(imported)
        pcm = output/(label + '.pcm')
        imported_flags = ['-fmodule-file=' + dependency + '=' + str(built[dependency]) for dependency in dict.fromkeys(dependencies)]
        command(label + '.interface', [*compile_flags, *imported_flags, '--precompile', str(source), '-o', str(pcm)])
        built[name] = pcm
        result['modules'][name] = {'source': str(source.relative_to(production)), 'imports': dependencies}
        active.pop()
        print('compiled ' + name, flush=True)
        save()
        return pcm

    try:
        if args.host_fp_mode:
            probe = output/'backend_macros.cpp'
            probe.write_text('#include <uwvm2/utils/macro/push_macros.h>\n'
                             '#include <uwvm2/uwvm/runtime/macro/push_macros.h>\n')
            macros = command('backend_macros', [*compile_flags, '-E', '-dM', str(probe)])
            llvm_enabled = bool(re.search(r'^#define UWVM_RUNTIME_LLVM_JIT(?:\s|$)', macros, re.M))
            result['llvm_runtime_marker'] = llvm_enabled
            assert llvm_enabled == (args.host_fp_mode == 'llvm'), 'Runtime macro configuration does not match the requested policy'
        for name in ('uwvm2.imported.ros.scene', 'uwvm2.uwvm.imported.scene.storage', 'uwvm2.uwvm.debugger.scene_state'):
            build(name)
        objects = []
        for name in ('uwvm2.imported.ros.scene', 'uwvm2.uwvm.imported.scene.storage'):
            obj = output/(name + '.o')
            command(name + '.object', [*compile_flags, '-c', str(built[name]), '-o', str(obj)])
            objects.append(str(obj))
        for name in ('module_owner', 'module_observer', 'module_header_glue'):
            obj = output/(name + '.o')
            command(name, [*compile_flags, '-c', str(ROOT/'test/0084.ros-runtime'/(name + '.cpp')), '-o', str(obj)])
            objects.append(str(obj))
        executable = output/'module_identity'
        command('link', [link_flags[0], *objects, *link_flags[1:], '-o', str(executable)])
        command('identity', [str(executable)])
        invalid = output/'invalid_owner.cppm'
        invalid.write_text('module;\nexport module invalid_owner;\n#define UWVM_MODULE\n#include <uwvm2/uwvm/imported/scene/storage.h>\n')
        command('reject_duplicate_owner', [*compile_flags, '--precompile', str(invalid), '-o', str(output/'invalid.pcm')],
                'Import uwvm2.uwvm.imported.scene.storage')
        if args.isolated_host:
            assert any(flag.split('=',1)[0]=='-DUWVM_IMPORT_ROSV2' for flag in compile_flags)
            assert not args.root, 'Isolated infrastructure must not stand in for a production graph root'
            build('fast_io_crypto')
            command('isolated_host', [*compile_flags, '--precompile', str(ROOT/'test/0084.ros-runtime/host_module_export.cppm'),
                                     '-o', str(output/'uwvm2.uwvm.imported.scene.pcm')])
            fixture = str(ROOT/'test/0084.ros-runtime/host_fp_policy.cpp')
            command('header_host_fp_policy', [*compile_flags, '-fsyntax-only', fixture])
            command('isolated_module_host_fp_policy', [*compile_flags, '-DUWVM_SCENE_TEST_MODULE', '-fsyntax-only', fixture])
        roots = list(args.root)
        if args.graph:
            roots += ['uwvm2.uwvm.cmdline.params:scene', 'uwvm2.uwvm.imported.scene',
                      'uwvm2.uwvm.wasm.loader:load_and_check_modules', 'uwvm2.uwvm.runtime.threaded_builtin_loader',
                      'uwvm2.uwvm.run:loader']
        graph_errors = {}
        for name in dict.fromkeys(roots):
            try:
                build(name)
            except Exception as error:
                graph_errors[name] = str(error)
                print('blocked ' + name + ': ' + str(error).splitlines()[0], flush=True)
        result['graph_errors'] = graph_errors
        if graph_errors:
            raise RuntimeError('Module graph compilation failed; see results.json for each root')
        result['complete'] = True
    finally:
        checks_complete = result['complete']
        result['complete'] = False
        result['scene_source_sha256'] = hashes
        changed = []
        for name, value in hashes.items():
            try:
                unchanged = hashlib.sha256((ROOT/name).read_bytes()).hexdigest() == value
            except OSError:
                unchanged = False
            if not unchanged:
                changed.append(name)
        result['changed_tracked_inputs'] = changed
        result['tracked_scene_sources_unchanged'] = not changed
        result['complete'] = checks_complete and not changed
        result['events_after'] = (GROUP/'memory.events').read_text()
        save()
        if changed:
            raise RuntimeError('Tracked scene sources or driver changed during checks: ' + ', '.join(changed))


if __name__ == '__main__':
    main()
