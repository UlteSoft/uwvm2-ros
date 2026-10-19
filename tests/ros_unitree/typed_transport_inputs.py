#!/usr/bin/env python3
"""Fail-closed admission for actual same-main typed DDS acceptance inputs.

The selection document contains *input labels*, never guessed executable pins
or caller-supplied native identities. Original producer JSON is read without
rewriting it. A restart can change only an explicitly mapped filesystem device;
the complete bytes, physical path, inode and both timestamps must still agree.
Every admitted current file remains in Frame's before/after pin ledger.

Build preparation is evidence of compilation only. None of the admission
functions below can produce a transport PASS; the separate session module must
observe all28 real Runtime/native lifecycles after these checks finish.
"""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import re
import stat
import xml.etree.ElementTree as ET


PIN = frozenset(('path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'))
FAMILIES = {'unitree_go': (1, 24, 'ros2', 'go-api'),
            'unitree_hg': (2, 11, 'dual', 'hg'),
            'unitree_api': (3, 8, 'ros2', 'go-api')}
BUILD_STATUS = 'PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY'


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


class Evidence:
    """A finite immutable-pin accessor, with separately retained device bridges."""
    def __init__(self, api, frame, mappings):
        self.api, self.frame, self.cache, self.bridges = api, frame, {}, []
        self.mappings = mappings
        require(type(mappings) is list and len(mappings) <= 64, 'finite explicit restart root mappings required')
        seen = set()
        for item in mappings:
            exact(item, ('root', 'historical_device', 'current_device'), 'closed restart root/device mapping required')
            root = Path(item['root'])
            require(root.is_absolute() and root.resolve(strict=True) == root and root.is_dir() and
                    root.stat().st_dev == item['current_device'] and
                    any(selected == root or selected.is_relative_to(root) for selected in frame.roots) and
                    type(item['historical_device']) is int and type(item['current_device']) is int and
                    (item['root'], item['historical_device']) not in seen and
                    item['historical_device'] != item['current_device'],
                    'restart device scope must physically contain explicitly inventoried roots')
            seen.add((item['root'], item['historical_device']))
        self.restart = api.frozen_module(frame.source, frame.frozen,
            'tests/ros_unitree/acceptance/requalify_after_restart.py', 'typed_transport_restart_bridge')
        self.old_sources = {}

    def pin(self, old):
        """Return the actual current full7; never synthesize an old-looking pin."""
        require(type(old) is dict and PIN <= old.keys(), 'actual full seven-field evidence pin required')
        old = {key: old[key] for key in PIN}
        path = Path(old['path'])
        require(path.is_absolute() and path.resolve(strict=True) == path and
                (path.is_relative_to(self.frame.source) or
                 any(path.is_relative_to(root) for root in self.frame.roots)),
                'evidence file must be physical and belong to a complete frozen/inventoried root')
        require(all(type(old[key]) is int and old[key] >= 0 for key in PIN - {'path', 'sha256'}) and
                old['inode'] > 0 and re.fullmatch('[0-9a-f]{64}', old['sha256']), 'malformed full physical pin')
        key = tuple(old[name] for name in sorted(PIN))
        if key in self.cache:
            self.frame.guard()
            return self.cache[key]
        require(len(self.cache) < 100000, 'bounded selected evidence file closure required')
        actual = self.frame.pin(path)
        if actual != old:
            matches = sorted((item for item in self.mappings if path.is_relative_to(Path(item['root'])) and
                              item['historical_device'] == old['device'] and item['current_device'] == actual['device']),
                             key=lambda item: len(Path(item['root']).parts), reverse=True)
            require(matches, 'no explicit device-only restart bridge for file')
            mapping = matches[0]
            bridge = self.restart.requalify_persistent_pin(old, str(path), self.frame.guard,
                historical_device=mapping['historical_device'], current_device=mapping['current_device'],
                observed_current=actual)
            require(bridge['current_pin'] == actual, 'restart bridge must preserve the actual current pin')
            self.bridges.append(bridge)
        self.cache[key] = actual
        return actual

    def document(self, old, maximum=64 * 1024**2):
        pin = self.pin(old)
        raw = self.api.bounded(Path(pin['path']), maximum)
        require(len(raw) == pin['bytes'] and hashlib.sha256(raw).hexdigest() == pin['sha256'],
                'selected metadata differs from complete actual file bytes')
        return decode(raw)

    def selected(self, label, maximum=64 * 1024**2):
        return self.document(self.frame.inputs[label], maximum)

    def retained(self, value):
        """Consume only the explicitly selected receipt/section, never project it.

        A dict with additional semantic fields may contain a full7 pin. Only
        those seven physical fields enter the accessor; the original object
        and its status/cgroup/source labels remain unmodified.
        """
        if type(value) is dict:
            if PIN <= value.keys():
                self.pin(value)
            for item in value.values():
                self.retained(item)
        elif type(value) is list:
            for item in value:
                self.retained(item)

    def source(self, report):
        """Reopen the original *complete* frozen source, with honest provenance."""
        source = Path(report['source'])
        require(source.is_absolute() and source.resolve(strict=True) == source and
                any(source == root or source.is_relative_to(root) for root in self.frame.roots),
                'the entire original compilation source must remain inventoried')
        original = report.get('source_before')
        if original is None:
            # Unitree Frame reports retain the manifest in their actual input
            # ledger rather than a source_before summary. Select that exact
            # sibling and complete digest; never guess a future physical pin.
            candidates = [pin for pin in report['input_pins'] if PIN <= pin.keys() and
                          pin['path'] == str(source.parent / 'source-manifest.json') and
                          pin['sha256'] == report['source_manifest_sha256']]
            candidates = list({json.dumps(pin, sort_keys=True): pin for pin in candidates}.values())
            require(len(candidates) == 1, 'one original complete Frame source manifest pin required')
            original = {'root': str(source), 'manifest': candidates[0]}
        manifest = self.pin(original['manifest'])
        require(manifest['sha256'] == report['source_manifest_sha256'], 'original complete manifest hash differs')
        key = (str(source), manifest['sha256'])
        if key not in self.old_sources:
            current = self.frame.common.source_state(source, Path(manifest['path']))
            require(current['root'] == str(source) and current['manifest'] == manifest and
                    ('files' not in original or current['files'] == original['files']),
                    'original frozen source changed beyond device rebinding')
            frozen = self.document(original['manifest'], 16 * 1024**2)
            self.old_sources[key] = frozen
        return source, self.old_sources[key]

    def source_counterpart(self, pin, source, frozen, *, mandatory=False):
        """Observe a consumed-source byte relation without relabeling a build.

        The actual interpreter may deliberately be an older complete frozen
        build. Differences are recorded and it retains that original source
        label. The native oracle itself is mandatory-byte-equal to this runner's
        reviewed witness, so changed oracle logic requires a fresh compilation.
        """
        actual = self.pin(pin)
        path = Path(actual['path'])
        if not path.is_relative_to(source):
            return None
        relative = str(path.relative_to(source))
        digest = {key: actual[key] for key in ('bytes', 'sha256')}
        require(frozen.get(relative) == digest, 'compiler-consumed source differs from original complete manifest')
        current = self.frame.frozen.get(relative)
        equal = current == digest
        require(equal or not mandatory, 'native witness source changed; fresh native compilation required')
        return {'relative': relative, 'original': actual, 'current_manifest_entry': current,
                'byte_relation': 'SAME_BYTES' if equal else 'DIFFERENT_OR_ABSENT_ORIGINAL_BUILD_LABEL_RETAINED'}


def clean_build(report, status, preservation=('source_preserved', 'inputs_preserved', 'products_preserved')):
    require(report.get('status') == status and not report.get('error') and not report.get('errors') and
            not report.get('finalization_errors') and all(report.get(key) is True for key in
            preservation), 'completed preserved producer required')
    cleanup = report.get('cleanup')
    cleanup = [cleanup] if type(cleanup) is dict else cleanup
    require(type(cleanup) is list and cleanup and all(item.get('status') == 'PASS' and
            not item.get('remaining') and not item.get('errors') for item in cleanup), 'completed owned producer cleanup required')


def binary_boundary(frame, api, path, reactor, command):
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
                for item in abi['modules']['uwvm_ros2_v1']['functions']}
    seen, ros, stdio = set(), set(), set()
    for item in imports:
        key = item['module'], item['name']
        require(item['kind'] == 0 and key not in seen, 'unique actual function-only command imports required')
        seen.add(key)
        signature = item['params'], item['results']
        if key[0] == 'uwvm_ros2_v1':
            require(signature == expected.get(key[1]), 'actual ROS2 ABI signature differs')
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
    core = lambda value: {'u32': 'i32', 'u64': 'i64'}.get(value, value)
    for name, wanted in reactor['exports'].items():
        observed, _ = signature(name)
        require(observed == ([core(v) for v in wanted['parameters']], [core(wanted['result'])]),
                'unchanged actual typed scalar export signature differs')
    entry, ordinal = signature(command['entry_export'])
    require(entry == ([], []), 'actual same-main entry must be local void()')
    require(exports.get('memory') == (2, 0), 'actual memory0 export required')
    return {'artifact': pin, 'imports': imports, 'exports': sorted(exports), 'memory_pages': memories,
            'implicit_start': False, 'entry_export': command['entry_export'], 'local_entry': ordinal,
            'full_validation': 'SEPARATELY_BOUND_COMPLETED_PRODUCER_RECEIPT'}


def commands(frame, api, evidence, selection):
    report = evidence.selected(selection['reports']['commands'])
    clean_build(report, BUILD_STATUS,
                ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved'))
    require(report.get('compiled_command_products') == 14 and report.get('actual_Runtime_cases_executed') == 0 and
            report.get('retained_command_products') == 9 and report.get('fresh_command_products') == 5,
            'actual fourteen binary-only command products required; no transport promotion')
    source, frozen = evidence.source(report)
    recipes, _ = frame.common.document(frame.source / 'tests/ros_unitree/typed_command/recipes.json')
    reactors, _ = frame.common.document(frame.source / recipes['reactor_recipe'])
    require(recipes['command_products'] == 14 and recipes['future_actual_cases'] == 28 and
            not recipes['default_authority'], 'closed maintained command recipe required')
    expected = {item['id']: item for item in recipes['products']}
    reactor_rows = {item['id']: item for item in reactors['products']}
    exact(selection['products'], expected, 'selection must bind exactly all fourteen command products')
    require(type(report.get('products')) is list and len(report['products']) == 14, 'complete producer product inventory required')
    built = {item['id']: item for item in report['products']}
    exact(built, expected, 'completed command inventory differs from maintained recipe')
    result = []
    for identifier, recipe in expected.items():
        row = built[identifier]
        chosen = selection['products'][identifier]
        exact(chosen, ('binary', 'receipt'), 'command selection requires exact input labels')
        require(row['status'] == 'COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED' and
                row['language'] == recipe['language'] and row['family'] == recipe['family'] and
                row['reactor_id'] == recipe['reactor_id'] and row['entry_export'] == recipe['entry_export'] and
                row['same_main_source_control'] is True and row['preloaded_application'] is False and
                row['cross_module_guard_modified'] is False and not row['granted_rights'],
                'actual unchanged same-main command compile receipt required')
        artifact = evidence.pin(row['artifact'])
        receipt_pin = evidence.pin(row['receipt'])
        require(artifact == frame.inputs[chosen['binary']] and receipt_pin == frame.inputs[chosen['receipt']],
                'selected actual command/receipt association differs')
        receipt = evidence.document(row['receipt'])
        require(receipt.get('format') == 'uwvm-unitree-same-main-command-guest-build-receipt-v1' and
                receipt.get('observed_execution') == 'NOT_EXECUTED' and receipt.get('artifact') == row['artifact'] and
                receipt.get('reactor_id') == recipe['reactor_id'], 'original actual command build receipt differs')
        evidence.retained(receipt)
        validation = row['validation']
        argv = validation['argv']
        require(validation['status'] == 'PASS' and validation['returncode'] == 0 and
                len(argv) == 3 and argv[1:] == ['validate', artifact['path']], 'actual completed full binary validator required')
        source_relations = []
        for relative in (recipe['actual_example'], recipe['command_source'], *([recipe['shared_source']] if 'shared_source' in recipe else [])):
            require(relative in frozen and frozen[relative] == frame.frozen.get(relative),
                    'typed command/example source changed; fresh command compilation required')
            source_relations.append({'relative': relative, 'original_manifest_entry': frozen[relative], 'relation': 'SAME_BYTES'})
        actual = binary_boundary(frame, api, Path(artifact['path']), reactor_rows[recipe['reactor_id']], recipe)
        result.append({'recipe': recipe, 'artifact': artifact, 'receipt': receipt_pin, 'boundary': actual,
                       'original_source': str(source), 'original_source_manifest_sha256': report['source_manifest_sha256'],
                       'source_relations': source_relations, 'compilation_provenance': row['compilation_provenance']})
    return result


def runtime_profiles(frame, evidence, selection):
    report = evidence.selected(selection['reports']['workers'])
    clean_build(report, 'PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED')
    require(report.get('kind') == 'native-interpreter-four-profile-build' and
            report.get('runtime_backend') == 'uwvm-interpreter' and report.get('no_reuse') is True and
            report.get('reused_products') == 0, 'actual fresh interpreter profile producer required')
    source, frozen = evidence.source(report)
    result, observed = {}, []
    for name, versions in (('ros2', {2}), ('dual', {1, 2})):
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
                relation = evidence.source_counterpart(dependency, source, frozen)
                if relation is not None:
                    relations.append(relation)
        result[name] = {'binary': binary, 'registry': header, 'custom_header_inputs': row['custom_header_inputs'],
                        'original_source': str(source), 'original_source_manifest_sha256': report['source_manifest_sha256']}
        observed.append({**result[name], 'profile': name, 'compiled_backends': sorted(versions),
                         'actual_source_relations': relations, 'raw_producer_rewritten': False})
        frame.workers[name] = Path(binary['path'])
    frame.report['actual_compiled_Runtime_profiles'] = observed
    return result


def compile_records(evidence, section):
    """Reopen complete per-TU MD receipts, not just summary dependency counts."""
    records = section['compile_records']
    require(type(records) is list and records, 'actual native compile records required')
    observed = []
    for record in records:
        evidence.retained(record)
        if 'full_MD_receipt' in record:
            complete = evidence.document(record['full_MD_receipt'], 32 * 1024**2)
            require(len(complete['dependencies']) == record['dependency_count'] and
                    all(complete[key] == value for key, value in record.items()
                        if key not in ('full_MD_receipt', 'dependency_count')),
                    'complete native MD receipt differs from actual summary')
        else:
            complete = record
        require(type(complete['dependencies']) is list and complete['dependencies'] and
                complete['source'] in complete['dependencies'], 'compiler source absent from full native MD closure')
        evidence.retained(complete)
        argv = complete['actual_compile']['argv']
        require(all(flag in argv for flag in ('-MD', '-MF', '-MT', '-c', '-o')) and '-MMD' not in argv,
                'actual native compiler must emit complete MD, including system headers')
        observed.append(complete)
    return observed


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


def retained_input_catalog(frame, evidence, receipt):
    """Stream a complete new-schema current ledger; authorize no extra path.

    Unconsumed rows are retained finalizer observations. They do not become
    execution rights or bypass Evidence's ordinary complete-root containment.
    Every row actually consumed by native admission is separately reopened by
    that accessor and must equal the pinned ledger's actual current full7.
    """
    require(receipt.get('input_pins_encoding') == 'PINNED_COMPLETE_CURRENT_FULL7_JSONL',
            'explicit complete retained74 current input-ledger encoding required')
    selected = receipt['input_pin_catalog']
    exact(selected, ('pin', 'rows', 'ordering', 'scope'), 'closed retained74 current input catalog required')
    require(type(selected['rows']) is int and 0 < selected['rows'] <= 250000 and
            selected['ordering'] == 'STRICT_PHYSICAL_PATH_ASCENDING_UNIQUE' and
            selected['scope'] == 'ALL_ACTUAL_FRAME_PINS_CHECKED_BY_NORMAL_FINISH_LEDGER_SELF_PIN_RETAINED_INLINE' and
            receipt['input_pins_checked_count'] == selected['rows'] + 1, 'complete checked pin-ledger cardinality required')
    pin = evidence.pin(selected['pin'])
    require(0 < pin['bytes'] <= 256 * 1024**2, 'bounded complete retained74 current pin-ledger bytes required')
    descriptor = os.open(pin['path'], os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    rows, digest, total, previous = {}, hashlib.sha256(), 0, ''
    try:
        first = os.fstat(descriptor)
        require(stat.S_ISREG(first.st_mode) and first.st_size == pin['bytes'], 'actual current input-ledger ordinary descriptor required')
        with os.fdopen(descriptor, 'rb', closefd=False) as stream:
            while True:
                frame.guard()
                raw = stream.readline(16385)
                if not raw:
                    break
                total += len(raw)
                require(len(raw) <= 16384 and raw.endswith(b'\n') and total <= pin['bytes'] and
                        len(rows) < selected['rows'], 'bounded complete ordered input-ledger record required')
                digest.update(raw)
                row = decode(raw)
                exact(row, PIN, 'exact full seven-field current input-ledger row required')
                path = row['path']
                require(type(path) is str and path > previous and Path(path).is_absolute() and
                        str(Path(path)) == path and '..' not in Path(path).parts and
                        not any(char in path for char in ('\0', '\n', '\r')) and
                        all(type(row[key]) is int and row[key] >= 0 for key in PIN - {'path', 'sha256'}) and
                        row['inode'] > 0 and type(row['sha256']) is str and re.fullmatch('[0-9a-f]{64}', row['sha256']),
                        'strict ordered distinct physical current full7 ledger row required')
                rows[path], previous = row, path
        last, named = os.fstat(descriptor), Path(pin['path']).stat(follow_symlinks=False)
        require(total == pin['bytes'] and digest.hexdigest() == pin['sha256'] and len(rows) == selected['rows'] and
                all(getattr(first, field) == getattr(last, field) == getattr(named, field) == pin[key]
                    for field, key in (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                                       ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))),
                'complete current input-ledger bytes or physical descriptor changed')
    finally:
        os.close(descriptor)
    require(pin['path'] not in rows, 'current pin ledger must retain its self-pin inline, without self-reference')
    return pin, rows


def native_products(frame, api, native, evidence, selection):
    label = selection['reports']['native_products']
    admitted = evidence.selected(label)
    admission_pin = frame.inputs[label]
    preparation_mode = 'completed-native-products'
    execution = admitted
    retained_ledger = None
    if admitted.get('schema') == 'uwvm-native74-retained-products-at-rest-v1':
        retained = api.frozen_module(frame.source, frame.frozen,
            'tests/ros_unitree/native_retained_products.py', 'typed_transport_retained74_contract')
        clean_build(admitted, retained.STATUS, ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved'))
        require(admitted.get('preparation_mode') == 'retained74-at-rest' and admitted.get('execution_performed') is False and
                admitted.get('compilation_performed') is False and admitted.get('mount_operations_performed') is False and
                admitted.get('Guest_rights_changed') is False and admitted.get('raw_reports_rewritten') is False and
                admitted.get('products_preserved') is True and admitted.get('original_statuses') == {'inner': 'FAIL', 'outer': 'FAIL'} and
                admitted['original_raw_inner']['sha256'] == retained.INNER_SHA and
                admitted['original_raw_outer']['sha256'] == retained.OUTER_SHA,
                'distinct metadata-only retained74 admission required; failed raw batch remains failed')
        report = evidence.document(admitted['original_raw_inner'])
        outer = evidence.document(admitted['original_raw_outer'])
        require(report.get('status') == outer.get('status') == 'FAIL' and
                report.get('finalization_errors') == outer.get('finalization_errors') == [] and
                admitted['retained_compilation']['translation_units'] == 74 and
                admitted['retained_compilation']['original_source'] == report['source'] and
                admitted['retained_compilation']['original_source_manifest_sha256'] == report['source_manifest_sha256'],
                'retained74 actual raw source/compilation association differs')
        roles = admitted['retained_compilation']['roles']
        require([row['role'] for row in roles] == ['gateway-builtin', 'gateway-go-api', 'gateway-hg', 'portable-peer',
                'string-identity', 'exec-gate', 'typed-peer', 'physics-controller'] and
                [row['translation_units'] for row in roles] == [1, 34, 34, 1, 1, 1, 1, 1] and
                all(row['actual_ELF_reparsed'] is True for row in roles), 'complete retained74 per-role compile/link proof required')
        evidence.retained(roles)
        evidence.retained(admitted['product_pins'])
        at_rest = admitted['at_rest']
        require(at_rest['selected_active_mounts'] == [] and len(at_rest['restored_ordinary_placeholders']) == 4 and
                at_rest['original_active_char_mount_facts'] == 'NOT_RETAINED_NOT_RECONSTRUCTED',
                'fresh at-rest closure cannot invent historical bound character-device facts')
        evidence.retained(at_rest['restored_ordinary_placeholders'])
        retained_ledger = retained_input_catalog(frame, evidence, admitted)
        preparation_mode = 'retained74-at-rest'
    else:
        report = admitted
        clean_build(report, 'PREPARED_NATIVE_PRODUCTS_NOT_EXECUTED')
    require(report.get('schema') == 'uwvm-embedded-js-native-products-v1' and
            report.get('execution_view_preserved') is True and report.get('qualified_source_translation_units') == 74 and
            report.get('typed_peer_protocol') == 'UTN1/M/Q/P/A/Z/F' and
            report.get('typed_peer_executed') is False and report.get('typed_example_cases_qualified') is False,
            'complete actual Native74 preparation required; compilation is not transport')
    source, frozen = evidence.source(report)
    peer = evidence.pin(report['typed_peer']['input'])
    gate = evidence.pin(report['exec_gate'])
    require(peer == frame.inputs[selection['native_peer']] and gate == frame.inputs[selection['exec_gate']] and
            report.get('exec_gate_source') == 'tests/ros_javascript_guest/exec_gate.cpp' and
            report.get('exec_gate_full_dependencies_verified') is True, 'exact compiled real typed witness/test exec gate required')
    evidence.retained(report['exec_gate_compile_commands'])
    typed = compile_records(evidence, report['typed_peer_compile'])
    require(len(typed) == 1 and Path(typed[0]['source']['path']) == source / 'tests/ros_unitree/native_typed_ros2_peer.cpp' and
            '-std=c++20' in typed[0]['actual_compile']['argv'], 'one actual strict C++20 real generated-message witness TU required')
    evidence.source_counterpart(typed[0]['source'], source, frozen, mandatory=True)
    # Gate source also must be the reviewed current held execveat implementation.
    gate_source = source / 'tests/ros_javascript_guest/exec_gate.cpp'
    require(frozen.get(str(gate_source.relative_to(source))) == frame.frozen.get('tests/ros_javascript_guest/exec_gate.cpp'),
            'test-only held exec gate source changed; fresh native compilation required')
    for inventory in execution['execution_view_after']:
        current = [value for value in frame.catalogs if value['root'] == inventory['root']]
        require(len(current) == 1 and {k: v for k, v in current[0].items() if k != 'catalog'} ==
                {k: v for k, v in inventory.items() if k != 'catalog'}, 'actual Native74 complete COPY/prefix inventory differs')
    file_catalog_pin = evidence.pin(execution['execution_file_catalog'])
    catalog = native.file_catalog(frame, file_catalog_pin)
    elfs = [evidence.pin(pin) for pin in execution['execution_elf_catalog']]
    require(1 <= len(elfs) <= 8192, 'bounded complete Native74 physical ELF catalog required')
    base = execution['execution_environment_base']
    require(base == report['execution_environment_base'], 'fresh at-rest catalog cannot change original native environment selection')
    exact(base, ('LD_LIBRARY_PATH', 'AMENT_PREFIX_PATH'), 'closed native COPY environment required')
    for value in base.values():
        require(type(value) is str and value and all(Path(path).is_absolute() and Path(path).resolve(strict=True) == Path(path) and
                Path(path).is_dir() and any(Path(path) == root or Path(path).is_relative_to(root) for root in frame.roots)
                for path in value.split(':')), 'native environment paths must belong to complete immutable inventories')
    profiles = {}
    for name in ('go-api', 'hg'):
        rows = [value for value in report['gateway_profiles'] if value['profile'] == name]
        require(len(rows) == 1 and rows[0]['status'] == 'BUILT_NOT_EXECUTED', 'one actual custom Gateway profile required')
        row = rows[0]
        records = compile_records(evidence, row['compile'])
        require(len(records) == 34, 'all32 custom shards, dispatcher and main must have actual complete native MD')
        profiles[name] = {'binary': evidence.pin(row['binary']), 'records': records,
                          'dependency_paths': {pin['path'] for record in records for pin in record['dependencies']}}
    if retained_ledger is not None:
        ledger_pin, ledger_rows = retained_ledger
        def consumed(value):
            if type(value) is dict:
                if PIN <= value.keys():
                    actual = evidence.pin(value)
                    require(actual == ledger_pin or ledger_rows.get(actual['path']) == actual,
                            'consumed native current physical pin is absent from complete retained74 admission ledger')
                for item in value.values():
                    consumed(item)
            elif type(value) is list:
                for item in value:
                    consumed(item)
        consumed(admitted['retained_compilation']['roles'])
        consumed(admitted['product_pins'])
        consumed(admitted['execution_elf_catalog'])
        consumed(admitted['execution_file_catalog'])
        consumed(admitted['at_rest']['restored_ordinary_placeholders'])
        consumed({'peer': peer, 'gate': gate, 'gateway_profiles': profiles})
        frame.report['native_retained_input_ledger'] = {'pin': ledger_pin, 'rows': len(ledger_rows),
            'validation': 'COMPLETE_STREAM_SHA256_STRICT_FULL7_ROWS_AND_REOPENED_CONSUMED_IDENTITIES',
            'authority': 'NO_ADDITIONAL_PATH_OR_GUEST_PERMISSION'}
    return {'report': report, 'preparation_mode': preparation_mode, 'admission_receipt': admitted, 'admission_pin': admission_pin,
            'source': source, 'frozen': frozen, 'peer': peer, 'exec_gate': gate,
            'environment': {**frame.environment, **base, 'LANG': 'C', 'LC_ALL': 'C'},
            'file_catalog': catalog, 'elf_catalog': elfs, 'profiles': profiles}


def generation_codecs(generation, identities):
    """Bind every Topic codec to the complete already admitted native type map.

    The bounded-v2 format uses its UWVMROS1_ canonical-codec prefix for both
    native backends. This is a codec-format identifier, not a ROS1 capability.
    Neither these metadata checks nor the returned map grant Guest authority.
    """
    require(type(identities) is dict and identities and
            all(type(generation.get(key)) is list and len(generation[key]) == len(identities)
                for key in ('records', 'types')), 'complete generated codec inventories required')
    codecs = {}
    for record in generation['records']:
        name, codec = record.get('type'), record.get('adapter_codec_identity')
        require(name in identities and name not in codecs and record.get('ros_version') == 2 and
                record.get('native_identity') == identities[name] and type(codec) is str and
                re.fullmatch('UWVMROS1_[0-9a-f]{64}', codec),
                'unique actual native Topic record and canonical adapter codec required')
        codecs[name] = codec
    roots = set()
    for row in generation['types']:
        name = row.get('type')
        require(name in codecs and name not in roots and row.get('ros_version') == 2 and
                row.get('native_identity') == identities[name] and
                row.get('adapter_codec_identity') == codecs[name],
                'generated root adapter codec differs from native-bound record')
        roots.add(name)
    require(roots == set(identities), 'complete native/generated adapter codec type set differs')
    return codecs


def family_bindings(frame, api, evidence, selection, workers, prepared):
    """Bind real exporter identities, generated records and compiled headers."""
    report = evidence.selected(selection['reports']['native_interfaces'])
    clean_build(report, 'PREPARED_NATIVE_INTERFACES_IDENTITIES_AND_SOURCE_PROVIDERS')
    source, frozen = evidence.source(report)
    original = prepared['report']['producer_reports']['native']
    require(evidence.pin(original) == frame.inputs[selection['reports']['native_interfaces']],
            'Native74 must consume this exact independently exported native-interface report')
    result = {}
    for name, (code, count, worker_profile, native_profile) in FAMILIES.items():
        chosen = selection['families'][name]
        exact(chosen, ('generation', 'native_export', 'gateway', 'worker_registry_generation',
                      'peer_mappings', 'worker_mappings', 'gateway_mappings', 'gateway_context_mappings'),
              'closed actual native family binding required')
        rows = [item for item in report['families'] if item['family'] == name]
        require(len(rows) == 1 and rows[0]['status'] == 'PREPARED_NATIVE_PACKAGE_IDENTITIES_AND_SOURCE_PROVIDER',
                'actual complete original native package family required')
        family = rows[0]
        evidence.retained({key: family[key] for key in ('binary', 'binding_receipt', 'generation', 'materialization', 'source_selection')})
        generation_pin = evidence.pin(family['generation'])
        require(generation_pin == frame.inputs[chosen['generation']], 'selected actual family generation differs')
        generation = evidence.document(family['generation'])
        candidates = [pin for pin in report['product_pins'] if Path(pin['path']).name == 'native-metadata.json' and
                      Path(pin['path']).is_relative_to(Path(family['derived_tree']['root']))]
        require(len(candidates) == 1 and evidence.pin(candidates[0]) == frame.inputs[chosen['native_export']],
                'unique actual exporter stdout must be physically inside this complete family tree')
        exported = evidence.document(candidates[0], 4 * 1024**2)
        exact(exported, ('format', 'ros_version', 'types'), 'actual native exporter document shape differs')
        require(exported.get('format') == 'uwvm-ros-official-bounded-native-v2' and
                exported.get('ros_version') == 2 and type(exported.get('types')) is dict and
                len(exported['types']) == count and generation['status'] == 'GENERATED_SOURCE_ONLY' and
                generation['native_registry_emitted'] is True and generation['implicit_permissions'] == [] and
                len(generation['records']) == count and len(generation['types']) == count,
                'all actual generated native family definitions/identities required')
        identities = {}
        for record in generation['records']:
            identity = record['native_identity']
            require(record['ros_version'] == 2 and record['type'].startswith(name + '/msg/') and
                    re.fullmatch('RIHS01_[0-9a-f]{64}', identity) and
                    exported['types'][record['type']]['native_identity'] == identity and
                    0 <= record['minimum_wire_bytes'] <= record['maximum_wire_bytes'] <= 65536,
                    'exact actually exported root/record identity or finite message bound differs')
            identities[record['type']] = identity
        require(set(identities) == set(exported['types']), 'native generation/export complete type inventories differ')
        codecs = generation_codecs(generation, identities)
        for record in generation['types']:
            require(record['ros_version'] == 2 and record['native_identity'] == identities[record['type']],
                    'generated root identity differs from actual native export')
        evidence.retained(generation['source_inputs'])
        generation_outputs(frame, generation, Path(generation_pin['path']).parent)
        bound_digest = generation['source_inputs']['catalog']
        bound_candidates = [pin for pin in report['product_pins'] if Path(pin['path']).name == 'catalog.json' and
                            Path(pin['path']).is_relative_to(Path(family['derived_tree']['root'])) and
                            all(pin[key] == bound_digest[key] for key in ('bytes', 'sha256'))]
        require(len(bound_candidates) == 1, 'one actual family-bound catalog must supply this complete generation')
        catalog_pin = evidence.pin(bound_candidates[0])
        catalog = evidence.document(bound_candidates[0])
        require(catalog.get('format') == 'uwvm-ros-custom-bounded-v2' and
                {row['type']: row['native_identity'] for row in catalog['types']} == identities,
                'actual bound source catalog identities differ from exported/generated native family')
        for definition in generation['source_inputs']['definitions']:
            relative = Path(definition['file'])
            require(not relative.is_absolute() and '..' not in relative.parts, 'safe bound native definition path required')
            path = Path(catalog_pin['path']).parent / relative
            require(path.resolve(strict=True) == path and any(path.is_relative_to(root) for root in frame.roots),
                    'native definition must remain ordinary in the complete original bound tree')
            actual = frame.pin(path)
            require(all(actual[key] == definition[key] for key in ('bytes', 'sha256')), 'complete bound native definition changed')
        # The actual full-MD/export/link reports remain original, not reconstructed
        # native identities supplied by this driver or a guest import argument.
        compile_records(evidence, family['export_build'])
        compile_records(evidence, family['official_package_build'])
        gateway = prepared['profiles'][native_profile]
        require(gateway['binary'] == frame.inputs[chosen['gateway']], 'exact compiled selected Gateway profile differs')
        registry_generation = evidence.selected(chosen['worker_registry_generation'])
        require(registry_generation['status'] == 'GENERATED_SOURCE_ONLY' and
                registry_generation['native_registry_emitted'] is True and not registry_generation['implicit_permissions'],
                'actual compiled Worker registry must have complete no-authority generation proof')
        generation_outputs(frame, registry_generation, frame.input(chosen['worker_registry_generation']).parent)
        actual_registry_types = {row['type']: row['native_identity'] for row in registry_generation['records']}
        require(all(actual_registry_types.get(type_name) == identity for type_name, identity in identities.items()),
                'compiled Worker registry identities differ from actual native family')
        registry_codecs = generation_codecs(registry_generation, actual_registry_types)
        require(all(registry_codecs.get(type_name) == codec for type_name, codec in codecs.items()),
                'compiled Worker registry adapter codecs differ from actual native family')
        header = workers[worker_profile]['registry']
        require(registry_generation['outputs']['manifest.hpp'] == {key: header[key] for key in ('bytes', 'sha256')},
                'actual compiled Worker manifest bytes differ from generated registry proof')
        # Gateway and Worker may use separately materialized copies, but both
        # must consume identical generated registry/provider bytes in full MD.
        for output in ('manifest.hpp', 'native_ros2.hpp', 'bounded_records.h'):
            wanted = registry_generation['outputs'][output]
            matching = [pin for record in gateway['records'] for pin in record['dependencies']
                        if Path(pin['path']).name == output and all(pin[key] == wanted[key] for key in ('bytes', 'sha256'))]
            require(matching, 'actual Gateway full MD lacks matching complete generated ' + output)
        for role in ('peer_mappings', 'worker_mappings', 'gateway_mappings', 'gateway_context_mappings'):
            labels = chosen[role]
            require(type(labels) is list and 1 <= len(labels) <= 32 and len(set(labels)) == len(labels) and
                    all(label in frame.inputs for label in labels), 'explicit actual mapped SDK roles required')
        result[name] = {**chosen, 'family_code': code, 'worker_profile': worker_profile,
                        'native_profile': native_profile, 'gateway': gateway['binary'],
                        'identities': identities, 'codec_identities': codecs, 'generation': generation_pin,
                        'original_source': str(source), 'original_source_manifest_sha256': report['source_manifest_sha256']}
    expected_go_api = {**result['unitree_go']['identities'], **result['unitree_api']['identities']}
    for name, expected in (('unitree_go', expected_go_api), ('unitree_api', expected_go_api),
                           ('unitree_hg', result['unitree_hg']['identities'])):
        document = evidence.selected(selection['families'][name]['worker_registry_generation'])
        require({row['type']: row['native_identity'] for row in document['records']} == expected,
                'compiled custom registry must contain exactly GO+API or exactly HG, without extra types')
    return result


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


def environment(frame, api, selection):
    """Admit a Root-owned loopback-only namespace and a pinned Cyclone profile."""
    expected = selection['expected_network_namespace']
    # An inherited sysfs mount can describe the original host interfaces after
    # unshare(CLONE_NEWNET). Query the current network namespace through the
    # reviewed native socket/ioctl helper; no packet, bind or setting is sent.
    network = frame.unitree.current_loopback(expected)
    env = frame.environment
    path = frame.input(selection['cyclonedds_xml'])
    require(env.get('ROS_DOMAIN_ID') == '1' and env.get('RMW_IMPLEMENTATION') == 'rmw_cyclonedds_cpp' and
            env.get('CYCLONEDDS_URI') == 'file://' + str(path) and env.get('ROS_SECURITY_ENABLE', 'false') == 'false' and
            not any(key in env for key in ('ROS_SECURITY_STRATEGY', 'ROS_SECURITY_KEYSTORE', 'ROS_SECURITY_ENCLAVE_OVERRIDE')),
            'closed actual domain1 Cyclone disabled-test environment required')
    raw = api.bounded(path, 65536)
    require(b'<!DOCTYPE' not in raw and b'<!ENTITY' not in raw, 'Cyclone XML entities/doctype forbidden')
    tree = ET.fromstring(raw)
    allowed = {'CycloneDDS', 'Domain', 'General', 'Interfaces', 'NetworkInterface', 'AllowMulticast',
               'NetworkInterfaceAddress', 'Discovery', 'ParticipantIndex', 'MaxAutoParticipantIndex', 'Peers', 'Peer',
               'EnableTopicDiscoveryEndpoints'}
    interfaces = []
    for item in tree.iter():
        tag = item.tag.rsplit('}', 1)[-1]
        require(tag in allowed and len(item.attrib) <= 4, 'unreviewed Cyclone profile element/attributes')
        if tag == 'NetworkInterface':
            require(item.attrib.get('name') == 'lo' and set(item.attrib) <= {'name', 'multicast'}, 'only actual loopback DDS interface allowed')
            interfaces.append('lo')
        if tag == 'NetworkInterfaceAddress':
            require(item.text in ('lo', '127.0.0.1'), 'only loopback DDS address allowed')
            interfaces.append('lo')
        if tag == 'Domain':
            require(set(item.attrib) <= {'Id', 'id'} and next(iter(item.attrib.values()), '1') in ('1', 'any'), 'closed DDS domain required')
        if tag == 'Peer':
            require(set(item.attrib) == {'Address'} and re.fullmatch(r'127\.0\.0\.1(?::[1-9][0-9]{0,4})?', item.attrib['Address']),
                    'explicit DDS peers must remain IPv4 loopback')
    require(tree.tag.rsplit('}', 1)[-1] == 'CycloneDDS' and interfaces, 'explicit actual loopback Cyclone configuration required')
    return {'network_namespace': expected, 'actual_interface_proof': network,
            'xml': frame.pin(path), 'domain': 1, 'rmw': env['RMW_IMPLEMENTATION'],
            'security_profile': 'disabled-test', 'physical_robot_access': False}


def policy(chosen, positive):
    """Build the complete literal test policy; no caller may inject grant argv."""
    connection = {'node-name': 'typed_gateway', 'namespace': '/uwvm_unitree_typed', 'domain-id': '1',
                  'rmw-implementation': 'rmw_cyclonedds_cpp', 'security-mode': 'disabled-test',
                  'keystore': 'not-applicable', 'enclave': 'not-applicable'}
    if not positive:
        return connection, []
    family = chosen['family_code']
    rows = [('unitree-lowstate', 'topic.subscribe', 'lowstate',
             ('unitree_go' if family == 1 else 'unitree_hg') + '/msg/LowState')] if family != 3 else [
        ('unitree-api-request', 'topic.publish', 'request', 'unitree_api/msg/Request'),
        ('unitree-api-response', 'topic.subscribe', 'response', 'unitree_api/msg/Response')]
    result = []
    for alias, operation, resource, type_name in rows:
        limits = {'max-message-bytes': '65536', 'max-rate-hz': '100', 'burst': '1',
            'adapter-codec-identity': chosen['codec_identities'][type_name],
            'max-bytes-per-second': '6553600', 'max-queue-bytes': '65536', 'max-inflight': '1',
            'overflow-policy': 'reject', 'validator': 'none', 'audit': 'allow-and-deny',
            'audit-max-rate-hz': '100', 'audit-burst': '16', 'audit-max-records': '10000', 'audit-max-bytes': '1048576',
            'max-publishers' if operation == 'topic.publish' else 'max-subscribers': '1',
            'qos.reliability': 'reliable', 'qos.durability': 'volatile', 'qos.history': 'keep-last', 'qos.depth': '1',
            'qos.deadline': 'infinite', 'qos.lifespan': 'infinite', 'qos.liveliness': 'automatic',
            'qos.liveliness-lease-duration': 'infinite'}
        result.append({'alias': alias, 'operation': operation, 'resource': '/uwvm_unitree_typed/' + resource,
                       'type': type_name, 'identity': chosen['identities'][type_name], 'limits': limits})
    return connection, result


def admit(frame, api, native, selection):
    """Admit a closed selection and all real products before any DDS actor."""
    exact(selection, ('format_version', 'kind', 'reports', 'workers', 'products', 'families', 'native_peer', 'exec_gate',
                      'restart_mappings', 'worker_catalog_roots', 'expected_network_namespace', 'cyclonedds_xml'),
          'closed maintained typed transport selection required')
    require(selection['format_version'] == 1 and selection['kind'] == 'uwvm-unitree-same-main-typed-dds-selection-v1',
            'typed transport selection version differs')
    exact(selection['reports'], ('commands', 'workers', 'native_products', 'native_interfaces'), 'four actual producer reports required')
    exact(selection['workers'], ('ros2', 'dual'), 'actual ros2 GO/API and dual HG interpreter inputs required')
    exact(selection['families'], FAMILIES, 'all three actual native family bindings required')
    evidence = Evidence(api, frame, selection['restart_mappings'])
    network = environment(frame, api, selection)
    products = commands(frame, api, evidence, selection)
    workers = runtime_profiles(frame, evidence, selection)
    prepared = native_products(frame, api, native, evidence, selection)
    families = family_bindings(frame, api, evidence, selection, workers, prepared)
    host_files, host_elfs = host_catalog(frame, api, selection['worker_catalog_roots'])
    # native_file_loader may retain the actual main Wasm file as an inode-backed
    # mapping. Admit exactly these already qualified products alongside host
    # execution files; never treat a Wasm mapping as an unknown host fallback.
    by_path = {pin['path']: pin for pin in host_files}
    for product in products:
        prior = by_path.setdefault(product['artifact']['path'], product['artifact'])
        require(prior == product['artifact'], 'current Worker/Guest mapping catalog pin conflict')
    host_files = list(by_path.values())
    frame.report['admission'] = {'reports': {key: frame.inputs[label] for key, label in selection['reports'].items()},
        'network': network, 'products': products, 'raw_producers_rewritten': False,
        'original_compiler_source_labels_retained': True, 'restart_device_bridges': evidence.bridges,
        'complete_native_file_catalog_entries': len(prepared['file_catalog']),
        'complete_Worker_file_catalog_entries': len(host_files), 'transport_execution': 'NOT_STARTED'}
    frame.save()
    return {**prepared, 'products': products, 'families': families, 'policy': policy,
            'worker_file_catalog': host_files, 'worker_elf_catalog': host_elfs, 'evidence': evidence}
