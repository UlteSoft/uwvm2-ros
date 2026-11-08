# Neovim debugger integration

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` defaults to `full`; other launches default to `limited`. See [Debugger permissions](../../documents/command-line/debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

`neovim/uwvm.lua` connects a real `nvim-dap` client to the existing authenticated
UWVM adapter. It supplies read-only views for Wasm disassembly, native
instructions, WASI/ROS observations, raw payload bytes and memory tags.

Use the [profile importer](README.ide-import.md) to generate a configuration,
or load the module from your own Neovim configuration:

```lua
local uwvm = dofile("/absolute/uwvm2-ros/tools/debug/neovim/uwvm.lua")
uwvm.setup({root = "/absolute/uwvm2-ros", python = "/usr/bin/python3"})
uwvm.attach("/absolute/private/broker", "source", {
  moduleId = 0,
  sourceMap = {["/application"] = "/absolute/local/application"}
})
```

The supplied integration requires Neovim 0.10 or newer and a compatible genuine
`nvim-dap` plugin. It has been exercised with Neovim 0.10.4 and nvim-dap 0.9.0.
It does not install either dependency or start the VM implicitly.

## Three execution views

Select `source`, `wasm` or `native` when attaching. Ordinary nvim-dap stack,
scope, variable, breakpoint and step actions use the adapter's corresponding
level. Source stepping requires genuine compiler metadata. Missing information
is reported unavailable; no substitute source location is invented.

`:UwvmDisassemble [COUNT]` opens a read-only instruction page for the selected
frame. The Lua function `uwvm.disassemble(count, callback, instruction_offset)`
can request subsequent pages. A page limit is a presentation bound, not a limit
on the module's instruction count.

Wasm virtual source documents are tied to a live session and an actual owned
source reference. In those documents use `:UwvmWasmBreakpoint` at the current
line. This preserves the source reference even in nvim-dap versions whose
normal gutter action sends only a file path. Ordinary native-source files use
normal nvim-dap breakpoints. Retired documents cannot install breakpoints in a
new session.

## Host calls and memory tags

At a cooperative stop:

- `:UwvmHostTrace [all|wasip1|ros1|ros2]` displays retained call records.
- `:UwvmHostTraceBytes CALL SEGMENT` displays the recorded raw payload segment.
- `:UwvmExportHostTrace /absolute/host/path/calls.json` saves human-readable JSON.
- `:UwvmMemoryTags MODULE MEMORY OFFSET COUNT` displays actual bytes and their
  per-granule tag numbers and colors.

Host recording is opt-in through the existing `trace host on` command. A viewer
does not grant an interface, enable recording or change a quota. Export uses
the host-side adapter's path and existing policy; with SSH that path is on the
remote host.

Memory-tag offsets and lengths are passed as decimal strings without Lua
floating-point conversion. Each view contains at most 256 bytes; request another
offset to inspect another region. Tag zero, disabled tagging and unsupported
tag storage are distinct states. Colored extmarks use the same canonical
palette as the CLI. Mixed tags and unaligned page boundaries retain their actual
granule coverage.

Every asynchronous view checks that its original session and stopped frame
still exist before displaying a reply. Stale replies are rejected. These
commands expose only the VM-owned memory and code admitted by the controller;
they do not inspect the VM host, native libraries or another process.
