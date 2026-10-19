#!/usr/bin/env python3
"""Admit current native-bound SOURCE products, never generate or qualify Native.

Reads exact actual generator/catalog/definition/product bytes. No imports of a
producer, subprocess, ROS library, native identity computation or policy grant.
The installed-SDK executable compile/link probe and native contract are separate.
"""
from pathlib import Path
import argparse
import hashlib
import json
import math
import os
import re
import stat
import sys

PROFILE='uwvm-ros-custom-bounded-v2'
REQUIRED={'bounded_records.h','bounded_records.hpp','bounded_interfaces.h','bounded_interfaces.hpp',
          'bounded_records.rs','bounded_records.mjs','bounded_records.go','bounded_records.ts',
          'manifest.hpp','native_ros1.hpp','native_ros2.hpp'}
OPTIONAL={'bounded_interfaces.'+suffix for suffix in('rs','mjs','go','ts')}
LIMIT=8*1024*1024


def read(path):
    fd=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
    try:
        first=os.fstat(fd)
        if not stat.S_ISREG(first.st_mode)or first.st_size>LIMIT:raise ValueError('bounded ordinary source required')
        parts=[];count=0
        while True:
            b=os.read(fd,min(65536,LIMIT-count+1))
            if not b:break
            count+=len(b)
            if count>LIMIT:raise ValueError('source grew beyond explicit bound')
            parts.append(b)
        last=os.fstat(fd);named=os.lstat(path)
        key=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mode,s.st_mtime_ns,s.st_ctime_ns,s.st_nlink)
        if key(first)!=key(last)or key(first)!=key(named)or count!=first.st_size:raise ValueError('source FD/name/extent changed')
        return b''.join(parts)
    finally:os.close(fd)


def strict(raw):
    def pairs(rows):
        out={}
        for k,v in rows:
            if k in out:raise ValueError('duplicate source JSON member')
            out[k]=v
        return out
    def finite(text):
        value=float(text)
        if not math.isfinite(value):raise ValueError('nonfinite source JSON')
        return value
    return json.loads(raw,object_pairs_hook=pairs,parse_float=finite,
        parse_constant=lambda _:(_ for _ in()).throw(ValueError('nonfinite source JSON')))


def content(raw):return {'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}


def relative(name):
    if type(name)is not str or not name or '\\'in name or name.startswith('/')or any(p in('','.','..')for p in name.split('/')):
        raise ValueError('closed relative source path required')
    return name


def catalog_interfaces(actual):
    """Derive category from actual catalog type AND original file extension.

    The generation proof cannot rename a Service to Message and thereby omit
    its protocol closure. These are source facts, never Native SDK identities.
    """
    if type(actual)is not dict or set(actual)!={'format','types'} or actual['format']!=PROFILE or type(actual['types'])is not list or not 1<=len(actual['types'])<=128:
        raise ValueError('complete native-bound custom bounded catalog required')
    result={}
    for row in actual['types']:
        if type(row)is not dict or type(row.get('ros_version'))is not int or row['ros_version']not in(1,2) or type(row.get('type'))is not str:
            raise ValueError('exact integer backend and actual catalog type required')
        version,name=row['ros_version'],row['type'];file=relative(row.get('file'))
        suffix=Path(file).suffix;kind={'.msg':'message','.srv':'service','.action':'action'}.get(suffix)
        parts=name.split('/');category={'message':'msg','service':'srv','action':'action'}.get(kind)
        if kind is None or len(parts)!=(2 if version==1 else 3) or any(not part for part in parts) or (version==2 and parts[1]!=category) or Path(file).stem!=parts[-1]:
            raise ValueError('catalog native type category and original definition extension/name differ')
        key=(version,name)
        if key in result:raise ValueError('duplicate native catalog interface')
        result[key]=(kind,row)
    return result


def validate_types(originals,types):
    if type(types)is not list or len(types)!=len(originals):raise ValueError('whole generated interface inventory differs')
    seen=set()
    for row in types:
        if type(row)is not dict or type(row.get('ros_version'))is not int or row['ros_version']not in(1,2) or type(row.get('type'))is not str:
            raise ValueError('exact integer generated backend and type required')
        key=(row['ros_version'],row['type'])
        if key in seen or key not in originals:raise ValueError('whole generated interface inventory differs')
        seen.add(key);kind,original=originals[key]
        if row.get('kind')!=kind:raise ValueError('generated kind differs from actual original catalog category')
        if row['native_identity']!=original['native_identity']:raise ValueError('native root identity differs')
        roles={'message':{'message'},'service':{'request','response'},'action':{'goal','result','feedback'}}[kind]
        if type(row['components'])is not dict or set(row['components'])!=roles:raise ValueError('complete native component inventory required')
        for role,component in row['components'].items():
            actual_id=original['native_identity']if role=='message'else original['component_identities'][role]
            actual_type=key[1]if role=='message'else key[1]+('_'if key[0]==2 else'')+role.title()
            if type(component)is not dict or component.get('type')!=actual_type or component.get('native_identity')!=actual_id:
                raise ValueError('native component owner/type/identity differs')
    if seen!=set(originals):raise ValueError('whole generated interface inventory differs')


def protocol_traits(native_raw,types):
    """A source guard for real fresh generator output, not installed SDK proof."""
    text=native_raw.decode('utf-8')
    for row in types:
        if row['ros_version']==2 and row['kind']in('service','action'):
            native='::'+row['type'].replace('/','::')
            if 'template<> struct custom_native_interface_schema<'+native+'> {'not in text:
                raise ValueError('current complete ROS2 protocol schema trait is absent')
    if '#define UWVM_ROS_CUSTOM_ROS2_TYPES('not in text:raise ValueError('actual provider table absent')


def admit(source_root,catalog,generated,version,expected):
    if not all(p.is_absolute()for p in(source_root,catalog,generated)):raise ValueError('explicit absolute source/catalog/product paths required')
    if type(version)is not int or version not in(1,2)or not expected or len(expected)!=len(set(expected)):raise ValueError('exact backend and interface set required')
    if generated.resolve()==(source_root/'sdk/custom-types/generated-bounded-v2').resolve():raise ValueError('historical delivered products are immutable, regenerate into fresh owned output')
    captures={}
    def capture(path):
        path=Path(path)
        if path not in captures:captures[path]=read(path)
        return captures[path]
    parent=capture(source_root/'sdk/tools/custom_types.py');bounded=capture(source_root/'sdk/custom-types/bounded_profile.py')
    catalog_raw=capture(catalog);actual=strict(catalog_raw);proof_raw=capture(generated/'generation.json');proof=strict(proof_raw)
    originals=catalog_interfaces(actual)
    if type(proof)is not dict or proof.get('format')!=PROFILE or proof.get('status')!='GENERATED_SOURCE_ONLY'or proof.get('native_registry_emitted')is not True or proof.get('implicit_permissions')!=[]:
        raise ValueError('native-bound SOURCE-ONLY generation proof required; guest-only or permissions refused')
    if proof.get('generator_sha256')!=content(parent)['sha256']or proof.get('bounded_generator_sha256')!=content(bounded)['sha256']:
        raise ValueError('generation is not from the actual selected current parent/bounded source')
    inputs=proof['source_inputs']
    if type(inputs)is not dict or not {'catalog','definitions'}<=set(inputs)or set(inputs)-{'catalog','definitions','facade_templates'}or inputs['catalog']!=content(catalog_raw):
        raise ValueError('actual catalog/source proof association differs')
    names=[];definitions=set();selected=[]
    for row in actual['types']:
        if type(row)is not dict or type(row.get('ros_version'))is not int or row['ros_version']not in(1,2)or type(row.get('type'))is not str:raise ValueError('complete typed catalog row required')
        key=(row['ros_version'],row['type'])
        if key in names:raise ValueError('duplicate native catalog interface')
        names.append(key);definitions.add(relative(row['file']))
        pattern=r'[0-9a-f]{32}'if row['ros_version']==1 else r'RIHS01_[0-9a-f]{64}'
        if type(row.get('native_identity'))is not str or re.fullmatch(pattern,row['native_identity'])is None:raise ValueError('actual exported native identity must already be bound')
        if row['ros_version']==version:selected.append(row['type'])
    if set(selected)!=set(expected):raise ValueError('selected actual interface set differs from the caller selection')
    declarations=inputs['definitions']
    if type(declarations)is not list or len(declarations)!=len(definitions)or {r['file']for r in declarations}!=definitions:raise ValueError('whole definition inventory differs')
    for row in declarations:
        if type(row)is not dict or set(row)!={'file','bytes','sha256'}or type(row['bytes'])is not int:raise ValueError('exact definition source pin required')
        if {k:row[k]for k in('bytes','sha256')}!=content(capture(catalog.parent/relative(row['file']))):raise ValueError('original definition bytes differ')
    outputs=proof['outputs']
    if type(outputs)is not dict or not REQUIRED<=set(outputs)or set(outputs)-REQUIRED-OPTIONAL:raise ValueError('complete closed generated product set required')
    if set(outputs)&OPTIONAL and set(outputs)&OPTIONAL!=OPTIONAL:raise ValueError('partial managed interface output set refused')
    total=0
    for name,pin in outputs.items():
        if type(pin)is not dict or set(pin)!={'bytes','sha256'}or type(pin['bytes'])is not int or pin!=content(capture(generated/name)):raise ValueError('paired whole product differs')
        total+=pin['bytes']
        if total>32*1024*1024:raise ValueError('complete native source product budget exceeded')
    templates=inputs.get('facade_templates',[])
    expected_templates={'interface_templates/bounded_interfaces.'+e+'.in'for e in('rs','mjs','go','ts')}if set(outputs)&OPTIONAL else set()
    if type(templates)is not list or len(templates)!=len(expected_templates)or {r['generator_relative']for r in templates}!=expected_templates:raise ValueError('actual managed template inventory differs')
    for row in templates:
        if type(row)is not dict or set(row)!={'generator_relative','bytes','sha256'}or type(row['bytes'])is not int:raise ValueError('exact template source pin required')
        if {k:row[k]for k in('bytes','sha256')}!=content(capture(source_root/'sdk/custom-types'/relative(row['generator_relative']))):raise ValueError('actual template source differs')
    validate_types(originals,proof['types'])
    # Required protocol traits are driven ONLY by the captured original catalog,
    # never by a kind/category asserted by the generated proof itself.
    protocol_traits(capture(generated/'native_ros2.hpp'),
        [{'ros_version':version,'type':name,'kind':item[0]}for(version,name),item in originals.items()])
    for path,raw in captures.items():
        if read(path)!=raw:raise ValueError('admitted native source changed')
    return {'status':'CURRENT_PAIRED_NATIVE_SOURCE_NOT_COMPILED_OR_QUALIFIED','source_inputs':{str(p):content(b)for p,b in captures.items()},
            'ros_version':version,'selected_interfaces':sorted(expected),'common_manifest':'manifest.hpp','native_provider':'native_ros'+str(version)+'.hpp',
            'actual_generation_required_and_present':True,'compiler_execution':'NOT_PERFORMED_BY_ADMISSION','Native_qualification':'REQUIRED_SEPARATELY','implicit_permissions':[]}


def main():
    p=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for k in('source-root','catalog','generated'):p.add_argument('--'+k,type=Path,required=True)
    p.add_argument('--ros-version',type=int,choices=(1,2),required=True);p.add_argument('--expected-interface',action='append',required=True)
    a=p.parse_args()
    try:result=admit(a.source_root,a.catalog,a.generated,a.ros_version,a.expected_interface)
    except(OSError,UnicodeError,ValueError,KeyError,TypeError,RecursionError)as error:
        print(str(error),file=sys.stderr);return 2
    print(json.dumps(result,indent=2));return 0
if __name__=='__main__':raise SystemExit(main())
