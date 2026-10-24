#!/usr/bin/env python3
"""Generate only deletion-projection WAT and profiles in a fresh directory."""
import argparse
import json
from pathlib import Path
import subprocess
import sys
sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'ros'))
import generate_typed_parameter_event_fixtures as template
sys.path.insert(0, str(Path(__file__).resolve().parent))
import events


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--compile', action='store_true')
    parser.add_argument('--wat2wasm', default='wat2wasm')
    a = parser.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    template.events = events
    profiles = []
    for case in events.cases():
        source, grants, assertions = template.guest(case)
        assertions.pop('native_kind_checked_before_typed_delete')
        assertions.update(native_not_set_deletion_only=True, prior_type_and_value_unknown=True,
                          prior_metadata_attested=False, official_sdk_delete_required=True)
        path = a.output / (case['name'] + '.wat')
        path.write_text(source)
        if a.compile:
            subprocess.run([a.wat2wasm, '--enable-all', str(path), '-o', str(path.with_suffix('.wasm'))], check=True, timeout=30)
        profiles.append({**case, 'grants': grants, 'guest_assertions': assertions})
    (a.output / 'profiles.json').write_text(json.dumps(profiles, indent=2) + '\n')
    print(f'WROTE deletion-only WAT/profiles: {len(profiles)}; compiled={a.compile}', flush=True)


if __name__ == '__main__':
    main()
