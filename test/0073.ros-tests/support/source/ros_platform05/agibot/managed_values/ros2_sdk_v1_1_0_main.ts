/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../sdk/origin/custom-types/agibot-ros2-sdk-v1-1-0-main-complete/bounded_records";
import {abortGuest} from "../../../../../../../sdk/origin/assemblyscript/api";
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

function test_3():void{const input=bytes("b3637f4f0a1a48d135a1a4db8cd89345070000007665663839ceb2");const codec=new r.Ros2Aimdk_msgsMsgAlertCodeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.timestamp == <u64><u64>0xd1481a0a4f7f63b3);
assert(value.first_timestamp == <u64><u64>0x4593d88cdba4a135);
assert(value.code == "vef89β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("e7ab8c5c71dd1024070000007639326665ceb260e2dbffd988d7dad05a552a010000003b35b332824c84ac56a7a6feefde9160070000007639376639ceb2");const codec=new r.Ros2Aimdk_msgsMsgAlertCodeArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1552722919));
assert(value.header.stamp.nanosec == <u32>(605085041));
assert(value.header.frame_id == "v92feβ");
assert(value.header.sequence == <u32>(4292600416));
assert(value.header.meas_stamp.sec == <i32>(-623408935));
assert(value.header.meas_stamp.nanosec == <u32>(710236880));
assert(value.list.length == 1);
assert(value.list[0].timestamp == <u64><u64>0xac844c8232b3353b);
assert(value.list[0].first_timestamp == <u64><u64>0x6091deeffea6a756);
assert(value.list[0].code == "v97f9β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("faab1a32f0ad180b1d139792ef272e464263b2070000007631353961ceb2070000007638343839ceb20100000028070000007665373430ceb2");const codec=new r.Ros2Aimdk_msgsMsgAudioCaptureCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamps.sec == <i32>(840608762));
assert(value.stamps.nanosec == <u32>(186166768));
assert(value.mic_channels == <u8>(29));
assert(value.ref_channels == <u8>(19));
assert(value.info.channels == <u8>(151));
assert(value.info.sample_rate == <u32>(774369170));
assert(value.info.size == <u32>(2992849478));
assert(value.info.sample_format == "v159aβ");
assert(value.info.coding_format == "v8489β");
assert(value.data.data.length == 1);
assert(value.data.data[0] == <u8>(40));
assert(value.pkg_name == "ve740β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("010000005c");const codec=new r.Ros2Aimdk_msgsMsgAudioDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.data.length == 1);
assert(value.data[0] == <u8>(92));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("070000007661306563ceb2070000007663653833ceb2070000007639626634ceb22eaeaca99336f34573070000007638396263ceb2070000007631386166ceb2b1005c5235131be4");const codec=new r.Ros2Aimdk_msgsMsgAudioFileCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pkg_name == "va0ecβ");
assert(value.file_name == "vce83β");
assert(value.file_path == "v9bf4β");
assert(value.info.channels == <u8>(46));
assert(value.info.sample_rate == <u32>(2477370542));
assert(value.info.size == <u32>(1933964086));
assert(value.info.sample_format == "v89bcβ");
assert(value.info.coding_format == "v18afβ");
assert(value.priority == <u32>(1381761201));
assert(value.priority_weight == <u32>(3826979637));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("72306fde431a9c97e9070000007636343133ceb2070000007666353030ceb2");const codec=new r.Ros2Aimdk_msgsMsgAudioInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.channels == <u8>(114));
assert(value.sample_rate == <u32>(1138650928));
assert(value.size == <u32>(3919027226));
assert(value.sample_format == "v6413β");
assert(value.coding_format == "vf500β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("dea399c93dfe302fc2976617c4190327e5070000007666633839ceb2070000007636643961ceb2010000003f070000007663626234ceb2070000007631613763ceb2");const codec=new r.Ros2Aimdk_msgsMsgAudioPlaybackCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamps.sec == <i32>(-912677922));
assert(value.stamps.nanosec == <u32>(791739965));
assert(value.info.channels == <u8>(194));
assert(value.info.sample_rate == <u32>(3289867927));
assert(value.info.size == <u32>(3844539161));
assert(value.info.sample_format == "vfc89β");
assert(value.info.coding_format == "v6d9aβ");
assert(value.data.data.length == 1);
assert(value.data.data[0] == <u8>(63));
assert(value.pkg_name == "vcbb4β");
assert(value.token_id == "v1a7cβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("9b");const codec=new r.Ros2Aimdk_msgsMsgAudioVadStateTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(155));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("16bb4308af59a921");const codec=new r.Ros2Aimdk_msgsMsgCommonRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(138656534));
assert(value.header.stamp.nanosec == <u32>(564746671));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("7e48d16d857af630efb3162656ca21b86ea99a05070000007634616561ceb2");const codec=new r.Ros2Aimdk_msgsMsgCommonResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1842432126));
assert(value.header.stamp.nanosec == <u32>(821459589));
assert(value.header.code == <i64><u64>0xb821ca562616b3ef);
assert(value.status.value == <i32>(94021998));
assert(value.message == "v4aeaβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("d226915e");const codec=new r.Ros2Aimdk_msgsMsgCommonStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1586570962));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("1571c861ab351d049c09c55f2570f2c1");const codec=new r.Ros2Aimdk_msgsMsgCommonTaskRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1640526101));
assert(value.header.stamp.nanosec == <u32>(69023147));
assert(value.task_id == <u64><u64>0xc1f270255fc5099c);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("0e92b1f4b82af9317ff79a4fc68eadd1888295fa31fba264f00f1460");const codec=new r.Ros2Aimdk_msgsMsgCommonTaskResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-189689330));
assert(value.header.stamp.nanosec == <u32>(838412984));
assert(value.header.code == <i64><u64>0xd1ad8ec64f9af77f);
assert(value.task_id == <u64><u64>0x64a2fb31fa958288);
assert(value.state.value == <i32>(1611927536));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("070000007666373438ceb2070000007637333563ceb201000000070000007635663631ceb28fd4245f36ad13c1");const codec=new r.Ros2Aimdk_msgsMsgCurrentVersionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.version == "vf748β");
assert(value.name == "v735cβ");
assert(value.files.length == 1);
assert(value.files[0] == "v5f61β");
assert(value.download_timestamp_seconds == <u64><u64>0xc113ad365f24d48f);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("776945abce1072358f85102a36fc27b44d57286e61b408a1c8f23f93f24b46a46c070000007637343364ceb2");const codec=new r.Ros2Aimdk_msgsMsgDiagnosticInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.timestamp == <u64><u64>0x357210ceab456977);
assert(value.first_timestamp == <u64><u64>0xb427fc362a10858f);
assert(value.ros_type.value == <i32>(1848137549));
assert(value.module_id == <u8>(97));
assert(value.code == <u32>(3365996724));
assert(value.diag_code == <u64><u64>0x6ca4464bf2933ff2);
assert(value.info == "v743dβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("1ad28af51e1a9d00070000007632383239ceb28c4c0c4531880c07ba35ef28010000009765ec4b2e1cdbd523f525609a8c12fe391cc2ce81c258c64b12333a12ab4a0d8c070000007666373561ceb2");const codec=new r.Ros2Aimdk_msgsMsgDiagnosticInfoArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-175451622));
assert(value.header.stamp.nanosec == <u32>(10295838));
assert(value.header.frame_id == "v2829β");
assert(value.header.sequence == <u32>(1158433932));
assert(value.header.meas_stamp.sec == <i32>(118261809));
assert(value.header.meas_stamp.nanosec == <u32>(686765498));
assert(value.diagnostics.length == 1);
assert(value.diagnostics[0].timestamp == <u64><u64>0xd5db1c2e4bec6597);
assert(value.diagnostics[0].first_timestamp == <u64><u64>0xfe128c9a6025f523);
assert(value.diagnostics[0].ros_type.value == <i32>(-826139591));
assert(value.diagnostics[0].module_id == <u8>(129));
assert(value.diagnostics[0].code == <u32>(1271290050));
assert(value.diagnostics[0].diag_code == <u64><u64>0x8c0d4aab123a3312);
assert(value.diagnostics[0].info == "vf75aβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("ffe92280");const codec=new r.Ros2Aimdk_msgsMsgDiagnosticTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(-2145195521));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("e1");const codec=new r.Ros2Aimdk_msgsMsgDomainErrorStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(225));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("1c4abca49d92eb07070000007663306231ceb2bd7094aded41fe734e14bc34070000007666313762ceb201000000070000007665653538ceb22581536c49895700000000000c614060070000007635356532ceb2351950b3");const codec=new r.Ros2Aimdk_msgsMsgFaceEmojiStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
assert(value.display_type == <u8>(96));
assert(value.trace_id == "v55e2β");
assert(value.error_code == <i32>(-1286596299));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("72a49b9ecbddac00070000007665373232ceb2070000007664663330ceb2838c0f773af538e9070000007630666238ceb2c45aae287d2399b68b96ec745f17d1a111aea8968f");const codec=new r.Ros2Aimdk_msgsMsgFixLineIntroStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.set_task_id == <u64><u64>0x00acddcb9e9ba472);
assert(value.set_task_name == "ve722β");
assert(value.task_name == "vdf30β");
assert(value.task_id == <u64><u64>0xe938f53a770f8c83);
assert(value.map_name == "v0fb8β");
assert(value.map_id == <u64><u64>0xb699237d28ae5ac4);
assert(value.rest_site_number == <u32>(1961662091));
assert(value.fix_line_intro_state == <u8>(95));
assert(value.cur_target_id == <u64><u64>0x8f96a8ae11a1d117);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("070000007663363537ceb22668e7346a478953");const codec=new r.Ros2Aimdk_msgsMsgFocusRequesterCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pkg_name == "vc657β");
assert(value.priority == <u32>(887580710));
assert(value.priority_weight == <u32>(1401505642));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("070000007661373961ceb200");const codec=new r.Ros2Aimdk_msgsMsgFocusResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pkg_name == "va79aβ");
assert(value.focus_gain == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_25():void{const input=bytes("cf029c7100000000000c674000000000001c6b400000000000fc66400000000000ac6b400000000000786dc00000000000f055c00000000000fc6c4001000000b2e9220f00000000007c6bc00000000000bc6bc00000000000286d400000000000d86c400000000000dc65c000000000006c67c00000000000b8604001");const codec=new r.Ros2Aimdk_msgsMsgHRPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.point_id == <i32>(1906049743));
assert(value.pose.position.x == <f64>(184.375));
assert(value.pose.position.y == <f64>(216.875));
assert(value.pose.position.z == <f64>(183.875));
assert(value.pose.orientation.x == <f64>(221.375));
assert(value.pose.orientation.y == <f64>(-235.75));
assert(value.pose.orientation.z == <f64>(-87.75));
assert(value.pose.orientation.w == <f64>(231.875));
assert(value.qr_list.length == 1);
assert(value.qr_list[0].code == <i32>(253946290));
assert(value.qr_list[0].pose.position.x == <f64>(-219.875));
assert(value.qr_list[0].pose.position.y == <f64>(-221.875));
assert(value.qr_list[0].pose.position.z == <f64>(233.25));
assert(value.qr_list[0].pose.orientation.x == <f64>(230.75));
assert(value.qr_list[0].pose.orientation.y == <f64>(-174.875));
assert(value.qr_list[0].pose.orientation.z == <f64>(-187.375));
assert(value.qr_list[0].pose.orientation.w == <f64>(133.75));
assert(value.use_head == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_26():void{const input=bytes("070000007636623363ceb20000000000e054c00000000000b46e400000000000a037c0000000000028624000000000002c6b40");const codec=new r.Ros2Aimdk_msgsMsgHandCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "v6b3cβ");
assert(value.position == <f64>(-83.5));
assert(value.velocity == <f64>(245.625));
assert(value.acceleration == <f64>(-23.625));
assert(value.deceleration == <f64>(145.25));
assert(value.effort == <f64>(217.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_27():void{const input=bytes("0a807d12feed941e070000007636373536ceb2825b730ae4d85ca71dacb439fa01000000070000007665323962ceb20000000000000ec0000000000038604000000000004c6ac00000000000946ac00000000000e068c09a01000000070000007666366562ceb20000000000905c40000000000090504000000000006469c000000000008047c000000000000c62c0");const codec=new r.Ros2Aimdk_msgsMsgHandCommandArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_28():void{const input=bytes("070000007663636334ceb20000000000f8574000000000000c614000000000009c6840f0e70c745ba49b55");const codec=new r.Ros2Aimdk_msgsMsgHandStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vccc4β");
assert(value.position == <f64>(95.875));
assert(value.velocity == <f64>(136.375));
assert(value.effort == <f64>(196.875));
assert(value.state == <i32>(1947002864));
assert(value.faultcode == <i32>(1436263515));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_29():void{const input=bytes("153e6a78fd2f7135070000007663306338ceb293b6edadc6d73a3e54d7710bf901000000070000007630333138ceb20000000000e03b400000000000b04ec00000000000346240434ccfb8b8fd74fdfebf7c3dfabb7839f6b76928ebaa6d2cefae61203071b2f33475b6f73879074685c403425312d1905716d5945b1aa2e32061a6e72465aaebfbba7938ffbe7d3cf3b2cc8d4e0fc889cb8a4908cf8e4d0cc3824001c2834405145596d7105192d31c5d8ecf0c4d8acb2263a0e12667a4e52a6b5b1ad9985f1eebaa6928efae6d2ce3a26e2fecad6a2b3071b2f33475b6f738795011d2935415b801000000070000007631383034ceb20000000000286e4000000000000017c00000000000b064401dfed4b2d38d9f5992d3105196d714559adbbcfd3e7fb8f93a7bb4f5e5a46726e1a06322edacd2935011d6973d7cbffe3978bbfa35743574b7f63170b3f23d7c6c2deeaf6829eaab64255b1ad9985f1e1e5f9cdd1a5b98d9165784c5064780c17a3bf8b97e3ffcbd723319589bda1d5cb5f43776b1f03372bdfcadec2f6ea9e898d91a5b9cdd1e5f90d1004182c30445a7e62564a3e22160afeea6e72465a2e3");const codec=new r.Ros2Aimdk_msgsMsgHandStateArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
assert(value.left_touch_sensors.palm_touch_data.length == 36);
assert(value.left_touch_sensors.palm_touch_data[0] == <u8>(254));
assert(value.left_touch_sensors.palm_touch_data[1] == <u8>(191));
assert(value.left_touch_sensors.palm_touch_data[2] == <u8>(124));
assert(value.left_touch_sensors.palm_touch_data[3] == <u8>(61));
assert(value.left_touch_sensors.palm_touch_data[4] == <u8>(250));
assert(value.left_touch_sensors.palm_touch_data[5] == <u8>(187));
assert(value.left_touch_sensors.palm_touch_data[6] == <u8>(120));
assert(value.left_touch_sensors.palm_touch_data[7] == <u8>(57));
assert(value.left_touch_sensors.palm_touch_data[8] == <u8>(246));
assert(value.left_touch_sensors.palm_touch_data[9] == <u8>(183));
assert(value.left_touch_sensors.palm_touch_data[10] == <u8>(105));
assert(value.left_touch_sensors.palm_touch_data[11] == <u8>(40));
assert(value.left_touch_sensors.palm_touch_data[12] == <u8>(235));
assert(value.left_touch_sensors.palm_touch_data[13] == <u8>(170));
assert(value.left_touch_sensors.palm_touch_data[14] == <u8>(109));
assert(value.left_touch_sensors.palm_touch_data[15] == <u8>(44));
assert(value.left_touch_sensors.palm_touch_data[16] == <u8>(239));
assert(value.left_touch_sensors.palm_touch_data[17] == <u8>(174));
assert(value.left_touch_sensors.palm_touch_data[18] == <u8>(97));
assert(value.left_touch_sensors.palm_touch_data[19] == <u8>(32));
assert(value.left_touch_sensors.palm_touch_data[20] == <u8>(48));
assert(value.left_touch_sensors.palm_touch_data[21] == <u8>(113));
assert(value.left_touch_sensors.palm_touch_data[22] == <u8>(178));
assert(value.left_touch_sensors.palm_touch_data[23] == <u8>(243));
assert(value.left_touch_sensors.palm_touch_data[24] == <u8>(52));
assert(value.left_touch_sensors.palm_touch_data[25] == <u8>(117));
assert(value.left_touch_sensors.palm_touch_data[26] == <u8>(182));
assert(value.left_touch_sensors.palm_touch_data[27] == <u8>(247));
assert(value.left_touch_sensors.palm_touch_data[28] == <u8>(56));
assert(value.left_touch_sensors.palm_touch_data[29] == <u8>(121));
assert(value.left_touch_sensors.palm_touch_data[30] == <u8>(7));
assert(value.left_touch_sensors.palm_touch_data[31] == <u8>(70));
assert(value.left_touch_sensors.palm_touch_data[32] == <u8>(133));
assert(value.left_touch_sensors.palm_touch_data[33] == <u8>(196));
assert(value.left_touch_sensors.palm_touch_data[34] == <u8>(3));
assert(value.left_touch_sensors.palm_touch_data[35] == <u8>(66));
assert(value.left_touch_sensors.back_of_hand_touch_data.length == 36);
assert(value.left_touch_sensors.back_of_hand_touch_data[0] == <u8>(83));
assert(value.left_touch_sensors.back_of_hand_touch_data[1] == <u8>(18));
assert(value.left_touch_sensors.back_of_hand_touch_data[2] == <u8>(209));
assert(value.left_touch_sensors.back_of_hand_touch_data[3] == <u8>(144));
assert(value.left_touch_sensors.back_of_hand_touch_data[4] == <u8>(87));
assert(value.left_touch_sensors.back_of_hand_touch_data[5] == <u8>(22));
assert(value.left_touch_sensors.back_of_hand_touch_data[6] == <u8>(213));
assert(value.left_touch_sensors.back_of_hand_touch_data[7] == <u8>(148));
assert(value.left_touch_sensors.back_of_hand_touch_data[8] == <u8>(91));
assert(value.left_touch_sensors.back_of_hand_touch_data[9] == <u8>(26));
assert(value.left_touch_sensors.back_of_hand_touch_data[10] == <u8>(162));
assert(value.left_touch_sensors.back_of_hand_touch_data[11] == <u8>(227));
assert(value.left_touch_sensors.back_of_hand_touch_data[12] == <u8>(32));
assert(value.left_touch_sensors.back_of_hand_touch_data[13] == <u8>(97));
assert(value.left_touch_sensors.back_of_hand_touch_data[14] == <u8>(166));
assert(value.left_touch_sensors.back_of_hand_touch_data[15] == <u8>(231));
assert(value.left_touch_sensors.back_of_hand_touch_data[16] == <u8>(36));
assert(value.left_touch_sensors.back_of_hand_touch_data[17] == <u8>(101));
assert(value.left_touch_sensors.back_of_hand_touch_data[18] == <u8>(170));
assert(value.left_touch_sensors.back_of_hand_touch_data[19] == <u8>(235));
assert(value.left_touch_sensors.back_of_hand_touch_data[20] == <u8>(251));
assert(value.left_touch_sensors.back_of_hand_touch_data[21] == <u8>(186));
assert(value.left_touch_sensors.back_of_hand_touch_data[22] == <u8>(121));
assert(value.left_touch_sensors.back_of_hand_touch_data[23] == <u8>(56));
assert(value.left_touch_sensors.back_of_hand_touch_data[24] == <u8>(255));
assert(value.left_touch_sensors.back_of_hand_touch_data[25] == <u8>(190));
assert(value.left_touch_sensors.back_of_hand_touch_data[26] == <u8>(125));
assert(value.left_touch_sensors.back_of_hand_touch_data[27] == <u8>(60));
assert(value.left_touch_sensors.back_of_hand_touch_data[28] == <u8>(243));
assert(value.left_touch_sensors.back_of_hand_touch_data[29] == <u8>(178));
assert(value.left_touch_sensors.back_of_hand_touch_data[30] == <u8>(204));
assert(value.left_touch_sensors.back_of_hand_touch_data[31] == <u8>(141));
assert(value.left_touch_sensors.back_of_hand_touch_data[32] == <u8>(78));
assert(value.left_touch_sensors.back_of_hand_touch_data[33] == <u8>(15));
assert(value.left_touch_sensors.back_of_hand_touch_data[34] == <u8>(200));
assert(value.left_touch_sensors.back_of_hand_touch_data[35] == <u8>(137));
assert(value.left_touch_sensors.thumb_touch_data.length == 16);
assert(value.left_touch_sensors.thumb_touch_data[0] == <u8>(203));
assert(value.left_touch_sensors.thumb_touch_data[1] == <u8>(138));
assert(value.left_touch_sensors.thumb_touch_data[2] == <u8>(73));
assert(value.left_touch_sensors.thumb_touch_data[3] == <u8>(8));
assert(value.left_touch_sensors.thumb_touch_data[4] == <u8>(207));
assert(value.left_touch_sensors.thumb_touch_data[5] == <u8>(142));
assert(value.left_touch_sensors.thumb_touch_data[6] == <u8>(77));
assert(value.left_touch_sensors.thumb_touch_data[7] == <u8>(12));
assert(value.left_touch_sensors.thumb_touch_data[8] == <u8>(195));
assert(value.left_touch_sensors.thumb_touch_data[9] == <u8>(130));
assert(value.left_touch_sensors.thumb_touch_data[10] == <u8>(64));
assert(value.left_touch_sensors.thumb_touch_data[11] == <u8>(1));
assert(value.left_touch_sensors.thumb_touch_data[12] == <u8>(194));
assert(value.left_touch_sensors.thumb_touch_data[13] == <u8>(131));
assert(value.left_touch_sensors.thumb_touch_data[14] == <u8>(68));
assert(value.left_touch_sensors.thumb_touch_data[15] == <u8>(5));
assert(value.left_touch_sensors.index_finger_touch_data.length == 16);
assert(value.left_touch_sensors.index_finger_touch_data[0] == <u8>(20));
assert(value.left_touch_sensors.index_finger_touch_data[1] == <u8>(85));
assert(value.left_touch_sensors.index_finger_touch_data[2] == <u8>(150));
assert(value.left_touch_sensors.index_finger_touch_data[3] == <u8>(215));
assert(value.left_touch_sensors.index_finger_touch_data[4] == <u8>(16));
assert(value.left_touch_sensors.index_finger_touch_data[5] == <u8>(81));
assert(value.left_touch_sensors.index_finger_touch_data[6] == <u8>(146));
assert(value.left_touch_sensors.index_finger_touch_data[7] == <u8>(211));
assert(value.left_touch_sensors.index_finger_touch_data[8] == <u8>(28));
assert(value.left_touch_sensors.index_finger_touch_data[9] == <u8>(93));
assert(value.left_touch_sensors.index_finger_touch_data[10] == <u8>(142));
assert(value.left_touch_sensors.index_finger_touch_data[11] == <u8>(207));
assert(value.left_touch_sensors.index_finger_touch_data[12] == <u8>(12));
assert(value.left_touch_sensors.index_finger_touch_data[13] == <u8>(77));
assert(value.left_touch_sensors.index_finger_touch_data[14] == <u8>(138));
assert(value.left_touch_sensors.index_finger_touch_data[15] == <u8>(203));
assert(value.left_touch_sensors.middle_finger_touch_data.length == 16);
assert(value.left_touch_sensors.middle_finger_touch_data[0] == <u8>(34));
assert(value.left_touch_sensors.middle_finger_touch_data[1] == <u8>(99));
assert(value.left_touch_sensors.middle_finger_touch_data[2] == <u8>(160));
assert(value.left_touch_sensors.middle_finger_touch_data[3] == <u8>(225));
assert(value.left_touch_sensors.middle_finger_touch_data[4] == <u8>(38));
assert(value.left_touch_sensors.middle_finger_touch_data[5] == <u8>(103));
assert(value.left_touch_sensors.middle_finger_touch_data[6] == <u8>(164));
assert(value.left_touch_sensors.middle_finger_touch_data[7] == <u8>(229));
assert(value.left_touch_sensors.middle_finger_touch_data[8] == <u8>(42));
assert(value.left_touch_sensors.middle_finger_touch_data[9] == <u8>(107));
assert(value.left_touch_sensors.middle_finger_touch_data[10] == <u8>(91));
assert(value.left_touch_sensors.middle_finger_touch_data[11] == <u8>(26));
assert(value.left_touch_sensors.middle_finger_touch_data[12] == <u8>(217));
assert(value.left_touch_sensors.middle_finger_touch_data[13] == <u8>(152));
assert(value.left_touch_sensors.middle_finger_touch_data[14] == <u8>(95));
assert(value.left_touch_sensors.middle_finger_touch_data[15] == <u8>(30));
assert(value.left_touch_sensors.ring_finger_touch_data.length == 16);
assert(value.left_touch_sensors.ring_finger_touch_data[0] == <u8>(235));
assert(value.left_touch_sensors.ring_finger_touch_data[1] == <u8>(170));
assert(value.left_touch_sensors.ring_finger_touch_data[2] == <u8>(105));
assert(value.left_touch_sensors.ring_finger_touch_data[3] == <u8>(40));
assert(value.left_touch_sensors.ring_finger_touch_data[4] == <u8>(239));
assert(value.left_touch_sensors.ring_finger_touch_data[5] == <u8>(174));
assert(value.left_touch_sensors.ring_finger_touch_data[6] == <u8>(109));
assert(value.left_touch_sensors.ring_finger_touch_data[7] == <u8>(44));
assert(value.left_touch_sensors.ring_finger_touch_data[8] == <u8>(227));
assert(value.left_touch_sensors.ring_finger_touch_data[9] == <u8>(162));
assert(value.left_touch_sensors.ring_finger_touch_data[10] == <u8>(110));
assert(value.left_touch_sensors.ring_finger_touch_data[11] == <u8>(47));
assert(value.left_touch_sensors.ring_finger_touch_data[12] == <u8>(236));
assert(value.left_touch_sensors.ring_finger_touch_data[13] == <u8>(173));
assert(value.left_touch_sensors.ring_finger_touch_data[14] == <u8>(106));
assert(value.left_touch_sensors.ring_finger_touch_data[15] == <u8>(43));
assert(value.left_touch_sensors.little_finger_touch_data.length == 16);
assert(value.left_touch_sensors.little_finger_touch_data[0] == <u8>(48));
assert(value.left_touch_sensors.little_finger_touch_data[1] == <u8>(113));
assert(value.left_touch_sensors.little_finger_touch_data[2] == <u8>(178));
assert(value.left_touch_sensors.little_finger_touch_data[3] == <u8>(243));
assert(value.left_touch_sensors.little_finger_touch_data[4] == <u8>(52));
assert(value.left_touch_sensors.little_finger_touch_data[5] == <u8>(117));
assert(value.left_touch_sensors.little_finger_touch_data[6] == <u8>(182));
assert(value.left_touch_sensors.little_finger_touch_data[7] == <u8>(247));
assert(value.left_touch_sensors.little_finger_touch_data[8] == <u8>(56));
assert(value.left_touch_sensors.little_finger_touch_data[9] == <u8>(121));
assert(value.left_touch_sensors.little_finger_touch_data[10] == <u8>(80));
assert(value.left_touch_sensors.little_finger_touch_data[11] == <u8>(17));
assert(value.left_touch_sensors.little_finger_touch_data[12] == <u8>(210));
assert(value.left_touch_sensors.little_finger_touch_data[13] == <u8>(147));
assert(value.left_touch_sensors.little_finger_touch_data[14] == <u8>(84));
assert(value.left_touch_sensors.little_finger_touch_data[15] == <u8>(21));
assert(value.right_hand_type.value == <u8>(184));
assert(value.right_hands.length == 1);
assert(value.right_hands[0].name == "v1804β");
assert(value.right_hands[0].position == <f64>(241.25));
assert(value.right_hands[0].velocity == <f64>(-5.75));
assert(value.right_hands[0].effort == <f64>(165.5));
assert(value.right_hands[0].state == <i32>(-1294664163));
assert(value.right_hands[0].faultcode == <i32>(1503628755));
assert(value.right_touch_sensors.palm_touch_data.length == 36);
assert(value.right_touch_sensors.palm_touch_data[0] == <u8>(146));
assert(value.right_touch_sensors.palm_touch_data[1] == <u8>(211));
assert(value.right_touch_sensors.palm_touch_data[2] == <u8>(16));
assert(value.right_touch_sensors.palm_touch_data[3] == <u8>(81));
assert(value.right_touch_sensors.palm_touch_data[4] == <u8>(150));
assert(value.right_touch_sensors.palm_touch_data[5] == <u8>(215));
assert(value.right_touch_sensors.palm_touch_data[6] == <u8>(20));
assert(value.right_touch_sensors.palm_touch_data[7] == <u8>(85));
assert(value.right_touch_sensors.palm_touch_data[8] == <u8>(154));
assert(value.right_touch_sensors.palm_touch_data[9] == <u8>(219));
assert(value.right_touch_sensors.palm_touch_data[10] == <u8>(188));
assert(value.right_touch_sensors.palm_touch_data[11] == <u8>(253));
assert(value.right_touch_sensors.palm_touch_data[12] == <u8>(62));
assert(value.right_touch_sensors.palm_touch_data[13] == <u8>(127));
assert(value.right_touch_sensors.palm_touch_data[14] == <u8>(184));
assert(value.right_touch_sensors.palm_touch_data[15] == <u8>(249));
assert(value.right_touch_sensors.palm_touch_data[16] == <u8>(58));
assert(value.right_touch_sensors.palm_touch_data[17] == <u8>(123));
assert(value.right_touch_sensors.palm_touch_data[18] == <u8>(180));
assert(value.right_touch_sensors.palm_touch_data[19] == <u8>(245));
assert(value.right_touch_sensors.palm_touch_data[20] == <u8>(229));
assert(value.right_touch_sensors.palm_touch_data[21] == <u8>(164));
assert(value.right_touch_sensors.palm_touch_data[22] == <u8>(103));
assert(value.right_touch_sensors.palm_touch_data[23] == <u8>(38));
assert(value.right_touch_sensors.palm_touch_data[24] == <u8>(225));
assert(value.right_touch_sensors.palm_touch_data[25] == <u8>(160));
assert(value.right_touch_sensors.palm_touch_data[26] == <u8>(99));
assert(value.right_touch_sensors.palm_touch_data[27] == <u8>(34));
assert(value.right_touch_sensors.palm_touch_data[28] == <u8>(237));
assert(value.right_touch_sensors.palm_touch_data[29] == <u8>(172));
assert(value.right_touch_sensors.palm_touch_data[30] == <u8>(210));
assert(value.right_touch_sensors.palm_touch_data[31] == <u8>(147));
assert(value.right_touch_sensors.palm_touch_data[32] == <u8>(80));
assert(value.right_touch_sensors.palm_touch_data[33] == <u8>(17));
assert(value.right_touch_sensors.palm_touch_data[34] == <u8>(214));
assert(value.right_touch_sensors.palm_touch_data[35] == <u8>(151));
assert(value.right_touch_sensors.back_of_hand_touch_data.length == 36);
assert(value.right_touch_sensors.back_of_hand_touch_data[0] == <u8>(61));
assert(value.right_touch_sensors.back_of_hand_touch_data[1] == <u8>(124));
assert(value.right_touch_sensors.back_of_hand_touch_data[2] == <u8>(191));
assert(value.right_touch_sensors.back_of_hand_touch_data[3] == <u8>(254));
assert(value.right_touch_sensors.back_of_hand_touch_data[4] == <u8>(57));
assert(value.right_touch_sensors.back_of_hand_touch_data[5] == <u8>(120));
assert(value.right_touch_sensors.back_of_hand_touch_data[6] == <u8>(187));
assert(value.right_touch_sensors.back_of_hand_touch_data[7] == <u8>(250));
assert(value.right_touch_sensors.back_of_hand_touch_data[8] == <u8>(53));
assert(value.right_touch_sensors.back_of_hand_touch_data[9] == <u8>(116));
assert(value.right_touch_sensors.back_of_hand_touch_data[10] == <u8>(53));
assert(value.right_touch_sensors.back_of_hand_touch_data[11] == <u8>(116));
assert(value.right_touch_sensors.back_of_hand_touch_data[12] == <u8>(183));
assert(value.right_touch_sensors.back_of_hand_touch_data[13] == <u8>(246));
assert(value.right_touch_sensors.back_of_hand_touch_data[14] == <u8>(49));
assert(value.right_touch_sensors.back_of_hand_touch_data[15] == <u8>(112));
assert(value.right_touch_sensors.back_of_hand_touch_data[16] == <u8>(179));
assert(value.right_touch_sensors.back_of_hand_touch_data[17] == <u8>(242));
assert(value.right_touch_sensors.back_of_hand_touch_data[18] == <u8>(61));
assert(value.right_touch_sensors.back_of_hand_touch_data[19] == <u8>(124));
assert(value.right_touch_sensors.back_of_hand_touch_data[20] == <u8>(108));
assert(value.right_touch_sensors.back_of_hand_touch_data[21] == <u8>(45));
assert(value.right_touch_sensors.back_of_hand_touch_data[22] == <u8>(238));
assert(value.right_touch_sensors.back_of_hand_touch_data[23] == <u8>(175));
assert(value.right_touch_sensors.back_of_hand_touch_data[24] == <u8>(104));
assert(value.right_touch_sensors.back_of_hand_touch_data[25] == <u8>(41));
assert(value.right_touch_sensors.back_of_hand_touch_data[26] == <u8>(234));
assert(value.right_touch_sensors.back_of_hand_touch_data[27] == <u8>(171));
assert(value.right_touch_sensors.back_of_hand_touch_data[28] == <u8>(100));
assert(value.right_touch_sensors.back_of_hand_touch_data[29] == <u8>(37));
assert(value.right_touch_sensors.back_of_hand_touch_data[30] == <u8>(91));
assert(value.right_touch_sensors.back_of_hand_touch_data[31] == <u8>(26));
assert(value.right_touch_sensors.back_of_hand_touch_data[32] == <u8>(217));
assert(value.right_touch_sensors.back_of_hand_touch_data[33] == <u8>(152));
assert(value.right_touch_sensors.back_of_hand_touch_data[34] == <u8>(95));
assert(value.right_touch_sensors.back_of_hand_touch_data[35] == <u8>(30));
assert(value.right_touch_sensors.thumb_touch_data.length == 16);
assert(value.right_touch_sensors.thumb_touch_data[0] == <u8>(30));
assert(value.right_touch_sensors.thumb_touch_data[1] == <u8>(95));
assert(value.right_touch_sensors.thumb_touch_data[2] == <u8>(156));
assert(value.right_touch_sensors.thumb_touch_data[3] == <u8>(221));
assert(value.right_touch_sensors.thumb_touch_data[4] == <u8>(26));
assert(value.right_touch_sensors.thumb_touch_data[5] == <u8>(91));
assert(value.right_touch_sensors.thumb_touch_data[6] == <u8>(152));
assert(value.right_touch_sensors.thumb_touch_data[7] == <u8>(217));
assert(value.right_touch_sensors.thumb_touch_data[8] == <u8>(22));
assert(value.right_touch_sensors.thumb_touch_data[9] == <u8>(87));
assert(value.right_touch_sensors.thumb_touch_data[10] == <u8>(132));
assert(value.right_touch_sensors.thumb_touch_data[11] == <u8>(197));
assert(value.right_touch_sensors.thumb_touch_data[12] == <u8>(6));
assert(value.right_touch_sensors.thumb_touch_data[13] == <u8>(71));
assert(value.right_touch_sensors.thumb_touch_data[14] == <u8>(128));
assert(value.right_touch_sensors.thumb_touch_data[15] == <u8>(193));
assert(value.right_touch_sensors.index_finger_touch_data.length == 16);
assert(value.right_touch_sensors.index_finger_touch_data[0] == <u8>(122));
assert(value.right_touch_sensors.index_finger_touch_data[1] == <u8>(59));
assert(value.right_touch_sensors.index_finger_touch_data[2] == <u8>(248));
assert(value.right_touch_sensors.index_finger_touch_data[3] == <u8>(185));
assert(value.right_touch_sensors.index_finger_touch_data[4] == <u8>(126));
assert(value.right_touch_sensors.index_finger_touch_data[5] == <u8>(63));
assert(value.right_touch_sensors.index_finger_touch_data[6] == <u8>(252));
assert(value.right_touch_sensors.index_finger_touch_data[7] == <u8>(189));
assert(value.right_touch_sensors.index_finger_touch_data[8] == <u8>(114));
assert(value.right_touch_sensors.index_finger_touch_data[9] == <u8>(51));
assert(value.right_touch_sensors.index_finger_touch_data[10] == <u8>(25));
assert(value.right_touch_sensors.index_finger_touch_data[11] == <u8>(88));
assert(value.right_touch_sensors.index_finger_touch_data[12] == <u8>(155));
assert(value.right_touch_sensors.index_finger_touch_data[13] == <u8>(218));
assert(value.right_touch_sensors.index_finger_touch_data[14] == <u8>(29));
assert(value.right_touch_sensors.index_finger_touch_data[15] == <u8>(92));
assert(value.right_touch_sensors.middle_finger_touch_data.length == 16);
assert(value.right_touch_sensors.middle_finger_touch_data[0] == <u8>(181));
assert(value.right_touch_sensors.middle_finger_touch_data[1] == <u8>(244));
assert(value.right_touch_sensors.middle_finger_touch_data[2] == <u8>(55));
assert(value.right_touch_sensors.middle_finger_touch_data[3] == <u8>(118));
assert(value.right_touch_sensors.middle_finger_touch_data[4] == <u8>(177));
assert(value.right_touch_sensors.middle_finger_touch_data[5] == <u8>(240));
assert(value.right_touch_sensors.middle_finger_touch_data[6] == <u8>(51));
assert(value.right_touch_sensors.middle_finger_touch_data[7] == <u8>(114));
assert(value.right_touch_sensors.middle_finger_touch_data[8] == <u8>(189));
assert(value.right_touch_sensors.middle_finger_touch_data[9] == <u8>(252));
assert(value.right_touch_sensors.middle_finger_touch_data[10] == <u8>(173));
assert(value.right_touch_sensors.middle_finger_touch_data[11] == <u8>(236));
assert(value.right_touch_sensors.middle_finger_touch_data[12] == <u8>(47));
assert(value.right_touch_sensors.middle_finger_touch_data[13] == <u8>(110));
assert(value.right_touch_sensors.middle_finger_touch_data[14] == <u8>(169));
assert(value.right_touch_sensors.middle_finger_touch_data[15] == <u8>(232));
assert(value.right_touch_sensors.ring_finger_touch_data.length == 16);
assert(value.right_touch_sensors.ring_finger_touch_data[0] == <u8>(152));
assert(value.right_touch_sensors.ring_finger_touch_data[1] == <u8>(217));
assert(value.right_touch_sensors.ring_finger_touch_data[2] == <u8>(26));
assert(value.right_touch_sensors.ring_finger_touch_data[3] == <u8>(91));
assert(value.right_touch_sensors.ring_finger_touch_data[4] == <u8>(156));
assert(value.right_touch_sensors.ring_finger_touch_data[5] == <u8>(221));
assert(value.right_touch_sensors.ring_finger_touch_data[6] == <u8>(30));
assert(value.right_touch_sensors.ring_finger_touch_data[7] == <u8>(95));
assert(value.right_touch_sensors.ring_finger_touch_data[8] == <u8>(144));
assert(value.right_touch_sensors.ring_finger_touch_data[9] == <u8>(209));
assert(value.right_touch_sensors.ring_finger_touch_data[10] == <u8>(0));
assert(value.right_touch_sensors.ring_finger_touch_data[11] == <u8>(65));
assert(value.right_touch_sensors.ring_finger_touch_data[12] == <u8>(130));
assert(value.right_touch_sensors.ring_finger_touch_data[13] == <u8>(195));
assert(value.right_touch_sensors.ring_finger_touch_data[14] == <u8>(4));
assert(value.right_touch_sensors.ring_finger_touch_data[15] == <u8>(69));
assert(value.right_touch_sensors.little_finger_touch_data.length == 16);
assert(value.right_touch_sensors.little_finger_touch_data[0] == <u8>(167));
assert(value.right_touch_sensors.little_finger_touch_data[1] == <u8>(230));
assert(value.right_touch_sensors.little_finger_touch_data[2] == <u8>(37));
assert(value.right_touch_sensors.little_finger_touch_data[3] == <u8>(100));
assert(value.right_touch_sensors.little_finger_touch_data[4] == <u8>(163));
assert(value.right_touch_sensors.little_finger_touch_data[5] == <u8>(226));
assert(value.right_touch_sensors.little_finger_touch_data[6] == <u8>(33));
assert(value.right_touch_sensors.little_finger_touch_data[7] == <u8>(96));
assert(value.right_touch_sensors.little_finger_touch_data[8] == <u8>(175));
assert(value.right_touch_sensors.little_finger_touch_data[9] == <u8>(238));
assert(value.right_touch_sensors.little_finger_touch_data[10] == <u8>(166));
assert(value.right_touch_sensors.little_finger_touch_data[11] == <u8>(231));
assert(value.right_touch_sensors.little_finger_touch_data[12] == <u8>(36));
assert(value.right_touch_sensors.little_finger_touch_data[13] == <u8>(101));
assert(value.right_touch_sensors.little_finger_touch_data[14] == <u8>(162));
assert(value.right_touch_sensors.little_finger_touch_data[15] == <u8>(227));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_30():void{const input=bytes("1b5a99d81f5e9ddc1352f4b57637f0b17233fcbdadec2f6ea9e82b6aa5e49adb18599edfe1a06322e5a46726e9a891d0135295d4175699d8c8894a0bcc8d4e0fc081ffbe7d3cfbba5617d4955213d0915e1f19589bda1d5ca6e72465a2e32061aeefbdfc3f7eb9f8115093d2155497d61958fdbc7f3ef9b8c2834001c6874405ca8bdc9d5e1fd899034281c0074685c40b4af6b77435f2b3");const codec=new r.Ros2Aimdk_msgsMsgHandTouchSensorDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.palm_touch_data.length == 36);
assert(value.palm_touch_data[0] == <u8>(27));
assert(value.palm_touch_data[1] == <u8>(90));
assert(value.palm_touch_data[2] == <u8>(153));
assert(value.palm_touch_data[3] == <u8>(216));
assert(value.palm_touch_data[4] == <u8>(31));
assert(value.palm_touch_data[5] == <u8>(94));
assert(value.palm_touch_data[6] == <u8>(157));
assert(value.palm_touch_data[7] == <u8>(220));
assert(value.palm_touch_data[8] == <u8>(19));
assert(value.palm_touch_data[9] == <u8>(82));
assert(value.palm_touch_data[10] == <u8>(244));
assert(value.palm_touch_data[11] == <u8>(181));
assert(value.palm_touch_data[12] == <u8>(118));
assert(value.palm_touch_data[13] == <u8>(55));
assert(value.palm_touch_data[14] == <u8>(240));
assert(value.palm_touch_data[15] == <u8>(177));
assert(value.palm_touch_data[16] == <u8>(114));
assert(value.palm_touch_data[17] == <u8>(51));
assert(value.palm_touch_data[18] == <u8>(252));
assert(value.palm_touch_data[19] == <u8>(189));
assert(value.palm_touch_data[20] == <u8>(173));
assert(value.palm_touch_data[21] == <u8>(236));
assert(value.palm_touch_data[22] == <u8>(47));
assert(value.palm_touch_data[23] == <u8>(110));
assert(value.palm_touch_data[24] == <u8>(169));
assert(value.palm_touch_data[25] == <u8>(232));
assert(value.palm_touch_data[26] == <u8>(43));
assert(value.palm_touch_data[27] == <u8>(106));
assert(value.palm_touch_data[28] == <u8>(165));
assert(value.palm_touch_data[29] == <u8>(228));
assert(value.palm_touch_data[30] == <u8>(154));
assert(value.palm_touch_data[31] == <u8>(219));
assert(value.palm_touch_data[32] == <u8>(24));
assert(value.palm_touch_data[33] == <u8>(89));
assert(value.palm_touch_data[34] == <u8>(158));
assert(value.palm_touch_data[35] == <u8>(223));
assert(value.back_of_hand_touch_data.length == 36);
assert(value.back_of_hand_touch_data[0] == <u8>(225));
assert(value.back_of_hand_touch_data[1] == <u8>(160));
assert(value.back_of_hand_touch_data[2] == <u8>(99));
assert(value.back_of_hand_touch_data[3] == <u8>(34));
assert(value.back_of_hand_touch_data[4] == <u8>(229));
assert(value.back_of_hand_touch_data[5] == <u8>(164));
assert(value.back_of_hand_touch_data[6] == <u8>(103));
assert(value.back_of_hand_touch_data[7] == <u8>(38));
assert(value.back_of_hand_touch_data[8] == <u8>(233));
assert(value.back_of_hand_touch_data[9] == <u8>(168));
assert(value.back_of_hand_touch_data[10] == <u8>(145));
assert(value.back_of_hand_touch_data[11] == <u8>(208));
assert(value.back_of_hand_touch_data[12] == <u8>(19));
assert(value.back_of_hand_touch_data[13] == <u8>(82));
assert(value.back_of_hand_touch_data[14] == <u8>(149));
assert(value.back_of_hand_touch_data[15] == <u8>(212));
assert(value.back_of_hand_touch_data[16] == <u8>(23));
assert(value.back_of_hand_touch_data[17] == <u8>(86));
assert(value.back_of_hand_touch_data[18] == <u8>(153));
assert(value.back_of_hand_touch_data[19] == <u8>(216));
assert(value.back_of_hand_touch_data[20] == <u8>(200));
assert(value.back_of_hand_touch_data[21] == <u8>(137));
assert(value.back_of_hand_touch_data[22] == <u8>(74));
assert(value.back_of_hand_touch_data[23] == <u8>(11));
assert(value.back_of_hand_touch_data[24] == <u8>(204));
assert(value.back_of_hand_touch_data[25] == <u8>(141));
assert(value.back_of_hand_touch_data[26] == <u8>(78));
assert(value.back_of_hand_touch_data[27] == <u8>(15));
assert(value.back_of_hand_touch_data[28] == <u8>(192));
assert(value.back_of_hand_touch_data[29] == <u8>(129));
assert(value.back_of_hand_touch_data[30] == <u8>(255));
assert(value.back_of_hand_touch_data[31] == <u8>(190));
assert(value.back_of_hand_touch_data[32] == <u8>(125));
assert(value.back_of_hand_touch_data[33] == <u8>(60));
assert(value.back_of_hand_touch_data[34] == <u8>(251));
assert(value.back_of_hand_touch_data[35] == <u8>(186));
assert(value.thumb_touch_data.length == 16);
assert(value.thumb_touch_data[0] == <u8>(86));
assert(value.thumb_touch_data[1] == <u8>(23));
assert(value.thumb_touch_data[2] == <u8>(212));
assert(value.thumb_touch_data[3] == <u8>(149));
assert(value.thumb_touch_data[4] == <u8>(82));
assert(value.thumb_touch_data[5] == <u8>(19));
assert(value.thumb_touch_data[6] == <u8>(208));
assert(value.thumb_touch_data[7] == <u8>(145));
assert(value.thumb_touch_data[8] == <u8>(94));
assert(value.thumb_touch_data[9] == <u8>(31));
assert(value.thumb_touch_data[10] == <u8>(25));
assert(value.thumb_touch_data[11] == <u8>(88));
assert(value.thumb_touch_data[12] == <u8>(155));
assert(value.thumb_touch_data[13] == <u8>(218));
assert(value.thumb_touch_data[14] == <u8>(29));
assert(value.thumb_touch_data[15] == <u8>(92));
assert(value.index_finger_touch_data.length == 16);
assert(value.index_finger_touch_data[0] == <u8>(166));
assert(value.index_finger_touch_data[1] == <u8>(231));
assert(value.index_finger_touch_data[2] == <u8>(36));
assert(value.index_finger_touch_data[3] == <u8>(101));
assert(value.index_finger_touch_data[4] == <u8>(162));
assert(value.index_finger_touch_data[5] == <u8>(227));
assert(value.index_finger_touch_data[6] == <u8>(32));
assert(value.index_finger_touch_data[7] == <u8>(97));
assert(value.index_finger_touch_data[8] == <u8>(174));
assert(value.index_finger_touch_data[9] == <u8>(239));
assert(value.index_finger_touch_data[10] == <u8>(189));
assert(value.index_finger_touch_data[11] == <u8>(252));
assert(value.index_finger_touch_data[12] == <u8>(63));
assert(value.index_finger_touch_data[13] == <u8>(126));
assert(value.index_finger_touch_data[14] == <u8>(185));
assert(value.index_finger_touch_data[15] == <u8>(248));
assert(value.middle_finger_touch_data.length == 16);
assert(value.middle_finger_touch_data[0] == <u8>(17));
assert(value.middle_finger_touch_data[1] == <u8>(80));
assert(value.middle_finger_touch_data[2] == <u8>(147));
assert(value.middle_finger_touch_data[3] == <u8>(210));
assert(value.middle_finger_touch_data[4] == <u8>(21));
assert(value.middle_finger_touch_data[5] == <u8>(84));
assert(value.middle_finger_touch_data[6] == <u8>(151));
assert(value.middle_finger_touch_data[7] == <u8>(214));
assert(value.middle_finger_touch_data[8] == <u8>(25));
assert(value.middle_finger_touch_data[9] == <u8>(88));
assert(value.middle_finger_touch_data[10] == <u8>(253));
assert(value.middle_finger_touch_data[11] == <u8>(188));
assert(value.middle_finger_touch_data[12] == <u8>(127));
assert(value.middle_finger_touch_data[13] == <u8>(62));
assert(value.middle_finger_touch_data[14] == <u8>(249));
assert(value.middle_finger_touch_data[15] == <u8>(184));
assert(value.ring_finger_touch_data.length == 16);
assert(value.ring_finger_touch_data[0] == <u8>(194));
assert(value.ring_finger_touch_data[1] == <u8>(131));
assert(value.ring_finger_touch_data[2] == <u8>(64));
assert(value.ring_finger_touch_data[3] == <u8>(1));
assert(value.ring_finger_touch_data[4] == <u8>(198));
assert(value.ring_finger_touch_data[5] == <u8>(135));
assert(value.ring_finger_touch_data[6] == <u8>(68));
assert(value.ring_finger_touch_data[7] == <u8>(5));
assert(value.ring_finger_touch_data[8] == <u8>(202));
assert(value.ring_finger_touch_data[9] == <u8>(139));
assert(value.ring_finger_touch_data[10] == <u8>(220));
assert(value.ring_finger_touch_data[11] == <u8>(157));
assert(value.ring_finger_touch_data[12] == <u8>(94));
assert(value.ring_finger_touch_data[13] == <u8>(31));
assert(value.ring_finger_touch_data[14] == <u8>(216));
assert(value.ring_finger_touch_data[15] == <u8>(153));
assert(value.little_finger_touch_data.length == 16);
assert(value.little_finger_touch_data[0] == <u8>(3));
assert(value.little_finger_touch_data[1] == <u8>(66));
assert(value.little_finger_touch_data[2] == <u8>(129));
assert(value.little_finger_touch_data[3] == <u8>(192));
assert(value.little_finger_touch_data[4] == <u8>(7));
assert(value.little_finger_touch_data[5] == <u8>(70));
assert(value.little_finger_touch_data[6] == <u8>(133));
assert(value.little_finger_touch_data[7] == <u8>(196));
assert(value.little_finger_touch_data[8] == <u8>(11));
assert(value.little_finger_touch_data[9] == <u8>(74));
assert(value.little_finger_touch_data[10] == <u8>(246));
assert(value.little_finger_touch_data[11] == <u8>(183));
assert(value.little_finger_touch_data[12] == <u8>(116));
assert(value.little_finger_touch_data[13] == <u8>(53));
assert(value.little_finger_touch_data[14] == <u8>(242));
assert(value.little_finger_touch_data[15] == <u8>(179));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_31():void{const input=bytes("25");const codec=new r.Ros2Aimdk_msgsMsgHandTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(37));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_32():void{const input=bytes("ee69f48d5710c31329d778db90ae4f45010000002507983e9c7eafa001000000070000007638343531ceb201000000070000007637386362ceb2de016ceb711245136300");const codec=new r.Ros2Aimdk_msgsMsgHdsEventQueryCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.start_time == <u64><u64>0x13c310578df469ee);
assert(value.end_time == <u64><u64>0x454fae90db78d729);
assert(value.diag_codes.length == 1);
assert(value.diag_codes[0] == <u64><u64>0xa0af7e9c3e980725);
assert(value.alert_codes.length == 1);
assert(value.alert_codes[0] == "v8451β");
assert(value.alert_locations.length == 1);
assert(value.alert_locations[0] == "v78cbβ");
assert(value.state == <u8>(222));
assert(value.order_desc == true);
assert(value.offset == <u32>(309455724));
assert(value.limit == <u32>(6492997));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_33():void{const input=bytes("728c8c3bcbf5bba58508d42b3c71e3b5070000007638623330ceb2070000007636633432ceb25bb2adabe2cb9a35d53f066b6c4631f5aaa58bdc13dcbc422858bdfd91218a6301");const codec=new r.Ros2Aimdk_msgsMsgHdsEventRecordCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.id == <u64><u64>0xa5bbf5cb3b8c8c72);
assert(value.diag_code == <u64><u64>0xb5e3713c2bd40885);
assert(value.alert_code == "v8b30β");
assert(value.alert_location == "v6c42β");
assert(value.first_timestamp == <u64><u64>0x359acbe2abadb25b);
assert(value.last_timestamp == <u64><u64>0xf531466c6b063fd5);
assert(value.recover_timestamp == <u64><u64>0x42bcdc13dc8ba5aa);
assert(value.duration_ns == <u64><u64>0x638a2191fdbd5828);
assert(value.recovered == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_34():void{const input=bytes("070000007666396635ceb2000000000030644000000000006059c000000000006856400000000000b4654000000000003043c0");const codec=new r.Ros2Aimdk_msgsMsgJointCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vf9f5β");
assert(value.position == <f64>(161.5));
assert(value.velocity == <f64>(-101.5));
assert(value.effort == <f64>(89.625));
assert(value.stiffness == <f64>(173.625));
assert(value.damping == <f64>(-38.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_35():void{const input=bytes("c4e2ccfa36661122070000007636656530ceb2c5a6c503155c6d32fa7a343001000000070000007631646364ceb200000000002855c00000000000fc62400000000000886dc0000000000028644000000000004866c0");const codec=new r.Ros2Aimdk_msgsMsgJointCommandArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_36():void{const input=bytes("070000007634386539ceb200000000008041c00000000000bc64400000000000bc6540cce1");const codec=new r.Ros2Aimdk_msgsMsgJointStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "v48e9β");
assert(value.position == <f64>(-35.0));
assert(value.velocity == <f64>(165.875));
assert(value.effort == <f64>(173.875));
assert(value.error_code == <u16>(57804));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_37():void{const input=bytes("1952db78f8dfe310070000007637316134ceb293ba811c5be6a64ea09f3a2a7401000000070000007638333532ceb20000000000c855c000000000003867400000000000c038c02b6c");const codec=new r.Ros2Aimdk_msgsMsgJointStateArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(2027639321));
assert(value.header.stamp.nanosec == <u32>(283369464));
assert(value.header.frame_id == "v71a4β");
assert(value.header.sequence == <u32>(478263955));
assert(value.header.meas_stamp.sec == <i32>(1319560795));
assert(value.header.meas_stamp.nanosec == <u32>(708485024));
assert(value.state.value == <u8>(116));
assert(value.joints.length == 1);
assert(value.joints[0].name == "v8352β");
assert(value.joints[0].position == <f64>(-87.125));
assert(value.joints[0].velocity == <f64>(185.75));
assert(value.joints[0].effort == <f64>(-24.75));
assert(value.joints[0].error_code == <u16>(27691));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_38():void{const input=bytes("9e616d45");const codec=new r.Ros2Aimdk_msgsMsgMcActionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1164796318));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_39():void{const input=bytes("6ff83014070000007633656562ceb2");const codec=new r.Ros2Aimdk_msgsMsgMcActionCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.action.value == <i32>(338753647));
assert(value.action_desc == "v3eebβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_40():void{const input=bytes("091b656e070000007630343534ceb2543effca");const codec=new r.Ros2Aimdk_msgsMsgMcActionInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.current_action.value == <i32>(1852119817));
assert(value.action_desc == "v0454β");
assert(value.status.value == <i32>(-889242028));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_41():void{const input=bytes("03a0131b");const codec=new r.Ros2Aimdk_msgsMsgMcActionStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(454270979));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_42():void{const input=bytes("27721ee2");const codec=new r.Ros2Aimdk_msgsMsgMcBodyPoseStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(-501321177));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_43():void{const input=bytes("14a80faac0a00404070000007661386462ceb2b54afec558ee636607f4df0b070000007639326436ceb26bf1d6dbe430b897732edbdb070000007631383331ceb2f694337f00e5dc6e2081c878a1a048d9a33a0f7349070000007634623639ceb2b619cb49010000000000d85b4056e8035b00000000002853c00000000000c03dc000000000005044c0000000000098574000000000006469c0aed2017ad6019dd6d4bf53c90000000000e030400000000000d8584000000000008021400000000000e03d400000000000bc63c00000000000306c40");const codec=new r.Ros2Aimdk_msgsMsgMcCommonStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-1441814508));
assert(value.header.stamp.nanosec == <u32>(67412160));
assert(value.header.frame_id == "va8dbβ");
assert(value.header.sequence == <u32>(3321776821));
assert(value.header.meas_stamp.sec == <i32>(1717825112));
assert(value.header.meas_stamp.nanosec == <u32>(199226375));
assert(value.input_source.name == "v92d6β");
assert(value.input_source.priority == <i32>(-606670485));
assert(value.input_source.timeout == <i32>(-1749536540));
assert(value.action_info.current_action.value == <i32>(-606392717));
assert(value.action_info.action_desc == "v1831β");
assert(value.action_info.status.value == <i32>(2134086902));
assert(value.runtime_model.waist_status == false);
assert(value.runtime_model.left_hand_status.value == <i32>(544136421));
assert(value.runtime_model.right_hand_status.value == <i32>(-1585919871));
assert(value.motion_status.player_state.value == <u32>(2748926112));
assert(value.motion_status.control_area.value == <i32>(1232277306));
assert(value.motion_status.motion == "v4b69β");
assert(value.motion_status.ros_type.value == <i32>(1238047158));
assert(value.servo_status.squat_enable == true);
assert(value.servo_status.squat_offset == <f64>(111.375));
assert(value.servo_status.squat_limit == <u32>(1526982742));
assert(value.servo_status.head_yaw_offset == <f64>(-76.625));
assert(value.servo_status.head_pitch_offset == <f64>(-29.75));
assert(value.servo_status.waist_yaw_offset == <f64>(-40.625));
assert(value.servo_status.waist_pitch_offset == <f64>(94.375));
assert(value.servo_status.waist_roll_offset == <f64>(-203.125));
assert(value.fsm_state.current_state == <u32>(2046939822));
assert(value.body_status.value == <i32>(-694353450));
assert(value.speed_status.speed_mode.value == <i32>(-917258284));
assert(value.speed_status.forward_bounds.min_value == <f64>(16.875));
assert(value.speed_status.forward_bounds.max_value == <f64>(99.375));
assert(value.speed_status.lateral_bounds.min_value == <f64>(8.75));
assert(value.speed_status.lateral_bounds.max_value == <f64>(29.875));
assert(value.speed_status.angular_bounds.min_value == <f64>(-157.875));
assert(value.speed_status.angular_bounds.max_value == <f64>(225.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_44():void{const input=bytes("c61a340d");const codec=new r.Ros2Aimdk_msgsMsgMcControlAreaCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(221518534));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_45():void{const input=bytes("4721c93c");const codec=new r.Ros2Aimdk_msgsMsgMcFsmStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.current_state == <u32>(1019814215));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_46():void{const input=bytes("3d7cbd94");const codec=new r.Ros2Aimdk_msgsMsgMcHandStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(-1799521219));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_47():void{const input=bytes("64436340");const codec=new r.Ros2Aimdk_msgsMsgMcInputActionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1080247140));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_48():void{const input=bytes("070000007664636264ceb2d4e7a260518ce9e8");const codec=new r.Ros2Aimdk_msgsMsgMcInputSourceCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vdcbdβ");
assert(value.priority == <i32>(1621288916));
assert(value.timeout == <i32>(-387347375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_49():void{const input=bytes("0000000000f86a400000000000185d40");const codec=new r.Ros2Aimdk_msgsMsgMcLocomotionSpeedBoundsCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.min_value == <f64>(215.75));
assert(value.max_value == <f64>(116.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_50():void{const input=bytes("80a19b9b");const codec=new r.Ros2Aimdk_msgsMsgMcLocomotionSpeedModeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(-1684299392));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_51():void{const input=bytes("3a1991dc00000000000000400000000000dc61c00000000000f85ec000000000004057c000000000008039c000000000000065c0");const codec=new r.Ros2Aimdk_msgsMsgMcLocomotionSpeedStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.speed_mode.value == <i32>(-594470598));
assert(value.forward_bounds.min_value == <f64>(2.0));
assert(value.forward_bounds.max_value == <f64>(-142.875));
assert(value.lateral_bounds.min_value == <f64>(-123.875));
assert(value.lateral_bounds.max_value == <f64>(-93.0));
assert(value.angular_bounds.min_value == <f64>(-25.5));
assert(value.angular_bounds.max_value == <f64>(-168.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_52():void{const input=bytes("1da7c0703c8e991c070000007630373663ceb2de93496a976034d2db683806070000007630363634ceb200000000000c69c0000000000038634000000000003064c0");const codec=new r.Ros2Aimdk_msgsMsgMcLocomotionVelocityCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_53():void{const input=bytes("e7b049707df7e39a070000007665356665ceb2b1546c44");const codec=new r.Ros2Aimdk_msgsMsgMcMotionStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.player_state.value == <u32>(1883877607));
assert(value.control_area.value == <i32>(-1696336003));
assert(value.motion == "ve5feβ");
assert(value.ros_type.value == <i32>(1147950257));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_54():void{const input=bytes("3e8a2753");const codec=new r.Ros2Aimdk_msgsMsgMcMotionTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1395100222));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_55():void{const input=bytes("6b254e3a");const codec=new r.Ros2Aimdk_msgsMsgMcPlayerStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u32>(978199915));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_56():void{const input=bytes("bbd1e14c");const codec=new r.Ros2Aimdk_msgsMsgMcPresetMotionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1289867707));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_57():void{const input=bytes("003738887b9d9dfec9");const codec=new r.Ros2Aimdk_msgsMsgMcRuntimeModelCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.waist_status == false);
assert(value.left_hand_status.value == <i32>(2072524855));
assert(value.right_hand_status.value == <i32>(-906060387));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_58():void{const input=bytes("000000000000e04c40f401d22100000000004c68c000000000000011c000000000007461400000000000a03ec00000000000b46840");const codec=new r.Ros2Aimdk_msgsMsgMcServoStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.squat_enable == false);
assert(value.squat_offset == <f64>(57.75));
assert(value.squat_limit == <u32>(567411188));
assert(value.head_yaw_offset == <f64>(-194.375));
assert(value.head_pitch_offset == <f64>(-4.25));
assert(value.waist_yaw_offset == <f64>(139.625));
assert(value.waist_pitch_offset == <f64>(-30.625));
assert(value.waist_roll_offset == <f64>(197.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_59():void{const input=bytes("5455aec10663d513070000007636323737ceb2e5fe520f50fd2f14d8db8710");const codec=new r.Ros2Aimdk_msgsMsgMessageHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(-1045539500));
assert(value.stamp.nanosec == <u32>(332751622));
assert(value.frame_id == "v6277β");
assert(value.sequence == <u32>(257097445));
assert(value.meas_stamp.sec == <i32>(338689360));
assert(value.meas_stamp.nanosec == <u32>(277339096));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_60():void{const input=bytes("2c023ff50000000000c050c000000000000052c00000000000d04240");const codec=new r.Ros2Aimdk_msgsMsgNaviPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.point_id == <i32>(-180420052));
assert(value.navi_point.x == <f64>(-67.0));
assert(value.navi_point.y == <f64>(-72.0));
assert(value.navi_point.theta == <f64>(37.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_61():void{const input=bytes("3dc46582010000000000000000b85e400000000000785f400000000000a85bc0");const codec=new r.Ros2Aimdk_msgsMsgPathCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.path_id == <i32>(-2107259843));
assert(value.points.length == 1);
assert(value.points[0].x == <f64>(122.875));
assert(value.points[0].y == <f64>(125.875));
assert(value.points[0].theta == <f64>(-110.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_62():void{const input=bytes("070000007665383830ceb2519f2d8ce8e61a12070000007639356463ceb2ee0df25d5774c5c3070000007633343232ceb2d22b843a6b52b3a46a");const codec=new r.Ros2Aimdk_msgsMsgPilotStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.target_name == "ve880β");
assert(value.target_id == <u64><u64>0x121ae6e88c2d9f51);
assert(value.intro_task_name == "v95dcβ");
assert(value.intro_task_id == <u64><u64>0xc3c574575df20dee);
assert(value.map_name == "v3422β");
assert(value.map_id == <u64><u64>0xa4b3526b3a842bd2);
assert(value.pilot_state == <u8>(106));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_63():void{const input=bytes("070000007665613133ceb2c5");const codec=new r.Ros2Aimdk_msgsMsgPlayStateChangeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.pkg_name == "vea13β");
assert(value.state.value == <u8>(197));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_64():void{const input=bytes("28");const codec=new r.Ros2Aimdk_msgsMsgPlayStateTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(40));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_65():void{const input=bytes("070000007637643533ceb29905780d6a070000007634396232ceb2070000007632646662ceb201");const codec=new r.Ros2Aimdk_msgsMsgPlayTtsRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "v7d53β");
assert(value.priority_level.value == <u8>(153));
assert(value.priority_weight == <u32>(1779267589));
assert(value.domain == "v49b2β");
assert(value.trace_id == "v2dfbβ");
assert(value.is_interrupted == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_66():void{const input=bytes("070000007639376439ceb22d13a7109d070000007636333533ceb2070000007665306536ceb201070000007637663738ceb20f38fdc705");const codec=new r.Ros2Aimdk_msgsMsgPlayTtsResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.text == "v97d9β");
assert(value.priority_level.value == <u8>(45));
assert(value.priority_weight == <u32>(2635114259));
assert(value.domain == "v6353β");
assert(value.trace_id == "ve0e6β");
assert(value.is_success == true);
assert(value.error_message == "v7f78β");
assert(value.estimated_duration == <u32>(3355260943));
assert(value.error_code == <u8>(5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_67():void{const input=bytes("19f6b624712c1638070000007661663661ceb228be4fc22222aeda7f072f28070000007638383739ceb2d4e9028db251d254");const codec=new r.Ros2Aimdk_msgsMsgPmuLedStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(615970329));
assert(value.header.stamp.nanosec == <u32>(940977265));
assert(value.header.frame_id == "vaf6aβ");
assert(value.header.sequence == <u32>(3260005928));
assert(value.header.meas_stamp.sec == <i32>(-626122206));
assert(value.header.meas_stamp.nanosec == <u32>(674170751));
assert(value.trace_id == "v8879β");
assert(value.led_strip_mode == <u8>(212));
assert(value.r == <u8>(233));
assert(value.g == <u8>(2));
assert(value.b == <u8>(141));
assert(value.priority == <i32>(1423069618));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_68():void{const input=bytes("070000007664623661ceb2070000007634316663ceb2070000007663383133ceb2a9e007c500000000008c6640000000000048584000000000003859c000000000002c63c00000000000203a4000000000004050c000000000004060c00000000000806cc00000000000cc644000000000003858400000000000504a40000000000048614000000000009c6b400000000000485bc000000000003c68c00000000000e03d40000000000000ec6e40070000007665386262ceb2070000007631333430ceb2070000007636353838ceb2070000007666663165ceb2f957209c893c0000000000685140000000000070404000000000007053400000000000b857403de21c6435561304202a24");const codec=new r.Ros2Aimdk_msgsMsgPmuStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
assert(value.fan_speed == <f64>(29.875));
assert(value.fan_pecentage == <u8>(0));
assert(value.pmu_temperature == <f64>(247.375));
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

function test_69():void{const input=bytes("0bdd3785106e3a2f070000007666666432ceb289d2f792e03256e08552f400b2bb01cf7d532f760464b1f907131c407e24822291b0d4ee371e2e070000007661363965ceb20000000000002bc000000000004050400000000000c058c00000000000f05e4000000000007866400000000000185ec00000000000c06940010000000000000000a4654000000000009042400000000000a049400000000000b86fc000c04bc3010000000000000000e05a400000000000c04b400000000000e46bc001000000000000000070624000000000008036c000000000000042c00000000000005ec00000000000c030c00000000000e056c090738971290abeef3a41b3c08338845ea1e02362a5e42766a9e8d5945716d1905312dd9c8ccd0e4f88c90a4b84c5bbfa3978bffe3d7cb3f23e7fbcfd3a7bb8f9367709488bca0d4c8fce01405011d293");const codec=new r.Ros2Aimdk_msgsMsgPncTaskRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-2059936501));
assert(value.header.stamp.nanosec == <u32>(792358416));
assert(value.header.frame_id == "vffd2β");
assert(value.header.sequence == <u32>(2465714825));
assert(value.header.meas_stamp.sec == <i32>(-531221792));
assert(value.header.meas_stamp.nanosec == <u32>(16011909));
assert(value.task_type == <u8>(178));
assert(value.task_request == <u8>(187));
assert(value.pnc_mode == <u8>(1));
assert(value.task_id == <u64><u64>0xb16404762f537dcf);
assert(value.map_id == <u64><u64>0x82247e401c1307f9);
assert(value.target_pose.header.stamp.sec == <i32>(-726626014));
assert(value.target_pose.header.stamp.nanosec == <u32>(773732334));
assert(value.target_pose.header.frame_id == "va69eβ");
assert(value.target_pose.pose.position.x == <f64>(-13.5));
assert(value.target_pose.pose.position.y == <f64>(65.0));
assert(value.target_pose.pose.position.z == <f64>(-99.0));
assert(value.target_pose.pose.orientation.x == <f64>(123.75));
assert(value.target_pose.pose.orientation.y == <f64>(179.75));
assert(value.target_pose.pose.orientation.z == <f64>(-120.375));
assert(value.target_pose.pose.orientation.w == <f64>(206.0));
assert(value.reference_line.length == 1);
assert(value.reference_line[0].x == <f64>(173.125));
assert(value.reference_line[0].y == <f64>(37.125));
assert(value.reference_line[0].theta == <f64>(51.25));
assert(value.target_pose_radius == <f64>(-253.75));
assert(value.max_forward_speed == <f32>(-203.75));
assert(value.trajectory_pos.length == 1);
assert(value.trajectory_pos[0].x == <f64>(107.5));
assert(value.trajectory_pos[0].y == <f64>(55.5));
assert(value.trajectory_pos[0].theta == <f64>(-223.125));
assert(value.trajectory_vel.length == 1);
assert(value.trajectory_vel[0].linear.x == <f64>(147.5));
assert(value.trajectory_vel[0].linear.y == <f64>(-22.5));
assert(value.trajectory_vel[0].linear.z == <f64>(-36.0));
assert(value.trajectory_vel[0].angular.x == <f64>(-120.0));
assert(value.trajectory_vel[0].angular.y == <f64>(-16.75));
assert(value.trajectory_vel[0].angular.z == <f64>(-91.5));
assert(value.play_timestamp == <u64><u64>0xefbe0a2971897390);
assert(value.time_duration == <u64><u64>0x5e843883c0b3413a);
assert(value.reserve_info.length == 64);
assert(value.reserve_info[0] == <u8>(161));
assert(value.reserve_info[1] == <u8>(224));
assert(value.reserve_info[2] == <u8>(35));
assert(value.reserve_info[3] == <u8>(98));
assert(value.reserve_info[4] == <u8>(165));
assert(value.reserve_info[5] == <u8>(228));
assert(value.reserve_info[6] == <u8>(39));
assert(value.reserve_info[7] == <u8>(102));
assert(value.reserve_info[8] == <u8>(169));
assert(value.reserve_info[9] == <u8>(232));
assert(value.reserve_info[10] == <u8>(213));
assert(value.reserve_info[11] == <u8>(148));
assert(value.reserve_info[12] == <u8>(87));
assert(value.reserve_info[13] == <u8>(22));
assert(value.reserve_info[14] == <u8>(209));
assert(value.reserve_info[15] == <u8>(144));
assert(value.reserve_info[16] == <u8>(83));
assert(value.reserve_info[17] == <u8>(18));
assert(value.reserve_info[18] == <u8>(221));
assert(value.reserve_info[19] == <u8>(156));
assert(value.reserve_info[20] == <u8>(140));
assert(value.reserve_info[21] == <u8>(205));
assert(value.reserve_info[22] == <u8>(14));
assert(value.reserve_info[23] == <u8>(79));
assert(value.reserve_info[24] == <u8>(136));
assert(value.reserve_info[25] == <u8>(201));
assert(value.reserve_info[26] == <u8>(10));
assert(value.reserve_info[27] == <u8>(75));
assert(value.reserve_info[28] == <u8>(132));
assert(value.reserve_info[29] == <u8>(197));
assert(value.reserve_info[30] == <u8>(187));
assert(value.reserve_info[31] == <u8>(250));
assert(value.reserve_info[32] == <u8>(57));
assert(value.reserve_info[33] == <u8>(120));
assert(value.reserve_info[34] == <u8>(191));
assert(value.reserve_info[35] == <u8>(254));
assert(value.reserve_info[36] == <u8>(61));
assert(value.reserve_info[37] == <u8>(124));
assert(value.reserve_info[38] == <u8>(179));
assert(value.reserve_info[39] == <u8>(242));
assert(value.reserve_info[40] == <u8>(62));
assert(value.reserve_info[41] == <u8>(127));
assert(value.reserve_info[42] == <u8>(188));
assert(value.reserve_info[43] == <u8>(253));
assert(value.reserve_info[44] == <u8>(58));
assert(value.reserve_info[45] == <u8>(123));
assert(value.reserve_info[46] == <u8>(184));
assert(value.reserve_info[47] == <u8>(249));
assert(value.reserve_info[48] == <u8>(54));
assert(value.reserve_info[49] == <u8>(119));
assert(value.reserve_info[50] == <u8>(9));
assert(value.reserve_info[51] == <u8>(72));
assert(value.reserve_info[52] == <u8>(139));
assert(value.reserve_info[53] == <u8>(202));
assert(value.reserve_info[54] == <u8>(13));
assert(value.reserve_info[55] == <u8>(76));
assert(value.reserve_info[56] == <u8>(143));
assert(value.reserve_info[57] == <u8>(206));
assert(value.reserve_info[58] == <u8>(1));
assert(value.reserve_info[59] == <u8>(64));
assert(value.reserve_info[60] == <u8>(80));
assert(value.reserve_info[61] == <u8>(17));
assert(value.reserve_info[62] == <u8>(210));
assert(value.reserve_info[63] == <u8>(147));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_70():void{const input=bytes("c1505a0ee3a75d05070000007632636537ceb2d3f2c2418fb5b8c466eeff0321b21487080b6db09688d03bca31a90c5407fbba7938ffbe7d3cf3b21f5e9ddc1b5a99d817564607c4854203c0814e0f7130f3b27534f7b67938f4b57637f0b17233fcbdc3824100c7864504cb8a9adb1859");const codec=new r.Ros2Aimdk_msgsMsgPncTaskResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(240799937));
assert(value.header.stamp.nanosec == <u32>(90023907));
assert(value.header.frame_id == "v2ce7β");
assert(value.header.sequence == <u32>(1103295187));
assert(value.header.meas_stamp.sec == <i32>(-994527857));
assert(value.header.meas_stamp.nanosec == <u32>(67104358));
assert(value.task_type == <u8>(33));
assert(value.task_id == <u64><u64>0x96b06d0b088714b2);
assert(value.map_id == <u64><u64>0x540ca931ca3bd088);
assert(value.task_status == <u8>(7));
assert(value.reserve_info.length == 64);
assert(value.reserve_info[0] == <u8>(251));
assert(value.reserve_info[1] == <u8>(186));
assert(value.reserve_info[2] == <u8>(121));
assert(value.reserve_info[3] == <u8>(56));
assert(value.reserve_info[4] == <u8>(255));
assert(value.reserve_info[5] == <u8>(190));
assert(value.reserve_info[6] == <u8>(125));
assert(value.reserve_info[7] == <u8>(60));
assert(value.reserve_info[8] == <u8>(243));
assert(value.reserve_info[9] == <u8>(178));
assert(value.reserve_info[10] == <u8>(31));
assert(value.reserve_info[11] == <u8>(94));
assert(value.reserve_info[12] == <u8>(157));
assert(value.reserve_info[13] == <u8>(220));
assert(value.reserve_info[14] == <u8>(27));
assert(value.reserve_info[15] == <u8>(90));
assert(value.reserve_info[16] == <u8>(153));
assert(value.reserve_info[17] == <u8>(216));
assert(value.reserve_info[18] == <u8>(23));
assert(value.reserve_info[19] == <u8>(86));
assert(value.reserve_info[20] == <u8>(70));
assert(value.reserve_info[21] == <u8>(7));
assert(value.reserve_info[22] == <u8>(196));
assert(value.reserve_info[23] == <u8>(133));
assert(value.reserve_info[24] == <u8>(66));
assert(value.reserve_info[25] == <u8>(3));
assert(value.reserve_info[26] == <u8>(192));
assert(value.reserve_info[27] == <u8>(129));
assert(value.reserve_info[28] == <u8>(78));
assert(value.reserve_info[29] == <u8>(15));
assert(value.reserve_info[30] == <u8>(113));
assert(value.reserve_info[31] == <u8>(48));
assert(value.reserve_info[32] == <u8>(243));
assert(value.reserve_info[33] == <u8>(178));
assert(value.reserve_info[34] == <u8>(117));
assert(value.reserve_info[35] == <u8>(52));
assert(value.reserve_info[36] == <u8>(247));
assert(value.reserve_info[37] == <u8>(182));
assert(value.reserve_info[38] == <u8>(121));
assert(value.reserve_info[39] == <u8>(56));
assert(value.reserve_info[40] == <u8>(244));
assert(value.reserve_info[41] == <u8>(181));
assert(value.reserve_info[42] == <u8>(118));
assert(value.reserve_info[43] == <u8>(55));
assert(value.reserve_info[44] == <u8>(240));
assert(value.reserve_info[45] == <u8>(177));
assert(value.reserve_info[46] == <u8>(114));
assert(value.reserve_info[47] == <u8>(51));
assert(value.reserve_info[48] == <u8>(252));
assert(value.reserve_info[49] == <u8>(189));
assert(value.reserve_info[50] == <u8>(195));
assert(value.reserve_info[51] == <u8>(130));
assert(value.reserve_info[52] == <u8>(65));
assert(value.reserve_info[53] == <u8>(0));
assert(value.reserve_info[54] == <u8>(199));
assert(value.reserve_info[55] == <u8>(134));
assert(value.reserve_info[56] == <u8>(69));
assert(value.reserve_info[57] == <u8>(4));
assert(value.reserve_info[58] == <u8>(203));
assert(value.reserve_info[59] == <u8>(138));
assert(value.reserve_info[60] == <u8>(154));
assert(value.reserve_info[61] == <u8>(219));
assert(value.reserve_info[62] == <u8>(24));
assert(value.reserve_info[63] == <u8>(89));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_71():void{const input=bytes("8f4d62e8363455760a1bb4b1b362832f07b3443f5523ae12efca622352c7ef7debbed8e3");const codec=new r.Ros2Aimdk_msgsMsgPostProcSigCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.timestamp == <u64><u64>0x76553436e8624d8f);
assert(value.diag_code == <u64><u64>0x2f8362b3b1b41b0a);
assert(value.sig_set.robot_sig == <i32>(1061466887));
assert(value.sig_set.motion_sig == <u8>(85));
assert(value.sig_set.sound_sig == <u8>(35));
assert(value.sig_set.led_sig == <u8>(174));
assert(value.sig_set.facial_sig == <u8>(18));
assert(value.sig_set.reserve == <u32>(593677039));
assert(value.sig_set.funcdisable_sig == <u64><u64>0xe3d8beeb7defc752);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_72():void{const input=bytes("ce91d0cb3ce5a71e070000007631633164ceb26b433871601636d2a59c8d0801000000f340d9c44a39ee5a76160f9dcf6f3803ab7750d1528fba15fbb5365cbe65a373071c94ed");const codec=new r.Ros2Aimdk_msgsMsgPostProcSigArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-875523634));
assert(value.header.stamp.nanosec == <u32>(514319676));
assert(value.header.frame_id == "v1c1dβ");
assert(value.header.sequence == <u32>(1899512683));
assert(value.header.meas_stamp.sec == <i32>(-768207264));
assert(value.header.meas_stamp.nanosec == <u32>(143498405));
assert(value.post_proc_sig.length == 1);
assert(value.post_proc_sig[0].timestamp == <u64><u64>0x5aee394ac4d940f3);
assert(value.post_proc_sig[0].diag_code == <u64><u64>0x03386fcf9d0f1676);
assert(value.post_proc_sig[0].sig_set.robot_sig == <i32>(-783255637));
assert(value.post_proc_sig[0].sig_set.motion_sig == <u8>(82));
assert(value.post_proc_sig[0].sig_set.sound_sig == <u8>(143));
assert(value.post_proc_sig[0].sig_set.led_sig == <u8>(186));
assert(value.post_proc_sig[0].sig_set.facial_sig == <u8>(21));
assert(value.post_proc_sig[0].sig_set.reserve == <u32>(1547089403));
assert(value.post_proc_sig[0].sig_set.funcdisable_sig == <u64><u64>0xed941c0773a365be);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_73():void{const input=bytes("74f6ac96d49fda04070000007630383631ceb291fb4465427b4e7c7214af30d8042faf470100000002");const codec=new r.Ros2Aimdk_msgsMsgProcessedAudioOutputCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_74():void{const input=bytes("af80e4d20000000000dc62400000000000a464400000000000b04e400000000000d4624000000000000c664000000000005041400000000000801740");const codec=new r.Ros2Aimdk_msgsMsgQRCodeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.code == <i32>(-756776785));
assert(value.pose.position.x == <f64>(150.875));
assert(value.pose.position.y == <f64>(165.125));
assert(value.pose.position.z == <f64>(61.375));
assert(value.pose.orientation.x == <f64>(150.625));
assert(value.pose.orientation.y == <f64>(176.375));
assert(value.pose.orientation.z == <f64>(34.625));
assert(value.pose.orientation.w == <f64>(5.875));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_75():void{const input=bytes("18260ba054830b6f0000000000806c4000000000005864400000000000e46dc00000000000003a400000000000805c4000000000008037c000000000000467c0");const codec=new r.Ros2Aimdk_msgsMsgQRPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.point_id == <i32>(-1609882088));
assert(value.qr_code.code == <i32>(1863025492));
assert(value.qr_code.pose.position.x == <f64>(228.0));
assert(value.qr_code.pose.position.y == <f64>(162.75));
assert(value.qr_code.pose.position.z == <f64>(-239.125));
assert(value.qr_code.pose.orientation.x == <f64>(26.0));
assert(value.qr_code.pose.orientation.y == <f64>(114.0));
assert(value.qr_code.pose.orientation.z == <f64>(-23.5));
assert(value.qr_code.pose.orientation.w == <f64>(-184.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_76():void{const input=bytes("fc19070000007635653736ceb2010000000040ec42004069c3000088c2");const codec=new r.Ros2Aimdk_msgsMsgRegionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ros_type == <u8>(252));
assert(value.drawing_type == <u8>(25));
assert(value.name == "v5e76β");
assert(value.polygon.points.length == 1);
assert(value.polygon.points[0].x == <f32>(118.125));
assert(value.polygon.points[0].y == <f32>(-233.25));
assert(value.polygon.points[0].z == <f32>(-68.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_77():void{const input=bytes("7ec27b1a2f62b81e");const codec=new r.Ros2Aimdk_msgsMsgRequestHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(444318334));
assert(value.stamp.nanosec == <u32>(515400239));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_78():void{const input=bytes("55c42dd4c7edd33a863659073f4f6e99");const codec=new r.Ros2Aimdk_msgsMsgResponseHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(-735198123));
assert(value.stamp.nanosec == <u32>(986967495));
assert(value.code == <i64><u64>0x996e4f3f07593686);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_79():void{const input=bytes("070000007636353030ceb2070000007631343664ceb2070000007662323335ceb201000000070000007638623339ceb20ef96a45b7805ddb");const codec=new r.Ros2Aimdk_msgsMsgRobotResourceCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.resource_key == "v6500β");
assert(value.current_version.version == "v146dβ");
assert(value.current_version.name == "vb235β");
assert(value.current_version.files.length == 1);
assert(value.current_version.files[0] == "v8b39β");
assert(value.current_version.download_timestamp_seconds == <u64><u64>0xdb5d80b7456af90e);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_80():void{const input=bytes("f19cfb5453d5cd148c7850e68eff26fc37861162");const codec=new r.Ros2Aimdk_msgsMsgSigSetCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.robot_sig == <i32>(1425775857));
assert(value.motion_sig == <u8>(83));
assert(value.sound_sig == <u8>(213));
assert(value.led_sig == <u8>(205));
assert(value.facial_sig == <u8>(20));
assert(value.reserve == <u32>(3864033420));
assert(value.funcdisable_sig == <u64><u64>0x62118637fc26ff8e);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_81():void{const input=bytes("070000007665663837ceb2");const codec=new r.Ros2Aimdk_msgsMsgSlaveDeviceCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ip == "vef87β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_82():void{const input=bytes("a2ac09e5dd4ef813070000007637313264ceb216ae081cc3abea0569c8d025070000007639663561ceb289");const codec=new r.Ros2Aimdk_msgsMsgSmSystemStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-452350814));
assert(value.header.stamp.nanosec == <u32>(335040221));
assert(value.header.frame_id == "v712dβ");
assert(value.header.sequence == <u32>(470330902));
assert(value.header.meas_stamp.sec == <i32>(99265475));
assert(value.header.meas_stamp.nanosec == <u32>(634439785));
assert(value.cur_state == "v9f5aβ");
assert(value.cur_status.value == <u8>(137));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_83():void{const input=bytes("46");const codec=new r.Ros2Aimdk_msgsMsgSystemStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(70));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_84():void{const input=bytes("ffe0dbc7383a647e410dfa070000007638373239ceb28acb671333b2508d070000007665363461ceb2070000007636393838ceb26e76baafd70f8d31070000007664316462ceb26c6d7f90d514480ef0061e5d61974bc9102e32fe8e070000007665376666ceb21d6d199ea4142e00070000007662663539ceb26acfe372d3b6d4ec070000007662373339ceb2e0bc4b7459c57cea67070000007664643464ceb2");const codec=new r.Ros2Aimdk_msgsMsgTaskMasterStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.main_state == <u8>(255));
assert(value.sub_state == <u8>(224));
assert(value.slam_state == <u8>(219));
assert(value.loc_map_id == <u64><u64>0xfa0d417e643a38c7);
assert(value.loc_map_name == "v8729β");
assert(value.fix_line_intro_state.set_task_id == <u64><u64>0x8d50b2331367cb8a);
assert(value.fix_line_intro_state.set_task_name == "ve64aβ");
assert(value.fix_line_intro_state.task_name == "v6988β");
assert(value.fix_line_intro_state.task_id == <u64><u64>0x318d0fd7afba766e);
assert(value.fix_line_intro_state.map_name == "vd1dbβ");
assert(value.fix_line_intro_state.map_id == <u64><u64>0x0e4814d5907f6d6c);
assert(value.fix_line_intro_state.rest_site_number == <u32>(1562248944));
assert(value.fix_line_intro_state.fix_line_intro_state == <u8>(97));
assert(value.fix_line_intro_state.cur_target_id == <u64><u64>0x8efe322e10c94b97);
assert(value.pilot_state.target_name == "ve7ffβ");
assert(value.pilot_state.target_id == <u64><u64>0x002e14a49e196d1d);
assert(value.pilot_state.intro_task_name == "vbf59β");
assert(value.pilot_state.intro_task_id == <u64><u64>0xecd4b6d372e3cf6a);
assert(value.pilot_state.map_name == "vb739β");
assert(value.pilot_state.map_id == <u64><u64>0xea7cc559744bbce0);
assert(value.pilot_state.pilot_state == <u8>(103));
assert(value.reserve_info == "vdd4dβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_85():void{const input=bytes("d7825fed79ab2507070000007634613365ceb25bf61b277bc4ddd4778e3f220152585ca1136947b8d03a6a93910b718a569d30c517ac2bdcd4ff06f795ce1deed3bcbd4e928da65751de8b7c10ef9065d779d12a9648ca33551be718142afc010100010001000100");const codec=new r.Ros2Aimdk_msgsMsgTouchStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_86():void{const input=bytes("bc");const codec=new r.Ros2Aimdk_msgsMsgTtsPriorityLevelCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(188));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_87():void{const input=bytes("56e2e6e5c42ccb21070000007639653633ceb2165a46f3cde52b61de547427070000007665353834ceb2a20000000000c865c00000000000a857c00000000000506a4000000000006469400000000000e049c00000000000985e400000000000946c400000000000a04e4000000000002043c000000000002038c000000000004c61400000000000206d400000000000a851c0000000000090554000000000008854400000000000205b40010000000000000000005040");const codec=new r.Ros2Aimdk_msgsMsgUpperBodyCommandArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-437853610));
assert(value.header.stamp.nanosec == <u32>(566963396));
assert(value.header.frame_id == "v9e63β");
assert(value.header.sequence == <u32>(4081474070));
assert(value.header.meas_stamp.sec == <i32>(1630266829));
assert(value.header.meas_stamp.nanosec == <u32>(661935326));
assert(value.source == "ve584β");
assert(value.hand_sub_mode == <u8>(162));
assert(value.head_pos.length == 2);
assert(value.head_pos[0] == <f64>(-174.25));
assert(value.head_pos[1] == <f64>(-94.625));
assert(value.arm_pos.length == 14);
assert(value.arm_pos[0] == <f64>(210.5));
assert(value.arm_pos[1] == <f64>(203.125));
assert(value.arm_pos[2] == <f64>(-51.75));
assert(value.arm_pos[3] == <f64>(122.375));
assert(value.arm_pos[4] == <f64>(228.625));
assert(value.arm_pos[5] == <f64>(61.25));
assert(value.arm_pos[6] == <f64>(-38.25));
assert(value.arm_pos[7] == <f64>(-24.125));
assert(value.arm_pos[8] == <f64>(138.375));
assert(value.arm_pos[9] == <f64>(233.0));
assert(value.arm_pos[10] == <f64>(-70.625));
assert(value.arm_pos[11] == <f64>(86.25));
assert(value.arm_pos[12] == <f64>(82.125));
assert(value.arm_pos[13] == <f64>(108.5));
assert(value.hand_pos.length == 1);
assert(value.hand_pos[0] == <f64>(64.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_88():void{const input=bytes("133fd9fc54a0e534070000007633636262ceb207e60bcef7dc061a");const codec=new r.Ros2Aimdk_msgsSrvAbandonAudioFocus_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-52871405));
assert(value.request.header.stamp.nanosec == <u32>(887464020));
assert(value.focus_requester.pkg_name == "v3cbbβ");
assert(value.focus_requester.priority == <u32>(3456886279));
assert(value.focus_requester.priority_weight == <u32>(436657399));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_89():void{const input=bytes("8162941f12b61c3ba79126fb1ee81165ee50407d070000007666376633ceb2070000007639306138ceb200");const codec=new r.Ros2Aimdk_msgsSrvAbandonAudioFocus_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(529818241));
assert(value.reponse.header.stamp.nanosec == <u32>(991737362));
assert(value.reponse.header.code == <i64><u64>0x6511e81efb2691a7);
assert(value.reponse.status.value == <i32>(2101367022));
assert(value.reponse.message == "vf7f3β");
assert(value.focus_response.pkg_name == "v90a8β");
assert(value.focus_response.focus_gain == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_90():void{const input=bytes("937dc53e00b1bf39070000007632616638ceb2070000007630353138ceb201000000070000007632323066ceb2070000007663636232ceb2");const codec=new r.Ros2Aimdk_msgsSrvExecuteActionResource_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(1053130131));
assert(value.header.header.stamp.nanosec == <u32>(968864000));
assert(value.resource_key == "v2af8β");
assert(value.resource_version == "v0518β");
assert(value.slaves.length == 1);
assert(value.slaves[0].ip == "v220fβ");
assert(value.meta == "vccb2β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_91():void{const input=bytes("c1864d406010982eebe25df0529b6a6e26262102070000007631343639ceb2");const codec=new r.Ros2Aimdk_msgsSrvExecuteActionResource_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(1078822593));
assert(value.header.header.stamp.nanosec == <u32>(781717600));
assert(value.header.header.code == <i64><u64>0x6e6a9b52f05de2eb);
assert(value.header.status.value == <i32>(35726886));
assert(value.header.message == "v1469β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_92():void{const input=bytes("42176be292645b22");const codec=new r.Ros2Aimdk_msgsSrvGetAllJointState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-496298174));
assert(value.request.header.stamp.nanosec == <u32>(576414866));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_93():void{const input=bytes("cbb1e60319f8d70d158016abacf921355fb27f58070000007635353831ceb201000000070000007630323635ceb20000000000205ac000000000008018c00000000000905040cae401000000070000007634366630ceb20000000000e060c000000000001859400000000000c466c0acbf01000000070000007665366135ceb200000000002064c000000000001054c00000000000204640bede01000000070000007637363262ceb20000000000a8544000000000004057400000000000d05e400889");const codec=new r.Ros2Aimdk_msgsSrvGetAllJointState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
assert(value.head_joints[0].error_code == <u16>(58570));
assert(value.arm_joints.length == 1);
assert(value.arm_joints[0].name == "v46f0β");
assert(value.arm_joints[0].position == <f64>(-135.0));
assert(value.arm_joints[0].velocity == <f64>(100.375));
assert(value.arm_joints[0].effort == <f64>(-182.125));
assert(value.arm_joints[0].error_code == <u16>(49068));
assert(value.waist_joints.length == 1);
assert(value.waist_joints[0].name == "ve6a5β");
assert(value.waist_joints[0].position == <f64>(-161.0));
assert(value.waist_joints[0].velocity == <f64>(-80.25));
assert(value.waist_joints[0].effort == <f64>(44.25));
assert(value.waist_joints[0].error_code == <u16>(57022));
assert(value.leg_joints.length == 1);
assert(value.leg_joints[0].name == "v762bβ");
assert(value.leg_joints[0].position == <f64>(82.625));
assert(value.leg_joints[0].velocity == <f64>(93.0));
assert(value.leg_joints[0].effort == <f64>(123.25));
assert(value.leg_joints[0].error_code == <u16>(35080));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_94():void{const input=bytes("356cda9cc4940907");const codec=new r.Ros2Aimdk_msgsSrvGetCurrentInputSource_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-1663407051));
assert(value.request.header.stamp.nanosec == <u32>(118068420));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_95():void{const input=bytes("8f9e7c10e0656a2587abc65d3ed2f1c340b51964f9cc2efa08534872070000007666656432ceb2d91539cd3bc79314");const codec=new r.Ros2Aimdk_msgsSrvGetCurrentInputSource_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_96():void{const input=bytes("216fa6ac032ce70e");const codec=new r.Ros2Aimdk_msgsSrvGetHandType_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-1398378719));
assert(value.request.header.stamp.nanosec == <u32>(250031107));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_97():void{const input=bytes("514c13d76fc2151cd7889bf26ef1ac6ccb9c4c2d070000007638303934ceb2a04f");const codec=new r.Ros2Aimdk_msgsSrvGetHandType_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_98():void{const input=bytes("7f463db47a7a8520bd5dd99a0424ee04a8ffdbee1186ec7001000000593897aee041a03001000000070000007664643732ceb201000000070000007639366263ceb21f00b386c5fe84eafcd7");const codec=new r.Ros2Aimdk_msgsSrvGetHdsEventHistory_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-1271052673));
assert(value.header.stamp.nanosec == <u32>(545618554));
assert(value.query.start_time == <u64><u64>0x04ee24049ad95dbd);
assert(value.query.end_time == <u64><u64>0x70ec8611eedbffa8);
assert(value.query.diag_codes.length == 1);
assert(value.query.diag_codes[0] == <u64><u64>0x30a041e0ae973859);
assert(value.query.alert_codes.length == 1);
assert(value.query.alert_codes[0] == "vdd72β");
assert(value.query.alert_locations.length == 1);
assert(value.query.alert_locations[0] == "v96bcβ");
assert(value.query.state == <u8>(31));
assert(value.query.order_desc == false);
assert(value.query.offset == <u32>(4274357939));
assert(value.query.limit == <u32>(3623676548));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_99():void{const input=bytes("7fef03592da7f7154d048518f47db286010000001aa44fc2a3dd785c7a46a92dc33f9eb3070000007661363334ceb2070000007664373965ceb24cfb7235f58245abd28bdad06bf2ed4ed74608d66e3f3f48b0cffdff09b6ca6100ce2c3ade01");const codec=new r.Ros2Aimdk_msgsSrvGetHdsEventHistory_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1493430143));
assert(value.header.stamp.nanosec == <u32>(368551725));
assert(value.header.code == <i64><u64>0x86b27df41885044d);
assert(value.records.length == 1);
assert(value.records[0].id == <u64><u64>0x5c78dda3c24fa41a);
assert(value.records[0].diag_code == <u64><u64>0xb39e3fc32da9467a);
assert(value.records[0].alert_code == "va634β");
assert(value.records[0].alert_location == "vd79eβ");
assert(value.records[0].first_timestamp == <u64><u64>0xab4582f53572fb4c);
assert(value.records[0].last_timestamp == <u64><u64>0x4eedf26bd0da8bd2);
assert(value.records[0].recover_timestamp == <u64><u64>0x483f3f6ed60846d7);
assert(value.records[0].duration_ns == <u64><u64>0x61cab609fffdcfb0);
assert(value.records[0].recovered == false);
assert(value.total_count == <u32>(3728354510));
assert(value.has_more == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_100():void{const input=bytes("16b2cbc742412923");const codec=new r.Ros2Aimdk_msgsSrvGetMcAction_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-942951914));
assert(value.request.header.stamp.nanosec == <u32>(589906242));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_101():void{const input=bytes("371ceb02ee57961fe9cb311a50b20684cb955833070000007630626164ceb2d172243e");const codec=new r.Ros2Aimdk_msgsSrvGetMcAction_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(48962615));
assert(value.header.stamp.nanosec == <u32>(529946606));
assert(value.header.code == <i64><u64>0x8406b2501a31cbe9);
assert(value.info.current_action.value == <i32>(861443531));
assert(value.info.action_desc == "v0badβ");
assert(value.info.status.value == <i32>(1042576081));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_102():void{const input=bytes("2d06ddaa1c62ff36");const codec=new r.Ros2Aimdk_msgsSrvGetMcLocomotionSpeedStatus_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-1428355539));
assert(value.request.header.stamp.nanosec == <u32>(922706460));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_103():void{const input=bytes("01bcfd124020a724239029f19ae91e6f1991baac0000000000e863c00000000000c04e400000000000e039400000000000d8674000000000008054c00000000000603cc03201709f000000000090654000000000002860c000000000005c64c00000000000a46dc00000000000d063400000000000f859c04ba31be70000000000cc6ac00000000000b064400000000000b86b400000000000804bc00000000000004cc00000000000e04c40e92930700000000000d05a400000000000c040c000000000004041400000000000d0464000000000005464c00000000000f866404fb9dba20000000000000c400000000000c857400000000000c02dc0000000000090414000000000008012c00000000000f856409e4b1aec000000000080304000000000002c65c000000000002461c00000000000f059c00000000000085a400000000000e86d40");const codec=new r.Ros2Aimdk_msgsSrvGetMcLocomotionSpeedStatus_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(318618625));
assert(value.header.stamp.nanosec == <u32>(614932544));
assert(value.header.code == <i64><u64>0x6f1ee99af1299023);
assert(value.current_speed_status.speed_mode.value == <i32>(-1397059303));
assert(value.current_speed_status.forward_bounds.min_value == <f64>(-159.25));
assert(value.current_speed_status.forward_bounds.max_value == <f64>(61.5));
assert(value.current_speed_status.lateral_bounds.min_value == <f64>(25.875));
assert(value.current_speed_status.lateral_bounds.max_value == <f64>(190.75));
assert(value.current_speed_status.angular_bounds.min_value == <f64>(-82.0));
assert(value.current_speed_status.angular_bounds.max_value == <f64>(-28.375));
assert(value.none_speed_status.speed_mode.value == <i32>(-1620049614));
assert(value.none_speed_status.forward_bounds.min_value == <f64>(172.5));
assert(value.none_speed_status.forward_bounds.max_value == <f64>(-129.25));
assert(value.none_speed_status.lateral_bounds.min_value == <f64>(-162.875));
assert(value.none_speed_status.lateral_bounds.max_value == <f64>(-237.125));
assert(value.none_speed_status.angular_bounds.min_value == <f64>(158.5));
assert(value.none_speed_status.angular_bounds.max_value == <f64>(-103.875));
assert(value.low_speed_status.speed_mode.value == <i32>(-417619125));
assert(value.low_speed_status.forward_bounds.min_value == <f64>(-214.375));
assert(value.low_speed_status.forward_bounds.max_value == <f64>(165.5));
assert(value.low_speed_status.lateral_bounds.min_value == <f64>(221.75));
assert(value.low_speed_status.lateral_bounds.max_value == <f64>(-55.0));
assert(value.low_speed_status.angular_bounds.min_value == <f64>(-56.0));
assert(value.low_speed_status.angular_bounds.max_value == <f64>(57.75));
assert(value.medium_speed_status.speed_mode.value == <i32>(1882204649));
assert(value.medium_speed_status.forward_bounds.min_value == <f64>(107.25));
assert(value.medium_speed_status.forward_bounds.max_value == <f64>(-33.5));
assert(value.medium_speed_status.lateral_bounds.min_value == <f64>(34.5));
assert(value.medium_speed_status.lateral_bounds.max_value == <f64>(45.625));
assert(value.medium_speed_status.angular_bounds.min_value == <f64>(-162.625));
assert(value.medium_speed_status.angular_bounds.max_value == <f64>(183.75));
assert(value.high_speed_status.speed_mode.value == <i32>(-1562658481));
assert(value.high_speed_status.forward_bounds.min_value == <f64>(3.5));
assert(value.high_speed_status.forward_bounds.max_value == <f64>(95.125));
assert(value.high_speed_status.lateral_bounds.min_value == <f64>(-14.875));
assert(value.high_speed_status.lateral_bounds.max_value == <f64>(35.125));
assert(value.high_speed_status.angular_bounds.min_value == <f64>(-4.625));
assert(value.high_speed_status.angular_bounds.max_value == <f64>(91.875));
assert(value.step_speed_status.speed_mode.value == <i32>(-333821026));
assert(value.step_speed_status.forward_bounds.min_value == <f64>(16.5));
assert(value.step_speed_status.forward_bounds.max_value == <f64>(-169.375));
assert(value.step_speed_status.lateral_bounds.min_value == <f64>(-137.125));
assert(value.step_speed_status.lateral_bounds.max_value == <f64>(-103.75));
assert(value.step_speed_status.angular_bounds.min_value == <f64>(104.125));
assert(value.step_speed_status.angular_bounds.max_value == <f64>(239.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_104():void{const input=bytes("bd17846bc49ea11b3ccc227885b515e6");const codec=new r.Ros2Aimdk_msgsSrvGetMcPresetMotionState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(1803818941));
assert(value.request.header.stamp.nanosec == <u32>(463576772));
assert(value.request.task_id == <u64><u64>0xe615b5857822cc3c);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_105():void{const input=bytes("26d3f1244fe1870fbb08ff030271c89da41d25061d64129834f0712c");const codec=new r.Ros2Aimdk_msgsSrvGetMcPresetMotionState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.header.stamp.sec == <i32>(619828006));
assert(value.response.header.stamp.nanosec == <u32>(260563279));
assert(value.response.header.code == <i64><u64>0x9dc8710203ff08bb);
assert(value.response.task_id == <u64><u64>0x9812641d06251da4);
assert(value.response.state.value == <i32>(745664564));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_106():void{const input=bytes("86376c09145e3d31");const codec=new r.Ros2Aimdk_msgsSrvGetMicSourceRequest_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(158087046));
assert(value.header.header.stamp.nanosec == <u32>(826105364));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_107():void{const input=bytes("60cba72d1064fa0c804d0c9e39343b0059cf2fd8070000007632373833ceb2ea22645e");const codec=new r.Ros2Aimdk_msgsSrvGetMicSourceRequest_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(765971296));
assert(value.header.header.stamp.nanosec == <u32>(217736208));
assert(value.header.header.code == <i64><u64>0x003b34399e0c4d80);
assert(value.header.status.value == <i32>(-667955367));
assert(value.header.message == "v2783β");
assert(value.mic_source == <u32>(1583620842));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_108():void{const input=bytes("40cabdd47aeddb2e");const codec=new r.Ros2Aimdk_msgsSrvGetMute_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-725759424));
assert(value.request.header.stamp.nanosec == <u32>(786165114));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_109():void{const input=bytes("3a06deedcacac334f4fabbbf4d838c21cbcd668f070000007630326338ceb201");const codec=new r.Ros2Aimdk_msgsSrvGetMute_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-304216518));
assert(value.reponse.header.stamp.nanosec == <u32>(885246666));
assert(value.reponse.header.code == <i64><u64>0x218c834dbfbbfaf4);
assert(value.reponse.status.value == <i32>(-1889088053));
assert(value.reponse.message == "v02c8β");
assert(value.is_mute == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_110():void{const input=bytes("eebded370f49c415");const codec=new r.Ros2Aimdk_msgsSrvGetRobotResources_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(938327534));
assert(value.header.header.stamp.nanosec == <u32>(365185295));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_111():void{const input=bytes("80a3f06e09c25e35210b423a987275a461125a79070000007637653831ceb201000000070000007661363138ceb2070000007638653337ceb2070000007631356533ceb201000000070000007636306164ceb2482ff1a8f156c636");const codec=new r.Ros2Aimdk_msgsSrvGetRobotResources_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(1861264256));
assert(value.header.header.stamp.nanosec == <u32>(895402505));
assert(value.header.header.code == <i64><u64>0xa47572983a420b21);
assert(value.header.status.value == <i32>(2035946081));
assert(value.header.message == "v7e81β");
assert(value.robot_resources.length == 1);
assert(value.robot_resources[0].resource_key == "va618β");
assert(value.robot_resources[0].current_version.version == "v8e37β");
assert(value.robot_resources[0].current_version.name == "v15e3β");
assert(value.robot_resources[0].current_version.files.length == 1);
assert(value.robot_resources[0].current_version.files[0] == "v60adβ");
assert(value.robot_resources[0].current_version.download_timestamp_seconds == <u64><u64>0x36c656f1a8f12f48);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_112():void{const input=bytes("1c6b470b45985818070000007633313862ceb2070000007666376238ceb2");const codec=new r.Ros2Aimdk_msgsSrvGetStoredMapByName_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(189229852));
assert(value.header.stamp.nanosec == <u32>(408459333));
assert(value.header.frame_id == "v318bβ");
assert(value.map_name == "vf7b8β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_113():void{const input=bytes("d9024b13607b7c8db0a5078da2eed02b070000007661323937ceb2161a82c1e352ff0000206fc30094b4038776135e0000000000304bc000000000000463c000000000002c6ac00000000000a85cc00000000000c465c0000000000058574000000000000026c0c83ff7417146c0df070000007634653336ceb2010000005501000000d95412150000000000e033400000000000d85ac00000000000c050c0010000002ee1f528cd63f74700000000004850400000000000a467400000000000485c4000000000000037c000000000001055400000000000b06b400000000000802640010000002a7939d90000000000d85b400000000000b46c400000000000b856400000000000e0374000000000002854400000000000f869c000000000002037c0010000000cbc470f00000000002044400000000000c050c00000000000105cc000000000004850c00000000000b857c0000000000060694000000000005067c00001000000d9e7b0bd0100000000000000000050c00000000000c058400000000000203c4001000000944f070000007661386238ceb2010000000000d141008074c200403343192f99f0a056ae6e");const codec=new r.Ros2Aimdk_msgsSrvGetStoredMapByName_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.code == <u64><u64>0x8d7c7b60134b02d9);
assert(value.header.stamp.sec == <i32>(-1928878672));
assert(value.header.stamp.nanosec == <u32>(735112866));
assert(value.header.frame_id == "va297β");
assert(value.map_info.map_load_time.sec == <i32>(-1048438250));
assert(value.map_info.map_load_time.nanosec == <u32>(16732899));
assert(value.map_info.resolution == <f32>(-239.125));
assert(value.map_info.width == <u32>(62166016));
assert(value.map_info.height == <u32>(1578333831));
assert(value.map_info.origin.position.x == <f64>(-54.375));
assert(value.map_info.origin.position.y == <f64>(-152.125));
assert(value.map_info.origin.position.z == <f64>(-209.375));
assert(value.map_info.origin.orientation.x == <f64>(-114.625));
assert(value.map_info.origin.orientation.y == <f64>(-174.125));
assert(value.map_info.origin.orientation.z == <f64>(93.375));
assert(value.map_info.origin.orientation.w == <f64>(-11.0));
assert(value.map_version == <u64><u64>0xdfc0467141f73fc8);
assert(value.map_path == "v4e36β");
assert(value.data.length == 1);
assert(value.data[0] == <i8>(85));
assert(value.navi_points.length == 1);
assert(value.navi_points[0].point_id == <i32>(353522905));
assert(value.navi_points[0].navi_point.x == <f64>(19.875));
assert(value.navi_points[0].navi_point.y == <f64>(-107.375));
assert(value.navi_points[0].navi_point.theta == <f64>(-67.0));
assert(value.qr_points.length == 1);
assert(value.qr_points[0].point_id == <i32>(687202606));
assert(value.qr_points[0].qr_code.code == <i32>(1207395277));
assert(value.qr_points[0].qr_code.pose.position.x == <f64>(65.125));
assert(value.qr_points[0].qr_code.pose.position.y == <f64>(189.125));
assert(value.qr_points[0].qr_code.pose.position.z == <f64>(113.125));
assert(value.qr_points[0].qr_code.pose.orientation.x == <f64>(-23.0));
assert(value.qr_points[0].qr_code.pose.orientation.y == <f64>(84.25));
assert(value.qr_points[0].qr_code.pose.orientation.z == <f64>(221.5));
assert(value.qr_points[0].qr_code.pose.orientation.w == <f64>(11.25));
assert(value.hr_points.length == 1);
assert(value.hr_points[0].point_id == <i32>(-650544854));
assert(value.hr_points[0].pose.position.x == <f64>(111.375));
assert(value.hr_points[0].pose.position.y == <f64>(229.625));
assert(value.hr_points[0].pose.position.z == <f64>(90.875));
assert(value.hr_points[0].pose.orientation.x == <f64>(23.875));
assert(value.hr_points[0].pose.orientation.y == <f64>(80.625));
assert(value.hr_points[0].pose.orientation.z == <f64>(-207.75));
assert(value.hr_points[0].pose.orientation.w == <f64>(-23.125));
assert(value.hr_points[0].qr_list.length == 1);
assert(value.hr_points[0].qr_list[0].code == <i32>(256359436));
assert(value.hr_points[0].qr_list[0].pose.position.x == <f64>(40.25));
assert(value.hr_points[0].qr_list[0].pose.position.y == <f64>(-67.0));
assert(value.hr_points[0].qr_list[0].pose.position.z == <f64>(-112.25));
assert(value.hr_points[0].qr_list[0].pose.orientation.x == <f64>(-65.125));
assert(value.hr_points[0].qr_list[0].pose.orientation.y == <f64>(-94.875));
assert(value.hr_points[0].qr_list[0].pose.orientation.z == <f64>(203.0));
assert(value.hr_points[0].qr_list[0].pose.orientation.w == <f64>(-186.5));
assert(value.hr_points[0].use_head == false);
assert(value.paths.length == 1);
assert(value.paths[0].path_id == <i32>(-1112479783));
assert(value.paths[0].points.length == 1);
assert(value.paths[0].points[0].x == <f64>(-64.0));
assert(value.paths[0].points[0].y == <f64>(99.0));
assert(value.paths[0].points[0].theta == <f64>(28.125));
assert(value.regions.length == 1);
assert(value.regions[0].ros_type == <u8>(148));
assert(value.regions[0].drawing_type == <u8>(79));
assert(value.regions[0].name == "va8b8β");
assert(value.regions[0].polygon.points.length == 1);
assert(value.regions[0].polygon.points[0].x == <f32>(26.125));
assert(value.regions[0].polygon.points[0].y == <f32>(-61.125));
assert(value.regions[0].polygon.points[0].z == <f32>(179.25));
assert(value.map_id == <u64><u64>0x6eae56a0f0992f19);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_114():void{const input=bytes("3a5c948150ab5c15");const codec=new r.Ros2Aimdk_msgsSrvGetSystemState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-2120983494));
assert(value.header.header.stamp.nanosec == <u32>(358394704));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_115():void{const input=bytes("acccf8ef2c09871b64cb6112ddb2568cbe84c47f070000007635643437ceb2070000007639336630ceb2ae");const codec=new r.Ros2Aimdk_msgsSrvGetSystemState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-268907348));
assert(value.header.header.stamp.nanosec == <u32>(461834540));
assert(value.header.header.code == <i64><u64>0x8c56b2dd1261cb64);
assert(value.header.status.value == <i32>(2143585470));
assert(value.header.message == "v5d47β");
assert(value.cur_state == "v93f0β");
assert(value.curr_status.value == <u8>(174));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_116():void{const input=bytes("3482754f43b7261d");const codec=new r.Ros2Aimdk_msgsSrvGetVolume_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(1333101108));
assert(value.request.header.stamp.nanosec == <u32>(489076547));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_117():void{const input=bytes("577b2dba3e2c9f05138b111baaf226856110acb1070000007638363464ceb2a22c91c9");const codec=new r.Ros2Aimdk_msgsSrvGetVolume_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-1171424425));
assert(value.reponse.header.stamp.nanosec == <u32>(94317630));
assert(value.reponse.header.code == <i64><u64>0x8526f2aa1b118b13);
assert(value.reponse.status.value == <i32>(-1314123679));
assert(value.reponse.message == "v864dβ");
assert(value.audio_volume == <u32>(3381734562));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_118():void{const input=bytes("aad844f82f8d150001000000070000007633323736ceb2");const codec=new r.Ros2Aimdk_msgsSrvGroupControlTimeSync_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-129705814));
assert(value.header.header.stamp.nanosec == <u32>(1412399));
assert(value.slaves.length == 1);
assert(value.slaves[0].ip == "v3276β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_119():void{const input=bytes("6c8f8e1fdf961e0e0091e335b9e8d4aba5a33c35070000007632633238ceb2ab007411070000007630663831ceb2");const codec=new r.Ros2Aimdk_msgsSrvGroupControlTimeSync_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(529436524));
assert(value.header.header.stamp.nanosec == <u32>(236885727));
assert(value.header.header.code == <i64><u64>0xabd4e8b935e39100);
assert(value.header.status.value == <i32>(893166501));
assert(value.header.message == "v2c28β");
assert(value.error_code == <i32>(292815019));
assert(value.error_msg == "v0f81β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_120():void{const input=bytes("986bc7be61089b29070000007631636235ceb2");const codec=new r.Ros2Aimdk_msgsSrvMigrateSystemState_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1094227048));
assert(value.header.header.stamp.nanosec == <u32>(698026081));
assert(value.state == "v1cb5β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_121():void{const input=bytes("5f5d1fd7173d3328413fcc53f846fbcd0dfd8134070000007663613535ceb2");const codec=new r.Ros2Aimdk_msgsSrvMigrateSystemState_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-685810337));
assert(value.header.header.stamp.nanosec == <u32>(674446615));
assert(value.header.header.code == <i64><u64>0xcdfb46f853cc3f41);
assert(value.header.status.value == <i32>(880934157));
assert(value.header.message == "vca55β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_122():void{const input=bytes("1dbc0108acada711070000007634613535ceb2070000007665386230ceb2070000007662646337ceb20088d997715e7a7655070000007636613064ceb2070000007666623165ceb281c4e5b8fc211462744e38b2b3c32611");const codec=new r.Ros2Aimdk_msgsSrvPlayAudioFile_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(134331421));
assert(value.request.header.stamp.nanosec == <u32>(296201644));
assert(value.file.pkg_name == "v4a55β");
assert(value.file.file_name == "ve8b0β");
assert(value.file.file_path == "vbdc7β");
assert(value.file.info.channels == <u8>(0));
assert(value.file.info.sample_rate == <u32>(1905777032));
assert(value.file.info.size == <u32>(1433827934));
assert(value.file.info.sample_format == "v6a0dβ");
assert(value.file.info.coding_format == "vfb1eβ");
assert(value.file.priority == <u32>(3102065793));
assert(value.file.priority_weight == <u32>(1645486588));
assert(value.play_stamps.sec == <i32>(-1304932748));
assert(value.play_stamps.nanosec == <u32>(287753139));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_123():void{const input=bytes("0597d8f897bd541193b1d2062ac8e5987b50095c070000007661363963ceb2");const codec=new r.Ros2Aimdk_msgsSrvPlayAudioFile_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-120023291));
assert(value.reponse.header.stamp.nanosec == <u32>(290766231));
assert(value.reponse.header.code == <i64><u64>0x98e5c82a06d2b193);
assert(value.reponse.status.value == <i32>(1544114299));
assert(value.reponse.message == "va69cβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_124():void{const input=bytes("d540d899759aad1c0100000027ab0a195a00");const codec=new r.Ros2Aimdk_msgsSrvPlayEmojiGroup_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1713880875));
assert(value.header.header.stamp.nanosec == <u32>(481139317));
assert(value.emotion_ids.length == 1);
assert(value.emotion_ids[0] == <u8>(39));
assert(value.mode == <u8>(171));
assert(value.priority == <i32>(5904650));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_125():void{const input=bytes("597f55dff89f510e5f332835e64a1fab6224e8ce070000007666626430ceb201070000007637326336ceb2");const codec=new r.Ros2Aimdk_msgsSrvPlayEmojiGroup_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-548044967));
assert(value.header.header.stamp.nanosec == <u32>(240230392));
assert(value.header.header.code == <i64><u64>0xab1f4ae63528335f);
assert(value.header.status.value == <i32>(-823647134));
assert(value.header.message == "vfbd0β");
assert(value.success == true);
assert(value.message == "v72c6β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_126():void{const input=bytes("fb206e27ed8a9b1302cbe66ca555");const codec=new r.Ros2Aimdk_msgsSrvPlayEmoji_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(661528827));
assert(value.header.header.stamp.nanosec == <u32>(328960749));
assert(value.emotion_id == <u8>(2));
assert(value.mode == <u8>(203));
assert(value.priority == <i32>(1436904678));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_127():void{const input=bytes("f6c43d030ac08f040abdee5cb3c4d9c2974780d5070000007635323838ceb201070000007632373339ceb2");const codec=new r.Ros2Aimdk_msgsSrvPlayEmoji_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_128():void{const input=bytes("16fddc490ed7d11b070000007639383961ceb213d9d1d86c070000007637343130ceb2070000007634343863ceb200");const codec=new r.Ros2Aimdk_msgsSrvPlayTts_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_129():void{const input=bytes("eee8e8dd779080066fe0aca5d6999b3b1d90a1e8070000007636303330ceb2070000007661363062ceb2fad6e30077070000007662653934ceb2070000007639336335ceb201070000007663366266ceb22bf63bd9a3");const codec=new r.Ros2Aimdk_msgsSrvPlayTts_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
assert(value.tts_resp.error_code == <u8>(163));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_130():void{const input=bytes("1dffbbb536b15e1d01000000070000007662393735ceb2f19e4bf33a");const codec=new r.Ros2Aimdk_msgsSrvPlayVideoGroup_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1245970659));
assert(value.header.header.stamp.nanosec == <u32>(492745014));
assert(value.video_path_list.length == 1);
assert(value.video_path_list[0] == "vb975β");
assert(value.mode == <u8>(241));
assert(value.priority == <i32>(989023134));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_131():void{const input=bytes("6456c64af906b92efe9c0dbd47e53a23b392b16f070000007634336230ceb201070000007634383666ceb2");const codec=new r.Ros2Aimdk_msgsSrvPlayVideoGroup_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(1254512228));
assert(value.header.header.stamp.nanosec == <u32>(783877881));
assert(value.header.header.code == <i64><u64>0x233ae547bd0d9cfe);
assert(value.header.status.value == <i32>(1873908403));
assert(value.header.message == "v43b0β");
assert(value.success == true);
assert(value.message == "v486fβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_132():void{const input=bytes("23679b0a535df030070000007639653337ceb2d417bc5be7");const codec=new r.Ros2Aimdk_msgsSrvPlayVideo_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(177956643));
assert(value.header.header.stamp.nanosec == <u32>(821058899));
assert(value.video_path == "v9e37β");
assert(value.mode == <u8>(212));
assert(value.priority == <i32>(-413418473));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_133():void{const input=bytes("576b188bc2210c1c3d435581843a621f6659e06d070000007663326330ceb200070000007639356337ceb2");const codec=new r.Ros2Aimdk_msgsSrvPlayVideo_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_134():void{const input=bytes("18d88cdd3ffa1b12070000007631646565ceb20c015eefd72b3dfb");const codec=new r.Ros2Aimdk_msgsSrvRequestAudioFocus_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-577972200));
assert(value.request.header.stamp.nanosec == <u32>(303823423));
assert(value.focus_requester.pkg_name == "v1deeβ");
assert(value.focus_requester.priority == <u32>(4015915276));
assert(value.focus_requester.priority_weight == <u32>(4215090135));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_135():void{const input=bytes("eeee67883aa9bb2ee91665a3506f523dfe7f7cec070000007665353835ceb2070000007662316664ceb201");const codec=new r.Ros2Aimdk_msgsSrvRequestAudioFocus_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-2006454546));
assert(value.reponse.header.stamp.nanosec == <u32>(784050490));
assert(value.reponse.header.code == <i64><u64>0x3d526f50a36516e9);
assert(value.reponse.status.value == <i32>(-327385090));
assert(value.reponse.message == "ve585β");
assert(value.focus_response.pkg_name == "vb1fdβ");
assert(value.focus_response.focus_gain == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_136():void{const input=bytes("38249d9b00ba66140100000035070000007635636336ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetAgentPropertiesRequest_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1684200392));
assert(value.header.header.stamp.nanosec == <u32>(342276608));
assert(value.contents.properties.length == 1);
assert(value.contents.properties[0].key.value == <u8>(53));
assert(value.contents.properties[0].value == "v5cc6β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_137():void{const input=bytes("f8a4ec016be9aa2e2f5836a996210137354b719e070000007661393561ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetAgentPropertiesRequest_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(32285944));
assert(value.header.header.stamp.nanosec == <u32>(782952811));
assert(value.header.header.code == <i64><u64>0x37012196a936582f);
assert(value.header.status.value == <i32>(-1636742347));
assert(value.header.message == "va95aβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_138():void{const input=bytes("7d1b67589f966f04070000007638616439ceb2bba8a149070000007663376130ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetMcAction_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1483152253));
assert(value.header.stamp.nanosec == <u32>(74421919));
assert(value.source == "v8ad9β");
assert(value.command.action.value == <i32>(1235331259));
assert(value.command.action_desc == "vc7a0β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_139():void{const input=bytes("afba180a3b3980022cc8591f95b16e811f75ce07070000007664656439ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetMcAction_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.header.stamp.sec == <i32>(169392815));
assert(value.response.header.stamp.nanosec == <u32>(41957691));
assert(value.response.header.code == <i64><u64>0x816eb1951f59c82c);
assert(value.response.status.value == <i32>(130970911));
assert(value.response.message == "vded9β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_140():void{const input=bytes("e1dc1ed564ff8c3919eb0151070000007630386639ceb220845fcc7a3873a9");const codec=new r.Ros2Aimdk_msgsSrvSetMcInputSource_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-719397663));
assert(value.request.header.stamp.nanosec == <u32>(965541732));
assert(value.action.value == <i32>(1359080217));
assert(value.input_source.name == "v08f9β");
assert(value.input_source.priority == <i32>(-866155488));
assert(value.input_source.timeout == <i32>(-1452066694));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_141():void{const input=bytes("dc64493ed322d40688507bf331294c6d83b456d53acd614b07a8f5dc");const codec=new r.Ros2Aimdk_msgsSrvSetMcInputSource_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.header.stamp.sec == <i32>(1044997340));
assert(value.response.header.stamp.nanosec == <u32>(114565843));
assert(value.response.header.code == <i64><u64>0x6d4c2931f37b5088);
assert(value.response.task_id == <u64><u64>0x4b61cd3ad556b483);
assert(value.response.state.value == <i32>(-587880441));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_142():void{const input=bytes("dfe0f0a55c978a25070000007639653666ceb25170b3227865e64b17dd2cabccdb867500070000007632633961ceb23d5331ae842a0630");const codec=new r.Ros2Aimdk_msgsSrvSetMcPresetMotion_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-1510940449));
assert(value.header.stamp.nanosec == <u32>(629839708));
assert(value.input_source.name == "v9e6fβ");
assert(value.input_source.priority == <i32>(582185041));
assert(value.input_source.timeout == <i32>(1273390456));
assert(value.area.value == <i32>(-1423123177));
assert(value.motion.value == <i32>(1971772364));
assert(value.interrupt == false);
assert(value.ani_path == "v2c9aβ");
assert(value.play_timestamp == <u64><u64>0x30062a84ae31533d);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_143():void{const input=bytes("f5dbfa9364cdbe0af9a4971d40dda08331ebc0438892f7dd765c1932");const codec=new r.Ros2Aimdk_msgsSrvSetMcPresetMotion_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.response.header.stamp.sec == <i32>(-1812276235));
assert(value.response.header.stamp.nanosec == <u32>(180276580));
assert(value.response.header.code == <i64><u64>0x83a0dd401d97a4f9);
assert(value.response.task_id == <u64><u64>0xddf7928843c0eb31);
assert(value.response.state.value == <i32>(840522870));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_144():void{const input=bytes("03d1e38f9ef4252bb61b1c57");const codec=new r.Ros2Aimdk_msgsSrvSetMicSourceRequest_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1880895229));
assert(value.header.header.stamp.nanosec == <u32>(723907742));
assert(value.mic_source == <u32>(1461459894));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_145():void{const input=bytes("2933f3b066ec323bed3978ea54404f74c8a658eb070000007637316262ceb2");const codec=new r.Ros2Aimdk_msgsSrvSetMicSourceRequest_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.header.stamp.sec == <i32>(-1326238935));
assert(value.header.header.stamp.nanosec == <u32>(993193062));
assert(value.header.header.code == <i64><u64>0x744f4054ea7839ed);
assert(value.header.status.value == <i32>(-346511672));
assert(value.header.message == "v71bbβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_146():void{const input=bytes("b2a0f11c2b0f5a1701");const codec=new r.Ros2Aimdk_msgsSrvSetMute_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(485597362));
assert(value.request.header.stamp.nanosec == <u32>(391778091));
assert(value.is_mute == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_147():void{const input=bytes("60d9a5be72edec0c49f2bc65f08b8bfbf2056c3a070000007634653835ceb201");const codec=new r.Ros2Aimdk_msgsSrvSetMute_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-1096427168));
assert(value.reponse.header.stamp.nanosec == <u32>(216853874));
assert(value.reponse.header.code == <i64><u64>0xfb8b8bf065bcf249);
assert(value.reponse.status.value == <i32>(980157938));
assert(value.reponse.message == "v4e85β");
assert(value.is_mute == true);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_148():void{const input=bytes("2d79f205650b472f070000007634613962ceb2d302e9666102309600");const codec=new r.Ros2Aimdk_msgsSrvSetPmuLed_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(99776813));
assert(value.request.header.stamp.nanosec == <u32>(793185125));
assert(value.trace_id == "v4a9bβ");
assert(value.led_strip_mode == <u8>(211));
assert(value.r == <u8>(2));
assert(value.g == <u8>(233));
assert(value.b == <u8>(102));
assert(value.priority == <i32>(-1775238559));
assert(value.reset_priority == false);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_149():void{const input=bytes("fa217ec38fc9392b97f462242e8d55ba4893");const codec=new r.Ros2Aimdk_msgsSrvSetPmuLed_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-1015143942));
assert(value.header.stamp.nanosec == <u32>(725207439));
assert(value.header.code == <i64><u64>0xba558d2e2462f497);
assert(value.status_code == <u16>(37704));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_150():void{const input=bytes("014198c4c0f25f164b1e246a");const codec=new r.Ros2Aimdk_msgsSrvSetVolume_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(-996654847));
assert(value.request.header.stamp.nanosec == <u32>(375386816));
assert(value.audio_volume == <u32>(1780751947));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_151():void{const input=bytes("b75215ec53a04b19d309a1446a7096da5362972a070000007665623661ceb2836cf7ed");const codec=new r.Ros2Aimdk_msgsSrvSetVolume_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(-334146889));
assert(value.reponse.header.stamp.nanosec == <u32>(424386643));
assert(value.reponse.header.code == <i64><u64>0xda96706a44a109d3);
assert(value.reponse.status.value == <i32>(714564179));
assert(value.reponse.message == "veb6aβ");
assert(value.audio_volume == <u32>(3992415363));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_152():void{const input=bytes("ccfce94f472b7b18");const codec=new r.Ros2Aimdk_msgsSrvStopAudioPlay_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.request.header.stamp.sec == <i32>(1340734668));
assert(value.request.header.stamp.nanosec == <u32>(410725191));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_153():void{const input=bytes("079d4b0999a9f10c8fe59c3b369caba560423548070000007631376538ceb2");const codec=new r.Ros2Aimdk_msgsSrvStopAudioPlay_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.reponse.header.stamp.sec == <i32>(155950343));
assert(value.reponse.header.stamp.nanosec == <u32>(217164185));
assert(value.reponse.header.code == <i64><u64>0xa5ab9c363b9ce58f);
assert(value.reponse.status.value == <i32>(1211449952));
assert(value.reponse.message == "v17e8β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_154():void{const input=bytes("072b89b550057009");const codec=new r.Ros2Builtin_interfacesMsgTimeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.sec == <i32>(-1249301753));
assert(value.nanosec == <u32>(158336336));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_155():void{const input=bytes("0000000000e86dc00000000000d054c000000000002042c0");const codec=new r.Ros2Geometry_msgsMsgPointCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(-239.25));
assert(value.y == <f64>(-83.25));
assert(value.z == <f64>(-36.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_156():void{const input=bytes("0000a041000086420000a841");const codec=new r.Ros2Geometry_msgsMsgPoint32Codec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f32>(20.0));
assert(value.y == <f32>(67.0));
assert(value.z == <f32>(21.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_157():void{const input=bytes("01000000008065c3008048c300a04b43");const codec=new r.Ros2Geometry_msgsMsgPolygonCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.points.length == 1);
assert(value.points[0].x == <f32>(-229.5));
assert(value.points[0].y == <f32>(-200.5));
assert(value.points[0].z == <f32>(203.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_158():void{const input=bytes("0000000000c02ec000000000003047c00000000000203d4000000000004857c000000000007049400000000000986dc00000000000585340");const codec=new r.Ros2Geometry_msgsMsgPoseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_159():void{const input=bytes("00000000008014c00000000000e03e400000000000b46440");const codec=new r.Ros2Geometry_msgsMsgPose2DCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(-5.125));
assert(value.y == <f64>(30.875));
assert(value.theta == <f64>(165.625));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_160():void{const input=bytes("72e71f7fe898242b070000007664386435ceb200000000003854c000000000006c66c00000000000f8654000000000001869c00000000000b06dc00000000000d053c00000000000e85dc0");const codec=new r.Ros2Geometry_msgsMsgPoseStampedCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_161():void{const input=bytes("0000000000a85cc00000000000046dc00000000000d045c00000000000005340");const codec=new r.Ros2Geometry_msgsMsgQuaternionCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(-114.625));
assert(value.y == <f64>(-232.125));
assert(value.z == <f64>(-43.625));
assert(value.w == <f64>(76.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_162():void{const input=bytes("0000000000e053c00000000000f063c00000000000306cc00000000000a06d4000000000000c60c00000000000906840");const codec=new r.Ros2Geometry_msgsMsgTwistCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.linear.x == <f64>(-79.5));
assert(value.linear.y == <f64>(-159.5));
assert(value.linear.z == <f64>(-225.5));
assert(value.angular.x == <f64>(237.0));
assert(value.angular.y == <f64>(-128.375));
assert(value.angular.z == <f64>(196.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_163():void{const input=bytes("0000000000546c400000000000e062c00000000000246a40");const codec=new r.Ros2Geometry_msgsMsgVector3Codec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(226.625));
assert(value.y == <f64>(-151.0));
assert(value.z == <f64>(209.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_164():void{const input=bytes("69e506ca03882d02002076c39e36eabd66961a4900000000000c66c000000000000068400000000000a048400000000000405f400000000000604ec00000000000c04a400000000000d05440");const codec=new r.Ros2Nav_msgsMsgMapMetaDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.map_load_time.sec == <i32>(-905517719));
assert(value.map_load_time.nanosec == <u32>(36538371));
assert(value.resolution == <f32>(-246.125));
assert(value.width == <u32>(3186243230));
assert(value.height == <u32>(1226479206));
assert(value.origin.position.x == <f64>(-176.375));
assert(value.origin.position.y == <f64>(192.0));
assert(value.origin.position.z == <f64>(49.25));
assert(value.origin.orientation.x == <f64>(125.0));
assert(value.origin.orientation.y == <f64>(-60.75));
assert(value.origin.orientation.z == <f64>(53.5));
assert(value.origin.orientation.w == <f64>(83.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_165():void{const input=bytes("681252e4acc1ba37070000007666316532ceb2");const codec=new r.Ros2Std_msgsMsgHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
}
