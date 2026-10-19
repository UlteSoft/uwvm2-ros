# Current-source native compilation qualification

`qualify_current_native.py` assembles the 18 original native products from three
**fresh successful builds of the same complete current source manifest**:

1. `build_gateways.py` supplies 18 freshly compiled products. Its 16 non-Gateway
   probes retain their exact current compiler-recorded project dependencies.
2. `rebuild_affected.py --version 1` supplies the current ROS 1 Gateway with the
   actual GCC 9 `-MD`/final `-MF`, standard/ROS SDK headers and complete static SDK
   input inventory.
3. `rebuild_affected.py --version 2` supplies the current ROS 2 Gateway with the
   corresponding GCC 13 qualification.

The wrapper preserves the 16 base probe images and selects the two strict
compiler products. It never rewrites a raw build report, modifies an executable,
or qualifies a historical-source/dependency-replayed product. Its `PASS` means
**compilation qualification only**. It supplies no ROS behavior, Wasm Guest,
physical ARM performance or complete historic acceptance verdict.

## Shared Gateway validator

`gateway_qualification.py` exports
`qualify_gateway(path, report_path, manifest_path, source, sdk, version)` for both
feature runners and the wrapper. The return value includes `binary`, `report`,
`source_manifest_binding`, `source_dependencies`, `sdk_header_dependencies`,
`sdk_link_inputs`, `architecture_evidence` and `input_pins`. Callers must retain
and recheck the complete `input_pins` after copying or executing the product.

The validator rederives the successful verbose compiler argv and checks the
compile database, effective codegen controls, last effective `-MF`, actual
depfile object rule, original/archived dependency bytes, exact project/system
header sets, linked scalar SDK inputs, complete static SDK keyset and root
aliases. It checks current compiler/frontend images, their actual PID-subtree
mapping records, host QEMU identity, same-incarnation zero OOM evidence,
completed mount cleanup and archived current frozen helper code.

`read_file_binding` opens nonblocking and admits an opened regular file before
reading. It streams binary pins and retains text only when requested. Together
with `read_json_binding`, it binds the same bytes parsed and their opened
device/inode/length/timestamps/hash. These are consistency checks around a read;
they do not lock the whole filesystem or provide a signed attestation.

The 16 retained base probes remain explicitly marked
`FRESH_CURRENT_BASE_PROJECT_MMD_ONLY`. Project `-MMD` records do not establish
their complete system-header/SDK closure. The wrapper makes that distinction
per product instead of upgrading their source records into stronger evidence.

## Prepare and materialize

Default mode prints a plan and writes nothing:

```sh
python3 -B tests/ros_aarch64/qualify_current_native.py \
  --source /absolute/current/source \
  --manifest /absolute/current/source-manifest.json \
  --base-report /absolute/fresh/base/build-report.json \
  --ros1-report /absolute/fresh/ros1/build-report.json \
  --ros2-report /absolute/fresh/ros2/build-report.json \
  --sdk-workspace /absolute/task-owned/sdk \
  --out /absolute/new/current-native-qualification \
  --view-tag current-source-r1
```

For actual materialization, use the same command with `--run` and the **current
observed** `--expected-leaf`, `--expected-device`, `--expected-inode` and
`--expected-boot`. Run only inside the provided existing Linux 64 GiB cgroup with
`memory.max=68719476736`, `memory.swap.max=0` and effective CPUs `0-31`. For
example, append the following after independently measuring the current values:

```text
--run --expected-leaf CURRENT_ABSOLUTE_LEAF \
  --expected-device CURRENT_DEVICE --expected-inode CURRENT_INODE \
  --expected-boot CURRENT_BOOT_UUID
```

This native qualification CLI uses the fixed 64 GiB contract; the SDK compiler
CLI separately accepts the explicit `--expected-memory-max 68719476736` flag.
Never copy an older boot/leaf pin into a new invocation.
The tool creates no cgroup, changes no limit, installs no compiler and launches
no native executable. Its output must be fresh and outside source/SDK inputs.

Both SDK roots must carry their existing `.uwvm-task-owned` marker. The tool
creates fresh `tmp/ros1-current-native-<tag>` and
`tmp/ros2-current-native-<tag>` view directories, with symlinks selecting the
qualified immutable products. Existing view tags are refused. After all
qualification and input/cgroup guards succeed, each existing `rosN-build`
canonical symlink is replaced atomically. A real directory at that canonical
path is refused.

Two different filesystems cannot provide a single atomic transaction. Both views
are staged first; failures restore only aliases still identified as this tool's
own newly published symlinks. A changed third-party alias is never overwritten
by rollback. Old reports/products/view directories remain available. The
wrapper records old/new alias facts and canonical paths for review. Concrete
qualification facts are also saved before alias publication with status
`QUALIFIED_INPUTS_NOT_MATERIALIZED`.

## Local preparation models

`current_native_qualification_models.py` uses ordinary synthetic text products,
handwritten compiler/dependency/architecture records and explicitly mocked ELF
facts. It tests both targeted producer schemas, input mutations, same-byte inode
replacement, unknown/replayed source, SDK/keyset changes, compiler/object/final
`-MF`/link/header discrepancies, OOM/incarnation failures and owned temporary
view rollback. These models never execute a compiler, ELF image, QEMU, Wasm,
Guest or ROS process, read a live cgroup, or change an actual task SDK alias.
Actual Linux reports must remain separate from these source/model results.
