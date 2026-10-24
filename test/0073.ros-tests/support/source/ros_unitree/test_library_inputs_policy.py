"""Actual-kernel copy/data policy tests, not CPP/GO/native acceptance.

A Root-owned frozen Linux policy runner supplies the already SHA-bound library
and restart modules, real current guard and a fresh owned fixture parent. This
factory executes no test at import time and imports no project helper. No SDK,
compiler, ROS context, simulated receipt or Guest is used by these copy tests.
"""
from pathlib import Path
import os
import tempfile
import types
import unittest


def suite(library,restart,guard,parent):
    class Policy(unittest.TestCase):
        def setUp(self):
            guard();self.temp=tempfile.TemporaryDirectory(prefix='library-policy-',dir=parent)
            self.root=Path(self.temp.name);source=self.root/'input';source.mkdir()
            output=self.root/'output';output.mkdir();self.guard_hook=None
            def actual_guard():
                guard()
                if self.guard_hook:self.guard_hook()
            def pin(path):return restart.opened_file(str(path),64*library.MIB,actual_guard)
            self.frame=types.SimpleNamespace(source=source,roots=[source],out=output,pin=pin,guard=actual_guard)
            # A consuming build has its own source/output and must explicitly
            # inventory the producer's completed copy directory as INPUT.
            # Merely owning that directory during preparation grants no later
            # input authority. The historical original is deliberately absent
            # from the consumer's source and selected roots.
            self.consumer_source=self.root/'consumer-source';self.consumer_source.mkdir()
            self.consumer_output=self.root/'consumer-output';self.consumer_output.mkdir()
            self.source=source/'original';self.source.write_bytes(b'actual library bytes\x00'*4096)
            self.expected=pin(self.source);self.destination=output/'library/leaf'
        def tearDown(self):
            self.temp.cleanup();guard()
        def copy(self):return library.descriptor_copy(self.frame,restart,self.expected,self.destination)
        def manifest(self,row):return {'object':{'historical':row['source'],**row}}
        def consumer(self,*,admit=True):
            """Observe every actual copied-root leaf before granting this input.

            This small filesystem fixture is not a real Frame or an asserted
            producer receipt. It models the production B-stage selection:
            one already-existing complete copy root, distinct owned output,
            and ordinary actual opened-file pins. No historical name is read.
            """
            guard();root=self.frame.out;inventory={}
            if admit:
                for folder,dirs,names in os.walk(root,followlinks=False):
                    guard();dirs.sort();names.sort()
                    for name in dirs:
                        path=Path(folder)/name
                        self.assertFalse(path.is_symlink());self.assertTrue(path.is_dir())
                    for name in names:
                        path=Path(folder)/name
                        self.assertFalse(path.is_symlink());inventory[str(path)]=self.frame.pin(path)
            result=types.SimpleNamespace(source=self.consumer_source,roots=[root] if admit else [],
                out=self.consumer_output,pin=self.frame.pin,guard=self.frame.guard,input_catalog=inventory)
            self.assertNotEqual(result.out,root);self.assertNotIn(self.frame.source,result.roots)
            if admit:self.assertIn(str(self.destination),result.input_catalog)
            return result
        def test_full_bytes_new_inode_and_sealed_mode(self):
            row=self.copy();self.assertEqual(self.source.read_bytes(),self.destination.read_bytes())
            self.assertNotEqual((row['source']['device'],row['source']['inode']),(row['copy']['device'],row['copy']['inode']))
            self.assertEqual(self.destination.stat().st_mode,0o100400)
            self.assertEqual(self.frame._library_open_descriptors,0)
            consumer=self.consumer()
            library.copied_leaves(consumer,self.manifest(row),self.frame.out)
        def test_unadmitted_producer_owned_output_is_not_consumer_input(self):
            row=self.copy()
            with self.assertRaisesRegex(ValueError,'physical selected input scope required'):
                library.copied_leaves(self.frame,self.manifest(row),self.frame.out)
            with self.assertRaisesRegex(ValueError,'physical selected input scope required'):
                library.copied_leaves(self.consumer(admit=False),self.manifest(row),self.frame.out)
        def test_empty_ordinary_source_is_a_real_copy(self):
            self.source.write_bytes(b'');self.expected=self.frame.pin(self.source)
            row=self.copy();self.assertEqual(row['copy']['bytes'],0)
            self.assertNotEqual(row['copy']['inode'],row['source']['inode'])
        def test_existing_destination_never_overwritten(self):
            self.destination.parent.mkdir();self.destination.write_bytes(b'keep')
            with self.assertRaises(ValueError):self.copy()
            self.assertEqual(self.destination.read_bytes(),b'keep')
        def test_source_symlink_is_not_an_input(self):
            link=self.source.parent/'alias';link.symlink_to(self.source.name)
            previous=dict(self.expected,path=str(link))
            with self.assertRaises(ValueError):library.descriptor_copy(self.frame,restart,previous,self.destination)
        def test_source_hardlink_is_not_an_independent_leaf(self):
            os.link(self.source,self.source.parent/'other-name');self.expected=self.frame.pin(self.source)
            with self.assertRaises(ValueError):self.copy()
            self.assertEqual(self.frame._library_open_descriptors,0)
        def test_mutation_while_both_files_open_is_refused(self):
            done=[]
            def mutate():
                if not done and getattr(self.frame,'_library_open_descriptors',0)==4:
                    self.source.write_bytes(b'changed');done.append(True)
            self.guard_hook=mutate
            with self.assertRaises(ValueError):self.copy()
            self.assertTrue(done);self.assertEqual(self.frame._library_open_descriptors,0)
        def test_copy_mode_or_link_change_is_refused(self):
            row=self.copy();os.chmod(self.destination,0o600)
            # Isolate the sealed-mode contract from the earlier full7 identity
            # check: admit the actual mutated ordinary bytes and current pin,
            # then require copied_leaves itself to refuse mode600. A stale pin
            # or unselected producer output must not make this negative pass.
            row['copy']=self.frame.pin(self.destination)
            with self.assertRaisesRegex(ValueError,'copied leaf mode or link topology changed'):
                library.copied_leaves(self.consumer(),self.manifest(row),self.frame.out)
            os.chmod(self.destination,0o400)
            os.link(self.destination,self.destination.parent/'unexpected')
            row['copy']=self.frame.pin(self.destination)
            with self.assertRaisesRegex(ValueError,'copied leaf mode or link topology changed'):
                library.copied_leaves(self.consumer(),self.manifest(row),self.frame.out)
        def test_consumer_does_not_reopen_historical_source(self):
            row=self.copy();self.source.unlink()
            result=library.copied_leaves(self.consumer(),self.manifest(row),self.frame.out)
            self.assertIn(row['copy'],result.values())
        def test_original_md_target_and_spellings_remain_historical(self):
            raw=b'/old/build/item.o: /old/source/item.cpp \\\n /old/build/../sdk/header.h\n'
            self.assertEqual(library.md_spellings(raw,'/old/build/item.o','/old/source/item.cpp','/old/build'),
                ['/old/source/item.cpp','/old/sdk/header.h'])
            with self.assertRaises(ValueError):library.md_spellings(raw,'/new/copied/item.o','/old/source/item.cpp','/old/build')
        def test_unaccounted_md_rule_or_source_is_refused(self):
            with self.assertRaises(ValueError):library.md_spellings(b'/old/item.o: /old/header.h\n','/old/item.o','/old/item.cpp','/old')
            with self.assertRaises(ValueError):library.md_spellings(b'/old/item.o: /old/item.cpp\nother:\n','/old/item.o','/old/item.cpp','/old')
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policy)
