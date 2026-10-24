/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/pal-ros1-humble-devel-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("41a82588c71e9838"),codec=r.Ros1DurationCodec,value=codec.decode(input);
check(value.sec === -2010798015);
check(value.nsec === 949493447);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("0000000000e46dc00000000000ec69c00000000000746fc00000000000ac66400000000000b4604000000000001c6640"),codec=r.Ros1Geometry_msgsAccelCodec,value=codec.decode(input);
check(value.linear.x === -239.125);
check(value.linear.y === -207.375);
check(value.linear.z === -251.625);
check(value.angular.x === 181.375);
check(value.angular.y === 133.625);
check(value.angular.z === 176.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes("0000000000002ec000000000000048c00000000000803f40"),codec=r.Ros1Geometry_msgsPointCodec,value=codec.decode(input);
check(value.x === -15.0);
check(value.y === -48.0);
check(value.z === 31.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("00c09ac200a015c300c04f43"),codec=r.Ros1Geometry_msgsPoint32Codec,value=codec.decode(input);
check(value.x === -77.375);
check(value.y === -149.625);
check(value.z === 207.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes("31da0840484793488cde3e1b070000007638633465ceb20000000000b869c000000000004066c00000000000b05fc0"),codec=r.Ros1Geometry_msgsPointStampedCodec,value=codec.decode(input);
check(value.header.seq === 1074321969);
check(value.header.stamp.sec === 1217611592);
check(value.header.stamp.nsec === 457105036);
check(value.header.frame_id === "v8c4eβ");
check(value.point.x === -205.75);
check(value.point.y === -178.0);
check(value.point.z === -126.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("0000000000905e40000000000000334000000000002865400000000000803dc0000000000080354000000000007061c000000000008c65c0"),codec=r.Ros1Geometry_msgsPoseCodec,value=codec.decode(input);
check(value.position.x === 122.25);
check(value.position.y === 19.0);
check(value.position.z === 169.25);
check(value.orientation.x === -29.5);
check(value.orientation.y === 21.5);
check(value.orientation.z === -139.5);
check(value.orientation.w === -172.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("00000000004060c00000000000d058c00000000000b069c0"),codec=r.Ros1Geometry_msgsPose2DCodec,value=codec.decode(input);
check(value.x === -130.0);
check(value.y === -99.25);
check(value.theta === -205.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("4b5798c4344f20f0939eaf18070000007638326533ceb20100000000000000002467c00000000000046bc00000000000005b400000000000905bc000000000001067c00000000000985a4000000000006038c0"),codec=r.Ros1Geometry_msgsPoseArrayCodec,value=codec.decode(input);
check(value.header.seq === 3298318155);
check(value.header.stamp.sec === 4028649268);
check(value.header.stamp.nsec === 414162579);
check(value.header.frame_id === "v82e3β");
check(value.poses.length == 1);
check(value.poses[0].position.x === -185.125);
check(value.poses[0].position.y === -216.125);
check(value.poses[0].position.z === 108.0);
check(value.poses[0].orientation.x === -110.25);
check(value.poses[0].orientation.y === -184.5);
check(value.poses[0].orientation.z === 106.375);
check(value.poses[0].orientation.w === -24.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("0b83eee805dc0efc3a3e7823070000007631613662ceb200000000009868400000000000a468c00000000000bc64c00000000000486340000000000030524000000000006865c000000000002855c0"),codec=r.Ros1Geometry_msgsPoseStampedCodec,value=codec.decode(input);
check(value.header.seq === 3907945227);
check(value.header.stamp.sec === 4228832261);
check(value.header.stamp.nsec === 595082810);
check(value.header.frame_id === "v1a6bβ");
check(value.pose.position.x === 196.75);
check(value.pose.position.y === -197.125);
check(value.pose.position.z === -165.875);
check(value.pose.orientation.x === 154.25);
check(value.pose.orientation.y === 72.75);
check(value.pose.orientation.z === -171.25);
check(value.pose.orientation.w === -84.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_9(){const input=bytes("000000000000e4bf00000000000c62c00000000000846bc0000000000080444000000000000045400000000000106b40000000000000d03f0000000000e85d400000000000206a4000000000003c61400000000000c0384000000000002463400000000000a46dc000000000005052c000000000004c63c00000000000a85bc00000000000d05bc00000000000785fc00000000000403fc000000000005c6b400000000000e060400000000000405cc00000000000402b4000000000006866c00000000000e46cc000000000005c68c00000000000486cc00000000000404dc0000000000000f6bf00000000006866400000000000b8594000000000000010c000000000004855400000000000cc6d40000000000040684000000000004021c000000000006053c00000000000402ac0000000000020514000000000006061c000000000007c6dc00000000000fc6ac000000000009059c0"),codec=r.Ros1Geometry_msgsPoseWithCovarianceCodec,value=codec.decode(input);
check(value.pose.position.x === -0.625);
check(value.pose.position.y === -144.375);
check(value.pose.position.z === -220.125);
check(value.pose.orientation.x === 41.0);
check(value.pose.orientation.y === 42.0);
check(value.pose.orientation.z === 216.5);
check(value.pose.orientation.w === 0.25);
check(value.covariance.length == 36);
check(value.covariance[0] === 119.625);
check(value.covariance[1] === 209.0);
check(value.covariance[2] === 137.875);
check(value.covariance[3] === 24.75);
check(value.covariance[4] === 153.125);
check(value.covariance[5] === -237.125);
check(value.covariance[6] === -73.25);
check(value.covariance[7] === -154.375);
check(value.covariance[8] === -110.625);
check(value.covariance[9] === -111.25);
check(value.covariance[10] === -125.875);
check(value.covariance[11] === -31.25);
check(value.covariance[12] === 218.875);
check(value.covariance[13] === 135.0);
check(value.covariance[14] === -113.0);
check(value.covariance[15] === 13.625);
check(value.covariance[16] === -179.25);
check(value.covariance[17] === -231.125);
check(value.covariance[18] === -194.875);
check(value.covariance[19] === -226.25);
check(value.covariance[20] === -58.5);
check(value.covariance[21] === -1.375);
check(value.covariance[22] === 179.25);
check(value.covariance[23] === 102.875);
check(value.covariance[24] === -4.0);
check(value.covariance[25] === 85.125);
check(value.covariance[26] === 238.375);
check(value.covariance[27] === 194.0);
check(value.covariance[28] === -8.625);
check(value.covariance[29] === -77.5);
check(value.covariance[30] === -13.125);
check(value.covariance[31] === 68.5);
check(value.covariance[32] === -139.0);
check(value.covariance[33] === -235.875);
check(value.covariance[34] === -215.875);
check(value.covariance[35] === -102.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("8431bd7b9987025f0e371c28070000007661613330ceb2000000000078544000000000007046c00000000000046dc00000000000d853400000000000a854c00000000000f0434000000000001c6f4000000000007044c000000000000010c000000000008c624000000000001c68c00000000000486240000000000020374000000000008035c000000000004026c000000000002067400000000000f86bc00000000000e46bc000000000004069c00000000000c022c00000000000286240000000000038604000000000004022c00000000000c04ec00000000000b850c00000000000e05e400000000000546a4000000000000053400000000000c856400000000000f867400000000000905fc00000000000c044c000000000004c67c000000000003855400000000000a05d400000000000e03d400000000000c05b400000000000385f4000000000001064400000000000805dc0000000000090494000000000007c6ec00000000000ec6140"),codec=r.Ros1Geometry_msgsPoseWithCovarianceStampedCodec,value=codec.decode(input);
check(value.header.seq === 2075996548);
check(value.header.stamp.sec === 1594001305);
check(value.header.stamp.nsec === 672937742);
check(value.header.frame_id === "vaa30β");
check(value.pose.pose.position.x === 81.875);
check(value.pose.pose.position.y === -44.875);
check(value.pose.pose.position.z === -232.125);
check(value.pose.pose.orientation.x === 79.375);
check(value.pose.pose.orientation.y === -82.625);
check(value.pose.pose.orientation.z === 39.875);
check(value.pose.pose.orientation.w === 248.875);
check(value.pose.covariance.length == 36);
check(value.pose.covariance[0] === -40.875);
check(value.pose.covariance[1] === -4.0);
check(value.pose.covariance[2] === 148.375);
check(value.pose.covariance[3] === -192.875);
check(value.pose.covariance[4] === 146.25);
check(value.pose.covariance[5] === 23.125);
check(value.pose.covariance[6] === -21.5);
check(value.pose.covariance[7] === -11.125);
check(value.pose.covariance[8] === 185.0);
check(value.pose.covariance[9] === -223.75);
check(value.pose.covariance[10] === -223.125);
check(value.pose.covariance[11] === -202.0);
check(value.pose.covariance[12] === -9.375);
check(value.pose.covariance[13] === 145.25);
check(value.pose.covariance[14] === 129.75);
check(value.pose.covariance[15] === -9.125);
check(value.pose.covariance[16] === -61.5);
check(value.pose.covariance[17] === -66.875);
check(value.pose.covariance[18] === 123.5);
check(value.pose.covariance[19] === 210.625);
check(value.pose.covariance[20] === 76.0);
check(value.pose.covariance[21] === 91.125);
check(value.pose.covariance[22] === 191.75);
check(value.pose.covariance[23] === -126.25);
check(value.pose.covariance[24] === -41.5);
check(value.pose.covariance[25] === -186.375);
check(value.pose.covariance[26] === 84.875);
check(value.pose.covariance[27] === 118.5);
check(value.pose.covariance[28] === 29.875);
check(value.pose.covariance[29] === 111.0);
check(value.pose.covariance[30] === 124.875);
check(value.pose.covariance[31] === 160.5);
check(value.pose.covariance[32] === -118.0);
check(value.pose.covariance[33] === 51.125);
check(value.pose.covariance[34] === -243.875);
check(value.pose.covariance[35] === 143.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes("0000000000905040000000000008624000000000005c6fc000000000003051c0"),codec=r.Ros1Geometry_msgsQuaternionCodec,value=codec.decode(input);
check(value.x === 66.25);
check(value.y === 144.25);
check(value.z === -250.875);
check(value.w === -68.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("0000000000e46d4000000000006c6b4000000000000029c000000000001855400000000000e035400000000000d462c00000000000504b40"),codec=r.Ros1Geometry_msgsTransformCodec,value=codec.decode(input);
check(value.translation.x === 239.125);
check(value.translation.y === 219.375);
check(value.translation.z === -12.5);
check(value.rotation.x === 84.375);
check(value.rotation.y === 21.875);
check(value.rotation.z === -150.625);
check(value.rotation.w === 54.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes("e83eb6c1ae23acbda5ac1729070000007662636565ceb2070000007639643537ceb200000000004c6b400000000000b46e4000000000001059c00000000000a854c00000000000e03a400000000000485ac000000000001867c0"),codec=r.Ros1Geometry_msgsTransformStampedCodec,value=codec.decode(input);
check(value.header.seq === 3249946344);
check(value.header.stamp.sec === 3182175150);
check(value.header.stamp.nsec === 689417381);
check(value.header.frame_id === "vbceeβ");
check(value.child_frame_id === "v9d57β");
check(value.transform.translation.x === 218.375);
check(value.transform.translation.y === 245.625);
check(value.transform.translation.z === -100.25);
check(value.transform.rotation.x === -82.625);
check(value.transform.rotation.y === 26.875);
check(value.transform.rotation.z === -105.125);
check(value.transform.rotation.w === -184.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("0000000000785140000000000000c03f0000000000985e4000000000009045400000000000885d400000000000385cc0"),codec=r.Ros1Geometry_msgsTwistCodec,value=codec.decode(input);
check(value.linear.x === 69.875);
check(value.linear.y === 0.125);
check(value.linear.z === 122.375);
check(value.angular.x === 43.125);
check(value.angular.y === 118.125);
check(value.angular.z === -112.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("00000000002850c00000000000b04cc000000000009461c0"),codec=r.Ros1Geometry_msgsVector3Codec,value=codec.decode(input);
check(value.x === -64.625);
check(value.y === -57.375);
check(value.z === -140.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_16(){const input=bytes("0000000000a85b4000000000003040c00000000000346240000000000000014000000000001c63400000000000f061c0"),codec=r.Ros1Geometry_msgsWrenchCodec,value=codec.decode(input);
check(value.force.x === 110.625);
check(value.force.y === -32.375);
check(value.force.z === 145.625);
check(value.torque.x === 2.125);
check(value.torque.y === 152.875);
check(value.torque.z === -143.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("0000000000d46fc000000000002467c000000000004046c09e"),codec=r.Ros1Humanoid_nav_msgsStepTargetCodec,value=codec.decode(input);
check(value.pose.x === -254.625);
check(value.pose.y === -185.125);
check(value.pose.theta === -44.5);
check(value.leg === 158);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes("7490fd23dc02a00a00e022438cf1ea7ee9e760ba0000000000c866400000000000646ec00000000000f859c00000000000d064400000000000706f400000000000a85dc00000000000206940"),codec=r.Ros1Nav_msgsMapMetaDataCodec,value=codec.decode(input);
check(value.map_load_time.sec === 603820148);
check(value.map_load_time.nsec === 178258652);
check(value.resolution === 162.875);
check(value.width === 2129326476);
check(value.height === 3126913001);
check(value.origin.position.x === 182.25);
check(value.origin.position.y === -243.125);
check(value.origin.position.z === -103.875);
check(value.origin.orientation.x === 166.5);
check(value.origin.orientation.y === 251.5);
check(value.origin.orientation.z === -118.625);
check(value.origin.orientation.w === 201.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_19(){const input=bytes("09e71d88eade7001d1bfaf33070000007630363930ceb26679eeff7d387a120000b1c277642f68e809199000000000001c6fc00000000000206e400000000000a043c000000000001056c00000000000002dc000000000001861400000000000cc66c001000000af"),codec=r.Ros1Nav_msgsOccupancyGridCodec,value=codec.decode(input);
check(value.header.seq === 2283661065);
check(value.header.stamp.sec === 24174314);
check(value.header.stamp.nsec === 867155921);
check(value.header.frame_id === "v0690β");
check(value.info.map_load_time.sec === 4293818726);
check(value.info.map_load_time.nsec === 309999741);
check(value.info.resolution === -88.5);
check(value.info.width === 1747936375);
check(value.info.height === 2417560040);
check(value.info.origin.position.x === -248.875);
check(value.info.origin.position.y === 241.0);
check(value.info.origin.position.z === -39.25);
check(value.info.origin.orientation.x === -88.25);
check(value.info.origin.orientation.y === -14.5);
check(value.info.origin.orientation.z === 136.75);
check(value.info.origin.orientation.w === -182.375);
check(value.data.length == 1);
check(value.data[0] === -81);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_20(){const input=bytes("070000007661323936ceb2070000007632313633ceb2"),codec=r.Ros1Pal_behaviour_msgsBehaviourArgumentCodec,value=codec.decode(input);
check(value.key === "va296β");
check(value.value === "v2163β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_21(){const input=bytes("070000007632323164ceb201000000070000007630663366ceb2070000007637393665ceb2"),codec=r.Ros1Pal_behaviour_msgsBehaviourEventRequestCodec,value=codec.decode(input);
check(value.name === "v221dβ");
check(value.arguments.length == 1);
check(value.arguments[0].key === "v0f3fβ");
check(value.arguments[0].value === "v796eβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_22(){const input=bytes("070000007634646535ceb2"),codec=r.Ros1Pal_behaviour_msgsBehaviourEventResponseCodec,value=codec.decode(input);
check(value.response === "v4de5β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_23(){const input=bytes(""),codec=r.Ros1Pal_behaviour_msgsBehaviourTaskFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_24(){const input=bytes("070000007630353634ceb201000000070000007630346535ceb2070000007665373334ceb2"),codec=r.Ros1Pal_behaviour_msgsBehaviourTaskGoalCodec,value=codec.decode(input);
check(value.taskName === "v0564β");
check(value.arguments.length == 1);
check(value.arguments[0].key === "v04e5β");
check(value.arguments[0].value === "ve734β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_25(){const input=bytes(""),codec=r.Ros1Pal_behaviour_msgsBehaviourTaskResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_26(){const input=bytes(""),codec=r.Ros1Pal_behaviour_msgsPresentationFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_27(){const input=bytes("070000007630363735ceb2"),codec=r.Ros1Pal_behaviour_msgsPresentationGoalCodec,value=codec.decode(input);
check(value.presentationFile === "v0675β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_28(){const input=bytes(""),codec=r.Ros1Pal_behaviour_msgsPresentationResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_29(){const input=bytes(""),codec=r.Ros1Pal_common_msgsDisableFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_30(){const input=bytes("0040b0c2"),codec=r.Ros1Pal_common_msgsDisableGoalCodec,value=codec.decode(input);
check(value.duration === -88.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_31(){const input=bytes(""),codec=r.Ros1Pal_common_msgsDisableResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_32(){const input=bytes(""),codec=r.Ros1Pal_common_msgsEmptyFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_33(){const input=bytes(""),codec=r.Ros1Pal_common_msgsEmptyGoalCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_34(){const input=bytes(""),codec=r.Ros1Pal_common_msgsEmptyResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_35(){const input=bytes(""),codec=r.Ros1Pal_common_msgsGetStringRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_36(){const input=bytes("070000007666363666ceb201"),codec=r.Ros1Pal_common_msgsGetStringResponseCodec,value=codec.decode(input);
check(value.data === "vf66fβ");
check(value.success === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_37(){const input=bytes("070000007637396139ceb200202cc3"),codec=r.Ros1Pal_common_msgsJointCurrentCodec,value=codec.decode(input);
check(value.joints === "v79a9β");
check(value.current_limit === -172.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_38(){const input=bytes("070000007663366662ceb2"),codec=r.Ros1Pal_common_msgsSetStringRequestCodec,value=codec.decode(input);
check(value.data === "vc6fbβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_39(){const input=bytes("00"),codec=r.Ros1Pal_common_msgsSetStringResponseCodec,value=codec.decode(input);
check(value.success === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_40(){const input=bytes("01000000070000007665666433ceb20100000000000000000027c0"),codec=r.Ros1Pal_control_msgsActuatorCurrentLimitCodec,value=codec.decode(input);
check(value.actuator_names.length == 1);
check(value.actuator_names[0] === "vefd3β");
check(value.current_limits.length == 1);
check(value.current_limits[0] === -11.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_41(){const input=bytes("070000007661663462ceb200000000009c6840"),codec=r.Ros1Pal_control_msgsCurrentLimitRequestCodec,value=codec.decode(input);
check(value.actuator_name === "vaf4bβ");
check(value.current_limit === 196.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_42(){const input=bytes("070000007638646431ceb2"),codec=r.Ros1Pal_control_msgsCurrentLimitResponseCodec,value=codec.decode(input);
check(value.status === "v8dd1β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_43(){const input=bytes(""),codec=r.Ros1Pal_control_msgsMotionManagerFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_44(){const input=bytes("070000007663316438ceb2010001012bd17d9c"),codec=r.Ros1Pal_control_msgsMotionManagerGoalCodec,value=codec.decode(input);
check(value.filename === "vc1d8β");
check(value.plan === true);
check(value.checkSafety === false);
check(value.repeat === true);
check(value.priority === 1);
check(value.queueTimeout === -1669476053);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_45(){const input=bytes("070000007633633565ceb2"),codec=r.Ros1Pal_control_msgsMotionManagerResultCodec,value=codec.decode(input);
check(value.message === "v3c5eβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_46(){const input=bytes("6fd1ae4f3a1e9b6ac56a5d2c070000007638363031ceb20001bce986752751992800000000004463c00000000000c85ac00000000000f466c000000000000012400000000000404ec00000000000206bc000000000009462c00000000000f46e400000000000cc6f40000000000034654000000000002866c000000000006060c00000000000e04f400000000000d461c00000000000801dc000000000000c60400000000000005a400000000000406a4000000000007467c0"),codec=r.Ros1Pal_control_msgsOperationalSpaceGoalCodec,value=codec.decode(input);
check(value.header.seq === 1336856943);
check(value.header.stamp.sec === 1788550714);
check(value.header.stamp.nsec === 744319685);
check(value.header.frame_id === "v8601β");
check(value.has_vel === false);
check(value.has_accel === true);
check(value.duration.sec === 1971775932);
check(value.duration.nsec === 681136423);
check(value.pose.position.x === -154.125);
check(value.pose.position.y === -107.125);
check(value.pose.position.z === -183.625);
check(value.pose.orientation.x === 4.5);
check(value.pose.orientation.y === -60.5);
check(value.pose.orientation.z === -217.0);
check(value.pose.orientation.w === -148.625);
check(value.velocity.linear.x === 247.625);
check(value.velocity.linear.y === 254.375);
check(value.velocity.linear.z === 169.625);
check(value.velocity.angular.x === -177.25);
check(value.velocity.angular.y === -131.0);
check(value.velocity.angular.z === 63.75);
check(value.acceleration.linear.x === -142.625);
check(value.acceleration.linear.y === -7.375);
check(value.acceleration.linear.z === 128.375);
check(value.acceleration.angular.x === 104.0);
check(value.acceleration.angular.y === 210.0);
check(value.acceleration.angular.z === -187.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_47(){const input=bytes(""),codec=r.Ros1Pal_control_msgsRigidBodyTrajectoryFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_48(){const input=bytes("e5564d68b5fbbe2f91078515070000007637636238ceb29d01000000edde607672d6f63400000000007c6f4000000000002860c000000000008020400000000000dc6cc0000000000048614000000000007053c00000000000446ac00000000000d466c000000000002466c00000000000085fc00000000000246cc00000000000b85fc00000000000c0214000000000001050400000000000f0634000000000005c6ac000000000008039c000000000001860c000000000000008c0"),codec=r.Ros1Pal_control_msgsRigidBodyTrajectoryGoalCodec,value=codec.decode(input);
check(value.header.seq === 1749898981);
check(value.header.stamp.sec === 801045429);
check(value.header.stamp.nsec === 361039761);
check(value.header.frame_id === "v7cb8β");
check(value.interpolation_type === -99);
check(value.trajectory.length == 1);
check(value.trajectory[0].duration.sec === 1986060013);
check(value.trajectory[0].duration.nsec === 888591986);
check(value.trajectory[0].pose.position.x === 251.875);
check(value.trajectory[0].pose.position.y === -129.25);
check(value.trajectory[0].pose.position.z === 8.25);
check(value.trajectory[0].pose.orientation.x === -230.875);
check(value.trajectory[0].pose.orientation.y === 138.25);
check(value.trajectory[0].pose.orientation.z === -77.75);
check(value.trajectory[0].pose.orientation.w === -210.125);
check(value.trajectory[0].velocity.linear.x === -182.625);
check(value.trajectory[0].velocity.linear.y === -177.125);
check(value.trajectory[0].velocity.linear.z === -124.125);
check(value.trajectory[0].velocity.angular.x === -225.125);
check(value.trajectory[0].velocity.angular.y === -126.875);
check(value.trajectory[0].velocity.angular.z === 8.875);
check(value.trajectory[0].acceleration.linear.x === 64.25);
check(value.trajectory[0].acceleration.linear.y === 159.5);
check(value.trajectory[0].acceleration.linear.z === -210.875);
check(value.trajectory[0].acceleration.angular.x === -25.5);
check(value.trajectory[0].acceleration.angular.y === -128.75);
check(value.trajectory[0].acceleration.angular.z === -3.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_49(){const input=bytes("9312887685ce150c00000000006863400000000000386d4000000000002868400000000000a06d400000000000c0674000000000008051c000000000003c62c00000000000f850c00000000000246fc00000000000a865400000000000dc6dc000000000004859c00000000000504d400000000000fc69c00000000000d856c000000000009042400000000000d462400000000000906ac00000000000e46d40"),codec=r.Ros1Pal_control_msgsRigidBodyTrajectoryPointCodec,value=codec.decode(input);
check(value.duration.sec === 1988629139);
check(value.duration.nsec === 202755717);
check(value.pose.position.x === 155.25);
check(value.pose.position.y === 233.75);
check(value.pose.position.z === 193.25);
check(value.pose.orientation.x === 237.0);
check(value.pose.orientation.y === 190.0);
check(value.pose.orientation.z === -70.0);
check(value.pose.orientation.w === -145.875);
check(value.velocity.linear.x === -67.875);
check(value.velocity.linear.y === -249.125);
check(value.velocity.linear.z === 173.25);
check(value.velocity.angular.x === -238.875);
check(value.velocity.angular.y === -101.125);
check(value.velocity.angular.z === 58.625);
check(value.acceleration.linear.x === -207.875);
check(value.acceleration.linear.y === -91.375);
check(value.acceleration.linear.z === 37.125);
check(value.acceleration.angular.x === 150.625);
check(value.acceleration.angular.y === -212.5);
check(value.acceleration.angular.z === 239.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_50(){const input=bytes("070000007663646331ceb2"),codec=r.Ros1Pal_control_msgsRigidBodyTrajectoryResultCodec,value=codec.decode(input);
check(value.message === "vcdc1β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_51(){const input=bytes("e589302f39f09bc891dd2503070000007665313965ceb2b0a940f52ffa5681070000007637343766ceb2aab6c06c6201000000a4070000007664643964ceb2"),codec=r.Ros1Pal_detection_msgsAddTexturedObjectRequestCodec,value=codec.decode(input);
check(value.img.header.seq === 791710181);
check(value.img.header.stamp.sec === 3365662777);
check(value.img.header.stamp.nsec === 52813201);
check(value.img.header.frame_id === "ve19eβ");
check(value.img.height === 4114655664);
check(value.img.width === 2169961007);
check(value.img.encoding === "v747fβ");
check(value.img.is_bigendian === 170);
check(value.img.step === 1651294390);
check(value.img.data.length == 1);
check(value.img.data[0] === 164);
check(value.fullPathFileName === "vdd9dβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_52(){const input=bytes("00"),codec=r.Ros1Pal_detection_msgsAddTexturedObjectResponseCodec,value=codec.decode(input);
check(value.result === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_53(){const input=bytes("070000007662303463ceb200"),codec=r.Ros1Pal_detection_msgsChangeObjectRecognizerModelRequestCodec,value=codec.decode(input);
check(value.model_name === "vb04cβ");
check(value.reset_desired_classes_param === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_54(){const input=bytes("00"),codec=r.Ros1Pal_detection_msgsChangeObjectRecognizerModelResponseCodec,value=codec.decode(input);
check(value.status === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_55(){const input=bytes("e38123f35af8146d75b12484ccc8131a87b8233e3ec114a0c0b2e60179cbd19f"),codec=r.Ros1Pal_detection_msgsDetection2dCodec,value=codec.decode(input);
check(value.x === 7860180319178293731n);
check(value.y === 1879066250259509621n);
check(value.width === -6911687055032207225n);
check(value.height === -6930534630963105088n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_56(){const input=bytes("21463d81267b2b08ef863e38070000007665623739ceb2010000009fee4ed62697794809de49a1b0a77e3f13ea16c7aa932159cfd07df676a94a68431449c43c31c89ddf3ef912070000007662373739ceb2070000007664316139ceb20000000000806dc00000000000746940000000000080124000000000000004c00000000000f05b4000000000004048c00000000000fc6ac0"),codec=r.Ros1Pal_detection_msgsDetections2dCodec,value=codec.decode(input);
check(value.header.seq === 2168276513);
check(value.header.stamp.sec === 137067302);
check(value.header.stamp.nsec === 943621871);
check(value.header.frame_id === "veb79β");
check(value.detections.length == 1);
check(value.detections[0].x === 5222371435972849311n);
check(value.detections[0].y === 4575278648517058057n);
check(value.detections[0].width === 6422576905300929043n);
check(value.detections[0].height === 7515005256627769551n);
check(value.camera_pose.header.seq === 3293123651);
check(value.camera_pose.header.stamp.sec === 2647142716);
check(value.camera_pose.header.stamp.nsec === 318324447);
check(value.camera_pose.header.frame_id === "vb779β");
check(value.camera_pose.child_frame_id === "vd1a9β");
check(value.camera_pose.transform.translation.x === -236.0);
check(value.camera_pose.transform.translation.y === 203.625);
check(value.camera_pose.transform.translation.z === 4.625);
check(value.camera_pose.transform.rotation.x === -2.5);
check(value.camera_pose.transform.rotation.y === 111.75);
check(value.camera_pose.transform.rotation.z === -48.5);
check(value.camera_pose.transform.rotation.w === -215.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_57(){const input=bytes("d6bb6ae5408b6d924ea49827203653cf017e9f016ee8af061992eac7b804dac0cf00200cc3002019c300800743070000007630646666ceb2008031c2070000007666643539ceb200005743070000007662646533ceb20000614200c059430000174300401e4300c0524300a03e4300408fc2008042c3"),codec=r.Ros1Pal_detection_msgsFaceDetectionCodec,value=codec.decode(input);
check(value.x === -445989930);
check(value.y === -1838314688);
check(value.width === 664314958);
check(value.height === -816630240);
check(value.eyesLocated === true);
check(value.leftEyeX === 1845600126);
check(value.leftEyeY === 419868648);
check(value.rightEyeX === -1194857838);
check(value.rightEyeY === -809444860);
check(value.position.x === -140.125);
check(value.position.y === -153.125);
check(value.position.z === 135.5);
check(value.name === "v0dffβ");
check(value.confidence === -44.375);
check(value.gender === "vfd59β");
check(value.genderConfidence === 215.0);
check(value.expression === "vbde3β");
check(value.expression_confidence === 56.25);
check(value.emotion_anger_confidence === 217.75);
check(value.emotion_disgust_confidence === 151.0);
check(value.emotion_fear_confidence === 158.25);
check(value.emotion_happiness_confidence === 210.75);
check(value.emotion_neutral_confidence === 190.625);
check(value.emotion_sadness_confidence === -71.625);
check(value.emotion_surprise_confidence === -194.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_58(){const input=bytes("0de3ca355fd66419da00f611070000007661356466ceb201000000a447a7633277a014fbc9ff204e165f7400934211ae057216d98a64bc661c54bb1100e00fc30040ffc200c05f43070000007630373536ceb20000dc41070000007634363535ceb2000054c1070000007661653531ceb20080164300007041000049c2008012c30000ba41002054430040c8c200a01fc3419240be0977c8cabac4f71b070000007664663861ceb2070000007666663061ceb2000000000050544000000000000862400000000000b05fc00000000000504fc000000000005467c00000000000c066400000000000446e40"),codec=r.Ros1Pal_detection_msgsFaceDetectionsCodec,value=codec.decode(input);
check(value.header.seq === 902488845);
check(value.header.stamp.sec === 426038879);
check(value.header.stamp.nsec === 301334746);
check(value.header.frame_id === "va5dfβ");
check(value.faces.length == 1);
check(value.faces[0].x === 1671907236);
check(value.faces[0].y === 346060594);
check(value.faces[0].width === 553634299);
check(value.faces[0].height === 1952388686);
check(value.faces[0].eyesLocated === false);
check(value.faces[0].leftEyeX === -1374600557);
check(value.faces[0].leftEyeY === -652840443);
check(value.faces[0].rightEyeX === 1723622538);
check(value.faces[0].rightEyeY === 297489436);
check(value.faces[0].position.x === -143.875);
check(value.faces[0].position.y === -127.625);
check(value.faces[0].position.z === 223.75);
check(value.faces[0].name === "v0756β");
check(value.faces[0].confidence === 27.5);
check(value.faces[0].gender === "v4655β");
check(value.faces[0].genderConfidence === -13.25);
check(value.faces[0].expression === "vae51β");
check(value.faces[0].expression_confidence === 150.5);
check(value.faces[0].emotion_anger_confidence === 15.0);
check(value.faces[0].emotion_disgust_confidence === -50.25);
check(value.faces[0].emotion_fear_confidence === -146.5);
check(value.faces[0].emotion_happiness_confidence === 23.25);
check(value.faces[0].emotion_neutral_confidence === 212.125);
check(value.faces[0].emotion_sadness_confidence === -100.125);
check(value.faces[0].emotion_surprise_confidence === -159.625);
check(value.camera_pose.header.seq === 3191902785);
check(value.camera_pose.header.stamp.sec === 3402135305);
check(value.camera_pose.header.stamp.nsec === 469222586);
check(value.camera_pose.header.frame_id === "vdf8aβ");
check(value.camera_pose.child_frame_id === "vff0aβ");
check(value.camera_pose.transform.translation.x === 81.25);
check(value.camera_pose.transform.translation.y === 144.25);
check(value.camera_pose.transform.translation.z === -126.75);
check(value.camera_pose.transform.rotation.x === -62.625);
check(value.camera_pose.transform.rotation.y === -186.625);
check(value.camera_pose.transform.rotation.z === 182.0);
check(value.camera_pose.transform.rotation.w === 242.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_59(){const input=bytes("a354da58297d8388789fcb30070000007632363138ceb20100000033c8e353adc8499f00f798f7ec409b3400"),codec=r.Ros1Pal_detection_msgsFaceThermalDetectionsCodec,value=codec.decode(input);
check(value.header.seq === 1490703523);
check(value.header.stamp.sec === 2290318633);
check(value.header.stamp.nsec === 818651000);
check(value.header.frame_id === "v2618β");
check(value.faces.length == 1);
check(value.faces[0].x_offset === 1407436851);
check(value.faces[0].y_offset === 2672412845);
check(value.faces[0].height === 4153997056);
check(value.faces[0].width === 882589932);
check(value.faces[0].do_rectify === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_60(){const input=bytes("0f8167d3b33e30a5619c2606070000007639376234ceb2070000007664636230ceb20000000000246ac000000000006865400000000000b462c0"),codec=r.Ros1Pal_detection_msgsGestureCodec,value=codec.decode(input);
check(value.header.seq === 3546775823);
check(value.header.stamp.sec === 2771402419);
check(value.header.stamp.nsec === 103193697);
check(value.header.frame_id === "v97b4β");
check(value.gestureId === "vdcb0β");
check(value.position3D.x === -209.125);
check(value.position3D.y === 171.25);
check(value.position3D.z === -149.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_61(){const input=bytes("cee08f3f4cede5683f2c041e070000007639353631ceb2010000000000000000603e400000000000b046c000000000006858c0ab6283125cfab9814cf83111070000007661616433ceb2070000007663636262ceb20000000000002dc00000000000e056c00000000000e06dc00000000000c0544000000000006066400000000000406b400000000000205d40"),codec=r.Ros1Pal_detection_msgsLegDetectionsCodec,value=codec.decode(input);
check(value.header.seq === 1066393806);
check(value.header.stamp.sec === 1759898956);
check(value.header.stamp.nsec === 503589951);
check(value.header.frame_id === "v9561β");
check(value.position3D.length == 1);
check(value.position3D[0].x === 30.375);
check(value.position3D[0].y === -45.375);
check(value.position3D[0].z === -97.625);
check(value.laser_pose.header.seq === 310600363);
check(value.laser_pose.header.stamp.sec === 2176449116);
check(value.laser_pose.header.stamp.nsec === 288487500);
check(value.laser_pose.header.frame_id === "vaad3β");
check(value.laser_pose.child_frame_id === "vccbbβ");
check(value.laser_pose.transform.translation.x === -14.5);
check(value.laser_pose.transform.translation.y === -91.5);
check(value.laser_pose.transform.translation.z === -239.0);
check(value.laser_pose.transform.rotation.x === 83.0);
check(value.laser_pose.transform.rotation.y === 179.0);
check(value.laser_pose.transform.rotation.z === 218.0);
check(value.laser_pose.transform.rotation.w === 116.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_62(){const input=bytes("9414bde32d6d8a7d0000000000403f400000000000a045400000000000086e400000000000585d400000000000001ac00000000000e06dc000000000000c64c000c09c4200a069c3"),codec=r.Ros1Pal_detection_msgsObstacleCodec,value=codec.decode(input);
check(value.id === 9046162845388838036n);
check(value.position.x === 31.25);
check(value.position.y === 43.25);
check(value.position.z === 240.25);
check(value.position_covariance.length == 4);
check(value.position_covariance[0] === 117.375);
check(value.position_covariance[1] === -6.5);
check(value.position_covariance[2] === -239.0);
check(value.position_covariance[3] === -160.375);
check(value.width === 78.375);
check(value.height === -233.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_63(){const input=bytes("64cf3aac10df0a46d20a5f09070000007663653565ceb2010000000c1d01d3b564364d0000000000e037c00000000000e85fc00000000000186d400000000000506a4000000000003c6a400000000000b050400000000000f46e4000e02a430040d0c2"),codec=r.Ros1Pal_detection_msgsObstaclesCodec,value=codec.decode(input);
check(value.header.seq === 2889535332);
check(value.header.stamp.sec === 1175117584);
check(value.header.stamp.nsec === 157223634);
check(value.header.frame_id === "vce5eβ");
check(value.obstacles.length == 1);
check(value.obstacles[0].id === 5563745121754750220n);
check(value.obstacles[0].position.x === -23.875);
check(value.obstacles[0].position.y === -127.625);
check(value.obstacles[0].position.z === 232.75);
check(value.obstacles[0].position_covariance.length == 4);
check(value.obstacles[0].position_covariance[0] === 210.5);
check(value.obstacles[0].position_covariance[1] === 209.875);
check(value.obstacles[0].position_covariance[2] === 66.75);
check(value.obstacles[0].position_covariance[3] === 247.625);
check(value.obstacles[0].width === 170.875);
check(value.obstacles[0].height === -104.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_64(){const input=bytes("58ace5c4e1d5d25ace9ce2b377e5d52d6871d8e1d108ef7fe0b68c3159cfbbaf8d670e541b57092300cdf002de32125e00150653e383365494db002a624d302d150000efc100801dc200c0e6c2070000007630613434ceb2006035c3070000007636633138ceb200204ac3070000007662323339ceb2006061430000a4400000c14100804ec300001ac100e067c300003dc30080b642f5cb39911b65b65be08d6a24070000007663356634ceb2010000000000000000846bc00000000000346cc00000000000146fc07d1acc2a2352736549609e0c070000007636656234ceb2070000007663613835ceb200000000005c6740000000000098524000000000007464c00000000000185fc000000000007c69c00000000000b040c00000000000405e40361b769be5bbdf8b25e1d03a070000007663633363ceb20000000000b864400000000000804f400000000000e86a40"),codec=r.Ros1Pal_detection_msgsPersonDetectionCodec,value=codec.decode(input);
check(value.full_body.x === 6544528374170496088n);
check(value.full_body.y === 3302798204027968718n);
check(value.full_body.width === 9218596659780940136n);
check(value.full_body.height === -5783801314436925728n);
check(value.face.x === 1410230157);
check(value.face.y === 587814683);
check(value.face.width === 49335552);
check(value.face.height === 1578250974);
check(value.face.eyesLocated === false);
check(value.face.leftEyeX === -481098219);
check(value.face.leftEyeY === -1806420349);
check(value.face.rightEyeX === 1646919899);
check(value.face.rightEyeY === 355283021);
check(value.face.position.x === -29.875);
check(value.face.position.y === -39.375);
check(value.face.position.z === -115.375);
check(value.face.name === "v0a44β");
check(value.face.confidence === -181.375);
check(value.face.gender === "v6c18β");
check(value.face.genderConfidence === -202.125);
check(value.face.expression === "vb239β");
check(value.face.expression_confidence === 225.375);
check(value.face.emotion_anger_confidence === 5.125);
check(value.face.emotion_disgust_confidence === 24.125);
check(value.face.emotion_fear_confidence === -206.5);
check(value.face.emotion_happiness_confidence === -9.625);
check(value.face.emotion_neutral_confidence === -231.875);
check(value.face.emotion_sadness_confidence === -189.0);
check(value.face.emotion_surprise_confidence === 91.25);
check(value.legs.header.seq === 2436484085);
check(value.legs.header.stamp.sec === 1538680091);
check(value.legs.header.stamp.nsec === 610962912);
check(value.legs.header.frame_id === "vc5f4β");
check(value.legs.position3D.length == 1);
check(value.legs.position3D[0].x === -220.125);
check(value.legs.position3D[0].y === -225.625);
check(value.legs.position3D[0].z === -248.625);
check(value.legs.laser_pose.header.seq === 718019197);
check(value.legs.laser_pose.header.stamp.sec === 1702056483);
check(value.legs.laser_pose.header.stamp.nsec === 211705929);
check(value.legs.laser_pose.header.frame_id === "v6eb4β");
check(value.legs.laser_pose.child_frame_id === "vca85β");
check(value.legs.laser_pose.transform.translation.x === 186.875);
check(value.legs.laser_pose.transform.translation.y === 74.375);
check(value.legs.laser_pose.transform.translation.z === -163.625);
check(value.legs.laser_pose.transform.rotation.x === -124.375);
check(value.legs.laser_pose.transform.rotation.y === -203.875);
check(value.legs.laser_pose.transform.rotation.z === -33.375);
check(value.legs.laser_pose.transform.rotation.w === 121.0);
check(value.position3D.header.seq === 2608208694);
check(value.position3D.header.stamp.sec === 2346695653);
check(value.position3D.header.stamp.nsec === 986767653);
check(value.position3D.header.frame_id === "vcc3cβ");
check(value.position3D.point.x === 165.75);
check(value.position3D.point.y === 63.0);
check(value.position3D.point.z === 215.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_65(){const input=bytes("78b892e4bdccba8f9493c923070000007637323664ceb20100000080b75c4f39ce6bd116875b38affe6ca6d6e3265d6f9a11c3f9d6e91d40afde834ff0527cd9c0550b5aff53050629abd501570bd720c13bd0576aa43bfafc943c8d00c0bdc200805b4200409342070000007663346232ceb2000098c2070000007665376131ceb200201143070000007630656337ceb200803c4200e04cc300a075430000b84100803ac300c0f3c2004085420080f1424b59c72d8e054d7a4b6d712d070000007638613630ceb20100000000000000000000c00000000000d058c000000000005063c0cccc023dca34ab3df314821a070000007664653965ceb2070000007633306535ceb20000000000402ec00000000000f04b4000000000005c6a400000000000705e400000000000d46ec00000000000f05d400000000000605fc0a37b8dba29f6ae37f38a8831070000007664626632ceb20000000000d0464000000000001c62400000000000c02fc003ad89f07ff268da362c5b2a070000007661353735ceb2070000007636613763ceb20000000000006ac00000000000b86fc00000000000d060c000000000003c624000000000004026c000000000002854c00000000000d46840"),codec=r.Ros1Pal_detection_msgsPersonDetectionsCodec,value=codec.decode(input);
check(value.header.seq === 3834820728);
check(value.header.stamp.sec === 2411383997);
check(value.header.stamp.nsec === 600413076);
check(value.header.frame_id === "v726dβ");
check(value.persons.length == 1);
check(value.persons[0].full_body.x === -3356362351734638720n);
check(value.persons[0].full_body.y === -6454504137410509034n);
check(value.persons[0].full_body.width === -4390558358614645802n);
check(value.persons[0].full_body.height === -8944519119996594439n);
check(value.persons[0].face.x === 2085810255);
check(value.persons[0].face.y === 190169305);
check(value.persons[0].face.width === 89390938);
check(value.persons[0].face.height === -710203130);
check(value.persons[0].face.eyesLocated === true);
check(value.persons[0].face.leftEyeX === 550964055);
check(value.persons[0].face.leftEyeY === 1473264577);
check(value.persons[0].face.rightEyeX === -96754582);
check(value.persons[0].face.rightEyeY === -1925409540);
check(value.persons[0].face.position.x === -94.875);
check(value.persons[0].face.position.y === 54.875);
check(value.persons[0].face.position.z === 73.625);
check(value.persons[0].face.name === "vc4b2β");
check(value.persons[0].face.confidence === -76.0);
check(value.persons[0].face.gender === "ve7a1β");
check(value.persons[0].face.genderConfidence === 145.125);
check(value.persons[0].face.expression === "v0ec7β");
check(value.persons[0].face.expression_confidence === 47.125);
check(value.persons[0].face.emotion_anger_confidence === -204.875);
check(value.persons[0].face.emotion_disgust_confidence === 245.625);
check(value.persons[0].face.emotion_fear_confidence === 23.0);
check(value.persons[0].face.emotion_happiness_confidence === -186.5);
check(value.persons[0].face.emotion_neutral_confidence === -121.875);
check(value.persons[0].face.emotion_sadness_confidence === 66.625);
check(value.persons[0].face.emotion_surprise_confidence === 120.75);
check(value.persons[0].legs.header.seq === 768039243);
check(value.persons[0].legs.header.stamp.sec === 2051868046);
check(value.persons[0].legs.header.stamp.nsec === 762408267);
check(value.persons[0].legs.header.frame_id === "v8a60β");
check(value.persons[0].legs.position3D.length == 1);
check(value.persons[0].legs.position3D[0].x === -2.0);
check(value.persons[0].legs.position3D[0].y === -99.25);
check(value.persons[0].legs.position3D[0].z === -154.5);
check(value.persons[0].legs.laser_pose.header.seq === 1023593676);
check(value.persons[0].legs.laser_pose.header.stamp.sec === 1034630346);
check(value.persons[0].legs.laser_pose.header.stamp.nsec === 444732659);
check(value.persons[0].legs.laser_pose.header.frame_id === "vde9eβ");
check(value.persons[0].legs.laser_pose.child_frame_id === "v30e5β");
check(value.persons[0].legs.laser_pose.transform.translation.x === -15.125);
check(value.persons[0].legs.laser_pose.transform.translation.y === 55.875);
check(value.persons[0].legs.laser_pose.transform.translation.z === 210.875);
check(value.persons[0].legs.laser_pose.transform.rotation.x === 121.75);
check(value.persons[0].legs.laser_pose.transform.rotation.y === -246.625);
check(value.persons[0].legs.laser_pose.transform.rotation.z === 119.75);
check(value.persons[0].legs.laser_pose.transform.rotation.w === -125.5);
check(value.persons[0].position3D.header.seq === 3129834403);
check(value.persons[0].position3D.header.stamp.sec === 934213161);
check(value.persons[0].position3D.header.stamp.nsec === 831032051);
check(value.persons[0].position3D.header.frame_id === "vdbf2β");
check(value.persons[0].position3D.point.x === 45.625);
check(value.persons[0].position3D.point.y === 144.875);
check(value.persons[0].position3D.point.z === -15.875);
check(value.camera_pose.header.seq === 4035554563);
check(value.camera_pose.header.stamp.sec === 3664310911);
check(value.camera_pose.header.stamp.nsec === 710618166);
check(value.camera_pose.header.frame_id === "va575β");
check(value.camera_pose.child_frame_id === "v6a7cβ");
check(value.camera_pose.transform.translation.x === -208.0);
check(value.camera_pose.transform.translation.y === -253.75);
check(value.camera_pose.transform.translation.z === -134.5);
check(value.camera_pose.transform.rotation.x === 145.875);
check(value.camera_pose.transform.rotation.y === -11.125);
check(value.camera_pose.transform.rotation.z === -80.625);
check(value.camera_pose.transform.rotation.w === 198.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_66(){const input=bytes(""),codec=r.Ros1Pal_detection_msgsRecognizeObjectsFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_67(){const input=bytes("73e6fcfd1b5550aad0e75925070000007666373236ceb2070000007633633332ceb201000000fe"),codec=r.Ros1Pal_detection_msgsRecognizeObjectsGoalCodec,value=codec.decode(input);
check(value.input_image.header.seq === 4261209715);
check(value.input_image.header.stamp.sec === 2857391387);
check(value.input_image.header.stamp.nsec === 626649040);
check(value.input_image.header.frame_id === "vf726β");
check(value.input_image.format === "v3c32β");
check(value.input_image.data.length == 1);
check(value.input_image.data[0] === 254);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_68(){const input=bytes("4d20693f057dc0ea9b888523070000007662383035ceb2070000007638343930ceb201000000a41c66f0f9a944310998e65a523308070000007635363661ceb201000000070000007635376362ceb2000018c112c36a278cc3c0eba2b0e1f6ade4ad9600"),codec=r.Ros1Pal_detection_msgsRecognizeObjectsResultCodec,value=codec.decode(input);
check(value.image.header.seq === 1063854157);
check(value.image.header.stamp.sec === 3938483461);
check(value.image.header.stamp.nsec === 595953819);
check(value.image.header.frame_id === "vb805β");
check(value.image.format === "v8490β");
check(value.image.data.length == 1);
check(value.image.data[0] === 164);
check(value.num_detections === 26140);
check(value.recognized_objects.header.seq === 1151990256);
check(value.recognized_objects.header.stamp.sec === 3868723505);
check(value.recognized_objects.header.stamp.nsec === 137581146);
check(value.recognized_objects.header.frame_id === "v566aβ");
check(value.recognized_objects.objects.length == 1);
check(value.recognized_objects.objects[0].object_class === "v57cbβ");
check(value.recognized_objects.objects[0].confidence === -9.5);
check(value.recognized_objects.objects[0].bounding_box.x_offset === 661308178);
check(value.recognized_objects.objects[0].bounding_box.y_offset === 3955278732);
check(value.recognized_objects.objects[0].bounding_box.height === 4141985954);
check(value.recognized_objects.objects[0].bounding_box.width === 2527978669);
check(value.recognized_objects.objects[0].bounding_box.do_rectify === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_69(){const input=bytes("fcadafdf8ca8e488ca03bd0e070000007634316364ceb201000000070000007666313132ceb20100000008811663b1f821fd01000000e62ea3715f5794ef"),codec=r.Ros1Pal_detection_msgsRecognizedActionsCodec,value=codec.decode(input);
check(value.header.seq === 3752832508);
check(value.header.stamp.sec === 2296686732);
check(value.header.stamp.nsec === 247268298);
check(value.header.frame_id === "v41cdβ");
check(value.action_name.length == 1);
check(value.action_name[0] === "vf112β");
check(value.u.length == 1);
check(value.u[0] === -206610667127013112n);
check(value.v.length == 1);
check(value.v[0] === -1183224734651568410n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_70(){const input=bytes("070000007632343737ceb200402b43b7e1b78129e11d4d7cc6dce83f5229aa01"),codec=r.Ros1Pal_detection_msgsRecognizedObjectCodec,value=codec.decode(input);
check(value.object_class === "v2477β");
check(value.confidence === 171.25);
check(value.bounding_box.x_offset === 2176311735);
check(value.bounding_box.y_offset === 1293803817);
check(value.bounding_box.height === 3906782844);
check(value.bounding_box.width === 2854834751);
check(value.bounding_box.do_rectify === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_71(){const input=bytes("c2e562e72a5f4870e0a9f031070000007638663934ceb201000000070000007666393162ceb200800cc29e9e0527009eafeb74511a1b2d12cfd501"),codec=r.Ros1Pal_detection_msgsRecognizedObjectArrayCodec,value=codec.decode(input);
check(value.header.seq === 3882018242);
check(value.header.stamp.sec === 1883791146);
check(value.header.stamp.nsec === 837855712);
check(value.header.frame_id === "v8f94β");
check(value.objects.length == 1);
check(value.objects[0].object_class === "vf91bβ");
check(value.objects[0].confidence === -35.125);
check(value.objects[0].bounding_box.x_offset === 654679710);
check(value.objects[0].bounding_box.y_offset === 3954155008);
check(value.objects[0].bounding_box.height === 454709620);
check(value.objects[0].bounding_box.width === 3587117613);
check(value.objects[0].bounding_box.do_rectify === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_72(){const input=bytes("0100205543"),codec=r.Ros1Pal_detection_msgsRecognizerRequestCodec,value=codec.decode(input);
check(value.enabled === true);
check(value.minConfidence === 213.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_73(){const input=bytes(""),codec=r.Ros1Pal_detection_msgsRecognizerResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_74(){const input=bytes("8583370eacf3872e59407535070000007639656331ceb20100000074a59391cddca40f0100000011c22f29a8bb18b7"),codec=r.Ros1Pal_detection_msgsRotatedDetection2dCodec,value=codec.decode(input);
check(value.header.seq === 238519173);
check(value.header.stamp.sec === 780661676);
check(value.header.stamp.nsec === 896876633);
check(value.header.frame_id === "v9ec1β");
check(value.x.length == 1);
check(value.x[0] === 1127268582218245492n);
check(value.y.length == 1);
check(value.y[0] === -5253242634407788015n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_75(){const input=bytes("070000007637313235ceb20000000000d068c00000000000ec62c0"),codec=r.Ros1Pal_detection_msgsSelectTexturedObjectRequestCodec,value=codec.decode(input);
check(value.imageFullPathFileName === "v7125β");
check(value.width === -198.5);
check(value.height === -151.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_76(){const input=bytes("01"),codec=r.Ros1Pal_detection_msgsSelectTexturedObjectResponseCodec,value=codec.decode(input);
check(value.result === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_77(){const input=bytes("070000007664346330ceb200"),codec=r.Ros1Pal_detection_msgsSetDatabaseRequestCodec,value=codec.decode(input);
check(value.databaseName === "vd4c0β");
check(value.purgeAll === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_78(){const input=bytes(""),codec=r.Ros1Pal_detection_msgsSetDatabaseResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_79(){const input=bytes("070000007663656636ceb2"),codec=r.Ros1Pal_detection_msgsStartEnrollmentRequestCodec,value=codec.decode(input);
check(value.name === "vcef6β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_80(){const input=bytes("01"),codec=r.Ros1Pal_detection_msgsStartEnrollmentResponseCodec,value=codec.decode(input);
check(value.result === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_81(){const input=bytes(""),codec=r.Ros1Pal_detection_msgsStopEnrollmentRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_82(){const input=bytes("01a5374fd3070000007666373664ceb2"),codec=r.Ros1Pal_detection_msgsStopEnrollmentResponseCodec,value=codec.decode(input);
check(value.enrollment_ok === true);
check(value.numFacesEnrolled === -749783131);
check(value.error_msg === "vf76dβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_83(){const input=bytes("92986aecadb465b488c84d07070000007663333335ceb200989ceef09e4244ebce271c070000007665623863ceb2070000007630336535ceb2010000008ed4f97b4ed5ba6ffb8861791c070000007631366339ceb201000000aaf04f0d1389789301000000cf97f3b576eec42b"),codec=r.Ros1Pal_detection_msgsTexturedObjectDetectionCodec,value=codec.decode(input);
check(value.header.seq === 3966408850);
check(value.header.stamp.sec === 3026564269);
check(value.header.stamp.nsec === 122538120);
check(value.header.frame_id === "vc335β");
check(value.img.header.seq === 4003239936);
check(value.img.header.stamp.sec === 1145216752);
check(value.img.header.stamp.nsec === 472370923);
check(value.img.header.frame_id === "veb8cβ");
check(value.img.format === "v03e5β");
check(value.img.data.length == 1);
check(value.img.data[0] === 142);
check(value.roi.header.seq === 1316747732);
check(value.roi.header.stamp.sec === 4218403541);
check(value.roi.header.stamp.nsec === 477716872);
check(value.roi.header.frame_id === "v16c9β");
check(value.roi.x.length == 1);
check(value.roi.x[0] === -7820350038008139606n);
check(value.roi.y.length == 1);
check(value.roi.y[0] === 3153907832692381647n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_84(){const input=bytes("685ae3f749809b13ddec5732070000007635613237ceb201f04a"),codec=r.Ros1Pal_detection_msgsWaveDetectionCodec,value=codec.decode(input);
check(value.header.seq === 4158872168);
check(value.header.stamp.sec === 328958025);
check(value.header.stamp.nsec === 844623069);
check(value.header.frame_id === "v5a27β");
check(value.wave === true);
check(value.u === -16);
check(value.v === 74);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_85(){const input=bytes("39"),codec=r.Ros1Pal_interaction_msgsASRActivationCodec,value=codec.decode(input);
check(value.action === 57);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_86(){const input=bytes("0477070000007663303866ceb25ce6716051557bcd2b340722fbdba0a41801000000070000007633343765ceb2070000007664626565ceb200"),codec=r.Ros1Pal_interaction_msgsASREventCodec,value=codec.decode(input);
check(value.event_id === 4);
check(value.listen_state === 119);
check(value.recognized_utterance.text === "vc08fβ");
check(value.recognized_utterance.confidence === 92);
check(value.recognized_utterance.start.sec === 1365275110);
check(value.recognized_utterance.start.nsec === 734886741);
check(value.recognized_utterance.end.sec === 4213311284);
check(value.recognized_utterance.end.nsec === 413442267);
check(value.recognized_utterance.tags.length == 1);
check(value.recognized_utterance.tags[0].key === "v347eβ");
check(value.recognized_utterance.tags[0].value === "vdbeeβ");
check(value.active === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_87(){const input=bytes("070000007630636336ceb279626d20a7703e850da8bd22780e214e3201000000070000007637663930ceb2070000007630383465ceb2"),codec=r.Ros1Pal_interaction_msgsASRFileFeedbackCodec,value=codec.decode(input);
check(value.recognised_utterance.text === "v0cc6β");
check(value.recognised_utterance.confidence === 121);
check(value.recognised_utterance.start.sec === 2803920226);
check(value.recognised_utterance.start.nsec === 226836080);
check(value.recognised_utterance.end.sec === 2015542696);
check(value.recognised_utterance.end.nsec === 843981070);
check(value.recognised_utterance.tags.length == 1);
check(value.recognised_utterance.tags[0].key === "v7f90β");
check(value.recognised_utterance.tags[0].value === "v084eβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_88(){const input=bytes("070000007662383437ceb2070000007639323366ceb2070000007665333735ceb2"),codec=r.Ros1Pal_interaction_msgsASRFileGoalCodec,value=codec.decode(input);
check(value.file === "vb847β");
check(value.lang_id === "v923fβ");
check(value.grammar === "ve375β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_89(){const input=bytes("070000007663646539ceb2070000007631613763ceb201000000070000007630336335ceb214a326680a1d1eb92e9450d9d464a68d1d01000000070000007634633766ceb2070000007632616237ceb2"),codec=r.Ros1Pal_interaction_msgsASRFileResultCodec,value=codec.decode(input);
check(value.file === "vcde9β");
check(value.msg === "v1a7cβ");
check(value.recognised_utterances.length == 1);
check(value.recognised_utterances[0].text === "v03c5β");
check(value.recognised_utterances[0].confidence === 20);
check(value.recognised_utterances[0].start.sec === 174597795);
check(value.recognised_utterances[0].start.nsec === 783883805);
check(value.recognised_utterances[0].end.sec === 3571011732);
check(value.recognised_utterances[0].end.nsec === 495822436);
check(value.recognised_utterances[0].tags.length == 1);
check(value.recognised_utterances[0].tags[0].key === "v4c7fβ");
check(value.recognised_utterances[0].tags[0].value === "v2ab7β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_90(){const input=bytes("a8070000007634616639ceb2"),codec=r.Ros1Pal_interaction_msgsASRLangModelMngmtCodec,value=codec.decode(input);
check(value.action === -88);
check(value.modelName === "v4af9β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_91(){const input=bytes("070000007663623731ceb2"),codec=r.Ros1Pal_interaction_msgsASRLanguageCodec,value=codec.decode(input);
check(value.language === "vcb71β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_92(){const input=bytes("0100000030411f070000007663306430ceb2070000007637313039ceb2"),codec=r.Ros1Pal_interaction_msgsASRServiceRequestCodec,value=codec.decode(input);
check(value.request.requests.length == 1);
check(value.request.requests[0] === 48);
check(value.request.activation.action === 65);
check(value.request.model.action === 31);
check(value.request.model.modelName === "vc0d0β");
check(value.request.lang.language === "v7109β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_93(){const input=bytes("00070000007631613439ceb2070000007637386662ceb2070000007634383563ceb2070000007666623334ceb24069f44f138b0b99f5070000007632376536ceb2070000007630353935ceb2"),codec=r.Ros1Pal_interaction_msgsASRServiceResponseCodec,value=codec.decode(input);
check(value.response.status.active === false);
check(value.response.status.language === "v1a49β");
check(value.response.status.enabled_grammar === "v78fbβ");
check(value.response.status.acoustic_env === "v485cβ");
check(value.response.status.device.device_name === "vfb34β");
check(value.response.status.device.sample_rate === 1341417792);
check(value.response.status.device.sample_size === 19);
check(value.response.status.device.format === -117);
check(value.response.status.device.buffer_size === 39179);
check(value.response.status.device.number_of_channels === -11);
check(value.response.error_msg === "v27e6β");
check(value.response.warn_msg === "v0595β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_94(){const input=bytes("0100000062c62a070000007630373536ceb2070000007632373366ceb2"),codec=r.Ros1Pal_interaction_msgsASRSrvRequestCodec,value=codec.decode(input);
check(value.requests.length == 1);
check(value.requests[0] === 98);
check(value.activation.action === -58);
check(value.model.action === 42);
check(value.model.modelName === "v0756β");
check(value.lang.language === "v273fβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_95(){const input=bytes("01070000007663613762ceb2070000007632323136ceb2070000007635623132ceb2070000007638663861ceb2cd904a3b9ecc8660a8070000007661333766ceb2070000007632323537ceb2"),codec=r.Ros1Pal_interaction_msgsASRSrvResponseCodec,value=codec.decode(input);
check(value.status.active === true);
check(value.status.language === "vca7bβ");
check(value.status.enabled_grammar === "v2216β");
check(value.status.acoustic_env === "v5b12β");
check(value.status.device.device_name === "v8f8aβ");
check(value.status.device.sample_rate === 994742477);
check(value.status.device.sample_size === -98);
check(value.status.device.format === -52);
check(value.status.device.buffer_size === 24710);
check(value.status.device.number_of_channels === -88);
check(value.error_msg === "va37fβ");
check(value.warn_msg === "v2257β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_96(){const input=bytes("00070000007639363839ceb2070000007633366230ceb2070000007666343836ceb2070000007663633933ceb2f9975378aa28b26771"),codec=r.Ros1Pal_interaction_msgsASRStatusCodec,value=codec.decode(input);
check(value.active === false);
check(value.language === "v9689β");
check(value.enabled_grammar === "v36b0β");
check(value.acoustic_env === "vf486β");
check(value.device.device_name === "vcc93β");
check(value.device.sample_rate === 2018744313);
check(value.device.sample_size === -86);
check(value.device.format === 40);
check(value.device.buffer_size === 26546);
check(value.device.number_of_channels === 113);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_97(){const input=bytes("070000007661303738ceb2c1b5b81492cf8a450c"),codec=r.Ros1Pal_interaction_msgsAudioDeviceDescriptionCodec,value=codec.decode(input);
check(value.device_name === "va078β");
check(value.sample_rate === 347649473);
check(value.sample_size === -110);
check(value.format === -49);
check(value.buffer_size === 17802);
check(value.number_of_channels === 12);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_98(){const input=bytes(""),codec=r.Ros1Pal_interaction_msgsAudioPlayFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_99(){const input=bytes("070000007630393232ceb271"),codec=r.Ros1Pal_interaction_msgsAudioPlayGoalCodec,value=codec.decode(input);
check(value.file === "v0922β");
check(value.priority === 113);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_100(){const input=bytes(""),codec=r.Ros1Pal_interaction_msgsAudioPlayResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_101(){const input=bytes("a49a0579339e1b84e188d419070000007630386530ceb200070000007637326465ceb2"),codec=r.Ros1Pal_interaction_msgsAudioPlayerStateCodec,value=codec.decode(input);
check(value.header.seq === 2030410404);
check(value.header.stamp.sec === 2216402483);
check(value.header.stamp.nsec === 433359073);
check(value.header.frame_id === "v08e0β");
check(value.isPlaying === false);
check(value.msg === "v72deβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_102(){const input=bytes("00e029c300c047430080e942070000007662343535ceb2"),codec=r.Ros1Pal_interaction_msgsDirectionOfArrivalCodec,value=codec.decode(input);
check(value.doa === -169.875);
check(value.prob === 199.75);
check(value.power === 116.75);
check(value.metadoa === "vb455β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_103(){const input=bytes("01"),codec=r.Ros1Pal_interaction_msgsEnablingSoundLocalisationCodec,value=codec.decode(input);
check(value.enable === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_104(){const input=bytes("070000007666636233ceb2070000007664336530ceb2070000007664616531ceb201000000070000007636333832ceb2070000007630616266ceb2070000007661356563ceb2070000007662646265ceb2070000007633326430ceb2"),codec=r.Ros1Pal_interaction_msgsGetSpeechDurationRequestCodec,value=codec.decode(input);
check(value.text.section === "vfcb3β");
check(value.text.key === "vd3e0β");
check(value.text.lang_id === "vdae1β");
check(value.text.arguments.length == 1);
check(value.text.arguments[0].section === "v6382β");
check(value.text.arguments[0].key === "v0abfβ");
check(value.text.arguments[0].expanded === "va5ecβ");
check(value.rawtext.text === "vbdbeβ");
check(value.rawtext.lang_id === "v32d0β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_105(){const input=bytes("0101000000070000007634376431ceb2010000008f185438c04ecf1f"),codec=r.Ros1Pal_interaction_msgsGetSpeechDurationResponseCodec,value=codec.decode(input);
check(value.ok === true);
check(value.words.length == 1);
check(value.words[0] === "v47d1β");
check(value.word_durations.length == 1);
check(value.word_durations[0].sec === 945035407);
check(value.word_durations[0].nsec === 533679808);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_106(){const input=bytes("070000007636303662ceb2070000007632633766ceb2070000007630313365ceb2"),codec=r.Ros1Pal_interaction_msgsI18nArgumentCodec,value=codec.decode(input);
check(value.section === "v606bβ");
check(value.key === "v2c7fβ");
check(value.expanded === "v013eβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_107(){const input=bytes("070000007664393538ceb2070000007662666533ceb2070000007666663061ceb201000000070000007665363562ceb2070000007661363933ceb2070000007639623037ceb2"),codec=r.Ros1Pal_interaction_msgsI18nTextCodec,value=codec.decode(input);
check(value.section === "vd958β");
check(value.key === "vbfe3β");
check(value.lang_id === "vff0aβ");
check(value.arguments.length == 1);
check(value.arguments[0].section === "ve65bβ");
check(value.arguments[0].key === "va693β");
check(value.arguments[0].expanded === "v9b07β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_108(){const input=bytes("070000007663326463ceb201000000070000007664626537ceb2070000007663643631ceb2"),codec=r.Ros1Pal_interaction_msgsInputCodec,value=codec.decode(input);
check(value.action === "vc2dcβ");
check(value.args.length == 1);
check(value.args[0].key === "vdbe7β");
check(value.args[0].value === "vcd61β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_109(){const input=bytes("070000007633643439ceb2070000007630343762ceb2"),codec=r.Ros1Pal_interaction_msgsInputArgumentCodec,value=codec.decode(input);
check(value.key === "v3d49β");
check(value.value === "v047bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_110(){const input=bytes(""),codec=r.Ros1Pal_interaction_msgsSoundFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_111(){const input=bytes("070000007639343131ceb2070000007630343630ceb2ca1d9d3eda0a7f1a"),codec=r.Ros1Pal_interaction_msgsSoundGoalCodec,value=codec.decode(input);
check(value.text === "v9411β");
check(value.lang_id === "v0460β");
check(value.wait_before_speaking.sec === 1050484170);
check(value.wait_before_speaking.nsec === 444533466);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_112(){const input=bytes("00"),codec=r.Ros1Pal_interaction_msgsSoundLocalisationServiceRequestCodec,value=codec.decode(input);
check(value.enabling.enable === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_113(){const input=bytes(""),codec=r.Ros1Pal_interaction_msgsSoundLocalisationServiceResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_114(){const input=bytes("070000007632633936ceb2"),codec=r.Ros1Pal_interaction_msgsSoundResultCodec,value=codec.decode(input);
check(value.text === "v2c96β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_115(){const input=bytes("00070000007662376432ceb2"),codec=r.Ros1Pal_interaction_msgsTTSstateCodec,value=codec.decode(input);
check(value.isSpeaking === false);
check(value.text === "vb7d2β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_116(){const input=bytes("9f7782e26734874fe936070000007662393837ceb2070000007661636133ceb2070000007638613335ceb2070000007630326632ceb201000000070000007661613766ceb201000000070000007664643233ceb2"),codec=r.Ros1Pal_interaction_msgsTtsFeedbackCodec,value=codec.decode(input);
check(value.event_type === 30623);
check(value.timestamp.sec === 879223426);
check(value.timestamp.nsec === 921259911);
check(value.text_said === "vb987β");
check(value.next_word === "vaca3β");
check(value.viseme_id === "v8a35β");
check(value.marks.id === "v02f2β");
check(value.marks.keys.length == 1);
check(value.marks.keys[0] === "vaa7fβ");
check(value.marks.value.length == 1);
check(value.marks.value[0] === "vdd23β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_117(){const input=bytes("070000007633383361ceb2070000007632356334ceb2070000007631653638ceb201000000070000007662656331ceb2070000007661643430ceb2070000007637643363ceb2070000007637393337ceb2070000007639343064ceb2070000007662316135ceb20000000000a05540"),codec=r.Ros1Pal_interaction_msgsTtsGoalCodec,value=codec.decode(input);
check(value.text.section === "v383aβ");
check(value.text.key === "v25c4β");
check(value.text.lang_id === "v1e68β");
check(value.text.arguments.length == 1);
check(value.text.arguments[0].section === "vbec1β");
check(value.text.arguments[0].key === "vad40β");
check(value.text.arguments[0].expanded === "v7d3cβ");
check(value.rawtext.text === "v7937β");
check(value.rawtext.lang_id === "v940dβ");
check(value.speakerName === "vb1a5β");
check(value.wait_before_speaking === 86.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_118(){const input=bytes("070000007664653138ceb201000000070000007633663531ceb201000000070000007665346235ceb2"),codec=r.Ros1Pal_interaction_msgsTtsMarkCodec,value=codec.decode(input);
check(value.id === "vde18β");
check(value.keys.length == 1);
check(value.keys[0] === "v3f51β");
check(value.value.length == 1);
check(value.value[0] === "ve4b5β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_119(){const input=bytes("070000007631633236ceb2070000007663376463ceb2"),codec=r.Ros1Pal_interaction_msgsTtsResultCodec,value=codec.decode(input);
check(value.text === "v1c26β");
check(value.msg === "vc7dcβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_120(){const input=bytes("070000007665343337ceb2070000007634353431ceb2"),codec=r.Ros1Pal_interaction_msgsTtsTextCodec,value=codec.decode(input);
check(value.text === "ve437β");
check(value.lang_id === "v4541β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_121(){const input=bytes("01"),codec=r.Ros1Pal_interaction_msgsVoiceActivityCodec,value=codec.decode(input);
check(value.isSpeech === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_122(){const input=bytes("070000007661363733ceb2070000007634616435ceb2"),codec=r.Ros1Pal_interaction_msgsWebGuiEventCodec,value=codec.decode(input);
check(value.name === "va673β");
check(value.arg === "v4ad5β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_123(){const input=bytes("070000007634633865ceb2070000007634313664ceb2"),codec=r.Ros1Pal_interaction_msgsActiontagCodec,value=codec.decode(input);
check(value.key === "v4c8eβ");
check(value.value === "v416dβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_124(){const input=bytes("070000007636343631ceb20573e3f6a70cfe9023085590200ed2ef3101000000070000007632363439ceb2070000007664633232ceb2"),codec=r.Ros1Pal_interaction_msgsAsrresultCodec,value=codec.decode(input);
check(value.text === "v6461β");
check(value.confidence === 5);
check(value.start.sec === 2817975155);
check(value.start.nsec === 596704780);
check(value.end.sec === 546329864);
check(value.end.nsec === 837800462);
check(value.tags.length == 1);
check(value.tags[0].key === "v2649β");
check(value.tags[0].value === "vdc22β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_125(){const input=bytes("070000007633643962ceb2070000007630316332ceb2070000007665613639ceb2070000007632363062ceb201"),codec=r.Ros1Pal_interaction_msgsAsrupdateCodec,value=codec.decode(input);
check(value.language === "v3d9bβ");
check(value.enable_grammar === "v01c2β");
check(value.disable_grammar === "vea69β");
check(value.acousticenv === "v260bβ");
check(value.active === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_126(){const input=bytes("010000000000000000d04540010000000000000000805240"),codec=r.Ros1Pal_interaction_msgsAudiosignalCodec,value=codec.decode(input);
check(value.channel0.length == 1);
check(value.channel0[0] === 43.625);
check(value.channel1.length == 1);
check(value.channel1[0] === 74.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_127(){const input=bytes("070000007665643135ceb2070000007666363231ceb2070000007666366632ceb2070000007633313739ceb201"),codec=r.Ros1Pal_interaction_msgsRecognizerServiceRequestCodec,value=codec.decode(input);
check(value.asrupdate.language === "ved15β");
check(value.asrupdate.enable_grammar === "vf621β");
check(value.asrupdate.disable_grammar === "vf6f2β");
check(value.asrupdate.acousticenv === "v3179β");
check(value.asrupdate.active === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_128(){const input=bytes(""),codec=r.Ros1Pal_interaction_msgsRecognizerServiceResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_129(){const input=bytes(""),codec=r.Ros1Pal_motion_model_msgsGetMotionMapRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_130(){const input=bytes("75135b19eb308a82a93b912a070000007632666662ceb26780fea91222443900005ec10b9069f90b723bc900000000003c65c000000000001858c00000000000186d4000000000005865400000000000105d4000000000003058c00000000000106b400100000001000000008097c2008044c20000eb4200e04fc300808cc20000b1c2"),codec=r.Ros1Pal_motion_model_msgsGetMotionMapResponseCodec,value=codec.decode(input);
check(value.map.header.seq === 425399157);
check(value.map.header.stamp.sec === 2190094571);
check(value.map.header.stamp.nsec === 714161065);
check(value.map.header.frame_id === "v2ffbβ");
check(value.map.info.map_load_time.sec === 2852028519);
check(value.map.info.map_load_time.nsec === 960766482);
check(value.map.info.resolution === -13.875);
check(value.map.info.width === 4184444939);
check(value.map.info.height === 3376116235);
check(value.map.info.origin.position.x === -169.875);
check(value.map.info.origin.position.y === -96.375);
check(value.map.info.origin.position.z === 232.75);
check(value.map.info.origin.orientation.x === 170.75);
check(value.map.info.origin.orientation.y === 116.25);
check(value.map.info.origin.orientation.z === -96.75);
check(value.map.info.origin.orientation.w === 216.5);
check(value.map.data.length == 1);
check(value.map.data[0].models.length == 1);
check(value.map.data[0].models[0].heading_mean === -75.75);
check(value.map.data[0].models[0].heading_std_dev === -49.125);
check(value.map.data[0].models[0].linear_speed_mean === 117.5);
check(value.map.data[0].models[0].linear_speed_std_dev === -207.875);
check(value.map.data[0].models[0].angular_speed_mean === -70.25);
check(value.map.data[0].models[0].angular_speed_std_dev === -88.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_131(){const input=bytes("00004043000066c200e00143004025430000cb42006061c3"),codec=r.Ros1Pal_motion_model_msgsMotionModelCodec,value=codec.decode(input);
check(value.heading_mean === 192.0);
check(value.heading_std_dev === -57.5);
check(value.linear_speed_mean === 129.875);
check(value.linear_speed_std_dev === 165.25);
check(value.angular_speed_mean === 101.5);
check(value.angular_speed_std_dev === -225.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_132(){const input=bytes("010000000000d8410040dc42004053c30020344300c098c2000034c1"),codec=r.Ros1Pal_motion_model_msgsMotionModelListCodec,value=codec.decode(input);
check(value.models.length == 1);
check(value.models[0].heading_mean === 27.0);
check(value.models[0].heading_std_dev === 110.125);
check(value.models[0].linear_speed_mean === -211.25);
check(value.models[0].linear_speed_std_dev === 180.125);
check(value.models[0].angular_speed_mean === -76.375);
check(value.models[0].angular_speed_std_dev === -11.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_133(){const input=bytes("aeaf02435cd26af864cbe810070000007661353433ceb2ab9b3c7392da163400c066c3d02c30a3eb2060d800000000006869400000000000a064400000000000b86b400000000000805cc00000000000c043c00000000000404dc000000000006068400100000001000000004069c30080acc200808c42002066430040db4200001142"),codec=r.Ros1Pal_motion_model_msgsMotionModelMapCodec,value=codec.decode(input);
check(value.header.seq === 1124249518);
check(value.header.stamp.sec === 4167750236);
check(value.header.stamp.nsec === 283691876);
check(value.header.frame_id === "va543β");
check(value.info.map_load_time.sec === 1933351851);
check(value.info.map_load_time.nsec === 873912978);
check(value.info.resolution === -230.75);
check(value.info.width === 2737843408);
check(value.info.height === 3630178539);
check(value.info.origin.position.x === 203.25);
check(value.info.origin.position.y === 165.0);
check(value.info.origin.position.z === 221.75);
check(value.info.origin.orientation.x === -114.0);
check(value.info.origin.orientation.y === -39.5);
check(value.info.origin.orientation.z === -58.5);
check(value.info.origin.orientation.w === 195.0);
check(value.data.length == 1);
check(value.data[0].models.length == 1);
check(value.data[0].models[0].heading_mean === -233.25);
check(value.data[0].models[0].heading_std_dev === -86.25);
check(value.data[0].models[0].linear_speed_mean === 70.25);
check(value.data[0].models[0].linear_speed_std_dev === 230.125);
check(value.data[0].models[0].angular_speed_mean === 109.625);
check(value.data[0].models[0].angular_speed_std_dev === 36.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_134(){const input=bytes("8ff4b36164415ee5693fcc28070000007632366330ceb2070000007633356332ceb2b9aa"),codec=r.Ros1Pal_multirobot_msgsWifiServiceDetectionCodec,value=codec.decode(input);
check(value.header.seq === 1639183503);
check(value.header.stamp.sec === 3848159588);
check(value.header.stamp.nsec === 684474217);
check(value.header.frame_id === "v26c0β");
check(value.id === "v35c2β");
check(value.signal === -21831);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_135(){const input=bytes("070000007665353164ceb2"),codec=r.Ros1Pal_navigation_msgsAcknowledgmentRequestCodec,value=codec.decode(input);
check(value.input === "ve51dβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_136(){const input=bytes("00070000007639326533ceb2"),codec=r.Ros1Pal_navigation_msgsAcknowledgmentResponseCodec,value=codec.decode(input);
check(value.output === false);
check(value.error === "v92e3β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_137(){const input=bytes("01000000070000007638363862ceb2"),codec=r.Ros1Pal_navigation_msgsAvailableMapsCodec,value=codec.decode(input);
check(value.map_names.length == 1);
check(value.map_names[0].data === "v868bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_138(){const input=bytes("070000007639656432ceb2070000007639303562ceb2"),codec=r.Ros1Pal_navigation_msgsChangeBuildingRequestCodec,value=codec.decode(input);
check(value.building_name === "v9ed2β");
check(value.floor_name === "v905bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_139(){const input=bytes("00"),codec=r.Ros1Pal_navigation_msgsChangeBuildingResponseCodec,value=codec.decode(input);
check(value.success === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_140(){const input=bytes("cf9507707f05a0c1c1e4402a070000007636633237ceb2079f9bfb555c09390040bd42b11635901f74e6e20000000000906ec00000000000b86dc000000000004025400000000000c023c000000000009045c000000000006038c000000000000c6c400100000085"),codec=r.Ros1Pal_navigation_msgsChangeMapRequestCodec,value=codec.decode(input);
check(value.map.header.seq === 1879545295);
check(value.map.header.stamp.sec === 3248489855);
check(value.map.header.stamp.nsec === 708895937);
check(value.map.header.frame_id === "v6c27β");
check(value.map.info.map_load_time.sec === 4221280007);
check(value.map.info.map_load_time.nsec === 956914773);
check(value.map.info.resolution === 94.625);
check(value.map.info.width === 2419398321);
check(value.map.info.height === 3806753823);
check(value.map.info.origin.position.x === -244.5);
check(value.map.info.origin.position.y === -237.75);
check(value.map.info.origin.position.z === 10.625);
check(value.map.info.origin.orientation.x === -9.875);
check(value.map.info.origin.orientation.y === -43.125);
check(value.map.info.origin.orientation.z === -24.375);
check(value.map.info.origin.orientation.w === 224.375);
check(value.map.data.length == 1);
check(value.map.data[0] === -123);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_141(){const input=bytes("00"),codec=r.Ros1Pal_navigation_msgsChangeMapResponseCodec,value=codec.decode(input);
check(value.success === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_142(){const input=bytes("a8c1c1fec6d44fa1386c6721070000007633333738ceb21d1100b4a1b0531200007ac3d642f31eb046be310000000000986cc000000000002046c000000000009868c00000000000146d4000000000007c6b4000000000001466400000000000b857400100000034070000007630663465ceb201000000070000007666613564ceb2"),codec=r.Ros1Pal_navigation_msgsChangeSyncMapRequestCodec,value=codec.decode(input);
check(value.map.header.seq === 4274110888);
check(value.map.header.stamp.sec === 2706363590);
check(value.map.header.stamp.nsec === 560426040);
check(value.map.header.frame_id === "v3378β");
check(value.map.info.map_load_time.sec === 3019903261);
check(value.map.info.map_load_time.nsec === 307474593);
check(value.map.info.resolution === -250.0);
check(value.map.info.width === 519258838);
check(value.map.info.height === 834553520);
check(value.map.info.origin.position.x === -228.75);
check(value.map.info.origin.position.y === -44.25);
check(value.map.info.origin.position.z === -196.75);
check(value.map.info.origin.orientation.x === 232.625);
check(value.map.info.origin.orientation.y === 219.875);
check(value.map.info.origin.orientation.z === 176.625);
check(value.map.info.origin.orientation.w === 94.875);
check(value.map.data.length == 1);
check(value.map.data[0] === 52);
check(value.map_name === "v0f4eβ");
check(value.sources.length == 1);
check(value.sources[0] === "vfa5dβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_143(){const input=bytes("00"),codec=r.Ros1Pal_navigation_msgsChangeSyncMapResponseCodec,value=codec.decode(input);
check(value.success === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_144(){const input=bytes("000000000094634001"),codec=r.Ros1Pal_navigation_msgsDisableEmergencyRequestCodec,value=codec.decode(input);
check(value.second === 156.625);
check(value.useLEDs === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_145(){const input=bytes("01"),codec=r.Ros1Pal_navigation_msgsDisableEmergencyResponseCodec,value=codec.decode(input);
check(value.success === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_146(){const input=bytes("01010001000000070000007663646338ceb2"),codec=r.Ros1Pal_navigation_msgsEmergencyCodec,value=codec.decode(input);
check(value.data === true);
check(value.forward === true);
check(value.backward === false);
check(value.msgs.length == 1);
check(value.msgs[0].data === "vcdc8β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_147(){const input=bytes("070000007639613036ceb200000000000070564000000000007066400000000000c86f40"),codec=r.Ros1Pal_navigation_msgsEulerAnglesCodec,value=codec.decode(input);
check(value.sequence === "v9a06β");
check(value.angles_in_degrees === false);
check(value.ai === 89.75);
check(value.aj === 179.5);
check(value.ak === 254.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_148(){const input=bytes("cebadede76e8d7c188610118070000007631626361ceb2070000007630363865ceb2000000000000385ec000000000006063400000000000b05f40"),codec=r.Ros1Pal_navigation_msgsEulerAnglesStampedCodec,value=codec.decode(input);
check(value.header.seq === 3739138766);
check(value.header.stamp.sec === 3252152438);
check(value.header.stamp.nsec === 402743688);
check(value.header.frame_id === "v1bcaβ");
check(value.angles.sequence === "v068eβ");
check(value.angles.angles_in_degrees === false);
check(value.angles.ai === -120.875);
check(value.angles.aj === 155.0);
check(value.angles.ak === 126.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_149(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsExecuteParkingFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_150(){const input=bytes("070000007635313463ceb2070000007637346566ceb20000000000000006400000000000c86ec0"),codec=r.Ros1Pal_navigation_msgsExecuteParkingGoalCodec,value=codec.decode(input);
check(value.source_frame === "v514cβ");
check(value.target_frame === "v74efβ");
check(value.backwards === false);
check(value.max_lin_vel === 2.75);
check(value.max_rot_vel === -246.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_151(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsExecuteParkingResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_152(){const input=bytes("00000000001c66c000000000000c6fc00000000000bc67c00000000000403a4000000000009052400000000000a04ac00000000000a04140"),codec=r.Ros1Pal_navigation_msgsFinalApproachPoseRequestCodec,value=codec.decode(input);
check(value.pose.position.x === -176.875);
check(value.pose.position.y === -248.375);
check(value.pose.position.z === -189.875);
check(value.pose.orientation.x === 26.25);
check(value.pose.orientation.y === 74.25);
check(value.pose.orientation.z === -53.25);
check(value.pose.orientation.w === 35.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_153(){const input=bytes("00"),codec=r.Ros1Pal_navigation_msgsFinalApproachPoseResponseCodec,value=codec.decode(input);
check(value.done.data === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_154(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsGetMapConfigurationRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_155(){const input=bytes("bb76201d010000000100000000000000001858c00000000000385dc00000000000000fc00100c0efc2070000007632386235ceb224ee3185a6296e559de20a23070000007663363330ceb201000000000005c200002e420080a94201000000070000007632363535ceb201000000000047c301000000070000007633373333ceb2070000007634393363ceb205e002c369e727b07278152c070000007635613430ceb201000000008003c300606b43000024c201000000070000007663333163ceb2010000000040ca4201000000070000007635626465ceb2070000007632356431ceb2cdb8dcde33018a977ef9e119070000007666356537ceb20100000000005c4200808ac20000fbc201000000070000007665346231ceb2010000000000274301000000070000007665323431ceb2070000007639633465ceb23650db50c46a0786aa5f193b070000007637393837ceb201000000004020c3008053c300e05f4301000000070000007666353363ceb20100000000c0f14201000000070000007633643466ceb2070000007634333430ceb201000000070000007630656466ceb201000000070000007665383037ceb201000000e4010000008b01000000326766b3d1eb52d171cb64c22a070000007663646538ceb260e182e1d037333000a00d4319e5813136eb4a3a0000000000a061400000000000e05c400000000000002bc00000000000446d40000000000078534000000000009041400000000000946b400100000060b636cbf35748e205760e311f070000007634663230ceb2d2a79d72be4afb54070000007663343063ceb2db94b59daf01000000de0000000000885dc00000000000c460c00000000000e46e400000000000706d40"),codec=r.Ros1Pal_navigation_msgsGetMapConfigurationResponseCodec,value=codec.decode(input);
check(value.map_config.numberOfSubMaps === 488666811);
check(value.map_config.highways.length == 1);
check(value.map_config.highways[0].points.length == 1);
check(value.map_config.highways[0].points[0].x === -96.375);
check(value.map_config.highways[0].points[0].y === -116.875);
check(value.map_config.highways[0].points[0].z === -3.875);
check(value.map_config.highways[0].directional.data === true);
check(value.map_config.highways[0].line_width.data === -119.875);
check(value.map_config.highways[0].name.data === "v28b5β");
check(value.map_config.pois.points.header.seq === 2234641956);
check(value.map_config.pois.points.header.stamp.sec === 1433282982);
check(value.map_config.pois.points.header.stamp.nsec === 587915933);
check(value.map_config.pois.points.header.frame_id === "vc630β");
check(value.map_config.pois.points.points.length == 1);
check(value.map_config.pois.points.points[0].x === -33.25);
check(value.map_config.pois.points.points[0].y === 43.5);
check(value.map_config.pois.points.points[0].z === 84.75);
check(value.map_config.pois.points.channels.length == 1);
check(value.map_config.pois.points.channels[0].name === "v2655β");
check(value.map_config.pois.points.channels[0].values.length == 1);
check(value.map_config.pois.points.channels[0].values[0] === -199.0);
check(value.map_config.pois.ids.length == 1);
check(value.map_config.pois.ids[0].data === "v3733β");
check(value.map_config.pois.map_id.data === "v493cβ");
check(value.map_config.vo.points.header.seq === 3271745541);
check(value.map_config.vo.points.header.stamp.sec === 2955405161);
check(value.map_config.vo.points.header.stamp.nsec === 739604594);
check(value.map_config.vo.points.header.frame_id === "v5a40β");
check(value.map_config.vo.points.points.length == 1);
check(value.map_config.vo.points.points[0].x === -131.5);
check(value.map_config.vo.points.points[0].y === 235.375);
check(value.map_config.vo.points.points[0].z === -41.0);
check(value.map_config.vo.points.channels.length == 1);
check(value.map_config.vo.points.channels[0].name === "vc31cβ");
check(value.map_config.vo.points.channels[0].values.length == 1);
check(value.map_config.vo.points.channels[0].values[0] === 101.125);
check(value.map_config.vo.ids.length == 1);
check(value.map_config.vo.ids[0].data === "v5bdeβ");
check(value.map_config.vo.map_id.data === "v25d1β");
check(value.map_config.zoi.points.header.seq === 3739007181);
check(value.map_config.zoi.points.header.stamp.sec === 2542403891);
check(value.map_config.zoi.points.header.stamp.nsec === 434239870);
check(value.map_config.zoi.points.header.frame_id === "vf5e7β");
check(value.map_config.zoi.points.points.length == 1);
check(value.map_config.zoi.points.points[0].x === 55.0);
check(value.map_config.zoi.points.points[0].y === -69.25);
check(value.map_config.zoi.points.points[0].z === -125.5);
check(value.map_config.zoi.points.channels.length == 1);
check(value.map_config.zoi.points.channels[0].name === "ve4b1β");
check(value.map_config.zoi.points.channels[0].values.length == 1);
check(value.map_config.zoi.points.channels[0].values[0] === 167.0);
check(value.map_config.zoi.ids.length == 1);
check(value.map_config.zoi.ids[0].data === "ve241β");
check(value.map_config.zoi.map_id.data === "v9c4eβ");
check(value.map_config.ramps.points.header.seq === 1356550198);
check(value.map_config.ramps.points.header.stamp.sec === 2248633028);
check(value.map_config.ramps.points.header.stamp.nsec === 991518634);
check(value.map_config.ramps.points.header.frame_id === "v7987β");
check(value.map_config.ramps.points.points.length == 1);
check(value.map_config.ramps.points.points[0].x === -160.25);
check(value.map_config.ramps.points.points[0].y === -211.5);
check(value.map_config.ramps.points.points[0].z === 223.875);
check(value.map_config.ramps.points.channels.length == 1);
check(value.map_config.ramps.points.channels[0].name === "vf53cβ");
check(value.map_config.ramps.points.channels[0].values.length == 1);
check(value.map_config.ramps.points.channels[0].values[0] === 120.875);
check(value.map_config.ramps.ids.length == 1);
check(value.map_config.ramps.ids[0].data === "v3d4fβ");
check(value.map_config.ramps.map_id.data === "v4340β");
check(value.map_config.poigroups.length == 1);
check(value.map_config.poigroups[0].group_name.data === "v0edfβ");
check(value.map_config.poigroups[0].poi_names.length == 1);
check(value.map_config.poigroups[0].poi_names[0].data === "ve807β");
check(value.map_config.visualLocDb.documents.length == 1);
check(value.map_config.visualLocDb.documents[0] === 228);
check(value.map_config.visualLocDb.tree.length == 1);
check(value.map_config.visualLocDb.tree[0] === 139);
check(value.map_config.visualLocDb.weights.length == 1);
check(value.map_config.visualLocDb.weights[0] === 50);
check(value.map_config.navigation_map.header.seq === 3518195303);
check(value.map_config.navigation_map.header.stamp.sec === 1909543659);
check(value.map_config.navigation_map.header.stamp.nsec === 717382859);
check(value.map_config.navigation_map.header.frame_id === "vcde8β");
check(value.map_config.navigation_map.info.map_load_time.sec === 3783450976);
check(value.map_config.navigation_map.info.map_load_time.nsec === 808662992);
check(value.map_config.navigation_map.info.resolution === 141.625);
check(value.map_config.navigation_map.info.width === 830596377);
check(value.map_config.navigation_map.info.height === 977988406);
check(value.map_config.navigation_map.info.origin.position.x === 141.0);
check(value.map_config.navigation_map.info.origin.position.y === 115.5);
check(value.map_config.navigation_map.info.origin.position.z === -13.5);
check(value.map_config.navigation_map.info.origin.orientation.x === 234.125);
check(value.map_config.navigation_map.info.origin.orientation.y === 77.875);
check(value.map_config.navigation_map.info.origin.orientation.z === 35.125);
check(value.map_config.navigation_map.info.origin.orientation.w === 220.625);
check(value.map_config.navigation_map.data.length == 1);
check(value.map_config.navigation_map.data[0] === 96);
check(value.map_config.user_map.header.seq === 4090181302);
check(value.map_config.user_map.header.stamp.sec === 98715735);
check(value.map_config.user_map.header.stamp.nsec === 523308662);
check(value.map_config.user_map.header.frame_id === "v4f20β");
check(value.map_config.user_map.height === 1922934738);
check(value.map_config.user_map.width === 1425754814);
check(value.map_config.user_map.encoding === "vc40cβ");
check(value.map_config.user_map.is_bigendian === 219);
check(value.map_config.user_map.step === 2946348436);
check(value.map_config.user_map.data.length == 1);
check(value.map_config.user_map.data[0] === 222);
check(value.map_config.transform.x === -118.125);
check(value.map_config.transform.y === -134.125);
check(value.map_config.transform.rotYawRad === 247.125);
check(value.map_config.transform.scale === 235.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_156(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsGetNodesRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_157(){const input=bytes("552a"),codec=r.Ros1Pal_navigation_msgsGetNodesResponseCodec,value=codec.decode(input);
check(value.node === 85);
check(value.totPOI === 42);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_158(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsGetPOIRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_159(){const input=bytes("2dcf4147689cd4779f81ef31070000007637653933ceb20100000000e036c3006079c300c0b9c201000000070000007630346566ceb2010000000040ccc201000000070000007661376230ceb2070000007664396266ceb2"),codec=r.Ros1Pal_navigation_msgsGetPOIResponseCodec,value=codec.decode(input);
check(value.poi.points.header.seq === 1195495213);
check(value.poi.points.header.stamp.sec === 2010422376);
check(value.poi.points.header.stamp.nsec === 837779871);
check(value.poi.points.header.frame_id === "v7e93β");
check(value.poi.points.points.length == 1);
check(value.poi.points.points[0].x === -182.875);
check(value.poi.points.points[0].y === -249.375);
check(value.poi.points.points[0].z === -92.875);
check(value.poi.points.channels.length == 1);
check(value.poi.points.channels[0].name === "v04efβ");
check(value.poi.points.channels[0].values.length == 1);
check(value.poi.points.channels[0].values[0] === -102.125);
check(value.poi.ids.length == 1);
check(value.poi.ids[0].data === "va7b0β");
check(value.poi.map_id.data === "vd9bfβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_160(){const input=bytes("1f"),codec=r.Ros1Pal_navigation_msgsGetSubMapRequestCodec,value=codec.decode(input);
check(value.index === 31);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_161(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsGetSubMapResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_162(){const input=bytes("cbd6a079c37f0f9a1c56fc35070000007631646633ceb20000000000985fc000000000007049c00000000000085d400000000000c86ec00000000000286cc000000000003052c000000000003852c0"),codec=r.Ros1Pal_navigation_msgsGoToFeedbackCodec,value=codec.decode(input);
check(value.base_position.header.seq === 2040583883);
check(value.base_position.header.stamp.sec === 2584707011);
check(value.base_position.header.stamp.nsec === 905729564);
check(value.base_position.header.frame_id === "v1df3β");
check(value.base_position.pose.position.x === -126.375);
check(value.base_position.pose.position.y === -50.875);
check(value.base_position.pose.position.z === 116.125);
check(value.base_position.pose.orientation.x === -246.25);
check(value.base_position.pose.orientation.y === -225.25);
check(value.base_position.pose.orientation.z === -72.75);
check(value.base_position.pose.orientation.w === -72.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_163(){const input=bytes("c6ac25599a943977a7432b0c070000007639333962ceb20000000000e864c00000000000c04cc00000000000dc6f400000000000d05cc00000000000306dc000000000005052c00000000000f04bc0"),codec=r.Ros1Pal_navigation_msgsGoToGoalCodec,value=codec.decode(input);
check(value.target_pose.header.seq === 1495641286);
check(value.target_pose.header.stamp.sec === 2000262298);
check(value.target_pose.header.stamp.nsec === 204161959);
check(value.target_pose.header.frame_id === "v939bβ");
check(value.target_pose.pose.position.x === -167.25);
check(value.target_pose.pose.position.y === -57.5);
check(value.target_pose.pose.position.z === 254.875);
check(value.target_pose.pose.orientation.x === -115.25);
check(value.target_pose.pose.orientation.y === -233.5);
check(value.target_pose.pose.orientation.z === -73.25);
check(value.target_pose.pose.orientation.w === -55.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_164(){const input=bytes("e5b85821e993d6a026a44d33070000007637313465ceb20000000000f86b400000000000d865400000000000586d400000000000d467400000000000d850400000000000b045c00000000000a05bc0"),codec=r.Ros1Pal_navigation_msgsGoToPOIFeedbackCodec,value=codec.decode(input);
check(value.base_position.header.seq === 559462629);
check(value.base_position.header.stamp.sec === 2698417129);
check(value.base_position.header.stamp.nsec === 860726310);
check(value.base_position.header.frame_id === "v714eβ");
check(value.base_position.pose.position.x === 223.75);
check(value.base_position.pose.position.y === 174.75);
check(value.base_position.pose.position.z === 234.75);
check(value.base_position.pose.orientation.x === 190.625);
check(value.base_position.pose.orientation.y === 67.375);
check(value.base_position.pose.orientation.z === -43.375);
check(value.base_position.pose.orientation.w === -110.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_165(){const input=bytes("070000007663396363ceb2"),codec=r.Ros1Pal_navigation_msgsGoToPOIGoalCodec,value=codec.decode(input);
check(value.poi.data === "vc9ccβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_166(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsGoToPOIResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_167(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsGoToResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_168(){const input=bytes("010000000000000000c05440000000000018604000000000000054c00100c0f6c2070000007638333863ceb2"),codec=r.Ros1Pal_navigation_msgsHighwaysCodec,value=codec.decode(input);
check(value.points.length == 1);
check(value.points[0].x === 83.0);
check(value.points[0].y === 128.75);
check(value.points[0].z === -80.0);
check(value.directional.data === true);
check(value.line_width.data === -123.375);
check(value.name.data === "v838cβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_169(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsJoyPriorityFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_170(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsJoyPriorityGoalCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_171(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsJoyPriorityResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_172(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsJoyTurboFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_173(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsJoyTurboGoalCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_174(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsJoyTurboResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_175(){const input=bytes("5c41ab52388c531a7b65530f070000007632343637ceb200000000009862c00000000000e042400000000000a86c40b20eebfb800703a14ea82207070000007637666162ceb2070000007631666635ceb201000000a000c092c200c0674300806b42"),codec=r.Ros1Pal_navigation_msgsLaserImageCodec,value=codec.decode(input);
check(value.header.seq === 1386955100);
check(value.header.stamp.sec === 441683000);
check(value.header.stamp.nsec === 257123707);
check(value.header.frame_id === "v2467β");
check(value.origin.x === -148.75);
check(value.origin.y === 37.75);
check(value.origin.z === 229.25);
check(value.image.header.seq === 4226485938);
check(value.image.header.stamp.sec === 2701330304);
check(value.image.header.stamp.nsec === 119711822);
check(value.image.header.frame_id === "v7fabβ");
check(value.image.format === "v1ff5β");
check(value.image.data.length == 1);
check(value.image.data[0] === 160);
check(value.height === -73.375);
check(value.width === 231.75);
check(value.resolution === 58.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_176(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsListMapsRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_177(){const input=bytes("0001000000070000007636633961ceb2"),codec=r.Ros1Pal_navigation_msgsListMapsResponseCodec,value=codec.decode(input);
check(value.success === false);
check(value.maps.length == 1);
check(value.maps[0] === "v6c9aβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_178(){const input=bytes("d624f24901000000010000000000000000b468c000000000004465c00000000000885ec00000c0aec2070000007633396534ceb231de4c995d203d2b00105f11070000007632343435ceb2010000000080e54200009c4200c07d4301000000070000007635383036ceb20100000000408cc201000000070000007630343634ceb2070000007637613662ceb2deed7e93a0fe1b7d7fc6f500070000007662313566ceb20100000000c01a430000664200c0364301000000070000007630653230ceb20100000000c0ecc201000000070000007635376462ceb2070000007632396434ceb29ccf8dcf3e47ff7585ca2f2b070000007633386462ceb20100000000e00143008003420040dcc201000000070000007630366334ceb2010000000000124301000000070000007662363933ceb2070000007663383963ceb2edc91a3d591216ac2bafb509070000007630376434ceb2010000000000e3420000db42000068c201000000070000007664663264ceb20100000000c0f74201000000070000007636383739ceb2070000007631363736ceb201000000070000007637303863ceb201000000070000007636363933ceb2010000002101000000fb0100000027923cac3aa375e0eb4ea6d92d070000007634333763ceb297387bc9b6c1630d00c0c142ecbf9edafff276f70000000000d051c00000000000d06ac000000000003c65400000000000cc6140000000000070434000000000005042c000000000007c6c400100000075c6d1b78aa212fdeebfb93c1f070000007635653463ceb2bff54f26480efe58070000007639313361ceb28a5f7bc21c01000000690000000000203ec00000000000e462c00000000000a46a400000000000c06b40"),codec=r.Ros1Pal_navigation_msgsMapConfigurationCodec,value=codec.decode(input);
check(value.numberOfSubMaps === 1240605910);
check(value.highways.length == 1);
check(value.highways[0].points.length == 1);
check(value.highways[0].points[0].x === -197.625);
check(value.highways[0].points[0].y === -170.125);
check(value.highways[0].points[0].z === -122.125);
check(value.highways[0].directional.data === false);
check(value.highways[0].line_width.data === -87.375);
check(value.highways[0].name.data === "v39e4β");
check(value.pois.points.header.seq === 2571951665);
check(value.pois.points.header.stamp.sec === 725426269);
check(value.pois.points.header.stamp.nsec === 291442688);
check(value.pois.points.header.frame_id === "v2445β");
check(value.pois.points.points.length == 1);
check(value.pois.points.points[0].x === 114.75);
check(value.pois.points.points[0].y === 78.0);
check(value.pois.points.points[0].z === 253.75);
check(value.pois.points.channels.length == 1);
check(value.pois.points.channels[0].name === "v5806β");
check(value.pois.points.channels[0].values.length == 1);
check(value.pois.points.channels[0].values[0] === -70.125);
check(value.pois.ids.length == 1);
check(value.pois.ids[0].data === "v0464β");
check(value.pois.map_id.data === "v7a6bβ");
check(value.vo.points.header.seq === 2474569182);
check(value.vo.points.header.stamp.sec === 2098986656);
check(value.vo.points.header.stamp.nsec === 16107135);
check(value.vo.points.header.frame_id === "vb15fβ");
check(value.vo.points.points.length == 1);
check(value.vo.points.points[0].x === 154.75);
check(value.vo.points.points[0].y === 57.5);
check(value.vo.points.points[0].z === 182.75);
check(value.vo.points.channels.length == 1);
check(value.vo.points.channels[0].name === "v0e20β");
check(value.vo.points.channels[0].values.length == 1);
check(value.vo.points.channels[0].values[0] === -118.375);
check(value.vo.ids.length == 1);
check(value.vo.ids[0].data === "v57dbβ");
check(value.vo.map_id.data === "v29d4β");
check(value.zoi.points.header.seq === 3482177436);
check(value.zoi.points.header.stamp.sec === 1979664190);
check(value.zoi.points.header.stamp.nsec === 724552325);
check(value.zoi.points.header.frame_id === "v38dbβ");
check(value.zoi.points.points.length == 1);
check(value.zoi.points.points[0].x === 129.875);
check(value.zoi.points.points[0].y === 32.875);
check(value.zoi.points.points[0].z === -110.125);
check(value.zoi.points.channels.length == 1);
check(value.zoi.points.channels[0].name === "v06c4β");
check(value.zoi.points.channels[0].values.length == 1);
check(value.zoi.points.channels[0].values[0] === 146.0);
check(value.zoi.ids.length == 1);
check(value.zoi.ids[0].data === "vb693β");
check(value.zoi.map_id.data === "vc89cβ");
check(value.ramps.points.header.seq === 1025165805);
check(value.ramps.points.header.stamp.sec === 2887127641);
check(value.ramps.points.header.stamp.nsec === 162901803);
check(value.ramps.points.header.frame_id === "v07d4β");
check(value.ramps.points.points.length == 1);
check(value.ramps.points.points[0].x === 113.5);
check(value.ramps.points.points[0].y === 109.5);
check(value.ramps.points.points[0].z === -58.0);
check(value.ramps.points.channels.length == 1);
check(value.ramps.points.channels[0].name === "vdf2dβ");
check(value.ramps.points.channels[0].values.length == 1);
check(value.ramps.points.channels[0].values[0] === 123.875);
check(value.ramps.ids.length == 1);
check(value.ramps.ids[0].data === "v6879β");
check(value.ramps.map_id.data === "v1676β");
check(value.poigroups.length == 1);
check(value.poigroups[0].group_name.data === "v708cβ");
check(value.poigroups[0].poi_names.length == 1);
check(value.poigroups[0].poi_names[0].data === "v6693β");
check(value.visualLocDb.documents.length == 1);
check(value.visualLocDb.documents[0] === 33);
check(value.visualLocDb.tree.length == 1);
check(value.visualLocDb.tree[0] === 251);
check(value.visualLocDb.weights.length == 1);
check(value.visualLocDb.weights[0] === 39);
check(value.navigation_map.header.seq === 984366226);
check(value.navigation_map.header.stamp.sec === 3957355939);
check(value.navigation_map.header.stamp.nsec === 769238606);
check(value.navigation_map.header.frame_id === "v437cβ");
check(value.navigation_map.info.map_load_time.sec === 3380295831);
check(value.navigation_map.info.map_load_time.nsec === 224641462);
check(value.navigation_map.info.resolution === 96.875);
check(value.navigation_map.info.width === 3667836908);
check(value.navigation_map.info.height === 4151767807);
check(value.navigation_map.info.origin.position.x === -71.25);
check(value.navigation_map.info.origin.position.y === -214.5);
check(value.navigation_map.info.origin.position.z === 169.875);
check(value.navigation_map.info.origin.orientation.x === 142.375);
check(value.navigation_map.info.origin.orientation.y === 38.875);
check(value.navigation_map.info.origin.orientation.z === -36.625);
check(value.navigation_map.info.origin.orientation.w === 227.875);
check(value.navigation_map.data.length == 1);
check(value.navigation_map.data[0] === 117);
check(value.user_map.header.seq === 2327302598);
check(value.user_map.header.stamp.sec === 4009562786);
check(value.user_map.header.stamp.nsec === 524073407);
check(value.user_map.header.frame_id === "v5e4cβ");
check(value.user_map.height === 642774463);
check(value.user_map.width === 1493044808);
check(value.user_map.encoding === "v913aβ");
check(value.user_map.is_bigendian === 138);
check(value.user_map.step === 482507615);
check(value.user_map.data.length == 1);
check(value.user_map.data[0] === 105);
check(value.transform.x === -30.125);
check(value.transform.y === -151.125);
check(value.transform.rotYawRad === 213.125);
check(value.transform.scale === 222.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_179(){const input=bytes("070000007636356564ceb2070000007663316635ceb2"),codec=r.Ros1Pal_navigation_msgsNavigationStatusCodec,value=codec.decode(input);
check(value.status.data === "v65edβ");
check(value.msg.data === "vc1f5β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_180(){const input=bytes("00000000006046c00000000000c04cc00000000000a048c00000000000b86740"),codec=r.Ros1Pal_navigation_msgsNiceMapTransformationCodec,value=codec.decode(input);
check(value.x === -44.75);
check(value.y === -57.5);
check(value.rotYawRad === -49.25);
check(value.scale === 189.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_181(){const input=bytes("1416a3f003b6236c6590c337070000007638383163ceb20100000000801e4200600d43000065c301000000070000007631663138ceb20100000000009b4201000000070000007638316365ceb2070000007666666331ceb2"),codec=r.Ros1Pal_navigation_msgsPOICodec,value=codec.decode(input);
check(value.points.header.seq === 4037219860);
check(value.points.header.stamp.sec === 1814279683);
check(value.points.header.stamp.nsec === 935563365);
check(value.points.header.frame_id === "v881cβ");
check(value.points.points.length == 1);
check(value.points.points[0].x === 39.625);
check(value.points.points[0].y === 141.375);
check(value.points.points[0].z === -229.0);
check(value.points.channels.length == 1);
check(value.points.channels[0].name === "v1f18β");
check(value.points.channels[0].values.length == 1);
check(value.points.channels[0].values[0] === 77.5);
check(value.ids.length == 1);
check(value.ids[0].data === "v81ceβ");
check(value.map_id.data === "vffc1β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_182(){const input=bytes("070000007631353564ceb201000000070000007631343133ceb2"),codec=r.Ros1Pal_navigation_msgsPOIGroupCodec,value=codec.decode(input);
check(value.group_name.data === "v155dβ");
check(value.poi_names.length == 1);
check(value.poi_names[0].data === "v1413β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_183(){const input=bytes("00a03dc300602ac3"),codec=r.Ros1Pal_navigation_msgsPolarReadingCodec,value=codec.decode(input);
check(value.angle === -189.625);
check(value.distance === -170.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_184(){const input=bytes("69749572a2e49c0f49181c20070000007633313232ceb20100000000a011c30000d041"),codec=r.Ros1Pal_navigation_msgsPolarReadingScanCodec,value=codec.decode(input);
check(value.header.seq === 1922397289);
check(value.header.stamp.sec === 261940386);
check(value.header.stamp.nsec === 538712137);
check(value.header.frame_id === "v3122β");
check(value.readings.length == 1);
check(value.readings[0].angle === -145.625);
check(value.readings[0].distance === 26.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_185(){const input=bytes("070000007665643364ceb2070000007630363735ceb2"),codec=r.Ros1Pal_navigation_msgsRenameMapRequestCodec,value=codec.decode(input);
check(value.current_map_name === "ved3dβ");
check(value.new_map_name === "v0675β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_186(){const input=bytes("00070000007637363163ceb2"),codec=r.Ros1Pal_navigation_msgsRenameMapResponseCodec,value=codec.decode(input);
check(value.success === false);
check(value.message === "v761cβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_187(){const input=bytes("0000000000402dc00000000000004bc000000000003868c05fa0cdb8"),codec=r.Ros1Pal_navigation_msgsSafetyZoneRequestCodec,value=codec.decode(input);
check(value.distance === -14.625);
check(value.leftAngleRad === -54.0);
check(value.rightAngleRad === -193.75);
check(value.sensorMask === 3100483679);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_188(){const input=bytes("0100000000003867c00000000000b040400100000000007c66400000000000002b40"),codec=r.Ros1Pal_navigation_msgsSafetyZoneResponseCodec,value=codec.decode(input);
check(value.leftSafe === true);
check(value.leftMinDist === -185.75);
check(value.leftMinDistAngleRad === 33.375);
check(value.rightSafe === true);
check(value.rightMinDist === 179.875);
check(value.rightMinDistAngleRad === 13.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_189(){const input=bytes("070000007666343136ceb2"),codec=r.Ros1Pal_navigation_msgsSaveMapRequestCodec,value=codec.decode(input);
check(value.directory === "vf416β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_190(){const input=bytes("01070000007638613263ceb2070000007630663438ceb2070000007663333064ceb2"),codec=r.Ros1Pal_navigation_msgsSaveMapResponseCodec,value=codec.decode(input);
check(value.success === true);
check(value.name === "v8a2cβ");
check(value.full_path === "v0f48β");
check(value.message === "vc30dβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_191(){const input=bytes("070000007661323239ceb2000000f241288511141633ca2c00"),codec=r.Ros1Pal_navigation_msgsServiceStatusCodec,value=codec.decode(input);
check(value.serviceName === "va229β");
check(value.is_running === false);
check(value.update_frequency === 30.25);
check(value.latest_run_time.sec === 336692520);
check(value.latest_run_time.nsec === 751448854);
check(value.status === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_192(){const input=bytes("f5f3ccd201000000010000000000000000904fc00000000000f043c00000000000446ac0000000803e070000007636326262ceb2e5b1721c01c8fa4e374e3d26070000007635333734ceb20100000000e01cc300602ac3006060c301000000070000007633646331ceb20100000000c0b5c201000000070000007661363938ceb2070000007664383937ceb261a2dc12a130b14b27400610070000007631386462ceb20100000000002c420000d8420080284301000000070000007633383861ceb20100000000c0e2c201000000070000007630646365ceb2070000007637336331ceb2cec3d29466ddce02d9584d13070000007630653731ceb20100000000801c4300009a410000214301000000070000007637316635ceb20100000000004a4301000000070000007632646164ceb2070000007635336132ceb24fe121bc6ec8aecefe006505070000007636323133ceb20100000000c0d2420000b5410040c1c201000000070000007662643935ceb2010000000000d3c201000000070000007663653637ceb2070000007662303638ceb201000000070000007631353462ceb201000000070000007664653463ceb201000000cd010000002f01000000f338cb2893e58ce11d12df463a070000007666626133ceb2cd8bab5cef3db20f006017c346481a73fe3cdcc1000000000080554000000000008024c00000000000004a4000000000001c63400000000000a039c00000000000485dc00000000000a86ac001000000a112a3ee9608e57947be7bd731070000007636363135ceb29c2271bd889deb02070000007633373234ceb2d8a1a08e0c01000000cc00000000004061c000000000006861c00000000000804b400000000000285440"),codec=r.Ros1Pal_navigation_msgsSetMapConfigurationRequestCodec,value=codec.decode(input);
check(value.map_config.numberOfSubMaps === -758320139);
check(value.map_config.highways.length == 1);
check(value.map_config.highways[0].points.length == 1);
check(value.map_config.highways[0].points[0].x === -63.125);
check(value.map_config.highways[0].points[0].y === -39.875);
check(value.map_config.highways[0].points[0].z === -210.125);
check(value.map_config.highways[0].directional.data === false);
check(value.map_config.highways[0].line_width.data === 0.25);
check(value.map_config.highways[0].name.data === "v62bbβ");
check(value.map_config.pois.points.header.seq === 477278693);
check(value.map_config.pois.points.header.stamp.sec === 1325058049);
check(value.map_config.pois.points.header.stamp.nsec === 641551927);
check(value.map_config.pois.points.header.frame_id === "v5374β");
check(value.map_config.pois.points.points.length == 1);
check(value.map_config.pois.points.points[0].x === -156.875);
check(value.map_config.pois.points.points[0].y === -170.375);
check(value.map_config.pois.points.points[0].z === -224.375);
check(value.map_config.pois.points.channels.length == 1);
check(value.map_config.pois.points.channels[0].name === "v3dc1β");
check(value.map_config.pois.points.channels[0].values.length == 1);
check(value.map_config.pois.points.channels[0].values[0] === -90.875);
check(value.map_config.pois.ids.length == 1);
check(value.map_config.pois.ids[0].data === "va698β");
check(value.map_config.pois.map_id.data === "vd897β");
check(value.map_config.vo.points.header.seq === 316449377);
check(value.map_config.vo.points.header.stamp.sec === 1269903521);
check(value.map_config.vo.points.header.stamp.nsec === 268845095);
check(value.map_config.vo.points.header.frame_id === "v18dbβ");
check(value.map_config.vo.points.points.length == 1);
check(value.map_config.vo.points.points[0].x === 43.0);
check(value.map_config.vo.points.points[0].y === 108.0);
check(value.map_config.vo.points.points[0].z === 168.5);
check(value.map_config.vo.points.channels.length == 1);
check(value.map_config.vo.points.channels[0].name === "v388aβ");
check(value.map_config.vo.points.channels[0].values.length == 1);
check(value.map_config.vo.points.channels[0].values[0] === -113.375);
check(value.map_config.vo.ids.length == 1);
check(value.map_config.vo.ids[0].data === "v0dceβ");
check(value.map_config.vo.map_id.data === "v73c1β");
check(value.map_config.zoi.points.header.seq === 2496840654);
check(value.map_config.zoi.points.header.stamp.sec === 47111526);
check(value.map_config.zoi.points.header.stamp.nsec === 323836121);
check(value.map_config.zoi.points.header.frame_id === "v0e71β");
check(value.map_config.zoi.points.points.length == 1);
check(value.map_config.zoi.points.points[0].x === 156.5);
check(value.map_config.zoi.points.points[0].y === 19.25);
check(value.map_config.zoi.points.points[0].z === 161.0);
check(value.map_config.zoi.points.channels.length == 1);
check(value.map_config.zoi.points.channels[0].name === "v71f5β");
check(value.map_config.zoi.points.channels[0].values.length == 1);
check(value.map_config.zoi.points.channels[0].values[0] === 202.0);
check(value.map_config.zoi.ids.length == 1);
check(value.map_config.zoi.ids[0].data === "v2dadβ");
check(value.map_config.zoi.map_id.data === "v53a2β");
check(value.map_config.ramps.points.header.seq === 3156336975);
check(value.map_config.ramps.points.header.stamp.sec === 3467561070);
check(value.map_config.ramps.points.header.stamp.nsec === 90505470);
check(value.map_config.ramps.points.header.frame_id === "v6213β");
check(value.map_config.ramps.points.points.length == 1);
check(value.map_config.ramps.points.points[0].x === 105.375);
check(value.map_config.ramps.points.points[0].y === 22.625);
check(value.map_config.ramps.points.points[0].z === -96.625);
check(value.map_config.ramps.points.channels.length == 1);
check(value.map_config.ramps.points.channels[0].name === "vbd95β");
check(value.map_config.ramps.points.channels[0].values.length == 1);
check(value.map_config.ramps.points.channels[0].values[0] === -105.5);
check(value.map_config.ramps.ids.length == 1);
check(value.map_config.ramps.ids[0].data === "vce67β");
check(value.map_config.ramps.map_id.data === "vb068β");
check(value.map_config.poigroups.length == 1);
check(value.map_config.poigroups[0].group_name.data === "v154bβ");
check(value.map_config.poigroups[0].poi_names.length == 1);
check(value.map_config.poigroups[0].poi_names[0].data === "vde4cβ");
check(value.map_config.visualLocDb.documents.length == 1);
check(value.map_config.visualLocDb.documents[0] === 205);
check(value.map_config.visualLocDb.tree.length == 1);
check(value.map_config.visualLocDb.tree[0] === 47);
check(value.map_config.visualLocDb.weights.length == 1);
check(value.map_config.visualLocDb.weights[0] === 243);
check(value.map_config.navigation_map.header.seq === 2468924216);
check(value.map_config.navigation_map.header.stamp.sec === 501320933);
check(value.map_config.navigation_map.header.stamp.nsec === 977723154);
check(value.map_config.navigation_map.header.frame_id === "vfba3β");
check(value.map_config.navigation_map.info.map_load_time.sec === 1554746317);
check(value.map_config.navigation_map.info.map_load_time.nsec === 263339503);
check(value.map_config.navigation_map.info.resolution === -151.375);
check(value.map_config.navigation_map.info.width === 1931102278);
check(value.map_config.navigation_map.info.height === 3252436222);
check(value.map_config.navigation_map.info.origin.position.x === 86.0);
check(value.map_config.navigation_map.info.origin.position.y === -10.25);
check(value.map_config.navigation_map.info.origin.position.z === 52.0);
check(value.map_config.navigation_map.info.origin.orientation.x === 152.875);
check(value.map_config.navigation_map.info.origin.orientation.y === -25.625);
check(value.map_config.navigation_map.info.origin.orientation.z === -117.125);
check(value.map_config.navigation_map.info.origin.orientation.w === -213.25);
check(value.map_config.navigation_map.data.length == 1);
check(value.map_config.navigation_map.data[0] === -95);
check(value.map_config.user_map.header.seq === 2532221714);
check(value.map_config.user_map.header.stamp.sec === 1199170824);
check(value.map_config.user_map.header.stamp.nsec === 836205502);
check(value.map_config.user_map.header.frame_id === "v6615β");
check(value.map_config.user_map.height === 3178308252);
check(value.map_config.user_map.width === 48995720);
check(value.map_config.user_map.encoding === "v3724β");
check(value.map_config.user_map.is_bigendian === 216);
check(value.map_config.user_map.step === 210673825);
check(value.map_config.user_map.data.length == 1);
check(value.map_config.user_map.data[0] === 204);
check(value.map_config.transform.x === -138.0);
check(value.map_config.transform.y === -139.25);
check(value.map_config.transform.rotYawRad === 55.0);
check(value.map_config.transform.scale === 80.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_193(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsSetMapConfigurationResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_194(){const input=bytes("f2571a9f68b25ceab1098332070000007632346361ceb2010000000080c5c20000f1c200003b4201000000070000007639393637ceb201000000000004c301000000070000007632393465ceb2070000007635373431ceb2"),codec=r.Ros1Pal_navigation_msgsSetPOIRequestCodec,value=codec.decode(input);
check(value.poi.points.header.seq === 2669303794);
check(value.poi.points.header.stamp.sec === 3931943528);
check(value.poi.points.header.stamp.nsec === 847448497);
check(value.poi.points.header.frame_id === "v24caβ");
check(value.poi.points.points.length == 1);
check(value.poi.points.points[0].x === -98.75);
check(value.poi.points.points[0].y === -120.5);
check(value.poi.points.points[0].z === 46.75);
check(value.poi.points.channels.length == 1);
check(value.poi.points.channels[0].name === "v9967β");
check(value.poi.points.channels[0].values.length == 1);
check(value.poi.points.channels[0].values[0] === -132.0);
check(value.poi.ids.length == 1);
check(value.poi.ids[0].data === "v294eβ");
check(value.poi.map_id.data === "v5741β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_195(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsSetPOIResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_196(){const input=bytes("8fa0"),codec=r.Ros1Pal_navigation_msgsSetSubMapFloorRequestCodec,value=codec.decode(input);
check(value.index === -113);
check(value.floor === -96);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_197(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsSetSubMapFloorResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_198(){const input=bytes("0100000000000000001c624000000000007c614000000000003860c001000000070000007665353965ceb2070000007635323530ceb2"),codec=r.Ros1Pal_navigation_msgsTabletPOICodec,value=codec.decode(input);
check(value.POIs.length == 1);
check(value.POIs[0].x === 144.875);
check(value.POIs[0].y === 139.875);
check(value.POIs[0].z === -129.75);
check(value.IDs.length == 1);
check(value.IDs[0].data === "ve59eβ");
check(value.map_id.data === "v5250β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_199(){const input=bytes("010000009801000000c5010000003a"),codec=r.Ros1Pal_navigation_msgsVisualLocDBCodec,value=codec.decode(input);
check(value.documents.length == 1);
check(value.documents[0] === 152);
check(value.tree.length == 1);
check(value.tree[0] === 197);
check(value.weights.length == 1);
check(value.weights[0] === 58);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_200(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsVisualLocRecognizeRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_201(){const input=bytes("000000000000c05b40285095b839a7e163c6e38c02070000007634616634ceb200000000008017c00000000000b049c000000000000c67c0000000000000274000000000009057c000000000009069c00000000000a469c000000000008021400000000000c85cc00000000000806fc00000000000ac65c00000000000c03ec00000000000a85ec00000000000e05d400000000000c46c4000000000004059400000000000906fc00000000000803ec00000000000c028400000000000386bc00000000000c869400000000000b04fc0000000000000274000000000004c63400000000000b058400000000000f060c000000000005468c000000000000858c000000000008026c00000000000ac65c00000000000346e4000000000005465c00000000000e04ac00000000000a46f4000000000009067400000000000f467400000000000d065400000000000e861c000000000005854c00000000000c4604000000000006050400000000000c040400000000000d85f40"),codec=r.Ros1Pal_navigation_msgsVisualLocRecognizeResponseCodec,value=codec.decode(input);
check(value.success === false);
check(value.confidence === 111.0);
check(value.estimatedPose.header.seq === 3096793128);
check(value.estimatedPose.header.stamp.sec === 1675732793);
check(value.estimatedPose.header.stamp.nsec === 42787782);
check(value.estimatedPose.header.frame_id === "v4af4β");
check(value.estimatedPose.pose.pose.position.x === -5.875);
check(value.estimatedPose.pose.pose.position.y === -51.375);
check(value.estimatedPose.pose.pose.position.z === -184.375);
check(value.estimatedPose.pose.pose.orientation.x === 11.5);
check(value.estimatedPose.pose.pose.orientation.y === -94.25);
check(value.estimatedPose.pose.pose.orientation.z === -204.5);
check(value.estimatedPose.pose.pose.orientation.w === -205.125);
check(value.estimatedPose.pose.covariance.length == 36);
check(value.estimatedPose.pose.covariance[0] === 8.75);
check(value.estimatedPose.pose.covariance[1] === -115.125);
check(value.estimatedPose.pose.covariance[2] === -252.0);
check(value.estimatedPose.pose.covariance[3] === -173.375);
check(value.estimatedPose.pose.covariance[4] === -30.75);
check(value.estimatedPose.pose.covariance[5] === -122.625);
check(value.estimatedPose.pose.covariance[6] === 119.5);
check(value.estimatedPose.pose.covariance[7] === 230.125);
check(value.estimatedPose.pose.covariance[8] === 101.0);
check(value.estimatedPose.pose.covariance[9] === -252.5);
check(value.estimatedPose.pose.covariance[10] === -30.5);
check(value.estimatedPose.pose.covariance[11] === 12.375);
check(value.estimatedPose.pose.covariance[12] === -217.75);
check(value.estimatedPose.pose.covariance[13] === 206.25);
check(value.estimatedPose.pose.covariance[14] === -63.375);
check(value.estimatedPose.pose.covariance[15] === 11.5);
check(value.estimatedPose.pose.covariance[16] === 154.375);
check(value.estimatedPose.pose.covariance[17] === 98.75);
check(value.estimatedPose.pose.covariance[18] === -135.5);
check(value.estimatedPose.pose.covariance[19] === -194.625);
check(value.estimatedPose.pose.covariance[20] === -96.125);
check(value.estimatedPose.pose.covariance[21] === -11.25);
check(value.estimatedPose.pose.covariance[22] === -173.375);
check(value.estimatedPose.pose.covariance[23] === 241.625);
check(value.estimatedPose.pose.covariance[24] === -170.625);
check(value.estimatedPose.pose.covariance[25] === -53.75);
check(value.estimatedPose.pose.covariance[26] === 253.125);
check(value.estimatedPose.pose.covariance[27] === 188.5);
check(value.estimatedPose.pose.covariance[28] === 191.625);
check(value.estimatedPose.pose.covariance[29] === 174.5);
check(value.estimatedPose.pose.covariance[30] === -143.25);
check(value.estimatedPose.pose.covariance[31] === -81.375);
check(value.estimatedPose.pose.covariance[32] === 134.125);
check(value.estimatedPose.pose.covariance[33] === 65.5);
check(value.estimatedPose.pose.covariance[34] === 33.5);
check(value.estimatedPose.pose.covariance[35] === 127.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_202(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsVisualTrainingFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_203(){const input=bytes("7b070000007666623533ceb2"),codec=r.Ros1Pal_navigation_msgsVisualTrainingGoalCodec,value=codec.decode(input);
check(value.action === 123);
check(value.database_path === "vfb53β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_204(){const input=bytes(""),codec=r.Ros1Pal_navigation_msgsVisualTrainingResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_205(){const input=bytes("4c0ad5ec4ee386721d2ef803070000007632636262ceb2070000007665613136ceb20000000000686cc00000000000d4674000000000007055c000000000000049c00000000000f05f4000000000002468c00000000000b861c000000000000011c00000000000604740fa362eda5dbfb804"),codec=r.Ros1Pal_simulation_msgsExternalWrenchCodec,value=codec.decode(input);
check(value.header.seq === 3973384780);
check(value.header.stamp.sec === 1921442638);
check(value.header.stamp.nsec === 66596381);
check(value.header.frame_id === "v2cbbβ");
check(value.link_name === "vea16β");
check(value.wrench.force.x === -227.25);
check(value.wrench.force.y === 190.625);
check(value.wrench.force.z === -85.75);
check(value.wrench.torque.x === -50.0);
check(value.wrench.torque.y === 127.75);
check(value.wrench.torque.z === -193.125);
check(value.application_point.x === -141.75);
check(value.application_point.y === -4.25);
check(value.application_point.z === 46.75);
check(value.duration.sec === -634505478);
check(value.duration.nsec === 79216477);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_206(){const input=bytes("070000007661346532ceb201"),codec=r.Ros1Pal_tablet_msgsFunctionalityStatusCodec,value=codec.decode(input);
check(value.errMessage.data === "va4e2β");
check(value.errStatus.data === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_207(){const input=bytes("01000000070000007661623865ceb200"),codec=r.Ros1Pal_tablet_msgsRobotStatusCodec,value=codec.decode(input);
check(value.messages.length == 1);
check(value.messages[0].errMessage.data === "vab8eβ");
check(value.messages[0].errStatus.data === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_208(){const input=bytes(""),codec=r.Ros1Pal_video_recording_msgsStartRecordingRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_209(){const input=bytes(""),codec=r.Ros1Pal_video_recording_msgsStartRecordingResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_210(){const input=bytes(""),codec=r.Ros1Pal_video_recording_msgsStopRecordingRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_211(){const input=bytes(""),codec=r.Ros1Pal_video_recording_msgsStopRecordingResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_212(){const input=bytes("5e9e5d91d9a4da6cbd985815070000007637313065ceb20000000000e46dc00000000000fc6ec000000000009461c0"),codec=r.Ros1Pal_vision_msgsDetectedObjectCodec,value=codec.decode(input);
check(value.header.seq === 2438831710);
check(value.header.stamp.sec === 1826268377);
check(value.header.stamp.nsec === 358127805);
check(value.header.frame_id === "v710eβ");
check(value.position3D.x === -239.125);
check(value.position3D.y === -247.875);
check(value.position3D.z === -140.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_213(){const input=bytes("46775b56985e10174123f814070000007630616466ceb2d9d929e060a01e7e00c068c3004071c3000047c20080ac420000000000b0534000000000009048c00000000000e037400000000000dc6740439cd05efae5e7c0d5acd7296cd5e0b78afc40f03385776e39ad997f80d4aee1ab1a15bc126322223d2a12cb845325553052bafe892b8d60050d2354bc7414ca"),codec=r.Ros1Pal_vision_msgsDetectedPersonCodec,value=codec.decode(input);
check(value.header.seq === 1448834886);
check(value.header.stamp.sec === 386948760);
check(value.header.stamp.nsec === 351806273);
check(value.header.frame_id === "v0adfβ");
check(value.objectId === 9087877436018383321n);
check(value.boxColour.r === -232.75);
check(value.boxColour.g === -241.25);
check(value.boxColour.b === -49.75);
check(value.boxColour.a === 86.25);
check(value.confidence === 78.75);
check(value.position3D.x === -49.125);
check(value.position3D.y === 23.875);
check(value.position3D.z === 190.875);
check(value.box.x === -4546412435311911869n);
check(value.box.y === -5196919309450367787n);
check(value.box.width === 7959977324521258122n);
check(value.box.height === -2184575120866431687n);
check(value.faceBox.x === 2459637278613772971n);
check(value.faceBox.y === 6135401897169922621n);
check(value.faceBox.width === 6957264871041421872n);
check(value.faceBox.height === -3885352226299638523n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_214(){const input=bytes("000000000024624000000000003c634000000000008064c00f9a8c8eb6e3bb1099aa8bf920d3bc6731a7d2b588dee52b66556f23df2c58bd010000000040f942"),codec=r.Ros1Pal_vision_msgsFaceDetectionCodec,value=codec.decode(input);
check(value.position3D.x === 145.125);
check(value.position3D.y === 153.875);
check(value.position3D.z === -164.0);
check(value.imageBoundingBox.x === 1205807698466871823n);
check(value.imageBoundingBox.y === 7475081620107274905n);
check(value.imageBoundingBox.width === 3163178992045041457n);
check(value.imageBoundingBox.height === -4803039665706805914n);
check(value.hog.length == 1);
check(value.hog[0] === 124.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_215(){const input=bytes("779fb48e683314cd30416a32070000007636653061ceb21765408f010000000000000000606f400000000000e86540000000000000124073e27903ca9b4e9de5d27e745cab49ea986b1b3421122caae687e48c5ffed3120100000000a01643"),codec=r.Ros1Pal_vision_msgsFaceDetectionsCodec,value=codec.decode(input);
check(value.header.seq === 2394201975);
check(value.header.stamp.sec === 3440653160);
check(value.header.stamp.nsec === 845824304);
check(value.header.frame_id === "v6e0aβ");
check(value.imgID === 2403362071);
check(value.faces.length == 1);
check(value.faces[0].position3D.x === 251.0);
check(value.faces[0].position3D.y === 175.25);
check(value.faces[0].position3D.z === 4.5);
check(value.faces[0].imageBoundingBox.x === -7111575469627415949n);
check(value.faces[0].imageBoundingBox.y === -1564530981954727195n);
check(value.faces[0].imageBoundingBox.width === -6184548254469100648n);
check(value.faces[0].imageBoundingBox.height === 1356707599107786726n);
check(value.faces[0].hog.length == 1);
check(value.faces[0].hog[0] === 150.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_216(){const input=bytes("8e01000000070000007639336262ceb201000000000054c2"),codec=r.Ros1Pal_vision_msgsFaceRecognitionFeedbackCodec,value=codec.decode(input);
check(value.order_id === 142);
check(value.names.length == 1);
check(value.names[0] === "v93bbβ");
check(value.confidence.length == 1);
check(value.confidence[0] === -53.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_217(){const input=bytes("bd070000007663643362ceb2"),codec=r.Ros1Pal_vision_msgsFaceRecognitionGoalCodec,value=codec.decode(input);
check(value.order_id === 189);
check(value.order_argument === "vcd3bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_218(){const input=bytes("f901000000070000007639333032ceb20100000000000641"),codec=r.Ros1Pal_vision_msgsFaceRecognitionResultCodec,value=codec.decode(input);
check(value.order_id === 249);
check(value.names.length == 1);
check(value.names[0] === "v9302β");
check(value.confidence.length == 1);
check(value.confidence[0] === 8.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_219(){const input=bytes(""),codec=r.Ros1Pal_vision_msgsFaceTrackingStartRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_220(){const input=bytes(""),codec=r.Ros1Pal_vision_msgsFaceTrackingStartResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_221(){const input=bytes(""),codec=r.Ros1Pal_vision_msgsFaceTrackingStopRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_222(){const input=bytes(""),codec=r.Ros1Pal_vision_msgsFaceTrackingStopResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_223(){const input=bytes("4fe4a5c6f48b133de7433b15070000007636363231ceb201"),codec=r.Ros1Pal_vision_msgsFollowMeResponseCodec,value=codec.decode(input);
check(value.header.seq === 3332760655);
check(value.header.stamp.sec === 1024691188);
check(value.header.stamp.nsec === 356205543);
check(value.header.frame_id === "v6621β");
check(value.targetDetected === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_224(){const input=bytes("00400143"),codec=r.Ros1Pal_vision_msgsFollowMeStartRequestCodec,value=codec.decode(input);
check(value.maxSecondsTargetLost === 129.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_225(){const input=bytes(""),codec=r.Ros1Pal_vision_msgsFollowMeStartResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_226(){const input=bytes(""),codec=r.Ros1Pal_vision_msgsFollowMeStopRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_227(){const input=bytes(""),codec=r.Ros1Pal_vision_msgsFollowMeStopResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_228(){const input=bytes("265cb62a06470bd01b868803070000007663363665ceb2070000007662346232ceb200000000006039c000000000000855c000000000007c6dc0"),codec=r.Ros1Pal_vision_msgsGestureCodec,value=codec.decode(input);
check(value.header.seq === 716594214);
check(value.header.stamp.sec === 3490400006);
check(value.header.stamp.nsec === 59278875);
check(value.header.frame_id === "vc66eβ");
check(value.gestureId === "vb4b2β");
check(value.position3D.x === -25.375);
check(value.position3D.y === -84.125);
check(value.position3D.z === -235.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_229(){const input=bytes("1a1efc378303d761f099b527070000007631316333ceb20080144300806a42"),codec=r.Ros1Pal_vision_msgsHeadPanTiltCodec,value=codec.decode(input);
check(value.header.seq === 939269658);
check(value.header.stamp.sec === 1641481091);
check(value.header.stamp.nsec === 666212848);
check(value.header.frame_id === "v11c3β");
check(value.pan === 148.5);
check(value.tilt === 58.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_230(){const input=bytes("31691232881025aca75915451e2022db19bdb13fa0c486a1869e50163fe767880000000000b0414000000000003463400000000000e86fc001000000004075c300004443002032c300e0604300c093420100000000202443000071c30000a24200808bc20100000065885855010000008b27ed47"),codec=r.Ros1Pal_vision_msgsHogDetectionCodec,value=codec.decode(input);
check(value.imageBoundingBox.x === -6042405147905988303n);
check(value.imageBoundingBox.y === -2656525515815036505n);
check(value.imageBoundingBox.width === -6807537594181501671n);
check(value.imageBoundingBox.height === -8617665103806816634n);
check(value.direction.x === 35.375);
check(value.direction.y === 153.625);
check(value.direction.z === -255.25);
check(value.hog.length == 1);
check(value.hog[0] === -245.25);
check(value.principalEigenVectorRGB.r === 196.0);
check(value.principalEigenVectorRGB.g === -178.125);
check(value.principalEigenVectorRGB.b === 224.875);
check(value.principalEigenVectorRGB.a === 73.875);
check(value.rgbClusterCenters.length == 1);
check(value.rgbClusterCenters[0].r === 164.125);
check(value.rgbClusterCenters[0].g === -241.0);
check(value.rgbClusterCenters[0].b === 81.0);
check(value.rgbClusterCenters[0].a === -69.75);
check(value.rgbDescriptor1.length == 1);
check(value.rgbDescriptor1[0] === 1431865445);
check(value.rgbDescriptor2.length == 1);
check(value.rgbDescriptor2[0] === 1206724491);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_231(){const input=bytes("f6e3af4ad91764350e260115070000007637633830ceb2a89da2c801000000e90f5c8950766b177f3f5bfec6466c60729342a2cbea753c78d5adccc1ac9a5200000000006065c00000000000846d400000000000c065c0010000000000714300e00ac30000b6c2002015c300c067430100000000c01843008085c200a03b43000075c301000000d0537910010000003efccc02"),codec=r.Ros1Pal_vision_msgsHogDetectionsCodec,value=codec.decode(input);
check(value.header.seq === 1253041142);
check(value.header.stamp.sec === 895752153);
check(value.header.stamp.nsec === 352396814);
check(value.header.frame_id === "v7c80β");
check(value.imgID === 3366100392);
check(value.persons.length == 1);
check(value.persons[0].imageBoundingBox.x === 1687572573654355945n);
check(value.persons[0].imageBoundingBox.y === 6948006145610694527n);
check(value.persons[0].imageBoundingBox.width === 4356646374872355698n);
check(value.persons[0].imageBoundingBox.height === 5952259805886141816n);
check(value.persons[0].direction.x === -171.0);
check(value.persons[0].direction.y === 236.125);
check(value.persons[0].direction.z === -174.0);
check(value.persons[0].hog.length == 1);
check(value.persons[0].hog[0] === 241.0);
check(value.persons[0].principalEigenVectorRGB.r === -138.875);
check(value.persons[0].principalEigenVectorRGB.g === -91.0);
check(value.persons[0].principalEigenVectorRGB.b === -149.125);
check(value.persons[0].principalEigenVectorRGB.a === 231.75);
check(value.persons[0].rgbClusterCenters.length == 1);
check(value.persons[0].rgbClusterCenters[0].r === 152.75);
check(value.persons[0].rgbClusterCenters[0].g === -66.75);
check(value.persons[0].rgbClusterCenters[0].b === 187.625);
check(value.persons[0].rgbClusterCenters[0].a === -245.0);
check(value.persons[0].rgbDescriptor1.length == 1);
check(value.persons[0].rgbDescriptor1[0] === 276386768);
check(value.persons[0].rgbDescriptor2.length == 1);
check(value.persons[0].rgbDescriptor2[0] === 46988350);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_232(){const input=bytes("7b99b44a2f3830a3085bae05070000007632313037ceb2010000000000000000e853c00000000000bc64c00000000000e03040"),codec=r.Ros1Pal_vision_msgsLegDetectionsCodec,value=codec.decode(input);
check(value.header.seq === 1253349755);
check(value.header.stamp.sec === 2737846319);
check(value.header.stamp.nsec === 95312648);
check(value.header.frame_id === "v2107β");
check(value.position3D.length == 1);
check(value.position3D[0].x === -79.625);
check(value.position3D[0].y === -165.875);
check(value.position3D[0].z === 16.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_233(){const input=bytes("000068c300e07843"),codec=r.Ros1Pal_vision_msgsLookToPixelRequestCodec,value=codec.decode(input);
check(value.x === -232.0);
check(value.y === 248.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_234(){const input=bytes(""),codec=r.Ros1Pal_vision_msgsLookToPixelResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_235(){const input=bytes("5dfb7189e4824617cbcb76fe72b241605ccbb5a2e5b2823cef2f7b1056564c8e"),codec=r.Ros1Pal_vision_msgsRectangleCodec,value=codec.decode(input);
check(value.x === 1677171829312191325n);
check(value.y === 6936021108092488651n);
check(value.width === 4360244088595139420n);
check(value.height === -8193078694450090001n);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_236(){const input=bytes(""),codec=r.Ros1Pal_visual_localization_msgsVisualLocAddPlaceFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_237(){const input=bytes(""),codec=r.Ros1Pal_visual_localization_msgsVisualLocAddPlaceGoalCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_238(){const input=bytes("01"),codec=r.Ros1Pal_visual_localization_msgsVisualLocAddPlaceResultCodec,value=codec.decode(input);
check(value.res === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_239(){const input=bytes(""),codec=r.Ros1Pal_visual_localization_msgsVisualLocRecognizeFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_240(){const input=bytes(""),codec=r.Ros1Pal_visual_localization_msgsVisualLocRecognizeGoalCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_241(){const input=bytes("000000000000606f4017de6a72d50926da3b28581c070000007665623865ceb20000000000986cc00000000000106fc00000000000b04f4000000000004869c000000000009066c000000000002042c000000000008c604000000000000869400000000000504540000000000000674000000000009c6c400000000000904d400000000000c04e400000000000446bc00000000000802ac0000000000098574000000000005868400000000000e06440000000000058564000000000002c6cc00000000000205fc00000000000402ec00000000000305fc000000000009c63c000000000000056c00000000000e4604000000000007062400000000000ec674000000000002863400000000000c06fc00000000000d463c0000000000070494000000000004033c00000000000fc6dc000000000001066c00000000000286dc00000000000ac66c00000000000506ec00000000000c0694000000000001051c000000000009047400000000000285fc00000000000786bc0"),codec=r.Ros1Pal_visual_localization_msgsVisualLocRecognizeResultCodec,value=codec.decode(input);
check(value.success === false);
check(value.confidence === 251.0);
check(value.estimatedPose.header.seq === 1919606295);
check(value.estimatedPose.header.stamp.sec === 3659925973);
check(value.estimatedPose.header.stamp.nsec === 475539515);
check(value.estimatedPose.header.frame_id === "veb8eβ");
check(value.estimatedPose.pose.pose.position.x === -228.75);
check(value.estimatedPose.pose.pose.position.y === -248.5);
check(value.estimatedPose.pose.pose.position.z === 63.375);
check(value.estimatedPose.pose.pose.orientation.x === -202.25);
check(value.estimatedPose.pose.pose.orientation.y === -180.5);
check(value.estimatedPose.pose.pose.orientation.z === -36.25);
check(value.estimatedPose.pose.pose.orientation.w === 132.375);
check(value.estimatedPose.pose.covariance.length == 36);
check(value.estimatedPose.pose.covariance[0] === 200.25);
check(value.estimatedPose.pose.covariance[1] === 42.625);
check(value.estimatedPose.pose.covariance[2] === 184.0);
check(value.estimatedPose.pose.covariance[3] === 228.875);
check(value.estimatedPose.pose.covariance[4] === 59.125);
check(value.estimatedPose.pose.covariance[5] === 61.5);
check(value.estimatedPose.pose.covariance[6] === -218.125);
check(value.estimatedPose.pose.covariance[7] === -13.25);
check(value.estimatedPose.pose.covariance[8] === 94.375);
check(value.estimatedPose.pose.covariance[9] === 194.75);
check(value.estimatedPose.pose.covariance[10] === 167.0);
check(value.estimatedPose.pose.covariance[11] === 89.375);
check(value.estimatedPose.pose.covariance[12] === -225.375);
check(value.estimatedPose.pose.covariance[13] === -124.5);
check(value.estimatedPose.pose.covariance[14] === -15.125);
check(value.estimatedPose.pose.covariance[15] === -124.75);
check(value.estimatedPose.pose.covariance[16] === -156.875);
check(value.estimatedPose.pose.covariance[17] === -88.0);
check(value.estimatedPose.pose.covariance[18] === 135.125);
check(value.estimatedPose.pose.covariance[19] === 147.5);
check(value.estimatedPose.pose.covariance[20] === 191.375);
check(value.estimatedPose.pose.covariance[21] === 153.25);
check(value.estimatedPose.pose.covariance[22] === -254.0);
check(value.estimatedPose.pose.covariance[23] === -158.625);
check(value.estimatedPose.pose.covariance[24] === 50.875);
check(value.estimatedPose.pose.covariance[25] === -19.25);
check(value.estimatedPose.pose.covariance[26] === -239.875);
check(value.estimatedPose.pose.covariance[27] === -176.5);
check(value.estimatedPose.pose.covariance[28] === -233.25);
check(value.estimatedPose.pose.covariance[29] === -181.375);
check(value.estimatedPose.pose.covariance[30] === -242.5);
check(value.estimatedPose.pose.covariance[31] === 206.0);
check(value.estimatedPose.pose.covariance[32] === -68.25);
check(value.estimatedPose.pose.covariance[33] === 47.125);
check(value.estimatedPose.pose.covariance[34] === -124.625);
check(value.estimatedPose.pose.covariance[35] === -219.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_242(){const input=bytes("de"),codec=r.Ros1Pal_walking_msgsContactStateCodec,value=codec.decode(input);
check(value.type === 222);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_243(){const input=bytes("ac4d371b7390ec427fe47523070000007666333337ceb2010000000000000000946c400000000000dc6d400000000000706cc000000000000065c00000000000805ec00000000000804c4000000000007041c0470012d067c11a9453c67939070000007664336335ceb2010000000000000000b85a400000000000ec644000000000009046c00000000000b044c00000000000f8504000000000005856c000000000003059c0334b30461f442fd8d840502c070000007638663530ceb2010000000000000000b46fc00000000000b0594000000000004045c00000000000f85f4000000000002467400000000000e036400000000000e064c0010000004e7cb36e8263fc03"),codec=r.Ros1Pal_walking_msgsMoveHipFeetRequestCodec,value=codec.decode(input);
check(value.hip_poses.header.seq === 456609196);
check(value.hip_poses.header.stamp.sec === 1122799731);
check(value.hip_poses.header.stamp.nsec === 594928767);
check(value.hip_poses.header.frame_id === "vf337β");
check(value.hip_poses.poses.length == 1);
check(value.hip_poses.poses[0].position.x === 228.625);
check(value.hip_poses.poses[0].position.y === 238.875);
check(value.hip_poses.poses[0].position.z === -227.5);
check(value.hip_poses.poses[0].orientation.x === -168.0);
check(value.hip_poses.poses[0].orientation.y === -122.0);
check(value.hip_poses.poses[0].orientation.z === 57.0);
check(value.hip_poses.poses[0].orientation.w === -34.875);
check(value.left_foot_poses.header.seq === 3490840647);
check(value.left_foot_poses.header.stamp.sec === 2484781415);
check(value.left_foot_poses.header.stamp.nsec === 964281939);
check(value.left_foot_poses.header.frame_id === "vd3c5β");
check(value.left_foot_poses.poses.length == 1);
check(value.left_foot_poses.poses[0].position.x === 106.875);
check(value.left_foot_poses.poses[0].position.y === 167.375);
check(value.left_foot_poses.poses[0].position.z === -45.125);
check(value.left_foot_poses.poses[0].orientation.x === -41.375);
check(value.left_foot_poses.poses[0].orientation.y === 67.875);
check(value.left_foot_poses.poses[0].orientation.z === -89.375);
check(value.left_foot_poses.poses[0].orientation.w === -100.75);
check(value.right_foot_poses.header.seq === 1177570099);
check(value.right_foot_poses.header.stamp.sec === 3626976287);
check(value.right_foot_poses.header.stamp.nsec === 743456984);
check(value.right_foot_poses.header.frame_id === "v8f50β");
check(value.right_foot_poses.poses.length == 1);
check(value.right_foot_poses.poses[0].position.x === -253.625);
check(value.right_foot_poses.poses[0].position.y === 102.75);
check(value.right_foot_poses.poses[0].position.z === -42.5);
check(value.right_foot_poses.poses[0].orientation.x === 127.875);
check(value.right_foot_poses.poses[0].orientation.y === 185.125);
check(value.right_foot_poses.poses[0].orientation.z === 22.875);
check(value.right_foot_poses.poses[0].orientation.w === -167.0);
check(value.times_after_previous.length == 1);
check(value.times_after_previous[0].sec === 1857256526);
check(value.times_after_previous[0].nsec === 66872194);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_244(){const input=bytes(""),codec=r.Ros1Pal_walking_msgsMoveHipFeetResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_245(){const input=bytes("0000000000d85b400000000000a85440000000000030424000000000004462c00000000000085bc00000000000d468c00000000000e062400000000000d063c00000000000206cc00000000000a46f4000000000001468c00000000000a460c00000000000f468c00000000000e861c0e000000000008025400000000000205f400000000000486a400000000000146b400000000000dc6740000000000068524000000000008068c02b"),codec=r.Ros1Pal_walking_msgsSMStatusCodec,value=codec.decode(input);
check(value.left_foot_pose.position.x === 111.375);
check(value.left_foot_pose.position.y === 82.625);
check(value.left_foot_pose.position.z === 36.375);
check(value.left_foot_pose.orientation.x === -146.125);
check(value.left_foot_pose.orientation.y === -108.125);
check(value.left_foot_pose.orientation.z === -198.625);
check(value.left_foot_pose.orientation.w === 151.0);
check(value.right_foot_pose.position.x === -158.5);
check(value.right_foot_pose.position.y === -225.0);
check(value.right_foot_pose.position.z === 253.125);
check(value.right_foot_pose.orientation.x === -192.625);
check(value.right_foot_pose.orientation.y === -133.125);
check(value.right_foot_pose.orientation.z === -199.625);
check(value.right_foot_pose.orientation.w === -143.25);
check(value.stance_foot_side === 224);
check(value.future_swing_landing_pose.position.x === 10.75);
check(value.future_swing_landing_pose.position.y === 124.5);
check(value.future_swing_landing_pose.position.z === 210.25);
check(value.future_swing_landing_pose.orientation.x === 216.625);
check(value.future_swing_landing_pose.orientation.y === 190.875);
check(value.future_swing_landing_pose.orientation.z === 73.625);
check(value.future_swing_landing_pose.orientation.w === -196.0);
check(value.future_swing_side === 43);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_246(){const input=bytes("0000000000003c60c0"),codec=r.Ros1Pal_walking_msgsSitRequestCodec,value=codec.decode(input);
check(value.down === false);
check(value.weight_threshold === -129.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_247(){const input=bytes(""),codec=r.Ros1Pal_walking_msgsSitResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_248(){const input=bytes("00000000004464c00000000000b050c00000000000ac674000000000006467c0"),codec=r.Ros1Pal_walking_msgsStepOverRequestCodec,value=codec.decode(input);
check(value.step_length === -162.125);
check(value.step_time === -66.75);
check(value.step_height === 189.375);
check(value.z_lipm === -187.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_249(){const input=bytes("070000007634353235ceb2"),codec=r.Ros1Pal_walking_msgsStepOverResponseCodec,value=codec.decode(input);
check(value.result === "v4525β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_250(){const input=bytes("ed8a7e9a0000000000b05ec00000000000a06ec0"),codec=r.Ros1Pal_walking_msgsWalkStepsRequestCodec,value=codec.decode(input);
check(value.nsteps === -1702982931);
check(value.step_length === -122.75);
check(value.step_time === -245.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_251(){const input=bytes(""),codec=r.Ros1Pal_walking_msgsWalkStepsResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_252(){const input=bytes("758c8fe4674ef6c5dc2a2839070000007665313461ceb20000000000f867c00000000000d4644000000000008868c00000000000006840000000000018644000000000007468c00000000000bc66c000000000008045400000000000e8604000000000006463c000000000004048c0000000000080404000000000000059c0000000000020574000000000002866400000000000d060400000000000c03f400000000000a859400000000000c86bc000000000004062c000000000000462400000000000f46cc00000000000f46fc00000000000805040"),codec=r.Ros1Pal_walking_msgsWalkingStatusCodec,value=codec.decode(input);
check(value.header.seq === 3834612853);
check(value.header.stamp.sec === 3321253479);
check(value.header.stamp.nsec === 958933724);
check(value.header.frame_id === "ve14aβ");
check(value.hip_reference_pose.position.x === -191.75);
check(value.hip_reference_pose.position.y === 166.625);
check(value.hip_reference_pose.position.z === -196.25);
check(value.hip_reference_pose.orientation.x === 192.0);
check(value.hip_reference_pose.orientation.y === 160.75);
check(value.hip_reference_pose.orientation.z === -195.625);
check(value.hip_reference_pose.orientation.w === -181.875);
check(value.foot_reference_pose.length == 2);
check(value.foot_reference_pose[0].position.x === 43.0);
check(value.foot_reference_pose[0].position.y === 135.25);
check(value.foot_reference_pose[0].position.z === -155.125);
check(value.foot_reference_pose[0].orientation.x === -48.5);
check(value.foot_reference_pose[0].orientation.y === 33.0);
check(value.foot_reference_pose[0].orientation.z === -100.0);
check(value.foot_reference_pose[0].orientation.w === 92.5);
check(value.foot_reference_pose[1].position.x === 177.25);
check(value.foot_reference_pose[1].position.y === 134.5);
check(value.foot_reference_pose[1].position.z === 31.75);
check(value.foot_reference_pose[1].orientation.x === 102.625);
check(value.foot_reference_pose[1].orientation.y === -222.25);
check(value.foot_reference_pose[1].orientation.z === -146.0);
check(value.foot_reference_pose[1].orientation.w === 144.125);
check(value.zmp_reference.x === -231.625);
check(value.zmp_reference.y === -255.625);
check(value.zmp_reference.z === 66.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_253(){const input=bytes("8f2dff7a58f70aa05867f617070000007661643261ceb20000000000986cc000000000007066c00000000000106840fef3ecae7323c72b1cb1c65982"),codec=r.Ros1Pal_walking_msgsWalkingStepCodec,value=codec.decode(input);
check(value.header.seq === 2063543695);
check(value.header.stamp.sec === 2685073240);
check(value.header.stamp.nsec === 402024280);
check(value.header.frame_id === "vad2aβ");
check(value.step_target.pose.x === -228.75);
check(value.step_target.pose.y === -179.5);
check(value.step_target.pose.theta === 192.5);
check(value.step_target.leg === 254);
check(value.step_duration.data.sec === 1940843763);
check(value.step_duration.data.nsec === 472631075);
check(value.step_list_size === -2108045647);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_254(){const input=bytes(""),codec=r.Ros1Pal_web_msgsGetUserInputFeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_255(){const input=bytes("ec070000007666366632ceb2070000007663626264ceb2070000007665343335ceb2070000007665646566ceb201000000070000007662626136ceb2070000007635343730ceb2070000007661656566ceb2070000007638616230ceb2070000007661346564ceb2070000007638663265ceb20000000000946bc001000000070000007634333265ceb2070000007661666133ceb27936e45bc7a7e81001070000007666323234ceb2070000007639376465ceb2070000007664343736ceb201000000070000007665633833ceb2070000007631383130ceb2070000007630336466ceb2070000007662333239ceb2070000007664353733ceb2070000007631353764ceb20000000000d8594000"),codec=r.Ros1Pal_web_msgsGetUserInputGoalCodec,value=codec.decode(input);
check(value.web_go_to.type === 236);
check(value.web_go_to.value === "vf6f2β");
check(value.question_tts.text.section === "vcbbdβ");
check(value.question_tts.text.key === "ve435β");
check(value.question_tts.text.lang_id === "vedefβ");
check(value.question_tts.text.arguments.length == 1);
check(value.question_tts.text.arguments[0].section === "vbba6β");
check(value.question_tts.text.arguments[0].key === "v5470β");
check(value.question_tts.text.arguments[0].expanded === "vaeefβ");
check(value.question_tts.rawtext.text === "v8ab0β");
check(value.question_tts.rawtext.lang_id === "va4edβ");
check(value.question_tts.speakerName === "v8f2eβ");
check(value.question_tts.wait_before_speaking === -220.625);
check(value.valid_asr_inputs.length == 1);
check(value.valid_asr_inputs[0] === "v432eβ");
check(value.asr_language === "vafa3β");
check(value.asr_timeout.sec === 1541682809);
check(value.asr_timeout.nsec === 283682759);
check(value.retry_asr === true);
check(value.retry_tts.text.section === "vf224β");
check(value.retry_tts.text.key === "v97deβ");
check(value.retry_tts.text.lang_id === "vd476β");
check(value.retry_tts.text.arguments.length == 1);
check(value.retry_tts.text.arguments[0].section === "vec83β");
check(value.retry_tts.text.arguments[0].key === "v1810β");
check(value.retry_tts.text.arguments[0].expanded === "v03dfβ");
check(value.retry_tts.rawtext.text === "vb329β");
check(value.retry_tts.rawtext.lang_id === "vd573β");
check(value.retry_tts.speakerName === "v157dβ");
check(value.retry_tts.wait_before_speaking === 103.375);
check(value.wait_for_interaction === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_256(){const input=bytes("070000007632343663ceb201000000070000007635333962ceb2070000007632373135ceb2"),codec=r.Ros1Pal_web_msgsGetUserInputResultCodec,value=codec.decode(input);
check(value.input.action === "v246cβ");
check(value.input.args.length == 1);
check(value.input.args[0].key === "v539bβ");
check(value.input.args[0].value === "v2715β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_257(){const input=bytes("ce070000007632336538ceb2"),codec=r.Ros1Pal_web_msgsWebGoToCodec,value=codec.decode(input);
check(value.type === 206);
check(value.value === "v23e8β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_258(){const input=bytes(""),codec=r.Ros1Pal_wifi_localization_msgsGetWifiMapRequestCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_259(){const input=bytes("3ab5016c19a769750e55f016070000007661653362ceb2f648c3e6085e330b000049c24436338cac742d2f000000000000e03f0000000000604dc000000000008052c00000000000f055c00000000000e46d40000000000000e8bf00000000001463403984c1aa0100000001000000070000007636393462ceb200c0fec20000e7c201719c568ef034265449fc30cdadd70e"),codec=r.Ros1Pal_wifi_localization_msgsGetWifiMapResponseCodec,value=codec.decode(input);
check(value.map.header.seq === 1812051258);
check(value.map.header.stamp.sec === 1969858329);
check(value.map.header.stamp.nsec === 384849166);
check(value.map.header.frame_id === "vae3bβ");
check(value.map.info.map_load_time.sec === 3871557878);
check(value.map.info.map_load_time.nsec === 187915784);
check(value.map.info.resolution === -50.25);
check(value.map.info.width === 2352166468);
check(value.map.info.height === 791508140);
check(value.map.info.origin.position.x === 0.5);
check(value.map.info.origin.position.y === -58.75);
check(value.map.info.origin.position.z === -74.0);
check(value.map.info.origin.orientation.x === -87.75);
check(value.map.info.origin.orientation.y === 239.125);
check(value.map.info.origin.orientation.z === -0.75);
check(value.map.info.origin.orientation.w === 152.625);
check(value.map.sectors === 2864809017);
check(value.map.data.length == 1);
check(value.map.data[0].networks.length == 1);
check(value.map.data[0].networks[0].id.data === "v694bβ");
check(value.map.data[0].networks[0].mean === -127.375);
check(value.map.data[0].networks[0].std_dev === -115.5);
check(value.map.data[0].start_time.sec === 1453093121);
check(value.map.data[0].start_time.nsec === 641003662);
check(value.map.data[0].end_time.sec === 821840212);
check(value.map.data[0].end_time.nsec === 249015757);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_260(){const input=bytes("070000007664636330ceb2008039c3002006c3"),codec=r.Ros1Pal_wifi_localization_msgsWifiSignalCodec,value=codec.decode(input);
check(value.id.data === "vdcc0β");
check(value.mean === -185.5);
check(value.std_dev === -134.125);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_261(){const input=bytes("01000000070000007661303930ceb2006064c3008024c25871c7f4dea2491870bb3342ee0cac07"),codec=r.Ros1Pal_wifi_localization_msgsWifiSignalListCodec,value=codec.decode(input);
check(value.networks.length == 1);
check(value.networks[0].id.data === "va090β");
check(value.networks[0].mean === -228.375);
check(value.networks[0].std_dev === -41.125);
check(value.start_time.sec === 4106711384);
check(value.start_time.nsec === 407479006);
check(value.end_time.sec === 1110686576);
check(value.end_time.nsec === 128716014);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_262(){const input=bytes("cdf4c44a27db3e65d9d9b913070000007632333364ceb28eb76daef98dc52d00a01c43b377f6aa52d6d20c0000000000a061400000000000a069400000000000002c400000000000606d4000000000008c69c00000000000406240000000000000164029b183270100000001000000070000007639356161ceb20060134300803d42798e321e7f752c11058cae956ade8e23"),codec=r.Ros1Pal_wifi_localization_msgsWifiSignalMapCodec,value=codec.decode(input);
check(value.header.seq === 1254421709);
check(value.header.stamp.sec === 1698618151);
check(value.header.stamp.nsec === 330947033);
check(value.header.frame_id === "v233dβ");
check(value.info.map_load_time.sec === 2926425998);
check(value.info.map_load_time.nsec === 767921657);
check(value.info.resolution === 156.625);
check(value.info.width === 2868279219);
check(value.info.height === 215144018);
check(value.info.origin.position.x === 141.0);
check(value.info.origin.position.y === 205.0);
check(value.info.origin.position.z === 14.0);
check(value.info.origin.orientation.x === 235.0);
check(value.info.origin.orientation.y === -204.375);
check(value.info.origin.orientation.z === 146.0);
check(value.info.origin.orientation.w === 5.5);
check(value.sectors === 662941993);
check(value.data.length == 1);
check(value.data[0].networks.length == 1);
check(value.data[0].networks[0].id.data === "v95aaβ");
check(value.data[0].networks[0].mean === 147.375);
check(value.data[0].networks[0].std_dev === 47.375);
check(value.data[0].start_time.sec === 506629753);
check(value.data[0].start_time.nsec === 288126335);
check(value.data[0].end_time.sec === 2511244293);
check(value.data[0].end_time.nsec === 596565610);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_263(){const input=bytes("070000007666663038ceb2010000000000fbc1"),codec=r.Ros1Sensor_msgsChannelFloat32Codec,value=codec.decode(input);
check(value.name === "vff08β");
check(value.values.length == 1);
check(value.values[0] === -31.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_264(){const input=bytes("f07a6dc9f1321c00d1f68120070000007662313064ceb2070000007633613638ceb2010000009a"),codec=r.Ros1Sensor_msgsCompressedImageCodec,value=codec.decode(input);
check(value.header.seq === 3379395312);
check(value.header.stamp.sec === 1848049);
check(value.header.stamp.nsec === 545388241);
check(value.header.frame_id === "vb10dβ");
check(value.format === "v3a68β");
check(value.data.length == 1);
check(value.data[0] === 154);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_265(){const input=bytes("0d3bc4cec509aff95fb1cc01070000007662626539ceb293698f68aa76e19c070000007635316232ceb21aa621f0620100000016"),codec=r.Ros1Sensor_msgsImageCodec,value=codec.decode(input);
check(value.header.seq === 3468966669);
check(value.header.stamp.sec === 4188998085);
check(value.header.stamp.nsec === 30191967);
check(value.header.frame_id === "vbbe9β");
check(value.height === 1754229139);
check(value.width === 2632021674);
check(value.encoding === "v51b2β");
check(value.is_bigendian === 26);
check(value.step === 1659904422);
check(value.data.length == 1);
check(value.data[0] === 22);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_266(){const input=bytes("69712da77cfaf894848f4c00070000007633316665ceb201000000008071420000994100c059c301000000070000007665376333ceb20100000000606143"),codec=r.Ros1Sensor_msgsPointCloudCodec,value=codec.decode(input);
check(value.header.seq === 2804773225);
check(value.header.stamp.sec === 2499345020);
check(value.header.stamp.nsec === 5017476);
check(value.header.frame_id === "v31feβ");
check(value.points.length == 1);
check(value.points[0].x === 60.375);
check(value.points[0].y === 19.125);
check(value.points[0].z === -217.75);
check(value.channels.length == 1);
check(value.channels[0].name === "ve7c3β");
check(value.channels[0].values.length == 1);
check(value.channels[0].values[0] === 225.375);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_267(){const input=bytes("f8804e436680e48f01fb39c2c4bb3f2000"),codec=r.Ros1Sensor_msgsRegionOfInterestCodec,value=codec.decode(input);
check(value.x_offset === 1129218296);
check(value.y_offset === 2414116966);
check(value.height === 3258579713);
check(value.width === 541047748);
check(value.do_rectify === false);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_268(){const input=bytes("01"),codec=r.Ros1Std_msgsBoolCodec,value=codec.decode(input);
check(value.data === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_269(){const input=bytes("002068c3008008c200809cc2000028c3"),codec=r.Ros1Std_msgsColorRGBACodec,value=codec.decode(input);
check(value.r === -232.125);
check(value.g === -34.125);
check(value.b === -78.25);
check(value.a === -168.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_270(){const input=bytes("4796f792cc0b1102"),codec=r.Ros1Std_msgsDurationCodec,value=codec.decode(input);
check(value.data.sec === -1829267897);
check(value.data.nsec === 34671564);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_271(){const input=bytes("00c03b43"),codec=r.Ros1Std_msgsFloat32Codec,value=codec.decode(input);
check(value.data === 187.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_272(){const input=bytes("a949162e20488fdde929b81e070000007632663062ceb2"),codec=r.Ros1Std_msgsHeaderCodec,value=codec.decode(input);
check(value.seq === 773212585);
check(value.stamp.sec === 3717154848);
check(value.stamp.nsec === 515385833);
check(value.frame_id === "v2f0bβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_273(){const input=bytes("070000007631643932ceb2"),codec=r.Ros1Std_msgsStringCodec,value=codec.decode(input);
check(value.data === "v1d92β");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_274(){const input=bytes("c8280b76c3d5bc08"),codec=r.Ros1TimeCodec,value=codec.decode(input);
check(value.sec === 1980442824);
check(value.nsec === 146593219);
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
test_222();
test_223();
test_224();
test_225();
test_226();
test_227();
test_228();
test_229();
test_230();
test_231();
test_232();
test_233();
test_234();
test_235();
test_236();
test_237();
test_238();
test_239();
test_240();
test_241();
test_242();
test_243();
test_244();
test_245();
test_246();
test_247();
test_248();
test_249();
test_250();
test_251();
test_252();
test_253();
test_254();
test_255();
test_256();
test_257();
test_258();
test_259();
test_260();
test_261();
test_262();
test_263();
test_264();
test_265();
test_266();
test_267();
test_268();
test_269();
test_270();
test_271();
test_272();
test_273();
test_274();
return 0;
}
