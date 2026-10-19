# ROS2 deletion-only ParameterEvent projection

This independent fixture tests `builtin/param-event-deletion-v1` with identity
`builtin:param-event-deletion-v1`. It delivers only an exact node/name match in
`deleted_parameters` whose native type is `PARAMETER_NOT_SET`. The version-2
event has kind 3, tag 0 and an empty payload. The notification does not attest
the deleted parameter's previous type, value or numeric bounds. The existing
nine typed projections continue to reject native NOT_SET deletion.

The grant does not authorize parameter reads or mutations, new/changed events,
or Topic operations. Numeric bounds, `finite` and `max-parameter-elements` are
inapplicable. Explicit QoS, audit, event-field, byte, queue and rate limits still
apply. `max-parameter-bytes` covers the complete envelope.

These scripts require a frozen complete source snapshot. Use a fresh output
path for every attempt; retain failed reports and executed driver bytes. Build
and run on Linux inside the established 64-GiB cgroup. Native execution uses a
private clone of the specified Jazzy rootfs, its own `/tmp` and ROS log directory,
and ROS domain 192. Do not point the output or short bind at an existing SDK,
source, evidence or test workspace.

```sh
python3 SOURCE/tests/ros_parameter_deletion/run_unit.py \
  --source SOURCE --out FRESH_UNIT_OUTPUT \
  --compiler CLANG --llvm-libs LLVM_LIBRARY_PATH

python3 SOURCE/tests/ros_parameter_deletion/run_cli.py \
  --uwvm FINAL_DUAL_WORKER --output FRESH_CLI_REPORT.json

python3 SOURCE/tests/ros_parameter_deletion/run_native.py \
  --source SOURCE --rootfs-workspace BASELINE_WORKSPACE \
  --gateway FINAL_ROS2_GATEWAY --uwvm FINAL_DUAL_WORKER \
  --out FRESH_NATIVE_OUTPUT --short-bind /pd192 \
  --llvm-libs LLVM_LIBRARY_PATH
```

`run_unit.py` runs six ASan/UBSan profiles: C++17/20/26 with libc++ and
libstdc++. C++17 covers IPC and Gateway contracts; C++20/26 additionally cover
Worker schema and SDK encode/decode. It checks an independent 42-byte golden
event, tag/kind/payload validation, transactional output and capability scope.
`run_cli.py` checks 37 actual global, single-module, group and ROS1 CLI cases.

`run_native.py` builds the native matrix and witness with raw compiler
dependencies, then generates and runs 19 Wasm cases when `--uwvm` is supplied.
Omit `--gateway` only to build an independent Gateway from the specified source.
The nine positive cases call the official AsyncParametersClient
`delete_parameters` API on dynamically declared parameters with different
prior kinds. Their declarations occur before subscription, so the projection
has no prior-value cache. Negative cases cover non-delete NOT_SET, concrete
typed deletion, wrong node/name, invalid timestamp, field/byte quotas and absent
authority. The native witness observes actual DDS reader identities, official
delete completion, exact wire bytes and reader teardown. The runner preserves
command logs, pre-exec records, actual-main executable identities, raw MMD,
source hashes, OOM counters and cleanup results.

The fixed ROS clock makes event bytes reproducible. Controlled negative events
use a same-writer valid marker; official SDK deletion is a separate positive
witness. A malformed event that arrives first fails the guest byte oracle.
The byte-quota case uses a distinct marker capability and expects no delivery
on the undersized capability.

The tests establish deletion notification semantics. They do not add remote
mutation authority or atomic conditional remote deletion. ROS2 has no remote
declaration service. Standard remote deletion uses SetParameters/NOT_SET for
dynamic parameters; it does not provide an atomic prior-kind/value condition.
Gateway-local parameter mutations keep their existing event-publication
settings. Actual results and compiler/profile scope come from each attempt's
report, rather than this fixture description.
