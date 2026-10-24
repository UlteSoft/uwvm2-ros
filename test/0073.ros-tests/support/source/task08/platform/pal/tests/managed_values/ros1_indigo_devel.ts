/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../../../sdk/origin/custom-types/pal-ros1-indigo-devel-complete/bounded_records";
import {abortGuest} from "../../../../../../../../../sdk/origin/assemblyscript/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("41a82588c71e9838");const codec=new r.Ros1DurationCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-2010798015));
assert(value.nsec == <i32>(949493447));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("0000000000e46dc00000000000ec69c00000000000746fc00000000000ac66400000000000b4604000000000001c6640");const codec=new r.Ros1Geometry_msgsAccelCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.linear.x == <f64>(-239.125));
assert(value.linear.y == <f64>(-207.375));
assert(value.linear.z == <f64>(-251.625));
assert(value.angular.x == <f64>(181.375));
assert(value.angular.y == <f64>(133.625));
assert(value.angular.z == <f64>(176.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("0000000000002ec000000000000048c00000000000803f40");const codec=new r.Ros1Geometry_msgsPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(-15.0));
assert(value.y == <f64>(-48.0));
assert(value.z == <f64>(31.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("00c09ac200a015c300c04f43");const codec=new r.Ros1Geometry_msgsPoint32Codec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f32>(-77.375));
assert(value.y == <f32>(-149.625));
assert(value.z == <f32>(207.75));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("31da0840484793488cde3e1b070000007638633465ceb20000000000b869c000000000004066c00000000000b05fc0");const codec=new r.Ros1Geometry_msgsPointStampedCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1074321969));
assert(value.header.stamp.sec == <u32>(1217611592));
assert(value.header.stamp.nsec == <u32>(457105036));
assert(value.header.frame_id == "v8c4eβ");
assert(value.point.x == <f64>(-205.75));
assert(value.point.y == <f64>(-178.0));
assert(value.point.z == <f64>(-126.75));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("0000000000905e40000000000000334000000000002865400000000000803dc0000000000080354000000000007061c000000000008c65c0");const codec=new r.Ros1Geometry_msgsPoseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.position.x == <f64>(122.25));
assert(value.position.y == <f64>(19.0));
assert(value.position.z == <f64>(169.25));
assert(value.orientation.x == <f64>(-29.5));
assert(value.orientation.y == <f64>(21.5));
assert(value.orientation.z == <f64>(-139.5));
assert(value.orientation.w == <f64>(-172.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("00000000004060c00000000000d058c00000000000b069c0");const codec=new r.Ros1Geometry_msgsPose2DCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(-130.0));
assert(value.y == <f64>(-99.25));
assert(value.theta == <f64>(-205.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("4b5798c4344f20f0939eaf18070000007638326533ceb20100000000000000002467c00000000000046bc00000000000005b400000000000905bc000000000001067c00000000000985a4000000000006038c0");const codec=new r.Ros1Geometry_msgsPoseArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3298318155));
assert(value.header.stamp.sec == <u32>(4028649268));
assert(value.header.stamp.nsec == <u32>(414162579));
assert(value.header.frame_id == "v82e3β");
assert(value.poses.length == 1);
assert(value.poses[0].position.x == <f64>(-185.125));
assert(value.poses[0].position.y == <f64>(-216.125));
assert(value.poses[0].position.z == <f64>(108.0));
assert(value.poses[0].orientation.x == <f64>(-110.25));
assert(value.poses[0].orientation.y == <f64>(-184.5));
assert(value.poses[0].orientation.z == <f64>(106.375));
assert(value.poses[0].orientation.w == <f64>(-24.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("000000000000e4bf00000000000c62c00000000000846bc0000000000080444000000000000045400000000000106b40000000000000d03f0000000000e85d400000000000206a4000000000003c61400000000000c0384000000000002463400000000000a46dc000000000005052c000000000004c63c00000000000a85bc00000000000d05bc00000000000785fc00000000000403fc000000000005c6b400000000000e060400000000000405cc00000000000402b4000000000006866c00000000000e46cc000000000005c68c00000000000486cc00000000000404dc0000000000000f6bf00000000006866400000000000b8594000000000000010c000000000004855400000000000cc6d40000000000040684000000000004021c000000000006053c00000000000402ac0000000000020514000000000006061c000000000007c6dc00000000000fc6ac000000000009059c0");const codec=new r.Ros1Geometry_msgsPoseWithCovarianceCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pose.position.x == <f64>(-0.625));
assert(value.pose.position.y == <f64>(-144.375));
assert(value.pose.position.z == <f64>(-220.125));
assert(value.pose.orientation.x == <f64>(41.0));
assert(value.pose.orientation.y == <f64>(42.0));
assert(value.pose.orientation.z == <f64>(216.5));
assert(value.pose.orientation.w == <f64>(0.25));
assert(value.covariance.length == 36);
assert(value.covariance[0] == <f64>(119.625));
assert(value.covariance[1] == <f64>(209.0));
assert(value.covariance[2] == <f64>(137.875));
assert(value.covariance[3] == <f64>(24.75));
assert(value.covariance[4] == <f64>(153.125));
assert(value.covariance[5] == <f64>(-237.125));
assert(value.covariance[6] == <f64>(-73.25));
assert(value.covariance[7] == <f64>(-154.375));
assert(value.covariance[8] == <f64>(-110.625));
assert(value.covariance[9] == <f64>(-111.25));
assert(value.covariance[10] == <f64>(-125.875));
assert(value.covariance[11] == <f64>(-31.25));
assert(value.covariance[12] == <f64>(218.875));
assert(value.covariance[13] == <f64>(135.0));
assert(value.covariance[14] == <f64>(-113.0));
assert(value.covariance[15] == <f64>(13.625));
assert(value.covariance[16] == <f64>(-179.25));
assert(value.covariance[17] == <f64>(-231.125));
assert(value.covariance[18] == <f64>(-194.875));
assert(value.covariance[19] == <f64>(-226.25));
assert(value.covariance[20] == <f64>(-58.5));
assert(value.covariance[21] == <f64>(-1.375));
assert(value.covariance[22] == <f64>(179.25));
assert(value.covariance[23] == <f64>(102.875));
assert(value.covariance[24] == <f64>(-4.0));
assert(value.covariance[25] == <f64>(85.125));
assert(value.covariance[26] == <f64>(238.375));
assert(value.covariance[27] == <f64>(194.0));
assert(value.covariance[28] == <f64>(-8.625));
assert(value.covariance[29] == <f64>(-77.5));
assert(value.covariance[30] == <f64>(-13.125));
assert(value.covariance[31] == <f64>(68.5));
assert(value.covariance[32] == <f64>(-139.0));
assert(value.covariance[33] == <f64>(-235.875));
assert(value.covariance[34] == <f64>(-215.875));
assert(value.covariance[35] == <f64>(-102.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("8431bd7b9987025f0e371c28070000007661613330ceb2000000000078544000000000007046c00000000000046dc00000000000d853400000000000a854c00000000000f0434000000000001c6f4000000000007044c000000000000010c000000000008c624000000000001c68c00000000000486240000000000020374000000000008035c000000000004026c000000000002067400000000000f86bc00000000000e46bc000000000004069c00000000000c022c00000000000286240000000000038604000000000004022c00000000000c04ec00000000000b850c00000000000e05e400000000000546a4000000000000053400000000000c856400000000000f867400000000000905fc00000000000c044c000000000004c67c000000000003855400000000000a05d400000000000e03d400000000000c05b400000000000385f4000000000001064400000000000805dc0000000000090494000000000007c6ec00000000000ec6140");const codec=new r.Ros1Geometry_msgsPoseWithCovarianceStampedCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(2075996548));
assert(value.header.stamp.sec == <u32>(1594001305));
assert(value.header.stamp.nsec == <u32>(672937742));
assert(value.header.frame_id == "vaa30β");
assert(value.pose.pose.position.x == <f64>(81.875));
assert(value.pose.pose.position.y == <f64>(-44.875));
assert(value.pose.pose.position.z == <f64>(-232.125));
assert(value.pose.pose.orientation.x == <f64>(79.375));
assert(value.pose.pose.orientation.y == <f64>(-82.625));
assert(value.pose.pose.orientation.z == <f64>(39.875));
assert(value.pose.pose.orientation.w == <f64>(248.875));
assert(value.pose.covariance.length == 36);
assert(value.pose.covariance[0] == <f64>(-40.875));
assert(value.pose.covariance[1] == <f64>(-4.0));
assert(value.pose.covariance[2] == <f64>(148.375));
assert(value.pose.covariance[3] == <f64>(-192.875));
assert(value.pose.covariance[4] == <f64>(146.25));
assert(value.pose.covariance[5] == <f64>(23.125));
assert(value.pose.covariance[6] == <f64>(-21.5));
assert(value.pose.covariance[7] == <f64>(-11.125));
assert(value.pose.covariance[8] == <f64>(185.0));
assert(value.pose.covariance[9] == <f64>(-223.75));
assert(value.pose.covariance[10] == <f64>(-223.125));
assert(value.pose.covariance[11] == <f64>(-202.0));
assert(value.pose.covariance[12] == <f64>(-9.375));
assert(value.pose.covariance[13] == <f64>(145.25));
assert(value.pose.covariance[14] == <f64>(129.75));
assert(value.pose.covariance[15] == <f64>(-9.125));
assert(value.pose.covariance[16] == <f64>(-61.5));
assert(value.pose.covariance[17] == <f64>(-66.875));
assert(value.pose.covariance[18] == <f64>(123.5));
assert(value.pose.covariance[19] == <f64>(210.625));
assert(value.pose.covariance[20] == <f64>(76.0));
assert(value.pose.covariance[21] == <f64>(91.125));
assert(value.pose.covariance[22] == <f64>(191.75));
assert(value.pose.covariance[23] == <f64>(-126.25));
assert(value.pose.covariance[24] == <f64>(-41.5));
assert(value.pose.covariance[25] == <f64>(-186.375));
assert(value.pose.covariance[26] == <f64>(84.875));
assert(value.pose.covariance[27] == <f64>(118.5));
assert(value.pose.covariance[28] == <f64>(29.875));
assert(value.pose.covariance[29] == <f64>(111.0));
assert(value.pose.covariance[30] == <f64>(124.875));
assert(value.pose.covariance[31] == <f64>(160.5));
assert(value.pose.covariance[32] == <f64>(-118.0));
assert(value.pose.covariance[33] == <f64>(51.125));
assert(value.pose.covariance[34] == <f64>(-243.875));
assert(value.pose.covariance[35] == <f64>(143.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("0000000000905040000000000008624000000000005c6fc000000000003051c0");const codec=new r.Ros1Geometry_msgsQuaternionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(66.25));
assert(value.y == <f64>(144.25));
assert(value.z == <f64>(-250.875));
assert(value.w == <f64>(-68.75));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("0000000000e46d4000000000006c6b4000000000000029c000000000001855400000000000e035400000000000d462c00000000000504b40");const codec=new r.Ros1Geometry_msgsTransformCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.translation.x == <f64>(239.125));
assert(value.translation.y == <f64>(219.375));
assert(value.translation.z == <f64>(-12.5));
assert(value.rotation.x == <f64>(84.375));
assert(value.rotation.y == <f64>(21.875));
assert(value.rotation.z == <f64>(-150.625));
assert(value.rotation.w == <f64>(54.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("e83eb6c1ae23acbda5ac1729070000007662636565ceb2070000007639643537ceb200000000004c6b400000000000b46e4000000000001059c00000000000a854c00000000000e03a400000000000485ac000000000001867c0");const codec=new r.Ros1Geometry_msgsTransformStampedCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3249946344));
assert(value.header.stamp.sec == <u32>(3182175150));
assert(value.header.stamp.nsec == <u32>(689417381));
assert(value.header.frame_id == "vbceeβ");
assert(value.child_frame_id == "v9d57β");
assert(value.transform.translation.x == <f64>(218.375));
assert(value.transform.translation.y == <f64>(245.625));
assert(value.transform.translation.z == <f64>(-100.25));
assert(value.transform.rotation.x == <f64>(-82.625));
assert(value.transform.rotation.y == <f64>(26.875));
assert(value.transform.rotation.z == <f64>(-105.125));
assert(value.transform.rotation.w == <f64>(-184.75));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("0000000000785140000000000000c03f0000000000985e4000000000009045400000000000885d400000000000385cc0");const codec=new r.Ros1Geometry_msgsTwistCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.linear.x == <f64>(69.875));
assert(value.linear.y == <f64>(0.125));
assert(value.linear.z == <f64>(122.375));
assert(value.angular.x == <f64>(43.125));
assert(value.angular.y == <f64>(118.125));
assert(value.angular.z == <f64>(-112.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("00000000002850c00000000000b04cc000000000009461c0");const codec=new r.Ros1Geometry_msgsVector3Codec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(-64.625));
assert(value.y == <f64>(-57.375));
assert(value.z == <f64>(-140.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("0000000000a85b4000000000003040c00000000000346240000000000000014000000000001c63400000000000f061c0");const codec=new r.Ros1Geometry_msgsWrenchCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.force.x == <f64>(110.625));
assert(value.force.y == <f64>(-32.375));
assert(value.force.z == <f64>(145.625));
assert(value.torque.x == <f64>(2.125));
assert(value.torque.y == <f64>(152.875));
assert(value.torque.z == <f64>(-143.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("0000000000d46fc000000000002467c000000000004046c09e");const codec=new r.Ros1Humanoid_nav_msgsStepTargetCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pose.x == <f64>(-254.625));
assert(value.pose.y == <f64>(-185.125));
assert(value.pose.theta == <f64>(-44.5));
assert(value.leg == <u8>(158));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("7490fd23dc02a00a00e022438cf1ea7ee9e760ba0000000000c866400000000000646ec00000000000f859c00000000000d064400000000000706f400000000000a85dc00000000000206940");const codec=new r.Ros1Nav_msgsMapMetaDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.map_load_time.sec == <u32>(603820148));
assert(value.map_load_time.nsec == <u32>(178258652));
assert(value.resolution == <f32>(162.875));
assert(value.width == <u32>(2129326476));
assert(value.height == <u32>(3126913001));
assert(value.origin.position.x == <f64>(182.25));
assert(value.origin.position.y == <f64>(-243.125));
assert(value.origin.position.z == <f64>(-103.875));
assert(value.origin.orientation.x == <f64>(166.5));
assert(value.origin.orientation.y == <f64>(251.5));
assert(value.origin.orientation.z == <f64>(-118.625));
assert(value.origin.orientation.w == <f64>(201.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("070000007661323936ceb2070000007632313633ceb2");const codec=new r.Ros1Pal_behaviour_msgsBehaviourArgumentCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.key == "va296β");
assert(value.value == "v2163β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("070000007632323164ceb201000000070000007630663366ceb2070000007637393665ceb2");const codec=new r.Ros1Pal_behaviour_msgsBehaviourEventRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "v221dβ");
assert(value.arguments.length == 1);
assert(value.arguments[0].key == "v0f3fβ");
assert(value.arguments[0].value == "v796eβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("070000007634646535ceb2");const codec=new r.Ros1Pal_behaviour_msgsBehaviourEventResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response == "v4de5β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("");const codec=new r.Ros1Pal_behaviour_msgsBehaviourTaskFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("070000007630353634ceb201000000070000007630346535ceb2070000007665373334ceb2");const codec=new r.Ros1Pal_behaviour_msgsBehaviourTaskGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.taskName == "v0564β");
assert(value.arguments.length == 1);
assert(value.arguments[0].key == "v04e5β");
assert(value.arguments[0].value == "ve734β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("");const codec=new r.Ros1Pal_behaviour_msgsBehaviourTaskResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("");const codec=new r.Ros1Pal_behaviour_msgsPresentationFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_25():void{const input=bytes("070000007630363735ceb2");const codec=new r.Ros1Pal_behaviour_msgsPresentationGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.presentationFile == "v0675β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_26():void{const input=bytes("");const codec=new r.Ros1Pal_behaviour_msgsPresentationResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_27():void{const input=bytes("");const codec=new r.Ros1Pal_common_msgsDisableFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_28():void{const input=bytes("0040b0c2");const codec=new r.Ros1Pal_common_msgsDisableGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.duration == <f32>(-88.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_29():void{const input=bytes("");const codec=new r.Ros1Pal_common_msgsDisableResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_30():void{const input=bytes("");const codec=new r.Ros1Pal_common_msgsEmptyFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_31():void{const input=bytes("");const codec=new r.Ros1Pal_common_msgsEmptyGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_32():void{const input=bytes("");const codec=new r.Ros1Pal_common_msgsEmptyResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_33():void{const input=bytes("");const codec=new r.Ros1Pal_common_msgsGetStringRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_34():void{const input=bytes("070000007666363666ceb201");const codec=new r.Ros1Pal_common_msgsGetStringResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data == "vf66fβ");
assert(value.success == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_35():void{const input=bytes("070000007637396139ceb200202cc3");const codec=new r.Ros1Pal_common_msgsJointCurrentCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.joints == "v79a9β");
assert(value.current_limit == <f32>(-172.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_36():void{const input=bytes("070000007663366662ceb2");const codec=new r.Ros1Pal_common_msgsSetStringRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data == "vc6fbβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_37():void{const input=bytes("00");const codec=new r.Ros1Pal_common_msgsSetStringResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.success == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_38():void{const input=bytes("01000000070000007665666433ceb20100000000000000000027c0");const codec=new r.Ros1Pal_control_msgsActuatorCurrentLimitCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.actuator_names.length == 1);
assert(value.actuator_names[0] == "vefd3β");
assert(value.current_limits.length == 1);
assert(value.current_limits[0] == <f64>(-11.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_39():void{const input=bytes("070000007661663462ceb200000000009c6840");const codec=new r.Ros1Pal_control_msgsCurrentLimitRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.actuator_name == "vaf4bβ");
assert(value.current_limit == <f64>(196.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_40():void{const input=bytes("070000007638646431ceb2");const codec=new r.Ros1Pal_control_msgsCurrentLimitResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == "v8dd1β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_41():void{const input=bytes("");const codec=new r.Ros1Pal_control_msgsMotionManagerFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_42():void{const input=bytes("070000007663316438ceb2010001012bd17d9c");const codec=new r.Ros1Pal_control_msgsMotionManagerGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.filename == "vc1d8β");
assert(value.plan == true);
assert(value.checkSafety == false);
assert(value.repeat == true);
assert(value.priority == <u8>(1));
assert(value.queueTimeout == <i32>(-1669476053));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_43():void{const input=bytes("070000007633633565ceb2");const codec=new r.Ros1Pal_control_msgsMotionManagerResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.message == "v3c5eβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_44():void{const input=bytes("6fd1ae4f3a1e9b6ac56a5d2c070000007638363031ceb20001bce986752751992800000000004463c00000000000c85ac00000000000f466c000000000000012400000000000404ec00000000000206bc000000000009462c00000000000f46e400000000000cc6f40000000000034654000000000002866c000000000006060c00000000000e04f400000000000d461c00000000000801dc000000000000c60400000000000005a400000000000406a4000000000007467c0");const codec=new r.Ros1Pal_control_msgsOperationalSpaceGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1336856943));
assert(value.header.stamp.sec == <u32>(1788550714));
assert(value.header.stamp.nsec == <u32>(744319685));
assert(value.header.frame_id == "v8601β");
assert(value.has_vel == false);
assert(value.has_accel == true);
assert(value.duration.sec == <i32>(1971775932));
assert(value.duration.nsec == <i32>(681136423));
assert(value.pose.position.x == <f64>(-154.125));
assert(value.pose.position.y == <f64>(-107.125));
assert(value.pose.position.z == <f64>(-183.625));
assert(value.pose.orientation.x == <f64>(4.5));
assert(value.pose.orientation.y == <f64>(-60.5));
assert(value.pose.orientation.z == <f64>(-217.0));
assert(value.pose.orientation.w == <f64>(-148.625));
assert(value.velocity.linear.x == <f64>(247.625));
assert(value.velocity.linear.y == <f64>(254.375));
assert(value.velocity.linear.z == <f64>(169.625));
assert(value.velocity.angular.x == <f64>(-177.25));
assert(value.velocity.angular.y == <f64>(-131.0));
assert(value.velocity.angular.z == <f64>(63.75));
assert(value.acceleration.linear.x == <f64>(-142.625));
assert(value.acceleration.linear.y == <f64>(-7.375));
assert(value.acceleration.linear.z == <f64>(128.375));
assert(value.acceleration.angular.x == <f64>(104.0));
assert(value.acceleration.angular.y == <f64>(210.0));
assert(value.acceleration.angular.z == <f64>(-187.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_45():void{const input=bytes("");const codec=new r.Ros1Pal_control_msgsRigidBodyTrajectoryFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_46():void{const input=bytes("e5564d68b5fbbe2f91078515070000007637636238ceb29d01000000edde607672d6f63400000000007c6f4000000000002860c000000000008020400000000000dc6cc0000000000048614000000000007053c00000000000446ac00000000000d466c000000000002466c00000000000085fc00000000000246cc00000000000b85fc00000000000c0214000000000001050400000000000f0634000000000005c6ac000000000008039c000000000001860c000000000000008c0");const codec=new r.Ros1Pal_control_msgsRigidBodyTrajectoryGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1749898981));
assert(value.header.stamp.sec == <u32>(801045429));
assert(value.header.stamp.nsec == <u32>(361039761));
assert(value.header.frame_id == "v7cb8β");
assert(value.interpolation_type == <i8>(-99));
assert(value.trajectory.length == 1);
assert(value.trajectory[0].duration.sec == <i32>(1986060013));
assert(value.trajectory[0].duration.nsec == <i32>(888591986));
assert(value.trajectory[0].pose.position.x == <f64>(251.875));
assert(value.trajectory[0].pose.position.y == <f64>(-129.25));
assert(value.trajectory[0].pose.position.z == <f64>(8.25));
assert(value.trajectory[0].pose.orientation.x == <f64>(-230.875));
assert(value.trajectory[0].pose.orientation.y == <f64>(138.25));
assert(value.trajectory[0].pose.orientation.z == <f64>(-77.75));
assert(value.trajectory[0].pose.orientation.w == <f64>(-210.125));
assert(value.trajectory[0].velocity.linear.x == <f64>(-182.625));
assert(value.trajectory[0].velocity.linear.y == <f64>(-177.125));
assert(value.trajectory[0].velocity.linear.z == <f64>(-124.125));
assert(value.trajectory[0].velocity.angular.x == <f64>(-225.125));
assert(value.trajectory[0].velocity.angular.y == <f64>(-126.875));
assert(value.trajectory[0].velocity.angular.z == <f64>(8.875));
assert(value.trajectory[0].acceleration.linear.x == <f64>(64.25));
assert(value.trajectory[0].acceleration.linear.y == <f64>(159.5));
assert(value.trajectory[0].acceleration.linear.z == <f64>(-210.875));
assert(value.trajectory[0].acceleration.angular.x == <f64>(-25.5));
assert(value.trajectory[0].acceleration.angular.y == <f64>(-128.75));
assert(value.trajectory[0].acceleration.angular.z == <f64>(-3.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_47():void{const input=bytes("9312887685ce150c00000000006863400000000000386d4000000000002868400000000000a06d400000000000c0674000000000008051c000000000003c62c00000000000f850c00000000000246fc00000000000a865400000000000dc6dc000000000004859c00000000000504d400000000000fc69c00000000000d856c000000000009042400000000000d462400000000000906ac00000000000e46d40");const codec=new r.Ros1Pal_control_msgsRigidBodyTrajectoryPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.duration.sec == <i32>(1988629139));
assert(value.duration.nsec == <i32>(202755717));
assert(value.pose.position.x == <f64>(155.25));
assert(value.pose.position.y == <f64>(233.75));
assert(value.pose.position.z == <f64>(193.25));
assert(value.pose.orientation.x == <f64>(237.0));
assert(value.pose.orientation.y == <f64>(190.0));
assert(value.pose.orientation.z == <f64>(-70.0));
assert(value.pose.orientation.w == <f64>(-145.875));
assert(value.velocity.linear.x == <f64>(-67.875));
assert(value.velocity.linear.y == <f64>(-249.125));
assert(value.velocity.linear.z == <f64>(173.25));
assert(value.velocity.angular.x == <f64>(-238.875));
assert(value.velocity.angular.y == <f64>(-101.125));
assert(value.velocity.angular.z == <f64>(58.625));
assert(value.acceleration.linear.x == <f64>(-207.875));
assert(value.acceleration.linear.y == <f64>(-91.375));
assert(value.acceleration.linear.z == <f64>(37.125));
assert(value.acceleration.angular.x == <f64>(150.625));
assert(value.acceleration.angular.y == <f64>(-212.5));
assert(value.acceleration.angular.z == <f64>(239.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_48():void{const input=bytes("070000007663646331ceb2");const codec=new r.Ros1Pal_control_msgsRigidBodyTrajectoryResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.message == "vcdc1β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_49():void{const input=bytes("e589302f39f09bc891dd2503070000007665313965ceb2b0a940f52ffa5681070000007637343766ceb2aab6c06c6201000000a4070000007664643964ceb2");const codec=new r.Ros1Pal_detection_msgsAddTexturedObjectRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.img.header.seq == <u32>(791710181));
assert(value.img.header.stamp.sec == <u32>(3365662777));
assert(value.img.header.stamp.nsec == <u32>(52813201));
assert(value.img.header.frame_id == "ve19eβ");
assert(value.img.height == <u32>(4114655664));
assert(value.img.width == <u32>(2169961007));
assert(value.img.encoding == "v747fβ");
assert(value.img.is_bigendian == <u8>(170));
assert(value.img.step == <u32>(1651294390));
assert(value.img.data.length == 1);
assert(value.img.data[0] == <u8>(164));
assert(value.fullPathFileName == "vdd9dβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_50():void{const input=bytes("00");const codec=new r.Ros1Pal_detection_msgsAddTexturedObjectResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.result == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_51():void{const input=bytes("070000007662303463ceb200");const codec=new r.Ros1Pal_detection_msgsChangeObjectRecognizerModelRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.model_name == "vb04cβ");
assert(value.reset_desired_classes_param == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_52():void{const input=bytes("00");const codec=new r.Ros1Pal_detection_msgsChangeObjectRecognizerModelResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_53():void{const input=bytes("e38123f35af8146d75b12484ccc8131a87b8233e3ec114a0c0b2e60179cbd19f");const codec=new r.Ros1Pal_detection_msgsDetection2dCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <i64><u64>0x6d14f85af32381e3);
assert(value.y == <i64><u64>0x1a13c8cc8424b175);
assert(value.width == <i64><u64>0xa014c13e3e23b887);
assert(value.height == <i64><u64>0x9fd1cb7901e6b2c0);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_54():void{const input=bytes("21463d81267b2b08ef863e38070000007665623739ceb2010000009fee4ed62697794809de49a1b0a77e3f13ea16c7aa932159cfd07df676a94a68431449c43c31c89ddf3ef912070000007662373739ceb2070000007664316139ceb20000000000806dc00000000000746940000000000080124000000000000004c00000000000f05b4000000000004048c00000000000fc6ac0");const codec=new r.Ros1Pal_detection_msgsDetections2dCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(2168276513));
assert(value.header.stamp.sec == <u32>(137067302));
assert(value.header.stamp.nsec == <u32>(943621871));
assert(value.header.frame_id == "veb79β");
assert(value.detections.length == 1);
assert(value.detections[0].x == <i64><u64>0x48799726d64eee9f);
assert(value.detections[0].y == <i64><u64>0x3f7ea7b0a149de09);
assert(value.detections[0].width == <i64><u64>0x592193aac716ea13);
assert(value.detections[0].height == <i64><u64>0x684aa976f67dd0cf);
assert(value.camera_pose.header.seq == <u32>(3293123651));
assert(value.camera_pose.header.stamp.sec == <u32>(2647142716));
assert(value.camera_pose.header.stamp.nsec == <u32>(318324447));
assert(value.camera_pose.header.frame_id == "vb779β");
assert(value.camera_pose.child_frame_id == "vd1a9β");
assert(value.camera_pose.transform.translation.x == <f64>(-236.0));
assert(value.camera_pose.transform.translation.y == <f64>(203.625));
assert(value.camera_pose.transform.translation.z == <f64>(4.625));
assert(value.camera_pose.transform.rotation.x == <f64>(-2.5));
assert(value.camera_pose.transform.rotation.y == <f64>(111.75));
assert(value.camera_pose.transform.rotation.z == <f64>(-48.5));
assert(value.camera_pose.transform.rotation.w == <f64>(-215.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_55():void{const input=bytes("d6bb6ae5408b6d924ea49827203653cf017e9f016ee8af061992eac7b804dac0cf00200cc3002019c300800743070000007630646666ceb2008031c2070000007666643539ceb200005743070000007662646533ceb20000614200c059430000174300401e4300c0524300a03e4300408fc2008042c3");const codec=new r.Ros1Pal_detection_msgsFaceDetectionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <i32>(-445989930));
assert(value.y == <i32>(-1838314688));
assert(value.width == <i32>(664314958));
assert(value.height == <i32>(-816630240));
assert(value.eyesLocated == true);
assert(value.leftEyeX == <i32>(1845600126));
assert(value.leftEyeY == <i32>(419868648));
assert(value.rightEyeX == <i32>(-1194857838));
assert(value.rightEyeY == <i32>(-809444860));
assert(value.position.x == <f32>(-140.125));
assert(value.position.y == <f32>(-153.125));
assert(value.position.z == <f32>(135.5));
assert(value.name == "v0dffβ");
assert(value.confidence == <f32>(-44.375));
assert(value.gender == "vfd59β");
assert(value.genderConfidence == <f32>(215.0));
assert(value.expression == "vbde3β");
assert(value.expression_confidence == <f32>(56.25));
assert(value.emotion_anger_confidence == <f32>(217.75));
assert(value.emotion_disgust_confidence == <f32>(151.0));
assert(value.emotion_fear_confidence == <f32>(158.25));
assert(value.emotion_happiness_confidence == <f32>(210.75));
assert(value.emotion_neutral_confidence == <f32>(190.625));
assert(value.emotion_sadness_confidence == <f32>(-71.625));
assert(value.emotion_surprise_confidence == <f32>(-194.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_56():void{const input=bytes("0de3ca355fd66419da00f611070000007661356466ceb201000000a447a7633277a014fbc9ff204e165f7400934211ae057216d98a64bc661c54bb1100e00fc30040ffc200c05f43070000007630373536ceb20000dc41070000007634363535ceb2000054c1070000007661653531ceb20080164300007041000049c2008012c30000ba41002054430040c8c200a01fc3419240be0977c8cabac4f71b070000007664663861ceb2070000007666663061ceb2000000000050544000000000000862400000000000b05fc00000000000504fc000000000005467c00000000000c066400000000000446e40");const codec=new r.Ros1Pal_detection_msgsFaceDetectionsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(902488845));
assert(value.header.stamp.sec == <u32>(426038879));
assert(value.header.stamp.nsec == <u32>(301334746));
assert(value.header.frame_id == "va5dfβ");
assert(value.faces.length == 1);
assert(value.faces[0].x == <i32>(1671907236));
assert(value.faces[0].y == <i32>(346060594));
assert(value.faces[0].width == <i32>(553634299));
assert(value.faces[0].height == <i32>(1952388686));
assert(value.faces[0].eyesLocated == false);
assert(value.faces[0].leftEyeX == <i32>(-1374600557));
assert(value.faces[0].leftEyeY == <i32>(-652840443));
assert(value.faces[0].rightEyeX == <i32>(1723622538));
assert(value.faces[0].rightEyeY == <i32>(297489436));
assert(value.faces[0].position.x == <f32>(-143.875));
assert(value.faces[0].position.y == <f32>(-127.625));
assert(value.faces[0].position.z == <f32>(223.75));
assert(value.faces[0].name == "v0756β");
assert(value.faces[0].confidence == <f32>(27.5));
assert(value.faces[0].gender == "v4655β");
assert(value.faces[0].genderConfidence == <f32>(-13.25));
assert(value.faces[0].expression == "vae51β");
assert(value.faces[0].expression_confidence == <f32>(150.5));
assert(value.faces[0].emotion_anger_confidence == <f32>(15.0));
assert(value.faces[0].emotion_disgust_confidence == <f32>(-50.25));
assert(value.faces[0].emotion_fear_confidence == <f32>(-146.5));
assert(value.faces[0].emotion_happiness_confidence == <f32>(23.25));
assert(value.faces[0].emotion_neutral_confidence == <f32>(212.125));
assert(value.faces[0].emotion_sadness_confidence == <f32>(-100.125));
assert(value.faces[0].emotion_surprise_confidence == <f32>(-159.625));
assert(value.camera_pose.header.seq == <u32>(3191902785));
assert(value.camera_pose.header.stamp.sec == <u32>(3402135305));
assert(value.camera_pose.header.stamp.nsec == <u32>(469222586));
assert(value.camera_pose.header.frame_id == "vdf8aβ");
assert(value.camera_pose.child_frame_id == "vff0aβ");
assert(value.camera_pose.transform.translation.x == <f64>(81.25));
assert(value.camera_pose.transform.translation.y == <f64>(144.25));
assert(value.camera_pose.transform.translation.z == <f64>(-126.75));
assert(value.camera_pose.transform.rotation.x == <f64>(-62.625));
assert(value.camera_pose.transform.rotation.y == <f64>(-186.625));
assert(value.camera_pose.transform.rotation.z == <f64>(182.0));
assert(value.camera_pose.transform.rotation.w == <f64>(242.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_57():void{const input=bytes("a354da58297d8388789fcb30070000007632363138ceb20100000033c8e353adc8499f00f798f7ec409b3400");const codec=new r.Ros1Pal_detection_msgsFaceThermalDetectionsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1490703523));
assert(value.header.stamp.sec == <u32>(2290318633));
assert(value.header.stamp.nsec == <u32>(818651000));
assert(value.header.frame_id == "v2618β");
assert(value.faces.length == 1);
assert(value.faces[0].x_offset == <u32>(1407436851));
assert(value.faces[0].y_offset == <u32>(2672412845));
assert(value.faces[0].height == <u32>(4153997056));
assert(value.faces[0].width == <u32>(882589932));
assert(value.faces[0].do_rectify == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_58():void{const input=bytes("0f8167d3b33e30a5619c2606070000007639376234ceb2070000007664636230ceb20000000000246ac000000000006865400000000000b462c0");const codec=new r.Ros1Pal_detection_msgsGestureCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3546775823));
assert(value.header.stamp.sec == <u32>(2771402419));
assert(value.header.stamp.nsec == <u32>(103193697));
assert(value.header.frame_id == "v97b4β");
assert(value.gestureId == "vdcb0β");
assert(value.position3D.x == <f64>(-209.125));
assert(value.position3D.y == <f64>(171.25));
assert(value.position3D.z == <f64>(-149.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_59():void{const input=bytes("cee08f3f4cede5683f2c041e070000007639353631ceb2010000000000000000603e400000000000b046c000000000006858c0ab6283125cfab9814cf83111070000007661616433ceb2070000007663636262ceb20000000000002dc00000000000e056c00000000000e06dc00000000000c0544000000000006066400000000000406b400000000000205d40");const codec=new r.Ros1Pal_detection_msgsLegDetectionsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1066393806));
assert(value.header.stamp.sec == <u32>(1759898956));
assert(value.header.stamp.nsec == <u32>(503589951));
assert(value.header.frame_id == "v9561β");
assert(value.position3D.length == 1);
assert(value.position3D[0].x == <f64>(30.375));
assert(value.position3D[0].y == <f64>(-45.375));
assert(value.position3D[0].z == <f64>(-97.625));
assert(value.laser_pose.header.seq == <u32>(310600363));
assert(value.laser_pose.header.stamp.sec == <u32>(2176449116));
assert(value.laser_pose.header.stamp.nsec == <u32>(288487500));
assert(value.laser_pose.header.frame_id == "vaad3β");
assert(value.laser_pose.child_frame_id == "vccbbβ");
assert(value.laser_pose.transform.translation.x == <f64>(-14.5));
assert(value.laser_pose.transform.translation.y == <f64>(-91.5));
assert(value.laser_pose.transform.translation.z == <f64>(-239.0));
assert(value.laser_pose.transform.rotation.x == <f64>(83.0));
assert(value.laser_pose.transform.rotation.y == <f64>(179.0));
assert(value.laser_pose.transform.rotation.z == <f64>(218.0));
assert(value.laser_pose.transform.rotation.w == <f64>(116.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_60():void{const input=bytes("9414bde32d6d8a7d0000000000403f400000000000a045400000000000086e400000000000585d400000000000001ac00000000000e06dc000000000000c64c000c09c4200a069c3");const codec=new r.Ros1Pal_detection_msgsObstacleCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.id == <i64><u64>0x7d8a6d2de3bd1494);
assert(value.position.x == <f64>(31.25));
assert(value.position.y == <f64>(43.25));
assert(value.position.z == <f64>(240.25));
assert(value.position_covariance.length == 4);
assert(value.position_covariance[0] == <f64>(117.375));
assert(value.position_covariance[1] == <f64>(-6.5));
assert(value.position_covariance[2] == <f64>(-239.0));
assert(value.position_covariance[3] == <f64>(-160.375));
assert(value.width == <f32>(78.375));
assert(value.height == <f32>(-233.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_61():void{const input=bytes("64cf3aac10df0a46d20a5f09070000007663653565ceb2010000000c1d01d3b564364d0000000000e037c00000000000e85fc00000000000186d400000000000506a4000000000003c6a400000000000b050400000000000f46e4000e02a430040d0c2");const codec=new r.Ros1Pal_detection_msgsObstaclesCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(2889535332));
assert(value.header.stamp.sec == <u32>(1175117584));
assert(value.header.stamp.nsec == <u32>(157223634));
assert(value.header.frame_id == "vce5eβ");
assert(value.obstacles.length == 1);
assert(value.obstacles[0].id == <i64><u64>0x4d3664b5d3011d0c);
assert(value.obstacles[0].position.x == <f64>(-23.875));
assert(value.obstacles[0].position.y == <f64>(-127.625));
assert(value.obstacles[0].position.z == <f64>(232.75));
assert(value.obstacles[0].position_covariance.length == 4);
assert(value.obstacles[0].position_covariance[0] == <f64>(210.5));
assert(value.obstacles[0].position_covariance[1] == <f64>(209.875));
assert(value.obstacles[0].position_covariance[2] == <f64>(66.75));
assert(value.obstacles[0].position_covariance[3] == <f64>(247.625));
assert(value.obstacles[0].width == <f32>(170.875));
assert(value.obstacles[0].height == <f32>(-104.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_62():void{const input=bytes("58ace5c4e1d5d25ace9ce2b377e5d52d6871d8e1d108ef7fe0b68c3159cfbbaf8d670e541b57092300cdf002de32125e00150653e383365494db002a624d302d150000efc100801dc200c0e6c2070000007630613434ceb2006035c3070000007636633138ceb200204ac3070000007662323339ceb2006061430000a4400000c14100804ec300001ac100e067c300003dc30080b642f5cb39911b65b65be08d6a24070000007663356634ceb2010000000000000000846bc00000000000346cc00000000000146fc07d1acc2a2352736549609e0c070000007636656234ceb2070000007663613835ceb200000000005c6740000000000098524000000000007464c00000000000185fc000000000007c69c00000000000b040c00000000000405e40361b769be5bbdf8b25e1d03a070000007663633363ceb20000000000b864400000000000804f400000000000e86a40");const codec=new r.Ros1Pal_detection_msgsPersonDetectionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.full_body.x == <i64><u64>0x5ad2d5e1c4e5ac58);
assert(value.full_body.y == <i64><u64>0x2dd5e577b3e29cce);
assert(value.full_body.width == <i64><u64>0x7fef08d1e1d87168);
assert(value.full_body.height == <i64><u64>0xafbbcf59318cb6e0);
assert(value.face.x == <i32>(1410230157));
assert(value.face.y == <i32>(587814683));
assert(value.face.width == <i32>(49335552));
assert(value.face.height == <i32>(1578250974));
assert(value.face.eyesLocated == false);
assert(value.face.leftEyeX == <i32>(-481098219));
assert(value.face.leftEyeY == <i32>(-1806420349));
assert(value.face.rightEyeX == <i32>(1646919899));
assert(value.face.rightEyeY == <i32>(355283021));
assert(value.face.position.x == <f32>(-29.875));
assert(value.face.position.y == <f32>(-39.375));
assert(value.face.position.z == <f32>(-115.375));
assert(value.face.name == "v0a44β");
assert(value.face.confidence == <f32>(-181.375));
assert(value.face.gender == "v6c18β");
assert(value.face.genderConfidence == <f32>(-202.125));
assert(value.face.expression == "vb239β");
assert(value.face.expression_confidence == <f32>(225.375));
assert(value.face.emotion_anger_confidence == <f32>(5.125));
assert(value.face.emotion_disgust_confidence == <f32>(24.125));
assert(value.face.emotion_fear_confidence == <f32>(-206.5));
assert(value.face.emotion_happiness_confidence == <f32>(-9.625));
assert(value.face.emotion_neutral_confidence == <f32>(-231.875));
assert(value.face.emotion_sadness_confidence == <f32>(-189.0));
assert(value.face.emotion_surprise_confidence == <f32>(91.25));
assert(value.legs.header.seq == <u32>(2436484085));
assert(value.legs.header.stamp.sec == <u32>(1538680091));
assert(value.legs.header.stamp.nsec == <u32>(610962912));
assert(value.legs.header.frame_id == "vc5f4β");
assert(value.legs.position3D.length == 1);
assert(value.legs.position3D[0].x == <f64>(-220.125));
assert(value.legs.position3D[0].y == <f64>(-225.625));
assert(value.legs.position3D[0].z == <f64>(-248.625));
assert(value.legs.laser_pose.header.seq == <u32>(718019197));
assert(value.legs.laser_pose.header.stamp.sec == <u32>(1702056483));
assert(value.legs.laser_pose.header.stamp.nsec == <u32>(211705929));
assert(value.legs.laser_pose.header.frame_id == "v6eb4β");
assert(value.legs.laser_pose.child_frame_id == "vca85β");
assert(value.legs.laser_pose.transform.translation.x == <f64>(186.875));
assert(value.legs.laser_pose.transform.translation.y == <f64>(74.375));
assert(value.legs.laser_pose.transform.translation.z == <f64>(-163.625));
assert(value.legs.laser_pose.transform.rotation.x == <f64>(-124.375));
assert(value.legs.laser_pose.transform.rotation.y == <f64>(-203.875));
assert(value.legs.laser_pose.transform.rotation.z == <f64>(-33.375));
assert(value.legs.laser_pose.transform.rotation.w == <f64>(121.0));
assert(value.position3D.header.seq == <u32>(2608208694));
assert(value.position3D.header.stamp.sec == <u32>(2346695653));
assert(value.position3D.header.stamp.nsec == <u32>(986767653));
assert(value.position3D.header.frame_id == "vcc3cβ");
assert(value.position3D.point.x == <f64>(165.75));
assert(value.position3D.point.y == <f64>(63.0));
assert(value.position3D.point.z == <f64>(215.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_63():void{const input=bytes("78b892e4bdccba8f9493c923070000007637323664ceb20100000080b75c4f39ce6bd116875b38affe6ca6d6e3265d6f9a11c3f9d6e91d40afde834ff0527cd9c0550b5aff53050629abd501570bd720c13bd0576aa43bfafc943c8d00c0bdc200805b4200409342070000007663346232ceb2000098c2070000007665376131ceb200201143070000007630656337ceb200803c4200e04cc300a075430000b84100803ac300c0f3c2004085420080f1424b59c72d8e054d7a4b6d712d070000007638613630ceb20100000000000000000000c00000000000d058c000000000005063c0cccc023dca34ab3df314821a070000007664653965ceb2070000007633306535ceb20000000000402ec00000000000f04b4000000000005c6a400000000000705e400000000000d46ec00000000000f05d400000000000605fc0a37b8dba29f6ae37f38a8831070000007664626632ceb20000000000d0464000000000001c62400000000000c02fc003ad89f07ff268da362c5b2a070000007661353735ceb2070000007636613763ceb20000000000006ac00000000000b86fc00000000000d060c000000000003c624000000000004026c000000000002854c00000000000d46840");const codec=new r.Ros1Pal_detection_msgsPersonDetectionsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3834820728));
assert(value.header.stamp.sec == <u32>(2411383997));
assert(value.header.stamp.nsec == <u32>(600413076));
assert(value.header.frame_id == "v726dβ");
assert(value.persons.length == 1);
assert(value.persons[0].full_body.x == <i64><u64>0xd16bce394f5cb780);
assert(value.persons[0].full_body.y == <i64><u64>0xa66cfeaf385b8716);
assert(value.persons[0].full_body.width == <i64><u64>0xc3119a6f5d26e3d6);
assert(value.persons[0].full_body.height == <i64><u64>0x83deaf401de9d6f9);
assert(value.persons[0].face.x == <i32>(2085810255));
assert(value.persons[0].face.y == <i32>(190169305));
assert(value.persons[0].face.width == <i32>(89390938));
assert(value.persons[0].face.height == <i32>(-710203130));
assert(value.persons[0].face.eyesLocated == true);
assert(value.persons[0].face.leftEyeX == <i32>(550964055));
assert(value.persons[0].face.leftEyeY == <i32>(1473264577));
assert(value.persons[0].face.rightEyeX == <i32>(-96754582));
assert(value.persons[0].face.rightEyeY == <i32>(-1925409540));
assert(value.persons[0].face.position.x == <f32>(-94.875));
assert(value.persons[0].face.position.y == <f32>(54.875));
assert(value.persons[0].face.position.z == <f32>(73.625));
assert(value.persons[0].face.name == "vc4b2β");
assert(value.persons[0].face.confidence == <f32>(-76.0));
assert(value.persons[0].face.gender == "ve7a1β");
assert(value.persons[0].face.genderConfidence == <f32>(145.125));
assert(value.persons[0].face.expression == "v0ec7β");
assert(value.persons[0].face.expression_confidence == <f32>(47.125));
assert(value.persons[0].face.emotion_anger_confidence == <f32>(-204.875));
assert(value.persons[0].face.emotion_disgust_confidence == <f32>(245.625));
assert(value.persons[0].face.emotion_fear_confidence == <f32>(23.0));
assert(value.persons[0].face.emotion_happiness_confidence == <f32>(-186.5));
assert(value.persons[0].face.emotion_neutral_confidence == <f32>(-121.875));
assert(value.persons[0].face.emotion_sadness_confidence == <f32>(66.625));
assert(value.persons[0].face.emotion_surprise_confidence == <f32>(120.75));
assert(value.persons[0].legs.header.seq == <u32>(768039243));
assert(value.persons[0].legs.header.stamp.sec == <u32>(2051868046));
assert(value.persons[0].legs.header.stamp.nsec == <u32>(762408267));
assert(value.persons[0].legs.header.frame_id == "v8a60β");
assert(value.persons[0].legs.position3D.length == 1);
assert(value.persons[0].legs.position3D[0].x == <f64>(-2.0));
assert(value.persons[0].legs.position3D[0].y == <f64>(-99.25));
assert(value.persons[0].legs.position3D[0].z == <f64>(-154.5));
assert(value.persons[0].legs.laser_pose.header.seq == <u32>(1023593676));
assert(value.persons[0].legs.laser_pose.header.stamp.sec == <u32>(1034630346));
assert(value.persons[0].legs.laser_pose.header.stamp.nsec == <u32>(444732659));
assert(value.persons[0].legs.laser_pose.header.frame_id == "vde9eβ");
assert(value.persons[0].legs.laser_pose.child_frame_id == "v30e5β");
assert(value.persons[0].legs.laser_pose.transform.translation.x == <f64>(-15.125));
assert(value.persons[0].legs.laser_pose.transform.translation.y == <f64>(55.875));
assert(value.persons[0].legs.laser_pose.transform.translation.z == <f64>(210.875));
assert(value.persons[0].legs.laser_pose.transform.rotation.x == <f64>(121.75));
assert(value.persons[0].legs.laser_pose.transform.rotation.y == <f64>(-246.625));
assert(value.persons[0].legs.laser_pose.transform.rotation.z == <f64>(119.75));
assert(value.persons[0].legs.laser_pose.transform.rotation.w == <f64>(-125.5));
assert(value.persons[0].position3D.header.seq == <u32>(3129834403));
assert(value.persons[0].position3D.header.stamp.sec == <u32>(934213161));
assert(value.persons[0].position3D.header.stamp.nsec == <u32>(831032051));
assert(value.persons[0].position3D.header.frame_id == "vdbf2β");
assert(value.persons[0].position3D.point.x == <f64>(45.625));
assert(value.persons[0].position3D.point.y == <f64>(144.875));
assert(value.persons[0].position3D.point.z == <f64>(-15.875));
assert(value.camera_pose.header.seq == <u32>(4035554563));
assert(value.camera_pose.header.stamp.sec == <u32>(3664310911));
assert(value.camera_pose.header.stamp.nsec == <u32>(710618166));
assert(value.camera_pose.header.frame_id == "va575β");
assert(value.camera_pose.child_frame_id == "v6a7cβ");
assert(value.camera_pose.transform.translation.x == <f64>(-208.0));
assert(value.camera_pose.transform.translation.y == <f64>(-253.75));
assert(value.camera_pose.transform.translation.z == <f64>(-134.5));
assert(value.camera_pose.transform.rotation.x == <f64>(145.875));
assert(value.camera_pose.transform.rotation.y == <f64>(-11.125));
assert(value.camera_pose.transform.rotation.z == <f64>(-80.625));
assert(value.camera_pose.transform.rotation.w == <f64>(198.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_64():void{const input=bytes("");const codec=new r.Ros1Pal_detection_msgsRecognizeObjectsFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_65():void{const input=bytes("73e6fcfd1b5550aad0e75925070000007666373236ceb2070000007633633332ceb201000000fe");const codec=new r.Ros1Pal_detection_msgsRecognizeObjectsGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.input_image.header.seq == <u32>(4261209715));
assert(value.input_image.header.stamp.sec == <u32>(2857391387));
assert(value.input_image.header.stamp.nsec == <u32>(626649040));
assert(value.input_image.header.frame_id == "vf726β");
assert(value.input_image.format == "v3c32β");
assert(value.input_image.data.length == 1);
assert(value.input_image.data[0] == <u8>(254));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_66():void{const input=bytes("4d20693f057dc0ea9b888523070000007662383035ceb2070000007638343930ceb201000000a41c66f0f9a944310998e65a523308070000007635363661ceb201000000070000007635376362ceb2000018c112c36a278cc3c0eba2b0e1f6ade4ad9600");const codec=new r.Ros1Pal_detection_msgsRecognizeObjectsResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.image.header.seq == <u32>(1063854157));
assert(value.image.header.stamp.sec == <u32>(3938483461));
assert(value.image.header.stamp.nsec == <u32>(595953819));
assert(value.image.header.frame_id == "vb805β");
assert(value.image.format == "v8490β");
assert(value.image.data.length == 1);
assert(value.image.data[0] == <u8>(164));
assert(value.num_detections == <i16>(26140));
assert(value.recognized_objects.header.seq == <u32>(1151990256));
assert(value.recognized_objects.header.stamp.sec == <u32>(3868723505));
assert(value.recognized_objects.header.stamp.nsec == <u32>(137581146));
assert(value.recognized_objects.header.frame_id == "v566aβ");
assert(value.recognized_objects.objects.length == 1);
assert(value.recognized_objects.objects[0].object_class == "v57cbβ");
assert(value.recognized_objects.objects[0].confidence == <f32>(-9.5));
assert(value.recognized_objects.objects[0].bounding_box.x_offset == <u32>(661308178));
assert(value.recognized_objects.objects[0].bounding_box.y_offset == <u32>(3955278732));
assert(value.recognized_objects.objects[0].bounding_box.height == <u32>(4141985954));
assert(value.recognized_objects.objects[0].bounding_box.width == <u32>(2527978669));
assert(value.recognized_objects.objects[0].bounding_box.do_rectify == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_67():void{const input=bytes("fcadafdf8ca8e488ca03bd0e070000007634316364ceb201000000070000007666313132ceb20100000008811663b1f821fd01000000e62ea3715f5794ef");const codec=new r.Ros1Pal_detection_msgsRecognizedActionsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3752832508));
assert(value.header.stamp.sec == <u32>(2296686732));
assert(value.header.stamp.nsec == <u32>(247268298));
assert(value.header.frame_id == "v41cdβ");
assert(value.action_name.length == 1);
assert(value.action_name[0] == "vf112β");
assert(value.u.length == 1);
assert(value.u[0] == <i64><u64>0xfd21f8b163168108);
assert(value.v.length == 1);
assert(value.v[0] == <i64><u64>0xef94575f71a32ee6);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_68():void{const input=bytes("070000007632343737ceb200402b43b7e1b78129e11d4d7cc6dce83f5229aa01");const codec=new r.Ros1Pal_detection_msgsRecognizedObjectCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.object_class == "v2477β");
assert(value.confidence == <f32>(171.25));
assert(value.bounding_box.x_offset == <u32>(2176311735));
assert(value.bounding_box.y_offset == <u32>(1293803817));
assert(value.bounding_box.height == <u32>(3906782844));
assert(value.bounding_box.width == <u32>(2854834751));
assert(value.bounding_box.do_rectify == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_69():void{const input=bytes("c2e562e72a5f4870e0a9f031070000007638663934ceb201000000070000007666393162ceb200800cc29e9e0527009eafeb74511a1b2d12cfd501");const codec=new r.Ros1Pal_detection_msgsRecognizedObjectArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3882018242));
assert(value.header.stamp.sec == <u32>(1883791146));
assert(value.header.stamp.nsec == <u32>(837855712));
assert(value.header.frame_id == "v8f94β");
assert(value.objects.length == 1);
assert(value.objects[0].object_class == "vf91bβ");
assert(value.objects[0].confidence == <f32>(-35.125));
assert(value.objects[0].bounding_box.x_offset == <u32>(654679710));
assert(value.objects[0].bounding_box.y_offset == <u32>(3954155008));
assert(value.objects[0].bounding_box.height == <u32>(454709620));
assert(value.objects[0].bounding_box.width == <u32>(3587117613));
assert(value.objects[0].bounding_box.do_rectify == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_70():void{const input=bytes("0100205543");const codec=new r.Ros1Pal_detection_msgsRecognizerRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.enabled == true);
assert(value.minConfidence == <f32>(213.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_71():void{const input=bytes("");const codec=new r.Ros1Pal_detection_msgsRecognizerResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_72():void{const input=bytes("8583370eacf3872e59407535070000007639656331ceb20100000074a59391cddca40f0100000011c22f29a8bb18b7");const codec=new r.Ros1Pal_detection_msgsRotatedDetection2dCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(238519173));
assert(value.header.stamp.sec == <u32>(780661676));
assert(value.header.stamp.nsec == <u32>(896876633));
assert(value.header.frame_id == "v9ec1β");
assert(value.x.length == 1);
assert(value.x[0] == <i64><u64>0x0fa4dccd9193a574);
assert(value.y.length == 1);
assert(value.y[0] == <i64><u64>0xb718bba8292fc211);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_73():void{const input=bytes("070000007637313235ceb20000000000d068c00000000000ec62c0");const codec=new r.Ros1Pal_detection_msgsSelectTexturedObjectRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.imageFullPathFileName == "v7125β");
assert(value.width == <f64>(-198.5));
assert(value.height == <f64>(-151.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_74():void{const input=bytes("01");const codec=new r.Ros1Pal_detection_msgsSelectTexturedObjectResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.result == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_75():void{const input=bytes("070000007664346330ceb200");const codec=new r.Ros1Pal_detection_msgsSetDatabaseRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.databaseName == "vd4c0β");
assert(value.purgeAll == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_76():void{const input=bytes("");const codec=new r.Ros1Pal_detection_msgsSetDatabaseResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_77():void{const input=bytes("070000007663656636ceb2");const codec=new r.Ros1Pal_detection_msgsStartEnrollmentRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vcef6β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_78():void{const input=bytes("01");const codec=new r.Ros1Pal_detection_msgsStartEnrollmentResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.result == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_79():void{const input=bytes("");const codec=new r.Ros1Pal_detection_msgsStopEnrollmentRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_80():void{const input=bytes("01a5374fd3070000007666373664ceb2");const codec=new r.Ros1Pal_detection_msgsStopEnrollmentResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.enrollment_ok == true);
assert(value.numFacesEnrolled == <i32>(-749783131));
assert(value.error_msg == "vf76dβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_81():void{const input=bytes("92986aecadb465b488c84d07070000007663333335ceb200989ceef09e4244ebce271c070000007665623863ceb2070000007630336535ceb2010000008ed4f97b4ed5ba6ffb8861791c070000007631366339ceb201000000aaf04f0d1389789301000000cf97f3b576eec42b");const codec=new r.Ros1Pal_detection_msgsTexturedObjectDetectionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3966408850));
assert(value.header.stamp.sec == <u32>(3026564269));
assert(value.header.stamp.nsec == <u32>(122538120));
assert(value.header.frame_id == "vc335β");
assert(value.img.header.seq == <u32>(4003239936));
assert(value.img.header.stamp.sec == <u32>(1145216752));
assert(value.img.header.stamp.nsec == <u32>(472370923));
assert(value.img.header.frame_id == "veb8cβ");
assert(value.img.format == "v03e5β");
assert(value.img.data.length == 1);
assert(value.img.data[0] == <u8>(142));
assert(value.roi.header.seq == <u32>(1316747732));
assert(value.roi.header.stamp.sec == <u32>(4218403541));
assert(value.roi.header.stamp.nsec == <u32>(477716872));
assert(value.roi.header.frame_id == "v16c9β");
assert(value.roi.x.length == 1);
assert(value.roi.x[0] == <i64><u64>0x937889130d4ff0aa);
assert(value.roi.y.length == 1);
assert(value.roi.y[0] == <i64><u64>0x2bc4ee76b5f397cf);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_82():void{const input=bytes("685ae3f749809b13ddec5732070000007635613237ceb201f04a");const codec=new r.Ros1Pal_detection_msgsWaveDetectionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(4158872168));
assert(value.header.stamp.sec == <u32>(328958025));
assert(value.header.stamp.nsec == <u32>(844623069));
assert(value.header.frame_id == "v5a27β");
assert(value.wave == true);
assert(value.u == <i8>(-16));
assert(value.v == <i8>(74));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_83():void{const input=bytes("81008043c2");const codec=new r.Ros1Pal_device_msgsBatteryStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.chargeState == <i8>(-127));
assert(value.batteryPercentage == <f32>(-48.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_84():void{const input=bytes("6713f410a7a14dd683bf292b070000007633363364ceb201");const codec=new r.Ros1Pal_device_msgsBumperCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(284431207));
assert(value.header.stamp.sec == <u32>(3595411879));
assert(value.header.stamp.nsec == <u32>(724156291));
assert(value.header.frame_id == "v363dβ");
assert(value.is_pressed == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_85():void{const input=bytes("88e89561");const codec=new r.Ros1Pal_device_msgsCancelEffectRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effectId == <u32>(1637214344));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_86():void{const input=bytes("");const codec=new r.Ros1Pal_device_msgsCancelEffectResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_87():void{const input=bytes("");const codec=new r.Ros1Pal_device_msgsDoTimedLedEffectFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_88():void{const input=bytes("01000000be3d5debb200c00d4300a01fc3000051c300c0a542e966b93ed3cbd3330000e3c10000ae41008003c20040f0420000fa4200003fc200800e4300007743fa86c469ed996a0401000011c2004020c300e06cc300e006c3008035c2000000c20080da4200006cc17f83824c47c6cd2ac847128aacdcd8070080e742008018c200401d4300c0eac200007bc20000464200e00ec300000f4300001e4100000ac300004a4100c0edc20000c84000c000c300c07ac30000d4c10040b44200603f4300005441002068c330070000007638383534ceb2010000000000674200c0e1420000a4420080e64233eb9b2394082e1a6f");const codec=new r.Ros1Pal_device_msgsDoTimedLedEffectGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.devices.length == 1);
assert(value.devices[0] == <u32>(3948756414));
assert(value.params.effectType == <u8>(178));
assert(value.params.fixed_color.color.r == <f32>(141.75));
assert(value.params.fixed_color.color.g == <f32>(-159.625));
assert(value.params.fixed_color.color.b == <f32>(-209.0));
assert(value.params.fixed_color.color.a == <f32>(82.875));
assert(value.params.rainbow.transition_duration.sec == <i32>(1052337897));
assert(value.params.rainbow.transition_duration.nsec == <i32>(869518291));
assert(value.params.fade.first_color.r == <f32>(-28.375));
assert(value.params.fade.first_color.g == <f32>(21.75));
assert(value.params.fade.first_color.b == <f32>(-32.875));
assert(value.params.fade.first_color.a == <f32>(120.125));
assert(value.params.fade.second_color.r == <f32>(125.0));
assert(value.params.fade.second_color.g == <f32>(-47.75));
assert(value.params.fade.second_color.b == <f32>(142.5));
assert(value.params.fade.second_color.a == <f32>(247.0));
assert(value.params.fade.transition_duration.sec == <i32>(1774487290));
assert(value.params.fade.transition_duration.nsec == <i32>(74095085));
assert(value.params.fade.reverse_fade == true);
assert(value.params.blink.first_color.r == <f32>(-36.25));
assert(value.params.blink.first_color.g == <f32>(-160.25));
assert(value.params.blink.first_color.b == <f32>(-236.875));
assert(value.params.blink.first_color.a == <f32>(-134.875));
assert(value.params.blink.second_color.r == <f32>(-45.375));
assert(value.params.blink.second_color.g == <f32>(-32.0));
assert(value.params.blink.second_color.b == <f32>(109.25));
assert(value.params.blink.second_color.a == <f32>(-14.75));
assert(value.params.blink.first_color_duration.sec == <i32>(1283621759));
assert(value.params.blink.first_color_duration.nsec == <i32>(718128711));
assert(value.params.blink.second_color_duration.sec == <i32>(-1978513464));
assert(value.params.blink.second_color_duration.nsec == <i32>(131652780));
assert(value.params.progress.first_color.r == <f32>(115.75));
assert(value.params.progress.first_color.g == <f32>(-38.125));
assert(value.params.progress.first_color.b == <f32>(157.25));
assert(value.params.progress.first_color.a == <f32>(-117.375));
assert(value.params.progress.second_color.r == <f32>(-62.75));
assert(value.params.progress.second_color.g == <f32>(49.5));
assert(value.params.progress.second_color.b == <f32>(-142.875));
assert(value.params.progress.second_color.a == <f32>(143.0));
assert(value.params.progress.percentage == <f32>(9.875));
assert(value.params.progress.led_offset == <f32>(-138.0));
assert(value.params.flow.first_color.r == <f32>(12.625));
assert(value.params.flow.first_color.g == <f32>(-118.875));
assert(value.params.flow.first_color.b == <f32>(6.25));
assert(value.params.flow.first_color.a == <f32>(-128.75));
assert(value.params.flow.second_color.r == <f32>(-250.75));
assert(value.params.flow.second_color.g == <f32>(-26.5));
assert(value.params.flow.second_color.b == <f32>(90.125));
assert(value.params.flow.second_color.a == <f32>(191.375));
assert(value.params.flow.percentage == <f32>(13.25));
assert(value.params.flow.velocity == <f32>(-232.125));
assert(value.params.preprogrammed.preprogrammed_id == <u8>(48));
assert(value.params.effect_via_topic.topic_name == "v8854β");
assert(value.params.data_array.data.length == 1);
assert(value.params.data_array.data[0].r == <f32>(57.75));
assert(value.params.data_array.data[0].g == <f32>(112.875));
assert(value.params.data_array.data[0].b == <f32>(82.0));
assert(value.params.data_array.data[0].a == <f32>(115.25));
assert(value.effectDuration.sec == <i32>(597420851));
assert(value.effectDuration.nsec == <i32>(439224468));
assert(value.priority == <u8>(111));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_89():void{const input=bytes("");const codec=new r.Ros1Pal_device_msgsDoTimedLedEffectResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_90():void{const input=bytes("00e02ec3000078c000a051c300c0c0c200203cc300c0e54200601c430000a4c000a86a70c1e47a374ec3944a3d3ce003");const codec=new r.Ros1Pal_device_msgsLedBlinkParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.first_color.r == <f32>(-174.875));
assert(value.first_color.g == <f32>(-3.875));
assert(value.first_color.b == <f32>(-209.625));
assert(value.first_color.a == <f32>(-96.375));
assert(value.second_color.r == <f32>(-188.125));
assert(value.second_color.g == <f32>(114.875));
assert(value.second_color.b == <f32>(156.375));
assert(value.second_color.a == <f32>(-5.125));
assert(value.first_color_duration.sec == <i32>(1886038016));
assert(value.first_color_duration.nsec == <i32>(930800833));
assert(value.second_color_duration.sec == <i32>(1251263310));
assert(value.second_color_duration.nsec == <i32>(65027133));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_91():void{const input=bytes("0100000000e01c430080bc420000a94100e02d43");const codec=new r.Ros1Pal_device_msgsLedDataArrayParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data.length == 1);
assert(value.data[0].r == <f32>(156.875));
assert(value.data[0].g == <f32>(94.25));
assert(value.data[0].b == <f32>(21.125));
assert(value.data[0].a == <f32>(173.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_92():void{const input=bytes("ec00a064c300e032430000d6c100008e4268777e13742b0c3200a044c300002dc300c05ac300004b4200206b4300a0034300a03d43008011c2964da85a558ea204000000c34100600ec3004005c300e04d4300800443000056c100004543002039c3808f7c89c6d70a074956d5a70ba6cb3200004441000056c20000a0bf00403cc300601e430000514200c0404300605cc3000041c200204c430000e24200807ac200008e4200003242002072c3006043430080ab4200800fc30000134300e00b43cf070000007665373832ceb20100000000803c42000000c300004e4300402743");const codec=new r.Ros1Pal_device_msgsLedEffectParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effectType == <u8>(236));
assert(value.fixed_color.color.r == <f32>(-228.625));
assert(value.fixed_color.color.g == <f32>(178.875));
assert(value.fixed_color.color.b == <f32>(-26.75));
assert(value.fixed_color.color.a == <f32>(71.0));
assert(value.rainbow.transition_duration.sec == <i32>(327055208));
assert(value.rainbow.transition_duration.nsec == <i32>(839658356));
assert(value.fade.first_color.r == <f32>(-196.625));
assert(value.fade.first_color.g == <f32>(-173.0));
assert(value.fade.first_color.b == <f32>(-218.75));
assert(value.fade.first_color.a == <f32>(50.75));
assert(value.fade.second_color.r == <f32>(235.125));
assert(value.fade.second_color.g == <f32>(131.625));
assert(value.fade.second_color.b == <f32>(189.625));
assert(value.fade.second_color.a == <f32>(-36.375));
assert(value.fade.transition_duration.sec == <i32>(1520979350));
assert(value.fade.transition_duration.nsec == <i32>(77762133));
assert(value.fade.reverse_fade == false);
assert(value.blink.first_color.r == <f32>(24.375));
assert(value.blink.first_color.g == <f32>(-142.375));
assert(value.blink.first_color.b == <f32>(-133.25));
assert(value.blink.first_color.a == <f32>(205.875));
assert(value.blink.second_color.r == <f32>(132.5));
assert(value.blink.second_color.g == <f32>(-13.375));
assert(value.blink.second_color.b == <f32>(197.0));
assert(value.blink.second_color.a == <f32>(-185.125));
assert(value.blink.first_color_duration.sec == <i32>(-1988325504));
assert(value.blink.first_color_duration.nsec == <i32>(118151110));
assert(value.blink.second_color_duration.sec == <i32>(-1479190967));
assert(value.blink.second_color_duration.nsec == <i32>(852207115));
assert(value.progress.first_color.r == <f32>(12.25));
assert(value.progress.first_color.g == <f32>(-53.5));
assert(value.progress.first_color.b == <f32>(-1.25));
assert(value.progress.first_color.a == <f32>(-188.25));
assert(value.progress.second_color.r == <f32>(158.375));
assert(value.progress.second_color.g == <f32>(52.25));
assert(value.progress.second_color.b == <f32>(192.75));
assert(value.progress.second_color.a == <f32>(-220.375));
assert(value.progress.percentage == <f32>(-48.25));
assert(value.progress.led_offset == <f32>(204.125));
assert(value.flow.first_color.r == <f32>(113.0));
assert(value.flow.first_color.g == <f32>(-62.625));
assert(value.flow.first_color.b == <f32>(71.0));
assert(value.flow.first_color.a == <f32>(44.5));
assert(value.flow.second_color.r == <f32>(-242.125));
assert(value.flow.second_color.g == <f32>(195.375));
assert(value.flow.second_color.b == <f32>(85.75));
assert(value.flow.second_color.a == <f32>(-143.5));
assert(value.flow.percentage == <f32>(147.0));
assert(value.flow.velocity == <f32>(139.875));
assert(value.preprogrammed.preprogrammed_id == <u8>(207));
assert(value.effect_via_topic.topic_name == "ve782β");
assert(value.data_array.data.length == 1);
assert(value.data_array.data[0].r == <f32>(47.125));
assert(value.data_array.data[0].g == <f32>(-128.0));
assert(value.data_array.data[0].b == <f32>(206.0));
assert(value.data_array.data[0].a == <f32>(167.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_93():void{const input=bytes("070000007635633930ceb2");const codec=new r.Ros1Pal_device_msgsLedEffectViaTopicParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.topic_name == "v5c90β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_94():void{const input=bytes("00805b420000f8c000a0644300c07bc30000334300800243000098c100006242e87efda85df5a70801");const codec=new r.Ros1Pal_device_msgsLedFadeParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.first_color.r == <f32>(54.875));
assert(value.first_color.g == <f32>(-7.75));
assert(value.first_color.b == <f32>(228.625));
assert(value.first_color.a == <f32>(-251.75));
assert(value.second_color.r == <f32>(179.0));
assert(value.second_color.g == <f32>(130.5));
assert(value.second_color.b == <f32>(-19.0));
assert(value.second_color.a == <f32>(56.5));
assert(value.transition_duration.sec == <i32>(-1459781912));
assert(value.transition_duration.nsec == <i32>(145225053));
assert(value.reverse_fade == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_95():void{const input=bytes("00605c43004026c300a00843008036c2");const codec=new r.Ros1Pal_device_msgsLedFixedColorParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.color.r == <f32>(220.375));
assert(value.color.g == <f32>(-166.25));
assert(value.color.b == <f32>(136.625));
assert(value.color.a == <f32>(-45.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_96():void{const input=bytes("000078c1008062c3002037c3008028c200804fc30020754300a043c300e00fc30080d34200803f43");const codec=new r.Ros1Pal_device_msgsLedFlowParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.first_color.r == <f32>(-15.5));
assert(value.first_color.g == <f32>(-226.5));
assert(value.first_color.b == <f32>(-183.125));
assert(value.first_color.a == <f32>(-42.125));
assert(value.second_color.r == <f32>(-207.5));
assert(value.second_color.g == <f32>(245.125));
assert(value.second_color.b == <f32>(-195.625));
assert(value.second_color.a == <f32>(-143.875));
assert(value.percentage == <f32>(105.75));
assert(value.velocity == <f32>(191.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_97():void{const input=bytes("f3abf131");const codec=new r.Ros1Pal_device_msgsLedGroupCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ledMask == <u32>(837921779));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_98():void{const input=bytes("f9");const codec=new r.Ros1Pal_device_msgsLedPreProgrammedParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.preprogrammed_id == <u8>(249));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_99():void{const input=bytes("0000234200e076c30000674200404c4300801e420000c9c10000564100206f43002019c30000c0c0");const codec=new r.Ros1Pal_device_msgsLedProgressParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.first_color.r == <f32>(40.75));
assert(value.first_color.g == <f32>(-246.875));
assert(value.first_color.b == <f32>(57.75));
assert(value.first_color.a == <f32>(204.25));
assert(value.second_color.r == <f32>(39.625));
assert(value.second_color.g == <f32>(-25.125));
assert(value.second_color.b == <f32>(13.375));
assert(value.second_color.a == <f32>(239.125));
assert(value.percentage == <f32>(-153.125));
assert(value.led_offset == <f32>(-6.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_100():void{const input=bytes("4e9625f468cd7504");const codec=new r.Ros1Pal_device_msgsLedRainbowParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.transition_duration.sec == <i32>(-198863282));
assert(value.transition_duration.nsec == <i32>(74829160));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_101():void{const input=bytes("966ac61272b0da04");const codec=new r.Ros1Pal_device_msgsShutdownAndWakeUpRobotRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.shutdown_duration.sec == <i32>(314993302));
assert(value.shutdown_duration.nsec == <i32>(81440882));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_102():void{const input=bytes("");const codec=new r.Ros1Pal_device_msgsShutdownAndWakeUpRobotResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_103():void{const input=bytes("e236b96f00002ac10040b7c2008013c2002023c300008dc10000864200804dc300e07d43b272119bbb8dff1cef6cf770ecc22d28159cbd581edd88106e");const codec=new r.Ros1Pal_device_msgsTimedBlinkEffectRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.leds.ledMask == <u32>(1874409186));
assert(value.firstColor.r == <f32>(-10.625));
assert(value.firstColor.g == <f32>(-91.625));
assert(value.firstColor.b == <f32>(-36.875));
assert(value.firstColor.a == <f32>(-163.125));
assert(value.secondColor.r == <f32>(-17.625));
assert(value.secondColor.g == <f32>(67.0));
assert(value.secondColor.b == <f32>(-205.5));
assert(value.secondColor.a == <f32>(253.875));
assert(value.firstColorDuration.sec == <i32>(-1693355342));
assert(value.firstColorDuration.nsec == <i32>(486510011));
assert(value.secondColorDuration.sec == <i32>(1895263471));
assert(value.secondColorDuration.nsec == <i32>(674087660));
assert(value.effectDuration.sec == <i32>(1488821269));
assert(value.effectDuration.nsec == <i32>(277404958));
assert(value.priority == <u8>(110));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_104():void{const input=bytes("26621073");const codec=new r.Ros1Pal_device_msgsTimedBlinkEffectResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effectId == <u32>(1930453542));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_105():void{const input=bytes("fe29448600402cc3004041c300e0394300402fc3f37b261cb44fc31b6a");const codec=new r.Ros1Pal_device_msgsTimedColourEffectRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.leds.ledMask == <u32>(2252614142));
assert(value.color.r == <f32>(-172.25));
assert(value.color.g == <f32>(-193.25));
assert(value.color.b == <f32>(185.875));
assert(value.color.a == <f32>(-175.25));
assert(value.effectDuration.sec == <i32>(472284147));
assert(value.effectDuration.nsec == <i32>(465784756));
assert(value.priority == <u8>(106));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_106():void{const input=bytes("cba02b74");const codec=new r.Ros1Pal_device_msgsTimedColourEffectResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effectId == <u32>(1949016267));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_107():void{const input=bytes("32a45d6f00a00f43008079c3008027c2006043c30040c4c200605843008006c200603f43d2acbba13d8c110b01252258b80c10522ab1");const codec=new r.Ros1Pal_device_msgsTimedFadeEffectRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.leds.ledMask == <u32>(1868407858));
assert(value.firstColor.r == <f32>(143.625));
assert(value.firstColor.g == <f32>(-249.5));
assert(value.firstColor.b == <f32>(-41.875));
assert(value.firstColor.a == <f32>(-195.375));
assert(value.secondColor.r == <f32>(-98.125));
assert(value.secondColor.g == <f32>(216.375));
assert(value.secondColor.b == <f32>(-33.625));
assert(value.secondColor.a == <f32>(191.375));
assert(value.colorChangeDuration.sec == <i32>(-1581536046));
assert(value.colorChangeDuration.nsec == <i32>(185699389));
assert(value.reverseFade == true);
assert(value.effectDuration.sec == <i32>(-1202183643));
assert(value.effectDuration.nsec == <i32>(710021132));
assert(value.priority == <u8>(177));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_108():void{const input=bytes("e2471d65");const codec=new r.Ros1Pal_device_msgsTimedFadeEffectResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effectId == <u32>(1696417762));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_109():void{const input=bytes("39");const codec=new r.Ros1Pal_interaction_msgsASRActivationCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.action == <i8>(57));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_110():void{const input=bytes("0477070000007663303866ceb25ce6716051557bcd2b340722fbdba0a41801000000070000007633343765ceb2070000007664626565ceb200");const codec=new r.Ros1Pal_interaction_msgsASREventCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.event_id == <i8>(4));
assert(value.listen_state == <i8>(119));
assert(value.recognized_utterance.text == "vc08fβ");
assert(value.recognized_utterance.confidence == <i8>(92));
assert(value.recognized_utterance.start.sec == <u32>(1365275110));
assert(value.recognized_utterance.start.nsec == <u32>(734886741));
assert(value.recognized_utterance.end.sec == <u32>(4213311284));
assert(value.recognized_utterance.end.nsec == <u32>(413442267));
assert(value.recognized_utterance.tags.length == 1);
assert(value.recognized_utterance.tags[0].key == "v347eβ");
assert(value.recognized_utterance.tags[0].value == "vdbeeβ");
assert(value.active == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_111():void{const input=bytes("070000007630636336ceb279626d20a7703e850da8bd22780e214e3201000000070000007637663930ceb2070000007630383465ceb2");const codec=new r.Ros1Pal_interaction_msgsASRFileFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.recognised_utterance.text == "v0cc6β");
assert(value.recognised_utterance.confidence == <i8>(121));
assert(value.recognised_utterance.start.sec == <u32>(2803920226));
assert(value.recognised_utterance.start.nsec == <u32>(226836080));
assert(value.recognised_utterance.end.sec == <u32>(2015542696));
assert(value.recognised_utterance.end.nsec == <u32>(843981070));
assert(value.recognised_utterance.tags.length == 1);
assert(value.recognised_utterance.tags[0].key == "v7f90β");
assert(value.recognised_utterance.tags[0].value == "v084eβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_112():void{const input=bytes("070000007662383437ceb2070000007639323366ceb2070000007665333735ceb2");const codec=new r.Ros1Pal_interaction_msgsASRFileGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.file == "vb847β");
assert(value.lang_id == "v923fβ");
assert(value.grammar == "ve375β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_113():void{const input=bytes("070000007663646539ceb2070000007631613763ceb201000000070000007630336335ceb214a326680a1d1eb92e9450d9d464a68d1d01000000070000007634633766ceb2070000007632616237ceb2");const codec=new r.Ros1Pal_interaction_msgsASRFileResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.file == "vcde9β");
assert(value.msg == "v1a7cβ");
assert(value.recognised_utterances.length == 1);
assert(value.recognised_utterances[0].text == "v03c5β");
assert(value.recognised_utterances[0].confidence == <i8>(20));
assert(value.recognised_utterances[0].start.sec == <u32>(174597795));
assert(value.recognised_utterances[0].start.nsec == <u32>(783883805));
assert(value.recognised_utterances[0].end.sec == <u32>(3571011732));
assert(value.recognised_utterances[0].end.nsec == <u32>(495822436));
assert(value.recognised_utterances[0].tags.length == 1);
assert(value.recognised_utterances[0].tags[0].key == "v4c7fβ");
assert(value.recognised_utterances[0].tags[0].value == "v2ab7β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_114():void{const input=bytes("a8070000007634616639ceb2");const codec=new r.Ros1Pal_interaction_msgsASRLangModelMngmtCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.action == <i8>(-88));
assert(value.modelName == "v4af9β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_115():void{const input=bytes("070000007663623731ceb2");const codec=new r.Ros1Pal_interaction_msgsASRLanguageCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.language == "vcb71β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_116():void{const input=bytes("0100000030411f070000007663306430ceb2070000007637313039ceb2");const codec=new r.Ros1Pal_interaction_msgsASRServiceRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.requests.length == 1);
assert(value.request.requests[0] == <i8>(48));
assert(value.request.activation.action == <i8>(65));
assert(value.request.model.action == <i8>(31));
assert(value.request.model.modelName == "vc0d0β");
assert(value.request.lang.language == "v7109β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_117():void{const input=bytes("00070000007631613439ceb2070000007637386662ceb2070000007634383563ceb2070000007666623334ceb24069f44f138b0b99f5070000007632376536ceb2070000007630353935ceb2");const codec=new r.Ros1Pal_interaction_msgsASRServiceResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.status.active == false);
assert(value.response.status.language == "v1a49β");
assert(value.response.status.enabled_grammar == "v78fbβ");
assert(value.response.status.acoustic_env == "v485cβ");
assert(value.response.status.device.device_name == "vfb34β");
assert(value.response.status.device.sample_rate == <u32>(1341417792));
assert(value.response.status.device.sample_size == <i8>(19));
assert(value.response.status.device.format == <i8>(-117));
assert(value.response.status.device.buffer_size == <u16>(39179));
assert(value.response.status.device.number_of_channels == <i8>(-11));
assert(value.response.error_msg == "v27e6β");
assert(value.response.warn_msg == "v0595β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_118():void{const input=bytes("0100000062c62a070000007630373536ceb2070000007632373366ceb2");const codec=new r.Ros1Pal_interaction_msgsASRSrvRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.requests.length == 1);
assert(value.requests[0] == <i8>(98));
assert(value.activation.action == <i8>(-58));
assert(value.model.action == <i8>(42));
assert(value.model.modelName == "v0756β");
assert(value.lang.language == "v273fβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_119():void{const input=bytes("01070000007663613762ceb2070000007632323136ceb2070000007635623132ceb2070000007638663861ceb2cd904a3b9ecc8660a8070000007661333766ceb2070000007632323537ceb2");const codec=new r.Ros1Pal_interaction_msgsASRSrvResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.status.active == true);
assert(value.status.language == "vca7bβ");
assert(value.status.enabled_grammar == "v2216β");
assert(value.status.acoustic_env == "v5b12β");
assert(value.status.device.device_name == "v8f8aβ");
assert(value.status.device.sample_rate == <u32>(994742477));
assert(value.status.device.sample_size == <i8>(-98));
assert(value.status.device.format == <i8>(-52));
assert(value.status.device.buffer_size == <u16>(24710));
assert(value.status.device.number_of_channels == <i8>(-88));
assert(value.error_msg == "va37fβ");
assert(value.warn_msg == "v2257β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_120():void{const input=bytes("00070000007639363839ceb2070000007633366230ceb2070000007666343836ceb2070000007663633933ceb2f9975378aa28b26771");const codec=new r.Ros1Pal_interaction_msgsASRStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.active == false);
assert(value.language == "v9689β");
assert(value.enabled_grammar == "v36b0β");
assert(value.acoustic_env == "vf486β");
assert(value.device.device_name == "vcc93β");
assert(value.device.sample_rate == <u32>(2018744313));
assert(value.device.sample_size == <i8>(-86));
assert(value.device.format == <i8>(40));
assert(value.device.buffer_size == <u16>(26546));
assert(value.device.number_of_channels == <i8>(113));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_121():void{const input=bytes("070000007661303738ceb2c1b5b81492cf8a450c");const codec=new r.Ros1Pal_interaction_msgsAudioDeviceDescriptionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.device_name == "va078β");
assert(value.sample_rate == <u32>(347649473));
assert(value.sample_size == <i8>(-110));
assert(value.format == <i8>(-49));
assert(value.buffer_size == <u16>(17802));
assert(value.number_of_channels == <i8>(12));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_122():void{const input=bytes("");const codec=new r.Ros1Pal_interaction_msgsAudioPlayFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_123():void{const input=bytes("070000007630393232ceb271");const codec=new r.Ros1Pal_interaction_msgsAudioPlayGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.file == "v0922β");
assert(value.priority == <u8>(113));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_124():void{const input=bytes("");const codec=new r.Ros1Pal_interaction_msgsAudioPlayResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_125():void{const input=bytes("a49a0579339e1b84e188d419070000007630386530ceb200070000007637326465ceb2");const codec=new r.Ros1Pal_interaction_msgsAudioPlayerStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(2030410404));
assert(value.header.stamp.sec == <u32>(2216402483));
assert(value.header.stamp.nsec == <u32>(433359073));
assert(value.header.frame_id == "v08e0β");
assert(value.isPlaying == false);
assert(value.msg == "v72deβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_126():void{const input=bytes("00e029c300c047430080e942070000007662343535ceb2");const codec=new r.Ros1Pal_interaction_msgsDirectionOfArrivalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.doa == <f32>(-169.875));
assert(value.prob == <f32>(199.75));
assert(value.power == <f32>(116.75));
assert(value.metadoa == "vb455β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_127():void{const input=bytes("01");const codec=new r.Ros1Pal_interaction_msgsEnablingSoundLocalisationCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.enable == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_128():void{const input=bytes("070000007666636233ceb2070000007664336530ceb2070000007664616531ceb201000000070000007636333832ceb2070000007630616266ceb2070000007661356563ceb2070000007662646265ceb2070000007633326430ceb2");const codec=new r.Ros1Pal_interaction_msgsGetSpeechDurationRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text.section == "vfcb3β");
assert(value.text.key == "vd3e0β");
assert(value.text.lang_id == "vdae1β");
assert(value.text.arguments.length == 1);
assert(value.text.arguments[0].section == "v6382β");
assert(value.text.arguments[0].key == "v0abfβ");
assert(value.text.arguments[0].expanded == "va5ecβ");
assert(value.rawtext.text == "vbdbeβ");
assert(value.rawtext.lang_id == "v32d0β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_129():void{const input=bytes("0101000000070000007634376431ceb2010000008f185438c04ecf1f");const codec=new r.Ros1Pal_interaction_msgsGetSpeechDurationResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ok == true);
assert(value.words.length == 1);
assert(value.words[0] == "v47d1β");
assert(value.word_durations.length == 1);
assert(value.word_durations[0].sec == <i32>(945035407));
assert(value.word_durations[0].nsec == <i32>(533679808));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_130():void{const input=bytes("070000007636303662ceb2070000007632633766ceb2070000007630313365ceb2");const codec=new r.Ros1Pal_interaction_msgsI18nArgumentCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.section == "v606bβ");
assert(value.key == "v2c7fβ");
assert(value.expanded == "v013eβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_131():void{const input=bytes("070000007664393538ceb2070000007662666533ceb2070000007666663061ceb201000000070000007665363562ceb2070000007661363933ceb2070000007639623037ceb2");const codec=new r.Ros1Pal_interaction_msgsI18nTextCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.section == "vd958β");
assert(value.key == "vbfe3β");
assert(value.lang_id == "vff0aβ");
assert(value.arguments.length == 1);
assert(value.arguments[0].section == "ve65bβ");
assert(value.arguments[0].key == "va693β");
assert(value.arguments[0].expanded == "v9b07β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_132():void{const input=bytes("070000007663326463ceb201000000070000007664626537ceb2070000007663643631ceb2");const codec=new r.Ros1Pal_interaction_msgsInputCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.action == "vc2dcβ");
assert(value.args.length == 1);
assert(value.args[0].key == "vdbe7β");
assert(value.args[0].value == "vcd61β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_133():void{const input=bytes("070000007633643439ceb2070000007630343762ceb2");const codec=new r.Ros1Pal_interaction_msgsInputArgumentCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.key == "v3d49β");
assert(value.value == "v047bβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_134():void{const input=bytes("");const codec=new r.Ros1Pal_interaction_msgsSoundFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_135():void{const input=bytes("070000007639343131ceb2070000007630343630ceb2ca1d9d3eda0a7f1a");const codec=new r.Ros1Pal_interaction_msgsSoundGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "v9411β");
assert(value.lang_id == "v0460β");
assert(value.wait_before_speaking.sec == <i32>(1050484170));
assert(value.wait_before_speaking.nsec == <i32>(444533466));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_136():void{const input=bytes("00");const codec=new r.Ros1Pal_interaction_msgsSoundLocalisationServiceRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.enabling.enable == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_137():void{const input=bytes("");const codec=new r.Ros1Pal_interaction_msgsSoundLocalisationServiceResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_138():void{const input=bytes("070000007632633936ceb2");const codec=new r.Ros1Pal_interaction_msgsSoundResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "v2c96β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_139():void{const input=bytes("00070000007662376432ceb2");const codec=new r.Ros1Pal_interaction_msgsTTSstateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.isSpeaking == false);
assert(value.text == "vb7d2β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_140():void{const input=bytes("9f7782e26734874fe936070000007662393837ceb2070000007661636133ceb2070000007638613335ceb2070000007630326632ceb201000000070000007661613766ceb201000000070000007664643233ceb2");const codec=new r.Ros1Pal_interaction_msgsTtsFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.event_type == <u16>(30623));
assert(value.timestamp.sec == <u32>(879223426));
assert(value.timestamp.nsec == <u32>(921259911));
assert(value.text_said == "vb987β");
assert(value.next_word == "vaca3β");
assert(value.viseme_id == "v8a35β");
assert(value.marks.id == "v02f2β");
assert(value.marks.keys.length == 1);
assert(value.marks.keys[0] == "vaa7fβ");
assert(value.marks.value.length == 1);
assert(value.marks.value[0] == "vdd23β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_141():void{const input=bytes("070000007633383361ceb2070000007632356334ceb2070000007631653638ceb201000000070000007662656331ceb2070000007661643430ceb2070000007637643363ceb2070000007637393337ceb2070000007639343064ceb2070000007662316135ceb20000000000a05540");const codec=new r.Ros1Pal_interaction_msgsTtsGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text.section == "v383aβ");
assert(value.text.key == "v25c4β");
assert(value.text.lang_id == "v1e68β");
assert(value.text.arguments.length == 1);
assert(value.text.arguments[0].section == "vbec1β");
assert(value.text.arguments[0].key == "vad40β");
assert(value.text.arguments[0].expanded == "v7d3cβ");
assert(value.rawtext.text == "v7937β");
assert(value.rawtext.lang_id == "v940dβ");
assert(value.speakerName == "vb1a5β");
assert(value.wait_before_speaking == <f64>(86.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_142():void{const input=bytes("070000007664653138ceb201000000070000007633663531ceb201000000070000007665346235ceb2");const codec=new r.Ros1Pal_interaction_msgsTtsMarkCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.id == "vde18β");
assert(value.keys.length == 1);
assert(value.keys[0] == "v3f51β");
assert(value.value.length == 1);
assert(value.value[0] == "ve4b5β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_143():void{const input=bytes("070000007631633236ceb2070000007663376463ceb2");const codec=new r.Ros1Pal_interaction_msgsTtsResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "v1c26β");
assert(value.msg == "vc7dcβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_144():void{const input=bytes("070000007665343337ceb2070000007634353431ceb2");const codec=new r.Ros1Pal_interaction_msgsTtsTextCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "ve437β");
assert(value.lang_id == "v4541β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_145():void{const input=bytes("01");const codec=new r.Ros1Pal_interaction_msgsVoiceActivityCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.isSpeech == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_146():void{const input=bytes("070000007661363733ceb2070000007634616435ceb2");const codec=new r.Ros1Pal_interaction_msgsWebGuiEventCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "va673β");
assert(value.arg == "v4ad5β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_147():void{const input=bytes("070000007634633865ceb2070000007634313664ceb2");const codec=new r.Ros1Pal_interaction_msgsActiontagCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.key == "v4c8eβ");
assert(value.value == "v416dβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_148():void{const input=bytes("070000007636343631ceb20573e3f6a70cfe9023085590200ed2ef3101000000070000007632363439ceb2070000007664633232ceb2");const codec=new r.Ros1Pal_interaction_msgsAsrresultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "v6461β");
assert(value.confidence == <i8>(5));
assert(value.start.sec == <u32>(2817975155));
assert(value.start.nsec == <u32>(596704780));
assert(value.end.sec == <u32>(546329864));
assert(value.end.nsec == <u32>(837800462));
assert(value.tags.length == 1);
assert(value.tags[0].key == "v2649β");
assert(value.tags[0].value == "vdc22β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_149():void{const input=bytes("070000007633643962ceb2070000007630316332ceb2070000007665613639ceb2070000007632363062ceb201");const codec=new r.Ros1Pal_interaction_msgsAsrupdateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.language == "v3d9bβ");
assert(value.enable_grammar == "v01c2β");
assert(value.disable_grammar == "vea69β");
assert(value.acousticenv == "v260bβ");
assert(value.active == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_150():void{const input=bytes("010000000000000000d04540010000000000000000805240");const codec=new r.Ros1Pal_interaction_msgsAudiosignalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.channel0.length == 1);
assert(value.channel0[0] == <f64>(43.625));
assert(value.channel1.length == 1);
assert(value.channel1[0] == <f64>(74.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_151():void{const input=bytes("070000007665643135ceb2070000007666363231ceb2070000007666366632ceb2070000007633313739ceb201");const codec=new r.Ros1Pal_interaction_msgsRecognizerServiceRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.asrupdate.language == "ved15β");
assert(value.asrupdate.enable_grammar == "vf621β");
assert(value.asrupdate.disable_grammar == "vf6f2β");
assert(value.asrupdate.acousticenv == "v3179β");
assert(value.asrupdate.active == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_152():void{const input=bytes("");const codec=new r.Ros1Pal_interaction_msgsRecognizerServiceResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_153():void{const input=bytes("");const codec=new r.Ros1Pal_motion_model_msgsGetMotionMapRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_154():void{const input=bytes("75135b19eb308a82a93b912a070000007632666662ceb26780fea91222443900005ec10b9069f90b723bc900000000003c65c000000000001858c00000000000186d4000000000005865400000000000105d4000000000003058c00000000000106b400100000001000000008097c2008044c20000eb4200e04fc300808cc20000b1c2");const codec=new r.Ros1Pal_motion_model_msgsGetMotionMapResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.map.header.seq == <u32>(425399157));
assert(value.map.header.stamp.sec == <u32>(2190094571));
assert(value.map.header.stamp.nsec == <u32>(714161065));
assert(value.map.header.frame_id == "v2ffbβ");
assert(value.map.info.map_load_time.sec == <u32>(2852028519));
assert(value.map.info.map_load_time.nsec == <u32>(960766482));
assert(value.map.info.resolution == <f32>(-13.875));
assert(value.map.info.width == <u32>(4184444939));
assert(value.map.info.height == <u32>(3376116235));
assert(value.map.info.origin.position.x == <f64>(-169.875));
assert(value.map.info.origin.position.y == <f64>(-96.375));
assert(value.map.info.origin.position.z == <f64>(232.75));
assert(value.map.info.origin.orientation.x == <f64>(170.75));
assert(value.map.info.origin.orientation.y == <f64>(116.25));
assert(value.map.info.origin.orientation.z == <f64>(-96.75));
assert(value.map.info.origin.orientation.w == <f64>(216.5));
assert(value.map.data.length == 1);
assert(value.map.data[0].models.length == 1);
assert(value.map.data[0].models[0].heading_mean == <f32>(-75.75));
assert(value.map.data[0].models[0].heading_std_dev == <f32>(-49.125));
assert(value.map.data[0].models[0].linear_speed_mean == <f32>(117.5));
assert(value.map.data[0].models[0].linear_speed_std_dev == <f32>(-207.875));
assert(value.map.data[0].models[0].angular_speed_mean == <f32>(-70.25));
assert(value.map.data[0].models[0].angular_speed_std_dev == <f32>(-88.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_155():void{const input=bytes("00004043000066c200e00143004025430000cb42006061c3");const codec=new r.Ros1Pal_motion_model_msgsMotionModelCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.heading_mean == <f32>(192.0));
assert(value.heading_std_dev == <f32>(-57.5));
assert(value.linear_speed_mean == <f32>(129.875));
assert(value.linear_speed_std_dev == <f32>(165.25));
assert(value.angular_speed_mean == <f32>(101.5));
assert(value.angular_speed_std_dev == <f32>(-225.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_156():void{const input=bytes("010000000000d8410040dc42004053c30020344300c098c2000034c1");const codec=new r.Ros1Pal_motion_model_msgsMotionModelListCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.models.length == 1);
assert(value.models[0].heading_mean == <f32>(27.0));
assert(value.models[0].heading_std_dev == <f32>(110.125));
assert(value.models[0].linear_speed_mean == <f32>(-211.25));
assert(value.models[0].linear_speed_std_dev == <f32>(180.125));
assert(value.models[0].angular_speed_mean == <f32>(-76.375));
assert(value.models[0].angular_speed_std_dev == <f32>(-11.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_157():void{const input=bytes("aeaf02435cd26af864cbe810070000007661353433ceb2ab9b3c7392da163400c066c3d02c30a3eb2060d800000000006869400000000000a064400000000000b86b400000000000805cc00000000000c043c00000000000404dc000000000006068400100000001000000004069c30080acc200808c42002066430040db4200001142");const codec=new r.Ros1Pal_motion_model_msgsMotionModelMapCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1124249518));
assert(value.header.stamp.sec == <u32>(4167750236));
assert(value.header.stamp.nsec == <u32>(283691876));
assert(value.header.frame_id == "va543β");
assert(value.info.map_load_time.sec == <u32>(1933351851));
assert(value.info.map_load_time.nsec == <u32>(873912978));
assert(value.info.resolution == <f32>(-230.75));
assert(value.info.width == <u32>(2737843408));
assert(value.info.height == <u32>(3630178539));
assert(value.info.origin.position.x == <f64>(203.25));
assert(value.info.origin.position.y == <f64>(165.0));
assert(value.info.origin.position.z == <f64>(221.75));
assert(value.info.origin.orientation.x == <f64>(-114.0));
assert(value.info.origin.orientation.y == <f64>(-39.5));
assert(value.info.origin.orientation.z == <f64>(-58.5));
assert(value.info.origin.orientation.w == <f64>(195.0));
assert(value.data.length == 1);
assert(value.data[0].models.length == 1);
assert(value.data[0].models[0].heading_mean == <f32>(-233.25));
assert(value.data[0].models[0].heading_std_dev == <f32>(-86.25));
assert(value.data[0].models[0].linear_speed_mean == <f32>(70.25));
assert(value.data[0].models[0].linear_speed_std_dev == <f32>(230.125));
assert(value.data[0].models[0].angular_speed_mean == <f32>(109.625));
assert(value.data[0].models[0].angular_speed_std_dev == <f32>(36.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_158():void{const input=bytes("8ff4b36164415ee5693fcc28070000007632366330ceb2070000007633356332ceb2b9aa");const codec=new r.Ros1Pal_multirobot_msgsWifiServiceDetectionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1639183503));
assert(value.header.stamp.sec == <u32>(3848159588));
assert(value.header.stamp.nsec == <u32>(684474217));
assert(value.header.frame_id == "v26c0β");
assert(value.id == "v35c2β");
assert(value.signal == <i16>(-21831));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_159():void{const input=bytes("4c0ad5ec4ee386721d2ef803070000007632636262ceb2070000007665613136ceb20000000000686cc00000000000d4674000000000007055c000000000000049c00000000000f05f4000000000002468c00000000000b861c000000000000011c00000000000604740fa362eda5dbfb804");const codec=new r.Ros1Pal_simulation_msgsExternalWrenchCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3973384780));
assert(value.header.stamp.sec == <u32>(1921442638));
assert(value.header.stamp.nsec == <u32>(66596381));
assert(value.header.frame_id == "v2cbbβ");
assert(value.link_name == "vea16β");
assert(value.wrench.force.x == <f64>(-227.25));
assert(value.wrench.force.y == <f64>(190.625));
assert(value.wrench.force.z == <f64>(-85.75));
assert(value.wrench.torque.x == <f64>(-50.0));
assert(value.wrench.torque.y == <f64>(127.75));
assert(value.wrench.torque.z == <f64>(-193.125));
assert(value.application_point.x == <f64>(-141.75));
assert(value.application_point.y == <f64>(-4.25));
assert(value.application_point.z == <f64>(46.75));
assert(value.duration.sec == <i32>(-634505478));
assert(value.duration.nsec == <i32>(79216477));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_160():void{const input=bytes("070000007661346532ceb201");const codec=new r.Ros1Pal_tablet_msgsFunctionalityStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.errMessage.data == "va4e2β");
assert(value.errStatus.data == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_161():void{const input=bytes("01000000070000007661623865ceb200");const codec=new r.Ros1Pal_tablet_msgsRobotStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.messages.length == 1);
assert(value.messages[0].errMessage.data == "vab8eβ");
assert(value.messages[0].errStatus.data == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_162():void{const input=bytes("");const codec=new r.Ros1Pal_video_recording_msgsStartRecordingRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_163():void{const input=bytes("");const codec=new r.Ros1Pal_video_recording_msgsStartRecordingResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_164():void{const input=bytes("");const codec=new r.Ros1Pal_video_recording_msgsStopRecordingRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_165():void{const input=bytes("");const codec=new r.Ros1Pal_video_recording_msgsStopRecordingResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_166():void{const input=bytes("5e9e5d91d9a4da6cbd985815070000007637313065ceb20000000000e46dc00000000000fc6ec000000000009461c0");const codec=new r.Ros1Pal_vision_msgsDetectedObjectCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(2438831710));
assert(value.header.stamp.sec == <u32>(1826268377));
assert(value.header.stamp.nsec == <u32>(358127805));
assert(value.header.frame_id == "v710eβ");
assert(value.position3D.x == <f64>(-239.125));
assert(value.position3D.y == <f64>(-247.875));
assert(value.position3D.z == <f64>(-140.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_167():void{const input=bytes("46775b56985e10174123f814070000007630616466ceb2d9d929e060a01e7e00c068c3004071c3000047c20080ac420000000000b0534000000000009048c00000000000e037400000000000dc6740439cd05efae5e7c0d5acd7296cd5e0b78afc40f03385776e39ad997f80d4aee1ab1a15bc126322223d2a12cb845325553052bafe892b8d60050d2354bc7414ca");const codec=new r.Ros1Pal_vision_msgsDetectedPersonCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1448834886));
assert(value.header.stamp.sec == <u32>(386948760));
assert(value.header.stamp.nsec == <u32>(351806273));
assert(value.header.frame_id == "v0adfβ");
assert(value.objectId == <i64><u64>0x7e1ea060e029d9d9);
assert(value.boxColour.r == <f32>(-232.75));
assert(value.boxColour.g == <f32>(-241.25));
assert(value.boxColour.b == <f32>(-49.75));
assert(value.boxColour.a == <f32>(86.25));
assert(value.confidence == <f64>(78.75));
assert(value.position3D.x == <f64>(-49.125));
assert(value.position3D.y == <f64>(23.875));
assert(value.position3D.z == <f64>(190.875));
assert(value.box.x == <i64><u64>0xc0e7e5fa5ed09c43);
assert(value.box.y == <i64><u64>0xb7e0d56c29d7acd5);
assert(value.box.width == <i64><u64>0x6e778533f040fc8a);
assert(value.box.height == <i64><u64>0xe1aed4807f99ad39);
assert(value.faceBox.x == <i64><u64>0x22226312bc151aab);
assert(value.faceBox.y == <i64><u64>0x55255384cb122a3d);
assert(value.faceBox.width == <i64><u64>0x608d2b89feba5230);
assert(value.faceBox.height == <i64><u64>0xca1474bc54230d05);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_168():void{const input=bytes("000000000024624000000000003c634000000000008064c00f9a8c8eb6e3bb1099aa8bf920d3bc6731a7d2b588dee52b66556f23df2c58bd010000000040f942");const codec=new r.Ros1Pal_vision_msgsFaceDetectionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.position3D.x == <f64>(145.125));
assert(value.position3D.y == <f64>(153.875));
assert(value.position3D.z == <f64>(-164.0));
assert(value.imageBoundingBox.x == <i64><u64>0x10bbe3b68e8c9a0f);
assert(value.imageBoundingBox.y == <i64><u64>0x67bcd320f98baa99);
assert(value.imageBoundingBox.width == <i64><u64>0x2be5de88b5d2a731);
assert(value.imageBoundingBox.height == <i64><u64>0xbd582cdf236f5566);
assert(value.hog.length == 1);
assert(value.hog[0] == <f32>(124.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_169():void{const input=bytes("779fb48e683314cd30416a32070000007636653061ceb21765408f010000000000000000606f400000000000e86540000000000000124073e27903ca9b4e9de5d27e745cab49ea986b1b3421122caae687e48c5ffed3120100000000a01643");const codec=new r.Ros1Pal_vision_msgsFaceDetectionsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(2394201975));
assert(value.header.stamp.sec == <u32>(3440653160));
assert(value.header.stamp.nsec == <u32>(845824304));
assert(value.header.frame_id == "v6e0aβ");
assert(value.imgID == <u32>(2403362071));
assert(value.faces.length == 1);
assert(value.faces[0].position3D.x == <f64>(251.0));
assert(value.faces[0].position3D.y == <f64>(175.25));
assert(value.faces[0].position3D.z == <f64>(4.5));
assert(value.faces[0].imageBoundingBox.x == <i64><u64>0x9d4e9bca0379e273);
assert(value.faces[0].imageBoundingBox.y == <i64><u64>0xea49ab5c747ed2e5);
assert(value.faces[0].imageBoundingBox.width == <i64><u64>0xaa2c1221341b6b98);
assert(value.faces[0].imageBoundingBox.height == <i64><u64>0x12d3fe5f8ce487e6);
assert(value.faces[0].hog.length == 1);
assert(value.faces[0].hog[0] == <f32>(150.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_170():void{const input=bytes("8e01000000070000007639336262ceb201000000000054c2");const codec=new r.Ros1Pal_vision_msgsFaceRecognitionFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.order_id == <u8>(142));
assert(value.names.length == 1);
assert(value.names[0] == "v93bbβ");
assert(value.confidence.length == 1);
assert(value.confidence[0] == <f32>(-53.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_171():void{const input=bytes("bd070000007663643362ceb2");const codec=new r.Ros1Pal_vision_msgsFaceRecognitionGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.order_id == <u8>(189));
assert(value.order_argument == "vcd3bβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_172():void{const input=bytes("f901000000070000007639333032ceb20100000000000641");const codec=new r.Ros1Pal_vision_msgsFaceRecognitionResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.order_id == <u8>(249));
assert(value.names.length == 1);
assert(value.names[0] == "v9302β");
assert(value.confidence.length == 1);
assert(value.confidence[0] == <f32>(8.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_173():void{const input=bytes("");const codec=new r.Ros1Pal_vision_msgsFaceTrackingStartRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_174():void{const input=bytes("");const codec=new r.Ros1Pal_vision_msgsFaceTrackingStartResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_175():void{const input=bytes("");const codec=new r.Ros1Pal_vision_msgsFaceTrackingStopRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_176():void{const input=bytes("");const codec=new r.Ros1Pal_vision_msgsFaceTrackingStopResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_177():void{const input=bytes("4fe4a5c6f48b133de7433b15070000007636363231ceb201");const codec=new r.Ros1Pal_vision_msgsFollowMeResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3332760655));
assert(value.header.stamp.sec == <u32>(1024691188));
assert(value.header.stamp.nsec == <u32>(356205543));
assert(value.header.frame_id == "v6621β");
assert(value.targetDetected == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_178():void{const input=bytes("00400143");const codec=new r.Ros1Pal_vision_msgsFollowMeStartRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.maxSecondsTargetLost == <f32>(129.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_179():void{const input=bytes("");const codec=new r.Ros1Pal_vision_msgsFollowMeStartResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_180():void{const input=bytes("");const codec=new r.Ros1Pal_vision_msgsFollowMeStopRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_181():void{const input=bytes("");const codec=new r.Ros1Pal_vision_msgsFollowMeStopResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_182():void{const input=bytes("265cb62a06470bd01b868803070000007663363665ceb2070000007662346232ceb200000000006039c000000000000855c000000000007c6dc0");const codec=new r.Ros1Pal_vision_msgsGestureCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(716594214));
assert(value.header.stamp.sec == <u32>(3490400006));
assert(value.header.stamp.nsec == <u32>(59278875));
assert(value.header.frame_id == "vc66eβ");
assert(value.gestureId == "vb4b2β");
assert(value.position3D.x == <f64>(-25.375));
assert(value.position3D.y == <f64>(-84.125));
assert(value.position3D.z == <f64>(-235.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_183():void{const input=bytes("1a1efc378303d761f099b527070000007631316333ceb20080144300806a42");const codec=new r.Ros1Pal_vision_msgsHeadPanTiltCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(939269658));
assert(value.header.stamp.sec == <u32>(1641481091));
assert(value.header.stamp.nsec == <u32>(666212848));
assert(value.header.frame_id == "v11c3β");
assert(value.pan == <f32>(148.5));
assert(value.tilt == <f32>(58.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_184():void{const input=bytes("31691232881025aca75915451e2022db19bdb13fa0c486a1869e50163fe767880000000000b0414000000000003463400000000000e86fc001000000004075c300004443002032c300e0604300c093420100000000202443000071c30000a24200808bc20100000065885855010000008b27ed47");const codec=new r.Ros1Pal_vision_msgsHogDetectionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.imageBoundingBox.x == <i64><u64>0xac25108832126931);
assert(value.imageBoundingBox.y == <i64><u64>0xdb22201e451559a7);
assert(value.imageBoundingBox.width == <i64><u64>0xa186c4a03fb1bd19);
assert(value.imageBoundingBox.height == <i64><u64>0x8867e73f16509e86);
assert(value.direction.x == <f64>(35.375));
assert(value.direction.y == <f64>(153.625));
assert(value.direction.z == <f64>(-255.25));
assert(value.hog.length == 1);
assert(value.hog[0] == <f32>(-245.25));
assert(value.principalEigenVectorRGB.r == <f32>(196.0));
assert(value.principalEigenVectorRGB.g == <f32>(-178.125));
assert(value.principalEigenVectorRGB.b == <f32>(224.875));
assert(value.principalEigenVectorRGB.a == <f32>(73.875));
assert(value.rgbClusterCenters.length == 1);
assert(value.rgbClusterCenters[0].r == <f32>(164.125));
assert(value.rgbClusterCenters[0].g == <f32>(-241.0));
assert(value.rgbClusterCenters[0].b == <f32>(81.0));
assert(value.rgbClusterCenters[0].a == <f32>(-69.75));
assert(value.rgbDescriptor1.length == 1);
assert(value.rgbDescriptor1[0] == <u32>(1431865445));
assert(value.rgbDescriptor2.length == 1);
assert(value.rgbDescriptor2[0] == <u32>(1206724491));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_185():void{const input=bytes("f6e3af4ad91764350e260115070000007637633830ceb2a89da2c801000000e90f5c8950766b177f3f5bfec6466c60729342a2cbea753c78d5adccc1ac9a5200000000006065c00000000000846d400000000000c065c0010000000000714300e00ac30000b6c2002015c300c067430100000000c01843008085c200a03b43000075c301000000d0537910010000003efccc02");const codec=new r.Ros1Pal_vision_msgsHogDetectionsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1253041142));
assert(value.header.stamp.sec == <u32>(895752153));
assert(value.header.stamp.nsec == <u32>(352396814));
assert(value.header.frame_id == "v7c80β");
assert(value.imgID == <u32>(3366100392));
assert(value.persons.length == 1);
assert(value.persons[0].imageBoundingBox.x == <i64><u64>0x176b7650895c0fe9);
assert(value.persons[0].imageBoundingBox.y == <i64><u64>0x606c46c6fe5b3f7f);
assert(value.persons[0].imageBoundingBox.width == <i64><u64>0x3c75eacba2429372);
assert(value.persons[0].imageBoundingBox.height == <i64><u64>0x529aacc1ccadd578);
assert(value.persons[0].direction.x == <f64>(-171.0));
assert(value.persons[0].direction.y == <f64>(236.125));
assert(value.persons[0].direction.z == <f64>(-174.0));
assert(value.persons[0].hog.length == 1);
assert(value.persons[0].hog[0] == <f32>(241.0));
assert(value.persons[0].principalEigenVectorRGB.r == <f32>(-138.875));
assert(value.persons[0].principalEigenVectorRGB.g == <f32>(-91.0));
assert(value.persons[0].principalEigenVectorRGB.b == <f32>(-149.125));
assert(value.persons[0].principalEigenVectorRGB.a == <f32>(231.75));
assert(value.persons[0].rgbClusterCenters.length == 1);
assert(value.persons[0].rgbClusterCenters[0].r == <f32>(152.75));
assert(value.persons[0].rgbClusterCenters[0].g == <f32>(-66.75));
assert(value.persons[0].rgbClusterCenters[0].b == <f32>(187.625));
assert(value.persons[0].rgbClusterCenters[0].a == <f32>(-245.0));
assert(value.persons[0].rgbDescriptor1.length == 1);
assert(value.persons[0].rgbDescriptor1[0] == <u32>(276386768));
assert(value.persons[0].rgbDescriptor2.length == 1);
assert(value.persons[0].rgbDescriptor2[0] == <u32>(46988350));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_186():void{const input=bytes("7b99b44a2f3830a3085bae05070000007632313037ceb2010000000000000000e853c00000000000bc64c00000000000e03040");const codec=new r.Ros1Pal_vision_msgsLegDetectionsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1253349755));
assert(value.header.stamp.sec == <u32>(2737846319));
assert(value.header.stamp.nsec == <u32>(95312648));
assert(value.header.frame_id == "v2107β");
assert(value.position3D.length == 1);
assert(value.position3D[0].x == <f64>(-79.625));
assert(value.position3D[0].y == <f64>(-165.875));
assert(value.position3D[0].z == <f64>(16.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_187():void{const input=bytes("000068c300e07843");const codec=new r.Ros1Pal_vision_msgsLookToPixelRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f32>(-232.0));
assert(value.y == <f32>(248.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_188():void{const input=bytes("");const codec=new r.Ros1Pal_vision_msgsLookToPixelResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_189():void{const input=bytes("5dfb7189e4824617cbcb76fe72b241605ccbb5a2e5b2823cef2f7b1056564c8e");const codec=new r.Ros1Pal_vision_msgsRectangleCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <i64><u64>0x174682e48971fb5d);
assert(value.y == <i64><u64>0x6041b272fe76cbcb);
assert(value.width == <i64><u64>0x3c82b2e5a2b5cb5c);
assert(value.height == <i64><u64>0x8e4c5656107b2fef);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_190():void{const input=bytes("");const codec=new r.Ros1Pal_visual_localization_msgsVisualLocAddPlaceFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_191():void{const input=bytes("");const codec=new r.Ros1Pal_visual_localization_msgsVisualLocAddPlaceGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_192():void{const input=bytes("01");const codec=new r.Ros1Pal_visual_localization_msgsVisualLocAddPlaceResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.res == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_193():void{const input=bytes("");const codec=new r.Ros1Pal_visual_localization_msgsVisualLocRecognizeFeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_194():void{const input=bytes("");const codec=new r.Ros1Pal_visual_localization_msgsVisualLocRecognizeGoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_195():void{const input=bytes("000000000000606f4017de6a72d50926da3b28581c070000007665623865ceb20000000000986cc00000000000106fc00000000000b04f4000000000004869c000000000009066c000000000002042c000000000008c604000000000000869400000000000504540000000000000674000000000009c6c400000000000904d400000000000c04e400000000000446bc00000000000802ac0000000000098574000000000005868400000000000e06440000000000058564000000000002c6cc00000000000205fc00000000000402ec00000000000305fc000000000009c63c000000000000056c00000000000e4604000000000007062400000000000ec674000000000002863400000000000c06fc00000000000d463c0000000000070494000000000004033c00000000000fc6dc000000000001066c00000000000286dc00000000000ac66c00000000000506ec00000000000c0694000000000001051c000000000009047400000000000285fc00000000000786bc0");const codec=new r.Ros1Pal_visual_localization_msgsVisualLocRecognizeResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.success == false);
assert(value.confidence == <f64>(251.0));
assert(value.estimatedPose.header.seq == <u32>(1919606295));
assert(value.estimatedPose.header.stamp.sec == <u32>(3659925973));
assert(value.estimatedPose.header.stamp.nsec == <u32>(475539515));
assert(value.estimatedPose.header.frame_id == "veb8eβ");
assert(value.estimatedPose.pose.pose.position.x == <f64>(-228.75));
assert(value.estimatedPose.pose.pose.position.y == <f64>(-248.5));
assert(value.estimatedPose.pose.pose.position.z == <f64>(63.375));
assert(value.estimatedPose.pose.pose.orientation.x == <f64>(-202.25));
assert(value.estimatedPose.pose.pose.orientation.y == <f64>(-180.5));
assert(value.estimatedPose.pose.pose.orientation.z == <f64>(-36.25));
assert(value.estimatedPose.pose.pose.orientation.w == <f64>(132.375));
assert(value.estimatedPose.pose.covariance.length == 36);
assert(value.estimatedPose.pose.covariance[0] == <f64>(200.25));
assert(value.estimatedPose.pose.covariance[1] == <f64>(42.625));
assert(value.estimatedPose.pose.covariance[2] == <f64>(184.0));
assert(value.estimatedPose.pose.covariance[3] == <f64>(228.875));
assert(value.estimatedPose.pose.covariance[4] == <f64>(59.125));
assert(value.estimatedPose.pose.covariance[5] == <f64>(61.5));
assert(value.estimatedPose.pose.covariance[6] == <f64>(-218.125));
assert(value.estimatedPose.pose.covariance[7] == <f64>(-13.25));
assert(value.estimatedPose.pose.covariance[8] == <f64>(94.375));
assert(value.estimatedPose.pose.covariance[9] == <f64>(194.75));
assert(value.estimatedPose.pose.covariance[10] == <f64>(167.0));
assert(value.estimatedPose.pose.covariance[11] == <f64>(89.375));
assert(value.estimatedPose.pose.covariance[12] == <f64>(-225.375));
assert(value.estimatedPose.pose.covariance[13] == <f64>(-124.5));
assert(value.estimatedPose.pose.covariance[14] == <f64>(-15.125));
assert(value.estimatedPose.pose.covariance[15] == <f64>(-124.75));
assert(value.estimatedPose.pose.covariance[16] == <f64>(-156.875));
assert(value.estimatedPose.pose.covariance[17] == <f64>(-88.0));
assert(value.estimatedPose.pose.covariance[18] == <f64>(135.125));
assert(value.estimatedPose.pose.covariance[19] == <f64>(147.5));
assert(value.estimatedPose.pose.covariance[20] == <f64>(191.375));
assert(value.estimatedPose.pose.covariance[21] == <f64>(153.25));
assert(value.estimatedPose.pose.covariance[22] == <f64>(-254.0));
assert(value.estimatedPose.pose.covariance[23] == <f64>(-158.625));
assert(value.estimatedPose.pose.covariance[24] == <f64>(50.875));
assert(value.estimatedPose.pose.covariance[25] == <f64>(-19.25));
assert(value.estimatedPose.pose.covariance[26] == <f64>(-239.875));
assert(value.estimatedPose.pose.covariance[27] == <f64>(-176.5));
assert(value.estimatedPose.pose.covariance[28] == <f64>(-233.25));
assert(value.estimatedPose.pose.covariance[29] == <f64>(-181.375));
assert(value.estimatedPose.pose.covariance[30] == <f64>(-242.5));
assert(value.estimatedPose.pose.covariance[31] == <f64>(206.0));
assert(value.estimatedPose.pose.covariance[32] == <f64>(-68.25));
assert(value.estimatedPose.pose.covariance[33] == <f64>(47.125));
assert(value.estimatedPose.pose.covariance[34] == <f64>(-124.625));
assert(value.estimatedPose.pose.covariance[35] == <f64>(-219.75));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_196():void{const input=bytes("de");const codec=new r.Ros1Pal_walking_msgsContactStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ros_type == <u8>(222));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_197():void{const input=bytes("ac4d371b7390ec427fe47523070000007666333337ceb2010000000000000000946c400000000000dc6d400000000000706cc000000000000065c00000000000805ec00000000000804c4000000000007041c0470012d067c11a9453c67939070000007664336335ceb2010000000000000000b85a400000000000ec644000000000009046c00000000000b044c00000000000f8504000000000005856c000000000003059c0334b30461f442fd8d840502c070000007638663530ceb2010000000000000000b46fc00000000000b0594000000000004045c00000000000f85f4000000000002467400000000000e036400000000000e064c0010000004e7cb36e8263fc03");const codec=new r.Ros1Pal_walking_msgsMoveHipFeetRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.hip_poses.header.seq == <u32>(456609196));
assert(value.hip_poses.header.stamp.sec == <u32>(1122799731));
assert(value.hip_poses.header.stamp.nsec == <u32>(594928767));
assert(value.hip_poses.header.frame_id == "vf337β");
assert(value.hip_poses.poses.length == 1);
assert(value.hip_poses.poses[0].position.x == <f64>(228.625));
assert(value.hip_poses.poses[0].position.y == <f64>(238.875));
assert(value.hip_poses.poses[0].position.z == <f64>(-227.5));
assert(value.hip_poses.poses[0].orientation.x == <f64>(-168.0));
assert(value.hip_poses.poses[0].orientation.y == <f64>(-122.0));
assert(value.hip_poses.poses[0].orientation.z == <f64>(57.0));
assert(value.hip_poses.poses[0].orientation.w == <f64>(-34.875));
assert(value.left_foot_poses.header.seq == <u32>(3490840647));
assert(value.left_foot_poses.header.stamp.sec == <u32>(2484781415));
assert(value.left_foot_poses.header.stamp.nsec == <u32>(964281939));
assert(value.left_foot_poses.header.frame_id == "vd3c5β");
assert(value.left_foot_poses.poses.length == 1);
assert(value.left_foot_poses.poses[0].position.x == <f64>(106.875));
assert(value.left_foot_poses.poses[0].position.y == <f64>(167.375));
assert(value.left_foot_poses.poses[0].position.z == <f64>(-45.125));
assert(value.left_foot_poses.poses[0].orientation.x == <f64>(-41.375));
assert(value.left_foot_poses.poses[0].orientation.y == <f64>(67.875));
assert(value.left_foot_poses.poses[0].orientation.z == <f64>(-89.375));
assert(value.left_foot_poses.poses[0].orientation.w == <f64>(-100.75));
assert(value.right_foot_poses.header.seq == <u32>(1177570099));
assert(value.right_foot_poses.header.stamp.sec == <u32>(3626976287));
assert(value.right_foot_poses.header.stamp.nsec == <u32>(743456984));
assert(value.right_foot_poses.header.frame_id == "v8f50β");
assert(value.right_foot_poses.poses.length == 1);
assert(value.right_foot_poses.poses[0].position.x == <f64>(-253.625));
assert(value.right_foot_poses.poses[0].position.y == <f64>(102.75));
assert(value.right_foot_poses.poses[0].position.z == <f64>(-42.5));
assert(value.right_foot_poses.poses[0].orientation.x == <f64>(127.875));
assert(value.right_foot_poses.poses[0].orientation.y == <f64>(185.125));
assert(value.right_foot_poses.poses[0].orientation.z == <f64>(22.875));
assert(value.right_foot_poses.poses[0].orientation.w == <f64>(-167.0));
assert(value.times_after_previous.length == 1);
assert(value.times_after_previous[0].sec == <i32>(1857256526));
assert(value.times_after_previous[0].nsec == <i32>(66872194));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_198():void{const input=bytes("");const codec=new r.Ros1Pal_walking_msgsMoveHipFeetResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_199():void{const input=bytes("0000000000d85b400000000000a85440000000000030424000000000004462c00000000000085bc00000000000d468c00000000000e062400000000000d063c00000000000206cc00000000000a46f4000000000001468c00000000000a460c00000000000f468c00000000000e861c0e000000000008025400000000000205f400000000000486a400000000000146b400000000000dc6740000000000068524000000000008068c02b");const codec=new r.Ros1Pal_walking_msgsSMStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.left_foot_pose.position.x == <f64>(111.375));
assert(value.left_foot_pose.position.y == <f64>(82.625));
assert(value.left_foot_pose.position.z == <f64>(36.375));
assert(value.left_foot_pose.orientation.x == <f64>(-146.125));
assert(value.left_foot_pose.orientation.y == <f64>(-108.125));
assert(value.left_foot_pose.orientation.z == <f64>(-198.625));
assert(value.left_foot_pose.orientation.w == <f64>(151.0));
assert(value.right_foot_pose.position.x == <f64>(-158.5));
assert(value.right_foot_pose.position.y == <f64>(-225.0));
assert(value.right_foot_pose.position.z == <f64>(253.125));
assert(value.right_foot_pose.orientation.x == <f64>(-192.625));
assert(value.right_foot_pose.orientation.y == <f64>(-133.125));
assert(value.right_foot_pose.orientation.z == <f64>(-199.625));
assert(value.right_foot_pose.orientation.w == <f64>(-143.25));
assert(value.stance_foot_side == <u8>(224));
assert(value.future_swing_landing_pose.position.x == <f64>(10.75));
assert(value.future_swing_landing_pose.position.y == <f64>(124.5));
assert(value.future_swing_landing_pose.position.z == <f64>(210.25));
assert(value.future_swing_landing_pose.orientation.x == <f64>(216.625));
assert(value.future_swing_landing_pose.orientation.y == <f64>(190.875));
assert(value.future_swing_landing_pose.orientation.z == <f64>(73.625));
assert(value.future_swing_landing_pose.orientation.w == <f64>(-196.0));
assert(value.future_swing_side == <u8>(43));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_200():void{const input=bytes("0000000000003c60c0");const codec=new r.Ros1Pal_walking_msgsSitRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.down == false);
assert(value.weight_threshold == <f64>(-129.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_201():void{const input=bytes("");const codec=new r.Ros1Pal_walking_msgsSitResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_202():void{const input=bytes("00000000004464c00000000000b050c00000000000ac674000000000006467c0");const codec=new r.Ros1Pal_walking_msgsStepOverRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.step_length == <f64>(-162.125));
assert(value.step_time == <f64>(-66.75));
assert(value.step_height == <f64>(189.375));
assert(value.z_lipm == <f64>(-187.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_203():void{const input=bytes("070000007634353235ceb2");const codec=new r.Ros1Pal_walking_msgsStepOverResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.result == "v4525β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_204():void{const input=bytes("ed8a7e9a0000000000b05ec00000000000a06ec0");const codec=new r.Ros1Pal_walking_msgsWalkStepsRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.nsteps == <i32>(-1702982931));
assert(value.step_length == <f64>(-122.75));
assert(value.step_time == <f64>(-245.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_205():void{const input=bytes("");const codec=new r.Ros1Pal_walking_msgsWalkStepsResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_206():void{const input=bytes("758c8fe4674ef6c5dc2a2839070000007665313461ceb20000000000f867c00000000000d4644000000000008868c00000000000006840000000000018644000000000007468c00000000000bc66c000000000008045400000000000e8604000000000006463c000000000004048c0000000000080404000000000000059c0000000000020574000000000002866400000000000d060400000000000c03f400000000000a859400000000000c86bc000000000004062c000000000000462400000000000f46cc00000000000f46fc00000000000805040");const codec=new r.Ros1Pal_walking_msgsWalkingStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3834612853));
assert(value.header.stamp.sec == <u32>(3321253479));
assert(value.header.stamp.nsec == <u32>(958933724));
assert(value.header.frame_id == "ve14aβ");
assert(value.hip_reference_pose.position.x == <f64>(-191.75));
assert(value.hip_reference_pose.position.y == <f64>(166.625));
assert(value.hip_reference_pose.position.z == <f64>(-196.25));
assert(value.hip_reference_pose.orientation.x == <f64>(192.0));
assert(value.hip_reference_pose.orientation.y == <f64>(160.75));
assert(value.hip_reference_pose.orientation.z == <f64>(-195.625));
assert(value.hip_reference_pose.orientation.w == <f64>(-181.875));
assert(value.foot_reference_pose.length == 2);
assert(value.foot_reference_pose[0].position.x == <f64>(43.0));
assert(value.foot_reference_pose[0].position.y == <f64>(135.25));
assert(value.foot_reference_pose[0].position.z == <f64>(-155.125));
assert(value.foot_reference_pose[0].orientation.x == <f64>(-48.5));
assert(value.foot_reference_pose[0].orientation.y == <f64>(33.0));
assert(value.foot_reference_pose[0].orientation.z == <f64>(-100.0));
assert(value.foot_reference_pose[0].orientation.w == <f64>(92.5));
assert(value.foot_reference_pose[1].position.x == <f64>(177.25));
assert(value.foot_reference_pose[1].position.y == <f64>(134.5));
assert(value.foot_reference_pose[1].position.z == <f64>(31.75));
assert(value.foot_reference_pose[1].orientation.x == <f64>(102.625));
assert(value.foot_reference_pose[1].orientation.y == <f64>(-222.25));
assert(value.foot_reference_pose[1].orientation.z == <f64>(-146.0));
assert(value.foot_reference_pose[1].orientation.w == <f64>(144.125));
assert(value.zmp_reference.x == <f64>(-231.625));
assert(value.zmp_reference.y == <f64>(-255.625));
assert(value.zmp_reference.z == <f64>(66.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_207():void{const input=bytes("8f2dff7a58f70aa05867f617070000007661643261ceb20000000000986cc000000000007066c00000000000106840fef3ecae7323c72b1cb1c65982");const codec=new r.Ros1Pal_walking_msgsWalkingStepCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(2063543695));
assert(value.header.stamp.sec == <u32>(2685073240));
assert(value.header.stamp.nsec == <u32>(402024280));
assert(value.header.frame_id == "vad2aβ");
assert(value.step_target.pose.x == <f64>(-228.75));
assert(value.step_target.pose.y == <f64>(-179.5));
assert(value.step_target.pose.theta == <f64>(192.5));
assert(value.step_target.leg == <u8>(254));
assert(value.step_duration.data.sec == <i32>(1940843763));
assert(value.step_duration.data.nsec == <i32>(472631075));
assert(value.step_list_size == <i32>(-2108045647));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_208():void{const input=bytes("");const codec=new r.Ros1Pal_wifi_localization_msgsGetWifiMapRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_209():void{const input=bytes("3ab5016c19a769750e55f016070000007661653362ceb2f648c3e6085e330b000049c24436338cac742d2f000000000000e03f0000000000604dc000000000008052c00000000000f055c00000000000e46d40000000000000e8bf00000000001463403984c1aa0100000001000000070000007636393462ceb200c0fec20000e7c201719c568ef034265449fc30cdadd70e");const codec=new r.Ros1Pal_wifi_localization_msgsGetWifiMapResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.map.header.seq == <u32>(1812051258));
assert(value.map.header.stamp.sec == <u32>(1969858329));
assert(value.map.header.stamp.nsec == <u32>(384849166));
assert(value.map.header.frame_id == "vae3bβ");
assert(value.map.info.map_load_time.sec == <u32>(3871557878));
assert(value.map.info.map_load_time.nsec == <u32>(187915784));
assert(value.map.info.resolution == <f32>(-50.25));
assert(value.map.info.width == <u32>(2352166468));
assert(value.map.info.height == <u32>(791508140));
assert(value.map.info.origin.position.x == <f64>(0.5));
assert(value.map.info.origin.position.y == <f64>(-58.75));
assert(value.map.info.origin.position.z == <f64>(-74.0));
assert(value.map.info.origin.orientation.x == <f64>(-87.75));
assert(value.map.info.origin.orientation.y == <f64>(239.125));
assert(value.map.info.origin.orientation.z == <f64>(-0.75));
assert(value.map.info.origin.orientation.w == <f64>(152.625));
assert(value.map.sectors == <u32>(2864809017));
assert(value.map.data.length == 1);
assert(value.map.data[0].networks.length == 1);
assert(value.map.data[0].networks[0].id.data == "v694bβ");
assert(value.map.data[0].networks[0].mean == <f32>(-127.375));
assert(value.map.data[0].networks[0].std_dev == <f32>(-115.5));
assert(value.map.data[0].start_time.sec == <u32>(1453093121));
assert(value.map.data[0].start_time.nsec == <u32>(641003662));
assert(value.map.data[0].end_time.sec == <u32>(821840212));
assert(value.map.data[0].end_time.nsec == <u32>(249015757));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_210():void{const input=bytes("070000007664636330ceb2008039c3002006c3");const codec=new r.Ros1Pal_wifi_localization_msgsWifiSignalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.id.data == "vdcc0β");
assert(value.mean == <f32>(-185.5));
assert(value.std_dev == <f32>(-134.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_211():void{const input=bytes("01000000070000007661303930ceb2006064c3008024c25871c7f4dea2491870bb3342ee0cac07");const codec=new r.Ros1Pal_wifi_localization_msgsWifiSignalListCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.networks.length == 1);
assert(value.networks[0].id.data == "va090β");
assert(value.networks[0].mean == <f32>(-228.375));
assert(value.networks[0].std_dev == <f32>(-41.125));
assert(value.start_time.sec == <u32>(4106711384));
assert(value.start_time.nsec == <u32>(407479006));
assert(value.end_time.sec == <u32>(1110686576));
assert(value.end_time.nsec == <u32>(128716014));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_212():void{const input=bytes("cdf4c44a27db3e65d9d9b913070000007632333364ceb28eb76daef98dc52d00a01c43b377f6aa52d6d20c0000000000a061400000000000a069400000000000002c400000000000606d4000000000008c69c00000000000406240000000000000164029b183270100000001000000070000007639356161ceb20060134300803d42798e321e7f752c11058cae956ade8e23");const codec=new r.Ros1Pal_wifi_localization_msgsWifiSignalMapCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(1254421709));
assert(value.header.stamp.sec == <u32>(1698618151));
assert(value.header.stamp.nsec == <u32>(330947033));
assert(value.header.frame_id == "v233dβ");
assert(value.info.map_load_time.sec == <u32>(2926425998));
assert(value.info.map_load_time.nsec == <u32>(767921657));
assert(value.info.resolution == <f32>(156.625));
assert(value.info.width == <u32>(2868279219));
assert(value.info.height == <u32>(215144018));
assert(value.info.origin.position.x == <f64>(141.0));
assert(value.info.origin.position.y == <f64>(205.0));
assert(value.info.origin.position.z == <f64>(14.0));
assert(value.info.origin.orientation.x == <f64>(235.0));
assert(value.info.origin.orientation.y == <f64>(-204.375));
assert(value.info.origin.orientation.z == <f64>(146.0));
assert(value.info.origin.orientation.w == <f64>(5.5));
assert(value.sectors == <u32>(662941993));
assert(value.data.length == 1);
assert(value.data[0].networks.length == 1);
assert(value.data[0].networks[0].id.data == "v95aaβ");
assert(value.data[0].networks[0].mean == <f32>(147.375));
assert(value.data[0].networks[0].std_dev == <f32>(47.375));
assert(value.data[0].start_time.sec == <u32>(506629753));
assert(value.data[0].start_time.nsec == <u32>(288126335));
assert(value.data[0].end_time.sec == <u32>(2511244293));
assert(value.data[0].end_time.nsec == <u32>(596565610));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_213():void{const input=bytes("f07a6dc9f1321c00d1f68120070000007662313064ceb2070000007633613638ceb2010000009a");const codec=new r.Ros1Sensor_msgsCompressedImageCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3379395312));
assert(value.header.stamp.sec == <u32>(1848049));
assert(value.header.stamp.nsec == <u32>(545388241));
assert(value.header.frame_id == "vb10dβ");
assert(value.format == "v3a68β");
assert(value.data.length == 1);
assert(value.data[0] == <u8>(154));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_214():void{const input=bytes("0d3bc4cec509aff95fb1cc01070000007662626539ceb293698f68aa76e19c070000007635316232ceb21aa621f0620100000016");const codec=new r.Ros1Sensor_msgsImageCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.seq == <u32>(3468966669));
assert(value.header.stamp.sec == <u32>(4188998085));
assert(value.header.stamp.nsec == <u32>(30191967));
assert(value.header.frame_id == "vbbe9β");
assert(value.height == <u32>(1754229139));
assert(value.width == <u32>(2632021674));
assert(value.encoding == "v51b2β");
assert(value.is_bigendian == <u8>(26));
assert(value.step == <u32>(1659904422));
assert(value.data.length == 1);
assert(value.data[0] == <u8>(22));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_215():void{const input=bytes("f8804e436680e48f01fb39c2c4bb3f2000");const codec=new r.Ros1Sensor_msgsRegionOfInterestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x_offset == <u32>(1129218296));
assert(value.y_offset == <u32>(2414116966));
assert(value.height == <u32>(3258579713));
assert(value.width == <u32>(541047748));
assert(value.do_rectify == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_216():void{const input=bytes("01");const codec=new r.Ros1Std_msgsBoolCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_217():void{const input=bytes("002068c3008008c200809cc2000028c3");const codec=new r.Ros1Std_msgsColorRGBACodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.r == <f32>(-232.125));
assert(value.g == <f32>(-34.125));
assert(value.b == <f32>(-78.25));
assert(value.a == <f32>(-168.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_218():void{const input=bytes("4796f792cc0b1102");const codec=new r.Ros1Std_msgsDurationCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data.sec == <i32>(-1829267897));
assert(value.data.nsec == <i32>(34671564));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_219():void{const input=bytes("a949162e20488fdde929b81e070000007632663062ceb2");const codec=new r.Ros1Std_msgsHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.seq == <u32>(773212585));
assert(value.stamp.sec == <u32>(3717154848));
assert(value.stamp.nsec == <u32>(515385833));
assert(value.frame_id == "v2f0bβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_220():void{const input=bytes("070000007631643932ceb2");const codec=new r.Ros1Std_msgsStringCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data == "v1d92β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_221():void{const input=bytes("c8280b76c3d5bc08");const codec=new r.Ros1TimeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <u32>(1980442824));
assert(value.nsec == <u32>(146593219));
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
test_120();
test_121();
test_122();
test_123();
test_124();
test_125();
test_126();
test_127();
test_128();
test_129();
test_130();
test_131();
test_132();
test_133();
test_134();
test_135();
test_136();
test_137();
test_138();
test_139();
test_140();
test_141();
test_142();
test_143();
test_144();
test_145();
test_146();
test_147();
test_148();
test_149();
test_150();
test_151();
test_152();
test_153();
test_154();
test_155();
test_156();
test_157();
test_158();
test_159();
test_160();
test_161();
test_162();
test_163();
test_164();
test_165();
test_166();
test_167();
test_168();
test_169();
test_170();
test_171();
test_172();
test_173();
test_174();
test_175();
test_176();
test_177();
test_178();
test_179();
test_180();
test_181();
test_182();
test_183();
test_184();
test_185();
test_186();
test_187();
test_188();
test_189();
test_190();
test_191();
test_192();
test_193();
test_194();
test_195();
test_196();
test_197();
test_198();
test_199();
test_200();
test_201();
test_202();
test_203();
test_204();
test_205();
test_206();
test_207();
test_208();
test_209();
test_210();
test_211();
test_212();
test_213();
test_214();
test_215();
test_216();
test_217();
test_218();
test_219();
test_220();
test_221();
}
