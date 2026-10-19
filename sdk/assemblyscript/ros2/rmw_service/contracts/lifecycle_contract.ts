/** Synthetic Guest-local mechanics only; no Runtime/Native authorization claim. */
import { Api } from '../../../api';
import { Capability } from '../../../roswasm';
import { ServiceImports } from '../imports';
import { OwnedService,Limits } from '../owned';
import { CheckedCodec,EncodeResult,DecodeResult } from '../checked';
import { SOURCE,PEER,AVAILABILITY,endpoint,packet } from './fixtures';
import { Empty,EmptyCodec } from '../examples/builtin_values';
export { abortGuest } from '../../../api';

class Business extends Api{
  opens:u32=0;closes:u32=0;failOpen:u32=0;closeError:u32=0;
  constructor(){super(2);}
  version():u32{return 1;}
  open(m:u32,p:u64,n:u64,out:u64):u32{++this.opens;if(this.opens==this.failOpen)return 13;
    store<u64>(<usize>out,this.opens==1?SOURCE:this.opens==2?PEER:AVAILABILITY);return 0;}
  close(h:u64):u32{++this.closes;return this.closeError;}
}
class Value{constructor(public n:u8=7){}}
class Codec extends CheckedCodec<Value>{
  encodeCode:u32=0;decodeCode:u32=0;
  encodeChecked(v:Value):EncodeResult{const b=new Uint8Array(1);b[0]=v.n;return new EncodeResult(this.encodeCode,b);}
  decodeChecked(b:Uint8Array):DecodeResult<Value>{
    if(this.decodeCode)return new DecodeResult<Value>(this.decodeCode);
    return b.length==1?new DecodeResult<Value>(0,new Value(b[0])):new DecodeResult<Value>(22);
  }
}
class CountingCodec extends Codec{
  calls:u32=0;
  decodeChecked(b:Uint8Array):DecodeResult<Value>{++this.calls;return super.decodeChecked(b);}
}
class ReentryCodec extends Codec{
  endpoint:OwnedService|null=null;source:Capability|null=null;peer:Capability|null=null;
  endpointCode:u32=0;sourceCode:u32=0;peerCode:u32=0;
  encodeChecked(v:Value):EncodeResult{
    const endpoint=this.endpoint,source=this.source,peer=this.peer;
    if(endpoint!==null)this.endpointCode=endpoint.close();if(source!==null)this.sourceCode=source.close();if(peer!==null)this.peerCode=peer.close();
    return super.encodeChecked(v);
  }
}
class Imports extends ServiceImports{
  kind:u8=3;activateCode:u32=0;retireCode:u32=0;commitCode:u32=0;cancelCode:u32=0;availableCode:u32=0;
  invalidReceipt:bool=false;invalidHeader:bool=false;invalidMetadata:bool=false;inspectChanged:bool=false;
  retires:u32=0;commits:u32=0;cancels:u32=0;submits:u32=0;peeks:u32=0;responds:u32=0;
  nextNonce:u64=1;sequence:u64=1;ticket:u64=u64.MAX_VALUE;idle:bool=false;availableByte:u8=1;
  bodyBytes:i32=1;
  version():u32{return 1;}
  activate(client:bool,s:u64,p:u64,m:u32,out:u64):u32{
    if(this.activateCode)return this.activateCode;this.kind=client?3:4;const b=new Uint8Array(256);endpoint(b,1,this.kind);
    if(this.invalidReceipt)b[15]=1;memory.copy(<usize>out,b.dataStart,256);return 0;
  }
  inspect(s:u64,i:u64,m:u32,out:u64):u32{const b=new Uint8Array(256);endpoint(b,2,this.kind);if(this.inspectChanged)store<u64>(b.dataStart+80,1001);memory.copy(<usize>out,b.dataStart,256);return 0;}
  submit(s:u64,i:u64,m:u32,p:u64,n:u64,out:u64):u32{++this.submits;store<u64>(<usize>out,this.ticket);return 0;}
  abandon(s:u64,i:u64,t:u64):u32{return 0;}
  respond(s:u64,i:u64,p:u64,t:u64,m:u32,b:u64,n:u64):u32{++this.responds;return 0;}
  peek(s:u64,i:u64,w:u32,m:u32,out:u64,n:u64):u32{
    ++this.peeks;if(this.idle)return 11;const b=new Uint8Array(<i32>n);packet(b,this.kind,this.sequence,this.nextNonce++,this.ticket,this.bodyBytes);
    if(this.invalidHeader)b[14]=1;if(this.invalidMetadata)b[283]=1;memory.copy(<usize>out,b.dataStart,n);return 0;
  }
  commit(s:u64,i:u64,q:u64,r:u64):u32{++this.commits;if(!this.commitCode){++this.sequence;this.idle=true;}return this.commitCode;}
  cancel(s:u64,i:u64,q:u64,r:u64):u32{++this.cancels;return this.cancelCode;}
  available(s:u64,i:u64,a:u64,w:u64,m:u32,out:u64):u32{if(!this.availableCode)store<u8>(<usize>out,this.availableByte);return this.availableCode;}
  retire(s:u64,i:u64):u32{++this.retires;return this.retireCode;}
}
let checks:u32=0;
function require(v:bool):void{if(!v)unreachable();++checks;}
function owned(b:Business,n:Imports):OwnedService{return OwnedService.openClient(b,n,'call','observer',new Limits(1,1,2));}
export function _start():void{
  // True source error is retained independently from rollback close107.
  const b0=new Business(),n0=new Imports();b0.failOpen=2;b0.closeError=107;const denied=owned(b0,n0);
  require(denied.status==13&&b0.closes==1&&n0.retires==0&&denied.diagnostics().sourceClose==107);
  const b1=new Business(),n1=new Imports();n1.invalidReceipt=true;const malformed=owned(b1,n1);
  require(malformed.status==22&&b1.closes==2&&n1.retires==0&&!malformed.diagnostics().associated);
  const b2=new Business(),n2=new Imports(),e=owned(b2,n2),c=new Codec();
  require(e.status==0&&b2.opens==2&&n2.submits==0);
  const submitted=e.submit<Value>(new Value(),c);require(submitted.status==0&&submitted.ticket==u64.MAX_VALUE);
  const taken=e.take<Value>(c);require(taken.status==0&&taken.committed&&taken.value!==null&&taken.value!.n==7);
  require(taken.metadata.nativeSequence==-1&&taken.metadata.writer[0]==0&&n2.commits==1);
  require(e.abandon(submitted.ticket)==9&&n2.retires==0);
  require(e.take<Value>(c).status==11&&n2.commits==1);
  require(e.close()==0&&n2.retires==1&&b2.closes==2&&e.close()==9&&n2.retires==1);
  const b3=new Business(),n3=new Imports(),bad=owned(b3,n3);bad.submit<Value>(new Value(),c);n3.invalidMetadata=true;n3.cancelCode=107;n3.retireCode=5;b3.closeError=110;
  const refused=bad.take<Value>(c),facts=bad.diagnostics();
  require(refused.status==22&&!refused.committed&&refused.value===null&&n3.cancels==1&&n3.commits==0);
  require(facts.firstPrimary==22&&facts.cancel==107&&facts.retire==5&&facts.sourceClose==110&&facts.peerClose==110&&b3.closes==2);
  const b4=new Business(),n4=new Imports(),unknown=owned(b4,n4);unknown.submit<Value>(new Value(),c);n4.invalidHeader=true;
  require(unknown.take<Value>(c).status==22&&n4.cancels==0&&n4.retires==1);
  const b5=new Business(),n5=new Imports(),ambiguous=owned(b5,n5);ambiguous.submit<Value>(new Value(),c);n5.commitCode=110;
  const ack=ambiguous.take<Value>(c);require(ack.status==110&&ack.value===null&&!ack.committed&&n5.commits==1&&n5.cancels==0&&n5.retires==1);
  require(ambiguous.take<Value>(c).status==125&&n5.commits==1);
  const b6=new Business(),n6=new Imports(),capacity=owned(b6,n6),small=new Codec();capacity.submit<Value>(new Value(),small);small.decodeCode=90;
  require(capacity.take<Value>(small).status==90&&n6.cancels==1&&n6.commits==0&&n6.retires==0);
  small.decodeCode=0;const retried=capacity.take<Value>(small);require(retried.status==0&&retried.metadata.reservation==2&&n6.commits==1);capacity.close();
  const b7=new Business(),n7=new Imports(),borrowSource=new Capability(b7),borrowPeer=new Capability(b7);
  require(borrowSource.open('call')==0&&borrowPeer.open('observer')==0);
  const borrowed=OwnedService.borrowClient(n7,borrowSource,borrowPeer,new Limits(1,1,2));n7.retireCode=107;
  require(borrowed.close()==107&&n7.retires==1&&b7.closes==0&&borrowSource.isOpen()&&borrowPeer.isOpen());
  borrowPeer.close();borrowSource.close();require(b7.closes==2);
  const b8=new Business(),n8=new Imports(),query=owned(b8,n8),available=new Capability(b8);
  require(available.open('availability')==0);const ready=query.available(available,0);
  require(ready.status==0&&ready.value);n8.availableByte=2;require(query.available(available,0).status==22&&n8.retires==1);
  available.close();query.close();
  const b9=new Business(),n9=new Imports(),server=OwnedService.openServer(b9,n9,'advertise','respond',new Limits(1,1,2));
  require(server.status==0);const request=server.take<Value>(c);
  require(request.status==0&&request.committed&&request.metadata.kind==2&&request.metadata.ticket==u64.MAX_VALUE);
  require(server.respond<Value>(request.metadata.ticket,new Value(),c)==0&&n9.responds==1);
  require(server.respond<Value>(request.metadata.ticket,new Value(),c)==9&&n9.responds==1);
  // A different local ticket cannot make the same native writer+signed seq new.
  n9.idle=false;n9.ticket=1;require(server.take<Value>(c).status==13&&n9.cancels==1&&n9.retires==1);
  const b10=new Business(),n10=new Imports(),retirement=owned(b10,n10);n10.retireCode=5;b10.closeError=107;
  require(retirement.close()==5&&n10.retires==1&&b10.closes==2);
  const cleanup=retirement.diagnostics();require(cleanup.retire==5&&cleanup.peerClose==107&&cleanup.sourceClose==107);
  const b11=new Business(),n11=new Imports(),source11=new Capability(b11),peer11=new Capability(b11);
  require(source11.open('call')==0&&peer11.open('observer')==0);
  const callback=OwnedService.borrowClient(n11,source11,peer11,new Limits(1,1,2)),reentry=new ReentryCodec();
  reentry.endpoint=callback;reentry.source=source11;reentry.peer=peer11;
  require(callback.submit<Value>(new Value(),reentry).status==0&&reentry.endpointCode==17&&reentry.sourceCode==17&&reentry.peerCode==17);
  callback.close();peer11.close();source11.close();
  const b12=new Business(),n12=new Imports(),checked=owned(b12,n12),encoder=new Codec();encoder.encodeCode=90;
  require(checked.submit<Value>(new Value(),encoder).status==90&&n12.submits==0&&checked.diagnostics().historyUsed==0);checked.close();
  const b13=new Business(),n13=new Imports(),emptyCodec=new EmptyCodec();n13.bodyBytes=0;
  const empty=OwnedService.openClient(b13,n13,'empty.call','empty.observe',new Limits(0,0,1));
  require(empty.status==0&&empty.submit<Empty>(new Empty(),emptyCodec).status==0);
  const noBody=empty.take<Empty>(emptyCodec);require(noBody.status==0&&noBody.committed&&noBody.value!==null&&noBody.metadata.bodyBytes==0);empty.close();
  // A response already queued before abandon is decoded completely, ACKed once,
  // and discarded without retiring an unrelated still-pending request.
  const b14=new Business(),n14=new Imports(),late=OwnedService.openClient(b14,n14,'late.call','late.observe',new Limits(1,1,3)),lateCodec=new CountingCodec();
  const lateTicket=late.submit<Value>(new Value(),lateCodec);require(lateTicket.status==0&&late.abandon(lateTicket.ticket)==0);
  n14.ticket=1;require(late.submit<Value>(new Value(),lateCodec).status==0);n14.ticket=lateTicket.ticket;
  const discarded=late.take<Value>(lateCodec);
  require(discarded.status==125&&discarded.abandoned&&discarded.committed&&discarded.value===null&&discarded.metadata.ticket==lateTicket.ticket);
  require(lateCodec.calls==1&&n14.commits==1&&n14.cancels==0&&n14.retires==0&&late.isLive());
  n14.ticket=1;n14.idle=false;const other=late.take<Value>(lateCodec);
  require(other.status==0&&other.committed&&!other.abandoned&&other.value!==null&&n14.commits==2&&n14.retires==0);
  // The consumed-discard tombstone cannot deliver a second success.
  n14.ticket=lateTicket.ticket;n14.idle=false;require(late.take<Value>(lateCodec).status==13&&n14.cancels==1&&n14.commits==2&&n14.retires==1);late.close();
  // Malformed abandoned data still reaches the actual checked decoder and
  // cancels its exact borrow; abandonment never bypasses typed validation.
  const b15=new Business(),n15=new Imports(),malformedLate=owned(b15,n15),badLateCodec=new CountingCodec();
  const badTicket=malformedLate.submit<Value>(new Value(),badLateCodec);require(badTicket.status==0&&malformedLate.abandon(badTicket.ticket)==0);
  badLateCodec.decodeCode=22;const badLate=malformedLate.take<Value>(badLateCodec);
  require(badLate.status==22&&!badLate.committed&&!badLate.abandoned&&badLate.value===null&&badLateCodec.calls==1&&n15.cancels==1&&n15.commits==0&&n15.retires==1);malformedLate.close();
  // Checked capacity90 retains state5 and a fresh explicit later borrow may
  // complete disposal. A failed/unknown commit never publishes disposal.
  const b16=new Business(),n16=new Imports(),capacityLate=owned(b16,n16),capacityLateCodec=new CountingCodec();
  const capTicket=capacityLate.submit<Value>(new Value(),capacityLateCodec);require(capTicket.status==0&&capacityLate.abandon(capTicket.ticket)==0);
  capacityLateCodec.decodeCode=90;require(capacityLate.take<Value>(capacityLateCodec).status==90&&n16.cancels==1&&n16.commits==0&&n16.retires==0);
  capacityLateCodec.decodeCode=0;const disposed=capacityLate.take<Value>(capacityLateCodec);
  require(disposed.status==125&&disposed.abandoned&&disposed.committed&&capacityLateCodec.calls==2&&disposed.metadata.reservation==2&&n16.commits==1&&n16.retires==0);capacityLate.close();
  const b17=new Business(),n17=new Imports(),unknownLate=owned(b17,n17),unknownLateCodec=new CountingCodec();
  const unknownTicket=unknownLate.submit<Value>(new Value(),unknownLateCodec);require(unknownTicket.status==0&&unknownLate.abandon(unknownTicket.ticket)==0);
  n17.commitCode=110;const uncertain=unknownLate.take<Value>(unknownLateCodec);
  require(uncertain.status==110&&!uncertain.committed&&!uncertain.abandoned&&uncertain.value===null&&unknownLateCodec.calls==1&&n17.commits==1&&n17.cancels==0&&n17.retires==1);unknownLate.close();
}
export function completedChecks():u32{return checks;}
