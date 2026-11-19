# SDK 客户模块源码调试

[English](debug-native-languages.md)

安装与应用对应的平台和语言工具。支持 Xmake 调试配置的项目使用
`xmake f -m debug` 后执行 `xmake build`。需要优化和调试信息时，已有
`releasedbg` 配置的项目可使用 `xmake f -m releasedbg`，也可以按适配器
文档在构建配置中设置 `set_symbols("debug")`。优化后，相邻语句可能共享
同一个指令位置，部分变量可能没有可读取的位置。部署、运行和调试均使用
同一份 Wasm 文件及应用原有的 ROSv2 和 WASI 权限配置。

VM 从已加载的 Wasm 读取编译器元数据。函数名用于函数断点，源码位置映射
决定源码断点和单步，变量查看还需要停点处的类型与位置。调试权限由宿主
单独授权，原有导入和资源权限仍由 VM 检查。恢复执行、替换模块或断开连接
会使相关的栈帧和变量引用失效。

## SDK v2 验收

| 语言 | 实际看到的调试能力 |
| --- | --- |
| Zig | 安装后的 Zig 0.16.0 在 wasm32 和 wasm64 上均命中原始源码第 10 行，读到只读标量 `v: u32 = 2`，单步到第 11 行，旧栈帧引用被拒绝。 |
| Go | 受限寄存器后端支持 Wasm 函数断点。使用 `wrgo -g` 会写入来自 Go 编译器 SSA 位置的映射，已验证原始源码断点和跨语句单步。该映射不包含变量位置或类型。 |
| Java、Kotlin | 更新后的 TeaVM C 路径保留字节码中的原始文件与行信息。设置宿主源码映射后，原始文件断点已通过，同一行内的不同位置可单步。局部变量仍使用生成 C 的表示。 |
| C | 优化后的 ABI 示例命中原始 `body.c`，下一处位置仍在同一行，列号不同。 |
| C++、Objective-C、D、Fortran、TinyGo | 样本行表含应用原始源码。各语言的断点、单步和变量还需针对实际项目分别验收。 |
| Rust | 样本行表含应用原始源码。优化产物上指定 ABI 调用行的 DAP 断点未验证。 |
| C# | AOT 样本含 `Filter.cs`。该路径的现场停点和变量暂未验收。 |
| Nim、F#、Visual Basic .NET | 样本含 DWARF，尚未观察到原始应用文件。 |
| JavaScript、TypeScript、Lua、Python、Ruby | 当前样本含解释器或桥接层信息。脚本源码停点和变量需要对应语言的元数据和调试路径。 |

认证后的 DBG 和 DAP 测试可验证 IDE 所用的协议。图形 IDE 另有界面验收，
未列出的语言也需要提供实际编译产物和对应源码来验收。

## 复验

在受控测试环境中运行 `test/0084.ros-runtime/run_sdk_source_debug.py`。
提供 VM、原始源码、实际 Wasm、两个有可执行位置的源码行和正常运行参数。
重复使用 `--vm-option=参数` 传入 VM 参数。使用 `--value NAME=VALUE`
时，程序会校验真实可见的变量值。若编译器路径与本地项目路径不同，
将由宿主审阅的源码根目录映射以 JSON 文件交给 `--source-map`。测试记录
包含输入摘要、原始停点、第二个停点和进程退出结果。

Linux 可将使用 `-O3 -g` 构建的 VM 的宿主 DWARF 单独打包。分离后的
调试文件用于 GDB 等宿主工具的源码和汇编定位，客户模块继续读取自己的
Wasm 元数据。部署时按打包后的 VM 摘要重新进行审批和固定。
[Linux 发布步骤](../how-to-build/linux.md#compact-runtime-with-separate-linux-debug-information)
