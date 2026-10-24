/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../../../sdk/origin/custom-types/pal-ros2-humble-devel-complete/bounded_records";
import {abortGuest} from "../../../../../../../../../sdk/origin/assemblyscript/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("8b8195af42ba7f2d");const codec=new r.Ros2Builtin_interfacesMsgDurationCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-1349156469));
assert(value.nanosec == <u32>(763345474));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("072b89b550057009");const codec=new r.Ros2Builtin_interfacesMsgTimeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-1249301753));
assert(value.nanosec == <u32>(158336336));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("");const codec=new r.Ros2Pal_device_msgsActionDoTimedLedEffect_FeedbackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("01000000af2de815f9008057c300a0314300a07ac300003d421fce0279706d2b350040d742008064c200408ac200c0fdc200c0e642000022c100c06ec30080f8c25e7b66172ff857070100409d4200207fc3000048420000264300c045c30040ed420000264300000142910ae54548f725193eefa9cd4b07a01e0040604300c0dd4200a03c4300207e4300008cc2000085c20000e0c200803fc300c01c4300403d43000072c100401d4300003ec30000414200405f4300000ec10080704300e023c300000fc300c061c3de070000007639626435ceb2010000000080844200c0ce4200003441000007424eff4e84b8dc870eb1");const codec=new r.Ros2Pal_device_msgsActionDoTimedLedEffect_GoalCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.devices.length == 1);
assert(value.devices[0] == <u32>(367537583));
assert(value.params.effect_type == <u8>(249));
assert(value.params.fixed_color.color.r == <f32>(-215.5));
assert(value.params.fixed_color.color.g == <f32>(177.625));
assert(value.params.fixed_color.color.b == <f32>(-250.625));
assert(value.params.fixed_color.color.a == <f32>(47.25));
assert(value.params.rainbow.transition_duration.sec == <i32>(2030226975));
assert(value.params.rainbow.transition_duration.nanosec == <u32>(892038512));
assert(value.params.fade.first_color.r == <f32>(107.625));
assert(value.params.fade.first_color.g == <f32>(-57.125));
assert(value.params.fade.first_color.b == <f32>(-69.125));
assert(value.params.fade.first_color.a == <f32>(-126.875));
assert(value.params.fade.second_color.r == <f32>(115.375));
assert(value.params.fade.second_color.g == <f32>(-10.125));
assert(value.params.fade.second_color.b == <f32>(-238.75));
assert(value.params.fade.second_color.a == <f32>(-124.25));
assert(value.params.fade.transition_duration.sec == <i32>(392592222));
assert(value.params.fade.transition_duration.nanosec == <u32>(123205679));
assert(value.params.fade.reverse_fade == true);
assert(value.params.blink.first_color.r == <f32>(78.625));
assert(value.params.blink.first_color.g == <f32>(-255.125));
assert(value.params.blink.first_color.b == <f32>(50.0));
assert(value.params.blink.first_color.a == <f32>(166.0));
assert(value.params.blink.second_color.r == <f32>(-197.75));
assert(value.params.blink.second_color.g == <f32>(118.625));
assert(value.params.blink.second_color.b == <f32>(166.0));
assert(value.params.blink.second_color.a == <f32>(32.25));
assert(value.params.blink.first_color_duration.sec == <i32>(1172638353));
assert(value.params.blink.first_color_duration.nanosec == <u32>(421918536));
assert(value.params.blink.second_color_duration.sec == <i32>(-844501186));
assert(value.params.blink.second_color_duration.nanosec == <u32>(513804107));
assert(value.params.progress.first_color.r == <f32>(224.25));
assert(value.params.progress.first_color.g == <f32>(110.875));
assert(value.params.progress.first_color.b == <f32>(188.625));
assert(value.params.progress.first_color.a == <f32>(254.125));
assert(value.params.progress.second_color.r == <f32>(-70.0));
assert(value.params.progress.second_color.g == <f32>(-66.5));
assert(value.params.progress.second_color.b == <f32>(-112.0));
assert(value.params.progress.second_color.a == <f32>(-191.5));
assert(value.params.progress.percentage == <f32>(156.75));
assert(value.params.progress.led_offset == <f32>(189.25));
assert(value.params.flow.first_color.r == <f32>(-15.125));
assert(value.params.flow.first_color.g == <f32>(157.25));
assert(value.params.flow.first_color.b == <f32>(-190.0));
assert(value.params.flow.first_color.a == <f32>(48.25));
assert(value.params.flow.second_color.r == <f32>(223.25));
assert(value.params.flow.second_color.g == <f32>(-8.875));
assert(value.params.flow.second_color.b == <f32>(240.5));
assert(value.params.flow.second_color.a == <f32>(-163.875));
assert(value.params.flow.percentage == <f32>(-143.0));
assert(value.params.flow.velocity == <f32>(-225.75));
assert(value.params.preprogrammed.preprogrammed_id == <u8>(222));
assert(value.params.effect_via_topic.topic_name == "v9bd5β");
assert(value.params.data_array.data.length == 1);
assert(value.params.data_array.data[0].r == <f32>(66.25));
assert(value.params.data_array.data[0].g == <f32>(103.375));
assert(value.params.data_array.data[0].b == <f32>(11.25));
assert(value.params.data_array.data[0].a == <f32>(33.75));
assert(value.effect_duration.sec == <i32>(-2075197618));
assert(value.effect_duration.nanosec == <u32>(243784888));
assert(value.priority == <u8>(177));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("");const codec=new r.Ros2Pal_device_msgsActionDoTimedLedEffect_ResultCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("0d00001543");const codec=new r.Ros2Pal_device_msgsMsgBatteryStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.charge_state == <i8>(13));
assert(value.battery_percentage == <f32>(149.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("0ea7b8eae9cbd33a070000007663306466ceb201");const codec=new r.Ros2Pal_device_msgsMsgBumperCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-356997362));
assert(value.header.stamp.nanosec == <u32>(986958825));
assert(value.header.frame_id == "vc0dfβ");
assert(value.is_pressed == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("00c0b24200806c4300206243008018c20080f24200c0d7c20080214300007cc1bb8255b4184e58276096e316d6f44410");const codec=new r.Ros2Pal_device_msgsMsgLedBlinkParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.first_color.r == <f32>(89.375));
assert(value.first_color.g == <f32>(236.5));
assert(value.first_color.b == <f32>(226.125));
assert(value.first_color.a == <f32>(-38.125));
assert(value.second_color.r == <f32>(121.25));
assert(value.second_color.g == <f32>(-107.875));
assert(value.second_color.b == <f32>(161.5));
assert(value.second_color.a == <f32>(-15.75));
assert(value.first_color_duration.sec == <i32>(-1269464389));
assert(value.first_color_duration.nanosec == <u32>(660098584));
assert(value.second_color_duration.sec == <i32>(384013920));
assert(value.second_color_duration.nanosec == <u32>(272954582));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("010000000080b0c2004035c300005ac2004078c3");const codec=new r.Ros2Pal_device_msgsMsgLedDataArrayParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data.length == 1);
assert(value.data[0].r == <f32>(-88.25));
assert(value.data[0].g == <f32>(-181.25));
assert(value.data[0].b == <f32>(-54.5));
assert(value.data[0].a == <f32>(-248.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("cf0000e14100000ac100e0354300008ac22820961a3e2f3d3a0000f841008070c3000041430000644200a01fc3000034c20000fdc200a075432e8b9172ce5f83130100e055c30040d7420080df420000334200001f430040e04200e06bc30000ce428ce467d7015dab0f09013dae05458e23008089c200c092c200e03ec300c005430000e84200a012430000e642000080420080b04200005ac200c058c300c014c300804ac200c0ca420000abc10000e1c1004035c30020404300805a4200402843c3070000007664636663ceb20100000000e0634300005ec100e05fc300005ec1");const codec=new r.Ros2Pal_device_msgsMsgLedEffectParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effect_type == <u8>(207));
assert(value.fixed_color.color.r == <f32>(28.125));
assert(value.fixed_color.color.g == <f32>(-8.625));
assert(value.fixed_color.color.b == <f32>(181.875));
assert(value.fixed_color.color.a == <f32>(-69.0));
assert(value.rainbow.transition_duration.sec == <i32>(446046248));
assert(value.rainbow.transition_duration.nanosec == <u32>(977088318));
assert(value.fade.first_color.r == <f32>(31.0));
assert(value.fade.first_color.g == <f32>(-240.5));
assert(value.fade.first_color.b == <f32>(193.0));
assert(value.fade.first_color.a == <f32>(57.0));
assert(value.fade.second_color.r == <f32>(-159.625));
assert(value.fade.second_color.g == <f32>(-45.0));
assert(value.fade.second_color.b == <f32>(-126.5));
assert(value.fade.second_color.a == <f32>(245.625));
assert(value.fade.transition_duration.sec == <i32>(1922140974));
assert(value.fade.transition_duration.nanosec == <u32>(327376846));
assert(value.fade.reverse_fade == true);
assert(value.blink.first_color.r == <f32>(-213.875));
assert(value.blink.first_color.g == <f32>(107.625));
assert(value.blink.first_color.b == <f32>(111.75));
assert(value.blink.first_color.a == <f32>(44.75));
assert(value.blink.second_color.r == <f32>(159.0));
assert(value.blink.second_color.g == <f32>(112.125));
assert(value.blink.second_color.b == <f32>(-235.875));
assert(value.blink.second_color.a == <f32>(103.0));
assert(value.blink.first_color_duration.sec == <i32>(-681057140));
assert(value.blink.first_color_duration.nanosec == <u32>(262888705));
assert(value.blink.second_color_duration.sec == <i32>(-1371733751));
assert(value.blink.second_color_duration.nanosec == <u32>(596526341));
assert(value.progress.first_color.r == <f32>(-68.75));
assert(value.progress.first_color.g == <f32>(-73.375));
assert(value.progress.first_color.b == <f32>(-190.875));
assert(value.progress.first_color.a == <f32>(133.75));
assert(value.progress.second_color.r == <f32>(116.0));
assert(value.progress.second_color.g == <f32>(146.625));
assert(value.progress.second_color.b == <f32>(115.0));
assert(value.progress.second_color.a == <f32>(64.0));
assert(value.progress.percentage == <f32>(88.25));
assert(value.progress.led_offset == <f32>(-54.5));
assert(value.flow.first_color.r == <f32>(-216.75));
assert(value.flow.first_color.g == <f32>(-148.75));
assert(value.flow.first_color.b == <f32>(-50.625));
assert(value.flow.first_color.a == <f32>(101.375));
assert(value.flow.second_color.r == <f32>(-21.375));
assert(value.flow.second_color.g == <f32>(-28.125));
assert(value.flow.second_color.b == <f32>(-181.25));
assert(value.flow.second_color.a == <f32>(192.125));
assert(value.flow.percentage == <f32>(54.625));
assert(value.flow.velocity == <f32>(168.25));
assert(value.preprogrammed.preprogrammed_id == <u8>(195));
assert(value.effect_via_topic.topic_name == "vdcfcβ");
assert(value.data_array.data.length == 1);
assert(value.data_array.data[0].r == <f32>(227.875));
assert(value.data_array.data[0].g == <f32>(-13.875));
assert(value.data_array.data[0].b == <f32>(-223.875));
assert(value.data_array.data[0].a == <f32>(-13.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("070000007664356264ceb2");const codec=new r.Ros2Pal_device_msgsMsgLedEffectViaTopicParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.topic_name == "vd5bdβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("004047c300c041c30000acc100404a430080bcc20040e8c200c006c300e01a4390890716d40e241a01");const codec=new r.Ros2Pal_device_msgsMsgLedFadeParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.first_color.r == <f32>(-199.25));
assert(value.first_color.g == <f32>(-193.75));
assert(value.first_color.b == <f32>(-21.5));
assert(value.first_color.a == <f32>(202.25));
assert(value.second_color.r == <f32>(-94.25));
assert(value.second_color.g == <f32>(-116.125));
assert(value.second_color.b == <f32>(-134.75));
assert(value.second_color.a == <f32>(154.875));
assert(value.transition_duration.sec == <i32>(369592720));
assert(value.transition_duration.nanosec == <u32>(438570708));
assert(value.reverse_fade == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("0000624300404b43008084420000d9c2");const codec=new r.Ros2Pal_device_msgsMsgLedFixedColorParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.color.r == <f32>(226.0));
assert(value.color.g == <f32>(203.25));
assert(value.color.b == <f32>(66.25));
assert(value.color.a == <f32>(-108.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("00008fc100002043008065c30000b0c200600fc300607e4300603c43008001c200002cc30040f3c2");const codec=new r.Ros2Pal_device_msgsMsgLedFlowParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.first_color.r == <f32>(-17.875));
assert(value.first_color.g == <f32>(160.0));
assert(value.first_color.b == <f32>(-229.5));
assert(value.first_color.a == <f32>(-88.0));
assert(value.second_color.r == <f32>(-143.375));
assert(value.second_color.g == <f32>(254.375));
assert(value.second_color.b == <f32>(188.375));
assert(value.second_color.a == <f32>(-32.375));
assert(value.percentage == <f32>(-172.0));
assert(value.velocity == <f32>(-121.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("2b00cefa");const codec=new r.Ros2Pal_device_msgsMsgLedGroupCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.led_mask == <u32>(4207804459));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("42");const codec=new r.Ros2Pal_device_msgsMsgLedPreProgrammedParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.preprogrammed_id == <u8>(66));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("002007430000a0c20000e541006052c300406043006075430080f8c200c06dc300602dc300a04c43");const codec=new r.Ros2Pal_device_msgsMsgLedProgressParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.first_color.r == <f32>(135.125));
assert(value.first_color.g == <f32>(-80.0));
assert(value.first_color.b == <f32>(28.625));
assert(value.first_color.a == <f32>(-210.375));
assert(value.second_color.r == <f32>(224.25));
assert(value.second_color.g == <f32>(245.375));
assert(value.second_color.b == <f32>(-124.25));
assert(value.second_color.a == <f32>(-237.75));
assert(value.percentage == <f32>(-173.375));
assert(value.led_offset == <f32>(204.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("60c352a8ff27260a");const codec=new r.Ros2Pal_device_msgsMsgLedRainbowParamsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.transition_duration.sec == <i32>(-1470971040));
assert(value.transition_duration.nanosec == <u32>(170272767));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("43ae8e6c");const codec=new r.Ros2Pal_device_msgsSrvCancelEffect_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effect_id == <u32>(1821290051));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("");const codec=new r.Ros2Pal_device_msgsSrvCancelEffect_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("ff69128671d0081d");const codec=new r.Ros2Pal_device_msgsSrvShutdownAndWakeUpRobot_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.shutdown_duration.sec == <i32>(-2045613569));
assert(value.shutdown_duration.nanosec == <u32>(487116913));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("");const codec=new r.Ros2Pal_device_msgsSrvShutdownAndWakeUpRobot_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){

const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("5a4b1f69008036c300807bc200c073c300a02743000026410000334200800f4200a05fc38b7d7260fcf60f143381ee30df28281e8db3f85dbabee5347f");const codec=new r.Ros2Pal_device_msgsSrvTimedBlinkEffect_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.leds.led_mask == <u32>(1763658586));
assert(value.first_color.r == <f32>(-182.5));
assert(value.first_color.g == <f32>(-62.875));
assert(value.first_color.b == <f32>(-243.75));
assert(value.first_color.a == <f32>(167.625));
assert(value.second_color.r == <f32>(10.375));
assert(value.second_color.g == <f32>(44.75));
assert(value.second_color.b == <f32>(35.875));
assert(value.second_color.a == <f32>(-223.625));
assert(value.first_color_duration.sec == <i32>(1618115979));
assert(value.first_color_duration.nanosec == <u32>(336590588));
assert(value.second_color_duration.sec == <i32>(820937011));
assert(value.second_color_duration.nanosec == <u32>(505948383));
assert(value.effect_duration.sec == <i32>(1576579981));
assert(value.effect_duration.nanosec == <u32>(887471802));
assert(value.priority == <u8>(127));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("e687ae1f");const codec=new r.Ros2Pal_device_msgsSrvTimedBlinkEffect_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effect_id == <u32>(531531750));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("72f54d350040e542002064c3000077c3008066425e92521de9e90b0c9f");const codec=new r.Ros2Pal_device_msgsSrvTimedColourEffect_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.leds.led_mask == <u32>(894301554));
assert(value.color.r == <f32>(114.625));
assert(value.color.g == <f32>(-228.125));
assert(value.color.b == <f32>(-247.0));
assert(value.color.a == <f32>(57.625));
assert(value.effect_duration.sec == <i32>(491950686));
assert(value.effect_duration.nanosec == <u32>(202107369));
assert(value.priority == <u8>(159));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_25():void{const input=bytes("8d1cb5cb");const codec=new r.Ros2Pal_device_msgsSrvTimedColourEffect_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effect_id == <u32>(3417644173));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_26():void{const input=bytes("22f11a0300008842008038c3000088420000dcc100800dc2000030c100807ac200e073c3a94ac4332cb9d62d01509260921a987e051e");const codec=new r.Ros2Pal_device_msgsSrvTimedFadeEffect_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.leds.led_mask == <u32>(52097314));
assert(value.first_color.r == <f32>(68.0));
assert(value.first_color.g == <f32>(-184.5));
assert(value.first_color.b == <f32>(68.0));
assert(value.first_color.a == <f32>(-27.5));
assert(value.second_color.r == <f32>(-35.375));
assert(value.second_color.g == <f32>(-11.0));
assert(value.second_color.b == <f32>(-62.625));
assert(value.second_color.a == <f32>(-243.875));
assert(value.color_change_duration.sec == <i32>(868502185));
assert(value.color_change_duration.nanosec == <u32>(769046828));
assert(value.reverse_fade == true);
assert(value.effect_duration.sec == <i32>(-1839164848));
assert(value.effect_duration.nanosec == <u32>(92182554));
assert(value.priority == <u8>(30));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_27():void{const input=bytes("827b2b85");const codec=new r.Ros2Pal_device_msgsSrvTimedFadeEffect_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.effect_id == <u32>(2234219394));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_28():void{const input=bytes("00e06dc300c06ac30000a8c200809942");const codec=new r.Ros2Std_msgsMsgColorRGBACodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.r == <f32>(-237.875));
assert(value.g == <f32>(-234.75));
assert(value.b == <f32>(-84.0));
assert(value.a == <f32>(76.75));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_29():void{const input=bytes("681252e4acc1ba37070000007666316532ceb2");const codec=new r.Ros2Std_msgsMsgHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
}
