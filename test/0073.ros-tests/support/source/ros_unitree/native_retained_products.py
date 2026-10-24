#!/usr/bin/env python3
"""Requalify the exact failed Native6 run's completed74 build products at rest.

This module never executes a target, compiler, loader, mount operation or DDS
endpoint. The immutable raw run remains FAIL. Its only admitted failure is the
ordinary-file collector encountering the temporarily bound /dev/null device,
after all74 full-MD compile records and linked products had been retained.
Fresh success describes retained compilation and a CURRENT at-rest mapping
catalog, not successful completion of that old batch or execution of a robot.
"""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import stat


SCHEMA = 'uwvm-native74-retained-products-at-rest-v1'
STATUS = 'PASS_RETAINED_NATIVE74_BUILD_PRODUCTS_FRESH_AT_REST_CATALOG_NOT_EXECUTED'
INNER_SHA = 'eb5e185dbb38efebe90faabb2a7e21c10a33005ba31af9ca3a6a071e54a72487'
OUTER_SHA = '169b495fabd271f5c76ea88057659c6da6eb1f53551d0ee6dece5090dd880a7f'
PRODUCER_SHA = 'f1f6e9efa9cc13b8569b4f66fe492dac2aee1e7e56e88405218e066056b5fd24'
MIB = 1024**2
PIN = frozenset(('path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'))
ORIGINAL_BOOT = 'ec0d5f0f-3060-4220-bee2-2a7791ed4e72'
HISTORICAL_DNS_PROVENANCE = {'path': '/run/systemd/resolve/stub-resolv.conf', 'bytes': 936,
    'sha256': '7b4d45022f47d4da0f1e78b1e3ae7b72f445002561395033698d684da83afd4b',
    'device': 27, 'inode': 6177, 'mtime_ns': 1792147910964367489, 'ctime_ns': 1792147910964372047}


def require(value, reason):
    if not value:
        raise ValueError(reason)


def exact(value, names, reason):
    require(type(value) is dict and set(value) == set(names), reason)


def _summary(value):
    return {key: item for key, item in value.items() if key != 'catalog'}


def finite_evidence(admission, api, frame, mappings, originals):
    """Admit exact transitive metadata pins without broad parent inventories.

    Old producers intentionally retained signed-download/acquisition/control
    pins beyond their compilation roots. Only a genuine, already hash-bound
    raw ledger or a subsequently hash-bound selected metadata document may
    register such a full7. This exception is local to metadata finalization;
    transport's general Evidence containment policy remains unchanged.
    Every consumed current pin still enters Frame's final preservation ledger.
    """
    class FiniteEvidence(admission.Evidence):
        def __init__(self):
            require(type(mappings) is list and 1 <= len(mappings) <= 64, 'finite explicit current device histories required')
            canonical, seen = {}, set()
            for row in mappings:
                exact(row, ('root', 'historical_device', 'current_device'), 'closed finite device history required')
                root = Path(row['root'])
                require(root.is_absolute() and root.resolve(strict=True) == root and root.is_dir() and
                        type(row['historical_device']) is int and type(row['current_device']) is int and
                        row['historical_device'] >= 0 and row['current_device'] >= 0 and root.stat().st_dev == row['current_device'] and
                        any(selected == root or selected.is_relative_to(root) for selected in frame.roots) and
                        (row['root'], row['historical_device']) not in seen,
                        'current physical root plus distinct exact historical-device selection required')
                seen.add((row['root'], row['historical_device']))
                require(row['root'] not in canonical or canonical[row['root']]['current_device'] == row['current_device'],
                        'one actual current device per physical mapping root required')
                if row['historical_device'] != row['current_device']:
                    canonical.setdefault(row['root'], row)
            require(canonical, 'at least one explicit genuine restart transition required')
            super().__init__(api, frame, list(canonical.values()))
            self.mappings = mappings
            self.expected = set()
            self.register(originals)

        def register(self, value):
            if type(value) is dict:
                if admission.PIN <= value.keys():
                    pin = {key: value[key] for key in admission.PIN}
                    self.expected.add(tuple(pin[key] for key in sorted(admission.PIN)))
                    require(len(self.expected) <= 250000, 'bounded finite raw-ledger metadata closure required')
                for item in value.values():
                    self.register(item)
            elif type(value) is list:
                for item in value:
                    self.register(item)

        def pin(self, old):
            require(type(old) is dict and admission.PIN <= old.keys(), 'exact selected finite full7 required')
            old = {key: old[key] for key in admission.PIN}
            path = Path(old['path'])
            key = tuple(old[name] for name in sorted(admission.PIN))
            contained = path.is_relative_to(frame.source) or any(path.is_relative_to(root) for root in frame.roots)
            require((contained or key in self.expected) and path.is_absolute() and path.resolve(strict=True) == path and
                    all(type(old[name]) is int and old[name] >= 0 for name in admission.PIN - {'path', 'sha256'}) and
                    old['inode'] > 0 and type(old['sha256']) is str and re.fullmatch('[0-9a-f]{64}', old['sha256']),
                    'external metadata must be an exact genuinely retained finite physical full7')
            if key in self.cache:
                frame.guard()
                return self.cache[key]
            require(len(self.cache) < 250000, 'bounded consumed retained metadata closure required')
            actual = frame.pin(path)
            if actual != old:
                candidates = sorted((row for row in self.mappings if path.is_relative_to(Path(row['root'])) and
                                     row['historical_device'] == old['device']),
                                    key=lambda row: len(Path(row['root']).parts), reverse=True)
                require(candidates and old['device'] == candidates[0]['historical_device'] and
                        actual['device'] == candidates[0]['current_device'], 'finite metadata needs an explicit device-only bridge')
                bridge = self.restart.requalify_persistent_pin(old, str(path), frame.guard,
                    historical_device=candidates[0]['historical_device'], current_device=candidates[0]['current_device'],
                    observed_current=actual)
                require(bridge['current_pin'] == actual, 'finite metadata bridge must retain actual current full7')
                self.bridges.append(bridge)
            self.cache[key] = actual
            return actual

        def document(self, old, maximum=64 * MIB):
            body = super().document(old, maximum)
            self.register(body)
            return body

    return FiniteEvidence()


def sections(report):
    """Select eight unique real link targets, retaining their original roles."""
    result = []
    profiles = report['gateway_profiles']
    require([row['profile'] for row in profiles] == ['builtin', 'go-api', 'hg'],
            'exact three original native Gateway profiles required')
    for row in profiles:
        require(row['status'] == 'BUILT_NOT_EXECUTED' and row['binary'] == row['actual_elf']['input'],
                'original Gateway role must bind its actual linked ELF')
        result.append(('gateway-' + row['profile'], row['compile'], row['fresh_objects'], row['actual_elf']))
    products = report['products']
    require(len(products) == 3 and [row['profile'] for row in products] == ['builtin', 'go-api', 'hg'],
            'exact three portable-peer profile associations required')
    first = products[0]
    for row, gateway in zip(products, profiles):
        require(row['compile_commands'][0] == gateway['compile'] and
                row['gateway'] == gateway['binary'] and row['gateway_elf'] == gateway['actual_elf'] and
                row['peer'] == first['peer'] == row['peer_elf']['input'] and
                row['peer_elf'] == first['peer_elf'] and row['compile_commands'][1:] == first['compile_commands'][1:] and
                row['fresh_peer_objects'] == first['fresh_peer_objects'] and
                row['fresh_identity_objects'] == first['fresh_identity_objects'] and
                row['interface_identity'] == first['interface_identity'] and row['full_dependencies_verified'] is True,
                'shared portable-peer/String evidence must be exact, not counted three times')
    identity = first['compile_commands'][2]
    # The String exporter executable is retained as an actual product pin and
    # by its successful mapped-export command, not a guessed generation path.
    candidates = [row for row in report['commands'] if row['label'] == 'string-identity-actual-mapped-export']
    require(len(candidates) == 1 and candidates[0]['status'] == 'PASS' and candidates[0]['returncode'] == 0,
            'actual original String export command must have completed')
    result.extend((('portable-peer', first['compile_commands'][1], first['fresh_peer_objects'], first['peer_elf']),
                   ('string-identity', identity, first['fresh_identity_objects'], candidates[0]['actual_ELF']),
                   ('exec-gate', report['exec_gate_compile_commands'][0], report['fresh_exec_gate_objects'], report['exec_gate_elf']),
                   ('typed-peer', report['typed_peer_compile'], report['typed_peer_fresh_objects'], report['typed_peer']),
                   ('physics-controller', report['physics_controller_compile'], report['physics_controller_fresh_objects'], report['physics_controller'])))
    require(len(report['exec_gate_compile_commands']) == 1 and report['exec_gate'] == report['exec_gate_elf']['input'] and
            len({row[3]['input']['path'] for row in result}) == 8,
            'eight distinct linked target roles required')
    return result


class Qualifier:
    """Bounded metadata reconstruction; every selected byte stays in Frame."""
    def __init__(self, frame, api, admission, evidence, report, outer):
        self.frame, self.api, self.admission, self.evidence = frame, api, admission, evidence
        self.report, self.outer = report, outer
        self.source, self.frozen = evidence.source(report)
        self.old_out = Path(report['configuration']['path'])  # Replaced by the selected actual inner parent below.
        self.products = {}
        self.original_source_relations = []
        producer = api.frozen_module(frame.source, frame.frozen,
            'test/0073.ros-tests/support/source/ros_unitree/acceptance/native74_r6/native_producer.py', 'retained74_ELF_reader')
        raw = api.bounded(Path(producer.__file__), MIB)
        require(hashlib.sha256(raw).hexdigest() == PRODUCER_SHA and outer['worker']['sha256'] == PRODUCER_SHA,
                'unchanged reviewed original Native6 generic ELF reader required')
        self.elf_reader = producer

    def product(self, path):
        value = self.frame.pin(path)
        require(value['path'] not in self.products or self.products[value['path']] == value,
                'new finalization product changed')
        self.products[value['path']] = value
        return value

    def body(self, pin, maximum=64 * MIB):
        if Path(pin['path']).is_relative_to(self.frame.out):
            current = self.frame.pin(Path(pin['path']))
            require(current == pin, 'fresh owned metadata physical pin changed')
        else:
            current = self.evidence.pin(pin)
        body = self.api.bounded(Path(current['path']), maximum)
        require(len(body) == current['bytes'] and hashlib.sha256(body).hexdigest() == current['sha256'],
                'retained metadata bytes changed after physical pin admission')
        return body

    def translate(self, name, cwd='/owned'):
        """Reproduce ONLY the actual Native6 logical chroot path mapping."""
        require(type(name) is str and name and '\0' not in name and '\n' not in name, 'clean compiler pathname required')
        logical = posixpath.normpath(name if name.startswith('/') else posixpath.join(cwd, name))
        pairs = [(str(self.rootfs), self.rootfs), (str(self.source), self.source)]
        pairs += [(str(value), value) for value in self.family_roots.values()]
        pairs += [(str(value), value) for value in self.generated.values()]
        pairs += [('/source', self.source)]
        pairs += [('/owned/' + key, value) for key, value in self.family_roots.items()]
        pairs += [('/providers/' + key, value) for key, value in self.generated.items()]
        pairs += [('/owned', self.work), ('/tmp', self.old_out / 'tmp'), ('/root', self.old_out / 'home')]
        for prefix, physical in pairs:
            if logical == prefix or logical.startswith(prefix + '/'):
                path = physical / logical[len(prefix):].lstrip('/')
                break
        else:
            require(any(logical == prefix or logical.startswith(prefix + '/') for prefix in
                    ('/usr', '/opt', '/lib', '/lib64', '/bin', '/sbin', '/etc')),
                    'compiler input outside the exact original logical mappings')
            path = self.rootfs / logical.lstrip('/')
        actual = path.resolve(strict=True)
        roots = [self.source, self.work, self.rootfs, *self.family_roots.values(),
                 *self.generated.values(), self.old_out / 'tmp', self.old_out / 'home']
        require(any(actual == root or actual.is_relative_to(root) for root in roots),
                'compiler pathname escapes original physical source/SDK/build roots')
        return actual

    def opened(self, path):
        return self.frame.pin(path)

    def elf(self, old, kind):
        actual = self.elf_reader.elf(self.frame.common, Path(old['input']['path']), kind)
        current_input = self.evidence.pin(old['input'])
        require({key: value for key, value in actual.items() if key != 'input'} ==
                {key: value for key, value in old.items() if key != 'input'} and current_input == actual['input'],
                'retained target/object actual ELF layout or full physical pin changed')
        return actual

    def compare_catalog(self, old, current):
        """Bridge each specifically selected physical pin, preserving raw JSONL."""
        require({key: old[key] for key in ('root', 'files', 'bytes', 'symlinks')} ==
                {key: current[key] for key in ('root', 'files', 'bytes', 'symlinks')}, 'complete old/current root cardinality differs')
        old_lines = self.body(old['catalog'], 128 * MIB).splitlines()
        current_lines = self.body(current['catalog'], 128 * MIB).splitlines()
        require(len(old_lines) == len(current_lines), 'complete retained root name count changed')
        for a, b in zip(old_lines, current_lines):
            require(len(a) <= 65536 and len(b) <= 65536, 'bounded complete retained root row required')
            before, now = self.admission.decode(a), self.admission.decode(b)
            relative = Path(before['relative'])
            require(not relative.is_absolute() and '..' not in relative.parts and str(relative) == before['relative'] and
                    now['relative'] == before['relative'] and now['kind'] == before['kind'], 'complete retained root name/order/kind changed')
            path = str(Path(old['root']) / relative)
            if before['kind'] == 'file':
                old_pin = {'path': path, **{key: before[key] for key in PIN - {'path'}}}
                actual_pin = {'path': path, **{key: now[key] for key in PIN - {'path'}}}
                require(self.evidence.pin(old_pin) == actual_pin and before['executable'] == now['executable'],
                        'complete retained ordinary file changed beyond explicit selected device history')
            else:
                require(before['kind'] == 'symlink' and set(before) == set(now) and
                        {key: value for key, value in before.items() if key not in ('device', 'target_file')} ==
                        {key: value for key, value in now.items() if key not in ('device', 'target_file')},
                        'complete retained symlink identity/text/resolution changed')
                if before['device'] != now['device']:
                    candidates = sorted((row for row in self.evidence.mappings if Path(path).is_relative_to(Path(row['root'])) and
                        row['historical_device'] == before['device']), key=lambda row: len(Path(row['root']).parts), reverse=True)
                    require(candidates and candidates[0]['current_device'] == now['device'], 'symlink device needs exact selected history')
                if 'target_file' in before:
                    self.evidence.register(before['target_file'])
                    require(self.evidence.pin(before['target_file']) == now['target_file'], 'retained symlink selected ordinary target changed')

    def setup(self, inner_pin):
        self.old_out = Path(inner_pin['path']).parent
        self.work = self.old_out / 'work'
        self.rootfs = Path(self.report['reused_COPY_after']['reused_root'])
        native = self.evidence.document(self.report['producer_reports']['native'])
        worker = self.evidence.document(self.report['producer_reports']['worker_registries'])
        self.family_roots = {row['family']: Path(row['derived_tree']['root']) for row in native['families']}
        require(set(self.family_roots) == {'unitree_go', 'unitree_hg', 'unitree_api'}, 'exact original native family roots required')
        hg = [row for row in native['families'] if row['family'] == 'unitree_hg'][0]
        self.generated = {'go-api': Path(worker['ros2_generation']['path']).parent,
                          'hg': Path(hg['generation']['path']).parent}
        for root in (self.source, self.work, self.rootfs, *self.family_roots.values(), *self.generated.values()):
            require(root.is_absolute() and root.resolve(strict=True) == root and root.is_dir() and
                    any(root == selected or root.is_relative_to(selected) for selected in self.frame.roots),
                    'entire original physical source/SDK/build root must remain inventoried')

    def inventories(self):
        """Compare complete genuine old catalogs with fresh current inventories."""
        observed = []
        for key, owner in (('input_roots_after', self.report), ('host_input_roots_after', self.outer),
                           ('execution_view_after', self.report)):
            for old in owner[key]:
                self.evidence.pin(old['catalog'])
                current = [row for row in self.frame.catalogs if row['root'] == old['root']]
                # Some complete prefix roots are contained within an already
                # inventoried family root; create a separate complete catalog.
                if not current:
                    root = Path(old['root'])
                    require(any(root.is_relative_to(item) for item in self.frame.roots), 'complete retained root missing from Frame')
                    current = [self.frame.common.catalog(root,
                        self.frame.out / ('retained-tree-%03d.jsonl' % len(observed)), self.frame.roots)]
                    self.product(current[0]['catalog']['path'])
                require(len(current) == 1, 'one complete current retained-root inventory required')
                self.compare_catalog(old, current[0])
                observed.append({'original_catalog': old['catalog'], 'current': current[0], 'role': key})
        return observed

    def reused_copy(self):
        """Reopen the complete genuine independent COPY ledger, not old TUs."""
        retained = self.report['reused_COPY_after']
        ledger = retained['ledger']
        base, root = Path(retained['source_root']), Path(retained['reused_root'])
        names, files, links, total = set(), 0, 0, 0
        for raw in self.body(ledger, 64 * MIB).splitlines(keepends=True):
            self.frame.guard()
            require(len(raw) <= 65536 and raw.endswith(b'\n'), 'bounded complete original independent COPY ledger row required')
            row = self.admission.decode(raw)
            self.evidence.register(row)
            relative = Path(row['relative'])
            require(not relative.is_absolute() and '..' not in relative.parts and str(relative) == row['relative'] and
                    row['relative'] not in names and len(names) < 65536, 'safe distinct original COPY ledger name required')
            names.add(row['relative'])
            a, b = base / relative, root / relative
            require(a.parent.resolve(strict=True).is_relative_to(base) and b.parent.resolve(strict=True).is_relative_to(root),
                    'original independent COPY name escapes its complete SDK root')
            if row['kind'] == 'file':
                exact(row, ('relative', 'kind', 'source', 'copy', 'identical_existing'), 'closed original ordinary COPY ledger row required')
                require(row['identical_existing'] is False and row['source']['path'] == str(a) and row['copy']['path'] == str(b) and
                        all(row['source'][key] == row['copy'][key] for key in ('bytes', 'sha256')) and
                        (row['source']['device'], row['source']['inode']) != (row['copy']['device'], row['copy']['inode']),
                        'original COPY must be independently allocated identical bytes')
                self.evidence.pin(row['source'])
                self.evidence.pin(row['copy'])
                files += 1
                total += row['source']['bytes']
            else:
                exact(row, ('relative', 'kind', 'target', 'source_device', 'source_inode', 'identical_existing'),
                      'closed original symlink COPY ledger row required')
                require(row['kind'] == 'symlink' and row['identical_existing'] is False and a.is_symlink() and b.is_symlink() and
                        os.readlink(a) == os.readlink(b) == row['target'], 'original independent COPY symlink text changed')
                before = a.lstat()
                mappings = sorted((item for item in self.evidence.mappings if a.is_relative_to(Path(item['root']))),
                                  key=lambda item: len(Path(item['root']).parts), reverse=True)
                admitted_device = row['source_device'] == before.st_dev or (mappings and
                    row['source_device'] == mappings[0]['historical_device'] and before.st_dev == mappings[0]['current_device'])
                after = a.lstat()
                require(admitted_device and before.st_ino == row['source_inode'] and
                        all(getattr(before, key) == getattr(after, key) for key in
                            ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns')) and
                        os.readlink(a) == row['target'], 'original source symlink inode/device changed beyond explicit mapping')
                links += 1
        require((files, links, total) == (retained['files'], retained['symlinks'], retained['file_bytes']) ==
                (32046, 1265, 1266016500), 'complete original independent COPY ledger cardinality changed')
        self.evidence.pin(ledger)
        return {'ledger': self.evidence.pin(ledger), 'files': files, 'symlinks': links, 'file_bytes': total,
                'every_original_source_and_COPY_full7_reopened': True,
                'old_target_object_admitted': False, 'symlink_current_whole_catalogs_checked_separately': True}

    def reused_directories(self):
        """Recheck the old COPY's separately retained directory metadata."""
        old = self.report['reused_sdk_directories_after']
        output = self.frame.out / 'reused-sdk-directories-current.jsonl'
        current = self.elf_reader.reused_directory_state(self.rootfs, output, self.frame.guard)
        current['catalog'] = self.product(output)
        require(current['root'] == old['root'] and current['directories'] == old['directories'],
                'complete reused SDK directory name count changed')
        before_rows = self.body(old['catalog'], 16 * MIB).splitlines()
        now_rows = self.body(current['catalog'], 16 * MIB).splitlines()
        require(len(before_rows) == len(now_rows) == old['directories'], 'complete old/current SDK directory catalog differs')
        for before_raw, now_raw in zip(before_rows, now_rows):
            before, now = self.admission.decode(before_raw), self.admission.decode(now_raw)
            require(set(before) == set(now) == {'relative', 'device', 'inode', 'mode', 'nlink', 'bytes', 'mtime_ns', 'ctime_ns'} and
                    {key: value for key, value in before.items() if key != 'device'} ==
                    {key: value for key, value in now.items() if key != 'device'}, 'reused SDK directory metadata changed')
            relative = Path(before['relative'])
            require(not relative.is_absolute() and '..' not in relative.parts and str(relative) == before['relative'], 'safe SDK directory name required')
            path = self.rootfs / relative
            if before['device'] != now['device']:
                pairs = sorted((row for row in self.evidence.mappings if path.is_relative_to(Path(row['root'])) and
                    row['historical_device'] == before['device']), key=lambda row: len(Path(row['root']).parts), reverse=True)
                require(pairs and pairs[0]['current_device'] == now['device'], 'SDK directory device needs exact selected history')
            descriptor = self.evidence.restart._directory(str(path))
            try:
                actual = os.fstat(descriptor)
                require(all(getattr(actual, field) == now[key] for field, key in
                    (('st_dev', 'device'), ('st_ino', 'inode'), ('st_mode', 'mode'), ('st_nlink', 'nlink'),
                     ('st_size', 'bytes'), ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))),
                    'actual descriptor-anchored SDK directory changed during qualification')
            finally:
                os.close(descriptor)
        return current

    def file_api(self, section, database):
        if 'file_api' not in section:
            return {'scope': 'NOT_CAPTURED_BY_ORIGINAL_ROLE_COMPILATION_DATABASE_AND_FULL_MD_RECHECKED'}
        selected = section['file_api']
        index = self.evidence.document(selected['index'])
        model = self.evidence.document(selected['codemodel'])
        target = self.evidence.document(selected['target'])
        reply = Path(selected['index']['path']).parent
        require(index['reply']['codemodel-v2']['kind'] == 'codemodel' and
                index['reply']['codemodel-v2']['version']['major'] == 2 and
                reply / index['reply']['codemodel-v2']['jsonFile'] == Path(selected['codemodel']['path']) and
                len(model['configurations']) == 1 and model['configurations'][0]['name'] == 'Release' and
                target['type'] == 'EXECUTABLE', 'original actual CMake FileAPI association changed')
        matches = [row for row in model['configurations'][0]['targets'] if row['name'] == target['name']]
        require(len(matches) == 1 and reply / matches[0]['jsonFile'] == Path(selected['target']['path']),
                'FileAPI target no longer belongs to selected codemodel')
        sources = []
        for row in target['sources']:
            if 'compileGroupIndex' not in row:
                continue
            require(target['compileGroups'][row['compileGroupIndex']]['language'] == 'CXX', 'actual CXX source group required')
            path = self.translate(row['path'], model['paths']['source'])
            sources.append({'path': str(path), 'input': self.opened(path), 'generated': row.get('isGenerated', False)})
        configured = [str(self.translate(row['file'], row['directory'])) for row in database]
        require(len(sources) == len(selected['sources']) and all(
                actual['path'] == old['path'] and actual['generated'] == old['generated'] and
                actual['input'] == self.evidence.pin(old['input']) for actual, old in zip(sources, selected['sources'])) and
                sources and len({row['path'] for row in sources}) == len(sources) and
                sorted(configured) == sorted(row['path'] for row in sources), 'complete configured FileAPI/source bijection changed')
        return {'scope': 'ACTUAL_ORIGINAL_FILE_API_INDEPENDENTLY_REPARSED', 'index': selected['index'],
                'codemodel': selected['codemodel'], 'target': selected['target'], 'source_count': len(sources)}

    @staticmethod
    def without_dependencies(argv):
        result, at = [], 0
        while at < len(argv):
            token = argv[at]
            if token == '-MD':
                at += 1
                continue
            if token in ('-MF', '-MT'):
                require(at + 1 < len(argv) and argv[at + 1] and not argv[at + 1].startswith('-'), 'dependency option operand required')
                at += 2
                continue
            require(not token.startswith(('-MF', '-MT')), 'unreviewed joined dependency option')
            result.append(token)
            at += 1
        return tuple(result)

    def compile(self, role, section, fresh, executable):
        """Reparse every actual verbose argv, emitted MD rule and linker trace."""
        database = self.evidence.document(section['compile_database'])
        records = self.admission.compile_records(self.evidence, section)
        require(len(database) == len(records) and fresh['compile_database'] == section['compile_database'],
                'complete original compile database/object inventory mismatch')
        log = self.body(section['verbose_build_log']).decode('utf-8', 'strict')
        lines = log.splitlines()
        logical_build = records[0]['actual_compile']['cwd']
        build = self.translate(logical_build)
        require(Path(section['compile_database']['path']).parent == build and build.is_relative_to(self.work),
                'compile database must belong to the original selected fresh build directory')
        configured, actual, stack, events = {}, {}, {}, []
        for row in database:
            argv = row.get('arguments') or shlex.split(row['command'])
            key = self.without_dependencies(argv)
            require(argv[0] in ('/usr/bin/gcc', '/usr/bin/g++') and row['directory'] == logical_build and key not in configured,
                    'unique original configured compiler recipe required')
            configured[key] = row
        for number, line in enumerate(lines, 1):
            stripped = line.strip()
            match = re.fullmatch(r"(?:make|gmake)(?:\[(\d+)\])?: (Entering|Leaving) directory '([^']+)'", stripped)
            if match:
                level, directory = int(match[1] or 0), posixpath.normpath(match[3])
                require(directory == logical_build or directory.startswith(logical_build + '/'), 'Make directory outside exact target build')
                if match[2] == 'Entering':
                    require(level not in stack and (not stack or level == max(stack) + 1), 'ambiguous Make entry context')
                    stack[level] = directory
                else:
                    require(stack and level == max(stack) and stack[level] == directory, 'unmatched actual Make exit')
                    del stack[level]
                events.append({'line_number': number, 'raw': line, 'level': level, 'operation': match[2], 'cwd': directory})
                continue
            if not stripped.startswith(('/usr/bin/gcc ', '/usr/bin/g++ ', 'cd ')):
                continue
            argv = shlex.split(stripped)
            if argv[0] == 'cd':
                require(not (len(argv) >= 4 and argv[2] == '&&' and argv[3] in ('/usr/bin/gcc', '/usr/bin/g++')),
                        'unreviewed cd-prefixed actual compiler invocation')
                continue
            if argv[0] not in ('/usr/bin/gcc', '/usr/bin/g++') or '-c' not in argv:
                continue
            require(stack and stack[max(stack)] == logical_build and
                    not any(token in ('&&', ';', '|', '>', '>>', '<') for token in argv), 'actual compiler lacks exact Make cwd witness')
            key = self.without_dependencies(argv)
            require(key in configured and key not in actual, 'extra, duplicate or unmatched compiler command')
            actual[key] = {'argv': argv, 'cwd': logical_build, 'make_level': max(stack), 'raw': line,
                'line_number': number, 'cwd_basis': 'ACTUAL_RETAINED_MAKE_ENTER_LEAVE_CURRENT_DIRECTORY'}
        require(not stack and set(actual) == set(configured) and events == section['actual_make_directory_events'],
                'actual full compiler/Make witness is not a complete database bijection')
        objects, dependencies, proofs = set(), {}, []
        for summary, full, row in zip(section['compile_records'], records, database):
            witness = actual[self.without_dependencies(row.get('arguments') or shlex.split(row['command']))]
            argv = witness['argv']
            require(full['actual_compile'] == witness and full['configured_compile_database_row'] == row and
                    full['physical_compile_cwd'] == str(build) and '-UNDEBUG' in argv and
                    not any(token.startswith('-DNDEBUG') for token in argv), 'retained complete compiler argv/assertion mode differs')
            flags = lambda option: [argv[at + 1] for at, token in enumerate(argv) if token == option]
            require(len(flags('-o')) == len(flags('-c')) == 1 and flags('-MF') and flags('-MT') and '-MMD' not in argv,
                    'complete actual MD/object/source option operands required')
            obj = self.translate(flags('-o')[0], logical_build)
            source = self.translate(row['file'], logical_build)
            require(self.translate(flags('-c')[0], logical_build) == source and self.translate(flags('-MF')[-1], logical_build) == Path(full['depfile']['path']),
                    'actual source/object/dependency argv association differs')
            raw = self.body(full['depfile'], 16 * MIB).decode('utf-8').replace('\\\n', '')
            require('$' not in raw and raw.count(':') == 1, 'unreviewed actual compiler Make dependency grammar')
            target, names = raw.split(':', 1)
            emitted = [self.translate(name, logical_build) for name in shlex.split(target)]
            requested = [self.translate(name, logical_build) for value in flags('-MT') for name in shlex.split(value)]
            require(obj in emitted and requested and all(value in emitted for value in requested), 'actual accumulated MD target missing')
            options = {'requested': {'MD_count': argv.count('-MD'), 'MF': flags('-MF'), 'MT': flags('-MT')},
                'effective': {'MF_last_argument': flags('-MF')[-1], 'MF_physical_depfile': str(Path(full['depfile']['path'])),
                    'MT_accumulated_arguments': flags('-MT'), 'MT_last_argument': flags('-MT')[-1],
                    'MT_requested_physical_targets': [str(value) for value in requested],
                    'emitted_physical_targets': [str(value) for value in emitted]},
                'semantics': 'LAST_MF_FILENAME_ALL_MT_TARGET_OPERANDS_ACCUMULATE'}
            require(options == full['dependency_options'], 'retained compiler MD option semantics differ')
            deps = [self.opened(self.translate(name, logical_build)) for name in shlex.split(names)]
            source_pin = self.evidence.pin(full['source'])
            require(deps == [self.evidence.pin(pin) for pin in full['dependencies']] and self.opened(source) == source_pin and source_pin in deps and
                    full['object']['input']['path'] == str(obj) and str(obj) not in objects,
                    'complete emitted MD bytes, ordered dependency pins or actual source/object association changed')
            self.elf(full['object'], {1})
            objects.add(str(obj))
            for pin in deps:
                dependencies[pin['path']] = pin
                relation = self.evidence.source_counterpart(pin, self.source, self.frozen)
                if relation is not None:
                    self.original_source_relations.append(relation)
            proofs.append({'full_MD_receipt': self.evidence.pin(summary['full_MD_receipt']), 'source': source_pin,
                           'object': self.evidence.pin(full['object']['input']), 'dependency_count': len(deps)})
        fresh_objects = fresh['objects']
        require(len(fresh_objects) == len(objects) and {row['physical'] for row in fresh_objects} == objects and
                all(row['existed_before_build'] is False and str(self.translate(row['logical'])) == row['physical'] for row in fresh_objects),
                'original fresh-before-build object evidence differs from complete actual objects')
        require(sorted(dependencies) == section['compiled_dependency_paths'], 'compiled full dependency path union differs')
        trace, resolved = [], {}
        for line in lines:
            if re.fullmatch(r'[^\s]+\.(?:o|a|so(?:\.[0-9]+)*)', line.strip()):
                pin = self.opened(self.translate(line.strip(), logical_build))
                trace.append({'raw': line, 'physical_input': pin})
                resolved[pin['path']] = pin
        require(len(trace) == len(section['actual_link_trace']) and all(
                actual['raw'] == old['raw'] and actual['physical_input'] == self.evidence.pin(old['physical_input'])
                for actual, old in zip(trace, section['actual_link_trace'])) and
                list(resolved.values()) == [self.evidence.pin(pin) for pin in section['resolved_link_inputs']] and
                objects <= set(resolved) and any(Path(path).is_relative_to(self.rootfs) for path in resolved),
                'complete actual GNU linker input trace differs')
        require(section['link_commands'], 'retained actual native link command required')
        linked = []
        for row in section['link_commands']:
            commands = self.body(row['link_script']).decode('utf-8').splitlines()
            require(commands == row['actual_command_lines'] and commands and
                    all(not line.strip() or line.strip() in log for line in commands), 'actual link script/log bytes differ')
            for line in commands:
                if not line.strip():
                    continue
                argv = shlex.split(line)
                require(argv[0] in ('/usr/bin/gcc', '/usr/bin/g++') and argv.count('-o') == 1 and
                        '-Wl,--trace' in argv and
                        self.translate(argv[argv.index('-o') + 1], logical_build) == Path(executable['input']['path']),
                        'actual target link output association missing')
                linked.append(row['link_script'])
        self.elf(executable, {2, 3})
        contract = self.report['native_link_contract']
        require(executable['PT_INTERP'] == [contract['required_PT_INTERP']['path']] and
                executable['DT_RPATH'] == [':'.join(contract['required_DT_RPATH'])] and executable['DT_RUNPATH'] == [] and
                executable['DT_NEEDED'] and all('/' not in item and item not in ('.', '..') for item in executable['DT_NEEDED']),
                'retained linked target physical COPY loader/RPATH/SONAME contract differs')
        return {'role': role, 'translation_units': len(records), 'binary': self.evidence.pin(executable['input']),
                'actual_ELF_reparsed': True, 'complete_MD': proofs, 'file_api': self.file_api(section, database),
                'compile_database': section['compile_database'], 'verbose_build_log': section['verbose_build_log'],
                'link_scripts': linked, 'actual_link_input_count': len(resolved),
                'freshness_scope': 'ORIGINAL_R6_RETAINED_FRESH_BEFORE_BUILD_EVIDENCE_NOT_A_NEW_COMPILATION'}

    def at_rest(self):
        """Prove restored placeholders, not unrecorded historical char facts."""
        current = os.readlink('/proc/self/ns/mnt')
        require(current == self.current_mount_namespace, 'explicit selected CURRENT at-rest mount namespace required')
        mounts = []
        for line in Path('/proc/self/mountinfo').read_text().splitlines():
            target = re.sub(r'\\([0-7]{3})', lambda match: chr(int(match[1], 8)), line.split()[4])
            mounts.append(Path(target))
        require(not any(mount == self.rootfs or mount.is_relative_to(self.rootfs) or
                        mount == self.work or mount.is_relative_to(self.work) for mount in mounts),
                'selected COPY/build tree still contains active mountpoints')
        declared = []
        for row in self.report['commands']:
            argv = row['argv']
            if len(argv) == 4 and argv[1] == '--bind' and argv[2] in ('/dev/null', '/dev/zero', '/dev/urandom', '/dev/random'):
                require(argv[3] == str(self.rootfs / argv[2].lstrip('/')) and row['status'] == 'PASS' and row['returncode'] == 0,
                        'original declared character-device bind association differs')
                declared.append(Path(argv[3]))
        require(len(declared) == 4 and len(set(declared)) == 4, 'exact four original nonhardware bind destinations required')
        pins = []
        for path in declared:
            pin = self.frame.pin(path)
            facts = path.stat(follow_symlinks=False)
            require(stat.S_ISREG(facts.st_mode) and pin['bytes'] == 0 and
                    all(getattr(facts, field) == pin[key] for field, key in
                        (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                         ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))),
                    'declared original bind destination is not a restored ordinary empty placeholder')
            pins.append(pin)
        return {'current_mount_namespace': current, 'historical_owner_mount_namespace': self.outer['original_mount_namespace'],
                'mount_namespace_continuity_claimed': False, 'selected_active_mounts': [], 'restored_ordinary_placeholders': pins,
                'original_active_char_mount_facts': 'NOT_RETAINED_NOT_RECONSTRUCTED',
                'scope': 'CURRENT_AT_REST_PHYSICAL_STATE_ONLY_ORIGINAL_CLEANUP_RETAINED_SEPARATELY'}

    def execution_catalog(self, roles):
        files, elfs = {}, []
        views = []
        for index, old in enumerate(self.report['execution_view_after']):
            row = self.frame.common.catalog(Path(old['root']), self.frame.out / ('execution-view-%02d.jsonl' % index), self.frame.roots)
            self.product(row['catalog']['path'])
            self.compare_catalog(old, row)
            views.append(row)
            raw = self.body(row['catalog'], 128 * MIB)
            for line in raw.splitlines():
                require(len(line) <= 16 * 1024, 'bounded execution inventory row required')
                item = self.admission.decode(line)
                if item['kind'] != 'file':
                    continue
                path = Path(old['root']) / item['relative']
                require(path.resolve(strict=True) == path, 'execution ordinary-file name became a symlink')
                pin = self.frame.pin(path)
                require(all(pin[key] == item[key] for key in ('bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns')),
                        'current ordinary execution file differs from complete fresh catalog')
                require(pin['path'] not in files or files[pin['path']] == pin, 'overlapping execution roots disagree')
                files[pin['path']] = pin
        for role in roles:
            pin = self.evidence.pin(role['binary'])
            files[pin['path']] = pin
        require(0 < len(files) <= 1000000, 'finite complete current native mapping closure required')
        ledger = self.frame.out / 'execution-files.jsonl'
        total = 0
        with ledger.open('xb') as stream:
            for path, pin in sorted(files.items()):
                self.frame.guard()
                fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
                try:
                    first = os.fstat(fd)
                    head = os.read(fd, 64)
                    last, named = os.fstat(fd), Path(path).stat(follow_symlinks=False)
                    require(stat.S_ISREG(first.st_mode) and all(getattr(first, field) == getattr(last, field) == getattr(named, field) == pin[key]
                            for field, key in (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                                               ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))),
                            'execution file physical identity changed during ELF classification')
                    if head[:4] == b'\x7fELF':
                        require(len(head) == 64 and head[4:6] == b'\x02\x01' and int.from_bytes(head[18:20], 'little') == 62,
                                'foreign or truncated ELF in current native execution closure')
                        elfs.append(pin)
                finally:
                    os.close(fd)
                body = (json.dumps(pin, sort_keys=True, separators=(',', ':')) + '\n').encode()
                total += len(body)
                require(len(body) <= 16 * 1024 and total <= 512 * MIB, 'bounded complete execution-file ledger required')
                stream.write(body)
            stream.flush()
            os.fsync(stream.fileno())
        loader = self.evidence.pin(self.report['execution_loader']['physical_loader']['input'])
        require(0 < len(elfs) <= 8192 and loader in elfs and all(role['binary'] in elfs for role in roles),
                'complete current ELF catalog must contain loader and all eight retained role executables')
        return {'execution_file_catalog': self.product(ledger), 'execution_file_catalog_rows': len(files),
                'execution_elf_catalog': elfs, 'execution_view_after': views}


def qualify(frame, api, admission, selection):
    """Return a distinct fresh receipt; raw report statuses are never changed."""
    exact(selection, ('format_version', 'raw_inner', 'raw_outer', 'restart_mappings', 'current_context'), 'closed retained Native74 selection required')
    require(selection['format_version'] == 1 and selection['raw_inner'] != selection['raw_outer'], 'explicit distinct genuine raw run reports required')
    inner_pin, outer_pin = frame.inputs[selection['raw_inner']], frame.inputs[selection['raw_outer']]
    require(inner_pin['sha256'] == INNER_SHA and inner_pin['bytes'] == 13359038 and
            outer_pin['sha256'] == OUTER_SHA and outer_pin['bytes'] == 109125,
            'only exact genuine terminal Native6 raw reports are admitted')
    # These two exact SHA/size-bound raw documents bootstrap the finite old
    # metadata set. No arbitrary external path is admitted by containment alone.
    originals = []
    for pin in (inner_pin, outer_pin):
        raw = api.bounded(Path(pin['path']), 64 * MIB)
        require(len(raw) == pin['bytes'] and hashlib.sha256(raw).hexdigest() == pin['sha256'], 'exact genuine raw report bytes changed')
        originals.append(admission.decode(raw))
    evidence = finite_evidence(admission, api, frame, selection['restart_mappings'], originals)
    context = evidence.selected(selection['current_context'])
    exact(context, ('cgroup', 'restart_mappings', 'mount_namespace'), 'closed explicit current context required')
    require(context['cgroup'] == frame.config['cgroup'] and context['restart_mappings'] == selection['restart_mappings'],
            'selected actual current cgroup/device context differs from finalizer configuration')
    require(type(context['mount_namespace']) is str and re.fullmatch(r'mnt:\[[1-9][0-9]*\]', context['mount_namespace']) and
            os.readlink('/proc/self/ns/mnt') == context['mount_namespace'], 'explicit actual current mount namespace required')
    require(evidence.pin(inner_pin) == inner_pin and evidence.pin(outer_pin) == outer_pin,
            'genuine raw report physical pins changed during finite-set registration')
    report, outer = originals
    rootfs = Path(report['reused_COPY_after']['reused_root'])
    expected_error = "ValueError('bounded regular file required: " + str(rootfs / 'dev/null') + "')"
    historical_provenance_only = []
    require(report.get('schema') == 'uwvm-embedded-js-native-products-v1' and report['status'] == outer['status'] == 'FAIL' and
            report['error'] == expected_error and ', line 1236, in main' in report['traceback'] and
            report.get('qualified_source_translation_units') == 74 and len(report['product_pins']) == 552 and len(report['commands']) == 85 and
            report['remaining_mounts'] == [] and report['derived_roots_after'] == [] and
            report['execution_view_preserved'] is True and report['reused_execution_sdk_preserved'] is True and
            outer['reused_execution_sdk_preserved'] is True,
            'exact post-build Native6 ordinary-file-collector failure required')
    for owner in (report, outer):
        require(owner.get('finalization_errors') == [] and all(owner.get(key) is True for key in
                ('source_preserved', 'inputs_preserved', 'products_preserved')) and
                owner['cleanup']['status'] == 'PASS' and owner['cleanup'].get('remaining') == [] and
                not owner['cleanup'].get('errors') and owner['source_before'] == owner['source_after'] and
                owner['source'] == report['source'] and owner['source_manifest_sha256'] == report['source_manifest_sha256'] and
                owner['reused_COPY_before'] == owner['reused_COPY_after'],
                'genuine terminal failed-run source/input/product/SDK preservation and owned cleanup required')
        old_before, old_after = owner['cgroup_before'], owner['cgroup_after']
        require(old_before['boot_id'] == ORIGINAL_BOOT and old_before['device'] == 29 and old_before['inode'] == 18315 and
                old_before['memory_max'] == 68719476736 and old_before['memory_swap_max'] == 0 and old_before['cpus'] == '0-31' and
                all(all(row.get('events', {}).get(name, 0) == 0 for name in ('oom', 'oom_kill', 'oom_group_kill'))
                    for row in old_before['ancestors']), 'exact genuine historical Native6 cgroup facts required')
        frame.common.cgroup_preserved(old_before, old_after)
        for key in ('Runtime_executed', 'compiled_object_reuse', 'SDK_COPY_performed'):
            require(owner.get(key) is False, 'retained scope cannot admit new execution or old object reuse')
        for pin in owner['input_pins']:
            if pin == HISTORICAL_DNS_PROVENANCE:
                # The exact genuine acquisition DNS snapshot is ephemeral
                # host provenance, not a source/MD/link/map input. No current
                # identity or bytes are claimed for it after a reboot. This
                # exception exists ONLY in this provenance ledger loop: every
                # actual compiler dependency/link/execution path still uses
                # the unconditional strict accessor and cannot use it.
                historical_provenance_only.append({'historical_pin': pin,
                    'current_qualification': 'NOT_PERFORMED_HISTORICAL_ACQUISITION_DNS_PROVENANCE_ONLY',
                    'execution_or_compiler_input_authority': False})
            else:
                evidence.retained(pin)
        evidence.retained(owner['product_pins'])
    require(outer['native_configuration'] == report['configuration'] and len(outer['commands']) == 1 and
            outer['commands'][0]['status'] == 'FAIL' and outer['commands'][0]['returncode'] == 1 and
            report['typed_peer_executed'] is report['typed_example_cases_qualified'] is False and
            report['physics_controller_executed'] is report['physics_controller_DDS_initialized'] is False,
            'outer must bind failed inner child; no typed transport/physics execution may be claimed')
    argv = outer['commands'][0]['argv']
    for option, value in (('--out', str(Path(inner_pin['path']).parent)), ('--config', report['configuration']['path']),
                          ('--config-sha256', report['configuration']['sha256']), ('--source', report['source']),
                          ('--manifest', report['source_before']['manifest']['path']), ('--manifest-sha256', report['source_manifest_sha256'])):
        require(argv.count(option) == 1 and argv[argv.index(option) + 1] == value, 'actual original outer/inner argv binding differs')
    require(all(row['status'] == 'PASS' and row['returncode'] == row['expected_returncode'] == 0 for row in report['commands']) and
            report['mount_cleanup_processes']['status'] == 'PASS' and report['mount_cleanup_processes']['remaining'] == [],
            'all original build/export/unmount commands must have completed under their original scope')
    evidence.retained(report['producer_reports'])
    evidence.retained(report['configuration'])
    evidence.retained(outer['worker'])
    qualifier = Qualifier(frame, api, admission, evidence, report, outer)
    qualifier.current_mount_namespace = context['mount_namespace']
    qualifier.setup(inner_pin)
    at_rest = qualifier.at_rest()
    inventory = qualifier.inventories()
    reused_copy = qualifier.reused_copy()
    reused_directories = qualifier.reused_directories()
    for old in report['execution_loader'].values():
        if type(old) is dict and 'input' in old:
            qualifier.elf(old, {2, 3})
    role_proofs = [qualifier.compile(*row) for row in sections(report)]
    require(sum(row['translation_units'] for row in role_proofs) == 74 and
            [row['translation_units'] for row in role_proofs] == [1, 34, 34, 1, 1, 1, 1, 1], 'all74 distinct actual target TUs required')
    catalog = qualifier.execution_catalog(role_proofs)
    derived = []
    for index, relative in enumerate(('gateways', 'peer', 'exec-gate', 'string-identity', 'typed-peer', 'physics-controller')):
        row = frame.common.catalog(qualifier.work / relative, frame.out / ('derived-current-%02d.jsonl' % index), frame.roots)
        qualifier.product(row['catalog']['path'])
        derived.append(row)
    require(qualifier.at_rest() == at_rest, 'selected at-rest placeholders or namespace changed during metadata qualification')
    # Full restart bridges are metadata in an explicitly pinned sidecar, not
    # repeated inline tens of thousands of times. This keeps the ordinary
    # consumer report below its existing64MiB contract without omitting proof.
    bridges = frame.out / 'restart-device-bridges.jsonl'
    with bridges.open('xb') as stream:
        for row in evidence.bridges:
            stream.write((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode())
        stream.flush()
        os.fsync(stream.fileno())
    bridge_pin = qualifier.product(bridges)
    # Multiple independent reads may admit the same actual full7. Preserve
    # every distinct physical name once; differing observations are fatal.
    unique = {}
    for pin in frame.pins:
        require(pin['path'] not in unique or unique[pin['path']] == pin, 'same selected physical name changed between independent reads')
        unique[pin['path']] = pin
    frame.pins[:] = list(unique.values())
    require(all(frame.common.opened(Path(pin['path'])) == pin for pin in qualifier.products.values()), 'fresh at-rest catalog product changed')
    source_relations = {row['relative']: row for row in qualifier.original_source_relations}
    return {'schema': SCHEMA, 'original_raw_inner': inner_pin, 'original_raw_outer': outer_pin,
        'original_statuses': {'inner': 'FAIL', 'outer': 'FAIL'}, 'raw_reports_rewritten': False,
        'preparation_mode': 'retained74-at-rest', 'execution_performed': False, 'compilation_performed': False,
        'current_context': frame.inputs[selection['current_context']],
        'historical_execution_context': {'boot_id': ORIGINAL_BOOT, 'cgroup_device': 29, 'cgroup_inode': 18315,
            'scope': 'ORIGINAL_RAW_BEFORE_AFTER_MUTUAL_PRESERVATION_ONLY_NOT_CURRENT_CGROUP_QUALIFICATION'},
        'historical_provenance_only': historical_provenance_only,
        'mount_operations_performed': False, 'Guest_rights_changed': False,
        'retained_compilation': {'translation_units': 74, 'original_source': report['source'],
            'original_source_manifest_sha256': report['source_manifest_sha256'], 'roles': role_proofs,
            'scope': 'COMPLETE_RETAINED_R6_COMPILE_AND_LINK_EVIDENCE_REOPENED_NOT_A_SUCCESSFUL_OLD_BATCH'},
        'original_source_relations': [source_relations[key] for key in sorted(source_relations)],
        'retained_complete_inventories': inventory, 'retained_independent_SDK_COPY': reused_copy,
        'reused_SDK_directories_at_rest': reused_directories, 'at_rest': at_rest, **catalog,
        'derived_products_at_rest': {'scope': 'FRESH_CURRENT_COMPLETE_CATALOGS_NO_HISTORICAL_WHOLE_BUILD_TREE_BASELINE', 'inventories': derived},
        'execution_environment_base': report['execution_environment_base'],
        'restart_device_bridges': {'catalog': bridge_pin, 'rows': len(evidence.bridges),
            'scope': 'EXPLICIT_DEVICE_ONLY_CURRENT_PHYSICAL_REOPENS_RAW_REPORTS_UNCHANGED'}, 'products_preserved': True,
        'product_pins': list(qualifier.products.values()),
        'cleanup': [{'status': 'PASS', 'remaining': [], 'scope': 'METADATA_ONLY_NO_ACTOR_OR_MOUNT_STARTED'}],
        'scope': 'Retained74 Native6 compile/link products plus a fresh complete at-rest map-input catalog only; raw failed batch remains FAIL; no Runtime, DDS, simulator, physical robot or performance execution'}
