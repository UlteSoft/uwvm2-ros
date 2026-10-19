import { EventBinding } from './events.mjs';
/**
 * @file async.mjs
 * @brief Asynchronous SDK over a real relay or explicit same-Wasm copied calls.
 *
 * This API does not change the synchronous Session/Node classes. It supplies
 * no WebAssembly.Memory/Instance and performs no native ROS or Gateway I/O.
 * The host supervisor attaches a qualified persistent Worker to RelayChannel.
 * An embedded engine instead supplies an explicit CopiedChannel; all resource,
 * codec, lifecycle and status rules below are shared by both transports.
 * Every alias and operation still requires explicit Runtime authority.
 */
import { RelayChannel, RelayError } from './relay.mjs';
import { CopiedTransport, CopiedChannel } from './copied_transport.mjs';
import { specification, Status, u64, prepareCallEntry, invokeCallEntry, callEntryAttempted } from './raw.mjs';
import { RosError } from './roswasm.mjs';
import { DeferredQueue, ParameterCallbacks } from './scheduling.mjs';
import { distinctOwners, ownerPolling, ownerRetiring, ownersBusy, acquirePolling, acquireRetirement } from './ownership.mjs';
export { Future, CallbackGroup, AsyncTimer, SchedulingError, ParameterCallbacks } from './scheduling.mjs';
import { StringMessage, BoolMessage, TriggerRequest, ServiceResponse,
  decodeServiceRequest, encodeServiceReply, FibonacciGoal, encodeGoalId,
  decodeGoalId, decodeIncomingGoal, encodeActionSequence, decodeActionSequence,
  TransformMessage, transformMessage, encodeLookupTime, LifecycleState, TimeNanoseconds,
  TypedParameterEvent, ActionResult } from './codecs.mjs';
const SENTINEL=0xffffffffffffffffn;
const pendingCapability=Symbol('pending SDK async capability');
const capabilityRecords=new WeakMap(),nodeRecords=new WeakMap();
function capabilityRecord(cap){const record=capabilityRecords.get(cap);if(!record)throw new TypeError('Actual SDK async capability required');return record;}
function capabilityRetired(cap){const record=capabilityRecord(cap);return record.closed||(record.ticket!==null&&callEntryAttempted(record.ticket));}
function nodeOwners(node){const record=nodeRecords.get(node),owners=node.owned.slice();for(const ledger of record.ledgers)for(const cap of ledger.owners)if(cap)owners.push(cap);return distinctOwners(owners);}
function pruneNodeLedgers(record){let at=0;for(const ledger of record.ledgers)if(ledger.owners.some(cap=>cap&&!capabilityRetired(cap)))record.ledgers[at++]=ledger;record.ledgers.length=at;}
function nodeHasLiveOwners(node){for(const cap of node.owned)if(cap&&!capabilityRetired(cap))return true;for(const ledger of nodeRecords.get(node).ledgers)for(const cap of ledger.owners)if(cap&&!capabilityRetired(cap))return true;return false;}


/** One explicitly selected channel; offsets describe its reserved scratch only.
 * Results are copied Uint8Arrays, never borrowed Wasm memory. Scheduling stays
 * caller-driven in either host or guest; it requests no implicit clock right.
 */
export class AsyncSession {
  static async attach(options) { return new AsyncSession(await RelayChannel.attach(options)); }
  /** Construct a same-Wasm copied session. This is an explicit second transport,
   * not a fabricated RelayChannel and not an external Node authority grant.
   */
  static fromCopiedTransport(transport) {
    if (!(transport instanceof CopiedTransport)) throw new TypeError('Explicit copied transport required');
    return new AsyncSession(new CopiedChannel(transport));
  }
  constructor(channel) {
    if((!(channel instanceof RelayChannel)&&!(channel instanceof CopiedChannel))||!channel.layout||channel.state!=='ready')
      throw new TypeError('Attached real relay or explicit same-Wasm copied channel required');
    this.channel=channel;this.version=channel.version;this.layout=channel.layout;
  }
  /** Open one exact UTF-8 alias. On success preserve all64 opaque handle bits.
   * A zero seed avoids confusing a legitimate high-bit handle with a length
   * sentinel. On refusal no owned handle is returned and no grant is inferred.
   */
  async open(alias) {
    if(typeof alias!=='string'||!alias.length||alias.includes('\0')) throw new TypeError('Nonempty NUL-free alias required');
    const input=StringMessage.encode(alias).subarray(4),s=this.layout;
    const owner=new AsyncCapability(this,0n,pendingCapability);
    const result=await this.channel.call('capability_open',[0,s.input,BigInt(input.length),s.slot],{input,slotSeed:0n});
    if(result.status) throw new RosError(result.status,'capability_open');
    if(!result.slot) throw new RelayError('Runtime did not return a nonzero owned handle');
    owner.handle=result.slot;return owner;
  }
  /** Execute any selected original ABI entry using explicit typed parameters.
   * This is the full38/43 raw transport surface with reserved-memory bounds.
   * It returns copied snapshots for sentinel diagnostics, not a fake memory.
   */
  async raw(name,args,options) { return this.channel.call(name,args,options); }
  /** Interpret output only under the original status/length contract. Error90
   * may leave the seed intact or consume a too-large event. Never replay it.
   */
  result(reply,capacity,operation) {
    const required=(reply.status===0||reply.status===Status.TOO_LARGE)&&reply.slot!==SENTINEL?reply.slot:null;
    if(reply.status===0&&(required===null||required>BigInt(capacity)))
      throw new RelayError(operation+': invalid successful output length');
    return Object.freeze({status:reply.status,required,
      bytes:reply.status?new Uint8Array():reply.snapshot.slice(0,Number(required)),
      slot:reply.slot,slotBytes:reply.slotBytes.slice()});
  }
  /** Invoke an exact seven-argument resource request once. Empty inputs do not
   * mean arbitrary resource selection: the handle's policy remains authoritative.
   */
  async request(name,handle,input=new Uint8Array(),capacity=this.layout.outputCapacity) {
    const entry=specification(this.version).functions.find(f=>f.name===name);
    if(!entry||entry.parameters.length!==7) throw new RosError(Status.UNSUPPORTED,name);
    if(!(input instanceof Uint8Array)||!Number.isSafeInteger(capacity)||capacity<0||capacity>this.layout.outputCapacity)
      throw new RangeError('Explicit bounded request/output storage required');
    const s=this.layout;
    const reply=await this.channel.call(name,[u64(handle),0,s.input,BigInt(input.length),s.output,BigInt(capacity),s.slot],
      {input,snapshot:capacity,slotSeed:SENTINEL});
    return this.result(reply,capacity,name);
  }
  /** Exact observer association over the real persistent Guest relay. */
  async watchEvents(observer,source,correlation=0n) {return (await this.channel.call('event_watch',[u64(observer),u64(source),u64(correlation)])).status;}
  /** New capacity-checked poll. Code90 retains a record; no implicit resize. */
  async pollEventV1(observer,capacity=this.layout.outputCapacity) {
    if(!Number.isSafeInteger(capacity)||capacity<0||capacity>this.layout.outputCapacity)throw new RangeError('Explicit bounded capacity required');const s=this.layout;
    return this.result(await this.channel.call('event_poll_v1',[u64(observer),0,s.output,BigInt(capacity),s.slot],{snapshot:capacity,slotSeed:SENTINEL}),capacity,'event_poll_v1');
  }
  /** Copied readiness counters, with no fabricated length-slot result. */
  async eventReadiness(observer) {const s=this.layout;if(s.outputCapacity<32)throw new RangeError('32-byte readiness capacity required');const reply=await this.channel.call('event_ready',[u64(observer),0,s.output],{snapshot:32});return {status:reply.status,bytes:reply.status?new Uint8Array():reply.snapshot.slice(0,32)};}
  /** Real bounded asynchronous service admission. Completion is an observer
   * event; awaiting this transport acknowledgement does not await native RPC.
   */
  async submitService(source,observer,input=new Uint8Array()) {
    if(!(input instanceof Uint8Array))throw new TypeError('Canonical copied request required');const s=this.layout;
    const reply=await this.channel.call('service_submit',[u64(source),u64(observer),0,s.input,BigInt(input.length),s.slot],{input,slotSeed:0n});
    if(!reply.status&&!reply.slot)throw new RelayError('Missing admitted requestID');return {status:reply.status,requestId:reply.status?null:reply.slot};
  }
  /** Poll one event, copying its exact bytes before any application callback.
   * Status11 can mean empty or quota refusal; it is not evidence of delivery.
   */
  async poll(handle,capacity=this.layout.outputCapacity) {
    if(!Number.isSafeInteger(capacity)||capacity<0||capacity>this.layout.outputCapacity)
      throw new RangeError('Explicit bounded poll capacity required');
    const s=this.layout;
    const reply=await this.channel.call('event_poll',[u64(handle),0,s.output,BigInt(capacity),s.slot],
      {snapshot:capacity,slotSeed:SENTINEL});
    return this.result(reply,capacity,'event_poll');
  }
  /** Publish/log once without a response-length slot. Return unchanged errno. */
  async write(name,handle,input) {
    if(name!=='topic_publish'&&name!=='log_write') throw new RosError(Status.UNSUPPORTED,name);
    if(!(input instanceof Uint8Array)) throw new TypeError('Copied byte input required');
    const s=this.layout;
    return (await this.channel.call(name,[u64(handle),0,s.input,BigInt(input.length)],{input})).status;
  }
  /** Invoke one scalar handle operation, including subscribe/advertise/close.
   * Unsupported-version operations are refused locally before a pipe write.
   */
  async handle(name,handle) {
    const entry=specification(this.version).functions.find(f=>f.name===name);
    if(!entry||entry.parameters.length!==1) throw new RosError(Status.UNSUPPORTED,name);
    return (await this.channel.call(name,[u64(handle)])).status;
  }
  /** Acknowledge explicit transport shutdown only after reporting handle closes.
   * The caller's supervisor separately waits for actual Worker exit/cleanup.
   */
  async shutdown() { return this.channel.shutdown(); }
}

/** Exact owned handle. Local ownership is retired before one close/revoke
 * attempt because a backend failure can follow Runtime retirement. GC never
 * retries cleanup and no opaque value grants authority by itself.
 */
export class AsyncCapability {
  constructor(session,handle,pending=null) {
    if(!(session instanceof AsyncSession)) throw new TypeError('AsyncSession required');
    if(!u64(handle)&&pending!==pendingCapability)throw new TypeError('Nonzero owned handle required');
    this.session=session;Object.defineProperty(this,'handle',{value:handle,writable:true,enumerable:true,configurable:true});
    capabilityRecords.set(this,{closed:false,preparing:false,ticket:null});
    Object.defineProperty(this,'closed',{enumerable:true,get:()=>capabilityRetired(this)});
  }
  check() { if(capabilityRetired(this)) throw new RosError(Status.BAD_HANDLE,'closed capability'); if(capabilityRecord(this).preparing||ownerRetiring(this))throw new RosError(Status.CONFLICT,'active owner cleanup'); }
  async close() {return retireCapability(this,'close');}
  async revoke() {return retireCapability(this,'revoke');}
  /** Associate explicit live owners in the same persistent Guest Session. */
  async watchEvents(source,correlation=0n) {this.check();source.check();if(source.session!==this.session)throw new TypeError('Same Session required');if(ownersBusy([this,source]))throw new RosError(Status.CONFLICT,'active callback');return this.session.watchEvents(this.handle,source.handle,correlation);}
  /** Callback-held observer cannot recursively dequeue a unified event. */
  async pollEventV1(capacity) {this.check();if(ownersBusy([this]))throw new RosError(Status.CONFLICT,'active callback');return this.session.pollEventV1(this.handle,capacity);}
  /** Positive counters do not reserve native admission capacity. */
  async eventReadiness() {this.check();if(ownersBusy([this]))throw new RosError(Status.CONFLICT,'active callback');return this.session.eventReadiness(this.handle);}
  /** Submit through this source; never replay a transport timeout. */
  async submitService(observer,input=new Uint8Array()) {this.check();observer.check();if(observer.session!==this.session)throw new TypeError('Same Session required');if(ownersBusy([this,observer]))throw new RosError(Status.CONFLICT,'active callback');return this.session.submitService(this.handle,observer.handle,input);}
  async request(name,input=new Uint8Array(),capacity) {this.check();return this.session.request(name,this.handle,input,capacity);}
  async poll(capacity) {return pollCapability(this,capacity);}
}

/** Typed publisher. The caller supplies a canonical encode/decode codec;
 * the alias still fixes type, resource, QoS and quotas in trusted CLI policy.
 */
export class AsyncPublisher {
  constructor(capability,codec=StringMessage) {checkedCapability(capability);this.capability=capability;this.codec=checkedCodec(codec);}
  async publish(value) {this.capability.check();return this.capability.session.write('topic_publish',this.capability.handle,this.codec.encode(value));}
  async close() {return this.capability.close();}
}
function checkedCodec(codec) {
  if(!codec||typeof codec.encode!=='function'||typeof codec.decode!=='function') throw new TypeError('Canonical codec required');
  return codec;
}

/** AsyncExecutor-compatible unified-event entity. Factory setup awaits exactly
 * one actual watch while retaining both local owners. It never opens aliases or
 * replaces service_submit with a deferred synchronous service_call.
 */
const EVENT_SUBSCRIPTION_TOKEN=Symbol('owned unified-event watch');
export class AsyncEventSubscription {
  /** Use watch(); the private token prevents constructing an unwatched entity. */
  constructor(observer,source,callback,binding,capacity,token){
    if(token!==EVENT_SUBSCRIPTION_TOKEN)throw new TypeError('Use AsyncEventSubscription.watch');
    this.observer=observer;this.source=source;this.callback=callback;this.binding=binding;this.capacity=capacity;this.spinning=false;
  }
  /** Borrow independent live owners in one Session, then bind once. Both owners
   * stay caller-owned on failure; inspect their separate checked close results.
   */
  static async watch(observer,source,callback,correlation=0n,capacity=observer?.session.layout.outputCapacity){
    checkedCapabilities([observer,source]);checkedHandler(callback);observer.check();source.check();
    if(observer.handle===source.handle||!Number.isInteger(capacity)||capacity<64||capacity>4194304||capacity>observer.session.layout.outputCapacity)
      throw new RosError(Status.INVALID,'bounded distinct event owners');
    const binding=new EventBinding(source.session.version,source.handle,correlation),release=acquirePolling([observer,source]);
    if(!release)throw new RosError(Status.CONFLICT,'active event owners');
    try{const status=await observer.session.watchEvents(observer.handle,source.handle,correlation);
      if(status)throw new RosError(status,'event_watch');observer.check();source.check();
      return new AsyncEventSubscription(observer,source,callback,binding,capacity,EVENT_SUBSCRIPTION_TOKEN);}
    finally{release();}
  }
  /** One copied event and awaited application callback. The exact observer lease
   * stays alive across every await; recursive spin or cleanup17 cannot issue a
   * second dequeue. Rejection releases the lease and never replays native data.
   */
  async spinOnce(){
    if(this.spinning)return Status.CONFLICT;const observer=this.observer;observer.check();
    const release=acquirePolling([observer]);if(!release)return Status.CONFLICT;this.spinning=true;
    try{const callback=this.callback;checkedHandler(callback);const reply=await observer.session.pollEventV1(observer.handle,this.capacity);
      if(!reply.status)await callback.call(this,this.binding.decode(reply.bytes));return reply.status;}
    finally{release();this.spinning=false;}
  }
  /** Native bounded admission only; match its requestID in a later event. */
  async submit(bytes=new Uint8Array()){return this.source.submitService(this.observer,bytes);}
  /** Snapshot only; no admission reservation, clock or additional authority. */
  async readiness(){return this.observer.eventReadiness();}
  /** Close observer only after its callback/await lease has ended. */
  async close(){return this.spinning?Status.CONFLICT:this.observer.close();}
  /** Revoke only the observer, never an implicit source or cancel-any grant. */
  async revoke(){return this.spinning?Status.CONFLICT:this.observer.revoke();}
}

/** A subscription is constructed only after successful explicit activation.
 * Poll copies/validates bytes, releases the channel borrow, then awaits the
 * application callback. No callback pointer enters native ROS or Wasm.
 */
export class AsyncSubscription {
  constructor(capability,codec,callback) {
    checkedCapability(capability);
    if(typeof callback!=='function') throw new TypeError('Local callback required');
    this.capability=capability;this.codec=checkedCodec(codec);this.callback=callback;this.spinning=false;
  }
  /** Hold the same callback and owner through poll/decode/await. Direct nested
   * spin/close/revoke is17 before a pipe request. Mutable captured state remains
   * on this exact callable; replacement affects the next pass. finally releases
   * both the resource flag and shared retirement lease after any rejection.
   */
  async spinOnce(capacity) {
    if(this.spinning)return Status.CONFLICT;const capability=this.capability,release=acquirePolling([capability]);if(!release)return Status.CONFLICT;this.spinning=true;
    try {const callback=this.callback,codec=this.codec;checkedHandler(callback);const result=await pollCapability(capability,capacity,true);if(result.status===0)await callback.call(this,codec.decode(result.bytes),result);return result.status;}
    finally {release();this.spinning=false;}
  }
  async close() {return this.spinning?Status.CONFLICT:this.capability.close();}
  /** Explicit alias revoke only after this resource's borrowed pass has ended. */
  async revoke() {return this.spinning?Status.CONFLICT:this.capability.revoke();}
}

/** Generic asynchronous canonical resource for every seven-argument ABI entry.
 * Application codecs must match the type fixed by capability policy.
 */
export class AsyncResource {
  constructor(capability,operation) {
    checkedCapability(capability);
    const entry=specification(capability.session.version).functions.find(f=>f.name===operation);
    if(!entry||entry.parameters.length!==7) throw new RosError(Status.UNSUPPORTED,operation);
    this.capability=capability;this.operation=operation;
  }
  async call(input=new Uint8Array(),capacity) {return this.capability.request(this.operation,input,capacity);}
  async close() {return this.capability.close();}
}

/** Typed service client. A Promise describes host pipe completion; native
 * service timeouts, concurrency and authority remain Runtime policy. Failed
 * calls, including failures with an unknown native outcome, are never replayed.
 */
export class AsyncClient {
  constructor(capability,requestCodec=TriggerRequest,responseCodec=ServiceResponse) {
    checkedCapability(capability);this.capability=capability;
    this.requestCodec=checkedCodec(requestCodec);this.responseCodec=checkedCodec(responseCodec);
  }
  /** Encode once and decode only a successful copied response. */
  async call(value=null,capacity) {
    const r=await this.capability.request('service_call',this.requestCodec.encode(value),capacity);
    return {...r,value:r.status?null:this.responseCodec.decode(r.bytes)};
  }
  /** Schedule one local deferred call; the retained Worker still executes the
   * original bounded synchronous service_call when this executor advances.
   */
  callDeferred(executor,value=null,capacity) {if(!(executor instanceof AsyncExecutor))throw new TypeError('Caller-driven AsyncExecutor required');return executor.defer(()=>this.call(value,capacity));}
  async close() {return this.capability.close();}
}

/** Poll-driven service server with distinct advertise/respond grants.
 * Use AsyncNode.createService to activate before committing ownership. The
 * local handler runs after the copied poll result releases the channel borrow.
 */
export class AsyncService {
  constructor(advertise,respond,handler,requestCodec=TriggerRequest) {
    checkedCapabilities([advertise,respond]);checkedHandler(handler);
    this.advertise=advertise;this.respond=respond;this.handler=handler;
    this.requestCodec=checkedCodec(requestCodec);this.spinning=false;
  }
  /** Poll one request, await its handler and send one reply with the exact ID.
   * A throwing/rejecting handler propagates; it cannot become a native reply.
   * The original handler/owner snapshot persists across await, and every direct
   * nested spin/close/revoke refuses17. Exact-ID responder requests stay allowed.
   */
  async spinOnce(capacity) {
    if(this.spinning)return Status.CONFLICT;const advertise=this.advertise,respond=this.respond,release=acquirePolling([advertise,respond]);if(!release)return Status.CONFLICT;this.spinning=true;
    try {const handler=this.handler,codec=this.requestCodec;checkedHandler(handler);const r=await pollCapability(advertise,capacity,true);if(r.status)return r.status;const q=decodeServiceRequest(r.bytes,codec),response=await handler.call(this,q.request,q);return (await respond.request('service_respond',encodeServiceReply(q.requestId,response))).status;}
    finally {release();this.spinning=false;}
  }
  async close() {return this.spinning?[{status:Status.CONFLICT}]:closeCapabilities([this.advertise,this.respond]);}
  /** Deliberate pair revocation is separate from ordinary owner cleanup. */
  async revoke() {return this.spinning?[{status:Status.CONFLICT}]:closeCapabilities([this.advertise,this.respond],'revoke');}
}

/** Fibonacci Action client for the built-in ABI profile. Optional operations
 * need separately supplied grants. This does not claim generic native Action
 * support or cancellation of an in-flight pipe request. IDs stay BigInt.
 */
export class AsyncActionClient {
  constructor(capabilities) {
    checkedMap(capabilities,['send','cancelOwn','cancelAny','feedback','result']);
    if(!capabilities.send)throw new TypeError('Missing Action send capability');
    this.capabilities={...capabilities};
  }
  capability(key,operation) {const cap=this.capabilities[key];if(!cap)throw new RosError(Status.DENIED,operation);return cap;}
  async sendGoal(order) {
    const r=await this.capabilities.send.request('action_send_goal',FibonacciGoal.encode(order));
    return {...r,goalId:r.status?null:decodeGoalId(r.bytes)};
  }
  async cancelOwn(goalId) {return this.capability('cancelOwn','action_cancel_own_goal').request('action_cancel_own_goal',encodeGoalId(goalId));}
  /** Only cancel-any accepts zero as the explicit cancel-all selector. */
  async cancelAny(goalId) {
    const bytes=new Uint8Array(8);new DataView(bytes.buffer).setBigUint64(0,u64(goalId),true);
    return this.capability('cancelAny','action_cancel_any_goal').request('action_cancel_any_goal',bytes);
  }
  async cancelAll() {return this.cancelAny(0n);}
  async feedback(goalId) {
    const r=await this.capability('feedback','action_receive_feedback').request('action_receive_feedback',encodeGoalId(goalId));
    if(r.status)return {...r,value:null};const value=decodeActionSequence(r.bytes,false);
    return value.goalId===goalId?{...r,value}:{...r,status:Status.INVALID,runtimeStatus:r.status,value:null};
  }
  /** Require terminal2/3/4 and the requested ID. A canonical other-ID reply is
   *22/value:null with unchanged copied raw metadata, no retry and no mutation
   * of an earlier caller result. Malformed wire still throws its codec error.
   */
  async result(goalId) {
    const r=await this.capability('result','action_receive_result').request('action_receive_result',encodeGoalId(goalId));
    if(r.status)return {...r,value:null};const value=ActionResult.decode(r.bytes);
    return value.goalId===goalId?{...r,value}:{...r,status:Status.INVALID,runtimeStatus:r.status,value:null};
  }
  async close() {return closeCapabilities(Object.values(this.capabilities));}
}

/** Built-in Fibonacci server. The Node factory advertises before committing
 * ownership. Handler {kind,values} preserves the exact received nonzero ID.
 */
export class AsyncActionServer {
  constructor(advertise,respond,handler) {
    checkedCapabilities([advertise,respond]);checkedHandler(handler);
    this.advertise=advertise;this.respond=respond;this.handler=handler;this.spinning=false;
  }
  /** Hold an intrinsic guard and original callable/owners through every await.
   * Callback replacement affects the next pass; response keeps the received ID.
   * Direct poll/close/revoke refuses17 and finally resets after any rejection.
   * Separate responder requests remain permitted while the poll borrow is held.
   */
  async spinOnce(capacity) {
    if(this.spinning)return Status.CONFLICT;const advertise=this.advertise,respond=this.respond,release=acquirePolling([advertise,respond]);if(!release)return Status.CONFLICT;this.spinning=true;
    try {const handler=this.handler;checkedHandler(handler);const r=await pollCapability(advertise,capacity,true);if(r.status)return r.status;const goal=decodeIncomingGoal(r.bytes),reply=await handler.call(this,goal);return (await respond.request('action_respond',encodeActionSequence({...reply,goalId:goal.goalId}))).status;}
    finally {release();this.spinning=false;}
  }
  async close() {return this.spinning?[{status:Status.CONFLICT}]:closeCapabilities([this.advertise,this.respond]);}
  /** Explicit pair revocation after a pass; it grants no client cancel authority. */
  async revoke() {return this.spinning?[{status:Status.CONFLICT}]:closeCapabilities([this.advertise,this.respond],'revoke');}
}

/** Exact alias-selected parameter operations. No arbitrary native parameter
 * name is supplied from JS; absent optional authority opens nothing. Version
 * unsupported operations fail before the serialized request is written.
 */
export class AsyncParameters {
  constructor(capabilities,codec=StringMessage) {
    checkedMap(capabilities,['get','set','list','describe','declare','undeclare','delete','exists','search','events']);
    this.capabilities={...capabilities};this.codec=checkedCodec(codec);this.callbacks=new ParameterCallbacks();
  }
  async invoke(key,input=new Uint8Array(),codec=null) {
    const cap=this.capabilities[key];if(!cap)throw new RosError(Status.DENIED,`parameter_${key}`);
    const r=await cap.request(`parameter_${key}`,input);
    return {...r,value:r.status||!codec?null:checkedCodec(codec).decode(r.bytes)};
  }
  async get() {return this.invoke('get',new Uint8Array(),this.codec);}
  async set(value) {return this.invoke('set',this.codec.encode(value));}
  async list() {return this.invoke('list',new Uint8Array(),StringMessage);}
  async describe() {return this.invoke('describe',new Uint8Array(),StringMessage);}
  async declare(value) {return this.invoke('declare',this.codec.encode(value));}
  async undeclare() {return this.invoke('undeclare');}
  async delete() {return this.invoke('delete');}
  async exists() {return this.invoke('exists',new Uint8Array(),BoolMessage);}
  async search() {return this.invoke('search',new Uint8Array(),StringMessage);}
  async subscribeEvents() {
    const cap=this.capabilities.events;if(!cap)throw new RosError(Status.DENIED,'parameter_subscribe_events');
    cap.check();return cap.session.handle('parameter_subscribe_events',cap.handle);
  }
  /** Supported typed envelope v2 is the default. Legacy compatibility is used
   * only if an application explicitly supplies its separate event codec.
   */
  async pollEvent(codec=TypedParameterEvent,capacity) {
    const cap=this.capabilities.events;if(!cap)throw new RosError(Status.DENIED,'parameter events');
    checkedCodec(codec);const r=await cap.poll(capacity),value=r.status?null:codec.decode(r.bytes);
    if(!r.status)await this.callbacks.dispatchAsync(value);return {...r,value};
  }
  onEvent(callback) {return this.callbacks.add(callback);}
  async close() {return closeCapabilities(Object.values(this.capabilities));}
}

/** Filtered read-only graph resource; it grants no unrestricted discovery.
 * Recheck the family on each call so public operation mutation cannot turn a
 * graph wrapper into an unrelated write operation.
 */
export class AsyncGraph extends AsyncResource {
  constructor(capability,operation) {family(operation,'graph_');super(capability,operation);}
  async call(input=new Uint8Array(),capacity) {family(this.operation,'graph_');empty(input);return super.call(input,capacity);}
  async read(capacity) {
    const r=await this.call(new Uint8Array(),capacity);let rows=null;
    if(!r.status) {
      const text=StringMessage.decode(r.bytes);if(text&&!text.endsWith('\n'))throw new RangeError('Graph row terminator');
      rows=text?text.slice(0,-1).split('\n').map(line=>{
        const fields=line.split('\t');if(fields.length>2||!fields[0])throw new RangeError('Graph row fields');
        return {name:fields[0],type:fields[1]??''};
      }):[];
    }
    return {...r,rows};
  }
}

/** One explicitly selected clock; no executor scheduling reads it implicitly. */
export class AsyncClock extends AsyncResource {
  constructor(capability,operation) {family(operation,'time_read_');super(capability,operation);}
  async call(input=new Uint8Array(),capacity) {family(this.operation,'time_read_');empty(input);return super.call(input,capacity);}
  async now() {const r=await this.call();return {...r,nanoseconds:r.status?null:TimeNanoseconds.decode(r.bytes)};}
}

/** Fixed-frame TF with independent lookup/broadcast grants. Zero timestamp
 * means latest and cannot replace the policy-selected parent/child frames.
 */
export class AsyncTf {
  constructor(lookup=null,broadcast=null) {checkedCapabilities([lookup,broadcast].filter(Boolean));this.lookupCapability=lookup;this.broadcastCapability=broadcast;}
  async lookup(nanoseconds=0n) {
    if(!this.lookupCapability)throw new RosError(Status.DENIED,'tf_lookup');
    const r=await this.lookupCapability.request('tf_lookup',encodeLookupTime(nanoseconds));
    return {...r,value:r.status?null:transformMessage(this.lookupCapability.session.version).decode(r.bytes)};
  }
  async broadcast(value) {
    if(!this.broadcastCapability)throw new RosError(Status.DENIED,'tf_broadcast');
    return this.broadcastCapability.request('tf_broadcast',transformMessage(this.broadcastCapability.session.version).encode(value));
  }
  async close() {return closeCapabilities([this.lookupCapability,this.broadcastCapability]);}
}

/** ROS2 exact-node lifecycle. Allowed transition and resource names live in
 * policy; changeState deliberately sends an empty canonical request.
 */
export class AsyncLifecycle {
  constructor(getState=null,changeState=null) {
    const caps=[getState,changeState].filter(Boolean);checkedCapabilities(caps);
    if(caps.some(cap=>cap.session.version!==2))throw new RosError(Status.UNSUPPORTED,'lifecycle');
    this.getCapability=getState;this.changeCapability=changeState;
  }
  async getState() {
    if(!this.getCapability)throw new RosError(Status.DENIED,'lifecycle_get_state');
    const r=await this.getCapability.request('lifecycle_get_state');return {...r,value:r.status?null:LifecycleState.decode(r.bytes)};
  }
  async changeState() {
    if(!this.changeCapability)throw new RosError(Status.DENIED,'lifecycle_change_state');
    const r=await this.changeCapability.request('lifecycle_change_state');return {...r,accepted:r.status?null:BoolMessage.decode(r.bytes)};
  }
  async close() {return closeCapabilities([this.getCapability,this.changeCapability]);}
}

/** Native log.write is an explicit capability, separate from Node console I/O.
 * Policy fixes severity and sink; no other logger authority is inferred.
 */
export class AsyncLogger {
  constructor(capability) {checkedCapability(capability);this.capability=capability;}
  async write(text) {this.capability.check();return this.capability.session.write('log_write',this.capability.handle,StringMessage.encode(text));}
  async close() {return this.capability.close();}
}

function checkedCapability(cap) {if(!(cap instanceof AsyncCapability))throw new TypeError('Owned AsyncCapability required');return cap;}
function checkedCapabilities(caps) {
  for(const cap of caps)checkedCapability(cap);
  if(caps.length&&caps.some(cap=>cap.session!==caps[0].session))throw new TypeError('Capabilities must belong to one AsyncSession');
}
function checkedMap(map,keys) {
  if(!map||typeof map!=='object'||Array.isArray(map))throw new TypeError('Explicit capability map required');
  for(const key of Object.keys(map))if(!keys.includes(key))throw new TypeError('Unknown capability operation');
  checkedCapabilities(Object.values(map));
}
function checkedHandler(handler) {if(typeof handler!=='function')throw new TypeError('Local handler required');}
function family(operation,prefix) {if(typeof operation!=='string'||!operation.startsWith(prefix))throw new RosError(Status.INVALID,operation);}
function empty(input) {if(!(input instanceof Uint8Array)||input.length)throw new RosError(Status.INVALID,'empty read request');}
/** Attempt every distinct owned close once even if another close throws.
 * Returned failures must be inspected; they never become successful cleanup.
 */
async function closeCapabilities(capabilities,operation='close',ledger=null) {
  const owners=distinctOwners(capabilities).filter(cap=>!capabilityRetired(cap));
  // Allocate every result/diagnostic slot and retain full handles before entry.
  const results=owners.map(cap=>({handle:cap.handle,status:undefined,error:undefined}));
  const attempts=owners.map(cap=>({handle:cap.handle,status:null,error:null,threw:false,attempted:false,retained:true}));
  Object.defineProperty(results,'attempts',{value:attempts});
  if(ledger)ledger.cleanupAttempts=attempts; // Retain diagnostics before any close attempt.
  const release=acquireRetirement(owners);
  if(!release){const conflict=[{status:Status.CONFLICT}];Object.defineProperty(conflict,'attempts',{value:attempts});return conflict;}
  try {
    for(let i=0;i<owners.length;++i) {
      try {const status=await retireCapability(owners[i],operation,true);results[i].status=status;delete results[i].error;attempts[i].status=status;}
      catch(error){results[i].error=error;delete results[i].status;attempts[i].error=error;attempts[i].threw=true;}
      finally {attempts[i].attempted=capabilityRetired(owners[i]);attempts[i].retained=!attempts[i].attempted;}
    }
  } finally {release();}
  return results;
}
/** Only the captured channel's actual native/write delegate attempt retires the
 * owner. All known local refusals preserve it for a later explicit cleanup.
 * The ticket proves a JavaScript delegate attempt, never a Runtime import.
 */
async function retireCapability(cap,operation,grouped=false) {
  const record=capabilityRecord(cap);
  if(capabilityRetired(cap))throw new RosError(Status.BAD_HANDLE,'closed capability');
  if(record.preparing||ownerPolling(cap)||(!grouped&&ownerRetiring(cap)))return Status.CONFLICT;
  record.preparing=true;let ticket=null;
  try {
    const session=cap.session,name=operation==='revoke'?'capability_revoke':'capability_close';
    const entry=specification(session.version).functions.find(value=>value.name===name);
    if(!entry||entry.parameters.length!==1)throw new RosError(Status.UNSUPPORTED,name);
    const handle=u64(cap.handle),channel=session.channel,target=channel.call;
    ticket=prepareCallEntry(target,[name,[handle]],channel);record.ticket=ticket;
    return (await invokeCallEntry(ticket)).status;
  } finally {
    if(ticket!==null&&callEntryAttempted(ticket))record.closed=true;
    record.ticket=null;record.preparing=false;
  }
}
/** Refuse public recursive owner polling; the typed dispatcher alone enters
 * with its retained lease. A raw Session.poll(handle) is a caller lifetime
 * escape and does not inherit the typed dispatcher's safety guarantee.
 */
async function pollCapability(cap,capacity,leased=false) {
  cap.check();if(ownerPolling(cap)&&!leased)throw new RosError(Status.CONFLICT,'active resource polling');
  return cap.session.poll(cap.handle,capacity);
}

/** Bounded local executor. Registration changes take effect next outer pass;
 * nested spin is a local conflict, not another native operation. Each callback
 * completes before the next entity is polled. No hidden timer/clock is created.
 */
export class AsyncExecutor {
  constructor(options={}) {this.entities=[];this.spinning=false;this.closed=false;this.tasks=new DeferredQueue(options);}
  add(entity) {if(this.closed)throw new RosError(Status.BAD_HANDLE,'closed Executor');if(typeof entity?.spinOnce!=='function') throw new TypeError('Polling entity required');this.entities.push(entity);return entity;}
  async spinOnce() {
    if(this.closed)return [Status.BAD_HANDLE];if(this.spinning) return [Status.CONFLICT];
    this.spinning=true;
    const pending=this.tasks.snapshot();
    try {const snapshot=this.entities.slice(),results=[];for(const entity of snapshot){if(this.closed)break;results.push(await entity.spinOnce());}await this.tasks.runAsync(pending);return results;}
    catch(error) {for(const {future} of pending)if(future.start())future.reject(error);throw error;}
    finally {this.tasks.finishSnapshot();this.spinning=false;}
  }
  defer(callback) {return this.tasks.defer(callback);}
  close() {this.closed=true;this.tasks.close();this.entities=[];}
}

/** Local Node organizer; names cannot change the Runtime principal. Factories
 * await every open/activation and commit only after construction succeeds.
 * On failure each acquired handle is closed exactly once; preserve the first
 * failure, attaching secondary cleanup failures instead of concealing them.
 */
export class AsyncNode {
  constructor(session,name='roswasm') {
    if(!(session instanceof AsyncSession)) throw new TypeError('AsyncSession required');
    if(typeof name!=='string')throw new TypeError('Local Node name required');
    this.session=session;this.name=name;this.owned=[];this.busy=false;this.closed=false;this.context=null;nodeRecords.set(this,{ledgers:[],lastFailure:null});
  }
  /** Refuse overlapping factory/close lifetimes before opening anything.
   * Serialization is explicit; the SDK creates no hidden unbounded work queue.
   */
  check(closing=false) {
    if(this.closed||(!closing&&this.context?.closed))throw new RosError(Status.BAD_HANDLE,'closed Node/Context');
    if(this.busy)throw new RosError(Status.CONFLICT,'Node ownership transaction');
  }
  async open(alias) {return this.transaction([alias],caps=>caps[0]);}
  async transaction(aliases,construct) {
    this.check();
    if(!Array.isArray(aliases)||typeof construct!=='function')throw new TypeError('Explicit alias transaction required');
    const selected=Array.from(aliases);
    for(const alias of selected)if(typeof alias!=='string'||!alias.length||alias.includes('\0'))throw new TypeError('Nonempty NUL-free alias required');
    const record=nodeRecords.get(this),ledger={owners:new Array(selected.length).fill(null),committed:false,primary:undefined,cleanupAttempts:null};
    // Persist this preallocated ledger before an awaited open can acquire.
    record.ledgers.push(ledger);this.busy=true;
    try {
      for(let i=0;i<selected.length;++i)ledger.owners[i]=await this.session.open(selected[i]);
      const result=await construct(ledger.owners.slice());
      this.owned.push(...ledger.owners.filter(cap=>!capabilityRetired(cap)));ledger.committed=true;return result;
    } catch(error) {
      ledger.primary=error;record.lastFailure=ledger;let cleanup;
      try {cleanup=await closeCapabilities(ledger.owners,'close',ledger);}
      catch(failure){cleanup=[{error:failure}];}
      if(error&&typeof error==='object') {
        try {
          Object.defineProperty(error,'cleanupAttempts',{value:cleanup.attempts||[],configurable:true});
          const failures=cleanup.filter(row=>Object.prototype.hasOwnProperty.call(row,'error')||row.status);
          if(failures.length)Object.defineProperty(error,'cleanupFailures',{value:Object.freeze(failures),configurable:true});
        } catch { /* The private Node ledger survives even a frozen primary. */ }
      }
      throw error;
    } finally {this.busy=false;pruneNodeLedgers(record);}
  }
  async createPublisher(alias,codec=StringMessage) {checkedCodec(codec);return this.transaction([alias],caps=>new AsyncPublisher(caps[0],codec));}
  async createSubscription(alias,codec=StringMessage,callback=()=>{}) {
    checkedCodec(codec);checkedHandler(callback);
    return this.transaction([alias],async caps=>{
      const status=await this.session.handle('topic_subscribe',caps[0].handle);
      if(status) throw new RosError(status,'topic_subscribe');
      return new AsyncSubscription(caps[0],codec,callback);
    });
  }
  async createResource(alias,operation) {
    this.validateOperation(operation);
    return this.transaction([alias],caps=>new AsyncResource(caps[0],operation));
  }
  /** Validate a version-selected seven-argument ABI operation before alias open. */
  validateOperation(operation) {
    const entry=specification(this.session.version).functions.find(f=>f.name===operation);
    if(!entry||entry.parameters.length!==7)throw new RosError(Status.UNSUPPORTED,operation);
  }
  async createClient(alias,requestCodec=TriggerRequest,responseCodec=ServiceResponse) {
    checkedCodec(requestCodec);checkedCodec(responseCodec);
    return this.transaction([alias],caps=>new AsyncClient(caps[0],requestCodec,responseCodec));
  }
  async createService(advertiseAlias,respondAlias,handler,requestCodec=TriggerRequest) {
    checkedHandler(handler);checkedCodec(requestCodec);
    return this.transaction([advertiseAlias,respondAlias],async caps=>{
      const service=new AsyncService(caps[0],caps[1],handler,requestCodec);
      const status=await this.session.handle('service_advertise',caps[0].handle);
      if(status)throw new RosError(status,'service_advertise');return service;
    });
  }
  async createActionClient(aliases) {
    const keys=aliasKeys(aliases,['send','cancelOwn','cancelAny','feedback','result']);
    if(!keys.includes('send'))throw new TypeError('Missing Action send alias');
    return this.transaction(keys.map(key=>aliases[key]),caps=>new AsyncActionClient(zip(keys,caps)));
  }
  async createActionServer(advertiseAlias,respondAlias,handler) {
    checkedHandler(handler);
    return this.transaction([advertiseAlias,respondAlias],async caps=>{
      const server=new AsyncActionServer(caps[0],caps[1],handler),r=await caps[0].request('action_advertise');
      if(r.status)throw new RosError(r.status,'action_advertise');return server;
    });
  }
  async createParameters(aliases,codec=StringMessage) {
    checkedCodec(codec);const keys=aliasKeys(aliases,['get','set','list','describe','declare','undeclare','delete','exists','search','events']);
    for(const key of keys) {
      const operation=key==='events'?'parameter_subscribe_events':`parameter_${key}`;
      if(!specification(this.session.version).functions.some(f=>f.name===operation))throw new RosError(Status.UNSUPPORTED,operation);
    }
    return this.transaction(keys.map(key=>aliases[key]),caps=>new AsyncParameters(zip(keys,caps),codec));
  }
  async createGraph(alias,operation) {
    family(operation,'graph_');this.validateOperation(operation);
    return this.transaction([alias],caps=>new AsyncGraph(caps[0],operation));
  }
  async createClock(alias,operation) {
    family(operation,'time_read_');this.validateOperation(operation);
    return this.transaction([alias],caps=>new AsyncClock(caps[0],operation));
  }
  async createTf(lookupAlias=null,broadcastAlias=null) {
    const aliases=[lookupAlias,broadcastAlias].filter(alias=>alias!==null);
    return this.transaction(aliases,caps=>new AsyncTf(lookupAlias!==null?caps[0]:null,broadcastAlias!==null?caps[lookupAlias!==null?1:0]:null));
  }
  async createLifecycle(getAlias=null,changeAlias=null) {
    if(this.session.version!==2)throw new RosError(Status.UNSUPPORTED,'lifecycle');
    const aliases=[getAlias,changeAlias].filter(alias=>alias!==null);
    return this.transaction(aliases,caps=>new AsyncLifecycle(getAlias!==null?caps[0]:null,changeAlias!==null?caps[getAlias!==null?1:0]:null));
  }
  async createLogger(alias) {return this.transaction([alias],caps=>new AsyncLogger(caps[0]));}
  /** Close every distinct still-owned handle even when another close throws.
   * A result item records a returned status or an exception. The application
   * must inspect all items; this method does not turn partial cleanup into PASS.
   */
  async close() {
    this.check(true);const owners=nodeOwners(this);if(ownersBusy(owners))throw new RosError(Status.CONFLICT,'active resource ownership');this.closed=true;this.busy=true;
    try {return await closeCapabilities(owners);}
    finally {this.closed=owners.every(cap=>capabilityRetired(cap));this.busy=false;pruneNodeLedgers(nodeRecords.get(this));}
  }
  /** Snapshot retained owners. Nonenumerable factory preserves the latest failed
   * factory primary/attempts, including a frozen or primitive primary. */
  pendingCleanup(){
    const pending=nodeOwners(this).filter(cap=>!capabilityRetired(cap)).map(cap=>({handle:cap.handle,attempted:false}));
    const failed=nodeRecords.get(this).lastFailure;
    Object.defineProperty(pending,'factory',{value:failed?{primary:failed.primary,attempts:(failed.cleanupAttempts||[]).map(row=>({...row}))}:null});
    return pending;
  }
}

/** Local ownership scope for a qualified retained relay Session. It opens no
 * alias and creates no native ROS context. Transport shutdown is separately
 * explicit: shutdown({transport:true}) closes handles, then requests the relay
 * acknowledgement; the supervisor still checks actual Worker exit/cleanup.
 */
export class AsyncContext {
  constructor(session) {if(!(session instanceof AsyncSession))throw new TypeError('Explicit AsyncSession required');this.session=session;this.nodes=[];this.closed=false;}
  createNode(name='roswasm') {
    if(this.closed)throw new RosError(Status.BAD_HANDLE,'closed Context');
    const node=new AsyncNode(this.session,name);node.context=this;this.nodes.push(node);return node;
  }
  async shutdown({transport=false}={}) {
    if(typeof transport!=='boolean')throw new TypeError('Explicit transport shutdown choice required');
    if(this.closed)throw new RosError(Status.BAD_HANDLE,'closed Context');
    if(this.nodes.some(node=>node.busy||ownersBusy(nodeOwners(node))))throw new RosError(Status.CONFLICT,'Active Node/resource ownership transaction');
    const nodes=[];this.closed=true;
    try {for(const node of this.nodes)if(!node.closed) {
      try {nodes.push({name:node.name,results:await node.close()});}
      catch(error) {nodes.push({name:node.name,error});}
    }}finally{this.closed=!this.nodes.some(nodeHasLiveOwners);}
    const retained=!this.closed;
    let transportResult=null;
    if(transport&&retained)transportResult={status:Status.CONFLICT,cleanupPending:true};
    else if(transport) {
      try {transportResult=await this.session.shutdown();}
      catch(error) {transportResult={error};}
    }
    return {nodes,transport:transportResult};
  }
}
function aliasKeys(aliases,allowed) {
  if(!aliases||typeof aliases!=='object'||Array.isArray(aliases))throw new TypeError('Explicit alias map required');
  const keys=Object.keys(aliases);
  for(const key of keys)if(!allowed.includes(key))throw new TypeError('Unknown alias operation');
  return keys;
}
function zip(keys,caps) {return Object.fromEntries(keys.map((key,index)=>[key,caps[index]]));}
export { StringMessage, RosError, Status };

/** Same additive event codecs for copied asynchronous results. */
export {EventKind,decodeUnifiedEvent,decodeEventReadiness,EventBinding} from "./events.mjs";
