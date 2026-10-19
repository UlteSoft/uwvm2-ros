"""Execute explicit official-case ports without accepting a custom PASS label.

An API port is trusted, reviewed executable SOURCE, never a claimed receipt.
It must belong to the same complete Runtime freeze. This owner provides actual
pidfd actors, physical executable/map_files admission, raw pipe observations,
Gateway credentials, disabled-target audit and terminal cleanup. An absent API
port remains a missing port. A native wrapper cannot stand in for its children.

No actor, project import, test or I/O runs when this module is imported. Only
``run`` is an execution entry. Root's already admitted loopback/cgroup guard is
mandatory and is kept on every actor operation and streamed mapping check.
"""
from __future__ import annotations
import errno
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import stat
import types
import sys
import time

PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
FACTS = ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns')
ENV = {'PATH','LD_LIBRARY_PATH','PYTHONPATH','PYTHONHOME','CMAKE_PREFIX_PATH','AMENT_PREFIX_PATH','ROS_PACKAGE_PATH',
       'PKG_CONFIG_PATH','ROS_DISTRO','ROS_VERSION','ROS_PYTHON_VERSION','RMW_IMPLEMENTATION',
       'ROS_DOMAIN_ID','ROS_MASTER_URI','ROS_IP','ROS_LOCALHOST_ONLY'}
SEMANTICS = {'backend', 'distro', 'architecture', 'rmw', 'features', 'permutation'}
TOKEN = re.compile(r'[a-z][a-z0-9_.-]{0,79}')
MIB = 1024**2


def need(value, reason):
    if not value: raise ValueError(reason)


def key(value):
    need(type(value) is dict and set(value) == {'suite', 'universe', 'id'} and
         all(type(item) is str and item for item in value.values()), 'exact actual discovered official-case key required')
    return tuple(value[name] for name in ('suite', 'universe', 'id'))


def discovered(suites):
    """Keep independent native universes; never sum nested/wrapper test sets."""
    rows = []
    for suite in suites:
        for universe in suite['universes']:
            for row in universe['cases']:
                rows.append({'key': {'suite': suite['id'], 'universe': universe['universe'], 'id': row['id']},
                    'native_status': row['status'], 'native_observation': row})
        for native_file in suite.get('native_junit_files', []):
            universe = 'junit-file:' + native_file['input']['sha256']
            for row in native_file['observation']['cases']:
                identity = json.dumps([row['classname'], row['name'], row['ordinal']], separators=(',', ':'), ensure_ascii=True)
                rows.append({'key': {'suite': suite['id'], 'universe': universe, 'id': identity},
                    'native_status': row['status'], 'native_observation': row,
                    'native_result_file': native_file['input']})
    need(len(rows) <= 400000 and len({key(row['key']) for row in rows}) == len(rows),
         'bounded distinct actual official test identities required')
    return rows


def authority(arguments, backend, mode, grants):
    """Parse EVERY ROS authority operand using the real global/single CLI.

    Global targets stay disabled even in a positive case; only exact main is
    enabled. Global/group grants and enabled preload principals are refused.
    Non-ROS runtime argv remains explicit source-reviewed adapter input.
    """
    need(type(arguments) is list and type(backend) is int and backend in (1,2) and
         mode in ('positive','default-denied') and type(grants) is list and
         all(type(v) is str and '\0' not in v and '\n' not in v for v in arguments), 'closed actual ROS authority argv required')
    records, seen, actual, created, toggles = [], set(), [], set(), {}
    connection = {'gateway-socket','gateway-uid','audit-deny-max-rate-hz','audit-deny-burst',
        'audit-deny-max-records','audit-deny-max-bytes','connect-timeout-ms','node-name','namespace',
        'master-uri','advertise-host','transport','domain-id','rmw-implementation','security-mode','keystore','enclave'}
    index = 0
    while index < len(arguments):
        token = arguments[index]
        match = re.fullmatch(r'--ros([12])-(.+)', token)
        if not match:
            need(not token.startswith(('--ros1','--ros2')), 'unknown ROS authority option spelling')
            index += 1; continue
        version, action = int(match[1]), match[2]
        need(action == 'global-disable' or action in ('single-create','single-enable','single-disable','single-grant','single-set') or
             action.startswith('single-') and action[7:] in connection, 'global/group/unknown ROS rights are not admitted')
        arity = 0 if action == 'global-disable' else 6 if action == 'single-grant' else 4 if action == 'single-set' else 1 if action in ('single-create','single-enable','single-disable') else 2
        values = arguments[index+1:index+1+arity]
        need(len(values) == arity, 'truncated actual ROS authority operand')
        if action != 'global-disable':
            module = values[0]
            need(re.fullmatch(r'[A-Za-z0-9_.-]{1,96}',module), 'explicit single ROS principal required')
            if action == 'single-create': created.add((version,module))
            if action in ('single-enable','single-disable'):
                need((version,module) not in toggles, 'duplicate or contradictory ROS principal toggle')
                toggles[(version,module)] = action
            if action == 'single-enable':
                need(version == backend and mode == 'positive' and module == 'main', 'only the selected positive main may be enabled')
            if action not in ('single-create','single-disable'):
                need(version == backend and module == 'main', 'extra backend or preload authority is not admitted')
            if action == 'single-grant':
                need(mode == 'positive' and all(values[1:]) and '*' not in ''.join(values), 'only exact positive main grants are admitted')
                actual.append(values[1:])
            if action == 'single-set':
                need(mode == 'positive' and any(row[0] == values[1] for row in grants), 'limit setter names no selected positive alias')
            if action.startswith('single-') and action[7:] in connection and mode == 'default-denied':
                need(action[7:].startswith('audit-deny-'), 'disabled main may only add denial-audit budgets')
        identity = (version, action if action not in ('single-enable','single-disable') else 'single-toggle',
            tuple(values[:2] if action == 'single-set' else values[:1]))
        if action == 'single-set': identity += (values[2],)
        if action != 'single-grant':
            need(identity not in seen, 'duplicate actual ROS configuration option'); seen.add(identity)
        records.append({'backend':version,'action':action,'operands':values}); index += arity+1
    need(any(row['backend'] == backend and row['action'] == 'global-disable' for row in records) and
         (backend,'main') in created and toggles.get((backend,'main')) ==
         ('single-enable' if mode == 'positive' else 'single-disable'), 'actual selected global denial/exact main policy required')
    need(actual == grants and (bool(actual) if mode == 'positive' else not actual) and
         len({row[0] for row in actual}) == len(actual), 'all actual ROS grants must equal the declared exact main aliases')
    for principal in created:
        need(principal in toggles, 'every explicitly created ROS principal must have one explicit toggle')
    return records


def catalog_file(catalog,row):
    """Return ONLY a catalog's actual physical file pin, never a CHROOT alias.

    The literal helper stores full-seven in row.pin, unlike common.catalog's
    flat file fields. No directory/symlink row from that schema qualifies a
    host target. Every regular pin must retain its exact root-relative path.
    """
    relative=row.get('relative')
    need(type(relative) is str and relative and not Path(relative).is_absolute() and
         '..' not in relative.split('/') and '\0' not in relative,'actual safe catalog-relative name required')
    expected=str(Path(catalog['root'])/relative)
    if catalog.get('catalog_format')=='literal-chroot-tree-v1':
        need(row.get('kind') in ('file','directory','symlink') and 'target_file' not in row and 'resolved' not in row,
             'literal CHROOT catalog cannot supply host resolution authority')
        if row['kind']!='file':return None
        selected=row.get('pin')
        need(type(selected) is dict and set(selected)==PIN and selected['path']==expected and
             all(selected[name]==row[name] for name in ('bytes','sha256')),
             'actual ordinary literal-catalog file pin/path differs')
        return selected
    need('catalog_format' not in catalog,'unknown physical catalog schema')
    return dict(path=expected,**{name:row[name] for name in PIN-{'path'}}) if row['kind']=='file' else row.get('target_file')


def retired_case_socket(out,path,error):
    """Recognize ONLY normal unlink of this owner's exact positive RPC leaf.

    This is advisory output-size accounting, not input/root/mapping admission.
    Every still-extant entry is counted normally. No permission, catalog row,
    product pin, arbitrary disappeared file or different errno is exempted.
    """
    if not isinstance(error,FileNotFoundError) or error.errno!=errno.ENOENT:return False
    try:parts=Path(path).relative_to(Path(out)).parts
    except ValueError:return False
    return len(parts)==2 and re.fullmatch(r'case-[0-9]{6}-positive',parts[0]) is not None and parts[1]=='rpc'


def run(common, unitree, *, source, frozen, manifest_current, profile, source_lock, suite_specs, suites, mapping,
        roots, catalogs, snapshot_roots, out, limits, environment, guard, pin, document, product, module, save):
    """Discover -> exact mapping -> bounded owned execution, serially.

    ``mapping`` is an actual opened current document, not an opaque prior PASS.
    An adapter has ADAPTER_ABI='official-api-port-v1', a finite literal
    SUPPORTED_OFFICIAL_CASES list, verify_prerequisites(scope, documents),
    execute(scope), and normalize(role, raw_bytes, configuration). execute()
    returns None. Only observations captured by this scope can become evidence.
    verify_prerequisites replays each real SDK/Worker producer's own proof; no
    universal PASS-string admission is provided. Such per-API ports are separate
    implementation prerequisites and NONE is fabricated by this module.
    """
    guard(); rows = discovered(suites)
    result = {'kind': 'owned-per-official-case-Wasm-Gateway-differential', 'cases': [{**row, 'status': 'UNMAPPED_API_ADAPTER_MISSING', 'executions': []} for row in rows],
        'all_official_tests_qualified': False, 'scope': 'Separate native discovery universes; no summed coverage',
        'actual_Wasm_cases_executed': 0, 'coverage_gaps': []}
    def persist():
        common.atomic_json(out / 'official-differential.json', result); save()
    persist()
    if mapping is None:
        result['coverage_gaps'].append('NO_SELECTED_FROZEN_PER_OFFICIAL_API_ADAPTERS')
        persist(); product(out / 'official-differential.json'); return result
    need(set(mapping) == {'format_version', 'kind', 'adapters', 'cases', 'coverage_gaps'} and
         mapping['format_version'] == 1 and mapping['kind'] == 'official-case-Wasm-Gateway-mapping-v1' and
         type(mapping['adapters']) is list and len(mapping['adapters']) <= 512 and
         type(mapping['cases']) is list and len(mapping['cases']) <= 400000 and
         type(mapping['coverage_gaps']) is list, 'closed finite official mapping selection required')
    adapters, selected = {}, {}
    for row in mapping['adapters']:
        need(set(row) == {'id', 'entry'} and TOKEN.fullmatch(row['id']) and row['id'] not in adapters,
             'distinct frozen API adapter role required')
        selected_entry = pin(row['entry']); path = Path(selected_entry['path'])
        need(path.is_relative_to(source) and path.suffix == '.py', 'API adapter must be actual same-frozen project source')
        relative = str(path.relative_to(source)); need(frozen.get(relative) ==
            {name: selected_entry[name] for name in ('bytes', 'sha256')}, 'API adapter differs from complete current MF')
        adapter = module(relative, 'official_port_' + row['id'])
        need(adapter.ADAPTER_ABI in ('official-api-port-v1','official-api-port-v2') and type(adapter.SUPPORTED_OFFICIAL_CASES) is list and
             len(adapter.SUPPORTED_OFFICIAL_CASES) <= 400000 and all(callable(getattr(adapter, name, None)) for name in
             ('verify_prerequisites', 'execute', 'normalize')), 'implemented source-bound official API port required')
        supported = {key(value) for value in adapter.SUPPORTED_OFFICIAL_CASES}
        need(len(supported) == len(adapter.SUPPORTED_OFFICIAL_CASES), 'API port support declarations must be distinct')
        if adapter.ADAPTER_ABI == 'official-api-port-v2':
            need(callable(getattr(adapter,'supports_discovered_case',None)), 'v2 requires an actual source-locked case matcher')
        adapters[row['id']] = (adapter, supported, selected_entry)
    known = {key(row['key']) for row in rows}
    for row in mapping['cases']:
        need(set(row) == {'native_case', 'adapter', 'positive', 'default_denied'} and row['adapter'] in adapters,
             'one explicit positive+default-denied port selection required')
        identity = key(row['native_case'])
        api, supported, _ = adapters[row['adapter']]
        actual_row = next((value for value in rows if key(value['key']) == identity), None)
        spec = next((value for value in suite_specs if value['id'] == identity[0]), None)
        matched = identity in supported or (api.ADAPTER_ABI == 'official-api-port-v2' and actual_row is not None and
            api.supports_discovered_case(actual_row,spec,source_lock) is True)
        need(identity in known and identity not in selected and matched,
             'mapping must name one actually discovered case supported by that exact frozen port')
        selected[identity] = row
    result['coverage_gaps'].extend(mapping['coverage_gaps'])
    low = module('tests/ros_aarch64/runtime.py', 'official_case_opened_mapping_observer')
    # The entire actual physical provider catalogs, not display pathnames,
    # constrain EVERY inode-backed mapped ELF/file. A selected required DSO
    # alone does not qualify unrelated mappings or a substituted host provider.
    allowed_files = {}
    for catalog in catalogs:
        expected_catalog = catalog['catalog']; pin(expected_catalog, 512 * MIB)
        with Path(expected_catalog['path']).open('rb') as stream:
            for raw in stream:
                guard(); need(len(raw) <= MIB, 'bounded actual catalog row required')
                value = unitree.decode(raw)
                selected_file = catalog_file(catalog,value)
                if selected_file is not None:
                    identity = tuple(selected_file[name] for name in FACTS)
                    need(identity not in allowed_files or allowed_files[identity]['sha256'] == selected_file['sha256'],
                         'one physical provider identity has inconsistent complete catalog bytes')
                    allowed_files[identity] = selected_file
        need(common.opened(Path(expected_catalog['path']), 512 * MIB) == expected_catalog, 'complete provider catalog changed')
    specs = {row['id']: row for row in suite_specs}
    for ordinal, (native_row, receipt) in enumerate(zip(rows, result['cases'])):
        guard(); identity = key(native_row['key']); chosen = selected.get(identity)
        if chosen is None: continue
        receipt['adapter'] = adapters[chosen['adapter']][2]
        if native_row['native_status'] != 'PASS':
            receipt['status'] = 'NATIVE_SKIPPED_DISABLED_FAILED_OR_UNEXECUTED_GAP'; continue
        adapter = adapters[chosen['adapter']][0]
        configs = [document(chosen[name]) for name in ('positive', 'default_denied')]
        try:
            # The two fresh executions differ in authority, never in the actual
            # Guest/program/source/SDK/official fixture selected for this case.
            same = {'wasm', 'sdk_evidence', 'worker_evidence', 'fixture_inputs', 'source_inputs', 'semantics', 'official_case'}
            need(all(configs[0][name] == configs[1][name] for name in same), 'positive/denied source SDK and Guest association differs')
            for mode, config in zip(('positive', 'default-denied'), configs):
                case_out = out / ('case-%06d-%s' % (ordinal, mode)); case_out.mkdir(mode=0o700)
                scope = Scope(common, unitree, low, adapter, config, native_row, mode, source, frozen, manifest_current, profile,
                    specs[native_row['key']['suite']], snapshot_roots, roots, allowed_files, case_out, limits, environment, guard,
                    pin, document, product, persist)
                scope.source_lock = source_lock
                scope.load_source = module
                receipt['executions'].append(scope.report)
                try:
                    scope.admit()
                    need(adapter.execute(scope) is None, 'an API port may not substitute its own PASS receipt for owned evidence')
                    scope.finish()
                finally:
                    try: cleanup = scope.close()
                    except BaseException as error:
                        cleanup = {'status':'CLEANUP_INCOMPLETE','error':repr(error),
                            'remaining':'UNPROVED_AFTER_CLOSE_EXCEPTION'}
                        scope.report.update(status='FAIL',cleanup=cleanup)
                    # Register the failed cleanup BEFORE optional publication;
                    # an empty/all-prior-PASS list cannot hide a thrown close.
                    receipt.setdefault('cleanup', []).append(cleanup)
                    if scope.released: result['actual_Wasm_cases_executed'] += 1
                    need(cleanup['status'] == 'PASS', 'actual official-case actor cleanup incomplete')
                    receipt.setdefault('execution_receipts', []).append(product(scope.out / 'report.json'))
            receipt['status'] = 'PASS_ACTUAL_MAPPED_OFFICIAL_CASE_POSITIVE_AND_DEFAULT_DENIED'
        except Exception as error:
            receipt.update(status='FAIL_MAPPED_OFFICIAL_CASE', error=repr(error))
            need(all(value['status'] == 'PASS' for value in receipt.get('cleanup', [])),
                 'incomplete owned cleanup prevents further official-case execution')
            guard()
        finally: persist()
    result['unmapped_or_unqualified_cases'] = [row['key'] for row in result['cases']
        if row['status'] != 'PASS_ACTUAL_MAPPED_OFFICIAL_CASE_POSITIVE_AND_DEFAULT_DENIED']
    # Even complete selected mappings cannot close absent packages/workflows,
    # unsupported features/hardware or undiscovered source tests by themselves.
    result['selected_native_universe_mapping_complete'] = bool(rows) and not result['unmapped_or_unqualified_cases'] and not result['coverage_gaps']
    result['all_official_tests_qualified'] = False
    persist(); product(out / 'official-differential.json'); return result


class Scope:
    """Actual per-case owner; API adapters may only claim observed token data.

    setup_write is a reviewed port's cooperative bootstrap/control protocol,
    recorded separately. release() requires mapped real Worker/native/Gateway
    births and activates effect-phase write/read. Ports must use a held ready
    boundary before release; a source reviewer must qualify this API-specific
    protocol. No generic receipt can assert such a boundary on its behalf.
    """
    def __init__(self, common, unitree, low, adapter, config, native_row, mode, source, frozen, manifest_current,
                 profile, spec, snapshot_roots, roots, allowed_files, out, limits, environment, guard, pin, document, product, save):
        self.common, self.unitree, self.low, self.adapter = common, unitree, low, adapter
        self.config, self.native_row, self.mode, self.source, self.frozen = config, native_row, mode, source, frozen
        self.profile, self.spec, self.roots, self.out, self.limits, self.environment = profile, spec, roots, out, limits, environment
        self.manifest_current, self.snapshot_roots, self.allowed_files = manifest_current, snapshot_roots, allowed_files
        self.guard, self.admit_pin, self.document, self.product, self.parent_save = guard, pin, document, product, save
        self.actors = []; self._next_actor_ledger = 0; self.owner = None; self.children = {}; self.proofs = {}
        self.observations = []; self.comparisons = []; self.quiet_proofs = []; self.released = False
        self.report = {'commands': [], 'setup_transfers': [], 'status': 'RUNNING'}; self.closed = False

    def pin(self, path):
        path = Path(path); value = self.common.opened(path)
        if path.is_relative_to(self.out): return self.product(path, 2 * 1024**3)
        need(any(path == root or path.is_relative_to(root) for root in self.roots) or path.is_relative_to(self.source),
             'official-case input outside selected complete source/provider roots')
        return self.admit_pin(value)

    def save(self):
        self.common.atomic_json(self.out / 'report.json', self.report); self.parent_save()

    def admit(self):
        c = self.config
        need(set(c) == {'format_version', 'kind', 'official_case', 'semantics', 'mode', 'wasm', 'sdk_evidence',
             'worker_evidence', 'fixture_inputs', 'source_inputs', 'roles', 'rights'} and c['format_version'] == 1 and
             c['kind'] == 'actual-official-api-port-execution-v1' and c['official_case'] == self.native_row['key'] and
             c['mode'] == self.mode, 'closed actual official-case execution configuration required')
        semantics = c['semantics']; expected = {'backend': self.profile['backend'], 'distro': self.profile['distro'],
            'architecture': self.profile['target']['architecture'], 'rmw': self.profile['runtime']['rmw'],
            'features': self.profile['features']}
        need(set(semantics) == SEMANTICS and all(semantics[name] == value for name, value in expected.items()) and
             semantics['permutation'] in self.spec['requested_permutations'], 'actual official distro/arch/RMW/feature permutation differs')
        need(c['sdk_evidence'] and c['worker_evidence'] and c['source_inputs'] and c['fixture_inputs'] and
             all(type(c[name]) is list and len(c[name]) <= 256 for name in
             ('sdk_evidence', 'worker_evidence', 'source_inputs', 'fixture_inputs')), 'actual SDK/Worker/source/official fixture evidence mandatory')
        documents = {name: [self.document(self.admit_pin(value)) for value in c[name]]
                     for name in ('sdk_evidence', 'worker_evidence')}
        for name in ('source_inputs', 'fixture_inputs'):
            for value in c[name]:
                selected_input = self.admit_pin(value); self.pin(selected_input['path'])
                if name == 'fixture_inputs':
                    need(Path(selected_input['path']).is_relative_to(self.snapshot_roots[self.spec['snapshot']]),
                         'official oracle fixture must belong to the exact independently verified original Git snapshot')
        wasm = self.admit_pin(c['wasm']); need(any(Path(wasm['path']).is_relative_to(root) for root in self.roots), 'actual Wasm product lacks preserved root')
        with Path(wasm['path']).open('rb') as stream: header = stream.read(8)
        need(header == b'\0asm\1\0\0\0' and self.common.opened(Path(wasm['path'])) == wasm, 'real selected version1 Wasm required')
        # The port's prerequisite verifier has its own real producer schemas.
        # Its return value is deliberately unused: no boolean/status qualifies
        # SDK authenticity or Worker compilation merely by being returned here.
        need(self.adapter.verify_prerequisites(self, documents) is None,
             'API prerequisite verifier must perform actual source-bound admission, not return a PASS claim')
        roles = c['roles']; required = {'worker', 'native_peer'} | ({'gateway'} if self.mode == 'positive' else set())
        need(type(roles) is dict and required <= set(roles) and set(roles) <= required | {'master'},
             'exact actual Worker/independent native peer and positive Gateway roles required')
        for role, selected in roles.items():
            need(set(selected) == {'host', 'image', 'loader', 'mapped', 'launcher', 'arguments', 'protocol', 'environment'} and
                 selected['protocol'] in ('line', 'binary') and type(selected['arguments']) is list and len(selected['arguments']) <= 128 and
                 type(selected['mapped']) is dict and 0 < len(selected['mapped']) <= 64,
                 'finite actual executable/target/loader/SDK mapping selection required')
            env = selected['environment']
            need(type(env) is dict and 'PATH' in env and set(env) <= ENV and all(type(value) is str and
                 '\0' not in value and '\n' not in value for value in env.values()), 'separate explicit native/Worker role environments required')
            for name in ('PATH','LD_LIBRARY_PATH','PYTHONPATH','PYTHONHOME','CMAKE_PREFIX_PATH','AMENT_PREFIX_PATH','ROS_PACKAGE_PATH','PKG_CONFIG_PATH'):
                if name not in env: continue
                need(env[name] and all(value and Path(value).is_absolute() and Path(value).resolve(strict=True).is_dir() and
                    any(Path(value).resolve(strict=True) == root or Path(value).resolve(strict=True).is_relative_to(root) for root in self.roots)
                    for value in env[name].split(':')), 'role search path lacks preserved physical provider coverage')
            if role in ('gateway','native_peer') and self.profile['backend'] == 2:
                need(env.get('RMW_IMPLEMENTATION') == self.profile['runtime']['rmw'], 'native role must use actual selected RMW')
            for value in [selected['host'], selected['image'], selected['launcher'], *selected['mapped'].values()]: self.pin(self.admit_pin(value)['path'])
            if selected['loader'] is not None: self.pin(self.admit_pin(selected['loader'])['path'])
            if self.profile['runtime']['mode'] == 'native':
                need(selected['host'] == selected['image'] or selected['loader'] == selected['host'],
                     'native host must be the actual image or explicitly selected ELF loader; both are physically mapped')
            else:
                need(selected['host']['path'] == self.profile['runtime']['qemu'] and selected['loader'] is not None and
                     Path(selected['loader']['path']).is_relative_to(Path(self.profile['tools']['sysroot'])),
                     'foreign actor requires actual selected QEMU host, target image and sysroot loader')
        rights = c['rights']
        need(set(rights) == {'default_authority', 'main_module', 'backend', 'grants', 'worker_arguments'} and
             rights['default_authority'] == [] and rights['main_module'] == 'main' and
             rights['backend'] == self.profile['backend'] and rights['worker_arguments'] == roles['worker']['arguments'] and
             type(rights['grants']) is list, 'actual main-module CLI authority must match selected worker argv exactly')
        args = roles['worker']['arguments']
        need(str(c['wasm']['path']) in args, 'actual Worker argv must select the admitted Wasm artifact')
        self.report['actual_ROS_authority'] = authority(args,rights['backend'],self.mode,rights['grants'])
        self.owner = self.unitree.Actors(self)
        self.report.update(configuration=c, default_authority=[], current_source=str(self.source),
            source_manifest_sha256=self.manifest_current['sha256'],
            actual_namespace=os.readlink('/proc/self/ns/net'))
        self.save()

    def expanded(self, value):
        need(type(value) is str and '\0' not in value and '\n' not in value, 'literal actor argument required')
        value = value.replace('{out}', str(self.out)).replace('{source}', str(self.source))
        need('{' not in value and '}' not in value, 'unknown official-case argument placeholder'); return value

    def start(self, role):
        need(self.owner is not None and role in self.config['roles'] and role not in self.children and not self.released,
             'one actual configured role may start only before effect release')
        selected = self.config['roles'][role]
        argv = [selected['launcher']['path'], *[self.expanded(value) for value in selected['arguments']]]
        # A selected launcher is a direct executable, not a shell/daemon or
        # guessed future descendant. A reviewed same-PID gate may exec image.
        need(Path(argv[0]).name not in ('sh', 'bash', 'sudo', 'docker', 'podman', 'systemd-run'), 'direct owned actor launcher required')
        # Native SDK LD_LIBRARY_PATH/PYTHONPATH must not accidentally replace
        # the host Worker's own C++/Python providers. Each role is explicit.
        fixed = {name:value for name,value in self.environment.items() if name not in ENV}
        environment = {**fixed, **selected['environment']}
        child, row = self.owner.start(role, argv, environment=environment, json_output=selected['protocol'] == 'line')
        self.children[role] = (child, row); return child

    def setup_write(self, role, data):
        need(not self.released and role in self.children, 'bootstrap transfer only before effect release')
        need(type(data) is bytes and len(data) <= MIB, 'bounded actual cooperative bootstrap bytes required')
        self.owner.write(self.children[role][0], data)
        self.report['setup_transfers'].append({'role': role, 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest()})

    def prove(self, role):
        """Hash actual /proc exe and opened maps while exact pidfd birth lives."""
        self.owner.check(); child, row = self.children[role]; selected = self.config['roles'][role]
        first = row['first_identity']; expected = selected['host']
        def live():
            self.owner.check(); current = self.common.identity(child.pid)
            need(current and self.common.birth(current) == self.common.birth(first) and current['parent_pid'] == os.getpid(),
                 'actual direct actor same birth/parent required')
            entries = [value for (pid, _), value in self.owner.owner.owned.items() if pid == child.pid]
            need(len(entries) == 1 and not self.common.pidfd_terminal(entries[0]['fd']), 'actual owned actor pidfd must be live')
            need(Path('/proc/%d/cgroup' % child.pid).read_bytes() == Path('/proc/self/cgroup').read_bytes(), 'actual actor cgroup changed')
            need(os.readlink('/proc/%d/ns/net' % child.pid) == os.readlink('/proc/self/ns/net'), 'actual actor escaped isolated loopback namespace')
            facts = self.low.mapping_file_facts(Path('/proc/%d/exe' % child.pid).stat())
            need(str(child.pid) in (Path(self.profile['runtime']['expected_leaf'])/'cgroup.procs').read_text().split(),
                 'actual actor is absent from current original cgroup.procs')
            need(facts == {name: expected[name] for name in FACTS}, 'actual host executable identity differs')
            return {'pid': child.pid, 'parent_pid': current['parent_pid'], 'starttime_ticks': current['starttime'],
                'state': current['state'], 'membership': Path('/proc/%d/cgroup' % child.pid).read_text().splitlines(),
                'live_cgroup_procs': str(child.pid) in (Path(self.profile['runtime']['expected_leaf'])/'cgroup.procs').read_text().split(),
                'host_identity': facts}
        def full_host_hash():
            live(); descriptor = os.open('/proc/%d/exe' % child.pid, os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK)
            try:
                before = os.fstat(descriptor); facts = self.low.mapping_file_facts(before)
                need(stat.S_ISREG(before.st_mode) and facts == {name: expected[name] for name in FACTS}, 'opened actual host executable differs')
                digest = hashlib.sha256(); count = 0
                while True:
                    live(); part = os.read(descriptor, min(MIB, expected['bytes']-count+1))
                    if not part: break
                    count += len(part); need(count <= expected['bytes'], 'actual executable grew'); digest.update(part)
                need(count == expected['bytes'] and digest.hexdigest() == expected['sha256'] and
                     self.low.mapping_file_facts(os.fstat(descriptor)) == facts, 'actual opened host executable bytes changed')
            finally: os.close(descriptor)
            live()
        full_host_hash()
        wanted = dict(selected['mapped'], actual_image=selected['image'])
        if selected['loader'] is not None: wanted['actual_loader'] = selected['loader']
        targets = {}
        for name, value in wanted.items():
            current = self.pin(value['path']); need(current == value, 'actual selected mapping pin changed')
            with Path(value['path']).open('rb') as stream: header = stream.read(64)
            need(len(header) >= 20 and header[:4] == b'\x7fELF' and header[5] in (1,2), 'actual selected mapping must be ELF')
            endian = 'little' if header[5] == 1 else 'big'; machine = int.from_bytes(header[18:20], endian)
            need((header[4], endian, machine) == ({32:1,64:2}[self.profile['target']['bits']],
                 self.profile['target']['endian'], self.profile['target']['elf_machine']), 'mapped target architecture differs from official-case semantics')
            targets[name] = dict(value, elf_class=header[4], elf_data=header[5], e_machine=machine)
        proof = self.low.opened_target_mappings(child.pid, targets, live)
        need(all(proof['targets'][name] for name in targets), 'real Worker/Gateway/native image or SDK mapping absent')
        live()
        with Path('/proc/%d/maps' % child.pid).open('rb') as stream: raw = stream.read(4*MIB+1)
        need(len(raw) <= 4*MIB, 'bounded complete actual mapping inventory required')
        checked = set()
        for line in raw.splitlines():
            parts = line.split(None, 5)
            if len(parts) < 5 or not parts[4].isdigit() or int(parts[4]) == 0: continue
            need(re.fullmatch(rb'[0-9a-fA-F]+-[0-9a-fA-F]+', parts[0]), 'actual physical mapping range malformed')
            live(); descriptor = os.open('/proc/%d/map_files/%s' % (child.pid, self.low.canonical_map_files_range(parts[0].decode('ascii'))),
                os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK)
            try:
                mapped_first = os.fstat(descriptor); physical = tuple(self.low.mapping_file_facts(mapped_first)[name] for name in FACTS)
                need(stat.S_ISREG(mapped_first.st_mode) and physical in self.allowed_files,
                     'actual mapped provider is absent from the complete selected physical catalogs')
                if physical not in checked:
                    value = self.allowed_files[physical]; digest = hashlib.sha256(); count = 0
                    while True:
                        live(); block = os.read(descriptor, min(MIB, value['bytes']-count+1))
                        if not block: break
                        count += len(block); need(count <= value['bytes'], 'mapped provider grew'); digest.update(block)
                    need(count == value['bytes'] and digest.hexdigest() == value['sha256'] and
                         self.low.mapping_file_facts(os.fstat(descriptor)) == self.low.mapping_file_facts(mapped_first),
                         'actual mapped provider bytes differ from complete current inventory')
                    checked.add(physical)
            finally: os.close(descriptor)
        full_host_hash(); proof['complete_current_mapped_file_identities'] = [list(value) for value in sorted(checked)]
        self.proofs[role] = proof; return proof

    def release(self):
        required = {'worker', 'native_peer'} | ({'gateway'} if self.mode == 'positive' else set())
        need(not self.released and required <= set(self.children), 'all actual principal and independent native witness roles required')
        for role in self.children: self.prove(role)
        self.released = True; self.report['effect_release_monotonic_ns'] = time.monotonic_ns(); self.save()

    def write(self, role, data):
        need(self.released, 'actual effects cannot be triggered before live physical mapping admission')
        self.owner.write(self.children[role][0], data)

    def tick(self, seconds=.01):
        """Pace an owned native/Guest protocol without granting a Guest clock."""
        need(type(seconds) in (int,float) and 0 < seconds <= .1, 'bounded explicit owner polling interval required')
        self.owner.check(); time.sleep(seconds); self.owner.check()

    def finish_role(self, role):
        """Retire one normal zero-exit role before its dependent server closes."""
        need(role in self.children, 'actual known role required')
        return self.owner.finish(self.children[role][0])

    def interrupt_master(self):
        """Gracefully stop only the original owned ROS Master via its pidfd.

        Gateway lifetime normally follows its bound Worker's pidfd. A ROS1
        Master is different: SIGINT is its documented normal shutdown request.
        Never signal a numeric PID, another role, an old namespace or a birth
        that has already retired. Its real return code is still required zero.
        """
        need('master' in self.children and self.released, 'actual released owned master required')
        child,row = self.children['master']; self.owner.check()
        current = self.common.identity(child.pid)
        entries = [(key,value) for key,value in self.owner.owner.owned.items() if key[0] == child.pid]
        need(child.poll() is None and current and len(entries) == 1 and
             self.common.birth(current) == entries[0][0] == self.common.birth(row['first_identity']) and
             current['parent_pid'] == os.getpid() and not self.common.pidfd_terminal(entries[0][1]['fd']),
             'live original direct Master birth/pidfd required for graceful shutdown')
        signal.pidfd_send_signal(entries[0][1]['fd'],signal.SIGINT)
        self.report['master_shutdown'] = {'signal':'SIGINT','mechanism':'HELD_SAME_BIRTH_PIDFD',
            'pid':child.pid,'starttime_ticks':current['starttime']}
        return self.finish_role('master')

    def read(self, role, count=None):
        need(role in self.children, 'actual known pipe source required')
        # Source adapters parse real marker/JSON/binary records. Tokens bind raw
        # logged bytes to one actual role; an arbitrary adapter dictionary is
        # never accepted as a native effect or a Wasm callback observation.
        if count is None:
            need(self.config['roles'][role]['protocol'] == 'line', 'actual line protocol selection required')
            raw = self.owner.receive_line(self.children[role][0]).encode('utf-8')
        else:
            need(self.config['roles'][role]['protocol'] == 'binary' and type(count) is int and 0 < count <= MIB,
                 'bounded actual binary protocol selection required')
            raw = self.owner.receive_bytes(self.children[role][0], count)
        token = len(self.observations); self.observations.append({'role': role, 'raw': raw,
            'captured_monotonic_ns': time.monotonic_ns(), 'after_release': self.released})
        return token

    def effect(self, label, native_token, wasm_token, expected):
        need(self.mode == 'positive' and self.released and type(label) is str and label, 'explicit positive official effect required')
        need(type(native_token) is int and type(wasm_token) is int and 0 <= native_token < len(self.observations) and
             0 <= wasm_token < len(self.observations), 'actual retained observation tokens required')
        native, wasm = self.observations[native_token], self.observations[wasm_token]
        need(native['role'] == 'native_peer' and wasm['role'] == 'worker' and native['after_release'] and wasm['after_release'],
             'separate actual native effect and Wasm callback/principal records required')
        left = self.adapter.normalize('native_peer', native['raw'], self.config)
        right = self.adapter.normalize('worker', wasm['raw'], self.config)
        need(left == right == expected, 'actual independent native/Guest effect differs from official fixture oracle')
        json.dumps(expected, allow_nan=False)
        self.comparisons.append({'label': label, 'native_token': native_token, 'wasm_token': wasm_token, 'expected': expected,
            'native_raw_sha256': hashlib.sha256(native['raw']).hexdigest(), 'wasm_raw_sha256': hashlib.sha256(wasm['raw']).hexdigest()})

    def quiet(self, native_token, *, minimum_ns):
        need(self.mode == 'default-denied' and self.released and type(native_token) is int and
             0 <= native_token < len(self.observations), 'actual independent denied-case native witness required')
        row = self.observations[native_token]
        need(row['role'] == 'native_peer' and row['after_release'], 'quiet witness must come from actual independently mapped native peer')
        value = self.adapter.normalize('native_peer', row['raw'], self.config)
        need(type(value) is dict and set(value) == {'effects', 'samples', 'elapsed_ns'} and value['effects'] == 0 and
             type(value['samples']) is int and value['samples'] > 0 and type(value['elapsed_ns']) is int and
             type(minimum_ns) is int and 1000000 <= minimum_ns <= value['elapsed_ns'] <= 600000000000,
             'actual native zero-effect interval/sample witness required')
        self.quiet_proofs.append({'token': native_token, 'observation': value, 'minimum_ns': minimum_ns})

    def finish(self):
        required = {'worker', 'native_peer'} | ({'gateway'} if self.mode == 'positive' else set())
        need(self.released and required <= set(self.proofs), 'actual same-main Worker/native/Gateway mapping boundary mandatory')
        need(bool(self.comparisons) if self.mode == 'positive' else bool(self.quiet_proofs), 'actual effect or default-denied native quiet proof missing')
        for child, row in self.children.values(): self.owner.finish(child)
        worker, worker_row = self.children['worker']; audit = []
        if self.mode == 'positive':
            _, gateway_row = self.children['gateway']
            raw, actual = self.common.body_pin(Path(gateway_row['stderr']), self.limits['log_bytes']); self.product(actual['path'])
            pattern = rb'ros_audit timestamp_ns=([0-9]+) backend=([12]) worker_uid=([0-9]+) worker_pid=([1-9][0-9]*) session=([1-9][0-9]*) operation=([a-z.-]+) alias=([A-Za-z0-9_-]+) error=([0-9]+)'
            for line in raw.splitlines():
                if not line.startswith(b'ros_audit '): continue
                match = re.fullmatch(pattern, line)
                need(match and int(match[2]) == self.profile['backend'] and int(match[3]) == os.getuid() and
                     int(match[4]) == worker.pid, 'actual Gateway SO_PEERCRED principal audit differs')
                audit.append({'session': int(match[5]), 'operation': match[6].decode(), 'alias': match[7].decode(), 'error': int(match[8])})
            binds = [row for row in audit if row['operation'] == 'session.bind']
            need(len(binds) == 1 and binds[0]['alias'] == 'target' and binds[0]['error'] == 0 and
                 all(row['session'] == binds[0]['session'] for row in audit), 'one actual admitted Gateway main principal required')
        else:
            raw, actual = self.common.body_pin(Path(worker_row['stderr']), self.limits['log_bytes']); self.product(actual['path'])
            for line in raw.splitlines():
                if not line.startswith(b'{'): continue
                value = self.unitree.decode(line)
                if value.get('event') != 'unresolved-denial': continue
                need(set(value) == {'wall_ns','ros','module','runtime_generation','observation_instance','event','cross_module','error'} and
                     value['ros'] == 'ros%d' % self.profile['backend'] and value['module'] == 'main' and
                     value['cross_module'] is False and value['error'] == 13 and all(type(value[name]) is int and value[name] > 0
                     for name in ('wall_ns','runtime_generation','observation_instance')), 'actual default-disabled main denial record required')
                audit.append(value)
            need(audit and len({(row['runtime_generation'],row['observation_instance']) for row in audit}) == 1,
                 'actual Worker unresolved-denial evidence missing or mixed lifetimes')
        self.report.update(status='PASS_ACTUAL_OFFICIAL_API_PORT_EXECUTION', live_mappings=self.proofs,
            effects=self.comparisons, native_zero_effects=self.quiet_proofs, authority_audit=audit,
            observations=[dict(row,raw_hex=row['raw'].hex(),raw=None) for row in self.observations])
        self.save(); return dict(self.report)

    def close(self):
        if self.closed: return self.report['cleanup']
        try:
            cleanup = self.owner.close() if self.owner else {'status':'PASS','remaining':[], 'scope':'NO_ACTOR_STARTED'}
        except BaseException as error:
            self.report.update(status='FAIL',cleanup={'status':'CLEANUP_INCOMPLETE','error':repr(error),
                'remaining':'UNPROVED_AFTER_CLOSE_EXCEPTION'})
            try: self.save()
            except BaseException as publication_error: self.report['cleanup']['publication_error']=repr(publication_error)
            # Not idempotently closed: callers must never interpret an
            # exception as a successful close or start another case.
            raise
        self.report['cleanup'] = cleanup
        self.closed = True
        self.save(); return cleanup
