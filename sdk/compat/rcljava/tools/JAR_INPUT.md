# Existing application JARs through the real linear-WASI producer

This SDK entry prepares a private, serial build plan for the existing HLL
`java/jar-input/produce_jar.py`. The producer consumes original JVM class-file
IR with the real TeaVM0.13.1 Maven compile goal and emits `WEBASSEMBLY_WASI`.
It does not run the application on a JVM, use JNI, synthesize a Java compiler,
or reinterpret a WasmGC object pointer as linear memory. The producer remains
in the separate HLL tool distribution; it is an explicit absolute input here.
A standalone SDK distribution must acquire that actual tool and its complete
helper/toolchain closure separately. No copy or replacement producer ships here.

All commands and contracts in this addition are source-authored and unexecuted.
Historical local JVM models, old Guest artifacts and HLL language benchmarks
are separate evidence. A successfully prepared JSON plan proves only that its
explicit input files matched this source observation; no compiler or Runtime
has run. The caller owns all real processes, descendants, deadlines, memory,
backing space and physical retirement when it later executes the plan.

## The concrete SDK entry

Run `tools/plan_jar.py` with actual absolute inputs. Its required roles mirror
the original producer rather than guessing command-line options:

```
PYTHON -I -B SOURCE/sdk/compat/rcljava/tools/plan_jar.py \
  --source SOURCE --producer ACTUAL_HLL_JAR_PRODUCER \
  --jar ACTUAL_APPLICATION_JAR --dependency-jar ACTUAL_DEPENDENCY_JAR \
  --main-class actual.reviewed.DeploymentBootstrap \
  --bootstrap-source ACTUAL_DEPLOYMENT_JAVA_SOURCE \
  --m2 ACTUAL_PATCHED_OFFLINE_M2 \
  --toolchain-receipt ACTUAL_PATCHED_TOOLCHAIN_RECEIPT \
  --toolchain-receipt-sha256 ACTUAL_RECEIPT_SHA256 \
  --patch-root ACTUAL_FOUR_PATCH_DIRECTORY \
  --maven ACTUAL_MAVEN --java-home ACTUAL_NATIVE_JAVA17_HOME \
  --helpers ACTUAL_HLL_SUITE_ROOT --python ACTUAL_NATIVE_PYTHON \
  --taskset ACTUAL_LINUX_TASKSET --wasm-tools ACTUAL_WASM_TOOLS \
  --cpu ACTUAL_AUTHORIZED_CPU --optimization SIMPLE --max-heap-mib 128 \
  --ros-backend 2 --scratch FRESH_PRIVATE_SCRATCH
```

These are role labels, not literal paths or an invitation to evaluate shell
text. Use the emitted `argv` arrays directly in the real owned runner. Omit
`--dependency-jar` when there is none; repeat it for at most62 original explicit
dependencies. Omit `--bootstrap-source` when the original chosen main already
installs the reviewed SDK deployment. Supply `--main-class` for an external
bootstrap; otherwise the original producer can read the actual application
manifest main section. The producer refuses unresolved Manifest `Class-Path`,
multi-release selection, duplicate class names, malformed bytecode and missing
main/dependencies. The planner does not weaken those admissions.

The jar-input-source-v1.json revision4 binds exactly61 current Java library
sources:25 core SDK sources plus36 rcljava facade sources. It includes all five
new owned-service classes and derives the exact61 protected top-level class
names from those actual Source paths. One selected fixed bootstrap is separately
compiled:61 library sources plus1 bootstrap is62. Both tools reject missing,
duplicate, renamed or extra Java Source roles and class names; changing only a
count cannot omit an owned class. Historical52/53 and revision3 56/57 cohorts
remain immutable and carry no current compilation credit.

The actual external producer/helpers, fixed reader, historical81 catalog,
current optional-client3 catalog and all three fixed bootstrap Sources have
explicit current byte/mode/SHA pins. Only the chosen bootstrap is compiled;
observing all three is not three application mains. A Source body/mode or
producer/helper change needs a coherent new contract.

The planner opens ordinary inputs with nonblocking/no-follow FD admission,
streams bounded hashes, checks metadata/name consistency, and reopens all
observed files before writing `plan.json`. It starts no subprocess and imports
no SDK or HLL producer. It creates only fresh scratch and small plan/directories;
it does not copy the Maven cache, compile sources, extract a JAR or execute
application code. Source-root directory contents and files referenced by a
source-map are admitted later by the actual producer, not certified by this
planner merely because their directory/map was named.

## Ordered real build stages

1. Compile the61 explicitly listed SDK/facade sources with the actual Java17
   `javac --release 17 -g -proc:none -implicit:none`, a768MiB Java heap cap,
   one requested compiler worker, an empty sourcepath,
   and the actual offline TeaVM interop/core/classlib JARs. No upstream JNI
   evidence, tests, original example mains or implicit source lookup is included.
2. Package only those new private class files with the actual Java `jar` tool.
3. If selected, compile the separately reviewed deployment bootstrap against
   the original application/dependency JARs and the new SDK JAR, then package
   only the private bootstrap classes. Original application classes are neither
   reconstructed nor recompiled. The original input JAR bytes remain unchanged.
4. Invoke the exact external producer under `taskset --cpu-list CPU`, the
   supplied Python and `-I -B`. Forward all original producer inputs explicitly:
   application/dependency JARs, chosen main, source roots/map, m2, receipt plus
   its actual SHA256, patches, Maven, Java home, helpers, CPU, optimization,
   heap and ROS namespace ceiling. The output is fresh `scratch/translated`.
5. Admit the actual producer receipt (`passed`, all command owners retired,
   and artifact byte extent/hash matching the real file), then run the genuine
   complete `wasm-tools validate` against the produced `application.wasm`.
6. Run the fixed SDK `check_jar_wasm.py` gate on that SAME artifact. It reuses
   the source-pinned existing Wasm parser plus the already admitted ABI JSON,
   checks every actual ROS function's
   exact ordered numeric signature, refuses unknown/wrong-backend ROS imports
   and all nonfunction imports, and permits only actual WASIp1 functions besides
   the selected business ABI. WASIp1 names/signatures still need the real Runtime
   loader's standard admission; this ROS checker does not certify them. It also
   requires exactly one defined, exported,
   unshared wasm32 memory0. It does not prove allocator safety or code validity.

The build plan is executable DATA for an external owner, not a process runner.
A runner must honor every preceding stage result and receipt gate; do not merely
iterate its arrays while ignoring the gate. Capture commands, input reopens,
complete compiler/link dependency closure, output hashes and actual retirement.
The source planner does not invent a compiler build receipt. The new SDK and
bootstrap JARs need their own actual build/ownership/input-association evidence
before use as compiler inputs.

## Original main and explicit deployment

`--bootstrap official-service` compiles the existing
`services/examples/OfficialServiceBootstrap.java` as a separate dependency and
selects its actual main. The application JAR must contain the unchanged official
`org.ros2.rcljava.examples.service.AddTwoIntsService`. That bootstrap installs
an explicit local name/type/QoS/advertise/respond mapping, then calls the original
main directly. It neither installs Host policy nor bypasses a callback lease.
The original handler ignores its request-header argument; the facade does not
fabricate a native writer GUID/timestamp to satisfy header-dependent APIs.
The bootstrap uses the existing finite stdin driver. Root must supply actual
owned input credits and explicit independent server grants for a real run.

`--bootstrap default-denied` instead selects the existing
`examples/DefaultDeniedBootstrap.java`. This is a separate SDK raw-import
Host-denial fixture: its reviewed local mapping attempts a real capability open
with no Host grant, and must observe13. It does NOT execute or qualify the
original application's main. Both existing presets require `--ros-backend 2`.
Neither changes the preserved original example/source/POM or any SDK core file.

For another existing main, supply an actual separately reviewed Java deployment
bootstrap source which statically calls that exact main; `--bootstrap-source`
compiles it against the original application JAR. There is no reflection-based
main lookup, guessed package rename, bytecode rewrite or silent grant preset.
A main that is already suitably bootstrapped can be selected directly. Reachable
unsupported methods/JNI dependencies must fail translation or explicit SDK API
admission; an arbitrary native rcljava binary is not automatically portable.
The full historical API remains required implementation work.

Do not combine this SDK with native rcljava classes or duplicate generated
message classes. The planner rejects any original explicit input JAR containing
one of the61 supplied top-level SDK class names or their nested classes. The
producer independently rejects every duplicate class across its actual full
classpath. It never chooses which duplicate should win and never strips a fat
JAR to hide a conflict. An application that bundles incompatible native classes
needs an explicit upstream build/dependency correction before this route.

## Linear memory, imports and authority

Actual `Ros1Raw`/`Ros2Raw` use Java `int` for the i32 memory index and Java `long`
for all i64 offsets, lengths, handles and correlations. `LinearMemory.Frame`
strongly roots complete arrays through the synchronous import. The actual
`Address.ofData(array).toLong()` intrinsic supplies the array payload's
zero-extended wasm32 offset, with no managed allocation between extraction and
import. A GC reference or object-header address is never a linear offset.
This recipe selects the ordinary linear-WASI backend, not WasmGC or WASI Threads.

The fixed reader is instantiated only from its already admitted sibling source
bytes with the correct SDK file context; a timestamp-matching ambient `.pyc`
is not selected. That controlled code reuse happens only when Root actually
invokes the post-link checker, never while authoring this source cohort. The
already bounded, ordinary-FD ABI bytes remain the signature schema; this gate
never calls the older Reader.check path that would reopen ABI with read_text.
A small schema-injection adapter retains the original exact signature/backend/
name/duplicate/required-inventory decision body and actual Reader.inspect. The
existing Reader, business ABI and Strategy source references stay unchanged.

The memory gate reports the actual minimum/optional maximum pages. Absence of
an encoded maximum is not a proof of bounded execution; the real Runtime must
still enforce its actual finite linear-memory/process budgets. Full Wasm code,
reference validity, data bounds and allocator correctness need independent
validation and actual regressions. A memory declaration does not prove that a
particular object graph is safe. The original producer's heap setting is also
not an observed Native peak RSS.

`--ros-backend` is the producer's import-inventory ceiling, never a capability
grant. The concrete rcljava facade uses ROS2; a separately implemented raw SDK
application can choose ROS1. All discovered ROS names and widths must match the
real ABI. There are38 ROS1 and43 ROS2 business functions. Use `--require-all`
only for the deliberate complete selected-backend raw fixture; ordinary typed
applications can legitimately dead-strip unused imports. The aggregate81
inventory requires the two real backend fixtures, not a false assertion that
one tiny application imports everything. Optional transport namespaces remain
separately admitted; this producer does not silently enable them.

No Guest debugging, source update, policy installer, unrestricted graph/clock/
environment right or NativeNode is added. A local binding maps a real name to
an alias; only the actual Runtime policy can grant that alias to the caller.
Default-denied execution must link/start and reach a real checked import with13;
a loader error, local unmapped-alias refusal or JVM model is not that evidence.
Positive execution needs current Runtime source/admission, exact real grants,
full canonical value checks and independent same-retained Native observations.
Original client availability now has the explicitly selected source-only
[availability facade](../availability/README.md), while positive timed Future.get
still refuses95 and full native wait-set/thread semantics remain required.
No graph-list or empty business call is used as a fake readiness implementation.

## Private tools, storage and evidence

All tools, JARs, m2, receipt, patches and helpers are actual absolute inputs.
Use a fresh scratch outside SDK/tool/m2/helper/source-root inputs. Clear the
plan's explicit ambient Python/JVM/Maven/Node injection variables and apply its
private TMPDIR/XDG cache/config and fixed PATH/JAVA_HOME. Preserve the caller's
original HOME/home/CODEX_HOME; no variable with those names is repurposed.
The producer constructs its own fixed compiler environment and owns the real
Java/Maven process trees through its original helper contract.

The present producer deliberately requires its actual existing Linux64GiB,
zero-swap cgroup and exact single authorized CPU. It requires4GiB free plus the
complete private m2/JAR copy headroom, and finite observed anon budget before
launching its compiler. It currently copies the actual full offline m2 (up to
its own1GiB bound); this SDK planner does not promise a small-cache substitute
or make that cost disappear. Root must budget SDK classes/JARs, compiler outputs
and private cache together BEFORE executing the first stage. A low-space machine
must hold execution. No SDK or tool source directory is modified by the plan.

Preserve five separate evidence levels: source admission; actual owned native
compiler/JAR translation; full binary validation and import/memory admission;
real default-denied or granted Guest execution; independent Native interop and
original official suites. Success at one level supplies no credit for later
levels. Hardware profiling, all releases/vendors/messages and full legacy API
compatibility remain actual implementation/qualification obligations.

The Java17 command options follow the official [javac tool specification](https://docs.oracle.com/en/java/javase/17/docs/specs/man/javac.html);
packaging follows the official [jar tool specification](https://docs.oracle.com/en/java/javase/17/docs/specs/man/jar.html).
The actual original HLL producer/POM source, rather than a guessed generic
TeaVM CLI, determines the versioned lowering invocation. `-J` sets only the
compiler JVM's configured heap/CPU preference; it is not an observed RSS bound
or a substitute for the caller's real process/cgroup controls.

Thirty-one DATA contracts are authored in `tools/tests/test_jar_tools.py`, not
run by this cohort: all fourteen original cases remain byte-exact, including
actual FIFO ABI admission and refusal to delegate to Reader.check's older path
read. Seventeen new optional cases cover closed catalog signatures, paired flags,
no-optin refusal, all3 exact signatures, wrong backend/name/kind/width/duplicates,
independent complete business/optional inventories, fixed-role SHA admission and
nonblocking FIFO refusal. The tiny Core bytes are structural fixtures only.
Existing ordinary-file/ZIP/memory/ordered-width checks remain.
A future owned invocation is `PYTHON -I -B SOURCE/sdk/compat/rcljava/tools/tests/test_jar_tools.py`;
it is a source/structural model, not a compiler, Guest or Native result.

## Explicit optional service-client admission

The historical JAR6 r1/r2 source contracts pinned52 Java sources and producer
`7a2a67e8...`; that producer rejected `uwvm_ros2_client_v1`. Those original
HOLDs, all six paths, before-source inventories, native/Guest evidence and the
FIFO-ABI-reader finding remain separately preserved and are not retagged.
This successor binds the actual current producer32161 bytes,
`fc4dbf08ecc936d0964097974028113ca669b1fde2c844cd9e3fa43ccd225f1e`,
whose source implements a separately reviewed explicit optional catalog pair.
That is source implementation, not successful lowering, native qualification,
threaded scheduling, permission installation or a producer execution result.

The default plan and checker still admit only selected original business ABI
plus WASIp1. To opt in, supply BOTH real arguments to this planner:

```
--ros-client-abi SOURCE/sdk/abi/service-client-v1.json \
--ros-client-abi-sha256 ACTUAL_SERVICE_CLIENT_ABI_SHA256
```

The path must resolve to the actual source-bound catalog role and its bounded
ordinary-FD bytes/mode/SHA must match both the fixed source contract and the
explicit supplied SHA. Supplying only one argument, a changed catalog or another
backend/module/name/type is an error. The actual producer receives these same
real `--ros-client-abi` and `--ros-client-abi-sha256` arguments, plus the existing
`--ros-backend`. No invented optional CLI or configurable arbitrary module
allowlist is used. The SDK post-link gate independently re-admits that catalog,
uses its actual exact3 Core signatures and reopens its held input before report.
Its closed validator is source-identical to the actual producer validator; it
imports no producer module. Old business signatures and actual defined exported
unshared wasm32 memory0 remain mandatory and unchanged.

`--bootstrap official-client-availability` selects the actual separately built
`availability/examples/OriginalClientAvailabilityBootstrap.java`. It requires
ROS2, the explicit optional pair, and the original unchanged
`org/ros2/rcljava/examples/client/AddTwoIntsClient.class` in the supplied JAR.
The bootstrap directly invokes that original main: no source reconstruction,
reflection dispatch, renamed package, replacement business loop or JNI is added.
Use the optional argv in `examples/jar-input-request-v1.json`; its original
server argv remains the default old-business-only route. This preset introduces
no Host grant: independent real call/observer/query/steady aliases and actual
native identity/canonical codec/QoS configuration are still required.

The original `--require-all` option continues to mean all selected ORIGINAL
business imports (38 or43). It does not silently require optional imports.
`--require-all-client` is a separate SDK fixture option requiring the explicit
catalog pair and all three optional functions; it is forwarded to the SDK gate,
not invented as a producer option. Ordinary typed application lowering may
legitimately strip unused optional functions. The checker still requires at
least one selected original business import: optional-only code is not original
business ABI coverage. Any unselected optional namespace is rejected even when
its three signatures look otherwise correct. WASIp1 signature/name validation
remains the actual full validator/Runtime's responsibility.

The Java facade's no-query constructor still uses its original two owners and
refuses95 for unavailable optional behavior. Query and optional elapsed Clock
owners are independent explicit grants. Its finite budget90, physical query
errors, true elapsed timeout, cooperative shutdown and separate async completion
are not rewritten by planning or binary admission. The current optional Host
observer owns an EXTRA retained native query client qualified against the source
configuration, not proof of the old business client's same Native object.

Root must build and bind a NEW coherent61-source SDK JAR, actual bootstrap JAR,
original application JAR, real producer inputs/receipt and resulting Wasm. Run
all actual local31 DATA contracts and Java availability28 models separately,
then full Wasm validation and exact optional gate, then actual default-denied
and granted Guest/Native tests. The old default-denied SDK fixture remains a
business-import Host13 proof; it is not automatically query13 proof. A separate
actual deployment must grant call/observer but deny query to reach that genuine
independent refusal. Keep all previous successful/failing artifacts immutable;
source, compiler, binary, Guest and Native evidence remain distinct.

## Revision4 owned classes and separate owned-service route

This compatibility planner always passes --ros-rmw-service-enabled 0 to the
actual source-pinned producer. Its final signature gate refuses
uwvm_ros2_rmw_srv_v1, even with the original business imports and explicit
client-availability3 pair. Compiling library classes is not a grant, import
selection or proof that a compiler strips unused Native methods. If reachable
owned calls survive lowering, this route must refuse the artifact. No generic
module allowance overrides that ROS namespace decision.

For new owned-service applications, use the separate
[Java owned-service recipe](../../../java/owned-service/README.md). It selects
backend2 and the complete explicit raw12 catalog, exact full-import policy,
inspector and checker Source pins through the actual producer's owned-service
options. It still needs genuine Session/capability caller grants and independent
raw-owner/Native cleanup evidence. This compatibility planner does not expose
or silently forward those options. Reusing its61-class SDK library needs a
separately reviewed build/producer input association.

An original application JAR is JVM bytecode input, not a native rcljava JNI port
or complete upstream API implementation. Reachable unsupported JNI, reflection,
OS/thread or facade methods must fail or be explicitly source-ported. An
existing .so/.dll cannot load inside this Wasm principal: port its required
implementation to admitted Wasm imports or retain it in an explicit trusted
Native gateway. Rebuild a Native .a for the selected Wasm toolchain/link contract;
renaming it does not change its target. Host policy still grants aliases and
SDK objects/method names grant nothing. The finite facade is not every
rcljava/rcl/rclcpp API.

The external keeper must bind actual entry/interpreter/stdlib and every helper
before first candidate instruction. The original producer's ordinary helper
imports and sampled process tracking are historical Source behavior, not proof
of exact executed helper bytes or today's process/output ownership. -B does not
prevent ambient .pyc reads; later self-hash does not authenticate an already
executed entry. Preserve actual source/artifact associations and enforce real
cgroup, PIPE bounded-drain, time/RSS/backing, cancellation and join contracts.

The unexecuted tools/tests/test_jar_source_revision4.py uses actual-tool closure
fixtures for missing/duplicate/renamed owned sources, missing/extra classes,
old revision refusal, actual Source body mismatch, raw12 refusal and optional3
controls. It loads captured pinned planner/checker bytes; entry/interpreter and
stdlib remain external keeper trust. Older31 tests are preserved, and their
ordinary test-loader path likewise needs keeper Source/cache admission.

Original JAR SourceFile/LineNumberTable/LocalVariableTable attributes and real
caller-supplied source maps remain the original debug inputs. DWARF section
presence and class/method counts do not prove breakpoints, exact typed locals,
full64-bit values, unwinding or ROS callback debugging. Require actual admitted
SIMPLE/FULL builds, line/function breakpoints, local/stack/step checks,
trap/exception cleanup and ROS denied/granted callback/service traces on the
coordinated Linux64GiB host. This Source contract asserts no DBG/cache/QEMU/VTune
result.

## Final artifact signing and per-module VM admission

WSEA is an explicit opt-in contract documented in the current
[secure execution guide](../../../../documents/security/wsea.md). No
cryptographic operation has run for this JAR cohort. Complete compilation,
linking, optimization, stripping and every embedded/custom/DWARF section change
before signing the exact final Wasm bytes. Run the fixed WSEA tool's detached
signing and verification flow after the final artifact is frozen. The reported
command families are cosign wasm secure policy-init, policy-update, sign, verify and fetch;
use that tool's actual versioned argument specification rather than guessing
key, policy or network options from this recipe.

The detached files are .sig.cose, .signer.pem and .crl.pem. Their presence is
not verification or trust: the actual verifier must enforce the operator's
protected policy, signature, certificate and revocation rules. Keep policy and
private signing authority outside Guest-writable inputs. A policy with exactly
one module rule binds its unique identity through this VM option:

    --secure module FINAL_WASM PROTECTED_POLICY
    --secure module DEPENDENCY_WASM DEPENDENCY_PROTECTED_POLICY

A shared policy containing several module rules requires an explicit expected
identity for each selected path:

    --secure named-module FINAL_WASM SHARED_PROTECTED_POLICY APPLICATION_ID
    --secure named-module DEPENDENCY_WASM SHARED_PROTECTED_POLICY DEPENDENCY_ID

A valid signature belonging to another authorized identity does not admit the
application at this path. Repeat selections only with the intended identity and
protected policy. This records the WSEA team's current source interface; the
real binary and this Java application still need their own execution evidence.

The real admitted VM executable and its complete launch arguments are separate
owned inputs. Admit every final/preloaded module under its own explicit policy;
one application's signature does not admit another module. A post-sign byte
change, including debug/custom-section changes, requires a new signature and
verification. External source/debug sidecars need their own recorded identity
and association; they are not signed merely because a Wasm signature exists.

The signed capability mask must fit the selected local policy rule; a wider
signed mask is refused rather than silently reduced. WSEA import-ceiling bits
are1 basic WASI,2 fd/path/poll including stdio,4 sockets,8 ROS1,16 ROS2 and32
other Native imports. Bits2 and4 also require bit1, so WASI stdout normally
requires mask3. Actual imports and import-reset destinations must fit that
signed ceiling. These bits never create a Runtime ROS alias, grant
service/topic authority, replace per-operation checks or bypass revocation.
Likewise the optional-client3 pair and this61-class library confer no rights;
compatibility raw12 remains explicitly off.

Cache hits, warm starts, checkpoint restoration and resolved module graphs must
preserve the same final-byte/policy signature admission. This language recipe
states that integration requirement; it does not certify the shared VM gate,
cryptography, cache or ROS enforcement. Keep their exact-source tests and real
Guest/Native/debug evidence separate.

Secure function replacement and compiled-source replacement currently report
`unsupported_mode`; use a fresh admitted load. An already running instance has
no background revocation/automatic stop mechanism in this increment.
