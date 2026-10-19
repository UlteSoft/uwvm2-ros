# Explicit SDK33 C++ termination dependency

The six WASIp1 C++ main recipes require a separately prepared official
termination object. The freestanding C/C++ providers do not use this policy.
There is no fallback that adds `fd_close`, `fd_seek` or another WASI import to
the existing basic-stdio contract.

This profile is for the SDK VERSION containing LLVM
`4434dabb69916856b824f68a64b029c67175e532` / LLVM22.1.0 and the exact five
upstream files in [official-inputs.json](official-inputs.json). The VERSION
file is part of the selected input identity. A version label does not
authenticate a compiler distribution, its loader/shared libraries or sysroot.
Authenticate the SDK's actual distribution using its upstream release process,
record the actual compiler byte SHA256, and inventory the entire selected tool
and SDK roots. Compiler host architecture is operator-selected; this tool
records physical bytes and invocation aliases without inventing a host ABI or
claiming that an unexecuted compiler works on that host.

## Obtain unchanged upstream inputs

Use the official [LLVM repository at the complete revision](https://github.com/llvm/llvm-project/tree/4434dabb69916856b824f68a64b029c67175e532).
Preserve these paths below one physical `OFFICIAL_ROOT`:

- `libcxxabi/src/abort_message.cpp`
- `libcxxabi/src/abort_message.h`
- `libcxxabi/include/cxxabi.h`
- `libcxxabi/include/__cxxabi_config.h`
- `LICENSE.TXT`

The portable lock includes each original Git blob OID, byte count, SHA256 and
immutable official Blob API URL. Preserve the license with the external source
copy. The preparer performs no network operation, download, extraction,
post-install action or SDK write. It refuses changed bodies or missing headers;
it does not generate replacement headers. TLS/Git content integrity and a
matching VERSION are distinct from a release signature or SDK-package trust.

## Prepare the explicit command

Use Python3.10 or later on a platform with ordinary POSIX descriptor APIs.
`SDK_ROOT`, `OFFICIAL_ROOT`, `COMPILER` and `OUT` below must be physical absolute
paths. The compiler invocation may end in the SDK's `clang++` symlink; its
spelling and resolved bytes are both recorded. Supply the actual tool digest,
not a digest copied from another architecture or package.

```sh
python3 -B sdk/examples/robotics_modular/cpp_termination/prepare.py \
  --sdk-root "$SDK_ROOT" \
  --official-root "$OFFICIAL_ROOT" \
  --compiler "$COMPILER" \
  --compiler-sha256 "$ACTUAL_COMPILER_SHA256" \
  --out "$FRESH_CPP_TERMINATION_OUT"
```

This writes `object-plan.json` and runs no compiler. Its status is deliberately
`PREPARED_COMMAND_NOT_EXECUTED`. The file contains an argv array, cwd, explicit
object/depfile paths, actual input pins and the finite command limits. Run that
array with a serial owned-process runner under the limits in `../recipes.json`:
512MiB per compiler,600s per command, one job, retained raw logs, exact original
64GiB/swap0 cgroup and its memory/backing reserves. Do not join the array into a
shell command or use `eval`. Keep the output outside every whole inventoried
input root. Reopen every pin and retain whole source/tool/SDK inventories before
and after actual execution. The metadata plan does not perform those execution
checks on the user's behalf.

The single official TU uses `-DLIBCXXABI_BAREMETAL -DNDEBUG`. This is **not** a
Guest/global NDEBUG setting. The preparer verifies the unchanged source has no
assertion invocation, exactly its official diagnostic guard, and an
unconditional final `abort()`. These defines remove only the official
termination diagnostic's stdio branch. They do not replace stdio or WASI
functions and do not make the failure path return successfully.

Retain the real successful compiler log/exit, fresh Wasm object and full `-MD`
file. Resolve relative dependency words from the recorded compile cwd; require
the object target, actual source and all four official compile inputs. Reopen
every dependency, including generated configuration, target headers and the
complete actual SDK/tool closure. A magic header or filename alone cannot
qualify the object.

## Use the object without weakening Guest assertions

Only after that compile and qualification, bind `CPP_TERMINATION_OBJECT` in the
six `main-cpp-*` recipes to the exact fresh object. Their Guest flags include
`-UNDEBUG` and the libc++ header customization
`-D_LIBCPP_VERBOSE_ABORT(...)=__builtin_trap()`. The existing libc++ failure
conditions still execute; an invoked verbose-failure handler traps inside Wasm
instead of pulling diagnostic printf machinery into the import graph. Reject
Guest/global `-DNDEBUG`, a different handler definition or an absent object.
Do not carry the official TU's two private defines into Guest compilations.

The direct object must be before driver-supplied C++ archives. Split each Guest
compile/link, retain full `-MD/-MF/-MT` evidence, and use the explicitly selected
Wasm linker with one thread, `--trace` and `--why-extract=PATH`. Preserve both raw
outputs. Require the trace to include the exact official object and fresh Guest
object. Refuse either `verbose_abort.cpp.o`/`.obj` from `libc++.a` or
`abort_message.cpp.o`/`.obj` from `libc++abi.a` in either trace or archive
extraction records. Successful linking does not waive this check.

Finally run the full Wasm validator and the existing independent closed binary
checks: exact ROS-version/scalar imports and basic WASI signatures, exports,
private memory bounds and absent start section. This policy neither adds
imports nor grants library/root ROS authority. Compile/link/validation and
Runtime/native ROS behavior remain separate results. A changed SDK revision
requires a separately reviewed policy and fresh evidence; this lock does not
claim compatibility with all WASI SDK versions.
