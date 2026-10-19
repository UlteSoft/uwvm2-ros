//! Wasm-only production Stream/Executor contract for a finite mocked import host.
//! This cdylib exports `contract`; the test host must implement the exact ROS2
//! open/watch/poll/close signatures and return one valid source2 envelope. No real
//! Gateway, permissions or ROS delivery is claimed. Build with the real current
//! SDK crate, features exactly ros2,alloc; never run against production grants.
use roswasm::{Backend,Capability,Error,codes,events};
use std::{rc::Rc,cell::Cell};
#[no_mangle]
pub extern "C" fn contract()->u32{
    fn run()->Result<(),Error>{
        let observer=Capability::open(Backend::Ros2,"observer")?;
        let source=Capability::open(Backend::Ros2,"source")?;
        let stream=events::Stream::new(observer,source,0)?;let calls=Rc::new(Cell::new(0));let count=calls.clone();
        let mut executor=events::Executor::new(64)?;
        executor.add(stream,move|event|{if event.source!=2||event.sequence!=1||!event.body.is_empty(){return Err(Error::new(codes::INVALID));}count.set(count.get()+1);Ok(())})?;
        if executor.spin_once()?!=1||calls.get()!=1{return Err(Error::new(codes::INVALID));}
        let mut stream=executor.remove(0)?;executor.close()?;stream.close()?;stream.close_source()?;Ok(())
    }
    match run(){Ok(())=>0,Err(error)=>error.code}
}
