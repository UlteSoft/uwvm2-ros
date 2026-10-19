//! Runnable Rust custom typed server composition over four private Capability owners.
//!
//! This entry genuinely advertises Echo and Digest, decodes complete generated
//! request/Goal components, preserves the exact nonzero ticket, responds once,
//! and checks all reverse cleanup. Handler functions accept only owned typed
//! data; they never receive capability references or native pointers. Safe Rust
//! borrowing retains the role group throughout dispatch. Builtin Trigger and
//! Fibonacci server assumptions are not used. Basic stdin R drives <=200 passes.
#[path="../generated-bounded-v2/bounded_records.rs"] mod bounded;
use roswasm::{Backend,Capability,Error,Operation};use roswasm::typed::Canonical;
use std::io::{Read,Write};
#[cfg(all(feature="ros1",not(feature="ros2")))]
use bounded::{Ros1Uwvm_ros_bounded_v2Envelope as Envelope,Ros1Uwvm_ros_bounded_v2Point as Point,Ros1Uwvm_ros_bounded_v2EchoRequest as Request,Ros1Uwvm_ros_bounded_v2EchoResponse as Response,Ros1Uwvm_ros_bounded_v2DigestGoal as Goal,Ros1Uwvm_ros_bounded_v2DigestFeedback as Feedback,Ros1Uwvm_ros_bounded_v2DigestResult as ResultValue};
#[cfg(all(feature="ros2",not(feature="ros1")))]
use bounded::{Ros2Uwvm_ros_bounded_v2MsgEnvelope as Envelope,Ros2Uwvm_ros_bounded_v2MsgPoint as Point,Ros2Uwvm_ros_bounded_v2SrvEcho_Request as Request,Ros2Uwvm_ros_bounded_v2SrvEcho_Response as Response,Ros2Uwvm_ros_bounded_v2ActionDigest_Goal as Goal,Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback as Feedback,Ros2Uwvm_ros_bounded_v2ActionDigest_Result as ResultValue};
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))]compile_error!("Select exactly one custom backend");
fn invalid()->Error{Error::new(22)}
fn sample()->Envelope{Envelope{label:"bounded-native".into(),samples:vec![3,5,8],points:vec![Point{value:-7,enabled:true},Point{value:11,enabled:false}],labels:["first".into(),"second".into()]}}
fn marker(text:&str)->Result<(),Error>{let mut out=std::io::stdout();out.write_all(text.as_bytes()).map_err(|_|Error::new(107))?;out.flush().map_err(|_|Error::new(107))}
/// Complete typed native Service handler; reject semantic mismatch before responding.
fn service(request:Request)->Result<Response,Error>{if request.limit!=32||request.sample!=sample(){return Err(invalid());}Ok(Response{accepted:true,echoed:request.sample})}
/// Complete typed native Goal handler, producing exactly one feedback and terminal body.
fn action(goal:Goal)->Result<(Feedback,ResultValue),Error>{if goal.sample!=sample(){return Err(invalid());}Ok((Feedback{processed:3,phase:"accepted".into()},ResultValue{accepted:true,sum:16,note:"bounded-native-result".into()}))}
/// Construct a Runtime envelope only after full component validation/encoding.
fn framed(id:u64,kind:Option<u8>,body:&[u8])->Result<Vec<u8>,Error>{if id==0{return Err(invalid());}let mut out=Vec::with_capacity(8+usize::from(kind.is_some())+body.len());out.extend_from_slice(&id.to_le_bytes());if let Some(value)=kind{out.push(value);}out.extend_from_slice(body);Ok(out)}
fn execute(caps:&[Capability])->Result<(),Error>{
    caps[0].advertise_service()?;let mut empty=[];if caps[2].request(Operation::ActionAdvertise,&[],&mut empty)?!=0{return Err(invalid());}marker("SDK_BOUNDED_SERVERS_READY\n")?;
    let mut services=0;let mut goals=0;let mut input=std::io::stdin();
    for _ in 0..200{
        let mut gate=[0u8;1];input.read_exact(&mut gate).map_err(|_|invalid())?;if gate[0]!=b'R'{return Err(invalid());}
        if services==0{
            let mut wire=vec![0u8;8+Request::MAX_WIRE_BYTES];
            match caps[0].poll(&mut wire){Err(error) if error.code==11=>{},Err(error)=>return Err(error),Ok(n)=>{if n<8{return Err(invalid());}let id=u64::from_le_bytes(wire[..8].try_into().map_err(|_|invalid())?);let response=service(Request::decode(&wire[8..n])?)?;let body=response.encode()?;let wire=framed(id,None,&body)?;if caps[1].request(Operation::ServiceRespond,&wire,&mut empty)?!=0{return Err(invalid());}services+=1;}}
        }
        if goals==0{
            let mut wire=vec![0u8;8+Goal::MAX_WIRE_BYTES];
            match caps[2].poll(&mut wire){Err(error) if error.code==11=>{},Err(error)=>return Err(error),Ok(n)=>{if n<8{return Err(invalid());}let id=u64::from_le_bytes(wire[..8].try_into().map_err(|_|invalid())?);let (feedback,result)=action(Goal::decode(&wire[8..n])?)?;let fb=feedback.encode()?;let rp=result.encode()?;let fb=framed(id,Some(1),&fb)?;let rp=framed(id,Some(2),&rp)?;if caps[3].request(Operation::ActionRespond,&fb,&mut empty)?!=0{return Err(invalid());}
                // The supervisor waits for independent native feedback receipt
                // before E. Retain both roles and the encoded result until then;
                // neither the successful response import nor this marker proves
                // that feedback already arrived through the other ROS channel.
                marker("SDK_BOUNDED_FEEDBACK_SENT\n")?;
                let mut terminal_gate=[0u8;1];input.read_exact(&mut terminal_gate).map_err(|_|invalid())?;
                if terminal_gate[0]!=b'E'{return Err(invalid());}
                if caps[3].request(Operation::ActionRespond,&rp,&mut empty)?!=0{return Err(invalid());}goals+=1;}}
        }
        if services==1&&goals==1{marker("SDK_BOUNDED_REPLIES_SENT\n")?;let mut gate=[0u8;1];input.read_exact(&mut gate).map_err(|_|invalid())?;if gate[0]!=b'D'{return Err(invalid());}return Ok(());}marker("SDK_BOUNDED_AGAIN\n")?;
    }
    Err(Error::new(110))
}
fn main()->Result<(),Error>{
    #[cfg(feature="ros1")]let backend=Backend::Ros1;#[cfg(feature="ros2")]let backend=Backend::Ros2;
    if backend.abi_version()!=1{return Err(Error::new(95));}let mut caps=Vec::new();let mut primary=Ok(());
    for alias in ["bounded-service-events","bounded-service-replies","bounded-action-events","bounded-action-replies"]{match Capability::open(backend,alias){Ok(cap)=>caps.push(cap),Err(error)=>{primary=Err(error);break;}}}
    if primary.is_ok(){primary=execute(&caps);}for cap in caps.iter_mut().rev(){let cleanup=cap.close();if primary.is_ok(){primary=cleanup;}}primary?;marker("PASS_BOUNDED_CUSTOM_SERVER\n")
}
