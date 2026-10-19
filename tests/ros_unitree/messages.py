"""Strict fixture records backed by official Unitree SDK2 Python objects.

No private replacement DDS type or handwritten motor CRC is implemented here.
Root pins the complete SDK2 Python tree and its native CRC library. An official
CRC detects accidental corruption; it is neither a signature nor authority.
Legacy ROS1 unitree_legged_msgs has a different field/CRC layout and is never
implicitly treated as SDK2 Go. This adapter deliberately names its actual wire
families: ``go`` and ``hg``.
"""
import dataclasses
import importlib
import math
import struct

from common import exact, finite, require

FIELDS = ("mode", "q", "dq", "kp", "kd", "tau")


def sdk(family):
    require(family in ("go", "hg"), "explicit SDK2 Go/HG family required")
    name = "unitree_" + family
    types = importlib.import_module("unitree_sdk2py.idl." + name + ".msg.dds_")
    defaults = importlib.import_module("unitree_sdk2py.idl.default")
    crc = importlib.import_module("unitree_sdk2py.utils.crc").CRC()
    return {"LowCmd": types.LowCmd_, "LowState": types.LowState_,
        "command": getattr(defaults, name + "_msg_dds__LowCmd_"),
        "state": getattr(defaults, name + "_msg_dds__LowState_"), "crc": crc,
        "capacity": 20 if family == "go" else 35}


def motors(value, capacity):
    require(type(value) is list and len(value) == capacity, "exact native motor capacity required")
    result = []
    for row in value:
        exact(row, FIELDS, "motor command")
        require(type(row["mode"]) is int and row["mode"] in (0, 1), "fixture supports passive/servo motor modes only")
        result.append({"mode": row["mode"], "q": finite(row["q"], -3E9, 3E9, "q"),
            "dq": finite(row["dq"], -20000, 20000, "dq"), "kp": finite(row["kp"], 0, 1000, "kp"),
            "kd": finite(row["kd"], 0, 100, "kd"), "tau": finite(row["tau"], -500, 500, "tau")})
    return result


def command(selected, family, values, mode_machine=0):
    """Build the official default object, then set only admitted public fields."""
    values = motors(values, selected["capacity"])
    result = selected["command"]()
    if family == "go":
        result.head[0], result.head[1], result.level_flag = 0xFE, 0xEF, 0xFF
    else:
        require(type(mode_machine) is int and 0 <= mode_machine <= 255, "actual HG mode_machine required")
        result.mode_pr = 0
        result.mode_machine = mode_machine
    for target, source in zip(result.motor_cmd, values):
        for field in FIELDS:
            setattr(target, field, source[field])
    result.crc = selected["crc"].Crc(result)
    require(type(result.crc) is int and 0 <= result.crc <= 0xFFFFFFFF, "official command CRC unavailable")
    return result


def checked_command(selected, value):
    require(isinstance(value, selected["LowCmd"]) and len(value.motor_cmd) == selected["capacity"],
            "actual official native command object required")
    require(value.crc == selected["crc"].Crc(value), "native command CRC differs")
    return motors([{field: getattr(motor, field) for field in FIELDS} for motor in value.motor_cmd], selected["capacity"])


def copy_fields(target, source):
    """Copy complete ROS2/IDL field trees, preserving arrays and nested objects.

    Target dataclass fields come from the actual official SDK2 definition. ROS2
    setters perform their own generated type checks. No C struct/padding bytes
    or guessed reflection hash is used. Missing fields are a hard mismatch.
    """
    names = ([field.name for field in dataclasses.fields(target)] if dataclasses.is_dataclass(target)
             else list(target.get_fields_and_field_types()))
    for name in names:
        require(hasattr(source, name), "ROS2/SDK2 field mismatch: " + name)
        destination, value = getattr(target, name), getattr(source, name)
        if dataclasses.is_dataclass(destination) or hasattr(destination, "get_fields_and_field_types"):
            copy_fields(destination, value)
        elif (isinstance(destination, (list, tuple)) and destination and
                (dataclasses.is_dataclass(destination[0]) or hasattr(destination[0], "get_fields_and_field_types"))):
            require(len(destination) == len(value), "nested native array length differs")
            for left, right in zip(destination, value):
                copy_fields(left, right)
        else:
            if isinstance(value, (list, tuple)) or hasattr(value, "tolist"):
                value = list(value)
            setattr(target, name, value)
    return target


def controller_bytes(raw, count, nanos):
    """Decode the actual portable controller output without host byte-order assumptions."""
    require(len(raw) == 12 + count * 484 and raw[:4] == b"UCM1" and
            struct.unpack_from("<II", raw, 4) == (count, nanos), "complete exact controller binary frame required")
    result = []
    for index in range(count):
        offset = 12 + 484 * index
        require(struct.unpack_from("<I", raw, offset)[0] == index + 1, "controller step reordered")
        values = []
        for motor in range(20):
            row = struct.unpack_from("<I5f", raw, offset + 4 + motor * 24)
            values.append(dict(zip(FIELDS, row)))
        result.append(motors(values, 20))
    return result
