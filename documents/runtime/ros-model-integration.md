# Portable model-provider contract

The external ABI is `uwvm_ros_v2` (`model.*` operations). Internal operation names are semantic and
unversioned. The model family starts hidden and disabled with no resource
grants. A ROS capability, model session, model name or tool proposal supplies
no inference, network, filesystem, accelerator or robot-control authority.

## Providers and sessions

Deployment binds a logical provider identifier to an exact model identity,
supported modalities, input/output schemas, execution placement and resource
limits. Placement can be local CPU, an explicitly bound accelerator, a sandboxed
worker or a specifically approved remote endpoint. Provider authentication is
host-owned and never copied into Guest memory, logs or ordinary debugger data.
Guest-controlled URLs, paths, environment variables and native plugin names
cannot override the binding. Network egress and data disclosure are separately
granted and the resolved destination is checked by the trusted transport.

Session creation selects only an approved binding and parameters within its
declared constraints. Session/request handles retain environment, generation,
owner grant and provider identity. Requests pin model configuration for their
whole lifetime. Session destruction cancels pending work and retires owned
outputs; disconnection never silently retries a non-idempotent operation.

## Requests and outputs

`model.open`, `model.submit`, `model.take`, `model.cancel` and `model.describe`
express asynchronous execution. The proposed common `request.describe` and
`request.release` operations inspect and retire owned terminal receipts;
`resource.close` extensions require an explicit provider cleanup contract.
These provider operations remain designs in the
[semantic catalogue](../../tools/ros_scene/interface-catalogue.json), not
registered imports. Outputs join the same portable readiness model as ROS
events. Cancellation acknowledgement and actual provider termination are distinct.
Provider errors and partial output have explicit states. Slow inference cannot
block a ROS control callback unless the caller explicitly waits for it.

Language generation, embeddings, structured extraction, image/video inference,
multimodal requests, world-state encoding and bounded world-model rollouts use
registered schemas. Every result carries its request identity, model identity,
source observation references and time/frame metadata where applicable. A
prediction is labelled as predicted, with uncertainty representation determined
by its schema; it cannot overwrite authoritative robot state through this ABI.

Limits include input/output bytes, tokens, tensor elements, tensor bytes,
frames, image dimensions, rollout horizon/branches, active sessions, concurrent
requests, buffered outputs, elapsed time and deployment-defined spending units.
Maximum reservations are admitted before submission. Provider usage settles
the reservation; missing accounting cannot grant an unlimited continuation.
Limits are independent of provider suggestions. No ambient unlimited model
execution or paid endpoint is selected as a default.

## Tensors, images and ownership

Buffers are registered resources with explicit dtype, rank, shape, strides,
byte order, layout, color space and bounds. Products and offsets are checked for
overflow before allocation or copy. Writable inputs must be sealed before an
inference request can reference them. A submitted request holds immutable
leases; mutation and release cannot race provider reads. Outputs become readable
only after validated publication. Batched and shared-buffer implementations
must satisfy the same rules and cannot expose native/GPU addresses.

Sensor export has its own capability: observing a ROS message does not by itself
authorize sending its contents to a remote model. The trusted data path records
the source capability and checks the specific destination grant. User-created
Guest data is identified as such; it cannot claim a trusted sensor provenance.
This is a rule for managed references, not general information-flow tracking
inside arbitrary Wasm code. A guest allowed both raw sensor reads and arbitrary
remote-model input could copy the bytes into an unlabelled input. Deployments
requiring that stronger isolation must separate those authorities into different
environments or accept only approved managed references at the egress boundary.

## Tools and robot actions

A model tool call is a proposal record, never an invocation. The proposal names
a registered tool schema, carries validated arguments and retains the source
request identity. A separate authorized application or trusted arbiter may
accept it. Acceptance checks that arbiter's own current tool/resource grants,
argument constraints, idempotency key and expiry. Approval cannot broaden a
grant. Text in a prompt, retrieved document, image or model output cannot create
or modify policy, obtain credentials, or change the set of available tools.

Robot trajectories and world-model actions additionally require a separately
granted control endpoint and its physical bounds, freshness, frame/type identity,
deadline and cancellation rules. Model confidence is data and cannot substitute
for authorization. A model cannot approve its own proposal. Training, adapter
loading and persistent model changes require separate deployment operations;
ordinary inference authority cannot mutate a shared model.

## Portability and observability

The Guest sees fixed-width scalar values, validated byte records and opaque
handles. No Python object, CUDA handle, HTTP client, file descriptor or framework
pointer crosses the ABI. Providers advertise supported schemas and execution
features; unsupported operations fail explicitly. Model metadata inspection is
read-only and excludes credentials and payloads. Audit records retain identities,
admission/usage, state changes and denial reasons without logging private content.

Native provider qualification is separate from the VM contract tests. A mock
provider can establish isolation, cancellation and accounting behavior, but it
does not establish model accuracy, remote endpoint correctness or accelerator
support. The deployment must supply the actual provider; no test invokes an
unrequested paid model or transfers user data to an external service.
