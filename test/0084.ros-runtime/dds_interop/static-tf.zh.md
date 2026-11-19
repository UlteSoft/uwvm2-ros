# ROS 2 静态坐标变换互通

这个测试使用原生 C++ ROS 节点发布包含两级变换的摄像头坐标树。Jazzy 的原版
`rclpy` 和 TF2 监听器通过 DDS 接收数据。测试检查帧名、每次运行的新鲜标记、
序号、有限坐标值、四元数，以及 TF2 在指定时间上的正向和逆向坐标查询。

- Jazzy Fast DDS 和 Humble Cyclone DDS 分别向晚加入的 Jazzy Fast DDS
  监听器发送数据。发布者在监听器启动前已写入快照，使用默认的 transient-local
  订阅时，监听器必须收到该快照和后续 24 次更新。
- 两组发布者也分别与 volatile 监听器匹配。测试独立记录历史样本数，并校验
  两端实际生效的持久性配置。Humble 发布者和 Jazzy 监听器都设成 volatile
  时，须收到后续 24 次更新，历史回放为零。
- 停止 Humble 发布者后再启动新的监听器，必须观察到零发布者和零历史样本。

在指定的 64 GiB、零 swap cgroup 和共用锁内执行。使用独立的 Jazzy、Humble
安装前缀，并传入相应的系统库路径和 Noble Python 3.12 前缀。Python 前缀应
含解释器、标准库及 `lib/x86_64-linux-gnu/libpython3.12.so.1.0`。需要补充 Python
依赖目录时可以重复指定 `--python-site`。从 VM 源码根目录执行以下命令。

```sh
"$QUALIFICATION_GATE" /usr/bin/python3 -B test/0084.ros-runtime/dds_interop/run_static_tf.py \
  --jazzy-prefix "$JAZZY_PREFIX" --jazzy-system "$JAZZY_SYSTEM" \
  --humble-prefix "$HUMBLE_PREFIX" --humble-system "$HUMBLE_SYSTEM" \
  --python-home "$NOBLE_PYTHON_PREFIX" --python-site "$PYTHON_DEPENDENCIES" \
  --output "$NEW_DIR"
```

输出目录必须是项目 `.tmp` 下的新目录。编译使用 `-O3 -g`。另起一次测试并加
`--sanitized` 可以对 C++ 发布者运行 ASan 和 UBSan。性能使用未插桩构建。
晚加入耗时从创建监听器到首次校验通过的坐标查询。实时更新耗时从发起发布到
监听器完成校验和两次坐标查询。百分位值去掉前四个预热样本。回执记录两端
实际加载的中间件库、消息类型库和 TF2 模块摘要。

本测试范围是私有 IPv4 回环中的明文 DDS 与指定的原生客户端。实际机器人、
Wasm 权限、DDS 加密和硬实时需分别验收。

优化构建完成六个场景。Humble 的 Cyclone DDS 0.10.5 发布者使用
transient-local 时，Jazzy Fast DDS 2.14.6 的 volatile 监听器仍在首次实时
发布前收到了加入前的历史样本。Jazzy 同栈的 volatile 监听器和混合栈的双
volatile 对照均未收到历史样本。该混合组合的观测与 ROS 2 QoS 文档不同。
要求数据新鲜度的应用应独立校验时序。默认 transient-local 监听器下，Jazzy
同栈和 Humble 到 Jazzy 的稳态发布至校验查询耗时 p50 分别为 202.7 µs 和
166.4 µs。每组有 24 次实时更新，混合组发布端各记录两次 rcutils 诊断。

[原版 Jazzy TF2 监听器](https://github.com/ros2/geometry2/blob/jazzy/tf2_ros_py/tf2_ros/transform_listener.py) · [English](README.md#static-tf-with-a-late-joining-rclpy-reader)
