/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/agibot-ros2-sdk-v1-0-0-main-complete/bounded_records.mjs";
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

function test_3(){const input=bytes("faab1a32f0ad180b1d139792ef272e464263b2070000007631353961ceb2070000007638343839ceb20100000028070000007665373430ceb2"),codec=r.Ros2Aimdk_msgsMsgAudioCaptureCodec,value=codec.decode(input);
check(value.stamps.sec === 840608762);
check(value.stamps.nanosec === 186166768);
check(value.mic_channels === 29);
check(value.ref_channels === 19);
check(value.info.channels === 151);
check(value.info.sample_rate === 774369170);
check(value.info.size === 2992849478);
check(value.info.sample_format === "v159aβ");
check(value.info.coding_format === "v8489β");
check(value.data.data.length == 1);
check(value.data.data[0] === 40);
check(value.pkg_name === "ve740β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("010000005c"),codec=r.Ros2Aimdk_msgsMsgAudioDataCodec,value=codec.decode(input);
check(value.data.length == 1);
check(value.data[0] === 92);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("070000007661306563ceb2070000007663653833ceb2070000007639626634ceb22eaeaca99336f34573070000007638396263ceb2070000007631386166ceb2b1005c5235131be4"),codec=r.Ros2Aimdk_msgsMsgAudioFileCodec,value=codec.decode(input);
check(value.pkg_name === "va0ecβ");
check(value.file_name === "vce83β");
check(value.file_path === "v9bf4β");
check(value.info.channels === 46);
check(value.info.sample_rate === 2477370542);
check(value.info.size === 1933964086);
check(value.info.sample_format === "v89bcβ");
check(value.info.coding_format === "v18afβ");
check(value.priority === 1381761201);
check(value.priority_weight === 3826979637);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("72306fde431a9c97e9070000007636343133ceb2070000007666353030ceb2"),codec=r.Ros2Aimdk_msgsMsgAudioInfoCodec,value=codec.decode(input);
check(value.channels === 114);
check(value.sample_rate === 1138650928);
check(value.size === 3919027226);
check(value.sample_format === "v6413β");
check(value.coding_format === "vf500β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("dea399c93dfe302fc2976617c4190327e5070000007666633839ceb2070000007636643961ceb2010000003f070000007663626234ceb2070000007631613763ceb2"),codec=r.Ros2Aimdk_msgsMsgAudioPlaybackCodec,value=codec.decode(input);
check(value.stamps.sec === -912677922);
check(value.stamps.nanosec === 791739965);
check(value.info.channels === 194);
check(value.info.sample_rate === 3289867927);
check(value.info.size === 3844539161);
check(value.info.sample_format === "vfc89β");
check(value.info.coding_format === "v6d9aβ");
check(value.data.data.length == 1);
check(value.data.data[0] === 63);
check(value.pkg_name === "vcbb4β");
check(value.token_id === "v1a7cβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("9b"),codec=r.Ros2Aimdk_msgsMsgAudioVadStateTypeCodec,value=codec.decode(input);
check(value.value === 155);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_9(){const input=bytes("16bb4308af59a921"),codec=r.Ros2Aimdk_msgsMsgCommonRequestCodec,value=codec.decode(input);
check(value.header.stamp.sec === 138656534);
check(value.header.stamp.nanosec === 564746671);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("7e48d16d857af630efb3162656ca21b86ea99a05070000007634616561ceb2"),codec=r.Ros2Aimdk_msgsMsgCommonResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === 1842432126);
check(value.header.stamp.nanosec === 821459589);
check(value.header.code === -5178635625144339473n);
check(value.status.value === 94021998);
check(value.message === "v4aeaβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes("d226915e"),codec=r.Ros2Aimdk_msgsMsgCommonStateCodec,value=codec.decode(input);
check(value.value === 1586570962);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("0e92b1f4b82af9317ff79a4fc68eadd1888295fa31fba264f00f1460"),codec=r.Ros2Aimdk_msgsMsgCommonTaskResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === -189689330);
check(value.header.stamp.nanosec === 838412984);
check(value.header.code === -3337854766421444737n);
check(value.task_id === 7251634542095991432n);
check(value.state.value === 1611927536);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes("070000007666373438ceb2070000007637333563ceb201000000070000007635663631ceb28fd4245f36ad13c1"),codec=r.Ros2Aimdk_msgsMsgCurrentVersionCodec,value=codec.decode(input);
check(value.version === "vf748β");
check(value.name === "v735cβ");
check(value.files.length == 1);
check(value.files[0] === "v5f61β");
check(value.download_timestamp_seconds === 13912654122913682575n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("e1"),codec=r.Ros2Aimdk_msgsMsgDomainErrorStateCodec,value=codec.decode(input);
check(value.value === 225);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("1c4abca49d92eb07070000007663306231ceb2bd7094aded41fe734e14bc34070000007666313762ceb201000000070000007665653538ceb22581536c49895700000000000c6140"),codec=r.Ros2Aimdk_msgsMsgFaceEmojiStatusCodec,value=codec.decode(input);
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

function test_16(){const input=bytes("070000007663363537ceb22668e7346a478953"),codec=r.Ros2Aimdk_msgsMsgFocusRequesterCodec,value=codec.decode(input);
check(value.pkg_name === "vc657β");
check(value.priority === 887580710);
check(value.priority_weight === 1401505642);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("070000007661373961ceb200"),codec=r.Ros2Aimdk_msgsMsgFocusResponseCodec,value=codec.decode(input);
check(value.pkg_name === "va79aβ");
check(value.focus_gain === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes("cf029c7100000000000c674000000000001c6b400000000000fc66400000000000ac6b400000000000786dc00000000000f055c00000000000fc6c4001000000b2e9220f00000000007c6bc00000000000bc6bc00000000000286d400000000000d86c400000000000dc65c000000000006c67c00000000000b8604001"),codec=r.Ros2Aimdk_msgsMsgHRPointCodec,value=codec.decode(input);
check(value.point_id === 1906049743);
check(value.pose.position.x === 184.375);
check(value.pose.position.y === 216.875);
check(value.pose.position.z === 183.875);
check(value.pose.orientation.x === 221.375);
check(value.pose.orientation.y === -235.75);
check(value.pose.orientation.z === -87.75);
check(value.pose.orientation.w === 231.875);
check(value.qr_list.length == 1);
check(value.qr_list[0].code === 253946290);
check(value.qr_list[0].pose.position.x === -219.875);
check(value.qr_list[0].pose.position.y === -221.875);
check(value.qr_list[0].pose.position.z === 233.25);
check(value.qr_list[0].pose.orientation.x === 230.75);
check(value.qr_list[0].pose.orientation.y === -174.875);
check(value.qr_list[0].pose.orientation.z === -187.375);
check(value.qr_list[0].pose.orientation.w === 133.75);
check(value.use_head === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_19(){const input=bytes("070000007636623363ceb20000000000e054c00000000000b46e400000000000a037c0000000000028624000000000002c6b40"),codec=r.Ros2Aimdk_msgsMsgHandCommandCodec,value=codec.decode(input);
check(value.name === "v6b3cβ");
check(value.position === -83.5);
check(value.velocity === 245.625);
check(value.acceleration === -23.625);
check(value.deceleration === 145.25);
check(value.effort === 217.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_20(){const input=bytes("0a807d12feed941e070000007636373536ceb2825b730ae4d85ca71dacb439fa01000000070000007665323962ceb20000000000000ec0000000000038604000000000004c6ac00000000000946ac00000000000e068c09a01000000070000007666366562ceb20000000000905c40000000000090504000000000006469c000000000008047c000000000000c62c0"),codec=r.Ros2Aimdk_msgsMsgHandCommandArrayCodec,value=codec.decode(input);
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

function test_21(){const input=bytes("070000007663636334ceb20000000000f8574000000000000c614000000000009c6840f0e70c745ba49b55"),codec=r.Ros2Aimdk_msgsMsgHandStateCodec,value=codec.decode(input);
check(value.name === "vccc4β");
check(value.position === 95.875);
check(value.velocity === 136.375);
check(value.effort === 196.875);
check(value.state === 1947002864);
check(value.faultcode === 1436263515);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_22(){const input=bytes("153e6a78fd2f7135070000007663306338ceb293b6edadc6d73a3e54d7710bf901000000070000007630333138ceb20000000000e03b400000000000b04ec00000000000346240434ccfb8b8fd74fdfebf7c3dfabb7839f6b76928ebaa6d2cefae61203071b2f33475b6f73879074685c403425312d1905716d5945b1aa2e32061a6e72465aaebfbba7938ffbe7d3cf3b2cc8d4e0fc889cb8a4908cf8e4d0cc3824001c2834405145596d7105192d31c5d8ecf0c4d8acb2263a0e12667a4e52a6b5b1ad9985f1eebaa6928efae6d2ce3a26e2fecad6a2b3071b2f33475b6f738795011d2935415b801000000070000007631383034ceb20000000000286e4000000000000017c00000000000b064401dfed4b2d38d9f5992d3105196d714559adbbcfd3e7fb8f93a7bb4f5e5a46726e1a06322edacd2935011d6973d7cbffe3978bbfa35743574b7f63170b3f23d7c6c2deeaf6829eaab64255b1ad9985f1e1e5f9cdd1a5b98d9165784c5064780c17a3bf8b97e3ffcbd723319589bda1d5cb5f43776b1f03372bdfcadec2f6ea9e898d91a5b9cdd1e5f90d1004182c30445a7e62564a3e22160afeea6e72465a2e3"),codec=r.Ros2Aimdk_msgsMsgHandStateArrayCodec,value=codec.decode(input);
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
check(value.left_touch_sensors.palm_touch_data.length == 36);
check(value.left_touch_sensors.palm_touch_data[0] === 254);
check(value.left_touch_sensors.palm_touch_data[1] === 191);
check(value.left_touch_sensors.palm_touch_data[2] === 124);
check(value.left_touch_sensors.palm_touch_data[3] === 61);
check(value.left_touch_sensors.palm_touch_data[4] === 250);
check(value.left_touch_sensors.palm_touch_data[5] === 187);
check(value.left_touch_sensors.palm_touch_data[6] === 120);
check(value.left_touch_sensors.palm_touch_data[7] === 57);
check(value.left_touch_sensors.palm_touch_data[8] === 246);
check(value.left_touch_sensors.palm_touch_data[9] === 183);
check(value.left_touch_sensors.palm_touch_data[10] === 105);
check(value.left_touch_sensors.palm_touch_data[11] === 40);
check(value.left_touch_sensors.palm_touch_data[12] === 235);
check(value.left_touch_sensors.palm_touch_data[13] === 170);
check(value.left_touch_sensors.palm_touch_data[14] === 109);
check(value.left_touch_sensors.palm_touch_data[15] === 44);
check(value.left_touch_sensors.palm_touch_data[16] === 239);
check(value.left_touch_sensors.palm_touch_data[17] === 174);
check(value.left_touch_sensors.palm_touch_data[18] === 97);
check(value.left_touch_sensors.palm_touch_data[19] === 32);
check(value.left_touch_sensors.palm_touch_data[20] === 48);
check(value.left_touch_sensors.palm_touch_data[21] === 113);
check(value.left_touch_sensors.palm_touch_data[22] === 178);
check(value.left_touch_sensors.palm_touch_data[23] === 243);
check(value.left_touch_sensors.palm_touch_data[24] === 52);
check(value.left_touch_sensors.palm_touch_data[25] === 117);
check(value.left_touch_sensors.palm_touch_data[26] === 182);
check(value.left_touch_sensors.palm_touch_data[27] === 247);
check(value.left_touch_sensors.palm_touch_data[28] === 56);
check(value.left_touch_sensors.palm_touch_data[29] === 121);
check(value.left_touch_sensors.palm_touch_data[30] === 7);
check(value.left_touch_sensors.palm_touch_data[31] === 70);
check(value.left_touch_sensors.palm_touch_data[32] === 133);
check(value.left_touch_sensors.palm_touch_data[33] === 196);
check(value.left_touch_sensors.palm_touch_data[34] === 3);
check(value.left_touch_sensors.palm_touch_data[35] === 66);
check(value.left_touch_sensors.back_of_hand_touch_data.length == 36);
check(value.left_touch_sensors.back_of_hand_touch_data[0] === 83);
check(value.left_touch_sensors.back_of_hand_touch_data[1] === 18);
check(value.left_touch_sensors.back_of_hand_touch_data[2] === 209);
check(value.left_touch_sensors.back_of_hand_touch_data[3] === 144);
check(value.left_touch_sensors.back_of_hand_touch_data[4] === 87);
check(value.left_touch_sensors.back_of_hand_touch_data[5] === 22);
check(value.left_touch_sensors.back_of_hand_touch_data[6] === 213);
check(value.left_touch_sensors.back_of_hand_touch_data[7] === 148);
check(value.left_touch_sensors.back_of_hand_touch_data[8] === 91);
check(value.left_touch_sensors.back_of_hand_touch_data[9] === 26);
check(value.left_touch_sensors.back_of_hand_touch_data[10] === 162);
check(value.left_touch_sensors.back_of_hand_touch_data[11] === 227);
check(value.left_touch_sensors.back_of_hand_touch_data[12] === 32);
check(value.left_touch_sensors.back_of_hand_touch_data[13] === 97);
check(value.left_touch_sensors.back_of_hand_touch_data[14] === 166);
check(value.left_touch_sensors.back_of_hand_touch_data[15] === 231);
check(value.left_touch_sensors.back_of_hand_touch_data[16] === 36);
check(value.left_touch_sensors.back_of_hand_touch_data[17] === 101);
check(value.left_touch_sensors.back_of_hand_touch_data[18] === 170);
check(value.left_touch_sensors.back_of_hand_touch_data[19] === 235);
check(value.left_touch_sensors.back_of_hand_touch_data[20] === 251);
check(value.left_touch_sensors.back_of_hand_touch_data[21] === 186);
check(value.left_touch_sensors.back_of_hand_touch_data[22] === 121);
check(value.left_touch_sensors.back_of_hand_touch_data[23] === 56);
check(value.left_touch_sensors.back_of_hand_touch_data[24] === 255);
check(value.left_touch_sensors.back_of_hand_touch_data[25] === 190);
check(value.left_touch_sensors.back_of_hand_touch_data[26] === 125);
check(value.left_touch_sensors.back_of_hand_touch_data[27] === 60);
check(value.left_touch_sensors.back_of_hand_touch_data[28] === 243);
check(value.left_touch_sensors.back_of_hand_touch_data[29] === 178);
check(value.left_touch_sensors.back_of_hand_touch_data[30] === 204);
check(value.left_touch_sensors.back_of_hand_touch_data[31] === 141);
check(value.left_touch_sensors.back_of_hand_touch_data[32] === 78);
check(value.left_touch_sensors.back_of_hand_touch_data[33] === 15);
check(value.left_touch_sensors.back_of_hand_touch_data[34] === 200);
check(value.left_touch_sensors.back_of_hand_touch_data[35] === 137);
check(value.left_touch_sensors.thumb_touch_data.length == 16);
check(value.left_touch_sensors.thumb_touch_data[0] === 203);
check(value.left_touch_sensors.thumb_touch_data[1] === 138);
check(value.left_touch_sensors.thumb_touch_data[2] === 73);
check(value.left_touch_sensors.thumb_touch_data[3] === 8);
check(value.left_touch_sensors.thumb_touch_data[4] === 207);
check(value.left_touch_sensors.thumb_touch_data[5] === 142);
check(value.left_touch_sensors.thumb_touch_data[6] === 77);
check(value.left_touch_sensors.thumb_touch_data[7] === 12);
check(value.left_touch_sensors.thumb_touch_data[8] === 195);
check(value.left_touch_sensors.thumb_touch_data[9] === 130);
check(value.left_touch_sensors.thumb_touch_data[10] === 64);
check(value.left_touch_sensors.thumb_touch_data[11] === 1);
check(value.left_touch_sensors.thumb_touch_data[12] === 194);
check(value.left_touch_sensors.thumb_touch_data[13] === 131);
check(value.left_touch_sensors.thumb_touch_data[14] === 68);
check(value.left_touch_sensors.thumb_touch_data[15] === 5);
check(value.left_touch_sensors.index_finger_touch_data.length == 16);
check(value.left_touch_sensors.index_finger_touch_data[0] === 20);
check(value.left_touch_sensors.index_finger_touch_data[1] === 85);
check(value.left_touch_sensors.index_finger_touch_data[2] === 150);
check(value.left_touch_sensors.index_finger_touch_data[3] === 215);
check(value.left_touch_sensors.index_finger_touch_data[4] === 16);
check(value.left_touch_sensors.index_finger_touch_data[5] === 81);
check(value.left_touch_sensors.index_finger_touch_data[6] === 146);
check(value.left_touch_sensors.index_finger_touch_data[7] === 211);
check(value.left_touch_sensors.index_finger_touch_data[8] === 28);
check(value.left_touch_sensors.index_finger_touch_data[9] === 93);
check(value.left_touch_sensors.index_finger_touch_data[10] === 142);
check(value.left_touch_sensors.index_finger_touch_data[11] === 207);
check(value.left_touch_sensors.index_finger_touch_data[12] === 12);
check(value.left_touch_sensors.index_finger_touch_data[13] === 77);
check(value.left_touch_sensors.index_finger_touch_data[14] === 138);
check(value.left_touch_sensors.index_finger_touch_data[15] === 203);
check(value.left_touch_sensors.middle_finger_touch_data.length == 16);
check(value.left_touch_sensors.middle_finger_touch_data[0] === 34);
check(value.left_touch_sensors.middle_finger_touch_data[1] === 99);
check(value.left_touch_sensors.middle_finger_touch_data[2] === 160);
check(value.left_touch_sensors.middle_finger_touch_data[3] === 225);
check(value.left_touch_sensors.middle_finger_touch_data[4] === 38);
check(value.left_touch_sensors.middle_finger_touch_data[5] === 103);
check(value.left_touch_sensors.middle_finger_touch_data[6] === 164);
check(value.left_touch_sensors.middle_finger_touch_data[7] === 229);
check(value.left_touch_sensors.middle_finger_touch_data[8] === 42);
check(value.left_touch_sensors.middle_finger_touch_data[9] === 107);
check(value.left_touch_sensors.middle_finger_touch_data[10] === 91);
check(value.left_touch_sensors.middle_finger_touch_data[11] === 26);
check(value.left_touch_sensors.middle_finger_touch_data[12] === 217);
check(value.left_touch_sensors.middle_finger_touch_data[13] === 152);
check(value.left_touch_sensors.middle_finger_touch_data[14] === 95);
check(value.left_touch_sensors.middle_finger_touch_data[15] === 30);
check(value.left_touch_sensors.ring_finger_touch_data.length == 16);
check(value.left_touch_sensors.ring_finger_touch_data[0] === 235);
check(value.left_touch_sensors.ring_finger_touch_data[1] === 170);
check(value.left_touch_sensors.ring_finger_touch_data[2] === 105);
check(value.left_touch_sensors.ring_finger_touch_data[3] === 40);
check(value.left_touch_sensors.ring_finger_touch_data[4] === 239);
check(value.left_touch_sensors.ring_finger_touch_data[5] === 174);
check(value.left_touch_sensors.ring_finger_touch_data[6] === 109);
check(value.left_touch_sensors.ring_finger_touch_data[7] === 44);
check(value.left_touch_sensors.ring_finger_touch_data[8] === 227);
check(value.left_touch_sensors.ring_finger_touch_data[9] === 162);
check(value.left_touch_sensors.ring_finger_touch_data[10] === 110);
check(value.left_touch_sensors.ring_finger_touch_data[11] === 47);
check(value.left_touch_sensors.ring_finger_touch_data[12] === 236);
check(value.left_touch_sensors.ring_finger_touch_data[13] === 173);
check(value.left_touch_sensors.ring_finger_touch_data[14] === 106);
check(value.left_touch_sensors.ring_finger_touch_data[15] === 43);
check(value.left_touch_sensors.little_finger_touch_data.length == 16);
check(value.left_touch_sensors.little_finger_touch_data[0] === 48);
check(value.left_touch_sensors.little_finger_touch_data[1] === 113);
check(value.left_touch_sensors.little_finger_touch_data[2] === 178);
check(value.left_touch_sensors.little_finger_touch_data[3] === 243);
check(value.left_touch_sensors.little_finger_touch_data[4] === 52);
check(value.left_touch_sensors.little_finger_touch_data[5] === 117);
check(value.left_touch_sensors.little_finger_touch_data[6] === 182);
check(value.left_touch_sensors.little_finger_touch_data[7] === 247);
check(value.left_touch_sensors.little_finger_touch_data[8] === 56);
check(value.left_touch_sensors.little_finger_touch_data[9] === 121);
check(value.left_touch_sensors.little_finger_touch_data[10] === 80);
check(value.left_touch_sensors.little_finger_touch_data[11] === 17);
check(value.left_touch_sensors.little_finger_touch_data[12] === 210);
check(value.left_touch_sensors.little_finger_touch_data[13] === 147);
check(value.left_touch_sensors.little_finger_touch_data[14] === 84);
check(value.left_touch_sensors.little_finger_touch_data[15] === 21);
check(value.right_hand_type.value === 184);
check(value.right_hands.length == 1);
check(value.right_hands[0].name === "v1804β");
check(value.right_hands[0].position === 241.25);
check(value.right_hands[0].velocity === -5.75);
check(value.right_hands[0].effort === 165.5);
check(value.right_hands[0].state === -1294664163);
check(value.right_hands[0].faultcode === 1503628755);
check(value.right_touch_sensors.palm_touch_data.length == 36);
check(value.right_touch_sensors.palm_touch_data[0] === 146);
check(value.right_touch_sensors.palm_touch_data[1] === 211);
check(value.right_touch_sensors.palm_touch_data[2] === 16);
check(value.right_touch_sensors.palm_touch_data[3] === 81);
check(value.right_touch_sensors.palm_touch_data[4] === 150);
check(value.right_touch_sensors.palm_touch_data[5] === 215);
check(value.right_touch_sensors.palm_touch_data[6] === 20);
check(value.right_touch_sensors.palm_touch_data[7] === 85);
check(value.right_touch_sensors.palm_touch_data[8] === 154);
check(value.right_touch_sensors.palm_touch_data[9] === 219);
check(value.right_touch_sensors.palm_touch_data[10] === 188);
check(value.right_touch_sensors.palm_touch_data[11] === 253);
check(value.right_touch_sensors.palm_touch_data[12] === 62);
check(value.right_touch_sensors.palm_touch_data[13] === 127);
check(value.right_touch_sensors.palm_touch_data[14] === 184);
check(value.right_touch_sensors.palm_touch_data[15] === 249);
check(value.right_touch_sensors.palm_touch_data[16] === 58);
check(value.right_touch_sensors.palm_touch_data[17] === 123);
check(value.right_touch_sensors.palm_touch_data[18] === 180);
check(value.right_touch_sensors.palm_touch_data[19] === 245);
check(value.right_touch_sensors.palm_touch_data[20] === 229);
check(value.right_touch_sensors.palm_touch_data[21] === 164);
check(value.right_touch_sensors.palm_touch_data[22] === 103);
check(value.right_touch_sensors.palm_touch_data[23] === 38);
check(value.right_touch_sensors.palm_touch_data[24] === 225);
check(value.right_touch_sensors.palm_touch_data[25] === 160);
check(value.right_touch_sensors.palm_touch_data[26] === 99);
check(value.right_touch_sensors.palm_touch_data[27] === 34);
check(value.right_touch_sensors.palm_touch_data[28] === 237);
check(value.right_touch_sensors.palm_touch_data[29] === 172);
check(value.right_touch_sensors.palm_touch_data[30] === 210);
check(value.right_touch_sensors.palm_touch_data[31] === 147);
check(value.right_touch_sensors.palm_touch_data[32] === 80);
check(value.right_touch_sensors.palm_touch_data[33] === 17);
check(value.right_touch_sensors.palm_touch_data[34] === 214);
check(value.right_touch_sensors.palm_touch_data[35] === 151);
check(value.right_touch_sensors.back_of_hand_touch_data.length == 36);
check(value.right_touch_sensors.back_of_hand_touch_data[0] === 61);
check(value.right_touch_sensors.back_of_hand_touch_data[1] === 124);
check(value.right_touch_sensors.back_of_hand_touch_data[2] === 191);
check(value.right_touch_sensors.back_of_hand_touch_data[3] === 254);
check(value.right_touch_sensors.back_of_hand_touch_data[4] === 57);
check(value.right_touch_sensors.back_of_hand_touch_data[5] === 120);
check(value.right_touch_sensors.back_of_hand_touch_data[6] === 187);
check(value.right_touch_sensors.back_of_hand_touch_data[7] === 250);
check(value.right_touch_sensors.back_of_hand_touch_data[8] === 53);
check(value.right_touch_sensors.back_of_hand_touch_data[9] === 116);
check(value.right_touch_sensors.back_of_hand_touch_data[10] === 53);
check(value.right_touch_sensors.back_of_hand_touch_data[11] === 116);
check(value.right_touch_sensors.back_of_hand_touch_data[12] === 183);
check(value.right_touch_sensors.back_of_hand_touch_data[13] === 246);
check(value.right_touch_sensors.back_of_hand_touch_data[14] === 49);
check(value.right_touch_sensors.back_of_hand_touch_data[15] === 112);
check(value.right_touch_sensors.back_of_hand_touch_data[16] === 179);
check(value.right_touch_sensors.back_of_hand_touch_data[17] === 242);
check(value.right_touch_sensors.back_of_hand_touch_data[18] === 61);
check(value.right_touch_sensors.back_of_hand_touch_data[19] === 124);
check(value.right_touch_sensors.back_of_hand_touch_data[20] === 108);
check(value.right_touch_sensors.back_of_hand_touch_data[21] === 45);
check(value.right_touch_sensors.back_of_hand_touch_data[22] === 238);
check(value.right_touch_sensors.back_of_hand_touch_data[23] === 175);
check(value.right_touch_sensors.back_of_hand_touch_data[24] === 104);
check(value.right_touch_sensors.back_of_hand_touch_data[25] === 41);
check(value.right_touch_sensors.back_of_hand_touch_data[26] === 234);
check(value.right_touch_sensors.back_of_hand_touch_data[27] === 171);
check(value.right_touch_sensors.back_of_hand_touch_data[28] === 100);
check(value.right_touch_sensors.back_of_hand_touch_data[29] === 37);
check(value.right_touch_sensors.back_of_hand_touch_data[30] === 91);
check(value.right_touch_sensors.back_of_hand_touch_data[31] === 26);
check(value.right_touch_sensors.back_of_hand_touch_data[32] === 217);
check(value.right_touch_sensors.back_of_hand_touch_data[33] === 152);
check(value.right_touch_sensors.back_of_hand_touch_data[34] === 95);
check(value.right_touch_sensors.back_of_hand_touch_data[35] === 30);
check(value.right_touch_sensors.thumb_touch_data.length == 16);
check(value.right_touch_sensors.thumb_touch_data[0] === 30);
check(value.right_touch_sensors.thumb_touch_data[1] === 95);
check(value.right_touch_sensors.thumb_touch_data[2] === 156);
check(value.right_touch_sensors.thumb_touch_data[3] === 221);
check(value.right_touch_sensors.thumb_touch_data[4] === 26);
check(value.right_touch_sensors.thumb_touch_data[5] === 91);
check(value.right_touch_sensors.thumb_touch_data[6] === 152);
check(value.right_touch_sensors.thumb_touch_data[7] === 217);
check(value.right_touch_sensors.thumb_touch_data[8] === 22);
check(value.right_touch_sensors.thumb_touch_data[9] === 87);
check(value.right_touch_sensors.thumb_touch_data[10] === 132);
check(value.right_touch_sensors.thumb_touch_data[11] === 197);
check(value.right_touch_sensors.thumb_touch_data[12] === 6);
check(value.right_touch_sensors.thumb_touch_data[13] === 71);
check(value.right_touch_sensors.thumb_touch_data[14] === 128);
check(value.right_touch_sensors.thumb_touch_data[15] === 193);
check(value.right_touch_sensors.index_finger_touch_data.length == 16);
check(value.right_touch_sensors.index_finger_touch_data[0] === 122);
check(value.right_touch_sensors.index_finger_touch_data[1] === 59);
check(value.right_touch_sensors.index_finger_touch_data[2] === 248);
check(value.right_touch_sensors.index_finger_touch_data[3] === 185);
check(value.right_touch_sensors.index_finger_touch_data[4] === 126);
check(value.right_touch_sensors.index_finger_touch_data[5] === 63);
check(value.right_touch_sensors.index_finger_touch_data[6] === 252);
check(value.right_touch_sensors.index_finger_touch_data[7] === 189);
check(value.right_touch_sensors.index_finger_touch_data[8] === 114);
check(value.right_touch_sensors.index_finger_touch_data[9] === 51);
check(value.right_touch_sensors.index_finger_touch_data[10] === 25);
check(value.right_touch_sensors.index_finger_touch_data[11] === 88);
check(value.right_touch_sensors.index_finger_touch_data[12] === 155);
check(value.right_touch_sensors.index_finger_touch_data[13] === 218);
check(value.right_touch_sensors.index_finger_touch_data[14] === 29);
check(value.right_touch_sensors.index_finger_touch_data[15] === 92);
check(value.right_touch_sensors.middle_finger_touch_data.length == 16);
check(value.right_touch_sensors.middle_finger_touch_data[0] === 181);
check(value.right_touch_sensors.middle_finger_touch_data[1] === 244);
check(value.right_touch_sensors.middle_finger_touch_data[2] === 55);
check(value.right_touch_sensors.middle_finger_touch_data[3] === 118);
check(value.right_touch_sensors.middle_finger_touch_data[4] === 177);
check(value.right_touch_sensors.middle_finger_touch_data[5] === 240);
check(value.right_touch_sensors.middle_finger_touch_data[6] === 51);
check(value.right_touch_sensors.middle_finger_touch_data[7] === 114);
check(value.right_touch_sensors.middle_finger_touch_data[8] === 189);
check(value.right_touch_sensors.middle_finger_touch_data[9] === 252);
check(value.right_touch_sensors.middle_finger_touch_data[10] === 173);
check(value.right_touch_sensors.middle_finger_touch_data[11] === 236);
check(value.right_touch_sensors.middle_finger_touch_data[12] === 47);
check(value.right_touch_sensors.middle_finger_touch_data[13] === 110);
check(value.right_touch_sensors.middle_finger_touch_data[14] === 169);
check(value.right_touch_sensors.middle_finger_touch_data[15] === 232);
check(value.right_touch_sensors.ring_finger_touch_data.length == 16);
check(value.right_touch_sensors.ring_finger_touch_data[0] === 152);
check(value.right_touch_sensors.ring_finger_touch_data[1] === 217);
check(value.right_touch_sensors.ring_finger_touch_data[2] === 26);
check(value.right_touch_sensors.ring_finger_touch_data[3] === 91);
check(value.right_touch_sensors.ring_finger_touch_data[4] === 156);
check(value.right_touch_sensors.ring_finger_touch_data[5] === 221);
check(value.right_touch_sensors.ring_finger_touch_data[6] === 30);
check(value.right_touch_sensors.ring_finger_touch_data[7] === 95);
check(value.right_touch_sensors.ring_finger_touch_data[8] === 144);
check(value.right_touch_sensors.ring_finger_touch_data[9] === 209);
check(value.right_touch_sensors.ring_finger_touch_data[10] === 0);
check(value.right_touch_sensors.ring_finger_touch_data[11] === 65);
check(value.right_touch_sensors.ring_finger_touch_data[12] === 130);
check(value.right_touch_sensors.ring_finger_touch_data[13] === 195);
check(value.right_touch_sensors.ring_finger_touch_data[14] === 4);
check(value.right_touch_sensors.ring_finger_touch_data[15] === 69);
check(value.right_touch_sensors.little_finger_touch_data.length == 16);
check(value.right_touch_sensors.little_finger_touch_data[0] === 167);
check(value.right_touch_sensors.little_finger_touch_data[1] === 230);
check(value.right_touch_sensors.little_finger_touch_data[2] === 37);
check(value.right_touch_sensors.little_finger_touch_data[3] === 100);
check(value.right_touch_sensors.little_finger_touch_data[4] === 163);
check(value.right_touch_sensors.little_finger_touch_data[5] === 226);
check(value.right_touch_sensors.little_finger_touch_data[6] === 33);
check(value.right_touch_sensors.little_finger_touch_data[7] === 96);
check(value.right_touch_sensors.little_finger_touch_data[8] === 175);
check(value.right_touch_sensors.little_finger_touch_data[9] === 238);
check(value.right_touch_sensors.little_finger_touch_data[10] === 166);
check(value.right_touch_sensors.little_finger_touch_data[11] === 231);
check(value.right_touch_sensors.little_finger_touch_data[12] === 36);
check(value.right_touch_sensors.little_finger_touch_data[13] === 101);
check(value.right_touch_sensors.little_finger_touch_data[14] === 162);
check(value.right_touch_sensors.little_finger_touch_data[15] === 227);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_23(){const input=bytes("1b5a99d81f5e9ddc1352f4b57637f0b17233fcbdadec2f6ea9e82b6aa5e49adb18599edfe1a06322e5a46726e9a891d0135295d4175699d8c8894a0bcc8d4e0fc081ffbe7d3cfbba5617d4955213d0915e1f19589bda1d5ca6e72465a2e32061aeefbdfc3f7eb9f8115093d2155497d61958fdbc7f3ef9b8c2834001c6874405ca8bdc9d5e1fd899034281c0074685c40b4af6b77435f2b3"),codec=r.Ros2Aimdk_msgsMsgHandTouchSensorDataCodec,value=codec.decode(input);
check(value.palm_touch_data.length == 36);
check(value.palm_touch_data[0] === 27);
check(value.palm_touch_data[1] === 90);
check(value.palm_touch_data[2] === 153);
check(value.palm_touch_data[3] === 216);
check(value.palm_touch_data[4] === 31);
check(value.palm_touch_data[5] === 94);
check(value.palm_touch_data[6] === 157);
check(value.palm_touch_data[7] === 220);
check(value.palm_touch_data[8] === 19);
check(value.palm_touch_data[9] === 82);
check(value.palm_touch_data[10] === 244);
check(value.palm_touch_data[11] === 181);
check(value.palm_touch_data[12] === 118);
check(value.palm_touch_data[13] === 55);
check(value.palm_touch_data[14] === 240);
check(value.palm_touch_data[15] === 177);
check(value.palm_touch_data[16] === 114);
check(value.palm_touch_data[17] === 51);
check(value.palm_touch_data[18] === 252);
check(value.palm_touch_data[19] === 189);
check(value.palm_touch_data[20] === 173);
check(value.palm_touch_data[21] === 236);
check(value.palm_touch_data[22] === 47);
check(value.palm_touch_data[23] === 110);
check(value.palm_touch_data[24] === 169);
check(value.palm_touch_data[25] === 232);
check(value.palm_touch_data[26] === 43);
check(value.palm_touch_data[27] === 106);
check(value.palm_touch_data[28] === 165);
check(value.palm_touch_data[29] === 228);
check(value.palm_touch_data[30] === 154);
check(value.palm_touch_data[31] === 219);
check(value.palm_touch_data[32] === 24);
check(value.palm_touch_data[33] === 89);
check(value.palm_touch_data[34] === 158);
check(value.palm_touch_data[35] === 223);
check(value.back_of_hand_touch_data.length == 36);
check(value.back_of_hand_touch_data[0] === 225);
check(value.back_of_hand_touch_data[1] === 160);
check(value.back_of_hand_touch_data[2] === 99);
check(value.back_of_hand_touch_data[3] === 34);
check(value.back_of_hand_touch_data[4] === 229);
check(value.back_of_hand_touch_data[5] === 164);
check(value.back_of_hand_touch_data[6] === 103);
check(value.back_of_hand_touch_data[7] === 38);
check(value.back_of_hand_touch_data[8] === 233);
check(value.back_of_hand_touch_data[9] === 168);
check(value.back_of_hand_touch_data[10] === 145);
check(value.back_of_hand_touch_data[11] === 208);
check(value.back_of_hand_touch_data[12] === 19);
check(value.back_of_hand_touch_data[13] === 82);
check(value.back_of_hand_touch_data[14] === 149);
check(value.back_of_hand_touch_data[15] === 212);
check(value.back_of_hand_touch_data[16] === 23);
check(value.back_of_hand_touch_data[17] === 86);
check(value.back_of_hand_touch_data[18] === 153);
check(value.back_of_hand_touch_data[19] === 216);
check(value.back_of_hand_touch_data[20] === 200);
check(value.back_of_hand_touch_data[21] === 137);
check(value.back_of_hand_touch_data[22] === 74);
check(value.back_of_hand_touch_data[23] === 11);
check(value.back_of_hand_touch_data[24] === 204);
check(value.back_of_hand_touch_data[25] === 141);
check(value.back_of_hand_touch_data[26] === 78);
check(value.back_of_hand_touch_data[27] === 15);
check(value.back_of_hand_touch_data[28] === 192);
check(value.back_of_hand_touch_data[29] === 129);
check(value.back_of_hand_touch_data[30] === 255);
check(value.back_of_hand_touch_data[31] === 190);
check(value.back_of_hand_touch_data[32] === 125);
check(value.back_of_hand_touch_data[33] === 60);
check(value.back_of_hand_touch_data[34] === 251);
check(value.back_of_hand_touch_data[35] === 186);
check(value.thumb_touch_data.length == 16);
check(value.thumb_touch_data[0] === 86);
check(value.thumb_touch_data[1] === 23);
check(value.thumb_touch_data[2] === 212);
check(value.thumb_touch_data[3] === 149);
check(value.thumb_touch_data[4] === 82);
check(value.thumb_touch_data[5] === 19);
check(value.thumb_touch_data[6] === 208);
check(value.thumb_touch_data[7] === 145);
check(value.thumb_touch_data[8] === 94);
check(value.thumb_touch_data[9] === 31);
check(value.thumb_touch_data[10] === 25);
check(value.thumb_touch_data[11] === 88);
check(value.thumb_touch_data[12] === 155);
check(value.thumb_touch_data[13] === 218);
check(value.thumb_touch_data[14] === 29);
check(value.thumb_touch_data[15] === 92);
check(value.index_finger_touch_data.length == 16);
check(value.index_finger_touch_data[0] === 166);
check(value.index_finger_touch_data[1] === 231);
check(value.index_finger_touch_data[2] === 36);
check(value.index_finger_touch_data[3] === 101);
check(value.index_finger_touch_data[4] === 162);
check(value.index_finger_touch_data[5] === 227);
check(value.index_finger_touch_data[6] === 32);
check(value.index_finger_touch_data[7] === 97);
check(value.index_finger_touch_data[8] === 174);
check(value.index_finger_touch_data[9] === 239);
check(value.index_finger_touch_data[10] === 189);
check(value.index_finger_touch_data[11] === 252);
check(value.index_finger_touch_data[12] === 63);
check(value.index_finger_touch_data[13] === 126);
check(value.index_finger_touch_data[14] === 185);
check(value.index_finger_touch_data[15] === 248);
check(value.middle_finger_touch_data.length == 16);
check(value.middle_finger_touch_data[0] === 17);
check(value.middle_finger_touch_data[1] === 80);
check(value.middle_finger_touch_data[2] === 147);
check(value.middle_finger_touch_data[3] === 210);
check(value.middle_finger_touch_data[4] === 21);
check(value.middle_finger_touch_data[5] === 84);
check(value.middle_finger_touch_data[6] === 151);
check(value.middle_finger_touch_data[7] === 214);
check(value.middle_finger_touch_data[8] === 25);
check(value.middle_finger_touch_data[9] === 88);
check(value.middle_finger_touch_data[10] === 253);
check(value.middle_finger_touch_data[11] === 188);
check(value.middle_finger_touch_data[12] === 127);
check(value.middle_finger_touch_data[13] === 62);
check(value.middle_finger_touch_data[14] === 249);
check(value.middle_finger_touch_data[15] === 184);
check(value.ring_finger_touch_data.length == 16);
check(value.ring_finger_touch_data[0] === 194);
check(value.ring_finger_touch_data[1] === 131);
check(value.ring_finger_touch_data[2] === 64);
check(value.ring_finger_touch_data[3] === 1);
check(value.ring_finger_touch_data[4] === 198);
check(value.ring_finger_touch_data[5] === 135);
check(value.ring_finger_touch_data[6] === 68);
check(value.ring_finger_touch_data[7] === 5);
check(value.ring_finger_touch_data[8] === 202);
check(value.ring_finger_touch_data[9] === 139);
check(value.ring_finger_touch_data[10] === 220);
check(value.ring_finger_touch_data[11] === 157);
check(value.ring_finger_touch_data[12] === 94);
check(value.ring_finger_touch_data[13] === 31);
check(value.ring_finger_touch_data[14] === 216);
check(value.ring_finger_touch_data[15] === 153);
check(value.little_finger_touch_data.length == 16);
check(value.little_finger_touch_data[0] === 3);
check(value.little_finger_touch_data[1] === 66);
check(value.little_finger_touch_data[2] === 129);
check(value.little_finger_touch_data[3] === 192);
check(value.little_finger_touch_data[4] === 7);
check(value.little_finger_touch_data[5] === 70);
check(value.little_finger_touch_data[6] === 133);
check(value.little_finger_touch_data[7] === 196);
check(value.little_finger_touch_data[8] === 11);
check(value.little_finger_touch_data[9] === 74);
check(value.little_finger_touch_data[10] === 246);
check(value.little_finger_touch_data[11] === 183);
check(value.little_finger_touch_data[12] === 116);
check(value.little_finger_touch_data[13] === 53);
check(value.little_finger_touch_data[14] === 242);
check(value.little_finger_touch_data[15] === 179);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_24(){const input=bytes("25"),codec=r.Ros2Aimdk_msgsMsgHandTypeCodec,value=codec.decode(input);
check(value.value === 37);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_25(){const input=bytes("070000007666396635ceb2000000000030644000000000006059c000000000006856400000000000b4654000000000003043c0"),codec=r.Ros2Aimdk_msgsMsgJointCommandCodec,value=codec.decode(input);
check(value.name === "vf9f5β");
check(value.position === 161.5);
check(value.velocity === -101.5);
check(value.effort === 89.625);
check(value.stiffness === 173.625);
check(value.damping === -38.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_26(){const input=bytes("c4e2ccfa36661122070000007636656530ceb2c5a6c503155c6d32fa7a343001000000070000007631646364ceb200000000002855c00000000000fc62400000000000886dc0000000000028644000000000004866c0"),codec=r.Ros2Aimdk_msgsMsgJointCommandArrayCodec,value=codec.decode(input);
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

function test_27(){const input=bytes("070000007634386539ceb200000000008041c00000000000bc64400000000000bc6540cce1"),codec=r.Ros2Aimdk_msgsMsgJointStateCodec,value=codec.decode(input);
check(value.name === "v48e9β");
check(value.position === -35.0);
check(value.velocity === 165.875);
check(value.effort === 173.875);
check(value.error_code === 57804);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_28(){const input=bytes("1952db78f8dfe310070000007637316134ceb293ba811c5be6a64ea09f3a2a7401000000070000007638333532ceb20000000000c855c000000000003867400000000000c038c02b6c"),codec=r.Ros2Aimdk_msgsMsgJointStateArrayCodec,value=codec.decode(input);
check(value.header.stamp.sec === 2027639321);
check(value.header.stamp.nanosec === 283369464);
check(value.header.frame_id === "v71a4β");
check(value.header.sequence === 478263955);
check(value.header.meas_stamp.sec === 1319560795);
check(value.header.meas_stamp.nanosec === 708485024);
check(value.state.value === 116);
check(value.joints.length == 1);
check(value.joints[0].name === "v8352β");
check(value.joints[0].position === -87.125);
check(value.joints[0].velocity === 185.75);
check(value.joints[0].effort === -24.75);
check(value.joints[0].error_code === 27691);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_29(){const input=bytes("9e616d45"),codec=r.Ros2Aimdk_msgsMsgMcActionCodec,value=codec.decode(input);
check(value.value === 1164796318);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_30(){const input=bytes("6ff83014070000007633656562ceb2"),codec=r.Ros2Aimdk_msgsMsgMcActionCommandCodec,value=codec.decode(input);
check(value.action.value === 338753647);
check(value.action_desc === "v3eebβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_31(){const input=bytes("091b656e070000007630343534ceb2543effca"),codec=r.Ros2Aimdk_msgsMsgMcActionInfoCodec,value=codec.decode(input);
check(value.current_action.value === 1852119817);
check(value.action_desc === "v0454β");
check(value.status.value === -889242028);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_32(){const input=bytes("03a0131b"),codec=r.Ros2Aimdk_msgsMsgMcActionStatusCodec,value=codec.decode(input);
check(value.value === 454270979);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_33(){const input=bytes("c61a340d"),codec=r.Ros2Aimdk_msgsMsgMcControlAreaCodec,value=codec.decode(input);
check(value.value === 221518534);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_34(){const input=bytes("64436340"),codec=r.Ros2Aimdk_msgsMsgMcInputActionCodec,value=codec.decode(input);
check(value.value === 1080247140);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_35(){const input=bytes("070000007664636264ceb2d4e7a260518ce9e8"),codec=r.Ros2Aimdk_msgsMsgMcInputSourceCodec,value=codec.decode(input);
check(value.name === "vdcbdβ");
check(value.priority === 1621288916);
check(value.timeout === -387347375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_36(){const input=bytes("1da7c0703c8e991c070000007630373663ceb2de93496a976034d2db683806070000007630363634ceb200000000000c69c0000000000038634000000000003064c0"),codec=r.Ros2Aimdk_msgsMsgMcLocomotionVelocityCodec,value=codec.decode(input);
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

function test_37(){const input=bytes("bbd1e14c"),codec=r.Ros2Aimdk_msgsMsgMcPresetMotionCodec,value=codec.decode(input);
check(value.value === 1289867707);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_38(){const input=bytes("5455aec10663d513070000007636323737ceb2e5fe520f50fd2f14d8db8710"),codec=r.Ros2Aimdk_msgsMsgMessageHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -1045539500);
check(value.stamp.nanosec === 332751622);
check(value.frame_id === "v6277β");
check(value.sequence === 257097445);
check(value.meas_stamp.sec === 338689360);
check(value.meas_stamp.nanosec === 277339096);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_39(){const input=bytes("2c023ff50000000000c050c000000000000052c00000000000d04240"),codec=r.Ros2Aimdk_msgsMsgNaviPointCodec,value=codec.decode(input);
check(value.point_id === -180420052);
check(value.navi_point.x === -67.0);
check(value.navi_point.y === -72.0);
check(value.navi_point.theta === 37.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_40(){const input=bytes("3dc46582010000000000000000b85e400000000000785f400000000000a85bc0"),codec=r.Ros2Aimdk_msgsMsgPathCodec,value=codec.decode(input);
check(value.path_id === -2107259843);
check(value.points.length == 1);
check(value.points[0].x === 122.875);
check(value.points[0].y === 125.875);
check(value.points[0].theta === -110.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_41(){const input=bytes("070000007665613133ceb2c5"),codec=r.Ros2Aimdk_msgsMsgPlayStateChangeCodec,value=codec.decode(input);
check(value.pkg_name === "vea13β");
check(value.state.value === 197);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_42(){const input=bytes("28"),codec=r.Ros2Aimdk_msgsMsgPlayStateTypeCodec,value=codec.decode(input);
check(value.value === 40);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_43(){const input=bytes("070000007637643533ceb29905780d6a070000007634396232ceb2070000007632646662ceb201"),codec=r.Ros2Aimdk_msgsMsgPlayTtsRequestCodec,value=codec.decode(input);
check(value.text === "v7d53β");
check(value.priority_level.value === 153);
check(value.priority_weight === 1779267589);
check(value.domain === "v49b2β");
check(value.trace_id === "v2dfbβ");
check(value.is_interrupted === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_44(){const input=bytes("070000007639376439ceb22d13a7109d070000007636333533ceb2070000007665306536ceb201070000007637663738ceb20f38fdc7"),codec=r.Ros2Aimdk_msgsMsgPlayTtsResponseCodec,value=codec.decode(input);
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

function test_45(){const input=bytes("070000007664623661ceb2070000007634316663ceb2070000007663383133ceb2a9e007c500000000008c6640000000000048584000000000003859c000000000002c63c00000000000203a4000000000004050c000000000004060c00000000000806cc00000000000cc644000000000003858400000000000504a40000000000048614000000000009c6b400000000000485bc000000000003c68c00000000000e03d40000000000000ec6e40070000007665386262ceb2070000007631333430ceb2070000007636353838ceb2070000007666663165ceb2f957209c893c0000000000685140000000000070404000000000007053400000000000b857403de21c6435561304202a24"),codec=r.Ros2Aimdk_msgsMsgPmuStateCodec,value=codec.decode(input);
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
check(value.fan_speed === 29.875);
check(value.fan_pecentage === 0);
check(value.pmu_temperature === 247.375);
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

function test_46(){const input=bytes("74f6ac96d49fda04070000007630383631ceb291fb4465427b4e7c7214af30d8042faf470100000002"),codec=r.Ros2Aimdk_msgsMsgProcessedAudioOutputCodec,value=codec.decode(input);
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

function test_47(){const input=bytes("af80e4d20000000000dc62400000000000a464400000000000b04e400000000000d4624000000000000c664000000000005041400000000000801740"),codec=r.Ros2Aimdk_msgsMsgQRCodeCodec,value=codec.decode(input);
check(value.code === -756776785);
check(value.pose.position.x === 150.875);
check(value.pose.position.y === 165.125);
check(value.pose.position.z === 61.375);
check(value.pose.orientation.x === 150.625);
check(value.pose.orientation.y === 176.375);
check(value.pose.orientation.z === 34.625);
check(value.pose.orientation.w === 5.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_48(){const input=bytes("18260ba054830b6f0000000000806c4000000000005864400000000000e46dc00000000000003a400000000000805c4000000000008037c000000000000467c0"),codec=r.Ros2Aimdk_msgsMsgQRPointCodec,value=codec.decode(input);
check(value.point_id === -1609882088);
check(value.qr_code.code === 1863025492);
check(value.qr_code.pose.position.x === 228.0);
check(value.qr_code.pose.position.y === 162.75);
check(value.qr_code.pose.position.z === -239.125);
check(value.qr_code.pose.orientation.x === 26.0);
check(value.qr_code.pose.orientation.y === 114.0);
check(value.qr_code.pose.orientation.z === -23.5);
check(value.qr_code.pose.orientation.w === -184.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_49(){const input=bytes("fc19070000007635653736ceb2010000000040ec42004069c3000088c2"),codec=r.Ros2Aimdk_msgsMsgRegionCodec,value=codec.decode(input);
check(value.type === 252);
check(value.drawing_type === 25);
check(value.name === "v5e76β");
check(value.polygon.points.length == 1);
check(value.polygon.points[0].x === 118.125);
check(value.polygon.points[0].y === -233.25);
check(value.polygon.points[0].z === -68.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_50(){const input=bytes("7ec27b1a2f62b81e"),codec=r.Ros2Aimdk_msgsMsgRequestHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === 444318334);
check(value.stamp.nanosec === 515400239);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_51(){const input=bytes("55c42dd4c7edd33a863659073f4f6e99"),codec=r.Ros2Aimdk_msgsMsgResponseHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -735198123);
check(value.stamp.nanosec === 986967495);
check(value.code === -7390882806343584122n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_52(){const input=bytes("070000007636353030ceb2070000007631343664ceb2070000007662323335ceb201000000070000007638623339ceb20ef96a45b7805ddb"),codec=r.Ros2Aimdk_msgsMsgRobotResourceCodec,value=codec.decode(input);
check(value.resource_key === "v6500β");
check(value.current_version.version === "v146dβ");
check(value.current_version.name === "vb235β");
check(value.current_version.files.length == 1);
check(value.current_version.files[0] === "v8b39β");
check(value.current_version.download_timestamp_seconds === 15806931791772317966n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_53(){const input=bytes("070000007665663837ceb2"),codec=r.Ros2Aimdk_msgsMsgSlaveDeviceCodec,value=codec.decode(input);
check(value.ip === "vef87β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_54(){const input=bytes("46"),codec=r.Ros2Aimdk_msgsMsgSystemStatusCodec,value=codec.decode(input);
check(value.value === 70);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_55(){const input=bytes("d7825fed79ab2507070000007634613365ceb25bf61b277bc4ddd4778e3f220152585ca1136947b8d03a6a93910b718a569d30c517ac2bdcd4ff06f795ce1deed3bcbd4e928da65751de8b7c10ef9065d779d12a9648ca33551be718142afc010100010001000100"),codec=r.Ros2Aimdk_msgsMsgTouchStateCodec,value=codec.decode(input);
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

function test_56(){const input=bytes("bc"),codec=r.Ros2Aimdk_msgsMsgTtsPriorityLevelCodec,value=codec.decode(input);
check(value.value === 188);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_57(){const input=bytes("133fd9fc54a0e534070000007633636262ceb207e60bcef7dc061a"),codec=r.Ros2Aimdk_msgsSrvAbandonAudioFocus_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -52871405);
check(value.request.header.stamp.nanosec === 887464020);
check(value.focus_requester.pkg_name === "v3cbbβ");
check(value.focus_requester.priority === 3456886279);
check(value.focus_requester.priority_weight === 436657399);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_58(){const input=bytes("8162941f12b61c3ba79126fb1ee81165ee50407d070000007666376633ceb2070000007639306138ceb200"),codec=r.Ros2Aimdk_msgsSrvAbandonAudioFocus_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === 529818241);
check(value.reponse.header.stamp.nanosec === 991737362);
check(value.reponse.header.code === 7282857292195074471n);
check(value.reponse.status.value === 2101367022);
check(value.reponse.message === "vf7f3β");
check(value.focus_response.pkg_name === "v90a8β");
check(value.focus_response.focus_gain === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_59(){const input=bytes("937dc53e00b1bf39070000007632616638ceb2070000007630353138ceb201000000070000007632323066ceb2070000007663636232ceb2"),codec=r.Ros2Aimdk_msgsSrvExecuteActionResource_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 1053130131);
check(value.header.header.stamp.nanosec === 968864000);
check(value.resource_key === "v2af8β");
check(value.resource_version === "v0518β");
check(value.slaves.length == 1);
check(value.slaves[0].ip === "v220fβ");
check(value.meta === "vccb2β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_60(){const input=bytes("c1864d406010982eebe25df0529b6a6e26262102070000007631343639ceb2"),codec=r.Ros2Aimdk_msgsSrvExecuteActionResource_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 1078822593);
check(value.header.header.stamp.nanosec === 781717600);
check(value.header.header.code === 7956342472225710827n);
check(value.header.status.value === 35726886);
check(value.header.message === "v1469β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_61(){const input=bytes("42176be292645b22"),codec=r.Ros2Aimdk_msgsSrvGetAllJointState_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -496298174);
check(value.request.header.stamp.nanosec === 576414866);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_62(){const input=bytes("cbb1e60319f8d70d158016abacf921355fb27f58070000007635353831ceb201000000070000007630323635ceb20000000000205ac000000000008018c00000000000905040cae401000000070000007634366630ceb20000000000e060c000000000001859400000000000c466c0acbf01000000070000007665366135ceb200000000002064c000000000001054c00000000000204640bede01000000070000007637363262ceb20000000000a8544000000000004057400000000000d05e400889"),codec=r.Ros2Aimdk_msgsSrvGetAllJointState_ResponseCodec,value=codec.decode(input);
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
check(value.head_joints[0].error_code === 58570);
check(value.arm_joints.length == 1);
check(value.arm_joints[0].name === "v46f0β");
check(value.arm_joints[0].position === -135.0);
check(value.arm_joints[0].velocity === 100.375);
check(value.arm_joints[0].effort === -182.125);
check(value.arm_joints[0].error_code === 49068);
check(value.waist_joints.length == 1);
check(value.waist_joints[0].name === "ve6a5β");
check(value.waist_joints[0].position === -161.0);
check(value.waist_joints[0].velocity === -80.25);
check(value.waist_joints[0].effort === 44.25);
check(value.waist_joints[0].error_code === 57022);
check(value.leg_joints.length == 1);
check(value.leg_joints[0].name === "v762bβ");
check(value.leg_joints[0].position === 82.625);
check(value.leg_joints[0].velocity === 93.0);
check(value.leg_joints[0].effort === 123.25);
check(value.leg_joints[0].error_code === 35080);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_63(){const input=bytes("356cda9cc4940907"),codec=r.Ros2Aimdk_msgsSrvGetCurrentInputSource_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -1663407051);
check(value.request.header.stamp.nanosec === 118068420);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_64(){const input=bytes("8f9e7c10e0656a2587abc65d3ed2f1c340b51964f9cc2efa08534872070000007666656432ceb2d91539cd3bc79314"),codec=r.Ros2Aimdk_msgsSrvGetCurrentInputSource_ResponseCodec,value=codec.decode(input);
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

function test_65(){const input=bytes("216fa6ac032ce70e"),codec=r.Ros2Aimdk_msgsSrvGetHandType_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -1398378719);
check(value.request.header.stamp.nanosec === 250031107);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_66(){const input=bytes("514c13d76fc2151cd7889bf26ef1ac6ccb9c4c2d070000007638303934ceb2a04f"),codec=r.Ros2Aimdk_msgsSrvGetHandType_ResponseCodec,value=codec.decode(input);
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

function test_67(){const input=bytes("16b2cbc742412923"),codec=r.Ros2Aimdk_msgsSrvGetMcAction_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -942951914);
check(value.request.header.stamp.nanosec === 589906242);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_68(){const input=bytes("371ceb02ee57961fe9cb311a50b20684cb955833070000007630626164ceb2d172243e"),codec=r.Ros2Aimdk_msgsSrvGetMcAction_ResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === 48962615);
check(value.header.stamp.nanosec === 529946606);
check(value.header.code === -8933256753736201239n);
check(value.info.current_action.value === 861443531);
check(value.info.action_desc === "v0badβ");
check(value.info.status.value === 1042576081);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_69(){const input=bytes("86376c09145e3d31"),codec=r.Ros2Aimdk_msgsSrvGetMicSourceRequest_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 158087046);
check(value.header.header.stamp.nanosec === 826105364);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_70(){const input=bytes("60cba72d1064fa0c804d0c9e39343b0059cf2fd8070000007632373833ceb2ea22645e"),codec=r.Ros2Aimdk_msgsSrvGetMicSourceRequest_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 765971296);
check(value.header.header.stamp.nanosec === 217736208);
check(value.header.header.code === 16664445695315328n);
check(value.header.status.value === -667955367);
check(value.header.message === "v2783β");
check(value.mic_source === 1583620842);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_71(){const input=bytes("40cabdd47aeddb2e"),codec=r.Ros2Aimdk_msgsSrvGetMute_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -725759424);
check(value.request.header.stamp.nanosec === 786165114);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_72(){const input=bytes("3a06deedcacac334f4fabbbf4d838c21cbcd668f070000007630326338ceb201"),codec=r.Ros2Aimdk_msgsSrvGetMute_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -304216518);
check(value.reponse.header.stamp.nanosec === 885246666);
check(value.reponse.header.code === 2417451469943601908n);
check(value.reponse.status.value === -1889088053);
check(value.reponse.message === "v02c8β");
check(value.is_mute === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_73(){const input=bytes("eebded370f49c415"),codec=r.Ros2Aimdk_msgsSrvGetRobotResources_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 938327534);
check(value.header.header.stamp.nanosec === 365185295);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_74(){const input=bytes("80a3f06e09c25e35210b423a987275a461125a79070000007637653831ceb201000000070000007661363138ceb2070000007638653337ceb2070000007631356533ceb201000000070000007636306164ceb2482ff1a8f156c636"),codec=r.Ros2Aimdk_msgsSrvGetRobotResources_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 1861264256);
check(value.header.header.stamp.nanosec === 895402505);
check(value.header.header.code === -6596240081076221151n);
check(value.header.status.value === 2035946081);
check(value.header.message === "v7e81β");
check(value.robot_resources.length == 1);
check(value.robot_resources[0].resource_key === "va618β");
check(value.robot_resources[0].current_version.version === "v8e37β");
check(value.robot_resources[0].current_version.name === "v15e3β");
check(value.robot_resources[0].current_version.files.length == 1);
check(value.robot_resources[0].current_version.files[0] === "v60adβ");
check(value.robot_resources[0].current_version.download_timestamp_seconds === 3946937719358304072n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_75(){const input=bytes("1c6b470b45985818070000007633313862ceb2070000007666376238ceb2"),codec=r.Ros2Aimdk_msgsSrvGetStoredMapByName_RequestCodec,value=codec.decode(input);
check(value.header.stamp.sec === 189229852);
check(value.header.stamp.nanosec === 408459333);
check(value.header.frame_id === "v318bβ");
check(value.map_name === "vf7b8β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_76(){const input=bytes("d9024b13607b7c8db0a5078da2eed02b070000007661323937ceb2161a82c1e352ff0000206fc30094b4038776135e0000000000304bc000000000000463c000000000002c6ac00000000000a85cc00000000000c465c0000000000058574000000000000026c0c83ff7417146c0df070000007634653336ceb2010000005501000000d95412150000000000e033400000000000d85ac00000000000c050c0010000002ee1f528cd63f74700000000004850400000000000a467400000000000485c4000000000000037c000000000001055400000000000b06b400000000000802640010000002a7939d90000000000d85b400000000000b46c400000000000b856400000000000e0374000000000002854400000000000f869c000000000002037c0010000000cbc470f00000000002044400000000000c050c00000000000105cc000000000004850c00000000000b857c0000000000060694000000000005067c00001000000d9e7b0bd0100000000000000000050c00000000000c058400000000000203c4001000000944f070000007661386238ceb2010000000000d141008074c200403343192f99f0a056ae6e"),codec=r.Ros2Aimdk_msgsSrvGetStoredMapByName_ResponseCodec,value=codec.decode(input);
check(value.code === 10195159309030720217n);
check(value.header.stamp.sec === -1928878672);
check(value.header.stamp.nanosec === 735112866);
check(value.header.frame_id === "va297β");
check(value.map_info.map_load_time.sec === -1048438250);
check(value.map_info.map_load_time.nanosec === 16732899);
check(value.map_info.resolution === -239.125);
check(value.map_info.width === 62166016);
check(value.map_info.height === 1578333831);
check(value.map_info.origin.position.x === -54.375);
check(value.map_info.origin.position.y === -152.125);
check(value.map_info.origin.position.z === -209.375);
check(value.map_info.origin.orientation.x === -114.625);
check(value.map_info.origin.orientation.y === -174.125);
check(value.map_info.origin.orientation.z === 93.375);
check(value.map_info.origin.orientation.w === -11.0);
check(value.map_version === 16122964118238347208n);
check(value.map_path === "v4e36β");
check(value.data.length == 1);
check(value.data[0] === 85);
check(value.navi_points.length == 1);
check(value.navi_points[0].point_id === 353522905);
check(value.navi_points[0].navi_point.x === 19.875);
check(value.navi_points[0].navi_point.y === -107.375);
check(value.navi_points[0].navi_point.theta === -67.0);
check(value.qr_points.length == 1);
check(value.qr_points[0].point_id === 687202606);
check(value.qr_points[0].qr_code.code === 1207395277);
check(value.qr_points[0].qr_code.pose.position.x === 65.125);
check(value.qr_points[0].qr_code.pose.position.y === 189.125);
check(value.qr_points[0].qr_code.pose.position.z === 113.125);
check(value.qr_points[0].qr_code.pose.orientation.x === -23.0);
check(value.qr_points[0].qr_code.pose.orientation.y === 84.25);
check(value.qr_points[0].qr_code.pose.orientation.z === 221.5);
check(value.qr_points[0].qr_code.pose.orientation.w === 11.25);
check(value.hr_points.length == 1);
check(value.hr_points[0].point_id === -650544854);
check(value.hr_points[0].pose.position.x === 111.375);
check(value.hr_points[0].pose.position.y === 229.625);
check(value.hr_points[0].pose.position.z === 90.875);
check(value.hr_points[0].pose.orientation.x === 23.875);
check(value.hr_points[0].pose.orientation.y === 80.625);
check(value.hr_points[0].pose.orientation.z === -207.75);
check(value.hr_points[0].pose.orientation.w === -23.125);
check(value.hr_points[0].qr_list.length == 1);
check(value.hr_points[0].qr_list[0].code === 256359436);
check(value.hr_points[0].qr_list[0].pose.position.x === 40.25);
check(value.hr_points[0].qr_list[0].pose.position.y === -67.0);
check(value.hr_points[0].qr_list[0].pose.position.z === -112.25);
check(value.hr_points[0].qr_list[0].pose.orientation.x === -65.125);
check(value.hr_points[0].qr_list[0].pose.orientation.y === -94.875);
check(value.hr_points[0].qr_list[0].pose.orientation.z === 203.0);
check(value.hr_points[0].qr_list[0].pose.orientation.w === -186.5);
check(value.hr_points[0].use_head === false);
check(value.paths.length == 1);
check(value.paths[0].path_id === -1112479783);
check(value.paths[0].points.length == 1);
check(value.paths[0].points[0].x === -64.0);
check(value.paths[0].points[0].y === 99.0);
check(value.paths[0].points[0].theta === 28.125);
check(value.regions.length == 1);
check(value.regions[0].type === 148);
check(value.regions[0].drawing_type === 79);
check(value.regions[0].name === "va8b8β");
check(value.regions[0].polygon.points.length == 1);
check(value.regions[0].polygon.points[0].x === 26.125);
check(value.regions[0].polygon.points[0].y === -61.125);
check(value.regions[0].polygon.points[0].z === 179.25);
check(value.map_id === 7975407239351054105n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_77(){const input=bytes("3a5c948150ab5c15"),codec=r.Ros2Aimdk_msgsSrvGetSystemState_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -2120983494);
check(value.header.header.stamp.nanosec === 358394704);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_78(){const input=bytes("acccf8ef2c09871b64cb6112ddb2568cbe84c47f070000007635643437ceb2070000007639336630ceb2ae"),codec=r.Ros2Aimdk_msgsSrvGetSystemState_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -268907348);
check(value.header.header.stamp.nanosec === 461834540);
check(value.header.header.code === -8334277397836608668n);
check(value.header.status.value === 2143585470);
check(value.header.message === "v5d47β");
check(value.cur_state === "v93f0β");
check(value.curr_status.value === 174);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_79(){const input=bytes("3482754f43b7261d"),codec=r.Ros2Aimdk_msgsSrvGetVolume_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === 1333101108);
check(value.request.header.stamp.nanosec === 489076547);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_80(){const input=bytes("577b2dba3e2c9f05138b111baaf226856110acb1070000007638363464ceb2a22c91c9"),codec=r.Ros2Aimdk_msgsSrvGetVolume_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -1171424425);
check(value.reponse.header.stamp.nanosec === 94317630);
check(value.reponse.header.code === -8852121205137634541n);
check(value.reponse.status.value === -1314123679);
check(value.reponse.message === "v864dβ");
check(value.audio_volume === 3381734562);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_81(){const input=bytes("986bc7be61089b29070000007631636235ceb2"),codec=r.Ros2Aimdk_msgsSrvMigrateSystemState_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -1094227048);
check(value.header.header.stamp.nanosec === 698026081);
check(value.state === "v1cb5β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_82(){const input=bytes("5f5d1fd7173d3328413fcc53f846fbcd0dfd8134070000007663613535ceb2"),codec=r.Ros2Aimdk_msgsSrvMigrateSystemState_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -685810337);
check(value.header.header.stamp.nanosec === 674446615);
check(value.header.header.code === -3604209044408221887n);
check(value.header.status.value === 880934157);
check(value.header.message === "vca55β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_83(){const input=bytes("1dbc0108acada711070000007634613535ceb2070000007665386230ceb2070000007662646337ceb20088d997715e7a7655070000007636613064ceb2070000007666623165ceb281c4e5b8fc211462744e38b2b3c32611"),codec=r.Ros2Aimdk_msgsSrvPlayAudioFile_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === 134331421);
check(value.request.header.stamp.nanosec === 296201644);
check(value.file.pkg_name === "v4a55β");
check(value.file.file_name === "ve8b0β");
check(value.file.file_path === "vbdc7β");
check(value.file.info.channels === 0);
check(value.file.info.sample_rate === 1905777032);
check(value.file.info.size === 1433827934);
check(value.file.info.sample_format === "v6a0dβ");
check(value.file.info.coding_format === "vfb1eβ");
check(value.file.priority === 3102065793);
check(value.file.priority_weight === 1645486588);
check(value.play_stamps.sec === -1304932748);
check(value.play_stamps.nanosec === 287753139);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_84(){const input=bytes("0597d8f897bd541193b1d2062ac8e5987b50095c070000007661363963ceb2"),codec=r.Ros2Aimdk_msgsSrvPlayAudioFile_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -120023291);
check(value.reponse.header.stamp.nanosec === 290766231);
check(value.reponse.header.code === -7429311927449112173n);
check(value.reponse.status.value === 1544114299);
check(value.reponse.message === "va69cβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_85(){const input=bytes("fb206e27ed8a9b1302cbe66ca555"),codec=r.Ros2Aimdk_msgsSrvPlayEmoji_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 661528827);
check(value.header.header.stamp.nanosec === 328960749);
check(value.emotion_id === 2);
check(value.mode === 203);
check(value.priority === 1436904678);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_86(){const input=bytes("f6c43d030ac08f040abdee5cb3c4d9c2974780d5070000007635323838ceb201070000007632373339ceb2"),codec=r.Ros2Aimdk_msgsSrvPlayEmoji_ResponseCodec,value=codec.decode(input);
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

function test_87(){const input=bytes("16fddc490ed7d11b070000007639383961ceb213d9d1d86c070000007637343130ceb2070000007634343863ceb200"),codec=r.Ros2Aimdk_msgsSrvPlayTts_RequestCodec,value=codec.decode(input);
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

function test_88(){const input=bytes("eee8e8dd779080066fe0aca5d6999b3b1d90a1e8070000007636303330ceb2070000007661363062ceb2fad6e30077070000007662653934ceb2070000007639336335ceb201070000007663366266ceb22bf63bd9"),codec=r.Ros2Aimdk_msgsSrvPlayTts_ResponseCodec,value=codec.decode(input);
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

function test_89(){const input=bytes("1dffbbb536b15e1d01000000070000007662393735ceb2f19e4bf33a"),codec=r.Ros2Aimdk_msgsSrvPlayVideoGroup_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -1245970659);
check(value.header.header.stamp.nanosec === 492745014);
check(value.video_path_list.length == 1);
check(value.video_path_list[0] === "vb975β");
check(value.mode === 241);
check(value.priority === 989023134);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_90(){const input=bytes("6456c64af906b92efe9c0dbd47e53a23b392b16f070000007634336230ceb201070000007634383666ceb2"),codec=r.Ros2Aimdk_msgsSrvPlayVideoGroup_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 1254512228);
check(value.header.header.stamp.nanosec === 783877881);
check(value.header.header.code === 2538593436253920510n);
check(value.header.status.value === 1873908403);
check(value.header.message === "v43b0β");
check(value.success === true);
check(value.message === "v486fβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_91(){const input=bytes("23679b0a535df030070000007639653337ceb2d417bc5be7"),codec=r.Ros2Aimdk_msgsSrvPlayVideo_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 177956643);
check(value.header.header.stamp.nanosec === 821058899);
check(value.video_path === "v9e37β");
check(value.mode === 212);
check(value.priority === -413418473);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_92(){const input=bytes("576b188bc2210c1c3d435581843a621f6659e06d070000007663326330ceb200070000007639356337ceb2"),codec=r.Ros2Aimdk_msgsSrvPlayVideo_ResponseCodec,value=codec.decode(input);
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

function test_93(){const input=bytes("18d88cdd3ffa1b12070000007631646565ceb20c015eefd72b3dfb"),codec=r.Ros2Aimdk_msgsSrvRequestAudioFocus_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -577972200);
check(value.request.header.stamp.nanosec === 303823423);
check(value.focus_requester.pkg_name === "v1deeβ");
check(value.focus_requester.priority === 4015915276);
check(value.focus_requester.priority_weight === 4215090135);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_94(){const input=bytes("eeee67883aa9bb2ee91665a3506f523dfe7f7cec070000007665353835ceb2070000007662316664ceb201"),codec=r.Ros2Aimdk_msgsSrvRequestAudioFocus_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -2006454546);
check(value.reponse.header.stamp.nanosec === 784050490);
check(value.reponse.header.code === 4418716576533255913n);
check(value.reponse.status.value === -327385090);
check(value.reponse.message === "ve585β");
check(value.focus_response.pkg_name === "vb1fdβ");
check(value.focus_response.focus_gain === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_95(){const input=bytes("38249d9b00ba66140100000035070000007635636336ceb2"),codec=r.Ros2Aimdk_msgsSrvSetAgentPropertiesRequest_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -1684200392);
check(value.header.header.stamp.nanosec === 342276608);
check(value.contents.properties.length == 1);
check(value.contents.properties[0].key.value === 53);
check(value.contents.properties[0].value === "v5cc6β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_96(){const input=bytes("f8a4ec016be9aa2e2f5836a996210137354b719e070000007661393561ceb2"),codec=r.Ros2Aimdk_msgsSrvSetAgentPropertiesRequest_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === 32285944);
check(value.header.header.stamp.nanosec === 782952811);
check(value.header.header.code === 3963486078030469167n);
check(value.header.status.value === -1636742347);
check(value.header.message === "va95aβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_97(){const input=bytes("7d1b67589f966f04070000007638616439ceb2bba8a149070000007663376130ceb2"),codec=r.Ros2Aimdk_msgsSrvSetMcAction_RequestCodec,value=codec.decode(input);
check(value.header.stamp.sec === 1483152253);
check(value.header.stamp.nanosec === 74421919);
check(value.source === "v8ad9β");
check(value.command.action.value === 1235331259);
check(value.command.action_desc === "vc7a0β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_98(){const input=bytes("afba180a3b3980022cc8591f95b16e811f75ce07070000007664656439ceb2"),codec=r.Ros2Aimdk_msgsSrvSetMcAction_ResponseCodec,value=codec.decode(input);
check(value.response.header.stamp.sec === 169392815);
check(value.response.header.stamp.nanosec === 41957691);
check(value.response.header.code === -9120156941344454612n);
check(value.response.status.value === 130970911);
check(value.response.message === "vded9β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_99(){const input=bytes("e1dc1ed564ff8c3919eb0151070000007630386639ceb220845fcc7a3873a9"),codec=r.Ros2Aimdk_msgsSrvSetMcInputSource_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -719397663);
check(value.request.header.stamp.nanosec === 965541732);
check(value.action.value === 1359080217);
check(value.input_source.name === "v08f9β");
check(value.input_source.priority === -866155488);
check(value.input_source.timeout === -1452066694);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_100(){const input=bytes("dc64493ed322d40688507bf331294c6d83b456d53acd614b07a8f5dc"),codec=r.Ros2Aimdk_msgsSrvSetMcInputSource_ResponseCodec,value=codec.decode(input);
check(value.response.header.stamp.sec === 1044997340);
check(value.response.header.stamp.nanosec === 114565843);
check(value.response.header.code === 7875715142879236232n);
check(value.response.task_id === 5431848278156555395n);
check(value.response.state.value === -587880441);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_101(){const input=bytes("dfe0f0a55c978a2517dd2cabccdb867500070000007632633961ceb23d5331ae842a0630"),codec=r.Ros2Aimdk_msgsSrvSetMcPresetMotion_RequestCodec,value=codec.decode(input);
check(value.header.stamp.sec === -1510940449);
check(value.header.stamp.nanosec === 629839708);
check(value.area.value === -1423123177);
check(value.motion.value === 1971772364);
check(value.interrupt === false);
check(value.ani_path === "v2c9aβ");
check(value.play_timestamp === 3460500113027322685n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_102(){const input=bytes("f5dbfa9364cdbe0af9a4971d40dda08331ebc0438892f7dd765c1932"),codec=r.Ros2Aimdk_msgsSrvSetMcPresetMotion_ResponseCodec,value=codec.decode(input);
check(value.response.header.stamp.sec === -1812276235);
check(value.response.header.stamp.nanosec === 180276580);
check(value.response.header.code === -8961919991023164167n);
check(value.response.task_id === 15994413715579530033n);
check(value.response.state.value === 840522870);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_103(){const input=bytes("03d1e38f9ef4252bb61b1c57"),codec=r.Ros2Aimdk_msgsSrvSetMicSourceRequest_RequestCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -1880895229);
check(value.header.header.stamp.nanosec === 723907742);
check(value.mic_source === 1461459894);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_104(){const input=bytes("2933f3b066ec323bed3978ea54404f74c8a658eb070000007637316262ceb2"),codec=r.Ros2Aimdk_msgsSrvSetMicSourceRequest_ResponseCodec,value=codec.decode(input);
check(value.header.header.stamp.sec === -1326238935);
check(value.header.header.stamp.nanosec === 993193062);
check(value.header.header.code === 8380988165014960621n);
check(value.header.status.value === -346511672);
check(value.header.message === "v71bbβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_105(){const input=bytes("b2a0f11c2b0f5a1701"),codec=r.Ros2Aimdk_msgsSrvSetMute_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === 485597362);
check(value.request.header.stamp.nanosec === 391778091);
check(value.is_mute === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_106(){const input=bytes("60d9a5be72edec0c49f2bc65f08b8bfbf2056c3a070000007634653835ceb201"),codec=r.Ros2Aimdk_msgsSrvSetMute_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -1096427168);
check(value.reponse.header.stamp.nanosec === 216853874);
check(value.reponse.header.code === -321009083811564983n);
check(value.reponse.status.value === 980157938);
check(value.reponse.message === "v4e85β");
check(value.is_mute === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_107(){const input=bytes("2d79f205650b472f070000007634613962ceb2d302e9666102309600"),codec=r.Ros2Aimdk_msgsSrvSetPmuLed_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === 99776813);
check(value.request.header.stamp.nanosec === 793185125);
check(value.trace_id === "v4a9bβ");
check(value.led_strip_mode === 211);
check(value.r === 2);
check(value.g === 233);
check(value.b === 102);
check(value.priority === -1775238559);
check(value.reset_priority === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_108(){const input=bytes("fa217ec38fc9392b97f462242e8d55ba4893"),codec=r.Ros2Aimdk_msgsSrvSetPmuLed_ResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === -1015143942);
check(value.header.stamp.nanosec === 725207439);
check(value.header.code === -5019950980316072809n);
check(value.status_code === 37704);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_109(){const input=bytes("014198c4c0f25f164b1e246a"),codec=r.Ros2Aimdk_msgsSrvSetVolume_RequestCodec,value=codec.decode(input);
check(value.request.header.stamp.sec === -996654847);
check(value.request.header.stamp.nanosec === 375386816);
check(value.audio_volume === 1780751947);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_110(){const input=bytes("b75215ec53a04b19d309a1446a7096da5362972a070000007665623661ceb2836cf7ed"),codec=r.Ros2Aimdk_msgsSrvSetVolume_ResponseCodec,value=codec.decode(input);
check(value.reponse.header.stamp.sec === -334146889);
check(value.reponse.header.stamp.nanosec === 424386643);
check(value.reponse.header.code === -2695843725214414381n);
check(value.reponse.status.value === 714564179);
check(value.reponse.message === "veb6aβ");
check(value.audio_volume === 3992415363);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_111(){const input=bytes("072b89b550057009"),codec=r.Ros2Builtin_interfacesMsgTimeCodec,value=codec.decode(input);
check(value.sec === -1249301753);
check(value.nanosec === 158336336);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_112(){const input=bytes("0000000000e86dc00000000000d054c000000000002042c0"),codec=r.Ros2Geometry_msgsMsgPointCodec,value=codec.decode(input);
check(value.x === -239.25);
check(value.y === -83.25);
check(value.z === -36.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_113(){const input=bytes("0000a041000086420000a841"),codec=r.Ros2Geometry_msgsMsgPoint32Codec,value=codec.decode(input);
check(value.x === 20.0);
check(value.y === 67.0);
check(value.z === 21.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_114(){const input=bytes("01000000008065c3008048c300a04b43"),codec=r.Ros2Geometry_msgsMsgPolygonCodec,value=codec.decode(input);
check(value.points.length == 1);
check(value.points[0].x === -229.5);
check(value.points[0].y === -200.5);
check(value.points[0].z === 203.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_115(){const input=bytes("0000000000c02ec000000000003047c00000000000203d4000000000004857c000000000007049400000000000986dc00000000000585340"),codec=r.Ros2Geometry_msgsMsgPoseCodec,value=codec.decode(input);
check(value.position.x === -15.375);
check(value.position.y === -46.375);
check(value.position.z === 29.125);
check(value.orientation.x === -93.125);
check(value.orientation.y === 50.875);
check(value.orientation.z === -236.75);
check(value.orientation.w === 77.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_116(){const input=bytes("00000000008014c00000000000e03e400000000000b46440"),codec=r.Ros2Geometry_msgsMsgPose2DCodec,value=codec.decode(input);
check(value.x === -5.125);
check(value.y === 30.875);
check(value.theta === 165.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_117(){const input=bytes("0000000000a85cc00000000000046dc00000000000d045c00000000000005340"),codec=r.Ros2Geometry_msgsMsgQuaternionCodec,value=codec.decode(input);
check(value.x === -114.625);
check(value.y === -232.125);
check(value.z === -43.625);
check(value.w === 76.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_118(){const input=bytes("69e506ca03882d02002076c39e36eabd66961a4900000000000c66c000000000000068400000000000a048400000000000405f400000000000604ec00000000000c04a400000000000d05440"),codec=r.Ros2Nav_msgsMsgMapMetaDataCodec,value=codec.decode(input);
check(value.map_load_time.sec === -905517719);
check(value.map_load_time.nanosec === 36538371);
check(value.resolution === -246.125);
check(value.width === 3186243230);
check(value.height === 1226479206);
check(value.origin.position.x === -176.375);
check(value.origin.position.y === 192.0);
check(value.origin.position.z === 49.25);
check(value.origin.orientation.x === 125.0);
check(value.origin.orientation.y === -60.75);
check(value.origin.orientation.z === 53.5);
check(value.origin.orientation.w === 83.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_119(){const input=bytes("681252e4acc1ba37070000007666316532ceb2"),codec=r.Ros2Std_msgsMsgHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -464383384);
check(value.stamp.nanosec === 934986156);
check(value.frame_id === "vf1e2β");
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
test_72();
test_73();
test_74();
test_75();
test_76();
test_77();
test_78();
test_79();
test_80();
test_81();
test_82();
test_83();
test_84();
test_85();
test_86();
test_87();
test_88();
test_89();
test_90();
test_91();
test_92();
test_93();
test_94();
test_95();
test_96();
test_97();
test_98();
test_99();
test_100();
test_101();
test_102();
test_103();
test_104();
test_105();
test_106();
test_107();
test_108();
test_109();
test_110();
test_111();
test_112();
test_113();
test_114();
test_115();
test_116();
test_117();
test_118();
test_119();
return 0;
}
