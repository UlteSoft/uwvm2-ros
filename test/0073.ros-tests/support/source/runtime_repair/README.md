# Runtime cache, WASI, service reuse and debugger regressions

Run these tests on Linux inside the project's 64 GiB, zero-swap test cgroup.
Use the shared execution lease and matching VM/SDK source checkouts. These
tests do not contact physical robots or grant application ROS permissions.

The native gateway witnesses compile the production adapter against the
selected installed ROS2 distribution:

```sh
cmake -S "$VM_ROOT/test/0073.ros-tests/support/source/runtime_repair" \
  -B "$TEST_OUTPUT/gateway" -DCMAKE_BUILD_TYPE=Release \
  -DUWVM_ROS_SOURCE_ROOT="$VM_ROOT" -DUWVM_ROS_SDK_ROOT="$SDK_ROOT"
cmake --build "$TEST_OUTPUT/gateway" --parallel 2
"$TEST_OUTPUT/gateway/service-client-pool"
"$TEST_OUTPUT/gateway/native-service-reuse"
```

Source the selected distribution's environment before configuring or executing.
The pool witness checks concurrent ownership and contract separation. The
native witness uses a real Trigger peer and the production service adapter;
it observes actual RCL initialization rather than mocking client creation.
Twenty-one successive calls must use one client. A timed-out call and its
successor retain that client, while alias close causes the next call to create
a new one. This is an adapter regression, not a full Wasm/IPC authorization
qualification or a latency benchmark.

Build the current VM with the LLVM backend and its default WASI provider.
Build `llvm_jit_cache_refusal_diagnostics` with `--enable-test-llvm-jit=y`
and pass a nonexistent directory as its only argument. Refused operations
must preserve their specific diagnostic and create no cache files.

```sh
python3 -B "$VM_ROOT/test/0073.ros-tests/support/source/runtime_repair/run_cache_wasi.py" \
  --uwvm "$UWVM_BINARY" --clang "$WASM_CLANG" --out "$TEST_OUTPUT/cache-wasi"
```

If receipt storage has a different owner, supply a fresh `--cache-root` beneath
the VM user's private scratch directory. Cache ancestors must be trusted by the
runtime's filesystem policy; a root process cannot use another user's writable
receipt directory as its cache parent. Keep the receipt directory separate.
The positive cache cohort requires a Linux policy that permits the runtime's
kernel-keyring operations. A syscall policy that rejects `keyctl` is a separate
negative cohort: retain its refusal, and do not count the fallback execution as
a warm hit. The explicit process seal remains required in both environments.

The cache test uses new processes for cold and warm launches, an explicit
process seal and actual signature-verified hit records. A missing hit fails
the case even if the guest exits successfully. The independent WASI case
imports `fd_write` and `clock_time_get`, checking an explicitly granted clock
and a denied clock. The denied call must return Preview 1 `ENOTCAPABLE` (76)
without changing the timestamp, and the guest must report that observed result.
A crash or a command-line error cannot satisfy the refusal case. Commands,
output hashes and original failures are retained.
The WASI cache cohort repeats the cold/write/two-hit sequence with those real
imports, then removes the clock grant in a fresh process using the same cache
directory. That process must still observe `ENOTCAPABLE`; code reuse cannot
retain a previous process's authorization.
The pure guest returns normally, so its completed cold write also checks the
ordering between the CLI cache writer and explicit runtime reset. Cold enqueue
success alone cannot satisfy this regression.

`run_live_disassembly.py` uses the matching runtime, secure broker and DAP
adapter to exercise real native instruction windows and stepping. It requires
a native-debug build, a complete source manifest and the build receipt emitted
by the controlled Linux batch. It verifies the executable hash and source
bytes before execution. Its native broker witness is shared with
`test/0018.debugger/run_dap_current_broker.py`; the result does not establish
VS Code user-interface acceptance.

Keep raw outputs outside tracked source directories. A source file or a mock
DAP formatting test never establishes runtime cache, native ROS or live
disassembly qualification; use the corresponding real execution receipt.
