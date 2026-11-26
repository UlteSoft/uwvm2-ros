# Wasm module policies

[中文](wasm-module-policy.zh.md)

Use these host options before `--run`. The loader resolves each key from its built-in default, then the global, group and single-module settings. Resolution happens before parsing the main module or any preload, in either CLI order. A module keeps its resolved parameters for validation, initialization and recompilation.

| Command | Arguments |
| --- | --- |
| `--wasm-policy-global` | `KEY VALUE` |
| `--wasm-policy-group` | `GROUP KEY VALUE` |
| `--wasm-policy-group-add-module` | `GROUP MODULE` |
| `--wasm-policy-single` | `MODULE KEY VALUE` |
| `--wasm-policy-memory-backend` | `MODULE LOCAL-DEFINITION-INDEX BACKEND` |

Use the final registered module name. An explicit main/preload rename takes precedence over the Wasm name section. The supplied file name provides the fallback. Each group needs a setting and at least one loaded member. Each module can belong to one group and have single-module settings. Unknown modules, unused groups, repeated memberships and duplicate keys in a scope produce errors.

## Parser keys

`parser.baseline` accepts `mvp`, `wasm1p1`, `wasm2` or `wasm3`. The product default is Core 3.0 with threads, WasmADM and WasmMTE enabled. Custom page sizes default to off. A baseline supplies defaults for Core features and threads. An explicit feature value takes precedence over a baseline.

| Key | Accepted values |
| --- | --- |
| `parser.FEATURE` | `on`, `off` |
| `parser.limit.TYPE` | Nonnegative decimal count representable by host `size_t` |

Feature names are `adm`, `memtag`, `custom-page-sizes`, `multi-value`, `reference-types`, `table-instructions`, `multiple-tables`, `bulk-memory`, `sign-extension`, `nontrapping-float-to-int`, `simd`, `extended-const`, `table-initializer`, `relaxed-simd`, `multi-memory`, `threads`, `tail-call`, `memory64`, `table64`, `function-references`, `gc` and `exceptions`. An MTE memory must declare ADM with at least four ignored bits. The MTE declaration, its instructions and its global metadata require both extensions.

| Parser limit type | Counted records |
| --- | --- |
| `codesec_codes`, `code_locals` | Code bodies and locals |
| `const_expr_opcodes` | Total instructions in constant expressions across globals, table initializers, element entries and active segment offsets (default 1,048,576). The final `end` consumes no entry. |
| `custom_sections` | Custom section records per module (default 65,536), including repeated unknown names. |
| `section_storage_bytes` | Cumulative native record bytes for section vectors, nested element entries, local declaration runs and constant instructions (default 256 MiB). Each entry costs its host record size. Import/export records also include their descriptor pointer. String and data views borrow the input bytes. |
| `typesec_types`, `tagsec_count` | Types and exception tags |
| `importsec_imports`, `exportsec_exports` | Imports and exports |
| `globalsec_globals`, `memorysec_memories`, `tablesec_tables` | Local global, memory and table declarations |
| `datasec_entries`, `datacountsec_count` | Data entries and declared data count |
| `elemsec_elems`, `elemsec_funcidx`, `elemsec_expr` | Element entries, function indices and initializer expressions |
| `custom_name_funcnames`, `custom_name_codelocal_funcs`, `custom_name_codelocal_name_per_funcs` | Function names, functions with local names and local names per function in the optional name section |
| `custom_name_local_names`, `custom_name_diagnostics` | Total local-name rows across functions (default 1,048,576) and detailed name warnings per module (default 256). A further warning emits one truncation notice. |

## Initializer and runtime keys

| Key | Accepted values and effect |
| --- | --- |
| `init.imported-FIELD` | Nonnegative decimal count. Fields are `functions`, `tables`, `memories`, `globals`, `tags`. |
| `init.local-defined-FIELD` | Nonnegative decimal count. Fields are `functions`, `codes`, `tables`, `memories`, `globals`, `tags`, `elements`, `datas`. |
| `init.element-expressions` | Total expression-form entries in all element segments of one module. The process default is 1,048,576. Index-form entries borrow parser storage. |
| `init.allocation-bytes` | Initial backing bytes per locally defined memory or table, at most the process `initializer allocation-bytes` ceiling. Applies to the defining module and to restored extents. |
| `runtime.memory-pages` | `MIN` or `MIN:MAX` in the defined memory’s Wasm pages. Each count is a nonnegative decimal `size_t`, with `MAX >= MIN`. |
| `runtime.memtag-checks` | `on` (default) or `off`. The accessing module controls ordinary pointer/allocation-tag comparisons, including access through imports. |
| `runtime.memtag-backend` | `auto` (default), `software`, `arm-mte`, `x86-mtt`. Supplies the backend for tagged memories defined by this module. |
| `runtime.interpreter.loop-unwind`, `runtime.interpreter.delay-local`, `runtime.interpreter.instruction-reorder` | `on` or `off` for the corresponding compiled interpreter optimization. |
| `runtime.interpreter.loop-unwind-max-size` | Nonnegative decimal maximum loop body size in Wasm bytes. |
| `runtime.interpreter.opcode-combine` | `disable`, `soft`, `heavy`, `extra`, up to the level compiled into the interpreter. |
| `runtime.llvm.policy` | `default`, `debug`, `fast-compile`, `balanced`, `max` for LLVM full-module translation. |
| `runtime.llvm.full-policy` | `auto`, `debug`, `legacy-light`, `pb-o1`, `pb-o2`, `pb-o3`. A choice other than `auto` selects the concrete pipeline before the high-level LLVM preset. |

The initializer enforces the smaller of the module count and the process ceiling. A build accepts the translation keys for its compiled backends and passes. The delay-local key requires opcode combining and a compiled `soft` or `heavy` delay-local level. Its switch enables or disables that level. Translation preserves the eligibility rules for each optimization. For example, an interpreter setting cannot authorize a fusion that assumes ordinary addresses on an ADM memory.

```sh
uwvm2-ros \
  --wasm-policy-global parser.custom-page-sizes off \
  --wasm-policy-group sensors parser.custom-page-sizes on \
  --wasm-policy-group sensors init.local-defined-memories 2 \
  --wasm-policy-group-add-module sensors camera \
  --wasm-policy-single camera runtime.memtag-checks off \
  --wasm-policy-memory-backend camera 0 software \
  --wasm-preload-library camera.wasm camera \
  --run app.wasm
```

Here `camera` must define an MTE memory at local definition index zero. The index excludes imports. The per-memory choice overrides the owner's module default. All aliases share the owner's backend, tag field and allocation state. The calling module still masks addresses, checks bounds and executes explicit tag and zeroing operations when ordinary comparisons are off. Another module can check tags on the same memory.

WasmMTE needs at least four ADM bits. New software owners use the highest four guest pointer bits. A full snapshot records the field. A restored owner keeps any saved four-bit field inside its ADM bits, including when the receiving VM selects Arm hardware. For memory64 with 16 ADM bits, the saved start can be 48 through 60.

For a new Arm MTE owner with 4–7 ADM bits, the tag occupies [31:28] in memory32 or [63:60] in memory64. With 8 or more ADM bits it occupies [27:24] or [59:56], respectively. All these fields map to [59:56] in the native AArch64 pointer. The default `auto` prefers an eligible physical owner. It selects software for an unsupported host, an unchecked caller, a moving allocation or continuing diagnostics. Explicit Arm requires checked callers, crash delivery and a usable checked mapping. The reserved Intel MTT layout uses the guest’s highest four bits for ADM 4–6 and [28:25] or [60:57] for ADM 7 and above. The VM rejects explicit Intel MTT requests until a checked backend is available. The SDK’s instrumented release ABI remains `wasm64_adm16`. [Owner layout and delivery](../runtime/memtag-extension.md)

### Logical pages of a defined memory

A page pair replaces the declared runtime limits just like `--wasm-set-memory-limit`. The minimum selects the initial page count. Omitting the maximum removes the explicit logical maximum. Widening the declaration uses the existing runtime warning policy. Allocation, `memory.grow`, the Wasm address domain, ADM, import compatibility and the mmap reservation still enforce their own bounds. For example, mmap plans against the larger declared/configured minimum and the smaller available maximum. The logical setting grants no additional reserved bytes.

```sh
uwvm2-ros \
  --wasm-policy-global runtime.memory-pages 1:4 \
  --wasm-policy-group sensors runtime.memory-pages 2:8 \
  --wasm-policy-group-add-module sensors camera \
  --wasm-policy-group-add-module sensors lidar \
  --wasm-policy-single camera runtime.memory-pages 3:6 \
  --wasm-set-memory-limit camera 1 4 5 \
  --wasm-preload-library camera.wasm camera \
  --wasm-preload-library lidar.wasm lidar \
  --run app.wasm
```

Here each local memory in `lidar` gets 2–8 pages. In `camera`, local definition 1 gets 4–5 pages and the other definitions get 3–6. Local memories in `app` get 1–4. An import or re-export follows its defining owner through every alias. A scoped default on a module that only imports memories therefore has no effect on them.

Each layer selects a whole pair. The legacy `--wasm-set-memory-limit MODULE all MIN [MAX]` occupies the single-module layer and conflicts with a single-module `runtime.memory-pages` setting on that module. A numeric selector overrides every module layer and starts at zero after excluding imports. Repeating an explicit selector for the same module is an error in either order.

The global **scope** applies settings across Wasm modules. Individual Wasm globals obtain their type, mutability and initial value from their declaration and initializer expression. For example, `(global (mut i32) (i32.const 7))` starts as a mutable 32-bit value of 7. `parser.limit.globalsec_globals` and `init.local-defined-globals` bound declaration counts. A module policy provides no per-global value override.

### Pointers shared between modules

A tagged load or store uses the original pointer in two ways. WasmADM masks its ignored high bits to obtain the dynamic offset. WasmMTE extracts the logical tag from the owner's four-bit field in the original pointer and compares it with the allocation tag of each 16-byte granule touched. With synchronous checking and the default crash action, a pointer tagged zero accessing a granule tagged `0xA` faults before it reads or writes the data.

A module built without tag instrumentation can forward a tagged pointer to another module that shares the owner. The interface must carry the complete `i32` or `i64` value, including the tag. Setting `runtime.memtag-checks off` for the forwarding module preserves this pointer ABI. The receiving module applies its own access policy when it dereferences the pointer.

For an ordinary 4 GiB memory32, `0xA0001234` is byte offset `0xA0001234`. For a tagged memory32 with four ignored bits, the same pointer carries tag `0xA` and dynamic offset `0x1234`. Its dynamic address domain is 256 MiB. Passing offsets between these two layouts requires an interface that accounts for their different meanings.

## Process and host controls

| Control | Reason for its scope |
| --- | --- |
| `--runtime-memory-reservation pool` / `participants` | All local owners share one virtual-address budget in the process. The same command accepts `memory MODULE INDEX BYTES` for one local owner's data quota. |
| `--wasm-process-initializer-limit TYPE COUNT` | Sets the initializer's hard ceiling across modules. `runtime_modules` bounds the whole linked graph. The other process ceiling names use underscores, for example `local_defined_memories`. |
| `--wasm-set-resource-limit`, `--wasm-depend-recursion-limit`, `--wasm-memory-grow-strict` | The allocator accounts host bytes, the dependency checker walks the linked graph, and the host chooses the process response to recoverable allocation/commit failure during growth. |
| `--runtime-memtag-mode`, `--runtime-memtag-action` | The execution threads, signal delivery and dedicated report stream serve the process. |
| Backend selection, compile threads, scheduling, LLVM exception ABI and the persistent cache directory | The process owns its execution backend, compiler resources, native exception transport and cache. |
| `--wasm-set-preload-module-attribute` | For example, `--wasm-set-preload-module-attribute driver copy 0` permits the native preload bound as `driver` to use the copy interface for caller memory index 0. This grant is independent of the defining Wasm module’s logical pages and group membership. |
| WASI, ROS and WASI Threads permissions | Each host interface has its own module bindings and permissions. A Wasm policy group confers no host permissions. |

[Address masking and reservations](../runtime/adm-memory-reservation.md) · [Host permissions](host-permissions.md)
