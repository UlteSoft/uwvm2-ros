//! Rust: fixed generated records implement the existing Canonical trait.
//!
//! Add roswasm with features `ros1` and `alloc` to the guest package and use the
//! bundled guest-only generated-r3 codecs. Real delivery requires official native identities. Host alias
//! `packet-pub` must explicitly grant the exact ROS1 native and codec identities.
//! Compile this entry with a temporary Cargo manifest selecting `ros1,alloc`
//! and wasm32-wasip1, with one build job and an output directory under `/tmp`.
//! The fixed caller-buffer encoder allocates no wire output. Standard Rust main
//! may retain separate WASI startup/stdio imports; inspect the emitted artifact.
//! stderr diagnostics use the basic stream, never a native logger capability.
//! Publish success alone does not prove delivery. A matching independent native
//! peer and qualified generated host adapter are separate acceptance prerequisites.
//! This is preparation source, not a successful compiled or native ROS test.
#[path = "../generated-r3/fixed_records.rs"]
mod fixed;
use roswasm::{Backend,Capability,Error};

/// Publish precisely one validated 23-byte record, then retire its handle.
fn main()->Result<(),Error> {
    let mut packet=fixed::Ros1Example_interfaces_customPacket::default();
    packet.sequence=7;packet.active=true;packet.measurement=1.25;
    packet.tag[0]=0x41;packet.samples[0]=-3;
    let mut bytes=[0u8;fixed::Ros1Example_interfaces_customPacket::WIRE_BYTES];
    let length=packet.encode_into(&mut bytes)?;
    let mut publisher=Capability::open(Backend::Ros1,"packet-pub")?;
    // Preserve publication as the primary outcome and inspect close on both
    // success/error paths. Never replay an admitted write or failed cleanup.
    let operation=publisher.publish(&bytes[..length]);
    let cleanup=publisher.close();
    if let Err(error)=&cleanup { eprintln!("checked close status {}",error.code); }
    // Result-returning main exits nonzero and prints the primary Error, or the
    // cleanup Error when publication succeeded. Both failures remain observable.
    match operation { Err(error)=>Err(error), Ok(())=>cleanup }
}
