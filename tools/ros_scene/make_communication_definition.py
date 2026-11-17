#!/usr/bin/env python3
"""Compile an explicit communication deployment JSON into inert canonical owned bytes.

This never starts a worker or grants a capability. Definitions carry exact
semantic profile and whole-file hashes separately; no native path or dlopen
selection is accepted. Every family-specific capacity must appear explicitly.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

MAGIC='uwvm.ros.communication.definition'
FAMILIES=('context','node','type','topic','service','action','graph','event')
LIMITS={
 'definition.bytes':(FAMILIES,4*1024*1024),
 'instances':(('context','node','topic','service','action','graph'),1),
 'observers':(('node',),65536),'descriptor.bytes':(('type',),4*1024*1024),
 'message.bytes':(('topic',),16*1024*1024),'queue.items':(('topic',),65536),
 'queue.bytes':(('topic','service','action'),1<<30),'deadline.ns':(('topic','service','action','event'),(1<<63)-1),
 'pending':(('service',),65536),'request.bytes':(('service',),16*1024*1024),'response.bytes':(('service',),16*1024*1024),
 'goals':(('action',),65536),'cancels_per_goal':(('action',),65536),'feedback.items':(('action',),65536),
 'goal.bytes':(('action',),16*1024*1024),'feedback.bytes':(('action',),16*1024*1024),
 'result.bytes':(('action','graph','event'),1<<30),'transitions':(('action',),510),
 'retention.ns':(('action',),(1<<63)-1),'rows':(('graph',),65536),'page.items':(('graph',),65536),
 'cursor.ns':(('graph',),(1<<63)-1),'capture.ns':(('graph',),(1<<63)-1),
 'streams':(('event',),65536),'sources':(('event',),64),'events':(('event',),65536),'workspace.bytes':(('event',),1<<30),
}
CODECS={
 'std-string':('std_msgs/msg/String','text<utf8,no-nul>'),
 'std-bool':('std_msgs/msg/Bool','u8<bool>'),
 'set-bool-request':('std_srvs/srv/SetBool_Request','u8<bool>'),
 'set-bool-response':('std_srvs/srv/SetBool_Response','u8<bool>,text<utf8,no-nul>'),
 'lookup-transform-goal':('tf2_msgs/action/LookupTransform_Goal','target:text,source:text,source_time:(i64<i32>,u64<ns>),timeout:(i64<i32>,u64<ns>),target_time:(i64<i32>,u64<ns>),fixed:text,advanced:u64<bool>'),
 'lookup-transform-feedback':('tf2_msgs/action/LookupTransform_Feedback','empty'),
 'lookup-transform-result':('tf2_msgs/action/LookupTransform_Result','stamp:(i64<i32>,u64<ns>),frame:text,child:text,translation:3*f64<finite>,rotation:4*f64<finite>,error:u64<u8>,message:text'),
}
def exact(value,keys):
 if not isinstance(value,dict) or set(value)!=set(keys):raise ValueError('Expected exactly fields: '+', '.join(keys))
def unsigned(value,maximum=(1<<64)-2):
 if type(value) is not int or value<0 or value>maximum:raise ValueError('Integer outside explicit finite limit')
 return value
def u64(value):return struct.pack('<Q',unsigned(value))
def blob(value):return u64(len(value))+value
def text(value,maximum=255,empty=True):
 if not isinstance(value,str) or '\0' in value or (not empty and not value):raise ValueError('Invalid text')
 encoded=value.encode('utf-8')
 if len(encoded)>maximum:raise ValueError('Text too large')
 return blob(encoded)
def profile(value):
 exact(value,('identity','sha256'));identity=text(value['identity'],empty=False);digest=value['sha256']
 if not isinstance(digest,str) or len(digest)!=64 or any(c not in '0123456789abcdef' for c in digest):raise ValueError('Expected lowercase SHA256')
 decoded=bytes.fromhex(digest)
 if not any(decoded):raise ValueError('Zero profile digest')
 return identity+blob(decoded)
def selected(value,choices):
 if value not in choices:raise ValueError('Unknown closed selection: '+str(value))
 return text(value)
def name(value,namespace=False):
 if not isinstance(value,str) or not value.startswith('/') or (value!='/' and value.endswith('/')):raise ValueError('Absolute ROS name required')
 if value=='/' and namespace:return text(value)
 for part in value[1:].split('/'):
  if not part or not (part[0].isascii() and (part[0].isalpha() or part[0]=='_')) or not all(c.isascii() and (c.isalnum() or c=='_') for c in part):raise ValueError('Invalid ROS name')
 return text(value,empty=False)
def duration(value):
 exact(value,('seconds','nanoseconds'));return u64(unsigned(value['seconds']))+u64(unsigned(value['nanoseconds'],999999999))
def qos(value):
 exact(value,('history','reliability','durability','liveliness','depth','deadline','lifespan','lease'))
 options={'history':('keep-last','keep-all'),'reliability':('reliable','best-effort'),'durability':('transient-local','volatile'),'liveliness':('automatic','manual-by-topic')}
 output=b''
 for key,choices in options.items():
  if value[key] not in choices:raise ValueError('Unknown QoS '+key)
  output+=u64(choices.index(value[key])+1)
 depth=unsigned(value['depth'],65536)
 if not depth:raise ValueError('Explicit finite QoS depth required')
 return output+u64(depth)+b''.join(duration(value[key]) for key in ('deadline','lifespan','lease'))
def type_ref(value,expected=None):
 exact(value,('profile','codec'))
 if value['codec'] not in CODECS or (expected and value['codec']!=expected):raise ValueError('Unsupported exact codec')
 return profile(value['profile'])+text(value['codec'])
def compile_definition(document):
 exact(document,('family','provider','identity','limits','definition'));kind=document['family']
 if kind not in FAMILIES:raise ValueError('Unknown family')
 provider='canonical-schema' if kind=='type' else 'local-event-stream' if kind=='event' else 'ros2-loopback-worker'
 if document['provider']!=provider:raise ValueError('Wrong closed provider selection')
 limits=document['limits'];required=[key for key,(families,_) in LIMITS.items() if kind in families];exact(limits,required)
 for key in required:unsigned(limits[key],LIMITS[key][1])
 if not limits['definition.bytes'] or ('instances' in limits and limits['instances']!=1):raise ValueError('Required positive deployment capacity')
 p=document['definition'];body=text(kind)+text(provider)
 if kind=='context':
  exact(p,('domain','enclave','transport'))
  if p['enclave']!='':raise ValueError('Loopback profile makes no enclave authentication claim')
  body+=u64(unsigned(p['domain'],232))+text(p['enclave'])+selected(p['transport'],('unauthenticated-loopback',))
 elif kind=='node':
  exact(p,('context','name','namespace'));name('/'+p['name'])
  if '/' in p['name']:raise ValueError('Node name cannot include slash')
  body+=profile(p['context'])+text(p['name'],empty=False)+name(p['namespace'],True)
 elif kind=='type':
  codec=p.get('codec') if isinstance(p,dict) else None
  if codec=='bounded-message':
   from bounded_message_definition import compile_schema
   exact(p,('codec','type','schema'));identity=p['type'];descriptor=compile_schema(p['schema'])
   encoding='cdr' if p['schema']['layout']=='cdr' else 'ros1-message'
  else:
   exact(p,('codec',))
   if codec not in CODECS:raise ValueError('Unsupported codec')
   identity,description=CODECS[codec];descriptor=description.encode();encoding='scene-canonical'
  if len(descriptor)>limits['descriptor.bytes']:raise ValueError('Descriptor capacity too small')
  body+=text(codec)+text(identity,empty=False)+text(encoding)+blob(descriptor)
 elif kind=='topic':
  exact(p,('node','type','name','role','qos','overflow'))
  body+=profile(p['node'])+profile(p['type'])+name(p['name'])+selected(p['role'],('publisher','subscription'))+qos(p['qos'])+selected(p['overflow'],('reject','drop-oldest'))
  if not limits['message.bytes'] or not limits['deadline.ns']:raise ValueError('Message and deadline capacities required')
  if p['role']=='publisher':
   if limits['queue.items'] or limits['queue.bytes']:raise ValueError('Publisher has no receive queue')
  elif not limits['queue.items'] or limits['queue.items']*limits['message.bytes']>limits['queue.bytes']:raise ValueError('Insufficient subscription queue reservation')
 elif kind=='service':
  exact(p,('node','request_type','response_type','name','type','role','qos'))
  body+=profile(p['node'])+profile(p['request_type'])+profile(p['response_type'])+name(p['name'])+selected(p['type'],('std_srvs/srv/SetBool',))+selected(p['role'],('client','server'))+qos(p['qos'])
  if not all(limits[k] for k in ('pending','request.bytes','response.bytes','deadline.ns')) or limits['pending']*max(limits['request.bytes'],limits['response.bytes'])>limits['queue.bytes']:raise ValueError('Insufficient service reservation')
 elif kind=='action':
  exact(p,('node','goal','feedback','result','name','type','role','qos'))
  exact(p['qos'],('goal','result','cancel','feedback','status'))
  body+=profile(p['node'])+b''.join(type_ref(p[k],'lookup-transform-'+k) for k in ('goal','feedback','result'))+name(p['name'])+selected(p['type'],('tf2_msgs/action/LookupTransform',))+selected(p['role'],('client','server'))+b''.join(qos(p['qos'][k]) for k in ('goal','result','cancel','feedback','status'))
  if not all(limits[k] for k in ('goals','goal.bytes','result.bytes','deadline.ns','retention.ns')) or limits['transitions']&~510:raise ValueError('Invalid action limits')
  maximum=max(limits[k] for k in ('goal.bytes','feedback.bytes','result.bytes'));maximum=max(maximum,8)
  depth=limits['goals']*(4+2*limits['cancels_per_goal'])+limits['feedback.items']
  if depth*(maximum+48)+limits['goals']*limits['goal.bytes']+2*maximum+48>limits['queue.bytes']:raise ValueError('Insufficient action critical reservation')
 elif kind=='graph':
  exact(p,('context','node','nodes','fields'));nodes=p['nodes']
  if not isinstance(nodes,list) or not nodes or len(nodes)>65536 or nodes!=sorted(set(nodes)):raise ValueError('Graph nodes must be sorted unique exact names')
  fields=unsigned(p['fields'],63)
  if not fields&31:raise ValueError('At least one graph entity field required')
  body+=profile(p['context'])+profile(p['node'])+u64(len(nodes))+b''.join(name(v) for v in nodes)+u64(fields)
  if not all(limits[k] for k in ('rows','result.bytes','page.items','cursor.ns','capture.ns')) or limits['page.items']>limits['rows']:raise ValueError('Invalid graph capacities')
 elif kind=='event':
  exact(p,('fields',));fields=unsigned(p['fields'],3)
  if not fields or not all(limits[k] for k in ('streams','sources','events','result.bytes','workspace.bytes','deadline.ns')) or limits['sources']>limits['events']:raise ValueError('Invalid event capacities')
  body+=u64(fields)
 digest=hashlib.sha256(body).digest();identity=text(document['identity'],empty=False)
 output=text(MAGIC)+identity+blob(digest)+body
 if len(output)>limits['definition.bytes']:raise ValueError('Definition exceeds explicit capacity')
 return output,{'family':kind,'provider':provider,'identity':document['identity'],'profile_sha256':digest.hex(),'file_sha256':hashlib.sha256(output).hexdigest(),'file_bytes':len(output),'limits':limits}
def unique(pairs):
 result={}
 for key,value in pairs:
  if key in result:raise ValueError('Duplicate JSON field: '+key)
  result[key]=value
 return result
def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--input',type=Path,required=True);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
 raw=args.input.read_bytes()
 if len(raw)>4*1024*1024:raise ValueError('Input JSON exceeds bounded definition size')
 document=json.loads(raw.decode('utf-8'),object_pairs_hook=unique,parse_constant=lambda value:(_ for _ in ()).throw(ValueError('Nonfinite JSON number')))
 output,metadata=compile_definition(document);args.output.parent.mkdir(parents=True,exist_ok=True)
 with args.output.open('xb') as stream:stream.write(output)
 metadata_path=args.output.with_suffix(args.output.suffix+'.json')
 with metadata_path.open('x') as stream:json.dump(metadata,stream,indent=2);stream.write('\n')
 print(json.dumps(metadata,sort_keys=True))
if __name__=='__main__':main()
