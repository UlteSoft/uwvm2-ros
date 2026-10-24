#!/usr/bin/env python3
"""Build real relocated QuickJS SDK applications and run bounded Wasm cases.

Use only the finite Linux owner. The independent SDK contains exact production
sources; select the integrated public producer or an explicitly reviewed
historical optional-recipe proposal. External tools
are pinned complete inputs. No Node evaluator, ROS stub, fake engine, native
identity or repository test enters the SDK application graph. Complete command
records are data unless a trusted Host separately authorizes actual transport;
no physical robot is used by these recording/default-denial checks.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def pin(path, relative=None):
    path = path.resolve(strict=True)
    return dict(path=path.relative_to(relative).as_posix() if relative else str(path),
                bytes=path.stat().st_size, sha256=sha(path))


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def frames(output):
    result, cursor = [], 0
    while cursor < len(output):
        require(len(output) - cursor >= 5, 'actual Guest output frame header is truncated')
        tag, size = struct.unpack_from('<BI', output, cursor)
        cursor += 5
        require(size <= len(output) - cursor, 'actual Guest output body is truncated')
        result.append((tag, output[cursor:cursor + size]))
        cursor += size
    return result


def primary_code(record):
    if not isinstance(record, dict):
        return None
    code = record.get('code')
    if type(code) is int and 0 <= code <= 0xffffffff:
        return code
    children = record.get('errors')
    if isinstance(children, list) and children:
        return primary_code(children[0])
    return primary_code(record.get('cause'))


def failure_matches(output, code, framed=False, prefix=None):
    if framed:
        records = []
        for tag, body in frames(output):
            if tag == 68:
                value = json.loads(body)
                if isinstance(value, dict) and 'failure' in value:
                    records.append(value['failure'])
        return len(records) == 1 and primary_code(records[0]) == code
    marker = b'roswasm_failure='
    position = output.rfind(marker)
    if position < 0 or prefix is not None and output[:position] != prefix:
        return False
    return primary_code(json.loads(output[position + len(marker):])) == code


def dobot_guests(args, artifact, run, guest):
    """Check the actual new Wasm with full pinned original table input, not ROS."""
    expected_tables = {
        'v3/alarm_controller.json': '28ae41285187582c57a2647b3cfada6adb87217a04b9eae01443bc7211b5fb8b',
        'v3/alarm_servo.json': 'd1a7152a0a42e467690886615869c291588d1ddc2127c1f657864feb2ea7014d',
        'v4/alarm_controller.json': 'f7b369a64bf1f00ec8320d23ca8333a298e12867476bc1fe19cd3c47ce97a310',
        'v4/alarm_servo.json': '19e577366d8568d5285ef8f7f1a6b262b3f8b53d302d5328916f5377d0bc01d8'
    }
    tables = {}
    table_pins = []
    for name, digest in expected_tables.items():
        path = args.alarms / name
        require(sha(path) == digest, 'exact whole original DOBOT alarm file required')
        table_pins.append(pin(path))
        tables[name] = path.read_text()
    save(args.out / 'dobot-alarm-inputs.json', table_pins)
    points = [[0, 0, 0, 0, 0, 0], [0.5, 0, 0.5, 0, 0, 0]]

    def event(tag, body=b''):
        return bytes([ord(tag)]) + struct.pack('<I', len(body)) + body

    def packet(mode, configuration, events):
        raw = json.dumps(configuration, ensure_ascii=False, allow_nan=False, separators=(',', ':')).encode()
        return struct.pack('<BII', mode, len(events), len(raw)) + raw + b''.join(events)

    def feedback(version, point, command, error=0, enabled=1, queued=1, mode=5):
        text = json.dumps(dict(EnableStatus=enabled, ErrorStatus=error, RunQueuedCmd=queued,
                               QactualVec=point, currentCommandId=command, robot_mode=mode,
                               unobserved=dict(complete=['original String', 17])),
                          ensure_ascii=False, allow_nan=False, separators=(',', ':')).encode()
        return event('F', struct.pack('<I', len(text)) + text)

    def alarm_response(version, ids, result=-17):
        array = struct.pack('<I', len(ids)) + b''.join(struct.pack('<i', value) for value in ids)
        return event('E', struct.pack('<i', result) + array if version == 3 else array + struct.pack('<i', result))

    def commands(output, tag):
        return [body for kind, body in frames(output) if kind == ord(tag)]

    def complete_cycle(output, version):
        wanted = [(b'' if version == 3 else b'\x01') + struct.pack('<6dI', *point, 0) for point in points]
        diagnostics = [json.loads(body) for body in commands(output, 'D')]
        return commands(output, 'M') == wanted and bool(diagnostics) and diagnostics[-1].get('done') is True

    for version in (3, 4):
        configuration = dict(version=version, cycles=1, points=points,
                             controllerJSON=tables['v' + str(version) + '/alarm_controller.json'],
                             servoJSON=tables['v' + str(version) + '/alarm_servo.json'])
        move_results = (10, 11)
        simple = []
        for index in range(2):
            simple += [event('M', struct.pack('<i', move_results[index])),
                       feedback(version, points[index], move_results[index])]
        run('dobot-v' + str(version) + '-whole-two-point-cycle', guest(artifact),
            'ACTUAL_GUEST_COMPLETE_DOBOT_RECORDING', 0, packet(0, configuration, simple),
            checker=lambda out, err, v=version: complete_cycle(out, v))
        controller = json.loads(configuration['controllerJSON'])
        servo = json.loads(configuration['servoJSON'])
        first = controller[0]['id']
        second = next(row['id'] for row in servo if row['id'] != first)
        ids = [second, first, first]

        def describe(values):
            text = ''
            for identifier in values:
                for label, table in (('Control', controller), ('Servo', servo)):
                    row = next((row for row in table if row['id'] == identifier), None)
                    if row:
                        text += label + ' ErrorID : ' + str(identifier) + ',  ' + row['zh_CN']['description'] + ', ' + row['en']['description'] + '\n'
                        break
            return text.encode()

        workflow = [event('M', struct.pack('<i', 10)),
                    feedback(version, [9] * 6, 9, error=1, enabled=1 if version == 3 else 0, queued=0, mode=4),
                    event('T', struct.pack('<Q', 0)), alarm_response(version, ids)]
        expected_alarm = describe(ids)
        if version == 3:
            workflow += [feedback(version, [9] * 6, 9, error=0, queued=0, mode=4),
                         event('T', struct.pack('<Q', 5000000000)), event('C', struct.pack('<i', -27))]
        else:
            workflow += [event('T', struct.pack('<Q', 5000000000)), alarm_response(version, [first, second, first]),
                         event('T', struct.pack('<Q', 10000000000)), alarm_response(version, [first, second])]
            expected_alarm += describe([first, second])
        workflow += [feedback(version, points[0], 10), event('M', struct.pack('<i', 11)), feedback(version, points[1], 11)]

        def workflow_matches(output, error, v=version, expected=expected_alarm):
            rows = frames(output)
            error_responses = [json.loads(body) for tag, body in rows if tag == 68 and 'errorResponse' in json.loads(body)]
            return complete_cycle(output, v) and b''.join(commands(output, 'O')) == expected and \
                commands(output, 'E') == [b''] * (1 if v == 3 else 3) and \
                commands(output, 'C') == ([b''] if v == 3 else []) and \
                all(row['errorResponse']['res'] == -17 for row in error_responses) and \
                (v == 3 or [row['suppressed'] for row in error_responses] == [False, True, False])

        run('dobot-v' + str(version) + '-whole-alarm-and-source-branches', guest(artifact),
            'ACTUAL_GUEST_COMPLETE_DOBOT_RECORDING', 0, packet(0, configuration, workflow), checker=workflow_matches)
        run('dobot-v' + str(version) + '-whole-move-response-trailing-refusal', guest(artifact),
            'ACTUAL_GUEST_DOBOT_NUMERIC_REFUSAL', 'NONZERO_SHARED_ENGINE_FAILURE',
            packet(0, configuration, [event('M', bytes(5))]), checker=lambda out, err:
            failure_matches(out, 90, framed=True) and len(commands(out, 'M')) == 1)
        unprinted = b'{"EnableStatus":1,"unobserved":{"value":1e400}}'
        bad_feedback = event('F', struct.pack('<I', len(unprinted)) + unprinted)
        run('dobot-v' + str(version) + '-unobserved-JSON-overflow-refusal', guest(artifact),
            'ACTUAL_GUEST_DOBOT_NUMERIC_REFUSAL', 'NONZERO_SHARED_ENGINE_FAILURE',
            packet(0, configuration, [event('M', struct.pack('<i', 10)), bad_feedback]), checker=lambda out, err:
            failure_matches(out, 22, framed=True) and len(commands(out, 'M')) == 1)
        run('dobot-v' + str(version) + '-local-cancel', guest(artifact),
            'ACTUAL_GUEST_DOBOT_NUMERIC_REFUSAL', 'NONZERO_SHARED_ENGINE_FAILURE',
            packet(0, configuration, [event('X')]), checker=lambda out, err:
            failure_matches(out, 125, framed=True) and len(commands(out, 'M')) == 1)
        roles = ('feed', 'move', 'error', 'clock', 'continue') if version == 3 else ('feed', 'move', 'error', 'clock')
        aliases = {name: 'deny-dobot-' + name for name in roles}
        native = dict(configuration, aliases=aliases)
        run('dobot-v' + str(version) + '-live-default-denial', guest(artifact),
            'ACTUAL_GUEST_DOBOT_DEFAULT_DENIAL', 'NONZERO_SHARED_ENGINE_FAILURE',
            packet(1, native, [event('P')]), checker=lambda out, err:
            failure_matches(out, 13, framed=True) and not any(commands(out, tag) for tag in ('M', 'E', 'C', 'O')))
        invalid = dict(configuration, aliases=dict(aliases, move=aliases['feed']))
        run('dobot-v' + str(version) + '-bad-aliases-before-open', guest(artifact),
            'ACTUAL_GUEST_FRONT_LOADED_VALIDATION', 'NONZERO_SHARED_ENGINE_FAILURE',
            packet(1, invalid, [event('P')]), checker=lambda out, err:
            failure_matches(out, 22, framed=True) and not any(commands(out, tag) for tag in ('M', 'E', 'C', 'O')))
        if version == 3:
            prefix = b'Finite virtual-address prerequisite: 32768 MiB; physical/RSS limits are separate\n'
            run('xmake-dobot-process', [args.xmake, 'sdk-example-run',
                '--receipt=' + str(args.out / 'dobot-output/build-receipt.json'), '--runtime=' + str(args.runtime)],
                'ACTUAL_XMAKE_GUEST_DOBOT', 0, packet(0, configuration, simple),
                checker=lambda out, err, p=prefix: out.startswith(p) and complete_cycle(out[len(p):], 3))
    for row in table_pins:
        require(pin(Path(row['path'])) == row, 'complete original alarm input changed')


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('source', 'quickjs',
                 'wasi-sdk', 'toolchain-receipt', 'validator', 'runtime', 'xmake', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--fixtures', type=Path)
    parser.add_argument('--dobot-only', action='store_true')
    parser.add_argument('--alarms', type=Path)
    args = parser.parse_args()
    require((args.dobot_only and args.alarms is not None) or
            (not args.dobot_only and args.alarms is None and args.fixtures is not None),
            'select canonical DOBOT/alarm inputs or the canonical five-application batch/fixtures')
    require(sys.platform == 'linux' and os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') == '1',
            'same admitted finite Linux owner required')
    require(sha(args.runtime) == '719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19',
            'exact frozen Runtime required')
    args.out.mkdir()
    source = args.source / 'sdk'
    assembled = args.out / 'assembled-sdk'
    assembled.mkdir()
    assembly = []
    cases = []

    def copy(original, relative):
        target = assembled / relative
        require(original.is_file() and not original.is_symlink(), 'ordinary actual production source required: ' + str(original))
        require(not target.exists(), 'duplicate installation output: ' + relative)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(original, target)
        assembly.append(dict(source=pin(original), destination=relative, sha256=sha(target)))

    copy(source / 'xmake.lua', 'xmake.lua')
    copy(source / 'example/xmake.lua', 'example/xmake.lua')
    patch_rows = []
    platforms = ('dobot',) if args.dobot_only else ('elephant', 'ufactory', 'flexiv', 'realman')
    selected_roots = ['tools', 'share/abi', 'include/platform/ros/js',
                      'example/platform/ros/js', 'src/platform/ros/js', 'lib/platform/ros/js']
    for platform in platforms:
        selected_roots.extend(('include/platform/' + platform + '/js',
                               'example/platform/' + platform + '/js'))
    for relative_root in selected_roots:
        base = source / relative_root
        for original in sorted(base.rglob('*')):
            require(not original.is_symlink(), 'canonical installed sources must be ordinary files')
            if original.is_file():
                copy(original, original.relative_to(source).as_posix())
    for relative in ('tools/shared_managed04/profile.py', 'tools/xmake/examples/managed_backend.py',
                     'example/platform/ros/js/javascript_telemetry/build.py'):
        require((assembled / relative).is_file(), 'canonical shared dependency is unavailable: ' + relative)
        patch_rows.append(dict(source=relative, source_sha256=sha(source / relative),
                               source_mode='CANONICAL_PUBLIC_SOURCE_NO_OVERRIDE', live_shared_source_edited=False))
    sdk = args.out / 'Relocated SDK with Unicode-λ'
    assembled.rename(sdk)
    require(not any(set(path.relative_to(sdk).parts) & {'origin', 'test', 'tests', '.xmake'} for path in sdk.rglob('*')),
            'no origin, test or inherited cache may enter the installed SDK')
    for name in ('configuration', 'global', 'temporary', 'commands'):
        (args.out / name).mkdir()
    environment = dict(os.environ, XMAKE_PROGRAM_DIR='/home/macromodel/.local/share/xmake',
                       XMAKE_STATS='false', XMAKE_SKIP_HISTORY='y', XMAKE_CONFIGDIR=str(args.out / 'configuration'),
                       XMAKE_GLOBALDIR=str(args.out / 'global'), XMAKE_TMPDIR=str(args.out / 'temporary'),
                       TMPDIR=str(args.out / 'temporary'), PYTHONDONTWRITEBYTECODE='1')

    def run(name, command, layer, expected=0, data=None, wanted=None, checker=None):
        folder = args.out / 'commands' / name
        folder.mkdir()
        if data is not None:
            (folder / 'input.bin').write_bytes(data)
        with (folder / 'stdout').open('xb') as out, (folder / 'stderr').open('xb') as err:
            result = subprocess.run(list(map(str, command)), cwd=sdk / 'example', env=environment,
                                    input=data, stdout=out, stderr=err, timeout=900, check=False)
        output = (folder / 'stdout').read_bytes()
        good = result.returncode != 0 if expected == 'NONZERO_SHARED_ENGINE_FAILURE' else result.returncode == expected
        if wanted is not None:
            good = good and output == wanted
        if checker is not None:
            good = good and checker(output, (folder / 'stderr').read_bytes())
        row = dict(name=name, layer=layer, argv=list(map(str, command)), exit=result.returncode,
                   expected_exit=expected, status='PASS' if good else 'FAIL',
                   stdout=pin(folder / 'stdout'), stderr=pin(folder / 'stderr'),
                   input=pin(folder / 'input.bin') if data is not None else None)
        cases.append(row)
        save(args.out / 'cases.json', cases)
        print(json.dumps({key: row[key] for key in ('name', 'layer', 'exit', 'status')}), flush=True)
        require(good, 'actual installed build/Guest outcome differs: ' + name)

    runtime = dict(name='QuickJS', version='2026-06-04', route='js-quickjs', mode='static-built-in-ros-bridge',
                   language='js', address_bits=32, target='wasm32-wasip1', startup='command', exceptions=False,
                   default_authority=[], source_ready=True, compiled=False, runtime_tested=False)
    tools = [pin(path) for path in (args.validator, args.toolchain_receipt)]
    tools += [pin(path) for path in sorted(args.quickjs.rglob('*')) if path.is_file()]
    inputs = sdk / 'share/quickjs-inputs.json'
    save(inputs, dict(format='uwvm-managed-example-inputs-v1', language='js', runtime=runtime,
                     paths=dict(quickjs=str(args.quickjs), wasi_sdk=str(args.wasi_sdk), validator=str(args.validator)),
                     directories=[str(args.quickjs), str(args.wasi_sdk)], files=tools,
                     file_receipts=[dict(path=str(args.toolchain_receipt), sha256=sha(args.toolchain_receipt), root=str(args.wasi_sdk))]))
    compiler = Path(sys.executable).resolve(strict=True)
    toolchain = compiler.parent.parent
    contract = dict(target='wasm32-wasip1', address_bits=32, wasi='wasip1', exceptions=False,
                    threads=False, startup='command', core_baseline='3.0')
    profile = dict(id='task06-real-quickjs20260604-complete-applications', profile=contract,
                   toolchain_id='actual-pinned-quickjs-20260604-wasi33', sysroot='include/platform/ros/js',
                   resource_dir=None, files=[pin(path, sdk) for path in sorted(sdk.rglob('*')) if path.is_file()],
                   runtime_requirements=[], library_abi=dict(address_bits=32, wasi_pointer_abi='wasip1-memory32',
                   exceptions=False, threads=False, exception_encoding='none', thread_abi='none', shared_memory=False,
                   required_files=['include/platform/ros/js/guest.mjs']), languages={'js': dict(
                   toolchain_id='actual-pinned-quickjs-20260604-wasi33', version='2026-06-04', compiler=pin(compiler, toolchain),
                   compiler_argv=[], linker_argv=[], library_modes=['runtime:js-quickjs'], source_ready=True,
                   compiled=False, runtime_tested=False, runtime_identity={key: runtime[key] for key in
                   ('route', 'name', 'version', 'mode', 'language', 'target', 'address_bits', 'startup', 'exceptions')},
                   driver='uwvm-managed-sdk-consumer-v1', supports_clang_config=False,
                   host_runtime_dependency_names=['Admitted Linux CPython loader/stdlib TCB for build tools only'])})
    save(sdk / 'share/wasm-profiles.json', dict(format='uwvm-sdk-wasm-profiles-v1', defaults=contract,
                                              profiles=[profile], default_authority=[]))
    frozen = [pin(path) for path in sorted(sdk.rglob('*')) if path.is_file()]
    save(args.out / 'sdk-source-pins.json', frozen)
    save(args.out / 'assembly.json', dict(files=assembly,
                                        private_shared_proposal=[],
                                        shared_producer_source_mode='CANONICAL_PUBLIC_NO_OVERRIDE',
                                        shared_producer_inputs=patch_rows,
                                        provider=pin(sdk / 'share/wasm-profiles.json'), installed_sdk=str(sdk)))
    for name, tool in (('xmake', args.xmake), ('validator', args.validator), ('clang', args.wasi_sdk / 'bin/clang')):
        run('version-' + name, [tool, '--version'], 'ACTUAL_TOOL_IDENTITY')
    artifacts = {}
    selections = [
            ('monitor', 'flexiv', 'robot-states-monitor-process'),
            ('controllers', 'flexiv', 'official-controllers-process'),
            ('keyboard', 'elephant', 'teleop-keyboard-process'),
            ('observer', 'ufactory', 'test-robot-states-process')]
    if args.dobot_only:
        selections = [('dobot', 'dobot', 'demo-cycle-process')]
    else:
        selections.append(('query', 'realman', 'get-arm-state-process'))
    for label, platform, selection in selections:
        run('configure-' + label, [args.xmake, 'f', '-c', '--yes', '--ros-sdk-platforms=ros', '--ros-sdk-languages=js',
            '--sdk-target=wasm32-wasip1', '--sdk-startup=command', '--sdk-exceptions=n', '--sdk-threads=n',
            '--sdk-offline=y', '--sdk-python=' + str(compiler), '--sdk-toolchain=' + str(toolchain),
            '--sdk-example-platform=' + platform, '--sdk-example-language=js', '--sdk-example=' + selection,
            '--ros-sdk-js-runtime=quickjs', '--ros-sdk-managed-inputs=' + str(inputs),
            '--ros-sdk-managed-inputs-sha256=' + sha(inputs)], 'ACTUAL_XMAKE_CONFIGURATION')
        product = args.out / (label + '-output')
        run('build-' + label, [args.xmake, 'sdk-example-build', '--output=' + str(product)], 'ACTUAL_XMAKE_QUICKJS_WASM_BUILD')
        receipt = json.loads((product / 'build-receipt.json').read_bytes())
        require(receipt['status'] == 'BUILT_NOT_EXECUTED' and len(receipt['artifacts']) == 1, 'actual one-Wasm SDK receipt required')
        artifact = Path(receipt['artifacts'][0]['path'])
        require(sha(artifact) == receipt['artifacts'][0]['sha256'], 'exact final SDK artifact required')
        run('validate-' + label, [args.validator, 'validate', artifact], 'ACTUAL_WASM_VALIDATION')
        run('imports-' + label, ['/usr/bin/wasm-objdump', '-x', artifact], 'ACTUAL_WASM_IMPORT_INVENTORY')
        artifacts[label] = artifact

    def guest(artifact):
        return [args.runtime, '-Rint', '--wasip1-global-noinherit-system-environment',
                '--ros1-global-disable', '--ros2-global-disable', '--run', artifact]

    def finish(scope):
        for value in frozen:
            require(pin(Path(value['path'])) == value, 'installed SDK source changed during actual builds/Guests')
        require({str(path) for path in sdk.rglob('*') if path.is_file()} == {row['path'] for row in frozen}, 'installed SDK file set changed')
        require(not any(set(path.relative_to(sdk).parts) & {'origin', 'test', 'tests', '.xmake'} for path in sdk.rglob('*')),
                'SDK gained a forbidden maintenance/test/cache directory')
        save(args.out / 'summary.json', dict(status='PASS_SCOPED_RELOCATED_JS_APPLICATIONS', cases=len(cases),
             artifacts={key: pin(path) for key, path in artifacts.items()},
             actual_xmake_builds=len(artifacts), wasm_validation=len(artifacts), host_js_evaluations=0, native_ros=0,
             actual_cleanup_failure_paths=0,
             cleanup_failure_qualification='PENDING_GENUINE_ISOLATED_NATIVE_PEER_AND_RETURNED_NUMERIC_FAILURES',
             gateway=0, hardware=0, original_processes=0, publication_ready=False, scope=scope))

    if args.dobot_only:
        dobot_guests(args, artifacts['dobot'], run, guest)
        finish('One complete dual-version DOBOT recording/default-denial artifact; native timing/transport remains pending')
        return

    for name in ('valid', 'empty-q', 'nan-unobserved', 'trailing', 'square-overflow'):
        good = name in ('valid', 'empty-q')
        data = struct.pack('<BBIBB', 0, 0, 1, 0, 0) + (args.fixtures / (name + '.bin')).read_bytes()
        wanted = (args.fixtures / (name + '.stdout')).read_bytes() if good else None
        expected_code = 34 if name == 'square-overflow' else 22
        prefix_output = b'' if name in ('nan-unobserved', 'trailing') else None
        checker = None if good else lambda out, err, code=expected_code, prefix=prefix_output: failure_matches(out, code, prefix=prefix)
        run('monitor-' + name, guest(artifacts['monitor']), 'ACTUAL_GUEST_COMPLETE_MONITOR_CALLBACK',
            0 if good else 'NONZERO_SHARED_ENGINE_FAILURE', data, wanted, checker)
    alias = b'task06-denied'
    denied = struct.pack('<BBIBB', 1, 0, 1, 0, len(alias)) + alias
    run('monitor-live-denial', guest(artifacts['monitor']), 'ACTUAL_GUEST_MONITOR_DEFAULT_DENIAL',
        'NONZERO_SHARED_ENGINE_FAILURE', denied, checker=lambda out, err: failure_matches(out, 13, prefix=b''))
    invalid_serial = struct.pack('<BBIB', 1, 0, 1, 1) + b'/' + bytes([len(alias)]) + alias
    run('monitor-invalid-serial-before-open', guest(artifacts['monitor']), 'ACTUAL_GUEST_FRONT_LOADED_VALIDATION',
        'NONZERO_SHARED_ENGINE_FAILURE', invalid_serial,
        checker=lambda out, err: failure_matches(out, 22, prefix=b''))
    valid = struct.pack('<BBIBB', 0, 0, 1, 0, 0) + (args.fixtures / 'valid.bin').read_bytes()
    prefix = b'Finite virtual-address prerequisite: 32768 MiB; physical/RSS limits are separate\n'
    run('xmake-monitor-process', [args.xmake, 'sdk-example-run', '--receipt=' + str(args.out / 'monitor-output/build-receipt.json'),
        '--runtime=' + str(args.runtime)], 'ACTUAL_XMAKE_GUEST_MONITOR', 0, valid,
        prefix + (args.fixtures / 'valid.stdout').read_bytes())

    def text(value):
        encoded = value.encode('utf-8')
        return struct.pack('<I', len(encoded)) + encoded

    def sequence(values):
        return struct.pack('<I', len(values)) + b''.join(struct.pack('<d', value) for value in values)

    def joint_state(names, positions, bad=False):
        header = struct.pack('<iI', 19, 123456789) + text('base')
        velocity = [index + .125 for index in range(len(names))]
        effort = [index - .5 for index in range(len(names))]
        if bad:
            effort[-1] = float('nan')
        return header + struct.pack('<I', len(names)) + b''.join(map(text, names)) + sequence(positions) + sequence(velocity) + sequence(effort)

    def event(tag, body):
        return bytes([tag]) + struct.pack('<I', len(body)) + body

    def recording(kind, configuration, events):
        encoded = json.dumps(configuration, separators=(',', ':')).encode()
        return struct.pack('<BII', kind, len(events), len(encoded)) + encoded + b''.join(events)

    def command(body):
        return event(67, body)

    trajectory_config = dict(joints=['a', 'b'], goals=[[1, 2], [3, 4]], checkStartingPoint=True)
    initial = event(83, joint_state(['a', 'b'], [0, 0]))
    ticks = [event(84, struct.pack('<Q', tick)) for tick in (0, 6000000000)]
    expected = b''
    for goal in ([1, 2], [3, 4]):
        body = struct.pack('<iII', 0, 0, 0) + struct.pack('<I', 2) + text('a') + text('b')
        body += struct.pack('<I', 1) + sequence(goal) + sequence([]) * 3 + struct.pack('<iI', 1, 0)
        expected += command(body)
    run('controller-trajectory-cycle', guest(artifacts['controllers']), 'ACTUAL_GUEST_COMPLETE_CONTROLLER_RECORDING',
        0, recording(0, trajectory_config, [initial, *ticks]), expected)
    seven = event(83, joint_state(['j' + str(index) for index in range(1, 8)], list(range(7))))
    first_tick = event(84, struct.pack('<Q', 0))
    order = [3, 0, 6, 4, 1, 2, 5]
    for kind in (1, 2):
        body = struct.pack('<II', 0, 0) + sequence(order) if kind == 1 else struct.pack('<14d', *order, *([0] * 7))
        run('controller-sine-' + str(kind), guest(artifacts['controllers']), 'ACTUAL_GUEST_COMPLETE_CONTROLLER_RECORDING',
            0, recording(kind, {}, [seven, first_tick]), command(body))
    run('controller-unobserved-nan-refusal', guest(artifacts['controllers']), 'ACTUAL_GUEST_CONTROLLER_REFUSAL',
        'NONZERO_SHARED_ENGINE_FAILURE', recording(1, {}, [event(83, joint_state(['j' + str(index) for index in range(7)], list(range(7)), True)), first_tick]),
        checker=lambda out, err: failure_matches(out, 22, framed=True) and not any(tag == 67 for tag, body in frames(out)))
    run('controller-cancel', guest(artifacts['controllers']), 'ACTUAL_GUEST_CONTROLLER_CANCEL',
        'NONZERO_SHARED_ENGINE_FAILURE', recording(0, trajectory_config, [event(88, b'')]),
        checker=lambda out, err: failure_matches(out, 125, framed=True) and not any(tag == 67 for tag, body in frames(out)))

    coordinates = struct.pack('<6f', 100, 10, 100, 0, 0, 0)
    keyboard_events = [event(67, coordinates), event(75, b'w'), event(75, b'g'), event(75, b'b')]
    wanted_coordinate = bytes([0]) + struct.pack('<6fbb', 112.5, 10, 100, 0, 0, 0, 50, 1)
    keyboard_commands = [wanted_coordinate, bytes([2, 1]), wanted_coordinate, bytes([3, 1, 2, 5]), wanted_coordinate]
    run('keyboard-movement-gripper-pump', guest(artifacts['keyboard']), 'ACTUAL_GUEST_COMPLETE_KEYBOARD_RECORDING',
        0, recording(0, {}, keyboard_events), checker=lambda out, err:
        [body for tag, body in frames(out) if tag == 67] == keyboard_commands and
        len([1 for tag, _ in frames(out) if tag == 68]) == 3)
    home_events = [event(67, coordinates), event(84, struct.pack('<Q', 0)), event(75, b'2'),
                   event(67, struct.pack('<6f', 200, 20, 200, 1, 2, 3)), event(84, struct.pack('<Q', 2000000000))]
    # MycobotSetAngles is six float32 joints then the original signed speed byte.
    home_commands = [bytes([1]) + struct.pack('<6fb', 0, 8, -127, 40, 0, 0, 50),
                     bytes([0]) + struct.pack('<6fbb', 200, 20, 200, 1, 2, 3, 50, 1)]
    run('keyboard-home-full-command-and-delayed-observation', guest(artifacts['keyboard']), 'ACTUAL_GUEST_COMPLETE_KEYBOARD_RECORDING',
        0, recording(0, {}, home_events), checker=lambda out, err: [body for tag, body in frames(out) if tag == 67] == home_commands)
    run('keyboard-whole-unprinted-nan', guest(artifacts['keyboard']), 'ACTUAL_GUEST_KEYBOARD_REFUSAL',
        'NONZERO_SHARED_ENGINE_FAILURE', recording(0, {}, [event(67, struct.pack('<6f', 100, 10, 100, 0, 0, math.nan))]),
        checker=lambda out, err: failure_matches(out, 22, framed=True) and not any(tag == 67 for tag, body in frames(out)))
    denied_aliases = dict(aliases={name: 'deny-' + name for name in (
        'coordinatesState', 'anglesState', 'coordinates', 'angles', 'gripper', 'pump', 'wallClock')})
    run('keyboard-live-default-denial', guest(artifacts['keyboard']), 'ACTUAL_GUEST_KEYBOARD_DEFAULT_DENIAL',
        'NONZERO_SHARED_ENGINE_FAILURE', recording(1, denied_aliases, [event(80, b'')]),
        checker=lambda out, err: failure_matches(out, 13, framed=True) and not any(tag == 67 for tag, body in frames(out)))
    invalid_keyboard = dict(denied_aliases, speed=129)
    run('keyboard-invalid-complete-config-before-open', guest(artifacts['keyboard']), 'ACTUAL_GUEST_FRONT_LOADED_VALIDATION',
        'NONZERO_SHARED_ENGINE_FAILURE', recording(1, invalid_keyboard, [event(80, b'')]),
        checker=lambda out, err: failure_matches(out, 22, framed=True) and not any(tag == 67 for tag, body in frames(out)))

    observed = joint_state(['a', 'b', 'c'], [1, -0.0, 1e21])
    robot_header = struct.pack('<iII', 0, 0, 0)
    robot_body = robot_header + struct.pack('<7hI12f', 4, 2, 120, 0, 0, -9, 0, 0, *([0] * 12))
    gpio_body = robot_header + struct.pack('<2h4H4f32B', 0, 0, 1, 2, 3, 4, 1.25, -2.5, 3.5, -4.5, *([0] * 32))
    startup = b'[test_robot_states] namespace: /xarm\n[test_robot_states] test_robot_states start\n'
    expected_observer = startup + b'[joint_states] positon: [ 1.000000 -0.000000 1000000000000000000000.000000 ]\n'
    expected_observer += b'[robot_states] state: 4, error: -9\n[xarm_cgpio_states] callback\n[test_robot_states] test_robot_states over\n'
    run('observer-whole-three-stream-workflow', guest(artifacts['observer']), 'ACTUAL_GUEST_COMPLETE_XARM_OBSERVER',
        0, recording(0, {}, [event(74, observed), event(82, robot_body), event(67, gpio_body)]), expected_observer)
    bad_robot = bytearray(robot_body)
    struct.pack_into('<f', bad_robot, len(bad_robot) - 4, math.nan)
    run('observer-unprinted-last-offset-refusal', guest(artifacts['observer']), 'ACTUAL_GUEST_XARM_OBSERVER_REFUSAL',
        'NONZERO_SHARED_ENGINE_FAILURE', recording(0, {}, [event(82, bad_robot)]),
        checker=lambda out, err: failure_matches(out, 22, prefix=startup))
    denied_aliases = dict(aliases={name: 'deny-' + name for name in ('jointStates', 'robotStates', 'cgpioStates')})
    run('observer-live-default-denial', guest(artifacts['observer']), 'ACTUAL_GUEST_XARM_OBSERVER_DEFAULT_DENIAL',
        'NONZERO_SHARED_ENGINE_FAILURE', recording(1, denied_aliases, [event(80, b'')]),
        checker=lambda out, err: failure_matches(out, 13, prefix=startup))
    invalid_observer = dict(aliases=dict(jointStates='same', robotStates='same', cgpioStates='gpio'))
    run('observer-invalid-distinct-aliases-before-open', guest(artifacts['observer']), 'ACTUAL_GUEST_FRONT_LOADED_VALIDATION',
        'NONZERO_SHARED_ENGINE_FAILURE', recording(1, invalid_observer, [event(80, b'')]),
        checker=lambda out, err: failure_matches(out, 22, prefix=startup))

    if 'query' in artifacts:
        ticks = [event(84, struct.pack('<Q', value)) for value in (0, 1000000000, 3000000000, 4000000000, 5000000000, 6000000000)]
        expected_commands = [bytes([0]), bytes([1]) + text('get_current_arm_state'), bytes([2])]
        expected_text = (b'*******Get Arm Software Version Pub show on the rm_driver node.*******\n'
                         b'*******Get Arm State Pub*******\n*******Get Arm Six Force Pub*******\n'
                         b'*******All command is over please click ctrl+c end*******\n')
        run('query-three-original-full-commands-and-waits', guest(artifacts['query']), 'ACTUAL_GUEST_COMPLETE_REALMAN_QUERY',
            0, recording(0, {}, ticks), checker=lambda out, err:
            [body for tag, body in frames(out) if tag == 67] == expected_commands and
            b''.join(body for tag, body in frames(out) if tag == 79) == expected_text and
            json.loads([body for tag, body in frames(out) if tag == 68][-1])['complete'] is True)
        ready = ticks[:2]
        pose = struct.pack('<I6f7dIB', 6, *range(1, 7), *range(7, 14), 0, 6)
        euler = struct.pack('<I7f6fIB', 7, *range(1, 8), -0.0, 2, 3, 4, 5, 6, 0, 7)
        force = struct.pack('<6f', 1, -0.0, 3, 4, 5, 6)
        observations = [*ready, event(86, bytes(48)), event(81, pose), event(69, euler)]
        observations += [event(70, bytes([kind]) + force) for kind in range(4)]
        version_labels = ['Product_version', 'controller_version', 'algorithm_info', 'ctrl_info build_time', 'ctrl_info version',
                          'com_info build_time', 'com_info version', 'program_info build_time', 'program_info version',
                          'dynamic_info', 'plan_info build_time', 'plan_info version']
        expected_callbacks = ''.join(label + ' is: \n' for label in version_labels)
        expected_callbacks += ('joint radian state is: [1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000]\n\n'
                               'pose Quaternion is: [7.000000, 8.000000, 9.000000, 10.000000, 11.000000, 12.000000, 13.000000]\n\n'
                               'joint angle state is: [1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000]\n\n'
                               'pose Euler angle is: [-0.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000]\n\n')
        for label in ('Six force', 'Zero six force', 'Work zro force', 'Tool zero force'):
            expected_callbacks += label + ' value is: [force_Fx:1.000000,\n force_Fy:-0.000000,\n force_Fz:3.000000,\n force_Mx:4.000000,\n force_My:5.000000,\n force_Mz:6.000000]\n\n'
        run('query-all-seven-whole-callbacks', guest(artifacts['query']), 'ACTUAL_GUEST_COMPLETE_REALMAN_CALLBACKS',
            0, recording(0, {}, observations), checker=lambda out, err:
            b''.join(body for tag, body in frames(out) if tag == 79) == expected_callbacks.encode() and
            not any(tag == 67 for tag, body in frames(out)))
        aliases = {name: 'deny-' + name for name in ('softwareCommand', 'armCommand', 'forceCommand', 'softwareState',
                   'armState', 'eulerState', 'rawForce', 'sensorZeroForce', 'workZeroForce', 'toolZeroForce', 'rosClock')}
        run('query-live-default-denial', guest(artifacts['query']), 'ACTUAL_GUEST_REALMAN_DEFAULT_DENIAL',
            'NONZERO_SHARED_ENGINE_FAILURE', recording(1, dict(aliases=aliases), [event(80, b'')]),
            checker=lambda out, err: failure_matches(out, 13, framed=True) and not any(tag in (67, 79) for tag, body in frames(out)))
        invalid_query = dict(aliases, softwareCommand=aliases['armCommand'])
        run('query-invalid-distinct-aliases-before-open', guest(artifacts['query']), 'ACTUAL_GUEST_FRONT_LOADED_VALIDATION',
            'NONZERO_SHARED_ENGINE_FAILURE', recording(1, dict(aliases=invalid_query), [event(80, b'')]),
            checker=lambda out, err: failure_matches(out, 22, framed=True) and not any(tag in (67, 79) for tag, body in frames(out)))
    finish('Complete recorded source workflows and default denial; native timer/parameter/transport parity remains pending')


if __name__ == '__main__':
    main()
