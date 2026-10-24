/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../sdk/origin/custom-types/ubtech-ros2-main-complete/bounded_records";
import {abortGuest} from "../../../../../../../sdk/origin/assemblyscript/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("072b89b550057009");const codec=new r.Ros2Builtin_interfacesMsgTimeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-1249301753));
assert(value.nanosec == <u32>(158336336));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("0000000000e86dc00000000000d054c000000000002042c0");const codec=new r.Ros2Geometry_msgsMsgPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(-239.25));
assert(value.y == <f64>(-83.25));
assert(value.z == <f64>(-36.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("0000000000c02ec000000000003047c00000000000203d4000000000004857c000000000007049400000000000986dc00000000000585340");const codec=new r.Ros2Geometry_msgsMsgPoseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.position.x == <f64>(-15.375));
assert(value.position.y == <f64>(-46.375));
assert(value.position.z == <f64>(29.125));
assert(value.orientation.x == <f64>(-93.125));
assert(value.orientation.y == <f64>(50.875));
assert(value.orientation.z == <f64>(-236.75));
assert(value.orientation.w == <f64>(77.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("72e71f7fe898242b070000007664386435ceb200000000003854c000000000006c66c00000000000f8654000000000001869c00000000000b06dc00000000000d053c00000000000e85dc0");const codec=new r.Ros2Geometry_msgsMsgPoseStampedCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(2132797298));
assert(value.header.stamp.nanosec == <u32>(723818728));
assert(value.header.frame_id == "vd8d5β");
assert(value.pose.position.x == <f64>(-80.875));
assert(value.pose.position.y == <f64>(-179.375));
assert(value.pose.position.z == <f64>(175.75));
assert(value.pose.orientation.x == <f64>(-200.75));
assert(value.pose.orientation.y == <f64>(-237.5));
assert(value.pose.orientation.z == <f64>(-79.25));
assert(value.pose.orientation.w == <f64>(-119.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("0000000000a85cc00000000000046dc00000000000d045c00000000000005340");const codec=new r.Ros2Geometry_msgsMsgQuaternionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(-114.625));
assert(value.y == <f64>(-232.125));
assert(value.z == <f64>(-43.625));
assert(value.w == <f64>(76.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("01172a7241d6691a070000007638303466ceb2aa39858f285bdb17070000007634643938ceb200000000005859400000000000d041c000000000005853400000000000e046c000000000000033400000000000fc6bc00000000000106440070000007662333937ceb200202643070000007634626166ceb2");const codec=new r.Ros2Grab_demo_msgsMsgGraspCandidateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1915361025));
assert(value.header.stamp.nanosec == <u32>(443143745));
assert(value.header.frame_id == "v804fβ");
assert(value.pose.header.stamp.sec == <i32>(-1887094358));
assert(value.pose.header.stamp.nanosec == <u32>(400251688));
assert(value.pose.header.frame_id == "v4d98β");
assert(value.pose.pose.position.x == <f64>(101.375));
assert(value.pose.pose.position.y == <f64>(-35.625));
assert(value.pose.pose.position.z == <f64>(77.375));
assert(value.pose.pose.orientation.x == <f64>(-45.75));
assert(value.pose.pose.orientation.y == <f64>(19.0));
assert(value.pose.pose.orientation.z == <f64>(-223.875));
assert(value.pose.pose.orientation.w == <f64>(160.5));
assert(value.group_name == "vb397β");
assert(value.confidence == <f32>(166.125));
assert(value.object_class == "v4bafβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("070000007663323866ceb2070000007634613137ceb20000000000604d4000000000004044c00000000000d05240000000000020344000000000002031c00000000000bc64c00000000000cc6d40");const codec=new r.Ros2Grab_demo_msgsMsgIKRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.group_name == "vc28fβ");
assert(value.frame_id == "v4a17β");
assert(value.position.x == <f64>(58.75));
assert(value.position.y == <f64>(-40.5));
assert(value.position.z == <f64>(75.25));
assert(value.orientation.x == <f64>(20.125));
assert(value.orientation.y == <f64>(-17.125));
assert(value.orientation.z == <f64>(-165.875));
assert(value.orientation.w == <f64>(238.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("681252e4acc1ba37070000007666316532ceb2");const codec=new r.Ros2Std_msgsMsgHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(-464383384));
assert(value.stamp.nanosec == <u32>(934986156));
assert(value.frame_id == "vf1e2β");
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
}
