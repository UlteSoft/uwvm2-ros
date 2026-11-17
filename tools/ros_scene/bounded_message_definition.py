#!/usr/bin/env python3
"""Encode an explicit bounded message schema; never load types or infer grants.

Every sequence/string bound and every validation budget is supplied by the
deployer. Source generators may build this document from .msg/IDL, but they
must preserve the source identity separately. Structural equivalence is not
ROS type identity or permission to publish or actuate.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct

KINDS=('bool','int8','uint8','int16','uint16','int32','uint32','int64','uint64',
       'float32','float64','string','wstring','record','array','sequence')
FLAGS={'finite':1,'unicode':2,'no-nul':4}

def exact(value,keys):
    if not isinstance(value,dict) or set(value)!=set(keys):
        raise ValueError('Expected exactly fields: '+', '.join(keys))

def number(value,maximum):
    if type(value) is not int or not 0<=value<=maximum:
        raise ValueError('Integer exceeds explicit capacity')
    return value

def u64(value):
    return struct.pack('<Q',value)

def text(value):
    if not isinstance(value,str) or len(value.encode('utf-8'))>255 or '\0' in value:
        raise ValueError('Invalid bounded text')
    data=value.encode('utf-8')
    return u64(len(data))+data

def compile_schema(document):
    exact(document,('layout','message_bytes','work','depth','nodes'))
    if document['layout'] not in ('ros1','cdr'):
        raise ValueError('Only packed ROS1 or plain CDR layouts are supported')
    for key,maximum in [('message_bytes',16*1024*1024),('work',64*1024*1024),('depth',64)]:
        if not number(document[key],maximum):raise ValueError('Positive '+key+' required')
    nodes=document['nodes']
    if not isinstance(nodes,list) or not 1<=len(nodes)<=4096:raise ValueError('Bounded node list required')
    result=text('uwvm.ros.bounded-message')+text(document['layout'])
    result+=b''.join(u64(document[key]) for key in ('message_bytes','work','depth'))+u64(len(nodes))
    work=[];depths=[];edges=0
    for index,node in enumerate(nodes):
        exact(node,('kind','bound','flags','members'));kind=node['kind']
        if kind not in KINDS:raise ValueError('Unsupported node kind')
        bound=number(node['bound'],(1<<32)-1);flags=node['flags'];members=node['members']
        if not isinstance(flags,list) or any(flag not in FLAGS for flag in flags) or len(set(flags))!=len(flags):raise ValueError('Unknown/duplicate flags')
        if not isinstance(members,list):raise ValueError('Explicit member list required')
        if kind in KINDS[:11]:
            if bound or members or set(flags)-({'finite'} if kind.startswith('float') else set()):raise ValueError('Invalid primitive restrictions')
        elif kind in ('string','wstring'):
            if members or bound==(1<<32)-1 or set(flags)-{'unicode','no-nul'}:raise ValueError('Invalid text restrictions')
            if kind=='wstring' and document['layout']=='ros1':raise ValueError('ROS1 has no wide-string layout')
        elif kind=='record':
            if bound or flags:raise ValueError('Invalid record restrictions')
        elif len(members)!=1 or flags or (kind=='array' and not bound):raise ValueError('Invalid array/sequence restrictions')
        edges+=len(members)
        if edges>65536:raise ValueError('Too many member edges')
        result+=text(kind)+u64(bound)+u64(sum(FLAGS[flag] for flag in flags))+u64(len(members))
        names=set();cost=1;depth=1
        for member in members:
            exact(member,('name','type'));name=member['name'];child=number(member['type'],len(nodes)-1)
            if not isinstance(name,str) or not re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]{0,254}',name) or name in names or child>=index:
                raise ValueError('Invalid/duplicate member name or forward/cyclic reference')
            if kind!='record' and name!='element':raise ValueError('Collection member must be named element')
            names.add(name);cost+=work[child]*(1 if kind=='record' else bound);depth=max(depth,depths[child]+1)
            result+=text(name)+u64(child)
        if kind in ('string','wstring'):cost+=bound*(2 if kind=='wstring' else 1)
        if cost>document['work'] or depth>document['depth']:raise ValueError('Schema exceeds declared worst-case traversal budget')
        work.append(cost);depths.append(depth)
    if nodes[-1]['kind']!='record':raise ValueError('Root must be a record')
    reached={len(nodes)-1}
    for index in range(len(nodes)-1,-1,-1):
        if index in reached:reached.update(member['type'] for member in nodes[index]['members'])
    if len(reached)!=len(nodes):raise ValueError('Unreachable nodes are not allowed')
    if len(result)>4*1024*1024:raise ValueError('Description exceeds byte limit')
    return result

def unique(pairs):
    result={}
    for key,value in pairs:
        if key in result:raise ValueError('Duplicate JSON key: '+key)
        result[key]=value
    return result

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input',type=Path);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    if args.input.stat().st_size>8*1024*1024:raise ValueError('Input document too large')
    result=compile_schema(json.loads(args.input.read_text(),object_pairs_hook=unique))
    with args.output.open('xb') as stream:stream.write(result)
    print(json.dumps(dict(bytes=len(result),sha256=hashlib.sha256(result).hexdigest())))

if __name__=='__main__':main()
