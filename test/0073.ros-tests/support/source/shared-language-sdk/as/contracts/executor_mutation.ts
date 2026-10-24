/** Prepared pure Wasm bounded-executor fixture. These guest-local Pollables use
 * no ROS imports, capabilities, native callbacks, clock or filesystem.
 * Source preparation/static verification is not an AssemblyScript execution
 * verdict; compile and execute this fixture only in the authorized Linux cgroup.
 */
import { Executor, Pollable } from '../../../../../../../sdk/origin/assemblyscript/roswasm';
import { Status } from '../../../../../../../sdk/origin/assemblyscript/api';
export { abortGuest } from '../../../../../../../sdk/origin/assemblyscript/api';
let checks: u32 = 0;

/** Fail by a local trap without importing an abort/logger service. */
function require(value: bool): void { if (!value) unreachable(); checks++; }

/** Self-registering entity models the original unbounded growing-loop trigger. */
class SelfAdding implements Pollable {
  calls: u32 = 0;
  constructor(private executor: Executor) {}
  spinOnce(): u32 { this.calls++; this.executor.add(this); return Status.Success; }
}
/** Counter entity exposes whether a newly registered entity ran in the same pass. */
class Counter implements Pollable {
  calls: u32 = 0;
  spinOnce(): u32 { this.calls++; return Status.Again; }
}
/** One handler registers a second entity without extending its current pass. */
class AddOther implements Pollable {
  calls: u32 = 0;
  private added: bool = false;
  constructor(private executor: Executor, private other: Counter) {}
  spinOnce(): u32 { this.calls++; if (!this.added) { this.executor.add(this.other); this.added = true; } return Status.Success; }
}
/** Nested polling must return Conflict before redispatching this same handler. */
class Nested implements Pollable {
  calls: u32 = 0;
  nestedCount: i32 = 0;
  nestedStatus: u32 = 0;
  constructor(private executor: Executor) {}
  spinOnce(): u32 { this.calls++; const nested = this.executor.spinOnce(); this.nestedCount = nested.length; this.nestedStatus = nested[0]; return Status.Success; }
}

/** Execute three finite prepared mutation/reentrancy scenarios. */
export function _start(): void {
  const selfExecutor = new Executor(), self = new SelfAdding(selfExecutor); selfExecutor.add(self);
  const selfFirst = selfExecutor.spinOnce(); require(selfFirst.length == 1 && self.calls == 1);
  const selfSecond = selfExecutor.spinOnce(); require(selfSecond.length == 2 && self.calls == 3);

  const addingExecutor = new Executor(), other = new Counter(), adding = new AddOther(addingExecutor, other); addingExecutor.add(adding);
  const addFirst = addingExecutor.spinOnce(); require(addFirst.length == 1 && adding.calls == 1 && other.calls == 0);
  const addSecond = addingExecutor.spinOnce(); require(addSecond.length == 2 && adding.calls == 2 && other.calls == 1); require(addSecond[0] == Status.Success && addSecond[1] == Status.Again);

  const nestedExecutor = new Executor(), nested = new Nested(nestedExecutor); nestedExecutor.add(nested);
  const outer = nestedExecutor.spinOnce(); require(outer.length == 1 && outer[0] == Status.Success && nested.calls == 1); require(nested.nestedCount == 1 && nested.nestedStatus == Status.Conflict);
  const next = nestedExecutor.spinOnce(); require(next.length == 1 && nested.calls == 2); require(nested.nestedStatus == Status.Conflict);
}
/** Count only after _start returns without a trap; prepared source is not a PASS. */
export function completedChecks(): u32 { return checks; }
