#!/usr/bin/env python3
"""Replay a real foreign-platform MTE journal without reproducing native I/O."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import time
from run_dump_launch_policy import VM, require
from run_snapshot_replay_memtag import fixture


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--binary', type=Path, required=True)
    parser.add_argument('--recorded', type=Path, required=True,
                        help='successful run_snapshot_replay_memtag output directory')
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--runner-json', type=Path)
    parser.add_argument('--backend', choices=('software', 'arm-mte'), default='software')
    parser.add_argument('--mode', choices=('sync', 'async'), default='sync')
    parser.add_argument('--memory64', action='store_true')
    parser.add_argument('--shared', action='store_true')
    parser.add_argument('--log', action='store_true')
    parser.add_argument('--tag', type=lambda x: int(x, 0), default=3)
    args = parser.parse_args()
    require(0 <= args.tag <= 15, 'tag width')
    require('/docker-' in Path('/proc/self/cgroup').read_text(), 'resource cgroup required')
    out = args.out.resolve(); out.mkdir(parents=True, exist_ok=False)
    recorded = args.recorded.resolve()
    evidence = json.loads((recorded / 'results.json').read_text())
    require(evidence['passed'] and len(evidence['rounds']) == 2, 'successful foreign recording required')
    archive = recorded / 'interval.uwsnap'
    digest = hashlib.sha256(archive.read_bytes()).hexdigest()
    require(digest == evidence['journalSha256'], 'recorded journal changed')
    wasm = out / 'tagged.wasm'
    wasm.write_bytes(fixture(args.log, args.memory64, args.shared, args.tag))
    require(wasm.read_bytes() == (recorded / 'tagged.wasm').read_bytes(), 'identical original module required')
    expected = evidence['rounds'][0]['observations']
    requests = ['memory 0 0 0 16', 'memory 0 0 256 32', 'memtags 0 0 256 32', 'memtags 0 0 512 32']
    result = {'passed': False, 'journalSha256': digest, 'recorded': str(recorded), 'rounds': []}
    vm = None
    try:
        flags = ['--wasm-policy-global', 'runtime.memtag-backend', args.backend, '--runtime-memtag-mode', args.mode]
        if args.log:
            flags += ['--runtime-memtag-action', 'silent-log', 'path', str(out / 'violations.log')]
        suffix = '_wasm64' if args.memory64 else ''
        runner = json.loads(args.runner_json.read_text()) if args.runner_json else []
        vm = VM([str(args.binary.resolve()), '-Raot', '-Rdbg', '-Rct', '0', '-Rllvm-cache-path', 'disable', *flags,
                 '--wasip1-global-enable', '--wasip1-global-interface', 'random_get' + suffix, 'allow',
                 '--wasip1-global-grant', 'random', '--wasip1-global-interface', 'fd_write' + suffix, 'allow',
                 '--wasip1-global-grant', 'stdout.write', '--run', str(wasm)], out, runner)
        require(not vm.request('breakwasm 0 2 2').startswith('error:'), 'loop breakpoint')

        def stopped():
            deadline = time.monotonic() + 120
            while True:
                status = vm.request('status'); match = re.search(r'stop-id ([1-9][0-9]*)', status)
                if match and re.search(r'^thread [1-9]', status, re.M):
                    return match[1]
                require(time.monotonic() < deadline, status); time.sleep(.01)

        vm.request('continue'); stop = stopped()
        require(b'MTE-REPLAY-EFFECT' not in (out / 'vm.log').read_bytes(), 'foreign replay started after a live effect')
        for _ in range(2):
            restored = vm.request(f'dump replay {stop} {archive}')
            for attempt in range(10):
                if not restored.startswith('snapshot-restore pending'):
                    break
                restored = vm.request('dump restore continue')
            require(restored.startswith('snapshot-restored'), restored)
            stop = stopped()
            require(not vm.request('continue').startswith('error:'), 'resume replay')
            stop = stopped()
            actual = [vm.request(command) for command in requests]
            require(actual == expected, {'expected': expected, 'actual': actual})
            status = vm.request('dump record status'); require('mode=4 ' in status, status)
            require(b'MTE-REPLAY-EFFECT' not in (out / 'vm.log').read_bytes(), 'foreign replay dispatched native output')
            result['rounds'].append({'status': status, 'observations': actual})
        result['passed'] = True
    except Exception as error:
        result['error'] = repr(error)
    finally:
        if vm:
            result['commands'] = vm.commands; vm.close()
        (out / 'results.json').write_text(json.dumps(result, indent=2))
    print(json.dumps({k: v for k, v in result.items() if k not in ('commands', 'rounds')}), flush=True)
    require(result['passed'], 'portable MTE replay failed; inspect results.json')


if __name__ == '__main__':
    main()
