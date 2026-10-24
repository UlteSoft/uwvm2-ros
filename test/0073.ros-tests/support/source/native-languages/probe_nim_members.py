#!/usr/bin/env python3
"""Inspect genuine Nim compiler ASTs for exported quoted record fields.

Reuse only the exact previously checked private fixture as a read-only input.
The active finite owner contains all new probe source, logs and output.
"""

from pathlib import Path
import hashlib
import json
import subprocess


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    root = Path.cwd()
    agents = Path('/home/macromodel/Documents/uwvm-test-261018/agents')
    parent = agents / '03/20261021-round03-nim-complete-docs-r3/work'
    receipt = parent / 'nim-docs.json'
    if digest(receipt) != '7f22fea44442d77bb87c89d7bf404347f4045e9ced43a00669fc07e3f582dd46':
        raise ValueError('Original retained parser receipt changed')
    previous = json.loads(receipt.read_bytes())
    package = parent / 'source/sdk/origin/nim/src'
    generated = parent / 'generated/nim'
    records = generated / 'records.nim'
    expected = next(row['source_sha256'] for row in previous['inventories'] if row['module'] == 'records')
    if digest(records) != expected:
        raise ValueError('Previously checked complete record source changed')
    nim = agents / '03/20261021-additional-tools-r2/work/toolchains/nim/nim-2.2.4/bin/nim'
    source = root / 'members.nim'
    source.write_text('import macros\nimport records\n'
                      'macro inspect(T: typedesc): untyped =\n'
                      '  echo "PUBLIC_TYPE_AST " & $T\n'
                      '  echo getImpl(T).treeRepr\n'
                      '  result = newEmptyNode()\n'
                      'inspect(Ros2Language_fixtureMsgSub)\n'
                      'inspect(Ros2Language_fixtureMsgAllValues)\n')
    command = [str(nim), 'check', '--cpu:wasm32', '--os:any', '--mm:arc',
               '--threads:off', '--exceptions:goto', '-d:wasm', '-d:useMalloc',
               '--path:' + str(package), '--path:' + str(generated), str(source)]
    log = root / 'members.log'
    with log.open('wb') as output:
        result = subprocess.run(command, stdout=output, stderr=subprocess.STDOUT, timeout=120)
    if digest(records) != expected:
        raise ValueError('Read-only record input changed during compiler inspection')
    report = {'kind': 'task03-genuine-Nim-public-member-AST-probe-v1',
              'status': 'PASS' if result.returncode == 0 else 'FAILED_RETAINED',
              'argv': command, 'exit': result.returncode, 'compiler_sha256': digest(nim),
              'source_sha256': digest(source), 'record_source_sha256': expected,
              'log_sha256': digest(log), 'record_input_unchanged': True,
              'scope': 'Actual wasm32 compiler AST inspection only; no SDK/Guest acceptance',
              'Guest_runs': 0, 'default_authority': []}
    (root / 'members.json').write_text(json.dumps(report, indent=2) + '\n')
    return result.returncode


if __name__ == '__main__':
    raise SystemExit(main())
