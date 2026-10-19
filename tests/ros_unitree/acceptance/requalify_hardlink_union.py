#!/usr/bin/env python3
"""Explicit complete historical hardlink classes across inventoried roots.

This is a separate opt-in policy. It neither modifies nor weakens the reviewed
strict/intra-root restart helper. The caller supplies that exact loaded helper
and a finite set of complete old root catalogs. Only regular-file *names* in
those catalogs count; a symlink's target_file observation never adds an alias.

Every current alias preserves its old path/inode/timestamps/content and an
explicit device mapping. Its actual nlink must equal the number of distinct
physical names in the entire selected historical union. All selected roots
must be walked, then all multi-name files fully reopened before admission.
Preservation walks every root again and repeats the global topology check.
This metadata proof launches no process and qualifies no old execution.
"""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path


BASE_SHA256 = '0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260'
POLICY = 'historical-complete-inventoried-union'
AUXILIARY_POLICY = 'current-complete-root-bound-to-old-anchors'
PIN = frozenset(('path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'))


def need(value, reason):
    if not value:
        raise ValueError(reason)


class HistoricalUnion:
    """One finite source-pinned catalog union and its separate current proof.

    ``selections`` contains exact ``{root, catalog}`` records; ``catalog`` is an
    old full7 pin. Root spellings are physical and unchanged. Overlapping roots
    may repeat the same physical pathname, but it counts once and every old
    observation must agree. Device transitions are explicit old→current pairs,
    such as persistent host66307→66308 or artifact81→68 after a known restart.
    Neither historical nlink nor full historical mode is invented: old common
    catalogs recorded only device/inode, timestamps, content and executable bit.
    """
    def __init__(self, base, selections, guard, limits, *, device_transitions,
                 auxiliary_roots=None,
                 maximum_roots=128, maximum_names=500000, maximum_catalog_bytes=128 * 1024**2):
        self.base, self.guard, self.limits = base, guard, dict(limits)
        self.selections, self.expected, self.groups, self.by_name = {}, {}, {}, {}
        self.current, self.members, self.owners, self.receipts = {}, {}, {}, {}
        self.singleton_modes = {}
        self.initial_complete = False
        self.auxiliary, self.auxiliary_receipts, self.auxiliary_names = {}, {}, {}
        self.auxiliary_regular = {}
        need(type(selections) is list and 1 <= len(selections) <= maximum_roots <= 128 and
             type(maximum_names) is int and 1 <= maximum_names <= 1000000 and
             type(maximum_catalog_bytes) is int and 1 <= maximum_catalog_bytes <= 256 * 1024**2,
             'explicit finite complete catalog union bounds required')
        need(type(device_transitions) is dict and 1 <= len(device_transitions) <= 8 and
             all(type(old) is int and old >= 0 and type(new) is int and new >= 0
                 for old, new in device_transitions.items()), 'finite explicit device transitions required')
        self.transitions = dict(device_transitions)
        # The already loaded base still needs a current source-byte binding.
        pin = base.opened_file(str(Path(base.__file__).absolute()), 1024**2, guard)
        need(pin['sha256'] == BASE_SHA256, 'exact separately reviewed unchanged restart base required')
        self.base_pin = pin
        total_catalog_bytes, total_rows = 0, 0
        for selected in selections:
            need(type(selected) is dict and set(selected) == {'root', 'catalog'}, 'closed historical union selection required')
            root = base.clean_absolute(selected['root'])
            catalog = base.historical_pin(selected['catalog'])
            key = (root, catalog['sha256'])
            need(key not in self.selections, 'duplicate historical root/catalog selection')
            total_catalog_bytes += catalog['bytes']
            need(total_catalog_bytes <= maximum_catalog_bytes, 'aggregate historical union catalog-byte bound exceeded')
            groups = {}
            rows, bridge = base._old_catalog(catalog, guard, limits, hardlink_groups=groups)
            total_rows += len(rows)
            need(total_rows <= maximum_names, 'aggregate historical union entry bound exceeded')
            self.selections[key] = {'root': root, 'catalog': catalog, 'catalog_bridge': bridge}
            self.expected[key] = rows
            for identity, group in groups.items():
                need(identity[0] in self.transitions, 'historical union member has no explicit device transition')
                combined = self.groups.setdefault(identity, {'members': set(), 'metadata': group['metadata']})
                need(combined['metadata'] == group['metadata'], 'historical same-inode aliases disagree across catalogs')
                for relative in group['members']:
                    name = root + '/' + relative
                    prior = self.by_name.setdefault(name, identity)
                    need(prior == identity, 'one physical name has conflicting historical inode observations')
                    combined['members'].add(name)
        need(self.by_name, 'nonempty historical regular-name union required')
        for group in self.groups.values():
            group['members'] = sorted(group['members'])
        auxiliary_roots = [] if auxiliary_roots is None else auxiliary_roots
        need(type(auxiliary_roots) is list and len(auxiliary_roots) <= 8,
             'explicit bounded current-only auxiliary root selection required')
        self.anchor_by_current = {}
        for selected in auxiliary_roots:
            need(type(selected) is dict and set(selected) == {'root', 'historical_anchors'},
                 'closed complete current auxiliary root with finite old inode anchors required')
            root = base.clean_absolute(selected['root'])
            need(root not in self.auxiliary and all(not (Path(root).is_relative_to(Path(value['root'])) or
                 Path(value['root']).is_relative_to(Path(root))) for value in self.selections.values()) and
                 all(not (Path(root).is_relative_to(Path(other)) or Path(other).is_relative_to(Path(root)))
                     for other in self.auxiliary), 'auxiliary roots must be disjoint from all historical selections and each other')
            anchors = selected['historical_anchors']
            need(type(anchors) is list and 1 <= len(anchors) <= 64, 'finite known historical auxiliary anchors required')
            identities = set()
            for anchor in anchors:
                need(type(anchor) is dict and set(anchor) == {'device', 'inode'} and
                     type(anchor['device']) is int and type(anchor['inode']) is int and anchor['inode'] > 0,
                     'exact genuinely recorded historical device/inode anchor required')
                identity = anchor['device'], anchor['inode']
                need(identity in self.groups and identity not in identities, 'auxiliary anchor must be one distinct actual selected historical class')
                current = self.transitions[identity[0]], identity[1]
                need(current not in self.anchor_by_current, 'one explicit auxiliary root per known historical class required')
                self.anchor_by_current[current] = identity, root
                identities.add(identity)
                self.auxiliary_names[identity] = set()
            self.auxiliary[root] = {'root': root, 'historical_anchors': sorted(identities)}
        self.auxiliary_qualified = not bool(self.auxiliary)
        encoded = [{'root': value['root'], 'catalog': value['catalog']}
                   for _, value in sorted(self.selections.items())]
        selection = {'historical_complete_roots': encoded,
            'explicit_current_auxiliary_roots': [self.auxiliary[root] for root in sorted(self.auxiliary)]}
        self.selection_sha256 = hashlib.sha256(json.dumps(selection, sort_keys=True, separators=(',', ':')).encode()).hexdigest()

    @staticmethod
    def metadata(pin, info):
        return {**{key: pin[key] for key in PIN - {'path'}}, 'mode': info.st_mode, 'nlink': info.st_nlink}

    def all_members(self, identity):
        return sorted(set(self.groups[identity]['members']) | self.auxiliary_names.get(identity, set()))

    def _observe_class(self, name, identity, pin, info, *, check_count=True):
        need(pin['path'] == name, 'current physical alias pathname differs')
        group = self.groups[identity]
        old = group['metadata']
        need(pin['device'] == self.transitions[identity[0]] and pin['inode'] == identity[1] and
             all(pin[key] == old[key] for key in ('bytes', 'sha256', 'mtime_ns', 'ctime_ns')) and
             bool(info.st_mode & 0o111) == old['executable'], 'historical union member changed beyond explicit device transition')
        if check_count:
            need(info.st_nlink == len(self.all_members(identity)),
                 'current nlink differs from all distinct selected historical/current auxiliary names; external alias refused')
        if len(self.all_members(identity)) == 1:
            # The complete current catalog already records this singleton's
            # actual full7, and every later walk checks the full catalog digest.
            # Keep only its additional full-mode observation here, avoiding a
            # second full-pin dictionary per ordinary file in huge tool roots.
            previous_mode = self.singleton_modes.setdefault(identity, info.st_mode)
            need(previous_mode == info.st_mode, 'current singleton full mode changed during admission/preservation')
            return
        current_identity = pin['device'], pin['inode']
        prior_owner = self.owners.setdefault(current_identity, identity)
        need(prior_owner == identity, 'distinct historical union classes merged into one current inode')
        metadata = self.metadata(pin, info)
        previous = self.current.setdefault(identity, metadata)
        need(previous == metadata, 'historical union class split or current mode/nlink/timestamps changed')
        previous_pin = self.members.setdefault(identity, {}).setdefault(name, dict(pin))
        need(previous_pin == pin, 'overlapping root observation changed one actual physical alias')

    def observe(self, root, relative, pin, info):
        name = root + '/' + relative
        need(self.auxiliary_qualified and name in self.by_name, 'current file has no selected historical union name or auxiliary proof is missing')
        self._observe_class(name, self.by_name[name], pin, info)

    def _observe_auxiliary(self, root, relative, pin, info, *, collecting=False):
        name = root + '/' + relative
        current = pin['device'], pin['inode']
        if current in self.anchor_by_current:
            identity, expected_root = self.anchor_by_current[current]
            need(root == expected_root and name not in self.by_name, 'auxiliary alias is outside its finite anchored root or duplicates an old name')
            if collecting:
                self.auxiliary_names[identity].add(name)
            else:
                need(name in self.auxiliary_names[identity], 'new auxiliary hardlink name appeared after admission')
            self._observe_class(name, identity, pin, info, check_count=not collecting)
        else:
            # Current-only ordinary files unrelated to an anchor are fully
            # inventoried, but never authorize an unaccounted hardlink class.
            need(info.st_nlink == 1 and pin['path'] == name, 'unanchored multi-name auxiliary inode refused')
            value = {'pin': pin, 'metadata': self.metadata(pin, info)}
            if collecting:
                need(name not in self.auxiliary_regular, 'duplicate current auxiliary ordinary file')
                self.auxiliary_regular[name] = value
            else:
                need(self.auxiliary_regular.get(name) == value, 'current-only auxiliary ordinary file identity/mode changed')

    def qualify_auxiliary(self, output_directory):
        """Hash complete CURRENT auxiliary roots before any historical walk.

        Old catalogs establish the exact anchor inode/times/bytes/hash only.
        Additional alias names in these roots are current observations; their
        historical names and historical nlink are explicitly NOT_RECORDED.
        A second descriptor walk closes the initial discovery/count interval.
        """
        need(not self.initial_complete and not self.receipts and not self.auxiliary_receipts,
             'auxiliary discovery must precede every historical root admission')
        directory = self.base.clean_absolute(str(output_directory))
        need(Path(directory).is_dir() and Path(directory).resolve(strict=True) == Path(directory) and
             all(not (Path(directory).is_relative_to(Path(value['root'])) or Path(value['root']).is_relative_to(Path(directory)))
                 for value in self.selections.values()) and
             all(not (Path(directory).is_relative_to(Path(root)) or Path(root).is_relative_to(Path(directory))) for root in self.auxiliary),
             'owned auxiliary proof output must be disjoint from every retained root')
        for index, root in enumerate(sorted(self.auxiliary)):
            output = Path(directory) / ('current-auxiliary-%02d.jsonl' % index)
            with output.open('xb') as stream:
                def visit(row):
                    stream.write((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode())
                totals = self.base._walk(root, self.guard, self.limits, visit,
                    file_identity=lambda relative, pin, info: self._observe_auxiliary(root, relative, pin, info, collecting=True))
                stream.flush()
                os.fsync(stream.fileno())
            self.auxiliary_receipts[root] = {'policy': AUXILIARY_POLICY, 'current_root': root, 'totals': totals,
                'current_catalog': self.base.opened_file(str(output), self.limits['catalog_bytes'], self.guard),
                'historical_auxiliary_names': 'NOT_RECORDED', 'historical_auxiliary_nlink': 'NOT_RECORDED',
                'historical_anchor_classes': [{'device': item[0], 'inode': item[1]} for item in self.auxiliary[root]['historical_anchors']],
                'symlink_target_qualification': 'NOT_PERFORMED', 'execution_qualification': 'NOT_PERFORMED'}
        for identity, names in self.auxiliary_names.items():
            need(names and self.current[identity]['nlink'] == len(self.all_members(identity)),
                 'finite historical anchor plus complete current auxiliary names do not account for actual nlink')
        self.auxiliary_qualified = True
        self._preserve_auxiliary()
        return list(self.auxiliary_receipts.values())

    def _preserve_auxiliary(self):
        need(self.auxiliary_qualified and set(self.auxiliary_receipts) == set(self.auxiliary),
             'all explicit current auxiliary complete roots must qualify first')
        for root, receipt in self.auxiliary_receipts.items():
            digest, count = hashlib.sha256(), 0
            def visit(row):
                nonlocal count
                raw = (json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode()
                digest.update(raw)
                count += len(raw)
            totals = self.base._walk(root, self.guard, self.limits, visit,
                file_identity=lambda relative, pin, info: self._observe_auxiliary(root, relative, pin, info))
            need(totals == receipt['totals'] and count == receipt['current_catalog']['bytes'] and
                 digest.hexdigest() == receipt['current_catalog']['sha256'] and
                 self.base.opened_file(receipt['current_catalog']['path'], self.limits['catalog_bytes'], self.guard) == receipt['current_catalog'],
                 'current complete auxiliary root changed after finite discovery')

    def requalify_tree(self, previous_catalog, current_root, output_catalog):
        """Walk one registered complete root with the union's exact global count."""
        need(not self.initial_complete and self.auxiliary_qualified, 'initial union admission closed or mandatory current auxiliary proof missing')
        root = self.base.clean_absolute(current_root)
        previous = self.base.historical_pin(previous_catalog)
        key = root, previous['sha256']
        need(key in self.selections and self.selections[key]['catalog'] == previous and key not in self.receipts,
             'one exact registered original root/catalog may be admitted once')
        expected, seen = self.expected[key], set()
        with open(output_catalog, 'xb') as stream:
            def visit(row):
                relative = row['relative']
                content = {key: value for key, value in row.items()
                           if key not in ('current_pin', 'current_link', 'current_nlink')}
                need(relative in expected and relative not in seen and content == expected[relative],
                     'current complete root content differs from selected old catalog')
                seen.add(relative)
                stream.write((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode())
            totals = self.base._walk(root, self.guard, self.limits, visit,
                file_identity=lambda relative, pin, info: self.observe(root, relative, pin, info))
            need(seen == set(expected), 'historical union root is missing a complete old name')
            stream.flush()
            os.fsync(stream.fileno())
        receipt = {'historical_catalog': self.selections[key]['catalog_bridge'], 'current_root': root, 'totals': totals,
            'current_catalog': self.base.opened_file(str(output_catalog), self.limits['catalog_bytes'], self.guard),
            'content_relation': 'EXACT_REGULAR_FILE_AND_SYMLINK_CONTENT_SET',
            'symlink_target_qualification': 'NOT_PERFORMED', 'execution_qualification': 'NOT_PERFORMED',
            'hardlink_policy': POLICY, 'hardlink_union_selection_sha256': self.selection_sha256,
            'hardlink_global_finalization_required': True}
        self.receipts[key] = receipt
        return receipt

    def _reopen_classes(self):
        """Fully hash every multi-name alias again after all root observations."""
        for identity, group in self.groups.items():
            members = self.all_members(identity)
            if len(members) == 1:
                need(identity in self.singleton_modes, 'selected historical singleton was not observed')
                continue
            need(set(self.members.get(identity, {})) == set(members), 'selected union class has unobserved physical names')
            for name in members:
                self.guard()
                parent, leaf = self.base._parent(name)
                try:
                    pin, _, info = self.base._opened_at(parent, leaf, name, self.limits['file_bytes'], self.guard)
                    self.base._verify_named_file(name, parent, leaf, info)
                finally:
                    os.close(parent)
                need(pin == self.members[identity][name] and self.metadata(pin, info) == self.current[identity],
                     'union alias bytes/inode/mode/timestamps/nlink changed after complete root walks')

    def finish(self):
        """Close initial admission only after every selected complete root."""
        need(set(self.receipts) == set(self.selections), 'all registered complete roots must qualify before compilation')
        self._preserve_auxiliary()
        self._reopen_classes()
        self.initial_complete = True
        return self.evidence

    @property
    def evidence(self):
        classes = []
        for identity, group in sorted(self.groups.items()):
            members = self.all_members(identity)
            if len(members) == 1:
                continue
            need(identity in self.current, 'current union topology not yet observed')
            classes.append({'historical_device': identity[0], 'historical_inode': identity[1],
                'historical_members': list(group['members']), 'historical_recorded_metadata': group['metadata'],
                'historical_nlink': 'NOT_RECORDED', 'historical_full_mode': 'NOT_RECORDED',
                'historical_auxiliary_names': 'NOT_RECORDED' if identity in self.auxiliary_names else 'NOT_SELECTED',
                'current_auxiliary_members': sorted(self.auxiliary_names.get(identity, set())),
                'current_identity': self.current[identity],
                'current_members': [self.members[identity][name] for name in members],
                'current_complete_union_member_count': len(members),
                'external_current_hardlinks': 'NONE_BY_EXACT_CURRENT_NLINK'})
        return {'policy': POLICY, 'base_helper': self.base_pin, 'selection_sha256': self.selection_sha256,
                'registered_complete_roots': list(self.selections.values()), 'current_classes': classes,
                'current_auxiliary_policy': AUXILIARY_POLICY if self.auxiliary else 'NOT_SELECTED',
                'current_auxiliary_complete_roots': list(self.auxiliary_receipts.values()),
                'all_selected_roots_admitted': self.initial_complete,
                'execution_qualification': 'NOT_PERFORMED'}

    def preserve(self):
        """Repeat every full root and final global class proof, without mutation."""
        need(self.initial_complete, 'cannot preserve an incomplete union')
        before = self.evidence
        self._preserve_auxiliary()
        for key, receipt in self.receipts.items():
            root = receipt['current_root']
            expected, bridge = self.base._old_catalog(self.selections[key]['catalog'], self.guard, self.limits)
            need(expected == self.expected[key] and bridge == self.selections[key]['catalog_bridge'],
                 'selected original union catalog bytes or physical identity changed')
            digest, count = hashlib.sha256(), 0
            def visit(row):
                nonlocal count
                line = (json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode()
                digest.update(line)
                count += len(line)
            totals = self.base._walk(root, self.guard, self.limits, visit,
                file_identity=lambda relative, pin, info: self.observe(root, relative, pin, info))
            need(totals == receipt['totals'] and count == receipt['current_catalog']['bytes'] and
                 digest.hexdigest() == receipt['current_catalog']['sha256'], 'current complete union root changed before finish')
            need(self.base.opened_file(receipt['current_catalog']['path'], self.limits['catalog_bytes'], self.guard) ==
                 receipt['current_catalog'], 'current union catalog changed')
        self._reopen_classes()
        self._preserve_auxiliary()
        need(self.evidence == before, 'current full union topology changed during preservation')
        need(self.base.opened_file(self.base_pin['path'], 1024**2, self.guard) == self.base_pin, 'reviewed base source changed')
        return self.evidence
