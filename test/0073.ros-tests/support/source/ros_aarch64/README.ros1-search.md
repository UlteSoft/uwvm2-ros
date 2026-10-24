# ROS1 parent-search and Action byte-budget ARM fixtures

`run_ros1_search.py` prepares and executes two current-source C++17 regression
fixtures against the task-owned ROS Noetic SDK. The driver is part of the
mandatory current-feature acceptance path. Preparation proves compilation
only. A `PREPARED` receipt contains zero behavior probes and
`actual_behavior_status: NOT_PERFORMED`; native ROS behavior requires a separate
successful execution receipt. Local Python source/model checks do not establish
either native compilation or behavior.

| Product | Original fixture | Native behavior contract |
| --- | --- | --- |
| `ros1-parent-search` | `test/0073.ros-tests/support/source/ros1_search/native_ros1.cc` | Sixteen actual Gateway/Master assertions covering exact search, explicit fixed-basename parent search, native single-key agreement, namespace isolation, refusal codes, request/response bounds, binding and revocation. The fixture receives the qualified Gateway pathname as its first argument. |
| `ros1-feedback-budget` | `test/0073.ros-tests/support/source/ros_action/feedback_budget_ros1.cc` | Seventeen actual adapter/ROS scenarios covering advertiser feedback payload bytes, responder framing bytes, terminal result bytes, quota refusal without goal mutation, subsequent legal response and stale handles. This fixture creates the real adapter and middleware directly. |

The two middleware-free contracts remain separate unit products:
`test/0073.ros-tests/support/source/ros1_search/contract_test.cc` requires the Worker C++20 baseline, while
`test/0073.ros-tests/support/source/ros1_search/gateway_contract_test.cc` uses the Gateway C++17 baseline.
They are compiled by the current ARM unit stage. This driver does not replace
those contracts or the original eighteen native acceptance products.

## Required immutable inputs

Run only as root on the authorized Linux host inside the verified existing
64 GiB cgroup with `memory.swap.max` equal to `0`. The parent must supply a
current fixed-interpreter QEMU binfmt lease, its exact lease JSON, the QEMU
pathname, and the current Docker `State.StartedAt`. It must export
`UWVM_ROS_ARM_BINFMT_ENTRY` with the leased entry name. Each invocation needs a
private mount namespace created inside that inherited cgroup and lease.

`--source` must be the complete frozen current source tree. Its parent directory
must contain `source-manifest.json`. The exact manifest must include the two
original fixtures, their transitive headers, and the entire controller directory
`test/0073.ros-tests/support/source/ros_aarch64`. Invoke the driver from that frozen directory. Historical
controller copies omitted from an old manifest cannot qualify this run.

`--rootfs` must be the task-owned canonical Noetic SDK with
`.uwvm-task-owned`, ARM GCC9, its ARM `cc1plus` frontend, ARM CMake, ROS1 Python,
and the catkin packages `roscpp`, `actionlib`, `actionlib_msgs` and
`actionlib_tutorials`. The driver binds the frozen source read-only at `/work`
inside a private SDK view, gives native tools a fresh `/tmp` and home directory,
and checks the complete static SDK file/symlink keyset and root aliases before
and after use. Source, SDK and output path spellings must not contain whitespace
or backslashes, and each output directory must be new and outside both inputs.

Execution also requires the original current ROS1 Gateway compiler product and
its strict targeted `rebuild_affected.py` report. Pass the complete absolute
product pathname recorded in that report as `--gateway`; pass the report as
`--gateway-report`. The shared `gateway_qualification.py` validates current
full-source membership, the actual system-inclusive compiler `-MD` output, final
effective `-MF`, compiled object rule, exact project/system dependencies, raw
compiler/link metadata, SDK/tool identities, helper archives, successful
compiler-subtree ARM mappings and same-incarnation zero-OOM evidence. A pool
copy or a historical source report is insufficient. The runner then creates its
own Gateway copy after checking the complete original product identity before
and after copying. This shared qualification proves compilation only.

## Prepare and execute

The following commands are recipes, not evidence of a completed Linux run.
Replace every uppercase placeholder with a verified current absolute pathname
or value. Keep the parent cgroup and QEMU lease active across each command.

```sh
unshare -m python3 -B CURRENT_SOURCE/test/0073.ros-tests/support/source/ros_aarch64/run_ros1_search.py \
  --mode prepare --source CURRENT_SOURCE --rootfs CANONICAL_NOETIC_ROOTFS \
  --out NEW_PREPARATION_DIRECTORY --qemu LEASED_QEMU \
  --binfmt-evidence CURRENT_BINFMT_LEASE_JSON \
  --container-started-at CURRENT_DOCKER_STARTED_AT

unshare -m python3 -B CURRENT_SOURCE/test/0073.ros-tests/support/source/ros_aarch64/run_ros1_search.py \
  --mode execute --source CURRENT_SOURCE --rootfs CANONICAL_NOETIC_ROOTFS \
  --out NEW_EXECUTION_DIRECTORY --qemu LEASED_QEMU \
  --binfmt-evidence CURRENT_BINFMT_LEASE_JSON \
  --container-started-at CURRENT_DOCKER_STARTED_AT \
  --prepared NEW_PREPARATION_DIRECTORY/report.json \
  --gateway CURRENT_ORIGINAL_ROS1_GATEWAY_PRODUCT \
  --gateway-report CURRENT_STRICT_ROS1_GATEWAY_REPORT \
  --master-port 11383
```

`--mode all` performs both stages in one new output directory and requires both
Gateway arguments. The parent acceptance runner uses independently qualified
preparation and execution receipts. The Master port must be `1024..65535` and
unused. The driver acquires an exclusive owned port lock and starts a fresh
isolated ROS1 Master with bounded actual XMLRPC readiness queries. Its `getUri`
reply must match the selected URI, and `getPid` must identify an observed live
owned birth with the current ARM Python/QEMU mapping. An unrelated listener
racing the initial port check cannot supply readiness evidence for this run.

Preparation runs serial CMake/catkin compilation with effective
`-O1 -g0 -UNDEBUG -std=gnu++17 -MD` and an explicit per-target `-MF` beneath
`/tmp/build/CMakeFiles/TARGET.dir/`. The generated CMake sets
`CATKIN_ENABLE_TESTING` to `OFF` before finding catkin, so the compile database
contains exactly the two owned original fixture sources. Extra framework or
fixture rows remain admission failures. The explicit depfile path supports
Noetic's CMake 3.16, which supplies no automatic `-MF`; admission still uses the
actual last effective output when a compiler recipe supplies another one.
A forced header inserts a constructor which
raises `SIGSTOP`; the original fixture sources are compiled unchanged. Raw
depfiles, compile database, effective verbose compiler argv, flags, link recipe
and generated CMake source are retained. Admission checks the actual last `-MF`
output, its object rule, original fixture, stop header, `/usr` headers and Noetic
headers. Compiler commands have explicit deadlines and a 2 GiB observed process
subtree RSS cap. The driver checks the 60 GiB (61,440 MiB) cgroup current-memory admission
ceiling throughout commands.

Before each fixture resumes from its constructor stop, evidence must identify
its actual owned process, birth, live cgroup membership, mapped ELF64
little-endian AArch64 product and current expected QEMU host image. The search
fixture also requires a stopped live mapping of its current qualified Gateway
child before that child resumes. This Gateway stop occurs after its original
image is detected; it does not attest a Gateway pre-main barrier. The trace
reports the host kernel executable separately from the mapped ARM image and
does not claim physical ARM hardware or performance results.

Successful execution requires exactly these unique semantic completion lines:

```text
PASS actual ROS1 exact/explicit-parent search and namespace isolation checks=16
PASS actual ROS1 feedback/result framing byte budgets scenarios=17 checks=POSITIVE_DECIMAL
```

`POSITIVE_DECIMAL` is the positive check count printed by the fixture. A nonzero
exit, any `FAIL ` line, missing/duplicate marker, missing stopped mapping,
changed input identity, changed cgroup incarnation, nonzero OOM delta, remaining
owned process or mount prevents `PASS`. Preparation returns `PREPARED` with
`compilation_status: PASS`; successful execution returns `PASS` with
`compilation_status: REUSED_QUALIFIED_PREPARATION` and exactly two native
behavior probes. Neither receipt is a Wasm E2E or full-project verdict.

## Ownership, cleanup and local model checks

Signals use pidfds acquired for observed Popen roots or proven descendant
births. Descendant discovery reads `/proc/PID/task/*/children` for every extant
thread and continues during bounded termination phases. Cleanup orders observed
children before parents and never signals a broad process group. The receipt
states this observed-birth scope; it makes no claim about an unobserved daemon's
ancestry. Owned recursive mounts and the exact owned port-lock inode are cleaned
up. All retained source, SDK, driver/archive, prepared-report, generated-input,
tool and Gateway control bindings are checked again before the final verdict.

The following command is permitted locally because it uses Python-only
synthetic argv/procfs/ELF/cgroup receipts and small temporary text files:

```sh
python3 -B test/0073.ros-tests/support/source/ros_aarch64/ros1_search_driver_models.py \
  --report NEW_SOURCE_MODEL_REPORT.json
```

The model report records current input hashes and explicitly labels native
compilation and ROS execution `NOT_PERFORMED`. See
`test/0073.ros-tests/support/source/ros1_search/README.md` for the bounded fixed-basename search semantics,
default-deny permission configuration and native relative multisegment
limitation. See the current parent acceptance documentation for the complete
thirty-product inventory and full regression sequence.
