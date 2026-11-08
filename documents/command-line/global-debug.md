# Global And Debug Commands

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` defaults to `full`; other launches default to `limited`. See [Debugger permissions](debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

Source focus:

- `src/uwvm2/uwvm/cmdline/params/{help,mode,run,version,debug_test}.h`
- `src/uwvm2/uwvm/cmdline/callback/{help,mode,version,debug_test}.h`
- `src/uwvm2/uwvm/cmdline/parser.h`

## Command Table

| Command | Aliases | Arguments | Repeatability | Availability | Behavior |
| --- | --- | --- | --- | --- | --- |
| `--help` | `-h` | `([all|global|debug|update|wasm|secure|log|runtime|wasi|ros1|ros2])` | Once | Core | Print category help and return success immediately. |
| `--mode` | `-m` | `[section-details|validation|run|debug]` | Once | Core | Select the top-level operation mode. |
| `--run` | `-r`, `--` | `<file argv[0]:path> <argv[1]:str> ...` | One parser boundary | Core | Set the guest Wasm file and stop host option parsing. |
| `--version` | `-v`, `-ver` | None | Once | Core | Print the build and feature report, then return success immediately. |
| `--debug-test` | None | None | Once | `_DEBUG` only | Run an internal debug-only test hook. |

## Debugger permissions

Use `--debug-permissions full|read-only|limited|none`. `-Rdbg` defaults to `full`; other launches default to `limited`. Explicit selections win, and the permission cannot be changed during a session. The host broker preserves the exact launch selection and does not silently grant full permissions.

`full` permits inspection and qualified mutation. `read-only` permits complete inspection and dumps while refusing state/code mutation and stepping. `limited` permits non-sensitive status and redacted dumps. `none` starts no debugger endpoint and denies dump. A separate authenticated FD/HANDLE is required for an actual session; Guest stdin is never repurposed.

See [Debugger permissions](debug-permissions.md) for the exact command and IDE contract, [bounded diagnostics](../runtime/release-diagnostics.md) for the metadata-only endpoint, and [production updates](../runtime/production-update.md) for independently signed update authority.

## `--help`

`--help` is an immediate inspection command. Without an argument, it prints global commands plus pointers to category-specific help.

Accepted extra help names:

- `all`: every compiled command in the current binary.
- `global`: global commands only.
- `debug`: debug commands only.
- `update`: independent managed launch and production update commands.
- `secure`: offline artifact verification commands, when compiled.
- `ros1` / `ros2`: exact ROS binding and operation commands, when compiled.
- `wasm`: WebAssembly commands only.
- `runtime`: runtime commands only.
- `log`: logging commands only.
- `wasi`: WASI commands only; WASI implementations are mandatory in this ROS product.

The extra help name is a normal argument and is marked occupied by the callback. Unknown names fail with an "Invalid Extra Help Name" error and show usage. Because the command returns immediately, do not place options after it expecting normal execution to continue.

Examples:

```bash
uwvm2-ros --help
uwvm2-ros --help all
uwvm2-ros -h runtime
```

## `--mode`

`--mode` selects `uwvm::wasm::storage::execute_wasm_mode`.

Accepted values:

- `run`: load, validate/initialize as required, and execute the selected entry.
- `validation`: validate the module graph without executing it.
- `section-details`: inspect and print section-level details instead of running the guest.
- `debug`: request the LLVM debugger, subject to the immutable debugger permissions and authenticated host channel; this mode grants no guest authority.

Notes:

- The default is `run`.
- The value is strict and case-sensitive.
- The option has an `is_exist` guard. Repeating it through any spelling is a duplicate-parameter error before callback behavior matters.
- The mode does not itself supply a module path. Normal runs need either `--run <file>` or an implicit trailing run tail.

Examples:

```bash
uwvm2-ros --mode validation --run app.wasm
uwvm2-ros -m section-details -r app.wasm
```

## `--run`

`--run` is not implemented as a normal callback. The parser special-cases it during preprocessing.

Behavior:

- The next token is required and becomes the Wasm file path.
- All following tokens are guest arguments.
- No following token is parsed as a host option.
- `-r` and `--` behave exactly like `--run`.

When no explicit `--run`, `-r`, or `--` appears, a trailing suffix of plain unconsumed arguments is accepted as an implicit run tail after callbacks finish. The first token in that suffix is the Wasm path and the rest are guest argv entries. Use an explicit boundary when guest argv contains option-looking tokens that begin with `-`.

The first token after `--run` is documented as `<file argv[0]:path>` because it both identifies the host Wasm file and becomes the natural guest argument root for the command line. WASI `argv[0]` can later be overridden with `--wasip1-*-set-argv0`, and the internal module name can be overridden with `--wasm-set-main-module-name`; those are separate concepts.

Examples:

```bash
uwvm2-ros --run app.wasm
uwvm2-ros -r app.wasm hello world
uwvm2-ros --runtime-aot -- app.wasm --guest-option
uwvm2-ros --runtime-aot app.wasm hello world
```

Host option placement:

```bash
uwvm2-ros --runtime-aot --run app.wasm
```

This selects full-module LLVM AOT.

```bash
uwvm2-ros --run app.wasm --runtime-aot
```

This passes `--runtime-aot` to the guest.

## `--version`

`--version` prints a formatted build report and exits successfully. It is not just a semantic version string.

The report can include:

- UWVM logo and version information.
- Git/build metadata when compiled in.
- Build mode and compiler information.
- Platform and ISA details.
- Allocator and install-path information where available.
- Runtime backend support, including the LLVM version when AOT support is compiled.
- Compiled WebAssembly feature support.
- WASI-related support when compiled.

Because it is immediate, it is best used alone or after only output-routing options such as `--log-output`.

Example:

```bash
uwvm2-ros --version
uwvm2-ros --log-output file version.txt --version
```

## `--debug-test`

`--debug-test` exists only in `_DEBUG` builds. It is a developer hook, not a stable end-user command.

Behavior:

- No arguments are consumed.
- The command has an `is_exist` duplicate guard.
- In release builds it is not registered. The same token is then an invalid parameter.

Use it only when working on the VM itself.

In the ROS product, full and read-only debugging can inspect the configured Guest WASIp1 arguments,
environment, descriptors and preopens. It cannot modify their launch contract,
create or duplicate native descriptors, or restore a prior host resource table.
These requests are rejected at the runtime boundary as well as the command parser.
