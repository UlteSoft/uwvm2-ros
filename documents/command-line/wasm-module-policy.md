# Wasm module policies

A launch can apply different parser features, initializer limits and runtime tag checks to the main Wasm module and its preloads. Each setting starts with the existing command-line default, then applies global, group and single-module overrides in that order. Argument order does not change that precedence. Each key can be set once per scope. Each module can belong to one Wasm policy group and can also have single-module overrides.

| Command | Arguments |
| --- | --- |
| `--wasm-policy-global` | `KEY VALUE` |
| `--wasm-policy-group` | `GROUP KEY VALUE` |
| `--wasm-policy-group-add-module` | `GROUP MODULE` |
| `--wasm-policy-single` | `MODULE KEY VALUE` |
| `--wasm-policy-memory-backend` | `MODULE LOCAL-DEFINITION-INDEX BACKEND` |

A group is created by its first setting or membership. The loader uses an explicit main/preload rename first, followed by the Wasm name section and then the supplied file name. The loader resolves its policy before decoding declarations and retains the result for recompilation. The Wasm group namespace is independent of WASI, ROS and thread permissions.

| Key | Values and effect |
| --- | --- |
| `parser.FEATURE` | `on` or `off`. Use the feature names below. |
| `init.imported-memories`, `init.local-defined-memories` | Nonnegative decimal counts. Limit the module's imported or defined memory records during initialization. |
| `init.imported-globals`, `init.local-defined-globals` | Nonnegative decimal counts. Limit the module's imported or defined global records during initialization. |
| `runtime.memtag-checks` | `on` or `off`. Controls pointer/allocation-tag comparisons for accesses made by this module, including through imported memory. |
| `runtime.memtag-backend` | `software`, `auto`, `arm-mte` or `x86-mtt`. Supplies the default for tagged memories defined by this module. |

The initializer uses the smaller of its existing process limit and the module's limit. Use `--runtime-memory-reservation pool <bytes>` or `participants <count>` to configure the shared reservation pool. The same command accepts `memory <module> <local-index> <data-bytes>` to set one owner's data quota. [Address masking and reservation](../runtime/adm-memory-reservation.md)

Parser feature names are `adm`, `memtag`, `custom-page-sizes`, `multi-value`, `reference-types`, `table-instructions`, `multiple-tables`, `bulk-memory`, `sign-extension`, `nontrapping-float-to-int`, `simd`, `extended-const`, `table-initializer`, `relaxed-simd`, `multi-memory`, `threads`, `tail-call`, `memory64`, `table64`, `function-references`, `gc` and `exceptions`. WasmADM and WasmMTE are enabled by default. Custom page sizes require explicit enablement.

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

This example assumes `camera` defines a WasmMTE memory at local definition index zero. Imported memories are excluded from that index. The explicit owner setting overrides the module's backend default. All aliases share the selected backend, tag layout, shadow and allocation directory. When `camera` accesses a memory, it masks the address and checks bounds. A different module can still check the tags on the same memory. Explicit tag queries, stores, allocation operations and zeroing keep their ordinary effects in both modules.

`software` selects four tag bits at the top of the Wasm pointer. `auto` selects usable Arm MTE for a compatible checked owner and software otherwise. Required `arm-mte` needs a checked Linux/AArch64 mapping, a compatible ignored-bit field, and a crash delivery policy. A reachable unchecked caller or a software diagnostic mode requires software. The current VM rejects `x86-mtt` because it has no qualified x86 checked mapping implementation. [Owner layout and delivery](../runtime/memtag-extension.md)

## 中文说明

模块的最终配置按“已有启动默认值 → Wasm 全局 → Wasm 组 → 单模块”逐项覆盖。同一层的同一键重复设置会报错。一个模块最多属于一个 Wasm 策略组，也可以另设单模块选项。WASI、ROS 和线程权限各有自己的分组。主模块和预加载模块都在解析声明前按实际模块名选择配置，显式重命名优先，其后依次是 Wasm 名称节和加载路径。

`parser.FEATURE on|off` 控制该模块接受的特性。`init.*` 限制导入或本地定义的内存与全局变量数量，并同时受进程级初始化上限约束。memory64 地址空间预留使用独立的 `--runtime-memory-reservation` 命令，可配置进程共享池大小、参与者数量或单个内存所有者的数据配额。[地址掩码与内存预留](../runtime/adm-memory-reservation.zh.md)

`runtime.memtag-checks off` 只关闭发起访问的模块的标签比较。地址掩码、越界检查、标签读写、分配登记及清零继续生效。上例中，`camera` 可以使用自定义页大小，最多定义两块内存，并对自己的普通访存关闭标签比较。其他模块通过导入访问同一块内存时采用各自的检查策略。

`--wasm-policy-memory-backend MODULE INDEX BACKEND` 的索引从零开始，只计入该模块本地定义的线性内存。初始化时由内存所有者确定标签布局，所有导入别名共享这一布局。`software` 使用指针最高四位。`auto` 可在不满足真实硬件条件时选择软件。显式 `arm-mte` 或 `x86-mtt` 要求可用的对应硬件实现。
