#!/usr/bin/env python3
"""Check bounded CPython producer refusals without compiling an interpreter."""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def main():
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('the original admitted Linux owner is required')
    producer, output = map(Path, sys.argv[1:])
    original = producer.read_bytes()
    output.mkdir(exist_ok=False)
    changed = output / 'changed-archive'
    changed.write_bytes(b'changed pinned input\n')
    arguments = ['--cpython-archive', str(changed), '--wasi-archive', str(changed),
                 '--wasmtime', str(changed), '--validator', str(changed)]
    report = dict(status='STARTED', producer=str(producer),
                  producer_sha256=hashlib.sha256(original).hexdigest(), cases=[],
                  compiler_product_credit=0, guest_credit=0, default_authority=[])
    selected = [
        ('unadmitted-work-directory', Path(os.environ['UWVM_RESOURCE_RUN']) / 'home/producer-refusal',
         {}, 'fresh producer output inside the admitted work directory required'),
        ('missing-owner-token', output / 'without-owner', {'UWVM_RESOURCE_HEAVY_LOCK_HELD': '0'},
         'the original admitted Linux resource owner is required'),
        ('changed-released-source', output / 'changed-source', {},
         'exact released cpython source archive required'),
    ]
    for label, target, extra, reason in selected:
        if target.exists():
            raise ValueError('fresh confined negative-control destination required')
        result = subprocess.run([sys.executable, '-I', '-S', '-B', str(producer),
                                 *arguments, '--out', str(target)],
                                stdin=subprocess.DEVNULL, capture_output=True,
                                env=dict(os.environ, **extra), timeout=10, check=False)
        passed = (result.returncode != 0 and reason.encode() in result.stderr and
                  not result.stdout and not target.exists())
        (output / (label + '.stdout')).write_bytes(result.stdout)
        (output / (label + '.stderr')).write_bytes(result.stderr)
        report['cases'].append(dict(label=label, exit=result.returncode, passed=passed,
                                   producer_output_created=target.exists(), expected_reason=reason))
        if not passed:
            raise RuntimeError('actual producer refusal failed: ' + label)
    if producer.read_bytes() != original:
        raise ValueError('selected producer source changed')
    report.update(status='THREE_ACTUAL_PRODUCER_REFUSALS_NO_COMPILER_OR_GUEST', passed=3)
    (output / 'results.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
