# ROS Runtime contracts / I/O delivery — 2026-10-13

本轮完成独立 ROS2 删除通知、Service 响应期限检查，以及 String、SDK 编码和审计 I/O 优化。四种新 Worker、18 个新原生目标、完整 A15、旧70个类型事件用例和新增19个删除通知 Wasm 用例均实际通过。沿用显式 capability、独立配额、默认拒绝和既有 ABI，没有新增隐藏权限等级或权限集合。

## 实现

删除通知使用独立 ROS2 type builtin/param-event-deletion-v1 / identity builtin:param-event-deletion-v1，version2、kind3、tag0、空 payload。只匹配明确 node/name 的 deleted_parameters / PARAMETER_NOT_SET。删除前的类型、值及数值界限未知，不缓存或声称验证它们。独立订阅 grant 保留八项 QoS、审计、字段、完整 envelope 字节、队列和速率限额；拒绝数值界限、finite、元素限额，不授予参数读写/声明/删除或 Topic 权限。原 tag1..9 的 NOT_SET 拒绝及先前值验证规则不变。

响应绝对调用期限在操作入口、锁和 QoS 查找前捕获。复制响应后重新检查关闭、身份、重复回答及 pending/call 两期限；原生准备后再次检查发送准入。短调用到期保留仍有效的 pending 请求以供重试。这是准入检查；mutex、内核和 middleware 调用不可硬中断，不承诺硬返回时间或中断已经开始的发送。

String 验证不再创建 owning String；需要 decoded output 时仍创建它。生成式/rclcpp ParameterValue result helper 在配额及 UTF-8 检查前借用原生 String，成功只分配最终 wire。字段/限额使用透明 string_view 查找，避免长 key 临时 String。完整 Node RPC 和上游反序列化仍可能复制。

SDK typed event 预检完整 envelope 和所选值，一次 reserve 直接写最终 wire。成功恰好一次分配，拒绝零分配且保留输出。固定宽度 little-endian u32/u64 未改为 LEB128；kind3 不序列化或验证没有 wire 表示的旧值字段。

审计用 fast_io 格式化及适用配置的文件输出，以 pinned fast_io::posix_file 管理所有权。Linux pipe/TTY 重新打开独立非阻塞 OFD，socket 使用非阻塞 send，不改变原 stderr flags。pipe 完整记录（含 newline）在缓冲区/PIPE_BUF 内原子接受或整条丢弃。TTY/socket 错误或部分写可留下部分输出并计 dropped；regular file 保留原 OFD offset/append，错误也可留下部分输出，无硬 I/O deadline。保留 SIGPIPE 状态与 caller errno；饱和 delivered/dropped 计数仅供可信运行时诊断，无 Guest ABI 或额外无预算日志。拒绝审计 sink mutex 在输出前释放。

C++20/26 使用适用的 fast_io；Noetic/Jazzy C++17 SDK 保留既有 borrowed view、native I/O / to_chars 适配，没有强制引入需要 concepts 的依赖。

## 冻结与实际测试

远程前缀 B = /home/macromodel/.local/share/uwvm-ros-extension。主线 B/20261013-contracts-integrated-r1 从不可变 events-integrated-r4 加33个显式 overlay 冻结，共4,138文件；没有吸收无关 Parser/JIT/LLVM 工作树修改。

| 实证 | 结果及范围 |
| --- | --- |
| Worker | none / ros1 / ros2 / dual，各3 compile、link、version均0；12实际MMD |
| Native | Noetic GCC9 / Jazzy GCC13，18 targets；32实际依赖文件，原生SDK C++17 |
| 完整 A15 | 15/15，包括既有CLI775、真实ROS1/ROS2/双后端/多模块网络、SROS2、资源、拒绝/撤销/Trap、配额、回归、有界性能 |
| 新删除契约 | 六个C++17/20/26 × libc++/libstdc++ SAN配置，224 checks；CLI37/37，无每CLI命令PID witness额外声称 |
| 新删除网络 | focused native：11官方SDK删除、9 prior kinds、8 rejection batches；最终19WAT/Wasm/DDS cases，81commands /59actual-main proofs |
| 旧类型事件 | 新同一GW/dual上70/70；285commands /212actual-main proofs，固定f1 runner |
| Service期限 | 原5个late-acceptance反例保留；最终ROS2六modes PASS。Noetic shared C++17检查及fresh build PASS，不追加ROS1网络声称 |
| String | 128KiB生成式入口4→1、native入口5→1分配；after全10拒绝路径0。只验证0、主动decode仍1。原before/after与新live-before-hash六SAN均保留；helpers不等于完整Node RPC |
| 审计永久测试 | 3fresh profiles /72scenarios PASS；standalone编译helper，host集成编译由新Worker MMD证明 |
| SDK永久测试 | 4fresh SAN profiles /440cases；每配置54accepted/56rejected，成功1分配、拒绝0，与旧oracle完整wire一致 |

主线实际耗时 Worker270.831s、native157.380s、A15 1484.122s。主报告SHA为3ff576750879c0aa0c96e6bf6c528a63502ba6718507e4df45cdbf023ca53885。新dual为9163252d4ee13d88c12fb3f1eccb45503e43cf8da6557f85f2a63c0dfb2f6ff4，新ROS2 GW为697c1d6ac505bd565be209b114d9247107a62ba47fdf08d35bce72869ea87d4c。

现有统一cgroup memory.max=68719476736。两个可读祖先的oom/oom_kill/oom_group_kill delta均0；根祖先不可读字段保持null，未计0。主线3,252个真实ELF PID记录绑定pinned FD、device/inode、PPid/ancestry/cgroup。三owned mount首次umount均0，owned processes/mounts空；前一source /18native binaries/report三preservation guards均true。

主线独立88,753 checks PASS；其余独立proof及准确范围见validation JSON。当前join623 checks PASS、68文件绑定；当前交付清单78文件，文档不冒称编译输入。

## 原失败与后置修订

原主线冻结源码不改。新测试最初成功bool判定写反、header包含顺序，以及String executor采样顺序，分别修正两份C++ fixture和一份Python driver，用新snapshot实际重验。七个audit/SDK永久测试文件在主线冻结后加入，各自fresh执行，没有声称被先前主线编译。current join单列三个差异的旧/新SHA、对应独立PASS proof，以及新增测试绑定。

原runtime/fixture/独立验证FAIL保持原状态及字节。历史String unit先hash后live采样如实保留；新六SAN为live-before-hash。早期unit FAIL没有postflight/OOM字段，不解释为0。官方SDK delete_parameters对动态参数的远程删除可产生NOT_SET；原Guest typed undeclare仍因缺少prior-kind/value原子条件而拒绝，新通知不改变这一权限边界。

交付工具原失败也保留：collector r1把main mapping计数当list；post-pack r1混比数据source table及其排除的两个自身metadata；下载校验首轮audit proof SHA手抄成66字符。原脚本和metadata在B/prepare-contracts-pack-r1.py及B/20261013-contracts-delivery-r2内。新只读校验修正工具判定，archive/index及runtime原结果没有重分类或改写。

工作站磁盘曾返回ENOSPC。闭合任务缓存先逐字节保存在B的local-preservation-r4/r5/r7及reclaim receipts，再移除本地重复缓存；源码及原39份正式交付未变。恢复可写后从固定远程SHA下载并回读。环境清理/tmp之后所需文件从远程恢复，复现不依赖这些临时路径。

## 归档与复现

归档24,496,705B，4,383个UTF-8 regular members /148,693,483payload B，无nested binary archive。SHA为a365b22b2eb1c6ebd01f1f12eddd8c4f97c9bc88a69cd03ba7bf319ed2ce8076。同SHA/size合并为一份payload；planning/evidence-aliases.json保留12,764条source/requested_archive/payload_archive引用，可据此恢复原树。数据source table故意排除自身两个metadata以避免循环，校验器另核两项。

alias metadata为8,905,346B，validation wrapper为8,865,421B，超过8MiB；本轮不声称所有归档成员≤8MiB。原各专项collector对其自身输入的单文件限制与这两份汇总metadata范围不同。归档不包含ELF/Wasm、rootfs或私钥；实际compiler/RMW/SDK依赖仍须按command及package inventories准备。

下载后独立PASS65,288 checks：全成员digest/size、固定tar metadata、完整gzip trailer、25硬SHA anchors、全部aliases、78当前文件及39原交付。它不重新运行ROS。

复查命令（output必须不存在）：
```
python3 documents/ros/verify-contracts-20261013-archive.py \
  --root /absolute/uwvm2-ros \
  --protected /absolute/uwvm2-ros/documents/ros/contracts-20261013-protected-prior.json \
  --out /absolute/fresh/contracts-archive-recheck.json
```

测试入口及各README见[testing.md](testing.md)。本报告独立于archive，避免archive digest自引用；其余8个正式文件如下。

| 文件 | 字节 | SHA256 |
| --- | ---: | --- |
| [contracts-20261013-acceptance.json](contracts-20261013-acceptance.json) | 5193619 | 3ff576750879c0aa0c96e6bf6c528a63502ba6718507e4df45cdbf023ca53885 |
| [contracts-20261013-validation.json](contracts-20261013-validation.json) | 8865421 | c3df08850e1865f2f624c0d84780dc854d88a59dea72f6b6ac7085f10f76c22f |
| [task-files-contracts-20261013.json](task-files-contracts-20261013.json) | 13386 | 2424c0be1f680beeff8cbbea63b27a7ebbe03a68a2917153278e07ef0007a7e4 |
| [evidence-20261013-contracts.tgz](evidence-20261013-contracts.tgz) | 24496705 | a365b22b2eb1c6ebd01f1f12eddd8c4f97c9bc88a69cd03ba7bf319ed2ce8076 |
| [contracts-20261013-evidence-index.json](contracts-20261013-evidence-index.json) | 3315084 | a9a1c310932f0e4a24cb5b79a4829ac02a158cbf02c5c6b0210cfdfc497e4b50 |
| [verify-contracts-20261013-archive.py](verify-contracts-20261013-archive.py) | 11953 | 8dc1bc20a4b56371b8ac506debc37d6bed053b3ee7e356a455831e39e0563076 |
| [contracts-20261013-archive-validation.json](contracts-20261013-archive-validation.json) | 28217 | 7f1371589580a6ec60b5d60519a0fb41b9c75773d8b5f80459aadf6c61b85d0a |
| [contracts-20261013-protected-prior.json](contracts-20261013-protected-prior.json) | 4608 | 0eabf6fbbc739506432591096a31e0ba713682b54ded7ab26d641661881343aa |

## 实际限制

适用于本次Linux、Clang23/libc++ Worker、Noetic GCC9、Jazzy GCC13及选定rmw_fastrtps_cpp。原生middleware和整个Worker没有完整SAN instrument；SAN只覆盖所列helper/codec/contracts。regular文件、mutex及middleware无硬中断保证。原Jazzy scalar String NUL FastCDR异常、StringArray NUL截断诊断保持；不承诺NUL网络互操作或其他RMW。arbitrary dynamic schema、其他已记录unsupported adapter、非Linux SDK、ROS+LLVM named-module组合不因此获得PASS或权限。
