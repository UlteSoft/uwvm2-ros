#!/usr/bin/env python3
"""Generate independent complete nonzero canonical vectors from original declarations.

This test tool does not import a codec generator or use generated field IR. It
reads unchanged original message/service/action bodies, fills every field using
a deterministic field-path seed, and packs reference bytes with Python's explicit
little-endian struct format. Synthetic values test wire representation, not
physical command validity, original native identity or official-suite acceptance.
All output is test source. No ROS/native operation or permission grant occurs.
"""
import argparse,hashlib,json,re,struct,zlib
from pathlib import Path


def sections(raw,v,kind):
 text=raw.decode()
 if v==1 and kind=='srv':
  before=[];after=[];target=before
  for line in text.split('\n'):
   if line.startswith('---'):target=after
   else:target.append(line+'\n')
  return [''.join(before),''.join(after)]
 return re.split(r'^\s*---\s*(?:#[^\n]*)?$',text,flags=re.MULTILINE)

SCALARS={'bool','byte','char','int8','uint8','int16','uint16','int32','uint32','int64','uint64','float32','float64','string'}
def build(source,profile,out,batch_size=32):
 source=Path(source).resolve(strict=True);out=Path(out)
 assert out.is_absolute() and not out.exists() and 1<=batch_size<=64
 root=source/'sdk/origin/robots/arms/ufactory/xarm/catalogs'/profile
 catalog=json.loads((root/'catalog.json').read_text());version=1 if profile.startswith('ros1_') else 2
 records={};pins={}
 def resolve(name,package):
  if name in SCALARS or name in ('time','duration'):return name
  if name=='Header':return 'std_msgs/Header'
  bits=name.split('/')
  if len(bits)==1:return package+('/msg/' if version==2 else '/')+name
  if len(bits)==2 and version==2:return bits[0]+'/msg/'+bits[1]
  return name
 for row in catalog['types']:
  path=root/row['file'];raw=path.read_bytes();pins[row['file']]=dict(bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest())
  roles={'.msg':['message'],'.srv':['request','response'],'.action':['goal','result','feedback']}[path.suffix]
  parts=sections(raw,version,path.parent.name);assert len(parts)==len(roles)
  for role,section in zip(roles,parts):
   name=row['type'] if role=='message' else row['type']+('_' if version==2 else '')+role.title()
   assert name not in records
   fields=[]
   for line in section.splitlines():
    line=line.split('#',1)[0].strip()
    if not line or re.match(r'^[A-Za-z0-9_]+\s+[A-Za-z0-9_]+\s*=',line):continue
    match=re.fullmatch(r'([A-Za-z0-9_/]+)(?:<=([0-9]+))?(?:\[([0-9]*|<=[0-9]+)\])?\s+([A-Za-z0-9_]+)(?:\s+.*)?',line);assert match,(name,line)
    typ,native_string,array,field=match.groups();quota=row['bounds'].get(('' if role=='message' else role+'.')+field,{})
    bound=quota.get('string',int(native_string) if native_string else 0)
    fields.append(dict(type=resolve(typ,name.split('/')[0]),array=array,name=field,string=bound))
   records[name]=fields
 if version==1:
  needed={field['type'] for fields in records.values() for field in fields if field['type'] in ('time','duration')}
  for name in needed:records[name]=[dict(type='uint32' if name=='time' else 'int32',array=None,name=field,string=0) for field in ('sec','nsec')]
 cases=[];total_fields=0
 def scalar(typ,path,field):
  seed=zlib.crc32(path.encode())&0xffffffff
  if typ=='byte':typ='int8' if version==1 else 'uint8'
  if typ=='char':typ='uint8'
  if typ=='bool':return 'true' if seed&1 else 'false',bytes([seed&1])
  if typ.startswith(('uint','int')):
   bits=int(typ[4:] if typ.startswith('uint') else typ[3:]);signed=typ.startswith('int')
   value=((seed<<32)|(seed^0x9e3779b9)) if bits==64 else seed&((1<<bits)-1)
   if signed and value>=(1<<(bits-1)):value-=1<<bits
   if field in ('nsec','nanosec'):value=seed%1000000000
   if bits==64:
    if signed:literal=('(-INT64_C(9223372036854775807)-1)' if value==-(1<<63) else ('-INT64_C('+str(-value)+')' if value<0 else 'INT64_C('+str(value)+')'))
    else:literal='UINT64_C('+str(value)+')'
   else:literal=str(value)+('u' if not signed else '')
   return literal,int(value).to_bytes(bits//8,'little',signed=signed)
  assert typ in ('float32','float64'),typ
  value=((seed%4093)-2046)/8.0
  literal=repr(value)+('f' if typ=='float32' else '')
  return literal,struct.pack('<f' if typ=='float32' else '<d',value)
 def fill(name,expression,path,lines):
  nonlocal total_fields
  wire=bytearray()
  assert name in records,name
  for field in records[name]:
   typ=field['type'];field_name=field['name'];target=expression+'.'+field_name;field_path=path+'.'+field_name;array=field['array']
   count=1 if array is None or not array or array.startswith('<=') else int(array)
   if array is not None and (not array or array.startswith('<=')):
    lines.append(target+'.size=1;');wire.extend(struct.pack('<I',1));target+='.data'
   for index in range(count):
    item=target+('['+str(index)+']' if array is not None else '')
    item_path=field_path+('['+str(index)+']' if array is not None else '')
    if typ=='string':
     seed=zlib.crc32(item_path.encode())&0xffffffff
     data=b'v'+format(seed,'08x')[:4].encode()+b'\xce\xb2'
     if len(data)>field['string']:data=format(seed,'08x').encode()[:field['string']]
     assert data and len(data)<=field['string']
     quoted='"'+''.join('\\x%02x'%byte for byte in data)+'"'
     lines.extend([item+'.size='+str(len(data))+';', 'memcpy('+item+'.data,'+quoted+','+str(len(data))+');'])
     wire.extend(struct.pack('<I',len(data)));wire.extend(data);total_fields+=1
    elif typ in SCALARS:
     literal,data=scalar(typ,item_path,field_name);lines.append(item+'='+literal+';');wire.extend(data);total_fields+=1
    else:wire.extend(fill(typ,item,item_path,lines))
  return wire
 out.mkdir(mode=0o700);batch=[];products=[]
 def flush():
  if not batch:return
  index=len(products);name='golden-%03d.c'%index
  body='/* Independent original-declaration vectors; never physical robot commands. */\n#include <assert.h>\n#include <stdint.h>\n#include <stdbool.h>\n#include <string.h>\n#include "bounded_records.h"\n'
  body+='\n'.join(case['code'] for case in batch)
  body+='\nint main(void){'+''.join('test_'+str(case['index'])+'();' for case in batch)+'return 0;}\n'
  p=out/name;p.write_text(body);products.append(dict(file=name,bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest(),records=[case['type'] for case in batch]));batch.clear()
 for index,name in enumerate(sorted(records)):
  lines=[];wire=bytes(fill(name,'value',name+'#nonzero1',lines));assert len(wire)<=32768,(name,len(wire))
  prefix='roswasmc_bounded_'+str(version)+'_'+name.replace('/','_')
  expected=','.join('0x%02x'%byte for byte in wire) or '0'
  code='static void test_'+str(index)+'(void){\n'+prefix+' value={0},decoded={0};\nunsigned char bytes[32768],again[32768];size_t used=0,second=0;\nstatic unsigned char const expected[]={'+expected+'};\n'+'\n'.join(lines)+'\n'
  code+='assert('+prefix+'_encode(&value,bytes,sizeof bytes,&used)==0);assert(used=='+str(len(wire))+');assert(!memcmp(bytes,expected,used));\n'
  code+='assert('+prefix+'_decode(expected,'+str(len(wire))+',&decoded)==0);assert('+prefix+'_encode(&decoded,again,sizeof again,&second)==0);assert(second==used&&!memcmp(again,expected,used));\n}\n'
  row=dict(index=index,type=name,wire_bytes=len(wire),wire_sha256=hashlib.sha256(wire).hexdigest());cases.append(row);batch.append(dict(row,code=code))
  if len(batch)==batch_size:flush()
 flush()
 report=dict(kind='ufactory-xarm-independent-original-nonzero-vectors-v1',profile=profile,ros_version=version,records=len(cases),scalar_or_string_field_instances=total_fields,cases=cases,products=products,source_pins=pins,default_authority=[],scope='Independently packed original-field canonical vectors only; source generation, not execution, native identity, physical validity or official acceptance.')
 (out/'vectors.json').write_text(json.dumps(report,indent=2)+'\n');return report
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--source',type=Path,required=True);parser.add_argument('--profile',choices=['ros1_kinetic', 'ros1_legacy', 'ros1_master', 'ros2_foxy', 'ros2_galactic', 'ros2_humble_gz', 'ros2_humble', 'ros2_jazzy', 'ros2_master', 'ros2_rolling'],required=True);parser.add_argument('--out',type=Path,required=True);parser.add_argument('--batch-size',type=int,default=32)
 args=parser.parse_args();report=build(args.source,args.profile,args.out,args.batch_size)
 print(json.dumps({k:report[k] for k in ['profile','records','scalar_or_string_field_instances']},indent=2))
