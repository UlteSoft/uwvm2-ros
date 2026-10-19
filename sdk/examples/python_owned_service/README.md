# Caller-driven CPython/WASI control examples

Read `sdk/python/OWNED_SERVICE.md` for the exact build, owner, wire and failure
contracts. Freeze `type_query.py` or `parameters.py` as `roswasm_app` with the
matching native CPython3.14.8 and the complete generated control package.

Both entries expose `main()`, use only the frozen `roswasm` package and drive
one real retained endpoint operation per explicit stdin command. No host Python,
ctypes/socket relay, implicit native identity or grant is substituted. First
supply role/plans/history; then select submit/take/respond/abandon/available or
close. Complete canonical requests/responses are lowercase hex, not a projected
subset of native records. The full seven Service family is supported: TypeQuery
and all six parameter services. Server handlers may also use the direct `run`
functions; they execute outside take/ACK.

Retain `drive.application` (or each direct function's `application`) on failure.

The frozen driver now stops at its first failed business command and returns1
even when all cleanup succeeds. `take` status11 without delivery/exception is
the explicit empty-poll exception; delivered controls still fail the lifetime.
The preallocated `application.command_failure` retains the first full outcome,
status and exact exception separately from cleanup/shutdown diagnostics. Raised
exceptions propagate unchanged. Check the actual process result and retained
evidence; printing a nonzero status is not success. Optional legacy open C
witnesses stay rooted by `application.factory.owners`, including a pending
zero-Python-handle owner whose real C fallback close already entered once.
It exposes the original primary exception, actual raw close results/exceptions,
Native attempt facts and any retained partial owner. Printing a delivery is a
post-consumption observation; failure cannot authorize another take/ACK.

These are authored Source examples, not executed Native or hardware results.

The optional legacy-open witness is an actual private GC-visible C type. Its
three strong Python edges, including the exact primary, are traversed. Dropping
an unreachable failed lifetime can therefore collect its traceback/capture
cycle without stripping a reachable exception or automatically invoking ROS.
The actual Python/C/GC regression remains unexecuted; see the full SDK guide.
