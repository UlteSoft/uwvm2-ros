/** @file
 * @brief Executable AssemblyScript typed custom server composition.
 * Four private independently acquired owners activate Echo and Digest server
 * endpoints. Pure handlers accept only decoded typed components; they cannot
 * retire an owner or issue a nested poll. Every nonzero Runtime ticket is kept
 * unchanged in the generated response envelope. No builtin server wire schema
 * is substituted, and only checked basic WASI stdin/stdout drives <=200 passes.
 */
import {NativeApi,Capability,Operation,Status,Reader,Writer} from "../../assemblyscript/ros1/roswasm";
import {Ros1Uwvm_ros_bounded_v2Envelope as Envelope,Ros1Uwvm_ros_bounded_v2EchoRequestCodec as RequestCodec,Ros1Uwvm_ros_bounded_v2EchoResponse as Response,Ros1Uwvm_ros_bounded_v2EchoResponseCodec as ResponseCodec,Ros1Uwvm_ros_bounded_v2DigestGoalCodec as GoalCodec,Ros1Uwvm_ros_bounded_v2DigestFeedback as Feedback,Ros1Uwvm_ros_bounded_v2DigestFeedbackCodec as FeedbackCodec,Ros1Uwvm_ros_bounded_v2DigestResult as ResultValue,Ros1Uwvm_ros_bounded_v2DigestResultCodec as ResultCodec} from "../generated-bounded-v2/bounded_records";
export {abortGuest} from "../../assemblyscript/api";
@external("wasi_snapshot_preview1","fd_read")declare function fdRead(fd:u32,iovs:u32,count:u32,slot:u32):u32;
@external("wasi_snapshot_preview1","fd_write")declare function fdWrite(fd:u32,iovs:u32,count:u32,slot:u32):u32;
@external("wasi_snapshot_preview1","proc_exit")declare function procExit(status:u32):void;
function gate(expected:u8=82):u32{const b=new Uint8Array(1),v=new Uint32Array(2),n=new Uint32Array(1);v[0]=<u32>b.dataStart;v[1]=1;return fdRead(0,<u32>v.dataStart,1,<u32>n.dataStart)!=0||n[0]!=1||b[0]!=expected?Status.Invalid:0;}
function marker(text:string):u32{const b=Uint8Array.wrap(String.UTF8.encode(text)),v=new Uint32Array(2),n=new Uint32Array(1);v[0]=<u32>b.dataStart;v[1]=<u32>b.length;return fdWrite(1,<u32>v.dataStart,1,<u32>n.dataStart)!=0||n[0]!=<u32>b.length?Status.Disconnected:0;}
/** Full independent semantic value check follows the complete generated decoder. */
function same(v:Envelope):bool{return v.label=="bounded-native"&&v.samples.length==3&&v.samples[0]==3&&v.samples[1]==5&&v.samples[2]==8&&v.points.length==2&&v.points[0].value==-7&&v.points[0].enabled&&v.points[1].value==11&&!v.points[1].enabled&&v.labels.length==2&&v.labels[0]=="first"&&v.labels[1]=="second";}
/** Encode before import; the Runtime owns this ID's alias/deadline/terminal state. */
function frame(id:u64,kind:u8,action:bool,body:Uint8Array):Uint8Array{const w=new Writer(body.length+(action?9:8));w.u64(id);if(action)w.u8(kind);w.bytes(body);return w.finish();}
function respond(cap:Capability,operation:Operation,bytes:Uint8Array):u32{const r=cap.request(operation,bytes,0);return r.status?r.status:r.bytes.length?Status.Invalid:0;}
let last:u32=0;
function execute(caps:Array<Capability>):u32{
 let code=caps[0].advertiseService();if(code)return code;code=respond(caps[2],Operation.action_advertise,new Uint8Array(0));if(code)return code;code=marker("SDK_BOUNDED_SERVERS_READY\n");if(code)return code;let services=0,goals=0;
 for(let pass=0;pass<200;++pass){
  code=gate();if(code)return code;
  if(!services){const r=caps[0].poll(RequestCodec.maxWireBytes+8);if(r.status!=Status.Again){if(r.status)return r.status;if(r.bytes.length<8)return Status.Invalid;const id=new Reader(r.bytes).u64(),body=r.bytes.slice(8),codec=new RequestCodec();if(!id||!codec.valid(body))return Status.Invalid;const request=codec.decode(body);if(request.limit!=32||!same(request.sample))return Status.Invalid;const response=new Response();response.accepted=true;response.echoed=request.sample;if(!response.validValue())return Status.Invalid;const wire=frame(id,0,false,new ResponseCodec().encode(response));code=respond(caps[1],Operation.service_respond,wire);if(code)return code;++services;}}
  if(!goals){const r=caps[2].poll(GoalCodec.maxWireBytes+8);if(r.status!=Status.Again){if(r.status)return r.status;if(r.bytes.length<8)return Status.Invalid;const id=new Reader(r.bytes).u64(),body=r.bytes.slice(8),codec=new GoalCodec();if(!id||!codec.valid(body))return Status.Invalid;const goal=codec.decode(body);if(!same(goal.sample))return Status.Invalid;const feedback=new Feedback();feedback.processed=3;feedback.phase="accepted";const result=new ResultValue();result.accepted=true;result.sum=16;result.note="bounded-native-result";if(!feedback.validValue()||!result.validValue())return Status.Invalid;const fb=frame(id,1,true,new FeedbackCodec().encode(feedback)),rp=frame(id,2,true,new ResultCodec().encode(result));code=respond(caps[3],Operation.action_respond,fb);if(code)return code;
   // The controller requires independent native feedback receipt before E.
   // All owners and the exact encoded terminal body stay live during this gate.
   code=marker("SDK_BOUNDED_FEEDBACK_SENT\n");if(code)return code;
   code=gate(69);if(code)return code;
   code=respond(caps[3],Operation.action_respond,rp);if(code)return code;++goals;}}
  if(services==1&&goals==1){code=marker("SDK_BOUNDED_REPLIES_SENT\n");return code?code:gate(68);}code=marker("SDK_BOUNDED_AGAIN\n");if(code)return code;
 }
 return Status.Timeout;
}
/** Runnable selected-backend entry, with reverse checked cleanup on every normal path. */
export function _start():void{
 const api=new NativeApi(),caps=new Array<Capability>();if(api.version()!=1){last=Status.Unsupported;procExit(last);return;}
 const aliases:Array<string>=["bounded-service-events","bounded-service-replies","bounded-action-events","bounded-action-replies"];
 for(let i=0;i<aliases.length;++i){const cap=new Capability(api);last=cap.open(aliases[i]);if(last)break;caps.push(cap);}
 if(!last)last=execute(caps);for(let i=caps.length-1;i>=0;--i){const code=caps[i].close();if(!last)last=code;}
 if(!last)last=marker("PASS_BOUNDED_CUSTOM_SERVER\n");procExit(last);
}
export function lastStatus():u32{return last;}
