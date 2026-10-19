"""Actual source-bound native entry. No ambient adapter or SDK PYC admission.

Run this entry, not an arbitrary import of native_owner. The normal Python
interpreter reads this __main__ script; fixed sibling source names and SHA are
captured below and executed exactly after sys.modules registration. Root still
owns/freeze-qualifies the Python/native dependency environment and model tree.
"""
import hashlib
import os
from pathlib import Path
import stat
import sys
import types
EXPECTED = {'owned_sdk_source': '2514e50fa51930a7aace90616848ed60d1001c6160d269dd86eb3a0ab4e56d91', 'native_conversion': 'f1d4f21150a74f681d751396f5de8949d2c36f7af59d8459803b98abc2067656', 'native_owner': '105bbedc5a75eb9d9df6e9d0d6f80690312a8a82ede467cecdb14322693610d0'}


def read(path):
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
    try:
        first=os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0<=first.st_size<=131072:raise ValueError('bounded adapter source required')
        raw=bytearray()
        while True:
            b=os.read(fd,min(16384,131073-len(raw)))
            if not b:break
            raw.extend(b)
            if len(raw)>131072:raise ValueError('adapter grew')
        last=os.fstat(fd);named=os.lstat(path)
        key=lambda s:(s.st_dev,s.st_ino,s.st_mode,s.st_size,s.st_mtime_ns,s.st_ctime_ns,s.st_nlink)
        if len(raw)!=first.st_size or key(first)!=key(last) or key(last)!=key(named):raise ValueError('adapter source/name drift')
        return bytes(raw)
    finally:os.close(fd)


def run():
    root=Path(__file__).absolute().parent.resolve(strict=True)
    captured={}
    for name,digest in EXPECTED.items():
        if name in sys.modules:raise ValueError('adapter already imported')
        path=root/(name+'.py');raw=read(path)
        if hashlib.sha256(raw).hexdigest()!=digest:raise ValueError('adapter Source SHA differs: '+name)
        captured[name]=(path,raw)
    for name,(path,raw) in captured.items():
        module=types.ModuleType(name);module.__file__=str(path);sys.modules[name]=module
        try:exec(compile(raw,str(path),'exec',dont_inherit=True),module.__dict__)
        except BaseException:
            if sys.modules.get(name) is module:del sys.modules[name]
            raise
    return sys.modules['native_owner'].main()


if __name__=='__main__':raise SystemExit(run())
