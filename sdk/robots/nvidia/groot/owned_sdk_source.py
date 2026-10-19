"""Cold trusted native source admission; no SDK or model import at module load.

Execute only the captured exact gr00t Python sources, never an ambient .pyc.
The explicit file/SHA inventory must come from the independently selected fixed
SDK source package. This is source admission, not a signature or model safety
proof. Root must freeze the native dependency environment and model tree for the
entire process; external packages and GPU kernels need separate qualification.
"""
import hashlib
import importlib.abc
import importlib.util
import json
import os
from pathlib import Path
import stat
import sys


def read_regular(path, maximum):
    flags = os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC
    fd = os.open(path, flags)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or before.st_size > maximum:
            raise ValueError('bounded ordinary source required')
        data = bytearray()
        while True:
            chunk = os.read(fd,min(1<<20,maximum-len(data)+1))
            if not chunk: break
            data.extend(chunk)
            if len(data)>maximum: raise ValueError('source grew past quota')
        after=os.fstat(fd); named=os.lstat(path)
        signature=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns,s.st_mode,s.st_nlink)
        if len(data)!=before.st_size or signature(before)!=signature(after) or signature(after)!=signature(named):
            raise ValueError('source/name changed during capture')
        return bytes(data)
    finally: os.close(fd)


def capture(root, inventory_path):
    root=Path(root).resolve(strict=True)
    inventory_raw=read_regular(inventory_path,1<<20)
    if hashlib.sha256(inventory_raw).hexdigest()!='f14cb630f2f6713b13c1bf8442563c02685c13bca780702bedcc5fa83b55b81a':raise ValueError('independently fixed SDK source inventory SHA differs')
    inventory=json.loads(inventory_raw)
    if type(inventory) is not dict or set(inventory)!={'format','commit','files'} or inventory['format']!='uwvm-groot-python-source-v1':
        raise ValueError('explicit independently selected SDK inventory required')
    if inventory['commit']!='dac54b313160c201c450f97df0fd74ebd379c892':
        raise ValueError('this adapter requires its reviewed original SDK commit')
    rows=inventory['files'];captured={};total=0
    if type(rows) is not dict or not 1<=len(rows)<=1024: raise ValueError('source inventory extent')
    actual=set(p.relative_to(root).as_posix() for p in (root/'gr00t').rglob('*.py'))
    if actual!=set(rows):raise ValueError('all original gr00t Python files must be independently listed')
    for name,expected in rows.items():
        relative=Path(name)
        if relative.is_absolute() or '..' in relative.parts or not name.startswith('gr00t/') or not name.endswith('.py'):
            raise ValueError('invalid source name')
        path=root/relative
        if path.parent.resolve(strict=True)!=root/relative.parent:raise ValueError('source ancestor symlink')
        raw=read_regular(path,2<<20);total+=len(raw)
        if total>32<<20 or hashlib.sha256(raw).hexdigest()!=expected:raise ValueError('actual SDK source identity mismatch')
        module=name[:-3].replace('/','.')
        package=module.endswith('.__init__')
        if package:module=module[:-9]
        captured[module]=(str(path),raw,package)
    if 'gr00t' not in captured:raise ValueError('actual package initializer missing')
    return captured


class CapturedSDK(importlib.abc.MetaPathFinder,importlib.abc.Loader):
    def __init__(self,captured):
        self.captured=captured
        # The fixed SDK contains genuine implicit namespace parents. Derive
        # their finite names only from admitted source keys, never sys.path.
        self.namespaces=set()
        for name in captured:
            parent=name.rpartition('.')[0]
            while parent:
                if parent not in captured:self.namespaces.add(parent)
                parent=parent.rpartition('.')[0]
    def find_spec(self,fullname,path=None,target=None):
        if fullname=='gr00t' or fullname.startswith('gr00t.'):
            if fullname in self.namespaces:
                return importlib.util.spec_from_loader(fullname,self,is_package=True)
            if fullname not in self.captured:raise ImportError('unlisted gr00t source refused: '+fullname)
            return importlib.util.spec_from_loader(fullname,self,is_package=self.captured[fullname][2])
        return None
    def create_module(self,spec):return None
    def exec_module(self,module):
        if module.__name__ in self.namespaces:
            # An empty search path prevents PathFinder/ambient/PYC fallback.
            # Descendants are resolved solely by this exact captured finder.
            module.__path__=[]
            return
        filename,raw,package=self.captured[module.__name__]
        module.__file__=filename
        if package:module.__path__=[str(Path(filename).parent)]
        # Import machinery registered module before exec. No SourceFileLoader,
        # timestamp cache, write-bytecode switch, or executable producer involved.
        exec(compile(raw,filename,'exec',dont_inherit=True),module.__dict__)


def install(captured):
    if any(n=='gr00t' or n.startswith('gr00t.') for n in sys.modules):raise ValueError('SDK already imported before admission')
    loader=CapturedSDK(captured);sys.meta_path.insert(0,loader);return loader
