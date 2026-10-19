# Direct3 hardware profiling in a fresh owned CP310 namespace

This entry closes the measured Direct3 report-to-VTune interface. It profiles one
of the actual `c`, `cpp`, or embedded `javascript` Wasm controllers through the
same maintained Go2 control coordinator, real interpreter exec gate, actual
Gateway subscription, independent native state witness and MuJoCo stepping
protocol. It is separate from the existing modular and older simulation profile
entries, whose command lines and admission contracts remain unchanged.

A completed profile does **not** establish native speed. The measured quantity
is caller-paced ROS/Gateway/physics round-trip latency, including the supervisor
and discovery barriers. VTune perturbs this quantity. A later source optimization
must be followed by a new unprofiled matched native/Wasm measurement before a
speed claim can be made.

## Prerequisites and preserved baseline

Root first freezes the complete source including these files and runs the normal
Direct3 baseline with `profiler: null`. All three baseline languages, their six
actual native/Wasm controller runs, three independent default-denied quiet
observations, full actual timing samples, source/input/whole-root preservation
and normal owned cleanup must complete. Baseline status must be exactly
`PASS_THREE_ACTUAL_STANDALONE_GUEST_NATIVE_DDS_MUJOCO_CONTROL_CASES`.

Profiling requires an actual completed baseline from the **same complete source
manifest**, the same current task cgroup selection, unchanged Worker, Gateway,
Guest artifacts, model, native controller, CP310 content view, codec policies,
input bytes, step count, warmup, trigger and control kernel. It reopens the
baseline's actual full-seven input pins. It preserves the raw original report,
original configuration, original physics selection, original case and original
cgroup observation as separate immutable inputs. A failed or running report,
partial language inventory, insufficient samples or another language's slowdown
cannot authorize collection.

The selected language must have at least thirty real post-warmup samples for
both roles. The entry always recomputes the original nearest-rank timing
summaries, measured ratio and trigger from retained samples, and checks that
the baseline's original `profiling_required` bit agrees with that ratio. No
profiling request rewrites those original timing facts.

The optional host recipe `collection_policy` has exactly two values:

* `measured-slowdown` is the existing default when the field is omitted. The
  selected language must exceed its original predeclared trigger; another
  language's slowdown or a top-level flag alone cannot authorize collection.
* `required-hardware` explicitly requests both actual hardware collections even
  when the measured language is fast or exactly at its trigger. This policy is
  selected by Root for a human-requested hardware investigation. It removes
  only the slowdown threshold admission. The successful full same-manifest
  baseline, raw samples, equal workload, native comparison, default denial,
  artifact/policy identities and normal preservation/cleanup remain mandatory.

Neither policy enables software sampling, QEMU profiling, new Guest rights or
privilege setup. Unsupported hardware collection still fails honestly. To
profile all three languages regardless of speed, Root selects the explicit
policy separately for `c`, `cpp` and `javascript`, with fresh outputs for every
replay. The same two hardware collectors run in each selected case.

## Root metadata staging

Use `acceptance/cp310_physics_current/stage.py --phase profile` with its existing
SHA-bound closed request and reviewed three-role payload. Root supplies current
physical observations; no example pin or namespace identifier is a forecast.
`profile-run-inputs.template.json` describes the six `run_inputs` fields:

* `view_receipt`: actual full-seven successful independent CP310 content-only
  preparation report already used by this baseline. Profiling does not copy a
  second 1.26-GB view and does not claim that an old namespace remains alive.
* `baseline_report`: actual full-seven terminal Direct3 report.
* `baseline_config`: the exact actual full-seven configuration selected in that
  report, including its original `profiler: null`.
* `physics_template`: the exact actual full-seven original physics selection in
  the baseline input ledger. Its original network namespace is historical data.
* `readonly_bind_roots`: the same explicit CP310 provider bind roots. VTune runs
  outside chroot and does not require widening CP310 provider authority.
* `profiler`: `language`, `vtune`, `roots`, `seconds`, with only optional
  `collection_policy` (`measured-slowdown` or `required-hardware`). `vtune` is an actual
  current full-seven executable pin. `roots` contains one to eight canonical
  complete immutable VTune/provider directory roots, including that executable.
  `seconds` is an integer from 1 through 120 and must cover the complete replay.

Choose **fresh** `code`, staging output, `run_owner_out`, `namespace_out`,
`run_parent`, `run_out` and `runtime_controls` paths. Retain the already prepared
`view_root`. All existing path separation, actual six host-tool pins, current
hard 64-GiB/zero-swap task context, source manifest, fixed reviewed restart helper
and content-cache provenance checks still apply. The source payload's owner
bytes must match the new complete source manifest. Do not substitute the older
sealed owner payload. The copy helper and restart helper remain their exact
reviewed bytes.

The profile stager starts no actor. It writes a separate profile configuration
with exactly four added inputs: the original report, configuration, physics
selection and current VTune executable. It retains all original input roots and
non-selection inputs. New complete roots are derived exactly from the selected
VTune/provider roots, the three metadata-file parents, and the one fresh runtime
control directory. Unselected extra roots or replaced workload inputs fail.
The immutable `simulation` field remains byte-equivalent as parsed data.

The original `profile-run-inputs.template.json` retains its omitted-policy
slowdown-only form. The separate
`profile-run-inputs.required-hardware.template.json` shows explicit opt-in; all
null pins still require actual observations. The selected policy is copied
unchanged into `profiler.direct3`, original profile evidence and the owning
report, then checked again by the real collector and terminal owner. Unknown,
null, nonstring or altered policies are refused.

The emitted owner argv uses `--phase profile`. This explicit phase must match the
source-bound profile metadata kind. Normal `--phase run` continues to require an
unprofiled baseline. An ordinary baseline cannot silently opt into PMU access.

## Fresh namespace, actual attachment and replay

The existing Root owner starts an owned same-PID `unshare --mount --net` child,
retains direct-parent birth/pidfd ownership, proves fresh actual mount/network
identities, makes propagation private and brings up only the actual private
loopback interface. It never forecasts a namespace ID, remounts all of `/sys`,
uses physical networking, or leaves the baseline namespace running.

Only the new actual expected-network-namespace field and four owned mutable
CP310 paths (`HOME`, `TMPDIR`, `XDG_CACHE_HOME`, `XDG_CONFIG_HOME`) change in the
physics selection. The owner publishes separate actual config/selection files,
then invokes `profile_vtune.py --action direct3-collect` in that live namespace.
It creates the same read-only view bindings, isolated proc/cgroup observations
and dedicated writable run parent as the baseline owner. Worker, Gateway and
VTune remain host-linked; only the simulator and independent Python witness
enter the selected CP310 root.

For each of two serial collections, `hotspots` and `uarch-exploration`, the entry
starts the real selected Wasm Worker through the existing gate and proves its
actual mapped executable/DSOs, explicit policy and real Gateway subscription.
It checks the same live birth against a retained pidfd, attaches VTune to that
actual Worker PID, and requires collector startup before the simulator bootstrap,
first genuine Guest callback/publication and first physics step. It checks that
the collector remains alive through the last actual physics state. A collection
that finishes early cannot qualify by producing idle counts alone.

The replay uses the unchanged genuine setup bytes, complete canonical state
records, command/state CRCs, endpoint barriers, control kernel and real MuJoCo
step sequence. Each resulting physical state is compared with the original
native baseline using the original state and rendering tolerances. Distinct
collection labels and output paths prevent logs or Gateway configs from being
reused between the two runs. Each collector and all associated actors must
retire normally; pidfd cleanup and reverse unmount remain independent final
checks, including failure below the backing-disk reserve.

## Hardware scope and reports

Both collections use actual hardware sampling. The entry retains installed
VTune version/help, command argv, raw stdout/stderr, result directories, summary,
hotspots CSV, raw hardware-events CSV, positive actual core-event columns, and
microarchitecture top-down labels. The latter requires `Retiring`, `Bad
Speculation` and `Front-End Bound`; absent raw cycles/instructions or unsupported
PMU collection fails closed. A software-only fallback, QEMU host translation or
availability probe cannot produce the hardware-profile success status.

The profile report records `physical_hardware_access: true` and scope
`HOST_PMU_ONLY_NO_ROBOT` as soon as explicit PMU collection is requested, even if
collection later fails. It records `hardware_privilege_setup_performed: false`:
this route changes no sysctl, sampling driver, capabilities, credentials, system
service, cgroup or global VTune installation. Root must supply an already
installed official VTune executable with existing supported Perf/PMU access.
The normal separate install/verify routes remain available for their own actual
receipts. No physical robot endpoint or Guest hardware right is added.

Success is exactly
`PASS_DIRECT3_SELECTED_WASM_ACTUAL_HARDWARE_PROFILES_NOT_SPEED_QUALIFIED`.
It retains `hardware_collection_policy`, its explicit-replay or measured-
slowdown authorization reason, the original baseline profiling flag, the
selected original measured case, both actual attachment proofs,
both real replay controls, original native comparison errors, hardware evidence
and cleanup. `performance_qualified` remains false. Owner success is separately
`COMPLETED_EXPLICIT_DIRECT3_HARDWARE_PROFILE_ONLY`; namespace or metadata success
alone cannot supply this report. On any failure, the raw baseline remains
unchanged and completed/partial collector data stay inspectable under the failed
owned output.

## Source-only policy coverage

`test_direct3_profile_policy.py` exposes `suite(rules)` for a source-pinned guarded
Linux test owner. Its twenty-four DATA-only cases cover measured per-language
selection, original producer cleanup placement, partial/failed/insufficient
reports, genuine raw statistics, quiet default denial, exact immutable workload
and selected root/input adaptation. Additional policies cover a valid fast
baseline under explicit hardware opt-in, unchanged default rejection, the
exact trigger boundary, original timing/flag consistency, baseline failures,
sample/summary requirements and closed policy values. It imports no candidate project module by
itself. These tests have not been executed as part of authoring this patch; even
passing them cannot qualify ROS, MuJoCo, hardware access, performance or any
real controller step.
