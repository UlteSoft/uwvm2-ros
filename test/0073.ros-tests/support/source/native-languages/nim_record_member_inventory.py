#!/usr/bin/env python3
"""Locate every generated Nim field through the genuine compiler's public AST.

Nim2.2.4 jsondoc omits quoted field declarations from its rendered code. Keep
the quoted public API unchanged: compiler AST export flags and source positions
bind each exact field to its native trailing documentation on pinned source.
This is a source/documentation inventory, not SDK, Guest or native acceptance.
"""

from pathlib import Path
import argparse
import hashlib
import json
import re
import subprocess


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


MACRO = '''import macros
import std/strutils
import records

macro inspectRecord(publicName: static[string], T: typedesc): untyped =
  let definition = getImpl(T)
  if definition.kind != nnkTypeDef or definition[0].kind != nnkPostfix:
    error("Expected actual exported record type", T)
  if definition[0][0].repr != "*" or definition[2].kind != nnkObjectTy:
    error("Expected exact public object declaration", T)
  echo "PUBLIC_RECORD\\t" & publicName
  let members = definition[2][2]
  for field in members:
    if field.kind == nnkEmpty:
      continue
    if field.kind != nnkIdentDefs:
      error("Expected complete ordinary record field", field)
    for index in 0 ..< field.len - 2:
      let declaration = field[index]
      if declaration.kind != nnkPostfix or declaration[0].repr != "*":
        error("Generated record member is not actually exported", declaration)
      let identifier = declaration[1]
      let location = identifier.lineInfoObj
      let name = identifier.repr.replace("`", "")
      echo "PUBLIC_FIELD\\t" & publicName & "\\t" & name & "\\t" &
        $location.line & "\\t" & $location.column & "\\t" &
        field[field.len - 2].repr & "\\t" & location.filename
  result = newEmptyNode()
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--generated', type=Path, required=True)
    parser.add_argument('--package', type=Path, required=True)
    parser.add_argument('--nim', type=Path, required=True)
    parser.add_argument('--work', type=Path, required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    generated = args.generated.resolve(strict=True)
    package = args.package.resolve(strict=True)
    source = generated / 'nim/records.nim'
    schema_path = generated / 'native-records.json'
    schema = json.loads(schema_path.read_bytes())
    records = schema['records']
    for record in records:
        if not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', record['symbol']):
            raise ValueError('Exact trusted generated type identifier required')
    pins = {str(path): digest(path) for path in [source, schema_path, *package.rglob('*.nim')]}
    args.work.mkdir(parents=True, exist_ok=False)
    probe = args.work / 'members.nim'
    probe.write_text(MACRO + ''.join('inspectRecord(' + json.dumps(record['symbol']) +
                                    ', ' + record['symbol'] + ')\n' for record in records))
    command = [str(args.nim), 'check', '--cpu:wasm32', '--os:any', '--mm:arc',
               '--threads:off', '--exceptions:goto', '-d:wasm', '-d:useMalloc',
               '--path:' + str(package), '--path:' + str(generated / 'nim'), str(probe)]
    log = args.work / 'members.log'
    with log.open('wb') as output:
        result = subprocess.run(command, stdout=output, stderr=subprocess.STDOUT, timeout=120)
    report = {'kind': 'task03-Nim-compiler-located-generated-field-docs-v1',
              'status': 'FAILED_RETAINED', 'argv': command, 'exit': result.returncode,
              'compiler_sha256': digest(args.nim), 'probe_sha256': digest(probe),
              'source_pins': pins, 'log_sha256': digest(log), 'records': [],
              'Guest_runs': 0, 'native_ROS_runs': 0, 'default_authority': [],
              'documentation_acceptance': False,
              'method': 'Actual exported AST identifiers and exact compiler source positions; native trailing comments on the same pinned declaration',
              'jsondoc_quoted_field_rendering_limitation': True}

    def save():
        args.report.write_text(json.dumps(report, indent=2) + '\n')

    save()
    if result.returncode:
        raise ValueError('Retained genuine Nim member-AST failure: ' + str(log))
    lines = source.read_text().splitlines()
    actual_records = []
    actual_fields = {}
    for line in log.read_text(errors='strict').splitlines():
        if line.startswith('PUBLIC_RECORD\t'):
            actual_records.append(line.split('\t')[1])
        elif line.startswith('PUBLIC_FIELD\t'):
            _, symbol, name, lineno, column, field_type, filename = line.split('\t')
            if Path(filename).resolve(strict=True) != source:
                raise ValueError('Compiler field source escaped its pinned generated module')
            key = symbol, name
            if key in actual_fields:
                raise ValueError('Duplicate actual compiler field declaration')
            actual_fields[key] = dict(line=int(lineno), column=int(column), type=field_type)
    if actual_records != [record['symbol'] for record in records]:
        raise ValueError('Compiler type inventory differs from the complete schema IR')
    expected_fields = {(record['symbol'], member['name']) for record in records for member in record['members']}
    if set(actual_fields) != expected_fields:
        raise ValueError('Actual exported compiler fields differ from complete original member inventory')
    for record in records:
        row = {'symbol': record['symbol'], 'native_type': record['type'], 'members': []}
        for member in record['members']:
            location = actual_fields[record['symbol'], member['name']]
            source_line = lines[location['line'] - 1]
            spelling = record['member_map']['nim'][member['name']]
            declaration = source_line.split('##', 1)[0].strip()
            if not re.match(re.escape(spelling) + r'\s*\*\s*:', declaration):
                raise ValueError('Compiler location does not bind the exact public source field')
            attached = source_line.split('##', 1)[1].strip() if '##' in source_line else ''
            if any(tag not in attached for tag in ('@brief', '@details', '@units', '@lifetime', '@see')):
                raise ValueError('Actual exported member lacks its complete attached source contract')
            qualified = record['type'] + '.' + member['name'] + ':'
            if qualified not in attached:
                raise ValueError('Member documentation describes another original record/field')
            row['members'].append(dict(name=member['name'], compiler_location=location,
                                       declaration=declaration, attached_doc=attached,
                                       original_field_contract=member))
        report['records'].append(row)
    if any(digest(Path(path)) != expected for path, expected in pins.items()):
        raise ValueError('Read-only parser input changed during field inspection')
    report.update(status='PASS_COMPILER_LOCATED_PUBLIC_FIELD_CONTRACTS',
                  record_count=len(records), field_count=len(actual_fields), inputs_unchanged=True)
    save()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
