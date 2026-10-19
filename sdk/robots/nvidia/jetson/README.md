# Jetson Orin / Thor deployment boundary

Jetson is the native deployment platform for selected perception/model/Runtime
processes. It is not another robot driver. The official NVIDIA module lineup
contains distinct AGX Orin, Orin NX/Nano and AGX Thor variants; the release38.4
Quick Start provides a versioned BSP installation context. Those website bodies
are timestamped primary documentation observations, not Git/SDK/board proofs.
No flashing, SDK Manager, container, driver, power/clock or device operation was
run here. Do not infer a universal module/carrier/JetPack/CUDA compatibility matrix
from the words aarch64 or Jetson. [Official module source](https://developer.nvidia.com/embedded/jetson-modules)
and [versioned BSP source](https://docs.nvidia.com/jetson/archives/r38.4/DeveloperGuide/IN/QuickStart.html).

A real deployment starts by independently selecting the exact module SKU,
carrier, boot firmware, JetPack/L4T/BSP, kernel, driver/CUDA/VPI/TensorRT/Python
artifacts, build ABI, model weights and ROS release. Record actual hashes and
full package/loader closure; a vendor name or power/performance headline is not
compatibility proof. The native detector and GR00T processes stay on this trusted
host, bounded by explicit CPU/memory/time ownership and their own real cleanup.
The Wasm Guest sees only the same canonical ROS imports described in the root
lesson. It receives no /dev, CUDA pointer, native network, flashing, root shell,
clock, GPIO, policy mutation or model-path authority.

`admit_platform.py` is actual read-only source. With its explicit enable flag it
requires aarch64 and reads two selected ordinary nofollow/nonblock files:
for example the physical device-tree model property and /etc/nv_tegra_release.
Pass independently captured exact expected SHA values; it compares whole counted
bytes including final NUL, retains hex and original paths, and refuses a mismatch.
Module/carrier/JetPack/CUDA CLI labels are preserved as operator labels and
explicitly marked **not** a native compatibility proof. No hardware is invented
by supplying a label. Without the flag it returns13 before a platform-file read.
The implementation never writes sysfs, loads CUDA, changes a clock, installs a
driver or calls a remote device. This is a deployment admission observation;
actual GPU execution and hardware support must still be qualified separately.

Future invocation (never performed here):

```
python3 admit_platform.py --enable-read-only-native \
 --model-file /sys/firmware/devicetree/base/model --model-sha256 ACTUAL_SHA256 \
 --release-file /etc/nv_tegra_release --release-sha256 ACTUAL_SHA256 \
 --module-label ACTUAL_SKU --carrier-label ACTUAL_CARRIER \
 --jetpack-label ACTUAL_JETPACK --cuda-label ACTUAL_CUDA
```

Build and deploy an identical source-bound Guest artifact first with no ROS
grants, then a separately authorized single-resource policy. Match its real
worker PID/UID/session, Native exports and QoS to the Gateway. Verify complete
typed observations/effects and independent teardown as in the root tutorial.
Native CUDA/ROS behavior does not establish Guest authority; Native debug does
not establish any high-level language DWARF claim. No Optimus SDK is supplied.
