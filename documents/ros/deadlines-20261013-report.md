# ROS client deadlines / native audit / immutable refusal — 2026-10-13

本轮交付三个实际反例对应的窄修复：IPC client 排队和编码预算、C++17 Gateway 审计输出的共享 OFD/SIGPIPE 状态、ROS2 本地只读参数删除异常。四种 Worker 新构建、36 项既有回归、六组 IPC sanitizer、真实 Jazzy 服务与参数/DDS 测试、84 项原生审计场景，以及 Noetic GCC9 编译链接均通过。没有新增权限、隐藏 Level/Role 或隐式权限集合。

## 实际修改与行为

[精确清单](task-files-deadlines-20261013.json)记录24个当前文件及 SHA：修改4生产文件、1测试入口和4文档；新增15个测试/README/只读核验文件。它只包含本轮明确选择的路径，不把工作区已有 Parser/JIT/LLVM 等修改归为本轮。

| 生产文件 | 修改及验证边界 |
| --- | --- |
| [ipc/unix.h](../../src/uwvm2/imported/ros/ipc/unix.h) | client::call 在函数入口、连接 mutex 前捕获绝对期限；锁等待及最终请求编码消耗预算。剩余不足1ms时不写入任何帧，返回110空输出并保留已认证会话；header仅发送剩余整毫秒。开始传输后的失败仍关闭会话，避免半帧污染。 |
| [native_io.h](../../gateways/common/native_io.h) | 审计 pin 原 stderr，用独立非阻塞 OFD 写 pipe/TTY，socket用 MSG_DONTWAIT/MSG_NOSIGNAL；不设置原共享 OFD flags。保留 caller errno、原SIGPIPE mask/handler及既有pending。pipe按完整记录/PIPE_BUF接受或丢弃；regular保持原offset/append。 |
| [server.h](../../gateways/common/server.h) | 预算内审计末尾改用新sink；格式、grant/global配额及饱和 suppressed 规则保持。 |
| [ros2/resources.h](../../gateways/ros2/resources.h) | 仅新增本地 ParameterImmutableException→13 catch，保留会话和兄弟资源。已有静态参数 InvalidParameterTypeException→95 正确，不是本轮新修复。 |

Noetic/Jazzy Gateway 保持 C++17。原生格式化使用定长缓冲、to_chars及有界 native I/O，实际开启测量的helper/预算audit调用中，探针普通C++ operator new/new[]计数为0；C allocator、aligned new及thread/fixture构造不在此测量内；vendored fast_io 需要 C++20 concepts，不强行破坏 GCC9 兼容性。参数测试 probe 使用 C++20及fast_io；Worker 保持 C++26及既有fast_io。

## 原反例与实际执行

远程前缀 B=/home/macromodel/.local/share/uwvm-ros-extension。独立快照继承已交付 contracts-integrated-r1/candidate-r2 的冻结输入，加明确 overlay，未吸收无关当前工作树修改。主线最终 Jazzy Gateway 实际 MMD 同时绑定四个生产新 SHA：unix ff6fe708…、native_io b95b3c93…、server1d78dae5…、resources11d21814…。

| 检查 | 实际结果及范围 |
| --- | --- |
| 原 IPC 反例 | 四个 real Unix socket modes：排队/编码过期仍发送并成功；未过期的 frame 重新获得原预算。原before保持 CONFIRMED_BEFORE_SEMANTIC_FAILURES。 |
| IPC after | 六组C++17/20/26 × libc++/libstdc++ ASan/UBSan，24 modes全部通过；另实际运行原 request-view byte/allocation/跳号回归。actual live pinned ELF记录25条，不是ROS网络。 |
| 测试入口 | 原 tests/ros/run_unit.py 的 framing用例及新增ipc-client-deadline均实际PASS；--suite ipc/all包含新测试，现有CI步骤会选择它，未声称托管CI执行。 |
| 四 Worker | none/ros1/ros2/dual各3compile、link、version均0；新构建耗时114.424/119.061/124.909/125.575s，共12实际MMD。 |
| 回归 | 上述四个新ELF各9项既有WASI/Memory64/tail-call/TrustedDL检查，36/36实际通过；不是全部历史/平台测试。 |
| 真实ROS2 IPC before | 100ms短调用实际排队约153ms；原版仍返回0且独立native quick Trigger请求1。 |
| 真实ROS2 IPC after | 新GW与probe均fresh GCC13 C++17 compile/link；2 modes/7 commands/2 MMD/4 actual-main proofs。过期110空输出、native quick0，同session重试精确响应且计数1；长预算正常请求1。domain196/rmw_fastrtps_cpp。 |
| 原 native审计反例 | 实际预算audit把共享blocking pipe flags1改为2049；reader关闭时默认SIGPIPE实际退出-13。直接函数反例，无原全GW网络声称。 |
| Native审计 after | 3个C++17 profiles：std normal/std SAN/libc++ SAN，每组28场景，共84。满/近满/关闭pipe、socket/TTY、regular offset/append、signals/errno/FDflags、quota/saturation及64并发记录均验证；仅实际开启测量的helper/audit区间普通new/new[]计数0；concurrent/guard场景未开启分配测量。 |
| Noetic | GCC9.4 C++17新ROS1 Gateway compile/link，3命令/18实际项目依赖；无ROS1网络或GW运行声称。一次link子PID的/proc/exe EACCES保留，没有声称所有采样可读。 |
| 本地参数 before | static use_sim_time已有95且同session可读；readonly start_type_description_service实际107、随后get107，兄弟DDS reader消失。原SDK fixture误认static异常的首次FAIL保留。 |
| 本地参数 after | fresh GW C++17/probe C++20，6命令/2 MMD/396项目输入；static95控制、readonly13/get0、动态正常删除2次、兄弟实际消息接收和正常close均通过。domain201。此独立after只覆盖一行catch及旧baseline，其编译输入不混为最终IPC客户端输入。 |

Worker原始四构建report为PASS。首completion wrapper在依赖含../的路径上KeyError而保持FAIL；修正normalize后逐完整snapshot/4ELF/12MMD只读匹配，复用已经成功的构建，仅执行尚未运行的36回归。没有重建或重标原wrapper。IPC早期driver变量遮蔽、两份fixture缺显式标准头、参数初始异常分类、Noetic预算string/int bootstrap，以及独立verifier误认“PASS all”的原失败均保留。

## 证据闭合

现有实际统一cgroup memory.max=68719476736；成功执行的两个可读祖先 oom/oom_kill/oom_group_kill增量均0。根祖先不可读保持null；早期失败缺少的postflight/OOM字段不计0。新native/Noetic/参数私有挂载首次umount均0，owned processes/mounts为空；原baseline source/binaries/report保存检查通过。

独立只读证明分别为：IPC/Worker91725、native audit1195、Noetic379、参数1318、实际本地48旧正式文件保护147。它们保持各自scope与SHA，不把只读核验计为新ROS执行。证明及原始引用见[validation](deadlines-20261013-validation.json)，实际报告索引见[acceptance](deadlines-20261013-acceptance.json)。

[归档](evidence-20261013-deadlines.tgz)为15,796,737B，SHA8c27029245831fdf3f520a2f731ee677d281d568354499cafeae68b4b9881759。含4013 UTF-8 regular成员/87,497,538 payload B：4011数据+2自身metadata。按内容SHA去重，19,730原始source aliases可据bundle/planning/aliases.json及其source_base恢复原拼写；真实MMD来源中的../保留，tar成员名严格规范。1124 binary/archive等条目仅保留排除元数据，无ELF/Wasm/rootfs/私钥payload。

下载后实际独立PASS113991：全成员digest/size、完整gzip trailer、固定tar metadata、全部aliases、硬SHA anchors、24当前文件和48旧正式文件。首archive校验器错误地把合法原MMD来源别名当规范tar名称而FAIL；其原tool6f9d8b56…/proof c3aa71b4…保存于B/20261013-deadlines-delivery-r1/archived-validator-r1。修正仅允许normpath后仍在source_base内的来源别名，tar成员规则未放宽；archive/index字节未改。

aliases metadata5,623,534B、validation wrapper5,793,947B均小于8MiB；字节/限额不从“文本”推导。report、外index、archive verifier/proof保存在archive外，避免digest循环。原48正式文件逐SHA未改。

## 复现与限制

入口：[IPC/client](../../tests/ros_ipc_client_deadline/README.md)、[native audit/Noetic](../../tests/ros_native_audit_io/README.md)、[immutable parameter](../../tests/ros_parameter_exception/README.md)。各driver拒绝覆盖旧输出，设置命令/网络/清理超时，绑定实际compiler inputs和ELF；原准备好的Noetic/Jazzy rootfs、toolchain/RMW仍是复现前提，未打包SDK/rootfs或测试ELF。archive verifier只读核包，不重新编译或启动ROS。

```
python3 documents/ros/verify-deadlines-20261013-archive.py \
  --root /absolute/uwvm2-ros \
  --archive /absolute/uwvm2-ros/documents/ros/evidence-20261013-deadlines.tgz \
  --index /absolute/uwvm2-ros/documents/ros/deadlines-20261013-evidence-index.json \
  --out /absolute/fresh/deadlines-archive-recheck.json
```

这是IPC方法入口/发送准入预算；mutex、regular file和middleware不可硬中断，不承诺硬返回时间，relative frame并不建立全网络共享绝对期限。TTY/socket/regular错误或部分写可留下部分记录；regular I/O可能阻塞。预算audit之外的std::cerr诊断不在本轮sink保证内。Gateway/native ROS库整体没有SAN instrument；SAN只覆盖所列fixture/helpers。

本轮没有重新运行此前的完整A15、70+19Wasm事件矩阵、ROS1网络或DDS-Security矩阵，也未赋予它们对新ELF的PASS。未知动态schema、其他RMW/NUL互操作、非LinuxSDK、ROS+LLVM/named-module和Wasm线程等原限制保持其已记录状态。本报告不声称原任务所有平台/动态类型目标已完成。

## 本轮正式文件

本报告独立于archive；其余8项如下。

| 文件 | 字节 | SHA256 |
| --- | ---: | --- |
| [deadlines-20261013-acceptance.json](deadlines-20261013-acceptance.json) | 7573 | 4492d4aea8655eb38aeaa11e566196e6c4deff517e0a1f79d2057354f560f169 |
| [deadlines-20261013-validation.json](deadlines-20261013-validation.json) | 5793947 | 52718a03ac8255562dc4c1bf79388187c7af64e1e550b63f5f97e3376ed5cafb |
| [task-files-deadlines-20261013.json](task-files-deadlines-20261013.json) | 3847 | d1e75ccb22232f870bc9dd30fde192a312af60d95e2b3c9cd53d9f495cbc93b4 |
| [evidence-20261013-deadlines.tgz](evidence-20261013-deadlines.tgz) | 15796737 | 8c27029245831fdf3f520a2f731ee677d281d568354499cafeae68b4b9881759 |
| [deadlines-20261013-evidence-index.json](deadlines-20261013-evidence-index.json) | 1889331 | c98be73c402e4a4ddad162dd75f9c2ffd2acd43fa093d480850dfd99512f68e9 |
| [verify-deadlines-20261013-archive.py](verify-deadlines-20261013-archive.py) | 24729 | 13ceb6f3ca9a05d620e80fa6fcc7887c3874e55a6cba027e4e5e3dab4bf28a8e |
| [deadlines-20261013-archive-validation.json](deadlines-20261013-archive-validation.json) | 18433 | 8dd03e00d125d2188ee278fa4259f3498734b743b6fc2d4fb6ee4c9de918e471 |
| [deadlines-20261013-protected-prior.json](deadlines-20261013-protected-prior.json) | 7665 | 920375c92235c5fe2c663311c2348f199a50aa5e6019b66ce2cab39e00a785ee |
