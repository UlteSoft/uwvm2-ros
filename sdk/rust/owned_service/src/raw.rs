//! Exact optional raw12 imports. The original81 business ABI is unchanged.
//! All memory indices are i32/u32; offsets/lengths/handles/tickets are i64/u64.
//! The memory-zero convenience transport refuses non-wasm32 before ANY import.
use crate::Transport;

#[cfg_attr(target_family="wasm",link(wasm_import_module="uwvm_ros2_rmw_srv_v1"))]
extern "C" {
    pub fn abi_version()->u32;
    pub fn client_activate(source:u64,observer:u64,memory:u32,receipt:u64)->u32;
    pub fn service_activate(source:u64,responder:u64,memory:u32,receipt:u64)->u32;
    pub fn endpoint_inspect(source:u64,incarnation:u64,memory:u32,receipt:u64)->u32;
    pub fn client_submit(source:u64,incarnation:u64,memory:u32,input:u64,length:u64,ticket:u64)->u32;
    pub fn client_abandon(source:u64,incarnation:u64,ticket:u64)->u32;
    pub fn service_respond(source:u64,incarnation:u64,responder:u64,ticket:u64,memory:u32,input:u64,length:u64)->u32;
    pub fn endpoint_peek(source:u64,incarnation:u64,writer:u32,memory:u32,packet:u64,capacity:u64)->u32;
    pub fn endpoint_commit(source:u64,incarnation:u64,sequence:u64,reservation:u64)->u32;
    pub fn endpoint_cancel(source:u64,incarnation:u64,sequence:u64,reservation:u64)->u32;
    pub fn client_available(source:u64,incarnation:u64,availability:u64,wait_ns:u64,memory:u32,available:u64)->u32;
    pub fn endpoint_retire(source:u64,incarnation:u64)->u32;
}
fn offset<T>(p:*const T)->u64 {p as usize as u64}
/// Genuine synchronous numeric memory-zero transport. This is not a mock or a
/// provider loader. Host policy still independently authorizes every operation.
pub struct Wasm;
macro_rules! actual {($body:expr)=>{{
    #[cfg(all(target_arch="wasm32",target_pointer_width="32"))]{unsafe{$body}}
    #[cfg(not(all(target_arch="wasm32",target_pointer_width="32")))]{95}
}};}
// SAFETY: Host calls copy complete slices synchronously; live Rust references
// cover every imported memory range. No GC object/native pointer is passed.
unsafe impl Transport for Wasm {
 fn supported(&self)->bool{cfg!(all(target_arch="wasm32",target_pointer_width="32"))}
 fn version(&mut self)->u32{actual!(abi_version())}
 fn business_version(&mut self)->u32{actual!(roswasm::raw::ros2::abi_version())}
 fn open(&mut self,a:&[u8],out:&mut[u8;8])->u32{actual!(roswasm::raw::ros2::capability_open(0,offset(a.as_ptr()),a.len()as u64,offset(out.as_mut_ptr())))}
 fn close(&mut self,h:u64)->u32{actual!(roswasm::raw::ros2::capability_close(h))}
 fn activate(&mut self,role:u8,h:u64,p:u64,out:&mut[u8;256])->u32{if role==3{actual!(client_activate(h,p,0,offset(out.as_mut_ptr())))}else if role==4{actual!(service_activate(h,p,0,offset(out.as_mut_ptr())))}else{95}}
 fn inspect(&mut self,h:u64,i:u64,out:&mut[u8;256])->u32{actual!(endpoint_inspect(h,i,0,offset(out.as_mut_ptr())))}
 fn submit(&mut self,h:u64,i:u64,b:&[u8],out:&mut[u8;8])->u32{actual!(client_submit(h,i,0,offset(b.as_ptr()),b.len()as u64,offset(out.as_mut_ptr())))}
 fn abandon(&mut self,h:u64,i:u64,t:u64)->u32{actual!(client_abandon(h,i,t))}
 fn respond(&mut self,h:u64,i:u64,p:u64,t:u64,b:&[u8])->u32{actual!(service_respond(h,i,p,t,0,offset(b.as_ptr()),b.len()as u64))}
 fn peek(&mut self,h:u64,i:u64,b:&mut[u8])->u32{actual!(endpoint_peek(h,i,16,0,offset(b.as_mut_ptr()),b.len()as u64))}
 fn commit(&mut self,h:u64,i:u64,s:u64,n:u64)->u32{actual!(endpoint_commit(h,i,s,n))}
 fn cancel(&mut self,h:u64,i:u64,s:u64,n:u64)->u32{actual!(endpoint_cancel(h,i,s,n))}
 fn available(&mut self,h:u64,i:u64,q:u64,w:u64,out:&mut[u8;1])->u32{actual!(client_available(h,i,q,w,0,offset(out.as_mut_ptr())))}
 fn retire(&mut self,h:u64,i:u64)->u32{actual!(endpoint_retire(h,i))}
}
