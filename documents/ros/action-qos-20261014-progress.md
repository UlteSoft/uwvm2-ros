# ROS2 Action QoS 契约补齐（2026-10-14）

本轮状态为 **IMPLEMENTED / ARM_VALIDATION_PENDING**。原任务 G04 的生产检查已实现，回归夹具和新的 ARM 驱动已完成本轮静态核验；尚未编译这些修改或执行新的 ROS 行为。`ssh linux` 仍超时，不能把本轮源码审查、Python 模型或已有 S3 产物证据记成新版本的 ARM PASS。原任务仍未完成。

Action 的 respond 复用 advertise 创建的五类原生 endpoint。此前，两份 grant 各自声明的 40 个 QoS 字段可以不一致，而 Worker、Gateway 和响应操作没有比较它们。新增公共 helper 逐项比较 goal/cancel/result service、feedback 和 status 的八类字段：可靠性、持久性、history、depth、deadline、lifespan、liveliness 和 lease duration。它使用固定 `string_view` 字面键和透明 `find`，不拼接临时键，不新增 I/O 或权限。

| 层级 | 新行为 | 检查范围 |
| --- | --- | --- |
| Worker policy | 不一致返回 conflict17 | 同一 target、resource、type、type_identity 的 advertise/respond |
| Gateway loader | 配置加载抛出明确契约错误 | 单字段合法性检查后，ROS Context 与 IPC socket 创建前 |
| ROS2 incoming Goal | 不一致返回 permission_denied13 | owned/live/incoming/server 检查后，Goal 锁内，原生反馈和结果操作前 |

保留 service 原有的精确文本语义：`8` 与 `08` 冲突，两边同为 `08` 可以匹配。五种 `keep-all` 变更仍属于现有 unsupported95；启用目标先拒绝 unsupported，禁用目标保留既有 unsupported 抑制行为，随后不一致配对返回17。公共 helper 对缺字段返回 false，完整 policy/loader 仍先执行各自的缺字段检查。检查不证明 RMW 实际采用或报告全部40项 QoS。

已有 Action close 的立即 Guest 句柄退役修复保留；ROS1 迟到服务修复源文件保持原 SHA。ROS 默认无权设计不变，WASIp1 默认仅 stdio 的 L01 仍待完成。

新增 [tests/ros_qos](../../tests/ros_qos/README.md) 使用独立40字段矩阵，准备 Worker policy、真实 trusted loader 和 incoming Action adapter 三类夹具，以及288项 CLI/config 输入。原生夹具预期验证35项合法字段 × feedback/success/abort/canceled，再加五项 `8`/`08`，共145次拒绝；检查 Goal、队列、timer、registry、owner/cleanup 配额不变，随后同 Goal 的正常反馈、结果和替代 Goal 成功。存储错误110的测试显式注入内部状态，不冒充真实超时。所有这些是待执行断言。

测试驱动也补了两项来源约束。[run_all.py](../../tests/ros_aarch64/run_all.py) 要求四个独立 Worker profile 及18个 native 产品报告都匹配当前冻结 manifest，并确认实际 canonical native 路径。[artifacts.py](../../tests/ros_aarch64/artifacts.py) 固定快照来源和软链接映射，拒绝把未核验文件或另一个已核验产品混入测试。三个本机 Python 单元方法通过；它们使用非可执行文本文件与 mocked ARM 检查，仅证明报告和文件模型，不是 ELF、QEMU 或 ROS 执行。

新的 Gateway 重编译驱动请求 `-MD`，从成功 verbose GCC 命令读取最终生效的 `-MF`。Jazzy 的 CMake 可覆盖较早的输出路径，报告保存实际 `.o.d`，不制造未生成的 origin 文件。28项纯模型/历史命令解析检查通过，新的 GCC 编译尚未执行。新的 QoS native 驱动核验实际停止的 ARM PID，可以是启动器本身或严格拥有的直接子进程；Worker 的 SO_PEERCRED 同 PID 规则保留。

本轮新增 helper、Worker policy 和公共 Gateway config 都影响编译输入。旧 S1→S2→S3 的窄复用资格只允许先前两项修复，不能拿来覆盖本轮。恢复 SSH 后须物化新的源码/测试 overlay，核验完整 manifest 和 SDK；重新构建或逐项证明四 Worker profile、两 Gateway 和其余受影响产品；在现有64GiB cgroup内执行新夹具、真实 Gateway 启动负例和 CLI 矩阵，再接续完整 network/RPC/performance 与原回归。

配套 [JSON](action-qos-20261014-progress.json) 记录源码、静态证据、prepared overlay 和驱动版本。已冻结12个 overlay 文件、41个驱动文件和静态证据，准备包 `/tmp/uwvm-action-qos-20261014-prepared-r3` 的67份 payload 共2548614字节。其4111文件 manifest 是待在 Linux 物化的完整输入计划，不冒充已存在的新全量源码树。夹具有1030项纯 Python AST/派生检查通过；不计作ARM断言。126份历史文档/证据字节保持不变，所有旧 raw FAIL 保留；本轮不更新最终完成状态或宣称 G04 已通过验收。
