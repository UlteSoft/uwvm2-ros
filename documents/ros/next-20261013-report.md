# UWVM2 ROS 句柄生命周期、Action 响应预算及 policy I/O 后续验收（2026-10-13）

本轮验收 **PASS**：四个 Worker 配置及18个原生目标重新构建成功，最终产物的完整 A15 为 **15/15 PASS，1486.698秒**。最终同 alias 多句柄 DDS/Wasm 回归及独立核验也通过。整轮远端独立核验通过86683项，本地范围核验通过140项、覆盖44个文件；最终关闭回归另通过33447项独立检查。

冻结基线是 `20261013-continuation-r6` 的源码，显式覆盖四个生产文件；并行发生的非 ROS/JIT 修改不归入本轮。原始聚合见[验收 JSON](next-20261013-acceptance.json)，独立结果见[验证 JSON](next-20261013-validation.json)，当前17个文件见[文件清单](task-files-next-20261013.json)。

| 修改 | 可观察的行为 |
|---|---|
| capability manager 与 Worker host | 关闭单个句柄使该句柄失效；仅在同一 capability 的最后一个开放句柄关闭时，发送网关 alias close。授权 lease 和 backend 锁覆盖最终 IPC 返回，防止 close/reopen 交错清除新一代实体。 |
| ROS2 Action respond | 从操作入口计算调用及 capability 预算；原生 Feedback/Result 准备后，在原生副作用前重新检查调用预算和已存 goal 截止时间。原生副作用前调用预算到期返回110和空载荷，保留目标供重试；goal 到期退休所有权并停止目标。已准入原生副作用后的最终响应也检查预算，此时目标可能已退休。 |
| Worker policy | owning 持久键继续持有字符串，查询改为透明比较器和借用 string_view；局部模块集合借用输入，减少复制。错误拼接使用 fast_io::concat_std，JSON 沿用 fast_io::print/ostring_ref_std。 |

公开 C ABI、IPC wire 和正常序列化字节保持原有契约。截止时间检查不能抢占阻塞的原生 API，也不能回滚已准入的原生副作用。

| 实际专项验证 | 结果与边界 |
|---|---|
| Action 旧、新对照 | 同一 C++ 探针测试 zero/call/cap/goal 四种预算和 feedback/success/abort/canceled 四种响应，共16模式。旧实现16次均出现指定语义失败；修改后16/16 PASS、364项检查。实际 DDS 观察禁止的反馈和终态，调用预算模式还验证正常重试；goal 模式验证退休及空结果。 |
| 同 alias 多句柄 | 同一 Wasm 和独立原生 witness，旧 v8 Worker 返回209，候选 Worker 返回0。最终集成 Worker/网关再次通过相同契约：h2关闭后原生订阅数仍1，h1收到新的after消息；最后关闭为0；重新打开为1并收到again，最终再次关闭为0。实际仅两次last-reference网关close，旧句柄返回9，第三次open受额度限制11。该见证使用持续DDS流量，不声称是idle测试。 |
| manager 边界与并发 | 普通及 ASan/UBSan 各10组：ROS1/ROS2、多句柄及额度、stale/revoke、独立 alias、并发仅一个 last-close、授权 lease 阻止 reopen。 |
| policy | Clang23 C++20/C++26 × libc++/libstdc++，4个非 PIE ELF 启用 ASan/UBSan，共11164项检查、60次 JSON 字节比较。80字节键查询的临时分配降至0；canonical_dump/validate/detail 分配下降。见[policy I/O 说明](policy-io-20261013.md)。分配计数不是吞吐量或 RSS 测量。 |
| ROS2 Subscription QoS 只读检查 | 7个合法单变量配置（含1ns与有限1秒）均核对实际 SDK 的8个 QoS 字段、原生 endpoint 数量和正常 close 后弱引用释放，无测试侧额外 notify。本项没有生产修改，不构成1ns调度、所有RMW或QoS不匹配覆盖。 |

构建分三次集成尝试。W1 实际重新编译 none/ros1/ros2/dual 四配置，每配置3个翻译单元；全部编译、链接和 version 通过，12份原始 MMD 保留。按实际 cgroup 内存余量选择 jobs=1，共约487秒。W1 后续原生构建的证据采样与进程 exec 变化竞态，原聚合保持 FAIL。

W2 实际重新构建官方 Noetic 的8个及 Jazzy 的10个原生目标，18/18 PASS；实际项目输入74个，32份原始依赖文件保留。W2 A15 的资源套件期间，采集器读取一个 /proc/PID/exe 返回权限错误，运行被清理信号中断，原聚合保持 FAIL；这不是 ROS 语义反例。

W3 精确核验并复用 W1 的四个 Worker ELF、源码及12份 MMD，复用 W2 的18个原生 ELF、74个实际输入、32份依赖、SDK包版本、编译/链接记录。W3 没有再次编译这些目标，完整 A15 从头运行；没有拼接前两轮的部分测试为 PASS。权限不可读采样单独记录，不能充当原生 main 身份证明。

| 最终 A15 套件 | 原始 JSON 条目数 | 结果 |
|---|---:|---|
| cgroup-before | — | PASS |
| unit | 3个子套件 | PASS |
| CLI | 590 | PASS |
| 解释器回归 | 9 | PASS |
| 原生通信 | 13 | PASS |
| ROS资源 | 243 | PASS |
| Wasm/SDK网络 | 70 | PASS |
| 五模块 | 1个完整场景 | PASS |
| 附加安全 | 33 | PASS |
| 入口/trap清理 | 16 | PASS |
| 撤销竞态 | 4 | PASS |
| 洪泛 | 6 | PASS |
| 载荷安全 | 18 | PASS |
| 性能 | 24 | PASS |
| cgroup-after | — | PASS |

这些是各报告列表的准确条目数，不能与专项探针的断言数或核验器检查数相加作为测试数量。整轮核验重读全部32份原生依赖，其中18份含项目输入，14份不含项目输入条目的depend.make；核对544份A15固定快照、2464条实际main存活证明。本地核验范围为44个ROS生产文件及测试/示例overlay文件；单独17文件清单还记录随后加入的generic回归驱动、独立核验器、packer、说明及生成Wasm的SHA。

实际主机为 SSH `linux`、现有容器 `4dd58ad4560f` 的 `/host` chroot。Worker 编译器为 Clang 23.0.0git，commit `4c4c1db7c69a6fda6cfa6bc6066bb09a433edc89`；官方 Noetic GCC9/Jazzy GCC13 原生 SDK 仍使用 C++17。仓库 fast_io 需要 C++20 concepts；原生生产网关继续采用已验证的 native_io、借用视图和 reserve，独立 Jazzy witness 使用 C++20 fast_io。

有效 memory.max=68719476736，三个可见祖先均记录，两个可读 memory.events 的 OOM/kill/group-kill 增量均为0，根目录的缺失状态前后相同。preexec、原生 main 的存活后代、父子链、设备/inode、ELF SHA 和实时 cgroup.procs 分开核验；会 fork 的 chroot 包装 PID 不作为原生 main。W3实际不可读采样数为0，三处挂载均首次卸载成功；最终关闭回归两处挂载也均卸载成功，独立kernel检查无本轮存活ELF或挂载残留。旧冻结源码、18个原生产物及本地20个旧交付文件保持 SHA。

最终dual Worker SHA256：`b4d6d1a46cfa68b043b581f4ac884a7aeae42ef89db06eb2999deee25df93d1c`。ROS1网关：`5dbb003862918a5240273e72385425e32e58e8550f0340c2dfcd89472cc3f1fd`；ROS2网关：`ce135a396e89a0daa6166c6ca349a235ef74b0360d0a5f1d8472f08a466679b6`。最终关闭Guest SHA256：`fc4e8f04356cd83f4cea8cca37d5884b18a048b35c6e0dff562b3a794d23556d`。

失败记录保留各轮原始报告及当时 driver：Action 首 fixture 编译失败、句柄 witness 的类型/额度/首消息 barrier 等调试失败、policy fixture 和缺失 cgroup 字段的聚合失败、QoS 首次 SDK 方法名编译失败、只读核验器初版的 schema/路径问题，以及 W1/W2 集成和清理记录。临时磁盘空间不足只影响本地探针及写入；回收仅限已与旧正式证据包逐字节核验的自建临时解包副本，原归档未删除。

最终关闭首轮在首native命令前失败：驱动列表形参遮蔽命令行参数，读取domain抛错，0命令/0用例；原FAIL及7c0驱动字节保留。只修变量名后，在新r2用同WAT/witness/ELF实际PASS。首整轮核验错误要求W1的alias卸载行；W1在native采集失败时尚未创建alias，新核验器明确要求W1未进入A15、native阶段失败、两SDK挂载和补充清理记录，同时检查/u17不存在。原FAIL及后续PASS均保留；后续输出精确区分32份原生依赖与18份含项目输入的文件，最终重新实际核验通过86683项。10项核验器自检通过，含8种损坏cgroup证明的拒绝。

旧 test-report.md、followup-20261013-report.md、continuation-20261013-report.md 及对应证据保持原样。原 v8 报告第22项的未支持范围仍适用；本轮没有实现全部 ROS 消息、全部 RMW 或所有分布式实时语义。SDK rootfs 是测试环境，证据保护范围为冻结项目源码、产物、包版本及报告，不声称整个 SDK rootfs 不发生 ROS 日志写入。

Action零预算模式为内部adapter探针，不构成Wasm零超时ABI验收。sanitizer覆盖独立policy和manager探针，全部ROS中间件和网关未整体插桩。原v8 I/O兼容路径仍保留浮点极小值的classic num_get边界回退；不能把减少字符串临时对象或本轮性能套件PASS声称为fast_io吞吐提升。

[正式证据包](evidence-20261013-next.tgz) 为 **25,177,484字节**，SHA256 `8a1820e3786381cc947596bed82dda257fe43b78085e1f46bbb26515c1629ba3`。148个输入映射生成8,304个payload成员，未压缩payload共133,406,763字节：8,303个UTF-8文件，以及一个明确标记的历史policy二进制压缩包。历史包保持原始SHA `c815a628a1067d3545443c621a5b022ebbaa2df03591d8563e17a4802e01434d`，内部83个普通文件和31个目录。主包不包含ELF、对象文件或生成Wasm；其原始SHA、构建输入和命令保留。2,622个跳过记录为273个ELF和2,349个不在文本扩展名集合内的文件。

[外部证据索引](next-20261013-evidence-index.json) 为4,427,778字节，SHA256 `ac51e8f214ce78f75f073d514c1a1e9b7355fe28246d73180a02f31e1b021302`；索引是单独交付文件，不是归档成员。归档内保留实际输入清单、打包命令和prepack核验。归档完成后生成的[实际打包执行记录](next-20261013-pack-execution.json)、本报告和本地归档核验也独立交付，避免自引用SHA。packer实际退出0，耗时4.609秒；主归档及索引的生成命令没有覆盖旧交付。

本地另用独立流式实现核对全部成员的SHA、长度、严格UTF-8、规范相对路径、普通文件类型和固定tar元数据，并读到主包及嵌套包gzip trailer。主成员集合与外部索引完全相等；17个当前文件SHA全部核对，其中16个文本源码/工具/说明在包内，生成Wasm按清单明确排除；必需的原始验收、完整远端独立核验、完整关闭核验及实际打包清单均按SHA定位。20个旧交付文件再次核对未变，见[本地归档验证](next-20261013-archive-validation.json)。这些是归档完整性检查，不是新增ROS测试。

归档核验可在仓库根目录复现，输出必须使用新的文件名：

```sh
python3 documents/ros/verify-next-20261013-archive.py \
  --root "$PWD" \
  --protected documents/ros/next-20261013-protected-before.json \
  --out /tmp/uwvm-ros-next-archive-verification-new.json
```

本地ROS范围核验可从包内准确提取一份275,335字节的远端核验JSON，不解包整个归档。在仓库根目录执行以下命令，读取原始报告及当前源码，实际冻结版本通过140项、覆盖44个文件：

```sh
python3 - <<'PYVERIFY'
from pathlib import Path
import hashlib, subprocess, sys, tarfile, tempfile
root = Path.cwd()
with tempfile.TemporaryDirectory(prefix='uwvm-ros-next-readonly-') as temporary:
    proof = Path(temporary) / 'remote-verification.json'
    with tarfile.open(root / 'documents/ros/evidence-20261013-next.tgz', 'r:gz') as archive:
        data = archive.extractfile('final/independent-total-verifier-r5/final-total-remote-r3.json').read()
    assert hashlib.sha256(data).hexdigest() == '4660ec865de0f8f757f3a0a3dcb93fe40607a79a3bb842c8763bc486146ee901'
    proof.write_bytes(data)
    subprocess.run([sys.executable, str(root / 'tests/ros/verify_ros2_next.py'),
                    '--local-root', str(root),
                    '--report', str(root / 'documents/ros/next-20261013-acceptance.json'),
                    '--verification', str(proof)], check=True)
PYVERIFY
```

远端完整只读核验在现有Linux容器的`/host` chroot内执行；原始命令保存在归档`final/independent-total-verifier-r5/executed-command.json`，与下列参数一致。核验器不编译、不运行ROS、不挂载、不改动原报告。进程存活及挂载检查反映核验当时的系统状态，因此后续环境变化会影响该部分：

```sh
ros_evidence_base=/home/macromodel/.local/share/uwvm-ros-extension
/usr/bin/python3 "$ros_evidence_base/20261013-next-integrated-verification-r5/verify_ros2_next.py" \
  --workspace "$ros_evidence_base/20261013-next-integrated-r3" \
  --baseline "$ros_evidence_base/20261013-continuation-r6" \
  --alias /u19 \
  --reuse-worker "$ros_evidence_base/20261013-next-integrated-r1" \
  --reuse-worker-alias /u17 \
  --reuse-native "$ros_evidence_base/20261013-next-integrated-r2" \
  --overlay "$ros_evidence_base/20261013-next-integrated-overlay-r3" \
  --sdk-rootfs-workspace /u8
```

需要重新编译并执行完整A15时，同一chroot内使用冻结overlay，创建新的输出目录和未占用的短alias；以下示例不传复用参数，会重新构建四个Worker及18个原生目标。它是复现命令，没有在本轮作为额外第四轮执行：

```sh
ros_evidence_base=/home/macromodel/.local/share/uwvm-ros-extension
/usr/bin/python3 "$ros_evidence_base/20261013-next-integrated-overlay-r3/tests/ros/run_ros2_next.py" \
  --baseline "$ros_evidence_base/20261013-continuation-r6" \
  --rootfs-workspace "$ros_evidence_base/20261012-recovery" \
  --overlay "$ros_evidence_base/20261013-next-integrated-overlay-r3" \
  --workspace "$ros_evidence_base/20261013-next-reproduction-new" \
  --alias /u20 \
  --compiler /home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang++ \
  --llvm-libs /home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib:/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu
```

冻结输入归档使本轮可复现，不能将当前工作区其他未归入overlay的变动视为已由本轮构建验证。当前交付文件的SHA和长度另见[交付元数据](next-20261013-delivery.json)。

