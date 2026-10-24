#!/usr/bin/env python3
"""Inspect the selected linear MoonBit command's actual reserved memory.

This finite Core section reader loads no SDK/application, starts no process
and executes no Wasm. Whole-module validity and dynamic behavior require the
separate admitted validator and Runtime. Unknown layouts fail explicitly.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path


def inspect(path):
    spec = importlib.util.spec_from_file_location('reviewed_core_sections',
        Path(__file__).with_name('verify_portable_telemetry.py'))
    core = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(core)
    body = path.read_bytes()
    reader = core.Reader(body)
    assert reader.take(8) == b'\0asm\x01\0\0\0'
    memories, globals_, data, exports, functions = [], [], [], [], []

    def signed(part, bits):
        value = 0
        for shift in range(0, bits + 6, 7):
            byte = part.byte()
            value |= (byte & 127) << shift
            if byte < 128:
                if byte & 64:
                    value -= 1 << (shift + 7)
                assert -(1 << (bits - 1)) <= value < 1 << (bits - 1)
                return value
        raise ValueError('unterminated signed constant')

    def constant(part, kind):
        assert part.byte() == {0x7f: 0x41, 0x7e: 0x42}[kind]
        value = signed(part, 32 if kind == 0x7f else 64)
        assert part.byte() == 0x0b
        return value

    while reader.at < len(body):
        kind = reader.byte()
        part = core.Reader(reader.take(reader.uint()))
        if kind == 5:
            assert part.uint() == 1 and part.byte() == 1
            memories.append((part.uint(), part.uint()))
        elif kind == 6:
            for _ in range(part.uint()):
                value_kind, mutable = part.byte(), part.byte()
                assert value_kind in (0x7f, 0x7e) and mutable in (0, 1)
                globals_.append({'type': value_kind, 'mutable': mutable,
                                 'value': constant(part, value_kind)})
        elif kind == 7:
            for _ in range(part.uint()):
                exports.append({'name': part.name(), 'kind': part.byte(), 'index': part.uint()})
        elif kind == 10:
            for _ in range(part.uint()):
                code = part.take(part.uint())
                local = core.Reader(code)
                for _ in range(local.uint()):
                    assert local.uint() <= 10000 and local.byte() in (0x7f, 0x7e, 0x7d, 0x7c, 0x7b, 0x70, 0x6f)
                functions.append((code, code[local.at:]))
        elif kind == 11:
            for _ in range(part.uint()):
                flag = part.uint()
                assert flag in (0, 2), 'passive data requires separate placement qualification'
                if flag == 2:
                    assert part.uint() == 0
                offset = constant(part, 0x7f)
                contents = part.take(part.uint())
                assert 8519680 <= offset <= offset + len(contents) <= 16 << 20
                data.append({'offset': offset, 'bytes': len(contents),
                             'sha256': hashlib.sha256(contents).hexdigest()})
        else:
            continue
        assert part.at == len(part.body)
    assert memories == [(256, 1024)]
    assert {'name': 'memory', 'kind': 2, 'index': 0} in exports
    assert any(row['name'] == '_start' and row['kind'] == 0 for row in exports)
    assert any(row['name'] == 'abi_coverage' and row['kind'] == 0 for row in exports)
    data_end = max((row['offset'] + row['bytes'] for row in data), default=8519680)
    # This pinned compiler initializes its allocator lazily. Global0 starts
    # at zero, not at a compile-time heap pointer. Check actual instruction
    # boundaries in its initializer and a caller's zero-test/call sequence.
    assert globals_[0] == {'type': 0x7f, 'mutable': 1, 'value': 0}
    imported = len(core.imports(body))
    initializers = []
    for index, (whole, instructions) in enumerate(functions):
        part = core.Reader(instructions)
        if not instructions or part.byte() != 0x41:
            continue
        address = signed(part, 32)
        if instructions[part.at:part.at + 10] != b'\x41\x0f\x6a\x41\x70\x71\x21\0\x3f\0':
            continue
        if not instructions.endswith(b'\x20\0\x24\0\x0b'):
            continue
        aligned = (address + 15) & ~15
        assert data_end <= address <= aligned < 16 << 20 and aligned >= 8519680
        target, callers = imported + index, []
        for caller_index, (caller_body, code) in enumerate(functions):
            if not code.startswith(b'\x23\0\x45\x04\x40\x10'):
                continue
            call = core.Reader(code)
            call.take(6)
            if call.uint() == target and call.take(3) == b'\x0b\x23\0':
                callers.append({'function': imported + caller_index,
                                'body_sha256': hashlib.sha256(caller_body).hexdigest()})
        assert callers, 'actual lazy allocation path must call the inspected initializer'
        initializers.append({'function': target, 'static_end': address, 'aligned_heap_start': aligned,
                             'body_sha256': hashlib.sha256(whole).hexdigest(), 'lazy_callers': callers})
    assert len(initializers) == 1, 'one recognized actual allocator initializer required'
    return {'status': 'ACTUAL_LINEAR_MEMORY_RESERVATION_DATA_INSPECTION_PASS',
            'artifact_bytes': len(body), 'artifact_sha256': hashlib.sha256(body).hexdigest(),
            'memory_pages': memories, 'globals': globals_, 'data': data, 'exports': exports,
            'allocator_initializers': initializers,
            'reserved_stdio': [[0, 16], [32, 289], [320, 321]],
            'reserved_ros_arena': [65536, 8454160], 'configured_heap_start': 8519680,
            'execution_performed': False,
            'scope': 'Actual linear memory, exports, active data, globals and pinned lazy-allocator instruction shape; no universal dynamic allocation or GC-layout certification'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('wasm', type=Path)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    result = inspect(args.wasm)
    with args.out.open('x') as stream:
        json.dump(result, stream, indent=2)
        stream.write('\n')
    print(result['status'])
