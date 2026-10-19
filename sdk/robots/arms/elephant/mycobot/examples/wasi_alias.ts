/** Basic stdin only: one nonzero byte length, followed by exactly that many
 * strict UTF-8 alias bytes (maximum255). No file, environment or network API.
 * Input chooses an existing alias, never a native topic or a Runtime grant.
 */
import {validUtf8} from "../../../../../assemblyscript/codecs";
@external("wasi_snapshot_preview1","fd_read") declare function fd_read(fd:u32,iovs:usize,count:u32,size:usize):u32;
@external("wasi_snapshot_preview1","proc_exit") export declare function proc_exit(code:u32):void;
export class AliasInput {constructor(public status:u32=22,public alias:string=""){} }
function exact(bytes:Uint8Array):bool {
  let offset:i32=0;const iov=new Uint32Array(2),received=new Uint32Array(1);
  while(offset<bytes.length){iov[0]=<u32>(bytes.dataStart+offset);iov[1]=<u32>(bytes.length-offset);received[0]=0;
    if(fd_read(0,iov.dataStart,1,received.dataStart)||received[0]==0||received[0]>iov[1])return false;
    offset+=<i32>received[0];}
  return true;
}
export function readAlias():AliasInput {
  const prefix=new Uint8Array(1);if(!exact(prefix)||prefix[0]==0)return new AliasInput();
  const bytes=new Uint8Array(prefix[0]);if(!exact(bytes)||!validUtf8(bytes))return new AliasInput();
  for(let i=0;i<bytes.length;++i)if(bytes[i]==0)return new AliasInput();
  return new AliasInput(0,String.UTF8.decode(bytes.buffer));
}
