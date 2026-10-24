#!/usr/bin/env python3
"""Add finite CURRENT file aliases to an unchanged historical hardlink union.

This policy selects regular files, never their containing directories. Every
selected name is a caller-pinned actual full-seven observation bound to one
genuine old inode class. Unrelated siblings are not visited, inventoried or
authorized. Actual global link counts must account for every old and selected
current name; an additional unknown link still refuses the entire class.

The factory verifies the loaded base and union source bytes before deriving a
class. The existing complete auxiliary-root policy remains separate: callers
may retain a complete ARM tool directory without claiming that a debug file's
parent is a complete auxiliary input. No program or filesystem mutation is
performed by this module, except the inherited owned metadata catalogs.
"""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path


BASE_SHA256 = '0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260'
UNION_SHA256 = '8fbc2d1da44ef07bd8e6f69f4b174df70d802c471e989c1e752073cc95627d61'
POLICY = 'current-exact-files-bound-to-old-anchors'


def need(value, reason):
    if not value:
        raise ValueError(reason)


def ExactAliasUnion(base, union_module, selections, guard, limits, *,
                    device_transitions, auxiliary_roots=None, exact_aliases,
                    maximum_roots=128, maximum_names=500000,
                    maximum_catalog_bytes=128 * 1024**2):
    """Return a union with separately selected exact current regular aliases.

    ``exact_aliases`` is a nonempty list of at most64 closed records:
    ``{current_pin: full7, historical_anchor: {device, inode}}``. The supplied
    current pin must equal a fresh full descriptor read, including path, size,
    SHA256, device, inode and both timestamps. A historical anchor must exist
    in the selected genuine old catalogs. Its old bytes/hash/timestamps/inode
    and executable bit remain mandatory through the inherited class observer.

    Call ``qualify_auxiliary`` once before any historical tree walk, including
    when no complete auxiliary roots are selected. It still returns only the
    complete-root receipts. Exact files appear separately in ``evidence``.
    Finish every old root and call ``finish`` before compiler admission, then
    ``preserve`` after the consuming work. This factory grants no Frame root,
    executable/DSO permission, Guest right, or execution qualification.
    """
    bindings = {}
    for label, module, expected in (('base', base, BASE_SHA256),
                                    ('union', union_module, UNION_SHA256)):
        pin = base.opened_file(str(Path(module.__file__).absolute()), 1024**2, guard)
        need(pin['sha256'] == expected, 'exact reviewed ' + label + ' helper bytes required')
        bindings[label] = pin

    class SelectedAliasUnion(union_module.HistoricalUnion):
        def __init__(self):
            self.exact_names, self.exact_selections, self.exact_receipts = {}, {}, {}
            self.exact_aliases_qualified = False
            super().__init__(base, selections, guard, limits,
                device_transitions=device_transitions, auxiliary_roots=auxiliary_roots,
                maximum_roots=maximum_roots, maximum_names=maximum_names,
                maximum_catalog_bytes=maximum_catalog_bytes)
            need(type(exact_aliases) is list and 1 <= len(exact_aliases) <= 64,
                 'finite nonempty exact current alias selection required')
            for selected in exact_aliases:
                need(type(selected) is dict and set(selected) == {'current_pin', 'historical_anchor'},
                     'closed exact current alias and old anchor required')
                current = base.historical_pin(selected['current_pin'])
                name = base.clean_absolute(current['path'])
                anchor = selected['historical_anchor']
                need(type(anchor) is dict and set(anchor) == {'device', 'inode'} and
                     type(anchor['device']) is int and anchor['device'] >= 0 and
                     type(anchor['inode']) is int and anchor['inode'] > 0,
                     'closed genuine old device/inode anchor required')
                identity = anchor['device'], anchor['inode']
                need(identity in self.groups and name not in self.by_name and name not in self.exact_selections and
                     all(not Path(name).is_relative_to(Path(root)) for root in self.auxiliary) and
                     Path(name).resolve(strict=True) == Path(name),
                     'exact alias must be a unique physical new name outside complete auxiliary roots')
                need(current['device'] == self.transitions[identity[0]] and current['inode'] == identity[1],
                     'exact selected current alias must match its old class and explicit device history')
                self.exact_names.setdefault(identity, set()).add(name)
                self.exact_selections[name] = {'current_pin': current,
                    'historical_anchor': {'device': identity[0], 'inode': identity[1]}}
            self.original_union_selection_sha256 = self.selection_sha256
            selected = {'original_union_selection_sha256': self.selection_sha256,
                'exact_current_alias_policy': POLICY,
                'exact_current_aliases': [self.exact_selections[name] for name in sorted(self.exact_selections)]}
            self.selection_sha256 = hashlib.sha256(json.dumps(selected, sort_keys=True, separators=(',', ':')).encode()).hexdigest()

        def all_members(self, identity):
            return sorted(set(super().all_members(identity)) | self.exact_names.get(identity, set()))

        def _read_exact_aliases(self, *, collecting=False):
            for name, selected in sorted(self.exact_selections.items()):
                self.guard()
                parent, leaf = self.base._parent(name)
                try:
                    pin, _, info = self.base._opened_at(parent, leaf, name,
                        self.limits['file_bytes'], self.guard, no_hardlinks=False)
                    self.base._verify_named_file(name, parent, leaf, info)
                finally:
                    os.close(parent)
                need(pin == selected['current_pin'], 'selected exact current alias full7 changed')
                anchor = selected['historical_anchor']
                self._observe_class(name, (anchor['device'], anchor['inode']), pin, info,
                                    check_count=not collecting)
                receipt = {'policy': POLICY, 'historical_anchor': dict(anchor), 'current_pin': dict(pin),
                    'current_mode': info.st_mode, 'current_nlink': info.st_nlink,
                    'historical_auxiliary_names': 'NOT_RECORDED', 'historical_auxiliary_nlink': 'NOT_RECORDED',
                    'sibling_directories': 'NOT_SELECTED', 'execution_qualification': 'NOT_PERFORMED'}
                need(collecting or self.exact_receipts.get(name) == receipt,
                     'exact current alias mode/link count or receipt changed')
                if collecting:
                    self.exact_receipts[name] = receipt

        def qualify_auxiliary(self, output_directory):
            need(not self.exact_aliases_qualified and not self.exact_receipts and not self.receipts,
                 'exact alias qualification must precede all historical root admission')
            # Names are finite before either observer runs. Complete auxiliary
            # discovery may add other members of a class; its existing second
            # walk closes counts. This final exact-file read then checks the
            # same global counts and the caller's selected actual full7 again.
            self._read_exact_aliases(collecting=True)
            complete_roots = super().qualify_auxiliary(output_directory)
            self._read_exact_aliases()
            self.exact_aliases_qualified = True
            return complete_roots

        def observe(self, root, relative, pin, info):
            need(self.exact_aliases_qualified, 'finite exact current alias proof is mandatory before historical walks')
            super().observe(root, relative, pin, info)

        def finish(self):
            need(self.exact_aliases_qualified, 'cannot finish without exact current alias admission')
            self._read_exact_aliases()
            super().finish()
            self._read_exact_aliases()
            return self.evidence

        @property
        def evidence(self):
            result = dict(super().evidence)
            # Keep complete-root auxiliary names separate from exact selected
            # files. The old catalogs recorded neither additional name set.
            for row in result['current_classes']:
                identity = row['historical_device'], row['historical_inode']
                names = sorted(self.exact_names.get(identity, set()))
                row['current_exact_alias_members'] = names
                row['historical_exact_alias_names'] = 'NOT_RECORDED' if names else 'NOT_SELECTED'
                row['historical_exact_alias_nlink'] = 'NOT_RECORDED' if names else 'NOT_SELECTED'
            result.update(exact_current_alias_policy=POLICY,
                exact_current_aliases=[self.exact_receipts[name] for name in sorted(self.exact_receipts)],
                exact_aliases_qualified=self.exact_aliases_qualified,
                original_union_selection_sha256=self.original_union_selection_sha256,
                exact_alias_helper_bindings=bindings)
            return result

        def preserve(self):
            need(self.exact_aliases_qualified, 'cannot preserve incomplete exact current alias admission')
            self._read_exact_aliases()
            result = super().preserve()
            self._read_exact_aliases()
            need(self.evidence == result, 'exact current alias evidence changed during terminal preservation')
            for pin in bindings.values():
                need(base.opened_file(pin['path'], 1024**2, guard) == pin, 'reviewed helper source changed')
            return self.evidence

    return SelectedAliasUnion()
