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
// AssemblyScript build note: the generated complete family resolves through
// sdk/custom-types/typed-family/bounded_records in the owned stage. Bind the
// actual SDK abortGuest and call the actual _initialize export explicitly once.

/** Stage the ENTIRE generated GO24 family at sdk/custom-types/typed-family.
 * These bytes are complete canonical LowState values, not native CRC bytes. */
import { Ros2Unitree_goMsgLowState as State, Ros2Unitree_goMsgLowStateCodec as StateCodec } from '../../../../custom-types/typed-family/bounded_records';
import { StateReader } from './state_reader';
const reader:StateReader<State>=new StateReader<State>(1,20,new StateCodec(),StateCodec.maxWireBytes);
function onState(value:State):void {
  if(value.motor_state.length!=20){reader.life.status=22;return;}
  for(let slot=0;slot<20;++slot){const m=value.motor_state[slot];if(!isFinite(m.q)||!isFinite(m.dq)||!isFinite(m.tau_est)){reader.life.status=22;return;}reader.words[slot*3]=reinterpret<u32>(m.q);reader.words[slot*3+1]=reinterpret<u32>(m.dq);reader.words[slot*3+2]=reinterpret<u32>(m.tau_est);}
  reader.tick=value.tick;reader.crc=value.crc;++reader.life.received;
}
// ABI initialize_case(mode,arg0,arg1)->u32 is single-use. Mode0 proves actual
// default denial; mode1 requires the explicit positive aliases stated above.
// LowState args are (family1=GO or2=HG,reserved0); API args are (request_id,
// api_id), each1..0x7fffffff. Return0 is setup/denial success, never delivery.
// An error attempts cleanup; a fresh instance is required for another case.
export function initialize_case(mode:u32=0,family:u32=1,reserved:u32=0):u32{return reader.initialize(mode,family,reserved,onState);}
// ABI poll_once()->u32 performs one explicit copied SDK poll pass. API must
// have submitted first. Return11 means no new value;0 means a newly validated
// value, and nonzero errors retain their status and attempt cleanup. No hidden
// retry occurs; the4097th attempt closes with110. Inspect count before getters.
export function poll_once():u32{return reader.life.poll();}
// ABI observed_tick()->u32 returns the last delivered native LowState tick.
// Require received_count>0 and case_status0. Zero is a legitimate tick value,
// not an error sentinel or proof that a native message was delivered.
export function observed_tick():u32{return reader.tick;}
// ABI observed_crc()->u32 returns the last delivered native LowState CRC field.
// Require received_count>0 and case_status0. This is observation, not command
// CRC generation, authentication or independent native-layout verification.
export function observed_crc():u32{return reader.crc;}
// ABI received_count()->u32 observes the number of validated complete values.
// Require successful initialization and a poll result0 first. Zero is a valid
// initial count and never proves delivery; this getter performs no native call.
export function received_count():u32{return reader.life.received;}
// ABI denied_verified()->u32 is1 ONLY after mode0 observed actual denial13.
// It is0 before that observation or in a positive case. The independent native
// observer must additionally prove zero new endpoints/payload; this flag alone
// cannot establish absence of effects. It grants no permission.
export function denied_verified():u32{return reader.life.denied?1:0;}
// ABI motor_word(slot,field)->u32 returns binary32 bits for one delivered
// carrier value: field0=q,1=dq,2=tau_est; slot<20(GO) or<35(HG). Require a
// received value and case_status0. Invalid/unreceived access closes with22 and
// returns0; zero bits may also represent valid +0.0, so inspect status/count.
export function motor_word(slot:u32,field:u32):u32{return reader.motorWord(slot,field);}
// ABI case_status()->u32 observes the preserved operation/cleanup status.
// Read it after any ambiguous scalar getter or callback pass. Zero is the
// initial/success value; it cannot replace the delivery count/native witness.
// This getter neither authorizes, polls nor consumes a native queue.
export function case_status():u32{return reader.life.status;}
// ABI close_case()->u32 retires every ordinarily acquired owner after the
// current poll/callback has unwound, attempting remaining closes after errors.
// Return0 means those closes succeeded; later close/poll/submit returns9.
// A denial case was already closed by initialize_case and therefore returns9.
// External ownership remains necessary for an unrecoverable trap/process kill.
export function close_case():u32{return reader.life.close();}
