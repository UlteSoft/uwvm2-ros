# Complete C# records and ROS JointState with Mono WASI

The [2026-10-19 command evidence](../../../../../0050.high_level_languages/results/native-budget/language-csharp-records-jointstate-20261019/README.md)
qualifies the unchanged complete C# record fixture inside the six frozen
Release/Debug UWVM products and the selected ROS1/ROS2 JointState static bridge,
link, imports and default-disabled paths. Positive JointState subscription is
built separately; native message delivery is not qualified by this matrix.

## Toolchain and isolation

Use the admitted .NET SDK **8.0.408**, Mono `wasi-wasm` runtime/workload
**8.0.15**, WASI SDK **20.0** (Clang 16) and the closed, already installed package
set. Preserve `global.json` and both project-specific `Directory.Build.props`
files. The evidence owner uses a complete private copy of .NET, private HOME,
NuGet cache, temporary files and output directories. The five installed-pack
receipts are relocated to that private SDK. Their identities and bytes must
remain unchanged; their timestamps may change. Other admitted tool files retain
full identity, metadata and content checks. No installed target is patched.
System host libraries and the external WASI toolchain remain part of the TCB;
this is not a hermetic compiler image.

All actual generation, builds, validators and UWVM executions run on SSH Linux
in the original 64 GiB cgroup with swap limit 0 and CPU 15 affinity. The candidate owner
bounds time, address space, RSS, output and tmpfs growth, closes discovered
process owners and verifies original resource events. There is no local host
.NET execution substitute. Recorded command durations are not benchmark data.

## Build the complete record fixture

The following names denote absolute paths selected by the trusted build owner:
`SOURCE` is a coherent source copy, `WORK` a fresh private directory, `DOTNET`
the private SDK's executable, `WASI_SDK_PATH` the admitted WASI SDK, and
`PINNED_PACKAGES` a private copy of the admitted offline NuGet package set.
Set private `HOME`, `DOTNET_CLI_HOME`, `TMPDIR`, `DOTNET_ROOT` and
`NUGET_PACKAGES` before invoking the toolchain. Disable telemetry, workload
update notification and build servers. Do not share output directories between
configuration, backend, mode or negative-control jobs.

```sh
mkdir -p "$WORK/generated" "$WORK/empty-feed"
python3 -I -S -B "$SOURCE/sdk/origin/custom-types/managed_profile.py" \
  "$SOURCE/test/0073.ros-tests/support/source/ros_custom_types/managed-records/fixture/catalog.preparation.json" \
  --out "$WORK/generated" --language csharp

"$DOTNET" restore \
  "$SOURCE/test/0073.ros-tests/support/source/ros_custom_types/managed-csharp/WasiRecordContract.csproj" \
  --source "$WORK/empty-feed" --packages "$PINNED_PACKAGES" --disable-parallel \
  -p:Configuration=Release -nr:false \
  -p:GeneratedRecords="$WORK/generated/csharp/BoundedRecords.cs" \
  -p:ContractSource="$SOURCE/test/0073.ros-tests/support/source/ros_custom_types/managed-csharp/RecordContract.cs" \
  -p:RosWasmSdkRoot="$SOURCE/sdk/origin/csharp/RosWasm" \
  -p:RosBuildRoot="$WORK/out" -p:WasmAppDir="$WORK/out/AppBundle/" \
  -p:UseSharedCompilation=false

"$DOTNET" publish \
  "$SOURCE/test/0073.ros-tests/support/source/ros_custom_types/managed-csharp/WasiRecordContract.csproj" \
  --no-restore --disable-build-servers -c Release -m:1 -nr:false \
  -p:GeneratedRecords="$WORK/generated/csharp/BoundedRecords.cs" \
  -p:ContractSource="$SOURCE/test/0073.ros-tests/support/source/ros_custom_types/managed-csharp/RecordContract.cs" \
  -p:RosWasmSdkRoot="$SOURCE/sdk/origin/csharp/RosWasm" \
  -p:RosBuildRoot="$WORK/out" -p:WasmAppDir="$WORK/out/AppBundle/" \
  -p:UseSharedCompilation=false
```

Use a fresh `WORK` for Debug. `WasiRecordContract.csproj` explicitly includes the
real generated record source, unchanged contract, SDK `Codecs.cs`, `Result.cs`
and `Retirement.cs`. Mono interprets the managed IL inside a single-file core
Wasm command; AOT is disabled and trimming is enabled. `ManagedContract.csproj`
is the separate host-source contract project; its dependency fix is present,
but this matrix does not execute the host project.

Read the actual native `clang-compile.rsp` to identify the final Wasm; do not
infer the artifact from a similarly named file. Validate the whole module with
`wasm-tools validate`. The baseline fixture command is
`uwvm-csharp-record-contract.wasm` under `WasmAppDir`. Run the actual interpreter
with the reviewed WASI Preview 1 imports and system-environment inheritance
disabled. The recorded driver retains its complete `-WFE-*` selection and argv:

```sh
"$UWVM" -Rint --wasip1-global-noinherit-system-environment \
  --wasm-set-main-module-name csharp_records \
  --run "$WORK/out/AppBundle/uwvm-csharp-record-contract.wasm"
```

Successful stdout is exactly:

```text
managed C# record cases=15 checks=57 native/ROS=false reflection=false
```

The fifteen methods exercise independent scalar bytes, signed widths, full
unsigned 64-bit values, signed zero, nonfinite rejection, strict bounded UTF-8,
truncation/trailing input, malformed booleans/text, nested/fixed/sequence fields,
independent defaults, empty service/action components, normalized ROS1 time and
duration, input ownership and contextual/verbatim field names. No reflection,
transport model or native ROS service supplies the result. A separately staged
one-byte scalar-golden corruption must exit 1 with the actual
`RecordContract.CompleteScalarGolden()` assertion frame and no success line.

## Build ROS1 or ROS2 JointState

Read the [complete application contract](../../../../../../sdk/origin/robots/vendors/examples/joint_state/README_MANAGED.md).
The application uses the real `Session`, typed `Node` subscription and complete
generated JointState codec. Generate the selected ROS preparation catalog into
the staged SDK's `custom-types/generated-official-standard-managed` directory.
Compile `sdk/origin/csharp/roswasm_bridge.c` with backend 1 or 2, actual selected SDK C
headers, WASI sysroot, `-MD`, `-O1 -g` and target `wasm32-wasi`. Keep the original
object named **roswasm_bridge.o**.

```sh
"$WASI_SDK_PATH/bin/clang" --target=wasm32-wasi \
  --sysroot="$WASI_SDK_PATH/share/wasi-sysroot" -O1 -g -MD \
  -DROSWASM_BACKEND=2 -I "$SOURCE/sdk" \
  -MF "$WORK/roswasm_bridge.d" -c "$SOURCE/sdk/origin/csharp/roswasm_bridge.c" \
  -o "$WORK/roswasm_bridge.o"

"$DOTNET" publish \
  "$SOURCE/sdk/origin/robots/vendors/examples/joint_state/csharp/JointStateLesson.csproj" \
  --no-restore --disable-build-servers -c Release -m:1 -nr:false \
  -p:RosBackend=2 -p:RosJointStateMode=0 \
  -p:RosNativeBridgeObject="$WORK/roswasm_bridge.o" \
  -p:RosNativeEvidenceDir="$WORK/out/link-evidence" \
  -p:RosBuildRoot="$WORK/out" -p:WasmAppDir="$WORK/out/AppBundle/" \
  -p:UseSharedCompilation=false
```

First restore this exact project with the same properties and offline package
arguments shown above. Keep source-relative generated SDK paths coherent.
`RosJointStateMode=1` is a separate positive subscription build, not an implicit
permission grant. Backend 1 requires the ROS1 generated catalog and bridge 1.
The actual P/Invoke table must contain the `roswasm_bridge` module and exactly
the selected 38 ROS1 or 43 ROS2 symbols with correct i32/i64 C prototypes. The
original object must occur exactly once in the actual final native response
and its scan metadata. The final Wasm must contain the matching numeric ROS
imports plus only the reviewed Mono WASIp1 signatures, with no other backend.

Default mode checks actual ABI version 1 and the literal `joint-state` alias.
Without backend enablement or grants, the ROS UWVM products must return 13 at
open, return no handle, close the Session successfully and print exactly
`PASS_JOINT_STATE_DEFAULT_DENIED`. Positive native delivery additionally needs
an independently qualified native-bound catalog/registry, explicit subscribe
permission, matching native type/codec identity, gateway and actual full-value
publisher/observer evidence. The preparation catalogs do not meet those
requirements. This matrix grants no ROS authority and performs no native
JointState delivery.

## Native symbols and result boundaries

The pinned Mono target unconditionally emits `-Wl,-s`; setting the project
properties alone does not retain native DWARF. Preserve its original response
and baseline output. Reinvoke the same ordered Clang arguments with exactly
one `-Wl,-s` removed, `-g` prepended and only the output path replaced. The
recorded matrix validates both complete Wasms, requires `name`, `.debug_info`
and `.debug_line` in the retained variant, and independently checks byte-identical
noncustom sections for every pair. It executes both variants of every
applicable positive, deliberately failing and default-disabled case. Portable
PDBs and native DWARF do not establish managed C# source/variable debugger
support; no debugger qualification is claimed here.

The accepted matrix has 12 producer builds, 24 final Wasms, 24 positive codec
runs (360 completed methods, 1368 checks), 24 expected managed assertion failures
and 16 default-disabled JointState outcomes. The six Runtime identities are
frozen earlier default-on products, not fresh builds of a later working tree.
Three earlier incomplete cohorts remain explicitly failed. The public inert
verifier reparses actual binary imports and P/Invoke prototypes without
executing any archived source. Complete owned intermediate/tool-copy bytes are
retained on Linux before only this task's temporary trees are removed.
