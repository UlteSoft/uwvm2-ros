# C++ contract fixtures

`wrapper_contract.cpp` is a prepared C++20 pure import-model test. It covers Node
construction without publication, denied aliases, typed String publication,
explicit callback dispatch, idle queues, removal of borrowed registrations,
transactional decoding, signed array boundaries, move ownership and cleanup
retirement. It uses the C model backend and never opens ROS resources.

Build with C++20 and `-I sdk/origin/include`; compile/run in the authorized Linux
cgroup before reporting an execution result. This model is not a substitute for
actual Worker/native ROS and aarch64/QEMU tests.

`golden_vectors` covers all57 independently specified positive values and11
negative wires from `sdk/origin/abi/wire-vectors-v1.json`. Expected bytes are embedded
from the authoritative fixture and its SHA256 is recorded in the source comment.
The fixtures are prepared; no execution result is claimed here.

`legacy_length_contract.cpp` models unwritten error90 lengths for both hosted
C++20 and freestanding C++11 owner methods, on both ROS backends.
`c_header_compatibility.cpp` prepares C++ inclusion of the C freestanding layer.

## Ownership and scheduling fixtures

`resource_lifetimes.cpp` checks56 declared model contracts, including every typed
facade's checked lifetime. `scheduler_contract.cpp` checks41 local scheduling
contracts: deferred execution once, cancellation, bounded queues, callback
borrow/reentrancy/group, synchronous timeout preservation, explicit-time Timer
and controlled shutdown. Compile as one C++20 native translation unit and run
serially with outputs in `/tmp`. Callback exceptions retain native C++ policy;
Wasm examples use a nonthrowing/no-exception profile. These pure fake-import
models are not native ROS/Wasm/QEMU delivery acceptance.

`poll_reentrancy.cpp` adds33 exception-enabled host checks (30 without exceptions)
of direct intrinsic leases, mutable callback state, safe reassignment, nested17,
closed9, explicit server-pair revoke and no extra fake imports. The earlier root
26-check snapshot is a distinct retained result; this expanded source requires
its own run. `action_bundle_contract.cpp` adds35 typed aggregate client contracts,
including local deferred send, unknown-ID refusal, result retirement, explicit
stronger nonzero/all selectors, reverse close/revoke and transactional output.
Neither fixture establishes native action/resource/QoS behavior.
