//! Stage2: owned handles plus checked canonical codecs, without the alloc feature.
//! The application supplies a fixed scratch capacity; encoding returns the exact
//! initialized prefix. Host byte/rate/type quotas remain independently enforced.
use roswasm::{codec, Backend, Capability, Error};
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
    // The host grants `pub`; no endpoint name is accepted by this API.
    let mut publisher = Capability::open(BACKEND, "pub")?;
    let mut scratch = [0u8; 64];
    let result = (|| {
        let n = codec::encode_string("hello from Rust", &mut scratch)?;
        publisher.publish(&scratch[..n])
    })();
    // Always attempt checked cleanup, preserving an earlier operation failure.
    let closed = publisher.close();
    if let Err(error) = closed { eprintln!("checked close error {}", error.code); }
    result.and(closed)
}
fn main() {
    if let Err(e) = run() {
        eprintln!("ROS error {}, observed required={:?}", e.code, e.required);
        std::process::exit(1);
    }
}
