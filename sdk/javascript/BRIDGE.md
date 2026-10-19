# Concrete Wasm bridge source

`bridge.c` exports every actual ABI function for **one selected backend** and
reserves an input/8-byte-slot/output scratch array. It uses the ordinary C guest
imports and adds no native ROS library, network host implementation, file,
clock, thread, automatic open or permission grant. `bridge.mjs` maps the actual
exports to the SDK's complete raw bridge table.

A representative freestanding Clang recipe is:

```sh
clang --target=wasm32 -std=c11 -O2 -ffreestanding -nostdlib \
  -DROSWASM_BRIDGE_ROS_VERSION=2 \
  -Wl,--no-entry -Wl,--export-memory \
  sdk/javascript/bridge.c -o /absolute/new-output/roswasm-bridge.wasm
python3 -B sdk/tools/check_wasm_imports.py \
  /absolute/new-output/roswasm-bridge.wasm --backend 2 --require-all
wasm-tools validate /absolute/new-output/roswasm-bridge.wasm
```

Change the macro to1 for ROS1. The default scratch capacities are explicitly
compiled4096-byte input and output regions; overriding
`ROSWASM_BRIDGE_INPUT_CAPACITY`/`ROSWASM_BRIDGE_OUTPUT_CAPACITY` changes storage,
not a host grant. The source statically bounds them by IPC/request ceilings.
Choose capacities consistent with the complete message/response grant.

The bridge has no `_start` and is called by an embedding through exports. It is
not a standalone CLI application and it does not make the uwvm CLI run JS.
The embedding must supply the selected `uwvm_rosN_v1` imports with the actual
authenticated Instance, caller ancestry, capability policy and Gateway binding.
A permissive JS object pretending to implement ROS is not that embedding.

After the embedding has validated and instantiated it, application-side setup is:

```js
import { fromGuestBridge } from './bridge.mjs';
import { Session, Node } from './roswasm.mjs';

// instance is supplied by the authenticated embedding, not created with mocks.
const { bridge, scratch } = fromGuestBridge(2, instance);
const session = new Session(2, bridge, scratch);
const node = new Node(session);
const publisher = node.createPublisher('pub'); // explicit host grant required
try {
  const status = publisher.publish('hello');
  if (status !== 0) throw new Error(`publish refused: ${status}`);
} finally {
  publisher.close(); // report nonzero cleanup status in a real application
}
```

Only one serialized Session owns the reserved C scratch array. The adapter
reacquires memory views after growth and preserves all i64 bits as BigInt. It
cannot attest the production embedding's identity or cgroup from JavaScript.
The no-entry bridge's compiled artifact history is separate from qualification
of an authenticated embedding. It does not gain a CLI `_start` or a JS engine.
The new [command relay](RELAY.md) adds a distinct `_start` dispatcher and copied
async API while preserving these original exports. Its actual compilation and
delivery are pending. Current local checks are scheduled serially by the owner
in fresh `/tmp` snapshots with low memory; no historical receipt is rewritten.
