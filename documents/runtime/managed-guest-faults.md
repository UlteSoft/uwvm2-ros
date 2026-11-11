# Managed Guest faults and process exit

Linux ROS production-update sessions retain the authenticated management channel after a qualified Guest trap. A fault closes real execution admission and reports phase `guest_fault` (wire value 9), `active=false`, while retaining installed-image, committed-epoch and decision-sequence evidence. STATUS remains available. Business calls, candidate publication and launch receipts cannot reopen the faulted execution. Disconnect exits with status 75; recovery needs a fresh externally authorized launch.

The ROS product's ordinary LLVM-full CLI uses the same real source-owned
activation and native-cleanup protection internally. It opens no debugger
transport or DeveloperGrant. A qualified Guest fault finishes the actual unwind
and preserves crash termination by default (`--debug-fault-action crash`).
An explicit `--debug-fault-action exit` finishes with the classified status
120 (unreachable), 121 (execution) or 122 (MTE). Internal failures still crash.
Pure Wasm preloads belong to the same owned source
graph; a provider graph without the required source ownership is refused before
Guest compilation/execution. Internal recording grows with actual activations,
without imposing the DEV recorder's default storage budget on ordinary programs.

DEV LLVM-full preparation enables protection for its actual runtime epoch.
After a qualified fault, the Linux authenticated control FD retains `status`,
`help` and `quit`; execution and mutation requests are refused for that faulted
world. An explicit controller quit follows the authenticated session terminal
contract; it is not implicit recovery under the ordinary crash policy. This does
not authorize resuming a damaged
activation or treating an internal/TCB failure as a Guest trap.

The compiler emits native unwind cleanup for marked trap sites and selected internal memory/table helpers. Helpers finish their native memory/table guards before raising the private fault signal. The actual outer entry accepts it only after current code/source/function generations are authenticated and generated activation/GC/shadow cleanup has completed. Guest exception handlers cannot catch this private signal. Runtime invariant failures keep their fatal behavior.

The implemented paths include generated numeric/unreachable/reference/memory traps; Memory32 and Memory64 bulk copy/fill/init; Memory64 scalar and SIMD fallback accesses; atomic load/store/RMW fallback bounds/alignment checks; table get/set/init/copy/fill; and GC status dispatch for null references, array bounds, allocation/size failure. These mechanisms do not convert arbitrary OS signals or native stack exhaustion into recoverable C++ exceptions, and do not authorize arbitrary foreign-provider exception unwinding. A failed internal ownership/cleanup proof is not reclassified as a Guest fault.

An uncaught typed Wasm tag exception is contained only at the actual outer
native catch, after generated cleanup. Its owning value must reference a tag
in the current canonical source generation, and the real activation, GC and
checkpoint-shadow ledgers must prove complete retirement. It then uses the same
fault-stop and first-`proc_exit` ordering; exception payload bytes are not logged.
Wasm exceptions caught by Wasm handlers retain their normal language semantics.
Unknown native exceptions, stale tag identities and failed cleanup proofs keep
their original error path.

Authorized WASIp1 `proc_exit` uses a separate process shutdown coordinator. The first authorized exit code wins. Ordinary imports, 32/64-bit builtin entry paths, threaded imports and checkpoint worker interception all use that coordinator. A disabled namespace or missing `proc-exit` permission cannot request process shutdown; in a managed session the rejected Guest execution stops while STATUS remains available.

While authorized process exit is waiting for required owners, managed STATUS reports `process_exit_pending` (wire value 10), `active=false`, retaining the same installed/decision evidence. Business and update operations cannot report a new execution or reopen admission during that interval.

There is no global immediate/drain mode. Each explicitly limited operation selects its own exit action: `cancel` by default or `wait`. New host operations cannot acquire a waiting exemption after process exit was requested. An already-pending operation configured to wait keeps its original limits and must really finish; a second `proc_exit` cannot bypass it. The requester's own lexical scopes are excluded from the pending-operation census so it cannot wait for itself. Once all required owners have completed, the coordinator terminates normally with the first exit code. Existing wait owners do not receive a blanket runtime/group stop merely because another Guest called `proc_exit`.

Provider return boundaries join an already claimed process exit after their real
quota/native-wait scopes have retired. A cancelled native call therefore cannot
resume Guest code, hit `unreachable`, and cancel another operation's explicit
exit-wait. Qualified managed Guest-fault stops are ordered with the first exit
claim: an earlier fault transition completes before exit publication; a later
fault joins the existing exit instead of stopping its wait owners. The actual
stop callback runs outside the process coordinator mutex, since domain closure
may notify provider events through that same coordinator. Native invariant
failures remain fatal and are not reclassified as Guest faults.

The private threaded status-EH arena classifies an actual uncaught tag and exhaustion of its precharged payload arena as Guest failures. Both use the same process-exit ordering as validated traps: an earlier process exit keeps all selected wait owners, while an earlier Guest failure completes its actual stop transition before process exit is published. Invalid tokens, schema mismatches and native ownership violations retain the invariant-failure path.
