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
// Rust ownership note: the entry borrows its single CASE with try_borrow_mut.
// No callback may recursively invoke this instance. Complete generated records
// are a #[path] module so their original inner module documentation is preserved.

//! Complete generated LowState reader, built as one explicit ROS2 cdylib.
//! Select exactly one unitree_go/unitree_hg cfg and stage that family's entire
//! bounded_records.rs beside this file. No LowCmd/CRC packing occurs here.
extern crate roswasm;
mod lifecycle;
#[path="bounded_records.rs"] mod vendor;
use lifecycle::Life;
use std::cell::RefCell;
use std::rc::Rc;
#[cfg(all(unitree_go,not(unitree_hg)))] use vendor::Ros2Unitree_goMsgLowState as LowState;
#[cfg(all(unitree_hg,not(unitree_go)))] use vendor::Ros2Unitree_hgMsgLowState as LowState;
#[cfg(not(any(all(unitree_go,not(unitree_hg)),all(unitree_hg,not(unitree_go)))))]
compile_error!("Select exactly one actual generated GO or HG family");
struct Case {life:Life,tick:u32,crc:u32,words:[u32;105],pending:Rc<RefCell<Option<Result<LowState,roswasm::Error>>>>}
thread_local!{static CASE:RefCell<Option<Case>>=const{RefCell::new(None)};}
fn access(f:impl FnOnce(&mut Case)->u32)->u32{CASE.with(|slot|match slot.try_borrow_mut(){Ok(mut value)=>match value.as_mut(){Some(case)=>f(case),None=>9},Err(_)=>17})}
// ABI initialize_case(mode,arg0,arg1)->u32 is single-use. Mode0 proves actual
// default denial; mode1 requires the explicit positive aliases stated above.
// LowState args are (family1=GO or2=HG,reserved0); API args are (request_id,
// api_id), each1..0x7fffffff. Return0 is setup/denial success, never delivery.
// An error attempts cleanup; a fresh instance is required for another case.
#[no_mangle] pub extern "C" fn initialize_case(mode:u32,family:u32,reserved:u32)->u32{
    CASE.with(|slot|{let Ok(mut value)=slot.try_borrow_mut() else{return 17;};if value.is_some(){return 9;}
        let selected=if cfg!(unitree_go){1}else{2};if mode>1||family!=selected||reserved!=0{return 22;}
        let mut case=Case{life:Life::new(),tick:0,crc:0,words:[0;105],pending:Rc::new(RefCell::new(None))};
        if mode==0 {
            let result=match roswasm::Capability::open(roswasm::Backend::Ros2,"unitree-lowstate"){
                Err(e)=>case.life.denied(e.code),Ok(mut cap)=>{let _cleanup=cap.close();case.life.close(22)}};
            *value=Some(case);return result;
        }
        let result=match case.life.node.subscription("unitree-lowstate"){
            Ok(sub)=>{case.life.subscription=Some(sub);{
                match roswasm::node::Executor::try_new(LowState::MAX_WIRE_BYTES){Err(e)=>case.life.close(e.code),Ok(mut executor)=>{
                    let observation=Rc::clone(&case.pending);
                    let added=executor.add_typed::<LowState>(case.life.subscription.take().unwrap(),move |value|{*observation.borrow_mut()=Some(value);});
                    case.life.executor=Some(executor);match added{Ok(())=>0,Err(e)=>case.life.close(e.code)}}}}},
            Err(e)=>case.life.close(e.code)};
        *value=Some(case);result})
}
// ABI poll_once()->u32 performs one explicit copied SDK poll pass. API must
// have submitted first. Return11 means no new value;0 means a newly validated
// value, and nonzero errors retain their status and attempt cleanup. No hidden
// retry occurs; the4097th attempt closes with110. Inspect count before getters.
#[no_mangle] pub extern "C" fn poll_once()->u32{access(|case|{
    if let Err(code)=case.life.polling(){return code;}
    match case.life.executor.as_mut().unwrap().spin_once(){Err(e)=>return case.life.close(e.code),Ok(0)=>return 11,Ok(_)=>{}}
    let result=case.pending.borrow_mut().take().unwrap_or_else(||Err(roswasm::Error::new(22)));
    match result{Err(e) if e.code==11=>11,Err(e)=>case.life.close(e.code),Ok(state)=>{
        let motors=if cfg!(unitree_go){20}else{35};
        if state.motor_state.len()!=motors{return case.life.close(22);}
        for (slot,motor) in state.motor_state.iter().enumerate(){
            for (field,number) in [motor.q,motor.dq,motor.tau_est].iter().enumerate(){
                if !number.is_finite(){return case.life.close(22);}case.words[slot*3+field]=number.to_bits();}}
        case.tick=state.tick;case.crc=state.crc;case.life.received+=1;0}}})}
// ABI observed_tick()->u32 returns the last delivered native LowState tick.
// Require received_count>0 and case_status0. Zero is a legitimate tick value,
// not an error sentinel or proof that a native message was delivered.
#[no_mangle] pub extern "C" fn observed_tick()->u32{access(|c|c.tick)}
// ABI observed_crc()->u32 returns the last delivered native LowState CRC field.
// Require received_count>0 and case_status0. This is observation, not command
// CRC generation, authentication or independent native-layout verification.
#[no_mangle] pub extern "C" fn observed_crc()->u32{access(|c|c.crc)}
// ABI received_count()->u32 observes the number of validated complete values.
// Require successful initialization and a poll result0 first. Zero is a valid
// initial count and never proves delivery; this getter performs no native call.
#[no_mangle] pub extern "C" fn received_count()->u32{access(|c|c.life.received)}
// ABI denied_verified()->u32 is1 ONLY after mode0 observed actual denial13.
// It is0 before that observation or in a positive case. The independent native
// observer must additionally prove zero new endpoints/payload; this flag alone
// cannot establish absence of effects. It grants no permission.
#[no_mangle] pub extern "C" fn denied_verified()->u32{access(|c|u32::from(c.life.denied))}
// ABI motor_word(slot,field)->u32 returns binary32 bits for one delivered
// carrier value: field0=q,1=dq,2=tau_est; slot<20(GO) or<35(HG). Require a
// received value and case_status0. Invalid/unreceived access closes with22 and
// returns0; zero bits may also represent valid +0.0, so inspect status/count.
#[no_mangle] pub extern "C" fn motor_word(slot:u32,field:u32)->u32{access(|c|{
    if c.life.received==0||slot>=(if cfg!(unitree_go){20}else{35})||field>=3{c.life.close(22);0}else{c.words[slot as usize*3+field as usize]}})}
// ABI case_status()->u32 observes the preserved operation/cleanup status.
// Read it after any ambiguous scalar getter or callback pass. Zero is the
// initial/success value; it cannot replace the delivery count/native witness.
// This getter neither authorizes, polls nor consumes a native queue.
#[no_mangle] pub extern "C" fn case_status()->u32{access(|c|c.life.status)}
// ABI close_case()->u32 retires every ordinarily acquired owner after the
// current poll/callback has unwound, attempting remaining closes after errors.
// Return0 means those closes succeeded; later close/poll/submit returns9.
// A denial case was already closed by initialize_case and therefore returns9.
// External ownership remains necessary for an unrecoverable trap/process kill.
#[no_mangle] pub extern "C" fn close_case()->u32{access(|c|c.life.close(0))}
