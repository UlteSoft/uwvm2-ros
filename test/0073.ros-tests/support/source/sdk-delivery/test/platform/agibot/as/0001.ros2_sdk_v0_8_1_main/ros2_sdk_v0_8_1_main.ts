/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../../../../sdk/include/platform/agibot/as/ros2/agibot-ros2-sdk-v0-8-1-main-complete/bounded_records";
import {abortGuest} from "../../../../../../../../../../sdk/include/platform/ros/as/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("010000002a070000007635303531ceb2");const codec=new r.Ros2Aimdk_msgsMsgAgentPropertiesCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.properties.length == 1);
assert(value.properties[0].key.value == <u8>(42));
assert(value.properties[0].value == "v5051β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("49070000007633356565ceb2");const codec=new r.Ros2Aimdk_msgsMsgAgentPropertiesValueCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.key.value == <u8>(73));
assert(value.value == "v35eeβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("d8");const codec=new r.Ros2Aimdk_msgsMsgAgentPropertyIdTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(216));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("9b");const codec=new r.Ros2Aimdk_msgsMsgAudioVadStateTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(155));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("16bb4308af59a921");const codec=new r.Ros2Aimdk_msgsMsgCommonRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(138656534));
assert(value.header.stamp.nanosec == <u32>(564746671));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("7e48d16d857af630efb3162656ca21b86ea99a05070000007634616561ceb2");const codec=new r.Ros2Aimdk_msgsMsgCommonResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1842432126));
assert(value.header.stamp.nanosec == <u32>(821459589));
assert(value.header.code == <i64><u64>0xb821ca562616b3ef);
assert(value.status.value == <i32>(94021998));
assert(value.message == "v4aeaβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("d226915e");const codec=new r.Ros2Aimdk_msgsMsgCommonStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1586570962));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("0e92b1f4b82af9317ff79a4fc68eadd1888295fa31fba264f00f1460");const codec=new r.Ros2Aimdk_msgsMsgCommonTaskResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-189689330));
assert(value.header.stamp.nanosec == <u32>(838412984));
assert(value.header.code == <i64><u64>0xd1ad8ec64f9af77f);
assert(value.task_id == <u64><u64>0x64a2fb31fa958288);
assert(value.state.value == <i32>(1611927536));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("1c4abca49d92eb07070000007663306231ceb2bd7094aded41fe734e14bc34070000007666313762ceb201000000070000007665653538ceb22581536c49895700000000000c6140");const codec=new r.Ros2Aimdk_msgsMsgFaceEmojiStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-1531164132));
assert(value.header.stamp.nanosec == <u32>(132879005));
assert(value.header.frame_id == "vc0b1β");
assert(value.header.sequence == <u32>(2912186557));
assert(value.header.meas_stamp.sec == <i32>(1946042861));
assert(value.header.meas_stamp.nanosec == <u32>(884741198));
assert(value.e_path == "vf17bβ");
assert(value.e_path_list.length == 1);
assert(value.e_path_list[0] == "vee58β");
assert(value.e_id == <u8>(37));
assert(value.mode == <u8>(129));
assert(value.priority == <i32>(-1991676845));
assert(value.status == <u8>(87));
assert(value.time_to_end_ms == <f64>(136.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("070000007636623363ceb20000000000e054c00000000000b46e400000000000a037c0000000000028624000000000002c6b40");const codec=new r.Ros2Aimdk_msgsMsgHandCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "v6b3cβ");
assert(value.position == <f64>(-83.5));
assert(value.velocity == <f64>(245.625));
assert(value.acceleration == <f64>(-23.625));
assert(value.deceleration == <f64>(145.25));
assert(value.effort == <f64>(217.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("0a807d12feed941e070000007636373536ceb2825b730ae4d85ca71dacb439fa01000000070000007665323962ceb20000000000000ec0000000000038604000000000004c6ac00000000000946ac00000000000e068c09a01000000070000007666366562ceb20000000000905c40000000000090504000000000006469c000000000008047c000000000000c62c0");const codec=new r.Ros2Aimdk_msgsMsgHandCommandArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(310214666));
assert(value.header.stamp.nanosec == <u32>(513076734));
assert(value.header.frame_id == "v6756β");
assert(value.header.sequence == <u32>(175332226));
assert(value.header.meas_stamp.sec == <i32>(-1487087388));
assert(value.header.meas_stamp.nanosec == <u32>(968141853));
assert(value.left_hand_type.value == <u8>(250));
assert(value.left_hands.length == 1);
assert(value.left_hands[0].name == "ve29bβ");
assert(value.left_hands[0].position == <f64>(-3.75));
assert(value.left_hands[0].velocity == <f64>(129.75));
assert(value.left_hands[0].acceleration == <f64>(-210.375));
assert(value.left_hands[0].deceleration == <f64>(-212.625));
assert(value.left_hands[0].effort == <f64>(-199.0));
assert(value.right_hand_type.value == <u8>(154));
assert(value.right_hands.length == 1);
assert(value.right_hands[0].name == "vf6ebβ");
assert(value.right_hands[0].position == <f64>(114.25));
assert(value.right_hands[0].velocity == <f64>(66.25));
assert(value.right_hands[0].acceleration == <f64>(-203.125));
assert(value.right_hands[0].deceleration == <f64>(-47.0));
assert(value.right_hands[0].effort == <f64>(-144.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("070000007663636334ceb20000000000f8574000000000000c614000000000009c6840f0e70c745ba49b55");const codec=new r.Ros2Aimdk_msgsMsgHandStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vccc4β");
assert(value.position == <f64>(95.875));
assert(value.velocity == <f64>(136.375));
assert(value.effort == <f64>(196.875));
assert(value.state == <i32>(1947002864));
assert(value.faultcode == <i32>(1436263515));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("153e6a78fd2f7135070000007663306338ceb293b6edadc6d73a3e54d7710bf901000000070000007630333138ceb20000000000e03b400000000000b04ec00000000000346240434ccfb8b8fd74fdb801000000070000007631383034ceb20000000000286e4000000000000017c00000000000b064401dfed4b2d38d9f59");const codec=new r.Ros2Aimdk_msgsMsgHandStateArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(2020228629));
assert(value.header.stamp.nanosec == <u32>(896610301));
assert(value.header.frame_id == "vc0c8β");
assert(value.header.sequence == <u32>(2918037139));
assert(value.header.meas_stamp.sec == <i32>(1044043718));
assert(value.header.meas_stamp.nanosec == <u32>(192010068));
assert(value.left_hand_type.value == <u8>(249));
assert(value.left_hands.length == 1);
assert(value.left_hands[0].name == "v0318β");
assert(value.left_hands[0].position == <f64>(27.875));
assert(value.left_hands[0].velocity == <f64>(-61.375));
assert(value.left_hands[0].effort == <f64>(145.625));
assert(value.left_hands[0].state == <i32>(-1194374077));
assert(value.left_hands[0].faultcode == <i32>(-42664520));
assert(value.right_hand_type.value == <u8>(184));
assert(value.right_hands.length == 1);
assert(value.right_hands[0].name == "v1804β");
assert(value.right_hands[0].position == <f64>(241.25));
assert(value.right_hands[0].velocity == <f64>(-5.75));
assert(value.right_hands[0].effort == <f64>(165.5));
assert(value.right_hands[0].state == <i32>(-1294664163));
assert(value.right_hands[0].faultcode == <i32>(1503628755));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("25");const codec=new r.Ros2Aimdk_msgsMsgHandTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(37));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("070000007666396635ceb2000000000030644000000000006059c000000000006856400000000000b4654000000000003043c0");const codec=new r.Ros2Aimdk_msgsMsgJointCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vf9f5β");
assert(value.position == <f64>(161.5));
assert(value.velocity == <f64>(-101.5));
assert(value.effort == <f64>(89.625));
assert(value.stiffness == <f64>(173.625));
assert(value.damping == <f64>(-38.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("c4e2ccfa36661122070000007636656530ceb2c5a6c503155c6d32fa7a343001000000070000007631646364ceb200000000002855c00000000000fc62400000000000886dc0000000000028644000000000004866c0");const codec=new r.Ros2Aimdk_msgsMsgJointCommandArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-87235900));
assert(value.header.stamp.nanosec == <u32>(571565622));
assert(value.header.frame_id == "v6ee0β");
assert(value.header.sequence == <u32>(63284933));
assert(value.header.meas_stamp.sec == <i32>(846027797));
assert(value.header.meas_stamp.nanosec == <u32>(808745722));
assert(value.joints.length == 1);
assert(value.joints[0].name == "v1dcdβ");
assert(value.joints[0].position == <f64>(-84.625));
assert(value.joints[0].velocity == <f64>(151.875));
assert(value.joints[0].effort == <f64>(-236.25));
assert(value.joints[0].stiffness == <f64>(161.25));
assert(value.joints[0].damping == <f64>(-178.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("070000007634386539ceb200000000008041c00000000000bc64400000000000bc6540bfcbe3");const codec=new r.Ros2Aimdk_msgsMsgJointStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "v48e9β");
assert(value.position == <f64>(-35.0));
assert(value.velocity == <f64>(165.875));
assert(value.effort == <f64>(173.875));
assert(value.coil_temp == <u8>(191));
assert(value.motor_temp == <u8>(203));
assert(value.motor_vol == <u8>(227));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("1952db78f8dfe310070000007637316134ceb293ba811c5be6a64ea09f3a2a01000000070000007638333532ceb20000000000c855c000000000003867400000000000c038c05a2c06");const codec=new r.Ros2Aimdk_msgsMsgJointStateArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(2027639321));
assert(value.header.stamp.nanosec == <u32>(283369464));
assert(value.header.frame_id == "v71a4β");
assert(value.header.sequence == <u32>(478263955));
assert(value.header.meas_stamp.sec == <i32>(1319560795));
assert(value.header.meas_stamp.nanosec == <u32>(708485024));
assert(value.joints.length == 1);
assert(value.joints[0].name == "v8352β");
assert(value.joints[0].position == <f64>(-87.125));
assert(value.joints[0].velocity == <f64>(185.75));
assert(value.joints[0].effort == <f64>(-24.75));
assert(value.joints[0].coil_temp == <u8>(90));
assert(value.joints[0].motor_temp == <u8>(44));
assert(value.joints[0].motor_vol == <u8>(6));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("9e616d45");const codec=new r.Ros2Aimdk_msgsMsgMcActionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1164796318));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("6ff83014070000007633656562ceb2");const codec=new r.Ros2Aimdk_msgsMsgMcActionCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.action.value == <i32>(338753647));
assert(value.action_desc == "v3eebβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("091b656e070000007630343534ceb2543effca");const codec=new r.Ros2Aimdk_msgsMsgMcActionInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.current_action.value == <i32>(1852119817));
assert(value.action_desc == "v0454β");
assert(value.status.value == <i32>(-889242028));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("03a0131b");const codec=new r.Ros2Aimdk_msgsMsgMcActionStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(454270979));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("c61a340d");const codec=new r.Ros2Aimdk_msgsMsgMcControlAreaCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(221518534));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("64436340");const codec=new r.Ros2Aimdk_msgsMsgMcInputActionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1080247140));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("070000007664636264ceb2d4e7a260518ce9e8");const codec=new r.Ros2Aimdk_msgsMsgMcInputSourceCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vdcbdβ");
assert(value.priority == <i32>(1621288916));
assert(value.timeout == <i32>(-387347375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_25():void{const input=bytes("1da7c0703c8e991c070000007630373663ceb2de93496a976034d2db683806070000007630363634ceb200000000000c69c0000000000038634000000000003064c0");const codec=new r.Ros2Aimdk_msgsMsgMcLocomotionVelocityCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1891673885));
assert(value.header.stamp.nanosec == <u32>(479825468));
assert(value.header.frame_id == "v076cβ");
assert(value.header.sequence == <u32>(1783206878));
assert(value.header.meas_stamp.sec == <i32>(-768319337));
assert(value.header.meas_stamp.nanosec == <u32>(104360155));
assert(value.source == "v0664β");
assert(value.forward_velocity == <f64>(-200.375));
assert(value.lateral_velocity == <f64>(153.75));
assert(value.angular_velocity == <f64>(-161.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_26():void{const input=bytes("bbd1e14c");const codec=new r.Ros2Aimdk_msgsMsgMcPresetMotionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1289867707));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_27():void{const input=bytes("5455aec10663d513070000007636323737ceb2e5fe520f50fd2f14d8db8710");const codec=new r.Ros2Aimdk_msgsMsgMessageHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(-1045539500));
assert(value.stamp.nanosec == <u32>(332751622));
assert(value.frame_id == "v6277β");
assert(value.sequence == <u32>(257097445));
assert(value.meas_stamp.sec == <i32>(338689360));
assert(value.meas_stamp.nanosec == <u32>(277339096));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_28():void{const input=bytes("070000007635663937ceb2f6cf1cd9128e891980070000007666626365ceb2070000007666656536ceb201");const codec=new r.Ros2Aimdk_msgsMsgPlayMediaFileRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.file_name == "v5f97β");
assert(value.sample_rate == <u32>(3642544118));
assert(value.priority_level.value == <u8>(18));
assert(value.priority_weight == <u32>(2149157262));
assert(value.domain == "vfbceβ");
assert(value.trace_id == "vfee6β");
assert(value.is_interrupted == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_29():void{const input=bytes("070000007637643533ceb29905780d6a070000007634396232ceb2070000007632646662ceb201");const codec=new r.Ros2Aimdk_msgsMsgPlayTtsRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "v7d53β");
assert(value.priority_level.value == <u8>(153));
assert(value.priority_weight == <u32>(1779267589));
assert(value.domain == "v49b2β");
assert(value.trace_id == "v2dfbβ");
assert(value.is_interrupted == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_30():void{const input=bytes("070000007639376439ceb22d13a7109d070000007636333533ceb2070000007665306536ceb201070000007637663738ceb20f38fdc7");const codec=new r.Ros2Aimdk_msgsMsgPlayTtsResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "v97d9β");
assert(value.priority_level.value == <u8>(45));
assert(value.priority_weight == <u32>(2635114259));
assert(value.domain == "v6353β");
assert(value.trace_id == "ve0e6β");
assert(value.is_success == true);
assert(value.error_message == "v7f78β");
assert(value.estimated_duration == <u32>(3355260943));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_31():void{const input=bytes("070000007664623661ceb2070000007634316663ceb2070000007663383133ceb2a9e007c500000000008c6640000000000048584000000000003859c000000000002c63c00000000000203a4000000000004050c000000000004060c00000000000806cc00000000000cc644000000000003858400000000000504a40000000000048614000000000009c6b400000000000485bc000000000003c68c0070000007665386262ceb2070000007631333430ceb2070000007636353838ceb2070000007666663165ceb2f957209c893c0000000000685140000000000070404000000000007053400000000000b857403de21c6435561304202a24");const codec=new r.Ros2Aimdk_msgsMsgPmuStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pmu_software_version == "vdb6aβ");
assert(value.pmu_hardware_version == "v41fcβ");
assert(value.pmu_protocol_version == "vc813β");
assert(value.pmu_bool_status == <u32>(3305627817));
assert(value.head_power_current == <f64>(180.375));
assert(value.output_48v_current == <f64>(97.125));
assert(value.rk3588_current == <f64>(-100.875));
assert(value.output_12v_current == <f64>(-153.375));
assert(value.bus_48v_current == <f64>(26.125));
assert(value.orin_current == <f64>(-65.0));
assert(value.bus_48v_pmos_voltage == <f64>(-130.0));
assert(value.battery_voltage == <f64>(-228.0));
assert(value.fan_voltage == <f64>(166.375));
assert(value.output_12v_voltage == <f64>(96.875));
assert(value.output_48v_voltage == <f64>(52.625));
assert(value.bus_48v_voltage == <f64>(138.25));
assert(value.head_power_voltage == <f64>(220.875));
assert(value.orin_voltage == <f64>(-109.125));
assert(value.rk3588_voltage == <f64>(-193.875));
assert(value.bms_manufacturer == "ve8bbβ");
assert(value.bms_serial_number == "v1340β");
assert(value.bms_hardware_version == "v6588β");
assert(value.bms_software_version == "vff1eβ");
assert(value.bms_status_bits == <u32>(2619365369));
assert(value.battery_balance_line_resistance == <u16>(15497));
assert(value.battery_pack_voltage == <f64>(69.625));
assert(value.battery_current == <f64>(32.875));
assert(value.battery_output_power == <f64>(77.75));
assert(value.battery_temperature == <f64>(94.875));
assert(value.battery_remaining_capacity == <u32>(1679614525));
assert(value.battery_remaining_capacity_percentage == <u8>(53));
assert(value.battery_cycle_count == <u16>(4950));
assert(value.battery_cycle_total_capacity == <u32>(606740484));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_32():void{const input=bytes("74f6ac96d49fda04070000007630383631ceb291fb4465427b4e7c7214af30d8042faf470100000002");const codec=new r.Ros2Aimdk_msgsMsgProcessedAudioOutputCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-1767049612));
assert(value.header.stamp.nanosec == <u32>(81436628));
assert(value.header.frame_id == "v0861β");
assert(value.header.sequence == <u32>(1699019665));
assert(value.header.meas_stamp.sec == <i32>(2085518146));
assert(value.header.meas_stamp.nanosec == <u32>(816780402));
assert(value.stream_id == <u32>(2939094232));
assert(value.audio_vad_state.value == <u8>(71));
assert(value.audio_data.length == 1);
assert(value.audio_data[0] == <u8>(2));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_33():void{const input=bytes("7ec27b1a2f62b81e");const codec=new r.Ros2Aimdk_msgsMsgRequestHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(444318334));
assert(value.stamp.nanosec == <u32>(515400239));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_34():void{const input=bytes("55c42dd4c7edd33a863659073f4f6e99");const codec=new r.Ros2Aimdk_msgsMsgResponseHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(-735198123));
assert(value.stamp.nanosec == <u32>(986967495));
assert(value.code == <i64><u64>0x996e4f3f07593686);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_35():void{const input=bytes("d7825fed79ab2507070000007634613365ceb25bf61b277bc4ddd4778e3f220152585ca1136947b8d03a6a93910b718a569d30c517ac2bdcd4ff06f795ce1deed3bcbd4e928da65751de8b7c10ef9065d779d12a9648ca33551be718142afc010100010001000100");const codec=new r.Ros2Aimdk_msgsMsgTouchStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-312507689));
assert(value.header.stamp.nanosec == <u32>(119909241));
assert(value.header.frame_id == "v4a3eβ");
assert(value.header.sequence == <u32>(656143963));
assert(value.header.meas_stamp.sec == <i32>(-723663749));
assert(value.header.meas_stamp.nanosec == <u32>(574590583));
assert(value.event_type == <u8>(1));
assert(value.data.length == 8);
assert(value.data[0] == <u32>(2707183698));
assert(value.data[1] == <u32>(3091687699));
assert(value.data[2] == <u32>(2473212624));
assert(value.data[3] == <u32>(2322664337));
assert(value.data[4] == <u32>(3308297558));
assert(value.data[5] == <u32>(3693849623));
assert(value.data[6] == <u32>(4144431060));
assert(value.data[7] == <u32>(3994930837));
assert(value.threshold.length == 8);
assert(value.threshold[0] == <u32>(1321057491));
assert(value.threshold[1] == <u32>(1470533010));
assert(value.threshold[2] == <u32>(2089541201));
assert(value.threshold[3] == <u32>(1703997200));
assert(value.threshold[4] == <u32>(718371287));
assert(value.threshold[5] == <u32>(868894870));
assert(value.threshold[6] == <u32>(417798997));
assert(value.threshold[7] == <u32>(33303060));
assert(value.is_touched.length == 8);
assert(value.is_touched[0] == true);
assert(value.is_touched[1] == false);
assert(value.is_touched[2] == true);
assert(value.is_touched[3] == false);
assert(value.is_touched[4] == true);
assert(value.is_touched[5] == false);
assert(value.is_touched[6] == true);
assert(value.is_touched[7] == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_36():void{const input=bytes("bc");const codec=new r.Ros2Aimdk_msgsMsgTtsPriorityLevelCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(188));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_37():void{const input=bytes("42176be292645b22");const codec=new r.Ros2Aimdk_msgsSrvGetAllJointState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-496298174));
assert(value.request.header.stamp.nanosec == <u32>(576414866));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_38():void{const input=bytes("cbb1e60319f8d70d158016abacf921355fb27f58070000007635353831ceb201000000070000007630323635ceb20000000000205ac000000000008018c0000000000090504080cddc01000000070000007634366630ceb20000000000e060c000000000001859400000000000c466c0a9abf501000000070000007665366135ceb200000000002064c000000000001054c0000000000020464078b92401000000070000007637363262ceb20000000000a8544000000000004057400000000000d05e40c20f9e");const codec=new r.Ros2Aimdk_msgsSrvGetAllJointState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(65450443));
assert(value.reponse.header.stamp.nanosec == <u32>(232257561));
assert(value.reponse.header.code == <i64><u64>0x3521f9acab168015);
assert(value.reponse.status.value == <i32>(1484763743));
assert(value.reponse.message == "v5581β");
assert(value.head_joints.length == 1);
assert(value.head_joints[0].name == "v0265β");
assert(value.head_joints[0].position == <f64>(-104.5));
assert(value.head_joints[0].velocity == <f64>(-6.125));
assert(value.head_joints[0].effort == <f64>(66.25));
assert(value.head_joints[0].coil_temp == <u8>(128));
assert(value.head_joints[0].motor_temp == <u8>(205));
assert(value.head_joints[0].motor_vol == <u8>(220));
assert(value.arm_joints.length == 1);
assert(value.arm_joints[0].name == "v46f0β");
assert(value.arm_joints[0].position == <f64>(-135.0));
assert(value.arm_joints[0].velocity == <f64>(100.375));
assert(value.arm_joints[0].effort == <f64>(-182.125));
assert(value.arm_joints[0].coil_temp == <u8>(169));
assert(value.arm_joints[0].motor_temp == <u8>(171));
assert(value.arm_joints[0].motor_vol == <u8>(245));
assert(value.waist_joints.length == 1);
assert(value.waist_joints[0].name == "ve6a5β");
assert(value.waist_joints[0].position == <f64>(-161.0));
assert(value.waist_joints[0].velocity == <f64>(-80.25));
assert(value.waist_joints[0].effort == <f64>(44.25));
assert(value.waist_joints[0].coil_temp == <u8>(120));
assert(value.waist_joints[0].motor_temp == <u8>(185));
assert(value.waist_joints[0].motor_vol == <u8>(36));
assert(value.leg_joints.length == 1);
assert(value.leg_joints[0].name == "v762bβ");
assert(value.leg_joints[0].position == <f64>(82.625));
assert(value.leg_joints[0].velocity == <f64>(93.0));
assert(value.leg_joints[0].effort == <f64>(123.25));
assert(value.leg_joints[0].coil_temp == <u8>(194));
assert(value.leg_joints[0].motor_temp == <u8>(15));
assert(value.leg_joints[0].motor_vol == <u8>(158));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_39():void{const input=bytes("356cda9cc4940907");const codec=new r.Ros2Aimdk_msgsSrvGetCurrentInputSource_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-1663407051));
assert(value.request.header.stamp.nanosec == <u32>(118068420));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_40():void{const input=bytes("8f9e7c10e0656a2587abc65d3ed2f1c340b51964f9cc2efa08534872070000007666656432ceb2d91539cd3bc79314");const codec=new r.Ros2Aimdk_msgsSrvGetCurrentInputSource_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.header.stamp.sec == <i32>(276602511));
assert(value.response.header.stamp.nanosec == <u32>(627729888));
assert(value.response.header.code == <i64><u64>0xc3f1d23e5dc6ab87);
assert(value.response.task_id == <u64><u64>0xfa2eccf96419b540);
assert(value.response.state.value == <i32>(1917342472));
assert(value.input_source.name == "vfed2β");
assert(value.input_source.priority == <i32>(-851896871));
assert(value.input_source.timeout == <i32>(345229115));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_41():void{const input=bytes("216fa6ac032ce70e");const codec=new r.Ros2Aimdk_msgsSrvGetHandType_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-1398378719));
assert(value.request.header.stamp.nanosec == <u32>(250031107));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_42():void{const input=bytes("514c13d76fc2151cd7889bf26ef1ac6ccb9c4c2d070000007638303934ceb2a04f");const codec=new r.Ros2Aimdk_msgsSrvGetHandType_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-686601135));
assert(value.reponse.header.stamp.nanosec == <u32>(471188079));
assert(value.reponse.header.code == <i64><u64>0x6cacf16ef29b88d7);
assert(value.reponse.status.value == <i32>(759995595));
assert(value.reponse.message == "v8094β");
assert(value.left_hands_type.value == <u8>(160));
assert(value.right_hands_type.value == <u8>(79));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_43():void{const input=bytes("16b2cbc742412923");const codec=new r.Ros2Aimdk_msgsSrvGetMcAction_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-942951914));
assert(value.request.header.stamp.nanosec == <u32>(589906242));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_44():void{const input=bytes("371ceb02ee57961fe9cb311a50b20684cb955833070000007630626164ceb2d172243e");const codec=new r.Ros2Aimdk_msgsSrvGetMcAction_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(48962615));
assert(value.header.stamp.nanosec == <u32>(529946606));
assert(value.header.code == <i64><u64>0x8406b2501a31cbe9);
assert(value.info.current_action.value == <i32>(861443531));
assert(value.info.action_desc == "v0badβ");
assert(value.info.status.value == <i32>(1042576081));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_45():void{const input=bytes("40cabdd47aeddb2e");const codec=new r.Ros2Aimdk_msgsSrvGetMute_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-725759424));
assert(value.request.header.stamp.nanosec == <u32>(786165114));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_46():void{const input=bytes("3a06deedcacac334f4fabbbf4d838c21cbcd668f070000007630326338ceb201");const codec=new r.Ros2Aimdk_msgsSrvGetMute_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-304216518));
assert(value.reponse.header.stamp.nanosec == <u32>(885246666));
assert(value.reponse.header.code == <i64><u64>0x218c834dbfbbfaf4);
assert(value.reponse.status.value == <i32>(-1889088053));
assert(value.reponse.message == "v02c8β");
assert(value.is_mute == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_47():void{const input=bytes("3482754f43b7261d");const codec=new r.Ros2Aimdk_msgsSrvGetVolume_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(1333101108));
assert(value.request.header.stamp.nanosec == <u32>(489076547));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_48():void{const input=bytes("577b2dba3e2c9f05138b111baaf226856110acb1070000007638363464ceb2a22c91c9");const codec=new r.Ros2Aimdk_msgsSrvGetVolume_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-1171424425));
assert(value.reponse.header.stamp.nanosec == <u32>(94317630));
assert(value.reponse.header.code == <i64><u64>0x8526f2aa1b118b13);
assert(value.reponse.status.value == <i32>(-1314123679));
assert(value.reponse.message == "v864dβ");
assert(value.audio_volume == <u32>(3381734562));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_49():void{const input=bytes("9a8f2cad6fd34d14c2907bf4");const codec=new r.Ros2Aimdk_msgsSrvLedStripCommand_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-1389588582));
assert(value.request.header.stamp.nanosec == <u32>(340644719));
assert(value.led_strip_mode == <u8>(194));
assert(value.r == <u8>(144));
assert(value.g == <u8>(123));
assert(value.b == <u8>(244));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_50():void{const input=bytes("8a74dbe32c035c37f42d67754d5450eb2b4a");const codec=new r.Ros2Aimdk_msgsSrvLedStripCommand_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-472157046));
assert(value.header.stamp.nanosec == <u32>(928777004));
assert(value.header.code == <i64><u64>0xeb50544d75672df4);
assert(value.status_code == <u16>(18987));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_51():void{const input=bytes("fb206e27ed8a9b1302cbe66ca555");const codec=new r.Ros2Aimdk_msgsSrvPlayEmoji_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(661528827));
assert(value.header.header.stamp.nanosec == <u32>(328960749));
assert(value.emotion_id == <u8>(2));
assert(value.mode == <u8>(203));
assert(value.priority == <i32>(1436904678));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_52():void{const input=bytes("f6c43d030ac08f040abdee5cb3c4d9c2974780d5070000007635323838ceb201070000007632373339ceb2");const codec=new r.Ros2Aimdk_msgsSrvPlayEmoji_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(54379766));
assert(value.header.header.stamp.nanosec == <u32>(76529674));
assert(value.header.header.code == <i64><u64>0xc2d9c4b35ceebd0a);
assert(value.header.status.value == <i32>(-713013353));
assert(value.header.message == "v5288β");
assert(value.success == true);
assert(value.message == "v2739β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_53():void{const input=bytes("02d555892c399c1c070000007638313562ceb26af2beda7d802e045e070000007663636532ceb2070000007638396235ceb200");const codec=new r.Ros2Aimdk_msgsSrvPlayMediaFile_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1990863614));
assert(value.header.header.stamp.nanosec == <u32>(480000300));
assert(value.media_file_req.file_name == "v815bβ");
assert(value.media_file_req.sample_rate == <u32>(3669946986));
assert(value.media_file_req.priority_level.value == <u8>(125));
assert(value.media_file_req.priority_weight == <u32>(1577332352));
assert(value.media_file_req.domain == "vcce2β");
assert(value.media_file_req.trace_id == "v89b5β");
assert(value.media_file_req.is_interrupted == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_54():void{const input=bytes("bbb5f2c7a587ad20b04b719c09324602f382f4fe070000007666336166ceb2070000007633303462ceb299f05c1b6c070000007638316232ceb2070000007634646464ceb201070000007630363336ceb2f9b7c30c");const codec=new r.Ros2Aimdk_msgsSrvPlayMediaFile_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-940395077));
assert(value.header.header.stamp.nanosec == <u32>(548243365));
assert(value.header.header.code == <i64><u64>0x024632099c714bb0);
assert(value.header.status.value == <i32>(-17530125));
assert(value.header.message == "vf3afβ");
assert(value.tts_resp.text == "v304bβ");
assert(value.tts_resp.priority_level.value == <u8>(153));
assert(value.tts_resp.priority_weight == <u32>(1813732592));
assert(value.tts_resp.domain == "v81b2β");
assert(value.tts_resp.trace_id == "v4dddβ");
assert(value.tts_resp.is_success == true);
assert(value.tts_resp.error_message == "v0636β");
assert(value.tts_resp.estimated_duration == <u32>(214153209));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_55():void{const input=bytes("16fddc490ed7d11b070000007639383961ceb213d9d1d86c070000007637343130ceb2070000007634343863ceb200");const codec=new r.Ros2Aimdk_msgsSrvPlayTts_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(1239219478));
assert(value.header.header.stamp.nanosec == <u32>(466736910));
assert(value.tts_req.text == "v989aβ");
assert(value.tts_req.priority_level.value == <u8>(19));
assert(value.tts_req.priority_weight == <u32>(1826148825));
assert(value.tts_req.domain == "v7410β");
assert(value.tts_req.trace_id == "v448cβ");
assert(value.tts_req.is_interrupted == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_56():void{const input=bytes("eee8e8dd779080066fe0aca5d6999b3b1d90a1e8070000007636303330ceb2070000007661363062ceb2fad6e30077070000007662653934ceb2070000007639336335ceb201070000007663366266ceb22bf63bd9");const codec=new r.Ros2Aimdk_msgsSrvPlayTts_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-571938578));
assert(value.header.header.stamp.nanosec == <u32>(109088887));
assert(value.header.header.code == <i64><u64>0x3b9b99d6a5ace06f);
assert(value.header.status.value == <i32>(-392064995));
assert(value.header.message == "v6030β");
assert(value.tts_resp.text == "va60bβ");
assert(value.tts_resp.priority_level.value == <u8>(250));
assert(value.tts_resp.priority_weight == <u32>(1996547030));
assert(value.tts_resp.domain == "vbe94β");
assert(value.tts_resp.trace_id == "v93c5β");
assert(value.tts_resp.is_success == true);
assert(value.tts_resp.error_message == "vc6bfβ");
assert(value.tts_resp.estimated_duration == <u32>(3644585515));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_57():void{const input=bytes("23679b0a535df030070000007639653337ceb2d417bc5be7");const codec=new r.Ros2Aimdk_msgsSrvPlayVideo_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(177956643));
assert(value.header.header.stamp.nanosec == <u32>(821058899));
assert(value.video_path == "v9e37β");
assert(value.mode == <u8>(212));
assert(value.priority == <i32>(-413418473));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_58():void{const input=bytes("576b188bc2210c1c3d435581843a621f6659e06d070000007663326330ceb200070000007639356337ceb2");const codec=new r.Ros2Aimdk_msgsSrvPlayVideo_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1961333929));
assert(value.header.header.stamp.nanosec == <u32>(470557122));
assert(value.header.header.code == <i64><u64>0x1f623a848155433d);
assert(value.header.status.value == <i32>(1843419494));
assert(value.header.message == "vc2c0β");
assert(value.success == false);
assert(value.message == "v95c7β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_59():void{const input=bytes("38249d9b00ba66140100000035070000007635636336ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetAgentPropertiesRequest_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1684200392));
assert(value.header.header.stamp.nanosec == <u32>(342276608));
assert(value.contents.properties.length == 1);
assert(value.contents.properties[0].key.value == <u8>(53));
assert(value.contents.properties[0].value == "v5cc6β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_60():void{const input=bytes("f8a4ec016be9aa2e2f5836a996210137354b719e070000007661393561ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetAgentPropertiesRequest_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(32285944));
assert(value.header.header.stamp.nanosec == <u32>(782952811));
assert(value.header.header.code == <i64><u64>0x37012196a936582f);
assert(value.header.status.value == <i32>(-1636742347));
assert(value.header.message == "va95aβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_61():void{const input=bytes("7d1b67589f966f04bba8a149070000007663376130ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetMcAction_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1483152253));
assert(value.header.stamp.nanosec == <u32>(74421919));
assert(value.command.action.value == <i32>(1235331259));
assert(value.command.action_desc == "vc7a0β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_62():void{const input=bytes("afba180a3b3980022cc8591f95b16e811f75ce07070000007664656439ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetMcAction_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.header.stamp.sec == <i32>(169392815));
assert(value.response.header.stamp.nanosec == <u32>(41957691));
assert(value.response.header.code == <i64><u64>0x816eb1951f59c82c);
assert(value.response.status.value == <i32>(130970911));
assert(value.response.message == "vded9β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_63():void{const input=bytes("e1dc1ed564ff8c3919eb0151070000007630386639ceb220845fcc7a3873a9");const codec=new r.Ros2Aimdk_msgsSrvSetMcInputSource_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-719397663));
assert(value.request.header.stamp.nanosec == <u32>(965541732));
assert(value.action.value == <i32>(1359080217));
assert(value.input_source.name == "v08f9β");
assert(value.input_source.priority == <i32>(-866155488));
assert(value.input_source.timeout == <i32>(-1452066694));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_64():void{const input=bytes("dc64493ed322d40688507bf331294c6d83b456d53acd614b07a8f5dc");const codec=new r.Ros2Aimdk_msgsSrvSetMcInputSource_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.header.stamp.sec == <i32>(1044997340));
assert(value.response.header.stamp.nanosec == <u32>(114565843));
assert(value.response.header.code == <i64><u64>0x6d4c2931f37b5088);
assert(value.response.task_id == <u64><u64>0x4b61cd3ad556b483);
assert(value.response.state.value == <i32>(-587880441));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_65():void{const input=bytes("dfe0f0a55c978a2517dd2cabccdb867500070000007632633961ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetMcPresetMotion_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-1510940449));
assert(value.header.stamp.nanosec == <u32>(629839708));
assert(value.area.value == <i32>(-1423123177));
assert(value.motion.value == <i32>(1971772364));
assert(value.interrupt == false);
assert(value.ani_path == "v2c9aβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_66():void{const input=bytes("f5dbfa9364cdbe0af9a4971d40dda08331ebc0438892f7dd765c1932");const codec=new r.Ros2Aimdk_msgsSrvSetMcPresetMotion_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.header.stamp.sec == <i32>(-1812276235));
assert(value.response.header.stamp.nanosec == <u32>(180276580));
assert(value.response.header.code == <i64><u64>0x83a0dd401d97a4f9);
assert(value.response.task_id == <u64><u64>0xddf7928843c0eb31);
assert(value.response.state.value == <i32>(840522870));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_67():void{const input=bytes("b2a0f11c2b0f5a1701");const codec=new r.Ros2Aimdk_msgsSrvSetMute_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(485597362));
assert(value.request.header.stamp.nanosec == <u32>(391778091));
assert(value.is_mute == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_68():void{const input=bytes("60d9a5be72edec0c49f2bc65f08b8bfbf2056c3a070000007634653835ceb201");const codec=new r.Ros2Aimdk_msgsSrvSetMute_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-1096427168));
assert(value.reponse.header.stamp.nanosec == <u32>(216853874));
assert(value.reponse.header.code == <i64><u64>0xfb8b8bf065bcf249);
assert(value.reponse.status.value == <i32>(980157938));
assert(value.reponse.message == "v4e85β");
assert(value.is_mute == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_69():void{const input=bytes("014198c4c0f25f164b1e246a");const codec=new r.Ros2Aimdk_msgsSrvSetVolume_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-996654847));
assert(value.request.header.stamp.nanosec == <u32>(375386816));
assert(value.audio_volume == <u32>(1780751947));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_70():void{const input=bytes("b75215ec53a04b19d309a1446a7096da5362972a070000007665623661ceb2836cf7ed");const codec=new r.Ros2Aimdk_msgsSrvSetVolume_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-334146889));
assert(value.reponse.header.stamp.nanosec == <u32>(424386643));
assert(value.reponse.header.code == <i64><u64>0xda96706a44a109d3);
assert(value.reponse.status.value == <i32>(714564179));
assert(value.reponse.message == "veb6aβ");
assert(value.audio_volume == <u32>(3992415363));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_71():void{const input=bytes("072b89b550057009");const codec=new r.Ros2Builtin_interfacesMsgTimeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-1249301753));
assert(value.nanosec == <u32>(158336336));
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
}
