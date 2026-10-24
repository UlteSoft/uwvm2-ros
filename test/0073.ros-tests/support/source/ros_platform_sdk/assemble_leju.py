#!/usr/bin/env python3
"""Assemble exact selected Leju source facades, entries and authored contracts.

Only caller-owned source files are written. This phase runs source authors and
checks source correspondence; it executes no compiler, Guest or native driver.
The separately retained execution phases must build every delivered entry.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    kit = source / 'sdk/origin/robots/humanoids/leju'
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', commands=[], source_outputs=[], default_authority=[],
                  compiler_guest_or_native_runs=0, original_official_cases_run=0)
    try:
        for script, name, target in [('emit_examples.py', 'examples', kit),
                                     ('emit_contracts.py', 'contracts', kit / 'tests')]:
            directory = out / name
            argv = [sys.executable, '-I', '-S', '-B', kit / 'tools' / script,
                    '--source', source, '--out', directory]
            result = subprocess.run(list(map(str, argv)), cwd=source, check=False,
                                    timeout=600, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            (out / (name + '.stdout')).write_bytes(result.stdout)
            (out / (name + '.stderr')).write_bytes(result.stderr)
            report['commands'].append(dict(argv=list(map(str, argv)), exit=result.returncode))
            assert result.returncode == 0, result.stderr.decode()[-3000:]
            for path in sorted(directory.rglob('*')):
                if path.is_file():
                    destination = target / path.relative_to(directory)
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    with destination.open('xb') as stream:
                        stream.write(path.read_bytes())
                    body = destination.read_bytes()
                    report['source_outputs'].append(dict(path=str(destination.relative_to(source)),
                        bytes=len(body), sha256=hashlib.sha256(body).hexdigest()))
        for profile in json.loads((kit / 'profiles.json').read_bytes())['profiles']:
            family = source / 'sdk/origin/custom-types' / ('leju-' + profile['id'].replace('_', '-') + '-complete')
            argv = [sys.executable, '-I', '-S', '-B', kit / 'tools/select_profile.py',
                    '--kit', kit, '--bundle', family, '--profile', profile['id']]
            result = subprocess.run(list(map(str, argv)), cwd=source, check=False, timeout=60,
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            (out / (profile['id'] + '.stderr')).write_bytes(result.stderr)
            report['commands'].append(dict(argv=list(map(str, argv)), exit=result.returncode))
            assert result.returncode == 0, result.stderr.decode()[-3000:]
        report['status'] = 'LEJU_COMPLETE_FACADES_AND_SIX_LANGUAGE_ENTRIES_AUTHORED_NOT_EXECUTED'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (out / 'assembly.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
