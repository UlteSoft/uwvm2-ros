#!/usr/bin/env python3
"""Qualify exact retained Native6 build products without recompilation.

Run only from a complete frozen source under the selected current64GiB/swap0
cgroup. The ordinary Unitree configuration explicitly inventories all original
source/SDK/tool/run roots. A pinned selection document names genuine raw inner
and owner reports; the immutable failed batch never becomes a successful batch.
This entrypoint creates metadata only and launches no compiler or target.
"""
from __future__ import annotations

import sys
import traceback


sys.dont_write_bytecode = True


def main():
    # The shared bounded bootstrap executes only its own first source-pinned
    # helper. Invoking this file is also independently bound below, before any
    # acceptance-specific module executes.
    from pathlib import Path
    import hashlib
    import argparse
    import os
    import stat
    import types
    import json
    root = Path(__file__).resolve().parents[5]
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'manifest', 'config', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    for name in ('manifest-sha256', 'config-sha256', 'selection-input'):
        parser.add_argument('--' + name, required=True)
    args = parser.parse_args()
    frame = None
    def need(value, reason):
        if not value:
            raise ValueError(reason)
    def bounded(path, maximum):
        path = Path(path)
        need(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute bootstrap metadata required')
        fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
        try:
            first = os.fstat(fd)
            need(stat.S_ISREG(first.st_mode) and 0 < first.st_size <= maximum, 'bounded ordinary bootstrap input required')
            body = bytearray()
            while True:
                block = os.read(fd, min(65536, maximum + 1 - len(body)))
                if not block:
                    break
                body.extend(block)
                need(len(body) <= maximum, 'bootstrap input grew')
            last, named = os.fstat(fd), path.stat(follow_symlinks=False)
            need(len(body) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
                 for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'bootstrap metadata changed')
            return bytes(body)
        finally:
            os.close(fd)
    try:
        need(args.source.resolve(strict=True) == root, 'invoke the actual complete frozen source entrypoint')
        raw = bounded(args.manifest, 16 * 1024**2)
        need(hashlib.sha256(raw).hexdigest() == args.manifest_sha256, 'explicit source manifest digest differs')
        def pairs(items):
            result = {}
            for key, value in items:
                need(key not in result, 'duplicate bootstrap metadata key')
                result[key] = value
            return result
        frozen = json.loads(raw, object_pairs_hook=pairs)
        helper = None
        for relative in ('test/0073.ros-tests/support/source/ros_unitree/finalize_native_products.py', 'test/0073.ros-tests/support/source/ros_javascript_guest/run.py'):
            body = bounded(root / relative, 1024**2)
            need(frozen.get(relative) == {'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()}, 'entry/helper absent from complete source freeze')
            if relative.endswith('ros_javascript_guest/run.py'):
                helper = body
        api = types.ModuleType('retained74_low_level_bootstrap')
        api.__file__ = str(root / 'test/0073.ros-tests/support/source/ros_javascript_guest/run.py')
        sys.modules[api.__name__] = api
        exec(compile(helper, api.__file__, 'exec'), api.__dict__)
        unitree = api.frozen_module(root, frozen, 'test/0073.ros-tests/support/source/ros_unitree/common.py', 'retained74_unitree_frame')
        frame = unitree.Frame(args, 'native74-retained-products-at-rest')
        frame.frozen = frozen
        need(all(frame.config[key] is None for key in ('simulation', 'benchmark', 'profiler')), 'metadata-only configuration required')
        admission = api.frozen_module(root, frozen, 'test/0073.ros-tests/support/source/ros_unitree/typed_transport_inputs.py', 'retained74_input_access')
        implementation = api.frozen_module(root, frozen, 'test/0073.ros-tests/support/source/ros_unitree/native_retained_products.py', 'retained74_at_rest_qualification')
        need(args.selection_input in frame.inputs, 'explicit pinned retained74 selection input required')
        pin = frame.inputs[args.selection_input]
        selection_raw = bounded(Path(pin['path']), 1024**2)
        need(len(selection_raw) == pin['bytes'] and hashlib.sha256(selection_raw).hexdigest() == pin['sha256'], 'selected receipt document changed')
        selection = admission.decode(selection_raw)
        receipt = implementation.qualify(frame, api, admission, selection)
        frame.report.update(receipt)
        frame.report['selection'] = pin
        # Keep ALL actual current pins for Frame.finish's genuine reopen pass.
        # Only serialization is compacted for this distinct receipt schema.
        # The sidecar excludes its own pin; that self-pin is explicit inline.
        ledger = frame.out / 'current-input-pins.jsonl'
        current = {item['path']: item for item in frame.pins}
        need(len(current) == len(frame.pins) and 0 < len(current) <= 250000, 'distinct bounded complete current pin ledger required')
        ledger_bytes = 0
        with ledger.open('xb') as stream:
            for path in sorted(current):
                raw = (json.dumps(current[path], sort_keys=True, separators=(',', ':')) + '\n').encode()
                ledger_bytes += len(raw)
                need(len(raw) <= 16384 and ledger_bytes <= 256 * 1024**2, 'bounded full7 current input ledger required')
                stream.write(raw)
            stream.flush()
            os.fsync(stream.fileno())
        ledger_pin = frame.pin(ledger)
        frame.report['product_pins'].append(ledger_pin)
        frame.report['input_pins_encoding'] = 'PINNED_COMPLETE_CURRENT_FULL7_JSONL'
        frame.report['input_pin_catalog'] = {'pin': ledger_pin, 'rows': len(current),
            'ordering': 'STRICT_PHYSICAL_PATH_ASCENDING_UNIQUE',
            'scope': 'ALL_ACTUAL_FRAME_PINS_CHECKED_BY_NORMAL_FINISH_LEDGER_SELF_PIN_RETAINED_INLINE'}
        bootstrap_pins = [frame.source_before['manifest'], frame.report['configuration'], *frame.inputs.values(), ledger_pin]
        inline = {item['path']: item for item in bootstrap_pins}
        frame.report['input_pins_checked_count'] = len(frame.pins)
        def save_scoped_receipt():
            # Frame has already performed its normal input/source/root checks
            # when its terminal save calls this hook. No legacy Frame code or
            # check is changed; every checked pin is retained in the sidecar.
            wire = dict(frame.report)
            if 'input_pins' in wire:
                wire['input_pins'] = [inline[key] for key in sorted(inline)]
            need(len((json.dumps(wire, sort_keys=True, indent=2) + '\n').encode()) <= 64 * 1024**2,
                 'new retained74 receipt exceeds its unchanged64MiB consumer contract')
            frame.common.atomic_json(frame.out / 'report.json', wire)
        frame.save = save_scoped_receipt
        return frame.finish(implementation.STATUS)
    except BaseException as error:
        if frame is not None:
            # Preserve the exact failing guard/call site in this new receipt;
            # the failed original producer and earlier attempts stay unchanged.
            frame.report['traceback'] = traceback.format_exc()
            return frame.finish('FAIL', repr(error))
        traceback.print_exc(file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
