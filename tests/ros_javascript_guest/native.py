#!/usr/bin/env python3
"""Real Gateway/native witnesses for the embedded JavaScript Runtime lane.

Root supplies prepared native executables, exact official interface/capability
proofs and an isolated loopback namespace. This adapter starts actual separate
Worker, Gateway and typed native peer processes; no Node engine, fake Memory,
ROS substitute, raw Gateway RPC client or extra Guest right is available.
"""
import argparse
import hashlib
import http.client
import json
import os
from pathlib import Path
import re
import stat
import sys
import time
import types
import xmlrpc.client

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]


def load_runtime(args):
    """Bind the common execution adapter before loading its definitions."""
    def read(path, maximum):
        descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
        try:
            before = os.fstat(descriptor)
            if not stat.S_ISREG(before.st_mode) or not 0 < before.st_size <= maximum:
                raise ValueError('bounded ordinary bootstrap file required')
            data = bytearray()
            while True:
                block = os.read(descriptor, min(65536, maximum-len(data)+1))
                if not block: break
                data.extend(block)
                if len(data) > maximum: raise ValueError('bootstrap input grew')
            after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
            if len(data) != before.st_size or any(getattr(before, key) != getattr(after, key) or
                getattr(before, key) != getattr(named, key) for key in
                ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')):
                raise ValueError('bootstrap ordinary file changed')
            return bytes(data)
        finally:
            os.close(descriptor)
    path = ROOT/'tests/ros_javascript_guest/run.py'
    raw = read(args.manifest, 16*1024**2)
    if not 0 < len(raw) <= 16*1024**2 or hashlib.sha256(raw).hexdigest() != args.manifest_sha256:
        raise ValueError('exact bounded manifest required')
    def pairs(values):
        result = {}
        for key, value in values:
            if key in result: raise ValueError('duplicate bootstrap manifest key')
            result[key] = value
        return result
    def constant(_): raise ValueError('nonfinite bootstrap manifest')
    manifest = json.loads(raw, object_pairs_hook=pairs, parse_constant=constant)
    # The called bootstrap additionally refuses duplicate/nonfinite metadata,
    # opens ordinary helpers without following symlinks and binds this entry.
    code = read(path, 1024**2)
    if manifest.get('tests/ros_javascript_guest/run.py') != {'bytes': len(code), 'sha256': hashlib.sha256(code).hexdigest()}:
        raise ValueError('Runtime adapter source is not frozen')
    module = types.ModuleType('embedded_js_bound_runtime_adapter')
    module.__file__ = str(path)
    sys.modules[module.__name__] = module
    exec(compile(code, str(path), 'exec'), module.__dict__)
    return module


def command(frame, job, replacements=None):
    """Keep selected multicall aliases distinct from their physical target."""
    frame.unitree.exact(job, ('executable', 'arguments', 'alias'), 'native executable selection')
    argv = frame.command({'executable': job['executable'], 'arguments': job['arguments']}, replacements)
    if job['alias'] is not None:
        alias = Path(job['alias'])
        frame.unitree.require(alias.is_absolute() and alias.parent.resolve(strict=True) == alias.parent and
            alias.resolve(strict=True) == frame.input(job['executable']) and
            any(alias.is_relative_to(root) for root in frame.roots), 'explicit inventoried executable alias required')
        current = alias.lstat()
        facts = {'path': str(alias), 'device': current.st_dev, 'inode': current.st_ino,
            'mtime_ns': current.st_mtime_ns, 'ctime_ns': current.st_ctime_ns,
            'target': os.readlink(alias) if alias.is_symlink() else None}
        previous = frame.native_aliases.setdefault(str(alias), facts)
        frame.unitree.require(previous == facts, 'native executable alias changed')
        argv[0] = str(alias)
    frame.unitree.require(Path(argv[0]).name not in ('bash', 'sh', 'sudo', 'systemd-run', 'podman', 'docker'),
                         'no shell, daemon or external launcher in native actor argv')
    return argv


def owner_pid(frame, owner, target):
    """Find an exact actual executable among already owned births, not argv."""
    owner.check()
    expected = frame.pin(target)
    matches = []
    for key, item in owner.owner.owned.items():
        if frame.common.pidfd_terminal(item['fd']):
            continue
        try:
            fd = os.open('/proc/%d/exe' % key[0], os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK)
        except (FileNotFoundError, ProcessLookupError):
            continue
        try:
            current = os.fstat(fd)
            if all(getattr(current, attr) == expected[name] for attr, name in
                   (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                    ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))):
                matches.append(key[0])
        finally:
            os.close(fd)
    frame.unitree.require(len(matches) == 1, 'one actual owned native executable required')
    return matches[0]


def network_namespace(frame, pid):
    """Native actors and the exec gate must retain this isolated loopback view."""
    own, actor = Path('/proc/self/ns/net').stat(), Path('/proc/%d/ns/net' % pid).stat()
    frame.unitree.require((own.st_dev, own.st_ino) == (actor.st_dev, actor.st_ino),
                         'actor escaped the explicitly isolated network namespace')
    return {'device': actor.st_dev, 'inode': actor.st_ino}


def mappings(api, frame, owner, pid, binary, labels, catalog, elf_catalog):
    """Prove selected real SDK DSOs through opened map_files descriptors.

    Only explicitly pinned target physical files are hashed. The shared helper
    never treats Btrfs maps display devices as stat identities and never stops
    a process. Full host executable and owned-birth guards remain separate.
    """
    require = frame.unitree.require
    require(type(labels) is list and 1 <= len(labels) <= 32 and len(set(labels)) == len(labels),
            'explicit nonempty native SDK mapping inputs required')
    helper = api.frozen_module(frame.source, frame.frozen, 'tests/ros_aarch64/runtime.py', 'embedded_js_native_fd_mappings')
    proof = api.executable_proof(frame, owner, pid, binary)
    network = network_namespace(frame, pid)
    wanted = {}
    for label in labels:
        pin = frame.pin(frame.input(label))
        with Path(pin['path']).open('rb') as stream:
            header = stream.read(64)
        require(frame.pin(pin['path']) == pin and len(header) >= 20 and header[:6] == b'\x7fELF\x02\x01' and
                int.from_bytes(header[18:20], 'little') == 62, 'actual native x86_64 SDK mapping input required')
        wanted[label] = dict(pin, elf_class=2, elf_data=1, e_machine=62)
    selected = proof['selected']
    initial = proof['last']
    def guard():
        frame.guard()
        now = frame.common.identity(pid)
        require(frame.common.birth(now) == frame.common.birth(initial) and
                Path('/proc/%d/cgroup' % pid).read_text() == Path('/proc/self/cgroup').read_text(),
                'native mapping birth/cgroup changed')
        entry = [item for key, item in owner.owner.owned.items() if key[0] == pid]
        require(len(entry) == 1 and not frame.common.pidfd_terminal(entry[0]['fd']), 'owned native mapping pidfd retired')
        host = helper.mapping_file_facts(Path('/proc/%d/exe' % pid).stat())
        require(host == {name: selected[name] for name in helper._MAP_FACTS}, 'native host executable changed')
        require(str(pid) in (Path(frame.config['cgroup']['expected_leaf'])/'cgroup.procs').read_text().split(),
                'native birth absent from original current64 leaf')
        require(network_namespace(frame, pid) == network, 'native network namespace changed')
        return {'pid': pid, 'parent_pid': now['parent_pid'], 'starttime_ticks': now['starttime'],
                'state': now['state'], 'membership': Path('/proc/%d/cgroup' % pid).read_text().splitlines(),
                'live_cgroup_procs': True, 'host_identity': host}
    receipt = helper.opened_target_mappings(pid, wanted, guard)
    require(all(receipt['targets'][label] for label in wanted), 'selected real native SDK DSO was not mapped')
    # The selected targets establish required SDK roles. Independently reject
    # any other actually mapped ELF not present in the producer's complete
    # physical SDK/product catalog. maps pathnames and display devices never
    # select a trusted file; every inode-backed entry is opened and fstat'ed.
    actual = []
    by_identity = {tuple(row[key] for key in ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns')): row
                   for row in [*catalog, frame.pin(binary)]}
    elf_identities = {tuple(row[key] for key in ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns'))
                      for row in elf_catalog}
    captured = {}
    guard()
    with Path('/proc/%d/maps' % pid).open('rb') as stream:
        raw = stream.read(4*1024**2+1)
    require(len(raw) <= 4*1024**2, 'bounded actual native map inventory')
    for line in raw.decode('utf-8', errors='surrogateescape').splitlines():
        parts = line.split(None, 5)
        if len(parts) < 5 or not parts[4].isdecimal() or int(parts[4]) == 0: continue
        require(re.fullmatch('[0-9a-f]+-[0-9a-f]+', parts[0]) is not None, 'actual map range malformed')
        guard()
        descriptor = os.open('/proc/%d/map_files/%s' % (pid, helper.canonical_map_files_range(parts[0])), os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK)
        try:
            before = os.fstat(descriptor)
            require(stat.S_ISREG(before.st_mode), 'actual native map must be an ordinary physical file')
            header = os.read(descriptor, 64)
            key = tuple(getattr(before, name) for name in
                        ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns'))
            require(key in by_identity, 'actual native mapping outside complete admitted SDK/product closure')
            require(not header.startswith(b'\x7fELF') or key in elf_identities or frame.pin(binary) == by_identity[key],
                    'actual native ELF missing from complete admitted ELF closure')
            selected_file = by_identity[key]
            if key not in captured:
                digest, size = hashlib.sha256(), 0
                os.lseek(descriptor, 0, os.SEEK_SET)
                while True:
                    guard(); block = os.read(descriptor, min(1024**2, selected_file['bytes']-size+1))
                    if not block: break
                    size += len(block); require(size <= selected_file['bytes'], 'native mapped ELF grew')
                    digest.update(block)
                require(size == selected_file['bytes'] and digest.hexdigest() == selected_file['sha256'],
                        'actual mapped native ELF differs from original SDK/product bytes')
                captured[key] = selected_file
            after = os.fstat(descriptor)
            require(all(getattr(before, key) == getattr(after, key) for key in
                        ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')),
                    'actual native map descriptor changed')
            actual.append({'range': parts[0], 'display_only_mapping': line, 'physical_file': captured[key],
                           'ELF': header.startswith(b'\x7fELF')})
        finally:
            os.close(descriptor)
        guard()
    guard()
    return {'executable': proof, 'network_namespace': network, 'SDK_mappings': receipt,
            'all_observed_file_mappings': actual, 'unknown_file_fallback_allowed': False}


def document(api, frame, path, maximum=64*1024**2):
    """Keep a larger native report bounded, stable and strict JSON."""
    raw = api.bounded(Path(path), maximum)
    pin = frame.pin(path)
    frame.unitree.require(len(raw) == pin['bytes'] and hashlib.sha256(raw).hexdigest() == pin['sha256'],
                         'native metadata changed between stable descriptor reads')
    def pairs(items):
        result = {}
        for key, value in items:
            frame.unitree.require(key not in result, 'duplicate native metadata key')
            result[key] = value
        return result
    def constant(_): raise ValueError('nonfinite native metadata')
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=constant), pin


def file_catalog(frame, pin):
    """Read the producer's complete physical file inventory as bounded JSONL."""
    require = frame.unitree.require
    require(frame.pin(pin['path']) == pin and 0 < pin['bytes'] <= 512*1024**2,
            'exact bounded complete native file catalog required')
    descriptor = os.open(pin['path'], os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    result, seen, count, digest = [], set(), 0, hashlib.sha256()
    with os.fdopen(descriptor, 'rb') as stream:
        while True:
            line = stream.readline(16385)
            if not line: break
            frame.guard(); require(len(line) <= 16384 and line.endswith(b'\n'), 'bounded native file catalog row')
            count += len(line); require(count <= pin['bytes'] and len(result) < 1000000, 'finite native catalog bounds')
            digest.update(line)
            row = json.loads(line)
            frame.unitree.exact(row, api_pin_fields(), 'native catalog physical pin')
            require(type(row['path']) is str and row['path'] not in seen and
                    Path(row['path']).resolve(strict=True) == Path(row['path']) and
                    any(Path(row['path']).is_relative_to(root) for root in frame.roots),
                    'native catalog must name unique inventoried physical files')
            require(frame.pin(row['path']) == row, 'native catalog file differs from current physical input')
            seen.add(row['path']); result.append(row)
    require(count == pin['bytes'] and digest.hexdigest() == pin['sha256'] and frame.pin(pin['path']) == pin,
            'complete native catalog changed')
    return result


def api_pin_fields():
    return ('path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns')


def qualify(api, frame, row):
    """Reopen original native producer provenance and selected execution copy."""
    exact, require = frame.unitree.exact, frame.unitree.require
    exact(row, ('backend', 'report', 'peer', 'peer_binary', 'peer_mappings', 'gateway', 'gateway_binary',
                'gateway_mappings', 'gateway_context_mappings', 'exec_gate', 'local_report', 'master',
                'connection', 'event_artifact', 'event_receipt', 'native_profile'), 'native case')
    backend = row['backend']
    require(type(backend) is int and backend in (1, 2), 'explicit ROS backend')
    report, pin = document(api, frame, frame.input(row['report']))
    require(report.get('schema') == 'uwvm-embedded-js-native-products-v1' and
            report.get('status') == 'PREPARED_NATIVE_PRODUCTS_NOT_EXECUTED' and
            report.get('source') == str(frame.source) and
            report.get('source_manifest_sha256') == frame.report['source_manifest_sha256'] and
            all(report.get(key) is True for key in ('source_preserved', 'inputs_preserved', 'products_preserved')) and
            report.get('cleanup', {}).get('status') == 'PASS' and not report['cleanup']['remaining'] and
            not report.get('finalization_errors'), 'actual current native product preparation required')
    api.retained(frame, report)
    base = report['execution_environment_base']
    exact(base, ('LD_LIBRARY_PATH', 'AMENT_PREFIX_PATH'), 'native-only SDK execution environment')
    for name, value in base.items():
        require(type(value) is str and value and not any(c in value for c in '\0\n\r'), 'clean native SDK path list')
        paths = value.split(':')
        require(len(paths) == len(set(paths)) and all(Path(p).is_absolute() and
            Path(p).resolve(strict=True) == Path(p) and Path(p).is_dir() and
            any(Path(p) == root or Path(p).is_relative_to(root) for root in frame.roots) for p in paths),
            'native SDK environment directories must belong to complete actual inventories')
    require(type(report['execution_view_after']) is list and report['execution_view_after'], 'complete native execution-view inventories')
    for inventory in report['execution_view_after']:
        current = [value for value in frame.catalogs if value['root'] == inventory['root']]
        require(len(current) == 1 and {k: v for k, v in current[0].items() if k != 'catalog'} ==
                {k: v for k, v in inventory.items() if k != 'catalog'}, 'native producer SDK view differs from current complete input inventory')
    require(type(report['execution_elf_catalog']) is list and 1 <= len(report['execution_elf_catalog']) <= 8192,
            'bounded complete native ELF catalog')
    for value in report['execution_elf_catalog']:
        exact(value, api_pin_fields(), 'native ELF full physical pin')
        require(frame.pin(value['path']) == value, 'native ELF catalog changed')
    ordinary_files = file_catalog(frame, report['execution_file_catalog'])
    gate = frame.pin(frame.input(row['exec_gate']))
    require(gate == report['exec_gate'] and report['exec_gate_source'] == 'tests/ros_javascript_guest/exec_gate.cpp' and
            report['exec_gate_full_dependencies_verified'] is True and report['exec_gate_compile_commands'],
            'actual compiled, source-bound test-only native execution gate required')
    require(row['native_profile'] in ('builtin', 'go-api', 'hg'), 'explicit complete native registry profile')
    selected = [value for value in report['products'] if value['backend'] == backend and value['profile'] == row['native_profile']]
    require(len(selected) == 1, 'one selected native backend product')
    selected = selected[0]
    for role, label in (('peer', row['peer_binary']), ('gateway', row['gateway_binary'])):
        origin = selected[role]
        copied = frame.pin(frame.input(label))
        require(copied['sha256'] == origin['sha256'] and copied['bytes'] == origin['bytes'],
                'explicit actual execution copy differs from original compiled native product')
    require(selected['peer_source'] == 'tests/ros_javascript_guest/native_peer.cpp' and
            selected['definitions'] == {'UWVM_JS_NATIVE_BACKEND': backend} and
            selected['full_dependencies_verified'] is True and selected['compile_commands'],
            'source-bound real portable typed native witness required')
    features, feature_pin = frame.common.document(Path(selected['features']['path']))
    identity, identity_pin = frame.common.document(Path(selected['interface_identity']['path']))
    require(frame.pin(feature_pin['path']) == selected['features'] and
            frame.pin(identity_pin['path']) == selected['interface_identity'] and
            features.get('backend') == backend and features.get('topic_string') is True and
            features.get('unified_events_v1') is True and
            type(features.get('runtime_type_hash')) is bool and identity.get('backend') == backend,
            'actual native capability/interface identity binding required')
    expected_type = 'std_msgs/String' if backend == 1 else 'std_msgs/msg/String'
    require(identity.get('type') == expected_type and type(identity.get('identity')) is str and
            (re.fullmatch('[0-9a-f]{32}', identity['identity']) if backend == 1 else
             re.fullmatch('RIHS01_[0-9a-f]{64}', identity['identity'])), 'actual canonical native String identity required')
    event, event_pin = frame.common.document(frame.input(row['event_receipt']))
    artifact = frame.pin(frame.input(row['event_artifact']))
    require(event.get('schema') == 'uwvm-embedded-js-actual-build-receipt-v1' and
            event.get('status') == 'PASS_BUILD_AND_BINARY_ONLY' and
            event.get('variant') == 'native-event-ros%d' % backend and
            event.get('source_manifest_sha256') == frame.report['source_manifest_sha256'] and event['artifact'] == artifact,
            'actual separately compiled native-event Guest required')
    api.retained(frame, event)
    prepared, preparation_pin = frame.common.document(Path(event['preparation']['path']))
    creator = api.frozen_module(frame.source, frame.frozen, 'tests/ros_javascript_guest/event_manifest.py',
                                'embedded_js_native_event_manifest')
    recipes, _ = frame.common.document(frame.source/'sdk/javascript/guest/recipes.json')
    manifest = creator.selection(recipes, backend)
    require(preparation_pin == event['preparation'] and prepared['manifest'] == manifest and
            prepared['status'] == 'PREPARED_NOT_COMPILED' and Path(prepared['artifact']) == Path(artifact['path']),
            'native-event actual original preparation association differs')
    argv = event['preparation_job']['argv']
    require(type(argv) is list and argv[1:3] == ['-B', str(frame.source/'sdk/javascript/guest/prepare.py')] and
        argv.count('--source-root') == argv.count('--manifest') == 1 and
        argv[argv.index('--source-root')+1] == str(frame.source) and
        argv[argv.index('--manifest')+1] == event['expanded_manifest']['path'],
        'additional event preparation must use this actual frozen source and generated document')
    generated, generated_pin = frame.common.document(Path(event['expanded_manifest']['path']))
    generation_argv = event['manifest_generation_job']['argv']
    require(generated == manifest and generated_pin == event['expanded_manifest'] and
        generation_argv == [generation_argv[0], '-B', str(frame.source/'tests/ros_javascript_guest/event_manifest.py'),
            '--recipes', str(frame.source/'sdk/javascript/guest/recipes.json'), '--backend', str(backend),
            '--out', generated_pin['path']], 'actual additional event manifest generation provenance differs')
    api.retained(frame, prepared)
    api.tool(frame, 'event%d-current-validate' % backend, [str(frame.validator), 'validate', artifact['path']])
    boundary = api.boundary(frame, Path(artifact['path']), manifest)
    return {'row': row, 'backend': backend, 'identity': identity, 'features': features,
            'native_report': pin, 'extra_event': artifact, 'event_boundary': boundary,
            # Native glibc must not select host locale mmap files outside its
            # admitted COPY closure. This changes neither the gate/Worker host
            # environment nor any ROS/Guest capability.
            'environment': {**frame.environment, **base, 'LANG': 'C', 'LC_ALL': 'C'}, 'exec_gate': gate,
            'file_catalog': ordinary_files, 'elf_catalog': report['execution_elf_catalog']}


def policy(frame, selected, mode):
    """Literal per-operation aliases; no event observer borrows source rights."""
    backend, identity = selected['backend'], selected['identity']
    connection = selected['row']['connection']
    expected = {'node-name': 'js_gateway', 'namespace': '/uwvm_js_guest'}
    required = {'node-name', 'namespace', 'master-uri', 'advertise-host', 'transport'} if backend == 1 else {
        'node-name', 'namespace', 'domain-id', 'rmw-implementation', 'security-mode', 'keystore', 'enclave'}
    frame.unitree.require(type(connection) is dict and set(connection) == required and
        all(type(value) is str and value and not any(c.isspace() for c in value) for value in connection.values()) and
        all(connection[key] == value for key, value in expected.items()), 'closed actual native connection required')
    if backend == 1:
        frame.unitree.require(re.fullmatch(r'http://127\.0\.0\.1:[1-9][0-9]{3,4}', connection['master-uri']) and
            connection['advertise-host'] == '127.0.0.1' and connection['transport'] == 'tcpros', 'isolated localhost Master/TCPROS')
    else:
        frame.unitree.require(connection['domain-id'].isdigit() and 1 <= int(connection['domain-id']) <= 232 and
            connection['security-mode'] == 'disabled-test' and connection['keystore'] == connection['enclave'] == 'not-applicable' and
            frame.environment.get('ROS_DOMAIN_ID') == connection['domain-id'] and
            frame.environment.get('RMW_IMPLEMENTATION') == connection['rmw-implementation'] and
            frame.environment.get('ROS_SECURITY_ENABLE', 'false') == 'false' and
            not any(key in frame.environment for key in ('ROS_SECURITY_STRATEGY', 'ROS_SECURITY_KEYSTORE',
                                                         'ROS_SECURITY_ENCLAVE_OVERRIDE')),
            'explicit isolated nonsecure test profile; secure cells are separate')
    qos = {'qos.reliability': 'reliable', 'qos.durability': 'volatile', 'qos.history': 'keep-last', 'qos.depth': '4',
           'qos.deadline': 'infinite', 'qos.lifespan': 'infinite', 'qos.liveliness': 'automatic',
           'qos.liveliness-lease-duration': 'infinite'}
    grants = []
    def topic(alias, operation, resource):
        limits = {'max-message-bytes': '64', 'max-rate-hz': '100', 'burst': '4', 'max-bytes-per-second': '65536',
            'max-queue-bytes': '256', 'overflow-policy': 'reject', 'validator': 'none',
            'audit': 'allow-and-deny', 'audit-max-rate-hz': '100', 'audit-burst': '16',
            'audit-max-records': '10000', 'audit-max-bytes': '1048576',
            'max-publishers' if operation == 'topic.publish' else 'max-subscribers': '1'}
        if backend == 1:
            limits.update({'queue-depth': '4', 'transport': 'tcpros'})
            if operation == 'topic.publish': limits['latch'] = 'false'
        else:
            limits.update(qos); limits['max-inflight'] = '1'
        grants.append({'alias': alias, 'operation': operation, 'resource': '/uwvm_js_guest/'+resource,
            'type': identity['type'], 'identity': identity['identity'], 'limits': limits})
    if mode == 'positive':
        topic('pub', 'topic.publish', 'out'); topic('sub', 'topic.subscribe', 'in')
    elif mode in ('event', 'ownership'):
        topic('source', 'topic.subscribe', 'event')
        grants.append({'alias': 'observer', 'operation': 'event.observe', 'resource': '/uwvm_js_guest/event',
            'type': 'builtin/event-v1', 'identity': 'builtin:ros-event-envelope-le64-v1', 'limits': {
                'event-source-alias': 'source', 'event-kinds': 'topic-message,resource-closed', 'max-event-bytes': '128',
                'max-event-queue-depth': '4', 'max-event-queue-bytes': '512', 'max-inflight': '2',
                'max-rate-hz': '100', 'burst': '4', 'max-bytes-per-second': '65536', 'overflow-policy': 'reject',
                'audit': 'allow-and-deny', 'audit-max-rate-hz': '100', 'audit-burst': '16',
                'audit-max-records': '10000', 'audit-max-bytes': '1048576'}})
    else:
        frame.unitree.require(mode == 'absent', 'closed native case mode')
    return connection, grants


def worker_and_config(api, frame, backend, artifact, connection, grants, socket_path):
    """Run the original JS artifact as main, never as a privileged preload."""
    argv = api.runtime_argv(frame, 'ros%d' % backend, artifact)
    prefix = '--ros%d-single-' % backend
    needle = [prefix+'disable', 'main']
    indices = [i for i in range(len(argv)-1) if argv[i:i+2] == needle]
    frame.unitree.require(len(indices) == 1, 'one explicit main-principal ROS policy')
    argv[indices[0]] = prefix+'enable'
    extra = []
    for key, value in {**connection, 'gateway-socket': str(socket_path), 'gateway-uid': str(os.getuid()),
                       'connect-timeout-ms': '10000', 'audit-deny-max-rate-hz': '100',
                       'audit-deny-burst': '16', 'audit-deny-max-records': '10000', 'audit-deny-max-bytes': '1048576'}.items():
        extra += [prefix+key, 'main', value]
    for grant in grants:
        extra += [prefix+'grant', 'main', *[grant[key] for key in ('alias', 'operation', 'resource', 'type', 'identity')]]
        for key, value in grant['limits'].items():
            extra += [prefix+'set', 'main', grant['alias'], key, value]
    return [*argv[:-2], *extra, *argv[-2:]]


def configuration(backend, pid, connection, grants, socket_path):
    lines = ['backend ros%d' % backend, 'worker-uid %d' % os.getuid(), 'worker-pid %d' % pid,
             'socket '+str(socket_path), 'timeout-ms 10000', 'max-sessions 2',
             'audit-max-records-per-second 100', 'audit-max-records 10000', 'audit-max-bytes 1048576']
    lines += ['connection '+key+' '+value for key, value in connection.items()]
    for grant in grants:
        lines.append('grant '+' '.join(grant[key] for key in ('alias', 'operation', 'resource', 'type', 'identity')))
        lines += ['set '+grant['alias']+' '+key+' '+value for key, value in grant['limits'].items()]
    return '\n'.join(lines)+'\n'


def line(frame, owner, child):
    result = bytearray()
    while not result.endswith(b'\n'):
        result.extend(owner.receive_bytes(child, 1))
        frame.unitree.require(len(result) <= 256, 'bounded exact process gate line')
    return bytes(result)


def gated_worker(api, frame, owner, label, selected, argv):
    """Bind the real kernel-held exec target before replacing the test gate.

    The native test gate and its target FD are independently proven. Its
    reviewed execveat has no pathname fallback and preserves the same owned
    PID/birth used by Gateway SO_PEERCRED. The JS artifact is argv's actual
    --run main module; no cross-module permission ancestry is bypassed.
    """
    require = frame.unitree.require
    child, _ = owner.start(label, [selected['exec_gate']['path'], *argv], json_output=False,
                           environment=frame.environment)
    marker = line(frame, owner, child)
    match = re.fullmatch(rb'JS_EXEC_GATE ([1-9][0-9]*) ([1-9][0-9]*)\n', marker)
    require(match is not None and int(match[1]) == child.pid and 3 <= int(match[2]) <= 1048576,
            'actual test-only exec gate protocol differs')
    proof = api.worker_proof(frame, owner, child, Path(selected['exec_gate']['path']))
    network = network_namespace(frame, child.pid)
    expected = frame.pin(Path(argv[0]))
    entry = next(value for key, value in owner.owner.owned.items() if key[0] == child.pid)
    first = frame.common.identity(child.pid)
    def guard():
        owner.check(); now = frame.common.identity(child.pid)
        require(frame.common.birth(first) == frame.common.birth(now) and
                not frame.common.pidfd_terminal(entry['fd']) and
                Path('/proc/%d/cgroup' % child.pid).read_text() == Path('/proc/self/cgroup').read_text() and
                network_namespace(frame, child.pid) == network, 'owned exec gate birth/cgroup/network changed')
        current = Path('/proc/%d/exe' % child.pid).stat()
        require(all(getattr(current, name) == selected['exec_gate'][key] for name, key in
                    (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                     ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))), 'exec gate changed before G')
        return now
    target_number = int(match[2])
    target_path = '/proc/%d/fd/%d' % (child.pid, target_number)
    descriptor = os.open(target_path, os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and all(getattr(before, name) == expected[key] for name, key in
                    (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                     ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))), 'kernel-held Worker FD differs')
        flags = [value.split(':', 1)[1].strip() for value in Path('/proc/%d/fdinfo/%d' %
            (child.pid, target_number)).read_text().splitlines() if value.startswith('flags:')]
        require(len(flags) == 1 and int(flags[0], 8) & os.O_CLOEXEC, 'actual exec target must close on successful exec')
        digest, size = hashlib.sha256(), 0
        while True:
            guard(); block = os.read(descriptor, min(1024**2, expected['bytes']-size+1))
            if not block: break
            size += len(block); require(size <= expected['bytes'], 'held exec target grew')
            digest.update(block)
        require(size == expected['bytes'] and digest.hexdigest() == expected['sha256'] and
                all(getattr(before, key) == getattr(os.fstat(descriptor), key) for key in
                    ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'full held exec target changed')
        guard()
    finally:
        os.close(descriptor)
    binding = {'method': 'owned-pinned-execveat-target-v1', 'gate_executable': proof,
        'initial_identity': first, 'network_namespace': network, 'kernel_held_fd': target_number,
        'target': expected, 'target_full_hash': digest.hexdigest(), 'target_bytes': size,
        'target_CLOEXEC': True, 'actual_worker_argv': argv, 'gate_stdout': marker.decode(),
        'principal': 'main', 'preloaded_application': False}
    # Reopen the actual still-held kernel FD immediately before G, not merely
    # a pathname that could subsequently select a different executable.
    def release():
        guard()
        current = Path(target_path).stat()
        require(all(getattr(current, name) == expected[key] for name, key in
                    (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                     ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))), 'held target changed at exact exec gate')
        binding['identity_immediately_before_G'] = guard()
        owner.write(child, b'G')
        binding['G_sent'] = True
    return child, binding, release


def gateway_audit(api, frame, row, backend, pid):
    """Actual successful bind follows the production SO_PEERCRED gate.

    This is useful even when lesson08 exits before a post-exec live /proc/exe
    sample can be taken. It is accompanied by the actual held-FD exec chain,
    actual immutable Gateway executable/mappings, Guest bytes and both exits.
    It never substitutes a fabricated credential or raw Gateway RPC client.
    """
    raw = api.bounded(Path(row['stderr']), frame.limits['log_bytes'])
    records = []
    pattern = rb'ros_audit timestamp_ns=([0-9]+) backend=([12]) worker_uid=([0-9]+) worker_pid=([1-9][0-9]*) session=([1-9][0-9]*) operation=([a-z.-]+) alias=([A-Za-z0-9_-]+) error=([0-9]+)'
    for value in raw.splitlines():
        if not value.startswith(b'ros_audit '): continue
        match = re.fullmatch(pattern, value)
        frame.unitree.require(match is not None and int(match[1]) > 0 and int(match[2]) == backend and
            int(match[3]) == os.getuid() and int(match[4]) == pid, 'actual Gateway audit identity differs')
        records.append({'timestamp_ns': int(match[1]), 'backend': int(match[2]), 'worker_uid': int(match[3]),
            'worker_pid': int(match[4]), 'session': int(match[5]), 'operation': match[6].decode(),
            'alias': match[7].decode(), 'error': int(match[8])})
    binds = [value for value in records if value['operation'] == 'session.bind']
    frame.unitree.require(len(binds) == 1 and binds[0]['alias'] == 'target' and binds[0]['error'] == 0 and
        all(value['session'] == binds[0]['session'] and value['error'] in (0, 11) for value in records),
        'one actual SO_PEERCRED-admitted successful manifest bind/session required')
    return {'stderr': frame.pin(row['stderr']), 'successful_manifest_bind': binds[0], 'records': records,
            'credential_evidence': 'immutable production Gateway checks SO_PEERCRED before this actual bind audit'}


def run_case(api, frame, selected, artifacts, variant, mode):
    require = frame.unitree.require
    backend, spec = selected['backend'], selected['row']
    label = 'ros%d-%s' % (backend, variant)
    owner = frame.unitree.Actors(frame)
    peer_command = command(frame, spec['peer'], {'mode': mode})
    peer, _ = owner.start(label+'-peer', peer_command, environment=selected['environment'])
    boot = owner.receive(peer)
    require(set(boot) == {'kind', 'backend', 'pid'} and boot['kind'] == 'boot' and boot['backend'] == backend,
            'actual typed peer boot protocol differs')
    peer_pid = owner_pid(frame, owner, frame.input(spec['peer_binary']))
    require(boot['pid'] == peer_pid, 'native JSON PID must be an actual owned executable birth')
    peer_exe = api.executable_proof(frame, owner, peer_pid, frame.input(spec['peer_binary']))
    network_namespace(frame, peer_pid)
    owner.write(peer, b'I')
    ready = owner.receive(peer)
    require(ready['kind'] == 'ready' and ready['pid'] == peer_pid and ready['backend'] == backend,
            'actual native context readiness differs')
    peer_mapping = mappings(api, frame, owner, peer_pid, frame.input(spec['peer_binary']), spec['peer_mappings'],
                            selected['file_catalog'], selected['elf_catalog'])
    connection, grants = policy(frame, selected, mode)
    artifact = Path(selected['extra_event']['path']) if variant == 'native-event' else artifacts[variant]
    socket_path = frame.out/(label+'.sock')
    require(len(os.fsencode(socket_path)) < 104 and not socket_path.exists(), 'fresh short owned UNIX socket path required')
    if mode == 'absent':
        argv = api.runtime_argv(frame, 'ros%d' % backend, artifact)
    else:
        argv = worker_and_config(api, frame, backend, artifact, connection, grants, socket_path)
    worker, worker_binding, release = gated_worker(api, frame, owner, label+'-worker', selected, argv)
    gateway = None; gateway_proof = None; config_pin = None
    if mode != 'absent':
        config_path = frame.out/(label+'.conf')
        config_path.write_text(configuration(backend, worker.pid, connection, grants, socket_path), encoding='utf-8')
        config_path.chmod(0o600); config_pin = frame.pin(config_path)
        gateway, _ = owner.start(label+'-gateway', command(frame, spec['gateway'], {'config': str(config_path)}),
                                 json_output=False, environment=selected['environment'])
        until = time.monotonic()+20
        while not socket_path.exists():
            owner.check(); require(gateway.poll() is None and time.monotonic() < until, 'actual native Gateway socket readiness')
            time.sleep(.01)
        state = socket_path.lstat()
        require(stat.S_ISSOCK(state.st_mode) and state.st_uid == os.getuid(), 'owned native Gateway socket identity')
        pid = owner_pid(frame, owner, frame.input(spec['gateway_binary']))
        gateway_proof = mappings(api, frame, owner, pid, frame.input(spec['gateway_binary']), spec['gateway_mappings'],
                                 selected['file_catalog'], selected['elf_catalog'])
    release()
    expected_body = b''
    if mode == 'positive':
        require(owner.receive_bytes(worker, len(b'JS_GUEST_ENDPOINTS_CREATED\n')) == b'JS_GUEST_ENDPOINTS_CREATED\n', 'actual JS endpoint creation')
        worker_binding['live_actual_Worker_after_exec'] = api.worker_proof(frame, owner, worker, frame.workers['ros%d' % backend])
        network_namespace(frame, worker.pid)
        gateway_proof['after_actual_ROS_context'] = mappings(api, frame, owner, pid, frame.input(spec['gateway_binary']),
            spec['gateway_context_mappings'], selected['file_catalog'], selected['elf_catalog'])
        matched = owner.receive(peer); require(matched['kind'] == 'matched' and matched['pid'] == peer_pid, 'independent native match')
        owner.write(worker, b'P')
        require(owner.receive_bytes(worker, len(b'JS_GUEST_FINAL_SENT\n')) == b'JS_GUEST_FINAL_SENT\n', 'actual JS final-send barrier')
        delivered = owner.receive(peer); require(delivered['kind'] == 'delivered' and delivered['pid'] == peer_pid, 'independent native exact reception/reply')
        retries = advance(frame, owner, worker, b'PASS_JS_GUEST_DELIVERY\n', b'JS_GUEST_POLL_AGAIN\n')
        expected_body = b'JS_GUEST_ENDPOINTS_CREATED\nJS_GUEST_FINAL_SENT\n'+b'JS_GUEST_POLL_AGAIN\n'*retries+b'PASS_JS_GUEST_DELIVERY\n'
    elif mode == 'event':
        require(owner.receive_bytes(worker, len(b'JS_NATIVE_EVENTS_READY\n')) == b'JS_NATIVE_EVENTS_READY\n', 'actual event source/observer readiness')
        worker_binding['live_actual_Worker_after_exec'] = api.worker_proof(frame, owner, worker, frame.workers['ros%d' % backend])
        network_namespace(frame, worker.pid)
        gateway_proof['after_actual_ROS_context'] = mappings(api, frame, owner, pid, frame.input(spec['gateway_binary']),
            spec['gateway_context_mappings'], selected['file_catalog'], selected['elf_catalog'])
        matched = owner.receive(peer); require(matched['kind'] == 'matched' and matched['pid'] == peer_pid, 'independent native event subscriber match')
        owner.write(peer, b'T')
        sent = owner.receive(peer); require(sent['kind'] == 'event-sent' and sent['pid'] == peer_pid, 'sole actual native publication witness')
        owner.write(worker, b'P')
        first_retries = advance(frame, owner, worker, b'JS_NATIVE_EVENTS_DATA\n', b'JS_NATIVE_EVENTS_AGAIN\n')
        second_retries = advance(frame, owner, worker, b'PASS_JS_NATIVE_EVENTS\n', b'JS_NATIVE_EVENTS_AGAIN\n')
        expected_body = b'JS_NATIVE_EVENTS_READY\n'+b'JS_NATIVE_EVENTS_AGAIN\n'*first_retries+b'JS_NATIVE_EVENTS_DATA\n'+b'JS_NATIVE_EVENTS_AGAIN\n'*second_retries+b'PASS_JS_NATIVE_EVENTS\n'
    worker_row = api.finish_actor(frame, owner, worker, frame.acceptance[variant]['exit'] if mode == 'absent' else 0, serial=False)
    raw = Path(worker_row['stdout']).read_bytes()
    expected = worker_binding['gate_stdout'].encode()
    if mode in ('absent', 'ownership'):
        plan = frame.acceptance[variant]['stdout']
        expected_body = (plan['text'] if plan['kind'] == 'exact' else plan['final']).encode()
    require(raw == expected+expected_body and not Path(worker_row['stderr']).read_bytes(),
            'exact native-observed Guest output/empty stderr differs')
    owner.write(peer, b'Q')
    complete = owner.receive(peer)
    require(complete['kind'] == 'complete' and complete['pid'] == peer_pid and complete['backend'] == backend and
            complete['excess'] == 0 and 30 <= complete['quiet_samples'] <= 10000 and
            2_000_000_000 <= complete['quiet_ns'] <= 10_000_000_000 and
            complete['final'] == complete['reply'] == (1 if mode == 'positive' else 0) and
            complete['event'] == (1 if mode == 'event' else 0) and complete['warmup'] in ((0, 1) if mode == 'positive' else (0,)),
            'independent exact native effects/quiet witness differs')
    peer_row = api.finish_actor(frame, owner, peer, serial=False)
    phases = [json.loads(value)['kind'] for value in Path(peer_row['stdout']).read_bytes().splitlines()]
    require(phases == ['boot', 'ready', *(['matched', 'delivered'] if mode == 'positive' else
            ['matched', 'event-sent'] if mode == 'event' else []), 'complete'], 'exact independent native phase inventory differs')
    gateway_credentials = None
    if gateway is not None:
        until = time.monotonic()+10
        while gateway.poll() is None and time.monotonic() < until:
            owner.check(); time.sleep(.01)
        require(gateway.poll() == 0, 'native Gateway must retire normally with its actual Worker principal')
        gateway_row = api.finish_actor(frame, owner, gateway, serial=False)
        require(not Path(gateway_row['stdout']).read_bytes(), 'unexpected native Gateway stdout')
        gateway_credentials = gateway_audit(api, frame, gateway_row, backend, worker.pid)
    api.terminal(frame, owner)
    frame.report['cases'].append({'case': label, 'variant': variant, 'mode': mode, 'status': 'PASS_ACTUAL_NATIVE_RUNTIME',
        'worker': worker_binding, 'peer_executable': peer_exe, 'peer_mapping': peer_mapping,
        'gateway_mapping': gateway_proof, 'gateway_config': config_pin, 'native_effects': complete,
        'gateway_credential_binding': gateway_credentials, 'main_principal': True,
        'guest_stdout': frame.pin(worker_row['stdout']), 'guest_artifact': frame.pin(artifact),
        'scope': 'topic payload + actual guest JS callback' if mode == 'positive' else
                 'native topic unified event + resource_closed' if mode == 'event' else
                 'ownership/resource_closed only' if mode == 'ownership' else 'actual default refusal + independent native quiet witness'})
    frame.save()


def advance(frame, owner, child, success, again):
    """Supply one R only after the preceding actual poll completed."""
    for attempts in range(200):
        owner.write(child, b'R')
        value = bytearray()
        while not value.endswith(b'\n'):
            value.extend(owner.receive_bytes(child, 1))
            frame.unitree.require(len(value) <= 256, 'bounded exact guest poll marker')
        if bytes(value) == success:
            return attempts
        frame.unitree.require(bytes(value) == again, 'actual Guest poll protocol differs')
        time.sleep(.01)
    raise TimeoutError('finite200 actual guest poll attempts exhausted')


def master(api, frame, selected):
    """Own a real isolated ROS1 Master; never adopt an existing daemon.

    The XML-RPC PID is matched to an already observed owned birth and the
    selected real Python executable. Source/package provenance comes from the
    fully inventoried native SDK. The external producer chooses the launch
    argv and its whole package; this function does not install or search PATH.
    """
    from urllib.parse import urlsplit
    import socket
    spec = selected['row']['master']
    frame.unitree.exact(spec, ('job', 'binary', 'script_input', 'uri'), 'owned native ROS1 Master selection')
    uri = urlsplit(spec['uri'])
    frame.unitree.require(uri.scheme == 'http' and uri.hostname == '127.0.0.1' and uri.username is None and
        uri.password is None and uri.path in ('', '/') and not uri.query and not uri.fragment and
        type(uri.port) is int and 1024 <= uri.port <= 65535 and
        selected['row']['connection']['master-uri'].rstrip('/') == spec['uri'].rstrip('/') and
        frame.environment.get('ROS_MASTER_URI', '').rstrip('/') == spec['uri'].rstrip('/'),
        'explicit isolated localhost Master endpoint/environment required')
    try:
        with socket.create_connection(('127.0.0.1', uri.port), timeout=.2):
            raise ValueError('reserved Master port is already occupied; existing service is not owned')
    except ConnectionRefusedError:
        pass
    frame.pin(frame.input(spec['script_input']))
    owner = frame.unitree.Actors(frame)
    child, _ = owner.start('ros1-private-master', command(frame, spec['job']), json_output=False)
    class Transport(xmlrpc.client.Transport):
        def make_connection(self, host):
            return http.client.HTTPConnection(host, timeout=.5)
    until = time.monotonic()+30
    while time.monotonic() < until:
        owner.check()
        frame.unitree.require(child.poll() is None, 'owned Master launcher exited before readiness')
        try:
            with xmlrpc.client.ServerProxy(spec['uri'], transport=Transport()) as client:
                identifier = client.getPid('/uwvm_js_guest/runtime_owner')
                response_uri = client.getUri('/uwvm_js_guest/runtime_owner')
        except (OSError, xmlrpc.client.Error):
            time.sleep(.02)
            continue
        frame.unitree.require(type(identifier) in (list, tuple) and len(identifier) == 3 and
            identifier[0] == 1 and type(identifier[2]) is int and identifier[2] > 0 and
            response_uri[0] == 1 and response_uri[2].rstrip('/') == spec['uri'].rstrip('/'),
            'actual owned Master XML-RPC readiness differs')
        proof = api.executable_proof(frame, owner, identifier[2], frame.input(spec['binary']))
        network_namespace(frame, identifier[2])
        frame.report['owned_master'] = {'uri': response_uri, 'PID_response': identifier,
            'executable': proof, 'script': frame.pin(frame.input(spec['script_input']))}
        return owner
    raise TimeoutError('owned isolated Master readiness exceeded')


def local_coverage(api, frame, selections, artifacts):
    """Combine original scopes only after reopening the actual local26 run."""
    labels = {row['local_report'] for row in selections}
    frame.unitree.require(len(labels) == 1, 'one explicit prior local Runtime report selection')
    label = next(iter(labels))
    if label is None:
        return False
    local, pin = document(api, frame, frame.input(label))
    expected = set(artifacts)-api.NATIVE_IDS
    frame.unitree.require(local.get('source') == str(frame.source) and
        local.get('source_manifest_sha256') == frame.report['source_manifest_sha256'] and
        local.get('status') == 'PASS_EMBEDDED_JS_LOCAL_RUNTIME_ONLY' and
        local.get('actual_Runtime_execution') is True and local.get('actual_native_ROS_execution') is False and
        local.get('all30_Runtime_qualified') is False and
        set(local.get('actual_Runtime_variants', [])) == expected and len(expected) == 26 and
        local.get('actual_Runtime_worker_profiles') == ['none', 'ros1', 'ros2', 'dual'] and
        local.get('build_provenance') == frame.report['build_provenance'] and
        all(local.get(key) is True for key in ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved')) and
        type(local.get('cleanup')) is list and local['cleanup'] and
        all(value.get('status') == 'PASS' and not value.get('remaining') for value in local['cleanup']) and
        not local.get('finalization_errors'), 'complete same-freeze actual local26 Runtime run required')
    commands = set(artifacts)-set(api.PURE.values())-api.NATIVE_IDS
    wanted_cases = {'pure-lifecycle', '00-raw-ros1-dual', '00-raw-ros2-dual'} | {
        variant+'-ros%d' % frame.acceptance[variant]['backend'] for variant in commands}
    frame.unitree.require(len(local['cases']) == 25 and {row['case'] for row in local['cases']} == wanted_cases and
        all(row['status'] == 'PASS_ACTUAL_RUNTIME' for row in local['cases']) and
        {variant for row in local['cases'] for variant in row['variants']} == expected,
        'exact original25 local cases covering26 distinct variants required')
    api.retained(frame, local)
    frame.report['actual_local26_Runtime_report'] = pin
    return True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'manifest', 'config', 'out'):
        parser.add_argument('--'+name, type=Path, required=True)
    for name in ('manifest-sha256', 'config-sha256'):
        parser.add_argument('--'+name, required=True)
    args = parser.parse_args(); api = load_runtime(args); frame = None
    try:
        frame = api.new_frame(args, 'native.py'); frame.native_aliases = {}
        frame.unitree.require(frame.limits['rss_bytes'] <= 1024**3 and frame.limits['command_seconds'] <= 600,
            'native JS actor group retains explicit1GiB/600s ceiling')
        frame.unitree.require(sorted(path.name for path in Path('/sys/class/net').iterdir()) == ['lo'] and
            int(Path('/sys/class/net/lo/flags').read_text(), 16) & 1, 'actual isolated loopback-only network namespace required')
        artifacts = api.products(frame)
        selections = frame.config['benchmark']['native_cases']
        frame.unitree.require(selections and len({row['backend'] for row in selections}) == len(selections), 'distinct actual native backend selections')
        local_pass = local_coverage(api, frame, selections, artifacts)
        for row in selections:
            selected = qualify(api, frame, row)
            backend = selected['backend']
            master_owner = master(api, frame, selected) if backend == 1 else None
            frame.unitree.require(backend != 2 or row['master'] is None, 'ROS2 has no ROS1 Master')
            run_case(api, frame, selected, artifacts, '02-node-ros%d' % backend, 'positive')
            run_case(api, frame, selected, artifacts, '08-unified-events-ros%d' % backend, 'ownership')
            run_case(api, frame, selected, artifacts, 'native-event', 'event')
            denied = ['00-raw-ros%d' % backend, '07-complete-denied-abi-ros%d' % backend, '09-boundary-refusals-ros%d' % backend]
            if backend == 2: denied += ['limit-heap', 'limit-jobs', 'limit-interrupt']
            for variant in denied:
                run_case(api, frame, selected, artifacts, variant, 'absent')
            if master_owner is not None:
                result = master_owner.close()
                frame.unitree.require(result['status'] == 'PASS' and not result['remaining'], 'owned Master cleanup incomplete')
                frame.actors.remove(master_owner)
                frame.report['owned_master']['cleanup'] = result
        native_variants = {row['variant'] for row in frame.report['cases'] if row['mode'] in ('positive', 'ownership')}
        complete = local_pass and native_variants == api.NATIVE_IDS and {row['backend'] for row in selections} == {1, 2}
        frame.report.update(actual_Runtime_execution=True, actual_native_ROS_execution=True,
            all30_Runtime_qualified=complete, backend_scope=[row['backend'] for row in selections],
            actual_original_native_Runtime_variants=sorted(native_variants),
            additional_native_event_variants=['native-event-ros%d' % row['backend'] for row in selections],
            runtime_type_hash_claim='Only the actual native producer feature/identity receipts; peer does not infer RIHS',
            native_zero_effect_witness_qualified=True)
        return frame.finish('PASS_EMBEDDED_JS_ALL30_RUNTIME_AND_NATIVE_ROS' if complete else 'PASS_EMBEDDED_JS_NATIVE_ROS_SUBSET_ONLY')
    except BaseException as error:
        if frame is not None: return frame.finish('FAIL', repr(error))
        raise


if __name__ == '__main__':
    raise SystemExit(main())
