#!/usr/bin/env python3
"""Produce language-neutral canonical byte examples, without running ROS.

The values are deliberately independent of language SDK implementations. A
codec test must encode these values and compare every byte, and decode them
without trailing bytes. Large signed/unsigned integers use decimal strings in
JSON so JavaScript readers never round them through Number. This generator is
a specification fixture, not proof that any language implementation passed.
"""
import argparse
import json
from pathlib import Path
import struct

SDK = Path(__file__).resolve().parents[1]


def text(value):
    """Encode UTF-8 byte length, not a character count or a NUL terminator."""
    data = value.encode('utf-8')
    return struct.pack('<I', len(data))+data


def counted(items):
    """Prefix the canonical element count before already encoded elements."""
    return struct.pack('<I', len(items))+b''.join(items)


def typed_event(kind, tag, payload):
    """Encode the v2 typed-event envelope and its separate raw payload length."""
    return struct.pack('<IIBBB', 1, 2, kind, 2, tag)+text('/demo')+text('value')+struct.pack('<I', len(payload))+payload


def vectors():
    """Return positive golden bytes and structural negative samples."""
    positive = []
    def add(name, codec, value, data, **extra):
        positive.append({'name':name, 'codec':codec, 'value':value, 'hex':data.hex(),
                         'bytes':len(data), **extra})
    for name, value in [('empty',''),('ascii','hello'),('utf8','robot \U0001f30d')]:
        add('string-'+name,'string',value,text(value))
    for value in (False, True):
        add('bool-'+str(value).lower(),'bool',value,bytes([value]))
    for value in (-2147483648, -1, 2147483647):
        add('int32-'+str(value),'parameter-int32',str(value),struct.pack('<i',value),backend=1)
    for value in (-9223372036854775808, -1, 9223372036854775807):
        add('int64-'+str(value),'parameter-int64',str(value),struct.pack('<q',value),backend=2)
    for name, value in [('zero',0.0),('negative-zero',-0.0),('fraction',1.25)]:
        add('float64-'+name,'parameter-float64',name if name=='negative-zero' else value,struct.pack('<d',value))
    twist = [1.0,-2.0,3.0,0.0,-0.0,0.5]
    add('twist','twist',[1,-2,3,0,'negative-zero',0.5],struct.pack('<6d',*twist))
    add('array-bool','parameter-bool-array',[False,True,False],counted([b'\0',b'\1',b'\0']))
    add('array-byte','parameter-byte-array',[0,127,255],counted([bytes([n]) for n in (0,127,255)]),backend=2)
    add('array-int32','parameter-int32-array',['-1','2147483647'],counted([struct.pack('<i',n) for n in (-1,2147483647)]),backend=1)
    add('array-int64','parameter-int64-array',['-9223372036854775808','9223372036854775807'],counted([struct.pack('<q',n) for n in (-9223372036854775808,9223372036854775807)]),backend=2)
    add('array-float64','parameter-float64-array',[1.25,'negative-zero'],counted([struct.pack('<d',n) for n in (1.25,-0.0)]))
    add('array-string','parameter-string-array',['','hello','\u03bb'],counted([text(n) for n in ('','hello','\u03bb')]))
    add('array-empty','parameter-string-array',[],struct.pack('<I',0))
    image={'sequence':0,'seconds':1,'nanoseconds':2,'frame_id':'camera','height':1,'width':1,
           'encoding':'rgb8','bigendian':False,'step':3,'data':[255,0,7]}
    add('image-rgb8','image',image,struct.pack('<III',0,1,2)+text('camera')+struct.pack('<II',1,1)+text('rgb8')+b'\0'+struct.pack('<II',3,3)+bytes(image['data']))
    transform={'sequence':0,'seconds':1,'nanoseconds':2,'parent_frame':'map','child_frame':'base',
               'translation':[1,2,3],'quaternion':[0,0,0,1]}
    add('transform','transform',transform,struct.pack('<III',0,1,2)+text('map')+text('base')+struct.pack('<7d',1,2,3,0,0,0,1))
    add('trigger-request','trigger-request',None,b'')
    add('set-bool-request','set-bool-request',True,b'\1')
    add('service-response','service-response',{'success':True,'message':'ok'},b'\1'+text('ok'))
    request_id=9223372036854775813
    add('service-request-event','service-request-event',{'request_id':str(request_id),'request_hex':'01'},struct.pack('<Q',request_id)+b'\1')
    add('service-response-envelope','service-response-envelope',{'request_id':str(request_id),'response_hex':(b'\1'+text('ok')).hex()},struct.pack('<Q',request_id)+b'\1'+text('ok'))
    add('fibonacci-order','fibonacci-goal',5,struct.pack('<i',5))
    add('goal-id','goal-id',str(request_id),struct.pack('<Q',request_id))
    add('incoming-goal','incoming-goal',{'goal_id':str(request_id),'order':5},struct.pack('<Qi',request_id,5))
    sequence=[0,1,1,2,3,5];tail=counted([struct.pack('<i',n) for n in sequence])
    add('feedback','action-feedback',{'goal_id':str(request_id),'values':sequence},struct.pack('<Q',request_id)+tail)
    add('success','action-result',{'goal_id':str(request_id),'kind':2,'values':sequence},struct.pack('<QB',request_id,2)+tail)
    add('server-feedback','action-response',{'goal_id':str(request_id),'kind':1,'values':sequence},struct.pack('<QB',request_id,1)+tail)
    add('lifecycle-state','lifecycle-state',{'id':3,'label':'active'},b'\3'+text('active'),backend=2)
    add('time-nanoseconds','time',str(request_id),struct.pack('<Q',request_id))
    add('legacy-parameter-event','parameter-event',{'seconds':1,'nanoseconds':2,'kind':2,'node':'/demo','name':'value','value':'true'},struct.pack('<IIB',1,2,2)+text('/demo')+text('value')+text('true'),backend=2)
    payloads={1:('bool',True,b'\1'),2:('int64','9223372036854775807',struct.pack('<q',9223372036854775807)),
              3:('float64',1.25,struct.pack('<d',1.25)),4:('string','hello',text('hello')),
              5:('byte-array',[0,255],counted([b'\0',b'\xff'])),6:('bool-array',[False,True],counted([b'\0',b'\1'])),
              7:('int64-array',['-1','9223372036854775807'],counted([struct.pack('<q',n) for n in (-1,9223372036854775807)])),
              8:('float64-array',[1.25],counted([struct.pack('<d',1.25)])),9:('string-array',['hello',''],counted([text('hello'),text('')]))}
    for tag,(name,value,payload) in payloads.items():
        add('typed-event-'+name,'typed-parameter-event',{'seconds':1,'nanoseconds':2,'kind':2,'tag':tag,'node':'/demo','name':'value','value':value},typed_event(2,tag,payload),backend=2)
    for tag in range(10):
        add('typed-deletion-tag-'+str(tag),'typed-parameter-event',{'seconds':1,'nanoseconds':2,'kind':3,'tag':tag,'node':'/demo','name':'value','value':None},typed_event(3,tag,b''),backend=2)
    negative = [
        {'codec':'bool','hex':'02','reason':'Bool must be exactly 0 or 1.'},
        {'codec':'bool','hex':'0100','reason':'Reject trailing bytes.'},
        {'codec':'string','hex':'050000006869','reason':'Declared UTF-8 byte length exceeds the remaining buffer.'},
        {'codec':'string','hex':'02000000c0af','reason':'Overlong UTF-8 is invalid.'},
        {'codec':'parameter-float64','hex':struct.pack('<d',float('inf')).hex(),'reason':'Float64 must be finite.'},
        {'codec':'parameter-float64-array','hex':counted([struct.pack('<d',float('nan'))]).hex(),'reason':'Every Float64 array element must be finite.'},
        {'codec':'parameter-bool-array','hex':'0100000002','reason':'Every Bool array element must be 0 or 1.'},
        {'codec':'parameter-string-array','hex':counted([text('a\0b')]).hex(),'reason':'String-array elements are NUL-free.'},
        {'codec':'action-result','hex':struct.pack('<QBI',0,2,0).hex(),'reason':'An owned Goal ID is nonzero.'},
        {'codec':'typed-parameter-event','hex':typed_event(1,0,b'').hex(),'reason':'Deletion-only tag 0 requires deleted kind 3.'},
        {'codec':'typed-parameter-event','hex':typed_event(3,1,b'\1').hex(),'reason':'Deleted events have an empty selected payload.'},
    ]
    return {'wire_version':1,'scope':'Language-neutral positive and structural negative specification fixtures; no Runtime, ROS or language implementation verdict',
            'integer_JSON_contract':'All i64/u64 values are decimal strings; never parse them through IEEE754 Number.',
            'positive':positive,'negative':negative}


def main():
    """Generate intentionally or check that committed golden fixtures match."""
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write',action='store_true')
    args=parser.parse_args();document=vectors();body=json.dumps(document,ensure_ascii=False,indent=2)+'\n'
    target=SDK/'abi/wire-vectors-v1.json'
    if args.write:
        target.parent.mkdir(parents=True,exist_ok=True);target.write_text(body)
    elif target.read_text()!=body:
        raise SystemExit('Golden wire fixture drift; review before regenerating')
    print('PASS wire fixture derivation: '+str(len(document['positive']))+' positive, '+str(len(document['negative']))+' negative')


if __name__=='__main__':
    main()
