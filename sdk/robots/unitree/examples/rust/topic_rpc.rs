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
// Rust ownership note: the entry borrows its single CASE with try_borrow_mut.
// No callback may recursively invoke this instance. Complete generated records
// are a #[path] module so their original inner module documentation is preserved.

//! Real typed Request/Response topics. This never invokes service_call or a
//! firmware API. A separately controlled native SIM peer acknowledges the
//! bounded request with matching identity and the documented test-only data.
extern crate roswasm;
mod lifecycle;
#[path="bounded_records.rs"] mod vendor;
use lifecycle::Life;
use std::cell::RefCell;
use vendor::{Ros2Unitree_apiMsgRequest as Request,Ros2Unitree_apiMsgResponse as Response};
struct Case{life:Life,id:i64,api:i64,submitted:bool,reply_code:u32}
thread_local!{static CASE:RefCell<Option<Case>>=const{RefCell::new(None)};}
fn access(f:impl FnOnce(&mut Case)->u32)->u32{CASE.with(|slot|match slot.try_borrow_mut(){Ok(mut value)=>match value.as_mut(){Some(case)=>f(case),None=>9},Err(_)=>17})}
// ABI initialize_case(mode,arg0,arg1)->u32 is single-use. Mode0 proves actual
// default denial; mode1 requires the explicit positive aliases stated above.
// LowState args are (family1=GO or2=HG,reserved0); API args are (request_id,
// api_id), each1..0x7fffffff. Return0 is setup/denial success, never delivery.
// An error attempts cleanup; a fresh instance is required for another case.
#[no_mangle] pub extern "C" fn initialize_case(mode:u32,id:u32,api:u32)->u32{
    CASE.with(|slot|{let Ok(mut value)=slot.try_borrow_mut() else{return 17;};if value.is_some(){return 9;}
        if mode>1||id==0||id>0x7fffffff||api==0||api>0x7fffffff{return 22;}
        let mut case=Case{life:Life::new(),id:id.into(),api:api.into(),submitted:false,reply_code:0};
        let result=match case.life.node.publisher("unitree-api-request"){
            Err(e)=>if mode==0{case.life.denied(e.code)}else{case.life.close(e.code)},
            Ok(publisher)=>{case.life.publisher=Some(publisher);if mode==0{case.life.close(22)}else{
                match case.life.node.subscription("unitree-api-response"){Ok(sub)=>{case.life.subscription=Some(sub);0},Err(e)=>case.life.close(e.code)}}}};
        *value=Some(case);result})
}
// ABI submit_once()->u32 sends one complete typed Request after successful
// positive initialization. It uses the configured ids, empty binary and "{}".
// Return0 means the SDK accepted publication; only the independent native
// peer and later matching Response prove transport. A repeated call returns9.
#[no_mangle] pub extern "C" fn submit_once()->u32{access(|c|{
    if c.life.closed||c.submitted{return 9;}let mut request=Request::default();
    request.header.identity.id=c.id;request.header.identity.api_id=c.api;request.parameter="{}".into();
    match c.life.publisher.as_ref().unwrap().publish_value(&request){Ok(())=>{c.submitted=true;0},Err(e)=>c.life.close(e.code)}})}
// ABI poll_once()->u32 performs one explicit copied SDK poll pass. API must
// have submitted first. Return11 means no new value;0 means a newly validated
// value, and nonzero errors retain their status and attempt cleanup. No hidden
// retry occurs; the4097th attempt closes with110. Inspect count before getters.
#[no_mangle] pub extern "C" fn poll_once()->u32{access(|c|{
    if !c.submitted{return 9;}if let Err(code)=c.life.polling(){return code;}
    match c.life.subscription.as_ref().unwrap().poll_value::<Response>(&mut c.life.scratch){
        Err(e)if e.code==11=>11,Err(e)=>c.life.close(e.code),Ok(response)=>{
            if response.header.identity.id!=c.id||response.header.identity.api_id!=c.api||response.header.status.code!=0||response.data!="typed-example-ack"||!response.binary.is_empty(){return c.life.close(22);}
            c.reply_code=response.header.status.code as u32;c.life.received+=1;0}}})}
// ABI received_count()->u32 observes the number of validated complete values.
// Require successful initialization and a poll result0 first. Zero is a valid
// initial count and never proves delivery; this getter performs no native call.
#[no_mangle] pub extern "C" fn received_count()->u32{access(|c|c.life.received)}
// ABI response_code()->u32 observes the accepted Response status after a
// successful poll and received_count>0. This fixture accepts status0 only;
// an initial zero is not an acknowledgement and invokes no native operation.
#[no_mangle] pub extern "C" fn response_code()->u32{access(|c|c.reply_code)}
// ABI denied_verified()->u32 is1 ONLY after mode0 observed actual denial13.
// It is0 before that observation or in a positive case. The independent native
// observer must additionally prove zero new endpoints/payload; this flag alone
// cannot establish absence of effects. It grants no permission.
#[no_mangle] pub extern "C" fn denied_verified()->u32{access(|c|u32::from(c.life.denied))}
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
