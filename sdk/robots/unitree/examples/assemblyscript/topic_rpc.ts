// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Role: complete unitree_api Request/Response topic RPC, NOT a ROS service or
// firmware API call. Mode0 opens only unitree-api-request and expects denial.
// Mode1 requires that alias as topic.publish and unitree-api-response as
// topic.subscribe, each with the actual corresponding native message identity.
// After both endpoints match, submit_once publishes exactly one full Request
// with explicit positive int64-compatible ids, parameter="{}" and empty binary.
// The independent SIM fixture must reply with those same ids, status0, exact
// data="typed-example-ack" and empty binary. No physical endpoint is selected.
// Ordered caller protocol: validate the real binary and explicitly initialize
// its selected reactor runtime first; use one fresh instance for mode0 denial
// and another for mode1 positive behavior. Then initialize_case once, explicitly
// submit for topic RPC, poll_once at most4096 times, inspect a received value,
// and close_case after the polling/callback stack has returned. No retry loop,
// clock, thread, goroutine or native endpoint is created by this source itself.
// A successful factory or codec operation does not prove native transport.
// The trusted external Runtime policy supplies rights; this code grants none.
// Status convention:0 means the documented operation succeeded;11 is an idle
// poll;9 rejects unused/closed/repeated lifecycle operations;22 rejects invalid
// configuration or message data;90 is a capacity bound;95 rejects the selected
// ABI when checked;110 exhausts the finite poll budget. Original SDK errors are
// preserved. Rust can additionally return17 for a conflicting state borrow.
// Expected mode0 alias denial13 is translated to0 ONLY with denied_verified=1.
// All acquired owners are explicitly retired on ordinary completion/failure,
// attempting later closes after an earlier close error. Traps/panics/kills still
// require the external process owner to finish actual Runtime/Gateway cleanup.
// AssemblyScript build note: the generated complete family resolves through
// sdk/custom-types/typed-family/bounded_records in the owned stage. Bind the
// actual SDK abortGuest and call the actual _initialize export explicitly once.

/** One actual typed topic request and finite response polling. The IDs select
 * only the independently configured SIM test peer, never a firmware address.
 * No service_call/advertise/respond abstraction is used for these two topics. */
import { Ros2Unitree_apiMsgRequest as Request, Ros2Unitree_apiMsgRequestCodec as RequestCodec,
         Ros2Unitree_apiMsgResponse as Response, Ros2Unitree_apiMsgResponseCodec as ResponseCodec } from '../../../../custom-types/typed-family/bounded_records';
import { Publisher } from '../../../../assemblyscript/ros2/roswasm';
import { Life } from './lifecycle';
const life=new Life();let publisher:Publisher<Request>|null=null;
let id:i64=0,api:i64=0,submitted:bool=false,replyCode:u32=0;
function onResponse(value:Response):void {
  if(!submitted||value.header.identity.id!=id||value.header.identity.api_id!=api||value.header.status.code!=0||value.data!='typed-example-ack'||value.binary.length){life.status=22;return;}
  replyCode=<u32>value.header.status.code;++life.received;
}
// ABI initialize_case(mode,arg0,arg1)->u32 is single-use. Mode0 proves actual
// default denial; mode1 requires the explicit positive aliases stated above.
// LowState args are (family1=GO or2=HG,reserved0); API args are (request_id,
// api_id), each1..0x7fffffff. Return0 is setup/denial success, never delivery.
// An error attempts cleanup; a fresh instance is required for another case.
export function initialize_case(mode:u32=0,request_id:u32=1,api_id:u32=1):u32 {
  if(!request_id||request_id>0x7fffffff||!api_id||api_id>0x7fffffff)return life.close(22);
  let first=life.begin(mode);if(first)return first;id=<i64>request_id;api=<i64>api_id;
  if(RequestCodec.maxWireBytes>32768||ResponseCodec.maxWireBytes>32768)return life.close(90);
  const pub=life.node.createPublisher<Request>('unitree-api-request',new RequestCodec());publisher=pub;
  if(mode==0){if(pub.capability.openStatus==13)return life.deny(13);return life.close(pub.capability.openStatus?pub.capability.openStatus:22);}
  if(pub.capability.openStatus)return life.close(pub.capability.openStatus);
  const sub=life.node.createSubscription<Response>('unitree-api-response',new ResponseCodec(),onResponse,ResponseCodec.maxWireBytes);
  if(sub.status)return life.close(sub.status);first=life.executor.add(sub);return first?life.close(first):0;
}
// ABI submit_once()->u32 sends one complete typed Request after successful
// positive initialization. It uses the configured ids, empty binary and "{}".
// Return0 means the SDK accepted publication; only the independent native
// peer and later matching Response prove transport. A repeated call returns9.
export function submit_once():u32 {
  const pub=publisher;if(!life.started||life.closed||submitted||!pub)return 9;
  const value=new Request();value.header.identity.id=id;value.header.identity.api_id=api;value.parameter='{}';
  const status=pub.publish(value);if(status)return life.close(status);submitted=true;return 0;
}
// ABI poll_once()->u32 performs one explicit copied SDK poll pass. API must
// have submitted first. Return11 means no new value;0 means a newly validated
// value, and nonzero errors retain their status and attempt cleanup. No hidden
// retry occurs; the4097th attempt closes with110. Inspect count before getters.
export function poll_once():u32{return submitted?life.poll():9;}
// ABI received_count()->u32 observes the number of validated complete values.
// Require successful initialization and a poll result0 first. Zero is a valid
// initial count and never proves delivery; this getter performs no native call.
export function received_count():u32{return life.received;}
// ABI response_code()->u32 observes the accepted Response status after a
// successful poll and received_count>0. This fixture accepts status0 only;
// an initial zero is not an acknowledgement and invokes no native operation.
export function response_code():u32{return replyCode;}
// ABI denied_verified()->u32 is1 ONLY after mode0 observed actual denial13.
// It is0 before that observation or in a positive case. The independent native
// observer must additionally prove zero new endpoints/payload; this flag alone
// cannot establish absence of effects. It grants no permission.
export function denied_verified():u32{return life.denied?1:0;}
// ABI case_status()->u32 observes the preserved operation/cleanup status.
// Read it after any ambiguous scalar getter or callback pass. Zero is the
// initial/success value; it cannot replace the delivery count/native witness.
// This getter neither authorizes, polls nor consumes a native queue.
export function case_status():u32{return life.status;}
// ABI close_case()->u32 retires every ordinarily acquired owner after the
// current poll/callback has unwound, attempting remaining closes after errors.
// Return0 means those closes succeeded; later close/poll/submit returns9.
// A denial case was already closed by initialize_case and therefore returns9.
// External ownership remains necessary for an unrecoverable trap/process kill.
export function close_case():u32{return life.close();}
