# Wasm disassembly language support

The VS Code/Cursor extension assigns **UWVM Wasm Disassembly** to authenticated
`uwvm-wasm:` virtual documents. It does not replace the editor's ordinary WAT
language, read host files, or evaluate guest expressions.

The generated grammar and hover catalogue follow the VM's instruction display
table: Wasm 3.0 scalar/control/reference/GC/SIMD operations, the 67 Threads atomic
opcodes, and the UWVM linear MTE v1 namespace. Memory64 and multi-memory use the
same instruction families; the displayed memory index and full-width offset are
preserved. Atomic, scalar and SIMD memory operands include their decoded
alignment, index and lane where applicable. Immediates not yet pretty-printed
remain explicitly labelled binary bytes, rather than fabricated values.

## Open the view

Use the [one-command IDE importer](README.ide-import.md) to open the generated
VS Code or Cursor workspace, then select its **Wasm** debug configuration.
When a full or read-only session stops, the current frame opens an authenticated
Wasm document automatically. Move the mouse over a mnemonic or its immediate
operands for the instruction description. The arrow and highlight identify the
actual next instruction anywhere in the function; the document contains the
complete function and retains the same URI while its code generation is unchanged. The same language provider works with the
importer's verified SSH transport.

This view does not grant permission or enable a disabled Wasm feature. The
runtime still validates the module against the selected feature set. Limited
sessions expose the permitted numeric crash location instead of the instruction
document; none has no debugger connection.

## Display and hover

Hover applies to the token under the pointer, not to the whole instruction row:

| Token | Compact content |
| --- | --- |
| Instruction | Coloured mnemonic and concise meaning where available; MTE adds its stack inputs/result |
| Operand | Meaning of that offset, alignment, lane or constant |
| Object | The actual function name when present; current local/global type and value; memory width and import metadata; table index or branch depth |
| Byte-offset column | Subdued hexadecimal coordinate: 8 digits for Wasm32, 16 for a module with memory64; distinct from operand syntax colours |

Blank space and undecoded byte tails do not open an instruction tooltip.
Instruction families, object indices, operand names and literal values have
separate syntax scopes, including inside the hover code block. Colours follow
the active editor theme.

Encoding dumps, stop IDs, repeated source coordinates, tag geometry and generic
safety descriptions are omitted from ordinary instruction hovers. The current
PC arrow/highlight remains; it does not add a second whole-line tooltip.
The reserved `memtag.add` descriptor is marked non-executable.

Mouse hover uses the editor's native delay/cancellation mechanism, with a
**350 ms** default for this language only. It adds no second timer or execution
pause. Explicit keyboard hover retains the editor's normal behavior. Users can
override the delay using a language-specific setting:

```json
{
  "[uwvm-wasm-disassembly]": {
    "editor.hover.delay": 500
  }
}
```

See the editor's [language configuration defaults](https://code.visualstudio.com/api/references/contribution-points#contributes.configurationDefaults).
No global user settings or theme colours are overwritten.

Runtime bytes, registers and allocation tags remain in their dedicated
inspection windows. Local/global hover uses the original typed slot at the
current authenticated stop. It never executes an operand expression or follows
a displayed address. Function names come from the actual name section, DWARF,
or import/export metadata; absent names remain absent.

## Permission and lifetime

Full and read-only sessions may inspect live instruction documents. Limited
sessions retain their numeric crash backtrace, but cannot read disassembly.
None creates no debug endpoint.

Every hover and current-instruction highlight checks the authenticated permission,
current stop, live session, exact code generation and the digest of the complete
document content. The generation-stable document reference is not a value
capability: each dynamic hover additionally binds the exact stop ID. Resume, mutation, publication, stop changes and session
termination retire pending queries and current-instruction decorations. A stale
or denied hover gives an explicit unavailable/permission message. A previously
copied IDE document is historical text, not a capability to read or modify a
later execution state.

No hover executes an instruction, invokes `evaluate`, reads an arbitrary native
address, or modifies guest memory. Source queries use the existing bounded,
compiler-owned instruction boundaries. The formatter is invoked only for debug
display; no new guest-execution instrumentation is added.

## Maintenance and other clients

Run `python3 tools/debug/generate_wasm_language.py --check` to detect catalogue
drift, or omit `--check` to regenerate the checked-in JSON assets from the native
formatter and MTE descriptors. Unknown opcodes do not acquire guessed semantics.

Other DAP clients receive the same Wasm listing and generation-stable source
references. Runtime frame, value, edit and native-code references remain stop-bound. Rich highlighting/hover is an editor feature: the bundled TextMate
grammar and VS Code API provider directly support VS Code/Cursor. A generic DAP
connection alone does not install a language provider in another IDE.
