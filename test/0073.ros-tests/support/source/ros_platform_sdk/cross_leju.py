#!/usr/bin/env python3
"""Execute original-field LEJU codec contracts on four x86/Arm user-QEMU targets.

This is data representation qualification, not a cross-host UWVM Runtime,
native ROS installation, controller, whole upstream suite or ARM PMU result.
The independent Linux owner bounds every serial compiler and QEMU descendant.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--vectors', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    kit = source / 'sdk/origin/robots/humanoids/leju'
    assert out.is_absolute() and out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    report = dict(status='RUNNING', commands=[], targets=[], default_authority=[],
                  scope='Complete C/C++ SDK and independent original-field C data contracts on user QEMU only; no Runtime/native ROS/controller/official/performance claim')
    def save():
        (out / 'cross.json').write_text(json.dumps(report, indent=2) + '\n')
    def pin(p):
        with p.open('rb') as f:
            digest = hashlib.file_digest(f, 'sha256').hexdigest()
        return dict(path=str(p), bytes=p.stat().st_size, sha256=digest)
    def run(label, argv):
        directory = out / 'commands' / label
        directory.mkdir()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            p = subprocess.run(list(map(str, argv)), cwd=source, stdout=stdout, stderr=stderr, timeout=180, check=False)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=p.returncode,
            stdout=pin(directory / 'stdout'), stderr=pin(directory / 'stderr')))
        save()
        assert p.returncode == 0, (label, p.returncode, (directory / 'stderr').read_text()[-2200:])
        return (directory / 'stdout').read_bytes()
    B = Path(inputs.get('cross_sdk_root','/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/extra-cross-sdk-inputs-r3'))
    A = Path(inputs.get('aarch64_sdk_root','/home/macromodel/.local/share/uwvm-ros-extension/20261013-aarch64-ros-sdk-r1/gateway/jazzy-rootfs'))
    qroot = Path('/home/macromodel/.local/opt/qemu-10.2.1/usr/bin')
    kernel = Path(inputs['kernel_headers'])
    # Restore the exact common dependency of Debian's cross-header link package.
    # These explicit kernel UAPI roots never select host libc or host asm data.
    assert kernel.is_absolute() and (kernel / 'usr/include/linux/types.h').is_file()
    report['kernel_uapi_root'] = str(kernel)
    report['kernel_generic_limits'] = pin(kernel / 'usr/include/linux/limits.h')
    matrix = [
        ('x86_64', 'x86_64-linux-gnu', None, None, 'qemu-x86_64'),
        ('i686', 'i686-linux-gnu', B / 'i386/sdk/usr/i686-linux-gnu', B / 'i386/sdk/usr/lib/gcc-cross/i686-linux-gnu/15', 'qemu-i386'),
        ('aarch64', 'aarch64-linux-gnu', A, A / 'usr/lib/gcc/aarch64-linux-gnu/13', 'qemu-aarch64'),
        ('armhf', 'arm-linux-gnueabihf', B / 'arm/sdk/usr/arm-linux-gnueabihf', B / 'arm/sdk/usr/lib/gcc-cross/arm-linux-gnueabihf/15', 'qemu-arm'),
    ]
    try:
        for name, target, sdk, gcc, qemu in matrix:
            emulator = qroot / qemu
            assert emulator.is_file()
            row = dict(architecture=name, target=target, emulator=pin(emulator), contracts=0,
                       independent_vector_programs=0, registry_boundary_programs=0, artifacts=[])
            report['targets'].append(row)
            flags = ['--target=' + target, '-fuse-ld=/usr/bin/ld.lld-21', '-static', '-Wl,--threads=1']
            if sdk:
                assert sdk.is_dir() and gcc.is_dir()
                flags += ['--sysroot=' + str(sdk), '--gcc-install-dir=' + str(gcc)]
                if name in ('i686', 'armhf'):
                    linux_arch = 'x86' if name == 'i686' else 'arm'
                    # C headers follow the explicit C++ roots: libstdc++'s
                    # include_next <stdlib.h> must reach this target libc.
                    flags += ['-idirafter', str(sdk / 'include'),
                              '-idirafter', str(kernel / 'usr/lib/linux/uapi' / linux_arch),
                              '-idirafter', str(kernel / 'usr/include'),
                              '-L' + str(sdk / 'lib')]
            for number, profile in enumerate(tuple(r['id'] for r in json.loads((kit/'profiles.json').read_bytes())['profiles']), 1):
                family = source / 'sdk/origin/custom-types' / ('leju-' + profile.replace('_', '-') + '-complete')
                include = ['-I' + str(source / 'sdk/origin/include'), '-I' + str(family), '-I' + str(kit / 'include')]
                for language, extension, compiler in [('c', 'c', '/usr/bin/clang-21'), ('cpp', 'cc', '/usr/bin/clang++-21')]:
                    label = name + '-' + profile + '-' + language
                    artifact = out / label
                    extra = ['-std=c11']
                    if language == 'cpp':
                        extra = ['-std=c++20', '-fno-exceptions', '-fno-rtti', '-nostdlib++']
                        if name in ('i686', 'armhf'):
                            headers = sdk / 'include/c++/15'
                            extra += ['-nostdinc++', '-isystem', str(headers), '-isystem', str(headers / target), '-isystem', str(headers / 'backward')]
                        elif name == 'aarch64':
                            extra += ['-nostdinc++', '-isystem', str(A / 'usr/include/c++/13'), '-isystem', str(A / 'usr/include/aarch64-linux-gnu/c++/13'), '-isystem', str(A / 'usr/include/c++/13/backward')]
                    run(label + '-build', [compiler, *flags, *extra, '-O1', *include,
                        '-DUWVM_LEJU_PROFILE=' + str(number), kit / ('tests/contract_' + profile + '.' + extension), '-o', artifact])
                    run(label + '-qemu', [emulator, artifact])
                    row['contracts'] += 1
                    row['artifacts'].append(pin(artifact))
                vectors = args.vectors / ('golden-vectors-' + profile)
                description = json.loads((vectors / 'vectors.json').read_bytes())
                for index, batch in enumerate(description['products']):
                    label = name + '-' + profile + '-golden-' + str(index)
                    artifact = out / label
                    run(label + '-build', ['/usr/bin/clang-21', *flags, '-std=c11', '-O1', *include, vectors / batch['file'], '-o', artifact])
                    run(label + '-qemu', [emulator, artifact])
                    row['independent_vector_programs'] += 1
                    row['artifacts'].append(pin(artifact))
                save()
            # Compile the real production count/cell/name validators on every
            # target, including actual 32-bit size_t overflow boundaries. The
            # fixture has empty registries and invents no native identities.
            # Unlike the C-backed SDK value contracts, production validators
            # call the actual standard string_view implementation. Retain its
            # real target runtime rather than omitting libstdc++ from the link.
            cpp_flags = ['-std=c++2c', *[f for f in extra[1:] if f != '-nostdlib++'], '-stdlib=libstdc++']
            library = run(name + '-production-cpp-runtime-path', ['/usr/bin/clang++-21',
                *flags, *cpp_flags, '-print-file-name=libstdc++.a']).decode().strip()
            library_path = Path(library)
            assert library_path.is_absolute() and library_path.is_file()
            row['production_cpp_runtime'] = pin(library_path)
            for selected, interfaces, cells in [('default', 128, 16384), ('explicit', 512, 65536)]:
                label = name + '-production-registry-' + selected
                artifact = out / label
                run(label + '-build', ['/usr/bin/clang++-21', *flags, *cpp_flags,
                    '-O1', '-fconstexpr-steps=8388608', '-I' + str(source / 'src'),
                    '-DUWVM_ROS_MAX_CUSTOM_INTERFACES=' + str(interfaces),
                    '-DUWVM_TEST_EXPECTED_REGISTRY_CAPACITY=' + str(interfaces),
                    '-DUWVM_ROS_MAX_CUSTOM_CELLS=' + str(cells),
                    '-DUWVM_TEST_EXPECTED_REGISTRY_CELLS=' + str(cells),
                    source / 'test/0073.ros-tests/support/source/ros_platform_sdk/registry_capacity.cc', '-o', artifact])
                run(label + '-qemu', [emulator, artifact])
                row['registry_boundary_programs'] += 1
                row['artifacts'].append(pin(artifact))
                save()
            assert pin(library_path) == row['production_cpp_runtime']
        report['status'] = 'LEJU_FOUR_ARCH_USER_QEMU_COMPLETE_DATA_CONTRACTS_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
