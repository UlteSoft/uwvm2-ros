//! Lesson6: typed ROS2 parameter events, TF, graph, clocks and Lifecycle.
//!
//! Parameter events/lifecycle are ROS2-only. ROS1 uses its explicitly granted
//! wall/ROS clocks and TF but cannot acquire these ROS2 operations. This example
//! is built only with ros2+alloc and opens exactly the aliases named below.
//! Configure their native resources/types/identities, frame pair, transition,
//! result bounds, queues and operation-specific quotas before running it.
//! Event delivery is guest-thread polling; no native callback calls Rust.
use roswasm::{codes, node::Executor, typed::TypedParameterEvent, Backend, Error, Operation, Session};
#[cfg(not(all(feature = "ros2", not(feature = "ros1"))))]
compile_error!("events_tf_lifecycle requires exactly ros2 plus alloc");
fn run() -> Result<(), Error> {
    let session = Session::new(Backend::Ros2, 6)?;
    let mut executor = Executor::try_new(4096)?;
    let callback_error=std::rc::Rc::new(std::cell::Cell::new(None)); let observed=std::rc::Rc::clone(&callback_error);
    let mut tf_owner=None; let mut graph_owner=None; let mut clock_owner=None; let mut state_owner=None; let mut change_owner=None;
    let result = (|| {
        let node = session.node("rust_events_and_resources");
        let queue = node.parameters("parameter-events")?.subscribe_events()?;
        executor.add_typed::<TypedParameterEvent>(queue, move |event| {
            // This owned value was fully decoded before the callback. Native
            // strings are input data, not aliases or newly granted operation names.
            match event { Ok(value) => println!("parameter event kind {}", value.kind), Err(error) => { observed.set(Some(error)); eprintln!("event decode status {}",error.code); } }
        })?;
        let delivered = executor.spin_once()?; println!("events delivered={delivered}");
        if let Some(error)=callback_error.take() { return Err(error); }
        // Closing the executor retires its queue; Session.close skips that same
        // retired state and does not issue an accidental second close import.
        let mut scratch = [0; 4096];
        tf_owner=Some(node.tf("transform-lookup")?); let tf=tf_owner.as_ref().unwrap();
        let transform = tf.lookup_transform(0, &mut scratch)?;
        println!("configured TF parent {}", transform.header.frame);
        graph_owner=Some(node.graph("nodes-list")?); let graph=graph_owner.as_ref().unwrap();
        println!("filtered nodes {}",graph.snapshot(Operation::GraphListNodes,&mut scratch)?);
        clock_owner=Some(node.clock("steady-clock")?); let clock=clock_owner.as_ref().unwrap();
        println!("exact steady nanoseconds {}",clock.read(Operation::TimeReadSteady)?);
        state_owner=Some(node.lifecycle("lifecycle-state")?); let lifecycle=state_owner.as_ref().unwrap();
        println!("native lifecycle state {}",lifecycle.state(&mut scratch)?.id);
        // A change-state alias is separate and its transition is host-fixed.
        // Reading state never grants or selects a transition.
        change_owner=Some(node.lifecycle("lifecycle-change")?); let change=change_owner.as_ref().unwrap();
        if !change.change()? { return Err(Error::new(codes::INVALID)); }
        Ok(())
    })();
    let mut first = result;
    let queue_close=executor.close(); if let Err(error)=queue_close { eprintln!("queue close status {}",error.code); if first.is_ok() { first=Err(error); } }
    for outcome in session.close() { if let Err(error) = outcome { eprintln!("checked close status {}",error.code); if first.is_ok() { first = Err(error); } } }
    first
}
fn main() { if let Err(error) = run() { eprintln!("ROS status {}",error.code); std::process::exit(1); } }
