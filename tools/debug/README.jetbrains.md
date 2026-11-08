# IntelliJ IDEA and CLion

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` defaults to `full`; other launches default to `limited`. See [Debugger permissions](../../documents/command-line/debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

The UWVM LSP4IJ bridge launches the Python stdio adapter and sends DAP
`attach` to the existing authenticated broker. It never uses native PID
attach, opens a DAP TCP listener, grants guest interfaces, or changes runtime
arguments. Local and SSH transport use the same owner-only import profile.

Build the small plugin using an existing JetBrains IDE/JDK and an existing
LSP4IJ 0.21.0 plugin:

```sh
python3 tools/debug/jetbrains/build.py --ide /path/to/idea \
  --lsp4ij /path/to/plugins/lsp4ij --output /private/output/uwvm-debug.zip
```

Install that ZIP into the selected IDE through **Plugins > Install Plugin
from Disk**. The build command does not download or reinstall the IDE.
The bridge currently targets the LSP4IJ 0.21.0 API; other versions require
a compatibility build and a real acceptance run.

Run `import_ide.py import --profile /private/profile.json --output /private/project`
and open the returned `jetbrains` project directory. The imported project
contains source, Wasm and native attach configurations. Launch the exact
host-owned broker first with `import_ide.py broker --profile /private/project/profile.json`
when the profile includes a runtime launch; otherwise attach to its existing
broker. Choose the corresponding **UWVM** run configuration and Debug.

The generated project references the original workspace as content. It does
not replace that workspace's existing `.idea` directory. Source maps retain
their exact original direction and module IDs remain exact integers.

Real IntelliJ IDEA Community 2025.2.6.4 with LSP4IJ 0.21.0 has passed a source
acceptance run using an SDK C++ Wasm: imported run configuration, real gutter
breakpoint, actual stopped source editor, stepping to another source line,
and standard DAP scopes. Its **UWVM Inspection** tool window has also passed
real visible-table checks against a Clang-generated WasmMTE module: allocation
tag hex values, exact backend palette colors, guest bytes, zero tags, disabled
MTE, and refusal of a foreign module selector. It uses only this project's
selected, paused UWVM session; pages are not a total memory-size limit.

The window also provides host-call pages, raw call bytes and host-side JSON
export. Those custom host-trace controls still need their own real UI acceptance
run. The current evidence does not qualify every language, a visible native
assembly editor, or every JetBrains product. CLion's built-in DAP support
is a separate client and needs its own acceptance run; a configuration file
alone is not that evidence.
