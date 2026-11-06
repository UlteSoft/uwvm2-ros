# 数据监视、临时断点、回调与类型化状态修改

这些命令属于宿主授权的 DEV 调试器。Linux x86_64、i686、AArch64、
ARM soft-float、ARM hard-float 使用同一套命令和权限检查。被调试的 Wasm
不能调用这些管理接口，也不能取得其他进程或宿主地址的管理权限。
汇编控制仍限于当前拥有的 Wasm JIT 激活。普通解释器不启用调试器。

## 数据 watchpoint

```text
watch memory MODULE MEMORY OFFSET LENGTH
watch locals wasm THREAD FRAME FIRST COUNT
watch globals THREAD MODULE FIRST COUNT
watch operands THREAD FRAME FIRST COUNT
watch EXPR
watch print EXPR
watchpoints [FIRST [COUNT]]
watchpoints disable ID
watchpoints enable ID
watchpoints delete ID
watchpoints old ID FIRST COUNT
watchpoints new ID FIRST COUNT
```

注册和重新启用时读取真实停止点的基准值。之后在完整停止的 Wasm 指令边界
比较；在汇编模式下，线性内存监视还在每次拥有的 JIT 指令单步后比较，
禁止越过可能修改被监视数据的区间进行批量推进。修改导致停下并报告旧值、
新值和命中数。源码表达式需要可用的 DWARF 标量位置；Wasm 选择器不需要 DWARF。

这是软件**值变化**监视：读访问、写回相同值不会触发；一次不可分割的宿主调用、
批量操作或到达完整线程停止之前的多个写入，只能观察其边界上的最终变化。
不把它称为硬件 `rwatch` / `awatch`。数值和 v128 可监视；GC/宿主引用的临时
展示编号不能充当稳定身份，不能作为数值 watchpoint。原生停止点拒绝使用
过时的类型化或源码快照。帧监视绑定原始激活身份，函数返回后明确停下报告并禁用，
不会自动转移到下一次同名函数调用。

监视数量和线性内存范围没有固定容量上限；实际 Wasm 内存边界、整数溢出、
分配失败仍检查。停止消息显示最多 32 字节预览。旧、新完整值保留到恢复执行，
通过 `watchpoints old|new` 每页最多 256 字节读完；列表每页最多 64 项。
这些是单次传输页大小，不是监视范围或保存容量上限。
调试器手动修改状态后重新建立基准值，不冒充客体写入事件。

## 通用临时断点

```text
tbreak bs MODULE FILE:LINE [ignore N] [if EXPR]
tbreak bw MODULE FUNCTION OFFSET [ignore N] [if EXPR]
tbreak bw MODULE SYMBOL [ignore N] [if EXPR]
tbreak bi FUNCTION_OFFSET
```

始终创建独立断点。同一位置的永久断点不被转换或删除。只有实际命中、
消耗完 ignore 且条件为真后才删除临时断点；条件为假或无法求值时保留。
同一停止点的多个断点分别处理。纯 WAT 的 `true` / `false` / `1` / `0`
不需要源码信息；其他源码/Wasm 表达式沿用源码求值器。

汇编断点可用 `condition ID EXPR` 设置只读数值条件，例如
`condition 3 ($pc != 0) && (pc == 0x1234)`。寄存器名可带 `$` 前缀；
五个平台使用各自的寄存器名，并统一支持 `pc`。
表达式支持数值运算、位运算、比较、逻辑运算和条件运算符；
`true` / `false` 按整数 1 / 0 求值，不依赖启用任何源码语言调试器。
条件安装前完整解析并保存不可变语法；每次命中都重新认证当前 JIT 停止点，
只使用编译器证明属于 Wasm 数值、且所有寄存器位均可用的投影值。
未知、部分可用或宿主地址寄存器、内存解引用、成员/下标访问、函数调用、
类型查询及类型转换均拒绝。不会读取旧 Wasm 快照或私有的完整宿主寄存器。
求值失败（例如除零、当前值不再可用）保持真实暂停，临时断点不会被消耗；
失败的条件安装保留原条件。未启用调试器时不解析、不求值这些条件。

汇编断点仍绑定当前代码 owner、generation 和激活；不成为任意原生地址断点。
长时间原生继续操作可以在真实停止点用尽管理时间片，明确返回
`continue time slice ended`，再次 `continue` 从该实际状态推进。
这不是命中目标，也不是不支持某个指令。

## 断点命令回调

```text
commands ID COMMAND[;COMMAND...]
commands ID append COMMAND[;COMMAND...]
commands ID clear
commands ID list [FIRST [COUNT]]
callbacks [FIRST [COUNT [BYTE_OFFSET [BYTES]]]]
```

第一条命令替换整份列表，`append` 扩展列表。整份输入在安装前完成检查，
失败不会安装半份列表。接受 `echo`、`print`、类型化 Wasm 查询/修改、
受保护的寄存器读取、断点 enable/disable，以及放在最后的 `continue`。
不接受 shell、宿主函数地址、任意函数调用、递归控制脚本或热替换命令。

回调在条件通过后的真实完整停止点按顺序运行；临时断点先消耗，再运行回调。
任一命令失败都记录失败并保持暂停，后续 `continue` 不执行。
同一位置有多个断点时，只有所有接受的断点都成功要求继续，且没有数据
watchpoint 命中，才自动继续。失败前已经成功的修改不回滚。

命令列表和执行记录使用动态存储。列表每页最多 32 条；历史每次传输最多
32,768 字节，`callback-next=RECORD byte=OFFSET total=TOTAL` 给出下一页。
UTF-8 字符不会被切断。记录不因分页被淘汰；没有固定历史条数或单条记录
长度上限。分配失败保留真实停止点，不恢复客体。回调输出通过 `callbacks`
读取，未冒充 DAP logpoint 或任意用户脚本运行环境。

## 当前帧局部变量和操作数修改

```text
set wasm local THREAD INDEX bits i32 HEX
set wasm operand THREAD INDEX bits i64 HEX
set wasm local THREAD INDEX bits f32 HEX
set wasm local THREAD INDEX bits f64 HEX
set wasm local THREAD INDEX bits v128 LOW64 HIGH64
set wasm local THREAD INDEX null
set wasm local THREAD INDEX function MODULE FUNCTION
set wasm local THREAD INDEX from locals FRAME INDEX
```

沿用 `set wasm` 的类型化值来源，包括 i31 和原始根/路径选择。
新增目标是当前真实顶层帧的 local、operand；已有 memory、mutable global、
table、mutable GC member 修改继续可用。源语言变量可通过对应 Wasm
局部槽或客体内存修改，没有加入任意源码赋值、宿主寄存器/栈/PC 写入。

完整线程停止、真实 capture、host closure、GC lease、当前 publication
全部重新验证后才写入私有观察包。恢复执行时 JIT 从该包恢复实际 local/SSA
操作数。修改引用时执行规范类型检查与保活；修改前的直接函数调用、GC 借用、
数值位置和长度缓存信息失效。未初始化槽、错误类型、错误线程、越界索引、
挂起的宿主等待、过期句柄和原生停止点均拒绝。修改成功产生新停止标签；
旧 DAP 变量、帧和数据选择句柄不能重用。

DAP 提供 `setVariable`、`dataBreakpointInfo`、`setDataBreakpoints`。
可修改当前类型化 numeric/vector local、operand、global；
整数/浮点文本及 `bits HEX` 转成明确位模式。内存数据断点可使用
`wasm-memory:MODULE:MEMORY:OFFSET:LENGTH`。返回的 dataId 属于当前会话/停止点，
不是地址或权限。只报告 write/value-change 支持；read/readWrite 明确拒绝。
终止陷阱和 uncaught 快照仍只读。

## 性能和验证

普通运行不创建观察包，也不发出状态回写 IR。没有启用的 watchpoint 时，
不会发布额外观察需求；禁用之后恢复原有调试观察门控。启用软件 watchpoint
会付出完整停止、比较和恢复的成本，范围越大成本越高。

维护中的实际产品测试位于 `test/0018.debugger/`：

- `run_debug_data_controls.py`：SDK WAT、数据/条件临时断点、回调成功/失败/
  继续、状态实际写回、DAP 与越界/过期拒绝、70,000 字节范围及长 UTF-8 记录。
- `run_debug_source_watch.py`：直接编译 SDK C 算法，源码变量变化、条件临时
  断点、表达式回调和原始激活退休。
- `run_debug_native_callbacks.py`：五种 ABI 的真实 JIT 停止点、寄存器、
  独立临时断点、回调和机器指令导致的内存变化。
- `run_debug_reference_edits.py`：Wasm 类型验证、函数引用的实际 local/operand
  写回、错误签名/模块拒绝，以及修改后不复用原 `ref.func` 直接调用信息。
- `debug_data_controls.cc`：命令与分页边界解析，根 xmake 的 `test/**.cc` 规则自动发现。
- `test_dap_data_controls.py`：DAP 能力、位宽、过期句柄、未确认修改不重试、
  实际 watch 状态等协议边界；真实写回及命中由前四个产品测试验证。

所有输出要求位于仓库 `.tmp`；测试进程必须实际属于 64 GiB、零 swap cgroup。
使用现有工具链和 QEMU launcher，不安装依赖。QEMU 结果只证明功能正确性；
性能测量仅使用 native x86_64，详见普通执行性能文档。


五平台分别通过 120 项实际产品检查（合计 600）；DAP 全套 553 项通过，
没有跳过。当前帧引用写回的测试包含错误签名、错误模块拒绝和实际调用结果，
不是只检查返回的确认字符串。性能测量和更广回归的范围见
[普通运行与调试性能](ordinary-execution-performance.md)。

运行上述 Python 产品测试时，使用现有 VM 或 QEMU launcher，
传入 `--vm`、`--sdk`、`--out`、`--arch` 和现有 `--wat2wasm` 路径。
源码测试还接受 `--wasm-clang`、`--wasm-ld`，函数引用测试接受
`--wasm-tools`。测试自己校验 cgroup 和输出目录，不下载或安装工具链。
