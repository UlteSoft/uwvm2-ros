/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/trossen-ros1-melodic-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("41a82588c71e9838"),codec=r.Ros1DurationCodec,value=codec.decode(input);
check(value.sec === -2010798015);
check(value.nsec === 949493447);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("5fef3f4f200e3620c74aacfe"),codec=r.Ros1Interbotix_xs_msgsArmJoyCodec,value=codec.decode(input);
check(value.ee_x_cmd === 95);
check(value.ee_y_cmd === -17);
check(value.ee_z_cmd === 63);
check(value.ee_roll_cmd === 79);
check(value.ee_pitch_cmd === 32);
check(value.waist_cmd === 14);
check(value.gripper_cmd === 54);
check(value.pose_cmd === 32);
check(value.speed_cmd === -57);
check(value.speed_toggle_cmd === 74);
check(value.gripper_pwm_cmd === -84);
check(value.torque_cmd === -2);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes("d464edf84898010dda8a07ceeaf8ac3fb9"),codec=r.Ros1Interbotix_xs_msgsHexJoyCodec,value=codec.decode(input);
check(value.world_x_cmd === -44);
check(value.world_y_cmd === 100);
check(value.world_yaw_cmd === -19);
check(value.place_x_cmd === -8);
check(value.place_y_cmd === 72);
check(value.place_z_cmd === -104);
check(value.place_roll_cmd === 1);
check(value.place_pitch_cmd === 13);
check(value.pose_cmd === -38);
check(value.move_type_cmd === -118);
check(value.gait_toggle_cmd === 7);
check(value.leg_toggle_cmd === -50);
check(value.stance_cmd === -22);
check(value.reboot_cmd === -8);
check(value.set_home_pose_cmd === -84);
check(value.speed_cmd === 63);
check(value.speed_toggle_cmd === -71);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("070000007630383663ceb201000000008020c3"),codec=r.Ros1Interbotix_xs_msgsJointGroupCommandCodec,value=codec.decode(input);
check(value.name === "v086cβ");
check(value.cmd.length == 1);
check(value.cmd[0] === -160.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("070000007666663037ceb200402143"),codec=r.Ros1Interbotix_xs_msgsJointSingleCommandCodec,value=codec.decode(input);
check(value.name === "vff07β");
check(value.cmd === 161.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("01000000070000007666323433ceb20100000062f87743"),codec=r.Ros1Interbotix_xs_msgsJointTempsCodec,value=codec.decode(input);
check(value.names.length == 1);
check(value.names[0] === "vf243β");
check(value.temps.length == 1);
check(value.temps[0] === 1131935842);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("070000007662626536ceb2070000007635343830ceb20d0a486d49725f192f305232070000007639653762ceb201000000070000007632323461ceb201000000010000000000000000286d40010000000000000000000ec0010000000000000000ec61400100000000000000000049c006397c9f65f1c703"),codec=r.Ros1Interbotix_xs_msgsJointTrajectoryCommandCodec,value=codec.decode(input);
check(value.cmd_type === "vbbe6β");
check(value.name === "v5480β");
check(value.traj.header.seq === 1833437709);
check(value.traj.header.stamp.sec === 425685577);
check(value.traj.header.stamp.nsec === 844247087);
check(value.traj.header.frame_id === "v9e7bβ");
check(value.traj.joint_names.length == 1);
check(value.traj.joint_names[0] === "v224aβ");
check(value.traj.points.length == 1);
check(value.traj.points[0].positions.length == 1);
check(value.traj.points[0].positions[0] === 233.25);
check(value.traj.points[0].velocities.length == 1);
check(value.traj.points[0].velocities[0] === -3.75);
check(value.traj.points[0].accelerations.length == 1);
check(value.traj.points[0].accelerations[0] === 143.375);
check(value.traj.points[0].effort.length == 1);
check(value.traj.points[0].effort[0] === -50.0);
check(value.traj.points[0].time_from_start.sec === -1619248890);
check(value.traj.points[0].time_from_start.nsec === 63435109);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("0000000000385840000000000040574016198c4fff2f0cec677530ae626f"),codec=r.Ros1Interbotix_xs_msgsLocobotJoyCodec,value=codec.decode(input);
check(value.base_x_cmd === 96.875);
check(value.base_theta_cmd === 93.0);
check(value.base_reset_odom_cmd === 22);
check(value.pan_cmd === 25);
check(value.tilt_cmd === -116);
check(value.ee_x_cmd === 79);
check(value.ee_y_cmd === -1);
check(value.ee_z_cmd === 47);
check(value.ee_roll_cmd === 12);
check(value.ee_pitch_cmd === -20);
check(value.waist_cmd === 103);
check(value.gripper_cmd === 117);
check(value.pose_cmd === 48);
check(value.speed_cmd === -82);
check(value.speed_toggle_cmd === 98);
check(value.gripper_pwm_cmd === 111);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("070000007663336535ceb2070000007638333865ceb2be42029a4db7f2f7fc73620f49b73c9bf3e6350273db68e9802e9884"),codec=r.Ros1Interbotix_xs_msgsMotorGainsRequestCodec,value=codec.decode(input);
check(value.cmd_type === "vc3e5β");
check(value.name === "v838eβ");
check(value.kp_pos === -1711127874);
check(value.ki_pos === -135088307);
check(value.kd_pos === 258110460);
check(value.k1 === -1690519735);
check(value.k2 === 37086963);
check(value.kp_vel === -379004045);
check(value.ki_vel === -2070401408);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_9(){const input=bytes(""),codec=r.Ros1Interbotix_xs_msgsMotorGainsResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("070000007662393232ceb2070000007666326536ceb2070000007633623066ceb2070000007666626334ceb243dd31b6513b864e"),codec=r.Ros1Interbotix_xs_msgsOperatingModesRequestCodec,value=codec.decode(input);
check(value.cmd_type === "vb922β");
check(value.name === "vf2e6β");
check(value.mode === "v3b0fβ");
check(value.profile_type === "vfbc4β");
check(value.profile_velocity === -1238246077);
check(value.profile_acceleration === 1317419857);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes(""),codec=r.Ros1Interbotix_xs_msgsOperatingModesResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("070000007638613737ceb2070000007632346531ceb20001"),codec=r.Ros1Interbotix_xs_msgsRebootRequestCodec,value=codec.decode(input);
check(value.cmd_type === "v8a77β");
check(value.name === "v24e1β");
check(value.enable === false);
check(value.smart_reboot === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes(""),codec=r.Ros1Interbotix_xs_msgsRebootResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("070000007633333063ceb2070000007635343239ceb2070000007636396638ceb27f0ea258"),codec=r.Ros1Interbotix_xs_msgsRegisterValuesRequestCodec,value=codec.decode(input);
check(value.cmd_type === "v330cβ");
check(value.name === "v5429β");
check(value.reg === "v69f8β");
check(value.value === 1487015551);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("010000000c086b41"),codec=r.Ros1Interbotix_xs_msgsRegisterValuesResponseCodec,value=codec.decode(input);
check(value.values.length == 1);
check(value.values[0] === 1097533452);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_16(){const input=bytes("070000007639616439ceb2070000007662393966ceb2"),codec=r.Ros1Interbotix_xs_msgsRobotInfoRequestCodec,value=codec.decode(input);
check(value.cmd_type === "v9ad9β");
check(value.name === "vb99fβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("070000007634646535ceb2070000007639613966ceb201000000070000007666393963ceb201000000e0ed0100000000c0d0c20100000000a041c30100000000000d420100000000204b4301000000e58b7bdf01000000070000007632626137ceb2"),codec=r.Ros1Interbotix_xs_msgsRobotInfoResponseCodec,value=codec.decode(input);
check(value.mode === "v4de5β");
check(value.profile_type === "v9a9fβ");
check(value.joint_names.length == 1);
check(value.joint_names[0] === "vf99cβ");
check(value.joint_ids.length == 1);
check(value.joint_ids[0] === -4640);
check(value.joint_lower_limits.length == 1);
check(value.joint_lower_limits[0] === -104.375);
check(value.joint_upper_limits.length == 1);
check(value.joint_upper_limits[0] === -193.625);
check(value.joint_velocity_limits.length == 1);
check(value.joint_velocity_limits[0] === 35.25);
check(value.joint_sleep_positions.length == 1);
check(value.joint_sleep_positions[0] === 203.125);
check(value.joint_state_indices.length == 1);
check(value.joint_state_indices[0] === -29723);
check(value.num_joints === -8325);
check(value.name.length == 1);
check(value.name[0] === "v2ba7β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes("070000007666376231ceb2070000007665623164ceb200"),codec=r.Ros1Interbotix_xs_msgsTorqueEnableRequestCodec,value=codec.decode(input);
check(value.cmd_type === "vf7b1β");
check(value.name === "veb1dβ");
check(value.enable === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_19(){const input=bytes(""),codec=r.Ros1Interbotix_xs_msgsTorqueEnableResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_20(){const input=bytes("352a8eb4"),codec=r.Ros1Interbotix_xs_msgsTurretJoyCodec,value=codec.decode(input);
check(value.pan_cmd === 53);
check(value.tilt_cmd === 42);
check(value.speed_cmd === -114);
check(value.speed_toggle_cmd === -76);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_21(){const input=bytes("a949162e20488fdde929b81e070000007632663062ceb2"),codec=r.Ros1Std_msgsHeaderCodec,value=codec.decode(input);
check(value.seq === 773212585);
check(value.stamp.sec === 3717154848);
check(value.stamp.nsec === 515385833);
check(value.frame_id === "v2f0bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_22(){const input=bytes("c8280b76c3d5bc08"),codec=r.Ros1TimeCodec,value=codec.decode(input);
check(value.sec === 1980442824);
check(value.nsec === 146593219);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_23(){const input=bytes("87c81e0ba0199d97984cdf00070000007665326666ceb201000000070000007637616235ceb201000000010000000000000000785ac0010000000000000000e034c0010000000000000000805240010000000000000000385bc0a2a8b9ee05f53c1f"),codec=r.Ros1Trajectory_msgsJointTrajectoryCodec,value=codec.decode(input);
check(value.header.seq === 186566791);
check(value.header.stamp.sec === 2543655328);
check(value.header.stamp.nsec === 14634136);
check(value.header.frame_id === "ve2ffβ");
check(value.joint_names.length == 1);
check(value.joint_names[0] === "v7ab5β");
check(value.points.length == 1);
check(value.points[0].positions.length == 1);
check(value.points[0].positions[0] === -105.875);
check(value.points[0].velocities.length == 1);
check(value.points[0].velocities[0] === -20.875);
check(value.points[0].accelerations.length == 1);
check(value.points[0].accelerations[0] === 74.0);
check(value.points[0].effort.length == 1);
check(value.points[0].effort[0] === -108.875);
check(value.points[0].time_from_start.sec === -289822558);
check(value.points[0].time_from_start.nsec === 524088581);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_24(){const input=bytes("010000000000000000c046c00100000000000000000008c00100000000000000004858400100000000000000008c64c02fd5b9dce5e29b05"),codec=r.Ros1Trajectory_msgsJointTrajectoryPointCodec,value=codec.decode(input);
check(value.positions.length == 1);
check(value.positions[0] === -45.5);
check(value.velocities.length == 1);
check(value.velocities[0] === -3.0);
check(value.accelerations.length == 1);
check(value.accelerations[0] === 97.125);
check(value.effort.length == 1);
check(value.effort[0] === -164.375);
check(value.time_from_start.sec === -591801041);
check(value.time_from_start.nsec === 94102245);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_25(){const input=bytes(""),codec=r.Ros1Xarm_msgsClearErrRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_26(){const input=bytes("04c2070000007633393261ceb2"),codec=r.Ros1Xarm_msgsClearErrResponseCodec,value=codec.decode(input);
check(value.ret === -15868);
check(value.message === "v392aβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_27(){const input=bytes("79c12017d60bdd87"),codec=r.Ros1Xarm_msgsConfigToolModbusRequestCodec,value=codec.decode(input);
check(value.baud_rate === 388022649);
check(value.timeout_ms === -2015556650);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_28(){const input=bytes("070000007638616361ceb2adff"),codec=r.Ros1Xarm_msgsConfigToolModbusResponseCodec,value=codec.decode(input);
check(value.message === "v8acaβ");
check(value.ret === -83);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_29(){const input=bytes("427a"),codec=r.Ros1Xarm_msgsGetAnalogIORequestCodec,value=codec.decode(input);
check(value.port_num === 31298);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_30(){const input=bytes("00808542f15e070000007665366165ceb2"),codec=r.Ros1Xarm_msgsGetAnalogIOResponseCodec,value=codec.decode(input);
check(value.analog_value === 66.75);
check(value.ret === 24305);
check(value.message === "ve6aeβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_31(){const input=bytes("1edc"),codec=r.Ros1Xarm_msgsGetControllerDigitalIORequestCodec,value=codec.decode(input);
check(value.io_num === -9186);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_32(){const input=bytes("f77f2f9f070000007631613862ceb2"),codec=r.Ros1Xarm_msgsGetControllerDigitalIOResponseCodec,value=codec.decode(input);
check(value.ret === 32759);
check(value.value === -24785);
check(value.message === "v1a8bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_33(){const input=bytes(""),codec=r.Ros1Xarm_msgsGetDigitalIORequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_34(){const input=bytes("853edd303f6fd4a9a446070000007630346339ceb2"),codec=r.Ros1Xarm_msgsGetDigitalIOResponseCodec,value=codec.decode(input);
check(value.digital_1 === 819805829);
check(value.digital_2 === -1445695681);
check(value.ret === 18084);
check(value.message === "v04c9β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_35(){const input=bytes(""),codec=r.Ros1Xarm_msgsGetErrRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_36(){const input=bytes("f045070000007636353464ceb2"),codec=r.Ros1Xarm_msgsGetErrResponseCodec,value=codec.decode(input);
check(value.err === 17904);
check(value.message === "v654dβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_37(){const input=bytes("00402543"),codec=r.Ros1Xarm_msgsGripperConfigRequestCodec,value=codec.decode(input);
check(value.pulse_vel === 165.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_38(){const input=bytes("7e09070000007636376264ceb2"),codec=r.Ros1Xarm_msgsGripperConfigResponseCodec,value=codec.decode(input);
check(value.ret === 2430);
check(value.message === "v67bdβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_39(){const input=bytes("00803ac2"),codec=r.Ros1Xarm_msgsGripperMoveRequestCodec,value=codec.decode(input);
check(value.pulse_pos === -46.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_40(){const input=bytes("f1c3070000007632636534ceb2"),codec=r.Ros1Xarm_msgsGripperMoveResponseCodec,value=codec.decode(input);
check(value.ret === -15375);
check(value.message === "v2ce4β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_41(){const input=bytes(""),codec=r.Ros1Xarm_msgsGripperStateRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_42(){const input=bytes("00802bc25fd4"),codec=r.Ros1Xarm_msgsGripperStateResponseCodec,value=codec.decode(input);
check(value.curr_pos === -42.875);
check(value.err_code === -11169);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_43(){const input=bytes("4e02582cf45351b500602ec300805f43"),codec=r.Ros1Xarm_msgsIOStateCodec,value=codec.decode(input);
check(value.digital_1 === 743965262);
check(value.digital_2 === -1252961292);
check(value.analog_1 === -174.375);
check(value.analog_2 === 223.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_44(){const input=bytes("01000000000081c10000b6c10000a4c1000009420040c9c2"),codec=r.Ros1Xarm_msgsMoveRequestCodec,value=codec.decode(input);
check(value.pose.length == 1);
check(value.pose[0] === -16.125);
check(value.mvvelo === -22.75);
check(value.mvacc === -20.5);
check(value.mvtime === 34.25);
check(value.mvradii === -100.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_45(){const input=bytes("55ee070000007635653732ceb2"),codec=r.Ros1Xarm_msgsMoveResponseCodec,value=codec.decode(input);
check(value.ret === -4523);
check(value.message === "v5e72β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_46(){const input=bytes("2bf13c7074b05c65b401b8c39286010000000000c3c1008010c200004d420040e14200008b42000079c20000dcc000a023430000354200e01f4300a058c3008027430040a242"),codec=r.Ros1Xarm_msgsRobotMsgCodec,value=codec.decode(input);
check(value.state === -3797);
check(value.mode === 28732);
check(value.cmdnum === -20364);
check(value.mt_brake === 25948);
check(value.mt_able === 436);
check(value.err === -15432);
check(value.warn === -31086);
check(value.angle.length == 1);
check(value.angle[0] === -24.375);
check(value.pose.length == 6);
check(value.pose[0] === -36.125);
check(value.pose[1] === 51.25);
check(value.pose[2] === 112.625);
check(value.pose[3] === 69.5);
check(value.pose[4] === -62.25);
check(value.pose[5] === -6.875);
check(value.offset.length == 6);
check(value.offset[0] === 163.625);
check(value.offset[1] === 45.25);
check(value.offset[2] === 159.875);
check(value.offset[3] === -216.625);
check(value.offset[4] === 167.5);
check(value.offset[5] === 81.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_47(){const input=bytes("259884ae"),codec=r.Ros1Xarm_msgsSetAxisRequestCodec,value=codec.decode(input);
check(value.id === -26587);
check(value.data === -20860);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_48(){const input=bytes("7ad9070000007666356461ceb2"),codec=r.Ros1Xarm_msgsSetAxisResponseCodec,value=codec.decode(input);
check(value.ret === -9862);
check(value.message === "vf5daβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_49(){const input=bytes("9f3385f8"),codec=r.Ros1Xarm_msgsSetDigitalIORequestCodec,value=codec.decode(input);
check(value.io_num === 13215);
check(value.value === -1915);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_50(){const input=bytes("da06070000007630386136ceb2"),codec=r.Ros1Xarm_msgsSetDigitalIOResponseCodec,value=codec.decode(input);
check(value.ret === 1754);
check(value.message === "v08a6β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_51(){const input=bytes("9b8a"),codec=r.Ros1Xarm_msgsSetInt16RequestCodec,value=codec.decode(input);
check(value.data === -30053);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_52(){const input=bytes("65fd070000007636373465ceb2"),codec=r.Ros1Xarm_msgsSetInt16ResponseCodec,value=codec.decode(input);
check(value.ret === -667);
check(value.message === "v674eβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_53(){const input=bytes("00c008c30040e5c200000ec300e007c3"),codec=r.Ros1Xarm_msgsSetLoadRequestCodec,value=codec.decode(input);
check(value.mass === -136.75);
check(value.xc === -114.625);
check(value.yc === -142.0);
check(value.zc === -135.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_54(){const input=bytes("55ef070000007663303261ceb2"),codec=r.Ros1Xarm_msgsSetLoadResponseCodec,value=codec.decode(input);
check(value.ret === -4267);
check(value.message === "vc02aβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_55(){const input=bytes("01000000f9a734"),codec=r.Ros1Xarm_msgsSetToolModbusRequestCodec,value=codec.decode(input);
check(value.send_data.length == 1);
check(value.send_data[0] === 249);
check(value.respond_len === 13479);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_56(){const input=bytes("91890100000067"),codec=r.Ros1Xarm_msgsSetToolModbusResponseCodec,value=codec.decode(input);
check(value.ret === -30319);
check(value.respond_data.length == 1);
check(value.respond_data[0] === 103);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_57(){const input=bytes("0000dac2004064c300801ec30000d44200c02fc30000f541"),codec=r.Ros1Xarm_msgsTCPOffsetRequestCodec,value=codec.decode(input);
check(value.x === -109.0);
check(value.y === -228.25);
check(value.z === -158.5);
check(value.roll === 106.0);
check(value.pitch === -175.75);
check(value.yaw === 30.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_58(){const input=bytes("283d070000007662626330ceb2"),codec=r.Ros1Xarm_msgsTCPOffsetResponseCodec,value=codec.decode(input);
check(value.ret === 15656);
check(value.message === "vbbc0β");
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
return 0;
}
