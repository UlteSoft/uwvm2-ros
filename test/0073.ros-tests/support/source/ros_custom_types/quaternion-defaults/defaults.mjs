/** Actual generated codec objects; no NativeApi, fake hash or ROS entity. */
import {Ros2Geometry_msgsMsgQuaternionCodec as Q,Ros2Default_fixtureMsgBundleCodec as B} from './bounded_records.mjs';
function check(ok){if(!ok)throw new Error('Quaternion declared-default contract');}
export function runDefaults(){
  const q=Q.defaultValue();check(q.x===0&&q.y===0&&q.z===0&&q.w===1);
  const b=B.defaultValue();check(b.fixed.length===2&&b.fixed[0].w===1&&b.fixed[1].w===1&&b.pose.orientation.w===1&&b.dynamic.length===0);
  b.fixed[0].w=.25;check(b.fixed[1].w===1);b.dynamic.push(Q.defaultValue());
  const out=B.decode(B.encode(b));check(out.fixed[0].w===.25&&out.fixed[1].w===1&&out.dynamic[0].w===1);
  check(!Q.valid(new Uint8Array(31)));check(Q.decode(new Uint8Array(32)).w===0);
  return 0;
}
