# Independent native Unitree preflights

These entrypoints make native observations before any DDS or ROS integration
run. Their source files, comments and mapping candidates are implementations,
not executed evidence. Run each actor only through the Root-owned serial Linux
view, after its complete source/input catalogs and exact current 64 GiB cgroup
have been admitted. Do not run them in a host Python environment or use a
package installer to make an import succeed.

`model_preflight.py` consumes real MuJoCo, NumPy and the complete selected
official model tree. It does not import SDK2, CycloneDDS or ROS. It opens no
viewer, starts no service and accepts no network address. The separate
`native_crc_preflight.py` imports official SDK2 IDL/default objects and its
original CRC implementation/native DSO. Importing those types needs the exact
SDK2 CycloneDDS dependency; the actor does not initialize its channel factory
or create a DDS participant. CRC agreement is not authentication.

## Inputs and the read-only view

The coordinator supplies a pinned actor configuration and the manifest of the
actual invoked complete frozen source. A full `config.schema.json` integration
configuration is accepted, but independent preflights only need the consumed
top-level `cgroup`, `input_roots`, `inputs`, `upstreams` and `simulation` members.
Model jobs additionally require `native_mappings`, an explicit list of one to
sixteen distinct selected `inputs` labels. Root selects actual wheel libmujoco
and, for rendering, the actual admitted OSMesa DSO. No provider is inferred
from a package or robot label. Every selected DSO must actually appear through
opened kernel mapping FDs after the native operations have run.
No unused DDS/controller selections are required. The CRC actor permits
`simulation=null`; the model actor consumes the finite fields listed below.
The Root-owned view must validate its minimal control document and all actual
physical package/model selections before launching the actor. In particular:

- `input_roots` inventories the complete selected native Python/wheel package
  trees, original official upstream trees and control files. Inside a chroot,
  the selected logical paths must be explicit read-only mounts of those exact
  physical inputs. A label or a report copied from another mount namespace is
  insufficient to prove the view.
- `inputs` contains actual path/byte-count/SHA256 selections, including the
  model scene and actual upstream acquisition proof. `upstreams` binds the
  original repository, exact Git commit, complete root and `lock_input`.
- The model actor consumes `simulation.robot`, `family`, `model_input`,
  `model_upstream`, `motor_sensors`, `imu`, `keyframe`, `dt_ns`,
  `state_abs_limit`, and optionally `render`. Additional unused integration
  fields confer no authority and do not cause native communication.
- `PYTHONPATH` must select the actual installed wheel prefix. The noDDS model
  job does not need the SDK2 tree. The separate CRC job adds the explicitly
  locked official SDK2 Python root. Preserve actual Python/native ABI and
  library lookup paths; do not substitute host Python or a newer CycloneDDS.
- Optional rendering additionally requires the actual admitted CPU OSMesa
  package closure and `MUJOCO_GL=osmesa` before importing MuJoCo/OpenGL. A model
  load without `--render` is not an offscreen-rendering result.

The coordinator must remain in the selected current cgroup before launching
anything. It owns finite RSS/deadline/output limits, one job at a time, four
GiB memory and eight GiB backing-storage reserves, all-ancestor zero OOM
counters, cancellation, complete before/after catalogs and pidfd cleanup. The
actors neither move processes nor alter a cgroup. Source and selected input
files are reopened before native imports and again before terminal success.
Their module-file lists record actual imported package bytes. Each model actor
also proves its explicit `native_mappings`; the CRC actor proves its separately
selected native DSO through opened kernel mapping FDs.
Python module paths alone do not establish an actual DSO mapping. No complete
mapping collector is claimed for the outer read-only simulation view.

## Closed actor interface

Both actors accept the same four existing internal control arguments:

```text
--control-config ABSOLUTE_CONFIGURATION
--control-config-sha256 ACTUAL_CONFIGURATION_SHA256
--control-manifest ABSOLUTE_FROZEN_SOURCE_MANIFEST
--control-manifest-sha256 ACTUAL_MANIFEST_SHA256
```

The trusted native view selects its exact admitted CPython executable and runs
its pinned bootstrap with `-I -S -B`; that bootstrap supplies only the explicit
native standard-library, selected wheel and frozen-script search paths before
running the actor. No host site processing is used. These are argv elements,
not shell fragments.
No stdin message is required. The owner retains stdout/stderr and the actual
return code and requires the exact terminal JSON record. Earlier JSON lines
never qualify a failed, interrupted or incompletely cleaned-up process.

For `model_preflight.py`, add `--steps N`, where N is explicitly 1 through 32.
Add `--render` only for a separate rendering cell with an explicit bounded
camera/size selection. Output order is:

1. `kind=model-preflight-loaded`: actual MuJoCo package/native version,
   original selected model blob, complete actual joint/sensor tables, ordered
   actuator transmission/joint/carrier map, exact sensor kinds/object IDs and
   addresses/dimensions, actual keyframe records and initial full snapshot.
2. N `kind=model-preflight-step` records. Each calls actual `mj_step` once with
   zero control, followed by `mj_forward` to refresh derived sensors. Snapshot
   fields include time, qpos, qvel, act, ctrl, sensordata and the actual
   `mjSTATE_FULLPHYSICS` vector/specification. The JSON digest covers the
   numeric record before the digest member is added.
3. Optional `kind=model-preflight-render` with actual bounded RGB bytes and
   digest from the selected CPU offscreen renderer.
4. Exactly one terminal `kind=model-preflight-complete`,
   `status=PASS_MODEL_PHYSICS_PREFLIGHT_ONLY`, actual step count and preserved
   selections. Native process return code must also be zero.

The actor rejects missing names, reordered/duplicated actuators or slots,
non-joint transmissions, a sensor on a different physical joint, non-scalar
joint mappings, wrong sensor kind/dimension/address, an IMU on another site,
missing selected keyframes, nonfinite/bounded-state violations and incorrect
step time. An unnamed unrelated free joint is recorded by ID; it is never
misidentified as a controlled motor. Extra sensors remain visible in the
complete table and do not replace selected sensors.

For `native_crc_preflight.py`, add `--sdk-upstream LABEL` and
`--native-crc-input LABEL`. The labels select the actual original SDK2 Python
root and its architecture-specific `crc_amd64.so` or `crc_aarch64.so` input.
The official upstream only supplies these native architectures; QEMU or a
model family label does not create another native-library build.

The CRC actor proves its actual loaded DSO through the frozen shared opened
`map_files` helper. It emits five finite generic word-vector comparisons plus
eight comparisons of the official Go/HG LowCmd/LowState default and finite
active-field objects. Original official private packers supply native padded
CRC input; the maintained SDK does not implement another packer or checksum
here. Message records retain the exact CRC-input bytes, their digest and the
native/Python/public-dispatch agreement. Changing the final CRC member must
not change that input. Terminal success is exactly
`kind=native-crc-preflight-complete`,
`status=PASS_NATIVE_CRC_PREFLIGHT_ONLY`, `actual_cases=13`, with process status
zero and the parent's complete provenance/cleanup checks also passing.

## Nine model candidates and qualification scope

`preflight-model-candidates.json` contains original integration mapping facts
for the pinned official Go2, G1, H1, H1-2, B2, B2W, Go2W, H2 and R1 scenes. It
does not contain model assets or execute a parser/simulator. Each row is
`[carrier_slot, actuator, q_sensor, dq_sensor, tau_sensor]`; preserve row order
when populating `simulation.motor_sensors`. Record the selected file's actual
hash and the current full upstream proof in each native configuration.

The official C++ simulator bridge maps actuator index i to DDS motor slot i;
this is distinct from body-tree joint order and does not generalize to a
different firmware or scene. The preflight independently verifies the actual
loaded actuator ID and physical joint/sensor relationship. The G1 candidate
selects the original 29-DOF scene and is not a 23-DOF hardware result. Family
and mode-machine candidates describe the selected original simulator route.

Run all nine admitted configurations serially and retain nine distinct actual
model/physics results. A candidate, source model, successful XML load, native
CRC comparison or zero-control step is not DDS delivery, typed Guest control,
balance, locomotion, perception, rendering, performance or hardware
qualification. Those later cells use their own existing real peer/controller
and Native/Wasm comparison lanes.

Primary source inputs are the exact official [Unitree MuJoCo simulator](https://github.com/unitreerobotics/unitree_mujoco) and [SDK2 Python](https://github.com/unitreerobotics/unitree_sdk2_python) selections, together
with the original [MuJoCo API documentation](https://mujoco.readthedocs.io/en/stable/APIreference/APIfunctions.html).
The candidate JSON records original source-blob hashes and exact commit
links; a moving website version never overrides the admitted byte selection.
