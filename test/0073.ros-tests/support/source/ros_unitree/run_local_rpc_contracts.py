#!/usr/bin/env python3
"""Run real C11/C++20 local RPC contracts inside an explicitly pinned cgroup.

This lane compiles the actual SDK headers and scenario sources with the selected
native compilers, in order, with optimization and undefined-behavior checks. It
is local SDK validation, not a ROS, QEMU, hardware or official-suite result.
Output is fresh and bounded; no SDK, robot endpoint, permission or tool is added.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import shutil
import signal
import subprocess
import time


def pin(path):
    raw = path.read_bytes()
    return dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--cc', type=Path, required=True)
    parser.add_argument('--cxx', type=Path, required=True)
    parser.add_argument('--cgroup-identity', type=Path, required=True,
        help='Explicit JSON with boot_id, membership, device, inode and cpus pins.')
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    identity = json.loads(args.cgroup_identity.read_text())
    if set(identity) != {'boot_id', 'membership', 'device', 'inode', 'cpus'}:
        raise ValueError('closed explicit cgroup identity required')
    group = Path('/sys/fs/cgroup') / identity['membership'].lstrip('/')

    def admit():
        membership = Path('/proc/self/cgroup').read_text().strip()
        facts = dict(boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
            membership=membership.removeprefix('0::'), device=group.stat().st_dev,
            inode=group.stat().st_ino, cpus=(group / 'cpuset.cpus.effective').read_text().strip())
        if facts != identity or membership != '0::' + identity['membership']:
            raise RuntimeError('actual original cgroup identity differs')
        if (group / 'memory.max').read_text().strip() != '68719476736' or (
                group / 'memory.swap.max').read_text().strip() != '0':
            raise RuntimeError('actual hard64GiB/swap0 limits required')
        if int((group / 'memory.current').read_text()) > 32 * 1024**3:
            raise RuntimeError('retain32GiB aggregate cgroup headroom for this small lane')
        return (group / 'memory.events').read_text()

    before_events = admit()
    out = args.out.absolute()
    if shutil.disk_usage(out.parent).free < 3 * 1024**3:
        raise RuntimeError('retain3GiB free disk')
    out.mkdir(mode=0o700)
    include = source / 'sdk/origin/robots/unitree/include'
    tests = source / 'sdk/origin/robots/unitree/tests'
    inputs = [include / 'roswasm_unitree/topic_rpc.h',
              include / 'roswasm_unitree/topic_rpc.hpp',
              tests / 'topic_rpc_contract.c', tests / 'topic_rpc_lifetime_contract.c',
              tests / 'topic_rpc_lifetime_contract.cpp', Path(__file__).resolve()]
    input_pins = [pin(p) for p in inputs]
    report = dict(passed=False, qualification='Actual local SDK native contracts only; '
        'no ROS, Guest, QEMU, hardware, performance or upstream-suite acceptance.',
        cgroup=identity, cgroup_events_before=before_events, source_inputs=input_pins,
        identity_input=pin(args.cgroup_identity), commands=[], native_contract_runs=0)
    tools = [args.cc.resolve(strict=True), args.cxx.resolve(strict=True)]
    report['compiler_inputs'] = [pin(p) for p in tools]
    environment = dict(PATH='/usr/bin:/bin', LC_ALL='C', LANG='C',
        TMPDIR=str(out), UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=0')

    def child_limits():
        resource.setrlimit(resource.RLIMIT_AS, (512 * 1024**2, 512 * 1024**2))
        resource.setrlimit(resource.RLIMIT_CPU, (15, 15))
        resource.setrlimit(resource.RLIMIT_FSIZE, (4 * 1024**2, 4 * 1024**2))
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))

    def run(label, argv):
        if admit() != before_events:
            raise RuntimeError('cgroup event counters changed before ' + label)
        record = dict(label=label, argv=list(map(str, argv)))
        report['commands'].append(record)
        started = time.monotonic()
        with (out / (label + '.stdout')).open('xb') as stdout, (
                out / (label + '.stderr')).open('xb') as stderr:
            process = subprocess.Popen(record['argv'], cwd=out, env=environment,
                stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr,
                start_new_session=True, preexec_fn=child_limits)
            try:
                record['returncode'] = process.wait(timeout=20)
            finally:
                # Compiler and linker descendants inherit this owned session.
                try:
                    os.killpg(process.pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
                process.wait()
        record['elapsed_seconds'] = time.monotonic() - started
        record['stdout'] = pin(out / (label + '.stdout'))
        record['stderr'] = pin(out / (label + '.stderr'))
        if record['returncode']:
            raise RuntimeError('actual command failed: ' + label)
        if admit() != before_events:
            raise RuntimeError('cgroup event counters changed after ' + label)

    try:
        for compiler, suffix, standard in ((tools[0], 'c', 'c11'), (tools[1], 'cpp', 'c++20')):
            names = ['topic_rpc_lifetime_contract']
            if suffix == 'c': names.insert(0, 'topic_rpc_contract')
            for variant, flags in (
                    ('optimized', ['-O2']),
                    ('undefined', ['-O1', '-fsanitize=undefined', '-fno-sanitize-recover=all'])):
                for name in names:
                    label = name + '-' + suffix + '-' + variant
                    artifact = out / label
                    run(label + '-compile', [compiler, '-std=' + standard, '-Wall',
                        '-Wextra', '-Werror', '-pedantic', *flags, '-I', include,
                        tests / (name + '.' + suffix), '-o', artifact])
                    report.setdefault('artifacts', []).append(pin(artifact))
                    run(label + '-run', [artifact])
                    report['native_contract_runs'] += 1
        if [pin(p) for p in inputs] != input_pins:
            raise RuntimeError('source inputs changed during execution')
        if [pin(p) for p in tools] != report['compiler_inputs']:
            raise RuntimeError('compiler inputs changed during execution')
        report['passed'] = True
    except BaseException as error:
        report['failure'] = repr(error)
        raise
    finally:
        report['cgroup_events_after'] = (group / 'memory.events').read_text()
        report['maximum_individual_child_rss_bytes'] = (
            resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss * 1024)
        (out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(passed=True, native_contract_runs=report['native_contract_runs'])))


if __name__ == '__main__':
    main()
