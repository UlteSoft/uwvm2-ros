// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Helper ownership: Life owns one real Node, optional Publisher/Subscription,
// and optional Executor with32768-byte copied scratch. Executor owns moved
// subscriptions. close preserves the first error, then closes remaining local
// owners and Node ledger; polling increments only on an explicit caller pass.
// This module exports no Wasm ABI by itself; the selected entry owns CASE.
// Ordered caller protocol: validate the real binary and explicitly initialize
// its selected reactor runtime first; use one fresh instance for mode0 denial
// and another for mode1 positive behavior. Then initialize_case once, explicitly
// submit for topic RPC, poll_once at most4096 times, inspect a received value,
// and close_case after the polling/callback stack has returned. No retry loop,
// clock, thread, goroutine or native endpoint is created by this source itself.
// A successful factory or codec operation does not prove native transport.
// The trusted external Runtime policy supplies rights; this code grants none.
// Status convention:0 means the documented operation succeeded;11 is an idle
// poll;9 rejects unused/closed/repeated lifecycle operations;22 rejects invalid
// configuration or message data;90 is a capacity bound;95 rejects the selected
// ABI when checked;110 exhausts the finite poll budget. Original SDK errors are
// preserved. Rust can additionally return17 for a conflicting state borrow.
// Expected mode0 alias denial13 is translated to0 ONLY with denied_verified=1.
// All acquired owners are explicitly retired on ordinary completion/failure,
// attempting later closes after an earlier close error. Traps/panics/kills still
// require the external process owner to finish actual Runtime/Gateway cleanup.
// Rust ownership note: the entry borrows its single CASE with try_borrow_mut.
// No callback may recursively invoke this instance. Complete generated records
// are a #[path] module so their original inner module documentation is preserved.

//! Local single-use reactor ownership. No native callback/thread/timer exists.
//! All typed operations use the selected ROS2 SDK; a Node label grants nothing.
use roswasm::Backend;
use roswasm::node::{Node, Publisher, Subscription, Executor};
pub struct Life {
    pub node: Node<'static>, pub publisher: Option<Publisher>, pub subscription: Option<Subscription>,
    pub executor: Option<Executor>,
    pub scratch: Vec<u8>, pub closed: bool, pub status: u32, pub polls: u32,
    pub received: u32, pub denied: bool,
}
impl Life {
    pub fn new() -> Self { Self {node:Node::new(Backend::Ros2,"unitree-typed-example"),publisher:None,
        subscription:None,executor:None,scratch:vec![0;32768],closed:false,status:0,polls:0,received:0,denied:false} }
    /// Attempt every acquired close, preserving the original operation error.
    /// Node.close sees the shared retired owners and does not replay a close.
    pub fn close(&mut self,mut first:u32)->u32 {
        if self.closed {return if first!=0 {first} else {9};} self.closed=true;
        if let Some(mut executor)=self.executor.take(){if let Err(e)=executor.close(){if first==0{first=e.code;}}}
        if let Some(mut sub)=self.subscription.take(){if let Err(e)=sub.close(){if first==0{first=e.code;}}}
        if let Some(mut publisher)=self.publisher.take(){if let Err(e)=publisher.close(){if first==0{first=e.code;}}}
        for result in self.node.close(){if let Err(e)=result{if first==0{first=e.code;}}}
        self.status=first;first
    }
    pub fn denied(&mut self,code:u32)->u32 {self.denied=code==13;self.close(if self.denied{0}else{code})}
    pub fn polling(&mut self)->Result<(),u32>{if self.closed{return Err(9);}if self.polls==4096{return Err(self.close(110));}self.polls+=1;Ok(())}
}
