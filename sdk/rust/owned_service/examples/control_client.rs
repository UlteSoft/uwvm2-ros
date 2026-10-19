//! Actual retained-client GetTypeDescription request. Explicit argv aliases;
//! complete include_type_sources=true response decode; no legacy RPC or graph.
//! A finite local step limit is WORK, never advertised as physical nanoseconds.
mod control_bindings;
use control_bindings::records;
use roswasm::{Error,codes};
use roswasm_owned_service::{Config,Delivery,Group};
fn invalid()->Error{Error::new(codes::INVALID)}
fn run()->Result<(),Error>{
 let args:Vec<String>=std::env::args().collect();
 // call observer availability-or-dash type_name actual_type_hash steps
 if args.len()!=7{return Err(invalid());}
 let steps:usize=args[6].parse().map_err(|_|invalid())?;if steps==0||steps>65536{return Err(invalid());}
 let mut group=Group::new(Config{endpoints:1,transactions_per_endpoint:1,maximum_request:records::Ros2Type_description_interfacesSrvGetTypeDescription_Request::MAX_WIRE_BYTES,maximum_response:records::Ros2Type_description_interfacesSrvGetTypeDescription_Response::MAX_WIRE_BYTES})?;
 let primary=(||{
  let client=group.client(control_bindings::get_type_description(),&args[1],&args[2],if args[3]=="-"{None}else{Some(args[3].as_str())})?;
  if args[3]!="-"&&!client.available(&mut group,0)?{return Err(Error::new(codes::AGAIN));}
  let mut request=records::Ros2Type_description_interfacesSrvGetTypeDescription_Request::default();request.type_name=args[4].clone();request.type_hash=args[5].clone();request.include_type_sources=true;
  let ticket=client.submit(&mut group,&request)?;
  for _ in 0..steps{match client.poll(&mut group)?{
   None=>{},Some(Delivery::Control(c))=>return Err(Error::new(if c.error!=0{c.error}else{125})),
   Some(Delivery::Value{ticket:actual,value,info})=>{
    if actual!=ticket||!value.successful||value.type_sources.is_empty()||value.type_sources.len()>12{return Err(invalid());}
    // Every field/raw byte was already fully typed-decoded before real ACK.
    // A marker/value count alone cannot prove SDK identity or DDS delivery.
    println!("owned GTD response sources={} references={} native_sequence={} source_timestamp={} received_timestamp={}",value.type_sources.len(),value.type_description.referenced_type_descriptions.len(),info.native_sequence,info.source_timestamp,info.received_timestamp);
    return Ok(());
   }
  }}
  // Explicit local abandonment, not native DDS cancellation or request replay.
  client.abandon(&mut group,ticket)?;Err(Error::new(codes::TOO_LARGE))
 })();
 let cleanup=group.shutdown();for i in 0..group.attempted_endpoints(){if let Some(d)=group.diagnostics(i){eprintln!("owned cleanup primary={} retire={:?} raw={:?}",d.primary,d.native_retire,d.raw_close);}}
 primary.and(cleanup)
}
fn main(){if let Err(e)=run(){eprintln!("owned client status {}",e.code);std::process::exit(1);}}
