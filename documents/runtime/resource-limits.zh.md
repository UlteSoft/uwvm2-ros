# 宿主分配额度

[English](resource-limits.md) · [模块策略及优先级](../command-line/wasm-module-policy.zh.md)

在 `--run` 前设置宿主的字节额度。同一进程中的 Wasm 实例、WASI 线程和热更新时同时存活的两代实例都从相应资源池取用额度。WASI 和 ROS 的授权由各自的宿主接口管理。

```text
--wasm-set-resource-limit STAGE RESOURCE BYTES
```

`BYTES` 是十进制非负整数，最大为宿主的 `PTRDIFF_MAX`。零会拒绝该项的新增分配。若重复设置同一项，最后一个宿主设置生效。未知资源、负数或溢出属于命令行错误，退出状态为 126。

## 解析与验证

| 阶段 | 资源 | 默认值 | 对象 |
| --- | --- | ---: | --- |
| `parser` | `input-bytes` | 256 MiB | 单个输入模块的文件大小 |
| `parser` | `input-live-bytes` | 1 GiB | 同一进程仍在使用的 Wasm 输入映射和不可变输入副本的总长度 |
| `parser` | `type-metadata-bytes` | 512 MiB | 模块递归类型及其平面 ABI 表示的宿主记录累计字节数 |
| `parser` | `function-body-bytes` | 8 MiB | 一个函数体的编码长度 |
| `parser` | `module-code-bytes` | 64 MiB | 一个模块内所有函数体的编码长度之和 |
| `parser` | `single-bytes`、`live-bytes` | 256 MiB、1 GiB | 解析后的记录、常量表达式、名称表和递归类型元数据的单次申请及同时存活的容量 |
| `validation` | `single-bytes`、`live-bytes` | 256 MiB、512 MiB | 验证器及翻译过程中受限操作数栈、控制栈和临时缓冲区 |

模块可用 `parser.limit.input-bytes`、`parser.limit.type-metadata-bytes`、`parser.limit.function-body-bytes` 和 `parser.limit.module-code-bytes` 收紧相应字节上限。这些字节键都支持全局、组、单模块覆盖，最终取模块配置和进程硬上限中的较小值。

模块的 `parser.limit.*` 数量上限也同时生效。以 `code_locals` 为例，解析器既检查局部变量声明组的数量，也检查所有声明组展开后的变量总数。模块的 `section_storage_bytes` 按记录的宿主类型计算逻辑大小。进程的 `parser live-bytes` 在容器实际申请和释放空间时计费，扩容期间新旧两块内存同时占用额度。多个模块复用一个已登记的递归类型时，注册表保留一份规范类型 ID。临时查找数据由验证器计费。

## 初始化与执行

| 阶段 | 单次资源 | 活跃总量资源 | 单次默认值 | 活跃总量默认值 |
| --- | --- | --- | ---: | ---: |
| `initializer` | `allocation-bytes` | 按资源使用下列执行期池 | 4 GiB | — |
| `initializer` | `metadata-single-bytes` | `metadata-live-bytes` | 256 MiB | 1 GiB |
| `runtime` | `gc-single-bytes` | `gc-live-bytes` | 256 MiB | 1 GiB |
| `runtime` | `table-single-bytes` | `table-live-bytes` | 128 MiB | 512 MiB |
| `runtime` | `memory-single-bytes` | `memory-live-bytes` | 4 GiB | 8 GiB |
| `runtime` | `memtag-single-bytes` | `memtag-live-bytes` | 256 MiB | 512 MiB |
| `runtime` | `auxiliary-single-bytes` | `auxiliary-live-bytes` | 256 MiB | 1 GiB |
| `runtime` | `threads-single-bytes` | `threads-live-bytes` | 256 MiB | 4 GiB |

在较窄的宿主上，字节默认值最高取 `PTRDIFF_MAX`。初始线性内存和表同时受初始化单次额度与各自执行期字节池约束。定义模块可通过 `init.allocation-bytes` 收紧单个表或内存的初始容量。线程实例及恢复的实例按定义模块的额度检查实际要创建的容量。初始化元数据包括模块实例的记录向量、声明的函数引用、表达式形式的元素段、LLVM 的间接调用表视图及常量表达式求值所需的工作区。恢复过程的私有副本和新工作线程各自计费。多个导入绑定同一张表或同一块线性内存时，表槽和内存页由实际所有者计费一次。

| 设置 | 作用范围 |
| --- | --- |
| `--wasm-process-initializer-limit runtime_modules N` | 整个链接图的注册记录，包括主模块、预载 Wasm、原生驱动和本地提供者 |
| `--wasm-process-initializer-limit dependency_edges N` | 一个链接图中不同的“发起模块、提供模块”对数，默认 1,048,576 |
| `--wasm-process-initializer-limit cycle_path_entries N` | 循环依赖诊断中保留的模块名总数，默认 65,536。一条自环路径记录两次同一个名字 |
| `--wasm-process-initializer-limit element_expressions N` | 每个模块所有元素段的表达式项总数，默认 1,048,576。`init.element-expressions` 可按模块收紧 |

通过 `global.get` 初始化相互依赖的全局变量时，初始化器把待求值的定义和操作数保存在受限工作栈中。根定义的深度为零，`--wasm-depend-recursion-limit` 默认允许 2,048 层依赖。把该参数设为零会解除层数限制，工作栈的字节额度依然生效。

例如，同一模块从一个提供者导入两个函数，产生两个导入记录和一条依赖边。为该模块收紧初始化额度可以使用以下形式，其中 `camera` 是预载模块的显式注册名。

```sh
uwvm2-ros \
  --wasm-set-resource-limit initializer metadata-live-bytes 67108864 \
  --wasm-policy-group sensors init.element-expressions 128 \
  --wasm-policy-group-add-module sensors camera \
  --wasm-preload-library camera.wasm camera \
  --run app.wasm
```

## 运行期间的记账和拒绝结果

GC 池按实际对象、数组和数值页的容量计费，包含分配头。回收器依据数组字节数及分配次数判断回收压力，只有能完整识别根的路径才允许自动回收。异常引用登记、GC 根及部分可恢复的 WASI 临时分配使用辅助池。WASI 临时分配被拒时，支持此错误的调用返回 `ENOMEM`。线程池计入原生栈和子线程构建额度，子线程与线程组各自的约束也适用。

表、线性内存和 MTE 标签映射在更换底层存储时计入新旧两份的重叠开销。线性内存的 mmap 后端按已提交的可访问页计费。保护窗口另受进程虚拟地址预留策略约束。受限分配器在移动缓冲区时预留两份容量，成功后才发布新的所有者状态。[地址掩码与预留](adm-memory-reservation.zh.md)

| 操作 | 拒绝结果 |
| --- | --- |
| CLI 指定的主模块和预加载项已超过链接图节点上限 | 命令行预检返回 126，尚未打开预加载模块 |
| 普通初始化器发现模块实体数超过配置的 reserve 数量上限 | 拒绝该运行配置，状态 126 |
| 普通 CLI 加载输入、解析元数据或分配实例存储时字节额度不足 | 拒绝启动，状态 127 |
| 启用 C++ 异常时，私有函数替换或检查点编译无法取得所需解析存储 | 丢弃候选，向调用方返回资源耗尽。已发布的运行代继续使用原对象 |
| WASI Threads 启动配置超额 | 状态 126，来宾入口尚未执行 |
| 执行期间的 `memory.grow` 或 `table.grow` | 返回 Wasm 规定的失败值，保留原大小 |
| 执行期间创建子线程 | 通过 spawn 的结果返回失败 |
| GC 分配或发布所需异常引用失败 | 按执行故障策略处理。`--debug-fault-action exit` 返回 121 |

递归类型注册表在所有新类型组都取得所需存储后一起发布。申请被拒时，已登记类型及调用者已有的类型编号映射保持有效。

判断一次分配是否受限，要从声明追到实际保有存储的对象。例如，自定义节的内容借用输入镜像，节记录的向量则按实际容量计入解析池。异常标签的本地标识按本模块已接纳的标签数创建，每个标识拥有一个固定大小的对象和共享控制块。线性内存每个映射另有固定的同步状态；多个导入指向同一映射时，共用这份状态。每个 Wasm 模块实例拥有 GC 存储及其租约所有者，件数受链接图与实例构建限制，动态 GC 存储另按相应字节池计费。

32 位宿主的受保护映射可以尝试更小且页对齐的可行窗口，最低满足内存声明的初始值。INT、AOT 按实际窗口选择边界检查。64 位宿主上的普通 memory32 使用完整保护窗口。LLVM 对象缓存标识包含所生成的边界检查及相关预留范围。改变预留配置时，仍须满足缓存签名和所有者匹配。

宿主分配器在所有者移动、释放和销毁时维护计费。可恢复的构建路径会清理已经完成的部分。VM 管理的对象和控制块通过 fast_io 分配，外部 LLVM 对象由其自己的分配接口管理。`--wasm-memory-grow-strict` 选择受支持的可恢复内存增长路径。

上述额度衡量列明的宿主请求。总 RSS 还包含 LLVM 优化器、第三方库、分配器内部开销和内核页表等内存。部署时使用操作系统的进程资源边界。比较执行开销时，固定 Wasm、入口和引擎配置，记录缓存状态和重复次数。`--log-verbose` 输出的 `Total WASM execution time` 采用宿主单调时钟，仅计入编译完成后的执行时间。
