# Core Wasm 3.0 引擎对照

[English](core3-engine-comparison.md)

同一份 Core Wasm 字节交给 UWVM 完整解释器、LLVM 后端、Wasmtime 和 WABT。
测试模块内部使用独立计算的标量校验和，且不导入 WASI。UWVM 与 Wasmtime
分别关闭本次对照无关的 WasmMTE 私有扩展和组件模型功能。

使用现有 64 GiB、零 swap cgroup 的入口，将进程固定到测试 CPU，并传入
对应的引擎路径。英文页含完整命令。固定矩阵覆盖 Memory64、多线性内存、
Table64、扩展常量、类型化引用、尾调用、异常、GC、Relaxed SIMD 和自定义
注解。选择 `--seconds 900 --seed 1935627 --iterations 4096` 并提供新的
输出目录，可运行约 15 分钟的可重复随机输入测试。
[运行命令](core3-engine-comparison.md)

记录的墙钟耗时包含进程启动、验证、编译、计算和退出。两种 VM 二进制之间
的比值用于验收部署体积调整。与 Wasmtime 或 WABT 的比值用于解释不同引擎
的端到端差异。若要单独测客户程序内部热路径时间，使用
`run_debug_core3_feature_performance.py`。

保留 WABT 的拒绝诊断，只有其验证器接受的模块才交给 WABT 执行。记录还
包含每个工具的版本、输入摘要和实际执行数。随机用例改变校验算法的输入。
官方 WAST 断言使用固定提交的上游测试单独验证。
[官方 Core 测试](https://github.com/WebAssembly/spec/tree/main/test/core)
· [WABT 测试工具](https://github.com/WebAssembly/wabt/blob/main/test/README.md)
· [Wasmtime WAST 命令](https://docs.wasmtime.dev/cli-options.html#wast)

首轮 x86-64 共执行 593 次，覆盖 20 个固定用例。WABT 1.0.36 接受了 9 个，
其中一个有效模块在实际执行时按预期触发 trap。正常用例中，紧凑版 VM AOT
中位耗时为原 VM 的 0.886–1.015 倍。异常负载和异常引用重抛测试各触发
100,000 次异常，耗时分别为 Wasmtime 52.0.0-dev 的 27.2 和 25.0 倍。原版
VM 也出现同一差距。
