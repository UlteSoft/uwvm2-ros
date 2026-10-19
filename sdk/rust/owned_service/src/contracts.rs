//! Authored explicit LOCAL transport-model contracts. Never Native evidence.
//! No RIHS or canonical identity is invented: tests use the private open seam
//! and an intentionally unregistrable LOCAL_MODEL definition for a one-byte
//! checked value. Public real examples use actual generated root constants.
extern crate std;
use super::*;
use alloc::{vec,collections::VecDeque};
use core::cell::RefCell;
#[derive(Default)]struct Model{opens:usize,closes:Vec<u64>,retire:usize,submit:usize,respond:usize,peek:usize,commit:usize,cancel:usize,queries:usize,role:u8,source:u64,peer:u64,next_ticket:u64,next_nonce:u64,receipt:Vec<u8>,queue:VecDeque<(u64,u8,Vec<u8>)>,commit_error:u32,cancel_error:u32,retire_error:u32,close_error:u32,query_error:u32,query_byte:Option<u8>,mutate_inspect:bool,fail_open_at:usize}
struct Backend(Rc<RefCell<Model>>);
fn put(b:&mut[u8],p:usize,n:u64,w:usize){for i in 0..w{b[p+i]=(n>>(8*i))as u8;}}
fn identity(b:&mut[u8],source:u64){b[7]=2;put(b,16,source,8);put(b,24,0xf000000000000001,8);put(b,32,0xe000000000000002,8);put(b,40,0xd000000000000003,8);put(b,48,0xc000000000000004,8);}
fn original(role:u8,h:u64)->[u8;256]{let mut b=[0;256];put(&mut b,0,0x45524d55,4);put(&mut b,4,1,2);b[6]=1;put(&mut b,8,256,4);b[12]=role;b[13]=2;if role==3{b[14]=16;}identity(&mut b,h);for p in[64,144]{put(&mut b,p,1,4);put(&mut b,p+4,1,4);put(&mut b,p+8,2,4);put(&mut b,p+12,1,4);put(&mut b,p+16,1,8);}b}
fn packet(role:u8,h:u64,peer:u64,ticket:u64,kind:u8,body:&[u8],sequence:u64,nonce:u64,capacity:usize)->Vec<u8>{let mut b=vec![0;capacity];put(&mut b,0,0x43524d55,4);put(&mut b,4,1,2);b[6]=5;identity(&mut b,h);put(&mut b,8,160,4);b[12]=role;b[13]=16;put(&mut b,56,peer,8);put(&mut b,64,ticket,8);put(&mut b,72,sequence,8);put(&mut b,80,nonce,8);put(&mut b,96,capacity as u64,8);put(&mut b,104,(192+body.len())as u64,8);let m=&mut b[160..352];put(m,0,0x494d5255,4);put(m,4,1,2);m[6]=kind;identity(m,h);put(m,8,192,4);put(m,56,sequence,8);put(m,64,ticket,8);put(m,88,u64::MAX,8);put(m,96,u64::MAX,8);put(m,112,body.len()as u64,8);if kind==6||kind==7{m[122]=if kind==7{1}else{0};put(m,124,if kind==6{5}else{0},4);}else{put(m,12,35,4);put(m,72,i64::MIN as u64,8);put(m,80,0,8);put(m,104,i64::MIN as u64,8);m[121]=16;/* real all-zero writer GUID is legal model data */}b[352..352+body.len()].copy_from_slice(body);b}
// SAFETY: This is ONLY an admitted local deterministic contract model. It does
// no Native, RPC or import, copies synchronously and preserves error outputs.
unsafe impl Transport for Backend{
 fn supported(&self)->bool{true}fn version(&mut self)->u32{1}fn business_version(&mut self)->u32{1}
 fn open(&mut self,_:&[u8],out:&mut[u8;8])->u32{let mut s=self.0.borrow_mut();s.opens+=1;if s.fail_open_at==s.opens{return 13;}*out=((1u64<<32)+s.opens as u64).to_le_bytes();0}
 fn close(&mut self,h:u64)->u32{let mut s=self.0.borrow_mut();s.closes.push(h);s.close_error}
 fn activate(&mut self,r:u8,h:u64,p:u64,out:&mut[u8;256])->u32{let mut s=self.0.borrow_mut();s.role=r;s.source=h;s.peer=p;s.receipt=original(r,h).to_vec();out.copy_from_slice(&s.receipt);0}
 fn inspect(&mut self,_:u64,_:u64,out:&mut[u8;256])->u32{let s=self.0.borrow();out.copy_from_slice(&s.receipt);out[6]=2;if s.mutate_inspect{out[224]=1;}0}
 fn submit(&mut self,_:u64,_:u64,_:&[u8],out:&mut[u8;8])->u32{let mut s=self.0.borrow_mut();s.submit+=1;s.next_ticket+=1;*out=s.next_ticket.to_le_bytes();0}
 fn abandon(&mut self,_:u64,_:u64,_:u64)->u32{0}
 fn respond(&mut self,_:u64,_:u64,_:u64,_:u64,_:&[u8])->u32{self.0.borrow_mut().respond+=1;0}
 fn peek(&mut self,_:u64,_:u64,out:&mut[u8])->u32{let mut s=self.0.borrow_mut();s.peek+=1;if let Some((ticket,kind,body))=s.queue.front().cloned(){s.next_nonce+=1;let data=packet(s.role,s.source,s.peer,ticket,kind,&body,(s.commit+1)as u64,s.next_nonce,out.len());out.copy_from_slice(&data);0}else{11}}
 fn commit(&mut self,_:u64,_:u64,_:u64,_:u64)->u32{let mut s=self.0.borrow_mut();s.commit+=1;s.queue.pop_front();s.commit_error}
 fn cancel(&mut self,_:u64,_:u64,_:u64,_:u64)->u32{let mut s=self.0.borrow_mut();s.cancel+=1;s.cancel_error}
 fn available(&mut self,_:u64,_:u64,_:u64,_:u64,out:&mut[u8;1])->u32{let mut s=self.0.borrow_mut();s.queries+=1;if s.query_error!=0{return s.query_error;}if let Some(v)=s.query_byte{out[0]=v;}0}
 fn retire(&mut self,_:u64,_:u64)->u32{let mut s=self.0.borrow_mut();s.retire+=1;s.retire_error}
}
#[derive(Debug,PartialEq)]struct Value(u8);
fn encode(v:&Value)->Result<Vec<u8>,Error>{if v.0>1{return Err(err(22));}Ok(vec![v.0])}
fn decode(b:&[u8])->Result<Value,Error>{if b.len()!=1||b[0]>1{return Err(err(22));}Ok(Value(b[0]))}
fn definition()->Definition<Value,Value>{Definition{native_type:"LOCAL_MODEL_NOT_REGISTRABLE",canonical_identity:"NO_NATIVE_OR_CANONICAL_HASH_CLAIM",request:Codec{maximum:1,encode,decode},response:Codec{maximum:1,encode,decode}}}
fn group(n:usize,history:usize)->(Group<Backend>,Rc<RefCell<Model>>){let state=Rc::new(RefCell::new(Model{next_ticket:1<<60,..Model::default()}));let g=Group::prepare(Config{endpoints:n,transactions_per_endpoint:history,maximum_request:1,maximum_response:1},Backend(Rc::clone(&state))).unwrap();(g,state)}
fn client(g:&mut Group<Backend>,q:bool)->Client<Value,Value>{let endpoint=g.open(3,"call","observe",if q{Some("query")}else{None}).unwrap();Client{endpoint,definition:definition()}}
fn server(g:&mut Group<Backend>)->Server<Value,Value>{let endpoint=g.open(4,"advertise","respond",None).unwrap();Server{endpoint,definition:definition()}}
#[test]fn invalid_public_definition_has_no_open(){let(mut g,s)=group(1,4);assert!(matches!(g.client(definition(),"call","observe",None),Err(e)if e.code==22));assert_eq!(s.borrow().opens,0);}
#[test]fn role_alias_preflight_and_third_owner_are_distinct(){let(mut g,s)=group(1,4);assert_eq!(g.open(3,"same","same",None).unwrap_err().code,22);assert_eq!(g.open(3,"call","observe",Some("call")).unwrap_err().code,22);assert_eq!(s.borrow().opens,0);let c=client(&mut g,true);assert!(c.receipt(&g).is_ok());assert_eq!(s.borrow().opens,3);}
#[test]fn full64_ticket_delayed_response_same_take_metadata(){let(mut g,s)=group(1,4);let c=client(&mut g,false);let t=c.submit(&mut g,&Value(1)).unwrap();assert!(t.value()>1<<53);assert_eq!(c.state(&g,t).unwrap(),RequestState::Waiting);assert!(c.poll(&mut g).unwrap().is_none());s.borrow_mut().queue.push_back((t.value(),3,vec![1]));match c.poll(&mut g).unwrap().unwrap(){Delivery::Value{ticket,value,info}=>{assert_eq!(ticket,t);assert_eq!(value,Value(1));assert_eq!(info.native_sequence,i64::MIN);assert_eq!(info.source_timestamp,i64::MIN);assert_eq!(info.received_timestamp,0);assert_eq!(info.writer_guid,[0;16]);},_=>panic!("data required")};assert_eq!(c.state(&g,t).unwrap(),RequestState::Completed{error:0});assert_eq!(s.borrow().commit,1);}
#[test]fn response_decode_failure_cancels_before_commit_and_retires_once(){let(mut g,s)=group(1,4);let c=client(&mut g,false);let t=c.submit(&mut g,&Value(1)).unwrap();s.borrow_mut().queue.push_back((t.value(),3,vec![2]));assert_eq!(c.poll(&mut g).unwrap_err().code,22);assert_eq!(s.borrow().cancel,1);assert_eq!(s.borrow().commit,0);assert_eq!(s.borrow().retire,1);let _=g.shutdown();assert_eq!(s.borrow().retire,1);}
#[test]fn abandoned_response_is_still_fully_decoded(){let(mut g,s)=group(1,4);let c=client(&mut g,false);let t=c.submit(&mut g,&Value(1)).unwrap();c.abandon(&mut g,t).unwrap();s.borrow_mut().queue.push_back((t.value(),3,vec![2]));assert_eq!(c.poll(&mut g).unwrap_err().code,22);assert_eq!(s.borrow().commit,0);assert_eq!(s.borrow().cancel,1);}
#[test]fn ambiguous_commit_is_not_replayed_or_cancelled(){let(mut g,s)=group(1,4);let c=client(&mut g,false);let t=c.submit(&mut g,&Value(1)).unwrap();s.borrow_mut().queue.push_back((t.value(),3,vec![1]));s.borrow_mut().commit_error=107;assert_eq!(c.poll(&mut g).unwrap_err().code,107);assert_eq!(c.poll(&mut g).unwrap_err().code,125);assert_eq!(s.borrow().commit,1);assert_eq!(s.borrow().cancel,0);assert_eq!(s.borrow().retire,1);}
#[test]fn cancel_retire_and_all_raw_close_failures_remain_separate(){let(mut g,s)=group(1,4);let c=client(&mut g,true);let t=c.submit(&mut g,&Value(1)).unwrap();{let mut m=s.borrow_mut();m.cancel_error=107;m.retire_error=5;m.close_error=17;m.queue.push_back((t.value(),3,vec![2]));}assert_eq!(c.poll(&mut g).unwrap_err().code,22);let d=g.diagnostics(0).unwrap();assert_eq!(d.primary,22);assert_eq!(d.cancel.code,107);assert_eq!(d.native_retire.code,5);assert!(d.raw_close.iter().all(|a|a.attempted&&a.code==17));assert_eq!(s.borrow().closes,vec![(1<<32)+3,(1<<32)+2,(1<<32)+1]);}
#[test]fn source_query_requires_third_owner_and_written_bool(){let(mut g,s)=group(1,4);let c=client(&mut g,false);assert_eq!(c.available(&mut g,0).unwrap_err().code,13);assert_eq!(s.borrow().queries,0);drop(g);let(mut g,s)=group(1,4);let c=client(&mut g,true);assert_eq!(c.available(&mut g,0).unwrap_err().code,22);assert_eq!(s.borrow().queries,1);}
#[test]fn genuine_false_and_error_never_become_false_success(){let(mut g,s)=group(1,4);let c=client(&mut g,true);s.borrow_mut().query_byte=Some(0);assert!(!c.available(&mut g,0).unwrap());s.borrow_mut().query_error=95;assert_eq!(c.available(&mut g,1).unwrap_err().code,95);assert_eq!(s.borrow().queries,2);}
#[test]fn inspect_mismatch_has_no_send(){let(mut g,s)=group(1,4);let c=client(&mut g,false);s.borrow_mut().mutate_inspect=true;assert_eq!(c.submit(&mut g,&Value(1)).unwrap_err().code,13);assert_eq!(s.borrow().submit,0);assert_eq!(s.borrow().retire,1);}
#[test]fn history_capacity_is_nonreused_before_effect(){let(mut g,s)=group(1,1);let c=client(&mut g,false);let _=c.submit(&mut g,&Value(1)).unwrap();assert_eq!(c.submit(&mut g,&Value(1)).unwrap_err().code,90);assert_eq!(s.borrow().submit,1);assert_eq!(g.diagnostics(0).unwrap().history_used,1);}
#[test]fn partial_factory_failure_closes_only_successful_open_once(){let(mut g,s)=group(1,4);s.borrow_mut().fail_open_at=2;assert_eq!(g.open(3,"call","observe",None).unwrap_err().code,13);assert_eq!(s.borrow().closes,vec![(1<<32)+1]);let _=g.shutdown();assert_eq!(s.borrow().closes.len(),1);assert_eq!(s.borrow().retire,0);}
#[test]fn original_server_request_then_one_response(){let(mut g,s)=group(1,4);let server=server(&mut g);s.borrow_mut().queue.push_back((0xf000000000000001,2,vec![1]));let ticket=match server.poll(&mut g).unwrap().unwrap(){Delivery::Value{ticket,value,..}=>{assert_eq!(value,Value(1));ticket},_=>panic!("request required")};server.respond(&mut g,ticket,&Value(0)).unwrap();assert_eq!(server.respond(&mut g,ticket,&Value(0)).unwrap_err().code,9);assert_eq!(s.borrow().respond,1);}
#[test]fn callback_error_never_sends_empty_success(){let(mut g,s)=group(1,4);let server=server(&mut g);s.borrow_mut().queue.push_back((0xf000000000000001,2,vec![1]));assert_eq!(server.serve_once(&mut g,|_,_|Err(err(5))).unwrap_err().code,5);assert_eq!(s.borrow().respond,0);assert_eq!(s.borrow().retire,1);}
#[test]fn per_request_error_does_not_complete_other_pending(){let(mut g,s)=group(1,4);let c=client(&mut g,false);let a=c.submit(&mut g,&Value(1)).unwrap();let b=c.submit(&mut g,&Value(1)).unwrap();s.borrow_mut().queue.push_back((a.value(),6,vec![]));assert!(matches!(c.poll(&mut g).unwrap(),Some(Delivery::Control(_))));assert_eq!(c.state(&g,a).unwrap(),RequestState::Completed{error:5});assert_eq!(c.state(&g,b).unwrap(),RequestState::Waiting);assert_eq!(s.borrow().retire,0);}
#[test]fn source_closed_wakes_local_status_and_defers_once_cleanup(){let(mut g,s)=group(1,4);let c=client(&mut g,false);let t=c.submit(&mut g,&Value(1)).unwrap();s.borrow_mut().queue.push_back((0,7,vec![]));assert!(matches!(c.poll(&mut g).unwrap(),Some(Delivery::Control(_))));assert_eq!(c.state(&g,t).unwrap_err().code,125);assert_eq!(s.borrow().commit,1);assert_eq!(s.borrow().retire,0);assert_eq!(s.borrow().closes.len(),0);c.close(&mut g).unwrap();assert_eq!(s.borrow().retire,1);assert_eq!(s.borrow().closes.len(),2);g.shutdown().unwrap();assert_eq!(s.borrow().retire,1);}
#[test]fn wire_reserved_width_and_unknown_control_are_rejected(){let r=original(3,(1<<32)+1);let receipt=wire::receipt(&r,1,3,(1<<32)+1).unwrap();let mut p=packet(3,receipt.binding.source,(1<<32)+2,9,3,&[1],1,1,353);let borrow=wire::envelope(&p,&receipt,(1<<32)+2,1).unwrap();p[160+121]=24;assert_eq!(wire::information(&p,&receipt,borrow).unwrap_err().code,95);p[160+121]=16;p[160+123]=1;assert_eq!(wire::information(&p,&receipt,borrow).unwrap_err().code,22);}
#[test]fn logical_zero_body_has_no_invented_byte(){let r=original(4,(1<<32)+1);let receipt=wire::receipt(&r,1,4,(1<<32)+1).unwrap();let p=packet(4,receipt.binding.source,(1<<32)+2,9,2,&[],1,1,352);let borrow=wire::envelope(&p,&receipt,(1<<32)+2,0).unwrap();assert_eq!(borrow.body_bytes,0);assert!(matches!(wire::information(&p,&receipt,borrow),Ok(wire::Information::Service(_))));}
#[test]fn callback_lease_rejects_all_group_mutation_before_effect(){let(mut g,s)=group(1,4);let c=client(&mut g,true);g.callback.set(true);assert_eq!(c.submit(&mut g,&Value(1)).unwrap_err().code,17);assert_eq!(c.poll(&mut g).unwrap_err().code,17);assert_eq!(c.available(&mut g,0).unwrap_err().code,17);assert_eq!(c.close(&mut g).unwrap_err().code,17);assert_eq!(g.shutdown().unwrap_err().code,17);assert_eq!(s.borrow().submit,0);assert_eq!(s.borrow().peek,0);assert_eq!(s.borrow().queries,0);assert_eq!(s.borrow().retire,0);assert_eq!(s.borrow().closes.len(),0);g.callback.set(false);g.shutdown().unwrap();}
#[test]fn genuine_wrong_pointer_width_refuses_before_imports(){if !cfg!(all(target_arch="wasm32",target_pointer_width="32")){assert!(matches!(Group::new(Config{endpoints:1,transactions_per_endpoint:1,maximum_request:0,maximum_response:0}),Err(e)if e.code==95));}}

#[test]fn valid_abandoned_late_response_is_discarded_without_false_completion(){let(mut g,s)=group(1,4);let c=client(&mut g,false);let t=c.submit(&mut g,&Value(1)).unwrap();c.abandon(&mut g,t).unwrap();s.borrow_mut().queue.push_back((t.value(),3,vec![1]));assert!(c.poll(&mut g).unwrap().is_none());assert_eq!(c.state(&g,t).unwrap(),RequestState::Abandoned);assert_eq!(s.borrow().commit,1);s.borrow_mut().queue.push_back((t.value(),3,vec![1]));assert_eq!(c.poll(&mut g).unwrap_err().code,13);assert_eq!(s.borrow().commit,1);}

// Real ACK success is followed only by local terminal bookkeeping. These two
// explicit LOCAL fixtures make cleanup fail if called; the genuine committed
// control must still be returned before any retire/close call is entered.
#[test]fn committed_client_retirement_defers_all_three_owner_cleanup_failures(){
 let(mut g,s)=group(1,4);let c=client(&mut g,true);let t=c.submit(&mut g,&Value(1)).unwrap();
 {let mut m=s.borrow_mut();m.retire_error=5;m.close_error=17;m.queue.push_back((0,7,vec![]));}
 let result=c.poll(&mut g).unwrap().unwrap();assert!(matches!(result,Delivery::Control(_)));
 assert_eq!(s.borrow().commit,1);assert_eq!(s.borrow().retire,0);assert!(s.borrow().closes.is_empty());
 let pending=g.ticket_index(c.endpoint.index,t).unwrap();assert_eq!(g.slots[0].history[pending].state,State::Abandoned);
 let d=g.diagnostics(0).unwrap();assert!(d.poisoned&&!d.closed&&!d.native_retire.attempted);assert_eq!(d.primary,125);
 let peeks=s.borrow().peek;assert_eq!(c.poll(&mut g).unwrap_err().code,125);assert_eq!(c.submit(&mut g,&Value(1)).unwrap_err().code,125);assert_eq!(c.available(&mut g,0).unwrap_err().code,125);assert_eq!(s.borrow().peek,peeks);
 let report=c.close(&mut g).unwrap();assert_eq!(report.primary,125);assert_eq!(report.native_retire,Attempt{attempted:true,code:5});assert!(report.raw_close.iter().all(|a|a.attempted&&a.code==17));
 assert_eq!(s.borrow().closes,vec![(1<<32)+3,(1<<32)+2,(1<<32)+1]);assert_eq!(s.borrow().retire,1);
 assert_eq!(g.shutdown().unwrap_err().code,5);assert_eq!(s.borrow().retire,1);assert_eq!(s.borrow().closes.len(),3);
}
#[test]fn committed_server_retirement_keeps_cleanup_separate_from_control(){
 let(mut g,s)=group(1,4);let server=server(&mut g);s.borrow_mut().queue.push_back((0xf000000000000001,2,vec![1]));
 let request=match server.poll(&mut g).unwrap().unwrap(){Delivery::Value{ticket,..}=>ticket,_=>panic!("request required")};
 s.borrow_mut().queue.push_back((0,7,vec![]));let result=server.poll(&mut g).unwrap().unwrap();assert!(matches!(result,Delivery::Control(_)));
 assert_eq!(s.borrow().commit,2);assert_eq!(s.borrow().retire,0);assert!(s.borrow().closes.is_empty());
 assert_eq!(server.respond(&mut g,request,&Value(0)).unwrap_err().code,125);assert_eq!(s.borrow().respond,0);
 let pending=g.ticket_index(server.endpoint.index,request).unwrap();assert_eq!(g.slots[0].history[pending].state,State::Abandoned);
 server.close(&mut g).unwrap();assert_eq!(s.borrow().retire,1);assert_eq!(s.borrow().closes.len(),2);g.shutdown().unwrap();assert_eq!(s.borrow().retire,1);
}
