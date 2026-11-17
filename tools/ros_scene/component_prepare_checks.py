#!/usr/bin/env python3
"""Qualify transactional supervisor preparation with real threads and sealed images.

Run inside the existing 64 GiB zero-swap gate. Inject each guarded-stack,
attribute and thread-creation failure at all three binding positions. A frozen
reference must fail the new rollback assertions; Clang, GCC, ASan/UBSan and
ThreadSanitizer candidates must pass. No external component is launched by
these preparation tests. Runtime component semantics have separate receipts.
"""
import argparse
import json
import os
from pathlib import Path
import resource
import shutil
import signal
import subprocess
import time
from module_exchange_performance import ROOT, GATE, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--reference-root', type=Path, required=True)
    parser.add_argument('--component-receipt', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    roots = dict(reference=args.reference_root.resolve(), corrected=args.source_root.resolve())
    manifests = {key: json.loads((root/'source-manifest.json').read_text()) for key, root in roots.items()}
    assert all(sha(roots[k]/p) == value for k, rows in manifests.items() for p, value in rows.items())
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    fixture = out/'fixtures'
    fixture.mkdir()
    pins = {str(Path(__file__).resolve()): sha(__file__)}
    for path in (ROOT/'test/0084.ros-runtime').iterdir():
        if path.suffix in ('.cpp', '.h', '.inc'):
            target = fixture/path.name
            shutil.copyfile(path, target)
            pins[str(path)] = pins[str(target)] = sha(path)
    receipt = args.component_receipt.resolve()
    original = json.loads(receipt.read_text())
    assert original['complete']
    images = [receipt.parent/'images'/name for name in (
        'bootstrap', 'backend.so', 'matrix.weights', 'ld-linux-x86-64.so.2',
        'libc.so.6', 'libstdc++.so.6', 'libgcc_s.so.1', 'libm.so.6', 'bwrap')]
    for path in [receipt, *images, *(root/'source-manifest.json' for root in roots.values())]:
        pins[str(path)] = sha(path)
    report = dict(complete=False, scope=__doc__, invocation=os.sys.argv,
        source_roots={k: str(v) for k, v in roots.items()}, inputs=pins,
        cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
        events_before=(GATE/'memory.events').read_text(), commands=[])

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(label, argv, expected=0, tsan=False):
        verify_gate()
        argv = list(map(str, argv))
        started = time.monotonic()
        process = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE, start_new_session=True,
            env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        timed_out = False
        try:
            stdout, stderr = process.communicate(timeout=300)
        except subprocess.TimeoutExpired:
            timed_out = True
            os.killpg(process.pid, signal.SIGKILL)
            stdout, stderr = process.communicate()
        (out/(label+'.stdout')).write_bytes(stdout)
        (out/(label+'.stderr')).write_bytes(stderr)
        startup_failure = (tsan and not stdout and
            stderr.startswith(b'FATAL: ThreadSanitizer: unexpected memory mapping'))
        report['commands'].append(dict(label=label, argv=argv, exit=process.returncode, expected_exit=expected,
            timeout=timed_out, sanitizer_startup_failure=startup_failure, seconds=time.monotonic()-started))
        save()
        assert not timed_out, label
        if startup_failure:
            return False
        assert process.returncode == expected, (label, stderr[-6000:])
        if expected:
            assert b'Assertion' in stderr and (b'prepare_fault::joined' in stderr or b'usage.retired' in stderr)
        elif label.startswith('run-'):
            assert b'PASS component preparation' in stdout and b'WARNING: ThreadSanitizer:' not in stderr
        print(label+': '+str(process.returncode), flush=True)
        return True

    complete = False
    try:
        cases = [(mode, i) for mode in ('guard', 'attribute', 'stack', 'thread')
                 for i in range(1, 7 if mode == 'guard' else 4)]
        for variant, compiler, root, flags in [
            ('reference', '/usr/bin/clang++-22', roots['reference'], []),
            ('clang', '/usr/bin/clang++-22', roots['corrected'], []),
            ('sanitized', '/usr/bin/clang++-22', roots['corrected'], ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']),
            ('gcc', '/usr/bin/g++-13', roots['corrected'], []),
            ('threaded', '/usr/bin/clang++-22', roots['corrected'], ['-fsanitize=thread', '-fno-omit-frame-pointer'])]:
            binary = out/variant
            run('compile-'+variant, [compiler, '-std=c++23', '-O3', '-g', '-UNDEBUG', '-Wall', '-Wextra', '-Werror', '-pthread',
                '-I'+str(root/'src'), '-isystem', root/'third-parties/fast_io/include', *flags,
                fixture/'component_prepare_transaction.cpp', '-Wl,--wrap=mprotect,--wrap=pthread_attr_init,--wrap=pthread_attr_destroy',
                '-Wl,--wrap=pthread_attr_setstack,--wrap=pthread_create,--wrap=pthread_join', '-ldl', '-o', binary])
            pins[str(binary)] = sha(binary)
            selected = cases if variant == 'reference' else [*cases, ('success', 0), ('race', 0)]
            for mode, ordinal in selected:
                for attempt in range(1, 9 if variant == 'threaded' else 2):
                    if run(f'run-{variant}-{mode}-{ordinal}-{attempt}', [binary, *images, mode, ordinal],
                           -signal.SIGABRT if variant == 'reference' else 0, tsan=variant == 'threaded'):
                        break
                else:
                    raise AssertionError('ThreadSanitizer did not initialize in eight recorded attempts')
        complete = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(name) == value for name, value in pins.items())
        report['sources_unchanged'] = all(sha(roots[k]/p) == value for k, rows in manifests.items() for p, value in rows.items())
        report['events_after'] = (GATE/'memory.events').read_text()
        report['complete'] = complete and report['inputs_unchanged'] and report['sources_unchanged']
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
