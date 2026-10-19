# C application examples

Start with exact raw imports (`01`), caller-buffer canonical encoding and checked
ownership (`02`), then Node/subscription/callback polling (`03`). Every main is
executable: client operations (`04`), queries/events (`05`), held actual relay
(`06`), service/Action servers (`07`), Context/Future/CallbackGroup/explicit-time
Timer (`08`), and stable Session cleanup/deliberate revocation (`09`). Advanced
applications select a documented compile-time case; no application uses an idle
main as API evidence.

Include `roswasm/roswasm.h` for the complete C application layer and select one
backend with `ROSWASM_EXAMPLE_ROS_VERSION=1` or `=2` (default). C11 needs only
caller-owned buffers and explicit owners. No Node/backend initialization opens
rights. Host policy starts empty except basic stdio, and only individually
reviewed CLI grants authorize native ROS effects. Default absent aliases return13.

Read [ADVANCED.md](ADVANCED.md) for exact recipes, compile-time cases, complete
CLI/limit generation and native prerequisites. [API_COVERAGE.md](API_COVERAGE.md)
and [API_COVERAGE.json](API_COVERAGE.json) map all38 ROS1 and43 ROS2 registered
imports to raw, typed and executable or explicitly identified contract-fixture
paths. service_submit is a source contract fixture plus separate Runtime/native
seven-event integration, not a claimed new service application. Coverage is source scope;
codecs/host models and Runtime/native acceptance are separate evidence.

`06_runtime_delivery.c` defines its own `_start`; compile it with the installed
WASI target/sysroot, `-nostartfiles -Wl,--entry=_start`. Other examples use normal
main/CRT recipes. The held relay requires actual native match before stdin P,
then one poll per external R token; its basic-WASI adapter checks every errno,
EOF and short transfer. It introduces no clock/random/environment grants. The
external supervisor must independently verify native bytes/counters/cleanup.

Close every acquired owner once, including error paths. Checked factories retain
primary setup and independent rollback errors; Session cleanup attempts all live
slots in reverse order even after107/110. Views borrow buffers; copy before reuse.
Empty queue11 is idle, unsupported95 is a real missing-version/native feature,
and neither proves successful ROS functionality. Local Context/Future/Timer
operate only under caller dispatch and do not create native context/threads/time
or undo an admitted synchronous RPC effect.

The runnable `10_action_client_bundle` adds explicitly configured four-role Action
ownership with an optional separate cancel-any alias, bounded goal-ID tracking,
checked close/revoke and deferred send. See [ADVANCED.md](ADVANCED.md) for0..4 cases and authority
boundaries. Pollable C++ resources also protect direct nested invocation and keep
mutable callback state alive through callback reassignment.

[Explicit policy coverage](POLICY_COVERAGE.md) maps every application/case to its selected aliases and one-shot lifetime boundaries.

## Explicit unified events

The next runnable lesson uses an independently opened Source and `subEvents`
observer, checks a single poll, closes the Source, verifies its explicitly
permitted closed event and checks observer cleanup. See
[unified event v1](../../docs/unified-events.md) for exact aliases, header,
limits, backend entries, native peer prerequisites and policy preparation.
Metadata-only observation grants no payload permission. New38/43 inventories
and event source fixtures await the unified serial test stage; historical
receipts keep their original revision and narrower scope.
