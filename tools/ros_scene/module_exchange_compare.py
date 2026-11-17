#!/usr/bin/env python3
"""Serial A/B/B/A comparison of the complete registered-message lifecycle.

Use in_existing_cgroup.py. Each observation contains 10,000 round trips per
timed batch by default, excluding startup/JIT. Report distributions of batch
means, never individual-request tail latency or a real-time guarantee. The
optional direct i64 call has a different security contract. Do not run another
profiler, simulator or benchmark concurrently with this comparison.
"""
import argparse
import json
import os
from pathlib import Path
import statistics
import subprocess
import sys
import time
from module_exchange_performance import ROOT, GATE, verify_gate, sha


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--before', type=Path, required=True)
    parser.add_argument('--after', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--cpu', type=int, default=2)
    parser.add_argument('--iterations', type=int, default=10000)
    parser.add_argument('--batches', type=int, default=51)
    parser.add_argument('--payloads', type=int, nargs='+', default=[8, 256, 4096])
    parser.add_argument('--direct', action='store_true')
    args = parser.parse_args()
    verify_gate()
    assert args.cpu in os.sched_getaffinity(0)
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    variants = dict(before=args.before.resolve(), after=args.after.resolve())
    runner = Path(__file__).with_name('module_exchange_performance.py').resolve()
    inputs = [Path(__file__).resolve(), runner,
              ROOT/'test/0084.ros-runtime/run_module_security_vm.py',
              Path(__file__).with_name('hardware_sampling.py'),
              Path(__file__).with_name('performance_build_policy.py')]
    for binary in variants.values():
        receipt = binary.parent/'full-build-results.json'
        build = json.loads(receipt.read_text())
        assert build['complete'] and build['binary_sha256'] == sha(binary)
        inputs += [binary, receipt, Path(build['source_root'])/'source-manifest.json']
    pins = {str(path): sha(path) for path in inputs}
    report = dict(complete=False, scope=__doc__, cgroup=str(GATE),
                  cgroup_inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
                  boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                  cpu=args.cpu, inputs=pins, runs=[], summary={},
                  events_before=(GATE/'memory.events').read_text())

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    samples = {version: {} for version in variants}
    guests = {}
    try:
        for number, version in enumerate(('before', 'after', 'after', 'before')):
            verify_gate()
            folder = out/(str(number)+'-'+version)
            argv = [sys.executable, str(runner), 'baseline', '--runtime', str(variants[version]),
                    '--output', str(folder), '--cpu', str(args.cpu),
                    '--iterations', str(args.iterations), '--batches', str(args.batches),
                    '--payloads', *map(str, args.payloads)] + (['--direct'] if args.direct else [])
            started = time.monotonic()
            print('Measuring '+version+' '+str(number), flush=True)
            with (out/(str(number)+'.log')).open('w') as log:
                process = subprocess.run(argv, stdout=log, stderr=subprocess.STDOUT, timeout=900)
            entry = dict(version=version, argv=argv, exit=process.returncode,
                         seconds=time.monotonic()-started)
            report['runs'].append(entry)
            save()
            assert process.returncode == 0, (out/(str(number)+'.log')).read_text()[-6000:]
            receipt = folder/'results.json'
            run = json.loads(receipt.read_text())
            assert run['complete'] and run['runtime_sha256'] == pins[str(variants[version])]
            entry.update(receipt=str(receipt), sha256=sha(receipt), observations=run['observations'])
            for observation in run['observations']:
                assert observation['profile'] is None
                label = observation['label']
                guest = observation['guest_sha256']
                assert guests.setdefault(label, guest) == guest, 'A/B guests differ'
                samples[version].setdefault(label, []).append(observation['batch_mean_ns_p50'])
            save()
        for label in samples['before']:
            before, after = (statistics.median(samples[v][label]) for v in ('before', 'after'))
            report['summary'][label] = dict(before_ns=before, after_ns=after,
                after_over_before=after/before, reduction_percent=100*(1-after/before),
                run_batch_medians_ns={v: samples[v][label] for v in variants})
        report['complete'] = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(Path(path)) == digest for path, digest in pins.items())
        report['complete'] = report['complete'] and report['inputs_unchanged']
        report['events_after'] = (GATE/'memory.events').read_text()
        save()
    assert report['complete']
    for label, values in report['summary'].items():
        print(label+': %.2f -> %.2f ns (%.1f%% reduction)' %
              (values['before_ns'], values['after_ns'], values['reduction_percent']), flush=True)


if __name__ == '__main__':
    main()
