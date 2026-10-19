#!/usr/bin/env python3
"""Read-only source/recipe admission for the Guest client-library porting ledger.

This tool reads bounded JSON/source bytes. It imports no SDK module, starts no
compiler/process, evaluates no build command, and never interprets a marker as
Guest or Native success. Actual Wasm validation/import inspection and owned
Runtime/native acceptance remain separate operations. A report must be a fresh
file outside the source tree so this audit cannot modify SDK input artifacts.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat

LIMIT = 4 * 1024 * 1024


def read_file(path):
    """Admit one nonblocking/no-follow FD before reading any source bytes.

    resolve() only normalizes the caller's source role. It does not prove that
    opening its final name will be safe: a FIFO or a replacement symlink must
    be refused at the actual descriptor boundary, before a blocking read/open.
    Keep this SAME descriptor through regular-file/extent checks and all reads;
    final lstat binds the observed name itself, rather than following a newly
    substituted link to the original inode and silently accepting that change.
    """
    physical = path.resolve(strict=True)
    descriptor = os.open(physical, os.O_RDONLY | os.O_NONBLOCK |
                         os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode) or before.st_size > LIMIT:
            raise ValueError('bounded ordinary source file required')
        parts, count = [], 0
        while True:
            chunk = os.read(descriptor, 65536)
            if not chunk:
                break
            count += len(chunk)
            if count > LIMIT:
                raise ValueError('source grew beyond its read limit')
            parts.append(chunk)
        after = os.fstat(descriptor)
    finally:
        os.close(descriptor)
    identity = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns,
                          s.st_ctime_ns, s.st_mode, s.st_nlink)
    if identity(before) != identity(after) or identity(after) != identity(physical.lstat()):
        raise ValueError('source changed during read')
    raw = b''.join(parts)
    if len(raw) != before.st_size:
        raise ValueError('source extent changed')
    return raw, {'path': str(physical), 'bytes': len(raw),
                 'sha256': hashlib.sha256(raw).hexdigest(),
                 'mode': stat.S_IMODE(before.st_mode)}


def source_path(root, relative):
    """Resolve a declared source role, never an ambient plugin or tool path."""
    rel = Path(relative)
    if rel.is_absolute() or '..' in rel.parts or not rel.parts:
        raise ValueError('relative source role required')
    actual = (root / rel).resolve(strict=True)
    if not actual.is_relative_to(root):
        raise ValueError('source role escapes its admitted tree')
    return actual


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    root = args.source.resolve(strict=True)
    output_parent = args.report.parent.resolve(strict=True)
    output = output_parent / args.report.name
    if output.is_relative_to(root) or output.exists() or output.is_symlink():
        raise ValueError('fresh report outside the source tree required')
    ledger_raw, ledger_pin = read_file(source_path(root, 'sdk/compatibility/rcl/guest-porting-v1.json'))
    ledger = json.loads(ledger_raw)
    if ledger['format_version'] != 1 or ledger['kind'] != 'guest-source-client-library-porting-contract-v1':
        raise ValueError('unknown source contract')
    if ledger['default_authority'] != [] or ledger['guest_management_imports'] != []:
        raise ValueError('a build ledger cannot grant authority')
    if ledger['claim_full_compatibility'] is not False or ledger['guest_native_ROS_binary_ABI'] is not False:
        raise ValueError('source route is not a full compatibility or binary ABI certificate')
    if ledger['counts']['candidate_executions_by_this_cohort'] != 0:
        raise ValueError('this source cohort contains no execution certificate')
    observed = []
    for row in ledger['source_references']:
        raw, actual = read_file(source_path(root, row['relative']))
        if actual['bytes'] != row['bytes'] or actual['sha256'] != row['sha256'] or actual['mode'] != row['mode']:
            raise ValueError('source role changed: ' + row['relative'])
        observed.append({'relative': row['relative'], 'observed': actual})
    abi = json.loads(read_file(source_path(root, 'sdk/abi/abi-v1.json'))[0])
    expected_memory = {'memory_index': 'i32', 'offset_length_handle': 'i64',
                       'endianness': 'little', 'output_slot_bytes': 8,
                       'pointer_wrappers_memory': 0}
    for key, value in expected_memory.items():
        if abi['memory_contract'][key] != value:
            raise ValueError('ROS memory contract changed')
    for module, expected in [('uwvm_ros1_v1', 38), ('uwvm_ros2_v1', 43)]:
        functions = abi['modules'][module]['functions']
        if abi['modules'][module]['function_count'] != expected or len(functions) != expected:
            raise ValueError('business ABI inventory changed')
        inventory = [{'name': f['name'], 'params': [p['type'] for p in f['params']],
                      'results': f['results']} for f in functions]
        if ledger['business_import_inventory'][module] != inventory:
            raise ValueError('ledger/import signature mismatch')
    recipes = json.loads(read_file(source_path(root, ledger['build_recipes']))[0])
    if recipes['format_version'] != 1 or recipes['executed_by_this_cohort'] != 0:
        raise ValueError('unknown build recipe contract')
    # Missing producer/JAR interfaces stay explicit. A recipe with no known
    # actual producer API must not contain made-up executable commands.
    for recipe in recipes['recipes']:
        if recipe['status'] == 'REQUIRED_PRODUCER_INTERFACE' and recipe['argv_stages']:
            raise ValueError('unknown producer interface was replaced by guessed commands')
    report = {'format_version': 1, 'status': 'SOURCE_CONTRACT_MATCHED_NOT_BUILT_OR_EXECUTED',
              'source_ledger': ledger_pin, 'observed_source_roles': observed,
              'business_imports': 81, 'candidate_executions': 0,
              'compiler_or_generator_processes': 0, 'Guest_or_native_acceptance': False}
    # A failed source read/admission writes no partial successful report.
    with output.open('x') as stream:
        stream.write(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
