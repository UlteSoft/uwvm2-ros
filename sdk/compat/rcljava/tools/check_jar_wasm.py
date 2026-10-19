#!/usr/bin/env python3
"""Check one actual JAR-producer artifact's ROS signatures and linear memory.

This does not validate code, instantiate a module or run ROS. Full external Wasm
validation precedes it. Native delivery, denied13 and granted execution require
separate real Runtime receipts. Read the adjacent fixed SDK reader, never an
ambient plugin, application JAR, configurable Python path or producer module.
"""
import argparse
import hashlib
import types
import json
import os
from pathlib import Path, PurePosixPath
import stat
import re

LIMIT = 64 << 20


def read(path, limit=LIMIT):
    physical = path.resolve(strict=True)
    descriptor = os.open(physical, os.O_RDONLY | os.O_NONBLOCK |
                         os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode) or before.st_size > limit:
            raise ValueError('bounded ordinary input required')
        chunks = []; count = 0
        while True:
            chunk = os.read(descriptor, 65536)
            if not chunk:
                break
            count += len(chunk)
            if count > limit:
                raise ValueError('input grew past its read bound')
            chunks.append(chunk)
        after = os.fstat(descriptor)
    finally:
        os.close(descriptor)
    def identity(s):
        return (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns,
                s.st_ctime_ns, s.st_mode, s.st_nlink)
    if identity(before) != identity(after) or identity(after) != identity(physical.lstat()):
        raise ValueError('input changed during read')
    raw = b''.join(chunks)
    if len(raw) != before.st_size:
        raise ValueError('input extent changed')
    return raw, dict(path=str(physical), bytes=len(raw),
                     sha256=hashlib.sha256(raw).hexdigest(),
                     device=before.st_dev, inode=before.st_ino,
                     mtime_ns=before.st_mtime_ns, ctime_ns=before.st_ctime_ns,
                     mode=stat.S_IMODE(before.st_mode), nlink=before.st_nlink)


def require(ok, message):
    if not ok:
        raise ValueError(message)


# This fixed revision describes Source DATA, not authenticated execution.
SOURCE_CONTRACT_REVISION = 4
SDK_SOURCE_COUNT = 61
SDK_ROLE_SET_SHA256 = '96f4df5ea668a05dad16104789d170ebf7dea0be9acaf734cbd0b0097cb5c1e9'
JAVA_SOURCE_ROOTS = (
    ('sdk/java/src/main/java', 25),
    ('sdk/compat/rcljava/src/main/java', 36),
)
SUPPORT_ROLES = {
    'sdk/abi/abi-v1.json', 'sdk/tools/check_wasm_imports.py',
    'sdk/compat/rcljava/tools/plan_jar.py',
    'sdk/compat/rcljava/tools/check_jar_wasm.py',
    'sdk/abi/service-client-v1.json',
}
HELPER_ROLES = {'python/linux_control.py', 'owned_process.py'}
BOOTSTRAP_ROLES = {
    'official-service': 'sdk/compat/rcljava/services/examples/OfficialServiceBootstrap.java',
    'default-denied': 'sdk/compat/rcljava/examples/DefaultDeniedBootstrap.java',
    'official-client-availability':
        'sdk/compat/rcljava/availability/examples/OriginalClientAvailabilityBootstrap.java',
}
PRODUCER_ROLE = 'test/0050.high_level_languages/java/jar-input/produce_jar.py'
OWNED_ROUTE = {
    'enabled': False, 'supported_by_this_route': False,
    'producer_arguments': ['--ros-rmw-service-enabled', '0'],
    'forbidden_namespace': 'uwvm_ros2_rmw_srv_v1',
    'separate_route': 'sdk/java/owned-service/README.md',
}


def strict_source_json(raw):
    """Only inert JSON DATA; reject duplicate keys before cohort admission."""
    def pairs(rows):
        result = {}
        for key, value in rows:
            require(key not in result, 'duplicate source-contract JSON key')
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda value: (_ for _ in ()).throw(
                          ValueError('nonfinite source-contract JSON')))


def source_roles(rows, label):
    require(type(rows) is list and 1 <= len(rows) <= 128,
            'finite explicit source roles required: ' + label)
    names = []
    for row in rows:
        require(type(row) is dict and set(row) == {'relative', 'bytes', 'sha256', 'mode'},
                'exact source identity fields required: ' + label)
        value = row['relative']
        require(type(value) is str and value and '\\' not in value
                and str(PurePosixPath(value)) == value
                and not PurePosixPath(value).is_absolute()
                and all(part not in ('', '.', '..') for part in value.split('/')),
                'canonical relative source role required: ' + label)
        require(type(row['bytes']) is int and 0 <= row['bytes'] <= 4 << 20
                and type(row['mode']) is int and row['mode'] == 0o644
                and type(row['sha256']) is str
                and re.fullmatch('[0-9a-f]{64}', row['sha256']) is not None,
                'bounded exact source pin required: ' + label)
        names.append(value)
    require(len(names) == len(set(names)), 'duplicate source role: ' + label)
    return names


def validate_source_contract(manifest):
    """Fixed61-role closure; a changed count cannot hide a dropped/aliased class."""
    require(type(manifest) is dict and type(manifest.get('format_version')) is int
            and manifest['format_version'] == 1
            and manifest.get('kind') == 'rcljava-existing-jar-source-inputs-v1'
            and type(manifest.get('source_contract_revision')) is int
            and manifest['source_contract_revision'] == SOURCE_CONTRACT_REVISION
            and type(manifest.get('sdk_source_count')) is int
            and manifest['sdk_source_count'] == SDK_SOURCE_COUNT,
            'coherent revision4 current61-source contract required')
    require(manifest.get('default_authority') == []
            and manifest.get('guest_management_imports') == [],
            'source contract cannot grant Guest authority')
    roots = [{'relative': name, 'count': count} for name, count in JAVA_SOURCE_ROOTS]
    require(manifest.get('sdk_source_roots') == roots
            and all(type(row.get('count')) is int for row in manifest['sdk_source_roots']),
            'exact25-core plus36-facade roots required')
    names = source_roles(manifest.get('sdk_sources'), 'SDK')
    require(len(names) == SDK_SOURCE_COUNT and
            hashlib.sha256(('\n'.join(sorted(names)) + '\n').encode()).hexdigest()
            == SDK_ROLE_SET_SHA256
            and manifest.get('sdk_role_set_sha256') == SDK_ROLE_SET_SHA256,
            'exact current61 source-path set required')
    classes = []
    for name in names:
        parents = [root for root, unused in JAVA_SOURCE_ROOTS if name.startswith(root + '/')]
        require(len(parents) == 1 and name.endswith('.java'), 'fixed Java source root required')
        classes.append(name[len(parents[0]) + 1:-5] + '.class')
    require(len(set(classes)) == SDK_SOURCE_COUNT
            and manifest.get('sdk_top_level_classes') == classes,
            'exact complete top-level and duplicate-class protection required')
    for root, count in JAVA_SOURCE_ROOTS:
        require(sum(name.startswith(root + '/') for name in names) == count,
                'complete Java root count required: ' + root)
    require(set(source_roles(manifest.get('support_sources'), 'support')) == SUPPORT_ROLES,
            'fixed reader/legacy ABI/planner/checker/optional3 support closure required')
    require(set(source_roles(manifest.get('helper_sources'), 'external helper')) == HELPER_ROLES,
            'exact external producer helper roles required')
    producer = manifest.get('producer')
    require(type(producer) is dict
            and producer.get('recommended_source_role') == PRODUCER_ROLE
            and producer.get('explicit_external_absolute_path_required') is True
            and producer.get('arguments_always_forwarded') == OWNED_ROUTE['producer_arguments']
            and type(producer.get('bytes')) is int and 0 < producer['bytes'] <= 4 << 20
            and type(producer.get('mode')) is int and producer['mode'] == 0o644
            and type(producer.get('sha256')) is str
            and re.fullmatch('[0-9a-f]{64}', producer['sha256']) is not None,
            'complete actual external producer pin required')
    bootstraps = manifest.get('bootstraps')
    require(type(bootstraps) is dict and set(bootstraps) == set(BOOTSTRAP_ROLES),
            'exact fixed bootstrap role set required')
    for name, relative in BOOTSTRAP_ROLES.items():
        row = bootstraps[name]
        require(type(row) is dict and row.get('relative') == relative
                and type(row.get('main_class')) is str,
                'fixed actual bootstrap/main required')
        source_roles([{key: row[key] for key in ('relative', 'bytes', 'sha256', 'mode')}],
                     'bootstrap')
    optional = manifest.get('optional_service_client')
    require(type(optional) is dict and optional.get('default_enabled') is False
            and optional.get('catalog_role') == 'sdk/abi/service-client-v1.json'
            and optional.get('argument_pair') == ['--ros-client-abi', '--ros-client-abi-sha256']
            and type(optional.get('functions')) is int and optional['functions'] == 3
            and optional.get('selected_module_template') == 'uwvm_ros{backend}_client_v1'
            and optional.get('old_business_inventory') == [38, 43]
            and optional.get('default_authority') == []
            and optional.get('capability_or_same_native_client_qualification') is False
            and optional.get('catalog_sha256') == next(
                row['sha256'] for row in manifest['support_sources']
                if row['relative'] == 'sdk/abi/service-client-v1.json'),
            'explicit defaultoff optional3 catalog pair required')
    require(manifest.get('owned_rmw_service') == OWNED_ROUTE
            and manifest['owned_rmw_service'].get('enabled') is False
            and manifest['owned_rmw_service'].get('supported_by_this_route') is False,
            'this compatibility JAR route forbids owned12; use its separately admitted route')
    return names


def closed_java_tree(root, manifest):
    """Observe exact lexical Java roles; this does not prove future javac inputs."""
    expected = set(validate_source_contract(manifest))
    actual = set()
    for relative, unused in JAVA_SOURCE_ROOTS:
        folder = root / relative
        require(folder.is_dir() and not folder.is_symlink(),
                'actual ordinary Java source directory required')
        for path in folder.rglob('*'):
            require(not path.is_symlink(), 'Java source tree symlink refused')
            if path.suffix != '.java':
                continue
            require(path.is_file() and len(actual) < 128, 'bounded Java source tree required')
            actual.add(path.relative_to(root).as_posix())
    require(actual == expected, 'missing/extra current SDK Java source')
    return expected


def admit_sdk_source_cohort(root, manifest):
    # These are current Source observations, not proof of compiler/JAR ancestry.
    closed_java_tree(root, manifest)
    roles = [*manifest['sdk_sources'], *manifest['support_sources']]
    for row in manifest['bootstraps'].values():
        roles.append({key: row[key] for key in ('relative', 'bytes', 'sha256', 'mode')})
    pins = []
    for role in roles:
        path = (root / role['relative']).resolve(strict=True)
        require(path.is_relative_to(root), 'fixed SDK source role escaped root')
        unused, pin = read(path, 4 << 20)
        require(all(pin[key] == role[key] for key in ('bytes', 'sha256', 'mode')),
                'actual coherent SDK source changed: ' + role['relative'])
        pins.append(pin)
    return pins


def client_abi_contract(document, backend):
    """Closed optional import contract, never a capability or native oracle."""
    require(isinstance(document,dict) and document.get('kind')=='optional-service-client-v1'
            and document.get('schema_version')==1 and document.get('extension_version')==1
            and document.get('original_business_modules_unchanged') is True,
            'unsupported optional service-client ABI catalog')
    require(document.get('output_contract')=={'features_bytes':8,'availability_bytes':1,
            'nonzero_error_leaves_output_untouched':True}
            and document.get('maximum_wait_ns')==60000000000,
            'optional service-client output contract mismatch')
    expected=[{'name':'abi_version','params':[],'results':['i32']},
              {'name':'features','params':['i64','i64','i32','i64'],'results':['i32']},
              {'name':'service_available','params':['i64','i64','i64','i32','i64'],'results':['i32']}]
    modules=document.get('modules')
    require(isinstance(modules,dict) and set(modules)=={'uwvm_ros1_client_v1','uwvm_ros2_client_v1'},
            'optional service-client module closure mismatch')
    for number in ['1','2']:
        row=modules['uwvm_ros'+number+'_client_v1']
        require(isinstance(row,dict) and row.get('backend')==int(number) and row.get('function_count')==3
                and row.get('functions')==expected,'optional service-client signatures mismatch')
    return {'module':'uwvm_ros'+backend+'_client_v1','functions':expected}


def client_abi_flags(path, sha, backend):
    """Explicit paired producer arguments; naming a catalog grants no authority."""
    require((path is None) == (sha is None),
            'optional ABI path and SHA256 must be supplied together')
    if path is None:
        return []
    require(backend in ('1', '2') and Path(path).is_absolute(),
            'optional ABI needs selected backend and absolute catalog path')
    require(re.fullmatch('[0-9a-f]{64}', sha) is not None,
            'actual lowercase optional ABI SHA256 required')
    return ['--ros-client-abi', str(path), '--ros-client-abi-sha256', sha]


def load_optional_catalog(root, manifest, path, sha, backend):
    """Bound actual optional ABI DATA before it can extend signature admission.

    The same nonblocking ordinary FD reader used for the business ABI protects
    this role; never use Path.read_text or load code from the catalog. The caller
    retains/reopens this exact observation before publishing success.
    """
    if not client_abi_flags(path, sha, backend):
        return None, None
    selected = Path(path).resolve(strict=True)
    if selected != (root / 'sdk/abi/service-client-v1.json').resolve(strict=True):
        raise ValueError('optional catalog is not the fixed SDK source role')
    raw, pin = read(selected, 4 << 20)
    expected = next(x for x in manifest['support_sources']
                    if x['relative'] == 'sdk/abi/service-client-v1.json')
    if any(pin[k] != expected[k] for k in ('bytes', 'sha256', 'mode')) or pin['sha256'] != sha:
        raise ValueError('optional catalog source or explicit SHA256 mismatched')
    return client_abi_contract(json.loads(raw), backend), pin


def load_fixed_reader(root, manifest):
    reader_path = root / 'sdk/tools/check_wasm_imports.py'
    abi_path = root / 'sdk/abi/abi-v1.json'
    fixed_pins = []; reader_raw = None; abi_raw = None
    for p in (reader_path, abi_path):
        raw, pin = read(p, 4 << 20)
        expected = next(x for x in manifest['support_sources']
                        if x['relative'] == str(p.relative_to(root)))
        if any(pin[k] != expected[k] for k in ('bytes', 'sha256', 'mode')):
            raise ValueError('fixed SDK import reader/ABI source changed')
        fixed_pins.append(pin)
        if p == reader_path:
            reader_raw = raw
        else:
            abi_raw = raw
    # Execute ONLY the already source-pinned sibling bytes, with the correct
    # __file__ for its fixed ABI path. A SourceFileLoader could instead accept
    # an ambient timestamp-matching .pyc; -B alone suppresses writes, not reads.
    # No configurable Python source/plugin/SDK or producer module is selected.
    module = types.ModuleType('_uwvm_fixed_jar_import_reader')
    module.__file__ = str(reader_path)
    exec(compile(reader_raw, str(reader_path), 'exec', dont_inherit=True), module.__dict__)
    # Preserve both actual current observations; never manufacture an inode or
    # compare two newly read pins while forgetting the pre-import admission.
    for pin in fixed_pins:
        if read(Path(pin['path']), 4 << 20)[1] != pin:
            raise ValueError('fixed SDK input changed while loading')
    return module, fixed_pins, json.loads(abi_raw)


def check_admitted_abi(data, backend, schema, inspect, allowed_modules=(), require_all=False):
    """Use already admitted ABI DATA and the exact real SDK Wasm parser.

    The remaining decision body below is byte-for-byte the original check()
    after its path-based schema load. This parameter seam prevents an unsafe
    second unbounded ABI read without editing the existing SDK Reader or ABI.
    """
    selected={f'uwvm_ros{n}_v1' for n in ((1,2) if backend=='dual' else (int(backend),))}
    expected={(module,row['name']):([p['type'] for p in row['params']],row['results'])
              for module in selected for row in schema['modules'][module]['functions']}
    rows=inspect(data);found=set()
    for row in rows:
        module=row['module'];key=module,row['name']
        if module.startswith('uwvm_ros'):
            if row['kind']!=0 or key not in expected:
                raise ValueError('unknown or wrong-backend ROS import: '+'.'.join(key))
            if (row['params'],row['results'])!=expected[key]:
                raise ValueError('wrong actual Wasm signature: '+'.'.join(key))
            if key in found:raise ValueError('duplicate backend-qualified ROS import')
            found.add(key)
        elif module not in allowed_modules:
            raise ValueError('non-ROS import module needs explicit allowance: '+module)
    if not found:raise ValueError('no ROS import; this is not a guest ABI coverage result')
    if require_all and found!=set(expected):raise ValueError('complete ABI smoke fixture is missing imports')
    return {'status':'BINARY_IMPORT_CONTRACT_PASS','scope':'Binary import signatures only; no full validation or guest/ROS execution',
            'backend':backend,'sha256':hashlib.sha256(data).hexdigest(),'ROS_functions_checked':len(found),
            'required_complete_inventory':require_all,'imports':rows}


def check_with_optional_client(data, backend, schema, inspect, client=None,
                               require_all=False, require_all_client=False):
    """Optional exact3 signature admission, preserving business81 by default.

    At least one original selected business import is still required. Explicit
    optional selection does not permit any other ROS backend, module, name,
    duplicate, kind or width. require_all concerns the ORIGINAL business set;
    require_all_client independently requires all3 optional functions.
    """
    if client is None:
        if require_all_client:
            raise ValueError('complete optional fixture requires explicit catalog pair')
        return check_admitted_abi(data, backend, schema, inspect,
                                  ('wasi_snapshot_preview1',), require_all)
    if client['module'] != 'uwvm_ros' + backend + '_client_v1':
        raise ValueError('wrong optional backend selection')
    rows = inspect(data); ordinary = []; found = set()
    expected = {x['name']:(x['params'], x['results']) for x in client['functions']}
    for row in rows:
        if row['module'] != client['module']:
            ordinary.append(row)
            continue
        if row['kind'] != 0 or row['name'] not in expected:
            raise ValueError('unknown or nonfunction optional service-client import')
        if row['name'] in found:
            raise ValueError('duplicate optional service-client import')
        if (row['params'], row['results']) != expected[row['name']]:
            raise ValueError('wrong actual optional Core Wasm signature')
        found.add(row['name'])
    result = check_admitted_abi(data, backend, schema, lambda unused: ordinary,
                                ('wasi_snapshot_preview1',), require_all)
    if require_all_client and found != set(expected):
        raise ValueError('complete optional service-client fixture is missing imports')
    result['imports'] = rows
    result['optional_service_client'] = dict(module=client['module'],
        functions_checked=len(found), required_complete_inventory=require_all_client,
        host_authority_granted=False, native_delivery_qualified=False)
    return result


def memory_contract(raw, module):
    reader = module.Reader(raw)
    if bytes(reader.take(8)) != b'\0asm\1\0\0\0':
        raise ValueError('expected actual Core Wasm v1')
    memories = []; exports = []; sections = set()
    while reader.position < len(reader.data):
        kind = reader.byte(); body = module.Reader(reader.take(reader.uint()))
        if kind in (5, 7):
            if kind in sections:
                raise ValueError('duplicate actual memory/export section')
            sections.add(kind)
        if kind == 5:
            count = body.count()
            if count != 1:
                raise ValueError('linear JAR bridge requires exactly one defined memory')
            flags = body.uint()
            # This source bridge uses Address.ofData wasm32 payload offsets;
            # memory64/shared/custom-page-size is a different producer contract.
            if flags not in (0, 1):
                raise ValueError('require ordinary unshared wasm32 memory')
            minimum = body.uint()
            maximum = body.uint() if flags & 1 else None
            if minimum > 65536 or maximum is not None and not minimum <= maximum <= 65536:
                raise ValueError('invalid ordinary wasm32 memory limits')
            memories.append(dict(index=0, address_width=32, shared=False,
                                 minimum_pages=minimum, maximum_pages=maximum))
            body.end()
        elif kind == 7:
            for _ in range(body.count()):
                name = body.name(); external = body.byte(); index = body.uint()
                exports.append(dict(name=name, kind=external, index=index))
            body.end()
    if len(memories) != 1:
        raise ValueError('missing actual defined memory0')
    exported_memory = [x for x in exports if x['kind'] == 2]
    if not any(x['index'] == 0 for x in exported_memory):
        raise ValueError('WASI bridge memory0 is not exported')
    if any(x['index'] != 0 for x in exported_memory):
        raise ValueError('unknown exported linear memory')
    # A full validator proves export-index/name/type correctness. No code/data,
    # allocator, TLSF or GC-safety property is inferred from these declarations.
    return dict(memory=memories[0], exported_memory=exported_memory,
                runtime_linear_memory_cap_still_required=True,
                allocator_or_code_validity_proved=False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--wasm', type=Path, required=True)
    parser.add_argument('--backend', choices=['1', '2'], required=True)
    parser.add_argument('--require-all', action='store_true')
    parser.add_argument('--ros-client-abi', type=Path)
    parser.add_argument('--ros-client-abi-sha256')
    parser.add_argument('--require-all-client', action='store_true')
    parser.add_argument('--report', type=Path, required=True)
    a = parser.parse_args()
    root = a.source.resolve(strict=True)
    if not a.source.is_absolute() or not a.wasm.is_absolute() or not a.report.is_absolute():
        raise ValueError('explicit absolute source/artifact/report roles required')
    raw_manifest, manifest_pin = read(root / 'sdk/compat/rcljava/tools/jar-input-source-v1.json', 4 << 20)
    manifest = strict_source_json(raw_manifest)
    cohort_pins = admit_sdk_source_cohort(root, manifest)
    module, fixed_pins, admitted_abi = load_fixed_reader(root, manifest)
    fixed_pins.extend(cohort_pins)
    client, client_pin = load_optional_catalog(root, manifest, a.ros_client_abi,
                                               a.ros_client_abi_sha256, a.backend)
    if client_pin is not None:
        fixed_pins.append(client_pin)
    raw, artifact = read(a.wasm)
    signatures = check_with_optional_client(raw, a.backend, admitted_abi, module.inspect,
                                            client, a.require_all, a.require_all_client)
    # Actual import kind, not a guessed module name: an imported memory or a
    # nonfunction WASI import is incompatible with this owned linear bridge.
    if any(row['kind'] != 0 for row in signatures['imports']):
        raise ValueError('JAR linear route requires function imports only')
    contract = memory_contract(raw, module)
    parent = a.report.parent.resolve(strict=True); target = parent / a.report.name
    if target.is_relative_to(root) or target.exists() or target.is_symlink() or target == Path(artifact['path']):
        raise ValueError('fresh report outside source and artifact required')
    # Observe the same admitted artifact again before any success report write.
    if read(a.wasm)[1] != artifact:
        raise ValueError('actual artifact changed during inspection')
    for pin in fixed_pins:
        if read(Path(pin['path']), 4 << 20)[1] != pin:
            raise ValueError('fixed SDK input changed during inspection')
    if read(root / 'sdk/compat/rcljava/tools/jar-input-source-v1.json', 4 << 20)[1] != manifest_pin:
        raise ValueError('SDK source contract changed during inspection')
    closed_java_tree(root, manifest)
    result = dict(format_version=1, status='JAR_LINEAR_MEMORY_AND_ROS_IMPORTS_MATCHED_NOT_EXECUTED',
                  source_manifest=manifest_pin, artifact=artifact,
                  imports=signatures, linear_memory=contract,
                  optional_catalog=client_pin,
                  source_contract_revision=SOURCE_CONTRACT_REVISION,
                  sdk_java_sources=SDK_SOURCE_COUNT,
                  sdk_role_set_sha256=SDK_ROLE_SET_SHA256,
                  fixed_source_cohort_observations=cohort_pins,
                  owned_rmw_service=dict(OWNED_ROUTE),
                  external_producer_helpers_qualified=False,
                  default_authority=[], guest_management_imports=[],
                  whole_wasm_validation_still_independent=True,
                  Guest_Runtime_Native_acceptance=False)
    with target.open('x') as stream:
        json.dump(result, stream, indent=2); stream.write('\n')
    print(str(target))


if __name__ == '__main__':
    main()
