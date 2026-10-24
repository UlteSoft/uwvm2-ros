# C contract fixtures

This suite is physically located at
`test/0073.ros-tests/support/source/sdk-tests/c/`; the old SDK test trees are removed.
Its owned-file/provenance request is in task02's
TEST-MIGRATION-MAP.json. No test is an installed SDK consumer dependency.
Drivers default their fixture root to their own directory and accept
--test-sources, so the move never requires restoring sdk/test paths.

`portable_headers.c`, `portable_contract.c` and `default_denial.c` cover the
shared C89/C90/C95/C99/C11/C17/C23 and supported draft modes. The serial
`standard_matrix.py` scheduler requires the common Linux finite owner and
records compiler mode/predefines, strict header syntax, native local behavior,
target library/archive builds, expected negative linking, complete38/43 binary
import signatures, full Wasm validation and actual Runtime execution separately.
Every unsupported/refused/unrun row remains visible. It grants no ROS alias.

`generated_portability.py` exercises the known C export transform without
publishing a global SDK or editing vendor maintenance products. Its adversarial
fixture keeps a member named `uint64_t` and a String containing C type/keyword
spellings, then checks complete i64/array/Float64, rounded Float32 and negative
zero wire values. Hex-float defaults become width-verified decimal C90 constants.
Its original C11
and adapted C modes are local codec evidence, not native ROS or official tests.
`qualify_generated_documentation.py` checks all 235 pinned captured headers for
the current documentation grammar, noncomment-token preservation and repeated
byte equality. This declaration check is separate from compiler/wire acceptance
and from source-backed physical field review. Its current execution status is
recorded in Task02's handoff, never inferred from the driver's existence.

`policy_tool_contract.py` prepares explicit policy JSON/configuration in source,
installed and split layouts and checks early refusal of missing/unsupported
inputs. It executes no emitted policy-check command, Gateway, ROS process or
physical action. The installed-layout test copies the exact shared serializer;
its result does not prove that the global producer exports that dependency.

`xmake_contract.lua` checks the real C language adapter's mode/argv handling;
`run_lua_contract.py` records its actual xmake VM/version, exact input hashes
and the nonempty 23-check denominator in a finite owner. Its latest execution
status is kept in the handoff; it gives no genuine Lua formatter credit.
it provides no C or Guest execution credit by itself. Public xmake entry-point
integration, matched library closures and global numbering belong to their
designated owners. The older model fixtures below retain their C11 scope.

`wrapper_contract.c` is a prepared, pure import-model test. It checks explicit
alias denial, eight-byte handles, little-endian codec values, transactional
decoding, both written and unwritten error90 length slots, empty queues, guest
callbacks, and close retirement after a transport error. It does not connect to
ROS, open sockets, execute a Worker, or prove ARM behavior.

Build with a C11 compiler and `-I sdk/origin/include`; execute inside the authorized
Linux cgroup before recording a successful build/run. The fixture uses only
`mock_backend.h`. Actual interpreter/native ROS acceptance remains a separate
required test, including default-deny and native side-effect witnesses.

`golden_vectors` covers all57 independently specified positive values and11
negative wires from `sdk/origin/abi/wire-vectors-v1.json`. Expected bytes are embedded
from the authoritative fixture and its SHA256 is recorded in the source comment.
The fixtures are prepared; no execution result is claimed here.

`legacy_length_contract.c` specifically models an error90 import that leaves the
length slot untouched for both legacy C poll helpers. The caller's length must
remain unchanged. It uses native mock definitions, not actual ROS imports.

## Ownership and scheduling fixtures

`resource_lifetimes.c` checks38 source-declared failure/cleanup contracts over a
pure fake import backend. `scheduler_contract.c` checks43 local Future queue,
capacity/cancel/reentrancy/group/explicit-time/controlled-shutdown contracts.
Compile as C11 with `-I sdk/origin/include` and run as one serial native process. The
root test coordinator records actual results in `/tmp`; these models do not
start or prove native ROS, Wasm Guest, QEMU or message delivery behavior.

`action_bundle_contract.c` contains68 declared pure-import contract checks for
explicit four/five-role preflight, dotted-alias rejection, bounded1..1024 storage,
all-owner rollback, goal/canonical provenance, written/unwritten90 output lengths,
foreign-ID/optional-stronger selectors, reverse cleanup/revoke and local IDs.
It uses actual SDK functions over a test-owned backend; root must execute it
serially. It does not prove native Action/QoS or automatic ROS authority.
