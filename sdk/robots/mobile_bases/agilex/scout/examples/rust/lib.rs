//! One explicit SCOUT Status subscription over genuine Wasm ROS imports.
//! Select exactly one backend feature and the corresponding complete original
//! record module. No native ROS, CAN, credential, clock or update API is used.
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))]
compile_error!("Select exactly one of ros1 or ros2");
#[cfg(feature="ros1")]
#[path="../../../../../../custom-types/agilex-scout-ros1-complete/bounded_records.rs"]
pub mod records;
#[cfg(feature="ros2")]
#[path="../../../../../../custom-types/agilex-scout-ros2-complete/bounded_records.rs"]
pub mod records;
#[cfg(feature="ros1")]
pub use records::Ros1Scout_msgsScoutStatus as Status;
#[cfg(feature="ros2")]
pub use records::Ros2Scout_msgsMsgScoutStatus as Status;
use roswasm::{Backend,Capability};
use roswasm::typed::Canonical;
/// Retain cleanup separately even if subscribe/poll/decode already failed.
/// A Some value is a fully decoded owned record, never a partial Host layout.
pub struct Outcome {pub primary:u32,pub cleanup:u32,pub value:Option<Status>}
pub fn read_once(alias:&str)->Outcome {
    let mut bytes=[0u8;Status::MAX_WIRE_BYTES];
    #[cfg(feature="ros1")] let backend=Backend::Ros1;
    #[cfg(feature="ros2")] let backend=Backend::Ros2;
    let mut cap=match Capability::open(backend,alias) {
        Ok(c)=>c,Err(e)=>return Outcome{primary:e.code,cleanup:0,value:None},
    };
    // One nonblocking poll: idle11 stays idle. Never grow/retry after error90.
    let result=cap.subscribe().and_then(|()|cap.poll(&mut bytes))
        .and_then(|used|Status::decode(&bytes[..used]));
    let cleanup=cap.close().err().map_or(0,|e|e.code);
    match result {
        Ok(value)=>Outcome{primary:cleanup,cleanup,value:Some(value)},
        Err(e)=>Outcome{primary:e.code,cleanup,value:None},
    }
}
