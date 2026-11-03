#!/usr/bin/env python3
"""Build one Linux release ROS product and execute its finite product smoke test."""
import argparse
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import tempfile

from receipt import save, sha


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--llvm-prefix", type=Path, required=True)
    args = parser.parse_args()
    source, out = args.source.resolve(strict=True), args.out.resolve(strict=True)
    if sys.platform != "linux":
        parser.error("ROS product qualification requires Linux")
    if str(args.llvm_prefix.resolve(strict=True)) != os.environ.get("UWVM_QUALIFIED_LLVM_PREFIX"):
        parser.error("run through guarded.py with the reviewed provisioning identity")
    helper = source / "test/0080.product-qualification/receipt.py"
    def run(name, dimensions, argv, timeout, subjects=(), environment=None):
        command = [sys.executable, "-B", str(helper), "command", "--out", str(out), "--id", name,
                   "--cwd", str(source), "--timeout", str(timeout)]
        for dimension in dimensions:
            command += ["--dimension", dimension]
        for subject in subjects:
            command += ["--subject", str(subject)]
        return subprocess.run([*command, "--", *map(str, argv)], check=False, env=environment).returncode
    status = run("toolchain", (), [sys.executable, "-B", source / "test/0080.product-qualification/probe_toolchain.py",
                                   "--prefix", args.llvm_prefix, "--out", out / "toolchain"], 300)
    if status:
        return status
    # These execute only inside the existing guarded.py batch after real
    # preprovisioned tool admission. A hosted PR performs file analysis only.
    for name, script, timeout in (
            ("module-source", "test/0006.check_module/check_uwvm_module.py", 120),
            ("module-checker-tests", "test/0006.check_module/test_checker_semantics.py", 180),
            ("ros-profile-tests", "test/0073.ros-tests/support/source/ros_distro/test_profiles.py", 120)):
        status = run(name, ("implementation",), [sys.executable, "-B", source / script], timeout, (source / script,))
        if status:
            return status
    status = run("qualification-helper-tests", ("implementation",),
                 [sys.executable, "-B", "-m", "unittest", "discover", "-s",
                  source / "test/0080.product-qualification", "-p", "test_*.py", "-v"], 180)
    if status:
        return status
    # The compiler and linker are the reviewed bootstrap prefix that passed the
    # unchanged actual probe; neither apt nor an alternate system compiler runs.
    prefix = args.llvm_prefix.resolve(strict=True)
    unit_environment = dict(os.environ, CXX=shlex.quote(str(prefix / "bin/clang++")),
                            ROS_TEST_CXXFLAGS="-std=c++26 -stdlib=libc++ -fuse-ld=lld -O1 -g0")
    save(out / "unit-toolchain.json", {"prefix": str(prefix), "CXX": unit_environment["CXX"],
        "ROS_TEST_CXXFLAGS": unit_environment["ROS_TEST_CXXFLAGS"],
        "compiler_sha256": sha(prefix / "bin/clang++"), "linker_sha256": sha(prefix / "bin/ld.lld"),
        "scope": "Finite ROS units only; existing product configure environment is unchanged."})
    unit_driver = source / "test/0073.ros-tests/support/source/ros/run_unit.py"
    for suite, timeout in (("security", 240), ("ipc", 720)):
        # Linux sockaddr_un is bounded. Keep real IPC paths short and private;
        # retain all subreceipts, logs and binaries before owned temporary cleanup.
        with tempfile.TemporaryDirectory(prefix="uq-", dir="/tmp") as temporary:
            unit_out = Path(temporary) / suite
            try:
                status = run("ros-" + suite + "-units", ("implementation",),
                             [sys.executable, "-B", unit_driver, "--suite", suite, "--out", unit_out],
                             timeout, (unit_driver,), environment=unit_environment)
            finally:
                if unit_out.exists():
                    shutil.copytree(unit_out, out / ("ros-" + suite + "-units"))
        if status:
            return status
    identity = (out / "source-id.txt").read_text().strip()
    configure = ["xmake", "f", "-y", "-c", "-m", "release", "--use-llvm-compiler=y",
                 "--sdk=" + str(args.llvm_prefix.resolve(strict=True)), "--stdlib=libc++", "--march=none",
                 "--static=none", "--execution-int=none", "--execution-jit=llvm",
                 "--import-ros1=y", "--import-ros2=y", "--linux-native-debug=y", "--use-cxx-module=n",
                 "--llvm-runtimes-provider=bundled", "--llvm-build-jobs=1", "--openssl-provider=bundled",
                 "--openssl-build-jobs=1", "--enable-lto=n", "--strip=none", "--ccache=n", "--build-source-id=" + identity]
    status = run("configure", ("build",), configure, 1800)
    if status:
        return status
    status = run("build", ("build",), ["xmake", "b", "-rv", "-j1", "uwvm2-ros"], 18000)
    if status:
        return status
    status = run("target", (), ["xmake", "show", "-t", "uwvm2-ros"], 120)
    if status:
        return status
    text = re.sub(r"\x1b\[[0-?]*[ -/]*[@-~]", "", (out / "target.stdout").read_text())
    paths = re.findall(r"^\s*targetfile:\s*(.+?)\s*$", text, re.M)
    if len(paths) != 1:
        raise RuntimeError("actual xmake product targetfile was not uniquely reported")
    binary = (source / paths[0]).resolve(strict=True)
    if not binary.is_relative_to(source / "build"):
        raise RuntimeError("configured product is not below build/")
    with binary.open("rb") as stream:
        header = stream.read(64)
    expected = {"x86_64": (2, 62), "aarch64": (2, 183), "armv7l": (1, 40)}.get(os.uname().machine)
    if (len(header) < 52 or header[:4] != b"\x7fELF" or header[5] != 1 or expected is None
            or (header[4], int.from_bytes(header[18:20], "little")) != expected):
        raise RuntimeError("configured product ELF class/machine/endian differs from the actual native runner")
    if expected == (1, 40):
        flags = int.from_bytes(header[36:40], "little")
        if flags >> 24 < 5 or not flags & 0x400 or flags & 0x200:
            raise RuntimeError("ARM candidate requires the actual little-endian EABI5 hard-float ELF")
    status = run("closure", ("build",), ["ldd", binary], 120, (binary,))
    if status:
        return status
    closure = (out / "closure.stdout").read_text()
    if "not found" in closure:
        raise RuntimeError("a product runtime library is unavailable")
    dependencies = sorted(set(re.findall(r"(/[^\s()]+)", closure)))
    if not dependencies:
        raise RuntimeError("the dynamic product loader/library closure was not identified")
    save(out / "runtime-closure.json", {"elf_class": header[4], "elf_machine": expected[1], "endian": "little",
        "host_machine": os.uname().machine, "dependencies": {
            name: {"physical_path": str(Path(name).resolve(strict=True)), "sha256": sha(name)} for name in dependencies}})
    save(out / "product.json", {"path": str(binary), "sha256": sha(binary), "source_id": identity,
                               "mode": "release", "ros_imports": [1, 2], "execution": "AOT", "stack": "checked native unwind",
                               "native_ros": "UNTESTED", "qemu": "UNTESTED", "performance": "UNTESTED"})
    shutil.copy2(binary, out / "uwvm2-ros")
    status = run("version", ("execution",), [binary, "--version"], 60, (binary,))
    if status:
        return status
    return run("basic", ("execution",), ["xmake", "basic_test", "--output=" + str(out / "basic")], 900, (binary,))


if __name__ == "__main__":
    raise SystemExit(main())
