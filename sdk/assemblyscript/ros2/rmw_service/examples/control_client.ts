/** Real full-control checked records, produced separately by the actual
 * records-only operator. No handwritten replacement generated types are here.
 * This helper is imported by an actual application with its explicit aliases.
 */
import * as records from '../../../../custom-types/generated-control-managed/bounded_records';
import { Api } from '../../../api';
import { ServiceImports } from '../imports';
import { OwnedService,Limits,SubmitResult,TakeResult } from '../owned';
import { CheckedCodec,EncodeResult,DecodeResult } from '../checked';

type Request=records.Ros2Type_description_interfacesSrvGetTypeDescription_Request;
type Response=records.Ros2Type_description_interfacesSrvGetTypeDescription_Response;
class RequestCodec extends CheckedCodec<Request>{
  private actual:records.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec=new records.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec();
  encodeChecked(v:Request):EncodeResult{const r=this.actual.encodeChecked(v);return new EncodeResult(r.status,r.bytes);}
  decodeChecked(b:Uint8Array):DecodeResult<Request>{const r=this.actual.decodeChecked(b);return new DecodeResult<Request>(r.status,r.value);}
}
class ResponseCodec extends CheckedCodec<Response>{
  private actual:records.Ros2Type_description_interfacesSrvGetTypeDescription_ResponseCodec=new records.Ros2Type_description_interfacesSrvGetTypeDescription_ResponseCodec();
  encodeChecked(v:Response):EncodeResult{const r=this.actual.encodeChecked(v);return new EncodeResult(r.status,r.bytes);}
  decodeChecked(b:Uint8Array):DecodeResult<Response>{const r=this.actual.decodeChecked(b);return new DecodeResult<Response>(r.status,r.value);}
}
/** No hidden wait, clock, thread, graph projection or default grant. */
export class GetDescriptionClient{
  private requests:RequestCodec=new RequestCodec();private responses:ResponseCodec=new ResponseCodec();
  private endpoint:OwnedService;
  constructor(api:Api,imports:ServiceImports,callAlias:string,observerAlias:string){
    this.endpoint=OwnedService.openClient(api,imports,callAlias,observerAlias,new Limits(
      records.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec.maxWireBytes,
      records.Ros2Type_description_interfacesSrvGetTypeDescription_ResponseCodec.maxWireBytes,64,16));
  }
  get status():u32{return this.endpoint.status;}
  /** The query's type hash is actual caller application DATA, not this
   * service's Native RIHS or permission. Default include_type_sources is true.
   */
  submit(actualTypeName:string,actualRequestedTypeHash:string):SubmitResult{
    const request=new records.Ros2Type_description_interfacesSrvGetTypeDescription_Request();request.type_name=actualTypeName;request.type_hash=actualRequestedTypeHash;
    request.include_type_sources=true;return this.endpoint.submit<Request>(request,this.requests);
  }
  /** One actual same-record delivery. Raw TypeSource bytes remain Uint8Array;
   * aggregate-capacity90 is returned distinctly and does not commit a record.
   */
  takeOnce():TakeResult<Response>{return this.endpoint.take<Response>(this.responses);}
  abandon(ticket:u64):u32{return this.endpoint.abandon(ticket);}
  close():u32{return this.endpoint.close();}
}
