# Actual native ROS witnesses for embedded JavaScript

This adapter executes guest JavaScript inside the real Wasm interpreter. The
Gateway and typed native witness are separate real native processes. The test
has no Node engine, host JavaScript callback, fabricated Wasm memory, substitute
ROS transport, direct Gateway RPC client or Runtime caller-identity override.
Every file here is source preparation until an owner retains actual results.

## Prepare native products and additional event artifacts

An owner first builds the real Gateway, `native_peer.cpp` and `exec_gate.cpp`
against an explicitly selected complete native SDK. The peer and gate require
C++20 for the selected fast_io headers; Gateway language selection remains its
separate actual feature probe. The producer retains complete compiler MD files,
objects, final linker inputs, original sources, executable bytes, capability
feature JSON and the actual native String interface identity. Compilation alone
establishes no native messaging or Guest execution.

The closed producer report has schema `uwvm-embedded-js-native-products-v1`,
status `PREPARED_NATIVE_PRODUCTS_NOT_EXECUTED`, the exact current `source` and
`source_manifest_sha256`, original `input_pins`/`product_pins`, and true
`source_preserved`, `inputs_preserved`, `products_preserved`. Its cleanup must
have `status:PASS`, no remaining owned birth and no finalization errors. Each
`products` row contains:

* `backend`1 or2 and `profile` `builtin`, `go-api` or `hg`;
* full seven-field `gateway`, `peer`, `features`, `interface_identity` pins;
* `peer_source:test/0073.ros-tests/support/source/ros_javascript_guest/native_peer.cpp`,
  `definitions:{UWVM_JS_NATIVE_BACKEND:<backend>}`;
* `full_dependencies_verified:true` and original `compile_commands` with their
  real dependency/link associations.

The report also retains a distinct full seven-field `exec_gate` pin,
`exec_gate_source:test/0073.ros-tests/support/source/ros_javascript_guest/exec_gate.cpp`,
`exec_gate_full_dependencies_verified:true`, and actual
`exec_gate_compile_commands`. A pin comprises `path`, `bytes`, `sha256`,
`device`, `inode`, `mtime_ns`, `ctime_ns`; a status literal cannot replace it.

`execution_environment_base` has exactly `LD_LIBRARY_PATH` and
`AMENT_PREFIX_PATH`, each a deduplicated list of actual physical SDK/install
directories. They override the selected host environment **only for native
peer/Gateway actors**. Native actors use LANG=C and LC_ALL=C so their locale
selection does not load unadmitted host locale mmap data. The independently linked gate must load under the host
Worker environment. An explicit SDK loader PT_INTERP and physical SDK DT_RPATH
can support this private, deliberately nonrelocatable test build; retain the
actual ELF fields and link argv. Never launch a target through a loader argv
and then identify that loader as the target executable. The SDK or original
ELF is never patched by this adapter.

`execution_view_after` is a list of complete `common.catalog` summaries for the
actual SDK COPY and complete native package prefixes. Their root/file/byte/link
counts and content hashes must match the executing Frame's original complete
inventories. `execution_file_catalog` pins a JSONL document of all ordinary
physical seven-field file pins in that closure, including mapped locale/data
files. `execution_elf_catalog` contains the actual ELF subset and native
products, at most8192 entries. Reports are stable-read bounded to64MiB; the
file catalog is streamed with16KiB rows,512MiB total and at most one million
rows. All original file pins are reopened. This is a finite admission bound,
not permission to allocate an unbounded catalog in one read.

The feature document supplies actual `backend`, `topic_string:true`,
`unified_events_v1:true` and a boolean `runtime_type_hash`. The identity document
supplies actual `backend`, String `type` and native `identity`: ROS1's MD5 or
ROS2's `RIHS01_` identity from the independently retained native producer.
The Humble peer does not call unavailable type-description APIs or infer a
runtime type hash from its own expectations. Real endpoint GIDs and MessageInfo
identify its observed writer.

The original lesson08 checks source/observer ownership and resource closure;
it does not subscribe to a native topic. `native_event.mjs` is an additional
positive fixture. Prepare its two expanded manifests as separate guarded jobs:

```sh
"$PINNED_PYTHON" -B "$FROZEN_SOURCE/test/0073.ros-tests/support/source/ros_javascript_guest/event_manifest.py" \
  --recipes "$FROZEN_SOURCE/sdk/origin/javascript/guest/recipes.json" \
  --backend 2 --out "$FRESH_OWNED_OUTPUT/native-event-ros2-manifest.json"
```

Repeat with backend1 and a fresh output. Then invoke the ordinary
`sdk/origin/javascript/guest/prepare.py` with that expanded `--manifest`, the exact
current `--source-root` and explicit original tool/archive inputs. Run every
declared preparation/compile/link/validation job separately and serially. Retain
the standard `uwvm-embedded-js-actual-build-receipt-v1` receipt with variant
`native-event-ros1` or `native-event-ros2`, current source manifest, real
`preparation_job`, `manifest_generation_job`, `expanded_manifest`, full-MD/link
records, actual artifact and independently inspected binary boundaries. The
additional command retains exactly38/43 ROS functions plus basic fd_read,
fd_write and proc_exit. Adding an event example never adds a filesystem right.

These two artifacts stay separate from the original exact30 recipe inventory.
A Root-owned private derivative of the already peered all30 producer may carry
out the two builds with these exact bounds. Its input driver/configuration must
be freshly staged and pinned, and its result is additional-event build/binary
preparation only. It cannot promote the original30 or assert native delivery.

## Select an actual native execution

Invoke `native.py` with the same explicit source/manifest/config/out CLI as
`run.py`. The same full Frame envelope and exact `benchmark` keys apply. Its
`native_cases` selects one or both distinct backends, with each row having
exactly these fields:

```json
{
  "backend": 2,
  "report": "native-product-report-input-label",
  "native_profile": "builtin",
  "exec_gate": "actual-compiled-gate-input-label",
  "local_report": "actual-same-freeze-local26-report-input-label",
  "peer": {"executable": "peer-input-label", "arguments": ["{mode}"], "alias": null},
  "peer_binary": "peer-input-label",
  "peer_mappings": ["sdk-loader", "sdk-libc", "sdk-rclcpp", "string-typesupport", "sdk-cyclonedds"],
  "gateway": {"executable": "gateway-input-label", "arguments": ["{config}"], "alias": null},
  "gateway_binary": "gateway-input-label",
  "gateway_mappings": ["sdk-loader", "sdk-libc", "sdk-rclcpp", "string-typesupport"],
  "gateway_context_mappings": ["sdk-loader", "sdk-libc", "sdk-rclcpp", "string-typesupport", "sdk-cyclonedds"],
  "master": null,
  "connection": {
    "node-name": "js_gateway", "namespace": "/uwvm_js_guest",
    "domain-id": "1", "rmw-implementation": "rmw_cyclonedds_cpp",
    "security-mode": "disabled-test", "keystore": "not-applicable", "enclave": "not-applicable"
  },
  "event_artifact": "actual-additional-event-wasm-input-label",
  "event_receipt": "actual-additional-event-build-receipt-input-label"
}
```

The strings above illustrate labels/schema; they are not acquired paths,
resolved SDK mappings, hashes, features or granted authority. Choose actual
required mapping roles from the selected SDK. The ROS1 equivalent selects real
roscpp/typesupport libraries and uses explicit localhost `master-uri`,
`advertise-host:127.0.0.1`, `transport:tcpros`. It also selects `master` with
exact `job`, `binary`, `script_input`, `uri` fields. The actual Master is a new
owned process, with its XML-RPC PID bound to the already owned executable birth.
An occupied endpoint or existing daemon is refused. There is no ROS1 Master
field for ROS2 other than null. A missing backend remains unqualified.

Native execution requires the original current64GiB/swap0 leaf and a new
owner-created network namespace whose only interface is an enabled lo. The
adapter validates actual namespace identities of its actors. It never creates
or joins a daemon, changes global settings or migrates cgroups. Native actor
RSS is bounded to1GiB and each deadline to600s; shared reserve/whole-input/
pidfd/cancellation limits remain mandatory. Select a fresh owned output and
short UNIX socket path. Default Guest permissions remain empty except basic
stdin/stdout/stderr. Each native positive case adds only its literal, bounded
main-principal topic/event grants. The source alias and event observer have
independent rights and queues.

## Verify real phase ordering and effects

1. The peer emits BOOT before creating a ROS context and waits for I. The owner
   proves its actual executable, owned pidfd/birth/cgroup/network, then sends I.
   Native readiness and selected real SDK mappings follow actual context init.
2. The separate test-only gate opens the exact Worker descriptor and emits
   `JS_EXEC_GATE <pid> <fd>`. The owner proves the gate executable and complete
   kernel-held FD bytes/metadata/CLOEXEC flag. Before G, Gateway configuration
   names that same owned PID and literal manifest; the owner proves the actual
   Gateway executable and startup SDK mappings. G atomically execs the held
   Worker. No WAT trampoline or preloaded application receives ROS authority.
3. Original02 creates actual endpoints, emits its existing marker and blocks
   on P. At this real Guest barrier the owner additionally hashes the live
   Worker `/proc/exe` and requires actual post-context Gateway SDK mappings.
   Independent native graph match permits P. The peer accepts one exact final
   message from the actual writer, replies once, and the guest's real JS
   callback must consume the exact reply within200 owner-paced polls.
4. The additional event fixture activates a native subscription and a separate
   observer. Native discovery permits one T publication. The guest must decode
   the exact topic event and then the correctly bound resource_closed record,
   one actual poll per R. No publish acknowledgment substitutes for reception.
5. Original08 remains ownership/resource_closed only. Its fast lifetime need
   not allow a post-exec live Worker or post-context Gateway mapping sample.
   That case instead retains the actual full-FD exec chain, immutable Gateway
   startup mappings, exact successful session.bind audit, guest bytes, both
   normal exits and the independent native quiet witness. Production Gateway
   checks SO_PEERCRED before this bind; no fabricated credential is supplied.
   The report does not pretend that an unavailable live sample was observed.
6. Every Gateway bind audit must name the actual Worker UID/PID and one nonzero
   session; unexpected identity, denied bind or error fails. Actual opened
   map_files descriptors for every inode-backed mapping must match the complete
   admitted SDK/product file closure; ELF mappings must also match its ELF
   subset. Unknown host fallback fails. Raw maps devices/pathnames remain
   display-only. This inventories the explicitly observed stages, not all
   unobserved instants of a fast process's lifetime.
7. Default-denied companions and armed engine limits also execute the actual
   original artifacts as main, with no ROS grants or Gateway. Independent
   native counts remain zero. Every case checks exact Guest output/exit and
   native phase inventory, then at least two seconds/30 quiet observations with
   no extra effect. All owned processes must terminate before the next case.

The subset status is `PASS_EMBEDDED_JS_NATIVE_ROS_SUBSET_ONLY`. A full result
requires an independently reopened, preserved **same-freeze actual local26**
report with exact25 cases/all four Worker profiles and the same build/Worker
provenance, plus actual original02/08 variants for both ROS1 and ROS2. Only then
is status `PASS_EMBEDDED_JS_ALL30_RUNTIME_AND_NATIVE_ROS` with
`all30_Runtime_qualified:true` possible. `local_report:null`, a single backend,
or preparation-only evidence cannot satisfy that gate. This denotes the exact
30 example variants' Runtime coverage; it does not assert native delivery for
their intentionally empty service/Action/custom configurations, every ROS API,
physical robot control, simulator control or native-performance parity.


## Native compiler language compatibility

The native String peer and same-process execution gate retain their existing
`fast_io` implementation when compiled in C++20 or later. For an explicitly
selected ROS1 C++17 SDK, their compatibility path uses the maintained
`src/ros/gateway/common/native_io.h` fixed-capacity numeric/text formatter and checked
POSIX pipe I/O. It does not change the real ROS1 publisher/subscriber, exact
TCPROS caller/MD5 checks, BOOT/I handshake, G gate or descriptor-based
`execveat`. The C++17 peer path selects ROS1 explicitly; it does not claim a
ROS2 SDK mode or silently substitute another middleware.

An extracted Noetic/Focal compiler is a separate physical SDK input. Select
its actual invocation alias and compiler/loader/stdlib bytes, retain real
full-MD and link closure, and qualify the mapped native products under the
owned current cgroup before execution. Choosing C++17, or successfully
compiling either I/O path, is not native ROS delivery or SDK compatibility
qualification. Historical C++20 Humble evidence is not a Noetic result.
