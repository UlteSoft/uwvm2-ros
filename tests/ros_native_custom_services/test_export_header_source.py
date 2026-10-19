"""Authored, unexecuted DATA policy tests; no native proof from model identities.

A future externally owned test runner may import ONLY this tool/source. These
fabricated lexical identity strings are TEST DATA for serialization, never a
native SDK export, generated codec, graph receipt or accepted vendor identity.
"""
import importlib.util,pathlib,tempfile,unittest,os
PATH=pathlib.Path(__file__).with_name('make_export_header.py')
spec=importlib.util.spec_from_file_location('reviewed_export_data_tool',PATH)
tool=importlib.util.module_from_spec(spec);spec.loader.exec_module(tool)
class SourcePolicy(unittest.TestCase):
 def test_duplicate_object_key_refused(self):
  with self.assertRaises(ValueError):tool.unique([('x',1),('x',2)])
 def test_extent_limit(self):
  with tempfile.TemporaryDirectory()as d:
   p=pathlib.Path(d)/'input';p.write_bytes(b'12345')
   with self.assertRaises(ValueError):tool.opened(p,4)
 def test_exact_extent_bytes(self):
  with tempfile.TemporaryDirectory()as d:
   p=pathlib.Path(d)/'input';p.write_bytes(b'zero\0native')
   self.assertEqual(tool.opened(p,11),b'zero\0native')
 def test_symlink_final_refused(self):
  with tempfile.TemporaryDirectory()as d:
   p=pathlib.Path(d)/'input';p.write_bytes(b'x');q=pathlib.Path(d)/'link';q.symlink_to(p)
   with self.assertRaises(OSError):tool.opened(q,1)
 def test_directory_refused(self):
  with tempfile.TemporaryDirectory()as d:
   with self.assertRaises((ValueError,OSError)):tool.opened(d,1)
if __name__=='__main__':unittest.main()
