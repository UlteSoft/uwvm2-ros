# ARM callee-pop caller spill restoration

The ARM and ARMHF LLVM JIT could read a caller spill before restoring SP after a
normal call to a TailCC function. The callee returned the correct value, but the
caller temporarily used the callee-popped stack pointer. Register allocation can
place the reload before ADJCALLSTACKUP. Debug optimization exposed this ordering;
optimized code sometimes avoided the spill and concealed the defect.

A minimal Wasm reproducer is in
`test/0013.uwvm_int/wasm3/fixtures/exception_control_joins/call_i64_spill.wat`.
For example, a real function call computing `0 + 2147483647` followed by a
comparison with `2147483647` incorrectly trapped on the old ARM backend.
In the observed caller the relevant order was:

```asm
bl   callee
ldr  r2, [sp, #8]  // wrong slot while SP is still displaced
sub  sp, sp, #8
```

The backend now emits the existing SP restoration immediately after the call,
before the spill reload. It does not change arguments, results, stack size,
tail-call transfers, memory checks, or native unwind selection. ARM and Thumb2
use the same repaired reserved-frame branch; zero-pop ordinary calls and setup
pseudo instructions do not enter it. The executable ROS qualification targets
ARM soft-float and ARM hard-float Linux.

`LLVM_UWVM_ARM_CALLEE_POP_SP_FIXED` is a capability of the rebuilt provider.
ROS ARM compilation rejects headers without it, and ARM object/checkpoint cache
keys include this admission revision. A new header or consumer definition alone
does not repair an old CodeGen archive. Rebuild the actual LLVMARMCodeGen object,
retain its source/archive hashes, then rebuild the runtime against that provider.
The vendored source inventory includes the backend and capability-template changes.
This addition is within the unpublished paired source; source inventory and
capability checks remain authoritative alongside the package version.

The portable backend patch is
`documents/toolchain/patches/llvm-arm-callee-pop.patch`.
The cross-platform driver
`test/0013.uwvm_int/wasm3/run_exception_control_joins.py` runs identical WAT
fixtures under each VM's interpreter/JIT and Wasmtime. Supply `--vm ARCH=LAUNCHER`
and `--runtime-binary ARCH=ELF` for each QEMU launcher, plus `--wasm-tools`,
`--wasmtime`, and a new `--out` directory. Run in the actual 64 GiB, zero-swap
cgroup. It records binaries, launchers, tools, source/compiled fixture hashes,
commands and raw streams. QEMU results establish correctness, not performance.

The SDK's original control_toolbox PID example supplies a larger source-level
regression: compare its 4,096-step trace and original diagnostic/error stream with
the native ROS library. The old backend preserved that trace but incorrectly
formatted some diagnostics; both streams must match. A successful process exit
or a numerical-only comparison is insufficient.
