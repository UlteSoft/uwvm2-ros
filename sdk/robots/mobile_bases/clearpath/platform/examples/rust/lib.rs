//! Whole original Clearpath/Husky/Jackal observations via controlled imports.
#[cfg(all(feature="noetic",not(any(feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="jazzy",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-noetic-complete/bounded_records.rs"] pub mod bounded_records;
#[cfg(all(feature="noetic",not(any(feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="jazzy",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-noetic-complete/bounded_interfaces.rs"] pub mod bounded_interfaces;
#[cfg(all(feature="noetic",not(any(feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="jazzy",feature="main"))))] pub use bounded_records::Ros1Clearpath_platform_msgsStatus as State;
#[cfg(all(feature="noetic_husky",not(any(feature="noetic",feature="noetic_jackal",feature="humble",feature="jazzy",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-noetic-complete/bounded_records.rs"] pub mod bounded_records;
#[cfg(all(feature="noetic_husky",not(any(feature="noetic",feature="noetic_jackal",feature="humble",feature="jazzy",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-noetic-complete/bounded_interfaces.rs"] pub mod bounded_interfaces;
#[cfg(all(feature="noetic_husky",not(any(feature="noetic",feature="noetic_jackal",feature="humble",feature="jazzy",feature="main"))))] pub use bounded_records::Ros1Husky_msgsHuskyStatus as State;
#[cfg(all(feature="noetic_jackal",not(any(feature="noetic",feature="noetic_husky",feature="humble",feature="jazzy",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-noetic-complete/bounded_records.rs"] pub mod bounded_records;
#[cfg(all(feature="noetic_jackal",not(any(feature="noetic",feature="noetic_husky",feature="humble",feature="jazzy",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-noetic-complete/bounded_interfaces.rs"] pub mod bounded_interfaces;
#[cfg(all(feature="noetic_jackal",not(any(feature="noetic",feature="noetic_husky",feature="humble",feature="jazzy",feature="main"))))] pub use bounded_records::Ros1Jackal_msgsStatus as State;
#[cfg(all(feature="humble",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="jazzy",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-humble-complete/bounded_records.rs"] pub mod bounded_records;
#[cfg(all(feature="humble",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="jazzy",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-humble-complete/bounded_interfaces.rs"] pub mod bounded_interfaces;
#[cfg(all(feature="humble",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="jazzy",feature="main"))))] pub use bounded_records::Ros2Clearpath_platform_msgsMsgStatus as State;
#[cfg(all(feature="jazzy",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-jazzy-complete/bounded_records.rs"] pub mod bounded_records;
#[cfg(all(feature="jazzy",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="main"))))]
#[path="../../../../../../custom-types/clearpath-jazzy-complete/bounded_interfaces.rs"] pub mod bounded_interfaces;
#[cfg(all(feature="jazzy",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="main"))))] pub use bounded_records::Ros2Clearpath_platform_msgsMsgStatus as State;
#[cfg(all(feature="main",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="jazzy"))))]
#[path="../../../../../../custom-types/clearpath-main-complete/bounded_records.rs"] pub mod bounded_records;
#[cfg(all(feature="main",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="jazzy"))))]
#[path="../../../../../../custom-types/clearpath-main-complete/bounded_interfaces.rs"] pub mod bounded_interfaces;
#[cfg(all(feature="main",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="jazzy"))))] pub use bounded_records::Ros2Clearpath_platform_msgsMsgStatus as State;
#[cfg(not(any(all(feature="noetic",not(any(feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="jazzy",feature="main"))),all(feature="noetic_husky",not(any(feature="noetic",feature="noetic_jackal",feature="humble",feature="jazzy",feature="main"))),all(feature="noetic_jackal",not(any(feature="noetic",feature="noetic_husky",feature="humble",feature="jazzy",feature="main"))),all(feature="humble",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="jazzy",feature="main"))),all(feature="jazzy",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="main"))),all(feature="main",not(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal",feature="humble",feature="jazzy"))))))] compile_error!("Select exactly one Clearpath application");
use roswasm::{Backend,Capability};use roswasm::typed::Canonical;
/// A complete observation may survive a later cleanup failure; never drop it.
pub struct Outcome{pub primary:u32,pub cleanup:u32,pub value:Option<State>}
/// Open only the explicit alias, subscribe once, poll once, then close once.
pub fn read_once(alias:&str)->Outcome{
 let mut bytes=[0u8;State::MAX_WIRE_BYTES];
 #[cfg(any(feature="noetic",feature="noetic_husky",feature="noetic_jackal"))] let backend=Backend::Ros1;
 #[cfg(any(feature="humble",feature="jazzy",feature="main"))] let backend=Backend::Ros2;
 let mut cap=match Capability::open(backend,alias){Ok(v)=>v,Err(e)=>return Outcome{primary:e.code,cleanup:0,value:None}};
 let result=cap.subscribe().and_then(|()|cap.poll(&mut bytes)).and_then(|n|State::decode(&bytes[..n]));
 let cleanup=cap.close().err().map_or(0,|e|e.code);
 match result{Ok(value)=>Outcome{primary:0,cleanup,value:Some(value)},Err(e)=>Outcome{primary:e.code,cleanup,value:None}}
}
