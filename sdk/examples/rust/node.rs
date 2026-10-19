//! Stage3: Node, Publisher and a registering poll-driven Executor.
//! Explicit aliases: `pub` publishes String and `sub` subscribes String. The
//! native topic binding/type/QoS/quotas belong in host policy. `rust_guest` below
//! is a local label, not the identity of a newly created native rclcpp node.
use roswasm::{
    codec,
    node::{Executor, Node},
    Backend, Error,
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
    let node = Node::new(BACKEND, "rust_guest");
    let mut publisher = node.publisher("pub")?;
    let mut executor = Executor::try_new(256)?;
    let callback_error=std::rc::Rc::new(std::cell::Cell::new(None));
    let observed=std::rc::Rc::clone(&callback_error);
    let result = (|| {
    let subscription = node.subscription("sub")?;
    // The callback executes during spin_once on this guest's execution thread.
    // Its borrowed bytes expire after this call; decode or copy before retaining.
    executor.add_subscription(subscription, move |bytes| match codec::decode_string(bytes) {
        Ok(text) => println!("received {text}"),
        Err(e) => { observed.set(Some(e)); eprintln!("canonical decode failed: {}", e.code); },
    })?;
    let mut scratch = [0u8; 64];
    publisher.publish_string("hello from Node", &mut scratch)?;
    // One pass can legitimately deliver zero callbacks because discovery/delivery
    // is asynchronous. An application's idle scheduling policy is explicit; do
    // not turn this example into an unbounded busy loop or retry errors blindly.
    println!("delivered {} callbacks", executor.spin_once()?);
    if let Some(error)=callback_error.take() { return Err(error); }
    Ok(())
    })();
    // Check both cleanup operations even if setup, publish or polling failed.
    // The earlier error is retained; no retired handle is replayed.
    let queues = executor.close();
    let published = publisher.close();
    if let Err(error) = queues { eprintln!("queue close status {}",error.code); }
    if let Err(error) = published { eprintln!("publisher close status {}",error.code); }
    result.and(queues).and(published)
}
fn main() {
    if let Err(e) = run() {
        eprintln!("ROS error {}", e.code);
        std::process::exit(1);
    }
}
