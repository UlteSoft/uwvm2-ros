#!/usr/bin/env python3
"""Measure mandatory native unwind validation and fresh-process loading.

Every current-VM process must execute the real generated-chain witness. The
10 ms native target is a measured acceptance budget, never a probe timeout.
QEMU results are rejected. Raw streams, argv, elapsed wall time and cache proof
are retained per process. This does not qualify ROS transport performance.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import resource
import statistics
import struct
import subprocess
import sys
import time

ANSI = re.compile(rb'\x1b\[[0-9;?]*[ -/]*[@-~]')
WASM = bytes.fromhex('0061736d0100000001040160000003020100070a01065f737461727400000a040102000b')


def digest(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def percentile(values, quantile):
    ordered = sorted(values)
    return ordered[min(len(ordered)-1, int((len(ordered)-1)*quantile+0.999999))]


def child_cgroup(process, binary):
    """Read the executing ELF's actual membership and effective kernel limits."""
    proc = Path('/proc')/str(process.pid)
    membership = (proc/'cgroup').read_text()
    relative = next(line[3:] for line in membership.splitlines() if line.startswith('0::'))
    root = Path('/sys/fs/cgroup')
    group = root/relative.lstrip('/')
    assert group.is_relative_to(root) and process.poll() is None
    # Popen's exec-error handshake has already completed for this exact argv.
    # Cache-origin key protection can subsequently deny /proc/PID/exe access;
    # retain that observation without disabling the VM's protection. The ELF
    # bytes are hashed before and after the complete cohort.
    executable = {'identity_basis': 'successful direct exec handshake and pre/post ELF SHA256'}
    try:
        actual, expected = (proc/'exe').stat(), binary.stat()
        assert (actual.st_dev, actual.st_ino) == (expected.st_dev, expected.st_ino)
        executable.update(procfs_access='verified', executable=os.readlink(proc/'exe'),
                          device=actual.st_dev, inode=actual.st_ino)
    except PermissionError:
        executable['procfs_access'] = 'permission-denied'
    ancestors = []
    for path in (group, *group.parents):
        if not path.is_relative_to(root):
            break
        ancestors.append({key: (path/key).read_text().strip()
                          for key in ('memory.max', 'memory.swap.max')})
        if path == root:
            break
    effective = {key: min(int(row[key]) for row in ancestors if row[key] != 'max')
                 for key in ('memory.max', 'memory.swap.max')}
    assert effective == {'memory.max': 68719476736, 'memory.swap.max': 0}
    assert str(process.pid) in (group/'cgroup.procs').read_text().split()
    return {'pid': process.pid, 'membership': membership, 'group': str(group),
            'effective': effective, 'ancestors': ancestors, **executable}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--uwvm', type=Path, required=True)
    p.add_argument('--baseline', type=Path)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--samples', type=int, default=31)
    p.add_argument('--native-target-ns', type=int, default=10_000_000)
    a = p.parse_args()
    if sys.platform != 'linux' or platform.machine() not in ('x86_64', 'aarch64'):
        p.error('run directly on qualified native Linux hardware')
    if a.samples < 15:
        p.error('at least 15 interleaved measured processes are required')
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    a.out.mkdir(parents=True, exist_ok=False)
    cpu = min(os.sched_getaffinity(0)); os.sched_setaffinity(0, {cpu})
    cg = Path('/sys/fs/cgroup')
    limits = {name: (cg/name).read_text().strip() for name in ('memory.max', 'memory.swap.max', 'cgroup.procs')}
    assert limits['memory.max'] == '68719476736' and limits['memory.swap.max'] == '0'
    assert str(os.getpid()) in limits['cgroup.procs'].split()
    wasm = a.out/'empty.wasm'; wasm.write_bytes(WASM)
    products = {'current': a.uwvm.resolve(strict=True)}
    if a.baseline:
        products['baseline'] = a.baseline.resolve(strict=True)
    expected_machine = {'x86_64': 62, 'aarch64': 183}[platform.machine()]
    report = {'passed': False, 'scope': 'native generated-chain witness and finite cold/warm process loading',
              'cgroup': limits, 'cpu_affinity': [cpu], 'machine': platform.machine(),
              'wasm_sha256': digest(wasm), 'products': {}, 'rows': [], 'summary': {}}
    for label, binary in products.items():
        header = binary.read_bytes()[:64]
        assert header[:6] == b'\x7fELF\x02\x01' and struct.unpack_from('<H', header, 18)[0] == expected_machine
        report['products'][label] = {'path': str(binary), 'sha256': digest(binary)}
    def save():
        (a.out/'result.json').write_text(json.dumps(report, indent=2)+'\n')
    try:
        for iteration in range(a.samples+3):
            order = list(products)
            if iteration%2:
                order.reverse()
            for label in order:
                for phase in ('cold', 'warm'):
                    cache = a.out/(label+'-cache')
                    argv = [str(products[label]), '-Raot', '-Rct', '0', '--log-verbose',
                            '--runtime-compiler-log', 'err', '--runtime-llvm-jit-cache-seal-process',
                            '-Rllvm-cache-path', 'path', str(cache), '--run', str(wasm)]
                    # A distinct cold directory requires real compilation; it is
                    # retained and becomes this iteration's warm directory.
                    cache = a.out/(label+'-cache-'+str(iteration))
                    argv[argv.index('path')+1] = str(cache)
                    stamp = f'{label}-{phase}-{iteration}'
                    failure = None
                    proof = None
                    begin = time.perf_counter_ns()
                    with subprocess.Popen(argv, stdin=subprocess.DEVNULL,
                                          stdout=subprocess.PIPE, stderr=subprocess.PIPE) as process:
                        try:
                            proof = child_cgroup(process, products[label])
                            stdout, stderr = process.communicate(timeout=90)
                        except BaseException as error:
                            failure = error
                            process.kill()
                            stdout, stderr = process.communicate()
                    elapsed = time.perf_counter_ns()-begin
                    row = {'label': label, 'phase': phase, 'iteration': iteration, 'warmup': iteration<3,
                           'argv': argv, 'returncode': process.returncode, 'wall_ns': elapsed,
                           'actual_child_cgroup': proof}
                    for stream in ('stdout', 'stderr'):
                        data = stdout if stream == 'stdout' else stderr
                        (a.out/(stamp+'.'+stream)).write_bytes(data)
                        row[stream+'_sha256'] = hashlib.sha256(data).hexdigest()
                    report['rows'].append(row); save()
                    if failure is not None:
                        row['failure'] = repr(failure)
                        save()
                        raise failure
                    assert process.returncode == 0 and stdout == b'', stamp
                    text = ANSI.sub(b'', stderr)
                    if label == 'current':
                        probes = re.findall(rb'\[unwind-check\] status=(\w+) elapsed_ns=(\d+)', text)
                        assert len(probes) == 1 and probes[0][0] == b'ok', (stamp, probes)
                        row['unwind_probe_ns'] = int(probes[0][1])
                        assert b'call_stack=unwind ' in text or phase == 'warm', stamp
                    if phase == 'cold':
                        assert re.search(rb'object-cache-store-complete [^\n]*status=ok\b', text), stamp
                    else:
                        assert re.search(rb'object-cache-hit [^\n]*signature_verified=1\b', text), stamp
                        assert b'object-cache-store-enqueue ' not in text, stamp
                    save()
        for label in products:
            for phase in ('cold', 'warm'):
                rows = [r for r in report['rows'] if r['label']==label and r['phase']==phase and not r['warmup']]
                vals = [r['wall_ns'] for r in rows]
                report['summary'][label+'-'+phase] = {'samples': len(vals), 'wall_median_ns': statistics.median(vals),
                    'wall_p95_ns': percentile(vals, .95)}
        probes = [r['unwind_probe_ns'] for r in report['rows'] if r['label']=='current' and not r['warmup']]
        report['unwind'] = {'samples': len(probes), 'median_ns': statistics.median(probes),
                            'p95_ns': percentile(probes, .95), 'max_ns': max(probes), 'native_target_ns': a.native_target_ns}
        report['correctness_passed'] = True
        report['native_budget_passed'] = report['unwind']['p95_ns'] <= a.native_target_ns
        if a.baseline:
            report['current_to_baseline_median_ratio'] = {phase:
                report['summary']['current-'+phase]['wall_median_ns']/report['summary']['baseline-'+phase]['wall_median_ns']
                for phase in ('cold','warm')}
        report['passed'] = report['native_budget_passed']
        for label, binary in products.items():
            assert digest(binary) == report['products'][label]['sha256']
        if not report['passed']:
            raise RuntimeError('accurate witness exceeds measured native startup budget; optimize before acceptance')
    except BaseException as error:
        report['failure'] = repr(error)
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
