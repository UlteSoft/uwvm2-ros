# Terminal faults and deployer-controlled handling

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` and `-m debug` (including their long spellings) default to `full`; other launches default to `limited`. See [Debugger permissions](debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

Omit `--debug-fault-action`, or explicitly select `--debug-fault-action crash`,
to report an unhandled Wasm fault and then invoke
`fast_terminate()`. The operating system decides whether to generate a native core
dump, according to its resource limits and core-dump configuration. The VM does
not automatically write a Wasm dump, attach a frontend, retry an instruction,
restore a checkpoint or apply an update.

Select `--debug-fault-action exit` for an ordinary, process-wide exit with a stable
Wasm-fault status: 120, 121 or 122. This is a launch choice for terminal Guest
faults, not a recovery option. Native runtime invariant failures, invalid internal
buffers, unattributed native signals and unknown exceptions retain crash
termination, even with `exit` selected.

Select `--debug-fault-action pause` for an authorized host stop. If that terminal
stop is continued without a qualified retained manager, it completes crash
termination; it never retries the rejected operation. The values `crash`, `pause`
and `exit` are mutually exclusive; `crash` is the default. Repeating the option is
rejected. The old `--debug`
spelling is rejected; there is no default, dump or attach selector. The choice is
immutable across Guest calls, runtime reset, hot replacement and reconnection.

A pause is an opportunity for an independently authorized host supervisor to
inspect the stopped execution, explicitly request a dump, or coordinate a
permitted update. It is not authorization to repair and resume a rejected
operation. Every existing update admission, ownership, state and signature
check remains in force. No automatic recovery is performed.

## Reports and exit status

Before stopping, the runtime writes a multi-line human-readable report on
stderr, headed by `uwvm: [crash] CATEGORY`, followed by `Reason`, `Exit status`,
`Backend`, available numeric Wasm coordinates and the selected fault action.
The recorded `Exit status` is the fault classification and the code used by
explicit `exit`; it is not a claim that default crash termination is a normal
OS exit. Supervisors must inspect `WIFEXITED` versus `WIFSIGNALED`.
Unavailable coordinates are printed as `unavailable`. Memory-tag reports retain
available memory index, pointer, granule and tag evidence; existing colored tag
diagnostics remain human-readable. No JSON crash record is written on stderr.
A closed pipe, closed descriptor or other rejected write drops diagnostics
without replacing the selected runtime fault exit status with SIGPIPE or a C++
I/O exception. The writer preserves the calling thread's prior SIGPIPE mask,
pending signal and errno; it does not change the process-wide signal handler. The same Linux diagnostic
writer also protects boot warnings, validation messages and compiler log writes.
An unavailable inherited stderr does not throw during initial log-handle
acquisition; an explicit usable `--log-output` can still select a new sink.

The trusted embedding API `crash::snapshot_host_api(event&)` returns the immutable
first-fault record independently of log delivery. A trusted cold host may install
one `install_event_sink_host_api` callback and keep its context alive until all
execution has drained. Ordinary embedding callbacks run only for non-signal
reports. The `report_signal` path never invokes an arbitrary callback: it
publishes the immutable record and uses fixed buffers. The CLI binds its
already authenticated socket and live-peer pidfd separately; signal notification
uses only a zero-time poll and nonblocking, SIGPIPE-suppressed send. A signal
pause never waits for a report or pause that it may have interrupted on the same
thread. These native APIs have no Guest import. The existing
launch-authenticated `--diagnostic-control-fd` endpoint publishes one bounded
`uwvm.crash.v2` event before terminal handling, and later diagnostic snapshots
include it in the `crash` member. That endpoint retains its non-sensitive policy:
module/function/byte-offset coordinates and all memory pointer, granule and tag values are redacted. Its records are independent
of stderr and of the debugger's command/reply endpoint. A disconnected or full
endpoint may drop the event; fault handling never waits for an acknowledgement.

On Linux, this best-effort behavior also covers explicitly selecting
`--log-output out|err` or `--runtime-compiler-log out|err`. A closed standard
descriptor or a failed diagnostic descriptor duplication disables that sink
without replacing the Guest's eventual status with an uncaught exception.
The original standard descriptor, its shared file flags, and the caller's
signal mask remain unchanged. Opening an explicitly named log file still
uses the normal command-line configuration error policy.

The native event retains exact available numeric evidence. Standard trap
coordinates identify the real retained module/function where available; a native
code offset is never passed off as a Wasm byte offset. Missing instruction
coordinates remain unavailable. Asynchronous delivery is a separate flag:
software deferred checks can retain evidence copied at the actual access;
hardware asynchronous notifications with no attributable address/instruction
report those fields unavailable. Host consumers must inspect these validity
conditions instead of assuming the notification site was the fault site.

Backend 0 means unavailable or a shared host boundary, 1 means interpreter,
and 2 means LLVM. The first runtime fault owns the record and its status;
concurrent faults cannot replace it.

- **120 — unreachable:** execution reached the Wasm `unreachable` instruction.
  C/C++ `abort` and third-party assertions commonly lower to it, but the
  instruction alone does not prove intent or identify the original source.
- **121 — execution:** a specified Wasm execution fault, such as a bounds
  violation, integer trap, failed reference operation or uncaught Wasm exception.
  Invalid numeric conversion is a Guest trap, not evidence of a JIT defect.
- **122 — memory protection:** MTE detected a tag mismatch. The default
  synchronous mode rejects the access before permitting it. Explicit
  `--runtime-memtag-mode async` with `--runtime-memtag-backend software|hardware` delays fault delivery;
  an offending access may already have executed. This status does not imply
  that corrupted state can be resumed or that the attempted access was harmless.
- **123 — runtime internal record:** a detected runtime invariant or internal
  execution failure. It always retains native crash termination; selecting `exit`
  does not turn this record into normal exit 123. Arbitrary native corruption and
  unknown host signals also retain native containment.

Startup parameter errors and explicitly promoted warnings exit normally with
126; module loading/validation and diagnosed feature-admission failures exit
normally with 127. Their existing diagnostic identifies the option, file, module
or rejected requirement. These failures do not depend on `--debug-fault-action`.
The product diagnostic `fast_exit` uses process-wide `_Exit`, without static
destructors on partial initialization. It deliberately does not use Linux
`fast_io::fast_exit`, whose syscall ends only the calling thread. The existing native-stack guard has a separate contract: diagnosed
native stack exhaustion exits 127, and inability to install its guard exits 126.
Its fixed diagnostic also survives a closed stderr; those native guard outcomes
are not reclassified as a Wasm memory-bounds trap or an MTE fault. A Guest can deliberately request an overlapping exit value.
A supervisor must combine the OS wait result with the authenticated host event
or the embedding record. Guest output directed to stderr can imitate text;
stderr is not an authenticated management protocol. A disconnected diagnostic
consumer cannot be guaranteed delivery.

## Pause semantics and authority

On ordinary Linux execution, `--debug-fault-action pause` reports first and then uses
`raise(SIGSTOP)` on the faulting thread to stop the whole process. A process-directed
`kill(getpid(), SIGSTOP)` can select a different thread and let the faulting worker
exit before the group stop occurs. The real parent can observe `SIGCHLD` and
`CLD_STOPPED` with `waitid`, or a stopped status with
`waitpid(..., WUNTRACED)`. The VM never sends a crash signal to an arbitrary PID.
`SIGCONT` continues terminal handling; it does not retry the faulting instruction.
The human action line and `pause_requested` event field describe the selected
policy, not proof of a completed stop. An OS refusal records a failed stop rather
than a successful one, and terminal handling still cannot resume Guest execution.
The signal-handler path uses the same async-signal-safe, thread-directed primitive
without waiting for a report or stop that it may have interrupted.

An existing, launch-authorized LLVM controller uses its genuine cooperative
terminal pause. Its management thread remains available for explicit debugger
commands, including supported dump operations. Continuing that terminal pause
terminates Guest execution. The crash option never creates an internal
controller or a new listener. Debugger permissions remain unchanged, and an interpreter
does not acquire the LLVM debugger.

A whole-process `SIGSTOP` also stops management threads. To use the in-process
debugger, a deployer must provision the supported controller at launch;
an arbitrary later frontend cannot gain authority merely because the process
faulted. External native debugging still follows operating-system authorization.
Hot update is an explicit host operation, not a promise that a faulted activation
can be resumed.

In the qualified synchronous managed-update path, continuation after a whole-process
stop can retire the Guest epoch and retain the faulted management coordinator.
Software-async delivery instead follows its `noexcept` terminal callback: after
the supervisor continues the stopped process it completes crash termination, without
requiring the update owner to disconnect. Throwing across that callback or a native
signal frame to imitate managed recovery is not supported. Maintained regressions
in `test/0017.runtime/crash_pause_worker.cc` and `run_fault_pause.py` check actual
kernel stop/continue/exit outcomes, not just diagnostic text.

Normal execution without an authorized controller adds no dump/activation
probes. Selecting terminal pause within a selected controller retains the observations
needed for that controller's actual Wasm stop. Unknown native faults are not
converted into an unsafe C++ recovery operation.

## Explicit debugger dumps

Dump commands remain available through the authorized debugger. The deployment
controller chooses whether, when and where to export data; no crash option
creates a file by itself. On the authenticated debug command channel, use
`status` to obtain the current `stop-id`, then issue
`dump wasm STOP_ID /absolute/private/output.wasm`. The path is the remaining
command text, so spaces are literal. The same command is available in a DAP
IDE's debug console. It requires an exact current cooperative stop;
stale stops, native instruction parks, existing files and symlinks are rejected.
Export succeeds without resuming execution. A failed export retains the stop.
Full and read-only permissions export available captured state. Limited permissions export only explicitly redacted, non-sensitive metadata. None denies the operation.

## Coherent full and read-only capture

The collector accepts a genuine pause ticket and the expected runtime epoch. It obtains an actual execution-generation lease, transfers the complete cooperative pause to an internal move-only guard, and revalidates canonical source owners and function generations. It rejects stale tickets, incomplete stops, native-step parks and mismatched source ownership.

The pause guard prevents resume and mutation while output streams. The collector releases the runtime publication mutex before invoking the output writer. Each memory read takes its owner's existing snapshot pin and verifies the expected committed length.

Memory and numeric globals require both the real host-operation admission gate and the sealed source/provider census. If that proof is unavailable, the corresponding state is explicitly unavailable. A stopped Guest thread alone is insufficient evidence that a native borrower has stopped writing.

Independent memory owners are exported once. Instance index maps preserve imported memory/global aliases. A module with no local functions is authenticated through its membership in the same sealed source registry, rather than requiring a nonexistent generated-code engine. This read-only proof grants no executable restore or mutation authority. Native-provider memory/global imports without a qualified collector currently reject export as `unsupported_snapshot`; the collector never invokes an arbitrary provider to manufacture a snapshot.

Captured activation identities produce youngest-first Wasm frames. The leaf byte offset is translated from the runtime's expression-relative coordinate to the convention's function-body-relative coordinate, including local declarations. Unknown caller offsets are explicitly marked unavailable. Local values and operand stacks without an authenticated producer are marked missing/partial; the collector never reads an old native stack address or substitutes an invented value.

Memory-tagged owners are read by allocation granule. On hardware MTE, the collector reconstructs the physical tag for each granule under the actual owner pin. Mixed allocation tags are valid diagnostic data. The raw data and tag sections retain their independent completeness information.

## Output and failures

The writer creates a private regular temporary file with mode `0600`, exclusive creation and no symlink following at that temporary name. It streams data into that file, synchronizes the file, then publishes it using a same-directory operation that cannot replace an existing file or symlink. The parent directory remains bound to an open directory descriptor throughout publication.

An output failure is reported explicitly. An incomplete temporary output is removed before normal controlled process termination. Existing destination files are preserved. A failed explicit export does not authorize resuming a faulting instruction.

There is no aggregate coredump memory limit. A 4 KiB transfer buffer is an implementation buffer, not a truncation policy. Memory64 ranges too large for one standard Wasm section use repeated versioned `uwvm.core.memory-chunks` sections. Readers must inspect the metadata before treating an extended or partial dump as a complete standard snapshot.

See [Wasm coredump encoding](../runtime/wasm-coredump-format.md) for the standard sections, versioned extensions, missing values and reader requirements.


Schema v2 adds explicit native memory-field availability and the available tag-bit mask. Synchronous hardware MTE can retain the validated memory index and granule, but its native VA is not a complete Guest pointer. Its tag evidence has mask `0x0f`; an unavailable pointer tag stays unavailable, and the physical allocation tag is labelled a post-fault hardware snapshot. Software records retain their captured numerical evidence.

`read-only` and `limited` cannot pause or continue even a terminal fault. They may observe a launcher/fault-policy stop; only full debugger authority controls execution. Blocking stderr with a live non-reading receiver has no hard completion deadline. Authenticated event publication precedes the common human report, but ordinary text delivery may still block; an external supervisor remains responsible for that availability boundary.

Limited diagnostics retain numerical Wasm module/function/instruction coordinates
when the runtime has that evidence. `bt THREAD` on an authenticated stopped
debug session returns a separate numerical frame projection. It exposes no
module paths, symbol names, local values, registers or native PCs and cannot
resume or alter terminal execution. Caller byte offsets that were not captured
are reported unavailable rather than inferred from a later notification site.
The standalone crash event also retains available numerical coordinates while
redacting tagged pointers and memory contents. With no debugger/control endpoint
configured, crash pause remains an OS process stop for the owning supervisor;
it does not open a listener or grant a later unauthenticated attachment.
