#!/usr/bin/env python3
"""Bounded read-only tool/product inventory, run by the shared finite owner."""
import hashlib
import json
import os
from pathlib import Path

roots = [Path('/host/home/macromodel/.local/share/uwvm-ros-extension'),
         Path('/host/home/macromodel/Documents/uwvm-test-261018/retained'),
         Path('/host/home/macromodel/.local/share'), Path('/usr/bin'),
         Path('/home/macromodel/Documents')]
names = {'dotnet', 'kotlinc', 'scala', 'scalac', 'java', 'javac', 'moon', 'grain',
         'dart', 'ruby', 'php', 'lua', 'R', 'wasm-tools', 'uwvm2-ros', 'uwvm2',
         'python.wasm', 'python.debug.wasm', 'libpython3.14.a', 'libpython3.14d.a',
         'FSharp.Core.dll', 'MonoAotCross', 'mono-aot-cross'}
rows, seen, count = [], set(), 0
for root in roots:
    if not root.is_dir():
        continue
    for folder, directories, files in os.walk(root):
        relative = Path(folder).relative_to(root)
        if len(relative.parts) >= 9:
            directories[:] = []
        directories[:] = sorted(d for d in directories if d not in {'node_modules', '.git', 'include', 'src', 'source', 'sources', 'archives', 'results'})
        for name in sorted(files):
            count += 1
            if count > 200000:
                raise RuntimeError('finite inventory entry bound reached')
            if name not in names:
                continue
            path = Path(folder) / name
            if path in seen:
                continue
            seen.add(path)
            try:
                info = path.stat()
                if info.st_size > 256 * 1024 * 1024:
                    rows.append({'path': str(path), 'bytes': info.st_size, 'hash': 'NOT_HASHED_SIZE_BOUND'})
                else:
                    with path.open('rb') as stream:
                        digest = hashlib.file_digest(stream, 'sha256').hexdigest()
                    rows.append({'path': str(path), 'bytes': info.st_size, 'sha256': digest})
            except OSError as error:
                rows.append({'path': str(path), 'read_error': str(error)})
report = {'kind': 'read-only-available-tool-products-v1', 'inspected_entries': count,
          'entries': rows, 'compiler_or_guest_execution': 'NOT_RUN'}
Path('tool-inventory.json').write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps(report, indent=2))
