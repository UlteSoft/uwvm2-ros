# Native gateway audit I/O tests

The native gateway headers remain C++17 for Noetic/Jazzy. The vendored fast_io concepts API requires C++20; this adapter uses the existing `fixed_text` / `to_chars` formatter and `owned_fd`. It changes only budgeted `ros_audit` output in `server.h`. Other `std::cerr` diagnostics and middleware logging are outside this fix.

```sh
python3 tests/ros_native_audit_io/run.py --work /tmp/uwvm-ros-native-audit-test --compiler clang++
```

Run on Linux in a readable cgroup v2 hierarchy with an effective 64 GiB limit. The driver requires a fresh work directory and checks an 8 GiB compiler estimate plus a 16 GiB reserve. `--source` defaults to this repository. `--audit-source` optionally names an explicit source root for the two audit headers; the configuration and remaining dependencies come from `--source`. This overlay is recorded in the actual argv and snapshots. `--compiler-library-path` can select the compiler/runtime libraries.

Three fresh C++17 profiles run 28 scenarios each: libstdc++ normal, libstdc++ ASan/UBSan, and libc++ ASan/UBSan. The SAN executables are non-PIE. The actual native probe stops inside its ELF while the driver captures PID, parent, cgroup, device/inode and pinned-FD SHA; each resumed test has a three-second fixture timeout. This does not make ordinary files, mutexes or middleware hard-interruptible.

`write_audit_text` accepts a borrowed complete record including its newline. It pins stderr, checks its identity, and reopens pipe/TTY descriptors through the pinned proc FD into an independent nonblocking OFD. Socket output uses `MSG_DONTWAIT | MSG_NOSIGNAL`. Original FD flags, caller errno and SIGPIPE handler/mask/pending state are preserved. A pipe record within the 4,096-byte nonregular output bound and `PIPE_BUF` is accepted atomically or wholly dropped. Oversized nonregular records are rejected before writing. Socket/TTY failures can leave a partial record and return false. Regular files preserve the original shared offset/append behavior; they can block, and a write failure can leave partial output.

The server retains the existing record format and quota accounting. A false output result increments its existing saturating `suppressed` count; no new summary records or Guest ABI are added. `completed_calls` / `failed_calls` in the probe are test-side observations, not production counters. The tests also call the actual budgeted server audit function for regular, writable/full/closed pipe and quota/saturation cases. Concurrent pipe tests verify 64 complete records. Helper and budgeted audit calls measured by the probe allocate zero C++ heap blocks; thread/fixture construction occurs outside that measurement.

Original actual evidence is under `/home/macromodel/.local/share/uwvm-ros-extension/`:

- `20261013-deadline-audit-native-before-r1`: confirmed two counterexamples from exact old server SHA `965c7b192b58d09f16786956cd848f87cb92b441234cba76234cbb1cc749ad3e`. A writable pipe's shared flags change from 1 to 2049. With default unblocked SIGPIPE and a closed reader, the process returns -13 before the audit function returns. Its PASS status means both counterexamples were observed, not that the old behavior is correct. Report SHA `d3195eda83f1c0a11c5144bc8e1024f381c3157cf126b1a654ab8dcf2ea0d558`.
- `20261013-deadline-audit-native-after-r1`: actual Clang 23 C++17 builds and all 84 scenarios PASS, with two readable cgroup ancestors showing zero OOM deltas and no remaining owned child. Report SHA `ee927c4608f3480eab925727f422f755756101fc8be26dcdecd3de6d41527e1e`. Both the helper and exact server audit function are compiled in this test. Raw MMD, source snapshots, argv, logs and readable sink bytes are retained.

These are standalone native audit tests. They do not execute ROS/DDS or a complete Gateway, and do not claim GCC9/GCC13 sanitizer instrumentation of the whole native SDK.

The current source additionally caches the four explicit numeric grant audit
settings when that grant limiter first reaches its existing initialization
point. The limiter belongs to one connection's request thread and is bound to
the original configuration and configured capability entry. The trusted loader
rejects duplicate aliases and fields and validates every entry; production
Gateway code retains the configuration unchanged until all session threads
join. These borrowed bindings do not escape that lifetime. A foreign capability
or a limiter reused with another configuration is refused. There is no process
wide cache, new mutex, implicit setting or reusable adapter grant cache.

Mode filtering and global refusal still precede grant initialization. Close,
reopen and revoke preserve the same grant audit counters. The session-wide
global window, grant token refill, lifetime record/byte limits, saturating
suppression and accounting before output remain unchanged. Request operation
and alias remain dynamic metadata, and payload bytes are excluded from audit
records. The output helper and its existing nonblocking pipe/socket behavior
are unchanged. The historical 84-scenario receipt above qualifies its original
source version; this numeric-cache change needs fresh native compilation and
execution before it receives that qualification.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -B tests/ros_native_audit_io/cache_models.py \
  --report /tmp/NEW_AUDIT_CACHE_SOURCE_MODEL_REPORT.json
```

This separate check runs only Python source checks and synthetic accounting
models. An independent history oracle derives token allowance from all admitted
record suffixes, while a rational-credit projection compares repeated numeric
parsing with one immutable cache. Fractional rates, exact burst/record/byte
boundaries, window rollover, output failure, multiple aliases, unknown aliases,
mode filtering, suppression saturation and configuration binding are covered.
Its conversion counts describe the Python model; C++ parser instrumentation,
native output behavior and a performance benchmark remain unexecuted.

`cache_unit.cc` is a separate C++17 contract calling the actual audit function
with explicitly validated grants stored in a real configuration map. It checks
all four cached values, original-entry binding, persistent record/byte quotas
across dynamic close/reopen metadata, independent aliases, filtering and global
refusal before initialization, and refused foreign configuration/capability
bindings. Large explicit bursts keep these checks independent of exact token
timing. After initialization it instruments standard scalar, array and aligned
`new` calls only during audit calls; no request construction is measured. A zero
count qualifies that C++ allocation scope, not kernel/C allocation or latency.
The fixture owns a temporary stderr `/dev/null` redirection and creates no ROS
adapter or Guest. The existing 84 I/O writer scenarios remain unchanged.

The additive opt-in ARM recipe is:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -B CURRENT_SOURCE/tests/ros_aarch64/run_unit.py \
  --source CURRENT_SOURCE --manifest CURRENT_SOURCE_MANIFEST \
  --config CURRENT_ARM_COMPILER_CONFIG --out NEW_AUDIT_CACHE_UNIT_OUTPUT \
  --suite native-audit-cache --sdk-manifest CURRENT_CROSS_SDK_MANIFEST \
  --compiler-resource-root CURRENT_COMPILER_RESOURCE_HEADERS
```

Use the verified Linux 64 GiB cgroup and current qualified QEMU/compiler inputs.
This selection requires actual full `-MD` source/SDK dependencies, a C++17 object
and linked ARM image, its stopped live mapping, and one exact cache completion
marker. It does not enter the default six-original/seventeen-WASI inventories.
This new native fixture is prepared source only until fresh execution succeeds.
