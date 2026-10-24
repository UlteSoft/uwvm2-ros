"""Reconstruct actual Git tree identities from complete recursive API entries.

GitHub's tree endpoint can echo a requested commit/treeish in its top-level
``sha`` field. That field must not be relabeled as the actual root tree object.
Every ordinary subtree is independently reconstructed and compared with its
declared Git identity. The root identity follows the same Git object encoding.
This utility reads metadata only and never checks out or executes source.
"""
import hashlib
import re


def tree_identity(document):
    assert document.get('truncated') is False
    entries, children = {}, {'': []}
    for row in document['tree']:
        name = row['path']
        assert isinstance(name, str) and name and not name.startswith('/')
        assert all(part not in ('', '.', '..') for part in name.split('/')) and '\0' not in name
        assert name not in entries and re.fullmatch('[0-9a-f]{40}', row['sha'])
        assert (row['type'], row['mode']) in {
            ('tree', '040000'), ('blob', '100644'), ('blob', '100755'),
            ('blob', '120000'), ('commit', '160000')}
        parent, _, leaf = name.rpartition('/')
        entries[name] = row
        children.setdefault(parent, []).append((leaf, row))
        if row['type'] == 'tree':
            children.setdefault(name, [])
    assert all(not name or name in entries and entries[name]['type'] == 'tree' for name in children)
    def compute(name):
        raw = bytearray()
        rows = sorted(children[name], key=lambda v: (v[0] + ('/' if v[1]['type'] == 'tree' else '')).encode())
        for leaf, row in rows:
            if row['type'] == 'tree':
                assert compute(row['path']) == row['sha'], ('Incomplete or changed subtree', row['path'])
            raw.extend(row['mode'].lstrip('0').encode() + b' ' + leaf.encode() + b'\0' + bytes.fromhex(row['sha']))
        return hashlib.sha1(b'tree ' + str(len(raw)).encode() + b'\0' + raw).hexdigest()
    return compute('')
