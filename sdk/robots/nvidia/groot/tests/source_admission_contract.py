"""Authored UNRUN exact-source admission regressions; no model/ROS SDK import.

Example: python3 source_admission_contract.py --adapter-root ABS_NVIDIA_ROOT
--helper-sha256 ACTUAL_REVIEWED_SHA --sdk-root FIXED_SDK_ROOT
--inventory ABS_FIXED_INVENTORY. All paths/digests are explicit caller inputs.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types
import unittest

ADAPTER=None
SDK=None
INVENTORY=None
HELPER=None

def captured(path,expected):
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
    try:
        first=os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0<=first.st_size<=131072:raise ValueError('bounded ordinary helper required')
        raw=bytearray()
        while True:
            part=os.read(fd,min(16384,131073-len(raw)))
            if not part:break
            raw.extend(part)
            if len(raw)>131072:raise ValueError('helper growth')
        last=os.fstat(fd);named=os.lstat(path)
        key=lambda v:(v.st_dev,v.st_ino,v.st_mode,v.st_size,v.st_mtime_ns,v.st_ctime_ns,v.st_nlink)
        if len(raw)!=first.st_size or key(first)!=key(last) or key(last)!=key(named):raise ValueError('helper identity drift')
        if hashlib.sha256(raw).hexdigest()!=expected:raise ValueError('explicit helper SHA differs')
        return bytes(raw)
    finally:os.close(fd)

class Contracts(unittest.TestCase):
    def test_actual_fixed_sdk_namespace_parents(self):
        values=HELPER.capture(SDK,INVENTORY);finder=HELPER.CapturedSDK(values)
        for name in ('gr00t.model.base','gr00t.utils','gr00t.data.state_action'):
            self.assertNotIn(name,values)
            self.assertTrue(any(k.startswith(name+'.') for k in values))
            spec=finder.find_spec(name);self.assertIsNotNone(spec.submodule_search_locations)
            module=types.ModuleType(name);module.__spec__=spec;finder.exec_module(module)
            self.assertEqual(module.__path__,[])
            self.assertNotIn('__file__',module.__dict__)
        with self.assertRaises(ImportError):finder.find_spec('gr00t.model.base.unlisted_neighbor')
    def test_dependency_roots_not_topic_registrations(self):
        for family in ('isaac_ros','isaac_sim'):
            rows=json.loads((ADAPTER/family/'source-definitions/ros2/catalog.preparation.json').read_bytes())['types']
            actual={r['type']:r for r in rows}
            for name in ('builtin_interfaces/msg/Time','std_msgs/msg/Header'):
                self.assertIs(actual[name]['registered'],False)
                self.assertTrue((ADAPTER/family/'source-definitions/ros2'/actual[name]['file']).is_file())
    def test_callback_error_cannot_be_overwritten_by_successful_poll(self):
        for family in ('isaac_ros','isaac_sim'):
            source=(ADAPTER/family/'examples/telemetry.cpp').read_text()
            self.assertIn('if(!primary&&n!=11)primary=n;',source)
            self.assertNotIn('if(n!=11)primary=n;',source)

if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('--adapter-root',type=Path,required=True);ap.add_argument('--helper-sha256',required=True);ap.add_argument('--sdk-root',type=Path,required=True);ap.add_argument('--inventory',type=Path,required=True)
    args,rest=ap.parse_known_args();ADAPTER=args.adapter_root;SDK=args.sdk_root;INVENTORY=args.inventory
    path=ADAPTER/'groot/owned_sdk_source.py';raw=captured(path,args.helper_sha256)
    HELPER=types.ModuleType('_reviewed_nvidia_source_helper');HELPER.__file__=str(path);sys.modules[HELPER.__name__]=HELPER
    try:exec(compile(raw,str(path),'exec',dont_inherit=True),HELPER.__dict__)
    except BaseException:
        if sys.modules.get(HELPER.__name__) is HELPER:del sys.modules[HELPER.__name__]
        raise
    unittest.main(argv=[sys.argv[0]]+rest)
