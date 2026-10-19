/**
 * @file Runnable JavaScript typed custom server composition.
 * Four independent explicit role owners cover Echo advertise/respond and Digest
 * advertise/respond. Generated component codecs have no builtin Bool/String or
 * Fibonacci assumption. The private polling lease spans decode, handler and
 * response, refusing direct Capability/Node retirement before any import. Basic
 * gate/marker adapters are supplied by the actual relay controller; no hidden
 * native clock, network, environment or file permission is acquired.
 */
import {RosError} from '../../javascript/roswasm.mjs';
import {AsyncSession} from '../../javascript/async.mjs';
import {acquirePolling} from '../../javascript/ownership.mjs';
import * as bounded from '../generated-bounded-v2/bounded_records.mjs';
function require(value){if(!value)throw new RosError(22,'custom_server_validation');}
function frame(id,kind,body){require(typeof id==='bigint'&&id!==0n);const prefix=kind===null?8:9,out=new Uint8Array(prefix+body.length);new DataView(out.buffer).setBigUint64(0,id,true);if(kind!==null)out[8]=kind;out.set(body,prefix);return out;}
function same(value){require(value.label==='bounded-native'&&value.samples.length===3&&value.samples.every((x,i)=>x===[3,5,8][i])&&value.points.length===2&&value.points[0].value===-7&&value.points[0].enabled===true&&value.points[1].value===11&&value.points[1].enabled===false&&value.labels[0]==='first'&&value.labels[1]==='second');}
/** Execute native-to-Guest typed handlers and return0 only after checked cleanup. */
export async function runCustomServer(session,gate,marker){
  if(!(session instanceof AsyncSession)||typeof gate!=='function'||typeof marker!=='function')throw new TypeError('Actual AsyncSession and checked basic I/O required');
  const prefix=session.version===1?'Ros1Uwvm_ros_bounded_v2':'Ros2Uwvm_ros_bounded_v2',scope=session.version===1?'':'Srv',act=session.version===1?'':'Action',sep=session.version===1?'':'_';
  const request=bounded[`${prefix}${scope}Echo${sep}RequestCodec`],response=bounded[`${prefix}${scope}Echo${sep}ResponseCodec`],goal=bounded[`${prefix}${act}Digest${sep}GoalCodec`],feedback=bounded[`${prefix}${act}Digest${sep}FeedbackCodec`],result=bounded[`${prefix}${act}Digest${sep}ResultCodec`];
  const caps=[];let error=0,services=0,goals=0;
  const print=async text=>{const code=await marker(text);require(Number.isInteger(code)&&code>=0&&code<=0xffffffff);if(code)throw new RosError(code,'basic_stdout');};
  const respond=async (cap,op,bytes)=>{const r=await cap.request(op,bytes);if(r.status)throw new RosError(r.status,op);require(r.bytes.length===0);};
  /** Poll the exact owner internally while retaining the complete two-role lease. */
  const once=async (advertise,replies,decode,handler)=>{const release=acquirePolling([advertise,replies]);if(!release)throw new RosError(17,'custom_server_poll');try{advertise.check();replies.check();const r=await session.poll(advertise.handle);if(r.status===11)return false;if(r.status)throw new RosError(r.status,'event_poll');require(r.bytes.length>=8);const id=new DataView(r.bytes.buffer,r.bytes.byteOffset,8).getBigUint64(0,true);require(id!==0n);await handler(id,decode.decode(r.bytes.subarray(8)));return true;}finally{release();}};
  try{
    for(const alias of ['bounded-service-events','bounded-service-replies','bounded-action-events','bounded-action-replies'])caps.push(await session.open(alias));
    const admitted=await session.handle('service_advertise',caps[0].handle);if(admitted)throw new RosError(admitted,'service_advertise');await respond(caps[2],'action_advertise',new Uint8Array());await print('SDK_BOUNDED_SERVERS_READY\n');
    for(let pass=0;pass<200&&(services!==1||goals!==1);++pass){
      require(await gate()==='R');
      if(!services&&await once(caps[0],caps[1],request,async (id,value)=>{require(value.limit===32);same(value.sample);await respond(caps[1],'service_respond',frame(id,null,response.encode({accepted:true,echoed:value.sample})));}))++services;
      if(!goals&&await once(caps[2],caps[3],goal,async (id,value)=>{same(value.sample);const fb=frame(id,1,feedback.encode({processed:3,phase:'accepted'})),rp=frame(id,2,result.encode({accepted:true,sum:16,note:'bounded-native-result'}));await respond(caps[3],'action_respond',fb);await print('SDK_BOUNDED_FEEDBACK_SENT\n');require(await gate('E')==='E');await respond(caps[3],'action_respond',rp);}))++goals;
      if(services!==1||goals!==1)await print('SDK_BOUNDED_AGAIN\n');
    }
    if(services!==1||goals!==1)throw new RosError(110,'bounded_server_pass_limit');
    await print('SDK_BOUNDED_REPLIES_SENT\n');require(await gate('D')==='D');
  }catch(failure){error=failure instanceof RosError?failure.code:22;}
  finally{for(const cap of caps.reverse()){try{const code=await cap.close();if(!error&&code)error=code;}catch(failure){if(!error)error=failure instanceof RosError?failure.code:22;}}}
  if(!error){const code=await marker('PASS_BOUNDED_CUSTOM_SERVER\n');if(!Number.isInteger(code)||code<0||code>0xffffffff)error=22;else error=code;}return error;
}
