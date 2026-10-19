# aarch64 ROS 阶段进展（2026-10-14）

当前状态是 **PARTIAL / WAITING_FOR_LINUX**，原任务未完成。阶段报告及完整本机路径索引已落地；仓库压缩包因ENOSPC未完成，未重复重试，r1/r2/r3原文保留。两项生产修复已有实际 QEMU ARM 语义行为证据；完整 network、RPC 和模拟性能验收仍待续跑。Linux Tailscale 离线，主线记录 lastSeen 为 2026-10-13 21:20Z；随后完整 RPC 的 SSH 启动返回255，远程命令尚未执行，不能记为已启动或已通过。

ROS2 Action close 原先在 native goal 尚未终结时保留 Guest 句柄，关闭后读取反馈仍返回 EAGAIN(11)。现在停止 goal 后立即退役 Guest 句柄，native owner/pool继续维持终结前的配额和回调寿命。修复后的原 close/revoke 场景及16项配额/生命周期测试实际通过。

ROS1 persistent service client 原先在服务广告前创建，等待成功后类型化调用仍返回107。现在先等广告，再创建 persistent连接，保留原3000ms期限和取消机制。迟到 Trigger/SetBool 的旧代码两例107、修复后两例Guest0及原native类型/计数断言，通过149项独立资格检查、50个引用hash复核。native是Rust chroot启动器的严格直接ARM子进程，不能称native同PID；Worker原同PID/birthtime/peerPID规则保留。原native只证明count==1且请求正确在未改动的500ms尾部等待之前成立，不作更强的最终计数主张。

| 已验证范围 | 实际结论 | 限定 |
| --- | --- | --- |
| S1→S2→S3源/SDK/编译参数/产物资格 | 独立3131项PASS；两Gateway新编译，16native及4Worker复用 | 属于产物资格，不等于全部ROS行为 |
| ROS1迟到服务配对 | 独立149项PASS；旧107/107、新0/0，4条native正映射 | 原R4外层rawFAIL保留；不把旧PID当成当前仍活跃 |
| ROS2 Action | 修复后原close/revoke及资源寿命场景实际PASS | native goal配额不会随Guest退役提前释放 |
| ROS2原native＋显式participant profile | 独立66项PASS，只有3项发现字段改变 | 默认profile两次FAIL另列；原endpoint QoS与期限未改 |
| G07真实ROS1/ROS2 multi-memory | memory1、非对齐句柄、7个EINVAL22/sentinel校验、native exact hello PASS | 每后端两次accepted publish，native收到1或2次；不声称必达2次 |
| ROS默认无权 | 六种配置实际拒绝，69个ROS导入合约覆盖 | WASIp1默认仅stdio的L01仍未完成 |

旧失败都保留：SDK R6的旧采样器rawFAIL、ROS2重编译-MMD缺system头的rawFAIL、acceptance R1–R7、迟到R4外层FAIL、RPC旧生命周期诊断FAIL，以及迟到资格R1的9项跨重启st_dev比较FAIL。迟到资格R2分别核对历史映射dev/inode与当前SHA/size/ELF/inode；主机重启后文件st_dev从66312变为66308，不能用新设备号替换旧执行身份。Jazzy期望SDK来自已核验官方layer/deb派生，不冒充原R6 SDK快照。

网络启动中重复串行SDK setup会占用原observer发现窗口；模拟性能的辅助SDK bash标签又被误套到普通native launch映射要求。新的缓存接入已完成**33项纯模型/本机源码审查**，属于 prepared-only，未取得新的完整运行PASS。G04此前仅完成35种有效QoS字段变更及5种history不支持情形的STATIC_ONLY方案审查；后续本轮已实现Worker、Gateway loader及incoming Goal三层检查，夹具/驱动状态见[Action QoS进展](action-qos-20261014-progress.md)，尚未新编译或ARM复现。RPC post-denial Gate的**6915字节静态审查**为PASS_WITH_LIMITS；它保留原错误13、sentinel、native GID谓词和五秒窗口，不授予额外权限。原五秒从baseline开始，稍早于Guest恢复，不能描述成denied操作后重新起算五秒。TwoGateR1在主线toolstdout观察到两例denied、native、architecture/lease PASS；原件尚未本地取回，独立运行资格仍PENDING，不能据此宣布完整RPC通过。

原任务未完成项继续列为G01–G06、G08–G14和L01：custom IDL/registry、统一七类事件、ROS1父参数搜索、Action QoS mismatch、通用backpressure、当前全量验收、Master/原生传输断开恢复、完整Guest SROS2 Enforce、55攻击场景校准、队列/纯IPC性能、完整CI/旧回归、三Group示例、最终完整交付、WASIp1默认stdio-only。G07由新增有限ARM实证补齐。完整条目仍以[原任务审计](original-task-audit-20261013.md)及配套JSON为准，不计算拼接后的完成百分比。

本次归档是r2完整基座＋r3增量；r1/r2/r3原始目录和126历史文件保留。迟到50个引用中22份匹配文本已有归档，14份ELF/Wasm/SDK库按政策仅保存hash，另14份小配置/日志共10025字节待恢复SSH后补取，未离线合成raw文件。新压缩包状态、字节、SHA与索引验证见[aarch64-20261014-progress.json](aarch64-20261014-progress.json)；仓库写包遇到ENOSPC且未完成，半包已清理，不再反复构建，仅清理本任务未发布的旧临时重复压缩副本，原始目录未动。压缩包不代表最终完整交付G14。

所有运行都是x86 Linux kernel上的QEMU user：kernel exe为ELF62，实际映射目标为ARM ELF183。原incarnation inode19435与重启后inode18873分别记录，禁止跨重启拼接OOM/event delta。没有物理ARM性能或内存序结论。

## 来源与接续

远端工作根为`/home/macromodel/.local/share/uwvm-ros-extension/20261013-aarch64-r1`，SDK根为相邻`20261013-aarch64-ros-sdk-r1`。S1/S2/S3均4102文件，manifest SHA分别为`4f3ec096…e1a904`、`bb9d8a05…7f205`、`8dee19dc…0a0d43`，完整SHA和冻结驱动版本路径在配套JSON。S3只改变`gateways/ros1/main.cc`（`68c429ff…5c28e`）和`gateways/ros2/action.h`（`0c9551ad…b28f`）。当前新准备harness单独封存在增量的`prepared-only/current-source`和`source-manifest.json`，不冒充已执行驱动。

先恢复并确认主机与现有容器；以下两条是只读连接检查：

```sh
tailscale status --json
ssh -o BatchMode=yes -o ConnectTimeout=5 linux \
  docker inspect --format '{{json .State}}' 4dd58ad4560f
```

恢复后先补取TwoGate原报告与14份迟到原文，校验各原SHA；重新确认当前boot/cgroup、64GiB/swap0/cpus0-31、源4102文件及SDK字节。将prepared-only harness完整冻结到新的driver目录并核对manifest，然后进入现有cgroup容器：

```sh
ssh linux docker exec -it -u 0 4dd58ad4560f chroot /host /bin/bash
```

在该容器shell中，以下命令以新输出目录和新临时lease继续S3版本的完整network/RPC/模拟性能；它不验收后来新增的G04修改。`TASK_DRIVER`必须已由归档的prepared source完整冻结，若目录或输出已存在，应使用新名称；旧执行报告和lease不能覆盖。G04须按单独进展报告创建新源码和产物资格。

```sh
TASK_WORK=/home/macromodel/.local/share/uwvm-ros-extension/20261013-aarch64-r1
TASK_SDK=/home/macromodel/.local/share/uwvm-ros-extension/20261013-aarch64-ros-sdk-r1
TASK_SRC="$TASK_WORK/inputs/service-discovery-r3/source"
TASK_DRIVER="$TASK_WORK/drivers/progress-resume-20261014"
TASK_QEMU=/home/macromodel/.local/opt/qemu-10.2.1/usr/bin/qemu-aarch64
TASK_LEASE="$TASK_WORK/reports/progress-resume-20261014-binfmt.json"
PYTHONDONTWRITEBYTECODE=1 python3 -B "$TASK_DRIVER/binfmt_lease.py" \
  --qemu "$TASK_QEMU" --name uwvm-ros-arm64-progress-resume-20261014 \
  --report "$TASK_LEASE" --timeout 12000 -- \
  unshare -m python3 -B "$TASK_DRIVER/run_all.py" \
  --workspace "$TASK_WORK" --sdk-workspace "$TASK_SDK" --source "$TASK_SRC" \
  --config "$TASK_WORK/worker-config-r1.json" \
  --worker-build "$TASK_WORK/gateway/service-discovery-r3-qualified/workers" \
  --gateway-build "$TASK_WORK/gateway/service-discovery-r3-qualified" \
  --prepared-resources "$TASK_WORK/resources-prepare-r2" \
  --source-addon "$TASK_WORK/inputs/addon-regression-r1/source" \
  --source-addon-manifest "$TASK_WORK/inputs/addon-regression-r1/source-manifest.json" \
  --topic-observers-manifest "$TASK_WORK/topic-observers-prepare-r2/manifest.json" \
  --qemu "$TASK_QEMU" --binfmt-evidence "$TASK_LEASE" \
  --suites network rpc performance --network-cache-sdk-environment \
  --performance-cache-sdk-environment --rpc-denial-gate \
  --out "$TASK_WORK/acceptance-progress-resume-20261014"
```

结束后须独立复核实际ARM映射、原断言、同一incarnation OOM增量、process/mount/binfmt清理及源/SDK保护，再更新各限定验收；当前阶段报告不授予最终完成状态。
