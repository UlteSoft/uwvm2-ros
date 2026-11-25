#!/usr/bin/env python3
"""Compare complete registered-message carriers with a native production-code reference.

The native fixture uses the actual carrier, authority, router, FP controls,
byte-atomic copies and erasure. Stable native arrays substitute for the VM's
identity/declaration/growth/MTE machinery. This is not native process IPC or
proof of native robot-application parity. Batch means are not request tails.
Run alone in the existing 64 GiB gate, with no concurrent compilation/profiling.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import statistics
import struct
import subprocess
import sys
import time

from module_exchange_performance import ROOT, GATE, verify_gate, sha, guests, CALLS, VTUNE
from performance_build_policy import compile_policy, vm_policy
from hardware_sampling import audit_samples, reference_frequency, vtune_periods


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode', choices=['baseline', 'profile', 'perf-import'])
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--compile-argv', type=Path, required=True)
    parser.add_argument('--link-argv', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--fixture-source', type=Path, help='Native fixture retained in the selected VM source manifest')
    parser.add_argument('--cpu', type=int, default=2)
    parser.add_argument('--payloads', type=int, nargs='+', default=[8, 256, 4096])
    parser.add_argument('--iterations', type=int, default=10000)
    parser.add_argument('--batches', type=int, default=51)
    parser.add_argument('--rounds', type=int, default=3)
    parser.add_argument('--clear-input', choices=['default', 'true', 'false'], default='default')
    parser.add_argument('--sampling-interval', type=float, default=.1)
    parser.add_argument('--reference-context', type=Path, help='Same-host VTune measured reference-frequency context; required for perf-import')
    parser.add_argument('--pmu', choices=['cpu_core', 'cpu_atom'], default='cpu_core')
    parser.add_argument('--reference-period', type=int, default=24000)
    parser.add_argument('--assembly-function', action='append', help='Exact source/assembly selector for both carriers; repeat to replace the default selectors')
    a = parser.parse_args()
    verify_gate()
    assert a.cpu in os.sched_getaffinity(0)
    os.sched_setaffinity(0, {a.cpu})
    assert 1 <= a.iterations <= 1000000 and 1 <= a.batches <= 501 and a.rounds > 0
    assert a.payloads and all(8 <= n <= 4096 for n in a.payloads) and .01 <= a.sampling_interval <= 1000
    if a.mode == 'perf-import':
        assert a.reference_context and 1 <= a.reference_period <= 1000000000
        reference_hz = reference_frequency(a.reference_context, os.uname().nodename)
        assert a.cpu in {int(cpu) for part in (Path('/sys/bus/event_source/devices')/a.pmu/'cpus').read_text().strip().split(',')
                         for cpu in (range(int(part.split('-')[0]), int(part.split('-')[1])+1) if '-' in part else [int(part)])}
    runtime = a.runtime.resolve()
    build_path = runtime.parent/'full-build-results.json'
    build = json.loads(build_path.read_text())
    assert build['complete'] and build['binary_sha256'] == sha(runtime)
    source = Path(build['source_root'])
    manifest = source/'source-manifest.json'
    files = json.loads(manifest.read_text())
    assert all(sha(source/name) == digest for name, digest in files.items())
    out = a.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    fixture = a.fixture_source.resolve() if a.fixture_source else ROOT/'test/0084.ros-runtime/module_exchange_native.cpp'
    if a.fixture_source:
        assert fixture.is_relative_to(source), 'The fixture must belong to the selected VM source snapshot'
        assert files.get(str(fixture.relative_to(source))) == sha(fixture), 'The fixture must be pinned by the VM source manifest'
    retained = out/fixture.name
    shutil.copyfile(fixture, retained)
    flags = json.loads(a.compile_argv.read_text())
    link = json.loads(a.link_argv.read_text())
    assert '-I'+str(source/'src') in flags and not any(x.startswith('-fprebuilt-module-path=') for x in flags)
    flags += ['-O3', '-g', '-UNDEBUG', '-fexceptions']
    paths = [runtime, build_path, manifest, fixture, retained, Path(__file__).resolve(),
             a.compile_argv.resolve(), a.link_argv.resolve(), Path(flags[0]).resolve(),
             *[Path(v).resolve() for v in link[1:] if v.endswith(('.a', '.so'))],
             *[Path(__file__).with_name(name) for name in ('module_exchange_performance.py', 'hardware_sampling.py', 'performance_build_policy.py')],
             ROOT/'test/0084.ros-runtime/run_module_security_vm.py', ROOT/'test/0084.ros-runtime/run_scene_vm.py']
    if a.mode == 'perf-import':
        paths.append(a.reference_context.resolve())
    pins = {str(path): sha(path) for path in paths}
    for path in paths:
        if path.is_relative_to(ROOT) and path.suffix in ('.py', '.cpp'):
            target = out/'retained-inputs'/path.relative_to(ROOT)
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(path, target)
    report = dict(complete=False, scope=__doc__, inputs=pins, source_root=str(source), clear_input=a.clear_input,
                  cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
                  boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(), cpu=a.cpu,
                  events_before=(GATE/'memory.events').read_text(), runtime_sha256=sha(runtime),
                  native_compile_policy=compile_policy(flags), vm_compile_policy=vm_policy(runtime),
                  commands=[], workloads={}, observations=[], profiles=[], summary={}, invocation=sys.argv,
                  requested_10us_target_met=False)
    if a.mode == 'perf-import':
        report['reference_context'] = dict(path=str(a.reference_context.resolve()), sha256=sha(a.reference_context), hz=reference_hz)

    def settings():
        return {name: (Path('/proc/sys/kernel')/name).read_text().strip()
                for name in ('perf_event_max_sample_rate', 'perf_cpu_time_max_percent', 'perf_event_paranoid')}

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(label, argv, timeout=300):
        verify_gate()
        argv = list(map(str, argv))
        started = time.monotonic()
        process = subprocess.run(argv, capture_output=True, timeout=timeout)
        (out/(label+'.stdout')).write_bytes(process.stdout)
        (out/(label+'.stderr')).write_bytes(process.stderr)
        report['commands'].append(dict(label=label, argv=argv, exit=process.returncode, seconds=time.monotonic()-started))
        save()
        assert process.returncode == 0, (label, process.stderr.decode(errors='replace')[-6000:])
        return process

    def observe(label, payload, result, profile=None):
        assert len(result.stdout) == 8*(a.batches+2), (label, len(result.stdout))
        values = struct.unpack('<'+'Q'*(a.batches+2), result.stdout)
        assert values[-1] == 22*(1000+a.iterations*a.batches)
        samples = sorted(value/a.iterations for value in values[:-2])
        report['observations'].append(dict(label=label, payload=payload, profile=profile,
            iterations=a.iterations, batches=a.batches, checksum=values[-1],
            batch_mean_ns_p50=statistics.median(samples), batch_mean_ns_p95=samples[min(len(samples)-1, int(.95*len(samples)))]))
        save()

    report['sampling_settings_before'] = settings()
    success = False
    try:
        # Correctness and byte-erasure assertions run under sanitizers first.
        # Their timings are deliberately excluded from the comparison.
        for name, extra in [('sanitized', ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']), ('native', [])]:
            run('compile-'+name, [*flags, *extra, '-c', retained, '-o', out/(name+'.o')])
            run('link-'+name, [link[0], *extra, out/(name+'.o'), *link[1:], '-lcrypto', '-lm', '-o', out/name])
            report[name+'_sha256'] = sha(out/name)
        for payload in a.payloads:
            result = run('check-native-'+str(payload), [out/'sanitized', payload, 8, 2, *([] if a.clear_input=='default' else [a.clear_input])])
            assert len(result.stdout) == 32 and struct.unpack('<QQQQ', result.stdout)[-1] == 22*1016
        for payload in a.payloads:
            name = 'broker-'+str(payload)
            main, worker = guests(payload, a.iterations, a.batches)
            for suffix, guest, export in [('', main, '_start'), ('-worker', worker, None)]:
                wat, wasm = out/(name+suffix+'.wat'), out/(name+suffix+'.wasm')
                wat.write_text(guest.render(export))
                run('compile-'+name+suffix, ['wat2wasm', '--enable-threads', '--enable-multi-memory', '--debug-names', wat, '-o', wasm])
            grants = ['--ros-scene-create', 'benchmark', '--ros-scene-bind', 'benchmark', 'main', '--ros-scene-bind', 'benchmark', 'worker']
            for op in ['capability.open', *CALLS]:
                grants += ['--ros-scene-interface', 'benchmark', op, 'allow']
            grants += ['--ros-scene-call-budget', 'benchmark', '65536', '1', '--ros-scene-call', 'benchmark', 'echo', 'main', 'worker', 'echo',
                'request-octets', 'response-octets', str(payload), str(payload), '1', '60000000000',
                '--ros-scene-grant', 'benchmark', 'exchange', 'module.call', 'echo', 'registered-message', ','.join(CALLS),
                '--wasip1-single-create', 'main', '--wasip1-single-enable', 'main', '--wasip1-single-interface', 'main', 'clock_time_get', 'allow',
                '--wasip1-single-interface', 'main', 'fd_write', 'allow', '--wasip1-single-grant', 'main', 'monotonic-clock', '--wasip1-single-grant', 'main', 'stdout.write']
            if a.clear_input!='default':grants+=['--ros-scene-call-clear-input','benchmark','echo',a.clear_input]
            commands = {'native': [out/'native', payload, a.iterations, a.batches, *([] if a.clear_input=='default' else [a.clear_input])],
                'wasm': [runtime, '--wasm-set-main-module-name', 'main', '--runtime-aot', '--wasm-policy-global', 'runtime.llvm.policy', 'max',
                    '--runtime-compile-threads', '0', '--runtime-llvm-jit-cache-path', 'disable', *grants,
                    '--wasm-preload-library', out/(name+'-worker.wasm'), 'worker', '--run', out/(name+'.wasm')]}
            report['workloads'][name] = {key: list(map(str, command)) for key, command in commands.items()}
            report['workloads'][name]['guest_sha256'] = sha(out/(name+'.wasm'))
            save()
            if a.mode == 'baseline':
                for repeat in range(a.rounds):
                    for variant in (('native', 'wasm') if repeat % 2 == 0 else ('wasm', 'native')):
                        label = name+'-'+variant
                        observe(label, payload, run(label+'-'+str(repeat), commands[variant]))
                medians = {variant: statistics.median(row['batch_mean_ns_p50'] for row in report['observations']
                    if row['label'] == name+'-'+variant) for variant in commands}
                report['summary'][name] = dict(native_ns=medians['native'], wasm_ns=medians['wasm'], wasm_over_native=medians['wasm']/medians['native'])
                save()
                print(name+' native=%.2f ns wasm=%.2f ns ratio=%.2f' % (medians['native'], medians['wasm'], medians['wasm']/medians['native']), flush=True)
            elif a.mode == 'perf-import':
                # Perf directly programs explicit user-only periods on the
                # selected hybrid PMU. VTune imports the retained raw trace;
                # this is hardware-event evidence, not its full TMA analysis.
                # No device permission or cgroup limit is modified here.
                events = {'CPU_CLK_UNHALTED.REF_TSC': ('umask=0x3', a.reference_period),
                          'CPU_CLK_UNHALTED.THREAD': ('event=0x3c', 240000),
                          'INST_RETIRED.ANY': ('event=0xc0', 240000),
                          'BR_INST_RETIRED.ALL_BRANCHES': ('event=0xc4', 240000),
                          'BR_MISP_RETIRED.ALL_BRANCHES': ('event=0xc5', 10000)}
                config = ','.join(f'{a.pmu}/{encoding},period={period},name={event}/u' for event, (encoding, period) in events.items())
                for variant, command in commands.items():
                    label = name+'-'+variant
                    trace, target = out/(label+'.perf'), out/(label+'-hardware')
                    result = run('collect-'+label, ['/usr/bin/perf', 'record', '-o', trace, '-N', '-B', '-T', '--sample-cpu', '-e', config, '--', *command])
                    observe(label, payload, result, 'explicit-hardware-perf-import')
                    attrs = run('attributes-'+label, ['/usr/bin/perf', 'evlist', '-v', '-i', trace]).stdout.decode()
                    samples = run('samples-'+label, ['/usr/bin/perf', 'script', '--ns', '-F', 'comm,pid,tid,time,event,period,ip', '-i', trace]).stdout.decode()
                    raw = run('records-'+label, ['/usr/bin/perf', 'script', '-D', '-i', trace]).stdout.decode()
                    audit = audit_samples(attrs, samples, raw, {event: period for event, (_, period) in events.items()},
                                          reference_hz, target_comm='native' if variant == 'native' else runtime.name)
                    report['profiles'].append(dict(label=label, analysis='explicit-hardware-perf-import', result=str(target),
                        trace_sha256=sha(trace), audit=audit))
                    save()
                    assert audit['periods_verified'], 'Missing, clamped, lost or throttled hardware samples'
                    run('import-'+label, [VTUNE, '-import', trace, '-result-dir', target, '-source-search-dir', source,
                        '-source-search-dir', out, '-finalization-mode', 'full'])
                    for kind, group in [('functions', 'function'), ('lines', 'function,source-file,source-line')]:
                        run(kind+'-'+label, [VTUNE, '-report', 'hw-events', '-result-dir', target, '-group-by', group, '-format', 'csv'])
                    functions = a.assembly_function or [('round_trip' if variant == 'native' else 'uwvm2::runtime::lib::ros_exchange_memory_erase_host_api')]
                    for index, function in enumerate(functions):
                        run('assembly-'+label+'-'+str(index), [VTUNE, '-report', 'hw-events', '-result-dir', target, '-source-object',
                            'function='+function, '-group-by', 'address', '-column', 'Assembly,Source Line,Hardware Event Count', '-format', 'csv'])
                    report['requested_10us_target_met'] = all(profile['audit']['ten_us_reference_period_verified'] for profile in report['profiles'])
                    save()
            else:
                for variant, command in commands.items():
                    label = name+'-'+variant
                    for analysis in ('hotspots', 'uarch-exploration'):
                        target = out/(label+'-'+analysis)
                        knobs = ['-knob', 'sampling-mode=hw', '-knob', 'enable-stack-collection=true'] if analysis == 'hotspots' else [
                            '-knob', 'pmu-collection-mode=detailed', '-knob', 'dram-bandwidth-limits=false']
                        result = run('collect-'+label+'-'+analysis, [VTUNE, '-collect', analysis, '-no-summary', '-no-analyze-system', '-return-app-exitcode',
                            '-data-limit=400', '-result-dir', target, '-knob', 'sampling-interval='+str(a.sampling_interval), *knobs,
                            '-source-search-dir', source, '-source-search-dir', out, '--', *command])
                        observe(label, payload, result, analysis)
                        report['profiles'].append(dict(label=label, analysis=analysis, requested_interval_ms=a.sampling_interval,
                            period_evidence=vtune_periods(target), result=str(target)))
                        save()
                        for kind, group in [('functions', 'function'), ('lines', 'function,source-file,source-line')]:
                            run(kind+'-'+label+'-'+analysis, [VTUNE, '-report', 'hotspots', '-result-dir', target, '-group-by', group, '-format', 'csv'])
                        functions = a.assembly_function or [('round_trip' if variant == 'native' else 'uwvm2::runtime::lib::ros_exchange_memory_erase_host_api')]
                        for index, function in enumerate(functions):
                            run('assembly-'+label+'-'+analysis+'-'+str(index), [VTUNE, '-report', 'hw-events', '-result-dir', target,
                                '-source-object', 'function='+function, '-group-by', 'address',
                                '-column', 'Assembly,Source Line,Hardware Event Count', '-format', 'csv'])
        success = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(Path(path)) == digest for path, digest in pins.items())
        report['sources_unchanged'] = all(sha(source/name) == digest for name, digest in files.items())
        report['events_after'] = (GATE/'memory.events').read_text()
        report['sampling_settings_after'] = settings()
        report['complete'] = success and report['inputs_unchanged'] and report['sources_unchanged']
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
