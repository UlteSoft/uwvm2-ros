# Owned ROS2 Subscription Host transport

These sources are implementation and future qualification inputs. No current
compiler, Runtime, native SDK/RMW, DDS delivery or performance result is implied.
They extend the original business ABI with a separate, explicitly selected
`uwvm_ros2_rmw_sub_v1` namespace. Existing publisher and service-client modules
retain their exact signatures. The original 81-function catalog is unchanged.

## Authority and object lifetime

Open an explicitly granted `topic.subscribe` source using the original ROS2
backend. Activation eagerly creates one real retained Subscription; inspection,
borrowing, confirmation and retirement address that same native incarnation.
The Guest selects no topic name, type, QoS or management principal through this
extension. The true Runtime identity, generation, full source handle, native
session and independently minted native incarnation must all match. Default
rights remain empty apart from the separately configured standard streams.
The extension exposes no update, debugger or Host policy operation.

Actual UMRE256 activation/inspection bytes include complete QoS for the retained
Subscription. They have no publisher GID: that fact belongs to a received record.
The finite history keeps failed/retired owners and prevents same-handle reopen,
role switching and native-incarnation reuse. Close, revoke, session loss and
instance drain tombstone all matching native owners before best-effort cleanup.
Publisher and Subscription cleanup share one deadline beginning before the
mandatory scan, then select the correct trusted private retirement action.

## Read, validate, then explicitly confirm

1. Preflight the entire destination, expected original GID width (1..32), and
   capacity including **UMRS96 + URMI192 + the entire canonical message**.
2. `subscription_peek` borrows one complete real record and performs one final
   checked Host copy. The internal `record_bytes` field supplies the length;
   there is no separately writable length pointer. Errors preserve destination
   bytes. Idle11, insufficient space90 and missing metadata95 do not consume.
3. Validate all header/record/body bounds and the complete native metadata.
   Its publisher GID, signed sequence fields and timestamps must come from the
   same real take as its body. A logical zero-length body stays zero.
4. Pass both the positive accepted stream sequence and the independent positive
   reservation nonce to `subscription_commit`. Only its exact successful ACK
   permits a prepared RMW result to become visible to application code.
5. Use `subscription_cancel` to release that exact borrow without consuming it.
   Re-reading the same accepted record receives a fresh nonce; stale tokens
   cannot commit or cancel a later borrow of the same sequence.

Host admission charges the canonical body once per admitted peek. UMRS/URMI and
ACKs are metadata, and cancellation does not refund already admitted traffic.
If a successful borrow fails Host quota or final-copy admission, the Host sends
one explicit cancel under the original deadline. Primary transfer error, last
cancel outcome and native retirement are independent retained observations.
Before entering an explicit Host cancel RPC, its separate observation becomes
pending115. This means a Host cancellation attempt began; allocation or framing
may fail before send, so it does not attest actual Native execution. A real ACK
or returned error replaces that pending observation. A thrown/unknown outcome
retains pending115 through session retirement, never an earlier successful cancel.
Malformed responses and uncertain/lost/late ACKs retire the session. Do not
retry them automatically, claim rollback or turn uncertain commit into success.

The wire accepts a whole reply up to4MiB. The existing gateway topic configuration
has a more conservative canonical-body ceiling retained for original request
compatibility. A wire-ceiling fixture is not proof that a configured application
was granted that ceiling. Native type-specific validation remains mandatory.

## Qualification sources

`value_contract.cc` supplies eight always-active production-history contracts,
including mixed publisher/subscriber drain and preservation of independent
primary/cancel/retire outcomes. It uses logical fixtures and supplies no native
ownership evidence. `raw_signature_contract.c` checks the actual seven SDK
function declaration types without substitutes. These two checks are disabled
by default and must be built in an isolated future test directory.

`raw_import_inventory.c` is a non-starting Wasm link fixture with one explicit
export. It deliberately refers to every real optional import and defines none
of them. It is not an application and must not be executed as a robot controller.
During the unified test phase, with the selected real WASI SDK:

```sh
mkdir -p /tmp/uwvm-rmw-subscription-qualification
"$WASI_SDK_PATH/bin/clang" --sysroot="$WASI_SDK_PATH/share/wasi-sysroot" \
  -O0 -nostdlib -I sdk/include tests/ros_rmw_subscription_transport/raw_import_inventory.c \
  -Wl,--no-entry -Wl,--export=uwvm_rmw_subscription_import_inventory \
  -o /tmp/uwvm-rmw-subscription-qualification/imports.wasm
python3 sdk/tools/check_rmw_subscription_imports.py \
  /tmp/uwvm-rmw-subscription-qualification/imports.wasm --require-all
```

Use the independent full validator on those exact bytes. Wasm32/Wasm64 builds,
Runtime import dispatch/caller guard, wrong-owner/default-denial, output
atomicity, quota/deadline failures, actual native GID/QoS and end-to-end embedded
NUL messages require separate real runs. For combined Guests explicitly select
`--with-publisher` and/or `--with-client`; every other non-ROS import module
requires `--allow-module`. Unknown ROS imports cannot be allowed through that
flag. The existing publisher checker remains deliberately publisher-only.

On Linux, perform all later configuration, compilers, fixtures, ROS nodes, QEMU
and profilers inside the approved64GiB memory/swap0 cgroup. On the local machine,
keep build/test products under `/tmp`. Implementation comes before the unified
correctness, security, interoperability and hardware-counter performance phase.
