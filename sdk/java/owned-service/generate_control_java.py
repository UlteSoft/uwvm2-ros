#!/usr/bin/env python3
"""Explicit complete32 record/seven Service Java SOURCE operator.

The external Linux keeper admits this entry's executed bytes, interpreter,
stdlib/extensions and process/resource lifetime before its first instruction.
This helper only binds local source bytes below; it is not a Python sandbox.
No SourceFileLoader, ambient local aliases or pyc is used for admitted helpers.
No compiler, native type exporter, ROS process or capability is invoked.
"""
import argparse
import dataclasses
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import stat
import sys
import types

PROFILE='uwvm-ros-original-node-control-c-cpp-v1'
LIMIT=8*1024*1024
EXPECTED_RECORDS=set(['builtin_interfaces/msg/Time', 'rcl_interfaces/msg/FloatingPointRange', 'rcl_interfaces/msg/IntegerRange', 'rcl_interfaces/msg/ListParametersResult', 'rcl_interfaces/msg/Log', 'rcl_interfaces/msg/Parameter', 'rcl_interfaces/msg/ParameterDescriptor', 'rcl_interfaces/msg/ParameterEvent', 'rcl_interfaces/msg/ParameterType', 'rcl_interfaces/msg/ParameterValue', 'rcl_interfaces/msg/SetParametersResult', 'rcl_interfaces/srv/DescribeParameters_Request', 'rcl_interfaces/srv/DescribeParameters_Response', 'rcl_interfaces/srv/GetParameterTypes_Request', 'rcl_interfaces/srv/GetParameterTypes_Response', 'rcl_interfaces/srv/GetParameters_Request', 'rcl_interfaces/srv/GetParameters_Response', 'rcl_interfaces/srv/ListParameters_Request', 'rcl_interfaces/srv/ListParameters_Response', 'rcl_interfaces/srv/SetParameters_Request', 'rcl_interfaces/srv/SetParameters_Response', 'rcl_interfaces/srv/SetParametersAtomically_Request', 'rcl_interfaces/srv/SetParametersAtomically_Response', 'service_msgs/msg/ServiceEventInfo', 'type_description_interfaces/msg/Field', 'type_description_interfaces/msg/FieldType', 'type_description_interfaces/msg/IndividualTypeDescription', 'type_description_interfaces/msg/KeyValue', 'type_description_interfaces/msg/TypeDescription', 'type_description_interfaces/msg/TypeSource', 'type_description_interfaces/srv/GetTypeDescription_Request', 'type_description_interfaces/srv/GetTypeDescription_Response'])
CANONICAL={'bounded_records.h','bounded_records.hpp','bounded_interfaces.h','bounded_interfaces.hpp','control_wire.hpp','control_manifest.hpp','control-schema.json'}

def require(ok,message):
    if not ok:raise ValueError(message)

def digest(raw):return hashlib.sha256(raw).hexdigest()
def pin(raw):return {'bytes':len(raw),'sha256':digest(raw)}
def sha(value):
    require(type(value)is str and re.fullmatch('[0-9a-f]{64}',value),'explicit lowercase SHA256 required')
    return value

def read(path,limit=LIMIT):
    require(path.is_absolute() and path.resolve(strict=True)==path,'resolved absolute input required')
    fd=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
    try:
        first=os.fstat(fd)
        require(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=limit,'bounded ordinary FD required')
        chunks=[];count=0
        while True:
            part=os.read(fd,min(1024*1024,limit-count+1))
            if not part:break
            count+=len(part);require(count<=limit,'capture grew beyond bound');chunks.append(part)
        last=os.fstat(fd);named=os.lstat(path)
        fields=('st_dev','st_ino','st_mode','st_size','st_nlink','st_uid','st_gid','st_mtime_ns','st_ctime_ns')
        require(count==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in fields),'FD/name changed')
        return b''.join(chunks)
    finally:os.close(fd)

def strict(raw):
    def pairs(rows):
        out={}
        for k,v in rows:
            require(k not in out,'duplicate JSON key');out[k]=v
        return out
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:(_ for _ in()).throw(ValueError('nonfinite JSON')))

def wrappers(facts):
    lines=['package org.uwvm.roswasm.generated;','import org.uwvm.roswasm.OwnedService;',
           '/** Complete seven original Service roots. No native registry/authority. */',
           'public final class ControlServices {private ControlServices(){}']
    for f in facts:
        q=f['request']['type'];s=f['response']['type'];sym=f['symbol']
        # The real authoritative Record.symbol, not a guessed type-name mapping.
        qsym=f['request']['symbol'];ssym=f['response']['symbol']
        lines += ['public static OwnedService.Definition<BoundedRecords.'+qsym+',BoundedRecords.'+ssym+'> '+sym+'(){return new OwnedService.Definition<>('
            +json.dumps(f['native_type'])+','+('null' if f['native_identity']is None else json.dumps(f['native_identity']))+','+json.dumps(f['codec_identity'])
            +',BoundedRecords.'+qsym+'.CODEC,'+str(f['request']['maximum_wire_bytes'])+',BoundedRecords.'+ssym+'.CODEC,'+str(f['response']['maximum_wire_bytes'])+');}']
    return '\n'.join(lines+['}',''])

def run(a):
    require(sys.platform=='linux','execution only under authorized Linux keeper')
    root=a.source_root.resolve(strict=True);catalog=a.catalog.resolve(strict=True);canonical=a.canonical.resolve(strict=True);out=a.out.resolve(strict=False)
    require(all(p.is_absolute()for p in(a.source_root,a.catalog,a.canonical,a.out)) and not out.exists() and not out.is_relative_to(root) and not out.is_relative_to(canonical) and not canonical.is_relative_to(out),'fresh separate absolute output required')
    captured={};roles={}
    def remember(path,raw):
        retained=sum(len(v)for k,v in captured.items()if k!=path)
        require(retained+len(raw)<=16*1024*1024,'total retained captured-source bound')
        captured[path]=raw
    sources={'parent':'sdk/tools/custom_types.py','bounded':'sdk/custom-types/bounded_profile.py','managed':'sdk/custom-types/managed_profile.py',
             'codec':'sdk/java/src/main/java/org/uwvm/roswasm/Codec.java','codecs':'sdk/java/src/main/java/org/uwvm/roswasm/Codecs.java',
             'owned_service':'sdk/java/src/main/java/org/uwvm/roswasm/OwnedService.java'}
    for role,rel in sources.items():
        p=(root/rel).resolve(strict=True);raw=read(p);require(digest(raw)==sha(getattr(a,role+'_sha256')),'admitted foundation source differs: '+role);remember(p,raw);roles[role]={'path':str(p),**pin(raw)}
    entry=Path(__file__).resolve(strict=True);raw=read(entry);require(digest(raw)==sha(a.operator_sha256),'operator self-consistency differs (keeper admits executed entry separately)');remember(entry,raw)
    catalog_raw=read(catalog);require(digest(catalog_raw)==sha(a.catalog_sha256),'catalog differs');remember(catalog,catalog_raw);declared=strict(catalog_raw)
    require(type(declared)is dict and set(declared)=={'format','types'} and declared['format']==PROFILE and type(declared['types'])is list,'actual complete control catalog required')
    for row in declared['types']:
        require(type(row)is dict and type(row.get('file'))is str,'definition source row required');p=(catalog.parent/row['file']).resolve(strict=True);require(p.is_relative_to(catalog.parent),'original definition escaped catalog root');remember(p,read(p))
    proof_path=canonical/'generation.json';proof_raw=read(proof_path);require(digest(proof_raw)==sha(a.canonical_proof_sha256),'canonical source proof differs');remember(proof_path,proof_raw);original=strict(proof_raw)
    require(type(original)is dict and original.get('format')==PROFILE and original.get('implemented_record_outputs')==['c','cpp'] and original.get('native_registry_emitted')is False and set(original.get('outputs',{}))==CANONICAL,'genuine separate seven products/eighth proof required')
    for name,value in original['outputs'].items():
        p=canonical/name;raw=read(p);require(pin(raw)==value,'canonical product differs');remember(p,raw)
    # Capture COMPLETE transitive local helper and all parser inputs BEFORE any
    # helper instruction. Dynamic plugin loading is never called. Dataclasses
    # needs its new nonce module in sys.modules while resolving its own metadata.
    nonce=secrets.token_hex(16);modules={}
    try:
        for role in ('parent','bounded','managed'):
            p=root/sources[role];name='_uwvm_java_control_'+nonce+'_'+role;require(name not in sys.modules,'fresh private module required')
            m=types.ModuleType(name);m.__file__=str(p);m.__package__='';m.__loader__=m.__spec__=m.__cached__=None;sys.modules[name]=m;modules[role]=(name,m)
            exec(compile(captured[p],str(p),'exec',dont_inherit=True),m.__dict__)
            require(sys.modules.get(name)is m,'captured helper changed its module owner')
        parent=modules['parent'][1];bounded=modules['bounded'][1];managed=modules['managed'][1]
        def admitted_read(path,maximum):
            p=Path(path).resolve(strict=True);require(p in captured and len(captured[p])<=maximum,'parser requested uncaptured input');return captured[p]
        parent.bounded_read=admitted_read
        records,ordered,interfaces,pins=bounded.load_catalog(parent,catalog,False)
        require(len(ordered)==32 and len(records)==32 and {r.name for r in ordered}==EXPECTED_RECORDS and all(r.version==2 and r.profile==PROFILE for r in ordered),'complete original32 control IR required')
        require(pins['catalog']==pin(catalog_raw) and original.get('source_inputs')==pins and original.get('generator_sha256')==roles['parent']['sha256'] and original.get('bounded_generator_sha256')==roles['bounded']['sha256'],'same captured IR/canonical source revision required')
        registered=[i for i in interfaces if i.registered]
        require({i.name for i in registered}==bounded.control_registered_names() and sum(i.kind=='service'for i in registered)==7 and sum(i.kind=='message'for i in registered)==2,'full seven Service/two Topic roots required')
        managed.validate_ir(parent,bounded,records,ordered,interfaces,['java'])
        products=managed.emit_java(parent,bounded,records,ordered,interfaces)
        require(set(products)=={'org/uwvm/roswasm/generated/BoundedRecords.java'},'actual shared complete Java emitter output required')
        facts=bounded.control_service_facts(interfaces)
        require(len(facts)==7,'complete seven Service facts required')
        for f in facts:
            for kind in ('request','response'):f[kind]['symbol']=records[(2,f[kind]['type'])].symbol
        products['org/uwvm/roswasm/generated/ControlServices.java']=wrappers(facts)
        proof={'kind':'complete-control-java-source-v1','status':'SOURCE_ONLY_NOT_COMPILED_OR_EXECUTED','source_inputs':pins,'foundation_sources':roles,
               'canonical_generation':pin(proof_raw),'canonical_outputs':original['outputs'],'records':[dict(dataclasses.asdict(r),symbol=r.symbol)for r in ordered],
               'interfaces':[dataclasses.asdict(i)for i in interfaces],'service_roots':facts,'native_identity_syntax_required':False,
               'native_registry_emitted':False,'implicit_permissions':[],'runtime_qualification':False,'compiler_execution':False,'operator':pin(captured[entry])}
        total=sum(len(b.encode())for b in products.values());require(total<=32*1024*1024,'bounded source revision required')
        for p,raw in captured.items():require(read(p)==raw,'admitted input changed before Source commit')
        require(not out.exists(),'output appeared before commit');out.mkdir(parents=True,exist_ok=False,mode=0o700)
        proof['outputs']={name:pin(body.encode())for name,body in products.items()}
        products['generation.control-java.json']=json.dumps(proof,indent=2,sort_keys=True)+'\n'
        require(sum(len(body.encode('utf8'))for body in products.values())<=32*1024*1024,'complete products/proof bound')
        for name,body in products.items():
            rel=Path(name);require(not rel.is_absolute() and '..'not in rel.parts,'closed source output path');p=out/rel;p.parent.mkdir(parents=True,exist_ok=True)
            with p.open('xb')as f:f.write(body.encode('utf8'))
        return proof
    finally:
        for name,m in modules.values():
            if sys.modules.get(name)is m:del sys.modules[name]

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('source-root','catalog','canonical','out'):p.add_argument('--'+name,type=Path,required=True)
    for name in ('parent','bounded','managed','codec','codecs','owned-service','operator','catalog','canonical-proof'):p.add_argument('--'+name+'-sha256',required=True)
    a=p.parse_args();print(json.dumps(run(a),indent=2))
if __name__=='__main__':main()
