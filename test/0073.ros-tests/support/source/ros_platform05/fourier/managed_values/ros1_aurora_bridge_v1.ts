/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../sdk/origin/custom-types/fourier-ros1-aurora-bridge-v1-complete/bounded_records";
import {abortGuest} from "../../../../../../../sdk/origin/assemblyscript/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("39b83ef880c109660100000069070000007639663361ceb2010000000000000000206f4001010000000000000000a861c0c3d964ee");const codec=new r.Ros1Uwvm_fourier_bridgeExecuteMoveRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x6609c180f83eb839);
assert(value.moves.length == 1);
assert(value.moves[0].move_type == <u8>(105));
assert(value.moves[0].group == "v9f3aβ");
assert(value.moves[0].position.length == 1);
assert(value.moves[0].position[0] == <f64>(249.0));
assert(value.moves[0].has_velocity == true);
assert(value.moves[0].velocity.length == 1);
assert(value.moves[0].velocity[0] == <f64>(-141.25));
assert(value.moves[0].expected_velocity == <i32>(-295380541));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("d0c1d4a1070000007665396331ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeExecuteMoveResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2715075024));
assert(value.error == "ve9c1β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("bddb14e204a2237c070000007639323631ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetBaseDataRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x7c23a204e214dbbd);
assert(value.key == "v9261β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("c4386351070000007635316162ceb2010000000000000000402f40");const codec=new r.Ros1Uwvm_fourier_bridgeGetBaseDataResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1365457092));
assert(value.error == "v51abβ");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(15.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("f029e24d4950d5d3070000007639386664ceb2070000007638656432ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetCartesianStateRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xd3d550494de229f0);
assert(value.group == "v98fdβ");
assert(value.key == "v8ed2β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("6d398aad070000007630313232ceb2010000000000000000546c40");const codec=new r.Ros1Uwvm_fourier_bridgeGetCartesianStateResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2911517037));
assert(value.error == "v0122β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(226.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("83295c483a506bd6070000007637643231ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetContactDataRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xd66b503a485c2983);
assert(value.key == "v7d21β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("aa59dd13070000007635643532ceb2010000000000000000f04dc0");const codec=new r.Ros1Uwvm_fourier_bridgeGetContactDataResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(333273514));
assert(value.error == "v5d52β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(-59.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("fda878d544d14f4b");const codec=new r.Ros1Uwvm_fourier_bridgeGetFsmNameRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x4b4fd144d578a8fd);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("2f882be5070000007631326135ceb2070000007635323066ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetFsmNameResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3844835375));
assert(value.error == "v12a5β");
assert(value.value == "v520fβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("554b99c1ec32ae5f");const codec=new r.Ros1Uwvm_fourier_bridgeGetFsmStateRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x5fae32ecc1994b55);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("336f9405070000007637386162ceb239ca0138");const codec=new r.Ros1Uwvm_fourier_bridgeGetFsmStateResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(93613875));
assert(value.error == "v78abβ");
assert(value.value == <i32>(939641401));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("00be22f4b9c7156a070000007633393836ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetGroupMotionStateRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x6a15c7b9f422be00);
assert(value.group == "v3986β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("1bc0c90b070000007666653863ceb231d026be");const codec=new r.Ros1Uwvm_fourier_bridgeGetGroupMotionStateResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(197771291));
assert(value.error == "vfe8cβ");
assert(value.value == <i32>(-1104752591));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("972834432e5103dd070000007661393034ceb2070000007635333638ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetGroupMotorCfgRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xdd03512e43342897);
assert(value.group == "va904β");
assert(value.key == "v5368β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("656c03a0070000007665353164ceb2010000000000000000d06fc0");const codec=new r.Ros1Uwvm_fourier_bridgeGetGroupMotorCfgResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2684578917));
assert(value.error == "ve51dβ");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(-254.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("a3572aa91a2e1d37070000007661343331ceb2070000007666363636ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetGroupStateRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x371d2e1aa92a57a3);
assert(value.group == "va431β");
assert(value.key == "vf666β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("e1613b6b070000007662636133ceb2010000000000000000c05b40");const codec=new r.Ros1Uwvm_fourier_bridgeGetGroupStateResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1799053793));
assert(value.error == "vbca3β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(111.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("8ed7fffe37aec860");const codec=new r.Ros1Uwvm_fourier_bridgeGetSnapshotRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x60c8ae37feffd78e);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("95690ed6070000007633343539ceb234f95aad070000007634633830ceb279ad4789070000007665353737ceb265081fee070000007634306537ceb20100000000000000009864c001000000070000007663383864ceb20100000000000000007c6dc00100000000000000008036c00100000000000000001861c09cb73da0");const codec=new r.Ros1Uwvm_fourier_bridgeGetSnapshotResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3591268757));
assert(value.error == "v3459β");
assert(value.value.fsm_state == <i32>(-1386546892));
assert(value.value.fsm_name == "v4c80β");
assert(value.value.upper_fsm_state == <i32>(-1991791239));
assert(value.value.upper_fsm_name == "ve577β");
assert(value.value.velocity_source == <i32>(-299956123));
assert(value.value.velocity_source_name == "v40e7β");
assert(value.value.stand_pose.length == 1);
assert(value.value.stand_pose[0] == <f64>(-164.75));
assert(value.value.groups.length == 1);
assert(value.value.groups[0].group == "vc88dβ");
assert(value.value.groups[0].position.length == 1);
assert(value.value.groups[0].position[0] == <f64>(-235.875));
assert(value.value.groups[0].velocity.length == 1);
assert(value.value.groups[0].velocity[0] == <f64>(-22.5));
assert(value.value.groups[0].effort.length == 1);
assert(value.value.groups[0].effort[0] == <f64>(-136.75));
assert(value.value.groups[0].motion_state == <i32>(-1606568036));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("5ca6d38ae5dfe414");const codec=new r.Ros1Uwvm_fourier_bridgeGetStandPoseRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x14e4dfe58ad3a65c);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("1621ad4b070000007634353636ceb20100000000000000001858c0");const codec=new r.Ros1Uwvm_fourier_bridgeGetStandPoseResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1269637398));
assert(value.error == "v4566β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(-96.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("9f3bf1e92642c677");const codec=new r.Ros1Uwvm_fourier_bridgeGetUpperFsmNameRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x77c64226e9f13b9f);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("0b3b5c6f070000007630306466ceb2070000007634303735ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetUpperFsmNameResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1868315403));
assert(value.error == "v00dfβ");
assert(value.value == "v4075β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("b2c219620bbb2efc");const codec=new r.Ros1Uwvm_fourier_bridgeGetUpperFsmStateRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xfc2ebb0b6219c2b2);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_25():void{const input=bytes("51fc1d39070000007666326463ceb21d7976b2");const codec=new r.Ros1Uwvm_fourier_bridgeGetUpperFsmStateResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(958266449));
assert(value.error == "vf2dcβ");
assert(value.value == <i32>(-1300858595));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_26():void{const input=bytes("7ac866f7c3b15169");const codec=new r.Ros1Uwvm_fourier_bridgeGetVelocitySourceNameRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x6951b1c3f766c87a);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_27():void{const input=bytes("99b65df9070000007630386661ceb2070000007634383530ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeGetVelocitySourceNameResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(4183668377));
assert(value.error == "v08faβ");
assert(value.value == "v4850β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_28():void{const input=bytes("81926f8938eb5817");const codec=new r.Ros1Uwvm_fourier_bridgeGetVelocitySourceRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x1758eb38896f9281);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_29():void{const input=bytes("94f07b41070000007639333139ceb2551eb3d3");const codec=new r.Ros1Uwvm_fourier_bridgeGetVelocitySourceResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1098641556));
assert(value.error == "v9319β");
assert(value.value == <i32>(-743236011));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_30():void{const input=bytes("070000007666313031ceb2010000000000000000f04440010000000000000000e05fc00100000000000000006c6340dfce3c49");const codec=new r.Ros1Uwvm_fourier_bridgeGroupStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.group == "vf101β");
assert(value.position.length == 1);
assert(value.position[0] == <f64>(41.875));
assert(value.velocity.length == 1);
assert(value.velocity[0] == <f64>(-127.5));
assert(value.effort.length == 1);
assert(value.effort[0] == <f64>(155.375));
assert(value.motion_state == <i32>(1228721887));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_31():void{const input=bytes("070000007666373530ceb20100000000000000007c6440");const codec=new r.Ros1Uwvm_fourier_bridgeGroupValuesCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.group == "vf750β");
assert(value.values.length == 1);
assert(value.values[0] == <f64>(163.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_32():void{const input=bytes("e4070000007634333463ceb20100000000000000008049c001010000000000000000b460403aa5eba5");const codec=new r.Ros1Uwvm_fourier_bridgeMotionRecipeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.move_type == <u8>(228));
assert(value.group == "v434cβ");
assert(value.position.length == 1);
assert(value.position[0] == <f64>(-51.0));
assert(value.has_velocity == true);
assert(value.velocity.length == 1);
assert(value.velocity[0] == <f64>(133.625));
assert(value.expected_velocity == <i32>(-1511283398));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_33():void{const input=bytes("9e7188182708bf86f5831c01");const codec=new r.Ros1Uwvm_fourier_bridgeSetFsmStateRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x86bf08271888719e);
assert(value.state == <i32>(18646005));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_34():void{const input=bytes("da78f4c2070000007663663134ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeSetFsmStateResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3270801626));
assert(value.error == "vcf14β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_35():void{const input=bytes("889be04f31e2d7d101000000070000007661343237ceb2010000000000000000604d4001000000070000007639653539ceb20100000000000000002038c001000000070000007639303463ceb20100000000000000003462c0");const codec=new r.Ros1Uwvm_fourier_bridgeSetGroupCommandRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xd1d7e2314fe09b88);
assert(value.position.length == 1);
assert(value.position[0].group == "va427β");
assert(value.position[0].values.length == 1);
assert(value.position[0].values[0] == <f64>(58.75));
assert(value.velocity.length == 1);
assert(value.velocity[0].group == "v9e59β");
assert(value.velocity[0].values.length == 1);
assert(value.velocity[0].values[0] == <f64>(-24.125));
assert(value.torque.length == 1);
assert(value.torque[0].group == "v904cβ");
assert(value.torque[0].values.length == 1);
assert(value.torque[0].values[0] == <f64>(-145.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_36():void{const input=bytes("ae4babc2070000007639303237ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeSetGroupCommandResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3266005934));
assert(value.error == "v9027β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_37():void{const input=bytes("e071229f5908150101000000070000007637373634ceb2010000000000000000801dc001000000070000007635393231ceb2010000000000000000a049c0");const codec=new r.Ros1Uwvm_fourier_bridgeSetMotorCfgPdRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x011508599f2271e0);
assert(value.kp.length == 1);
assert(value.kp[0].group == "v7764β");
assert(value.kp[0].values.length == 1);
assert(value.kp[0].values[0] == <f64>(-7.375));
assert(value.kd.length == 1);
assert(value.kd[0].group == "v5921β");
assert(value.kd[0].values.length == 1);
assert(value.kd[0].values[0] == <f64>(-51.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_38():void{const input=bytes("4bd45760070000007630626339ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeSetMotorCfgPdResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(1616368715));
assert(value.error == "v0bc9β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_39():void{const input=bytes("5eacbc86e7d58b180000000000c034c00000000000802e400000000000ac6440");const codec=new r.Ros1Uwvm_fourier_bridgeSetStandPoseRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x188bd5e786bcac5e);
assert(value.delta_z == <f64>(-20.75));
assert(value.delta_pitch == <f64>(15.25));
assert(value.delta_yaw == <f64>(165.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_40():void{const input=bytes("dd1bbc92070000007638323036ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeSetStandPoseResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(2461801437));
assert(value.error == "v8206β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_41():void{const input=bytes("acb4542e15cd63b0b60565ff");const codec=new r.Ros1Uwvm_fourier_bridgeSetUpperFsmStateRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xb063cd152e54b4ac);
assert(value.state == <i32>(-10156618));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_42():void{const input=bytes("70bc7b1d070000007632303366ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeSetUpperFsmStateResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(494648432));
assert(value.error == "v203fβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_43():void{const input=bytes("46b99d6fffc0aaf10000000000f466c00000000000bc6cc00000000000386bc00000000000446840");const codec=new r.Ros1Uwvm_fourier_bridgeSetVelocityRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xf1aac0ff6f9db946);
assert(value.vx == <f64>(-183.625));
assert(value.vy == <f64>(-229.875));
assert(value.yaw == <f64>(-217.75));
assert(value.duration == <f64>(194.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_44():void{const input=bytes("db360cd0070000007638306233ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeSetVelocityResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3490461403));
assert(value.error == "v80b3β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_45():void{const input=bytes("94e22c732d9b1bed1314fbc0");const codec=new r.Ros1Uwvm_fourier_bridgeSetVelocitySourceRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0xed1b9b2d732ce294);
assert(value.source == <i32>(-1057287149));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_46():void{const input=bytes("8a86360d070000007662373766ceb2");const codec=new r.Ros1Uwvm_fourier_bridgeSetVelocitySourceResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(221677194));
assert(value.error == "vb77fβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_47():void{const input=bytes("345117e9070000007630353035ceb2d2dcfdd7070000007638316666ceb2ce79a5b0070000007637383632ceb2010000000000000000f05bc001000000070000007639363337ceb2010000000000000000005a400100000000000000002461c00100000000000000006c67c048864a54");const codec=new r.Ros1Uwvm_fourier_bridgeStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.fsm_state == <i32>(-384347852));
assert(value.fsm_name == "v0505β");
assert(value.upper_fsm_state == <i32>(-671228718));
assert(value.upper_fsm_name == "v81ffβ");
assert(value.velocity_source == <i32>(-1331332658));
assert(value.velocity_source_name == "v7862β");
assert(value.stand_pose.length == 1);
assert(value.stand_pose[0] == <f64>(-111.75));
assert(value.groups.length == 1);
assert(value.groups[0].group == "v9637β");
assert(value.groups[0].position.length == 1);
assert(value.groups[0].position[0] == <f64>(104.0));
assert(value.groups[0].velocity.length == 1);
assert(value.groups[0].velocity[0] == <f64>(-137.125));
assert(value.groups[0].effort.length == 1);
assert(value.groups[0].effort[0] == <f64>(-187.375));
assert(value.groups[0].motion_state == <i32>(1414170184));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_48():void{const input=bytes("534074feea3943600000000000dc60400000000000ec6f400000000000405ac00000000000b86ac0");const codec=new r.Ros1Uwvm_fourier_bridgeVelocityCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x604339eafe744053);
assert(value.vx == <f64>(134.875));
assert(value.vy == <f64>(255.375));
assert(value.yaw == <f64>(-105.0));
assert(value.duration == <f64>(-213.75));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_49():void{const input=bytes("683d4c02d1447b9c01000000070000007636316534ceb20000000000904cc00000000000205340");const codec=new r.Ros1Uwvm_fourier_bridgeWaitGroupsMotionCompleteRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sequence == <u64><u64>0x9c7b44d1024c3d68);
assert(value.groups.length == 1);
assert(value.groups[0] == "v61e4β");
assert(value.timeout_seconds == <f64>(-57.125));
assert(value.poll_seconds == <f64>(76.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_50():void{const input=bytes("cef36cdc070000007635363664ceb201");const codec=new r.Ros1Uwvm_fourier_bridgeWaitGroupsMotionCompleteResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == <u32>(3698127822));
assert(value.error == "v566dβ");
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
