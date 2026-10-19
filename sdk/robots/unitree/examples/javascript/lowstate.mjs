// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Role: read actual ROS2 unitree_go/msg/LowState (20 carrier slots) or
// unitree_hg/msg/LowState (35), as explicitly selected by this entry/build.
// Mode0 checks the first unitree-lowstate alias open is denied without activating
// a subscription. Mode1 requires ONLY that alias as topic.subscribe; no publisher,
// LowCmd, service, robot address or CRC packer is introduced. Complete generated
// decode precedes observation of finite q/dq/tau_est, tick and the native CRC
// field. Every observed word must match an independent actual native witness.
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
// Embedded-JS note: execute this only in the selected real QuickJS-in-Wasm Guest
// with the closed source module manifest and UTF-8 bootstrap. Host Node or a mock
// bridge cannot qualify these examples or their native transport.

/** Complete real GO/HG LowState subscription. The selected generated complete
 * family is staged as bounded_records.mjs beside this source. No LowCmd,
 * native endpoint, timer, CRC packing or actuation is performed by this reader.
 */
import * as Vendor from './bounded_records.mjs';
import { Lifecycle } from './lifecycle.mjs';
const life=new Lifecycle(), words=new Uint32Array(105), view=new DataView(new ArrayBuffer(4));
let tick=0, crc=0, motors=0;
// ABI initialize_case(mode,arg0,arg1)->u32 is single-use. Mode0 proves actual
// default denial; mode1 requires the explicit positive aliases stated above.
// LowState args are (family1=GO or2=HG,reserved0); API args are (request_id,
// api_id), each1..0x7fffffff. Return0 is setup/denial success, never delivery.
// An error attempts cleanup; a fresh instance is required for another case.
export function initialize_case(mode=0,family=1,reserved=0) {
  if(reserved!==0 || (family!==1 && family!==2))return life.fail(22);
  const started=life.begin(mode);if(started)return started;
  const codec=Vendor[family===1?'Ros2Unitree_goMsgLowStateCodec':'Ros2Unitree_hgMsgLowStateCodec'];motors=family===1?20:35;
  try {
    if(!codec || codec.maxWireBytes>32768 || life.session.outputCapacity<codec.maxWireBytes)return life.fail(90);
    if(mode===0){life.node.open('unitree-lowstate');return life.fail(22);}
    const sub=life.node.createSubscription('unitree-lowstate',codec,value=>{
      if(value.motor_state.length!==motors)throw new TypeError('Exact carrier count required');
      for(let slot=0;slot<motors;++slot)for(let field=0;field<3;++field){const item=value.motor_state[slot],number=field===0?item.q:field===1?item.dq:item.tau_est;if(!Number.isFinite(number))throw new TypeError('Finite native state required');view.setFloat32(0,number,true);words[slot*3+field]=view.getUint32(0,true);}
      tick=value.tick>>>0;crc=value.crc>>>0;++life.received;
    });
    life.executor.add(sub);return 0;
  } catch(error) { return mode===0?life.finishDenied(error):life.fail(error); }
}
// ABI poll_once()->u32 performs one explicit copied SDK poll pass. API must
// have submitted first. Return11 means no new value;0 means a newly validated
// value, and nonzero errors retain their status and attempt cleanup. No hidden
// retry occurs; the4097th attempt closes with110. Inspect count before getters.
export function poll_once(){return life.poll();}
// ABI observed_tick()->u32 returns the last delivered native LowState tick.
// Require received_count>0 and case_status0. Zero is a legitimate tick value,
// not an error sentinel or proof that a native message was delivered.
export function observed_tick(){return tick;}
// ABI observed_crc()->u32 returns the last delivered native LowState CRC field.
// Require received_count>0 and case_status0. This is observation, not command
// CRC generation, authentication or independent native-layout verification.
export function observed_crc(){return crc;}
// ABI received_count()->u32 observes the number of validated complete values.
// Require successful initialization and a poll result0 first. Zero is a valid
// initial count and never proves delivery; this getter performs no native call.
export function received_count(){return life.received;}
// ABI denied_verified()->u32 is1 ONLY after mode0 observed actual denial13.
// It is0 before that observation or in a positive case. The independent native
// observer must additionally prove zero new endpoints/payload; this flag alone
// cannot establish absence of effects. It grants no permission.
export function denied_verified(){return life.denied?1:0;}
// ABI motor_word(slot,field)->u32 returns binary32 bits for one delivered
// carrier value: field0=q,1=dq,2=tau_est; slot<20(GO) or<35(HG). Require a
// received value and case_status0. Invalid/unreceived access closes with22 and
// returns0; zero bits may also represent valid +0.0, so inspect status/count.
export function motor_word(slot,field){if(!Number.isInteger(slot)||!Number.isInteger(field)||slot<0||slot>=motors||field<0||field>=3||!life.received){life.fail(22);return 0;}return words[slot*3+field];}
// ABI case_status()->u32 observes the preserved operation/cleanup status.
// Read it after any ambiguous scalar getter or callback pass. Zero is the
// initial/success value; it cannot replace the delivery count/native witness.
// This getter neither authorizes, polls nor consumes a native queue.
export function case_status(){return life.status;}
// ABI close_case()->u32 retires every ordinarily acquired owner after the
// current poll/callback has unwound, attempting remaining closes after errors.
// Return0 means those closes succeeded; later close/poll/submit returns9.
// A denial case was already closed by initialize_case and therefore returns9.
// External ownership remains necessary for an unrecoverable trap/process kill.
export function close_case(){return life.close();}
