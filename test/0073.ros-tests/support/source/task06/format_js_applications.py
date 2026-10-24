#!/usr/bin/env python3
"""Run real pinned clang-format JS grammar and genuine Acorn equivalence/API QA.

Finite Linux owner only. This scoped formatter result does not enroll a global
JavaScript formatter, build any SDK or run a Guest/native ROS application.
"""
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
    for name in ('source', 'formatter', 'inspector', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--expected-source-count', type=int)
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same admitted finite Linux owner required')
    if sha(args.formatter) != '91a554a9e6fe6aa7b46a2757ee4eb233a2489710c06d28078199bf9ad43ec344':
        raise RuntimeError('real pinned clang-format21 required')
    args.out.mkdir()
    tool_pins = [dict(path=str(path), sha256=sha(path)) for path in (
        Path(__file__), args.inspector, args.formatter, Path('/usr/bin/node'), Path('/usr/share/nodejs/acorn/dist/acorn.mjs'))]
    configuration = args.out / 'javascript-format.yaml'
    configuration.write_text('BasedOnStyle: Google\nLanguage: JavaScript\nColumnLimit: 100\n'
                             'SortIncludes: Never\nJavaScriptWrapImports: true\n')
    version = subprocess.run([args.formatter, '--version'], capture_output=True, check=True, timeout=30)
    if b'clang-format version 21.1.8' not in version.stdout:
        raise RuntimeError('actual formatter version differs')
    (args.out / 'formatter-version.stdout').write_bytes(version.stdout)
    rows = []
    sources = sorted(args.source.rglob('*.mjs'))
    if args.expected_source_count is not None and len(sources) != args.expected_source_count:
        raise RuntimeError('exact explicit authored/canonical formatter input count differs')
    for path in sources:
        relative = path.relative_to(args.source)
        if path.is_symlink():
            raise RuntimeError('ordinary authored source only')
        before = sha(path)
        target = args.out / 'formatted' / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        command = [args.formatter, '--style=file:' + str(configuration), '--assume-filename=' + str(path), path]
        first = subprocess.run(command, capture_output=True, timeout=30, check=False)
        if first.returncode:
            (args.out / 'formatter-failure.stderr').write_bytes(first.stderr)
            raise RuntimeError('actual formatter refused its explicit JS configuration/source')
        target.write_bytes(first.stdout)
        second = subprocess.run([args.formatter, '--style=file:' + str(configuration), '--assume-filename=' + str(target), target],
                                capture_output=True, timeout=30, check=True)
        if first.stdout != second.stdout or sha(path) != before:
            raise RuntimeError('formatter not idempotent or input changed')
        report = args.out / 'inventory' / relative.with_suffix('.json')
        report.parent.mkdir(parents=True, exist_ok=True)
        with report.with_suffix('.stdout').open('xb') as out, report.with_suffix('.stderr').open('xb') as err:
            result = subprocess.run(['/usr/bin/node', args.inspector, path, target, report], stdout=out, stderr=err,
                                    timeout=60, check=False)
        if result.returncode:
            raise RuntimeError('actual Acorn syntax/token/API attachment inspection failed: ' + str(relative))
        inventory = json.loads(report.read_bytes())
        rows.append(dict(source=relative.as_posix(), before_sha256=before, bytes=target.stat().st_size,
                         after_sha256=sha(target), inventory_sha256=sha(report), public_api_items=len(inventory['public_api_inventory'])))
        (args.out / 'sources.json').write_text(json.dumps(rows, indent=2) + '\n')
    summary = dict(status='PASS_SCOPED_ACTUAL_JS_FORMAT_AST_API_ATTACHMENTS', sources=len(rows),
                   public_api_items=sum(row['public_api_items'] for row in rows),
                   formatter=dict(path=str(args.formatter), sha256=sha(args.formatter)),
                   configuration_sha256=sha(configuration), idempotent=True, full_acorn_ast_and_tokens_equal=True,
                   global_formatter_enrollment=False, sdk_builds=0, wasm_guest=0, native_ros=0)
    if any(sha(Path(row['path'])) != row['sha256'] for row in tool_pins):
        raise RuntimeError('actual formatter/parser/inspector changed')
    (args.out / 'tool-pins.json').write_text(json.dumps(tool_pins, indent=2) + '\n')
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
