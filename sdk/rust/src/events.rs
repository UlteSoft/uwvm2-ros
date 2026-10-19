//! Unified event v1 codecs. These functions allocate nothing and grant no rights.
//! Use `Capability::watch_events` with an independently opened observer/source;
//! ordinary Session/Node ownership and checked cleanup still apply. The body is
//! the source's unchanged codec, including request/goalID8 where applicable.
use crate::{codes,Error};
/// Stable seven-kind unified event selector.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
#[repr(u16)]
pub enum Kind {
    /// Canonical topic body; flags8 selects an explicitly authorized parameter projection.
    TopicMessage=1,
    /// ID8 plus request body; flags4 identifies an incoming Action source.
    ServiceRequest=2,
    /// Canonical service response; correlation is a successful submit's owned ID.
    ServiceResponse=3,
    /// ID8 plus canonical Action feedback.
    ActionFeedback=4,
    /// ID8 plus outcome and canonical Action result.
    ActionResult=5,
    /// Body-free numeric failure. Source retirement is explicitly marked.
    ResourceError=6,
    /// Body-free source-retirement notification; observer remains independently live.
    ResourceClosed=7,
}
/// Complete borrowed event. Its lifetime is tied to the caller-owned input copy.
#[derive(Debug)]
pub struct Event<'a> {
    /// Exact decoded kind.
    pub kind:Kind,
    /// Exactly1 or2, never inferred from a topic name.
    pub backend:u8,
    /// Stable flags: retired1,external-effects2,incoming-action4,parameter8.
    pub flags:u8,
    /// Numeric failure or zero for data. It carries no native payload/credentials.
    pub error:u32,
    /// Exact watched source handle in this Instance/backend.
    pub source:u64,
    /// Owned request/goal ID, or zero for topic/lifecycle metadata.
    pub correlation:u64,
    /// Strictly increasing stream delivery sequence.
    pub sequence:u64,
    /// Trusted monotonic Runtime Instance identity; never a pointer.
    pub instance:u64,
    /// Original authenticated native session, including retired-session metadata.
    pub session:u64,
    /// Unchanged canonical body; decode using the actual source type.
    pub body:&'a [u8],
}
fn integer(bytes:&[u8],at:usize,width:usize)->u64 { let mut value=0;for i in 0..width {value|=(bytes[at+i] as u64)<<(8*i);}value }
/// Validate an entire unaligned little-endian64-byte envelope transactionally.
/// Unknown flags, backend/kind/version, nonzero reserved bytes, malformed lengths,
/// zero identities and mismatched duplicated ID prefixes return22.
pub fn decode(bytes:&[u8])->Result<Event<'_>,Error> {
    let invalid=||Error::new(codes::INVALID);
    if bytes.len()<64||bytes.len()>4_194_304||integer(bytes,0,4)!=0x45525755||integer(bytes,4,2)!=1||integer(bytes,10,2)!=0 {return Err(invalid());}
    let kind=match integer(bytes,6,2) {1=>Kind::TopicMessage,2=>Kind::ServiceRequest,3=>Kind::ServiceResponse,4=>Kind::ActionFeedback,5=>Kind::ActionResult,6=>Kind::ResourceError,7=>Kind::ResourceClosed,_=>return Err(invalid())};
    let event=Event{kind,backend:bytes[8],flags:bytes[9],error:integer(bytes,12,4) as u32,source:integer(bytes,16,8),correlation:integer(bytes,24,8),sequence:integer(bytes,32,8),instance:integer(bytes,40,8),session:integer(bytes,48,8),body:&bytes[64..]};
    if !(1..=2).contains(&event.backend)||event.source==0||event.sequence==0||event.instance==0||event.session==0||integer(bytes,56,8)!=event.body.len() as u64 {return Err(invalid());}
    if matches!(kind,Kind::ResourceError|Kind::ResourceClosed) {
        if !event.body.is_empty()||event.flags&!3!=0||(kind==Kind::ResourceError&&event.error==0)||(kind==Kind::ResourceClosed&&event.flags&1==0) {return Err(invalid());}
    } else {
        if event.error!=0 {return Err(invalid());}
        if kind==Kind::TopicMessage {if event.correlation!=0||!(event.flags==0||event.flags==8) {return Err(invalid());}}
        else if event.correlation==0||(kind==Kind::ServiceRequest&&!(event.flags==0||event.flags==4))||(kind!=Kind::ServiceRequest&&event.flags!=0) {return Err(invalid());}
        if matches!(kind,Kind::ServiceRequest|Kind::ActionFeedback|Kind::ActionResult)&&(event.body.len()<8||integer(event.body,0,8)!=event.correlation) {return Err(invalid());}
    }
    Ok(event)
}
/// Full-width finite readiness snapshot. A snapshot is not reserved capacity.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct Readiness {
    /// Data/control records currently available.
    pub ready_events:u64,
    /// Complete next envelope bytes, or zero when empty.
    pub next_event_bytes:u64,
    /// Slots available for new bounded admission.
    pub admission_slots:u64,
    /// Bytes available before allocating input/task/response credit.
    pub admission_bytes:u64,
}
/// Decode exactly32 bytes, without narrowing values on wasm32.
pub fn decode_readiness(bytes:&[u8])->Result<Readiness,Error> {
    if bytes.len()!=32 {return Err(Error::new(codes::INVALID));}
    Ok(Readiness{ready_events:integer(bytes,0,8),next_event_bytes:integer(bytes,8,8),admission_slots:integer(bytes,16,8),admission_bytes:integer(bytes,24,8)})
}

#[cfg(test)]
mod tests {
    use super::*;
    fn put(bytes:&mut [u8],at:usize,value:u64,width:usize){for i in 0..width{bytes[at+i]=(value>>(8*i))as u8;}}
    fn record(kind:u16)->([u8;72],usize){
        let mut b=[0u8;72];put(&mut b,0,0x45525755,4);put(&mut b,4,1,2);put(&mut b,6,kind as u64,2);b[8]=2;
        put(&mut b,16,0xf000000000000011,8);put(&mut b,32,1,8);put(&mut b,40,0x20000000000001,8);put(&mut b,48,0xe000000000000021,8);
        if (2..=5).contains(&kind){put(&mut b,24,17,8);}
        let length=if kind==2||kind==4||kind==5{put(&mut b,64,17,8);put(&mut b,56,8,8);72}else{64};
        if kind==6{b[9]=2;put(&mut b,12,5,4);}if kind==7{b[9]=1;}(b,length)
    }
    #[test]
    fn all_seven_kinds_preserve_full_width_and_borrow(){
        for kind in 1..=7 {let (bytes,n)=record(kind);let event=decode(&bytes[..n]).unwrap();assert_eq!(event.kind as u16,kind);assert_eq!(event.backend,2);assert_eq!(event.source,0xf000000000000011);assert_eq!(event.instance,0x20000000000001);assert_eq!(event.session,0xe000000000000021);assert_eq!(event.body.as_ptr(),bytes[64..].as_ptr());}
    }
    #[test]
    fn malformed_flags_ticket_and_lengths_are_refused(){
        let (valid,n)=record(6);let mut bad=valid;bad[9]=4;assert_eq!(decode(&bad[..n]).unwrap_err().code,codes::INVALID);
        let (valid,n)=record(2);let mut bad=valid;bad[64]=18;assert_eq!(decode(&bad[..n]).unwrap_err().code,codes::INVALID);
        let (valid,n)=record(1);for at in [4,6,10,56] {let mut bad=valid;bad[at]=if at==6{8}else{2};assert_eq!(decode(&bad[..n]).unwrap_err().code,codes::INVALID);}
        let mut bad=valid;put(&mut bad,16,0,8);assert!(decode(&bad[..n]).is_err());put(&mut bad,16,1,8);put(&mut bad,32,0,8);assert!(decode(&bad[..n]).is_err());
    }
    #[test]
    fn readiness_is_exact_and_full_width(){
        let mut bytes=[0u8;32];for (i,value) in [0x20000000000001,69,2,u64::MAX].iter().enumerate(){put(&mut bytes,i*8,*value,8);}
        let value=decode_readiness(&bytes).unwrap();assert_eq!(value.ready_events,0x20000000000001);assert_eq!(value.next_event_bytes,69);assert_eq!(value.admission_slots,2);assert_eq!(value.admission_bytes,u64::MAX);assert!(decode_readiness(&bytes[..31]).is_err());
    }
}

/// Allocation-free local binding for one explicitly watched source.
/// A malformed record does not advance sequence or change the trusted session.
/// This audit never substitutes for Runtime capability/goal ownership checks.
#[derive(Clone,Copy,Debug,PartialEq,Eq)]
pub struct Binding { backend:u8, source:u64, correlation:u64, sequence:u64, instance:u64, session:u64 }
impl Binding {
    /// Initialize local state only; the first valid delivery must be sequence1.
    pub fn new(backend:u8,source:u64,correlation:u64)->Result<Self,Error> {
        if !(1..=2).contains(&backend)||source==0 {return Err(Error::new(codes::INVALID));}
        Ok(Self{backend,source,correlation,sequence:0,instance:0,session:0})
    }
    /// Validate source, contiguous sequence, session and optional owned goal.
    /// Uncorrelated error metadata is permitted on a goal-bound watch. Body
    /// storage remains borrowed from `bytes`; retain an owning copy if needed.
    pub fn decode<'a>(&mut self,bytes:&'a[u8])->Result<Event<'a>,Error> {
        let event=decode(bytes)?;
        if event.backend!=self.backend||event.source!=self.source||self.sequence.checked_add(1)!=Some(event.sequence)||
            (self.instance!=0&&(event.instance!=self.instance||event.session!=self.session))||
            (self.correlation!=0&&event.correlation!=0&&event.correlation!=self.correlation) {
            return Err(Error::new(codes::INVALID));
        }
        self.sequence=event.sequence;self.instance=event.instance;self.session=event.session;Ok(event)
    }
}

/// Owned unified-event source and independent observer, without implicit opens.
/// Both actual owners are transferred in, then exactly one watch is attempted.
/// Failed construction drops both transferred owners once; explicitly retain
/// aliases outside this API only if separate caller-controlled cleanup is needed.
/// No legacy subscribe/advertise, clock, worker thread or synchronous RPC runs.
pub struct Stream { observer:crate::Capability,source:crate::Capability,binding:Binding }
impl Stream {
    /// Bind two live owners in the same selected backend. Zero correlation is
    /// used for topics/services and metadata; outgoing Action data uses its owned ID.
    pub fn new(observer:crate::Capability,source:crate::Capability,correlation:u64)->Result<Self,Error> {
        let backend=match source.backend() {
            #[cfg(feature="ros1")] crate::Backend::Ros1=>1,
            #[cfg(feature="ros2")] crate::Backend::Ros2=>2,
        };
        let source_id=source.raw_handle()?;
        if observer.backend()!=source.backend()||observer.raw_handle()?==source_id {return Err(Error::new(codes::INVALID));}
        let binding=Binding::new(backend,source_id,correlation)?;observer.watch_events(&source,correlation)?;
        Ok(Self{observer,source,binding})
    }
    /// Receive one complete record into explicit caller storage. Capacity90
    /// retains the native record; no resize/retry or callback occurs here.
    pub fn poll<'a>(&mut self,output:&'a mut[u8])->Result<Event<'a>,Error> {
        let n=self.observer.poll_event_v1(output)?;self.binding.decode(&output[..n])
    }
    /// Dispatch one complete event on the Guest thread, returning false on idle11.
    /// The shared observer retirement lease spans the callback, including unwind;
    /// Session/Node cleanup cannot retire it while borrowed. Source operation
    /// identity remains independent. A callback failure does not replay the event.
    pub fn spin_once(&mut self,output:&mut[u8],handler:impl FnOnce(Event<'_>)->Result<(),Error>)->Result<bool,Error> {
        let n=match self.observer.poll_event_v1(output) {Ok(n)=>n,Err(e) if e.code==codes::AGAIN=>return Ok(false),Err(e)=>return Err(e)};
        let event=self.binding.decode(&output[..n])?;let _lease=self.observer.callback_guard()?;
        handler(event)?;Ok(true)
    }
    /// Observe finite credit without reserving a subsequent request.
    pub fn readiness(&self)->Result<Readiness,Error>{self.observer.event_readiness()}
    /// Submit real bounded native asynchronous work once. Its nonzero ID is
    /// admission only; inspect a later response/error event with that exact ID.
    pub fn submit(&self,input:&[u8])->Result<u64,Error>{self.source.submit_service(&self.observer,input)}
    /// Retire the source separately; observer remains available for permitted
    /// error/closed metadata. This never cancels another principal's work.
    pub fn close_source(&mut self)->Result<(),Error>{self.source.close()}
    /// Close only the observer once, preserving the real cleanup status.
    pub fn close(&mut self)->Result<(),Error>{self.observer.close()}
    /// Revoke only the observer alias, with no inferred source/cancel-any rights.
    pub fn revoke(&mut self)->Result<(),Error>{self.observer.revoke()}
}

#[cfg(feature="alloc")]
struct ExecutorEntry { stream:Stream,callback:alloc::boxed::Box<dyn for<'a> FnMut(Event<'a>)->Result<(),Error>> }
/// Bounded registering executor for unified events, enabled by `alloc`.
/// Existing legacy Subscription executors keep their original queue semantics.
/// This executor owns Stream registrations and polls each once per explicit pass;
/// no background task, implicit clock or native callback enters Wasm.
#[cfg(feature="alloc")]
pub struct Executor { entries:alloc::vec::Vec<ExecutorEntry>,scratch:alloc::vec::Vec<u8>,closed:bool }
#[cfg(feature="alloc")]
impl Executor {
    /// Allocate64..4194304 bytes including the envelope; host quotas stay fixed.
    pub fn new(capacity:usize)->Result<Self,Error>{
        if !(64..=4_194_304).contains(&capacity){return Err(Error::new(codes::INVALID));}
        Ok(Self{entries:alloc::vec::Vec::new(),scratch:alloc::vec![0;capacity],closed:false})
    }
    /// Transfer one Stream and guest callback, up to1024 registrations. A
    /// rejected transferred Stream drops its owners; check setup errors first.
    pub fn add(&mut self,stream:Stream,callback:impl for<'a> FnMut(Event<'a>)->Result<(),Error>+'static)->Result<(),Error>{
        if self.closed{return Err(Error::new(codes::BAD_HANDLE));}if self.entries.len()>=1024{return Err(Error::new(codes::TOO_LARGE));}
        self.entries.push(ExecutorEntry{stream,callback:alloc::boxed::Box::new(callback)});Ok(())
    }
    /// Remove a Stream without closing either of its owners. Rust's exclusive
    /// borrow prevents registration mutation or nested spin during this pass.
    pub fn remove(&mut self,index:usize)->Result<Stream,Error>{
        if self.closed{return Err(Error::new(codes::BAD_HANDLE));}if index>=self.entries.len(){return Err(Error::new(codes::INVALID));}
        Ok(self.entries.remove(index).stream)
    }
    /// Poll each registration once; idle is skipped and the first actual
    /// failure ends the pass. Previously consumed events are never replayed.
    pub fn spin_once(&mut self)->Result<usize,Error>{
        if self.closed{return Err(Error::new(codes::BAD_HANDLE));}let mut delivered=0;
        for entry in &mut self.entries{let callback=&mut entry.callback;if entry.stream.spin_once(&mut self.scratch,|event|callback(event))?{delivered+=1;}}
        Ok(delivered)
    }
    /// Attempt every live observer AND transferred source close once, then
    /// retire registrations. Report the first error without skipping cleanup.
    pub fn close(&mut self)->Result<(),Error>{
        if self.closed{return Err(Error::new(codes::BAD_HANDLE));}self.closed=true;let mut first=None;
        for entry in &mut self.entries{for owner in [&mut entry.stream.observer,&mut entry.stream.source]{if owner.is_open(){if let Err(e)=owner.close(){if first.is_none(){first=Some(e);}}}}}
        self.entries.clear();match first{Some(e)=>Err(e),None=>Ok(())}
    }
}

#[cfg(test)]
mod binding_tests {
    use super::*;
    fn record(sequence:u64)->[u8;64]{let mut b=[0;64];b[..4].copy_from_slice(b"UWRE");b[4]=1;b[6]=1;b[8]=2;
        for (at,n) in [(16,2u64),(32,sequence),(40,17),(48,19)]{b[at..at+8].copy_from_slice(&n.to_le_bytes());}b}
    #[test]
    fn binding_commits_only_complete_exact_source_session_sequence(){
        let mut binding=Binding::new(2,2,0).unwrap();assert_eq!(binding.decode(&record(1)).unwrap().sequence,1);
        let previous=binding;assert!(binding.decode(&record(1)).is_err());assert_eq!(binding,previous);
        for (offset,value) in [(16,3u64),(48,20),(56,1)]{let mut bad=record(2);bad[offset..offset+8].copy_from_slice(&value.to_le_bytes());
            assert_eq!(binding.decode(&bad).unwrap_err().code,codes::INVALID);assert_eq!(binding,previous);}
        assert_eq!(binding.decode(&record(2)).unwrap().sequence,2);
    }
    #[test]
    fn true_sequence_exhaustion_does_not_wrap(){
        let mut binding=Binding::new(2,2,0).unwrap();binding.sequence=u64::MAX;let previous=binding;
        assert!(binding.decode(&record(1)).is_err());assert_eq!(binding,previous);
        assert!(Binding::new(0,2,0).is_err());assert!(Binding::new(2,0,0).is_err());
    }
}
