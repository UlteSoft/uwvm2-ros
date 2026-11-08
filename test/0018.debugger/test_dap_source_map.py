#!/usr/bin/env python3
"""Explicit reproducible compiler paths map both IDE display and breakpoints."""
import importlib.util
from pathlib import Path
import unittest
SPEC=importlib.util.spec_from_file_location('source_map_dap',Path(__file__).resolve().parents[2]/'tools/debug/dap_adapter.py')
dap=importlib.util.module_from_spec(SPEC);SPEC.loader.exec_module(dap)
class SourceMapTests(unittest.TestCase):
    def test_component_longest_match_and_reverse(self):
        maps=dap.compile_source_map({'/sdk':'/work/sdk','/application':'/work/project','/sdk/special':'/work/override'})
        for original,local in [('/sdk/include/a.hpp','/work/sdk/include/a.hpp'),('/application/main.c','/work/project/main.c'),('/sdk/special/a.d','/work/override/a.d')]:
            self.assertEqual(dap.map_source_path(original,maps),local)
            self.assertEqual(dap.map_source_path(local,maps,reverse=True),original)
        self.assertEqual(dap.map_source_path('/sdk-other/a',maps),'/sdk-other/a')
        self.assertEqual(dap.map_source_path('relative/file.rs',maps),'relative/file.rs')
    def test_windows_paths_and_spaces(self):
        maps=dap.compile_source_map({'C:\\sources':'D:\\source tree'})
        self.assertEqual(dap.map_source_path('C:\\sources\\main.cpp',maps),'D:/source tree/main.cpp')
        self.assertEqual(dap.map_source_path('D:/source tree/main.cpp',maps,True),'C:/sources/main.cpp')
    def test_bad_and_ambiguous_config(self):
        for value in [None,[],{'/sdk':False},{'relative':'/work'},{'/sdk':'/work/../other'},{'/sdk':'/a\ncommand'}, {'/a':'/work','/b':'/work'}]:
            with self.subTest(value=value),self.assertRaises(ValueError):dap.compile_source_map(value)
    def test_mapped_path_does_not_escape_root(self):
        maps=dap.compile_source_map({'/sdk':'/work/sdk'})
        for value in ['/sdk/../secret','/sdk/a/../../secret']:
            with self.assertRaises(ValueError):dap.map_source_path(value,maps)
        with self.assertRaises(ValueError):dap.map_source_path('/work/sdk/../secret',maps,True)
if __name__=='__main__':unittest.main()
