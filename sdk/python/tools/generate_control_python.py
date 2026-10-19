#!/usr/bin/env python3
"""Explicit complete32/7-root Python source adapter; never a Native authority.

The keeper must admit this entry/interpreter/stdlib before its first instruction.
This program captures exact SHA-paired parser/emitter bytes, never uses pyc or
ordinary local module imports, and gives the real parser captured catalog data.
Only future admitted Linux execution may run it; no outputs exist at authoring.
"""
from pathlib import Path
import argparse
import dataclasses
import hashlib
import json
import os
import stat
import sys
import types

PROFILE = 'uwvm-ros-original-node-control-c-cpp-v1'
EXPECTED_RECORDS=set(['builtin_interfaces/msg/Time', 'rcl_interfaces/msg/FloatingPointRange', 'rcl_interfaces/msg/IntegerRange', 'rcl_interfaces/msg/ListParametersResult', 'rcl_interfaces/msg/Log', 'rcl_interfaces/msg/Parameter', 'rcl_interfaces/msg/ParameterDescriptor', 'rcl_interfaces/msg/ParameterEvent', 'rcl_interfaces/msg/ParameterType', 'rcl_interfaces/msg/ParameterValue', 'rcl_interfaces/msg/SetParametersResult', 'rcl_interfaces/srv/DescribeParameters_Request', 'rcl_interfaces/srv/DescribeParameters_Response', 'rcl_interfaces/srv/GetParameterTypes_Request', 'rcl_interfaces/srv/GetParameterTypes_Response', 'rcl_interfaces/srv/GetParameters_Request', 'rcl_interfaces/srv/GetParameters_Response', 'rcl_interfaces/srv/ListParameters_Request', 'rcl_interfaces/srv/ListParameters_Response', 'rcl_interfaces/srv/SetParameters_Request', 'rcl_interfaces/srv/SetParameters_Response', 'rcl_interfaces/srv/SetParametersAtomically_Request', 'rcl_interfaces/srv/SetParametersAtomically_Response', 'service_msgs/msg/ServiceEventInfo', 'type_description_interfaces/msg/Field', 'type_description_interfaces/msg/FieldType', 'type_description_interfaces/msg/IndividualTypeDescription', 'type_description_interfaces/msg/KeyValue', 'type_description_interfaces/msg/TypeDescription', 'type_description_interfaces/msg/TypeSource', 'type_description_interfaces/srv/GetTypeDescription_Request', 'type_description_interfaces/srv/GetTypeDescription_Response'])
LIMIT = 8 * 1024 * 1024
CANONICAL = {'bounded_records.h', 'bounded_records.hpp', 'bounded_interfaces.h',
             'bounded_interfaces.hpp', 'control_wire.hpp', 'control_manifest.hpp', 'control-schema.json'}


def capture(path, limit=LIMIT):
    path = Path(path)
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise ValueError('explicit resolved source required')
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= limit:
            raise ValueError('bounded ordinary source required')
        parts, size = [], 0
        while True:
            data = os.read(fd, min(65536, limit - size + 1))
            if not data:
                break
            size += len(data)
            if size > limit:
                raise ValueError('source extent exceeded')
            parts.append(data)
        fields = ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_nlink')
        last, named = os.fstat(fd), os.lstat(path)
        if size != first.st_size or any(getattr(first, k) != getattr(last, k) or getattr(first, k) != getattr(named, k) for k in fields):
            raise ValueError('source changed while captured')
        return b''.join(parts)
    finally:
        os.close(fd)


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def strict(raw):
    def pairs(rows):
        out = {}
        for key, value in rows:
            if key in out:
                raise ValueError('duplicate JSON key')
            out[key] = value
        return out
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: (_ for _ in ()).throw(ValueError('nonfinite JSON')))


def run(root, catalog, canonical, output, expected):
    if any(not p.is_absolute() or p.resolve(strict=True) != p for p in (root, catalog, canonical)) or not output.is_absolute() or output.resolve() != output or output.exists():
        raise ValueError('resolved sources and a fresh absolute output required')
    if output.is_relative_to(root) or output.is_relative_to(canonical) or canonical.is_relative_to(output):
        raise ValueError('separate fresh generated source revision required')
    sources = {'parent': root / 'sdk/tools/custom_types.py',
               'bounded': root / 'sdk/custom-types/bounded_profile.py',
               'emitter': Path(__file__).resolve().with_name('control_python_emitter.py'),
               'operator': Path(__file__).resolve()}
    raw = {role: capture(path) for role, path in sources.items()}
    if set(expected) != set(sources) or any(type(expected[role]) is not str or sha(body) != expected[role] for role, body in raw.items()):
        raise ValueError('all four actually captured generator SHA values required')
    admitted = {str(path): raw[role] for role, path in sources.items()}
    catalog_raw = capture(catalog)
    declared = strict(catalog_raw)
    if type(declared) is not dict or set(declared) != {'format', 'types'} or declared['format'] != PROFILE:
        raise ValueError('exact original control catalog required')
    admitted[str(catalog)] = catalog_raw
    for row in declared['types']:
        path = (catalog.parent / row['file']).resolve(strict=True)
        if not path.is_relative_to(catalog.parent):
            raise ValueError('definition escaped admitted source root')
        admitted[str(path)] = capture(path)
    names = ('_roswasm_control_python_parent', '_roswasm_control_python_bounded', '_roswasm_control_python_emitter')
    modules = {}
    try:
        for role, name in zip(('parent', 'bounded', 'emitter'), names):
            if name in sys.modules:
                raise ValueError('private source module name already occupied')
            module = types.ModuleType(name)
            module.__file__, module.__package__, module.__cached__ = str(sources[role]), '', None
            module.__spec__, module.__loader__ = None, None
            sys.modules[name] = module
            modules[name] = module
            exec(compile(raw[role], str(sources[role]), 'exec', dont_inherit=True), module.__dict__)
        parent, bounded, emitter = (modules[name] for name in names)
        def bounded_read(path, maximum):
            key = str(Path(path).resolve(strict=True))
            if key not in admitted or len(admitted[key]) > maximum:
                raise ValueError('parser requested source outside captured closure')
            return admitted[key]
        parent.bounded_read = bounded_read
        records, ordered, interfaces, pins = bounded.load_catalog(parent, catalog, False)
        if len(ordered) != 32 or len(records) != 32 or any(record.profile != PROFILE for record in ordered):
            raise ValueError('complete original32 control IR required')
        expected_record_names = EXPECTED_RECORDS
        if {record.name for record in ordered} != expected_record_names:
            raise ValueError('complete fixed original source family required')
        registered = [interface for interface in interfaces if interface.registered]
        if {interface.name for interface in registered} != bounded.control_registered_names() or sum(interface.kind == 'service' for interface in registered) != 7 or sum(interface.kind == 'message' for interface in registered) != 2:
            raise ValueError('complete two Topic/seven Service roots required')
        emitter.validate_ir(parent, bounded, records, ordered, interfaces, ('python',))
        generation_raw = capture(canonical / 'generation.json')
        generation = strict(generation_raw)
        if (generation.get('format') != PROFILE or generation.get('source_inputs') != pins
                or generation.get('implemented_record_outputs') != ['c', 'cpp'] or generation.get('native_registry_emitted') is not False
                or generation.get('generator_sha256') != expected['parent'] or generation.get('bounded_generator_sha256') != expected['bounded']
                or set(generation.get('outputs', {})) != CANONICAL):
            raise ValueError('exact same-source authoritative C/C++ canonical proof required')
        canonical_raw = {canonical / name: capture(canonical / name) for name in CANONICAL}
        for path, body in canonical_raw.items():
            if generation['outputs'][path.name] != {'bytes': len(body), 'sha256': sha(body)}:
                raise ValueError('canonical product identity differs')
        products = emitter.emit_python(parent, bounded, records, ordered, interfaces)
        if set(products) != {'bounded_records.py'}:
            raise ValueError('exact complete Python record source required')
        full_ir = [{**dataclasses.asdict(record), 'symbol': record.symbol} for record in ordered]
        services = []
        for interface in registered:
            if interface.kind == 'service':
                services.append({'symbol': interface.symbol, 'native_type': interface.name,
                    'codec_identity': interface.codec_identity, 'native_identity': interface.identity,
                    'request': interface.components['request'].symbol, 'response': interface.components['response'].symbol})
        # Entire Field/Record metadata is retained; constants/defaults/raw byte
        # domains and native-vs-canonical bounds cannot disappear in a projection.
        products['control_services.py'] = ('# Complete seven original roots; no Native identity or grant inference.\n'
            'from roswasm.owned_service.control import install_services\n'
            'from . import bounded_records\n'
            'install_services(globals(), bounded_records, ' + repr(services) + ')\n')
        proof = {'format': PROFILE, 'status': 'PYTHON_SOURCE_ONLY_NOT_COMPILED_OR_EXECUTED',
            'sources': {role: {'path': str(sources[role]), 'bytes': len(body), 'sha256': sha(body)} for role, body in raw.items()},
            'source_inputs': pins, 'complete_records': full_ir, 'services': services,
            'canonical_generation': {'bytes': len(generation_raw), 'sha256': sha(generation_raw)},
            'canonical_products': generation['outputs'], 'native_registry_emitted': False, 'native_qualification': 'NOT_PERFORMED',
            'implicit_permissions': [], 'actual_execution_count': 0,
            'outputs': {name: {'bytes': len(body.encode()), 'sha256': sha(body.encode())} for name, body in products.items()}}
        for path, body in {**{Path(key): value for key, value in admitted.items()}, **canonical_raw, canonical / 'generation.json': generation_raw}.items():
            if capture(path) != body:
                raise ValueError('admitted source changed before output commit')
        if any(sys.modules.get(name) is not module for name, module in modules.items()):
            raise ValueError('private source module binding changed')
        if sum(len(body.encode()) for body in products.values()) > 32 * 1024 * 1024:
            raise ValueError('generated source extent exceeded')
        output.mkdir(parents=True, exist_ok=False)
        products['__init__.py'] = '# Explicit generated package; importing performs no ROS operation.\n'
        products['generation.python.json'] = json.dumps(proof, indent=2) + '\n'
        for name, body in products.items():
            with (output / name).open('x', encoding='utf-8') as stream:
                stream.write(body)
        return proof
    finally:
        for name, module in modules.items():
            if sys.modules.get(name) is module:
                del sys.modules[name]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source-root', 'catalog', 'canonical', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    for name in ('parent', 'bounded', 'emitter', 'operator'):
        parser.add_argument('--' + name + '-sha256', required=True)
    args = parser.parse_args()
    proof = run(args.source_root, args.catalog, args.canonical, args.out,
                {name: getattr(args, name + '_sha256') for name in ('parent', 'bounded', 'emitter', 'operator')})
    print(json.dumps(proof, indent=2))


if __name__ == '__main__':
    main()
