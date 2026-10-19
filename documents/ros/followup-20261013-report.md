# UWVM2 ROS2 后续实现与验收（2026-10-13）

本轮针对性验收：**PASS**。实际通过13项原生回归、78项 ROS2 Wasm 资源测试和2项 Wasm Action 通信测试；相同探针对照为旧 Action **FAIL**、新 Action **PASS**。源码与二进制的范围见下文。

本轮补齐 Graph/参数 metadata 的有界编码、Action 计时器的并发所有权，以及发送、取消、回复和原生结果接收的截止时间检查。版本基线是 2026-10-12 冻结的 v8 交付；该版本的完整 15/15 验收仍见 [原报告](test-report.md) 和 [原证据](evidence-20261012-v8.tgz)。本报告只记录本轮实际重跑的范围。

| 实际源码改动 | 行为与目的 |
|---|---|
| gateways/common/bounded_text.h（新增）、config.h | 借用 string_view 先检查完整额度，再验证 UTF8；一次预留输出空间。唯一排序行的文本总量受额度限制，满额重复行不再占额度。canonical 接受借用视图，避免从原生 Graph C 字符串复制。 |
| gateways/ros2/resources.h | Graph、远端/本地参数 list 先过滤再累计额度；描述字段直接编码完整 canonical wire，避免先拼接大描述字符串。逐行及输出前检查截止时间。 |
| gateways/ros1/resources.h、ros2/main.cc、ros2/lifecycle.h | 预留已知输出长度；ROS2 Service/Lifecycle 在发现就绪后、请求发送前及 future 就绪后检查截止时间。 |
| gateways/ros2/action.h | goal->timer 的发布和提取统一受 goal 锁保护；锁外取消并释放旧/拒绝的 Timer。过期回复返回110空输出并退役目标；接收过期反馈/结果不能交付成功。发送和取消在原生副作用前与就绪结果接纳处检查截止时间。 |

新增四个 C++ 探针和三个 Python 驱动，精确路径与相对冻结 v8 基线的新增/修改分类见 [逐文件清单](task-files-followup-20261013.json)。Worker 沿用 v8 dual 二进制 SHA `38b83fa9d79f2eeae32b2bb105bb30abc22ba3f8edac3c18333ea28ba6ac58c6`；本轮没有重新编译 Worker，也不对当前工作区并行发生的非 ROS/JIT 改动做验收归属。

fast_io 复核继续保留 Worker 的字符串格式化、审计/策略输出和 native_file_loader 路径。真实 Noetic/Jazzy 网关仍使用官方 C++17 构建，仓库 fast_io 依赖 C++20 concepts；这些网关使用已有固定缓冲区 native_io 和借用视图，并保留需要原子文件标志、peer credentials、poll 等语义的系统调用。本轮不增加另一个格式化框架，不声称未经测量的速度提升百分比。

| 实际验证 | 结果 |
|---|---|
| Noetic GCC9 / Jazzy GCC13 原生目标 | 8+10=18目标构建通过；原冻结源码及18个旧二进制未变 |
| 有界编码 ASan+UBSan | 两个平台各1139项检查通过；直接编码的超额度/非法结果无需输出分配，完整wire一次预留 |
| 真实 ROS2 metadata/Graph | 43项实际检查通过（含发现重试），含描述128 KiB早期拒绝、本地/远端list与真实DDS前缀过滤 |
| 实际 Node Timer 所有权 | 2个确定性退役屏障、64次替换/关闭竞争、4个锁外析构观察通过 |
| 实际 Action 截止时间 | 1500 ms目标期限；停止派发后到期respond110空，目标/计时器/ID退役；恢复派发后客户端ABORTED空。零预算发送/own和any取消各110空，独立原生请求计数为0 |
| 原生回归 | 13/13 PASS，含ROS1/ROS2通信、资源、TF、Lifecycle、Action、Service及SROS2生成/Enforce |
| ROS2 Wasm 资源 | 78/78 PASS，含参数标量、typed事件、metadata、Graph、Time和Log |
| ROS2 Wasm Action | 2/2 PASS，实际客户端及服务器角色 |
| 旧实现对照 | 只替换旧Action头，其余实际编译依赖与当前候选相同。旧探针实际退出1、日志为过期respond仍被接受；新探针实际退出0且原生ABORTED。原FAIL保留为FAIL；整体修复复现为PASS |
| cgroup / 清理 | 实际有效memory.max=68719476736；OOM计数增量0。新递归rootfs与短别名卸载均返回0，基线输入保持不变 |

[实际聚合报告](followup-20261013-acceptance.json)、[旧实现对照报告](followup-20261013-reproduction.json)、[独立验证与本地源码核对](followup-20261013-validation.json)、[证据索引](followup-20261013-evidence-index.json) 和 [证据包](evidence-20261013-followup.tgz) 记录原始命令、日志、preexec PID/cgroup、实际环境和逐文件SHA。完整包SHA256：`06a5422f7f7d179867931f77836ffd9e57027ea9a12a23f1c83e8499e32536b1`。实际MMD项目依赖57份均与本地当前文件一致；14份本轮源码/测试/驱动均与归档字节一致。二进制保留在SSH工作区，包内只有哈希与文本证据，未包含ELF/Wasm或私钥。


执行环境继续使用用户提供的 SSH 主机 linux、现有容器 4dd58ad4560f 及其 /host chroot；没有创建新容器或新的 memory cgroup。实际有效 memory.max、各命令 preexec PID/成员及 OOM 前后计数由原始报告与独立验证输出记录。

三个先前失败记录原样保留：r1 是新增探针缺失 executor 声明头文件的编译失败；r2 的原生检查通过后，证据脚本把 Make rule 的 .o target 误当输入；r3 过期回复/所有权断言通过，但原生终止结果在 executor 重启后的3秒等待未完成。当前探针改用同步 spin_some，跨 deadline 的暂停点无 executor 回调在执行，并仍要求真实 ABORTED 和空序列。没有把上述历史 FAIL 改写为 PASS，也没有将 executor 取消的调度推测写成已证实根因。

两个验收辅助失败也保留在包内：独立验证器首次把某轮44次检查误当固定值，现按完整固定断言与实际发现重试次数核对，r4日志实际为43次；旧实现对照首轮只采样chroot包装PID而缺少原生main证明。主机uutils chroot实际会fork，修正后的对照遍历存活后代，分别记录旧/新原生main的设备、inode、SHA、父子链及真实cgroup成员。两份原始辅助FAIL均未覆盖。

设 B=min(配置额度,IPC 4 MiB)，B 包含4字节长度；唯一保留行的逻辑文字合计和单条临时行各≤B−4，最终规范 wire≤B。容器节点、string容量/分配器开销及原生 ROS 已生成的 names/types、描述字段不计入该额度，不能称为进程 RSS 或原生反序列化的逐消息硬内存上限。截止时间检查约束副作用准入和返回结果接纳，不提供硬实时抢占，也不能回滚已送出的原生请求。ASan/UBSan 覆盖独立编码探针；原生 ROS 中间件库未整体插桩。原 v8 报告第22项列出的其他 UNSUPPORTED 边界继续适用。


复现入口为 [run_ros2_followup.py](../../tests/ros/run_ros2_followup.py)、[run_action_deadline_reproduction.py](../../tests/ros/run_action_deadline_reproduction.py)、[verify_ros2_followup.py](../../tests/ros/verify_ros2_followup.py)。构建/回归必须在原64 GiB cgroup和原mount namespace内执行，驱动要求新的workspace和未使用的短alias；覆盖文件必须与manifest.json精确匹配。以下为此次实际入口，重跑时将workspace与alias改为新的路径：

```sh
ssh linux 'docker exec 4dd58ad4560f chroot /host /usr/bin/python3 /home/macromodel/.local/share/uwvm-ros-extension/20261013-followup-overlay-r4/tests/ros/run_ros2_followup.py --baseline /u8 --workspace /home/macromodel/.local/share/uwvm-ros-extension/20261013-followup-r4 --overlay /home/macromodel/.local/share/uwvm-ros-extension/20261013-followup-overlay-r4 --alias /u9 --llvm-libs /home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib:/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu'
ssh linux 'docker exec 4dd58ad4560f chroot /host /usr/bin/python3 /tmp/run_action_deadline_reproduction.py --baseline /u8 --followup /home/macromodel/.local/share/uwvm-ros-extension/20261013-followup-r4 --workspace /home/macromodel/.local/share/uwvm-ros-extension/20261013-action-deadline-reproduction-r2 --domain-id 89'
```

原v8的23项交付内容、全部CLI/Schema及未支持范围仍在日期明确的[原报告](test-report.md)；本轮没有把旧15/15当成针对当前所有源码的新完整验收。
