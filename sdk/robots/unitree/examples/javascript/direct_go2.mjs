/** @file Real embedded-JavaScript standalone Go2 ROS2 simulator command.
 *
 * Compile this main and its complete generated GO24 bounded_records.mjs into
 * the pinned QuickJS-in-Wasm engine. --unitree-direct-enabled 0|1 selects the
 * Guest-local C computation module; it supplies no permissions. Normal builds
 * do not register that optional module. Host Node and a fabricated transport
 * cannot execute or qualify this entry. No application runs at module load.
 *
 * Only the existing real Guest SDK opens/polls/publishes ROS. The optional C
 * module computes the exact authoritative shared kernel and official CRC in
 * this SAME Wasm, returning copied bytes. It owns no ROS handle and calls no
 * native endpoint. There is no copied JS trajectory or host math substitute.
 * The trusted supervisor selects the actual Go2 model, loopback/domain1 and
 * independent native state/command witnesses. Never use on physical hardware.
 */
import { createGuestSession, Node, Executor, readGuestByte, writeGuestOutput } from '../../../../javascript/guest.mjs';
import { enabled, modular as modularEnabled, command as computeCommand } from 'roswasm:unitree_control';
import { Ros2Unitree_goMsgLowCmdCodec as Command, Ros2Unitree_goMsgLowStateCodec as State } from './bounded_records.mjs';

function code(error){return Number.isInteger(error?.code)?error.code:typeof error==='number'?error:22;}
function write(text){const status=writeGuestOutput(new TextEncoder().encode(text));if(status)throw status;}
function read(count){const result=new Uint8Array(count);for(let i=0;i<count;++i)result[i]=readGuestByte();return result;}
function bits(value){const buffer=new ArrayBuffer(4),view=new DataView(buffer);view.setFloat32(0,value,true);return view.getUint32(0,true);}
/** Single owned main: finite stdin poll gates, one genuine LowState per step,
 * complete generated decode/encode and checked retirement after callbacks
 * unwind. Every error preserves its original status while attempting cleanup.
 */
export function main(){
  let node=null,executor=null,first=0;
  try{
    const session=createGuestSession();if(session.version!==2)throw 95;
    node=new Node(session,'unitree-direct-javascript');executor=new Executor();
    if(enabled()===0){
      for(const alias of ['unitree-lowcmd','unitree-lowstate']){
        let denied=false;
        try{const owner=node.open(alias);if(owner)throw 22;}catch(error){if(code(error)!==13)throw error;denied=true;}
        if(!denied)throw 22;
      }
      const closed=node.close();node=null;for(const status of closed)if(status)throw status;
      executor.close();executor=null;write('PASS_UNITREE_DIRECT_DEFAULT_DENIED\n');return 0;
    }
    const setup=read(12),header=new DataView(setup.buffer),count=header.getUint32(4,true),dt=header.getUint32(8,true);
    if(String.fromCharCode(...setup.subarray(0,4))!=='UCK1'||count<1||count>32||dt<100000||dt>20000000)throw 22;
    if(Command.maxWireBytes>32768||State.maxWireBytes>32768||session.outputCapacity<State.maxWireBytes)throw 90;
    const useModules=modularEnabled()===1;
    const publisher=node.createPublisher('unitree-lowcmd',Command);let step=0,polls=0;
    const subscription=node.createSubscription('unitree-lowstate',State,state=>{
      if(step>=count||state.tick!==step+1||state.motor_state.length!==20)throw 22;
      const words=[];
      for(const motor of state.motor_state.slice(0,12))for(const field of ['q','dq','tau_est']){
        if(!Number.isFinite(motor[field]))throw 22;words.push(bits(motor[field]));
      }
      // Allocate the copied perception envelope only in the explicit modular
      // build. Standalone keeps its original local-kernel command shape.
      let packed;
      if(useModules){
        const observed=new Uint8Array(144),observedView=new DataView(observed.buffer);
        words.forEach((word,index)=>observedView.setUint32(4*index,word,true));
        packed=computeCommand(step+1,dt,observed);
      }else packed=computeCommand(step+1,dt);
      if(!(packed instanceof Uint8Array)||packed.length!==484)throw 22;
      const view=new DataView(packed.buffer,packed.byteOffset,packed.byteLength),message=Command.defaultValue();
      message.head[0]=0xfe;message.head[1]=0xef;message.level_flag=0xff;message.crc=view.getUint32(0,true);
      if(message.motor_cmd.length!==20)throw 22;
      for(let i=0;i<20;++i){const at=4+24*i,motor=message.motor_cmd[i];motor.mode=view.getUint32(at,true);
        if(motor.mode>255)throw 22;for(const [field,offset] of [['q',4],['dq',8],['kp',12],['kd',16],['tau',20]]){
          motor[field]=view.getFloat32(at+offset,true);if(!Number.isFinite(motor[field]))throw 22;
        }
      }
      const status=publisher.publish(message);if(status)throw status;++step;
      write(`UNITREE_STEP ${step} ${state.tick} ${message.crc} ${state.crc}\nUNITREE_STATE ${step} ${state.tick} ${words.join(' ')}\n`);
    });
    executor.add(subscription);write('READY_UNITREE_DIRECT_V1\n');
    while(step<count&&polls++<4096){
      if(readGuestByte()!==82)throw 22;const before=step;
      for(const status of executor.spinOnce())if(status&&status!==11)throw status;
      if(step===before)write('UNITREE_AGAIN\n');else if(step!==before+1)throw 22;
    }
    if(step!==count)throw 110;if(readGuestByte()!==81)throw 22;
    executor.close();executor=null;
    const statuses=node.close();node=null;for(const status of statuses)if(status)throw status;
    write(`PASS_UNITREE_DIRECT steps=${step}\n`);
  }catch(error){first=code(error)||22;}
  // No garbage-collector finalizer performs ROS cleanup. Attempt both owners
  // even after an earlier failure, retaining that first failure unchanged.
  try{if(executor)executor.close();}catch(error){if(!first)first=code(error)||22;}
  try{if(node)for(const status of node.close())if(!first&&status)first=status;}catch(error){if(!first)first=code(error)||22;}
  return first;
}
