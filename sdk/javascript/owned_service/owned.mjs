/**
 * @file owned.mjs
 * @brief Caller-driven typed ROS2 Service endpoints over the REAL optional raw12.
 *
 * Constructors perform local preparation only. Call activate() explicitly after
 * retaining the endpoint in a finally/cleanup owner. Every source/observer or
 * advertise/responder is an independently granted genuine SDK Capability from
 * one ROS2 Session. Availability additionally requires its third distinct grant.
 * There is no business service_call/service_submit/event_poll fallback.
 *
 * Native identity, policy and scope are checked by the actual Runtime. Definition
 * strings, expected QoS, integer handles and JavaScript ownership are assertions,
 * never grants or Native SDK qualification. The production embedded builtin is
 * enabled separately with its exact catalog/authority/header source identities.
 */
import { Capability, Session } from '../roswasm.mjs';
import { Status, u64 } from '../raw.mjs';
import { acquirePolling } from '../ownership.mjs';
import { OwnedServiceBridge, prepareServiceCall, invokeServiceCall, serviceCallAttempted } from './bridge.mjs';
import { WireError, CLIENT, SERVICE, MAXIMUM_PACKET_BYTES, MAXIMUM_WAIT_NS,
  DELIVERY_PREFIX_BYTES, fullHandle, copyExpectedQos, decodeEndpointReceipt,
  sameEndpointReceipt, decodeDelivery } from './wire.mjs';

const states = new WeakMap(), definitions = new WeakMap();
const apply = Reflect.apply, capCheck = Capability.prototype.check, capClose = Capability.prototype.close;
const noArgs = [], empty = new Uint8Array();
const typed = Object.getPrototypeOf(Uint8Array.prototype);
const typedLength = Object.getOwnPropertyDescriptor(typed, 'length').get;
const typedSet = Uint8Array.prototype.set;
const FREE = 0, RESERVED = 1, ADMITTED = 2, DELIVERED = 3, DONE = 4, ABANDONED = 5, ABANDONED_DELIVERED = 6;
function need(value, message) { if (!value) throw new TypeError(message); }
function integer(value, low, high, message) { need(Number.isInteger(value) && value >= low && value <= high, message); return value; }
function dataProperty(object, name) {
  const d = Object.getOwnPropertyDescriptor(object, name);
  need(d && Object.prototype.hasOwnProperty.call(d, 'value'), 'Genuine unchanged SDK data property required'); return d.value;
}
function owner(cap) {
  need(cap instanceof Capability, 'Genuine SDK Capability required');
  apply(capCheck, cap, noArgs);
  const session = dataProperty(cap, 'session'), handle = dataProperty(cap, 'handle');
  need(session instanceof Session && session.version === 2 && fullHandle(handle), 'Live complete ROS2 capability handle required');
  return { cap, session, handle, close: capClose, cleanupAttempted: false, cleanupFinished: false,
    cleanupCode: null, cleanupError: null, cleanupThrew: false, externallyClosed: false, retained: true };
}
function current(row) {
  need(dataProperty(row.cap, 'session') === row.session && dataProperty(row.cap, 'handle') === row.handle,
    'Capability/session changed after endpoint preparation');
  apply(capCheck, row.cap, noArgs);
}
function checkedCodec(codec) {
  need(codec && typeof codec === 'object', 'Explicit bounded canonical codec required');
  // Capture once; no application getter or method lookup occurs after ACK.
  const encode = codec.encode, decode = codec.decode, maximum = codec.maxWireBytes, identity = codec.codecIdentity;
  need(typeof encode === 'function' && typeof decode === 'function' && typeof identity === 'string' && /^UWVMROS1_[0-9a-f]{64}$/.test(identity),
    'Actual complete canonical codec identity/encode/decode required');
  integer(maximum, 0, MAXIMUM_PACKET_BYTES - DELIVERY_PREFIX_BYTES, 'Explicit canonical wire maximum required');
  return Object.freeze({ object: codec, encode, decode, maximum, identity });
}
/** Bind the ORIGINAL Service root identity separately from its component IDs.
 * NativeIdentity may be null during source preparation. That absence is kept;
 * no hash or source-derived value is substituted as a Native certificate.
 */
export function serviceDefinition(identity, requestCodec, responseCodec) {
  need(identity && typeof identity === 'object', 'Original Service identity facts required');
  const nativeType = identity.nativeType, codecIdentity = identity.codecIdentity, nativeIdentity = identity.nativeIdentity;
  need(typeof nativeType === 'string' && /^[A-Za-z_][A-Za-z0-9_]*\/srv\/[A-Za-z_][A-Za-z0-9_]*$/.test(nativeType) &&
    typeof codecIdentity === 'string' && /^UWVMROS1_[0-9a-f]{64}$/.test(codecIdentity) &&
    (nativeIdentity === null || (typeof nativeIdentity === 'string' && nativeIdentity.length > 0 && nativeIdentity.length <= 256 && !nativeIdentity.includes('\0'))), 'Exact Service root facts required');
  const selected = { nativeType, codecIdentity, nativeIdentity,
    request: checkedCodec(requestCodec), response: checkedCodec(responseCodec) };
  const result = Object.freeze({ nativeType, codecIdentity, nativeIdentity,
    requestCodecIdentity: selected.request.identity, responseCodecIdentity: selected.response.identity,
    maximumRequestBytes: selected.request.maximum, maximumResponseBytes: selected.response.maximum });
  definitions.set(result, selected); return result;
}
function encoded(codec, value) {
  const bytes = apply(codec.encode, codec.object, [value]);
  need(bytes instanceof Uint8Array, 'Canonical encoder must return owned Uint8Array');
  const n = typedLength.call(bytes);
  if (n > codec.maximum) throw new WireError(Status.TOO_LARGE, 'Canonical value exceeds its selected wire bound');
  const copy = new Uint8Array(n); typedSet.call(copy, bytes); return copy;
}
function sameWriter(a, b) { for (let i = 0; i < 16; ++i) if (a[i] !== b[i]) return false; return true; }
function findTicket(s, ticket) {
  for (let i = 0; i < s.used; ++i) if (s.history[i].ticket === ticket && s.history[i].state !== FREE && s.history[i].ticket !== 0n) return s.history[i];
  return null;
}
function fact() { return { attempts: 0, code: null, threw: false, error: null }; }
function first(s, code) { if (code && s.primary === 0) s.primary = code; }
function raw(s, index, args, input, output, ledger) {
  if (ledger.attempts >= Number.MAX_SAFE_INTEGER) throw new WireError(Status.TOO_LARGE, 'Local call counter exhausted');
  const token = prepareServiceCall(s.bridge, index, args, input, output);
  // Preparing can refuse locally. Once the token enters its delegate, every
  // exception/invalid status is sticky unknown and cannot be replayed.
  try {
    const result = invokeServiceCall(token);
    ledger.attempts += 1; ledger.code = result; return result;
  } catch (error) {
    if (serviceCallAttempted(token)) { ledger.attempts += 1; ledger.threw = true; ledger.error = error; }
    throw error;
  }
}
function retire(s) {
  if (s.retired || !s.receipt) return;
  s.retired = true; // One attempt, including error/throw. Never infer rollback.
  try { raw(s, 11, [s.source.handle, s.receipt.incarnation], empty, empty, s.calls.retire); }
  catch (error) { s.calls.retire.error = error; }
}
function terminal(s, code) { first(s, code); s.failed = true; s.active = false; retire(s); return code; }
function enter(endpoint, requireActive = true) {
  const s = states.get(endpoint); need(s, 'Actual owned-Service endpoint required');
  if (s.busy) return { state: s, release: null, status: Status.CONFLICT };
  if (s.closed || (requireActive && (!s.active || s.failed))) return { state: s, release: null, status: s.primary || Status.BAD_HANDLE };
  for (const row of s.owners) current(row);
  const release = acquirePolling(s.owners.map(row => row.cap));
  if (!release) return { state: s, release: null, status: Status.CONFLICT };
  s.busy = true; return { state: s, release, status: 0 };
}
function leave(entry) { if (entry.release) { entry.state.busy = false; entry.release(); } }
function inspect(s) {
  s.inspectBytes.fill(0xa5);
  const status = raw(s, 3, [s.source.handle, s.receipt.incarnation], empty, s.inspectBytes, s.calls.inspect);
  if (status) return terminal(s, status);
  try {
    const receipt = decodeEndpointReceipt(s.inspectBytes, s.source.handle, s.kind, 2, s.expectedQos);
    if (!sameEndpointReceipt(s.receipt, receipt)) return terminal(s, Status.DENIED);
    return 0;
  } catch (error) {
    if (error instanceof WireError) return terminal(s, error.code);
    throw error;
  }
}
function cancel(s, borrow, code) {
  first(s, code);
  try {
    const status = raw(s, 9, [s.source.handle, s.receipt.incarnation, borrow.sequence, borrow.reservation], empty, empty, s.calls.cancel);
    if (status) terminal(s, code);
  } catch (error) { s.calls.cancel.error = error; terminal(s, code); }
  return code;
}
function exceptionCode(error) {
  // Decoder failure happens before ACK. Preserve explicit codec90, map other
  // local failures to22. A throwing error.code getter also becomes22.
  try { return error?.code === Status.TOO_LARGE ? Status.TOO_LARGE : Status.INVALID; } catch { return Status.INVALID; }
}
function cleanupOwners(s) {
  if (!s.ownCapabilities) return;
  // The Native lease has ended. Each genuine SDK Capability performs its OWN
  // private close ticket and actual delegate-entry retirement. A preflight
  // refusal retains ownership; it is never relabelled an attempted Host close.
  for (let i = s.owners.length - 1; i >= 0; --i) {
    const row = s.owners[i]; if (row.cleanupFinished) continue;
    try {
      if (row.cap.closed) { row.externallyClosed = true; row.retained = false; row.cleanupFinished = true; continue; }
      current(row);
      row.cleanupCode = apply(row.close, row.cap, noArgs);
    } catch (error) { row.cleanupError = error; row.cleanupThrew = true; }
    finally {
      row.cleanupAttempted = row.cap.closed && !row.externallyClosed;
      row.retained = !row.cap.closed; row.cleanupFinished = !row.retained;
    }
  }
}
function construct(endpoint, kind, bridge, sourceCap, peerCap, definition, options) {
  need(bridge instanceof OwnedServiceBridge, 'Explicit optional owned-Service bridge required');
  need(options && typeof options === 'object', 'Explicit finite endpoint options required');
  const source = owner(sourceCap), peer = owner(peerCap), queryCap = options.availability ?? null;
  need(source.cap !== peer.cap && source.handle !== peer.handle && source.session === peer.session, 'Distinct same-Session source/peer capabilities required');
  const query = queryCap === null ? null : owner(queryCap);
  need(query === null || (kind === CLIENT && query.session === source.session && query.cap !== source.cap && query.cap !== peer.cap &&
    query.handle !== source.handle && query.handle !== peer.handle), 'Availability requires its own distinct same-Session owner');
  const ownCapabilities = options.ownership === 'owned';
  need(ownCapabilities || options.ownership === 'borrowed', 'Explicit owned or borrowed capability lifetime required');
  const count = integer(options.historyEntries, 1, 65536, 'Explicit finite nonreused history entry budget required');
  const expectedQos = copyExpectedQos(options.expectedQos);
  // Only the actual factory's privately captured codec delegates are used.
  // A lookalike descriptor cannot replace the admitted decoder after activation.
  const selected = definitions.get(definition);
  need(selected, 'Actual serviceDefinition result required');
  const bound = kind === CLIENT ? selected.response.maximum : selected.request.maximum;
  const history = Array.from({ length: count }, () => ({ state: FREE, ticket: 0n, nativeSequence: 0n, writer: new Uint8Array(16) }));
  const calls = { abi: fact(), activate: fact(), inspect: fact(), submit: fact(), abandon: fact(), respond: fact(),
    peek: fact(), commit: fact(), cancel: fact(), available: fact(), retire: fact() };
  const s = { kind, bridge, source, peer, query, owners: query ? [source, peer, query] : [source, peer], ownCapabilities,
    definition: selected, expectedQos, history, used: 0, receipt: null, receiptBytes: new Uint8Array(256), inspectBytes: new Uint8Array(256),
    packet: new Uint8Array(DELIVERY_PREFIX_BYTES + bound), ticketBytes: new Uint8Array(8), availableBytes: new Uint8Array(1),
    calls, primary: 0, activationAttempted: false, activated: false, active: false, failed: false, retired: false, closed: false,
    busy: false, lastCommitted: 0n, lastReservation: 0n };
  s.ticketView = new DataView(s.ticketBytes.buffer);
  states.set(endpoint, s); Object.freeze(endpoint);
}

class Endpoint {
  /** Activate exactly once after local storage and ownership have been retained.
   * A failure leaves this object available for close()/facts(). Receipt decode
   * failure or ambiguous activation never retries and never invents incarnation.
   */
  activate() {
    const entry = enter(this, false), s = entry.state;
    if (entry.status) return entry.status;
    try {
      if (s.activationAttempted) return Status.CONFLICT;
      s.activationAttempted = true;
      const version = raw(s, 0, [], empty, empty, s.calls.abi);
      if (version !== 1) return terminal(s, Status.UNSUPPORTED);
      s.receiptBytes.fill(0xa5);
      const status = raw(s, s.kind === CLIENT ? 1 : 2, [s.source.handle, s.peer.handle], empty, s.receiptBytes, s.calls.activate);
      if (status) return terminal(s, status);
      s.receipt = decodeEndpointReceipt(s.receiptBytes, s.source.handle, s.kind, 1, s.expectedQos);
      s.activated = true; s.active = true; return 0;
    } catch (error) {
      if (error instanceof WireError) return terminal(s, error.code);
      terminal(s, exceptionCode(error)); throw error;
    } finally { leave(entry); }
  }
  /** Poll at most ONE retained record. No callback, Promise or job is pumped.
   * Full metadata, canonical decode, duplicate-history checks and the complete
   * returned value are prepared before one ACK. After ACK success only private
   * scalar/history assignments and returning the prepared object remain for a
   * delivery. A terminal control record only marks local terminal state;
   * explicit close performs the independent once-only Native retirement.
   */
  take() {
    const result = { status: 0, delivery: null, discarded: false }, entry = enter(this), s = entry.state;
    if (entry.status) { result.status = entry.status; return result; }
    let borrow = null, commitToken = null;
    try {
      if (s.kind === SERVICE && s.used === s.history.length) { result.status = Status.TOO_LARGE; return result; }
      result.status = inspect(s); if (result.status) return result;
      s.packet.fill(0xa5);
      result.status = raw(s, 7, [s.source.handle, s.receipt.incarnation, 16], empty, s.packet, s.calls.peek);
      if (result.status) {
        if (result.status !== Status.AGAIN && result.status !== Status.TOO_LARGE && result.status !== Status.UNSUPPORTED) terminal(s, result.status);
        return result;
      }
      const codec = s.kind === CLIENT ? s.definition.response : s.definition.request;
      let packet;
      try { packet = decodeDelivery(s.packet, s.receipt, s.peer.handle, codec.maximum, s.lastCommitted, s.lastReservation); }
      catch (error) {
        if (error instanceof WireError && error.borrow !== null) {
          borrow = error.borrow; s.lastReservation = borrow.reservation; result.status = cancel(s, borrow, error.code);
        } else result.status = terminal(s, error instanceof WireError ? error.code : Status.INVALID);
        return result;
      }
      borrow = packet.borrow; s.lastReservation = borrow.reservation;
      let row = findTicket(s, borrow.ticket), value = null, discarded = false;
      if (!packet.control) {
        if (s.kind === CLIENT ? (!row || (row.state !== ADMITTED && row.state !== ABANDONED)) : row !== null) {
          result.status = cancel(s, borrow, Status.DENIED); terminal(s, result.status); return result;
        }
        if (s.kind === SERVICE) {
          const id = packet.information.requestId;
          for (let i = 0; i < s.used; ++i) if (s.history[i].nativeSequence === id.sequence && sameWriter(s.history[i].writer, id.writer)) {
            result.status = cancel(s, borrow, Status.DENIED); terminal(s, result.status); return result;
          }
          row = s.history[s.used]; row.state = RESERVED; row.ticket = borrow.ticket;
          row.nativeSequence = id.sequence; typedSet.call(row.writer, id.writer);
        }
        discarded = s.kind === CLIENT && row.state === ABANDONED;
        // Late abandoned responses still pass the COMPLETE actual body decoder.
        // They may be ACKed once but can never invoke an application success.
        try { value = apply(codec.decode, codec.object, [packet.body]); }
        catch (error) { result.status = cancel(s, borrow, exceptionCode(error)); return result; }
      }
      // All allocations/callback-capable decoder work are above the commit.
      const delivery = Object.freeze({ ticket: borrow.ticket, sequence: borrow.sequence,
        kind: packet.kind, control: packet.control, controlFlags: packet.controlFlags,
        error: packet.error, information: packet.information, value });
      const args = [s.source.handle, s.receipt.incarnation, borrow.sequence, borrow.reservation];
      if (s.calls.commit.attempts >= Number.MAX_SAFE_INTEGER) throw new WireError(Status.TOO_LARGE, 'Commit counter exhausted');
      commitToken = prepareServiceCall(s.bridge, 8, args, empty, empty);
      result.status = invokeServiceCall(commitToken); s.calls.commit.attempts += 1; s.calls.commit.code = result.status;
      if (result.status) { terminal(s, result.status); return result; }
      s.lastCommitted = borrow.sequence;
      if (!packet.control) { row.state = s.kind === CLIENT ? (discarded ? ABANDONED_DELIVERED : DONE) : DELIVERED; if (s.kind === SERVICE) s.used += 1; }
      else if (row && row.state === ADMITTED) row.state = DONE;
      result.delivery = discarded ? null : delivery; result.discarded = discarded;
      if (packet.control) { first(s, packet.error || Status.REVOKED); s.failed = true; s.active = false; }
      return result;
    } catch (error) {
      if (commitToken !== null && serviceCallAttempted(commitToken)) { s.calls.commit.attempts += 1; s.calls.commit.threw = true; s.calls.commit.error = error; terminal(s, exceptionCode(error)); }
      else if (borrow !== null) cancel(s, borrow, exceptionCode(error));
      else terminal(s, exceptionCode(error));
      throw error;
    } finally { leave(entry); }
  }
  /** Native retirement once, then independent reverse capability cleanup only
   * for explicit ownership:'owned'. Borrowed owners remain the caller's duty.
   * This endpoint never changes Runtime debug/update/Host policy settings.
   * Repeating close may only retry a genuine SDK LOCAL close preflight which
   * never entered its delegate; Native retirement is always at most once.
   */
  close() {
    const s = states.get(this); need(s, 'Actual owned-Service endpoint required');
    if (s.busy) return Status.CONFLICT;
    // This endpoint guard spans Native retirement AND every actual SDK close.
    // The owner polling lease ends BEFORE Capability.close, since that genuine
    // core correctly refuses17 while ownerPolling is held. Synchronous adapted
    // delegate reentry therefore sees17 without corrupting attempted/external
    // attribution or starting another cleanup pass.
    s.busy = true;
    try {
      if (!s.closed) {
        const release = acquirePolling(s.owners.map(row => row.cap));
        if (!release) return Status.CONFLICT;
        try { s.active = false; s.failed = true; retire(s); s.closed = true; }
        finally { release(); }
      }
      cleanupOwners(s);
      if (s.calls.retire.code) return s.calls.retire.code;
      if (s.calls.retire.threw || s.calls.retire.error !== null) return Status.INVALID;
      for (const row of s.owners) if (s.ownCapabilities && row.retained) return row.cleanupCode || Status.CONFLICT;
      for (const row of s.owners) if (row.cleanupCode) return row.cleanupCode;
      // An attempted close retires its SDK lifetime even on delegate throw.
      // Retired ownership does not convert that unknown cleanup into success0.
      for (const row of s.owners) if (row.cleanupThrew) return Status.INVALID;
      return 0;
    } finally { s.busy = false; }
  }
  /** Allocate a diagnostic COPY on explicit request. Never expose private
   * receipt/QoS/history buffers or a mutable ownership ledger to callers.
   */
  facts() {
    const s = states.get(this); need(s, 'Actual owned-Service endpoint required');
    const calls = {};
    for (const name of Object.keys(s.calls)) calls[name] = Object.freeze({ ...s.calls[name] });
    return Object.freeze({ kind: s.kind, nativeType: s.definition.nativeType, codecIdentity: s.definition.codecIdentity,
      primary: s.primary, active: s.active, failed: s.failed, closed: s.closed, activated: s.activated,
      activationScheduled: s.activationAttempted, activationAttempted: s.calls.activate.attempts !== 0,
      retireScheduled: s.retired, retireAttempted: s.calls.retire.attempts !== 0, historyEntries: s.history.length,
      historyUsed: s.used, lastCommittedSequence: s.lastCommitted, lastReservation: s.lastReservation,
      capabilityOwnership: s.ownCapabilities ? 'owned' : 'borrowed',
      borrowedOwnerCleanupRequired: !s.ownCapabilities && s.activationAttempted,
      calls: Object.freeze(calls), owners: Object.freeze(s.owners.map(row => Object.freeze({ handle: row.handle,
        cleanupAttempted: row.cleanupAttempted, cleanupCode: row.cleanupCode, cleanupError: row.cleanupError, cleanupThrew: row.cleanupThrew,
        cleanupFinished: row.cleanupFinished, externallyClosed: row.externallyClosed, retained: row.retained }))) });
  }
}

/** call+event.observe pair, plus optional independent service.available owner. */
export class OwnedClient extends Endpoint {
  constructor(bridge, call, observer, definition, options) { super(); construct(this, CLIENT, bridge, call, observer, definition, options); }
  /** Admit exactly once. A returned ticket means admission, never completion.
   * Full unsigned64 tickets, including UINT64_MAX, are legal; only zero is bad.
   * Every attempted submit burns one finite local lifetime row, including11.
   */
  submit(request) {
    const result = { status: 0, ticket: null }, entry = enter(this), s = entry.state;
    if (entry.status) { result.status = entry.status; return result; }
    try {
      if (s.used === s.history.length) { result.status = Status.TOO_LARGE; return result; }
      let bytes;
      try { bytes = encoded(s.definition.request, request); }
      catch (error) { result.status = exceptionCode(error); return result; }
      result.status = inspect(s); if (result.status) return result;
      const row = s.history[s.used++]; row.state = RESERVED;
      s.ticketBytes.fill(0);
      result.status = raw(s, 4, [s.source.handle, s.receipt.incarnation], bytes, s.ticketBytes, s.calls.submit);
      if (result.status) { if (result.status !== Status.AGAIN) terminal(s, result.status); return result; }
      const ticket = s.ticketView.getBigUint64(0, true);
      if (ticket === 0n || findTicket(s, ticket) !== null) { result.status = terminal(s, Status.DENIED); return result; }
      row.ticket = ticket; row.state = ADMITTED; result.ticket = ticket; return result;
    } catch (error) { terminal(s, exceptionCode(error)); throw error; }
    finally { leave(entry); }
  }
  /** Retire a pending request once. This is not a network resend or rollback. */
  abandon(ticket) {
    u64(ticket); const entry = enter(this), s = entry.state;
    if (entry.status) return entry.status;
    try {
      const row = findTicket(s, ticket); if (!row || row.state !== ADMITTED) return Status.BAD_HANDLE;
      const checked = inspect(s); if (checked) return checked;
      row.state = ABANDONED;
      const code = raw(s, 5, [s.source.handle, s.receipt.incarnation, ticket], empty, empty, s.calls.abandon);
      if (code) terminal(s, code); return code;
    } catch (error) { terminal(s, exceptionCode(error)); throw error; }
    finally { leave(entry); }
  }
  /** Query the genuine same-endpoint Native client using its third grant.
   * waitNanoseconds is explicit BigInt in[0,60s]; positive waits use the real
   * Host steady deadline, not a JS timer or readiness count guessed as truth.
   */
  available(waitNanoseconds = 0n) {
    u64(waitNanoseconds); need(waitNanoseconds <= MAXIMUM_WAIT_NS, 'Availability wait exceeds60s');
    const result = { status: 0, available: null }, entry = enter(this), s = entry.state;
    if (entry.status) { result.status = entry.status; return result; }
    try {
      if (!s.query) { result.status = Status.DENIED; return result; }
      result.status = inspect(s); if (result.status) return result;
      s.availableBytes[0] = 0xa5;
      result.status = raw(s, 10, [s.source.handle, s.receipt.incarnation, s.query.handle, waitNanoseconds], empty, s.availableBytes, s.calls.available);
      if (result.status) { first(s, result.status); return result; }
      if (s.availableBytes[0] > 1) { result.status = terminal(s, Status.INVALID); return result; }
      result.available = s.availableBytes[0] === 1; return result;
    } catch (error) { terminal(s, exceptionCode(error)); throw error; }
    finally { leave(entry); }
  }
}

/** advertise+respond pair. take() returns an exact one-shot request ticket. */
export class OwnedServer extends Endpoint {
  constructor(bridge, advertise, responder, definition, options) { super(); construct(this, SERVICE, bridge, advertise, responder, definition, options); }
  /** Send one exact-ID typed response. The handler runs OUTSIDE take()/ACK.
   * A handler exception should close this endpoint explicitly, never send an
   * empty successful response. The DONE row prevents a duplicate response even
   * if the Native call throws or returns an uncertain nonzero status.
   */
  respond(ticket, response) {
    u64(ticket); const entry = enter(this), s = entry.state;
    if (entry.status) return entry.status;
    try {
      const row = findTicket(s, ticket); if (!row || row.state !== DELIVERED) return Status.BAD_HANDLE;
      let bytes;
      try { bytes = encoded(s.definition.response, response); }
      catch (error) { return exceptionCode(error); }
      const checked = inspect(s); if (checked) return checked;
      row.state = DONE;
      const code = raw(s, 6, [s.source.handle, s.receipt.incarnation, s.peer.handle, ticket], bytes, empty, s.calls.respond);
      if (code) terminal(s, code); return code;
    } catch (error) { terminal(s, exceptionCode(error)); throw error; }
    finally { leave(entry); }
  }
}
