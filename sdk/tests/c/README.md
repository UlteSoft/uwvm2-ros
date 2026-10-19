# C contract fixtures

`wrapper_contract.c` is a prepared, pure import-model test. It checks explicit
alias denial, eight-byte handles, little-endian codec values, transactional
decoding, both written and unwritten error90 length slots, empty queues, guest
callbacks, and close retirement after a transport error. It does not connect to
ROS, open sockets, execute a Worker, or prove ARM behavior.

Build with a C11 compiler and `-I sdk/include`; execute inside the authorized
Linux cgroup before recording a successful build/run. The fixture uses only
`mock_backend.h`. Actual interpreter/native ROS acceptance remains a separate
required test, including default-deny and native side-effect witnesses.

`golden_vectors` covers all57 independently specified positive values and11
negative wires from `sdk/abi/wire-vectors-v1.json`. Expected bytes are embedded
from the authoritative fixture and its SHA256 is recorded in the source comment.
The fixtures are prepared; no execution result is claimed here.

`legacy_length_contract.c` specifically models an error90 import that leaves the
length slot untouched for both legacy C poll helpers. The caller's length must
remain unchanged. It uses native mock definitions, not actual ROS imports.

## Ownership and scheduling fixtures

`resource_lifetimes.c` checks38 source-declared failure/cleanup contracts over a
pure fake import backend. `scheduler_contract.c` checks43 local Future queue,
capacity/cancel/reentrancy/group/explicit-time/controlled-shutdown contracts.
Compile as C11 with `-I sdk/include` and run as one serial native process. The
root test coordinator records actual results in `/tmp`; these models do not
start or prove native ROS, Wasm Guest, QEMU or message delivery behavior.

`action_bundle_contract.c` contains68 declared pure-import contract checks for
explicit four/five-role preflight, dotted-alias rejection, bounded1..1024 storage,
all-owner rollback, goal/canonical provenance, written/unwritten90 output lengths,
foreign-ID/optional-stronger selectors, reverse cleanup/revoke and local IDs.
It uses actual SDK functions over a test-owned backend; root must execute it
serially. It does not prove native Action/QoS or automatic ROS authority.
