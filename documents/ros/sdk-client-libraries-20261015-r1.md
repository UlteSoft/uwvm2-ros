# Six-language WasmROS client-library delivery

The C11 `roswasmc`, C++20 `roswasmcpp`, Rust, JavaScript, AssemblyScript and
TinyGo libraries now provide the complete client families supported by the
current ROS1/ROS2 Runtime ABI. Their public layers and progressive applications
cover exact imports, canonical codecs, checked ownership, Node/Session,
publish/subscribe, service and Action clients/servers, parameters/events,
graph/TF/clock/logger/lifecycle and local Context/Future/Timer/CallbackGroup.
All new guides, headers and examples contain English usage and lifetime
documentation. The [SDK entry point](../../sdk/README.md) and
[coverage matrix](../../sdk/docs/client-library-coverage.md) explain the actual
APIs and backend differences.

This is completion of the supported ABI client-library work. It is not native
rclcpp/rclc/rclpy source or binary compatibility, arbitrary dynamic IDL/CDR or
custom native Service/Action support. Native ROS communication and current-SDK
Linux/aarch64/QEMU acceptance remain unexecuted after the user closed SSH.
The original full Runtime task remains incomplete.

## Actual serial verification

All executed builds, models, caches, temporary sources and raw logs are below
`/tmp`. Compilers ran sequentially with one build job. The command process-group
RSS limit was 512 MiB; the highest sampled aggregate RSS was **367.406 MiB**.
This is a sampled local observation, not an instantaneous bound or performance
benchmark. No SSH, native ROS launch or parallel compilation was performed.

| Language / target | Real Wasm products passing import inspection and full validation |
| --- | ---: |
| C11 | 131 |
| C++20 | 131 |
| Rust | 20 |
| TinyGo | 20 |
| AssemblyScript | 28 |
| JavaScript C bridge and command relay | 4 |
| Total | **334** |

Both ROS backends are selected explicitly. The complete raw declarations match
the registered 34 ROS1 and 39 ROS2 functions. C/C++ includes every tutorial
selector and all twenty Action-bundle case/cancel-any/revoke combinations per
backend/language. Five handwritten fixed-record Wasm examples also compile;
the sixth JavaScript example executes its actual local roundtrip. JavaScript
is a supplied-bridge client rather than a JS engine inside uwvm.

| Executed check | Required and observed result |
| --- | --- |
| Source and relative link checks | Complete source verifier passed; all shipped JS entries parsed |
| CMake relocation / installed headers | Configure, install, move prefix, configure consumer; six C and eight C++ fixtures passed from the installed tree |
| New C/C++ lifetime contracts | C Action bundle 68 checks, C++ Action bundle 35 checks, intrinsic callback guards 33 checks |
| Native Rust | Exactly 17 tests passed; zero ignored or filtered tests |
| Native Go common logic | Exactly 18 top-level tests and 57 positive-vector subtests passed |
| JS client API / stream protocol | 54 named API cases and 824 protocol checks passed |
| Pure compiled AssemblyScript | Nine import-free Wasm fixtures executed; eight numeric fixtures completed 413 assertions |
| Compiled C command relay | 80 cases executed against the real ROS1/ROS2 relay Wasm products and bounded mathematical imports |
| Compiled local scheduling lessons | Ten real Wasm artifacts executed in twelve cases; caller-driven Futures/Timers completed |
| Fixed-record generator/codecs | Fifteen source models and 77 JS checks passed; generated-r3 bytes reproduced without inventing native identities |
| Explicit policy format/validators | Earlier unchanged SDK policy code passed 75 positive CLI profiles, 13 version refusals and actual Worker/Gateway parser checks including Action channel fields |

The final [compiler/source/model report](/tmp/uwvm-sdk-local-full-clients-20261015-r2/report.json)
records 578 successful current commands and the complete 334-product table.
The [installed C/C++ report](/tmp/uwvm-sdk-local-c-cpp-installed-20261015-r1/report.json),
[Go retry report](/tmp/uwvm-sdk-local-go-retry-20261015-r1/report.json) and
[current scheduling report](/tmp/uwvm-sdk-local-current-scheduling-20261015-r1/report.json)
retain their separate actual execution scopes. Native Rust's successful command
and log remain in the interrupted run; its source inputs match the current SDK.

## Repairs included in the delivered source

The grouped C/C++ Action client acquires four independent send/cancel-own/
feedback/result aliases, with an optional fifth explicit cancel-any role.
It validates every alias/role before acquisition, reports reverse rollback and
all cleanup statuses, bounds caller goal storage to 1..1024 slots, binds local
goal IDs and retires them only after valid terminal reception. Stronger nonzero
and all-goal cancellation never follows implicitly from send or cancel-own.

C++ callbacks retain their actual stateful callable instance while permitting
safe handler reassignment. Direct recursive poll, callback-time close/revoke
and exception paths are tested. JS validates feedback/result goal IDs and shares
polling/retirement leases across resources, their public Capability objects and
Node/Context cleanup. Rust, TinyGo and AssemblyScript share owner guards with
group cleanup; AssemblyScript also protects direct subscription and public
Capability polling. Refused group cleanup preserves every owner for a later
checked close. Exact-ID responses inside the handler remain valid.

Action/parameter lessons cancel before terminal-result reception instead of
using a retired goal. Native backend outcomes remain visible, including ROS1
terminal cancellation conflict 17. Handwritten custom examples preserve primary
and cleanup errors; TinyGo and AssemblyScript expose inspectable status values.

## Interrupted-run history and product reuse

The first final run retained 327 command records before an `ENOSPC` error
prevented saving more progress. Its source-link failure and empty-log Go
failure remain in the [interrupted receipt](/tmp/uwvm-sdk-local-full-clients-20261015-r1/interrupted-receipt.json).
The missing native-peer document and repository-root include recognition were
repaired, then the complete source check passed. Go independently passed after
space recovery. No earlier failure was rewritten as a pass.

Old task-owned local source snapshots were removed only after a complete archive
and every member byte had been independently rechecked. The
[archive receipt](/tmp/uwvm-sdk-space-recovery-20261015-r2/receipt.json)
preserves 9,397 file bindings and the archive hash. Source, raw logs, actual
Wasm/native executables and historical native acceptance evidence were kept.
Rebuildable Go/Rust incremental caches were cleaned separately.

The final run reused **139** retained successful compiler products and issued
**195** fresh compiler commands. All **334** products received current binary
signature inspection and a fresh full validator command. Resume admission
requires exact frozen SDK/body/dependency equality except the two administrative
checker scripts, exact command/path/log pins and bounded regular-file copies.
Prior artifact/tool/environment fingerprints were not recorded by the old
progress format; product hashes are first available at current admission.
External compiler packages, target libraries and dependency closure remain
unqualified. These limitations are explicit in the new receipt; the old run is
never promoted to an overall pass. Progress and row results now save atomically.

## Authority and remaining native acceptance

The SDK creates no ROS grant, backend enablement, native identity or connection
from a Node/Session/Context label. Default ROS authority is empty. Timers use
explicit caller time/readiness; deferred RPC remains a bounded synchronous
import during a later explicit pass. There is no automatic retry, effect replay,
timeout rollback, native cancellation or stronger permission acquisition.
Compiler startup imports require separately reviewed WASIp1 permissions and
cannot increase the default policy.

The mathematical relay and local models do not establish authenticated Runtime
admission or native DDS/TCPROS delivery. Guest-only custom codecs do not create
a native type registry. No local receipt substitutes for current Linux cgroup,
QEMU target mapping, native interoperability, toolchain closure or the original
Runtime acceptance matrix.

The [machine-readable delivery summary](sdk-client-libraries-20261015-r1.json)
pins all reports, reconciles tested C/C++/Rust/Go/policy inputs with current bytes,
records the 334 products, actual model counts, memory controls and every scope
boundary. Its SHA-256 is
`d9afaa78704335797393ca8598a8718030dd58a2e0e5f7a20361c88f1933f877`.
