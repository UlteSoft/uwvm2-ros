#!/usr/bin/env python3
"""Finite source contracts for the two pure Unitree Wasm provider recipes.

These checks read the declared recipe only. They do not expand tool paths,
import a build producer, invoke a compiler, parse a compiled Wasm binary, run
callbacks or publish ROS. Actual full-MD builds, retained linker traces, Wasm
validation and exact binary boundaries remain separate Root-owned jobs.
"""
import json
from pathlib import Path
import unittest


SDK = Path(__file__).resolve().parents[1]


class UnitreePreloadModels(unittest.TestCase):
    def setUp(self):
        self.recipe = json.loads((SDK / 'examples/robotics_modular/unitree-preload-recipes.json').read_text())

    def test_explicit_wasm_selector_and_serial_float_contract(self):
        """Prevent the actual LLVM22 ignored-selector/link-group regression."""
        self.assertEqual(self.recipe['jobs'], 1)
        self.assertEqual(self.recipe['command_rss_max_bytes'], 512 * 1024**2)
        self.assertEqual(self.recipe['command_timeout_seconds'], 600)
        builds = self.recipe['builds']
        self.assertEqual([row['id'] for row in builds], ['unitree-callback', 'unitree-motion'])
        for row in builds:
            argv = row['argv']
            self.assertEqual(argv[0], '{WASI_CLANG}')
            self.assertEqual(argv.count('-fuse-ld={WASM_LD}'), 1)
            self.assertFalse(any(flag.startswith('--ld-path=') or flag in
                ('-Wl,--start-group', '-Wl,--end-group') for flag in argv))
            for flag in ('--target=wasm32-wasip1', '--sysroot={WASI_SYSROOT}',
                         '-O3', '-fno-fast-math', '-ffp-contract=off', '-nostdlib',
                         '-Wl,--no-entry', '-Wl,--threads=1',
                         '-Wl,--initial-memory=131072', '-Wl,--max-memory=131072'):
                self.assertIn(flag, argv)

    def test_archive_selection_and_pure_module_authority(self):
        """Source declarations do not replace later actual binary inspection."""
        callback, motion = self.recipe['builds']
        archives = ('{WASI_LIBM}', '{WASI_LIBC}', '{WASI_BUILTINS}')
        self.assertTrue(all(value not in callback['argv'] for value in archives))
        self.assertTrue(all(motion['argv'].count(value) == 1 for value in archives))
        self.assertEqual(self.recipe['module_names'],
                         {'unitree-callback': 'callbacks', 'unitree-motion': 'motion'})
        self.assertEqual(self.recipe['contracts']['unitree-callback']['imports'], {})
        self.assertEqual(set(self.recipe['contracts']['unitree-motion']['imports']),
                         {'callbacks.joint_begin', 'callbacks.on_joint',
                          'callbacks.joint_end', 'callbacks.joint_abort'})
        self.assertEqual(self.recipe['memory'], {'count': 1, 'initial_pages': 2,
                         'maximum_pages': 2, 'shared': False, 'memory64': False})
        for literal in ('-MD -MF row.d -MT row.o', 'linker --trace'):
            self.assertIn(literal, self.recipe['full_dependencies'])


if __name__ == '__main__':
    unittest.main()
