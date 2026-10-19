/** Same-module test stdio only. Importing this helper opens no ROS capability.
 * The actual AS module initializer owns these arrays, so no guessed scratch
 * address can alias the SDK heap. Trusted parent controls at most16 setup
 * bytes and one-byte commands; output is bounded to4096 ASCII bytes per line.
 * The only introduced host ABI is basic WASIp1 fd_read/fd_write/proc_exit.
 */
import {close_case} from '../../../sdk/robots/unitree/examples/legacy/lowlevel';
@external('wasi_snapshot_preview1','fd_read')
declare function readFd(fd:i32,iov:usize,count:i32,done:usize):u32;
@external('wasi_snapshot_preview1','fd_write')
declare function writeFd(fd:i32,iov:usize,count:i32,done:usize):u32;
@external('wasi_snapshot_preview1','proc_exit')
declare function exitProcess(code:u32):void;
const incoming=new StaticArray<u8>(16);
const outgoing=new StaticArray<u8>(4096);
const vector=new StaticArray<u32>(2);
const transferred=new StaticArray<u32>(1);
/** An ordinary failure attempts the actual example close before exiting.
 * An unexpectedly returning proc_exit is a trap, never a successful return. */
export function finish(code:u32):void {if(code!=0)close_case();exitProcess(code);unreachable();}
export function check(ok:bool):void {if(!ok)finish(22);}
export function output(value:string):void {
  check(value.length<=4096);
  for(let i=0;i<value.length;++i){const c=value.charCodeAt(i);check(c<128);unchecked(outgoing[i]=<u8>c);}
  let at:u32=0,attempts:u32=0;
  while(at < (<u32>value.length) && attempts++<4096){
    unchecked(vector[0]=<u32>changetype<usize>(outgoing)+at);unchecked(vector[1]=<u32>value.length-at);unchecked(transferred[0]=0);
    const code=writeFd(1,changetype<usize>(vector),1,changetype<usize>(transferred));
    if(code==27)continue;if(code!=0)finish(code);
    const done=unchecked(transferred[0]);check(done>0&&done<=(<u32>value.length)-at);at+=done;
  }
  if(at!=(<u32>value.length))finish(110);
}
function input(length:u32):void {
  check(length>0&&length<=16);let at:u32=0,attempts:u32=0;
  while(at<length&&attempts++<32){
    unchecked(vector[0]=<u32>changetype<usize>(incoming)+at);unchecked(vector[1]=length-at);unchecked(transferred[0]=0);
    const code=readFd(0,changetype<usize>(vector),1,changetype<usize>(transferred));
    if(code==27)continue;if(code!=0)finish(code);
    const done=unchecked(transferred[0]);check(done>0&&done<=length-at);at+=done;
  }
  if(at!=length)finish(110);
}
function le32(offset:i32):u32 {return load<u32>(changetype<usize>(incoming)+offset);}
/** Emit readiness after actual AS runtime/global initialization, before the
 * typed example's initialize_case. Return explicit mode0 or1, never a grant.
 */
export function setup(family:u32):u32 {
  output('READY_LEGACY_MAIN\n');input(16);
  check(unchecked(incoming[0])==85&&unchecked(incoming[1])==76&&unchecked(incoming[2])==71&&unchecked(incoming[3])==49);
  check(le32(4)==family&&le32(8)<=1&&le32(12)==0);return le32(8);
}
export function control():u8 {input(1);return unchecked(incoming[0]);}
