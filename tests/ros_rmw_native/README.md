# Actual native endpoint and record foundations

This increment implements an eagerly created, owned native ROS2 publisher,
actual native QoS/GID queries, whole String/Bool/Twist/Image codecs, and strict
additive endpoint/receive-metadata IPC encoders. The source has not been
configured, compiled, imported or executed. Its authored tests provide no native
compatibility or performance credit. It does not complete the required original
rcl/rclc/rclcpp/rclpy/rcljava acceptance or the all-distribution/vendor matrix.

## Ownership and actual native evidence

`gateways/ros2/native_rmw_publisher.h` keeps a real generated native publisher
alive from successful activation through all publications and retirement. The
factory checks the real generated TypeSupport/complete schema and selected
codec, source resource, complete configured QoS, actual native QoS and actual GID.
It allocates local owner storage before creating the native entity. Any later
factory failure destroys the native owner and notifies the executor. It sends
zero probe messages. A separate temporary endpoint cannot qualify another
lazy publisher. The legacy Gateway publish path remains a different interface.

Every inspect/publish/retire requires the exact backend2/source handle/Instance/
runtime generation/authenticated native session/native incarnation association.
The transport must independently hold the actual source grant and allocate a
nonreusing native incarnation; numeric associations are not authentication.
Inspection queries the same native object again. Changed identity or QoS
refuses instead of replacing the previously admitted observation. Publication
decodes the whole canonical message and calls that publisher once. A timeout or
ambiguous admission must never replay it. Retirement removes that native owner
once and tombstones the local endpoint. Stale owners receive125; foreign owners
receive13. Resource owners must also preserve cleanup failures separately.

The generic custom binding uses actual generated Native/Codec association and
the complete existing trusted custom manifest. This includes bounded vendor
messages when their real native provider is compiled and qualified. Generic
dispatch/sharding must still be integrated into the new transport; an existing
Unitree source descriptor alone is not a running new endpoint. Builtin String,
Bool, Twist and Image bindings use the actual installed builtin TypeSupport
identity/schema checks. Every original field is retained. String copies preserve
embedded NUL by explicit byte count; Twist checks all six finite components and
the configured bounds. Image reuses one allocation-free view validator with the
unchanged existing canonical restrictions, including supported encodings/frame
syntax. This is not a claim of support for other native Image encoding names.

The native C++17 SDK path deliberately reuses fixed binary wire helpers and
borrowed views. It does not add printf-based serialization, string streams or
an encode/decode validation allocation. Vendored fast_io requires newer C++
features than some original ROS SDKs; no original SDK is silently upgraded.

## Additive wire formats

`ipc/rmw_endpoint.h` defines UMRE version1, exactly256 bytes. It carries a
trusted source association and actual native incarnation. Activation/inspection
responses contain one actual80-byte QoS profile for a publisher/subscription or
two independently queried profiles for a client/service. Publisher responses
also contain actual native GID bytes. Native enum values are explicitly mapped,
not copied as assumed wire enums. SYSTEM_DEFAULT/UNKNOWN and missing native
getter APIs refuse. Every unused byte is zero and independently checked. Failed
decode/encode/association checks leave the caller's output unchanged.

Client replies may additionally contain a real native client GID when the
installed rcl/rmw API can obtain it from the same retained client. Absence is
explicit; a local request-correlation counter is not a globally unique GID.
Preserve the complete native byte extent. Differing native/Guest GID widths
cannot be handled by truncation; a separately defined consistent domain-wide
mapping or explicit unsupported result is required.

The effective-QoS adapter accepts the original raw rmw_qos_profile_t getters
and the newer rclcpp::QoS getter objects. A separate actual compile/link probe
checks the complete native RMW QoS struct/policies independently of whether
rclcpp/qos.hpp exists. This avoids confusing an old getter spelling with a
missing middleware capability. These probes remain unexecuted source here.

`ipc/rmw_metadata.h` defines URMI version1, exactly192 header bytes plus the
complete canonical body. It is independent of the unchanged UWRE-v1 ABI. The
record associates owner/incarnation/queue sequence/opaque Gateway correlation
with native source/receive timestamps, publisher GID, native publication/reception
sequences, or native service writer GUID and signed sequence. A Gateway ticket
does not become a native sequence/GUID. Signed fields retain every bit. Unsupported
publication/reception sequence numbers use the original explicit UINT64_MAX
sentinel and absent presence bits. No unknown publisher GID is fabricated.

Presence bits describe actually obtained native fields. Strict upstream take
adapters must check the complete required mask before consuming a record and
return95 when native fields are unavailable. A native timestamp of zero can be
an actual observed value; an absent timestamp is explicitly marked absent.
Native implementation identifiers are native pointers and never cross the
wire; GID value bytes are scoped by the original authenticated native session.
This IPC path is not Guest rclcpp intra-process delivery, so Guest-facing
from_intra_process is false. A native node's own intra-process state must not be
relabelled as communication inside the Guest context.

Error/closed records have no native presence bits or canonical payload. Their
independent control_flags field preserves the original retired/external-effects-
may-remain facts, full correlation and actual error/close reason, including125.
They are not discarded because ordinary metadata is absent. No queue may pair
a body with a later callback's metadata: retain, peek and commit one owned record
atomically. The helper captures topic metadata from the actual callback's RMW
info; service metadata must come from the SAME real rcl_take_*_with_info call.
The old rclcpp callback's rmw_request_id_t alone has already lost timestamps.
The real witness selects the original raw rmw_message_info_t callback or newer
rclcpp::MessageInfo wrapper by the actual available header. A native SDK without
timestamp fields returns95 before initialization for this strict full-info
witness. It is not converted into a passing reduced-metadata case.

## Authored contracts and actual execution recipe

The standalone wire contract has nine groups: exact known bytes and full-width
association, reserved/unused bytes, concrete QoS, two independent QoS profiles,
activation/inspection/retirement, signed native fields and unsupported sequences,
metadata absence/extent/atomicity, native service identity versus Gateway ticket,
and faithful error/closed records. They have not been run.

The Image regression includes the exact original decoder body, renamed only,
as an independent before/after oracle. It compares admitted and rejected formats
and all decoded fields across encodings, frames, dimensions, stride, backend,
short/trailing records and timestamp boundaries. The reference is test-only;
production has one validator. It has not been run.

After independently admitting the exact coherent sources and compiler, Root's
resource owner can copy this complete source closure into an owned `/tmp` tree,
configure with `-DUWVM_NATIVE_RMW_MODELS=ON -DCMAKE_CXX_STANDARD=17`, and build
serially using `cmake --build <owned-build> --parallel 1`. Run the two executables
separately under the owner's actual memory/deadline guard, preserving source/
tool hashes, commands, dependency files, outputs and actual process retirement.
These local format tests are not native ROS tests.
Both model targets explicitly enable assertions, and their sources refuse a
remaining NDEBUG definition. A Release build cannot erase the actual contract
operations and return a misleading zero status.

For the real witness, admit the complete installed original ROS SDK, real link
libraries and actual configured feature header, then select
`-DUWVM_NATIVE_RMW_REAL_SDK=ON -DUWVM_ROS2_GATEWAY_FEATURE_DIR=<actual-directory>`.
Build serially in the original64GiB/swap0 SSH Linux cgroup. Supply one unused
absolute topic name to `native-rmw-publisher-contract`. The external supervisor
must enforce a finite deadline and preserve every native dependency/product/
teardown identity; a selected getter cannot preempt a stuck native RMW call.
The fixture checks eager native existence, zero publications during creation/
inspection, exact owner/GID/QoS association, one full String publication including
embedded NUL, independent real receive metadata, and rejection after retirement.
It also drives the real executor/discovery until the selected publisher has
actually disappeared, within one finite five-second bound; local tombstoning
alone is insufficient. The external supervisor still verifies whole-process
and original SDK teardown separately.
Any nonzero result, missing marker, unsupported95 or failed teardown stays a
failure/unqualified observation. No Wasm/native interop or performance follows
from this one native witness. It has not been compiled or executed.

## Required remaining integration

The authenticated Host/IPC operation and optional Guest namespace, nonreusing
native owner registry, per-operation quota/retirement handling, generic custom
factory dispatch/shards, atomic metadata queue, real subscription path and direct
native service-info path still need implementation. Existing81 imports and old
backend structs must remain unchanged. Original-RMW provider callbacks must use
these same owned endpoints for subsequent publication/submit/response/take and
native retirement; they must not fall back to legacy lazy endpoints or queues.
Missing providers fail before effects. Default permissions remain empty.

After the complete integration is implemented and independently reviewed, run
local contracts, original upstream libraries/examples/tests, explicit default-
denied and granted Wasm/native routes, all admitted ROS distribution/vendor
combinations, QEMU architectures, simulation and separate release-mode native
performance/VTune hardware Hotspots/microarchitecture comparisons. None of those
acceptance results is asserted by this source increment.


The complete QoS feature additionally requires the selected original
`rmw_time_equal` and `rmw_time_normalize` symbols to compile and link. Header
presence cannot enable it. Effective duration profiles are compared with that
actual RMW utility, and transported duration values use its canonical saturated
representation. This covers equivalent nonnormalized nanoseconds and native
infinite encodings without guessing from raw fields. Missing actual utilities
leave the feature unavailable. The real selected-SDK publisher witness includes
these original-utility contracts before ROS initialization; none has run yet.

The factory reports unavailable95 before local/native entity allocation when
the complete actual-QoS compile/link feature is absent. Other queried QoS fields,
including `avoid_ros_namespace_conventions`, must also match. The real-utility
witness explicitly rejects a changed namespace-convention flag; this additional
source contract remains unexecuted.
