#!/usr/bin/env python3
"""DATA-only association of a real independent producer report and SDK receipt.

No candidate/Native/generator import or invocation. This does not authenticate
an arbitrary JSON receipt: Root must independently source-peer/own the actual
producer+same original SDK+MD/ELF/maps report. Header output is trusted explicit
build DATA, never a Guest input or a claimed Native qualification.
"""
import argparse,hashlib,json,os,pathlib,re,stat
LIMIT=67108864
def opened(path,limit):
 p=pathlib.Path(path).absolute();fd=os.open(p,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
 try:
  first=os.fstat(fd)
  if not stat.S_ISREG(first.st_mode)or first.st_size>limit:raise ValueError('bounded regular input required')
  parts=[];n=0
  while True:
   b=os.read(fd,min(1048576,limit-n+1))
   if not b:break
   n+=len(b)
   if n>limit:raise ValueError('input grew beyond bound')
   parts.append(b)
  last=os.fstat(fd)
  keys=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns,s.st_mode,s.st_nlink)
  if keys(first)!=keys(last)or n!=first.st_size:raise ValueError('input drift/extent mismatch')
 finally:os.close(fd)
 return b''.join(parts)
def unique(pairs):
 d={}
 for k,v in pairs:
  if k in d:raise ValueError('duplicate JSON key')
  d[k]=v
 return d
def constant_refused(value):raise ValueError('nonfinite JSON constant refused')
def main():
 p=argparse.ArgumentParser();p.add_argument('--actual-native-export',required=True);p.add_argument('--actual-sdk-receipt',required=True);p.add_argument('--output',required=True);a=p.parse_args()
 raw=opened(a.actual_native_export,LIMIT);sdk=opened(a.actual_sdk_receipt,16777216)
 # Preserve the WHOLE original SDK evidence as an independent material; no
 # boolean such as native_verified can upgrade an unowned source fixture.
 if not isinstance(json.loads(sdk,object_pairs_hook=unique,parse_constant=constant_refused),dict):raise ValueError('actual independently reviewed SDK receipt object required')
 d=json.loads(raw,object_pairs_hook=unique,parse_constant=constant_refused)
 if set(d)!={'format_version','kind','services'}or type(d['format_version'])is not int or d['format_version']!=1 or d['kind']!='actual-original-native-custom-service-export' or not isinstance(d['services'],list)or not 1<=len(d['services'])<=128:raise ValueError('complete actual export schema required')
 digest=hashlib.sha256(raw).hexdigest();sdkhash=hashlib.sha256(sdk).hexdigest();lines=['/* Actual independently owned native metadata export DATA; no default IDs. */','#pragma once','#include "custom_owned_service_exports.h"','#include <cstddef>','#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>','namespace uwvm2::ros_gateway::custom_owned_services {'];seen=set()
 for index,s in enumerate(d['services']):
  if set(s)!={'type','records'}or not re.fullmatch(r'[a-z][a-z0-9_]*/srv/[A-Za-z][A-Za-z0-9_]*',s['type'])or s['type']in seen:raise ValueError('unique original typed service required')
  seen.add(s['type']);rows=s['records']
  if not isinstance(rows,list)or not 1<=len(rows)<=256:raise ValueError('complete original protocol graph required')
  entries=[];layouts=[];names=set();total=0;fields_total=0
  for row in rows:
   if set(row)!={'type','native_identity','encoding','source_sha256','source_hex','members'}or not re.fullmatch(r'[a-z][a-z0-9_]*/(?:msg|srv)/[A-Za-z][A-Za-z0-9_]*',row['type'])or row['type']in names:raise ValueError('unique complete native row required')
   names.add(row['type'])
   if not re.fullmatch(r'RIHS01_[0-9a-f]{64}',row['native_identity'])or row['encoding']not in ('msg','srv','implicit')or not re.fullmatch(r'[0-9a-f]{64}',row['source_sha256'])or not re.fullmatch(r'(?:[0-9a-f]{2})*',row['source_hex']):raise ValueError('actual hash/encoding/source bytes required')
   source=bytes.fromhex(row['source_hex']);total+=len(source)
   if len(source)>4194304 or total>16777216 or hashlib.sha256(source).hexdigest()!=row['source_sha256']:raise ValueError('full actual source extent/hash mismatch')
   entries.append('{'+','.join(json.dumps(row[k])for k in ('type','native_identity','encoding','source_sha256'))+'}')
   members=row['members']
   if row['type']==s['type']:
    if members is not None:raise ValueError('Service virtual root has no Native object member projection')
   else:
    if not isinstance(members,list)or len(members)>4096 or len(set(members))!=len(members)or any(not isinstance(f,str)or not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*',f)for f in members):raise ValueError('complete typed Message member names required')
    fields_total+=len(members)
    if fields_total>16384:raise ValueError('complete typed layout field bound')
    native_type='::'+row['type'].replace('/','::')
    test=['if(name=='+json.dumps(row['type'])+'){using Native='+native_type+';if(!m||m->size_of_!=sizeof(Native)||m->member_count_!='+str(len(members))+')return false;']
    if members:test+=['if(!m->members_)return false;']
    for position,field in enumerate(members):test+=['if(m->members_['+str(position)+'].offset_!=offsetof(Native,'+field+'))return false;']
    test+=['return true;}'];layouts.append(''.join(test))
  if s['type']not in names or s['type']+'_Event'not in names or 'service_msgs/msg/ServiceEventInfo'not in names or 'builtin_interfaces/msg/Time'not in names:raise ValueError('complete actual root/Event/Info/Time export required')
  native='::'+s['type'].replace('/','::');lines+=['template<>struct independent_native_exports<'+native+'> {','inline static constexpr exported_identity records[]={'+','.join(entries)+'};','static bool layout(std::string_view name,void const* raw)noexcept{auto m=static_cast<rosidl_typesupport_introspection_cpp::MessageMembers const*>(raw);'+''.join(layouts)+'return false;}','static export_view get()noexcept{return {records,sizeof(records)/sizeof(records[0]),"'+digest+'","'+sdkhash+'",&layout};}','};']
 lines+=['}'];out=('\n'.join(lines)+'\n').encode();q=pathlib.Path(a.output);fd=os.open(q,os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW,0o400)
 try:
  cursor=0
  while cursor<len(out):
   n=os.write(fd,out[cursor:])
   if n<=0:raise OSError('short header write')
   cursor+=n
  os.fsync(fd)
 finally:os.close(fd)
 print(json.dumps({'kind':'source-DATA-header-association-not-native-qualification','bytes':len(out),'sha256':hashlib.sha256(out).hexdigest(),'original_export_sha256':digest,'original_sdk_receipt_sha256':sdkhash}))
if __name__=='__main__':main()
