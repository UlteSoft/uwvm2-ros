#!/usr/bin/env python3
"""@brief Qualify Nim documentation attachment with the real compiler frontend.

@details Source comes from one captured capsule. Nimpretty, wasm-target semantic
checks and host-target jsondoc are distinct rows. The latter avoids Nim2.2.4's
nimdoc/os:any standard-library branch error and supplies parser-bound API docs;
it earns no host-as-Wasm or target-library credit. No public SDK is regenerated.
@see native_languages.documentation.attach_nim
"""
from pathlib import Path
import argparse
import hashlib
import html
import json
import os
import re
import subprocess
import sys
import tarfile


def digest(path):
    """@brief Hash one complete ordinary captured input or source product.

    @param[in] path Readable file; borrowed until the synchronous read completes.
    @return Complete lowercase SHA-256; I/O errors propagate.
    @see main
    """
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def audit_frontend_entry(entry, include_description=False):
    """Compare actual frontend names/types with their attached doc contracts.

    Nim jsondoc renders the public Decoder procedure type as code rather than a
    routine signature. Its exact reviewed callback parameter is still an actual
    public parameter. Quoted generated fields need the separate compiler AST
    inventory because this frontend omits their names from rendered type code.
    """
    description = entry.get('description', '')
    absent = [tag for tag in ('@brief', '@details', '@see') if tag not in description]
    text = html.unescape(re.sub('<[^>]+>', '', description))
    arguments = entry.get('signature', {}).get('arguments', [])
    signature = entry.get('signature')
    if entry.get('type') == 'skType' and entry.get('name') == 'Decoder':
        if entry.get('code') != 'Decoder[T] = proc (bytes: seq[byte]): T {.closure.}':
            raise ValueError('Unreviewed compiler-rendered public Decoder callback signature')
        arguments = [{'name': 'bytes', 'type': 'seq[byte]'}]
        signature = {'arguments': arguments, 'return': 'T',
                     'source': 'exact compiler-rendered public callback type'}
    documented_arguments = re.findall(r'@param\[(?:in|out|in,out)\]\s+([A-Za-z_][A-Za-z0-9_]*)', text)
    missing_parameters = [argument['name'] for argument in arguments
                          if documented_arguments.count(argument['name']) != 1]
    unexpected_parameters = [name for name in documented_arguments
                             if name not in {argument['name'] for argument in arguments}]
    if missing_parameters:
        absent.append('exact_parameter_contracts:' + ','.join(missing_parameters))
    if unexpected_parameters:
        absent.append('unknown_parameter_contracts:' + ','.join(unexpected_parameters))
    if (signature or {}).get('return') and '@return' not in text:
        absent.append('@return')
    if re.search(r'href="#(?:in|out)', description):
        absent.append('direction_rendered_as_reference')
    members = []
    if entry.get('type') == 'skType':
        for code_line in entry.get('code', '').splitlines()[1:]:
            match = re.match(r'\s*(`[^`]+`|[A-Za-z_][A-Za-z0-9_]*)\*?\s*[:=]', code_line)
            if not match or ('*:' not in code_line and not match[1].startswith('rpc_')):
                continue
            attached = code_line.split('##', 1)[1].strip() if '##' in code_line else ''
            missing_member = [tag for tag in ('@brief', '@details', '@see') if tag not in attached]
            members.append({'name': match[1], 'frontend_code': code_line,
                            'attached_doc': attached, 'missing': missing_member})
            if missing_member:
                absent.append('member:' + match[1] + ':' + ','.join(missing_member))
    row = {key: entry[key] for key in ('name', 'type', 'line', 'col', 'code') if key in entry}
    row.update(description_sha256=hashlib.sha256(description.encode()).hexdigest(), missing=absent,
               signature=signature, documented_parameters=documented_arguments, members=members)
    if include_description:
        row['description'] = description
    return row, absent


def main():
    """@brief Produce attached documentation, format it and bind it to declarations.

    @return Zero after genuine formatter idempotence, source semantics and all
    required frontend documentation fields pass. Semantic review is separate.
    @failure Preserve failures and never convert tag coverage into API acceptance.
    @see digest; numeric.raw_source; records.document_records
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--sha256', required=True)
    parser.add_argument('--core-only', action='store_true',
                        help='Explicit four-module scope; generated record/facade acceptance remains pending')
    args = parser.parse_args()
    root = Path.cwd()
    if digest(args.source) != args.sha256:
        raise ValueError('Current source capsule changed')
    source = root / 'source'
    source.mkdir()
    with tarfile.open(args.source) as archive:
        members = archive.getmembers()
        if sum(item.size for item in members) > 2 * 1024 * 1024 or any(
                Path(item.name).is_absolute() or '..' in Path(item.name).parts
                or not (item.isfile() or item.isdir()) for item in members):
            raise ValueError('Bounded ordinary source capsule required')
        archive.extractall(source, filter='data')
    origin = source / 'sdk/origin'
    helper = origin / 'tools/native_languages'
    sys.path[:0] = [str(helper), str(origin / 'tools')]
    for path in sorted(helper.glob('*.py')):
        compile(path.read_bytes(), str(path), 'exec', dont_inherit=True)
    import documentation
    import numeric
    before = {str(path.relative_to(source)): digest(path)
              for path in (origin / 'nim').rglob('*') if path.is_file()}
    for relative, body in numeric.products((origin / 'abi/abi-v1.json').read_bytes()).items():
        if relative.startswith('nim/src/' if args.core_only else 'nim/'):
            path = origin / relative
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(body)
    for path in (origin / 'nim').rglob('*.nim'):
        attached = documentation.attach_nim(path.read_text())
        if documentation.attach_nim(attached) != attached:
            raise ValueError('Documentation attachment must be byte-idempotent')
        path.write_text(attached)
    rows = []
    report = {'kind': 'task03-parser-bound-Nim-docs-v1', 'status': 'RUNNING',
              'source_sha256': args.sha256, 'rows': rows, 'inventories': [],
              'documentation_acceptance': False, 'content_review_required': True,
              'scope': 'four_core_modules' if args.core_only else 'core_and_generated_fixture',
              'default_authority': [], 'Guest_runs': 0, 'installed_build_credit': 0}

    def save():
        (root / 'nim-docs.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(name, layer, command):
        log = root / (name + '.log')
        row = {'name': name, 'layer': layer, 'argv': [str(item) for item in command], 'log': log.name}
        rows.append(row)
        save()
        with log.open('wb') as output:
            result = subprocess.run(row['argv'], cwd=root, stdout=output,
                                    stderr=subprocess.STDOUT, timeout=120)
        row.update(exit=result.returncode, log_sha256=digest(log),
                   status='PASS' if result.returncode == 0 else 'FAIL')
        if result.returncode:
            report['status'] = 'FAILED_RETAINED'
        save()
        print(name, row['status'], flush=True)
        if result.returncode:
            raise ValueError('Retained documentation/semantic failure: ' + name)

    generated = root / 'generated'
    if args.core_only:
        generated.mkdir()
    else:
        fixture = source / 'test/0073.ros-tests/support/source/sdk-native-languages/fixtures/catalog.json'
        run('full-field-emission', 'real_complete_record_generator', [sys.executable, '-I', '-S', '-B',
            helper / 'emit.py', '--origin', origin, '--catalog', fixture, '--guest-only',
            '--language', 'nim', '--out', generated])
    tools = Path('/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-additional-tools-r2/work/toolchains/nim/nim-2.2.4/bin')
    nim = tools / 'nim'
    nimpretty = tools / 'nimpretty'
    files = sorted((origin / 'nim').rglob('*.nim')) + sorted((generated / 'nim').glob('*.nim'))
    command = [nimpretty, '--indent:2', '--maxLineLen:100', *files]
    run('format-first', 'real_nimpretty', command)
    first = {str(path): digest(path) for path in files}
    run('format-second', 'real_nimpretty', command)
    if any(digest(Path(name)) != pin for name, pin in first.items()):
        raise ValueError('Formatter is not byte-idempotent')
    report['formatter'] = {'sha256': digest(nimpretty), 'inputs': first, 'idempotent': True}
    package = origin / 'nim/src'
    semantic_targets = [('sdk', package / 'roswasm/typed.nim')]
    if not args.core_only:
        semantic_targets += [('records', generated / 'nim/records.nim'), ('interfaces', generated / 'nim/interfaces.nim')]
    for name, path in semantic_targets:
        run(name + '-wasm-semantics', 'actual_Nim_wasm32_semantic_check', [nim, 'check', '--mm:arc',
            '--threads:off', '--exceptions:goto', '--cpu:wasm32', '--os:any', '-d:wasm', '-d:useMalloc',
            '--path:' + str(package), '--path:' + str(generated / 'nim'), path])
    docs = root / 'frontend-docs'
    docs.mkdir()
    missing = []
    inventory_targets = [('owner', package / 'roswasm.nim'), ('wire', package / 'roswasm/wire.nim'),
                         ('typed', package / 'roswasm/typed.nim'), ('raw', package / 'roswasm/raw.nim')]
    if not args.core_only:
        inventory_targets += [('records', generated / 'nim/records.nim'), ('interfaces', generated / 'nim/interfaces.nim')]
    for name, path in inventory_targets:
        output = docs / (name + '.json')
        run(name + '-frontend', 'actual_Nim_linux_amd64_jsondoc', [nim, 'jsondoc', '--mm:arc',
            '--threads:off', '--exceptions:goto', '--cpu:amd64', '--os:linux',
            '--path:' + str(package), '--path:' + str(generated / 'nim'), '--out:' + str(output), path])
        inventory = json.loads(output.read_bytes())
        entries = []
        for entry in inventory['entries']:
            row, absent = audit_frontend_entry(entry, include_description=not args.core_only)
            entries.append(row)
            if absent:
                missing.append({'module': name, 'name': entry['name'], 'line': entry.get('line'), 'missing': absent})
        report['inventories'].append({'module': name, 'source_sha256': digest(path),
            'frontend_output_sha256': digest(output), 'entries': entries,
            'parser_target': 'linux/amd64; declaration/doc inventory only'})
        save()
    if not args.core_only:
        member_report = root / 'member-inventory.json'
        run('records-member-AST', 'actual_Nim_exported_field_AST_and_source_attachment',
            [sys.executable, '-I', '-S', '-B', Path(__file__).parent / 'nim_record_member_inventory.py',
             '--generated', generated, '--package', package, '--nim', nim,
             '--work', root / 'member-AST', '--report', member_report])
        member_inventory = json.loads(member_report.read_bytes())
        report['generated_member_inventory'] = {
            'status': member_inventory['status'], 'path': str(member_report),
            'sha256': digest(member_report), 'record_count': member_inventory['record_count'],
            'field_count': member_inventory['field_count'],
            'method': member_inventory['method'],
            'jsondoc_quoted_field_rendering_limitation': True,
        }
    changes = []
    with tarfile.open(root / 'nim-doc-candidates.tar', 'w') as archive:
        for path in sorted((origin / 'nim').rglob('*')):
            if not path.is_file():
                continue
            relative = str(path.relative_to(source))
            if before.get(relative) != digest(path):
                changes.append({'path': relative, 'before_sha256': before.get(relative),
                                'after_sha256': digest(path), 'bytes': path.stat().st_size})
                archive.add(path, arcname=relative, recursive=False)
    report.update(changes=changes, candidate_archive_sha256=digest(root / 'nim-doc-candidates.tar'),
                  missing_required_doc_fields=missing,
                  status='PASS_PARSER_BOUND_DOC_FIELDS_AND_WASM_SEMANTICS' if not missing else 'INCOMPLETE_DOC_FIELDS')
    save()
    return 0 if not missing else 1


if __name__ == '__main__':
    raise SystemExit(main())
