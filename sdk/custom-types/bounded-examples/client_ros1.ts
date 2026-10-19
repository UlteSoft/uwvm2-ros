/** @file
 * @brief Complete AssemblyScript custom bounded Service/Action execution.
 * Select this backend-specific file only; no cross-version fallback is compiled.
 * Generated typed components use the existing Capability/NativeApi imports.
 * Direct checked WASI stdio supplies externally paced R polls (at most200);
 * no WASI clock, randomness, environment, file or socket grant is requested.
 * Compile/inspect/validate and real native delivery are unified acceptance steps.
 */
import {NativeApi,Capability,Operation,Status,Reader} from "../../assemblyscript/ros1/roswasm";
import {Ros1Uwvm_ros_bounded_v2Envelope as Envelope,Ros1Uwvm_ros_bounded_v2Point as Point,Ros1Uwvm_ros_bounded_v2EchoRequest as Request,Ros1Uwvm_ros_bounded_v2EchoResponseCodec as ResponseCodec,Ros1Uwvm_ros_bounded_v2EmptyRequest as EmptyRequest,Ros1Uwvm_ros_bounded_v2EmptyRequestCodec as EmptyRequestCodec,Ros1Uwvm_ros_bounded_v2EmptyResponseCodec as EmptyResponseCodec,Ros1Uwvm_ros_bounded_v2DigestGoal as Goal,Ros1Uwvm_ros_bounded_v2DigestGoalCodec as GoalCodec,Ros1Uwvm_ros_bounded_v2EchoRequestCodec as RequestCodec,Ros1Uwvm_ros_bounded_v2DigestFeedbackCodec as FeedbackCodec,Ros1Uwvm_ros_bounded_v2DigestResultCodec as ResultCodec} from "../generated-bounded-v2/bounded_records";
export {abortGuest} from "../../assemblyscript/api";
@external("wasi_snapshot_preview1","fd_read") declare function fdRead(fd:u32,iovs:u32,count:u32,slot:u32):u32;
@external("wasi_snapshot_preview1","fd_write") declare function fdWrite(fd:u32,iovs:u32,count:u32,slot:u32):u32;
@external("wasi_snapshot_preview1","proc_exit") declare function procExit(status:u32):void;
/** Checked exact one-byte reads distinguish EOF, short output and raw ABI refusal. */
function gate(expected:u8=82):u32{const byte=new Uint8Array(1),iovs=new Uint32Array(2),slot=new Uint32Array(1);iovs[0]=<u32>byte.dataStart;iovs[1]=1;slot[0]=0;if(fdRead(0,<u32>iovs.dataStart,1,<u32>slot.dataStart)!=0||slot[0]!=1||byte[0]!=expected)return Status.Invalid;return 0;}
/** Check every syscall, including short writes; keep all managed arrays rooted. */
function marker(text:string):u32{const bytes=Uint8Array.wrap(String.UTF8.encode(text)),iovs=new Uint32Array(2),slot=new Uint32Array(1);iovs[0]=<u32>bytes.dataStart;iovs[1]=<u32>bytes.length;slot[0]=0;return fdWrite(1,<u32>iovs.dataStart,1,<u32>slot.dataStart)!=0||slot[0]!=<u32>bytes.length?Status.Disconnected:0;}
function sample():Envelope{const v=new Envelope();v.label="bounded-native";v.samples.push(3);v.samples.push(5);v.samples.push(8);const first=new Point();first.value=-7;first.enabled=true;const second=new Point();second.value=11;second.enabled=false;v.points.push(first);v.points.push(second);v.labels[0]="first";v.labels[1]="second";return v;}
function same(v:Envelope):bool{return v.label=="bounded-native"&&v.samples.length==3&&v.samples[0]==3&&v.samples[1]==5&&v.samples[2]==8&&v.points.length==2&&v.points[0].value==-7&&v.points[0].enabled&&v.points[1].value==11&&!v.points[1].enabled&&v.labels.length==2&&v.labels[0]=="first"&&v.labels[1]=="second";}
let last:u32=0;
/** Execute the real import flow once, retaining every acquired owner until cleanup. */
function execute(caps:Array<Capability>):u32{
 const request=new Request();request.sample=sample();request.limit=32;
 let reply=caps[0].request(Operation.service_call,new RequestCodec().encode(request),ResponseCodec.maxWireBytes);if(reply.status)return reply.status;
 const responseCodec=new ResponseCodec();if(!responseCodec.valid(reply.bytes))return Status.Invalid;const response=responseCodec.decode(reply.bytes);if(!response.accepted||!same(response.echoed))return Status.Invalid;
 reply=caps[1].request(Operation.service_call,new EmptyRequestCodec().encode(new EmptyRequest()),0);if(reply.status)return reply.status;if(!new EmptyResponseCodec().valid(reply.bytes))return Status.Invalid;
 const goal=new Goal();goal.sample=sample();reply=caps[2].request(Operation.action_send_goal,new GoalCodec().encode(goal),8);if(reply.status)return reply.status;if(reply.bytes.length!=8)return Status.Invalid;
 const idBytes=reply.bytes,id=new Reader(idBytes).u64();if(!id)return Status.Invalid;let code=marker("SDK_BOUNDED_GOAL_SENT\n");if(code)return code;let gotFeedback=false;
 for(let pass=0;pass<200;++pass){
  code=gate();if(code)return code;
  if(!gotFeedback){reply=caps[4].request(Operation.action_receive_feedback,idBytes,FeedbackCodec.maxWireBytes+8);if(reply.status==Status.Again){code=marker("SDK_BOUNDED_AGAIN\n");if(code)return code;continue;}if(reply.status)return reply.status;if(reply.bytes.length<8||new Reader(reply.bytes).u64()!=id)return Status.Invalid;const body=reply.bytes.slice(8),codec=new FeedbackCodec();if(!codec.valid(body))return Status.Invalid;const value=codec.decode(body);if(value.processed!=3||value.phase!="accepted")return Status.Invalid;gotFeedback=true;
   // The marker follows complete ID/component/semantic validation. Exactly
   // one E gate then permits terminal delivery on its independent ROS channel.
   code=marker("SDK_BOUNDED_FEEDBACK_DELIVERED\n");if(code)return code;
   code=gate(69);if(code)return code;
  }
  reply=caps[5].request(Operation.action_receive_result,idBytes,ResultCodec.maxWireBytes+9);if(reply.status==Status.Again){code=marker("SDK_BOUNDED_AGAIN\n");if(code)return code;continue;}if(reply.status)return reply.status;if(reply.bytes.length<9||new Reader(reply.bytes).u64()!=id||reply.bytes[8]!=2)return Status.Invalid;const body=reply.bytes.slice(9),codec=new ResultCodec();if(!codec.valid(body))return Status.Invalid;const value=codec.decode(body);if(!value.accepted||value.sum!=16||value.note!="bounded-native-result")return Status.Invalid;return 0;
 }
 return Status.Timeout;
}
/** Runnable Wasm entry. Cleanup proceeds in reverse even when a prior close fails. */
export function _start():void{
 const api=new NativeApi(),caps=new Array<Capability>();if(api.version()!=1){last=Status.Unsupported;procExit(last);return;}
 const aliases:Array<string>=["bounded-echo","bounded-empty","bounded-send","bounded-cancel","bounded-feedback","bounded-result"];
 for(let i=0;i<aliases.length;++i){const cap=new Capability(api);last=cap.open(aliases[i]);if(last)break;caps.push(cap);}
 if(!last)last=execute(caps);
 for(let i=caps.length-1;i>=0;--i){const code=caps[i].close();if(!last)last=code;}
 if(!last)last=marker("PASS_BOUNDED_CUSTOM_CLIENT\n");procExit(last);
}
/** Exact final status for embedders; zero requires operation and cleanup success. */
export function lastStatus():u32{return last;}
