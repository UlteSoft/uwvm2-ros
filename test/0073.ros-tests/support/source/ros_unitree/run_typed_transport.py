#!/usr/bin/env python3
"""Run all28 actual same-main typed Unitree ROS2 denied/positive DDS cases.

Invoke only from a complete frozen source inside Root's selected current64GiB
cgroup and isolated loopback-only namespace. All input labels come from the
ordinary Unitree Frame configuration; --selection-input names one separately
pinned closed selection document. This runner does not build, install, download,
mount, change cgroups, initialize a simulator or access a physical robot.

Original build statuses remain binary-only. The final transport PASS requires
fourteen actual command binaries,28 fresh actual Runtime instances,28 real
generated-message native witnesses, complete live descriptor-based executable/
DSO mapping proofs, exact transcripts, quiet endpoint/payload checks and normal
owned cleanup. Any missing report, identity, SDK map or byte relation fails.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types


ROOT = Path(__file__).resolve().parents[5]
sys.dont_write_bytecode = True


def require(value, reason):
    if not value:
        raise ValueError(reason)


def bounded(path, limit):
    """Read one stable ordinary bootstrap file without importing project code."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute bootstrap file required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= limit, 'bounded bootstrap file required')
        chunks, size = [], 0
        while True:
            block = os.read(descriptor, min(65536, limit + 1 - size))
            if not block:
                break
            chunks.append(block)
            size += len(block)
            require(size <= limit, 'bootstrap file grew')
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        require(size == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'bootstrap file changed')
        return b''.join(chunks)
    finally:
        os.close(descriptor)


def bootstrap(args):
    """Bind this entrypoint and first helper before any project helper executes."""
    source = args.source.resolve(strict=True)
    require(source == ROOT, 'invoke actual complete frozen source entrypoint')
    raw = bounded(args.manifest, 16 * 1024**2)
    require(hashlib.sha256(raw).hexdigest() == args.manifest_sha256, 'explicit current source manifest hash differs')
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, 'duplicate source manifest key')
            result[key] = value
        return result
    frozen = json.loads(raw, object_pairs_hook=pairs)
    require(type(frozen) is dict and frozen, 'complete frozen source manifest required')
    helper = None
    for relative in ('test/0073.ros-tests/support/source/ros_unitree/run_typed_transport.py', 'test/0073.ros-tests/support/source/ros_javascript_guest/run.py'):
        body = bounded(source / relative, 1024**2)
        require(frozen.get(relative) == {'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()},
                'actual entry/helper missing from complete frozen manifest')
        if relative.endswith('ros_javascript_guest/run.py'):
            helper = body
    api = types.ModuleType('typed_transport_low_level_execution')
    api.__file__ = str(source / 'test/0073.ros-tests/support/source/ros_javascript_guest/run.py')
    sys.modules[api.__name__] = api
    exec(compile(helper, api.__file__, 'exec'), api.__dict__)
    unitree = api.frozen_module(source, frozen, 'test/0073.ros-tests/support/source/ros_unitree/common.py', 'typed_transport_unitree_owners')
    return source, frozen, api, unitree


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--manifest', type=Path, required=True)
    parser.add_argument('--manifest-sha256', required=True)
    parser.add_argument('--config', type=Path, required=True)
    parser.add_argument('--config-sha256', required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--selection-input', required=True)
    args = parser.parse_args()
    frame = None
    try:
        source, frozen, api, unitree = bootstrap(args)
        frame = unitree.Frame(args, 'unitree-same-main-typed-native-DDS')
        frame.unitree, frame.frozen, frame.native_aliases, frame.workers = unitree, frozen, {}, {}
        require(all(frame.config[key] is None for key in ('simulation', 'benchmark', 'profiler')),
                'typed transport configuration must not contain unrelated executable workloads')
        require(args.selection_input in frame.inputs, 'explicit selected actual input document required')
        require(len(os.fsencode(frame.out / 's27')) < 104,
                'owned output path must leave room for compact actual UNIX socket names')
        admission = api.frozen_module(source, frozen, 'test/0073.ros-tests/support/source/ros_unitree/typed_transport_inputs.py', 'typed_transport_input_admission')
        session = api.frozen_module(source, frozen, 'test/0073.ros-tests/support/source/ros_unitree/typed_transport_session.py', 'typed_transport_actual_sessions')
        native = api.frozen_module(source, frozen, 'test/0073.ros-tests/support/source/ros_javascript_guest/native.py', 'typed_transport_native_descriptor_proofs')
        selection, selection_pin = native.document(api, frame, frame.input(args.selection_input), 1024**2)
        frame.report.update(selection=selection_pin, cases=[], actual_Runtime_cases_executed=0,
            main_principal='main', optional_WASI_rights=[], default_ROS_authority=[],
            scope='Actual historical-source-labeled interpreter and native prepared products; same-main typed ROS2 DDS only; no physical robot, ROS1, simulator or performance qualification')
        frame.save()
        admitted = admission.admit(frame, api, native, selection)
        frame.report['admission']['transport_execution'] = 'RUNNING_ACTUAL_OWNED_CASES'
        for product in admitted['products']:
            for positive in (False, True):
                session.run_case(api, native, frame, admitted, product, positive)
                frame.report['completed_actual_cases'] = len(frame.report['cases'])
                frame.save()
        cases = frame.report['cases']
        require(len(cases) == 28 and len({row['case'] for row in cases}) == 28 and
                all(row['status'] == 'PASS_ACTUAL_TYPED_DDS_CASE' and row['worker_exit'] == row['native_exit'] == 0 and
                    row['cleanup']['status'] == 'PASS' for row in cases) and
                sum(row['positive'] for row in cases) == 14, 'all28 complete fresh actual transport cases required')
        frame.report['admission']['transport_execution'] = 'COMPLETED_28_ACTUAL_OWNED_CASES'
        frame.report['transport_claim'] = '14 original binary-only commands independently executed in28 fresh same-main denied/positive actual native DDS lifecycles'
        return frame.finish('PASS_UNITREE_FOURTEEN_SAME_MAIN_COMMANDS_TWENTY_EIGHT_ACTUAL_TYPED_DDS_CASES')
    except BaseException as error:
        if frame is not None:
            return frame.finish('FAIL', repr(error))
        print(repr(error), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
