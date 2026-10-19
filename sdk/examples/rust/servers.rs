//! Lesson5: real polling Service and Action server APIs with checked ownership.
//!
//! Enable exactly one backend plus alloc. The operator must grant independent
//! trigger-events/service.advertise and trigger-replies/service.respond aliases
//! for the same official Trigger resource, and fibonacci-events/action.advertise
//! plus fibonacci-replies/action.respond for the supported Fibonacci Action.
//! Every byte/rate/queue/inflight/goal/deadline bound is supplied by host policy.
//! No factory discovers resources, chooses native names or grants respond rights.
//!
//! This example performs one poll for each server. AGAIN is normal idle, not a
//! delivered request. An application supplies its own bounded outer scheduler;
//! the SDK creates no clock, thread, sleep loop or callback inside native ROS.
use roswasm::{codec, codes, Backend, Error, Session};
#[cfg(all(feature = "ros1", not(feature = "ros2")))]
const BACKEND: Backend = Backend::Ros1;
#[cfg(all(feature = "ros2", not(feature = "ros1")))]
const BACKEND: Backend = Backend::Ros2;
#[cfg(any(all(feature = "ros1", feature = "ros2"), not(any(feature = "ros1", feature = "ros2"))))]
compile_error!("Select exactly one example backend: ros1 or ros2");
fn run() -> Result<(), Error> {
    let session = Session::new(BACKEND, 4)?;
    let mut service_owner = None; let mut action_owner = None;
    let result = (|| {
        let node = session.node("rust_servers");
        // Both factories acquire their respond grant before activating events.
        // Failure closes already-acquired handles and preserves the first error.
        service_owner = Some(node.server("trigger-events", "trigger-replies")?);
        action_owner = Some(node.action_server("fibonacci-events", "fibonacci-replies")?);
        let service=service_owner.as_ref().unwrap(); let action=action_owner.as_ref().unwrap();
        let mut request = [0; 64]; let mut reply = [0; 256];
        let delivered = service.spin_once(&mut request, &mut reply, |id, bytes, out| {
            codec::decode_trigger_request(bytes)?;
            // The server verifies this response's ID matches the incoming ID
            // and checks the complete Bool/String envelope before importing.
            codec::encode_service_reply(id, &codec::ServiceResponse { success: true, message: "request handled" }, out)
        })?;
        println!("service request delivered={delivered}");
        let mut scratch = [0; 256];
        let handled = action.spin_goal_once(|id, order| {
            // The supported bound0..46 keeps signed int32 additions exact.
            // Match the repository's official Fibonacci tutorial sequence:
            // [0,1], then one appended sum for indices1..order-1. No sleep or
            // hidden feedback loop occurs; this pass emits one terminal result.
            if !(0..=46).contains(&order) { return Err(Error::new(codes::INVALID)); }
            let mut values=vec![0i32,1];
            for index in 1..order as usize { values.push(values[index]+values[index-1]); }
            action.respond_fibonacci(id, 2, &values, &mut scratch)
        })?;
        println!("action goal delivered={handled}");
        Ok(())
    })();
    let cleanup = session.close();
    // Always inspect checked group cleanup, even after an operation failure.
    // Preserve the earlier error: failed notification does not undo a response.
    let mut first = result;
    for outcome in cleanup { if let Err(error) = outcome { eprintln!("checked close status {}",error.code); if first.is_ok() { first = Err(error); } } }
    first
}
fn main() {
    if let Err(error) = run() { eprintln!("ROS status {}; earlier native effects may remain",error.code); std::process::exit(1); }
}
