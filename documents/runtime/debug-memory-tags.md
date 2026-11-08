# WasmMTE allocation-tag inspection

The debugger reads allocation tags only from a completely stopped, host-authorized Wasm instance. Guest module and memory indices select an owned linear memory; they are never native pointers. Imported aliases resolve to the same backing owner. Independent memories retain independent tag stores.

Use `memtags MODULE MEMORY OFFSET COUNT` in the debugger console. Arguments are decimal byte selectors. A console page contains at most 256 bytes, including unaligned ranges; every intersecting 16-byte granule is reported with its allocation tag in hexadecimal and its stable RGB color. Continue with another valid offset to inspect any total memory range. The page size does not limit the addressable or retained memory.

`disabled` means that this backing has no enabled tag store. It does not mean that every allocation tag is zero. `unsupported-memory` means that the backing provider does not expose this observation. An unavailable or out-of-range owned memory is rejected. Tag reads use the actual current native memory length, not reserved capacity or shadow pages prepared for a failed growth operation.

The terminal memory pane colors copied bytes with their allocation tags. The numeric tag remains authoritative; color is an additional visual cue shared with the runtime fatal diagnostics. Selecting a new stop clears the old memory and tag preview.

In VS Code or Cursor, run **UWVM: Show Memory Tags** while an authorized UWVM session is stopped. Enter `MODULE MEMORY OFFSET COUNT`. The command obtains bytes and tags from one authenticated stop and opens a colored snapshot labeled with its stop ID. Run the command again to refresh or inspect another page. This preview neither attaches to another process nor grants a new memory capability.

The DAP extension request `uwvm/memoryTags` accepts `memoryReference` in the existing `wasm-memory:MODULE:MEMORY:OFFSET` form, an optional signed byte `offset`, `count` (0 through 256 bytes), and optional boolean `includeBytes`. Its response contains status, stop ID, memory size, tag width, 16-byte granule size, numeric/hex/color tag rows, and a next-page memory reference when one remains. With `includeBytes`, base64 bytes come from the same stop. Native addresses, running/native-only stops, stale stop generations, changed participant cohorts, malformed rows, and mismatched palettes are rejected.

The read path is debugger-only. It adds no observer, lock, tag load, or generated instruction to ordinary execution. Copying a growable shadow retains the native owner's existing memory-operation pin. Shared tags use atomic reads. The host API does not return a shadow pointer or expose a guest-callable import.

## Validation boundary

The component tests cover private/shared stores, wasm32/wasm64 tag widths, actual-length bounds, overflow, unaligned granules, disabled state, owner moves, independent memories, and palette rendering. Protocol tests cover coherent stop ownership and malformed data. These checks alone do not certify that a particular compiler artifact enables its memory owners or that a target uses hardware MTE; those require runtime execution tests on the actual selected backend.
