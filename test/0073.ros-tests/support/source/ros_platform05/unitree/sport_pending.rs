//! Actual-Wasm local correlation contract, separate from native transport.
//! Stage as main.rs beside complete bounded_records.rs and current sport.rs.
extern crate roswasm;
mod bounded_records;
mod sport;
use bounded_records::{Ros2Unitree_apiMsgRequest as Request, Ros2Unitree_apiMsgResponse as Response};
use roswasm::typed::Canonical;
use sport::SportPending;
const APIS:[i64;39]=[1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1015,1016,1017,1020,1022,1023,1027,1028,1029,1030,1031,1032,1036,1061,1062,1063,2041,2043,2044,2045,2046,2047,2048,2049,2050,2051,2054,2055,2058];
fn request(id:i64,api:i64)->Request {
    let mut value=Request::default();value.header.identity.id=id;value.header.identity.api_id=api;
    value.header.lease.id=-9007199254740993;value.header.policy.priority=-101;
    value.parameter="{\"complete\":\"λ\"}".into();value.binary=vec![0,255,1,128];value
}
fn response(id:i64,api:i64)->Response {
    let mut value=Response::default();value.header.identity.id=id;value.header.identity.api_id=api;
    value.header.status.code=-2147483647;value.data="{\"data\":false,\"full\":\"λ\"}".into();
    value.binary=vec![0,-1,1,-128];value
}
fn main(){
    let mut pending=SportPending::new();
    for (offset,api) in APIS.iter().enumerate(){
        let id=9007199254740993+offset as i64;
        let req=request(id,*api);let before=req.encode().unwrap();
        pending.register(&req,7,11).unwrap();assert_eq!(req.encode().unwrap(),before);
        assert_eq!(pending.pending_count(),1);
        assert_eq!(pending.register(&req,7,11).unwrap_err().code,17);
        let mut reply=response(id,*api);let before=reply.encode().unwrap();
        let wrong=response(id,*api+90000);assert_eq!(pending.check(&wrong,8).unwrap_err().code,9);
        assert_eq!(pending.pending_count(),1);pending.check(&reply,10).unwrap();
        assert_eq!(pending.accept(&reply,11).unwrap_err().code,110);assert_eq!(pending.pending_count(),1);
        let mut invalid=reply.clone();invalid.data="x".repeat(4097);
        assert_eq!(pending.accept(&invalid,10).unwrap_err().code,22);assert_eq!(pending.pending_count(),1);
        let accepted=pending.accept(&reply,10).unwrap();assert_eq!(accepted.encode().unwrap(),before);
        reply.binary[0]=7;assert_eq!(accepted.binary[0],0);assert_eq!(accepted.header.status.code,-2147483647);
        assert_eq!(pending.accept(&accepted,10).unwrap_err().code,9);
        assert_eq!(pending.register(&req,7,11).unwrap_err().code,17);
    }
    for id in 9007199254741032..9007199254741048 { pending.register(&request(id,1003),0,u64::MAX).unwrap(); }
    let next=request(9007199254741048,1003);
    assert_eq!(pending.register(&next,0,u64::MAX).unwrap_err().code,11);
    assert_eq!(pending.pending_count(),16);
    assert_eq!(pending.expire(9007199254741032,1003,u64::MAX-1),11);
    assert_eq!(pending.expire(9007199254741032,1003,u64::MAX),110);
    pending.register(&next,0,u64::MAX).unwrap(); // full refusal did not burn ID.
    pending.forget(next.header.identity.id,1003).unwrap();
    let mut invalid=request(i64::MAX,1003);invalid.header.policy.noreply=true;
    assert_eq!(pending.register(&invalid,0,1).unwrap_err().code,22);
    invalid.header.policy.noreply=false;invalid.header.identity.api_id=65536+1003;
    assert_eq!(pending.register(&invalid,0,1).unwrap_err().code,22);
    invalid.header.identity.api_id=1003;
    assert_eq!(pending.register(&invalid,1,1).unwrap_err().code,22);
    pending.register(&invalid,u64::MAX-1,u64::MAX).unwrap();
    pending.forget(i64::MAX,1003).unwrap();
    assert_eq!(pending.register(&invalid,0,1).unwrap_err().code,17);
    pending.close();assert_eq!(pending.pending_count(),0);
    assert_eq!(pending.register(&invalid,0,1).unwrap_err().code,9);
    assert_eq!(pending.expire(9007199254741033,1003,u64::MAX),9);
    println!("SPORT_PENDING_OK operations=39 capacity=16 identity_bits=64");
}
