# UWVM2 ROS 截止时间与 Image 后续验收（2026-10-13）

本轮针对性验收 **PASS**：18个原生目标重新构建，原生13项、ROS2 Wasm资源78项、Action 2项、ROS1/ROS2 Topic 12项通信回归共 **105项全部通过**。7个单文件旧代码对照的19次运行均出现指定语义失败；原始 `FAIL` 和退出码1保留。独立远端核验通过2297项检查，本地源码核验通过182项检查。

基线为冻结的 v8 及同日先前交付；原[完整 v8 报告](test-report.md)、[先前后续报告](followup-20261013-report.md)及其证据保持原样。本轮的完整原始聚合记录见[验收 JSON](continuation-20261013-acceptance.json)，核验记录见[验证 JSON](continuation-20261013-validation.json)。本轮没有重新运行完整 A15，也没有重新构建 Worker。

| 实现 | 结果与目的 |
|---|---|
| IPC 请求准入 | 从完整请求解码后的准入点计算截止时间。扣除策略及额度处理耗时，再向 Service 和资源 adapter 传递剩余预算；配置与 capability 上限不重新计时。操作截止与原 IPC wire 截止分开，操作到期仍可在 IPC 预算内回送110，之后同一会话可服务下一请求。 |
| ROS2 Action 查询 | executor 尚未派发到期计时器时，查询同步使未完成目标过期，清除反馈、取消计时器并请求原生取消。截止时间前已完成的结果仍可随后读取。 |
| ROS2 参数和 Graph | 本地参数准备及 undeclare 输出预检后、原生修改前检查截止时间；本地及远端参数、Graph 完整编码后再次检查，拒绝晚到的成功结果。 |
| ROS1/ROS2 TF | 从调用入口计算预算，覆盖实体创建、入口锁、缓存查询及最终编码。零 adapter 预算不创建 TF pub/sub；ROS2 被拒绝的订阅释放后唤醒 waitset。 |
| ROS2 Service/Lifecycle | 完整响应 wire 编码后检查截止时间，正常协议字节保持一致。 |
| Image 和固定长度编码 | 借用 Image 输入，去掉原生订阅编码前的 owning 像素及字符串临时复制，一次预留完整 wire。两网关拒绝非规范 uint8 大小端值；String/Twist 也预留已知输出长度。 |

fast_io 的 Worker 路径继续沿用先前优化结果。官方 Noetic GCC9/Jazzy GCC13 原生网关使用 C++17，仓库 fast_io 需要 C++20 concepts；本轮采用借用视图、单次 reserve 和已有 native_io，保持实际原生构建兼容。[Image 编码说明](image-encoding-20261013.md)记录了逐字节差分和分配统计：预先准备1 MiB像素及两个255字节字符串后，仅统计编码表达式，分配由12次/2100139字节降为1次/1049123字节，输出逐字节相同。该统计不代表吞吐量、延迟或整个链路零拷贝。

| 实际验证 | 结果与边界 |
|---|---|
| 原生全量构建 | Noetic/Jazzy各9个目标，共18个，最终 r6 全部重新构建。另编译独立 sanitizer 探针及原生边界探针。 |
| Noetic/Jazzy ASan+UBSan | 两种工具链各运行 bounded-text 1139项、Image 675项/30个差分用例、4个真实认证 IPC 场景。6个独立 ELF 均为非 PIE `ET_EXEC`，两种 sanitizer 保持启用。 |
| 真实认证 IPC | IPC预算20ms/准入阻塞80ms时 adapter 调用0次；200ms/阻塞60ms时收到剩余139ms；IPC400ms、cap200ms时同样收到139ms。cap到期返回110和空载荷，原400ms IPC会话内下一请求成功，收到399ms独立预算。 |
| ROS2 原生资源 | bounded metadata 43项；Timer含64次 replace/stop竞态；incoming Action到期后拒绝晚响应并退休所有权；outgoing Action反馈和结果查询各在未泵 executor 时同步到期，原生取消被观察到，按时完成结果仍可读取。 |
| ROS2 参数/Graph | 83项检查。本地 set/declare回调拖过截止时间后返回110，已准入的修改保留；get/describe/list/undeclare、实际远端 GetParameters、实际 Graph 编码拖过截止时间后拒绝成功载荷。正常读取及修改仍成功。 |
| ROS2 TF/Image/响应 | TF零预算无原生实体或发布，缓存序列化50ms超过10ms时返回110。Image DDS独立 witness收到4条消息，其中2条非法 endian 2/255；网关队列只交付合法0/1。Trigger/GetState/ChangeState正常字节精确，零预算原生请求0，响应就绪后编码阻塞1700ms超过1500ms时返回110。 |
| 原生通信回归 | 13/13 PASS，含ROS1/ROS2、资源、TF、Lifecycle、Action等已有真实通信场景。 |
| ROS2 Wasm资源回归 | 78/78 PASS。 |
| ROS2 Wasm Action回归 | 2/2 PASS。 |
| ROS1/ROS2 Wasm Topic回归 | 12/12 PASS，覆盖两版本String/Twist/Image发布及订阅。 |
| 旧实现对照 | 7个候选源树副本各替换一个生产文件为先前交付版本，保留实际依赖、编译输入和ELF SHA。IPC 4、Action查询2、参数/Graph 8、TF 1、Image 1、Service 1、Lifecycle 2，共19次指定语义FAIL。 |

最终聚合共78条实际命令：59条PASS、19条旧实现预期FAIL。原生依赖由53份编译依赖文件交叉核验，覆盖83个当前项目输入；Noetic CMake 3.16 的 `depend.make` 和普通 `.d` 均被解析和保留。25个 overlay 文件中8个与先前交付SHA完全相同，17个属于本轮改动；新增的独立核验器另行打包。本轮相对先前交付修改9个生产文件、新增7个C++探针和2个Python驱动，共18个源码及测试文件，另新增编码说明；[文件清单](task-files-continuation-20261013.json)给出具体范围与SHA。并行发生的非ROS/JIT修改不归入本轮。

运行使用 SSH 主机 `linux`、现有容器 `4dd58ad4560f` 及其 `/host` chroot。实际有效 `memory.max=68719476736`，未创建新容器或 memory cgroup，OOM计数增量0。preexec PID、长运行原生 main 的存活后代、父子链、设备/inode、ELF SHA和cgroup成员分别记录；会fork的chroot包装PID没有被当成原生main。r6三处临时挂载均第一次卸载成功；独立核验时无本轮存活ELF或残留挂载。v8源码及18个原生产物、先前后续源码及18个原生快照均保持原SHA，本地11个受保护旧交付文件也已核对。

Worker沿用冻结 v8 dual 二进制，SHA256为 `38b83fa9d79f2eeae32b2bb105bb30abc22ba3f8edac3c18333ea28ba6ac58c6`。本轮最终原生网关SHA256分别为ROS1 `c57ac858047ccf9b75ce1d976752abdb148fece7071674da82842ce0123e32aa`、ROS2 `3e41179c429431a2141c69858f190fc5bb54cb4b154bb0707cff189c1e2c3805`。

失败记录也随证据交付。r1/r2/r3/r4/r5聚合均保持FAIL，实际命令数分别为28/0/8/31/72：r1修正新探针SharedPtr用法；r2补齐Noetic `depend.make`依赖解析；r3独立ASan探针失败，原同一PIE产物三次诊断实际为PASS/PASS/FAIL。失败日志未出现 `ASan Init done`，最后记录为安装signal handler，原因未被确定。随后独立探针保留ASan+UBSan并改为非PIE。r4参数正常undeclare探针误用了静态类型fixture，实际SDK验证静态类型拒绝95、动态类型成功0后修正fixture。

r5真实原生回归为7/13 FAIL，暴露操作截止被误用于IPC写回期限的问题。修正共享服务端后，最终r6重新构建全部18个原生目标并通过全部通信回归。r5的 `/u14` busy卸载原FAIL记录保留，另存补充卸载成功记录；最终r6清理无重试。第四IPC探针首次误检请求ID字段的FAIL、以及首次独立核验误用原生JSON身份字段的FAIL和当时核验器字节均保留，随后按真实schema修正并重新实际核验，未把这些失败改写为成功。

测试边界：TF adapter零预算证明没有创建TF pub/sub；Service/Lifecycle零预算证明原生请求计数0，仍可能创建client。IPC帧拒绝 `timeout_ms=0`，这些内部adapter检查不构成Wasm零超时ABI验收。新增malformed Image及TF边界DDS探针只运行ROS2；ROS1相应实现经实际构建和已有正常原生通信回归覆盖。GetState/ChangeState新边界探针使用生成的service端点，完整Lifecycle状态机由已有原生回归覆盖。截止时间检查无法硬实时抢占，不能回滚已经准入的原生参数修改、请求或发布；阻塞探针的实际耗时有意超过deadline。原IPC wire期限本身耗尽时响应仍可能写不出并关闭连接；操作期限单独耗尽时保留原IPC写回预算。

ASan/UBSan覆盖独立bounded-text、Image编码和IPC准入探针，ROS中间件及全部原生目标没有整体插桩。Worker Image解码及验证仍会复制。原v8报告第22项的其他未支持范围继续适用。

证据包为[evidence-20261013-continuation.tgz](evidence-20261013-continuation.tgz)，大小8778003字节，含2180个UTF8常规文件及一份索引；原始日志、源文件、依赖、PID/cgroup证明、5个失败轮次、5份诊断、独立核验器均保留。原生/Worker/Wasm二进制留在SSH主机，以SHA标识。[外部证据索引](continuation-20261013-evidence-index.json)与包内 `index.json` 字节完全相同。证据包SHA256为 `66e196868ca4d6bcd65e74f47fa9c34efaa20c529ae46e1d906a3342e9a48d88`；原始验收JSON SHA256为 `541a211463ba279e62ded350397d0142a52e914f3215512fb461054b36f66e35`。

复核当前源码可运行：

```sh
python3 tests/ros/verify_ros2_continuation.py \
  --local-root "$PWD" \
  --report documents/ros/continuation-20261013-acceptance.json
```

该指令检查当前源码与原始聚合记录。可将证据包中的 `candidate/reports/independent-validation.json` 提取后传入 `--verification`，同时核对当时远端核验器字节和验收记录SHA。

远端保留的最终工作区为 `/home/macromodel/.local/share/uwvm-ros-extension/20261013-continuation-r6`，overlay为同前缀下 `20261013-continuation-overlay-r6`。再次运行使用新的工作区和未占用的短alias路径，保留原始r6；例如以下命令的两个新路径须先确认未占用。该入口重新构建原生目标，沿用v8 Worker；其执行器SHA256为 `aedc4af2445cd99d580d6f445873674fc938509eb008171376bcd34d26d9be31`。

```sh
ssh linux 'docker exec -u 0 4dd58ad4560f chroot /host /usr/bin/python3 \
  /home/macromodel/.local/share/uwvm-ros-extension/20261013-continuation-r6/source/tests/ros/run_ros2_continuation.py \
  --baseline /u8 \
  --previous /home/macromodel/.local/share/uwvm-ros-extension/20261013-followup-r4 \
  --overlay /home/macromodel/.local/share/uwvm-ros-extension/20261013-continuation-overlay-r6 \
  --workspace /home/macromodel/.local/share/uwvm-ros-extension/continuation-reproduction-new \
  --alias /u16 \
  --llvm-libs /home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib:/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu'
```
