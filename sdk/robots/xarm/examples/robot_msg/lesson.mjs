/** Runnable copied-Guest JS entry, using the REAL current synchronous SDK.
 * Root supplies a fresh same-Wasm CopiedTransport Session, checked basic stdin
 * gate and stdout marker from the actual JS Guest relay. A host Node model or
 * fabricated binding never qualifies. No authority is inferred from instanceof.
 */
import {Session,Node,RosError} from '../../../../javascript/roswasm.mjs';
import {CopiedTransport} from '../../../../javascript/copied_transport.mjs';
import * as records from '../../../../custom-types/generated-official-standard/bounded_records.mjs';
function vector(value,wanted){return value&&value.length===wanted.length&&wanted.every((x,i)=>value[i]===x);}
function same(v,version){return v&&v.header&&
  (version===1?v.header.seq===19&&v.header.stamp.sec===42&&v.header.stamp.nsec===300000000:v.header.stamp.sec===-2&&v.header.stamp.nanosec===300000000)&&
  v.header.frame_id==='xarm-base'&&v.state===4&&v.mode===0&&v.cmdnum===3&&v.mt_brake===85&&v.mt_able===42&&v.err===7&&v.warn===9&&
  vector(v.angle,[0.0,0.25,-0.5,1.0,-1.25,1.5,-1.75])&&vector(v.pose,[125.5,-250.25,375.0,0.25,-0.5,0.75])&&vector(v.offset,[1.0,2.0,3.0,-0.125,0.25,-0.5]);}
function code(e){return e instanceof RosError?e.code:22;}
export async function runRobotMsg(session,mode,gate,marker){
  if(!(session instanceof Session)||!(session.storage instanceof CopiedTransport)||
      ![1,2].includes(session.version)||(mode!==0&&mode!==1)||typeof gate!=='function'||typeof marker!=='function')
    throw new TypeError('Fresh real copied-Guest Session and exact mode/checked I/O required');
  const codec=session.version===1?records.Ros1Xarm_msgsRobotMsgCodec:records.Ros2Xarm_msgsMsgRobotMsgCodec;
  if(!codec||!Number.isInteger(codec.maxWireBytes)||codec.maxWireBytes<1||codec.maxWireBytes>32768||session.outputCapacity<codec.maxWireBytes)
    throw new TypeError('Actual complete generated codec bound must fit selected Guest scratch');
  const node=new Node(session,'xarm-robot-msg-lesson');let first=0,received=0,denied=false;
  try{
    if(mode===0){
      let unexpected=null;
      try{unexpected=node.open('xarm-robot-msg');first=22;}
      catch(e){denied=e instanceof RosError&&e.code===13&&unexpected===null;first=denied?0:code(e);}
    }else{
      const sub=node.createSubscription('xarm-robot-msg',codec,value=>{if(!same(value,session.version))throw new RosError(22,'full_xarm_robot_msg');++received;});
      for(let pass=0;pass<64&&received===0;++pass){
        if(await gate()!=='R')throw new RosError(22,'caller_poll_credit');
        const status=sub.spinOnce();if(status!==0&&status!==11)throw new RosError(status,'poll');
      }
      if(received!==1)first=110;
    }
  }catch(e){first=code(e);}
  finally{
    // Node.close attempts every acquired capability, including unexpected opens
    // and partial subscribe failure. No native close/reopen is blindly replayed.
    try{for(const status of node.close())if(!first&&status)first=status;}
    catch(e){if(!first)first=code(e);}
  }
  if(!first&&mode===0&&(!denied||received!==0))first=22;
  if(!first){const status=await marker(mode?'PASS_XARM_ROBOT_MSG_FULL_VALUE\n':'PASS_XARM_ROBOT_MSG_DEFAULT_DENIED\n');if(!Number.isInteger(status)||status<0||status>0xffffffff)first=22;else first=status;}
  return {status:first,received,deniedVerified:denied,nativeDeliveryQualified:false};
}
