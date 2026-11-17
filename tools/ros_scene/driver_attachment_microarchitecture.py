#!/usr/bin/env python3
"""Qualify matched encrypted driver clients with VTune hardware microarchitecture sampling.

Keep the client, receiver and Python provisioner distinct. Only the actual client
PID contributes to the exported summary, source and assembly reports. Record CPU
topology, requested sampling interval and the collector's effective configuration;
an interval request alone never establishes a fixed wall-clock sampling period.
Run serially through in_existing_cgroup.py. No fallback can silently pass a failed
analysis, and profiled timings are never used as the latency comparison.
"""
import argparse
import csv
import json
import math
import re
import subprocess
import sys
from pathlib import Path

from driver_attachment_performance import ROOT, VTUNE
from hardware_sampling import reference_frequency, vtune_periods
from performance_build_policy import compile_policy, vm_policy
from run_scene_vm import GATE, sha, verify_gate


def topology(cpu):
    root = Path('/sys/devices/system/cpu') / f'cpu{cpu}' / 'topology'
    return {name: (root / name).read_text().strip()
            for name in ('physical_package_id', 'core_id', 'thread_siblings_list')}


def mapped_samples(path, assembly=False):
    """Require a sampled source instruction, not just an empty report header."""
    with path.open() as stream:
        rows = csv.DictReader((line for line in stream if not line.startswith('war:')), delimiter='\t')
        return sum(1 for row in rows if row.get('Source Line', '').isdigit()
                   and (row.get('Address', '').startswith('0x') if assembly
                        else 'driver_' in row.get('Source File', ''))
                   and any(float(value or 0) > 0 for key, value in row.items()
                           if key and key.startswith('Hardware Event Count:')))


def sample_statistics(text):
    """Read the aggregate perf record counts before per-event summaries."""
    if not text.lstrip().startswith('Aggregated stats:'):
        raise ValueError('missing aggregate perf record statistics')
    counts = {}
    for line in text.splitlines()[2:]:
        if line and not line[0].isspace():
            break
        match = re.match(r'\s*(\w+) events:\s*(\d+)', line)
        if match:
            counts[match[1]] = int(match[2])
    if counts.get('SAMPLE', 0) == 0:
        raise ValueError('trace has no actual samples')
    return counts


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('runtime', 'native', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--iterations', type=int, default=500000)
    parser.add_argument('--sampling-interval', type=float, default=0.1)
    parser.add_argument('--cpu', type=int, default=4)
    parser.add_argument('--peer-cpu', type=int, default=6)
    parser.add_argument('--collector-cpus', default=None,
                        help='explicit CPU list for the collector; hybrid presets may require '
                             'one CPU from each PMU (for example 4,16); reports remain client-only')
    args = parser.parse_args()
    verify_gate()
    if not 10000 <= args.iterations <= 2000000:
        parser.error('iterations must be between 10000 and 2000000')
    if not math.isfinite(args.sampling_interval) or not 0.01 <= args.sampling_interval <= 1000:
        parser.error('sampling interval must be finite and between 0.01 and 1000 milliseconds')
    if args.collector_cpus is not None:
        if not re.fullmatch(r'[0-9]+(?:,[0-9]+)*', args.collector_cpus):
            parser.error('collector CPUs must be an explicit comma-separated list')
        collector_cpus = [int(cpu) for cpu in args.collector_cpus.split(',')]
        if len(set(collector_cpus)) != len(collector_cpus) or args.cpu not in collector_cpus:
            parser.error('collector CPU list must be unique and include the client CPU')
        if any(not (Path('/sys/devices/system/cpu') / f'cpu{cpu}').is_dir() for cpu in collector_cpus):
            parser.error('collector CPU does not exist')
    args.runtime = args.runtime.resolve()
    args.native = args.native.resolve()
    args.output = args.output.resolve()
    if '.tmp' not in args.output.parts:
        parser.error('write results beneath a project .tmp directory')
    args.output.mkdir(parents=True, exist_ok=False)
    build_path = args.runtime.parent / 'full-build-results.json'
    native_path = args.native.parent / 'native-build.json'
    build = json.loads(build_path.read_text())
    native = json.loads(native_path.read_text())
    assert build['complete'] and build['binary_sha256'] == sha(args.runtime)
    assert native['complete'] and native['native_sha256'] == sha(args.native)
    assert native['source_root'] == build['source_root']
    native_compile = next(row['argv'] for row in native['commands'] if row['name'] == 'compile')
    cpus = {str(cpu): topology(cpu) for cpu in (args.cpu, args.peer_cpu)}
    report = dict(complete=False, scope=__doc__, runtime_sha256=sha(args.runtime),
                  native_sha256=sha(args.native), source_root=build['source_root'],
                  build_policy=vm_policy(args.runtime), native_build_policy=compile_policy(native_compile),
                  cgroup=str(GATE), cpu_topology=cpus,
                  shared_physical_core=(cpus[str(args.cpu)]['physical_package_id'], cpus[str(args.cpu)]['core_id']) ==
                                       (cpus[str(args.peer_cpu)]['physical_package_id'], cpus[str(args.peer_cpu)]['core_id']),
                  requested_sampling_interval_ms=args.sampling_interval,
                  collector_cpu_mask=args.collector_cpus or 'ALL',
                  fixed_wall_clock_sampling_verified=False,
                  profiled_latency_is_baseline=False, profiles=[], commands=[],
                  inputs={str(path): sha(path) for path in
                          (Path(__file__), ROOT / 'tools/ros_scene/driver_attachment_performance.py',
                           build_path, native_path)}, events_before=(GATE / 'memory.events').read_text())

    def save():
        (args.output / 'results.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(label, argv):
        command = list(map(str, argv))
        result = subprocess.run(command, capture_output=True, text=True, timeout=240)
        (args.output / (label + '.stdout')).write_text(result.stdout)
        (args.output / (label + '.stderr')).write_text(result.stderr)
        report['commands'].append(dict(label=label, argv=command, exit=result.returncode))
        save()
        if result.returncode:
            raise RuntimeError(f'{label} failed: {result.stderr[-2000:]}')
        print(label, 'passed', flush=True)
        return result.stdout

    save()
    try:
        for kind in ('native', 'wasm'):
            target = args.output / (kind + '-uarch')
            stdout = run(kind + '-collect', [VTUNE, '-collect', 'uarch-exploration',
                '-knob', 'pmu-collection-mode=detailed', '-knob', 'dram-bandwidth-limits=false',
                '-knob', f'sampling-interval={args.sampling_interval}', '-no-analyze-system',
                *(['-cpu-mask', args.collector_cpus] if args.collector_cpus else []),
                '-no-summary', '-data-limit=400', '-return-app-exitcode', '-result-dir', target,
                '-source-search-dir', build['source_root'], '-source-search-dir', ROOT,
                '--', sys.executable, ROOT / 'tools/ros_scene/driver_attachment_performance.py',
                '--one', kind, '--tls', '--runtime', args.runtime, '--native', args.native,
                '--output', args.output / (kind + '-work'), '--iterations', args.iterations,
                '--cpu', args.cpu, '--peer-cpu', args.peer_cpu])
            trials = [json.loads(line) for line in stdout.splitlines() if line.startswith('{"calls":')]
            assert len(trials) == 1 and trials[0]['calls'] == args.iterations
            trial = trials[0]
            row = dict(kind=kind, client_pid=trial['client_pid'], receiver_pid=trial['receiver_pid'],
                       source_mapping=False, assembly_mapping=False, topdown_summary=False,
                       loss_and_throttle_free=False)
            report['profiles'].append(row)
            assert row['client_pid'] != row['receiver_pid']
            selected = ['-filter', 'process-id=' + str(row['client_pid'])]
            for label, options in (
                    ('summary', ['-report', 'summary']),
                    ('functions', ['-report', 'hw-events', '-group-by', 'function']),
                    ('source', ['-report', 'hw-events', '-group-by', 'function,source-file,source-line'])):
                path = args.output / (kind + '-' + label + '.tsv')
                run(kind + '-' + label, [VTUNE, *options, '-result-dir', target, *selected,
                                        '-format', 'csv', '-csv-delimiter', 'tab', '-report-output', path])
                if label == 'summary':
                    text = path.read_text()
                    row['topdown_summary'] = all(metric in text for metric in
                                                ('Retiring', 'Front-End Bound', 'Bad Speculation', 'Back-End Bound'))
                if label == 'source':
                    row['sampled_source_rows'] = mapped_samples(path)
                    row['source_mapping'] = row['sampled_source_rows'] > 0
            function = ('uwvm2::imported::ros::scene::attachment_tls::session::receive' if kind == 'native'
                        else 'uwvm2::imported::ros::scene::driver_dispatch::preflight')
            path = args.output / (kind + '-assembly.tsv')
            run(kind + '-assembly', [VTUNE, '-report', 'hw-events', '-result-dir', target, *selected,
                '-source-object', 'function=' + function, '-group-by', 'address', '-column',
                'Assembly,Source Line,Hardware Event Count', '-format', 'csv', '-report-output', path])
            row['sampled_assembly_rows'] = mapped_samples(path, assembly=True)
            row['assembly_mapping'] = row['sampled_assembly_rows'] > 0
            trace = target / 'data.0/system-wide.perf'
            stats = run(kind + '-sample-statistics', ['/usr/bin/perf', 'report', '--stdio', '--stats',
                                                     '--percent-limit', '100', '-i', trace])
            row['sample_statistics'] = sample_statistics(stats)
            row['loss_and_throttle_free'] = not any(row['sample_statistics'].get(name, 0) for name in
                ('LOST', 'LOST_SAMPLES', 'THROTTLE', 'UNTHROTTLE'))
            row['period_configuration'] = vtune_periods(target)
            row['reference_hz'] = reference_frequency(target / 'config/context_values.cfg')
            run(kind + '-event-attributes', ['/usr/bin/perf', 'evlist', '-v', '-i', trace])
            row['configuration'] = {str(path.relative_to(args.output)): sha(path) for path in
                                    (target / 'config/runsa.options', target / 'config/context_values.cfg')}
            save()
        report['complete'] = len(report['profiles']) == 2 and all(
            row['source_mapping'] and row['assembly_mapping'] and row['topdown_summary']
            and row['loss_and_throttle_free']
            for row in report['profiles'])
    finally:
        report['inputs_unchanged'] = all(sha(path) == digest for path, digest in report['inputs'].items())
        report['inputs_unchanged'] &= sha(args.runtime) == report['runtime_sha256']
        report['inputs_unchanged'] &= sha(args.native) == report['native_sha256']
        report['events_after'] = (GATE / 'memory.events').read_text()
        report['complete'] &= report['inputs_unchanged']
        save()
    if not report['complete']:
        raise SystemExit('Microarchitecture qualification incomplete; inspect retained results')


if __name__ == '__main__':
    main()
