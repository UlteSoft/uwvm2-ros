/** Prepared pure Wasm adapter-contract fixture with a guest-local fake Api.
 * The fake creates no ROS endpoints, imports no network or runtime functions,
 * and does not establish real authorization. Actual cgroup compilation/execution
 * is required before any test verdict is claimed for this AssemblyScript source.
 */
import { Api, Status, Operation } from '../../../../../../../sdk/origin/assemblyscript/api';
import { Node, ActionClient } from '../../../../../../../sdk/origin/assemblyscript/roswasm';
export { abortGuest } from '../../../../../../../sdk/origin/assemblyscript/api';

/** Guest-local fake validates only ownership, output-slot and optional-grant mechanics. */
class FakeApi extends Api {
  opens: u32 = 0; closes: u32 = 0; revokes: u32 = 0; requests: u32 = 0;
  mode: u32 = 0; cancelSelector: u64 = 1;
  constructor() { super(2); }
  version(): u32 { return 1; }
  supports(operation: Operation): bool { return operation != Operation.parameter_delete; }
  open(memory: u32, offset: u64, length: u64, slot: u64): u32 { this.opens++; if (this.mode == 13) return 13; store<u64>(<usize>slot, u64.MAX_VALUE); return 0; }
  close(handle: u64): u32 { this.closes++; return 107; }
  revoke(handle: u64): u32 { this.revokes++; return 107; }
  request(operation: Operation, handle: u64, memory: u32, input: u64, length: u64, output: u64, capacity: u64, slot: u64): u32 { this.requests++; if (operation == Operation.action_cancel_any_goal) { this.cancelSelector = load<u64>(<usize>input); return 0; } if (this.mode == 90) return 90; if (this.mode == 91) { store<u64>(<usize>slot, 9007199254740993); return 90; } store<u64>(<usize>slot, 0); return 0; }
}
let checks: u32 = 0;
/** Fail by a local trap; no log/clock/native permission is added to the fixture. */
function require(value: bool): void { if (!value) unreachable(); checks++; }

/** Execute prepared pure contracts without contacting native ROS. */
export function _start(): void {
  const api = new FakeApi(), node = new Node(api);
  const cap = node.open('owned'); require(cap.openStatus == 0 && cap.handle == u64.MAX_VALUE);
  api.mode = 90; const unknown = cap.request(Operation.service_call); require(unknown.status == 90 && !unknown.requiredValid);
  api.mode = 91; const known = cap.request(Operation.service_call); require(known.status == 90 && known.requiredValid && known.required == 9007199254740993);
  require(api.requests == 2); require(cap.close() == 107 && !cap.isOpen()); require(cap.close() == 9 && api.closes == 1);
  api.mode = 0; const revoked = node.open('revoke'); require(revoked.revoke() == 107 && !revoked.isOpen()); require(revoked.revoke() == 9 && api.revokes == 1);
  const before = api.opens, action = node.createActionClient('send'); require(api.opens == before + 1);
  require(action.cancelOwn(1).status == 13 && action.feedback(1).status == 13 && action.result(1).status == 13 && action.cancelAll().status == 13);
  const any = node.open('explicit-any'), privileged = new ActionClient(node.open('send-2'), null, null, null, 4096, any);
  const cancel = privileged.cancelAll(); require(api.cancelSelector == 0 && api.requests == 3);
  // Fake any-cancel's successful length slot is intentionally malformed here: status 22
  // demonstrates that a zero selector never makes the output contract implicit.
  require(cancel.status == Status.Invalid);
  const beforeUnsupported = api.opens, resource = node.createResource('wrong-backend', Operation.parameter_delete); require(resource.call().status == 95 && api.opens == beforeUnsupported);
  api.mode = 13; const denied = node.open('no-rights'); require(denied.openStatus == 13 && !denied.isOpen());
  node.close();
}
/** Completed checks only after _start returns without a trap. */
export function completedChecks(): u32 { return checks; }
