"""Report the actual matching build-Python import locations; no Guest runs."""
import argparse
import hashlib
import importlib.machinery
import json
import marshal
from pathlib import Path
import struct
import sys
import typing


def main():
    if sys.version_info[:3] != (3, 14, 8) or sys.platform != 'linux':
        raise ValueError('matching native CPython 3.14.8 on Linux required')
    modules = []
    for name, module in sorted(sys.modules.items()):
        spec = getattr(module, '__spec__', None)
        origin = getattr(spec, 'origin', None)
        row = {'name': name, 'origin': origin}
        if origin not in (None, 'built-in', 'frozen'):
            path = Path(origin).resolve(strict=True)
            raw = path.read_bytes()
            if len(raw) > 64 << 20:
                raise ValueError('bounded native module image required')
            row.update(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
        modules.append(row)
    print(json.dumps({'kind': 'ACTUAL_BUILD_PYTHON_IMPORT_PROBE_NOT_GUEST_EXECUTION',
                      'executable': sys.executable, 'version': sys.version,
                      'sys_path': sys.path, 'modules': modules}, indent=2))


if __name__ == '__main__':
    main()
