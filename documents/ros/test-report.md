# UWVM2 ROS 1 + ROS 2 交付与真实验收报告

验收日期：2026-10-12。源码工作区：`/Users/liyinan/Documents/MacroModel/src/uwvm2-ros`。实际 SSH 工作区：`linux:/tmp/uwvm-ros-extension-20261012`。

最终v8真实验收：**PASS，15/15套件，1504.007秒**。此次在 ROS1 Bool/int32/Float64/String 与 ROS2 Bool/int64/Float64/String/byte 参数数组上继续 Worker/native I/O 改写，保留既有标量与typed事件适配；结果仅覆盖明确的 Linux 解释器、Noetic/Jazzy 初始生成类型和有界工作负载。原始命令、SHA和限制见 [最终聚合报告](final-acceptance.json)、[证据索引](final-evidence-index.json)、[证据包](evidence-20261012-v8.tgz) 与第22项。

## 1. 实际修改的源码文件

精确文件列表、修改/新增分类、逐文件 SHA-256 和可用的任务开始基线由 [task_manifest.py](../../tests/ros/task_manifest.py) 生成于 [task-files.json](task-files.json)。现有文件的任务修改涉及：

| 文件 | 本次修改目的 |
|---|---|
| `src/uwvm2/runtime/lib/uwvm_runtime.h` | 受信任 Instance 标识、实际调用链与 ROS 内存复制入口 |
| `src/uwvm2/runtime/lib/uwvm_runtime.default.cpp` | 解释器跨模块调用、尾调用身份追踪 |
| `src/uwvm2/runtime/lib/uwvm_stub_runtime.default.cpp` | 不支持的执行路径拒绝 ROS 身份查询 |
| `src/uwvm2/uwvm/run/run.h` | 策略验证、独立后端实例注册、RAII 清理、严格入口限制 |
| `src/uwvm2/utils/cmdline/handle.h`、`src/uwvm2/uwvm/cmdline/parser.h` | 增加独立 ROS 帮助分类和参数处理 |
| `src/uwvm2/uwvm/cmdline/params.h`、`cmdline/params/impl.h`、`cmdline/params/help.h`、`cmdline/callback/impl.h`、`cmdline/callback/help.h` | 注册 ROS1/ROS2 显式参数与回调 |
| `xmake.lua`、`xmake/option.lua` | 独立 `import-ros1` / `import-ros2` 编译开关 |
| `README.md` | 扩展、构建与测试文档入口 |
| `cmdline/callback/ros.h`、`cmdline/params/ros1.h`、`ros2.h` | numeric参数min/max值槽的窄signed pretreatment；六种set注册，不放宽通用parser或unsigned quota |

任务开始时仓库已有大量未提交修改。本次没有重置它们。保存的前端/README 基线可区分本次编辑；三个 Runtime 文件没有任务开始前完整副本，因此不能把它们相对 HEAD 的整个差异归因于本次任务。最终清单记录完整交付文件及这一限制。

[最终源码绑定检查](final-v8-source-comparison.json)记录当前产品 manifest SHA `4ee3ff82481241cb9c6f2a1a14f098ac19f2d4b726e903267449cce7cca9e036`、dual SHA `38b83fa9d79f2eeae32b2bb105bb30abc22ba3f8edac3c18333ea28ba6ac58c6`：完整manifest有3168项实际输入，其中src为2307份；新构建采用恢复后当前源快照，包括保留的用户非ROS修改，不把这些修改归为ROS工作。远端完整输入与冻结副本一致；当前本地差异11项按比较JSON逐项记录，本地全src状态为DRIFT。Runtime cpp/h/stub三文件为完整字节及SHA精确一致，无历史attribute例外。`reports/final-v8-core-inputs.json`包含完整manifest所有路径，实际编译输入副本在`reports/final-v8-core-inputs/`，未reset或覆盖用户编辑。GW18冻结73份实际依赖/CMake输入；B100K记录397项实际依赖token、387个不同路径，SAN规范化400份独立输入和400个实际依赖记录，SP2为4份输入；每项SHA与不可变副本核对。保留的恢复时用户非ROS输入为12项；并行非任务变更若出现将准确记录而不豁免Runtime或任务源码。

## 2. 新增源码文件

| 目录/文件 | 内容 |
|---|---|
| `src/uwvm2/imported/ros/` | 公共类型、严格消息验证、策略 Schema、独立 Capability Manager、Linux IPC |
| `src/uwvm2/uwvm/imported/ros/` | Wasm Host ABI、实际 Instance 绑定、授权与受控复制 |
| `src/uwvm2/uwvm/cmdline/params/ros1.h`、`ros2.h` | 完整独立 CLI 参数注册 |
| `gateways/common/`、`gateways/ros1/`、`gateways/ros2/` | 两个原生网关、会话/资源状态、Topic/Service/Action/TF/参数/Graph 适配 |
| `sdk/ros1/`、`sdk/ros2/` | C ABI、C++ RAII 与规范消息编码工具 |
| `examples/ros1/`、`examples/ros2/`、`examples/ros_dual/`、`examples/ros_multimodule/` | 实际 Wasm、C/C++ SDK、五模块、双后端与恶意 Guest 示例 |
| `tests/ros*/`、`tests/test_ros*.sh` | 编译、单元、原生/Wasm 网络、安全、性能、cgroup 和回归入口 |
| `documents/ros/`、`documents/command-line/ros1.md`、`ros2.md` | 架构、协议、ABI、CLI、Schema、审计与复现文档 |
| `.github/workflows/ros-extension.yml` | 四编译组合和回归检查工作流；本次没有声称远端 CI 已运行 |

最终文件数量及逐文件哈希以交付清单为准。

## 3. 实现的 ROS1 API

ROS1 有 **29 个独立操作 Schema**：Topic publish/subscribe；Service call/advertise/respond；Parameter get/set/delete/exists/search/list；Graph list-topics/list-nodes/list-services/inspect-topic/inspect-node/inspect-service；Action send-goal/cancel-own-goal/cancel-any-goal/receive-feedback/receive-result/advertise/respond；TF lookup/broadcast；Time read-ros/read-wall；Log write。

另外提供 ABI 版本、Capability open/close/revoke 与 event_poll。消息初始适配为 String、Bool、Twist、Image；Service 为 Trigger、SetBool；Actionlib 为 Fibonacci；TF 为 TransformStamped。准确函数签名、规范字节布局和错误码见 [abi.md](abi.md)，所有操作字段见 [capabilities.md](capabilities.md)。

ROS1 参数 get/set 支持 String、Bool、int32 与有限 Float64；metadata 操作保持 String。ROS1 受限 scalar set 可创建不存在的参数，已有参数要求实际原生 kind 一致。

ROS1 参数数组 get/set 显式支持 builtin/bool-array-v1、builtin/int32-array-v1、builtin/float64-array-v1、builtin/string-array-v1；元素种类、inclusive bounds、count与完整wire额度按cap校验，缺失参数可创建。typed数组不扩展metadata或事件操作。

## 4. 实现的 ROS2 API

ROS2 有 **34 个独立操作 Schema**。公共 Topic/Service/Action/TF/Log 操作独立实现；参数为 get/set/declare/undeclare/describe/list/subscribe-events；Graph 在公共六项上增加 inspect-action；Time 为 read-ros/read-system/read-steady；Lifecycle 为 get-state/change-state。

Jazzy 适配使用官方生成的 String、Bool、Twist、Image、Trigger、SetBool、Fibonacci、TransformStamped、ParameterEvent 和 Lifecycle 接口。参数支持 String、Bool、int64、有限 Float64。declare/undeclare 仅操作明确配置的 Gateway 节点，远端 declare/undeclare 返回95 UNSUPPORTED。

ROS2 typed get按原生kind读取；非String typed远端set要求static-existing参数；本地 typed declare/undeclare 仅受信任 Gateway 节点。显式参数数组现受支持；ROS2 param.subscribe-events 显式支持 Bool/int64/有限Float64 typed事件，与String legacy事件和scalar get/set权限独立。

ROS2 参数数组 get/set 与Gateway本地declare/undeclare显式支持 builtin/bool-array-v1、builtin/int64-array-v1、builtin/float64-array-v1、builtin/string-array-v1、builtin/byte-array-v1；远端array set要求static-existing并核对native kind，数组事件与远端declare/undeclare不支持。

## 5. 命令行参数完整列表

[ROS1 CLI 完整表](../command-line/ros1.md) 列出 **53** 个已注册参数；[ROS2 CLI 完整表](../command-line/ros2.md) 列出 **59** 个已注册参数及各参数占用的实参。实际 `--help ros1`、`--help ros2`、缺失字段、未知字段、重复配置、非法绑定检查已通过590项产品 CLI 测试（包括 enable→disable 的最终配置语义）。完整列表不使用 Level、Role、Preset 或 JSON 授权。

## 6. 每种权限的参数 Schema

[capabilities.md](capabilities.md) 给出全部63个版本/操作 Schema、94项固定字段和5项条件字段（Twist2、数值参数范围2、数组元素quota1）的类型/范围、跨字段约束和官方类型身份。启用目标必须显式提供连接和拒绝审计字段；每个 grant 必须给出别名、操作、绝对资源名、类型、类型身份及全部操作字段。缺项、未知键、非法数值/资源/类型/作用域均在 Guest 初始化前拒绝。

ROS2 Topic/TF/ParameterEvent 的八个 QoS 字段和 Action 五通道的40个 QoS 字段均显式配置并传给原生接口。Service call/advertise/respond、Parameter get/set/describe/list、Lifecycle get-state/change-state 共九操作要求完整八字段 qos.*；client/service 的两个实际请求/响应端点均比较八字段。respond 必须与独立授予的 advertise 契约一致，不授予创建服务权限；本地 declare/undeclare 拒绝不适用的 QoS 字段。

Bool规范载荷为1字节0/1，ROS1 int32为4字节有符号小端、ROS2 int64为8字节有符号小端，Float64为8字节有限binary64。数值类型必须显式 parameter-min-value/max-value 且min≤max；String/Bool拒绝这些字段。最小参数字节额度为Bool1/int32 4/int64或Float64 8/String envelope4；CLI590覆盖85项标量契约与六种set作用域42项signed参数扫描及缺项/后续option/unsigned quota拒绝。

typed事件规范wire为u32 sec/u32 nsec/u8 kind/u8 version=2/u8 tag（Bool1/int64 2/Float64 3），随后node/name/scalar三个u32长度字段，固定头23字节。删除payload长度0且保留tag；仅实际selected kind与范围内pre-delete值可交付，NOT_SET typed删除过滤。数值bounds inclusive；正max-parameter-bytes可小于完整envelope，max-event-fields计native三数组条目总数。CLI590=既有329+新增213数组契约与48六scope set检查；事件授权不由scalar get/set隐含。

数组wire为u32LE count加元素：Bool/byte1、ROS1 int32LE4、ROS2 int64/有限Float64LE8、String u32长度加UTF8。count0合法；必须显式给出数组专属max-parameter-elements（1..1048576），完整wire字节额度至少4，数值范围逐元素inclusive。String数组严格UTF8且无NUL；ROS1额外拒绝XML1.0非法控制字符及U+FFFE/U+FFFF，scalar String不变。数组get/set和ROS2本地declare/undeclare不隐含数组事件或metadata权限。CLI590=既有329+213数组契约+48六种set作用域检查。

## 7. Global/Single/Group 的语义

每个 ROS 版本独立选择目标。未绑定模块选择该版本 Global；Single 绑定准确的受信任已加载模块名；Group 只共享不可变策略描述。已选择 Single/Group 的模块不继承 Global grant。一个模块每个后端只能有一个显式绑定，冲突或未知模块拒绝启动。ROS1、ROS2 与 WASI 的同名 Group 不共享授权。完整语义与可执行例子见 [multimodule.md](multimodule.md)。

## 8. 多模块权限隔离实现

身份来自实际 Runtime Instance 指针、运行时 generation 与解释器 module ID。可信调用帧追踪普通跨模块导入及尾调用；跨模块代理路径不能使用被调用模块的 ROS 权限。Guest 没有可传入模块名、任意资源名或 Group 名来选择权限的 ABI 参数。每个 Instance/后端具有独立句柄表、quota、事件、审计和 IPC 会话。

## 9. Capability 安全设计

Handle 为 generation/slot 的64位值，绑定实际 Instance 与 ROS 版本；关闭、撤销、Trap/退出和会话失效会退役资源。操作、资源、类型、身份、限制在 Host 和 Gateway 两次验证。授权 lease 持有到 IPC 副作用/事件交付完成，撤销在已准入操作之后线性化；撤销后不能重新打开原别名。ROS1/ROS2 锁、状态和失败路径独立。

Service 请求与 Action Goal ID 使用进程随机64位、会话/资源/类型绑定、进程内不重复分配；旧 ID 在关闭再注册及实际 SIGKILL/重启后不能回答新请求。跨进程碰撞概率受64位随机空间约束，不声称数学上绝不碰撞。安全边界和受信任组件见 [security.md](security.md)。

## 10. ROS1/ROS2 Gateway 架构

UWVM2 继续使用 C++26，不链接 ROS 开发库。Noetic 的 roscpp Gateway 与 Jazzy 的 rclcpp Gateway 在各自官方支持的用户空间独立编译、独立执行。Gateway 保存原生 Publisher/Subscriber/Service/Action/TF 状态。原生回调只在 Gateway 内执行；Guest 事件使用校验后的自有副本和有界队列，TF 使用独立的过滤有界缓存，Action 回调还处理原生目标生命周期。回调不进入 Wasm、不保留 Guest 指针。双后端不会自动桥接、回退或共享权限。见 [architecture.md](architecture.md)。

## 11. IPC 协议设计

私有 Unix socket 验证 SO_PEERCRED：Worker 验证 Gateway UID，Gateway 验证准确 Worker UID/PID，并以 pidfd 跟踪该进程生存期。每条连接得到随机会话 ID；长度、版本、请求序号、超时、规范编码、帧上限均校验。第一次资源操作前必须完整 `session.bind`，与 Gateway 从受信任文件加载的不可变内存策略快照匹配：版本/enabled、网络身份和全部 grant/operation/resource/type/identity/limit 必须相同。文件本身为 owner 可写、group/other 不可写的普通文件；scope/name 和 Worker 本地 socket/UID/timeout/拒绝审计字段不进入网络 manifest。错误 UID/PID、未绑定、错误 manifest、伪造 session 和超长/部分帧均拒绝。协议见 [ipc.md](ipc.md)。

## 12. 消息类型验证机制

Guest 传入规范小端数据而非任意 CDR。受控复制检查 memory index、64位 offset/length 的溢出与边界，复制后校验结构、UTF-8、精确长度、Bool0/1、有限浮点、显式 Twist/TF 界限、Image 维度/stride/data、时间戳和 quaternion，再构造官方生成的原生对象。ROS1 MD5 与 ROS2 RIHS 类型身份必须与受信任适配匹配；未知自定义类型配置在 Guest 启动前拒绝（实际产品退出126）；ABI95用于已准入但不支持的操作变体，例如远端 ROS2 declare/undeclare。Memory growth 后重新解析当前内存，不保留 Guest 指针。

## 13. ROS1 通信测试结果

**PASS**。N13/W70/R243 实际使用 Noetic Master、roscpp 官方生成类型和独立原生节点：String/Bool/Twist/Image 的 Wasm→原生与原生→Wasm 逐字段/字节检查；Trigger/SetBool 的 client/server；Fibonacci Actionlib 的目标、完整 feedback/result、取消范围与所有权；string 参数 get/set/delete/exists/search/list；过滤 Graph、ROS/wall 时间、原生 Log、TransformStamped lookup/broadcast。所有测试同时核对 Guest 结果与原生 ROS 可观察行为，不以进程退出0代替内容验证。TF cap120ms 在 caller2000ms 的原生等待中仍生效。具体初始类型和权限范围见第3、6项。

R243 = 44项既有资源用例 + 49项 typed 标量参数 Wasm 用例 + 29项 ROS2 typed事件 Wasm 用例 + 121项数组 Wasm 用例。标量49项包含ROS1 19项、ROS2 30项。ROS1 Bool false→true、int32 -7→-123456789、Float64 -0.5→1.25 roundtrip以及三种缺失参数创建均有独立原生读取。拒绝后ROS1仅在5秒窗口采样 exact type/value 不变；未统计Master setParam调用次数，不能证明没有瞬时改变再恢复。失败get会保留length和全部64字节output哨兵；成功空mutation不覆盖output。

## 14. ROS2 通信测试结果

**PASS**。N13/W70/R243 实际使用 Jazzy/FastDDS、官方生成类型和独立原生节点，完成对应 Topic/Service/Fibonacci Action/TF/Graph/Time/Log 操作，增加 string 参数描述、Gateway 本地 declare/undeclare、精确节点/参数/值的 ParameterEvent 及 Lifecycle get-state/change-state。Topic/TF/ParameterEvent 的八个 QoS 字段与实际配置比较；Action 五通道40字段传入 rcl_action options，选定配置互通通过，完整有效值 getter 限制见第22项。

N13 的原生资源、Service 与 Lifecycle 套件验证 Service/Parameter/Lifecycle 的八字段显式 QoS：非默认 client depth6、Service advertise/respond depth7 生效；Gateway 自有 request publisher/response subscription 与 request subscription/response publisher 的实际 QoS 均核对。不兼容 best-effort 请求返回110，原生 Service callback 为0、参数值和 Lifecycle 状态不变。CLI590 覆盖九操作逐字段缺项拒绝、respond 契约一致/不一致以及本地 declare/undeclare 不适用字段拒绝。参数 get/set/describe/list 指向 Gateway 本地节点时仍要求字段，但不创建 DDS client。端点创建与实际 QoS 检查耗时计入操作 deadline；外部节点 QoS 和全部 RMW 排列未穷举。

N13 的 SROS2 单独采用真实签名 enclave 与强制 DDS 权限，成功双向通信，并验证被 DDS 签名策略拒绝的原生 Publisher 没有消息。普通 disabled-test 网络结果不代替此项；临时私钥由 runner 在 finally 中销毁，交付包不含密钥。

ROS2 非String标量远端写要求GetParameters实际kind一致、DescribeParameters dynamic_typing=false，再在同一有界deadline内SetParameters。该检查不是CAS，不能保证外部并发变化下的原子型别锁定。Wasm int64 9007199254740993按8字节精确比较；动态/不存在远端写95，wrong-kind95，越界13。三种Wasm缺失参数用custom NOT_SET witness：Guest get2、generated SetParameters callback0、无关既有值/类型不变；官方返回empty values缺失分支由direct-native检测修复及验证，不能把custom场景声称成官方分支。8项Gateway本地typed用例通过独立getcap核对本地state，没有external witness；typed mutation在有界本地锁内检查resident kind和bounds。窄undeclare拒绝13后独立get证明旧值，显式set将值调整至允许范围后，同一undeclarealias成功再get2。

新增49项Wasm用例中的 malformed 测试含Bool2、integer截断/过长、Float64 NaN/±Inf，错误22；显式grant64字节额度下9字节excess integer尚在额度内，bytequota90由direct-native及其他套件验证。49项标量get/set Wasm中的Float64只测-0.5/1.25；标量signed extrema、nextafter、-0、minnormal和largefinite精确roundtrip仅由direct-native与SDK验证；新增typed事件Wasm的MIN/MAX/nextafter/-0另有下述证据。数组get/set与ROS2本地declare/undeclare现受显式支持，metadata及String事件保持原适配；数组事件仍不支持；新增typed事件的证据范围单列如下。

R243新增29项typed事件Wasm：Bool10、int64 9、Float64 10。官方生成ParameterEvent受控publisher证明new/changed/deleted的完整wire；Bool false/true、int64 MIN/9007199254740993/MAX、Float64 -0与nextafter(1,2)按全部字节精确比较；typed删除携带selected tag与空payload，native NOT_SET删除过滤。拒绝wrong-kind/node/name、malformed nsec、native条目field预算、数值inclusive bounds、NaN/±Inf及完整envelope bytequota后，poll11保持length和全部4096字节output哨兵，并仍能接收后续合法marker。tiny quota1与充分额度marker使用不同精确参数名的两个独立cap，publisher等待实际两个匹配订阅；不把这种有界marker测试声称成任意ROS流的无损交付。permission/no-cap两项Guest13，原生writer连续5秒相对baseline的新增授权订阅数0；撤销旧handle9。每项在ROS启动前编译WAT，记录命令/source稳定与实际执行Guest snapshot SHA；只有native witness实际exit0及日志SHA/完成标记核对后才记PASS。[49项标量字节绑定](final-v8-scalar-fixtures.json)、[29项事件字节绑定](final-v8-event-fixtures.json)进一步核对实际remote编译输出/Guest快照、local下载Wasm与冻结WAT副本，不把仅元数据相等当实际复制证明。

历史v7首次focused Native真实报告为ROS1资源PASS、ROS2资源FAIL：原生测试夹具的本地declare/undeclare镜像错误插入qos.depth，Gateway在资源创建前正确返回unknown/inapplicable field；local get/set仍按既有Schema保留其显式QoS契约。修正仅涉及两个测试头的配置镜像，不改变生产权限规则、冻结四core产品、CLI590或U62 sanitizer结果；首个FAIL及日志保留，此次v8 GW18采用新恢复环境重新构建的真实provenance，不把旧失败改写为PASS。

历史v6两次focused Native真实FAIL记录（`reports/final-v6-native-resources`、`reports/final-v6-native-resources-final`）保留：初次DDS绝对reader数量断言错误，另次close0后projection GID仍存在。官方Node与Gateway相同NodeOptions各自已有框架Reader。最终测试按节点FQN/GID捕获baseline，只将新增授权Reader和关闭后该GID消失计为效果。29typed用例在实际BASELINE_READY日志标记（deadline8秒）后才恢复Guest；byte场景核对两个新增订阅，permission/no-cap证明没有新增授权订阅，而非全域零Reader。完整端点八字段均打印；图接口history=UNKNOWN/depth0时不能据此保证graph八字段全等，原有Gateway自有QoS检查范围另列。绝对reader观察修正仅增强FQN/GID baseline与起步barrier；close0却GID保留则暴露Jazzy idle waitset的shared_subscriptions_强引用，Gateway使用public notify唤醒executor以释放关闭订阅。最终实际关闭确认新增GIDs消失、原baseline保留；不改冻结UWVM core产品，也不把两个真实FAIL转成历史PASS。

N13另用官方Node set_parameter自动发布路径验证三种typed **changed**事件及实际原生value/kind，动态native timestamp经校验；没有声称自动declare/delete事件已覆盖new/deleted。new/changed/deleted精确payload来自上述manual generated消息。9组typed队列合同为三种kind×三种overflow-policy，20条/depth2/两完整envelope预算，保留精确首末2条后11；它们是native→认证IPC检查，不是29项之外的Wasm用例。

W70 ROS2 Action client使用测试专用publicAPI辅助generated Fibonacci::Impl::FeedbackMessage publisher（真实action/_action/feedback、accepted UUID、reliable/volatile/depth10）。实际writer subscription_count>0及Gateway send-goal success audit触发private ACK后才单次发布exact feedback；普通rclcpp_action server处理Goal/Result。N13 direct-native仍用普通ServerGoalHandle多反馈。这个有界起步条件不证明原early/late feedback竞态已修复。

R243新增121项数组Wasm：ROS1 49、ROS2 72。每种支持类型分别验证空→多值与多值→空，完整wire与成功输出尾区按字节比较；Bool false/true与byte[0,255]、signed extrema与int64 9007199254740993、Float64 -0/nextafter(1,2)/1.25、String空元素/Unicode/换行/`&<>`均有实际原生读取。malformed count/width/trailing、Bool2、非finite与String NUL/非法UTF8返回22；ROS1额外XML非法字符拒绝22；显式count/bytesquota90、数值逐元素bounds13、operation-denied13与wrong-native-kind95均检查全部4096字节输出和length哨兵。get native count/bytes/range/nonfinite以及ROS1 heterogeneous array拒绝，不复制敏感数据。只在完整shape/element预检后分配SDK结果；74项SDK数组互操作/拒绝检查另由U62 sanitizer验证。

ROS1数组missing写可创建，既有原生数组逐元素type检查；native空XmlRpc数组不携带元素type，按显式cap种类解释，不声称Master保留空数组element identity。ROS1拒绝观察只在5秒窗口采样native kind/value，不统计Master setParam次数，不能证明没有瞬时写入后恢复。ROS2数组远端写要求static-existing且Get/Describe实际kind与dynamic_typing检查、deadline受限，仍不是CAS。dynamic/absent95，custom NOT_SET absent get2、generated SetParameters请求0、无关既有数组不变。成功原生观察要求恰好1个generated setter与至少3个getter（initial/preflight/final），拒绝setter0。12项本地declare/undeclare由独立getcap核对Gateway本地state，无external witness；kind或bounds不符拒绝且保留旧数组，窄bounds经显式set调整后同一undeclarealias才成功。

数组121项全在ROS启动前编译选中WAT；短物理array-12hex目录避免AF_UNIX路径溢出，actual executed Guest snapshot与source稳定SHA、native probe snapshot、完成exit0与日志SHA核对后才PASS。[121项数组字节绑定](final-v8-array-fixtures.json)与标量/事件proof同样绑定最终R243报告，remote原始编译输出、actual Guest snapshot及本地Wasm实际bytes；不把只生成夹具当执行。

## 15. ROS1+ROS2 同时运行测试结果

**PASS**。W70 的一个 hybrid Wasm 在同一 dual 产品/进程内分别访问两套独立 ABI，四类型双向通信由两侧原生节点验证。实际分别 SIGKILL ROS1、ROS2 Gateway 时，失败后端107及输出哨兵不变，另一后端继续精确发布；重启、新 PID/session.bind 后实际恢复通信。C4 双方向撤销一后端时，revoked 原生计数保持1，survivor 到3，并在 Worker 与存活 Gateway 仍运行时观察，无跨后端权限或锁传播。

## 16. 多模块测试结果

**PASS**。G5 为用户要求的一个完整五模块场景：camera_a、camera_b 共用 ROS1 legacy_sensors Group；controller 为 ROS1 Single；planner 为 ROS2 modern_planners Group；hybrid 同时为两个后端的独立 Single。实际 Instance ID 为1–5，hybrid 在两后端使用同一实际 Instance5。六个必需资源分别准确收到2条；两台 camera 另通过同一 quota 别名各成功2次、第三次11，原生共享 Topic 为2+2，Gateway 审计显示两个独立 session 各2次请求。

实际仍存活的外来非零 Handle 转交另一模块返回9；未授予别名/后端返回13，无额外消息。W70 普通与尾调用代理验证实际祖先权限，A33 验证 Single/Group 不继承 Global、后端与 WASI 的同名 Group 不互授。例子与完整配置见 [multimodule.md](multimodule.md) 和 [五模块自动测试](../../tests/ros_multimodule/README.md)。

## 17. 安全攻击、稳定性和性能测试结果

原始 [55类攻击矩阵](acceptance-matrix.md) 全部具有实际执行映射与明确的错误码/有界效果、原生副作用或披露观察：**55/55 scoped PASS**。其中原生 callback lifetime 是有界 teardown/晚回调压力，未把它写成原生 ASan 验证；并发授权使用真实 C++线程，未声称 Wasm线程导入。所有范围与未支持项见第22项。

### 最终证据索引

报告路径相对 `linux:/home/macromodel/.local/share/uwvm-ros-extension/20261012-recovery/`，均在证据包内；每份报告 SHA 列于 [final-evidence-index.json](final-evidence-index.json) 和55行矩阵。根 [final-acceptance.json](final-acceptance.json) 保存全部15套准确命令与嵌套证据 SHA。

| 标识 | 实际用例/套件数 | 状态 | 最终报告 |
|---|---:|---|---|
| N13 | 13 | PASS | `acceptance-final-v8-io-final/native/report.json` |
| W70 | 70 | PASS | `network/20261013-043328/report.json` |
| R243 | 243 | PASS | `rpc/20261013-041831/report.json` |
| G5 | 1 | PASS | `five-module/20261013-043647/report.json` |
| A33 | 33 | PASS | `additional-security/20261013-043656/report.json` |
| I16 | 16 | PASS | `ingress/20261013-043746/report.json` |
| C4 | 4 | PASS | `revoke-race/20261013-043849/report.json` |
| F6 | 6 | PASS | `network/20261013-043944/flood.json` |
| P18 | 18 | PASS | `network/20261013-044006/payload-security.json` |
| T24 | 24 | PASS | `network/20261013-044110/performance.json` |
另外：B4四独立产品、CLI590（既有329+213数组契约+48数组set六作用域）、REG36四产品回归（16次执行+20次夹具编译）、U62安全/IPC/SDK ASan+UBSan均通过。SAN记录400份规范化实际唯一输入与400个依赖token，不是安全测试数量。SP2为Noetic GCC9/Jazzy GCC13实际C++17编译并执行2项scalar/typed-event/array codec可移植检查（不含ROS网络），实际冻结4份输入。B100K授权开销、N13内实际QoS、SC63和BC4另行绑定。历史FAIL（含本轮Native夹具错误QoS镜像被正确拒绝，以及历史v6两次focused Native的absolute Reader观察失败及close0后GID保留）、测试观察增强前和修正前报告保留，最终索引不将它们转换为PASS。v5历史包括library-path启动错误、49个Wasm未预编译、两个bound-undeclare夹具重复grant被正确拒绝，以及Jazzy单次earlyfeedback竞态；前两项和夹具修正属于测试集成，ACK条件不等于修复上游feedback竞态。[清理复核](final-cleanup.json)记录测试结束后任务拥有的 Gateway/probe/Guest/ROS Master 进程及临时测试 mount 均为空；既有容器和 cgroup 保留。


### I/O语义差分与有界分配检查

IOG 独立离线测试实际PASS：5265个JSON字节转义用例、4份完整canonical policy输出、12119条浮点文本和63400条数组验证/输出哨兵/成功wire比较。常规浮点走fast_io，非零且绝对值不大于最小正规值时保留classic-stream冷路径；严格文法、finite与transactional结果保持。实际accept/reject与IEEE754位型差异0，双方接受10147条；errno差异1773条单独记录，errno不是这个解析函数的输出契约。4个实际探针/编译依赖、10次main-entry PID/cgroup证明、原始输出和820份冻结输入逐SHA核对。format/scan候选编译使用ASan+UBSan；隔离数组variant仅完整validate_parameter_value函数字节与当前源码相等，其他旧variant头不冒充当前组合，当前完整组合另由SAN验证。

12组allocation检查限定replacement global operator new计数、4096个元素、预分配输入/输出哨兵、仅validate调用：int64/Bool/byte成功且不请求decoded输出时从1次降为0；int64 bounds拒绝从1次降为0，Bool/byte拒绝原已0仍0。请求decoded成功结果时仍1次。这个结果不是整个分配器、ROS库、吞吐或延迟的测速。Worker使用concat_std、ostring_ref_std与直接预留JSON输出，审计记录减少中间字符串；固定LE、IEEE位型、UTF8和完整shape先验仍保留。

NIO 在Noetic GCC9和Jazzy GCC13各实际执行24项C++17/ASan+UBSan原生FD/config/audit检查，nested report、compiler/build/run log、当前project依赖与冻结副本均逐SHA核对；这是离线文件I/O范围，没有ROS网络或整套ROS库sanitizer声明。原生C++17保留bounded to_chars/raw POSIX兼容路径；显式确认4096/4097行边界、EOF/CRLF、O_NOFOLLOW/FD关闭/路径替换、EBADF/ENOSPC/EAGAIN/short-write及有界审计。未注入EINTR或fstat/fcntl故障，非Linux fallback与特权mount/device未覆盖。没有独立measured speedup，不声称更快比例。

NIO只有compiler-recorded project依赖要求当前字节相同；未编译candidate snapshot保留精确旧字节，不冒充完整当前ROS资源构建。每发行版实际12份未编译项目snapshot，其后发生变化的路径与旧/当前SHA在索引current_input_scope逐项列出；其中ROS2 resources后加的Bool/byte reserve由当前GW18/SAN和完整聚合覆盖，独立FD/config/audit24项没有编译这个header。原validator范围过宽的纯读取FAIL保留，修正只收窄对未编译snapshot的声明，不取消实际编译依赖或冻结字节校验。

IOFA另在同一真实Linux cgroup运行public json_quote、canonical_dump与capability_manager.record_result路径：只在实际操作内部计数replacement global operator new，构造输入和hex输出在计数区外；不是malloc/整体heap或速度比例。JSON三种输入各4096字节；dense policy导出是3种scope各20个synthetic capability的序列化压力，含故意无效的text字段以覆盖escaping，不执行这些grant。审计100条仅规范化time_unix_ns数字为0，剩余整份记录逐字节相同。实际2compiler+2main-entry PID/cgroup、全部输出/依赖、797份冻结输入及3个完整active header的当前SHA均核对；其余variant fixture context明确为历史冻结上下文，当前完整组合由SAN/GW18/A15验证。

| 实际Linux分配用例 | baseline operator new | candidate operator new | 完整相同输出字节 |
|---|---:|---:|---:|
| `json_quote-ascii` | 8 | 1 | 4098 |
| `json_quote-control` | 11 | 1 | 24578 |
| `json_quote-escaped` | 9 | 1 | 8194 |
| `canonical_dump-ros1` | 3153 | 17 | 119627 |
| `canonical_dump-ros2` | 5614 | 18 | 212867 |
| `capability_manager.record_result-100` | 1402 | 502 | 94200 |

每项完整输出SHA、原始hex字节与immutable dependency输入在IOFA/IOFA-INPUTS中；没有独立measured speedup，不声称更快比例。

### 24项有界性能工作负载

T24 使用独立 native emitter/observer 与实际被授权的 Wasm。每项先发送一条 all-w warmup（不计入数据），然后64或4096字节 String payload；普通发送100条/100Hz，两模块各100条（preload start 和主模块顺序执行），dual 交错向两后端各100条。高频项64/4096字节200条/目标1000Hz；队列深度32、burst32、权限校验始终开启，观察超时10秒。clock测1000次认证 IPC；官方 Trigger 测100次/concurrency4，使用可信原生 Worker 调用真实 Gateway。

以下 p50/p95 为微秒，Hz 为实际接收速率或 IPC/service 完成速率。Topic 的 N 为 received/sent，最后四项为 completed；它们是 paced workload 的实测，不能当作最大持续吞吐。多模块包含两段之间的等待。clock latency 包括认证、Unix transport、Gateway clock 与序列化，不能当作纯 IPC 开销。

| 工作负载 | payload B | N | dropped/errors | queue depth | p50/p95 µs | Hz |
|---|---:|---|---:|---:|---|---:|
| `ros1-64-native` | 64 | 100/100 | 0 | 32 | 244.96/317.11 | 98.38 |
| `ros1-64-wasm` | 64 | 100/100 | 0 | 32 | 466.84/526.79 | 98.58 |
| `ros1-64-multimodule` | 64 | 200/200 | 0 | 32 | 422.37/542.43 | 49.51 |
| `ros1-4096-native` | 4096 | 100/100 | 0 | 32 | 223.44/340.11 | 98.65 |
| `ros1-4096-wasm` | 4096 | 100/100 | 0 | 32 | 479.58/527.71 | 98.48 |
| `ros2-64-native` | 64 | 100/100 | 0 | 32 | 869.76/1190.80 | 97.99 |
| `ros2-64-wasm` | 64 | 100/100 | 0 | 32 | 833.96/1321.15 | 98.06 |
| `ros2-64-multimodule` | 64 | 200/200 | 0 | 32 | 828.63/1323.60 | 49.44 |
| `ros2-4096-native` | 4096 | 100/100 | 0 | 32 | 702.78/1181.02 | 97.79 |
| `ros2-4096-wasm` | 4096 | 100/100 | 0 | 32 | 745.10/1269.91 | 98.33 |
| `dual-64-ros1` | 64 | 100/100 | 0 | 32 | 441.74/513.29 | 97.04 |
| `dual-64-ros2` | 64 | 100/100 | 0 | 32 | 905.51/1451.54 | 97.02 |
| `dual-4096-ros1` | 4096 | 100/100 | 0 | 32 | 459.01/553.87 | 97.25 |
| `dual-4096-ros2` | 4096 | 100/100 | 0 | 32 | 949.28/1392.17 | 97.24 |
| `dual-64-high-frequency-ros1` | 64 | 200/200 | 0 | 32 | 439.51/541.07 | 750.33 |
| `dual-64-high-frequency-ros2` | 64 | 200/200 | 0 | 32 | 892.04/1491.91 | 751.21 |
| `dual-4096-high-frequency-ros1` | 4096 | 200/200 | 0 | 32 | 416.40/517.09 | 781.76 |
| `dual-4096-high-frequency-ros2` | 4096 | 200/200 | 0 | 32 | 1059.27/2278.18 | 779.67 |
| `ros1-high-frequency` | 64 | 200/200 | 0 | 32 | 415.89/481.36 | 881.93 |
| `ros2-high-frequency` | 64 | 200/200 | 0 | 32 | 3023.09/7351.86 | 903.45 |
| `ros1 clock IPC inclusive` | 0→≤1024 | 1000 | 0 | 1 | 12.74/13.29 | 74507.10 |
| `ros1 Trigger concurrency4` | 0→≤1024 | 100 | 0 | 1 | 6158.11/6780.28 | 624.11 |
| `ros2 clock IPC inclusive` | 0→≤1024 | 1000 | 0 | 1 | 12.54/14.44 | 70075.70 |
| `ros2 Trigger concurrency4` | 0→≤1024 | 100 | 0 | 1 | 5888.78/87123.80 | 180.06 |

Topic20项均无实测丢包，clock/Service4项均无错误。逐项实测 p50/p95 和接收速率见上表；这些值包括当前工作负载、调度和 Gateway，不隔离为授权检查成本，也不当作最大持续吞吐。权限隔离在所有工作负载中保持开启。

以下 CPU% 为该报告进程 CPU_seconds/elapsed_seconds×100（1核100%）；前20项是原生 observer，后4项是可信 IPC Worker。RSS为这些进程的实际峰值，CS为自愿/非自愿 context switches，不能当整个 Gateway+Guest 路径总消耗。

| 工作负载 | elapsed s | CPU s / 1核% | peak RSS MiB | CS voluntary/involuntary |
|---|---:|---|---:|---|
| `ros1-64-native` | 1.439 | 0.0128/0.89% | 10.57 | 381/0 |
| `ros1-64-wasm` | 3.005 | 0.0232/0.77% | 10.56 | 597/0 |
| `ros1-64-multimodule` | 6.006 | 0.0397/0.66% | 10.40 | 1189/1 |
| `ros1-4096-native` | 1.412 | 0.0113/0.80% | 10.82 | 378/0 |
| `ros1-4096-wasm` | 3.006 | 0.0240/0.80% | 10.62 | 595/0 |
| `ros2-64-native` | 1.270 | 0.0619/4.87% | 26.59 | 2338/1 |
| `ros2-64-wasm` | 2.948 | 0.1282/4.35% | 26.06 | 5269/1 |
| `ros2-64-multimodule` | 5.958 | 0.2891/4.85% | 26.26 | 10513/4 |
| `ros2-4096-native` | 1.245 | 0.0710/5.70% | 26.29 | 2277/2 |
| `ros2-4096-wasm` | 2.929 | 0.1305/4.46% | 25.99 | 5223/2 |
| `dual-64-ros1` | 3.279 | 0.0217/0.66% | 10.57 | 636/0 |
| `dual-64-ros2` | 3.051 | 0.1443/4.73% | 26.08 | 5426/1 |
| `dual-4096-ros1` | 3.278 | 0.0223/0.68% | 10.57 | 635/0 |
| `dual-4096-ros2` | 3.036 | 0.1361/4.48% | 26.50 | 5409/1 |
| `dual-64-high-frequency-ros1` | 2.545 | 0.0234/0.92% | 10.57 | 749/0 |
| `dual-64-high-frequency-ros2` | 2.302 | 0.1268/5.51% | 26.33 | 4122/2 |
| `dual-4096-high-frequency-ros1` | 2.537 | 0.0224/0.88% | 10.57 | 749/0 |
| `dual-4096-high-frequency-ros2` | 2.293 | 0.1127/4.92% | 26.32 | 4117/0 |
| `ros1-high-frequency` | 2.208 | 0.0278/1.26% | 10.65 | 702/0 |
| `ros2-high-frequency` | 2.191 | 0.0981/4.48% | 26.21 | 3962/0 |
| `ros1 clock IPC inclusive` | 0.013 | 0.0061/45.43% | 10.36 | 1000/0 |
| `ros1 Trigger concurrency4` | 0.160 | 0.0403/25.17% | 10.36 | 801/2 |
| `ros2 clock IPC inclusive` | 0.014 | 0.0061/42.58% | 25.64 | 1000/0 |
| `ros2 Trigger concurrency4` | 0.555 | 0.0495/8.91% | 25.89 | 1951/1 |

此外，10ms采样器追踪实际 `/proc` 后代树（Ubuntu chroot wrapper和真正执行子 PID均记录），覆盖 startup/Guest wait/Gateway/ROS Master；单一被采样进程组 RSS停止预算512MiB，非 cgroup 放宽。采样值可漏掉采样间隔内的短命进程，逐 PID命令、成员、CPU、RSS、context switch均在T24 `process_metrics`，下表是同角色跨多次测试的采样汇总，CPU和CS为各测试进程树的累计值，RSS为各组最大同时RSS。

| 角色 | 测量进程组 | 最大组 RSS MiB | 累计采样 CPU s | 累计采样 CS voluntary/involuntary |
|---|---:|---:|---:|---|
| ROS Master | 1 | 102.15 | 0.65 | 1929/23 |
| Native observer | 20 | 34.64 | 2.46 | 49533/67 |
| Native emitter | 4 | 34.95 | 0.24 | 821/18 |
| UWVM Worker | 12 | 7.25 | 38.69 | 2498/209 |
| ROS Gateway | 16 | 35.51 | 0.96 | 1041/53 |
| IPC/service Worker tree | 2 | 63.59 | 0.37 | 2188/7 |

B100K：100,000次 Manager authorize，总39,960,173ns，整数平均399ns，峰值16,449,536B；address-space预算67,108,864B，编译器记录397项依赖token。

历史验收归档副本在 `reports/acceptance-history-v8/manifest.json`：2个范围、112份文件、4576695字节，逐文件SHA/长度与原始路径映射核验；首轮实际aggregate仍为FAIL，第二轮停止的未完成运行没有伪造完成aggregate。原目录路径仅标识实际历史位置，归档读取使用 `reports/acceptance-history-v8/` 的exactbytes副本。

历史v7的完整15套验收因远端主机重启中断，不能记为完整PASS；此次v8使用persistent工作区恢复环境及重新冻结的当前源码/产物独立验证。I/O改写保留固定LE/浮点位型、严格UTF8和完整shape先验；Worker格式使用fast_io reserve/string_ref/concat，C++17原生网关使用兼容的bounded转换和经过检查的文件I/O。未将静态减少临时对象数量声称为实际吞吐提升。

历史v8首次完整聚合因persistent工作区Unix socket路径超107字节而被正确启动拒绝，另有重启后未恢复编译的多模块夹具；失败报告在acceptance-final-v8-io-before-short-workspace保留。环境修复采用既有persistent目录到短绝对路径/u8的bind mount，并重新编译缺失夹具，不修改生产guard或把首个FAIL计为PASS。WS-ALIAS实际stat device/inode一致与mountinfo绑定该精确路径；/u8只是已证明的同一目录，不允许任意路径别名。

第二轮短路径使用普通bind但没有带入rootfs的/work子挂载，实际native12/13通过后SROS2生成缺/work；运行250.413秒后主动SIGTERM停止不完整聚合，报告与清理证明在reports/v8-short-workspace-pre-submount-stop.json保留，不能记完整PASS。再次仅修复环境为recursive bind，WS-ALIAS另外验证Noetic/Jazzy两个/work子挂载都指向同一当前source；生产和测试入口源码没有为此变更。

B100K 独立授权微基准的实际数字由下面的最终报告字段读取；实际子进程 cgroup 已核对，`network_test=false`。其编译依赖逐项保留 SHA 和不可变输入副本。原生 event queue flood 为20条/depth2/128B、三种溢出策略准确首末2条/随后11；100次 Publisher/Service/Action/Log/Graph flood 分别证明 burst2/98次11及实际原生有界计数。它们与上述无丢包 paced Topic 工作负载分开报告。

## 18. SSH Linux 环境信息

实际目标为 `ssh linux`，主机`macromodel-ThinkBook-16p-G5-IRX`，架构`x86_64`，内核`6.17.0-29-generic`，32逻辑CPU；OS记录`PRETTY_NAME="Ubuntu 25.10"`；MemTotal:       96207992 kB。UWVM 为 LLVM23 Clang C++26/libc++，解释器产品禁用 JIT；Noetic 在 Focal/GCC9，Jazzy 在 Noble/GCC13 用户空间构建，均执行在既有容器内。环境、toolchain完整版本、输入依赖和实际命令保存在 `build/environment-*.json`、各产品 `commands.json` 和 GW18 provenance。

## 19. 64 GiB Cgroup 验证结果

**PASS**：复用实际既有64GiB cgroup，未放宽限制；有效路径`/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope`，CPU集合`0-31`、cpu.max=`max 100000`、pids.max=`112500`。runner与实际编译/原生/Gateway/Guest后代 PID均有成员记录；chroot父进程不代替子进程证明。

| 指标（字节） | 最终聚合前 | 最终聚合后 |
|---|---:|---:|
| memory.max | 68719476736 | 68719476736 |
| memory.current | 18323357696 | 22534397952 |
| memory.peak | 35375910912 | 35375910912 |

memory.events before=`{'low': 0, 'high': 0, 'max': 0, 'oom': 0, 'oom_kill': 0, 'oom_group_kill': 0}`；after=`{'low': 0, 'high': 0, 'max': 0, 'oom': 0, 'oom_kill': 0, 'oom_group_kill': 0}`；oom/oom_kill/oom_group_kill增量均0。

前后memory.events数值及OOM增量由最终证据核对；共享cgroup累计峰值不归为本任务独占峰值，memory.current变化也含其他既有任务和文件缓存。编译最多2并行并保留16GiB headroom，性能按上文512MiB RSS停止预算运行，无无限制压力。

## 20. 编译和运行命令

完整四组合与 Gateway 构建见 [build.md](build.md)。在报告记录的既有容器/cgroup内，产品构建示例（使用新的输出目录；完整实际命令在B4 commands.json）：

```sh

ssh linux 'docker exec "$PROVIDED_CONTAINER_ID" chroot /host /usr/bin/env CXX=/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang++ LD_LIBRARY_PATH=/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib:/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu /usr/bin/python3 /home/macromodel/.local/share/uwvm-ros-extension/20261012-recovery/source/tests/ros/build_product.py --profile all --out /home/macromodel/.local/share/uwvm-ros-extension/20261012-recovery/fresh-products'

```

**PASS 四独立最终v8产品**。各编译 Runtime/main/HostAPI三个翻译单元、链接并运行 version。`product-final-v8-io/results.json`、`source-manifest.json` 和各产品 `commands.json` 保留实际输入/命令：

| 产品 | 秒 | 二进制 SHA-256 |
|---|---:|---|
| none | 67.820 | `f9d26ed7a681fecc2bdb38f5c3fc3c37f117a59eba8939f91dab733a29050937` |
| ros1 | 81.683 | `779290dc31ea11f76981aa2cbeff1e29b50c0e44b470996dbee6068b2ca7bbec` |
| ros2 | 77.618 | `f1e6e614291b31dab6c009fdd190bac97574807e0935deb4b619fc3b54654848` |
| dual | 74.527 | `38b83fa9d79f2eeae32b2bb105bb30abc22ba3f8edac3c18333ea28ba6ac58c6` |

最终 Gateway 二进制：

| Gateway | SHA-256 |
|---|---|
| ROS1 | `da479179cff47e9885f4d2ca40a03d7b9ee48bb3dcc9f2cc5833c78823335b20` |
| ROS2 | `3bc069c6cec8429b0b210aa7705c6a5c2f1e0fe684554ad172f2beb950fd86fa` |

GW18 `gateway/final-v8-io-provenance/gateway-frozen-manifest.json`：73份实际源码/CMake输入、18套dependency、18个二进制，build-report耗时173.266秒。

IOG `reports/io-golden-v8-final/report.json`实际4个C++26探针构建和10次调用，准确命令保存在builds/runtime_probes，输出、MMD依赖及kernel PID/cgroup记录保留；NIO `reports/native-io-v8-golden/report.json`实际两发行版命令及24项各自执行日志由nested report绑定。IOG frozen-inputs和NIO两份frozen-inputs分别记录820、25、25项真实源输入；candidate/vendor/current与历史隔离variant的绑定范围如第17项。这些是本次实际离线检查，不能替代后续15套网络聚合。

IOFA `reports/io-golden-v8-final/format-allocation/report.json`实际两次C++26构建/两次运行，6个public formatter/audit allocation用例；记录exact command、4个真实PID/cgroup、full outputs与797份冻结输入，不是模拟或仅静态计数。

GW18 的最终I/O/array编译器依赖、源码/CMake输入及原生程序二进制SHA均由最终provenance读取；历史失败和旧产物只作为历史证据保留。

在报告记录的既有容器/cgroup内，统一验收示例（实际逐套命令在final-acceptance.json）：

```sh

ssh linux 'docker exec "$PROVIDED_CONTAINER_ID" chroot /host /usr/bin/env CXX=/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang++ CC=/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang ROS_TEST_CXXFLAGS="-std=c++26 -O1 -g0 -pthread -stdlib=libc++" LD_LIBRARY_PATH=/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib:/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu /usr/bin/python3 /u8/source/tests/ros/run_all.py --uwvm /u8/product-final-v8-io/dual/uwvm --out /u8/acceptance-final-v8-io-final --workspace /u8'

```

在准备好相同 ROS环境/工具/固定产物的既有 cgroup 内，选用新的 `--out` 目录重跑。新的机器需先按 [testing.md](testing.md) 准备依赖与实际 cgroup；缺少条件明确 BLOCKED，不能从本次证据推断它自动可运行。

本次实际WS-ALIAS报告记录persistent目录recursive bind mount到/u8（非symlink；Path.resolve不会展开bind mount），mountinfo的root/mount point和两端st_dev/st_ino一致，并保留两个rootfs的/work与proc/sys/dev子挂载。复现时先在有权的既有部署环境创建空/u8挂载点，使用mount --rbind把persistent工作区挂在那里并核验/proc/self/mountinfo、两个/work源和stat；不要覆盖已有不相关挂载。生成式socket的107字节限制仍生效，短物理输出目录必须保留。实际命令和该alias证明均在证据包，未把缺挂载机器当自动可运行。

## 21. 可复现的自动化测试入口

[tests/test_ros_all.sh](../../tests/test_ros_all.sh) 调用 [run_all.py](../../tests/ros/run_all.py)，先验证有效64GiB cgroup，顺序运行全部实际原生/Wasm/拒绝/性能套件，最终核对 OOM差值和嵌套报告状态/SHA。各套件有超时与 SIGTERM清理；缺少 rootfs、工具或有效成员返回 BLOCKED。独立入口和临时密钥清理见 [testing.md](testing.md)。最终真实聚合 **15/15 PASS**，不是只生成了测试入口。

REG36 在none/ROS1/ROS2/dual四个最终v8产品执行 WASI args/fd_write、memory64、解释器尾调用和仓库已有 `examples/0002.dl` TrustedDL回归：**36/36检查，16次真实执行+20次夹具编译**，报告 `reports/final-v8-regression-{none,ros1,ros2,dual}/results.json`。它是已执行的回归子集；未声称全部历史测试和平台运行。CLI590实际验证help、所有受检缺项/非法绑定，以及 enable→disable最终关闭成功、另一后端仍启用时保持严格入口限制。GitHub工作流已添加，本次未声称托管CI运行。

BC4 另用三个当前产品执行四项拒绝检查，真实验证 none 拒绝 ROS1/ROS2、ROS1-only 拒绝 ROS2、ROS2-only 拒绝 ROS1 参数（均退出126）。测试入口把 LLVM library 环境传给全部子测试，向 CLI 转发指定 wat2wasm，撤销测试支持 CXX 包装器并拒绝不会执行任何用例的参数组合。聚合要求非空证据列表且每份报告明确 PASS；早期两项 CLI 诊断文字断言失败记录保留，当前590项与此次15套聚合重新通过。

## 22. 尚未解决的技术限制

| 状态 | 限制/未验证范围 |
|---|---|
| **UNSUPPORTED** | 开启 ROS 的 LLVM native backend、C++ named-module构建和非 Linux Gateway。无法安全认证的调用链在 Guest启动前拒绝。 |
| **UNSUPPORTED** | 任意自定义 Message/Service/Action 动态 Schema；当前只有第3、4项列出的官方生成初始适配。未知配置在启动前拒绝。 |
| **UNSUPPORTED** | 参数数组事件、ROS1 int64/byte数组与ROS2 int32数组、ROS1 typed ParameterEvents、除Bool/int64/Float64外的typed事件、远端 ROS2 declare/undeclare、负 ROS2时间戳、QoS keep-all和队列backpressure。 |
| **UNSUPPORTED** | 在 roscpp/RMW原生反序列化前对不可信 ROS输入施加本扩展的逐消息内存硬上限。桥接复制/IPC/队列有界，原生库瞬时分配属于TCB；64GiB cgroup不等于单条消息安全硬限。 |
| **UNSUPPORTED（未执行的验证）** | Action40字段应用与所选配置互通通过，但 Jazzy公开接口不能查询所有五通道的最终有效QoS，任意profile有效值完全相等未验证。普通 Service/Parameter/Lifecycle 九操作的八字段显式配置及实际端点比较已实现，仍未穷举所有 QoS 排列。 |
| **UNSUPPORTED（未修复的上游限制）** | Jazzy官方Action client在Goal-response注册UUID前或终态后可能忽略feedback；最终W70采用上述ACK起步条件，未声称无条件lossless feedback。 |
| **UNSUPPORTED** | 当前测试产品的 Wasm线程导入；授权竞争由实际原生 C++线程调用 Manager/IPC完成。 |
| **UNSUPPORTED（未执行的验证）** | 原生 Gateway/ROS库整体 ASan/UBSan验证、任意QoS排列、全部仓库历史/平台测试、无限持续负载。已执行的是原生 callback/Action生命周期有界压力，独立 Manager/codec/IPC/SDK sanitizer与本报告回归子集。 |

部署TCB包括 Worker/Gateway/native ROS库、管理员的文件/网络隔离和可信 native plugins。严格ROS启动guard在Guest执行前拒绝混用TrustedDL，但不sandbox或回滚已经运行的原生initializer。超时/关闭会话也不能保证回滚外部ROS节点已准入的请求。ROS1 cancel-any ID0的显式权限覆盖该Action资源全部原生Goal（含外部client）；ROS2 cancel-any只覆盖Gateway registry中的Goal。原生授权拒绝/有界观察PASS不能扩展为对以上范围的保证。

## 23. 最终验收清单与交付文件

[原请求50项清单](final-checklist.md)逐字保留验收项，**50项 scoped PASS**，各行注明证据和第22项限制；[55类攻击](acceptance-matrix.md)逐条记录真实拒绝/有界效果与原生观察。实现、文档、SDK、示例、自动测试已留在当前工作区，逐文件修改/新增分类及SHA见 [task-files.json](task-files.json)，仓库既有无关修改未重置。

[证据包](evidence-20261012-v8.tgz)包含9682份报告、日志、显式测试策略及冻结的编译输入/元数据，压缩22335598字节，SHA-256 `fe8b37f3bb67c05cd0eff420f39214f7ffe7a46ae19ca19da39c9a612c88e114`；[导出元数据](evidence-20261012-v8.tgz.json)。生成前核验归档SHA与长度；归档条目/私钥审查范围以导出元数据和最终QA为准。二进制准确SHA与远端路径在报告/provenance中，源码与构建命令可复现它们。
