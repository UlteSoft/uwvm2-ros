#!/usr/bin/env python3
"""Prepare real native peers from the independent installed-IDL value oracle.

The containment owner runs this tool before compiling any actor. Captured
catalog, installed source facts and unchanged oracle contract must agree by
digest. Every registered interface gets a separate translation unit to bound
compiler memory without dropping fields. Production codecs never generate the
native expected values used by these peers. No robot driver is started.
"""
import argparse
import json
from pathlib import Path
import re
import runpy


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('catalog', 'installed-idl', 'oracle', 'out'):
        parser.add_argument('--' + name, required=True, type=Path)
    args = parser.parse_args()
    helper = runpy.run_path(str(Path(__file__).resolve().parents[1] / 'industrial_value_oracle.py'))
    read, need, strict, pin = (helper[k] for k in ('read', 'need', 'strict', 'pin'))
    catalog_raw, installed_raw = read(args.catalog), read(args.installed_idl)
    contract = read(args.oracle / 'contract.cc')
    report = strict(read(args.oracle / 'source-oracle.json'))
    need(report['original_catalog'] == pin(catalog_raw) and
         report['original_installed_idl'] == pin(installed_raw) and
         report['contract'] == pin(contract), 'original independent oracle/source pins differ')
    marker = b'unsigned payloads{},cases{},refusals{};'
    need(contract.count(marker) == 1, 'one exact independent value boundary required')
    blocks = contract.split(marker)[0].decode('utf-8').split('template<>struct oracle<')
    values = {}
    for block in blocks[1:]:
        name = block[:block.index('>')]
        need(re.fullmatch(r'::[a-z][a-z0-9_]*::(?:msg|srv|action)::[A-Z][A-Za-z0-9_]*', name)
             and name not in values and block.endswith('}}};\n'),
             'exact complete original native value specialization required')
        values[name] = 'template<>struct oracle<' + block
    catalog, installed = strict(catalog_raw), strict(installed_raw)
    interfaces = {row['type']: row for row in installed['interfaces']}
    rows = [row for row in catalog['types'] if row['registered']]
    need(1 <= len(rows) <= 128, 'finite explicit registered interface selection required')
    output = args.out
    need(output.is_absolute() and output.parent.resolve(strict=True) == output.parent and
         not output.exists() and not output.is_symlink(), 'fresh physical output required')
    output.mkdir(mode=0o700)
    declarations, calls, cases = [], [], []
    counts = dict(message=0, service=0, action=0)
    for ordinal, row in enumerate(rows):
        name = row['type']; interface = interfaces[name]; kind = interface['kind']
        counts[kind] += 1
        package, directory, typename = name.split('/')
        snake = re.sub(r'([a-z0-9])([A-Z])', r'\1_\2',
                       re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', typename)).lower()
        function = 'case_' + str(ordinal)
        declarations.append('void ' + function + '(context&);')
        calls.append(function + '(ctx);')
        members = ''.join(values['::' + component.replace('/', '::')]
                          for component in interface['components'].values())
        source = ('#include "native_common.h"\n#include <' + package + '/' + directory + '/' + snake + '.hpp>\n' +
                  members + '\nvoid ' + function + '(context& ctx){run_' + kind + '<::' +
                  name.replace('/', '::') + '>(ctx,' + json.dumps('v' + str(ordinal)) + ',' +
                  json.dumps(name) + ');}\n')
        (output / (function + '.cc')).write_text(source)
        cases.append(dict(ordinal=ordinal, alias='v' + str(ordinal), type=name, kind=kind))
    (output / 'cases.h').write_text('\n'.join(declarations) + '\ninline void run_cases(context& ctx){\n' +
                                   '\n'.join(calls) + '\n}\n')
    (output / 'selection.json').write_text(json.dumps(dict(cases=cases, counts=counts,
        original_catalog=pin(catalog_raw), original_installed_idl=pin(installed_raw),
        original_oracle=pin(contract), default_authority=[]), sort_keys=True, indent=2) + '\n')
    need(read(args.catalog) == catalog_raw and read(args.installed_idl) == installed_raw and
         read(args.oracle / 'contract.cc') == contract, 'original input drift during preparation')


if __name__ == '__main__':
    main()
