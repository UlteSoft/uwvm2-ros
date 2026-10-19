# Custom fixed-topic qualification

These fixtures cover the first statically registered custom Topic profile.
They do not establish complete custom Service/Action support or the original
project's full interoperability acceptance. Compilation and execution belong
only in the provided SSH Linux cgroup, with zero swap and bounded serial
compiler processes. Never substitute a local build for that evidence.

The current user-selected budget is exactly64GiB, matching the original request
(`68719476736` bytes). Historical fixed-v1 reports retain their recorded budget,
source and boot. Current runs require a newly inspected cgroup incarnation.
The additional complete bounded Topic/Service/Action workflow is documented in
[bounded-v2/README.md](bounded-v2/README.md); its final composite keeps these
fixed-v1 providers and identities unchanged. Registration still grants no rights.

Follow the [ROS1 guide](ros1/README.md) and [ROS2 guide](ros2/README.md) to build
the exact official interface packages and export their genuine native MD5 and
RIHS identities. Generate one reviewed deployment bundle containing both
records with [the SDK generator](../../sdk/custom-types/README.md). Preserve
the catalog, definitions, all output hashes, official package artifacts and
native identity probes. Both Gateways and the Worker must use that same
common manifest. Selecting it does not enable ROS or grant a resource.

`contract_test.cc` checks canonical scanner bounds, boolean representation,
finite floating-point bits including signaling NaN, exact byte length,
malformed registry metadata and an empty default table. It does not use
native message generation or contact ROS.

`policy_test.cc` runs twice. The default build checks disabled targets with
empty connections and grants, plus absent custom support. The explicitly
selected deployment build checks every required capability field, native
identity and codec identity, cross-version denial, category restrictions,
byte quotas, malformed incoming records, event queue closure and duplicate
command-line fields. These are in-process semantic tests, not authenticated
IPC or Wasm caller-identity evidence.

The existing ARM unit controller has a separate closed selection for these
three tests, without changing the original mandatory unit inventory:

```text
python3 <frozen-source>/tests/ros_aarch64/run_unit.py \
  --source <frozen-source> --manifest <complete-source-manifest.json> \
  --config <pinned-cross-compiler-config.json> --out <fresh-output> \
  --suite custom-topic-contracts \
  --custom-registry-header <frozen-source>/tests/ros_custom_types/deployment-r1/generated/manifest.hpp
```

Run the command inside the verified Linux cgroup. The selected compiler config
must identify the existing AArch64 cross SDK, frozen SDK inventory, compiler
resources and pinned QEMU. The controller uses actual `-MD` dependencies,
pre-main stopped ARM mappings, bounded process working sets and raw cgroup
counter comparisons. It refuses an external custom manifest. Inspect all raw
logs, exact markers and the final architecture/cleanup result; a prepared
recipe or a successful source model is not an execution pass.

The native fixture executables then exercise ordinary real ROS publishers and
subscribers through the reusable custom bindings. Successful direct binding
tests must be followed by current custom-enabled Worker/Gateway/Guest tests
with explicit command-line policy and authenticated IPC. Keep those different
scope claims distinct in the final task audit.
