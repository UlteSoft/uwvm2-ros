/* Authored original-field checks; no ROS calls or physical command validity. */
import * as r from "sdk/origin/custom-types/trossen-ros1-manipulators-melodic-complete/bounded_records.mjs";
function check(v){if(!v)throw new Error("original field or wire mismatch");}
function reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}
function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}
function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}
function test_0(){const input=bytes("10a07080b93e496ff78927b6"),codec=r.Ros1Interbotix_uxarm_joyArmJoyCodec,value=codec.decode(input);
check(value.ee_x_cmd === 16);
check(value.ee_y_cmd === -96);
check(value.ee_z_cmd === 112);
check(value.ee_yaw_cmd === -128);
check(value.ee_pitch_cmd === -71);
check(value.waist_cmd === 62);
check(value.gripper_cmd === 73);
check(value.pose_cmd === 111);
check(value.speed_cmd === -9);
check(value.speed_toggle_cmd === -119);
check(value.gripper_speed_cmd === 39);
check(value.mode_cmd === -74);
check(equal(codec.encode(value),input));
if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));
const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}

export function main(){
test_0();
return 0;
}
