#!/usr/bin/env python3
"""Compile a pinned ROS 2 generated type description into an inert bounded schema.

Only the supplied JSON is read; references resolve within its complete declared
type closure. No include, package search, expression, native code or plugin is
executed. All operational bounds and floating-point restrictions are explicit.
The output does not register a native endpoint or grant authority. Native worker
admission must independently match generated introspection and type support.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from bounded_message_definition import compile_schema,exact,number,unique

KINDS={2:'int8',3:'uint8',4:'int16',5:'uint16',6:'int32',7:'uint32',8:'int64',9:'uint64',
       10:'float32',11:'float64',13:'uint8',15:'bool',16:'uint8',17:'string',18:'wstring',21:'string',22:'wstring'}

def compile_description(document,*,sequence_bound,string_bound,message_bytes,work,depth,finite_floats):
    exact(document,('type_description_msg','type_hashes'))
    description=document['type_description_msg'];exact(description,('type_description','referenced_type_descriptions'))
    number(sequence_bound,(1<<32)-1);number(string_bound,(1<<32)-2)
    if type(finite_floats)is not bool:raise ValueError('Explicit floating-point restriction required')
    sources={};nodes=[];cache={};built={};active=set();used=set()
    for item in [description['type_description'],*description['referenced_type_descriptions']]:
        exact(item,('type_name','fields'));name=item['type_name']
        if not isinstance(name,str)or not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*/(?:msg|srv|action)/[A-Za-z][A-Za-z0-9_]*',name)or name in sources:raise ValueError('Invalid or duplicate ROS type identity')
        if not isinstance(item['fields'],list)or len(item['fields'])>65536:raise ValueError('Bounded field list required')
        sources[name]=item
    def node(kind,bound=0,members=()):
        value=dict(kind=kind,bound=bound,flags=['finite']if finite_floats and kind in ('float32','float64')else [],members=[dict(name=name,type=index)for name,index in members])
        key=json.dumps(value,sort_keys=True)
        if key not in cache:
            if len(nodes)==4096:raise ValueError('Schema node capacity exceeded')
            cache[key]=len(nodes);nodes.append(value)
        return cache[key]
    def build(name):
        if name in active:raise ValueError('Recursive native type cannot acquire an implicit bound')
        if name in built:return built[name]
        if name not in sources or len(active)>=64:raise ValueError('Missing or excessive nested type')
        used.add(name);active.add(name);members=[]
        for field in sources[name]['fields']:
            exact(field,('name','type','default_value'));value=field['type'];exact(value,('type_id','capacity','string_capacity','nested_type_name'))
            identifier=number(value['type_id'],255);capacity=number(value['capacity'],(1<<32)-1);string_capacity=number(value['string_capacity'],(1<<32)-2)
            collection,base=divmod(identifier,48)
            if collection>3 or base not in (1,*KINDS):raise ValueError('Unsupported native representation')
            if collection==0 and capacity or collection==3 and capacity or collection==1 and not capacity:raise ValueError('Inconsistent array or sequence capacity')
            if base==1:
                if string_capacity:raise ValueError('Nested type cannot have a string capacity')
                child=build(value['nested_type_name'])
            else:
                if value['nested_type_name']:raise ValueError('Primitive cannot select nested native metadata')
                kind=KINDS[base];bound=0
                if kind in ('string','wstring'):
                    if base in (17,18)and string_capacity:raise ValueError('Unbounded string with contradictory source bound')
                    bound=min(string_bound,string_capacity)if base in (21,22)else string_bound
                elif string_capacity:raise ValueError('Non-string native field has a string capacity')
                child=node(kind,bound)
            if collection:
                child=node('array'if collection==1 else 'sequence',capacity if collection==1 else min(sequence_bound,capacity)if collection==2 else sequence_bound,[('element',child)])
            members.append((field['name'],child))
        active.remove(name);built[name]=node('record',members=members);return built[name]
    identity=description['type_description']['type_name'];root=build(identity)
    if root!=len(nodes)-1 or used!=set(sources):raise ValueError('Unused or inconsistent native type closure')
    hashes={}
    for item in document['type_hashes']:
        exact(item,('type_name','hash_string'));name=item['type_name'];digest=item['hash_string']
        if name not in sources or name in hashes or not isinstance(digest,str)or not re.fullmatch(r'RIHS01_[0-9a-f]{64}',digest)or set(digest[7:])=={'0'}:raise ValueError('Missing, duplicate or unsupported native type hash')
        hashes[name]=digest
    if set(hashes)!=used:raise ValueError('Incomplete native type hash closure')
    schema=dict(layout='cdr',message_bytes=message_bytes,work=work,depth=depth,nodes=nodes)
    binary=compile_schema(schema)
    return schema,binary,dict(type_identity=identity,native_type_hash=hashes[identity],native_type_hashes=hashes,
        descriptor_sha256=hashlib.sha256(binary).hexdigest(),sequence_bound=sequence_bound,string_bound=string_bound,finite_floats=finite_floats)

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--type-description',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    for name in ('sequence-bound','string-bound','message-bytes','work','depth'):p.add_argument('--'+name,type=int,required=True)
    floating=p.add_mutually_exclusive_group(required=True);floating.add_argument('--finite-floats',dest='finite_floats',action='store_true');floating.add_argument('--allow-nonfinite-floats',dest='finite_floats',action='store_false')
    a=p.parse_args()
    with a.type_description.open('rb')as stream:raw=stream.read(4*1024*1024+1)
    if len(raw)>4*1024*1024:raise ValueError('Native type description exceeds capacity')
    schema,binary,metadata=compile_description(json.loads(raw,object_pairs_hook=unique),sequence_bound=a.sequence_bound,string_bound=a.string_bound,
        message_bytes=a.message_bytes,work=a.work,depth=a.depth,finite_floats=a.finite_floats)
    metadata.update(source=str(a.type_description.resolve()),source_sha256=hashlib.sha256(raw).hexdigest(),schema=schema)
    with a.output.open('xb')as stream:stream.write(binary)
    with a.output.with_suffix(a.output.suffix+'.json').open('x')as stream:json.dump(metadata,stream,indent=2);stream.write('\n')

if __name__=='__main__':main()
