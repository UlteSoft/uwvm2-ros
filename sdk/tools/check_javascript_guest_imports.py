#!/usr/bin/env python3
"""Check the entire prepared embedded-JS import multiset, without instantiation.

Run only in the owner's admitted Linux interpreter/stdlib/startup and bounded
process framework. The owner must bind this entry's executed bytes BEFORE its
first instruction. Captured-byte loading below binds only the inspector helper;
it does not retrospectively authenticate this entry or make Python hermetic.
There is no module-wide allowance, fallback decoder, source .pyc or local import.
The independent full Wasm validator and real Guest/native runs remain separate.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys
import types


def require(condition, message):
    if not condition:
        raise ValueError(message)


def capture(path, maximum, expected=None):
    """Read one bounded resolved regular descriptor and check exact captured bytes."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'resolved absolute file required')
    selected = path.stat(follow_symlinks=False)
    require(stat.S_ISREG(selected.st_mode) and 0 <= selected.st_size <= maximum, 'bounded regular file required')
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    with os.fdopen(fd, 'rb') as stream:
        first = os.fstat(stream.fileno())
        require(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum and
                (first.st_dev, first.st_ino) == (selected.st_dev, selected.st_ino), 'file changed at open')
        data = stream.read(maximum + 1)
        last = os.fstat(stream.fileno())
    final = path.stat(follow_symlinks=False)
    fields = ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
    require(len(data) == first.st_size and len(data) <= maximum and
            all(getattr(first, k) == getattr(last, k) == getattr(final, k) for k in fields), 'file changed during capture')
    digest = hashlib.sha256(data).hexdigest()
    require(expected is None or digest == expected, 'captured source/contract hash differs')
    return data, {'path': str(path), 'bytes': len(data), 'sha256': digest,
                  'stat': {k: str(getattr(first, k)) for k in fields}}


# BEGIN SHARED PURE CONTRACT DATA
RMW_MODULE = 'uwvm_ros2_rmw_srv_v1'
RMW_SIGNATURES = [
    ('abi_version', []),
    ('client_activate', ['i64', 'i64', 'i32', 'i64']),
    ('service_activate', ['i64', 'i64', 'i32', 'i64']),
    ('endpoint_inspect', ['i64', 'i64', 'i32', 'i64']),
    ('client_submit', ['i64', 'i64', 'i32', 'i64', 'i64', 'i64']),
    ('client_abandon', ['i64', 'i64', 'i64']),
    ('service_respond', ['i64', 'i64', 'i64', 'i64', 'i32', 'i64', 'i64']),
    ('endpoint_peek', ['i64', 'i64', 'i32', 'i32', 'i64', 'i64']),
    ('endpoint_commit', ['i64', 'i64', 'i64', 'i64']),
    ('endpoint_cancel', ['i64', 'i64', 'i64', 'i64']),
    ('client_available', ['i64', 'i64', 'i64', 'i64', 'i32', 'i64']),
    ('endpoint_retire', ['i64', 'i64'])]
RMW_OUTPUT = {
    'activation_inspection_bytes': 256, 'submit_ticket_bytes': 8,
    'availability_bytes': 1,
    'peek': 'one complete UMRC160+URMI192+canonical body checked copy, no separate length write',
    'minimum_peek_capacity': 352, 'maximum_peek_capacity': 4194304,
    'nonzero_error_preserves_output': True, 'ack_abandon_retire_no_output': True}
LEGACY_CATALOG_SHA256 = '0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0'


def rmw_catalog_contract(document):
    """Validate the complete optional12 numeric/output contract as DATA only."""
    fields = {'schema_version', 'extension_version', 'kind', 'source_status',
              'authority_source', 'implementation_source', 'raw_header',
              'original_business_modules_unchanged', 'authority_contract', 'modules',
              'output_contract', 'quota_contract', 'pair_history',
              'transaction_contract', 'legacy_preservation', 'actual_compilers_models_Guest_native_ROS'}
    require(type(document) is dict and set(document) == fields, 'closed owned-RMW service catalog')
    require(type(document['schema_version']) is int and document['schema_version'] == 1 and
            type(document['extension_version']) is int and document['extension_version'] == 1 and
            document['kind'] == 'optional-owned-rmw-service-v1' and
            document['source_status'] == 'SOURCE_ONLY_HELD_UNEXECUTED' and
            document['original_business_modules_unchanged'] is True and
            type(document['actual_compilers_models_Guest_native_ROS']) is int and
            document['actual_compilers_models_Guest_native_ROS'] == 0, 'owned-RMW source-only catalog version')
    paths = {'authority_source': 'src/uwvm2/uwvm/imported/ros/host.h',
             'implementation_source': 'src/uwvm2/uwvm/imported/ros/rmw_service_host.h',
             'raw_header': 'sdk/include/roswasm/rmw_service_v1.h'}
    for key, path in paths.items():
        pin = document[key]
        require(type(pin) is dict and set(pin) == {'path', 'sha256'} and pin['path'] == path and
                isinstance(pin['sha256'], str) and re.fullmatch('[0-9a-f]{64}', pin['sha256']), 'fixed owned-RMW source binding')
    functions = [{'name': name, 'params': params, 'results': ['i32']} for name, params in RMW_SIGNATURES]
    require(document['modules'] == {RMW_MODULE: {'backend': 2, 'function_count': 12, 'functions': functions}},
            'complete ordered12 exact owned-RMW signatures required')
    require(type(document['modules'][RMW_MODULE]['backend']) is int and
            type(document['modules'][RMW_MODULE]['function_count']) is int, 'exact numeric catalog fields')
    require(document['output_contract'] == RMW_OUTPUT and
            all(type(document['output_contract'][k]) is int for k in
                ('activation_inspection_bytes', 'submit_ticket_bytes', 'availability_bytes', 'minimum_peek_capacity', 'maximum_peek_capacity')) and
            document['output_contract']['nonzero_error_preserves_output'] is True and
            document['output_contract']['ack_abandon_retire_no_output'] is True, 'exact owned-RMW output contract')
    require(document['legacy_preservation'] == {'catalog_path': 'sdk/abi/abi-v1.json',
            'catalog_sha256': LEGACY_CATALOG_SHA256}, 'unchanged original business81 catalog required')
    require(isinstance(document['authority_contract'], str) and document['authority_contract'] and
            isinstance(document['pair_history'], str) and document['pair_history'] and
            type(document['quota_contract']) is dict and type(document['transaction_contract']) is dict,
            'catalog authority/lifetime scope required')
    return functions


def full_import_contract(manifest, original, optional, modular_motion=False):
    """Build a closed multiset of actual numeric imports, never a module allowance.

    Optional activation is decided by the explicit CLI, not manifest modules or
    generic scalar declarations. Keep business81/client extension files unchanged.
    """
    rows = []
    def add(module, name, params, results, role):
        require(isinstance(module, str) and isinstance(name, str) and module and name and
                type(params) is list and type(results) is list and
                all(t in ('i32', 'i64', 'f32', 'f64') for t in params + results), 'closed numeric import row')
        require(not any(r['module'] == module and r['name'] == name for r in rows), 'duplicate expected module/name')
        rows.append({'module': module, 'name': name, 'kind': 0, 'params': list(params), 'results': list(results), 'role': role})
    backend = manifest['backend']
    if backend:
        module = 'uwvm_ros' + str(backend) + '_v1'
        require(type(original) is dict and set(original['modules']) == {'uwvm_ros1_v1', 'uwvm_ros2_v1'}, 'original business modules')
        for number in (1, 2):
            block = original['modules']['uwvm_ros' + str(number) + '_v1']
            require(type(block['backend']) is int and block['backend'] == number and
                    type(block['function_count']) is int and block['function_count'] == (38 if number == 1 else 43) and
                    len(block['functions']) == block['function_count'], 'complete unchanged business inventory')
        for row in original['modules'][module]['functions']:
            add(module, row['name'], [p['type'] for p in row['params']], row['results'], 'business')
    if optional is not None:
        require(backend == 2, 'owned-RMW optional bridge is ROS2 only')
        for row in rmw_catalog_contract(optional):
            add(RMW_MODULE, row['name'], row['params'], row['results'], 'owned-rmw-service')
    if backend:
        for name in ('fd_read', 'fd_write'):
            add('wasi_snapshot_preview1', name, ['i32'] * 4, ['i32'], 'basic-wasi')
    if manifest['mode'] == 'command':
        add('wasi_snapshot_preview1', 'proc_exit', ['i32'], [], 'basic-wasi')
    for row in manifest['imports']:
        require(row['module'] != RMW_MODULE, 'reserved owned-RMW namespace requires checked dedicated bridge')
        add(row['module'], row['name'], [t.replace('u', 'i', 1) if t.startswith('u') else t for t in row['parameters']],
            [row['result'].replace('u', 'i', 1) if row['result'].startswith('u') else row['result']], 'scalar-library')
    if modular_motion:
        for module, name, count in [('motion', 'motion_begin', 2), ('motion', 'motion_status', 0),
                ('motion', 'motion_word', 3), ('motion', 'motion_end', 1),
                ('perception', 'perception_begin', 1), ('perception', 'perception_status', 0),
                ('perception', 'perception_word', 3), ('perception', 'perception_end', 2), ('perception', 'perception_abort', 1)]:
            add(module, name, ['i32'] * count, ['i32'], 'unitree-pure-scalar')
    return rows

# END SHARED PURE CONTRACT DATA


def compare_rows(actual, expected):
    """Require exactly every module/name/kind/parameter/result, once each."""
    wanted = {}
    for row in expected:
        require(type(row) is dict and set(row) == {'module', 'name', 'kind', 'params', 'results', 'role'} and
                type(row['kind']) is int and row['kind'] == 0, 'closed expected function row')
        key = row['module'], row['name']
        require(key not in wanted, 'duplicate expected import')
        wanted[key] = {k: row[k] for k in ('module', 'name', 'kind', 'params', 'results')}
    found = {}
    for row in actual:
        require(type(row) is dict and set(row) == {'module', 'name', 'kind', 'params', 'results'} and
                type(row['kind']) is int and row['kind'] == 0, 'unexpected non-function or malformed import')
        key = row['module'], row['name']
        require(key not in found, 'duplicate actual import')
        require(key in wanted and row == wanted[key], 'unexpected import or wrong complete actual signature')
        found[key] = row
    require(found == wanted and wanted, 'missing complete expected import inventory')


def admitted_contract(document):
    """Capture all contract-listed Source inputs before executing inspector bytes."""
    require(type(document) is dict and set(document) == {'schema', 'backend', 'mode', 'rmw_service_enabled',
            'source_root', 'catalog_sha256', 'scalar_imports', 'unitree_modular_motion',
            'scratch', 'expected_imports', 'source_bindings'}, 'closed full-import contract')
    require(document['schema'] == 'uwvm-javascript-owned-rmw-full-imports-v1' and
            type(document['backend']) is int and document['backend'] == 2 and
            type(document['rmw_service_enabled']) is int and document['rmw_service_enabled'] == 1 and
            document['mode'] in ('command', 'reactor') and type(document['unitree_modular_motion']) is bool,
            'explicit ROS2 owned-RMW contract required')
    scratch = document['scratch']
    require(type(scratch) is dict and set(scratch) == {'input_bytes', 'output_bytes'} and
            all(type(n) is int for n in scratch.values()) and
            0 < scratch['input_bytes'] <= 4193786 and 352 <= scratch['output_bytes'] <= 4193786,
            'independently bounded scratch required')
    root = Path(document['source_root'])
    require(root.is_absolute() and root.resolve(strict=True) == root, 'resolved Source root required')
    paths = {'producer': 'sdk/javascript/guest/prepare.py',
             'checker': 'sdk/tools/check_javascript_guest_imports.py',
             'inspector': 'sdk/tools/check_wasm_imports.py',
             'legacy_catalog': 'sdk/abi/abi-v1.json', 'catalog': 'sdk/abi/rmw-service-v1.json',
             'authority_source': 'src/uwvm2/uwvm/imported/ros/host.h',
             'implementation_source': 'src/uwvm2/uwvm/imported/ros/rmw_service_host.h',
             'raw_header': 'sdk/include/roswasm/rmw_service_v1.h',
             'engine_header': 'sdk/javascript/guest/engine.h', 'engine_source': 'sdk/javascript/guest/engine.c',
             'native_bridge': 'sdk/javascript/guest/native_rmw_service.c'}
    if document['unitree_modular_motion']:
        paths.update(unitree_motion='sdk/examples/robotics_modular/unitree_motion_abi.h',
                     unitree_perception='sdk/examples/robotics_modular/unitree_perception_abi.h')
    bindings = document['source_bindings']
    require(type(bindings) is dict and set(bindings) == set(paths), 'all required contract Source bindings required')
    captured, observed = {}, {}
    for name, relative in paths.items():
        pin = bindings[name]
        require(type(pin) is dict and set(pin) == {'path', 'bytes', 'sha256'} and pin['path'] == str(root / relative) and
                type(pin['bytes']) is int and 0 < pin['bytes'] <= 2097152 and
                isinstance(pin['sha256'], str) and re.fullmatch('[0-9a-f]{64}', pin['sha256']), 'exact Source binding required')
        data, actual = capture(Path(pin['path']), pin['bytes'], pin['sha256'])
        require(actual['bytes'] == pin['bytes'], 'exact Source extent differs')
        captured[name], observed[name] = data, actual
    require(observed['legacy_catalog']['sha256'] == LEGACY_CATALOG_SHA256 and
            observed['catalog']['sha256'] == document['catalog_sha256'], 'raw catalog digest differs')
    original, optional = json.loads(captured['legacy_catalog']), json.loads(captured['catalog'])
    rmw_catalog_contract(optional)
    for key in ('authority_source', 'implementation_source', 'raw_header'):
        require(optional[key] == {'path': paths[key], 'sha256': observed[key]['sha256']}, 'catalog does not bind actual Source bytes')
    scalar = document['scalar_imports']
    require(type(scalar) is list and len(scalar) <= 1024, 'bounded scalar import list')
    for row in scalar:
        require(type(row) is dict and set(row) == {'module', 'name', 'parameters', 'result'} and
                isinstance(row['module'], str) and isinstance(row['name'], str) and
                type(row['parameters']) is list and len(row['parameters']) <= 7 and
                all(t in ('i32', 'u32', 'i64', 'u64', 'f32', 'f64') for t in row['parameters']) and
                row['result'] in ('i32', 'u32', 'i64', 'u64', 'f32', 'f64'), 'closed numeric scalar declaration')
    expected = full_import_contract({'backend': 2, 'mode': document['mode'], 'imports': scalar},
                                    original, optional, document['unitree_modular_motion'])
    require(document['expected_imports'] == expected, 'complete producer import contract differs')
    return captured, observed, expected


def inspect_captured(source, filename, data):
    """Execute exactly admitted inspector bytes under a fresh private name.

    No SourceFileLoader/importlib/from-local import is used, so .pyc and ambient
    module aliases cannot replace these bytes. Stdlib imports/caches remain part
    of the externally admitted interpreter trust boundary. __file__ is the real
    pinned helper path, package empty; only inspect(data), not helper.check/main,
    is called. Registration is ours and removed on every return/exception.
    """
    name = '_uwvm_js_binary_inspector_' + hashlib.sha256(source).hexdigest() + '_' + str(id(source))
    require(name not in sys.modules, 'private inspector name collision')
    module = types.ModuleType(name)
    module.__file__, module.__package__, module.__spec__, module.__loader__ = str(filename), '', None, None
    sys.modules[name] = module
    try:
        exec(compile(source, str(filename), 'exec', dont_inherit=True, optimize=0), module.__dict__)
        require(sys.modules.get(name) is module and callable(module.__dict__.get('inspect')), 'inspector context changed')
        return module.inspect(data)
    finally:
        if sys.modules.get(name) is module:
            del sys.modules[name]


def main():
    require(sys.platform == 'linux' and sys.flags.isolated and sys.flags.no_site,
            'owner-admitted Linux Python -I -S required; -B does not prevent reading pyc')
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--wasm', type=Path, required=True)
    parser.add_argument('--contract', type=Path, required=True)
    parser.add_argument('--contract-sha256', required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    require(re.fullmatch('[0-9a-f]{64}', args.contract_sha256), 'exact contract digest required')
    raw, contract_pin = capture(args.contract, 2097152, args.contract_sha256)
    document = json.loads(raw)
    captured, source_pins, expected = admitted_contract(document)
    wasm, wasm_pin = capture(args.wasm, 64 * 1024 * 1024)
    actual = inspect_captured(captured['inspector'], source_pins['inspector']['path'], wasm)
    compare_rows(actual, expected)
    result = {'status': 'JS_OWNED_RMW_COMPLETE_BINARY_IMPORT_CONTRACT_PASS',
              'scope': 'Exact imports/signatures only; full validator and actual Runtime/native execution are separate',
              'wasm': wasm_pin, 'contract': contract_pin, 'captured_sources': source_pins,
              'business_functions': 43, 'owned_rmw_service_functions': 12, 'imports': actual,
              'runtime_or_native_execution': 0, 'entry_startup_admission': 'REQUIRED_EXTERNAL_OWNER',
              'inspector_loading': 'same captured hash-checked Source bytes compile/exec; no pyc/local import'}
    require(args.report.is_absolute() and args.report.parent.resolve(strict=True) == args.report.parent,
            'resolved absolute report parent required')
    with args.report.open('x') as output:
        output.write(json.dumps(result, indent=2, sort_keys=True) + '\n')
    print('PASS exact complete embedded-JS imports:', len(actual))


if __name__ == '__main__':
    main()
