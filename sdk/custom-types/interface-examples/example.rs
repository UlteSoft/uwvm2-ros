//! Reusable caller-driven RPC steps; build with roswasm alloc + explicit ros2.
//! Generate the sibling generated-rpc directory before adding this module.
//! Applications own returned facades and explicitly inspect every checked close.
#[path="../generated-rpc/bounded_records.rs"]
pub mod bounded_records;
#[path="../generated-rpc/bounded_interfaces.rs"]
pub mod bounded_interfaces;
use bounded_records::*;
use bounded_interfaces::*;
use roswasm::{Backend,Error};

/// Open only the exact service-call and separately granted completion aliases.
/// Backend::Ros2 must be selected explicitly by the actual application. No
/// source/ABI feature creates an alias or substitutes a native type identity.
pub fn open_client(backend:Backend,call_alias:&str,observer_alias:&str)->Result<Ros2Demo_rpcSrvIncrementClient,OpenError>{
    Ros2Demo_rpcSrvIncrementClient::open(backend,call_alias,Some(observer_alias),8)
}
/// Admit once. Keep the client alive and later call poll_completion() once per
/// caller step; Completion::Failure preserves a numeric external-effect error.
pub fn submit_step(client:&mut Ros2Demo_rpcSrvIncrementClient,value:i32)->Result<u64,Error>{
    client.submit(&Ros2Demo_rpcSrvIncrement_Request{value})
}
/// Dispatch one service request. The supplied handler receives owned typed data,
/// never a capability or a native function pointer. It must not overflow i32.
pub fn service_step(server:&mut Ros2Demo_rpcSrvIncrementServer)->Result<(),Error>{
    server.spin_once(|_ticket,request|Ok(Ros2Demo_rpcSrvIncrement_Response{value:request.value.checked_add(1).ok_or(Error::new(22))?}))
}
/// An action response remains explicitly caller-driven, preserving the delivered
/// ID. Feedback retains ownership; a successful terminal result removes it.
pub fn action_step(server:&mut Ros2Demo_rpcActionCountServer)->Result<(),Error>{
    let(id,goal)=server.poll_once()?;
    server.respond_feedback(id,&Ros2Demo_rpcActionCount_Feedback{progress:goal.count})?;
    server.respond_result(id,2,&Ros2Demo_rpcActionCount_Result{total:goal.count})
}
/// Caller examples must inspect the returned Cleanup, including statuses after
///107/110. Dropping an owner performs best-effort close, never implicit revoke.
pub fn finish_client(client:&mut Ros2Demo_rpcSrvIncrementClient,primary:u32)->Cleanup{client.close_checked(primary)}
