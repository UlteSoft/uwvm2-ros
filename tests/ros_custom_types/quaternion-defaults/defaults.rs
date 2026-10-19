//! Place beside real freshly generated bounded_records.rs; use actual SDK crate.
extern crate alloc;
mod bounded_records;
use bounded_records::{Ros2Geometry_msgsMsgQuaternion as Q,Ros2Default_fixtureMsgBundle as B};
use roswasm::typed::Canonical;
fn main(){
    let q=Q::default();assert_eq!((q.x,q.y,q.z,q.w),(0.,0.,0.,1.));
    let mut b=B::default();assert_eq!(b.fixed[0].w,1.);assert_eq!(b.fixed[1].w,1.);assert_eq!(b.pose.orientation.w,1.);assert!(b.dynamic.is_empty());
    b.fixed[0].w=0.25;assert_eq!(b.fixed[1].w,1.);b.dynamic.push(Q::default());
    let bytes=b.encode().unwrap();let out=B::decode(&bytes).unwrap();assert_eq!(out.fixed[0].w,0.25);assert_eq!(out.fixed[1].w,1.);assert_eq!(out.dynamic[0].w,1.);
    assert!(Q::decode(&[0u8;31]).is_err());assert_eq!(Q::decode(&[0u8;32]).unwrap().w,0.);
}
