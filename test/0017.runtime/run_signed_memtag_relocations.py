#!/usr/bin/env python3
"""Exercise checked cold MTE relocations and live state through the real signed manager."""
import argparse
import json
from pathlib import Path
import resource
import struct

from run_signed_module_state import Harness, Instance, section, s32, u
from run_signed_update_boundaries import call_export, permit, request, sha


def fixture(bits, generation=0, change=None):
    # Two unaligned, little-endian data pointers refer to the same static
    # object. The second is one-past and must retain the base object's tag.
    width = bits // 8
    const = b'\x42' if bits == 64 else b'\x41'
    add = b'\x7c' if bits == 64 else b'\x6a'
    load = bytes([0x29 if bits == 64 else 0x28, 0, 0])
    store = bytes([0x37 if bits == 64 else 0x36, 0, 0])
    i32_load, i32_store = b'\x28\0\0', b'\x36\0\0'
    end = b'\x0b'

    def ptr(value):
        return const + s32(value & 0xffffffff)

    def tagged(value):
        return ptr(value) + b'\xff' + u(0x01010027) + b'\0'

    first, second = 35, 51
    def reloc(slot, addend):
        return tagged(slot) + tagged(64) + ptr(addend) + add + store

    initializer = reloc(first, 4) + reloc(second, 16)
    if change == 'ordinary-start':
        initializer += b'\x01'  # Legal nop in a function with the same linker name.
    if change == 'overlapping-slots':
        initializer += reloc(first, 4)
    # Dereference the second pointer after stepping back into the first object.
    # Failure here is observable even if the approved business call succeeds.
    entry = tagged(second) + load + ptr(-12) + add + i32_load + b'\x41\x11\x47\x04\x40\x00\x0b'
    # Mode 1 changes both the live pointer word and its new pointee.
    value = (b'\x20\0\x04\x40' + tagged(first) + tagged(96) + store
             + tagged(96) + b'\x41' + s32(43) + i32_store + end
             + tagged(first) + load + i32_load + b'\x41' + s32(generation) + b'\x6a')
    bodies = [b'\0' + instructions + end for instructions in (entry, value, initializer)]
    payload = bytearray(80)
    for slot, raw in ((first, 69 if change == 'wrong-data' else 68), (second, 80)):
        payload[slot - 32:slot - 32 + width] = raw.to_bytes(width, 'little')
    payload[36:40] = struct.pack('<I', 17)
    # static descriptors [32,48), [48,64), [64,80) and [96,112)
    globals_name = b'uwvm.memtag.linear.globals'
    static = b'\x02\x04' + b''.join(b'\0' + u(at) + u(16) for at in (32, 48, 64, 96))
    name = b'__wasm_memtag_init_globals'
    function_names = b'\x01\x02' + u(len(name)) + name
    return (b'\0asm\x01\0\0\0' + section(1, bytes.fromhex('0260000060017f017f'))
            + section(3, b'\x03\x00\x01\x00')
            + section(5, bytes([1, 0x34 if bits == 64 else 0x30, 16 if bits == 64 else 4, 1]))
            + section(7, b'\x02\x06_start\0\0\x05value\0\x01') + section(8, b'\x02')
            + section(10, u(len(bodies)) + b''.join(u(len(b)) + b for b in bodies))
            + section(11, b'\x01\x00' + ptr(32) + end + u(len(payload)) + payload)
            + section(0, u(len(globals_name)) + globals_name + static)
            + section(0, b'\x04name\x01' + u(len(function_names)) + function_names))


def profile(bits):
    settings = {'parser.baseline': 'wasm3', 'parser.memory64': 'on' if bits == 64 else 'off',
                'parser.adm': 'on', 'parser.memtag': 'on', 'runtime.memtag-checks': 'on'}
    return [word for key, value in settings.items() for word in ('--wasm-policy-global', key, value)]


def check_live(h, bits):
    image, target = fixture(bits), fixture(bits, 12)
    h.current = f'private-{bits}'
    with Instance(h, h.current, image, features=profile(bits)) as vm:
        state, launch = h.launch(vm)
        h.check('measured initial image', state['installed'] == sha(image).hex(), state)
        def value(mode, expected):
            reply = call_export(vm.host, struct.pack('<IIII', 1, 4, 4, mode))
            h.check('business pointer and pointee', reply['active'] and reply['status'] == 0
                    and reply['result'] == struct.pack('<I', expected).hex(), reply)

        value(0, 17)
        value(1, 43)
        retry = request(vm.host, 9, launch)
        h.check('repeated launch is a receipt', retry['status'] == 0 and retry['epoch'] == 1, retry)
        value(0, 43)
        for index, candidate, expected in ((2, fixture(bits, change='ordinary-start'), 7), (1, target, 0)):
            scope = struct.pack('<Q', index)
            before = request(vm.host, 0)
            transaction = f'{bits}-scope-{index}'.encode()
            token = permit(h.key, before, 1, sha(candidate), sha(transaction), declaration=scope)
            reply = request(vm.host, 1, token + struct.pack('<I', 1) + scope + candidate)
            h.check('signed function scope', reply['status'] == expected, reply)
            if expected:
                h.check('cold initializer stays installed', reply['active'] and reply['installed'] == state['installed'], reply)
                value(0, 43)
            else:
                h.commit(vm, before, candidate, scope, transaction, test_tamper=True)
        value(0, 55)
        h.stop(vm)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vm', type=Path, required=True)
    parser.add_argument('--runner', default='')
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    args.engine = 'aot'
    args.out.mkdir(parents=True, exist_ok=False)
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    h = Harness(args)
    try:
        for bits in (32, 64):
            check_live(h, bits)
        for change, status in (('ordinary-start', 1), ('wrong-data', 10), ('overlapping-slots', 10)):
            h.current = change
            with Instance(h, change, fixture(64, change=change), features=profile(64)) as vm:
                code = vm.child.wait(timeout=15)
                message = vm.error_path.read_text(errors='replace')
                h.check('refused before signed manager admission', code == 126 and
                        f'production update configuration refused: {status}' in message, dict(exit=code, stderr=message))
    finally:
        (args.out / 'report.json').write_text(json.dumps(dict(checks=h.rows, processes=h.processes), indent=2) + '\n')


if __name__ == '__main__':
    main()
