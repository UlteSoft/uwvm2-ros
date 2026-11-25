# Wasm 模块策略

[English](wasm-module-policy.md)

在 `--run` 前指定宿主选项。主模块和预加载模块在解析声明前按“内置默认值 → 全局 → 组 → 单模块”逐项覆盖。参数在命令行中的顺序不影响结果。验证、初始化和重新编译都使用该模块已解析的配置。

| 命令 | 参数 |
| --- | --- |
| `--wasm-policy-global` | `键 值` |
| `--wasm-policy-group` | `组 键 值` |
| `--wasm-policy-group-add-module` | `组 模块` |
| `--wasm-policy-single` | `模块 键 值` |
| `--wasm-policy-memory-backend` | `模块 本地定义索引 后端` |

模块名使用最终注册名，优先取启动时显式指定的主模块或预加载别名，再取 Wasm 名称节，最后使用传入的文件名。每组须有设置且至少有一个已加载成员。每个模块最多加入一个组，也可有单模块设置。未知模块、空组、重复绑定和同一层的重复键会报错。

## 解析器

`parser.baseline` 支持 `mvp`、`wasm1p1`、`wasm2`、`wasm3`。产品默认启用 Core 3.0、threads、WasmADM 和 WasmMTE，自定义页大小默认关闭。基线提供 Core 特性和线程的默认值，显式特性设置优先于基线。

`parser.特性 on|off` 的特性名包括 `adm`、`memtag`、`custom-page-sizes`、`multi-value`、`reference-types`、`table-instructions`、`multiple-tables`、`bulk-memory`、`sign-extension`、`nontrapping-float-to-int`、`simd`、`extended-const`、`table-initializer`、`relaxed-simd`、`multi-memory`、`threads`、`tail-call`、`memory64`、`table64`、`function-references`、`gc`、`exceptions`。MTE 内存必须声明 ADM，且至少忽略四位。其声明、指令和全局元数据均要求同时开启 ADM 与 MTE。

`parser.limit.类型` 接受宿主 `size_t` 可表示的非负十进制计数。

| 类型 | 计数对象 |
| --- | --- |
| `codesec_codes`、`code_locals` | 函数体、局部变量 |
| `typesec_types`、`tagsec_count` | 类型、异常标签 |
| `importsec_imports`、`exportsec_exports` | 导入、导出 |
| `globalsec_globals`、`memorysec_memories`、`tablesec_tables` | 本地全局变量、线性内存、表 |
| `datasec_entries`、`datacountsec_count` | 数据段条目、声明的数据段数 |
| `elemsec_elems`、`elemsec_funcidx`、`elemsec_expr` | 元素段条目、函数索引、初始化表达式 |
| `custom_name_funcnames`、`custom_name_codelocal_funcs`、`custom_name_codelocal_name_per_funcs` | 可选名称节中的函数名、带局部名的函数数量、每个函数的局部名数量 |

## 初始化与运行时

| 键 | 值及行为 |
| --- | --- |
| `init.imported-字段` | 非负十进制计数，字段为 `functions`、`tables`、`memories`、`globals`、`tags`。 |
| `init.local-defined-字段` | 非负十进制计数，字段为 `functions`、`codes`、`tables`、`memories`、`globals`、`tags`、`elements`、`datas`。 |
| `runtime.memtag-checks` | `on`（默认）或 `off`。由发起访问的模块决定是否比较指针和分配标签，导入内存也遵循调用方策略。 |
| `runtime.memtag-backend` | `software`（默认）、`auto`、`arm-mte`、`x86-mtt`，用于该模块定义的带标签内存。 |
| `runtime.interpreter.loop-unwind`、`runtime.interpreter.delay-local`、`runtime.interpreter.instruction-reorder` | 用 `on` 或 `off` 控制已编入解释器的对应优化。 |
| `runtime.interpreter.loop-unwind-max-size` | 允许展开的循环体最大 Wasm 字节数，非负十进制整数。 |
| `runtime.interpreter.opcode-combine` | `disable`、`soft`、`heavy`、`extra`，上限取决于编入的级别。 |
| `runtime.llvm.policy` | LLVM 整模块翻译预设，可选 `default`、`debug`、`fast-compile`、`balanced`、`max`。 |
| `runtime.llvm.full-policy` | `auto`、`debug`、`legacy-light`、`pb-o1`、`pb-o2`、`pb-o3`。显式指定优化流水线时优先于 LLVM 预设。 |

初始化模块时取模块计数和进程硬上限中的较小值。只有实际编入的翻译后端和优化才接受对应配置。优化还需满足该模块的地址和类型条件。

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

上例要求 `camera` 在本地定义索引零处声明 MTE 内存。索引排除所有导入内存。单内存后端优先于所有者模块默认值，所有导入别名共用该所有者的后端、标签位域与分配状态。`camera` 跳过普通访问的标签比较，仍执行地址掩码、边界检查、显式标签操作和清零。其他模块访问同一内存时可以开启比较。

新建软件内存默认使用指针最高四位。从完整快照重建时，软件后端会保留原有的四位标签字段，可位于 ADM 忽略区内的任何连续四位。例如，memory64 忽略 16 位时，最低标签位可为第 48 至 60 位。兼容的 Linux/AArch64 映射可由 `auto` 选用真实硬件。要求 Arm 硬件的内存须具有相容的位域，使用崩溃处理，并要求所有可访问者进行比较。当前 VM 会拒绝要求 Intel MTT 的内存。VM 格式支持两种指针宽度和不同 ADM 位数。SDK 本次发布的带 MTE guest ABI 为 `wasm64_adm16`。[所有者布局与故障交付](../runtime/memtag-extension.md)

### 跨模块传递指针

一次带标签的读写会从原始指针取得两个值。WasmADM 用掩码清除忽略的高位，得到动态地址偏移。WasmMTE 从原始指针中所有者指定的四位提取逻辑标签，与访问覆盖的每个 16 字节粒度的分配标签比较。在同步检查和默认崩溃处置下，标签为零的指针访问分配标签为 `0xA` 的粒度，会在读写数据之前触发故障。

未插桩的模块也可以把带标签的指针传给共享同一内存的另一个模块。接口须传递包含标签的完整 `i32` 或 `i64` 值。给转发模块设置 `runtime.memtag-checks off` 会保留该指针 ABI。接收模块解引用时仍使用自己的访问策略。

在普通 4 GiB memory32 中，`0xA0001234` 表示字节偏移 `0xA0001234`。在忽略高四位的带标签 memory32 中，同一个值的标签是 `0xA`，动态偏移是 `0x1234`，动态地址域为 256 MiB。在这两种布局之间传递偏移时，接口需要明确约定它的含义。

## 进程和宿主接口

| 命令或配置 | 作用范围 |
| --- | --- |
| `--runtime-memory-reservation pool`、`participants` | 整个进程共享一个虚拟地址预留池。同一命令的 `memory 模块 索引 字节数` 子命令设置单个本地所有者的数据配额。 |
| `--wasm-process-initializer-limit TYPE COUNT` | 对进程中所有模块设置初始化硬上限。`runtime_modules` 约束整个链接图，其他类型用下划线拼写，例如 `local_defined_memories`。 |
| `--wasm-set-resource-limit`、`--wasm-depend-recursion-limit`、`--wasm-memory-grow-strict` | 分别控制宿主分配记账、整个链接图的遍历深度，以及内存增长遇到可恢复宿主分配失败时的进程处理方式。 |
| `--runtime-memtag-mode`、`--runtime-memtag-action` | 执行线程、信号交付和专用报告输出由进程管理。 |
| 执行后端、编译线程、调度、LLVM 异常 ABI 和持久缓存目录 | 进程统一选择执行引擎，管理编译资源、异常传播和对象缓存。 |
| WASI、ROS 和 WASI Threads 权限 | 每个宿主接口独立绑定模块和权限，Wasm 策略组不授予宿主权限。 |

[地址掩码与预留](../runtime/adm-memory-reservation.zh.md) · [宿主权限](host-permissions.md)
