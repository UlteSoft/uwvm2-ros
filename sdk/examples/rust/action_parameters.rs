//! Lesson8: operation-specific Fibonacci client and typed BoolArray parameters.
//!
//! Grant fibonacci-send/cancel-own/feedback/result separately for the same
//! supported native Action. No cancel-any grant is requested. Grant array-get,
//! array-set and parameter-list independently with exact native parameter/node,
//! type, inclusive bounds and positive element/byte/rate limits. One pass reads
//! each queue once; Again is idle. Timeout may leave an admitted effect in flight.
use roswasm::{codes, typed::{ParameterKind,ParameterValue}, Backend,Error,Session,codec};
#[cfg(all(feature="ros1",not(feature="ros2")))] const BACKEND:Backend=Backend::Ros1;
#[cfg(all(feature="ros2",not(feature="ros1")))] const BACKEND:Backend=Backend::Ros2;
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))]
compile_error!("Choose exactly one backend");
fn run()->Result<(),Error> {
    let session=Session::new(BACKEND,7)?; let node=session.node("rust_action_parameters");
    let mut send_owner=None; let mut cancel_owner=None; let mut feedback_owner=None; let mut result_owner=None;
    let mut get_owner=None; let mut set_owner=None; let mut list_owner=None;
    let result=(|| {
        send_owner=Some(node.action_client("fibonacci-send")?);
        cancel_owner=Some(node.action_client("fibonacci-cancel-own")?);
        feedback_owner=Some(node.action_client("fibonacci-feedback")?);
        result_owner=Some(node.action_client("fibonacci-result")?);
        // Only send returns the opaque ID; it cannot grant sibling operations.
        let goal=send_owner.as_ref().unwrap().send_fibonacci_goal(5)?; let mut scratch=[0;1024];
        match feedback_owner.as_ref().unwrap().feedback(goal,&mut scratch) {
            Ok(value)=>println!("feedback values {}",value.values().count()),
            Err(error) if error.code==codes::AGAIN=>println!("feedback idle"),
            Err(error)=>return Err(error),
        }
        // Cancel before consuming a terminal result: result reception retires
        // this ID in the Runtime, so a later cancel would be stale9. A false
        // native Bool is valid when work has already finished but its result
        // has not been received. Cancellation never rolls back admitted work.
        println!("own cancel accepted {}",cancel_owner.as_ref().unwrap().cancel_own(goal)?);
        match result_owner.as_ref().unwrap().result(goal,&mut scratch) {
            Ok(value)=>println!("terminal kind {}",value.kind),
            Err(error) if error.code==codes::AGAIN=>println!("result idle"),
            Err(error)=>return Err(error),
        }
        get_owner=Some(node.parameters("array-get")?);
        let value=get_owner.as_ref().unwrap().get_value(ParameterKind::BoolArray,&mut scratch)?;
        if let ParameterValue::BoolArray(values)=value { println!("array elements {}",values.len()); }
        set_owner=Some(node.parameters("array-set")?);
        set_owner.as_ref().unwrap().set_value(&ParameterValue::BoolArray(vec![true,false]))?;
        list_owner=Some(node.parameters("parameter-list")?);
        let n=list_owner.as_ref().unwrap().list(&[],&mut scratch)?; println!("filtered parameters {}",codec::decode_string(&scratch[..n])?);
        Ok(())
    })();
    let mut first=result;
    for outcome in session.close() { if let Err(error)=outcome { eprintln!("checked close status {}",error.code); if first.is_ok() { first=Err(error); } } }
    first
}
fn main(){ if let Err(error)=run() { eprintln!("ROS status {}; admitted effects may remain",error.code); std::process::exit(1); } }
