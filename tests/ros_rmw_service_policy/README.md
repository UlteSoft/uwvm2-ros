# ROS2 RMW service policy and canonical-body admission

This source cohort connects the exact ROS2 native names `std_srvs/srv/Empty`
and `example_interfaces/srv/AddTwoInts` to the actual Host policy and Gateway
policy validator. It adds no default grant. Users still explicitly select the
backend, original native type identity, resource, each operation, all QoS and
finite quotas. A client, its response observer, a server and its responder, and
an availability capability remain separate capabilities. ROS1 names and hashes
are not inferred from these ROS2 interfaces.

The complete published service hashes are copied from the original generated
Jazzy `std_srvs` and `example_interfaces` type-description JSON, preserved in
this cohort's source evidence. A known policy identity is only a selection
constraint. The native factory must independently obtain the actual selected
SDK support and prove the full service/request/response graph, native identity
and canonical codec. Missing metadata APIs or a different native graph fail
closed; these constants do not make an older/newer distribution qualified.

Empty's canonical request and response contain zero bytes. The generated C/C++
placeholder belongs to the actual native object and must be validated by that
native codec; it must never appear in canonical payloads. AddTwoInts uses two
little-endian signed-i64 fields (`a`, `b`) in its 16-byte request and one signed
field (`sum`) in its 8-byte response. This policy validates extent without
reinterpreting unaligned bytes, doing signed arithmetic, or imposing a
floating-point range. All signed-i64 bit patterns are valid. Overflow behavior
of a particular application callback remains that application's obligation.

`capability_manager::authorize_rmw_service_envelope` is a trusted Host-only
entry. It validates the genuine current server capability, positive 8-byte
request ticket and complete canonical request/response using the production
validator. It keeps the actual capability lease across the admitted operation.
Only the canonical body is charged to business byte credit; the ticket and
UMRC/URMI control metadata are excluded. It supports only ROS2
`service.advertise` and `service.respond`. Client submissions still use their
ordinary request admission. Response observers still use their independently
granted logical UWRE record credit. The original `authorize` entry retains its
original whole-envelope accounting, so existing imports are unchanged.

A Host may temporarily form a ticket/body view in its own reserved scratch
space. It must restore all reserved wire bytes before RPC serialization or a
Guest write, must validate actual native ownership/metadata separately, and
must cancel or retire a retained borrow when policy admission fails. No ticket
creates native authority. A quota failure must never acknowledge a delivery.
Cleanup and ACK/control frames do not become new service requests.

Nine authored groups exercise the real production policy and manager: exact
published identities/ROS2 names; all four service operation schemas through
both policy validators; zero-byte Empty; complete signed-i64 extents/quotas;
old Trigger/SetBool behavior; body-only versus original whole-envelope
accounting; positive-ticket requirements; owner/version/epoch/revoke failures;
and default-denied instance registration. These sources have not been compiled
or executed. The executable does not fake a native RMW or count as a native,
SDK, upstream rcl, vendor, distribution, architecture or performance result.

After implementation and source composition are complete, the bounded Linux
qualification owner selects this exact full source tree and a real compiler:

```sh
cmake -S tests/ros_rmw_service_policy -B /tmp/uwvm-service-policy-build \
  -DUWVM_BUILD_RMW_SERVICE_POLICY_CONTRACT=ON -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/uwvm-service-policy-build -j1
/tmp/uwvm-service-policy-build/uwvm_rmw_service_policy_contract
```

All actual Linux work must occur inside the assigned 64 GiB/swap-zero cgroup
with physical disk headroom and process-retirement accounting. Local build or
test products belong under `/tmp`. These are future instructions, not executed
commands. Native client/server, Guest default-denial, exact transport effects,
independent cleanup ledgers and end-to-end original-rcl tests remain mandatory
separate qualifications. An explicit custom codec identity is never silently
dropped to select a builtin native converter.

The separate `uwvm_rmw_service_max_wait_contract` is an authored regression for
availability admission. `max-wait-ns` is parsed using its actual protocol ceiling
of 60,000,000,000 ns; all other generic `limit` keys retain their original 1 GiB
ceiling. Two seconds and 59.999 seconds with genuine timeout headroom must pass
both actual policy validators; a full60-second wait without remaining timeout
headroom, values beyond the protocol ceiling and malformed integers must fail.
This only corrects grant parsing. It does not implement a positive graph wait,
create a native client, enlarge a byte quota or qualify an actual availability
result. The new executable is built by the same explicit opt-in and is not run
automatically. No regression source in this cohort has been executed.
