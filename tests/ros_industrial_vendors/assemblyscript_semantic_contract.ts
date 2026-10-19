/** Runtime contracts for the actual complete KUKA AssemblyScript codec.
 * Compile only after regenerating the official KUKA catalog with the reviewed
 * producer. This module imports the generated classes directly and exports one
 * finite, allocation-bounded entry. Zero means every assertion below held.
 * Nonzero values identify a specific failed protocol property. No ROS import,
 * Host mock, Native registration, socket, robot or permission grant is needed.
 * These are authored contracts, not an execution receipt.
 */
import {Ros2Builtin_interfacesMsgTime, Ros2Builtin_interfacesMsgTimeCodec,
  Ros2Builtin_interfacesMsgDuration, Ros2Builtin_interfacesMsgDurationCodec,
  Ros2Sensor_msgsMsgJointState, Ros2Sensor_msgsMsgJointStateCodec}
  from "../../sdk/custom-types/kuka-ros2-complete/bounded_records";

/** Exercise the real encoder and decoder, including malformed complete wires. */
export function run_contract():i32 {
  const timeCodec=new Ros2Builtin_interfacesMsgTimeCodec();
  const time=new Ros2Builtin_interfacesMsgTime();
  time.sec=-1;time.nanosec=999999999;
  const accepted=timeCodec.encodeChecked(time);
  if(accepted.status!=0||accepted.bytes.length!=8)return 1;
  const roundtrip=timeCodec.decodeChecked(accepted.bytes);
  if(roundtrip.status!=0||roundtrip.value===null)return 2;
  time.nanosec=1000000000;
  if(timeCodec.encodeChecked(time).status!=22)return 3;
  // The two little-endian canonical u32 fields are sec=0 and nanosec=1e9.
  const malformedTime=new Uint8Array(8);
  malformedTime[4]=0;malformedTime[5]=202;malformedTime[6]=154;malformedTime[7]=59;
  const badTime=timeCodec.decodeChecked(malformedTime);
  if(badTime.status!=22||badTime.value!==null||timeCodec.valid(malformedTime))return 4;
  const durationCodec=new Ros2Builtin_interfacesMsgDurationCodec();
  const duration=new Ros2Builtin_interfacesMsgDuration();duration.sec=-2;duration.nanosec=999999999;
  if(durationCodec.encodeChecked(duration).status!=0)return 5;
  duration.nanosec=1000000000;
  if(durationCodec.encodeChecked(duration).status!=22)return 6;
  const badDuration=durationCodec.decodeChecked(malformedTime);
  if(badDuration.status!=22||badDuration.value!==null)return 7;
  const jointCodec=new Ros2Sensor_msgsMsgJointStateCodec();
  const joint=new Ros2Sensor_msgsMsgJointState();
  joint.name.push("a");joint.position.push(0.0);
  const validJoint=jointCodec.encodeChecked(joint);
  if(validJoint.status!=0||jointCodec.decodeChecked(validJoint.bytes).status!=0)return 8;
  // Optional arrays can be empty; populated arrays must agree with each other.
  joint.velocity.push(0.0);
  if(jointCodec.encodeChecked(joint).status!=0)return 9;
  joint.position.push(1.0);
  if(jointCodec.encodeChecked(joint).status!=22)return 10;
  // Complete independent wire: empty Header, one name "a", two f64 positions,
  // and empty velocity/effort. Every scalar is valid; only the lengths differ.
  const malformedJoint=new Uint8Array(49);
  malformedJoint[12]=1;malformedJoint[16]=1;malformedJoint[20]=97;malformedJoint[21]=2;
  const badJoint=jointCodec.decodeChecked(malformedJoint);
  if(badJoint.status!=22||badJoint.value!==null||jointCodec.valid(malformedJoint))return 11;
  return 0;
}
