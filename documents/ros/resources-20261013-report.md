# ROS 默认权限与 ROS2 资源边界复核（2026-10-13）

ROS1、ROS2 默认禁用，连接字段和 Capability 表为空。Global、Single、Group 都采用这个初始状态。`enable` 只启用指定 target，不生成资源授权；`grant` 与 `set` 不会启用 target。显式绑定的 Single/Group 完整替代 Global，禁用或空 target 不回退。连接地址、环境中的 ROS 配置、ABI 导入和版本查询都不产生资源权限。权限操作、资源、类型身份和资源限额仍须逐项显式填写；未完整配置的启用 target 拒绝启动。

实际双 ROS Worker 的六种默认/禁用配置全部通过。每种配置的两个 cap_open 返回 13，37 个伪造资源句柄调用返回 9；两个私有 IPC 监听器接受连接数均为 0。25 个 RPC 检查保留输出前 8 字节及 8 字节长度哨兵，cap_open 保留句柄输出，测试没有检查整个 64 字节输出区。基础 stdin/stdout 握手用于捕获真实 ELF 身份。该 Worker 为此前冻结的 C++26 libc++ 二进制，本轮复用执行，八份 ROS 权限相关源码与其真实编译输入及当前工作区一致。

现有 WASIp1 **并非**“除基础 stdio 外默认零权”。真实诊断显示默认继承环境，clock/random 可以调用，普通 stdio 的 base/inheriting rights 是全位掩码；空 Single/Group 也保留这些默认行为。使用合成环境的八种配置确认了继承与显式 noinherit/继承覆盖关系，未输出真实宿主环境。显式把私有监听 socket 作为 stdout 时，sock_accept 能生成 fd3；这使用了启动器已经提供的 socket，不代表 Guest 获得了凭空创建网络资源的能力。目录 stdout 的 path_open 实际返回 54，不能声称能派生目录访问。

本轮没有改变 WASIp1 核心行为，因此“WASIp1 自身默认只允许基础 stdio”明确记录为 **FAIL（现有行为）**。启用 ROS 的严格 Worker 仍要求显式禁止 WASI 环境继承，拒绝 socket/directory preopens、额外原生插件及 socket 标准流。ROS 权限默认拒绝与 WASIp1 既有默认值分别记录。已完善 [安全边界](security.md)、[权限说明](capabilities.md) 和 [ROS1](../command-line/ros1.md)/[ROS2](../command-line/ros2.md) CLI 文档，53/59 个注册参数与文档表一致。

| 本轮验证 | 实际结果 |
|---|---|
| 双 ROS Worker 默认拒绝 | 6 配置 PASS；6 个真实 ELF 捕获；3 份原始 MMD、8 份当前 ROS 源码绑定 |
| sorted_rows 分配复核 | C++17 libstdc++ 普通、ASan/UBSan，libc++ ASan/UBSan，3 profile PASS；每 profile 2182 cases、6571 checks |
| Noetic/Jazzy SDK 编译兼容性 | GCC9/GCC13 的 C++17 sorted_rows 单元实际 PASS；不是 Noetic Gateway 新构建 |
| 共享测试入口 | C++26 sorted-rows PASS，已加入 tests/ros/run_unit.py 的 all |
| ROS2 静态未初始化参数 | 修复前实际 FAIL；修复后及最终 C3 Gateway 原生测试 PASS |
| 清理池所有权/容量 | C++17 普通与 ASan/UBSan 各 8 groups PASS；不代证 SDK/DDS |
| Action advertise/respond 字节限额 | 修复前 4 实际语义 FAIL、4 controls PASS；最终 C3 的 8 modes 全 PASS |
| Action 原生生命周期 | 最终 C3 的 7 modes 全 PASS、14 个实际 ELF 捕获 |
| Action 原生限额回归 | 同一 C3 Gateway 的 8 modes 全 PASS、16 个实际 ELF 捕获 |
| 同一 live owner 额度 | C2 修复前 expired-owner 单模式实际 FAIL；C3 Guest ID 9、终态前额度 11、终态后重试 0 |
| 独立只读证据核验 | 六份原证明合计 10021 checks；保留各自 scope，不等同于 10021 次 ROS 测试 |
| WASIp1 默认只允许基础 stdio | FAIL_EXISTING_BEHAVIOR_UNCHANGED |

本轮实际生产修改为 [bounded_text.h](../../gateways/common/bounded_text.h)、[resources.h](../../gateways/ros2/resources.h)、[action.h](../../gateways/ros2/action.h)、[main.cc](../../gateways/ros2/main.cc)，新增 [action_cleanup.h](../../gateways/ros2/action_cleanup.h)。

sorted_rows 用借用的字符串片段先检查单行限额、查重和总量限额，只为接受的新行分配拥有存储。四种长重复/总量拒绝路径的普通 new/new[] 次数从 1 降至 0；接受新行仍为 2，单行超额原本为 0。保留 UTF-8 字节顺序、错误优先级、完整行格式和嵌入 NUL 的比较语义。该测量不包含 C 分配器、aligned allocation、SDK 内部分配，也不作延迟/吞吐性能声明。Native Gateway 维持 Noetic/Jazzy 可用的 C++17；vendored fast_io 需要 C++20，相关 C++17 I/O 保留固定缓冲、to_chars、借用视图和 native FD，实际 C++20 探针使用 fast_io。

ROS2 参数修复允许显式静态声明、正确类型、尚未初始化的 Bool 首次赋值。正式 SDK 的 Get 返回空 values，随后必须查询正确的静态 descriptor。不存在、动态或错误类型仍返回 95；畸形 multiplicity/name 返回 22。没有隐式声明参数、扩大类型支持或授予新权限；Get/Describe/Set 共享原请求 deadline/QoS。

Action responder 同时遵守自身和 advertiser 的反馈/结果字节限额。反馈比较去掉 responder 的 kind envelope；超出 advertiser 限额返回 90，实际原生反馈或终态均未发生。原 responder 自身超额的错误契约保留，有效的随后回复仍可处理同一 Goal。

Action 清理池在发送原生请求前保留 Goal/client，关闭 alias、会话断开、延迟 acceptance、取消拒绝和读取超时结果都不能提前释放原生生命周期额度。取消回复不是终态；只有已知 SUCCEEDED/ABORTED/CANCELED Result 或尚未开始的拒绝请求才释放相应清理槽。额度严格取自显式 policy 的 max-sessions 与 action.send-goal/max-active-goals，检查溢出，并在启动 spinner 前构造。Guest 读取超时结果后立即作废 GoalID，但 live owner 中保留不可访问的额度占位直到原生终态。expired-owner 明确配置 max-sessions=2、owner limit=1、global capacity=2，验证有空闲全局槽也不能越过该 owner 限额。

服务器拒绝取消或永不终结时，清理容量保持占用并产生有界背压；没有 TTL 丢弃后宣称已取消。Gateway 退出或崩溃不能保证远端 Goal 取消。原生 SDK cancel reply code/UUID 没有原样记录，精确取消由源码、第一/其他 Goal 的实际取消计数和原生状态共同支持。Action advertise/respond QoS 一致性及更广泛的并发组合不由这八个字节限额场景代证。本轮没有广泛 deadline/callback race、整个 SDK sanitizer、跨会话全局身份额度、重新构建四 Worker/运行 36 regressions、重跑 A15、网络性能或 hosted CI 的新声明。原验收范围继续由 [上一轮报告](deadlines-20261013-report.md) 与既有报告说明。

SSH Linux 的实际测试仍在已有 68719476736 字节（64 GiB）cgroup 内，没有修改 memory.max/reclaim。最终 native 构建 jobs=1，实际 headroom 大于 8 GiB 保留量。最终运行首次 umount 成功，任务进程与挂载无残留；两可读祖先的 oom/oom_kill/oom_group_kill 增量均为 0。不可读根祖先字段保留 null，不据此推断为 0。Mount 源只读且递归私有，实际执行前先固定 ELF FD/设备/inode、live PID/PPid/cgroup，再计算 ELF hash。

新增可复现入口在 [sorted_rows](../../tests/ros_sorted_rows/README.md)、[参数初始化](../../tests/ros_parameter_initialize/README.md)、[Action 限额](../../tests/ros_action_limits/README.md)、[清理池](../../tests/ros_action_cleanup/README.md)、[Action 生命周期](../../tests/ros_action_late_close/README.md)。实际完整命令、编译参数、原始 MMD、source/hash 映射、日志、preexec 与原生观测保留在 [验收记录](resources-20261013-acceptance.json)、[独立证明](resources-20261013-validation.json) 和新归档中。所有 bootstrap/mount/sampler/fixture 的原始失败及 C1/C2 中间候选按原字节保留；C1 的早期 PASS 只证明当时测试的 ACK 范围，不证明最终原生终态边界。

最终 [归档](evidence-20261013-resources.tgz) 为 11073239 字节，SHA256 `a9c06365be5260e4c0db0b8b181294362a315cc14654bd090f095ec28cddb3ed`。[索引](resources-20261013-evidence-index.json) SHA256 `dfc83ff2b9086522025bd5de41c9309ebb5266cc77ff368dd3ca446c1115a039`。976 个 regular UTF-8 成员为 974 个数据 payload 加 2 个 planning 元数据，4300 个明确原路径映射，当前 36 项有 canonical 路径。26 条二进制排除声明只包含原记录的元数据；没有二进制 payload。

[本地独立归档证明](resources-20261013-archive-validation.json) 实际 PASS 26146 checks，SHA256 `9df4df5566dcdf16f3669376a74ff863c90a09c0818dbcfd7f4221a84e4bb52c`。它核验真实下载的归档、索引、原状态、最终五份生产编译输入、八份实际 Worker ROS 默认策略输入及当前 36 文件。此前 [57 个正式交付文件](resources-20261013-protected-prior.json) 的实际 SHA/bytes 保持，6 个旧归档未作为嵌套 payload 再次加入。

第一版资源归档漏收清理池报告/proof 及 Action 限额 before 原报告；独立 mandatory-anchor 检查发现三项遗漏。最终第二版补入已存在的原始证据与必要编译闭包，第一版原归档/metadata 和 `FAIL_ARCHIVE_COMPLETENESS` 记录保持，未改变运行测试的状态。

独立本地归档核验命令：

```sh
python3 documents/ros/verify-resources-20261013-archive.py \
  --root "$PWD" \
  --archive documents/ros/evidence-20261013-resources.tgz \
  --index documents/ros/resources-20261013-evidence-index.json \
  --out /tmp/resources-20261013-new-archive-proof.json
```

输出路径须为新的路径。核验不解包、不执行归档内容，逐成员验证 UTF-8/hash/size、固定 tar 元数据、完整 gzip trailer、明确 source aliases、原始状态、当前源码绑定及实际旧文件保护。归档不包含 ELF、Wasm、SDK/rootfs、私钥或嵌套归档。
