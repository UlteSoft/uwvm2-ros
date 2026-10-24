# Retained QEMU installation layout

Pass an explicit executable or installation root to test/0073.ros-tests/sdk/tools/aarch64/qemu.py
locate(). A root can contain usr/bin/qemu-aarch64 or bin/qemu-aarch64. Exactly
one resolved executable is accepted, with an optional exact SHA-256 check.
The locator performs no download or execution. Its ELF describes the native
QEMU host; inspect the ARM participant independently for ELF64 machine183.

Execute --version and an actual ARM ELF only inside a fresh64GiB/zero-swap
container under the global lease. Preserve the actual cgroup receipt and boot
identity. A retained sentinel does not bound another test container. Version
output may use stdout or stderr; preserve both and the real exit status. Probe
success qualifies ARM execution, not a native ARM compiler host or ROS exchange.
