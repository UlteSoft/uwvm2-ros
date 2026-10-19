# SDK compatibility and actual validation, 2026-10-18

Full existing rclc/rclcpp/rclpy/rcljava product compatibility remains required.
The implemented migration profiles below cover concrete original application
paths. Compatibility means recompiling C/C++ rcl/rclc/rclcpp source to Wasm;
rclpy executes inside the actual CPython.wasm bridge, and Java/JAR passes through
a real bytecode/IR-to-Wasm producer. All resulting business calls enter the same
uwvm2 ROS imports and explicit permissions. Guest code receives no native ROS
binary ABI, host pointer, JNI/JVM shortcut or update/debug/policy manager import.
Native Gateway libraries are the controlled network boundary and independent
behavior oracle. These profiles do not establish complete upstream API,
distribution, vendor or native middleware compatibility.

## Integrated implementations and actual local checks

| Surface | Actual evidence | Qualification boundary |
| --- | --- | --- |
| C/rclc migration | Earlier two byte-exact official String examples compiled; ten local groups/391 checks passed and both earlier WASI modules validated with39 ROS2 imports. Additive three-family service source130 is independently reviewed and integrated;27 new groups remain unexecuted | Earlier producer identities remain historical; current combined successor, original service examples, actual availability/headers and native delivery still require testing |
| C++/rclcpp migration | Both byte-exact official topic examples and the original service server compiled; ten topic scenarios and 26 service/Future models passed; real fast_io bounded logger compiled | Typed async submit/observer/correlation and callback-lifetime profile; complete native availability/wait sets and remaining client APIs required |
| Python/rclpy foundation | 75 local ownership/wire/lifecycle/service/Future models passed, including the unchanged original service main through its explicit bootstrap | Latest additive service successor; local backend results, not native delivery or a rebuild of the older Python Wasm artifacts |
| Java foundation | Real TeaVM annotations compiled; 63 JVM ownership/codec assertions passed | Local logic is separate from TeaVM/WASI execution |
| rcljava migration | Twelve byte-exact official topic/composition/timer/service/client classes compiled; local policy 61 checks, original-source model 11 checks, 28 service groups/70 checks and the entire original service-server main model passed | New Service/Future source successor; historical original client only compiled and refused availability95; the new explicit availability13 source is integrated and unexecuted. Older TeaVM/default-denial artifacts keep their earlier source identity; no native RMW/JNI/wait-set claim |
| Optional Python/Java records | Python 29 tests; Java 14 cases/47 checks; six full-wire JointState/RobotMsg/CIOState catalog roles passed | Same bounded schema IR, full fields/bounds/supported defaults; no native identity or delivery proof |
| C# foundation | .NET 8 host 16 local contracts; real ROS1/ROS2 Wasm C bridge objects validated with all 38/43 exact import signatures | The model and bridge objects alone do not qualify Mono/WASI |
| Optional C# records | Twelve source policies and eight actual generated catalogs passed; .NET compiled and ran fifteen record cases/57 checks, one eight-field naming case and six complete family wire roles | Same authoritative bounded IR, strongly typed fields and no reflection; no Wasm or native ROS delivery proof |
| MoonBit foundation | Twelve packages compiled for both `wasm` and `wasm-gc`; all 24 actual modules passed validation, import/layout checks and Runtime execution | Four raw ABI runs, four actual default-denied Host opens, sixteen empty-alias local cleanup runs; separately, 66 models per target passed on Node with zero ROS imports |
| Optional native service availability | Independent full/focused source review and integration; exact original81 ABI association, eight preservation policies and nine DAP policies passed locally | New explicit observation grant and optional per-backend namespace; current-source Runtime, selected-SDK native, Guest and language-facade adoption remain required |
| Original-upstream static WASI platform | Eighteen clock/logging/bootstrap/static-build source files independently reviewed and integrated, including explicit genuine std_msgs generated-library dependencies | Zero configure/build/test executions; real RMW entity/metadata providers, default generated endpoints and genuine rclcpp threading remain incomplete |
| Optional C/C++ service-client SDK | Fifteen independently reviewed source files integrated, including complete raw/C/C++ ownership and finite rclcpp profile adoption | Six raw signature declarations and29 new model groups are authored and unexecuted; old availability policy checks do not qualify these SDK bytes |
| Original-upstream RMW core | Thirteen source files independently reviewed and integrated; real original SDK types, bounded node/owner registry, generated C codecs and same-native-publisher provider contract | Twenty-two groups remain unexecuted; actual original-header build and concrete Host/Gateway/provider transport remain required |
| Original-upstream RMW C delivery | Fourteen independently reviewed source files integrated for guards, bounded finite waits, owned subscription/service delivery, exact native/local request identities and complete AddTwoInts C codec | Thirty-five groups remain unexecuted; native transport and real finite wait providers are separate unfinished dependencies. This is not full RMW or concurrent executor support |
| Native RMW endpoint and metadata | Sixteen independently reviewed source files integrated for complete UMRE/URMI records, real retained publishers, actual QoS/GID and whole String/Bool/Twist/Image codecs | Source-only integration; actual selected-SDK compilation, native full-value/disappearance witnesses and Wasm transport validation remain unexecuted |
| Native custom publisher dispatch | Seven independently reviewed source files integrated, retaining the existing trusted type table and partitioning actual publisher factories across the selected shards | Full native SDK/link/MD checks, including an omitted-shard negative case, remain unexecuted |
| Native subscription queue | Six independently reviewed source files integrated for real same-callback metadata/body capture, bounded owned queues and independent nonreused reservation tokens | Nineteen queue groups and real original-header native subscription witness are authored and unexecuted; Host/Gateway/Guest source routing is integrated; actual current-source qualification remains required |
| Concrete Guest C publisher | Six independently reviewed source files integrated for the actual five-import owned publisher provider, complete native receipt association and once-only native cleanup | Twenty groups remain unexecuted; Host/Gateway source is now integrated, while actual original SDK/Wasm/native qualification is still required |
| Original C++ generated codecs | Ten independently reviewed source files integrated for real generated Message/Service handles, full native metadata graphs and String/Empty/AddTwoInts canonical conversion | Eighteen groups remain unexecuted; the descriptor source-layout extension requires all consumers to recompile. This is not full rclcpp compatibility |
| Owned publisher Host/Gateway route | Seventeen independently reviewed source files integrated, including the five numeric imports, real Gateway owner registry, Host lease/history/quota/cleanup, actual custom factories and Runtime dispatch | Nineteen value groups plus current-source raw/Wasm/native qualification remain unexecuted; no existing Runtime binary is relabeled |
| Complete original RMW publisher example | Three independently reviewed source files integrated for a complete standalone original-header String bootstrap, checked actual QoS/GID and reverse cleanup | No compile/run; its submission marker cannot prove external native receipt, and it is not unchanged default rclcpp acceptance |
| Owned native subscription registry | Seven independently reviewed source files integrated for UMRS borrow/commit/cancel, complete URMI/body association and independent cancel/commit/retirement diagnostics | Twelve wire groups, six diagnostic cases and actual selected-SDK/native witnesses remain unexecuted; Host/Gateway/C provider source composition is now integrated; actual current-source qualification remains separate |
| Direct native RCL client/service | Eight independently reviewed source files integrated, retaining one private rcl client/service for actual sends/takes/responses, both QoS profiles and independently granted same-client availability | Fifteen value groups/two native service families unexecuted; actual RMW metadata completeness and Host/IPC/Guest routing remain required |
| Complete owned subscription source chain | Native custom factory9, Host/Runtime/raw7 route16, authenticated Gateway7 and concrete original-RMW C provider/example9 independently reviewed and integrated | Source chain complete for selected implemented families; every current-source model, raw link, original SDK, native/Wasm and cleanup qualification remains unexecuted |
| Retained service wire and pair history | Seven UMRC/raw12 protocol paths and four two-capability/history paths independently reviewed and integrated | Eighteen wire and eighteen pair value groups unexecuted; numeric values never confer native authority |
| Retained direct-RCL service registry | Nine source paths integrated for actual retained Empty/AddTwoInts objects, exact full pair, native receipt and independent effect/ACK/cleanup diagnostics | Eight helper groups and two real native family witnesses unexecuted; positive graph waits and genuine explicit custom canonical factories remain required |
| Real service policy and admission | Six source paths integrated for original ROS2 Empty/AddTwoInts identities/canonical bodies and true Host body-only admission; four further paths correct nanosecond wait-limit parsing | Nine policy groups and separate wait-limit regressions unexecuted; a policy identity or bounded parser is not actual native readiness |
| Concrete Guest C service transport | Fourteen source paths integrated for raw12, real service/subscription provider composition, complete C main with actual C++ objects, separate pure-C AddTwoInts main and native counterpart | Eighteen fault groups unexecuted; full pure-C Registry and actual old-RMW availability adoption are source-integrated; current-source native/SDK qualification is still required |
| Managed SDK retirement composition | Thirty-one Java/Python/C# source paths independently reviewed and integrated, preserving original handles on proven pre-entry failure and separating attempted/unknown cleanup | All new regression groups remain unexecuted. Earlier JVM/CPython/Mono/WASI results above predate this source composition |
| Service Host/Runtime successor | Seventeen source paths independently reviewed through full baseline and two narrow corrections, then integrated: actual raw12, current paired/query leases, full deadline/quota/borrow compensation and independent RPC/API journals | All five original findings plus early-journal refinement closed in source;27 helper groups,8 parser cases and12 signature checks remain unexecuted |
| Authenticated owned-service Gateway | Nine independently reviewed source paths integrated for actual retained Native Registry/CPP10, opt-in trusted profile, correct public/private retirement and native cleanup before async join | Ten helper groups and genuine selected-SDK/socket witness remain unexecuted; the separately reviewed real same-client positive graph wait is now source-integrated13; actual SDK/link/runtime qualification is pending |
| Complete original-generated-C object Registry | Fourteen independently reviewed new files integrated with seven full C-object codecs, eleven independent native identities, complete original graph/source/event callback validation and three complete C bootstraps | Twenty-two actual-SDK groups and two original-native counterparts unexecuted; finite String/Empty/Add family only, not full interface/vendor coverage |
| Native-only ROS IPC cancellation | Six independently reviewed files integrated for true borrowed ChildLease eventfd propagation through connect/hello/call/poll, no replay and independent unknown-effect/old-session diagnostics | Twelve genuine Linux primitive groups unexecuted; actual threaded bridge/current-source admission, nesting, physical join and complete management lifecycle remain required |
| Original RMW independent client availability | Twenty independently reviewed source paths integrated for same-owned-client query, optional explicit third query owner, preflight capacity3, full Context scope and one validated public bool write | Existing35 plus17 new C model groups and actual transport wrapper unexecuted; no alias/provider means95 without an extra native client or default rights |
| Concrete finite WASIP1 wait provider | Twelve independently reviewed source files integrated with actual original monotonic poll_oneoff, validated event/count/full userdata, once-owned allocator/buffer state and complete C composition | Positive actual compile/link probes, native unsupported case and all Guest granted/denied/clock cases remain unexecuted; real Runtime monotonic-clock and original RMW clock grants are independent |
| Same retained Native client positive wait | Thirteen source files independently reviewed through three corrections and integrated; real graph event/Context/session/worker/atomic-stop observations, requested versus transport deadlines and output preservation | Both-family five-mode original-SDK witnesses and shared graph/concurrency/fault/retirement cases are authored and unexecuted; source integration requires all common server consumers to recompile |
| General ROS2 scalar defaults | Twenty-one independently reviewed source files integrated for bool/integer/finitefloat defaults, nine language record outputs and complete modern Service/Action graph/default proof | Original xArm GetSetModbusData full source/defaults retained, typed Native and maintained policy sources authored; all current generation/build/Native/Guest tests unexecuted; owned custom factory default admission is now source-integrated11 and unexecuted |
| Owned native custom Service factory | Twenty-five source files independently reviewed through UTF-8/native-fixture and RAII retirement corrections, then integrated; complete modern native graph/source/layout proof, real typed objects, retained endpoints and bounded owned canonical records | Original GetFloat32List/SetInt16List and strict malformed-body/native fixtures are authored and unexecuted. The independently reviewed scalar-default factory11 successor is now source-integrated; missing modern metadata/export proof still refuses |
| Production custom Service Gateway | Eleven reviewed source paths integrated for actual selected factory/bounds, before-Node qualification, exact private storage admission and one body validator before effect/credit | Original fixed service/default-off behavior retained; actual selected-SDK/link, full wire, shared-graph and physical cleanup witnesses remain unexecuted |
| Complete original custom Service method shards | Nine source paths plus a separately reviewed two-line CMake seam integrated; exact original typed invoke/serve and lifecycle bodies moved into all selected shards | Metadata, empty/O0 and omitted-shard link fixtures are source-only. Generated-header parsing and unrelated Action work remain; no measured memory/performance improvement claimed |
| Guest source migration and actual JAR SDK entry | Four reviewed strategy/recipe/checker files and the actual external-producer SDK entry integrated; historical52-source contract preserved, current56-source JAR entry8 with separate original-main bootstrap and explicit optional3tuple admission | Source-only;31 current DATA cases unexecuted. Generic producer remains an explicit external tool. Original Java service waiting, full old APIs and current producer/Guest/Native qualification remain required |
| Cold management and genuine threaded Runtime | Coherent66-path source composition integrated with exact r2/r3 overlays, true Source/World/compiler/native-thread owners, cold six-function exclusion and actual named-effect generation-lease proof | Two confirmed lifecycle findings source-corrected and independently reviewed. No current compilation, race/physical join, Guest/Native/DBG or complete original-rclcpp qualification; selected tag/EH routes do not prove the full C++ ABI |


Local builds/tests ran from source copies under `/tmp`, serially. The new
managed-record Python/JVM run observed approximately 115 MB peak process RSS;
rcljava approximately 123 MB; rclc approximately 18 MB. These are resource
observations, not throughput or VTune performance measurements.
The later Java Service/Future local run used five serial commands and observed
approximately133 MB; the optional availability source-policy run used two serial
commands and approximately51 MB. Neither exercised new Wasm or native ROS.

## Additional source integrations; no new candidate executions

The explicit Java service-availability implementation now contributes13 reviewed
source paths. Existing no-query construction keeps its two-owner behavior;
query and real elapsed-time Clock are separate explicit roles. The independent
optional ABI retains the exact three Core signatures and8-byte feature/1-byte
boolean outputs. Positive/infinite waiting uses real bounded queries and an
actual granted steady Clock, with query-budget exhaustion reported90. It does
not convert executor credits into time or readiness. Its28 authored policy groups
and new actual Wasm/Native routes remain unexecuted.

The current existing-JAR SDK entry has an independently reviewed eight-path
successor. Its56 library sources are49 unchanged historical sources, three
explicit Java13 changes and four new optional classes. One selected original-main
bootstrap adds one separate compilation input. The real external producer is
source-bound at SHA256
`9053229590f2401502feba6ab9b6f0265ba596ed3243607c82e7a65cf32f93c0`;
optional admission requires both the actual catalog path and its independent
SHA256. The default business81/WASIp1 boundary and the complete original14 DATA
cases remain unchanged. Seventeen additional DATA cases are authored, not run.
Old52-source/7a2 producer records and the corrected FIFO-ABI reader finding remain
historical. Current source is not qualified by an older43-import artifact.

The scalar-default native custom Service factory now contributes11 reviewed
source paths. Private modern proof checks the complete original default text,
source digests, independent exported identities, typed layouts and genuine
Service/Event ownership before treating an introspection default pointer as
unknown. The public legacy matcher remains strict. Complete real C initialization,
C++ construction and xArm GetSetModbusData Native companions are authored. A real
mandatory rosidl_runtime_c dependency now also covers the unsupported95 C path;
actual selected-SDK/configure/link/value/Native execution remains required.

The original-rclcpp WASI signal component now contributes25 reviewed new files,
including the actual fixed private original header/implementation pair, genuine
thread/semaphore/TLS/Wasm-EH link-probe source and complete English contracts.
Its receiver and original deferred worker use real lexical worker identity to
refuse reentrant install/uninstall before the installation mutex; private options
are atomic. The original deadlock/race/probe findings and their narrow corrections
remain recorded. All actual compiler/probe/Guest results remain zero. The trusted Host pipe/map/sender and shared RMW dispatch/infinite-wait source
have since been integrated and freshly preserved; the explicit original source
and composite build hook are now implemented. Actual complete native/default
Node/SDK qualification and the corrected control profile remain required.

## Current source continuation after temporary-draft loss

The current integrated source additionally contains genuine threaded RMW29,
original default-control C/C++ canonical source60, SDK33 standard-EH toolchain4
and Source-owned Host signal14 plus the Runtime's two include hunks. Their
recent temporary integration/peer receipt bodies disappeared before durable
archiving; the cause is unknown. Complete actual public bodies were freshly
preserved as a 161-file recovery observation, together with five exact earlier
preimages recovered from real r13 gzip/SHA data. This does not recreate those
missing receipt hashes, physical metadata, compiled artifacts or test results.
Historical language SDK source remains independently identified.

The following new exact source integrations have complete durable before/after,
whole/narrow corrections and independent source reviews:

| Surface | Implemented current source | Actual qualification |
| --- | --- | --- |
| Complete type-hash selector | Four paths correct GetTypeDescription hash capacity64 to71, preserve declared true default, and author real generated-C full-wire/refusal contracts | Zero generation/compile/run; independent actual native hashes remain required |
| Original C service ownership | Seventeen paths add stable zero C owners with genuine finalizers, exact two-header source ports, real C-link probe and14 ownership contracts | Zero actual SDK/probe/contracts; original RCL remains the response's single initializer |
| Complete original source/build entry | Five paths extract the fixed full original Git tree, verify every intermediate tree/blob, apply four exact ports and compose genuine threaded static targets with PUBLIC C ownership | Explicit option defaultsOFF; actual compiler/archive/import/MD/EH/TLS/default-Node acceptance remains required |
| Standalone OS signal CLI | Sixteen paths add explicit six-field configuration, fast_io decimal admission, real lock-free notification/ordinary-thread sender, absolute retirement budgets and terminal process containment | Linux nonmodule feature profile only; all19 pure checkpoints, Native probes, real OS/Guest and retained-owner cases unexecuted |
| Threaded readonly debug metadata | Twelve exact source hunks compose StageA r2 with current Host signal, preserving default-none image/map/gettid/TLS-registration absence | Source-only; threaded -Rdbg, pause, checkpoint and complete ROS debugger remain closed/incomplete |

The original source extractor's first review found an intermediate Git-tree
membership gap; the separate r2 verifies every binary tree before deriving its
children. Signal CLI `_exit(125)` is actual terminal process containment when
Native/Source retirement remains pending, never a successful physical join.
Its cold-preparation/underlying-syscall deadline still belongs to the external
Root process owner. No Guest management or additional default authority is added.

The historical Native default-control18 r1 HOLD retains its two real bootstrap
findings. Its independently reviewed Native19 r3 successor is now integrated:
the actual original TimeSource subscription and strong run_original source are
present, with positive deadline/pending-ID cleanup and genuine fast_io decimal
admission. It keeps the original Node defaults and all10 endpoint roles. The
mixed C Log/GTD and C++ parameter objects have complete separate inspectors,
metadata/default/source proofs and ownership. No genuine SDK object was run.

| Further closed source | Exact integration | Actual qualification |
| --- | --- | --- |
| Counted raw source and aggregate quota | Ten paths preserve TypeSource bytes including NUL/nonUTF8, fields10/refs11/sources12/raw6162, and separate32768 total quotas | Native true-source witness and all canonical contracts unexecuted; Cartesian/stack/RSS remain separate |
| Common full control and vendor catalog | Eight production plus3 contract paths, operation-aware complete ParameterEvent, whole-table poison and genuine policy/manager/handle lifecycle contracts | Default authority empty;5 preparation/8 frozen and poison groups unexecuted |
| Strong cold factories and storage | Three actual registry factory seams plus3 independent canonical-history/Native-depth files | Native DDS1000 and canonical history1..256 stay separate; no physical memory or QoS proof inferred |
| Original Guest Native control | Nineteen new actual-header adapter/Registry/composer/freezer/default-Node/contract sources | Zero generation, SDK compilation/link or Guest/native communication |
| Independent Native Gateway | Twenty-one separately reviewed all-C++ proof/converter/typed-factory/API/build/contract sources | No actual installed SDK probe, endpoint, source serialization or native receipt result |
| Explicit Host composition | Six reviewed main/build/profile files; split7 and complete linked common/native-provider/static/contract environment corrected | Explicit opt-in build/storage/ordinary grants; legacy full-control business paths refuse95 |
| Actual source-byte loading | Seven reviewed SDK/contract files compile captured admitted source, exclude pyc, retain descriptor/name/SHA identity and fix scratch aliases |10 identity,11 source-domain and20 original source-profile methods remain unexecuted |
| English migration and source ledger | Seven corrected guides/recipes/ledger files;62 exact current selected source refs | Data reopens only; checker, compilers and source recipes were not executed |

The raw source getter's counted terminal NUL is retained. Genuine
GetTypeDescription(true) source compares all11 referenced descriptions and12
source rows, including all source bytes. Explicit32768 total wire admission is
separate from full Cartesian storage. A source review cannot establish that a
particular middleware's native serialization preserves those counted bytes.

The authorized HLL task also integrated12 B1 debugger development-source paths,
4 unexecuted contract paths and4 English/entry documents, then224 reviewed cache
source paths across3 repositories. Both keep their own complete actual receipts.
Ordinary threaded -Rdbg/checkpoint gates remain closed; private Host control
source is not complete ROS debugging qualification. ROS/threaded/DBG persistent
cache admission stays refused. Actual keyring/PR0, cache cold/warm/negative cases,
full compilation/races/physical joins and hardware performance are unexecuted.
The current ROS Runtime TU is1,066,847 bytes, SHA256
`856d10135ae2656faa99c6fbf0463370db6d9d4e58dc892b705136f68ea2124c`.
All prior Runtime/CLI/StageA/Signal receipts keep their original source hashes.

The complete four-language control record source is now independently reviewed
and integrated as12 paths: Rust, JavaScript, AssemblyScript and TinyGo retain all32
records, full scalar/default/constant/source-byte semantics and the seven original
Service root identities. A separate records-only operator preserves the old C/C++
eight-product route. Its captured-catalog and executed-source/proof association
findings are corrected. No products have been generated by this new source.

The genuine optional raw12 Service SDK source is also integrated separately:
Rust10, TinyGo12, AssemblyScript15 and JavaScript10 paths. They use explicit
source/peer/query owners, full UMRE256/UMRC160/URMI192 values, actual Native
request metadata and independently expected QoS, bounded lifetime histories,
full typed preparation before ACK and once-only independent cleanup. The old
business service_call/service_submit/event_poll path is not a substitute and
continues to refuse95 for full control. No implicit permission is added.

| Current source cohort | Actual source correction/contract | Execution |
| --- | --- | --- |
| Records12 | Full four emitters; captured catalog/generator proof pair;14 source policies plus typed language contracts | No generation or contracts run |
| Rust10 | Full7 actual generated root bindings; acknowledged control tail performs only prepared local work, cleanup remains explicit;23 authored model groups | Unexecuted |
| TinyGo12 | Genuine revoke retirement, independent owner cleanup and preprepared control/typed ACK result;30 authored wire/ownership methods | Unexecuted |
| AssemblyScript15 | Complete checked codec status22/90, metadata and late abandoned response disposal before ACK;65 authored assertion sites | Unexecuted |
| JavaScript10 | Real single-invoke C bridge adapter, full7 bindings and complete caller-driven applications;5 discovered once/cleanup/audit/failure-carrier/throw-value findings corrected;23 authored model groups | Unexecuted |

The authorized HLL task separately integrated the actual opt-in QuickJS raw12
C builtin and closed producer/artifact gate as9 source paths. Fresh complete
QuickJS2026-06-04 archive and all29 locked member bytes match the existing source
lock. Additional actual retained CPython3.14.8 C API bodies were captured read-only
from Linux in the64GiB/swap0 cgroup for Python bridge implementation; they are not
claimed to be pristine upstream sources or a new compiler/runtime qualification.
Python/Java/C#/MoonBit raw12 SDK/producer source work continues in that task.

Full named-module threaded Runtime/initializer/Group/Host/loader/API/CLI type
ownership is a concrete remaining implementation: the current header-only path
cannot be advertised as a complete module build. Its genuine Source closure is
being implemented privately. Native service examples also need the independently
identified paired-manifest/provider include/build correction and truly fresh
current-generator native output. Old generated-v2 headers remain historical;
current generator source does not retroactively qualify their contents.

All new generation/compiler/model/Guest/Native/ROS/VM/QEMU/PMU/VTune execution
counts remain zero. Previous actual tests retain their original source/artifact
identity. Complete upstream APIs, all distributions/vendors/official suites,
physical memory, simulation and performance acceptance remain required.

## Actual managed Wasm producer and default-denial checks

The actual Java producer used TeaVM 0.13.1 and an independently acquired,
complete private JDK. Its Wasm is 414,999 bytes, SHA-256
`e6a0dfeadf6a3216cbf3fd86313394c79d66ff091b00cb54caac34ce799c8717`.
Full validation and all 43 ROS2 import signatures passed. The actual no-grant
Runtime execution returned zero and exactly
`PASS ROSWASM_JAVA_DEFAULT_DENIED13` plus newline, with empty stderr.

The actual .NET 8.0.408 / Mono 8.0.15 producer used the original, separately
compiled `roswasm_bridge.o` in both the generated P/Invoke table and final link
response. Both final modules passed full validation and their complete 38/43
ROS import checks. The retained-symbol modules are:

| Backend | Bytes | SHA-256 |
| --- | --- | --- |
| ROS1 | 10,595,827 | `82ac78aebc5bfc5714f1d265be0c9cb1a980f0101a8179174b9a3dade0fce0b4` |
| ROS2 | 10,599,224 | `78ca641f3eb50acec66611c64ba8fb115572a0332cd3d384b51343231521d1f6` |

Both actually executed with no additional ROS/WASI grants, returned zero,
printed exactly `actual default-denied open13` plus newline and had empty
stderr. Observed runtime process peaks were approximately 857/863 MB. No
successful native ROS delivery is inferred from a denied open.

The rcljava migration profile also has two actual TeaVM artifacts, each with all
43 exact ROS2 imports. The independently selected default-denial bootstrap ran
on the same qualified Runtime, verified actual Host error 13 and checked its
rollback diagnostics. Its success does not establish positive middleware
delivery by the ten original application classes.

MoonBit's 24 debug artifacts were produced by the actual pinned compiler using
the local `moon.mod`/`moon.work` workspace. Every actual module passed full
validation and the complete selected 38/43 import inventory. A separately
reviewed bounded import inspector now handles GC recursive type groups without
losing non-function type slots; ten model checks and all 24 real import checks
passed. Validation remains a separate mandatory gate.

The layout gate checked each actual memory declaration/export and active data
against the reserved stdout/ROS arenas. The linear target's pinned producer
allocator initializer starts above both the arena and active data; GC artifacts
have no ordinary-memory allocator or memory-initialization/growth instructions.
This is a scoped artifact/producer layout check, not a formal allocator proof.
All 24 actually executed with exact expected stdout and empty stderr. Examples
with empty deployment aliases test local startup and checked cleanup; they do
not send topic, service or Action payloads to native ROS.

The earlier CPython 3.14.8 / WASI SDK 24 artifacts for both ROS versions passed
full validation, actual 38/43 import checks and actual default-denial execution
on the separately qualified Source64 dual baseline. Both printed exactly
`PASS ROSWASM_PYTHON_DEFAULT_DENIED13` plus newline. Those artifacts predate the
new Python lifecycle successor; their receipts retain that original source
identity. They are not promoted to acceptance of newer SDK files.

## Exact runtime identity and preserved failures

Java, rcljava, C# and MoonBit ran on an independent baseline containing Source64 plus exactly six
WASI getter/directory-read fixes and the existing two-stage command-entry fix.
Its source manifest SHA-256 is
`42a532b8637fed42663bf1cc01cbcae5ecfef74bd0a309379ec66ae13ff9e58d`;
its dual ELF SHA-256 is
`649822657c11a9ee82a253f0f7334e1999944a4640329115095d632263627d99`.
This is not whole-current Source65 qualification. All three translation units,
actual compiler loader/link inputs and canonical full MD dependencies were
checked. Builds and tests stayed inside the exact existing 64 GiB/swap-zero
Linux cgroup. The temporary, coordinated serial compiler window ended when its
owned processes retired; ordinary per-tree/aggregate limits then resumed.

The first Java baseline ran only TeaVM's start-section initializer and returned
zero without executing its distinct `_start` command. That result failed the
required marker check. Explicitly selecting the command on that old baseline
then triggered out-of-bounds protection because initialization was skipped.
Both failures remain preserved. The existing scoped entry fix runs initialization
before a distinct command and avoids running the same initializer twice; the
actual successful rerun uses the newly built ELF above.

Other preserved failures include an incomplete JRE used as a compiler, an old
local Python selected by the first record-test runner, and metadata-driver
errors. They are distinct from SDK failures and remain separate records.
MoonBit's first two real pinned-compiler checks exposed private-test visibility
and ambiguous record construction errors. Narrow reviewed corrections moved
private tests to whitebox files without making SDK state public, and supplied
the two precise result types in each backend. Fresh SDK checks passed for both
targets. The first example build then exposed obsolete JSON manifest syntax;
the reviewed local workspace migration retained all twelve original example
bodies and link settings, and both actual builds passed. Failed attempts remain
preserved. Subsequent real test production built both targets, and all eight
nonempty model modules passed whole validation, observed numeric imports, tag
and scoped layout checks. A separately reviewed exact-frame runner executed
66 models per target on the pinned Node reference engine, with zero ROS import
calls. These 132 executions are not UWVM or native ROS cases.

The first actual C++ service build found a missing byte-write helper in both
its complete AddTwoInts codec and the model. A reviewed correction used the
existing wire append helpers and a private two-u32 model slot writer. The next
run found a model RPC that returned success without writing the mandatory zero
output length. A single-model correction now follows the actual Host response
contract. The production callback lease and all 26 assertions stayed unchanged.
Fresh compilation, linking and all 26 service plus ten prior topic cases passed;
all earlier failed reports and diagnostic traces remain failed records.

The Python service successor adds 26 models to the previous 49. All 75 passed
using copied sources under `/tmp`. The freezer subsequently received a separate
reviewed package-selection correction: its five explicit compatibility package
roots preserve the same 21 module names while excluding 27 preserved upstream
and example audit modules. That freezer successor has not yet run, and none of
these newer sources inherit the old CPython Wasm artifact qualification.

## Required work still outstanding

Complete upstream APIs and official test suites remain required, including
complete Service/Future/Task and Action APIs, parameter transactions, callback groups, native
availability/wait sets, graph/remapping/QoS and complete generated-message APIs.
Actual native availability cannot be replaced by a graph name-list result.
New service compatibility work uses the existing asynchronous submit/observer
protocol and exact request IDs; no synchronous call is labeled asynchronous.

The newly integrated real-RMW source corrects native all-zero GID/writer handling
and accepts equivalent nonnormalized RMW time representations. Release-mode
contract operations cannot disappear through `NDEBUG`. These are reviewed source
corrections; no original SDK compile, Wasm/native run or hardware result has been
inferred. Native endpoint/metadata, custom publisher shards and subscription
queues are now independently reviewed source integrations (29 files). Concrete
C++ generated codecs and the Guest C publisher transport are also independently
reviewed source integrations (16 files). The owned publisher Host/Gateway route
is now independently reviewed and integrated (17 files), along with its complete
original-RMW publisher example (3 files). Native subscription transactions (7
files) and the same-object direct native RCL client/service provider (8 files)
are independently reviewed source integrations. Actual original SDK, Wasm and
native qualification of all these successors is still unexecuted.
At the earlier66-path source integration, the Runtime production source
included the reviewed publisher/subscription/service namespaces and explicit
threaded profile. Its then-current SHA-256 was
`ee226a6c36b40690202be702844f5b49932f39994c227dc0b6ea425041bf0433`.
The preceding `3d2371f6...` source is retained in the exact66-path preimages.
The current initializer has a separate token-equivalent trailing-space successor,
SHA-256 `58e470b5a8a905186c51ca3ba56dffc5b698dcb5f753cb013f5fef4a109e4b9d`.
This is source identity, not a new Runtime binary qualification. The original
service12 Host findings and early-journal correction remain source-corrected;
no earlier binary result is promoted to this combined source.

The complete managed Java/Python/C# retirement composition is now integrated.
Actual call-site witnesses retain all original opaque handle bits across proven
pre-entry preparation failures; attempted/unknown/returned effects are not
replayed. rclpy default-context exit and subscription scheduler cleanup, plus
rcljava terminal Future reads, are source-corrected. Every previous managed
model and Wasm result keeps its earlier identity; current-source generator,
model, compiler, Guest, permission and native qualification are still required.

Concrete subscription source now connects the real native queue/registry,
Host/raw7 and authenticated Gateway to a complete original-RMW C provider and
example. Service source now includes UMRC/raw12 values, immutable two-party
history, real same-object Native registry, actual builtin policy/body credit
and concrete C transport/examples. Service Host/Runtime17, authenticated Gateway9,
complete pure-C object Registry/examples14, true IPC cancellation6 and original
RMW independent availability20 are now source-integrated. The actual same-client graph wait13, genuine finite WASIP1 wait provider12 and
complete ROS2 scalar-default source/modern protocol proof21 are now also
source-integrated. The compiled custom Service factory25, production Gateway11,
complete original Service method shards9 and the actual threaded Runtime66 are
now also source-integrated after independent review. The zero-local quota and
first-terminal-publication findings are closed by exact narrow source patches.
The independent nine named-effect report and separate initial-read gates retain
their platform/restored-world limits; a historical12-boundary map is not twelve
new tests. Full original-rclcpp platform/thread-safe RMW, default endpoint graph,
complete managed client APIs, cache and all legacy API work remain. Explicit Java availability13 source is now integrated and unexecuted. Every new model,
compiler, Guest/SDK/native and hardware result remains unexecuted.
These components are not complete upstream APIs or official-suite passes.

Direct RCL takes preserve same-take native request IDs and timestamps. RCL
initializes timestamp members to zero before invoking the selected RMW; struct
availability and a returned zero alone cannot prove that concrete RMW supplied
complete metadata. Actual selected SDK/RMW qualification is mandatory, and zero
is not treated as an absent-value sentinel.

The actual threaded source uses a native pthread Group with explicit quotas,
separate child Worlds and retained Source/code/provider owners. Only real native
joins, including compiler workers, can complete its retirement. It retains a
pending state while terminal publication or physical resources are unfinished.
The zero-local budget remains zero; it admits an imported-shared-only shape
without silently granting child-local memory. True lexical cold exclusion and
current generation leases preserve the existing debugger/checkpoint boundaries.
Default thread/ROS authority remains absent. Ordinary disabled-profile Host
paths can contain TLS checks/branches, so zero overhead is not claimed.

Original rclcpp still requires actual threaded libc++/libc++abi/unwinder,
source-specific signal notification, thread-safe RMW guard/Context/infinite waits
and complete default entity codec qualification. Their newer source components
and actual composite build entry do not constitute compiled/linked acceptance. A historical SDK24 single-threaded build
does not prove that dependency closure. The private SDK33 primary-source and
C++ EH/thread foundation work is not a compiled or executed rclcpp result.
Real Java/JAR SDK argv now targets the actual external lowering producer;
the old frozen strategy ledger must be explicitly refreshed after all relevant
source successors are composed. Its old39 pins remain historical and correctly
refuse changed source rather than certifying the current tree.

The full 26-distribution ROS1/ROS2 and 19-vendor matrix, simultaneous independent
vendor flows, Unitree simulation, all requested QEMU architectures including
LoongArch, and native-versus-Wasm performance qualification remain open. Actual
VTune hardware Hotspots and Microarchitecture Exploration still require a
separately coordinated quiet collection with meaningful samples/counters and
equivalent useful workloads. Compiler backends, source inventory and local
models do not complete those requirements.

The Guest still receives no ROS authority by default and cannot change update,
debugger or Host grant settings. Deployment mappings select explicit aliases;
they do not grant them. Signing, WasmGit, hub and multi-program orchestration
remain outside the requested Runtime scope.

## Durable receipts

[The current archive index](continuation-20261018-evidence/index-r16.json) binds
22,425 source/review/result records to lossless gzip copies (142,512,783 compressed
bytes counted by record), retaining all20,387 records from
[index-r15](continuation-20261018-evidence/index-r15.json) unchanged, including
its complete r14/r13/r12 predecessor records. All retained gzip CRC/body hashes
were reverified before this append. Complete
fixed source bodies, original preimages, whole/narrow patches, actual official
Git/source bytes, independent findings/corrections and exact integrations retain
their source and execution identities. Source-only cohorts add zero executed
model, Runtime, Wasm, native ROS or hardware results. The66-path source
integration's read-only-file recovery and subsequent26-line whitespace-only
check correction are retained separately; final `git diff --check` passed.

Bounded Linux QEMU/ARMhf tool, image and ABI inventories ran in the exact64GiB,
zero-swap cgroup. They did not launch a VM/Guest, candidate compiler or test and
did not modify the user disks. Linux root free space was4,300,402,688 bytes at the
last storage inventory, close to the required4GiB floor. The HLL storage DFS
stopped at100,000 entries; partial directory totals are not complete storage or
safe-deletion proofs. These are actual read-only SSH observations, not QEMU,
architecture or ROS acceptance. Original failures remain preserved and every
binary/source/build association still requires its own genuine receipt.

[The separate maintenance archive](continuation-20261018-maintenance-evidence/index-r3.json)
preserves58 original cleanup/diagnostic/source records. All four actual cleanup
attempts remained HOLD: zero leaves deleted, zero FITRIM and no physical-space
reclamation claim. Read-only diagnostics identified the exact inaccessible FUSE
mounts and an occluded systemd credentials mount. No general filesystem/alias
exception was added. Root-owned maintenance and diagnostic processes retired;
the exact64GiB/swap0 cgroup and zero-OOM observations are separately recorded.
This is storage maintenance evidence, not ROS acceptance or performance evidence.

The owned idle Linux envelope has now been restarted after observing that it
was stopped. Its actual new boot is51c27aa1-d709-4a06-ba16-2ac3003fbcbd; payload
work reasserted the actual64GiB memory cap and zero swap before read-only facts.
Root physical free space was4,298,502,144 bytes. This is only slightly above the
4GiB safety floor; no large candidate build, ROS test or VM was started. Old
boot/inode/mount/deletion observations are historical and do not authorize new
cleanup. Root keeps the remote window exclusive while source work continues.

The latest actual read-only HLL maintenance window returned the same actual
51c27aa1-d709-4a06-ba16-2ac3003fbcbd boot and exact64GiB/swap0 envelope. Root
physical available space was4,298,530,816 bytes. A bounded complete scan of two
known HLL intermediate roots observed2,534 .o/.d leaves,304,701,440 allocated
bytes; that inventory alone is not deletion admission. Original batch archive
SHA values remain exact, the old candidate manifest names are absent and the
old failed cleanup keeps its original status. Nothing was deleted, trimmed,
compiled or run. Toolchains, final Wasm, DSO/.a and historical VTune data remain
protected. The explicitly returned SSH window is Root-exclusive again.


## Current source and storage successor; no new candidate executions

The generated-source verifier is now integrated through its separately reviewed
r2 and actual-public-r2-to-r3 narrow successor. It binds the current complete
32-record producer/operator/catalog route and all seven precise control-record
consumers, including the real JS model literal import. Existing three routes,
original native/record includes and typed-example rules retain their own checks.
Source-model loading compiles the actual captured trusted source without using
local cached bytecode; finite JSON numeric parsing rejects exponent overflow.
Twenty meaningful source-model groups are authored and remain unexecuted.
No generated product, compiler result or native type identity is inferred.
The current selected migration ledger's62 source references were reopened and
still match their complete recorded bytes/SHA values.

A complete90-path named-module Runtime/ROS/CLI source successor is frozen for
independent full review. Its Runtime implementation and threaded lifecycle
whole bodies are preserved, while real module exports/owners, initializer,
ROS Core/state/Host and actual CLI closure are being reviewed. A separate12-path
current Native selection/link-probe successor is also under review after actual
catalog/proof association and permissive linker-flag findings. Neither private
cohort is integrated or compiled at this checkpoint. Python23 and Java18
owned-Service source candidates are privately frozen with independent peers in
progress; C# and the complete MoonBit SDK remain in implementation. Earlier
advanced-language model or Wasm results do not qualify those current candidates.

The current exact64GiB/swap0 envelope first observed an empty underlying artifact
mount directory. This did not show that its old files had been deleted. Root
verified the original image inode30723291,256GiB logical extent and filesystem
UUID3fe7718a-8039-4f1c-b540-7c5cf05b65ca, then restored it read-only in the existing
private cgroup mount namespace. The actual kernel rejected standalone
`nologreplay`; the original failed attempt is retained. The documented
`rescue=nologreplay` option succeeded without disabling any checksum validation,
replaying the log, changing host-global mounts or reallocating the backing image.
All69 old ELF/object bodies then matched their complete historical SHA values.

Root durably preserved798 actual metadata/catalog roles as214 complete distinct
gzip bodies, reopening their CRC, extent and full SHA. These are fresh captures;
the missing two old temporary export packets were not recreated or relabeled.
After fresh exact body/parent-dirfd/one-link/live-process checks, Root retired
only69 owned obsolete ELF/object leaves outside all83 protected roots. No
source, directory, report, SDK/toolchain, LLVM/provider, final Wasm, HLL artifact
or raw performance input was removed. The first action's own validation FDs
caused a pre-unlink hold; zero deletion occurred and the diagnosis is preserved.
The successor exempted only those exact self descriptor numbers with their
pinned inode association. Other process, executable, mapping and FD checks
remained active.

The original task volume now remains normally mounted in that same private
namespace with `nodev,nosuid,noatime,compress=zstd:1`. Synchronization and FITRIM
were confined to this owned filesystem. Its backing allocation actually fell
from21,655,949,312 to21,306,847,232 bytes, a349,102,080-byte reduction. Observed
physical free space increased from4,298,194,944 to4,647,297,024 bytes (about4.33GiB).
The843,801,368 logical retired bytes are a different quantity. These actual
maintenance results supersede only the current storage observation; the four
older HOLD attempts remain unchanged historical results. No new model,
generator, compiler, Guest, Native ROS, QEMU, PMU or VTune workload was executed.
The remote SSH window is currently idle and exclusively coordinated by Root.


## Source integration and actual SDK materialization successor (r18)

This section supersedes earlier **current-status** statements above; their
original source identities, failures and immutable receipts remain historical.
The reviewed named-module90, current Native-selection12 and fast_io6 cohorts are
now public Source. The new complete-vendor native domain12, VendorIR4 and
vendor28 operator5 are also independently reviewed and Source-integrated. The
original19 inventory rows and old preparation behavior remain intact. Twenty-eight
inventory candidates do not mean28 qualified robot/controller families.

Root has now actually executed9 implementation Source-preparation commands.
Eight complete families succeeded: Booster official ROS2, Booster SDK bridge,
LimX bridge, ABB ROS1, ABB ROS2, FANUC ROS2, KUKA ROS2 and the corrected Yaskawa
ROS2 catalog. Together they produced104 actual SDK Source products
(10,891,686 bytes), including C/C++ headers and Rust/JavaScript/AssemblyScript/
TinyGo record/interface sources. Inputs, exact commands, actual outputs and
original stderr are retained. The original Yaskawa catalog was refused before
output because its Cartesian response maximum exceeded32KiB. Its separately
reviewed successor adds an existing aggregate response budget while preserving
all original fields, constants and per-field/count limits. No prefix or text
truncation is used. The wrapper's omitted `wire_limits` key and prepare/bind
file-count mismatch were independently found and fixed before public integration.

These are real implementation products, not successful compiler, test-model,
Guest, Native ROS, robot, QEMU or performance results. Root adds zero such runs
for these cohorts and supplies no invented native identities. The411-path
industrial kit and94-path Booster/LimX kit still need their complete independent
Source peer before concrete SDK/header publication. The457-path Spot/Kinova/
Stretch finite kit is frozen; full RobotState C/C++ protobuf field access is a
separate active successor. NVIDIA Isaac ROS, Isaac Sim, Jetson and GR00T remain
four distinct implementation targets. The genuine ROS readonly DBG loader
census is being reviewed; cache/checkpoint/mutation admission is not broadened
by that work. Other chats' language/security/debug runs retain their own scope.

Linux task storage is consolidated under
`/home/macromodel/Documents/uwvm-test-261018`. The7 moved owned roots originally
occupied9,523,855,360 allocated bytes (8.87GiB); the unified retained task data
occupied8,992,673,792 bytes (8.38GiB) at the cleanup observation. These totals are
task data, not garbage. Eight verified duplicate working paths represented
580,056,608 bytes, but retained external hardlinks meant actual observed
physical reclamation was98,328,576 bytes (93.8MiB). Original source, toolchains,
final Wasm, reports and compressed backups were preserved, with zero historical
receipt rewrites. Current free disk space must be freshly observed by each
resource owner; no old free-space number is presented as current.

All remote payloads remain inside the actual hard64GiB/swap0 cgroup. DBG's
previous8GiB build stopped under its resource guard without a qualified product;
that failure is retained. The next separately admitted window allows12GiB
observed total, an11.875GiB early stop and single-TU6GiB address-space limit,
with `-j1`, original `-O2` and a3GiB filesystem floor. Actual worker/child drain
precedes handoff. HLL returned its unused light window with no SSH/process
execution; Root runs no competing remote workload. VTune hardware Hotspots and
Microarchitecture analysis remain later exclusive qualification, after actual
build/VM retirement.

## Complete vendor SDK publication and actual Runtime build (r19)

This dated observation supersedes the earlier current-status paragraphs, while
preserving every original receipt, source identity and failure. The complete
Booster/LimX94, industrial411 and NVIDIA74 Source kits have passed independent
Source review and are now public. The exact138 generated C/C++/Rust/JavaScript/
AssemblyScript/TinyGo Source products for11 selected families are published
with their actual generation/preparation records and professional English
family instructions. Start at
[`COMPLETE_VENDOR_SDK.md`](../../sdk/custom-types/COMPLETE_VENDOR_SDK.md).
These11 catalog families are not11 fully qualified robot products or all28
requested vendor candidates. Native identity export/binding remains separate.

Current public generated vendor Source totals11,433,838 bytes. Root has actually
run21 implementation Source-generation commands cumulatively:20 succeeded and
one original Yaskawa Cartesian-budget refusal remains retained. This includes
old and fresh generations of the same families, so the command count is not a
coverage count. A separate private teaching RPC generation produced12 exact
Source files (160,783 bytes) against the reviewed JS cleanup template and the
real staged SDK. Its34 real-Session fixture cases are authored and unexecuted.
The fixture uses an instrumented local Native binding double and, when run,
can establish only its explicitly labelled contract model. Source generation
in fresh local `/tmp` is implementation, with no compiler/test/ROS credit.

The public successors also correct complete byte-span accounting, checked
AssemblyScript Time/Duration and JointState semantics, and genuine JS Session
RPC rollback/primary/cleanup/reentry handling. Full original definitions,
constants, supported defaults, per-field bounds and failure output preservation
remain part of the Source contract. Generated source is not hand patched.

The independent DBG owner has now built a fresh ROS product from all three
Runtime/main/HostAPI translation units and linked it successfully. The immutable
r10 Source ID is
`sha256:291c0c5cebb798bab1c9dcc4138866d906f8cc363f86d9e700d7b46c16a686b0`;
the ELF SHA256 is
`664ea84cbf6520c249b075a1817978e489a0c44362e800167bc6cbaa3f1a5a10`.
The build retained `-O2`, actual TLS, both ROS import profiles, the real project
constexpr budget, unchanged full dependencies/providers and fresh objects.
Seven fixture sources compiled using both actual WASI and Native producers.
The observed cgroup peak was10,060,193,792 bytes. This scoped build result is
not a live debugger or ROS transport result: the first actual container session
exited with SIGILL before its first pause packet. Diagnosis is active. New
private/source revisions do not inherit r10 qualification.

Spot/Kinova/Stretch and the full Spot original protobuf SDK remain under full
independent Source review. The mutable r2 fixes legal singular-submessage
merging and marks trajectory receive exceptions as an unknown outcome. Genuine
pre-RIHS ROS2 Native type/default/constant/IDL proof is also in implementation;
modern complete binding does not silently fall back to weak introspection.
Those private packages have no new compiler, Guest, Native or robot result.

Every Guest still begins with no ROS authority, and no update/debug/policy
management import. No kit opens native sockets, credentials, leases or CUDA
pointers inside Wasm. All actual Linux testing remains in the existing hard
64GiB/swap0 cgroup and the coordinated owner window, with actual retirement
before handoff. Full upstream client APIs, selected ROS1/ROS2 official suites,
all28 vendor/model/native SDK combinations, supported x86/ARM ROS targets,
simulation/hardware behavior and equivalent Native/Wasm VTune hardware Hotspots
and Microarchitecture measurements remain required.

## ROS owner continuation: vendor selection and complete Spot Source (r20)

The new coherent Spot/Kinova/Stretch Source directory is now published as1432
files (51,180,182 bytes). Its complete Spot protobuf field SDK retains1319
original messages,4409 fields and624 actual generated Source products. The
accepted singular-submessage merge and unknown trajectory-receive outcome
corrections are included. The entire verified all-new directory was published
by one same-filesystem rename; the historical per-file helper partial-write
rollback limitation is not represented as fixed by a file-count change. Native
SDK/threaded Wasm/compiler/ROS/hardware qualification is still required.

All28 vendor schema selections now have independent xmake switches and matching
explicit registry-header options, with every switch default OFF. Isaac ROS,
Isaac Sim and GR00T are independent ROS2 data options; Jetson is a separate
Linux ARM64 deployment check. A disabled vendor cannot inject its header, no
ROS backend or right is enabled implicitly, and multiple selected adapters must
use one explicitly merged registry. Native SDK libraries stay in separately
built Gateways. See [the complete option guide](vendor-build-options.md).

Actual Linux xmake configuration passed104 cases serially in the original
64GiB/swap0 cgroup, at an observed13,606,912-byte child peak. These cases ran
the genuine production selector through the actual xmake interpreter, with
original Source, logs and failed attempts retained. They establish configuration
behavior only. The separate full-project configuration stopped at missing
bundled OpenSSL Source in an incomplete remote snapshot; no alternative crypto
provider or full-project build PASS was invented. No new compiler, Native ROS,
Guest, QEMU, binary-size or PMU result is supplied by this continuation.

The owner continues only uwvm2-ros work without communicating with other agents.
Full native-bound model/SDK/driver adaptation and official ROS1/ROS2 suites remain
required. ROS architecture qualification is scoped to the selected upstream
x86/ARM platform support; actual big-endian SDK/sysroot/loader and runtime
behavior must be established separately. Default ROS authority and Guest
management imports remain empty.

## Explicit x86/ARM ABI inspection (r21)

The distribution profile now explicitly names i686, armel/armhf, armeb/armebhf
and aarch64_be/aarch64be alongside existing x86_64/i386/AArch64/generic Arm.
Actual ELF inspection distinguishes ARM-v5 soft/hard procedure-call flags,
including the specified dynamic loader, and refuses contradictory/incorrect
ABI, class/endianness mismatches and truncated64-bit headers. The Linux
metadata-only contract passed20 cases in the original64GiB/swap0 group. Its
synthetic headers were never executed and do not establish QEMU or ROS support.
See [distribution and architecture compatibility](distribution-compatibility.md)
for exact scope and original AAELF32 provenance.


## Unitree complete families: actual Native/Wasm/QEMU r22

The four complete-domain SDK directories contain all54 locked Unitree message
definitions (GO24, HG11, API8, legacyROS1 11) in six generated languages. Full
Native identities remain unbound, no registry is emitted, and no authority is
granted. C/C++ examples progress from the raw imports to genuine typed copied
telemetry. The local RPC helper now requires strictly increasing lifetime IDs,
including completion/expiry/forget, with checked signed64 exhaustion and no
automatic retry or remote-cancellation claim.

Actual execution in the original64GiB/swap0 Linux cgroup includes25 Source
parser contracts, six optimized/UBSan RPC-lifetime programs, eight Native
C11/C++20 codec programs (324 scenarios),16 telemetry object compilations,
and eight codec programs/324 scenarios under each user-QEMU target:
x86_64, i686, aarch64 and armhf. The missing exact common kernel-header
dependency for i686/armhf was authenticated against retained signed Debian
metadata and supplied from an independent root; old SDKs were not overwritten.

Sixteen newly linked telemetry Wasm applications ran on retained R10 ELF
`664ea84cbf6520c249b075a1817978e489a0c44362e800167bc6cbaa3f1a5a10`
with no ROS options or grants: eight denial-mode exits0 after observed13 and
eight receive-mode exits13. Eight further Wasm codec programs passed324
independent original-field scenarios. Their explicit1MiB stack, 2MiB initial
memory and 32MiB maximum replace an insufficient default64KiB test stack;
the original trapped run remains a failure. The same fixed Runtime binary
is not evidence of a latest complete Runtime rebuild.

The complete [evidence packet](continuation-20261018-evidence/unitree-complete-54-wasm-qemu-r1/integration.json)
retains original compiler/CLI/dependency/resource failures and final results.
The actual1023-file Source/evidence/Runtime bundle was retained byte-exact
(`a4a9280891ab8edd1eba8265549619c75a264e76f6164cbfba8968c6e922dabd`).
The current common SDK headers match the successful compiler inputs; explicit
uint32 return conversions fix GCC enum/integer conditional diagnostics without
changing the wire ABI. Serial compile/link jobs were used.

A previous3GiB disk guard stopped the first matrix before its next actor.
The bounded subsequent codec-only frame explicitly retained2GiB plus64MiB
actor headroom, with128MiB frame/16MiB child-file ceilings and no new SDK
downloads. Memory remained hard64GiB/swap0; OOM counters did not change.
No large Runtime build is admitted by this smaller codec-only policy.

This is not complete vendor/platform acceptance. Native ROS delivery, all
official upstream suites, complete model/revision compatibility, current
Runtime/Gateway binding, soft-float/big-endian SDK execution, real simulation/
controllers/hardware, and VTune hardware Hotspots/Microarchitecture comparison
remain separate open work. The28 per-vendor xmake switches remain default-off.


## Distinct complete-message payload coverage r23

The independent oracle now varies scalar/fixed-array values between modes and
refuses unless every one of the54 original messages has three distinct expected
payload hashes. Earlier r22 dynamic prefixes varied, but fixed-only messages
repeated their scalar data. Those earlier results remain unchanged; their
physical scenario counts must not be interpreted as distinct fixed-only inputs.

The strengthened162 record/payload pairs passed in both C11 and C++20:
324 Native local scenarios,324 per user-QEMU target (x86_64/i686/aarch64/armhf),
and324 in eight newly compiled Wasm programs on the same immutable R10 ELF.
The current independent oracle is byte-identical to the actual executed producer.
The [r23 packet](continuation-20261018-evidence/unitree-complete-54-distinct-payloads-r23/integration.json)
retains all270 Source/result/artifact roles with archive SHA256
`5c02fbfbce760336ddabfa6e904ebf493c34d17173aa54efbe337f519ef8d018`.
These are stronger local SDK contracts, with the same outstanding Native ROS,
official-suite, current Runtime, simulator/hardware and VTune acceptance gaps.


## Complete SDK delivery closure r24

Final local delivery verification found that the GO directory lacked
`bounded_records.h` and `bounded_records.go`. The earlier50-file local
publication was therefore incomplete even though the remote generated/compiled
inputs were complete. Both original products were restored byte-exact from
the verified archive. Each of the four public SDK directories now equals its
whole expected generated-file set plus generation/preparation metadata and
README:52 files in total,54 complete records. Every generated product hash
matches its original generator receipt. The [closure receipt](continuation-20261018-evidence/unitree-complete-54-distinct-payloads-r23/delivery-closure-r24.json)
records the correction. No compiler or execution result is reclassified by this
delivery repair; the original incomplete publication remains explicitly retained.

## Genuine Unitree ROS2 message values and CDR r25

The complete-field oracle now has an explicit native-ROS2 mode and a standalone
native CMake fixture. Three actual C++20/Humble amd64 programs passed129 distinct
CDR round trips over every field of43 original GO/HG/API messages. They use real
ROS2 generated types, `rclcpp::Serialization`, the actual FastRTPS RMW and actual
generated serialization libraries; restored canonical bytes equal independent
expected bytes. Its field-copy bridge is independent test code. This is not
production Native-converter, complete-domain identity, Gateway or ROS network
acceptance. No ROS1 native-serde result or official test credit is inferred.

The final r7 owner retains full-MD/source/library closure, actual compiler/Make
argv/cwd, original stopped direct-child birth/ELF/cgroup and kernel `map_files`
FD associations.35368 original SDK/derived files were rehashed before and after.
Seven private mounts were cleaned up. Hard64GiB/swap0 remained in force with
unchanged zero OOM counters; waited-child RSS peaked at341327872 bytes. Earlier
six failed owner/configuration attempts remain unchanged and uncredited.

Four newly generated default canonical C entries match the original executed
r23 inputs byte-exact. The actual legacy ROS1 catalog is refused in native-ROS2
mode before a C++ entry is written. Those are Source-only checks. The
[r25 receipt](continuation-20261018-evidence/unitree-native-cdr-values-r25/integration.json)
identifies the verified32.99MB archive with2373 roles and907 unique objects.
It was streamed to local `/tmp`, then moved to owned recovery storage; no large
duplicate archive was written to the nearly full Linux physical filesystem.

All requested vendor/model/firmware/ROS1/ROS2 native routes and actual official
suites, current Runtime/Gateway identity binding, remaining admitted x86/ARM
SDK routes, simulator/controllers/hardware and equivalent Native/Wasm VTune
hardware Hotspots/Microarchitecture remain open work. These local native CDR
witnesses grant no authority and do not turn the28 vendor options into completed
platform acceptance.

## Complete pre-RIHS Message Native binding r26

The explicit installed-source path now qualifies complete Messages on genuine
older SDKs without weakening modern hash-capable SDKs. It pairs original
installed IDL with actual field types, containers, native constants, constructor
defaults, offsets and complete introspection before deriving RIHS. A native
provider must explicitly compile its matching generated proof specialization;
the same bound converter without that wrapper remains refused. No default
registry, Guest authority, management import, Node or endpoint is added. The
[English operator guide](../../sdk/robots/industrial/pre_rihs/README.md) describes
the actual source/export/bind/provider sequence and separate ownership duties.

Final r10 executed19 genuine Humble amd64 native programs through58 serial
commands. All43 complete Unitree GO/HG/API Messages passed production binding
and129 independently populated production-converter/CDR payloads. Two additional
owned interfaces were generated by the genuine SDK, retaining empty text
defaults, signed floating zero,64-bit constants and nested/bounded fields; all45
native types passed default-value conversion. These two types are local authored
tests and receive no vendor or official upstream credit.104 source-binding DATA
refusals and21 source regressions also passed, including recomputed digests with
wrong JSON numeric types. Original SDK/parser behavior found during development
and all failed capture/configuration attempts remain retained.

All35368 original SDK/derived ordinary files and755 actual fast_io files were
rehashed before and after.194 stopped direct-child kernel `map_files` FD
associations retain actual mapped ELF bytes.38 full-MD dependency files identify
1209 external physical headers, preserved by the separate r9 capture; the final
r10 dependency inventory matches. Seven newly generated default canonical and
independent field-CDR entries remain byte-identical to original executed r23/r25
inputs. Those compatibility checks add no Native, Wasm or QEMU execution credit.

The original hard64GiB/swap0 cgroup stayed in force with zero OOM-counter change.
Each frame used a private hard32MiB tmpfs under that cgroup, downloaded no new
SDK, and left no owned bootstrap process. Eight inner mounts and the outer
tmpfs were cleaned. Maximum waited-child RSS was897998848 bytes, distinct from
total cgroup peak. The [r26 receipt](continuation-20261018-evidence/unitree-installed-source-binding-r26/integration.json)
identifies the verified13.88MB final archive,1723 roles and993 unique objects,
plus prior attempts and the separate5.17MB header/compatibility archive. All165
selected current source files match actual final execution inputs. The resource
record also retains deletion of60.6MiB of two verified duplicate Root archives;
original SDKs, source, logs and binaries were preserved.

Complete pre-RIHS Service/Action protocol admission and broader native SDK
matrices, current whole Runtime/Gateway builds, all requested vendor/model and
ROS distribution routes, official upstream suites, remaining admitted x86/ARM
SDK/QEMU routes, real simulation/controllers/hardware and equivalent Native/Wasm
VTune hardware Hotspots/Microarchitecture remain open. These complete Message
contracts do not establish those separate acceptance results.

## Complete legacy protocol and production Gateway integration r27

The explicit installed-source domain now covers complete old-SDK Service and
Action protocols as well as Messages. It proves actual native request/response
introspection and all five Action links, including complete cancellation/status
dependencies. Its derived RIHS01 identifies the actual legacy graph; no missing
ServiceEvent is projected into the SDK and no baked modern hash is claimed.
Modern hash-capable SDKs retain their existing strict path, and the same bound
legacy converters without the explicitly compiled proof remain refused.
The [English protocol guide](../../sdk/robots/industrial/pre_rihs/PROTOCOLS.md)
describes the actual capture, source, export, bind and Gateway build sequence.

Unified r4 executed23 real native programs through63 serial commands. It checks
43 complete Unitree Messages plus seven payload records in four owned interfaces:
150 distinct production converter/CDR payloads,50 unique default conversions,
105 malformed fixture decodes with unchanged outputs,108 source-binding DATA
refusals and21 source regressions. Nineteen actual SDK-generated Message types
and five legacy Service/Action roots participate in the fixture proof. The
fixtures are local authored tests, not vendor models or official upstream cases.

Final r6 compiled and linked the actual production Gateway on the genuine
Humble amd64 SDK, with successful selected custom-Action, quiet-Node and
deferred-Service compile/link probes. Its5746944-byte ELF refused startup with
no trusted configuration. No flags were asserted in place of actual probes.
The Gateway target and probe now include the reviewed source root needed by
the explicit proof headers. Action admission also returns refusal for absent
codec fields and accepts only its seven implemented operations. No update/debug
operation or new Guest management import was added.

All211 selected current source files match the r6 snapshot. Since r4 only the
two Gateway build/probe include roots and prepare CLI help text changed. Four
actual current rebinds produced the same56 SDK files compiled/executed in r4;
the C++ contracts, data generators and protocol headers remain byte-identical.
The [r27 receipt](continuation-20261018-evidence/legacy-protocol-and-gateway-r27/integration.json)
retains this exact delta, original failures, original owner/source/MD/link
records and actual artifacts.63 full-MD files identify1393 physical external
headers, preserved separately in r7.253 original stopped-child kernel FD
associations retain actual mapped ELF bytes.35368 original SDK/derived files,
755 actual fast_io files and172 owned SDK-generated package files were rehashed
before and after the relevant lanes.

Every compiler/process remained in the original hard64GiB/swap0 cgroup with
serial jobs and zero OOM-counter change. Native contracts peaked at898576384
bytes waited-child RSS. The whole Gateway exceeded the initial2GiB address-space
guard; its4GiB retry succeeded at2464305152 bytes RSS. These are per-lane child
measurements, not aggregate cgroup peaks. Hard private tmpfs budgets were64MiB
for native contracts,256MiB for the Gateway build and16MiB for header retention.
All private frames and owned processes were cleaned; no SDK was downloaded.

Actual ROS network/Guest delivery, lifecycle behavior, all requested vendor,
controller/firmware and distribution matrices, official suites, remaining
admitted x86/ARM SDK/QEMU routes, real simulation/hardware, current whole Runtime
builds and equivalent Native/Wasm VTune measurements remain open work. A
successful production Gateway build is not a ROS transport or robot result.

## r28: actual legacy protocol delivery and endpoint-free production Node

The original r7 snapshot now builds and executes the real Humble amd64 Gateway
with an original native SDK peer over isolated loopback DDS. Five commands
return zero and seven live stages pass: zero-grant graph/admission checks,
Service call/refusal, bidirectional Topics, Service advertise/respond, outgoing
Action feedback/result/cancellation and cross-session refusal, incoming Action
feedback/result, and graceful session/socket/process retirement. This native IPC
client is not a Guest Runtime or robot execution.

The real zero-grant test exposed Humble standard Node's implicit
`/parameter_events` subscription. Production now composes public SDK interfaces
without NodeTimeSource, checks quiet options before NodeBase construction, and
requires the same node type throughout its adapter TUs/libraries. The successful
zero-grant Gateway has no Node-associated publishers, subscriptions or services;
foreign `use_sim_time` events cannot create a `/clock` reader. Debug/update and
ungranted operations, pre-bind/changed/repeated manifests, different peer PIDs,
malformed requests, repeated responses and foreign/retired goals are refused.
No Guest import or default authority was added.

The actual final ELF is5,790,920 bytes, SHA256
`70f3a4a9aa7f236a0aabbb427ffc843b44b9579ae35229fd75ca8d1f5b91110e`.
Real quiet-Node, deferred-Service and custom-Action compile/link probes pass;
Humble TypeDescription remains unavailable. The witness policy explicitly
matches the native Action client's transient-local status channel. Prior
failed volatile/durable pairing remains a failed run, not transport credit.

The hard64GiB/swap0 cgroup and serial compiler jobs remained unchanged. Maximum
waited-child RSS was2,221,686,784 bytes, under the4GiB child address-space limit;
this is not an aggregate cgroup peak. The private frame was256MiB, no OOM event
occurred, and all original actors and mounts were retired.178 kernel map_files
FD associations preserve actual mapped bytes.35368 original SDK/derived files,
755 fast_io files and172 generated package files were checked before/after.
Six full-MD files identify945 external headers, physically preserved in the
separate read/hash-only r8 lane. All217 selected functional source inputs match
current bytes; the only selected-source delta is explanatory witness README
text. Original failed/partial runs and the cleanup correction remain explicit.

The [r28 receipt](continuation-20261018-evidence/legacy-protocol-live-and-quiet-node-r28/integration.json)
and [delivery notes](continuation-20261018-evidence/legacy-protocol-live-and-quiet-node-r28/README.md)
retain exact snapshots, archive/ELF hashes, owner/source/compile/link/MD/mapping
records and scope. Verified selected r6 diagnostics replace its accidental
borrowed-SDK rootfs traversal archive. Linux cleanup removed317,869,515 logical
bytes of owned intermediates; local duplicate/recoverable staging cleanup
removed143,862,101 logical bytes. Final evidence and needed SDKs remain retained.

Actual Guest-to-Gateway execution of the current whole Runtime, other SDKs and
optional composed-Node profiles, every requested vendor/controller/firmware and
ROS1/ROS2 distribution matrix, official suites, remaining admitted x86/ARM
SDK/QEMU routes, credential-enforced DDS, real simulation/hardware and equivalent
Native/Wasm VTune hardware measurements remain open. No such credit is inferred
from this one-SDK/one-architecture native fixture lane.


## FANUC original interface SDK and actual Wasm examples, r29

The captured unmodified fanuc_msgs/rmi_msgs2.4.0 and control_msgs5.10.0 build
with the genuine Humble amd64 SDK. The selected80-definition catalog passes
complete installed protocol/source qualification.105 public payloads pass315
independent native/converter/CDR scenarios and1,275 decoder refusals preserving
destination values;30 source-binding DATA mutations are refused.106 actual
kernel map_files associations,825 full-MD files and1,575 external header paths
are retained. The original2,451-file overlay is preserved exactly.

The corrected C11/C++20 industrial examples produce actual Wasm reactors. Four
modules pass full validation,39 fixed ROS import signatures each, and memory0
read-back at2MiB initial/32MiB maximum. Two no-grant executable witnesses run on
the explicitly pinned existing R110 Runtime, including complete C++ fixture
codec/capacity checks. This does not qualify the current whole Runtime or
positive FANUC Guest/Gateway/driver delivery. The shared example fixes distinct
counted-field assignments, preserves failed-copy outputs, checks fixture array
capacities and supports exception-free WASI with explicit stack/memory limits.

All work stays in the original hard64GiB/swap0 cgroup with serial compilers.
Maximum waited-child RSS is1,244,028,928 native and206,475,264 Guest bytes, not
an aggregate cgroup peak. All OOM counters stay zero and owned actors/mounts
are retired. Four Guest full-MD files additionally retain1,599 actual SDK
headers. All320 native and49 Guest selected source bytes match current files.

See the [r29 delivery](continuation-20261018-evidence/fanuc-original-sdk-and-native-values-r29/README.md)
and [receipt](continuation-20261018-evidence/fanuc-original-sdk-and-native-values-r29/integration.json).
Other vendors and distributions, official suites, positive driver/controller
lifecycles, remaining admitted x86/ARM SDK/QEMU routes and equivalent native/Wasm
VTune hardware-counter Hotspots/Microarchitecture measurement remain open.


## FANUC complete original native production Gateway route, r30

All 66 public interfaces from the captured 80-definition FANUC ROS2 catalog pass
real bidirectional DDS through the production Gateway and independent original
Humble amd64 native peers: 28 Messages, 37 Services and FollowJointTrajectory.
This includes 84 Topic mode pairs, 111 completed Service cycles per direction,
111 malformed pre-native refusals, 111 duplicate Service responses, three Action
goals per direction, owned cancel and six foreign-session goal refusals. The
zero-grant Gateway remains endpoint-free, including after foreign simulation
clock injection. Guest debugger/update administration stays refused.

The actual complete Gateway and 67 native objects build in earlier runs; final
execution reuses their exact ELFs after unchanged production/native source checks
and byte-identical independent value regeneration. The sole native snapshot
delta corrects the fixture policy burst to 16 within depth 16. All 326 selected
final source files match current bytes. The 207 original kernel map_files
associations, 82 full-MD files, 501 owned dependency roles and 1,403 external
header paths close the original compilation/mapping evidence. Every original
archive and header-archive object is independently read back and hashed.

All work remains in the original hard 64 GiB/swap 0 cgroup with serial builds,
4 GiB child address-space limits and a 1 GiB private volume; all memory.events
counters remain zero and owned actors/mounts retire. Maximum waited-child RSS
is 3,619,610,624 bytes for Gateway compilation, 899,600,384 for native compilation
and 182,259,712 for actual live execution, not aggregate cgroup peaks.

See the [r30 delivery](continuation-20261018-evidence/fanuc-complete-production-gateway-r30/README.md)
and [receipt](continuation-20261018-evidence/fanuc-complete-production-gateway-r30/integration.json),
SHA256 2f4db4ceb20885ef96bacc02613a87b2b43a39e9fdaf578ef720d5460323f16d.
Original complete archives remain in the unified Linux scratch location, with
local recovery metadata. Verified duplicate cleanup removes 5,619,061 logical
Linux bytes and 22,143,016 local bytes without removing original evidence.

This is a trusted-native original-interface protocol lane, not Guest-to-Gateway,
full FANUC driver/controller, real simulation/hardware, ROS1, other distributions,
other vendors, QEMU architecture or official upstream suite qualification.
Current whole-Runtime positive delivery and equivalent Native/Wasm VTune
hardware-counter Hotspots/Microarchitecture measurements remain open.

## KUKA original interfaces and complete native Gateway, r31

The captured unmodified kuka_driver_interfaces 2.0.0 package builds with genuine
Humble amd64; all nine original package files match captured Git tree blobs.
The full 26-definition catalog passes installed protocol/source binding and 30
source DATA refusal cases. All seven registered Messages and one Action pass
real production Gateway delivery against independently compiled native DDS peers:
21 bidirectional Topic value pairs, three Action successes per direction, owned
cancel and six foreign-session goal-read/cancel refusals. Ten public payloads
pass 30 complete independent native/converter/CDR cases and 330 decoder refusals
preserving destination values. Zero-capability endpoint absence, foreign clock,
peer PID, manifest, absent alias and debugger/update refusals pass.

The shared binding test now permits a nonempty Service or Action family instead
of incorrectly requiring both. Its original failed assertion is preserved;
the complete corrected native lane is rebuilt and executed. All 25 commands
pass inside the original hard 64 GiB/swap 0 cgroup, with serial compilers,
4 GiB child address spaces, 1 GiB private tmpfs and only-loopback networking.
Maximum waited-child RSS is 2,216,714,240 bytes, not aggregate cgroup peak or a
benchmark. Events remain zero; actors, sockets and mounts retire.

All 371 selected current sources, 35,368 SDK/derived files, 755 fast_io files
and 1,073 native overlay files match. There are 304 original kernel map_files
associations, 74 full-MD files, 250 owned dependency roles and 1,546 external
header paths. All original archive/header objects are read back and hashed.
Cleanup removes 1,424,832 logical duplicate input bytes and two empty frames.
The separate Yaskawa static audit records six historical industrial_msgs Service
sources absent from the original ROS2 CMake generation list; this earns no
native SDK execution credit. No positive Guest, full robot driver/controller,
all-distribution/official-suite/QEMU or VTune completion is claimed.

See [r31 evidence](continuation-20261018-evidence/kuka-complete-production-gateway-r31/README.md).

## KUKA actual Wasm examples and zero-grant execution, r32

The C11 status reactor now exposes all nine complete original KssStatus fields,
sample readiness and a saturating counter, with detailed English embedding
comments. A complete candidate is committed only after canonical validation;
operation and close errors remain separately observable. The actual C WASI
wrapper checks 64 Boolean patterns, 1,024 malformed-payload refusals preserving
sample/counter, saturation and default-denial/closed-session behavior. The
actual C++ wrapper checks complete trajectory fixture codecs/capacity failures
and no-grant denial. Neither test submits a robot command.

Four actual Wasm modules pass full validation and 39 fixed ROS import signatures
each. Two wrappers execute successfully on the pinned existing R110 Runtime;
this is not a current whole-Runtime build or positive Guest/Gateway delivery.
Actual memory metadata is 2 MiB initial/32 MiB maximum for all four modules.
The 51 selected sources, 18,658 original WASI SDK files/18,739 tree entries,
validator and Runtime match their baselines. Four complete application MD files
close over 1,599 actual SDK headers; all original execution/header objects are
read back and hashed. The 17 native bound generation files come from r31.

All 14 commands stay inside the original hard 64 GiB/swap 0 cgroup with one
compiler job/thread, 128 MiB private tmpfs and only-loopback networking.
Maximum waited-child RSS is 180,088,832 bytes, not an aggregate peak or a
benchmark. Events remain zero; actors/mounts retire. Cleanup removes 1,079,323
logical recoverable duplicate bytes and one empty retired frame. No all-vendor,
ROS distribution, official-suite, QEMU or VTune completion is claimed.

See [r32 evidence](continuation-20261018-evidence/kuka-actual-wasm-examples-r32/README.md).

## Yaskawa original interfaces and explicit historical-Service overlay, r33

The 52-definition Yaskawa catalog passes complete installed native source/protocol
binding on genuine Humble amd64. The original MotoROS2 0.2.0 and industrial_msgs
0.7.3 interface packages build from 48 exact captured Git blobs. The original
industrial_msgs package and the separate default-OFF compatibility build export
none of its six historical Services; explicit UWVM_INDUSTRIAL_LEGACY_SERVICES=ON
exports all six complete families. Source definitions remain byte-identical;
CMake/package dependency changes are retained as a diff/receipt. Eight source
preparation and 30 source-binding DATA refusals pass, with no implicit rights.

All 15 Messages, 19 Services and one Action pass production Gateway traffic
against independent native peers: 45 bidirectional Topic pairs, 57 Service cycles
per direction, 57 pre-native malformed-body and 57 duplicate-response refusals,
three Action successes per direction, owned cancel and six foreign-session
read/cancel refusals. Fifty-six payloads pass 168 complete native/converter/CDR
cases and 869 decoder refusals preserving destinations. Zero-grant endpoint,
clock, credential, binding, absent-alias and debugger/update refusal checks pass.

All 33 commands stay inside the original hard 64 GiB/swap 0 cgroup, serial
compilers, 4 GiB child address spaces, 1 GiB private tmpfs and only-loopback
networking. Maximum waited-child RSS is 3,014,995,968 bytes; events remain zero.
There are 324 original kernel map_files associations, 435 full-MD files,
1,076 owned dependency roles and 1,578 external header paths. All archive/header
objects are read back and hashed; all 448 selected sources match. Cleanup removes
1,574,738 logical recoverable duplicate bytes and two empty frames. Three older
original archives are moved intact to Linux with two hash checks before local
duplicate removal, relocating 121,903,961 logical bytes. No evidence is discarded.
No Guest, full driver/Agent/controller/firmware, actual controller control_msgs
revision, official-suite, QEMU or performance qualification is inferred.

See [r33 evidence](continuation-20261018-evidence/yaskawa-complete-production-gateway-r33/README.md).

## Yaskawa actual Wasm examples and default denial, r34

The C ReadSingleIO reactor exposes all four complete response fields and
all-or-refuse diagnostic copying, with detailed English lifecycle comments.
Its actual WASI wrapper checks five diagnostic lengths through full capacity,
signed/unsigned getter values, copy/suffix preservation, insufficient/null
buffers, an invalid saved count, and zero-grant/closed-session behavior. The C++
wrapper checks complete trajectory fixtures, capacity refusal and default denial.

Four actual modules pass full validation and 39 fixed ROS import signatures
each; two wrappers execute on the pinned existing R110 Runtime. All four memories
are 2 MiB initial/32 MiB maximum. The trajectory reactor is byte-identical to the
KUKA r32 artifact, demonstrating compiled standard-interface module reuse across
two selected profiles, with no simultaneous robot communication claim.
All 52 current sources and original WASI SDK/validator/Runtime inputs match.
Four complete application MD files close over 1,599 actual headers; all execution
and header objects are read back. All 14 commands stay inside the hard 64 GiB,
swap 0 cgroup with one compiler job/thread and 128 MiB private tmpfs. Maximum
waited-child RSS is 192,106,496 bytes; events remain zero and actors/mounts retire.
Cleanup removes 1,080,748 logical duplicate bytes and one empty frame.
Current whole-Runtime, positive Guest-to-Gateway, full robot, all-release/vendor,
official-suite, QEMU and equivalent Native/Wasm VTune acceptance remain open.

See [r34 evidence](continuation-20261018-evidence/yaskawa-actual-wasm-examples-r34/README.md).
