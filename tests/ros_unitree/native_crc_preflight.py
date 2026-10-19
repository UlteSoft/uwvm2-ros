#!/usr/bin/env python3
"""Compare the actual official SDK2 Python and native CRC implementations.

This finite native actor imports official IDL/default objects and the pinned
upstream CRC module. It never imports the SDK2 channel factory, initializes a
DDS participant, publishes a command, opens a robot connection or steps any
physics. Its use of name-mangled packers is deliberate and bound to the exact
official source blob: the SDK exposes no public API for its native padded CRC
input. No replacement packing layout or handwritten CRC is implemented here.

The parent must select --sdk-upstream and --native-crc-input from its actual
complete input lock, bind the same paths read-only inside its native view, and
retain actual native library provenance and owned-process cleanup. Successful
records qualify these finite CRC observations only, never ROS/DDS/firmware,
cryptographic authenticity or a physical robot's behavior.
"""
import argparse
import base64
import hashlib
import importlib
from pathlib import Path
import platform
import struct
import sys

from common import child_arguments, require, write_record
from preflight_support import admit, file_pin, mapped_native_file, official_file, package_files, preserve


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    child_arguments(parser)
    parser.add_argument("--sdk-upstream", required=True)
    parser.add_argument("--native-crc-input", required=True)
    args = parser.parse_args()
    config, guard, pins, roots = admit(args, "native_crc_preflight.py")
    require(args.sdk_upstream in config["upstreams"] and args.native_crc_input in pins,
            "explicit original SDK2 upstream and native CRC file input required")
    root = Path(config["upstreams"][args.sdk_upstream]["root"]).resolve(strict=True)
    source = root / "unitree_sdk2py/utils/crc.py"
    source_proof = official_file(config, pins, args.sdk_upstream, source)
    machine = platform.machine()
    require(platform.system() == "Linux" and machine in ("x86_64", "aarch64"),
            "official pinned native CRC supports only the selected actual x86_64/AArch64 host")
    native_path = root / "unitree_sdk2py/utils/lib" / ("crc_amd64.so" if machine == "x86_64" else "crc_aarch64.so")
    native = file_pin(native_path)
    require(all(native[key] == pins[args.native_crc_input][key] for key in native),
            "explicit CRC input is not the actual official architecture-selected native file")
    native_proof = official_file(config, pins, args.sdk_upstream, native_path)
    guard()
    crc_module = importlib.import_module("unitree_sdk2py.utils.crc")
    require(Path(crc_module.__file__).resolve(strict=True) == source and
            file_pin(source) == source_proof["file"], "a different SDK2 CRC Python module was imported")
    defaults = importlib.import_module("unitree_sdk2py.idl.default")
    require(Path(defaults.__file__).resolve(strict=True).is_relative_to(root),
            "actual default IDL objects must come from the selected official SDK2 tree")
    crc = crc_module.CRC()
    require(crc.platform == "Linux" and Path(crc.crc_lib._name).resolve(strict=True) == native_path,
            "actual official CRC instance selected a different native library")
    mapping = mapped_native_file(args, native, guard)
    write_record({"kind": "native-crc-preflight-loaded", "protocol": "unitree-native-crc-preflight-v1",
                  "actual_host_machine": machine, "python_crc_source": source_proof,
                  "native_crc_source": native_proof, "opened_native_mapping": mapping,
                  "dds_initialized": False, "scope": "Actual finite native/Python CRC agreement only; CRC is not authentication"})
    # Exercise the generic official algorithms independently of message packers.
    # These are word arrays, not canonical ROS/CDR bytes or guest wire buffers.
    vectors = ([], [0], [0xFFFFFFFF], [0, 1, 0x80000000, 0xFFFFFFFF],
               [(index * 0x01010101) & 0xFFFFFFFF for index in range(256)])
    observed = 0
    for index, words in enumerate(vectors):
        guard()
        python_value, native_value = int(crc._crc_py(words)), int(crc._crc_ctypes(words))
        require(0 <= python_value <= 0xFFFFFFFF and python_value == native_value,
                "actual official generic Python/native CRC implementations differ")
        raw = struct.pack("<" + str(len(words)) + "I", *words)
        write_record({"kind": "native-crc-word-vector", "index": index, "word_count": len(words),
                      "payload_sha256": hashlib.sha256(raw).hexdigest(), "crc": native_value})
        observed += 1
    types = (("go", "LowCmd", 812), ("go", "LowState", 1180),
             ("hg", "LowCmd", 1004), ("hg", "LowState", 2092))
    for family, kind, packed_size in types:
        constructor = getattr(defaults, "unitree_" + family + "_msg_dds__" + kind + "_")
        packer = getattr(crc, "_CRC__Pack" + ("HG" if family == "hg" else "") + kind)
        for variant in ("default", "finite-active-fields"):
            guard()
            message = constructor()
            capacity = 20 if family == "go" else 35
            require(message.__idl_typename__ == "unitree_" + family + ".msg.dds_." + kind + "_",
                    "official default constructor returned a different actual IDL object")
            if variant == "finite-active-fields":
                if family == "go":
                    message.head[0], message.head[1], message.level_flag = 0xFE, 0xEF, 0xFF
                else:
                    message.mode_pr, message.mode_machine = 0, 5
                motors = message.motor_cmd if kind == "LowCmd" else message.motor_state
                require(len(motors) == capacity, "actual official motor carrier length differs")
                for slot, motor in enumerate(motors):
                    motor.mode, motor.q, motor.dq = slot % 2, -0.25 + slot / 128, slot / 64
                    if kind == "LowCmd":
                        motor.kp, motor.kd, motor.tau = 20 + slot / 8, 1 + slot / 256, -slot / 32
                    else:
                        motor.ddq, motor.tau_est = -slot / 128, slot / 32
                if kind == "LowState":
                    message.tick = 123
                    message.imu_state.quaternion[0] = 1.0
            words = packer(message)
            require(type(words) is list and len(words) * 4 == packed_size - 4 and
                    all(type(value) is int and 0 <= value <= 0xFFFFFFFF for value in words),
                    "actual official padded CRC input word layout differs")
            raw = struct.pack("<" + str(len(words)) + "I", *words)
            python_value = int(crc._crc_py(words))
            native_value = int(crc._crc_ctypes(words))
            public_value = int(crc.Crc(message))
            require(0 <= native_value <= 0xFFFFFFFF and python_value == native_value == public_value,
                    "actual official message dispatch/packing/native/Python CRC implementations differ")
            message.crc = public_value ^ 0xFFFFFFFF
            require(packer(message) == words and int(crc.Crc(message)) == public_value,
                    "last CRC word was unexpectedly part of the official CRC input")
            write_record({"kind": "native-crc-message", "family": family, "message": kind,
                          "variant": variant, "padded_native_bytes": packed_size, "word_count": len(words),
                          "payload_sha256": hashlib.sha256(raw).hexdigest(),
                          "payload_base64": base64.b64encode(raw).decode("ascii"), "crc": public_value})
            observed += 1
    require("unitree_sdk2py.core.channel" not in sys.modules and "rclpy" not in sys.modules,
            "CRC-only preflight unexpectedly imported native ROS/channel initialization code")
    modules = package_files(("unitree_sdk2py", "cyclonedds"), roots)
    preserve(pins, guard)
    require(file_pin(source) == source_proof["file"] and file_pin(native_path) == native,
            "actual official CRC sources changed during observation")
    write_record({"kind": "native-crc-preflight-complete", "status": "PASS_NATIVE_CRC_PREFLIGHT_ONLY",
                  "actual_cases": observed, "word_vector_cases": 5, "message_cases": 8,
                  "dds_initialized": False, "ros_initialized": False, "selected_inputs_preserved": True,
                  "module_files": modules})
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
