//! Private source-independent dispatch core, also used by native fake fixtures.
//! It validates canonical bodies/owned IDs and guards before polling. Actual
//! Server methods supply their real capability imports; tests supply counters,
//! without creating a second public/mock backend or weakening native authority.
use crate::{codec,codes,Error};
use core::cell::Cell;

pub(crate) fn idle(spinning:&Cell<bool>)->Result<(),Error> {
    if spinning.get() { Err(Error::new(codes::CONFLICT)) } else { Ok(()) }
}
struct Reset<'a>(&'a Cell<bool>);
impl Drop for Reset<'_> { fn drop(&mut self) { self.0.set(false); } }
/// Keep a capability's shared retirement state busy until callback dispatch ends.
/// Request/response imports remain allowed; close/revoke must check this state.
pub(crate) struct OwnerGuard<'a>(&'a Cell<bool>);
impl Drop for OwnerGuard<'_> { fn drop(&mut self) { self.0.set(false); } }
pub(crate) fn guard_owner(busy:&Cell<bool>)->Result<OwnerGuard<'_>,Error> {
    if busy.replace(true) { return Err(Error::new(codes::CONFLICT)); }
    Ok(OwnerGuard(busy))
}
/// Preflight the entire ownership group before retiring even its first handle.
/// A refusal leaves local factories/owners open for later checked cleanup.
pub(crate) fn cleanup_idle<'a>(owners:impl IntoIterator<Item=(bool,&'a Cell<bool>)>)->Result<(),Error> {
    for (live,busy) in owners { if live { idle(busy)?; } } Ok(())
}
pub(crate) fn service_once(
    spinning:&Cell<bool>, input:&mut[u8], output:&mut[u8],
    poll:impl FnOnce(&mut[u8])->Result<usize,Error>,
    respond:impl FnOnce(&[u8])->Result<(),Error>,
    handler:impl FnOnce(codec::RequestId,&[u8],&mut[u8])->Result<usize,Error>,
)->Result<bool,Error> {
    if spinning.replace(true) { return Err(Error::new(codes::CONFLICT)); }
    let _reset=Reset(spinning);
    let count=match poll(input) { Ok(count)=>count, Err(error) if error.code==codes::AGAIN=>return Ok(false), Err(error)=>return Err(error) };
    if count>input.len() { return Err(Error::new(codes::INVALID)); }
    let request=codec::decode_service_request(&input[..count])?;
    let used=handler(request.id,request.request,output)?;
    if used>output.len() || used<13 || output[..8]!=request.id.raw().to_le_bytes() { return Err(Error::new(codes::INVALID)); }
    codec::decode_service_response(&output[8..used])?;
    respond(&output[..used])?; Ok(true)
}
pub(crate) fn goal_once(
    spinning:&Cell<bool>, poll:impl FnOnce()->Result<(codec::GoalId,i32),Error>,
    handler:impl FnOnce(codec::GoalId,i32)->Result<(),Error>,
)->Result<bool,Error> {
    if spinning.replace(true) { return Err(Error::new(codes::CONFLICT)); }
    let _reset=Reset(spinning);
    match poll() { Ok((id,order))=>{handler(id,order)?; Ok(true)}, Err(error) if error.code==codes::AGAIN=>Ok(false), Err(error)=>Err(error) }
}

#[cfg(test)]
mod tests {
    use super::*;
    fn event(out:&mut[u8])->Result<usize,Error> { out[..8].copy_from_slice(&7u64.to_le_bytes()); Ok(8) }
    fn reply(id:codec::RequestId,_request:&[u8],out:&mut[u8])->Result<usize,Error> { codec::encode_service_reply(id,&codec::ServiceResponse{success:true,message:"ok"},out) }
    #[test]
    fn exact_service_id_and_checked_state() {
        let busy=Cell::new(false); let responses=Cell::new(0); let mut input=[0;16]; let mut output=[0;32];
        assert!(service_once(&busy,&mut input,&mut output,event,|wire|{ assert_eq!(&wire[..8],&7u64.to_le_bytes()); responses.set(1); Ok(()) },reply).unwrap());
        assert_eq!(responses.get(),1); assert!(idle(&busy).is_ok());
    }
    #[test]
    fn reentry_and_close_guard_do_not_poll_or_overwrite() {
        let busy=Cell::new(false); let mut input=[0;16]; let mut output=[0;32]; let polls=Cell::new(0);
        service_once(&busy,&mut input,&mut output,event,|_|Ok(()),|id,request,out|{
            assert_eq!(idle(&busy).unwrap_err().code,codes::CONFLICT);
            let mut other_input=[3;16]; let mut other_output=[4;32];
            let nested=service_once(&busy,&mut other_input,&mut other_output,|_|{ polls.set(1); Ok(0) },|_|panic!("nested response"),reply);
            assert_eq!(nested.unwrap_err().code,codes::CONFLICT); assert_eq!(other_input,[3;16]); assert_eq!(other_output,[4;32]);
            reply(id,request,out)
        }).unwrap(); assert_eq!(polls.get(),0); assert!(idle(&busy).is_ok());
    }
    #[test]
    fn mismatched_reply_and_malformed_body_never_respond() {
        let busy=Cell::new(false); let mut input=[0;16]; let mut output=[0;32];
        let wrong=service_once(&busy,&mut input,&mut output,event,|_|panic!("wrong ID imported"),|id,request,out|{ let n=reply(id,request,out)?; out[0]=8; Ok(n) });
        assert_eq!(wrong.unwrap_err().code,codes::INVALID);
        let malformed=service_once(&busy,&mut input,&mut output,|out|{event(out)?;out[8]=2;Ok(9)},|_|panic!("malformed imported"),|_,_,_|panic!("malformed callback"));
        assert_eq!(malformed.unwrap_err().code,codes::INVALID); assert!(idle(&busy).is_ok());
    }
    #[test]
    fn idle_or_failure_does_not_dispatch_and_guard_resets() {
        let busy=Cell::new(false); let mut input=[0;16]; let mut output=[0;32];
        assert!(!service_once(&busy,&mut input,&mut output,|_|Err(Error::new(codes::AGAIN)),|_|panic!("idle response"),|_,_,_|panic!("idle callback")).unwrap());
        let error=service_once(&busy,&mut input,&mut output,event,|_|panic!("failed callback response"),|_,_,_|Err(Error::new(codes::DENIED)));
        assert_eq!(error.unwrap_err().code,codes::DENIED); assert!(idle(&busy).is_ok());
    }
    #[test]
    fn goal_handler_keeps_identity_and_refuses_nested_poll() {
        let busy=Cell::new(false); let id=codec::decode_goal_id(&7u64.to_le_bytes()).unwrap(); let polls=Cell::new(0);
        assert!(goal_once(&busy,||Ok((id,5)),|selected,order|{
            assert_eq!(selected.raw(),7); assert_eq!(order,5);
            let nested=goal_once(&busy,||{polls.set(1);Ok((id,5))},|_,_|panic!("nested handler"));
            assert_eq!(nested.unwrap_err().code,codes::CONFLICT); assert_eq!(idle(&busy).unwrap_err().code,codes::CONFLICT); Ok(())
        }).unwrap()); assert_eq!(polls.get(),0); assert!(idle(&busy).is_ok());
    }
    #[test]
    fn shared_owner_lease_blocks_retirement_and_resets() {
        let busy=Cell::new(false); let live=Cell::new(true); let notifications=Cell::new(0);
        {
            let _lease=guard_owner(&busy).unwrap();
            assert_eq!(guard_owner(&busy).err().unwrap().code,codes::CONFLICT);
            let attempted=(|| { idle(&busy)?; live.set(false); notifications.set(1); Ok::<(),Error>(()) })();
            assert_eq!(attempted.unwrap_err().code,codes::CONFLICT);
            assert!(live.get()); assert_eq!(notifications.get(),0);
        }
        idle(&busy).unwrap(); live.set(false); notifications.set(1);
        assert!(!live.get()); assert_eq!(notifications.get(),1);
    }
    #[test]
    fn group_cleanup_preflights_all_live_owners_before_retirement() {
        let idle_owner=Cell::new(false); let callback_owner=Cell::new(false); let retired_owner=Cell::new(true);
        let first_live=Cell::new(true); let second_live=Cell::new(true); let notifications=Cell::new(0);
        {
            let _lease=guard_owner(&callback_owner).unwrap();
            let cleanup=(|| {
                cleanup_idle([(first_live.get(),&idle_owner),(second_live.get(),&callback_owner),(false,&retired_owner)])?;
                first_live.set(false); second_live.set(false); notifications.set(2); Ok::<(),Error>(())
            })();
            assert_eq!(cleanup.unwrap_err().code,codes::CONFLICT);
            assert!(first_live.get()&&second_live.get()); assert_eq!(notifications.get(),0);
        }
        cleanup_idle([(true,&idle_owner),(true,&callback_owner),(false,&retired_owner)]).unwrap();
        first_live.set(false); second_live.set(false); notifications.set(2);
        assert!(!first_live.get()&&!second_live.get()); assert_eq!(notifications.get(),2);
    }
}
