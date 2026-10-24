/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../sdk/origin/custom-types/deep-robotics-ros2-main-complete/bounded_records";
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

function test_1():void{const input=bytes("929ea15d2be796c39d76d7f9127a8b310100000069bba56cc8ed0a7c0f65ccdbccfb3305dcf234f0b115470100000000802bc23a7bb8f93e7fbcfd32732061a2e32465a6e728697938fbba7d3cffbe71304e0f");const codec=new r.Ros2DrddsMsgBatteryDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.frame_id == <u64><u64>0xc396e72b5da19e92);
assert(value.header.stamp.sec == <i32>(-103319907));
assert(value.header.stamp.nanosec == <u32>(831224338));
assert(value.data.length == 1);
assert(value.data[0].voltage == <u16>(47977));
assert(value.data[0].current == <i16>(27813));
assert(value.data[0].remaining_capacity == <u16>(60872));
assert(value.data[0].nominal_capacity == <u16>(31754));
assert(value.data[0].cycles == <u16>(25871));
assert(value.data[0].production_date == <u16>(56268));
assert(value.data[0].balanced_low == <u16>(64460));
assert(value.data[0].balanced_high == <u16>(1331));
assert(value.data[0].protected_state == <u16>(62172));
assert(value.data[0].software_version == <u8>(52));
assert(value.data[0].battery_level == <u8>(240));
assert(value.data[0].mos_state == <u8>(177));
assert(value.data[0].battery_quantity == <u8>(21));
assert(value.data[0].battery_ntc == <u8>(71));
assert(value.data[0].battery_temperature.length == 1);
assert(value.data[0].battery_temperature[0] == <f32>(-42.875));
assert(value.data[0].battery_serialnum.length == 32);
assert(value.data[0].battery_serialnum[0] == <u8>(58));
assert(value.data[0].battery_serialnum[1] == <u8>(123));
assert(value.data[0].battery_serialnum[2] == <u8>(184));
assert(value.data[0].battery_serialnum[3] == <u8>(249));
assert(value.data[0].battery_serialnum[4] == <u8>(62));
assert(value.data[0].battery_serialnum[5] == <u8>(127));
assert(value.data[0].battery_serialnum[6] == <u8>(188));
assert(value.data[0].battery_serialnum[7] == <u8>(253));
assert(value.data[0].battery_serialnum[8] == <u8>(50));
assert(value.data[0].battery_serialnum[9] == <u8>(115));
assert(value.data[0].battery_serialnum[10] == <u8>(32));
assert(value.data[0].battery_serialnum[11] == <u8>(97));
assert(value.data[0].battery_serialnum[12] == <u8>(162));
assert(value.data[0].battery_serialnum[13] == <u8>(227));
assert(value.data[0].battery_serialnum[14] == <u8>(36));
assert(value.data[0].battery_serialnum[15] == <u8>(101));
assert(value.data[0].battery_serialnum[16] == <u8>(166));
assert(value.data[0].battery_serialnum[17] == <u8>(231));
assert(value.data[0].battery_serialnum[18] == <u8>(40));
assert(value.data[0].battery_serialnum[19] == <u8>(105));
assert(value.data[0].battery_serialnum[20] == <u8>(121));
assert(value.data[0].battery_serialnum[21] == <u8>(56));
assert(value.data[0].battery_serialnum[22] == <u8>(251));
assert(value.data[0].battery_serialnum[23] == <u8>(186));
assert(value.data[0].battery_serialnum[24] == <u8>(125));
assert(value.data[0].battery_serialnum[25] == <u8>(60));
assert(value.data[0].battery_serialnum[26] == <u8>(255));
assert(value.data[0].battery_serialnum[27] == <u8>(190));
assert(value.data[0].battery_serialnum[28] == <u8>(113));
assert(value.data[0].battery_serialnum[29] == <u8>(48));
assert(value.data[0].battery_serialnum[30] == <u8>(78));
assert(value.data[0].battery_serialnum[31] == <u8>(15));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("2a3ae6eda69b5d4560efe476ef6bd1bbf45f63126b4229010000000000b0c0e5a46726e1a06322edac5d1cdf9e5918db9a5514044586c7004182c30c4d3372");const codec=new r.Ros2DrddsMsgBatteryDataValueCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.voltage == <u16>(14890));
assert(value.current == <i16>(-4634));
assert(value.remaining_capacity == <u16>(39846));
assert(value.nominal_capacity == <u16>(17757));
assert(value.cycles == <u16>(61280));
assert(value.production_date == <u16>(30436));
assert(value.balanced_low == <u16>(27631));
assert(value.balanced_high == <u16>(48081));
assert(value.protected_state == <u16>(24564));
assert(value.software_version == <u8>(99));
assert(value.battery_level == <u8>(18));
assert(value.mos_state == <u8>(107));
assert(value.battery_quantity == <u8>(66));
assert(value.battery_ntc == <u8>(41));
assert(value.battery_temperature.length == 1);
assert(value.battery_temperature[0] == <f32>(-5.5));
assert(value.battery_serialnum.length == 32);
assert(value.battery_serialnum[0] == <u8>(229));
assert(value.battery_serialnum[1] == <u8>(164));
assert(value.battery_serialnum[2] == <u8>(103));
assert(value.battery_serialnum[3] == <u8>(38));
assert(value.battery_serialnum[4] == <u8>(225));
assert(value.battery_serialnum[5] == <u8>(160));
assert(value.battery_serialnum[6] == <u8>(99));
assert(value.battery_serialnum[7] == <u8>(34));
assert(value.battery_serialnum[8] == <u8>(237));
assert(value.battery_serialnum[9] == <u8>(172));
assert(value.battery_serialnum[10] == <u8>(93));
assert(value.battery_serialnum[11] == <u8>(28));
assert(value.battery_serialnum[12] == <u8>(223));
assert(value.battery_serialnum[13] == <u8>(158));
assert(value.battery_serialnum[14] == <u8>(89));
assert(value.battery_serialnum[15] == <u8>(24));
assert(value.battery_serialnum[16] == <u8>(219));
assert(value.battery_serialnum[17] == <u8>(154));
assert(value.battery_serialnum[18] == <u8>(85));
assert(value.battery_serialnum[19] == <u8>(20));
assert(value.battery_serialnum[20] == <u8>(4));
assert(value.battery_serialnum[21] == <u8>(69));
assert(value.battery_serialnum[22] == <u8>(134));
assert(value.battery_serialnum[23] == <u8>(199));
assert(value.battery_serialnum[24] == <u8>(0));
assert(value.battery_serialnum[25] == <u8>(65));
assert(value.battery_serialnum[26] == <u8>(130));
assert(value.battery_serialnum[27] == <u8>(195));
assert(value.battery_serialnum[28] == <u8>(12));
assert(value.battery_serialnum[29] == <u8>(77));
assert(value.battery_serialnum[30] == <u8>(51));
assert(value.battery_serialnum[31] == <u8>(114));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("94cec9042db7fe9af8bbed10952a610e002029c30000b7c100c0eac20000d3c1e0a8");const codec=new r.Ros2DrddsMsgGamepadDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.frame_id == <u64><u64>0x9afeb72d04c9ce94);
assert(value.header.stamp.sec == <i32>(284015608));
assert(value.header.stamp.nanosec == <u32>(241248917));
assert(value.left_axis_x == <f32>(-169.125));
assert(value.left_axis_y == <f32>(-22.875));
assert(value.right_axis_x == <f32>(-117.375));
assert(value.right_axis_y == <f32>(-26.375));
assert(value.buttons == <u16>(43232));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("b51f36cc0c66015277544c5c7639c31e00203ac300c09bc200005ac300806dc300a07f430000b0bf0000ac410080b9c200801ac3");const codec=new r.Ros2DrddsMsgImuDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.frame_id == <u64><u64>0x5201660ccc361fb5);
assert(value.header.stamp.sec == <i32>(1548506231));
assert(value.header.stamp.nanosec == <u32>(516110710));
assert(value.data.roll == <f32>(-186.125));
assert(value.data.pitch == <f32>(-77.875));
assert(value.data.yaw == <f32>(-218.0));
assert(value.data.omega_x == <f32>(-237.5));
assert(value.data.omega_y == <f32>(255.625));
assert(value.data.omega_z == <f32>(-1.375));
assert(value.data.acc_x == <f32>(21.5));
assert(value.data.acc_y == <f32>(-92.75));
assert(value.data.acc_z == <f32>(-154.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("0060134300606ec300002243008072c200e002c30080794300e0654300e0474300006dc2");const codec=new r.Ros2DrddsMsgImuDataValueCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.roll == <f32>(147.375));
assert(value.pitch == <f32>(-238.375));
assert(value.yaw == <f32>(162.0));
assert(value.omega_x == <f32>(-60.625));
assert(value.omega_y == <f32>(-130.875));
assert(value.omega_z == <f32>(249.5));
assert(value.acc_x == <f32>(229.875));
assert(value.acc_y == <f32>(199.875));
assert(value.acc_z == <f32>(-59.25));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("010000006744013238000071430020474300001143000086c2000090c1");const codec=new r.Ros2DrddsMsgJointCmdValueCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name.length == 1);
assert(value.name[0] == <u8>(103));
assert(value.data_id == <u16>(324));
assert(value.control_word == <u16>(14386));
assert(value.position == <f32>(241.0));
assert(value.torque == <f32>(199.125));
assert(value.velocity == <f32>(145.0));
assert(value.kp == <f32>(-67.0));
assert(value.kd == <f32>(-18.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_7():void{const input=bytes("de9f5c1dfdffa8330040ee4200c0ff4200e05343004043c300207343");const codec=new r.Ros2DrddsMsgJointDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name.length == 4);
assert(value.name[0] == <u8>(222));
assert(value.name[1] == <u8>(159));
assert(value.name[2] == <u8>(92));
assert(value.name[3] == <u8>(29));
assert(value.data_id == <u16>(65533));
assert(value.status_word == <u16>(13224));
assert(value.position == <f32>(119.125));
assert(value.torque == <f32>(127.875));
assert(value.velocity == <f32>(211.875));
assert(value.motion_temp == <f32>(-195.25));
assert(value.driver_temp == <f32>(243.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("7130f3b252857cfa00c098c2000066c100008c41004095420000b4c1");const codec=new r.Ros2DrddsMsgJointDataCmdCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name.length == 4);
assert(value.name[0] == <u8>(113));
assert(value.name[1] == <u8>(48));
assert(value.name[2] == <u8>(243));
assert(value.name[3] == <u8>(178));
assert(value.data_id == <u16>(34130));
assert(value.control_word == <u16>(64124));
assert(value.position == <f32>(-76.375));
assert(value.torque == <f32>(-14.375));
assert(value.velocity == <f32>(17.5));
assert(value.kp == <f32>(74.625));
assert(value.kd == <f32>(-22.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_9():void{const input=bytes("01000000ac8f2122df006001c30020504300c0744300803dc20040acc2");const codec=new r.Ros2DrddsMsgJointValueCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name.length == 1);
assert(value.name[0] == <u8>(172));
assert(value.data_id == <u16>(8591));
assert(value.status_word == <u16>(57122));
assert(value.position == <f32>(-129.375));
assert(value.torque == <f32>(208.125));
assert(value.velocity == <f32>(244.75));
assert(value.motion_temp == <f32>(-47.375));
assert(value.driver_temp == <f32>(-86.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("ebefb356529684c874ad05d0801b3c387839fabb5bca363b00c0c1420000dcc00080674300807ec300c0a6424607c48565a140da00a040430040264300005d4200806cc20000dac24504c786661a9bff00c0a7420080844200008ac20000cf410000ea417b3af9b85871ed1e000042c300009b410040734300805cc300a07e43024380c1216a2db400002f4200601cc300009cc1006066c300800a423c7dbeff1f015b55008017c300c072c300e058430080a8c2000072c23f7ebdfc1cba8070004001430000504000c05c43000054c300a07dc3014083c222d1f691008064c20000e6c100c04bc300206cc3008062c38ccd0e4faf8a0025008005430080ed4200807bc3004020c300604c43b2f3307191e176c40000e4c1006011c300a073430000a9c100401e43ecad6e2fcf88e5e50080e542000089c200205e4300e052430000c0c0d2935011f1e3930400e01dc30000a8c100c0c5c200e01bc300e061c3d1905312f2584821004008c30080b6c2002020430000264300c082c2efae6d2ccc333ec000008941000018c100801e4200c0b1c200802e4396d71455b528fe6a000021420060264300001ac200a0634300c08fc2a8e92a6b8b43888b00408cc200a04e430000f74200602dc3008021c3");const codec=new r.Ros2DrddsMsgJointsDataCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.frame_id == <u64><u64>0xc884965256b3efeb);
assert(value.header.stamp.sec == <i32>(-804934284));
assert(value.header.stamp.nanosec == <u32>(943463296));
assert(value.data.joints_data.length == 16);
assert(value.data.joints_data[0].name.length == 4);
assert(value.data.joints_data[0].name[0] == <u8>(120));
assert(value.data.joints_data[0].name[1] == <u8>(57));
assert(value.data.joints_data[0].name[2] == <u8>(250));
assert(value.data.joints_data[0].name[3] == <u8>(187));
assert(value.data.joints_data[0].data_id == <u16>(51803));
assert(value.data.joints_data[0].status_word == <u16>(15158));
assert(value.data.joints_data[0].position == <f32>(96.875));
assert(value.data.joints_data[0].torque == <f32>(-6.875));
assert(value.data.joints_data[0].velocity == <f32>(231.5));
assert(value.data.joints_data[0].motion_temp == <f32>(-254.5));
assert(value.data.joints_data[0].driver_temp == <f32>(83.375));
assert(value.data.joints_data[1].name.length == 4);
assert(value.data.joints_data[1].name[0] == <u8>(70));
assert(value.data.joints_data[1].name[1] == <u8>(7));
assert(value.data.joints_data[1].name[2] == <u8>(196));
assert(value.data.joints_data[1].name[3] == <u8>(133));
assert(value.data.joints_data[1].data_id == <u16>(41317));
assert(value.data.joints_data[1].status_word == <u16>(55872));
assert(value.data.joints_data[1].position == <f32>(192.625));
assert(value.data.joints_data[1].torque == <f32>(166.25));
assert(value.data.joints_data[1].velocity == <f32>(55.25));
assert(value.data.joints_data[1].motion_temp == <f32>(-59.125));
assert(value.data.joints_data[1].driver_temp == <f32>(-109.0));
assert(value.data.joints_data[2].name.length == 4);
assert(value.data.joints_data[2].name[0] == <u8>(69));
assert(value.data.joints_data[2].name[1] == <u8>(4));
assert(value.data.joints_data[2].name[2] == <u8>(199));
assert(value.data.joints_data[2].name[3] == <u8>(134));
assert(value.data.joints_data[2].data_id == <u16>(6758));
assert(value.data.joints_data[2].status_word == <u16>(65435));
assert(value.data.joints_data[2].position == <f32>(83.875));
assert(value.data.joints_data[2].torque == <f32>(66.25));
assert(value.data.joints_data[2].velocity == <f32>(-69.0));
assert(value.data.joints_data[2].motion_temp == <f32>(25.875));
assert(value.data.joints_data[2].driver_temp == <f32>(29.25));
assert(value.data.joints_data[3].name.length == 4);
assert(value.data.joints_data[3].name[0] == <u8>(123));
assert(value.data.joints_data[3].name[1] == <u8>(58));
assert(value.data.joints_data[3].name[2] == <u8>(249));
assert(value.data.joints_data[3].name[3] == <u8>(184));
assert(value.data.joints_data[3].data_id == <u16>(29016));
assert(value.data.joints_data[3].status_word == <u16>(7917));
assert(value.data.joints_data[3].position == <f32>(-194.0));
assert(value.data.joints_data[3].torque == <f32>(19.375));
assert(value.data.joints_data[3].velocity == <f32>(243.25));
assert(value.data.joints_data[3].motion_temp == <f32>(-220.5));
assert(value.data.joints_data[3].driver_temp == <f32>(254.625));
assert(value.data.joints_data[4].name.length == 4);
assert(value.data.joints_data[4].name[0] == <u8>(2));
assert(value.data.joints_data[4].name[1] == <u8>(67));
assert(value.data.joints_data[4].name[2] == <u8>(128));
assert(value.data.joints_data[4].name[3] == <u8>(193));
assert(value.data.joints_data[4].data_id == <u16>(27169));
assert(value.data.joints_data[4].status_word == <u16>(46125));
assert(value.data.joints_data[4].position == <f32>(43.75));
assert(value.data.joints_data[4].torque == <f32>(-156.375));
assert(value.data.joints_data[4].velocity == <f32>(-19.5));
assert(value.data.joints_data[4].motion_temp == <f32>(-230.375));
assert(value.data.joints_data[4].driver_temp == <f32>(34.625));
assert(value.data.joints_data[5].name.length == 4);
assert(value.data.joints_data[5].name[0] == <u8>(60));
assert(value.data.joints_data[5].name[1] == <u8>(125));
assert(value.data.joints_data[5].name[2] == <u8>(190));
assert(value.data.joints_data[5].name[3] == <u8>(255));
assert(value.data.joints_data[5].data_id == <u16>(287));
assert(value.data.joints_data[5].status_word == <u16>(21851));
assert(value.data.joints_data[5].position == <f32>(-151.5));
assert(value.data.joints_data[5].torque == <f32>(-242.75));
assert(value.data.joints_data[5].velocity == <f32>(216.875));
assert(value.data.joints_data[5].motion_temp == <f32>(-84.25));
assert(value.data.joints_data[5].driver_temp == <f32>(-60.5));
assert(value.data.joints_data[6].name.length == 4);
assert(value.data.joints_data[6].name[0] == <u8>(63));
assert(value.data.joints_data[6].name[1] == <u8>(126));
assert(value.data.joints_data[6].name[2] == <u8>(189));
assert(value.data.joints_data[6].name[3] == <u8>(252));
assert(value.data.joints_data[6].data_id == <u16>(47644));
assert(value.data.joints_data[6].status_word == <u16>(28800));
assert(value.data.joints_data[6].position == <f32>(129.25));
assert(value.data.joints_data[6].torque == <f32>(3.25));
assert(value.data.joints_data[6].velocity == <f32>(220.75));
assert(value.data.joints_data[6].motion_temp == <f32>(-212.0));
assert(value.data.joints_data[6].driver_temp == <f32>(-253.625));
assert(value.data.joints_data[7].name.length == 4);
assert(value.data.joints_data[7].name[0] == <u8>(1));
assert(value.data.joints_data[7].name[1] == <u8>(64));
assert(value.data.joints_data[7].name[2] == <u8>(131));
assert(value.data.joints_data[7].name[3] == <u8>(194));
assert(value.data.joints_data[7].data_id == <u16>(53538));
assert(value.data.joints_data[7].status_word == <u16>(37366));
assert(value.data.joints_data[7].position == <f32>(-57.125));
assert(value.data.joints_data[7].torque == <f32>(-28.75));
assert(value.data.joints_data[7].velocity == <f32>(-203.75));
assert(value.data.joints_data[7].motion_temp == <f32>(-236.125));
assert(value.data.joints_data[7].driver_temp == <f32>(-226.5));
assert(value.data.joints_data[8].name.length == 4);
assert(value.data.joints_data[8].name[0] == <u8>(140));
assert(value.data.joints_data[8].name[1] == <u8>(205));
assert(value.data.joints_data[8].name[2] == <u8>(14));
assert(value.data.joints_data[8].name[3] == <u8>(79));
assert(value.data.joints_data[8].data_id == <u16>(35503));
assert(value.data.joints_data[8].status_word == <u16>(9472));
assert(value.data.joints_data[8].position == <f32>(133.5));
assert(value.data.joints_data[8].torque == <f32>(118.75));
assert(value.data.joints_data[8].velocity == <f32>(-251.5));
assert(value.data.joints_data[8].motion_temp == <f32>(-160.25));
assert(value.data.joints_data[8].driver_temp == <f32>(204.375));
assert(value.data.joints_data[9].name.length == 4);
assert(value.data.joints_data[9].name[0] == <u8>(178));
assert(value.data.joints_data[9].name[1] == <u8>(243));
assert(value.data.joints_data[9].name[2] == <u8>(48));
assert(value.data.joints_data[9].name[3] == <u8>(113));
assert(value.data.joints_data[9].data_id == <u16>(57745));
assert(value.data.joints_data[9].status_word == <u16>(50294));
assert(value.data.joints_data[9].position == <f32>(-28.5));
assert(value.data.joints_data[9].torque == <f32>(-145.375));
assert(value.data.joints_data[9].velocity == <f32>(243.625));
assert(value.data.joints_data[9].motion_temp == <f32>(-21.125));
assert(value.data.joints_data[9].driver_temp == <f32>(158.25));
assert(value.data.joints_data[10].name.length == 4);
assert(value.data.joints_data[10].name[0] == <u8>(236));
assert(value.data.joints_data[10].name[1] == <u8>(173));
assert(value.data.joints_data[10].name[2] == <u8>(110));
assert(value.data.joints_data[10].name[3] == <u8>(47));
assert(value.data.joints_data[10].data_id == <u16>(35023));
assert(value.data.joints_data[10].status_word == <u16>(58853));
assert(value.data.joints_data[10].position == <f32>(114.75));
assert(value.data.joints_data[10].torque == <f32>(-68.5));
assert(value.data.joints_data[10].velocity == <f32>(222.125));
assert(value.data.joints_data[10].motion_temp == <f32>(210.875));
assert(value.data.joints_data[10].driver_temp == <f32>(-6.0));
assert(value.data.joints_data[11].name.length == 4);
assert(value.data.joints_data[11].name[0] == <u8>(210));
assert(value.data.joints_data[11].name[1] == <u8>(147));
assert(value.data.joints_data[11].name[2] == <u8>(80));
assert(value.data.joints_data[11].name[3] == <u8>(17));
assert(value.data.joints_data[11].data_id == <u16>(58353));
assert(value.data.joints_data[11].status_word == <u16>(1171));
assert(value.data.joints_data[11].position == <f32>(-157.875));
assert(value.data.joints_data[11].torque == <f32>(-21.0));
assert(value.data.joints_data[11].velocity == <f32>(-98.875));
assert(value.data.joints_data[11].motion_temp == <f32>(-155.875));
assert(value.data.joints_data[11].driver_temp == <f32>(-225.875));
assert(value.data.joints_data[12].name.length == 4);
assert(value.data.joints_data[12].name[0] == <u8>(209));
assert(value.data.joints_data[12].name[1] == <u8>(144));
assert(value.data.joints_data[12].name[2] == <u8>(83));
assert(value.data.joints_data[12].name[3] == <u8>(18));
assert(value.data.joints_data[12].data_id == <u16>(22770));
assert(value.data.joints_data[12].status_word == <u16>(8520));
assert(value.data.joints_data[12].position == <f32>(-136.25));
assert(value.data.joints_data[12].torque == <f32>(-91.25));
assert(value.data.joints_data[12].velocity == <f32>(160.125));
assert(value.data.joints_data[12].motion_temp == <f32>(166.0));
assert(value.data.joints_data[12].driver_temp == <f32>(-65.375));
assert(value.data.joints_data[13].name.length == 4);
assert(value.data.joints_data[13].name[0] == <u8>(239));
assert(value.data.joints_data[13].name[1] == <u8>(174));
assert(value.data.joints_data[13].name[2] == <u8>(109));
assert(value.data.joints_data[13].name[3] == <u8>(44));
assert(value.data.joints_data[13].data_id == <u16>(13260));
assert(value.data.joints_data[13].status_word == <u16>(49214));
assert(value.data.joints_data[13].position == <f32>(17.125));
assert(value.data.joints_data[13].torque == <f32>(-9.5));
assert(value.data.joints_data[13].velocity == <f32>(39.625));
assert(value.data.joints_data[13].motion_temp == <f32>(-88.875));
assert(value.data.joints_data[13].driver_temp == <f32>(174.5));
assert(value.data.joints_data[14].name.length == 4);
assert(value.data.joints_data[14].name[0] == <u8>(150));
assert(value.data.joints_data[14].name[1] == <u8>(215));
assert(value.data.joints_data[14].name[2] == <u8>(20));
assert(value.data.joints_data[14].name[3] == <u8>(85));
assert(value.data.joints_data[14].data_id == <u16>(10421));
assert(value.data.joints_data[14].status_word == <u16>(27390));
assert(value.data.joints_data[14].position == <f32>(40.25));
assert(value.data.joints_data[14].torque == <f32>(166.375));
assert(value.data.joints_data[14].velocity == <f32>(-38.5));
assert(value.data.joints_data[14].motion_temp == <f32>(227.625));
assert(value.data.joints_data[14].driver_temp == <f32>(-71.875));
assert(value.data.joints_data[15].name.length == 4);
assert(value.data.joints_data[15].name[0] == <u8>(168));
assert(value.data.joints_data[15].name[1] == <u8>(233));
assert(value.data.joints_data[15].name[2] == <u8>(42));
assert(value.data.joints_data[15].name[3] == <u8>(107));
assert(value.data.joints_data[15].data_id == <u16>(17291));
assert(value.data.joints_data[15].status_word == <u16>(35720));
assert(value.data.joints_data[15].position == <f32>(-70.125));
assert(value.data.joints_data[15].torque == <f32>(206.625));
assert(value.data.joints_data[15].velocity == <f32>(123.5));
assert(value.data.joints_data[15].motion_temp == <f32>(-173.375));
assert(value.data.joints_data[15].driver_temp == <f32>(-161.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("d60425726f7d12ec8e7749886a7a6f237031f2b353f2ef5200c08ec20040aa4200e005c300201343000078c04e0fcc8d6d9907890040f64200e029c300c00a430000ac4100c027434d0ccf8e6e227ee30040c44200803ec20080eac200e032c3000016c27332f1b0504996380000cc41008018420000474200400bc3008050430a4b88c929528c3700207ec300c02fc300607f4300002ac200c0ae423475b6f7173964ec0000c1c200200e4300e02e43006071c30080e3423776b5f414821d860000844100e01ec3002055c300009442008054c209488bca2ae9f55d00205bc300607bc3000086c100c0e042000078c384c50647a7b2689e00805643008067430020384300c003c300c03b43bafb387999d980450080e64200000cc300c089420000dcc00000db41e6a76425c53a8dae00c03cc30000a0420080aec200a035c300c0a642d8995a1bfb51657500c0e4420000044200c0494300009c4100006d42db9a5918f8ea1c1f00406cc300a014c300e06dc300e03dc300000cc3e5a46726c681f4c40000b44200005fc300c0adc2000062c3008064429cdd1e5fbf9aeecb0000a5c200c0904200a0064300a07e4300c03ec3a2e3206181f1061000c083420000fc4100805bc200a005c300001343");const codec=new r.Ros2DrddsMsgJointsDataCmdCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.frame_id == <u64><u64>0xec127d6f722504d6);
assert(value.header.stamp.sec == <i32>(-2008451186));
assert(value.header.stamp.nanosec == <u32>(594508394));
assert(value.data.joints_data.length == 16);
assert(value.data.joints_data[0].name.length == 4);
assert(value.data.joints_data[0].name[0] == <u8>(112));
assert(value.data.joints_data[0].name[1] == <u8>(49));
assert(value.data.joints_data[0].name[2] == <u8>(242));
assert(value.data.joints_data[0].name[3] == <u8>(179));
assert(value.data.joints_data[0].data_id == <u16>(62035));
assert(value.data.joints_data[0].control_word == <u16>(21231));
assert(value.data.joints_data[0].position == <f32>(-71.375));
assert(value.data.joints_data[0].torque == <f32>(85.125));
assert(value.data.joints_data[0].velocity == <f32>(-133.875));
assert(value.data.joints_data[0].kp == <f32>(147.125));
assert(value.data.joints_data[0].kd == <f32>(-3.875));
assert(value.data.joints_data[1].name.length == 4);
assert(value.data.joints_data[1].name[0] == <u8>(78));
assert(value.data.joints_data[1].name[1] == <u8>(15));
assert(value.data.joints_data[1].name[2] == <u8>(204));
assert(value.data.joints_data[1].name[3] == <u8>(141));
assert(value.data.joints_data[1].data_id == <u16>(39277));
assert(value.data.joints_data[1].control_word == <u16>(35079));
assert(value.data.joints_data[1].position == <f32>(123.125));
assert(value.data.joints_data[1].torque == <f32>(-169.875));
assert(value.data.joints_data[1].velocity == <f32>(138.75));
assert(value.data.joints_data[1].kp == <f32>(21.5));
assert(value.data.joints_data[1].kd == <f32>(167.75));
assert(value.data.joints_data[2].name.length == 4);
assert(value.data.joints_data[2].name[0] == <u8>(77));
assert(value.data.joints_data[2].name[1] == <u8>(12));
assert(value.data.joints_data[2].name[2] == <u8>(207));
assert(value.data.joints_data[2].name[3] == <u8>(142));
assert(value.data.joints_data[2].data_id == <u16>(8814));
assert(value.data.joints_data[2].control_word == <u16>(58238));
assert(value.data.joints_data[2].position == <f32>(98.125));
assert(value.data.joints_data[2].torque == <f32>(-47.625));
assert(value.data.joints_data[2].velocity == <f32>(-117.25));
assert(value.data.joints_data[2].kp == <f32>(-178.875));
assert(value.data.joints_data[2].kd == <f32>(-37.5));
assert(value.data.joints_data[3].name.length == 4);
assert(value.data.joints_data[3].name[0] == <u8>(115));
assert(value.data.joints_data[3].name[1] == <u8>(50));
assert(value.data.joints_data[3].name[2] == <u8>(241));
assert(value.data.joints_data[3].name[3] == <u8>(176));
assert(value.data.joints_data[3].data_id == <u16>(18768));
assert(value.data.joints_data[3].control_word == <u16>(14486));
assert(value.data.joints_data[3].position == <f32>(25.5));
assert(value.data.joints_data[3].torque == <f32>(38.125));
assert(value.data.joints_data[3].velocity == <f32>(49.75));
assert(value.data.joints_data[3].kp == <f32>(-139.25));
assert(value.data.joints_data[3].kd == <f32>(208.5));
assert(value.data.joints_data[4].name.length == 4);
assert(value.data.joints_data[4].name[0] == <u8>(10));
assert(value.data.joints_data[4].name[1] == <u8>(75));
assert(value.data.joints_data[4].name[2] == <u8>(136));
assert(value.data.joints_data[4].name[3] == <u8>(201));
assert(value.data.joints_data[4].data_id == <u16>(21033));
assert(value.data.joints_data[4].control_word == <u16>(14220));
assert(value.data.joints_data[4].position == <f32>(-254.125));
assert(value.data.joints_data[4].torque == <f32>(-175.75));
assert(value.data.joints_data[4].velocity == <f32>(255.375));
assert(value.data.joints_data[4].kp == <f32>(-42.5));
assert(value.data.joints_data[4].kd == <f32>(87.375));
assert(value.data.joints_data[5].name.length == 4);
assert(value.data.joints_data[5].name[0] == <u8>(52));
assert(value.data.joints_data[5].name[1] == <u8>(117));
assert(value.data.joints_data[5].name[2] == <u8>(182));
assert(value.data.joints_data[5].name[3] == <u8>(247));
assert(value.data.joints_data[5].data_id == <u16>(14615));
assert(value.data.joints_data[5].control_word == <u16>(60516));
assert(value.data.joints_data[5].position == <f32>(-96.5));
assert(value.data.joints_data[5].torque == <f32>(142.125));
assert(value.data.joints_data[5].velocity == <f32>(174.875));
assert(value.data.joints_data[5].kp == <f32>(-241.375));
assert(value.data.joints_data[5].kd == <f32>(113.75));
assert(value.data.joints_data[6].name.length == 4);
assert(value.data.joints_data[6].name[0] == <u8>(55));
assert(value.data.joints_data[6].name[1] == <u8>(118));
assert(value.data.joints_data[6].name[2] == <u8>(181));
assert(value.data.joints_data[6].name[3] == <u8>(244));
assert(value.data.joints_data[6].data_id == <u16>(33300));
assert(value.data.joints_data[6].control_word == <u16>(34333));
assert(value.data.joints_data[6].position == <f32>(16.5));
assert(value.data.joints_data[6].torque == <f32>(-158.875));
assert(value.data.joints_data[6].velocity == <f32>(-213.125));
assert(value.data.joints_data[6].kp == <f32>(74.0));
assert(value.data.joints_data[6].kd == <f32>(-53.125));
assert(value.data.joints_data[7].name.length == 4);
assert(value.data.joints_data[7].name[0] == <u8>(9));
assert(value.data.joints_data[7].name[1] == <u8>(72));
assert(value.data.joints_data[7].name[2] == <u8>(139));
assert(value.data.joints_data[7].name[3] == <u8>(202));
assert(value.data.joints_data[7].data_id == <u16>(59690));
assert(value.data.joints_data[7].control_word == <u16>(24053));
assert(value.data.joints_data[7].position == <f32>(-219.125));
assert(value.data.joints_data[7].torque == <f32>(-251.375));
assert(value.data.joints_data[7].velocity == <f32>(-16.75));
assert(value.data.joints_data[7].kp == <f32>(112.375));
assert(value.data.joints_data[7].kd == <f32>(-248.0));
assert(value.data.joints_data[8].name.length == 4);
assert(value.data.joints_data[8].name[0] == <u8>(132));
assert(value.data.joints_data[8].name[1] == <u8>(197));
assert(value.data.joints_data[8].name[2] == <u8>(6));
assert(value.data.joints_data[8].name[3] == <u8>(71));
assert(value.data.joints_data[8].data_id == <u16>(45735));
assert(value.data.joints_data[8].control_word == <u16>(40552));
assert(value.data.joints_data[8].position == <f32>(214.5));
assert(value.data.joints_data[8].torque == <f32>(231.5));
assert(value.data.joints_data[8].velocity == <f32>(184.125));
assert(value.data.joints_data[8].kp == <f32>(-131.75));
assert(value.data.joints_data[8].kd == <f32>(187.75));
assert(value.data.joints_data[9].name.length == 4);
assert(value.data.joints_data[9].name[0] == <u8>(186));
assert(value.data.joints_data[9].name[1] == <u8>(251));
assert(value.data.joints_data[9].name[2] == <u8>(56));
assert(value.data.joints_data[9].name[3] == <u8>(121));
assert(value.data.joints_data[9].data_id == <u16>(55705));
assert(value.data.joints_data[9].control_word == <u16>(17792));
assert(value.data.joints_data[9].position == <f32>(115.25));
assert(value.data.joints_data[9].torque == <f32>(-140.0));
assert(value.data.joints_data[9].velocity == <f32>(68.875));
assert(value.data.joints_data[9].kp == <f32>(-6.875));
assert(value.data.joints_data[9].kd == <f32>(27.375));
assert(value.data.joints_data[10].name.length == 4);
assert(value.data.joints_data[10].name[0] == <u8>(230));
assert(value.data.joints_data[10].name[1] == <u8>(167));
assert(value.data.joints_data[10].name[2] == <u8>(100));
assert(value.data.joints_data[10].name[3] == <u8>(37));
assert(value.data.joints_data[10].data_id == <u16>(15045));
assert(value.data.joints_data[10].control_word == <u16>(44685));
assert(value.data.joints_data[10].position == <f32>(-188.75));
assert(value.data.joints_data[10].torque == <f32>(80.0));
assert(value.data.joints_data[10].velocity == <f32>(-87.25));
assert(value.data.joints_data[10].kp == <f32>(-181.625));
assert(value.data.joints_data[10].kd == <f32>(83.375));
assert(value.data.joints_data[11].name.length == 4);
assert(value.data.joints_data[11].name[0] == <u8>(216));
assert(value.data.joints_data[11].name[1] == <u8>(153));
assert(value.data.joints_data[11].name[2] == <u8>(90));
assert(value.data.joints_data[11].name[3] == <u8>(27));
assert(value.data.joints_data[11].data_id == <u16>(20987));
assert(value.data.joints_data[11].control_word == <u16>(30053));
assert(value.data.joints_data[11].position == <f32>(114.375));
assert(value.data.joints_data[11].torque == <f32>(33.0));
assert(value.data.joints_data[11].velocity == <f32>(201.75));
assert(value.data.joints_data[11].kp == <f32>(19.5));
assert(value.data.joints_data[11].kd == <f32>(59.25));
assert(value.data.joints_data[12].name.length == 4);
assert(value.data.joints_data[12].name[0] == <u8>(219));
assert(value.data.joints_data[12].name[1] == <u8>(154));
assert(value.data.joints_data[12].name[2] == <u8>(89));
assert(value.data.joints_data[12].name[3] == <u8>(24));
assert(value.data.joints_data[12].data_id == <u16>(60152));
assert(value.data.joints_data[12].control_word == <u16>(7964));
assert(value.data.joints_data[12].position == <f32>(-236.25));
assert(value.data.joints_data[12].torque == <f32>(-148.625));
assert(value.data.joints_data[12].velocity == <f32>(-237.875));
assert(value.data.joints_data[12].kp == <f32>(-189.875));
assert(value.data.joints_data[12].kd == <f32>(-140.0));
assert(value.data.joints_data[13].name.length == 4);
assert(value.data.joints_data[13].name[0] == <u8>(229));
assert(value.data.joints_data[13].name[1] == <u8>(164));
assert(value.data.joints_data[13].name[2] == <u8>(103));
assert(value.data.joints_data[13].name[3] == <u8>(38));
assert(value.data.joints_data[13].data_id == <u16>(33222));
assert(value.data.joints_data[13].control_word == <u16>(50420));
assert(value.data.joints_data[13].position == <f32>(90.0));
assert(value.data.joints_data[13].torque == <f32>(-223.0));
assert(value.data.joints_data[13].velocity == <f32>(-86.875));
assert(value.data.joints_data[13].kp == <f32>(-226.0));
assert(value.data.joints_data[13].kd == <f32>(57.125));
assert(value.data.joints_data[14].name.length == 4);
assert(value.data.joints_data[14].name[0] == <u8>(156));
assert(value.data.joints_data[14].name[1] == <u8>(221));
assert(value.data.joints_data[14].name[2] == <u8>(30));
assert(value.data.joints_data[14].name[3] == <u8>(95));
assert(value.data.joints_data[14].data_id == <u16>(39615));
assert(value.data.joints_data[14].control_word == <u16>(52206));
assert(value.data.joints_data[14].position == <f32>(-82.5));
assert(value.data.joints_data[14].torque == <f32>(72.375));
assert(value.data.joints_data[14].velocity == <f32>(134.625));
assert(value.data.joints_data[14].kp == <f32>(254.625));
assert(value.data.joints_data[14].kd == <f32>(-190.75));
assert(value.data.joints_data[15].name.length == 4);
assert(value.data.joints_data[15].name[0] == <u8>(162));
assert(value.data.joints_data[15].name[1] == <u8>(227));
assert(value.data.joints_data[15].name[2] == <u8>(32));
assert(value.data.joints_data[15].name[3] == <u8>(97));
assert(value.data.joints_data[15].data_id == <u16>(61825));
assert(value.data.joints_data[15].control_word == <u16>(4102));
assert(value.data.joints_data[15].position == <f32>(65.875));
assert(value.data.joints_data[15].torque == <f32>(31.5));
assert(value.data.joints_data[15].velocity == <f32>(-54.875));
assert(value.data.joints_data[15].kp == <f32>(-133.625));
assert(value.data.joints_data[15].kd == <f32>(147.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("ebaa6928c868199500c014c300a034430040104300a0604300009ec2d5945716f603f14e00205cc30080c4c2006046c3000029c20000a7c2d6975415f5b8882400405bc3002073c300c0a142000063c300a072c3e8a96a2bcbd360ff00809bc200803e4300604fc30020144300e009c391d01352b2c87af0000087c20080fac200009fc2008025c3004028c3afee2d6c8ca3922b008018430080c542008070420000864200a07e43aced2e6f8f18eb41008064c3004011c300a04b430000e242002057c392d31051b173039a008048c200002e4200200c43000011420000ab411f5e9ddc3c289e5900008cc10000de420080b7c20000cec20000c2422160a3e202437682006066c30000a54100c0c5c200204a4300809f42b0f13273937b630a0000f3420080fbc2000047c200206643004091428ecf0c4dad108bd1002045430060764300c055c30000e24200c04c438dcc0f4eaeabf2bb006026c300a0194300c0adc20040184300207e43b3f2317090c01a6000c0194300808ac200201b430080bec200a06ac3ca8b4809e9db006f000046c30040f8c200806dc200401cc300c0a9c2f4b57637d7b0e8b400800a420000004100e07c4300807a4300800343");const codec=new r.Ros2DrddsMsgJointsDataCmdValueCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.joints_data.length == 16);
assert(value.joints_data[0].name.length == 4);
assert(value.joints_data[0].name[0] == <u8>(235));
assert(value.joints_data[0].name[1] == <u8>(170));
assert(value.joints_data[0].name[2] == <u8>(105));
assert(value.joints_data[0].name[3] == <u8>(40));
assert(value.joints_data[0].data_id == <u16>(26824));
assert(value.joints_data[0].control_word == <u16>(38169));
assert(value.joints_data[0].position == <f32>(-148.75));
assert(value.joints_data[0].torque == <f32>(180.625));
assert(value.joints_data[0].velocity == <f32>(144.25));
assert(value.joints_data[0].kp == <f32>(224.625));
assert(value.joints_data[0].kd == <f32>(-79.0));
assert(value.joints_data[1].name.length == 4);
assert(value.joints_data[1].name[0] == <u8>(213));
assert(value.joints_data[1].name[1] == <u8>(148));
assert(value.joints_data[1].name[2] == <u8>(87));
assert(value.joints_data[1].name[3] == <u8>(22));
assert(value.joints_data[1].data_id == <u16>(1014));
assert(value.joints_data[1].control_word == <u16>(20209));
assert(value.joints_data[1].position == <f32>(-220.125));
assert(value.joints_data[1].torque == <f32>(-98.25));
assert(value.joints_data[1].velocity == <f32>(-198.375));
assert(value.joints_data[1].kp == <f32>(-42.25));
assert(value.joints_data[1].kd == <f32>(-83.5));
assert(value.joints_data[2].name.length == 4);
assert(value.joints_data[2].name[0] == <u8>(214));
assert(value.joints_data[2].name[1] == <u8>(151));
assert(value.joints_data[2].name[2] == <u8>(84));
assert(value.joints_data[2].name[3] == <u8>(21));
assert(value.joints_data[2].data_id == <u16>(47349));
assert(value.joints_data[2].control_word == <u16>(9352));
assert(value.joints_data[2].position == <f32>(-219.25));
assert(value.joints_data[2].torque == <f32>(-243.125));
assert(value.joints_data[2].velocity == <f32>(80.875));
assert(value.joints_data[2].kp == <f32>(-227.0));
assert(value.joints_data[2].kd == <f32>(-242.625));
assert(value.joints_data[3].name.length == 4);
assert(value.joints_data[3].name[0] == <u8>(232));
assert(value.joints_data[3].name[1] == <u8>(169));
assert(value.joints_data[3].name[2] == <u8>(106));
assert(value.joints_data[3].name[3] == <u8>(43));
assert(value.joints_data[3].data_id == <u16>(54219));
assert(value.joints_data[3].control_word == <u16>(65376));
assert(value.joints_data[3].position == <f32>(-77.75));
assert(value.joints_data[3].torque == <f32>(190.5));
assert(value.joints_data[3].velocity == <f32>(-207.375));
assert(value.joints_data[3].kp == <f32>(148.125));
assert(value.joints_data[3].kd == <f32>(-137.875));
assert(value.joints_data[4].name.length == 4);
assert(value.joints_data[4].name[0] == <u8>(145));
assert(value.joints_data[4].name[1] == <u8>(208));
assert(value.joints_data[4].name[2] == <u8>(19));
assert(value.joints_data[4].name[3] == <u8>(82));
assert(value.joints_data[4].data_id == <u16>(51378));
assert(value.joints_data[4].control_word == <u16>(61562));
assert(value.joints_data[4].position == <f32>(-67.5));
assert(value.joints_data[4].torque == <f32>(-125.25));
assert(value.joints_data[4].velocity == <f32>(-79.5));
assert(value.joints_data[4].kp == <f32>(-165.5));
assert(value.joints_data[4].kd == <f32>(-168.25));
assert(value.joints_data[5].name.length == 4);
assert(value.joints_data[5].name[0] == <u8>(175));
assert(value.joints_data[5].name[1] == <u8>(238));
assert(value.joints_data[5].name[2] == <u8>(45));
assert(value.joints_data[5].name[3] == <u8>(108));
assert(value.joints_data[5].data_id == <u16>(41868));
assert(value.joints_data[5].control_word == <u16>(11154));
assert(value.joints_data[5].position == <f32>(152.5));
assert(value.joints_data[5].torque == <f32>(98.75));
assert(value.joints_data[5].velocity == <f32>(60.125));
assert(value.joints_data[5].kp == <f32>(67.0));
assert(value.joints_data[5].kd == <f32>(254.625));
assert(value.joints_data[6].name.length == 4);
assert(value.joints_data[6].name[0] == <u8>(172));
assert(value.joints_data[6].name[1] == <u8>(237));
assert(value.joints_data[6].name[2] == <u8>(46));
assert(value.joints_data[6].name[3] == <u8>(111));
assert(value.joints_data[6].data_id == <u16>(6287));
assert(value.joints_data[6].control_word == <u16>(16875));
assert(value.joints_data[6].position == <f32>(-228.5));
assert(value.joints_data[6].torque == <f32>(-145.25));
assert(value.joints_data[6].velocity == <f32>(203.625));
assert(value.joints_data[6].kp == <f32>(113.0));
assert(value.joints_data[6].kd == <f32>(-215.125));
assert(value.joints_data[7].name.length == 4);
assert(value.joints_data[7].name[0] == <u8>(146));
assert(value.joints_data[7].name[1] == <u8>(211));
assert(value.joints_data[7].name[2] == <u8>(16));
assert(value.joints_data[7].name[3] == <u8>(81));
assert(value.joints_data[7].data_id == <u16>(29617));
assert(value.joints_data[7].control_word == <u16>(39427));
assert(value.joints_data[7].position == <f32>(-50.125));
assert(value.joints_data[7].torque == <f32>(43.5));
assert(value.joints_data[7].velocity == <f32>(140.125));
assert(value.joints_data[7].kp == <f32>(36.25));
assert(value.joints_data[7].kd == <f32>(21.375));
assert(value.joints_data[8].name.length == 4);
assert(value.joints_data[8].name[0] == <u8>(31));
assert(value.joints_data[8].name[1] == <u8>(94));
assert(value.joints_data[8].name[2] == <u8>(157));
assert(value.joints_data[8].name[3] == <u8>(220));
assert(value.joints_data[8].data_id == <u16>(10300));
assert(value.joints_data[8].control_word == <u16>(22942));
assert(value.joints_data[8].position == <f32>(-17.5));
assert(value.joints_data[8].torque == <f32>(111.0));
assert(value.joints_data[8].velocity == <f32>(-91.75));
assert(value.joints_data[8].kp == <f32>(-103.0));
assert(value.joints_data[8].kd == <f32>(97.0));
assert(value.joints_data[9].name.length == 4);
assert(value.joints_data[9].name[0] == <u8>(33));
assert(value.joints_data[9].name[1] == <u8>(96));
assert(value.joints_data[9].name[2] == <u8>(163));
assert(value.joints_data[9].name[3] == <u8>(226));
assert(value.joints_data[9].data_id == <u16>(17154));
assert(value.joints_data[9].control_word == <u16>(33398));
assert(value.joints_data[9].position == <f32>(-230.375));
assert(value.joints_data[9].torque == <f32>(20.625));
assert(value.joints_data[9].velocity == <f32>(-98.875));
assert(value.joints_data[9].kp == <f32>(202.125));
assert(value.joints_data[9].kd == <f32>(79.75));
assert(value.joints_data[10].name.length == 4);
assert(value.joints_data[10].name[0] == <u8>(176));
assert(value.joints_data[10].name[1] == <u8>(241));
assert(value.joints_data[10].name[2] == <u8>(50));
assert(value.joints_data[10].name[3] == <u8>(115));
assert(value.joints_data[10].data_id == <u16>(31635));
assert(value.joints_data[10].control_word == <u16>(2659));
assert(value.joints_data[10].position == <f32>(121.5));
assert(value.joints_data[10].torque == <f32>(-125.75));
assert(value.joints_data[10].velocity == <f32>(-49.75));
assert(value.joints_data[10].kp == <f32>(230.125));
assert(value.joints_data[10].kd == <f32>(72.625));
assert(value.joints_data[11].name.length == 4);
assert(value.joints_data[11].name[0] == <u8>(142));
assert(value.joints_data[11].name[1] == <u8>(207));
assert(value.joints_data[11].name[2] == <u8>(12));
assert(value.joints_data[11].name[3] == <u8>(77));
assert(value.joints_data[11].data_id == <u16>(4269));
assert(value.joints_data[11].control_word == <u16>(53643));
assert(value.joints_data[11].position == <f32>(197.125));
assert(value.joints_data[11].torque == <f32>(246.375));
assert(value.joints_data[11].velocity == <f32>(-213.75));
assert(value.joints_data[11].kp == <f32>(113.0));
assert(value.joints_data[11].kd == <f32>(204.75));
assert(value.joints_data[12].name.length == 4);
assert(value.joints_data[12].name[0] == <u8>(141));
assert(value.joints_data[12].name[1] == <u8>(204));
assert(value.joints_data[12].name[2] == <u8>(15));
assert(value.joints_data[12].name[3] == <u8>(78));
assert(value.joints_data[12].data_id == <u16>(43950));
assert(value.joints_data[12].control_word == <u16>(48114));
assert(value.joints_data[12].position == <f32>(-166.375));
assert(value.joints_data[12].torque == <f32>(153.625));
assert(value.joints_data[12].velocity == <f32>(-86.875));
assert(value.joints_data[12].kp == <f32>(152.25));
assert(value.joints_data[12].kd == <f32>(254.125));
assert(value.joints_data[13].name.length == 4);
assert(value.joints_data[13].name[0] == <u8>(179));
assert(value.joints_data[13].name[1] == <u8>(242));
assert(value.joints_data[13].name[2] == <u8>(49));
assert(value.joints_data[13].name[3] == <u8>(112));
assert(value.joints_data[13].data_id == <u16>(49296));
assert(value.joints_data[13].control_word == <u16>(24602));
assert(value.joints_data[13].position == <f32>(153.75));
assert(value.joints_data[13].torque == <f32>(-69.25));
assert(value.joints_data[13].velocity == <f32>(155.125));
assert(value.joints_data[13].kp == <f32>(-95.25));
assert(value.joints_data[13].kd == <f32>(-234.625));
assert(value.joints_data[14].name.length == 4);
assert(value.joints_data[14].name[0] == <u8>(202));
assert(value.joints_data[14].name[1] == <u8>(139));
assert(value.joints_data[14].name[2] == <u8>(72));
assert(value.joints_data[14].name[3] == <u8>(9));
assert(value.joints_data[14].data_id == <u16>(56297));
assert(value.joints_data[14].control_word == <u16>(28416));
assert(value.joints_data[14].position == <f32>(-198.0));
assert(value.joints_data[14].torque == <f32>(-124.125));
assert(value.joints_data[14].velocity == <f32>(-59.375));
assert(value.joints_data[14].kp == <f32>(-156.25));
assert(value.joints_data[14].kd == <f32>(-84.875));
assert(value.joints_data[15].name.length == 4);
assert(value.joints_data[15].name[0] == <u8>(244));
assert(value.joints_data[15].name[1] == <u8>(181));
assert(value.joints_data[15].name[2] == <u8>(118));
assert(value.joints_data[15].name[3] == <u8>(55));
assert(value.joints_data[15].data_id == <u16>(45271));
assert(value.joints_data[15].control_word == <u16>(46312));
assert(value.joints_data[15].position == <f32>(34.625));
assert(value.joints_data[15].torque == <f32>(8.0));
assert(value.joints_data[15].velocity == <f32>(252.875));
assert(value.joints_data[15].kp == <f32>(250.5));
assert(value.joints_data[15].kd == <f32>(131.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("e3a26120c050a68900001e4200603ac300009cc2000088c200005dc3dd9c5f1efe3bd06800a032c300e0194300e055c300203cc300c087c2de9f5c1dfd800b4d00204a43006042c30020154300c0dcc200400b43e0a16223c3eb7dac004065c300c053c3008027c2000044c10000d14199d81b5abaf0bd06006079430000f0c10080a0420040804200e03543a7e62564849bcbe7000089420040dd4200202943008082c2006057c3a4e52667872010c200c040c300c04ec300e0104300401cc3000048439adb1859b94b662300e03243008016c300c097c2008010420000fbc1175695d43410909700001ec200804442000053c200c0384300604cc32968abea0a7be676000052420040f9c200c0ad4200408fc20000f841bafb387999c913220000e1c10000fe420000104000c070c30080e94284c50647a7a265c300c021c3002054c300c06a4300607ec300603b4387c60544a419bee60040f64200804f4200803043008061c200800042b9f83b7a9a72c807008000c3000055c30000f7c200003942000093c2c0814203e36908ad000035c3000001430000b3c100a007c30000a241febf7c3ddd027e4c000002420000e6410000714300201543008067c3");const codec=new r.Ros2DrddsMsgJointsDataValueCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.joints_data.length == 16);
assert(value.joints_data[0].name.length == 4);
assert(value.joints_data[0].name[0] == <u8>(227));
assert(value.joints_data[0].name[1] == <u8>(162));
assert(value.joints_data[0].name[2] == <u8>(97));
assert(value.joints_data[0].name[3] == <u8>(32));
assert(value.joints_data[0].data_id == <u16>(20672));
assert(value.joints_data[0].status_word == <u16>(35238));
assert(value.joints_data[0].position == <f32>(39.5));
assert(value.joints_data[0].torque == <f32>(-186.375));
assert(value.joints_data[0].velocity == <f32>(-78.0));
assert(value.joints_data[0].motion_temp == <f32>(-68.0));
assert(value.joints_data[0].driver_temp == <f32>(-221.0));
assert(value.joints_data[1].name.length == 4);
assert(value.joints_data[1].name[0] == <u8>(221));
assert(value.joints_data[1].name[1] == <u8>(156));
assert(value.joints_data[1].name[2] == <u8>(95));
assert(value.joints_data[1].name[3] == <u8>(30));
assert(value.joints_data[1].data_id == <u16>(15358));
assert(value.joints_data[1].status_word == <u16>(26832));
assert(value.joints_data[1].position == <f32>(-178.625));
assert(value.joints_data[1].torque == <f32>(153.875));
assert(value.joints_data[1].velocity == <f32>(-213.875));
assert(value.joints_data[1].motion_temp == <f32>(-188.125));
assert(value.joints_data[1].driver_temp == <f32>(-67.875));
assert(value.joints_data[2].name.length == 4);
assert(value.joints_data[2].name[0] == <u8>(222));
assert(value.joints_data[2].name[1] == <u8>(159));
assert(value.joints_data[2].name[2] == <u8>(92));
assert(value.joints_data[2].name[3] == <u8>(29));
assert(value.joints_data[2].data_id == <u16>(33021));
assert(value.joints_data[2].status_word == <u16>(19723));
assert(value.joints_data[2].position == <f32>(202.125));
assert(value.joints_data[2].torque == <f32>(-194.375));
assert(value.joints_data[2].velocity == <f32>(149.125));
assert(value.joints_data[2].motion_temp == <f32>(-110.375));
assert(value.joints_data[2].driver_temp == <f32>(139.25));
assert(value.joints_data[3].name.length == 4);
assert(value.joints_data[3].name[0] == <u8>(224));
assert(value.joints_data[3].name[1] == <u8>(161));
assert(value.joints_data[3].name[2] == <u8>(98));
assert(value.joints_data[3].name[3] == <u8>(35));
assert(value.joints_data[3].data_id == <u16>(60355));
assert(value.joints_data[3].status_word == <u16>(44157));
assert(value.joints_data[3].position == <f32>(-229.25));
assert(value.joints_data[3].torque == <f32>(-211.75));
assert(value.joints_data[3].velocity == <f32>(-41.875));
assert(value.joints_data[3].motion_temp == <f32>(-12.25));
assert(value.joints_data[3].driver_temp == <f32>(26.125));
assert(value.joints_data[4].name.length == 4);
assert(value.joints_data[4].name[0] == <u8>(153));
assert(value.joints_data[4].name[1] == <u8>(216));
assert(value.joints_data[4].name[2] == <u8>(27));
assert(value.joints_data[4].name[3] == <u8>(90));
assert(value.joints_data[4].data_id == <u16>(61626));
assert(value.joints_data[4].status_word == <u16>(1725));
assert(value.joints_data[4].position == <f32>(249.375));
assert(value.joints_data[4].torque == <f32>(-30.0));
assert(value.joints_data[4].velocity == <f32>(80.25));
assert(value.joints_data[4].motion_temp == <f32>(64.125));
assert(value.joints_data[4].driver_temp == <f32>(181.875));
assert(value.joints_data[5].name.length == 4);
assert(value.joints_data[5].name[0] == <u8>(167));
assert(value.joints_data[5].name[1] == <u8>(230));
assert(value.joints_data[5].name[2] == <u8>(37));
assert(value.joints_data[5].name[3] == <u8>(100));
assert(value.joints_data[5].data_id == <u16>(39812));
assert(value.joints_data[5].status_word == <u16>(59339));
assert(value.joints_data[5].position == <f32>(68.5));
assert(value.joints_data[5].torque == <f32>(110.625));
assert(value.joints_data[5].velocity == <f32>(169.125));
assert(value.joints_data[5].motion_temp == <f32>(-65.25));
assert(value.joints_data[5].driver_temp == <f32>(-215.375));
assert(value.joints_data[6].name.length == 4);
assert(value.joints_data[6].name[0] == <u8>(164));
assert(value.joints_data[6].name[1] == <u8>(229));
assert(value.joints_data[6].name[2] == <u8>(38));
assert(value.joints_data[6].name[3] == <u8>(103));
assert(value.joints_data[6].data_id == <u16>(8327));
assert(value.joints_data[6].status_word == <u16>(49680));
assert(value.joints_data[6].position == <f32>(-192.75));
assert(value.joints_data[6].torque == <f32>(-206.75));
assert(value.joints_data[6].velocity == <f32>(144.875));
assert(value.joints_data[6].motion_temp == <f32>(-156.25));
assert(value.joints_data[6].driver_temp == <f32>(200.0));
assert(value.joints_data[7].name.length == 4);
assert(value.joints_data[7].name[0] == <u8>(154));
assert(value.joints_data[7].name[1] == <u8>(219));
assert(value.joints_data[7].name[2] == <u8>(24));
assert(value.joints_data[7].name[3] == <u8>(89));
assert(value.joints_data[7].data_id == <u16>(19385));
assert(value.joints_data[7].status_word == <u16>(9062));
assert(value.joints_data[7].position == <f32>(178.875));
assert(value.joints_data[7].torque == <f32>(-150.5));
assert(value.joints_data[7].velocity == <f32>(-75.875));
assert(value.joints_data[7].motion_temp == <f32>(36.125));
assert(value.joints_data[7].driver_temp == <f32>(-31.375));
assert(value.joints_data[8].name.length == 4);
assert(value.joints_data[8].name[0] == <u8>(23));
assert(value.joints_data[8].name[1] == <u8>(86));
assert(value.joints_data[8].name[2] == <u8>(149));
assert(value.joints_data[8].name[3] == <u8>(212));
assert(value.joints_data[8].data_id == <u16>(4148));
assert(value.joints_data[8].status_word == <u16>(38800));
assert(value.joints_data[8].position == <f32>(-39.5));
assert(value.joints_data[8].torque == <f32>(49.125));
assert(value.joints_data[8].velocity == <f32>(-52.75));
assert(value.joints_data[8].motion_temp == <f32>(184.75));
assert(value.joints_data[8].driver_temp == <f32>(-204.375));
assert(value.joints_data[9].name.length == 4);
assert(value.joints_data[9].name[0] == <u8>(41));
assert(value.joints_data[9].name[1] == <u8>(104));
assert(value.joints_data[9].name[2] == <u8>(171));
assert(value.joints_data[9].name[3] == <u8>(234));
assert(value.joints_data[9].data_id == <u16>(31498));
assert(value.joints_data[9].status_word == <u16>(30438));
assert(value.joints_data[9].position == <f32>(52.5));
assert(value.joints_data[9].torque == <f32>(-124.625));
assert(value.joints_data[9].velocity == <f32>(86.875));
assert(value.joints_data[9].motion_temp == <f32>(-71.625));
assert(value.joints_data[9].driver_temp == <f32>(31.0));
assert(value.joints_data[10].name.length == 4);
assert(value.joints_data[10].name[0] == <u8>(186));
assert(value.joints_data[10].name[1] == <u8>(251));
assert(value.joints_data[10].name[2] == <u8>(56));
assert(value.joints_data[10].name[3] == <u8>(121));
assert(value.joints_data[10].data_id == <u16>(51609));
assert(value.joints_data[10].status_word == <u16>(8723));
assert(value.joints_data[10].position == <f32>(-28.125));
assert(value.joints_data[10].torque == <f32>(127.0));
assert(value.joints_data[10].velocity == <f32>(2.25));
assert(value.joints_data[10].motion_temp == <f32>(-240.75));
assert(value.joints_data[10].driver_temp == <f32>(116.75));
assert(value.joints_data[11].name.length == 4);
assert(value.joints_data[11].name[0] == <u8>(132));
assert(value.joints_data[11].name[1] == <u8>(197));
assert(value.joints_data[11].name[2] == <u8>(6));
assert(value.joints_data[11].name[3] == <u8>(71));
assert(value.joints_data[11].data_id == <u16>(41639));
assert(value.joints_data[11].status_word == <u16>(50021));
assert(value.joints_data[11].position == <f32>(-161.75));
assert(value.joints_data[11].torque == <f32>(-212.125));
assert(value.joints_data[11].velocity == <f32>(234.75));
assert(value.joints_data[11].motion_temp == <f32>(-254.375));
assert(value.joints_data[11].driver_temp == <f32>(187.375));
assert(value.joints_data[12].name.length == 4);
assert(value.joints_data[12].name[0] == <u8>(135));
assert(value.joints_data[12].name[1] == <u8>(198));
assert(value.joints_data[12].name[2] == <u8>(5));
assert(value.joints_data[12].name[3] == <u8>(68));
assert(value.joints_data[12].data_id == <u16>(6564));
assert(value.joints_data[12].status_word == <u16>(59070));
assert(value.joints_data[12].position == <f32>(123.125));
assert(value.joints_data[12].torque == <f32>(51.875));
assert(value.joints_data[12].velocity == <f32>(176.5));
assert(value.joints_data[12].motion_temp == <f32>(-56.375));
assert(value.joints_data[12].driver_temp == <f32>(32.125));
assert(value.joints_data[13].name.length == 4);
assert(value.joints_data[13].name[0] == <u8>(185));
assert(value.joints_data[13].name[1] == <u8>(248));
assert(value.joints_data[13].name[2] == <u8>(59));
assert(value.joints_data[13].name[3] == <u8>(122));
assert(value.joints_data[13].data_id == <u16>(29338));
assert(value.joints_data[13].status_word == <u16>(1992));
assert(value.joints_data[13].position == <f32>(-128.5));
assert(value.joints_data[13].torque == <f32>(-213.0));
assert(value.joints_data[13].velocity == <f32>(-123.5));
assert(value.joints_data[13].motion_temp == <f32>(46.25));
assert(value.joints_data[13].driver_temp == <f32>(-73.5));
assert(value.joints_data[14].name.length == 4);
assert(value.joints_data[14].name[0] == <u8>(192));
assert(value.joints_data[14].name[1] == <u8>(129));
assert(value.joints_data[14].name[2] == <u8>(66));
assert(value.joints_data[14].name[3] == <u8>(3));
assert(value.joints_data[14].data_id == <u16>(27107));
assert(value.joints_data[14].status_word == <u16>(44296));
assert(value.joints_data[14].position == <f32>(-181.0));
assert(value.joints_data[14].torque == <f32>(129.0));
assert(value.joints_data[14].velocity == <f32>(-22.375));
assert(value.joints_data[14].motion_temp == <f32>(-135.625));
assert(value.joints_data[14].driver_temp == <f32>(20.25));
assert(value.joints_data[15].name.length == 4);
assert(value.joints_data[15].name[0] == <u8>(254));
assert(value.joints_data[15].name[1] == <u8>(191));
assert(value.joints_data[15].name[2] == <u8>(124));
assert(value.joints_data[15].name[3] == <u8>(61));
assert(value.joints_data[15].data_id == <u16>(733));
assert(value.joints_data[15].status_word == <u16>(19582));
assert(value.joints_data[15].position == <f32>(32.5));
assert(value.joints_data[15].torque == <f32>(28.75));
assert(value.joints_data[15].velocity == <f32>(241.0));
assert(value.joints_data[15].motion_temp == <f32>(149.125));
assert(value.joints_data[15].driver_temp == <f32>(-231.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("66d84f42dfa178dcdeae18435aae5b12");const codec=new r.Ros2DrddsMsgMetaTypeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.frame_id == <u64><u64>0xdc78a1df424fd866);
assert(value.stamp.sec == <i32>(1125691102));
assert(value.stamp.nanosec == <u32>(307998298));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("f030a06c44301607070000007630623961ceb2070000007637356464ceb2070000007630316339ceb2070000007630393832ceb254070000007632366539ceb2d05e0000000000f04a400000000000d858400000000000603d4000000000002031400000000000485ec0000000000090404000f144aced483d9b73222429569b5d1ec800000000005057c000000000003862c000f9379ca02f2c4d31070000007633626561ceb201000000070000007666356635ceb2010000000000000000203a40010000000000000000e064400100000000000000004067c00000000000185e400000000000f045400000000000104fc0000000000000f8bf0000000000c056400000000000004cc0");const codec=new r.Ros2DrddsMsgRobotStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1822437616));
assert(value.header.stamp.nanosec == <u32>(118894660));
assert(value.header.frame_id == "v0b9aβ");
assert(value.robot == "v75ddβ");
assert(value.command_source == "v01c9β");
assert(value.current_state == "v0982β");
assert(value.current_state_id == <u8>(84));
assert(value.requested_state == "v26e9β");
assert(value.requested_state_id == <u8>(208));
assert(value.safety_mode == <u8>(94));
assert(value.command.linear.x == <f64>(53.875));
assert(value.command.linear.y == <f64>(99.375));
assert(value.command.linear.z == <f64>(29.375));
assert(value.command.angular.x == <f64>(17.125));
assert(value.command.angular.y == <f64>(-121.125));
assert(value.command.angular.z == <f64>(33.125));
assert(value.command_timed_out == false);
assert(value.rejected_commands == <u64><u64>0x739b3d48edac44f1);
assert(value.command_updates == <u64><u64>0xc81e5d9b56292422);
assert(value.joint_stamp == <f64>(-93.25));
assert(value.imu_stamp == <f64>(-145.75));
assert(value.telemetry_fresh == false);
assert(value.joints.header.stamp.sec == <i32>(-1600374791));
assert(value.joints.header.stamp.nanosec == <u32>(827141167));
assert(value.joints.header.frame_id == "v3beaβ");
assert(value.joints.name.length == 1);
assert(value.joints.name[0] == "vf5f5β");
assert(value.joints.position.length == 1);
assert(value.joints.position[0] == <f64>(26.125));
assert(value.joints.velocity.length == 1);
assert(value.joints.velocity[0] == <f64>(167.0));
assert(value.joints.effort.length == 1);
assert(value.joints.effort[0] == <f64>(-186.0));
assert(value.base_rpy.x == <f64>(120.375));
assert(value.base_rpy.y == <f64>(43.875));
assert(value.base_rpy.z == <f64>(-62.125));
assert(value.base_angular_velocity.x == <f64>(-1.5));
assert(value.base_angular_velocity.y == <f64>(91.0));
assert(value.base_angular_velocity.z == <f64>(-56.0));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("00602043");const codec=new r.Ros2DrddsMsgStdMsgFloat32Codec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <f32>(160.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("7dabbf97");const codec=new r.Ros2DrddsMsgStdMsgInt32Codec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(-1749046403));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("8473bbf2978fc928");const codec=new r.Ros2DrddsMsgStdStatusCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.state == <i32>(-222596220));
assert(value.error_code == <u32>(684298135));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_19():void{const input=bytes("9db8a64324c191dd2a6c76697bac900700003742000098c100c076c3000034420040cc42000016c2");const codec=new r.Ros2DrddsMsgSteerCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.frame_id == <u64><u64>0xdd91c12443a6b89d);
assert(value.header.stamp.sec == <i32>(1769368618));
assert(value.header.stamp.nanosec == <u32>(126921851));
assert(value.data.x == <f32>(45.75));
assert(value.data.y == <f32>(-19.0));
assert(value.data.z == <f32>(-246.75));
assert(value.data.roll == <f32>(45.0));
assert(value.data.pitch == <f32>(102.125));
assert(value.data.yaw == <f32>(-37.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_20():void{const input=bytes("000000c30080b5c2008026430040444300801d420000a7c2");const codec=new r.Ros2DrddsMsgSteerValueCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f32>(-128.0));
assert(value.y == <f32>(-90.75));
assert(value.z == <f32>(166.5));
assert(value.roll == <f32>(196.25));
assert(value.pitch == <f32>(39.375));
assert(value.yaw == <f32>(-83.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_21():void{const input=bytes("2332e082");const codec=new r.Ros2DrddsSrvStdSrvInt32_RequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.command == <i32>(-2099236317));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_22():void{const input=bytes("41ea5b24");const codec=new r.Ros2DrddsSrvStdSrvInt32_ResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.result == <i32>(610003521));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_23():void{const input=bytes("0000000000e053c00000000000f063c00000000000306cc00000000000a06d4000000000000c60c00000000000906840");const codec=new r.Ros2Geometry_msgsMsgTwistCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.linear.x == <f64>(-79.5));
assert(value.linear.y == <f64>(-159.5));
assert(value.linear.z == <f64>(-225.5));
assert(value.angular.x == <f64>(237.0));
assert(value.angular.y == <f64>(-128.375));
assert(value.angular.z == <f64>(196.5));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_24():void{const input=bytes("0000000000546c400000000000e062c00000000000246a40");const codec=new r.Ros2Geometry_msgsMsgVector3Codec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.x == <f64>(226.625));
assert(value.y == <f64>(-151.0));
assert(value.z == <f64>(209.125));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_25():void{const input=bytes("735aeca75614571f070000007632363433ceb201000000070000007663343031ceb2010000000000000000f05e40010000000000000000b46440010000000000000000ac64c0");const codec=new r.Ros2Sensor_msgsMsgJointStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-1477682573));
assert(value.header.stamp.nanosec == <u32>(525800534));
assert(value.header.frame_id == "v2643β");
assert(value.name.length == 1);
assert(value.name[0] == "vc401β");
assert(value.position.length == 1);
assert(value.position[0] == <f64>(123.75));
assert(value.velocity.length == 1);
assert(value.velocity[0] == <f64>(165.625));
assert(value.effort.length == 1);
assert(value.effort[0] == <f64>(-165.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_26():void{const input=bytes("681252e4acc1ba37070000007666316532ceb2");const codec=new r.Ros2Std_msgsMsgHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
}
