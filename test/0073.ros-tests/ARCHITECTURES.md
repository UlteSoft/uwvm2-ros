# ROS host architecture checks

The SDK currently selects ROS 1 Noetic and ROS 2 Humble/Jazzy. The host VM
architecture is independent of a guest's `wasm32` or `wasm64` memory model.
An architecture listed by ROS is a test target, not proof that this VM build
has passed. Acceptance requires execution of the exact compiled product.

| Host target | Noetic | Humble / Jazzy | Product qualification |
| --- | --- | --- | --- |
| Linux x86_64 / amd64 | Listed in REP 3 | Ubuntu Tier 1 | Native execution and QEMU CPU checks |
| Linux AArch64 / arm64, little-endian | Listed in REP 3 | Ubuntu Tier 1 | QEMU CPU/VM checks; native ROS processes checked separately |
| Linux ARMv7 hard-float / armhf, little-endian | REP 3 arm32 | Tier 3, source builds | Separate EHABI, libc and native ROS qualification |
| i686 / i386 | Not in Noetic's list | Not in these release lists | Outside the selected release scope |
| armeb / AArch64 big-endian / ARM soft-float | No specified matching release ABI | No specified matching release ABI | No support inferred from the word “ARM” |

Sources: [ROS REP 3](https://github.com/openrobotics/reps/blob/main/_posts/rep-0003.md)
and [ROS REP 2000](https://github.com/openrobotics/reps/blob/main/_posts/rep-2000.md).
Noetic is an archived release; its target list is retained to qualify the
explicit SDK profile, not described as a currently maintained ROS release.
Humble targets Ubuntu 22.04 and Jazzy targets Ubuntu 24.04 for Tier 1.

ARM32 uses the source-paired libunwind hook for owned JIT exidx tables and
bounded `dl_iterate_phdr` lookup for loader-owned static images. LLVM provider
revision 14 also repairs callee-pop alignment and early-return stack restoration.
The exact ARMv7 hard-float build passed full-system QEMU cache, checked-unwind and
24 native call ABI cases with target libc 2.35. This removes the implementation's
previous `_dl_find_object` dependency, but does not qualify Noetic's stock Focal
libc, ARM32 native debugging, or every native ROS endpoint. See the exact hashes
and remaining limits in the
[runtime qualification report](../../documents/toolchain/ros-runtime-qualification.md).

NVIDIA support is a hardware/software tuple, not an extra generic CPU family.
[Isaac ROS 3.2](https://nvidia-isaac-ros.github.io/v/release-3.2/getting_started/index.html)
uses Humble with the documented Jetson Orin/JetPack or x86_64/CUDA configuration.
[Isaac ROS 4.3](https://nvidia-isaac-ros.github.io/v/release-4.3/getting_started/index.html)
uses Jazzy and lists specific Jetson Thor, x86_64 GPU and DGX Spark platforms.
[Isaac Sim 5.1](https://docs.isaacsim.omniverse.nvidia.com/5.1.0/installation/requirements.html)
limits its AArch64 builds to DGX Spark. These versioned examples do not qualify
another Isaac release or an arbitrary pinned upstream source commit.
QEMU userspace CPU execution cannot qualify CUDA, TensorRT, sensors, GPU
drivers or a Jetson BSP. Those results require the corresponding hardware.

## Short product check

Build the Linux LLVM product with both ROS imports and native debugging, then
run `xmake basic_test`. Python 3's standard library is sufficient; no separate
Wasm compiler is needed. The command executes the built target and requires:

- A completed signed AOT cache write and two fresh-process verified hits.
- Real WASI and ROS owner validation; a revoked clock grant stays revoked.
- Checked native unwinding with the exact three-function trap chain.
- Refusal of removed `instruction`, `none`, `auto` and `unwind-uncheck` options.
- Live native disassembly, native stepping and rejection of stale references.

Raw logs and a JSON receipt go in a fresh folder under the build directory;
`xmake basic_test --output=PATH` selects another parent. A container blocking
the required kernel keyring operations fails the cache check with its actual
diagnostic. It must not be recorded as a cache-hit timing measurement.
This smoke check does not replace native ROS graph tests, architecture
qualification, or vendor hardware tests. It grants no ROS endpoints.
