#!/usr/bin/env python3
"""Exercise managed static extents declared by two modules importing the same memory."""
import argparse
import hashlib
import json
import resource
import subprocess
from pathlib import Path

from initialization_matrix import HEADER, exports, text
from vm_matrix import c, op, sec, u, memory_limits

SLOTS = 257
BASE = 64


def manifest(indices, step):
    descriptors = b"".join(u(0) + u(BASE + step * i + 3) + u(7) for i in indices)
    return sec(0, text("uwvm.memtag.linear.globals") + u(2) + u(len(indices)) + descriptors)


def programs(wide, distant=False):
    bits = 64 if wide else 32
    slots = 15 if distant else SLOTS
    step = 96 if distant else 16
    limits = memory_limits(wide,maximum=1)
    memory = sec(5, u(1) + limits)
    owner = HEADER + memory + exports([("mem", 2, 0)]) + manifest(range(0, slots, 2), step)
    imported = sec(2, u(1) + text("owner") + text("mem") + bytes([2]) + limits)
    alias = HEADER + imported + exports([("mem", 2, 0)])
    main_import = sec(2, u(1) + text("alias") + text("mem") + bytes([2]) + limits)
    get = b"\x20\x00"
    set_local = b"\x21\x00"
    add = b"\x7c" if wide else b"\x6a"
    eqz = b"\x50" if wide else b"\x45"
    equal = b"\x51" if wide else b"\x46"
    less = b"\x54" if wide else b"\x49"
    trap_if = b"\x04\x40\x00\x0b"
    load_color = op(0x27) + op(0x21)
    # A mismatching observation reaches unreachable. One loop per gap keeps the
    # executable small. The dense fixture covers all near pairs, and the
    # distant fixture uses every color once across the imported owner.
    code = b""
    for gap in range(1, slots if distant else 6):
        a = get + load_color
        b = get + c(gap * step, bits) + add + load_color
        checks = a + eqz + trap_if + b + eqz + trap_if + a + b + equal + trap_if
        code += (c(BASE, bits) + set_local + b"\x03\x40" + checks +
                 get + c(step, bits) + add + set_local +
                 get + c(BASE + (slots-gap)*step, bits) + less + b"\x0d\x00\x0b")
    body = b"\x01\x01" + bytes([0x7e if wide else 0x7f]) + code + b"\x0b"
    main = (HEADER + sec(1,b"\x01\x60\x00\x00") + main_import + sec(3,b"\x01\x00") +
            exports([("_start",0,0)]) + sec(10,u(1)+u(len(body))+body) + manifest(range(1,slots,2), step))
    return owner, alias, main


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--vm", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    args.out.mkdir(parents=True, exist_ok=True)
    cases = []
    for wide in (False, True):
        for disabled, distant in ((False, False), (True, False), (False, True)):
            density = "-distant" if distant else ""
            cohort = args.out / f'{64 if wide else 32}-{"off" if disabled else "on"}{density}'
            cohort.mkdir(exist_ok=True)
            for name, data in zip(("owner","alias","main"), programs(wide, distant), strict=True):
                (cohort / f"{name}.wasm").write_bytes(data)
            for engine in ("int","aot"):
                command = [str(args.vm.resolve()), f"-R{engine}", "--runtime-compile-threads","0",
                           "--wasm-policy-global", "parser.memory64", "on","--wasm-preload-library",str(cohort/"owner.wasm"),"owner",
                           "--wasm-preload-library",str(cohort/"alias.wasm"),"alias"]
                if disabled:
                    command.extend(['--wasm-policy-global', 'runtime.memtag-checks', 'off'])
                command += ["--run", str(cohort/"main.wasm")]
                result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=60)
                (cohort / f"{engine}.log").write_bytes(result.stdout)
                cases.append(dict(bits=64 if wide else 32, disabled=disabled, distant=distant, engine=engine,
                                  command=command, exit=result.returncode))
    report = dict(cases=cases, passed=sum(case["exit"]==0 for case in cases),
                  vm=hashlib.sha256(args.vm.read_bytes()).hexdigest())
    (args.out / "results.json").write_text(json.dumps(report, indent=2)+"\n")
    return int(report["passed"] != len(cases))


if __name__ == "__main__":
    raise SystemExit(main())
