# UWVM Command-Line Reference

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` defaults to `full`; other launches default to `limited`. See [Debugger permissions](debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

This directory is the source-driven command-line reference for the `uwvm2-ros` executable. It expands the compact `uwvm2-ros --help` output into behavior notes, parameter interaction rules, platform gates, path handling details, and practical combinations.

The command registry and callbacks used for this reference are primarily in:

- `src/uwvm2/uwvm/cmdline/params.h`
- `src/uwvm2/uwvm/cmdline/parser.h`
- `src/uwvm2/uwvm/cmdline/params/**`
- `src/uwvm2/uwvm/cmdline/callback/**`
- selected runtime, Wasm loader, logging, and WASI storage headers referenced by those callbacks

Build-time `xmake f ...` switches are documented separately in [`../xmake-options.md`](../xmake-options.md).

## Documents

- [Parsing And Paths](parsing-and-paths.md): argument preprocessing, duplicate handling, `--run`, strict parsing, file path classes, and Windows NT `::NT::` paths.
- [Operator Whitepaper](../user-whitepaper.md): build, deployment, authority, debugging, updates and exit status.
- [Complete CLI Catalog](cli-catalog.md): every command advertised by the qualified integration binary.
- [Crash Policy](crash-policy.md): immutable crash handling and dump boundaries.
- [Global And Debug Commands](global-debug.md): `--help`, `--mode`, `--run`, `--version`, and `--debug-test`.
- [Language Debugger Build Options](language-debug-build.md): independent language switches, macros, explicit selection and build validation.
- [WebAssembly Commands](wasm.md): module loading, explicit start-function invocation, preload modules, native dynamic libraries, import binding rewrites, weak symbols, parser limits, initializer limits, memory limits, preload memory attributes, and strict memory growth.
- [Secure Module Verification](../security/wsea.md): opt-in `--secure module`, `--secure help/status`, `--help secure`, protected offline policy, signatures and revocation evidence.
- [Runtime Commands](runtime.md): runtime mode/compiler selection, shortcut conflicts, LLVM IR verifier control, compile-thread resolution, scheduling, and compiler logs.
- [Logging Commands](logging.md): diagnostic output routing, color policy, warning enable/disable/fatal conversion, verbose mode, and Windows ANSI mode.
- [WASI Commands](wasi.md): core WASI option, WASI Preview 1 global/single/group targets, environment variables, mounts, trace output, sockets, and target override rules.

## Quick Availability Model

The ROS build always includes WASI Preview 1 (memory32 and memory64), both
socket ABI implementations, ROS1 and ROS2. Xmake rejects attempts to remove
those interfaces. Whether a guest may import or call them is an explicit
per-module runtime policy; a compiled interface is not a permission grant.

Other command availability can depend on the optional interpreter, internal
debug build hooks, preload support and platform-specific management transport.
The ROS product admits only its five Linux GNU target ABIs; a cross-platform
help description does not admit a Windows or macOS ROS deployment.

The binary itself is always authoritative:

```bash
uwvm2-ros --help all
uwvm2-ros --help global
uwvm2-ros --help debug
uwvm2-ros --help update
uwvm2-ros --help ros1
uwvm2-ros --help ros2
uwvm2-ros --help wasm
uwvm2-ros --help runtime
uwvm2-ros --help log
uwvm2-ros --help wasi
```

## Syntax Conventions

- `None` means no argument is consumed.
- `<name:type>` means one required argument is consumed.
- `(<name:type>)` means an optional argument may be consumed when the next token is still a plain argument.
- `[a|b|c]` means the slot accepts one of the listed literal forms.
- `[a|<int>]` means the slot accepts either a literal or a value parsed as the stated type.
- `...` means all later guest tokens belong to `--run` and are not parsed as host options.
- When `--run`, `-r`, or `--` is absent, trailing plain unconsumed arguments may form an implicit run tail.
- `path` values are host-side paths and can have platform-specific behavior.
- `str` values are raw command-line strings unless a command-specific rule validates them.
- `size_t`, `ssize_t`, and `i32` are parsed strictly: the whole token must parse and fit the target type.

## Reading Order

Read [Parsing And Paths](parsing-and-paths.md) first if you need exact behavior. Many surprising cases, especially `-1` as a value, optional arguments, `--run`, and `::NT::` paths, are parser-level behavior rather than individual command behavior.
