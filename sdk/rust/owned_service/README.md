# Explicit Rust owned-RMW Service transport

This separate Rust crate implements the actual optional
`uwvm_ros2_rmw_srv_v1` twelve-import protocol. It creates one retained native
client/service from explicitly opened owners and uses that same incarnation for
inspect, request, response, metadata, availability and retirement. It performs
no old business `service_call`, `service_submit`, observer poll or legacy
advertise/respond fallback. The existing `sdk/rust` crate and original81 business
imports are unchanged. All source, contracts and examples in this revision are
unexecuted; no compiler, Guest, Native SDK or ROS success is claimed.

The crate depends on the real `roswasm` SDK with `alloc,ros2`. The ordinary Wasm
transport requires wasm32 pointers into memory0 and refuses95 before every import
on other pointer targets. Imports keep memory i32 and all offsets, extents,
handles, incarnations, tickets, sequence and reservation arguments u64. Only
original capability open/close/ABI query is reused. There is no Guest management,
policy, debug, clock, thread or provider-loader import. Default rights remain
empty. `Group::new` allocates storage but opens no capability or native endpoint.

## Exact codec and Service bindings

`Codec<T>` contains an explicit maximum, an encode function returning owned bytes
or `Error`, and a complete private decode function. `Codec::canonical(maximum)`
adopts the genuine `roswasm::typed::Canonical` implementation. `Definition<Q,S>`
contains the original **Service-root** native type and canonical identity plus
the two component codecs. Root and component canonical IDs are distinct. These
strings are declaration/selection facts for a coherent bootstrap, not authority
or Native proof. The numeric receipt has no type-name discovery field: this
crate cannot independently read the Host's source configuration. The actual Host
checks current alias, owner, original native type, full codec identity, both QoS
contracts, paired operation and quotas on every real operation.

`examples/control_bindings.rs` binds all seven original control roots to the
separate actual generated records and their exact root identity constants:
DescribeParameters, GetParameterTypes, GetParameters, ListParameters,
SetParameters, SetParametersAtomically and GetTypeDescription. It never computes
a Native hash. Stage the independently reviewed records-only generator output
at `sdk/custom-types/generated-control-managed` in a coherent private SDK tree.
The source operator/HOLD is a separate dependency; this crate does not invoke it,
create replacement generated records or modify its historical C/C++ outputs.

All32 generated records preserve complete fields, constants and defaults.
Only `TypeSource.raw_file_contents` is counted raw bytes, including every NUL and
invalid UTF8 byte. The GTD response has the actual independent aggregate32768
cap, raw6162/source12/refs11 bounds and complete Cartesian shape. Ordinary text
remains strict UTF8 without NUL. A malformed record is22; a logical/output cap is90.
No failed reply/callback is converted into an empty successful response.

## Owners, permanent storage and lifetime

`Config` explicitly selects1..256 endpoint attempts,1..65536 transaction slots
per endpoint and canonical request/response maxima up to32768. Total history
storage is checked using the actual Rust `size_of::<Transaction>()` and capped
at64 MiB. All endpoint slots, fixed alias copies, three owner slots, journals,
original receipts, full UMRC160+URMI192+body scratch and all transaction history
are allocated before the first capability/native effect. Allocation refusal is12
while ordinary Rust can still execute. This is not a terminal trap/OOM immunity
claim. The caller must admit the real physical memory of the selected profile;
wire32768 is not object/stack/RSS or Native DDS memory.

A client needs explicit **distinct** service.call and event.observe aliases;
the observer policy must independently admit service-response and whichever
resource-error/closed controls the application requires. Availability, when
selected, is a third distinct service.available owner, never borrowed call rights.
A server needs distinct advertise and respond aliases. All selected aliases are
validated together before any open, with no default or synthesized alias.
This finite Group admits an alias only once in its lifetime, including retired
attempts; it does not recycle quota/history or silently reopen. A new Group still
needs fresh actual Host admission and does not reset Host/session quotas.

Group owns every raw/native resource. Client/Server are local views with private
full-source tokens; dropping a view does not secretly close or reopen it. Explicit
close or Group shutdown retires native once and closes query, peer, source in
reverse order. Every attempted result is separately recorded. Native retirement
failure never prevents any remaining raw close; no failed close is retried. A
failed activation without an admitted incarnation closes known raw owners and
relies on the Host's real owned-session retirement for any uncertain native state.
No bogus incarnation or handle is guessed for cleanup.

Group intentionally is `!Send`/`!Sync` and all business mutation requires `&mut
Group`. It is caller-driven and serial, not concurrent RMW or a native waitset.
The model seam is an explicit unsafe `Transport` contract, not a plugin or an
ambient backend. Safe Rust and the whole callback flag prevent this Group from
being mutated recursively during a callback; sibling groups are independent.
Imported calls copy their borrowed ranges synchronously and no native thread
invokes Rust callbacks or retains a Guest pointer. Application-level callbacks
are invoked on the caller only after a real fully decoded committed request.

## Submission, borrow, ACK and metadata

Before each business call, inspect must return the original full source,
Instance, Runtime generation, session, native incarnation, both actual concrete
QoS profiles and complete GID extent/value. Only the request opcode differs.
A present all-zero native GID is valid data; extent0 means absent. The client GID
is never substituted for the same-request writer GUID.

`Client::submit` validates/encodes the entire request before send, reserves a
nonreused history entry before the actual raw send and returns the actual
positive full64 Gateway ticket. It does not return a completed value. All send
errors are terminal for this finite endpoint; automatic retries are absent.
`poll` drives actual raw peek and can return idle `None`. Exact peek11 means no
visible borrow/ACK; the native provider may have fully validated/disposed an
already explicitly abandoned late response. It does not imply zero native take.

A successful peek is copied into preallocated private scratch. The parser checks
all fixed headers/reserved fields, full owner/peer/current incarnation,
strict stream sequence and independently increasing nonce, exact sizes and
complete URMI metadata. This profile requests native writer extent16. Different
selected native metadata widths remain unsupported95; no native struct layout or
missing field is guessed. Publication/reception sequence fields remain the
protocol's unsupported UINT64_MAX sentinels. Source/receive timestamps, signed
native request sequence and all16 writer GUID bytes are preserved from the SAME
take as the complete body, including legitimate zero/negative values.

The full typed decoder constructs an owned `Prepared<T>` **before** commit. Even
an abandoned response must fully decode. Only successful ACK publishes the value,
Ticket and ServiceInfo or explicit control. No post-ACK parsing, allocation or
fallible value preparation remains. The same rule applies to logical zero-byte
Empty: there is no artificial payload byte. Typed callbacks receive the complete
request only after ACK; response validation occurs before its once-only send.

Known complete borrow refusal attempts exact cancel under its actual sequence
and nonce, then terminally retires this finite endpoint. A malformed prefix is
not enough authority to invent cancel fields: it triggers retirement instead.
Any uncertain commit is terminal and is never cancelled/replayed as though it
were rolled back. All non-idle peek failures are conservative terminal failures.
The journal keeps primary, cancel, commit, native-retire and three raw-close
results separately. A cleanup error never overwrites the application error.

Per-request resource-error completes only its owned request, preserving other
pending requests. Resource-closed or explicit retired flag marks all remaining
local pending work terminal with125. The committed Control is returned before
any further import: its success tail performs only local fixed bookkeeping.
All later business calls refuse125 without another inspect/peek/send/query.
Genuine native retirement and reverse raw-owner closes remain pending until a
separate explicit `Client::close`, `Server::close`, Group `shutdown` or ordinary
Group destruction. A terminal view still owns that exact cleanup obligation;
local poison does not mean physical cleanup already ran. The caller must retain
and inspect its separate CloseReport/Diagnostic. A retire/close failure cannot
replace the already committed Control result, and a repeated shutdown does not
retry once-attempted cleanup. A trap/aborting process still needs actual Host
session drain. Unknown
or duplicate tickets and reused stream/nonce values are refused. Abandonment
keeps a full nonreused local tombstone and calls genuine client_abandon; it is not
native DDS cancellation or permission to repeat the original request. A valid
already queued late response is fully decoded and ACKed but never delivered;
the local state remains Abandoned, with a separate consumed tombstone that
rejects a second duplicate rather than publishing completion.

## Availability and finite drivers

`Client::available(wait_ns)` requires the selected independent third owner and
inspects this actual retained client first. It calls the real raw query with the
caller-selected0..60s bound and a private0xa5 sentinel byte. Success must write
exactly0 or1; unwritten/other bytes refuse22. Errors are never returned as a
successful false result. Actual wait0 is one observation. Positive waits depend
on actual Host/native support and that alias's explicit budget; returned95 stays95.
This layer does no graph-list inference, new probe-client creation, sleep,
step-count-to-nanosecond conversion or automatic query retry. There is no implicit
Clock grant. Use a genuinely admitted external driver for any application-level
finite/infinite wait; this revision supplies no fake blocking Future/get method.

`poll`, `submit` and `respond` are the actual delayed Service API; clients can
store Ticket and drive poll explicitly. `state` reports pending/completed/error
or abandoned local state while live. Shutdown makes remaining views return125.
The two lessons use a finite **work** iteration ceiling and return90 on work
exhaustion; they never call it a physical timeout. No executor, std sleep or
synchronous operation is disguised as async completion.

## Future build, artifact and real-host recipes

No command here has run. Freeze a coherent private tree containing the actual
records12 source revision, parent generator, catalog/definitions, generated record
output, this crate and unchanged `sdk/rust`. Acquire/retain the real existing
Rust compiler target, WASI sysroot/libc archive and every helper. Do not install a
compiler, fetch dependencies or write global caches as part of a test run.
Use `cargo --offline -j1`, a fresh caller-owned `/tmp` target dir and the installed
`wasm32-wasip1` target. The existing SDK Rust recipe binds actual `libc.a` through
separate encoded linker args; that archive is required for memory intrinsics,
not a reason to admit `env.memcmp` or host `-lc`. See the real
`sdk/examples/rust/README.md` for the selected producer's exact archive route.

Examples are explicitly listed in Cargo.toml; helper bindings are not an
independent executable. `control_client` argv is:

```text
control_client CALL_ALIAS OBSERVER_ALIAS AVAILABILITY_ALIAS_OR_DASH TYPE_NAME ACTUAL_TYPE_HASH WORK_STEPS
```

`control_server` argv is `ADVERTISE_ALIAS RESPOND_ALIAS WORK_STEPS`. Each spelling
is a real independently configured grant; `-` explicitly omits availability.
The client sends include_type_sources=true and receives the complete default GTD
response. Its output count is diagnostic DATA, not a Native witness. The server
handles real SetParametersAtomically requests and deliberately returns an
application denial as a complete typed result; it does not pretend to implement
a native parameter store or full unchanged default Node.

Run a genuine full Wasm validator separately. Then the existing source tool
`sdk/tools/check_rmw_service_imports.py ACTUAL.wasm --allow-module wasi_snapshot_preview1`
checks exact selected optional service/business signatures, backend and duplicate
imports. Only allow a non-ROS module actually present in the selected producer.
Use `--require-all` only with an actual all12 coverage artifact; linker dead
stripping means an individual client/server lesson need not import every method.
The checker is not a validator or execution/native proof. Also inspect actual
memory count/index/limits/layout and exported entry/signature. Never admit an
unknown optional namespace using an undifferentiated module allowlist.

For real positive tests, use actual selected Native SDK/generated support and the
existing Stage2 Native control peer/exporter, Stage3 Gateway profile and actual
full-control Host selection (`gateways/ros2/README.default_control_profile.md`).
Require real frozen source/schema/native identities and full original QoS/codec
proof before Node creation. Actual Worker UID/PID, Gateway UID, socket, native
node/namespace/service names and concrete quotas must come from the owned run.
Client call+observer mask/body/source pairing, server advertise+respond,
max-request/max-response bytes, queues/inflight/bytes/rates and history capacities
are independently explicit. Query needs its own availability budget. No numeric
IDs or successful local model can stand in for this admission.

Keep separate results for model contracts, actual import/artifact gates,
real-Host default-deny13, real explicit-pair lifecycle, all seven Service native
interop, GTD true complete12-source/raw content, actual same-take metadata,
controls/abandonment/no-replay and all cleanup/resource measures. Do not mark any
as PASS from source or a marker. Full physical/Native metadata qualification and
producer compilation remain pending.

## Authored local contracts

`src/contracts.rs` contains21 unexecuted meaningful local-model groups. They use
an intentionally unregistrable model Definition and private test seam, with no
invented RIHS/canonical hash or replacement SDK. They exercise partial-factory
rollback, aliases, same receipt, full64 delayed tickets, decode-before-ACK,
malformed abandoned response, uncertain commit no replay, independent three-way
cleanup failure, query sentinel/error/false, actual output statuses, finite
history, one response, callback error, per-request control isolation,
source close and exact wire/zero-body validation. A model is not Native evidence.

## Reviewed post-ACK cleanup correction

This source successor preserves the first package and its independent complete
review. It separates local terminal rejection from physical once-cleanup only
for a genuinely ACKed retired Control. The original source-closed model is
truthfully revised to expect zero retire/close calls when poll returns, followed
by one cleanup on explicit close. Twenty other original model functions stay
byte-identical; two additional local models exercise a client with three
independent cleanup failures and a server with an already delivered request.
All23 model groups are authored source, never executed. The separately reviewed
managed-record operator successor and actual source pairing must be included in
any future private build; old retained records source observations are historical
and are not relabelled as generated products or current canonical proof.
