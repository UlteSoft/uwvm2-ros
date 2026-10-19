# Complete Unitree ROS2 HG SDK Source

Complete HG35-slot state, command, hands, BMS, board and pressure messages.

This directory contains actual C11, C++20, Rust, JavaScript, AssemblyScript and
TinyGo Source for **all 11 messages** in the explicitly locked
`unitree_hg` profile. Original fields and their order are retained. Finite
string/sequence capacities are explicit; oversized values are refused and never
silently truncated. Native identities are unbound, no registry is emitted, and
default authority is empty.

## Start with imports, then use complete typed values

Read the [raw backend ABI](../../ros2/abi.h), the actual
[C capability API](../../include/roswasm/ros2.h), and
[typed C++ API](../../include/roswasm/roswasmcpp.hpp). Include this directory's
`bounded_records.h` for C or `bounded_records.hpp` for C++20. Use counted
owned text/sequences and check every encode/decode result before exposing data.
The C++ `uwvm_custom` records specialize the actual
`roswasmcpp::Codec<T>`; no native SDK pointer crosses the import boundary.

The real [C telemetry example](../../robots/unitree/examples/complete_telemetry.c)
starts at the raw capability imports, decodes one full selected message and
checks cleanup. The [C++ telemetry example](../../robots/unitree/examples/complete_telemetry.cpp)
uses the genuine typed Subscription and copied callback value. Select
`UWVM_UNITREE_COMPLETE_FAMILY=2` and exactly this generated include
directory. Default `UWVM_UNITREE_COMPLETE_RECEIVE=0` expects permission denial13
with no live handle. A positive build explicitly selects1; the operator must
independently bind the real backend/alias, Native and canonical identity, QoS and
quotas. One poll may return11 when idle; the examples do not retry or infer delivery.

For example, with an explicitly reviewed Wasm C++ producer and its matching
standard library/sysroot, build outside the checkout:

```sh
/reviewed/wasm-sdk/bin/clang++ --target=wasm32-wasip1 \
  --sysroot=/reviewed/wasm-sdk/share/wasi-sysroot -std=c++20 \
  -I sdk/include -I sdk/robots/unitree/include \
  -I sdk/custom-types/unitree-hg-complete \
  -DUWVM_UNITREE_COMPLETE_FAMILY=2 \
  sdk/robots/unitree/examples/complete_telemetry.cpp \
  -o /fresh/owned/unitree-telemetry.wasm
```

This is a build recipe, not an executed Wasm or default-denial receipt. Inspect
the actual produced imports and select only the backend used here. Execute in
uwvm2 with its default empty ROS policy first. Adding a build option does not
grant permissions; see [vendor build options](../../../documents/ros/vendor-build-options.md).

Rust uses the real `sdk/rust` crate and matching backend feature with
`bounded_records.rs`. JavaScript uses `*Codec.defaultValue/encode/decode` from
`bounded_records.mjs`, retaining i64/u64 as BigInt. AssemblyScript includes the
real record and `*Codec` classes in `bounded_records.ts`; keep this directory
one level below `sdk/custom-types` for the genuine relative imports. TinyGo
resolves `uwvm2.local/roswasm` to the real `sdk/tinygo` module and selects the
matching backend. The [Unitree typed examples](../../robots/unitree/TYPED_EXAMPLES.md)
provide the existing ROS2 examples and exact producer contracts; their old
receipts do not qualify these new complete-domain Source revisions.

## Bind Native independently

`generation.vendor.json` binds the actual complete generator pair, every
generated product, original declarations and canonical identities.
`preparation.vendor.json` retains the actual preparation and original inputs.
Regenerate from the explicitly selected complete catalog as described in the
[Unitree guide](../../robots/unitree/README.md) and
[complete SDK guide](../COMPLETE_VENDOR_SDK.md). Build `native_identity.cc`
against the actual official SDK and retain the full original generated
schema/definition/constants/defaults and dependency/link closure before binding.
A previous bounded-v2 Native export cannot be relabeled as complete-domain
evidence. The required complete Native guard may refuse an older SDK lacking
independent original definition proof. No Native identity is fabricated here.

## Actual validation and limits

All actual executions below used the pinned64GiB/swap0 Linux cgroup.

The strengthened original-field corpus has three distinct payloads for each
of54 records:162 record/payload pairs, exercised in both C11 and C++20
(324 local scenarios). Scalar and fixed-array values vary as well as dynamic
prefixes. Original earlier fixed-only repeated-mode results remain retained.
The same corpus was additionally executed under user QEMU on x86_64, i686,
aarch64 and armhf:8 programs and324 scenarios per architecture. Eight newly
compiled Wasm programs executed the324 codec scenarios through the exact retained
R10 ROS Runtime ELF `664ea84cbf6520c249b075a1817978e489a0c44362e800167bc6cbaa3f1a5a10`.
Sixteen telemetry examples compiled/linked as real Wasm; with no ROS options or
grants, eight denial-mode applications returned0 only after observing13, and
eight receive-mode applications returned13. The Runtime remains a historical
immutable input, not a new qualification of the latest complete Runtime Source.

The independent expected bytes cover nested/fixed arrays, empty/single/full
dynamic prefixes, large signed/unsigned64 values and UTF-8. Tests also check
malformed lengths, booleans where present, truncation/trailing data and
transactional preservation. The large oracle programs explicitly declare a
1MiB stack, 2MiB initial memory and 32MiB maximum memory. Their earlier default
64KiB stack exhausted and was trapped; that original failure is retained.
Source generation, Native local codecs, target ELF checks, emulation, Wasm
execution and permission rejection each have separate original receipts.
These results establish neither native ROS delivery nor an official upstream
suite, simulation, controller, hardware performance or full rcl client-library
compatibility. Big-endian/soft-float Arm SDK execution remains unqualified.


Preserve the original selected Unitree repository license and third-party notices when supplying its dependencies. This generated SDK does not change those terms.
