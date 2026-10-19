# Image 编码的借用输入与单次分配（2026-10-13）

本轮编码改动消除原生 ROS1/ROS2 Image 订阅路径中的完整像素临时复制，并对完整 wire 一次预留容量。`ipc::image_view` 借用两个 `string_view` 和像素 pointer/size；调用方须保持这些输入存活至同步编码返回。原 `encode_image(image const&)` 接口保留并委托借用编码器，现有调用无需改签名。两网关的 String/Twist 编码也按已知完整长度预留容量。

共享头继续支持官方原生 C++17。仓库 fast_io 依赖 C++20 concepts，因此这里使用借用视图及 `vector::reserve`，不增加另一个格式化框架。固定小端字段、原始文字/像素、字段顺序和规范 Bool 字节保持原样；编码器沿用调用方校验契约，未增加编码类型、IPC 额度或文字合法性限制。表示不下的完整输出在任何文字/像素访问和输出 reserve 前拒绝；此路径的 `length_error` 异常本身可能发生小额分配。

`decode_image` 和 Worker 的 Image 验证路径没有改变。本说明不声明重新构建或验收当前 Worker；原 v8 及先前后续报告/驱动/证据保持原样，新的原生通信验收由本轮独立报告记录。

新增 [image_encode_test.cc](../../tests/ros_ipc/image_encode_test.cc) 保留旧 `encode_image` 完整函数字节作为差分基准。修改前共享头 SHA256 为 `5a9b156dbe2414a6a7d6b6ff7f313d95e811430231029a489848b9c04f34c5d6`，基准函数 SHA256 为 `258eb685445222a991f06637c0158607129f42e8ceaba786a6330a7dea0b213c`。本地 `/usr/bin/clang++ -std=c++17` 的优化构建及 ASan+UBSan 构建均实际通过 **675项检查、30个逐字节差分用例**；无 exceptions 的共享编码头单独编译通过。这些本地运行不替代原生 GCC9/GCC13 或真实 ROS 通信；后续原生工具链结果记录在末段。

差分覆盖空像素与默认空/null视图、两个大小端 Bool 值、255/256/4096字节文字、嵌入 NUL/非 UTF8文字、所有像素字节、u32字段极值、IPC边界及超过 IPC 额度的纯编码输入，并检查独立固定小端 golden。完整长度超过 `vector::max_size()` 或 `size_t` 表示范围时，探针要求 `length_error` 且未尝试输出大小分配。

分配统计限定替换全局 `operator new` 的作用域计数：提前准备1 MiB像素与两个255字节字符串，仅统计原生编码表达式。旧表达式构造拥有输入的临时 `image` 后编码，实际为12次分配、请求2100139字节；新借用表达式为1次分配、请求1049123字节，恰好等于完整 wire 长度，并保持输入不变。所有差分用例的借用及 owning 编码各只分配一次完整 wire。统计不包括 ROS 生成对象、原生反序列化、分配器元数据或其他线程，也不是 RSS、吞吐量或延迟测量。

后续 r6 在原64 GiB cgroup中，以 Noetic GCC9 和 Jazzy GCC13 各实际运行同一探针，ASan+UBSan 均通过675项检查和30个逐字节差分，分配计数也均为12次/2100139字节到1次/1049123字节。独立探针以非 PIE 构建，两个 sanitizer 均保持启用；这不代表 ROS 中间件整体插桩。原始日志和真实 ROS 通信结果见[本轮验收报告](continuation-20261013-report.md)。
