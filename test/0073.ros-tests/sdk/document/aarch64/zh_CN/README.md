# 原生架构验收

Linux 宿主入口为 `test/0073.ros-tests/sdk/tools/aarch64/run_container.py`。先传输完整工作文件夹，再提供 `--work`、已安装镜像 `--image`、新的 `--label` 和公共 `--lock`。依赖通过 `--read-only HOST:/deps/NAME` 明确只读挂载，`--` 后的命令在容器内执行。flock 一直持有到本批容器退出并删除；实际内核 cgroup 必须记录 68719476736 字节内存和零 swap。仅有 Docker 命令参数不算通过。默认无外部网络；bridge 仅用于可验证的源码依赖获取。

在该容器中运行 `accept.py --manifest INPUT --output 新目录`。输入格式为 `uwvm-native-acceptance-v1`。真实 QEMU AArch64 进程使用 `target_machine=183`；另行验收的 x86_64 批次使用 62。每个 asset 明确路径、完整 SHA-256、角色；runtime、gateway、原生 peer/解释器必须核对真实 ELF64 架构。QEMU 自己的 ELF 和版本单独记录。launcher 脚本必须指定所配对的 runtime asset；脚本哈希不能代替真实 runtime ELF。

peer 声明独立 ID、完整 argv、明确环境和有界就绪条件。就绪依据为完整 JSONL 原生事件或本批日志文本。`${asset:NAME}` 展开为已核对资产，`${output}` 为新结果目录，未知占位符拒绝。runner 启动独立进程组，保留完整 stdout/stderr、实时 loader maps 和持久 ELF 身份，只停止/回收自己启动的进程。强制清理会明确记录，并使 cleanup 行失败。

每个 case 指定精确 Wasm、SHA、内存位宽、argv、预期退出码及原生见证。命令必须包含已核对的 runtime 或配对 launcher 及精确 Wasm。工具检查真实 memory64 标志并拒绝导入内存、截断元数据和位宽不符；最终 Wasm 合法性仍由真实 runtime 验证。`wire_stdout_regex` 可提取 guest 完整十六进制消息体并与原生 `wire_hex` 比较。发布、服务和动作见证通常必须在 case 启动后出现；订阅只在 guest 完整结果也独立验证时才可显式使用就绪阶段的完整见证。

多模块 case 增加 `main_module_name` 和有序的 `preload_modules` 列表，每项包含 `name`、`wasm`、`sha256`、`memory_bits`。手工 argv 必须明确包含 `--wasm-set-main-module-name main`，按列表顺序给出 `--wasm-preload-library /absolute/module.wasm module_name`，最后给出主模块的 `--run`。rename 是一个普通参数；CLI usage 中的括号表示可选，不是模块名的字面字符。工具核对并记录每个模块的实际字节和内存标志，要求所有 preload 均有声明，不自动添加模块、授权或策略。

不定义内存的纯函数库使用 `memory_bits: null`，它不能证明指针宽度。导入内存仍未获准。声明位宽必须与文件一致；不同模块的明确位宽差异和重复名称可以作为真实 VM 的拒绝测试输入。使用精确非零 `expected_exit_code`、`stderr_contains` 的加载器原始诊断以及原生无影响见证。元数据检查不能证明导入/导出绑定、名称冲突处理、归档提取、跨模块内存安全或 ROS Single 隔离；这些都需要真实生产者/消费者及原生 peer 执行。

权限拒绝使用 `native_witness.absent` 和 0.1–5 秒观察窗口，要求没有对应原生影响。`initial_files`/case 的 `fixture_files` 仅可在新结果目录下写明确的 peer 模式数据，不得修改 runtime 策略。生产 launcher 还必须保留原始 runtime/gateway 回执、主状态、实际长度和独立资源关闭。单独的 peer 成功标志或 guest 退出码不足以通过。

ARM 二进制交叉编译不代表 ARM 编译宿主。该回执始终将 `arm_compiler_host_qualified` 记为 false；ARM 原生 Xmake/前端源码构建需要独立实测批次。QEMU 用户态也不代表物理 ARM 硬件、ARM 内核或 GPU/机器人功能。各组通过同一 runner 提供自己原始类型的完整消息、服务、动作和坏包用例。

`acceptance.json` 在每一步影响前持续写入并保留失败；`files.sha256.json` 封存除自身以外的完整原始结果。已存在的结果目录拒绝覆盖。`test/0073.ros-tests/sdk/tests/aarch64` 中的合成元数据只测试检查器边界，永远不作为 runtime 执行。

--tmpfs-mib选择512..8192MiB可执行临时空间，默认512MiB，全部计入原有64GiB容器限制，不增加内存或宿主挂载权限；明确依赖挂载仍只读。
