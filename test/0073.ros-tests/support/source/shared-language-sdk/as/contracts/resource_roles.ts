/** Prepared pure Wasm typed-resource role fixture. The fake Api counts local
 * calls only; no native ROS resource, socket, guest bridge or grant exists here.
 * Actual cgroup compilation/execution is pending. Wrong named operation roles
 * must return Invalid (22) before host open/request/poll/control dispatch.
 */
import { Api, Operation, Status } from '../../../../../../../sdk/origin/assemblyscript/api';
import { Node, Graph, Clock, Parameters, Tf, Lifecycle } from '../../../../../../../sdk/origin/assemblyscript/roswasm';
import { ParameterKind, ParameterValue, Transform } from '../../../../../../../sdk/origin/assemblyscript/codecs';
export { abortGuest } from '../../../../../../../sdk/origin/assemblyscript/api';

/** Guest-local fake demonstrates that registered operations are insufficient for a typed role. */
class FakeApi extends Api {
  opens: u32 = 0; requests: u32 = 0; polls: u32 = 0; events: u32 = 0;
  last: Operation = Operation.service_call;
  constructor() { super(1); }
  version(): u32 { return 1; }
  supports(operation: Operation): bool { return true; }
  open(memory: u32, offset: u64, length: u64, slot: u64): u32 { this.opens++; store<u64>(<usize>slot, this.opens); return 0; }
  close(handle: u64): u32 { return 0; }
  parameterEvents(handle: u64): u32 { this.events++; return 0; }
  poll(handle: u64, memory: u32, offset: u64, capacity: u64, slot: u64): u32 { this.polls++; return 11; }
  request(operation: Operation, handle: u64, memory: u32, input: u64, length: u64, output: u64, capacity: u64, slot: u64): u32 { this.requests++; this.last = operation; store<u64>(<usize>slot, 0); return 0; }
}
let checks: u32 = 0;
/** Fail by a local trap without importing native logging or abort. */
function require(value: bool): void { if (!value) unreachable(); checks++; }

/** Check wrong-family factories, direct constructors, exact methods and explicit mutations. */
export function _start(): void {
  const api = new FakeApi(), node = new Node(api);
  require(node.createGraph('delete', Operation.parameter_delete).readText().status == 22);
  require(node.createClock('delete', Operation.parameter_delete).now().status == 22);
  require(node.createParameters('wrong', Operation.service_respond, ParameterKind.Int32).getReply().status == 22);
  require(node.createTf('delete', Operation.parameter_delete).lookup().status == 22);
  require(node.createLifecycle('delete', Operation.parameter_delete).getStateReply().status == 22);
  require(api.opens == 0 && api.requests == 0);

  const cap = node.open('existing-explicit-authority');
  require(new Graph(cap, Operation.parameter_delete).readText().status == 22);
  require(new Clock(cap, Operation.parameter_delete).now().status == 22);
  const parameter = new Parameters(cap, Operation.parameter_delete, ParameterKind.Int32), value = new ParameterValue(ParameterKind.Int32);
  require(parameter.getReply().status == 22 && !parameter.get().valid);
  require(parameter.set(value).status == 22 && parameter.declareValue(value).status == 22);
  require(parameter.readTextReply().status == 22 && parameter.exists().status == 22 && parameter.search().status == 22);
  require(parameter.subscribeEvents() == 22 && parameter.pollEventReply().status == 22);
  require(new Tf(cap, Operation.tf_broadcast).lookup().status == 22);
  require(new Tf(cap, Operation.tf_lookup).broadcast(new Transform()).status == 22);
  require(new Lifecycle(cap, Operation.lifecycle_change_state).getStateReply().status == 22);
  require(new Lifecycle(cap, Operation.lifecycle_get_state).changeState().status == 22);
  require(api.opens == 1 && api.requests == 0 && api.polls == 0 && api.events == 0);

  // The explicit mutation method still issues exactly its selected operation.
  require(parameter.delete().status == 0 && api.requests == 1 && api.last == Operation.parameter_delete);
  const getter = new Parameters(cap, Operation.parameter_get, ParameterKind.Int32);
  require(getter.getReply().status == 0 && api.requests == 2 && api.last == Operation.parameter_get);
  require(getter.delete().status == 22 && api.requests == 2);
  const setter = new Parameters(cap, Operation.parameter_set, ParameterKind.Int32);
  require(setter.set(value).status == 0 && api.requests == 3 && api.last == Operation.parameter_set);
  require(setter.getReply().status == 22 && api.requests == 3);
  // Events remain dedicated handle-only subscribe plus poll, never seven-argument RPC.
  const events = new Parameters(cap, Operation.parameter_subscribe_events, ParameterKind.Int32);
  require(events.call().status == 22 && api.requests == 3);
  require(events.subscribeEvents() == 0 && api.events == 1 && api.requests == 3);
  require(events.pollEventReply().status == 11 && api.polls == 1 && api.requests == 3);
  node.close();
}
/** Count only after _start returns without a trap; prepared source is not a PASS. */
export function completedChecks(): u32 { return checks; }
