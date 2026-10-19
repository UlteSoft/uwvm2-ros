//! Source-only generic complete-record telemetry over the actual Rust Guest SDK.
//! Build with alloc and exactly one ROS feature. T is a freshly generated
//! Canonical record; the model observer sees only a completely decoded value.
//! This component does not publish commands, derive robot identity or add grants.
extern crate roswasm;
extern crate alloc;
use roswasm::{Backend,Capability,Error,Session};
use roswasm::node::{Node,Executor};
use roswasm::typed::Canonical;
use core::cell::Cell;
use core::marker::PhantomData;
use alloc::rc::Rc;
pub struct Telemetry<T:Canonical+'static> {
    session:Session,node:Node<'static>,executor:Executor,
    observations:Rc<Cell<(u32,u32)>>,backend:Backend,
    started:bool,closed:bool,denied:bool,status:u32,cleanup:u32,polls:u32,
    bound:usize,_record:PhantomData<T>,
}
impl<T:Canonical+'static> Telemetry<T> {
    /// Session::new checks actual ABI version and reserves one resource. No
    /// alias opens here. Max wire bytes must come from the complete generator.
    pub fn new(backend:Backend,maximum:usize)->Result<Self,Error> {
        if maximum==0||maximum>32768{return Err(Error::new(90));}
        let session=Session::new(backend,1)?;
        let node=session.node("vendor-typed-telemetry");
        let executor=Executor::try_new(maximum)?;
        Ok(Self {session,node,executor,observations:Rc::new(Cell::new((0,0))),
            backend,started:false,closed:false,denied:false,status:0,cleanup:0,polls:0,bound:maximum,_record:PhantomData})
    }
    /// The separately source-locked observer validates/exposes actual vendor
    /// semantics; it must not make recursive calls into this active instance.
    pub fn initialize(&mut self,mode:u32,alias:&str,observe:fn(&T)->Result<(),Error>)->u32 {
        if self.started||self.closed{return 9;}
        if mode>1||alias.is_empty()||alias.len()>64{return 22;}
        self.started=true;
        if mode==0 {
            return match Capability::open(self.backend,alias) {
                Err(e)=>{self.denied=e.code==13;self.finish(if self.denied{0}else{e.code})},
                Ok(mut c)=>{let close=c.close();self.cleanup=close.err().map_or(0,|e|e.code);self.finish(22)}
            }
        }
        let sub=match self.node.subscription(alias){Ok(v)=>v,Err(e)=>return self.finish(e.code)};
        let state=Rc::clone(&self.observations);
        match self.executor.add_typed::<T>(sub,move|result|{
            let next=result.and_then(|value|observe(&value));
            // A checked callback failure remains an operation failure. No
            // partial record increments the delivery count. Native evidence
            // still has to come from the independent actual peer.
            let (count,previous)=state.get();match next {
                Ok(())=>state.set((count+1,previous)),Err(e)=>state.set((count,if previous!=0{previous}else{e.code}))}
        }) {Ok(())=>0,Err(e)=>self.finish(e.code)}
    }
    pub fn poll_once(&mut self)->u32 {
        if !self.started||self.closed{return 9;}
        if self.polls>=4096{return self.finish(110);}self.polls+=1;
        let count=match self.executor.spin_once(){Ok(v)=>v,Err(e)=>return self.finish(e.code)};
        let code=self.observations.get().1;
        if code!=0{return self.finish(code);}if count==0{11}else{0}
    }
    /// Close every organizer after dispatch returns. All cleanup attempts run;
    /// an earlier operation error stays primary. No close or RPC is replayed.
    fn finish(&mut self,first:u32)->u32 {
        if self.closed{return if first!=0{first}else{9};}self.closed=true;
        if let Err(e)=self.executor.close(){if self.cleanup==0{self.cleanup=e.code;}}
        for result in self.node.close().into_iter().chain(self.session.close()) {
            if let Err(e)=result {if self.cleanup==0{self.cleanup=e.code;}}
        }
        self.status=if first!=0{first}else{self.cleanup};self.status
    }
    pub fn close(&mut self)->u32{self.finish(self.status)}
    pub fn received(&self)->u32{self.observations.get().0}
    pub fn denied_verified(&self)->bool{self.denied}
    pub fn status(&self)->u32{self.status}
    pub fn cleanup_error(&self)->u32{self.cleanup}
    pub fn complete_wire_bound(&self)->usize{self.bound}
}
