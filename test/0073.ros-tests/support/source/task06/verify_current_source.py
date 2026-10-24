#!/usr/bin/env python3
"""Check current authored drivers/JSON and actual canonical JS source graph.

Finite Linux owner only. Acorn parses without importing/evaluating SDK modules.
This source-only packet neither replaces nor qualifies an SDK build/Guest run.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import subprocess


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('input', 'reference', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--include-dobot', action='store_true')
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same admitted finite Linux owner required')
    args.out.mkdir()
    files = [path for path in args.input.rglob('*') if path.is_file()]
    pins = [dict(path=str(path), bytes=path.stat().st_size, sha256=sha(path)) for path in files]
    pins += [dict(path=str(path), bytes=path.stat().st_size, sha256=sha(path)) for path in (
        Path(__file__), Path('/usr/bin/node'), Path('/usr/share/nodejs/acorn/dist/acorn.mjs'))]
    parsed = []
    for path in files:
        if path.suffix == '.py':
            ast.parse(path.read_bytes(), filename=str(path)); parsed.append(dict(path=str(path), kind='ACTUAL_PYTHON_AST'))
        elif path.suffix == '.json':
            json.loads(path.read_bytes()); parsed.append(dict(path=str(path), kind='ACTUAL_JSON_PARSE'))
    command = ['/usr/bin/node', args.input / 'inspect_canonical_js.mjs', args.input / 'sdk', args.reference, args.out / 'canonical-graph.json']
    if args.include_dobot:
        command.append('--include-dobot')
    with (args.out / 'graph.stdout').open('xb') as out, (args.out / 'graph.stderr').open('xb') as err:
        result = subprocess.run(list(map(str, command)), stdin=subprocess.DEVNULL, stdout=out, stderr=err, timeout=120, check=False)
    if result.returncode:
        raise RuntimeError('actual Acorn canonical AST/import source graph failed')
    for row in pins:
        if sha(Path(row['path'])) != row['sha256']:
            raise RuntimeError('source or actual parser tool changed')
    graph = json.loads((args.out / 'canonical-graph.json').read_bytes())
    (args.out / 'source-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
    (args.out / 'parsed-inputs.json').write_text(json.dumps(parsed, indent=2) + '\n')
    summary = dict(status='PASS_CURRENT_SOURCE_ONLY', parsed_python_json=len(parsed),
                   actual_canonical_js_ast_pairs=len(graph['authored_ast_pairs']), js_source_modules=graph['source_modules'],
                   sdk_builds=0, wasm_guest=0, native_ros=0, global_formatter_enrollment=False, publication_ready=False)
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
