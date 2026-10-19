#!/usr/bin/env python3
"""Build exact synchronous MoonBit SDK models on both pinned Wasm backends.

The default four-package profile runs66 methods per target. The explicit owned
service profile adds four pure frame/protocol methods per target; neither
profile calls a native ROS import or proves middleware delivery.

This serial developer tool owns a fresh output directory, NOT a process/cgroup
supervisor. Its caller must bound the entire tree, memory, disk and wall time,
retain real descendant ownership and perform mandatory final cleanup. A local
model pass confers no UWVM execution, ROS grant or native-delivery qualification.
Only fixed adjacent SDK tools/code are imported; catalogs cannot supply code.
"""
from __future__ import annotations
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import stat
import subprocess
import sys
import time

SDK = Path(__file__).resolve().parents[1]
ROOT = SDK.parent
MIB = 1024 * 1024
PINNED_MOON = 'a31026b83410607909ebae469fa19e46c32201cfe403be6099e1da961a5ebf0f'
PINNED_MOONC = 'fb950cbc125168c49523ef554d2649effd66499c596dafcb15c80b561d45fd8c'
CHECKER_SHA = '327fccbd6bdcfae0ae8da593014f4fb4451747b1ee1c9991996242b798c9bfa4'
RUNNER_SHA = '1f39d35bc20cbd8ad42ea93661cde30b38ce51a97a84cac4958fc18c85da641f'
RESERVATION = '\n// Test-producer-only reservation; SDK implementation/test bodies remain exact.\noptions(link: { "wasm": { "export-memory-name": "memory", "heap-start-address": 8519680, "memory-limits": { "min": 256, "max": 1024 } }, "wasm-gc": { "export-memory-name": "memory", "use-js-builtin-string": false, "memory-limits": { "min": 256, "max": 1024 } }, },)\n'
PACKAGES = {'linear': ('whitebox', 'linear_wbtest.mbt', 14),
            'ros1': ('whitebox', 'client_wbtest.mbt', 13),
            'ros2': ('whitebox', 'client_wbtest.mbt', 13),
            'wire': ('blackbox', 'wire_test.mbt', 26)}
# This is a closed test-host API. The filesystem-named hooks only read the
# already supplied filename string in the byte-exact runner, never a host file.
HOST = {
    ('exception', 'throw'): ([], []),
    ('wasi_snapshot_preview1', 'fd_write'): (['i32'] * 4, ['i32']),
    ('spectest', 'print_char'): (['i32'], []),
    ('__moonbit_fs_unstable', 'begin_read_string'): (['externref'], ['externref']),
    ('__moonbit_fs_unstable', 'string_read_char'): (['externref'], ['i32']),
    ('__moonbit_fs_unstable', 'finish_read_string'): (['externref'], []),
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def physical(value):
    """Allow parent aliases such as macOS /tmp; reject final symlink adoption."""
    p = Path(value).expanduser().absolute()
    require(not p.is_symlink(), 'final path alias refused: ' + str(p))
    return p.resolve(strict=True)


def opened(path, limit=256 * MIB):
    """Same-FD hash plus named metadata, mode and nlink bracket for each input."""
    path = Path(path)
    require(path.resolve(strict=True) == path, 'physical file required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW)
    try:
        first = os.fstat(descriptor)
        require(stat.S_ISREG(first.st_mode) and first.st_size <= limit,
                'bounded ordinary file required: ' + str(path))
        digest = hashlib.sha256()
        chunks = []
        total = 0
        while True:
            chunk = os.read(descriptor, 65536)
            if not chunk:
                break
            require(len(chunk) <= limit - total, 'input exceeded cumulative byte bound')
            total += len(chunk)
            chunks.append(chunk)
            digest.update(chunk)
        last = os.fstat(descriptor)
        named = path.lstat()
        shape = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns,
                           s.st_ctime_ns, s.st_mode, s.st_nlink)
        require(total == first.st_size, 'input extent changed before EOF')
        require(shape(first) == shape(last) == shape(named), 'input changed while read')
        raw = b''.join(chunks)
        return raw, {'path': str(path), 'bytes': len(raw), 'sha256': digest.hexdigest(),
                     'device': last.st_dev, 'inode': last.st_ino,
                     'mtime_ns': last.st_mtime_ns, 'ctime_ns': last.st_ctime_ns,
                     'mode': stat.S_IMODE(last.st_mode), 'nlink': last.st_nlink}
    finally:
        os.close(descriptor)


def save(path, value):
    """Publish a bounded report only below this tool's fresh owned scratch."""
    raw = (json.dumps(value, indent=2) + '\n').encode()
    require(len(raw) <= 32 * MIB, 'report ceiling')
    with path.open('xb') as stream:
        stream.write(raw)
    return opened(path)[1]


def literal_identity(path):
    """Retain a literal alias separately from its actual physical target."""
    first = path.lstat()
    link = os.readlink(path) if stat.S_ISLNK(first.st_mode) else None
    last = path.lstat()
    shape = lambda v: (v.st_dev, v.st_ino, v.st_size, v.st_mtime_ns,
                       v.st_ctime_ns, v.st_mode, v.st_nlink)
    require(shape(first) == shape(last), 'literal input changed')
    return {'path': str(path), 'device': first.st_dev, 'inode': first.st_ino,
            'bytes': first.st_size, 'mtime_ns': first.st_mtime_ns,
            'ctime_ns': first.st_ctime_ns, 'mode': first.st_mode,
            'nlink': first.st_nlink, 'link': link}


def tree_inventory(source):
    """Name/type inventory brackets a complete finite selected input tree."""
    rows = []
    for entry in sorted(source.rglob('*')):
        require(len(rows) < 20000, 'selected input entry ceiling')
        rows.append({'relative': str(entry.relative_to(source)),
                     'literal': literal_identity(entry)})
    return rows


def copy_tree(source, destination):
    """Copy complete ordinary source/tool files without changing the input tree.

    A regular-file alias inside the selected root (the pinned core has one
    README alias) is materialized as bytes, with its literal/target recorded.
    Directory aliases, foreign targets, sockets and special files refuse.
    Compilation/caches can then mutate only the private copy. This is not an
    assertion that an arbitrary tool tree is an approved native SDK.
    """
    destination.mkdir(mode=0o700)
    rows = []
    total = 0
    inventory = tree_inventory(source)
    entries = [source / row['relative'] for row in inventory]
    for entry in entries:
        relative = entry.relative_to(source)
        target = destination / relative
        literal = entry.lstat()
        if stat.S_ISDIR(literal.st_mode):
            target.mkdir(mode=0o700)
            continue
        alias = None
        if stat.S_ISLNK(literal.st_mode):
            actual = entry.resolve(strict=True)
            require(actual.is_relative_to(source) and actual.is_file(),
                    'only in-root ordinary-file aliases allowed')
            alias = {'literal': literal_identity(entry), 'target': str(actual)}
        else:
            actual = entry
        raw, record = opened(actual)
        if alias is not None:
            require(literal_identity(entry) == alias['literal'] and entry.resolve(strict=True) == actual,
                    'literal alias/target changed during input read')
        total += len(raw)
        require(total <= 1024 * MIB, 'complete copied input byte ceiling')
        target.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
        with target.open('xb') as stream:
            stream.write(raw)
        target.chmod(0o700 if record['mode'] & 0o111 else 0o600)
        rows.append({'relative': str(relative), 'original': record, 'alias': alias,
                     'copied': opened(target)[1]})
    require(tree_inventory(source) == inventory, 'input tree changed during copy')
    return rows, inventory


def invoke(argv, directory, environment, timeout, ledger):
    """One serial direct subprocess; the external caller still owns its tree.

    No shell, package-manager, network, default moonrun or ambient PATH is used.
    Raw stdout/stderr and terminal exit are retained even on a command error.
    On timeout subprocess.run kills/waits its retained direct child; genuine
    descendant/cgroup cleanup is deliberately the outer owner's obligation.
    """
    directory.mkdir(mode=0o700)
    begin = time.monotonic()
    row = {'argv': [str(x) for x in argv], 'timeout_seconds': timeout,
           'cwd': str(directory), 'direct_child_terminal': False}
    ledger.append(row)  # Retain the known row before any process effect.
    launch_error = None
    with (directory / 'stdout.log').open('xb') as output, (directory / 'stderr.log').open('xb') as error:
        try:
            done = subprocess.run(row['argv'], cwd=directory, env=environment,
                                  stdin=subprocess.DEVNULL, stdout=output,
                                  stderr=error, timeout=timeout, check=False)
            row['exit_code'] = done.returncode
            row['direct_child_terminal'] = True
        except subprocess.TimeoutExpired:
            row['timeout'] = True
            row['direct_child_terminal'] = True
            row['exit_code'] = None
        except BaseException as error:
            launch_error = error
            row['error'] = {'type': type(error).__name__, 'message': str(error)}
    row['seconds'] = time.monotonic() - begin
    row['stdout'] = opened(directory / 'stdout.log', 32 * MIB)[1]
    row['stderr'] = opened(directory / 'stderr.log', 32 * MIB)[1]
    row['command_report'] = save(directory / 'command.json', row)
    if launch_error is not None:
        raise launch_error
    require(row['exit_code'] == 0 and not row.get('timeout'), 'command failed: ' + str(directory))
    return row


def inspect_artifact(checker, data, target, package, wat):
    """Actual sections, types/imports, data extents and pinned allocator anchor.

    Full wasm-tools validation is an independent preceding gate. The TLSF first
    initializer constant is an actual pinned-producer anchor, not a formal
    proof of all possible allocator writes or a general future compiler theorem.
    """
    source = checker.Reader(data)
    require(bytes(source.take(8)) == b'\0asm\1\0\0\0', 'core module required')
    types, imports, functions, tags = [], [], [], []
    memories, segments, exports, starts, seen = [], [], {}, [], set()
    while source.position < len(source.data):
        section = source.byte()
        body = checker.Reader(source.take(source.uint()))
        if section in (1, 2, 3, 5, 7, 8, 11, 13):
            require(section not in seen, 'duplicate parsed section')
            seen.add(section)
        if section == 1:
            types = body.types(); body.end()
        elif section == 2:
            for _ in range(body.count()):
                module, name, kind = body.name(), body.name(), body.byte()
                require(kind in (0, 4), 'only fixed function/tag imports allowed')
                if kind == 4:
                    require(body.byte() == 0, 'tag attribute')
                index = body.uint()
                require(index < len(types) and types[index] is not None, 'actual function type required')
                signature = types[index]
                if kind == 4:
                    require((module, name) == ('exception', 'tag') and signature == ([], []),
                            'only the actual empty exception tag is supported')
                    tags.append(signature)
                elif module.startswith('uwvm_ros'):
                    require(package in ('ros1', 'ros2') and module == 'uwvm_' + package + '_v1',
                            'wrong backend or unexpected ROS import')
                    functions.append(index)
                else:
                    require(HOST.get((module, name)) == signature, 'unknown host function/signature')
                    functions.append(index)
                imports.append({'module': module, 'name': name,
                                'kind': 'function' if kind == 0 else 'tag'})
            body.end()
        elif section == 3:
            for _ in range(body.count()):
                index = body.uint()
                require(index < len(types) and types[index] is not None, 'defined function type')
                functions.append(index)
            body.end()
        elif section == 5:
            for _ in range(body.count()):
                require(body.uint() == 1, 'ordinary bounded memory only')
                memories.append([body.uint(), body.uint()])
            body.end()
        elif section == 7:
            for _ in range(body.count()):
                name, kind, index = body.name(), body.byte(), body.uint()
                require(name not in exports, 'duplicate export')
                exports[name] = [kind, index]
            body.end()
        elif section == 8:
            starts.append(body.uint()); body.end()
        elif section == 13:
            require(body.count() == 0, 'defined or additional exception tags need a separate reviewed runner')
            body.end()
        elif section == 11:
            for _ in range(body.count()):
                mode = body.uint()
                require(mode in (0, 1, 2), 'data segment mode')
                row = {'mode': mode}
                if mode != 1:
                    memory = 0 if mode == 0 else body.uint()
                    require(memory == 0 and body.byte() == 0x41, 'constant memory0 offset only')
                    row['offset'] = body.sint(32) & 0xffffffff
                    require(body.byte() == 0x0b, 'constant offset expression')
                row['bytes'] = len(body.take(body.uint())); segments.append(row)
            body.end()
    require(memories == [[256, 1024]] and exports.get('memory') == [2, 0], 'actual reserved memory limits/export')
    require(len(starts) == 1 and tags == [([], [])], 'actual pinned start/tag inventory')
    require(starts[0] < len(functions) and types[functions[starts[0]]] == ([], []),
            'actual start function signature')
    required_exports = {'memory', '_start', 'moonbit_test_driver_internal_execute', 'moonbit_test_driver_finish'}
    require(set(exports) == required_exports, 'closed pinned test export inventory')
    for name, signature in (('_start', ([], [])),
                            ('moonbit_test_driver_finish', ([], [])),
                            ('moonbit_test_driver_internal_execute', (['externref', 'i32'], []))):
        kind, index = exports[name]
        require(kind == 0 and index < len(functions) and types[functions[index]] == signature,
                'unexpected generated test entry signature')
    active_end = 0
    for row in segments:
        if row['mode'] == 1:
            continue
        begin, end = row['offset'], row['offset'] + row['bytes']
        require(end <= 256 * 65536 and all(end <= lo or begin >= hi for lo, hi in
                ((0, 16), (32, 289), (65536, 8454160))), 'active data overlaps reserved memory')
        active_end = max(active_end, end)
    anchor = None
    if target == 'wasm':
        lines = wat.splitlines()
        positions = [i for i, line in enumerate(lines) if line.startswith('  (func $tlsf/initialize ')]
        require(len(positions) == 1, 'actual pinned TLSF initializer required')
        begin = positions[0]
        end = next((i for i in range(begin + 1, len(lines)) if lines[i].startswith('  (func ')), len(lines))
        first = next(line.strip() for line in lines[begin + 1:end] if not line.strip().startswith('(local '))
        match = re.fullmatch(r'i32.const ([0-9]+)', first)
        require(match is not None, 'actual pinned initializer first instruction')
        anchor = int(match[1])
        require(anchor >= 8519680 and anchor >= active_end, 'allocator anchor precedes reserved/data extent')
    else:
        require('$tlsf/' not in wat and re.search(r'^\s+memory\.(init|grow)\b', wat, re.M) is None,
                'GC test producer must not initialize/grow the copy arena')
    # Shared maintained reader validates every observed numeric ROS name and
    # signature against the actual adjacent abi-v1.json; not all imports are
    # required because this is a model, NOT a complete ABI coverage artifact.
    if package in ('ros1', 'ros2'):
        checker.check(data, package[-1], ('exception', 'wasi_snapshot_preview1',
                      'spectest', '__moonbit_fs_unstable'), False)
    return {'imports': imports, 'exports': exports, 'start': starts,
            'memories': memories, 'segments': segments,
            'pinned_TLSF_initializer_anchor': anchor,
            'qualification_limit': 'actual sections/extent/producer anchor only; no formal allocator proof'}


def test_info(path, filename, expected):
    raw, identity = opened(path, MIB)
    value = json.loads(raw)
    require(value.get('tests') == value.get('no_args_tests'), 'synchronous no-argument tests only')
    for role in ('with_args_tests', 'with_bench_args_tests', 'async_tests', 'async_tests_with_args'):
        require(isinstance(value.get(role), dict) and all(v == [] for v in value[role].values()), 'unsupported test mode')
    selected = [(file, rows) for file, rows in value['tests'].items() if rows]
    require(len(selected) == 1 and selected[0][0] == filename, 'exact source test file')
    rows = selected[0][1]
    require(len(rows) == expected and all(row.get('index') == i and row.get('attrs') == []
            and isinstance(row.get('name'), str) and len(row['name']) <= 512 for i, row in enumerate(rows)),
            'complete unskipped test inventory required')
    return identity


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--moon', type=Path, required=True)
    parser.add_argument('--moon-home', type=Path, required=True)
    parser.add_argument('--node', type=Path, required=True)
    parser.add_argument('--wasm-tools', type=Path, required=True)
    parser.add_argument('--scratch', type=Path, required=True)
    parser.add_argument('--include-owned-service', action='store_true',
                        help='add four pure owned-service frame/protocol models per target; no native ROS')
    args = parser.parse_args()
    packages = dict(PACKAGES)
    if args.include_owned_service:
        packages['owned_service'] = ('whitebox', 'protocol_contract_wbtest.mbt', 4)
    expected_tests = sum(row[2] for row in packages.values())
    expected_products = sum(2 if package == 'wire' else 3 for package in packages)
    moon_home_source, moon, node, wasm_tools = map(physical, (args.moon_home, args.moon, args.node, args.wasm_tools))
    require(moon_home_source.is_dir() and moon == (moon_home_source / 'bin/moon').resolve(strict=True), 'selected home/bin/moon required')
    pending = args.scratch.expanduser().absolute()
    require(not pending.exists() and not pending.is_symlink(), 'fresh scratch required')
    scratch = physical(pending.parent) / pending.name
    for input_root in (ROOT, moon_home_source, node.parent, wasm_tools.parent):
        require(not scratch.is_relative_to(input_root) and not input_root.is_relative_to(scratch), 'scratch overlaps an input root')
    # Only explicit physical executable selections are used. No environment
    # executable or registry/module path is accepted as a substitute.
    bin_literal = literal_identity(moon_home_source / 'bin')
    bin_directory = (moon_home_source / 'bin').resolve(strict=True)
    require(bin_directory.is_dir(), 'selected actual bin directory required')
    admitted = [opened(p)[1] for p in (moon, physical(moon_home_source / 'bin/moonc'), node, wasm_tools)]
    require(admitted[0]['sha256'] == PINNED_MOON and admitted[1]['sha256'] == PINNED_MOONC, 'exact pinned producer required')
    for identity in admitted:
        require(identity['mode'] & 0o111, 'executable input required')
    reader_path = physical(Path(__file__).with_name('check_wasm_imports.py'))
    runner_path = physical(Path(__file__).with_suffix('.mjs'))
    code_inputs = [opened(p)[1] for p in (physical(__file__), reader_path, runner_path, SDK / 'abi/abi-v1.json')]
    require(code_inputs[1]['sha256'] == CHECKER_SHA, 'fixed adjacent GC reader source required')
    require(code_inputs[2]['sha256'] == RUNNER_SHA, 'byte-exact independently reviewed model runner required')
    scratch.mkdir(mode=0o700)
    report = {'format_version': 2, 'kind': 'MOONBIT_BOUNDED_LOCAL_MODELS_NOT_ROS_ACCEPTANCE',
              'expected_models_per_target': expected_tests, 'selected_model_packages': list(packages),
              'expected_build_products_per_target': expected_products,
              'status': 'FAIL', 'passed': False, 'tools': admitted, 'code_inputs': code_inputs,
              'commands': [], 'modules': [], 'model_tests_per_target': {'wasm': 0, 'wasm-gc': 0},
              'ros_import_calls': None, 'uwvm_execution_cases': 0, 'native_ros_delivery_cases': 0,
              'original_inputs_preserved': False, 'input_copy_admission_completed': False,
              'process_tree_cleanup': 'REQUIRED_EXTERNAL_CALLER_PROOF_NOT_PROVIDED_BY_THIS_TOOL'}
    source_rows, core_rows, inventories = [], [], []
    def command(argv, directory, timeout):
        return invoke(argv, directory, environment, timeout, report['commands'])
    try:
        source = SDK / 'moonbit'
        source_rows, source_inventory = copy_tree(source, scratch / 'source')
        inventories.append((source, source_inventory))
        report['source_inputs'] = source_rows
        private_moon = scratch / 'moon-home'
        private_moon.mkdir(mode=0o700)
        (private_moon / 'lib').mkdir(mode=0o700)
        # Copy only the actual core and its existing bundles. The selected
        # tool bin directory remains a read-only input reference; no341MiB
        # whole-tool-home copy or global SDK/cache update is attempted.
        core_source = physical(moon_home_source / 'lib/core')
        core_rows, core_inventory = copy_tree(core_source, private_moon / 'lib/core')
        inventories.append((core_source, core_inventory))
        report['core_inputs'] = core_rows
        report['input_copy_admission_completed'] = True
        # The pinned CLI resolves its core relative to its executable. Copy
        # exactly the two admitted native tools so the selected private core is
        # used; a bin-directory symlink resolves back into the original home.
        private_bin = private_moon / 'bin'
        private_bin.mkdir(mode=0o700)
        report['private_tool_copies'] = []
        for original in (moon, physical(moon_home_source / 'bin/moonc')):
            body, identity = opened(original)
            require(identity in admitted, 'native tool copy must be an admitted input')
            copied = private_bin / original.name
            with copied.open('xb') as stream:
                stream.write(body)
            copied.chmod(0o700)
            pin = opened(copied)[1]
            require(pin['sha256'] == identity['sha256'], 'native tool copy identity')
            report['private_tool_copies'].append({'original': identity, 'copied': pin})
        report['source_inputs'], report['core_inputs'] = source_rows, core_rows
        report['original_tree_inventories'] = [{'path': str(root), 'entries': rows} for root, rows in inventories]
        report['bin_input_reference'] = {'literal': bin_literal, 'physical_target': str(bin_directory)}
        for package in packages:
            raw, _ = opened(scratch / 'source' / package / 'moon.pkg')
            require(raw.endswith(RESERVATION.encode()) and raw.count(RESERVATION.encode()) == 1,
                    'exact already-reviewed test linker reservation required')
        for name in ('tmp', 'cache', 'config', 'data'):
            (scratch / name).mkdir(mode=0o700)
        # Retain the caller's original HOME without repurposing it. Compiler
        # core/cache/config/tmp writes are selected explicitly below.
        environment = {name: os.environ[name] for name in ('HOME',) if name in os.environ}
        environment.update({'PATH': str(private_moon / 'bin') + os.pathsep + os.defpath,
                       'MOON_HOME': str(private_moon),
                       'TMPDIR': str(scratch / 'tmp'), 'XDG_CACHE_HOME': str(scratch / 'cache'),
                       'XDG_CONFIG_HOME': str(scratch / 'config'), 'XDG_DATA_HOME': str(scratch / 'data'),
                       'LANG': 'C.UTF-8', 'LC_ALL': 'C.UTF-8'})
        # This is intentionally a newly constructed environment. NODE_OPTIONS,
        # NODE_PATH, NODE_REPL_EXTERNAL_MODULE, MOON*/CC/loader/Python injections
        # are never inherited. Only the private selected MOON_HOME/PATH is set.
        sys.dont_write_bytecode = True
        specification = importlib.util.spec_from_file_location('_ros_moonbit_model_import_checker', reader_path)
        checker = importlib.util.module_from_spec(specification)
        specification.loader.exec_module(checker)
        for target in ('wasm', 'wasm-gc'):
            output = scratch / ('build-' + target)
            selected = []
            if not args.include_owned_service:
                # Select the closed four-package profile even when the copied
                # SDK contains an independent opt-in extension package.
                selected = ['--package', *('uwvm/ros/' + package for package in packages)]
            command([private_moon / 'bin/moon', '-C', scratch / 'source',
                'test', '--frozen', '--build-only', '--target', target, '-j', '1', '--debug',
                '--verbose', '--target-dir', output, *selected], scratch / ('command-build-' + target), 180)
            expected = {f'{package}/{package}.{mode}_test.wasm' for package in packages
                        for mode in (('blackbox', 'internal') if package == 'wire' else ('blackbox', 'internal', 'whitebox'))}
            built = output / target / 'debug/test'
            actual = {str(p.relative_to(built)) for p in built.rglob('*.wasm')}
            require(actual == expected and len(actual) == expected_products,
                    'exact selected pinned compiler test modules per target')
            report.setdefault('all_build_products', []).extend(opened(built / rel, 64 * MIB)[1] for rel in sorted(actual))
            for package, (mode, filename, count) in packages.items():
                artifact = built / package / f'{package}.{mode}_test.wasm'
                raw, wasm = opened(artifact, 64 * MIB)
                wasm['target'] = target
                label = target + '-' + package
                command([wasm_tools, 'validate', artifact], scratch / ('validate-' + label), 30)
                printed = scratch / ('print-' + label)
                command([wasm_tools, 'print', artifact], printed, 30)
                wat = opened(printed / 'stdout.log', 32 * MIB)[0].decode('utf-8', 'strict')
                inspection = inspect_artifact(checker, raw, target, package, wat)
                info = test_info(built / package / f'__{mode}_test_info.json', filename, count)
                # Node checks actual bytes, paths and SHA256. Nanosecond and
                # inode integers belong to the exact Python-side physical pin;
                # JSON Number cannot preserve their full width in JavaScript.
                runner_wasm = {key: wasm[key] for key in ('path', 'bytes', 'sha256', 'target')}
                runner_info = {key: info[key] for key in ('path', 'bytes', 'sha256')}
                manifest = {'kind': 'moonbit-synchronous-local-model-module-v1', 'wasm': runner_wasm,
                            'test_info': runner_info, 'imports': inspection['imports'],
                            'exception_tag_parameters': [], 'expected_test_count': count}
                manifest_path = scratch / ('manifest-' + label + '.json')
                manifest_identity = save(manifest_path, manifest)
                execution_directory = scratch / ('run-' + label)
                command([node, '--no-warnings', '--max-old-space-size=256',
                    runner_path, manifest_path], execution_directory, 45)
                result = json.loads(opened(execution_directory / 'stdout.log', MIB)[0])
                require(result.get('kind') == 'ACTUAL_COMPILED_MOONBIT_LOCAL_MODEL_EXECUTION_NOT_UWVM_OR_NATIVE_ROS'
                        and result.get('wasm') == runner_wasm and result.get('test_info') == runner_info
                        and opened(execution_directory / 'stderr.log')[0] == b'' and result.get('passed') is True
                        and result.get('model_tests') == count and result.get('ros_import_calls') == 0
                        and result.get('uwvm_execution_cases') == 0 and result.get('native_ros_delivery_cases') == 0,
                        'complete actual local model result required')
                # Reopen both products after Node; neither a historical receipt
                # nor an exit0 without the actual complete typed report suffices.
                require(opened(artifact)[1]['sha256'] == wasm['sha256']
                        and opened(Path(info['path']))[1] == info, 'model products changed during execution')
                report['modules'].append({'wasm': wasm, 'test_info': info, 'inspection': inspection,
                                          'manifest': manifest_identity, 'execution': result})
                report['model_tests_per_target'][target] += count
        require(len(report['modules']) == 2 * len(packages) and report['model_tests_per_target'] == {'wasm': expected_tests, 'wasm-gc': expected_tests}, 'exact complete selected model inventory')
        report['status'], report['passed'] = f'PASS_{expected_tests}_MODELS_EACH_TARGET_ZERO_ROS_IMPORTS', True
        report['ros_import_calls'] = 0
    except BaseException as error:
        report['error'] = {'type': type(error).__name__, 'message': str(error)}
    finally:
        # No write-back, source repair, package fetching or cleanup deletion.
        # Compare original physical inputs including mode/nlink, then leave all
        # scratch/raw evidence to the real outer process-tree owner.
        try:
            # A partially copied/admitted tree is not evidence that every
            # original input was preserved, even though the whole run failed.
            require(report['input_copy_admission_completed'],
                    'complete source/core copy admission did not finish')
            for identity in admitted + code_inputs + [row['original'] for row in source_rows + core_rows]:
                require(opened(Path(identity['path']))[1] == identity, 'original input preservation failure')
            for root, inventory in inventories:
                require(tree_inventory(root) == inventory, 'original selected input tree inventory changed')
            require(literal_identity(moon_home_source / 'bin') == bin_literal
                    and (moon_home_source / 'bin').resolve(strict=True) == bin_directory,
                    'original bin alias/physical target changed')
            for row in source_rows + core_rows:
                if row['alias'] is not None:
                    alias = row['alias']
                    require(literal_identity(Path(alias['literal']['path'])) == alias['literal']
                            and Path(alias['literal']['path']).resolve(strict=True) == Path(alias['target']),
                            'original regular-file alias changed')
            report['original_inputs_preserved'] = True
            # Actual logs/products are reopened without reusing their earlier
            # hash; this local report is not the outer owner's cleanup proof.
            product_pins = list(report.get('all_build_products', []))
            product_pins.extend(row['copied'] for row in report.get('private_tool_copies', []))
            product_pins.extend(row['copied'] for row in source_rows)
            for row in report['commands']:
                product_pins.extend(row[key] for key in ('stdout', 'stderr', 'command_report') if key in row)
            for row in report['modules']:
                product_pins.extend((row['test_info'], row['manifest']))
            keys = ('path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns', 'mode', 'nlink')
            for identity in product_pins:
                require(opened(Path(identity['path']))[1] == {key: identity[key] for key in keys},
                        'actual output changed before terminal report')
            report['actual_products_preserved'] = True
        except BaseException as error:
            report['status'], report['passed'] = 'FAIL', False
            report['preservation_error'] = {'type': type(error).__name__, 'message': str(error)}
        save(scratch / 'report.json', report)
    print(json.dumps({'report': str(scratch / 'report.json'), 'passed': report['passed'],
                      'model_tests_per_target': report['model_tests_per_target'],
                      'ros_import_calls': report['ros_import_calls'],
                      'native_ros_delivery_cases': 0, 'uwvm_execution_cases': 0}))
    return 0 if report['passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
