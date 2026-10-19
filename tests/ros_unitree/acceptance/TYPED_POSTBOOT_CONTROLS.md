# Typed40 controls after the second Linux boot

These source files restore the current control plane around the original
Source40 compiler-only continuation. They retain the original failed batch's
exact bytes and original nine completed product receipts. The new builder
independently requalifies those persistent inputs, compiles the three TinyGo
commands plus the C and C++ API commands, and validates their Wasm binaries.
Neither this stage nor the two owners execute a Wasm body, ROS transport,
native Unitree actor, simulator, physical robot command or performance probe.

`typed_command_root_control.template.json` is a **non-runnable review
template**. Its null values are deliberate: no future manifest, interpreter,
helper, metadata or build observation has been invented. The actual stager
fills current physical observations only after opening the real files inside
the selected boot/cgroup incarnation. A subsequent reboot requires a fresh
reviewed incarnation selection; matching `memory.max` alone is insufficient.

## Review and stage the exact source package

The Root coordinator prepares a SHA256-bound JSON payload with this closed
shape. Each helper record has its exact destination filename, actual source
byte count, complete SHA256 and canonical Base64 encoding of those bytes:

```json
{
  "format_version": 1,
  "kind": "typed-command-postboot-reviewed-source-package",
  "files": {
    "maker": {"name": "command14-input-maker.py", "bytes": 1, "sha256": "ACTUAL_SHA256", "base64": "ACTUAL_BASE64"},
    "builder": {"name": "command14-builder.py", "bytes": 1, "sha256": "ACTUAL_SHA256", "base64": "ACTUAL_BASE64"},
    "adapter": {"name": "typed-command-postboot.py", "bytes": 1, "sha256": "ACTUAL_SHA256", "base64": "ACTUAL_BASE64"},
    "restart": {"name": "requalify_after_restart.py", "bytes": 1, "sha256": "ACTUAL_SHA256", "base64": "ACTUAL_BASE64"},
    "cpp_qualifier": {"name": "official-cpp-qualifier.py", "bytes": 1, "sha256": "ACTUAL_SHA256", "base64": "ACTUAL_BASE64"},
    "utilities": {"name": "command14-utilities.py", "bytes": 1, "sha256": "ACTUAL_SHA256", "base64": "ACTUAL_BASE64"},
    "outer": {"name": "root-launch.py", "bytes": 1, "sha256": "ACTUAL_SHA256", "base64": "ACTUAL_BASE64"}
  }
}
```

The displayed values are placeholders and fail admission. Use the current
reviewed maker, derived builder, postboot adapter, restart helper, derived C++
qualifier, original unchanged utilities, and `typed_command_root_owner.py` as
the outer role. The derived builder embeds its exact adapter/qualifier hashes;
the adapter embeds the exact restart-helper hash. Review the complete final
chain together. The stager does not derive, compile, syntax-check or execute
any payload body.

Root invokes the pinned physical `typed_command_stage.py` with
`--payload /absolute/reviewed-package.json --payload-sha256 ACTUAL_SHA256`
inside the selected **64 GiB / zero-swap** cgroup. The stager verifies the full
17,928-file Source40 freeze, fresh zero-OOM/current cgroup admission, the
current selected Python bytes, the exact retained r2 seed configuration and
the unchanged failed r1 batch. It then writes only the fresh control directory:

`/var/tmp/uwvm-ros-unitree-20261016-r1/source40-unitree-command14-boot2-controls-r1`

The sealed directory contains the seven selected source files,
`helper-selection.json` and actual `root-control.json`. It has no mutable report
or cache. The separate stage receipt is:

`/var/tmp/uwvm-ros-unitree-20261016-r1/source40-unitree-command14-boot2-stage-r1.json`

Existing control/phase outputs are rejected instead of reset or overwritten.
The stager retains the exact current full-seven input/staged-file observations
and reports `PASS_SOURCE_STAGE_ONLY_NO_PHASE_EXECUTED` only after before/after
source and control checks. Its reported prepare/build commands are metadata;
neither command is executed by the stager.

## Execute preparation and build as separate owned phases

The actual stage receipt records two literal commands using the admitted
Python and `root-launch.py`. Each command contains the exact control SHA256:

```sh
/usr/bin/python3.14 -B /absolute/code/root-launch.py \
  --phase prepare --control /absolute/code/root-control.json \
  --control-sha256 ACTUAL_CURRENT_CONTROL_SHA256

/usr/bin/python3.14 -B /absolute/code/root-launch.py \
  --phase build --control /absolute/code/root-control.json \
  --control-sha256 ACTUAL_CURRENT_CONTROL_SHA256
```

Use the actual paths in the receipt. Preparation launches only the reviewed
metadata maker. The maker preserves the original configuration and derives a
new configuration with the actual current cgroup, selected new C++ qualifier,
complete original input roots and exact new code/metadata control roots. It
emits an unexecuted build invocation and starts no compiler.

Build requires the successful, fully preserved preparation report and the
exact published configuration/invocation. The owner verifies all current
helper/input pins and complete input catalogs, then starts only the published
builder command. The metadata controls contain the new configuration and
invocation; helpers execute directly from the sealed code directory, which is
a complete read-only input root. Preparation and build never automatically
chain together.

The outer owner keeps the original **512 MiB metadata / 1,536 MiB aggregate
owned-process-tree** ceilings, eight-GiB backing-filesystem reserve and finite
command/phase deadlines. Ordinary compiler jobs retain their 512 MiB/600-second
guards; only the three previously bounded TinyGo jobs use the explicitly
reviewed 768 MiB reservation and `GOMEMLIMIT=128MiB`. The historical nine
completed commands are not counted as newly executed compiler jobs. The
expected fourteen new process jobs are the five fresh builds plus their
required compiler/linker/validator work, as prescribed by the reviewed builder.

Cleanup first offers the actual retained direct coordinator a 600-second
cooperative SIGTERM grace through its owned pidfd. It verifies the original
process birth and direct parent before signalling. It then always performs the
existing child-first owned-pidfd TERM/KILL fallback and records any unresolved
ownership or remaining process as failure. No numeric PID/process-group kill,
daemon cleanup or resource-limit change is used.

## Keep current observations separate from original evidence

Every current owner input ledger uses newly opened full-seven pins. Retained
product rows keep their original source/artifact/receipt fields and attach a
separate `current_file_qualification` sidecar. The owner requires these current
sidecars to have the exact preserved path, inode, timestamps and complete bytes,
with only the explicit filesystem device renumbering allowed. Original raw
reports, receipts and historical cgroup facts remain unchanged. The old failed
batch remains `FAIL`; its incomplete TinyGo row remains ineligible.

Complete Source40 content, current cgroup incarnation, exact source payloads
and all consumed current input roots are checked before and after each phase.
The successful owner status remains explicitly compilation-only. Actual ROS,
default-denial, simulator, performance and broad architecture/distro acceptance
must be established by their own later owned execution lanes.
