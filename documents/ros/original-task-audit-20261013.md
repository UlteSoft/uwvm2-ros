# 对照最早任务的完整完成度审计（2026-10-13）

**结论：原任务尚未完整完成。** 核心 ROS1/ROS2 扩展已实现，并有真实 Noetic/Jazzy、Guest/native、多模块、双后端和权限拒绝证据；原文规定的部分功能、故障测试、性能指标、示例资料和最终当前版本统一验收仍未齐备。不能把“50项 scoped PASS”“55类有界验证”写成原任务全范围完成。

本审计完整覆盖原文35章、50项最终验收、55类攻击、20个示例、12阶段、23项输出，并区分后续“默认仅基础stdio”要求。[机器可读全表](original-task-audit-20261013.json)保存原项、实现、证据、缺口和独立审查输入；[原始任务快照](original-task-audit-20261013-evidence/original-task.txt)的SHA256为 `6d117bd8a05d6efee23d0b2ac10409c6684a4c1baffec798b0f0255e051f2a9f`。

本次只读核对源码、旧报告和归档；另外通过只读SSH取回4份现存原始子报告并匹配SHA。没有改生产代码、没有执行新构建/ROS测试/CI，未解决下列缺口。此前66份正式交付文件的实际SHA/字节数保持。

“已实现且有真实验收”指明确支持类型/平台/冻结版本中的真实执行，源码存在不等于通过；“部分”明确列缺项；“未找到实证”不当作FAIL或伪造PASS。公开的安全拒绝不等于功能已实现。这里不计算完成百分比，也不新增无限负载、所有未来攻击或任意QoS排列的证明义务。

## 已完成的主体

| 主体 | 实际完成范围 | 主要证据 |
|---|---|---|
| 架构/构建 | 单一C++26 UWVM无ROS开发库；Noetic/Focal与Jazzy/Noble独立C++17 Gateway；none/ros1/ros2/dual真构建 | [build.md](build.md)；[contracts-20261013-report.md](contracts-20261013-report.md)；[deadlines-20261013-report.md](deadlines-20261013-report.md) |
| 权限/CLI | ROS默认禁用、空授权；53/59注册参数；Global/Single/Group独立且不继承；无Level/Role/Preset；缺项拒绝 | [capabilities.md](capabilities.md)；[resources-20261013-report.md](resources-20261013-report.md) |
| 身份/Handle/IPC | 真实Instance/generation、普通/尾调用祖先、跨Instance/跨ROS拒绝；SO_PEERCRED、随机session、完整manifest、长度/超时检查 | [architecture.md](architecture.md)；[ipc.md](ipc.md)；W70/G5/I16/C4 |
| 初始消息与业务 | 两版String/Bool/Twist/Image、Trigger/SetBool、Fibonacci、TF/Graph/Time/Log；ROS2 Lifecycle；真实Guest/native双向 | [test-report.md](test-report.md)；[events-20261013-report.md](events-20261013-report.md)及原报告 |
| 参数/事件 | 支持投影的标量/数组；ROS2九类typed事件及独立删除通知；静态Bool首次初始化修复 | [events-20261013-report.md](events-20261013-report.md)；[contracts-20261013-report.md](contracts-20261013-report.md)；[resources-20261013-report.md](resources-20261013-report.md) |
| 多模块/双ROS | 五个指定模块；双向hybrid；只有一边权限时拒绝另一边；Gateway崩溃、重启与撤销隔离 | G5/W70原报告；[multimodule.md](multimodule.md) |
| 资源与安全测试 | 速率/字节/数量/三种队列溢出；Trap清理；真实拒绝和native观察；最新Action7生命周期+8限额 | [acceptance-matrix.md](acceptance-matrix.md)；[resources-20261013-report.md](resources-20261013-report.md) |
| 环境/工具/资料 | 既有64GiB、实际PID/ELF/cgroup、有限负载/OOM/清理；SDK C/C++、文档、自动脚本、CI workflow | [testing.md](testing.md)；[test-report.md](test-report.md)；[resources-20261013-validation.json](resources-20261013-validation.json) |

## 明确未完成、部分完成与后续要求

| ID | 类别/原文 | 项目 | 状态及剩余 |
|---|---|---|---|
| G01 | 功能缺项 / §13/§27 | 自定义类型扩展 | **未交付**：封闭白名单之外，无 .msg/.srv/.action/IDL 生成、可信 adapter 注册机制及至少一个安全 custom 端到端例子。拒绝不安全任意动态类型本身符合原任务。 [src/uwvm2/imported/ros/policy/schema.h](/Users/liyinan/Documents/MacroModel/src/uwvm2-ros/src/uwvm2/imported/ros/policy/schema.h:114) |
| G02 | 协议缺项 / §15 | 七类统一事件系统 | **部分实现**：现 event_poll 是资源 payload 轮询；service-response 同步返回、action feedback/result 独立 RPC；resource-error/resource-closed 事件未实现。不能把 errno/清空队列算已交付闭合事件。 [src/uwvm2/uwvm/imported/ros/host.h](/Users/liyinan/Documents/MacroModel/src/uwvm2-ros/src/uwvm2/uwvm/imported/ros/host.h:362) |
| G03 | 功能语义收窄 / §11/§27 | ROS1 Parameter Search | **仅精确名称安全子集**：调用 hasParam，存在则返回 cap.resource。原生父命名空间检索/searchParam 语义未交付，现有 PASS 只证明精确名称变体。 [gateways/ros1/resources.h](/Users/liyinan/Documents/MacroModel/src/uwvm2-ros/gateways/ros1/resources.h:218) |
| G04 | 配置契约缺口 / §12/§20/§24 | ROS2 Action respond QoS | **源码确认缺检查；未实际复现**：respond 复用 advertiser 的 endpoint，未比较自己的 40 个 QoS 与 advertiser；Worker/Gateway 的 QoS 一致性循环仅覆盖 service.respond。不能用 service 或字节限额 PASS 代证 Action。 [gateways/ros2/action.h](/Users/liyinan/Documents/MacroModel/src/uwvm2-ros/gateways/ros2/action.h:410) |
| G05 | 功能缺项 / §17 | 通用队列 backpressure | **明确 UNSUPPORTED**：只支持 drop-oldest/drop-newest/reject，backpressure 返回 unsupported。Action 清理池暂时拒绝新 Goal 是额度背压，不是通用队列协议。 [src/uwvm2/imported/ros/policy/schema.h](/Users/liyinan/Documents/MacroModel/src/uwvm2-ros/src/uwvm2/imported/ros/policy/schema.h:196) |
| G06 | 最终集成验收 / §21/§30/§34/§35 | 最终当前源码的统一重建/全量验收 | **未完成**：最后完整 A15 是 contracts 冻结版；随后 deadlines/resources 有生产变更，当前 Worker 1678 个已列编译输入中 8 项 SHA 不同，另有 Gateway/IPC 差异。历史 PASS 与专项 PASS 不能拼成当前完整 PASS。 |
| G07 | 明确场景缺测 / §3/§15/§34 | 真实 ROS 多 memory 运行 | **未找到执行证据**：多 memory 夹具和 wat2wasm 编译存在，但 W70 只有 Memory64 bounds/growth，没有 multi_memory。旧 final-checklist 的多内存通过表述超出证据。Memory64 的真实 PASS 保留。 |
| G08 | 明确故障场景缺测 / §23/§26 | Master 中断恢复及单方 ROS 网络断开 | **未找到实证**：现故障注入杀的是 Gateway；不能代证 ROS1 Master kill/restart 或 DDS/ROS 传输网络断开恢复。 |
| G09 | 组合场景缺测 / §19/§24 | 撤销后重连与真实 Guest Enforce | **未找到完整组合 E2E**：撤销、重连分别有测试；撤销→故障→新连接仍拒绝未完整组合。SROS2 签名 enclave/permission 真实通过的是 native IPC probe；Wasm 普通网络用 disabled-test。 |
| G10 | 安全验证覆盖 / §28 | 55 项攻击的额外副作用/原生矩阵 | **有界 PASS；仍有补证**：Graph 拒绝/洪泛缺独立完整 query 次数；错误 UID、原始帧 native 副作用矩阵缺。queue flood/native contenders 与部分 callback/UAF stress 是真实测试，但不能写成恶意并发 Wasm或整 SDK sanitizer。未穷举所有可能攻击不是新增任务义务。 |
| G11 | 性能缺测 / §29 | 队列和 IPC 的独立指标 | **部分完成**：未见 Event Queue 专项性能、实际 occupancy/high-water；queue depth=32 为配置常量。clock 往返含鉴权/native/编码，不是隔离的纯 IPC overhead。multi 场景顺序执行，非并发 Wasm。 |
| G12 | CI/回归验证 / §30/§34 | 完整既有回归及实际 CI | **部分完成**：四产品矩阵及 workflow 已实现并有 SSH 真构建；hosted CI 未运行。REG36 是 16 真实执行+20 夹具编译，不能称全部 WASI/TrustedDL/Runtime 历史与平台测试。 |
| G13 | 示例交付 / §32 | 20 例完整材料和三种 Group 组合 | **部分完成**：源例/Wasm 存在，但每例编译、完整 CLI、全权限/限额、期望输出、真实方法未全部集中交付。没有一份完整 WASI Group+ROS1 Group+ROS2 Group 同时配置；精确 unauthorized.wasm 两例运行未确认。 |
| G14 | 证据交付 / §35 | 最终原始子报告与统一状态清单 | **需补齐最终包**：contracts 的 4 个原始 RPC/network/five/performance 报告未作为独立 payload 收入其包。只读 SSH 已核实原件存在且 SHA/PASS 匹配 aggregate，因此是打包补项，不是测试未执行。旧 50/55 scoped PASS 必须连同范围阅读。 |
| L01 | 后续明确要求 / 用户后续默认无权要求 | WASIp1 自身默认仅基础 stdio | **FAIL，尚未同步收紧**：ROS 默认禁用/空 grants 已实测。现 WASIp1 仍默认环境继承、clock/random 和宽 stdio rights；最早 §18 并未要求改全部 WASI 默认值，因此此项单列后续需求。 |

G04是源码确认的配置契约缺口，尚未进行故意配置不一致的原生复现，不把它写成已经证实的越权。ROS1 Action 已同时检查advertiser/responder字节额度；其feedback envelope可能有1字节过严边界，仅静态推断、未测试，见[original-task-audit-20261013-evidence/features-supplement.md](original-task-audit-20261013-evidence/features-supplement.md)。

## 35章逐章对照

| 章 | 原题 | 完成度 | 已做/剩余 |
|---:|---|---|---|
| 1 | 一、开发者角色与技术要求 | 已实施 | C++26 Worker、兼容 Noetic/Jazzy 的独立 C++17 Gateway；必要身份/内存入口扩展已说明。原有回归只有子集。 |
| 2 | 二、已知的 uwvm2 能力 | 已保留基础设施；有支持边界 | WASI、多模块、Memory64、TrustedDL 继续存在；ROS+LLVM/named-module/Wasm线程不支持，多 memory 实际 ROS 执行缺证。 |
| 3 | 三、首先检查真实仓库 | 已完成必要分析 | 已按真实 CLI、Runtime 身份、Host Memory API、构建和 SSH 环境实施；不表示所有后端都适配。 |
| 4 | 四、ROS1 与 ROS2 同时接入的架构 | 核心架构完成且已测 | 单 Worker、两个独立进程 Gateway、不同用户空间、稳定 IPC、独立崩溃/恢复。OS 网络隔离仍依赖可信部署。 |
| 5 | 五、明确区分 ROS1、ROS2 的 API | 完成且已测 | 两个独立 Import Module，无自动版本回退或 ros1_bridge；真实双向转换已测。 |
| 6 | 六、完全取消 Level 和权限模板 | 完成且已测 | 没有 Level/Role/Preset/隐式授权；ROS 默认 disabled、空 grants。 |
| 7 | 七、ROS1 与 ROS2 的 Global / Single / Group | 完成且已测 | 每版本独立 Global/Single/Group；显式绑定不继承 Global；Group 不共享 Handle。 |
| 8 | 八、新增两个命令行大类 | 完成且已测 | --help ros1 / ros2 分类及拒绝不支持配置。 |
| 9 | 九、ROS1 命令行 | CLI 实现完成 | 53 个 ROS1 注册参数、显式 grant/set/schema；参数搜索语义另有缺口。 |
| 10 | 十、ROS2 命令行 | CLI 实现完成；契约部分 | 59 个 ROS2 注册参数；Action respond 的 QoS 字段一致性/实际采用存在缺口。 |
| 11 | 十一、ROS1 权限操作清单 | 部分完成 | 29 操作的支持类型具备实现/实证；param.search 只有精确名称 hasParam 变体，未交付父命名空间检索。 |
| 12 | 十二、ROS2 权限操作清单 | 支持类型已实现并验证 | 34 操作，包括本地 declare/undeclare、九类 typed events、独立删除事件和 Lifecycle；不支持范围明确拒绝。 |
| 13 | 十三、消息类型系统 | 部分完成 | 规定的初始类型与可信身份/结构/字段校验已测；无自定义 msg/srv/action 生成/注册机制及安全 custom 样例。 |
| 14 | 十四、Capability Handle 安全 | 完成且已测（解释器） | 可信 Instance、generation、跨 Instance/ROS 句柄拒绝、普通/尾调用代理拒绝；没有显式委托。 |
| 15 | 十五、定义稳定 Host ABI | 部分完成 | C/C++ SDK、i64 offset、受控复制、Memory64/growth 已测；七类统一事件系统不全，多 memory 运行缺证。 |
| 16 | 十六、权限检查位置 | 检查链已实现并有实证 | 每次操作/交付/撤销检查、lease 线性化有证据；无法回滚外部已准入效果，原生所有 race/UAF 无形式证明。 |
| 17 | 十七、资源限制 | 部分完成 | 显式速率/字节/资源/队列/超时额度有实证；drop-oldest/drop-newest/reject 已实现，通用 backpressure 未实现。 |
| 18 | 十八、WASI 与 ROS 的联合隔离 | 严格 ROS 场景完成且已测 | 有效 WASI 配置/环境继承/preopens/插件/socket stdio guard；网络 namespace、pipe/file relay 和 native TCB 由部署保证。 |
| 19 | 十九、ROS1 和 ROS2 的实际运行时生命周期 | 部分验收完成 | 四模式、Gateway 崩溃/恢复、Trap 清理和最新 Action 有界生命周期已测；撤销后断连重连组合未找到完整 E2E。 |
| 20 | 二十、配置透明性与审计 | 部分完成 | 静态 check/dump、稳定排序、审计大小/速率完成；Action respond QoS 冲突未拒绝；静态 dump 不等于 runtime Instance ID 记录。 |
| 21 | 二十一、构建与平台兼容 | 四 Linux 配置历史通过 | none/ros1/ros2/dual 无 ROS 开发库 Worker 构建真实通过；最终当前源码尚无统一全量重建验收。 |
| 22 | 二十二、SSH Linux 测试环境 | 历史真实环境验证完成 | 既有 SSH/Noetic/Jazzy/64 GiB、预算/PID/清理/OOM 记录齐备；本次审计没有执行新测试。 |
| 23 | 二十三、ROS1 真实通信测试 | 部分完成 | ROS1 四类型双向、注册、拒绝、撤销真实通过；明确要求的 Master 中断/恢复缺测。 |
| 24 | 二十四、ROS2 真实通信测试 | 部分完成 | Jazzy 收发/发现/Topic QoS/资源限额等通过；Enforce 仅 native IPC→Gateway→DDS，Guest Enforce E2E 缺测，Action QoS 有缺口。 |
| 25 | 二十五、多模块 Group 测试 | 完成且已测（解释器） | 五个指定模块、两版本 Group/Single、真实模块调用/Handle 转交/代理拒绝和独立额度通过。 |
| 26 | 二十六、ROS1 + ROS2 同时通信测试 | 部分完成 | 同进程双向 hybrid、独立 grants、Gateway 故障和撤销隔离、高频工作负载通过；ROS 网络本身断开缺测。 |
| 27 | 二十七、Service、Parameter、Action、TF 测试 | 部分完成 | 初始 Service/Parameter/Action/TF/Graph/Lifecycle 真实 native+Guest 测试；custom 扩展与 ROS1 层级 search 未交付。 |
| 28 | 二十八、完整权限攻击测试 | 有真实有界证据；补证未全 | 55 条逐项映射已核对，部分只有 native/单元或缺独立副作用计数；不能把矩阵名数等同全场景完成。 |
| 29 | 二十九、性能与稳定性测试 | 部分完成 | 24 性能行、native/Wasm 比较、CPU/RSS/CS/计数及授权微基准真实存在；队列 occupancy/专项性能、纯 IPC overhead 缺测。 |
| 30 | 三十、测试基础设施和 CI | 部分完成 | 自动入口、四组合构建、超时/清理/状态存在且运行过；hosted CI 未运行，既有测试只回归子集，最新未统一 A15。 |
| 31 | 三十一、最终目录组织 | 目录组织完成 | 采用真实仓库逻辑分层；原文目录为建议，不要求机械复刻目录名。 |
| 32 | 三十二、必须提供的示例 | 部分完成 | 20 源例与 Wasm 均存在；逐例完整七件套资料、精确 unauthorized 两例执行证据及三种 Group 集中组合不足。 |
| 33 | 三十三、任务实施顺序 | 阶段工作实际存在；未整体收尾 | 12 阶段均有对应实现/记录，不能因阶段有产物就说全部原目标完成。 |
| 34 | 三十四、最终验收标准 | 50 项逐字重新判定 | 保留每项的实现、历史验证与剩余范围；最终当前版本全量验收尚未完成。 |
| 35 | 三十五、最终输出要求 | 报告结构完成；最终交付部分 | 23 类输出材料存在；需统一最终当前版验收、补遗漏原始子报告和完整示例/未完成项。 |

## 原文50项最终验收逐字对照

下表保留原项，不继承旧表“一律PASS”。所有历史网络验收对应冻结产物；**当前最终源码完整重跑仍未完成**。逐项原始记录与当前缺口在JSON内。

| # | 原始验收项 | 当前判断 | 证据/未完成范围 |
|---:|---|---|---|
| 1 | uwvm2 能在无 ROS 依赖时正常构建 | 已实现且有历史真实验收（限定范围） | B4：`none` 独立产品实际编译、链接、执行 version；UWVM 不包含或链接 ROS 头文件/库。REG36 在该产品上执行回归子集。 |
| 2 | uwvm2 支持 ROS1-only | 已实现且有历史真实验收（限定范围） | B4：ROS1-only v8 实际构建；REG36 回归，BC4 真实拒绝未编译的 ROS2 参数（126）。 |
| 3 | uwvm2 支持 ROS2-only | 已实现且有历史真实验收（限定范围） | B4：ROS2-only v8 实际构建；REG36 回归，BC4 真实拒绝未编译的 ROS1 参数（126）。 |
| 4 | uwvm2 支持 ROS1+ROS2 | 已实现且有历史真实验收（限定范围） | B4：dual v8 实际构建；W70、R243、G5 在同一 dual 产品上执行真实两后端通信。 |
| 5 | uwvm --help ros1 正常 | 已实现且有历史真实验收（限定范围） | CLI590：真实执行 `--help ros1`，验证分类输出、已注册 ROS1 参数；[build.md](build.md)、[capabilities.md](capabilities.md) 对应当前 CLI。 |
| 6 | uwvm --help ros2 正常 | 已实现且有历史真实验收（限定范围） | CLI590：真实执行 `--help ros2`，验证分类输出、已注册 ROS2 参数；[build.md](build.md)、[capabilities.md](capabilities.md) 对应当前 CLI。 |
| 7 | ROS1 Global/Single/Group 完整 | 已实现且有历史真实验收（限定范围） | CLI590、W70、A33、G5：显式创建/配置/绑定与原生通信；显式 Single/Group 不继承 Global，未知模块与不完整目标启动拒绝；两个 camera Group 实例独立会话及配额。 |
| 8 | ROS2 Global/Single/Group 完整 | 已实现且有历史真实验收（限定范围） | CLI590、W70、A33、G5：显式目标/绑定与原生通信；ROS2 Group 与 ROS1/WASI 同名 Group 独立，Single/Group 不继承 Global；planner Group 和 hybrid Single 分别执行。 |
| 9 | 没有 Level 权限模型 | 已实现并核对当前源码/文档；有真实执行记录 | 当前 Schema/CLI 源码对照：63个独立操作 Schema，无 Level、大小关系或数值权限展开；CLI590、A33 的缺项/未知字段拒绝与独立 grant 测试符合该模型。 |
| 10 | 没有隐藏 Role 权限模型 | 已实现并核对当前源码/文档；有真实执行记录 | 当前 Schema/CLI 源码对照：授权只取显式目标中的独立 capability grant，无 Role/预设角色展开；A33、W70 的未授予别名/操作返回拒绝。 |
| 11 | 没有隐式权限展开 | 已实现并核对当前源码/文档；有真实执行记录 | A33、G5、W70：Global 不并入显式 Group/Single；ROS/WASI Group 不互授；跨模块普通/尾调用不构成委托；Service/Action 的独立操作分别授权。 |
| 12 | 所有安全相关参数均显式配置 | 已实现并核对当前源码/文档；有真实执行记录 | SC63 当前63项操作、94项固定字段及5项条件字段（Twist2+numeric参数bounds2+array元素quota1）对照；连接、类型身份、资源、限流/队列/审计缺项拒绝。CLI590 对九项 Service/Parameter/Lifecycle 操作逐一缺失八字段均拒绝；Topic/TF/ParameterEvent八字段、Action五通道40字段显式。 |
| 13 | 配置不完整时拒绝启动 | 已实现并核对当前源码/文档；有真实执行记录 | CLI590、A33：缺连接/操作字段、未知/不适用字段、非法值/资源/类型身份均在 Guest 初始化前拒绝；真实启动错误与 Guest 未进入哨兵、原生零通信共同验证。 |
| 14 | 每个 Wasm Instance 拥有独立 Capability Table | 已实现且有历史真实验收（限定范围） | G5：五个实际 Instance，camera_a/b 共用 Group 模板却各有独立会话/Handle 表；共享 quota 别名各允许2次、第三次11，原生各收到2条，审计每会话2次。C4 验证独立撤销。 |
| 15 | ROS1 Handle 不能用于 ROS2 | 已实现且有历史真实验收（限定范围） | W70：真实 Wasm 将 ROS1 Handle 用于 ROS2 返回9/拒绝，输出哨兵保持，原生无对应消息；G5 的跨后端未授予别名拒绝补充验证。 |
| 16 | ROS2 Handle 不能用于 ROS1 | 已实现且有历史真实验收（限定范围） | W70：真实 Wasm 将 ROS2 Handle 用于 ROS1 返回9/拒绝，输出哨兵保持，原生无对应消息；两后端的表、会话和撤销独立。 |
| 17 | 跨模块 Handle 不能越权使用 | 已实现且有历史真实验收（限定范围） | W70 的外来 Handle 与跨模块普通/尾调用拒绝；G5 的 camera_b 读取 camera_a 仍存活的真实 Handle 后 publish 返回9，独立原生观察无额外消息。 |
| 18 | Wasm 能向 ROS1 发布消息 | 已实现且有历史真实验收（限定范围） | W70：String/Bool/Twist/Image 实际 Wasm publish，独立 Noetic 原生订阅者逐字段/字节验证；G5 验证 camera/controller/hybrid 实际发布及准确次数。 |
| 19 | Wasm 能从 ROS1 订阅消息 | 已实现且有历史真实验收（限定范围） | W70：独立 Noetic 原生节点发布四种初始类型，实际 Wasm 经事件队列读取并校验内容；N13 验证显式队列边界和三种溢出策略。 |
| 20 | Wasm 能向 ROS2 发布消息 | 已实现且有历史真实验收（限定范围） | W70：String/Bool/Twist/Image 实际 Wasm publish，独立 Jazzy 原生订阅者逐字段/字节验证；G5 验证 planner/hybrid 实际发布及准确次数。 |
| 21 | Wasm 能从 ROS2 订阅消息 | 已实现且有历史真实验收（限定范围） | W70：独立 Jazzy 原生节点发布四种初始类型，实际 Wasm 读取并校验内容；N13 验证实际 QoS、显式队列边界和溢出策略。 |
| 22 | ROS1 Service 正常 | 已实现且有真实验收（初始类型） | Trigger/SetBool 两端 client/server 实证；custom 类型机制另见G01，不要求允许不安全任意动态类型。 |
| 23 | ROS2 Service 正常 | 已实现且有真实验收（初始类型） | 同上；最终GW整体未统一复验。 |
| 24 | ROS1 Parameter 正常 | 部分完成 | String/Bool/int32/Float64及数组、exists/delete/list有实证；search只是精确名称安全子集，见G03。 |
| 25 | ROS2 Parameter 正常 | 已实现且有真实验收（支持投影） | 标量/数组/九类事件/独立删除已实现；remote declare/undeclare明确不支持，本地操作已测。 |
| 26 | ROS1 Actionlib 正常 | 已实现且有真实验收（Fibonacci） | Goal/cancel/feedback/result/client/server已测；custom扩展未交付。 |
| 27 | ROS2 Action 正常 | 已实现、部分验收 | Fibonacci及最新7生命周期+8限额通过；Action respond QoS字段可能被忽略、最终全量复验未做。 |
| 28 | ROS1 TF 正常 | 已实现且有历史真实验收（限定范围） | R243、N13：TransformStamped 实际 lookup/broadcast，固定父/子 frame、有限数值/translation/quaternion 限界；原生验证1/2/3平移和单位旋转；A33 拒绝 lookup 后原生订阅数0。 |
| 29 | ROS2 TF 正常 | 已实现且有历史真实验收（限定范围） | R243、N13：TransformStamped 实际 lookup/broadcast、精确 frame 过滤与缓存边界；原生逐字段验证及实际 QoS/deadline 检查。负 ROS2 时间戳等未支持范围明确拒绝。 |
| 30 | ROS2 QoS 正确生效 | 部分完成 | Topic及Service/Parameter/Lifecycle实际QoS有验证；Action respond/advertise一致性缺检查，任意不安全配置须拒绝。 |
| 31 | 多模块权限隔离成立 | 已实现且有历史真实验收（限定范围） | G5：必需五模块实际 start 段/主入口、六资源精确消息、Group 独立 quota、跨模块 Handle9/别名13；W70 普通/尾调用代理拒绝；A33 Global、后端 Group 与 WASI Group 混淆拒绝。 |
| 32 | ROS1 与 ROS2 同时运行成功 | 已实现且有历史真实验收（限定范围） | W70：同一产品/进程的双向四类型桥接；G5：ROS1 camera/controller/hybrid 与 ROS2 planner/hybrid 同时执行，原生分别证明内容和数量。 |
| 33 | 同一个 Wasm 能分别访问两套 ROS API | 已实现且有历史真实验收（限定范围） | W70 双后端 hybrid 同一 Wasm 分别调用两套 import；G5 的 hybrid 同一实际 Instance ID 在两后端分别发布，必须分别授予 ROS1/ROS2 grant。 |
| 34 | 未授权 ROS 操作被拒绝且无对应副作用 | 真实有界验证；补证未全 | 55条逐项实证模式见后表；缺独立Graph query次数、wrongUID/raw frame native矩阵等。不等于已有漏洞。 |
| 35 | WASI 不能绕过 ROS 授权层 | 已实现且有真实验收（严格ROS配置） | 检查最终WASI配置及preopens/环境/插件/socket stdio；部署网络/可信native不属于Wasm沙箱。后续WASI默认仅stdio另列L01。 |
| 36 | IPC 身份和数据长度经过验证 | 部分完成/验证 | UID/PID/session/manifest及部分payload真实验证；错误UID/raw frame完整副作用攻击矩阵缺测 |
| 37 | Gateway 异常不破坏另一 ROS 后端 | 已实现且有历史真实验收（限定范围） | W70：分别终止 ROS1/ROS2 Gateway，另一后端原生通信继续、实际 Worker 保持运行；恢复用例验证重新连接，不复用失效会话/Handle。仅所测 Gateway 故障方向，不承诺宿主整体故障隔离。 |
| 38 | Wasm Trap 后资源安全释放 | 有真实Trap清理验证；边界保留 | I16 endpoints消失、Gateway自行退出；不能保证crash时远端原生Goal取消，也非全部资源/异步组合证明。 |
| 39 | SSH Linux 上真实 ROS 测试完成 | 已实现且有历史真实验收（限定范围） | 最终 `acceptance-final-v8-io-final/report.json` 15/15套件PASS、1504.007秒；当前v8产品/Gateway输入SHA与嵌套报告SHA核对，真实Noetic/Jazzy通信及拒绝观察通过。 |
| 40 | 测试进程处于已有 64 GiB Cgroup 内 | 已实现且有历史真实验收（限定范围） | B4、N13、W70、R243、G5、A33、I16、C4、F6、P18、T24 保留实际 runner/编译/原生/Worker 后代成员记录；复用既有容器 cgroup，有效 `memory.max=68719476736`，未创建新 cgroup 或放宽限制。 |
| 41 | 测试期间没有无控制的 Cgroup OOM | 已实现且有历史真实验收（限定范围） | 已执行报告的前后 `oom`/`oom_kill`/`oom_group_kill` 无增量，性能另有进程 RSS 停止预算；共享 cgroup 累计峰值不当作本任务独占峰值。最终聚合再次核对 `oom`/`oom_kill`/`oom_group_kill` 前后均0。 |
| 42 | 原有 WASI Preview 1 测试通过 | 部分验收 | WASI args/fd_write/Memory64等既有回归子集；未执行完整既有历史套件。 |
| 43 | 原有 Trusted DL Plugin 测试通过 | 部分验收 | 0002.dl等既有TrustedDL路径；未执行完整所有插件/平台回归。 |
| 44 | 原有 Wasm Runtime 测试通过 | 部分验收 | 解释器/Memory64/tail子集；多memory真实ROS未确认，旧W70覆盖多memory表述不成立。 |
| 45 | 测试可自动复现 | 部分完成 | 冻结源/ELF、命令、依赖说明及脚本可复现；当前源drift、SDK/rootfs前提、部分原报告未完整打包需补。 |
| 46 | CLI 文档完整 | 已实现并核对当前源码/文档；有真实执行记录 | [build.md](build.md)、[capabilities.md](capabilities.md) 覆盖当前 ROS1 53/ROS2 59个已注册参数、目标/操作字段、合法范围及拒绝规则；CLI590 实际 help/参数/enable→disable 四种切换验证。范围为已实现 CLI，未支持功能明确列出。 |
| 47 | 安全模型文档完整 | 已交付准确边界说明；需统一状态 | 安全文档区分可信native/ROS1 ACL/中间件分配；历史scoped清单不能当最终全任务PASS。 |
| 48 | ABI 文档完整 | 已交付现有ABI文档；原协议部分缺失 | 已有C/C++签名/布局/错误码等；七类统一事件系统未全实现，见G02。 |
| 49 | 多模块配置示例完整 | 部分完成 | 五模块源码/真实调用/runner完整；逐例七件套和三种Group集中组合缺。 |
| 50 | ROS1+ROS2 双运行示例完整 | 部分完成 | 双向hybrid真实通过；逐例完整操作资料仍不足。 |

## 原文55类攻击逐字对照

W70/R243/G5/A33/I16/C4/F6/P18/N13/T24是实际报告标签，旧映射及精确SHA见[acceptance-matrix.md](acceptance-matrix.md)，本审计独立输入见[original-task-audit-20261013-evidence/validation.json](original-task-audit-20261013-evidence/validation.json)。以下保留真实Expected Error/原生效果与实测方式；不是55个无限范围安全证明。

| # | 原攻击项 | 实际方式/覆盖状态 | 错误与native效果 | 仍需说明/补证 |
|---:|---|---|---|---|
| 1 | 无授权 Topic Publish | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `topic_denied`、普通/尾调用代理均13；同时运行的原生观察器无消息、无 Publisher。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 2 | 无授权 Topic Subscribe | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | A33 `topic_subscribe_denied`：publish-only handle 订阅13；原生发布者活动，Subscriber 数量0。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 3 | 无授权 Service Call | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `service_denied`：advertise-only handle 调用13、输出哨兵不变；活动的官方 Trigger Server 实际 callback 数0。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 4 | 无授权 Parameter Get | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | A33 `parameter_get_denied`：set-only handle 读取13、输出不变；实际 ROS1 Master getParam / ROS2 GetParameters callback 数0。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 5 | 无授权 Parameter Set | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | R243 `parameter_set_denied`：13、输出不变；独立原生读取仍为 hello。 | ROS1 denied mutation仅5秒原生值采样，不能证明瞬时set/restore次数为0；ROS2有setter0计数。 |
| 6 | 无授权 Action Goal | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `action_denied`：非 send-goal 授权13；活动的官方 Fibonacci Server 实际 Goal request/accepted 数0。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 7 | 无授权 Action Cancel | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | A33 `action_cancel_denied`：cancel-own 与 cancel-any 均13；真实活动 Goal 的 cancel callback 数0，正常结果成功。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 8 | 无授权 TF Lookup | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | A33 `tf_lookup_denied`：broadcast-only handle 查询13；原生持续发布 Transform，未经授权的 TF Subscriber 数0。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 9 | 无授权 TF Broadcast | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | R243 `tf_broadcast_denied`：lookup-only handle 广播13；原生观察器收到未经授权 Transform 数0。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 10 | 无授权 ROS Graph 查询 | actual Wasm + native witness / PARTIAL | W70 `graph_denied`：错误 Graph operation13，输出和长度哨兵不变，无 Graph 数据披露；原生环境同时保持活动。 | Guest 断言 EACCES/输出不披露，有活动 ROS 环境；原生 absent witness 只看 forbidden Topic，无独立 Master/DDS graph 查询次数计数。 |
| 11 | 伪造 Capability Handle | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `handle_security`：伪造 handle9；无原生消息、无 Publisher。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 12 | 旧 Handle 重用 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `handle_security`：close 后旧 handle9、reopen 为新 generation；revoke 后旧 handle9、别名重新 open125；无原生发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 13 | 跨 Instance Handle 使用 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `cross-module-handle` 与 G5：实际非零 live handle 转交另一 Instance 后9；观察器无对应原生消息。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 14 | ROS1 Handle 用于 ROS2 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `cross-backend-handle`：实际 ROS1 handle 传给 ROS2 返回9；双后端无对应发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 15 | ROS2 Handle 用于 ROS1 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `cross-backend-handle`：实际 ROS2 handle 传给 ROS1 返回9；双后端无对应发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 16 | Module Name 伪造 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 `forged-module-name`：启动126、准确 absent-module diagnostic；Guest sentinel201 未进入、原生无发布。ABI 无可自行声明模块名的参数。 | 是CLI startup negative，不是Guest可以自报ModuleName的ABI测试；Guest未进入。 |
| 17 | Module Group 混淆 | mixed Guest/native / VERIFIED_BOUNDED_NETWORK | A33 多 Group 绑定启动126；Global+Group 不继承测试 open13；G5 同一 Group 的两个真实 Instance 各自成功2次、各自第三次11，原生总数4。 | 多组绑定拒绝/Global不继承 + G5独立quota，但不是并发调度Wasm线程。 |
| 18 | ROS1/ROS2 Group 混淆 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | A33 `backend_group_confusion`：同名 ROS1/ROS2 Group，ROS1 open0、ROS2 空 Group open13；无越权原生发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 19 | WASI/ROS Group 混淆 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | A33 `wasi_group_confusion`：同名 WASI Group 的 proc_exit0 不授予 ROS 权限；空 ROS Group open13、handle 哨兵不变、原生无发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 20 | 多模块间权限代理攻击 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `cross-module-proxy` / `cross-module-tail-proxy`：依据解释器实际调用祖先13，原生无消息/Publisher。LLVM ROS 执行另列 UNSUPPORTED。 | 真实解释器普通/尾调用代理拒绝；开启ROS的LLVM调用链仍UNSUPPORTED。 |
| 21 | Topic 名称规范化绕过 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 `normalized-resource`：含 /../ 的资源启动126、准确策略 diagnostic、Guest 未执行，原生无发布。 | 这里只证明非规范配置在startup被拒绝；不等于所有ROS remapping动态路径。 |
| 22 | Relative Name 绕过 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 `relative-resource`：相对资源启动126、准确策略 diagnostic、Guest 未执行，原生无发布。 | 拒绝相对CLI资源，支持范围不允许relative Guest资源名。 |
| 23 | Private Name 绕过 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 `private-resource`：~output 启动126、准确策略 diagnostic、Guest 未执行，原生无发布。 | 拒绝私有CLI资源。 |
| 24 | Namespace 绕过 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 `namespace-bypass`：非规范 namespace 启动126、Guest 未执行，原生无发布。 | 拒绝非规范namespace配置。 |
| 25 | Remapping 绕过 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 `remapping-resource`：含 := 的资源启动126、Guest 未执行，原生无发布。 | 拒绝包含:=的CLI资源。 |
| 26 | 错误 MD5 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 ROS1 `wrong-md5`：启动126、准确 identity mismatch diagnostic；Guest 未进入，原生无发布。 | 校验配置与trusted generated adapter MD5，不是网络伪造同名publisher的全面协议测试。 |
| 27 | 错误 ROS Type | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 两版本 `wrong-ros-type` / `cross-version-type`：启动126、准确 adapter diagnostic；Guest 未进入，原生无发布。 | 初始generated适配器的错误/跨版本Type配置拒绝。 |
| 28 | 错误 ROS2 Type Support | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 ROS2 `wrong-typesupport`：启动126、准确 generated identity diagnostic；Guest 未进入，原生 DDS 无发布。 | trusted generated ROS2 identity/typesupport mismatch拒绝，不是任意动态typesupport适配器。 |
| 29 | 错误 Serialized Message | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | P18 非法 String 长度和 overlong UTF-8 均22；N13 直接 IPC 的0xff 也22；持续活动原生观察器无消息/Publisher。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 30 | 消息字段验证绕过 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | P18 Bool2、截断 Twist、错误 Image 长度22，超出显式 Twist bound13；无原生消息/Publisher。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 31 | NaN | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | P18 实际 Wasm Twist NaN22；原生无消息/Publisher。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 32 | Infinity | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | P18 实际 Wasm Twist Infinity22；原生无消息/Publisher。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 33 | 超大消息 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | P18 规范 String4097字节超过4096上限90；W70 UINT64_MAX length90；N13 原生输入65字节/cap64丢弃后poll11、合法消息仍可用。ROS 库反序列化前分配硬上限另列 UNSUPPORTED。 | 真实Guest过限载荷/UINT64_MAX拒绝；原生反序列化前硬分配上限仍UNSUPPORTED。 |
| 34 | 超长资源名称 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | A33 `overlong-resource`：1025字节资源启动126、准确 diagnostic、Guest 未执行，原生无发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 35 | Offset Overflow | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `memory64_bounds`：UINT64_MAX offset、末页跨界22；原生无发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 36 | Length Overflow | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `memory64_bounds`：UINT64_MAX length90、非法输出 offset22，输出哨兵不变，原生无发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 37 | Memory64 Offset 错误 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 实际启用 memory64 的 Guest 使用错误64位 offset，精确22/90；原生无发布。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 38 | Memory Growth 后使用旧指针 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `memory64_growth`：实际 grow 成功，新页数据通过重新解析的当前内存视图发布，原生收到精确内容。ABI 使用 offset/length，不保留 Guest 指针。 | 实际Memory64 grow后新页正确发送；ABI使用offset/length不保存Guest裸指针，并非把真实dangling裸指针交给API。 |
| 39 | Publisher Flood | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `publish_flood`：100次 Guest 调用，burst2，98次11；原生实际消息数2。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 40 | Service Flood | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | F6 Service：100次 Guest 调用，2次成功/98次11；官方 Trigger 实际 callback 数2。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 41 | Action Flood | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `action_flood`：100次 Guest 调用，2次成功/98次11；原生 Fibonacci Goal 数2。 | 仅对固定Linux解释器、官方初始适配器和有界fixture成立；没有宣称所有平台/动态类型。 |
| 42 | Event Queue Flood | native IPC/thread stress + helpers / PARTIAL | N13 原生 ROS 20条输入、depth2/128字节预算；drop-oldest 精确保留最后2条，drop-newest/reject 保留最初2条，随后poll11。使用真实原生 ROS→认证 IPC，不计作恶意 Guest emitter 或 middleware 精确丢包率。 | 是真实 native ROS→authenticated IPC 的20条输入/三overflow策略功能测试，非恶意 Guest emitter；未给 Event Queue 专门性能测量或middleware全丢包率。 |
| 43 | Logging Flood | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | F6 Log：100次 Guest 调用，2次成功/98次11；原生 logger 精确2条唯一 allow_flood 记录。rosout 被禁用，此项不声称 DDS rosout 发布。 | 原生logger精确2条；rosout关闭，不是DDS rosout flood测量。 |
| 44 | Graph Query Flood | actual Wasm + native witness / PARTIAL | F6 Graph：2次精确且经 scope 过滤的原生 Graph 响应、98次11且输出/长度哨兵不变，原生节点持续活动。没有独立统计全部 Master/DDS query 次数。 | 100次 Guest 请求、2次原生Graph结果及98次EAGAIN哨兵；原生节点活动，但没有独立统计98次拒绝对应的全部Master/DDS query数。 |
| 45 | Gateway 断开 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `fault`：双方向实际 SIGKILL 一个 Gateway，失败后端107/输出不变；存活后端仍实际发布精确数据。 | 两方向SIGKILL一个Gateway/另一仍发；不是宿主全故障保证。 |
| 46 | Gateway 恢复 | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | W70 `recover`：双方向实际重启 Gateway、新 PID 认证与完整 session.bind 成功；原生通信恢复、另一后端持续可用。N13 旧 Service/Goal ID 重启后9，新请求正常。 | 重建PID/session成功；旧ID失效；范围为两后端所测重启。 |
| 47 | 权限撤销 | mixed Guest/native / VERIFIED_BOUNDED_NETWORK | C4 真实 Manager→IPC lease 持有至原生确认，revoke barrier 等待已准入操作；之后100次 Manager9/100次 Gateway125，原生计数在30次采样中不增长。实际双后端 Guest 撤销后 revoked1/survivor3，进程仍存活。R243 ParameterEvent 旧 handle9。 | manager/native线程lease barrier + actual dual-Wasm独立撤销；更多全局跨session reconnection矩阵未验。 |
| 48 | Wasm Trap | actual Wasm + native witness / VERIFIED_BOUNDED_NETWORK | I16 实际 unreachable 导致 SIGILL(-4)；Trap 前原生 Publisher/Subscriber/Trigger 和 hello 存在，随后三种 endpoint 连续10次采样均消失；Gateway pidfd 无外部 kill 自行exit0/socket删除，观察完成后才测试清理。 | unreachable真实SIGILL，原生publisher/subscriber/service消失与pidfd自行退出证明；不得混用runner最终kill清理。 |
| 49 | Callback Race | native IPC/thread stress + helpers / PARTIAL | N13 每后端20轮 Topic callback teardown，ROS2另10轮活动 Action feedback close/revoke 和超时晚 Goal-response；9/125/110正确；Topic subscriber endpoint 消失，Action 实际 native cancel 被观察且无晚 feedback 交付，未统计全部 Action endpoint 消失。 | 有实际native callback close/revoke及ROS2 late-goal有界压力；没有完整Wasm callback-race矩阵或全部Action端点消失计数。 |
| 50 | Use-after-free | native IPC/thread stress + helpers / PARTIAL | N13 同上真实有界 lifetime/late-delivery 压力通过；U62 Manager/codec/IPC/SDK ASan/UBSan 通过。原生 Gateway/ROS库未加 ASan/UBSan，此项是压力观察的 PASS，不是所有 UAF 的形式证明。 | 原生有界lifetime stress + manager/codec/IPC/SDK helper sanitizer；Gateway及ROS库未整体ASan/UBSan，不能宣称所有UAF路径验完。 |
| 51 | 通过 WASI Socket 绕过 ROS1 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | I16 ROS1：继承 socket stdio、Global/Single/Group socket ingress、缺少 noinherit、真实 credential directory 均启动126且 Guest 未进入，原生无发布。 | startup ingress guard，不是已成功取得任意raw socket后运行攻击代码。 |
| 52 | 通过 WASI Socket 绕过 ROS2 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | I16 ROS2：同样严格 ingress 启动126且 Guest 未进入，原生 DDS 无发布。 | startup ingress guard；宿主网络/文件部署仍TCB。 |
| 53 | 通过 Trusted Plugin 路径混淆授权 | config/startup negative + native witness / VERIFIED_STARTUP_BOUNDARY | I16 实际 TrustedDL plugin 与开启 ROS 混用启动126、Guest 未进入、原生无发布。Native initializer 属 TCB；该 guard 不回滚或 sandbox plugin initializer。 | ROS+实际TrustedDL混用startup拒绝；插件initializer是TCB，guard不回滚initializer或沙箱任意native代码。 |
| 54 | 通过 IPC 消息伪造身份 | native IPC/thread stress + helpers / PARTIAL | N13 活动 String/Twist 订阅计数始终0且并发 Graph 采样无 Publisher；真实同UID不同PID拒绝、未 bind/错误 manifest13、伪造 server-issued session 关闭连接、非法规范消息/UTF-8/NaN拒绝。UID相等是正常连接的实测条件，错误UID分支未单独攻击；raw framing/header长度和超时由协议单元验证，W70/P18另验真实Guest长度与载荷。不能把协议单元的所有帧攻击写成原生副作用测试，也不把所有断连统一虚称某个错误码。 | 真实IPC不同PID/manifest/session伪造且native计数0；错误UID未单独攻击；raw framing/header长度/timeout主要unit，未全部配ROS副作用观察。 |
| 55 | 通过并发请求制造授权状态竞争 | native IPC/thread stress + helpers / PARTIAL | C4 四个真实 C++ contender threads、100次 Manager竞争准入，获准请求经真实 IPC/native receipt 验证；另100次撤销后 IPC125，原生计数在revoke后30次采样不增长；T24实际认证 IPC Trigger concurrency4、无错误。当前没有 Wasm线程导入，此项明确使用原生 C++线程。 | native C++四线程竞争真实授权/IPC，并有撤销后100拒绝/native计数冻结；没有恶意Wasm线程用例，Wasm thread import目前不支持。 |

具体补证集中在Graph拒绝/洪泛的独立query计数、wrongUID/raw frame native矩阵，以及更广泛的原生生命周期组合。Callback/UAF的有界压力是真实PASS；原文没有要求形式证明所有UAF，也没有要求把不支持的Wasm线程伪装成已执行。整SDK sanitizer未做是验证边界，不能自动抹掉已执行测试。

## 性能与稳定性要求

24条实测行和详细值在JSON及旧[test-report.md](test-report.md)内。native/Wasm比较、64/4096B、单/双ROS和高频、两实例、Service concurrency4、clock1000次均有真实数据。multi两实例业务顺序运行；Service并发来自可信native IPC clients，不能声称并发Wasm。

| 原要求指标 | 当前状态 | 测量/差距 |
|---|---|---|
| Latency | VERIFIED | one-way monotonic p50/p95/max；IPC/service roundtrip p50/p95/max |
| Throughput | VERIFIED_PACED_ONLY | receive_hz/throughput_hz，固定paced workload，不是最大可持续吞吐 |
| CPU Usage | VERIFIED_CPU_TIME | native getrusage与实际PID/descendants 10ms采样cpu_seconds；未单列归一化CPU百分比；旧报告另按CPU_seconds/elapsed计算1核百分比。 |
| Memory Usage | VERIFIED | native max_rss_kib + wrappers/actual descendant max_rss_bytes，512MiB stop budget |
| Queue Depth | PARTIAL_CONFIGURED_ONLY | queue_depth32 / queue_depth_per_session1为代码输出的配置常数；无实际occupancy/high-water时间序列 |
| Dropped Messages | VERIFIED_BOUNDED_COUNTS | sent==received时dropped0；功能queue测试retention可验，非全部middleware drop counters |
| Permission Check Cost | VERIFIED_ISOLATED_MANAGER | 真实100000次Manager authorize微基准（含验证/额度/审计）；网络测试=false，不是全链权限成本。 |
| IPC Overhead | PARTIAL_INCLUSIVE_TIMING | clock p50 ROS1 12.736us/ROS2 12.539us包括transport/auth/native/serialize，没有subtract/direct baseline隔离overhead |
| Context Switches | VERIFIED | native getrusage及PID/descendant voluntary/involuntary fields |
| cgroup memory.current/peak/events | VERIFIED_SHARED_GROUP | 实际共享cgroup before/after current、peak、events；OOM增量0；不归为任务独占峰值。 |

剩余：Event Queue独立性能测试；实际队列occupancy/high-water而非配置depth；隔离transport的IPC overhead；按原文需要的并发/事件组合补齐最终当前版性能。当前数据并不宣称最大持续吞吐，也没有关闭授权以提升性能。

## 原文20例逐项对照

原文每例要求Guest源码、编译命令、完整UWVM CLI、权限、资源限额、预期输出、实际测试方法。源码和Wasm20/20存在；有runner/归档命令不等于每例已经集中交付完整七件套。

| # | 原例 | 源码/Wasm | 已确认实际证据 | 文档/剩余 |
|---:|---|---|---|---|
| 1 | ROS1 Talker Wasm | examples/ros1/talker.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 2 | ROS1 Listener Wasm | examples/ros1/listener.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 3 | ROS2 Talker Wasm | examples/ros2/talker.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 4 | ROS2 Listener Wasm | examples/ros2/listener.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 5 | ROS1 Service Client Wasm | examples/ros1/service_client_trigger.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 6 | ROS1 Service Server Wasm | examples/ros1/service_server_trigger.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 7 | ROS2 Service Client Wasm | examples/ros2/service_client_trigger.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 8 | ROS2 Service Server Wasm | examples/ros2/service_server_trigger.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 9 | ROS1 Parameter Wasm | examples/ros1/parameter_get.wat / 存在 | R243 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 10 | ROS2 Parameter Wasm | examples/ros2/parameter_get.wat / 存在 | R243 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 11 | ROS1 Action Wasm | examples/ros1/action_client.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 12 | ROS2 Action Wasm | examples/ros2/action_client.wat / 存在 | W70 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 13 | ROS1 TF Wasm | examples/ros1/tf_lookup.wat / 存在 | R243 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 14 | ROS2 TF Wasm | examples/ros2/tf_lookup.wat / 存在 | R243 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 15 | ROS1 Multi-Module Wasm | examples/ros_multimodule/camera_a.wat / 存在 | G5 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 16 | ROS2 Multi-Module Wasm | examples/ros_multimodule/planner.wat / 存在 | G5 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 17 | ROS1+ROS2 Hybrid Wasm | examples/ros_dual/hybrid_1_to_2.wat / 存在 | W70/G5 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |
| 18 | Unauthorized ROS1 Wasm | examples/ros1/unauthorized.wat / 存在 | 原unauthorized WAT/Wasm存在；已核查的运行报告使用其他topic_denied/default-deny fixtures，未在本范围找到这个exact unauthorized.wasm的独立执行+native side-effect报告 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套；同类拒绝测试通过不能替代该exact示例执行证明 |
| 19 | Unauthorized ROS2 Wasm | examples/ros2/unauthorized.wat / 存在 | 原unauthorized WAT/Wasm存在；已核查的运行报告使用其他topic_denied/default-deny fixtures，未在本范围找到这个exact unauthorized.wasm的独立执行+native side-effect报告 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套；同类拒绝测试通过不能替代该exact示例执行证明 |
| 20 | Cross-Module Capability Attack Wasm | examples/ros_dual/handle_attack.wat / 存在 | W70/G5 | Guest源码和实际脚本/归档command可查；README只给通用compile、说明与runner引用，未逐例汇集完整uwvm CLI + 全权限/资源limits + expected output + 实际测试方法七件套 |

三Group完整集中配置例不足：A33分别测试WASI+ROS1同名组、ROS1+ROS2同名组；G5使用WASI Global noinherit与ROS1/ROS2 Group。它们不等于一个完整的WASI Group+ROS1 Group+ROS2 Group组合例。

## 12阶段与23类输出

| 阶段 | 原工作主题 | 完成度 |
|---:|---|---|
| 1 | 仓库/环境/架构 | 已完成必要分析与真实环境验证 |
| 2 | Capability/身份/Handle | 已实现并有真实验证（解释器范围） |
| 3 | CLI/Scope/Schema/Dump | 主体完成；Action QoS契约检查缺口 |
| 4 | Host ABI/Memory/IPC/Event | 主体完成；统一七类事件、多memory实跑未齐 |
| 5 | 双Gateway/生命周期 | 主体完成；特定恢复组合/远端取消边界保留 |
| 6 | Topics/类型/QoS | 初始类型已测；custom扩展与Action契约未齐 |
| 7 | Service/Parameter/Graph/Time/Log | 主体已测；ROS1层级search未交付 |
| 8 | Action/TF/Lifecycle/custom | 初始功能已测；custom与Action QoS契约缺项 |
| 9 | Group/多模块/双ROS | 核心实际场景通过；ROS网络故障/三Group完整例未齐 |
| 10 | 权限/旁路/IPC/多模块测试 | 有55类有界映射；补证项未全 |
| 11 | 完整Linux/性能/回归 | 历史A15真实通过；最终当前版/性能缺指标/完整回归未齐 |
| 12 | 文档/示例/CI/最终交付 | 主体资料存在；逐例手册、最终统一验收/包尚未齐 |

原文35章要求的23类输出材料：

| # | 原输出要求 | 当前材料/状态 |
|---:|---|---|
| 1 | 实际修改的源码文件 | 源码清单/逐文件SHA存在；Runtime起始完整基线缺失的归属限制已说明，不能把全部HEAD差异归为ROS。 |
| 2 | 新增的源码文件 | 源码清单/逐文件SHA存在；Runtime起始完整基线缺失的归属限制已说明，不能把全部HEAD差异归为ROS。 |
| 3 | 实现的 ROS1 API | 支持类型/API/验证机制已文档化；custom、search、统一事件及QoS缺口见本审计。 |
| 4 | 实现的 ROS2 API | 支持类型/API/验证机制已文档化；custom、search、统一事件及QoS缺口见本审计。 |
| 5 | 命令行参数完整列表 | 对应资料存在，见test-report.md同编号章节与build/ABI/CLI/security/multimodule文档。 |
| 6 | 每种权限的参数 Schema | 对应资料存在，见test-report.md同编号章节与build/ABI/CLI/security/multimodule文档。 |
| 7 | Global/Single/Group 的语义 | 对应资料存在，见test-report.md同编号章节与build/ABI/CLI/security/multimodule文档。 |
| 8 | 多模块权限隔离实现 | 对应资料存在，见test-report.md同编号章节与build/ABI/CLI/security/multimodule文档。 |
| 9 | Capability 安全设计 | 对应资料存在，见test-report.md同编号章节与build/ABI/CLI/security/multimodule文档。 |
| 10 | ROS1/ROS2 Gateway 架构 | 对应资料存在，见test-report.md同编号章节与build/ABI/CLI/security/multimodule文档。 |
| 11 | IPC 协议设计 | 对应资料存在，见test-report.md同编号章节与build/ABI/CLI/security/multimodule文档。 |
| 12 | 消息类型验证机制 | 支持类型/API/验证机制已文档化；custom、search、统一事件及QoS缺口见本审计。 |
| 13 | ROS1 通信测试结果 | 多轮真实冻结版本报告与原始记录存在；最终当前版未统一全量复验，不能当整体完成。 |
| 14 | ROS2 通信测试结果 | 多轮真实冻结版本报告与原始记录存在；最终当前版未统一全量复验，不能当整体完成。 |
| 15 | ROS1+ROS2 同时运行测试结果 | 多轮真实冻结版本报告与原始记录存在；最终当前版未统一全量复验，不能当整体完成。 |
| 16 | 多模块测试结果 | 多轮真实冻结版本报告与原始记录存在；最终当前版未统一全量复验，不能当整体完成。 |
| 17 | 安全攻击测试结果 | 多轮真实冻结版本报告与原始记录存在；最终当前版未统一全量复验，不能当整体完成。 |
| 18 | SSH Linux 环境信息 | 实际环境/cgroup/命令/自动入口齐备；复现需要已有SDK/rootfs和正确权限，不能从历史结果推断任意机器自动PASS。 |
| 19 | 64 GiB Cgroup 验证结果 | 实际环境/cgroup/命令/自动入口齐备；复现需要已有SDK/rootfs和正确权限，不能从历史结果推断任意机器自动PASS。 |
| 20 | 编译和运行命令 | 实际环境/cgroup/命令/自动入口齐备；复现需要已有SDK/rootfs和正确权限，不能从历史结果推断任意机器自动PASS。 |
| 21 | 可复现的自动化测试入口 | 实际环境/cgroup/命令/自动入口齐备；复现需要已有SDK/rootfs和正确权限，不能从历史结果推断任意机器自动PASS。 |
| 22 | 尚未解决的技术限制 | 本审计给出当前限制与完整原项判断；历史50/55 scoped清单不是全任务完成。 |
| 23 | 最终验收清单 | 本审计给出当前限制与完整原项判断；历史50/55 scoped清单不是全任务完成。 |

## 证据版本、支持边界与后续默认权限要求

最后完整A15为contracts冻结版本。其后deadlines/resources的专项修正有效，但不能拼出当前全量PASS。current-worker-binding.json按12份旧MMD归并1678输入，当前8项SHA不同，其中含后续ROS IPC修复及已有用户Runtime/debug改动；这些非ROS改动不被归因为本任务。[具体差异](original-task-audit-20261013-evidence/current-worker-binding.json)。

contracts聚合中的4份RPC/network/five/performance原报告未独立收入其旧归档。本次只读SSH确认原件存在、SHA完全匹配aggregate、status=PASS，原字节副本放在[回读清单](original-task-audit-20261013-evidence/contracts-subreports/receipt.json)及同目录。因此这是最终打包完整性补项，不是测试未执行；旧报告和归档没有被修改。

公开UNSUPPORTED范围仍包括ROS启用的LLVM执行、named-module构建、非Linux Gateway、Wasm线程、remote ROS2 declare/undeclare、任意不安全动态类型、keep-all与通用backpressure、原生middleware反序列化前逐消息分配硬上限。需要区分原文明确功能缺项、允许的安全拒绝边界和未测试范围。原任务不要求把第三方ROS生态或可信native C++变成Wasm沙箱。签名/生产证书/高级策略生成器按原文排除，不计作未完成。

后续“默认除了基础stdin/stdout/stderr无任何权利”要求另列L01：ROS默认拒绝已经真实6配置验证；现WASIp1默认仍继承环境、可clock/random、stdio宽rights，因此WASIp1同步收紧未完成。最早§18要求联合隔离，没有要求重写全部WASI默认值，不混淆两个验收范围。

fast_io后续建议已有I/O、request_view和sorted_rows的实际精确输出/分配减少证据；C++17 Gateway不能直接用要求C++20的vendored fast_io。没有全路径吞吐更快的声明，不能用减少临时对象静态推断取代性能测量。

收尾顺序应为：补功能/契约（G01—G05），补明确缺测场景与指标（G07—G13），冻结最终当前源码并统一重建/全量验收（G06），最后完整打包和更新统一原任务清单（G14）。后续WASIp1默认收紧按L01独立实施与回归。
