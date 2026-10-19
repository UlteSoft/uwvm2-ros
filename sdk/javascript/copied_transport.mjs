/**
 * @file copied_transport.mjs
 * @brief Bounded copied storage for an ECMAScript engine inside real Wasm.
 *
 * This transport is not WebAssembly.Memory and never impersonates it. The
 * embedded engine's native module supplies describe()/invoke(); its C functions
 * and original ROS imports are compiled into the same actual Wasm principal.
 * Merely constructing this JavaScript class is not identity attestation. Final
 * artifact/import inspection and an actual Runtime execution establish that
 * provenance. Host mathematical models remain mathematical models.
 */
import { specification, u32, u64, resultU32, registerCallPreparation, delegateCallEntry } from './raw.mjs';

const SENTINEL = 0xffffffffffffffffn;
function require(condition, message) { if (!condition) throw new TypeError(message); }
function byteArray(value) { require(value instanceof Uint8Array, 'Owned Uint8Array input required'); return value; }

/** One copied native binding; no file, network, process or implicit permission.
 * The layout describes C-owned scratch in the same Wasm, not a host memory view.
 * Calls are synchronous and serialized. C independently validates every scalar,
 * form and range before copying bytes or entering an original ROS import.
 */
export class CopiedTransport {
  constructor(version, binding) {
    const spec = specification(version);
    require(binding && typeof binding.describe === 'function' && typeof binding.invoke === 'function',
      'Explicit embedded native binding required');
    const layout = binding.describe();
    require(layout && layout.backend === version && layout.abiVersion === 1 && layout.functionCount === spec.functions.length,
      'Exact selected embedded ABI required');
    for (const name of ['input', 'slot', 'output']) u64(layout[name]);
    for (const name of ['inputCapacity', 'outputCapacity']) {
      u32(layout[name]); require(layout[name] > 0 && layout[name] <= 4193786, 'Bounded native scratch capacity required');
    }
    require(layout.slot === layout.input + BigInt(layout.inputCapacity) && layout.output === layout.slot + 8n,
      'Disjoint exact embedded scratch layout required');
    this.version = version; this.binding = binding; this.layout = Object.freeze({ ...layout });
    this.input = layout.input; this.slot = layout.slot; this.output = layout.output;
    this.inputCapacity = layout.inputCapacity; this.outputCapacity = layout.outputCapacity;
    this.active = false; this.invoking = false; this.closed = false;
    this.inputBytes = new Uint8Array(); this.slotSeed = SENTINEL; this.last = null;
    this.functions = Object.freeze(Object.fromEntries(spec.functions.map((entry, index) =>
      [entry.name, registerCallPreparation((...args) => this.forward(index, entry, args),
        (_receiver, args) => prepareForward(this, index, entry, args))])));
  }
  /** Borrow only until the synchronous Session operation returns. No JS callback
   * or Promise job is pumped while this scratch/input borrow is active.
   */
  begin(input) {
    require(!this.closed && !this.active && !this.invoking, 'Copied scratch is closed or already borrowed');
    byteArray(input); require(input.length <= this.inputCapacity, 'Input exceeds embedded scratch');
    this.active = true; this.inputBytes = input; this.last = null;
  }
  end() { this.inputBytes = new Uint8Array(); this.active = false; }
  seed(value) { this.slotSeed = u64(value); this.last = null; }
  readSlot() { return this.last ? this.last.slot : this.slotSeed; }
  copyOutput(length) {
    u64(length);
    require(this.last && length <= BigInt(this.last.snapshot.length), 'Native snapshot is shorter than the written result');
    return this.last.snapshot.slice(0, Number(length));
  }
  forward(index, entry, args) { return prepareForward(this, index, entry, args)(null); }
  /** The copied primitive also serves the asynchronous facade. It does not make
   * a blocking ROS operation asynchronous; Promise resolution stays local.
   */
  call(name, args, options) { return prepareCopiedCall(this, name, args, options)(null); }
  callIndex(index, args, input, slotSeed, snapshot) {
    return prepareCopiedIndex(this, index, args, input, slotSeed, snapshot)(null);
  }
}

/** Explicit Promise-facing adapter with no Node streams, host timer or engine
 * teardown. shutdown retires this channel only; the embedding closes the engine
 * after all application-owned resources have been checked and released.
 */
export class CopiedChannel {
  constructor(transport) {
    require(transport instanceof CopiedTransport && !transport.closed, 'Live copied transport required');
    this.transport = transport; this.version = transport.version;
    this.layout = transport.layout; this.state = 'ready';
  }
  async call(name, args, options) {
    require(this.state === 'ready', 'Copied channel is closed');
    return this.transport.call(name, args, options);
  }
  async shutdown() {
    require(this.state === 'ready' && !this.transport.active && !this.transport.invoking, 'Copied channel is busy or closed');
    this.state = 'closed'; this.transport.closed = true;
    return { status: 0, local: true };
  }
}

/** Private serialization is independent of a caller changing a diagnostic flag. */
const copiedInvocations = new WeakSet();
function prepareForward(transport, index, entry, supplied) {
  const args = supplied; let snapshot = 0;
  if (entry.name === 'event_ready') snapshot = 32;
  else if (args.length === 5) snapshot = Number(args[3]);
  else if (args.length === 7) snapshot = Number(args[5]);
  const run = prepareCopiedIndex(transport, index, args,
    transport.active ? transport.inputBytes : new Uint8Array(), transport.slotSeed, snapshot);
  return ticket => { const reply = run(ticket); transport.last = reply; return reply.status; };
}
function prepareCopiedCall(transport, name, args, { input = new Uint8Array(), slotSeed = SENTINEL, snapshot = 0 } = {}) {
  require(transport instanceof CopiedTransport, 'Actual copied transport required');
  const spec = specification(transport.version), index = spec.functions.findIndex(entry => entry.name === name);
  require(index >= 0, 'Unknown embedded ABI operation');
  return prepareCopiedIndex(transport, index, args, input, slotSeed, snapshot);
}
/** Validate the copied native binding's reserved forms before binding.invoke.
 * Native C independently validates them again and uses the real Wasm imports.
 */
function copiedForm(entry, args, input, snapshot, layout) {
  const count = args.length, length = BigInt(input.length);
  if (entry.name === 'event_watch') { require(!input.length && snapshot === 0, 'Observer association has no transfer'); return; }
  if (entry.name === 'event_ready') { require(!input.length && args[1] === 0 && args[2] === layout.output && snapshot === 32 && layout.outputCapacity >= 32, 'Exact readiness output required'); return; }
  if (entry.name === 'service_submit') { require(args[2] === 0 && args[3] === layout.input && args[4] === length && args[5] === layout.slot && snapshot === 0, 'Exact admission input required'); return; }
  if (count === 0 || count === 1) { require(!input.length && snapshot === 0, 'Scalar call has no transfer'); return; }
  if (count === 4 && entry.name === 'capability_open') require(args[0] === 0 && args[1] === layout.input && args[2] === length && args[3] === layout.slot, 'Reserved open ranges required');
  else if (count === 4) require(args[1] === 0 && args[2] === layout.input && args[3] === length && snapshot === 0, 'Reserved write ranges required');
  else if (count === 5) require(!input.length && args[1] === 0 && args[2] === layout.output && args[3] === BigInt(snapshot) && args[4] === layout.slot, 'Reserved poll ranges required');
  else if (count === 7) require(args[1] === 0 && args[2] === layout.input && args[3] === length && args[4] === layout.output && args[5] === BigInt(snapshot) && args[6] === layout.slot, 'Reserved request ranges required');
  else require(false, 'Unsupported original ABI form');
}
function prepareCopiedIndex(transport, index, supplied, input, slotSeed, snapshot) {
  require(transport instanceof CopiedTransport, 'Actual copied transport required');
  require(!transport.closed && !transport.invoking && !copiedInvocations.has(transport), 'Embedded transport is closed or reentrant');
  const spec = specification(transport.version);
  require(Number.isInteger(index) && index >= 0 && index < spec.functions.length, 'Exact embedded function index required');
  const entry = spec.functions[index];
  require(Array.isArray(supplied) && supplied.length === entry.parameters.length, 'Exact embedded ABI arity required');
  const args = Array.from(supplied);
  for (let i = 0; i < args.length; ++i) (entry.parameters[i].type === 'i64' ? u64 : u32)(args[i]);
  byteArray(input); u64(slotSeed); u32(snapshot);
  const layout = transport.layout;
  require(input.length <= layout.inputCapacity && snapshot <= layout.outputCapacity, 'Embedded transfer exceeds explicit capacity');
  copiedForm(entry, args, input, snapshot, layout);
  // C takes its owned input copy synchronously; no await occurs before invoke.
  const copied = input, binding = transport.binding, invoke = binding?.invoke;
  require(typeof invoke === 'function', 'Actual native invoke delegate required');
  const invokeArgs = [index, args, copied, slotSeed, snapshot];
  return ticket => {
    require(!transport.closed && !transport.invoking && !copiedInvocations.has(transport), 'Embedded transport is closed or reentrant');
    copiedInvocations.add(transport);
    try {
      transport.invoking = true;
      const reply = delegateCallEntry(ticket, invoke, binding, invokeArgs);
      require(reply && typeof reply === 'object', 'Missing native copied result');
      const status = resultU32(reply.status); u64(reply.slot); byteArray(reply.snapshot);
      require(reply.snapshot.length === snapshot, 'Exact native snapshot length required');
      const slotBytes = new Uint8Array(8);
      new DataView(slotBytes.buffer).setBigUint64(0, reply.slot, true);
      return { status, slot: reply.slot, slotBytes, snapshot: reply.snapshot };
    } finally { copiedInvocations.delete(transport); transport.invoking = false; }
  };
}
registerCallPreparation(CopiedTransport.prototype.call, (transport, [name, args, options]) => prepareCopiedCall(transport, name, args, options));
registerCallPreparation(CopiedTransport.prototype.callIndex, (transport, [index, args, input, seed, snapshot]) => prepareCopiedIndex(transport, index, args, input, seed, snapshot));
registerCallPreparation(CopiedChannel.prototype.call, (channel, [name, args, options]) => {
  require(channel instanceof CopiedChannel && channel.state === 'ready', 'Copied channel is closed');
  return prepareCopiedCall(channel.transport, name, args, options);
});
