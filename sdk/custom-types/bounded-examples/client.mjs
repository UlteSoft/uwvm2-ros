/**
 * @file Complete JavaScript custom bounded Service/Action execution.
 * The embedding application supplies one actual asynchronous SDK Session backed
 * by its compiled Wasm relay; ordinary objects never become Wasm addresses.
 * This function opens all six explicit aliases, calls real imports, validates
 * exact typed responses and closes every owner. gate()/marker() must perform
 * checked basic stdin/stdout via the relay; no host/network timer is invented.
 * It is an executable entry function, not a codec-only or no-op demonstration.
 * Native delivery and transport identity are separately qualified by acceptance.
 */
import {RosError} from '../../javascript/roswasm.mjs';
import {AsyncSession} from '../../javascript/async.mjs';
import * as bounded from '../generated-bounded-v2/bounded_records.mjs';
function require(value){if(!value)throw new RosError(22,'custom_bounded_validation');}
const sample=()=>({label:'bounded-native',samples:[3,5,8],points:[{value:-7,enabled:true},{value:11,enabled:false}],labels:['first','second']});
function sameSample(value){require(value.label==='bounded-native'&&value.samples.length===3&&value.samples.every((x,i)=>x===[3,5,8][i])&&value.points.length===2&&value.points[0].value===-7&&value.points[0].enabled===true&&value.points[1].value===11&&value.points[1].enabled===false&&value.labels.length===2&&value.labels[0]==='first'&&value.labels[1]==='second');}
/** Run the complete bounded application. All callbacks execute outside scratch borrow.
 * @param {AsyncSession} session Actual already instantiated relay and reserved scratch.
 * @param {(expected?:string)=>string} gate Checked R passes and one delivery E gate.
 * @param {(text:string)=>number} marker Checked basic stdout returning numeric errno.
 * @returns {number} Zero only after typed completion and checked cleanup.
 */
export async function runCustomClient(session,gate,marker){
  if(!(session instanceof AsyncSession)||typeof gate!=='function'||typeof marker!=='function')throw new TypeError('Actual AsyncSession and checked I/O adapters required');
  const prefix=session.version===1?'Ros1Uwvm_ros_bounded_v2':'Ros2Uwvm_ros_bounded_v2';
  const scope=session.version===1?'':'Srv',actionScope=session.version===1?'':'Action';
  const part=session.version===1?'':'_';
  const request=bounded[`${prefix}${scope}Echo${part}RequestCodec`],response=bounded[`${prefix}${scope}Echo${part}ResponseCodec`];
  const emptyRequest=bounded[`${prefix}${scope}Empty${part}RequestCodec`],emptyResponse=bounded[`${prefix}${scope}Empty${part}ResponseCodec`];
  const goal=bounded[`${prefix}${actionScope}Digest${part}GoalCodec`],feedback=bounded[`${prefix}${actionScope}Digest${part}FeedbackCodec`],result=bounded[`${prefix}${actionScope}Digest${part}ResultCodec`];
  const caps=[];let error=0;const checkedMarker=async text=>{const code=await marker(text);if(!Number.isInteger(code)||code<0||code>0xffffffff)throw new RosError(22,'invalid_marker_status');if(code)throw new RosError(code,'basic_stdout');};
  const call=async (cap,operation,bytes)=>{const r=await cap.request(operation,bytes);if(r.status)throw new RosError(r.status,operation);return r.bytes;};
  try{
    for(const alias of ['bounded-echo','bounded-empty','bounded-send','bounded-cancel','bounded-feedback','bounded-result'])caps.push(await session.open(alias));
    const answer=response.decode(await call(caps[0],'service_call',request.encode({sample:sample(),limit:32})));require(answer.accepted===true);sameSample(answer.echoed);
    emptyResponse.decode(await call(caps[1],'service_call',emptyRequest.encode({})));
    const id=await call(caps[2],'action_send_goal',goal.encode({sample:sample()}));require(id.length===8&&new DataView(id.buffer,id.byteOffset,8).getBigUint64(0,true)!==0n);await checkedMarker('SDK_BOUNDED_GOAL_SENT\n');
    let gotFeedback=false,gotResult=false;
    for(let pass=0;pass<200&&!gotResult;++pass){
      require(await gate()==='R');
      if(!gotFeedback){const r=await caps[4].request('action_receive_feedback',id);if(r.status===11){await checkedMarker('SDK_BOUNDED_AGAIN\n');continue;}if(r.status)throw new RosError(r.status,'action_receive_feedback');require(r.bytes.length>=8&&r.bytes.subarray(0,8).every((b,i)=>b===id[i]));const value=feedback.decode(r.bytes.subarray(8));require(value.processed===3&&value.phase==='accepted');gotFeedback=true;await checkedMarker('SDK_BOUNDED_FEEDBACK_DELIVERED\n');require(await gate('E')==='E');}
      const r=await caps[5].request('action_receive_result',id);if(r.status===11){await checkedMarker('SDK_BOUNDED_AGAIN\n');continue;}if(r.status)throw new RosError(r.status,'action_receive_result');require(r.bytes.length>=9&&r.bytes.subarray(0,8).every((b,i)=>b===id[i])&&r.bytes[8]===2);const value=result.decode(r.bytes.subarray(9));require(value.accepted===true&&value.sum===16&&value.note==='bounded-native-result');gotResult=true;
    }
    if(!gotFeedback||!gotResult)throw new RosError(110,'bounded_poll_budget');
  }catch(failure){error=failure instanceof RosError?failure.code:22;}
  finally{for(const cap of caps.reverse()){try{const code=await cap.close();if(!error&&code)error=code;}catch(failure){if(!error)error=failure instanceof RosError?failure.code:22;}}}
  if(!error){const code=await marker('PASS_BOUNDED_CUSTOM_CLIENT\n');if(!Number.isInteger(code)||code<0||code>0xffffffff)error=22;else if(code)error=code;}return error;
}
