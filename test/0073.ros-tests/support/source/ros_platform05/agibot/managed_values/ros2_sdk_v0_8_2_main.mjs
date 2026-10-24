/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/agibot-ros2-sdk-v0-8-2-main-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("010000002a070000007635303531ceb2"),codec=r.Ros2Aimdk_msgsMsgAgentPropertiesCodec,value=codec.decode(input);
check(value.properties.length == 1);
check(value.properties[0].key.value === 42);
check(value.properties[0].value === "v5051β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("49070000007633356565ceb2"),codec=r.Ros2Aimdk_msgsMsgAgentPropertiesValueCodec,value=codec.decode(input);
check(value.key.value === 73);
check(value.value === "v35eeβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes("d8"),codec=r.Ros2Aimdk_msgsMsgAgentPropertyIdTypeCodec,value=codec.decode(input);
check(value.value === 216);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("9b"),codec=r.Ros2Aimdk_msgsMsgAudioVadStateTypeCodec,value=codec.decode(input);
check(value.value === 155);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("16bb4308af59a921"),codec=r.Ros2Aimdk_msgsMsgCommonRequestCodec,value=codec.decode(input);
check(value.header.stamp.sec === 138656534);
check(value.header.stamp.nanosec === 564746671);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("7e48d16d857af630efb3162656ca21b86ea99a05070000007634616561ceb2"),codec=r.Ros2Aimdk_msgsMsgCommonResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === 1842432126);
check(value.header.stamp.nanosec === 821459589);
check(value.header.code === -5178635625144339473n);
check(value.status.value === 94021998);
check(value.message === "v4aeaβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("d226915e"),codec=r.Ros2Aimdk_msgsMsgCommonStateCodec,value=codec.decode(input);
check(value.value === 1586570962);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("0e92b1f4b82af9317ff79a4fc68eadd1888295fa31fba264f00f1460"),codec=r.Ros2Aimdk_msgsMsgCommonTaskResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === -189689330);
check(value.header.stamp.nanosec === 838412984);
check(value.header.code === -3337854766421444737n);
check(value.task_id === 7251634542095991432n);
check(value.state.value === 1611927536);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("1c4abca49d92eb07070000007663306231ceb2bd7094aded41fe734e14bc34070000007666313762ceb201000000070000007665653538ceb22581536c49895700000000000c6140"),codec=r.Ros2Aimdk_msgsMsgFaceEmojiStatusCodec,value=codec.decode(input);
check(value.header.stamp.sec === -1531164132);
check(value.header.stamp.nanosec === 132879005);
check(value.header.frame_id === "vc0b1β");
check(value.header.sequence === 2912186557);
check(value.header.meas_stamp.sec === 1946042861);
check(value.header.meas_stamp.nanosec === 884741198);
check(value.e_path === "vf17bβ");
check(value.e_path_list.length == 1);
check(value.e_path_list[0] === "vee58β");
check(value.e_id === 37);
check(value.mode === 129);
check(value.priority === -1991676845);
check(value.status === 87);
check(value.time_to_end_ms === 136.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_9(){const input=bytes("070000007636623363ceb20000000000e054c00000000000b46e400000000000a037c0000000000028624000000000002c6b40"),codec=r.Ros2Aimdk_msgsMsgHandCommandCodec,value=codec.decode(input);
check(value.name === "v6b3cβ");
check(value.position === -83.5);
check(value.velocity === 245.625);
check(value.acceleration === -23.625);
check(value.deceleration === 145.25);
check(value.effort === 217.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("0a807d12feed941e070000007636373536ceb2825b730ae4d85ca71dacb439fa01000000070000007665323962ceb20000000000000ec0000000000038604000000000004c6ac00000000000946ac00000000000e068c09a01000000070000007666366562ceb20000000000905c40000000000090504000000000006469c000000000008047c000000000000c62c0"),codec=r.Ros2Aimdk_msgsMsgHandCommandArrayCodec,value=codec.decode(input);
check(value.header.stamp.sec === 310214666);
check(value.header.stamp.nanosec === 513076734);
check(value.header.frame_id === "v6756β");
check(value.header.sequence === 175332226);
check(value.header.meas_stamp.sec === -1487087388);
check(value.header.meas_stamp.nanosec === 968141853);
check(value.left_hand_type.value === 250);
check(value.left_hands.length == 1);
check(value.left_hands[0].name === "ve29bβ");
check(value.left_hands[0].position === -3.75);
check(value.left_hands[0].velocity === 129.75);
check(value.left_hands[0].acceleration === -210.375);
check(value.left_hands[0].deceleration === -212.625);
check(value.left_hands[0].effort === -199.0);
check(value.right_hand_type.value === 154);
check(value.right_hands.length == 1);
check(value.right_hands[0].name === "vf6ebβ");
check(value.right_hands[0].position === 114.25);
check(value.right_hands[0].velocity === 66.25);
check(value.right_hands[0].acceleration === -203.125);
check(value.right_hands[0].deceleration === -47.0);
check(value.right_hands[0].effort === -144.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes("070000007663636334ceb20000000000f8574000000000000c614000000000009c6840f0e70c745ba49b55"),codec=r.Ros2Aimdk_msgsMsgHandStateCodec,value=codec.decode(input);
check(value.name === "vccc4β");
check(value.position === 95.875);
check(value.velocity === 136.375);
check(value.effort === 196.875);
check(value.state === 1947002864);
check(value.faultcode === 1436263515);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("153e6a78fd2f7135070000007663306338ceb293b6edadc6d73a3e54d7710bf901000000070000007630333138ceb20000000000e03b400000000000b04ec00000000000346240434ccfb8b8fd74fdb801000000070000007631383034ceb20000000000286e4000000000000017c00000000000b064401dfed4b2d38d9f59"),codec=r.Ros2Aimdk_msgsMsgHandStateArrayCodec,value=codec.decode(input);
check(value.header.stamp.sec === 2020228629);
check(value.header.stamp.nanosec === 896610301);
check(value.header.frame_id === "vc0c8β");
check(value.header.sequence === 2918037139);
check(value.header.meas_stamp.sec === 1044043718);
check(value.header.meas_stamp.nanosec === 192010068);
check(value.left_hand_type.value === 249);
check(value.left_hands.length == 1);
check(value.left_hands[0].name === "v0318β");
check(value.left_hands[0].position === 27.875);
check(value.left_hands[0].velocity === -61.375);
check(value.left_hands[0].effort === 145.625);
check(value.left_hands[0].state === -1194374077);
check(value.left_hands[0].faultcode === -42664520);
check(value.right_hand_type.value === 184);
check(value.right_hands.length == 1);
check(value.right_hands[0].name === "v1804β");
check(value.right_hands[0].position === 241.25);
check(value.right_hands[0].velocity === -5.75);
check(value.right_hands[0].effort === 165.5);
check(value.right_hands[0].state === -1294664163);
check(value.right_hands[0].faultcode === 1503628755);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes("25"),codec=r.Ros2Aimdk_msgsMsgHandTypeCodec,value=codec.decode(input);
check(value.value === 37);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("070000007666396635ceb2000000000030644000000000006059c000000000006856400000000000b4654000000000003043c0"),codec=r.Ros2Aimdk_msgsMsgJointCommandCodec,value=codec.decode(input);
check(value.name === "vf9f5β");
check(value.position === 161.5);
check(value.velocity === -101.5);
check(value.effort === 89.625);
check(value.stiffness === 173.625);
check(value.damping === -38.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("c4e2ccfa36661122070000007636656530ceb2c5a6c503155c6d32fa7a343001000000070000007631646364ceb200000000002855c00000000000fc62400000000000886dc0000000000028644000000000004866c0"),codec=r.Ros2Aimdk_msgsMsgJointCommandArrayCodec,value=codec.decode(input);
check(value.header.stamp.sec === -87235900);
check(value.header.stamp.nanosec === 571565622);
check(value.header.frame_id === "v6ee0β");
check(value.header.sequence === 63284933);
check(value.header.meas_stamp.sec === 846027797);
check(value.header.meas_stamp.nanosec === 808745722);
check(value.joints.length == 1);
check(value.joints[0].name === "v1dcdβ");
check(value.joints[0].position === -84.625);
check(value.joints[0].velocity === 151.875);
check(value.joints[0].effort === -236.25);
check(value.joints[0].stiffness === 161.25);
check(value.joints[0].damping === -178.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_16(){const input=bytes("070000007634386539ceb200000000008041c00000000000bc64400000000000bc6540bfcbe3"),codec=r.Ros2Aimdk_msgsMsgJointStateCodec,value=codec.decode(input);
check(value.name === "v48e9β");
check(value.position === -35.0);
check(value.velocity === 165.875);
check(value.effort === 173.875);
check(value.coil_temp === 191);
check(value.motor_temp === 203);
check(value.motor_vol === 227);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("1952db78f8dfe310070000007637316134ceb293ba811c5be6a64ea09f3a2a01000000070000007638333532ceb20000000000c855c000000000003867400000000000c038c05a2c06"),codec=r.Ros2Aimdk_msgsMsgJointStateArrayCodec,value=codec.decode(input);
check(value.header.stamp.sec === 2027639321);
check(value.header.stamp.nanosec === 283369464);
check(value.header.frame_id === "v71a4β");
check(value.header.sequence === 478263955);
check(value.header.meas_stamp.sec === 1319560795);
check(value.header.meas_stamp.nanosec === 708485024);
check(value.joints.length == 1);
check(value.joints[0].name === "v8352β");
check(value.joints[0].position === -87.125);
check(value.joints[0].velocity === 185.75);
check(value.joints[0].effort === -24.75);
check(value.joints[0].coil_temp === 90);
check(value.joints[0].motor_temp === 44);
check(value.joints[0].motor_vol === 6);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes("9e616d45"),codec=r.Ros2Aimdk_msgsMsgMcActionCodec,value=codec.decode(input);
check(value.value === 1164796318);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_19(){const input=bytes("6ff83014070000007633656562ceb2"),codec=r.Ros2Aimdk_msgsMsgMcActionCommandCodec,value=codec.decode(input);
check(value.action.value === 338753647);
check(value.action_desc === "v3eebβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_20(){const input=bytes("091b656e070000007630343534ceb2543effca"),codec=r.Ros2Aimdk_msgsMsgMcActionInfoCodec,value=codec.decode(input);
check(value.current_action.value === 1852119817);
check(value.action_desc === "v0454β");
check(value.status.value === -889242028);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_21(){const input=bytes("03a0131b"),codec=r.Ros2Aimdk_msgsMsgMcActionStatusCodec,value=codec.decode(input);
check(value.value === 454270979);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_22(){const input=bytes("c61a340d"),codec=r.Ros2Aimdk_msgsMsgMcControlAreaCodec,value=codec.decode(input);
check(value.value === 221518534);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_23(){const input=bytes("64436340"),codec=r.Ros2Aimdk_msgsMsgMcInputActionCodec,value=codec.decode(input);
check(value.value === 1080247140);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_24(){const input=bytes("070000007664636264ceb2d4e7a260518ce9e8"),codec=r.Ros2Aimdk_msgsMsgMcInputSourceCodec,value=codec.decode(input);
check(value.name === "vdcbdβ");
check(value.priority === 1621288916);
check(value.timeout === -387347375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_25(){const input=bytes("1da7c0703c8e991c070000007630373663ceb2de93496a976034d2db683806070000007630363634ceb200000000000c69c0000000000038634000000000003064c0"),codec=r.Ros2Aimdk_msgsMsgMcLocomotionVelocityCodec,value=codec.decode(input);
check(value.header.stamp.sec === 1891673885);
check(value.header.stamp.nanosec === 479825468);
check(value.header.frame_id === "v076cβ");
check(value.header.sequence === 1783206878);
check(value.header.meas_stamp.sec === -768319337);
check(value.header.meas_stamp.nanosec === 104360155);
check(value.source === "v0664β");
check(value.forward_velocity === -200.375);
check(value.lateral_velocity === 153.75);
check(value.angular_velocity === -161.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_26(){const input=bytes("bbd1e14c"),codec=r.Ros2Aimdk_msgsMsgMcPresetMotionCodec,value=codec.decode(input);
check(value.value === 1289867707);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_27(){const input=bytes("5455aec10663d513070000007636323737ceb2e5fe520f50fd2f14d8db8710"),codec=r.Ros2Aimdk_msgsMsgMessageHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -1045539500);
check(value.stamp.nanosec === 332751622);
check(value.frame_id === "v6277β");
check(value.sequence === 257097445);
check(value.meas_stamp.sec === 338689360);
check(value.meas_stamp.nanosec === 277339096);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_28(){const input=bytes("070000007635663937ceb2f6cf1cd9128e891980070000007666626365ceb2070000007666656536ceb201"),codec=r.Ros2Aimdk_msgsMsgPlayMediaFileRequestCodec,value=codec.decode(input);
check(value.file_name === "v5f97β");
check(value.sample_rate === 3642544118);
check(value.priority_level.value === 18);
check(value.priority_weight === 2149157262);
check(value.domain === "vfbceβ");
check(value.trace_id === "vfee6β");
check(value.is_interrupted === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_29(){const input=bytes("070000007637643533ceb29905780d6a070000007634396232ceb2070000007632646662ceb201"),codec=r.Ros2Aimdk_msgsMsgPlayTtsRequestCodec,value=codec.decode(input);
check(value.text === "v7d53β");
check(value.priority_level.value === 153);
check(value.priority_weight === 1779267589);
check(value.domain === "v49b2β");
check(value.trace_id === "v2dfbβ");
check(value.is_interrupted === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_30(){const input=bytes("070000007639376439ceb22d13a7109d070000007636333533ceb2070000007665306536ceb201070000007637663738ceb20f38fdc7"),codec=r.Ros2Aimdk_msgsMsgPlayTtsResponseCodec,value=codec.decode(input);
check(value.text === "v97d9β");
check(value.priority_level.value === 45);
check(value.priority_weight === 2635114259);
check(value.domain === "v6353β");
check(value.trace_id === "ve0e6β");
check(value.is_success === true);
check(value.error_message === "v7f78β");
check(value.estimated_duration === 3355260943);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_31(){const input=bytes("070000007664623661ceb2070000007634316663ceb2070000007663383133ceb2a9e007c500000000008c6640000000000048584000000000003859c000000000002c63c00000000000203a4000000000004050c000000000004060c00000000000806cc00000000000cc644000000000003858400000000000504a40000000000048614000000000009c6b400000000000485bc000000000003c68c0070000007665386262ceb2070000007631333430ceb2070000007636353838ceb2070000007666663165ceb2f957209c893c0000000000685140000000000070404000000000007053400000000000b857403de21c6435561304202a24"),codec=r.Ros2Aimdk_msgsMsgPmuStateCodec,value=codec.decode(input);
check(value.pmu_software_version === "vdb6aβ");
check(value.pmu_hardware_version === "v41fcβ");
check(value.pmu_protocol_version === "vc813β");
check(value.pmu_bool_status === 3305627817);
check(value.head_power_current === 180.375);
check(value.output_48v_current === 97.125);
check(value.rk3588_current === -100.875);
check(value.output_12v_current === -153.375);
check(value.bus_48v_current === 26.125);
check(value.orin_current === -65.0);
check(value.bus_48v_pmos_voltage === -130.0);
check(value.battery_voltage === -228.0);
check(value.fan_voltage === 166.375);
check(value.output_12v_voltage === 96.875);
check(value.output_48v_voltage === 52.625);
check(value.bus_48v_voltage === 138.25);
check(value.head_power_voltage === 220.875);
check(value.orin_voltage === -109.125);
check(value.rk3588_voltage === -193.875);
check(value.bms_manufacturer === "ve8bbβ");
check(value.bms_serial_number === "v1340β");
check(value.bms_hardware_version === "v6588β");
check(value.bms_software_version === "vff1eβ");
check(value.bms_status_bits === 2619365369);
check(value.battery_balance_line_resistance === 15497);
check(value.battery_pack_voltage === 69.625);
check(value.battery_current === 32.875);
check(value.battery_output_power === 77.75);
check(value.battery_temperature === 94.875);
check(value.battery_remaining_capacity === 1679614525);
check(value.battery_remaining_capacity_percentage === 53);
check(value.battery_cycle_count === 4950);
check(value.battery_cycle_total_capacity === 606740484);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_32(){const input=bytes("74f6ac96d49fda04070000007630383631ceb291fb4465427b4e7c7214af30d8042faf470100000002"),codec=r.Ros2Aimdk_msgsMsgProcessedAudioOutputCodec,value=codec.decode(input);
check(value.header.stamp.sec === -1767049612);
check(value.header.stamp.nanosec === 81436628);
check(value.header.frame_id === "v0861β");
check(value.header.sequence === 1699019665);
check(value.header.meas_stamp.sec === 2085518146);
check(value.header.meas_stamp.nanosec === 816780402);
check(value.stream_id === 2939094232);
check(value.audio_vad_state.value === 71);
check(value.audio_data.length == 1);
check(value.audio_data[0] === 2);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_33(){const input=bytes("7ec27b1a2f62b81e"),codec=r.Ros2Aimdk_msgsMsgRequestHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === 444318334);
check(value.stamp.nanosec === 515400239);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_34(){const input=bytes("55c42dd4c7edd33a863659073f4f6e99"),codec=r.Ros2Aimdk_msgsMsgResponseHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -735198123);
check(value.stamp.nanosec === 986967495);
check(value.code === -7390882806343584122n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_35(){const input=bytes("d7825fed79ab2507070000007634613365ceb25bf61b277bc4ddd4778e3f220152585ca1136947b8d03a6a93910b718a569d30c517ac2bdcd4ff06f795ce1deed3bcbd4e928da65751de8b7c10ef9065d779d12a9648ca33551be718142afc010100010001000100"),codec=r.Ros2Aimdk_msgsMsgTouchStateCodec,value=codec.decode(input);
check(value.header.stamp.sec === -312507689);
check(value.header.stamp.nanosec === 119909241);
check(value.header.frame_id === "v4a3eβ");
check(value.header.sequence === 656143963);
check(value.header.meas_stamp.sec === -723663749);
check(value.header.meas_stamp.nanosec === 574590583);
check(value.event_type === 1);
check(value.data.length == 8);
check(value.data[0] === 2707183698);
check(value.data[1] === 3091687699);
check(value.data[2] === 2473212624);
check(value.data[3] === 2322664337);
check(value.data[4] === 3308297558);
check(value.data[5] === 3693849623);
check(value.data[6] === 4144431060);
check(value.data[7] === 3994930837);
check(value.threshold.length == 8);
check(value.threshold[0] === 1321057491);
check(value.threshold[1] === 1470533010);
check(value.threshold[2] === 2089541201);
check(value.threshold[3] === 1703997200);
check(value.threshold[4] === 718371287);
check(value.threshold[5] === 868894870);
check(value.threshold[6] === 417798997);
check(value.threshold[7] === 33303060);
check(value.is_touched.length == 8);
check(value.is_touched[0] === true);
check(value.is_touched[1] === false);
check(value.is_touched[2] === true);
check(value.is_touched[3] === false);
check(value.is_touched[4] === true);
check(value.is_touched[5] === false);
check(value.is_touched[6] === true);
check(value.is_touched[7] === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_36(){const input=bytes("bc"),codec=r.Ros2Aimdk_msgsMsgTtsPriorityLevelCodec,value=codec.decode(input);
check(value.value === 188);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_37(){const input=bytes("42176be292645b22"),codec=r.Ros2Aimdk_msgsSrvGetAllJointState_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -496298174);
check(value.request.header.stamp.nanosec === 576414866);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_38(){const input=bytes("cbb1e60319f8d70d158016abacf921355fb27f58070000007635353831ceb201000000070000007630323635ceb20000000000205ac000000000008018c0000000000090504080cddc01000000070000007634366630ceb20000000000e060c000000000001859400000000000c466c0a9abf501000000070000007665366135ceb200000000002064c000000000001054c0000000000020464078b92401000000070000007637363262ceb20000000000a8544000000000004057400000000000d05e40c20f9e"),codec=r.Ros2Aimdk_msgsSrvGetAllJointState_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === 65450443);
check(value.reponse.header.stamp.nanosec === 232257561);
check(value.reponse.header.code === 3828615678241701909n);
check(value.reponse.status.value === 1484763743);
check(value.reponse.message === "v5581β");
check(value.head_joints.length == 1);
check(value.head_joints[0].name === "v0265β");
check(value.head_joints[0].position === -104.5);
check(value.head_joints[0].velocity === -6.125);
check(value.head_joints[0].effort === 66.25);
check(value.head_joints[0].coil_temp === 128);
check(value.head_joints[0].motor_temp === 205);
check(value.head_joints[0].motor_vol === 220);
check(value.arm_joints.length == 1);
check(value.arm_joints[0].name === "v46f0β");
check(value.arm_joints[0].position === -135.0);
check(value.arm_joints[0].velocity === 100.375);
check(value.arm_joints[0].effort === -182.125);
check(value.arm_joints[0].coil_temp === 169);
check(value.arm_joints[0].motor_temp === 171);
check(value.arm_joints[0].motor_vol === 245);
check(value.waist_joints.length == 1);
check(value.waist_joints[0].name === "ve6a5β");
check(value.waist_joints[0].position === -161.0);
check(value.waist_joints[0].velocity === -80.25);
check(value.waist_joints[0].effort === 44.25);
check(value.waist_joints[0].coil_temp === 120);
check(value.waist_joints[0].motor_temp === 185);
check(value.waist_joints[0].motor_vol === 36);
check(value.leg_joints.length == 1);
check(value.leg_joints[0].name === "v762bβ");
check(value.leg_joints[0].position === 82.625);
check(value.leg_joints[0].velocity === 93.0);
check(value.leg_joints[0].effort === 123.25);
check(value.leg_joints[0].coil_temp === 194);
check(value.leg_joints[0].motor_temp === 15);
check(value.leg_joints[0].motor_vol === 158);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_39(){const input=bytes("356cda9cc4940907"),codec=r.Ros2Aimdk_msgsSrvGetCurrentInputSource_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -1663407051);
check(value.request.header.stamp.nanosec === 118068420);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_40(){const input=bytes("8f9e7c10e0656a2587abc65d3ed2f1c340b51964f9cc2efa08534872070000007666656432ceb2d91539cd3bc79314"),codec=r.Ros2Aimdk_msgsSrvGetCurrentInputSource_ResponseCodec,value=codec.decode(input);
check(value.response.header.stamp.sec === 276602511);
check(value.response.header.stamp.nanosec === 627729888);
check(value.response.header.code === -4327446601623229561n);
check(value.response.task_id === 18027571729909003584n);
check(value.response.state.value === 1917342472);
check(value.input_source.name === "vfed2β");
check(value.input_source.priority === -851896871);
check(value.input_source.timeout === 345229115);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_41(){const input=bytes("216fa6ac032ce70e"),codec=r.Ros2Aimdk_msgsSrvGetHandType_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -1398378719);
check(value.request.header.stamp.nanosec === 250031107);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_42(){const input=bytes("514c13d76fc2151cd7889bf26ef1ac6ccb9c4c2d070000007638303934ceb2a04f"),codec=r.Ros2Aimdk_msgsSrvGetHandType_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -686601135);
check(value.reponse.header.stamp.nanosec === 471188079);
check(value.reponse.header.code === 7830899310909425879n);
check(value.reponse.status.value === 759995595);
check(value.reponse.message === "v8094β");
check(value.left_hands_type.value === 160);
check(value.right_hands_type.value === 79);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_43(){const input=bytes("16b2cbc742412923"),codec=r.Ros2Aimdk_msgsSrvGetMcAction_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -942951914);
check(value.request.header.stamp.nanosec === 589906242);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_44(){const input=bytes("371ceb02ee57961fe9cb311a50b20684cb955833070000007630626164ceb2d172243e"),codec=r.Ros2Aimdk_msgsSrvGetMcAction_ResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === 48962615);
check(value.header.stamp.nanosec === 529946606);
check(value.header.code === -8933256753736201239n);
check(value.info.current_action.value === 861443531);
check(value.info.action_desc === "v0badβ");
check(value.info.status.value === 1042576081);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_45(){const input=bytes("40cabdd47aeddb2e"),codec=r.Ros2Aimdk_msgsSrvGetMute_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -725759424);
check(value.request.header.stamp.nanosec === 786165114);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_46(){const input=bytes("3a06deedcacac334f4fabbbf4d838c21cbcd668f070000007630326338ceb201"),codec=r.Ros2Aimdk_msgsSrvGetMute_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -304216518);
check(value.reponse.header.stamp.nanosec === 885246666);
check(value.reponse.header.code === 2417451469943601908n);
check(value.reponse.status.value === -1889088053);
check(value.reponse.message === "v02c8β");
check(value.is_mute === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_47(){const input=bytes("3482754f43b7261d"),codec=r.Ros2Aimdk_msgsSrvGetVolume_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === 1333101108);
check(value.request.header.stamp.nanosec === 489076547);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_48(){const input=bytes("577b2dba3e2c9f05138b111baaf226856110acb1070000007638363464ceb2a22c91c9"),codec=r.Ros2Aimdk_msgsSrvGetVolume_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -1171424425);
check(value.reponse.header.stamp.nanosec === 94317630);
check(value.reponse.header.code === -8852121205137634541n);
check(value.reponse.status.value === -1314123679);
check(value.reponse.message === "v864dβ");
check(value.audio_volume === 3381734562);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_49(){const input=bytes("9a8f2cad6fd34d14c2907bf4"),codec=r.Ros2Aimdk_msgsSrvLedStripCommand_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -1389588582);
check(value.request.header.stamp.nanosec === 340644719);
check(value.led_strip_mode === 194);
check(value.r === 144);
check(value.g === 123);
check(value.b === 244);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_50(){const input=bytes("8a74dbe32c035c37f42d67754d5450eb2b4a"),codec=r.Ros2Aimdk_msgsSrvLedStripCommand_ResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === -472157046);
check(value.header.stamp.nanosec === 928777004);
check(value.header.code === -1490598785000722956n);
check(value.status_code === 18987);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_51(){const input=bytes("fb206e27ed8a9b1302cbe66ca555"),codec=r.Ros2Aimdk_msgsSrvPlayEmoji_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 661528827);
check(value.header.header.stamp.nanosec === 328960749);
check(value.emotion_id === 2);
check(value.mode === 203);
check(value.priority === 1436904678);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_52(){const input=bytes("f6c43d030ac08f040abdee5cb3c4d9c2974780d5070000007635323838ceb201070000007632373339ceb2"),codec=r.Ros2Aimdk_msgsSrvPlayEmoji_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 54379766);
check(value.header.header.stamp.nanosec === 76529674);
check(value.header.header.code === -4406274485767979766n);
check(value.header.status.value === -713013353);
check(value.header.message === "v5288β");
check(value.success === true);
check(value.message === "v2739β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_53(){const input=bytes("02d555892c399c1c070000007638313562ceb26af2beda7d802e045e070000007663636532ceb2070000007638396235ceb200"),codec=r.Ros2Aimdk_msgsSrvPlayMediaFile_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -1990863614);
check(value.header.header.stamp.nanosec === 480000300);
check(value.media_file_req.file_name === "v815bβ");
check(value.media_file_req.sample_rate === 3669946986);
check(value.media_file_req.priority_level.value === 125);
check(value.media_file_req.priority_weight === 1577332352);
check(value.media_file_req.domain === "vcce2β");
check(value.media_file_req.trace_id === "v89b5β");
check(value.media_file_req.is_interrupted === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_54(){const input=bytes("bbb5f2c7a587ad20b04b719c09324602f382f4fe070000007666336166ceb2070000007633303462ceb299f05c1b6c070000007638316232ceb2070000007634646464ceb201070000007630363336ceb2f9b7c30c"),codec=r.Ros2Aimdk_msgsSrvPlayMediaFile_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -940395077);
check(value.header.header.stamp.nanosec === 548243365);
check(value.header.header.code === 163873453306366896n);
check(value.header.status.value === -17530125);
check(value.header.message === "vf3afβ");
check(value.tts_resp.text === "v304bβ");
check(value.tts_resp.priority_level.value === 153);
check(value.tts_resp.priority_weight === 1813732592);
check(value.tts_resp.domain === "v81b2β");
check(value.tts_resp.trace_id === "v4dddβ");
check(value.tts_resp.is_success === true);
check(value.tts_resp.error_message === "v0636β");
check(value.tts_resp.estimated_duration === 214153209);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_55(){const input=bytes("16fddc490ed7d11b070000007639383961ceb213d9d1d86c070000007637343130ceb2070000007634343863ceb200"),codec=r.Ros2Aimdk_msgsSrvPlayTts_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 1239219478);
check(value.header.header.stamp.nanosec === 466736910);
check(value.tts_req.text === "v989aβ");
check(value.tts_req.priority_level.value === 19);
check(value.tts_req.priority_weight === 1826148825);
check(value.tts_req.domain === "v7410β");
check(value.tts_req.trace_id === "v448cβ");
check(value.tts_req.is_interrupted === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_56(){const input=bytes("eee8e8dd779080066fe0aca5d6999b3b1d90a1e8070000007636303330ceb2070000007661363062ceb2fad6e30077070000007662653934ceb2070000007639336335ceb201070000007663366266ceb22bf63bd9"),codec=r.Ros2Aimdk_msgsSrvPlayTts_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -571938578);
check(value.header.header.stamp.nanosec === 109088887);
check(value.header.header.code === 4295195816809521263n);
check(value.header.status.value === -392064995);
check(value.header.message === "v6030β");
check(value.tts_resp.text === "va60bβ");
check(value.tts_resp.priority_level.value === 250);
check(value.tts_resp.priority_weight === 1996547030);
check(value.tts_resp.domain === "vbe94β");
check(value.tts_resp.trace_id === "v93c5β");
check(value.tts_resp.is_success === true);
check(value.tts_resp.error_message === "vc6bfβ");
check(value.tts_resp.estimated_duration === 3644585515);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_57(){const input=bytes("23679b0a535df030070000007639653337ceb2d417bc5be7"),codec=r.Ros2Aimdk_msgsSrvPlayVideo_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 177956643);
check(value.header.header.stamp.nanosec === 821058899);
check(value.video_path === "v9e37β");
check(value.mode === 212);
check(value.priority === -413418473);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_58(){const input=bytes("576b188bc2210c1c3d435581843a621f6659e06d070000007663326330ceb200070000007639356337ceb2"),codec=r.Ros2Aimdk_msgsSrvPlayVideo_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -1961333929);
check(value.header.header.stamp.nanosec === 470557122);
check(value.header.header.code === 2261434303673353021n);
check(value.header.status.value === 1843419494);
check(value.header.message === "vc2c0β");
check(value.success === false);
check(value.message === "v95c7β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_59(){const input=bytes("38249d9b00ba66140100000035070000007635636336ceb2"),codec=r.Ros2Aimdk_msgsSrvSetAgentPropertiesRequest_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -1684200392);
check(value.header.header.stamp.nanosec === 342276608);
check(value.contents.properties.length == 1);
check(value.contents.properties[0].key.value === 53);
check(value.contents.properties[0].value === "v5cc6β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_60(){const input=bytes("f8a4ec016be9aa2e2f5836a996210137354b719e070000007661393561ceb2"),codec=r.Ros2Aimdk_msgsSrvSetAgentPropertiesRequest_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 32285944);
check(value.header.header.stamp.nanosec === 782952811);
check(value.header.header.code === 3963486078030469167n);
check(value.header.status.value === -1636742347);
check(value.header.message === "va95aβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_61(){const input=bytes("7d1b67589f966f04bba8a149070000007663376130ceb2"),codec=r.Ros2Aimdk_msgsSrvSetMcAction_RequestCodec,value=codec.decode(input);
check(value.header.stamp.sec === 1483152253);
check(value.header.stamp.nanosec === 74421919);
check(value.command.action.value === 1235331259);
check(value.command.action_desc === "vc7a0β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_62(){const input=bytes("afba180a3b3980022cc8591f95b16e811f75ce07070000007664656439ceb2"),codec=r.Ros2Aimdk_msgsSrvSetMcAction_ResponseCodec,value=codec.decode(input);
check(value.response.header.stamp.sec === 169392815);
check(value.response.header.stamp.nanosec === 41957691);
check(value.response.header.code === -9120156941344454612n);
check(value.response.status.value === 130970911);
check(value.response.message === "vded9β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_63(){const input=bytes("e1dc1ed564ff8c3919eb0151070000007630386639ceb220845fcc7a3873a9"),codec=r.Ros2Aimdk_msgsSrvSetMcInputSource_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -719397663);
check(value.request.header.stamp.nanosec === 965541732);
check(value.action.value === 1359080217);
check(value.input_source.name === "v08f9β");
check(value.input_source.priority === -866155488);
check(value.input_source.timeout === -1452066694);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_64(){const input=bytes("dc64493ed322d40688507bf331294c6d83b456d53acd614b07a8f5dc"),codec=r.Ros2Aimdk_msgsSrvSetMcInputSource_ResponseCodec,value=codec.decode(input);
check(value.response.header.stamp.sec === 1044997340);
check(value.response.header.stamp.nanosec === 114565843);
check(value.response.header.code === 7875715142879236232n);
check(value.response.task_id === 5431848278156555395n);
check(value.response.state.value === -587880441);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_65(){const input=bytes("dfe0f0a55c978a2517dd2cabccdb867500070000007632633961ceb2"),codec=r.Ros2Aimdk_msgsSrvSetMcPresetMotion_RequestCodec,value=codec.decode(input);
check(value.header.stamp.sec === -1510940449);
check(value.header.stamp.nanosec === 629839708);
check(value.area.value === -1423123177);
check(value.motion.value === 1971772364);
check(value.interrupt === false);
check(value.ani_path === "v2c9aβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_66(){const input=bytes("f5dbfa9364cdbe0af9a4971d40dda08331ebc0438892f7dd765c1932"),codec=r.Ros2Aimdk_msgsSrvSetMcPresetMotion_ResponseCodec,value=codec.decode(input);
check(value.response.header.stamp.sec === -1812276235);
check(value.response.header.stamp.nanosec === 180276580);
check(value.response.header.code === -8961919991023164167n);
check(value.response.task_id === 15994413715579530033n);
check(value.response.state.value === 840522870);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_67(){const input=bytes("b2a0f11c2b0f5a1701"),codec=r.Ros2Aimdk_msgsSrvSetMute_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === 485597362);
check(value.request.header.stamp.nanosec === 391778091);
check(value.is_mute === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_68(){const input=bytes("60d9a5be72edec0c49f2bc65f08b8bfbf2056c3a070000007634653835ceb201"),codec=r.Ros2Aimdk_msgsSrvSetMute_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -1096427168);
check(value.reponse.header.stamp.nanosec === 216853874);
check(value.reponse.header.code === -321009083811564983n);
check(value.reponse.status.value === 980157938);
check(value.reponse.message === "v4e85β");
check(value.is_mute === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_69(){const input=bytes("014198c4c0f25f164b1e246a"),codec=r.Ros2Aimdk_msgsSrvSetVolume_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -996654847);
check(value.request.header.stamp.nanosec === 375386816);
check(value.audio_volume === 1780751947);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_70(){const input=bytes("b75215ec53a04b19d309a1446a7096da5362972a070000007665623661ceb2836cf7ed"),codec=r.Ros2Aimdk_msgsSrvSetVolume_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -334146889);
check(value.reponse.header.stamp.nanosec === 424386643);
check(value.reponse.header.code === -2695843725214414381n);
check(value.reponse.status.value === 714564179);
check(value.reponse.message === "veb6aβ");
check(value.audio_volume === 3992415363);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_71(){const input=bytes("072b89b550057009"),codec=r.Ros2Builtin_interfacesMsgTimeCodec,value=codec.decode(input);
check(value.sec === -1249301753);
check(value.nanosec === 158336336);
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
test_59();
test_60();
test_61();
test_62();
test_63();
test_64();
test_65();
test_66();
test_67();
test_68();
test_69();
test_70();
test_71();
return 0;
}
