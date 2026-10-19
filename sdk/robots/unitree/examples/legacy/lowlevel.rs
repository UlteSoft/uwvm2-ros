//! Real ROS1 Rust Guest application. A same-module command entry calls these
//! exports; no WAT root or privileged preload is used for positive operations.
//! Stage ALL eleven generated records using #[path], preserving their //! docs.
//! Explicit Node.subscription activates topic_subscribe before READY/native M.
extern crate roswasm;
#[path="bounded_records.rs"] mod vendor;
mod values;
use roswasm::{Backend,Capability};
use roswasm::node::{Node,Publisher,Subscription};
use roswasm::typed::Canonical;
use vendor::Ros1Unitree_legged_msgsLowState as State;
use std::cell::RefCell;
struct Case {node:Node<'static>,publisher:Option<Publisher>,subscription:Option<Subscription>,
    scratch:[u8;1536],observed:Vec<u8>,closed:bool,status:u32,polls:u32,received:u32,denials:u32,submitted:bool}
impl Case {
    fn new()->Self{Self{node:Node::new(Backend::Ros1,"legacy-typed-example"),publisher:None,subscription:None,scratch:[0;1536],observed:Vec::new(),closed:false,status:0,polls:0,received:0,denials:0,submitted:false}}
    fn close(&mut self,mut first:u32)->u32{
        if self.closed{return if first!=0 {first}else{9};}self.closed=true;
        if let Some(mut owner)=self.subscription.take(){if let Err(e)=owner.close(){if first==0{first=e.code;}}}
        if let Some(mut owner)=self.publisher.take(){if let Err(e)=owner.close(){if first==0{first=e.code;}}}
        for result in self.node.close(){if let Err(e)=result{if first==0{first=e.code;}}}self.status=first;first
    }
    fn initialize(&mut self,mode:u32,family:u32,reserved:u32)->u32{
        if mode>1||family!=4||reserved!=0{return self.close(22);}
        if mode==0{
            for alias in ["legacy-lowcmd","legacy-lowstate"]{
                match Capability::open(Backend::Ros1,alias){Err(e) if e.code==13=>{self.denials+=1;},
                    Err(e)=>return self.close(e.code),Ok(mut cap)=>{let _=cap.close();return self.close(22);}}
            }return self.close(0);
        }
        self.publisher=match self.node.publisher("legacy-lowcmd"){Ok(v)=>Some(v),Err(e)=>return self.close(e.code)};
        self.subscription=match self.node.subscription("legacy-lowstate"){Ok(v)=>Some(v),Err(e)=>return self.close(e.code)};0
    }
}
thread_local!{static CASE:RefCell<Option<Case>>=const{RefCell::new(None)};}
fn access(f:impl FnOnce(&mut Case)->u32)->u32{CASE.with(|slot|match slot.try_borrow_mut(){Ok(mut value)=>match value.as_mut(){Some(case)=>f(case),None=>9},Err(_)=>17})}
#[no_mangle] pub extern "C" fn initialize_case(mode:u32,family:u32,reserved:u32)->u32{
    CASE.with(|slot|{let Ok(mut value)=slot.try_borrow_mut() else{return 17;};if value.is_some(){return 9;}
        let mut case=Case::new();let code=case.initialize(mode,family,reserved);*value=Some(case);code})
}
#[no_mangle] pub extern "C" fn poll_once()->u32{access(|case|{
    if case.closed||case.received!=0{return 9;}if case.polls>=4096{return case.close(110);}case.polls+=1;
    let n=match case.subscription.as_ref().unwrap().poll(&mut case.scratch){Ok(n)=>n,Err(e) if e.code==11=>return 11,Err(e)=>return case.close(e.code)};
    if n>case.scratch.len(){return case.close(22);}
    let value=match State::decode(&case.scratch[..n]){Ok(v)=>v,Err(e)=>return case.close(e.code)};
    let actual=match value.encode(){Ok(v)=>v,Err(e)=>return case.close(e.code)};
    let expected=match values::state().encode(){Ok(v)=>v,Err(e)=>return case.close(e.code)};
    if actual.is_empty()||actual.len()>1536||actual!=expected||actual.as_slice()!=&case.scratch[..n]{return case.close(22);}
    case.observed=actual;case.received=1;0})}
#[no_mangle] pub extern "C" fn submit_once()->u32{access(|case|{
    if case.closed||case.received!=1||case.submitted{return 9;}
    match case.publisher.as_ref().unwrap().publish_value(&values::command()){Err(e)=>case.close(e.code),Ok(())=>{case.submitted=true;0}}})}
#[no_mangle] pub extern "C" fn observed_length()->u32{access(|c|if c.received!=0{c.observed.len() as u32}else{0})}
#[no_mangle] pub extern "C" fn observed_byte(index:u32)->u32{access(|c|if c.received==0||index as usize>=c.observed.len(){c.status=22;0}else{c.observed[index as usize] as u32})}
#[no_mangle] pub extern "C" fn received_count()->u32{access(|c|c.received)}
#[no_mangle] pub extern "C" fn denied_count()->u32{access(|c|c.denials)}
#[no_mangle] pub extern "C" fn case_status()->u32{access(|c|c.status)}
#[no_mangle] pub extern "C" fn close_case()->u32{access(|c|c.close(0))}
