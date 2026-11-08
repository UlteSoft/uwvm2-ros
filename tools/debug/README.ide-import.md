# Import a host-owned IDE debug project

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` defaults to `full`; other launches default to `limited`. See [Debugger permissions](../../documents/command-line/debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

`import_ide.py` creates isolated debug configurations for VS Code, Cursor,
Neovim, the UWVM JetBrains bridge and the UWVM NetBeans bridge. Automatic editor
opening is currently provided for VS Code, Cursor and Neovim. The same profile
selects either a local adapter or an adapter carried
over authenticated SSH standard input/output. It never adds WASI or ROS grants,
starts a public DAP listener, or copies a broker capability into a project.

The importer currently targets Unix hosts. The existing Windows named-pipe
adapter remains available through [manual configuration](README.dap.md).
For remote profiles, the target broker runs on Linux.

## Prepare a profile

Save the following as an absolute, host-owned regular file with mode `0600`.
Use a private directory outside every guest preopen. Paths are examples; replace
them with the actual host paths.

```json
{
  "format": "uwvm-debug-project-v1",
  "name": "Robot controller",
  "workspace": "/home/operator/robot",
  "transport": {
    "kind": "local",
    "python": "/usr/bin/python3"
  },
  "broker": {
    "socketDir": "/home/operator/private/robot-debug"
  },
  "moduleId": 0,
  "stepLevel": "source",
  "sourceMap": {
    "/application": "/home/operator/robot"
  },
  "language": "cpp"
}
```

This is an attach-only profile. Start the VM through the existing secure broker
before attaching. The broker's directory, authorization and guest-preopen checks
still apply. The adapter cannot turn an ordinary running VM into a debug VM.

For explicit launch authority, add a `runtime` object containing
`"executable": "/absolute/path/to/uwvm2-ros"` and an `"arguments": [...]` array.
Copy the exact working debug invocation, including `--mode debug`, the runtime
selection, module path, and every individual WASI/ROS permission. The importer
preserves each argument; it does not infer permissions from the source language,
file extension or selected IDE. Use `import_ide.py broker --profile ...` to
launch this profile. Generated Code/Cursor configurations also offer that broker
command as their background pre-launch task.

A `sourceMap` maps real compiler path prefixes to local source roots. Only add
mappings required by that compiler. It does not authorize arbitrary host reads.
For native compiler limitations, see
[native source metadata](../../documents/runtime/debug-native-languages.md).

## Generate and open

```sh
python3 /absolute/uwvm2-ros/tools/debug/import_ide.py validate \
  --profile /home/operator/private/robot-debug.json
python3 /absolute/uwvm2-ros/tools/debug/import_ide.py import \
  --profile /home/operator/private/robot-debug.json --open code
```

Use `--open cursor` for Cursor. For Neovim, select an existing genuine
`nvim-dap` installation with `--nvim-dap /absolute/path/to/nvim-dap` and
`--open neovim`. No plugin or compiler is automatically installed.

The default output is the owner-only `.uwvm-debug` directory inside the
workspace. `--output /absolute/private/directory` selects another location.
The importer writes a separate `uwvm.code-workspace`, a Neovim configuration,
a generic `dap.json`, and a normalized profile. It leaves the project's
`.vscode/launch.json` unchanged. Repeating the same import is idempotent;
changing generated files requires `--replace`. Do not place this directory
inside a guest-mounted host tree.

Code/Cursor start with this repository's debug extension loaded. Choose one of
the source, Wasm or native configurations. In Neovim, use `:UwvmAttach source`,
`:UwvmAttach wasm` or `:UwvmAttach native`; see
[Neovim commands](README.neovim.md).

## Remote transport

Replace `transport` with:

```json
{
  "kind": "ssh",
  "host": "operator@robot-host",
  "port": 22,
  "python": "/usr/bin/python3",
  "debugDirectory": "/opt/uwvm2-ros/tools/debug",
  "identityFile": "/home/operator/.ssh/robot_debug",
  "knownHostsFile": "/home/operator/.ssh/known_hosts"
}
```

The broker socket directory, runtime executable and runtime arguments now name
paths on the remote host; the workspace and source-map destinations remain
local. Install the matching debug tools on that host through the deployment
process, and verify its host key before use.

SSH uses batch authentication, mandatory strict host-key checking, no agent
forwarding and no port forwarding. Adapter messages travel through SSH stdio;
the remote adapter authenticates to the remote private broker. The profile
cannot weaken these settings or inject arbitrary SSH options. A missing key,
unknown host or changed host key fails the connection. An IDE session's virtual
Wasm sources remain bound to that actual local IDE session.

An SSH configuration does not grant guest networking, permit cross-process
debugging, or authorize a guest to access the broker. Continue to keep remote
broker files outside all guest preopens.

## Other DAP clients and qualification

`dap.json` supplies the adapter executable/argument vector and three attach
argument objects. A real DAP client must consume these settings. Generated
configuration alone is not evidence that a particular IDE works.

Current CLion has a [built-in DAP stdio client](https://www.jetbrains.com/help/clion/dap-support.html).
Select the generated executable and arguments, choose **Attach**, and use the
corresponding attach object. Do not select a GDB/LLDB process-attach backend:
UWVM's native view stays restricted to its own Wasm JIT code.

IntelliJ IDEA can use the [LSP4IJ DAP client](https://github.com/redhat-developer/lsp4ij).
NetBeans and JDeveloper require their own IDE integration and acceptance tests.
Their names in a configuration manifest must not be interpreted as completed
UI, source-language, host-trace or memory-tag coverage.

Validation distinguishes actual editor sessions from protocol tests, local
sessions from SSH sessions, and engine source from a guest interpreter's source.
A JavaScript program running inside a Wasm interpreter, for example, does not
acquire JavaScript source stepping merely because the interpreter's Wasm
instructions can be inspected.

JetBrains and NetBeans have dedicated bridge instructions: [JetBrains](README.jetbrains.md) and [NetBeans](README.netbeans.md). The importer also writes their exact attach configurations; client and language qualification remains separate.


Linux full-JIT startup waits for the first authenticated VM reply, the original
VM process to exit, or the IDE to cancel. It has no default compilation deadline.
The private broker and DAP adapter use socket/pidfd readiness; neither grants a
guest additional interfaces or executes guest code to manufacture readiness.
Authentication and later command transport deadlines remain separate.

An operator may set `startupTimeoutSeconds` to a positive finite number in the
IDE profile or DAP attach configuration. The broker CLI exposes the corresponding
`--startup-timeout SECONDS` option before `--`. This is an explicit deadline for
the first reply, not a permission quota. IDE Cancel, Disconnect, stdin closure,
or peer exit interrupts a pending attach. Once a command has been forwarded,
cancellation retires that original launch and its private endpoint, so a late
unsequenced reply cannot satisfy a later client's command.
