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
| `typesec_types`, `tagsec_count` | Types and exception tags |
| `importsec_imports`, `exportsec_exports` | Imports and exports |
| `globalsec_globals`, `memorysec_memories`, `tablesec_tables` | Local global, memory and table declarations |
| `datasec_entries`, `datacountsec_count` | Data entries and declared data count |
| `elemsec_elems`, `elemsec_funcidx`, `elemsec_expr` | Element entries, function indices and initializer expressions |
| `custom_name_funcnames`, `custom_name_codelocal_funcs`, `custom_name_codelocal_name_per_funcs` | Function names, functions with local names and local names per function in the optional name section |

## Initializer and runtime keys

| Key | Accepted values and effect |
| --- | --- |
| `init.imported-FIELD` | Nonnegative decimal count. Fields are `functions`, `tables`, `memories`, `globals`, `tags`. |
| `init.local-defined-FIELD` | Nonnegative decimal count. Fields are `functions`, `codes`, `tables`, `memories`, `globals`, `tags`, `elements`, `datas`. |
| `runtime.memtag-checks` | `on` (default) or `off`. The accessing module controls ordinary pointer/allocation-tag comparisons, including access through imports. |
| `runtime.memtag-backend` | `software` (default), `auto`, `arm-mte`, `x86-mtt`. Supplies the backend for tagged memories defined by this module. |
| `runtime.interpreter.loop-unwind`, `runtime.interpreter.delay-local`, `runtime.interpreter.instruction-reorder` | `on` or `off` for the corresponding compiled interpreter optimization. |
| `runtime.interpreter.loop-unwind-max-size` | Nonnegative decimal maximum loop body size in Wasm bytes. |
| `runtime.interpreter.opcode-combine` | `disable`, `soft`, `heavy`, `extra`, up to the level compiled into the interpreter. |
| `runtime.llvm.policy` | `default`, `debug`, `fast-compile`, `balanced`, `max` for LLVM full-module translation. |
| `runtime.llvm.full-policy` | `auto`, `debug`, `legacy-light`, `pb-o1`, `pb-o2`, `pb-o3`. A choice other than `auto` selects the concrete pipeline before the high-level LLVM preset. |

The initializer enforces the smaller of the module count and the process ceiling. A build accepts the translation keys for its compiled backends and passes. Translation preserves the eligibility rules for each optimization. For example, an interpreter setting cannot authorize a fusion that assumes ordinary addresses on an ADM memory.

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

A new software memory uses the highest four pointer bits by default. `auto` can select a compatible Linux/AArch64 checked mapping. A required Arm owner needs hardware support, compatible geometry, crash delivery and checked callers. The current VM rejects a required Intel MTT owner. The VM format admits ADM with either pointer width and variable ignored-bit counts. The SDK's instrumented release target is `wasm64_adm16`. [Owner layout and delivery](../runtime/memtag-extension.md)

## Process and host controls

| Control | Reason for its scope |
| --- | --- |
| `--runtime-memory-reservation pool` / `participants` | All local owners share one virtual-address budget in the process. The same command accepts `memory MODULE INDEX BYTES` for one local owner's data quota. |
| `--wasm-process-initializer-limit TYPE COUNT` | Sets the initializer's hard ceiling across modules. `runtime_modules` bounds the whole linked graph. The other process ceiling names use underscores, for example `local_defined_memories`. |
| `--wasm-set-resource-limit`, `--wasm-depend-recursion-limit`, `--wasm-memory-grow-strict` | The allocator accounts host bytes, the dependency checker walks the linked graph, and the host chooses the process response to recoverable allocation/commit failure during growth. |
| `--runtime-memtag-mode`, `--runtime-memtag-action` | The execution threads, signal delivery and dedicated report stream serve the process. |
| Backend selection, compile threads, scheduling, LLVM exception ABI and the persistent cache directory | The process owns its execution backend, compiler resources, native exception transport and cache. |
| WASI, ROS and WASI Threads permissions | Each host interface has its own module bindings and permissions. A Wasm policy group confers no host permissions. |

[Address masking and reservations](../runtime/adm-memory-reservation.md) · [Host permissions](host-permissions.md)
