//! Complete caller-driven unified-event application, compiled separately with
//! ros1 or ros2. Explicit grants: sub=String topic.subscribe; subEvents=
//! event.observe/source=sub with topic-message,resource-error,resource-closed.
//! One poll may be idle11. This bounded lesson then closes the source, verifies
//! permitted closed metadata and checks observer cleanup; no clock/thread/retry.
//! Default deny13 is reported as failure; see sdk/docs/unified-events.md.
use roswasm::{Backend,Capability,Error,codes,events};
#[cfg(all(feature="ros1",not(feature="ros2")))] const BACKEND:Backend=Backend::Ros1;
#[cfg(all(feature="ros2",not(feature="ros1")))] const BACKEND:Backend=Backend::Ros2;
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))] compile_error!("Select exactly one backend");
fn run()->Result<(),Error>{
    let mut source=Capability::open(BACKEND,"sub")?;
    let observer=match Capability::open(BACKEND,"subEvents"){Ok(owner)=>owner,Err(primary)=>{if let Err(e)=source.close(){eprintln!("source cleanup {}",e.code);}return Err(primary)}};
    // The owned Stream performs one real watch. Construction failure retires
    // its transferred owners by Drop; no alias/legacy queue is opened implicitly.
    let mut stream=events::Stream::new(observer,source,0)?;
    let mut storage=[0u8;256];let primary=(||{let _ready=stream.readiness()?;
        let _delivered=stream.spin_once(&mut storage,|event|{
            // Body borrows storage until return. Select the actual Source codec;
            // a numeric event kind never grants a responder or cancel-any role.
            let _kind=event.kind;Ok(())
        })?;Ok(())})();
    let cleanup=stream.close_source();if let Err(e)=cleanup{eprintln!("source cleanup {}",e.code);}
    let closed=if primary.is_ok()&&cleanup.is_ok(){(||{let event=stream.poll(&mut storage)?;
        if event.kind!=events::Kind::ResourceClosed{return Err(Error::new(codes::INVALID));}Ok(())})()}else{Ok(())};
    let observed=stream.close();if let Err(e)=observed{eprintln!("observer cleanup {}",e.code);}
    primary.and(cleanup).and(closed).and(observed)
}
fn main(){if let Err(e)=run(){eprintln!("unified event status {}",e.code);std::process::exit(1);}}
