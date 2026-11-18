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
and these two peers. It supplies no DDS Security or physical-device evidence.
