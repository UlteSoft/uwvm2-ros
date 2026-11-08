# Terminal faults and deployer-controlled handling

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` defaults to `full`; other launches default to `limited`. See [Debugger permissions](debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

Omit `--debug` for ordinary terminal handling. The VM reports the fault and
terminates execution. It does not automatically write a dump, attach a frontend,
retry an instruction, restore a checkpoint or apply an update.

The only selectable crash action is `--debug pause`. The launch choice is
immutable for the process lifetime. Guest instructions, runtime reset, hot
replacement and debugger reconnection cannot change it. There is no explicit
default selector and no automatic dump/attach action or dump-path option.

A pause is an opportunity for an independently authorized host supervisor to
inspect the stopped execution, explicitly request a dump, or coordinate a
permitted update. It is not authorization to repair and resume a rejected
operation. Every existing update admission, ownership, state and signature
check remains in force. No automatic recovery is performed.

## Reports and exit status

Before stopping, the runtime writes a single process-level JSON record on the
host's inherited stderr, prefixed by `uwvm: [crash] `. Schema
`uwvm.crash.v1` includes the category, exact available runtime reason,
exit status, backend, PID, pause selection and numeric Wasm source coordinates.
Unavailable coordinates are JSON `null`; native addresses and Guest-controlled
names or exception payloads are excluded. Memory-tag reports include available
memory-index, pointer, granule and tag evidence. Existing human-readable
diagnostics retain their detailed Wasm memory and colored tag information.

Backend 0 means unavailable or a shared host boundary, 1 means interpreter,
and 2 means LLVM. The first runtime fault owns the record and its status;
concurrent faults cannot replace it.

- **120 — unreachable:** execution reached the Wasm `unreachable` instruction.
  C/C++ `abort` and third-party assertions commonly lower to it, but the
  instruction alone does not prove intent or identify the original source.
- **121 — execution:** a specified Wasm execution fault, such as a bounds
  violation, integer trap, failed reference operation or uncaught Wasm exception.
  Invalid numeric conversion is a Guest trap, not evidence of a JIT defect.
- **122 — memory protection:** MTE rejected an access before permitting it.
  This describes the detection mechanism, not a claim that the attempted
  access would have been harmless.
- **123 — runtime internal:** a detected runtime invariant or internal
  execution failure. Arbitrary native corruption and unknown host signals
  retain native containment; they are not misrepresented as a managed Guest trap.

Startup parameter errors remain 126 and module loading/validation failures
remain 127. A Guest can deliberately request an overlapping exit value.
A supervisor must combine the OS wait result with its diagnostic stream.
Guest output directed to the same stderr can imitate text; this stream is not
an authenticated management protocol. Deployment must retain the host/Guest
logging separation required by its trust model. A disconnected diagnostic
consumer cannot be guaranteed delivery.

## Pause semantics and authority

On ordinary Linux execution, `--debug pause` reports first and then sends
`SIGSTOP` to the current process. The real parent can observe `SIGCHLD` and
`CLD_STOPPED` with `waitid`, or a stopped status with
`waitpid(..., WUNTRACED)`. The VM never sends a crash signal to an arbitrary PID.
`SIGCONT` continues terminal handling; it does not retry the faulting instruction.

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

