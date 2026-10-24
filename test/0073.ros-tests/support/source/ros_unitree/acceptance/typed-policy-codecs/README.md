# Explicit custom Topic codec identity in acceptance policies

The maintained Typed28 and controlled Direct physics policies must supply the
same explicit `adapter-codec-identity` that the production Worker and Gateway
require. The former positive policies omitted that required field and would
fail admission. Source43 and all staged historical controls retain their
original bytes; their existing preparation receipts do not establish positive
transport acceptance.

`typed_transport_inputs.generation_codecs()` checks the complete generated
record and root type sets against the already admitted native identity map.
It rejects missing, duplicate, foreign, malformed or mismatched rows and binds
the exact codec map to the actual compiled Worker registry. The bounded-v2
generator uses its `UWVMROS1_` canonical codec format prefix for both native
ROS versions. That prefix names the codec format and supplies no capability.

Each actual positive grant now carries the exact admitted type's codec string.
The Direct controller additionally checks its actual record's native identity
and codec against that admitted map before choosing its existing bounded byte
capacity. Default-denied policies still emit an empty grant list.

The seven tests in `test_typed_transport_policy.py` use explicit synthetic
metadata. They cover complete codec correspondence, malformed or conflicting
inventories, each GO/HG/API positive policy, both Direct aliases, and empty
denial policies. They create no compiler, Guest, Gateway, native SDK, network
or namespace. Root must run them as part of the separately admitted Linux core
regression batch after the complete new source freeze. Passing them does not
replace actual native exporter/full-MD admission,28 real Typed lifecycles or
controlled ROS/MuJoCo execution.

The original source bodies are preserved beside this note. The independent
`r2` source peer binds the final three changed files. Neither source readiness
nor its hashes are an executed-test receipt.
