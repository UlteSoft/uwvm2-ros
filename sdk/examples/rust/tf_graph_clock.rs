//! Lesson9: actual ROS1/ROS2 TF lookup/broadcast, graph and explicit ROS clock.
//!
//! Grant transform-lookup and transform-broadcast for the same fixed frame pair
//! and complete finite translation/quaternion/cache/rate/byte/QoS limits. Grant
//! nodes-list and ros-clock independently. The peer must provide the transform;
//! a lookup never creates or grants it. ROS1 uses its official Header semantics.
use roswasm::{Backend,Error,Session,Operation};
#[cfg(all(feature="ros1",not(feature="ros2")))] const BACKEND:Backend=Backend::Ros1;
#[cfg(all(feature="ros2",not(feature="ros1")))] const BACKEND:Backend=Backend::Ros2;
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))]
compile_error!("Choose exactly one backend");
fn run()->Result<(),Error> {
    let session=Session::new(BACKEND,4)?; let node=session.node("rust_tf");
    let mut lookup_owner=None; let mut broadcast_owner=None; let mut graph_owner=None; let mut clock_owner=None;
    let result=(|| {
        lookup_owner=Some(node.tf("transform-lookup")?); let mut scratch=[0;4096];
        let transform=lookup_owner.as_ref().unwrap().lookup_transform(0,&mut scratch)?;
        broadcast_owner=Some(node.tf("transform-broadcast")?);
        broadcast_owner.as_ref().unwrap().broadcast_transform(&transform)?;
        graph_owner=Some(node.graph("nodes-list")?);
        println!("filtered graph {}",graph_owner.as_ref().unwrap().snapshot(Operation::GraphListNodes,&mut scratch)?);
        clock_owner=Some(node.clock("ros-clock")?);
        println!("exact ROS nanoseconds {}",clock_owner.as_ref().unwrap().read(Operation::TimeReadRos)?);
        Ok(())
    })();
    let mut first=result;
    for outcome in session.close() { if let Err(error)=outcome { eprintln!("checked close status {}",error.code); if first.is_ok() { first=Err(error); } } }
    first
}
fn main(){ if let Err(error)=run(){ eprintln!("ROS status {}",error.code); std::process::exit(1); } }
