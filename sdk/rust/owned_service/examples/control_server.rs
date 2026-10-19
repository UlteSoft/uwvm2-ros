//! One real retained original SetParametersAtomically server request/response.
//! This is an application handler, not a fake native parameter store/default Node.
mod control_bindings;
use control_bindings::records;
use roswasm::{Error,codes};
use roswasm_owned_service::{Config,Group};
fn invalid()->Error{Error::new(codes::INVALID)}
fn run()->Result<(),Error>{
 let args:Vec<String>=std::env::args().collect();if args.len()!=4{return Err(invalid());}
 let steps:usize=args[3].parse().map_err(|_|invalid())?;if steps==0||steps>65536{return Err(invalid());}
 let mut group=Group::new(Config{endpoints:1,transactions_per_endpoint:1,maximum_request:records::Ros2Rcl_interfacesSrvSetParametersAtomically_Request::MAX_WIRE_BYTES,maximum_response:records::Ros2Rcl_interfacesSrvSetParametersAtomically_Response::MAX_WIRE_BYTES})?;
 let primary=(||{let server=group.server(control_bindings::set_parameters_atomically(),&args[1],&args[2])?;for _ in 0..steps{
  if server.serve_once(&mut group,|request,_same_take|{
   // Application explicitly refuses updates; every actual ParameterValue field
   // has been delivered/decoded. A denial is a genuine nonempty typed result.
   let mut response=records::Ros2Rcl_interfacesSrvSetParametersAtomically_Response::default();response.result.successful=false;response.result.reason=if request.parameters.is_empty(){"no parameters".into()}else{"application does not authorize mutation".into()};Ok(response)
  })?{return Ok(());}
 }Err(Error::new(codes::TOO_LARGE))})();
 let cleanup=group.shutdown();for i in 0..group.attempted_endpoints(){if let Some(d)=group.diagnostics(i){eprintln!("owned cleanup primary={} retire={:?} raw={:?}",d.primary,d.native_retire,d.raw_close);}}
 primary.and(cleanup)
}
fn main(){if let Err(e)=run(){eprintln!("owned server status {}",e.code);std::process::exit(1);}}
