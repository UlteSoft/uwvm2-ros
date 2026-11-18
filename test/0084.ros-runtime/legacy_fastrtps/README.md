# Archived native ROS 2 String examples

Beta 2 (`r2b2`), Beta 3 (`r2b3`) and Ardent use the Xenial ROS archive. Bouncy and
Crystal use Bionic. The fixture runs each release's original
`publisher_member_function` and `subscriber_member_function` binaries from
`examples_rclcpp_minimal_*`. The application topic is `/topic`, with message type
`std_msgs/msg/String`. A finite Jazzy rclcpp application can send a run-specific
random string or observe the strings published by the historical example.

| Case | Evidence required |
|---|---|
| Historical publisher to Jazzy | At least three distinct strings in the original publisher log agree with strings received by the Jazzy subscription callback. |
| Jazzy publisher to historical subscriber | Jazzy sends ten copies of a random string. The original subscriber prints that string. |
| Same release | The two original ROS executables from one release log at least three distinct matching strings. |

All three cases check the actual rclcpp, RMW and Fast RTPS library mappings for both
processes. The worker compares the historical executables and their mapped middleware
libraries byte-for-byte against members of the authenticated Debian archives. Each
case retains its payloads, stdout/stderr hashes, exit status, harness shutdown signals
and diagnostic counts. A pseudo-terminal captures the long-running historical examples'
line-buffered logs. `complete` means that every case produced a result.
`crossReleaseBothDirections` and `nativePairPassed` report independent outcomes.
The measurements cover plaintext native topic traffic. ROS graph compatibility,
SROS2, ROS services, VM/SDK execution and physical equipment have separate qualification.

The ROS spelling `/topic` spans a change in DDS naming. Ardent's RMW 0.4.0 splits
the ROS name into a DDS topic and a partition. Bouncy's RMW 0.5.1 prefixes the full
ROS name in the DDS topic. The fixture preserves the application names and publishes
its outcome for each direction. [Ardent implementation](https://github.com/ros2/rmw_fastrtps/blob/0.4.0/rmw_fastrtps_cpp/src/assign_partitions.hpp), [Bouncy implementation](https://github.com/ros2/rmw_fastrtps/blob/0.5.1/rmw_fastrtps_cpp/src/rmw_publisher.cpp)

## Inputs

Run the commands below through the repository's 64 GiB qualification cgroup and serial
lock. Use the system Python 3.11+ on x86_64 Linux. The host needs gpgv, dpkg-deb,
bubblewrap for the namespace variant, and the previously qualified Jazzy sysroot.

Place the ROS archive's original `InRelease` and `main/binary-amd64/Packages.gz` in
`$INDEXES/ros-xenial` and `$INDEXES/ros-bionic`, with each index saved as `Packages.gz`.
Supply the archive signing keyring. The preparation tool authenticates the metadata,
downloads the ROS dependency closure, and verifies the hash and size of every .deb.
The runner repeats these checks at execution time. Supply system-library directories
for the corresponding Ubuntu releases, including their OpenSSL, Poco and TinyXML2
libraries. Jazzy uses its own sysroot and system-library directory.

```sh
python3 -B packages.py ardent --index-dir "$INDEXES/ros-xenial" --keyring "$KEYRING" --output "$INPUTS/ardent"
python3 -B build_witness.py --jazzy-receipt "$JAZZY_DDS_RESULTS" --output "$BUILD"
```

Prepare the other desired releases using their matching indexes. The builder reuses
the native DDS fixture's successful Jazzy compiler invocation, headers, libraries,
`-O3` and debug symbols.

## Loopback network namespace

```sh
python3 -B run.py --plan "$INPUTS/ardent/signed-plan.json" --index-dir "$INDEXES/ros-xenial" \
  --keyring "$KEYRING" --legacy-system "$XENIAL_LIBS" --jazzy-prefix "$JAZZY_PREFIX" \
  --jazzy-system "$JAZZY_LIBS" --profile "$DDS_LOOPBACK_XML" --witness "$BUILD/jazzy-string" \
  --output "$RESULTS"
```

The worker unpacks the legacy packages into a temporary memory filesystem. The private
network namespace has one loopback interface. The profile can be the native DDS fixture's
`dds_interop/fastdds-loopback.xml`. For older vendors which look in the current directory,
`--legacy-profile FILE` places a copy at `DEFAULT_FASTRTPS_PROFILES.xml`.
`--legacy-defaults` uses the vendor defaults for the historical process.
The receipt records the actual interfaces, addresses, routes and profile choices.

## QEMU guest with local DDS multicast

On older releases, discovery may require a multicast-capable interface and an advertised
address other than 127.0.0.1. Fast RTPS 1.5 filters that address during IPv4 enumeration.
[Fast RTPS 1.5 IPFinder](https://github.com/eProsima/Fast-DDS/blob/v1.5.0/src/cpp/utils/IPFinder.cpp)

`run_qemu.py` stages an initramfs in `/dev/shm`. Supply a Linux x86_64 kernel with built-in
initramfs, procfs, sysfs, devtmpfs, devpts and loopback support, static BusyBox with
mount/tar/base64/poweroff, QEMU, and its matching BIOS and firmware directory.
The host uses GNU find, cpio, gzip, readelf and ldconfig. The guest has two virtual CPUs
and 2 GiB of RAM. The default topology starts QEMU with `-nodefaults -nic none`.
Its Linux loopback interface has 127.0.0.1/8, 192.0.2.1/32 and a local
route to 239.255.0.1 for DDS discovery. Use `guest_loopback.xml` for this topology.

With `--isolated-ethernet-module /path/to/e1000.ko`, QEMU instead attaches one
emulated Intel Ethernet controller to a private QEMU hub with a single port and
no host network backend. The guest loads the supplied kernel-matched e1000 module.
Its loopback retains 127.0.0.1, and its Ethernet interface receives 192.0.2.1/24 and
the multicast route. The receipt records the module hash, QEMU argv and both interfaces.
This variant exercises the vendor’s normal multicast interface selection. Use
`guest_fastdds.xml` for Jazzy. The historical application uses its vendor defaults.

The ROS observation runs with all capability
sets at zero and `no_new_privs`. Logs and receipts return through the serial console.

```sh
python3 -B run_qemu.py r2b2 r2b3 ardent bouncy crystal \
  --inputs "$INPUTS" --indexes "$INDEXES" --keyring "$KEYRING" \
  --xenial-system "$XENIAL_LIBS" --bionic-system "$BIONIC_LIBS" \
  --jazzy-prefix "$JAZZY_PREFIX" --jazzy-system "$JAZZY_LIBS" \
  --jazzy-profile guest_loopback.xml --witness "$BUILD/jazzy-string" \
  --qemu "$QEMU" --kernel "$KERNEL" --busybox "$STATIC_BUSYBOX" \
  --bios "$BIOS" --firmware-dir "$FIRMWARE" --accel tcg --output "$RESULTS"
```

TCG is the default accelerator. `--keep-initramfs` retains the image alongside its
SHA-256. `receipt.json` identifies the inputs and every release receipt.
`reports/RELEASE/results.json` contains the three case results and references their logs.
The command returns a nonzero status when either direction or a same-release control
fails. A successful control establishes that the release's own two applications exchanged
samples under the recorded settings. Cross-release scope follows the two directed cases.

## Recorded dual-address loopback observations

The archived run used a NIC-less QEMU TCG guest and a single multicast-enabled lo
interface carrying both IPv4 addresses. Both signed sample programs and the Jazzy
application used the default ROS domain. The kernel, initramfs, package plans, RMW
and ELF hashes are in the receipts. The ROS guest did not retain capabilities.

| ROS release | Same-release String samples | Historical → Jazzy | Jazzy → historical |
|---|---:|---|---|
| Beta 2 | 0 | No delivery | No delivery |
| Beta 3 | 0 | No delivery | No delivery |
| Ardent | 10 | No delivery | No delivery |
| Bouncy | 10 | No delivery | No delivery |
| Crystal | 10 | No delivery | No delivery |

Both Beta releases emitted `set_option: Address already in use` and exited.
In a separate host loopback run Bouncy published three distinct strings received by
Jazzy. That harness subsequently had to kill the Bouncy publisher, and its reverse
direction recorded zero matching payloads. The table describes the QEMU run above.

The SHA-256 of the retained host receipt for this measurement is
`2833ca75684d1c238e2c10918ff4b750676310cd3ad139b3f1ec3eb9f0ca7d89`.
The runner writes the per-release receipts and endpoint logs to the chosen
output directory; keep these together to verify a run independently.
