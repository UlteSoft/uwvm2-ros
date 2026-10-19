#!/usr/bin/env python3
"""Build one selected native Gateway and optionally run a bounded SDK topic test.

This is an additional distribution lane. It never re-labels the original
Noetic/Jazzy acceptance products. Gateway linkage is BUILD_COVERED; the optional
native SDK pub/sub witness is VALIDATED only for that narrow scope. Neither
result claims Guest delivery, all features, native custom identity, or SROS2.
"""
from __future__ import annotations

import json
from pathlib import Path
import shlex
import sys

sys.dont_write_bytecode = True
from common import atomic_json, body_pin, install_interrupts, opened, require
from probe import Preparation, arguments


def elf(path, target, runtime=None, sysroot=None):
    """Verify the actual binary class, byte order and e_machine, never its label."""
    pin = opened(path)
    interpreter = None
    with Path(path).open("rb") as stream:
        header = stream.read(64)
    require(opened(path) == pin and header[:4] == b"\x7fELF" and len(header) >= 52,
            "stable ELF artifact required")
    endian = {1: "little", 2: "big"}.get(header[5])
    bits = {1: 32, 2: 64}.get(header[4])
    require(bits in (32, 64) and len(header) >= (64 if bits == 64 else 52),
            "complete actual ELF header required")
    require(endian == target["endian"] and bits == target["bits"] and
            int.from_bytes(header[18:20], endian) == target["elf_machine"] and
            int.from_bytes(header[16:18], endian) in (2, 3),
            "compiled ELF is not the explicitly selected architecture")
    with Path(path).open("rb") as stream:
        offset = int.from_bytes(header[32:40] if bits == 64 else header[28:32], endian)
        step = int.from_bytes(header[54:56] if bits == 64 else header[42:44], endian)
        count = int.from_bytes(header[56:58] if bits == 64 else header[44:46], endian)
        require(count <= 256 and (not count or 32 <= step <= 256), "bounded ELF program headers required")
        for index in range(count):
            stream.seek(offset + index * step)
            ph = stream.read(step)
            require(len(ph) == step, "truncated ELF program header")
            if int.from_bytes(ph[:4], endian) != 3:  # PT_INTERP
                continue
            require(interpreter is None, "duplicate ELF dynamic interpreter")
            start = int.from_bytes(ph[8:16] if bits == 64 else ph[4:8], endian)
            size = int.from_bytes(ph[32:40] if bits == 64 else ph[16:20], endian)
            require(1 < size <= 4096 and start + size <= pin["bytes"], "invalid ELF interpreter span")
            stream.seek(start)
            value = stream.read(size)
            require(value.endswith(b"\0") and b"\0" not in value[:-1], "invalid ELF interpreter string")
            interpreter = value[:-1].decode("utf-8")
            require(Path(interpreter).is_absolute() and ".." not in Path(interpreter).parts,
                    "absolute ELF interpreter required")
    # EM_ARM/class/byte order alone cannot distinguish the hard-float calling
    # convention from soft-float. Inspect actual ELF32 e_flags in its declared
    # byte order, including the selected loader below. Generic historical arm
    # profiles keep their old contract; explicitly named ABI profiles fail if
    # the binary does not positively identify the requested float ABI.
    arm_float_abi = None
    if target["elf_machine"] == 40 and bits == 32:
        arm_flags = int.from_bytes(header[36:40], endian)
        arm_abi_version = arm_flags >> 24
        float_bits = arm_flags & 0x600
        require(float_bits != 0x600, "contradictory ARM soft/hard-float ELF flags")
        arm_float_abi = {0: "unspecified", 0x200: "soft", 0x400: "hard"}[float_bits]
        # AAELF32 specifies the base (soft-float) procedure-call standard when
        # both bits are clear in an ABI-v5 executable; zero is not hard-float.
        if float_bits == 0 and arm_abi_version >= 5:
            arm_float_abi = "soft"
        requested = {"armel": "soft", "armhf": "hard", "armebhf": "hard"}.get(target["architecture"])
        require(requested is None or (arm_abi_version >= 5 and arm_float_abi == requested),
                "actual ARM ELF float ABI differs from explicit target")
    result = {"input": pin, "bits": bits, "endian": endian, "elf_machine": target["elf_machine"],
              "interpreter": interpreter}
    if arm_float_abi is not None:
        result["arm_float_abi"] = arm_float_abi
    if runtime is not None and runtime["mode"] == "qemu":
        require(interpreter is not None, "QEMU profile explicitly selects a dynamic loader")
        selected = Path(runtime["loader"]).resolve(strict=True)
        expected = (Path(sysroot) / interpreter.lstrip("/")).resolve(strict=True)
        require(selected == expected and Path(sysroot).resolve(strict=True) in selected.parents,
                "QEMU loader differs from the compiled ELF interpreter in its own sysroot")
        result["selected_loader"] = elf(selected, target)
    require(opened(path) == pin, "ELF artifact changed during inspection")
    return result


def dependency_records(build):
    """Retain real make dependency files and every physical header they name.

    This records the compiler's retained closure. It does not invent a unique
    full-MD invocation from generated makefiles or claim unrecorded runtime
    libraries. The actual verbose build log and compile_commands database are
    retained separately. Paths with make escapes are decoded before opening.
    """
    result = []
    known = {}
    for dependency in sorted(build.rglob("*.d")):
        data, pin = body_pin(dependency)
        text = data.decode().replace("\\\n", "")
        require(":" in text, "malformed compiler dependency output")
        names = shlex.split(text.split(":", 1)[1], posix=True)
        require(names, "empty compiler dependency output")
        files = []
        for name in names:
            path = Path(name)
            if not path.is_absolute():
                path = build / path
            path = path.resolve(strict=True)
            if str(path) not in known:
                known[str(path)] = opened(path)
            files.append(known[str(path)])
        result.append({"dependency_file": pin, "inputs": files})
    require(result, "no retained compiler dependency files")
    return result


def native_configure(prep):
    """Build the small official-API witness with the same SDK and ABI selection."""
    profile = prep.profile
    tools = profile["tools"]
    build = prep.out / "native-basic-build"
    flags = profile["cxx"]["flags"] + ["-O0", "-g0", "-MD"]
    if profile["backend"] == 1:
        cache = (prep.build / "CMakeCache.txt").read_text()
        choices = [line.split("=", 1)[1] for line in cache.splitlines()
                   if line.startswith("UWVM_ROS1_SELECTED_CXX11_ABI:")]
        require(len(choices) == 1 and choices[0] in ("0", "1", "NOT_LIBSTDCXX"),
                "Gateway must expose its actual ROS1 standard-library ABI link selection")
        prep.state["ros1_selected_cxx11_abi"] = choices[0]
        if choices[0] != "NOT_LIBSTDCXX":
            flags.append("-D_GLIBCXX_USE_CXX11_ABI=" + choices[0])
    argv = [tools["cmake"], "-S", str(prep.source / "tests/ros_distro"), "-B", str(build),
            "-G", "Unix Makefiles", "-DUWVM_ROS_BACKEND=" + str(profile["backend"]),
            "-DCMAKE_CXX_COMPILER=" + tools["cxx"], "-DCMAKE_MAKE_PROGRAM=" + tools["make"],
            "-DCMAKE_CXX_STANDARD=" + str(profile["cxx"]["standard"]),
            "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
            "-DCMAKE_CXX_FLAGS=" + shlex.join(flags),
            "-DCMAKE_EXE_LINKER_FLAGS=" + shlex.join(profile["cxx"]["link_flags"]),
            "-DPYTHON_EXECUTABLE=" + tools["generator_python"],
            "-DPython3_EXECUTABLE=" + tools["host_python"]]
    for key, option in (("sysroot", "CMAKE_SYSROOT"), ("toolchain_file", "CMAKE_TOOLCHAIN_FILE")):
        if tools[key]:
            argv.append("-D" + option + "=" + tools[key])
    if profile["backend"] == 2:
        argv.append("-DUWVM_BASIC_HAVE_QOS=" + str(int(prep.state["actual_features"]["features"]["qos_class"])))
    else:
        for key in ("UWVM_ROS1_DISCOVERY", "UWVM_ROS1_INCLUDE_DIRS", "UWVM_ROS1_LIBRARIES"):
            if key in profile["cmake_options"]:
                value = profile["cmake_options"][key]
                argv.append("-D" + key + "=" + (";".join(value) if isinstance(value, list) else value))
    prep.runner.run("native-basic-configure", argv, prep.out, prep.environment)
    prep.build_target("native-basic-build", build, "uwvm-distro-native-basic")
    product = build / "uwvm-distro-native-basic"
    prep.state["native_basic_artifact"] = elf(product, profile["target"], profile["runtime"], tools["sysroot"])
    prep.state["native_basic_dependencies"] = dependency_records(build)
    prep.state["native_basic_compile_database"] = opened(build / "compile_commands.json")
    prep.state["native_basic_cache"] = opened(build / "CMakeCache.txt")
    return product


def main():
    install_interrupts()
    args = arguments(__doc__)
    prep = Preparation(args.profile, args.source, args.source_manifest, args.out)
    error = None
    try:
        prep.begin()
        prep.configure()
        profile = prep.profile
        target = "uwvm-ros%d-gateway" % profile["backend"]
        prep.build_target("gateway-build", prep.build, target)
        prep.state["gateway_artifact"] = elf(prep.build / target, profile["target"], profile["runtime"], profile["tools"]["sysroot"])
        prep.state["gateway_dependencies"] = dependency_records(prep.build)
        prep.state["gateway_compile_database"] = opened(prep.build / "compile_commands.json")
        prep.state["gateway_cache"] = opened(prep.build / "CMakeCache.txt")
        prep.state["build_status"] = "BUILD_COVERED"
        prep.state["gateway_behavior_status"] = "UNVERIFIED"
        if profile["runtime"]["behavior"] == "native-sdk-topic":
            product = native_configure(prep)
            if profile["runtime"]["mode"] == "build-only":
                prep.state["behavior_status"] = "UNVERIFIED_BUILD_ONLY_SELECTION"
            else:
                command = [str(product)]
                if profile["runtime"]["mode"] == "qemu":
                    command = [profile["runtime"]["qemu"], "-L", profile["tools"]["sysroot"]] + command
                row = prep.runner.run("native-sdk-topic", command, prep.out, prep.environment)
                content, _ = body_pin(Path(row["log"]["path"]), profile["limits"]["log_bytes"])
                expected = b"PASS_ROS_DISTRO_NATIVE_TOPIC sent=1 received=1 mismatches=0\n"
                require(content.splitlines(keepends=True).count(expected) == 1 and
                        b"FAIL_ROS_DISTRO_NATIVE_TOPIC" not in content,
                        "bounded native topic witness did not attest exact independent counters")
                prep.state["behavior_status"] = "VALIDATED"
                prep.state["validated_scope"] = ["native_sdk_string_pubsub_same_process"]
        else:
            prep.state["behavior_status"] = "UNVERIFIED_NOT_REQUESTED"
        prep.state["scope"] = "NATIVE_GATEWAY_BUILD_AND_OPTIONAL_BASIC_SDK_TOPIC_ONLY"
    except BaseException as caught:
        error = caught
    status = prep.finish(error)
    print(json.dumps({"status": status, "report": str(prep.out / "report.json"),
                      "build_status": prep.state["build_status"], "behavior_status": prep.state["behavior_status"]}))
    return 0 if status == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
