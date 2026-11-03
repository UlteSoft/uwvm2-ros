# Portable joint state delivery evidence

`--portable-robot-audit-run <32-lowercase-hex-run-id>` opts into the existing
bounded portable runtime audit stream on stdout. It grants no ROS permission,
does not create a deployment, and does not change the result of an import. The
channel remains disabled by default and has one shared eight-record limit for
binding, operation and state-delivery observations in a process.

After an admitted `read_state` validates a fresh native response and successfully
copies its exact bytes into the actual caller's guest memory, the VM can emit
`uwvm-sdk-simulation-state-delivery-v1`. Refused calls, malformed/stale native
responses, and failed memory writes produce no successful delivery record.
Observation failure or an exhausted audit quota does not retry a robot command
or change a successful import into an error; consumers requiring complete
evidence must reject an incomplete stream.

The record binds the current process/run/time, whole Wasm SHA256, actual module
instance, runtime generation, permission session and import-call sequence. It
includes the exact adapter/policy/native provenance digests, owner handle and
mapping/policy/native epochs. `payload_hex` and its SHA256 identify the exact
copied state frame, including its sequence, native timestamp, freshness window,
joint slots and positions. `memory_index`, `output_offset` and `output_bytes`
describe the successful destination copy.

A pre-command state has command ID/outcome/observed-sequence zero, a null native
operation ID, and no guest-command digest. After command consumption, an actual
command snapshot and the accepted native UUID are required: a later state must
never be emitted as a fabricated baseline. The snapshot's command fields and
digest bind the final copy to the accepted/terminal operation.

`guest_memory_written=true` establishes the successful VM memory copy.
`guest_processing_proven=false` deliberately leaves guest computation to the
consumer: the SDK separately checks exact frozen source/Wasm identity, actual
after-exec process evidence, command construction, terminal correlation, and the
guest's checked zero exit. The record alone proves neither application behavior
nor official simulation or physical robot acceptance.

The current portable joint ABI selects memory zero and does not qualify an
independent multi-memory library route. Those products need their own actual
module/memory/permission integration and acceptance evidence.

Regression sources:

- `test/0084.portable-robot/state_delivery.cc` exercises the production encoder,
  bounds, disabled audit, invalid identity/freshness/UUID and the shared quota.
- `test/0084.portable-robot/vm_state_delivery.py` runs the actual LLVM-AOT ROS VM
  and frozen SDK joint-step Wasm against an explicitly synthetic native protocol
  peer. It checks copied payloads, disabled audit, permission denial and stale
  responses. This is a VM regression, **not official simulator acceptance**.

Both compilation and execution require the campaign's current verified common
64 GiB, zero-swap admission. Current commands, executable/source hashes, results
and raw logs belong in the campaign report; do not reuse a historical build,
PID, boot, cgroup inode or admission record as current qualification.
