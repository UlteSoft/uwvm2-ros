# C++ application examples

The executable sequence is raw imports (`01`), move-only owner/canonical encoding
(`02`), typed Node/pub-sub/Executor (`03`), service/Action clients (`04`), bounded
server callbacks (`05`), queries/control (`06_queries_and_control`), held real
relay (`06_runtime_delivery`), local Context/Future/CallbackGroup/Timer (`08`),
and stable Session/checked close/deliberate revoke (`09`). Every advanced main
executes its selected operation. There is no export-only idle application entry.

Use C++20, the installed target standard library, `-fno-exceptions -fno-rtti`, `-O0 -g0`,
`-I sdk/include`, and one `ROSWASM_EXAMPLE_ROS_VERSION=1` or `=2` backend. The public
umbrella is `roswasm/roswasm.hpp`; `roswasmcpp.hpp` remains compatible. Typed
String/vector/function values allocate; exhaustion follows target-library policy
and cannot be reported as a recoverable ROS errno. The allocation-free C layer
and separate freestanding C++11 ABI owners remain available.

See [../c/ADVANCED.md](../c/ADVANCED.md) for exact per-case aliases, complete
CLI/limit policy generation, native services/Actions/parameters/TF/lifecycle
prerequisites, and real held-delivery recipes. [../c/API_COVERAGE.md](../c/API_COVERAGE.md)
and its JSON inventory map38/43 raw imports to checked owners, typed operations
and runnable entries. This is source/API coverage, not native rclcpp/rclc
source/ABI compatibility or blanket Runtime acceptance.

`06_runtime_delivery.cpp` defines `_start`: use WASI target/sysroot with
`-nostartfiles -Wl,--entry=_start`, retaining libc/libc++/libc++abi/builtins.
Do not combine the normal CRT main recipe with its custom entry, or remove the
standard libraries required by typed resources. Inspect final imports; extra
unexercised toolchain imports are recorded rather than silently granted rights.
Basicstdio gates require native match before P, at most200 one-poll R tokens,
exact canonical reply+one typed callback, checked close and raw stale9 sentinels.

Context and Node open no alias. Backend/Node outlive owners; resources and groups
outlive borrowed Executor registrations/deferred captures. Remove/cancel before
closing/moving them. Callbacks run on explicit spin after decoded storage owns
its data; nested spin is refused. Exception-enabled host builds release borrowed
scheduler/group state on unwinding. No-exception Wasm callbacks must return
normally. `completed_requests()` excludes a completion that throws even though
its Future may already be ready; inspect the Future's actual operation code.

Future work is bounded local scheduling over an existing synchronous RPC; it
executes once when the caller spins. Timer needs explicit caller time or an
independently granted Clock; there is no implicit chrono/thread/sleep. Shutdown
stops local dispatch and cancels pending work without native alias revoke or
Lifecycle transition. Every explicitly acquired resource has checked close;
server factory rollback retains independent primary/cleanup codes. Numeric13,
95,11,90,107,110,125 remain their documented negative/idle boundaries. Successful
local scheduling or one empty poll never proves native ROS delivery.

The runnable `10_action_client_bundle` adds explicitly configured four-role Action
ownership with an optional separate cancel-any alias, bounded goal-ID tracking,
checked close/revoke and deferred send. See [../c/ADVANCED.md](../c/ADVANCED.md) for0..4 cases and authority
boundaries. Pollable C++ resources also protect direct nested invocation and keep
mutable callback state alive through callback reassignment.

[Explicit policy coverage](../c/POLICY_COVERAGE.md) maps every application/case to its selected aliases and one-shot lifetime boundaries.

## Explicit unified events

The next runnable lesson uses an independently opened Source and `subEvents`
observer, checks a single poll, closes the Source, verifies its explicitly
permitted closed event and checks observer cleanup. See
[unified event v1](../../docs/unified-events.md) for exact aliases, header,
limits, backend entries, native peer prerequisites and policy preparation.
Metadata-only observation grants no payload permission. New38/43 inventories
and event source fixtures await the unified serial test stage; historical
receipts keep their original revision and narrower scope.
