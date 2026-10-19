# Linux-only source/build/qualification recipe

All commands below describe future work. None was run for this packet. Use the
already authorized existing64GiB cgroup, real cgroup inode/membership binding,
bounded pipe draining, RSS/time/output ceilings and descendant retirement.
An outer owner must admit each executed entry byte stream, interpreter, stdlib,
JDK/Maven/compiler/extensions and toolchain before execution. The historical
JAR producer's fixed cgroup check is additional legacy functionality; it is not
a substitute for the current owner/keeper protocol. Do not run these commands
on the macOS review machine. Do not change the cgroup or download dependencies.

1. Stage one fresh private coherent source tree from the reviewed exact patch.
   Recheck every current preimage/absent path and all foundation pins. Keep the
   public tree and all historical artifacts unchanged. The modified
   `managed_profile.py` Java branch and `Codecs.Writer.position()` must belong to
   this same source tree. `bounded_profile.py` is the independently reviewed
  238722-byte40e30d54… revision; this packet does not replace it.
2. Admit the exact original control catalog and all25 original declaration
   files. Prepare the separate canonical C/C++ source revision with the real
   existing `custom_types.py` original-control guest-only route. That revision
   must contain the seven unchanged C/C++ products plus actual `generation.json`
   with the same parent/bounded SHAs and exact source_inputs. Do not fabricate
   this proof by writing hashes into a hand-made JSON file.
3. Run the new explicit Java adapter under the owned Linux Python entry:

```text
python3 -I -S sdk/java/owned-service/generate_control_java.py
  --source-root /absolute/private/source
  --catalog /absolute/private/source/sdk/rmw_wasm/default_control/catalog.preparation.json
  --canonical /absolute/genuine/canonical-c-cpp-source-revision
  --out /absolute/fresh/control-java
  --parent-sha256 <actual reviewed custom_types.py SHA256>
  --bounded-sha256 <actual reviewed bounded_profile.py SHA256>
  --managed-sha256 <actual reviewed modified managed_profile.py SHA256>
  --codec-sha256 <actual reviewed Codec.java SHA256>
  --codecs-sha256 <actual reviewed modified Codecs.java SHA256>
  --owned-service-sha256 <actual reviewed OwnedService.java SHA256>
  --operator-sha256 <actual reviewed generate_control_java.py SHA256>
  --catalog-sha256 <actual reviewed catalog exact-byte SHA256>
  --canonical-proof-sha256 <genuine canonical generation.json exact-byte SHA256>
```

`-I -S` is interpreter startup hygiene, not source authentication, a sandbox or
proof of no ambient stdlib/extension cache. The owner must execute admitted entry
bytes. Local helper source is executed from the exact captured bytes and fresh
private module identities, with own sys.modules registrations cleaned afterward.
This adapter never calls managed_profile's legacy `_trusted_module` or emits a
native registry. Retain the source proof; products are still uncompiled source.

4. Form an explicit source list of all SDK Java `.java` files, generated
   `org/uwvm/roswasm/generated/BoundedRecords.java`, `ControlServices.java` and
   the chosen example/test sources. Keep each list/path/SHA in the build receipt.
   Compile with the actual admitted JDK17 and pinned local
   `teavm-interop-0.13.1.jar`; package the real compiled classes in an application
   JAR. Do not make a fake JAR or reconstruct classes from a decompiler listing.
   The existing `sdk/java/pom.xml` describes the real library dependencies, and
   the JAR producer translates that JAR without running its main on a JVM.
   JDK/Maven commands require the same external owner/resource gates as all
   other tools; plain unowned subprocess execution is not qualification.
5. Read the actual future emitter proof and compile/run the labelled local
   `OwnedServiceModel`, `ControlRecordsContract` and binary-model source fixtures
   on Linux. Record each stage separately. Java local models use a fake transport
   deliberately; they cannot satisfy Guest/native positive acceptance. Test the
   default non-control Java emitter for exact output preservation and all other
   optional-language outputs against their existing independent pins. This
   preservation has only been examined as source here, not executed/compared.
6. Translate a real application JAR using the already pinned/owned TeaVM0.13.1
   toolchain and its existing DWARF/ordered/typed/environ patch receipt:

```text
python3 -I -S test/0050.high_level_languages/java/jar-input/produce_jar.py
  --jar /absolute/real/application.jar
  --main-class org.uwvm.roswasm.examples.owned.TypeQueryApplication
  --source-root /absolute/private/source/sdk/java/src/main/java
  --source-root /absolute/fresh/control-java
  --source-map /absolute/reviewed/application-source-map.json
  --m2 /absolute/existing/pinned/local/m2
  --toolchain-receipt /absolute/genuine/teavm-toolchain-receipt.json
  --toolchain-receipt-sha256 <actual receipt SHA256>
  --patch-root /absolute/pinned/teavm/patches
  --maven /absolute/admitted/maven
  --java-home /absolute/admitted/jdk17
  --helpers /absolute/admitted/high-level-suite
  --cpu <current authorized CPU> --optimization SIMPLE --max-heap-mib 128
  --ros-backend 2 --ros-rmw-service-enabled 1
  --ros-rmw-source-root /absolute/private/source
  --ros-rmw-service-abi /absolute/private/source/sdk/abi/rmw-service-v1.json
  --ros-rmw-service-abi-sha256 <exact reviewed catalog SHA256>
  --ros-rmw-imports /absolute/reviewed/exact-import-policy.json
  --ros-rmw-imports-sha256 <exact policy SHA256>
  --ros-rmw-inspector-sha256 <actual reviewed check_wasm_imports.py SHA256>
  --ros-rmw-checker-sha256 <actual reviewed owned-service/check_imports.py SHA256>
  --out /absolute/fresh/translation
```

The application source map pins the real flat example files to their actual
package-relative paths, for example keys
`org/uwvm/roswasm/examples/owned/CallerArguments.java`,
`org/uwvm/roswasm/examples/owned/TypeQueryApplication.java` and
`org/uwvm/roswasm/examples/owned/ParameterApplication.java` map to their actual
absolute files under `sdk/java/owned-service/examples/`. Do not invent source
lines or reconstruct bytecode debug attributes.

The exact Maven goal remains `org.teavm:teavm-maven-plugin:0.13.1:compile`, with
`WEBASSEMBLY_WASI`. There is no guessed TeaVM CLI API or WasmGC target. Existing
optional-client3 users additionally pass the original `--ros-client-abi` and
SHA arguments. Owned12 uses an independent defaultoff gate and does not widen
that original optional catalog. Missing or wrong-type owned imports fail.

The reviewed full import policy is this exact closed schema:

```json
{"kind":"java-linear-wasi-exact-imports-v1","imports":[
  {"module":"uwvm_ros2_rmw_srv_v1","name":"abi_version","kind":0,"params":[],"results":["i32"]}
]}
```

The single row above illustrates syntax only; it is deliberately incomplete and
will fail. Supply every actual intended optional12 row plus the selected
business/client and exact WASI rows, each exactly once. Do not copy a produced
inventory into its own expected oracle and claim independent approval. Unknown,
duplicate, missing, wrong backend/kind/params/results all fail. Non-WASI non-ROS
modules are refused. Full code validation and actual provider linking remain
separate; policy equality alone does not prove the WASI provider contract.

7. Run a real full Wasm validator, preserve its owned process receipt and actual
   entire binary inventory, then actual Guest/ROS tests under the real Runtime
   caller and deployed native Gateway. For `TypeQueryApplication`, supply:

```text
callAlias observerAlias availabilityAliasOrDash requestQos80Hex responseQos80Hex pollBudget
original/package/msg/Type actual-RIHS01-type-hash 0-or-1
```

For `ParameterApplication`, main class is
`org.uwvm.roswasm.examples.owned.ParameterApplication`; common first six arguments
are identical and remaining arguments are actual requested parameter names.
Actual service targets, type identities, native QoS, instance, namespace and
quotas are trusted deployment rows, not hard-coded by either application.
Availability uses wait0 once; finite polling is1..4096 exact attempts, without
hidden graph queries or sleeps. An idle/quota error or exhausted poll budget
is not a successful service response. Record request, genuine response, exact
same-take metadata, Native API evidence and separate cleanup outcomes.

Both entries now allocate `OwnedApplicationFailure` before their first open and
throw that same envelope after finally when body or cleanup fails. Its primary
field/cause retains the exact original Throwable; native/raw outcomes and full
owner retirement facts remain independent. There is no print-only cleanup
failure or per-error wrapper allocation on the exit path. Include the new SDK
class in the explicit source inventory from step4. This is a Source correction,
not an observed TeaVM/OS exit result.

Run `ApplicationFailureContract` only as a labelled local model, compiled with
the unchanged `OwnedServiceModel` source. It checks successful cleanup, cleanup
failure after a successful body, an exact body primary, all native/raw failures,
detached snapshot mutation, diagnostics failure and repeated finish without
replay. It does not execute the real main methods or establish native behavior.

For actual entry qualification, compile `ApplicationFailureGuest.java` with the
two real examples and their complete generated records. Use its main class
`org.uwvm.roswasm.examples.owned.ApplicationFailureGuest` and arguments
`type-query|parameter expected-primary0|1 REAL_APPLICATION_ARGUMENTS...`.
The external real provider/controller must independently cause cleanup failure:
test both a successful body with failed cleanup and a real body failure plus
cleanup failure. Require every expected actual outcome and the complete framed
diagnostic marker, then verify the same envelope is uncaught and the actual
Guest/process exits nonzero. Nonzero alone, a model, an idle response, a missing
marker or a diagnostic failure is not qualification. The fixture adds no fake
Host error and preserves any reporting Throwable in the same envelope.
The source-map key for that flat fixture file is
`org/uwvm/roswasm/examples/owned/ApplicationFailureGuest.java`; map it to the
actual `sdk/java/owned-service/tests/ApplicationFailureGuest.java` Source path.

8. Exercise permitted and denied client/server pairs, all seven complete control
   services, zero-body components, TypeSource invalidUTF8/NUL bytes, full-width
   handles/tickets/nonces/sequences, foreign/stale/swap/native disconnect/revoke,
   actual resource-error/closed controls, late abandoned responses, malformed
   canonical values and unknown ACK/send outcomes. A server uses
   `group.server(definition,advertiseAlias,responderAlias,q0,q1)`,
   `takeRequest()` then exactly one `respond(delivery.ticket,completeResponse)`.
   Decode and prepare all response data under the same group lease; original
   rcljava callback/executor semantics are not inferred from this finite loop.
   Preserve all denied observations rather than skipping native cases.
9. Qualify actual Java DBG, ROS DBG and code-cache security separately. Run real
   CPU feature/cache identity and cold/store/warm/hit/refusal combinations. Run
   VTune hardware timing/hotspot/microarchitecture with native and runtime
   comparisons, plus required x86_64/aarch64 and applicable QEMU platforms.
   Source review supplies no timing, functional or architecture credit.

Retain originals and bounded receipts, clean only private disposable artifacts
that this task created when space is needed, and never delete user disk images,
SDK/tool installations, original source evidence or other agents' work.
