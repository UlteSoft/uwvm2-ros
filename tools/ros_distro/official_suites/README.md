# Owned official upstream native suites

This additional callable runs original upstream source/build/test workflows.
It addresses a concrete limitation of `ros_distro/build.py`: that older lane
builds the Gateway and optionally checks a same-process native String topic;
those observations are not complete official suites. The26 distribution names
in `distributions.json` remain targets, not passed cells.

The initial explicit project scope is the ROS1 core projects `ros_comm`,
`roscpp_core`, `actionlib`, `dynamic_reconfigure`, the ROS2 projects `rcl`,
`rclcpp`, `rclpy`, `system_tests`, and the nine projects selected by the existing
Unitree lock. A target lock explicitly chooses its whole intended project
universe; it must list unavailable, inapplicable, ignored or unresolved projects
as gaps. This is not automatic selection of every third-party repository in a
rosdistro index. Preserve the actual official index/distribution/REP metadata,
original upstream Git references and resolved commit lock separately. A mutable
branch name or a repository name is not a physical source lock or SDK.

## Executable interface and caller ownership

Root first reviews/applies these files, freezes a NEW complete source, supplies
actual tools/SDK/profile/source metadata, and enters an owned private Linux
private mount/network namespaces with only loopback and read-only original input mounts. Root admits the exact current source, manifest,
helper bytes, current original64-GiB/zero-swap cgroup, process birth/pidfd and
native-only provider views before loading this module. It then calls:

```python
report = runner.run_cell(
    admitted_ros_distro_common, admitted_unitree_common,
    source=actual_frozen_source,
    manifest_current=actual_manifest_full7,
    plan_current=actual_plan_full7,
    out=fresh_owned_output,
    guard=current_containment_source_rss_deadline_guard,
)
```

The module imports only Python's standard library and starts nothing on import.
There is no discovery of a compiler, Python interpreter, SDK, RMW or branch from
ambient PATH. It verifies the passed helpers and its own code against the
complete manifest. The plan's actual network namespace must equal the running
owner and differ from its actual direct parent. The source-bound current
loopback check uses kernel interfaces, not inherited sysfs interface names.
The external guard must preserve containment/source/RSS/deadlines during cleanup;
this runner owns the8-GiB ACTIVE backing reserve so its final cleanup still runs
below that floor. Root's outer supervisor remains responsible for the whole
owner lifetime and private view/namespace cleanup.

The selected `python` tool must equal the actual coordinator and profile execution
Python. Native SDK pytest/unittest can use a separate explicitly pinned Python2/3 tool.
A QEMU cell uses its explicit pinned emulator/sysroot prefix for inner gtest and
pytest; CTest cross-launch rules remain the original configured workflow, never
a hidden binfmt assumption. Build-only profiles cannot execute suites.

Every actual Git, build, discovery, test and result-tool command uses the same
bounded `ros_distro.common.SerialRunner` pidfd owner. Native upstream tests may
need concurrent descendants, but workflows/cells execute serially with default
`-j1` build environment and a finite aggregate RSS limit. Current cgroup and
all ancestor OOM facts, active backing reserve, total output, wall time and logs
remain bounded. Only the advisory output walk is cached for at most0.5seconds;
terminal checks force a fresh scan. Tools and SDK/source roots are inventoried
before/after. Root must provision read-only immutable providers; native upstream
code is trusted test software, not a Wasm capability sandbox.

`plan.template.json` names the exact closed plan fields. Nulls are deliberately
NOT valid execution pins. Full7 is exactly path,bytes,sha256,device,inode,mtime_ns,
ctime_ns observed from one actual current ordinary file. Profile cgroup facts
must match the plan. One source lock has kind
`official-upstream-source-lock-v1`, exact backend/distro/upstream_metadata full7,
and the exact selected `snapshots` list. Each snapshot has exactly
`id,repository,commit,root`; repository is its original HTTPS origin, commit is
an actual40-hex Git commit and root is a current existing checkout in a complete
input root. The official distribution metadata resolver/acquisition remains a
separate prerequisite; this runner neither fabricates source pins nor installs
missing historical interpreters, system packages, SDKs or test dependencies.

Git HEAD, origin, clean worktree and every complete `ls-tree -r -z` blob are
checked. Git's optional index writes and ambient global configuration are
disabled. Builds run in a fresh local clone with `--no-hardlinks`; every copied
regular source file must have a distinct inode,nlink1, same content and original
executable bit. Symlink text is preserved. An unresolved Git submodule is an
explicit acquisition failure, never silently dropped or recursively downloaded.
Original source snapshots remain part of exact before/after root preservation.

`literal_provider_reports` is an optional finite list (at most16) of actual
current Noetic native producer report full7s. Its default is empty; old closed
plans remain accepted. This opt-in is needed when an already selected complete
producer root contains a copied Focal SDK with absolute CHROOT aliases. Each
report must be the exact existing root's `report.json`, current source/MF,
preserved terminal Noetic status and exact `sdk-rootfs`/`native-work` children.
Only that already-selected whole root uses the current source-bound
`noetic_native_support.tree` before/after. The shared `common.catalog` function
and root set remain unchanged. No symlink is resolved as a host target or
granted authority by this branch. The physical map catalog takes only actual
regular `row.pin` values and checks their exact root-relative paths; directory
and symlink literals establish no target bytes. Native SDK/MD/ELF replay still
occurs separately in the selected port before any actor starts. Opting into a
literal inventory is not Runtime or official-test qualification.

## Original workflows and separate test universes

Each `suites` row has exactly these fields:

* `id`, `snapshot`, `engine`: one unique suite id, selected snapshot id and one
  of `ctest`, `pytest`, `gtest`, `unittest`, `upstream-junit`.
* `workflow_inputs`: actual full7s of original committed README/build/workflow
  files. Every preparatory command references one of these actual inputs.
* `commands`: ordered objects `tool,arguments,cwd,upstream_reference`. Executable
  is one selected tool key. Cwd and argv placeholders expand only `{workspace}`,
  `{suite}`, `{out}`, `{source}`. Cwd belongs to owned output. The administrator
  records exact applicable upstream build/test/result commands; this program
  does not guess a universal catkin/colcon/rosbuild setup or run dependency
  installers/publishing steps.
* `build`: actual owned configured CTest build path for the CTest engine.
* `pytest_tool`, `pytest_paths`: actual native SDK Python tool key and complete
  original owned-source paths for a pytest universe; null/empty for other engines.
* `junit_roots`: complete separately owned native result directories. They must
  contain no old XML before the workflow, and no external/symlink result can be
  admitted later. Include actual inner gtest/rostest/nose/unittest outputs; the
  CTest wrapper file alone is insufficient.
* `source_test_files`: original committed test-source paths reviewed for this
  universe. Conservative test-source candidates outside that list are retained
  as unregistered gaps, not invented testcase counts or silent exclusions.
* `coverage_gaps`, `ignored_packages`, `disabled_features`,
  `requested_permutations`: original selection decisions and unresolved scope.
  Preserve actual package discovery/ignore markers, feature flags and RMW
  permutations. A passing selected RMW does not imply every RMW passed.

The CTest engine requires an actually selected modern CTest supporting
`--show-only=json-v1` and `--output-junit`; historical SDK CMake versions may not.
It records every registered wrapper/command/property including disabled tests,
runs the entire registered set with no name/label filter, and reconciles actual
JUnit names to that discovery. CTest wrappers are NOT inner gtest cases. Select a separate `gtest` universe for each actual inner binary/permutation.
Its extra `gtest` field has exactly `binary,arguments,include_disabled`. The
actual owned executable is physically pinned and checked against the selected
native target ELF. The runner calls its real `--gtest_list_tests`, retains all
DISABLED_ identities and executes the unfiltered set with a fresh actual XML
output. Disabled cases run only when explicitly selected; otherwise they remain
unexecuted gaps. CTest wrapper success never fills a missing inner universe.
The native or explicit QEMU mode remains separately recorded.

The pytest engine uses a manifest-bound Python2.7-syntax-compatible observation
plugin. It records actual nodeids/locations and deselected nodes during real
collection, then real setup/call/teardown outcomes during execution. Independent
collection and run collection must match. Missing phases become
`UNEXECUTED_OR_INCOMPLETE`; skip, failure and deselection are not passes. Actual
JUnit remains separate native result evidence. An SDK using nose/unittest or
rostest is not automatically switched to pytest.

The `unittest` engine adds the closed `unittest` field with exactly
`python_tool,top,modules`; see `unittest-suite.template.json`. The selected
actual SDK interpreter runs the complete-MF-bound
`uwvm_official_unittest.py` observer as a separate bounded owned process.
`top` expands to a canonical directory in the independent original Git clone.
Each requested name is one whole original committed module or package in
`source_test_files`; class/method filters and same-named ambient modules are
refused. The observer imports each actual module and uses its real
`TestLoader.loadTestsFromModule`, including an original `load_tests` hook. It
does not construct testcase names from source text. Cases outside that original
source top or duplicate actual IDs fail closed. Explicit selected modules do
not silently imply every upstream test source was registered.

Collection and execution use two fresh processes. Their actual loader
inventories, including source files/lines and errors, must be identical. The
runtime observer retains start/stop, all body/tearDown outcomes, skip, expected
failure, unexpected success, module/class fixture failures, subtests and final
run completion. A truncated process cannot qualify an earlier passing method.
Subtests remain observations of their parent case, never extra discovered
method counts. Module fixture errors stay separate gaps; unstarted methods
stay unexecuted. The observer deliberately uses Python2.6-compatible syntax;
actual selected SDK availability/ABI remains a provider prerequisite. The
operator-only `unittest_fixture.py` needs unittest skip/expectedFailure support
(normally2.7+); it is not part of an official source universe.

Root can call `test_unittest_events.suite(actual_formats)` after full-MF
admission for15 DATA regressions without candidate imports or subprocesses.
Separately, Root can copy the4-method operator fixture to an owned temporary
top and run the real observer through its bounded SDK owner. Its deliberate
skip/expected/unexpected outcomes must remain non-PASS. Neither regression
lane is official ROS execution. `unittest` does not launch a ROS Master,
provide rostest initialization or replace nose, rosbuild, ament or catkin's
original fixture workflow.

The `upstream-junit` engine runs the original workflow and retains all emitted
JUnit cases as a separate reported universe. For catkin this means actual
`catkin_make run_tests` plus `catkin_test_results`, not plain CTest0. Colcon needs
its selected historical command, `test --return-code-on-test-failure` when
supported, and actual `test-result --all --verbose` accounting. Ardent/historical
ament_tools and pre-Groovy rosbuild require their own pinned original workflow.
JUnit-only observations cannot establish complete discovery; they always retain
that gap until the appropriate native discovery adapter is supplied. Failed
workflows retain available partial results; later unstarted suites remain
explicitly `UNEXECUTED` with unknown case count.

Unitree CI build success is not a test suite. ROS2 package BUILD_TESTING lint
registration, SDK examples and simulator programs need their own original
workflow selections. MuJoCo `./test` and SDK2 Python demo scripts are live
simulator/torque examples, not generic pytest/JUnit programs. Physical-robot
workflows, unavailable GPU/Isaac/runtime providers and missing original
architecture-specific prebuilt SDK libraries remain prerequisites/gaps. This
runner grants no physical robot connection and provisions none of those inputs.

The first concrete network API adaptation is documented in
[`ports/README.md`](ports/README.md). It binds one exact original rospy
subscriber-first test/launch/echo implementation and runs real native/Wasm
positive and default-denied observations. Its actual dynamic case matcher is
consumed by the dispatcher; missing original native execution still prevents
qualification. This single port does not turn Unitree demo builds or absent
ROS1/ROS2 API ports into official-suite coverage.

## Reports and acceptance boundary

CTest wrapper, pytest node and each native JUnit result file are disjoint named
universes. Raw classname/name/ordinal, disabled/skipped/failure/error, missing
execution, declared counters and physical report pins remain inspectable.
Outer and inner totals are never added to inflate coverage. The top status is
`COMPLETED_NATIVE_UPSTREAM_WORKFLOWS_WITH_EXPLICIT_COVERAGE_GAPS_NOT_ALL_OFFICIAL_PASS`
or FAIL; it never equates observed native tests with all official coverage.
`all_official_tests_qualified` always remains false. The separate
`Wasm_official_suite_executed` flag becomes true only after the current owner
actually releases at least one admitted per-case Wasm execution; authoring,
compilation or an old custom report cannot set it.

`differential_reports` retains separately selected actual custom Wasm/Gateway
receipts with their ORIGINAL kind/status. A failed receipt stays failed. They
are not assigned arbitrary official testcase IDs. Official per-test native/Wasm
input equivalence, complete registered inner-test discovery and all requested
project/feature/RMW permutations are material remaining acceptance work; this
runner reports those boundaries instead of pretending a few custom transports
executed the official suites. No original report/source is relabelled.

This implementation was authored and AST-parsed only. No candidate module,
test, compiler, ROS process, QEMU or SSH operation was executed by its author.
Primary upstream source bytes were retrieved as data for the explicit port
identity record. Root must independently review its exact bytes and run real
per-target acquisition, configuration, original workflows and acceptance.

## Successor r2: physical providers and truthful result accounting

The original six-file r1 candidate and its source-only peer findings are kept
unchanged. This successor invokes the actual complete-manifest-bound
`tools/ros_distro/profile.py::validate` through the already admitted current
`common` module. QEMU is an actual current selected host executable; the target
sysroot is canonical and covered by complete provider roots. The target loader
has an explicit actual full-seven pin, lives inside that sysroot and has the
selected target ELF class, endianness and machine. The real selected pytest
interpreter is checked as a target ELF, rather than inferring its ABI from a
Python minimum version. Native mode also checks the actual kernel host
architecture. Python on the host remains a separate coordinator role.

`mount_namespace` and `network_namespace` are actual identities populated only
inside Root's live owned private view. They must both differ from the direct
parent's current namespace. The runner performs no `unshare`, mount, network
configuration, image acquisition or privileged setup. Root must create that
view with all original source/SDK/tool/snapshot roots bound read-only and the
fresh output writable. The runner re-reads actual bounded `mountinfo` and
requires the effective input mount, and every nested mount under an input, to
be read-only. It does not remount `/sys`; normal cgroup proof stays active. The
LO-only interface proof uses the actual current network namespace. The
current `common.catalog` walks `.git`, `node_modules`, `target` and other
entries normally; its declared `EXCLUDED` constant is unused by `catalog` and
is not treated as a hidden exclusion policy.

Every JUnit suite or aggregate reconciles its declared counters against its
own descendant testcase rows. Nested counters are never added together.
Mismatches are retained as accounting gaps and cannot produce a successful
summary or be lost by CTest/gTest reconciliation. Actual pytest XFAIL and
XPASS remain distinct statuses, with raw `wasxfail` phase records; neither is
ordinary PASS. Suite IDs are unique. Active work uses its own wall budget;
cleanup and terminal rehashing have an independent maximum 600 seconds from
cleanup entry, including on an early failure. An ordinary upstream test
failure does not hide other selected independent suites: those suites continue
only after the live containment guard still passes. Interruption or a failing
containment guard stops the lane and leaves future suites unexecuted.

## Executable per-official-test Wasm/Gateway mapping

The optional actual `official_mapping` full-seven input selects
`mapping.template.json`'s closed schema. A null mapping runs no Wasm actor and
produces an explicit missing-port row for every actually discovered native
case. A previous custom Typed28/Legacy12/physics PASS is retained separately and
cannot satisfy this mapping. Counts of CTest wrappers, pytest nodes, gTest
children and JUnit-file testcase rows remain distinct universes.

A mapping has `adapters`, `cases` and `coverage_gaps`. An adapter is exactly
`{"id": "reviewed-port-name", "entry": ACTUAL_CURRENT_FULL7}`. Its entry must
be a Python source file in the same complete Runtime freeze. It must implement:

```python
ADAPTER_ABI = 'official-api-port-v1'
SUPPORTED_OFFICIAL_CASES = [
    {'suite': 'selected-suite-id', 'universe': 'gtest-listed-inner-case',
     'id': 'ActualDiscoveredSuite.ActualDiscoveredCase'}
]

def verify_prerequisites(scope, documents):
    # Replay the actual SDK/Worker producer schemas, complete source/MD/link
    # proofs, message identity/codec provenance and original native test oracle.
    # Use scope.pin/document; do not convert a status-string into authenticity.
    # Return None after those checks or raise. Actual ports must implement this.
    ...

def execute(scope):
    # Start only selected roles with scope.start(role). Use setup_write/read
    # for this API's reviewed cooperative READY bootstrap; call release() only
    # at the real held boundary before the first ROS effect. Then use write/read,
    # effect/quiet with the actual role-bound observation tokens. Return None.
    ...

def normalize(role, raw_bytes, configuration):
    # Parse the actual original API-specific peer/Guest protocol. For positive
    # effects return the complete canonical value, including every relevant
    # payload/callback/order/error field. For a denied native quiet record return
    # {'effects': 0, 'samples': REAL_COUNT, 'elapsed_ns': REAL_DURATION}.
    ...
```

The ellipses above are an ABI explanation, not supplied executable ports.
No generic `PASS` adapter or stand-in native test has been implemented. The
source reviewer must qualify each port's original test correspondence,
fixture/oracle, bootstrap-before-effects boundary, producer admission and
normalization. Wrong or absent ports remain implementation gaps. Arbitrary
pytest/gTest/rostest functions cannot automatically execute inside Wasm merely
because the owner can discover their native names.

Each selected mapping row is exactly:

```json
{
  "native_case": {"suite": "actual-suite", "universe": "actual-universe", "id": "actual-case-id"},
  "adapter": "reviewed-port-name",
  "positive": "ACTUAL_FULL7_CONFIGURATION_OBJECT_REQUIRED",
  "default_denied": "ACTUAL_FULL7_CONFIGURATION_OBJECT_REQUIRED"
}
```

The two configuration values are actual full-seven **objects**, not the
illustrative strings. They must name actual newly published immutable JSON
controls. Each control has exactly these fields:

- `format_version: 1`, `kind: "actual-official-api-port-execution-v1"`,
  `official_case` equal to the exact discovered key, and `mode` equal to
  `positive` or `default-denied`.
- `semantics` has exactly `backend`, `distro`, `architecture`, `rmw`, `features`
  equal to the selected validated profile, and `permutation` belonging to the
  exact suite's explicit `requested_permutations` list. Selecting one RMW or
  architecture establishes nothing about another permutation.
- `wasm` is the actual module full-seven pin. `sdk_evidence`, `worker_evidence`,
  `source_inputs` and `fixture_inputs` are nonempty bounded full-seven arrays.
  Fixture inputs must belong to the original verified selected Git snapshot.
  The positive and denied controls must select identical values for these
  fields, the official key and semantic descriptor.
- `roles` selects real `worker` and independent `native_peer`; positive also
  requires `gateway`, while a disabled-target denial does not start a Gateway.
  `master` is the only optional additional role. Each role has exactly `host`,
  `image`, `loader`, `mapped`, `launcher`, `arguments`, `protocol`, `environment`. Host/image/
  launcher and every `mapped` value are current full-seven pins; loader is a
  pin or null. Protocol is `line` or `binary`. Arguments are an explicit argv
  array. Each role has its own closed explicit SDK/Worker environment; the
  native SDK LD_LIBRARY_PATH/PYTHONPATH is never inherited by the host Worker
  by accident. Native ROS2 roles must select the actual profile RMW. Only `{out}` and `{source}` placeholders are expanded. A reviewed
  same-PID cooperative exec gate may be the launcher; proof must later open
  the actual final host image. There is no shell or daemon launcher. Foreign
  execution distinguishes the selected real QEMU host from the actual mapped
  target image/loader/SDK files.
- `rights` has exactly `default_authority: []`, `main_module: "main"`, `backend`,
  `grants` and `worker_arguments`. The latter must exactly match the selected
  Worker's argv arguments and contain the actual Wasm path. Positive cases
  keep the global target disabled and enable only the exact selected main, with explicit five-token
  grants matching the actual CLI; no wildcard is admitted. Global/group grant forms, duplicate or contradictory toggles, and enabled
  preload principals are rejected by a complete closed ROS CLI parser. Negative cases
  leave the global backend and main disabled and grant nothing. This metadata
  creates no new Runtime capability and cannot enable the other ROS backend.

`scope.release()` independently opens the actual direct owned births, their
live pidfds/current cgroup/isolated namespace, actual `/proc/PID/exe` bytes and
selected target/SDK `map_files` descriptors before effect writes are allowed.
Every other inode-backed mapped file must also match the complete selected
physical provider catalogs, including its full streamed SHA. Display-only
`maps` device numbers and argv names do not select trust. Source adapters must
use `scope.read` observation tokens, not construct synthetic payload records.
`scope.effect` compares an actual independent native-peer record and an actual
Worker/callback record against the source-reviewed original fixture oracle.
`scope.quiet` requires a real zero-effect sample/duration witness from the
independent native peer. An API-specific port must enforce the original test's
actual required interval, not merely the generic owner's minimum bound.

The v1 static support list remains supported. The concrete rospy port uses
`ADAPTER_ABI='official-api-port-v2'` and
`supports_discovered_case(actual_row, original_suite_spec, source_lock)`.
The dispatcher actually calls this source-bound matcher for each selected
discovered key before execution. It permits the exact source-locked method
inside an actual JUnit-file universe, whose file SHA necessarily varies between
runs. A dynamic match cannot add a case absent from discovery, change its raw
identity/status, bypass the original fixture pins or replace actual execution.
`scope.load_source` binds every port helper through the same complete MF.
For native explicit-loader launches, host may equal the actual selected loader
while image is its mapped target executable; both are opened and proven.
`scope.interrupt_master()` accepts only the live original direct owned Master's
held same-birth pidfd, sends SIGINT and still requires zero normal exit.

For positive completion, the actual Gateway stderr must contain the immutable
production `ros_audit` successful `session.bind` for the exact owned Worker's
PID/current UID. This follows the real production SO_PEERCRED check and is
accompanied by that Gateway's physical mapping proof. For default-denied
completion, the real Worker stderr must contain main-principal
`unresolved-denial` error13 records for a single current Runtime lifetime,
with zero grants and no Gateway, plus the independent native zero-effect
witness. All real actors must exit zero and owned pidfd cleanup must pass.
Raw case controls, actual argv/logs, effect observations, mapping and audit
records and final case receipts are retained. The adapter returns None;
a custom status dictionary cannot qualify anything.

This closes the executable **hook and accounting** gap. The required
additional API-specific official-case ports, source/SDK locks, features, historical build
engines, disabled tests, unavailable hardware and unexecuted cohorts remain
explicit prerequisites. `all_official_tests_qualified` stays false; an actual
complete mapping of a selected discovered universe is a narrower separate
fact. Hardware demonstrations run only through separately implemented simulator
ports; physical robots, production buses and model downloads are not enabled.

The source-only `test_formats.py::suite(formats, differential)` factory contains
27 DATA-accounting/authority/cleanup regression cases. Root must bind all three
files to the actual complete freeze before calling it under the current64
policy-test owner. The tests use no ROS actor, fake ROS transport or asserted
official producer. They cover nested aggregate count mismatches, XFAIL/XPASS,
separate universes, the real global-disable/main-enable CLI grammar, extra
global/preload rights, and a deterministic close-exception barrier. These
27 definitions have not been imported or executed by the source author.
