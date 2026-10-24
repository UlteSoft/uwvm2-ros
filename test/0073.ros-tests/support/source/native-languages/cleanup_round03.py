#!/usr/bin/env python3
"""@brief Reclaim fixed redundant source/C-cache copies after two-archive checks.

@details Reuse the exact ordinary-file, ownership, entry-set, byte-hash and
two-original-archive preflight from the existing cleanup helper. Tools, original
downloads/inputs, actual Wasm commands, logs and both immutable archive copies
remain untouched. Run only through the unchanged shared finite owner.
@see clean_archived_intermediates.py
"""
from pathlib import Path
import types


def main():
    """@brief Check every selected redundant directory before any deletion.

    @return Zero after exact checks and removal of only the fixed intermediates.
    @failure Any missing/changed entry, byte, archive or owner preserves all candidates.
    @see clean_archived_intermediates.main
    """
    path = Path(__file__).parent / 'clean_archived_intermediates.py'
    module = types.ModuleType('_task03_review03_cleanup')
    module.__file__ = str(path)
    exec(compile(path.read_bytes(), str(path), 'exec', dont_inherit=True), module.__dict__)
    module.RUNS = {
        '20261021-round03-source-r1': ('source', 'raw-products', 'records'),
        '20261021-round03-installed-nim-r1': ('source',),
        '20261021-native-corrected-r1': ('nim-cache-1', 'nim-cache-2'),
        '20261021-native-corrected-r4': ('nim-cache-1', 'nim-cache-2'),
        '20261021-native-corrected-r5': ('nim-cache-1', 'nim-cache-2'),
        '20261021-nim-odin-r1': ('nim-cache-1', 'nim-cache-2'),
    }
    module.main()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
