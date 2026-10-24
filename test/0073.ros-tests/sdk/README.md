# SDK compatibility tests for uwvm2-ros

These tests verify the virtual machine's support for SDK-generated WebAssembly
programs. SDK bindings, examples, compiler adapters and prebuilt development
inputs belong to the separate
[uwvm2-ros-sdk repository](https://github.com/UlteSoft/uwvm2-ros-sdk).
Select an explicit SDK checkout and retain its revision and input hashes in
each test manifest. The VM repository does not contain another SDK copy.

- `tools/aarch64` provides the shared Linux container, native ROS witness,
  architecture, memory-width and library-metadata helpers.
- `tests/aarch64` contains metadata-admission unit tests. Synthetic headers
  test the inspectors; they do not certify language compilation or VM execution.
- `document/aarch64/en_US` and `document/aarch64/zh_CN` describe the native
  acceptance protocol and its exact scope.

Run tests on Linux in a container with an actual 64 GiB memory limit and no
swap, while holding the common test execution lease. Transfer the complete
source folder before running a batch. For the metadata checks, execute this
command from the VM repository root inside that container:

```sh
python3 -B -m unittest discover -s test/0073.ros-tests/sdk/tests/aarch64 -v
```

Check the selected SDK's backend directories, public references and frozen
Python package parents in the same Linux environment:

```sh
python3 -B test/0073.ros-tests/sdk/tools/check_backend_layout.py --sdk-root /path/to/uwvm2-ros-sdk
```

Native acceptance additionally requires exact runtime, gateway, peer, QEMU and
Wasm identities, explicit module permissions and observed ROS messages. The
[English protocol](document/aarch64/en_US/README.md) describes those inputs.
Keep raw receipts, generated Wasm and build caches in ignored output folders.
Reusable test source is checked in; a recorded passing count is not a test.

## 中文说明

此目录验证 uwvm2-ros 对 SDK 生成程序的实际支持能力。SDK 源码、独立示例、
编译适配器和预编译开发组件在独立的 uwvm2-ros-sdk 仓库维护。每批测试显式
选择 SDK 版本并记录输入哈希，不在虚拟机仓库保存另一份 SDK。

测试在 Linux 的实际 64 GiB、零 swap cgroup 中串行执行。上述命令仅检查
元数据校验逻辑；多语言编译、动静态库、真实 ROS 通信和 QEMU AArch64 执行
需要各自的完整证据。具体流程见[中文协议](document/aarch64/zh_CN/README.md)。
可复用的测试源码纳入版本管理，原始回执、生成的 Wasm 和缓存放在忽略目录。
