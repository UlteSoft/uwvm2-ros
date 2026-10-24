#!/usr/bin/env python3
"""Regenerate/format an exact owned C family and compare real compiler tokens.

No upstream/provider file or accepted SDK is modified. Compiler syntax/token
checks are not xmake artifact builds, Guests, native observations or port credit.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess


def sha(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        while block := stream.read(1 << 20):
            digest.update(block)
    return digest.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('emitter', 'source', 'sdk', 'monitor', 'formatter-root', 'formatting', 'compiler', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same common finite Linux owner required')
    args.out.mkdir()
    original = args.out / 'raw'
    formatted = args.out / 'formatted'
    original.mkdir()
    formatted.mkdir()
    family = 'flexiv-ros2-humble-v1-complete'
    vendor = args.source / 'sdk/origin/custom-types' / family / 'generation.vendor.json'
    catalog = args.source / 'sdk/origin/robots/arms/flexiv/catalogs/ros2_humble_v1/catalog.json'
    metadata = json.loads(vendor.read_bytes())
    declaration = json.loads(catalog.read_bytes())
    expected = metadata['source_inputs']['catalog']['sha256']
    normalized = (json.dumps(declaration, indent=2, sort_keys=True) + '\n').encode()
    if sha(catalog) != expected and hashlib.sha256(normalized).hexdigest() != expected:
        raise RuntimeError('exact original prepared catalog required')
    registered = {row['type'] for row in declaration['types'] if row.get('registered', True)}
    types = [row for row in metadata['types'] if row['type'] in registered]
    if {row['type'] for row in types} != registered:
        raise RuntimeError('complete selected original type set required')
    spec = importlib.util.spec_from_file_location('owned_operations_emitter', args.emitter)
    emitter = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(emitter)
    selected = args.sdk / 'include/platform/flexiv/c/ros2' / family
    original_header = selected / 'bounded_records.h'
    bodies = {'operations.h': emitter.header('flexiv', family, 2, types),
              'operations.c': emitter.application('flexiv', family, 2, types),
              'robot_states_monitor.c': args.monitor.read_text()}
    for name, body in emitter.preload('flexiv', family, 2, types, original_header.read_text()).items():
        if name.endswith('.c'):
            bodies['preload/' + name] = body
    if len(bodies) != 7:
        raise RuntimeError('exact complete seven-file owned formatting scope required')
    manifest = json.loads((args.formatting / 'formatters.json').read_bytes())
    tool = manifest['tools']['clang-format-21']
    executable = args.formatter_root / tool['path']
    configuration = args.formatting / tool['configuration']
    if sha(executable) != tool['sha256'] or sha(configuration) != tool['configuration_sha256']:
        raise RuntimeError('exact current shared formatter/configuration pins required')
    input_paths = [Path(__file__), args.emitter, args.monitor, vendor, catalog,
                   original_header, executable, configuration, args.formatting / 'formatters.json', args.compiler]
    pins = [dict(path=str(path), sha256=sha(path), bytes=path.stat().st_size) for path in input_paths]
    cases = []

    def invoke(name, command, data=None):
        result = subprocess.run(list(map(str, command)), input=data, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, timeout=45, check=False)
        if len(result.stdout) > 2 << 20 or len(result.stderr) > 65536:
            raise RuntimeError('bounded qualification output exceeded')
        (args.out / (name + '.stdout')).write_bytes(result.stdout)
        (args.out / (name + '.stderr')).write_bytes(result.stderr)
        cases.append(dict(name=name, argv=list(map(str, command)), exit=result.returncode,
                          status='PASS' if result.returncode == 0 else 'FAIL',
                          stdout_sha256=hashlib.sha256(result.stdout).hexdigest(),
                          stderr_sha256=hashlib.sha256(result.stderr).hexdigest()))
        if result.returncode:
            raise RuntimeError('actual formatter/compiler failure: ' + name)
        return result.stdout

    invoke('formatter-version', [executable, '--version'])
    invoke('compiler-version', [args.compiler, '--version'])
    products = []
    for index, (name, text) in enumerate(bodies.items()):
        before, after = original / name, formatted / name
        before.parent.mkdir(parents=True, exist_ok=True)
        after.parent.mkdir(parents=True, exist_ok=True)
        raw = text.encode()
        before.write_bytes(raw)
        command = [executable, '--style=file:' + str(configuration), '--assume-filename=' + str(before)]
        cooked = invoke('format-' + str(index), command, raw)
        repeated = invoke('repeat-' + str(index), command, cooked)
        if cooked != repeated:
            raise RuntimeError('formatter is not byte idempotent: ' + name)
        # Documentation/string literals are also covered by actual compiler
        # token equality below; keep their full source bytes in both versions.
        for marker in (b'@brief', b'@details', b'@see'):
            if raw.count(marker) != cooked.count(marker):
                raise RuntimeError('documentation tag extent changed')
        after.write_bytes(cooked)
        products.append(dict(path=name, before_sha256=sha(before), after_sha256=sha(after),
                             idempotent=True, bytes=len(cooked)))

    def compiler_argv(path, standard):
        language = 'c++' if standard == 'c++20' else 'c'
        root = original if path.is_relative_to(original) else formatted
        return [str(args.compiler), '--target=wasm32-wasip1', '--sysroot=' + str(args.sdk),
                '-std=' + standard, '-x', language, '-I', str(root), '-I', str(selected),
                '-I', str(args.sdk / 'include'), '-fsyntax-only', str(path)]

    def tokens(path, standard):
        command = compiler_argv(path, standard)
        command[1:1] = ['-Xclang', '-dump-tokens']
        digest, raw_digest, count = hashlib.sha256(), hashlib.sha256(), 0
        diagnostics = bytearray()
        process = subprocess.Popen(command, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        for line in process.stderr:
            raw_digest.update(line)
            if b'Loc=<' in line:
                token = line.split(b'\t', 1)[0]
                digest.update(token + b'\n')
                count += 1
            elif len(diagnostics) < 65536:
                diagnostics.extend(line[:65536-len(diagnostics)])
        status = process.wait(timeout=45)
        if status or not count:
            raise RuntimeError('actual compiler token inventory failed: ' + diagnostics.decode(errors='replace'))
        return dict(argv=command, exit=status, tokens=count, normalized_sha256=digest.hexdigest(),
                    raw_stderr_sha256=raw_digest.hexdigest(),
                    retention='Complete streamed token digest; raw included-header token output is not stored')

    comparisons = []
    for index, name in enumerate(bodies):
        if name.endswith('.h'):
            continue  # Every translation unit consumes the exact generated header.
        for standard in ('c11', 'c++20'):
            before, after = original / name, formatted / name
            invoke('syntax-before-' + str(index) + '-' + standard, compiler_argv(before, standard))
            invoke('syntax-after-' + str(index) + '-' + standard, compiler_argv(after, standard))
            first, second = tokens(before, standard), tokens(after, standard)
            if (first['tokens'], first['normalized_sha256']) != (second['tokens'], second['normalized_sha256']):
                raise RuntimeError('formatter changed actual target compiler tokens: ' + name)
            comparisons.append(dict(path=name, standard=standard, before=first, after=second))
    command = compiler_argv(formatted / 'robot_states_monitor.c', 'c11')
    command[1:1] = ['-Xclang', '-ast-dump=json', '-Xclang', '-ast-dump-filter=uwvm_flexiv_monitor_value']
    ast = json.loads(invoke('public-monitor-ast', command))
    if ast['kind'] != 'FunctionDecl' or ast['name'] != 'uwvm_flexiv_monitor_value':
        raise RuntimeError('exact actual public monitor declaration required')
    parameters = [node['name'] for node in ast.get('inner', []) if node['kind'] == 'ParmVarDecl']
    if parameters != ['value'] or not any(node['kind'] == 'FullComment' for node in ast.get('inner', [])):
        raise RuntimeError('parser-bound parameter/documentation attachment required')
    for pin in pins:
        if sha(Path(pin['path'])) != pin['sha256']:
            raise RuntimeError('immutable formatter/compiler/source input changed')
    for name, value in (('source-pins.json', pins), ('cases.json', cases), ('products.json', products),
                        ('compiler-token-comparisons.json', comparisons)):
        (args.out / name).write_text(json.dumps(value, indent=2) + '\n')
    summary = dict(status='PASS_OWNED_SOURCE_SUBSET_ONLY', formatted_inputs=len(products),
                   byte_idempotence=True, target_syntax_and_token_pairs=len(comparisons),
                   parser_bound_monitor_function=dict(name=ast['name'], parameters=parameters,
                                                     source_sha256=sha(formatted / 'robot_states_monitor.c')),
                   complete_platform_input_coverage=False, post_format_xmake_builds=0,
                   guest=0, native_ros=0, publication_ready=False)
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
