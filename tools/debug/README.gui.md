# UWVM inspection windows

Load this directory as the debug extension through the host-owned IDE importer.
VS Code and Cursor provide independent **Memory**, **Registers**, **WASIp1**,
**ROS**, **Breakpoints**, and **Diagnostics & Dump** editor tabs. The **UWVM**
view in Run and Debug exposes the three instruction levels and every independent
inspection window. Commands also remain available in the Command Palette
(`UWVM: Open …`) and debug toolbar. Every open creates a new instance;
**New window** duplicates the current kind without changing the original tab.
Drag tabs into editor groups to compare them side by side.

Each window owns its session binding, selectors, page and display preferences.
A duplicate inspection window retains its original session; **Session → Use
active session** explicitly switches it. The memory-write command targets the
focused memory window and refuses to guess a target when another kind is active.

The permission badge comes from the authenticated runtime. Full allows supported
inspection and memory writes. Read-only permits inspection and disables memory
editing with an explanation. Limited shows non-sensitive diagnostics and explains
why sensitive windows are disabled. None disables attachment and the windows
show that reason. A client setting cannot upgrade these permissions. Every
backend request independently enforces them.

## Memory and allocation tags

Choose the numeric Wasm module and memory, byte offset, range size, and number of
bytes per row. Rows and ranges accept explicit positive sizes; a 256-byte wire
reply is not a view-size limit. The window loads large ranges incrementally in
coherent stopped pages. **Refresh**, **Previous range**, **Next range**, and
**Load more rows** read actual memory. Offsets remain exact unsigned 64-bit
coordinates, not host addresses; their hexadecimal display uses 8 digits for a
32-bit memory and 16 for a 64-bit memory, independently of whether runtime MTE
is enabled.

**Memtag** is a per-window switch inside Memory. A new window defaults to colours
when runtime MTE is enabled, and stays off otherwise. It controls allocation-tag
colours, not runtime MTE execution. You can turn colours off for an individual
window without affecting other windows or the runtime. Enabling it authenticates a
fresh stopped snapshot. If runtime MTE is disabled, a short notice explains why
and the switch stays off; unsupported memories behave the same way. No setting
or guest state is changed to enable tagging. `UWVM: Show Memory Tags` opens a
new Memory window and requests this same switch.

When enabled, each byte uses its actual allocation granule's backend palette
colour. Hover shows the full hexadecimal tag and granule offset. The allocation
tag column aligns each tag with the displayed bytes and its 16-byte granule. Zero tags remain real zero tags;
unaligned pages retain every intersecting 16-byte granule. Each Memory window
keeps its own Memtag selection, independently of other tabs.

With full permission, click a byte to edit it inline: Enter commits two hex
digits and Escape cancels. Selecting several bytes opens the same explicit
selection editor; one commit contains up to 256 bytes and is atomic. A larger
selection remains inspectable and explains why that single commit is disabled.
Drag across bytes (or Shift-select) to inspect their interpretation. The view
offers ASCII, UTF-8/16/32, char/wchar, integers, floating point and LEB128 only
when the entire selection matches the type's width and encoding. Invalid or
incomplete character encodings are identified explicitly; mismatched numeric
widths are omitted. Control characters use names such as `(tab)`, while valid
printable characters retain their actual text. Little, big and PDP byte order are selectable; LEB byte order follows its
encoding. See [the complete type and byte-order catalogue](README.memory-values.md). Interpretation uses only the
already copied bytes and performs no additional runtime reads. A write must fit inside the displayed page. It carries the exact
displayed stop ID to the adapter and native commit, so a retired snapshot cannot
write into a newer stop. A successful commit is read back and its receipt names
the original module, memory, and committed stop. Later snapshot retirement does
not erase that completed receipt. An uncertain transport result is shown as an
error and is never automatically retried. Uncommitted input survives a same-stop
render; a changed stop or range discards it with an explicit no-write notice.

## Registers

Registers use standard DAP stack/scopes/variables requests. They are available
only for an actual owned native instruction stop. A cooperative Wasm/source stop
shows an explicit explanation; it does not invent native registers or infer
addresses from Wasm locals. Select the native stepping configuration and stop on
an instruction before refreshing this window.

## Wasm instruction highlighting and hover

Choose the **wasm** stepping configuration. Authenticated `uwvm-wasm:` documents
use the dedicated **UWVM Wasm Disassembly** language; ordinary `.wat` files keep
their existing editor language. Mnemonics, byte offsets, immediates and comments
are colored separately. The current instruction retains its arrow and line highlight.

Hover the exact instruction, operand or object for its compact description,
including Threads atomics and the experimental UWVM MTE namespace. The default
mouse delay is 350 ms; see [language support](README.wasm-language.md). Metadata describes the instruction; it does
not execute it, evaluate an expression, dereference an address or invent a
runtime value. Runtime feature switches and reserved encodings remain engine
decisions. For example, a reserved MTE operation does not become executable
because its spelling is documented.

Both full and read-only require a live stopped document, an authenticated
permission reply and a fresh exact source copy at the same stop before showing
semantics. A changed stop, publication, session, cancellation or content leaves
the hover explicitly unavailable. Limited and none do not acquire instruction
inspection authority. Execution and mutations clear the old current-PC marker.

## WASI and ROS groups

The WASIp1 window separates immutable runtime **Policy** rows from **Environment**
groups: arguments, environment variables, file descriptors, and preopens. The
policy query reports the actual interface exposure, grants, limits, quotas,
counters, environment identity and sharing groups admitted by the runtime.
These are views, including at full debug permission; deployment authority remains
separate. Choose another module explicitly and paginate policy or environment
groups to inspect their remaining records.

The ROS window separates authoritative cold **Policy** from current **Internal
state**. Policy pages show actual targets, exact group/module bindings,
interfaces, capabilities, limits and redacted connection metadata. Use the **View** selector to choose internal groups, including capability runtime state, sessions, queues, services, actions,
parameters, transforms, authorized graph metadata, statistics and recorded call
metadata. Select all, ROS1 or ROS2 explicitly. Empty tables remain empty; missing
backends or unsupported groups show the backend error. Viewing these windows
does not create a ROS connection or consume an event.

ROS also offers per-interface call statistics. **Recorded calls** in WASI,
and **Recorded calls** under one explicit ROS backend, show
the stopped journal. Read a retained payload by call ID, segment and byte offset,
or export the recorded family to a new JSON file on the VM host. Read-only can
read and export retained data. Start/stop recording and clearing the journal
require full and an explicit button press; opening a window never enables
recording. **Clear all families** removes the retained journal for every host family.
No recorded call or payload is synthesized when recording was disabled.

ROS policy scope, target and capability-alias filters limit the policy card;
the selected internal group remains an independent runtime view.

Each group is copied at one coherent stop. Pages are replaced rather than
combined across stop or runtime-epoch changes. Execution, invalidation, memory
changes and session termination retire copied snapshots and disable editing.
Refresh at the new stop to obtain current data. A local or verified SSH stdio
profile uses the same windows; paths used for backend operations remain on the
VM host.

Long policy values show their fragment byte offset and total length; one row is
never presented as the complete value. Exact fragment bytes remain available in
base64 metadata and hover text. A long WASI group name can span
`group.name.fragment.*` rows, including across transport pages.

## Breakpoints and execution state

The Breakpoints window shows the runtime inventory, including points created in
the console. Full permission exposes enable/disable, deletion, condition, skip
count, temporary Wasm instruction points and callback action forms. The callback
viewer pages through installed actions. Replace/append/clear operate on one
validated command packet at a time; they never invoke a shell. The runtime
independently admits each action. Read-only views cannot change any point.

Editor synchronization reuses runtime breakpoint IDs, preserving hit counters
and callbacks. A disabled source breakpoint remains in the editor gutter and
Breakpoints sidebar with VS Code's disabled icon and unchecked checkbox; deleting
it removes the point. Unverified points use the editor's separate unverified
state. At the current WAT instruction, VS Code's execution arrow can cover its
disabled glyph, so a grey **Breakpoint disabled** annotation also names that
retained state. Enabling or deleting the point removes this annotation. The
annotation reads the runtime inventory and never changes breakpoint permissions
or execution. Per-session WAT URIs keep their gutters separate. VS Code real-source breakpoints
are workspace-global: when multiple active UWVM sessions share a real source
URI, one VM's console event does not overwrite the other VM's editor switches.
Use each session's Breakpoints window for its authoritative independent state.

The VARIABLES tree contains locals, operand stack, globals that actually exist,
control/handler declarations and actual table inventory. Declaration parameter
and result types expand into their original indexed types. Recorded interface
calls and deployment policies have their own windows instead of occupying this
tree. A pre-trap operand snapshot carries a separate warning within its scope;
ordinary stops do not add that warning to the title. Frame details expose actual
metadata separately from compact call-stack labels.

## Independent authorized hot updates

**UWVM: Submit Authorized Hot Update Request** opens a separate window even
without a debug session or with debugger permission `none`. Multiple independent
update windows are supported. Only the fields used by the selected transport are
shown. Its transports are:

- **Local host**, using a local Linux update relay.
- **Imported profile / SSH**, preserving strict host-key
  verification and batch authentication. The frame path is on the VM host.
- **Active debug transport**, using that session's independent `updateSocketDir`.

Use **Browse…** for the local Python executable, transport profile, update
socket directory or frame file. The picker fills the field; **Submit once**
remains an explicit separate action. Cancelling retains the typed paths.
Host-call JSON export also offers the IDE's save-path dialog before the explicit
**Export JSON** action. Choosing a destination does not write a file.

The file picker browses the filesystem of the IDE extension host. An SSH VM's
frame or JSON destination must still be entered as an absolute path on that VM;
a local selection is never silently mapped to a remote host. The profile's
transport determines this boundary. A different filesystem provider URI is
rejected rather than treated as a local filename.

Enter an existing absolute frame path and choose **Submit once**.
Coordinator status, phase, epochs, decision sequence and UUR1 reply bytes appear
as structured fields with expandable details. Receiving a reply does not itself mean a commit.
The frontend does not hold signing keys, sign frames, construct a transaction,
choose COMMIT, resume execution or retry requests. Deployment contracts,
signatures, scope and the separate update capability remain backend checks.
Debugger display permission cannot grant deployment authority.

Optional settings `uwvm.update.pythonPath`, `uwvm.update.socketDir` and
`uwvm.update.transportProfile` prefill the form. No editor setting is changed.
None uses the independent host/profile route because it has no DAP endpoint.
The relay currently runs on a Linux VM host, which another editor host can reach
through a verified SSH profile.

## Other clients

Generating configuration alone is not an installed-editor GUI test or a promise
of dedicated custom panes.

| Client | Source / Wasm / owned asm | Memory and registers | MTE and WASI/ROS policy |
| --- | --- | --- | --- |
| VS Code / Cursor | Extension virtual Wasm source and standard DAP source/disassembly | Dedicated memory read/edit and native register tabs | Memtag switch inside Memory, typed values and rounding controls; separate WASI/ROS tabs |
| Neovim with genuine nvim-dap | Lua source/Wasm/native stepping and owned disassembly commands | Standard DAP variables/memory; client UI required | Existing text tag command and standard policy scopes; no Code webviews |
| JetBrains LSP4IJ / CLion DAP | Generated stdio attach configurations and standard DAP views, subject to client support | Standard register scopes/readMemory; client UI required | Bridge has read-only tag/trace panels and standard policy scopes; no new typed-memory webview |
| NetBeans bridge | Generated DAP bridge attach configurations; client required | Standard DAP scopes/requests; client UI required | Standard policy scopes; no dedicated custom windows |
| JDeveloper | Dedicated extension and acceptance required; no complete integration shipped | Unqualified | Unqualified |

All connected clients receive authoritative permission negotiation. Unsupported
sensitive requests fail at the backend, including from generic DAP clients.
Registers need a real native stop and source stepping needs usable guest DWARF.
The new power-of-two row layout, inline byte editing, typed values, byte-order
and rounding controls are currently implemented by the Code/Cursor webview.
The importer does not install that webview into other IDEs. See each
integration README for its commands and limitations.

In the Memory window, vertical wheel movement advances or reverses the displayed
address range in whole rows. Horizontal scrolling remains independent. Active
byte edits, input fields and text selections are not consumed by range scrolling.
The first range clamps at zero; requests remain bound to the current stopped
snapshot and permission.

## Dedicated Wasm instruction editor

Authenticated `.uwvm-wat` stack-frame resources open in **UWVM Wasm
Instructions**, a custom editor based on the disassembly-table interaction model.
Ordinary `.wat` source files are unaffected. The fixed left rail contains separate
breakpoint, next-instruction pointer and hexadecimal byte-offset cells. A disabled
breakpoint stays visibly grey even at the current instruction; absence has no
marker. The instruction pane scrolls horizontally without moving that rail.

Block, loop, if/else, try-table and end delimiters determine indentation and faint
vertical guides. The complete original opcode stream is decoded independently of
optimized executable stop points: an opcode without a surviving stop point is
shown but cannot receive an executable breakpoint. Addresses retain exact 8-digit
or 16-digit formatting, and mnemonic/operand columns use the function's measured
maximum text widths. Long functions use virtual rows and rolling scroll segments,
not a limit on the function or available instructions. **Offset → Go** accepts an
exact decimal or hexadecimal byte offset.

A new execution stop scrolls horizontally only if the instruction start is
outside the visible instruction pane. An instruction beyond the left edge moves
to 25% of that pane; one beyond the right edge moves to 75%, clamped to the
available range. A visible instruction and manual scrolling are preserved through
metadata and breakpoint refreshes. The address rail always remains fixed. Each
indentation level spans four editor characters, with a faint guide at its start.

Font family, size, weight, spacing and line height follow VS Code's editor
settings. Instruction families retain the original editor colours: control flow
is purple, ordinary numeric instructions such as i32.const are pale yellow, and
numeric operands are green in the default dark theme. The execution marker uses
VS Code's stack-frame glyph and current-frame theme colour. Hover adds semantic
details without changing the pointer or the marker's shape.

Execution uses the standard IDE toolbar and F5/F10/F11/Shift+F11 actions.
The toolbar's clickable S/W/A indicator selects Source, Wasm or Assembly and
reflects the adapter's acknowledged level for the active session. The custom
view has no second execution toolbar. Full permission is required for execution
or breakpoint changes; read-only can inspect. Limited and none do not reveal
code. Closing the Wasm view suppresses automatic reopening on the next stop;
an explicit Open Wasm Instructions action restores it.

Click the breakpoint gutter to add/remove; Shift-click toggles enabled state;
right-click provides conditions and hit conditions. The supported SourceBreakpoint
API and existing complete-inventory bridge synchronize the native BREAKPOINTS
sidebar and console, with runtime confirmation distinguished from pending requests.
Branch-depth operands are yellow, distinct from purple control-flow opcodes.
Ctrl-click (Cmd-click on macOS) navigates within the copied function document:
loop labels lead to the loop header; block, if and function exits lead to their
matching end boundary. The execution pointer is unchanged. Ordinary clicks
select text/rows; they do not invoke execution controls.

Hover supplies instruction meaning, actual named functions and current typed
locals/globals, declared memory identity, constant bit representations or resolved
branch destinations. Live values are queried against the exact stop and generation;
historical views do not retain live read or mutation authority. This UI does not
extend native ASM authority beyond the already authenticated LLVM-JIT code owner.
