/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/agibot-ros2-sdk-v1-0-0-rl-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("16bb4308af59a921"),codec=r.Ros2Aimdk_msgsMsgCommonRequestCodec,value=codec.decode(input);
check(value.header.stamp.sec === 138656534);
check(value.header.stamp.nanosec === 564746671);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("7e48d16d857af630efb3162656ca21b86ea99a05070000007634616561ceb2"),codec=r.Ros2Aimdk_msgsMsgCommonResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === 1842432126);
check(value.header.stamp.nanosec === 821459589);
check(value.header.code === -5178635625144339473n);
check(value.status.value === 94021998);
check(value.message === "v4aeaβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes("d226915e"),codec=r.Ros2Aimdk_msgsMsgCommonStateCodec,value=codec.decode(input);
check(value.value === 1586570962);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("0e92b1f4b82af9317ff79a4fc68eadd1888295fa31fba264f00f1460"),codec=r.Ros2Aimdk_msgsMsgCommonTaskResponseCodec,value=codec.decode(input);
check(value.header.stamp.sec === -189689330);
check(value.header.stamp.nanosec === 838412984);
check(value.header.code === -3337854766421444737n);
check(value.task_id === 7251634542095991432n);
check(value.state.value === 1611927536);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("e1"),codec=r.Ros2Aimdk_msgsMsgDomainErrorStateCodec,value=codec.decode(input);
check(value.value === 225);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("070000007666396635ceb2000000000030644000000000006059c000000000006856400000000000b4654000000000003043c0"),codec=r.Ros2Aimdk_msgsMsgJointCommandCodec,value=codec.decode(input);
check(value.name === "vf9f5β");
check(value.position === 161.5);
check(value.velocity === -101.5);
check(value.effort === 89.625);
check(value.stiffness === 173.625);
check(value.damping === -38.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("c4e2ccfa36661122070000007636656530ceb2c5a6c503155c6d32fa7a343001000000070000007631646364ceb200000000002855c00000000000fc62400000000000886dc0000000000028644000000000004866c0"),codec=r.Ros2Aimdk_msgsMsgJointCommandArrayCodec,value=codec.decode(input);
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

function test_7(){const input=bytes("070000007634386539ceb200000000008041c00000000000bc64400000000000bc6540cce1"),codec=r.Ros2Aimdk_msgsMsgJointStateCodec,value=codec.decode(input);
check(value.name === "v48e9β");
check(value.position === -35.0);
check(value.velocity === 165.875);
check(value.effort === 173.875);
check(value.error_code === 57804);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("1952db78f8dfe310070000007637316134ceb293ba811c5be6a64ea09f3a2a7401000000070000007638333532ceb20000000000c855c000000000003867400000000000c038c02b6c"),codec=r.Ros2Aimdk_msgsMsgJointStateArrayCodec,value=codec.decode(input);
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

function test_9(){const input=bytes("5455aec10663d513070000007636323737ceb2e5fe520f50fd2f14d8db8710"),codec=r.Ros2Aimdk_msgsMsgMessageHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -1045539500);
check(value.stamp.nanosec === 332751622);
check(value.frame_id === "v6277β");
check(value.sequence === 257097445);
check(value.meas_stamp.sec === 338689360);
check(value.meas_stamp.nanosec === 277339096);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("070000007661353766ceb2070000007637373832ceb20000000000306f4000000000002c6b400000000000fc6c400000000000c0594084977d530000000000306dc0000101000000070000007665343437ceb2"),codec=r.Ros2Aimdk_msgsMsgModuleChannelInfoCodec,value=codec.decode(input);
check(value.name === "va57fβ");
check(value.type === "v7782β");
check(value.frequency === 249.5);
check(value.min_time_diff === 217.375);
check(value.max_time_diff === 231.875);
check(value.std_dev === 103.0);
check(value.window_size === 1400739716);
check(value.timeout_threshold === -233.5);
check(value.is_active === false);
check(value.is_timeout === true);
check(value.backends.length == 1);
check(value.backends[0] === "ve447β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes("070000007637383665ceb201000000070000007635323338ceb2070000007638653130ceb2070000007663336438ceb201000000070000007638663834ceb201000000070000007665356532ceb2070000007664666565ceb2070000007639323236ceb201000000070000007666393431ceb201000000070000007637303332ceb2070000007661326366ceb2000000000000634000000000000856c0000000000000444000000000004064c00bfaca6000000000009065c0000001000000070000007664376630ceb201000000070000007664663962ceb2070000007630643636ceb20000000000b85b400000000000486ac000000000004867c000000000006468c0c8d75ed300000000001461c0010001000000070000007636343634ceb2"),codec=r.Ros2Aimdk_msgsMsgModuleResourceInfoCodec,value=codec.decode(input);
check(value.module_name === "v786eβ");
check(value.rpc_clients.length == 1);
check(value.rpc_clients[0].name === "v5238β");
check(value.rpc_clients[0].req_type === "v8e10β");
check(value.rpc_clients[0].res_type === "vc3d8β");
check(value.rpc_clients[0].backends.length == 1);
check(value.rpc_clients[0].backends[0] === "v8f84β");
check(value.rpc_services.length == 1);
check(value.rpc_services[0].name === "ve5e2β");
check(value.rpc_services[0].req_type === "vdfeeβ");
check(value.rpc_services[0].res_type === "v9226β");
check(value.rpc_services[0].backends.length == 1);
check(value.rpc_services[0].backends[0] === "vf941β");
check(value.pub_channels.length == 1);
check(value.pub_channels[0].name === "v7032β");
check(value.pub_channels[0].type === "va2cfβ");
check(value.pub_channels[0].frequency === 152.0);
check(value.pub_channels[0].min_time_diff === -88.125);
check(value.pub_channels[0].max_time_diff === 40.0);
check(value.pub_channels[0].std_dev === -162.0);
check(value.pub_channels[0].window_size === 1623915019);
check(value.pub_channels[0].timeout_threshold === -172.5);
check(value.pub_channels[0].is_active === false);
check(value.pub_channels[0].is_timeout === false);
check(value.pub_channels[0].backends.length == 1);
check(value.pub_channels[0].backends[0] === "vd7f0β");
check(value.sub_channels.length == 1);
check(value.sub_channels[0].name === "vdf9bβ");
check(value.sub_channels[0].type === "v0d66β");
check(value.sub_channels[0].frequency === 110.875);
check(value.sub_channels[0].min_time_diff === -210.25);
check(value.sub_channels[0].max_time_diff === -186.25);
check(value.sub_channels[0].std_dev === -195.125);
check(value.sub_channels[0].window_size === -748759096);
check(value.sub_channels[0].timeout_threshold === -136.625);
check(value.sub_channels[0].is_active === true);
check(value.sub_channels[0].is_timeout === false);
check(value.sub_channels[0].backends.length == 1);
check(value.sub_channels[0].backends[0] === "v6464β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("070000007666383631ceb2070000007664323333ceb2070000007639666662ceb201000000070000007638323634ceb2"),codec=r.Ros2Aimdk_msgsMsgModuleRpcInfoCodec,value=codec.decode(input);
check(value.name === "vf861β");
check(value.req_type === "vd233β");
check(value.res_type === "v9ffbβ");
check(value.backends.length == 1);
check(value.backends[0] === "v8264β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes("679360fe46e90229070000007631396237ceb243109274aa1f6762d1da2301070000007662383664ceb27ffc42c3c685755d070000007633373363ceb2255e887a9c27bfe40000000000c02cc00000000000685bc051bc9c97e8c5ab0901000000e216f9b45b6fce2a070000007662333063ceb200000000003c6e40070000007632353439ceb28521267701000000682dd7e701000000070000007664333964ceb201000000070000007630333162ceb2070000007664386236ceb2070000007639353765ceb201000000070000007665376238ceb201000000070000007637323631ceb2070000007634656463ceb2070000007630333134ceb201000000070000007637633463ceb201000000070000007665376231ceb2070000007633353463ceb20000000000c02ec000000000001854400000000000e050400000000000c04f40fff6c7e50000000000a46140000001000000070000007635326664ceb201000000070000007634383138ceb2070000007639616535ceb200000000001460c00000000000104bc000000000005c65c000000000003065c03cdb535600000000005c6ac0010001000000070000007665313639ceb2"),codec=r.Ros2Aimdk_msgsMsgProcessInfoCodec,value=codec.decode(input);
check(value.header.stamp.sec === -27225241);
check(value.header.stamp.nanosec === 688056646);
check(value.header.frame_id === "v19b7β");
check(value.header.sequence === 1955729475);
check(value.header.meas_stamp.sec === 1650925482);
check(value.header.meas_stamp.nanosec === 19126993);
check(value.app_name === "vb86dβ");
check(value.resource.process_id === 6734435906528410751n);
check(value.resource.process_name === "v373cβ");
check(value.resource.process_mem_usage === 16482936714223443493n);
check(value.resource.process_mem_usage_ratio === -14.375);
check(value.resource.process_cpu_usage_ratio === -109.625);
check(value.resource.thread_count === 696868170125589585n);
check(value.resource.threads.length == 1);
check(value.resource.threads[0].thread_id === 3084525234464298722n);
check(value.resource.threads[0].thread_name === "vb30cβ");
check(value.resource.threads[0].cpu_usage_ratio === 241.875);
check(value.resource.threads[0].cpu_sched_policy === "v2549β");
check(value.resource.threads[0].cpu_sched_priority === 1998987653);
check(value.resource.threads[0].bounding_cpus.length == 1);
check(value.resource.threads[0].bounding_cpus[0] === -405328536);
check(value.modules.length == 1);
check(value.modules[0].module_name === "vd39dβ");
check(value.modules[0].rpc_clients.length == 1);
check(value.modules[0].rpc_clients[0].name === "v031bβ");
check(value.modules[0].rpc_clients[0].req_type === "vd8b6β");
check(value.modules[0].rpc_clients[0].res_type === "v957eβ");
check(value.modules[0].rpc_clients[0].backends.length == 1);
check(value.modules[0].rpc_clients[0].backends[0] === "ve7b8β");
check(value.modules[0].rpc_services.length == 1);
check(value.modules[0].rpc_services[0].name === "v7261β");
check(value.modules[0].rpc_services[0].req_type === "v4edcβ");
check(value.modules[0].rpc_services[0].res_type === "v0314β");
check(value.modules[0].rpc_services[0].backends.length == 1);
check(value.modules[0].rpc_services[0].backends[0] === "v7c4cβ");
check(value.modules[0].pub_channels.length == 1);
check(value.modules[0].pub_channels[0].name === "ve7b1β");
check(value.modules[0].pub_channels[0].type === "v354cβ");
check(value.modules[0].pub_channels[0].frequency === -15.375);
check(value.modules[0].pub_channels[0].min_time_diff === 80.375);
check(value.modules[0].pub_channels[0].max_time_diff === 67.5);
check(value.modules[0].pub_channels[0].std_dev === 63.5);
check(value.modules[0].pub_channels[0].window_size === -439879937);
check(value.modules[0].pub_channels[0].timeout_threshold === 141.125);
check(value.modules[0].pub_channels[0].is_active === false);
check(value.modules[0].pub_channels[0].is_timeout === false);
check(value.modules[0].pub_channels[0].backends.length == 1);
check(value.modules[0].pub_channels[0].backends[0] === "v52fdβ");
check(value.modules[0].sub_channels.length == 1);
check(value.modules[0].sub_channels[0].name === "v4818β");
check(value.modules[0].sub_channels[0].type === "v9ae5β");
check(value.modules[0].sub_channels[0].frequency === -128.625);
check(value.modules[0].sub_channels[0].min_time_diff === -54.125);
check(value.modules[0].sub_channels[0].max_time_diff === -170.875);
check(value.modules[0].sub_channels[0].std_dev === -169.5);
check(value.modules[0].sub_channels[0].window_size === 1448336188);
check(value.modules[0].sub_channels[0].timeout_threshold === -210.875);
check(value.modules[0].sub_channels[0].is_active === true);
check(value.modules[0].sub_channels[0].is_timeout === false);
check(value.modules[0].sub_channels[0].backends.length == 1);
check(value.modules[0].sub_channels[0].backends[0] === "ve169β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("59d85f80e0a1681e070000007663386531ceb243b1095bfac83ec500000000003058400000000000b866c0b998416800e176f601000000c4fdfabb7d84cd25070000007634373061ceb20000000000806740070000007665613634ceb2d34bc9980100000009a0fa28"),codec=r.Ros2Aimdk_msgsMsgProcessResourceInfoCodec,value=codec.decode(input);
check(value.process_id === 2191179204314257497n);
check(value.process_name === "vc8e1β");
check(value.process_mem_usage === 14213018451622605123n);
check(value.process_mem_usage_ratio === 96.75);
check(value.process_cpu_usage_ratio === -181.75);
check(value.thread_count === 17759629572447508665n);
check(value.threads.length == 1);
check(value.threads[0].thread_id === 2723979025188584900n);
check(value.threads[0].thread_name === "v470aβ");
check(value.threads[0].cpu_usage_ratio === 188.0);
check(value.threads[0].cpu_sched_policy === "vea64β");
check(value.threads[0].cpu_sched_priority === -1731638317);
check(value.threads[0].bounding_cpus.length == 1);
check(value.threads[0].bounding_cpus[0] === 687513609);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("7ec27b1a2f62b81e"),codec=r.Ros2Aimdk_msgsMsgRequestHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === 444318334);
check(value.stamp.nanosec === 515400239);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_16(){const input=bytes("55c42dd4c7edd33a863659073f4f6e99"),codec=r.Ros2Aimdk_msgsMsgResponseHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -735198123);
check(value.stamp.nanosec === 986967495);
check(value.code === -7390882806343584122n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("8aea70843393471a070000007661613233ceb20000000000406e40070000007662333964ceb2b0abad2101000000cadc0371"),codec=r.Ros2Aimdk_msgsMsgThreadResourceInfoCodec,value=codec.decode(input);
check(value.thread_id === 1893644017807190666n);
check(value.thread_name === "vaa23β");
check(value.cpu_usage_ratio === 242.0);
check(value.cpu_sched_policy === "vb39dβ");
check(value.cpu_sched_priority === 565029808);
check(value.bounding_cpus.length == 1);
check(value.bounding_cpus[0] === 1896078538);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes("072b89b550057009"),codec=r.Ros2Builtin_interfacesMsgTimeCodec,value=codec.decode(input);
check(value.sec === -1249301753);
check(value.nanosec === 158336336);
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
return 0;
}
