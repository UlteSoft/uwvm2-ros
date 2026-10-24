# Owned Direct3 composition with real Wasm preloads

This is an explicit simulator-only alternative to the ordinary standalone
Direct Go2 route. C, C++ and embedded JavaScript retain their own main-module
ROS2 SDK owners, complete generated GO24 codecs, actual LowState subscription
and LowCmd publication. They compose these separately instantiated modules:

1. `callbacks` exports four scalar functions and imports nothing.
2. `motion` imports exactly those four callback functions, calls the callback
   instance twelve times in joint order, and exposes twenty computed motors.
3. `perception` imports nothing. It receives the 36 real decoded LowState
   q/dq/tau_est binary32 words, checks ordering and finiteness, and validates an
   independently accumulated value checksum before retiring its registration.

Main calls `perception`, then `motion`; `motion` calls `callbacks`. All values
are explicit i32 scalars. Every module has its own memory. No C++ object,
pointer, ROS capability, native handle or native callback crosses a module
boundary. Only main computes the official LowCmd layout/CRC and performs ROS
operations, after the complete cross-module call stack has unwound. The
Runtime's existing cross-module host-call guard is unchanged.

`perception` is joint-state perception, **not image vision**. The current real
Direct input protocol provides LowState joint words and does not deliver RGB
frames to a Guest. Actual camera-image processing needs a separately selected
frame protocol, owned image inputs and corresponding native comparison; this
route does not claim that result.

## Explicit selection and actual compilation

Root first freezes a complete source revision containing these files. All
actual operations belong inside the selected original 64-GiB, zero-swap
cgroup. The existing serial compiler guard, RSS/deadline limits, pidfd owner,
all-ancestor eight-GiB backing reserve and bounded cleanup remain in force.
No command below starts a physical robot or obtains a hardware permission.

The existing metadata maker gains an explicit option:

```text
python -B SOURCE/test/0073.ros-tests/support/source/ros_unitree/acceptance/make_direct_build_configuration.py \
  --request ACTUAL_REQUEST --request-sha256 ACTUAL_REQUEST_SHA \
  --maker-sha256 ACTUAL_MAKER_SHA --composition modular
```

`SOURCE` and every `ACTUAL_*` operand above are operator-selected existing
observations, not literal executable placeholders or predicted future pins.
The default option is `standalone`. Root supplies the same actual qualified
WASI C/C++ toolchain, complete archives/QuickJS, current cgroup and original
completed Typed14/native-family inputs as the standalone maker. The maker
publishes a new sealed selection with kind
`unitree-direct-go2-modular-build-selection-v1` and an unexecuted build argv.
It does not compile or run a Guest.

Root then executes that exact published argv. The maintained
`prepare_direct_guests.py` performs real serial work:

* Compile, link and validate three fresh pure providers with full `-MD -MF
  -MT` evidence, actual linker trace, pinned archives and actual binary tables.
  Each provider has one private two-page wasm32 memory, no start section and
  exactly the named scalar import/export set. Neither WASI nor ROS imports are
  permitted in any provider.
* Compile and validate the real denied/positive main for each of C, C++ and
  embedded JavaScript. Positive mains select `UWVM_UNITREE_MODULAR_MOTION=1`.
  The new owned graph also selects `UWVM_UNITREE_MODULAR_PERCEPTION=1`.
  Historical C++ `MOTION=1` with omitted/default-zero `PERCEPTION` retains its
  literal two-provider motion/callback path and original header contract.
  Denied mains select both flags zero, contain no provider imports, and receive no preload
  artifacts at execution. The local entry hold remains before application
  initialization. Main memory remains bounded to 64 MiB.
* C and C++ include `unitree_modular_control.h`. The embedded-JavaScript main
  uses its genuine QuickJS module map and ROS SDK; its same-main C adapter copies
  exactly 144 bytes from a genuine Uint8Array before entering the pure Wasm
  graph. No host JavaScript, native plugin or substitute transport is used.

The successful build status is
`PASS_SIX_DIRECT_GO2_MODULAR_COMMANDS_AND_THREE_PRELOADS_COMPILED_VALIDATED_NOT_EXECUTED`.
There are six command products plus three provider products. This is binary
preparation only: no DDS, Runtime, MuJoCo or performance result is implied.

## Current generator and retained native-family provenance

The old Typed14/native reports retain their original source labels, flags,
identities and statuses. A changed current generator cannot be authorized by
an old `current_emitter_bytes_verified` flag. For this explicit modular route,
the prerequisite instead reopens the complete original Typed14 source and
manifest and binds the original generation's emitter hashes to that source.
The ordinary standalone route retains its exact current-generator requirement.

The owned build then actually runs the **current** complete generator on the
original pinned native-bound GO24 catalog in a fresh owned directory. It
requires all 24 records/types/native IDs/canonical codec IDs, original catalog
and definition digests, and **all eleven generated output body digests** to be
identical to those used by the independently qualified native providers. A
different output, missing language module or changed codec fails and requires
a separately qualified native rebuild; it is never silently accepted.

A distinct `uwvm-unitree-current-go24-generation-admission-v1` receipt retains
the real generator command/logs, current emitter pins, original source/report
roles, original generation and native catalog, fresh generation and all eleven
fresh output pins. Each main receipt binds that exact admission and its exact
three provider products. C/C++ full-MD evidence must consume the fresh current
generated header; JavaScript's actual embedded source-copy ledger must consume
the fresh generated module. The physics owner independently cross-binds the
original generation to its real exporter/full-MD/Worker/Gateway admission.

This is current source regeneration with byte equivalence to compiled native
provider inputs. It does not claim that native binaries were recompiled or that
old process identities survived a reboot. Old documents are never rewritten.

## Fresh physics metadata and actual ownership

The maintained DATA/helper module `direct_modular.py` exposes:

```python
result = graph.configuration(frame, frozen_loader,
    base_config_input, base_physics_input, completed_modular_build_input)
```

The caller supplies an already admitted metadata-only Frame, loaded from the
complete frozen manifest. Its inputs are an actual unexecuted standalone
baseline configuration, that baseline's unforecast namespace physics template,
and the completed current modular build report. All original complete input
roots plus the exact complete modular producer root must already be selected.
The helper reopens actual pins, appends distinct explicit artifact/receipt
labels, preserves existing baseline inputs/roots, and writes two fresh metadata
files under `frame.out`. It starts no actor, predicts no namespace and returns
their actual full-seven pins. Root finishes the Frame normally to preserve all
selected inputs and roots before using either output.

Use the returned configuration and physics-template pins as the unchanged
CP310 stager's actual `run_inputs.baseline_config` and
`run_inputs.physics_template`, together with its genuine completed view receipt
and existing explicit read-only bind roots. The new physics kind is
`unitree-go2-three-modular-direct-physics-v1`; its sole added field is the exact
ordered `libraries` list of `callbacks`, `motion`, `perception`, each with a
distinct actual artifact label and build-receipt label. Root then uses the
existing reviewed CP310 `--phase run` stager/outer-owner flow. That owner
observes the new private loopback namespace and owns all mutable paths; the
metadata helper does neither.

Every positive Runtime argv explicitly creates and disables WASI/ROS2 contexts
for all three providers and preloads only their admitted artifacts in dependency
order. No provider receives main's rights. Global contexts remain disabled;
main receives only its existing basic stdio plus the explicit granular LowState
subscribe/LowCmd publish grants. No clock, random, filesystem, raw socket,
environment inheritance or thread permission is introduced. Denied runs load
no provider and retain the independent zero-command/zero-step quiet witness.

Gateway, Worker, native controller, simulator and independent witness remain
separate owned processes. The exact native controller still compiles the same
`simulation_control.h` kernel and official CRC contract. Real Guest callbacks,
payload bits, DDS publication, physical states and final native-vs-Wasm replay
tolerances remain mandatory; no host observer fills in missing Guest work.

The complete successful result has kind
`unitree-three-modular-direct-Guest-controlled-physics` and status
`PASS_THREE_ACTUAL_MODULAR_GUEST_NATIVE_DDS_MUJOCO_CONTROL_CASES`. It requires all
three language cases, six actual matched roles and three default-denied quiet
witnesses. Performance requires the unchanged sample count/timing criteria.

## Profiling and tests

The existing explicit Direct3 VTune route accepts either a genuine standalone
baseline or a genuine modular baseline, preserving its exact kind, artifacts,
provider graph, grants and measured inputs. It cannot substitute a standalone
baseline for a modular run. It reuses the actual mapped Worker/Gateway hooks,
same-birth pidfd attachment, pre-bootstrap collector readiness and liveness
through the last real physics state. Hardware profiles do not qualify speed.

`test_direct_modular_policy.py::suite(graph)` defines 26 pure DATA cases. A
guarded Linux test owner must supply the actual manifest-bound graph module;
the file performs no import/compile/test on its own. Its separate
`contract_suite(simulation)` supplies four production-contract regressions for
the historical two-provider C++ route and the new owned three-provider route. Cases reject widened or
reordered provider graphs, unlisted/ROS/WASI imports, wrong signatures,
unbounded/imported memory, implicit starts, duplicate imports and altered
current-family identity/codec/definition/output claims.

`unitree_composition_protocol.c` is a real-Wasm protocol fixture for the same
three preloads. Root must compile/validate it, inspect its actual local export
index and invoke `composition_protocol` with every module's ROS/WASI contexts
disabled. It checks rejected inputs, busy/stale IDs, partial/invalid perception
words, independent checksums, checked abort/end and actual motion/callback
retirement. A violation traps. This fixture is not a physics result.

All those tests and real compilation/control/profiling remain unexecuted until
the Linux owner runs them. The source-only patch records no future manifest,
artifact identity, hardware event, successful test count or native-speed claim.
