"""Meaningful authored source policies; never executed while this stage is held.

Root may run these against the complete reviewed source tree later. These are
parser/IR/emitter contracts, not actual native TypeSupport/Registry proof.
"""
from pathlib import Path
import sys
import argparse
import hashlib
import os
import re
import stat
import types
import tempfile
import unittest

MAX_SOURCE_BYTES=1024*1024

def _source_identity(info):
    return (info.st_dev, info.st_ino, info.st_size, info.st_mode,
            info.st_nlink, info.st_uid, info.st_gid, info.st_mtime_ns, info.st_ctime_ns)

def _source_unchanged(fd, path, identity):
    if _source_identity(os.fstat(fd)) != identity or _source_identity(os.lstat(path)) != identity:
        raise ValueError(f"generator source identity changed: {path}")

def _open_source_bytes(path, maximum):
    """Keep this SAME ordinary no-follow descriptor through source execution."""
    if any(not hasattr(os, flag) for flag in ('O_RDONLY', 'O_NONBLOCK', 'O_NOFOLLOW', 'O_CLOEXEC')):
        raise ValueError("actual nonblocking/no-follow/close-on-exec source admission is unavailable")
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= maximum:
            raise ValueError(f"generator source must be a bounded regular file: {path}")
        identity = _source_identity(first)
        raw = bytearray()
        while len(raw) <= first.st_size:
            part = os.read(fd, min(65536, first.st_size + 1 - len(raw)))
            if not part:
                break
            raw.extend(part)
            if len(raw) > first.st_size:
                raise ValueError(f"generator source grew during capture: {path}")
        if len(raw) != first.st_size:
            raise ValueError(f"generator source extent changed: {path}")
        _source_unchanged(fd, path, identity)
        return fd, bytes(raw), identity
    except BaseException:
        os.close(fd)
        raise

def _captured_module(name, path, expected_sha256=None, expected_identity=None):
    """Compile captured source directly; never consult or produce its pyc."""
    path = Path(os.path.abspath(os.fspath(path)))
    if expected_sha256 is not None and (not isinstance(expected_sha256, str) or not re.fullmatch(r"[0-9a-f]{64}", expected_sha256)):
        raise ValueError("an explicit lowercase full source SHA256 is required")
    fd, raw, identity = _open_source_bytes(path, MAX_SOURCE_BYTES)
    try:
        digest = hashlib.sha256(raw).hexdigest()
        if (expected_sha256 is not None and digest != expected_sha256) or (expected_identity is not None and identity != expected_identity):
            raise ValueError(f"generator source differs from the admitted source: {path}")
        code = compile(raw, str(path), 'exec', dont_inherit=True)
        module = types.ModuleType(name)
        module.__file__ = str(path)
        module.__package__ = ''
        module.__loader__ = None
        module.__spec__ = None
        module.__cached__ = None
        missing = object()
        previous = sys.modules.get(name, missing)
        # Real dataclass/future annotation resolution requires this entry BEFORE
        # exec. On every exception restore the exact previous module binding.
        sys.modules[name] = module
        try:
            exec(code, module.__dict__)
            _source_unchanged(fd, path, identity)
            if sys.modules.get(name) is not module:
                raise ValueError("captured source replaced its admitted module binding")
            # Commit only the actual captured facts, never code-supplied claims.
            module.__file__ = str(path)
            module.__loader__ = None
            module.__spec__ = None
            module.__cached__ = None
            module._uwvm_source_sha256 = digest
            module._uwvm_source_identity = identity
        except BaseException:
            if previous is missing:
                sys.modules.pop(name, None)
            else:
                sys.modules[name] = previous
            raise
        return module
    finally:
        os.close(fd)


class SourceProfile(unittest.TestCase):
 def rows(self):return bounded.load_catalog(parent,CATALOG,False)
 def test_complete_default_closure(self):
  records,ordered,interfaces,pins=self.rows()
  self.assertEqual(len(interfaces),25);self.assertEqual(len(pins['definitions']),25)
  self.assertEqual(sum(i.registered for i in interfaces),9)
  self.assertIn((2,'type_description_interfaces/srv/GetTypeDescription_Response'),records)
 def test_actual_constants_are_not_fields(self):
  records,_,_,_=self.rows();log=records[(2,'rcl_interfaces/msg/Log')]
  self.assertEqual([(c['name'],c['value']) for c in log.constants],[('DEBUG','10'),('INFO','20'),('WARN','30'),('ERROR','40'),('FATAL','50')])
  self.assertNotIn('DEBUG',[m.name for m in log.members])
 def test_all_fieldtype_constants_preserved(self):
  records,_,_,_=self.rows();v=records[(2,'type_description_interfaces/msg/FieldType')]
  self.assertEqual(len(v.constants),88)
  self.assertEqual(next(c['value'] for c in v.constants if c['name']=='FIELD_TYPE_BYTE'),'16')
  self.assertEqual(next(c['value'] for c in v.constants if c['name']=='FIELD_TYPE_UINT8'),'3')
 def test_complete_inactive_parameter_value_fields(self):
  records,_,_,_=self.rows();v=records[(2,'rcl_interfaces/msg/ParameterValue')]
  self.assertEqual([m.name for m in v.members],['type','bool_value','integer_value','double_value','string_value','byte_array_value','bool_array_value','integer_array_value','double_array_value','string_array_value'])
 def test_legal_type_stays_exact(self):
  bounded.control_member_name(parent,'type')
  with self.assertRaises(parent.GenerationError):parent.field_name('type',2,bounded=True)
 def test_c_cpp_keyword_still_refused(self):
  for name in ['class','switch','co_await','unsigned']:
   with self.assertRaises(parent.GenerationError):bounded.control_member_name(parent,name)
 def test_actual_byte_and_char_are_distinct_ir(self):
  records,_,_,_=self.rows()
  byte=next(m for m in records[(2,'rcl_interfaces/msg/ParameterValue')].members if m.name=='byte_array_value')
  char=next(m for m in records[(2,'service_msgs/msg/ServiceEventInfo')].members if m.name=='client_gid')
  self.assertEqual((byte.type,byte.native_scalar),('uint8','byte'))
  self.assertEqual((char.type,char.native_scalar,char.count),('uint8','char',16))
 def test_declared_false_defaults_retained(self):
  records,_,_,_=self.rows();v=records[(2,'rcl_interfaces/msg/ParameterDescriptor')]
  self.assertEqual({m.name:m.native_default for m in v.members if m.native_default is not None},{'read_only':'False','dynamic_typing':'False'})
 def test_declared_true_default_retained(self):
  records,_,_,_=self.rows();v=records[(2,'type_description_interfaces/srv/GetTypeDescription_Request')]
  self.assertEqual(next(m.native_default for m in v.members if m.name=='include_type_sources'),'True')
 def test_declared_zero_default_retained(self):
  records,_,_,_=self.rows();v=records[(2,'type_description_interfaces/msg/FieldType')]
  self.assertEqual(next(m.native_default for m in v.members if m.name=='type_id'),'0')
 def test_constant_only_record_has_zero_logical_wire(self):
  records,_,_,_=self.rows();v=records[(2,'rcl_interfaces/msg/ParameterType')]
  self.assertEqual((v.minimum,v.maximum,len(v.members),len(v.constants)),(0,0,0,10))
 def test_every_record_retains_canonical_ceiling(self):
  _,ordered,_,_=self.rows()
  self.assertTrue(all(r.maximum<=32768 and r.nodes<=4096 and r.depth<=16 for r in ordered))
 def test_old_profile_still_refuses_constant(self):
  with self.assertRaises(parent.GenerationError):bounded.parse_fields(parent,'uint8 VALUE=1',2,'vendor',{},set())
 def test_old_profile_still_refuses_alias(self):
  with self.assertRaises(parent.GenerationError):bounded.parse_fields(parent,'byte[] value',2,'vendor',{'value':{'sequence':2}},set())
 def test_control_does_not_register_unselected_logger(self):
  self.assertNotIn('rcl_interfaces/srv/GetLoggerLevels',bounded.control_registered_names())
 def test_control_rejects_unknown_reserved_type(self):
  with self.assertRaises(parent.GenerationError):bounded.control_native_name(parent,2,'rcl_interfaces/msg/Invented','message')
 def test_control_rejects_legacy_version(self):
  with self.assertRaises(parent.GenerationError):bounded.control_native_name(parent,1,'rcl_interfaces/Log','message')
  for version in (2.0,True,'2',None):
   with self.assertRaises(parent.GenerationError):bounded.control_native_name(parent,version,'rcl_interfaces/msg/Log','message')
  for name in (None,[],{}):
   with self.assertRaises(parent.GenerationError):bounded.control_native_name(parent,2,name,'message')
 def test_actual_c_constant_output(self):
  records,ordered,_,_=self.rows();body=bounded.emit_c(parent,records,ordered)
  self.assertIn('#define ROSWASMC_BOUNDED_2_RCL_INTERFACES_MSG_LOG_DEBUG UINT32_C(10)',body)
  self.assertIn('uint8_t type;',body)
 def test_control_emits_only_implemented_languages(self):
  with tempfile.TemporaryDirectory(dir=SCRATCH) as root:
   out=Path(root)/'new';proof=bounded.generate(parent,CATALOG,out,True)
   self.assertEqual(set(proof['implemented_record_outputs']),{'c','cpp'})
   self.assertFalse(proof['native_registry_emitted'])
   self.assertEqual(set(p.name for p in out.iterdir()),{'bounded_records.h','bounded_records.hpp','bounded_interfaces.h','bounded_interfaces.hpp','control-schema.json','generation.json','control_wire.hpp','control_manifest.hpp'})
 def test_control_native_emission_refuses(self):
  records,ordered,interfaces,pins=self.rows()
  with tempfile.TemporaryDirectory(dir=SCRATCH) as root:
   out=Path(root)/'none'
   with self.assertRaises(parent.GenerationError):bounded.generate_control(parent,CATALOG,out,records,ordered,interfaces,pins,False)
   self.assertFalse(out.exists())

def main():
 parser=argparse.ArgumentParser(description=__doc__)
 parser.add_argument('--sdk',required=True,type=Path);parser.add_argument('--parent-sha256',required=True);parser.add_argument('--bounded-sha256',required=True);parser.add_argument('--scratch',required=True,type=Path);args=parser.parse_args()
 global ROOT,SCRATCH,parent,bounded,CATALOG
 ROOT=args.sdk.resolve(strict=True);SCRATCH=args.scratch.resolve(strict=True)
 if SCRATCH==ROOT or ROOT in SCRATCH.parents:parser.error('scratch must be outside the selected SDK')
 try:
  parent=_captured_module('control_parent',ROOT/'sdk/tools/custom_types.py',args.parent_sha256)
  bounded=_captured_module('control_bounded',ROOT/'sdk/custom-types/bounded_profile.py',args.bounded_sha256)
  parent._remember_source(parent);parent._remember_source(bounded)
 except(OSError,ValueError)as error:parser.error(str(error))
 CATALOG=ROOT/'sdk/rmw_wasm/default_control/catalog.preparation.json'
 result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(SourceProfile))
 return 0 if result.wasSuccessful()else 1
if __name__=='__main__':raise SystemExit(main())
