# Independent SDK native delivery peers

Both peers are standalone processes and exchange the same exact canonical
String records with every language. Compile `native_ros1_peer.cc` as Noetic
C++17 and `native_ros2_peer.cc` as Jazzy C++20 using the frozen full SDK. Retain
`-MD`, the final target-owned `-MF`, compile database, original verbose compile
and link commands, objects, linked library pins and actual ARM compiler/CMake
mapping evidence. Preparation is not delivery. Neither new peer has an
accepted execution receipt merely because its source is present.

The controller reserves Master `http://127.0.0.1:11384` for ROS1 and domain205
for ROS2. Exact nodes are `/uwvm_sdk_relay/sdk_peer` and
`/uwvm_sdk_relay/sdk_gateway`; topics are `/uwvm_sdk_relay/pub` (Guest to
native) and `/uwvm_sdk_relay/sub` (native to Guest). ROS1 is TCPROS, queue4,
unlatched, with exact String MD5. ROS2 is `rmw_fastrtps_cpp`, reliable,
volatile, keep-last4 with infinite timers and exact official String type hash.
ROS2 GIDs are exactly16 bytes/32 lowercase hexadecimal digits. Native security
is explicitly disabled for this delivery fixture; signed Enforce is a
separate acceptance case. After sourcing Jazzy setup the peer-only wrapper
must checked-unset `ROS_AUTOMATIC_DISCOVERY_RANGE`, then verify absence before
exec. The C++ strict environment checks remain in force.

CLI is exactly `positive` or `absent`. A constructor SIGSTOP precedes main.
After the live ARM/QEMU/birth/leaf proof, the controller resumes and sends I.
The positive protocol emits READY_SDK_PEER, MATCHED_SDK_PEER,
DELIVERED_SDK_PEER and PASS_SDK_PEER. The absent protocol emits READY_SDK_PEER
then PASS_SDK_PEER. Every line includes backend, mode, PID, node, gateway_node,
type, out_topic and in_topic; ROS1 adds master/transport/md5, ROS2 adds
domain/rmw/hash/own_pub_gid/own_sub_gid. ROS2 matched/final markers additionally
bind the Gateway publisher/subscriber GIDs. Absent means no Gateway topic
endpoints and zero native messages, not merely no successful import.

Guest publishes `warmup-js-relay` once (15 UTF8 bytes/19 canonical bytes) to
create the lazy publisher. After actual MATCHED the controller sends P to the
SDK application. It publishes `roswasm-js-relay-out` once (20/24 bytes). The
peer validates its source identity and replies exactly once with
`roswasm-js-relay-in` (19/23 bytes). Up to200 R gates perform one bounded poll
each. Exact full reply bytes, denial13 output sentinels, explicit close and
stale9 are required. Q goes to native only after SDK PASS and actual Worker
exit0. The final quiet window is at least2 seconds/30 samples. Prematch
warmup is bounded0..1 and reconciled with the qualified origin. All other
payloads, duplicate deliveries and foreign named endpoints refuse the case.

Native completion markers retain the established matched endpoint snapshot;
they do not assert that Guest endpoints remain alive after explicit close.
The external receipt binds source, SDK, products, stopped actor mappings,
markers, actual command exits, current cgroup/OOM and owned cleanup. JS runs
outside the Wasm engine through the real C relay in the retained Instance; it
never opens Gateway IPC or invents a Wasm caller identity.
