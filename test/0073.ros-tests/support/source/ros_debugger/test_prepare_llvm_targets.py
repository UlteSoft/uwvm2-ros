"""Small backend-selection DATA factory, not a build or target execution test.

Root passes the exact complete-MF admitted prepare_llvm module. This file
imports no project source and calls no factory at module load. Real additional
archives and their file-API/MD/ELF closure remain prepare_llvm's actual job.
"""
import unittest


def suite(prepare):
    class Backends(unittest.TestCase):
        def test_version1_keeps_original_native_x86_selection(self):
            self.assertEqual(prepare.backend_targets({'format_version':1}),['X86'])
        def test_all_eight_runtime_isas_have_seven_explicit_backend_names(self):
            # X86 serves both i386 and x86_64; this does not change ELF class.
            selected=['AArch64','ARM','RISCV','LoongArch','PowerPC','SystemZ']
            self.assertEqual(prepare.backend_targets({'format_version':2,'additional_llvm_targets':selected}),['X86',*selected])
        def test_each_explicit_backend_preserves_native_x86_first(self):
            for backend in prepare.EXTRA_BACKENDS:
                self.assertEqual(prepare.backend_targets({'format_version':2,'additional_llvm_targets':[backend]}),['X86',backend])
        def test_version1_cannot_silently_select_extra_backend(self):
            with self.assertRaises(ValueError):prepare.backend_targets({'format_version':1,'additional_llvm_targets':['LoongArch']})
        def test_unknown_all_or_host_alias_is_refused(self):
            for backend in ('all','native','loongarch64','X86',';LoongArch'):
                with self.assertRaises(ValueError):prepare.backend_targets({'format_version':2,'additional_llvm_targets':[backend]})
        def test_empty_implicit_selection_is_refused(self):
            for value in (None,[],{},'LoongArch'):
                with self.assertRaises(ValueError):prepare.backend_targets({'format_version':2,'additional_llvm_targets':value})
        def test_duplicate_or_nontext_backend_is_refused(self):
            for value in (['ARM','ARM'],[True],[None]):
                with self.assertRaises(ValueError):prepare.backend_targets({'format_version':2,'additional_llvm_targets':value})
        def test_boolean_or_unknown_schema_version_is_refused(self):
            for value in (True,0,3,'2'):
                with self.assertRaises(ValueError):prepare.backend_targets({'format_version':value})
    return unittest.defaultTestLoader.loadTestsFromTestCase(Backends)
