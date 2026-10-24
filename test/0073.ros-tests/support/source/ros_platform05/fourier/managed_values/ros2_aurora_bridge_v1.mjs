/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/fourier-ros2-aurora-bridge-v1-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("070000007661313764ceb20100000000000000009053c0010000000000000000486d400100000000000000004045c05a55a620"),codec=r.Ros2Uwvm_fourier_bridgeMsgGroupStateCodec,value=codec.decode(input);
check(value.group === "va17dβ");
check(value.position.length == 1);
check(value.position[0] === -78.25);
check(value.velocity.length == 1);
check(value.velocity[0] === 234.25);
check(value.effort.length == 1);
check(value.effort[0] === -42.5);
check(value.motion_state === 547771738);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("070000007635643061ceb2010000000000000000a05bc0"),codec=r.Ros2Uwvm_fourier_bridgeMsgGroupValuesCodec,value=codec.decode(input);
check(value.group === "v5d0aβ");
check(value.values.length == 1);
check(value.values[0] === -110.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes("77070000007631623865ceb201000000000000000090444001010000000000000000b05ac0d701fcd5"),codec=r.Ros2Uwvm_fourier_bridgeMsgMotionRecipeCodec,value=codec.decode(input);
check(value.move_type === 119);
check(value.group === "v1b8eβ");
check(value.position.length == 1);
check(value.position[0] === 41.125);
check(value.has_velocity === true);
check(value.velocity.length == 1);
check(value.velocity[0] === -106.75);
check(value.expected_velocity === -704904745);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("5a336bb9070000007637633136ceb241a7a44b070000007664313832ceb25d02fc2c070000007665633633ceb2010000000000000000f0584001000000070000007630613665ceb2010000000000000000c856c0010000000000000000003240010000000000000000a863c04de41a29"),codec=r.Ros2Uwvm_fourier_bridgeMsgStatusCodec,value=codec.decode(input);
check(value.fsm_state === -1184156838);
check(value.fsm_name === "v7c16β");
check(value.upper_fsm_state === 1269081921);
check(value.upper_fsm_name === "vd182β");
check(value.velocity_source === 754713181);
check(value.velocity_source_name === "vec63β");
check(value.stand_pose.length == 1);
check(value.stand_pose[0] === 99.75);
check(value.groups.length == 1);
check(value.groups[0].group === "v0a6eβ");
check(value.groups[0].position.length == 1);
check(value.groups[0].position[0] === -91.125);
check(value.groups[0].velocity.length == 1);
check(value.groups[0].velocity[0] === 18.0);
check(value.groups[0].effort.length == 1);
check(value.groups[0].effort[0] === -157.25);
check(value.groups[0].motion_state === 689628237);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("67adcf63ded4f8fd00000000004033c0000000000000324000000000005048400000000000805740"),codec=r.Ros2Uwvm_fourier_bridgeMsgVelocityCommandCodec,value=codec.decode(input);
check(value.sequence === 18300611137442393447n);
check(value.vx === -19.25);
check(value.vy === 18.0);
check(value.yaw === 48.625);
check(value.duration === 94.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("c31eeddb7a67da4501000000a1070000007664616265ceb2010000000000000000406d4001010000000000000000c462401ae52d8c"),codec=r.Ros2Uwvm_fourier_bridgeSrvExecuteMove_RequestCodec,value=codec.decode(input);
check(value.sequence === 5033449310913371843n);
check(value.moves.length == 1);
check(value.moves[0].move_type === 161);
check(value.moves[0].group === "vdabeβ");
check(value.moves[0].position.length == 1);
check(value.moves[0].position[0] === 234.0);
check(value.moves[0].has_velocity === true);
check(value.moves[0].velocity.length == 1);
check(value.moves[0].velocity[0] === 150.125);
check(value.moves[0].expected_velocity === -1943149286);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("278415ab070000007632383261ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvExecuteMove_ResponseCodec,value=codec.decode(input);
check(value.status === 2870314023);
check(value.error === "v282aβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("99732231200a15af070000007663653536ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetBaseData_RequestCodec,value=codec.decode(input);
check(value.sequence === 12616001064527885209n);
check(value.key === "vce56β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("a3e73fb1070000007664383964ceb2010000000000000000e85740"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetBaseData_ResponseCodec,value=codec.decode(input);
check(value.status === 2973755299);
check(value.error === "vd89dβ");
check(value.values.length == 1);
check(value.values[0] === 95.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_9(){const input=bytes("e2bd15ec5bc42272070000007637663665ceb2070000007665323632ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetCartesianState_RequestCodec,value=codec.decode(input);
check(value.sequence === 8224351768613862882n);
check(value.group === "v7f6eβ");
check(value.key === "ve262β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("cc558d8b070000007664663765ceb2010000000000000000046ac0"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetCartesianState_ResponseCodec,value=codec.decode(input);
check(value.status === 2341295564);
check(value.error === "vdf7eβ");
check(value.values.length == 1);
check(value.values[0] === -208.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes("4223028dfb5a3513070000007661623562ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetContactData_RequestCodec,value=codec.decode(input);
check(value.sequence === 1384112496935314242n);
check(value.key === "vab5bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("1226ecf7070000007636326532ceb2010000000000000000485a40"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetContactData_ResponseCodec,value=codec.decode(input);
check(value.status === 4159448594);
check(value.error === "v62e2β");
check(value.values.length == 1);
check(value.values[0] === 105.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes("5f5d7a91e6244d0f"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetFsmName_RequestCodec,value=codec.decode(input);
check(value.sequence === 1102578056477433183n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("4359f3e3070000007637636331ceb2070000007633633662ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetFsmName_ResponseCodec,value=codec.decode(input);
check(value.status === 3824376131);
check(value.error === "v7cc1β");
check(value.value === "v3c6bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("4394d09ffaede701"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetFsmState_RequestCodec,value=codec.decode(input);
check(value.sequence === 137339974336943171n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_16(){const input=bytes("4b77b292070000007638633661ceb2323ac0cc"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetFsmState_ResponseCodec,value=codec.decode(input);
check(value.status === 2461169483);
check(value.error === "v8c6aβ");
check(value.value === -859817422);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("f0e2cd04499bfa9a070000007662656463ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetGroupMotionState_RequestCodec,value=codec.decode(input);
check(value.sequence === 11167408963934085872n);
check(value.group === "vbedcβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes("8b7406eb070000007665346431ceb23b5e7ba4"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetGroupMotionState_ResponseCodec,value=codec.decode(input);
check(value.status === 3943068811);
check(value.error === "ve4d1β");
check(value.value === -1535418821);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_19(){const input=bytes("bbd9cfa902a0f837070000007664633932ceb2070000007633656266ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetGroupMotorCfg_RequestCodec,value=codec.decode(input);
check(value.sequence === 4033149399609629115n);
check(value.group === "vdc92β");
check(value.key === "v3ebfβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_20(){const input=bytes("e179b58e070000007665373035ceb2010000000000000000486bc0"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetGroupMotorCfg_ResponseCodec,value=codec.decode(input);
check(value.status === 2394257889);
check(value.error === "ve705β");
check(value.values.length == 1);
check(value.values[0] === -218.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_21(){const input=bytes("dc404ef16539796f070000007636396237ceb2070000007663353735ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetGroupState_RequestCodec,value=codec.decode(input);
check(value.sequence === 8032514520394907868n);
check(value.group === "v69b7β");
check(value.key === "vc575β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_22(){const input=bytes("dc0f6067070000007635316436ceb2010000000000000000805f40"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetGroupState_ResponseCodec,value=codec.decode(input);
check(value.status === 1734348764);
check(value.error === "v51d6β");
check(value.values.length == 1);
check(value.values[0] === 126.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_23(){const input=bytes("83f9ee8e3a80d910"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetSnapshot_RequestCodec,value=codec.decode(input);
check(value.sequence === 1214142563547543939n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_24(){const input=bytes("90ebd4ad070000007635666630ceb2d59f8598070000007632363465ceb20769ba1e070000007663613661ceb21bcce279070000007637633336ceb20100000000000000001c604001000000070000007635663730ceb2010000000000000000685f40010000000000000000606b40010000000000000000885640b90d2006"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetSnapshot_ResponseCodec,value=codec.decode(input);
check(value.status === 2916412304);
check(value.error === "v5ff0β");
check(value.value.fsm_state === -1736073259);
check(value.value.fsm_name === "v264eβ");
check(value.value.upper_fsm_state === 515533063);
check(value.value.upper_fsm_name === "vca6aβ");
check(value.value.velocity_source === 2044906523);
check(value.value.velocity_source_name === "v7c36β");
check(value.value.stand_pose.length == 1);
check(value.value.stand_pose[0] === 128.875);
check(value.value.groups.length == 1);
check(value.value.groups[0].group === "v5f70β");
check(value.value.groups[0].position.length == 1);
check(value.value.groups[0].position[0] === 125.625);
check(value.value.groups[0].velocity.length == 1);
check(value.value.groups[0].velocity[0] === 219.0);
check(value.value.groups[0].effort.length == 1);
check(value.value.groups[0].effort[0] === 90.125);
check(value.value.groups[0].motion_state === 102763961);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_25(){const input=bytes("4e0195adf778a233"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetStandPose_RequestCodec,value=codec.decode(input);
check(value.sequence === 3720669247325929806n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_26(){const input=bytes("67994b0f070000007661376633ceb2010000000000000000105440"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetStandPose_ResponseCodec,value=codec.decode(input);
check(value.status === 256612711);
check(value.error === "va7f3β");
check(value.values.length == 1);
check(value.values[0] === 80.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_27(){const input=bytes("d0b950ac69c06732"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetUpperFsmName_RequestCodec,value=codec.decode(input);
check(value.sequence === 3632083184592665040n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_28(){const input=bytes("26018211070000007636663939ceb2070000007632663333ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetUpperFsmName_ResponseCodec,value=codec.decode(input);
check(value.status === 293732646);
check(value.error === "v6f99β");
check(value.value === "v2f33β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_29(){const input=bytes("1620eae2af59dd7c"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetUpperFsmState_RequestCodec,value=codec.decode(input);
check(value.sequence === 8997446242517262358n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_30(){const input=bytes("c49398af070000007637653162ceb25762b13e"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetUpperFsmState_ResponseCodec,value=codec.decode(input);
check(value.status === 2946012100);
check(value.error === "v7e1bβ");
check(value.value === 1051812439);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_31(){const input=bytes("ce479e27773ea9b9"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetVelocitySourceName_RequestCodec,value=codec.decode(input);
check(value.sequence === 13378292849567483854n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_32(){const input=bytes("7cf5c461070000007634333939ceb2070000007630333333ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetVelocitySourceName_ResponseCodec,value=codec.decode(input);
check(value.status === 1640297852);
check(value.error === "v4399β");
check(value.value === "v0333β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_33(){const input=bytes("f371d3cb4a08e455"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetVelocitySource_RequestCodec,value=codec.decode(input);
check(value.sequence === 6189080905254138355n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_34(){const input=bytes("bdee004f070000007633333866ceb25d052573"),codec=r.Ros2Uwvm_fourier_bridgeSrvGetVelocitySource_ResponseCodec,value=codec.decode(input);
check(value.status === 1325461181);
check(value.error === "v338fβ");
check(value.value === 1931806045);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_35(){const input=bytes("419ebf93f8e7880de6c4e7a1"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetFsmState_RequestCodec,value=codec.decode(input);
check(value.sequence === 975284374142426689n);
check(value.state === -1578646298);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_36(){const input=bytes("804da34b070000007634623061ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetFsmState_ResponseCodec,value=codec.decode(input);
check(value.status === 1268993408);
check(value.error === "v4b0aβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_37(){const input=bytes("b72d252f0e5412b101000000070000007664336537ceb2010000000000000000206f4001000000070000007665393939ceb201000000000000000080644001000000070000007631383237ceb2010000000000000000286640"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetGroupCommand_RequestCodec,value=codec.decode(input);
check(value.sequence === 12759353114191277495n);
check(value.position.length == 1);
check(value.position[0].group === "vd3e7β");
check(value.position[0].values.length == 1);
check(value.position[0].values[0] === 249.0);
check(value.velocity.length == 1);
check(value.velocity[0].group === "ve999β");
check(value.velocity[0].values.length == 1);
check(value.velocity[0].values[0] === 164.0);
check(value.torque.length == 1);
check(value.torque[0].group === "v1827β");
check(value.torque[0].values.length == 1);
check(value.torque[0].values[0] === 177.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_38(){const input=bytes("31a193b7070000007663323665ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetGroupCommand_ResponseCodec,value=codec.decode(input);
check(value.status === 3079905585);
check(value.error === "vc26eβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_39(){const input=bytes("d058ad1e69219a8001000000070000007636613465ceb201000000000000000020404001000000070000007634343062ceb2010000000000000000b042c0"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetMotorCfgPd_RequestCodec,value=codec.decode(input);
check(value.sequence === 9266755918638176464n);
check(value.kp.length == 1);
check(value.kp[0].group === "v6a4eβ");
check(value.kp[0].values.length == 1);
check(value.kp[0].values[0] === 32.25);
check(value.kd.length == 1);
check(value.kd[0].group === "v440bβ");
check(value.kd[0].values.length == 1);
check(value.kd[0].values[0] === -37.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_40(){const input=bytes("9f296851070000007635616261ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetMotorCfgPd_ResponseCodec,value=codec.decode(input);
check(value.status === 1365780895);
check(value.error === "v5abaβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_41(){const input=bytes("680f9743d176a0dd0000000000c852400000000000d06dc00000000000b461c0"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetStandPose_RequestCodec,value=codec.decode(input);
check(value.sequence === 15969894919809994600n);
check(value.delta_z === 75.125);
check(value.delta_pitch === -238.5);
check(value.delta_yaw === -141.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_42(){const input=bytes("65932403070000007637656532ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetStandPose_ResponseCodec,value=codec.decode(input);
check(value.status === 52728677);
check(value.error === "v7ee2β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_43(){const input=bytes("0350a918ba299e863d35e3d7"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetUpperFsmState_RequestCodec,value=codec.decode(input);
check(value.sequence === 9700236526657032195n);
check(value.state === -672975555);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_44(){const input=bytes("dae5d5e3070000007635613764ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetUpperFsmState_ResponseCodec,value=codec.decode(input);
check(value.status === 3822446042);
check(value.error === "v5a7dβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_45(){const input=bytes("68d6f4a6d1afc33824394608"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetVelocitySource_RequestCodec,value=codec.decode(input);
check(value.sequence === 4090306201566631528n);
check(value.source === 138819876);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_46(){const input=bytes("a89e43b5070000007637666332ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetVelocitySource_ResponseCodec,value=codec.decode(input);
check(value.status === 3041107624);
check(value.error === "v7fc2β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_47(){const input=bytes("6fd1c01bd6a8f78500000000007040c000000000008013400000000000385d400000000000405740"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetVelocity_RequestCodec,value=codec.decode(input);
check(value.sequence === 9653369963834036591n);
check(value.vx === -32.875);
check(value.vy === 4.875);
check(value.yaw === 116.875);
check(value.duration === 93.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_48(){const input=bytes("5885b63c070000007635396632ceb2"),codec=r.Ros2Uwvm_fourier_bridgeSrvSetVelocity_ResponseCodec,value=codec.decode(input);
check(value.status === 1018594648);
check(value.error === "v59f2β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_49(){const input=bytes("dc08b02f657187b101000000070000007661646335ceb20000000000f86ac00000000000d85340"),codec=r.Ros2Uwvm_fourier_bridgeSrvWaitGroupsMotionComplete_RequestCodec,value=codec.decode(input);
check(value.sequence === 12792317945974884572n);
check(value.groups.length == 1);
check(value.groups[0] === "vadc5β");
check(value.timeout_seconds === -215.75);
check(value.poll_seconds === 79.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_50(){const input=bytes("31cab622070000007635383466ceb201"),codec=r.Ros2Uwvm_fourier_bridgeSrvWaitGroupsMotionComplete_ResponseCodec,value=codec.decode(input);
check(value.status === 582404657);
check(value.error === "v584fβ");
check(value.complete === true);
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
return 0;
}
