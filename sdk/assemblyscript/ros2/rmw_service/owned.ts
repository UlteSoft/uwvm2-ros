/** Explicit retained-native services. No old81 service business fallback. */
import { Api, Status } from '../../api';
import { Capability } from '../../roswasm';
import { ServiceImports } from './imports';
import { CheckedCodec } from './checked';
import { Binding, Delivery, receipt, sameReceipt, delivery, fullHandle,
  INPUT_MAX, PREFIX_BYTES, MAX_WAIT_NS } from './wire';

export class Limits {
  /** Local lifetime slots never recycle. These bounds grant no Host quota. */
  constructor(public requestBytes:i32=4096,public responseBytes:i32=4096,
    public transactions:i32=64,public expectedWriter:u32=16){}
}
export class SubmitResult {status:u32=0;ticket:u64=0;}
export class AvailabilityResult {status:u32=0;value:bool=false;}
export class TakeResult<T> {
  status:u32=0;value:T|null=null;committed:bool=false;abandoned:bool=false;
  /** Set only after successful exact ACK. Control records have no typed value. */
  metadata:Delivery=new Delivery();
}
/** Independent once-ledger. 115 means entered but no returned observation yet.
 * Mutating a diagnostic snapshot never changes endpoint ownership/state.
 */
export class Diagnostics {
  firstPrimary:u32=0;activation:u32=0;activationValidation:u32=0;inspect:u32=0;inspectValidation:u32=0;
  submit:u32=0;peek:u32=0;peekValidation:u32=0;respond:u32=0;available:u32=0;abandon:u32=0;
  encode:u32=0;decode:u32=0;
  commit:u32=0;cancel:u32=0;retire:u32=0;sourceClose:u32=0;peerClose:u32=0;
  activationAttempted:bool=false;commitAttempted:bool=false;cancelAttempted:bool=false;
  retireAttempted:bool=false;sourceCloseAttempted:bool=false;peerCloseAttempted:bool=false;
  associated:bool=false;poisoned:bool=false;lastCommitSucceeded:bool=false;
  submits:u64=0;peeks:u64=0;commits:u64=0;cancels:u64=0;responses:u64=0;
  lastSequence:u64=0;lastReservation:u64=0;lastTicket:u64=0;historyUsed:i32=0;
  originalPresent:bool=false;inspectPresent:bool=false;recordPresent:bool=false;
  original:Uint8Array=new Uint8Array(256);observed:Uint8Array=new Uint8Array(256);prefix:Uint8Array=new Uint8Array(352);
}

export class OwnedService {
  private source:Capability;private peer:Capability;private api:Api;
  private sourceID:u64=0;private peerID:u64=0;
  private initial:u32=0;private ownsRaw:bool;private client:bool;private busy:bool=false;
  private attempted:bool=false;private live:bool=false;private closed:bool=false;private poisoned:bool=false;
  private cleanupNeeded:bool=false;private nativeRetired:bool=false;
  private bound:Binding=new Binding();private observedBinding:Binding=new Binding();private original:Uint8Array=new Uint8Array(256);
  private observed:Uint8Array=new Uint8Array(256);private slot:Uint8Array=new Uint8Array(8);
  private availableSlot:Uint8Array=new Uint8Array(1);private packet:Uint8Array=new Uint8Array(0);
  private tickets:StaticArray<u64>=new StaticArray<u64>(0);
  private nativeSequences:StaticArray<i64>=new StaticArray<i64>(0);
  private writers:Uint8Array=new Uint8Array(0);private states:Uint8Array=new Uint8Array(0);
  private used:i32=0;private requestBound:i32=0;private responseBound:i32=0;private writer:u32=16;
  private lastCommitted:u64=0;private lastNonce:u64=0;
  private facts:Diagnostics=new Diagnostics();
  private constructor(private imports:ServiceImports,source:Capability,peer:Capability,client:bool,own:bool,limits:Limits){
    this.source=source;this.peer=peer;this.api=source.api;this.client=client;this.ownsRaw=own;
    if(limits.requestBytes<0||limits.responseBytes<0||limits.requestBytes>INPUT_MAX||limits.responseBytes>INPUT_MAX||
      limits.transactions<1||limits.transactions>256||limits.expectedWriter<1||limits.expectedWriter>16){this.initial=22;return;}
    this.requestBound=limits.requestBytes;this.responseBound=limits.responseBytes;this.writer=limits.expectedWriter;
    this.packet=new Uint8Array(PREFIX_BYTES+(client?limits.responseBytes:limits.requestBytes));
    this.tickets=new StaticArray<u64>(limits.transactions);this.nativeSequences=new StaticArray<i64>(limits.transactions);
    this.writers=new Uint8Array(limits.transactions*16);this.states=new Uint8Array(limits.transactions);
  }
  /** All permanent state/buffers/history exist before either raw open or Native
   * activation. The factory owns exactly the two capabilities it constructs.
   */
  static openClient(api:Api,imports:ServiceImports,call:string,observer:string,limits:Limits=new Limits()):OwnedService{
    return OwnedService.openPair(api,imports,call,observer,true,limits);
  }
  static openServer(api:Api,imports:ServiceImports,advertise:string,responder:string,limits:Limits=new Limits()):OwnedService{
    return OwnedService.openPair(api,imports,advertise,responder,false,limits);
  }
  private static openPair(api:Api,imports:ServiceImports,a:string,b:string,client:bool,limits:Limits):OwnedService{
    const source=new Capability(api),peer=new Capability(api),result=new OwnedService(imports,source,peer,client,true,limits);
    if(result.initial)return result;
    if(api.backend!=2||a==b){result.initial=22;return result;}
    let error=source.open(a);if(!error)error=peer.open(b);
    if(!error)error=result.activate();result.initial=error;
    if(error){result.note(error);result.cleanupRaw();result.closed=true;}
    return result;
  }
  /** Borrowing never transfers raw owners. Caller keeps both alive and closes
   * them independently after endpoint.close(), even when Native retirement fails.
   */
  static borrowClient(imports:ServiceImports,source:Capability,observer:Capability,limits:Limits=new Limits()):OwnedService{
    return OwnedService.borrowPair(imports,source,observer,true,limits);
  }
  static borrowServer(imports:ServiceImports,source:Capability,responder:Capability,limits:Limits=new Limits()):OwnedService{
    return OwnedService.borrowPair(imports,source,responder,false,limits);
  }
  private static borrowPair(imports:ServiceImports,source:Capability,peer:Capability,client:bool,limits:Limits):OwnedService{
    const result=new OwnedService(imports,source,peer,client,false,limits);
    if(!result.initial)result.initial=result.activate();return result;
  }
  get status():u32{return this.initial;}
  isLive():bool{return this.live&&!this.closed&&!this.poisoned;}
  private note(n:u32):void{if(n&&!this.facts.firstPrimary)this.facts.firstPrimary=n;}
  private pairStatus():u32{
    if(this.api.backend!=2||this.source.api!==this.peer.api||this.source===this.peer)return 22;
    if(!this.source.isOpen()||!this.peer.isOpen()||!fullHandle(this.source.handle)||!fullHandle(this.peer.handle)||
      this.source.handle==this.peer.handle)return 9;
    if(this.attempted&&(this.source.handle!=this.sourceID||this.peer.handle!=this.peerID))return 9;
    return 0;
  }
  private preflight():u32{
    if(this.busy)return 17;if(this.initial)return this.initial;
    if(!this.isLive())return 125;return this.pairStatus();
  }
  private activate():u32{
    if(this.initial)return this.initial;let error=this.pairStatus();if(error)return error;
    const a=this.source.enterCallback();if(a===null)return 17;
    const b=this.peer.enterCallback();if(b===null){this.source.leaveCallback(a);return 17;}
    this.busy=true;this.sourceID=this.source.handle;this.peerID=this.peer.handle;this.attempted=true;
    if(this.imports.version()!=1)error=95;
    else{
      this.observed.fill(0xa5);this.facts.activationAttempted=true;this.facts.activation=115;
      error=this.imports.activate(this.client,this.sourceID,this.peerID,0,<u64>this.observed.dataStart);
      this.facts.activation=error;
      if(!error){
        this.facts.originalPresent=true;this.facts.original.set(this.observed);
        error=receipt(this.observed,1,this.client?3:4,this.sourceID,this.bound);this.facts.activationValidation=error;
        if(!error){this.original.set(this.observed);this.live=true;this.facts.associated=true;}
      }
    }
    this.note(error);this.busy=false;this.peer.leaveCallback(b);this.source.leaveCallback(a);return error;
  }
  private find(ticket:u64):i32{if(!ticket)return -1;for(let i=0;i<this.used;++i)if(this.tickets[i]==ticket)return i;return -1;}
  private retireOnce():u32{
    if(this.nativeRetired)return this.facts.retire;
    if(!this.facts.associated)return 0;
    this.nativeRetired=true;this.facts.retireAttempted=true;this.facts.retire=115;
    const error=this.imports.retire(this.sourceID,this.bound.incarnation);this.facts.retire=error;return error;
  }
  private terminal(error:u32):u32{
    this.note(error);this.poisoned=true;this.facts.poisoned=true;this.live=false;
    this.cleanupNeeded=true;this.retireOnce();return error;
  }
  private cleanupRaw():void{
    if(!this.ownsRaw)return;
    if(this.peer.isOpen()&&!this.facts.peerCloseAttempted){this.facts.peerCloseAttempted=true;this.facts.peerClose=115;this.facts.peerClose=this.peer.close();}
    if(this.source.isOpen()&&!this.facts.sourceCloseAttempted){this.facts.sourceCloseAttempted=true;this.facts.sourceClose=115;this.facts.sourceClose=this.source.close();}
  }
  private afterLease():void{this.busy=false;if(this.cleanupNeeded){this.cleanupNeeded=false;this.cleanupRaw();}}
  private inspectInside():u32{
    this.observed.fill(0xa5);this.facts.inspect=115;this.facts.inspectValidation=0;
    let error=this.imports.inspect(this.sourceID,this.bound.incarnation,0,<u64>this.observed.dataStart);
    this.facts.inspect=error;if(error)return this.terminal(error);
    this.facts.inspectPresent=true;this.facts.observed.set(this.observed);
    error=receipt(this.observed,2,this.client?3:4,this.sourceID,this.observedBinding);
    if(!error&&!sameReceipt(this.original,this.observed))error=13;
    this.facts.inspectValidation=error;return error?this.terminal(error):0;
  }
  /** Copies the actual associated UMRE, not a requested QoS profile. */
  inspect():u32{
    let error=this.preflight();if(error)return error;
    const a=this.source.enterCallback();if(a===null)return 17;
    const b=this.peer.enterCallback();if(b===null){this.source.leaveCallback(a);return 17;}
    this.busy=true;error=this.inspectInside();this.peer.leaveCallback(b);this.source.leaveCallback(a);this.afterLease();return error;
  }
  /** Snapshot allocation is a separate local operation; never invoked after an
   * ACK by take(). Caller changes cannot mutate the retained original receipt.
   */
  originalReceipt():Uint8Array{return this.original.slice();}
  submit<T>(value:T,codec:CheckedCodec<T>):SubmitResult{
    const out=new SubmitResult();out.status=this.preflight();if(out.status)return out;if(!this.client){out.status=95;return out;}
    const a=this.source.enterCallback();if(a===null){out.status=17;return out;}
    const b=this.peer.enterCallback();if(b===null){this.source.leaveCallback(a);out.status=17;return out;}
    this.busy=true;const encoded=codec.encodeChecked(value);this.facts.encode=encoded.status;
    if(encoded.status)out.status=encoded.status;
    else if(encoded.bytes.length>this.requestBound)out.status=90;
    else if(this.used==this.tickets.length||this.facts.submits==u64.MAX_VALUE)out.status=90;
    else{
      out.status=this.inspectInside();
      if(!out.status){
        const index=this.used++;this.states[index]=1;this.facts.historyUsed=this.used;++this.facts.submits;
        this.slot.fill(0);this.facts.submit=115;
        let error=this.imports.submit(this.sourceID,this.bound.incarnation,0,<u64>encoded.bytes.dataStart,<u64>encoded.bytes.length,<u64>this.slot.dataStart);
        this.facts.submit=error;const ticket=error?0:load<u64>(this.slot.dataStart);
        if(!error&&(!ticket||this.find(ticket)>=0))error=22;
        if(error)out.status=this.terminal(error);
        else{this.tickets[index]=ticket;this.states[index]=2;this.facts.lastTicket=ticket;out.ticket=ticket;}
      }
    }
    this.note(out.status);this.peer.leaveCallback(b);this.source.leaveCallback(a);this.afterLease();return out;
  }
  private cancelBorrow(item:Delivery,reason:u32,terminal:bool):u32{
    this.note(reason);this.facts.cancelAttempted=true;this.facts.cancel=115;++this.facts.cancels;
    const error=this.imports.cancel(this.sourceID,this.bound.incarnation,item.sequence,item.reservation);this.facts.cancel=error;
    if(error||terminal)this.terminal(reason);return reason;
  }
  private duplicateNative(item:Delivery):bool{
    for(let i=0;i<this.used;++i){
      if(this.states[i]<3||this.nativeSequences[i]!=item.nativeSequence)continue;
      let equal=true;for(let j=0;j<16;++j)if(this.writers[i*16+j]!=item.writer[j])equal=false;
      if(equal)return true;
    }return false;
  }
  private rememberNative(index:i32,item:Delivery):void{
    this.nativeSequences[index]=item.nativeSequence;for(let j=0;j<16;++j)this.writers[index*16+j]=item.writer[j];
  }
  private copyMetadata(from:Delivery,to:Delivery):void{
    to.associated=true;to.sequence=from.sequence;to.reservation=from.reservation;to.ticket=from.ticket;to.bodyBytes=from.bodyBytes;
    to.source=from.source;to.instance=from.instance;to.generation=from.generation;to.session=from.session;to.incarnation=from.incarnation;to.role=from.role;
    to.kind=from.kind;to.flags=from.flags;to.error=from.error;to.nativePresent=from.nativePresent;
    to.sourceTimestamp=from.sourceTimestamp;to.receivedTimestamp=from.receivedTimestamp;to.nativeSequence=from.nativeSequence;
    for(let i=0;i<16;++i)to.writer[i]=from.writer[i];
  }
  take<T>(codec:CheckedCodec<T>):TakeResult<T>{
    // Result, metadata and token ledger exist before the first actual peek.
    const out=new TakeResult<T>(),item=new Delivery();out.status=this.preflight();if(out.status)return out;
    if((!this.client&&this.used==this.tickets.length)||this.facts.peeks==u64.MAX_VALUE||
      this.facts.commits==u64.MAX_VALUE||this.facts.cancels==u64.MAX_VALUE){out.status=90;return out;}
    const a=this.source.enterCallback();if(a===null){out.status=17;return out;}
    const b=this.peer.enterCallback();if(b===null){this.source.leaveCallback(a);out.status=17;return out;}
    this.busy=true;this.takeInside(codec,item,out);this.peer.leaveCallback(b);this.source.leaveCallback(a);this.afterLease();return out;
  }
  private takeInside<T>(codec:CheckedCodec<T>,item:Delivery,out:TakeResult<T>):void{
    out.status=this.inspectInside();if(out.status)return;
    this.packet.fill(0xa5);++this.facts.peeks;this.facts.peek=115;this.facts.peekValidation=0;
    let error=this.imports.peek(this.sourceID,this.bound.incarnation,this.writer,0,<u64>this.packet.dataStart,<u64>this.packet.length);
    this.facts.peek=error;
    if(error){out.status=error;if(error!=11&&error!=90&&error!=95)this.terminal(error);else if(error!=11)this.note(error);return;}
    this.facts.recordPresent=true;for(let i=0;i<352;++i)this.facts.prefix[i]=this.packet[i];
    error=delivery(this.packet,this.bound,this.peerID,this.writer,item);this.facts.peekValidation=error;
    if(error){out.status=item.associated?this.cancelBorrow(item,error,true):this.terminal(error);return;}
    this.facts.lastSequence=item.sequence;this.facts.lastReservation=item.reservation;
    if(this.lastCommitted==u64.MAX_VALUE||item.sequence!=this.lastCommitted+1||item.reservation<=this.lastNonce){
      this.facts.peekValidation=13;out.status=this.terminal(13);return;
    }
    this.lastNonce=item.reservation;
    let index=this.find(item.ticket);
    if(item.nativePresent&&((this.client?(index<0||(this.states[index]!=2&&this.states[index]!=5)):index>=0)||(!this.client&&this.duplicateNative(item)))){
      this.facts.peekValidation=13;out.status=this.cancelBorrow(item,13,true);return;
    }
    if(!item.nativePresent&&item.ticket&&index<0){this.facts.peekValidation=13;out.status=this.cancelBorrow(item,13,true);return;}
    // State5 is an intentionally abandoned request, not a completed response.
    // A queued late body must still pass the identical full checked decoder.
    const wasAbandoned=item.nativePresent&&this.client&&this.states[index]==5;
    // Every allocation and typed validation is BEFORE commit. Failed checked
    // decode90 is retained cancellation, never swallowed by a bool predicate.
    let value:T|null=null;
    if(item.nativePresent){
      const bytes=this.packet.slice(352,352+item.bodyBytes),decoded=codec.decodeChecked(bytes);
      this.facts.decode=decoded.status;
      if(decoded.status||decoded.value===null){error=decoded.status?decoded.status:22;this.facts.decode=error;out.status=this.cancelBorrow(item,error,error!=90);return;}
      value=decoded.value;
    }
    this.facts.commitAttempted=true;this.facts.lastCommitSucceeded=false;this.facts.commit=115;++this.facts.commits;
    error=this.imports.commit(this.sourceID,this.bound.incarnation,item.sequence,item.reservation);this.facts.commit=error;
    if(error){out.status=this.terminal(error);return;}
    // No allocation, codec call, callback or import on the success tail.
    this.facts.lastCommitSucceeded=true;this.lastCommitted=item.sequence;
    if(item.nativePresent){
      if(this.client)this.states[index]=wasAbandoned?6:4; //6: once-discarded tombstone
      else{index=this.used++;this.tickets[index]=item.ticket;this.states[index]=3;this.facts.historyUsed=this.used;}
      this.rememberNative(index,item);this.facts.lastTicket=item.ticket;
      if(wasAbandoned){out.status=125;out.abandoned=true;}else out.value=value;
    }else if(item.ticket&&index>=0)this.states[index]=4;
    this.copyMetadata(item,out.metadata);out.committed=true;
    if(!item.nativePresent&&(item.flags&1)){this.live=false;this.poisoned=true;this.facts.poisoned=true;}
  }
  respond<T>(ticket:u64,value:T,codec:CheckedCodec<T>):u32{
    let error=this.preflight();if(error)return error;if(this.client)return 95;
    const index=this.find(ticket);if(index<0||this.states[index]!=3)return 9;
    const a=this.source.enterCallback();if(a===null)return 17;
    const b=this.peer.enterCallback();if(b===null){this.source.leaveCallback(a);return 17;}
    this.busy=true;const encoded=codec.encodeChecked(value);this.facts.encode=encoded.status;
    if(encoded.status)error=encoded.status;
    else if(encoded.bytes.length>this.responseBound||this.facts.responses==u64.MAX_VALUE)error=90;
    else{
      error=this.inspectInside();if(!error){
        this.states[index]=4;++this.facts.responses;this.facts.respond=115;
        error=this.imports.respond(this.sourceID,this.bound.incarnation,this.peerID,ticket,0,<u64>encoded.bytes.dataStart,<u64>encoded.bytes.length);
        this.facts.respond=error;if(error)this.terminal(error);
      }
    }
    this.note(error);this.peer.leaveCallback(b);this.source.leaveCallback(a);this.afterLease();return error;
  }
  abandon(ticket:u64):u32{
    let error=this.preflight();if(error)return error;if(!this.client)return 95;
    const index=this.find(ticket);if(index<0||this.states[index]!=2)return 9;
    const a=this.source.enterCallback();if(a===null)return 17;
    const b=this.peer.enterCallback();if(b===null){this.source.leaveCallback(a);return 17;}
    this.busy=true;error=this.inspectInside();if(!error){this.states[index]=5;this.facts.abandon=115;
      error=this.imports.abandon(this.sourceID,this.bound.incarnation,ticket);this.facts.abandon=error;if(error)this.terminal(error);}
    this.note(error);this.peer.leaveCallback(b);this.source.leaveCallback(a);this.afterLease();return error;
  }
  available(owner:Capability,waitNs:u64=0):AvailabilityResult{
    const out=new AvailabilityResult();out.status=this.preflight();if(out.status)return out;
    if(!this.client){out.status=95;return out;}
    if(waitNs>MAX_WAIT_NS){out.status=22;return out;}
    if(owner.api!==this.api||!owner.isOpen()||!fullHandle(owner.handle)||owner.handle==this.sourceID||owner.handle==this.peerID){out.status=13;return out;}
    const a=this.source.enterCallback();if(a===null){out.status=17;return out;}
    const b=this.peer.enterCallback();if(b===null){this.source.leaveCallback(a);out.status=17;return out;}
    const c=owner.enterCallback();if(c===null){this.peer.leaveCallback(b);this.source.leaveCallback(a);out.status=17;return out;}
    this.busy=true;out.status=this.inspectInside();if(!out.status){
      this.availableSlot[0]=0xa5;this.facts.available=115;
      let error=this.imports.available(this.sourceID,this.bound.incarnation,owner.handle,waitNs,0,<u64>this.availableSlot.dataStart);
      this.facts.available=error;if(!error&&this.availableSlot[0]>1)error=22;
      if(error)out.status=this.terminal(error);else out.value=this.availableSlot[0]!=0;
    }
    owner.leaveCallback(c);this.peer.leaveCallback(b);this.source.leaveCallback(a);this.afterLease();return out;
  }
  /** Once-retire then independently close both genuinely owned raw owners,
   * regardless of Native error. Borrowed raw owners are NEVER closed here.
   */
  close():u32{
    if(this.busy||this.source.retirementStatus()==17||this.peer.retirementStatus()==17)return 17;
    if(this.closed)return 9;this.closed=true;this.live=false;this.busy=true;
    const error=this.facts.associated?this.retireOnce():this.initial;this.cleanupRaw();
    this.busy=false;
    return error?error:this.facts.peerClose?this.facts.peerClose:this.facts.sourceClose;
  }
  diagnostics():Diagnostics{
    const d=new Diagnostics(),f=this.facts;
    d.firstPrimary=f.firstPrimary;d.activation=f.activation;d.activationValidation=f.activationValidation;d.inspect=f.inspect;d.inspectValidation=f.inspectValidation;
    d.submit=f.submit;d.peek=f.peek;d.peekValidation=f.peekValidation;d.respond=f.respond;d.available=f.available;d.abandon=f.abandon;
    d.encode=f.encode;d.decode=f.decode;
    d.commit=f.commit;d.cancel=f.cancel;d.retire=f.retire;d.sourceClose=f.sourceClose;d.peerClose=f.peerClose;
    d.activationAttempted=f.activationAttempted;d.commitAttempted=f.commitAttempted;d.cancelAttempted=f.cancelAttempted;
    d.retireAttempted=f.retireAttempted;d.sourceCloseAttempted=f.sourceCloseAttempted;d.peerCloseAttempted=f.peerCloseAttempted;
    d.associated=f.associated;d.poisoned=f.poisoned;d.lastCommitSucceeded=f.lastCommitSucceeded;
    d.submits=f.submits;d.peeks=f.peeks;d.commits=f.commits;d.cancels=f.cancels;d.responses=f.responses;
    d.lastSequence=f.lastSequence;d.lastReservation=f.lastReservation;d.lastTicket=f.lastTicket;d.historyUsed=f.historyUsed;
    d.originalPresent=f.originalPresent;d.inspectPresent=f.inspectPresent;d.recordPresent=f.recordPresent;
    d.original.set(f.original);d.observed.set(f.observed);d.prefix.set(f.prefix);return d;
  }
}
