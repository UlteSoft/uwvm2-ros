# Complete Java records on TeaVM linear WASI

`ManagedContract.java` runs fourteen complete-record methods against the real
generated `BoundedRecords` and SDK `Codecs`. Its successful command output is
exactly:

```text
managed Java record cases=14 checks=47 native/ROS=false
```

It covers an independent scalar golden, all uint64 bits, signed zero,
narrow-range and nonfinite refusal, strict text, malformed/truncated/trailing
input, nested/fixed/sequence arrays, independent defaults, complete empty
service/action components, normalized ROS1 primitives, input ownership and
native field spellings. It invokes no ROS import. A host JVM test is not a
substitute for compiling and executing this Wasm command.

## Toolchain and isolation

Use the admitted Maven 3.9.11, TeaVM 0.13.1 linear `WEBASSEMBLY_WASI` class
library and the existing private stdin/DWARF compiler patch. The qualified
patched `teavm-core` JAR has SHA256
`04f3dc950c662aa6fc1318c458a1791b2c09911f234fa9711cb2ffdd76c6ff82`.
Verify its original source/patch receipt and replaced class hashes; copying an
unrelated JAR with the same version is insufficient. Use a fresh private M2
copy and offline builds. Keep the original repository unchanged.

The tested Linux OpenJDK 21 image contains `jdk.compiler`, but lacks a `javac`
launcher and `lib/ct.sym`. Maven invokes the compiler module inside its JVM.
These POMs explicitly use Java 17 **source and classfile target** flags, as the
existing producer does. They do not claim the JDK 17 API filtering supplied by
`--release 17`; that option failed on this minimal image. The actual classfiles
must have major version 61. TeaVM's strict class-library resolution is a separate
gate. In particular, the linear class library lacks `Integer.toUnsignedLong`;
the SDK's float32 writer widens and masks the raw int bits instead, retaining
negative zero without that unavailable method.

Perform every generation, build, validation and execution only on SSH Linux
inside the existing 64 GiB, swap-disabled cgroup. Check its identity, current
limits and memory events before and after. An external owner must supervise the
whole process tree and bound wall time, memory, output and temporary storage.
The POM is not a resource supervisor. Use serial jobs, a fresh HOME/TMPDIR and
bounded `MAVEN_OPTS`, for example the qualified 1 GiB heap/one-processor/SerialGC
profile. Native JDK, loader and system libraries remain trusted Host inputs.

## Build the complete fixture

Start from the repository root. Set `work` to a fresh owned directory outside
the checkout, `maven` to the admitted Maven launcher and `m2` to its fresh
private patched repository. Set JAVA_HOME to the admitted compiler image.

```sh
work=/absolute/path/to/fresh-owned-java-work
maven=/absolute/path/to/admitted/maven/bin/mvn
m2=/absolute/path/to/fresh-private-patched-m2
mkdir -p "$work/project/sdk" "$work/project/tests/ros_custom_types/managed-records"
cp -R sdk/java "$work/project/sdk/java"
cp tests/ros_custom_types/managed-records/ManagedContract.java \
  tests/ros_custom_types/managed-records/pom.xml \
  "$work/project/tests/ros_custom_types/managed-records/"
python3 -I -B sdk/custom-types/managed_profile.py \
  tests/ros_custom_types/managed-records/fixture/catalog.preparation.json \
  --language java --out "$work/generated"
cp -R "$work/generated" "$work/project/tests/ros_custom_types/managed-records/generated"
"$maven" --offline -B --no-transfer-progress \
  -Dmaven.repo.local="$m2" -Dteavm.optimization=SIMPLE \
  -f "$work/project/tests/ros_custom_types/managed-records/pom.xml" process-classes
```

The output is `target/wasm/record-contract-java.wasm` beside that staged POM.
Repeat in a separate fresh project for `-Dteavm.optimization=FULL`. Both profiles
retain embedded producer debug information; optimization levels are not
language debugger qualification or xmake Runtime build modes.

Validate the actual Wasm with `wasm-tools validate`, retain its WAT and check
its imports before running. This fixture imports only WASIp1 `clock_time_get`,
`args_sizes_get`, `args_get` and `fd_write`. Clock access is part of the actual
TeaVM runtime; it must be supported rather than replaced by a stub. No ROS,
environment, preopen, filesystem, browser or JNI import is needed.

```sh
uwvm -Rint -WFE-gc -WFE-exceptions -WFE-tail-call \
  -WFE-function-references -WFE-reference-types -WFE-simd \
  -WFE-relaxed-simd -WFE-extended-const -WFE-multi-memory \
  -WFE-memory64 -WFE-table64 --wasip1-global-noinherit-system-environment \
  --wasm-set-main-module-name java_records \
  --run /absolute/path/to/record-contract-java.wasm
```

Require the complete exact line above, zero exit and empty stderr. The existing
command entry must pass zero Java application arguments; an extra argument
would not be an equivalent run for the JointState lesson below.

For a negative control, make a separate fresh fixture project and change only
`hex("01ff80ffff0080` to `hex("00ff80ffff0080` in its **staged** test copy. Keep
the original test unchanged. Build both optimization levels. The qualified
Runtime reports the scalar golden assertion in the Java/Wasm stack, emits no
success line and terminates with `catch unreachable`/SIGILL. Its producer does
not print the AssertionError message; do not require a fabricated message.
Core dumps must be disabled by the process owner. This checks failed assertions,
not recovery from a Guest trap inside a shared Runtime process.

## Build the actual ROS1/ROS2 JointState applications

Use the complete lesson in
`sdk/robots/vendors/examples/joint_state/java`. For each ROS version, mode and
optimization level, create a separate coherent staged SDK containing:

1. The exact `sdk/java` tree and lesson Java directory.
2. The complete generated bundle from the matching official-standard ROS1 or
   ROS2 catalog at `sdk/custom-types/generated-official-standard-managed`.
3. A byte-exact copy of `SelectedRos1.java` or `SelectedRos2.java` named
   `Selected.java`.
4. A byte-exact copy of `ModeDefault.java` or `ModePositive.java` named
   `LessonMode.java`.

Invoke the same offline Maven command with the lesson's staged `pom.xml` and
explicit `-Dteavm.optimization=SIMPLE` or `FULL`. The result is
`target/wasm/joint-state-java.wasm`. The explicit include set omits unselected
source files. Inspect all 38 ROS1 or 43 ROS2 numeric import signatures against
`sdk/abi/abi-v1.json`; the other backend must be absent. Importing the selected
ABI does not grant its operations. Positive builds additionally need actual
WASIp1 `fd_read` and `proc_exit`; default builds need the four fixture imports.

Mode0 checks ABI value 1, actual open error 13 with no returned owner, checked
Session cleanup and `PASS_JOINT_STATE_DEFAULT_DENIED`. Run it only on a product
containing the selected ROS imports. The qualified default-disabled runs use
the ROS interpreter without ROS enablement or grant arguments. They are Guest
denial checks; native endpoint/queue and enabled-policy audits remain separate.

Mode1 is an explicitly selected subscription application. Its builds validate
complete Header/time, names, position, empty velocity and effort against the
lesson fixture and use real stdin poll credits. Its successful compilation
does not prove middleware delivery. A native-bound catalog, real publisher,
matching Gateway/type identity and explicit `joint-state` subscription grant
are still required before a positive delivery run. Preparation catalogs used
for codec/build tests have null native identities and confer no native trust.

The [dated Linux report](../../../test/0050.high_level_languages/results/native-budget/language-java-records-jointstate-20261019/README.md)
records the actual compiler/product/source identities, positive codec runs,
failure controls, default-disabled outcomes and retained failed attempts.
