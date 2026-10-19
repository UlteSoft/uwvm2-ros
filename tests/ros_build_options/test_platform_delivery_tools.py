#!/usr/bin/env python3
"""Linux contracts for explicit selection, source union and observed ELF budgets.

Synthetic ELF/type data exercises readers and refusal behavior only. It is never
executed, presented as a native SDK, assigned a ROS support tier or granted an
endpoint. Actual project catalogs are merged separately and retain their scope.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys


def load(root, relative, name):
    spec = importlib.util.spec_from_file_location(name, root / relative)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    group = Path('/sys/fs/cgroup') / next(x[3:].lstrip('/') for x in
            Path('/proc/self/cgroup').read_text().splitlines() if x.startswith('0::'))
    if ((group / 'memory.max').read_text().strip() != str(64 * 1024**3) or
            (group / 'memory.swap.max').read_text().strip() != '0'):
        raise RuntimeError('hard 64 GiB/swap0 cgroup required')
    events = (group / 'memory.events').read_text()
    root, out = a.source.resolve(strict=True), a.out.absolute()
    out.mkdir(mode=0o700)
    config = load(root, 'sdk/tools/ros_build_config.py', 'selection')
    size = load(root, 'sdk/tools/ros_binary_size.py', 'sizes')
    merge = load(root, 'sdk/robots/vendors/merge_catalogs.py', 'union')
    cases = []
    def check(label, call):
        call()
        cases.append(dict(label=label, passed=True))
    def refuse(label, call):
        try:
            call()
        except (ValueError, OSError):
            cases.append(dict(label=label, refused=True, passed=True))
        else:
            raise AssertionError('expected refusal: ' + label)
    header = out / 'trusted-compiler-input.hpp'
    header.write_text('// configuration DATA only; not a native registry\n')
    common = dict(source=root, build=out / 'build', backend='ros2', arch='x86_64')
    def selection():
        result = config.configuration(**common, vendors=('fanuc', 'kuka'), registry=header)
        argv = result['configure_argv']
        for prefix, names in (('ros-vendor-', config.VENDORS), ('ros-platform-', config.PLATFORMS)):
            for name in names:
                yes = name in ('fanuc', 'kuka')
                assert '--' + prefix + name + '=' + ('y' if yes else 'n') in argv
                assert '--' + prefix + name + '-registry-header=' + (str(header) if yes else 'none') in argv
        assert '--import-ros1=n' in argv and '--import-ros2=y' in argv
        assert '--ros-platform-jetson=n' in argv and result['default_authority'] == []
        (out / 'actual-selected-command.json').write_text(json.dumps(result, indent=2) + '\n')
    check('A+B-explicitly-close-every-other-platform', selection)
    for backend in ('none', 'ros1', 'ros2', 'dual'):
        check('backend-' + backend, lambda b=backend: config.configuration(**dict(common, backend=b)))
    for label, kwargs in (
        ('missing-registry', dict(vendors=('fanuc',))),
        ('disabled-registry-injection', dict(registry=header)),
        ('duplicate-vendor', dict(vendors=('fanuc', 'fanuc'), registry=header)),
        ('unknown-vendor', dict(vendors=('unknown',), registry=header)),
        ('jetson-x86', dict(jetson=True)),
        ('target-family-conflict', dict(target='aarch64-unknown-linux-gnu')),
        ('missing-hardfloat-triple', dict(arch='armhf')),
        ('hardfloat-with-soft-triple', dict(arch='armhf', target='armv7-unknown-linux-gnueabi')),
        ('big-endian-with-little-triple', dict(arch='aarch64be', target='aarch64-unknown-linux-gnu'))):
        refuse(label, lambda k=kwargs: config.configuration(**dict(common, **k)))
    for arch, target, normalized in (
        ('i686', 'i686-unknown-linux-gnu', 'i386'),
        ('armhf', 'armv7-unknown-linux-gnueabihf', 'arm'),
        ('armeb', 'armeb-unknown-linux-gnueabi', 'arm'),
        ('armebhf', 'armeb-unknown-linux-gnueabihf', 'arm'),
        ('aarch64be', 'aarch64_be-unknown-linux-gnu', 'arm64')):
        result = config.configuration(**dict(common, arch=arch), target=target)
        assert result['configure_argv'][result['configure_argv'].index('-a') + 1] == normalized
        cases.append(dict(label='target-normalization-' + arch, passed=True))
    def elf(bits, order):
        data = bytearray(512)
        data[:7] = b'\x7fELF' + bytes((1 if bits == 32 else 2, 1 if order == '<' else 2, 1))
        eh, pe, se = ((52, 32, 40) if bits == 32 else (64, 56, 64))
        struct.pack_into(order + ('HHIIIIIHHHHHH' if bits == 32 else 'HHIQQQIHHHHHH'), data, 16,
                         2, 40 if bits == 32 else 183, 1, 0x101c0, 64, 128, 0, eh, pe, 1, se, 3, 1)
        if bits == 32:
            struct.pack_into(order + 'IIIIIIII', data, 64, 1, 0, 0x10000, 0x10000, 480, 512, 5, 4096)
        else:
            struct.pack_into(order + 'IIQQQQQQ', data, 64, 1, 5, 0, 0x10000, 0x10000, 480, 512, 4096)
        names = b'\0.shstrtab\0.text\0'
        fmt = order + ('IIIIIIIIII' if bits == 32 else 'IIQQQQIIQQ')
        struct.pack_into(fmt, data, 128 + se, 1, 3, 0, 0, 384, len(names), 0, 0, 1, 0)
        struct.pack_into(fmt, data, 128 + se * 2, 11, 1, 6, 0x101c0, 448, 32, 0, 0, 16, 0)
        data[384:384 + len(names)] = names
        return data
    for bits in (32, 64):
        for order in ('<', '>'):
            path = out / ('metadata-%d-%s.elf' % (bits, 'le' if order == '<' else 'be'))
            path.write_bytes(elf(bits, order))
            result = size.measure(path)
            assert (result['bytes'], result['load_file_bytes'], result['load_memory_bytes'],
                    result['allocated_section_file_bytes']) == (512, 480, 512, 32)
            assert result['elf_class'] == bits
            cases.append(dict(label=path.name, passed=True))
    path = out / 'metadata-64-le.elf'
    for budget, expected in ((512, 0), (511, 1)):
        result = subprocess.run([sys.executable, '-I', '-S', '-B', str(root / 'sdk/tools/ros_binary_size.py'),
                                 str(path), '--max-file-bytes', str(budget)], capture_output=True, timeout=10)
        assert result.returncode == expected and json.loads(result.stdout)['passed'] == (expected == 0)
        cases.append(dict(label='actual-budget-cli-' + str(budget), passed=True))
    link = out / 'symlink.elf'; link.symlink_to(path)
    refuse('ELF-leaf-symlink', lambda: size.measure(link))
    broken = out / 'broken.elf'; broken.write_bytes(elf(64, '<')[:80])
    refuse('truncated-ELF-tables', lambda: size.measure(broken))
    def catalog(parent, typename='demo/msg/Value', body='uint32 value\n', bound=4):
        parent.mkdir()
        (parent / 'Value.msg').write_text(body)
        file = parent / 'catalog.json'
        file.write_text(json.dumps(dict(format=merge.PROFILE, types=[dict(
            ros_version=2, type=typename, file='Value.msg', registered=True, bounds={'value': bound})])))
        return file
    a1, a2 = catalog(out / 'a'), catalog(out / 'b')
    receipt = merge.merge([a1, a2], out / 'union')
    assert receipt['unique_definitions'] == 1 and receipt['input_definitions'] == 2
    cases.append(dict(label='exact-whole-source-deduplication', passed=True))
    different = catalog(out / 'different', body='uint64 value\n')
    refuse('different-original-definition', lambda: merge.merge([a1, different], out / 'bad-union'))
    different_bound = catalog(out / 'bound', bound=5)
    refuse('different-canonical-capacity', lambda: merge.merge([a1, different_bound], out / 'bad-bound'))
    refuse('existing-union', lambda: merge.merge([a1, a2], out / 'union'))
    refuse('output-inside-input', lambda: merge.merge([a1, a2], a1.parent / 'output'))
    actual = merge.merge([
        root / 'sdk/robots/industrial/fanuc/catalogs/ros2/catalog.preparation.json',
        root / 'sdk/robots/industrial/kuka/catalogs/ros2/catalog.preparation.json'], out / 'fanuc-kuka-source')
    cases.append(dict(label='actual-complete-FANUC-KUKA-source-union', passed=True,
                      input_definitions=actual['input_definitions'], unique_definitions=actual['unique_definitions']))
    if (group / 'memory.events').read_text() != events:
        raise RuntimeError('cgroup memory events changed')
    result = dict(passed=True, cases=cases, executed_cases=len(cases),
                  cgroup=str(group), events_before=events, events_after=(group / 'memory.events').read_text(),
                  actual_Guest_or_native_ROS_runs=0, default_authority=[])
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(dict(passed=True, cases=len(cases))))


if __name__ == '__main__':
    main()
