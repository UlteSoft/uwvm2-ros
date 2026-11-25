#!/usr/bin/env python3
"""Record across real native NI calls and stop native replay at its endpoint."""
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
    args = parser.parse_args()
    args.out = args.out.resolve()
    args.out.mkdir(parents=True, exist_ok=False)
    require('/docker-' in Path('/proc/self/cgroup').read_text(), 'resource cgroup required')
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    wat = args.out / 'program.wat'
    wat.write_text('''(module (memory 1) (global $g (mut i32) (i32.const 0))
      (func (export "_start") (loop call $f br 0))
      (func $f global.get $g i32.const 1 i32.add global.set $g))''')
    wasm = wat.with_suffix('.wasm')
    subprocess.run(['/usr/bin/wat2wasm', str(wat), '-o', str(wasm)], check=True)
    rows = []
    for name in ('native-record', 'native-endpoint'):
        folder = args.out / name
        folder.mkdir()
        row = {'case': name, 'passed': False}
        vm = None
        try:
            vm = VM([str(args.binary.resolve()), '-Raot', '-Rdbg', '-Rct', '0',
                     '-Rllvm-cache-path', 'disable', '--wasm-policy-global', 'runtime.memtag-checks', 'off', '--run', str(wasm)], folder, prefix)

            def stopped():
                deadline = time.monotonic() + 120
                while True:
                    status = vm.request('status')
                    stop = re.search(r'stop-id ([1-9][0-9]*)', status)
                    thread = re.search(r'^thread ([1-9][0-9]*)', status, re.M)
                    if stop and thread:
                        return stop[1], thread[1], status
                    require(time.monotonic() < deadline, status)
                    time.sleep(.01)

            require('breakpoint 1 ' in vm.request('breakwasm 0 0 2'), 'origin breakpoint')
            vm.request('continue')
            stop, thread, status = stopped()
            require('mode=1 ' in vm.request('dump record start ' + stop), 'start recording')
            if name == 'native-record':
                require('native instruction ' in vm.request('step asm ' + thread), 'native stop required')
                for _ in range(256):
                    reply = vm.request('ni ' + thread)
                    require(not reply.startswith('error:'), reply)
                    progress = vm.request('dump record status')
                    instructions = int(re.search(r'instructions=(\d+)', progress)[1])
                    if instructions > 1:
                        row['nativeRecordedInstructions'] = instructions
                        break
                else:
                    raise AssertionError('native NI omitted guest opcodes')
            else:
                # The replay endpoint lies INSIDE the function that NI passes
                # over, so a caller-return trap cannot enforce the endpoint.
                require('breakpoint 2 ' in vm.request('breakwasm 0 1 2'), 'callee endpoint')
            vm.request('continue')
            stop, thread, status = stopped()
            archive = folder / 'interval.uwsnap'
            saved = vm.request(f'dump record save {stop} {archive}')
            require('saved=1' in saved, saved)
            require('snapshot-restored' in vm.request(f'dump replay {stop} {archive}'), 'restore journal')
            stop, thread, status = stopped()
            if name == 'native-record':
                vm.request('continue')
                stop, thread, status = stopped()
            else:
                require('native instruction ' in vm.request('step asm ' + thread), 'replay native stop')
                for _ in range(256):
                    reply = vm.request('ni ' + thread)
                    progress = vm.request('dump record status')
                    if 'mode=4 ' in progress:
                        break
                    require(not reply.startswith('error:'), reply)
                else:
                    raise AssertionError('native replay did not reach its endpoint')
                stop, thread, status = stopped()
            progress = vm.request('dump record status')
            require('mode=4 ' in progress and 'journal-error=0 ' in progress, progress)
            if name == 'native-endpoint':
                require('function=1 byte-offset=2 ' in status and 'native-pc=' not in status,
                        'endpoint must retain the exact cooperative callee stop: ' + status)
            require(vm.request('continue').startswith('error:'), 'endpoint allowed live execution')
            row.update(passed=True, endpoint=status, journal=progress)
        except Exception as error:
            row['error'] = repr(error)
        finally:
            if vm:
                row['commands'] = vm.commands
                vm.close()
            rows.append(row)
            (args.out / 'results.json').write_text(json.dumps(rows, indent=2))
        print(name, row['passed'], row.get('error', ''), flush=True)
    require(all(row['passed'] for row in rows), 'native replay failed; inspect results.json')


if __name__ == '__main__':
    main()
