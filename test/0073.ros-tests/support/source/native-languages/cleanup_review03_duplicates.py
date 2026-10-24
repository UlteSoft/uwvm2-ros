#!/usr/bin/env python3
"""Verify both immutable archives before removing fixed Task03 duplicates.

Only reproducible private source copies and Nim intermediate C/object caches
are selected. Original inputs, tools, Wasm products, logs and archives remain.
Run exclusively inside the unchanged shared finite resource owner.
"""

from pathlib import Path
import types


def main():
    """Preflight all archive entries, kinds, owners and hashes before deletion."""
    path = Path(__file__).parent / 'clean_archived_intermediates.py'
    module = types.ModuleType('_task03_review03_duplicate_cleanup')
    module.__file__ = str(path)
    exec(compile(path.read_bytes(), str(path), 'exec', dont_inherit=True), module.__dict__)
    module.RUNS = {
        '20261021-round03-nim-docs-r1': ('source', 'generated'),
        '20261021-round03-nim-core-docs-r2': ('source',),
        '20261021-round03-patch-audit-r1': ('captured',),
        '20261021-round03-installed-nim-r2': (
            'example-ros1/native_languages/work/nim-cache',
            'example-ros2/native_languages/work/nim-cache',
        ),
    }
    module.main()


if __name__ == '__main__':
    main()
