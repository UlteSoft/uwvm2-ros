#!/usr/bin/env python3
"""Apply the real pinned Go formatter to owned source, never build or run it."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'gofmt', 'go', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--go-source', type=Path, help='Also format the distinct standard-Go application source')
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same finite Linux owner/common lock required')
    if sha(args.gofmt) != 'acb1f5c293c3f0e2032676b72a288f7f18022653aedb465e3c1bd0ea78eab6e5':
        raise RuntimeError('exact actual Go1.24.2 gofmt required')
    args.out.mkdir()
    env = dict(os.environ, GOTOOLCHAIN='local', GOTELEMETRY='off', GOENV='off',
               GOWORK='off', GOPROXY='off', GOSUMDB='off')
    version = subprocess.run([args.go, 'version'], env=env, capture_output=True, timeout=30, check=True)
    if b'go1.24.2 ' not in version.stdout:
        raise RuntimeError('actual formatter toolchain version differs')
    (args.out / 'go-version.stdout').write_bytes(version.stdout)
    selected = [('tinygo', args.source)] + ([('go', args.go_source)] if args.go_source else [])
    results = []
    for language, source in selected:
        before = sha(source)
        first = subprocess.run([args.gofmt, source], env=env, capture_output=True, timeout=30, check=True)
        product = args.out / language / 'main.go' if args.go_source else args.out / 'main.go'
        product.parent.mkdir(exist_ok=True)
        product.write_bytes(first.stdout)
        second = subprocess.run([args.gofmt, product], env=env, capture_output=True, timeout=30, check=True)
        if first.stdout != second.stdout or sha(source) != before:
            raise RuntimeError('Go formatter not idempotent or immutable source changed')
        results.append(dict(language=language, input=dict(path=str(source), sha256=before),
                            output=dict(path=str(product), bytes=product.stat().st_size, sha256=sha(product))))
    report = dict(status='PASS_ACTUAL_GOFMT_SOURCE_ONLY', sources=results,
                  tools=[dict(path=str(path), sha256=sha(path)) for path in (args.go, args.gofmt)],
                  idempotent=True, sdk_builds=0, wasm_guest=0, native_ros=0,
                  go_api_parser_inventory='NOT_RUN', semantic_typecheck='NOT_RUN')
    (args.out / 'summary.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report), flush=True)


if __name__ == '__main__':
    main()
