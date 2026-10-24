#!/usr/bin/env python3
"""Measure a selected complete-field codec workload and request real VTune PMU data.

The identical independent reference-vector translation unit runs natively and
inside Wasm. Separate driver objects prevent whole-program loop elimination.
Calibration chooses finite iteration counts for each execution route; reported
rates are per complete reference-vector batch, not native ROS latency or robot
performance. Native Clang and WASI Clang revisions/flags remain explicit.

Only the named application's hardware Hotspots and Microarchitecture
Exploration are requested. No system-wide mode, software-sampling substitute,
kernel-module installation, CPU governor change or permission change occurs.
Raw collection/report failures are retained. An explicit hardware request alone
does not establish that usable hardware counters were actually collected: the
result contexts and reports require a separate review before that claim.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import statistics
import subprocess
import time


ENTRY = r'''/* Test-only finite driver; no ROS capability or controller command. */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
extern int codec_case_main(void);
int main(int argc,char **argv) {
  char *end=0;unsigned long n;volatile unsigned result=0;
  if(argc!=2)return 22;
  errno=0;n=strtoul(argv[1],&end,10);
  if(errno||!end||*end||n<1||n>10000000UL)return 22;
  for(unsigned long i=0;i<n;++i)result|=(unsigned)codec_case_main();
  printf("iterations=%lu result=%u\n",n,result);
  return result?1:0;
}
'''


def pin(path):
    with path.open('rb') as stream:
        digest = hashlib.file_digest(stream, 'sha256').hexdigest()
    return dict(path=str(path), bytes=path.stat().st_size, sha256=digest)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    direct = root / 'out/qualify-agibot-r2'
    vectors = direct / 'golden-vectors-ros2_sdk_v1_1_0_main'
    description = json.loads((vectors / 'vectors.json').read_bytes())
    batch = description['products'][0]
    original = vectors / batch['file']
    assert original.stat().st_size == batch['bytes'] and pin(original)['sha256'] == batch['sha256']
    family = source / 'sdk/origin/custom-types/agibot-ros2-sdk-v1-1-0-main-complete'
    vtune = Path('/opt/intel/oneapi/vtune/2026.4/bin64/vtune')
    wasi = Path(inputs['toolchains']) / 'wasi-sdk-33.0-x86_64-linux'
    assert vtune.is_file() and out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    (out / 'driver.c').write_text(ENTRY)
    report = dict(status='RUNNING', commands=[], measurements=[], collections=[],
                  default_authority=[], original_official_cases_run=0,
                  native_ros_or_robot_performance_qualified=False,
                  hardware_data_independently_verified=False,
                  shared_machine_not_exclusive=True,
                  background_load_before=Path('/proc/loadavg').read_text().strip(),
                  scope='One unchanged independent AgiBot reference-vector batch; native versus frozen interpreted Wasm codec execution only',
                  original_vector=pin(original), batch_records=batch['records'],
                  runtime=pin(Path(inputs['runtime'])), vtune=pin(vtune),
                  cpu_affinity=sorted(os.sched_getaffinity(0)),
                  cpu_identity=[line for line in Path('/proc/cpuinfo').read_text().splitlines()
                                if line.startswith(('vendor_id', 'cpu family', 'model\t', 'model name', 'stepping'))][:5],
                  loaded_sampling_modules=[line for line in Path('/proc/modules').read_text().splitlines()
                                           if line.startswith(('sep', 'pax', 'socperf'))],
                  official_cli_references=[
                      'https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2025-0/collect.html',
                      'https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2023-1/run-general-exploration-analysis-command-line.html'])

    def save():
        (out / 'performance.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(label, argv, expected=0, timeout=180):
        directory = out / 'commands' / label
        directory.mkdir()
        started = time.monotonic_ns()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=source,
                stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, check=False, timeout=timeout,
                env=dict(os.environ, VTUNE_USER_DIR=str(out / 'vtune-user'), TMPDIR=str(out / 'tmp')))
        row = dict(label=label, argv=list(map(str, argv)), exit=result.returncode,
                   elapsed_ns=time.monotonic_ns() - started,
                   stdout=pin(directory / 'stdout'), stderr=pin(directory / 'stderr'))
        report['commands'].append(row)
        save()
        if expected is not None:
            assert result.returncode == expected, (label, (directory / 'stderr').read_text()[-2600:])
        return row, (directory / 'stdout').read_bytes()

    def application(route, artifact, iterations, label):
        prefix = [] if route == 'native' else [inputs['runtime'], '-Rint', '--wasip1-global-noinherit-system-environment', '--run']
        row, output = run(label, [*prefix, artifact, str(iterations)], timeout=90)
        assert output == ('iterations=' + str(iterations) + ' result=0\n').encode()
        return row['elapsed_ns'] / 1e9

    try:
        (out / 'tmp').mkdir()
        run('vtune-version', [vtune, '-version'])
        artifacts = {}
        for route, compiler in [('native', Path('/usr/bin/clang-21')), ('wasm', wasi / 'bin/clang')]:
            flags = ['-std=c11', '-O2', '-g1', '-fno-lto', '-I' + str(family)]
            if route == 'native':
                flags += ['-fuse-ld=/usr/bin/ld.lld-21']
            run(route + '-original-vector-object', [compiler, *flags, '-Dmain=codec_case_main', '-c', original, '-o', out / (route + '-vectors.o')])
            run(route + '-driver-object', [compiler, *flags, '-c', out / 'driver.c', '-o', out / (route + '-driver.o')])
            artifact = out / ('codec-native' if route == 'native' else 'codec.wasm')
            link = ['-Wl,--threads=1']
            if route == 'native':
                link += ['-fuse-ld=/usr/bin/ld.lld-21']
            else:
                link += ['-Wl,-z,stack-size=1048576', '-Wl,--initial-memory=16777216', '-Wl,--max-memory=67108864']
            run(route + '-link', [compiler, *link, out / (route + '-vectors.o'), out / (route + '-driver.o'), '-o', artifact])
            if route == 'wasm':
                run('validate-actual-benchmark-wasm', [inputs['validator'], 'validate', artifact])
            artifacts[route] = artifact
            pilot = application(route, artifact, 128, route + '-pilot-128')
            count = max(128, min(10000000, int(128 * 8 / max(pilot, .001))))
            # Runtime startup is included. Three fresh processes report a median
            # per batch; no cross-process startup subtraction is invented.
            samples = [application(route, artifact, count, route + '-timing-' + str(n)) for n in range(3)]
            report['measurements'].append(dict(route=route, artifact=pin(artifact), iterations=count,
                seconds=samples, median_seconds=statistics.median(samples),
                median_seconds_per_reference_batch=statistics.median(samples) / count,
                process_startup_included=True, compiler=pin(compiler)))
            save()
        for route, artifact in artifacts.items():
            count = next(row['iterations'] for row in report['measurements'] if row['route'] == route)
            target = [artifact, str(count)] if route == 'native' else [inputs['runtime'], '-Rint',
                '--wasip1-global-noinherit-system-environment', '--run', artifact, str(count)]
            for analysis in ['hotspots', 'uarch-exploration']:
                label = route + '-' + analysis
                result = out / label
                knobs = ['-knob', 'sampling-mode=hw', '-knob', 'enable-stack-collection=false'] if analysis == 'hotspots' else []
                row, output = run(label, [vtune, '-collect', analysis, *knobs, '-no-analyze-system',
                    '-no-follow-child', '-data-limit=16', '-r', result, '--', *target], expected=None, timeout=180)
                collection = dict(route=route, analysis=analysis, result=str(result),
                                  collection_exit=row['exit'], explicit_hardware_request=True, reports=[])
                report['collections'].append(collection)
                if row['exit'] == 0:
                    assert ('iterations=' + str(count) + ' result=0\n').encode() in output
                    for kind in ['summary', 'hw-events']:
                        observed, _ = run(label + '-' + kind, [vtune, '-report', kind, '-r', result,
                            '-format=csv', '-report-output', out / (label + '-' + kind + '.csv')], expected=None)
                        collection['reports'].append(dict(kind=kind, exit=observed['exit']))
                save()
        assert pin(vtune) == report['vtune'] and pin(original) == report['original_vector']
        report['status'] = 'SELECTED_CODEC_TIMINGS_AND_VTUNE_HW_REQUEST_OUTCOMES_RECORDED_REVIEW_REQUIRED'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        report['background_load_after'] = Path('/proc/loadavg').read_text().strip()
        save()


if __name__ == '__main__':
    main()
