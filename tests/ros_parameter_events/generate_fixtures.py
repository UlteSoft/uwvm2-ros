#!/usr/bin/env python3
"""Generate only new expanded event fixtures using the frozen guest assertion template."""
import argparse
import json
import sys
from pathlib import Path
import subprocess

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'ros'))
import generate_typed_parameter_event_fixtures as template
sys.path.insert(0, str(Path(__file__).resolve().parent))
import events


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--compile', action='store_true')
    parser.add_argument('--wat2wasm', default='/usr/bin/wat2wasm')
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    template.events = events
    records = []
    for description in events.cases():
        source, grants, assertions = template.guest(description)
        path = args.output / (description['name'] + '.wat')
        path.write_text(source)
        records.append({**description, 'grants': grants, 'guest_assertions': assertions})
        if args.compile:
            subprocess.run([args.wat2wasm, '--enable-all', str(path), '-o', str(path.with_suffix('.wasm'))], check=True, timeout=30)
    (args.output / 'profiles.json').write_text(json.dumps(records, indent=2) + '\n')
    print(f'WROTE expanded ParameterEvent WAT/profiles: {len(records)}; compiled={args.compile}', flush=True)


if __name__ == '__main__':
    main()
