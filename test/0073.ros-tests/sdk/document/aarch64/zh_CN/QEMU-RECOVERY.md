# 保留 QEMU 安装布局

向 test/0073.ros-tests/sdk/tools/aarch64/qemu.py 的 locate() 明确传入可执行文件或安装根目录。
根目录下可为 usr/bin/qemu-aarch64 或 bin/qemu-aarch64；必须只解析出一份
实际可执行文件，可指定精确SHA-256。定位函数不下载、不执行。
QEMU 的 ELF 反映原生宿主；ARM参与者须独立检查为ELF64、machine183。

版本命令和真正ARM ELF必须在新建64GiB、零swap容器中持全局锁运行，保存
实测cgroup及boot身份。保留哨兵不约束其他容器。版本信息可能在stdout或stderr，
两者和实际退出状态都应保留。探针只证明ARM执行，不证明ARM编译宿主或ROS通信。
