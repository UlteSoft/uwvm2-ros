/** Capability-scoped AssemblyScript client-library concepts.
 * All imports execute synchronously inside the Wasm guest. Every alias must have
 * an explicit runtime grant; Node construction and codecs create no authority.
 * Polling dispatches guest functions only after a complete copied event arrives.
 * No function pointer or managed object address is passed to native ROS.
 */
import { Api, Operation, Status } from './api';
import { Codec, Reader, Writer, MAX_WIRE, StringCodec, BoolCodec, ServiceResponse,
  ServiceRequest, encodeServiceReply, decodeServiceRequest, decodeServiceResponse,
  encodeFibonacciGoal, encodeGoalId, decodeGoalId, ActionSequence, IncomingGoal,
  encodeActionSequence, decodeActionSequence, decodeIncomingGoal, ParameterValue,
  ParameterKind, encodeParameter, decodeParameter, TypedParameterEvent,
  decodeTypedParameterEvent, Transform, encodeTransform, decodeTransform,
  LifecycleState, decodeLifecycleState, validUnicode } from './codecs';
import { EventBinding, UnifiedEvent } from './events';
const SENTINEL: u64 = u64.MAX_VALUE;

/** Restrict a named Graph facade to graph query selectors. */
export function graphOperation(op: Operation): bool { return op == Operation.graph_list_topics || op == Operation.graph_list_nodes || op == Operation.graph_list_services || op == Operation.graph_inspect_topic || op == Operation.graph_inspect_node || op == Operation.graph_inspect_service || op == Operation.graph_inspect_action; }
/** Restrict a named Clock facade to clock reads. */
export function clockOperation(op: Operation): bool { return op == Operation.time_read_ros || op == Operation.time_read_wall || op == Operation.time_read_system || op == Operation.time_read_steady; }
/** Restrict Parameters facades to parameter operations/events. */
export function parameterOperation(op: Operation): bool { return op == Operation.parameter_get || op == Operation.parameter_set || op == Operation.parameter_delete || op == Operation.parameter_exists || op == Operation.parameter_list || op == Operation.parameter_search || op == Operation.parameter_describe || op == Operation.parameter_declare || op == Operation.parameter_undeclare || op == Operation.parameter_subscribe_events; }
/** Restrict TF facades to lookup/broadcast operations. */
export function tfOperation(op: Operation): bool { return op == Operation.tf_lookup || op == Operation.tf_broadcast; }
/** Restrict Lifecycle facades to the two exact-node lifecycle operations. */
export function lifecycleOperation(op: Operation): bool { return op == Operation.lifecycle_get_state || op == Operation.lifecycle_change_state; }

/** Copied request result. requiredValid is false when an error left the slot untouched. */
export class Reply {
  /** Status zero means bytes are a complete copied response. */
  constructor(public status: u32 = Status.Success, public bytes: Uint8Array = new Uint8Array(0), public required: u64 = 0, public requiredValid: bool = false) {}
}

/** Internal identity token. A handler receives copied values, never this token.
 * Nested callers cannot acquire or release another dispatch pass's lease.
 */
class CallbackLease {}

/** Owned opaque capability. Open() reports denial instead of inventing a handle. */
export class Capability {
  /** Zero until successful open; never forge a handle from an alias or native ID. */
  private handleValue: u64 = 0;
  /** Read-only view of the owned ID; rawHandle also checks current ownership. */
  get handle(): u64 { return this.handleValue; }
  /** Last open status, including unsupported ABI or default-deny 13. */
  openStatus: u32 = Status.BadHandle;
  private owned: bool = false;
  private callbackLease: CallbackLease | null = null;
  /** @param api Select the actual ROS 1 or ROS 2 NativeApi; no native ROS libraries are linked. */
  constructor(private selectedApi: Api) {}
  /** Backend adapter is fixed when this owner is constructed. */
  get api(): Api { return this.selectedApi; }
  /** UTF-8 encode one configured alias and decode the eight-byte little-endian handle.
   * Managed array references remain rooted during the synchronous import. They
   * are not strings' UTF-16 object addresses, and runtime copies them immediately.
   */
  open(alias: string): u32 {
    if (this.owned || !alias.length || alias.length > 255 || alias.indexOf("\0") >= 0 || !validUnicode(alias)) return this.openStatus = Status.Invalid;
    if (this.api.version() != 1) return this.openStatus = Status.Unsupported;
    const input = Uint8Array.wrap(String.UTF8.encode(alias)), slot = new Uint8Array(8);
    if (input.length > 255) return this.openStatus = Status.Invalid;
    store<u64>(slot.dataStart, 0);
    this.openStatus = this.api.open(0, <u64>input.dataStart, <u64>input.length, <u64>slot.dataStart);
    if (!this.openStatus) { const id = load<u64>(slot.dataStart); if (!id) return this.openStatus = Status.Invalid; this.handleValue = id; this.owned = true; }
    return this.openStatus;
  }
  /** Whether this wrapper currently owns a successfully opened handle. */
  isOpen(): bool { return this.owned; }
  /** Borrow a full-width live ID and status without forging or transferring ownership. */
  rawHandle(): HandleReply { return new HandleReply(this.owned ? Status.Success : Status.BadHandle, this.owned ? this.handle : 0); }
  /** Internal caller-step lease shared by typed facades and Node/Session owners.
   * It protects retirement, not ordinary response imports. No native lock,
   * thread or permission is acquired. Nested dispatch returns17 before polling.
   */
  enterCallback(): CallbackLease | null { if(!this.owned || this.callbackLease) return null; const lease=new CallbackLease(); this.callbackLease=lease; return lease; }
  /** Internal release after a normal callback pass, including idle/error returns.
   * A fatal Wasm trap/abort cannot promise cleanup or lease recovery.
   */
  leaveCallback(lease:CallbackLease): u32 { if(this.callbackLease!==lease) return Status.Conflict; this.callbackLease=null; return Status.Success; }
  /** Preflight retirement without changing state or dispatching an import. */
  retirementStatus(): u32 { return !this.owned?Status.BadHandle:this.callbackLease?Status.Conflict:Status.Success; }
  /** Close once and release ownership even on 107/110: the runtime may already retire the handle. */
  close(): u32 { const status=this.retirementStatus(); if(status) return status; this.owned = false; const id = this.handle; this.handleValue = 0; return this.api.close(id); }
  /** Revoke with current authority and release ownership after one attempt, even on backend failure. */
  revoke(): u32 { const status=this.retirementStatus(); if(status) return status; this.owned = false; const id = this.handle; this.handleValue = 0; return this.api.revoke(id); }
  /** Publish a copied canonical message once through topic.publish authority. */
  publish(bytes: Uint8Array): u32 { return this.owned ? this.api.publish(this.handle, 0, <u64>bytes.dataStart, <u64>bytes.length) : Status.BadHandle; }
  /** Start an explicitly provisioned topic subscription. */
  subscribe(): u32 { return this.owned ? this.api.subscribe(this.handle) : Status.BadHandle; }
  /** Start an explicitly provisioned service server. */
  advertiseService(): u32 { return this.owned ? this.api.advertiseService(this.handle) : Status.BadHandle; }
  /** Start ROS 2 parameter events; ROS 1 returns 95. */
  subscribeParameterEvents(): u32 { return this.owned ? this.api.parameterEvents(this.handle) : Status.BadHandle; }
  /** Write canonical String bytes to an explicitly provisioned logger. */
  log(bytes: Uint8Array): u32 { return this.owned ? this.api.log(this.handle, 0, <u64>bytes.dataStart, <u64>bytes.length) : Status.BadHandle; }
  /** Interpret a slot only on success/90 and only when actually overwritten. */
  private finish(status: u32, output: Uint8Array, slot: Uint8Array): Reply { const length = load<u64>(slot.dataStart), known = (status == 0 || status == Status.TooLarge) && length != SENTINEL; if (!status && (!known || length > <u64>output.length)) return new Reply(Status.Invalid); return new Reply(status, status ? new Uint8Array(0) : output.slice(0, <i32>length), known ? length : 0, known); }
  /** Bind this observer to an already granted source once. A shared callback
   * lease refuses nested association before imports. Correlation is zero except
   * for selected outgoing Action data. Metadata-only watches require zero and
   * do not activate a reader; a data watch must precede legacy polling.
   */
  watchEvents(source:Capability,correlation:u64=0):u32 {
    let error=this.retirementStatus();if(error)return error;error=source.retirementStatus();if(error)return error;
    if(this.api!==source.api||this.handle==source.handle)return Status.Invalid;
    return this.api.watchEvents(this.handle,source.handle,correlation);
  }
  /** New poll90 retains a record and conditionally writes required bytes. No
   * resize/retry is performed. The returned complete copy needs event decoding.
   */
  pollEventV1(capacity:i32):Reply {
    const status=this.retirementStatus();return status?new Reply(status):this.pollUnifiedOnce(capacity);
  }
  /** Internal exact-token first poll. Public poll/close/revoke still refuse17
   * while a unified-event callback owns this observer's shared lifetime lease.
   */
  pollEventV1Leased(lease:CallbackLease,capacity:i32):Reply {
    return this.callbackLease!==lease?new Reply(Status.Conflict):this.pollUnifiedOnce(capacity);
  }
  private pollUnifiedOnce(capacity:i32):Reply {
    if(!this.owned)return new Reply(Status.BadHandle);
    if(capacity<0||capacity>4194304)return new Reply(Status.Invalid);
    const output=new Uint8Array(capacity),slot=new Uint8Array(8);store<u64>(slot.dataStart,SENTINEL);
    return this.finish(this.api.pollEventV1(this.handle,0,<u64>output.dataStart,<u64>capacity,<u64>slot.dataStart),output,slot);
  }
  /** Copy32 readiness bytes; positive credit is only a snapshot. */
  eventReadiness():Reply {
    const status=this.retirementStatus();if(status)return new Reply(status);const bytes=new Uint8Array(32);
    const error=this.api.eventReady(this.handle,0,<u64>bytes.dataStart);return new Reply(error,error?new Uint8Array(0):bytes);
  }
  /** Submit from this service source through a distinct live observer. The
   * HandleReply contains a requestID after admission, never a capability handle.
   * Completion is an independently authorized response/error event. No thread,
   * implicit clock or automatic timeout replay is introduced by the SDK.
   */
  submitService(observer:Capability,input:Uint8Array):HandleReply {
    let status=this.retirementStatus();if(status)return new HandleReply(status,0);status=observer.retirementStatus();if(status)return new HandleReply(status,0);
    if(this.api!==observer.api||this.handle==observer.handle)return new HandleReply(Status.Invalid,0);
    const slot=new Uint8Array(8);store<u64>(slot.dataStart,0);
    status=this.api.submitService(this.handle,observer.handle,0,<u64>input.dataStart,<u64>input.length,<u64>slot.dataStart);
    const id=status?0:load<u64>(slot.dataStart);return new HandleReply(!status&&!id?Status.Invalid:status,id);
  }
  /** Perform one request with caller-selected bounded capacity. Never retry or resize after 90.
   * An event may already be removed from its queue when the output does not fit.
   */
  request(operation: Operation, input: Uint8Array = new Uint8Array(0), capacity: i32 = 4096): Reply { if (!this.owned) return new Reply(this.openStatus ? this.openStatus : Status.BadHandle); if (!this.api.supports(operation)) return new Reply(Status.Unsupported); if (capacity < 0 || capacity > MAX_WIRE || input.length > MAX_WIRE) return new Reply(Status.Invalid); const output = new Uint8Array(capacity), slot = new Uint8Array(8); store<u64>(slot.dataStart, SENTINEL); const status = this.api.request(operation, this.handle, 0, <u64>input.dataStart, <u64>input.length, <u64>output.dataStart, <u64>capacity, <u64>slot.dataStart); return this.finish(status, output, slot); }
  /** Poll one event using fixed capacity. While typed callback dispatch owns a
   * lease, this public method returns17 before allocation or another dequeue.
   */
  poll(capacity: i32 = 4096): Reply { if(this.callbackLease) return new Reply(Status.Conflict); return this.pollOnce(capacity); }
  /** Internal first poll for the exact token held by typed dispatch. A caller
   * cannot mint a second token while busy or borrow the handler's private token.
   */
  pollLeased(lease:CallbackLease,capacity:i32): Reply { if(this.callbackLease!==lease) return new Reply(Status.Conflict); return this.pollOnce(capacity); }
  /** Ordinary single import after the caller's exact lease/ownership checks. */
  private pollOnce(capacity:i32): Reply { if (!this.owned) return new Reply(Status.BadHandle); if (capacity < 0 || capacity > MAX_WIRE) return new Reply(Status.Invalid); const output = new Uint8Array(capacity), slot = new Uint8Array(8); store<u64>(slot.dataStart, SENTINEL); const status = this.api.poll(this.handle, 0, <u64>output.dataStart, <u64>capacity, <u64>slot.dataStart); return this.finish(status, output, slot); }
}

/** Local polling contract. No native ROS callback is registered by implementing this interface. */
export interface Pollable { spinOnce(): u32; }
/** Unified-event entity accepted by Executor.add. It borrows independently
 * opened source/observer owners and performs one explicit watch, with no legacy
 * queue activation or hidden alias. Both owners remain caller-owned on failure.
 * No native callback or managed pointer crosses this copied polling boundary.
 */
export class EventSubscription implements Pollable {
  /** Checked construction status; inspect before registering with an Executor. */
  status:u32=Status.Success;
  private binding:EventBinding;
  constructor(private observer:Capability,private source:Capability,
      private handler:(event:UnifiedEvent)=>u32,private capacity:i32=4096,correlation:u64=0){
    this.binding=new EventBinding(source.api.backend,source.handle,correlation);
    // AssemblyScript constructors must complete with their instance value.
    // Invalid local bindings never issue a watch or acquire native authority.
    if(capacity<64||capacity>4194304||!this.binding.valid){this.status=Status.Invalid;}
    else{this.status=observer.watchEvents(source,correlation);}
  }
  /** Snapshot callback per pass. Reassignment affects the next delivery only. */
  onEvent(handler:(event:UnifiedEvent)=>u32):void{this.handler=handler;}
  /** Dispatch at most one completely decoded event;11 is idle,90 retained size.
   * The observer's shared lease spans decode and callback. Direct poll/cleanup,
   * Node/Session retirement and recursive spin return17 before another dequeue.
   * A fatal Wasm trap cannot promise lease cleanup. Copy values before retention.
   */
  spinOnce():u32{
    if(this.status)return this.status;const lease=this.observer.enterCallback();
    if(lease===null)return this.observer.retirementStatus();const status=this.dispatch(lease);
    this.observer.leaveCallback(lease);return status;
  }
  private dispatch(lease:CallbackLease):u32{
    const callback=this.handler;const reply=this.observer.pollEventV1Leased(lease,this.capacity);if(reply.status)return reply.status;
    const event=this.binding.decode(reply.bytes);if(event===null)return Status.Invalid;return callback(event);
  }
  /** Native service admission, never a deferred blocking RPC. Match its owned
   * ID with a later response/error event; observer response/error grants are explicit.
   */
  submit(input:Uint8Array):HandleReply{return this.source.submitService(this.observer,input);}
  /** Finite credit only; a later submit can still return11 without dispatch. */
  readiness():Reply{return this.observer.eventReadiness();}
  /** Close only the observer. Keep the source independently closeable. */
  close():u32{return this.observer.close();}
  /** Revoke only the observer alias, never an inferred source/cancel-any grant. */
  revoke():u32{return this.observer.revoke();}
}
/** Typed publisher preserving the exact alias/type binding. */
export class Publisher<T> {
  /** Codec controls canonical bytes; it cannot change the native resource/type selected by the capability. */
  constructor(public capability: Capability, public codec: Codec<T>) {}
  /** Encode and publish once. Empty invalid encodings are rejected by runtime validators. */
  publish(value: T): u32 { return this.capability.publish(this.codec.encode(value)); }
  /** Close the explicitly owned publisher handle once. */
  close(): u32 { return this.capability.close(); }
  /** Revoke this exact alias once; failed notification does not restore authority. */
  revoke(): u32 { return this.capability.revoke(); }
  /** Borrow a checked live opaque ID without transferring ownership. */
  rawHandle(): HandleReply { return this.capability.rawHandle(); }
}
/** Typed polling subscription with a guest-local handler and fixed receive capacity. */
export class Subscription<T> implements Pollable {
  /** Initialization status; nonzero means no successful native subscription. */
  status: u32;
  /** A callback is called only from spinOnce after decode, never from native ROS. */
  constructor(public capability: Capability, public codec: Codec<T>, private handler: (value: T) => void, private capacity: i32 = 4096) { this.status = capability.isOpen() ? capability.subscribe() : capability.openStatus; if (this.status && capability.isOpen()) capability.close(); }
  /** Poll at most one event, validate the complete wire, then invoke the guest callback. */
  spinOnce(): u32 { if(this.status) return this.status; const lease=this.capability.enterCallback(); if(!lease) return this.capability.retirementStatus(); const status=this.dispatchOnce(lease); this.capability.leaveCallback(lease); return status; }
  /** Validate copied bytes before dispatch; the shared owner lease remains held
   * until the handler and this pass finish, including normal idle/error returns.
   */
  private dispatchOnce(lease:CallbackLease): u32 { const r=this.capability.pollLeased(lease,this.capacity); if(r.status) return r.status; if(!this.codec.valid(r.bytes)) return Status.Invalid; this.handler(this.codec.decode(r.bytes)); return Status.Success; }
  /** Close the owned handle once. */
  close(): u32 { return this.capability.close(); }
  /** Revoke this exact alias once; failed notification does not restore authority. */
  revoke(): u32 { return this.capability.revoke(); }
  /** Borrow a checked live opaque ID without transferring ownership. */
  rawHandle(): HandleReply { return this.capability.rawHandle(); }
}
/** Single-thread bounded executor; the caller explicitly supplies scheduling and time. */
export class Executor {
  private entities: Array<Pollable> = new Array<Pollable>();
  private spinning: bool = false;
  private closed: bool = false;
  /** Local registration budget; no native quota is increased. */
  constructor(private maximum: i32 = 1024) {}
  /** Register a guest-local entity; this method opens no capabilities.
   * Registrations made by a handler are deferred to the next outer spin pass.
   */
  add(entity: Pollable): u32 { if (this.closed) return Status.BadHandle; if (this.maximum < 1 || this.maximum > 1024) return Status.Invalid; if (this.entities.length >= this.maximum) return Status.TooLarge; this.entities.push(entity); return Status.Success; }
  /** Remove one registration locally; ownership stays with Node/Session. Mutation during a pass is17. */
  remove(index: i32): u32 { if (this.closed) return Status.BadHandle; if (this.spinning) return Status.Conflict; if (index < 0 || index >= this.entities.length) return Status.Invalid; this.entities.splice(index,1); return Status.Success; }
  /** Disable dispatch and discard registrations once. This generic Pollable executor does not own native handles; close the Node/Session separately. */
  close(): u32 { if (this.closed) return Status.BadHandle; if (this.spinning) return Status.Conflict; this.closed = true; this.entities.length = 0; return Status.Success; }
  /** Poll only the registrations present at pass entry, at most once each.
   * A nested spin returns [Conflict] without polling any entity. A handler may
   * add entities, but they never extend the current pass. This creates no thread,
   * clock read or Again retry. An unrecoverable guest trap terminates the pass.
   */
  spinOnce(): Array<u32> { if (this.closed) return [Status.BadHandle]; if (this.spinning) return [Status.Conflict]; const count = this.entities.length; this.spinning = true; const out = new Array<u32>(); for (let i = 0; i < count; ++i) out.push(this.entities[i].spinOnce()); this.spinning = false; return out; }
}
/** Typed Trigger/SetBool service client with one service.call capability. */
export class Client {
  /** Fixed output bound; runtime timeout and quotas remain authoritative. */
  constructor(public capability: Capability, private capacity: i32 = 4096) {}
  /** Trigger request is zero bytes; the result status is unchanged. */
  trigger(): Reply { return this.capability.request(Operation.service_call, new Uint8Array(0), this.capacity); }
  /** SetBool request is one canonical Bool byte. */
  setBool(value: bool): Reply { return this.capability.request(Operation.service_call, new BoolCodec().encode(value), this.capacity); }
  /** Decode successful Trigger/SetBool bytes; result.valid rejects malformed envelopes. */
  response(reply: Reply): ServiceResponse { const value = decodeServiceResponse(reply.bytes); value.valid = value.valid && reply.status == 0; return value; }
  /** Close the owned service.call handle once. */
  close(): u32 { return this.capability.close(); }
  /** Revoke this exact alias once; failed notification does not restore authority. */
  revoke(): u32 { return this.capability.revoke(); }
  /** Borrow a checked live opaque ID without transferring ownership. */
  rawHandle(): HandleReply { return this.capability.rawHandle(); }
}
/** Poll-driven service server with separate advertise/respond authority. */
export class Service implements Pollable {
  status: u32;
  private spinning: bool = false;
  /** Handler sees an owned request ID and raw typed request bytes; decode Trigger/SetBool as appropriate. */
  constructor(public advertise: Capability, public respond: Capability, private handler: (request: ServiceRequest) => ServiceResponse, private capacity: i32 = 4096) { this.status = !advertise.isOpen() ? (advertise.openStatus ? advertise.openStatus : Status.BadHandle) : !respond.isOpen() ? (respond.openStatus ? respond.openStatus : Status.BadHandle) : advertise.advertiseService(); }
  /** Handle one copied request and emit one owned-ID response. No retry occurs on native failure. */
  spinOnce(): u32 { if(this.spinning) return Status.Conflict; if(this.status) return this.status; const first=this.advertise.enterCallback(); if(!first) return this.advertise.retirementStatus(); const second=this.respond.enterCallback(); if(!second) { this.advertise.leaveCallback(first); return this.respond.retirementStatus(); } this.spinning=true; const status=this.dispatchOnce(first); this.spinning=false; this.respond.leaveCallback(second); this.advertise.leaveCallback(first); return status; }
  /** Internal pass; the public guard refuses recursive polling before consuming another event. */
  private dispatchOnce(lease:CallbackLease): u32 { if (this.status) return this.status; const r = this.advertise.pollLeased(lease,this.capacity); if (r.status) return r.status; const request = decodeServiceRequest(r.bytes); if (!request.valid) return Status.Invalid; const response=this.handler(request); if(!response.valid) return Status.Invalid; const wire=encodeServiceReply(request.id,response); if(wire.length<13 || !decodeServiceResponse(wire.slice(8)).valid) return Status.Invalid; return this.respond.request(Operation.service_respond,wire,this.capacity).status; }
  /** Close both owned aliases once, returning each status separately. */
  close(): Array<u32> { if(this.spinning || this.advertise.retirementStatus()==Status.Conflict || this.respond.retirementStatus()==Status.Conflict) return [Status.Conflict]; this.status=Status.BadHandle; return [this.advertise.close(), this.respond.close()]; }
  /** Revoke both independently granted aliases, attempting both and retaining every status. */
  revoke(): Array<u32> { if(this.spinning || this.advertise.retirementStatus()==Status.Conflict || this.respond.retirementStatus()==Status.Conflict) return [Status.Conflict]; this.status=Status.BadHandle; return [this.advertise.revoke(),this.respond.revoke()]; }
  /** Borrow both checked full-width event/respond IDs; neither becomes a permission. */
  rawHandles(): Array<HandleReply> { return [this.advertise.rawHandle(),this.respond.rawHandle()]; }
}
/** Fibonacci Action client: each operation has its own exact capability. */
export class ActionClient {
  /** Nonzero when any explicitly supplied operation capability failed to open. No partial client is activated. */
  status: u32 = 0;
  /** Only send is required. Cancel-any/cancel-all needs its own explicitly supplied optional capability. */
  constructor(public send: Capability, public cancel: Capability | null = null, public feedbackCapability: Capability | null = null, public resultCapability: Capability | null = null, private capacity: i32 = 4096, public cancelAnyCapability: Capability | null = null) { if (!send.isOpen()) this.status = send.openStatus ? send.openStatus : Status.BadHandle; const optional: Array<Capability | null> = [cancel, feedbackCapability, resultCapability, cancelAnyCapability]; for (let i = 0; i < optional.length; ++i) { const cap = optional[i]; if (cap && !cap.isOpen() && !this.status) this.status = cap.openStatus ? cap.openStatus : Status.BadHandle; } }
  /** Send one bounded order; successful response contains an opaque u64 goal ID. */
  sendGoal(order: i32): Reply { if (this.status) return new Reply(this.status); const request = encodeFibonacciGoal(order); return request.length ? this.send.request(Operation.action_send_goal, request, this.capacity) : new Reply(Status.Invalid); }
  /** Extract a successful nonzero goal ID; zero means invalid/error. */
  goalId(reply: Reply): u64 { return reply.status ? 0 : decodeGoalId(reply.bytes); }
  /** Cancel only a goal owned by the explicit cancel-own capability. */
  cancelOwn(id: u64): Reply { if (this.status) return new Reply(this.status); const cap = this.cancel; if (!cap) return new Reply(Status.Denied); return id ? cap.request(Operation.action_cancel_own_goal, encodeGoalId(id), this.capacity) : new Reply(Status.Invalid); }
  /** Cancel another goal only with explicit cancel-any authority. ID zero is ABI's cancel-all selector. */
  cancelAny(id: u64): Reply { if (this.status) return new Reply(this.status); const cap = this.cancelAnyCapability; if (!cap) return new Reply(Status.Denied); return cap.request(Operation.action_cancel_any_goal, new Writer(8).u64(id).finish(), this.capacity); }
  /** Cancel all goals using only the explicitly supplied cancel-any-goal capability. */
  cancelAll(): Reply { return this.cancelAny(0); }
  /** Fetch one feedback sequence, whose wire omits kind. */
  feedback(id: u64): Reply { if (this.status) return new Reply(this.status); const cap = this.feedbackCapability; if (!cap) return new Reply(Status.Denied); return id ? cap.request(Operation.action_receive_feedback, encodeGoalId(id), this.capacity) : new Reply(Status.Invalid); }
  /** Fetch one terminal sequence, whose kind encodes success/abort/cancel. */
  result(id: u64): Reply { if (this.status) return new Reply(this.status); const cap = this.resultCapability; if (!cap) return new Reply(Status.Denied); return id ? cap.request(Operation.action_receive_result, encodeGoalId(id), this.capacity) : new Reply(Status.Invalid); }
  /** Decode copied feedback and check the exact requested owned ID. Preserve
   * the original Reply separately to distinguish idle11, timeout and denial.
   */
  feedbackValue(id: u64, reply: Reply): ActionSequence { const value=decodeActionSequence(reply.bytes,false); value.valid=value.valid && !reply.status && id!=0 && value.id==id; return value; }
  /** Decode a copied terminal result without confusing it with feedback. */
  resultValue(id: u64, reply: Reply): ActionSequence { const value=decodeActionSequence(reply.bytes,true); value.valid=value.valid && !reply.status && id!=0 && value.id==id && value.kind>=2 && value.kind<=4; return value; }
  /** Close only supplied, still-owned handles once; optional cancellation authority is never required. */
  close(): Array<u32> { this.status=Status.BadHandle; const out = new Array<u32>(); if (this.send.isOpen()) out.push(this.send.close()); const cancel = this.cancel; if (cancel && cancel.isOpen()) out.push(cancel.close()); const feedback = this.feedbackCapability; if (feedback && feedback.isOpen()) out.push(feedback.close()); const result = this.resultCapability; if (result && result.isOpen()) out.push(result.close()); const any = this.cancelAnyCapability; if (any && any.isOpen()) out.push(any.close()); return out.length?out:[Status.BadHandle]; }
  /** Revoke every supplied live operation once. Missing sibling grants remain
   * absent; this does not invent cancel-any, feedback or result authority.
   */
  revoke(): Array<u32> { this.status=Status.BadHandle; const out=new Array<u32>(); const handles:Array<Capability|null>=[this.send,this.cancel,this.feedbackCapability,this.resultCapability,this.cancelAnyCapability]; for(let i=0;i<handles.length;++i) { const cap=handles[i]; if(cap && cap.isOpen()) out.push(cap.revoke()); } return out.length?out:[Status.BadHandle]; }
}
/** Fibonacci Action server with local polling and exact advertise/respond capabilities. */
export class ActionServer implements Pollable {
  status: u32;
  private spinning: bool = false;
  /** Callback returns one feedback or terminal ActionSequence using the incoming goal ID. */
  constructor(public advertise: Capability, public respond: Capability, private handler: (goal: IncomingGoal) => ActionSequence, private capacity: i32 = 4096) { this.status = !advertise.isOpen() ? (advertise.openStatus ? advertise.openStatus : Status.BadHandle) : !respond.isOpen() ? (respond.openStatus ? respond.openStatus : Status.BadHandle) : advertise.request(Operation.action_advertise, new Uint8Array(0), capacity).status; }
  /** Process at most one incoming goal; enforce response ID identity before native dispatch. */
  spinOnce(): u32 { if(this.spinning) return Status.Conflict; if(this.status) return this.status; const first=this.advertise.enterCallback(); if(!first) return this.advertise.retirementStatus(); const second=this.respond.enterCallback(); if(!second) { this.advertise.leaveCallback(first); return this.respond.retirementStatus(); } this.spinning=true; const status=this.dispatchOnce(first); this.spinning=false; this.respond.leaveCallback(second); this.advertise.leaveCallback(first); return status; }
  /** Internal pass; the public guard refuses recursive polling before consuming another event. */
  private dispatchOnce(lease:CallbackLease): u32 { if (this.status) return this.status; const r = this.advertise.pollLeased(lease,this.capacity); if (r.status) return r.status; const goal = decodeIncomingGoal(r.bytes); if (!goal.valid) return Status.Invalid; const response = this.handler(goal); if (response.id != goal.id) return Status.Invalid; const wire = encodeActionSequence(response); return wire.length ? this.respond.request(Operation.action_respond, wire, this.capacity).status : Status.Invalid; }
  /** Close both handles once. */
  close(): Array<u32> { if(this.spinning || this.advertise.retirementStatus()==Status.Conflict || this.respond.retirementStatus()==Status.Conflict) return [Status.Conflict]; this.status=Status.BadHandle; return [this.advertise.close(), this.respond.close()]; }
  /** Revoke both independently granted aliases, attempting both and retaining every status. */
  revoke(): Array<u32> { if(this.spinning || this.advertise.retirementStatus()==Status.Conflict || this.respond.retirementStatus()==Status.Conflict) return [Status.Conflict]; this.status=Status.BadHandle; return [this.advertise.revoke(),this.respond.revoke()]; }
  /** Borrow both checked full-width event/respond IDs; neither becomes a permission. */
  rawHandles(): Array<HandleReply> { return [this.advertise.rawHandle(),this.respond.rawHandle()]; }
}
/** One exact request operation for all remaining ABI resources. */
export class Resource {
  /** operation selects a registered import; the runtime still checks that the handle grants it. */
  constructor(public capability: Capability, public operation: Operation, public capacity: i32 = 4096) {}
  /** Perform one request, copying output and retaining an unknown/known required-length distinction. */
  call(bytes: Uint8Array = new Uint8Array(0)): Reply { return this.capability.request(this.operation, bytes, this.capacity); }
  /** Close the owned exact-operation handle. */
  close(): u32 { return this.capability.close(); }
  /** Revoke this exact alias once; failed notification does not restore authority. */
  revoke(): u32 { return this.capability.revoke(); }
  /** Borrow a checked live opaque ID without transferring ownership. */
  rawHandle(): HandleReply { return this.capability.rawHandle(); }
}
/** Typed parameter resource; instantiate one object per explicitly provisioned operation alias. */
export class Parameters extends Resource {
  /** The configured kind must match the capability's builtin parameter type. */
  constructor(capability: Capability, operation: Operation, public kind: ParameterKind, capacity: i32 = 4096) { super(capability, operation, capacity); }
  /** Read/decode one parameter through parameter_get. Inspection/list use readText instead. */
  get(): ParameterValue { return this.decode(this.getReply()); }
  /** Preserve the actual runtime status/required-length contract before decoding get() bytes. */
  getReply(): Reply { return this.operation == Operation.parameter_get ? this.call() : new Reply(Status.Invalid); }
  /** Canonical parameter call; event subscription/polling use their dedicated methods. */
  call(bytes: Uint8Array = new Uint8Array(0)): Reply { return parameterOperation(this.operation) && this.operation != Operation.parameter_subscribe_events ? super.call(bytes) : new Reply(Status.Invalid); }
  /** Decode a previously obtained reply; its status remains available to the caller. */
  decode(reply: Reply): ParameterValue { const value = decodeParameter(reply.bytes, this.kind); value.valid = value.valid && reply.status == 0; return value; }
  /** Set only through parameter_set; selector/kind errors fail before host dispatch. */
  set(value: ParameterValue): Reply { if (this.operation != Operation.parameter_set || value.kind != this.kind) return new Reply(Status.Invalid); const bytes = encodeParameter(value); return bytes.length ? this.call(bytes) : new Reply(Status.Invalid); }
  /** Declare only through the explicit ROS 2 parameter_declare selector.
   * Use declareValue because declare is a reserved AssemblyScript keyword.
   * The renamed facade preserves the exact operation/kind checks and does not
   * acquire declaration authority or change the native parameter_declare ABI.
   */
  declareValue(value: ParameterValue): Reply { if (this.operation != Operation.parameter_declare || value.kind != this.kind) return new Reply(Status.Invalid); const bytes = encodeParameter(value); return bytes.length ? this.call(bytes) : new Reply(Status.Invalid); }
  /** Delete only through the explicit ROS 1 parameter_delete selector. */
  delete(): Reply { return this.operation == Operation.parameter_delete ? this.call() : new Reply(Status.Invalid); }
  /** Undeclare only through the explicit ROS 2 selector. */
  undeclare(): Reply { return this.operation == Operation.parameter_undeclare ? this.call() : new Reply(Status.Invalid); }
  /** Existence queries never substitute a parameter mutation. */
  exists(): Reply { return this.operation == Operation.parameter_exists ? this.call() : new Reply(Status.Invalid); }
  /** List only through the exact permitted list selector. */
  list(): Reply { return this.operation == Operation.parameter_list ? this.call() : new Reply(Status.Invalid); }
  /** Describe only through the exact metadata selector. */
  describe(): Reply { return this.operation == Operation.parameter_describe ? this.call() : new Reply(Status.Invalid); }
  /** Search only through the explicit ROS 1 selector. */
  search(): Reply { return this.operation == Operation.parameter_search ? this.call() : new Reply(Status.Invalid); }
  /** Read canonical list/describe/search text, represented by a single length-prefixed String. */
  readText(): string { const r = this.readTextReply(), codec = new StringCodec(); return !r.status && codec.valid(r.bytes) ? codec.decode(r.bytes) : ""; }
  /** Obtain list/describe/search bytes without conflating an empty String with denial/disconnection. */
  readTextReply(): Reply { return this.operation == Operation.parameter_list || this.operation == Operation.parameter_describe || this.operation == Operation.parameter_search ? this.call() : new Reply(Status.Invalid); }
  /** Subscribe to ROS 2 parameter events with the exact events capability. */
  subscribeEvents(): u32 { return this.operation == Operation.parameter_subscribe_events ? this.capability.subscribeParameterEvents() : Status.Invalid; }
  /** Poll one typed event; its valid flag also includes the runtime status. */
  pollEvent(): TypedParameterEvent { const r = this.pollEventReply(), v = decodeTypedParameterEvent(r.bytes); v.valid = v.valid && r.status == 0; return v; }
  /** Retain poll status, and reject a non-events facade before any host poll. */
  pollEventReply(): Reply { return this.operation == Operation.parameter_subscribe_events ? this.capability.poll(this.capacity) : new Reply(Status.Invalid); }
}
/** One graph operation; native name filters and quotas come from the configured capability. */
export class Graph extends Resource {
  /** Reject non-graph selectors/nonempty queries before any host call. */
  call(bytes: Uint8Array = new Uint8Array(0)): Reply { return graphOperation(this.operation) && !bytes.length ? super.call(bytes) : new Reply(Status.Invalid); }
  /** Return canonical newline/tab row text. This is one String, not a String-array parameter. */
  readText(): Reply { return this.call(); }
  /** Decode only a successful complete String response. Empty text may mean no permitted rows. */
  text(reply: Reply): string { const c = new StringCodec(); return !reply.status && c.valid(reply.bytes) ? c.decode(reply.bytes) : ""; }
}
/** One explicitly authorized clock kind. No scheduling clock is opened implicitly. */
export class Clock extends Resource {
  /** Reject a non-clock selector/nonempty request locally. */
  call(bytes: Uint8Array = new Uint8Array(0)): Reply { return clockOperation(this.operation) && !bytes.length ? super.call(bytes) : new Reply(Status.Invalid); }
  /** Read exact eight-byte unsigned nanoseconds; caller checks status and byte length. */
  now(): Reply { return this.call(); }
  /** Decode successful nanoseconds; zero is also a valid ROS simulated-time value. */
  nanoseconds(reply: Reply): u64 { return !reply.status && reply.bytes.length == 8 ? new Reader(reply.bytes).u64() : 0; }
}
/** TF resource for a configured frame pair and exact operation. */
export class Tf extends Resource {
  /** Expose only the selected TF operation through the canonical byte-level method. */
  call(bytes: Uint8Array = new Uint8Array(0)): Reply { return tfOperation(this.operation) ? super.call(bytes) : new Reply(Status.Invalid); }
  /** Request the configured frame pair at a timestamp; zero selects latest. */
  lookup(nanoseconds: u64 = 0): Reply { return this.operation == Operation.tf_lookup ? this.call(new Writer(8).u64(nanoseconds).finish()) : new Reply(Status.Invalid); }
  /** Decode a complete successful transform. */
  transform(reply: Reply): Transform { const v = decodeTransform(reply.bytes); v.valid = v.valid && reply.status == 0; return v; }
  /** Broadcast canonical bytes through explicit tf.broadcast authority. */
  broadcast(value: Transform): Reply { return this.operation == Operation.tf_broadcast ? this.call(encodeTransform(value)) : new Reply(Status.Invalid); }
}
/** ROS 2 lifecycle resource; operation/allowed-transition is provisioned by policy. */
export class Lifecycle extends Resource {
  /** Lifecycle requests are empty and use only the selected exact-node operation. */
  call(bytes: Uint8Array = new Uint8Array(0)): Reply { return lifecycleOperation(this.operation) && !bytes.length ? super.call(bytes) : new Reply(Status.Invalid); }
  /** GetState uses an empty request and canonical state/label response. */
  getState(): LifecycleState { const r = this.getStateReply(), v = decodeLifecycleState(r.bytes); v.valid = v.valid && r.status == 0; return v; }
  /** Preserve status and reject a change-state facade before a named get-state call. */
  getStateReply(): Reply { return this.operation == Operation.lifecycle_get_state ? this.call() : new Reply(Status.Invalid); }
  /** ChangeState uses an empty request: the transition is fixed in the capability. */
  changeState(): Reply { return this.operation == Operation.lifecycle_change_state ? this.call() : new Reply(Status.Invalid); }
}
/** Explicitly authorized logger; log text is a canonical String, not bare UTF-8. */
export class Logger {
  /** No default logging capability or severity is selected by the SDK. */
  constructor(public capability: Capability) {}
  /** Copy one UTF-8 length-prefixed log string through log.write authority. */
  write(text: string): u32 { return this.capability.log(new StringCodec().encode(text)); }
  /** Close the owned log capability. */
  close(): u32 { return this.capability.close(); }
  /** Revoke this exact alias once; failed notification does not restore authority. */
  revoke(): u32 { return this.capability.revoke(); }
  /** Borrow a checked live opaque ID without transferring ownership. */
  rawHandle(): HandleReply { return this.capability.rawHandle(); }
}
/** Internal acquisition transaction: failed factory handles are closed before retry. */
class FactoryHandles {
  status: u32 = 0;
  handles: Array<Capability> = new Array<Capability>();
  /** Release every successfully acquired handle once, preserving the original factory status. */
  rollback(): void { for (let i = 0; i < this.handles.length; ++i) if (this.handles[i].isOpen()) this.handles[i].close(); }
}
/** Guest-local Node organizer resembling familiar ROS client-library concepts. */
export class Node {
  private owned: Array<Capability> = new Array<Capability>();
  private closed: bool = false;
  /** Local label does not change the guest's authenticated runtime principal. */
  constructor(private selectedApi: Api, public name: string = "roswasm", private session: Session | null = null) {}
  /** Read the fixed adapter; a local label cannot change the native backend. */
  get api(): Api { return this.selectedApi; }
  /** Acquire one capability before remembering it. Session state is shared, so
   * closing a group retires every retained typed facade without another import.
   */
  private acquireOne(alias: string): Capability {
    const cap = new Capability(this.api);
    if (this.closed) { cap.openStatus = Status.BadHandle; return cap; }
    for (let i=this.owned.length-1; i>=0; --i) if (!this.owned[i].isOpen()) this.owned.splice(i,1);
    if (this.owned.length >= 1024) { cap.openStatus = Status.TooLarge; return cap; }
    const group = this.session;
    if (group) return group.open(alias);
    cap.open(alias); return cap;
  }
  /** Open exactly one CLI-configured alias; inspect openStatus before use. */
  open(alias: string): Capability { const cap = this.acquireOne(alias); if (cap.isOpen()) this.owned.push(cap); return cap; }
  /** Acquire all requested aliases before any native advertiser can be activated. */
  private acquire(aliases: Array<string>): FactoryHandles { const result = new FactoryHandles(); for (let i=this.owned.length-1;i>=0;--i) if(!this.owned[i].isOpen()) this.owned.splice(i,1); if(this.owned.length+aliases.length>1024) result.status=Status.TooLarge; for (let i = 0; i < aliases.length; ++i) { const cap = !result.status ? this.acquireOne(aliases[i]) : new Capability(this.api); if (!result.status) result.status = cap.openStatus; else cap.openStatus = result.status; result.handles.push(cap); } if (result.status) result.rollback(); return result; }
  /** Remember handles only after complete factory success. */
  private commit(result: FactoryHandles): void { for (let i = 0; i < result.handles.length; ++i) if (result.handles[i].isOpen()) this.owned.push(result.handles[i]); }
  /** Reject an unregistered backend operation before attempting to open its alias. */
  openFor(alias: string, operation: Operation): Capability { if (this.closed) { const cap = new Capability(this.api); cap.openStatus = Status.BadHandle; return cap; } if (this.api.supports(operation)) return this.open(alias); const cap = new Capability(this.api); cap.openStatus = Status.Unsupported; return cap; }
  /** Create a failed local facade without any host open/call. */
  private invalidCapability(): Capability { const cap = new Capability(this.api); cap.openStatus = this.closed?Status.BadHandle:Status.Invalid; return cap; }
  /** Create a typed publisher with explicit topic.publish authority. */
  createPublisher<T>(alias: string, codec: Codec<T>): Publisher<T> { return new Publisher<T>(this.open(alias), codec); }
  /** Create a typed guest-polling subscription; handler never enters native ROS. */
  createSubscription<T>(alias: string, codec: Codec<T>, handler: (value: T) => void, capacity: i32 = 4096): Subscription<T> { return new Subscription<T>(this.open(alias), codec, handler, capacity); }
  /** Create a typed Trigger/SetBool client from one service.call alias. */
  createClient(alias: string, capacity: i32 = 4096): Client { return new Client(this.open(alias), capacity); }
  /** Create a polling service using independently provisioned advertise/respond aliases. */
  createService(advertiseAlias: string, respondAlias: string, handler: (request: ServiceRequest) => ServiceResponse, capacity: i32 = 4096): Service { const acquired = this.acquire([advertiseAlias, respondAlias]), service = new Service(acquired.handles[0], acquired.handles[1], handler, capacity); if (acquired.status) service.status = acquired.status; if (service.status) acquired.rollback(); else this.commit(acquired); return service; }
  /** Open send authority and only the optional Action aliases explicitly supplied by the application. */
  createActionClient(sendAlias: string, cancelAlias: string = "", feedbackAlias: string = "", resultAlias: string = "", capacity: i32 = 4096, cancelAnyAlias: string = ""): ActionClient { const aliases: Array<string> = [sendAlias]; if (cancelAlias.length) aliases.push(cancelAlias); if (feedbackAlias.length) aliases.push(feedbackAlias); if (resultAlias.length) aliases.push(resultAlias); if (cancelAnyAlias.length) aliases.push(cancelAnyAlias); const acquired = this.acquire(aliases); let index = 1; const cancel = cancelAlias.length ? acquired.handles[index++] : null, feedback = feedbackAlias.length ? acquired.handles[index++] : null, result = resultAlias.length ? acquired.handles[index++] : null, any = cancelAnyAlias.length ? acquired.handles[index++] : null; const client = new ActionClient(acquired.handles[0], cancel, feedback, result, capacity, any); if (acquired.status) client.status = acquired.status; if (client.status) acquired.rollback(); else this.commit(acquired); return client; }
  /** Create a polling Action server from distinct advertise/respond authority. */
  createActionServer(advertiseAlias: string, respondAlias: string, handler: (goal: IncomingGoal) => ActionSequence, capacity: i32 = 4096): ActionServer { const acquired = this.acquire([advertiseAlias, respondAlias]), server = new ActionServer(acquired.handles[0], acquired.handles[1], handler, capacity); if (acquired.status) server.status = acquired.status; if (server.status) acquired.rollback(); else this.commit(acquired); return server; }
  /** Open one exact typed parameter operation; kind is fixed by the configured alias's type. */
  createParameters(alias: string, operation: Operation, kind: ParameterKind, capacity: i32 = 4096): Parameters { return new Parameters(parameterOperation(operation) ? this.openFor(alias, operation) : this.invalidCapability(), operation, kind, capacity); }
  /** Open one exact filtered graph operation. */
  createGraph(alias: string, operation: Operation, capacity: i32 = 4096): Graph { return new Graph(graphOperation(operation) ? this.openFor(alias, operation) : this.invalidCapability(), operation, capacity); }
  /** Open one explicitly authorized clock kind. The executor opens no clock by default. */
  createClock(alias: string, operation: Operation = Operation.time_read_ros, capacity: i32 = 4096): Clock { return new Clock(clockOperation(operation) ? this.openFor(alias, operation) : this.invalidCapability(), operation, capacity); }
  /** Open one configured transform operation for the bound frame pair. */
  createTf(alias: string, operation: Operation, capacity: i32 = 4096): Tf { return new Tf(tfOperation(operation) ? this.openFor(alias, operation) : this.invalidCapability(), operation, capacity); }
  /** Open a ROS 2 lifecycle operation. ROS 1 NativeApi dispatch rejects it with 95. */
  createLifecycle(alias: string, operation: Operation, capacity: i32 = 4096): Lifecycle { return new Lifecycle(lifecycleOperation(operation) ? this.openFor(alias, operation) : this.invalidCapability(), operation, capacity); }
  /** Create an exact request resource for any registered operation. */
  createResource(alias: string, operation: Operation, capacity: i32 = 4096): Resource { return new Resource(this.openFor(alias, operation), operation, capacity); }
  /** Create a logger from one explicit log.write alias. */
  createLogger(alias: string): Logger { return new Logger(this.open(alias)); }
  /** Close every still-owned handle once, including after backend close failures.
   * Busy callback17 refuses the whole group before retirement; owners and
   * factories remain available for explicit cleanup after the callback returns.
   */
  close(): Array<u32> { if(this.closed) return [Status.BadHandle]; for(let i=0;i<this.owned.length;++i) if(this.owned[i].isOpen() && this.owned[i].retirementStatus()==Status.Conflict) return [Status.Conflict]; this.closed=true; const statuses=new Array<u32>(); for(let i=0;i<this.owned.length;++i) if(this.owned[i].isOpen()) statuses.push(this.owned[i].close()); this.owned.length=0; return statuses; }
}

/** Checked full-width opaque handle view; it never grants or transfers ownership. */
export class HandleReply { constructor(public status: u32, public handle: u64) {} }

/** Bounded local ownership group over one explicit backend Api. This is not a
 * native ROS Context, connection or identity. Native policy remains default deny;
 * every factory opens its own configured alias with independent host quotas.
 */
export class Session {
  private owned: Array<Capability> = new Array<Capability>();
  private closed: bool = false;
  /** Initialization status; invalid local budget22 or unsupported ABI95. */
  status: u32 = Status.Success;
  /** Constructor opens nothing; select a live-capability budget of1..1024. */
  constructor(private selectedApi: Api, private maximum: i32 = 1024) {
    if (maximum < 1 || maximum > 1024) this.status=Status.Invalid;
    else if (selectedApi.version()!=1) this.status=Status.Unsupported;
  }
  /** Backend adapter remains fixed throughout this group's lifetime. */
  get api(): Api { return this.selectedApi; }
  /** Create a local named organizer attached to this ownership group. */
  node(name: string = "roswasm"): Node { return new Node(this.api,name,this); }
  /** Whether factories are locally enabled; no native connection/authority claim. */
  isOpen(): bool { return !this.closed && !this.status; }
  /** Open one explicit alias after pruning retired records and checking budget.
   * The returned capability is the same object retained by Node/typed facades.
   */
  open(alias: string): Capability {
    const cap = new Capability(this.api);
    if (this.closed || this.status) { cap.openStatus=this.closed?Status.BadHandle:this.status; return cap; }
    for (let i=this.owned.length-1; i>=0; --i) if (!this.owned[i].isOpen()) this.owned.splice(i,1);
    if (this.owned.length>=this.maximum) { cap.openStatus=Status.TooLarge; return cap; }
    cap.open(alias); if (cap.isOpen()) this.owned.push(cap); return cap;
  }
  /** Mark closed first, attempt every live handle once and return every status.
   * Inspect all results even after an operation fails. No finalizer or trap-time
   * cleanup is promised. Retained resources/Nodes report9 without reopening.
   * Busy callback17 preflights all owners and preserves the open group; call
   * checked cleanup after dispatch ends instead of retrying inside a handler.
   */
  close(): Array<u32> { if(this.closed) return [Status.BadHandle]; for(let i=0;i<this.owned.length;++i) if(this.owned[i].isOpen() && this.owned[i].retirementStatus()==Status.Conflict) return [Status.Conflict]; this.closed=true; const out=new Array<u32>(); for(let i=0;i<this.owned.length;++i) if(this.owned[i].isOpen()) out.push(this.owned[i].close()); this.owned.length=0; return out; }
}
