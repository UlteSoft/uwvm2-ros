#!/usr/bin/env python3
"""Admit actual complete ROS1 legacy products before any TCPROS actor starts.

The closed selection contains only configured input labels and actual root names.
It cannot supply an MD5, arbitrary command suffix, replacement provider or Guest
permission. Native generated identities, full compiler dependencies and the
Runtime registry must agree for all eleven original ROS1 definitions. Historical
exec-gate provenance is a separate finite scope and never supplies ROS1 types.
"""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import re
import stat

PIN = frozenset(('path','bytes','sha256','device','inode','mtime_ns','ctime_ns'))
FAMILY = 'unitree_legged_msgs'
MESSAGES = ('BmsCmd','BmsState','Cartesian','HighCmd','HighState','IMU','LED','LowCmd','LowState','MotorCmd','MotorState')
COMMAND_STATUS = 'PREPARED_SIX_LEGACY_COMMAND_PRODUCTS_BINARY_VALIDATED_NOT_EXECUTED'

def require(value, reason):
    if not value:
        raise ValueError(reason)


def exact(value, names, reason):
    require(type(value) is dict and set(value) == set(names), reason)


def decode(raw):
    """Reject duplicate keys and nonfinite constants in all evidence JSON."""
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, 'duplicate typed transport evidence key')
            result[key] = value
        return result
    def invalid(_):
        raise ValueError('nonfinite typed transport metadata')
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=invalid)


def generation_outputs(frame, generation, directory):
    """Reopen every flat generated output against its complete content digest."""
    require(generation.get('format') == 'uwvm-ros-custom-bounded-v2' and
            type(generation.get('outputs')) is dict and generation['outputs'], 'complete bounded generation proof required')
    directory = Path(directory)
    for name, expected in generation['outputs'].items():
        require(type(name) is str and Path(name).name == name and name not in ('.', '..'), 'flat generated output name required')
        path = directory / name
        require(path.resolve(strict=True) == path and any(path.is_relative_to(root) for root in frame.roots),
                'generated output must remain ordinary and inside a complete inventoried root')
        actual = frame.pin(path)
        require(expected == {key: actual[key] for key in ('bytes', 'sha256')}, 'actual generated output bytes differ')


def host_catalog(frame, api, roots):
    """Select complete *current* host catalogs for real Worker /map_files proof.

    The native COPY catalog cannot qualify an interpreter linked to the host
    SDK. These rows come from Frame's actual current full-root inventories.
    Symlink targets use their already resolved physical full7 pins. No maps
    display-name/device fallback or arbitrary host path is ever introduced.
    """
    require(type(roots) is list and 1 <= len(roots) <= 64 and len(set(roots)) == len(roots) and
            all(Path(root) in frame.roots for root in roots), 'explicit complete Worker execution roots required')
    files, elfs = {}, {}
    for inventory in frame.catalogs:
        if inventory['root'] not in roots:
            continue
        catalog_pin = frame.pin(inventory['catalog']['path'])
        digest, count = hashlib.sha256(), 0
        with Path(catalog_pin['path']).open('rb') as stream:
            for line in stream:
                frame.guard()
                require(len(line) <= 65536 and line.endswith(b'\n'), 'bounded current Worker catalog row required')
                digest.update(line)
                count += len(line)
                row = decode(line)
                if row['kind'] == 'file':
                    pin = {key: row[key] for key in PIN - {'path'}}
                    pin['path'] = str(Path(inventory['root']) / row['relative'])
                elif row['kind'] == 'symlink' and 'target_file' in row:
                    pin = row['target_file']
                else:
                    continue
                exact(pin, PIN, 'actual complete Worker catalog physical pin required')
                prior = files.setdefault(pin['path'], pin)
                require(prior == pin and len(files) <= 1000000, 'Worker inventory conflict or entry limit')
                path = Path(pin['path'])
                descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
                try:
                    before = os.fstat(descriptor)
                    require(stat.S_ISREG(before.st_mode) and all(getattr(before, attr) == pin[key] for attr, key in
                        (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                         ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))), 'Worker catalog file identity changed')
                    header = os.read(descriptor, 64)
                    after = os.fstat(descriptor)
                    require(all(getattr(before, key) == getattr(after, key) for key in
                        ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'Worker ELF classification file changed')
                finally:
                    os.close(descriptor)
                if header.startswith(b'\x7fELF'):
                    elfs[pin['path']] = pin
        require(count == catalog_pin['bytes'] and digest.hexdigest() == catalog_pin['sha256'], 'complete Worker catalog changed')
    require(files and elfs, 'nonempty complete current Worker file/ELF catalogs required')
    return list(files.values()), list(elfs.values())


def binary_boundary(frame, api, path, command):
    """Reparse actual signatures, bounded memory and the same-module local entry.

    Full code-body validation is separately bound to the actual completed build
    receipt. This inspection never substitutes a parser for a full validator.
    """
    inspector = api.frozen_module(frame.source, frame.frozen, 'sdk/tools/check_wasm_imports.py', 'typed_command_import_reader')
    raw, pin = frame.common.body_pin(path, 64 * 1024**2)
    require(frame.pin(path) == pin, 'actual command bytes changed')
    imports = inspector.inspect(raw)
    abi, _ = frame.common.document(frame.source / 'sdk/abi/abi-v1.json')
    expected = {item['name']: ([p['type'] for p in item['params']], item['results'])
                for item in abi['modules']['uwvm_ros1_v1']['functions']}
    seen, ros, stdio = set(), set(), set()
    for item in imports:
        key = item['module'], item['name']
        require(item['kind'] == 0 and key not in seen, 'unique actual function-only command imports required')
        seen.add(key)
        signature = item['params'], item['results']
        if key[0] == 'uwvm_ros1_v1':
            require(signature == expected.get(key[1]), 'actual ROS1 ABI signature differs')
            ros.add(key[1])
        else:
            shape = (['i32'], []) if key[1] == 'proc_exit' else (['i32'] * 4, ['i32'])
            require(key[0] == 'wasi_snapshot_preview1' and key[1] in ('fd_read', 'fd_write', 'proc_exit') and
                    signature == shape, 'optional WASI or foreign bridge import forbidden')
            stdio.add(key[1])
    require('capability_open' in ros and stdio == {'fd_read', 'fd_write', 'proc_exit'}, 'actual SDK plus basic command IO required')
    reader = inspector.Reader
    root = reader(raw)
    require(bytes(root.take(8)) == b'\0asm\1\0\0\0', 'actual Wasm v1 core artifact required')
    signatures, functions, memories, exports, sections = [], [], [], {}, set()
    while root.position < len(root.data):
        section = root.byte()
        body = reader(root.take(root.uint()))
        if section:
            require(section not in sections, 'duplicate actual core section')
            sections.add(section)
        if section == 1:
            for _ in range(body.count()):
                require(body.byte() == 0x60, 'ordinary function type required')
                signatures.append(([body.value() for _ in range(body.count())], [body.value() for _ in range(body.count())]))
            body.end()
        elif section == 3:
            functions = [body.uint() for _ in range(body.count())]
            body.end()
        elif section == 5:
            for _ in range(body.count()):
                require(body.uint() == 1, 'bounded unshared32-bit memory required')
                memories.append((body.uint(), body.uint()))
            body.end()
        elif section == 7:
            for _ in range(body.count()):
                name, kind, index = body.name(), body.byte(), body.uint()
                require(name not in exports, 'duplicate actual export name')
                exports[name] = kind, index
            body.end()
        elif section == 8:
            raise ValueError('implicit Wasm start section forbidden')
    require(len(memories) == 1 and 0 <= memories[0][0] <= memories[0][1] == 1024,
            'one own bounded64MiB memory required')
    imported = len(imports)
    def signature(name):
        require(name in exports, 'actual required scalar export missing: ' + name)
        kind, index = exports[name]
        require(kind == 0 and imported <= index < imported + len(functions), 'actual local function export required')
        ordinal = index - imported
        require(functions[ordinal] < len(signatures), 'actual function type index invalid')
        return signatures[functions[ordinal]], ordinal
    entry, ordinal = signature(command['entry_export'])
    require(entry == ([], []), 'actual same-main entry must be local void()')
    require(exports.get('memory') == (2, 0), 'actual memory0 export required')
    return {'artifact': pin, 'imports': imports, 'exports': sorted(exports), 'memory_pages': memories,
            'implicit_start': False, 'entry_export': command['entry_export'], 'local_entry': ordinal,
            'full_validation': 'SEPARATELY_BOUND_COMPLETED_PRODUCER_RECEIPT'}


def runtime_profiles(frame, evidence, selection):
    report = evidence.selected(selection['reports']['workers'])
    clean_build(report, 'PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED')
    require(report.get('kind') == 'native-interpreter-four-profile-build' and
            report.get('runtime_backend') == 'uwvm-interpreter' and report.get('no_reuse') is True and
            report.get('reused_products') == 0, 'actual fresh interpreter profile producer required')
    source, frozen = compiled_source(frame, evidence, report)
    result, observed = {}, []
    for name, versions in (('ros1', {1}), ('dual', {1, 2})):
        rows = [item for item in report['profiles'] if item['profile'] == name]
        require(len(rows) == 1, 'one actual compiled Runtime profile required')
        row = rows[0]
        binary = evidence.pin(row['binary'])
        require(binary == frame.inputs[selection['workers'][name]] and row['status'] == 'BUILT_NOT_EXECUTED' and
                row['full_dependencies_verified'] is True and row['no_reuse'] is True and row['reused_products'] == 0 and
                len(row['objects']) == 3, 'selected exact three-TU actual interpreter required')
        evidence.retained(row)
        require(type(row['custom_header_inputs']) is list and row['custom_header_inputs'], 'actual custom registry MD headers required')
        manifests = [pin for pin in row['custom_header_inputs'] if Path(pin['path']).name == 'manifest.hpp']
        require(len(manifests) == 1, 'one exact compiled custom registry entry header required')
        header = evidence.pin(manifests[0])
        relations = []
        for item in row['objects']:
            command = report['commands'][item['command_index']]
            argv = command['argv']
            require(command['status'] == 'PASS' and command['returncode'] == 0 and
                    all(flag in argv for flag in ('-MD', '-MF', '-MT', '-c', '-o', '-UNDEBUG')) and '-MMD' not in argv and
                    argv[argv.index('-c') + 1] == item['source']['path'] and argv[argv.index('-o') + 1] == item['object']['path'] and
                    argv[argv.index('-MF') + 1] == item['dependency_file']['path'] and
                    argv[argv.index('-MT') + 1] == item['object']['path'], 'actual full-MD Worker compile association required')
            macros = {version for version in (1, 2) if '-DUWVM_IMPORT_ROS' + str(version) in argv}
            require(macros == versions and not any(token.startswith(('-DUWVM_IMPORT_ROS1=', '-DUWVM_IMPORT_ROS2=',
                    '-UUWVM_IMPORT_ROS', '@')) for token in argv), 'unambiguous actual compiled ROS profile required')
            custom = [token for token in argv if token.startswith('-DUWVM_ROS_CUSTOM_TOPIC_MANIFEST=')]
            require(custom == ['-DUWVM_ROS_CUSTOM_TOPIC_MANIFEST="' + header['path'] + '"'] and
                    manifests[0] in item['dependencies'], 'each actual TU must consume the exact quoted registry header')
            evidence.retained(command)
            for dependency in item['dependencies']:
                relation = evidence.source_counterpart(dependency, source, frozen, mandatory=True)
                if relation is not None:
                    relations.append(relation)
        result[name] = {'binary': binary, 'registry': header, 'custom_header_inputs': row['custom_header_inputs'],
                        'original_source': str(source), 'original_source_manifest_sha256': report['source_manifest_sha256']}
        observed.append({**result[name], 'profile': name, 'compiled_backends': sorted(versions),
                         'actual_source_relations': relations, 'raw_producer_rewritten': False})
        frame.workers[name] = Path(binary['path'])
    frame.report['actual_compiled_Runtime_profiles'] = observed
    return result


def clean_build(report, status, preservation=('source_preserved', 'inputs_preserved', 'products_preserved')):
    """Require an actual terminal producer and complete owned cleanup."""
    require(report.get('status') == status and not report.get('error') and not report.get('errors') and
            not report.get('finalization_errors') and all(report.get(name) is True for name in preservation),
            'completed preserved legacy prerequisite required')
    cleanup = report.get('cleanup')
    cleanup = [cleanup] if type(cleanup) is dict else cleanup
    require(type(cleanup) is list and cleanup and all(row.get('status') == 'PASS' and
            not row.get('remaining') and not row.get('errors') for row in cleanup),
            'legacy prerequisite owned cleanup must be complete')


def compiled_source(frame, evidence, report):
    """Keep the real compilation freeze and verify its complete source again.

    An initial build is fresh; a later harness-only freeze is not a new build.
    Actual compiler-consumed dependencies are checked separately below against
    both complete manifests. Any changed consumed source/header requires a
    new build. Documentation or unrelated harness changes confer no authority
    and do not rewrite the original source labels or preparation status.
    """
    if report.get('source') == str(frame.source):
        require(report.get('source_manifest_sha256') == frame.report['source_manifest_sha256'],
                'one physical source cannot claim a different complete immutable manifest')
        if 'source_before' in report:
            require(report['source_before'] == frame.source_before, 'fresh original source facts differ')
        return frame.source, frame.frozen
    return evidence.source(report)


def consumed_sources(evidence, dependencies, source, frozen):
    """Compare only actual consumed project source, never relabel the receipt."""
    require(type(dependencies) is list and dependencies, 'complete actual consumed input list required')
    relations = []
    for old in dependencies:
        relation = evidence.source_counterpart(old, source, frozen, mandatory=True)
        if relation is not None:
            relations.append(relation)
    return relations


def commands(frame, api, evidence, selection, generation_pin):
    """Bind six complete freshly compiled commands and their real validators."""
    report = evidence.selected(selection['reports']['commands'])
    clean_build(report, COMMAND_STATUS, ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved'))
    require(report.get('kind') == 'uwvm-unitree-legacy-same-main-command-products-v1' and
            report.get('actual_Runtime_cases_executed') == 0, 'six legacy binary-only products required')
    source, frozen = compiled_source(frame, evidence, report)
    recipes, _ = frame.common.document(frame.source / 'sdk/robots/unitree/recipes/legacy-command-recipes.json')
    require(recipes['kind'] == 'uwvm-unitree-legacy-same-main-command-recipes-v1' and
            not recipes['default_authority'] and len(recipes['products']) == 6, 'complete maintained six-language recipes required')
    expected = {row['id']: row for row in recipes['products']}
    exact(selection['products'], expected, 'selection must bind exactly the six maintained legacy commands')
    require(type(report.get('products')) is list and len(report['products']) == 6, 'complete six-command build inventory required')
    built = {row['id']: row for row in report['products']}
    exact(built, expected, 'actual legacy product inventory differs from maintained recipes')
    result = []
    for identifier, recipe in expected.items():
        row, chosen = built[identifier], selection['products'][identifier]
        exact(chosen, ('binary', 'receipt', 'worker_profile'), 'closed actual legacy command selection required')
        require(chosen['worker_profile'] in ('ros1', 'dual') and row['language'] == recipe['language'] and
                row['family'] == FAMILY and row['backend'] == 1 and row['entry_export'] == recipe['entry_export'] and
                row['status'] == 'COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED' and
                row['same_main_source_control'] is True and row['preloaded_application'] is False and
                row['cross_module_guard_modified'] is False and row['granted_rights'] == [],
                'fresh actual same-main ROS1 command provenance differs')
        evidence.retained(row)
        artifact, receipt_pin = evidence.pin(row['artifact']), evidence.pin(row['receipt'])
        require(artifact == frame.inputs[chosen['binary']] and receipt_pin == frame.inputs[chosen['receipt']] and
                evidence.pin(row['generation']) == generation_pin, 'actual command/provider/receipt association differs')
        receipt = evidence.document(row['receipt'])
        require(receipt.get('format') == 'uwvm-unitree-legacy-same-main-command-guest-build-receipt-v1' and
                receipt.get('observed_execution') == 'NOT_EXECUTED' and receipt.get('artifact') == row['artifact'] and
                receipt.get('id') == identifier and receipt.get('generation') == row['generation'],
                'original legacy command receipt differs')
        evidence.retained(receipt)
        provenance = row['compilation_provenance']
        manifest = evidence.pin(provenance['original_manifest'])
        original_state = provenance['complete_original_source_state']
        require(provenance['original_source'] == str(source) and
                provenance['source_manifest_sha256'] == report['source_manifest_sha256'] == manifest['sha256'] and
                original_state == {'root': str(source), 'manifest': provenance['original_manifest'], 'files': len(frozen)} and
                (report.get('source_before', {}).get('manifest') == provenance['original_manifest'] or
                 provenance['original_manifest'] in report.get('input_pins', [])),
                'Guest must preserve its actual complete original compilation source and manifest')
        relations = consumed_sources(evidence, provenance['consumed_project_inputs'], source, frozen)
        require(relations, 'actual Guest project-source consumption must be recorded')
        source_bindings, generated_bindings = row['source_bindings'], row['generated_bindings']
        require(type(source_bindings) is list and source_bindings and type(generated_bindings) is list and generated_bindings,
                'complete actual staged source/provider copy relations required')
        consumed = {pin['path']: evidence.pin(pin) for pin in provenance['consumed_project_inputs']}
        for binding in source_bindings:
            exact(binding, ('relative', 'original', 'compiled'), 'closed compiler-source copy binding required')
            relative = Path(binding['relative'])
            require(not relative.is_absolute() and '..' not in relative.parts and str(relative) == binding['relative'] and
                    binding['original']['path'] == str(source / relative), 'staged source must name its exact original MF relative path')
            original, compiled = evidence.pin(binding['original']), evidence.pin(binding['compiled'])
            relation = evidence.source_counterpart(binding['original'], source, frozen, mandatory=True)
            require(relation is not None and all(original[key] == compiled[key] for key in ('bytes', 'sha256')),
                    'explicit staged source package copy differs from its original/current manifests')
        old_generation = evidence.document(row['generation'])
        copied_names = set()
        for binding in generated_bindings:
            exact(binding, ('name', 'generation', 'original', 'compiled'), 'closed actual generated-provider copy binding required')
            name = binding['name']
            require(type(name) is str and Path(name).name == name and name in old_generation['outputs'] and
                    binding['generation'] == row['generation'], 'generated copy must bind this actual complete generation')
            original, compiled = evidence.pin(binding['original']), evidence.pin(binding['compiled'])
            require(original['path'] == str(Path(row['generation']['path']).parent / name) and
                    all(original[key] == compiled[key] == old_generation['outputs'][name][key] for key in ('bytes', 'sha256')),
                    'actual staged generated-provider bytes differ')
            copied_names.add(name)
        require(copied_names == set(old_generation['outputs']), 'all eleven complete generated modules must retain copy bindings')
        require(str(source / recipe['actual_example']) in consumed, 'actual original language example missing from consumed source closure')
        validation = row['validation']
        require(validation['status'] == 'PASS' and validation['returncode'] == 0 and
                len(validation['argv']) == 3 and validation['argv'][1:] == ['validate', artifact['path']],
                'actual full Wasm validator success required')
        result.append({'recipe': recipe, 'artifact': artifact, 'receipt': receipt_pin,
            'boundary': binary_boundary(frame, api, Path(artifact['path']), recipe),
            'worker_profile': chosen['worker_profile'], 'compilation_provenance': provenance,
            'original_source': str(source), 'original_source_manifest_sha256': report['source_manifest_sha256'],
            'consumed_source_relations': relations})
    require({row['worker_profile'] for row in result} == {'ros1', 'dual'}, 'both actual ROS1 and dual Workers must execute')
    return result


def native_products(frame, api, native, evidence, selection):
    """Reopen the new Noetic provider, compiler MD, link trace and complete COPY."""
    report = evidence.selected(selection['reports']['native_products'])
    clean_build(report, 'PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED',
                ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved',
                 'execution_view_preserved', 'original_sdk_preserved',
                 'original_official_upstream_preserved', 'official_source_copy_unmodified', 'acquisition_source_preserved'))
    require(report.get('schema') == 'uwvm-unitree-noetic-native-products-v1' and
            report.get('qualified_primary_translation_units') == 3 and report.get('native_finalization_errors') == [] and
            report.get('all_target_products_fresh') is True and report.get('metadata_export_executed') is True and
            report.get('native_contexts_executed') == 0 and report.get('ROS_TCPROS_executed') is False and
            report.get('Runtime_executed') is False and report.get('Guest_rights_changed') is False,
            'new honest three-primary-TU Noetic producer required')
    source, frozen = compiled_source(frame, evidence, report)
    upstreams = [row for row in frame.config['upstreams'].values()
        if row['repository'].removeprefix('https://github.com/').removesuffix('.git') ==
            'unitreerobotics/unitree_ros_to_real' and row['commit'] == 'b989870124913091fbe75e0dcfb047eb4ca00a09']
    require(len(upstreams) == 1, 'one actual complete Git-tree/all-blob verified original legacy upstream required')
    original_upstream = Path(upstreams[0]['root'])
    exact(report['products'], ('gateway', 'legacy_peer', 'identity_exporter'), 'three distinct native products required')
    support = api.frozen_module(frame.source, frame.frozen, 'tests/ros_unitree/noetic_native_support.py', 'legacy_native_build_recheck')
    decoder = api.frozen_module(frame.source, frame.frozen,
        'tests/ros_unitree/acceptance/native74_r6/native_producer.py', 'legacy_native_ELF_decoder')
    views = report['execution_view_after']
    exact(views, ('sdk', 'work'), 'complete independent Noetic SDK/work views required')
    for name, old in views.items():
        root = Path(old['root'])
        require(root.is_relative_to(frame.literal_native_root) and
                any(root == selected or root.is_relative_to(selected) for selected in frame.roots),
                'entire immutable Noetic execution view must belong to the exact literal producer root')
        evidence.pin(old['catalog'])
        current = support.tree(frame.common, root, frame.out / ('native-view-current-' + name + '.jsonl'), frame.guard)
        require({k: v for k, v in old.items() if k != 'catalog'} ==
                {k: v for k, v in current.items() if k != 'catalog'}, 'complete native SDK/work physical tree changed')
    sdk, work = Path(views['sdk']['root']), Path(views['work']['root'])
    source_roles = {'gateway': 'gateways/ros1/main.cc', 'legacy_peer': 'tests/ros_unitree/native_legacy_ros1_peer.cpp',
                    'identity_exporter': 'sdk/integrations/unitree/native_identity.cpp'}
    native_relations = {}
    for name, row in report['products'].items():
        evidence.retained(row)
        actual = support.build_proof(frame, sdk, work, Path(row['compile_database']['path']).parent,
            {'stdout': row['verbose_stdout']['path'], 'stderr': row['verbose_stderr']['path']},
            Path(row['primary_source']['path']), Path(row['binary']['path']), decoder.elf, compiled_source=source)
        require(all(actual[key] == value for key, value in row.items() if key != 'original_frozen_source') and
                row['primary_translation_units'] == 1, 'actual native compile/MD/link/ELF receipt differs')
        original = row.get('original_frozen_source', row['primary_source'])
        wanted = frame.pin(source / source_roles[name])
        require(evidence.pin(original) == wanted, 'native role must preserve its actual original compiled source')
        require(evidence.source_counterpart(original, source, frozen, mandatory=True) is not None,
                'native primary source differs from the current reviewed source')
        native_relations[name] = consumed_sources(evidence, row['dependencies'], source, frozen)
        if name == 'identity_exporter':
            require(all(row['primary_source'][key] == wanted[key] for key in ('bytes', 'sha256')),
                    'copied exporter primary source bytes differ')
    family = report['family']
    require(family['name'] == FAMILY and family['ros_version'] == 1 and family['message_count'] == 11,
            'complete original eleven-message ROS1 family required')
    evidence.retained(family)
    profile_relation = evidence.source_counterpart(family['profile'], source, frozen, mandatory=True)
    require(profile_relation is not None and Path(family['profile']['path']) ==
            source / 'sdk/robots/unitree/profiles/unitree_legged_msgs.json',
            'exact reviewed original legacy family profile required')
    exported = evidence.document(family['native_metadata'], 4 * 1024**2)
    exact(exported, ('format', 'ros_version', 'types'), 'closed actual native metadata required')
    require(exported['format'] == 'uwvm-ros-official-bounded-native-v2' and exported['ros_version'] == 1 and
            set(exported['types']) == {FAMILY + '/' + name for name in MESSAGES}, 'actual exporter must cover all eleven ROS1 types')
    identities = {name: row['native_identity'] for name, row in exported['types'].items()}
    require(all(re.fullmatch('[0-9a-f]{32}', value) for value in identities.values()), 'actual ROS1 MD5 identities required')
    mapping = family['native_export_mapping']
    require(mapping['pidfd_owned'] is True and mapping['returncode'] == 0 and mapping['all_file_mappings'] and
            mapping['gate'] == 'COOPERATIVE_SIGSTOP_SAME_BIRTH_PIDFD_CONT_THEN_SINGLE_I' and
            mapping['actual_executable'] == report['products']['identity_exporter']['binary'] and
            mapping['stdout'] == family['native_metadata'], 'genuine opened stopped native exporter proof required')
    generation = evidence.document(family['generation'])
    require(generation['format'] == 'uwvm-ros-custom-bounded-v2' and generation['status'] == 'GENERATED_SOURCE_ONLY' and
            generation['native_registry_emitted'] is True and generation['implicit_permissions'] == [] and
            len(generation['records']) == len(generation['types']) == 11 and
            {row['type']: row['native_identity'] for row in generation['records']} == identities and
            {row['type']: row['native_identity'] for row in generation['types']} == identities and
            all(row['ros_version'] == 1 and 0 <= row['minimum_wire_bytes'] <= row['maximum_wire_bytes'] <= 1536
                for row in generation['records']), 'complete newly native-bound ROS1 generator output required')
    require(generation['generator_sha256'] == frame.frozen['sdk/tools/custom_types.py']['sha256'] and
            generation['bounded_generator_sha256'] == frame.frozen['sdk/custom-types/bounded_profile.py']['sha256'],
            'generation must bind this complete reviewed generator source')
    codecs = {row['type']: row['adapter_codec_identity'] for row in generation['records']}
    require(all(re.fullmatch('UWVMROS1_[0-9a-f]{64}', value) for value in codecs.values()) and
            all(row['adapter_codec_identity'] == codecs[row['type']] for row in generation['types']),
            'actual compiled canonical adapter identities required')
    generation_outputs(frame, generation, family['generated_directory'])
    require(set(family['all_generated_outputs']) == set(generation['outputs']), 'every generated language/provider file must remain present')
    for name, old in family['all_generated_outputs'].items():
        pin = evidence.pin(old)
        require(pin['path'] == str(Path(family['generated_directory']) / name) and
                generation['outputs'][name] == {key: pin[key] for key in ('bytes', 'sha256')}, 'generated output association differs')
    catalog = evidence.document(family['catalog'])
    require(catalog['format'] == 'uwvm-ros-custom-bounded-v2' and
            {row['type']: row['native_identity'] for row in catalog['types']} == identities and
            all(row['ros_version'] == 1 for row in catalog['types']), 'bound catalog and actual exported identities differ')
    binding = evidence.document(family['binding_receipt'])
    require(binding['status'] == 'NATIVE_IDENTITY_BINDING_SOURCE_ONLY' and binding['implicit_permissions'] == [] and
            binding['official_export'] == {key: family['native_metadata'][key] for key in ('path', 'bytes', 'sha256')} and
            binding['output'] == {key: family['catalog'][key] for key in ('path', 'bytes', 'sha256')}, 'original native binding receipt differs')
    for definition in generation['source_inputs']['definitions']:
        relative = Path(definition['file'])
        require(not relative.is_absolute() and '..' not in relative.parts, 'safe original ROS1 definition path required')
        actual = frame.pin(Path(family['catalog']['path']).parent / relative)
        original = frame.pin(original_upstream / relative)
        require(all(actual[key] == definition[key] == original[key] for key in ('bytes', 'sha256')),
                'original Git-verified complete ROS1 message body differs from native-bound definition')
    gateway = report['products']['gateway']
    for output in ('manifest.hpp', 'native_ros1.hpp', 'bounded_records.h'):
        require(family['all_generated_outputs'][output] in gateway['dependencies'], 'actual Gateway full MD lacks complete generated ' + output)
    files = native.file_catalog(frame, evidence.pin(report['execution_file_catalog']))
    elfs_document = evidence.document(report['execution_elf_catalog'], 64 * 1024**2)
    require(type(elfs_document) is list and 1 <= len(elfs_document) <= 8192, 'complete bounded native ELF catalog required')
    elfs = []
    for image in elfs_document:
        pin = evidence.pin(image['input'])
        require(image == decoder.elf(frame.common, Path(pin['path'])) and pin in files, 'actual ELF catalog member differs')
        elfs.append(pin)
    launch = report['launch']
    exact(launch, ('gateway', 'legacy_peer', 'master'), 'closed direct native launch selection required')
    for role in ('gateway', 'legacy_peer'):
        exact(launch[role], ('executable', 'arguments'), 'direct native argv contract required')
        require(launch[role]['executable'] == report['products'][role]['binary'] and launch[role]['arguments'] == [],
                'native peer/Gateway must launch actual linked binaries directly')
    master = launch['master']
    exact(master, ('executable', 'arguments', 'python', 'script', 'port_placeholder'), 'closed genuine Focal master launch required')
    evidence.retained(master)
    expected_args = ['--library-path', report['environment']['LD_LIBRARY_PATH'], master['python']['path'],
                     master['script']['path'], '--core', '-p', '{port}']
    require(master['arguments'] == expected_args and master['port_placeholder'] == '{port}' and
            Path(master['python']['path']).name == 'python3.8' and Path(master['script']['path']).name == 'rosmaster' and
            all(evidence.pin(master[role]) in files for role in ('executable', 'python', 'script')),
            'master must use selected copied Focal loader/Python3.8/script')
    for row in report['products'].values():
        require(row['ELF']['PT_INTERP'] == [master['executable']['path']] and
                row['ELF']['DT_RPATH'] == [report['environment']['LD_LIBRARY_PATH']] and row['ELF']['DT_RUNPATH'] == [],
                'native direct loader and absolute selected RPATH differ')
    require(report['environment']['PATH'] == ':'.join(dict.fromkeys(str(support.rooted(sdk, name))
            for name in ('/usr/bin', '/bin'))), 'native PATH must select only actual copied SDK binary directories')
    frame.report['native_original_compilation_sources'] = {'root': str(source),
        'source_manifest_sha256': report['source_manifest_sha256'], 'actual_consumed_source_relations': native_relations,
        'current_runner_source': str(frame.source), 'original_labels_and_raw_report_preserved': True}
    return {'report': report, 'family': family, 'generation': evidence.pin(family['generation']),
            'identities': identities, 'codecs': codecs, 'identity_order': [FAMILY + '/' + name for name in MESSAGES],
            'gateway': evidence.pin(gateway['binary']), 'peer': evidence.pin(report['products']['legacy_peer']['binary']),
            'launch': launch, 'file_catalog': files, 'elf_catalog': elfs}


def exec_gate(frame, api, evidence, selection):
    """Retain only the independent held-exec role from actual Native74 evidence.

    Its ROS2 package/product status confers no ROS1 authority or compatibility.
    An at-rest admission is required if the original batch itself failed; the
    original failure is read unchanged, never converted to a completed build.
    """
    report = evidence.selected(selection['reports']['exec_gate'])
    if report.get('schema') == 'uwvm-native74-retained-products-at-rest-v1':
        retained = api.frozen_module(frame.source, frame.frozen,
            'tests/ros_unitree/native_retained_products.py', 'legacy_gate_retained_role_contract')
        clean_build(report, retained.STATUS, ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved', 'products_preserved'))
        require(report['execution_performed'] is False and report['compilation_performed'] is False and
                report['original_statuses'] == {'inner': 'FAIL', 'outer': 'FAIL'}, 'distinct retained gate content admission required')
        roles = [row for row in report['retained_compilation']['roles'] if row['role'] == 'exec-gate']
        require(len(roles) == 1 and roles[0]['translation_units'] == 1 and roles[0]['actual_ELF_reparsed'] is True,
                'exact complete actual held-gate role required')
        evidence.retained(roles[0])
        raw = evidence.document(report['original_raw_inner'])
        require(raw['status'] == 'FAIL' and raw['source'] == report['retained_compilation']['original_source'],
                'original failed batch must remain historical failure')
        gate = evidence.pin(roles[0]['binary'])
    else:
        clean_build(report, 'PREPARED_NATIVE_PRODUCTS_NOT_EXECUTED')
        raw, gate = report, evidence.pin(report['exec_gate'])
    source, frozen = evidence.source(raw)
    require(raw['exec_gate_source'] == 'tests/ros_javascript_guest/exec_gate.cpp' and
            raw['exec_gate_full_dependencies_verified'] is True and len(raw['exec_gate_compile_commands']) == 1 and
            gate == evidence.pin(raw['exec_gate']) == frame.inputs[selection['exec_gate']], 'actual selected held gate differs')
    section = raw['exec_gate_compile_commands'][0]
    records = section['compile_records']
    require(len(records) == 1, 'one actual held-gate C++ TU required')
    full = evidence.document(records[0]['full_MD_receipt']) if 'full_MD_receipt' in records[0] else records[0]
    evidence.retained(full)
    require(full['source'] in full['dependencies'] and Path(full['source']['path']) == source / 'tests/ros_javascript_guest/exec_gate.cpp' and
            all(flag in full['actual_compile']['argv'] for flag in ('-MD', '-MF', '-MT', '-c', '-o', '-UNDEBUG')),
            'complete original held-gate source/MD evidence required')
    evidence.source_counterpart(full['source'], source, frozen, mandatory=True)
    return gate


def policy(identities, port, positive, codecs):
    """Return the complete literal ROS1 test policy; no configurable grant argv."""
    connection = {'node-name': 'legacy_gateway', 'namespace': '/uwvm_unitree_legacy',
        'master-uri': 'http://127.0.0.1:%d' % port, 'advertise-host': '127.0.0.1', 'transport': 'tcpros'}
    grants = []
    if positive:
        for alias, operation, resource, name in (
                ('legacy-lowstate', 'topic.subscribe', 'lowstate', 'LowState'),
                ('legacy-lowcmd', 'topic.publish', 'lowcmd', 'LowCmd')):
            limits = {'adapter-codec-identity': codecs[FAMILY + '/' + name],
                'max-message-bytes': '1536', 'max-rate-hz': '100', 'burst': '1',
                'max-bytes-per-second': '153600', 'max-queue-bytes': '1536', 'queue-depth': '1',
                'transport': 'tcpros', 'overflow-policy': 'reject', 'validator': 'none',
                'audit': 'allow-and-deny', 'audit-max-rate-hz': '100', 'audit-burst': '16',
                'audit-max-records': '10000', 'audit-max-bytes': '1048576',
                'max-publishers' if operation == 'topic.publish' else 'max-subscribers': '1'}
            if operation == 'topic.publish':
                limits['latch'] = 'true'
            grants.append({'alias': alias, 'operation': operation,
                'resource': '/uwvm_unitree_legacy/' + resource, 'type': FAMILY + '/' + name,
                'identity': identities[FAMILY + '/' + name], 'limits': limits})
    return connection, grants


def admit(frame, api, native, selection):
    """Close every executable/provider/input relation before namespace creation."""
    exact(selection, ('format_version', 'kind', 'reports', 'workers', 'products', 'exec_gate',
        'worker_catalog_roots', 'mappings', 'parent_network_namespace', 'master_port', 'restart_mappings'),
        'closed actual ROS1 legacy transport selection required')
    require(type(selection['format_version']) is int and selection['format_version'] == 1 and
            selection['kind'] == 'uwvm-unitree-legacy-same-main-tcpros-selection-v1' and
            type(selection['master_port']) is int and 1024 <= selection['master_port'] <= 65524 and
            re.fullmatch(r'net:\[[1-9][0-9]*\]', selection['parent_network_namespace']) and
            os.readlink('/proc/self/ns/net') == selection['parent_network_namespace'],
            'actual parent namespace and finite unprivileged loopback port required')
    exact(selection['reports'], ('native_products', 'workers', 'commands', 'exec_gate'), 'four explicit actual prerequisite reports required')
    exact(selection['workers'], ('ros1', 'dual'), 'fresh actual ROS1 and dual profiles required')
    exact(selection['mappings'], ('master', 'peer', 'peer_context', 'gateway', 'gateway_context', 'gate', 'worker_ros1', 'worker_dual'),
          'all actual executable/SDK mapping roles required')
    for labels in selection['mappings'].values():
        require(type(labels) is list and 1 <= len(labels) <= 32 and len(set(labels)) == len(labels) and
                all(label in frame.inputs for label in labels), 'explicit mapped SDK input labels required')
    shared = api.frozen_module(frame.source, frame.frozen, 'tests/ros_unitree/typed_transport_inputs.py', 'legacy_shared_content_admission')
    evidence = shared.Evidence(api, frame, selection['restart_mappings'])
    workers = runtime_profiles(frame, evidence, selection)
    prepared = native_products(frame, api, native, evidence, selection)
    for profile, worker in workers.items():
        require({key: worker['registry'][key] for key in ('bytes', 'sha256')} ==
                {key: prepared['family']['registry'][key] for key in ('bytes', 'sha256')},
                'actual Worker registry must equal the complete newly native-bound eleven-type registry')
    products = commands(frame, api, evidence, selection, prepared['generation'])
    gate = exec_gate(frame, api, evidence, selection)
    require(str(frame.literal_native_root) not in selection['worker_catalog_roots'],
            'copied native SDK is not a host Worker/gate catalog provider')
    worker_files, worker_elfs = host_catalog(frame, api, selection['worker_catalog_roots'])
    by_path = {pin['path']: pin for pin in worker_files}
    for product in products:
        prior = by_path.setdefault(product['artifact']['path'], product['artifact'])
        require(prior == product['artifact'], 'actual Wasm mapping pin conflicts with complete Worker catalog')
    worker_files = list(by_path.values())
    base = prepared['report']['environment']
    exact(base, ('LD_LIBRARY_PATH', 'PYTHONHOME', 'PYTHONPATH', 'ROS_PACKAGE_PATH', 'CMAKE_PREFIX_PATH', 'ROS_DISTRO', 'PATH'),
          'closed independent copied Noetic environment required')
    require(base['ROS_DISTRO'] == 'noetic', 'explicit native Noetic environment required')
    for name in ('LD_LIBRARY_PATH', 'PYTHONHOME', 'PYTHONPATH', 'ROS_PACKAGE_PATH', 'CMAKE_PREFIX_PATH', 'PATH'):
        require(type(base[name]) is str and base[name] and all(Path(value).is_absolute() and
                Path(value).resolve(strict=True) == Path(value) and Path(value).is_dir() and
                any(Path(value) == root or Path(value).is_relative_to(root) for root in frame.roots)
                for value in base[name].split(':')), 'native environment must remain inside complete selected SDK/prefix roots')
    require(frame.config['environment'] == {}, 'host Worker environment must not inherit a copied-native LD/Python environment')
    environment = {**frame.environment, **base, 'ROS_IP': '127.0.0.1',
        'ROS_MASTER_URI': 'http://127.0.0.1:%d' % selection['master_port'], 'PYTHONNOUSERSITE': '1',
        'LANG': 'C', 'LC_ALL': 'C'}
    python_pin = prepared['launch']['master']['python']
    require(any(frame.inputs[label] == python_pin for label in selection['mappings']['master']),
            'live master mapping proof must explicitly include the selected copied Focal Python image')
    frame.report['admission'] = {'status': 'ADMITTED_CONTENT_ONLY_NOT_TCPROS_EXECUTED',
        'native_report': frame.inputs[selection['reports']['native_products']],
        'worker_report': frame.inputs[selection['reports']['workers']],
        'command_report': frame.inputs[selection['reports']['commands']],
        'held_gate_report': frame.inputs[selection['reports']['exec_gate']],
        'native_identities': prepared['identities'], 'restart_file_bridges': evidence.bridges,
        'backend': 'ros1', 'original_native_primary_translation_units': 3,
        'original_native_source': prepared['report']['source'],
        'original_native_source_manifest_sha256': prepared['report']['source_manifest_sha256'],
        'original_compilation_labels_and_statuses_preserved': True, 'transport_execution': 'NOT_STARTED'}
    frame.save()
    return {**prepared, 'products': products, 'exec_gate': gate, 'environment': environment,
        'mappings': selection['mappings'], 'port': selection['master_port'], 'policy': policy,
        'worker_file_catalog': worker_files, 'worker_elf_catalog': worker_elfs}


