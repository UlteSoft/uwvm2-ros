#!/usr/bin/env python3
"""Check canonical positive ROS command tables without executing test drivers.

Production/build removed-backend checks remain in the shell audit. Upstream,
negative-refusal and test-local offline-IR vocabularies are not product routes.
This finite source check is never runtime or comprehensive driver coverage.
"""
from __future__ import annotations
import argparse
import ast
import hashlib
import json
from pathlib import Path

# These are positive command authorities, not rejection-probe lists.
AUTHORITIES = (
    ('tools/ci/run_wasm3_core3_native_17_modes.py', 'modes', 'branch'),
    ('test/0014.llvm_jit/run_wasm3_multi_memory.py', 'configurations', 'branch'),
    ('test/0014.llvm_jit/run_wasm3_branch_semantics.py', 'ROS', 'constant'),
    ('test/0017.runtime/run_wasm_exception_performance.py', 'ROS', 'constant'),
)
FORBIDDEN = frozenset(('-Rint', '-Rjit', '-Rtiered', '-Rcc', '-Rcm',
    '-Rllvm-call-stack', '--runtime-int', '--runtime-jit', '--runtime-tiered',
    '--runtime-custom-compiler', '--runtime-custom-mode',
    '--runtime-llvm-jit-call-stack', '--runtime-llvm-jit-lazy-policy'))


def ros_table(source: str, symbol: str, kind: str) -> ast.AST:
    tree = ast.parse(source)
    if kind == 'constant':
        matches = [node.value for node in tree.body if isinstance(node, ast.Assign)
                   and any(isinstance(name, ast.Name) and name.id == symbol for name in node.targets)]
    else:
        functions = [node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == symbol]
        if len(functions) != 1:
            raise ValueError('missing or duplicate route function: ' + symbol)
        branches = [node for node in functions[0].body if isinstance(node, ast.If)
                    and isinstance(node.test, ast.Name) and node.test.id == 'ros']
        matches = [node.value for branch in branches for node in branch.body if isinstance(node, ast.Return)]
    if len(matches) != 1:
        raise ValueError('positive ROS table is not a unique explicit authority: ' + symbol)
    return matches[0]


def verify_table(table: ast.AST) -> dict:
    if not isinstance(table, (ast.List, ast.Tuple, ast.Dict)):
        raise ValueError('ROS positive table must be a literal finite sequence or mapping')
    count = len(table.keys) if isinstance(table, ast.Dict) else len(table.elts)
    if count != 1:
        raise ValueError('ROS has exactly one mandatory-native execution row')
    tokens = [node.value for node in ast.walk(table) if isinstance(node, ast.Constant) and isinstance(node.value, str)]
    bad = sorted({token for token in tokens if token.split('=', 1)[0] in FORBIDDEN})
    if bad:
        raise ValueError('unsupported positive ROS option(s): ' + ', '.join(bad))
    if '-Raot' not in tokens:
        raise ValueError('positive ROS route must select the full native compiler')
    return {'rows': count, 'literal_options': [token for token in tokens if token.startswith('-')]}


def audit(root: Path) -> list[dict]:
    rows = []
    for relative, symbol, kind in AUTHORITIES:
        raw = (root / relative).read_bytes()
        result = verify_table(ros_table(raw.decode('utf-8'), symbol, kind))
        rows.append({'path': relative, 'symbol': symbol, 'sha256': hashlib.sha256(raw).hexdigest(), **result})
    return rows


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    rows = audit(args.root.resolve(strict=True))
    report = {'source_contract': 'PASS', 'scope': 'four canonical positive ROS route authorities only',
              'runtime_execution': 'UNTESTED', 'upstream_matrices_preserved': True, 'authorities': rows}
    if args.output:
        args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('positive ROS source-route audit: four finite native authorities; runtime execution remains separate')


if __name__ == '__main__':
    main()
