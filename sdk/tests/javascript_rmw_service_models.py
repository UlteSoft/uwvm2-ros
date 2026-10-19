#!/usr/bin/env python3
"""Unexecuted Source and synthetic-binary fixtures for optional JS owned-RMW.

The Linux owner must first admit this entry/interpreter/stdlib and exact selected
source closure under its existing process framework. These models compile/exec
only captured hash-checked producer/checker/inspector Python bytes to exercise
pure constructors and a binary syntax decoder. They run no compiler, engine,
Wasm instance, Runtime, native ROS process or network. A model result is not an
actual compiled-artifact, permission, native-ownership or delivery qualification.
"""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types
import unittest

CONTEXT = {}


def selected(path, digest):
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise ValueError('resolved model source required')
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    with os.fdopen(fd, 'rb') as stream:
        first = os.fstat(stream.fileno())
        if not stat.S_ISREG(first.st_mode) or first.st_size > 2097152:
            raise ValueError('bounded regular model source required')
        raw = stream.read(2097153); last = os.fstat(stream.fileno())
    final = path.stat(follow_symlinks=False)
    fields = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')
    if len(raw) != first.st_size or any(getattr(first, k) != getattr(last, k) or
            getattr(last, k) != getattr(final, k) for k in fields) or hashlib.sha256(raw).hexdigest() != digest:
        raise ValueError('model captured Source differs')
    return raw


def load(raw, path, name):
    if name in sys.modules:
        raise ValueError('model private context collision')
    module = types.ModuleType(name)
    module.__file__, module.__package__ = str(path), ''
    sys.modules[name] = module
    try:
        exec(compile(raw, str(path), 'exec', dont_inherit=True, optimize=0), module.__dict__)
        return module
    except BaseException:
        del sys.modules[name]
        raise


def leb(value):
    out = bytearray()
    while True:
        part = value & 127; value >>= 7; out.append(part | (128 if value else 0))
        if not value:
            return bytes(out)


def name(text):
    raw = text.encode('utf-8'); return leb(len(raw)) + raw


def binary(rows, memory=False):
    """A synthetic valid core type/import module, never a compiled Guest."""
    values = {'i32': 0x7f, 'i64': 0x7e, 'f32': 0x7d, 'f64': 0x7c}
    types_body = bytearray(leb(len(rows)))
    imports = bytearray(leb(len(rows) + int(memory)))
    for index, row in enumerate(rows):
        types_body += b'\x60' + leb(len(row['params'])) + bytes(values[t] for t in row['params'])
        types_body += leb(len(row['results'])) + bytes(values[t] for t in row['results'])
        imports += name(row['module']) + name(row['name']) + b'\0' + leb(index)
    if memory:
        imports += name('unexpected') + name('memory') + b'\x02\x00\x01'
    return b'\0asm\1\0\0\0' + b'\x01' + leb(len(types_body)) + bytes(types_body) + b'\x02' + leb(len(imports)) + bytes(imports)


class Models(unittest.TestCase):
    def setUp(self):
        self.p, self.c, self.i = (CONTEXT[k] for k in ('producer', 'checker', 'inspector'))
        self.catalog, self.original = (copy.deepcopy(CONTEXT[k]) for k in ('catalog', 'original'))
        self.manifest = {'backend': 2, 'mode': 'command', 'imports': [
            {'module': 'numerical', 'name': 'mixed', 'parameters': ['i32', 'u32', 'i64', 'u64', 'f32', 'f64'], 'result': 'f64'}]}
        self.expected = self.p.full_import_contract(self.manifest, self.original, self.catalog)

    def check(self, rows, memory=False):
        data = binary(rows, memory)
        actual = self.i.inspect(data)
        self.c.compare_rows(actual, self.expected)

    def test_exact_complete_synthetic_signature_inventory(self):
        self.assertEqual(len(self.expected), 43 + 12 + 3 + 1)
        self.check(self.expected)
        self.assertEqual([r['name'] for r in self.expected if r['role'] == 'owned-rmw-service'],
                         [r[0] for r in self.p.RMW_SIGNATURES])

    def test_catalog_full12_and_output_mutations_refuse(self):
        cases = []
        missing = copy.deepcopy(self.catalog); missing['modules'][self.p.RMW_MODULE]['functions'].pop(); cases.append(missing)
        wrong = copy.deepcopy(self.catalog); wrong['modules'][self.p.RMW_MODULE]['functions'][7]['params'][2] = 'i64'; cases.append(wrong)
        output = copy.deepcopy(self.catalog); output['output_contract']['minimum_peek_capacity'] = 0; cases.append(output)
        legacy = copy.deepcopy(self.catalog); legacy['legacy_preservation']['catalog_sha256'] = '0' * 64; cases.append(legacy)
        source = copy.deepcopy(self.catalog); source['raw_header']['path'] = '../escape'; cases.append(source)
        for case in cases:
            with self.assertRaises(ValueError): self.p.rmw_catalog_contract(case)

    def test_missing_duplicate_unexpected_wrong_signatures_refuse(self):
        mutations = [self.expected[:-1], self.expected + [self.expected[0]]]
        for field in ('params', 'results'):
            wrong = copy.deepcopy(self.expected)
            wrong[44][field] = ['i32'] if field == 'params' else ['i64']
            mutations.append(wrong)
        foreign = copy.deepcopy(self.expected); foreign[-1]['module'] = 'wasi_snapshot_preview1'; foreign[-1]['name'] = 'fd_seek'; mutations.append(foreign)
        for rows in mutations:
            with self.assertRaises(ValueError): self.check(rows)
        with self.assertRaises(ValueError): self.check(self.expected, memory=True)

    def test_reserved_generic_and_duplicate_key_refuse(self):
        row = {'module': self.p.RMW_MODULE, 'name': 'abi_version', 'parameters': [], 'result': 'u32'}
        with self.assertRaises(ValueError): self.p.scalar_trampolines([], [row])
        with self.assertRaises(ValueError): self.p.full_import_contract({**self.manifest, 'imports': [row]}, self.original, None)
        duplicate = {**self.manifest, 'imports': [self.manifest['imports'][0]] * 2}
        with self.assertRaises(ValueError): self.p.full_import_contract(duplicate, self.original, self.catalog)

    def test_default_off_no_optional_imports_and_ros1_refuses_optional(self):
        off = self.p.full_import_contract(self.manifest, self.original, None)
        self.assertFalse(any(r['module'] == self.p.RMW_MODULE for r in off))
        with self.assertRaises(ValueError): self.p.full_import_contract({**self.manifest, 'backend': 1}, self.original, self.catalog)

    def test_shared_pure_contract_bytes_are_identical(self):
        def region(raw, end):
            text = raw.decode(); return text[text.index("RMW_MODULE ="):text.index(end)].strip()
        self.assertEqual(region(CONTEXT['producer_raw'], 'def main():'),
                         region(CONTEXT['checker_raw'], '# END SHARED PURE CONTRACT DATA'))

    def test_exact_wasi_and_scalar_widths(self):
        rows = {(r['module'], r['name']): r for r in self.expected}
        self.assertEqual(rows['wasi_snapshot_preview1', 'fd_read']['params'], ['i32'] * 4)
        self.assertEqual(rows['wasi_snapshot_preview1', 'proc_exit']['results'], [])
        self.assertEqual(rows['numerical', 'mixed']['params'], ['i32', 'i32', 'i64', 'i64', 'f32', 'f64'])
        self.assertEqual(rows['numerical', 'mixed']['results'], ['f64'])


def main():
    if sys.platform != 'linux' or not sys.flags.isolated or not sys.flags.no_site:
        raise ValueError('externally admitted Linux Python -I -S required')
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    for key in ('producer', 'checker', 'inspector', 'catalog'):
        parser.add_argument('--' + key + '-sha256', required=True)
    args = parser.parse_args()
    root = args.source_root.resolve(strict=True)
    paths = {'producer': 'sdk/javascript/guest/prepare.py', 'checker': 'sdk/tools/check_javascript_guest_imports.py',
             'inspector': 'sdk/tools/check_wasm_imports.py', 'catalog': 'sdk/abi/rmw-service-v1.json'}
    captured = {key: selected(root / relative, getattr(args, key + '_sha256')) for key, relative in paths.items()}
    loaded = []
    try:
        for key in ('producer', 'checker', 'inspector'):
            module = load(captured[key], root / paths[key], '_uwvm_rmw_source_model_' + key)
            loaded.append(module)
            CONTEXT[key] = module
        CONTEXT.update(producer_raw=captured['producer'], checker_raw=captured['checker'],
            catalog=json.loads(captured['catalog']), original=json.loads(selected(root / 'sdk/abi/abi-v1.json', CONTEXT['producer'].LEGACY_CATALOG_SHA256)))
        outcome = unittest.TextTestRunner().run(unittest.defaultTestLoader.loadTestsFromTestCase(Models))
        if not outcome.wasSuccessful(): raise SystemExit(1)
    finally:
        for module in reversed(loaded):
            if sys.modules.get(module.__name__) is module: del sys.modules[module.__name__]
        CONTEXT.clear()
    print('SOURCE_AND_SYNTHETIC_BINARY_MODELS_ONLY: actual compiler/Guest/native_ROS=0')


if __name__ == '__main__':
    main()
