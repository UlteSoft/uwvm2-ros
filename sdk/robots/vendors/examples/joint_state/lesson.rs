//! Runnable WASI command over the actual Rust SDK and complete generated type.
//! Enable exactly ros1/ros2 plus SDK alloc; feature positive is explicit mode1.
//! Each R byte gives one poll credit, max64. No autonomous retry or callback I/O.
#[path="../../../../custom-types/generated-official-standard/bounded_records.rs"] mod records;
#[path="../telemetry.rs"] mod telemetry;
use roswasm::{Backend,Error};
use std::io::{Read,Write};
#[cfg(all(feature="ros1",not(feature="ros2")))] use records::Ros1Sensor_msgsJointState as JointState;
#[cfg(all(feature="ros2",not(feature="ros1")))] use records::Ros2Sensor_msgsMsgJointState as JointState;
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))]
compile_error!("Select exactly one JointState ROS backend");
fn observe(v:&JointState)->Result<(),Error> {
    #[cfg(feature="ros1")] let header=v.header.seq==19&&v.header.stamp.sec==42&&v.header.stamp.nsec==300000000;
    #[cfg(feature="ros2")] let header=v.header.stamp.sec == -2&&v.header.stamp.nanosec==300000000;
    if header&&v.header.frame_id=="base"&&v.name.len()==2&&v.name[0]=="left"&&v.name[1]=="right"&&
        v.position.as_slice()==[1.25,-2.5]&&v.velocity.is_empty()&&v.effort.as_slice()==[0.5,-0.25] {Ok(())}else{Err(Error::new(22))}
}
fn main()->Result<(),Error> {
    #[cfg(feature="ros1")] let backend=Backend::Ros1;
    #[cfg(feature="ros2")] let backend=Backend::Ros2;
    let positive=cfg!(feature="positive");
    let mut owner=telemetry::Telemetry::<JointState>::new(backend,JointState::MAX_WIRE_BYTES)?;
    let mut first=owner.initialize(if positive{1}else{0},"joint-state",observe);
    if !positive {if first==0&&(!owner.denied_verified()||owner.received()!=0){first=22;}}
    else if first==0 {
        let mut input=std::io::stdin();
        for _ in 0..64 {
            if owner.received()!=0{break;}
            let mut gate=[0u8;1];if input.read_exact(&mut gate).is_err()||gate[0]!=b'R'{first=22;break;}
            first=owner.poll_once();if first==11{first=0;continue;}if first!=0{break;}
        }
        if first==0&&owner.received()!=1{first=110;}
        let cleanup=owner.close();if first==0{first=cleanup;}
    }
    if first==0{first=owner.cleanup_error();}if first!=0{return Err(Error::new(first));}
    let marker=if positive{b"PASS_JOINT_STATE_FULL_VALUE\n".as_slice()}else{b"PASS_JOINT_STATE_DEFAULT_DENIED\n".as_slice()};
    let mut output=std::io::stdout();output.write_all(marker).and_then(|_|output.flush()).map_err(|_|Error::new(107))
}
