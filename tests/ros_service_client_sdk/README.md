# Optional C/C++ service availability source contracts

This cohort adds an explicit C/C++ convenience layer and finite rclcpp source
adapter integration over the **already separately defined** optional
`uwvm_ros1_client_v1` / `uwvm_ros2_client_v1` catalog. It does not change the
original business ABI-v1 catalog, its81 tuples, its namespace versions, native
service request bytes, queue semantics, or existing imported backend structs.
It has not been compiled, tested, generated, imported, run in Wasm, sent over
SSH or applied by this author. Existing local Java/Python/C++ results remain
attached to their original artifacts; none qualifies this new query path.

## What the SDK actually does

Include `roswasm/service_client_ros1.h` or `service_client_ros2.h` explicitly and
select the corresponding immutable optional table. Merely declaring both raw
namespaces in a header is not an import invocation or grant. An unused selector
must not create a runtime dependency; actual compiled import inspection remains
required. A selected business backend table can still reference its original
38/43 functions; this extension adds at most the selected optional three tuples,
not permission to invoke all declared operations.

The existing actual `service_client_v1.h` is the raw contract, not a new copy:

* `abi_version()` returns the extension version1.
* `features(available, source, memory, out8)` returns errno and writes exactly8
  LE feature bits only on success.
* `service_available(available, source, wait_ns, memory, out1)` returns errno and
  writes exactly one canonical bool byte0/1 only on success.

There is no output-length slot. Both full64-bit values are actual independently
owned handles: `service.available` plus its exact live `service.call` source.
They are not alias addresses, guessed IDs, native pointers or a debugger token.
The Host independently checks operation, configured source/resource/type/native
identity/codec/all QoS, real instance/generation and authenticated Gateway session
and both leases. SDK-local pointer checks supplement that boundary; they do not
replace it. Default policy remains empty. A source declaration cannot grant a
query, business call, graph listing, management or update/debug operation.

The C wrappers use private stack output slots initialized to invalid sentinels.
They commit caller outputs only after actual success and canonical validation.
Unknown feature bits, incomplete output, bool2/255, invalid memory/local owner
association, unsupported/missing tables or any nonzero errno preserve output.
Version1 accepts only its explicit full ROS1 mask11 / ROS2 mask7 or actual0;
future masks need a reviewed versioned change. No error is turned into false.
Codes11,13,95,110,125 remain busy/quota, denied, unsupported, timeout and revoked
respectively. A zero wait performs a real query; finite waits are limited to
60,000,000,000ns and any smaller actual Host grant. True is a snapshot, never a
promise that a later business request will succeed. The SDK neither submits an
empty request nor retries a business operation after a positive observation.

## C++ ownership and the existing source adapter

`roswasmcpp::ServiceAvailability` owns only its separate query capability and
borrows the already-open call owner/Context. Their storage and the immutable
tables must outlive it. Its constructor checks the actual optional ABI before
opening the query alias, and it remembers the exact full64 source handle. Closing
or replacing that borrowed source causes later methods to refuse9 before import;
it never silently pairs to a reopened owner. The Host still independently admits
the actual native pairing. The wrapper is noncopyable/nonmovable. Explicit close
and revoke retain their separate cleanup result and retire the local query owner
once even after107/110; they never retire the borrowed source. Destruction is
best-effort only. Save checked cleanup while the object is alive; no destructor
claim replaces actual Runtime instance drain after traps or unknown completion.

The finite rclcpp adapter gains two **trailing optional** fields:
`ServiceBinding.availability_alias` and `ServiceProfile.clients`. Existing
aggregate tables keep empty/null defaults. With those defaults,
`service_is_ready()` and finite `wait_for_service` still refuse95, without
opening a query alias or using Graph, clock ticks or a request as readiness.
Server bindings reject a nonempty query alias. A selected client validates its
table and distinct call/observer/query aliases before any resource opens.

A new client acquires call source, independent event observer/watch and finally
the optional query owner. Construction failure rolls them back in reverse order;
the original primary failure survives separate cleanup failures. Normal close
retires query first, observer second, source last. The original two cleanup slots
stay observer/source; `availability_cleanup_status()` is separate while the
aggregate report counts every actual attempt. No failed close is replayed.

Queries run under the existing real adapter ServiceLease, which retains the whole
current facade/Core group and refuses nested query, factory, teardown or shutdown
with17 during a local transport callback. This is the finite source adapter's
actual lifecycle mechanism, not a claim that arbitrary original rclcpp/RMW
callbacks have an externally imposed lease. Event/Future completion is unchanged:
query neither consumes an observer record nor completes/submits a request.

`wait_for_service(duration)` rejects negative, NaN, infinite and values above60s
before import. Finite fractional nanoseconds use duration_cast-style truncation;
sub-nanosecond positive durations therefore request a real zero-wait observation.
It sends one genuine bounded native query, not a hidden spin or synthetic sleep.
The no-argument infinite overload remains explicit95 until a real shutdown/waitset
contract exists. Existing ERROR logging and broader native compatibility remain
their own pending profiles; this cohort does not quietly turn an INFO alias into
another severity or modify an official application's source.

## Meaningful authored contracts, all unexecuted

`raw_signature_contract.c` has six C11 exact signature assertions over the actual
raw header and one integer-width assertion. It calls no raw import.
`foundation_contract.cc` has15 real SDK-wrapper host-model groups: full-u64 pairs,
false/finite true/max bound, all failure/output preservation, canonical bool and
missing writes, exact contexts/backends, optional absence/version, closed feature
masks for both versions, null outputs, checked C++ ownership, ABI-before-open,
denied independent owner, source reopen refusal, cleanup110/no replay, and stack
retirement. The model transport is explicitly local and provides no native proof.

`rclcpp_contract.cc` has14 finite adapter groups: legacy95, genuine injected query
false/true, high handles, invalid/maximum/fractional durations, infinite95,
unchanged errno, invalid bool, three-owner construction rollback, all cleanup
statuses, whole-group reentrancy, pending event/request independence, retired
source refusal, and last-public-reference retention through the query. Its
selection fixture preserves the complete existing pure model transport prefix
and adds only a separate explicitly injected query lane. No model claims an
underlying ROS server, actual elapsed wait or native participant.

Root may later compile under its separately owned local limits using actual
admitted tools/current source. Example commands, **not executed here**:

```sh
<cc> -std=c11 -I<source>/sdk/include \
  <source>/tests/ros_service_client_sdk/raw_signature_contract.c -o <fresh>/raw
<cxx> -std=c++20 -I<source>/sdk/include \
  <source>/tests/ros_service_client_sdk/foundation_contract.cc -o <fresh>/foundation
<cxx> -std=c++20 -I<source>/sdk/compat/rclcpp/include \
  -I<source>/sdk/include -I<actual-fast_io-include> \
  -I<source>/tests/ros_service_client_sdk \
  -DUWVM_RCLCPP_COMPAT_SELECTION_HEADER='"rclcpp_model_binding.hpp"' \
  <source>/tests/ros_service_client_sdk/rclcpp_contract.cc -o <fresh>/adapter
```

Use fresh outputs, complete admitted include/dependency files, serial commands,
the actual fast_io release used by the current compatibility header, finite RSS
and deadlines, raw logs and terminal child cleanup. Original old26 service and10
topic groups should then be rerun as regressions. Those future runs cannot be
claimed from this source review, and host models are not Wasm/native ROS tests.

## Future real route and explicit remaining scope

The new metadata-only C/C++ examples use actual explicit backend tables and two
illustrative alias literals; they submit no business request. False intentionally
returns1 after successful cleanup, distinct from errno. The rclcpp selected-header
example can be supplied to the already acquired unchanged official
minimal_client/main.cpp. Its native query may now support the client's finite
wait loop, but no build/run/whole-official-client claim is made. The original
program's source/license/provenance remains untouched in its existing package.

Real execution requires a new coherent source/Runtime/Gateway generation with the
actual optional namespace imports, typed native service/exporter/QoS evidence,
explicit paired grants, feature negotiation, independent owned process/deadline
and terminal cleanup. The native availability cohort's genuine server witness
must prove no handler request during false/true/removed metadata observations,
then a separate authorized business positive control. Do not reuse retired
Runtime60–63 products or promote earlier local Java/Python/native records into
new extension proof. Current raw/native implementations still own their actual
limitations, including ROS1 numeric-only/localhost URI admission and unsupported
unbounded DNS/ROS_HOSTNAME resolution. This SDK cannot erase those limitations.
