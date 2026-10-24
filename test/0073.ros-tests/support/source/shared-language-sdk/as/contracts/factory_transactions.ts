/** Prepared pure Wasm failed-factory fixture. The guest-local fake tracks handle
 * ownership only, creates no native ROS endpoint and proves no authorization.
 * Actual compilation/execution is pending in the authorized Linux cgroup.
 */
import { Api, Operation } from '../../../../../../../sdk/origin/assemblyscript/api';
import { Node } from '../../../../../../../sdk/origin/assemblyscript/roswasm';
import { ServiceRequest, ServiceResponse, IncomingGoal, ActionSequence } from '../../../../../../../sdk/origin/assemblyscript/codecs';
export { abortGuest } from '../../../../../../../sdk/origin/assemblyscript/api';

/** Fake bridge mechanics with deterministic open/activate/close failures. */
class FakeApi extends Api {
  opens: u32 = 0; closes: u32 = 0; live: u32 = 0; activations: u32 = 0;
  constructor(public failAt: u32 = 2, public openError: u32 = 13, public activationError: u32 = 0) { super(2); }
  version(): u32 { return 1; }
  supports(operation: Operation): bool { return true; }
  open(memory: u32, offset: u64, length: u64, slot: u64): u32 { this.opens++; if (this.opens == this.failAt) return this.openError; this.live++; store<u64>(<usize>slot, <u64>this.opens); return 0; }
  close(handle: u64): u32 { if (!this.live) unreachable(); this.live--; this.closes++; return 107; }
  advertiseService(handle: u64): u32 { this.activations++; return this.activationError; }
  request(operation: Operation, handle: u64, memory: u32, input: u64, length: u64, output: u64, capacity: u64, slot: u64): u32 { if (operation == Operation.action_advertise) this.activations++; store<u64>(<usize>slot, 0); return this.activationError; }
}
let checks: u32 = 0;
/** Local assertion trap with no implicit native logger or clock. */
function require(value: bool): void { if (!value) unreachable(); checks++; }
/** Stateless service handler stays guest-local and is never invoked by these creation tests. */
function serve(request: ServiceRequest): ServiceResponse { return new ServiceResponse(true, 'ok'); }
/** Stateless Action handler preserves the incoming owned goal ID. */
function act(goal: IncomingGoal): ActionSequence { return new ActionSequence(goal.id, 2, [0, 1]); }
/** Construct one selected factory and retain the original initialization status. */
function construct(node: Node, kind: u32): u32 { if (kind == 0) return node.createService('advertise', 'respond', serve).status; if (kind == 1) return node.createActionServer('advertise', 'respond', act).status; return node.createActionClient('send', '', 'feedback', 'result').status; }

/** Execute all prepared ownership/no-activation conditions once. */
export function _start(): void {
  for (let kind: u32 = 0; kind < 3; ++kind) for (let variant: u32 = 0; variant < 2; ++variant) {
    const error: u32 = variant ? 107 : 13, api = new FakeApi(2, error), node = new Node(api);
    require(construct(node, kind) == error); require(api.opens == 2 && api.closes == 1 && api.live == 0); require(api.activations == 0);
    node.close(); require(api.closes == 1); api.opens = 0;
    // A closed Node cannot reopen. A fresh organizer retains the original
    // second rollback scenario and every open/close/activation assertion.
    require(construct(node, kind) == 9); require(api.opens == 0 && api.closes == 1);
    const retryNode = new Node(api);
    require(construct(retryNode, kind) == error); require(api.opens == 2 && api.closes == 2 && api.live == 0); require(api.activations == 0);
    retryNode.close(); require(api.closes == 2);
  }
  const thirdApi = new FakeApi(3), thirdNode = new Node(thirdApi); require(construct(thirdNode, 2) == 13); require(thirdApi.closes == 2 && thirdApi.live == 0 && thirdApi.activations == 0); thirdNode.close(); require(thirdApi.closes == 2);
  for (let kind: u32 = 0; kind < 2; ++kind) {
    const failedApi = new FakeApi(0, 13, 107), failedNode = new Node(failedApi); require(construct(failedNode, kind) == 107); require(failedApi.activations == 1 && failedApi.closes == 2 && failedApi.live == 0); failedNode.close(); require(failedApi.closes == 2);
    const okApi = new FakeApi(0), okNode = new Node(okApi); require(construct(okNode, kind) == 0); require(okApi.activations == 1 && okApi.closes == 0 && okApi.live == 2); okNode.close(); require(okApi.closes == 2 && okApi.live == 0);
  }
}
/** Count only after _start returns without a trap; source preparation is not a PASS. */
export function completedChecks(): u32 { return checks; }
