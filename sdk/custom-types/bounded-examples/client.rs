//! Complete custom bounded Service/Action calls using existing Rust Capability ownership.
//!
//! Copy fresh bounded_records.rs beside this source or retain the documented
//! relative generated-bounded-v2 directory. Enable exactly ros1 or ros2 plus
//! alloc, using a temporary serial Cargo workspace. Every alias is explicit;
//! builtin Node::Client/Fibonacci factories are not reinterpreted here. Caller
//! stdin R releases <=200 poll passes; there are no guest clocks/threads/retries.
//! Standard WASI startup/stdio imports require inspection in the unified lane.
//! The native peer and Gateway must use actual official root/component identities.
#[path="../generated-bounded-v2/bounded_records.rs"] mod bounded;
use roswasm::{Backend,Capability,Error,Operation};
use roswasm::typed::Canonical;
use std::io::{Read,Write};
#[cfg(all(feature="ros1",not(feature="ros2")))]
use bounded::{Ros1Uwvm_ros_bounded_v2Envelope as Envelope,Ros1Uwvm_ros_bounded_v2Point as Point,Ros1Uwvm_ros_bounded_v2EchoRequest as Request,Ros1Uwvm_ros_bounded_v2EchoResponse as Response,Ros1Uwvm_ros_bounded_v2EmptyRequest as EmptyRequest,Ros1Uwvm_ros_bounded_v2EmptyResponse as EmptyResponse,Ros1Uwvm_ros_bounded_v2DigestGoal as Goal,Ros1Uwvm_ros_bounded_v2DigestFeedback as Feedback,Ros1Uwvm_ros_bounded_v2DigestResult as ResultValue};
#[cfg(all(feature="ros2",not(feature="ros1")))]
use bounded::{Ros2Uwvm_ros_bounded_v2MsgEnvelope as Envelope,Ros2Uwvm_ros_bounded_v2MsgPoint as Point,Ros2Uwvm_ros_bounded_v2SrvEcho_Request as Request,Ros2Uwvm_ros_bounded_v2SrvEcho_Response as Response,Ros2Uwvm_ros_bounded_v2SrvEmpty_Request as EmptyRequest,Ros2Uwvm_ros_bounded_v2SrvEmpty_Response as EmptyResponse,Ros2Uwvm_ros_bounded_v2ActionDigest_Goal as Goal,Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback as Feedback,Ros2Uwvm_ros_bounded_v2ActionDigest_Result as ResultValue};
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))]
compile_error!("Select exactly one custom bounded ROS backend");
fn invalid()->Error {Error::new(22)}
fn sample()->Envelope {Envelope{label:"bounded-native".into(),samples:vec![3,5,8],points:vec![Point{value:-7,enabled:true},Point{value:11,enabled:false}],labels:["first".into(),"second".into()]}}
/// Check every basic stdout write/flush; a failed diagnostic is not a successful test.
fn marker(value:&str)->Result<(),Error> {let mut out=std::io::stdout();out.write_all(value.as_bytes()).map_err(|_|Error::new(107))?;out.flush().map_err(|_|Error::new(107))}
/// These borrowed owners remain live for the complete one-shot operation flow.
fn execute(caps:&[Capability])->Result<(),Error> {
    let mut output=vec![0u8;Response::MAX_WIRE_BYTES];
    let bytes=Request{sample:sample(),limit:32}.encode()?;
    let n=caps[0].request(Operation::ServiceCall,&bytes,&mut output)?;
    let response=Response::decode(&output[..n])?;if !response.accepted||response.echoed!=sample(){return Err(invalid());}
    let bytes=EmptyRequest::default().encode()?;let mut empty=[];
    let n=caps[1].request(Operation::ServiceCall,&bytes,&mut empty)?;EmptyResponse::decode(&empty[..n])?;
    let bytes=Goal{sample:sample()}.encode()?;let mut id_bytes=[0u8;8];
    let n=caps[2].request(Operation::ActionSendGoal,&bytes,&mut id_bytes)?;
    if n!=8||u64::from_le_bytes(id_bytes)==0{return Err(invalid());}
    marker("SDK_BOUNDED_GOAL_SENT\n")?;let mut got_feedback=false;
    let mut input=std::io::stdin();
    for _ in 0..200 {
        let mut gate=[0u8;1];input.read_exact(&mut gate).map_err(|_|invalid())?;if gate[0]!=b'R'{return Err(invalid());}
        if !got_feedback {
            let mut bytes=vec![0u8;8+Feedback::MAX_WIRE_BYTES];
            let n=match caps[4].request(Operation::ActionReceiveFeedback,&id_bytes,&mut bytes){Err(error) if error.code==11=>{marker("SDK_BOUNDED_AGAIN\n")?;continue;},Err(error)=>return Err(error),Ok(n)=>n};
            if n<8||bytes[..8]!=id_bytes{return Err(invalid());}let value=Feedback::decode(&bytes[8..n])?;if value.processed!=3||value.phase!="accepted"{return Err(invalid());}got_feedback=true;
            // Confirm exact feedback before releasing terminal-result delivery
            // on the independent native channel. This E gate grants no ROS
            // authority, retries no operation and occurs exactly once.
            marker("SDK_BOUNDED_FEEDBACK_DELIVERED\n")?;
            let mut terminal_gate=[0u8;1];input.read_exact(&mut terminal_gate).map_err(|_|invalid())?;
            if terminal_gate[0]!=b'E'{return Err(invalid());}
        }
        let mut bytes=vec![0u8;9+ResultValue::MAX_WIRE_BYTES];
        let n=match caps[5].request(Operation::ActionReceiveResult,&id_bytes,&mut bytes){Err(error) if error.code==11=>{marker("SDK_BOUNDED_AGAIN\n")?;continue;},Err(error)=>return Err(error),Ok(n)=>n};
        if n<9||bytes[..8]!=id_bytes||bytes[8]!=2{return Err(invalid());}let value=ResultValue::decode(&bytes[9..n])?;
        if !value.accepted||value.sum!=16||value.note!="bounded-native-result"{return Err(invalid());}return Ok(());
    }
    Err(Error::new(110))
}
fn main()->Result<(),Error> {
    #[cfg(feature="ros1")] let backend=Backend::Ros1;
    #[cfg(feature="ros2")] let backend=Backend::Ros2;
    if backend.abi_version()!=1{return Err(Error::new(95));}
    let mut caps=Vec::new();let mut primary=Ok(());
    for alias in ["bounded-echo","bounded-empty","bounded-send","bounded-cancel","bounded-feedback","bounded-result"] {
        match Capability::open(backend,alias){Ok(cap)=>caps.push(cap),Err(error)=>{primary=Err(error);break;}}
    }
    if primary.is_ok(){primary=execute(&caps);}
    // Close every acquired owner in reverse, even after an operation or earlier
    // cleanup failure. Preserve the first error without retrying a retired ID.
    for cap in caps.iter_mut().rev(){let result=cap.close();if primary.is_ok(){primary=result;}}
    primary?;marker("PASS_BOUNDED_CUSTOM_CLIENT\n")
}
