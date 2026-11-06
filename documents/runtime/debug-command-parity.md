# 三层调试命令：五个平台的实现范围与 GDB/LLDB 差距

本次核对 Linux x86_64、i686、AArch64、ARM soft-float、ARM hard-float。
数据 watchpoint、通用临时断点、命令回调及当前帧类型化修改见
[数据控制命令](debug-data-controls.md)。五者使用同一套源码/Wasm 命令、条件求值和权限判定；汇编解码、寄存器、
陷阱和 Unwind 各有 ABI 实现，需要分别实际运行。

**五个平台一致，不等于已经与 GDB/LLDB 的基础功能完全等价。**
汇编调试始终限于当前进程拥有的 Wasm JIT 代码；没有开启宿主、libc
或其他进程调试的开关。

## 源码层已实现

- `bs MODULE FILE:LINE` 设置可执行源码行断点；`bw MODULE SYMBOL`
  按已发布函数名定位。需要可用的嵌入式 DWARF。
- `bs ... [ignore N] [if EXPR]` 设置条件与忽略次数。
  `condition ID EXPR` 修改条件，`condition ID` 删除条件。
  `ignore ID N` 忽略接下来 N 次命中；先消耗 ignore，再检查条件。
- `info breakpoints`、`enable [ID]`、`disable [ID]`、`delete ID`
  分别列出、启用、禁用、删除断点。
- `step` / `next` / `finish` 对应进入、越过、返回；
  `until FILE:LINE` / `advance FILE:LINE` 提供两种到指定行的执行方式。
  显式形式为 `step src THREAD into|over|out`。
- `bt`、`frame`、`up`、`down`、`info locals`、`print EXPR`
  检查源码栈帧、局部值和只读表达式。可用的 DWARF 内联帧、
  结构体成员、类型和 pretty-printer 查询不涉及宿主代码求值。
- `display EXPR` 提供每次新停止时的自动显示，属于交互 CLI 功能。
- `jump src LINE` 具有解析和拒绝诊断；当前没有可用的源码恢复入口，
  返回 `restore_state_unavailable` 并保留原停止点，不能算作已实现的源码跳转。
- 交互 CLI 的 `run`、`start`、`kill` 管理客体生命周期；
  `continue`、`pause`、`status` 管理运行与停止。

条件为假时恢复执行；为真时停下；类型、变量位置或表达式不可用时，
输出 `breakpoint-condition ... unavailable ... actual stop retained`，
保留真实停止点。优化掉的变量不能伪装成零、false 或成功求值。

## Wasm 层已实现及条件限制

- `bw MODULE FUNCTION OFFSET` / `bw MODULE SYMBOL` 定位实际可执行指令。
  断点启停、删除、命中计数、ignore 与源码层共享。
- `step wasm THREAD into|over|out`、`next wasm THREAD`、`finish wasm THREAD`
  提供无 DWARF 的指令级进入、越过和返回。
- `frames wasm`、`locals wasm`、`operands`、`saved`、`globals`、
  `table`、`memory` 检查逻辑 Wasm 帧、类型化值和线性内存。
- `controls`、`handlers` 及参数/结果布局查询检查结构化控制流；
  `members` / `path` 查询 GC 引用内的字段和元素。
- `set wasm local|operand|memory|global|table|member` 修改允许修改的客体状态，
  当前帧 local/operand 的修改写回实际执行状态；不接受原生寄存器或宿主内存赋值。
- `catch wasm` 支持 call、memory、table、atomic、SIMD、GC、
  reference、control、throw/exception、trap、uncaught 等事件。
  `trace wasm` 记录相应观察，`wasm-script` 可组合受限诊断命令。
- `jump wasm OFFSET` 当前同样返回 `restore_state_unavailable`；
  类型化 local/operand 写回不等于任意 Wasm PC/控制栈迁移。

**当前 `bw ... if EXPR` 仍使用源码表达式求值器。**
有可用源码上下文时与源码断点共用条件语义；纯 WAT、移除 DWARF
的模块不能用 local、operand、global 等 Wasm 原生选择器作为条件。
`true`、`false`、`1`、`0` 可在无 DWARF 时求值；其他表达式仍需要源码上下文。
无 DWARF 的普通断点、单步和类型化状态查询仍可用。
Wasm 条件执行与其普通状态查询尚未达到同等覆盖面。

## 汇编层已实现及条件限制

- `si` / `step asm THREAD`、`ni` / `next asm THREAD`、
  `finish asm THREAD` 提供 Wasm JIT 指令进入、越过、返回。
  导入的宿主函数正常执行，只停在已验证的客体边界。
- `bi OFFSET` / `breakasm OFFSET` 设置当前 Wasm JIT 函数内的指令断点；
  `enable`、`disable`、`delete`、`ignore` 可用于这些断点。
- `disas` / `disassemble` / `disassemble-range` 反汇编当前拥有的 JIT 范围。
- `info registers` / `info all-registers` 读取当前客体陷阱的寄存器。
  各 ABI 布局不同，功能一致不要求名字、数量相同。
- `jump asm OFFSET|*ADDRESS` 只接受当前函数内、通过完整指令边界和
  帧/栈寄存器保护检查的直线区间。
- 汇编断点绑定当前物理激活、代码 owner 和 generation；释放激活后失效。

**汇编断点的表达式条件当前未实现。** 非空 `condition ID EXPR`
被明确拒绝，原停止点不变；不能把 ignore 算作完整条件表达式支持。
源码/Wasm 快照在机器单步后可能失效，不能拿旧快照做汇编条件判断。

## 与 GDB / LLDB 的基础功能差距

比较基准：
[GDB 条件断点](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Conditions.html)、
[GDB 执行控制](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Continuing-and-Stepping.html)、
[LLDB 的 GDB 命令映射](https://lldb.llvm.org/use/map.html)。

- 行/函数断点、普通进入/越过/返回、栈帧、变量和反汇编已有对应能力。
  UWVM 额外提供逻辑 Wasm 栈、GC 引用、结构化控制和 ABI 观察。
- 汇编条件表达式缺失；无源码元数据的 Wasm 条件表达式也缺失。
- 已提供软件值变化 watchpoint；同值写入和读访问不触发，详情及边界见数据控制文档。
- 不提供一般源码赋值、任意 native register 写入、任意函数求值调用。
  类型化 Wasm 状态修改与经过验证的汇编 jump 覆盖的是较小范围；
  源码/Wasm jump 尚未实现可恢复入口。
- 已提供通用 `tbreak` 和命令回调。DAP logpoint 仍未提供；DAP hitCondition
  仍只支持 `>=N` 与 `>N`。
- 汇编断点不是通用持久地址断点：当前激活绑定、指令边界证明和受限
  继续策略会拒绝不能证明安全的目标。原有 65,536 字节断点偏移上限属于当前实现限制，不能称为完整 GDB 等价行为，
  也不是本次 JSON 历史记录的容量上限。
- DAP `setInstructionBreakpoints` 接收 `wasm:MODULE:FUNCTION:OFFSET`。
  原生断点需要调试控制台显式发送 `breakasm`，不能宣称 IDE
  反汇编窗口点击断点已完全对齐。run/restart 由交互 launcher 提供，
  broker/DAP 不支持通用 restart。
- 宿主源码/汇编、任意进程 attach、任意宿主内存属于明确禁止的范围。
  这部分应由用户在宿主使用 GDB/LLDB，而不是放宽客体调试边界。

交互 CLI 的简写、自动显示和生命周期命令不全部属于 broker wire 语法。
DAP 应使用标准 stepIn/next/stepOut，或带明确线程选择器的协议命令。
未验证的 Windows/macOS 产品组合不包括在本次五种 Linux ABI 结论内。

## WASI / ROS 调用过程导出

新增 `trace host save "/absolute/host/path/calls.json"`（DAP Debug Console），
或 DAP `uwvm/exportHostTrace`、独立 `export_host_trace.py`。
详见 [JSON 命令与格式](debug-host-observations.md#save-human-readable-json-on-the-host)。

导出保留参数、返回、耗时、模块身份和完整分段输入输出；
二进制字节不会因 NUL、非法 UTF-8、超出单页而丢失。
保存不会开启记录、改变权限、恢复执行或消费 ROS 消息。
失败、未正常返回、观察丢失如实保留，不能将观察到输入当成操作成功。

## 验证范围

以下保留的是引入数据控制命令之前的验证记录。新增数据控制包含 JIT/控制器修改，
使用重新构建的五个平台 ELF 和数据控制文档列出的维护中测试，不沿用旧 ELF 结论。

每个平台通过真实 broker/DAP 执行 SDK C/WAT 模块，检查：
源码和 Wasm 条件真/假/不可用、更新和删除条件、ignore、启停；
汇编断点、单步、寄存器分页、无效地址和过期引用拒绝；
旧版本曾明确拒绝汇编条件表达式；当前维护测试新增了受保护寄存器条件的
真/假、临时断点保留与消耗、非法读取拒绝及算术失败保持暂停检查；
WASI/ROS 三类调用 JSON 分页、70,000 字节随机输出和失败返回；
当时的纯 WAT 条件不可用检查、普通断点，以及原生停止点拒绝导出。

另在每个平台分别运行真实 ROS1 和 ROS2 gateway 与原生接收端，
核对保存的 topic_publish 输入字节与实际收到的消息。
ABI 版本查询不单独充当 ROS 集成测试。
源码进入/越过/返回、Wasm DAP 状态/修改、热替换回归也重新执行。

生成模块、JSON、原始会话和日志在 `.tmp/debug-command-audit/`。
全部运行在现有 64 GiB / 零 swap cgroup 中；QEMU 仅用于正确性。
本次未修改 VM 热路径，没有把功能测试当成新的 VTune 性能结论。

本轮结果：544 项 DAP 自动测试通过；每个平台 19 项条件断点检查、
33 项原生控制检查、23 项混合调用/导出检查和 3 项边界检查通过。
每个平台另保存并核对 214 个可返回导入（90 个 WASI、124 个 ROS）的
参数位宽和值；两个 proc_exit 导入不计入“可返回”样本。
十组真实 ROS1/ROS2 收发、五个平台的源码与 Wasm/DAP 回归通过。
结果索引是 .tmp/debug-command-audit/qualification.json，包含产品 SHA-256。
首次附加测试选到了只执行一次的启动指令，修正为循环内偏移后重测；
首次单元测试临时目录过长触及 Linux Unix socket 地址长度，改用仓库
.tmp 下较短的测试目录后 544 项通过。这两处均为测试驱动修正。
