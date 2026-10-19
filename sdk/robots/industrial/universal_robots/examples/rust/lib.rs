//! Whole selected UR I/O state over genuine Wasm imports; no native UR library.
#[cfg(any(all(feature="ros1",feature="ros2"),all(feature="ros1",feature="polyscopex"),all(feature="ros2",feature="polyscopex"),not(any(feature="ros1",feature="ros2",feature="polyscopex"))))]
compile_error!("Select exactly one UR source feature");
#[cfg(feature="ros1")]
#[path="../../../../../custom-types/universal-robots-ros1-complete/bounded_records.rs"]
pub mod records;
#[cfg(feature="ros1")] pub use records::Ros1Ur_msgsIOStates as IOState;
#[cfg(feature="ros2")]
#[path="../../../../../custom-types/universal-robots-ros2-complete/bounded_records.rs"]
pub mod records;
#[cfg(feature="ros2")] pub use records::Ros2Ur_msgsMsgIOStates as IOState;
#[cfg(feature="polyscopex")]
#[path="../../../../../custom-types/universal-robots-polyscopex-complete/bounded_records.rs"]
pub mod records;
#[cfg(feature="polyscopex")] pub use records::Ros2UrinterfacesMsgDigitalIOStateStamped as IOState;
use roswasm::{Backend,Capability};use roswasm::typed::Canonical;
/// Primary remains the operation outcome; cleanup cannot overwrite it.
/// A Some value contains the complete value even if later cleanup failed.
pub struct Outcome{pub primary:u32,pub cleanup:u32,pub value:Option<IOState>}
pub fn read_once(alias:&str)->Outcome{
 let mut bytes=[0u8;IOState::MAX_WIRE_BYTES];
 #[cfg(feature="ros1")] let backend=Backend::Ros1;
 #[cfg(any(feature="ros2",feature="polyscopex"))] let backend=Backend::Ros2;
 let mut cap=match Capability::open(backend,alias){Ok(v)=>v,Err(e)=>return Outcome{primary:e.code,cleanup:0,value:None}};
 let result=cap.subscribe().and_then(|()|cap.poll(&mut bytes)).and_then(|n|IOState::decode(&bytes[..n]));
 let cleanup=cap.close().err().map_or(0,|e|e.code);
 match result{Ok(value)=>Outcome{primary:0,cleanup,value:Some(value)},Err(e)=>Outcome{primary:e.code,cleanup,value:None}}
}
