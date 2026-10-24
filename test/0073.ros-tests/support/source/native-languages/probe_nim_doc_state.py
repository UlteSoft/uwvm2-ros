#!/usr/bin/env python3
"""@brief Inspect retained Nim documentation candidates with real frontend tools.

@details Resume from exact failed-run source bytes without rerunning unrelated
generation. All new copies, formatter passes and jsondoc outputs remain below
this owner. This is source/doc qualification, not installed-SDK build credit.
@see review03_nim_docs.py
"""
from pathlib import Path
import difflib
import hashlib
import json
import shutil
import subprocess
import tarfile


def digest(path):
    """@brief Hash the complete bytes of a readable ordinary file.

    @param[in] path Input or product file, borrowed during the read.
    @return Complete lowercase SHA-256; I/O errors propagate.
    @see main
    """
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    """@brief Measure formatter convergence and actual declaration doc attachment.

    @details Linux/amd64 jsondoc inventories exact source declarations, while
    separate wasm32/os:any checks qualify the source target semantics. Neither
    inventory/tag coverage nor a parser result certifies semantic doc quality.
    @return Zero after semantics/frontend coverage and final formatter stability.
    @see digest; native_languages.documentation.attach_nim
    """
    root = Path.cwd()
    base = Path('/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-round03-nim-docs-r1')
    source = root / 'source'
    source.mkdir()
    originals = {}
    for relative in ('examples/progressive.nim', 'src/roswasm.nim', 'src/roswasm/raw.nim',
                     'src/roswasm/wire.nim', 'src/roswasm/typed.nim'):
        incoming = base / 'work/source/sdk/origin/nim' / relative
        target = source / 'nim' / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(incoming, target)
        originals[str(target.relative_to(root))] = digest(incoming)
    for name in ('records.nim', 'interfaces.nim'):
        incoming = base / 'work/generated/nim' / name
        target = source / 'generated' / name
        target.parent.mkdir(exist_ok=True)
        shutil.copy2(incoming, target)
        originals[str(target.relative_to(root))] = digest(incoming)
    rows = []
    report = {'kind': 'task03-Nim-doc-frontend-state-v1', 'status': 'RUNNING', 'rows': rows,
              'retained_input_pins': originals, 'inventories': [], 'installed_build_credit': 0,
              'Guest_runs': 0, 'documentation_acceptance': False, 'default_authority': []}

    def save():
        (root / 'nim-doc-state.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(name, layer, command):
        log = root / (name + '.log')
        row = {'name': name, 'layer': layer, 'argv': [str(item) for item in command], 'log': log.name}
        rows.append(row)
        with log.open('wb') as output:
            result = subprocess.run(row['argv'], cwd=root, stdout=output,
                                    stderr=subprocess.STDOUT, timeout=120)
        row.update(exit=result.returncode, status='PASS' if result.returncode == 0 else 'FAIL',
                   log_sha256=digest(log))
        save()
        if result.returncode:
            raise ValueError('Retained frontend failure: ' + name)

    tools = Path('/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-additional-tools-r2/work/toolchains/nim/nim-2.2.4/bin')
    files = sorted(source.rglob('*.nim'))
    prior = {path: path.read_text() for path in files}
    run('format-third', 'real_nimpretty', [tools / 'nimpretty', '--indent:2', '--maxLineLen:100', *files])
    third = {path: path.read_text() for path in files}
    changes = {str(path.relative_to(root)): '\n'.join(list(difflib.unified_diff(prior[path].splitlines(), third[path].splitlines()))[:60])
               for path in files if prior[path] != third[path]}
    report['third_pass_changes'] = changes
    run('format-fourth', 'real_nimpretty', [tools / 'nimpretty', '--indent:2', '--maxLineLen:100', *files])
    stable = all(path.read_text() == third[path] for path in files)
    report['final_formatter_stable'] = stable
    package = source / 'nim/src'
    generated = source / 'generated'
    for name, path in [('sdk', package / 'roswasm/typed.nim'), ('records', generated / 'records.nim'),
                       ('interfaces', generated / 'interfaces.nim')]:
        run(name + '-wasm', 'actual_Nim_wasm32_semantic_check', [tools / 'nim', 'check', '--mm:arc',
            '--threads:off', '--exceptions:goto', '--cpu:wasm32', '--os:any', '-d:wasm', '-d:useMalloc',
            '--path:' + str(package), '--path:' + str(generated), path])
    missing = []
    for name, path in [('owner', package / 'roswasm.nim'), ('wire', package / 'roswasm/wire.nim'),
                       ('typed', package / 'roswasm/typed.nim'), ('raw', package / 'roswasm/raw.nim'),
                       ('records', generated / 'records.nim'), ('interfaces', generated / 'interfaces.nim')]:
        output = root / (name + '.json')
        run(name + '-jsondoc', 'actual_host_target_Nim_frontend_inventory', [tools / 'nim', 'jsondoc',
            '--mm:arc', '--threads:off', '--exceptions:goto', '--cpu:amd64', '--os:linux',
            '--path:' + str(package), '--path:' + str(generated), '--out:' + str(output), path])
        parsed = json.loads(output.read_bytes())
        for entry in parsed['entries']:
            absent = [tag for tag in ('@brief', '@details', '@see') if tag not in entry.get('description', '')]
            if absent:
                missing.append({'module': name, 'name': entry['name'], 'line': entry.get('line'), 'missing': absent})
        report['inventories'].append({'module': name, 'source_sha256': digest(path),
            'output_sha256': digest(output), 'declarations': len(parsed['entries']),
            'description_count': sum(bool(entry.get('description')) for entry in parsed['entries']),
            'parser_target': 'linux/amd64; documentation only'})
        save()
    before = {}
    with tarfile.open(base / 'inputs/task03-review03-nim-docs-r1.tar') as archive:
        for item in archive:
            if item.isfile() and item.name.startswith('sdk/origin/nim/'):
                with archive.extractfile(item) as stream:
                    before[item.name] = hashlib.file_digest(stream, 'sha256').hexdigest()
    candidate_changes = []
    with tarfile.open(root / 'nim-doc-candidates.tar', 'w') as archive:
        for path in sorted((source / 'nim').rglob('*.nim')):
            relative = 'sdk/origin/nim/' + str(path.relative_to(source / 'nim'))
            if before[relative] != digest(path):
                archive.add(path, arcname=relative, recursive=False)
                candidate_changes.append({'path': relative, 'before_sha256': before[relative],
                                          'after_sha256': digest(path), 'bytes': path.stat().st_size})
    report.update(missing_required_doc_fields=missing, changes=candidate_changes,
                  candidate_archive_sha256=digest(root / 'nim-doc-candidates.tar'),
                  status='PASS_FRONTEND_FIELDS_AND_SEMANTICS' if stable and not missing else 'INCOMPLETE')
    save()
    return 0 if stable and not missing else 1


if __name__ == '__main__':
    raise SystemExit(main())
