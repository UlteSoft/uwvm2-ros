# Audit diagnostic qualification

This increment makes bounded WASIp1/ROS diagnostic loss and numeric IO results
observable. It does not establish a complete, durable security audit trail.
No request/response payload, guest-memory byte dump, native address/descriptor,
connection credential or arbitrary gateway detail text is added by default.
WASIp1 ABI arguments retain guest offsets as unsigned scalar bits.

The actual builtin WASIp1 wrapper journal records entry/return labels, errno,
thread/module/environment labels where observed, start time and return duration.
Original fd_read/fd_write/fd_pread/fd_pwrite and sock_send/sock_recv code supplies
only validated requested-byte totals and successfully published transfer counts,
for wasm32 and configured wasm64 variants. Early descriptor/permission errors
leave counts explicitly unknown. EOF and zero-iovec success publish known zero.
A native error after validated iovec admission may have a known requested count
and unknown transfer count. Successful close, renumber and rights changes get
explicit numeric lifecycle labels; path_open/sock_accept retain created-fd.

The fixed 512-row WASIp1 journal uses try-lock append. Busy, invalid metadata,
sequence exhaustion and returns retired by clear have distinct saturating
counters. Overwrite and explicit clear report cursor gaps, including initial
cursor zero. Sequence exhaustion cannot be cleared by trace enable. A missing
normal return remains an entry only; it is never synthesized as success.

ROS trace metadata adds a public ROS resource-name prefix (64 bytes, explicit
truncation) and stable ABI error categories. URI/secret-bearing resource text
is redacted. Arbitrary detail text remains excluded. Initial reads after ring
overwrite report a gap, empty filtered pages preserve their cursor, and
sequence exhaustion has a distinct counter from busy append loss.

Bounded JSON audit retention now configures transactionally, distinguishes
rate/size/overflow/allocation/unconfigured loss, saturates counters and preserves
retained records/caller output when drain allocation fails. Linux sink writes
retain delivered/dropped totals and expose per-reason failure counters. These
programmatic counters are not a replacement for the legacy sink's persisted
loss-marker/export design. The existing Host currently ignores the cumulative
`drain_audit` dropped result. Regular-file sink writes may block on storage;
nonblocking pipe/socket writes can fail or deliver partial records. Source string
construction before `record()` and formatting allocations remain outside its
internal allocation-failure catch. These remain audit-completeness gaps.

All test sources must be authored locally, then a whole frozen thin source
packet copied to Linux before execution. macOS compilation/execution is refused.
The Linux runner requires the parent-selected 64 GiB cgroup and effective
swap limit zero, runs compilers and native cases serially, records exact source
and executable hashes, and proves each executable's live `/proc/PID/exe` identity
while stopped in native main. It does not select or expose a ROS unwind mode.

```
python3 test/0082.audit-qualification/package.py --source . --out /absolute/new-packet
# Copy that entire packet to Linux, then run inside the existing bounded group:
python3 PACKET/test/0082.audit-qualification/run_linux.py \
  --source PACKET --out NEW_RESULT_DIRECTORY --cxx /selected/clang++
```

The runner builds native and ASan/UBSan versions of:

- `journal_contract.cc`: real diagnostic primitive contention, clear/off/filter,
  cursor/overflow, saturation, exhaustion, and worst-case reply bounds.
- `wasip1_io.cc`: actual builtin wrappers, native regular file and local sockets,
  both ABI widths, partial/EOF/zero reads, permission and descriptor errors,
  EAGAIN, lifecycle observations and default payload absence.
- `ros_audit_contract.cc`: actual bounded retention including injected allocation
  failure, ROS ring gaps/filters/outcomes, redaction and worst-case escaped page.
- `sink_contract.cc`: regular file, pipe, socket, full/closed/oversized/invalid
  and readonly sinks, explicit reason counts and preserved fd flags/errno.

A native nine-sample microbenchmark compares the actual fd_write wrapper with
tracing disabled against its original `base::call`, then measures an installed
numeric hook. Raw samples, medians, absolute extra nanoseconds and percentages
are retained. It does not infer full Wasm/ROS application overhead from this
zero-iovec workload. No performance acceptance threshold is invented.

The status is source-only until Linux `result.json` records actual execution.
Current full Runtime authentication, CLI/DAP transport, ROS1/ROS2 native gateway
calls and policy integration still require the parent's integrated qualification.
Existing unit suites and sanitizer components do not replace those checks.
