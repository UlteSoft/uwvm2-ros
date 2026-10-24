#!/usr/bin/env python3
"""Run actual Node complete-value keyboard models and attached API inventory.

Only within the finite Linux owner; zero SDK/Guest/native/device qualification.
Source copies retain genuine complete codecs. No ROS/Guest module is stubbed
or executed by Node; only the pure local business module is imported.
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
    for name in ('source', 'records', 'model', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same finite Linux owner required')
    args.out.mkdir()
    root = args.out / 'source-model'
    files = []
    for relative in ('include/teleop.mjs', 'example/official/teleop_keyboard.mjs'):
        path = args.source / relative
        files.append((path, root / 'sdk/origin/platform/elephant/javascript' / relative))
    files.append((args.records, root / 'sdk/origin/custom-types/elephant-mycobot-noetic-complete/bounded_records.mjs'))
    pins = []
    for original, target in files:
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(original.read_bytes())
        pins.append(dict(path=str(original), bytes=original.stat().st_size, sha256=sha(original)))
    pins += [dict(path=str(path), bytes=path.stat().st_size, sha256=sha(path))
             for path in (Path(__file__), args.model, Path('/usr/bin/node'), Path('/usr/share/nodejs/acorn/dist/acorn.mjs'))]
    ast.parse(Path(__file__).read_bytes())
    (args.out / 'source-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
    commands = [('/usr/bin/node', '--version'),
                ('/usr/bin/node', '--check', str(files[1][1])),
                ('/usr/bin/node', str(args.model), '--root', str(root), '--out', str(args.out / 'model.json'))]
    for index, command in enumerate(commands):
        with (args.out / (str(index) + '.stdout')).open('xb') as out, (args.out / (str(index) + '.stderr')).open('xb') as err:
            result = subprocess.run(command, stdout=out, stderr=err, stdin=subprocess.DEVNULL, timeout=90, check=False)
        if result.returncode:
            raise RuntimeError('actual Node syntax/model/parser failure: ' + str(index))
    for row in pins:
        if sha(Path(row['path'])) != row['sha256']:
            raise RuntimeError('immutable source or tool changed')
    model = json.loads((args.out / 'model.json').read_bytes())
    summary = dict(status=model['status'], host_model_cases=len(model['cases']), public_api_items=len(model['public_api_inventory']),
                   actual_sdk_builds=0, wasm_guest=0, native_ros=0, original_processes=0, physical_motion=False,
                   publication_ready=False, scope='Actual complete-source host data model and parser attachment only')
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
