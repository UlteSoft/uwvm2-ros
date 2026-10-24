/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../../../../sdk/include/platform/trossen/as/ros2/trossen-ros2-galactic-complete/bounded_records";
import {abortGuest} from "../../../../../../../../../../sdk/include/platform/ros/as/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("8b8195af42ba7f2d");const codec=new r.Ros2Builtin_interfacesMsgDurationCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-1349156469));
assert(value.nanosec == <u32>(763345474));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("072b89b550057009");const codec=new r.Ros2Builtin_interfacesMsgTimeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-1249301753));
assert(value.nanosec == <u32>(158336336));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("b101d13db0e844ce211b6970");const codec=new r.Ros2Interbotix_xs_msgsMsgArmJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ee_x_cmd == <i8>(-79));
assert(value.ee_y_cmd == <i8>(1));
assert(value.ee_z_cmd == <i8>(-47));
assert(value.ee_roll_cmd == <i8>(61));
assert(value.ee_pitch_cmd == <i8>(-80));
assert(value.waist_cmd == <i8>(-24));
assert(value.gripper_cmd == <i8>(68));
assert(value.pose_cmd == <i8>(-50));
assert(value.speed_cmd == <i8>(33));
assert(value.speed_toggle_cmd == <i8>(27));
assert(value.gripper_pwm_cmd == <i8>(105));
assert(value.torque_cmd == <i8>(112));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("a616f08a3aea4ac83497c285647695d9e8");const codec=new r.Ros2Interbotix_xs_msgsMsgHexJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.world_x_cmd == <i8>(-90));
assert(value.world_y_cmd == <i8>(22));
assert(value.world_yaw_cmd == <i8>(-16));
assert(value.place_x_cmd == <i8>(-118));
assert(value.place_y_cmd == <i8>(58));
assert(value.place_z_cmd == <i8>(-22));
assert(value.place_roll_cmd == <i8>(74));
assert(value.place_pitch_cmd == <i8>(-56));
assert(value.pose_cmd == <i8>(52));
assert(value.move_type_cmd == <i8>(-105));
assert(value.gait_toggle_cmd == <i8>(-62));
assert(value.leg_toggle_cmd == <i8>(-123));
assert(value.stance_cmd == <i8>(100));
assert(value.reboot_cmd == <i8>(118));
assert(value.set_home_pose_cmd == <i8>(-107));
assert(value.speed_cmd == <i8>(-39));
assert(value.speed_toggle_cmd == <i8>(-24));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("070000007665393031ceb20100000000e01443");const codec=new r.Ros2Interbotix_xs_msgsMsgJointGroupCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "ve901β");
assert(value.cmd.length == 1);
assert(value.cmd[0] == <f32>(148.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("070000007631346538ceb2006058c3");const codec=new r.Ros2Interbotix_xs_msgsMsgJointSingleCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "v14e8β");
assert(value.cmd == <f32>(-216.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("01000000070000007634633432ceb201000000f2a176fd");const codec=new r.Ros2Interbotix_xs_msgsMsgJointTempsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.names.length == 1);
assert(value.names[0] == "v4c42β");
assert(value.temps.length == 1);
assert(value.temps[0] == <i32>(-42556942));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("070000007662626165ceb2070000007661396536ceb29cb7b9d877c30737070000007631396133ceb201000000070000007666323937ceb2010000000100000000000000002852c0010000000000000000105fc0010000000000000000106bc00100000000000000005462c0083b49bb3f860d2d");const codec=new r.Ros2Interbotix_xs_msgsMsgJointTrajectoryCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "vbbaeβ");
assert(value.name == "va9e6β");
assert(value.traj.header.stamp.sec == <i32>(-658917476));
assert(value.traj.header.stamp.nanosec == <u32>(923255671));
assert(value.traj.header.frame_id == "v19a3β");
assert(value.traj.joint_names.length == 1);
assert(value.traj.joint_names[0] == "vf297β");
assert(value.traj.points.length == 1);
assert(value.traj.points[0].positions.length == 1);
assert(value.traj.points[0].positions[0] == <f64>(-72.625));
assert(value.traj.points[0].velocities.length == 1);
assert(value.traj.points[0].velocities[0] == <f64>(-124.25));
assert(value.traj.points[0].accelerations.length == 1);
assert(value.traj.points[0].accelerations[0] == <f64>(-216.5));
assert(value.traj.points[0].effort.length == 1);
assert(value.traj.points[0].effort[0] == <f64>(-146.625));
assert(value.traj.points[0].time_from_start.sec == <i32>(-1152828664));
assert(value.traj.points[0].time_from_start.nanosec == <u32>(755861055));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("00000000009853400000000000e852c0166b1cdf6fbfc9bd7ab0a0b32775");const codec=new r.Ros2Interbotix_xs_msgsMsgLocobotJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.base_x_cmd == <f64>(78.375));
assert(value.base_theta_cmd == <f64>(-75.625));
assert(value.base_reset_odom_cmd == <i8>(22));
assert(value.pan_cmd == <i8>(107));
assert(value.tilt_cmd == <i8>(28));
assert(value.ee_x_cmd == <i8>(-33));
assert(value.ee_y_cmd == <i8>(111));
assert(value.ee_z_cmd == <i8>(-65));
assert(value.ee_roll_cmd == <i8>(-55));
assert(value.ee_pitch_cmd == <i8>(-67));
assert(value.waist_cmd == <i8>(122));
assert(value.gripper_cmd == <i8>(-80));
assert(value.pose_cmd == <i8>(-96));
assert(value.speed_cmd == <i8>(-77));
assert(value.speed_toggle_cmd == <i8>(39));
assert(value.gripper_pwm_cmd == <i8>(117));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("bb581eae");const codec=new r.Ros2Interbotix_xs_msgsMsgTurretJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pan_cmd == <i8>(-69));
assert(value.tilt_cmd == <i8>(88));
assert(value.speed_cmd == <i8>(30));
assert(value.speed_toggle_cmd == <i8>(-82));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("070000007638386462ceb2070000007637653063ceb2dfa9413e2c5cb1539d9821ab43b3634af9e26ad312302b4de1c5db20");const codec=new r.Ros2Interbotix_xs_msgsSrvMotorGains_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "v88dbβ");
assert(value.name == "v7e0cβ");
assert(value.kp_pos == <i32>(1044490719));
assert(value.ki_pos == <i32>(1404132396));
assert(value.kd_pos == <i32>(-1423861603));
assert(value.k1 == <i32>(1248047939));
assert(value.k2 == <i32>(-747969799));
assert(value.kp_vel == <i32>(1294675986));
assert(value.ki_vel == <i32>(551273953));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("");const codec=new r.Ros2Interbotix_xs_msgsSrvMotorGains_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("070000007631386365ceb2070000007634653936ceb2070000007638373766ceb2070000007633373861ceb24fb5ebc2c1862b6e");const codec=new r.Ros2Interbotix_xs_msgsSrvOperatingModes_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "v18ceβ");
assert(value.name == "v4e96β");
assert(value.mode == "v877fβ");
assert(value.profile_type == "v378aβ");
assert(value.profile_velocity == <i32>(-1024740017));
assert(value.profile_acceleration == <i32>(1848346305));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("");const codec=new r.Ros2Interbotix_xs_msgsSrvOperatingModes_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("070000007665353131ceb2070000007631356366ceb20000");const codec=new r.Ros2Interbotix_xs_msgsSrvReboot_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "ve511β");
assert(value.name == "v15cfβ");
assert(value.enable == false);
assert(value.smart_reboot == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("");const codec=new r.Ros2Interbotix_xs_msgsSrvReboot_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("070000007637653432ceb2070000007637323333ceb2070000007661613938ceb2016ee2eb");const codec=new r.Ros2Interbotix_xs_msgsSrvRegisterValues_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "v7e42β");
assert(value.name == "v7233β");
assert(value.reg == "vaa98β");
assert(value.value == <i32>(-337482239));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("010000001ad1dd83");const codec=new r.Ros2Interbotix_xs_msgsSrvRegisterValues_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.values.length == 1);
assert(value.values[0] == <i32>(-2082615014));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("070000007634623831ceb2070000007636373831ceb2");const codec=new r.Ros2Interbotix_xs_msgsSrvRobotInfo_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "v4b81β");
assert(value.name == "v6781β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("070000007631663533ceb2070000007632356366ceb201000000070000007666383966ceb20100000048760100000000c043430100000000c0de4201000000000032c10100000000c0594301000000acd1ee2701000000070000007666616666ceb2");const codec=new r.Ros2Interbotix_xs_msgsSrvRobotInfo_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.mode == "v1f53β");
assert(value.profile_type == "v25cfβ");
assert(value.joint_names.length == 1);
assert(value.joint_names[0] == "vf89fβ");
assert(value.joint_ids.length == 1);
assert(value.joint_ids[0] == <i16>(30280));
assert(value.joint_lower_limits.length == 1);
assert(value.joint_lower_limits[0] == <f32>(195.75));
assert(value.joint_upper_limits.length == 1);
assert(value.joint_upper_limits[0] == <f32>(111.375));
assert(value.joint_velocity_limits.length == 1);
assert(value.joint_velocity_limits[0] == <f32>(-11.125));
assert(value.joint_sleep_positions.length == 1);
assert(value.joint_sleep_positions[0] == <f32>(217.75));
assert(value.joint_state_indices.length == 1);
assert(value.joint_state_indices[0] == <i16>(-11860));
assert(value.num_joints == <i16>(10222));
assert(value.name.length == 1);
assert(value.name[0] == "vfaffβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("070000007637643633ceb2070000007633656462ceb200");const codec=new r.Ros2Interbotix_xs_msgsSrvTorqueEnable_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.cmd_type == "v7d63β");
assert(value.name == "v3edbβ");
assert(value.enable == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("");const codec=new r.Ros2Interbotix_xs_msgsSrvTorqueEnable_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("681252e4acc1ba37070000007666316532ceb2");const codec=new r.Ros2Std_msgsMsgHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(-464383384));
assert(value.stamp.nanosec == <u32>(934986156));
assert(value.frame_id == "vf1e2β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("9e1673e752d0f60f070000007636363034ceb201000000070000007633663435ceb201000000010000000000000000d850400100000000000000000863c0010000000000000000b857c0010000000000000000d8514075dbb8e27217aa02");const codec=new r.Ros2Trajectory_msgsMsgJointTrajectoryCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-411887970));
assert(value.header.stamp.nanosec == <u32>(267833426));
assert(value.header.frame_id == "v6604β");
assert(value.joint_names.length == 1);
assert(value.joint_names[0] == "v3f45β");
assert(value.points.length == 1);
assert(value.points[0].positions.length == 1);
assert(value.points[0].positions[0] == <f64>(67.375));
assert(value.points[0].velocities.length == 1);
assert(value.points[0].velocities[0] == <f64>(-152.25));
assert(value.points[0].accelerations.length == 1);
assert(value.points[0].accelerations[0] == <f64>(-94.875));
assert(value.points[0].effort.length == 1);
assert(value.points[0].effort[0] == <f64>(71.375));
assert(value.points[0].time_from_start.sec == <i32>(-491201675));
assert(value.points[0].time_from_start.nanosec == <u32>(44701554));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("010000000000000000a042c00100000000000000007c6e400100000000000000002034c0010000000000000000fc6840b47e2a6f9fb8c510");const codec=new r.Ros2Trajectory_msgsMsgJointTrajectoryPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.positions.length == 1);
assert(value.positions[0] == <f64>(-37.25));
assert(value.velocities.length == 1);
assert(value.velocities[0] == <f64>(243.875));
assert(value.accelerations.length == 1);
assert(value.accelerations[0] == <f64>(-20.125));
assert(value.effort.length == 1);
assert(value.effort[0] == <f64>(199.875));
assert(value.time_from_start.sec == <i32>(1865055924));
assert(value.time_from_start.nanosec == <u32>(281393311));
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
}
