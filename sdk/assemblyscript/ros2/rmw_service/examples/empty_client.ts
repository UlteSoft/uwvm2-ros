/** Caller-driven whole Guest entry. An external actual bounded driver calls
 * begin, step, finish; step11 is idle, never a guessed completed roundtrip.
 * Compiled alias strings select Host-configured grants and grant nothing.
 */
import { NativeApi } from '../../roswasm';
import { NativeServiceImports } from '../imports';
import { OwnedService,Limits } from '../owned';
import { Empty,EmptyCodec } from './builtin_values';
export {abortGuest} from '../../../api';
let endpoint:OwnedService|null=null;
const codec=new EmptyCodec();
let submitted:u64=0;let complete:bool=false;
export function begin():u32{
  if(endpoint!==null)return 17;
  const opened=OwnedService.openClient(new NativeApi(),new NativeServiceImports(),'empty.call','empty.observe',new Limits(0,0,1));
  endpoint=opened;if(opened.status)return opened.status;
  const result=opened.submit<Empty>(new Empty(),codec);if(!result.status)submitted=result.ticket;return result.status;
}
export function step():u32{
  const current=endpoint;if(current===null||!submitted||complete)return 9;
  const result=current.take<Empty>(codec);if(result.status)return result.status;
  if(!result.metadata.nativePresent)return result.metadata.error?result.metadata.error:125;
  if(result.value===null||result.metadata.ticket!=submitted)return 22;complete=true;return 0;
}
export function completed():bool{return complete;}
export function finish():u32{const current=endpoint;return current===null?9:current.close();}
