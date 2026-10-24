"""Targeted actual C11 staged-record codec build/run and Wasm validation."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--test-source", type=Path, required=True)
    p.add_argument("--generated", type=Path, required=True)
    p.add_argument("--wasi-sdk", type=Path, required=True)
    a = p.parse_args()
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("shared finite owner required")
    out = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work" / "staged-codec"
    out.mkdir()
    rows = []

    def run(name, args):
        result = subprocess.run([str(x) for x in args], capture_output=True, timeout=90)
        (out / (name + ".stdout")).write_bytes(result.stdout)
        (out / (name + ".stderr")).write_bytes(result.stderr)
        rows.append(
            {
                "case": name,
                "argv": [str(x) for x in args],
                "exit_code": result.returncode,
            }
        )
        if result.returncode:
            raise RuntimeError(name + " failed; original logs retained")

    run(
        "native-build",
        [
            "/usr/bin/clang-21",
            "-std=c11",
            "-O2",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-pedantic-errors",
            "-isystem",
            a.generated,
            a.test_source,
            "-o",
            out / "staged-codec",
        ],
    )
    run("native-run", [out / "staged-codec"])
    run("wasi-version", [a.wasi_sdk / "bin/clang", "--version"])
    run(
        "wasi-build",
        [
            a.wasi_sdk / "bin/clang",
            "--sysroot=" + str(a.wasi_sdk / "share/wasi-sysroot"),
            "-std=c11",
            "-O2",
            "-Wall",
            "-Wextra",
            "-Werror",
            "-pedantic-errors",
            "-isystem",
            a.generated,
            a.test_source,
            "-o",
            out / "staged-codec.wasm",
        ],
    )
    run("wasm-validation", ["/usr/bin/wasm-validate", out / "staged-codec.wasm"])
    receipt = {
        "format": "uwvm-groot-staged-c-codec-qualification-v1",
        "cases": rows,
        "source_sha256": hashlib.sha256(a.test_source.read_bytes()).hexdigest(),
        "generated_header_sha256": hashlib.sha256(
            (a.generated / "bounded_records.h").read_bytes()
        ).hexdigest(),
        "wasm_sha256": hashlib.sha256(
            (out / "staged-codec.wasm").read_bytes()
        ).hexdigest(),
        "wasm_bytes": (out / "staged-codec.wasm").stat().st_size,
        "model_calls": 0,
        "native_ros_exchanges": 0,
        "wasm_guest_executions": 0,
        "gateway_integrations": 0,
        "official_upstream_suites": 0,
        "default_authority": [],
        "generated_dependency_diagnostics": "Original generated header is an -isystem dependency; handwritten test retains -Wall -Wextra -Werror -pedantic-errors. The four unused generated wire_size parameters are reported separately to task10.",
    }
    (out / "receipt.json").write_text(
        json.dumps(receipt, indent=2, sort_keys=True) + "\n"
    )
    print((out / "native-run.stdout").read_text(), end="")
    print(json.dumps(receipt, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
