# Fixed-layout custom ROS1 topic preparation

This directory prepares one concrete custom **topic message** through official
ROS1 generation and the trusted fixed-topic adapter. It does not implement custom
Service or Action dispatch. Source checks, generation, and a native binding probe
do not complete the original custom-type interoperability requirement: current
Worker/Gateway/Guest execution and permission checks are also required.

## The official message and canonical body

`uwvm_custom_ros1_fixture/msg/Packet.msg` is the authoritative ROS1 source. Build
its catkin package with `message_generation`; do not substitute a handwritten ROS
header or invent an MD5. The message contains an `int32`, a boolean, a `float64`,
four fixed `uint8` elements, and three fixed `int16` elements. Its UWVM canonical
body occupies exactly 23 bytes, in declaration order, with little-endian scalar
bits, boolean values restricted to 0 or 1, and finite floating-point values.
Fixed arrays have no length prefix. Native TCPROS framing is not this IPC body.

The first profile rejects strings, dynamic arrays, unsupported field types,
recursive layouts, custom Service/Action definitions, oversized layouts, and
untrusted definition paths. Official ROS1 `IsFixedSize` traits provide an
additional compile-time check in the typed native binding. Native boolean members
may have `uint8_t` storage; the generated codec rejects values greater than 1.
The binding also compares the complete official native `Definition` with the
generated schema before ROS initialization. It checks all ordered fields, fixed
array lengths, nested record names, and every dependency body; only comments and
whitespace are ignored. Primitive `byte`/`char` aliases and relative nested names
normalize to their ROS1 meanings. A genuine native MD5 paired with an altered
catalog is refused, including a `bool` changed to `uint8` despite equal C++
storage. Missing, extra, and duplicated dependency bodies are refused.

## Prepare a reviewed adapter bundle

The checked-in `catalog.schema.json` intentionally has no native identity. It is
a schema-preparation input and cannot generate a trusted deployment bundle. The
following pure Python command checks that fixed layout only:

```sh
python3 sdk/tools/custom_types.py tests/ros_custom_types/ros1/catalog.schema.json --check
```

After official catkin generation, create a separate deployment catalog in a
reviewed staging directory. Copy the exact message file beneath that directory;
definition paths must remain within the catalog root. Add `native_identity` with
the exact official generated `MD5Sum<uwvm_custom_ros1_fixture::Packet>::value()`.
Obtain it through the installed official generated traits or ROS message tools
inside the required Linux test cgroup. Never use a Guest-supplied identity or a
placeholder identity as qualification evidence.

```json
{
  "format": "uwvm-ros-custom-fixed-v1",
  "types": [
    {
      "ros_version": 1,
      "type": "uwvm_custom_ros1_fixture/Packet",
      "file": "msg/Packet.msg",
      "native_identity": "<replace with the official generated MD5>"
    }
  ]
}
```

Generate into a fresh output directory with
`python3 sdk/tools/custom_types.py <deployment-catalog.json> --out <fresh-output>`.
The bundle includes `manifest.hpp`, `native_ros1.hpp`, `native_ros2.hpp`, six
language codec files, and `generation.json`. Verify and preserve its input/output
hashes with the current source manifest. Generation is not a native type-support
verification or an execution result.

## Select common and native support together

Configure `gateways/ros1` with all three explicit options:

```text
-DUWVM_ROS_CUSTOM_REGISTRY_HEADER=<absolute output>/manifest.hpp
-DUWVM_ROS_CUSTOM_ROS1_HEADER=<absolute output>/native_ros1.hpp
-DUWVM_ROS_CUSTOM_ROS1_PACKAGES=uwvm_custom_ros1_fixture
```

The package must already be available in the selected catkin prefix. For several
packages, supply a quoted CMake semicolon-separated list. Missing paired headers,
missing packages, duplicate package names, and unsafe include paths fail during
configuration. Build the Worker with the same common descriptor header through
its `UWVM_ROS_CUSTOM_TOPIC_MANIFEST` compile definition. The native header's
`UWVM_ROS_CUSTOM_ROS1_TYPES(M)` rows bind official native types to their generated
codecs. Native codecs perform no IO and remain compatible with C++17.

An ordinary build selects no provider and has an empty custom registry. Compiling
a provider does not enable ROS, configure a connection, grant a resource, or
create any ROS entity. A real custom grant still requires an explicit module or
group target, operation, absolute resource, exact type/official MD5, every normal
topic quota/audit/transport/latch field, and `adapter-codec-identity` equal to the
generated codec fingerprint. Worker and Gateway both validate that fingerprint.
Unknown types, cross-version names, mismatched identities, unsupported operations,
and attempts to replace a built-in type are denied.

## Prepared native binding probe

`native_topics.cc` checks official native name/MD5 and codec agreement before
`ros::init`, complete official field/dependency definitions, malformed canonical
rejection before publisher creation, real
TCPROS publication/subscription with exact bytes, native NaN rejection before
canonical output allocation, and removal of resources on close. It uses the
reusable typed binding directly. It does not authenticate an IPC session or
execute a Wasm module, so its success cannot establish those properties.

The fixture forks exactly one native peer **before either process calls
`ros::init`**. Its single nonblocking `SOCK_SEQPACKET` control channel carries
fixed 24-byte records. The peer publishes valid content, NaN, and a native bool
byte of 2 on the inbound topic. The parent holds the custom subscriber and an
independent raw witness on that topic; both require the opposite publisher's
actual `MessageEvent` caller ID. The custom publisher uses a separate outbound
topic, whose raw witness exists only in the peer process and returns all 23
canonical bytes. A local callback cannot satisfy the outbound acknowledgement.

Before closing resources, the parent calls its live XMLRPC `getBusInfo` endpoint
through nonblocking official XMLRPC client APIs. It requires connected inbound
and outbound `TCPROS` rows, bound respectively to the peer XMLRPC URI and peer
node name, and prints one bounded `TRANSPORT ROS1` evidence line. The controller
binds that line's child PID to an observed native ARM/QEMU descendant birth in
the same verified cgroup. The final PASS marker requires all negative cases,
complete grant reuse checks, native close assertions, explicit peer success, and
successful `waitpid` reaping. Exceptions retire that exact fork child; no process
group is signalled. Multiple callbacks in one ROS process are not counted as
multiple TCPROS subscriptions.

Configure this directory with the same two header options after the fixture
catkin package is available. Build target
`uwvm-ros1-custom-topic-native-fixture`. Run it only with a dedicated ROS Master
and controlled `ROS_MASTER_URI`/`ROS_IP`, within the original verified Linux
64 GiB cgroup, with swap disabled. For ARM validation, preserve current ELF,
QEMU, source, generated input, dependency, cgroup, and live process mapping
evidence. Do not compile or execute it locally merely because Linux is offline.

Behavior phases and IO transfers have absolute ten-second deadlines. The peer's
idle control receive allows thirty seconds for the parent's bounded publication
and bus-information phases; child exception cleanup allows two seconds before
SIGKILL. The enclosing controller still enforces its 120-second command limit.
A valid result requires
exit code 0 and the exact marker
`PASS ROS1 fixed custom topic native binding exact_body=23`. Missing master,
missing native provider, invalid metadata, and failed behavior checks yield
failure. No such native result is recorded by the preparation files.

The remaining acceptance work includes the custom-enabled Gateway over its real
authenticated IPC session, a real SDK Guest in each supported language, denial
with no native side effect when no explicit grant exists, wrong native/codec
identity, cross-instance handle attempts, revoke/close races, bounded overflow,
and current ARM execution. Custom `.srv`, `.action`, dynamic ROS1 fields, and
broader IDL support remain outside this increment.
