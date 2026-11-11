#!/usr/bin/env python3
"""Bit-exact GC code-generation contracts, including slow-path and borrow boundaries."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import time

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--assembler", required=True, type=Path)
    parser.add_argument("--assembler-style", choices=("wasm-tools", "wat2wasm", "wat-to-wasm"), default="wasm-tools")
    parser.add_argument("--runner-json", default="[]", help="JSON argv prefix, e.g. an existing QEMU and sysroot")
    parser.add_argument("--platform", default="native")
    parser.add_argument("--timeout", type=float, default=900)
    args = parser.parse_args()
    root = Path(__file__).resolve().parent / "numeric_codegen"
    cases = json.loads((root / "cases.json").read_text())
    prefix = json.loads(args.runner_json)
    if not isinstance(prefix, list) or any(not isinstance(x, str) for x in prefix):
        parser.error("--runner-json must be an argv array of strings")
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    wasm = {}
    sources = {c["source"] for c in cases}
    sources.update(c["preload"] for c in cases if c.get("preload"))
    for name in sorted(sources):
        src = root / name
        dst = out / (src.stem + ".wasm")
        if args.assembler_style == "wasm-tools":
            cmd = [str(args.assembler), "parse", str(src), "-o", str(dst)]
        elif args.assembler_style == "wat2wasm":
            cmd = [str(args.assembler), "--enable-all", str(src), "-o", str(dst)]
        else:
            cmd = [str(args.assembler), str(src), str(dst)]
        result = subprocess.run(cmd, capture_output=True, timeout=args.timeout)
        (out / (src.stem + ".assemble.log")).write_bytes(result.stdout + result.stderr)
        if result.returncode:
            raise RuntimeError(f"assembly failed: {src}; see {out}")
        wasm[name] = dst
    binary_hash = hashlib.file_digest(args.vm.open("rb"), "sha256").hexdigest()
    rows = []
    for case in cases:
        for engine in ("int", "aot"):
            flags = ["--wasm-feature-enable-gc", "--wasm-feature-enable-function-references",
                     "--runtime-disable-memtag", "--wasm-set-start-func", str(case["entry"])]
            if case.get("preload"):
                flags += ["--wasm-preload-library", str(wasm[case["preload"]]), "provider"]
            cmd = prefix + [str(args.vm), "-R" + engine, "-Rct", "0", "-Rllvm-cache-path", "disable",
                            *flags, "--run", str(wasm[case["source"]])]
            start = time.monotonic()
            try:
                result = subprocess.run(cmd, capture_output=True, timeout=args.timeout)
                exit_code, output = result.returncode, result.stdout + result.stderr
            except subprocess.TimeoutExpired as exc:
                exit_code, output = None, (exc.stdout or b"") + (exc.stderr or b"")
            name = case["name"] + "-" + engine
            (out / (name + ".log")).write_bytes(output)
            expected = 121 if case["trap"] else 0
            passed = exit_code == expected and (b"[crash] execution" in output if case["trap"] else not output)
            rows.append(dict(name=name, platform=args.platform, engine=engine, expected=expected,
                             exit=exit_code, passed=passed, seconds=time.monotonic()-start, argv=cmd))
            (out / "results.json").write_text(json.dumps(dict(complete=False, binary_sha256=binary_hash, rows=rows), indent=2))
            if not passed:
                print(f"FAIL {args.platform} {name}: {exit_code} != {expected}", flush=True)
    failed = sum(not r["passed"] for r in rows)
    (out / "results.json").write_text(json.dumps(dict(complete=True, binary_sha256=binary_hash, rows=rows, failed=failed), indent=2))
    print(f"{args.platform}: {len(rows)} cases, {failed} failures", flush=True)
    return bool(failed)

if __name__ == "__main__":
    raise SystemExit(main())
