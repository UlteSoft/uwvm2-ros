/** Exact canonical Empty / AddTwoInts fields, independent of native CDR.
 * Actual Native identities/full original graph are verified by the Host and
 * genuine selected Native Registry; no hash or fake TypeSupport is supplied.
 */
import { CheckedCodec,EncodeResult,DecodeResult } from '../checked';
export class Empty{}
export class EmptyCodec extends CheckedCodec<Empty>{
  encodeChecked(value:Empty):EncodeResult{return new EncodeResult(0,new Uint8Array(0));}
  decodeChecked(bytes:Uint8Array):DecodeResult<Empty>{return bytes.length==0?new DecodeResult<Empty>(0,new Empty()):new DecodeResult<Empty>(22);}
}
export class AddRequest{constructor(public a:i64=0,public b:i64=0){}}
export class AddResponse{constructor(public sum:i64=0){}}
export class AddRequestCodec extends CheckedCodec<AddRequest>{
  encodeChecked(v:AddRequest):EncodeResult{const b=new Uint8Array(16);store<i64>(b.dataStart,v.a);store<i64>(b.dataStart+8,v.b);return new EncodeResult(0,b);}
  decodeChecked(b:Uint8Array):DecodeResult<AddRequest>{return b.length==16?new DecodeResult<AddRequest>(0,new AddRequest(load<i64>(b.dataStart),load<i64>(b.dataStart+8))):new DecodeResult<AddRequest>(22);}
}
export class AddResponseCodec extends CheckedCodec<AddResponse>{
  encodeChecked(v:AddResponse):EncodeResult{const b=new Uint8Array(8);store<i64>(b.dataStart,v.sum);return new EncodeResult(0,b);}
  decodeChecked(b:Uint8Array):DecodeResult<AddResponse>{return b.length==8?new DecodeResult<AddResponse>(0,new AddResponse(load<i64>(b.dataStart))):new DecodeResult<AddResponse>(22);}
}
