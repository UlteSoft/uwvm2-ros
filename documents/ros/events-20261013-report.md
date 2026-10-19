# ROS 2 参数事件、静态 TF 与 IPC 编码补充交付（2026-10-13）

本轮在既有冻结版本上新增 String、ByteArray、BoolArray、Int64Array、Float64Array、StringArray 六种 ROS 2 typed parameter events，保留 Bool/Int64/Float64 的 tag1..3 和旧 String event wire。当前九种 tag1..9 与 ROS 2 ParameterType 精确对应。ROS1 typed events 与 int32 event 仍拒绝，不从普通 get/set、legacy events 或 metadata grant 推导订阅权限。

## 实现边界

新增类型/identity均需显式选择：`builtin/param-event-<suffix>-v1` / `builtin:param-event-<suffix>-v1`。数组还必须配置 `max-parameter-elements`，数值元素必须同时配置 inclusive min/max。Int64 用整数比较，覆盖超过2^53的边界；Float64 所有元素必须 finite；canonical String 可含NUL，canonical StringArray拒绝NUL并逐元素验证UTF-8。Worker与C++17原生Gateway的条件字段和队列额度规则一致：完整事件额度不能超过queue bytes或bandwidth，burst不能超过QoS depth。

原生NUL边界限定于实测Jazzy `rmw_fastrtps_cpp` / FastCDR：同一独立原生ELF发送普通UTF-8 String（8B）后精确回收；发送含NUL的scalar String在序列化时抛错（rc255）；StringArray末元素发送`a\0b`（3B）则被原生take为`a`（1B，rc0）。Gateway只能检查收到的合法前缀，不能还原或拒绝中间件已经丢弃的发送字节。SDK/IPC的canonical StringArray NUL拒绝单元保留；Native/Wasm矩阵移除此网络拒绝用例，最终70 cases。不扩展为所有RMW的结论，也不把原诊断aggregate FAIL改成PASS。

`max-parameter-bytes` 对完整 `23 + node + name + payload` envelope 生效，先检查完整 byte/element/UTF-8/数值额度，再复制借用原生值。typed encoder直接写入一个最终buffer，避免临时payload复制。删除事件由官方 ParameterEvent 中带旧值的 deleted_parameters 显式夹具验证；自动外部Node验证的是六种新增类型的声明与设置，不宣称远端 undeclare。删除保留tag、outer payload长度0；Gateway仍检查借用旧值的type、element quota、canonical hard payload size、UTF-8/finite及bounds，不复制旧数组。删除的delivery quota针对实际空payload envelope，合法旧值超过新值delivery quota不因此抑制删除。

静态TF同一合法frame pair保留一个最新值。同stamp更新数值会替换旧值；全部wire字段相同的重复值不追加历史；double以wire bits比较。动态TF重复stamp规则、显式frame/value/rate/byte/queue限制均沿用原规则。独立真实Jazzy DDS复现：原生BufferCore在stamp0:0从x=1.25读到2.5，旧adapter仍1.25；修复后两端均2.5。Noetic验证是共享header的C++17单元，不宣称ROS1网络复现。

IPC新增独立命名request_view/encode_request_view，borrow metadata/payload直接编码；保留owning API与braced调用，保证非法metadata不改输出、请求ID/认证/错误/deadline语义不变。C++17 Gateway和SDK compatibility paths沿用借用views与已有native I/O；C++20/26可用路径使用vendored fast_io。`native_file_loader`不适合零stat size的/proc文件，实际测试collector采用fast_io ibuf_file+scan读取cgroup。

## 实际验证

- parameter event IPC：最终4个ASan/UBSan配置（C++20/C++26 × libc++/libstdc++），各630 checks，9条实际命令，原报告SHA `a01f3d87f2c91cacaa1eb6b524c5731a3500f4bb927567c73e775276f770034b`。新增54 checks覆盖九种tag的queue bytes、bandwidth、burst，在Worker和Gateway一致拒绝。独立5845项PASS（SHA `77a77222dce75b3c18ce9b90b707c87a375cc4e40e4ec5ad2136dd258a9ebb9c`），405 lexical paths归并392 canonical files，无SHA冲突；实际独立verifier SHA `2eeb3cc4b9f939ed52d9538e8476e3adbad4b41e0973dabe555c25c326175111`。原r1仅Worker+wire，各565 checks；r2实际捕获Gateway array event元素额度字段遗漏而FAIL；r3各576 checks、独立5835项PASS，仍作为修复过程保留。
- SDK codec：normal/SAN运行各13组，实际driver4命令；独立核验792项。保留早期fast_io字符span调用、/proc零stat读取和缺header导致的测试失败，不改其原报告。
- request view：4个SAN配置各98 checks，实际authenticated Unix会话验证有效请求allocation3→1，非法metadata2→0，同会话后续有效请求3→1。输出与旧owning编码逐字节相等。独立核验280项；没有吞吐提升断言。
- TF：Jazzy真实DDS前后对照与两发行版共享header86 checks；独立核验4397项。第一次修后语义PASS但aggregate因短unit缺实际main proof仍FAIL；增加观测窗口的新attempt补齐实际ELF/PID证明，原失败保留。
- fresh integration-r4：四Worker none/ros1/ros2/dual 与18原生目标（ROS1 8、ROS2 10）全部重新构建；dual SHA 88eff21e009d0611d604fe21c4160e28845b1b93514b82e8e9da6b36dee49d30，Gateway SHA d7fdb5db0383eacde5ffada61e4939349c3f6ed70d6ae2c82bb47d2b447bf82f。完整A15 15/15 PASS，实际 1487.709s；775项CLI通过。主aggregate原报告SHA 219823d30475e8316517a88bc32b9b3a0318e347b53b4bb3fa478677fe71e1bf。
- 新增原生参数事件矩阵：positive78、rejection batches52、automatic6、queue overflow18 全部PASS；使用上述新Gateway及冻结夹具，旧native-smoke-r5提供独立5113项证据作为历史对照。
- 最终参数化入口r2：70份WAT真实编译，70项Wasm全PASS，285条实际命令；212份实际main记录（native2，加70×Gateway/Worker/witness）完整。实际aggregate SHA 5defd6a36c76dd43feed82b20341bd1562624f46b41b2f7c64c792aa898cb55e。Worker/Gateway为同一fresh-r4二进制；native两目标MMD绑定389项目输入。
- 主线独立82,745项PASS（实际输出SHA b18f78a10f30f2d312fb925781508ebf9534e5f697b96e6a0ae228ea8137c4c2，实际verifier b1966c8e…）；union实际38,891项PASS（原输出SHA 99cbaad6f1e4b981b9795e8d14b390d16f85f58e53d6aa18ce7c9a60f0717914）、local current272项/66限定路径PASS（a98842317cc6e557f5d7eb95c96b72c93676b6bc708e6edc7d71f65865d5f58c）；当前42文件另50项绑定PASS。实际union工具21461b18bdedca1a766175fd5346594e760cda9a8ad7f0066552ce0642310dc2。验证工具不重新执行ROS；首union因历史中断error完整字面断言错误而FAIL（b801a91d…），原输出与0b2e工具保留，修正后独立新attempt通过。

## 环境、失败与保存

既有SSH linux/容器4dd58ad4560f、/host、Ubuntu25.10、CPU32。有效cgroup64GiB=68719476736；root不可读memory.max/events保留None，不冒充0。所有可读祖先的OOM/kill/groupkill增量逐次校验。Clang23.0.0git commit4c4c1db7c69a6fda6cfa6bc6066bb09a433edc89；Noetic GCC9/Jazzy GCC13原生CPP17。

integration-r1编译前budget返回77，current45758812160 bytes、jobs0、没有compile；原报告保留FAIL。单独对既有容器cgroup请求memory.reclaim 12GiB清理可回收缓存，before45728514048→after32334794752，限额不变、OOM增量0。integration-r2遵守原8GiB/compiler估计和16GiB reserve，实际jobs2重建四Worker和18native，A15的三条旧CLI断言仍预期“数组事件不支持”而FAIL；本次明确停止已知失败aggregate并完整清理，不称A15通过。

CLI重写保留原590项，并新增185项，合计775。首次独立预检原FAIL保留：20项拒绝行为正确但诊断片段断言过宽；另外6项揭示Worker未执行Gateway已存在的event queue额度冲突校验。修复生产schema后冻结candidate3并重新构建四Worker和18native；最终不复用旧production-input二进制。共享cgroup累计peak/current不作为本轮独占峰值。

integration-r3的ROS2 `runtime.o` 编译在Clang的MachineDCE/LiveRegUnits代码生成阶段收到SIGSEGV（compile rc-11），另外三个配置通过，原aggregate SHA `83bfdfe4bd0616c948f35ba5e670a4c22a96a5f5a64a5964841be07d3da59a35` 保持FAIL。可读祖先OOM增量0、两SDK私有mount第一次清理均0且无剩余mount。原回溯、24,788,290B预处理CPP（SHA `c0cff6881f06b62572d5e5e08b4e0a8a73f1c908c7a70014adf42b3a6bb19d80`）及5,477B重现shell保存。最终integration-r4使用同一candidate3源码、同一Clang和原优化选项重新构建全部四配置，不从此失败aggregate补写PASS，也不推断编译器崩溃根因。

新增参数化入口首轮 aggregate 实际FAIL（SHA f237b17a217a6fab1b308cea2bd659876e30c355885b815a2cddaf88eb31df82）：native矩阵、70份WAT编译及前9项Wasm均通过，第10项短Worker已rc0，但实际main证据在ELF哈希完成前退出而漏采。保留105条真实命令及原失败；仅将测试执行器run_native.py从34b468…改为f1d914…，先记录live PID/cgroup/PPid/FD inode再哈希同一打开的ELF，缺main仍FAIL。最终入口从头重跑285条命令，70项Wasm均PASS，不改生产/SDK/夹具或内存/编译选项。5秒采样窗口不提供同步FD哈希期间硬中断保证。

主线冻结W6+显式37-file overlay（完整candidate3 source4115 files）；当前工作区有无关用户修改，本轮仅核验ROS生产/SDK、实际消费的新测试和维护文档。早期DDS fixture发生fast_io raw char*编译错误，独立revision修正类型适配；随后原生scalar NUL序列化异常及StringArray NUL归一化暴露两项上游边界，所有原失败报告和输入均保留。最终main overlay与参数化Native/Wasm入口使用同一fixture SHA；实际MMD证明其消费范围。后置四份维护文档的三revision逐字节/diff连锁保存，并新增参数事件测试README。当前42文件清单绑定36个原冻结overlay、唯一实际执行工具修订和五份维护文档；旧37-file overlay仍保留原执行器字节，独立验证证明其不在主线CPP MMD或A15执行路径中。

主integration及最终70项入口均完成私有mount/rootfs/short-alias清理，首次umount返回0，剩余自有进程/挂载为空；源、production ELF、输入manifest保持。两个可读cgroup祖先的oom/oom_kill/oom_group_kill均delta0；不可读root数据保持None。

31份之前正式报告、验收JSON、索引、归档及脚本逐SHA保护；旧v8/followup/continuation/next报告均是其冻结版本的历史事实，完全不改。维护文档增加本轮类型和边界。历史test-report§22里“参数数组事件及其余typed事件未支持”仅被本轮针对ROS2九种event实现与实际证据替代；其余限制继续适用：任意自定义Schema、开启ROS的LLVM/named-module/非LinuxGateway、原生反序列化前逐消息硬内存上限、任意QoS排列、整体原生库sanitizer/全部历史平台测试、远端declare/undeclare等均无本轮实现/验证声明。

## 交付与复现

| 文件 | Bytes | SHA256 |
| --- | ---: | --- |
| [events-20261013-acceptance.json](events-20261013-acceptance.json) | 4861129 | `219823d30475e8316517a88bc32b9b3a0318e347b53b4bb3fa478677fe71e1bf` |
| [events-20261013-validation.json](events-20261013-validation.json) | 1615169 | `612f19344fe74982a8a76b606a84fc961783895c7c4daf38319ab99ccae25b62` |
| [task-files-events-20261013.json](task-files-events-20261013.json) | 9520 | `6df72eff628ac136271c93763fadf0b891cd59ccc3cd0cd93ea836326506cd72` |
| [evidence-20261013-events.tgz](evidence-20261013-events.tgz) | 47784834 | `6934f28d18a7916acdcc1d37adf160b3149683b416a1a90cff5c25aa20f5e86c` |
| [events-20261013-evidence-index.json](events-20261013-evidence-index.json) | 8817136 | `b6a7dec42e69a31f9390d47f77bb13e39fbfe99fb6aebc7b268b8de8e063939e` |
| [verify-events-20261013-archive.py](verify-events-20261013-archive.py) | 11483 | `e1d793794b07b614dbe258f239d8ea5464e8b9df704491403737d2a0e55721f2` |
| [events-20261013-archive-validation.json](events-20261013-archive-validation.json) | 60176 | `0aba1b4357869fc4fc07603d424dca9a6ca84b040a3f818372e28c0e8018a05c` |

acceptance 为远端主aggregate逐字节副本；validation 仅封装四份实际独立证明，逐份保留原输出SHA。当前42-file manifest和两份实际入口版本均有明确字节绑定。新report、外部index、外部archive verifier/validation均不作为本归档member，避免自引用。

归档来自3686个显式映射，包含13840个UTF-8 regular members，payload 349,432,206B；生成后全部member复读核验。下载SHA/size与远端相同。独立归档实际110,086项PASS，完整gzip trailer/固定uid,gid,mtime0/mode0644/精确member集合与34个强SHA证据anchor均通过，当前42文件与31历史正式文件保持。SDK/rootfs、ELF、生成Wasm、对象文件、unsupported extensions/private paths不在payload；skipped记录在外部索引，无nested archives。

从项目根复核下载归档（`--out`必须选不存在的新文件）：

```sh
tar -xOf documents/ros/evidence-20261013-events.tgz current/protected-deliveries.json > /tmp/uwvm-ros-events-protected-recheck.json
python3 documents/ros/verify-events-20261013-archive.py --root "$PWD" --protected /tmp/uwvm-ros-events-protected-recheck.json --out /tmp/uwvm-ros-events-archive-recheck.json
```

新增原生和70项Wasm入口说明见 [tests/ros_parameter_events/README.md](../../tests/ros_parameter_events/README.md)。实际driver/argv/raw MMD/flags/link/WAT/profile/策略/三类进程证明、独立工具执行与失败history均在本归档中；remote durable workspace `/home/macromodel/.local/share/uwvm-ros-extension/20261013-events-integrated-r4` 和 `20261013-parameter-events-r1/integrated-parameter-events-r2` 保留生产ELF。


归档从明确输入清单生成，只包括允许UTF-8源码/策略/报告/日志/MMD/实际执行驱动与元数据；ELF/生成Wasm及SDK rootfs不打包。ELF SHA/远端路径与编译命令保留以供复现。索引在归档外，避免自引用；本地下载后逐member SHA/size/UTF-8/gzip trailer及固定元数据再独立核验。所有测试结论只对应上述实际编译输入与执行范围。
