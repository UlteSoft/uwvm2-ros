# 55 类攻击的最终真实验收映射

最终状态：**PASS（以下明确的有界测试与 Linux 解释器适配范围）**。每行保留用户原始攻击名称，记录实际错误码或预期有界效果及同时存在的原生观察。未运行的实现、普通退出、仅编译的夹具和历史二进制不代替真实结果。技术上未支持的路径列于 [交付报告第22项](test-report.md#22-尚未解决的技术限制)。

所有报告路径相对 SSH 工作区 `/home/macromodel/.local/share/uwvm-ros-extension/20261012-recovery/`，都包含在 [证据包](evidence-20261012-v8.tgz) 内。以下 SHA-256 与 [机器可读索引](final-evidence-index.json) 一致；根聚合报告逐套件保存实际命令、日志和报告 SHA。历史 FAIL、修正前报告和历史产物也保留，最终索引只引用此次固定 v8 产品和加强后原生观察器的实际结果。

## 最终证据索引

`G5` 是一个包含5个真实 Instance 的完整场景；`REG36-*` 共36检查，其中16次产品执行、20次夹具编译。`U62` 为62安全单元及 IPC/SDK sanitizer 检查。并发、原生 UAF stress 和 IPC 时间的范围见第42、50、55行与交付报告，不能扩展为未执行的保证。

| 标识 | 真实报告 | 状态 | 报告 SHA-256 |
|---|---|---|---|
| N13 | `acceptance-final-v8-io-final/native/report.json` | PASS | `040132c0ee139df2d359f94c787f9d46509916eda4311f64086ed625bc9fd5cc` |
| W70 | `network/20261013-043328/report.json` | PASS | `7d9bd08bb545395e18cc7d99a571631a10e33739b225fe6a8390459cf480509c` |
| R243 | `rpc/20261013-041831/report.json` | PASS | `6b736d13b80b78fafa3b84dfdeec91ade6956c688703f40696d02a8cccf8d22b` |
| G5 | `five-module/20261013-043647/report.json` | PASS | `49112a1999ca584a946e4b3a14c475fc8efb3b1a8b88a7666e4ba3f06960cf8c` |
| A33 | `additional-security/20261013-043656/report.json` | PASS | `bc6b93c5356c5a94174442a9235e97f178c3194d2dc036253b7f1df68234a29a` |
| I16 | `ingress/20261013-043746/report.json` | PASS | `dfc28abecdd758f2f50c1453acfd0e8a22592bfea1604bb0b70c7133ef9931ad` |
| C4 | `revoke-race/20261013-043849/report.json` | PASS | `990d0efd0f389938b47cfc1da41419a276e00c6020d98a7289354241d0007bce` |
| F6 | `network/20261013-043944/flood.json` | PASS | `27ad802a55b9a5945ae1b301450753bf033fcbc98292bfc9e36d2c62501bd3fa` |
| P18 | `network/20261013-044006/payload-security.json` | PASS | `20080dbbb505817c43a2a9f0dae228131639db8ad1b9f47885465d814257ec86` |
| T24 | `network/20261013-044110/performance.json` | PASS | `8985119b3ca34985cd3383687a1506e85b93045d5e1577123c3cce8375b7eda0` |
| WS-ALIAS | `reports/workspace-alias-v8.json` | PASS | `5d13a385720d474f338c79fcc45548251cbe173ffe784b3a7db846ad6d54d42f` |
| B4 | `product-final-v8-io/results.json` | PASS | `7b1bbbf8a3218c373c1a9eb1e56c8a090fd0517a01486819514c22aba31cb812` |
| CLI590 | `reports/final-v8-cli.json` | PASS | `44f0d20ff023635de10c38099de70fc25b4ae8957be4e1ff8adca727dc0d2a08` |
| U62 | `build/unit-v8-io-sanitized/results.json` | PASS | `76217aadbd909f0683776e36422c24faccbfbf79f5629d8f81bb14b5f18568df` |
| B100K | `reports/capability-benchmark-v8/report.json` | PASS | `a113c25e874e103909fd90b6d3cc1e945e23938f2c37ff36333186faeead1bab` |
| SC63 | `build/schema-v8/report.json` | PASS | `b51bf3bd9b3146e65e20a982343e13ef8912dc5f246562a03caba9fe6fe395c5` |
| SP2 | `reports/array-v8-portability/report.json` | PASS | `8d24ce5e15fc7803f664ffaffdeabdd135ebf1b9acbec2a356ff3345fed7086f` |
| BC4 | `reports/final-v8-compiled-backend-cli.json` | PASS | `4292cc1a0cd5629c652795d03c37d6ad5c502f6666287e51f0d2bea1dada89f6` |
| REG36-none | `reports/final-v8-regression-none/results.json` | PASS | `327a9354f8a85ea7fdd76507315c7c9daa9b1cf169c037190944082500739b1c` |
| REG36-ros1 | `reports/final-v8-regression-ros1/results.json` | PASS | `04356d4d77a488a42b662f1985a328f2cc3bc8df482b91d494f38830e5dd5d9b` |
| REG36-ros2 | `reports/final-v8-regression-ros2/results.json` | PASS | `feb22edf21d5d1a51803b01247cdf00ae1bbd7fbf95efb0a825aa6d25373f961` |
| REG36-dual | `reports/final-v8-regression-dual/results.json` | PASS | `9714c9379762d6e92262ebc1c2dec508792b7ab4b2a4ece037c650ad520f53a6` |
| GW18 | `gateway/final-v8-io-provenance/gateway-frozen-manifest.json` | FROZEN | `8ee6894adc9b42e43232a9796dae039dfc2e3c356fe68295b4df59cc05d41ad4` |
| SAN | `build/unit-v8-io-sanitized/frozen-inputs.json` | PASS | `2511d5afd593610885fe8166514e736145263aa022a88cca8cb0cd09c3806ca2` |
| CORE | `reports/final-v8-core-inputs.json` | PASS | `020c26ed9e62355160bafd6e513118f937503204d93e6ea87e0587eaba3a58b6` |
| HISTORY | `reports/acceptance-history-v8/manifest.json` | PASS | `00acef686159621d1c526d836ee5b18b12e4c8c830ab67575f9dec41b49e51d9` |
| IOG | `reports/io-golden-v8-final/report.json` | PASS | `24eb72795554cd84e41cf81291cc613c274d6400b4b95ce510daf8c2811833a3` |
| NIO | `reports/native-io-v8-golden/report.json` | PASS | `a2b2aea88d6745cf522020f7f849321faeff2d145748cbd4b2b41c6518976e3b` |
| IOG-INPUTS | `reports/io-golden-v8-final/frozen-inputs.json` | PASS | `8f04c811286e871db9ea9b5ab3c82578bb91884479f1ebb00b8c66c9c9e39a3e` |
| NIO-noetic | `reports/native-io-v8-golden/noetic/report.json` | PASS | `011864c331c3590707ccc04f9585e91617b7f2d8ef821997515d87f6d8621e20` |
| NIO-noetic-INPUTS | `reports/native-io-v8-golden/noetic/frozen-inputs.json` | PASS | `873a0ebfcb9f47516913c100ae6973341132ff1f2ca2f7ad9a9d31037f501c89` |
| NIO-jazzy | `reports/native-io-v8-golden/jazzy/report.json` | PASS | `152c30f3865a708bf326a373b9cde69c1df422210b643e5c3f789fa89547d91e` |
| NIO-jazzy-INPUTS | `reports/native-io-v8-golden/jazzy/frozen-inputs.json` | PASS | `7f66cf2a5deaa8b1d900c2ee0654a55fad53ab8aea5542aa8bd4fcd4316bde61` |
| IOFA | `reports/io-golden-v8-final/format-allocation/report.json` | PASS | `42f4ffb36b86d3a42d922e2e842457ab9947fafeb6c2bf5af262ce23164c26e5` |
| IOFA-INPUTS | `reports/io-golden-v8-final/format-allocation/frozen-inputs.json` | PASS | `98b6f5e3a5d95ee0feff93596ecf768a326394e3a617c64eb7e113052dd9c4b1` |
## 原始 55 类攻击

| # | Original attack | Status | Exact test/fixture and native evidence |
|---:|---|---|---|
| 1 | 无授权 Topic Publish | PASS | W70 `topic_denied`、普通/尾调用代理均13；同时运行的原生观察器无消息、无 Publisher。 |
| 2 | 无授权 Topic Subscribe | PASS | A33 `topic_subscribe_denied`：publish-only handle 订阅13；原生发布者活动，Subscriber 数量0。 |
| 3 | 无授权 Service Call | PASS | W70 `service_denied`：advertise-only handle 调用13、输出哨兵不变；活动的官方 Trigger Server 实际 callback 数0。 |
| 4 | 无授权 Parameter Get | PASS | A33 `parameter_get_denied`：set-only handle 读取13、输出不变；实际 ROS1 Master getParam / ROS2 GetParameters callback 数0。 |
| 5 | 无授权 Parameter Set | PASS | R243 `parameter_set_denied`：13、输出不变；独立原生读取仍为 hello。 |
| 6 | 无授权 Action Goal | PASS | W70 `action_denied`：非 send-goal 授权13；活动的官方 Fibonacci Server 实际 Goal request/accepted 数0。 |
| 7 | 无授权 Action Cancel | PASS | A33 `action_cancel_denied`：cancel-own 与 cancel-any 均13；真实活动 Goal 的 cancel callback 数0，正常结果成功。 |
| 8 | 无授权 TF Lookup | PASS | A33 `tf_lookup_denied`：broadcast-only handle 查询13；原生持续发布 Transform，未经授权的 TF Subscriber 数0。 |
| 9 | 无授权 TF Broadcast | PASS | R243 `tf_broadcast_denied`：lookup-only handle 广播13；原生观察器收到未经授权 Transform 数0。 |
| 10 | 无授权 ROS Graph 查询 | PASS | W70 `graph_denied`：错误 Graph operation13，输出和长度哨兵不变，无 Graph 数据披露；原生环境同时保持活动。 |
| 11 | 伪造 Capability Handle | PASS | W70 `handle_security`：伪造 handle9；无原生消息、无 Publisher。 |
| 12 | 旧 Handle 重用 | PASS | W70 `handle_security`：close 后旧 handle9、reopen 为新 generation；revoke 后旧 handle9、别名重新 open125；无原生发布。 |
| 13 | 跨 Instance Handle 使用 | PASS | W70 `cross-module-handle` 与 G5：实际非零 live handle 转交另一 Instance 后9；观察器无对应原生消息。 |
| 14 | ROS1 Handle 用于 ROS2 | PASS | W70 `cross-backend-handle`：实际 ROS1 handle 传给 ROS2 返回9；双后端无对应发布。 |
| 15 | ROS2 Handle 用于 ROS1 | PASS | W70 `cross-backend-handle`：实际 ROS2 handle 传给 ROS1 返回9；双后端无对应发布。 |
| 16 | Module Name 伪造 | PASS | A33 `forged-module-name`：启动126、准确 absent-module diagnostic；Guest sentinel201 未进入、原生无发布。ABI 无可自行声明模块名的参数。 |
| 17 | Module Group 混淆 | PASS | A33 多 Group 绑定启动126；Global+Group 不继承测试 open13；G5 同一 Group 的两个真实 Instance 各自成功2次、各自第三次11，原生总数4。 |
| 18 | ROS1/ROS2 Group 混淆 | PASS | A33 `backend_group_confusion`：同名 ROS1/ROS2 Group，ROS1 open0、ROS2 空 Group open13；无越权原生发布。 |
| 19 | WASI/ROS Group 混淆 | PASS | A33 `wasi_group_confusion`：同名 WASI Group 的 proc_exit0 不授予 ROS 权限；空 ROS Group open13、handle 哨兵不变、原生无发布。 |
| 20 | 多模块间权限代理攻击 | PASS | W70 `cross-module-proxy` / `cross-module-tail-proxy`：依据解释器实际调用祖先13，原生无消息/Publisher。LLVM ROS 执行另列 UNSUPPORTED。 |
| 21 | Topic 名称规范化绕过 | PASS | A33 `normalized-resource`：含 /../ 的资源启动126、准确策略 diagnostic、Guest 未执行，原生无发布。 |
| 22 | Relative Name 绕过 | PASS | A33 `relative-resource`：相对资源启动126、准确策略 diagnostic、Guest 未执行，原生无发布。 |
| 23 | Private Name 绕过 | PASS | A33 `private-resource`：~output 启动126、准确策略 diagnostic、Guest 未执行，原生无发布。 |
| 24 | Namespace 绕过 | PASS | A33 `namespace-bypass`：非规范 namespace 启动126、Guest 未执行，原生无发布。 |
| 25 | Remapping 绕过 | PASS | A33 `remapping-resource`：含 := 的资源启动126、Guest 未执行，原生无发布。 |
| 26 | 错误 MD5 | PASS | A33 ROS1 `wrong-md5`：启动126、准确 identity mismatch diagnostic；Guest 未进入，原生无发布。 |
| 27 | 错误 ROS Type | PASS | A33 两版本 `wrong-ros-type` / `cross-version-type`：启动126、准确 adapter diagnostic；Guest 未进入，原生无发布。 |
| 28 | 错误 ROS2 Type Support | PASS | A33 ROS2 `wrong-typesupport`：启动126、准确 generated identity diagnostic；Guest 未进入，原生 DDS 无发布。 |
| 29 | 错误 Serialized Message | PASS | P18 非法 String 长度和 overlong UTF-8 均22；N13 直接 IPC 的0xff 也22；持续活动原生观察器无消息/Publisher。 |
| 30 | 消息字段验证绕过 | PASS | P18 Bool2、截断 Twist、错误 Image 长度22，超出显式 Twist bound13；无原生消息/Publisher。 |
| 31 | NaN | PASS | P18 实际 Wasm Twist NaN22；原生无消息/Publisher。 |
| 32 | Infinity | PASS | P18 实际 Wasm Twist Infinity22；原生无消息/Publisher。 |
| 33 | 超大消息 | PASS | P18 规范 String4097字节超过4096上限90；W70 UINT64_MAX length90；N13 原生输入65字节/cap64丢弃后poll11、合法消息仍可用。ROS 库反序列化前分配硬上限另列 UNSUPPORTED。 |
| 34 | 超长资源名称 | PASS | A33 `overlong-resource`：1025字节资源启动126、准确 diagnostic、Guest 未执行，原生无发布。 |
| 35 | Offset Overflow | PASS | W70 `memory64_bounds`：UINT64_MAX offset、末页跨界22；原生无发布。 |
| 36 | Length Overflow | PASS | W70 `memory64_bounds`：UINT64_MAX length90、非法输出 offset22，输出哨兵不变，原生无发布。 |
| 37 | Memory64 Offset 错误 | PASS | W70 实际启用 memory64 的 Guest 使用错误64位 offset，精确22/90；原生无发布。 |
| 38 | Memory Growth 后使用旧指针 | PASS | W70 `memory64_growth`：实际 grow 成功，新页数据通过重新解析的当前内存视图发布，原生收到精确内容。ABI 使用 offset/length，不保留 Guest 指针。 |
| 39 | Publisher Flood | PASS | W70 `publish_flood`：100次 Guest 调用，burst2，98次11；原生实际消息数2。 |
| 40 | Service Flood | PASS | F6 Service：100次 Guest 调用，2次成功/98次11；官方 Trigger 实际 callback 数2。 |
| 41 | Action Flood | PASS | W70 `action_flood`：100次 Guest 调用，2次成功/98次11；原生 Fibonacci Goal 数2。 |
| 42 | Event Queue Flood | PASS | N13 原生 ROS 20条输入、depth2/128字节预算；drop-oldest 精确保留最后2条，drop-newest/reject 保留最初2条，随后poll11。使用真实原生 ROS→认证 IPC，不计作恶意 Guest emitter 或 middleware 精确丢包率。 |
| 43 | Logging Flood | PASS | F6 Log：100次 Guest 调用，2次成功/98次11；原生 logger 精确2条唯一 allow_flood 记录。rosout 被禁用，此项不声称 DDS rosout 发布。 |
| 44 | Graph Query Flood | PASS | F6 Graph：2次精确且经 scope 过滤的原生 Graph 响应、98次11且输出/长度哨兵不变，原生节点持续活动。没有独立统计全部 Master/DDS query 次数。 |
| 45 | Gateway 断开 | PASS | W70 `fault`：双方向实际 SIGKILL 一个 Gateway，失败后端107/输出不变；存活后端仍实际发布精确数据。 |
| 46 | Gateway 恢复 | PASS | W70 `recover`：双方向实际重启 Gateway、新 PID 认证与完整 session.bind 成功；原生通信恢复、另一后端持续可用。N13 旧 Service/Goal ID 重启后9，新请求正常。 |
| 47 | 权限撤销 | PASS | C4 真实 Manager→IPC lease 持有至原生确认，revoke barrier 等待已准入操作；之后100次 Manager9/100次 Gateway125，原生计数在30次采样中不增长。实际双后端 Guest 撤销后 revoked1/survivor3，进程仍存活。R243 ParameterEvent 旧 handle9。 |
| 48 | Wasm Trap | PASS | I16 实际 unreachable 导致 SIGILL(-4)；Trap 前原生 Publisher/Subscriber/Trigger 和 hello 存在，随后三种 endpoint 连续10次采样均消失；Gateway pidfd 无外部 kill 自行exit0/socket删除，观察完成后才测试清理。 |
| 49 | Callback Race | PASS | N13 每后端20轮 Topic callback teardown，ROS2另10轮活动 Action feedback close/revoke 和超时晚 Goal-response；9/125/110正确；Topic subscriber endpoint 消失，Action 实际 native cancel 被观察且无晚 feedback 交付，未统计全部 Action endpoint 消失。 |
| 50 | Use-after-free | PASS | N13 同上真实有界 lifetime/late-delivery 压力通过；U62 Manager/codec/IPC/SDK ASan/UBSan 通过。原生 Gateway/ROS库未加 ASan/UBSan，此项是压力观察的 PASS，不是所有 UAF 的形式证明。 |
| 51 | 通过 WASI Socket 绕过 ROS1 | PASS | I16 ROS1：继承 socket stdio、Global/Single/Group socket ingress、缺少 noinherit、真实 credential directory 均启动126且 Guest 未进入，原生无发布。 |
| 52 | 通过 WASI Socket 绕过 ROS2 | PASS | I16 ROS2：同样严格 ingress 启动126且 Guest 未进入，原生 DDS 无发布。 |
| 53 | 通过 Trusted Plugin 路径混淆授权 | PASS | I16 实际 TrustedDL plugin 与开启 ROS 混用启动126、Guest 未进入、原生无发布。Native initializer 属 TCB；该 guard 不回滚或 sandbox plugin initializer。 |
| 54 | 通过 IPC 消息伪造身份 | PASS | N13 活动 String/Twist 订阅计数始终0且并发 Graph 采样无 Publisher；真实同UID不同PID拒绝、未 bind/错误 manifest13、伪造 server-issued session 关闭连接、非法规范消息/UTF-8/NaN拒绝。UID相等是正常连接的实测条件，错误UID分支未单独攻击；raw framing/header长度和超时由协议单元验证，W70/P18另验真实Guest长度与载荷。不能把协议单元的所有帧攻击写成原生副作用测试，也不把所有断连统一虚称某个错误码。 |
| 55 | 通过并发请求制造授权状态竞争 | PASS | C4 四个真实 C++ contender threads、100次 Manager竞争准入，获准请求经真实 IPC/native receipt 验证；另100次撤销后 IPC125，原生计数在revoke后30次采样不增长；T24实际认证 IPC Trigger concurrency4、无错误。当前没有 Wasm线程导入，此项明确使用原生 C++线程。 |
