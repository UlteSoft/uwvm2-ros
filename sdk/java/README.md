# Java ROS Wasm SDK

Use TeaVM0.13.1 `WEBASSEMBLY_WASI`, its linear heap and the numeric
`org.teavm.interop.Import`/`Address` APIs. Java `int` represents i32 and `long`
represents i64. All long bit patterns remain valid for opaque unsigned handles;
do not reject a handle merely because its Java value is negative.

This route is separate from TeaVM WasmGC. A GC reference is not a linear-memory
offset. No browser imports, JavaScript relay, JNI library or host JVM participates
in the Wasm guest. The generic producer and language debugger work belong to
`test/0050.high_level_languages/java`; the ROS SDK belongs here.

## Exact imports, then buffer ownership

`Ros1Raw` contains all38 ROS1 declarations and `Ros2Raw` contains all43 ROS2
declarations from [the exact ABI schema](../abi/abi-v1.json). They use i32 memory
indices, i64 offsets/lengths/handles and i32 returns. `abi_version` returns a
version value; other imports return errno. The generated sources and transports
are reproduced by `sdk/tools/generate_managed_raw.py --out /tmp/managed-review`.

Select `new Ros1Transport()` or `new Ros2Transport()` explicitly. Reachable
backend classes stay separate so a ROS1-only guest does not acquire an implicit
ROS2 import dependency. Missing backends never fall back to each other.

`LinearMemory` retains input/output/slot arrays in a strongly rooted frame for
the complete synchronous call, passes only `Address.ofData(bytes).toLong()`
offsets, and returns owned copies. The actual TeaVM intrinsic zero-extends the
wasm32 address to i64. Casting `address.toInt()` directly to long would instead
sign-extend addresses with bit31 set. `Address.pin` is a no-op on this backend
and is intentionally not used as a lifetime guarantee. Reentrancy is rejected
before another call can reuse storage.

Only output lengths use an all-ones sentinel. Open/submit value slots use zero
as the invalid sentinel and preserve all64 valid bits. Error90 exposes a required
size only if actually written. Error110 never causes replay. If constructing a
successful open result fails, the transport closes that handle once.
`lastOpenCleanup()` exposes already-reserved primary/attempted/code/exception
storage for that failure path. It resets before the next open. Optional Java
suppressed exceptions cannot replace the primary or cause a second close;
the numeric/exception fields retain the cleanup outcome independently.

## Typed resources and checked shutdown

```java
try (Session session = new Session(new Ros2Transport(), 16)) {
    session.checkAbi();
    Node.Publisher<String> publisher =
        new Node(session).createPublisher(Codecs.STRING, "publish");
    publisher.publish("robot status").checked();
}
```

The Host must separately grant `publish` for its exact resource/native type/QoS.
Session construction, Java Node objects and codecs grant nothing. Cleanup
reserves result storage before effects, attempts every live close in reverse
order and retains errors/exceptions. A close error retires the local handle and
is never retried. Callback leases prevent closing their active owner/session.

`Node` also provides typed subscriptions, bounded round-robin execution,
service clients and servers, exact-ID responses and outgoing Action ownership.
Other ABI operation families are reached with an explicit operation alias and
`Capability.invoke`. A source/observer pair is separately owned; no callback runs
on a native ROS thread. Codecs use canonical ROS Wasm bytes, not CDR or Java
object serialization. Decode rejects malformed UTF-8, trailing bytes, nonfinite
Twist values and truncated/full-width integer fields.

## Build and run the real examples

The [example project](../examples/java/pom.xml) compiles SDK sources and chooses
an application main without rewriting SDK code. Make a `/tmp` staging copy
preserving the SDK layout, use a private Maven repository, one Maven process
and a bounded `MAVEN_OPTS` heap. Choose the producer's `WEBASSEMBLY_WASI` target.
The [default-denied example](../examples/java/DefaultDenied.java) requires actual
imports and error13, rather than treating a link/startup failure as denial.
The [topic example](../examples/java/TypedTopics.java) needs two independently
granted aliases, a native observer and bounded stdin scheduling credits.

Compiled Java classes are not a Wasm result. Validate and inspect the final
Wasm's actual import types with `sdk/tools/check_wasm_imports.py`, then execute
the actual denied/granted Runtime/native peer cases. JVM-only
`org.uwvm.roswasm.ContractTest` verifies local wire and ownership semantics only;
it never proves ROS delivery or Java Wasm compatibility.

## Actual complete-record and JointState checkpoint

The [complete-record WASI recipe](../../tests/ros_custom_types/managed-records/README_JAVA_WASM.md)
now has actual SSH Linux receipts. Twelve compiled applications cover the
complete fixture plus ROS1/ROS2 JointState, default/positive application modes
and TeaVM `SIMPLE`/`FULL` optimization. The fixture executes all fourteen methods
and 47 checks on each of six frozen Release/Debug Runtime products, completing
168 methods and 564 checks across twelve runs. Twelve deliberately incorrect
golden-byte controls report the real scalar assertion stack and unreachable
SIGILL without success output. Eight actual default-disabled JointState
outcomes execute on the ROS products; positive subscription builds have not
yet established native JointState delivery.

Actual compilation exposed the linear class library's missing
`Integer.toUnsignedLong`. The float32 writer now widens and masks the raw int
bits instead. The independent scalar golden and signed-zero cases execute
this implementation inside UWVM. The admitted minimal OpenJDK image also lacks
`ct.sym`: the tested POMs select Java17 source/classfile targets, separately
verified as major61, and do not claim `--release17` API filtering. Other SDK
projects with their own release setting still need their stated compiler.

The [dated report](../../test/0050.high_level_languages/results/native-budget/language-java-records-jointstate-20261019/README.md)
retains exact source/tool/product/import identities and earlier failed attempts.
These are functional interpreter results, separate from native delivery,
complete rcljava compatibility, managed debugging and performance qualification.

## Existing rcljava products

The current capability API is the underlying implementation for ongoing rcljava
source compatibility work. It does not yet provide the complete
`org.ros2.rcljava` package, generated message API, parameter transaction/callback
semantics, native wait sets or every historical product interface. Those remain
required work and must be checked against pinned upstream sources and tests.
See [the compatibility ledger](../compatibility/rcl/README.md). Keeping the
boundary explicit avoids silently accepting an old API while changing its
behavior. No guest SDK API controls updates, debugger settings or Host policy.
