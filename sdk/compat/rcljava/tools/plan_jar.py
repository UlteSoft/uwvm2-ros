#!/usr/bin/env python3
"""Prepare SDK/classpath and real HLL JAR-producer argv, without executing them.

This is a Linux-target build *planner*, not another JavaIR compiler or a process
owner. It starts no subprocess, imports no producer/SDK module, and grants no
Host right. Execute its ordered argv only through the caller's genuine owned,
bounded process runner. The explicitly source-bound HLL producer owns its Java/Maven children
and independently enforces its current Linux cgroup/affinity/storage contract.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import stat
import zipfile

SOURCE_LIMIT = 4 << 20
TOOL_LIMIT = 256 << 20
JAR_LIMIT = 256 << 20


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


def physical(value):
    p = Path(value)
    require(p.is_absolute(), 'explicit absolute input path required')
    return p.resolve(strict=True)


def identity(s):
    return (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns,
            s.st_ctime_ns, stat.S_IMODE(s.st_mode), s.st_nlink)


def admit(path, limit=SOURCE_LIMIT, *, text=False, executable=False, jar=False):
    """One nonblocking/no-follow ordinary FD; bound hash/read and final name.

    ZIP is DATA. Inspect only its directory, after finite ordinary-file admission;
    never extract/load Java classes. The HLL producer does the full bytecode,
    manifest, archive-expansion and combined-classpath validation independently.
    """
    p = physical(path)
    fd = os.open(p, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    names = None
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and before.st_size <= limit,
                'bounded ordinary input required: ' + str(p))
        require(not executable or before.st_mode & 0o111,
                'explicit tool is not executable: ' + str(p))
        sha = hashlib.sha256(); count = 0; chunks = []
        while True:
            chunk = os.read(fd, 65536)
            if not chunk:
                break
            count += len(chunk)
            require(count <= limit, 'input grew past its read bound')
            sha.update(chunk)
            if text:
                chunks.append(chunk)
        if jar:
            os.lseek(fd, 0, os.SEEK_SET)
            # closefd=False leaves the admitted FD alive through final fstat.
            with os.fdopen(fd, 'rb', closefd=False) as stream:
                with zipfile.ZipFile(stream) as archive:
                    info = archive.infolist()
                    require(len(info) <= 100000, 'JAR directory entry cap')
                    names = set(); expanded = 0
                    for item in info:
                        n = item.filename; rel = PurePosixPath(n)
                        require(n and not rel.is_absolute() and '..' not in rel.parts
                                and '\\' not in n and '\0' not in n,
                                'unsafe JAR member name')
                        require(n not in names, 'duplicate JAR member')
                        require(not item.flag_bits & 1 and
                                (item.external_attr >> 16) & 0o170000 != 0o120000,
                                'encrypted/symlink JAR member unsupported')
                        require(not n.startswith('META-INF/versions/'),
                                'multi-release JAR selection is not implemented')
                        expanded += item.file_size
                        require(expanded <= 512 << 20, 'JAR expanded byte cap')
                        names.add(n)
        after = os.fstat(fd)
    finally:
        os.close(fd)
    require(identity(before) == identity(after) == identity(p.lstat()) and
            count == before.st_size, 'input changed during admission')
    pin = dict(path=str(p), bytes=count, sha256=sha.hexdigest(),
               device=before.st_dev, inode=before.st_ino,
               mtime_ns=before.st_mtime_ns, ctime_ns=before.st_ctime_ns,
               mode=stat.S_IMODE(before.st_mode), nlink=before.st_nlink)
    return b''.join(chunks) if text else None, pin, names


def source_role(root, relative):
    rel = Path(relative)
    require(not rel.is_absolute() and '..' not in rel.parts and rel.parts,
            'closed relative source role required')
    p = (root / rel).resolve(strict=True)
    require(p.is_relative_to(root), 'source role escaped the admitted tree')
    return p


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True,
                   help='actual coherent source root, containing sdk/')
    p.add_argument('--producer', type=Path, required=True,
                   help='explicit actual external HLL java/jar-input/produce_jar.py')
    p.add_argument('--jar', type=Path, required=True)
    p.add_argument('--dependency-jar', type=Path, action='append', default=[])
    p.add_argument('--main-class')
    p.add_argument('--bootstrap', choices=['none', 'official-service', 'default-denied', 'official-client-availability'],
                   default='none', help='exact existing SDK bootstrap source, not reflection')
    p.add_argument('--bootstrap-source', type=Path, action='append', default=[],
                   help='actual reviewed deployment Java source; never generated from text')
    p.add_argument('--source-root', type=Path, action='append', default=[])
    p.add_argument('--source-map', type=Path)
    p.add_argument('--m2', type=Path, required=True)
    p.add_argument('--toolchain-receipt', type=Path, required=True)
    p.add_argument('--toolchain-receipt-sha256', required=True)
    p.add_argument('--patch-root', type=Path, required=True)
    p.add_argument('--maven', type=Path, required=True)
    p.add_argument('--java-home', type=Path, required=True)
    p.add_argument('--helpers', type=Path, required=True)
    p.add_argument('--python', type=Path, required=True)
    p.add_argument('--taskset', type=Path, required=True)
    p.add_argument('--wasm-tools', type=Path, required=True)
    p.add_argument('--cpu', type=int, required=True)
    p.add_argument('--optimization', choices=['SIMPLE', 'FULL'], default='SIMPLE')
    p.add_argument('--max-heap-mib', type=int, default=128)
    p.add_argument('--ros-backend', choices=['1', '2'], default='2')
    p.add_argument('--ros-client-abi', type=Path, help='explicit actual optional service-client catalog; paired SHA required')
    p.add_argument('--ros-client-abi-sha256', help='exact independent optional catalog SHA256, not a Host grant')
    p.add_argument('--require-all-client', action='store_true', help='deliberate optional3tuple fixture only; requires explicit catalog pair')
    p.add_argument('--require-all', action='store_true',
                   help='only for a deliberate complete selected-backend raw fixture')
    p.add_argument('--scratch', type=Path, required=True)
    a = p.parse_args()
    client_flags = client_abi_flags(a.ros_client_abi, a.ros_client_abi_sha256, a.ros_backend)
    require(not a.require_all_client or client_flags, 'complete optional fixture needs explicit catalog pair')
    require(a.bootstrap != 'official-client-availability' or client_flags,
            'original client availability bootstrap needs explicit optional catalog pair')
    require(0 <= a.cpu <= 1048575 and 1 <= a.max_heap_mib <= 1024,
            'finite CPU/Guest heap choice required')
    require(len(a.dependency_jar) <= 62 and len(a.bootstrap_source) <= 32 and
            len(a.source_root) <= 32, 'finite input count required')
    require(re.fullmatch('[0-9a-f]{64}', a.toolchain_receipt_sha256) is not None,
            'actual toolchain receipt SHA256 required')
    root = physical(a.source); helpers = physical(a.helpers)
    m2 = physical(a.m2); java = physical(a.java_home); patches = physical(a.patch_root)
    inputs = []

    def read(path, **kw):
        raw, pin, names = admit(path, **kw); inputs.append(pin)
        return raw, Path(pin['path']), names

    manifest_path = source_role(root, 'sdk/compat/rcljava/tools/jar-input-source-v1.json')
    raw, _, _ = read(manifest_path, text=True)
    manifest = strict_source_json(raw)
    closed_java_tree(root, manifest)
    actual_sources = []
    for role in manifest['sdk_sources']:
        path = source_role(root, role['relative'])
        _, pin, _ = admit(path)
        require(all(pin[k] == role[k] for k in ('bytes', 'sha256', 'mode')),
                'actual SDK source changed: ' + role['relative'])
        inputs.append(pin); actual_sources.append(path)
    require(len(actual_sources) == SDK_SOURCE_COUNT and
            len(set(actual_sources)) == SDK_SOURCE_COUNT,
            'this coherent SDK cohort has exactly61 explicit Java sources')
    for role in manifest['support_sources']:
        _, pin, _ = admit(source_role(root, role['relative']))
        require(all(pin[k] == role[k] for k in ('bytes', 'sha256', 'mode')),
                'actual support source changed: ' + role['relative'])
        inputs.append(pin)
    # All fixed presets are Source inputs of this revision, even when unused.
    for role in manifest['bootstraps'].values():
        _, pin, _ = admit(source_role(root, role['relative']))
        require(all(pin[key] == role[key] for key in ('bytes', 'sha256', 'mode')),
                'actual fixed bootstrap source changed: ' + role['relative'])
        inputs.append(pin)
    client_admission = None
    if client_flags:
        selected_catalog = physical(a.ros_client_abi)
        require(selected_catalog == source_role(root, 'sdk/abi/service-client-v1.json'),
                'optional catalog must be the actual source-bound SDK role')
        client_raw, selected_catalog, _ = read(selected_catalog, text=True)
        client_pin = inputs[-1]
        expected = next(x for x in manifest['support_sources']
                        if x['relative'] == 'sdk/abi/service-client-v1.json')
        require(all(client_pin[k] == expected[k] for k in ('bytes', 'sha256', 'mode'))
                and client_pin['sha256'] == a.ros_client_abi_sha256,
                'actual optional catalog changed or supplied SHA256 mismatched')
        contract = client_abi_contract(json.loads(client_raw), a.ros_backend)
        client_flags = client_abi_flags(selected_catalog, client_pin['sha256'], a.ros_backend)
        client_admission = dict(catalog=client_pin, contract=contract,
                                host_authority_granted=False, native_delivery_qualified=False,
                                require_all_optional=a.require_all_client)
    _, producer, _ = read(a.producer)
    require(all(inputs[-1][key] == manifest['producer'][key]
                for key in ('bytes', 'sha256', 'mode')),
            'external HLL producer body changed; requalify this source contract')
    for role in manifest['helper_sources']:
        _, path, _ = read(source_role(helpers, role['relative']))
        pin = inputs[-1]
        require(all(pin[k] == role[k] for k in ('bytes', 'sha256', 'mode')),
                'actual external HLL helper changed: ' + role['relative'])
    _, receipt, _ = read(a.toolchain_receipt, text=True)
    require(inputs[-1]['sha256'] == a.toolchain_receipt_sha256,
            'actual receipt bytes do not match supplied pin')
    # Do not promote guessed receipt keys/tool versions into successful compiler
    # admission. The actual producer reopens and validates the complete real
    # receipt, four patches, patched class files and full private Maven snapshot.
    tools = {}
    for key, path in [('python', a.python), ('taskset', a.taskset),
                      ('wasm_tools', a.wasm_tools), ('maven', a.maven),
                      ('javac', java / 'bin/javac'), ('jar', java / 'bin/jar'),
                      ('java', java / 'bin/java')]:
        _, admitted, _ = read(path, limit=TOOL_LIMIT, executable=True); tools[key] = admitted
    def classpath_text(paths):
        require(all(':' not in str(x) and '\n' not in str(x) and '\r' not in str(x) for x in paths),
                'classpath input contains a Linux separator/control character')
        return ':'.join(map(str, paths))

    cp = []
    for name in ('teavm-interop', 'teavm-core', 'teavm-classlib'):
        _, path, _ = read(m2 / 'org/teavm' / name / '0.13.1' / (name + '-0.13.1.jar'),
                          limit=JAR_LIMIT); cp.append(path)
    jars = []; seen = set(); protected = set(manifest['sdk_top_level_classes'])
    for path in [a.jar, *a.dependency_jar]:
        _, admitted, names = read(path, limit=JAR_LIMIT, jar=True)
        classes = {name for name in names if name.endswith('.class')}
        require(not seen.intersection(classes), 'duplicate class across original explicit JARs')
        require(not any(c in protected or any(c.startswith(q[:-6] + '$') for q in protected)
                        for c in classes),
                'application/dependency bundles a duplicate SDK/native facade class')
        seen.update(classes); jars.append(admitted)
    require(sum(pin['bytes'] for pin in inputs if pin['path'] in {str(x) for x in jars})
            <= 512 << 20, 'combined original JAR compressed byte cap')
    bootstrap_sources = []
    main_class = a.main_class
    if a.bootstrap != 'none':
        require(a.ros_backend == '2', 'existing finite rcljava bootstraps require ROS2')
        require(not a.bootstrap_source and not a.main_class,
                'a selected exact SDK bootstrap supplies its own main/source')
        role = manifest['bootstraps'][a.bootstrap]
        _, path, _ = read(source_role(root, role['relative']))
        require(inputs[-1]['sha256'] == role['sha256'], 'SDK bootstrap source changed')
        main_class = role['main_class']; bootstrap_sources.append(path)
        if a.bootstrap == 'official-client-availability':
            require('org/ros2/rcljava/examples/client/AddTwoIntsClient.class' in seen,
                    'availability preset requires the unchanged original client main in application JAR')
        if a.bootstrap == 'official-service':
            require('org/ros2/rcljava/examples/service/AddTwoIntsService.class' in seen,
                    'official-service requires the original service main in the actual application JAR')
    else:
        for path in a.bootstrap_source:
            _, path, _ = read(path)
            require(path.suffix == '.java', 'actual bootstrap Java source required')
            bootstrap_sources.append(path)
        require(not bootstrap_sources or main_class is not None,
                'actual direct-call deployment bootstrap needs explicit --main-class')
    require(main_class is None or re.fullmatch(
        r'[A-Za-z_$][A-Za-z0-9_$]*(?:\.[A-Za-z_$][A-Za-z0-9_$]*)*', main_class),
        'unsupported actual main spelling')
    require(not main_class or main_class.replace('.', '/') + '.class' not in protected,
            'main cannot be an SDK library class')
    source_roots = [physical(x) for x in a.source_root]
    source_map = None
    if a.source_map:
        _, source_map, _ = read(a.source_map, text=True)
    scratch_parent = physical(a.scratch.parent)
    scratch = scratch_parent / a.scratch.name
    require(a.scratch.is_absolute() and a.scratch.name not in ('', '.', '..') and
            not scratch.exists() and not scratch.is_symlink(), 'fresh absolute scratch required')
    forbidden = [root, m2, helpers, java, patches, *source_roots]
    require(all(not scratch.is_relative_to(x) for x in forbidden),
            'private output must be outside SDK/compiler/cache/helper/source inputs')
    scratch.mkdir(exist_ok=False)
    for folder in ('sdk-classes', 'bootstrap-classes', 'tmp', 'cache', 'config'):
        (scratch / folder).mkdir()
    sdk_jar = scratch / 'uwvm-rcljava-sdk.jar'
    bootstrap_jar = scratch / 'deployment-bootstrap.jar'
    translated = scratch / 'translated'  # producer must create this itself
    artifact = translated / 'project/target/wasi/application.wasm'
    classpath = classpath_text(cp)
    stages = []

    def stage(name, argv, **fields):
        stages.append(dict(name=name, argv=list(map(str, argv)), cwd=str(scratch), **fields))

    stage('compile-actual-sdk', [tools['javac'], '-J-Xmx768m', '-J-XX:ActiveProcessorCount=1', '--release', '17', '-g', '-encoding',
          'UTF-8', '-proc:none', '-implicit:none', '-sourcepath', '', '-classpath',
          classpath, '-d', scratch / 'sdk-classes', *actual_sources])
    stage('package-actual-sdk', [tools['jar'], '--create', '--file', sdk_jar,
                               '-C', scratch / 'sdk-classes', '.'])
    extra_jars = [sdk_jar, *jars[1:]]
    if bootstrap_sources:
        stage('compile-actual-bootstrap', [tools['javac'], '-J-Xmx768m', '-J-XX:ActiveProcessorCount=1', '--release', '17', '-g',
              '-encoding', 'UTF-8', '-proc:none', '-implicit:none', '-sourcepath', '',
              '-classpath', classpath_text([sdk_jar, *jars, *cp]),
              '-d', scratch / 'bootstrap-classes', *bootstrap_sources])
        stage('package-actual-bootstrap', [tools['jar'], '--create', '--file', bootstrap_jar,
                                         '-C', scratch / 'bootstrap-classes', '.'])
        extra_jars.append(bootstrap_jar)
    argv = [tools['taskset'], '--cpu-list', str(a.cpu), tools['python'], '-I', '-B',
            producer, '--jar', jars[0]]
    for dep in extra_jars:
        argv.extend(['--dependency-jar', dep])
    if main_class:
        argv.extend(['--main-class', main_class])
    for source in source_roots:
        argv.extend(['--source-root', source])
    if source_map:
        argv.extend(['--source-map', source_map])
    for key, value in [('m2', m2), ('toolchain-receipt', receipt),
                       ('toolchain-receipt-sha256', a.toolchain_receipt_sha256),
                       ('patch-root', patches), ('maven', tools['maven']),
                       ('java-home', java), ('helpers', helpers), ('cpu', a.cpu),
                       ('optimization', a.optimization), ('max-heap-mib', a.max_heap_mib),
                       ('ros-backend', a.ros_backend), ('out', translated)]:
        argv.extend(['--' + key, str(value)])
    argv.extend(client_flags)
    # Compiling owned SDK classes does not select their foreign import namespace.
    # The independent raw12 route needs its own policy/catalog/checker pins.
    argv.extend(OWNED_ROUTE['producer_arguments'])
    stage('actual-hll-jar-to-linear-wasi', argv,
          requires='All earlier native commands succeeded and their real processes retired',
          receipt_gate={'path': str(translated / 'receipt.json'), 'passed': True,
                        'all_recorded_commands_owned_retired': True,
                        'artifact_bytes_sha256_must_match_actual_file': True})
    stage('validate-entire-wasm', [tools['wasm_tools'], 'validate', artifact],
          requires='Actual producer receipt passes and matches the admitted artifact')
    gate = [tools['python'], '-I', '-B', source_role(root,
            'sdk/compat/rcljava/tools/check_jar_wasm.py'), '--source', root,
            '--wasm', artifact, '--backend', a.ros_backend,
            '--report', scratch / 'ros-linear-import-contract.json']
    if a.require_all:
        gate.append('--require-all')
    gate.extend(client_flags)
    if a.require_all_client:
        gate.append('--require-all-client')
    stage('check-actual-memory-and-ros-signatures', gate,
          requires='Full real Wasm validation succeeded')
    plan = dict(format_version=1, status='SDK_JAR_ARGV_PREPARED_NOT_EXECUTED',
                compiler_or_producer_invocations=0, default_authority=[],
                guest_management_imports=[], source_manifest=str(manifest_path),
                source_contract_revision=SOURCE_CONTRACT_REVISION,
                sdk_java_sources=SDK_SOURCE_COUNT,
                sdk_source_roots=manifest['sdk_source_roots'],
                sdk_role_set_sha256=SDK_ROLE_SET_SHA256,
                owned_rmw_service=dict(OWNED_ROUTE),
                bootstrap_java_sources=len(bootstrap_sources),
                optional_service_client=client_admission,
                observed_inputs=inputs, stages=stages,
                environment_delta={'JAVA_HOME': str(java),
                    'PATH': str(java / 'bin') + ':/usr/bin:/bin',
                    'LC_ALL': 'C.UTF-8', 'TMPDIR': str(scratch / 'tmp'),
                    'XDG_CACHE_HOME': str(scratch / 'cache'),
                    'XDG_CONFIG_HOME': str(scratch / 'config')},
                remove_ambient_environment=['PYTHONPATH', 'PYTHONHOME', 'CLASSPATH',
                    'JAVA_TOOL_OPTIONS', '_JAVA_OPTIONS', 'JDK_JAVA_OPTIONS', 'JDK_JAVAC_OPTIONS',
                    'MAVEN_ARGS', 'MAVEN_OPTS', 'NODE_OPTIONS'],
                HOME_home_CODEX_HOME='Do not override; preserve caller originals',
                execution_contract=['Root/caller must own every physical process/descendant and enforce CPU, memory, backing and whole/final deadlines; this planner provides no process ownership.',
                    'Serial stages only. Do not shell-eval argv, skip receipt gates, launch the application JVM, or write the original SDK/JAR/m2/tool directories.',
                    'Source-bound producer independently admits its exact Linux cgroup/CPU, four actual compiler patches/receipt, original+private m2 snapshots, complete classes and original-input reopens.',
                    'Producer requires4GiB floor plus private m2/JAR copy headroom and observed anon budget. Preflight real space before any compiler command; a prepared plan proves none of it.',
                    'All inputs are observations at plan creation. Root must bind/reopen the actual inputs and generated SDK/deployment JARs in its owned build receipt before producer use; the producer reopens its own declared JAR/tool inputs.',
                    'The legacy producer helper paths and hashes are Source observations, not proof of their ordinary imports/sys.modules/pyc execution. External keeper must admit actual entry, interpreter/stdlib/helper bytes and preserve them through use.',
                    'Sampled helper ownership and a legacy receipt are insufficient for current execution qualification. External owner must enforce the required process, PIPE bounded-drain, stop/join and resource contracts independently.',
                    'This compatibility plan explicitly disables owned raw12. Use the separately admitted Java owned-service recipe/policy/checker for that route; neither class presence nor an optional-client3 pair enables it.',
                    'Validation/import/memory admission is structural only. Real current Runtime denied13, granted canonical values and independently retained Native interop remain required.'])
    # Emitting a plan cannot certify future compiler inputs; nevertheless a
    # read-time mutation must refuse this plan rather than declare stale pins.
    terminal = {}
    for pin in inputs:
        if pin['path'] not in terminal:
            terminal[pin['path']] = admit(Path(pin['path']), limit=TOOL_LIMIT)[1]
        require(terminal[pin['path']] == pin, 'input changed before plan publication')
    closed_java_tree(root, manifest)
    target = scratch / 'plan.json'
    with target.open('x') as stream:
        json.dump(plan, stream, indent=2); stream.write('\n')
    print(str(target))


if __name__ == '__main__':
    main()
