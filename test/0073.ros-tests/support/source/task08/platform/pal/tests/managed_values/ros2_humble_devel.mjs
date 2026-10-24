/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/pal-ros2-humble-devel-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("8b8195af42ba7f2d"),codec=r.Ros2Builtin_interfacesMsgDurationCodec,value=codec.decode(input);
check(value.sec === -1349156469);
check(value.nanosec === 763345474);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_1(){const input=bytes("072b89b550057009"),codec=r.Ros2Builtin_interfacesMsgTimeCodec,value=codec.decode(input);
check(value.sec === -1249301753);
check(value.nanosec === 158336336);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_2(){const input=bytes(""),codec=r.Ros2Pal_device_msgsActionDoTimedLedEffect_FeedbackCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_3(){const input=bytes("01000000af2de815f9008057c300a0314300a07ac300003d421fce0279706d2b350040d742008064c200408ac200c0fdc200c0e642000022c100c06ec30080f8c25e7b66172ff857070100409d4200207fc3000048420000264300c045c30040ed420000264300000142910ae54548f725193eefa9cd4b07a01e0040604300c0dd4200a03c4300207e4300008cc2000085c20000e0c200803fc300c01c4300403d43000072c100401d4300003ec30000414200405f4300000ec10080704300e023c300000fc300c061c3de070000007639626435ceb2010000000080844200c0ce4200003441000007424eff4e84b8dc870eb1"),codec=r.Ros2Pal_device_msgsActionDoTimedLedEffect_GoalCodec,value=codec.decode(input);
check(value.devices.length == 1);
check(value.devices[0] === 367537583);
check(value.params.effect_type === 249);
check(value.params.fixed_color.color.r === -215.5);
check(value.params.fixed_color.color.g === 177.625);
check(value.params.fixed_color.color.b === -250.625);
check(value.params.fixed_color.color.a === 47.25);
check(value.params.rainbow.transition_duration.sec === 2030226975);
check(value.params.rainbow.transition_duration.nanosec === 892038512);
check(value.params.fade.first_color.r === 107.625);
check(value.params.fade.first_color.g === -57.125);
check(value.params.fade.first_color.b === -69.125);
check(value.params.fade.first_color.a === -126.875);
check(value.params.fade.second_color.r === 115.375);
check(value.params.fade.second_color.g === -10.125);
check(value.params.fade.second_color.b === -238.75);
check(value.params.fade.second_color.a === -124.25);
check(value.params.fade.transition_duration.sec === 392592222);
check(value.params.fade.transition_duration.nanosec === 123205679);
check(value.params.fade.reverse_fade === true);
check(value.params.blink.first_color.r === 78.625);
check(value.params.blink.first_color.g === -255.125);
check(value.params.blink.first_color.b === 50.0);
check(value.params.blink.first_color.a === 166.0);
check(value.params.blink.second_color.r === -197.75);
check(value.params.blink.second_color.g === 118.625);
check(value.params.blink.second_color.b === 166.0);
check(value.params.blink.second_color.a === 32.25);
check(value.params.blink.first_color_duration.sec === 1172638353);
check(value.params.blink.first_color_duration.nanosec === 421918536);
check(value.params.blink.second_color_duration.sec === -844501186);
check(value.params.blink.second_color_duration.nanosec === 513804107);
check(value.params.progress.first_color.r === 224.25);
check(value.params.progress.first_color.g === 110.875);
check(value.params.progress.first_color.b === 188.625);
check(value.params.progress.first_color.a === 254.125);
check(value.params.progress.second_color.r === -70.0);
check(value.params.progress.second_color.g === -66.5);
check(value.params.progress.second_color.b === -112.0);
check(value.params.progress.second_color.a === -191.5);
check(value.params.progress.percentage === 156.75);
check(value.params.progress.led_offset === 189.25);
check(value.params.flow.first_color.r === -15.125);
check(value.params.flow.first_color.g === 157.25);
check(value.params.flow.first_color.b === -190.0);
check(value.params.flow.first_color.a === 48.25);
check(value.params.flow.second_color.r === 223.25);
check(value.params.flow.second_color.g === -8.875);
check(value.params.flow.second_color.b === 240.5);
check(value.params.flow.second_color.a === -163.875);
check(value.params.flow.percentage === -143.0);
check(value.params.flow.velocity === -225.75);
check(value.params.preprogrammed.preprogrammed_id === 222);
check(value.params.effect_via_topic.topic_name === "v9bd5β");
check(value.params.data_array.data.length == 1);
check(value.params.data_array.data[0].r === 66.25);
check(value.params.data_array.data[0].g === 103.375);
check(value.params.data_array.data[0].b === 11.25);
check(value.params.data_array.data[0].a === 33.75);
check(value.effect_duration.sec === -2075197618);
check(value.effect_duration.nanosec === 243784888);
check(value.priority === 177);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_4(){const input=bytes(""),codec=r.Ros2Pal_device_msgsActionDoTimedLedEffect_ResultCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_5(){const input=bytes("0d00001543"),codec=r.Ros2Pal_device_msgsMsgBatteryStateCodec,value=codec.decode(input);
check(value.charge_state === 13);
check(value.battery_percentage === 149.0);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_6(){const input=bytes("0ea7b8eae9cbd33a070000007663306466ceb201"),codec=r.Ros2Pal_device_msgsMsgBumperCodec,value=codec.decode(input);
check(value.header.stamp.sec === -356997362);
check(value.header.stamp.nanosec === 986958825);
check(value.header.frame_id === "vc0dfβ");
check(value.is_pressed === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_7(){const input=bytes("00c0b24200806c4300206243008018c20080f24200c0d7c20080214300007cc1bb8255b4184e58276096e316d6f44410"),codec=r.Ros2Pal_device_msgsMsgLedBlinkParamsCodec,value=codec.decode(input);
check(value.first_color.r === 89.375);
check(value.first_color.g === 236.5);
check(value.first_color.b === 226.125);
check(value.first_color.a === -38.125);
check(value.second_color.r === 121.25);
check(value.second_color.g === -107.875);
check(value.second_color.b === 161.5);
check(value.second_color.a === -15.75);
check(value.first_color_duration.sec === -1269464389);
check(value.first_color_duration.nanosec === 660098584);
check(value.second_color_duration.sec === 384013920);
check(value.second_color_duration.nanosec === 272954582);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_8(){const input=bytes("010000000080b0c2004035c300005ac2004078c3"),codec=r.Ros2Pal_device_msgsMsgLedDataArrayParamsCodec,value=codec.decode(input);
check(value.data.length == 1);
check(value.data[0].r === -88.25);
check(value.data[0].g === -181.25);
check(value.data[0].b === -54.5);
check(value.data[0].a === -248.25);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_9(){const input=bytes("cf0000e14100000ac100e0354300008ac22820961a3e2f3d3a0000f841008070c3000041430000644200a01fc3000034c20000fdc200a075432e8b9172ce5f83130100e055c30040d7420080df420000334200001f430040e04200e06bc30000ce428ce467d7015dab0f09013dae05458e23008089c200c092c200e03ec300c005430000e84200a012430000e642000080420080b04200005ac200c058c300c014c300804ac200c0ca420000abc10000e1c1004035c30020404300805a4200402843c3070000007664636663ceb20100000000e0634300005ec100e05fc300005ec1"),codec=r.Ros2Pal_device_msgsMsgLedEffectParamsCodec,value=codec.decode(input);
check(value.effect_type === 207);
check(value.fixed_color.color.r === 28.125);
check(value.fixed_color.color.g === -8.625);
check(value.fixed_color.color.b === 181.875);
check(value.fixed_color.color.a === -69.0);
check(value.rainbow.transition_duration.sec === 446046248);
check(value.rainbow.transition_duration.nanosec === 977088318);
check(value.fade.first_color.r === 31.0);
check(value.fade.first_color.g === -240.5);
check(value.fade.first_color.b === 193.0);
check(value.fade.first_color.a === 57.0);
check(value.fade.second_color.r === -159.625);
check(value.fade.second_color.g === -45.0);
check(value.fade.second_color.b === -126.5);
check(value.fade.second_color.a === 245.625);
check(value.fade.transition_duration.sec === 1922140974);
check(value.fade.transition_duration.nanosec === 327376846);
check(value.fade.reverse_fade === true);
check(value.blink.first_color.r === -213.875);
check(value.blink.first_color.g === 107.625);
check(value.blink.first_color.b === 111.75);
check(value.blink.first_color.a === 44.75);
check(value.blink.second_color.r === 159.0);
check(value.blink.second_color.g === 112.125);
check(value.blink.second_color.b === -235.875);
check(value.blink.second_color.a === 103.0);
check(value.blink.first_color_duration.sec === -681057140);
check(value.blink.first_color_duration.nanosec === 262888705);
check(value.blink.second_color_duration.sec === -1371733751);
check(value.blink.second_color_duration.nanosec === 596526341);
check(value.progress.first_color.r === -68.75);
check(value.progress.first_color.g === -73.375);
check(value.progress.first_color.b === -190.875);
check(value.progress.first_color.a === 133.75);
check(value.progress.second_color.r === 116.0);
check(value.progress.second_color.g === 146.625);
check(value.progress.second_color.b === 115.0);
check(value.progress.second_color.a === 64.0);
check(value.progress.percentage === 88.25);
check(value.progress.led_offset === -54.5);
check(value.flow.first_color.r === -216.75);
check(value.flow.first_color.g === -148.75);
check(value.flow.first_color.b === -50.625);
check(value.flow.first_color.a === 101.375);
check(value.flow.second_color.r === -21.375);
check(value.flow.second_color.g === -28.125);
check(value.flow.second_color.b === -181.25);
check(value.flow.second_color.a === 192.125);
check(value.flow.percentage === 54.625);
check(value.flow.velocity === 168.25);
check(value.preprogrammed.preprogrammed_id === 195);
check(value.effect_via_topic.topic_name === "vdcfcβ");
check(value.data_array.data.length == 1);
check(value.data_array.data[0].r === 227.875);
check(value.data_array.data[0].g === -13.875);
check(value.data_array.data[0].b === -223.875);
check(value.data_array.data[0].a === -13.875);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_10(){const input=bytes("070000007664356264ceb2"),codec=r.Ros2Pal_device_msgsMsgLedEffectViaTopicParamsCodec,value=codec.decode(input);
check(value.topic_name === "vd5bdβ");
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_11(){const input=bytes("004047c300c041c30000acc100404a430080bcc20040e8c200c006c300e01a4390890716d40e241a01"),codec=r.Ros2Pal_device_msgsMsgLedFadeParamsCodec,value=codec.decode(input);
check(value.first_color.r === -199.25);
check(value.first_color.g === -193.75);
check(value.first_color.b === -21.5);
check(value.first_color.a === 202.25);
check(value.second_color.r === -94.25);
check(value.second_color.g === -116.125);
check(value.second_color.b === -134.75);
check(value.second_color.a === 154.875);
check(value.transition_duration.sec === 369592720);
check(value.transition_duration.nanosec === 438570708);
check(value.reverse_fade === true);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_12(){const input=bytes("0000624300404b43008084420000d9c2"),codec=r.Ros2Pal_device_msgsMsgLedFixedColorParamsCodec,value=codec.decode(input);
check(value.color.r === 226.0);
check(value.color.g === 203.25);
check(value.color.b === 66.25);
check(value.color.a === -108.5);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_13(){const input=bytes("00008fc100002043008065c30000b0c200600fc300607e4300603c43008001c200002cc30040f3c2"),codec=r.Ros2Pal_device_msgsMsgLedFlowParamsCodec,value=codec.decode(input);
check(value.first_color.r === -17.875);
check(value.first_color.g === 160.0);
check(value.first_color.b === -229.5);
check(value.first_color.a === -88.0);
check(value.second_color.r === -143.375);
check(value.second_color.g === 254.375);
check(value.second_color.b === 188.375);
check(value.second_color.a === -32.375);
check(value.percentage === -172.0);
check(value.velocity === -121.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_14(){const input=bytes("2b00cefa"),codec=r.Ros2Pal_device_msgsMsgLedGroupCodec,value=codec.decode(input);
check(value.led_mask === 4207804459);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_15(){const input=bytes("42"),codec=r.Ros2Pal_device_msgsMsgLedPreProgrammedParamsCodec,value=codec.decode(input);
check(value.preprogrammed_id === 66);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_16(){const input=bytes("002007430000a0c20000e541006052c300406043006075430080f8c200c06dc300602dc300a04c43"),codec=r.Ros2Pal_device_msgsMsgLedProgressParamsCodec,value=codec.decode(input);
check(value.first_color.r === 135.125);
check(value.first_color.g === -80.0);
check(value.first_color.b === 28.625);
check(value.first_color.a === -210.375);
check(value.second_color.r === 224.25);
check(value.second_color.g === 245.375);
check(value.second_color.b === -124.25);
check(value.second_color.a === -237.75);
check(value.percentage === -173.375);
check(value.led_offset === 204.625);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_17(){const input=bytes("60c352a8ff27260a"),codec=r.Ros2Pal_device_msgsMsgLedRainbowParamsCodec,value=codec.decode(input);
check(value.transition_duration.sec === -1470971040);
check(value.transition_duration.nanosec === 170272767);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_18(){const input=bytes("43ae8e6c"),codec=r.Ros2Pal_device_msgsSrvCancelEffect_RequestCodec,value=codec.decode(input);
check(value.effect_id === 1821290051);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_19(){const input=bytes(""),codec=r.Ros2Pal_device_msgsSrvCancelEffect_ResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_20(){const input=bytes("ff69128671d0081d"),codec=r.Ros2Pal_device_msgsSrvShutdownAndWakeUpRobot_RequestCodec,value=codec.decode(input);
check(value.shutdown_duration.sec === -2045613569);
check(value.shutdown_duration.nanosec === 487116913);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_21(){const input=bytes(""),codec=r.Ros2Pal_device_msgsSrvShutdownAndWakeUpRobot_ResponseCodec,value=codec.decode(input);

check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_22(){const input=bytes("5a4b1f69008036c300807bc200c073c300a02743000026410000334200800f4200a05fc38b7d7260fcf60f143381ee30df28281e8db3f85dbabee5347f"),codec=r.Ros2Pal_device_msgsSrvTimedBlinkEffect_RequestCodec,value=codec.decode(input);
check(value.leds.led_mask === 1763658586);
check(value.first_color.r === -182.5);
check(value.first_color.g === -62.875);
check(value.first_color.b === -243.75);
check(value.first_color.a === 167.625);
check(value.second_color.r === 10.375);
check(value.second_color.g === 44.75);
check(value.second_color.b === 35.875);
check(value.second_color.a === -223.625);
check(value.first_color_duration.sec === 1618115979);
check(value.first_color_duration.nanosec === 336590588);
check(value.second_color_duration.sec === 820937011);
check(value.second_color_duration.nanosec === 505948383);
check(value.effect_duration.sec === 1576579981);
check(value.effect_duration.nanosec === 887471802);
check(value.priority === 127);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_23(){const input=bytes("e687ae1f"),codec=r.Ros2Pal_device_msgsSrvTimedBlinkEffect_ResponseCodec,value=codec.decode(input);
check(value.effect_id === 531531750);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_24(){const input=bytes("72f54d350040e542002064c3000077c3008066425e92521de9e90b0c9f"),codec=r.Ros2Pal_device_msgsSrvTimedColourEffect_RequestCodec,value=codec.decode(input);
check(value.leds.led_mask === 894301554);
check(value.color.r === 114.625);
check(value.color.g === -228.125);
check(value.color.b === -247.0);
check(value.color.a === 57.625);
check(value.effect_duration.sec === 491950686);
check(value.effect_duration.nanosec === 202107369);
check(value.priority === 159);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_25(){const input=bytes("8d1cb5cb"),codec=r.Ros2Pal_device_msgsSrvTimedColourEffect_ResponseCodec,value=codec.decode(input);
check(value.effect_id === 3417644173);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_26(){const input=bytes("22f11a0300008842008038c3000088420000dcc100800dc2000030c100807ac200e073c3a94ac4332cb9d62d01509260921a987e051e"),codec=r.Ros2Pal_device_msgsSrvTimedFadeEffect_RequestCodec,value=codec.decode(input);
check(value.leds.led_mask === 52097314);
check(value.first_color.r === 68.0);
check(value.first_color.g === -184.5);
check(value.first_color.b === 68.0);
check(value.first_color.a === -27.5);
check(value.second_color.r === -35.375);
check(value.second_color.g === -11.0);
check(value.second_color.b === -62.625);
check(value.second_color.a === -243.875);
check(value.color_change_duration.sec === 868502185);
check(value.color_change_duration.nanosec === 769046828);
check(value.reverse_fade === true);
check(value.effect_duration.sec === -1839164848);
check(value.effect_duration.nanosec === 92182554);
check(value.priority === 30);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_27(){const input=bytes("827b2b85"),codec=r.Ros2Pal_device_msgsSrvTimedFadeEffect_ResponseCodec,value=codec.decode(input);
check(value.effect_id === 2234219394);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_28(){const input=bytes("00e06dc300c06ac30000a8c200809942"),codec=r.Ros2Std_msgsMsgColorRGBACodec,value=codec.decode(input);
check(value.r === -237.875);
check(value.g === -234.75);
check(value.b === -84.0);
check(value.a === 76.75);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

function test_29(){const input=bytes("681252e4acc1ba37070000007666316532ceb2"),codec=r.Ros2Std_msgsMsgHeaderCodec,value=codec.decode(input);
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
return 0;
}
