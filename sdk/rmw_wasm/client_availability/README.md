# Original RMW service availability on the same retained client

This SOURCE cohort connects the real original `rmw_service_server_is_available`
entrypoint to the actual C14 retained-native raw12 helper. No candidate was
imported, configured, generated, compiled, linked, probed, tested, executed or
applied by its author. This is source implementation and authored contracts,
not an installed Runtime, SDK, native endpoint or ROS delivery qualification.
Old81 imports, raw12 numeric declarations, Host/Gateway, codecs, Native protocol
and original subscription/publisher providers are unchanged.

## Explicit selection and ownership

`uwvm_rmw_endpoint_selection` gains only a trailing optional
`availability_alias`. The original `aliases[2]` and `alias_count==2` remain the
service.call/event.observe pair. Only a CLIENT can have a query alias. It must
be nonempty bounded valid UTF-8 and distinct across the WHOLE selection from
all call/observe/advertise/respond/topic/query aliases, including later rows.
NULL confers no query right and keeps the exact original two-owner client path.
No endpoint count, native object quota or permission is implicitly increased.

The private resource has one extra fixed owner-index slot. A new private
`uwvm_rmw_owner_open_availability` function accepts only a known live CLIENT
resource in its actual admitted leased Context. It verifies both original open
owners are already retained in that same Session/context before using ONLY its
selected trailing alias. It is not a public arbitrary-alias opener. Successful
Session retention is followed immediately by fixed resource retention; malformed
returned ownership is still retained before refusal. Capacity3 is checked BEFORE
ANY client owner opens, allocations or native effects. Other endpoints keep their
original bounds and opener. The total Session capacity remains UWVM_RMW_MAX_OWNERS (256 here): an optional client
uses3 instead of2, not an enlarged arena.

The order is reserve local request/event buffers and cleanup record; open actual
call; open actual observe; optionally open actual query; then activate the SAME
original typed native client and attest both actual QoS profiles. Query-open
failure retires the first two owners without native activation. Later activation
failure retains all three independent cleanup obligations. No query creates a
second native/probe client. It is a logical permission owner for the already
retained native client, not an advertised service/readiness declaration.

Native-retire-first and reverse raw cleanup remain the ORIGINAL shared B loops.
They now naturally include the recorded query index: query, observe, call.
Shutdown also visits all retained raw owners; later entity destruction does not
replay native or raw retirement. Every query-close failure remains in its own
preallocated B diagnostic slot, alongside independent source/observer and native
retirement results. C14 raw_close[2] still denotes ONLY call/observe; a query
result must not be projected into that pair. Every actual B slot is retained and
printed by these bootstraps. Unknown effects still require the real Host
instance/process owner to drain; no ordinary cleanup retry is introduced.

These are source-level trailing fields/private layout extensions. All B/C,
providers and applications MUST recompile from the exact overlaid headers and
complete system-inclusive MD closure. Version1 is not a promise of old binary
layout compatibility. Immutable selection/provider/state must outlive every
option, context, entity and callback. The C singleton still has no uninstall.

## Genuine immediate query, atomic output

The C profile gains a trailing optional versioned availability vtable. Zero is
absent; unversioned state/callback or incomplete/unknown version is refused at
bootstrap. A configured query alias with an absent vtable refuses95 BEFORE
opening any client owner. With no query alias, construction follows the original
two-owner path and public availability returns95 without querying anything.

The public original RMW function validates the registered node/client and exact
node association, enters the real whole-Context scope, checks the retained query
owner, inspects the SAME complete native endpoint receipt/source/observe pair,
and calls the explicit provider once. Its private unsigned output starts at
UINT_MAX; success requires exactly0 or1, then one final public bool write.
Errors, missing writes, malformed boolean, foreign node, busy context, revoked
query or changed source/incarnation/QoS preserve the caller's output. Query
failure does not send/replay a business request or manufacture success=false.

`uwvm_rmw_service_transport_availability_callbacks` exports that actual C14 state
as the optional vtable. Its wrapper calls the ALREADY IMPLEMENTED genuine helper
with wait0, preserving same-native association checks and raw12's strict byte
0/1 validation. It is also valid with the same C14 state's explicitly composed
C9 subscription callbacks; no subscription state is replaced. Output conversion
is private and nonfallible after validated success. Export makes process lifetime
sticky exactly like the other C14 callback accessors.

Zero wait means one actual native observation. There is no graph-list surrogate,
new timer/clock grant, hidden WASI sleep, polling-to-time conversion, blocking
waitset claim or native availability result inferred from a declaration. Real
finite upper-layer waits remain separately admitted implementation/qualification.
Every Runtime service.available alias still needs the actual same-service-source
alias, resource/type/native/codec/QoS binding and policy; selection grants none.
Actual complete zero-valued native GIDs/writers remain data, not absence markers.

## Complete original-object examples

`original_client_c` is a complete original RMW C main using the separately closed
pure-C Registry and actual generated C Empty/Add values. `original_client_cpp_objects`
is an honest C API main plus a complete actual generated C++ object/CPP10 companion.
The latter does NOT encode a C structure through a C++ codec. Original companion
source is copied byte-for-byte and its native identities are eleven mandatory
actual independently exported values. No hash is embedded/invented here.
The C Registry remains process-lived after any export; the C++ companion has
process lifetime as in the independently reviewed C14 example.

Both programs retain full client/server modes and complete request/response
values. Add request17,-9 and response8, Empty placeholder0/logical0. Explicit
8-domain QoS, domain, aliases and service name are required. In client mode,
call/observe/query aliases are all distinct. After actual endpoint creation,
one public availability query prints the genuine boolean. A false result stops
without sending a request; it is valid data, not a retry or successful response.
True permits ONE actual request and bounded complete response take as in C14.
Server mode uses two original advertise/respond owners; the required extra CLI
token is explicitly unused and never opened. Finite4096 take attempts are not
seconds or a readiness guarantee. Actual same-record native ID/times remain
separate from the legitimate RMW-local request sequence.

The complete invocation shape is:

```
original_client_c client empty call-alias observe-alias query-alias /empty 0 ID0 ... ID10
original_client_c client add call-alias observe-alias query-alias /add 0 ID0 ... ID10
original_client_cpp_objects client add call-alias observe-alias query-alias /add 0 ID0 ... ID10
original_client_c server add advertise-alias respond-alias unused /add 0 ID0 ... ID10
```

The ID tokens are placeholders for actual external64-hex inputs in original
Registry order; no command was executed. Opposite ORIGINAL native peers are the
current reviewed C14 `tests/native_services.cc`, with separate process/toolchain,
actual domain/isolation/readiness/ownership. No native peer is embedded into the
Guest or automatically launched. B raw-close and C14 native-retire diagnostics
are kept independently, all endpoint/node/context/options cleanup attempts run,
and registry/provider immutable storage is never freed while C can borrow it.

## Explicit source composition and unexecuted contracts

Build instructions are future Root-owned operations, not execution permission:

```sh
cmake -S "$source/sdk/rmw_wasm/client_availability/compose" -B "$fresh_guest" \
 -DUWVM_COMPOSED_SOURCE_ROOT="$source" \
 -DCMAKE_TOOLCHAIN_FILE="$actual_complete_static_wasi_toolchain" \
 -DUWVM_RMW_AVAILABILITY_MODE=STATIC_WASI \
 -DUWVM_RMW_AVAILABILITY_BUILD_C_EXAMPLE=ON \
 -DUWVM_RMW_AVAILABILITY_BUILD_CPP_EXAMPLE=ON
cmake --build "$fresh_guest" --target original_client_c original_client_cpp_objects --parallel 1
```

Actual A platform package, complete original SDK/generated libraries and genuine
static forwarding/link facilities must exist. No fetch, shim, weak raw symbol or
substitute ROS header is supplied. Pure-C14 must be reviewed/integrated first;
its held/final source provenance stays a separate actual dependency. The CPP10
attachment is explicit and may coexist at build time; a selected endpoint/object
still uses its ONE matching C or C++ codec table. Do not select both same-name
descriptor families in one profile.

A SEPARATE native-contract overlay/toolchain can opt into BUILD_MODELS with
NATIVE_CONTRACT. `tests/contract.c` retains the original35 C groups and adds17
named availability groups, each fresh process because C bootstrap is immutable.
They cover absent alias/provider, true/false, permission13/revocation125,
unwritten/invalid bool, foreign node, busy scope, source/QoS drift, three-slot
capacity before opens, query-open failure, activation failure, once-shutdown,
independent query-close failure/order and all-table alias uniqueness. These are
explicit LOCAL provider models linked to real original C objects and real B/C;
synthetic metadata/permission results are not Native/Host proof. Assertions remain
active via -UNDEBUG and source NDEBUG refusal.

A separate C++ model unit reuses the exact current C14 full raw12/raw7 test fixture
as source, renaming but NOT invoking its original main. It calls the new wrapper
on actual C14 state with true response,13/125, invalid boolean, same-native/source
mismatch, foreign context and duplicate source/peer query handles. This separately
checks the wrapper delegates the real helper and preserves output/once ownership;
its raw definitions are explicit models, never production imports or Native
values. Original18 C14 tests and original35 C models remain unchanged in repo.
All groups are authored/unexecuted here, with no claimed test pass.

Actual compiler/link/probe checks, complete MD/ELF/import classification, default13
and granted fresh Guests, actual same typed native server appearance/disappearance,
query revoke/close, source mismatch and complete retirement under independent
Root ownership remain required. Native/Wasm/ROS counts remain0. This finite
availability seam does not complete original RMW or all distributions/interfaces.
