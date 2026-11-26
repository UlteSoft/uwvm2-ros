"""Exercise complete, valid function bodies at a small module-code-bytes limit."""
import argparse
import os
from pathlib import Path
import re
import socket
import struct
import subprocess
import sys
import time

from run_signed_update_boundaries import call_export, permit, request, sha
from cryptography.hazmat.primitives.asymmetric.ed25519 import Ed25519PrivateKey
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / '0018.debugger'))
from run_dump_launch_policy import VM


START = bytes.fromhex('00 10 01 1a 10 02 1a 0b')  # call both functions, drop their results
SHORT = bytes.fromhex('00 41 07 0b')
LOCAL = bytes.fromhex('01 01 7f 41 09 0b')  # one i32 local, same expression length
NOP = bytes.fromhex('00 01 41 0a 0b')
KEY = 'parser.limit.module-code-bytes'


def module(first, second):
    # All section lengths and body lengths in this fixture fit in one LEB byte.
    sections = [(1, bytes.fromhex('02 60 00 00 60 00 01 7f')), (3, bytes([3, 0, 1, 1])),
                (5, bytes([1, 1, 1, 1])), (7, b'\x02\x06_start\x00\x00\x05value\x00\x01')]
    bodies = b''.join(bytes([len(body)]) + body for body in (START, first, second))
    sections.append((10, b'\x03' + bodies))
    return b'\x00asm\x01\x00\x00\x00' + b''.join(bytes([kind, len(data)]) + data for kind, data in sections)


def signed(binary, root, image):
    key = Ed25519PrivateKey.from_private_bytes(bytes(range(1, 33)))
    public = key.public_key().public_bytes(Encoding.Raw, PublicFormat.Raw)
    host, guest = socket.socketpair()
    with host, guest, (root / 'signed.log').open('wb') as log:
        guest.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        host.settimeout(30)
        args = [binary, '-Raot', '-Rct', '0', '-Rllvm-cache-path', 'disable', '--debug-permissions', 'none',
                '--wasm-policy-group', 'app', KEY, '24', '--wasm-policy-group-add-module', 'app', 'main',
                '--wasm-set-resource-limit', 'parser', 'module-code-bytes', '18',
                '--update-control-fd', str(guest.fileno()), '--update-public-key', public.hex(),
                '--update-contract', sha(b'code budget').hex(), '--update-scope', sha(b'fixture').hex(),
                '--update-call-export', 'value', '--wasip1-single-create', 'main', '--wasip1-single-enable', 'main',
                '--wasm-set-main-module-name', 'main', '-Wstart', '0', '--run', image]
        vm = subprocess.Popen(args, stdin=subprocess.DEVNULL, stdout=log, stderr=log, pass_fds=(guest.fileno(),))
        guest.close()
        try:
            current = request(host, 0)
            launch = permit(key, current, 4, bytes.fromhex(current['installed']), sha(b'launch'))
            current = request(host, 9, launch)
            assert current['status'] == 0 and current['epoch'] == 1, current
            # LAUNCH may acknowledge while _start still holds its execution pin.
            value_call = struct.pack('<III', 1, 0, 4)
            initial = call_export(host, value_call)
            assert initial['status'] == 0 and initial['result'] == '07000000', initial
            scope = struct.pack('<QQ', 1, 2)
            for target, expected in ((module(LOCAL, NOP), 10), (module(LOCAL, SHORT), 0)):
                transaction = sha(target + b'batch')
                prepare = permit(key, current, 1, sha(target), transaction, declaration=scope)
                outcome = request(host, 1, prepare + struct.pack('<I', 2) + scope + target)
                assert outcome['status'] == expected, outcome
                if expected:
                    assert outcome['active'] and outcome['installed'] == current['installed'], outcome
                    assert call_export(host, value_call)['result'] == '07000000'
            ready = request(host, 2)
            assert ready['status'] == 0, ready
            commit = permit(key, current, 2, sha(target), transaction, declaration=scope,
                            prepared=bytes.fromhex(ready['prepared']), ready=bytes.fromhex(ready['ready']))
            for _ in range(2):
                outcome = request(host, 3, commit)
                assert outcome['status'] == 0 and outcome['epoch'] == 2 and outcome['installed'] == sha(target).hex(), outcome
            assert call_export(host, value_call)['result'] == '09000000'
            assert request(host, 6)['status'] == 0 and vm.wait(timeout=15) == 0
        finally:
            if vm.poll() is None:
                vm.kill()
                vm.wait()


def continue_to_instruction(vm, offset):
    assert not vm.request('continue').startswith('error:')
    deadline = time.monotonic() + 15
    while time.monotonic() < deadline:
        status = vm.request('status')
        stop = re.search(r'^stop-id ([1-9][0-9]*)$', status, re.M)
        if stop and re.search(rf'^thread [1-9][0-9]* module=0 function=0 byte-offset={offset} generation=[1-9][0-9]*$', status, re.M):
            return stop[1]
        time.sleep(.01)
    raise AssertionError(status)


def restore(vm, stop, archive):
    reply = vm.request(f'dump restore current {stop} {archive}')
    for _ in range(10):
        if not reply.startswith('snapshot-restore pending'):
            return reply
        reply = vm.request('dump restore continue')
    raise AssertionError(reply)


def debug(binary, root, image):
    archive = root / 'updated.uwsnap'
    for name, body in [('short', SHORT), ('local', LOCAL), ('nop', NOP)]:
        (root / name).write_bytes(body)
    for limit in (20, 18):
        folder = root / f'debug-{limit}'
        folder.mkdir()
        vm = VM([binary, '-Raot', '-Rdbg', '-Rct', '0', '-Rllvm-cache-path', 'disable', '--debug-permissions', 'full',
                 '--wasip1-single-create', 'main', '--wasip1-single-disable', 'main', '--wasm-set-main-module-name', 'main',
                 '--wasm-policy-single', 'main', KEY, str(limit), '--run', str(image)], folder, stdin=subprocess.DEVNULL)
        try:
            assert 'breakpoint 1 ' in vm.request('breakwasm 0 0 0')
            stop = continue_to_instruction(vm, 0)
            if limit == 20:
                assert 'function replaced; generation 2' in vm.request(f'replace 0 1 1 {root / "local"}')
                # The first call has returned and its result has been dropped at offset 3.
                assert 'breakpoint 2 ' in vm.request('breakwasm 0 0 3')
                assert not vm.request('delete 1').startswith('error:')
                stop = continue_to_instruction(vm, 3)
                assert 'wasm-dump saved' in vm.request(f'dump wasm full {stop} {archive}')
                assert restore(vm, stop, archive).startswith('snapshot-restored')
                # Replacing either restored 6-byte body with 4 bytes releases two logical bytes.
                assert 'function replaced; generation 3' in vm.request(f'replace 0 1 2 {root / "short"}')
                assert 'function replaced; generation 4' in vm.request(f'replace 0 1 3 {root / "local"}')
            else:
                assert 'original stopped world retained' in restore(vm, stop, archive)
                assert f'stop-id {stop}\n' in vm.request('status')
                assert 'failed WebAssembly validation' in vm.request(f'replace 0 1 1 {root / "nop"}')
                assert 'function replaced; generation 2' in vm.request(f'replace 0 2 1 {root / "short"}')
                assert 'function replaced; generation 2' in vm.request(f'replace 0 1 1 {root / "local"}')
                assert 'failed WebAssembly validation' in vm.request(f'replace 0 2 2 {root / "nop"}')
        finally:
            vm.close()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vm', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--overlap-host', type=Path, help='module_update_code_overlap linked to the product runtime')
    args = parser.parse_args()
    root = args.out.resolve()
    root.mkdir(mode=0o700, parents=True, exist_ok=False)
    image = root / 'main.wasm'
    image.write_bytes(module(SHORT, LOCAL))
    if args.overlap_host:
        subprocess.run([args.overlap_host.resolve(), image], check=True, timeout=30)
    signed(str(args.vm.resolve()), root, str(image))
    debug(str(args.vm.resolve()), root, image)
    print('PASS signed batch, DEV replacement and checkpoint module code limits')


if __name__ == '__main__':
    main()
