#!/usr/bin/env python3
"""Write an explicit metadata-only Native6 finalizer configuration.

This maker reads the exact terminal raw reports and their native configuration,
but launches no program and changes no input root. Invoke it only for Root's
selected live Linux lane. The finalizer independently reopens every selected
byte, probes the actual cgroup and inventories all complete compilation roots.
Output is a fresh private controls directory; the finalizer output must be a
different fresh path outside every selected input root.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys


INNER = ('eb5e185dbb38efebe90faabb2a7e21c10a33005ba31af9ca3a6a071e54a72487', 13359038)
OUTER = ('169b495fabd271f5c76ea88057659c6da6eb1f53551d0ee6dece5090dd880a7f', 109125)


def require(value, reason):
    if not value:
        raise ValueError(reason)


def pairs(items):
    result = {}
    for key, value in items:
        require(key not in result, 'duplicate metadata member')
        result[key] = value
    return result


def read(path, maximum):
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute metadata required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        require(stat.S_ISREG(first.st_mode) and 0 < first.st_size <= maximum, 'bounded ordinary metadata required')
        body = bytearray()
        while True:
            block = os.read(fd, min(65536, maximum + 1 - len(body)))
            if not block:
                break
            body.extend(block)
            require(len(body) <= maximum, 'metadata grew')
        after, named = os.fstat(fd), path.stat(follow_symlinks=False)
        require(len(body) == first.st_size and all(getattr(first, key) == getattr(after, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'metadata identity changed')
        pin = {'path': str(path), 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest(),
               'device': first.st_dev, 'inode': first.st_ino, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}
        return json.loads(body, object_pairs_hook=pairs,
            parse_constant=lambda value: require(False, 'nonfinite metadata')), pin
    finally:
        os.close(fd)


def write(path, value):
    raw = (json.dumps(value, sort_keys=True, indent=2) + '\n').encode()
    with path.open('xb') as stream:
        stream.write(raw)
        stream.flush()
        os.fsync(stream.fileno())
    return {'path': str(path), 'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inner', type=Path, required=True)
    parser.add_argument('--outer', type=Path, required=True)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--manifest', type=Path, required=True)
    parser.add_argument('--manifest-sha256', required=True)
    parser.add_argument('--controls', type=Path, required=True)
    parser.add_argument('--current-context', type=Path, required=True)
    parser.add_argument('--current-context-sha256', required=True)
    args = parser.parse_args()
    context, context_pin = read(args.current_context, 1024**2)
    require(context_pin['sha256'] == args.current_context_sha256 and type(context) is dict and
            set(context) == {'cgroup', 'restart_mappings', 'mount_namespace'} and sys.platform == 'linux' and
            Path('/proc/sys/kernel/random/boot_id').read_text().strip() == context['cgroup']['expected_boot_id'] and
            os.readlink('/proc/self/ns/mnt') == context['mount_namespace'] and
            context['cgroup']['memory_max'] == 68719476736 and context['cgroup']['memory_swap_max'] == 0,
            'explicit SHA-bound selected live Linux cgroup/device context required')
    inner, ip = read(args.inner, 64 * 1024**2)
    outer, op = read(args.outer, 64 * 1024**2)
    require((ip['sha256'], ip['bytes']) == INNER and (op['sha256'], op['bytes']) == OUTER and
            inner['status'] == outer['status'] == 'FAIL', 'exact genuine Native6 raw terminal reports required')
    original_config, cp = read(Path(inner['configuration']['path']), 4 * 1024**2)
    old_cp = inner['configuration']
    require(old_cp == outer['native_configuration'] and
            all(cp[key] == old_cp[key] for key in ('path', 'bytes', 'sha256', 'inode', 'mtime_ns', 'ctime_ns')) and
            (cp['device'] == old_cp['device'] or any(Path(cp['path']).is_relative_to(Path(row['root'])) and
                row['historical_device'] == old_cp['device'] and row['current_device'] == cp['device']
                for row in context['restart_mappings'])), 'original native configuration changed beyond explicit selected device history')
    source = args.source.resolve(strict=True)
    frozen, mp = read(args.manifest, 16 * 1024**2)
    require(mp['sha256'] == args.manifest_sha256 and type(frozen) is dict and frozen,
            'explicit complete new source manifest required')
    for relative in ('test/0073.ros-tests/support/source/ros_unitree/finalize_native_products.py', 'test/0073.ros-tests/support/source/ros_unitree/native_retained_products.py',
                     'test/0073.ros-tests/support/source/ros_unitree/typed_transport_inputs.py'):
        raw = (source / relative).read_bytes()
        require(0 < len(raw) <= 1024**2 and frozen.get(relative) ==
                {'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}, 'new finalizer absent from complete source freeze')
    roots = {row['root'] for row in inner['input_roots_after'] + outer['host_input_roots_after']}
    roots.update((str(Path(inner['source']).parent), str(args.inner.parent), str(args.outer.parent)))
    controls = args.controls.absolute()
    require(controls.resolve() == controls and not controls.exists() and not controls.is_symlink(), 'fresh physical controls directory required')
    roots.add(str(controls))
    require(len(roots) <= 64 and all(Path(root).is_absolute() and Path(root) != Path('/') for root in roots),
            'finite complete original compilation roots required')
    require(not any(controls.is_relative_to(Path(root)) or Path(root).is_relative_to(controls)
                    for root in roots if root != str(controls)), 'controls must not overlap preserved input roots')
    controls.mkdir(mode=0o700, parents=True, exist_ok=False)
    # Longest matching component-prefix AND old device select the history.
    # Multiple historical devices may map to the same current physical root.
    # The host '/' transition admits only exact finite metadata pins; '/' is
    # never an inventoried root or a Guest grant. No device is predicted here.
    for row in context['restart_mappings']:
        require(type(row) is dict and set(row) == {'root', 'historical_device', 'current_device'} and
                Path(row['root']).resolve(strict=True) == Path(row['root']) and
                Path(row['root']).stat().st_dev == row['current_device'], 'mapping must name its actual current physical root device')
    copied_context = write(controls / 'current-context.json', context)
    selection = {'format_version': 1, 'raw_inner': 'raw-inner', 'raw_outer': 'raw-outer',
        'current_context': 'current-context', 'restart_mappings': context['restart_mappings']}
    selected = write(controls / 'selection.json', selection)
    config = {'format_version': 1, 'cgroup': context['cgroup'], 'input_roots': sorted(roots),
        'inputs': {'raw-inner': {key: ip[key] for key in ('path', 'bytes', 'sha256')},
                   'raw-outer': {key: op[key] for key in ('path', 'bytes', 'sha256')},
                   'current-context': copied_context, 'selection': selected},
        'environment': {}, 'limits': {'jobs': 1, 'command_seconds': 3600, 'rss_bytes': 1024**3,
            'log_bytes': 16 * 1024**2, 'output_bytes': 512 * 1024**2},
        'upstreams': {}, 'simulation': None, 'benchmark': None, 'profiler': None}
    receipt = write(controls / 'config.json', config)
    print(json.dumps({'status': 'CONFIGURATION_WRITTEN_NOT_FINALIZER_EXECUTED', 'configuration': receipt,
        'selection': selected, 'raw_inner': ip, 'raw_outer': op, 'original_context': context_pin,
        'copied_current_context': copied_context, 'complete_input_roots': len(roots)}, sort_keys=True))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
