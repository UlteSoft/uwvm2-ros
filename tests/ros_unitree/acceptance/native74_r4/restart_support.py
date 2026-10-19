#!/usr/bin/env python3
"""Explicit post-restart content admission for the Source39 native74 producer.

Raw reports, their historical cgroup snapshots and all statuses remain intact.
Only an explicitly selected pin comparison may bridge a retained file from its
known old device to the observed new device. There is no recursive report
projection, no executable/object reuse, and no cgroup continuity claim across
boots. Importing this module starts no actors and changes no cgroup setting.
"""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import sys
import types

sys.dont_write_bytecode = True
MIB = 1024**2
GIB = 1024**3
OLD_BOOT = 'af121220-8c6e-4302-9853-f08bf4c50dbb'
NEW_BOOT = 'ec0d5f0f-3060-4220-bee2-2a7791ed4e72'
PERSISTENT = '/var/tmp/uwvm-ros-complete-20261015-r1'
ARTIFACTS = PERSISTENT + '/acceptance-storage-r1'
CONTROLS = '/var/tmp/uwvm-ros-unitree-20261016-r1'
SOURCE = PERSISTENT + '/inputs-r39/source'
MF_SHA = '54d6c79978e36bd13fa37272d8dd5f2cc94a9d2e62ac437b51dfc4aa4f6ae5b9'
PIN_KEYS = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
ROOTS = [(ARTIFACTS, 81, 68), (PERSISTENT, 66307, 66308),
         (CONTROLS, 66307, 66308), ('/usr', 66307, 66308),
         ('/home/macromodel', 66307, 66308)]


def need(value, reason):
    if not value:
        raise ValueError(reason)


class RestartAdmission:
    """Keep finite historical/current bridges beside fresh current evidence.

    The caller supplies exact, physically staged helper and completed receipt
    pins through its separately hash-bound configuration. The completed receipt
    must requalify the same complete Source39 bytes in the new original cgroup.
    It is a prerequisite, never an execution or old SDK-completeness certificate.
    Every selected file and whole tree is independently reopened again here.
    """
    def __init__(self, selection, common, read, cg, source, manifest_pin):
        need(type(selection) is dict and set(selection) ==
             {'helper', 'parallel_helper', 'support', 'completed_receipt'},
             'closed explicit restart helper/receipt selections required')
        self.common, self.read, self.cg = common, read, dict(cg)
        self.before = common.cgroup_state(cg)
        need(self.before['boot_id'] == NEW_BOOT and source == Path(SOURCE) and
             manifest_pin['sha256'] == MF_SHA and cg['expected_inode'] == 18315 and
             cg['expected_device'] == 29, 'actual post-restart Source39/current cgroup selection required')
        self.inputs, self.entries, self.trees = [], {}, {}
        self.output, self.stream, self.terminal_pin = None, None, None
        self.helper = self._load(selection['helper'], 'uwvm_native74_restart_file_helper')
        self.parallel = self._load(selection['parallel_helper'], 'uwvm_native74_memory_parallel_helper', common_alias=True)
        _, own_pin = read(Path(__file__).resolve(strict=True), MIB)
        need(own_pin == selection['support'], 'physical reviewed restart support changed')
        self.inputs.append(own_pin)
        raw, receipt_pin = read(Path(selection['completed_receipt']['path']), 16*MIB)
        need(receipt_pin == selection['completed_receipt'], 'completed Root requalification receipt changed')
        receipt = json.loads(raw, object_pairs_hook=self._pairs)
        need(receipt.get('kind') == 'uwvm-ros-restart-content-requalification' and
             receipt.get('status') == 'CONTENT_REQUALIFIED' and
             receipt.get('execution_qualification') == 'NOT_PERFORMED' and
             receipt.get('historical_boot_id') == OLD_BOOT and
             receipt.get('historical_manifest_content_sha256') == MF_SHA and
             receipt.get('historical_source_root') == receipt.get('current_source_root') == SOURCE and
             receipt.get('current_inputs_preserved') is True and
             receipt.get('source_before') == receipt.get('source_after'),
             'complete content-only same-Source39 Root requalification is required')
        for name in ('cgroup_before', 'cgroup_after'):
            row = receipt[name]
            need(row['status'] == 'PASS' and row['cgroup_incarnation'] ==
                 {'path': cg['expected_leaf'], 'device': 29, 'inode': 18315, 'boot_id': NEW_BOOT},
                 'completed prerequisite belongs to another current cgroup incarnation')
            need(row['cgroup_ancestors'][0]['cpuset.cpus.effective'] == cg['expected_cpus'],
                 'completed prerequisite CPU selection differs')
        self.receipt_pin = receipt_pin
        self.receipt = receipt
        self.inputs.append(receipt_pin)
        self.guard()

    @staticmethod
    def _pairs(items):
        result = {}
        for key, value in items:
            need(key not in result, 'duplicate restart metadata key')
            result[key] = value
        return result

    def _load(self, pin, name, *, common_alias=False):
        need(type(pin) is dict and set(pin) == PIN_KEYS, 'actual seven-field helper pin required')
        raw, current = self.read(Path(pin['path']), MIB)
        need(current == pin, 'reviewed physical helper changed')
        module = types.ModuleType(name)
        module.__file__ = pin['path']
        prior = sys.modules.get('common')
        if common_alias:
            # The pure shared planner imports only common.require. Bind that
            # name to the already MF-admitted owner module for this import,
            # then restore the process module table before any actor starts.
            sys.modules['common'] = self.common
        try:
            exec(compile(raw, module.__file__, 'exec'), module.__dict__)
        finally:
            if common_alias:
                if prior is None:
                    del sys.modules['common']
                else:
                    sys.modules['common'] = prior
        self.inputs.append(current)
        return module

    def guard(self):
        self.common.cgroup_preserved(self.before, self.common.cgroup_state(self.cg))

    @staticmethod
    def _mapping(pin):
        for root, old, new in ROOTS:
            if pin['path'] == root or pin['path'].startswith(root + '/'):
                need(pin['device'] == old, 'unexpected historical filesystem device in explicit selected root')
                return root, old, new
        need(False, 'historical pin outside finite native74 persistent roots')

    def compare(self, actual, previous):
        """Compare fresh facts with one explicitly selected historical pin.

        Exact current/current comparisons require no bridge. A historical
        comparison fully reopens on first use, and caches only after the
        caller's independently opened current seven fields agree. Finalization
        freshly reopens every consumed current file again.
        """
        if actual == previous:
            return True
        need(type(previous) is dict and set(previous) == PIN_KEYS and
             type(actual) is dict and set(actual) == PIN_KEYS, 'closed selected file pin comparison required')
        root, old, new = self._mapping(previous)
        path = previous['path']
        if path in self.entries:
            bridge = self.entries[path]
            need(bridge['historical_pin'] == previous and bridge['current_pin'] == actual,
                 'selected historical/current pin changed after its first admission')
            self.guard()
            return True
        need(len(self.entries) < 131072, 'finite native74 selected historical pin closure exceeded')
        bridge = self.helper.requalify_persistent_pin(previous, path, self.guard,
            historical_device=old, current_device=new, observed_current=actual)
        bridge['root_mapping'] = {'root': root, 'historical_device': old, 'current_device': new}
        self.entries[path] = bridge
        if self.stream:
            self._append(bridge)
        return True

    def link_source(self, path, previous_device, previous_inode, target):
        """Admit the COPY ledger's narrower source-symlink provenance.

        This ledger did not record old link timestamps or COPY-link inodes.
        Those fields are not invented. Whole-root admission separately checks
        the exact link set/text; the fresh common.py catalogs record current
        link identities, resolved paths and independently opened target files.
        """
        info = path.lstat()
        _, old, new = self._mapping({'path': str(path), 'device': previous_device})
        need(info.st_dev == new and info.st_ino == previous_inode and
             path.is_symlink() and os.readlink(path) == target,
             'COPY source link changed beyond the explicit restart device renumbering')
        self.guard()
        return True

    def start(self, output):
        self.output = Path(output)
        need(self.stream is None and self.output.is_dir(), 'one fresh existing owner output required')
        self.ledger = self.output / 'restart-selected-file-bridges.jsonl'
        self.stream = self.ledger.open('xb')
        for path in sorted(self.entries):
            self._append(self.entries[path])

    def _append(self, row):
        raw = (json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode()
        need(len(raw) <= 32768 and self.stream.tell() + len(raw) <= 128*MIB,
             'finite retained restart bridge ledger exceeded')
        self.stream.write(raw)
        if self.stream.tell() % MIB < len(raw):
            self.stream.flush()

    def same_tree(self, current, previous):
        """Compare old semantic content, retaining a fresh physical catalog.

        common.py catalog hashes contain device numbers and cannot be compared
        across reboots. Require a matching complete prerequisite tree, then
        independently rehash its exact file/link content set in this owner.
        Keep that result and the current common.py physical catalog separately.
        Existing same-boot before/after equality remains unchanged.
        """
        need(self.output is not None and current['root'] == previous['root'],
             'same-path finite whole-root selection required')
        for key in ('files', 'symlinks', 'bytes'):
            need(current[key] == previous[key], 'historical whole-root count/size differs')
        matches = [row for row in self.receipt['trees'] if row['current_root'] == previous['root'] and
                   row['historical_catalog']['historical_pin'] == previous['catalog'] and
                   row['content_relation'] == 'EXACT_REGULAR_FILE_AND_SYMLINK_CONTENT_SET' and
                   row['execution_qualification'] == 'NOT_PERFORMED']
        need(len(matches) == 1, 'completed Root receipt lacks this exact historical whole tree')
        root = current['root']
        if root not in self.trees:
            need(len(self.trees) < 64, 'finite native74 restart tree closure exceeded')
            limits = {'file_bytes': 2*GIB, 'catalog_bytes':64*MIB, 'catalog_entries':100000,
                      'total_bytes':64*GIB, 'seconds':3600}
            result = self.helper.requalify_tree(previous['catalog'], root,
                self.output / ('restart-whole-tree-%02d.jsonl' % len(self.trees)), self.guard, limits)
            need(result['totals']['files'] == previous['files'] and
                 result['totals']['symlinks'] == previous['symlinks'] and
                 result['totals']['bytes'] == previous['bytes'], 'fresh semantic whole-root counts differ')
            self.trees[root] = result
        result = self.trees[root]
        # Independently compare the fresh strict catalog with this owner's
        # common.py catalog. Link targets are qualified only by common.py's
        # current opens, never by historical target_file observations.
        rows = {}
        raw, pin = self.read(Path(current['catalog']['path']), 64*MIB)
        need(pin == current['catalog'], 'fresh current physical tree catalog changed')
        for line in raw.splitlines():
            row = json.loads(line, object_pairs_hook=self._pairs)
            need(row['relative'] not in rows, 'duplicate current physical tree path')
            rows[row['relative']] = row
        strict_raw, strict_pin = self.read(Path(result['current_catalog']['path']), 64*MIB)
        need(strict_pin == result['current_catalog'], 'fresh strict semantic catalog changed')
        for line in strict_raw.splitlines():
            strict = json.loads(line, object_pairs_hook=self._pairs)
            row = rows.pop(strict['relative'])
            need(row['kind'] == strict['kind'], 'current physical tree kind differs')
            if row['kind'] == 'file':
                fresh = strict['current_pin']
                need(all(row[key] == fresh[key] for key in
                     ('bytes','sha256','device','inode','mtime_ns','ctime_ns')) and
                     row['executable'] == strict['executable'], 'current physical file differs from semantic admission')
            else:
                fresh = strict['current_link']
                need(all(row[key] == fresh[key] for key in
                     ('target','device','inode','mtime_ns','ctime_ns')), 'current physical symlink differs from semantic admission')
        need(not rows, 'current physical tree has extra files or symlinks')
        self.guard()
        return True

    def producer_provenance(self, label, previous, own_namespace, parent_namespace):
        """Retain old resource provenance without claiming cgroup continuity."""
        need(previous['boot_id'] == OLD_BOOT and previous['device'] == 29 and
             previous['inode'] == 18151 and previous['path'] == self.cg['expected_leaf'] and
             previous['memory_max'] == 64*GIB and previous['memory_swap_max'] == 0 and
             previous['cpus'] == '0-31', 'unexpected historical native input producer incarnation')
        need(all(row['events'].get(key, 0) == 0 for row in previous['ancestors']
                 for key in ('oom','oom_kill','oom_group_kill')), 'historical producer recorded an OOM event')
        need(own_namespace != parent_namespace and os.readlink('/proc/self/ns/mnt') == own_namespace and
             os.readlink('/proc/%d/ns/mnt' % os.getppid()) == parent_namespace,
             'current independent child/actual-parent mount namespaces required')
        self.guard()
        return {'producer':label, 'scope':'RETAINED_CONTENT_PROVENANCE_ONLY_NO_CGROUP_CONTINUITY_ACROSS_REBOOT',
                'historical_cgroup_after':previous, 'current_cgroup_before':self.before,
                'completed_content_receipt':self.receipt_pin, 'execution_qualification':'NOT_PERFORMED'}

    def build_plan(self, *, maximum=4):
        self.guard()
        return self.parallel.plan(self.common.cgroup_state(self.cg),
            {'jobs': maximum, 'rss_bytes':20*GIB}, {'UWVM_ROS_BUILD_JOB_RSS_BYTES':str(5*GIB)},
            self.parallel.current_affinity(), self.parallel.current_host_memory_available())

    def finish(self):
        """Reopen every consumed current input and seal the separate ledger."""
        for path in sorted(self.entries):
            row = self.entries[path]
            current = self.helper.opened_file(path, row['current_pin']['bytes'], self.guard)
            need(current == row['current_pin'], 'consumed retained input changed before final admission')
        for pin in self.inputs:
            _, current = self.read(Path(pin['path']), max(MIB, pin['bytes']))
            need(current == pin, 'restart helper or completed prerequisite changed')
        if self.stream:
            self.stream.flush()
            os.fsync(self.stream.fileno())
            self.stream.close()
            self.stream = None
            self.terminal_pin = self.read(self.ledger, 128*MIB)[1]
        return self.summary()

    def summary(self):
        return {'scope':'EXPLICIT_RETAINED_FILE_AND_WHOLE_TREE_CONTENT_ONLY',
                'completed_prerequisite':self.receipt_pin, 'helper_inputs':self.inputs,
                'selected_file_count':len(self.entries), 'file_bridge_ledger':self.terminal_pin,
                'whole_trees':list(self.trees.values()),
                'historical_reports_modified':False, 'historical_execution_statuses_promoted':False,
                'cgroup_continuity_across_reboot_claimed':False, 'compiled_object_reuse':False}
