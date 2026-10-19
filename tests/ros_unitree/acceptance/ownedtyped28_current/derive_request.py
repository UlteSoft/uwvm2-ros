#!/usr/bin/env python3
"""Source-only request derivation for Root's future owned private-net lane.

This callable writes no files, starts no child and never creates a namespace.
Root must SHA-admit this source and the reviewed external metadata maker first.
All function arguments describing future execution must be actual observations
made inside the still-live, same-PID-unshared direct child of Root's pidfd owner.
No namespace number, completed Typed14 report or file identity is predicted here.
"""
import os
from pathlib import Path


PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
MAKER_SOURCE = 'tests/ros_unitree/acceptance/make_typed_transport_configuration.py'
WORKER_SHA = '18932cb8abc7d9998eb7431f751d6d1feea687cb380f1b247eef12750de0fb64'
MIB, GIB = 1024**2, 1024**3


def derive_typed28_request(common, metadata, unitree, elf_reader, guard, *,
        typed_build_current, workers_current, native_retained_current,
        metadata_maker_current, source, manifest_current, python_current,
        cgroup, parent_handoff_current, owned_namespace_identity,
        cyclonedds_current, fresh_paths, restart_mappings):
    """Return a closed request and a separate, honest derivation sidecar.

    ``common``, ``unitree`` and ``elf_reader`` must come from the actual selected
    complete source. ``metadata`` is its manifest-bound external Typed28 maker. Root's
    live guard is mandatory throughout. The outer Root owner must independently
    own this SAME process birth by a live pidfd; this callable checks the actual
    direct-parent handoff and live birth but does not pretend JSON proves pidfd
    ownership. Keep that outer owner alive through maker and all28 transport
    cases, inside this same private network namespace.
    """
    def need(value, reason):
        if not value:
            raise ValueError(reason)

    def now(pin, maximum=64 * MIB):
        guard()
        need(type(pin) is dict and set(pin) == PIN, 'closed CURRENT full-seven observation required')
        metadata.full_pin(pin)
        need(0 < pin['bytes'] <= 2 * GIB, 'finite ordinary current input required')
        actual = metadata.opened(pin['path'], max(maximum, pin['bytes']), guard)
        need(actual == pin, 'supplied current observation changed')
        current_inputs[pin['path']] = actual
        return actual

    def doc(pin, maximum=64 * MIB):
        now(pin, maximum)
        return metadata.document(pin, maximum, guard)

    def source_module(relative, module_file):
        target = Path(source) / relative
        need(str(target) == str(Path(module_file).absolute()), 'selected source helper location differs')
        observed = metadata.opened(target, 2 * MIB, guard)
        need(frozen.get(relative) == {key: observed[key] for key in ('bytes', 'sha256')},
             'complete selected source must bind helper bytes')
        current_inputs[observed['path']] = observed

    def actual_elf(pin):
        # Reuse the existing source-bound, ordinary-descriptor ELF64 decoder;
        # no readelf subprocess, compiler, loader or target executable runs.
        now(pin)
        guard()
        record = elf_reader(common, Path(pin['path']), {2, 3})
        guard()
        need(record['input'] == pin and record['bits'] == 64 and record['endian'] == 'little' and
             record['machine'] == 62, 'actual opened x86_64 ELF differs')
        return record

    current_inputs = {}
    guard()
    need(type(cgroup) is dict and set(cgroup) == {'memory_max', 'memory_swap_max', 'expected_device',
         'expected_inode', 'expected_boot_id', 'expected_leaf', 'expected_cpus'} and
         cgroup['memory_max'] == 64 * GIB and cgroup['memory_swap_max'] == 0 and cgroup['expected_cpus'] == '0-31',
         'actual closed current64/zero-swap/cpus context required')
    before_cgroup = common.cgroup_state(cgroup)
    need(metadata.__file__ == metadata_maker_current['path'], 'actual prestaged metadata maker location differs')
    now(metadata_maker_current, MIB)
    need(Path(source).resolve(strict=True) == Path(source) and
         manifest_current['path'] == str(Path(source).parent / 'source-manifest.json'), 'actual complete selected source required')
    frozen = doc(manifest_current, 16 * MIB)
    need(type(frozen) is dict and frozen.get(MAKER_SOURCE) == {key: metadata_maker_current[key] for key in ('bytes', 'sha256')},
         'actual metadata maker must equal its selected complete-manifest source entry')
    original_source_state = common.source_state(Path(source), Path(manifest_current['path']))
    need(original_source_state['manifest'] == manifest_current, 'actual complete current source changed')
    source_module('tools/ros_distro/common.py', common.__file__)
    source_module('tests/ros_unitree/common.py', unitree.__file__)
    source_module('tests/ros_unitree/acceptance/native74_r6/native_producer.py', elf_reader.__code__.co_filename)
    now(python_current, 128 * MIB)
    import sys
    need(str(Path(sys.executable).resolve(strict=True)) == python_current['path'], 'actual executing host Python required')

    handoff = doc(parent_handoff_current, MIB)
    need(type(handoff) is dict and set(handoff) == {'format_version', 'kind', 'outer_identity',
         'parent_network_namespace', 'cgroup', 'source_manifest'} and handoff['format_version'] == 1 and
         handoff['kind'] == 'uwvm-unitree-typed28-owned-net-handoff-v1' and handoff['cgroup'] == cgroup and
         handoff['source_manifest'] == manifest_current, 'exact owned same-PID unshare parent handoff required')
    need(common.birth(common.identity(os.getppid())) == common.birth(handoff['outer_identity']) and
         os.readlink('/proc/%d/ns/net' % os.getppid()) == handoff['parent_network_namespace'] and
         common.birth(common.identity(os.getpid())) == common.birth(owned_namespace_identity),
         'live same-birth direct parent and independently owned current child birth required')
    namespace = os.readlink('/proc/self/ns/net')
    need(namespace != handoff['parent_network_namespace'], 'actual private network namespace must differ from its live parent')
    loopback_before = unitree.current_loopback(namespace)

    # Only references IN unchanged producer documents may use selected restart
    # transitions. All request-level observations remain actual current pins.
    discovery = metadata.Discovery({'restart_mappings': restart_mappings}, guard)
    commands = doc(typed_build_current)
    need(commands.get('status') == 'PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY' and
         commands.get('compiled_command_products') == 14 and commands.get('retained_command_products') == 9 and
         commands.get('fresh_command_products') == 5 and commands.get('actual_Runtime_cases_executed') == 0 and
         commands.get('source_manifest_sha256') == 'a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd',
         'actual completed Typed14 continuation with original Source40 label required')
    workers = doc(workers_current)
    need(workers_current['sha256'] == WORKER_SHA and
         workers.get('status') == 'PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED' and
         workers.get('source_manifest_sha256') == 'c8a2bde2c12fab767ca31eba68a08c10093dc5844ef76ea2dec8f87ae9738ee7',
         'unchanged actual Source37 interpreter producer required')
    retained = doc(native_retained_current)
    need(retained.get('schema') == 'uwvm-native74-retained-products-at-rest-v1' and
         retained.get('status') == 'PASS_RETAINED_NATIVE74_BUILD_PRODUCTS_FRESH_AT_REST_CATALOG_NOT_EXECUTED' and
         retained.get('preparation_mode') == 'retained74-at-rest' and retained.get('execution_performed') is False and
         retained['retained_compilation']['translation_units'] == 74,
         'actual successful distinct retained74 at-rest receipt required')
    raw = discovery.doc(retained['original_raw_inner'])
    raw_outer = discovery.doc(retained['original_raw_outer'])
    need(raw.get('status') == raw_outer.get('status') == 'FAIL' and raw['source_manifest_sha256'] ==
         '54d6c79978e36bd13fa37272d8dd5f2cc94a9d2e62ac437b51dfc4aa4f6ae5b9',
         'original Native failures and Source39 provenance must remain unchanged')
    interfaces = discovery.doc(raw['producer_reports']['native'])
    old_registries = discovery.doc(raw['producer_reports']['worker_registries'])
    need(old_registries['status'] == 'PREPARED_FOUR_NATIVE_INTERPRETER_WORKERS_WITH_EXPLICIT_REGISTRIES_NOT_EXECUTED' and
         interfaces['status'] == 'PREPARED_NATIVE_INTERFACES_IDENTITIES_AND_SOURCE_PROVIDERS',
         'actual native interface and historical registry producers required')
    families = {row['family']: row for row in interfaces['families']}
    need(set(families) == {'unitree_go', 'unitree_hg', 'unitree_api'}, 'all three actual native families required')
    registries = {'go-api': discovery.pin(old_registries['ros2_generation']),
                  'hg': discovery.pin(families['unitree_hg']['generation'])}

    # The fresh at-rest catalog is a COMPLETE physical file selection. Selecting
    # required DSOs below does not prove that any process has mapped them.
    catalog_pin = discovery.pin(retained['execution_file_catalog'])
    body, actual_catalog = metadata.opened(catalog_pin['path'], 128 * MIB, guard, body=True)
    need(actual_catalog == catalog_pin, 'fresh complete native catalog changed')
    catalog = {}
    for line in body.splitlines(keepends=True):
        guard()
        need(len(line) <= 16384 and line.endswith(b'\n'), 'bounded complete native file catalog row required')
        row = metadata.decode(line)
        need(type(row) is dict and set(row) == PIN and row['path'] not in catalog, 'unique complete native physical catalog pin required')
        catalog[row['path']] = row
    need(catalog, 'nonempty complete native file catalog required')
    del body

    library_dirs = retained['execution_environment_base']['LD_LIBRARY_PATH'].split(':')
    need(library_dirs and len(library_dirs) == len(set(library_dirs)) and all(
         str(Path(p).resolve(strict=True)) == p and Path(p).is_dir() for p in library_dirs),
         'actual unique complete native library search directories required')
    native_derivation = {}
    def native_set(role, previous_elf):
        binary = discovery.pin(previous_elf['input'])
        observed = actual_elf(binary)
        need({k: v for k, v in observed.items() if k != 'input'} ==
             {k: v for k, v in previous_elf.items() if k != 'input'} and
             observed['DT_RPATH'] == [':'.join(library_dirs)] and observed['DT_RUNPATH'] == [],
             'actual native ELF tags differ from original witnessed exact search path')
        result, resolution = {}, []
        for soname in observed['DT_NEEDED']:
            need(type(soname) is str and soname not in ('', '.', '..') and '/' not in soname, 'ordinary actual DT_NEEDED SONAME required')
            existing = [Path(root) / soname for root in library_dirs if (Path(root) / soname).exists()]
            need(existing, 'actual selected native search path does not resolve SONAME')
            physical = str(existing[0].resolve(strict=True))
            need(physical in catalog, 'resolved native DSO absent from fresh complete at-rest catalog')
            pin = now(catalog[physical])
            need(actual_elf(pin)['type'] == 3, 'required native SDK mapping target must be a real DSO')
            result[physical] = pin
            resolution.append({'soname': soname, 'selected_spelling': str(existing[0]), 'current_pin': pin})
        need(1 <= len(result) <= 32, 'finite native required DSO subset required')
        native_derivation[role] = {'actual_binary_ELF': observed, 'DT_NEEDED_resolution': resolution,
            'complete_live_mapping_qualification': 'NOT_PERFORMED'}
        return list(result.values())

    mappings = {'peer': native_set('peer', raw['typed_peer'])}
    gateways = {row['profile']: row for row in raw['gateway_profiles']}
    for profile in ('go-api', 'hg'):
        mappings['gateway-' + profile] = native_set('gateway-' + profile, gateways[profile]['actual_elf'])
        # The same linked DSOs are required both before and after actual ROS
        # context initialization. This is deliberately a required SUBSET, not a
        # prediction that a particular dynamically loaded RMW is already mapped.
        # The real runner independently proves every ordinary map_file and DDS.
        mappings['context-' + profile] = list(mappings['gateway-' + profile])

    config = discovery.doc(workers['configuration'])
    observed_roots = {row['root'] for row in workers['input_roots_after']}
    need(set(config['input_roots']) == observed_roots, 'original Worker config/complete inventory roots differ')
    host_roots = [Path(root) for root in config['input_roots']]
    old_inputs = {}
    for pin in workers['input_pins']:
        if type(pin) is dict and PIN <= pin.keys():
            previous = {k: pin[k] for k in PIN}
            prior = old_inputs.setdefault(previous['path'], previous)
            need(prior == previous, 'conflicting original Worker physical selections')
    def worker_selected(path, linked=None):
        physical = str(Path(path).resolve(strict=True))
        need(physical in old_inputs, 'resolved host DSO/interpreter lacks original Worker selected input')
        if linked is not None:
            need(physical in linked and linked[physical] == old_inputs[physical],
                 'required Worker DSO must be in this profile actual physical linker trace')
        return discovery.pin(old_inputs[physical])
    worker_roots = {str(Path(workers_current['path']).parent)}
    worker_derivation = {}
    for profile in ('ros2', 'dual'):
        rows = [row for row in workers['profiles'] if row['profile'] == profile]
        need(len(rows) == 1, 'one actual three-TU Worker profile required')
        binary = discovery.pin(rows[0]['binary'])
        linked = {}
        for value in rows[0]['link_inputs']:
            need(type(value) is dict and PIN <= value.keys(), 'actual full-seven Worker linker trace input required')
            previous = {key: value[key] for key in PIN}
            prior = linked.setdefault(previous['path'], previous)
            need(prior == previous, 'conflicting actual profile linker input')
        elf = actual_elf(binary)
        need(len(elf['PT_INTERP']) == 1, 'actual single host Worker interpreter required')
        interpreter = worker_selected(elf['PT_INTERP'][0])
        selections = {interpreter['path']: interpreter}
        resolved = []
        for soname in elf['DT_NEEDED']:
            need(type(soname) is str and soname not in ('', '.', '..') and '/' not in soname, 'ordinary host Worker SONAME required')
            candidates = {str((root / soname).resolve(strict=True)) for root in host_roots if (root / soname).exists()}
            need(len(candidates) == 1, 'actual complete Worker roots must resolve one physical DSO; explicit ambiguity refused')
            pin = worker_selected(next(iter(candidates)), linked)
            need(actual_elf(pin)['type'] == 3, 'actual host Worker SDK DSO required')
            selections[pin['path']] = pin
            resolved.append({'soname': soname, 'current_pin': pin})
        need(1 <= len(selections) <= 32, 'finite real host Worker mapping subset required')
        for pin in selections.values():
            roots = [root for root in host_roots if Path(pin['path']).is_relative_to(root)]
            need(roots, 'Worker execution input lacks actual complete producer root')
            worker_roots.add(str(max(roots, key=lambda root: len(root.parts))))
        mappings['worker-' + profile] = list(selections.values())
        worker_derivation[profile] = {'actual_binary_ELF': elf, 'actual_interpreter': interpreter,
            'DT_NEEDED_resolution': resolved, 'complete_live_mapping_qualification': 'NOT_PERFORMED'}
    # Frame deliberately executes in C.UTF-8. Preserve the actual recorded host
    # locale tree as well as ELF roots; select no guessed /usr ancestor.
    locale_roots = {str(root) for root in host_roots if root.name == 'locale'}
    need(locale_roots and config['environment']['LANG'] == config['environment']['LC_ALL'] == 'C.UTF-8',
         'actual complete Worker C.UTF-8 locale inventory required')
    worker_roots.update(locale_roots)
    need(1 <= len(worker_roots) <= 32, 'finite complete Worker execution root subset required')
    now(cyclonedds_current, MIB)
    need(type(fresh_paths) is dict and set(fresh_paths) == {'controls', 'admission_out', 'transport_out'} and
         len(os.fsencode(Path(fresh_paths['transport_out']) / 's27')) < 104,
         'closed fresh path plan with short actual UNIX socket path required')
    request = {'format_version': 1, 'kind': 'uwvm-unitree-typed28-configuration-request-v1',
        'source': {'root': source, 'manifest': manifest_current}, 'python': python_current,
        'current_context': {'cgroup': cgroup, 'network_namespace': namespace},
        'reports': {'commands': typed_build_current, 'workers': workers_current, 'native_products': native_retained_current},
        'worker_registries': registries, 'mapping_sets': mappings, 'worker_catalog_roots': sorted(worker_roots),
        'restart_mappings': restart_mappings, 'cyclonedds_xml': cyclonedds_current, 'paths': fresh_paths,
        'limits': {'jobs': 1, 'command_seconds': 600, 'rss_bytes': 2 * GIB,
                   'log_bytes': 16 * MIB, 'output_bytes': 4 * GIB}}
    for pin in [*current_inputs.values(), *discovery.pins.values()]:
        now(pin, max(64 * MIB, pin['bytes']))
    need(common.source_state(Path(source), Path(manifest_current['path'])) == original_source_state,
         'complete selected source changed during request derivation')
    common.cgroup_preserved(before_cgroup, common.cgroup_state(cgroup))
    need(common.birth(common.identity(os.getppid())) == common.birth(handoff['outer_identity']) and
         common.birth(common.identity(os.getpid())) == common.birth(owned_namespace_identity) and
         os.readlink('/proc/%d/ns/net' % os.getppid()) == handoff['parent_network_namespace'],
         'live owned namespace/direct parent changed before request acceptance')
    loopback_after = unitree.current_loopback(namespace)
    return request, {'parent_handoff': parent_handoff_current, 'namespace_owner_identity': owned_namespace_identity,
        'actual_loopback_before': loopback_before, 'actual_loopback_after': loopback_after,
        'outer_pidfd_ownership': 'REQUIRED_AND_PROVED_BY_ROOT_OWNER_NOT_BY_THIS_CALLABLE',
        'native_required_linked_DSO_derivation': native_derivation, 'Worker_required_host_DSO_derivation': worker_derivation,
        'registry_generation_source': {'go-api': 'original registry producer.ros2_generation',
            'hg': 'original native interfaces.unitree_hg.generation'},
        'planning_historical_current_bridges': discovery.bridges,
        'original_raw_native_statuses': {'inner': raw['status'], 'outer': raw_outer['status']},
        'compiler_or_Runtime_or_DDS_executed': False, 'Guest_rights_changed': False,
        'metadata_maker_or_transport_started': False}
