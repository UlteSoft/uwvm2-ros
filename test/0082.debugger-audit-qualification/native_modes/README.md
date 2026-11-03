# Native debugger mode qualification

Run this standalone public Xmake task only in an independently admitted Linux
executor. The existing R2 component project one directory above is unchanged.
Xmake exposes custom commands through `task` and `set_menu`, as documented in
[the Xmake task API](https://xmake.io/api/description/plugin-and-task.html).

```sh
cd test/0082.debugger-audit-qualification/native_modes
xmake native_modes --plan=/absolute/plan.json --plan-sha256=FULL_SHA256 \
  --executor=/absolute/admission_adapter.py --executor-sha256=FULL_SHA256 \
  --out=/absolute/new-output-directory
```

The two SHA values are mandatory. They identify inputs, not authority. The
trusted adapter's complete source/tool/input closure, live process identities,
current boot, actual memory/swap limits, common ancestor, canonical lease,
independent guardian and physical retirement must already be qualified by the
external executor. This task neither acquires a competing lease nor creates a
container or weakens limits. It has no default adapter and no simulation mode.

`execution.Executor` is the explicit version-1 boundary:

- `admit(stage)` returns an original live check before or after an operation.
- `verify_product(specification)` binds an exact binary to its original build
  and source identity. A hand-written JSON label is not build provenance.
- `command(argv)` returns the admitted command gate. The gate checks and records
  the actual PID immediately before exec, including the executed file SHA.
- `finish()` verifies the frozen inputs again and records the final live state.

The selected Python adapter exports `API_VERSION = 1` and `bind(plan)`. It is
host code selected explicitly by the executor, never a guest-provided plugin.
The runner owns each original child with a pidfd and keeps management open
until natural process retirement. Failures preserve the commands and logs.
The actual executed adapter and closure belong in the result's evidence bundle.

The `debugger-native-mode-plan-v1` JSON names explicit absolute tool paths,
`cpu`, `seed`, bounded `rounds`, `workloads` and `products`. Workloads are `smoke`
and `timed`; each can override `rounds`. Products must include `subject` and may
include `feature_off`. Each product supplies its pinned `binary` and original
provenance consumed by the adapter. A `feature_off` product runs only normal
mode and requires verified `features_compiled: true` and
`runtime_features_enabled: false`; absence is reported without inventing a
reference. Required tools are `wat2wasm`, `wasm-validate` and `wasm-objdump`.

The timed workload must explicitly name `"wasi_permissions": ["monotonic-clock"]`
in its plan. The driver passes only that required clock grant to every mode.
The smoke workload requires an empty list. Import visibility alone does not
authorize the VM's clock operation; a denied clock is a failed guest assertion,
never a timing sample.

For each workload the producer compiles exactly one Wasm, validates/disassembles
it, sets it read-only and verifies its whole SHA before and after every run.
The five subject modes are normal, launch-debug-idle, one unhit breakpoint,
two unhit breakpoints and one unhit site in an executed function. Installation
must return actual breakpoint IDs. No source/Wasm rewriting varies by mode.
Launch-authorized management is not arbitrary PID or mid-execution attachment.

The timed workload performs 1,048,576 dependent xorshift32 updates and stores,
then reads its final memory cells and checks the checksum. An independently
executed Python oracle produces that checksum and checks the nonzero state
invariant used by the related-function unhit site. There is no per-mode oracle.
The exact 48-byte guest output contains monotonic clock readings, checksum and
update count. Separate stdout/stderr files preserve that record unchanged.
The workload describes semantic calls/writes; the driver does not claim to have
audited their native occurrence or prevented every compiler optimization.

Timing fields keep cold launch, observed initial-pause management, resume to
natural retirement and the guest clock window separate. The guest window ends
before result output and control-thread retirement, and successful unhit cases
have no management pauses inside it. It includes the clock boundary calls.
A back-to-back clock calibration is reported without subtraction. No JSON
checkpoint I/O occurs between `continue` and pidfd retirement. Guest CPU time,
native pause-transition timestamps, hit/step costs, feature-off build provenance,
hardware PMU and external debugger baselines require their own evidence. Passing
this finite task never asserts complete audit or zero measurable regression.
