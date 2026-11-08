# Native host MTE provider probes

These probes test the AArch64/Linux compiler, linker, loader, allocator and thread prerequisites for **native VM host** MTE. They do not test guest WasmMTE and do not qualify a complete VM build.

Run `qualify.py --help`, supplying an existing compiler and cross flags with repeated `--cflag=...` arguments. For QEMU, also provide `--qemu`, `--runtime-prefix` and optionally `--cpu=max`. All output must be under the repository's `.tmp`; the runner requires the 64 GiB, zero-swap cgroup and CPU affinity excluding core 0. It does not install or replace anything.

Example argument structure (paths must refer to existing dependencies):

~~~sh
python3 test/0086.memtag/host_native/qualify.py \
  --compiler=/existing/llvm/bin/clang \
  --cflag=--target=aarch64-linux-gnu \
  --cflag=--sysroot=/existing/cross-sdk \
  --cflag=--gcc-install-dir=/existing/cross-sdk/usr/lib/gcc-cross/aarch64-linux-gnu/15 \
  --cflag=-idirafter \
  --cflag=/existing/cross-sdk/usr/aarch64-linux-gnu/include \
  --qemu=/existing/qemu-aarch64 \
  --runtime-prefix=/existing/cross-sdk/usr/aarch64-linux-gnu \
  --out=.tmp/native-host-mte-probe
~~~

`control.c` observes native pointers and faults without sanitizer instrumentation. `payload.c` is separately compiled with no instrumentation, stack, heap, globals and combined native MTE profiles. The test uses inline assembly for deliberate invalid accesses, avoiding optimizer elimination based on C undefined behavior. Core dumps are disabled before fault probes. Each case runs in a separate process.

The runner does real links with fatal linker warnings and records ELF metadata. Each successfully linked profile executes 21 cases with both the default allocator policy and `glibc.mem.tagging=3`: main and worker stack/heap, globals, explicit protected worker stack, wrong-tag reads/writes, use after free, double free, and calloc/realloc/alignment lifecycle. Signal code 9 is accepted only after the intended negative-test marker. The explicit-stack case demonstrates a limited primitive and is not treated as loader qualification.

An exit status of zero from the Python runner means observations were collected. Read `provider_prerequisites_pass` to determine whether the tested prerequisites passed. `full_vm_qualified` is always false for this bounded probe: it does not cover the complete VM, native dependencies, TLS, alternate/switchable stacks, loader cooperation across shared libraries or physical-hardware performance. Unsupported compiler/linker behavior and failed positive cases remain explicit in the report.

See [the native host MTE build policy](../../../documents/toolchain/native-host-mte.md) for the current gate and qualification gaps.
