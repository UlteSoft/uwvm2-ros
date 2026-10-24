/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "../../../../../../../../../sdk/origin/custom-types/trossen-ros1-manipulators-noetic-complete/bounded_records";
import {abortGuest} from "../../../../../../../../../sdk/origin/assemblyscript/api";
export {abortGuest};
function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}
function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}
function test_0():void{const input=bytes("10a07080b93e496ff78927b6");const codec=new r.Ros1Interbotix_uxarm_joyArmJoyCodec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){
assert(value.ee_x_cmd == <i8>(16));
assert(value.ee_y_cmd == <i8>(-96));
assert(value.ee_z_cmd == <i8>(112));
assert(value.ee_yaw_cmd == <i8>(-128));
assert(value.ee_pitch_cmd == <i8>(-71));
assert(value.waist_cmd == <i8>(62));
assert(value.gripper_cmd == <i8>(73));
assert(value.pose_cmd == <i8>(111));
assert(value.speed_cmd == <i8>(-9));
assert(value.speed_toggle_cmd == <i8>(-119));
assert(value.gripper_speed_cmd == <i8>(39));
assert(value.mode_cmd == <i8>(-74));
const encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}
if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}

export function _start():void{
test_0();
}
