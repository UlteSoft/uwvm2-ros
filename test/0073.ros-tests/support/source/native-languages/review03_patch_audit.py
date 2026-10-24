#!/usr/bin/env python3
"""@brief Check captured shared patches and isolated numeric-generator entry points.

@details The complete capsule is immutable input. Only fresh private patch trees,
source products and receipts are written under the active finite Linux owner.
No shared live source, SDK publication, test catalog or other owner's file changes.
@see handoffs/03/review03/patches
"""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess
import sys
import tarfile


def digest(path):
    """@brief Hash a complete ordinary input/product file.

    @param[in] path Readable file; borrowed only during this synchronous read.
    @return Complete lowercase SHA-256; I/O errors propagate.
    @see main
    """
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    """@brief Validate exact private patch postimages and real generator CLI imports.

    @details Syntax/patch/generator passes earn no language compilation, Wasm
    validation, Guest or native credit. Products are retained compressed after
    read-back verification to fit the same finite owner and reserve policy.
    @return Zero after every captured-source and actual command check passes.
    @failure Source drift, invalid patches, wrong postimages or a failed entry
    point preserves evidence and aborts subsequent dependent checks.
    @see digest; numeric.main; generate_portable_numeric_bindings.main
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capsule', type=Path, required=True)
    parser.add_argument('--sha256', required=True)
    args = parser.parse_args()
    if digest(args.capsule) != args.sha256:
        raise ValueError('Captured patch/helper input changed')
    root = Path.cwd()
    with tarfile.open(args.capsule) as archive:
        source = {item.name: archive.extractfile(item).read() for item in archive if item.isfile()}
    if sum(map(len, source.values())) > 2 * 1024 * 1024:
        raise ValueError('Finite source extent exceeded')
    for name, body in source.items():
        if name.endswith('.py'):
            compile(body, name, 'exec', dont_inherit=True)
    rows = []
    report = {'kind': 'task03-round03-patch-generator-audit-v1', 'status': 'RUNNING',
              'capsule_sha256': args.sha256, 'rows': rows, 'source_inputs': {
                  name: hashlib.sha256(body).hexdigest() for name, body in source.items()},
              'language_compiler_runs': 0, 'Guest_runs': 0, 'default_authority': []}

    def save():
        (root / 'patch-audit.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(name, command, cwd=root):
        log = root / (name + '.log')
        row = {'name': name, 'argv': [str(item) for item in command], 'cwd': str(cwd), 'log': log.name}
        rows.append(row)
        save()
        with log.open('wb') as output:
            result = subprocess.run(row['argv'], cwd=cwd, stdout=output,
                                    stderr=subprocess.STDOUT, timeout=120)
        row.update(exit=result.returncode, status='PASS' if result.returncode == 0 else 'FAIL',
                   log_sha256=digest(log))
        save()
        if result.returncode:
            raise ValueError('Retained private check failure: ' + name)

    handoff = 'sdk/origin/coordination/20261021-director/handoffs/03/review03/'
    private = root / 'patch-source'
    for name, target, patch in (
            ('wasm_profiles.py', 'sdk/origin/tools/wasm_profiles.py', 'native-example-action.patch'),
            ('bounded_profile.py', 'sdk/origin/custom-types/bounded_profile.py', 'native-doc-helper-capture.patch'),
            ('generate_portable_numeric_bindings.py', 'sdk/origin/tools/generate_portable_numeric_bindings.py', 'portable-numeric-docs.patch')):
        path = private / target
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(source[handoff + 'before/' + name])
        patch_path = root / patch
        patch_path.write_bytes(source[handoff + 'patches/' + patch])
        run(name + '-check', ['/usr/bin/git', 'apply', '--check', patch_path], private)
        run(name + '-apply', ['/usr/bin/git', 'apply', patch_path], private)
        if path.read_bytes() != source[handoff + 'candidates/' + name]:
            raise ValueError('Private patch postimage differs: ' + name)
    # Every candidate/postimage remains byte-for-byte present in the input
    # capsule; remove only these new redundant scratch copies before generation.
    shutil.rmtree(private)
    tools = root / 'captured/tools'
    helper = tools / 'native_languages'
    helper.mkdir(parents=True)
    for name in ('numeric.py', 'documentation.py'):
        (helper / name).write_bytes(source['sdk/origin/tools/native_languages/' + name])
    portable = tools / 'generate_portable_numeric_bindings.py'
    portable.write_bytes(source[handoff + 'candidates/generate_portable_numeric_bindings.py'])
    schema = root / 'abi-v1.json'
    schema.write_bytes(source['sdk/origin/abi/abi-v1.json'])
    for name, script in [('portable', portable), ('native-five', helper / 'numeric.py')]:
        output = root / (name + '-products')
        run(name + '-isolated-cli', [sys.executable, '-I', '-S', '-B', script,
                                     '--schema', schema, '--out', output])
        facts = {str(path.relative_to(output)): {'bytes': path.stat().st_size, 'sha256': digest(path)}
                 for path in output.rglob('*') if path.is_file()}
        compressed = root / (name + '-products.tar.gz')
        with tarfile.open(compressed, 'w:gz') as archive:
            archive.add(output, arcname=output.name)
        with tarfile.open(compressed) as archive:
            for relative, pin in facts.items():
                with archive.extractfile(output.name + '/' + relative) as stream:
                    if hashlib.file_digest(stream, 'sha256').hexdigest() != pin['sha256']:
                        raise ValueError('New source-product archive read-back mismatch')
        report[name + '_products'] = facts
        report[name + '_archive_sha256'] = digest(compressed)
        save()
        shutil.rmtree(output)
    report['status'] = 'PASS_PRIVATE_PATCHES_SYNTAX_AND_REAL_ISOLATED_GENERATORS'
    save()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
