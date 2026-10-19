/** Actual embedded QuickJS-in-Wasm command entry, not a Node-host test.
 * Basic stdin contains a u8 length and strict UTF-8 alias bytes. Output is two
 * u32 little-endian statuses: primary, cleanup. Engine exit0 means this script
 * completed; the primary status still describes the ROS operation (e.g.13).
 * No exception is discarded: unexpected thrown values terminate the script.
 */
import {createGuestSession,readGuestByte,writeGuestOutput} from "../../../../../javascript/guest.mjs";
import {readOnce} from "./telemetry_galactic_pro450.mjs";
function byte(){const n=readGuestByte();if(!Number.isInteger(n)||n<0||n>255)throw new RangeError('Basic stdin byte required');return n;}
export let result=null;
/** The genuine command engine calls this export after module initialization.
 * Module import itself never reads stdin, opens a capability or starts work.
 */
export function main(){
  const size=byte();if(!size)throw new RangeError('Explicit nonempty alias required');
  const bytes=new Uint8Array(size);for(let i=0;i<size;++i)bytes[i]=byte();
  const alias=new TextDecoder('utf-8',{fatal:true,ignoreBOM:true}).decode(bytes);
  const session=createGuestSession();
  result=readOnce(session,alias);
  if(result.error&&(!Number.isInteger(result.error.code)||result.error.code<=0))throw result.error;
  if(result.cleanupError)throw result.cleanupError;
  const output=new Uint8Array(8),view=new DataView(output.buffer);
  view.setUint32(0,result.primary,true);view.setUint32(4,result.cleanup,true);
  const written=writeGuestOutput(output);if(written!==0)throw new Error('Basic stdout refused: '+written);
  return 0;
}
