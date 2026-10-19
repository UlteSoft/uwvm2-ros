# Preparing complete vendor interfaces

This document describes the explicit `uwvm-ros-vendor-complete-bounded-v1` source domain. Its implementation is `complete_prepare.py`, backed by `../industrial/vendor_profile.py`. It preserves full original Message, Service and Action definitions, including constants and supported declared defaults. The original custom, standard and legacy-introspection preparers keep their own formats and admission rules. Selecting a vendor inventory never selects a driver or grants a permission.

The additive `inventory-v2.json` records 28 vendor research targets. The original `inventory.json` remains the historical nineteen-vendor observation. Repository links establish research provenance. They do not establish a matching robot model, installed SDK, firmware, native delivery, official test-suite result or hardware acceptance. NVIDIA's software, simulation, computing hardware and model-service layers remain separate platform targets. Tesla Optimus requires a verified public control interface before a concrete adapter can be claimed.

## Follow the trust boundary

An application uses the existing checked ROS imports, complete typed records, and explicit resource aliases. It never imports the native SDK. A separately built native Gateway or model bridge owns any vendor credential, lease, native socket, hardware pointer or SDK thread. The bridge can further restrict an operation but cannot enlarge the Runtime grant. A Guest-provided token is not an authenticated module identity. A shared native bridge profile must document its shared lease/feedback scope rather than claim isolation by a payload token.

Start every Guest instance with no ROS grants. Check the ABI, then demonstrate that each requested alias returns permission error 13 without retaining an acquired handle. A positive example uses a fresh instance and grants each exact operation and resource independently. Publishing commands, observing state, calling a service, sending goals, cancelling one's own goals, observing feedback and obtaining results are different authorities. Neither a vendor name nor a robot model creates a blanket role. The SDK does not expose update, debugger configuration, credential selection or policy administration to Guest business code.

## Retain the complete source closure

The trusted operator supplies `read(role, full7)` and `load(relative, private_name)`. They are host administrative dependencies, not Guest callbacks. A full7 is an actual ordinary-file observation containing absolute path, byte extent, full SHA256, device, inode, nanosecond modification time and nanosecond change time. The caller must enforce its actual selected-root, file-descriptor, mode and ownership policy before returning the bytes. The preparer checks byte identity again. It does not adopt a magic success boolean from another report.

The request has exactly these keys:

```text
format_version = 1
profile = uwvm-ros-vendor-complete-bounded-v1
vendor = one reviewed inventory ID
model = exact individually reviewed robot/controller selection
sdk_revision = exact selected SDK/driver revision
ros_version = integer 1 or 2
source_lock = actual full7 of the complete source lock
support = every fixed role in complete_prepare.SUPPORT, each an actual full7
```

The source lock has format version 1 and kind `vendor-complete-multiple-repository-source-lock-v1`. Its vendor, model, SDK revision and ROS major must match the request. It also contains `repositories`, the actual catalog full7, `definitions`, `source_review = CLOSED_SOURCE_REVIEW_NOT_EXECUTION`, and `default_authority = []`.

Each repository row contains a unique `id`, its HTTPS `url`, exact Git `commit`, `tree_git_sha1`, the actual `license` full7, the original `capture` report full7 and `source_files`. Each selected driver/SDK source file has `relative`, `git_blob_sha1` and `pin`. Retain files needed to establish model, API and native conversion behavior even when the driver defines no custom ROS interfaces. This is common for drivers that use the complete standard JointState and FollowJointTrajectory interfaces. The operator checks captured definition/source Git blob bytes. Independent source review must still establish commit/tree associations; a declared hash or capture-report string is not a cryptographic attestation.

Each definition has one explicit origin:

* `OFFICIAL_GIT`: retain `repository`, `original_file`, `generation_file`, `git_blob_sha1` and the actual definition `pin`. Copy the entire original file. Do not remove constants, comments, defaults, fields or Service/Action sections to satisfy an older parser.
* `AUTHORED_BRIDGE`: retain `generation_file`, the complete authored definition `pin`, the actual typed conversion `mapping` source full7, and `sdk_sources`. Each original SDK source contains `repository`, `original_file`, `git_blob_sha1` and `pin`. The bridge interface uses a distinct `uwvm_` package and later obtains its own real rosidl identity. It must not impersonate an official vendor message, reuse an incompatible native ID, or cast away a signedness mismatch.

All catalog entries explicitly name their full original definition, ROS major, type, canonical capacities and registration choice. Registration describes an implemented endpoint. Dependencies can remain complete without becoming endpoints. Existing builtin endpoints remain in their original domains; a complete vendor registry cannot shadow them. Each original repository must contribute an actual selected source, definition or SDK mapping dependency. Source-role and retained-input reads have finite cumulative byte budgets before allocation.

## Prepare, export, bind, then build

1. Call `complete_prepare.prepare(inventory, request, read=read, load=load, out=fresh_output)`. The output must be a fresh, canonical, caller-owned directory. The operator retains original definitions, per-repository licenses and provenance, then emits complete six-language Guest source, typed native constant checks and a native identity exporter. No compiler, Runtime, ROS node or robot is launched by this call.
2. Build the actual exporter against the exact installed native SDK. Retain every original generated header, full system-inclusive compiler dependency list, actual link closure and selected library. A separately owned process must prove its real stopped mapping and release its original gate before exporting finite raw metadata. The real SDK supplies all native identities. Type spelling and canonical codec hashes cannot manufacture MD5 or RIHS values.
3. Call `complete_prepare.bind_and_generate(inventory, actual_preparation_pin, actual_native_metadata_pin, actual_native_owner_report_pin, read=read, load=load, out=another_fresh_output)`. The complete adapter compares the entire root/component/protocol graph, field order, native capacities, defaults and typed constants before copying any identity. The owner report remains retained DATA; the function does not pronounce a native process qualified from its status field.
4. Build the Worker, Gateway, native model bridge and Guest against that same admitted source revision and generated manifest. The complete provider requires the matching descriptor tail, native traits and Gateway checks. An older binary cannot consume the new source layout. Record actual Wasm imports and every final artifact hash before correctness, permission, adversarial, official-suite, architecture or performance results are credited.

The standalone `../industrial/prepare.py` provides a source-oriented command for individually SHA-paired inputs. It is useful for developing a schema family. It does not replace the multiple-repository source-lock admission or native owner evidence above.

Explicitly empty string defaults remain in the original schema and codec identity. Current TypeDescription metadata cannot distinguish their presence from an absent default. Native binding refuses that case until a separate original-native-IDL proof is implemented. Source generation additionally supports finite, length-exact fixed float32/float64 array defaults of at most 128 elements; the six-language UR qualification retains their actual bytes. This Source support does not establish native binding for array defaults. Unsupported array/sequence/nested defaults, IDL preprocessing and C/C++ member spellings still fail explicitly at their respective admission boundary. Never describe a refusal as complete compatibility for that interface. Modern complete native binding currently requires actual full TypeDescription support; historical ROS SDK compatibility remains a separately implemented and qualified path.

## Compose vendors in one application

Use one explicitly merged, deduplicated catalog per physical Runtime/Gateway compilation domain. Coordinate each repeated native type and its exact original definition, native identity, canonical capacities and codec identity, then generate the complete bundle once. Every translation unit and shard must select the same physical manifest and native source revision. Full compiler dependency/link admission must verify this agreement. Merely placing two SDK directories beside each other does not provide it.

Independent generated bundles can define the same Header, record or C++ trait with different bounds or catalog-level constant proofs. Arbitrarily including or linking those bundles creates namespace or ODR conflicts. The current design does not claim that composition mode. Separate Wasm applications can use separate bundles; one Wasm application can use one coordinated bundle and hold multiple vendor aliases. Acquiring one alias must not open another alias or convert a denial into success. If later acquisition fails, close every earlier owned handle and retain both the original error and any cleanup error.

A joint-state source is not a universal motion profile. A model adapter must validate exact joint order, dimensions, limits, freshness, controller mode and result/feedback semantics. A zero vector is not a portable hold command. Expiring a native command lease can stop future sends; it does not prove a physical stop. Document each model's actual stop, cancel, timeout, unknown-outcome and recovery behavior. A native Trigger service is not a certified emergency stop.

## Test and measure the actual system

Complete implementation precedes the unified qualification run. Source receipts, authored test cases and generated headers are not executed tests. Test zero grants first, then each narrowly granted operation, cross-alias refusal, stale/closed/revoked owners, malformed complete values, queue and message ceilings, partial cleanup, cancellation races and unknown outcomes. Compare official full native values in both directions. Use the actual upstream suite for the selected source/SDK/controller and report exact selected, passed, failed and unavailable cases without extrapolating to all releases or physical hardware.

Linux work belongs under `/home/macromodel/Documents/uwvm-test-261018`, normally `build/robots/<owned-run>`, inside the actual hard 64 GiB, swap-disabled cgroup. Directory location alone does not establish cgroup membership. Inspect memory limits and process ownership before each real job. Local build/test files belong in `/tmp`. Preserve the agreed disk floor, bounded job resources and raw failed attempts; `/tmp` may share the root filesystem.

ROS architecture qualification covers the selected upstream-supported x86 and ARM targets. The ordinary Runtime's additional architecture matrix is separate. VTune hardware-counter Hotspots and Microarchitecture measurements require actual supported PMU access, a recorded core type, native/Wasm workload equivalence, hardware counters and an otherwise idle measurement window. Source-level efficiency and functional pass counts do not establish native-like performance.

When the separate security implementation is enabled, finish compilation, linking, stripping and any embedded-signature edits before detached signing of the final complete Wasm bytes. Select verification explicitly per module with the documented `--secure module` interface. Signing/import restrictions and local signature policy do not grant ROS, WASI, filesystem, socket, debug or update authority. A changed artifact or fresh load requires verification of its actual final bytes; never transfer an earlier artifact's verification receipt.

## Finite preparation inventory

The complete operator accepts at most479 retained original input assets. It
reserves32 generated products and one `generation.vendor.json` before creating
the output directory, so every published preparation fits the same512-entry
whole-file inventory consumed by native binding. The64MiB aggregate byte budget
remains independent. Excess source-file counts are refused as a whole before
output creation; a successful preparation is never later rejected merely
because preparation and binding used different file-count domains. These
limits narrow this explicit Source profile and do not grant permissions or
claim complete upstream acceptance.

## Complete original multi-vendor source unions

The explicit complete-source merger accepts two to eight catalogs and at most
512 unique definition roots within 64 MiB; downstream preparation/native-binding
inventory and per-record wire/node ceilings still apply independently. Legacy
bounded-v2 retains its separate ceiling. It preserves legal ROS1 same-name
Message/Action definitions using both native name and original kind, while
refusing other category collisions and all conflicting bodies, registration
choices and bounds. Actual Clearpath Noetic plus Elephant Noetic preparation
retains 334 definitions and generates 534 complete records; the SDK includes
read-only C/C++ two-alias examples using that one source domain. This source
union emits no native registry or permission and does not establish positive
native/driver/controller execution. See the Elephant SDK multi_vendor guide.
