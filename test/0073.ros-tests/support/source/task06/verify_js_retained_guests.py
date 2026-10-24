#!/usr/bin/env python3
"""Exercise remaining complete recordings on actual retained SDK Wasm builds.

The original r2 four-build packet remains FAILED_RETAINED. Its live error code
was hidden by the engine, so this continuation claims zero qualified default
denials. It executes only recording-mode Wasm; no compiler/SDK result is made
for the newer diagnostic/formatter source postimages.
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


def need(value, message):
    if not value:
        raise RuntimeError(message)


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
        effort[-1] = math.nan
    return header + struct.pack('<I', len(names)) + b''.join(map(text, names)) + sequence(positions) + sequence(velocity) + sequence(effort)


def event(tag, body):
    return bytes([tag]) + struct.pack('<I', len(body)) + body


def recording(kind, configuration, events):
    encoded = json.dumps(configuration, separators=(',', ':')).encode()
    return struct.pack('<BII', kind, len(events), len(encoded)) + encoded + b''.join(events)


def frames(output):
    result, cursor = [], 0
    while cursor < len(output):
        need(len(output) - cursor >= 5, 'actual complete output header truncated')
        tag, size = struct.unpack_from('<BI', output, cursor)
        cursor += 5
        need(size <= len(output) - cursor, 'actual complete output body truncated')
        result.append((tag, output[cursor:cursor + size]))
        cursor += size
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('built', 'runtime', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    need(os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') == '1', 'same admitted finite Linux owner required')
    need(sha(args.runtime) == '719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19', 'exact frozen Runtime required')
    args.out.mkdir()
    sdk = args.built / 'Relocated SDK with Unicode-λ'
    source_pins = json.loads((args.built / 'sdk-source-pins.json').read_bytes())
    artifacts = {}
    for label in ('monitor', 'controllers', 'keyboard', 'observer'):
        receipt_path = args.built / (label + '-output/build-receipt.json')
        receipt = json.loads(receipt_path.read_bytes())
        need(receipt['status'] == 'BUILT_NOT_EXECUTED' and len(receipt['artifacts']) == 1, 'actual original xmake build receipt required')
        fact = receipt['artifacts'][0]
        path = Path(fact['path'])
        need(path.is_relative_to(args.built) and path.stat().st_size == fact['bytes'] and sha(path) == fact['sha256'],
             'retained actual final Wasm changed')
        artifacts[label] = path

    def verify_sdk():
        for row in source_pins:
            path = Path(row['path'])
            need(path.is_relative_to(sdk) and path.stat().st_size == row['bytes'] and sha(path) == row['sha256'], 'original installed SDK changed')
        need({str(path) for path in sdk.rglob('*') if path.is_file()} == {row['path'] for row in source_pins}, 'SDK file set changed')
        need(not any(set(path.relative_to(sdk).parts) & {'origin', 'test', 'tests', '.xmake'} for path in sdk.rglob('*')), 'forbidden SDK maintenance/test/cache path')

    verify_sdk()
    cases = []

    def run(name, label, data, expected=None, checker=None, negative=False):
        folder = args.out / name
        folder.mkdir()
        (folder / 'input.bin').write_bytes(data)
        command = [args.runtime, '-Rint', '--wasip1-global-noinherit-system-environment', '--ros1-global-disable', '--ros2-global-disable', '--run', artifacts[label]]
        with (folder / 'stdout').open('xb') as out, (folder / 'stderr').open('xb') as err:
            result = subprocess.run(list(map(str, command)), input=data, stdout=out, stderr=err, timeout=90, check=False)
        output = (folder / 'stdout').read_bytes()
        good = result.returncode != 0 if negative else result.returncode == 0
        if expected is not None:
            good = good and output == expected
        if checker:
            good = good and checker(output)
        cases.append(dict(name=name, application=label, status='PASS' if good else 'FAIL', exit=result.returncode,
                          artifact_sha256=sha(artifacts[label]), full_input_sha256=sha(folder / 'input.bin'),
                          stdout_sha256=sha(folder / 'stdout'), stderr_sha256=sha(folder / 'stderr'),
                          negative_status_scope='Engine failure only; numeric ROS/codec status not externally observed' if negative else None))
        (args.out / 'cases.json').write_text(json.dumps(cases, indent=2) + '\n')
        print(json.dumps(cases[-1]), flush=True)

    configuration = dict(joints=['a', 'b'], goals=[[1, 2], [3, 4]], checkStartingPoint=True)
    expected = b''
    for goal in ([1, 2], [3, 4]):
        body = struct.pack('<iII', 0, 0, 0) + struct.pack('<I', 2) + text('a') + text('b')
        body += struct.pack('<I', 1) + sequence(goal) + sequence([]) * 3 + struct.pack('<iI', 1, 0)
        expected += event(67, body)
    run('trajectory-two-complete-commands', 'controllers', recording(0, configuration, [
        event(83, joint_state(['a', 'b'], [0, 0])), event(84, struct.pack('<Q', 0)), event(84, struct.pack('<Q', 6000000000))]), expected)
    initial = event(83, joint_state(['j' + str(index) for index in range(1, 8)], list(range(7))))
    tick = event(84, struct.pack('<Q', 0))
    order = [3, 0, 6, 4, 1, 2, 5]
    for kind in (1, 2):
        command = struct.pack('<II', 0, 0) + sequence(order) if kind == 1 else struct.pack('<14d', *order, *([0] * 7))
        run('sine-full-command-' + str(kind), 'controllers', recording(kind, {}, [initial, tick]), event(67, command))
    run('controller-unprinted-nan', 'controllers', recording(1, {}, [
        event(83, joint_state(['j' + str(index) for index in range(7)], list(range(7)), True)), tick]), b'', negative=True)
    run('controller-local-cancel', 'controllers', recording(0, configuration, [event(88, b'')]), b'', negative=True)

    coordinates = struct.pack('<6f', 100, 10, 100, 0, 0, 0)
    coordinate = bytes([0]) + struct.pack('<6fbb', 112.5, 10, 100, 0, 0, 0, 50, 1)
    commands = [coordinate, bytes([2, 1]), coordinate, bytes([3, 1, 2, 5]), coordinate]
    run('keyboard-cartesian-gripper-pump', 'keyboard', recording(0, {}, [
        event(67, coordinates), event(75, b'w'), event(75, b'g'), event(75, b'b')]),
        checker=lambda out: [body for tag, body in frames(out) if tag == 67] == commands and len([tag for tag, _ in frames(out) if tag == 68]) == 3)
    commands = [bytes([1]) + struct.pack('<6fb', 0, 8, -127, 40, 0, 0, 50),
                bytes([0]) + struct.pack('<6fbb', 200, 20, 200, 1, 2, 3, 50, 1)]
    run('keyboard-home-wait-and-latest-observation', 'keyboard', recording(0, {}, [event(67, coordinates),
        event(84, struct.pack('<Q', 0)), event(75, b'2'), event(67, struct.pack('<6f', 200, 20, 200, 1, 2, 3)),
        event(84, struct.pack('<Q', 2000000000))]), checker=lambda out: [body for tag, body in frames(out) if tag == 67] == commands)
    run('keyboard-whole-nonfinite-observation', 'keyboard', recording(0, {}, [
        event(67, struct.pack('<6f', 100, 10, 100, 0, 0, math.nan))]), b'', negative=True)

    header = struct.pack('<iII', 0, 0, 0)
    robot = header + struct.pack('<7hI12f', 4, 2, 120, 0, 0, -9, 0, 0, *([0] * 12))
    gpio = header + struct.pack('<2h4H4f32B', 0, 0, 1, 2, 3, 4, 1.25, -2.5, 3.5, -4.5, *([0] * 32))
    startup = b'[test_robot_states] namespace: /xarm\n[test_robot_states] test_robot_states start\n'
    expected = startup + b'[joint_states] positon: [ 1.000000 -0.000000 1000000000000000000000.000000 ]\n'
    expected += b'[robot_states] state: 4, error: -9\n[xarm_cgpio_states] callback\n[test_robot_states] test_robot_states over\n'
    run('observer-all-three-whole-types', 'observer', recording(0, {}, [
        event(74, joint_state(['a', 'b', 'c'], [1, -0.0, 1e21])), event(82, robot), event(67, gpio)]), expected)
    invalid = bytearray(robot)
    struct.pack_into('<f', invalid, len(invalid) - 4, math.nan)
    run('observer-unprinted-last-offset', 'observer', recording(0, {}, [event(82, invalid)]), startup, negative=True)
    verify_sdk()
    need(all(row['status'] == 'PASS' for row in cases), 'one or more actual retained Guest cases failed')
    summary = dict(status='PASS_SCOPED_RETAINED_JS_RECORDING_GUESTS', actual_guest_cases=len(cases),
                   complete_positive_recordings=sum(row['negative_status_scope'] is None for row in cases),
                   native_ros=0, original_processes=0, qualified_default_denials=0, sdk_builds=0,
                   source_scope='Exact four r2 xmake artifacts; newer diagnostic/format postimages not qualified',
                   sdk_has_no_origin_test_or_xmake_cache=True, sdk_bytes_and_file_set_unchanged=True,
                   publication_ready=False, physical_motion=False)
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
