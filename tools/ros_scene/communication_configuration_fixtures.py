#!/usr/bin/env python3
"""Independent JSON inputs and malformed-input checks for all closed communication definitions."""
import argparse,copy,json
from pathlib import Path
import make_communication_definition as compiler

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path,required=True);args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=False)
 reference=lambda name:{'identity':name,'sha256':'11'*32}
 q={'history':'keep-last','reliability':'reliable','durability':'volatile','liveliness':'automatic','depth':2,'deadline':{'seconds':0,'nanoseconds':0},'lifespan':{'seconds':0,'nanoseconds':0},'lease':{'seconds':0,'nanoseconds':0}}
 definitions={
  'context':{'domain':12,'enclave':'','transport':'unauthenticated-loopback'},
  'node':{'context':reference('ctx'),'name':'robot','namespace':'/scene'},
  'type':{'codec':'std-string'},
  'topic':{'node':reference('node'),'type':reference('message'),'name':'/scene/data','role':'subscription','qos':q,'overflow':'reject'},
  'service':{'node':reference('node'),'request_type':reference('request'),'response_type':reference('response'),'name':'/scene/service','type':'std_srvs/srv/SetBool','role':'client','qos':q},
  'action':{'node':reference('node'),'goal':{'profile':reference('goal'),'codec':'lookup-transform-goal'},'feedback':{'profile':reference('feedback'),'codec':'lookup-transform-feedback'},'result':{'profile':reference('result'),'codec':'lookup-transform-result'},'name':'/scene/action','type':'tf2_msgs/action/LookupTransform','role':'server','qos':{k:q for k in ('goal','result','cancel','feedback','status')}},
  'graph':{'context':reference('ctx'),'node':reference('node'),'nodes':['/scene/robot'],'fields':63},
  'event':{'fields':3},
 }
 capacities={'definition.bytes':4096,'instances':1,'observers':4,'descriptor.bytes':4096,'message.bytes':128,'queue.items':2,'queue.bytes':8192,'deadline.ns':1000000000,'pending':2,'request.bytes':128,'response.bytes':128,'goals':2,'cancels_per_goal':2,'feedback.items':1,'goal.bytes':128,'feedback.bytes':0,'result.bytes':4096,'transitions':510,'retention.ns':1000000000,'rows':8,'page.items':1,'cursor.ns':1000000000,'capture.ns':1000000000,'streams':2,'sources':1,'events':2,'workspace.bytes':4096}
 denied=0
 for fixture in (*compiler.FAMILIES,'bounded-cdr','bounded-ros1'):
  kind='type' if fixture.startswith('bounded-') else fixture
  index=compiler.FAMILIES.index(kind)
  provider='canonical-schema' if kind=='type' else 'local-event-stream' if kind=='event' else 'ros2-loopback-worker'
  limits={key:capacities[key] for key,(families,_) in compiler.LIMITS.items() if kind in families}
  if kind=='action':limits['result.bytes']=128
  doc={'family':kind,'provider':provider,'identity':'qualification-'+kind,'limits':limits,'definition':definitions[kind]}
  if fixture.startswith('bounded-'):
   doc['definition']={'codec':'bounded-message','type':'qualification_msgs/msg/Position','schema':{
    'layout':fixture[8:],'message_bytes':128,'work':8,'depth':3,'nodes':[
     {'kind':'float64','bound':0,'flags':['finite'],'members':[]},
     {'kind':'array','bound':3,'flags':[],'members':[{'name':'element','type':0}]},
     {'kind':'record','bound':0,'flags':[],'members':[{'name':'position','type':1}]}]}}
  output,metadata=compiler.compile_definition(doc)
  (args.output/(fixture+'.json')).write_text(json.dumps(doc,indent=2)+'\n');(args.output/(fixture+'.bin')).write_bytes(output)
  (args.output/(fixture+'.metadata.json')).write_text(json.dumps(metadata,indent=2)+'\n')
  provider_id={'ros2-loopback-worker':1,'canonical-schema':2,'local-event-stream':3}[provider]
  selected={'identity':doc['identity'],'sha256':metadata['profile_sha256']}
  bundle=compiler.u64(index)+compiler.u64(provider_id)+compiler.profile(selected)+compiler.u64(len(limits))
  for key,value in limits.items():bundle+=compiler.text(key)+compiler.u64(value)
  bundle+=compiler.blob(output);(args.output/(fixture+'.bundle')).write_bytes(bundle)
  mutations=[]
  wrong=copy.deepcopy(doc);wrong['native_path']='/tmp/untrusted.so';mutations.append(wrong)
  wrong=copy.deepcopy(doc);wrong['provider']='native-dlopen';mutations.append(wrong)
  wrong=copy.deepcopy(doc);del wrong['limits']['definition.bytes'];mutations.append(wrong)
  wrong=copy.deepcopy(doc);wrong['limits']['definition.bytes']=True;mutations.append(wrong)
  wrong=copy.deepcopy(doc);wrong['limits']['definition.bytes']=2**64-1;mutations.append(wrong)
  for wrong in mutations:
   try:compiler.compile_definition(wrong)
   except ValueError:denied+=1
   else:raise AssertionError('Malformed configuration accepted')
 try:json.loads('{"identity":"a","identity":"b"}',object_pairs_hook=compiler.unique)
 except ValueError:denied+=1
 else:raise AssertionError('Duplicate JSON field accepted')
 assert denied==51
 print('passed JSON compilation for 8 families, 2 bounded layouts and 51 closed-configuration rejection cases')
if __name__=='__main__':main()
