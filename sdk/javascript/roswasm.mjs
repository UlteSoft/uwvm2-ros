import { EventBinding } from './events.mjs';
/** Poll-driven resources over a real bridge or same-Wasm copied C transport.
 * This API resembles common ROS client-library concepts while retaining ABI v1's
 * capability model. A Node is a guest-side organizer, not permission to create
 * arbitrary native nodes, topics, services, sockets or timers.
 */
import { Status, createRawAdapter, memoryRange, readU64, specification, u64, prepareCallEntry, invokeCallEntry, callEntryAttempted } from './raw.mjs';
/** Share the same immutable ABI statuses through the high-level and embedded
 * guest entrypoints. Exporting constants acquires no Runtime capability.
 */
export { Status };
import { StringMessage, BoolMessage, TwistMessage, ServiceResponse, TriggerRequest,
  decodeServiceRequest, encodeServiceReply, FibonacciGoal, encodeGoalId,
  decodeGoalId, decodeActionSequence, decodeIncomingGoal, encodeActionSequence,
  TransformMessage, transformMessage, encodeLookupTime, LifecycleState, TimeNanoseconds, ActionResult } from './codecs.mjs';
const encoder = new TextEncoder(), SENTINEL = 0xffffffffffffffffn;
const pendingCapability = Symbol('pending SDK capability');
const capabilityRecords = new WeakMap(), nodeRecords = new WeakMap();
function capabilityRecord(cap) { const record=capabilityRecords.get(cap);if(!record)throw new TypeError('Actual SDK capability required');return record; }
function capabilityRetired(cap) { const record=capabilityRecord(cap);return record.closed||(record.ticket!==null&&callEntryAttempted(record.ticket)); }
function nodeOwners(node) { const record=nodeRecords.get(node),owners=node.owned.slice();for(const ledger of record.ledgers)for(const cap of ledger.owners)if(cap)owners.push(cap);return distinctOwners(owners); }
function pruneNodeLedgers(record) { let at=0;for(const ledger of record.ledgers)if(ledger.owners.some(cap=>cap&&!capabilityRetired(cap)))record.ledgers[at++]=ledger;record.ledgers.length=at; }
function nodeHasLiveOwners(node){for(const cap of node.owned)if(cap&&!capabilityRetired(cap))return true;for(const ledger of nodeRecords.get(node).ledgers)for(const cap of ledger.owners)if(cap&&!capabilityRetired(cap))return true;return false;}

import { DeferredQueue, ParameterCallbacks } from './scheduling.mjs';
import { CopiedTransport } from './copied_transport.mjs';
import { distinctOwners, ownerPolling, ownerRetiring, ownersBusy, acquirePolling, acquireRetirement } from './ownership.mjs';
export { Future, CallbackGroup, Timer, SchedulingError, ParameterCallbacks } from './scheduling.mjs';

/** Attempt each owned cleanup once while preserving the original factory failure.
 * A throwing bridge is a failure to report separately, not permission to reuse
 * retired ownership or skip cleanup of another acquired handle.
 */
function rollback(capabilities, original = null, ledger = null) {
  let failures=[],attempts=[];
  try { const results=closeOwned(capabilities,'close',ledger);attempts=results.attempts||[];failures=results.filter(value=>value instanceof Error||value!==0); }
  catch(error) {failures=[error];attempts=error?.attempts||[];}
  if(original&&typeof original==='object') {
    try {
      Object.defineProperty(original,'cleanupAttempts',{value:attempts,configurable:true});
      if(failures.length)Object.defineProperty(original,'cleanupFailures',{value:Object.freeze(failures),configurable:true});
    } catch { /* A frozen primary does not discard the Node's retained ledger. */ }
  }
}
/** Preallocate complete IDs/results before any retirement. The ordinary status
 * array remains compatible; nonenumerable attempts distinguishes retained owners
 * from delegate attempts without claiming a Host import or granting authority.
 */
function closeOwned(capabilities, operation = 'close', ledger = null) {
  const owners=distinctOwners(capabilities).filter(cap=>!capabilityRetired(cap));
  const results=new Array(owners.length).fill(null),attempts=owners.map(cap=>({handle:cap.handle,status:null,error:null,threw:false,attempted:false,retained:true}));
  Object.defineProperty(results,'attempts',{value:attempts});
  if(ledger)ledger.cleanupAttempts=attempts; // Retain diagnostics before any close attempt.
  const release=acquireRetirement(owners);
  if(!release){const conflict=[Status.CONFLICT];Object.defineProperty(conflict,'attempts',{value:attempts});return conflict;}
  try {
    for(let i=0;i<owners.length;++i) {
      try {results[i]=retireCapability(owners[i],operation,true);attempts[i].status=results[i];}
      catch(error){results[i]=error;attempts[i].error=error;attempts[i].threw=true;}
      finally {attempts[i].attempted=capabilityRetired(owners[i]);attempts[i].retained=!attempts[i].attempted;}
    }
  } finally {release();}
  const failures=attempts.filter(row=>row.threw).map(row=>row.error);
  if(failures.length){const error=new AggregateError(failures,'Owned capability cleanup failed');error.results=results;error.attempts=attempts;throw error;}
  return results;
}
/** Local preflight failures preserve ownership. The private ticket becomes
 * sticky exactly at the captured raw/native delegate attempt, including throw.
 * A public closed=false assignment cannot revive that SDK lifetime record.
 */
function retireCapability(cap, operation, grouped = false) {
  const record=capabilityRecord(cap);
  if(capabilityRetired(cap))throw new RosError(Status.BAD_HANDLE,'closed capability');
  if(record.preparing||ownerPolling(cap)||(!grouped&&ownerRetiring(cap)))return Status.CONFLICT;
  record.preparing=true;let ticket=null;
  try {
    const session=cap.session,handle=u64(cap.handle),raw=session.raw;
    const target=raw[operation==='revoke'?'capability_revoke':'capability_close'];
    ticket=prepareCallEntry(target,[handle],raw);record.ticket=ticket;
    return invokeCallEntry(ticket);
  } finally {
    if(ticket!==null&&callEntryAttempted(ticket))record.closed=true;
    record.ticket=null;record.preparing=false;
  }
}
function revokeOwned(capabilities) { return closeOwned(capabilities, 'revoke'); }
/** Public owner polling cannot consume another event during a typed pass.
 * The private typed dispatcher already holds this owner's polling lease.
 * Requests are deliberately separate, permitting the exact server responder.
 */
function pollCapability(cap, leased = false) {
  cap.check(); if (ownerPolling(cap) && !leased) throw new RosError(Status.CONFLICT, 'active resource polling');
  return cap.session.poll(cap.handle);
}
function checkedCodec(codec) { if (!codec || typeof codec.encode !== 'function' || typeof codec.decode !== 'function') throw new TypeError('Canonical encode/decode codec required'); return codec; }
function checkedHandler(handler) { if (typeof handler !== 'function') throw new TypeError('Local callback/handler required'); }
function checkedCapabilities(capabilities) {
  for (const cap of capabilities) if (!(cap instanceof Capability)) throw new TypeError('Owned Capability required');
  if (capabilities.length && capabilities.some(cap => cap.session !== capabilities[0].session)) throw new TypeError('Capabilities must belong to one Session');
}

/** Runtime error carrying an unchanged ABI status code. It never retries a failed operation. */
export class RosError extends Error {
  /** @param {number} code Runtime status. @param {string} operation Failed ABI operation. */
  constructor(code, operation) { super(`${operation} failed with ROS ABI status ${code}`); this.name = 'RosError'; this.code = code; this.operation = operation; }
}

/** Memory owner for a caller-reserved bridge scratch range.
 * The embedder reserves this region against the guest allocator. This module
 * cannot prove that reservation: never pass arbitrary heap addresses or borrowed
 * memory. Calls are synchronous and serialized; callbacks run after the copy.
 */
class MemoryStorage {
  constructor(version, bridge, scratch) {
    this.functions = createRawAdapter(version, bridge);
    for (const key of ['inputCapacity', 'outputCapacity'])
      if (!Number.isSafeInteger(scratch[key]) || scratch[key] < 0) throw new RangeError('Invalid scratch capacity');
    this.input = u64(scratch.offset); this.inputCapacity = scratch.inputCapacity;
    this.slot = this.input + BigInt(this.inputCapacity); this.output = this.slot + 8n;
    this.outputCapacity = scratch.outputCapacity;
    memoryRange(this.functions.memory, this.input, BigInt(this.inputCapacity) + 8n + BigInt(this.outputCapacity));
  }
  begin(bytes) { memoryRange(this.functions.memory, this.input, BigInt(bytes.length)).set(bytes); }
  end() {}
  seed(value) { const slot = memoryRange(this.functions.memory, this.slot, 8n); new DataView(slot.buffer, slot.byteOffset, 8).setBigUint64(0, value, true); }
  readSlot() { return readU64(this.functions.memory, this.slot); }
  copyOutput(length) { return memoryRange(this.functions.memory, this.output, length).slice(); }
}

export class Session {
  /** @param {number} version ROS 1 or 2.
   * @param {object} bridge Trusted createRawAdapter bridge or explicit CopiedTransport.
   * @param {{offset: bigint, inputCapacity: number, outputCapacity: number}} scratch
   * Exclusive bridge layout: input, eight-byte slot, output; no automatic growth.
   * Omit scratch for CopiedTransport, whose exact native layout is checked.
   */
  constructor(version, bridge, scratch) {
    // The host branch retains its genuine WebAssembly.Memory validation. The
    // explicit copied branch owns native guest scratch and supplies no pretend
    // Memory/Instance object. Both branches share every resource/codec semantic.
    if (bridge instanceof CopiedTransport && (bridge.version !== version || scratch !== undefined))
      throw new TypeError('Copied transport requires its exact backend and own scratch');
    this.storage = bridge instanceof CopiedTransport ? bridge : new MemoryStorage(version, bridge, scratch);
    this.raw = this.storage.functions; this.version = version; this.busy = false;
    for (const key of ['input', 'slot', 'output', 'inputCapacity', 'outputCapacity']) this[key] = this.storage[key];
    if (this.raw.abi_version() !== 1) throw new RosError(Status.UNSUPPORTED, 'abi_version');
  }
  /** Run a single memory-using import, releasing the borrow even if the bridge throws. */
  withScratch(bytes, operation) {
    if (this.busy) throw new Error('Reentrant scratch use is forbidden');
    if (!(bytes instanceof Uint8Array) || bytes.length > this.inputCapacity) throw new RangeError('Input exceeds reserved scratch');
    this.busy = true;
    let borrowed = false;
    try {
      this.storage.begin(bytes);
      borrowed = true;
      this.storage.seed(SENTINEL);
      return operation();
    } finally { if (borrowed) this.storage.end(); this.busy = false; }
  }
  /** Open exactly one configured UTF-8 alias. Empty/NUL aliases are rejected locally.
   * Code 13 under the default policy is expected; SDK imports grant no authority.
   * @returns {Capability} Owned handle. Close it explicitly; GC does not call imports.
   */
  open(alias) {
    if (typeof alias !== 'string' || !alias.length || alias.includes('\0')) throw new TypeError('Nonempty UTF-8 alias required');
    const bytes = StringMessage.encode(alias).subarray(4);
    // Allocate the real owner/closed record before capability_open can acquire.
    const owner = new Capability(this, 0n, pendingCapability);
    return this.withScratch(bytes, () => {
      this.storage.seed(0n);
      const status = this.raw.capability_open(0, this.input, BigInt(bytes.length), this.slot);
      if (status) throw new RosError(status, 'capability_open');
      const handle = this.storage.readSlot();
      if (!handle) throw new Error('Bridge did not write a valid capability handle');
      owner.handle = handle; return owner;
    });
  }
  /** Invoke a seven-argument request/response import once.
   * Output lengths are meaningful on success, or on code 90 only when the slot
   * was actually overwritten. Code 90 may otherwise be an admission rejection.
   * The response/event may already be consumed; this function never replays it.
   * @returns {{status: number, bytes: Uint8Array, required: bigint|null}} Copied result.
   */
  request(name, handle, bytes = new Uint8Array()) {
    const entry = specification(this.version).functions.find(f => f.name === name);
    if (!entry || entry.parameters.length !== 7) throw new RosError(Status.UNSUPPORTED, name);
    return this.withScratch(bytes, () => this.result(this.raw[name](u64(handle), 0, this.input,
      BigInt(bytes.length), this.output, BigInt(this.outputCapacity), this.slot), name));
  }
  /** Bind separately granted observer/source owners. No legacy consumer migration. */
  watchEvents(observer,source,correlation=0n) {return this.raw.event_watch(u64(observer),u64(source),u64(correlation));}
  /** Capacity-checked event90 retains the record. It never grows/retries implicitly. */
  pollEventV1(observer) {return this.withScratch(new Uint8Array(),()=>this.result(this.raw.event_poll_v1(u64(observer),0,this.output,BigInt(this.outputCapacity),this.slot),'event_poll_v1'));}
  /** Copied32-byte readiness snapshot; no output-length slot is implied. */
  eventReadiness(observer) {return this.withScratch(new Uint8Array(),()=>{if(this.outputCapacity<32)throw new RangeError('32-byte readiness capacity required');const status=this.raw.event_ready(u64(observer),0,this.output);return {status,bytes:status?new Uint8Array():this.storage.copyOutput(32n)};});}
  /** Submit once. A successful acknowledgement returns an owned requestID and
   * means admission only. No synchronous RPC is disguised as asynchronous work.
   */
  submitService(source,observer,bytes=new Uint8Array()) {
    return this.withScratch(bytes,()=>{this.storage.seed(0n);const status=this.raw.service_submit(u64(source),u64(observer),0,this.input,BigInt(bytes.length),this.slot),requestId=status?null:this.storage.readSlot();if(!status&&!requestId)throw new Error('Missing admitted requestID');return {status,requestId};});
  }
  /** Poll exactly one event using the same bounded output storage. */
  poll(handle) {
    return this.withScratch(new Uint8Array(), () => this.result(this.raw.event_poll(u64(handle), 0,
      this.output, BigInt(this.outputCapacity), this.slot), 'event_poll'));
  }
  /** Interpret an output slot only under the documented status rules. */
  result(status, operation) {
    const written = this.storage.readSlot();
    const required = (status === 0 || status === Status.TOO_LARGE) && written !== SENTINEL ? written : null;
    if (!status && (required === null || required > BigInt(this.outputCapacity)))
      throw new Error(`${operation}: invalid successful output length`);
    return { status, required, bytes: status ? new Uint8Array() : this.storage.copyOutput(required) };
  }
  /** Copy a publish/log payload once; no length output is expected. */
  write(name, handle, bytes) {
    if (name !== 'topic_publish' && name !== 'log_write') throw new RosError(Status.UNSUPPORTED, name);
    return this.withScratch(bytes, () => this.raw[name](u64(handle), 0, this.input, BigInt(bytes.length)));
  }
}

/** Explicitly owned capability. Handle values never identify resources without runtime authorization. */
export class Capability {
  /** Internal constructor; normally obtain ownership from Session.open(alias). */
  constructor(session, handle, pending = null) {
    if (!(session instanceof Session) || (!u64(handle) && pending !== pendingCapability)) throw new TypeError('Session and nonzero owned handle required');
    this.session=session;Object.defineProperty(this,'handle',{value:handle,writable:true,enumerable:true,configurable:true});
    const record={closed:false,preparing:false,ticket:null};capabilityRecords.set(this,record);
    // closed is a read-only diagnostic mirror. session/handle retain their
    // existing writable API; neither this record nor that API authorizes Host I/O.
    Object.defineProperty(this,'closed',{enumerable:true,get:()=>capabilityRetired(this)});
  }
  /** Guard against use after this wrapper has released ownership. */
  check() { if (capabilityRetired(this)) throw new RosError(Status.BAD_HANDLE, 'closed capability'); if (capabilityRecord(this).preparing || ownerRetiring(this)) throw new RosError(Status.CONFLICT, 'active owner cleanup'); }
  /** Release ownership after one close attempt, including backend failure.
   * The runtime can retire a handle before returning 107/110; never blindly retry close.
   */
  close() { return retireCapability(this, 'close'); }
  /** Revoke using current authority and release local ownership after one attempt.
   * Runtime revocation retires alias handles; even a backend failure is not proof of liveness.
   */
  revoke() { return retireCapability(this, 'revoke'); }
  /** Invoke an exact request import through this handle and return its status/copy. */
  request(name, bytes = new Uint8Array()) { this.check(); return this.session.request(name, this.handle, bytes); }
  /** Associate this explicit observer with a source owner in the same Session. */
  watchEvents(source,correlation=0n) {this.check();source.check();if(source.session!==this.session)throw new TypeError('Same Session required');if(ownersBusy([this,source]))throw new RosError(Status.CONFLICT,'active callback');return this.session.watchEvents(this.handle,source.handle,correlation);}
  /** Poll copied unified bytes; a callback-held owner refuses17 before dequeue. */
  pollEventV1() {this.check();if(ownersBusy([this]))throw new RosError(Status.CONFLICT,'active callback');return this.session.pollEventV1(this.handle);}
  /** Read bounded credit without creating/reserving capacity. */
  eventReadiness() {this.check();if(ownersBusy([this]))throw new RosError(Status.CONFLICT,'active callback');return this.session.eventReadiness(this.handle);}
  /** Admit a canonical service request through this source and an explicit observer. */
  submitService(observer,bytes=new Uint8Array()) {this.check();observer.check();if(observer.session!==this.session)throw new TypeError('Same Session required');if(ownersBusy([this,observer]))throw new RosError(Status.CONFLICT,'active callback');return this.session.submitService(this.handle,observer.handle,bytes);}
  /** Poll an event once. Code 11 does not by itself distinguish an empty queue from rate limiting. */
  poll() { return pollCapability(this); }
}

/** Typed publisher; the alias still fixes the native type and QoS in CLI policy. */
export class Publisher {
  /** @param {Capability} capability Explicit topic.publish handle. @param {object} codec Canonical encode/decode object. */
  constructor(capability, codec = StringMessage) { checkedCapabilities([capability]); this.capability = capability; this.codec = checkedCodec(codec); }
  /** Encode and copy one value. Returns the runtime status without retry. */
  publish(value) { this.capability.check(); return this.capability.session.write('topic_publish', this.capability.handle, this.codec.encode(value)); }
  /** Close once; caller remains responsible for checking returned backend status. */
  close() { return this.capability.close(); }
}

/** Unified-event polling entity accepted by Executor.add. Owners are borrowed,
 * already open and independently granted. One explicit watch is attempted;
 * failure leaves both owners with the caller for checked cleanup. No legacy
 * subscription factory, new alias, native thread or synchronous RPC is invoked.
 */
export class EventSubscription {
  constructor(observer,source,callback,correlation=0n) {
    checkedCapabilities([observer,source]);checkedHandler(callback);observer.check();source.check();
    if(observer.handle===source.handle)throw new RosError(Status.INVALID,'distinct event owners');
    const binding=new EventBinding(source.session.version,source.handle,correlation);
    const status=observer.watchEvents(source,correlation);if(status)throw new RosError(status,'event_watch');
    this.observer=observer;this.source=source;this.callback=callback;this.binding=binding;this.spinning=false;
  }
  /** Copy/decode/dispatch once. The shared observer lease spans the callback,
   * refusing direct/Node/Context retirement and nested polling before imports.
   * The body is already an owning copy. Binding commits before user code;
   * handler exceptions propagate after lease release, without event replay.
   * This callback must complete synchronously; returned Promises are not awaited.
   * Use AsyncEventSubscription/AsyncExecutor for an awaited handler lifetime.
   */
  spinOnce() {
    if(this.spinning)return Status.CONFLICT;const observer=this.observer;observer.check();
    const release=acquirePolling([observer]);if(!release)return Status.CONFLICT;this.spinning=true;
    try {const callback=this.callback;checkedHandler(callback);const reply=observer.session.pollEventV1(observer.handle);
      if(!reply.status)callback.call(this,this.binding.decode(reply.bytes));return reply.status;}
    finally {release();this.spinning=false;}
  }
  /** Real bounded native service admission; completion needs a later exact-ID
   * event. Do not call from this observer's active callback or replay timeouts.
   */
  submit(bytes=new Uint8Array()){return this.source.submitService(this.observer,bytes);}
  /** Positive counters are a snapshot, never reserved admission credit. */
  readiness(){return this.observer.eventReadiness();}
  /** Close only the observer; retain independent source lifetime/cleanup. */
  close(){return this.spinning?Status.CONFLICT:this.observer.close();}
  /** Revoke only the observer alias, without implicit cancel-any permission. */
  revoke(){return this.spinning?Status.CONFLICT:this.observer.revoke();}
}

/** Typed subscription serviced entirely by guest/bridge-side polling. */
export class Subscription {
  /** Subscribe explicitly. Constructor failure closes the newly owned capability once. */
  constructor(capability, codec = StringMessage, callback = () => {}) {
    checkedCapabilities([capability]); checkedCodec(codec); checkedHandler(callback);
    this.capability = capability; this.codec = codec; this.callback = callback; this.spinning = false;
    const status = capability.session.raw.topic_subscribe(capability.handle);
    if (status) { rollback([capability]); throw new RosError(status, 'topic_subscribe'); }
  }
  /** Poll at most one event, copy/decode it, then run the local callback.
   * Direct recursive polling and close/revoke return17 before any import.
   * The exact callable is retained for this pass, preserving its captured state
   * when callback is reassigned. An exception propagates after guard release.
   * Another independently owned resource remains callable after scratch release.
   */
  spinOnce() {
    if (this.spinning) return Status.CONFLICT; const capability = this.capability, release = acquirePolling([capability]); if (!release) return Status.CONFLICT; this.spinning = true;
    try { const callback = this.callback, codec = this.codec; checkedHandler(callback); const r = pollCapability(capability, true); if (!r.status) callback.call(this, codec.decode(r.bytes)); return r.status; }
    finally { release(); this.spinning = false; }
  }
  /** Close the owned subscription capability once. */
  close() { return this.spinning ? Status.CONFLICT : this.capability.close(); }
  /** Explicit alias revocation, refused during this resource's active pass. */
  revoke() { return this.spinning ? Status.CONFLICT : this.capability.revoke(); }
}

/** Bounded single-thread executor. It does not create OS threads, sleep or read clocks. */
export class Executor {
  /** Initialize an empty list; an application supplies scheduling explicitly. */
  constructor(options = {}) { this.entities = []; this.spinning = false; this.closed = false; this.tasks = new DeferredQueue(options); }
  /** Register a local entity exposing spinOnce(). No authority is opened here.
   * Additions made while spinning become eligible only in the next outer pass.
   */
  add(entity) { if (this.closed) throw new RosError(Status.BAD_HANDLE, 'closed Executor'); if (typeof entity.spinOnce !== 'function') throw new TypeError('Polling entity required'); this.entities.push(entity); return entity; }
  /** Poll the registration snapshot once. Nested spin returns [Conflict] without dispatch.
   * A callback exception propagates while finally releases the busy guard; the
   * executor never automatically retries a partially completed pass.
   */
  spinOnce() {
    if (this.closed) return [Status.BAD_HANDLE]; if (this.spinning) return [Status.CONFLICT]; this.spinning = true;
    const pending = this.tasks.snapshot();
    try { const registrations = this.entities.slice(), results = []; for (const entity of registrations) { if (this.closed) break; results.push(entity.spinOnce()); } this.tasks.run(pending); return results; }
    catch (error) { for (const { future } of pending) if (future.start()) future.reject(error); throw error; }
    finally { this.tasks.finishSnapshot(); this.spinning = false; }
  }
  /** Queue one bounded local task. The native service_call inside it still
   * blocks this caller-driven pass; a Future is not native async execution.
   */
  defer(callback) { return this.tasks.defer(callback); }
  /** Cancel unstarted local tasks only; registered resources close explicitly. */
  close() { this.closed = true; this.tasks.close(); this.entities = []; }
}

/** Synchronous typed service client. Runtime-configured timeout and quotas remain authoritative. */
export class Client {
  /** Construct from service.call capability and canonical request/response codecs. */
  constructor(capability, requestCodec = TriggerRequest, responseCodec = ServiceResponse) { checkedCapabilities([capability]); this.capability = capability; this.requestCodec = checkedCodec(requestCodec); this.responseCodec = checkedCodec(responseCodec); }
  /** Perform one call; decode only success. This is not an async Promise-based native executor. */
  call(value = null) { const r = this.capability.request('service_call', this.requestCodec.encode(value)); return { ...r, value: r.status ? null : this.responseCodec.decode(r.bytes) }; }
  /** Return a local Future, executing exactly one original bounded call only
   * when the explicitly supplied executor advances its deferred task snapshot.
   */
  callDeferred(executor, value = null) { if (!(executor instanceof Executor)) throw new TypeError('Caller-driven Executor required'); return executor.defer(() => this.call(value)); }
  /** Release the owned service.call handle. */
  close() { return this.capability.close(); }
}

/** Service server with distinct advertise/respond capabilities, each explicitly provisioned. */
export class Service {
  /** Advertise once and retain a local request handler; failure closes both owned handles. */
  constructor(advertise, respond, handler, requestCodec = TriggerRequest) {
    checkedCapabilities([advertise, respond]); checkedHandler(handler); checkedCodec(requestCodec);
    this.advertise = advertise; this.respond = respond; this.handler = handler; this.requestCodec = requestCodec; this.spinning = false;
    const status = advertise.session.raw.service_advertise(advertise.handle);
    if (status) { rollback([advertise, respond]); throw new RosError(status, 'service_advertise'); }
  }
  /** Poll one request ID and respond once. No handler pointer enters native ROS.
   * The pass holds one intrinsic guard and the original handler/owner references;
   * callback replacement affects the next pass. Nested poll/close/revoke is17.
   * Handler/codec/import exceptions propagate and finally releases the guard.
   */
  spinOnce() {
    if (this.spinning) return Status.CONFLICT; const advertise = this.advertise, respond = this.respond, release = acquirePolling([advertise, respond]); if (!release) return Status.CONFLICT; this.spinning = true;
    try { const handler = this.handler, codec = this.requestCodec; checkedHandler(handler); const r = pollCapability(advertise, true); if (r.status) return r.status; const q = decodeServiceRequest(r.bytes, codec); return respond.request('service_respond', encodeServiceReply(q.requestId, handler.call(this, q.request))).status; }
    finally { release(); this.spinning = false; }
  }
  /** Close each owned handle once; return both statuses. */
  close() { return this.spinning ? [Status.CONFLICT] : closeOwned([this.advertise, this.respond]); }
  /** Deliberately revoke both exact aliases, without creating stronger rights. */
  revoke() { return this.spinning ? [Status.CONFLICT] : revokeOwned([this.advertise, this.respond]); }
}

/** Fibonacci Action client. Each operation needs its own configured alias/handle. */
export class ActionClient {
  /** @param {object} capabilities send is required. cancelOwn, feedback, result and privileged cancelAny are optional explicit grants. */
  constructor(capabilities) { if (!capabilities || !(capabilities.send instanceof Capability)) throw new TypeError('Missing Action send capability'); for (const key of Object.keys(capabilities)) if (!['send', 'cancelOwn', 'cancelAny', 'feedback', 'result'].includes(key)) throw new TypeError('Unknown Action capability'); checkedCapabilities(Object.values(capabilities)); this.capabilities = { ...capabilities }; }
  /** Require only the operation requested now; missing optional authority opens nothing. */
  capability(key, operation) { const cap = this.capabilities[key]; if (!cap) throw new RosError(Status.DENIED, operation); return cap; }
  /** Send a bounded goal; return an opaque BigInt goal ID only on success. */
  sendGoal(order) { const r = this.capabilities.send.request('action_send_goal', FibonacciGoal.encode(order)); return { ...r, goalId: r.status ? null : decodeGoalId(r.bytes) }; }
  /** Cancel only an owned goal using its explicit cancel-own capability. */
  cancelOwn(goalId) { return this.capability('cancelOwn', 'action_cancel_own_goal').request('action_cancel_own_goal', encodeGoalId(goalId)); }
  /** Cancel another goal only if the caller explicitly supplied a cancelAny capability. */
  cancelAny(goalId) { if (!this.capabilities.cancelAny) throw new RosError(Status.DENIED, 'action_cancel_any_goal'); return this.capabilities.cancelAny.request('action_cancel_any_goal', this.cancelAnyBytes(goalId)); }
  /** Encode cancel-any's special zero ID for cancel-all; all other goal operations still reject zero. */
  cancelAnyBytes(goalId) { u64(goalId); const bytes = new Uint8Array(8); new DataView(bytes.buffer).setBigUint64(0, goalId, true); return bytes; }
  /** Cancel all goals only through an explicitly supplied action.cancel-any-goal capability. */
  cancelAll() { return this.cancelAny(0n); }
  /** Poll feedback by goal ID; wire omits kind. A canonical envelope for another
   * ID becomes22/value:null without modifying raw copied bytes or replaying.
   */
  feedback(goalId) { const r = this.capability('feedback', 'action_receive_feedback').request('action_receive_feedback', encodeGoalId(goalId)); if (r.status) return { ...r, value: null }; const value = decodeActionSequence(r.bytes, false); return value.goalId === goalId ? { ...r, value } : { ...r, status: Status.INVALID, runtimeStatus: r.status, value: null }; }
  /** Poll one terminal2/3/4 and require the exact requested ID. Mismatch is22;
   * prior caller results and the actual copied status/length bytes stay intact.
   */
  result(goalId) { const r = this.capability('result', 'action_receive_result').request('action_receive_result', encodeGoalId(goalId)); if (r.status) return { ...r, value: null }; const value = ActionResult.decode(r.bytes); return value.goalId === goalId ? { ...r, value } : { ...r, status: Status.INVALID, runtimeStatus: r.status, value: null }; }
  /** Close each distinct handle once. No native cancellation is implied by this convenience method. */
  close() { return closeOwned(Object.values(this.capabilities)); }
}

/** Fibonacci Action server; feedback/result uses explicit respond authority. */
export class ActionServer {
  /** Advertise with an empty canonical request and retain a local handler. */
  constructor(advertise, respond, handler) { checkedCapabilities([advertise, respond]); checkedHandler(handler); this.advertise = advertise; this.respond = respond; this.handler = handler; this.spinning = false; const r = advertise.request('action_advertise'); if (r.status) { rollback([advertise, respond]); throw new RosError(r.status, 'action_advertise'); } }
  /** Poll one incoming goal; handler returns {kind, values}, sent for its exact ID.
   * Recursive polling/close/revoke is17 throughout the callback and response.
   * Explicit responder requests remain permitted after copied poll completion.
   * Handler replacement affects only the next pass; exceptions reset the guard.
   */
  spinOnce() {
    if (this.spinning) return Status.CONFLICT; const advertise = this.advertise, respond = this.respond, release = acquirePolling([advertise, respond]); if (!release) return Status.CONFLICT; this.spinning = true;
    try { const handler = this.handler; checkedHandler(handler); const r = pollCapability(advertise, true); if (r.status) return r.status; const goal = decodeIncomingGoal(r.bytes), reply = handler.call(this, goal); return respond.request('action_respond', encodeActionSequence({ ...reply, goalId: goal.goalId })).status; }
    finally { release(); this.spinning = false; }
  }
  /** Close the two owned handles once. */
  close() { return this.spinning ? [Status.CONFLICT] : closeOwned([this.advertise, this.respond]); }
  /** Explicitly revoke the two independently acquired aliases after a pass. */
  revoke() { return this.spinning ? [Status.CONFLICT] : revokeOwned([this.advertise, this.respond]); }
}

/** Generic exact-operation resource for Parameters, Graph, Clock, Tf or Lifecycle.
 * These operations remain complete at the byte level; application codecs must
 * match the resource/type chosen by each configured alias. No native API parity
 * or implicit resource discovery is promised.
 */
export class Resource {
  /** @param {Capability} capability Handle for exactly one allowed operation.
   * @param {string} operation Exact ABI import name (for example parameter_get).
   */
  constructor(capability, operation) { checkedCapabilities([capability]); const f = specification(capability.session.version).functions.find(x => x.name === operation); if (!f || f.parameters.length !== 7) throw new RosError(Status.UNSUPPORTED, operation); this.capability = capability; this.operation = operation; }
  /** Make one canonical request, returning unchanged error code and copied wire. */
  call(bytes = new Uint8Array()) { return this.capability.request(this.operation, bytes); }
  /** Close the owned exact-operation handle. */
  close() { return this.capability.close(); }
}

/** Typed parameter convenience object; names/resources are fixed by explicit aliases.
 * Each key is a Capability for that operation, not an unrestricted parameter server.
 */
export class Parameters {
  /** @param {object} capabilities Optional get/set/list/describe/declare/undeclare/delete/exists/search/events handles.
   * @param {object} codec Canonical scalar/array codec for the configured parameter type.
   */
  constructor(capabilities, codec = StringMessage) { if (!capabilities || typeof capabilities !== 'object' || Array.isArray(capabilities)) throw new TypeError('Explicit parameter capability map required'); for (const key of Object.keys(capabilities)) if (!['get', 'set', 'list', 'describe', 'declare', 'undeclare', 'delete', 'exists', 'search', 'events'].includes(key)) throw new TypeError('Unknown parameter capability'); checkedCapabilities(Object.values(capabilities)); this.capabilities = { ...capabilities }; this.codec = checkedCodec(codec); this.callbacks = new ParameterCallbacks(); }
  /** Invoke one configured operation, rejecting a missing handle without opening anything. */
  invoke(key, bytes = new Uint8Array(), codec = null) { const cap = this.capabilities[key]; if (!cap) throw new RosError(Status.DENIED, `parameter_${key}`); const r = cap.request(`parameter_${key}`, bytes); return { ...r, value: r.status || !codec ? null : codec.decode(r.bytes) }; }
  /** Read one policy-selected parameter; no name is supplied from JavaScript. */
  get() { return this.invoke('get', new Uint8Array(), this.codec); }
  /** Set one policy-selected parameter with its canonical typed wire. */
  set(value) { return this.invoke('set', this.codec.encode(value)); }
  /** Read the permitted newline-separated list as canonical UTF-8 text. */
  list() { return this.invoke('list', new Uint8Array(), StringMessage); }
  /** ROS 2 metadata is returned as documented canonical text; no adjacent names are requested. */
  describe() { return this.invoke('describe', new Uint8Array(), StringMessage); }
  /** ROS 2 declaration requires explicit declare authority and configured type. */
  declare(value) { return this.invoke('declare', this.codec.encode(value)); }
  /** ROS 2 undeclaration uses its own alias and an empty request. */
  undeclare() { return this.invoke('undeclare'); }
  /** ROS 1 deletion uses its own alias and an empty request. */
  delete() { return this.invoke('delete'); }
  /** ROS 1 exact-name existence query decodes a canonical Bool. */
  exists() { return this.invoke('exists', new Uint8Array(), BoolMessage); }
  /** ROS 1 search uses the runtime's current exact-name safe subset. */
  search() { return this.invoke('search', new Uint8Array(), StringMessage); }
  /** Start ROS 2 events using an explicit param.subscribe-events handle. */
  subscribeEvents() { const cap = this.capabilities.events; if (!cap) throw new RosError(Status.DENIED, 'parameter_subscribe_events'); cap.check(); if (cap.session.version !== 2) throw new RosError(Status.UNSUPPORTED, 'parameter_subscribe_events'); return cap.session.raw.parameter_subscribe_events(cap.handle); }
  /** Poll one typed/legacy parameter event; caller chooses the event codec explicitly. */
  pollEvent(codec) { checkedCodec(codec); const cap = this.capabilities.events; if (!cap) throw new RosError(Status.DENIED, 'parameter events'); const r = cap.poll(), value = r.status ? null : codec.decode(r.bytes); if (!r.status) this.callbacks.dispatch(value); return { ...r, value }; }
  /** Register a local post-decode event callback, returning a removal function. */
  onEvent(callback) { return this.callbacks.add(callback); }
  /** Release each distinct supplied handle once. */
  close() { return closeOwned(Object.values(this.capabilities)); }
}

/** One policy-filtered graph operation, decoded into canonical newline/tab rows. */
export class Graph extends Resource {
  /** Reject unrelated operations even when constructing this public class directly. */
  constructor(capability, operation) { if (typeof operation !== 'string' || !operation.startsWith('graph_')) throw new RosError(Status.INVALID, operation); super(capability, operation); }
  /** A graph read never substitutes an allowed write operation or sends a payload. */
  call(bytes = new Uint8Array()) { if (typeof this.operation !== 'string' || !this.operation.startsWith('graph_') || bytes.length) throw new RosError(Status.INVALID, this.operation); return super.call(bytes); }
  /** Read permitted rows; type is empty for node lists. Empty text yields an empty array. */
  read() { const r = this.call(); let rows = null; if (!r.status) { const text = StringMessage.decode(r.bytes); if (text && !text.endsWith('\n')) throw new RangeError('Graph row terminator'); rows = text ? text.slice(0, -1).split('\n').map(line => { const fields = line.split('\t'); if (fields.length > 2) throw new RangeError('Graph row fields'); return { name: fields[0], type: fields[1] ?? '' }; }) : []; } return { ...r, rows }; }
}

/** One explicitly authorized clock kind. Each read returns BigInt nanoseconds. */
export class Clock extends Resource {
  /** Reject unrelated operations before this public Clock can dispatch a host request. */
  constructor(capability, operation) { if (typeof operation !== 'string' || !operation.startsWith('time_read_')) throw new RosError(Status.INVALID, operation); super(capability, operation); }
  /** Recheck the read-only clock family, including after caller mutation of operation. */
  call(bytes = new Uint8Array()) { if (typeof this.operation !== 'string' || !this.operation.startsWith('time_read_') || bytes.length) throw new RosError(Status.INVALID, this.operation); return super.call(bytes); }
  /** Read once without assuming any relationship between ROS, wall/system and steady epochs. */
  now() { const r = this.call(); return { ...r, nanoseconds: r.status ? null : TimeNanoseconds.decode(r.bytes) }; }
}

/** TF convenience object; frame pair and authority remain fixed in separate aliases. */
export class Tf {
  /** @param {Capability|null} lookup Explicit tf.lookup handle. @param {Capability|null} broadcast Explicit tf.broadcast handle. */
  constructor(lookup = null, broadcast = null) { checkedCapabilities([lookup, broadcast].filter(Boolean)); this.lookupCapability = lookup; this.broadcastCapability = broadcast; }
  /** Request one configured transform at a timestamp, zero meaning latest. */
  lookup(nanoseconds = 0n) { if (!this.lookupCapability) throw new RosError(Status.DENIED, 'tf_lookup'); const r = this.lookupCapability.request('tf_lookup', encodeLookupTime(nanoseconds)); return { ...r, value: r.status ? null : transformMessage(this.lookupCapability.session.version).decode(r.bytes) }; }
  /** Broadcast one canonical transform with explicit authority. */
  broadcast(value) { if (!this.broadcastCapability) throw new RosError(Status.DENIED, 'tf_broadcast'); return this.broadcastCapability.request('tf_broadcast', transformMessage(this.broadcastCapability.session.version).encode(value)); }
  /** Release supplied transform handles once. */
  close() { return closeOwned([this.lookupCapability, this.broadcastCapability]); }
}

/** ROS 2 exact-node lifecycle convenience object. The transition is chosen in policy. */
export class Lifecycle {
  /** @param {Capability|null} getState Explicit lifecycle.get-state handle. @param {Capability|null} changeState Explicit lifecycle.change-state handle. */
  constructor(getState = null, changeState = null) { const caps = [getState, changeState].filter(Boolean); checkedCapabilities(caps); if (caps.some(cap => cap.session.version !== 2)) throw new RosError(Status.UNSUPPORTED, 'lifecycle'); this.getCapability = getState; this.changeCapability = changeState; }
  /** Read the configured node's state, using an empty request. */
  getState() { if (!this.getCapability) throw new RosError(Status.DENIED, 'lifecycle_get_state'); const r = this.getCapability.request('lifecycle_get_state'); return { ...r, value: r.status ? null : LifecycleState.decode(r.bytes) }; }
  /** Apply only the configured allowed-transition; no transition/name enters the payload. */
  changeState() { if (!this.changeCapability) throw new RosError(Status.DENIED, 'lifecycle_change_state'); const r = this.changeCapability.request('lifecycle_change_state'); return { ...r, accepted: r.status ? null : BoolMessage.decode(r.bytes) }; }
  /** Release supplied lifecycle handles once. */
  close() { return closeOwned([this.getCapability, this.changeCapability]); }
}

/** Capability-scoped logger. Logs require a log.write alias; console is separate. */
export class Logger {
  /** Construct from an explicitly opened log.write handle. */
  constructor(capability) { checkedCapabilities([capability]); this.capability = capability; }
  /** Copy UTF-8 text once; severity is fixed in runtime policy, not selected here. */
  write(text) { this.capability.check(); return this.capability.session.write('log_write', this.capability.handle, StringMessage.encode(text)); }
  /** Release log authority ownership. */
  close() { return this.capability.close(); }
}

/** Guest-side Node organizer. Names are local labels and never change runtime principal identity. */
export class Node {
  /** @param {Session} session Explicit trusted bridge session. @param {string} name Local diagnostic label. */
  constructor(session, name = 'roswasm') { if (!(session instanceof Session) || typeof name !== 'string') throw new TypeError('Explicit Session/local Node name required'); this.session = session; this.name = name; this.owned = []; this.context = null; nodeRecords.set(this,{active:false,ledgers:[],lastFailure:null}); }
  /** Context is a local ownership scope; it does not initialize native ROS. */
  checkContext() { if(nodeRecords.get(this).active)throw new RosError(Status.CONFLICT,'Node ownership transaction'); if (this.context?.closed) throw new RosError(Status.BAD_HANDLE, 'closed Context'); }
  /** Open one configured alias and remember ownership for close(). */
  open(alias) { return this.transaction([alias],caps=>caps[0]); }
  /** Acquire factory aliases as one local ownership transaction.
   * Commit handles only after every requested open and construction succeeds.
   * On failure close each newly acquired handle once, preserving the original
   * exception. No advertiser constructor runs until all its aliases are open.
   */
  transaction(aliases, construct) {
    this.checkContext();
    if (!Array.isArray(aliases) || typeof construct !== 'function') throw new TypeError('Explicit alias transaction required');
    const selected=Array.from(aliases);
    for (const alias of selected) if (typeof alias !== 'string' || !alias.length || alias.includes('\0')) throw new TypeError('Nonempty NUL-free alias required');
    const record=nodeRecords.get(this),ledger={owners:new Array(selected.length).fill(null),committed:false,primary:undefined,cleanupAttempts:null};
    // Both storage and its persistent Node link exist before the first open.
    record.ledgers.push(ledger);record.active=true;
    try {
      for(let i=0;i<selected.length;++i)ledger.owners[i]=this.session.open(selected[i]);
      const result=construct(ledger.owners.slice());
      this.owned.push(...ledger.owners.filter(cap=>!capabilityRetired(cap)));
      ledger.committed=true;return result;
    } catch(error) {ledger.primary=error;record.lastFailure=ledger;rollback(ledger.owners,error,ledger);throw error;}
    finally {record.active=false;pruneNodeLedgers(record);}
  }
  /** Create a typed publisher from one exact topic.publish alias. */
  createPublisher(alias, codec = StringMessage) { checkedCodec(codec); return this.transaction([alias], caps => new Publisher(caps[0], codec)); }
  /** Create a typed polling subscription from one exact topic.subscribe alias. */
  createSubscription(alias, codec = StringMessage, callback = () => {}) { checkedCodec(codec); checkedHandler(callback); return this.transaction([alias], caps => new Subscription(caps[0], codec, callback)); }
  /** Create a typed synchronous service client. Trigger and SetBool codecs are included. */
  createClient(alias, requestCodec = TriggerRequest, responseCodec = ServiceResponse) { checkedCodec(requestCodec); checkedCodec(responseCodec); return this.transaction([alias], caps => new Client(caps[0], requestCodec, responseCodec)); }
  /** Create a service server using explicitly distinct advertise/respond aliases. */
  createService(advertiseAlias, respondAlias, handler, requestCodec = TriggerRequest) { checkedHandler(handler); checkedCodec(requestCodec); return this.transaction([advertiseAlias, respondAlias], caps => new Service(caps[0], caps[1], handler, requestCodec)); }
  /** Open exactly the Action operation aliases supplied by the application. */
  createActionClient(aliases) { if (typeof aliases.send !== 'string') throw new TypeError('Missing Action send alias'); const keys = Object.keys(aliases); for (const key of keys) if (!['send', 'cancelOwn', 'cancelAny', 'feedback', 'result'].includes(key)) throw new TypeError(`Unknown Action operation ${key}`); return this.transaction(keys.map(key => aliases[key]), caps => new ActionClient(Object.fromEntries(keys.map((key, index) => [key, caps[index]])))); }
  /** Create a polling Action server from explicit advertise/respond aliases. */
  createActionServer(advertiseAlias, respondAlias, handler) { checkedHandler(handler); return this.transaction([advertiseAlias, respondAlias], caps => new ActionServer(caps[0], caps[1], handler)); }
  /** Open only the named parameter-operation aliases; no parameter namespace is inherited. */
  createParameters(aliases, codec = StringMessage) { checkedCodec(codec); if (!aliases || typeof aliases !== 'object' || Array.isArray(aliases)) throw new TypeError('Explicit parameter alias map required'); const keys = Object.keys(aliases); for (const key of keys) { const operation = key === 'events' ? 'parameter_subscribe_events' : `parameter_${key}`; if (!specification(this.session.version).functions.some(f => f.name === operation)) throw new RosError(Status.UNSUPPORTED, operation); } return this.transaction(keys.map(key => aliases[key]), caps => new Parameters(Object.fromEntries(keys.map((key, index) => [key, caps[index]])), codec)); }
  /** Open one exact graph operation; the native result is filtered by capability policy. */
  createGraph(alias, operation) { this.validateOperation(operation); if (!operation.startsWith('graph_')) throw new RosError(Status.INVALID, operation); return this.transaction([alias], caps => new Graph(caps[0], operation)); }
  /** Open one clock kind explicitly; executor scheduling does not call this factory. */
  createClock(alias, operation = 'time_read_ros') { this.validateOperation(operation); if (!operation.startsWith('time_read_')) throw new RosError(Status.INVALID, operation); return this.transaction([alias], caps => new Clock(caps[0], operation)); }
  /** Open optional, independently granted lookup and broadcast aliases. */
  createTf(lookupAlias = null, broadcastAlias = null) { const aliases = [lookupAlias, broadcastAlias].filter(alias => alias !== null); return this.transaction(aliases, caps => new Tf(lookupAlias !== null ? caps[0] : null, broadcastAlias !== null ? caps[lookupAlias !== null ? 1 : 0] : null)); }
  /** Open optional ROS 2 exact-node lifecycle operations, with transition fixed in policy. */
  createLifecycle(getAlias = null, changeAlias = null) { if (this.session.version !== 2) throw new RosError(Status.UNSUPPORTED, 'lifecycle'); const aliases = [getAlias, changeAlias].filter(alias => alias !== null); return this.transaction(aliases, caps => new Lifecycle(getAlias !== null ? caps[0] : null, changeAlias !== null ? caps[getAlias !== null ? 1 : 0] : null)); }
  /** Open an exact operation resource; unsupported version operations are rejected before opening authority. */
  createResource(alias, operation) { this.validateOperation(operation); return this.transaction([alias], caps => new Resource(caps[0], operation)); }
  /** Validate a request operation before attempting to open its alias. */
  validateOperation(operation) { const f = specification(this.session.version).functions.find(x => x.name === operation); if (!f || f.parameters.length !== 7) throw new RosError(Status.UNSUPPORTED, operation); }
  /** Create a capability-scoped logger from an explicit log.write alias. */
  createLogger(alias) { return this.transaction([alias], caps => new Logger(caps[0])); }
  /** Release all still-owned handles once, including after a backend failure. */
  close() { const record=nodeRecords.get(this);if(record.active)throw new RosError(Status.CONFLICT,'Node ownership transaction');try{return closeOwned(nodeOwners(this));}finally{pruneNodeLedgers(record);} }
  /** Snapshot pending owners. Nonenumerable factory retains the latest failed
   * factory primary/attempts even when the primary cannot accept properties. */
  pendingCleanup() {
    const pending=nodeOwners(this).filter(cap=>!capabilityRetired(cap)).map(cap=>({handle:cap.handle,attempted:false}));
    const failed=nodeRecords.get(this).lastFailure;
    Object.defineProperty(pending,'factory',{value:failed?{primary:failed.primary,attempts:(failed.cleanupAttempts||[]).map(row=>({...row}))}:null});
    return pending;
  }
}

/** Local application ownership context over one already admitted Session.
 * It creates no native node/context/thread and opens no alias by itself.
 * shutdown attempts all registered Node cleanup even if one bridge throws.
 */
export class Context {
  constructor(session) { if (!(session instanceof Session)) throw new TypeError('Explicit Session required'); this.session = session; this.nodes = []; this.closed = false; }
  createNode(name = 'roswasm') { if (this.closed) throw new RosError(Status.BAD_HANDLE, 'closed Context'); const node = new Node(this.session, name); node.context = this; this.nodes.push(node); return node; }
  shutdown() {
    if (this.closed) throw new RosError(Status.BAD_HANDLE, 'closed Context');
    if (this.nodes.some(node => nodeRecords.get(node).active || ownersBusy(nodeOwners(node)))) throw new RosError(Status.CONFLICT, 'active resource ownership'); this.closed = true;
    const results = [], errors = [];
    try {for (const node of this.nodes) { try { results.push(node.close()); } catch (error) { results.push(error); errors.push(error); } }}
    finally {this.closed=!this.nodes.some(nodeHasLiveOwners);}
    if (errors.length) { const error = new AggregateError(errors, 'Context cleanup failed'); error.results = results; throw error; }
    return results;
  }
}

/** Frequently used message codecs re-exported for application ergonomics. */
export { StringMessage, BoolMessage, TwistMessage, ServiceResponse, TriggerRequest, TransformMessage, encodeLookupTime };

/** Additive copied unified event codecs and local binding guard. */
export {EventKind,decodeUnifiedEvent,decodeEventReadiness,EventBinding} from "./events.mjs";
