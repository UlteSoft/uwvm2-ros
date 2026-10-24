#!/usr/bin/env python3
"""Qualify complete xArm callback models and parser-bound API attachments.

Run only inside the admitted finite Linux owner/common lock. The real original
source is pinned, not launched. Python's independent fixed-six binary64 oracle
is data-model evidence, never native C++/ROS or Wasm equivalence evidence.
"""
import argparse
import ast
import hashlib
import json
import math
import os
from pathlib import Path
import struct
import subprocess


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'records', 'core', 'model', 'original', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same finite Linux owner required')
    args.out.mkdir()
    root = args.out / 'source-model'
    files = [(args.source / relative, root / 'sdk/origin/platform/ufactory/javascript' / relative)
             for relative in ('include/robot_states.mjs', 'example/official/test_robot_states.mjs')]
    files += [(args.records, root / 'sdk/origin/custom-types/ufactory-xarm-ros2-humble-gz-complete/bounded_records.mjs'),
              (args.core, root / 'sdk/origin/platform/ufactory/javascript/include/core/humble/bounded_records.mjs')]
    pins = []
    for original, target in files:
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(original.read_bytes())
        pins.append(dict(path=str(original), bytes=original.stat().st_size, sha256=sha(original)))
    pins += [dict(path=str(path), bytes=path.stat().st_size, sha256=sha(path))
             for path in (Path(__file__), args.model, args.original, Path('/usr/bin/node'), Path('/usr/share/nodejs/acorn/dist/acorn.mjs'))]
    original = args.original.read_text()
    for required in ('std::to_string(states->position[i])', '"positon: %s"', '"state: %d, error: %d"',
                     '"xarm_cgpio_states"', '"joint_states", 100', '"robot_states", 100', '"xarm_cgpio_states", 100'):
        if required not in original:
            raise RuntimeError('pinned original workflow no longer matches review')
    values = [0.0, -0.0, 0.5, -0.125, 2.345, 1e21, -1e21, 1e308, -1e308,
              math.ulp(0.0), -math.ulp(0.0), math.nextafter(0.0000005, 0.0),
              0.0000005, math.nextafter(0.0000005, math.inf), 0.0000015, 0.0000025]
    # Deterministic bit-pattern coverage across subnormal, normal, ties and large
    # finite exponents. These are independent full binary64 input values.
    state = 0x123456789abcdef0
    for _ in range(512):
        state = (state * 6364136223846793005 + 1442695040888963407) & ((1 << 64) - 1)
        value = struct.unpack('<d', state.to_bytes(8, 'little'))[0]
        if math.isfinite(value):
            values.append(value)
    numeric = [dict(bits=struct.pack('>d', value).hex(), fixed=format(value, '.6f')) for value in values]
    numeric_path = args.out / 'numeric-oracle.json'
    numeric_path.write_text(json.dumps(numeric, indent=2) + '\n')
    ast.parse(Path(__file__).read_bytes())
    (args.out / 'source-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
    commands = [('/usr/bin/node', '--version'),
                ('/usr/bin/node', '--check', str(files[1][1])),
                ('/usr/bin/node', str(args.model), '--root', str(root), '--numeric', str(numeric_path), '--out', str(args.out / 'model.json'))]
    for index, command in enumerate(commands):
        with (args.out / (str(index) + '.stdout')).open('xb') as out, (args.out / (str(index) + '.stderr')).open('xb') as err:
            result = subprocess.run(command, stdout=out, stderr=err, stdin=subprocess.DEVNULL, timeout=90, check=False)
        if result.returncode:
            raise RuntimeError('actual Node syntax/model/parser failure: ' + str(index))
    if any(sha(Path(row['path'])) != row['sha256'] for row in pins):
        raise RuntimeError('immutable source or tool changed')
    model = json.loads((args.out / 'model.json').read_bytes())
    summary = dict(status=model['status'], host_model_cases=len(model['cases']),
                   numeric_oracle_values=len(numeric), public_api_items=len(model['public_api_inventory']),
                   sdk_builds=0, wasm_guest=0, native_ros=0, original_processes=0, physical_motion=False,
                   publication_ready=False, scope='Actual whole-message source callback model and parser attachments only')
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
