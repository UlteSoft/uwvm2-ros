#!/usr/bin/env python3
"""Print an explicit, reproducible Linux ROS xmake configuration; execute nothing.

Every vendor/data-platform selection and both ROS backends are written into the
command. A fresh xmake configuration therefore cannot inherit an old selection.
The supplied registry is trusted compiled C++ source, not a permission file.
This tool does not generate, merge, authenticate or filter that registry.
"""
import argparse
import json
from pathlib import Path
import shlex


VENDORS = tuple('unitree agilex elephant ufactory dobot flexiv leju deep-robotics '
                'realman clearpath robotis universal-robots franka pal doosan '
                'trossen agibot ubtech fourier fanuc kuka yaskawa boston-dynamics '
                'kinova abb booster limx hello-robot'.split())
PLATFORMS = ('isaac-ros', 'isaac-sim', 'groot')


def configuration(*, source, build, backend, arch, vendors=(), platforms=(),
                  registry=None, jetson=False, mode='minsizerel', lto=False,
                  xmake='xmake', target=None, sysroot=None):
    """Return argv DATA, refusing ambiguous selection before producing a command.

    One coherent Native-bound bundle must already contain only the requested
    schemas. Naming two vendors cannot make independent generated domains safe
    to concatenate. Each positive deployment still needs separate Guest grants.
    """
    for selected, allowed in ((vendors, VENDORS), (platforms, PLATFORMS)):
        if len(set(selected)) != len(selected) or not set(selected) <= set(allowed):
            raise ValueError('unknown or repeated vendor/platform selection')
    if backend not in ('none', 'ros1', 'ros2', 'dual'):
        raise ValueError('unknown backend')
    if mode not in ('release', 'minsizerel', 'releasedbg', 'debug'):
        raise ValueError('unknown optimization mode')
    architectures = dict(x86_64='x86_64', i386='i386', i686='i386', arm='arm',
                         armel='arm', armv7='arm', armhf='arm', armeb='arm', armebhf='arm',
                         arm64='arm64', aarch64='arm64', aarch64_be='arm64', aarch64be='arm64')
    if arch not in architectures:
        raise ValueError('use an explicit reviewed x86/ARM target name')
    if arch in ('armhf', 'armeb', 'armebhf', 'aarch64_be', 'aarch64be') and target is None:
        raise ValueError('float ABI/endian selections require the exact compiler --target')
    if target is not None:
        import re
        if not re.fullmatch(r'[A-Za-z0-9_]+(?:-[A-Za-z0-9_]+){2,4}', target):
            raise ValueError('explicit compiler target triple required')
        little_arm = target.startswith(('arm-', 'armv7-', 'armv7a-'))
        big_arm = target.startswith(('armeb-', 'armv7eb-'))
        family = architectures[arch]
        if ((family == 'x86_64' and not target.startswith('x86_64-')) or
                (family == 'i386' and not target.startswith(('i386-', 'i486-', 'i586-', 'i686-'))) or
                (family == 'arm' and not (little_arm or big_arm)) or
                (family == 'arm64' and not target.startswith(('aarch64-', 'aarch64_be-'))) or
                (arch in ('arm64', 'aarch64') and not target.startswith('aarch64-')) or
                (arch == 'armhf' and not (little_arm and target.endswith('eabihf'))) or
                (arch == 'armebhf' and not (big_arm and target.endswith('eabihf'))) or
                (arch == 'armeb' and not (big_arm and target.endswith('eabi'))) or
                (arch in ('aarch64_be', 'aarch64be') and not target.startswith('aarch64_be-'))):
            raise ValueError('compiler target contradicts the requested float ABI/endian')
    if (vendors or platforms) and backend == 'none':
        raise ValueError('selected schemas require an explicit ROS backend')
    if (platforms or jetson) and backend not in ('ros2', 'dual'):
        raise ValueError('NVIDIA layers require ROS2')
    if jetson and arch not in ('arm64', 'aarch64'):
        raise ValueError('Jetson selection requires Linux little-endian ARM64')
    selected = bool(vendors or platforms)
    if selected != (registry is not None):
        raise ValueError('supply exactly one reviewed registry for selected schemas')
    if registry is not None:
        registry = Path(registry).resolve(strict=True)
        if not registry.is_file() or any(c in str(registry) for c in ';"\r\n'):
            raise ValueError('registry must be an existing safe ordinary source path')
    source, build = Path(source).resolve(strict=True), Path(build).absolute()
    if not (source / 'xmake.lua').is_file():
        raise ValueError('source must contain the project xmake.lua')
    if build == source or source in build.parents:
        raise ValueError('use a build directory outside the source checkout')
    argv = [str(xmake), 'f', '-c', '-P', str(source), '-p', 'linux', '-a', architectures[arch],
            '-m', mode, '-o', str(build), '--use-cxx-module=n',
            '--use-llvm-compiler=y', '--execution-int=uwvm-int',
            '--execution-jit=none', '--enable-lto=' + ('y' if lto else 'n'),
            '--cxflags=-ffunction-sections -fdata-sections', '--ldflags=-Wl,--gc-sections',
            '--strip=' + ('none' if mode in ('debug', 'releasedbg') else 'ident'),
            '--import-ros1=' + ('y' if backend in ('ros1', 'dual') else 'n'),
            '--import-ros2=' + ('y' if backend in ('ros2', 'dual') else 'n'),
            '--ros-custom-registry-header=none',
            '--ros-original-control-manifest-header=none']
    if target is not None:
        argv += ['--target=' + target, '--llvm-target=' + target]
    if sysroot is not None:
        sysroot = Path(sysroot).resolve(strict=True)
        if not sysroot.is_dir():
            raise ValueError('sysroot must be an existing reviewed directory')
        argv += ['--sysroot=' + str(sysroot)]
    for prefix, allowed, choices in (('ros-vendor-', VENDORS, vendors),
                                     ('ros-platform-', PLATFORMS, platforms)):
        for name in allowed:
            enabled = name in choices
            argv += ['--' + prefix + name + '=' + ('y' if enabled else 'n'),
                     '--' + prefix + name + '-registry-header=' +
                     (str(registry) if enabled else 'none')]
    argv += ['--ros-platform-jetson=' + ('y' if jetson else 'n')]
    return dict(kind='explicit-ros-xmake-configuration-not-execution',
                configure_argv=argv,
                build_argv=[str(xmake), 'b', '-P', str(source), '-j1', 'uwvm'],
                selected_vendors=list(vendors), selected_data_platforms=list(platforms),
                backend=backend, architecture=arch, default_authority=[],
                registry_scope='Operator-reviewed coherent schema source; no automatic merge or filtering.')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[2])
    p.add_argument('--build', type=Path, required=True)
    p.add_argument('--backend', choices=('none', 'ros1', 'ros2', 'dual'), default='ros2')
    p.add_argument('--arch', default='x86_64')
    p.add_argument('--target', help='Exact reviewed compiler target triple for cross builds')
    p.add_argument('--sysroot', type=Path, help='Matching target libc/C++ development root')
    p.add_argument('--vendor', action='append', default=[], choices=VENDORS)
    p.add_argument('--platform', action='append', default=[], choices=PLATFORMS)
    p.add_argument('--registry', type=Path)
    p.add_argument('--jetson', action='store_true')
    p.add_argument('--mode', default='minsizerel', choices=('release', 'minsizerel', 'releasedbg', 'debug'))
    p.add_argument('--lto', action='store_true')
    p.add_argument('--xmake', default='xmake')
    p.add_argument('--format', choices=('shell', 'json'), default='shell')
    a = p.parse_args()
    try:
        result = configuration(source=a.source, build=a.build, backend=a.backend,
                               arch=a.arch, vendors=a.vendor, platforms=a.platform,
                               registry=a.registry, jetson=a.jetson, mode=a.mode,
                               lto=a.lto, xmake=a.xmake, target=a.target, sysroot=a.sysroot)
    except (ValueError, OSError) as error:
        p.error(str(error))
    if a.format == 'json':
        print(json.dumps(result, indent=2))
    else:
        print('#!/bin/sh\nset -eu')
        print(shlex.join(result['configure_argv']))
        print(shlex.join(result['build_argv']))


if __name__ == '__main__':
    main()
