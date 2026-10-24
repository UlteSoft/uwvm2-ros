/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/fourier-ros1-aurora-bridge-v1-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("39b83ef880c109660100000069070000007639663361ceb2010000000000000000206f4001010000000000000000a861c0c3d964ee"),codec=r.Ros1Uwvm_fourier_bridgeExecuteMoveRequestCodec,value=codec.decode(input);
check(value.sequence === 7352620626323879993n);
check(value.moves.length == 1);
check(value.moves[0].move_type === 105);
check(value.moves[0].group === "v9f3aβ");
check(value.moves[0].position.length == 1);
check(value.moves[0].position[0] === 249.0);
check(value.moves[0].has_velocity === true);
check(value.moves[0].velocity.length == 1);
check(value.moves[0].velocity[0] === -141.25);
check(value.moves[0].expected_velocity === -295380541);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("d0c1d4a1070000007665396331ceb2"),codec=r.Ros1Uwvm_fourier_bridgeExecuteMoveResponseCodec,value=codec.decode(input);
check(value.status === 2715075024);
check(value.error === "ve9c1β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes("bddb14e204a2237c070000007639323631ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetBaseDataRequestCodec,value=codec.decode(input);
check(value.sequence === 8945171426744523709n);
check(value.key === "v9261β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("c4386351070000007635316162ceb2010000000000000000402f40"),codec=r.Ros1Uwvm_fourier_bridgeGetBaseDataResponseCodec,value=codec.decode(input);
check(value.status === 1365457092);
check(value.error === "v51abβ");
check(value.values.length == 1);
check(value.values[0] === 15.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("f029e24d4950d5d3070000007639386664ceb2070000007638656432ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetCartesianStateRequestCodec,value=codec.decode(input);
check(value.sequence === 15264194787811666416n);
check(value.group === "v98fdβ");
check(value.key === "v8ed2β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("6d398aad070000007630313232ceb2010000000000000000546c40"),codec=r.Ros1Uwvm_fourier_bridgeGetCartesianStateResponseCodec,value=codec.decode(input);
check(value.status === 2911517037);
check(value.error === "v0122β");
check(value.values.length == 1);
check(value.values[0] === 226.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("83295c483a506bd6070000007637643231ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetContactDataRequestCodec,value=codec.decode(input);
check(value.sequence === 15450531157876943235n);
check(value.key === "v7d21β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("aa59dd13070000007635643532ceb2010000000000000000f04dc0"),codec=r.Ros1Uwvm_fourier_bridgeGetContactDataResponseCodec,value=codec.decode(input);
check(value.status === 333273514);
check(value.error === "v5d52β");
check(value.values.length == 1);
check(value.values[0] === -59.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("fda878d544d14f4b"),codec=r.Ros1Uwvm_fourier_bridgeGetFsmNameRequestCodec,value=codec.decode(input);
check(value.sequence === 5426786169574172925n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_9(){const input=bytes("2f882be5070000007631326135ceb2070000007635323066ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetFsmNameResponseCodec,value=codec.decode(input);
check(value.status === 3844835375);
check(value.error === "v12a5β");
check(value.value === "v520fβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("554b99c1ec32ae5f"),codec=r.Ros1Uwvm_fourier_bridgeGetFsmStateRequestCodec,value=codec.decode(input);
check(value.sequence === 6894504071992527701n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes("336f9405070000007637386162ceb239ca0138"),codec=r.Ros1Uwvm_fourier_bridgeGetFsmStateResponseCodec,value=codec.decode(input);
check(value.status === 93613875);
check(value.error === "v78abβ");
check(value.value === 939641401);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("00be22f4b9c7156a070000007633393836ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetGroupMotionStateRequestCodec,value=codec.decode(input);
check(value.sequence === 7644235544010079744n);
check(value.group === "v3986β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes("1bc0c90b070000007666653863ceb231d026be"),codec=r.Ros1Uwvm_fourier_bridgeGetGroupMotionStateResponseCodec,value=codec.decode(input);
check(value.status === 197771291);
check(value.error === "vfe8cβ");
check(value.value === -1104752591);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("972834432e5103dd070000007661393034ceb2070000007635333638ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetGroupMotorCfgRequestCodec,value=codec.decode(input);
check(value.sequence === 15925661966450043031n);
check(value.group === "va904β");
check(value.key === "v5368β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("656c03a0070000007665353164ceb2010000000000000000d06fc0"),codec=r.Ros1Uwvm_fourier_bridgeGetGroupMotorCfgResponseCodec,value=codec.decode(input);
check(value.status === 2684578917);
check(value.error === "ve51dβ");
check(value.values.length == 1);
check(value.values[0] === -254.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_16(){const input=bytes("a3572aa91a2e1d37070000007661343331ceb2070000007666363636ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetGroupStateRequestCodec,value=codec.decode(input);
check(value.sequence === 3971381138452797347n);
check(value.group === "va431β");
check(value.key === "vf666β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("e1613b6b070000007662636133ceb2010000000000000000c05b40"),codec=r.Ros1Uwvm_fourier_bridgeGetGroupStateResponseCodec,value=codec.decode(input);
check(value.status === 1799053793);
check(value.error === "vbca3β");
check(value.values.length == 1);
check(value.values[0] === 111.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes("8ed7fffe37aec860"),codec=r.Ros1Uwvm_fourier_bridgeGetSnapshotRequestCodec,value=codec.decode(input);
check(value.sequence === 6974015578507827086n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_19(){const input=bytes("95690ed6070000007633343539ceb234f95aad070000007634633830ceb279ad4789070000007665353737ceb265081fee070000007634306537ceb20100000000000000009864c001000000070000007663383864ceb20100000000000000007c6dc00100000000000000008036c00100000000000000001861c09cb73da0"),codec=r.Ros1Uwvm_fourier_bridgeGetSnapshotResponseCodec,value=codec.decode(input);
check(value.status === 3591268757);
check(value.error === "v3459β");
check(value.value.fsm_state === -1386546892);
check(value.value.fsm_name === "v4c80β");
check(value.value.upper_fsm_state === -1991791239);
check(value.value.upper_fsm_name === "ve577β");
check(value.value.velocity_source === -299956123);
check(value.value.velocity_source_name === "v40e7β");
check(value.value.stand_pose.length == 1);
check(value.value.stand_pose[0] === -164.75);
check(value.value.groups.length == 1);
check(value.value.groups[0].group === "vc88dβ");
check(value.value.groups[0].position.length == 1);
check(value.value.groups[0].position[0] === -235.875);
check(value.value.groups[0].velocity.length == 1);
check(value.value.groups[0].velocity[0] === -22.5);
check(value.value.groups[0].effort.length == 1);
check(value.value.groups[0].effort[0] === -136.75);
check(value.value.groups[0].motion_state === -1606568036);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_20(){const input=bytes("5ca6d38ae5dfe414"),codec=r.Ros1Uwvm_fourier_bridgeGetStandPoseRequestCodec,value=codec.decode(input);
check(value.sequence === 1505574352418219612n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_21(){const input=bytes("1621ad4b070000007634353636ceb20100000000000000001858c0"),codec=r.Ros1Uwvm_fourier_bridgeGetStandPoseResponseCodec,value=codec.decode(input);
check(value.status === 1269637398);
check(value.error === "v4566β");
check(value.values.length == 1);
check(value.values[0] === -96.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_22(){const input=bytes("9f3bf1e92642c677"),codec=r.Ros1Uwvm_fourier_bridgeGetUpperFsmNameRequestCodec,value=codec.decode(input);
check(value.sequence === 8630658470803225503n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_23(){const input=bytes("0b3b5c6f070000007630306466ceb2070000007634303735ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetUpperFsmNameResponseCodec,value=codec.decode(input);
check(value.status === 1868315403);
check(value.error === "v00dfβ");
check(value.value === "v4075β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_24(){const input=bytes("b2c219620bbb2efc"),codec=r.Ros1Uwvm_fourier_bridgeGetUpperFsmStateRequestCodec,value=codec.decode(input);
check(value.sequence === 18171667204051419826n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_25(){const input=bytes("51fc1d39070000007666326463ceb21d7976b2"),codec=r.Ros1Uwvm_fourier_bridgeGetUpperFsmStateResponseCodec,value=codec.decode(input);
check(value.status === 958266449);
check(value.error === "vf2dcβ");
check(value.value === -1300858595);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_26(){const input=bytes("7ac866f7c3b15169"),codec=r.Ros1Uwvm_fourier_bridgeGetVelocitySourceNameRequestCodec,value=codec.decode(input);
check(value.sequence === 7589042302323443834n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_27(){const input=bytes("99b65df9070000007630386661ceb2070000007634383530ceb2"),codec=r.Ros1Uwvm_fourier_bridgeGetVelocitySourceNameResponseCodec,value=codec.decode(input);
check(value.status === 4183668377);
check(value.error === "v08faβ");
check(value.value === "v4850β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_28(){const input=bytes("81926f8938eb5817"),codec=r.Ros1Uwvm_fourier_bridgeGetVelocitySourceRequestCodec,value=codec.decode(input);
check(value.sequence === 1682353088879366785n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_29(){const input=bytes("94f07b41070000007639333139ceb2551eb3d3"),codec=r.Ros1Uwvm_fourier_bridgeGetVelocitySourceResponseCodec,value=codec.decode(input);
check(value.status === 1098641556);
check(value.error === "v9319β");
check(value.value === -743236011);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_30(){const input=bytes("070000007666313031ceb2010000000000000000f04440010000000000000000e05fc00100000000000000006c6340dfce3c49"),codec=r.Ros1Uwvm_fourier_bridgeGroupStateCodec,value=codec.decode(input);
check(value.group === "vf101β");
check(value.position.length == 1);
check(value.position[0] === 41.875);
check(value.velocity.length == 1);
check(value.velocity[0] === -127.5);
check(value.effort.length == 1);
check(value.effort[0] === 155.375);
check(value.motion_state === 1228721887);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_31(){const input=bytes("070000007666373530ceb20100000000000000007c6440"),codec=r.Ros1Uwvm_fourier_bridgeGroupValuesCodec,value=codec.decode(input);
check(value.group === "vf750β");
check(value.values.length == 1);
check(value.values[0] === 163.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_32(){const input=bytes("e4070000007634333463ceb20100000000000000008049c001010000000000000000b460403aa5eba5"),codec=r.Ros1Uwvm_fourier_bridgeMotionRecipeCodec,value=codec.decode(input);
check(value.move_type === 228);
check(value.group === "v434cβ");
check(value.position.length == 1);
check(value.position[0] === -51.0);
check(value.has_velocity === true);
check(value.velocity.length == 1);
check(value.velocity[0] === 133.625);
check(value.expected_velocity === -1511283398);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_33(){const input=bytes("9e7188182708bf86f5831c01"),codec=r.Ros1Uwvm_fourier_bridgeSetFsmStateRequestCodec,value=codec.decode(input);
check(value.sequence === 9709488285642420638n);
check(value.state === 18646005);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_34(){const input=bytes("da78f4c2070000007663663134ceb2"),codec=r.Ros1Uwvm_fourier_bridgeSetFsmStateResponseCodec,value=codec.decode(input);
check(value.status === 3270801626);
check(value.error === "vcf14β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_35(){const input=bytes("889be04f31e2d7d101000000070000007661343237ceb2010000000000000000604d4001000000070000007639653539ceb20100000000000000002038c001000000070000007639303463ceb20100000000000000003462c0"),codec=r.Ros1Uwvm_fourier_bridgeSetGroupCommandRequestCodec,value=codec.decode(input);
check(value.sequence === 15120802975341124488n);
check(value.position.length == 1);
check(value.position[0].group === "va427β");
check(value.position[0].values.length == 1);
check(value.position[0].values[0] === 58.75);
check(value.velocity.length == 1);
check(value.velocity[0].group === "v9e59β");
check(value.velocity[0].values.length == 1);
check(value.velocity[0].values[0] === -24.125);
check(value.torque.length == 1);
check(value.torque[0].group === "v904cβ");
check(value.torque[0].values.length == 1);
check(value.torque[0].values[0] === -145.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_36(){const input=bytes("ae4babc2070000007639303237ceb2"),codec=r.Ros1Uwvm_fourier_bridgeSetGroupCommandResponseCodec,value=codec.decode(input);
check(value.status === 3266005934);
check(value.error === "v9027β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_37(){const input=bytes("e071229f5908150101000000070000007637373634ceb2010000000000000000801dc001000000070000007635393231ceb2010000000000000000a049c0"),codec=r.Ros1Uwvm_fourier_bridgeSetMotorCfgPdRequestCodec,value=codec.decode(input);
check(value.sequence === 77977749563797984n);
check(value.kp.length == 1);
check(value.kp[0].group === "v7764β");
check(value.kp[0].values.length == 1);
check(value.kp[0].values[0] === -7.375);
check(value.kd.length == 1);
check(value.kd[0].group === "v5921β");
check(value.kd[0].values.length == 1);
check(value.kd[0].values[0] === -51.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_38(){const input=bytes("4bd45760070000007630626339ceb2"),codec=r.Ros1Uwvm_fourier_bridgeSetMotorCfgPdResponseCodec,value=codec.decode(input);
check(value.status === 1616368715);
check(value.error === "v0bc9β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_39(){const input=bytes("5eacbc86e7d58b180000000000c034c00000000000802e400000000000ac6440"),codec=r.Ros1Uwvm_fourier_bridgeSetStandPoseRequestCodec,value=codec.decode(input);
check(value.sequence === 1768742469047725150n);
check(value.delta_z === -20.75);
check(value.delta_pitch === 15.25);
check(value.delta_yaw === 165.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_40(){const input=bytes("dd1bbc92070000007638323036ceb2"),codec=r.Ros1Uwvm_fourier_bridgeSetStandPoseResponseCodec,value=codec.decode(input);
check(value.status === 2461801437);
check(value.error === "v8206β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_41(){const input=bytes("acb4542e15cd63b0b60565ff"),codec=r.Ros1Uwvm_fourier_bridgeSetUpperFsmStateRequestCodec,value=codec.decode(input);
check(value.sequence === 12710228064224982188n);
check(value.state === -10156618);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_42(){const input=bytes("70bc7b1d070000007632303366ceb2"),codec=r.Ros1Uwvm_fourier_bridgeSetUpperFsmStateResponseCodec,value=codec.decode(input);
check(value.status === 494648432);
check(value.error === "v203fβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_43(){const input=bytes("46b99d6fffc0aaf10000000000f466c00000000000bc6cc00000000000386bc00000000000446840"),codec=r.Ros1Uwvm_fourier_bridgeSetVelocityRequestCodec,value=codec.decode(input);
check(value.sequence === 17413943112503245126n);
check(value.vx === -183.625);
check(value.vy === -229.875);
check(value.yaw === -217.75);
check(value.duration === 194.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_44(){const input=bytes("db360cd0070000007638306233ceb2"),codec=r.Ros1Uwvm_fourier_bridgeSetVelocityResponseCodec,value=codec.decode(input);
check(value.status === 3490461403);
check(value.error === "v80b3β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_45(){const input=bytes("94e22c732d9b1bed1314fbc0"),codec=r.Ros1Uwvm_fourier_bridgeSetVelocitySourceRequestCodec,value=codec.decode(input);
check(value.sequence === 17085420230868263572n);
check(value.source === -1057287149);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_46(){const input=bytes("8a86360d070000007662373766ceb2"),codec=r.Ros1Uwvm_fourier_bridgeSetVelocitySourceResponseCodec,value=codec.decode(input);
check(value.status === 221677194);
check(value.error === "vb77fβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_47(){const input=bytes("345117e9070000007630353035ceb2d2dcfdd7070000007638316666ceb2ce79a5b0070000007637383632ceb2010000000000000000f05bc001000000070000007639363337ceb2010000000000000000005a400100000000000000002461c00100000000000000006c67c048864a54"),codec=r.Ros1Uwvm_fourier_bridgeStatusCodec,value=codec.decode(input);
check(value.fsm_state === -384347852);
check(value.fsm_name === "v0505β");
check(value.upper_fsm_state === -671228718);
check(value.upper_fsm_name === "v81ffβ");
check(value.velocity_source === -1331332658);
check(value.velocity_source_name === "v7862β");
check(value.stand_pose.length == 1);
check(value.stand_pose[0] === -111.75);
check(value.groups.length == 1);
check(value.groups[0].group === "v9637β");
check(value.groups[0].position.length == 1);
check(value.groups[0].position[0] === 104.0);
check(value.groups[0].velocity.length == 1);
check(value.groups[0].velocity[0] === -137.125);
check(value.groups[0].effort.length == 1);
check(value.groups[0].effort[0] === -187.375);
check(value.groups[0].motion_state === 1414170184);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_48(){const input=bytes("534074feea3943600000000000dc60400000000000ec6f400000000000405ac00000000000b86ac0"),codec=r.Ros1Uwvm_fourier_bridgeVelocityCommandCodec,value=codec.decode(input);
check(value.sequence === 6936451532534857811n);
check(value.vx === 134.875);
check(value.vy === 255.375);
check(value.yaw === -105.0);
check(value.duration === -213.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_49(){const input=bytes("683d4c02d1447b9c01000000070000007636316534ceb20000000000904cc00000000000205340"),codec=r.Ros1Uwvm_fourier_bridgeWaitGroupsMotionCompleteRequestCodec,value=codec.decode(input);
check(value.sequence === 11275681756529573224n);
check(value.groups.length == 1);
check(value.groups[0] === "v61e4β");
check(value.timeout_seconds === -57.125);
check(value.poll_seconds === 76.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_50(){const input=bytes("cef36cdc070000007635363664ceb201"),codec=r.Ros1Uwvm_fourier_bridgeWaitGroupsMotionCompleteResponseCodec,value=codec.decode(input);
check(value.status === 3698127822);
check(value.error === "v566dβ");
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
