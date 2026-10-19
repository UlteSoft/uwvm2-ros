"""Authored future parser/emitter contracts. NOT executed in a source HOLD.

Root must pass the actual before and after bounded modules explicitly. Temporary
catalogs live in --scratch outside --sdk. No generator CLI is invoked; tests call
actual reviewed APIs only when Root separately authorizes this future execution.
"""
import argparse
import copy
import json
import hashlib
import os
import re
import stat
import types
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
sys.dont_write_bytecode = True

MAX_SOURCE_BYTES = 1024 * 1024

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

def module(name,path,expected_sha256):
    return _captured_module(name,path,expected_sha256)

class DomainPolicy(unittest.TestCase):
    def catalog(self,change=None):
        body=copy.deepcopy(CATALOG)
        if change:change(body)
        path=WORK/('catalog-'+str(self.id()).split('.')[-1]+'.json');path.write_text(json.dumps(body))
        return path
    def load(self,change=None):return after.load_catalog(parent,self.catalog(change),False)
    def test_exact_raw_domain(self):
        records,_,_,_=self.load();r=records[(2,'type_description_interfaces/msg/TypeSource')]
        self.assertEqual([(m.name,m.type,m.string_encoding) for m in r.members],[('type_name','string','utf8'),('encoding','string','utf8'),('raw_file_contents','string','raw_bytes')])
        self.assertEqual(r.members[2].string_bound,6162)
    def test_raw_domain_cannot_escape_field(self):
        for field in ('type_name','encoding'):
            def wrong(c):next(x for x in c['types'] if x['type'].endswith('/TypeSource'))['bounds'][field]['encoding']='raw_bytes'
            with self.assertRaises(parent.GenerationError):self.load(wrong)
    def test_raw_domain_requires_explicit_token(self):
        for token in ('utf8','bytes',True,None,[]):
            def wrong(c):next(x for x in c['types'] if x['type'].endswith('/TypeSource'))['bounds']['raw_file_contents']['encoding']=token
            with self.assertRaises(parent.GenerationError):self.load(wrong)
    def test_legacy_encoding_is_rejected(self):
        for standard in (False,True):
            with self.assertRaises(parent.GenerationError):after.parse_fields(parent,'string value',2,'vendor',{'value':{'string':4,'encoding':'raw_bytes'}},set(),standard=standard)
    def test_quota_values_are_strict_integers(self):
        for value in (True,32768.0,'32768',0,32769,None):
            def wrong(c):next(x for x in c['types'] if x['type'].endswith('/GetTypeDescription'))['wire_limits']={'response':value}
            with self.assertRaises(parent.GenerationError):self.load(wrong)
    def test_quota_roles_and_minimum(self):
        for limit in ({},{'message':32768},{'response':1},{'response':32768,'invented':1}):
            def wrong(c):next(x for x in c['types'] if x['type'].endswith('/GetTypeDescription'))['wire_limits']=limit
            with self.assertRaises(parent.GenerationError):self.load(wrong)
    def test_full_cartesian_and_admitted_extent(self):
        records,ordered,_,_=self.load();r=records[(2,'type_description_interfaces/srv/GetTypeDescription_Response')]
        self.assertEqual(r.maximum,32768);self.assertEqual(r.aggregate_wire_bytes,32768);self.assertGreater(r.cartesian_maximum,32768)
        self.assertTrue(all(x.nodes<=4096 and x.depth<=16 and x.maximum<=32768 for x in ordered));self.assertLessEqual(sum(x.nodes for x in ordered),16384)
        report=after.control_summary(ordered,[]);self.assertIn('NOT_WIRE_QUOTA',report['inline_storage_size_status'])
    def test_quota_and_raw_change_canonical_identity_only(self):
        old_records,_,_,_=before.load_catalog(parent,OLD_CONTROL,False);records,_,_,_=self.load()
        raw=(2,'type_description_interfaces/msg/TypeSource');self.assertNotEqual(old_records[raw].codec_identity,records[raw].codec_identity)
        request=(2,'type_description_interfaces/srv/GetTypeDescription_Request');self.assertEqual(old_records[request].codec_identity,records[request].codec_identity)
        self.assertEqual([(m.name,m.native_default) for m in old_records[request].members],[(m.name,m.native_default) for m in records[request].members])
    def test_existing_catalog_emissions_exact(self):
        # Actual source-owned legacy catalogs exercise old bounded, official
        # standard, scalar defaults, and original control with no new opt-in.
        for path in LEGACY:
            a,ao,ai,_=before.load_catalog(parent,path,False);b,bo,bi,_=after.load_catalog(parent,path,False)
            self.assertEqual([(r.name,r.codec_identity,r.minimum,r.maximum) for r in ao],[(r.name,r.codec_identity,r.minimum,r.maximum) for r in bo])
            self.assertEqual(before.emit_c(parent,a,ao),after.emit_c(parent,b,bo));self.assertEqual(before.emit_cpp(ao),after.emit_cpp(bo))
            self.assertEqual(before.emit_c_interfaces(ai),after.emit_c_interfaces(bi));self.assertEqual(before.emit_cpp_interfaces(ai),after.emit_cpp_interfaces(bi))
            if ao[0].profile!=before.CONTROL_PROFILE:
                for name in ('emit_js','emit_rust','emit_go','emit_as'):
                    self.assertEqual(getattr(before,name)(parent,a,ao),getattr(after,name)(parent,b,bo))
    def test_old_control_complete_outputs_and_schema_exact(self):
        left=WORK/'old-control-before-output';right=WORK/'old-control-after-output'
        before.generate(parent,OLD_CONTROL,left,True);after.generate(parent,OLD_CONTROL,right,True)
        self.assertEqual(sorted(x.name for x in left.iterdir()),sorted(x.name for x in right.iterdir()))
        for path in left.iterdir():
            if path.name=='generation.json':
                a=json.loads(path.read_text());b=json.loads((right/path.name).read_text())
                # The actual generator source changed; preserve honest provenance.
                a.pop('bounded_generator_sha256');b.pop('bounded_generator_sha256')
                self.assertEqual(a,b)
            else:self.assertEqual(path.read_bytes(),(right/path.name).read_bytes())
    def test_new_grammar_and_transactional_measure_source(self):
        records,ordered,_,_=self.load();body=after.emit_c(parent,records,ordered);grammar=after.emit_control_wire(parent,records,ordered)
        self.assertIn('_measure(',body);self.assertIn('_CARTESIAN_MAX_WIRE_BYTES UINT64_C(',body)
        self.assertIn('custom_wire_kind::raw_bytes',grammar);self.assertIn('custom_wire_kind::bounded_record,32768',grammar)
        raw=records[(2,'type_description_interfaces/msg/TypeSource')];self.assertEqual(raw.members[2].type,'string')

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--sdk',required=True,type=Path);parser.add_argument('--before',required=True,type=Path);parser.add_argument('--after',required=True,type=Path);parser.add_argument('--old-control',required=True,type=Path);parser.add_argument('--new-control',required=True,type=Path);parser.add_argument('--scratch',required=True,type=Path);parser.add_argument('--parent-sha256',required=True);parser.add_argument('--before-sha256',required=True);parser.add_argument('--after-sha256',required=True);args=parser.parse_args()
    global parent,before,after,CATALOG,WORK,OLD_CONTROL,LEGACY
    sdk=args.sdk.resolve(strict=True);scratch=args.scratch.resolve(strict=True)
    if scratch==sdk or sdk in scratch.parents:parser.error('scratch must be private and outside the SDK source tree')
    try:
        parent=module('source_domain_parent',sdk/'sdk/tools/custom_types.py',args.parent_sha256)
        before=module('source_domain_before',args.before,args.before_sha256)
        after=module('source_domain_after',args.after,args.after_sha256)
        # Future provenance reads must still match ALL three captured sources.
        parent._remember_source(parent);parent._remember_source(before);parent._remember_source(after)
    except (OSError,ValueError) as error:
        parser.error(str(error))
    CATALOG=json.loads(args.new_control.read_text());LEGACY=[sdk/p for p in ('tests/ros_custom_types/bounded-v2/ros1/catalog.preparation.json','tests/ros_custom_types/bounded-v2/ros2/catalog.preparation.json','tests/ros_custom_types/official-standard/ros2/catalog.preparation.json','tests/ros_custom_types/quaternion-defaults/fixture/catalog.preparation.json')]
    with tempfile.TemporaryDirectory(prefix='control-source-contract-',dir=scratch) as directory:
        WORK=Path(directory);shutil.copytree(sdk/'sdk/rmw_wasm/default_control/original',WORK/'original')
        OLD_CONTROL=WORK/'old-control.json';OLD_CONTROL.write_bytes(args.old_control.read_bytes());LEGACY.append(OLD_CONTROL)
        result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(DomainPolicy));return 0 if result.wasSuccessful() else 1
if __name__=='__main__':raise SystemExit(main())
