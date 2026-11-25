#!/usr/bin/env python3
"""Replay path_open/fd_write/fd_close without reopening a real sandbox file."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time
from run_dump_launch_policy import VM, require


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--binary', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--runner-json', type=Path)
    parser.add_argument('--reject-archive', type=Path, help='older provider journal to refuse before retirement')
    args = parser.parse_args()
    args.out = args.out.resolve()
    args.out.mkdir(parents=True, exist_ok=False)
    require('/docker-' in Path('/proc/self/cgroup').read_text(), 'resource cgroup required')
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    root = args.out / 'sandbox'
    root.mkdir()
    wat = args.out / 'program.wat'
    wat.write_text(r'''(module
      (import "wasi_snapshot_preview1" "path_open" (func $open (param i32 i32 i32 i32 i32 i64 i64 i32 i32) (result i32)))
      (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
      (import "wasi_snapshot_preview1" "fd_close" (func $close (param i32) (result i32)))
      (memory 1) (data (i32.const 0) "effect.txt")
      (data (i32.const 128) "\a0\00\00\00\11\00\00\00")
      (data (i32.const 160) "FILE-REPLAY-ONCE\0a")
      (func (export "_start") (loop
        i32.const 3 i32.const 0 i32.const 0 i32.const 10 i32.const 9
        i64.const 64 i64.const 0 i32.const 0 i32.const 64 call $open if unreachable end
        i32.const 64 i32.load i32.const 128 i32.const 1 i32.const 80 call $write if unreachable end
        i32.const 80 i32.load i32.const 17 i32.ne if unreachable end
        i32.const 64 i32.load call $close if unreachable end br 0)))''')
    wasm = wat.with_suffix('.wasm')
    subprocess.run(['/usr/bin/wat2wasm', str(wat), '-o', str(wasm)], check=True)
    vm = None
    result = {'passed': False, 'replays': []}
    try:
        # Parent: PATH_OPEN | PATH_CREATE_FILE | PATH_FILESTAT_SET_SIZE.
        # Inheriting/child: FD_WRITE only; no implicit filesystem grant.
        flags = ['--wasip1-global-enable', '--wasip1-global-mount-dir', '/sandbox', str(root),
                 '--wasip1-global-mount-dir-rights', '/sandbox', '533504', '64']
        for api in ('path_open', 'fd_write', 'fd_close'):
            flags += ['--wasip1-global-interface', api, 'allow']
        vm = VM([str(args.binary.resolve()), '-Raot', '-Rdbg', '-Rct', '0', '-Rllvm-cache-path', 'disable',
                 '--wasm-policy-global', 'runtime.memtag-checks', 'off', *flags, '--run', str(wasm)], args.out, prefix)

        def stopped():
            deadline = time.monotonic() + 120
            while True:
                status = vm.request('status')
                stop = re.search(r'stop-id ([1-9][0-9]*)', status)
                if stop and 'function=3 byte-offset=2 ' in status:
                    return stop[1]
                require(time.monotonic() < deadline, status)
                time.sleep(.01)

        require('breakpoint 1 ' in vm.request('breakwasm 0 3 2'), 'loop breakpoint')
        vm.request('continue')
        stop = stopped()
        require('mode=1 ' in vm.request('dump record start ' + stop), 'record start')
        vm.request('continue')
        stop = stopped()
        effect = root / 'effect.txt'
        require(effect.read_bytes() == b'FILE-REPLAY-ONCE\n', 'real recording file write')
        before = effect.stat()
        preserved = root / 'recorded-effect.txt'
        effect.rename(preserved)
        archive = args.out / 'interval.uwsnap'
        require('saved=1' in vm.request(f'dump record save {stop} {archive}'), 'save journal')
        memory = vm.request('memory 0 0 64 20')
        if args.reject_archive:
            status = vm.request('status')
            rejected = vm.request(f'dump replay {stop} {args.reject_archive.resolve()}')
            require(rejected.startswith('error:') and 'state-error=14 applied=0 retained=1;' in rejected,
                    'incompatible provider shape was not rejected before retirement: ' + rejected)
            require(vm.request('status') == status and vm.request('memory 0 0 64 20') == memory,
                    'rejected journal changed the current stop/generation/memory')
            require(not effect.exists() and preserved.read_bytes() == b'FILE-REPLAY-ONCE\n',
                    'rejected journal dispatched a real file operation')
            result['incompatibleProviderRejected'] = rejected
        for _ in range(2):
            reply = vm.request(f'dump replay {stop} {archive}')
            for attempt in range(12):
                if not reply.startswith('snapshot-restore pending'):
                    break
                reply = vm.request('dump restore continue')
            require(reply.startswith('snapshot-restored'), reply)
            vm.request('continue')
            stop = stopped()
            require('mode=4 ' in vm.request('dump record status'), 'replay endpoint')
            require(vm.request('memory 0 0 64 20') == memory, 'recorded fd/results differ')
            require(not effect.exists(), 'replay reopened the real file')
            after = preserved.stat()
            require(preserved.read_bytes() == b'FILE-REPLAY-ONCE\n' and
                    (after.st_ino, after.st_size, after.st_mtime_ns) == (before.st_ino, before.st_size, before.st_mtime_ns),
                    'replay changed the preserved native effect')
            require(vm.request('continue').startswith('error:'), 'replay escaped to live file operations')
            result['replays'].append({'stop': stop, 'fileAbsent': True, 'originalInode': before.st_ino})
        result['passed'] = True
    except Exception as error:
        result['error'] = repr(error)
    finally:
        if vm:
            result['commands'] = vm.commands
            vm.close()
        (args.out / 'results.json').write_text(json.dumps(result, indent=2))
    print(json.dumps({k: v for k, v in result.items() if k != 'commands'}), flush=True)
    require(result['passed'], 'file replay failed; inspect results.json')


if __name__ == '__main__':
    main()
