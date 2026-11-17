# UWVM inspection windows

The debugger UI follows VS Code's own debugging conventions: inspectors are
editor tabs styled with the active colour theme and the editor font, every
control has a visible entry point, and the standard debug toolbar (F5, F10,
F11, Shift+F11) drives execution at every level.

## Where everything is

- **Run and Debug → UWVM view.** Before a session it explains how to start
  one. When the first UWVM session starts, the view is revealed and shows the
  session (name, permission, paused/running) and the inspectors grouped as
  *Step level*, *Code & breakpoints* (Wasm instructions, Breakpoints),
  *Guest state* (Memory, Registers), *Host interfaces* (WASI, WASI Threads,
  u2rosv1, u2rosv2, preloaddl) and *Session* (Diagnostics, Save Wasm dump,
  Hot update).
  One click opens a window. An entry that the current permission does not
  allow shows the reason inline (for example `limited`) instead of failing.
- **Status bar.** `W · Wasm` (or `S · Source`, `A · Assembly`) shows the step
  level used by F10/F11. Click it, or press Ctrl+Alt+L, to change it.
- **Debug toolbar.** The S/W/A button changes the level; the memory button
  opens Memory.
- **Command palette.** Every window is also available as `UWVM: Open …`.

Memory opens in the active editor group. Other inspectors reuse an inspector
group when available, or open beside the code.
**⋯ → Open another … window** creates an independent copy (its own range,
filters and session binding) for side-by-side comparison. Each window shows
its session, stop, and permission in the title bar; Ctrl+R (or the refresh
action) reads the current stop again. F5 stays VS Code's Continue.

The permission badge comes from the authenticated runtime. Full allows
inspection and edits. Read-only inspects and explains why edits are disabled.
Limited shows numeric diagnostics only. None disables attachment. A client
setting cannot upgrade a permission, and the backend checks every request.
When the guest moves on, copied values stay visible with a banner saying they
belong to an earlier stop and offering **Read current stop**.

## Back and Forward

The debugger toolbar and every inspector provide **Back** and **Forward**.
Alt+Left / Alt+Right navigate the active debug session's history (on macOS,
the editor shortcuts are Ctrl+- / Ctrl+Shift+-). Text inputs retain their
normal cursor shortcuts. Each session owns a bounded 256-location trail;
visiting a new destination after going back discards its forward branch.

Source, Wasm, owned JIT assembly, Memory, Registers, WASIp1, WASI Threads,
u2rosv1, u2rosv2, preload libraries, breakpoints, grouped Watch, Diagnostics
and Hot Update participate. A location contains view coordinates and selectors,
not a copy of guest state. Returning to an inspector refreshes its current
state under the current permission. It never resumes execution, changes step
level, restores a checkpoint, repeats an edit, or submits a hot update.
Rebinding a window removes its old session's entries. An expired JIT code
reference is refused and removed from the trail, without reading other native
code; further Back/Forward navigation can continue.

Ctrl-click (Cmd-click on macOS) a Wasm branch depth to browse its structural
target; Back restores the original instruction and horizontal/vertical view.
A function operand can open a body already captured in the same runtime epoch.
The current VM exposes only the stopped function's instruction body: an
uncaptured target or host import produces an explicit explanation and never
executes the call to obtain its body. Historical code has no live value or
execution authority. Closing a Wasm/inspector tab does not discard its location;
Back can reopen it.

WASIp1 descriptor rows show the guest mount, the path sampled from the live
host handle, and base/inheriting rights separately. These paths are display
observations, not filesystem authority. ROS builds keep launch environments
immutable to the debugger, including in full mode. To revoke a preopen in the
guided program, execute its authorized `fd_close`; check that the preopen and
any child descriptors are absent before removing the dedicated host directory.

## Memory and allocation tags

The Memory window is a hex editor over the **whole** linear memory: address
column, hex bytes (a gap every 8), text column and, when tags are on, the
allocation tag of each row. Its scrollbar spans the entire memory; the mouse
wheel, trackpad and scrollbar scroll smoothly by pixels, and the bytes on
screen are read in 256-byte pages as they come into view (placeholders `··`
show for the moment a page is loading). Type an address in **Go to** (hex
`0x1f0` or decimal) and press Enter. **Per row** selects 1–64 bytes.
Addresses use 8 hex digits for a 32-bit memory and 16 for a 64-bit memory.

Stepping does not rebuild or flash the window. Values from the previous stop
stay on screen and are replaced in place when the new stop is read; bytes
that changed briefly highlight. A run longer than a moment dims the bytes and
says they belong to an earlier stop. At an assembly stop the runtime does not
read guest memory, so the window keeps the last Wasm-stop values with a note.

- **Select:** click a byte, drag across bytes (the selection only follows the
  mouse while the button is held), or Shift+arrows. Ctrl+A selects the rows on
  screen; Escape clears the selection.
- **Interpret:** the *Selection* panel (below the bytes in a narrow editor)
  shows every type whose width matches the selection: u/i8–128, hex, binary,
  ASCII/UTF-8/16/32, char types, FP8 through Float128, Decimal BID/DPD, LEB128
  and BCD. Characters read `'a' 'b' 'c' nul tab`: printable characters in grey
  single quotes, control characters by their short name. Byte order, decimal
  rounding and significant digits are panel settings. See
  [the complete type catalogue](README.memory-values.md).
- **Edit in place (full permission):** double-click a byte, press Enter/F2,
  or type a hex digit. Enter or Tab commits and moves to the next byte; Escape
  cancels. The byte shows the new value in italics until the VM confirms the
  write and the value is read back. For several bytes use right-click → **Edit
  selection…** (one atomic write of up to 256 bytes). When editing is not
  allowed, the status line says why. An uncertain transport result is reported
  and never retried automatically.
- **Copy:** Ctrl+C copies the selection as hex; the context menu also copies
  it as a C array or copies the address.
- **Allocation tags:** the *Allocation tags* switch colours every byte with its
  16-byte granule's tag; a legend lists the tags on screen. The switch only
  changes this window's display. If runtime MTE is off it stays off.

## Registers

Registers use standard DAP stack/scopes/variables requests. They are available
only for an actual owned native instruction stop. A cooperative Wasm/source stop
shows an explicit explanation; it does not invent native registers or infer
addresses from Wasm locals. Switch the step level to **A · Assembly** (status
bar or debug toolbar), step once, then refresh this window. Rows can be
filtered and each value copied.

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

## WASI and u2ros interface windows

Both windows use a section list on the left and a data table on the right,
like the Keyboard Shortcuts editor. Every table has a filter box (Ctrl+F),
keyboard navigation (arrows, Home/End, Page Up/Down), a twistie to expand a
row's complete record, and **Copy as JSON** per row. Columns that are empty in
every row are hidden; the expanded record still lists every field.
Structured values are trees with typed colours, never raw JSON text.

**WASI** sections: Arguments, Environment (name and value columns),
Descriptors, Preopens, **Policy** and **Recorded calls**. Policy rows are
grouped by area (`interface`, `group`, …), colour `allow`/`hidden`/`deny`, and
keep the exact value bytes in the expanded record. Choose the module in the
title bar.

**u2rosv1** sections: Policy (with scope, target and alias filters), runtime
state (Capabilities, Sessions, Queues, Services, Actions, Parameters,
Transforms, Graph) and activity (Interface statistics, Call statistics, Call
metadata, Recorded calls). The hierarchy is **u2rosv1 → ros1 / ros2**;
the backend switch selects All backends, ros1 or ros2. These backend versions
are independent of the u2ros interface ABI version.
Policy combines authenticated wire pages into 64-row display pages. Previous
and Next traverse the complete filtered inventory, with a visible row range
and total. Expand a setting to inspect its scope and original fields.
Durations are shown in ns/µs/ms with the exact nanosecond count on hover.

**Recorded calls** show one row per call: status dot, function and
namespace, arguments, result, duration and payload segments. Expand a row for
module, participant, generation and arguments; **Read payload bytes** shows
the retained bytes (hex and UTF-8). The recording bar shows whether this
family is being recorded; **Start/Stop recording** and **Clear** need full
permission and an explicit click; opening a window never enables recording.
The inactive indicator is blue; active recording is red. Payload readers
select the call, segment and byte offset, including multi-segment calls.
ROS results use their interface-specific conventions: a nonzero return value
alone is not an error (for example, ABI version 1 is a successful result).
**Export JSON** writes the recorded family to a new file on the VM host;
**Browse…** fills the path only for local VM hosts (an SSH host path must be
typed).

Each page is copied at one coherent stop and replaced, not merged, after the
stop changes. Long policy values show their fragment offset and total length.
Viewing these windows never creates a ROS connection or consumes an event.
Memory and variable edits retire old snapshot handles and refresh the selected
frame before the next edit. An open inspector stays visible; variable scopes
keep their expansion state instead of reopening the first scope after edits.

**u2rosv2 · Robot scene** is a separate inspector for the interface under
development. It displays copied scene authority and resource metadata; it is
not another name for the ros2 backend.

The sidebar and Diagnostics overview use authenticated build metadata.
Compiled-out interfaces are marked **not built** and their runtime state is
not queried. A backend that was built still needs its own runtime grants;
opening an inspector grants nothing. Build availability is not an assertion
that a session is connected or that an operation is authorized.

**preloaddl** displays the actual loader build status and a paged inventory of
loaded native libraries. Each entry contains the module name, file identity,
exported Wasm function count and whether signature verification was requested
and completed at load time. File paths are folded initially. This immutable
launch inventory remains readable after execution ends. Full or read-only
debugger permission is required. It exposes no native addresses, symbols or
instructions and cannot enable loading or native debugging. The xmake native
loader option remains disabled by default. Pages respect both the 32-entry
limit and the control channel's byte limit, so long names and paths reduce the
page size without hiding later libraries.

## Diagnostics, saved environments and replay

Diagnostics has four sections: **Overview**, **Save & record**, **History**
and **Restore & replay**. State, permission and the current stop stay visible
above each section. A joined thread cohort is described as exited with replay
available, rather than as an invented instruction at stop zero.

Overview separates participants, runtime details and interface build status.
Save & record separates dump disclosure and output from recording actions.
History separates the saved file and sample selector from the selected
environment. WASIp1 modules and **u2rosv1 → ros1 / ros2** environments have
separate selectors. Policy groups, resources, interface grants and archive
metadata are folded independently; all recorded fields remain available.
Restore & replay groups recovery inputs separately from external-effect
boundaries. Each section retains its scroll position and unfinished inputs
while switching tabs, and those drafts are cleared when binding another session.

Narrow inspectors use one card column; wider inspectors use two where the
content permits. Forms have explicit labels, long values wrap, tables scroll
within their cards and the selected task keeps its primary action together.
The launch dump policy and debugger permissions continue to determine which
actions are enabled.

## Breakpoints and execution state

Breakpoints exist at three levels — **Source** (a file line), **Wasm** (an
instruction) and **Assembly** (an owned JIT instruction) — and every enabled
breakpoint stops the guest whatever the S/W/A step level is. The step level
only decides what Step Over/Into/Out (F10/F11/Shift+F11) step through.

The Breakpoints window groups the runtime inventory by level (Source, Wasm,
Assembly; all expanded by default; click a group header to fold it). Each row
has an enable checkbox, location (`file:line`, `module · func · @offset` or
`func · JIT +offset`), condition, hit count and skip count; disabled rows are
dimmed. Breakpoints created in the console (`bs`, `bw`, `bi`) appear in the
right group and, for source and Wasm lines, in the editor gutter. Full
permission can add a Wasm instruction breakpoint, toggle, delete, and expand a
row to edit its condition, skip count and callback actions (paged;
replace/append/clear, one validated packet at a time, never a shell).
Read-only can inspect but not change points.

## Execution position at every level

A stop has authenticated positions in source, Wasm and owned LLVM-JIT code.
The native VS Code source editor owns its arrows, inline column marker, line
highlight and breakpoint-dot composition. Yellow identifies the current top
frame; green identifies a selected caller, following VS Code itself. The
extension never adds a second source gutter icon or line background, changes
a real stack location, or invents a caller to obtain a different color.

Source statement exactness is independent of those native frame colors. Hover
over the source position to distinguish a statement start from execution
inside the statement. Switching Source/Wasm/ASM without executing cannot
change this authenticated exactness. Source-backed Wasm frames retain the
real source location for the native editor and carry their independent Wasm
location in `uwvmWasmLocation` for the Wasm view. Missing debug information
does not produce a guessed source marker.

An [explicit optional workbench repair](README.vscode-workbench.md) lets the
native source editor use green for an authenticated in-statement UWVM stop,
including after switching back from Wasm. It also fixes the native WATCH
type label remaining visible during editing. This is a version-checked
workbench change, not an automatically installed extension feature; the
stock-editor behavior described above remains until it is explicitly applied.

In the custom Wasm view, **Exact** is yellow at an instruction start and
**Next** is green inside an instruction. Native Disassembly marks the current
owned LLVM-JIT instruction; the extension never paints a projected green
ASM marker. Both code views continue to follow source/Wasm/native stops.

At a native instruction stop, WASI/ROS/Threads and memory inspectors remain
open. The VM currently requires a cooperative Wasm/source stop for a new
host-state snapshot. Matching previously read data remains explicitly
historical and read-only; an unobserved selection shows an explanation.
Switching modules, backends or pages never relabels a previous snapshot.
A local refusal preserves valid register/disassembly references, and the
next cooperative stop refreshes the view. Browsing does not execute the
guest or automatically advance to a supposedly safe location.

Drag the **exact** arrow in the Wasm view gutter to another instruction to
move execution there (a click without dragging toggles the breakpoint as
usual). A next arrow can never be dragged. The runtime decides whether a move
is possible: today it relocates only the native PC at an assembly stop on
macOS arm64, within one JIT function and without crossing calls or branches;
elsewhere the drag is refused with the runtime's reason and execution stays
where it was.

Editor synchronization reuses runtime breakpoint IDs, preserving hit counters
and callbacks. A disabled source breakpoint stays in the editor gutter and the
BREAKPOINTS view with VS Code's disabled icon. Per-session WAT URIs keep their
gutters separate. Real-source breakpoints are workspace-global in VS Code:
when several UWVM sessions share a source file, one VM's console change does
not overwrite another VM's editor switches; use each session's Breakpoints
window for its own state.

The VARIABLES tree contains locals, operand stack, globals that actually exist,
control/handler declarations and actual table inventory. Declaration parameter
and result types expand into their original indexed types. Recorded interface
calls and deployment policies have their own windows instead of occupying this
tree. A pre-trap operand snapshot carries a separate warning within its scope;
ordinary stops do not add that warning to the title. Frame details expose actual
metadata separately from compact call-stack labels.

## Independent authorized hot updates

**Hot update…** (UWVM view → Session, or `UWVM: Submit Hot Update…`) works
without a debug session and with debugger permission `none`. Pick a
transport card — **Local host** (Linux relay), **Imported profile / SSH**
(strict host-key checking, batch authentication) or **Active debug session**
(its own `updateSocketDir`; disabled with an explanation when absent) — and
only that transport's paths are shown. **Browse…** fills a path; it never
submits. **Submit once** sends an existing signed frame exactly once and shows
the coordinator reply as an expandable tree; a reply is not a commit.

The window never holds signing keys, signs frames, builds a transaction,
chooses COMMIT, resumes execution or retries. Deployment contracts,
signatures, scope and the separate update capability remain backend checks.
The file picker browses the IDE host; an SSH VM's frame path must be typed as
an absolute path on that VM. Settings `uwvm.update.pythonPath`,
`uwvm.update.socketDir` and `uwvm.update.transportProfile` prefill the form.

## Other clients

Generating configuration alone is not an installed-editor GUI test or a promise
of dedicated custom panes.

| Client | Source / Wasm / owned asm | Memory and registers | MTE and WASI/ROS policy |
| --- | --- | --- | --- |
| VS Code / Cursor | Extension virtual Wasm source and standard DAP source/disassembly | Hex-editor memory window with in-place editing; native register tab | Allocation-tag switch inside Memory, typed values and rounding controls; WASI/ROS windows with filterable tables |
| Neovim with genuine nvim-dap | Lua source/Wasm/native stepping and owned disassembly commands | Standard DAP variables/memory; client UI required | Existing text tag command and standard policy scopes; no Code webviews |
| JetBrains LSP4IJ / CLion DAP | Generated stdio attach configurations and standard DAP views, subject to client support | Standard register scopes/readMemory; client UI required | Bridge has read-only tag/trace panels and standard policy scopes; no new typed-memory webview |
| NetBeans bridge | Generated DAP bridge attach configurations; client required | Standard DAP scopes/requests; client UI required | Standard policy scopes; no dedicated custom windows |
| JDeveloper | Dedicated extension and acceptance required; no complete integration shipped | Unqualified | Unqualified |

All connected clients receive authoritative permission negotiation. Unsupported
sensitive requests fail at the backend, including from generic DAP clients.
Registers need a real native stop and source stepping needs usable guest DWARF.
The memory hex editor, typed values, byte-order and rounding controls are
implemented by the Code/Cursor webviews. The importer does not install them
into other IDEs. See each integration README for its commands and limitations.

## Dedicated Wasm instruction editor

Authenticated `.uwvm-wat` stack frames open in **UWVM Wasm Instructions**
(UWVM view → Wasm instructions, or call stack context menu). Ordinary `.wat`
files are unaffected. It uses the editor font, size, weight, line height and
colour theme, so switching between source, Wasm and the native Disassembly
view does not change the look.

The fixed left rail has one glyph cell and the address. As in the editor
gutter, the breakpoint dot and the current-instruction arrow share that cell:
at the current instruction a disabled or enabled breakpoint is drawn as a
small dot inside the arrow, so the address column never shifts. Addresses are
grey hex with 8 digits (Wasm32) or 16 digits (Wasm64). The listing starts at
the rail edge; indent guides sit in fixed 4-character columns and run through
every row of their block, so `block`, `loop` and nested instructions line up:

```
block
|   loop
|   |   local.get 0
```

Mnemonics keep their syntax colours (control purple, numeric pale yellow,
variable access blue, GC/reference teal); immediates are green and branch
depths yellow. Hover shows instruction meaning, function names, current typed
local/global values and branch destinations from the exact current stop.

- **Breakpoints:** click the glyph cell to add or remove; Shift-click
  enables/disables; right-click offers condition and hit condition. F9 toggles
  the selected row. Rows without a stop point in the optimized generation are
  shown but cannot take a breakpoint.
- **Navigate:** arrows move the selection, Home/End jump to the ends, **Offset
  → Go** jumps to a byte offset, Ctrl/Cmd-click a branch depth to see its
  destination (execution is not changed). Ctrl+C copies the selected rows.
- **Follow:** on each stop the view scrolls vertically to the current
  instruction; it scrolls horizontally only when the instruction start is
  outside the visible area. A brief retire/refresh between two stops keeps
  the previous highlight instead of flashing.
- **Execute:** use the standard debug toolbar and F5/F10/F11/Shift+F11. There
  is no second toolbar inside the view.
- **Close:** Ctrl+W or the tab's ×. A closed view stays closed on later stops;
  reopen it from the UWVM view. Reopening reveals the existing tab rather than
  creating a duplicate.

Full permission is required for execution or breakpoint changes; read-only
can inspect. Limited and none do not reveal code. Large functions use virtual
rows; there is no limit on function size.
