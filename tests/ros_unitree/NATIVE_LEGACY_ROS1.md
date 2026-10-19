# Native ROS1 legacy Unitree transport witness

This fixture uses real `roscpp` and the complete, freshly built official
`unitree_legged_msgs` catkin package. It publishes one `LowState`, receives one
`LowCmd`, and compares every field through the actual ROS1 serializers. It is a
native transport witness for a future Guest/Gateway acceptance run. Writing or
compiling these sources alone proves no native delivery, Guest permission,
robot control, timing performance, or hardware CRC.

The selected upstream is **unitree_ros_to_real** commit
`b989870124913091fbe75e0dcfb047eb4ca00a09`. Its eleven `.msg` definitions are
`BmsCmd`, `BmsState`, `Cartesian`, `HighCmd`, `HighState`, `IMU`, `LED`, `LowCmd`,
`LowState`, `MotorCmd`, and `MotorState`. The retained actual passive packet
has SHA256
`7af2979ea61d25e377a2c542ecb7d71ad646a2696cff04ad978376437cb512bf`.
The complete message contract header records each original definition hash.
Do not substitute ROS2 `unitree_go`, ROS2 `unitree_hg`, a differently named
upstream checkout, a projected schema, committed stale generated headers, or
unavailable native identities from an earlier Guest-only preparation.

## Sources and exact coverage

`native_legacy_ros1_values.h` constructs independent native expected values using
actual catkin-generated C++ types. Every LowCmd/LowState field and every fixed
array element is assigned, including both twenty-motor arrays, all IMU/BMS
fields, remote bytes, every Cartesian coordinate, reserved values, and both
`crc` members. Values use finite, exactly representable binary32 fractions.
`native_legacy_ros1_contract.h` compiles all eleven real headers and checks their
ROS1 traits and exact field names, scalar types, and array element counts. It is
included in the same executable TU; it is not a source-string or arithmetic
mirror test and adds no fictional second compilation unit.

The peer uses `ros::serialization::serializationLength`, `OStream` and
`serialize` on the actual generated type. It compares complete serialized bytes
and lengths, including nested fields, rather than native C++ object memory or
padding. The selected family has no strings or variable-length fields; the
fixture nevertheless bounds the serialization output to 1536 bytes. Every line
is bounded to 4096 bytes. Each received command additionally binds the actual
TCPROS connection header's caller ID, type and MD5 to the selected Gateway and
compiled generated traits. A native self-subscription checks that the state
publisher really delivered its value locally; the parent must separately
observe the Guest's complete received state. Self-delivery is not TCPROS delivery
to the Guest.

`state_crc_field=0x13572468` and `command_crc_field=0x89abcdef` are explicit test
payload values. Their comparison proves their transport alongside all other
fields. **No firmware CRC algorithm or hardware authentication is claimed.**
The emitted FNV1a64 is only a compact diagnostic alongside the full bytes. No
GO/HG ROS2 command layout, `memcpy` packet layout, or CRC packer is used here.

## Build gate

A parent producer must first finish the real Noetic SDK acquisition and its
independent native package build. Copy the exact official catkin package into
fresh owned work, generate all eleven headers, record the fresh devel headers,
and compare every installed header to its fresh devel counterpart after the
unmodified official install. A stale source `include/` overwriting a generated
header is a failure. Retain original source/license/provenance, actual SDK and
tool directory catalogs, source manifest, real command logs, and full file pins.

Stage `native_legacy_ros1_CMakeLists.txt` unchanged as the new project's
`CMakeLists.txt`. Its required explicit cache paths are:

- `UWVM_SOURCE`: the complete immutable source containing these files.
- `UWVM_NOETIC_PREFIX`: the actual qualified SDK's Noetic prefix.
- `UWVM_LEGACY_PREFIX`: the new official package install prefix qualified above.

The selected physical SDK compiler and generator are parent inputs, not a
compiler located by a label or PATH guess. The target is C++17, O3, assertions
enabled with `-UNDEBUG`, `-fno-fast-math`, and `-ffp-contract=off`. It has one
primary TU. The parent must retain actual GNU Make compiler `-MD`, effective
`-MF`, actual `-MT` targets and complete system-header dependencies, reconcile
compileDB and verbose Make working directories, prove a fresh object and direct
link trace, and inspect the actual native ELF. Compiler/configure probe TUs are
separate from the one primary fixture TU and must be reported truthfully.
A configured contract file has status `CONFIGURED_NOT_BUILT_OR_EXECUTED`.

An earlier version of this guide referred to an existing six-TU Noetic package.
No verified report for that package has been selected in the current lane, so
that sentence supplies no build evidence or required count. The maintained
[`prepare_noetic_native.py`](prepare_noetic_native.py) instead selects the real
identity exporter, custom ROS1 Gateway and this independent witness, and reports
the actual primary translation units from their compile databases. No old
report is modified or promoted. The official message package has zero invented
C++ message-definition TUs; its actual genmsg and install outputs are evidence
in their own right. See [the maintained preparation guide](NOETIC_NATIVE.md).

## Native ownership and network gate

The parent owns a fresh loopback-only Linux network namespace, a bounded native
ROS Master, this peer, the real ROS1 Gateway, and a fresh Worker/Guest. It must
prove the network namespace and only-up loopback from actual current kernel
state; environment strings alone are insufficient. Every native role uses only
the selected read-only SDK and fresh writable work/home/tmp. The parent binds
whole input/output/source catalogs, actual complete manifests, tool aliases,
ELF loader/RPATH closure, original process births, pidfds and the exact current
64 GiB cgroup, and performs serial owned cleanup with a finite deadline.

The native peer requires `ROS_IP=127.0.0.1` and a literal
`ROS_MASTER_URI=http://127.0.0.1:PORT` (one trailing slash is allowed), with PORT
1024..65535. `ROS_HOSTNAME`, `ROS_NAMESPACE`, and `LD_PRELOAD` must be absent.
It accepts no arguments or user-supplied remapping. Its ROS node is exactly
`/uwvm_unitree_legacy/native_peer`. The Gateway node must be exactly
`/uwvm_unitree_legacy/legacy_gateway`. Topics are only
`/uwvm_unitree_legacy/lowstate` and `/uwvm_unitree_legacy/lowcmd`; they are test
endpoints, never firmware `/lowstate` or `/lowcmd` endpoints.

The process first emits `LEGACY_NATIVE_GATE PID` and blocks on a finite stdin
setup frame. Before giving the setup, the parent must prove this actual owned
process's executable, unchanged birth, pidfd and current cgroup and qualify its
actual mapped SDK DSOs using opened `/proc/PID/map_files/RANGE` FDs against the
complete physical SDK/catalog. Raw maps device/inode tuples are display data,
not physical-file qualification. The host native executable and mapped target
DSOs remain distinct from a Wasm Guest. A second actual SDK map proof is required
after context creation. This process does not implement or replace that trusted
parent gate. It never receives a fake native identity, loose path fallback, or
permission bit from its own protocol.

The peer has no author-created callback thread or async spinner. It services its
explicit callback queue in finite passes. roscpp's own implementation threads
are still native process descendants/threads under the parent's ownership and
resource limits. Native OS ROS access cannot prove Wasm authorization.

## Binary protocol and positive ordering

All stdin control bytes are binary. The first frame is exactly sixteen bytes:
`ULN1`, then little-endian uint32 positive (0 or1), sequence42, and reserved0.
The sequence is a fixture data selector, not authentication. The setup is
single-use. Read operations have a 30-second/128-attempt bound. Outputs and
polling are finite. Root's outer deadline, cgroup limits, logs and pidfd cleanup
remain necessary for library waits or an unrecoverable failure.

After setup the peer emits eleven `LEGACY_NATIVE_TYPE DataType MD5` lines, then
`READY_NATIVE_LEGACY_V1 POSITIVE`. These values are real compiled traits. The
parent must compare all eleven to the actual Noetic native identity export,
including its complete DataType/MD5/Definition records, selected installation,
source and SDK closure. Printed MD5 syntax alone is not identity qualification.

For the positive case, start a real same-main Guest command with only its exact
ROS1 LowState subscribe and LowCmd publish aliases. Use the actual complete
native-bound generated legacy provider and exact adapter codec identities.
The Guest main must register its LowState subscription and open its LowCmd
publisher alias without publishing a command yet. The actual ROS1 custom-topic
adapter advertises the publisher lazily on its first publish, so opening the
publisher alias does not create a LowCmd publisher. For this isolated test,
the explicit positive LowCmd grant must set `latch=true`, queue-depth1 and
burst1. Its first and only command then remains available while the real TCPROS
handshake completes; a nonlatched first publication can legitimately precede
that handshake and be lost. The native LowState publisher stays nonlatched.
No retry, fake pre-advertising role, implicit capability, or firmware topic is
introduced. This test-only durability is a selected grant, not a default right.

1. Send native `M`. It waits at most five seconds for exactly two LowState
   subscribers (one native self plus one Gateway/Guest) and zero LowCmd publishers,
   with one native self publisher. Read `LEGACY_MATCHED 1 2 0`.
2. Send `P`. The peer publishes exactly one complete LowState, waits at most
   five seconds for exactly one native self callback, and prints
   `LEGACY_STATE LENGTH CRC_FIELD HEX_BYTES FNV1A64`. Independently collect the
   Guest's complete decoded value and canonical bytes. Compare all fields and
   bytes, not just motor q/dq/tau or tick. For this fixed-width finite legacy
   family the generated canonical codec and ROS1 serializer can be compared
   after their actual artifact-qualified outputs are retained; no C++ layout
   comparison or ROS2 CDR encapsulation is valid.
3. Send `A` and read `LEGACY_ARMED_COMMAND`. Only then tell the Guest main to
   construct and publish exactly one complete expected LowCmd. The native
   queue waits at most five seconds, binds the actual Gateway caller ID and
   compiled type/MD5, requires the one real LowCmd publisher, compares all canonical bytes including CRC, and prints
   `LEGACY_COMMAND LENGTH CRC_FIELD HEX_BYTES FNV1A64`. A Guest publish-success
   return is not this native delivery evidence. The parent must send/receive
   both roles concurrently under its single serial process-owner ledger;
   waiting for the final native command line before instructing the Guest would
   deadlock the intended protocol.
4. Retire all ordinary Guest topic/node owners after callbacks unwind; await
   the actual Worker completion and exact Guest close observations. Send native
   `Z`. It waits for exactly the native self LowState connection and zero
   LowCmd publishers, then checks at least two seconds and thirty actual quiet
   samples. Duplicates, reordered payloads, wrong callers or reappearing Guest
   endpoints fail. Read `LEGACY_QUIET 1 SAMPLES ELAPSED_NS 1 1`.
5. Send `F`. Read `PASS_NATIVE_LEGACY_WITNESS 1`, then require return0, ordinary
   ROS shutdown, actual terminal pidfd evidence and complete owned cleanup.

Each letter is single-use and its phase is checked. Repeated initialization,
unknown letters, invalid lengths or values, unexpected callbacks, serialization
errors, duplicates and finite polling exhaustion fail the process. Callback
failures become sticky and are checked by the owning queue pump; no exception
is intentionally thrown through roscpp's callback trampoline.

## Empty-policy case

Use another fresh Guest instance with empty ROS grants. The same native peer is
started with positive0, so its own self subscriber/publisher still exist. The
Guest must observe actual denial13 for its attempted aliases without making a
new native endpoint. The parent independently checks the exact denied SDK
statuses and Gateway audit/principal, not a Guest boolean alone.

Send `M` and require `LEGACY_MATCHED 0 1 0`. After the denied Guest has closed
and actually terminated, send `Q`: at least two seconds/thirty samples must keep
only the native self connection, zero LowCmd publishers and zero callbacks.
Require `LEGACY_QUIET 0 SAMPLES ELAPSED_NS 0 0`, then `F`,
`PASS_NATIVE_LEGACY_WITNESS 0`, return0 and complete cleanup. This demonstrates
no additional selected-topic effects during the observed interval only; it
makes no global claim about unrelated ROS graph activity.

## Six-language same-main follow-up

The native fixture is a prerequisite for six real command applications in C,
C++, Rust, TinyGo, AssemblyScript and JavaScript-in-Wasm. Each must use the
actual Guest SDK with Backend ROS1 and the ENTIRE newly native-bound eleven-type
provider emitted after the genuine Noetic export. Original Guest-only Source33
legacy providers remain guest-only; their unavailable native identities cannot
be promoted by relabeling. No dynamic bridge should supply fake native MD5s.

The command shape can reuse the existing same-main UTM1 entry mechanism and
actual exported function-index selection. Positive code runs as the actual
Guest main, not a WAT root calling a privileged preload. It subscribes to
LowState, explicitly inspects every field, reports its complete canonical
record, waits for the parent's command-arm input, publishes a full LowCmd, then
retires every acquired SDK owner after callback unwind. The negative run uses
another fresh instance and an empty policy. No library inherits the main's
rights. The actual binary must be compiled, fully validated and import/memory/
start inspected for its selected command runtime. A Root fixture may provide
only the already reviewed minimal command stdio imports; it must not broaden
the ROS ABI or permit arbitrary WASI calls to make an example pass.

This source proposal contains the native fixture and compile contracts. It
does not contain or claim those six applications, their compiled artifacts,
twelve native transport cases, an actual Noetic SDK, native MD5 export, or a
successful ROS1 Gateway execution. Those are separate explicit implementation
and actual-execution stages. Preserve all failed and historical reports.

## API references

The source shapes were reviewed against the official Noetic APIs, not executed:
[SubscribeOptions callback/queue contract](https://github.com/ros/ros_comm/blob/noetic-devel/clients/roscpp/include/ros/subscribe_options.h),
[NodeHandle options subscription](https://github.com/ros/ros_comm/blob/noetic-devel/clients/roscpp/src/libros/node_handle.cpp),
[actual intraprocess connection header fields](https://github.com/ros/ros_comm/blob/noetic-devel/clients/roscpp/src/libros/intraprocess_publisher_link.cpp), and
[ROS1 serialization](https://github.com/ros/roscpp_core/blob/noetic-devel/roscpp_serialization/include/ros/serialization.h).
The selected completed SDK's actual full-MD headers and library bytes remain
mandatory build and runtime inputs; these mutable reference URLs are not a
replacement SDK lock or native compatibility qualification.
