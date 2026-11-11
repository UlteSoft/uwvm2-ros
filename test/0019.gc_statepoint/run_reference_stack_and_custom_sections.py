#!/usr/bin/env python3
"""GC implicit-drop and custom-section placement regressions on a supplied VM."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

def leb(n):
    out = bytearray()
    while n >= 128:
        out.append((n & 127) | 128)
        n >>= 7
    out.append(n)
    return bytes(out)

def section(k, payload):
    return bytes([k]) + leb(len(payload)) + payload

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--assembler", required=True, type=Path)
    parser.add_argument("--runner-json", default="[]")
    parser.add_argument("--timeout", type=float, default=180)
    args = parser.parse_args()
    prefix = json.loads(args.runner_json)
    if not isinstance(prefix, list) or any(not isinstance(x, str) for x in prefix):
        parser.error("--runner-json must be an argv array")
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    cases = []
    sources = {
        "i31": "(ref.i31 (i32.const 7))",
        "struct": "(struct.new $S (i32.const 7))",
        "array": "(array.new $A (i32.const 7) (i32.const 1))",
        "nullany": "(ref.null any)", "nullfunc": "(ref.null func)",
        "funcref": "(ref.func $dummy)", "nullextern": "(ref.null extern)",
        "nullexn": "(ref.null exn)",
    }
    for label, junk in sources.items():
        for op in ("return", "br", "br_if", "br_table", "return_call", "void_return"):
            if op == "return":
                body = junk + " (return (i32.const -1))"
            elif op == "br":
                body = f"(block $b (result i32) {junk} (br $b (i32.const -1)))"
            elif op == "br_if":
                body = f"(block $b (result i32) {junk} i32.const -1 i32.const 1 br_if $b unreachable)"
            elif op == "br_table":
                body = f"(block $b (result i32) {junk} i32.const -1 i32.const 0 br_table $b $b)"
            elif op == "return_call":
                body = junk + " (return_call $tail)"
            else:
                body = junk + " return"
            result = "" if op == "void_return" else "(result i32)"
            check = ("i32.const 123 call $f i32.const 123 i32.ne if unreachable end"
                     if op == "void_return" else "call $f i32.const -1 i32.ne if unreachable end")
            wat = (f"(module (type $S (struct (field i32))) (type $A (array i32)) "
                   f"(type $F (func)) (func $dummy (type $F)) (func $tail (result i32) i32.const -1) "
                   f'(elem declare func $dummy) (func $f {result} {body}) (func (export "_start") {check}))')
            name = label + "-" + op
            source = out / (name + ".wat")
            source.write_text(wat)
            wasm = source.with_suffix(".wasm")
            subprocess.run([str(args.assembler), "parse", str(source), "-o", str(wasm)], check=True, timeout=args.timeout)
            cases.append(dict(name=name, path=wasm, entry=3, expected=0, modes=("int", "aot")))

    magic = b"\0asm\1\0\0\0"
    core = [section(1, b"\x01\x60\0\0"), section(3, b"\x01\0"),
            section(7, b"\x01\x06_start\0\0"), section(10, b"\x01\x02\0\x0b")]
    payloads = {"probe": b"\x05probe", "name": b"\x04name",
                "name-type": b"\x04name\x04\x04\x01\0\x01T", "producers": b"\x09producers\0",
                "name-invalid-metadata": b"\x04name\xff\xff"}
    for label, payload in payloads.items():
        for position in range(len(core) + 1):
            name = f"{label}-position-{position}"
            wasm = out / (name + ".wasm")
            wasm.write_bytes(magic + b"".join(core[:position] + [section(0, payload)] + core[position:]))
            cases.append(dict(name=name, path=wasm, entry=0, expected=0, modes=("validation", "int", "aot")))
    malformed = {
        "standard-out-of-order": core[1] + section(0, b"\x04name") + core[0] + b"".join(core[2:]),
        "duplicate-standard-type": core[0] + section(0, b"\x04name") + b"".join(core),
        "custom-truncated-name": section(0, b"\x05name") + b"".join(core),
        "custom-invalid-name-utf8": section(0, b"\x01\xff") + b"".join(core),
        "custom-missing-name-length": section(0, b"") + b"".join(core),
        "custom-section-out-of-bounds": b"\0\xff\x01\x04name",
    }
    for name, payload in malformed.items():
        wasm = out / (name + ".wasm")
        wasm.write_bytes(magic + payload)
        cases.append(dict(name=name, path=wasm, entry=0, expected=127, modes=("validation", "int", "aot")))
    wasm = out / "duplicate-custom-name.wasm"
    wasm.write_bytes(magic + section(0, b"\x04name") + b"".join(core) + section(0, b"\x04name"))
    cases.append(dict(name=wasm.stem, path=wasm, entry=0, expected=0, modes=("validation", "int", "aot")))

    flags = ["-WFE-gc", "-WFE-function-references", "-WFE-exceptions", "-WFE-tail-call"]
    rows = []
    digest = hashlib.file_digest(args.vm.open("rb"), "sha256").hexdigest()
    def save(complete):
        (out / "results.json").write_text(json.dumps(dict(complete=complete, binary_sha256=digest,
            rows=rows, failed=sum(not x["passed"] for x in rows)), indent=2))
    for case in cases:
        for mode in case["modes"]:
            opts = (["-m", "validation"] if mode == "validation" else
                    ["-R" + mode, "-Rct", "0", "-Rllvm-cache-path", "disable", "--wasm-set-start-func", str(case["entry"])])
            cmd = prefix + [str(args.vm), *opts, *flags, "--run", str(case["path"])]
            try:
                result = subprocess.run(cmd, capture_output=True, timeout=args.timeout)
                code, data = result.returncode, result.stdout + result.stderr
            except subprocess.TimeoutExpired as exc:
                code, data = None, (exc.stdout or b"") + (exc.stderr or b"")
            log = out / (case["name"] + "-" + mode + ".log")
            log.write_bytes(data)
            passed = code == case["expected"] and (case["expected"] == 0 or b"[error]" in data)
            rows.append(dict(name=case["name"], mode=mode, expected=case["expected"], exit=code, passed=passed, argv=cmd))
            save(False)
    save(True)
    failed = sum(not x["passed"] for x in rows)
    print(f"{len(rows)} checks, {failed} failures", flush=True)
    return bool(failed)

if __name__ == "__main__":
    raise SystemExit(main())
