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

## Choosing a release or a middleware

The release selector recognizes thirteen ROS 1 distributions, thirteen named
ROS 2 targets including Rolling, and eleven ROS 2 Alpha/Beta releases. Each
running network peer uses its own native ROS libraries. Record the peer's
release, middleware plugin, vendor library, message type, QoS, and security
profile when describing an installation. The following observations are
scoped to the stated operations.

| Peer and transport | Operation exercised |
| --- | --- |
| Alpha 1–8 and Beta 1 native Linux talker and listener | All nine original pairs delivered distinct String samples. Neither direction matched a Jazzy peer for any of the nine. A Jazzy-to-Jazzy control using the same witness and profile delivered all three random values. [Per-release observations and reproduction](../../test/0084.ros-runtime/prerelease_archives/README.md) |
| Beta 2, Beta 3, Ardent, Bouncy and Crystal original Fast RTPS nodes | In a Linux QEMU guest with one isolated Ethernet port, each release’s original publisher and subscriber exchanged ten distinct String values. No direction with Jazzy matched on that network. The two-address loopback fixture records its own observations. [Reproduction](../../test/0084.ros-runtime/legacy_fastrtps/README.md) |
| Jazzy Fast DDS with Dashing, Eloquent, Foxy, Galactic, Humble, Iron, Kilted, Lyrical or Rolling Cyclone DDS | Native rclcpp endpoints exchanged a reliable Int32 request and its incremented response over private loopback. The Rolling run used packages from the signed Resolute testing archive. The Foxy, Galactic and Humble runs retained rcutils diagnostics. |
| Jazzy Fast DDS with Humble int2DDS, dynamic Fast DDS or Fast DDS | Two rclcpp processes exchanged an Int32 challenge. The library-mapping fixture checked the plugin and vendor library in each process. Dynamic Fast DDS emitted a participant deletion diagnostic after delivering the sample. |
| Jazzy Fast DDS with Dashing OpenSplice 6.9 | Two native rclcpp processes exchanged an Int32 challenge and its incremented reply on a private loopback domain. The test checked both processes’ loaded libraries, used the archived OpenSplice configuration schema and retained its startup and shutdown logs. |
| Jazzy on a secured Fast DDS domain | Native SROS2 enclaves exchanged authorized messages. The participant rejected creation of the unauthorized topic. |
| Box Turtle, C Turtle, Diamondback, Electric and Fuerte TCPROS clients with Humble | Each release supplied its archived rospy and generated String. The original clients used a Noetic master and the upstream Noetic–Humble bridge. Each release passed both send and receive in the interpreter and AOT, and both withheld-grant cases produced zero requests at the TLS adapter. |
| Groovy, Hydro, Indigo, Jade, Kinetic, Lunar and Melodic TCPROS clients with Humble | The archived master, rospy and generated String for each release communicated through the Noetic-built upstream bridge. Each release passed four Wasm send/receive cases across the interpreter and AOT, plus two withheld-grant cases with zero requests at the adapter. |
| Noetic TCPROS with Humble Cyclone DDS | The upstream ROS 1 bridge passed a String in both directions. The Wasm fixture exercised a registered TLS adapter and checked the original payload at the other endpoint. With the submission grant withheld, the test observed zero adapter requests. |

A C application compiled with the publicly installed SDK also passed the two
directions with Box Turtle, Melodic and Noetic. Each tested release contributed
four permitted interpreter/AOT runs and two denied runs. For Box Turtle the
bridge used a Noetic master and the archived Lucid client stack.

The ROS 1 fixture grants access to one direction on one topic. A site must
explicitly approve the isolated legacy plaintext link. Its result leaves
direct use of the guest's general bridge operations, custom message pairs,
and authentication on the legacy leg unqualified. Each table row represents
one or more independent runs. A plaintext middleware run provides no DDS
Security result. A successful data sample can coexist with graph diagnostics.

The [native image fixture](../../test/0084.ros-runtime/dds_interop/README.md#bounded-native-image-exchange)
also delivered and byte-verified 5,760- and 921,600-byte RGB8 images in both
Jazzy Fast DDS and Humble Cyclone DDS orderings with its dedicated paced
loopback profiles. It exercised the two same-installation controls and
verified zero replies for reliability incompatibility and an invalid row
stride. The observations apply to the named image endpoints and profiles.

For static coordinate frames, the
[native TF2 fixture](../../test/0084.ros-runtime/dds_interop/README.md#static-tf-with-a-late-joining-rclpy-reader)
uses a Jazzy rclpy listener to compare a same-installation Fast DDS control
and a Humble Cyclone DDS publisher. It distinguishes delivery of a retained
transform from delivery of a live update and verifies the composed frame
tree through the installed TF2 API. A Humble transient-local publisher sent
the pre-join snapshot to a Jazzy volatile reader before sending any live
update. The Jazzy same-installation control with a volatile reader and the
Humble-to-Jazzy control with both ends volatile received zero snapshots.

With Jazzy SROS2 Enforce, an authorized Jazzy Python TF2 listener also
received and verified the pre-join snapshot and 24 live updates. A separate
signed identity was allowed to subscribe to `/tf` while Fast DDS refused
creation of its `/tf_static` reader. It remained online and received zero
samples during the authorized exchange. The
[native Enforce fixture](../../test/0084.ros-runtime/dds_interop/README.md#jazzy-sros2-enforce)
records the three loaded identities and the default-deny grants for this
same-release loopback.

The optional syscall witness matched 58 distinct AES-256-GCM RTPS datagrams
between the writer's completed sends and the authorized listener's completed
receives during the same static TF workload. The count can include middleware
control traffic. The listener's TF2 results independently identify the retained
and live application samples. The denied reader remained online and delivered
zero callbacks throughout.

Run a locally licensed middleware through the
[additional RMW fixture](../../test/0084.ros-runtime/dds_rmw_plugins/README.md).
Run a ROS 1 network peer through the
[Noetic–Humble fixture](../../test/0084.ros-runtime/ros1_native_bridge/README.md).
For deployment acceptance, use original endpoints with the actual message,
service or action and the site's configured QoS and identity. ROS documents
its cross-distribution compatibility policy in the
[upstream release guide](https://github.com/ros2/ros2_documentation/blob/rolling/source/Releases.rst).

### Native parameter services

Jazzy Fast DDS and Humble Cyclone DDS each completed both independent
GetParameters requests when the server and client used the same installation.
A Jazzy Fast DDS client also received both typed replies from a Humble Fast DDS server. With a Jazzy Fast DDS client and Humble Cyclone DDS server on the same
isolated loopback network, neither process completed the service exchange.
The same Jazzy client also timed out before its first typed reply with Dashing, Eloquent, Foxy, Galactic, Iron, Kilted, Lyrical and Rolling Cyclone servers. All nine cross-RMW pairs passed the separate reliable Int32 topic test. The [per-pair service observations](../../test/0084.ros-runtime/dds_interop/service_observations.json) identify the direction, server implementation and receipt for each run. Record the
requested operation, each endpoint's RMW and the request direction when
approving a route. The [service fixture](../../test/0084.ros-runtime/dds_interop/README.md#native-getparameters-service)
accepts a separate server RMW and includes controls for both installations.

A typed relay delivered both named parameter responses between those same
Jazzy Fast DDS and Humble Cyclone DDS applications. Each side used its local
service mapping and exchanged only the two approved reads over reliable ROS
topics on a third isolated domain. Removing the egress process prevented
completion. This result covers the specified private plaintext relay and
GetParameters operation. The same two reads and the missing-egress rejection also passed with Dashing, Eloquent, Foxy, Galactic, Iron, Kilted, Lyrical and Rolling Cyclone servers. Dashing, Eloquent and Foxy shared their server domain with the two explicitly selected transit topics. The other six server installations kept the third transit domain. Other service types, protected relay endpoints and field deployments require separate verification.

### Micro XRCE-DDS

An upstream eProsima Micro XRCE-DDS Client 2.4.3 sent a reliable Int32 topic
sample through an upstream Agent 2.4.3 to a Jazzy Fast DDS 2.14.6 rclcpp node.
The original client received the incremented reply. With the Agent absent, the
XRCE session failed to open. The two wire legs were private plaintext UDPv4.
The test binds the Agent, client and mapped Jazzy middleware libraries to this
observation. [Reproduction and inputs](../../test/0084.ros-runtime/micro_xrce_dds/README.md)

### GurumDDS and RTI Connext

The Humble x86-64 echo process mapped the original GurumDDS 3.0.3 library and
the ROS 3.4.2 GurumDDS RMW plugin. Their executable mappings matched the ELF
files extracted from the recorded Debian packages. During startup the vendor
library reported a missing `gurumdds-x86_64-linux` license feature and exited
with status 255. The Jazzy requester received no reply. Repeating the same
Int32 exchange with Humble Fast DDS returned the expected value. Humble
printed `sequence size exceeds remaining buffer` diagnostics during that run.

For RTI Connext, the Dashing and Eloquent ROS type-support libraries reference
`libnddscore.so`, `libnddsc.so` and `libnddscpp.so`. The test host could
not load those libraries. Native topic exchange and DDS Security remain
unverified for both vendors. To reproduce the topic test, use a locally
enabled vendor runtime and its matching ROS plugin.
[Native RMW fixture](../../test/0084.ros-runtime/dds_rmw_plugins/README.md)

### Dust DDS

In an x86-64 QEMU guest with one isolated Ethernet port, both the original Rust Dust DDS 0.16.0 pair and the Jazzy Fast DDS pair returned the incremented Int32 sample. With one of each vendor, both client directions discovered the other participant but matched zero application endpoints. The upstream Rust crate and Python wheel have independent controls. The [Dust DDS fixture](../../test/0084.ros-runtime/dds_interop/dust/README.md) identifies the build, network and wire type used.

### Zenoh RMW

Two native rclcpp endpoints using Jazzy's archived Zenoh RMW 0.2.13 exchanged
an Int32 challenge through a Jazzy router. A Kilted endpoint using Zenoh RMW
0.6.8 also returned the incremented Jazzy challenge. The fixture matched the
two distinct loopback TCP connections at the router and checked the middleware
libraries in both application processes. With the router absent, both peers
reported connection refusal. These tests cover plaintext Zenoh on an isolated
network. [Reproduction and package identities](../../test/0084.ros-runtime/zenoh_rmw/README.md)

### Site middleware and application security

Record the ROS release, RMW, vendor runtime version and the negotiated
security policy separately. For example, GurumNetworks publishes a
[version pairing table](https://github.com/ros2/rmw_gurumdds#requirements)
and lists DDS Security as incomplete. The
[RTI Connext RMW configuration](https://github.com/ros2/rmw_connextdds#rmw_connext_cyclone_compatibility_mode)
documents a distinct service request mapping for Cyclone DDS. A successful
topic exchange provides no evidence about that service mapping.

With the upstream SROS2 policies enforced, Jazzy Fast DDS 2.14.6 and Lyrical Fast DDS 3.6.2.0 exchanged a fresh String in both directions. Both application callbacks validated the payload. The fixture recorded AES-256-GCM protected user traffic to each receiving process’s UDP port. The unauthorized observer stayed online during the exchange and received zero samples. Both endpoints rejected an unissued identity. For Jazzy and Humble Fast DDS 2.6.12, plaintext worked but the default Enforce exchange failed. Humble rejected the permissions CA algorithm identifier advertised by Jazzy. See the [native strict-security fixture](../../test/0084.ros-runtime/sros2_cross/README.md) for the exact policies, endpoint diagnostics and reproducible controls.

Two Lyrical Cyclone DDS 11.0.1 endpoints loaded the vendor authentication, access-control and cryptography plugins and exchanged the same protected String. The Jazzy Fast DDS to Lyrical Cyclone DDS plaintext control also returned the expected String. Under Enforce on those mixed endpoints, Cyclone reported a handshake key conversion error and neither authorized callback received a value. Site admission must qualify the actual middleware and security policies on both ends. The [cross-vendor fixture](../../test/0084.ros-runtime/sros2_cross/README.md) records the positive controls and the failed authenticated pair.

A fixed String exchange also ran between the two mixed-vendor applications through two independent SROS2 security domains. Each native gateway decrypted its approved topic, authenticated its peer over a TLS 1.3 socket pair and republished into the other trust domain. Both applications checked fresh values. Removing the gateways, substituting an untrusted TLS client certificate, or substituting an untrusted server certificate prevented delivery. The [secure relay fixture](../../test/0084.ros-runtime/sros2_tls_relay/README.md) covers four native processes and this topic only. Each domain must authorize its gateway to read and publish; the gateway can see plaintext.

An upstream SROS2 interoperability run covers the enclaves, policy files
and endpoints used by that run. [ESROS2](https://www.csa.iisc.ac.in/~vg/papers/sigops_atc2026/)
adds native application sandboxing using eBPF and Intel MPK. Qualifying it
requires the original sandboxed endpoint as well as a wire exchange. The
[authors’ artifact](https://github.com/Esros2/ESROS2/tree/3d43c3433e43e7d7d6f7267fb4eaa2f1caf26678)
is an independent upstream source.
