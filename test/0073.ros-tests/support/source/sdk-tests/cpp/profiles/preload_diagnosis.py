#!/usr/bin/env python3
"""Small actual Guest reductions for independently instanced Wasm preloads."""

import json
import hashlib
import os
from pathlib import Path
import subprocess
import sys

source, sdk, output = map(Path, sys.argv[1:4])
output.mkdir()
assert os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') == '1'
wasi = Path('/home/macromodel/Documents/uwvm-test-261018/agents/10/staged-wasi-r1/staged/wasi-sdk')
runtime = (
    Path(sys.argv[4])
    if len(sys.argv) > 4
    else Path(os.environ['UWVM_RESOURCE_RUN']) / 'inputs/uwvm2-ros'
)
assert (
    runtime.is_absolute() and runtime.is_file()
), 'An explicit retained actual Runtime input is required'
runtime_before = hashlib.sha256(runtime.read_bytes()).hexdigest()
commands = []
rows = []


def run(name, argv):
    stdout = output / (name + '.stdout')
    stderr = output / (name + '.stderr')
    with stdout.open('wb') as out, stderr.open('wb') as err:
        r = subprocess.run(list(map(str, argv)), stdout=out, stderr=err, timeout=60, check=False)
    commands.append(
        dict(
            label=name,
            argv=list(map(str, argv)),
            exit_code=r.returncode,
            stdout=stdout.name,
            stderr=stderr.name,
        )
    )
    return r.returncode == 0


flags = [
    wasi / 'bin/clang++',
    '--target=wasm32-wasip1',
    '-std=c++20',
    '-DROSWASM_CPP_STANDARD=20',
    '-fno-exceptions',
    '-fno-rtti',
    '-O2',
    '-I' + str(sdk / 'include'),
]
for p in ('ros', 'ros1', 'ros2'):
    for l in ('cpp', 'c'):
        flags += ['-I' + str(sdk / 'include/platform' / p / l)]
fixtures = {
    'leaf': 'extern "C" __attribute__((export_name("echo"))) unsigned echo(unsigned n){return n+1;}\n',
    'leaf-main': 'extern "C" __attribute__((import_module("leaf"),import_name("echo"))) unsigned echo(unsigned);\nint main(){return echo(40)==41?0:1;}\n',
    'callbacks-main': '#include <stdio.h>\nextern "C" __attribute__((import_module("callbacks"),import_name("count"))) unsigned count();\nextern "C" __attribute__((import_module("callbacks"),import_name("notify"))) unsigned notify(unsigned);\nint main(){puts("before callbacks");if(count())return 1;if(notify(13))return 2;puts("after callbacks");return count()==1?0:3;}\n',
    'codec-main': '#include <stdio.h>\nextern "C" __attribute__((import_module("codec"),import_name("check_twist"))) unsigned check_twist();\nint main(){puts("before codec");unsigned n=check_twist();puts("after codec");return n==48?0:1;}\n',
    'client-main': '#include <stdio.h>\nextern "C" __attribute__((import_module("client"),import_name("denied_once"))) unsigned denied_once();\nint main(){puts("before client");unsigned n=denied_once();puts("after client");return n==13?0:1;}\n',
}
combined = '''#include <stdio.h>
extern "C" __attribute__((import_module("callbacks"),import_name("count"))) unsigned count();
extern "C" __attribute__((import_module("codec"),import_name("check_twist"))) unsigned check_twist();
extern "C" __attribute__((import_module("client"),import_name("denied_once"))) unsigned denied_once();
int main(){puts("before count");if(count())return 1;puts("before codec");if(check_twist()!=48)return 2;puts("before client");if(denied_once()!=13)return 3;puts("before final count");return count()==1?0:4;}
'''
fixtures['combined-main'] = combined
fixtures['combined-init-main'] = combined.replace(
    'int main(){',
    'extern "C" __attribute__((import_module("callbacks"),import_name("_initialize"))) void ci();\nextern "C" __attribute__((import_module("codec"),import_name("_initialize"))) void xi();\nextern "C" __attribute__((import_module("client"),import_name("_initialize"))) void ri();\nint main(){puts("before initialize");ci();xi();ri();puts("after initialize");',
)
for name, text in fixtures.items():
    (output / (name + '.cpp')).write_text(text)
modules = {}
for name in ('leaf', 'callbacks', 'codec', 'client'):
    file = (
        output / 'leaf.cpp'
        if name == 'leaf'
        else source / 'sdk/example/platform/ros/cpp/profiles/preload' / (name + '.cpp')
    )
    exe = output / (name + '.wasm')
    extra = ['-nostdlib', '-Wl,--no-entry'] if name == 'leaf' else ['-mexec-model=reactor']
    good = run('compile-' + name, flags + extra + [file, '-o', exe])
    rows.append(dict(layer='wasm-compile', label=name, status='PASS' if good else 'FAIL'))
    if good:
        modules[name] = exe
        run('imports-' + name, ['/usr/bin/wasm-objdump', '-x', exe])
        run('validate-' + name, ['/usr/bin/wasm-validate', exe])
for name, names in [
    ('leaf', ['leaf']),
    ('callbacks', ['callbacks']),
    ('codec', ['codec']),
    ('client', ['callbacks', 'client']),
    ('combined', ['callbacks', 'codec', 'client']),
    ('combined-init', ['callbacks', 'codec', 'client']),
]:
    exe = output / (name + '-main.wasm')
    good = run('compile-' + name + '-main', flags + [output / (name + '-main.cpp'), '-o', exe])
    if good and all(n in modules for n in names):
        argv = [
            runtime,
            '--runtime-int',
            '-Rct',
            '0',
            '--wasm-set-main-module-name',
            'main',
            '--wasip1-global-disable',
            '--wasip1-global-noinherit-system-environment',
        ]
        for n in ['main'] + names:
            argv += [
                '--wasip1-single-create',
                n,
                '--wasip1-single-enable',
                n,
                '--wasip1-single-hide-host-api',
                n,
                '--wasip1-single-noinherit-system-environment',
                n,
            ]
            for right in (
                'realtime-clock',
                'monotonic-clock',
                'process-clock',
                'thread-clock',
                'random',
                'proc-raise',
                'sched-yield',
                'stdio-full-rights',
            ):
                argv += ['--wasip1-single-deny', n, right]
        for n in names:
            argv += ['--wasm-preload-library', modules[n], n]
        good = run('guest-' + name, argv + ['--run', exe])
    rows.append(
        dict(
            layer='actual-preload-reduction',
            label=name,
            modules=len(names) + 1,
            status='PASS' if good else 'FAIL',
            ros_grants=[],
        )
    )
runtime_after = hashlib.sha256(runtime.read_bytes()).hexdigest()
rows.append(
    dict(
        layer='input-integrity',
        label='runtime-unchanged',
        status='PASS' if runtime_before == runtime_after else 'FAIL',
    )
)
result = dict(
    status='PASS' if all(r['status'] == 'PASS' for r in rows) else 'FAIL',
    cases=rows,
    commands=commands,
    native_ros=0,
    runtime=dict(path=str(runtime), before_sha256=runtime_before, after_sha256=runtime_after),
)
(output / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(rows), flush=True)
sys.exit(result['status'] != 'PASS')
