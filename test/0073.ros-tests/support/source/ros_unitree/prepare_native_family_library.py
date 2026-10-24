#!/usr/bin/env python3
"""A2: independently qualify/copy the genuine terminal native GO24 family.

Root supplies a real current64-GiB/zero-swap closed Frame config, original
terminal report and all original23 roots, execution SDK and complete GO family. Success is written only by normal
Frame.finish after preservation and cleanup. A new inode is not a new compile.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys
import traceback
import time
import types

sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[5]
MIB=1024**2
CG_LEAF='/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'


def need(value,reason):
    if not value:raise ValueError(reason)


def bounded(path,maximum):
    path=Path(path);need(path.is_absolute() and path.resolve(strict=True)==path,'physical metadata required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
    try:
        before=os.fstat(fd);need(stat.S_ISREG(before.st_mode) and 0<=before.st_size<=maximum,'bounded ordinary metadata required')
        body=bytearray()
        while True:
            part=os.read(fd,min(65536,maximum-len(body)+1))
            if not part:break
            body.extend(part);need(len(body)<=maximum,'metadata grew')
        need(len(body)==before.st_size and all(getattr(before,k)==getattr(os.fstat(fd),k)==getattr(path.lstat(),k)
             for k in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),'opened metadata changed')
        return bytes(body)
    finally:os.close(fd)


def decode(raw):
    def pairs(rows):
        result={}
        for key,value in rows:need(key not in result,'duplicate library metadata key');result[key]=value
        return result
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite metadata'))


def frozen_module(source,frozen,relative,name):
    path=source/relative;raw=bounded(path,2*MIB)
    need(frozen.get(relative)=={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()},'helper absent from current complete source')
    value=types.ModuleType(name);value.__file__=str(path);sys.modules[name]=value
    exec(compile(raw,str(path),'exec'),value.__dict__);return value


def bootstrap(args,own='test/0073.ros-tests/support/source/ros_unitree/prepare_native_family_library.py'):
    need(sys.platform=='linux' and os.geteuid()==0 and args.source.resolve(strict=True)==ROOT and
         Path(__file__).absolute()==ROOT/own,'actual frozen Linux Root entry required')
    for key in ('source','manifest','config','out'):need(getattr(args,key).is_absolute(),'absolute actual path required')
    for key in ('manifest_sha256','config_sha256'):need(re.fullmatch('[0-9a-f]{64}',getattr(args,key)),'complete actual SHA required')
    raw=bounded(args.manifest,16*MIB);need(hashlib.sha256(raw).hexdigest()==args.manifest_sha256,'current source MF changed')
    frozen=decode(raw);body=bounded(ROOT/own,MIB)
    need(type(frozen) is dict and frozen.get(own)=={'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()},'actual entry absent from MF')
    raw=bounded(args.config,16*MIB);need(hashlib.sha256(raw).hexdigest()==args.config_sha256,'actual config changed')
    cg=decode(raw)['cgroup'];keys={'memory_max','memory_swap_max','expected_leaf','expected_device','expected_inode','expected_boot_id','expected_cpus'}
    need(type(cg) is dict and set(cg)==keys and type(cg['memory_max']) is int and cg['memory_max']==64*1024**3 and
         type(cg['memory_swap_max']) is int and cg['memory_swap_max']==0 and cg['expected_leaf']==CG_LEAF and
         cg['expected_cpus']=='0-31','unchanged original64/swap0/CPU leaf required')
    leaf=Path(CG_LEAF);info=leaf.stat()
    need(leaf.resolve(strict=True)==leaf and (info.st_dev,info.st_ino)==(cg['expected_device'],cg['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip()==cg['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines()==['0::'+CG_LEAF.removeprefix('/sys/fs/cgroup')] and
         (leaf/'memory.max').read_text().strip()==str(64*1024**3) and (leaf/'memory.swap.max').read_text().strip()=='0' and
         (leaf/'cpuset.cpus.effective').read_text().strip()=='0-31','actual selected cgroup incarnation differs')
    api=types.SimpleNamespace(frozen_module=frozen_module,bounded=bounded)
    unitree=frozen_module(ROOT,frozen,'test/0073.ros-tests/support/source/ros_unitree/common.py','library_current_owners')
    return frozen,api,unitree


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for key in ('source','manifest','config','out'):parser.add_argument('--'+key,type=Path,required=True)
    for key in ('manifest-sha256','config-sha256','selection-input'):parser.add_argument('--'+key,required=True)
    args=parser.parse_args();frame=None
    try:
        frozen,api,unitree=bootstrap(args)
        library=api.frozen_module(ROOT,frozen,'test/0073.ros-tests/support/source/ros_unitree/library_inputs.py','library_input_copy_contract')
        native=api.frozen_module(ROOT,frozen,'test/0073.ros-tests/support/source/ros_unitree/native_family_inputs.py','native_family_input_qualification')
        class LibraryFrame(unitree.Frame):
            def __init__(self,selection_args):
                self.library_deadline=time.monotonic()+3600
                super().__init__(selection_args,native.KIND)
            def guard(self):
                super().guard()
                need(time.monotonic()<self.library_deadline,'finite3600s library preparation deadline exceeded')
                raw=Path('/proc/self/status').read_text()
                rows=re.findall(r'^VmRSS:\s+([0-9]+) kB$',raw,re.M)
                need(len(raw)<=128*1024 and len(rows)==1 and int(rows[0])*1024<=768*MIB,
                     'independent metadata coordinator RSS exceeds768MiB')
        frame=LibraryFrame(args);frame.frozen=frozen;frame.unitree=unitree
        admission=api.frozen_module(ROOT,frozen,'test/0073.ros-tests/support/source/ros_unitree/typed_transport_inputs.py','library_finite_original_evidence')
        selection,pin=frame.common.document(frame.input(args.selection_input))
        need(pin==frame.inputs[args.selection_input],'selected library metadata changed')
        status=native.stage(frame,api,admission,selection,library)
        return frame.finish(status)
    except BaseException as error:
        if frame is not None:
            frame.report['traceback']=traceback.format_exc();return frame.finish('FAIL',repr(error))
        print(traceback.format_exc(),file=sys.stderr);return 1


if __name__=='__main__':raise SystemExit(main())
