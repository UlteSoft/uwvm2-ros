/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../../../../sdk/include/platform/trossen/as/ros1/trossen-ros1-main-complete/bounded_records";
import {abortGuest} from "../../../../../../../../../../sdk/include/platform/ros/as/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("41a82588c71e9838");const codec=new r.Ros1DurationCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-2010798015));
assert(value.nsec == <i32>(949493447));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("070000007633613163ceb2");const codec=new r.Ros1Interbotix_slate_msgsSetStringRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data == "v3a1cβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("01070000007639373336ceb2");const codec=new r.Ros1Interbotix_slate_msgsSetStringResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.success == true);
assert(value.message == "v9736β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("5fef3f4f200e3620c74aacfe");const codec=new r.Ros1Interbotix_xs_msgsArmJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ee_x_cmd == <i8>(95));
assert(value.ee_y_cmd == <i8>(-17));
assert(value.ee_z_cmd == <i8>(63));
assert(value.ee_roll_cmd == <i8>(79));
assert(value.ee_pitch_cmd == <i8>(32));
assert(value.waist_cmd == <i8>(14));
assert(value.gripper_cmd == <i8>(54));
assert(value.pose_cmd == <i8>(32));
assert(value.speed_cmd == <i8>(-57));
assert(value.speed_toggle_cmd == <i8>(74));
assert(value.gripper_pwm_cmd == <i8>(-84));
assert(value.torque_cmd == <i8>(-2));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("d464edf84898010dda8a07ceeaf8ac3fb9");const codec=new r.Ros1Interbotix_xs_msgsHexJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.world_x_cmd == <i8>(-44));
assert(value.world_y_cmd == <i8>(100));
assert(value.world_yaw_cmd == <i8>(-19));
assert(value.place_x_cmd == <i8>(-8));
assert(value.place_y_cmd == <i8>(72));
assert(value.place_z_cmd == <i8>(-104));
assert(value.place_roll_cmd == <i8>(1));
assert(value.place_pitch_cmd == <i8>(13));
assert(value.pose_cmd == <i8>(-38));
assert(value.move_type_cmd == <i8>(-118));
assert(value.gait_toggle_cmd == <i8>(7));
assert(value.leg_toggle_cmd == <i8>(-50));
assert(value.stance_cmd == <i8>(-22));
assert(value.reboot_cmd == <i8>(-8));
assert(value.set_home_pose_cmd == <i8>(-84));
assert(value.speed_cmd == <i8>(63));
assert(value.speed_toggle_cmd == <i8>(-71));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("070000007630383663ceb201000000008020c3");const codec=new r.Ros1Interbotix_xs_msgsJointGroupCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "v086cβ");
assert(value.cmd.length == 1);
assert(value.cmd[0] == <f32>(-160.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("070000007666663037ceb200402143");const codec=new r.Ros1Interbotix_xs_msgsJointSingleCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vff07β");
assert(value.cmd == <f32>(161.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("01000000070000007666323433ceb20100000062f87743");const codec=new r.Ros1Interbotix_xs_msgsJointTempsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.names.length == 1);
assert(value.names[0] == "vf243β");
assert(value.temps.length == 1);
assert(value.temps[0] == <i32>(1131935842));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("070000007662626536ceb2070000007635343830ceb20d0a486d49725f192f305232070000007639653762ceb201000000070000007632323461ceb201000000010000000000000000286d40010000000000000000000ec0010000000000000000ec61400100000000000000000049c006397c9f65f1c703");const codec=new r.Ros1Interbotix_xs_msgsJointTrajectoryCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "vbbe6β");
assert(value.name == "v5480β");
assert(value.traj.header.seq == <u32>(1833437709));
assert(value.traj.header.stamp.sec == <u32>(425685577));
assert(value.traj.header.stamp.nsec == <u32>(844247087));
assert(value.traj.header.frame_id == "v9e7bβ");
assert(value.traj.joint_names.length == 1);
assert(value.traj.joint_names[0] == "v224aβ");
assert(value.traj.points.length == 1);
assert(value.traj.points[0].positions.length == 1);
assert(value.traj.points[0].positions[0] == <f64>(233.25));
assert(value.traj.points[0].velocities.length == 1);
assert(value.traj.points[0].velocities[0] == <f64>(-3.75));
assert(value.traj.points[0].accelerations.length == 1);
assert(value.traj.points[0].accelerations[0] == <f64>(143.375));
assert(value.traj.points[0].effort.length == 1);
assert(value.traj.points[0].effort[0] == <f64>(-50.0));
assert(value.traj.points[0].time_from_start.sec == <i32>(-1619248890));
assert(value.traj.points[0].time_from_start.nsec == <i32>(63435109));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("0000000000385840000000000040574016198c4fff2f0cec677530ae626f");const codec=new r.Ros1Interbotix_xs_msgsLocobotJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.base_x_cmd == <f64>(96.875));
assert(value.base_theta_cmd == <f64>(93.0));
assert(value.base_reset_odom_cmd == <i8>(22));
assert(value.pan_cmd == <i8>(25));
assert(value.tilt_cmd == <i8>(-116));
assert(value.ee_x_cmd == <i8>(79));
assert(value.ee_y_cmd == <i8>(-1));
assert(value.ee_z_cmd == <i8>(47));
assert(value.ee_roll_cmd == <i8>(12));
assert(value.ee_pitch_cmd == <i8>(-20));
assert(value.waist_cmd == <i8>(103));
assert(value.gripper_cmd == <i8>(117));
assert(value.pose_cmd == <i8>(48));
assert(value.speed_cmd == <i8>(-82));
assert(value.speed_toggle_cmd == <i8>(98));
assert(value.gripper_pwm_cmd == <i8>(111));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("070000007663336535ceb2070000007638333865ceb2be42029a4db7f2f7fc73620f49b73c9bf3e6350273db68e9802e9884");const codec=new r.Ros1Interbotix_xs_msgsMotorGainsRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "vc3e5β");
assert(value.name == "v838eβ");
assert(value.kp_pos == <i32>(-1711127874));
assert(value.ki_pos == <i32>(-135088307));
assert(value.kd_pos == <i32>(258110460));
assert(value.k1 == <i32>(-1690519735));
assert(value.k2 == <i32>(37086963));
assert(value.kp_vel == <i32>(-379004045));
assert(value.ki_vel == <i32>(-2070401408));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("");const codec=new r.Ros1Interbotix_xs_msgsMotorGainsResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("070000007662393232ceb2070000007666326536ceb2070000007633623066ceb2070000007666626334ceb243dd31b6513b864e");const codec=new r.Ros1Interbotix_xs_msgsOperatingModesRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "vb922β");
assert(value.name == "vf2e6β");
assert(value.mode == "v3b0fβ");
assert(value.profile_type == "vfbc4β");
assert(value.profile_velocity == <i32>(-1238246077));
assert(value.profile_acceleration == <i32>(1317419857));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("");const codec=new r.Ros1Interbotix_xs_msgsOperatingModesResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("070000007638613737ceb2070000007632346531ceb20001");const codec=new r.Ros1Interbotix_xs_msgsRebootRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "v8a77β");
assert(value.name == "v24e1β");
assert(value.enable == false);
assert(value.smart_reboot == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("");const codec=new r.Ros1Interbotix_xs_msgsRebootResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("070000007633333063ceb2070000007635343239ceb2070000007636396638ceb27f0ea258");const codec=new r.Ros1Interbotix_xs_msgsRegisterValuesRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "v330cβ");
assert(value.name == "v5429β");
assert(value.reg == "v69f8β");
assert(value.value == <i32>(1487015551));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("010000000c086b41");const codec=new r.Ros1Interbotix_xs_msgsRegisterValuesResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.values.length == 1);
assert(value.values[0] == <i32>(1097533452));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("070000007639616439ceb2070000007662393966ceb2");const codec=new r.Ros1Interbotix_xs_msgsRobotInfoRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "v9ad9β");
assert(value.name == "vb99fβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("070000007634646535ceb2070000007639613966ceb201000000070000007666393963ceb201000000e0ed0100000000c0d0c20100000000a041c30100000000000d420100000000204b4301000000e58b7bdf01000000070000007632626137ceb2");const codec=new r.Ros1Interbotix_xs_msgsRobotInfoResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.mode == "v4de5β");
assert(value.profile_type == "v9a9fβ");
assert(value.joint_names.length == 1);
assert(value.joint_names[0] == "vf99cβ");
assert(value.joint_ids.length == 1);
assert(value.joint_ids[0] == <i16>(-4640));
assert(value.joint_lower_limits.length == 1);
assert(value.joint_lower_limits[0] == <f32>(-104.375));
assert(value.joint_upper_limits.length == 1);
assert(value.joint_upper_limits[0] == <f32>(-193.625));
assert(value.joint_velocity_limits.length == 1);
assert(value.joint_velocity_limits[0] == <f32>(35.25));
assert(value.joint_sleep_positions.length == 1);
assert(value.joint_sleep_positions[0] == <f32>(203.125));
assert(value.joint_state_indices.length == 1);
assert(value.joint_state_indices[0] == <i16>(-29723));
assert(value.num_joints == <i16>(-8325));
assert(value.name.length == 1);
assert(value.name[0] == "v2ba7β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("070000007666376231ceb2070000007665623164ceb200");const codec=new r.Ros1Interbotix_xs_msgsTorqueEnableRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "vf7b1β");
assert(value.name == "veb1dβ");
assert(value.enable == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("");const codec=new r.Ros1Interbotix_xs_msgsTorqueEnableResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("352a8eb4");const codec=new r.Ros1Interbotix_xs_msgsTurretJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pan_cmd == <i8>(53));
assert(value.tilt_cmd == <i8>(42));
assert(value.speed_cmd == <i8>(-114));
assert(value.speed_toggle_cmd == <i8>(-76));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("a949162e20488fdde929b81e070000007632663062ceb2");const codec=new r.Ros1Std_msgsHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.seq == <u32>(773212585));
assert(value.stamp.sec == <u32>(3717154848));
assert(value.stamp.nsec == <u32>(515385833));
assert(value.frame_id == "v2f0bβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("c8280b76c3d5bc08");const codec=new r.Ros1TimeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <u32>(1980442824));
assert(value.nsec == <u32>(146593219));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_25():void{const input=bytes("87c81e0ba0199d97984cdf00070000007665326666ceb201000000070000007637616235ceb201000000010000000000000000785ac0010000000000000000e034c0010000000000000000805240010000000000000000385bc0a2a8b9ee05f53c1f");const codec=new r.Ros1Trajectory_msgsJointTrajectoryCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(186566791));
assert(value.header.stamp.sec == <u32>(2543655328));
assert(value.header.stamp.nsec == <u32>(14634136));
assert(value.header.frame_id == "ve2ffβ");
assert(value.joint_names.length == 1);
assert(value.joint_names[0] == "v7ab5β");
assert(value.points.length == 1);
assert(value.points[0].positions.length == 1);
assert(value.points[0].positions[0] == <f64>(-105.875));
assert(value.points[0].velocities.length == 1);
assert(value.points[0].velocities[0] == <f64>(-20.875));
assert(value.points[0].accelerations.length == 1);
assert(value.points[0].accelerations[0] == <f64>(74.0));
assert(value.points[0].effort.length == 1);
assert(value.points[0].effort[0] == <f64>(-108.875));
assert(value.points[0].time_from_start.sec == <i32>(-289822558));
assert(value.points[0].time_from_start.nsec == <i32>(524088581));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_26():void{const input=bytes("010000000000000000c046c00100000000000000000008c00100000000000000004858400100000000000000008c64c02fd5b9dce5e29b05");const codec=new r.Ros1Trajectory_msgsJointTrajectoryPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.positions.length == 1);
assert(value.positions[0] == <f64>(-45.5));
assert(value.velocities.length == 1);
assert(value.velocities[0] == <f64>(-3.0));
assert(value.accelerations.length == 1);
assert(value.accelerations[0] == <f64>(97.125));
assert(value.effort.length == 1);
assert(value.effort[0] == <f64>(-164.375));
assert(value.time_from_start.sec == <i32>(-591801041));
assert(value.time_from_start.nsec == <i32>(94102245));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_27():void{const input=bytes("");const codec=new r.Ros1Xarm_msgsClearErrRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_28():void{const input=bytes("04c2070000007633393261ceb2");const codec=new r.Ros1Xarm_msgsClearErrResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(-15868));
assert(value.message == "v392aβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_29():void{const input=bytes("79c12017d60bdd87");const codec=new r.Ros1Xarm_msgsConfigToolModbusRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.baud_rate == <i32>(388022649));
assert(value.timeout_ms == <i32>(-2015556650));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_30():void{const input=bytes("070000007638616361ceb2adff");const codec=new r.Ros1Xarm_msgsConfigToolModbusResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.message == "v8acaβ");
assert(value.ret == <i16>(-83));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_31():void{const input=bytes("427a");const codec=new r.Ros1Xarm_msgsGetAnalogIORequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.port_num == <i16>(31298));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_32():void{const input=bytes("00808542f15e070000007665366165ceb2");const codec=new r.Ros1Xarm_msgsGetAnalogIOResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.analog_value == <f32>(66.75));
assert(value.ret == <i16>(24305));
assert(value.message == "ve6aeβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_33():void{const input=bytes("1edc");const codec=new r.Ros1Xarm_msgsGetControllerDigitalIORequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.io_num == <i16>(-9186));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_34():void{const input=bytes("f77f2f9f070000007631613862ceb2");const codec=new r.Ros1Xarm_msgsGetControllerDigitalIOResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(32759));
assert(value.value == <i16>(-24785));
assert(value.message == "v1a8bβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_35():void{const input=bytes("");const codec=new r.Ros1Xarm_msgsGetDigitalIORequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_36():void{const input=bytes("853edd303f6fd4a9a446070000007630346339ceb2");const codec=new r.Ros1Xarm_msgsGetDigitalIOResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.digital_1 == <i32>(819805829));
assert(value.digital_2 == <i32>(-1445695681));
assert(value.ret == <i16>(18084));
assert(value.message == "v04c9β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_37():void{const input=bytes("");const codec=new r.Ros1Xarm_msgsGetErrRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_38():void{const input=bytes("f045070000007636353464ceb2");const codec=new r.Ros1Xarm_msgsGetErrResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.err == <i16>(17904));
assert(value.message == "v654dβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_39():void{const input=bytes("00402543");const codec=new r.Ros1Xarm_msgsGripperConfigRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pulse_vel == <f32>(165.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_40():void{const input=bytes("7e09070000007636376264ceb2");const codec=new r.Ros1Xarm_msgsGripperConfigResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(2430));
assert(value.message == "v67bdβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_41():void{const input=bytes("00803ac2");const codec=new r.Ros1Xarm_msgsGripperMoveRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pulse_pos == <f32>(-46.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_42():void{const input=bytes("f1c3070000007632636534ceb2");const codec=new r.Ros1Xarm_msgsGripperMoveResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(-15375));
assert(value.message == "v2ce4β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_43():void{const input=bytes("");const codec=new r.Ros1Xarm_msgsGripperStateRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_44():void{const input=bytes("00802bc25fd4");const codec=new r.Ros1Xarm_msgsGripperStateResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.curr_pos == <f32>(-42.875));
assert(value.err_code == <i16>(-11169));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_45():void{const input=bytes("4e02582cf45351b500602ec300805f43");const codec=new r.Ros1Xarm_msgsIOStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.digital_1 == <i32>(743965262));
assert(value.digital_2 == <i32>(-1252961292));
assert(value.analog_1 == <f32>(-174.375));
assert(value.analog_2 == <f32>(223.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_46():void{const input=bytes("01000000000081c10000b6c10000a4c1000009420040c9c2");const codec=new r.Ros1Xarm_msgsMoveRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pose.length == 1);
assert(value.pose[0] == <f32>(-16.125));
assert(value.mvvelo == <f32>(-22.75));
assert(value.mvacc == <f32>(-20.5));
assert(value.mvtime == <f32>(34.25));
assert(value.mvradii == <f32>(-100.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_47():void{const input=bytes("55ee070000007635653732ceb2");const codec=new r.Ros1Xarm_msgsMoveResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(-4523));
assert(value.message == "v5e72β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_48():void{const input=bytes("2bf13c7074b05c65b401b8c39286010000000000c3c1008010c200004d420040e14200008b42000079c20000dcc000a023430000354200e01f4300a058c3008027430040a242");const codec=new r.Ros1Xarm_msgsRobotMsgCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.state == <i16>(-3797));
assert(value.mode == <i16>(28732));
assert(value.cmdnum == <i16>(-20364));
assert(value.mt_brake == <i16>(25948));
assert(value.mt_able == <i16>(436));
assert(value.err == <i16>(-15432));
assert(value.warn == <i16>(-31086));
assert(value.angle.length == 1);
assert(value.angle[0] == <f32>(-24.375));
assert(value.pose.length == 6);
assert(value.pose[0] == <f32>(-36.125));
assert(value.pose[1] == <f32>(51.25));
assert(value.pose[2] == <f32>(112.625));
assert(value.pose[3] == <f32>(69.5));
assert(value.pose[4] == <f32>(-62.25));
assert(value.pose[5] == <f32>(-6.875));
assert(value.offset.length == 6);
assert(value.offset[0] == <f32>(163.625));
assert(value.offset[1] == <f32>(45.25));
assert(value.offset[2] == <f32>(159.875));
assert(value.offset[3] == <f32>(-216.625));
assert(value.offset[4] == <f32>(167.5));
assert(value.offset[5] == <f32>(81.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_49():void{const input=bytes("259884ae");const codec=new r.Ros1Xarm_msgsSetAxisRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.id == <i16>(-26587));
assert(value.data == <i16>(-20860));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_50():void{const input=bytes("7ad9070000007666356461ceb2");const codec=new r.Ros1Xarm_msgsSetAxisResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(-9862));
assert(value.message == "vf5daβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_51():void{const input=bytes("9f3385f8");const codec=new r.Ros1Xarm_msgsSetDigitalIORequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.io_num == <i16>(13215));
assert(value.value == <i16>(-1915));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_52():void{const input=bytes("da06070000007630386136ceb2");const codec=new r.Ros1Xarm_msgsSetDigitalIOResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(1754));
assert(value.message == "v08a6β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_53():void{const input=bytes("9b8a");const codec=new r.Ros1Xarm_msgsSetInt16RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data == <i16>(-30053));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_54():void{const input=bytes("65fd070000007636373465ceb2");const codec=new r.Ros1Xarm_msgsSetInt16ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(-667));
assert(value.message == "v674eβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_55():void{const input=bytes("00c008c30040e5c200000ec300e007c3");const codec=new r.Ros1Xarm_msgsSetLoadRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.mass == <f32>(-136.75));
assert(value.xc == <f32>(-114.625));
assert(value.yc == <f32>(-142.0));
assert(value.zc == <f32>(-135.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_56():void{const input=bytes("55ef070000007663303261ceb2");const codec=new r.Ros1Xarm_msgsSetLoadResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(-4267));
assert(value.message == "vc02aβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_57():void{const input=bytes("01000000f9a734");const codec=new r.Ros1Xarm_msgsSetToolModbusRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.send_data.length == 1);
assert(value.send_data[0] == <u8>(249));
assert(value.respond_len == <i16>(13479));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_58():void{const input=bytes("91890100000067");const codec=new r.Ros1Xarm_msgsSetToolModbusResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(-30319));
assert(value.respond_data.length == 1);
assert(value.respond_data[0] == <u8>(103));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_59():void{const input=bytes("0000dac2004064c300801ec30000d44200c02fc30000f541");const codec=new r.Ros1Xarm_msgsTCPOffsetRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f32>(-109.0));
assert(value.y == <f32>(-228.25));
assert(value.z == <f32>(-158.5));
assert(value.roll == <f32>(106.0));
assert(value.pitch == <f32>(-175.75));
assert(value.yaw == <f32>(30.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_60():void{const input=bytes("283d070000007662626330ceb2");const codec=new r.Ros1Xarm_msgsTCPOffsetResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ret == <i16>(15656));
assert(value.message == "vbbc0β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

export function _start():void{
test_0();
test_1();
test_2();
test_3();
test_4();
test_5();
test_6();
test_7();
test_8();
test_9();
test_10();
test_11();
test_12();
test_13();
test_14();
test_15();
test_16();
test_17();
test_18();
test_19();
test_20();
test_21();
test_22();
test_23();
test_24();
test_25();
test_26();
test_27();
test_28();
test_29();
test_30();
test_31();
test_32();
test_33();
test_34();
test_35();
test_36();
test_37();
test_38();
test_39();
test_40();
test_41();
test_42();
test_43();
test_44();
test_45();
test_46();
test_47();
test_48();
test_49();
test_50();
test_51();
test_52();
test_53();
test_54();
test_55();
test_56();
test_57();
test_58();
test_59();
test_60();
}
