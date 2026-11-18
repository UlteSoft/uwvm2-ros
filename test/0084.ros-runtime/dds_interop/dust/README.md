# Dust DDS 0.16.0 with ROS 2 Jazzy

This fixture compares four native process pairs using the same ROS topic names,
message type and request/reply check. Each requester publishes a random positive
Int32 and accepts only its successor. Each responder verifies the request before
writing the reply. Each pair has a fresh ROS domain. The Jazzy process checks its
RMW identifier. The Rust process advertises the final DDS type
`std_msgs::msg::dds_::Int32_` with one signed 32-bit member, reliable delivery and
XCDR1. The Python wheel uses its public dataclass API and reliable delivery.

| Requester → responder | Rust crate 0.16.0 | CPython wheel 0.16.0 |
| --- | --- | --- |
| Dust → Dust | Challenge and reply verified | Responder received an object without the declared `data` member. Requester timed out. |
| Jazzy → Jazzy | Challenge and reply verified | Challenge and reply verified |
| Jazzy → Dust | No endpoint match or reply | No endpoint match or reply |
| Dust → Jazzy | No endpoint match or reply | No endpoint match or reply |

In both mixed Rust trials Dust reported two discovered participants, zero
matched readers and zero matched writers. These results leave cross-vendor
Int32 delivery unqualified for this configuration. The observation does not
identify which endpoint matching rule prevented delivery. The Python trial used
its own Fast DDS interface profile. Its failing same-binding control is
recorded separately from the Rust result.

The endpoints run as UID/GID 65534 with an empty capability bounding set in
one x86-64 QEMU guest. Its Ethernet hub has one port and no host network backend
or default route. The data path uses guest-local plaintext UDPv4. These tests
cover the independent native peers. DDS Security, physical networks, Wasm
application imports, other ROS releases and other message types require
separate measurements.

## Inputs

- Use the original Rust crate and checked-in Cargo lock. The crate SHA-256 is
  `769e637705ab26402e67a5fdb526776eaeb444e75bcc8f88e1e32ca77cd45be0`.
  [Published crate](https://crates.io/crates/dust_dds/0.16.0), [upstream revision](https://github.com/s2e-systems/dust-dds/tree/9bd8b71c256025fe534f51ca79c39d59240bcc1c)
- For the independent Python test, supply the original CPython 3.14
  manylinux x86-64 wheel for Dust DDS 0.16.0. The runner enforces SHA-256
  `b9b71e871abe50d0aa2f3166ef21937aa4035a3a4c6a6c1157a52f643be7a8f2`.
  [Published wheels](https://pypi.org/project/dust-dds/0.16.0/#files)
- Use the native Jazzy/Fast DDS `ros-client` beside its completed
  [Int32 fixture receipt](../README.md). The script checks the executable
  digest and uses the prefix and system libraries from that receipt. Keep
  these directories available when running the guest.
- Supply an x86-64 QEMU executable, a Linux kernel and matching e1000 module,
  SeaBIOS and QEMU firmware, and static BusyBox. The host requires CPython
  3.14, iproute2, setpriv, readelf, cpio, gzip and sufficient tmpfs space for
  the temporary initramfs.

## Reproduce

Run from the VM repository. Set the path variables below to your prepared inputs.
Use a fresh directory under the repository's ignored `.tmp` for outputs.

```sh
fixture=test/0084.ros-runtime/dds_interop/dust
scratch=$(mktemp -d "$PWD/.tmp/dust-exchange.XXXXXXXX")
run_in_gate() {
    /usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
        /usr/bin/taskset -c 4-7 /usr/bin/flock \
        ../uwvm2-ros-sdk-v2/.tmp/sdk-qualification.lock "$@"
}
run_in_gate cargo build --locked --release \
    --manifest-path "$fixture/rust/Cargo.toml" --target-dir "$scratch/target"

run_guest() {
    run_in_gate /usr/bin/python3 -B "$fixture/run_qemu.py" \
        --ros-receipt "$ROS_RECEIPT" --qemu "$QEMU" --kernel "$KERNEL" \
        --ethernet-module "$E1000_KO" --busybox "$BUSYBOX" --bios "$BIOS" \
        --firmware-dir "$FIRMWARE_DIR" "$@"
}
run_guest --rust-peer "$scratch/target/release/wros-dust-witness" --output "$scratch/rust"
run_guest --wheel "$DUST_WHEEL" --output "$scratch/python"
```

Each invocation keeps a serial log, its digest, the native input digests,
endpoint process exit codes and the challenge in the specified output directory.
A run exits successfully only when both controls and both cross-vendor exchanges
pass. In an observed failing run inspect `results.json` for `executed` to
distinguish a completed four-case run from an incomplete guest boot.
