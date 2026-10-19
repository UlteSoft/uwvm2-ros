/** Real caller-driven server over the separate optional raw12 route.
 * No native callback enters Wasm; actual application dispatch follows ACK.
 */
import { NativeApi } from '../../roswasm';
import { NativeServiceImports } from '../imports';
import { OwnedService,Limits } from '../owned';
import { AddRequest,AddResponse,AddRequestCodec,AddResponseCodec } from './builtin_values';
export {abortGuest} from '../../../api';
let endpoint:OwnedService|null=null;
const requestCodec=new AddRequestCodec(),responseCodec=new AddResponseCodec();
export function begin():u32{
  if(endpoint!==null)return 17;
  const opened=OwnedService.openServer(new NativeApi(),new NativeServiceImports(),'add.advertise','add.respond',new Limits(16,8,64));
  endpoint=opened;return opened.status;
}
export function step():u32{
  const current=endpoint;if(current===null)return 9;const result=current.take<AddRequest>(requestCodec);
  if(result.status)return result.status;if(!result.metadata.nativePresent)return result.metadata.error?result.metadata.error:125;
  const request=result.value;if(request===null)return 22;const sum=request.a+request.b;
  // Application-level explicit refusal instead of signed arithmetic overflow.
  if((request.a>=0&&request.b>=0&&sum<0)||(request.a<0&&request.b<0&&sum>=0))return 22;
  return current.respond<AddResponse>(result.metadata.ticket,new AddResponse(sum),responseCodec);
}
export function finish():u32{const current=endpoint;return current===null?9:current.close();}
