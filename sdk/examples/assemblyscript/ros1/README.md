# ROS 1 AssemblyScript lessons

These eight entries provide the same raw-to-codec-to-Node-to-resource learning path
as the ROS 2 lessons in the parent directory. They use the actual
`uwvm_ros1_v1` imports through `assemblyscript/ros1` and do not require a build
script to rewrite SDK source. Compile one entry at a time from repository root:

```sh
asc sdk/examples/assemblyscript/ros1/03-node.ts \
  --outFile /tmp/roswasm-as-ros1-node.wasm \
  --runtime incremental \
  --use abort=sdk/assemblyscript/api/abortGuest
```

The local abort override avoids `env.abort`. Inspect the compiled imports and
their exact i32/i64 signatures before running under a runtime profile that enables
ROS 1. Actual compilation and execution of these SDK entries remain pending in
the authorized Linux cgroup; this is a prepared recipe, not a PASS record.

`01-raw.ts` opens one explicitly granted alias and closes it once. `02-codecs.ts`
publishes a canonical String. `03-node.ts` polls a typed subscription with a
guest-local callback. `04-resources.ts` calls Trigger, reads a configured ROS 1
Int32 parameter, and reads an explicitly authorized ROS clock. It does not
substitute a ROS 2 Int64 parameter type or silently narrow 64-bit native values.

Configure `pub`, `sub`, `trigger-call`, `counter-get` and
`clock-ros` as needed, using the exact supported ROS 1 type identities and limits.
Those names are policy labels, not implicit native resources. Default ROS policy
grants none of these aliases. Every entry exports `_start` and optional diagnostic
values; they are not automatically printed and require no additional log grant.

Lessons5..8 are the complete service/Action server, actual ROS1 TF/graph/wall
clock, pure local scheduling, and Action/BoolArray parameter entries. All use
checked cleanup. ROS2 events/lifecycle are unavailable in ROS1 ABI v1 and are
not emulated. See the [parent guide](../README.md) for native peer prerequisites,
CLI limit schemas, default13, bounded loops and the required lastStatus check.
Compile-only retention fixtures are separate from these real application entries.
