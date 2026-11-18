# ROS distribution and middleware evidence

`tools/ros_scene/rmw_inventory.py` combines the VM compatibility catalog, a
local ROS package index and operator-selected execution receipts. Each output row
identifies a ROS generation and a release. ROS 2 Alpha 1 through Alpha 8 and
Beta 1 through Beta 3 each have their own row even with an older compatibility
catalog that begins at Ardent. ROS 1 releases use their own transport stack.

Run the inventory from a checkout with an exported rosindex directory. The
historical SDK cache stores these exports under `.tmp/ros-ecosystem/metadata`.

```sh
python3 tools/ros_scene/rmw_inventory.py \
  --package-index-dir "$INDEX_DIR" \
  --sdk-native "$SDK_NATIVE_RECEIPT" \
  --scene-dds "$SCENE_RECEIPT" "$PACK_0/manifest.json" "$PACK_1/manifest.json" \
  > rmw-inventory.json
```

Both receipt options are optional and repeatable. The command reads local files
and writes deterministic JSON to standard output. The catalog, indexes,
execution receipts and worker manifests have SHA-256 identities in the output.

| Field | Interpretation |
| --- | --- |
| `index.status` | `indexed` means the reader examined the release-specific export. `not-supplied` means that file was absent. ROS 1 uses `not-applicable`. |
| `index.packages` | Every package with an `rmw_` prefix in the supplied index, including support packages, with its recorded version and release marker. Recognized middleware implementations also have a display label. |
| `executions` | Positive guest observations from the supplied successful receipts. An empty list means no qualifying receipt was supplied for that row. |
| `interface` | `sdk-v1` identifies the historical native SDK fixture. `scene-v2` identifies an actual Wasm scene String/SetBool loopback through two external workers. |
| `providerProfile` | The scene harness uses the sealed Fast DDS configuration and the two pinned worker manifests identify the ROS prefix and plugin. `rmwReportedAtRuntime` remains null when the receipt lacks an independent runtime identifier. |

The historical SDK native suite emits `uwvm-actual-native-ros-v1`. The reader
counts granted, successful guest cases with observed native events. For scene
runs, the reader accepts the external-Wasm String/SetBool fixture and
`uwvm-scene-dds-roundtrip-v1` receipts. A scene row requires two matching
worker-manifest digests, positive guest cases and the VM digest. Receipts are
operator-supplied evidence. File digests identify their contents. Middleware
installation, cross-vendor interoperability, DDS Security, licensing, hard
real-time behavior and device operation each require their own evidence.

## Re-run the scene guest with a new VM

The existing `run_scene_communication.py` suite produces known-good Wasm
String/SetBool guests and identifies the provisioner's gateway, peer
definitions and packaged images. Use its completed external-gateway receipt
with a completed VM build receipt containing `source_commit` and
`vm_sha256`. The focused runner verifies the pins and exercises the original
wasm32 interpreter and wasm64 AOT cases against the selected VM. The fixture
supplies the authenticated local communication channels through the public VM
entry point and checks that both gateways survive VM exit. Invoke this in the
existing qualification cgroup, CPU set and shared qualification lock:

```sh
python3 -B test/0084.ros-runtime/run_scene_pinned_dds.py \
  --runtime "$VM" --build-receipt "$BUILD_RECEIPT" \
  --baseline-run "$PREVIOUS_GUEST_RUN/results.json" \
  --pair-receipt "$PAIR/results.json" --gateway "$GATEWAY" --output "$NEW_DIR"
```

Use `$NEW_DIR/results.json` with the same two manifests for the inventory.
The native run covers the specific worker profiles, operations, carrier
layouts and execution engines recorded in the receipt.

## Independent native DDS exchange

Pass `--native-wire "$NATIVE_RUN/results.json"` for each attempt from
`test/0084.ros-runtime/dds_interop/run.py`. The `nativeWireRuns` array records
the discovery/transport profile, actual client and peer APIs, middleware
identities, and the checked Int32 challenge and reply. A raw Cyclone DDS C
participant has a null ROS RMW identity. Its distribution describes the origin
of the DDS installation. A peer using Humble rclcpp instead checks
`rmw_cyclonedds_cpp` inside the running process. Neither native variant drives
a Wasm guest.

A completed run requires the original pinned IDL and source, the vendor
libraries, the built peers, the loopback isolation step, the two peer completion
logs, and a verified response of challenge plus one. Keep the output files next
to the receipt. When inputs have moved, use `--wire-remap OLD_ROOT NEW_ROOT` to
recheck their original byte identities in the new directory. An incomplete
build or failed exchange retains its steps and exit statuses as a separate
attempt. The isolated plaintext UDPv4 experiment covers this message type
and these two peers. The `diagnostics` entry records each peer's log digest,
tagged ROS warning, error and fatal lines, rcutils error-state reports, and literal message counts for invalid data
size and unterminated string data. A single rcutils report can mention both
the prior and the new error. Counts measure log occurrences. Each completed row
has the scope `reliable-Int32-topic-roundtrip`. ROS graph interoperability
remains unverified, including when an echo succeeds with diagnostics. The
fixture does not identify their cause. DDS Security and physical-device
operation require separate receipts.

## Native Jazzy SROS2 Enforce

The Jazzy test uses the existing `native_secure_ros2.cc` witness and its signed
resource policy. Build `uwvm-ros2-gateway` and `uwvm-ros2-secure-probe` from
matching native SDK and VM protocol sources with
`-DUWVM_ROS2_BUILD_PROBES=ON` and native compiler flags `-O3 -g`. From the
qualification cgroup, CPU set and shared lock, run the two executables against
a Jazzy root filesystem with the ROS security CLI installed:

```sh
python3 -B test/0084.ros-runtime/run_native_sros2.py \
  --rootfs "$JAZZY_ROOTFS" --gateway "$NATIVE_GATEWAY" --probe "$SECURE_PROBE" \
  --output "$NEW_DIR"
```

Each run uses an isolated loopback network and generates a fresh test keystore
in temporary memory. The two enclaves exchange the fixture's exact String
payload on their authorized topics. The witness also checks that its attempt to
publish to `/uwvm_test/not_permitted` raises an error. The receipt records the
two programs, middleware libraries, ROS package versions, public certificate
and signed policy digests, and the status of issuance and enforcement.
This native Fast DDS test covers the named Jazzy enclaves and topic operations.
Scene guest authorization and cross-vendor DDS Security have separate
qualification requirements.

## Identify a native peer from an archived ROS release

`test/0084.ros-runtime/run_archived_dds_peer.py` runs the original Int32
fixture and samples executable library mappings in each actual endpoint. It
checks the selected release's Cyclone, rclcpp and RMW binaries against the
members of their Debian packages. The package versions and digests are
verified against the signed amd64 ROS archive index. Keep the acquisition
plan, InRelease, Packages.gz and downloaded Debian files together. Their
expected layout is `ARCHIVE/RELEASE-plan.json` and
`ARCHIVE/RELEASE/PACKAGE.deb`.

```sh
python3 -B test/0084.ros-runtime/run_archived_dds_peer.py \
  --runner "$FIXTURE/dds_interop/run.py" --plan "$ARCHIVE/$RELEASE-plan.json" \
  --keyring "$ROS_KEYRING" --ros-prefix "$JAZZY_PREFIX" \
  --ros-system-libs "$JAZZY_SYSTEM" --peer-prefix "$PEER_PREFIX" \
  --peer-system-libs "$PEER_SYSTEM" --output "$NEW_DIR"
```

Run this inside the existing qualification cgroup, CPU set and lock. For an
rclcpp peer whose installation omits idlc, `--auxiliary-cyclone` selects an
installed generator for the fixture's auxiliary C build. The echo process
must still load the selected peer release's Cyclone runtime and RMW. Use
`--raw-cyclone` for a direct C participant. The receipt identifies the API,
both loaded library sets, actual challenge/response and endpoint diagnostics.
A failed attempt retains its stage and raw fixture log.
