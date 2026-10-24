/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/trossen-ros2-humble-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("8b8195af42ba7f2d"),codec=r.Ros2Builtin_interfacesMsgDurationCodec,value=codec.decode(input);
check(value.sec === -1349156469);
check(value.nanosec === 763345474);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("072b89b550057009"),codec=r.Ros2Builtin_interfacesMsgTimeCodec,value=codec.decode(input);
check(value.sec === -1249301753);
check(value.nanosec === 158336336);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes("671d78bf933ae800070000007630353433ceb2000100"),codec=r.Ros2Interbotix_footswitch_msgsMsgFootswitchStateCodec,value=codec.decode(input);
check(value.header.stamp.sec === -1082647193);
check(value.header.stamp.nanosec === 15219347);
check(value.header.frame_id === "v0543β");
check(value.state.length == 3);
check(value.state[0] === false);
check(value.state[1] === true);
check(value.state[2] === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("796194e6"),codec=r.Ros2Interbotix_slate_msgsSrvSetLightState_RequestCodec,value=codec.decode(input);
check(value.light_state === 3868483961);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("01070000007666363238ceb2"),codec=r.Ros2Interbotix_slate_msgsSrvSetLightState_ResponseCodec,value=codec.decode(input);
check(value.success === true);
check(value.message === "vf628β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("070000007663616139ceb2"),codec=r.Ros2Interbotix_slate_msgsSrvSetString_RequestCodec,value=codec.decode(input);
check(value.data === "vcaa9β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("01070000007633666136ceb2"),codec=r.Ros2Interbotix_slate_msgsSrvSetString_ResponseCodec,value=codec.decode(input);
check(value.success === true);
check(value.message === "v3fa6β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("b101d13db0e844ce211b6970"),codec=r.Ros2Interbotix_xs_msgsMsgArmJoyCodec,value=codec.decode(input);
check(value.ee_x_cmd === -79);
check(value.ee_y_cmd === 1);
check(value.ee_z_cmd === -47);
check(value.ee_roll_cmd === 61);
check(value.ee_pitch_cmd === -80);
check(value.waist_cmd === -24);
check(value.gripper_cmd === 68);
check(value.pose_cmd === -50);
check(value.speed_cmd === 33);
check(value.speed_toggle_cmd === 27);
check(value.gripper_pwm_cmd === 105);
check(value.torque_cmd === 112);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("a616f08a3aea4ac83497c285647695d9e8"),codec=r.Ros2Interbotix_xs_msgsMsgHexJoyCodec,value=codec.decode(input);
check(value.world_x_cmd === -90);
check(value.world_y_cmd === 22);
check(value.world_yaw_cmd === -16);
check(value.place_x_cmd === -118);
check(value.place_y_cmd === 58);
check(value.place_z_cmd === -22);
check(value.place_roll_cmd === 74);
check(value.place_pitch_cmd === -56);
check(value.pose_cmd === 52);
check(value.move_type_cmd === -105);
check(value.gait_toggle_cmd === -62);
check(value.leg_toggle_cmd === -123);
check(value.stance_cmd === 100);
check(value.reboot_cmd === 118);
check(value.set_home_pose_cmd === -107);
check(value.speed_cmd === -39);
check(value.speed_toggle_cmd === -24);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_9(){const input=bytes("070000007665393031ceb20100000000e01443"),codec=r.Ros2Interbotix_xs_msgsMsgJointGroupCommandCodec,value=codec.decode(input);
check(value.name === "ve901β");
check(value.cmd.length == 1);
check(value.cmd[0] === 148.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("070000007631346538ceb2006058c3"),codec=r.Ros2Interbotix_xs_msgsMsgJointSingleCommandCodec,value=codec.decode(input);
check(value.name === "v14e8β");
check(value.cmd === -216.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes("01000000070000007634633432ceb201000000f2a176fd"),codec=r.Ros2Interbotix_xs_msgsMsgJointTempsCodec,value=codec.decode(input);
check(value.names.length == 1);
check(value.names[0] === "v4c42β");
check(value.temps.length == 1);
check(value.temps[0] === -42556942);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("070000007662626165ceb2070000007661396536ceb29cb7b9d877c30737070000007631396133ceb201000000070000007666323937ceb2010000000100000000000000002852c0010000000000000000105fc0010000000000000000106bc00100000000000000005462c0083b49bb3f860d2d"),codec=r.Ros2Interbotix_xs_msgsMsgJointTrajectoryCommandCodec,value=codec.decode(input);
check(value.cmd_type === "vbbaeβ");
check(value.name === "va9e6β");
check(value.traj.header.stamp.sec === -658917476);
check(value.traj.header.stamp.nanosec === 923255671);
check(value.traj.header.frame_id === "v19a3β");
check(value.traj.joint_names.length == 1);
check(value.traj.joint_names[0] === "vf297β");
check(value.traj.points.length == 1);
check(value.traj.points[0].positions.length == 1);
check(value.traj.points[0].positions[0] === -72.625);
check(value.traj.points[0].velocities.length == 1);
check(value.traj.points[0].velocities[0] === -124.25);
check(value.traj.points[0].accelerations.length == 1);
check(value.traj.points[0].accelerations[0] === -216.5);
check(value.traj.points[0].effort.length == 1);
check(value.traj.points[0].effort[0] === -146.625);
check(value.traj.points[0].time_from_start.sec === -1152828664);
check(value.traj.points[0].time_from_start.nanosec === 755861055);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes("00000000009853400000000000e852c0166b1cdf6fbfc9bd7ab0a0b32775"),codec=r.Ros2Interbotix_xs_msgsMsgLocobotJoyCodec,value=codec.decode(input);
check(value.base_x_cmd === 78.375);
check(value.base_theta_cmd === -75.625);
check(value.base_reset_odom_cmd === 22);
check(value.pan_cmd === 107);
check(value.tilt_cmd === 28);
check(value.ee_x_cmd === -33);
check(value.ee_y_cmd === 111);
check(value.ee_z_cmd === -65);
check(value.ee_roll_cmd === -55);
check(value.ee_pitch_cmd === -67);
check(value.waist_cmd === 122);
check(value.gripper_cmd === -80);
check(value.pose_cmd === -96);
check(value.speed_cmd === -77);
check(value.speed_toggle_cmd === 39);
check(value.gripper_pwm_cmd === 117);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("bb581eae"),codec=r.Ros2Interbotix_xs_msgsMsgTurretJoyCodec,value=codec.decode(input);
check(value.pan_cmd === -69);
check(value.tilt_cmd === 88);
check(value.speed_cmd === 30);
check(value.speed_toggle_cmd === -82);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("070000007638386462ceb2070000007637653063ceb2dfa9413e2c5cb1539d9821ab43b3634af9e26ad312302b4de1c5db20"),codec=r.Ros2Interbotix_xs_msgsSrvMotorGains_RequestCodec,value=codec.decode(input);
check(value.cmd_type === "v88dbβ");
check(value.name === "v7e0cβ");
check(value.kp_pos === 1044490719);
check(value.ki_pos === 1404132396);
check(value.kd_pos === -1423861603);
check(value.k1 === 1248047939);
check(value.k2 === -747969799);
check(value.kp_vel === 1294675986);
check(value.ki_vel === 551273953);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_16(){const input=bytes(""),codec=r.Ros2Interbotix_xs_msgsSrvMotorGains_ResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("070000007631386365ceb2070000007634653936ceb2070000007638373766ceb2070000007633373861ceb24fb5ebc2c1862b6e"),codec=r.Ros2Interbotix_xs_msgsSrvOperatingModes_RequestCodec,value=codec.decode(input);
check(value.cmd_type === "v18ceβ");
check(value.name === "v4e96β");
check(value.mode === "v877fβ");
check(value.profile_type === "v378aβ");
check(value.profile_velocity === -1024740017);
check(value.profile_acceleration === 1848346305);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes(""),codec=r.Ros2Interbotix_xs_msgsSrvOperatingModes_ResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_19(){const input=bytes("070000007665353131ceb2070000007631356366ceb20000"),codec=r.Ros2Interbotix_xs_msgsSrvReboot_RequestCodec,value=codec.decode(input);
check(value.cmd_type === "ve511β");
check(value.name === "v15cfβ");
check(value.enable === false);
check(value.smart_reboot === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_20(){const input=bytes(""),codec=r.Ros2Interbotix_xs_msgsSrvReboot_ResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_21(){const input=bytes("070000007637653432ceb2070000007637323333ceb2070000007661613938ceb2016ee2eb"),codec=r.Ros2Interbotix_xs_msgsSrvRegisterValues_RequestCodec,value=codec.decode(input);
check(value.cmd_type === "v7e42β");
check(value.name === "v7233β");
check(value.reg === "vaa98β");
check(value.value === -337482239);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_22(){const input=bytes("010000001ad1dd83"),codec=r.Ros2Interbotix_xs_msgsSrvRegisterValues_ResponseCodec,value=codec.decode(input);
check(value.values.length == 1);
check(value.values[0] === -2082615014);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_23(){const input=bytes("070000007634623831ceb2070000007636373831ceb2"),codec=r.Ros2Interbotix_xs_msgsSrvRobotInfo_RequestCodec,value=codec.decode(input);
check(value.cmd_type === "v4b81β");
check(value.name === "v6781β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_24(){const input=bytes("070000007631663533ceb2070000007632356366ceb201000000070000007666383966ceb20100000048760100000000c043430100000000c0de4201000000000032c10100000000c0594301000000acd1ee2701000000070000007666616666ceb2"),codec=r.Ros2Interbotix_xs_msgsSrvRobotInfo_ResponseCodec,value=codec.decode(input);
check(value.mode === "v1f53β");
check(value.profile_type === "v25cfβ");
check(value.joint_names.length == 1);
check(value.joint_names[0] === "vf89fβ");
check(value.joint_ids.length == 1);
check(value.joint_ids[0] === 30280);
check(value.joint_lower_limits.length == 1);
check(value.joint_lower_limits[0] === 195.75);
check(value.joint_upper_limits.length == 1);
check(value.joint_upper_limits[0] === 111.375);
check(value.joint_velocity_limits.length == 1);
check(value.joint_velocity_limits[0] === -11.125);
check(value.joint_sleep_positions.length == 1);
check(value.joint_sleep_positions[0] === 217.75);
check(value.joint_state_indices.length == 1);
check(value.joint_state_indices[0] === -11860);
check(value.num_joints === 10222);
check(value.name.length == 1);
check(value.name[0] === "vfaffβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_25(){const input=bytes("070000007637643633ceb2070000007633656462ceb200"),codec=r.Ros2Interbotix_xs_msgsSrvTorqueEnable_RequestCodec,value=codec.decode(input);
check(value.cmd_type === "v7d63β");
check(value.name === "v3edbβ");
check(value.enable === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_26(){const input=bytes(""),codec=r.Ros2Interbotix_xs_msgsSrvTorqueEnable_ResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_27(){const input=bytes("681252e4acc1ba37070000007666316532ceb2"),codec=r.Ros2Std_msgsMsgHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -464383384);
check(value.stamp.nanosec === 934986156);
check(value.frame_id === "vf1e2β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_28(){const input=bytes("9e1673e752d0f60f070000007636363034ceb201000000070000007633663435ceb201000000010000000000000000d850400100000000000000000863c0010000000000000000b857c0010000000000000000d8514075dbb8e27217aa02"),codec=r.Ros2Trajectory_msgsMsgJointTrajectoryCodec,value=codec.decode(input);
check(value.header.stamp.sec === -411887970);
check(value.header.stamp.nanosec === 267833426);
check(value.header.frame_id === "v6604β");
check(value.joint_names.length == 1);
check(value.joint_names[0] === "v3f45β");
check(value.points.length == 1);
check(value.points[0].positions.length == 1);
check(value.points[0].positions[0] === 67.375);
check(value.points[0].velocities.length == 1);
check(value.points[0].velocities[0] === -152.25);
check(value.points[0].accelerations.length == 1);
check(value.points[0].accelerations[0] === -94.875);
check(value.points[0].effort.length == 1);
check(value.points[0].effort[0] === 71.375);
check(value.points[0].time_from_start.sec === -491201675);
check(value.points[0].time_from_start.nanosec === 44701554);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_29(){const input=bytes("010000000000000000a042c00100000000000000007c6e400100000000000000002034c0010000000000000000fc6840b47e2a6f9fb8c510"),codec=r.Ros2Trajectory_msgsMsgJointTrajectoryPointCodec,value=codec.decode(input);
check(value.positions.length == 1);
check(value.positions[0] === -37.25);
check(value.velocities.length == 1);
check(value.velocities[0] === 243.875);
check(value.accelerations.length == 1);
check(value.accelerations[0] === -20.125);
check(value.effort.length == 1);
check(value.effort[0] === 199.875);
check(value.time_from_start.sec === 1865055924);
check(value.time_from_start.nanosec === 281393311);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

export function main(){
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
return 0;
}
