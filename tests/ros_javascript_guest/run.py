#!/usr/bin/env python3
"""Execute actual embedded-JavaScript Wasm with freshly qualified Workers.

This is an execution lane, not the Node bridge/relay model or the all30 build
lane. It uses the maintained current64GiB/serial/pidfd framework. Input build
reports remain build-only evidence; only a retained real Worker transcript,
exit status and independent oracle can add a Runtime result. Native ROS rows
remain separately unqualified until their real Gateway/peer adapter is run.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import select
import stat
import sys
import time
import types

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
PIN_FIELDS = ('path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns')
PURE = {'vision': 'reactor-vision', 'callbacks': 'reactor-callback',
        'motion': 'reactor-motion', 'floating': 'float_guest'}
NATIVE_IDS = {'02-node-ros1', '02-node-ros2', '08-unified-events-ros1', '08-unified-events-ros2'}
LIBRARIES = {'benchmark_guest': ('callbacks', 'vision', 'motion'),
             'benchmark_session_guest': ('callbacks', 'vision', 'motion'),
             '10-floating-modules': ('floating',)}
NONBASIC = ('realtime-clock', 'monotonic-clock', 'process-clock', 'thread-clock',
            'random', 'proc-raise', 'sched-yield', 'stdio-full-rights')


def require(value, reason):
    if not value:
        raise ValueError(reason)


def bounded(path, maximum):
    """Bootstrap an ordinary input without FIFO blocking or unbounded reads."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute bootstrap input')
    descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= maximum, 'bounded ordinary bootstrap input')
        blocks, size = [], 0
        while True:
            block = os.read(descriptor, min(65536, maximum+1-size))
            if not block:
                break
            blocks.append(block)
            size += len(block)
            require(size <= maximum, 'bootstrap input grew beyond bound')
        after = os.fstat(descriptor)
        named = path.stat(follow_symlinks=False)
        require(size == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')),
                'bootstrap input identity changed')
        return b''.join(blocks)
    finally:
        os.close(descriptor)


def frozen_module(source, manifest, relative, name):
    """Admit reviewed Python bytes before loading a helper; never PATH-import."""
    path = source / relative
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= 1024**2, 'bounded regular frozen helper')
        raw = bytearray()
        while True:
            part = os.read(descriptor, 65536)
            if not part:
                break
            raw.extend(part)
            require(len(raw) <= 1024**2, 'frozen helper grew')
        after = os.fstat(descriptor)
        named = path.stat(follow_symlinks=False)
        require(all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                    for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')),
                'frozen helper identity changed')
        require(manifest.get(relative) == {'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()},
                'helper missing from exact executing source manifest')
    finally:
        os.close(descriptor)
    module = types.ModuleType(name)
    module.__file__ = str(path)
    sys.modules[name] = module
    exec(compile(raw, str(path), 'exec'), module.__dict__)
    return module


def bootstrap(args, entry='run.py'):
    """Read only bounded metadata before any project helper can execute.

    The manifest must also bind this entrypoint, not merely its dependencies.
    The common framework reopens the manifest/config and verifies the whole
    source tree before any child process can be started.
    """
    source = args.source.resolve(strict=True)
    require(source == ROOT, 'actual complete frozen entry source')
    raw = bounded(args.manifest, 16*1024**2)
    require(0 < len(raw) <= 16*1024**2 and hashlib.sha256(raw).hexdigest() == args.manifest_sha256,
            'selected source manifest differs')
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, 'duplicate manifest key')
            result[key] = value
        return result
    frozen = json.loads(raw, object_pairs_hook=pairs,
                        parse_constant=lambda _: require(False, 'nonfinite manifest'))
    require(entry in ('run.py', 'native.py'), 'closed execution entry selection')
    for relative in ('tests/ros_javascript_guest/run.py', 'tests/ros_javascript_guest/'+entry,
                     'tests/ros_cgroup/probe.py'):
        path = source / relative
        body = bounded(path, 1024**2)
        require(frozen.get(relative) == {'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()},
                'executing entry/probe is not frozen')
    unitree = frozen_module(source, frozen, 'tests/ros_unitree/common.py', 'embedded_js_execution_owners')
    return source, frozen, unitree


def retained(frame, value):
    """Reopen every physically pinned input/product in a producer record.

    Status strings alone do not authenticate an original object, dependency,
    compiler log, source, binary or validator job. Whole roots are independently
    inventoried by Frame before and after these original records are opened.
    """
    if type(value) is dict:
        if {'path', 'bytes', 'sha256'} <= set(value):
            actual = frame.pin(value['path'])
            require(all(actual[key] == expected for key, expected in value.items() if key in PIN_FIELDS),
                    'retained producer physical input/product changed: ' + value['path'])
            for root, old in frame.bridges:
                path = Path(value['path'])
                if path.is_relative_to(root):
                    name = str(path.relative_to(root))
                    require(frame.frozen.get(name) == old.get(name) ==
                            {'bytes': actual['bytes'], 'sha256': actual['sha256']},
                            'current source changed an actually compiled source input: ' + name)
        for item in value.values():
            retained(frame, item)
    elif type(value) is list:
        for item in value:
            retained(frame, item)


def source_bridge(frame, root, manifest_label, expected_sha):
    """Keep an older build freeze separate from the executing harness freeze.

    All original bytes are reopened. Original physical source pins appearing
    in the retained full-MD/preparation records must match the current freeze;
    documentation and newly added harness files need not pretend to be original
    compiler inputs. Changed compiled bytes require a new build rather than
    changing an old report's manifest label.
    """
    manifest_path = frame.input(manifest_label)
    frozen, pin = frame.common.document(manifest_path)
    require(pin['sha256'] == expected_sha and root != Path('/') and
            root.resolve(strict=True) == root and any(root == p or root.is_relative_to(p) for p in frame.roots),
            'original complete source freeze must be explicitly inventoried')
    state = frame.common.source_state(root, manifest_path)
    frame.bridges.append((root, frozen))
    frame.report.setdefault('original_source_bridges', []).append({'source': str(root), 'manifest': pin, 'state': state})
    return frozen


def products(frame):
    """Qualify actual all30 binaries and all four native interpreter profiles."""
    spec = frame.config['benchmark']
    frame.unitree.exact(spec, ('schema', 'build_report', 'build_manifest', 'build_source',
        'worker_report', 'worker_manifest', 'worker_source', 'workers', 'validator', 'native_cases'),
        'embedded execution selection')
    require(spec['schema'] == 'uwvm-embedded-js-runtime-inputs-v1' and type(spec['native_cases']) is list and
            len(spec['native_cases']) <= 2, 'closed Runtime selection and at most two explicit native backends')
    require(frame.config['simulation'] is None and frame.config['profiler'] is None and not frame.config['upstreams'],
            'local Runtime case has no implicit physics/profiler/native upstream task')
    report, report_pin = frame.common.document(frame.input(spec['build_report']))
    require(report.get('schema') == 'uwvm-embedded-js-build-report-v1' and
            report.get('status') == 'PASS_EMBEDDED_JS_BUILD_BINARY_PREPARATION_ONLY' and
            report.get('runtime_executed') is False and report.get('native_ros_executed') is False and
            all(report.get(key) is True for key in ('source_preserved', 'physical_pins_preserved', 'whole_inputs_preserved')) and
            report.get('cleanup', {}).get('status') == 'PASS' and not report['cleanup']['remaining'] and
            not report.get('finalization_errors'), 'complete actual all30 build/binary report required')
    frame.pin(frame.input(spec['build_report']))
    source_bridge(frame, Path(spec['build_source']), spec['build_manifest'], report['source_manifest']['sha256'])
    retained(frame, report)
    recipes, _ = frame.common.document(frame.source/'sdk/javascript/guest/recipes.json')
    acceptance, _ = frame.common.document(frame.source/'sdk/javascript/guest/acceptance.json')
    frame.acceptance = {row['recipe_variant']: row for row in acceptance['variants']}
    require(len(recipes['variants']) == len(frame.acceptance) == 30 and
            {row['variant'] for row in report['products']} == set(recipes['variants']) and len(report['products']) == 30,
            'exact all30 artifact inventory required')
    result = {}
    for row in report['products']:
        variant = row['variant']
        receipt, receipt_pin = frame.common.document(Path(row['receipt']['path']))
        require(frame.pin(receipt_pin['path']) == row['receipt'] and
                receipt['schema'] == 'uwvm-embedded-js-actual-build-receipt-v1' and
                receipt['status'] == 'PASS_BUILD_AND_BINARY_ONLY' and receipt['variant'] == variant and
                receipt['source_manifest_sha256'] == report['source_manifest']['sha256'] and
                receipt['artifact'] == row['artifact'] and receipt['boundaries'] == row['boundaries'],
                'exact actual artifact/receipt/source association required')
        # The original compiler's full-MD paths belong to the source actually
        # selected by its preparation child. A byte-identical directory copy
        # is not interchangeable with that physical root: otherwise a bridge
        # could overlook changed current files named by the original MD list.
        preparation_argv = receipt['preparation_job']['argv']
        build_source = Path(spec['build_source'])
        require(type(preparation_argv) is list and len(preparation_argv) >= 3 and
                preparation_argv[1:3] == ['-B', str(build_source/'sdk/javascript/guest/prepare.py')] and
                preparation_argv.count('--source-root') == preparation_argv.count('--manifest') == 1,
                'actual preparation executable/source selectors differ')
        source_index = preparation_argv.index('--source-root')
        manifest_index = preparation_argv.index('--manifest')
        require(source_index+1 < len(preparation_argv) and manifest_index+1 < len(preparation_argv) and
                preparation_argv[source_index+1] == str(build_source) and
                preparation_argv[manifest_index+1] == str(build_source/'sdk/javascript/guest/recipes.json'),
                'actual preparation source/recipe roots differ from the retained bridge')
        retained(frame, receipt)
        prepared, prepared_pin = frame.common.document(Path(receipt['preparation']['path']))
        require(prepared_pin == receipt['preparation'] and prepared['status'] == 'PREPARED_NOT_COMPILED' and
                prepared['manifest'] == {**recipes['common'], **recipes['variants'][variant]} and
                Path(prepared['artifact']) == Path(row['artifact']['path']),
                'original preparation manifest/artifact association differs')
        retained(frame, prepared)
        require(frame.pin(row['artifact']['path']) == row['artifact'], 'actual binary bytes changed')
        result[variant] = Path(row['artifact']['path'])
    workers, workers_pin = frame.common.document(frame.input(spec['worker_report']))
    require(workers.get('kind') == 'native-interpreter-four-profile-build' and
            workers.get('status') == 'PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED' and
            workers.get('runtime_backend') == 'uwvm-interpreter' and workers.get('no_reuse') is True and
            workers.get('reused_products') == 0 and
            all(workers.get(key) is True for key in ('source_preserved', 'inputs_preserved', 'products_preserved')) and
            all(workers.get(key) is False for key in ('Worker_executed', 'Guest_executed', 'native_ROS_executed', 'Guest_permissions_granted')) and
            workers.get('cleanup', {}).get('status') == 'PASS' and not workers['cleanup']['remaining'] and
            not workers.get('finalization_errors'), 'actual native4 build provenance required')
    require(workers.get('source') == spec['worker_source'],
            'native4 full-MD source must be the explicitly bridged physical root')
    source_bridge(frame, Path(spec['worker_source']), spec['worker_manifest'], workers['source_manifest_sha256'])
    retained(frame, workers)
    frame.unitree.exact(spec['workers'], ('none', 'ros1', 'ros2', 'dual'), 'exact Worker profile inputs')
    require([row['profile'] for row in workers['profiles']] == ['none', 'ros1', 'ros2', 'dual'] and
            all(row['status'] == 'BUILT_NOT_EXECUTED' and row['full_dependencies_verified'] is True and
                row['no_reuse'] is True and row['reused_products'] == 0 for row in workers['profiles']),
            'all four real full-MD linked native profiles required')
    frame.workers = {}
    for row in workers['profiles']:
        path = frame.input(spec['workers'][row['profile']])
        require(frame.pin(path) == row['binary'] and os.access(path, os.X_OK), 'actual selected native Worker differs')
        with path.open('rb') as stream:
            header = stream.read(64)
        require(header[:6] == b'\x7fELF\x02\x01' and int.from_bytes(header[18:20], 'little') == 62,
                'actual native4 x86_64 ELF required; this is not a QEMU qualification')
        frame.workers[row['profile']] = path
    frame.validator = frame.input(spec['validator'])
    frame.report['build_provenance'] = {'all30_report': report_pin, 'native4_report': workers_pin,
        'actual_Runtime_execution_previously_claimed': False}
    frame.manifests = {name: {**recipes['common'], **row} for name, row in recipes['variants'].items()}
    frame.abi, _ = frame.common.document(frame.source/'sdk/abi/abi-v1.json')
    frame.inspector = frozen_module(frame.source, frame.frozen, 'sdk/tools/check_wasm_imports.py',
                                    'embedded_js_execution_binary_reader')
    # Validation is deliberately an actual separate child. The independent
    # Reader checks the bytes that are subsequently reopened before execution.
    for variant, path in sorted(result.items()):
        before = frame.pin(path)
        tool(frame, variant+'-current-validate', [str(frame.validator), 'validate', str(path)])
        require(frame.pin(path) == before, 'binary changed during current validation')
        frame.report.setdefault('current_binary_boundaries', {})[variant] = boundary(frame, path, frame.manifests[variant])
    return result


def boundary(frame, path, manifest):
    """Independently inspect exact actual core signatures/memory/exports.

    Unknown encodings refuse; full validation is an independent child. Neither
    source recipe names nor the old producer's boundary summary supply this
    observation. Only ordinary bounded32-bit memories/functions are admitted.
    """
    data, pin = frame.common.body_pin(path, 64*1024**2)
    require(frame.pin(path) == pin, 'actual binary changed between hash and parse')
    rows = frame.inspector.inspect(data)
    expected = {}
    core = lambda value: 'i32' if value == 'u32' else 'i64' if value == 'u64' else value
    if manifest['backend']:
        module = 'uwvm_ros%d_v1' % manifest['backend']
        for row in frame.abi['modules'][module]['functions']:
            expected[(module, row['name'])] = ([p['type'] for p in row['params']], row['results'])
        expected.update({('wasi_snapshot_preview1', 'fd_read'): (['i32']*4, ['i32']),
                         ('wasi_snapshot_preview1', 'fd_write'): (['i32']*4, ['i32'])})
    if manifest['mode'] == 'command':
        expected[('wasi_snapshot_preview1', 'proc_exit')] = (['i32'], [])
    for row in manifest['imports']:
        key = row['module'], row['name']
        require(key not in expected, 'scalar import collision')
        expected[key] = ([core(value) for value in row['parameters']], [core(row['result'])])
    require(len(rows) == len(expected) and len({(r['module'], r['name']) for r in rows}) == len(rows),
            'actual current binary import count/uniqueness differs')
    for row in rows:
        require(row['kind'] == 0 and (row['params'], row['results']) == expected.get((row['module'], row['name'])),
                'actual current binary imported signature differs')
    reader = frame.inspector.Reader
    root = reader(data)
    root.take(8)
    signatures, functions, exports, memory, seen = [], [], {}, [], set()
    while root.position < len(root.data):
        section = root.byte()
        body = reader(root.take(root.uint()))
        if section:
            require(section not in seen, 'duplicate actual core section')
            seen.add(section)
        if section == 1:
            for _ in range(body.count()):
                require(body.byte() == 0x60, 'ordinary core function type required')
                signatures.append(([body.value() for _ in range(body.count())], [body.value() for _ in range(body.count())]))
            body.end()
        elif section == 3:
            functions = [body.uint() for _ in range(body.count())]
            body.end()
        elif section == 5:
            for _ in range(body.count()):
                require(body.uint() == 1, 'bounded unshared32-bit actual memory required')
                memory.append((body.uint(), body.uint()))
            body.end()
        elif section == 7:
            for _ in range(body.count()):
                name, kind, index = body.name(), body.byte(), body.uint()
                require(name not in exports, 'duplicate actual export')
                exports[name] = (kind, index)
            body.end()
        elif section == 8:
            require(False, 'Wasm start section forbidden')
    require(memory == [(manifest['initial_memory']//65536, manifest['maximum_memory']//65536)], 'actual memory bounds differ')
    wanted = {
        'roswasm_js_init': ([], ['i32']), 'roswasm_js_status': ([], ['i32']),
        'roswasm_js_pump': (['i32'], ['i32']), 'roswasm_js_close': ([], ['i32'])}
    if manifest['mode'] == 'command':
        wanted['_start'] = ([], [])
    for row in manifest['exports']:
        wanted[row['name']] = ([core(value) for value in row['parameters']], [core(row['result'])])
    require(set(exports) == {'memory', *wanted}, 'actual export inventory differs')
    require(exports['memory'] == (2, 0), 'actual defined memory0 export required')
    imported = [(row['params'], row['results']) for row in rows]
    for name, signature in wanted.items():
        kind, index = exports[name]
        require(kind == 0, 'actual function export required')
        if index < len(imported):
            actual = imported[index]
        else:
            ordinal = index-len(imported)
            require(ordinal < len(functions) and functions[ordinal] < len(signatures), 'actual function export index invalid')
            actual = signatures[functions[ordinal]]
        require(actual == signature, 'actual exported signature differs')
    return {'artifact': pin, 'imports': rows, 'exports': sorted(exports), 'memory_pages': memory,
            'start_section': False, 'actual_validator': True}


def runtime_argv(frame, profile, guest, libraries=(), application=None):
    """Construct the entire empty ROS policy; accept no caller grant argv."""
    versions = (1,) if profile == 'ros1' else (2,) if profile == 'ros2' else (1, 2) if profile == 'dual' else ()
    argv = [str(frame.workers[profile]), '--runtime-int', '-Rct', '0',
            '--wasm-set-main-module-name', 'main', '--wasip1-global-disable',
            '--wasip1-global-noinherit-system-environment']
    for version in versions:
        argv.append('--ros%d-global-disable' % version)
    modules = [('main', True)] + [(name, False) for name, _ in libraries]
    if application is not None:
        modules.append(('application', True))
    for module, basic in modules:
        argv += ['--wasip1-single-create', module,
                 '--wasip1-single-enable' if basic else '--wasip1-single-disable', module,
                 '--wasip1-single-hide-host-api', module, '--wasip1-single-noinherit-system-environment', module]
        for right in NONBASIC:
            argv += ['--wasip1-single-deny', module, right]
        for version in versions:
            argv += ['--ros%d-single-create' % version, module, '--ros%d-single-disable' % version, module]
    for name, artifact in libraries:
        argv += ['--wasm-preload-library', str(artifact), name]
    if application is not None:
        argv += ['--wasm-preload-library', str(application), 'application']
    argv += ['--run', str(guest)]
    require(not any(token.endswith(('-grant', '-set', '-mount', '-add-environment')) for token in argv),
            'local runner cannot add rights')
    return argv


def terminal(frame, owner):
    """Refuse a new serial case if any owned or unproven live birth remains."""
    owner.owner.discover()
    require(all(frame.common.pidfd_terminal(item['fd']) for item in owner.owner.owned.values()),
            'owned process survived a completed serial Runtime case')
    for key, original in owner.owner.unproven.items():
        now = frame.common.identity(original['pid'])
        require(frame.common.birth(now) != key or now['state'] == 'Z', 'unproven live birth remains')


def finish_actor(frame, owner, child, expected=0, serial=True):
    while child.poll() is None:
        owner.check()
        time.sleep(.005)
    owner.check()
    row = next(row for actor, row in owner.rows if actor is child)
    row.update(returncode=child.returncode, status='PASS' if child.returncode == expected else 'FAIL',
               expected_returncode=expected, finished_monotonic_ns=time.monotonic_ns())
    require(child.returncode == expected, 'actual Runtime exit differs: ' + row['name'])
    if serial:
        terminal(frame, owner)
    frame.save()
    return row


def executable_proof(frame, owner, pid, selected):
    """Prove the owned live /proc/exe through its actual kernel descriptor.

    The main gate has already written its exact marker and is blocked on G.
    A procfs executable FD is intentional; ordinary selected paths still use
    O_NOFOLLOW. No raw maps device is compared to stat, and no signal is sent.
    """
    owner.check()
    matches = [item for key, item in owner.owner.owned.items() if key[0] == pid]
    require(len(matches) == 1, 'one retained owned Worker pidfd required')
    item = matches[0]
    def live():
        frame.guard()
        now = frame.common.identity(pid)
        require(frame.common.birth(now) == frame.common.birth(item['identity']) and
                not frame.common.pidfd_terminal(item['fd']) and
                Path('/proc/%d/cgroup' % pid).read_text() == Path('/proc/self/cgroup').read_text(),
                'Worker birth/pidfd/cgroup changed before execution gate')
        return now
    initial = live()
    expected = frame.pin(selected)
    fd = os.open('/proc/%d/exe' % pid, os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and all(getattr(before, attr) == expected[key]
                for attr, key in (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                                  ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))),
                'actual current executable physical identity differs')
        digest = hashlib.sha256()
        size = 0
        while True:
            live()
            block = os.read(fd, 1048576)
            if not block:
                break
            size += len(block)
            require(size <= expected['bytes'], 'actual executable grew')
            digest.update(block)
        after = os.fstat(fd)
        require(size == expected['bytes'] and digest.hexdigest() == expected['sha256'] and
                all(getattr(before, key) == getattr(after, key) for key in
                    ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')),
                'complete actual executable bytes changed')
        final = live()
    finally:
        os.close(fd)
    return {'protocol': 'owned-current-executable-full-fd-v1', 'first': initial, 'last': final,
            'selected': expected, 'pidfd_live': True, 'same_cgroup': True, 'no_stop_or_signal': True}


def worker_proof(frame, owner, child, selected):
    return executable_proof(frame, owner, child.pid, selected)


def tool(frame, label, argv):
    """One actual validator/translator, terminal proof and FD retirement."""
    require(argv[0] == str(frame.validator), 'only selected validator tool admitted')
    runner = frame.tools_runner
    row = runner.run(label, argv, frame.out, frame.environment)
    frame.report['commands'].append(row)
    runner.discover()
    require(all(frame.common.pidfd_terminal(item['fd']) for item in runner.owned.values()),
            'validator child remains live')
    for key, original in runner.unproven.items():
        now = frame.common.identity(original['pid'])
        require(frame.common.birth(now) != key or now['state'] == 'Z', 'unproven validator descendant remains')
    row['retired_terminal_identities'] = []
    for key, item in list(runner.owned.items()):
        require(frame.common.pidfd_terminal(item['fd']), 'validator terminal changed')
        row['retired_terminal_identities'].append(item['identity'])
        os.close(item['fd'])
        del runner.owned[key]
    frame.save()
    return row


def translate(frame, label, wat):
    path = frame.out/(label + '.wat')
    path.write_text(wat, encoding='utf-8')
    frame.pin(path)
    binary = frame.out/(label + '.wasm')
    for suffix, argv in (('parse', [str(frame.validator), 'parse', str(path), '-o', str(binary)]),
                         ('validate', [str(frame.validator), 'validate', str(binary)])):
        tool(frame, label+'-'+suffix, argv)
    require(frame.pin(binary)['bytes'] <= 65536, 'small independent oracle binary bound')
    return binary


def local(frame, artifacts):
    """Run every local candidate; four actual native ROS rows remain pending."""
    fixture = frozen_module(frame.source, frame.frozen, 'tests/ros_javascript_guest/fixtures.py', 'embedded_js_real_callers')
    gate = translate(frame, 'command-gate', fixture.command_gate())
    pure = translate(frame, 'pure-lifecycle', fixture.pure_lifecycle())
    owner = frame.unitree.Actors(frame)
    libraries = [(name, artifacts[variant]) for name, variant in PURE.items()]
    child, _ = owner.start('pure-lifecycle', runtime_argv(frame, 'none', pure, libraries), json_output=False)
    require(owner.receive_bytes(child, len(fixture.GATE)) == fixture.GATE.encode(), 'actual independent main gate missing')
    proof = worker_proof(frame, owner, child, frame.workers['none'])
    owner.write(child, b'G')
    require(owner.receive_bytes(child, len(fixture.PURE_PASS)) == fixture.PURE_PASS.encode(), 'pure JS lifecycle oracle failed')
    row = finish_actor(frame, owner, child)
    require(Path(row['stdout']).read_bytes() == (fixture.GATE+fixture.PURE_PASS).encode(), 'pure oracle output differs')
    frame.report['cases'].append({'case': 'pure-lifecycle', 'variants': list(PURE.values()),
        'status': 'PASS_ACTUAL_RUNTIME', 'worker': proof, 'zero_ROS_and_WASI_library_imports': True,
        'native_ROS_qualified': False, 'oracle': frame.pin(pure)})
    command_ids = set(artifacts) - set(PURE.values()) - NATIVE_IDS
    require(len(command_ids) == 22, 'exact local22 command and4 pure variants required')
    selections = [(variant, 'ros%d' % frame.acceptance[variant]['backend']) for variant in sorted(command_ids)]
    # Actual dual execution is an additional profile check, not two new recipe
    # products. Both real ABI callers retain empty ROS rights in this profile.
    selections += [('00-raw-ros1', 'dual'), ('00-raw-ros2', 'dual')]
    for variant, profile in selections:
        plan = frame.acceptance[variant]
        selected = [(name, artifacts[PURE[name]]) for name in LIBRARIES.get(variant, ())]
        label = variant+'-'+profile
        child, _ = owner.start(label, runtime_argv(frame, profile, gate, selected, artifacts[variant]), json_output=False)
        require(owner.receive_bytes(child, len(fixture.GATE)) == fixture.GATE.encode(), 'actual command gate missing')
        proof = worker_proof(frame, owner, child, frame.workers[profile])
        owner.write(child, b'G')
        output = plan['stdout']
        frames = []
        if output['kind'] == 'session':
            require(owner.receive_bytes(child, 6) == b'READY\n', 'actual retained JS session did not initialize')
            for command in (b'W', b'P', b'P'):
                owner.write(child, command)
                observed = owner.receive_bytes(child, 8)
                require(observed == fixture.checksum(), 'actual persistent JS modular checksum differs')
                frames.append({'command': command.decode(), 'checksum_le_hex': observed.hex()})
            owner.write(child, b'Q')
            expected = b'READY\n' + fixture.checksum()*3
        elif output['kind'] == 'binary':
            expected = fixture.checksum()
        elif output['kind'] == 'exact':
            expected = output['text'].encode()
        else:
            require(output['kind'] == 'marker_lines', 'closed actual command stdout protocol')
            expected = output['final'].encode()
        row = finish_actor(frame, owner, child, plan['exit'])
        raw = Path(row['stdout']).read_bytes()
        require(raw == fixture.GATE.encode()+expected, 'exact actual JS command output differs: ' + variant)
        require(not Path(row['stderr']).read_bytes(), 'unexpected Runtime/Guest stderr: ' + variant)
        frame.report['cases'].append({'case': label, 'variants': [variant], 'status': 'PASS_ACTUAL_RUNTIME', 'worker_profile': profile,
            'worker': proof, 'artifact': frame.pin(artifacts[variant]), 'exit': row['returncode'],
            'stdout': frame.pin(row['stdout']), 'session_frames': frames, 'ROS_grants': [],
            'native_ROS_qualified': False,
            'scope': 'Actual Wasm/guest-local or protected default-denial only; no independent native-effect claim'})
        frame.save()
    covered = {variant for row in frame.report['cases'] for variant in row['variants']}
    require(covered == set(artifacts)-NATIVE_IDS and len(covered) == 26, 'exact26 local Runtime variants required')
    frame.report.update(actual_Runtime_variants=sorted(covered),
        pending_native_variants=sorted(NATIVE_IDS), actual_Runtime_execution=True, actual_native_ROS_execution=False,
        native_zero_effect_witness_qualified=False, all30_Runtime_qualified=False,
        actual_Runtime_worker_profiles=['none', 'ros1', 'ros2', 'dual'])


def new_frame(args, entry='run.py'):
    """Share the exact maintained owner with the separate native adapter."""
    source, frozen, unitree = bootstrap(args, entry)
    class RuntimeFrame(unitree.Frame):
        def pin(self, path):
            path = Path(path)
            require(path.is_absolute() and path.resolve(strict=True) == path and
                    (path.is_relative_to(self.source) or path.is_relative_to(self.out) or
                     any(path == root or path.is_relative_to(root) for root in self.roots)),
                    'physical input/product must belong to a whole inventoried root')
            actual = self.common.opened(path)
            if not hasattr(self, 'runtime_pins'):
                self.runtime_pins = {}
            previous = self.runtime_pins.get(str(path))
            require(previous is None or actual == previous, 'previous physical pin changed')
            if previous is None:
                self.pins.append(actual)
                self.runtime_pins[str(path)] = actual
            return actual
        def guard(self):
            require(getattr(self, 'finalizing', False) or not getattr(self, 'cancelled', {}).get('cancelled'),
                    'cooperative cancellation requested')
            super().guard()
        def finish(self, status, error=None):
            self.finalizing = True
            return super().finish(status, error)
    frame = RuntimeFrame(args, 'embedded-js-actual-runtime-local' if entry == 'run.py' else 'embedded-js-actual-runtime-native')
    frame.unitree, frame.frozen, frame.bridges = unitree, frozen, []
    frame.tools_runner = frame.common.SerialRunner(frame.out, frame.limits, frame.guard)
    frame.actors.append(types.SimpleNamespace(close=frame.tools_runner.close))
    frame.report.update(cases=[], source_prepared_only=False, actual_native_ROS_execution=False,
        scope='Actual Wasm observations; local computation, native ROS and independent zero-effect witnesses remain distinct')
    return frame


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'manifest', 'config', 'out'):
        parser.add_argument('--'+name, type=Path, required=True)
    for name in ('manifest-sha256', 'config-sha256'):
        parser.add_argument('--'+name, required=True)
    args = parser.parse_args()
    frame = None
    try:
        frame = new_frame(args)
        require(frame.limits['rss_bytes'] <= 512*1024**2 and frame.limits['command_seconds'] <= 600,
                'local JS Runtime lane retains512MiB/600s actor limits')
        local(frame, products(frame))
        return frame.finish('PASS_EMBEDDED_JS_LOCAL_RUNTIME_ONLY')
    except BaseException as error:
        if frame is not None:
            return frame.finish('FAIL', repr(error))
        raise


if __name__ == '__main__':
    raise SystemExit(main())
