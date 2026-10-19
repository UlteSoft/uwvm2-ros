#!/usr/bin/env python3
"""Fresh all-package extraction seeded only by authenticated CoW file bytes.

The interrupted r5 report, ledger and SDK are immutable failed-run provenance.
No old package count, status, ledger checkpoint, process or cgroup snapshot is
continued. After fresh signed repositories and an empty-status solver, ALL513
signed DEBs are freshly decoded into a complete virtual tar model. Only actual
old normal files/links equal to that authenticated model may seed a new CoW SDK.
Owned pending temporaries and deferred hardlink destinations are not copied.
The maintained SafeTree starts with counters0, pending[], and an empty ledger,
and freshly extracts ALL513 packages. Its complete ledger and physical final
tree must match the independently generated model before any SDK success.
"""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import stat

MIB, GIB = 1024 ** 2, 1024 ** 3
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
TEMP = re.compile(r'\.uwvm-acquire-[0-9a-f]{16}\.pending\Z')


def need(value, message):
    if not value: raise ValueError(message)


def facts(info):
    return (info.st_dev, info.st_ino, info.st_mode, info.st_size,
            info.st_mtime_ns, info.st_ctime_ns, info.st_nlink)


def directory(path):
    path = Path(path); need(path.is_absolute() and '..' not in path.parts, 'explicit physical directory required')
    descriptor = os.open(path.anchor, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC)
    try:
        for name in path.parts[1:]:
            other = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=descriptor)
            os.close(descriptor); descriptor = other
        return descriptor
    except BaseException:
        os.close(descriptor); raise


def directory_names(held, guard):
    """Observe names through an independent open file description.

    The caller's held directory can predate tree population or have an
    enumeration cursor consumed by an earlier audit. dup() would share
    that description/state. openat(held, ".") opens the same pinned inode
    independently, so neither observation starts from nor changes the
    caller's enumeration state. Metadata and exact-name comparisons stay
    mandatory; no retry, ignored name or timestamp tolerance is used.
    """
    guard(); before = os.fstat(held)
    reader = os.open('.', os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=held)
    try:
        need(facts(before) == facts(os.fstat(reader)), 'independent directory description changed identity')
        names = sorted(os.listdir(reader))
        need(facts(before) == facts(os.fstat(reader)) == facts(os.fstat(held)), 'directory changed during independent name observation')
        return names
    finally: os.close(reader)


class GeneratedLedger:
    """Record fresh signed tar rows; never compare to an old partial ledger."""
    def __init__(self, path):
        self.stream = Path(path).open('xb'); self.offset = self.rows = 0
        self.next_row = object()  # Only non-partial VirtualTree.audit is used.
        self.digest = hashlib.sha256(); self.package_max_file = 0

    def match(self, row):
        raw = (json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode()
        self.stream.write(raw); self.digest.update(raw); self.offset += len(raw); self.rows += 1
        if row['kind'] == 'file': self.package_max_file = max(self.package_max_file, row['bytes'])

    def close(self):
        self.stream.flush(); os.fsync(self.stream.fileno()); self.stream.close()


class ResumeSession:
    """Keep the old producer integration API while doing a fresh rebuild."""
    def __init__(self, *, control, restart, maintained, common, guard, safety_guard, remember, product,
                 report, run, decode, tools, limits, out, inputs, products, model_module, storage_module, storage_session):
        self.control, self.restart, self.maintained, self.common = control, restart, maintained, common
        self.guard, self.safety_guard, self.remember, self.product = guard, safety_guard, remember, product
        self.report, self.run, self.decode, self.tools = report, run, decode, tools
        self.limits, self.out, self.inputs, self.products = limits, Path(out), inputs, products
        self.model_module, self.storage_module, self.storage = model_module, storage_module, storage_session
        self.model = None; self.expected = None; self.fd = None; self.source_snapshot = None
        self.audit_dir = self.out / 'fresh-signed-tar-audit'; self.audit_dir.mkdir(mode=0o700)
        self.control_logs = {}; self.tar_sizes = []; self.max_files = []; self.link_groups = {}; self.hardlink_destinations = set()

    def document(self, expected):
        need(type(expected) is dict and set(expected) == PIN and expected['bytes'] <= 16 * MIB, 'current bounded full7 report required')
        actual, raw = self.restart.opened_file(expected['path'], 16 * MIB, self.safety_guard, collect=True)
        need(actual == expected and self.remember(actual['path'], actual['sha256']) == actual, 'current selected report changed')
        def pairs(rows):
            result = {}
            for key, value in rows:
                need(key not in result, 'duplicate rebuild metadata key'); result[key] = value
            return result
        return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite rebuild metadata'))

    def prerequisites(self):
        need(type(self.control) is dict and set(self.control) == {'format_version', 'kind', 'old_report', 'old_outer_report',
            'old_ledger', 'old_partial_sdk', 'source_manifest_sha256', 'committed_packages', 'complete_packages', 'owned_storage'} and
            self.control['format_version'] == 1 and self.control['kind'] == 'noetic-fresh-all513-authenticated-CoW-subset-v1' and
            self.control['committed_packages'] == 0 and self.control['complete_packages'] == 513,
            'closed fresh-all513 rebuild control required; no old completed prefix is admitted')
        old, outer = self.document(self.control['old_report']), self.document(self.control['old_outer_report'])
        need(old.get('status') == 'RUNNING' and outer.get('status') == 'FAIL' and
             old.get('source_manifest_sha256') == self.control['source_manifest_sha256'] and
             outer.get('source_manifest_sha256') == self.control['source_manifest_sha256'] and
             outer.get('cleanup', {}).get('status') == 'PASS' and not outer.get('cleanup', {}).get('remaining') and
             not old.get('acquisition_complete'), 'only genuine interrupted r5 acquisition provenance is accepted')
        previous_ledger = self.control['old_ledger']
        need(set(previous_ledger) == PIN and self.remember(previous_ledger['path'], previous_ledger['sha256']) == previous_ledger,
             'current original interrupted ledger changed')
        selected = self.control['old_partial_sdk']
        need(set(selected) == {'path', 'device', 'inode', 'mode', 'mtime_ns', 'ctime_ns'} and
             Path(selected['path']) == Path(self.control['old_report']['path']).parent / 'sdk', 'explicit current old partial SDK directory required')
        self.fd = directory(selected['path']); current = os.fstat(self.fd)
        need({'path': selected['path'], 'device': current.st_dev, 'inode': current.st_ino, 'mode': current.st_mode,
              'mtime_ns': current.st_mtime_ns, 'ctime_ns': current.st_ctime_ns} == selected, 'current partial SDK root changed')
        self.report['continuation'] = {'kind': self.control['kind'], 'old_report': self.control['old_report'],
            'old_outer_report': self.control['old_outer_report'], 'old_ledger': previous_ledger,
            'actual_old_inner_status': old['status'], 'actual_old_outer_status': outer['status'],
            'historical_reported_package_rows': len(old.get('packages', [])), 'historical_rows_admitted_as_completed': 0,
            'old_cgroup_or_process_continuity': 'NOT_CLAIMED', 'old_status_promoted': False,
            'new_SafeTree_initial_state': {'entries': 0, 'bytes': 0, 'sequence': 0, 'pending': []},
            'new_payload_ledger_initial_bytes': 0, 'all_packages_freshly_extracted': False,
            'old_SDK_renamed_or_modified': False, 'target_program_executed': False, 'compatibility_qualified': False}

    def cache_controls(self, cache_index, ordered):
        signed = {}
        for row in ordered: signed.setdefault((row['bytes'], row['sha256']), []).append(row)
        observations = []
        for sequence, (key, pins) in enumerate(sorted(cache_index.items())):
            need(key in signed and pins, 'retained DEB outside freshly authenticated closure')
            chosen = sorted(pins, key=lambda row: row['path'])[0]
            command = self.run('fresh-cache-%05d-control' % sequence, [self.tools['dpkg_deb'], '--field', chosen['path']])
            values = list(self.maintained.file_paragraphs(command['log']['path']))
            need(len(values) == 1 and all(all(values[0].get(k) == row[v] for k, v in
                (('Package', 'package'), ('Version', 'version'), ('Architecture', 'architecture'))) for row in signed[key]),
                'current retained DEB fields differ from fresh signed authority')
            self.control_logs[key] = command['log']
            observations.append({'physical_current_files': pins, 'actual_control_log': command['log'], 'fresh_signed_packages': signed[key]})
        path = self.out / 'all-retained-DEBs-fresh-control-fields.json'
        self.common.atomic_json(path, {'kind': 'fresh-signed-retained-DEB-byte-and-control-observations', 'observations': observations})
        self.report['continuation']['retained_DEB_control_validation'] = self.product(path, 16 * MIB)

    def retire_audit_tar(self, path, pin, ordinal):
        need(path == self.audit_dir / ('%05d.data.tar' % ordinal) and
             self.common.opened(path, self.limits['max_tar_bytes']) == pin, 'only fresh exact owned audit tar may retire')
        row = self.report['commands'][-1]
        need(row.get('stdout_data') == pin and row['status'] == 'PASS' and row.get('returncode') == 0, 'actual terminal signed-tar decoder required')
        proof_path = self.audit_dir / ('%05d-authenticated-model-tar-retirement.json' % ordinal)
        self.common.atomic_json(proof_path, {'kind': 'fresh-signed-tar-complete-virtual-model-read-only-audit',
            'fresh_tar': pin, 'actual_decoder_argv': row['argv'], 'actual_same_pid_exec_argv': row['same_pid_exec_argv'],
            'actual_decoder_log': row['log'], 'actual_decoder_returncode': 0,
            'SafeTree_extract_executed': False, 'historical_partial_ledger_used': False,
            'fresh_generated_ledger_rows_so_far': self.expected.rows,
            'scope': 'FRESH_SIGNED_TAR_MODEL_ONLY_FINAL_NEW_EXTRACTION_AND_TREE_CHECK_REQUIRED'})
        proof = self.product(proof_path, 16 * MIB)
        parent = directory(path.parent)
        try:
            observed = os.stat(path.name, dir_fd=parent, follow_symlinks=False)
            need((observed.st_dev, observed.st_ino, observed.st_size, observed.st_mtime_ns, observed.st_ctime_ns) ==
                 tuple(pin[key] for key in ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns')), 'fresh audit tar name changed')
            os.unlink(path.name, dir_fd=parent); os.fsync(parent)
        finally: os.close(parent)
        need(self.products.pop(pin['path'], None) == pin, 'fresh audit tar registry mismatch')
        del row['stdout_data']
        row['stdout_data_historical'] = {'physical_path': pin['path'], **{key: value for key, value in pin.items() if key != 'path'},
            'disposition': 'RETIRED_OWNED_AFTER_FRESH_SIGNED_VIRTUAL_MODEL_AUDIT', 'retained_proof': proof}
        row['stdout_data_retired_after_fresh_signed_model_audit'] = True
        self.common.atomic_json(self.out / 'commands.json', self.report['commands'])
        self.storage.trim('after-model-tar-%05d' % ordinal)

    def complete_model(self):
        nodes = self.model.nodes
        groups = {name: {name} for name, row in nodes.items() if row['kind'] == 'file'}
        owners = {name: name for name in groups}
        for parts, target, row in self.model.pending:
            source = '/'.join(self.model.parent(target[:-1]) + [target[-1]])
            destination = '/'.join(self.model.parent(parts[:-1]) + [parts[-1]])
            need(source in nodes and nodes[source]['kind'] == 'file' and destination not in nodes,
                 'fresh signed hardlink order/target conflicts with complete model')
            nodes[destination] = dict(nodes[source]); group = owners[source]
            owners[destination] = group; groups[group].add(destination); self.hardlink_destinations.add(destination)
            self.expected.match({**row, 'kind': 'hardlink-resolved', 'original_target': '/'.join(target)})
        self.link_groups = {name: frozenset(groups[owners[name]]) for name in owners}
        need(not any(TEMP.fullmatch(Path(name).name) for name in nodes), 'signed payload uses the reserved SafeTree temporary namespace')

    def reconcile(self, ordered, cache_index):
        need(len(ordered) == 513 and self.fd is not None, 'fresh complete513-package closure and explicit old root required')
        self.cache_controls(cache_index, ordered)
        self.model = self.model_module.VirtualTree(self.maintained, self.limits, self.guard)
        self.expected = GeneratedLedger(self.out / 'fresh-signed-expected-payloads.jsonl'); decoded = 0
        try:
            for ordinal, package in enumerate(ordered):
                self.storage.trim('before-model-tar-%05d' % ordinal); self.guard()
                candidates = cache_index.get((package['bytes'], package['sha256']), [])
                need(candidates, 'all513 model DEBs must be retained or separately acquired before model audit')
                chosen = sorted(candidates, key=lambda row: row['path'])[0]
                tar = self.audit_dir / ('%05d.data.tar' % ordinal)
                pin = self.decode('fresh-model-%05d-data-tar' % ordinal, tar, self.limits['max_tar_bytes'],
                    [self.tools['dpkg_deb'], '--fsys-tarfile', chosen['path']])
                decoded += pin['bytes']; need(decoded <= self.limits['max_total_tar_bytes'], 'fresh model decoded quota exceeded')
                self.expected.package_max_file = 0
                self.model.audit(tar, package['package'] + '=' + package['version'], self.expected, partial=False)
                self.tar_sizes.append(pin['bytes']); self.max_files.append(self.expected.package_max_file)
                self.retire_audit_tar(tar, pin, ordinal)
            self.complete_model()
        finally: self.expected.close()
        expected_pin = self.product(self.out / 'fresh-signed-expected-payloads.jsonl', 64 * MIB)
        self.report['continuation'].update(fresh_all513_virtual_model=True, expected_complete_ledger=expected_pin,
            authenticated_tar_sizes=self.tar_sizes, authenticated_largest_regular_files=self.max_files,
            read_only_model_decoded_bytes=decoded, old_ledger_used_as_authority=False)
        self.storage.trim('before-authenticated-subset-clone'); self.guard()
        self.clone_subset()
        tree = self.maintained.SafeTree(self.out / 'sdk', self.out / 'payloads.jsonl', self.limits, self.guard)
        need(tree.entries == tree.bytes == tree.sequence == 0 and tree.pending == [] and tree.ledger.tell() == 0,
             'all-fresh SafeTree must begin empty, never inherit old checkpoint state')
        return tree, [], decoded

    def old_rows(self, *, copy_to=None):
        root_first = os.fstat(self.fd); root = Path(self.control['old_partial_sdk']['path'])
        seen = 0
        def walk(source, relative, depth, destination=None):
            nonlocal seen
            self.safety_guard(); need(depth <= 128, 'old subset depth exceeded')
            before = os.fstat(source); names = directory_names(source, self.safety_guard)
            need(before.st_dev == root_first.st_dev, 'old partial SDK crossed filesystem')
            for name in names:
                self.safety_guard(); seen += 1; need(seen <= self.limits['max_entries'], 'old subset entry bound exceeded')
                path = relative + '/' + name if relative else name; expected = self.model.nodes.get(path)
                first = os.stat(name, dir_fd=source, follow_symlinks=False)
                need(first.st_dev == root_first.st_dev, 'old partial SDK crossed filesystem')
                base = {'relative': path, 'device': first.st_dev, 'inode': first.st_ino, 'mode': first.st_mode,
                        'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns, 'nlink': first.st_nlink}
                if stat.S_ISDIR(first.st_mode):
                    need(expected is not None and expected['kind'] == 'directory' and not first.st_mode & 0o7022,
                         'old directory is not an authenticated confined parent')
                    oldfd = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=source)
                    newfd = None
                    try:
                        need(facts(first) == facts(os.fstat(oldfd)), 'old directory changed while opening')
                        if destination is not None:
                            os.mkdir(name, first.st_mode & 0o777, dir_fd=destination)
                            newfd = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=destination)
                            os.fchmod(newfd, first.st_mode & 0o777)
                        yield {**base, 'kind': 'directory'}
                        yield from walk(oldfd, path, depth + 1, newfd)
                        if newfd is not None: os.fsync(newfd)
                    finally:
                        os.close(oldfd)
                        if newfd is not None: os.close(newfd)
                elif stat.S_ISLNK(first.st_mode):
                    target = os.readlink(name, dir_fd=source)
                    need(expected is not None and expected['kind'] == 'symlink' and target == expected['physical_target'] and
                         not target.startswith('/') and first.st_nlink == 1, 'old symlink differs from authenticated signed model')
                    resolved = posixpath.normpath(posixpath.join(relative or '.', target))
                    need(resolved != '..' and not resolved.startswith('../'), 'old relative alias escapes root')
                    if destination is not None: os.symlink(target, name, dir_fd=destination)
                    yield {**base, 'kind': 'symlink', 'target': target, 'copied': True}
                else:
                    need(stat.S_ISREG(first.st_mode) and first.st_nlink == 1 and first.st_size <= self.limits['max_file_bytes'], 'bounded ordinary old partial file required')
                    pending = bool(TEMP.fullmatch(name)) and expected is None
                    deferred = path in self.hardlink_destinations
                    need(pending or expected is not None and expected['kind'] == 'file', 'unknown non-temporary old SDK file refused')
                    sourcefd = os.open(name, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC, dir_fd=source)
                    targetfd = None
                    try:
                        need(facts(first) == facts(os.fstat(sourcefd)), 'old file changed while opening')
                        digest = hashlib.sha256(); count = 0
                        while True:
                            self.safety_guard(); block = os.read(sourcefd, MIB)
                            if not block: break
                            count += len(block); need(count <= first.st_size, 'old partial file grew'); digest.update(block)
                        need(count == first.st_size and facts(first) == facts(os.fstat(sourcefd)), 'old partial file changed')
                        if not pending:
                            need((count, digest.hexdigest(), first.st_mode & 0o777) ==
                                 (expected['bytes'], expected['sha256'], expected['mode']), 'old normal file differs from authenticated signed bytes/mode')
                        if destination is not None and not pending and not deferred:
                            self.guard()
                            targetfd = os.open(name, os.O_RDWR | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o600, dir_fd=destination)
                            clone = self.storage_module.clone_descriptors(sourcefd, targetfd, self.guard)
                            need((clone['bytes'], clone['sha256']) == (count, digest.hexdigest()), 'authenticated clone content differs')
                            os.fchmod(targetfd, expected['mode']); os.fsync(targetfd)
                        yield {**base, 'kind': 'file', 'bytes': count, 'sha256': digest.hexdigest(),
                            'copied': not pending and not deferred,
                            'exclusion': 'OWNED_PENDING_TEMPORARY_UNCHANGED_IN_OLD_TREE' if pending else
                            'DEFERRED_HARDLINK_RECREATED_BY_FRESH_FINISH' if deferred else None}
                    finally:
                        os.close(sourcefd)
                        if targetfd is not None: os.close(targetfd)
                need(facts(first) == facts(os.stat(name, dir_fd=source, follow_symlinks=False)), 'old named entry changed')
            need(names == directory_names(source, self.safety_guard) and facts(before) == facts(os.fstat(source)), 'old directory changed during subset admission')
        yield from walk(self.fd, '', 0, copy_to)
        need(facts(root_first) == facts(os.fstat(self.fd)) == facts(root.stat(follow_symlinks=False)), 'old source root changed')

    def clone_subset(self):
        digest = hashlib.sha256(); counts = {'copied_files': 0, 'copied_symlinks': 0, 'excluded_pending_files': 0, 'excluded_hardlink_destinations': 0}
        path = self.out / 'authenticated-CoW-subset-source-current.jsonl'
        # Finish a read-only inventory before touching the fresh destination.
        # Even a failed clone then retains an initial whole-source digest that
        # finalization can compare without writing another large catalog.
        with path.open('xb') as output:
            for row in self.old_rows():
                raw = (json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode(); output.write(raw); digest.update(raw)
                if row['kind'] == 'file':
                    if row['copied']: counts['copied_files'] += 1
                    elif row['exclusion'].startswith('OWNED_PENDING'): counts['excluded_pending_files'] += 1
                    else: counts['excluded_hardlink_destinations'] += 1
                elif row['kind'] == 'symlink': counts['copied_symlinks'] += 1
            output.flush(); os.fsync(output.fileno())
        self.source_snapshot = digest.hexdigest()
        self.report['continuation'].update(authenticated_subset=counts, old_source_current_snapshot=self.product(path, 64 * MIB),
            source_snapshot_sha256=self.source_snapshot, CoW_required=True, ordinary_copy_fallback=False)
        sdk = self.out / 'sdk'; sdk.mkdir(mode=0o700); target = directory(sdk)
        copied_digest = hashlib.sha256()
        try:
            for row in self.old_rows(copy_to=target):
                copied_digest.update((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode())
            os.fsync(target)
        finally: os.close(target)
        need(copied_digest.hexdigest() == self.source_snapshot, 'old complete authenticated subset changed during clone')
        self.report['continuation']['authenticated_subset_clone_completed'] = True

    def before_package(self, ordinal):
        self.storage.trim('before-fresh-package-%05d' % ordinal); self.guard()
        # Full model audit makes the exact decoded output and largest temporary
        # regular-file size known before each fresh extraction.
        additional = self.tar_sizes[ordinal] + self.max_files[ordinal] + 64 * MIB
        for path in (self.out, *self.out.parents):
            space = os.statvfs(path)
            need(space.f_bavail * space.f_frsize >= self.limits['min_free_bytes'] + additional,
                 'fresh package lacks eight-GiB reserve plus authenticated transient footprint')

    def verify_complete(self, tree):
        need(len(self.report['packages']) == 513 and (tree.entries, tree.bytes, tree.sequence) ==
             (self.model.entries, self.model.bytes, self.model.sequence), 'all513 fresh counters differ from signed model')
        actual = self.common.opened(self.out / 'payloads.jsonl', 64 * MIB); expected = self.report['continuation']['expected_complete_ledger']
        need((actual['bytes'], actual['sha256']) == (expected['bytes'], expected['sha256']), 'fresh complete payload ledger differs from independently signed tar model')
        verify_physical(self.model.nodes, self.link_groups, tree.rootfd, self.out / 'complete-fresh-SDK-signed-model.jsonl', self.guard, self.limits)
        self.report['continuation'].update(all_packages_freshly_extracted=True, fresh_package_count=513,
            complete_signed_model_tree=self.product(self.out / 'complete-fresh-SDK-signed-model.jsonl', 64 * MIB))

    def preserve(self):
        if self.source_snapshot is None: return
        digest = hashlib.sha256()
        for row in self.old_rows(): digest.update((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode())
        need(digest.hexdigest() == self.source_snapshot, 'old partial SDK changed after new CoW extraction')
        self.report['continuation']['old_SDK_preserved'] = True

    def close(self):
        if self.fd is not None: os.close(self.fd); self.fd = None


def verify_physical(nodes, groups, rootfd, output, guard, limits):
    """Require the exact complete signed name/content set and hardlink classes."""
    seen = set(); actual_groups = {}; owners = {}; first_root = os.fstat(rootfd)
    with Path(output).open('xb') as stream:
        def walk(fd, relative, depth):
            guard(); need(depth <= 128, 'complete SDK depth exceeded'); before = os.fstat(fd); names = directory_names(fd, guard)
            need(before.st_dev == first_root.st_dev, 'complete SDK crossed filesystem')
            for name in names:
                guard(); path = relative + '/' + name if relative else name
                need(path in nodes and path not in seen and len(seen) < limits['max_entries'], 'unexpected final SDK name')
                seen.add(path); expected = nodes[path]; first = os.stat(name, dir_fd=fd, follow_symlinks=False)
                need(first.st_dev == first_root.st_dev, 'complete SDK crossed filesystem')
                row = {'relative': path, 'kind': expected['kind'], 'device': first.st_dev, 'inode': first.st_ino,
                    'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns, 'mode': first.st_mode, 'nlink': first.st_nlink}
                if expected['kind'] == 'directory':
                    need(stat.S_ISDIR(first.st_mode) and not first.st_mode & 0o7022, 'final SDK directory type/mode differs')
                    child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=fd)
                    try:
                        need(facts(first) == facts(os.fstat(child)), 'final SDK directory changed while opening'); walk(child, path, depth + 1)
                    finally: os.close(child)
                elif expected['kind'] == 'symlink':
                    need(stat.S_ISLNK(first.st_mode) and first.st_nlink == 1 and os.readlink(name, dir_fd=fd) == expected['physical_target'], 'final signed symlink differs')
                    row['target'] = expected['physical_target']
                else:
                    selected = groups[path]; key = min(selected); physical = (first.st_dev, first.st_ino)
                    need(stat.S_ISREG(first.st_mode) and first.st_nlink == len(selected) and first.st_size == expected['bytes'] and
                         first.st_mode & 0o777 == expected['mode'], 'final signed regular metadata/link count differs')
                    need(key not in actual_groups or actual_groups[key] == physical, 'signed hardlink class split across actual inodes')
                    need(physical not in owners or owners[physical] == key, 'separate signed files unexpectedly share an inode')
                    actual_groups[key] = physical; owners[physical] = key
                    opened = os.open(name, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC, dir_fd=fd)
                    try:
                        need(facts(first) == facts(os.fstat(opened)), 'final file changed while opening'); digest = hashlib.sha256(); count = 0
                        while True:
                            guard(); block = os.read(opened, MIB)
                            if not block: break
                            count += len(block); need(count <= expected['bytes'], 'final file grew'); digest.update(block)
                        need(count == expected['bytes'] and digest.hexdigest() == expected['sha256'] and facts(first) == facts(os.fstat(opened)), 'final signed bytes differ')
                    finally: os.close(opened)
                    row.update(bytes=count, sha256=digest.hexdigest())
                need(facts(first) == facts(os.stat(name, dir_fd=fd, follow_symlinks=False)), 'final SDK named entry changed')
                stream.write((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode())
            need(names == directory_names(fd, guard) and facts(before) == facts(os.fstat(fd)), 'final SDK directory changed')
        walk(rootfd, '', 0)
        need(seen == set(nodes) - {''}, 'final SDK is missing signed names'); stream.flush(); os.fsync(stream.fileno())
    need(facts(first_root) == facts(os.fstat(rootfd)), 'final SDK root changed')
