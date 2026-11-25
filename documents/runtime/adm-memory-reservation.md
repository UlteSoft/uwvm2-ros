# Address masking and linear memory reservations

A memory with address width `W` and `N` ignored high bits has a dynamic address domain of `D = 2^(W-N)` bytes. Ordinary memory uses `N = 0`. The VM resolves imports and re-exports to their defining memory before charging the process reservation pool. All aliases use the same mapping, page size and tag layout.

The initializer accounts for three different byte counts. The committed logical length changes with `memory.grow`. The data quota limits how far the memory can grow. The reservation includes the data capacity and inaccessible guard space. Only committed data is made accessible. The pool is shared across memory owners, including live replacement generations.

| Decision | Inputs | Consequence |
| --- | --- | --- |
| Automatic data quota | Process budget, declared minima and maxima, existing owners, fixed quotas | The common power-of-two share must fund every owner's minimum and guard. |
| Manual data quota | Local definition, requested bytes, declared limits and process budget | A request below the minimum fails. A request above a declared maximum is clipped with a warning. |
| Full address window | Dynamic address domain and actual reserved span | A load or store may use page protection when its complete effective range fits the window. |
| Partial address window | Effective range exceeds the usable window | Generated code must prove bounds or check before accessing memory. |

Ordinary memory32 on a 64-bit host keeps its fixed window. With a 4-bit mask on memory32, the dynamic domain is 256 MiB. A 32-bit and a 64-bit memory can have identical dynamic domains. For example, memory64 with 36 ignored bits also has a 256 MiB domain. Memory64 with 16 ignored bits has a 256 TiB domain, larger than the default 16 TiB process budget on a 64-bit host.

For a scalar access, the VM masks the dynamic address, adds the instruction's static offset at widened precision, and accounts for the entire access width. A full window can cover offsets below `D` with roughly `2D` bytes of virtual address space plus a trailing guard. An instruction with a larger offset needs its own proof. Masking the sum would change Wasm semantics. Bulk lengths retain their integer meaning. Zero-length operations still have their specified boundary behavior.

When a manual quota covers `D`, the planner can select either window. It first budgets the accepted data quota rounded up to a system page, plus the trailing guard. It upgrades to full protection when the pool can fund the full span alongside the other owners. For a single owner with `D` and quota both 256 MiB, 64 KiB Wasm pages and 4 KiB system pages, the trailing guard is 69,632 bytes. Pools of 384 MiB and 512 MiB both select a 268,505,088-byte partial window. The full window needs 536,940,544 bytes. If the OS declines the full mapping, the VM retries the partial window with the same data quota. Initialization fails if that window cannot fit.

A custom page can be one byte. After growing such a memory to 4097 bytes on a host with 4096-byte pages, byte 4096 is valid and byte 4097 is outside the logical memory. Both bytes occupy the same native page. The runtime must enforce the logical boundary even with a full address window, unless the instruction is independently proven in bounds. Tag checking is an additional per-access requirement.

Verbose initialization reports the owner and local index, ABI width and ignored bits, full or partial protection, automatic or manual allocation, alias count, Wasm and system page bytes, data and guard bytes, and total reserved bytes. Performance comparisons must hold the useful byte length, workload and access count constant and measure execution separately from initialization and compilation.

[WebAssembly custom page sizes](https://github.com/WebAssembly/custom-page-sizes/blob/main/proposals/custom-page-sizes/Overview.md)
