//! Stage4: operation-specific service, Action, parameter, graph, clock and logger.
//! Every alias below needs its own explicit host grant; remove any operation you
//! do not intend to authorize. There are no wildcard/default capabilities. See
//! docs/rust-tinygo.md for server, TF, lifecycle and parameter-event recipes.
use roswasm::{
    typed::{ParameterKind, ParameterValue},
    Backend, Error, Operation, Session,
};
#[cfg(all(feature = "ros1", not(feature = "ros2")))]
const BACKEND: Backend = Backend::Ros1;
#[cfg(all(feature = "ros2", not(feature = "ros1")))]
const BACKEND: Backend = Backend::Ros2;
#[cfg(any(
    all(feature = "ros1", feature = "ros2"),
    not(any(feature = "ros1", feature = "ros2"))
))]
compile_error!("Choose exactly one example feature: ros1 or ros2");
fn run() -> Result<(), Error> {
    let session=Session::new(BACKEND,8)?;
    let node = session.node("rust_resources");
    // Retain owners until checked group close on both success and error paths.
    let mut service_owner=None; let mut send_owner=None; let mut get_owner=None; let mut set_owner=None; let mut graph_owner=None; let mut clock_owner=None; let mut logger_owner=None;
    let result=(|| {
    let mut scratch = [0; 1024];
    service_owner=Some(node.client("trigger-call")?); let service=service_owner.as_ref().unwrap();
    let response = service.trigger(&mut scratch)?;
    println!(
        "native service success={}, message={}",
        response.success, response.message
    );
    send_owner=Some(node.action_client("fibonacci-send")?); let send=send_owner.as_ref().unwrap();
    let goal = send.send_fibonacci_goal(5)?;
    // Result/feedback/cancel require additional independent grants. A goal ID
    // does not grant those operations, and receiving can return AGAIN/timeout.
    println!("Gateway-issued goal ID {}", goal.raw());
    get_owner=Some(node.parameters("bool-get")?); let get=get_owner.as_ref().unwrap();
    let value = get.get_value(ParameterKind::Bool, &mut scratch)?;
    set_owner=Some(node.parameters("bool-set")?); let set=set_owner.as_ref().unwrap();
    set.set_value(&ParameterValue::Bool(match value {
        ParameterValue::Bool(v) => !v,
        _ => unreachable!(),
    }))?;
    graph_owner=Some(node.graph("topics-list")?); let graph=graph_owner.as_ref().unwrap();
    println!(
        "topics: {}",
        graph.snapshot(Operation::GraphListTopics, &mut scratch)?
    );
    clock_owner=Some(node.clock("ros-clock")?); let clock=clock_owner.as_ref().unwrap();
    // Keep nanoseconds as an exact u64; never round through floating point.
    println!("ROS nanoseconds {}", clock.read(Operation::TimeReadRos)?);
    logger_owner=Some(node.logger("log-write")?); logger_owner.as_ref().unwrap()
        .write("resource example completed", &mut scratch)?;
    Ok(())
    })();
    let mut first=result;
    for outcome in session.close() { if let Err(error)=outcome { eprintln!("checked close status {}",error.code); if first.is_ok() { first=Err(error); } } }
    first
}
fn main() {
    if let Err(e) = run() {
        eprintln!("ROS error {}; effects before a timeout may remain", e.code);
        std::process::exit(1);
    }
}
