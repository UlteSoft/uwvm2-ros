/** Compile with actual AS SDK codecs and freshly generated bounded_records.ts. */
import {Ros2Geometry_msgsMsgQuaternion as Q,Ros2Geometry_msgsMsgQuaternionCodec as QC,Ros2Default_fixtureMsgBundle as B,Ros2Default_fixtureMsgBundleCodec as BC} from './bounded_records';
let checks:u32=0;
function check(ok:bool):void{checks++;if(!ok)unreachable();}
export function _start():void{
  checks=0;const q=new Q(),b=new B(),qc=new QC(),bc=new BC();
  check(q.x==0&&q.y==0&&q.z==0&&q.w==1);
  check(b.fixed.length==2&&b.fixed[0].w==1&&b.fixed[1].w==1&&b.pose.orientation.w==1&&b.dynamic.length==0);
  b.fixed[0].w=.25;check(b.fixed[1].w==1);b.dynamic.push(new Q());
  const bytes=bc.encode(b);check(bc.valid(bytes));const out=bc.decode(bytes);
  check(out.fixed[0].w==.25&&out.fixed[1].w==1&&out.dynamic.length==1&&out.dynamic[0].w==1);
  check(!qc.valid(new Uint8Array(31)));const zero=new Uint8Array(32);check(qc.valid(zero));check(qc.decode(zero).w==0);
}
export function completedChecks():u32{return checks;}
