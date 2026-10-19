# Python owned ROS2 Service SDK

This is an additive CPython3.14.8/WASI implementation over the optional
`uwvm_ros2_rmw_srv_v1` ABI. It executes in the same Wasm as the original Python
SDK. It uses actual numeric imports and independently opened SDK capabilities.
It supplies no host-Python bridge, ctypes, socket transport, native rclpy wheel,
background thread, implicit discovery, automatic retry or policy-management API.

**Status:** authored private Source; no syntax parser, generator, compiler,
fixture, Guest, Native ROS, debugger, QEMU or performance run has executed for
this package. Independent Source review and the unified Linux qualification are
required. C DWARF is not Python source/locals debugging. Nothing here qualifies
the entire CPython runtime or proves that an application cannot exhaust memory.

## Select the bridge explicitly

`ROSWASM_OWNED_SERVICE` defaults to `0`, accepts only `0` or `1`, and `1` requires
explicit `ROSWASM_BACKEND=2`. The launcher conditionally registers the new
`_roswasm_owned_service` builtin. The business81 catalog, selected ROS1/38 or
ROS2/43 imports and three legacy client-extension imports remain unchanged.
The original generated dispatch is preserved, with an explicit optional C/open
witness seam and compatible default client call forms described below. No
ordinary/stable Runtime source is changed. A default build does not register
or link the new namespace or expose the optional open-witness methods.

Build `_owned_service.c` only into the actual target interpreter. Include its
target `pyconfig.h`, the matching CPython `Include`, `sdk` and `sdk/include`.
The final Wasm must contain all43 ROS2 business functions and all12 optional
functions with the complete exact parameter/result signatures. A whole-module
allowance is insufficient. `tools/check_owned_service_imports.py` captures the
actual original inspector bytes by SHA, calls its pure `inspect`, and compares
a closed full-signature multiset: missing, duplicate, nonfunction, unexpected
or wrong-signature imports fail. Reserved `uwvm_ros*` namespaces cannot be
declared through generic/startup rows. A separately reviewed SHA-paired finite
startup manifest enumerates every actual WASI/generic signature, with no module
wildcard. This manifest's provenance must be admitted by the keeper; the checker
cannot decide that arbitrary operator-supplied startup imports are safe.

The raw catalog SHA is
`9e743ad0077cfab6d96bd9ca37d4d23a8804d9c2593af70678b6406222b9ac18`.
Its actual raw-header/Host/implementation references are captured and checked.

This Source authority cohort is a new input identity. Frozen SDK packets and
their original pins remain historical; prior model/build/runtime/ROS/DBG results
are not retagged or extended by this metadata update. Fresh composition and
separate qualification are required.
The business catalog SHA remains
`0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0`.

| Index | Operation | Non-address arguments | Input | Output |
|---:|---|---|---|---|
|0|abi_version|none|empty|version retained in C; `version_one` boolean|
|1|client_activate|source, observer|empty|UMRE256|
|2|service_activate|advertise, responder|empty|UMRE256|
|3|endpoint_inspect|source, incarnation|empty|UMRE256|
|4|client_submit|source, incarnation|complete request|ticket8|
|5|client_abandon|source, incarnation, ticket|empty|empty|
|6|service_respond|advertise, incarnation, responder, ticket|complete response|empty|
|7|endpoint_peek|source, incarnation, expected writer extent|empty|UMRC160 + URMI192 + full body|
|8|endpoint_commit|source, incarnation, sequence, reservation|empty|empty|
|9|endpoint_cancel|same exact association|empty|empty|
|10|client_available|source, incarnation, distinct availability owner, finite wait_ns|empty|byte0/1|
|11|endpoint_retire|source, incarnation|empty|empty|

All handles/tickets/incarnations/sequences/nonces retain full unsigned64,
including `UINT64_MAX`. The writer extent is unsigned32. Python bool/subclasses,
negative/out-of-range integers, wrong input/output shapes and noncanonical
unused arguments are refused before entry. The C bridge chooses actual memory0,
addresses and extents. No Python caller supplies a Wasm memory address.
The copied-buffer maximum is4193786 (narrower than raw4194304); actual Host body
and grant quotas can be smaller, including the current body ceiling4193626. The Python typed delivery bound is at most4193434
(4193786 minus the complete352-byte prefix), and current control records are
bounded much more tightly by the shared32KiB schema.

## Retained C attempt and allocation failure

`prepare(index, handle, incarnation, a, b, exact_bytes, capacity)` allocates an
opaque capsule, exact immutable input root, private guarded C output, owned output
bytes and four errno bytes before the import. It admits the entire shape and
all exact integer conversions first. The capsule has this module's actual
destructor identity. It is not a capability, a Native object or a bearer token.

`execute(capsule)` sets the real attempted bit immediately before the import.
After return it checks canaries, stores the status and owned output, sets the
returned bit and returns the existing None singleton. This C path performs no
PyLong, PyTuple, Py_BuildValue, bytes resize, buffer release callback or result
allocation. Canary failure traps with an unknown outcome; retry is prohibited.
The context is permanently single-use. A successful output is copied once;
nonzero status preserves the already-zero owned output. `attempted`, `returned`,
`success`, `version_one`, `output` and `status_bytes` return existing rooted bytes
or singletons. Numeric diagnostic conversion can allocate later; its failure
does not discard the C context/status/output. Capsule destruction frees Guest
memory only and never invokes ROS.

This boundary is grounded in the retained adapted CPython3.14.8 Source, not a
claim about a pristine official tree or every interpreter configuration.
`PyCapsule_New` allocates before entry; normal `GetPointer` and destructor checks
are direct checks; `PyBytes_FromStringAndSize(NULL,n)` reserves writable bytes;
`Py_NewRef` increments a reference and returns the existing root; this version's
None return is the immortal singleton. Real compiler/allocator/OOM behavior
still needs the matching target build. Free-threaded/subinterpreter parallel
operation is outside this single Guest-thread bridge contract.

Python endpoint facts and fixed history rows install their C context before
execution. A complete delivery/result object, decoded body, full writer copy,
next history index and ACK context exist before commit. The endpoint becomes
inactive before ACK; success uses already retained values and existing private
field assignments to publish the delivery. An interruption leaves the context
and pending result/borrow observable. It never causes a second ACK/cancel/send.
This does not promise that arbitrary Python VM/tracing/application code cannot
allocate after the C return or that terminal Wasm traps run Python finally.

## Complete records and the seven original roots

`tools/generate_control_python.py` uses the actual shared `load_catalog` resolver
from exact captured SHA-paired `custom_types.py` and `bounded_profile.py`.
It does not call their generators or modify their shared Source. Catalog and
all original definitions are captured once; the actual parser reads only those
captured bytes. It requires the exact32 original record names, two registered
Topic roots and seven Service roots, with the real authoritative C/C++ canonical
generation proof/products from the same source pins.

The disjoint `control_python_emitter.py` reuses the existing Python dataclass
emitter and adds the explicit control raw-byte string domain, constants,
aggregate/cartesian metadata and per-record nested read/write extent checks.
It preserves every field, scalar alias, default, native bound, canonical bound,
constant and identity in the complete IR proof. Source `byte` and `char` keep
their actual native facts. `TypeSource.raw_file_contents` is arbitrary owned
bytes, including NUL/non-UTF8; ordinary strings are strict UTF8 without NUL.
Finite IEEE754 values, strict booleans, exact integers, every array/sequence,
all ParameterValue fields and full descriptions/type_sources are retained.
Aggregate quotas are independent from cartesian storage/wire capacities.
No Native identity is invented for a null preparation identity.

The generated package contains `bounded_records.py`, `control_services.py`,
`__init__.py` and `generation.python.json`. Stage it explicitly as
`roswasm/generated_control` before freezing. Root definitions are separate from
request/response component identities:

1. `rcl_interfaces/srv/DescribeParameters`
2. `rcl_interfaces/srv/GetParameterTypes`
3. `rcl_interfaces/srv/GetParameters`
4. `rcl_interfaces/srv/ListParameters`
5. `rcl_interfaces/srv/SetParameters`
6. `rcl_interfaces/srv/SetParametersAtomically`
7. `type_description_interfaces/srv/GetTypeDescription`

No generated outputs are claimed to exist until this operator actually runs on
the admitted Linux producer. The old optional managed CLI's ordinary/PYC loader
is not used. The new adapter captures/executed bytes only after explicit SHA
pairing; its entry, interpreter, stdlib and C extensions remain the keeper's
trusted startup boundary, which cannot be retrospectively certified by itself.

## Construct and drive genuine owners

Import the generated service definitions, then independently open call and
event.observe owners from one real `Session(Backend(2))`; a server needs separate
advertise and respond owners. Optional availability needs a third independent
same-Session grant. A borrowed descriptor, integer, matching alias/name, checksum,
QoS row or codec identity does not grant authority. Constructors require exact
registered SDK owners with the actual built-in backend, distinct references and
full saved handles. Host again validates current caller/source/session/pair,
operation grants, Native identity and quotas on every operation.

Python's existing Capability constructor and Session owner list are public and
mutable. Exact class/builtin identity and list membership are local consistency
checks, not a private-mint certificate or an isolation boundary inside one Guest.
The actual applications acquire their owners through real Session.open. A
malicious Guest could already invoke numeric imports directly; only the real
Host current-instance/session/generation/operation checks confer authority.

```python
from roswasm import Backend, Session
from roswasm.owned_service import OwnedClient
from roswasm.generated_control import control_services as services

session = Session(Backend(2))
call = session.open('explicit-call-alias')
observer = session.open('explicit-observer-alias')
client = OwnedClient(call, observer,
    services.Ros2Rcl_interfacesSrvGetParameters,
    expected_profiles=(actual_selected_request_qos80, actual_selected_reply_qos80),
    history_entries=64, ownership='owned')
try:
    status = client.activate()
    # Stop on nonzero; retain client for diagnostics and independent cleanup.
    if status == 0:
        submitted = client.submit(complete_generated_request)
        # Only explicit caller credits call client.take(); no loop/timer here.
finally:
    cleanup_status = client.close()
    # Preserve cleanup_status and client.facts(), even with a primary exception.
```

For allocation-safe multi-open setup use the application classes in
`owned_service/applications.py`: retain the application before `open()`; it
reserves the real Session factory's owner slots before first acquisition, stores
each endpoint before activation, then independently rolls back all raw owners.
Both primary and cleanup results/exceptions remain in that application even
when the original exception cannot accept attributes. The short example above
shows the direct owner form and requires the caller's own partial-open finally.

The application factory selects `Session.open(..., owned_capture=True)`.
This explicit path requires an already retained factory frame; it is refused
before any import without one. Each pending owner holds its genuine optional
legacy C open witness before acquisition. The witness retains the complete
returned handle, open entry/return/code, exact C primary exception reference,
and once-only fallback-close entry/return/code independently of all subsequent
legacy invokes. The actual C close updates that owner's pre-reserved retirement
cell immediately before the numeric import, so a later observation/OOM cannot
make the frame replay it. Python `Result` construction failure uses this same
actual C close, rather than another allocating generic invoke. Cleanup errors
do not depend on adding an attribute to the primary exception.

The legacy-open token is now a private exact C GC type, rather than an untracked
capsule with a hidden primary reference. Its real `tp_traverse` visits input,
retirement cell and exact primary; it is tracked only after every edge is
initialized. `tp_clear` first marks the token unavailable, then releases Guest
references, and deallocation untracks/frees only Guest storage. These routines
make no direct ROS/Native/import calls and never replay cleanup. General DECREF
can run Guest finalizers; reentry into a cleared witness is refused. Reachable
failures keep their exact primary and traceback unchanged. Neither normal close
nor snapshot strips an exception traceback to hide retention. The type has no
public constructor or subclass permission. Its readiness is established during
module initialization, before any capability effect. This remains a single
Guest-thread, actual-target C API contract, not all-interpreter qualification.

`application.factory.owners` retains these pending owners even when they never
received a Python handle. `application.facts()['open_captures']` exposes their
capture objects; `capture.snapshot()` copies the retained full facts and may
allocate. A failed snapshot does not clear them. `capture.cleanup_failed()` and
`capture.pending()` distinguish failed/unknown attempted cleanup from a known
unattempted handle. No automatic retry of an entered close is permitted. A
snapshot or local object is not a Host grant. The original generic bridge's
last-global diagnostic remains a legacy observation, not this path's carrier.

The optional C API exists only with `ROSWASM_OWNED_SERVICE=1` and backend2.
The original generated dispatch is preserved. Ordinary calls without the
explicit capture selection keep their original generic open/retirement route;
default0 does not expose these new builtin methods. Source/API additions can
still change import-time memory or interpreter work and need actual regression
and performance tests; no zero-overhead claim is made.

Activation is once-only. UMRE256 validates the complete current binding,
nonzero incarnation, TWO QoS80 rows and full client GID of0..32 bytes; a Service
has no client GID. Inspection must match every activation byte except its
operation byte. A client GID is not the request writer GUID16. Every delivery
validates full UMRC160 association, finite exact capacity/record extent,
monotonic fresh nonce and sequence (client stream contiguous), then full URMI192
same-take owner/incarnation/ticket/body extent and all reserved fields. Native
sequence and timestamps preserve signed64, including zero/negative values; full
zero writer GUID16 remains actual data. Error/closed control packets preserve
their actual reason/flags without invented service metadata.

`submit` consumes a finite lifetime row even on attempted error11. Admission is
not completion. `take` decodes the entire body and checks ticket/native-ID
history before ACK. Associated refusal can cancel the exact sequence+nonce;
unassociated bytes cannot supply guessed cancellation tokens. A successful
`abandon` enters raw5 once from ADMITTED; a second abandon is locally refused.
Late abandoned replies still pass the entire codec before one ACK/disposal.
Unknown commit/cancel never retries, rolls back or delivers application success.
`respond` burns a delivered request row before actual entry and never resends on
timeout/unknown status. Application handlers run after take/ACK.

`close` keeps endpoint reentry exclusion through Native retirement and raw
cleanup. It releases each SDK owner busy lease before that owner's genuine
close. Native retirement is at most one actual attempt with its preallocated
context; separate raw owners close independently in reverse acquisition order.
Returned errors and raised exceptions are retained separately and produce
nonzero cleanup status. Genuine pre-entry raw-close refusal keeps ownership;
attempted raw-close failure remains retired by the original C witness. Borrowed
owners remain the caller's explicit cleanup duty. If activation cannot produce
a validated receipt/incarnation, this API cannot invent a Native retirement
token: it preserves the activation context and still closes actual raw owners;
Host instance/session teardown is required for unresolved terminal outcomes.

## Two caller-driven applications

`sdk/examples/python_owned_service/type_query.py` and `parameters.py` are actual
frozen entries with `main()`. Both use `owned_service.drive`; they do not depend
on ambient filesystem modules. The first selects GetTypeDescription; the second
selects all six parameter services with separate alias pairs. Both support
client/server roles, full generated request/response objects and explicit
availability owners. The caller supplies the first bounded JSON line:

```json
{"role":"client","history_entries":64,"plans":[{"source":"type-call","peer":"type-observe","availability":null,"qos":["FULL_QOS80_LOWERCASE_HEX","FULL_QOS80_LOWERCASE_HEX"]}]}
```

Use six plan rows in the documented service order for `parameters.py`. Each
subsequent line is one command: `submit`/`respond` includes the entire canonical
wire as lowercase hex; `respond` also includes the exact accepted ticket;
`take`, `abandon`, `available` and `close` are explicit. All non-close commands
include a service index. `available` includes a finite wait_ns0..60000000000.
There is no namespace wildcard, callback pump or automatic scheduler. A take
reports full canonical output and actual signed timestamps/writer GUID. Output
printing can fail after consumption; endpoint `last_outcome`/C context retains
the result, and this cannot authorize a retry.

The driver reserves `application.command_failure` before any open. The first
nonzero business status stops its command loop and returns process failure1
through the actual frozen embedding, even if every cleanup returns0. The one
documented exception is `take` status11 with no delivery and no exception: it is
an expected empty poll. A delivered error/closed control is a failure even if
the consume operation itself returned0. Statuses, the complete first outcome,
and exact raised objects stay retained independently of cleanup/shutdown facts.
Raised exceptions are re-raised unchanged. Cleanup and shutdown are attempted
independently; their exceptions never mask that primary. EOF/explicit close
returns0 only when no prior command, cleanup or shutdown failure was observed.
No status-only stdout grep can substitute for checking this actual exit code.

The unexecuted `owned_service_main_failure.py` entry calls that same real driver
and preserves exit1. The keeper must require its exact marker, expected reached
operation/errno and real process result together. Failure before the intended
operation is not acceptance. It needs actual plans and a genuine Native peer.
`owned_service_command_failure.py` is only a local carrier model.

`owned_service_open_boundary_fixture.c`, linked instead of legacy `_native.c`
in a separate target interpreter, includes the actual bridge and new header.
Its counted open returns a full high-bit handle, then an actual Obj allocator
fails before value/result construction. It requires actual attribute-attachment
failure, a later actual generic invoke clearing the old global, retained exact
primary/fullid/cleanup90 and exactly one close of that handle. Cached small errno
objects are not assumed to allocate. A missing intended fault fails the fixture.
The chosen open/close callees are counted doubles; other imports remain real.
This is not Native delivery or all Python VM OOM coverage, and it has not run.

The separate unexecuted `owned_service_open_gc.py` uses the real Backend capture
and the fixture's actual C allocation-failure path. The exact propagated
MemoryError acquires a real Python traceback back to the capture/token frame.
It verifies GC-visible referents, exact primary/traceback/full-id/close90 while
reachable, then drops every test root and explicitly collects three independent
failed lifetimes. Actual tracked-witness counts must return to their baseline,
and counted raw close totals must not change during GC. It also refuses direct
construction and subclassing. No fake exception, edited traceback, manual
`tp_clear`, weak-reference stand-in or model collector substitutes for this
path. This fixture is not a Native ROS result and has not run.

## Build and qualify on the existing Linux64GiB cgroup

1. Independently review this package and admit exact Source closures. Acquire
   the locked adapted CPython3.14.8 and matching SDK24 using the existing producer
   procedure; preserve all old artifacts. Pin interpreter/compiler/libpython,
   startup/stdlib/C-extension closure and actual original build receipt.
2. Run the explicit Python control generator with source root, catalog, genuine
   same-source canonical C/C++ revision and all four actual generator SHA values.
   Use a fresh output directory; inspect the complete32/seven-root proof.
3. Construct an externally SHA-pinned finite source closure, including complete
   Python SDK, dependencies, headers, selected entry and generated package.
   `prepare_owned_service.py` stages those exact bytes and emits commands only.
   `--owned-service` defaults0; `1` requires backend2. Its link command preserves
   the actual successful original libpython/static-library recipe, replaces only
   the launcher/frozen include, and adds the selected bridge objects/flags.
4. Execute every emitted command through the actual bounded keeper with finite
   timeout/RSS/process/cgroup physical-retirement evidence. Validate the whole
   final Wasm separately after closed imports. No successful preparation receipt
   is a compile/import/Guest result.
5. Run production-builtin `owned_service_guest.py` with EMPTY real ROS grants.
   Run `owned_service_values.py` for complete records/association negatives.
   Run the separately linked C boundary fixture and `owned_service_boundary.py`:
   its counted submit/commit doubles arm a real failing Obj allocator after
   effect, then require the actual C result/observation path to allocate nothing.
   It is a bridge contract test, not Native ROS delivery evidence.
6. Run both real apps against independently observed actual C/C++/rclpy/rcljava
   Native peers: all seven services, all fields/limits, availability third grant,
   quota/backpressure, full64 tickets, native-ID duplicates, abandoned late full
   decode, commit/cancel loss, revoked/session-stale handles and reverse cleanup.
   Inject actual pre/post allocation failures, raw-close returned error/throw,
   reentry and trace interruption with exact attempt counts. Repeat with missing
   grants and unsupported Native capabilities; refusal is not a positive pass.
7. Qualify actual UWVM DBG/source mapping and Python tracing separately. Compare
   hardware timing, VTune hotspot and microarchitecture against the baseline,
   Wasmtime/WAVM/WasmEdge where compatible. QEMU x86_64/aarch64 and other admitted
   ROS architectures remain separate real runs. Keep all under the existing
   cgroup, and clean only task-owned disposable outputs after retaining receipts.

The raw service transport and DefaultControl Native integration still require
their own complete target qualification. Canonical records, model output, C
capsule identity, CRC, source SHA, import inspection and generated proof do not
replace that qualification or the independent Host operation grants.

## Final-artifact signatures and debug reload admission

Follow the current [WSEA secure execution contract](../../documents/security/wsea.md),
including its actual platform, protected-storage and policy rules. Its tools
are `cosign wasm secure policy-init`, `policy-update`, `sign`, `verify` and
`fetch`. Admit the actual selected binary/tool and its complete arguments.
None of those operations has run for this Python application cohort.

Finish compilation, stripping and any embedded-signature/custom-section changes
before computing the final whole-Wasm SHA and detached signature. Retain the
exact final bytes with their `.sig.cose`, `.signer.pem` and `.crl.pem` evidence.
The coordinated Runtime selection is explicit and per module. Use
`--secure module <wasm-path> <protected-policy-path>` only when the policy has
exactly one module rule; its unique identity is bound to that path. A shared
policy with several rules requires
`--secure named-module <wasm-path> <protected-policy-path> <expected-id>`.
Repeat selections with each path's intended identity. A valid signature for
another authorized identity does not admit this application. A missing,
changed, rejected or unverifiable module must not
inherit another module's admission. A debugger reload that changes any final
byte must repeat verification and current policy admission before execution.

Secure function replacement and compiled-source replacement currently return
`unsupported_mode`; use a fresh admitted load. Already running instances have
no automatic background revocation or stop mechanism in this increment. These
source contracts do not supply Python application or VM execution evidence.

The signed capability mask must fit the selected local rule; a wider signed
mask is refused, not silently reduced. Bits are basic WASI=1, fd/path/poll
including stdio=2, sockets=4, ROS1=8, ROS2=16 and other Native imports=32. Bits2
and4 also require bit1, so WASI stdout normally needs mask3. Actual imports and
import-reset destinations must fit the signed ceiling. These bits do not mint a ROS capability, replace
instance/session/generation checks, authorize a vendor endpoint, prove Native
identity or establish sandbox/debugger correctness. A non-ROS example still
needs its actual WASI/import policy, and each ROS/vendor deployment needs its
own independently admitted endpoint grants and operational tutorial.

Python source maps, typed locals, frames, argv behavior, tracing and ROS-aware
DBG are separate unfinished qualification areas. C DWARF, a debug section name,
a signed module or a successful different-language example does not satisfy
them. Full rcl/rclcpp/rcljava/rclpy feature parity and complete vendor migration
tutorials are also broader work than this owned-Service addition.
