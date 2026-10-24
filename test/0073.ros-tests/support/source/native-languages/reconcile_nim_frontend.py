#!/usr/bin/env python3
"""Reconcile exact retained Nim frontend/field receipts without recompiling.

The prior full job genuinely formatted and checked source, but its documentation
checker misclassified the public Decoder procedure-type parameter. This reader
uses the repaired checker against the same pinned outputs and separately binds
the genuine compiler-located field inventory. It changes no old receipt/source.
Run only through the unchanged shared Linux finite owner after it is available.
"""

from pathlib import Path
import argparse
import hashlib
import json
import types


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--parent', type=Path, required=True)
    parser.add_argument('--parent-sha256', required=True)
    parser.add_argument('--members', type=Path, required=True)
    parser.add_argument('--members-sha256', required=True)
    args = parser.parse_args()
    parent = args.parent.resolve(strict=True)
    members = args.members.resolve(strict=True)
    if digest(parent) != args.parent_sha256 or digest(members) != args.members_sha256:
        raise ValueError('Original exact parser/member receipt changed')
    previous = json.loads(parent.read_bytes())
    field_inventory = json.loads(members.read_bytes())
    expected_missing = [{'module': 'typed', 'name': 'Decoder', 'line': 7,
                         'missing': ['unknown_parameter_contracts:bytes']}]
    if previous.get('missing_required_doc_fields') != expected_missing:
        raise ValueError('A different prior failure needs separate source review')
    if not previous.get('formatter', {}).get('idempotent'):
        raise ValueError('Prior genuine formatter idempotence receipt required')
    required = {'sdk-wasm-semantics', 'records-wasm-semantics', 'interfaces-wasm-semantics',
                'format-first', 'format-second', 'owner-frontend', 'wire-frontend',
                'typed-frontend', 'raw-frontend', 'records-frontend', 'interfaces-frontend'}
    rows = {row['name']: row for row in previous['rows']}
    if not required <= set(rows) or any(rows[name]['status'] != 'PASS' for name in required):
        raise ValueError('Required original genuine tool row did not pass')
    if field_inventory.get('status') != 'PASS_COMPILER_LOCATED_PUBLIC_FIELD_CONTRACTS':
        raise ValueError('Independent actual compiler field inventory required')
    if not field_inventory.get('inputs_unchanged') or field_inventory.get('field_count') != 28:
        raise ValueError('Unexpected original nine-record complete-field scope')
    helper = Path(__file__).parent / 'review03_nim_docs.py'
    helper_bytes = helper.read_bytes()
    module = types.ModuleType('_task03_exact_nim_frontend_checker')
    module.__file__ = str(helper)
    exec(compile(helper_bytes, str(helper), 'exec', dont_inherit=True), module.__dict__)
    root = parent.parent
    package = root / 'source/sdk/origin/nim/src'
    sources = {'owner': package / 'roswasm.nim', 'wire': package / 'roswasm/wire.nim',
               'typed': package / 'roswasm/typed.nim', 'raw': package / 'roswasm/raw.nim',
               'records': root / 'generated/nim/records.nim',
               'interfaces': root / 'generated/nim/interfaces.nim'}
    inventories = []
    missing = []
    captured = {str(parent): args.parent_sha256, str(members): args.members_sha256}
    for original in previous['inventories']:
        name = original['module']
        source = sources[name]
        frontend = root / 'frontend-docs' / (name + '.json')
        if digest(source) != original['source_sha256'] or digest(frontend) != original['frontend_output_sha256']:
            raise ValueError('Exact prior source/frontend output drift: ' + name)
        captured[str(source)] = original['source_sha256']
        captured[str(frontend)] = original['frontend_output_sha256']
        data = json.loads(frontend.read_bytes())
        if [row['name'] for row in data['entries']] != [row['name'] for row in original['entries']]:
            raise ValueError('Prior parser declaration inventory changed')
        entries = []
        for entry in data['entries']:
            row, absent = module.audit_frontend_entry(entry)
            entries.append(row)
            if absent:
                missing.append({'module': name, 'name': entry['name'], 'missing': absent})
        inventories.append({'module': name, 'source_sha256': original['source_sha256'],
                            'frontend_sha256': original['frontend_output_sha256'],
                            'entries': entries})
    if field_inventory['source_pins'].get(str(sources['records'])) != digest(sources['records']):
        raise ValueError('Quoted-field AST belongs to another generated source')
    for spelling, expected in field_inventory['source_pins'].items():
        path = Path(spelling)
        if not path.resolve(strict=True).is_relative_to(root) or digest(path) != expected:
            raise ValueError('Actual compiler-located member source changed')
        captured[spelling] = expected
    if helper.read_bytes() != helper_bytes or any(digest(Path(path)) != expected for path, expected in captured.items()):
        raise ValueError('Read-only reconciliation input changed')
    report = {'kind': 'task03-exact-prior-Nim-frontend-reconciliation-v1',
              'status': 'PASS_EXACT_PRIOR_FRONTEND_PARAMETER_RECONCILIATION' if not missing else 'INCOMPLETE',
              'prior_owner_status_preserved': previous['status'],
              'prior_source_semantic_checks': 3, 'prior_frontend_outputs': 6,
              'current_job_language_compiler_runs': 0, 'current_job_formatter_runs': 0,
              'source_pins': captured, 'checker_sha256': hashlib.sha256(helper_bytes).hexdigest(),
              'inventories': inventories, 'missing': missing,
              'compiler_located_generated_records': field_inventory['record_count'],
              'compiler_located_generated_fields': field_inventory['field_count'],
              'documentation_semantic_content_review_required': True,
              'new_codec_return_contract_regeneration_required': True,
              'all_public_API_documentation_accepted': False,
              'current_installed_SDK_rebuild_accepted': False,
              'Guest_runs': 0, 'native_ROS_runs': 0, 'default_authority': []}
    (Path.cwd() / 'frontend-reconciliation.json').write_text(json.dumps(report, indent=2) + '\n')
    return 0 if not missing else 1


if __name__ == '__main__':
    raise SystemExit(main())
