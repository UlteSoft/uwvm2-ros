/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../../../../sdk/include/platform/fourier/as/ros2/fourier-ros2-aurora-bridge-v1-complete/bounded_records";
import {abortGuest} from "../../../../../../../../../../sdk/include/platform/ros/as/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("070000007661313764ceb20100000000000000009053c0010000000000000000486d400100000000000000004045c05a55a620");const codec=new r.Ros2Uwvm_fourier_bridgeMsgGroupStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.group == "va17dβ");
assert(value.position.length == 1);
assert(value.position[0] == <f64>(-78.25));
assert(value.velocity.length == 1);
assert(value.velocity[0] == <f64>(234.25));
assert(value.effort.length == 1);
assert(value.effort[0] == <f64>(-42.5));
assert(value.motion_state == <i32>(547771738));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("070000007635643061ceb2010000000000000000a05bc0");const codec=new r.Ros2Uwvm_fourier_bridgeMsgGroupValuesCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.group == "v5d0aβ");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(-110.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("77070000007631623865ceb201000000000000000090444001010000000000000000b05ac0d701fcd5");const codec=new r.Ros2Uwvm_fourier_bridgeMsgMotionRecipeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.move_type == <u8>(119));
assert(value.group == "v1b8eβ");
assert(value.position.length == 1);
assert(value.position[0] == <f64>(41.125));
assert(value.has_velocity == true);
assert(value.velocity.length == 1);
assert(value.velocity[0] == <f64>(-106.75));
assert(value.expected_velocity == <i32>(-704904745));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("5a336bb9070000007637633136ceb241a7a44b070000007664313832ceb25d02fc2c070000007665633633ceb2010000000000000000f0584001000000070000007630613665ceb2010000000000000000c856c0010000000000000000003240010000000000000000a863c04de41a29");const codec=new r.Ros2Uwvm_fourier_bridgeMsgStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.fsm_state == <i32>(-1184156838));
assert(value.fsm_name == "v7c16β");
assert(value.upper_fsm_state == <i32>(1269081921));
assert(value.upper_fsm_name == "vd182β");
assert(value.velocity_source == <i32>(754713181));
assert(value.velocity_source_name == "vec63β");
assert(value.stand_pose.length == 1);
assert(value.stand_pose[0] == <f64>(99.75));
assert(value.groups.length == 1);
assert(value.groups[0].group == "v0a6eβ");
assert(value.groups[0].position.length == 1);
assert(value.groups[0].position[0] == <f64>(-91.125));
assert(value.groups[0].velocity.length == 1);
assert(value.groups[0].velocity[0] == <f64>(18.0));
assert(value.groups[0].effort.length == 1);
assert(value.groups[0].effort[0] == <f64>(-157.25));
assert(value.groups[0].motion_state == <i32>(689628237));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("67adcf63ded4f8fd00000000004033c0000000000000324000000000005048400000000000805740");const codec=new r.Ros2Uwvm_fourier_bridgeMsgVelocityCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xfdf8d4de63cfad67);
assert(value.vx == <f64>(-19.25));
assert(value.vy == <f64>(18.0));
assert(value.yaw == <f64>(48.625));
assert(value.duration == <f64>(94.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("c31eeddb7a67da4501000000a1070000007664616265ceb2010000000000000000406d4001010000000000000000c462401ae52d8c");const codec=new r.Ros2Uwvm_fourier_bridgeSrvExecuteMove_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x45da677adbed1ec3);
assert(value.moves.length == 1);
assert(value.moves[0].move_type == <u8>(161));
assert(value.moves[0].group == "vdabeβ");
assert(value.moves[0].position.length == 1);
assert(value.moves[0].position[0] == <f64>(234.0));
assert(value.moves[0].has_velocity == true);
assert(value.moves[0].velocity.length == 1);
assert(value.moves[0].velocity[0] == <f64>(150.125));
assert(value.moves[0].expected_velocity == <i32>(-1943149286));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("278415ab070000007632383261ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvExecuteMove_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2870314023));
assert(value.error == "v282aβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("99732231200a15af070000007663653536ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetBaseData_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xaf150a2031227399);
assert(value.key == "vce56β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("a3e73fb1070000007664383964ceb2010000000000000000e85740");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetBaseData_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2973755299));
assert(value.error == "vd89dβ");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(95.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("e2bd15ec5bc42272070000007637663665ceb2070000007665323632ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetCartesianState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x7222c45bec15bde2);
assert(value.group == "v7f6eβ");
assert(value.key == "ve262β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("cc558d8b070000007664663765ceb2010000000000000000046ac0");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetCartesianState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2341295564));
assert(value.error == "vdf7eβ");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(-208.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("4223028dfb5a3513070000007661623562ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetContactData_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x13355afb8d022342);
assert(value.key == "vab5bβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("1226ecf7070000007636326532ceb2010000000000000000485a40");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetContactData_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(4159448594));
assert(value.error == "v62e2β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(105.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("5f5d7a91e6244d0f");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetFsmName_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x0f4d24e6917a5d5f);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("4359f3e3070000007637636331ceb2070000007633633662ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetFsmName_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3824376131));
assert(value.error == "v7cc1β");
assert(value.value == "v3c6bβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("4394d09ffaede701");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetFsmState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x01e7edfa9fd09443);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("4b77b292070000007638633661ceb2323ac0cc");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetFsmState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2461169483));
assert(value.error == "v8c6aβ");
assert(value.value == <i32>(-859817422));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("f0e2cd04499bfa9a070000007662656463ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetGroupMotionState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x9afa9b4904cde2f0);
assert(value.group == "vbedcβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("8b7406eb070000007665346431ceb23b5e7ba4");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetGroupMotionState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3943068811));
assert(value.error == "ve4d1β");
assert(value.value == <i32>(-1535418821));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("bbd9cfa902a0f837070000007664633932ceb2070000007633656266ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetGroupMotorCfg_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x37f8a002a9cfd9bb);
assert(value.group == "vdc92β");
assert(value.key == "v3ebfβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("e179b58e070000007665373035ceb2010000000000000000486bc0");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetGroupMotorCfg_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2394257889));
assert(value.error == "ve705β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(-218.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("dc404ef16539796f070000007636396237ceb2070000007663353735ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetGroupState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x6f793965f14e40dc);
assert(value.group == "v69b7β");
assert(value.key == "vc575β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("dc0f6067070000007635316436ceb2010000000000000000805f40");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetGroupState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1734348764));
assert(value.error == "v51d6β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(126.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("83f9ee8e3a80d910");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetSnapshot_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x10d9803a8eeef983);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("90ebd4ad070000007635666630ceb2d59f8598070000007632363465ceb20769ba1e070000007663613661ceb21bcce279070000007637633336ceb20100000000000000001c604001000000070000007635663730ceb2010000000000000000685f40010000000000000000606b40010000000000000000885640b90d2006");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetSnapshot_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2916412304));
assert(value.error == "v5ff0β");
assert(value.value.fsm_state == <i32>(-1736073259));
assert(value.value.fsm_name == "v264eβ");
assert(value.value.upper_fsm_state == <i32>(515533063));
assert(value.value.upper_fsm_name == "vca6aβ");
assert(value.value.velocity_source == <i32>(2044906523));
assert(value.value.velocity_source_name == "v7c36β");
assert(value.value.stand_pose.length == 1);
assert(value.value.stand_pose[0] == <f64>(128.875));
assert(value.value.groups.length == 1);
assert(value.value.groups[0].group == "v5f70β");
assert(value.value.groups[0].position.length == 1);
assert(value.value.groups[0].position[0] == <f64>(125.625));
assert(value.value.groups[0].velocity.length == 1);
assert(value.value.groups[0].velocity[0] == <f64>(219.0));
assert(value.value.groups[0].effort.length == 1);
assert(value.value.groups[0].effort[0] == <f64>(90.125));
assert(value.value.groups[0].motion_state == <i32>(102763961));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_25():void{const input=bytes("4e0195adf778a233");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetStandPose_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x33a278f7ad95014e);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_26():void{const input=bytes("67994b0f070000007661376633ceb2010000000000000000105440");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetStandPose_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(256612711));
assert(value.error == "va7f3β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(80.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_27():void{const input=bytes("d0b950ac69c06732");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetUpperFsmName_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x3267c069ac50b9d0);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_28():void{const input=bytes("26018211070000007636663939ceb2070000007632663333ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetUpperFsmName_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(293732646));
assert(value.error == "v6f99β");
assert(value.value == "v2f33β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_29():void{const input=bytes("1620eae2af59dd7c");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetUpperFsmState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x7cdd59afe2ea2016);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_30():void{const input=bytes("c49398af070000007637653162ceb25762b13e");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetUpperFsmState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2946012100));
assert(value.error == "v7e1bβ");
assert(value.value == <i32>(1051812439));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_31():void{const input=bytes("ce479e27773ea9b9");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetVelocitySourceName_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xb9a93e77279e47ce);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_32():void{const input=bytes("7cf5c461070000007634333939ceb2070000007630333333ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetVelocitySourceName_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1640297852));
assert(value.error == "v4399β");
assert(value.value == "v0333β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_33():void{const input=bytes("f371d3cb4a08e455");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetVelocitySource_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x55e4084acbd371f3);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_34():void{const input=bytes("bdee004f070000007633333866ceb25d052573");const codec=new r.Ros2Uwvm_fourier_bridgeSrvGetVelocitySource_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1325461181));
assert(value.error == "v338fβ");
assert(value.value == <i32>(1931806045));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_35():void{const input=bytes("419ebf93f8e7880de6c4e7a1");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetFsmState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x0d88e7f893bf9e41);
assert(value.state == <i32>(-1578646298));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_36():void{const input=bytes("804da34b070000007634623061ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetFsmState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1268993408));
assert(value.error == "v4b0aβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_37():void{const input=bytes("b72d252f0e5412b101000000070000007664336537ceb2010000000000000000206f4001000000070000007665393939ceb201000000000000000080644001000000070000007631383237ceb2010000000000000000286640");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetGroupCommand_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xb112540e2f252db7);
assert(value.position.length == 1);
assert(value.position[0].group == "vd3e7β");
assert(value.position[0].values.length == 1);
assert(value.position[0].values[0] == <f64>(249.0));
assert(value.velocity.length == 1);
assert(value.velocity[0].group == "ve999β");
assert(value.velocity[0].values.length == 1);
assert(value.velocity[0].values[0] == <f64>(164.0));
assert(value.torque.length == 1);
assert(value.torque[0].group == "v1827β");
assert(value.torque[0].values.length == 1);
assert(value.torque[0].values[0] == <f64>(177.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_38():void{const input=bytes("31a193b7070000007663323665ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetGroupCommand_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3079905585));
assert(value.error == "vc26eβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_39():void{const input=bytes("d058ad1e69219a8001000000070000007636613465ceb201000000000000000020404001000000070000007634343062ceb2010000000000000000b042c0");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetMotorCfgPd_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x809a21691ead58d0);
assert(value.kp.length == 1);
assert(value.kp[0].group == "v6a4eβ");
assert(value.kp[0].values.length == 1);
assert(value.kp[0].values[0] == <f64>(32.25));
assert(value.kd.length == 1);
assert(value.kd[0].group == "v440bβ");
assert(value.kd[0].values.length == 1);
assert(value.kd[0].values[0] == <f64>(-37.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_40():void{const input=bytes("9f296851070000007635616261ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetMotorCfgPd_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1365780895));
assert(value.error == "v5abaβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_41():void{const input=bytes("680f9743d176a0dd0000000000c852400000000000d06dc00000000000b461c0");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetStandPose_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xdda076d143970f68);
assert(value.delta_z == <f64>(75.125));
assert(value.delta_pitch == <f64>(-238.5));
assert(value.delta_yaw == <f64>(-141.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_42():void{const input=bytes("65932403070000007637656532ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetStandPose_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(52728677));
assert(value.error == "v7ee2β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_43():void{const input=bytes("0350a918ba299e863d35e3d7");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetUpperFsmState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x869e29ba18a95003);
assert(value.state == <i32>(-672975555));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_44():void{const input=bytes("dae5d5e3070000007635613764ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetUpperFsmState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3822446042));
assert(value.error == "v5a7dβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_45():void{const input=bytes("68d6f4a6d1afc33824394608");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetVelocitySource_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x38c3afd1a6f4d668);
assert(value.source == <i32>(138819876));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_46():void{const input=bytes("a89e43b5070000007637666332ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetVelocitySource_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3041107624));
assert(value.error == "v7fc2β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_47():void{const input=bytes("6fd1c01bd6a8f78500000000007040c000000000008013400000000000385d400000000000405740");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetVelocity_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x85f7a8d61bc0d16f);
assert(value.vx == <f64>(-32.875));
assert(value.vy == <f64>(4.875));
assert(value.yaw == <f64>(116.875));
assert(value.duration == <f64>(93.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_48():void{const input=bytes("5885b63c070000007635396632ceb2");const codec=new r.Ros2Uwvm_fourier_bridgeSrvSetVelocity_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1018594648));
assert(value.error == "v59f2β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_49():void{const input=bytes("dc08b02f657187b101000000070000007661646335ceb20000000000f86ac00000000000d85340");const codec=new r.Ros2Uwvm_fourier_bridgeSrvWaitGroupsMotionComplete_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xb18771652fb008dc);
assert(value.groups.length == 1);
assert(value.groups[0] == "vadc5β");
assert(value.timeout_seconds == <f64>(-215.75));
assert(value.poll_seconds == <f64>(79.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_50():void{const input=bytes("31cab622070000007635383466ceb201");const codec=new r.Ros2Uwvm_fourier_bridgeSrvWaitGroupsMotionComplete_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(582404657));
assert(value.error == "v584fβ");
assert(value.complete == true);
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
}
