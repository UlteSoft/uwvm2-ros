#!/usr/bin/env python3
"""Compile an inert, path-free, single-node communication worker manifest.

This grants nothing and starts no worker. Its binary is independently pinned as
the sandbox's NX configuration image. The C++ decoder repeats every validation.
"""
import argparse,hashlib,json
from pathlib import Path
import make_communication_definition as core

NETWORK_FIELDS=('generation','domain','local_participant','peer_participant','local_discovery','local_user','peer_discovery','peer_user',
 'local_discovery_source','local_user_source','peer_source','local_prefix','peer_prefix','allowed','user_data_receive','user_data_transmit',
 'allow_uncertain_attenuation','frame_deadline_ns','max_datagram_bytes','max_sample_bytes','max_submessages','socket_buffer_bytes','queues')
CAPACITY_FIELDS=('contexts','nodes','types','topics','services','actions','graphs','streams','queue_bytes','definition_bytes','observers')
QUOTA_FIELDS=('packets','bytes','attenuated','packets_per_second','bytes_per_second')
MAXIMUM_TYPES=1024
MAXIMUM_DEFINITIONS=2+MAXIMUM_TYPES+64+32+16+8
def boolean(value):
 if type(value) is not bool:raise ValueError('Expected an explicit boolean')
 return core.u64(int(value))
def prefix(value):
 if not isinstance(value,str) or len(value)!=24 or any(x not in '0123456789abcdef' for x in value):raise ValueError('Expected 12-byte lowercase hexadecimal prefix')
 result=bytes.fromhex(value)
 if not any(result):raise ValueError('Zero participant prefix')
 return result
def network(p):
 core.exact(p,NETWORK_FIELDS)
 for key in NETWORK_FIELDS[:11]:core.unsigned(p[key],(1<<64)-2 if key=='generation' else 65535)
 if not p['generation'] or p['domain']>232 or p['local_participant']>119 or p['peer_participant']>119 or p['local_participant']==p['peer_participant']:raise ValueError('Invalid domain/participant')
 local=7400+250*p['domain']+10+2*p['local_participant'];peer=7400+250*p['domain']+10+2*p['peer_participant']
 if max(local,peer)+1>65535 or (p['local_discovery'],p['local_user'],p['peer_discovery'],p['peer_user'])!=(local,local+1,peer,peer+1):raise ValueError('Expected exact standard unicast ports')
 ports=[p[k] for k in NETWORK_FIELDS[4:11]]
 if min(ports)<1024 or len(set(ports))!=7:raise ValueError('Duplicate or privileged port')
 local_prefix,peer_prefix=prefix(p['local_prefix']),prefix(p['peer_prefix'])
 if local_prefix[:8]==peer_prefix[:8]:raise ValueError('Participant prefixes must not imply same-process routing')
 allowed=p['allowed']
 if not isinstance(allowed,list) or len(allowed)!=4 or not all(type(v) is bool for v in allowed) or not all(allowed[:2]):raise ValueError('Four explicit directions and reserved discovery required')
 if type(p['user_data_receive']) is not bool or type(p['user_data_transmit']) is not bool or type(p['allow_uncertain_attenuation']) is not bool:raise ValueError('Explicit direction booleans required')
 if p['user_data_receive'] and not allowed[2] or p['user_data_transmit'] and not allowed[3]:raise ValueError('Data needs its pipe right')
 if not 1<=core.unsigned(p['frame_deadline_ns'],60000000000):raise ValueError('Finite frame deadline required')
 maximum=core.unsigned(p['max_datagram_bytes'],65000)
 if maximum<64 or not maximum<=core.unsigned(p['max_sample_bytes'],1<<26) or not 1<=core.unsigned(p['max_submessages'],1024) or core.unsigned(p['socket_buffer_bytes'],1<<24)<2*maximum:raise ValueError('Invalid finite network capacities')
 queues=p['queues']
 if not isinstance(queues,list) or len(queues)!=4:raise ValueError('Four separate queues required')
 encoded=b''.join(core.u64(p[k]) for k in NETWORK_FIELDS[:11])+core.blob(local_prefix)+core.blob(peer_prefix)+b''.join(boolean(x) for x in allowed)
 encoded+=b''.join(boolean(p[k]) for k in NETWORK_FIELDS[14:17])+b''.join(core.u64(p[k]) for k in NETWORK_FIELDS[17:22])
 for i,q in enumerate(queues):
  core.exact(q,QUOTA_FIELDS);boolean(q['attenuated'])
  for key in ('packets','bytes','packets_per_second','bytes_per_second'):core.unsigned(q[key])
  if not allowed[i]:
   if any(q.values()):raise ValueError('Absent direction cannot reserve or attenuate')
  elif not 1<=q['packets']<=65536 or q['bytes']>1<<28 or q['bytes']<q['packets']*maximum:raise ValueError('Insufficient queue reservation')
  if q['attenuated'] and not p['allow_uncertain_attenuation']:raise ValueError('Attenuation needs explicit uncertainty acceptance')
  if not q['attenuated'] and (q['packets_per_second'] or q['bytes_per_second']):raise ValueError('Absent attenuation uses no implicit rates')
  encoded+=core.u64(q['packets'])+core.u64(q['bytes'])+boolean(q['attenuated'])+core.u64(q['packets_per_second'])+core.u64(q['bytes_per_second'])
 return encoded
def compile_definition(document):
 core.exact(document,('network','capacity','request_bytes','response_bytes','definitions'));body=core.text('uwvm.ros.communication.worker')+network(document['network'])
 capacity=document['capacity'];core.exact(capacity,CAPACITY_FIELDS)
 for key,maximum in zip(CAPACITY_FIELDS,(1,1,MAXIMUM_TYPES,64,32,16,8,0,1<<30,1<<24,65536)):core.unsigned(capacity[key],maximum)
 if capacity['contexts']!=1 or capacity['nodes']!=1 or not capacity['definition_bytes']:raise ValueError('Exactly one context and node required')
 body+=b''.join(core.u64(capacity[k]) for k in CAPACITY_FIELDS)
 for key in ('request_bytes','response_bytes'):
  if not 1<=core.unsigned(document[key],4*1024*1024):raise ValueError('Finite RPC byte bound required')
  body+=core.u64(document[key])
 definitions=document['definitions']
 if not isinstance(definitions,list) or not 2<=len(definitions)<=MAXIMUM_DEFINITIONS:raise ValueError('Finite owned definitions required')
 if len(definitions)>sum(capacity[k] for k in CAPACITY_FIELDS[:7]):raise ValueError('Definition count exceeds explicit family capacities')
 body+=core.u64(len(definitions));metadata=[];profiles={};totals={k:0 for k in CAPACITY_FIELDS}
 for definition in definitions:
  compiled,meta=core.compile_definition(definition);kind=definition['family']
  if kind=='event':raise ValueError('Events remain host-local')
  key=(meta['identity'],meta['profile_sha256'])
  if key in profiles:raise ValueError('Duplicate semantic profile')
  profiles[key]=definition;metadata.append(meta);totals[kind+'s' if kind!='graph' else 'graphs']+=1
  limits=definition['limits'];totals['definition_bytes']+=len(compiled);totals['queue_bytes']+=limits.get('result.bytes',0) if kind=='graph' else limits.get('queue.bytes',0);totals['observers']+=limits.get('observers',0)
  body+=core.u64(core.FAMILIES.index(kind))+core.u64(2 if kind=='type' else 1)+core.profile({'identity':meta['identity'],'sha256':meta['profile_sha256']})+core.u64(len(limits))
  for key,value in limits.items():body+=core.text(key)+core.u64(value)
  body+=core.blob(compiled)
 if any(totals[k]>capacity[k] for k in CAPACITY_FIELDS):raise ValueError('Aggregate capacity exceeded')
 contexts=[d for d in definitions if d['family']=='context'];nodes=[d for d in definitions if d['family']=='node']
 if len(contexts)!=1 or len(nodes)!=1 or contexts[0]['definition']['domain']!=document['network']['domain']:raise ValueError('One matching context/node required')
 def dependency(ref,kind):
  core.profile(ref);found=profiles.get((ref['identity'],ref['sha256']))
  if not found or found['family']!=kind:raise ValueError('Missing exact profile dependency')
  return found
 if dependency(nodes[0]['definition']['context'],'context') is not contexts[0]:raise ValueError('Wrong node context')
 for d in definitions:
  p=d['definition'];kind=d['family']
  if kind in ('topic','service','action','graph') and dependency(p['node'],'node') is not nodes[0]:raise ValueError('Wrong endpoint node')
  if kind=='topic':
   schema=dependency(p['type'],'type')['definition']
   if schema['codec'] not in ('std-string','std-bool') and not(schema['codec']=='bounded-message'and schema['schema']['layout']=='cdr'):raise ValueError('Unsupported native topic representation')
  if kind=='service':
   for part in ('request','response'):
    if dependency(p[part+'_type'],'type')['definition']['codec']!='set-bool-'+part:raise ValueError('Wrong service schema')
  if kind=='action':
   for part in ('goal','feedback','result'):
    if dependency(p[part]['profile'],'type')['definition']['codec']!=p[part]['codec']:raise ValueError('Wrong action schema')
  if kind=='graph' and dependency(p['context'],'context') is not contexts[0]:raise ValueError('Wrong graph context')
 if len(body)>16*1024*1024:raise ValueError('Manifest exceeds profile capacity')
 return body,{'file_bytes':len(body),'sha256':hashlib.sha256(body).hexdigest(),'definitions':metadata,'totals':totals,'scope':'inert single-node manifest; no worker activation or authority'}
def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--input',type=Path,required=True);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
 raw=args.input.read_bytes()
 if len(raw)>16*1024*1024:raise ValueError('JSON too large')
 document=json.loads(raw,object_pairs_hook=core.unique,parse_constant=lambda _:(_ for _ in ()).throw(ValueError('Nonfinite JSON')))
 output,metadata=compile_definition(document);args.output.parent.mkdir(parents=True,exist_ok=True)
 with args.output.open('xb') as f:f.write(output)
 with args.output.with_suffix(args.output.suffix+'.json').open('x') as f:json.dump(metadata,f,indent=2);f.write('\n')
 print(json.dumps(metadata,sort_keys=True))
if __name__=='__main__':main()
