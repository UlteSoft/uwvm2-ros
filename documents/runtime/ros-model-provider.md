# Bounded model jobs and native worker admission

The five `model.*` imports and two `request.*` imports currently dispatch only
explicit `local-model` bindings for `model.language`, `model.vision` and
`model.world`. Their registration does not complete the native worker backend
or qualify real Wasm execution. Other request-owning purpose families in the
catalogue still need their own routing. No name loads a library, selects a URL,
opens a socket or confers device/control authority.

## Deployment and worker lifetime

The trusted deployment supplies exact model and task identities and SHA256
digests, message types, a transport codec, finite capacity, and a separate
native worker manifest. All imports and resource grants start disabled.
`--ros-scene-check` validates inert configuration only. Preparation must return
a real owned worker and attach a fresh provider generation before any model
session can open. Missing or failed preparation refuses launch; there is no
guest-triggered lazy process spawn or fallback to ambient model services.

Worker owners live after `model_store` in the broker so their cleanup occurs
first. Revocation calls only nonblocking `request_stop()` while authority is
locked. `retire()` and destruction confirm cleanup outside that lock before
the dispatcher is destroyed. A worker must not retain an owning reference back
to its broker. Forked descendants must reject access before acquiring inherited
locks or using a parent worker's PIDFD.

The isolated CPU worker implementation is being integrated separately. The
current core tests use a trusted arithmetic consumer and lifecycle-owner
fixtures; they do not establish sandbox isolation, arbitrary LLM compatibility,
GPU support, remote export authority or physical safe-stop behavior.

## Canonical carrier and reservations

All seven functions use the common carrier
`(capability:i64, memory:i32, request:i64, request_bytes:i64, output:i64,
output_capacity:i64, length_output:i64) -> errno:i32`. Records follow
[`model_contract.h`](../../src/uwvm2/imported/ros/scene/model_contract.h) and the
semantic catalogue. The host owns the complete request before parsing or
publishing any result. The request and result may overlap; the eight-byte
length cell may overlap neither. Partial runtime reads are never parsed.

Requests larger than the 512-byte inline buffer reserve their whole encoded
extent from model workspace before recoverable fast_io allocation. Model job
parameters, native copies and output copies also have explicit reservations.
Native scratch cannot use the same bytes simultaneously for multiple copies;
the token outlives the actual allocation and keeps its job charged even after
the guest releases its receipt. Capacities and optional rate attenuation are
separate. Missing rate limits are unrestricted within an explicit grant; an
explicit zero rate remains zero.

`text.utf8` accepts a complete NUL-free UTF-8 body. `bytes` accepts opaque bounded
data without claiming application-level validity. The fixed isolated worker
owns task-specific parsing. A valid transport body or successful model result
does not authorize any proposed tool or actuator command.

## Admission, retry and cancellation

`model.submit` atomically admits the originating model capability and every
input's `buffer.read` capability. Failed compound quota checks cannot partially
charge another scope. Inputs must currently be sealed guest-origin buffers;
forged sensor source identities, timestamps, generations and frames are refused.
Trusted sensor provenance is a separate integration obligation.

Submission keys use the model session and strictly increasing sequence. The
last complete canonical request hash identifies exact retry. Retrying returns
only its retained admission receipt, charges the current origin carrier, and
does not read, re-pin or charge an input again. It works after input capability
closure/revocation or `request.release`, while the origin session and submit
authority remain valid. It cannot restart inference or acquire current source
authority. Changing any field under the same key returns conflict.

Worker computation occurs outside authority locks. Each bounded source borrow,
parameter copy, scratch reservation and prediction publication reacquires the
complete original scope set. Cancellation cannot recall data already disclosed
to native computation; it prevents subsequent access/publication. A denied
worker can still acknowledge actual cleanup without publishing a prediction.

`model.take` measures the actual canonical result before charging or copying it.
Short output reports its required length; failed delivery preserves the chunk.
Mutating submission/cancellation receipt carriers conservatively require 160
bytes before effects, then report the exact encoded length. `request.release`
requires a terminal request with confirmed cleanup; closing a receipt does not
refund memory still retained by a native work object or scratch token. Provider
loss with uncertain execution cannot be treated as proof of cleanup.

## Debugger and qualification

`info scene resources` emits one `model` row per configured alias with `id=0`.
`open_handles` counts retained sessions and `members` retained requests;
`input_bytes`, `output_bytes`, `output_items`, and `workspace_bytes` report
reservations. No prompt, model output, digest, request key, worker PID or native
address is exposed. Inspection uses cached counters with try-locks and does not
expire a deadline, claim a job or run inference. DAP and the IDE retain exact
decimal uint64 strings and reject model counters attached to another row type.

Current receipts under `uwvm2-ros-sdk/.tmp/ros-scene-security` include:

- `model-dispatch-receipt-fence-final`: seven security groups under Clang
  ASan/UBSan, GCC and TSan, including real malloc-family failure injection,
  source revocation, retained retry receipts and fork-before-lock rejection.
- `model-deployment-second`: four configuration/aggregate/native-owner cleanup
  groups under the same three variants. Native-owner fixtures do not execute a
  model process.
- `model-debug-contract-first`: C++ formatter, closed DAP schema and actual
  editor-controller regressions. This is not graphical IDE acceptance.

These receipts cover their recorded source hashes. Later worker integration,
native scratch changes and actual VM execution need new qualification records.
All compilation and execution use the existing 64 GiB, zero-swap cgroup.
