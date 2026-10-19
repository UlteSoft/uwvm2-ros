# MoonBit progressive ROS examples

These twelve examples select exactly one ROS backend and use the reviewed
`sdk/moonbit` sources. On 2026-10-19 they compiled for `wasm` and `wasm-gc` in
Debug and default stripped Release. The48 actual baseline artifacts passed
validation, complete selected38/43 import signatures and scoped layout checks,
then executed with exact stdout and zero exit on the dual-ROS UWVM product.
The current Runtime profile leaves both ROS policies disabled and grants nothing:
eight ABI cases, eight well-formed default-disabled denial cases and32 local
empty-alias startup/cleanup cases passed. There is no positive native delivery
credit. See the [actual evidence](../../../test/0050.high_level_languages/results/native-budget/language-runtime-control-edges-20261019/README.md).

Prepare a fresh owned project by copying this directory and an exact reviewed
`sdk/moonbit` tree into its `ros-sdk/` subdirectory. The pinned compiler requires the new `moon.mod` manifest. The `moon.work`
members are this directory and only its exact local `ros-sdk` copy; the module
import resolves to that workspace member, with no package-registry fallback
needed. The copied SDK module name is `uwvm/ros`. Keep both workspace manifests
when staging and pass `--frozen` to the actual check/build command.
Select a package such as `00_raw_ros1` with the pinned producer. Final package
link options reserve the exact arena; build both `wasm` and `wasm-gc` debug and
release products serially under the caller's admitted process/resource owner.

| Package prefix (each ROS1/ROS2) | Real source behavior |
| --- | --- |
| 00_raw | Calls actual abi_version1 and emits one bounded ASCII success marker |
| 01_default_denied | Calls a well-formed valid-memory open twice with no grants; requires13 and an unchanged eight-byte raw sentinel |
| 02_topic | Optional explicit publisher alias; canonical String publication and checked shutdown |
| 03_service | Optional explicit client alias; one empty Trigger request and strict Bool+String response |
| 04_action | Four explicit independent aliases; bounded Fibonacci goal, actual returned ID and one own-goal cancellation |
| 05_executor | Optional explicit subscription; one bounded Guest-thread polling pass and strict canonical String callback |

All optional aliases are empty by default. Those examples then prove only
startup/local no-effect selection/checked cleanup when actually run; they are
**not** topic/service/Action/native-delivery passes. An owner must review the
source selection, matching explicit grant, native type/QoS/quota and independent
native observer before qualifying a nonempty selection. The files generate no
host policy. No cancel-any role, wildcard alias, network address or native node
identity is chosen by the SDK.

Every package exports `abi_coverage` only for complete import/type inspection.
Never call it with a valid index: its arguments are deliberate zero placeholders.
Each final module must have exactly the selected ROS namespace and all 38/43
signatures. The sole example stdio import is real WASIp1 fd_write. ASCII markers
are inert data and no control parser consumes them. One marker is at most256
ASCII bytes plus newline and uses exactly one fd_write, requiring the actual
nwritten to equal the complete request; errno, unwritten slots and short writes
fail without retry. Descriptor0..16 and data32..289 are separate from ROS bytes
at65536; verify every reservation against actual static segments and heap.
Do not use builtin println, which can add unsupported spectest imports.

Check the exact stdout marker and zero exit of a genuine invoked `_start`,
actual business namespace presence, enabled full-LLVM instruction-frame mode
where required, no timeouts and owned cleanup. Raw/codec tests and compiler
success are separate from Runtime/native ROS acceptance. Keep permission-denied,
unsupported imports/backends and invalid-memory failures distinct.

The initial real example build refused the deprecated JSON manifest before
compilation. The replacement module/workspace files preserve the module name,
version, preferred target and exact local dependency. Subsequent qualified builds
retain these files and the unchanged twelve application bodies. The rejected
attempt remains a separate historical result.

Pinned-toolchain module/workspace syntax is described in the official
[Moon module configuration](https://docs.moonbitlang.com/en/latest/toolchain/moon/module.html).
Keep the local workspace members when applying future toolchain migrations.


## Reproduce the build and default-disabled baseline

Use an existing Linux process/resource owner and the admitted pinned Moon tools.
Copy this directory to a fresh `PROJECT`, then copy `sdk/moonbit` to
`PROJECT/ros-sdk`. Keep both `moon.mod` files and `moon.work`. For each target:

```sh
MOON_HOME=ABS_MOON_HOME ABS_MOON_HOME/bin/moon -C PROJECT build --frozen \
  --target wasm --debug -j 1 --target-dir ABS_DEBUG_OUTPUT
MOON_HOME=ABS_MOON_HOME ABS_MOON_HOME/bin/moon -C PROJECT build --frozen \
  --target wasm --release --strip -j 1 --target-dir ABS_RELEASE_OUTPUT
```

Repeat with `--target wasm-gc` and distinct output directories. Each profile
produces twelve application Wasms and five known SDK library products; library
validation is separate from application execution. Validate each application
with the admitted `wasm-tools validate`. Check all selected ROS signatures
against `sdk/abi/abi-v1.json` and the sole WASI import `fd_write(i32,i32,i32,i32)
-> i32`. Never execute the inspection-only `abi_coverage` selector.

Build UWVM with `--import-ros1=y --import-ros2=y`. The actual qualified product
uses `--execution-int=uwvm-int --execution-jit=none --use-cxx-module=n`, Clang21
and `--debug-languages=python`; that flag selects compiled debug adapter code,
not the Guest language or permission policy. For the default-disabled baseline:

```sh
ABS_UWVM -Rint -WFE-gc -WFE-exceptions -WFE-tail-call \
  -WFE-function-references -WFE-reference-types -WFE-simd \
  -WFE-relaxed-simd -WFE-extended-const -WFE-multi-memory \
  -WFE-memory64 -WFE-table64 --wasip1-noinherit-system-environment \
  --run ABS_EXAMPLE_WASM
```

Require its source-declared marker plus exactly one newline, empty stderr and
zero exit. The `01_default_denied` memory sentinel must remain unchanged. This
profile tests disabled policy, not enabled zero-grant policy.

## Enabled and authorized native scenarios

An enabled target requires every explicit connection and denial-audit field
listed in [capabilities](../../../documents/ros/capabilities.md), including its
exact Gateway socket and peer UID. The Runtime authenticates and binds the
complete target policy during preflight, before Wasm start. A missing Gateway,
missing connection field or unsupported namespace is a startup failure, not a
successful denied open. The initial current enabled attempt was refused for a
missing node-name and has no execution credit.

For Topic, Service, Action and executor delivery, first select nonempty aliases
in the example source and rebuild all affected artifacts. Bind exact grants,
type identities, semantic/resource limits and backend-specific QoS through the
[capability model](../../../documents/ros/capabilities.md). Use the native
[Gateway setup tutorial](../c/ADVANCED.md) and an independent native observer
that checks complete values and denied side effects. Every Action role needs
its own grant; own-goal cancellation never implies cancel-any permission.
Preserve both successful and denied policy receipts and checked cleanup.

These native selections remain unqualified for this MoonBit cohort. Raw ABI and
byte-codec coverage do not establish full rcl/rclcpp/rclpy/rcljava parity, vendor
integration, ROS1/ROS2 mixed delivery, debugger support or VTune performance.

## Later enabled native telemetry qualification

The separate [receipt-gated telemetry application](../moonbit_native_telemetry/README.md)
adds four actual ROS2/Humble producer profiles repeated twice, with a native
subscriber and real ungranted-alias denial. Its source, policy and qualification
boundary are separate from the twelve default-disabled examples above. The
final policy also passed 24 enabled zero-grant ROS2 baseline executions.
