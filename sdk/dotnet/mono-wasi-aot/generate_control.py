#!/usr/bin/env python3
"""Explicit captured-source C# CONTROL producer. Author packet has not run it."""
from pathlib import Path
import argparse
import hashlib
import json
import os
import stat
import sys
import types

SOURCES={
 'sdk/tools/custom_types.py':'1a73d6a87624e563bbb6be2624e77b51d7dba0f0f2fb524cda4079f493381826',
 'sdk/custom-types/bounded_profile.py':'40e30d542283cecad4ca210599dbc6d96259f774a1c778050920b32dbe4a0bf4',
 'sdk/custom-types/managed_profile.py':'3edb80ff26d48d8e3c9b9ba3ebf363ae09c0b194200527c0fa0fce16e5af14c7',
 'sdk/custom-types/managed_csharp.py':'16f4d958745b25154f55fb19a7b1ac0cce684f15271a38d6344256148deb1b81'}
PROFILE='uwvm-ros-original-node-control-c-cpp-v1'
CANONICAL={'bounded_records.h','bounded_records.hpp','bounded_interfaces.h','bounded_interfaces.hpp','control_wire.hpp','control_manifest.hpp','control-schema.json'}
EXPECTED=set('''builtin_interfaces/msg/Time
rcl_interfaces/msg/FloatingPointRange
rcl_interfaces/msg/IntegerRange
rcl_interfaces/msg/ListParametersResult
rcl_interfaces/msg/Log
rcl_interfaces/msg/Parameter
rcl_interfaces/msg/ParameterDescriptor
rcl_interfaces/msg/ParameterEvent
rcl_interfaces/msg/ParameterType
rcl_interfaces/msg/ParameterValue
rcl_interfaces/msg/SetParametersResult
rcl_interfaces/srv/DescribeParameters_Request
rcl_interfaces/srv/DescribeParameters_Response
rcl_interfaces/srv/GetParameterTypes_Request
rcl_interfaces/srv/GetParameterTypes_Response
rcl_interfaces/srv/GetParameters_Request
rcl_interfaces/srv/GetParameters_Response
rcl_interfaces/srv/ListParameters_Request
rcl_interfaces/srv/ListParameters_Response
rcl_interfaces/srv/SetParameters_Request
rcl_interfaces/srv/SetParameters_Response
rcl_interfaces/srv/SetParametersAtomically_Request
rcl_interfaces/srv/SetParametersAtomically_Response
service_msgs/msg/ServiceEventInfo
type_description_interfaces/msg/Field
type_description_interfaces/msg/FieldType
type_description_interfaces/msg/IndividualTypeDescription
type_description_interfaces/msg/KeyValue
type_description_interfaces/msg/TypeDescription
type_description_interfaces/msg/TypeSource
type_description_interfaces/srv/GetTypeDescription_Request
type_description_interfaces/srv/GetTypeDescription_Response'''.splitlines())
def sha(raw):return hashlib.sha256(raw).hexdigest()
def identity(s):return tuple(getattr(s,'st_'+x)for x in('dev','ino','mode','nlink','uid','gid','size','mtime_ns','ctime_ns'))
def strict(raw):
    def pairs(rows):
        result={}
        for k,v in rows:
            if k in result:raise ValueError('duplicate JSON key')
            result[k]=v
        return result
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:(_ for _ in()).throw(ValueError('nonfinite JSON')))
class Capture:
    def __init__(self):self.files={};self.total=0
    def get(self,path):
        path=Path(path).absolute()
        if path in self.files:return self.files[path][1]
        fd=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_CLOEXEC|os.O_NOFOLLOW)
        try:
            s=os.fstat(fd)
            if not stat.S_ISREG(s.st_mode)or not 0<=s.st_size<=8*1024*1024:raise ValueError('bounded regular source required')
            chunks=[];count=0
            while count<=s.st_size:
                part=os.read(fd,min(65536,s.st_size+1-count))
                if not part:break
                chunks.append(part);count+=len(part)
            raw=b''.join(chunks)
            if len(raw)!=s.st_size or identity(s)!=identity(os.fstat(fd))or identity(s)!=identity(os.lstat(path)):raise ValueError('source FD/name changed')
            self.total+=len(raw)
            if self.total>32*1024*1024:raise ValueError('aggregate source capture limit')
            self.files[path]=(fd,raw,identity(s));return raw
        except BaseException:os.close(fd);raise
    def verify(self):
        for p,(fd,raw,key)in self.files.items():
            if identity(os.fstat(fd))!=key or identity(os.lstat(p))!=key:raise ValueError('captured source changed before commit')
            os.lseek(fd,0,os.SEEK_SET);parts=[];count=0
            while count<=len(raw):
                part=os.read(fd,min(65536,len(raw)+1-count))
                if not part:break
                parts.append(part);count+=len(part)
            if b''.join(parts)!=raw:raise ValueError('captured source bytes changed before commit')
    def close(self):
        for fd,_,_ in self.files.values():os.close(fd)
        self.files.clear()
def load(name,path,raw,names):
    if name in sys.modules:raise ValueError('fixed private source namespace occupied')
    module=types.ModuleType(name);module.__file__=str(path);sys.modules[name]=module;names.append(name)
    exec(compile(raw,str(path),'exec'),module.__dict__)
    return module
def run(root,canonical,out,operator_sha,emitter_sha):
    if not all(p.is_absolute()for p in(root,canonical,out)):raise ValueError('explicit absolute paths required')
    root=root.resolve(strict=True);canonical=canonical.resolve(strict=True);out=out.resolve(strict=False)
    if out.exists()or out.is_relative_to(root)or out.is_relative_to(canonical)or canonical.is_relative_to(out):raise ValueError('fresh output outside source/canonical roots required')
    capture=Capture();names=[]
    try:
        own_path=Path(__file__).absolute();own=capture.get(own_path)
        emitter_path=own_path.with_name('control_csharp_emitter.py');emitter_raw=capture.get(emitter_path)
        if sha(own)!=operator_sha or sha(emitter_raw)!=emitter_sha:raise ValueError('explicit reviewed operator/emitter SHA pair required')
        raw={rel:capture.get(root/rel)for rel in SOURCES}
        if any(sha(raw[rel])!=pin for rel,pin in SOURCES.items()):raise ValueError('current captured generator source differs from reviewed exact pair')
        catalog=root/'sdk/rmw_wasm/default_control/catalog.preparation.json';catalog_raw=capture.get(catalog);declared=strict(catalog_raw)
        if set(declared)!={'format','types'}or declared['format']!=PROFILE:raise ValueError('exact CONTROL catalog required')
        base=catalog.parent;definitions={}
        for row in declared['types']:
            path=(base/row['file']).resolve(strict=True)
            if not path.is_relative_to(base):raise ValueError('original definition escaped catalog root')
            definitions[path]=capture.get(path)
        parent=load('_uwvm_csharp_control_parent',root/'sdk/tools/custom_types.py',raw['sdk/tools/custom_types.py'],names)
        bounded=load('_uwvm_csharp_control_bounded',root/'sdk/custom-types/bounded_profile.py',raw['sdk/custom-types/bounded_profile.py'],names)
        managed=load('_uwvm_csharp_control_managed',root/'sdk/custom-types/managed_profile.py',raw['sdk/custom-types/managed_profile.py'],names)
        emitter=load('_uwvm_csharp_control_adapter',emitter_path,emitter_raw,names)
        inputs={catalog:catalog_raw,**definitions}
        original_read=parent.bounded_read
        def captured_read(path,maximum):
            p=Path(path).resolve(strict=True)
            if p not in inputs or len(inputs[p])>maximum:raise ValueError('parser requested uncaptured definition/input')
            return inputs[p]
        parent.bounded_read=captured_read
        try:records,ordered,interfaces,pins=bounded.load_catalog(parent,catalog,False)
        finally:parent.bounded_read=original_read
        if len(ordered)!=32 or {r.name for r in ordered}!=EXPECTED or any(r.profile!=PROFILE for r in ordered):raise ValueError('complete original32 record closure required')
        managed.validate_ir(parent,bounded,records,ordered,interfaces,['csharp'])
        registered=[i for i in interfaces if i.registered]
        if {i.name for i in registered}!=bounded.control_registered_names()or sum(i.kind=='service'for i in registered)!=7:raise ValueError('exact seven Service and two Topic roots required')
        if pins.get('catalog')!={'bytes':len(catalog_raw),'sha256':sha(catalog_raw)}:raise ValueError('actual IR/catalog binding differs')
        for row in pins['definitions']:
            b=definitions[(base/row['file']).resolve(strict=True)]
            if row['bytes']!=len(b)or row['sha256']!=sha(b):raise ValueError('actual IR/definition binding differs')
        generation_raw=capture.get(canonical/'generation.json');generation=strict(generation_raw)
        if generation.get('format')!=PROFILE or generation.get('source_inputs')!=pins or generation.get('implemented_record_outputs')!=['c','cpp']or generation.get('native_registry_emitted')is not False:raise ValueError('actual same-source complete C/C++ source revision required')
        if generation.get('generator_sha256')!=SOURCES['sdk/tools/custom_types.py']or generation.get('bounded_generator_sha256')!=SOURCES['sdk/custom-types/bounded_profile.py']:raise ValueError('canonical revision source pair differs')
        if set(generation['outputs'])!=CANONICAL:raise ValueError('complete seven canonical source products required')
        for name,pin in generation['outputs'].items():
            b=capture.get(canonical/name)
            if pin!={'bytes':len(b),'sha256':sha(b)}:raise ValueError('canonical source product differs')
        products,details=emitter.emit(parent,bounded,records,ordered,interfaces,raw['sdk/custom-types/managed_csharp.py'])
        proof={'format':'uwvm-ros-control-csharp-source-v1','status':'GENERATED_SOURCE_ONLY_NOT_COMPILED_OR_EXECUTED','native_registry_emitted':False,'implicit_permissions':[],
            'source_inputs':pins,'generator_source_sha256':SOURCES,'operator_sha256':sha(own),'adapter_sha256':sha(emitter_raw),'canonical_generation_sha256':sha(generation_raw),
            'complete_record_names':[r.name for r in ordered],'complete_record_contracts':{r.name:{'minimum':r.minimum,'maximum':r.maximum,'aggregate':r.aggregate_wire_bytes,'cartesian':r.cartesian_maximum,'constants':r.constants,'members':[vars(m)for m in r.members]}for r in ordered},
            'emitter':details,'external_execution_tcb':'actual admitted Python interpreter and standard library require independent qualification; no -B/pyc claim',
            'outputs':{name:{'bytes':len(body.encode()),'sha256':sha(body.encode())}for name,body in products.items()}}
        if set(products)!={'BoundedRecords.cs','ControlRoots.cs'}or sum(len(b.encode())for b in products.values())>8*1024*1024:raise ValueError('fixed bounded C# source products required')
        capture.verify()
        if out.exists():raise ValueError('fresh output appeared')
        out.mkdir(parents=True,exist_ok=False)
        products['generation.control-csharp.json']=json.dumps(proof,indent=2)+'\n'
        for name,body in products.items():
            fd=os.open(out/name,os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o644)
            try:
                b=body.encode();at=0
                while at<len(b):
                    n=os.write(fd,b[at:])
                    if n<=0:raise OSError('source write made no progress')
                    at+=n
            finally:os.close(fd)
        return proof
    finally:
        for name in reversed(names):sys.modules.pop(name,None)
        capture.close()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in('source-root','canonical','out'):p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--operator-sha256',required=True);p.add_argument('--emitter-sha256',required=True);p.add_argument('--explicit-owned-ros2',action='store_true',required=True)
    a=p.parse_args()
    try:print(json.dumps(run(a.source_root,a.canonical,a.out,a.operator_sha256,a.emitter_sha256),indent=2))
    except(Exception)as e:print(str(e),file=sys.stderr);return 2
    return 0
if __name__=='__main__':raise SystemExit(main())
