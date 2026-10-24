#!/usr/bin/env python3
"""Measure real relocatable WASI sysroot assembly, compilation and validation.

This does not qualify ROS transport, native ROS, another Guest language, hardware
or an upstream test suite. It uses the selected real WASI SDK and wasm-tools.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


class Writer:
    def __init__(self, root):
        self.root = root
        self.paths = {}

    def emit(self, path, body):
        path = Path(path)
        if path.is_absolute() or '..' in path.parts:
            raise ValueError('confined test output required')
        destination = self.root / path
        if str(path) in self.paths:
            if destination.read_bytes() != body:
                raise ValueError('conflicting output')
            return
        destination.parent.mkdir(parents=True, exist_ok=True)
        with destination.open('xb') as stream:
            stream.write(body)
        self.paths[str(path)] = hashlib.sha256(body).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--tools', type=Path, required=True)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--wasm-tools', type=Path, required=True)
    parser.add_argument('--runtime', type=Path)
    args = parser.parse_args()
    sys.path.insert(0, str(args.tools.resolve(strict=True)))
    from resource_contract import validate_resource_owner
    from wasi_sysroot import install_sysroot
    resource = validate_resource_owner()
    root = Path.cwd()
    report = dict(status='RUNNING', resource=resource, cases=[], commands=[], default_authority=[],
        layers=dict(source=0, compile=0, wasm_validation=0, actual_guest=0,
            native_ros=0, gateway=0, upstream=0, qemu=0, vtune_hardware=0))
    result_path = root / 'wasi-result.json'

    def save():
        result_path.write_text(json.dumps(report, indent=2, sort_keys=True) + '\n')

    def case(name, layer, facts=None):
        report['cases'].append(dict(name=name, layer=layer, status='PASS', facts=facts or {}))
        report['layers'][layer] += 1
        save()

    def run(name, argv, expected=0):
        stdout, stderr = root / (name + '.stdout'), root / (name + '.stderr')
        with stdout.open('xb') as out, stderr.open('xb') as err:
            value = subprocess.run(list(map(str, argv)), stdout=out, stderr=err,
                stdin=subprocess.DEVNULL, timeout=90)
        row = dict(name=name, argv=list(map(str, argv)), exit=value.returncode,
            stdout_sha256=hashlib.sha256(stdout.read_bytes()).hexdigest(),
            stderr_sha256=hashlib.sha256(stderr.read_bytes()).hexdigest())
        report['commands'].append(row)
        save()
        if (expected == 0 and value.returncode != 0) or (expected == 'nonzero' and value.returncode == 0):
            raise RuntimeError((row, stderr.read_text()[-4000:]))
        return stdout.read_text()

    try:
        sdk = args.sdk.resolve(strict=True)
        sysroot = sdk / 'share/wasi-sysroot'
        original = root / 'assembled'
        writer = Writer(original)
        metadata = install_sysroot(writer, sysroot, {'c', 'cpp'}, sdk)
        if not metadata['compiler_resources_installed'] or metadata['cpp_header_root'] != 'include/c++/v1':
            raise RuntimeError('compiler target resources and unique C++ header root required')
        if any((original / name).exists() for name in ('include/wasm32-wasip1/noeh/c++', 'include/wasm32-wasip1/eh',
                'lib/wasm32-wasip1/eh', 'lib/wasm32-wasip1/llvm-lto')):
            raise RuntimeError('duplicate/unselected header or target profile installed')
        relocated = root / 'relocated-sdk'
        original.rename(relocated)
        for spelling, digest in writer.paths.items():
            path = relocated / spelling
            if path.is_symlink() or hashlib.sha256(path.read_bytes()).hexdigest() != digest:
                raise RuntimeError('relocated ordinary file pin mismatch: ' + spelling)
        case('complete-real-sysroot-relocated-and-all-file-pins', 'source',
            dict(files=len(metadata['files']), version=metadata['sdk_version']))
        compiler = sdk / 'bin/clang'
        cxx = sdk / 'bin/clang++'
        common = ['--target=wasm32-wasip1', '--sysroot=' + str(relocated),
            '-resource-dir=' + str(relocated / metadata['clang_resource_directory']),
            '-Os', '-g0', '-Werror', '-Wl,--threads=1']
        c = root / 'stdlib.c'
        c.write_text('#include <stdio.h>\n#include <stdint.h>\n#include <stdlib.h>\n#include <string.h>\n'
            'int main(void){uint64_t *p=calloc(4,sizeof(*p));unsigned i;uint64_t total=0;'
            'if(!p)return 2;for(i=0;i<4;++i)p[i]=(uint64_t)i+1;'
            'for(i=0;i<4;++i)total+=p[i];free(p);printf("wasi-c=%u\\n",(unsigned)total);return total!=10;}\n')
        cpp = root / 'stdlib.cpp'
        cpp.write_text('#include <cstdio>\n#include <cstdint>\n#include <vector>\n#include <string>\n'
            '#include <algorithm>\nint main(){std::vector<std::string> v={"wasi","relocation"};'
            'std::sort(v.begin(),v.end());std::uint64_t n=v[0].size()+v[1].size();'
            'std::printf("wasi-cpp=%u\\n",unsigned(n));return n!=14;}\n')
        generated = []
        for name, command, source, flags in (('c-command', compiler, c, ['-std=c11']),
                ('cpp-command', cxx, cpp, ['-std=c++20', '-fno-exceptions', '-fno-rtti'])):
            artifact = root / (name + '.wasm')
            run(name, [command, *common, *flags, source, '-o', artifact])
            case(name + '-actual-clang-link', 'compile')
            run(name + '-validate', [args.wasm_tools, 'validate', artifact])
            case(name + '-full-wasm-tools-validation', 'wasm_validation')
            generated.append((name, artifact))
        reactor = root / 'reactor.c'
        reactor.write_text('int answer(int x){return x+42;}\n')
        artifact = root / 'reactor.wasm'
        run('reactor', [compiler, *common, '-std=c11', '-mexec-model=reactor', '-Wl,--export=answer', reactor, '-o', artifact])
        case('real-reactor-startup-object-link', 'compile')
        run('reactor-validate', [args.wasm_tools, 'validate', artifact])
        case('reactor-full-wasm-tools-validation', 'wasm_validation')
        wide = root / 'wide.c'
        wide.write_text('volatile unsigned __int128 x=((unsigned __int128)1<<100)+123;'
            'volatile unsigned __int128 y=97;int main(void){return (unsigned)(x/y)==0;}\n')
        artifact = root / 'wide.wasm'
        run('compiler-rt-wide-division', [compiler, *common, '-std=c11', wide, '-o', artifact])
        case('actual-wasm32-compiler-rt-wide-division-link', 'compile')
        run('wide-validate', [args.wasm_tools, 'validate', artifact])
        case('wide-full-wasm-tools-validation', 'wasm_validation')
        builtins = relocated / metadata['clang_builtins']
        hidden = builtins.with_suffix('.held')
        builtins.rename(hidden)
        try:
            run('missing-builtins-refused', [compiler, *common, '-std=c11', wide, '-o', root / 'must-not-link.wasm'], 'nonzero')
        finally:
            hidden.rename(builtins)
        case('missing-real-compiler-rt-refused', 'compile')
        broken = root / 'incomplete-sysroot'
        broken.mkdir()
        try:
            install_sysroot(Writer(root / 'must-not-publish'), broken, {'c'})
        except ValueError as error:
            if 'complete selected WASI target input missing' not in str(error):
                raise
        else:
            raise RuntimeError('include-only/incomplete target was accepted')
        if (root / 'must-not-publish').exists():
            raise RuntimeError('incomplete target published output')
        case('incomplete-target-refused-before-publication', 'source')
        # Optional Runtime execution is deliberately separate. Never infer it
        # from a successful Clang link or wasm-tools validation.
        if args.runtime is not None:
            for name, artifact in generated:
                run(name + '-guest', [args.runtime, '-Rint', '--wasip1-global-noinherit-system-environment', '--run', artifact])
                case(name + '-actual-selected-runtime-guest', 'actual_guest')
        else:
            report['actual_guest_status'] = 'NOT_RUN_NO_CURRENT_PINNED_RUNTIME_SELECTED'
        report.update(status='PASS', passed=len(report['cases']), denominator=11 if args.runtime is None else 13,
            selected_toolchain=str(sdk), sysroot=str(relocated), sysroot_metadata=metadata)
        if report['passed'] != report['denominator']:
            raise RuntimeError('incorrect acceptance denominator')
        save()
        print(json.dumps(dict(status='PASS', passed=report['passed'], denominator=report['denominator'],
            layers=report['layers'], result=str(result_path))), flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        save()
        raise


if __name__ == '__main__':
    main()
