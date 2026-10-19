"""Authored future source-identity contracts; never run during a source HOLD.

The trusted parent is bootstrapped from captured SHA-paired source bytes only.
Every actual assertion below exercises that loaded parent's production helpers.
Synthetic Python files/cache traps are private loader fixtures, not ROS stubs.
Run under Root's separately admitted finite process owner, outside the SDK.
"""
import argparse
import hashlib
import importlib.util
import json
import marshal
import os
from pathlib import Path
import re
import stat
import struct
import sys
import tempfile
import types
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


def _scratch_is_outside_sdk(selected, scratch):
    # Resolve only the containing SDK directory for isolation. Keep the source
    # leaf lexical so _captured_module still enforces its genuine O_NOFOLLOW.
    sdk = selected.parent.parent.resolve(strict=True)
    return scratch != sdk and sdk not in scratch.parents

class SourceIdentity(unittest.TestCase):
    def setUp(self):
        self.temporary=tempfile.TemporaryDirectory(prefix='source-identity-case-',dir=SCRATCH)
        self.addCleanup(self.temporary.cleanup)
        self.root=Path(self.temporary.name)
        self.name='_uwvm_source_identity_'+self.id().split('.')[-1]
        missing=object();old=sys.modules.get(self.name,missing)
        def restore():
            if old is missing:sys.modules.pop(self.name,None)
            else:sys.modules[self.name]=old
        self.addCleanup(restore)
    def source(self,body='value=7\n'):
        path=self.root/'input.py';path.write_text(body,encoding='utf-8')
        return path,hashlib.sha256(path.read_bytes()).hexdigest()
    def test_matching_timestamp_cache_is_not_executed(self):
        path,digest=self.source('value=7\n');s=path.stat()
        cache=Path(importlib.util.cache_from_source(str(path)));cache.parent.mkdir()
        poison=compile('raise RuntimeError("CACHED_CODE_MUST_NOT_RUN")',str(path),'exec',dont_inherit=True)
        cache.write_bytes(importlib.util.MAGIC_NUMBER+struct.pack('<III',0,int(s.st_mtime)&0xffffffff,s.st_size)+marshal.dumps(poison))
        previous=cache.read_bytes();value=PARENT._captured_module(self.name,path,digest)
        self.assertEqual(value.value,7);self.assertEqual(cache.read_bytes(),previous)
        self.assertEqual(value._uwvm_source_sha256,digest);self.assertIsNone(value.__loader__);self.assertIsNone(value.__cached__)
    def test_real_dataclass_future_annotations_have_module(self):
        path,digest=self.source('from __future__ import annotations\nfrom dataclasses import dataclass,fields\nfrom typing import ClassVar\n@dataclass\nclass Value:\n label:ClassVar[str]="retained"\n count:int=7\n')
        value=PARENT._captured_module(self.name,path,digest)
        self.assertIs(sys.modules[self.name],value);self.assertEqual(value.Value().count,7)
        self.assertEqual([f.name for f in value.fields(value.Value)],['count'])
        self.assertFalse((self.root/'__pycache__').exists())
    def test_wrong_sha_refuses_before_exec_and_binding_change(self):
        marker=self.root/'executed';path,digest=self.source('from pathlib import Path\nPath('+repr(str(marker))+').write_text("bad")\n')
        previous=types.ModuleType(self.name);sys.modules[self.name]=previous
        wrong=('1'if digest[0]!='1'else'2')+digest[1:]
        with self.assertRaises(ValueError):PARENT._captured_module(self.name,path,wrong)
        self.assertFalse(marker.exists());self.assertIs(sys.modules[self.name],previous)
    def test_symlink_refuses_before_exec(self):
        path,digest=self.source();link=self.root/'link.py';link.symlink_to(path)
        with self.assertRaises(OSError):PARENT._captured_module(self.name,link,digest)
        self.assertNotIn(self.name,sys.modules)
    def test_fifo_is_opened_nonblocking_and_refused(self):
        path=self.root/'input.py';os.mkfifo(path)
        with self.assertRaises(ValueError):PARENT._captured_module(self.name,path,'0'*64)
        self.assertNotIn(self.name,sys.modules)
    def test_post_exec_source_drift_restores_previous_module(self):
        path,digest=self.source('from pathlib import Path\nPath(__file__).write_text("changed")\n')
        previous=types.ModuleType(self.name);sys.modules[self.name]=previous
        with self.assertRaises(ValueError):PARENT._captured_module(self.name,path,digest)
        self.assertIs(sys.modules[self.name],previous)
    def test_exception_or_rebinding_restores_previous_module(self):
        for body in ('raise RuntimeError("primary")\n','import sys,types\nsys.modules[__name__]=types.ModuleType(__name__)\n'):
            path,digest=self.source(body);previous=types.ModuleType(self.name);sys.modules[self.name]=previous
            with self.assertRaises((RuntimeError,ValueError)):PARENT._captured_module(self.name,path,digest)
            self.assertIs(sys.modules[self.name],previous)
    def test_later_provenance_requires_exact_captured_source(self):
        path,digest=self.source();value=PARENT._captured_module(self.name,path,digest);PARENT._remember_source(value)
        original=path.read_bytes();self.assertEqual(PARENT.bounded_read(path,PARENT.MAX_SOURCE_BYTES),original)
        previous=path.stat();path.write_bytes(b'value=8\n');os.utime(path,ns=(previous.st_atime_ns,previous.st_mtime_ns))
        with self.assertRaises(PARENT.GenerationError):PARENT.bounded_read(path,PARENT.MAX_SOURCE_BYTES)
        with self.assertRaises(ValueError):PARENT._captured_module(self.name,path,digest,value._uwvm_source_identity)
    def test_sdk_ancestor_alias_rejects_actual_internal_scratch(self):
        # This entire synthetic tree is inside the already admitted private
        # outer scratch. No fixture is created in the selected actual SDK.
        physical=self.root/'private'/'tmp'/'checkout'/'sdk'
        (physical/'tools').mkdir(parents=True)
        selected=physical/'tools'/'custom_types.py';selected.write_text('value=7\n')
        inside=physical/'fixtures';inside.mkdir();inside=inside.resolve(strict=True)
        # A direct SDK directory alias and a /tmp -> /private/tmp-shaped
        # ancestor alias both resolve to the SAME genuine containing SDK.
        sdk_link=self.root/'sdk-link';sdk_link.symlink_to(physical,target_is_directory=True)
        tmp_link=self.root/'tmp';tmp_link.symlink_to(self.root/'private'/'tmp',target_is_directory=True)
        for aliased in (sdk_link/'tools'/'custom_types.py',tmp_link/'checkout'/'sdk'/'tools'/'custom_types.py'):
            self.assertNotEqual(aliased.parent.parent,physical.resolve(strict=True))
            self.assertFalse(_scratch_is_outside_sdk(aliased,inside))
            self.assertFalse(_scratch_is_outside_sdk(aliased,physical.resolve(strict=True)))
            self.assertTrue(_scratch_is_outside_sdk(aliased,self.root.resolve(strict=True)))
        # Also cover the actual host's /tmp alias when this owned fixture is
        # under that physical directory (e.g. macOS /tmp -> /private/tmp).
        host_tmp=Path('/tmp')
        if host_tmp.is_dir():
            real_tmp=host_tmp.resolve(strict=True)
            if real_tmp!=host_tmp and selected.is_relative_to(real_tmp):
                actual_alias=host_tmp/selected.relative_to(real_tmp)
                self.assertFalse(_scratch_is_outside_sdk(actual_alias,inside))

    def test_catalog_cannot_select_an_arbitrary_module(self):
        sdk=self.root/'sdk';tools=sdk/'tools';bounded=sdk/'custom-types';tools.mkdir(parents=True);bounded.mkdir()
        installed=tools/'custom_types.py';installed.write_bytes(PARENT_BYTES)
        fixed=bounded/'bounded_profile.py';fixed.write_text('value="fixed-reviewed-sibling"\n')
        marker=self.root/'evil-executed';evil=self.root/'evil.py';evil.write_text('from pathlib import Path\nPath('+repr(str(marker))+').write_text("bad")\n')
        catalog=self.root/'catalog.json';catalog.write_text(json.dumps({'format':'uwvm-ros-custom-bounded-v2','provider':str(evil),'module':str(evil)}))
        selected=PARENT._captured_module(self.name,installed,PARENT_SHA)
        fixed_name='_uwvm_ros_custom_bounded_profile_v2';missing=object();previous=sys.modules.get(fixed_name,missing)
        def restore_fixed():
            if previous is missing:sys.modules.pop(fixed_name,None)
            else:sys.modules[fixed_name]=previous
        self.addCleanup(restore_fixed)
        loaded=selected.bounded_profile(catalog)
        self.assertEqual(loaded.value,'fixed-reviewed-sibling');self.assertEqual(Path(loaded.__file__),fixed)
        self.assertFalse(marker.exists())

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--parent',required=True,type=Path);parser.add_argument('--parent-sha256',required=True);parser.add_argument('--scratch',required=True,type=Path);args=parser.parse_args()
    global PARENT,PARENT_BYTES,PARENT_SHA,SCRATCH
    SCRATCH=args.scratch.resolve(strict=True);selected=Path(os.path.abspath(args.parent))
    # Do not permit private fixture writes in the actual selected SDK tree.
    if not _scratch_is_outside_sdk(selected,SCRATCH):parser.error('scratch must be outside the selected actual SDK')
    try:
        PARENT=_captured_module('_source_identity_actual_parent',selected,args.parent_sha256)
        PARENT._remember_source(PARENT)
        PARENT_BYTES=PARENT.bounded_read(selected,PARENT.MAX_SOURCE_BYTES);PARENT_SHA=args.parent_sha256
    except (OSError,ValueError)as error:parser.error(str(error))
    result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(SourceIdentity))
    return 0 if result.wasSuccessful()else 1
if __name__=='__main__':raise SystemExit(main())
