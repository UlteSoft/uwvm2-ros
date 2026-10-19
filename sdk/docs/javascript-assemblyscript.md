# JavaScript and AssemblyScript integration boundary

JavaScript and AssemblyScript have similar source syntax but different execution
roles. AssemblyScript is the direct compiled Wasm guest SDK. Its `@external`
declarations emit real imports from `uwvm_ros1_v1` or `uwvm_ros2_v1`. JavaScript
offers reusable canonical codecs and an adapter for an explicitly supplied,
authenticated guest bridge. The additive
[embedded engine](../javascript/guest/README.md) instead compiles pinned QuickJS,
closed ESM source and copied C bindings into an actual Wasm. Its user callbacks
run inside that instance. This is a separately qualified source feature, not a
general host filesystem loader that makes arbitrary `.js` files executable.

Start with each language's raw lesson. It exposes memory index i32 plus handle,
offset and length i64. An output slot is eight little-endian bytes in wasm32 as
well as memory64. Continue with codecs, then typed Node/publisher/subscription
wrappers and the guest-local polling executor. Services and Action servers pair
independently granted advertise/respond handles; no native callback obtains a
guest function pointer. Parameters, graph, clock, logger, TF and lifecycle objects
also retain exact resource grants.

For the original JavaScript host path, supply complete ABI functions, exact shared Wasm memory and an
exclusive scratch reservation. BigInt is mandatory for every i64 value. The
adapter rejects Number narrowing and recreates memory views after calls. The
embedder must preserve uwvm guest identity and authorization; directly implementing
host ROS calls behind this adapter is outside its sandbox claim. External Node
tests of these adapters remain models even when a separate relay is real Wasm.

For JavaScript inside Wasm, `guest.mjs` constructs the same Session/resource
classes over explicit `CopiedTransport`; it does not impersonate Memory. The
C builtin checks real typed arrays, exact Number/BigInt widths, arity and C-owned
scratch shapes, then enters the selected original38/43 imports in the same
principal. Closed module maps, finite heap/jobs/interrupt polls, explicit
command/reactor entrypoints and per-module grants apply. Pure JS vision/motion/
callback libraries compile a backend-zero flavor with no ROS or WASI imports.
Actual final binary import inspection and actual Runtime/native observations
remain mandatory; source models cannot establish either result.

For AssemblyScript, use managed Uint8Array data addresses and UTF-8 conversions,
not string object addresses. Imported functions copy synchronously while managed
arrays remain rooted. Compile with the supplied local trap abort override to avoid
an unwanted `env.abort` import. Do not import DOM, console, random, filesystem or
clock services unless their actual Wasm imports are supported and explicitly
authorized. These details follow the official [external/import concepts](https://www.assemblyscript.org/concepts.html)
and [UTF-8 encoding API](https://www.assemblyscript.org/stdlib/string.html).

Both layers preserve default-deny. Creating SDK objects, encoding bytes or
importing functions grants no ROS rights. Error 90 can leave the length slot
untouched and can consume an event before finding the receive buffer too small.
Initialize the slot to UINT64_MAX and inspect it only when overwritten; neither
language retries or grows a buffer automatically. Close ownership is released
after one attempt because backend failure can follow runtime handle retirement.
An admitted timeout can leave native effects in flight.

The JavaScript test suite passes all 57 positive and 11 negative independent
wire fixtures plus adapter mechanics. Its in-memory fake is explicitly a pure
test, not native ROS or uwvm integration. AssemblyScript contains complete raw
inventory, semantic coverage entries and matching prepared codec fixtures; actual
cgroup compilation, binary import inspection and runtime execution remain pending
for this SDK revision while the authorized Linux host is offline.
