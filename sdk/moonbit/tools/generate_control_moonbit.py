#!/usr/bin/env python3
"""Explicit full32/seven-Service MoonBit source adapter, no Native authority.

The Linux keeper must admit this entry, interpreter/startup/stdlib and complete
source/toolchain inputs before their first instruction. The captures below bind
the actual helper bytes executed here, not an arbitrary ambient Python module
or PYC. Generated source is not a native compatibility or runtime test result.
"""
import argparse
import dataclasses
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types

PROFILE = 'uwvm-ros-original-node-control-c-cpp-v1'
LIMIT = 8 * 1024 * 1024
CANONICAL = {'bounded_records.h', 'bounded_records.hpp', 'bounded_interfaces.h',
             'bounded_interfaces.hpp', 'control_wire.hpp', 'control_manifest.hpp', 'control-schema.json'}
ROLES = ('parent', 'bounded', 'emitter', 'services', 'runtime', 'operator', 'catalog', 'wire')


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def capture(path, maximum=LIMIT):
    path = Path(path)
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise ValueError('explicit resolved source path required')
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= maximum:
            raise ValueError('bounded ordinary file required')
        blocks, size = [], 0
        while True:
            block = os.read(fd, min(65536, maximum - size + 1))
            if not block:
                break
            size += len(block)
            if size > maximum:
                raise ValueError('file grew beyond capture bound')
            blocks.append(block)
        fields = ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_nlink')
        end, named = os.fstat(fd), os.lstat(path)
        if size != first.st_size or any(getattr(first, key) != getattr(end, key) or
                getattr(first, key) != getattr(named, key) for key in fields):
            raise ValueError('captured source changed')
        return b''.join(blocks)
    finally:
        os.close(fd)


def strict_json(raw):
    def pairs(rows):
        result = {}
        for key, value in rows:
            if key in result:
                raise ValueError('duplicate JSON key')
            result[key] = value
        return result
    def invalid_constant(_value):
        raise ValueError('finite JSON data required')
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=invalid_constant)


def generate(root, catalog, canonical, output, expected):
    if any(not path.is_absolute() or path.resolve(strict=True) != path for path in (root, catalog, canonical)):
        raise ValueError('resolved source tree/catalog/canonical bundle required')
    if not output.is_absolute() or output.resolve() != output or output.exists() or output.is_symlink():
        raise ValueError('fresh resolved output required')
    if output.is_relative_to(root) or output.is_relative_to(canonical) or canonical.is_relative_to(output):
        raise ValueError('separate fresh output revision required')
    sources = {'parent': root / 'sdk/tools/custom_types.py',
               'bounded': root / 'sdk/custom-types/bounded_profile.py',
               'emitter': root / 'sdk/custom-types/moonbit_profile.py',
               'services': Path(__file__).resolve().with_name('control_services_moonbit.py'),
               'runtime': root / 'sdk/custom-types/moonbit_runtime.mbt.in',
               'operator': Path(__file__).resolve(),
               'catalog': catalog,
               'wire': root / 'sdk/moonbit/wire/wire.mbt'}
    raw = {role: capture(path) for role, path in sources.items()}
    if set(expected) != set(ROLES) or any(digest(raw[role]) != expected[role] for role in ROLES):
        raise ValueError('all exact captured helper/catalog/foundation source SHA values required')
    data = {str(path): raw[role] for role, path in sources.items()}
    declared = strict_json(raw['catalog'])
    if type(declared) is not dict or set(declared) != {'format', 'types'} or declared['format'] != PROFILE:
        raise ValueError('actual complete original control catalog required')
    for row in declared['types']:
        path = (catalog.parent / row['file']).resolve(strict=True)
        if not path.is_relative_to(catalog.parent):
            raise ValueError('definition escaped original source root')
        data[str(path)] = capture(path)
    modules = {}
    try:
        for role in ('parent', 'bounded', 'emitter', 'services'):
            name = '_uwvm_control_moonbit_' + role
            if name in sys.modules:
                raise ValueError('fresh private module name required')
            module = types.ModuleType(name)
            module.__file__, module.__package__, module.__cached__ = str(sources[role]), '', None
            module.__spec__, module.__loader__ = None, None
            sys.modules[name] = module
            modules[role] = module
            exec(compile(raw[role], str(sources[role]), 'exec', dont_inherit=True), module.__dict__)
        parent, bounded, emitter = (modules[key] for key in ('parent', 'bounded', 'emitter'))
        def bounded_read(path, maximum):
            key = str(Path(path).resolve(strict=True))
            if key not in data or len(data[key]) > maximum:
                raise ValueError('actual parser requested data outside captured closure')
            return data[key]
        parent.bounded_read = bounded_read
        records, ordered, interfaces, pins = bounded.load_catalog(parent, catalog, False)
        if (len(records) != 32 or len(ordered) != 32 or any(record.profile != PROFILE for record in ordered) or
                {item.name for item in interfaces if item.registered} != bounded.control_registered_names()):
            raise ValueError('complete original32 IR and actual registered roots required')
        # The real locked parser itself verifies the complete fixed original
        # source family, source extents, fields/defaults/constants and identities.
        emitter.checked_ir(parent, bounded, records, ordered, interfaces)
        canonical_proof = capture(canonical / 'generation.json')
        proof = strict_json(canonical_proof)
        if (proof.get('format') != PROFILE or proof.get('source_inputs') != pins or
                proof.get('implemented_record_outputs') != ['c', 'cpp'] or proof.get('native_registry_emitted') is not False or
                proof.get('generator_sha256') != expected['parent'] or proof.get('bounded_generator_sha256') != expected['bounded'] or
                set(proof.get('outputs', {})) != CANONICAL):
            raise ValueError('same-source authoritative C/C++ complete proof required')
        canonical_bytes = {canonical / name: capture(canonical / name) for name in CANONICAL}
        for path, body in canonical_bytes.items():
            if proof['outputs'][path.name] != {'bytes': len(body), 'sha256': digest(body)}:
                raise ValueError('actual canonical product differs')
        products = emitter.emit(parent, bounded, records, ordered, interfaces, raw['runtime'].decode('utf-8'))
        if set(products) != {'moonbit/bounded_records.mbt', 'moonbit/moon.pkg'}:
            raise ValueError('closed full-record emitter outputs required')
        services, service_facts = modules['services'].emit(parent, bounded, ordered, interfaces)
        products['moonbit/control_services.mbt'] = services
        products['moonbit/default_contracts.mbt'] = modules['services'].emit_default_contracts(parent, bounded, ordered)
        products['moonbit/moon.pkg'] = 'import { "uwvm/ros/wire", "uwvm/ros/owned_service", "moonbitlang/core/encoding/utf8", "moonbitlang/core/float" }\n'
        report = {'format': PROFILE, 'status': 'MOONBIT_FULL32_SEVEN_SOURCE_ONLY_NOT_COMPILED_OR_EXECUTED',
                  'generators_and_foundations': {role: {'path': str(sources[role]), 'bytes': len(body), 'sha256': digest(body)} for role, body in raw.items()},
                  'source_inputs': pins, 'complete_records': [{**dataclasses.asdict(record), 'symbol': record.symbol,
                      'moonbit_member_spellings': {member.name: emitter.field_name(record, member) for member in record.members}} for record in ordered],
                  'complete_services': service_facts,
                  'canonical_generation': {'bytes': len(canonical_proof), 'sha256': digest(canonical_proof)},
                  'canonical_products': proof['outputs'],
                  'native_registry_emitted': False, 'native_qualification': 'NOT_PERFORMED', 'implicit_permissions': [],
                  'compiler_execution': 'NOT_RUN', 'guest_execution': 'NOT_RUN',
                  'outputs': {name: {'bytes': len(body.encode('utf-8')), 'sha256': digest(body.encode('utf-8'))} for name, body in products.items()}}
        for path, body in {**{Path(name): body for name, body in data.items()}, **canonical_bytes,
                           canonical / 'generation.json': canonical_proof}.items():
            if capture(path) != body:
                raise ValueError('admitted source/canonical data changed before write')
        if any(sys.modules.get('_uwvm_control_moonbit_' + role) is not module for role, module in modules.items()):
            raise ValueError('captured helper replaced its module binding')
        if sum(len(body.encode('utf-8')) for body in products.values()) > 32 * 1024 * 1024:
            raise ValueError('bounded generated source extent exceeded')
        products['generation.control-moonbit.json'] = json.dumps(report, indent=2) + '\n'
        output.mkdir(parents=True, exist_ok=False)
        for name, body in products.items():
            path = output / name
            path.parent.mkdir(parents=True, exist_ok=True)
            with path.open('x', encoding='utf-8') as stream:
                stream.write(body)
        return report
    finally:
        for role, module in modules.items():
            name = '_uwvm_control_moonbit_' + role
            if sys.modules.get(name) is module:
                del sys.modules[name]


def main():
    if sys.platform != 'linux' or not sys.flags.isolated or not sys.flags.no_site or not sys.flags.safe_path:
        raise ValueError('externally admitted Linux Python -I -S entry required')
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source-root', 'catalog', 'canonical', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    for role in ROLES:
        parser.add_argument('--' + role + '-sha256', required=True)
    parser.add_argument('--rmw-service-enabled', type=int, choices=(0, 1), default=0)
    parser.add_argument('--backend', type=int, choices=(2,), required=True)
    args = parser.parse_args()
    if args.rmw_service_enabled != 1:
        raise ValueError('owned control package generation is an explicit opt-in; default is off')
    report = generate(args.source_root, args.catalog, args.canonical, args.out,
                      {role: getattr(args, role + '_sha256') for role in ROLES})
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
