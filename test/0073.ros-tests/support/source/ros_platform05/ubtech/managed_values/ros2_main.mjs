/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/ubtech-ros2-main-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("072b89b550057009"),codec=r.Ros2Builtin_interfacesMsgTimeCodec,value=codec.decode(input);
check(value.sec === -1249301753);
check(value.nanosec === 158336336);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("0000000000e86dc00000000000d054c000000000002042c0"),codec=r.Ros2Geometry_msgsMsgPointCodec,value=codec.decode(input);
check(value.x === -239.25);
check(value.y === -83.25);
check(value.z === -36.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes("0000000000c02ec000000000003047c00000000000203d4000000000004857c000000000007049400000000000986dc00000000000585340"),codec=r.Ros2Geometry_msgsMsgPoseCodec,value=codec.decode(input);
check(value.position.x === -15.375);
check(value.position.y === -46.375);
check(value.position.z === 29.125);
check(value.orientation.x === -93.125);
check(value.orientation.y === 50.875);
check(value.orientation.z === -236.75);
check(value.orientation.w === 77.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("72e71f7fe898242b070000007664386435ceb200000000003854c000000000006c66c00000000000f8654000000000001869c00000000000b06dc00000000000d053c00000000000e85dc0"),codec=r.Ros2Geometry_msgsMsgPoseStampedCodec,value=codec.decode(input);
check(value.header.stamp.sec === 2132797298);
check(value.header.stamp.nanosec === 723818728);
check(value.header.frame_id === "vd8d5β");
check(value.pose.position.x === -80.875);
check(value.pose.position.y === -179.375);
check(value.pose.position.z === 175.75);
check(value.pose.orientation.x === -200.75);
check(value.pose.orientation.y === -237.5);
check(value.pose.orientation.z === -79.25);
check(value.pose.orientation.w === -119.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("0000000000a85cc00000000000046dc00000000000d045c00000000000005340"),codec=r.Ros2Geometry_msgsMsgQuaternionCodec,value=codec.decode(input);
check(value.x === -114.625);
check(value.y === -232.125);
check(value.z === -43.625);
check(value.w === 76.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("01172a7241d6691a070000007638303466ceb2aa39858f285bdb17070000007634643938ceb200000000005859400000000000d041c000000000005853400000000000e046c000000000000033400000000000fc6bc00000000000106440070000007662333937ceb200202643070000007634626166ceb2"),codec=r.Ros2Grab_demo_msgsMsgGraspCandidateCodec,value=codec.decode(input);
check(value.header.stamp.sec === 1915361025);
check(value.header.stamp.nanosec === 443143745);
check(value.header.frame_id === "v804fβ");
check(value.pose.header.stamp.sec === -1887094358);
check(value.pose.header.stamp.nanosec === 400251688);
check(value.pose.header.frame_id === "v4d98β");
check(value.pose.pose.position.x === 101.375);
check(value.pose.pose.position.y === -35.625);
check(value.pose.pose.position.z === 77.375);
check(value.pose.pose.orientation.x === -45.75);
check(value.pose.pose.orientation.y === 19.0);
check(value.pose.pose.orientation.z === -223.875);
check(value.pose.pose.orientation.w === 160.5);
check(value.group_name === "vb397β");
check(value.confidence === 166.125);
check(value.object_class === "v4bafβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("070000007663323866ceb2070000007634613137ceb20000000000604d4000000000004044c00000000000d05240000000000020344000000000002031c00000000000bc64c00000000000cc6d40"),codec=r.Ros2Grab_demo_msgsMsgIKRequestCodec,value=codec.decode(input);
check(value.group_name === "vc28fβ");
check(value.frame_id === "v4a17β");
check(value.position.x === 58.75);
check(value.position.y === -40.5);
check(value.position.z === 75.25);
check(value.orientation.x === 20.125);
check(value.orientation.y === -17.125);
check(value.orientation.z === -165.875);
check(value.orientation.w === 238.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("681252e4acc1ba37070000007666316532ceb2"),codec=r.Ros2Std_msgsMsgHeaderCodec,value=codec.decode(input);
check(value.stamp.sec === -464383384);
check(value.stamp.nanosec === 934986156);
check(value.frame_id === "vf1e2β");
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
return 0;
}
