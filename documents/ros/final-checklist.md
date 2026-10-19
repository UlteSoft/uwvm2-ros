# 最终验收清单

以下 50 项逐字保留用户请求第三十四节的验收项。`PASS` 仅覆盖证据列所列的已执行范围：当前 Linux 解释器、Noetic/Jazzy 独立 Gateway 和已列出的官方生成初始类型。它不表示全部平台、任意自定义类型或仓库全部历史测试均通过。最终加强后聚合审计15/15套件已完成并通过；全部50项的PASS均受上述范围和报告第22项限制约束。

证据代号与不可变产品、Gateway、Guest SHA、实际命令和报告路径对应于 [test-report.md 的最终证据索引](test-report.md)：`B4` 四独立 v8 产品；`CLI590` 590项真实 CLI 调用；`REG36` 四产品回归子集；`N13` 13项原生 ROS 套件；`W70` 70项实际 Wasm 网络用例；`R243` 243项资源操作（44既有+49标量+29typed事件+121数组）；`G5` 必需五模块场景；`A33` 33项额外拒绝/配置/Group 用例；`I16` 16项入口与 Trap 用例；`C4` 四项撤销/并发用例；`F6` 六项 Service/Log/Graph 洪泛用例；`P18` 18项畸形载荷用例；`T24` 24项实际性能用例；`U62` 62安全单元及 IPC/SDK sanitizer；`SC63` Schema/文档对照；`BC4` 编译后端拒绝四项。代号引用最终 v8 证据，不以旧产品的历史 PASS 代替最终产品结果。

已支持类型及明确的 `UNSUPPORTED` 范围见 [capabilities.md](capabilities.md) 和 [test-report.md](test-report.md)。ROS2 Service call/advertise/respond、Parameter get/set/describe/list、Lifecycle get-state/change-state 要求完整八字段 QoS，DDS 路径比较 Gateway 自有实际请求/响应端点；respond 检查已有独立授权服务且契约必须一致，本地 get/set/describe/list 不创建 DDS client，declare/undeclare 拒绝不适用字段。原生 ROS 库、RMW 和受信任部署仍在 TCB 内，Trusted DL Plugin 本身没有被本扩展沙箱化。

原文要求：“所有验收项必须与真实执行结果对应。”

| 序号 | 用户原始验收项（原文） | 状态 | 最终证据 |
|---|---|---|---|
| 1 | [ ] uwvm2 能在无 ROS 依赖时正常构建 | PASS | B4：`none` 独立产品实际编译、链接、执行 version；UWVM 不包含或链接 ROS 头文件/库。REG36 在该产品上执行回归子集。 |
| 2 | [ ] uwvm2 支持 ROS1-only | PASS | B4：ROS1-only v8 实际构建；REG36 回归，BC4 真实拒绝未编译的 ROS2 参数（126）。 |
| 3 | [ ] uwvm2 支持 ROS2-only | PASS | B4：ROS2-only v8 实际构建；REG36 回归，BC4 真实拒绝未编译的 ROS1 参数（126）。 |
| 4 | [ ] uwvm2 支持 ROS1+ROS2 | PASS | B4：dual v8 实际构建；W70、R243、G5 在同一 dual 产品上执行真实两后端通信。 |
| 5 | [ ] uwvm --help ros1 正常 | PASS | CLI590：真实执行 `--help ros1`，验证分类输出、已注册 ROS1 参数；[build.md](build.md)、[capabilities.md](capabilities.md) 对应当前 CLI。 |
| 6 | [ ] uwvm --help ros2 正常 | PASS | CLI590：真实执行 `--help ros2`，验证分类输出、已注册 ROS2 参数；[build.md](build.md)、[capabilities.md](capabilities.md) 对应当前 CLI。 |
| 7 | [ ] ROS1 Global/Single/Group 完整 | PASS | CLI590、W70、A33、G5：显式创建/配置/绑定与原生通信；显式 Single/Group 不继承 Global，未知模块与不完整目标启动拒绝；两个 camera Group 实例独立会话及配额。 |
| 8 | [ ] ROS2 Global/Single/Group 完整 | PASS | CLI590、W70、A33、G5：显式目标/绑定与原生通信；ROS2 Group 与 ROS1/WASI 同名 Group 独立，Single/Group 不继承 Global；planner Group 和 hybrid Single 分别执行。 |
| 9 | [ ] 没有 Level 权限模型 | PASS | 当前 Schema/CLI 源码对照：63个独立操作 Schema，无 Level、大小关系或数值权限展开；CLI590、A33 的缺项/未知字段拒绝与独立 grant 测试符合该模型。 |
| 10 | [ ] 没有隐藏 Role 权限模型 | PASS | 当前 Schema/CLI 源码对照：授权只取显式目标中的独立 capability grant，无 Role/预设角色展开；A33、W70 的未授予别名/操作返回拒绝。 |
| 11 | [ ] 没有隐式权限展开 | PASS | A33、G5、W70：Global 不并入显式 Group/Single；ROS/WASI Group 不互授；跨模块普通/尾调用不构成委托；Service/Action 的独立操作分别授权。 |
| 12 | [ ] 所有安全相关参数均显式配置 | PASS | SC63 当前63项操作、94项固定字段及5项条件字段（Twist2+numeric参数bounds2+array元素quota1）对照；连接、类型身份、资源、限流/队列/审计缺项拒绝。CLI590 对九项 Service/Parameter/Lifecycle 操作逐一缺失八字段均拒绝；Topic/TF/ParameterEvent八字段、Action五通道40字段显式。 |
| 13 | [ ] 配置不完整时拒绝启动 | PASS | CLI590、A33：缺连接/操作字段、未知/不适用字段、非法值/资源/类型身份均在 Guest 初始化前拒绝；真实启动错误与 Guest 未进入哨兵、原生零通信共同验证。 |
| 14 | [ ] 每个 Wasm Instance 拥有独立 Capability Table | PASS | G5：五个实际 Instance，camera_a/b 共用 Group 模板却各有独立会话/Handle 表；共享 quota 别名各允许2次、第三次11，原生各收到2条，审计每会话2次。C4 验证独立撤销。 |
| 15 | [ ] ROS1 Handle 不能用于 ROS2 | PASS | W70：真实 Wasm 将 ROS1 Handle 用于 ROS2 返回9/拒绝，输出哨兵保持，原生无对应消息；G5 的跨后端未授予别名拒绝补充验证。 |
| 16 | [ ] ROS2 Handle 不能用于 ROS1 | PASS | W70：真实 Wasm 将 ROS2 Handle 用于 ROS1 返回9/拒绝，输出哨兵保持，原生无对应消息；两后端的表、会话和撤销独立。 |
| 17 | [ ] 跨模块 Handle 不能越权使用 | PASS | W70 的外来 Handle 与跨模块普通/尾调用拒绝；G5 的 camera_b 读取 camera_a 仍存活的真实 Handle 后 publish 返回9，独立原生观察无额外消息。 |
| 18 | [ ] Wasm 能向 ROS1 发布消息 | PASS | W70：String/Bool/Twist/Image 实际 Wasm publish，独立 Noetic 原生订阅者逐字段/字节验证；G5 验证 camera/controller/hybrid 实际发布及准确次数。 |
| 19 | [ ] Wasm 能从 ROS1 订阅消息 | PASS | W70：独立 Noetic 原生节点发布四种初始类型，实际 Wasm 经事件队列读取并校验内容；N13 验证显式队列边界和三种溢出策略。 |
| 20 | [ ] Wasm 能向 ROS2 发布消息 | PASS | W70：String/Bool/Twist/Image 实际 Wasm publish，独立 Jazzy 原生订阅者逐字段/字节验证；G5 验证 planner/hybrid 实际发布及准确次数。 |
| 21 | [ ] Wasm 能从 ROS2 订阅消息 | PASS | W70：独立 Jazzy 原生节点发布四种初始类型，实际 Wasm 读取并校验内容；N13 验证实际 QoS、显式队列边界和溢出策略。 |
| 22 | [ ] ROS1 Service 正常 | PASS | N13、W70：官方 Trigger/SetBool 类型的真实 Wasm client 与排队 server 双角色互通，原生分别调用/响应并验证内容；独立 advertise/respond grant，超时/撤销/旧请求 ID 拒绝。范围限这两个初始生成服务。 |
| 23 | [ ] ROS2 Service 正常 | PASS | N13、W70：官方 Trigger/SetBool 真实 Wasm client/server 与 Jazzy 原生互通；八字段 QoS 实际请求/响应端点比较，非默认 depth6/7 生效；不兼容请求110且原生 callback0；独立权限、respond 契约及所有权验证。 |
| 24 | [ ] ROS1 Parameter 正常 | PASS | R243/N13：既有String/Bool/int32/Float64标量，新增Bool/int32/Float64/String数组49项ROS1 Wasm；空/多值、missing创建与exact原生元素核对；malformed22/quota90/bounds13/operation13/kind95及4096哨兵。deny仅5秒采样，无setParam次数/瞬时restore证明；空XmlRpc数组元素kind不可观测。 |
| 25 | [ ] ROS2 Parameter 正常 | PASS | R243/N13：既有30项ROS2标量/29typed事件；新增Bool/int64/Float64/String/byte数组72项ROS2 Wasm，native setter1/getter>=3或拒绝setter0；12local数组由独立getcap核对state。static-existing远端set不是CAS；dynamic/absent95、custom NOT_SET get2，与官方empty分支证据分开。数组events不支持；auto事件仅changed。 |
| 26 | [ ] ROS1 Actionlib 正常 | PASS | N13：生成 Fibonacci 七项操作、目标所有权/取消范围及原生互通；W70：实际 Wasm client/server，原生完整 feedback/result 序列验证及洪泛计数边界。范围为 Fibonacci 初始适配。 |
| 27 | [ ] ROS2 Action 正常 | PASS | N13、W70：生成 Fibonacci 七项操作、真实 Wasm 双角色、原生完整 feedback/result 和目标所有权验证；活跃反馈 teardown/迟到 goal 回调测试通过。五通道40个 QoS 选项应用，非任意 Action 动态类型。 ；W70 ROS2 Action client采用报告第14项的测试专用辅助feedback publisher与private ACK起步条件，不证明普通early/late feedback无损。 |
| 28 | [ ] ROS1 TF 正常 | PASS | R243、N13：TransformStamped 实际 lookup/broadcast，固定父/子 frame、有限数值/translation/quaternion 限界；原生验证1/2/3平移和单位旋转；A33 拒绝 lookup 后原生订阅数0。 |
| 29 | [ ] ROS2 TF 正常 | PASS | R243、N13：TransformStamped 实际 lookup/broadcast、精确 frame 过滤与缓存边界；原生逐字段验证及实际 QoS/deadline 检查。负 ROS2 时间戳等未支持范围明确拒绝。 |
| 30 | [ ] ROS2 QoS 正确生效 | PASS | N13、CLI590：Topic/TF/ParameterEvent 与九项 Service/Parameter/Lifecycle 操作八字段显式；创建 DDS client/server 时核对 Gateway 自有端点，respond 复核已有服务，本地参数路径不创建 DDS client。非默认depth6/7及不兼容110无副作用验证。Action五通道40字段应用并互通，未核验全部最终 getter；任意排列及keep-all/backpressure限制见报告第22项。 |
| 31 | [ ] 多模块权限隔离成立 | PASS | G5：必需五模块实际 start 段/主入口、六资源精确消息、Group 独立 quota、跨模块 Handle9/别名13；W70 普通/尾调用代理拒绝；A33 Global、后端 Group 与 WASI Group 混淆拒绝。 |
| 32 | [ ] ROS1 与 ROS2 同时运行成功 | PASS | W70：同一产品/进程的双向四类型桥接；G5：ROS1 camera/controller/hybrid 与 ROS2 planner/hybrid 同时执行，原生分别证明内容和数量。 |
| 33 | [ ] 同一个 Wasm 能分别访问两套 ROS API | PASS | W70 双后端 hybrid 同一 Wasm 分别调用两套 import；G5 的 hybrid 同一实际 Instance ID 在两后端分别发布，必须分别授予 ROS1/ROS2 grant。 |
| 34 | [ ] 未授权 ROS 操作被拒绝且无对应副作用 | PASS | W70、R243、A33、I16、P18、F6、C4 与 N13：拒绝 errno/启动诊断、输出哨兵及独立原生消息/发布订阅图/Service/Parameter/Action 调用计数共同验证；Graph 拒绝还验证无输出披露。55项逐类范围见 [acceptance-matrix.md](acceptance-matrix.md)，不把仅源码/单元结果当原生无效果证据。 |
| 35 | [ ] WASI 不能绕过 ROS 授权层 | PASS | I16：Global/Single/Group 的 socket/目录 preopen、socket 标准描述符、环境继承及 TrustedDL 混用均在 Guest 前拒绝；原生通信/OS连接0。ROS Worker 必须显式 noinherit、无上述入口；宿主文件/网络隔离与受信任部署仍属 TCB，Trusted Plugin 本身不被沙箱化。 |
| 36 | [ ] IPC 身份和数据长度经过验证 | PASS | N13：实际 Linux SO_PEERCRED正常UID/精确活跃PID、固定manifest/session检查；同UID不同PID、错误manifest/session和非法规范消息拒绝时原生无Publisher/消息。raw framing/header长度/超时由协议单元验证，W70/P18另验真实Guest offset/length/载荷拒绝；错误UID分支未单独攻击。Guest无权指定身份或原始ROS请求。 |
| 37 | [ ] Gateway 异常不破坏另一 ROS 后端 | PASS | W70：分别终止 ROS1/ROS2 Gateway，另一后端原生通信继续、实际 Worker 保持运行；恢复用例验证重新连接，不复用失效会话/Handle。仅所测 Gateway 故障方向，不承诺宿主整体故障隔离。 |
| 38 | [ ] Wasm Trap 后资源安全释放 | PASS | I16：实际 Guest `unreachable` 前创建 publish/subscribe/Trigger 并发生 hello，Trap 后原生发布/订阅/服务图均降为0；pidfd 驱动 Gateway 无协助退出、socket删除。C4/N13 补充撤销/回调并发释放。 |
| 39 | [ ] SSH Linux 上真实 ROS 测试完成 | PASS | 最终 `acceptance-final-v8-io-final/report.json` 15/15套件PASS、1504.007秒；当前v8产品/Gateway输入SHA与嵌套报告SHA核对，真实Noetic/Jazzy通信及拒绝观察通过。 |
| 40 | [ ] 测试进程处于已有 64 GiB Cgroup 内 | PASS | B4、N13、W70、R243、G5、A33、I16、C4、F6、P18、T24 保留实际 runner/编译/原生/Worker 后代成员记录；复用既有容器 cgroup，有效 `memory.max=68719476736`，未创建新 cgroup 或放宽限制。 |
| 41 | [ ] 测试期间没有无控制的 Cgroup OOM | PASS | 已执行报告的前后 `oom`/`oom_kill`/`oom_group_kill` 无增量，性能另有进程 RSS 停止预算；共享 cgroup 累计峰值不当作本任务独占峰值。最终聚合再次核对 `oom`/`oom_kill`/`oom_group_kill` 前后均0。 |
| 42 | [ ] 原有 WASI Preview 1 测试通过 | PASS | REG36：四个 v8 产品真实执行 args/fd_write 精确输出及 memory64 回归；整套共16次真实产品执行、20次夹具编译检查。这是已执行回归子集，未声称仓库全部历史 WASI 测试通过。 |
| 43 | [ ] 原有 Trusted DL Plugin 测试通过 | PASS | REG36：四产品编译并执行仓库现有 `examples/0002.dl` 的插件/Guest 回归；16次执行+20次编译为整套总数。I16 另验证启用 ROS 时该入口启动拒绝；TrustedDL 原生代码仍被信任，未沙箱化，也未执行全部历史 Plugin 测试。 |
| 44 | [ ] 原有 Wasm Runtime 测试通过 | PASS | REG36：四产品实际解释器尾调用、memory64/WASI/现有 Plugin 回归；整套16次真实执行、20次夹具编译。W70 补充 ROS memory64边界/growth/多内存和跨模块路径；不声称全部 Runtime/平台历史测试执行。 |
| 45 | [ ] 测试可自动复现 | PASS | 最终真实运行 `tests/ros/run_all.py` 15套件全部PASS；[testing.md](testing.md)、准确命令/输入SHA、日志和 [证据索引](final-evidence-index.json) 提供复现入口。证据必须非空且明确PASS，缺依赖/有效cgroup为BLOCKED；不声称托管CI已运行。 |
| 46 | [ ] CLI 文档完整 | PASS | [build.md](build.md)、[capabilities.md](capabilities.md) 覆盖当前 ROS1 53/ROS2 59个已注册参数、目标/操作字段、合法范围及拒绝规则；CLI590 实际 help/参数/enable→disable 四种切换验证。范围为已实现 CLI，未支持功能明确列出。 |
| 47 | [ ] 安全模型文档完整 | PASS | [security.md](security.md)、[architecture.md](architecture.md)、[ipc.md](ipc.md) 说明真实 Instance/代际 Handle、显式 grant、会话/manifest、配额/撤销、WASI和宿主TCB边界；[acceptance-matrix.md](acceptance-matrix.md) 按55项攻击注明真实证据和范围。 |
| 48 | [ ] ABI 文档完整 | PASS | 数组u32count+规范固定width/UTF8字符串，121Wasm全wire/4096字节及length事务性哨兵；数值逐元素bounds、count/bytequota，SDK74完整预检后分配；typed事件fixed23+三个u32长度/空删除payload+tag，29Wasm全wire及4096字节/length哨兵；ABI/scalar规范载荷为Bool1/int32LE4/int64LE8/finiteFloat64LE8；R243标量失败get全部64字节和length哨兵不变；int64大于2^53精确；标量get/set的signed extrema/nextafter/-0/minnormal/largefinite仅direct-native/SDK；新增typed事件Wasm已另验MIN/MAX/nextafter/-0。 |
| 49 | [ ] 多模块配置示例完整 | PASS | [multimodule.md](multimodule.md)、[必需五模块说明](../../tests/ros_multimodule/README.md) 和 `examples/ros_multimodule` 五个源例完整配置；G5 原生观察六资源及共享 quota2+2，五实际 Instance 与 Group/Single 隔离通过。 |
| 50 | [ ] ROS1+ROS2 双运行示例完整 | PASS | [双后端例说明](../../examples/ros_dual/README.md) 包含四类型双向 hybrid、外来 Handle、普通/尾调用代理和独立撤销/故障例；W70 实际双运行及独立原生消息验证，G5 补充同一 hybrid 的两套独立 grant。 |
