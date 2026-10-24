//! Real allocation-free Rust SDK String delivery, not a host mathematical model.
//! Build the cdylib with exactly one ros1/ros2 feature; denied selects an empty
//! ROS policy row. Only direct basic WASI stdin/stdout/exit are imported. The
//! controller's real preloaded-module gate proves the retained Worker before
//! calling this _start. P follows independent native match; each R performs one
//! actual poll. No implicit clock, random, socket or environment right exists.
#![no_std]
use roswasm::{codes, codec, node::Node, Backend, Error};
#[cfg(all(feature="ros1", not(feature="ros2")))]
use roswasm::raw::ros1 as raw;
#[cfg(all(feature="ros2", not(feature="ros1")))]
use roswasm::raw::ros2 as raw;
#[cfg(all(feature="ros1", not(feature="ros2")))]
const BACKEND: Backend=Backend::Ros1;
#[cfg(all(feature="ros2", not(feature="ros1")))]
const BACKEND: Backend=Backend::Ros2;
#[cfg(any(all(feature="ros1",feature="ros2"),not(any(feature="ros1",feature="ros2"))))]
compile_error!("select exactly one ROS backend");

#[repr(C)] struct Iovec { address: u32, length: u32 }
#[link(wasm_import_module="wasi_snapshot_preview1")]
extern "C" {
    fn fd_read(fd:u32, vectors:*const Iovec, count:u32, actual:*mut u32)->u32;
    fn fd_write(fd:u32, vectors:*const Iovec, count:u32, actual:*mut u32)->u32;
    fn proc_exit(code:u32)->!;
}
fn address<T>(value:*const T)->u64 { value as usize as u64 }
fn fail_if(condition:bool)->Result<(),Error> { if condition {Err(Error::new(codes::INVALID))}else{Ok(())} }
fn emit(text:&[u8])->Result<(),Error> {
    let vector=Iovec{address:text.as_ptr() as usize as u32,length:text.len() as u32}; let mut count=u32::MAX;
    let status=unsafe{fd_write(1,&vector,1,&mut count)};
    if status!=0 {return Err(Error::new(status));} fail_if(count!=text.len() as u32)
}
fn gate(expected:u8)->Result<(),Error> {
    let mut byte=0u8; let vector=Iovec{address:&mut byte as *mut u8 as usize as u32,length:1}; let mut count=u32::MAX;
    let status=unsafe{fd_read(0,&vector,1,&mut count)};
    if status!=0 {return Err(Error::new(status));} fail_if(count!=1 || byte!=expected)
}
fn denied(alias:&str)->Result<(),Error> {
    let mut slot=0x8877665544332211u64;
    let status=unsafe{raw::capability_open(0,address(alias.as_ptr()),alias.len() as u64,address(&mut slot))};
    fail_if(status!=codes::DENIED || slot!=0x8877665544332211)
}
fn refused_poll(handle:u64,expected:u32)->Result<(),Error> {
    let mut bytes=[0xa5u8;64]; let mut length=u64::MAX;
    let status=unsafe{raw::event_poll(handle,0,address(bytes.as_mut_ptr()),64,address(&mut length))};
    fail_if(status!=expected || length!=u64::MAX || bytes.iter().any(|b| *b!=0xa5))
}
fn run()->Result<(),Error> {
    fail_if(unsafe{raw::abi_version()}!=1)?;
    #[cfg(feature="denied")]
    { denied("pub")?;denied("sub")?;denied("ungranted")?;return emit(b"PASS_SDK_DEFAULT_DENIED\n"); }
    #[cfg(not(feature="denied"))]
    {
        let node=Node::new(BACKEND,"sdk-delivery"); let mut publisher=node.publisher("pub")?;
        let mut subscription=node.subscription("sub")?;let mut buffer=[0xa5u8;64];let mut scratch=[0u8;64];
        let old_pub=publisher.raw_handle()?;let old_sub=subscription.raw_handle()?;
        let result=(|| {
            denied("ungranted")?;refused_poll(old_pub,codes::DENIED)?;
            publisher.publish_string("warmup-js-relay",&mut scratch)?;emit(b"SDK_ENDPOINTS_CREATED\n")?;gate(b'P')?;
            publisher.publish_string("roswasm-js-relay-out",&mut scratch)?;emit(b"SDK_FINAL_SENT\n")?;
            let mut received=false;
            for _ in 0..200 {
                gate(b'R')?;buffer.fill(0xa5);
                match subscription.poll(&mut buffer) {
                    Err(error) if error.code==codes::AGAIN=>{fail_if(buffer.iter().any(|b| *b!=0xa5))?;emit(b"SDK_POLL_AGAIN\n")?;},
                    Err(error)=>return Err(error),
                    Ok(count)=>{
                        fail_if(count!=23 || &buffer[..23]!=b"\x13\0\0\0roswasm-js-relay-in" || buffer[23..].iter().any(|b|*b!=0xa5))?;
                        fail_if(codec::decode_string(&buffer[..count])? != "roswasm-js-relay-in")?;
                        received=true;break;
                    }
                }
            }
            fail_if(!received)?;emit(b"SDK_DELIVERED\n")
        })();
        // Both owners retire even when the earlier operation failed. A no-alloc
        // Node is an organizer; it does not implicitly own these two handles.
        let sub_close=subscription.close();let pub_close=publisher.close();let node_close=node.close();
        result.and(sub_close).and(pub_close).and(node_close)?;
        refused_poll(old_pub,codes::BAD_HANDLE)?;refused_poll(old_sub,codes::BAD_HANDLE)?;
        emit(b"PASS_SDK_DELIVERY\n")
    }
}
/// Freestanding entry executes real SDK calls and reports failures through exit.
#[no_mangle] pub extern "C" fn _start()->! {
    let status=match run(){Ok(())=>0,Err(error)=>error.code};unsafe{proc_exit(status)}
}
#[panic_handler] fn panic(_: &core::panic::PanicInfo<'_>)->! {unsafe{proc_exit(codes::INVALID)}}
