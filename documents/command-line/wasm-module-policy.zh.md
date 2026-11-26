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
| `input-bytes` | 单模块源文件字节数，同时受进程单文件硬上限约束（默认 256 MiB）。 |
| `codesec_codes`、`code_locals` | 函数体、局部变量 |
| `const_expr_opcodes` | 模块内全局变量、表初值、元素条目和主动段偏移量的常量表达式指令总数（默认 1,048,576）。结尾的 `end` 不计数。 |
| `custom_sections` | 每模块自定义节记录总数（默认 65,536），包括名称重复的未知自定义节。 |
| `section_storage_bytes` | 节向量、嵌套元素条目、局部变量声明组及常量指令的宿主记录累计字节数（默认 256 MiB）。每条按宿主记录大小计费，导入导出记录另计描述符指针大小。字符串和数据视图借用输入字节。 |
| `type-metadata-bytes`、`function-body-bytes`、`module-code-bytes` | 默认继承进程 parser 资源限额。模块配置可进一步收紧生效值。类型元数据包括递归定义、类型验证记录和签名投影。代码限额分别计每个函数体与模块全部函数体的编码字节数。 |
| `typesec_types`、`tagsec_count` | 类型、异常标签 |
| `importsec_imports`、`exportsec_exports` | 导入、导出 |
| `globalsec_globals`、`memorysec_memories`、`tablesec_tables` | 本地全局变量、线性内存、表 |
| `datasec_entries`、`datacountsec_count` | 数据段条目、声明的数据段数 |
| `elemsec_elems`、`elemsec_funcidx`、`elemsec_expr` | 元素段条目、函数索引、初始化表达式 |
| `custom_name_funcnames`、`custom_name_codelocal_funcs`、`custom_name_codelocal_name_per_funcs` | 可选名称节中的函数名、带局部名的函数数量、每个函数的局部名数量 |
| `custom_name_local_names`、`custom_name_diagnostics` | 全部函数的局部名称总条数（默认 1,048,576）和每模块详细名称警告总数（默认 256）。超出警告上限时另记一条截断提示。 |

宿主 `--wasm-set-resource-limit parser single-bytes`（默认 256 MiB）限制每次元数据分配，`live-bytes`（默认 1 GiB）限制全进程同时存活的解析元数据，包括扩容时同时存活的新旧缓冲区。模块释放后归还其独占分配。全局递归类型注册表仅为新增规范类型保留键和编号，直至注册表销毁。递归类型按组批量发布，额度不足时保留原注册表和调用方映射。启用 C++ 异常时，私有函数替换和检查点编译遇解析元数据或解析引起的验证暂存额度不足时返回资源耗尽，原运行代继续持有其存储。

显式指定模块名时，加载器在读入或映射前检查该名称对应的额度。未显式命名时，加载器先按已打开文件的长度预留进程输入额度，再通过同一句柄读取节边界和模块名，并在交给解析器的字节上复核。进程的 `parser input-live-bytes` 默认为 1 GiB，包含同时存活的输入映射及不可变副本，也包含替换或恢复期间保留的源版本。签名更新记录模块与进程单文件上限共同决定的有效输入字节数。内容相同的更新沿用已安装的输入。目标内容变更时预留完整目标的额度，重置后的数据段可继续引用同一份目标。

## 初始化与运行时

| 键 | 值及行为 |
| --- | --- |
| `init.imported-字段` | 非负十进制计数，字段为 `functions`、`tables`、`memories`、`globals`、`tags`。 |
| `init.local-defined-字段` | 非负十进制计数，字段为 `functions`、`codes`、`tables`、`memories`、`globals`、`tags`、`elements`、`datas`。 |
| `init.element-expressions` | 一个模块所有元素段中的表达式项总数。进程默认上限为 1,048,576；索引形式使用解析器已持有的存储。 |
| `init.allocation-bytes` | 每个本地定义的内存或表允许申请的初始字节数，受进程 `initializer allocation-bytes` 上限约束。定义模块的策略也适用于恢复时重建的长度。 |
| `runtime.memory-pages` | `MIN` 或 `MIN:MAX`，单位为该内存定义的 Wasm 页。页数为宿主 `size_t` 可表示的非负十进制整数，且 `MAX >= MIN`。 |
| `runtime.memtag-checks` | `on`（默认）或 `off`。由发起访问的模块决定是否比较指针和分配标签，导入内存也遵循调用方策略。 |
| `runtime.memtag-backend` | `auto`（默认）、`software`、`arm-mte`、`x86-mtt`，用于该模块定义的带标签内存。 |
| `runtime.interpreter.loop-unwind`、`runtime.interpreter.delay-local`、`runtime.interpreter.instruction-reorder` | 用 `on` 或 `off` 控制已编入解释器的对应优化。 |
| `runtime.interpreter.loop-unwind-max-size` | 允许展开的循环体最大 Wasm 字节数，非负十进制整数。 |
| `runtime.interpreter.opcode-combine` | `disable`、`soft`、`heavy`、`extra`，上限取决于编入的级别。 |
| `runtime.llvm.policy` | LLVM 整模块翻译预设，可选 `default`、`debug`、`fast-compile`、`balanced`、`max`。 |
| `runtime.llvm.full-policy` | `auto`、`debug`、`legacy-light`、`pb-o1`、`pb-o2`、`pb-o3`。显式指定优化流水线时优先于 LLVM 预设。 |

初始化模块时取模块计数和进程硬上限中的较小值。只有实际编入的翻译后端和优化才接受对应配置。`delay-local` 键要求编入指令合并以及 `soft` 或 `heavy` 延迟局部变量优化，其开关控制已编入的级别。优化还需满足该模块的地址和类型条件。

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

WasmMTE 至少需要四位 ADM。新建软件内存的标签占指针最高四位。完整快照记录客体标签位置，恢复时，软件和 Arm 硬件均保留 ADM 忽略区内原有的连续四位。例如，memory64 忽略 16 位时，保存的标签起点可以是第 48 至 60 位。

新建 Arm MTE 内存按 ADM 位数选择标签。ADM 为 4 至 7 时，memory32 使用 [31:28]，memory64 使用 [63:60]。ADM 达到 8 时，两者分别使用 [27:24] 和 [59:56]。这些字段均映入 AArch64 宿主指针 [59:56]。默认的 `auto` 优先选择可用硬件。宿主不支持、有调用者关闭检查、分配可移动或配置为记录故障后继续时，自动选择软件。显式 Arm 要求映射能接受硬件检查、使用崩溃处理且所有调用者开启比较。Intel MTT 预留布局在 ADM 为 4 至 6 时使用客体最高四位，ADM 达到 7 时，memory32 使用 [28:25]，memory64 使用 [60:57]。当前 VM 拒绝显式 Intel MTT 请求。SDK 带 MTE 的发布 ABI 仍为 `wasm64_adm16`。[所有者布局与故障交付](../runtime/memtag-extension.md)

### 本地内存定义的逻辑页数

页数对沿用 `--wasm-set-memory-limit` 的算法，替换内存声明的运行时上下限。最小值决定初始页数，省略最大值表示取消显式逻辑上限。放宽声明会触发现有运行时警告策略。分配、`memory.grow`、Wasm 地址域、ADM、导入兼容性和 mmap 预留各自施加约束。例如，mmap 规划取声明与配置中较大的最小值及较小的可用最大值。逻辑页数设置不会增加预留字节配额。

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

上例中，`lidar` 的每个本地内存定义使用 2–8 页。`camera` 的本地定义 1 使用 4–5 页，其余本地定义使用 3–6 页。`app` 的本地定义使用 1–4 页。导入和再次导出的内存始终使用定义者的页数。模块若只导入内存，给它设置模块默认页数不会影响这些别名。

每层选择完整的页数对。旧命令 `--wasm-set-memory-limit 模块 all MIN [MAX]` 位于单模块层，与同一模块的 `--wasm-policy-single` 页数键重复时会报错。数字选择器覆盖各模块层，按本地定义从零编号并排除导入。同一模块重复设置相同选择器时，无论顺序均报错。

全局**策略范围**表示向所有 Wasm 模块提供设置。每个普通 Wasm 全局变量的类型、可变性与初值由自身声明和初始化表达式决定。例如，`(global (mut i32) (i32.const 7))` 的初值为 7，类型为可变的 32 位整数。`parser.limit.globalsec_globals` 和 `init.local-defined-globals` 约束声明数量。模块策略没有逐个全局变量改值的语义。

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
| `--wasm-set-preload-module-attribute` | 例如 `--wasm-set-preload-module-attribute driver copy 0` 授权名为 `driver` 的原生预加载模块通过复制接口访问调用方的内存索引 0。此授权独立于 Wasm 内存定义者的逻辑页数和策略组成员关系。 |
| WASI、ROS 和 WASI Threads 权限 | 每个宿主接口独立绑定模块和权限，Wasm 策略组不授予宿主权限。 |

[地址掩码与预留](../runtime/adm-memory-reservation.zh.md) · [宿主权限](host-permissions.md)
