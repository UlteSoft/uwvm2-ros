# GR00T N1.7 typed inference bridge

The actual fixed SDK's `Gr00tPolicy(embodiment_tag, model_path, *, device,
strict=True)` obtains its modality configuration from the real selected
processor. `get_action(observation, options=None)` returns `(action, info)`.
The native owner calls this actual method once. It neither speaks an invented
model protocol nor exposes the original ZMQ server's reset/kill/model management
operations to Wasm. The original SDK server is preserved as primary Source;
its port, pickle policy or optional token are not a Guest network authority.

`uwvm_groot_bridge` is an **authored ROS2 interface package**, not an official
NVIDIA message name. Its four complete original .msg/.srv definitions are
preserved along with an explicit preparation catalog. Build it with a real
ROSIDL C/C++/Python producer, then export full actual TypeSupport/Event/source
identities with `native/export_identities.cpp`. No generated headers or hashes
are hand-written. The service root has a separate identity from all components.

## Complete selected tensor domain

The bridge admits one batch. Video values are keyed uint8 `[B,T,H,W,C]` tensors
with B1, C3, actual model T, at most3 keys and768 values per tensor. State and
action values are complete finite float32 `[B,T,D]` tensors with at most4/8 keys
and320 values per tensor. Language has at most2 actual keys with `[1,1]` strings
of256 UTF8 bytes, forbidding NUL. Keys are exact actual configured keys, bounded
to64 UTF8 bytes, with no extras, duplicates or invented defaults. All dimensions,
counts, ordering and values survive; no native pointer, stride or GPU object is
serialized. Model-specific D and image suitability remain actual SDK/processor
checks; no fabricated full-model compatibility follows from matching shape.

The selected request/response also stay within the real4096-node IR ceiling.
The request cartesian upper is below32KiB and the response cartesian upper is
below32KiB; both remain subject to the actual generated complete bound and Host
policy. This deliberately finite image domain can be too small for a selected
real model. Such a model is unsupported95 / a whole value over quota is90; the
bridge never crops images, removes action fields, shrinks horizons, truncates
arrays, or calls a partial value success. Current original Gr00tPolicy returns
an empty info dictionary. A new nonempty info domain is95 and terminal after
inference, requiring a separately typed protocol extension instead of erasure.
The full native model/RMW may allocate beyond canonical storage; external memory
and time limits are independently necessary.

## Trusted native owner

Freeze a local actual SDK tree, Python/CUDA environment and local model/processor
weight tree. Keep all read-only for the owner lifetime. The shipped
`run_captured.py` first binds/captures the three exact adapter siblings and
executes them without PYC. Use that actual entry for source identity, not an
arbitrary ambient import of native_owner.
`sdk-source-inventory.json` is an independently Git-extracted SHA inventory of
all70 original gr00t Python files from fixed dac54b3. Its SHA is literally pinned
by `owned_sdk_source.py`. The same ordinary nofollow/nonblock descriptor captures
each source; a custom import loader compiles those captured bytes after normal
sys.modules registration. It cannot use ambient stale PYC or a different SDK
inventory. This does not authenticate external NumPy/Torch/transformers binary
packages, GPU kernels, model weights or installed ROS; independently retain their
actual package/source/loader/full-link identities. No remote model download is
admitted: the owner requires an absolute physical model directory and offline
HuggingFace/transformers mode. Operator-provided trusted local models are not
untrusted executable Guest input.

Future explicit native invocation (never run here):

```
python3 run_captured.py --enable-native-inference \
  --sdk-root ACTUAL_PHYSICAL_SDK --sdk-source-inventory sdk-source-inventory.json \
  --model-root ACTUAL_PHYSICAL_FROZEN_MODEL --embodiment ACTUAL_TAG --device cuda:0 \
  --service /uwvm_groot/infer --domain ACTUAL_DOMAIN \
  --lifetime-ms 10000 --call-ms 1000 --maximum-requests 8
```

Without `--enable-native-inference`, a fully formed invocation returns13 before
source/model/ROS opens. Path placeholders must be replaced with real admitted
artifacts; the package does not supply model weights, credentials or devices.
Lifetime1..60000ms/call1..lifetime/count1..64 are explicit. Model initialization
may itself exceed the wall budget; a post-initialization check refuses110 before
creating the service, while the external process owner provides preemption.

The native callback preallocates bounded nonreusing nonce/result bookkeeping.
A valid application nonce is positive u64. It is not a native ROS request ID or
Gateway ticket. An admitted nonce is never reused, including failed requests.
The callback prepares full private NumPy observation before inference, calls
actual get_action once, then validates/copies every actual action into a private
real ROS Response before returning it. A fault, unsupported/oversized returned
value or late result after inference makes the native owner terminal. There is
no retry/reset. The returned nonzero status retains a full typed failure domain;
no action prefix is emitted. Real ROS middleware retains its own genuine request
writer/sequence; Host UMRC/URMI correlates that independently.

`native_owner.main.last` retains actual inference uncertainty and independent
service/node/context cleanup outcomes. Process kill/OOM cannot promise Python
finally completion. Lease expiry does not cancel a GPU kernel. The actual native
process performs inference only and never commands a motor.

## Real Wasm client

`examples/client.go` is a complete TinyGo main using real generated records and
the actual owned-service SDK. Copy the actual generated bounded Go record source
into its `customtypes` package in a fresh build module. Select actual wasm32 WASI,
TinyGo tags `ros2 rmw_service_v1`, and a real local SDK module replacement. No
native ABI, JNI, ZMQ import, weak stub or fake producer is used. Its seven args
are call alias, observer alias, query alias, actual SERVICE-root canonical ID,
actual request QoS12 tuple, actual response QoS12 tuple and complete typed JSON request
file (1MiB parsing cap; actual canonical wire remains below32KiB). Unknown
JSON fields, duplicate keys, trailing values, wrong fixed-shape lengths and
invalid numeric extents are refused; no extra array member is silently discarded. QoS tuple order is history,reliability,durability,liveliness,depth,
deadline-sec/nsec,lifespan-sec/nsec,lease-sec/nsec,avoid-namespace. All values
are independent actual observations, not guessed defaults.

The request is parsed into the actual complete generated Go struct and fully
encoded/validated before activation. Supply all actual model keys, fixed shape
arrays and full pixel/state/language sequences; the program does not manufacture
a synthetic camera observation or guess a real robot state. One actual finite availability
query precedes one submit. Up to64 explicit receives are an observation budget,
not elapsed time or an executor. A late deadline / exhausted budget abandons
locally once; it never replays inference. Complete typed response values are
constructed before ACK, then every tensor key/shape/value is printed. Group
cleanup retires the same native object once and independently closes query,
observer and call owners. Retain its `lastCleanup` separately from the primary.
The14 authored Python local contracts use actual future generated ROS types and
NumPy, with a clearly local policy model; none were executed and they do not
qualify a GPU, Native TypeSupport, Runtime or network exchange. Seven additional
Go input contracts retain full-u64 nonces and whole values, rejecting shape
projection, duplicate/case-alias keys, byte wrapping and invalid Unicode; they
also remain unrun.
