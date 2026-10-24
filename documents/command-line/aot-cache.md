# AOT cache operation and measurement

Selecting a cache directory chooses storage; it does not by itself make a
process eligible to load or store native code. Use the current executable's
cache diagnostics when qualifying a launch. Historical binaries and timings
do not establish the behavior of a newly built executable.

On Linux, explicitly request the process seal before running an eligible
module:

```sh
uwvm2-ros --runtime-aot --runtime-llvm-jit-cache-seal-process \
  -Rllvm-cache-path path /absolute/private/cache \
  --log-verbose --runtime-compiler-log err --run application.wasm
```

The seal applies `PR_SET_DUMPABLE=0` and checks the actual result. It disables
ordinary core dumps and usual same-user ptrace attachment for this process.
The runtime does not restore that setting. Debugger qualification and cache
startup measurements therefore need separate launches. A seal failure stops
before guest execution; a cache refusal otherwise falls back to compilation.

The current authenticated cache additionally requires the actual executable
identity, signing support, kernel-origin admission, and an eligible runtime
publication. A supplied source identifier is not an admission token. Kernel
keyring restrictions, including a container's syscall policy, can prevent
admission. Changing the directory cannot fix that refusal. Do not disable
signature checks or relabel another executable to obtain a positive result.

The CLI registers ROS providers only when the main module or a Wasm preload
actually imports their namespace, including configured import rewrites. Merely
building with ROS support therefore does not disqualify an otherwise eligible
pure or WASI-only module. This does not grant cache eligibility to arbitrary
native ROS providers. A returning command also completes accepted asynchronous
writes before runtime reset revokes their source and generation admission;
waiting until process-static destruction is too late.

Inspect these records in the runtime log:

| Record | Interpretation |
| --- | --- |
| `object-cache-miss ... status=process-not-sealed` | The actual process remains dumpable. Use the explicit seal option where appropriate. |
| `... status=executable-identity-unavailable` | The running executable's identity could not be read. |
| `... status=executable-identity-mismatch` | The cache context does not match the actual running executable. |
| `... status=unsupported-signature` | This build cannot supply the required signature operation. |
| `... status=generation-bound-plan` or `debugger-active` | The generated object uses process-specific plan or debugger state and must be compiled for this launch. |
| `... status=experimental-gc-exceptions` or `native-eh-private-binding` | The selected experimental implementation requires fresh compilation. |
| `... status=origin-admission-unavailable` or `kernel-origin-unavailable` | The runtime or host refused origin admission. The cache remains unavailable. |
| `object-cache-store-enqueue ... status=ok` | A store was queued; this alone does not prove durable storage. |
| `object-cache-store-complete ... status=ok` | That asynchronous store completed successfully. |
| `object-cache-hit ... signature_verified=1` | The object was loaded with its required signature verified. |

For a warm-start measurement, keep the executable, guest bytes, host, and
code-generation options unchanged. Finish a cold process, confirm the store
completion and actual cache files, then start fresh processes. Count a warm
round only when its own log contains the verified hit and no object recompilation.
Report cold, missed, and verified-hit rounds separately. An empty directory,
`status=disabled`, or zero hit records cannot support a cache-hit timing claim.
These are native object-cache hits. Parsing, validation and other runtime
initialization still occur; a hit is not a claim that all startup work vanished.

WASI imports also require a runtime built with the WASI provider. A diagnostic
that the whole `wasi_snapshot_preview1` module is missing differs from a denied
operation on an available provider. Grant the required operation explicitly;
for example, `--wasip1-global-grant monotonic-clock` enables the clock used by
the focused AOT/WASI regression. Keep standard streams as pipes when testing
the supported WASI cache admission path; regular-file descriptors and additional
host resources can change eligibility.

The executable regression lives in
[`run_cache_wasi.py`](../../test/0073.ros-tests/support/source/runtime_repair/run_cache_wasi.py).
It preserves cold/warm logs and independent cache/WASI results. Test sources
are not a claim that the selected runtime has passed; inspect its result file
and the recorded executable SHA-256.
