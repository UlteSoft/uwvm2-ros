# Complete MoonBit records in UWVM

This directory tests the actual generated bounded record codecs. The fixture
covers all scalar widths, an independent 49-byte golden value, signed zero,
UTF-8 limits, malformed input, nested values, independent defaults and an empty
service request. The separate JointState files use the complete ROS1 and ROS2
schemas, including their different Header and timestamp definitions.

`prepare_uwvm_app.py` turns those same test bodies into a standalone WASIp1
application. It changes test declarations into ordinary functions and changes
`assert_true` into a local abort-on-false check. It preserves every assertion
expression and input. A completion line is written only after each function
returns. The resulting module needs only `wasi_snapshot_preview1.fd_write`;
there is no compiler test-host API, JavaScript shim or ROS import.

## Build and run on Linux

Run all generation, compilation and execution commands inside the existing
64 GiB, swap-disabled test cgroup. Its identity and limits must be checked by
the caller before starting. This preparation script does not supervise
processes, set resource limits or create a cgroup. Use an owned external work
directory, serial compiler jobs and a bounded process-tree supervisor.

Select one catalog and matching case:

| Case | Source catalog | Methods |
| --- | --- | ---: |
| `fixture` | `test/0073.ros-tests/support/source/ros_custom_types/moonbit-records/fixture/catalog.preparation.json` | 5 |
| `joint1` | `test/0073.ros-tests/support/source/ros_custom_types/official-standard/ros1/catalog.preparation.json` | 1 |
| `joint2` | `test/0073.ros-tests/support/source/ros_custom_types/official-standard/ros2/catalog.preparation.json` | 1 |

The following commands show the fixture case from the repository root. Set
`work` to a **fresh directory outside the checkout** and use the maintained
MoonBit toolchain identified in the qualification report. No network package
installation is needed: the SDK is a local workspace member and MoonBit core
comes from that toolchain.

```sh
work=/absolute/path/to/fresh-owned-codec-work
mkdir -p "$work/project/guest" "$work/project/stdio"
python3 -I -B sdk/origin/custom-types/moonbit_profile.py \
  test/0073.ros-tests/support/source/ros_custom_types/moonbit-records/fixture/catalog.preparation.json \
  --language moonbit --out "$work/generated"
cp test/0073.ros-tests/support/source/ros_custom_types/moonbit-records/moon.mod "$work/project/"
cp test/0073.ros-tests/support/source/ros_custom_types/moonbit-records/moon.work "$work/project/"
cp -R sdk/origin/moonbit "$work/project/ros-sdk"
cp -R "$work/generated/moonbit" "$work/project/records"
cp sdk/origin/examples/moonbit/stdio/moon.pkg "$work/project/stdio/"
cp sdk/origin/examples/moonbit/stdio/write.mbt "$work/project/stdio/"
python3 -I -B test/0073.ros-tests/support/source/ros_custom_types/moonbit-records/prepare_uwvm_app.py \
  fixture "$work/project/guest/main.mbt" > "$work/transformation.json"
cat > "$work/project/guest/moon.pkg" <<'PKG'
import {
  "uwvm/moonbit_record_contract/records",
  "uwvm/moonbit_record_contract/stdio",
  "moonbitlang/core/float",
}
options("is-main": true, link: {
  "wasm": {
    "export-memory-name": "memory", "heap-start-address": 8519680,
    "memory-limits": { "min": 256, "max": 1024 },
  },
  "wasm-gc": {
    "export-memory-name": "memory", "use-js-builtin-string": false,
    "memory-limits": { "min": 256, "max": 1024 },
  },
},)
PKG
moon -C "$work/project" build --frozen --target wasm --debug -j 1 \
  --verbose --target-dir "$work/build-wasm-debug"
```

For each of `wasm` and `wasm-gc`, build both `--debug` and
`--release --no-strip` into different directories. Retaining Release names
permits the pinned allocator initialization check; it does not enable language
debugging. Validate each actual `guest/guest.wasm` using `wasm-tools validate`
and retain its `wasm-tools print` output. Before execution, check the exact
`fd_write(i32,i32,i32,i32)->i32` import, the `memory` and `_start` exports,
memory limits and the low-memory reservation. The linked application may have
zero or one core start section; `_start` is the WASI entry. The ASCII writer
uses addresses 0 through 288, so a generic unreserved linker layout is unsafe.

Run the selected artifact with the interpreter and the feature switches used
by the frozen qualification products:

```sh
uwvm2-ros -Rint -WFE-gc -WFE-exceptions -WFE-tail-call \
  -WFE-function-references -WFE-reference-types -WFE-simd \
  -WFE-relaxed-simd -WFE-extended-const -WFE-multi-memory \
  -WFE-memory64 -WFE-table64 --wasm-set-main-module-name codec \
  --run /absolute/path/to/selected/guest/guest.wasm
```

The fixture must exit zero with exactly five lines, `PASS_CODEC_fixture_0`
through `PASS_CODEC_fixture_4`, and empty stderr. Each JointState case must
produce its one corresponding line. No ROS command-line grant is required.

## Failure control and interpretation

In a separate fresh application, add `--mutate-golden` to the preparation
command for `fixture`. This changes only the first golden byte from `01` to
`00`. Build it in both targets and profiles. UWVM must exit nonzero with an
unreachable trap and no completion line. This checks that a failed assertion
cannot be reported as success; it is an expected negative outcome.

The preparation JSON binds the original and transformed source hashes, case
names, assertion-call count and expected stdout. Retain the generated record
sources, original tests, transformed application, package manifests, actual
compiler commands, Wasm, WAT and both output streams. An independent verifier
can reconstruct the narrow source changes without executing archived code.

These checks exercise real codec computation inside UWVM. A JointState codec
pass does not prove native middleware delivery or authenticate the preparation
catalog's type identity. The native gateway, policy and complete application
must be qualified separately. Debug and Release here describe build profiles;
they do not claim managed debugger support, cache safety, architecture coverage
or performance parity.
