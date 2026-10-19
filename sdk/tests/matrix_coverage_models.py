#!/usr/bin/env python3
"""Pure source/recipe checks for dedicated C/C++ complete ABI coverage.

This model reads the current public ABI schema and source text. It extracts
only side-effect-free selector/recipe functions from the planner AST; it never
imports that planner, loads a cgroup probe, invokes a compiler or executes Wasm
or ROS. Actual binary imports and full validation remain separate Linux jobs.
"""
import ast
import hashlib
import json
from pathlib import Path
import re
from types import SimpleNamespace
import unittest

SDK = Path(__file__).resolve().parents[1]


def pure_recipes():
    """Load only reviewed pure constructors, independent of external context."""
    path = SDK / 'tools/build_cgroup_matrix.py'
    wanted = {'complete_abi_required', 'recipes', 'custom_example_recipes'}
    tree = ast.parse(path.read_text())
    selected = [node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name in wanted]
    if {node.name for node in selected} != wanted:
        raise AssertionError('current explicit pure recipe functions are missing')
    namespace = {'SDK': SDK, 'ROOT': SDK.parent, 'Path': Path,
                 'digest': lambda value: hashlib.sha256(Path(value).read_bytes()).hexdigest()}
    code = ast.fix_missing_locations(ast.Module(body=selected, type_ignores=[]))
    exec(compile(code, str(path), 'exec'), namespace)
    return namespace


def pure_variants(constructor, selected):
    """Replay just frozen row expansion, before any symbols-file write."""
    path = SDK / 'tools/run_local_checks.py'
    tree = ast.parse(path.read_text())
    function = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == 'wasm_checks')
    def assigns(node, name):
        return isinstance(node, ast.Assign) and any(isinstance(item, ast.Name) and item.id == name for item in node.targets)
    begin = [index for index, node in enumerate(function.body) if assigns(node, 'rows')]
    end = [index for index, node in enumerate(function.body) if assigns(node, 'schema')]
    if len(begin) != 1 or len(end) != 1 or begin[0] >= end[0]:
        raise AssertionError('pure row-expansion boundaries changed')
    nodes = function.body[begin[0]:end[0]]
    if any(isinstance(item, (ast.Import, ast.ImportFrom)) or isinstance(item, ast.Attribute) and
           item.attr in ('run', 'Popen', 'write_text', 'write_bytes', 'mkdir')
           for node in nodes for item in ast.walk(node)):
        raise AssertionError('side effect in source-only row expansion')
    namespace = {'module': SimpleNamespace(recipes=constructor), 'args': selected,
                 'base': Path('/explicit-unwritten-model-output'), 'Path': Path}
    code = ast.fix_missing_locations(ast.Module(body=nodes, type_ignores=[]))
    exec(compile(code, str(path), 'exec'), namespace)
    return namespace['rows']


class CompleteAbiCoverageModels(unittest.TestCase):
    def test_shared_probe_has_every_exact_schema_call_and_scalar_width(self):
        source = (SDK / 'examples/c/abi_coverage_impl.h').read_text()
        schema = json.loads((SDK / 'abi/abi-v1.json').read_text())
        probe = source.split('uint32_t roswasm_abi_coverage(uint32_t index) {', 1)[1]
        first, second = probe.split('#else', 1)
        for version, block in ((1, first), (2, second)):
            rows = re.findall(r'case ([0-9]+): return uwvm_ros([12])_([a-z0-9_]+)\((.*?)\);', block)
            expected = schema['modules']['uwvm_ros' + str(version) + '_v1']['functions']
            self.assertEqual(len(rows), len(expected))
            self.assertEqual([int(row[0]) for row in rows], list(range(len(expected))))
            self.assertEqual({row[2] for row in rows}, {row['name'] for row in expected})
            functions = {row['name']: row for row in expected}
            for _, backend, name, arguments in rows:
                self.assertEqual(int(backend), version)
                tokens = arguments.split(',') if arguments else []
                self.assertTrue(all(token in ('0u', 'UINT64_C(0)') for token in tokens))
                widths = ['i32' if token == '0u' else 'i64' for token in tokens]
                self.assertEqual(widths, [item['type'] for item in functions[name]['params']], name)
                self.assertEqual(functions[name]['results'], ['i32'])
        self.assertIn('export_name("roswasm_abi_coverage"), used, noinline', source)
        self.assertIn('switch(index)', probe)
        self.assertIn('default: return 95u;', first)
        self.assertIn('default: return 95u;', second)

    def test_both_wrappers_share_probe_but_main_performs_no_ros_call(self):
        for relative, include in (('examples/c/00_abi_coverage.c', '"abi_coverage_impl.h"'),
                                  ('examples/cpp/00_abi_coverage.cpp', '"../c/abi_coverage_impl.h"')):
            source = (SDK / relative).read_text()
            self.assertIn('#include ' + include, source)
            self.assertRegex(source, r'int main\((?:void)?\)\s*\{\s*return 0;\s*\}')
            self.assertNotRegex(source, r'uwvm_ros[12]_[a-z0-9_]+\s*\(')
            self.assertIn('not a runnable ROS lesson', source)

    def test_only_dedicated_application_rows_require_complete_imports(self):
        constructor = pure_recipes()['complete_abi_required']
        for language in ('c', 'cpp'):
            self.assertTrue(constructor(language, '00_abi_coverage'))
            for name in ('04_service_action_resources', '04_services_actions', '04_other', '01_raw_imports'):
                self.assertFalse(constructor(language, name))
        for language in ('rust', 'tinygo'):
            self.assertTrue(constructor(language, 'coverage'))
            self.assertFalse(constructor(language, 'resources'))
        for name in ('bridge', 'relay'):
            self.assertTrue(constructor('javascript_bridge', name))

    def test_c_cpp_recipes_keep_four_full_probes_and_typed_examples_separate(self):
        namespace = pure_recipes()
        selected = SimpleNamespace(languages=['c', 'cpp'], backends=['1', '2'],
            wasi_sdk=Path('/explicit-unexecuted-wasi-sdk'), wasm_tools=Path('/explicit-unexecuted-wasm-tools'), example_variants=True)
        rows = pure_variants(namespace['recipes'], selected)
        complete = [row for row in rows if row['require_all_imports']]
        self.assertEqual({row['id'] for row in complete}, {
            language + '-ros' + backend + '-00_abi_coverage'
            for language in ('c', 'cpp') for backend in ('1', '2')})
        for row in complete:
            self.assertTrue(row['expects_ros_imports'])
            self.assertIn('-Wl,--export=roswasm_abi_coverage', row['argv'])
            self.assertTrue(Path(row['source']).is_file())
        for language, name in (('c', '04_service_action_resources'), ('cpp', '04_services_actions')):
            typed = [row for row in rows if row['language'] == language and Path(row['source']).stem == name]
            self.assertEqual(len(typed), 14)
            self.assertEqual({row['id'] for row in typed}, {
                language + '-ros' + backend + '-' + name + '-case' + str(case)
                for backend in ('1', '2') for case in range(7)})
            self.assertTrue(all(not row['require_all_imports'] and row['expects_ros_imports'] for row in typed))
            for row in typed:
                case = int(row['id'].rsplit('case', 1)[1])
                self.assertEqual(row['variant'], ['-DROSWASM_EXAMPLE_CASE=' + str(case)])
        # A new first-sorted 00 fixture must not leak its required probe export
        # into the independently compiled custom packet application's recipe.
        custom = [row for row in rows if row.get('custom_example')]
        self.assertEqual(len(custom), 2)
        self.assertTrue(all(not row['require_all_imports'] and
            '-Wl,--export=roswasm_abi_coverage' not in row['argv'] for row in custom))


if __name__ == '__main__':
    unittest.main()
