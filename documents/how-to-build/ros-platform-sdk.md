# Separate SDK and VM repositories

[UlteSoft/uwvm2-ros-sdk](https://github.com/UlteSoft/uwvm2-ros-sdk) provides
language bindings, robot platform SDKs, independent Xmake examples, precompiled
toolchains, Wasm language runtimes and bilingual development manuals.
This repository provides the uwvm2-ros virtual machine.

Clone the SDK with Git LFS enabled and follow its
[installation instructions](https://github.com/UlteSoft/uwvm2-ros-sdk#obtain-the-complete-sdk).
Set `UWVM_ROS_SDK_ROOT` to the SDK checkout. The include, src, lib, share, tools,
example and document directories are directly beneath that root. This VM
checkout's `sdk` directory contains only a migration README.

A qualified example uses its own `xmake.lua` to build a Wasm program with the
precompiled SDK inputs. Building that program does not require an installed
uwvm2-ros. Running it requires the VM and the example's explicitly configured
WASI/ROS permissions and native resources. Consult the SDK's
[English review](https://github.com/UlteSoft/uwvm2-ros-sdk/blob/main/document/REVIEW.en_US.md)
or [Chinese review](https://github.com/UlteSoft/uwvm2-ros-sdk/blob/main/document/REVIEW.zh_CN.md)
for the actual qualified scope.

## VM build and vendor selection

The VM's ROS import and vendor selection options remain in this repository's
root `xmake.lua` and `xmake/ros_vendors.lua`. See the
[Linux build guide](linux.md) for explicit `import-ros1`, `import-ros2`,
`ros-vendor-*` and registry-header configuration.

Vendor selections require the coherent reviewed registry and matching native
provider built from the same interface definitions. These are explicit source
inputs; the VM does not silently select an SDK checkout or grant ROS authority.
Native Gateway sources are maintained in the SDK repository and their build
commands use `UWVM_ROS_SDK_ROOT`.

## Regression tests

Maintained VM regression sources remain under `test`, including
`test/0073.ros-tests`. Test/tool output directories have local `.gitignore`
rules. SDK orchestration, private candidate deliveries and historical SDK
evidence moved to the SDK repository's ignored `.tmp` directory.
