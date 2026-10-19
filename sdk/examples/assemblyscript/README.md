# AssemblyScript lessons

Compile one entry at a time using the [AssemblyScript build recipe](../../assemblyscript/README.md).
Use the matching entries in [ros1](ros1/README.md) for ROS 1; the four entries in
this directory select ROS 2. The advanced ROS 1 parameter example uses Int32 and
the ROS 2 example uses Int64 explicitly. Every lesson is a Wasm command
with `_start`, not a JavaScript source file interpreted by uwvm.

1. `01-raw.ts`: exact import names and u32/u64 argument widths, UTF-8 alias bytes,
   eight-byte handle output and one explicit close.
2. `02-codecs.ts`: String length prefix, synchronous byte copies and no publish retry.
3. `03-node.ts`: typed publisher/subscription, guest-local callback and one executor poll.
4. `04-resources.ts`: typed Trigger request/response, exact Int64 parameter and ROS clock.
5. `05-servers.ts`: transactional service/Action acquisition and owned-ID guest callbacks.
6. `06-events-tf-lifecycle.ts`: separately granted ROS2 parameter events, TF,
   filtered graph, explicit steady clock and lifecycle operations. ROS1 has a
   separate TF/graph/clock lesson and does not emulate events/lifecycle.
7. `07-scheduling.ts`: purely local copied-result Future, caller-time Timer,
   CallbackGroup and Context shutdown, with no native clock import or grant.
8. `08-action-parameters.ts`: Fibonacci send/own-cancel/feedback/result and
   BoolArray get/set/list, each exact alias and validated owned goal ID.

Lesson8 reads feedback, cancels its own goal, and then receives the result,
each once. Consuming a terminal result retires that ID in the Runtime, so a
later cancellation would be stale9. A false Bool is valid when the completed
result remains unread. Again11 is an idle read and never triggers a retry.

Configure the named aliases externally with exact operation/resource/type
identity/limits. Default runtime policy grants no ROS capabilities. Local diagnostic
exports do not automatically print and do not add log or clock rights. Actual SDK
compiler/runtime validation for this revision is coordinated serially in `/tmp`; these
prepared examples must not be described as an executed integration matrix.

The [Rust example host instructions](../rust/README.md#build-and-host-prerequisites)
also describe this backend's required real native peer, actual Worker PID/UID,
CLI grant fields and explicit type/byte/rate/queue/goal/parameter/frame/QoS bounds.
Lessons1..3 use `pub`/`sub`, so the reviewed String tutorial generator can produce
their exact Worker arguments and matching Gateway configuration. Other lessons
require their own operation schemas; a topic grant supplies none of them.
Each entry makes actual imports and inspects all cleanup statuses. Because
AssemblyScript `_start` is void, a returned diagnostic does not automatically
set an OS exit code: a behavioral harness must call lastStatus and require0,
or the application must explicitly trap on nonzero. A plain command exit alone
is not acceptance. No-op raw-retention entries live under SDK tests, not among
these eight application lessons. A valid default-deny backend reports13 from
opening an ungranted alias; failed Gateway preflight or a missing module is a
separate startup/load failure. No implicit logger/WASI clock/env/preopen is added.

For a caller-driven bounded loop, invoke spinOnce once per application step,
handle11 as idle, enforce a chosen maximum step count/explicit clock deadline,
and then inspect Executor.close and all Session.close statuses. Native publish0
or a warmup is not proof of matched delivery. These lessons perform one pass and
do not spawn peers, wait unboundedly, retry90, or infer cancellation rollback.

## Explicit unified events

The next runnable lesson uses an independently opened Source and `subEvents`
observer, checks a single poll, closes the Source, verifies its explicitly
permitted closed event and checks observer cleanup. See
[unified event v1](../../docs/unified-events.md) for exact aliases, header,
limits, backend entries, native peer prerequisites and policy preparation.
Metadata-only observation grants no payload permission. New38/43 inventories
and event source fixtures await the unified serial test stage; historical
receipts keep their original revision and narrower scope.
