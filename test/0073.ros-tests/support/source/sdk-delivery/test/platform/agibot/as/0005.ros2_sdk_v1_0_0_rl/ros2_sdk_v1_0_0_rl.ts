/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../../../../sdk/include/platform/agibot/as/ros2/agibot-ros2-sdk-v1-0-0-rl-complete/bounded_records";
import {abortGuest} from "../../../../../../../../../../sdk/include/platform/ros/as/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("16bb4308af59a921");const codec=new r.Ros2Aimdk_msgsMsgCommonRequestCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(138656534));
assert(value.header.stamp.nanosec == <u32>(564746671));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_1():void{const input=bytes("7e48d16d857af630efb3162656ca21b86ea99a05070000007634616561ceb2");const codec=new r.Ros2Aimdk_msgsMsgCommonResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(1842432126));
assert(value.header.stamp.nanosec == <u32>(821459589));
assert(value.header.code == <i64><u64>0xb821ca562616b3ef);
assert(value.status.value == <i32>(94021998));
assert(value.message == "v4aeaβ");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_2():void{const input=bytes("d226915e");const codec=new r.Ros2Aimdk_msgsMsgCommonStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <i32>(1586570962));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_3():void{const input=bytes("0e92b1f4b82af9317ff79a4fc68eadd1888295fa31fba264f00f1460");const codec=new r.Ros2Aimdk_msgsMsgCommonTaskResponseCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-189689330));
assert(value.header.stamp.nanosec == <u32>(838412984));
assert(value.header.code == <i64><u64>0xd1ad8ec64f9af77f);
assert(value.task_id == <u64><u64>0x64a2fb31fa958288);
assert(value.state.value == <i32>(1611927536));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_4():void{const input=bytes("e1");const codec=new r.Ros2Aimdk_msgsMsgDomainErrorStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.value == <u8>(225));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_5():void{const input=bytes("070000007666396635ceb2000000000030644000000000006059c000000000006856400000000000b4654000000000003043c0");const codec=new r.Ros2Aimdk_msgsMsgJointCommandCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vf9f5β");
assert(value.position == <f64>(161.5));
assert(value.velocity == <f64>(-101.5));
assert(value.effort == <f64>(89.625));
assert(value.stiffness == <f64>(173.625));
assert(value.damping == <f64>(-38.375));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_6():void{const input=bytes("c4e2ccfa36661122070000007636656530ceb2c5a6c503155c6d32fa7a343001000000070000007631646364ceb200000000002855c00000000000fc62400000000000886dc0000000000028644000000000004866c0");const codec=new r.Ros2Aimdk_msgsMsgJointCommandArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_7():void{const input=bytes("070000007634386539ceb200000000008041c00000000000bc64400000000000bc6540cce1");const codec=new r.Ros2Aimdk_msgsMsgJointStateCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "v48e9β");
assert(value.position == <f64>(-35.0));
assert(value.velocity == <f64>(165.875));
assert(value.effort == <f64>(173.875));
assert(value.error_code == <u16>(57804));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_8():void{const input=bytes("1952db78f8dfe310070000007637316134ceb293ba811c5be6a64ea09f3a2a7401000000070000007638333532ceb20000000000c855c000000000003867400000000000c038c02b6c");const codec=new r.Ros2Aimdk_msgsMsgJointStateArrayCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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

function test_9():void{const input=bytes("5455aec10663d513070000007636323737ceb2e5fe520f50fd2f14d8db8710");const codec=new r.Ros2Aimdk_msgsMsgMessageHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(-1045539500));
assert(value.stamp.nanosec == <u32>(332751622));
assert(value.frame_id == "v6277β");
assert(value.sequence == <u32>(257097445));
assert(value.meas_stamp.sec == <i32>(338689360));
assert(value.meas_stamp.nanosec == <u32>(277339096));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_10():void{const input=bytes("070000007661353766ceb2070000007637373832ceb20000000000306f4000000000002c6b400000000000fc6c400000000000c0594084977d530000000000306dc0000101000000070000007665343437ceb2");const codec=new r.Ros2Aimdk_msgsMsgModuleChannelInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "va57fβ");
assert(value.ros_type == "v7782β");
assert(value.frequency == <f64>(249.5));
assert(value.min_time_diff == <f64>(217.375));
assert(value.max_time_diff == <f64>(231.875));
assert(value.std_dev == <f64>(103.0));
assert(value.window_size == <i32>(1400739716));
assert(value.timeout_threshold == <f64>(-233.5));
assert(value.is_active == false);
assert(value.is_timeout == true);
assert(value.backends.length == 1);
assert(value.backends[0] == "ve447β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_11():void{const input=bytes("070000007637383665ceb201000000070000007635323338ceb2070000007638653130ceb2070000007663336438ceb201000000070000007638663834ceb201000000070000007665356532ceb2070000007664666565ceb2070000007639323236ceb201000000070000007666393431ceb201000000070000007637303332ceb2070000007661326366ceb2000000000000634000000000000856c0000000000000444000000000004064c00bfaca6000000000009065c0000001000000070000007664376630ceb201000000070000007664663962ceb2070000007630643636ceb20000000000b85b400000000000486ac000000000004867c000000000006468c0c8d75ed300000000001461c0010001000000070000007636343634ceb2");const codec=new r.Ros2Aimdk_msgsMsgModuleResourceInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.module_name == "v786eβ");
assert(value.rpc_clients.length == 1);
assert(value.rpc_clients[0].name == "v5238β");
assert(value.rpc_clients[0].req_type == "v8e10β");
assert(value.rpc_clients[0].res_type == "vc3d8β");
assert(value.rpc_clients[0].backends.length == 1);
assert(value.rpc_clients[0].backends[0] == "v8f84β");
assert(value.rpc_services.length == 1);
assert(value.rpc_services[0].name == "ve5e2β");
assert(value.rpc_services[0].req_type == "vdfeeβ");
assert(value.rpc_services[0].res_type == "v9226β");
assert(value.rpc_services[0].backends.length == 1);
assert(value.rpc_services[0].backends[0] == "vf941β");
assert(value.pub_channels.length == 1);
assert(value.pub_channels[0].name == "v7032β");
assert(value.pub_channels[0].ros_type == "va2cfβ");
assert(value.pub_channels[0].frequency == <f64>(152.0));
assert(value.pub_channels[0].min_time_diff == <f64>(-88.125));
assert(value.pub_channels[0].max_time_diff == <f64>(40.0));
assert(value.pub_channels[0].std_dev == <f64>(-162.0));
assert(value.pub_channels[0].window_size == <i32>(1623915019));
assert(value.pub_channels[0].timeout_threshold == <f64>(-172.5));
assert(value.pub_channels[0].is_active == false);
assert(value.pub_channels[0].is_timeout == false);
assert(value.pub_channels[0].backends.length == 1);
assert(value.pub_channels[0].backends[0] == "vd7f0β");
assert(value.sub_channels.length == 1);
assert(value.sub_channels[0].name == "vdf9bβ");
assert(value.sub_channels[0].ros_type == "v0d66β");
assert(value.sub_channels[0].frequency == <f64>(110.875));
assert(value.sub_channels[0].min_time_diff == <f64>(-210.25));
assert(value.sub_channels[0].max_time_diff == <f64>(-186.25));
assert(value.sub_channels[0].std_dev == <f64>(-195.125));
assert(value.sub_channels[0].window_size == <i32>(-748759096));
assert(value.sub_channels[0].timeout_threshold == <f64>(-136.625));
assert(value.sub_channels[0].is_active == true);
assert(value.sub_channels[0].is_timeout == false);
assert(value.sub_channels[0].backends.length == 1);
assert(value.sub_channels[0].backends[0] == "v6464β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_12():void{const input=bytes("070000007666383631ceb2070000007664323333ceb2070000007639666662ceb201000000070000007638323634ceb2");const codec=new r.Ros2Aimdk_msgsMsgModuleRpcInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.name == "vf861β");
assert(value.req_type == "vd233β");
assert(value.res_type == "v9ffbβ");
assert(value.backends.length == 1);
assert(value.backends[0] == "v8264β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_13():void{const input=bytes("679360fe46e90229070000007631396237ceb243109274aa1f6762d1da2301070000007662383664ceb27ffc42c3c685755d070000007633373363ceb2255e887a9c27bfe40000000000c02cc00000000000685bc051bc9c97e8c5ab0901000000e216f9b45b6fce2a070000007662333063ceb200000000003c6e40070000007632353439ceb28521267701000000682dd7e701000000070000007664333964ceb201000000070000007630333162ceb2070000007664386236ceb2070000007639353765ceb201000000070000007665376238ceb201000000070000007637323631ceb2070000007634656463ceb2070000007630333134ceb201000000070000007637633463ceb201000000070000007665376231ceb2070000007633353463ceb20000000000c02ec000000000001854400000000000e050400000000000c04f40fff6c7e50000000000a46140000001000000070000007635326664ceb201000000070000007634383138ceb2070000007639616535ceb200000000001460c00000000000104bc000000000005c65c000000000003065c03cdb535600000000005c6ac0010001000000070000007665313639ceb2");const codec=new r.Ros2Aimdk_msgsMsgProcessInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.header.stamp.sec == <i32>(-27225241));
assert(value.header.stamp.nanosec == <u32>(688056646));
assert(value.header.frame_id == "v19b7β");
assert(value.header.sequence == <u32>(1955729475));
assert(value.header.meas_stamp.sec == <i32>(1650925482));
assert(value.header.meas_stamp.nanosec == <u32>(19126993));
assert(value.app_name == "vb86dβ");
assert(value.resource.process_id == <u64><u64>0x5d7585c6c342fc7f);
assert(value.resource.process_name == "v373cβ");
assert(value.resource.process_mem_usage == <u64><u64>0xe4bf279c7a885e25);
assert(value.resource.process_mem_usage_ratio == <f64>(-14.375));
assert(value.resource.process_cpu_usage_ratio == <f64>(-109.625));
assert(value.resource.thread_count == <u64><u64>0x09abc5e8979cbc51);
assert(value.resource.threads.length == 1);
assert(value.resource.threads[0].thread_id == <u64><u64>0x2ace6f5bb4f916e2);
assert(value.resource.threads[0].thread_name == "vb30cβ");
assert(value.resource.threads[0].cpu_usage_ratio == <f64>(241.875));
assert(value.resource.threads[0].cpu_sched_policy == "v2549β");
assert(value.resource.threads[0].cpu_sched_priority == <i32>(1998987653));
assert(value.resource.threads[0].bounding_cpus.length == 1);
assert(value.resource.threads[0].bounding_cpus[0] == <i32>(-405328536));
assert(value.modules.length == 1);
assert(value.modules[0].module_name == "vd39dβ");
assert(value.modules[0].rpc_clients.length == 1);
assert(value.modules[0].rpc_clients[0].name == "v031bβ");
assert(value.modules[0].rpc_clients[0].req_type == "vd8b6β");
assert(value.modules[0].rpc_clients[0].res_type == "v957eβ");
assert(value.modules[0].rpc_clients[0].backends.length == 1);
assert(value.modules[0].rpc_clients[0].backends[0] == "ve7b8β");
assert(value.modules[0].rpc_services.length == 1);
assert(value.modules[0].rpc_services[0].name == "v7261β");
assert(value.modules[0].rpc_services[0].req_type == "v4edcβ");
assert(value.modules[0].rpc_services[0].res_type == "v0314β");
assert(value.modules[0].rpc_services[0].backends.length == 1);
assert(value.modules[0].rpc_services[0].backends[0] == "v7c4cβ");
assert(value.modules[0].pub_channels.length == 1);
assert(value.modules[0].pub_channels[0].name == "ve7b1β");
assert(value.modules[0].pub_channels[0].ros_type == "v354cβ");
assert(value.modules[0].pub_channels[0].frequency == <f64>(-15.375));
assert(value.modules[0].pub_channels[0].min_time_diff == <f64>(80.375));
assert(value.modules[0].pub_channels[0].max_time_diff == <f64>(67.5));
assert(value.modules[0].pub_channels[0].std_dev == <f64>(63.5));
assert(value.modules[0].pub_channels[0].window_size == <i32>(-439879937));
assert(value.modules[0].pub_channels[0].timeout_threshold == <f64>(141.125));
assert(value.modules[0].pub_channels[0].is_active == false);
assert(value.modules[0].pub_channels[0].is_timeout == false);
assert(value.modules[0].pub_channels[0].backends.length == 1);
assert(value.modules[0].pub_channels[0].backends[0] == "v52fdβ");
assert(value.modules[0].sub_channels.length == 1);
assert(value.modules[0].sub_channels[0].name == "v4818β");
assert(value.modules[0].sub_channels[0].ros_type == "v9ae5β");
assert(value.modules[0].sub_channels[0].frequency == <f64>(-128.625));
assert(value.modules[0].sub_channels[0].min_time_diff == <f64>(-54.125));
assert(value.modules[0].sub_channels[0].max_time_diff == <f64>(-170.875));
assert(value.modules[0].sub_channels[0].std_dev == <f64>(-169.5));
assert(value.modules[0].sub_channels[0].window_size == <i32>(1448336188));
assert(value.modules[0].sub_channels[0].timeout_threshold == <f64>(-210.875));
assert(value.modules[0].sub_channels[0].is_active == true);
assert(value.modules[0].sub_channels[0].is_timeout == false);
assert(value.modules[0].sub_channels[0].backends.length == 1);
assert(value.modules[0].sub_channels[0].backends[0] == "ve169β");
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_14():void{const input=bytes("59d85f80e0a1681e070000007663386531ceb243b1095bfac83ec500000000003058400000000000b866c0b998416800e176f601000000c4fdfabb7d84cd25070000007634373061ceb20000000000806740070000007665613634ceb2d34bc9980100000009a0fa28");const codec=new r.Ros2Aimdk_msgsMsgProcessResourceInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.process_id == <u64><u64>0x1e68a1e0805fd859);
assert(value.process_name == "vc8e1β");
assert(value.process_mem_usage == <u64><u64>0xc53ec8fa5b09b143);
assert(value.process_mem_usage_ratio == <f64>(96.75));
assert(value.process_cpu_usage_ratio == <f64>(-181.75));
assert(value.thread_count == <u64><u64>0xf676e100684198b9);
assert(value.threads.length == 1);
assert(value.threads[0].thread_id == <u64><u64>0x25cd847dbbfafdc4);
assert(value.threads[0].thread_name == "v470aβ");
assert(value.threads[0].cpu_usage_ratio == <f64>(188.0));
assert(value.threads[0].cpu_sched_policy == "vea64β");
assert(value.threads[0].cpu_sched_priority == <i32>(-1731638317));
assert(value.threads[0].bounding_cpus.length == 1);
assert(value.threads[0].bounding_cpus[0] == <i32>(687513609));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_15():void{const input=bytes("7ec27b1a2f62b81e");const codec=new r.Ros2Aimdk_msgsMsgRequestHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(444318334));
assert(value.stamp.nanosec == <u32>(515400239));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_16():void{const input=bytes("55c42dd4c7edd33a863659073f4f6e99");const codec=new r.Ros2Aimdk_msgsMsgResponseHeaderCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.stamp.sec == <i32>(-735198123));
assert(value.stamp.nanosec == <u32>(986967495));
assert(value.code == <i64><u64>0x996e4f3f07593686);
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_17():void{const input=bytes("8aea70843393471a070000007661613233ceb20000000000406e40070000007662333964ceb2b0abad2101000000cadc0371");const codec=new r.Ros2Aimdk_msgsMsgThreadResourceInfoCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.thread_id == <u64><u64>0x1a4793338470ea8a);
assert(value.thread_name == "vaa23β");
assert(value.cpu_usage_ratio == <f64>(242.0));
assert(value.cpu_sched_policy == "vb39dβ");
assert(value.cpu_sched_priority == <i32>(565029808));
assert(value.bounding_cpus.length == 1);
assert(value.bounding_cpus[0] == <i32>(1896078538));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

function test_18():void{const input=bytes("072b89b550057009");const codec=new r.Ros2Builtin_interfacesMsgTimeCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
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
}
