# Address masking and linear memory reservations

A memory with address width `W` and `N` ignored high bits has a dynamic address domain of `D = 2^(W-N)` bytes. Ordinary memory uses `N = 0`. The VM resolves imports and re-exports to their defining memory before charging the process reservation pool. All aliases use the same mapping, page size and tag layout.

WasmMTE needs four ADM bits at either guest width. New software owners use bit `W−4` as the tag start. Arm MTE uses `W−4` for ADM 4–7 and `W−8` for ADM 8 and above. The Intel MTT layout reserves `W−4` for ADM 4–6 and `W−7` for ADM 7 and above. Intel MTT execution requires a future checked backend. The address mask always uses the declared ADM count. For example, memory32 ADM4 has a 256 MiB domain and ADM8 has a 16 MiB domain. Tagged memory64 retains its existing native mapping limits. A full snapshot preserves the guest tag field, including when restored on a different physical backend. [Module and memory backend policy](../command-line/wasm-module-policy.md)

The initializer accounts for three different byte counts. The committed logical length changes with `memory.grow`. The data quota limits how far the memory can grow. The reservation includes the data capacity and inaccessible guard space. Only committed data is made accessible. The pool is shared across memory owners, including live replacement generations.

An LLVM debugger with full permission can prepare a snapshot replacement while the paused module graph keeps its mappings. With automatic participant sizing, the initial graph can use half the free pool. At each restore the planner counts the free bytes and the paused graph's actual mapping sizes. The replacement can use up to half that total, rounded down to a byte, within the remaining free space. After the old workers physically join, their join receipts retain their identity. Publication and debugger rebinding release the old graph's remaining references in an ordinary CLI session, returning its mappings to the pool for the next restore. A host that holds a separate source or code owner keeps the corresponding mapping charged. Each generation counts each local definition once, including its guard and any manual data quota. With the default i686 pool and a single session, each graph has 1 GiB. Initialization fails when its required data and minimum guards exceed that budget. An explicit `--runtime-memory-reservation participants` count selects the participant-based plan. The private WASI Threads driver and sessions with preloaded native drivers use the ordinary plan.

| Decision | Inputs | Consequence |
| --- | --- | --- |
| Automatic data quota | Process budget, declared minima and maxima, existing owners, fixed quotas | The common power-of-two share must fund every owner's minimum and guard. |
| Manual data quota | Local definition, requested bytes, declared limits and process budget | A request below the minimum fails. A request above a declared maximum is clipped with a warning. |
| Full address window | Dynamic address domain and actual reserved span | A load or store may use page protection when its complete effective range fits the window. |
| Partial address window | Effective range exceeds the usable window | Generated code must prove bounds or check before accessing memory. |

Ordinary memory32 on a 64-bit host keeps its fixed window. With a 4-bit mask on memory32, the dynamic domain is 256 MiB. A 32-bit and a 64-bit memory can have identical dynamic domains. For example, memory64 with 36 ignored bits also has a 256 MiB domain. Memory64 with 16 ignored bits has a 256 TiB domain, larger than the default 16 TiB process budget on a 64-bit host.

For a scalar access, the VM masks the dynamic address, adds the instruction's static offset at widened precision, and accounts for the entire access width. A full window can cover offsets below `D` with roughly `2D` bytes of virtual address space plus a trailing guard. An instruction with a larger offset needs its own proof. Masking the sum would change Wasm semantics. Bulk lengths retain their integer meaning. Zero-length operations still have their specified boundary behavior.

For every ADM data quota, the planner prefers a full window when the process pool can fund it at the common share. A manual quota fixes the data capacity in bytes after any clipping at the declared maximum. Growth still uses complete Wasm pages. A partial window needs the accepted quota rounded up to a system page plus the trailing guard. If the pool or OS cannot provide the full window, the VM uses the partial window at the same quota. Initialization fails if that window cannot fit.

For a single owner with a 256 MiB dynamic domain, 64 KiB Wasm pages and 4 KiB system pages, the trailing guard is 69,632 bytes. A pool of 536,940,544 bytes funds a full window for either a 16 MiB or a 256 MiB data quota. Pools of 384 MiB and 512 MiB select partial windows of 16,846,848 bytes and 268,505,088 bytes for those two quotas respectively.

A custom page can be one byte. After growing such a memory to 4097 bytes on a host with 4096-byte pages, byte 4096 is valid and byte 4097 is outside the logical memory. Both bytes occupy the same native page. The runtime must enforce the logical boundary even with a full address window, unless the instruction is independently proven in bounds. Tag checking is an additional per-access requirement.

Verbose initialization reports the owner and local index, ABI width and ignored bits, full or partial protection, automatic or manual allocation, alias count, Wasm and system page bytes, data and guard bytes, and total reserved bytes. Performance comparisons must hold the useful byte length, workload and access count constant and measure execution separately from initialization and compilation.

[WebAssembly custom page sizes](https://github.com/WebAssembly/custom-page-sizes/blob/main/proposals/custom-page-sizes/Overview.md)
