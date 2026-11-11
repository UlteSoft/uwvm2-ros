# Apache NetBeans

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` and `-m debug` (including their long spellings) default to `full`; other launches default to `limited`. See [Debugger permissions](../../documents/command-line/debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

The UWVM module adds **Run > Attach UWVM Wasm...** and connects the genuine
NetBeans DAP client to the authenticated Python stdio adapter. It accepts a
generated NetBeans configuration, validates the selected host profile using
the same importer, and preserves local or SSH transport. It does not open a
TCP listener or infer any WASI/ROS permission.

Build with an existing NetBeans installation and JDK:

```sh
python3 tools/debug/netbeans/build.py --netbeans /path/to/netbeans \
  --jdk /path/to/jdk --output /private/output/org-uwvm-debug.jar
```

The build reuses the IDE API jars and performs no downloads. Install the
module in the selected NetBeans user directory and enable it using the normal
NetBeans module mechanism. Run the unified Python importer and select the
returned `netbeans.source`, `netbeans.wasm`, or `netbeans.native` JSON in the
attach action. A host-authorized broker must already be running; use the
importer's `broker` operation when the profile has explicit launch arguments.

## NetBeans 31 prepared-stop compatibility

NetBeans 31's DAP client unconditionally unboxes the optional DAP
`stopped.threadId`. A UWVM prepared pause can legitimately have no guest
thread because no Wasm instruction has executed. The exact-release patch
in `netbeans/patches/netbeans31-prepared-stop.patch` preserves a paused
session without inventing a guest thread or frame. Continue then uses
UWVM's explicit zero control identifier, which the adapter accepts only
for an identified empty prepared stop; it rejects zero after any guest
participant exists. Thread-level stepping remains unavailable until a real
thread exists.

Apply this patch to the matching Apache NetBeans 31 source and rebuild the
DAP client in an isolated installation. Do not substitute a thread ID,
auto-execute the guest during attach, or disable adapter validation as a
workaround. Retain the original IDE module to make the isolated build
reversible.

The patched NetBeans 31 client has passed an actual desktop source test:
SDK C++ gutter breakpoint, authenticated DAP attachment, source stop, source
step to another line, and standard scopes. Its stock client does not yet
provide UWVM virtual Wasm listing, native disassembly, MTE colors, or custom
host-trace panels. Those require client UI implementation and separate
acceptance; selecting a generated configuration is not a qualification.
