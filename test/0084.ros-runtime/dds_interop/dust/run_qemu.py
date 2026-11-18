"""Run two independent native Dust and Jazzy Int32 endpoints on one hostless guest Ethernet."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import zipfile

HERE = Path(__file__).resolve().parent
sys.path[:0] = [str(HERE.parents[1] / "legacy_fastrtps"), str(HERE.parents[1])]
from run_qemu import copy, pack, runtime_libraries
from run_scene_vm import sha, verify_gate

WHEEL_SHA256 = "b9b71e871abe50d0aa2f3166ef21937aa4035a3a4c6a6c1157a52f643be7a8f2"
ROLES = [("dust", "dust"), ("fast", "fast"), ("fast", "dust"), ("dust", "fast")]


def guest_inputs(root, args, native, prefix, system):
    for name in ("bin", "usr/bin", "usr/lib", "lib64", "dev", "sys", "proc", "tmp", "etc", "fixture/dust/dust_dds", "syslib/jazzy"):
        (root / name).mkdir(parents=True, exist_ok=True)
    (root / "lib").symlink_to("usr/lib")
    (root / "bin/sh").symlink_to("busybox")
    (root / "tmp").chmod(0o1777)
    (root / "etc/hosts").write_text("127.0.0.1 localhost\n192.0.2.1 dust-hub\n")
    (root / "etc/passwd").write_text("root:x:0:0:root:/tmp:/bin/sh\nnobody:x:65534:65534:nobody:/tmp:/bin/sh\n")
    (root / "etc/group").write_text("root:x:0:\nnogroup:x:65534:\n")
    inputs = {args.busybox: "bin/busybox", args.ethernet_module: "devices/e1000.ko", native: "fixture/ros-client",
              HERE / "guest_init.sh": "init", HERE / "guest.py": "fixture/guest.py", HERE / "peer.py": "fixture/peer.py",
              args.fastdds_profile: "fixture/fastdds.xml"}
    for name in ("/usr/sbin/ip", "/usr/bin/setpriv", "/usr/bin/python3", "/lib64/ld-linux-x86-64.so.2", "/usr/lib/python3.14"):
        inputs[Path(name)] = name[1:]
    for source, destination in inputs.items():
        copy(source, root / destination)
    (root / "init").chmod(0o755)
    for name in ("share", "lib"):
        copy(prefix / name, root / "opt/ros/jazzy" / name)
    for path in system.glob("*.so*"):
        if path.is_file():
            copy(path, root / "syslib/jazzy" / path.name)
    member_hashes = {}
    if args.wheel:
        with zipfile.ZipFile(args.wheel) as wheel:
            for name in ("dust_dds/__init__.py", "dust_dds/dust_dds.cpython-314-x86_64-linux-gnu.so"):
                entry = wheel.getinfo(name)
                if entry.file_size > 16 << 20:
                    raise ValueError("unexpected wheel member size")
                data = wheel.read(entry)
                (root / "fixture/dust" / name).write_bytes(data)
                member_hashes[name] = hashlib.sha256(data).hexdigest()
    if args.rust_peer:
        copy(args.rust_peer, root / "fixture/rust-peer")
    runtime_libraries(root, ["/fixture/ros-client", *(["/fixture/rust-peer"] if args.rust_peer else []), *["/fixture/dust/" + n for n in member_hashes if n.endswith(".so")]])
    return member_hashes


def verified_result(row):
    for endpoint, role, key in ((row["requester"], "request", "request"), (row["responder"], "echo", "response")):
        proc = row[key]
        if proc["code"] != 0:
            return False
        lines = proc["output"].splitlines()
        if endpoint == "dust":
            source = json.loads(lines[-1])
            if (source["role"] != role or source["challenge"] != row["token"] or
                    min(source["received"], source["writes"], source["matched_readers"], source["matched_writers"]) <= 0):
                return False
        elif f"rclcpp rmw_fastrtps_cpp {role} completed" not in lines:
            return False
    return True


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ("ros-receipt", "qemu", "kernel", "ethernet-module", "busybox", "bios", "firmware-dir", "output"):
        p.add_argument("--" + name, type=Path, required=True)
    bindings = p.add_mutually_exclusive_group(required=True)
    bindings.add_argument("--wheel", type=Path, help="Pinned CPython 3.14 wheel for the independent Python binding test")
    bindings.add_argument("--rust-peer", type=Path, help="Use the separately built locked Rust witness for the Dust endpoint")
    a = p.parse_args()
    a.fastdds_profile = HERE / ("fastdds.xml" if a.rust_peer else "fastdds-python.xml")
    verify_gate()
    if not os.sched_getaffinity(0) <= {4, 5, 6, 7} or sys.version_info[:2] != (3, 14):
        p.error("use the designated CPUs and the CPython 3.14 host for the pinned wheel")
    if a.wheel and sha(a.wheel) != WHEEL_SHA256:
        p.error("the supplied wheel does not match Dust DDS 0.16.0 for CPython 3.14 x86_64")
    original = json.loads(a.ros_receipt.read_bytes())
    native = a.ros_receipt.parent / "ros-client"
    if not original["complete"] or sha(native) != original["outputs"]["ros-client"]:
        p.error("the native rclcpp binary differs from the supplied DDS receipt")
    args = original["steps"][-1]["argv"]
    system = Path(args[args.index("--ros-system-libs") + 1])
    prefix = Path(original["ros_prefix"])
    a.output.mkdir(mode=0o700, parents=True, exist_ok=False)
    inputs = [a.fastdds_profile, a.qemu, a.kernel, a.ethernet_module, a.busybox, a.bios, native, a.ros_receipt, Path(__file__),
              *(HERE / name for name in ("guest_init.sh", "guest.py", "peer.py"))]
    if a.wheel:
        inputs.append(a.wheel)
    if a.rust_peer:
        inputs.extend([a.rust_peer, HERE / "rust/Cargo.lock", HERE / "rust/Cargo.toml", HERE / "rust/src/main.rs"])
    inputs.append(HERE.parents[1] / "legacy_fastrtps/run_qemu.py")
    report = dict(complete=False, binding="rust" if a.rust_peer else "python", network="single guest Ethernet hub with no host backend", dust_version="0.16.0",
                  inputs={str(path.resolve()): sha(path) for path in inputs}, cgroup=Path("/proc/self/cgroup").read_text().strip(),
                  cases=[], ros_dds_security="plaintext isolated test only")
    def save():
        (a.output / "results.json").write_text(json.dumps(report, indent=2) + "\n")
    save()
    with tempfile.TemporaryDirectory(dir="/dev/shm", prefix="wros-dust-hub-") as scratch:
        root = Path(scratch) / "root"
        report["wheel_members"] = guest_inputs(root, a, native, prefix, system)
        archive = Path(scratch) / "initrd.cpio.gz"
        pack(root, archive, a.output / "pack.log")
        report["initrd_sha256"] = sha(archive)
        report["initrd_bytes"] = archive.stat().st_size
        accel = "kvm" if os.access("/dev/kvm", os.R_OK | os.W_OK) else "tcg"
        cmd = [a.qemu, "-nodefaults", "-machine", "q35", "-accel", accel, "-smp", "2", "-m", "1536",
               "-netdev", "hubport,id=fixture,hubid=0", "-device", "e1000,netdev=fixture,mac=52:54:00:00:00:01",
               "-display", "none", "-no-reboot", "-monitor", "none", "-serial", "stdio", "-bios", a.bios, "-L", a.firmware_dir,
               "-kernel", a.kernel, "-initrd", archive, "-append", "console=ttyS0 loglevel=2 rdinit=/init panic=-1"]
        report["argv"] = list(map(str, cmd))
        save()
        serial = a.output / "serial.log"
        with serial.open("xb") as log:
            result = subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT, timeout=145)
        if serial.stat().st_size > 1 << 20:
            raise ValueError("unexpected guest console extent")
        report["exit"] = result.returncode
        report["serial_sha256"] = sha(serial)
        lines = serial.read_text(errors="replace").splitlines()
        report["cases"] = [json.loads(line.removeprefix("DUST_CASE ")) for line in lines if line.startswith("DUST_CASE ")]
        report["executed"] = (result.returncode == 0 and "DUST_DONE 4" in lines and
                              [(row["requester"], row["responder"]) for row in report["cases"]] == ROLES)
        results = [verified_result(row) for row in report["cases"]]
        report["controls_passed"] = report["executed"] and all(results[:2])
        report["cross_vendor_passed"] = report["executed"] and all(results[2:])
        report["complete"] = report["controls_passed"] and report["cross_vendor_passed"]
        save()
    return 0 if report["complete"] else 1


if __name__ == "__main__":
    sys.exit(main())
