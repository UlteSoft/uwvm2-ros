#!/usr/bin/env python3
"""Pure positive/negative models for the actual raw Go backend source guard.

Run ``python3 -B sdk/origin/tests/go_build_constraint_models.py`` or the maintained
``sdk/origin/tools/verify_sources.py`` aggregate in an owned test environment.  These
models parse source constraints only; they do not run Go, TinyGo, Wasm or ROS.
The Linux unified SDK owner separately compiles its selected real fixtures.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path


import importlib.util
from pathlib import Path
import sys
import unittest

sys.dont_write_bytecode = True
SDK = _CHECKOUT_ROOT / 'sdk/origin'
SPEC = importlib.util.spec_from_file_location('sdk_go_build_constraints', _sdk_path(SDK, 'tools/go_build_constraints.py'))
CONSTRAINTS = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(CONSTRAINTS)


class GoBuildConstraintModels(unittest.TestCase):
    """Both true platform alternatives must retain exact backend isolation."""

    def source(self, expression):
        return '//go:build ' + expression + '\n\npackage roswasm\n'

    def valid(self, backend):
        return f'tinygo && (wasm || tinygo.wasm) && ros{backend} && !ros{3 - backend}'

    def rejected(self, expression, backend=1):
        with self.assertRaises(ValueError):
            CONSTRAINTS.validate_backend_source(self.source(expression), backend)

    def test_actual_raw_bindings_and_event_adapters_have_full_truth_tables(self):
        for backend in (1, 2):
            for stem in ('raw', 'event_backend'):
                with self.subTest(backend=backend, source=stem):
                    row = CONSTRAINTS.validate_backend_source(
                        (_sdk_path(SDK, f'tinygo/{stem}_ros{backend}.go')).read_text(), backend)
                    self.assertEqual(row['assignments_checked'], 32)
                    self.assertEqual(len(row['selected_assignments']), 3)
                    for tags in row['selected_assignments']:
                        self.assertTrue(tags['tinygo'])
                        self.assertTrue(tags['wasm'] or tags['tinygo.wasm'])
                        self.assertTrue(tags[f'ros{backend}'])
                        self.assertFalse(tags[f'ros{3 - backend}'])

    def test_equivalent_parentheses_and_order_preserve_the_same_contract(self):
        for backend in (1, 2):
            expression = f'!ros{3-backend} && ros{backend} && ((tinygo.wasm || wasm) && tinygo)'
            self.assertEqual(CONSTRAINTS.validate_backend_source(self.source(expression), backend)['backend'], backend)

    def test_missing_opposite_backend_exclusion_is_refused(self):
        self.rejected('tinygo && (wasm || tinygo.wasm) && ros1')
        self.rejected('tinygo && (wasm || tinygo.wasm) && (ros1 || ros2)')

    def test_wrong_backend_and_both_selected_backends_are_refused(self):
        self.rejected(self.valid(2))
        self.rejected('tinygo && (wasm || tinygo.wasm) && ros1 && ros2')

    def test_platform_or_compiler_bypasses_are_refused(self):
        self.rejected('(wasm || tinygo.wasm) && ros1 && !ros2')
        self.rejected('tinygo && ros1 && !ros2')
        self.rejected(self.valid(1) + ' || ros2')
        self.rejected('tinygo && wasm || tinygo.wasm && ros1 && !ros2')

    def test_losing_either_supported_wasm_platform_is_refused(self):
        self.rejected('tinygo && wasm && ros1 && !ros2')
        self.rejected('tinygo && tinygo.wasm && ros1 && !ros2')
        self.rejected('tinygo && (wasm && tinygo.wasm) && ros1 && !ros2')

    def test_unknown_tags_and_non_go_tokens_are_refused(self):
        for expression in (self.valid(1) + ' && extra', 'True', 'tinygo & wasm',
                           'tinygo | wasm', 'tinygo == wasm', '__import__(x)',
                           self.valid(1) + ' # comment',
                           self.valid(1).replace(' && ', '\u00a0&& ', 1)):
            with self.subTest(expression=expression):
                self.rejected(expression)

    def test_malformed_and_trailing_expressions_are_refused(self):
        for expression in ('', '()', 'tinygo &&', '! ', '(tinygo', 'tinygo)',
                           'tinygo wasm', 'tinygo || || wasm', 'tinygo !wasm'):
            with self.subTest(expression=expression):
                self.rejected(expression)

    def test_absent_duplicate_legacy_or_misplaced_directives_are_refused(self):
        valid = self.source(self.valid(1))
        for source in ('package roswasm\n', '\n' + valid,
                       valid + '//go:build ros2\n', valid + '// +build ros1\n',
                       valid.replace('\n\npackage', '\npackage')):
            with self.subTest(source=source):
                with self.assertRaises(ValueError):
                    CONSTRAINTS.validate_backend_source(source, 1)

    def test_constraints_and_sources_have_finite_parser_budgets(self):
        for expression in (' ' * 4097, '(' * 40 + 'tinygo' + ')' * 40,
                           ' && '.join(['tinygo'] * 70)):
            with self.subTest(length=len(expression)):
                self.rejected(expression)
        with self.assertRaises(ValueError):
            CONSTRAINTS.validate_backend_source('x' * (1024 * 1024 + 1), 1)

    def test_backend_selection_requires_an_exact_integer(self):
        for backend in (0, 3, True, '1', None):
            with self.subTest(backend=backend):
                with self.assertRaises(ValueError):
                    CONSTRAINTS.validate_backend_source(self.source(self.valid(1)), backend)


if __name__ == '__main__':
    unittest.main(verbosity=2)
