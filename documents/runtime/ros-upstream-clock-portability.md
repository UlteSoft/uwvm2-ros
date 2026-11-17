# Upstream ROS time-source portability

The reproducible source slice in `tools/ros_scene/upstream_clock_portability.py`
executes actual Jazzy RCL, RMW and rcutils C implementations as native code and
as a Wasm32 guest. It qualifies these sources and their exercised semantics;
it does not qualify complete packages, executors, DDS, ROS graph I/O, or the
entire acquired ecosystem.

## Actual upstream implementation

The runner verifies each downloaded archive against its existing acquisition
receipt and extracts a fresh test snapshot under SDK `.tmp`. Source snapshots
are verified unchanged at completion; filesystem immutability is not enforced. The
selected archives are Jazzy `rcl`, `rmw`, `rcutils` and `rosidl` (the last
supplies transitive type headers). Archive identities, URLs, checksums,
compiled source checksums and upstream header checksums are saved in
`results.json`, with the original acquisition receipts beside it.

The compiled upstream sources are `rcl/src/rcl/time.c`, `rmw/src/time.c`, and
rcutils `src/time_unix.c`, `src/allocator.c`, `src/error_handling.c`,
`src/strdup.c`, and `src/snprintf.c`. Their contents are unmodified. The
rcutils logging header is generated from its real Jazzy Empy template using
the actual Empy implementation; no generated-header stub is used. Optional
diagnostic printing from rcutils error-handler internals is disabled equally
in native and Wasm builds, while error state and return semantics remain.

## Deliberate adapter boundary

For the Wasm build only, the POSIX `clock_gettime` reference in upstream code
is redirected at compilation to `upstream_clock_adapter.c`. The adapter
accepts only realtime and monotonic clocks, corresponding to exact `system`
and `steady` deployment aliases. It opens a capability, creates at most one
clock for that alias, and reads the checked four-word `clock.read` result.
Unknown clock types fail. It grants no source update, sleeping, raw clock,
path, socket or fallback authority.

Interface exposure, capability creation and reading are separately authorized.
Each grant is bounded to one open capability and one runtime resource.
Ordinary stable grants impose no rate limit. A separate negative case opts
into `delivery=may-reject` and a one-call, sixty-second budget: creation
consumes the admitted call and the subsequent read fails. This follows the
zero-authority → explicit stable authority → explicit attenuation sequence.

The existing WASI libc is used for local allocation and string/error support.
Its incidental `clock_time_get`, `fd_close` and `fd_seek` imports are explicitly
denied. Only `fd_write` is exposed with `stdout.write` authority for reporting;
no general WASI clock, file or network capability is granted. The runner
disassembles both guests and rejects any unexpected import.

ROS-time override values and jump callbacks stay in the genuine upstream
`rcl/time.c` object. They are local RCL semantics in this slice, not a claim
that shared scene ROS-clock synchronization or ROS `/clock` subscription has
been ported.

## Qualified behavior

The native and Wasm positive executions produce byte-identical transcripts
after 215 checks. Dynamic physical timestamps are checked by positive-system
time and nondecreasing-steady-time invariants; their absolute values are not
compared across independent processes. Deterministic checks cover:

- ROS-time activation, deactivation, zero/not-started state and exact overrides.
- Threshold filtering and ordered before/after jump callbacks, including
  forward and backward jumps and duplicate/removal errors.
- Positive/negative differences and rejection of incompatible clock types.
- RMW normalization, nanosecond carry, infinite/negative-duration conversion
  and saturation at the signed 64-bit nanosecond limit.
- Null-argument error behavior and preserved rcutils error state.

The same native sources pass AddressSanitizer and UndefinedBehaviorSanitizer
and produce the same transcript. Three Wasm refusal configurations pass ten
checks each: no resource grants, create permission without read permission,
and explicit rate attenuation. They leave the caller's timestamp untouched
and set the upstream error state.

One upstream distinction matters when translating errors: Jazzy RCL's
physical-clock callback directly forwards `RCUTILS_RET_ERROR` (2), whereas
`RCL_RET_ERROR` is 1. The fixture preserves and checks the actual forwarded
code; it does not reinterpret a refused physical clock as an RCL error of a
different numeric value.

## Reproduction and evidence

Run the script through `tools/ros_scene/in_existing_cgroup.py` and bind it to
CPUs 8–15 with `taskset -c 8-15`. The runner rejects a different cgroup, memory
limit, swap limit, affinity, or output outside SDK `.tmp`. Pass the acquired
corpus, a reviewed Wasm32 sysroot, its compiler-builtins archive, real `em.py`,
the selected VM and a fresh output directory. `--help` lists the arguments.

Final evidence for this run is in SDK
`.tmp/ros-scene-security/upstream-clock-integrity-final/results.json`. It records 13
successful commands, the selected `vm-local-callback/uwvm2-ros` digest, exact
compiler/linker/library hashes, source receipts, guest imports, transcripts
and zero cgroup OOM/limit events. Completion also requires unchanged fixture
sources and copies, upstream source/header files, generated logging header,
guest artifacts and VM executable. A changed, removed or unreadable recorded
input saves `complete=false` and makes qualification exit unsuccessfully.
The earlier `upstream-clock-final` receipt is retained separately.
`complete=true` means only the source-slice
and refusal checks above completed; it is not a whole-package portability or
performance certification.
