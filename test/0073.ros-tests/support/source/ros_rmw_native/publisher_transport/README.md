# Concrete Guest C publisher transport

This optional component implements the original B publisher callback seam using
only the five actual `uwvm_ros2_rmw_v1` imports. It does not change the original81
ABI, B structs, generated codecs, A/C providers, installed ROS headers, policy,
source open, topic dispatch or Host/Gateway implementation. All sources and the20
local groups are unexecuted. This is neither native endpoint qualification nor
ROS acceptance.

## Source and native prerequisites

Use the complete genuine upstream RMW SDK selected for A/B/C. The actual B
`bridge.h`, `codec.h`, `contracts.h` and the original rmw/rcutils/time library must
be in the same source-recompile and compiler-MD closure. The independent finite
`compat/rclc` facade must never shadow original `rmw` headers. Link real
`rmw_time_equal`, whose documented normalization/infinity semantics this provider
uses for all three duration comparisons. In particular, a valid nonnormalized
requested/effective duration is not rejected merely because its nanoseconds are
at least one billion. The attested UMRE wire itself has normalized nanoseconds.

The caller must first compose the reviewed Root raw header
`sdk/origin/include/roswasm/rmw_publisher_v1.h` and its actual five-import Host/Gateway
implementation. This candidate pins that real fixed header as read-only support;
it does not copy or replace Root's mutable transport cohort. Absence of those raw
symbols in a native final application is an unresolved-link error. For a Wasm
application they are five genuine unresolved Guest imports requiring the actual
selected Runtime namespace; do not enable a generic undefined-import stub or
pretend a static archive alone proves that namespace exists. Requalify the actual
final import inventory and whole MD/object/link closure.

## Bootstrap and lifetime

Before any init-options/context can borrow the selected profile, allocate state
with `uwvm_rmw_publisher_transport_create(capacity,&allocator,&state)`. Capacity is
explicitly1..2048, bounded by eight contexts times256 B owner slots. The factory
makes a single checked `zero_allocate` for every slot, full256-byte original and
last raw receipt, complete parsed binding and fixed diagnostics. No callback
allocates, grows a container, retains message bytes or opens an alias. Logical
zero-byte codecs are admitted and stay zero bytes on the wire.

Obtain the real callback table with
`uwvm_rmw_publisher_transport_callbacks(state,&provider)`, then put that table into
a NEW immutable explicit `uwvm_rmw_profile` before `uwvm_rmw_install_profile` and
before any init options. Supply actual complete generated native codec
validation/encode/decode callbacks, not the local descriptor from the models.
The profile must explicitly map the original node/topic/type/requested and actual
selected effective QoS to one `topic.publish` alias. The provider never infers
rights from a backend, topic string, GID, receipt or template. B repeats exact
profile/codec selection and opens the logical source through the real original
backend. Only then does this provider activate the new native path. It never
calls the old lazy topic-publish import to create a second native owner.

State and allocator must outlive ALL borrowing profiles/options/contexts and
cannot be moved or reset. Bootstrap/destruction are exclusive caller operations.
The atomic busy flag refuses17 for recursive callback/diagnostic use of fixed
state; it does not add cross-function or upper-rclcpp callback leases, claim a
multithread executor, or replace B's real RMW function-scope lifetime. One complete
source handle consumes one permanent state slot before even the raw ABI query.
A failed version check, activation, timeout, retired source or different later
context never automatically reuses that slot or retries that handle. Create fresh
state only after ALL old profile/context ownership has ended, never as an
activation-retry mechanism.

After real RMW resources/nodes are destroyed or shutdown has attempted all B
cleanup, save both B diagnostics and this provider's diagnostics. Finalize every
borrower, uninstall the selected profile and only then call
`uwvm_rmw_publisher_transport_destroy(&state)`. It refuses17 if a known associated
incarnation has not received one retire attempt. Its successful return releases
LOCAL storage only. It deliberately does not dereference cached context pointers
that B may already have finalized. The external lifetime precondition is an
ordinary explicit C ownership contract; the function cannot inspect arbitrary
live RMW contexts and does not claim it has done so. Never free it with a live
borrower merely because that borrower's native retire has already run.

## Complete native receipt and same-object traffic

UMRE-v1 is decoded by offsets, not by C struct memcpy or enum ordinal casts. The
provider requires all256 bytes; correct magic/version/operation/backend/length;
publisher role; exactly one complete actual QoS profile; all header, unused second
profile and GID-tail reserved bytes; full nonzero source halves and nonzero
instance/generation/session/incarnation. It maps every explicit protocol enum to
its original named RMW enum and checks size_t depth, KEEP_LAST depth, normalized
wire durations and the avoid-namespace boolean. All eight QoS domains are kept:
history/depth, reliability, durability, deadline, lifespan, liveliness, lease and
avoid_ros_namespace_conventions. Actual QoS comes exclusively from this receipt,
not the selection row. The row is only the expected comparison after parsing.
The UMRE record is NOT a native type hash/schema proof: full native admission
remains the actual selected codec callback plus the actual Host grant and typed
factory. The model token is deliberately not such a proof.

A full all-zero actual GID of the selected complete extent is valid. Widths up
to32 retain every raw byte. If the actual native extent differs from
`RMW_GID_STORAGE_SIZE`, activation returns95 with its full associated cleanup
receipt and actual `native_gid_bytes`; the incompatible public fixed array stays
zero and is NOT exposed as a padded/truncated GID. `original_wire` preserves all
native GID bytes. B stores that binding even with the95 return and still invokes
once-only native retire before raw source close. A real QoS mismatch similarly
returns22 with a cleanup-only receipt. Neither cleanup-only activation can later
become a business endpoint by calling inspect/publish directly.

On a successful activation every inspect uses the same original owner address,
full source, context, instance/generation/session and native incarnation. It
compares the complete new raw receipt against the ORIGINAL admitted raw snapshot,
ignoring only the command byte1/2. Drift never refreshes or replaces the admitted
snapshot. It preserves failed inspection output and retains the successful raw
reply and separate validation error. Publish first obtains that same-object
inspection and then invokes the one real new publish import once with the whole
bounded canonical body. It does not retain caller bytes or send a private dummy
byte for an Empty message. It never retries a90/107/110, reallocates and replays,
or substitutes an old81 publication. A later explicit application call is a new
operation, not automatic replay of an earlier ambiguous operation.

Retire validates the same complete admitted public binding, including a
cleanup-only incompatible-width binding. It records attempted BEFORE its sole raw
retire call. A failure is retained separately and a second attempt returns125
without another import. Business traffic returns125 after the attempt. B must
still close the logical raw source even when native retirement fails, and its
fixed native/raw/local cleanup outcomes stay distinct. A malformed successful
activation reply or an activation error may leave an unknown native effect: the
provider preserves the raw diagnostic but never invents a usable incarnation.
B raw source close and actual Host session/instance drain remain independently
required. A local free or failed cleanup is not proof that the native owner died.

## Build and local protocol-model recipe

These command shapes have NOT run. Use distinct owned build/output directories,
the actual WASI toolchain and installed SDK, strict original Host admission and
complete compiler MD/link/ELF/import evidence under the Root execution owner.
The component CMake entry is `sdk/origin/rmw_wasm/publisher_transport`; it requires
`UWVM_RMW_INCLUDE_DIR` pointing at current B headers and
`UWVM_ROS_SDK_INCLUDE_DIR` pointing at the genuine SDK raw include root. For an
actual application, add this component to the real A/B/C source build and link
`uwvm_rmw_publisher_transport` explicitly into the final static application.
Do not assume the unchanged A CMake automatically discovers this new directory.
All imported source headers, target flags and actual selected SDK/library inputs
must appear in the final MD closure; install/export packaging is still an
explicit deployment step, not performed by this candidate.

For local model construction only, set
`-DUWVM_RMW_PUBLISHER_TRANSPORT_BUILD_MODELS=ON`. Its native-only executable defines the exact
five raw prototypes strongly and uses ORIGINAL installed RMW/rcutils headers and
real linked rmw_time_equal. It explicitly refuses a Wasm build, where the genuine
header declares imported functions that this model must not redefine. Use an
independently admitted native SDK/toolchain for models; this is no Wasm result.
No model source is in the production library. The
always-active CHECK macro and explicit -UNDEBUG/source NDEBUG refusal prevent a
Release build from eliding checks/effects. The20 directly called groups cover
capacity/allocator validation; complete zero GID/QoS; explicit enum mapping;
reserved fields; every binding member; malformed QoS; incompatible native GID
retirement; cleanup-only QoS mismatch; equivalent nonnormalized native duration;
version absence/no reactivation; ambiguous activation; permanent source slots;
full snapshot drift; copied-owner/changed receipt; bounded real import payload;
logical0; timeout/cleanup separation; inspection error; retire once and recursive
admission. They are a source inventory, not20 passed tests. The local descriptor
has no fabricated native hash and is intentionally insufficient to install a B
profile; these groups cannot be used as a native type or RMW acceptance result.

Required later positive work remains actual complete source/native codec export,
real Raw namespace import matching, installed SDK compilation/link, real native
publisher GID/QoS/full-value/disappearance witness, Host same-incarnation registry
and Runtime cleanup, zero-default authority denial, real Wasm calls and unchanged
upstream RMW/rcl/rclcpp lifetime integration. No marker or model count supplies
that evidence. Other languages, subscriptions, services, discovery, availability,
finite wait and vendor source/native qualification remain separate work.
