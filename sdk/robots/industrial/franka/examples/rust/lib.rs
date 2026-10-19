//! Complete selected Franka state through actual controlled Wasm imports.
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))]
compile_error!("Select exactly one Franka ROS feature");
#[cfg(feature="ros1")]
#[path="../../../../../custom-types/franka-ros1-complete/bounded_records.rs"] pub mod records;
#[cfg(feature="ros1")] pub use records::Ros1Franka_msgsFrankaState as State;
#[cfg(feature="ros2")]
#[path="../../../../../custom-types/franka-ros2-complete/bounded_records.rs"] pub mod records;
#[cfg(feature="ros2")] pub use records::Ros2Franka_msgsMsgFrankaRobotState as State;
use roswasm::{Backend,Capability};use roswasm::typed::Canonical;
/// A complete observation may survive a later cleanup failure; never drop it.
pub struct Outcome{pub primary:u32,pub cleanup:u32,pub value:Option<State>}
/// Open only the explicit alias, subscribe once, poll once, then close once.
pub fn read_once(alias:&str)->Outcome{
 let mut bytes=[0u8;State::MAX_WIRE_BYTES];
 #[cfg(feature="ros1")] let backend=Backend::Ros1;
 #[cfg(feature="ros2")] let backend=Backend::Ros2;
 let mut cap=match Capability::open(backend,alias){Ok(v)=>v,Err(e)=>return Outcome{primary:e.code,cleanup:0,value:None}};
 let result=cap.subscribe().and_then(|()|cap.poll(&mut bytes)).and_then(|n|State::decode(&bytes[..n]));
 let cleanup=cap.close().err().map_or(0,|e|e.code);
 match result{Ok(value)=>Outcome{primary:0,cleanup,value:Some(value)},Err(e)=>Outcome{primary:e.code,cleanup,value:None}}
}
