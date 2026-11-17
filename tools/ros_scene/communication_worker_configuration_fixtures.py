#!/usr/bin/env python3
"""Build a complete single-node manifest plus independent rejected JSON cases."""
import argparse,copy,json
from pathlib import Path
import make_communication_definition as definition
import make_communication_worker_definition as worker
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path,required=True);args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=False)
 entries=[]
 def add(kind,name,semantic,limits):
  doc={'family':kind,'provider':'canonical-schema' if kind=='type' else 'ros2-loopback-worker','identity':name,'definition':semantic,'limits':{'definition.bytes':4096,**limits}}
  _,meta=definition.compile_definition(doc);entries.append(doc);return {'identity':name,'sha256':meta['profile_sha256']}
 context=add('context','context',{'domain':194,'enclave':'','transport':'unauthenticated-loopback'},{'instances':1})
 node=add('node','node',{'context':context,'name':'robot','namespace':'/scene'},{'instances':1,'observers':8})
 types={codec:add('type',codec,{'codec':codec},{'descriptor.bytes':4096}) for codec in definition.CODECS}
 q={'history':'keep-last','reliability':'reliable','durability':'volatile','liveliness':'automatic','depth':2,'deadline':{'seconds':9223372036,'nanoseconds':854775807},'lifespan':{'seconds':9223372036,'nanoseconds':854775807},'lease':{'seconds':9223372036,'nanoseconds':854775807}}
 for role in ('publisher','subscription'):
  add('topic',role,{'node':node,'type':types['std-string'],'name':'/scene/'+role,'role':role,'qos':q,'overflow':'reject'},
   {'instances':1,'message.bytes':1024,'queue.items':0 if role=='publisher' else 4,'queue.bytes':0 if role=='publisher' else 4096,'deadline.ns':1000000000})
 for role in ('client','server'):
  add('service','service-'+role,{'node':node,'request_type':types['set-bool-request'],'response_type':types['set-bool-response'],'name':'/scene/service_'+role,'type':'std_srvs/srv/SetBool','role':role,'qos':q},
   {'instances':1,'pending':2,'request.bytes':1024,'response.bytes':1024,'queue.bytes':4096,'deadline.ns':1000000000})
  add('action','action-'+role,{'node':node,**{k:{'profile':types['lookup-transform-'+k],'codec':'lookup-transform-'+k} for k in ('goal','feedback','result')},'name':'/scene/action_'+role,'type':'tf2_msgs/action/LookupTransform','role':role,'qos':{k:({**q,'depth':1,'durability':'transient-local'} if k=='status' else q) for k in ('goal','result','cancel','feedback','status')}},
   {'instances':1,'goals':2,'cancels_per_goal':2,'feedback.items':2,'goal.bytes':1024,'feedback.bytes':0,'result.bytes':1024,'queue.bytes':65536,'deadline.ns':1000000000,'retention.ns':1000000000,'transitions':510})
 add('graph','graph',{'context':context,'node':node,'nodes':['/scene/robot'],'fields':63},{'instances':1,'rows':8,'page.items':2,'result.bytes':4096,'cursor.ns':1000000000,'capture.ns':1000000000})
 p={'generation':735,'domain':194,'local_participant':0,'peer_participant':1,'local_discovery':55910,'local_user':55911,'peer_discovery':55912,'peer_user':55913,'local_discovery_source':60120,'local_user_source':60121,'peer_source':60122,'local_prefix':'010f030405060708090a0b01','peer_prefix':'010f080706050403090a0b02','allowed':[True]*4,'user_data_receive':True,'user_data_transmit':True,'allow_uncertain_attenuation':False,'frame_deadline_ns':5000000000,'max_datagram_bytes':4096,'max_sample_bytes':65536,'max_submessages':128,'socket_buffer_bytes':262144,'queues':[{'packets':128,'bytes':524288,'attenuated':False,'packets_per_second':0,'bytes_per_second':0} for _ in range(4)]}
 document={'network':p,'capacity':dict(zip(worker.CAPACITY_FIELDS,(1,1,7,2,2,2,1,0,1048576,65536,8))),'request_bytes':65536,'response_bytes':65536,'definitions':entries}
 raw,metadata=worker.compile_definition(document);(args.output/'worker.json').write_text(json.dumps(document,indent=2)+'\n');(args.output/'worker.bin').write_bytes(raw);(args.output/'worker.metadata.json').write_text(json.dumps(metadata,indent=2)+'\n')
 # The catalogue is an inert set of owned schemas, not a registration of native
 # topic factories. The maximum crosses both old codec-count and manifest-count
 # restrictions while keeping all queue/endpoint reservations at explicit zero.
 def catalogue(count):
  result=copy.deepcopy(document);result['definitions']=copy.deepcopy(entries[:2])
  result['capacity']=dict(zip(worker.CAPACITY_FIELDS,(1,1,count,0,0,0,0,0,0,16*1024*1024,8)))
  schema={'layout':'cdr','message_bytes':1024,'work':128,'depth':3,'nodes':[
   {'kind':'float64','bound':0,'flags':['finite'],'members':[]},
   {'kind':'sequence','bound':16,'flags':[],'members':[{'name':'element','type':0}]},
   {'kind':'record','bound':0,'flags':[],'members':[{'name':'positions','type':1}]}]}
  for index in range(count):
   result['definitions'].append({'family':'type','provider':'canonical-schema','identity':'bounded-'+str(index).zfill(4),
    'definition':{'codec':'bounded-message','type':'catalogue_msgs/msg/Position'+str(index).zfill(4),'schema':schema},
    'limits':{'definition.bytes':4096,'descriptor.bytes':4096}})
  return result
 for count in (0,8,129,worker.MAXIMUM_TYPES):
  value=catalogue(count);binary,meta=worker.compile_definition(value)
  (args.output/('catalogue-'+str(count)+'.bin')).write_bytes(binary)
  (args.output/('catalogue-'+str(count)+'.json')).write_text(json.dumps(meta,indent=2)+'\n')
 # Retain an internally consistent duplicate entry on wire. Both profile copies
 # and the hashed semantic body remain valid; only global uniqueness is broken.
 value=catalogue(8);binary,_=worker.compile_definition(value)
 def encoded(entry):
  body,meta=definition.compile_definition(entry);limits=entry['limits']
  return definition.u64(definition.FAMILIES.index(entry['family']))+definition.u64(2)+definition.profile({'identity':meta['identity'],'sha256':meta['profile_sha256']})+definition.u64(len(limits))+b''.join(definition.text(k)+definition.u64(v) for k,v in limits.items())+definition.blob(body)
 first,last=encoded(value['definitions'][2]),encoded(value['definitions'][-1]);assert len(first)==len(last) and binary.endswith(last)
 (args.output/'catalogue-duplicate.bin').write_bytes(binary[:-len(last)]+first)
 invalid=[]
 def bad():
  result=copy.deepcopy(document);invalid.append(result);return result
 bad()['native_path']='/tmp/unapproved.so';bad()['network']['native_fd']=5;bad()['network']['peer_prefix']=p['local_prefix']
 bad()['network']['peer_source']=p['local_user'];bad()['network']['allowed'][0]=False;bad()['network']['queues'][1]['attenuated']=True
 bad()['network']['frame_deadline_ns']=0;bad()['network']['queues'][0]['bytes']=4096;bad()['network']['user_data_receive']=1
 bad()['capacity']['nodes']=0;bad()['capacity']['queue_bytes']=0;bad()['definitions'].append(copy.deepcopy(entries[1]));bad()['definitions'][1]['definition']['context']['sha256']='22'*32
 bad()['definitions'][9]['definition']['type']=types['lookup-transform-goal'];bad()['response_bytes']=2**64-1;del bad()['network']['socket_buffer_bytes']
 invalid.append(catalogue(worker.MAXIMUM_TYPES+1))
 small=catalogue(8);small['capacity']['types']=7;invalid.append(small)
 duplicate=catalogue(8);duplicate['definitions'][-1]=copy.deepcopy(duplicate['definitions'][2]);invalid.append(duplicate)
 unsupported=catalogue(8);unsupported['definitions'][2]['definition']['schema']['layout']='ros1';_,meta=definition.compile_definition(unsupported['definitions'][2]);endpoint=copy.deepcopy(entries[9])
 endpoint['definition']['type']={'identity':meta['identity'],'sha256':meta['profile_sha256']};unsupported['definitions'].append(endpoint);unsupported['capacity']['topics']=1;invalid.append(unsupported)
 for entry in invalid:
  try:worker.compile_definition(entry)
  except ValueError:pass
  else:raise AssertionError('invalid worker definition accepted')
 print('passed complete single-node worker manifest and '+str(len(invalid))+' closed JSON rejection cases')
if __name__=='__main__':main()
