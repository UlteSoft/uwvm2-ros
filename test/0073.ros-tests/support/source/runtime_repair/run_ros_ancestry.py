#!/usr/bin/env python3
"""@brief Exercise actual cross-module ROS provenance under native unwinding.

@details Direct, indirect, reference, tail and exception edges use real Wasm
modules. Every denied cross-module edge is followed by an admitted same-module
query, detecting leaked provenance after return or exception propagation.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('uwvm', 'wat2wasm', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=False)
    source = Path(__file__).resolve().parent.parent / 'ros_debugger'
    sys.path.insert(0, str(source))
    from ancestry_guest import fixtures
    report = {'passed': False, 'runtime_sha256': hashlib.sha256(args.uwvm.read_bytes()).hexdigest(), 'commands': []}

    def run(label, command, expected=0):
        command = list(map(str, command))
        child = subprocess.run(command, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=60)
        (args.out / (label + '.log')).write_bytes(child.stdout)
        report['commands'].append({'name': label, 'argv': command, 'returncode': child.returncode})
        assert child.returncode == expected, (label, child.returncode)

    try:
        features = ['-WFE-' + feature for feature in ('reference-types', 'function-references', 'tail-call', 'exceptions')]
        for version in (1, 2):
            folder = args.out / f'ros{version}'
            folder.mkdir()
            for name, text in fixtures(version).items():
                wat = folder / (name + '.wat')
                wat.write_text(text)
                # WABT's --enable-all also selects experimental compact import
                # encodings. These fixtures require typed references, not a
                # different import-section format from the product's contract.
                run(f'ros{version}-{name}-compile', [args.wat2wasm, '--enable-function-references',
                    '--debug-names', wat, '-o', folder / (name + '.wasm')])
            run(f'ros{version}-run', [args.uwvm, '-Raot', '-Rct', '0', '-Rllvm-call-stack', 'unwind',
                '-Rllvm-cache-path', 'disable', *features, '--wasm-preload-library', folder / 'callbacks.wasm', 'callbacks',
                '--wasm-preload-library', folder / 'library.wasm', 'library', '--run', folder / 'main.wasm'])
        report['passed'] = True
    finally:
        (args.out / 'result.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
