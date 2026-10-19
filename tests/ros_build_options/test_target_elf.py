#!/usr/bin/env python3
"""Check actual distribution-lane ELF inspection with bounded metadata files.

Synthetic ELF headers below exercise metadata admission only; no fixture is
executed, emulated, presented as a real SDK, or assigned a ROS support tier.
"""
import argparse
import importlib
import json
from pathlib import Path
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    membership = Path('/proc/self/cgroup').read_text().strip().split('0::', 1)[1]
    group = Path('/sys/fs/cgroup') / membership.lstrip('/')
    if ((group / 'memory.max').read_text().strip() != '68719476736' or
            (group / 'memory.swap.max').read_text().strip() != '0'):
        raise RuntimeError('actual64GiB/swap0 group required')
    root = args.source_root.resolve(strict=True)
    sys.path.insert(0, str(root / 'tools/ros_distro'))
    actual = importlib.import_module('build')
    profiles = importlib.import_module('profile')
    out = args.out.absolute()
    out.mkdir(mode=0o700)
    cases = []
    def check(label, architecture, *, flags=0, physical=None, accepted=True, float_abi=None, truncate=None):
        machine, bits, endian = profiles.ARCHES[physical or architecture]
        header = bytearray(64 if bits == 64 else 52)
        header[:4] = b'\x7fELF'
        header[4:7] = bytes((2 if bits == 64 else 1, 1 if endian == 'little' else 2, 1))
        header[16:18] = (2).to_bytes(2, endian)
        header[18:20] = machine.to_bytes(2, endian)
        header[20:24] = (1).to_bytes(4, endian)
        if bits == 32: header[36:40] = flags.to_bytes(4, endian)
        path = out / (label + '.metadata')
        path.write_bytes(header if truncate is None else header[:truncate])
        target_machine, target_bits, target_endian = profiles.ARCHES[architecture]
        target = dict(architecture=architecture, elf_machine=target_machine,
                      bits=target_bits, endian=target_endian)
        try:
            result = actual.elf(path, target)
        except ValueError as error:
            if accepted: raise
            cases.append(dict(label=label, accepted=False, reason=str(error)))
        else:
            if not accepted or float_abi is not None and result['arm_float_abi'] != float_abi:
                raise AssertionError('wrong actual ELF admission: ' + label)
            cases.append(dict(label=label, accepted=True, observation=result))
    for name in ('x86_64', 'i386', 'i686', 'aarch64', 'aarch64_be', 'aarch64be', 'arm', 'armeb'):
        check(name, name)
    check('armhf-hard', 'armhf', flags=0x05000400, float_abi='hard')
    check('armel-explicit-soft', 'armel', flags=0x05000200, float_abi='soft')
    check('armel-implied-base', 'armel', flags=0x05000000, float_abi='soft')
    check('armebhf-hard', 'armebhf', flags=0x05000400, float_abi='hard')
    check('armhf-refuses-base', 'armhf', flags=0x05000000, accepted=False)
    check('armel-refuses-hard', 'armel', flags=0x05000400, accepted=False)
    check('armhf-refuses-legacy', 'armhf', flags=0x04000400, accepted=False)
    check('arm-refuses-contradictory', 'arm', flags=0x05000600, accepted=False)
    check('armeb-refuses-little', 'armeb', physical='arm', accepted=False)
    check('aarch64be-refuses-little', 'aarch64be', physical='aarch64', accepted=False)
    check('i686-refuses-x64', 'i686', physical='x86_64', accepted=False)
    check('truncated-ELF64', 'aarch64', truncate=52, accepted=False)
    report = dict(passed=True, actual_metadata_cases=len(cases), cases=cases,
        cgroup=str(group), compiler_Guest_Native_ROS_QEMU_runs=0,
        qualification='Actual production ELF metadata reader only. No executable, Native SDK, QEMU, hardware or ROS compatibility result.')
    (out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(passed=True, actual_metadata_cases=len(cases))))


if __name__ == '__main__':
    main()
