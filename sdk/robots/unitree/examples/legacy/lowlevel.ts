/** Actual ROS1 AssemblyScript SDK owners and complete generated records.
 * No capturing callback, fake native handle, native ROS or hidden poll loop.
 * The command's actual main calls these functions within the same Wasm module.
 * Raw typed Subscription.spinOnce releases its lease before cleanup occurs.
 */
import {Node,NativeApi,Publisher,Subscription,Capability} from '../../../../assemblyscript/ros1/roswasm';
import {Ros1Unitree_legged_msgsLowCmd as Command,Ros1Unitree_legged_msgsLowState as State,
 Ros1Unitree_legged_msgsLowCmdCodec as CommandCodec,Ros1Unitree_legged_msgsLowStateCodec as StateCodec} from '../../../../custom-types/legacy-family/bounded_records';
import {expectedCommand,expectedState} from './values';
const node=new Node(new NativeApi(),'legacy-typed-example');
const stateCodec=new StateCodec(),commandCodec=new CommandCodec();
let publisher:Publisher<Command>|null=null,subscription:Subscription<State>|null=null;
let started:bool=false,closed:bool=false,submitted:bool=false;
let status:u32=0,polls:u32=0,received:u32=0,denials:u32=0;
let observed=new Uint8Array(0);
function closeAll(first:u32=0):u32 {
 if(closed)return first?first:9;closed=true;
 // Node owns both handles. Its close attempts every live owner after poll has
 // returned, including when an earlier individual SDK transaction failed.
 const results=node.close();for(let i=0;i<results.length;++i)if(!first&&results[i])first=results[i];
 publisher=null;subscription=null;status=first;return first;
}
function onState(value:State):void {
 if(received!=0){status=22;return;}const actual=stateCodec.encode(value),expected=stateCodec.encode(expectedState());
 if(actual.length==0||actual.length>1536||actual.length!=expected.length){status=22;return;}
 for(let i=0;i<actual.length;++i)if(actual[i]!=expected[i]){status=22;return;}observed=actual;received=1;
}
export function initialize_case(mode:u32,family:u32,reserved:u32):u32 {
 if(started||closed)return 9;started=true;if(mode>1||family!=4||reserved)return closeAll(22);
 if(node.api.version()!=1)return closeAll(95);
 if(mode==0){
  const a=node.open('legacy-lowcmd');if(a.openStatus!=13||a.isOpen())return closeAll(a.openStatus?a.openStatus:22);++denials;
  const b=node.open('legacy-lowstate');if(b.openStatus!=13||b.isOpen())return closeAll(b.openStatus?b.openStatus:22);++denials;return closeAll();
 }
 const pub=node.createPublisher<Command>('legacy-lowcmd',commandCodec);publisher=pub;
 if(pub.capability.openStatus)return closeAll(pub.capability.openStatus);
 // This constructor invokes actual topic_subscribe before READY/native M.
 const sub=node.createSubscription<State>('legacy-lowstate',stateCodec,onState,1536);subscription=sub;
 if(sub.status)return closeAll(sub.status);return 0;
}
export function poll_once():u32 {
 if(!started||closed||received!=0)return 9;if(polls++>=4096)return closeAll(110);
 const sub=subscription;if(!sub)return closeAll(9);const code=sub.spinOnce();
 if(code==11)return 11;if(code)return closeAll(code);if(status)return closeAll(status);return received==1?0:closeAll(22);
}
export function submit_once():u32 {
 if(!started||closed||received!=1||submitted)return 9;const pub=publisher;if(!pub)return closeAll(9);
 const code=pub.publish(expectedCommand());if(code)return closeAll(code);submitted=true;return 0;
}
export function observed_length():u32{return received?<u32>observed.length:0;}
export function observed_byte(index:u32):u32{if(!received||index>=<u32>observed.length){status=22;return 0;}return observed[<i32>index];}
export function received_count():u32{return received;}
export function denied_count():u32{return denials;}
export function case_status():u32{return status;}
export function close_case():u32{return closeAll();}
