#!/usr/bin/env python3
"""Qualify external model/ROS attachments, relay fencing and bounded RPC erasure.

This is a test driver, not a deployment service. All builds and descendants
must already belong to the existing 64 GiB zero-swap cgroup. Receipts bind
compiler flags, immutable source snapshots and observed exit status.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import shlex
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()

def guard():
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP / 'memory.max').read_text().strip() == '68719476736'
    assert (GROUP / 'memory.swap.max').read_text().strip() == '0'
    assert os.sched_getaffinity(0).issubset({4, 5, 6, 7})

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    guard()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent / 'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    source = out / 'source'
    units = ['communication_attachment', 'model_attachment', 'communication_private_link', 'communication_worker_async', 'communication_worker_stage']
    flags = ['-std=c++23', '-O3', '-g', '-Wall', '-Wextra', '-Werror', '-pthread', '-fno-rtti']
    report = dict(complete=False, scope=__doc__, cgroup=str(GROUP), memory_max=68719476736, swap_max=0,
                  events_before=(GROUP/'memory.events').read_text(), affinity=sorted(os.sched_getaffinity(0)),
                  flags=flags, sources={}, commands=[], binaries={}, compilers={})
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    def run(label, argv):
        guard()
        start = time.monotonic()
        result = subprocess.run(argv, cwd=ROOT, capture_output=True, text=True, timeout=240,
                                env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        (out/(label+'.stdout')).write_text(result.stdout)
        (out/(label+'.stderr')).write_text(result.stderr)
        report['commands'].append(dict(label=label, argv=argv, exit=result.returncode, seconds=time.monotonic()-start))
        save()
        print(label, result.returncode, flush=True)
        if result.returncode:
            raise RuntimeError(result.stderr[-9000:])
        return result
    success = False
    try:
        files = {Path(__file__).resolve()}
        for unit in units:
            command = ['/usr/bin/clang++-22', *flags, '-I'+str(ROOT/'src'), '-isystem', str(ROOT/'third-parties/fast_io/include'),
                       '-MM', '-MT', 'objects', str(ROOT/'test/0084.ros-runtime'/ (unit+'.cpp'))]
            result = run(unit+'-dependencies', command)
            names = shlex.split(result.stdout.replace('\\\n', ' '))
            assert names.pop(0) == 'objects:'
            for name in names:
                path = Path(name).resolve()
                assert path.is_relative_to(ROOT)
                files.add(path)
        files.update(p.resolve() for p in (ROOT/'third-parties/fast_io/include').rglob('*') if p.is_file())
        for path in sorted(files):
            relative = path.relative_to(ROOT)
            target = source/relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(path, target)
            report['sources'][str(relative)] = sha(target)
        save()
        for unit in units:
            variants = [('asan', '/usr/bin/clang++-22', ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']),
                        ('gcc', '/usr/bin/g++-13', [])]
            if unit in ('communication_private_link', 'communication_worker_stage'):
                variants.append(('allocation', '/usr/bin/g++-13', ['-DUWVM_PRIVATE_LINK_ALLOCATION' if unit == 'communication_private_link' else '-DUWVM_WORKER_STAGE_ALLOCATION', '-Wl,--wrap=malloc', '-Wl,--wrap=calloc', '-Wl,--wrap=realloc']))
            if unit == 'communication_worker_async':
                variants.append(('read-fence', '/usr/bin/g++-13', ['-DUWVM_RPC_READ_FENCE', '-Wl,--wrap=read']))
            for variant, compiler, extra in variants:
                label = unit+'-'+variant
                binary = out/label
                report['compilers'][compiler] = sha(compiler)
                run(label+'-compile', [compiler, *flags, '-I'+str(source/'src'), '-isystem', str(source/'third-parties/fast_io/include'),
                                      *extra, str(source/'test/0084.ros-runtime'/(unit+'.cpp')), '-o', str(binary)])
                report['binaries'][str(binary)] = sha(binary)
                marker = 'passed 11 external model security groups' if unit == 'model_attachment' else 'PASS '
                assert marker in run(label, [str(binary)]).stdout
        success = True
    finally:
        guard()
        report['sources_unchanged'] = all(sha(ROOT/name) == digest for name,digest in report['sources'].items())
        report['snapshot_unchanged'] = all(sha(source/name) == digest for name,digest in report['sources'].items())
        report['events_after'] = (GROUP/'memory.events').read_text()
        report['complete'] = success and report['sources_unchanged'] and report['snapshot_unchanged']
        save()
    assert report['complete']

if __name__ == '__main__':
    main()
