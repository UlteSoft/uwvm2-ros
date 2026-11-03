#!/usr/bin/env python3
"""Compile and execute a finite bootstrap probe, retaining actual tool/header bytes."""
import argparse
import json
from pathlib import Path
import subprocess
import sys

from receipt import save, sha


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--prefix", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=False)
    report = {"status": "FAIL", "commands": [], "scope": "Bootstrap only; bundled product LLVM/runtimes require their own build evidence."}
    try:
        prefix = args.prefix.resolve(strict=True)
        tools = {name: prefix / "bin" / name for name in ("clang", "clang++", "ld.lld", "llvm-ar", "llvm-config")}
        report["tools"] = {name: {"path": str(path), "sha256": sha(path)} for name, path in tools.items()}
        headers = prefix / "include/c++/v1"
        report["headers"] = {str(path.relative_to(prefix)): sha(path) for path in sorted(headers.rglob("*")) if path.is_file()}
        if not report["headers"]:
            raise RuntimeError("explicit matching libc++ headers missing")
        source = args.out / "probe.cc"
        source.write_text('#include <array>\n#include <version>\n#include <functional>\n#include <print>\n#if __clang_major__ != 23\n#error LLVM23 bootstrap required\n#endif\nconstexpr std::array values{1,2,3};\nstatic_assert(values[2] == 3);\nint main(){std::println("UWVM-CXX26-BOOTSTRAP-OK");}\n')
        binary = args.out / "probe"
        for label, argv in (("version", [str(tools["clang++"]), "--version"]),
                            ("compile", [str(tools["clang++"]), "-std=c++26", "-stdlib=libc++", "-fuse-ld=lld", str(source), "-o", str(binary)]),
                            ("execute", [str(binary)])):
            result = subprocess.run(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=120)
            row = {"id": label, "argv": argv, "returncode": result.returncode}
            for stream in ("stdout", "stderr"):
                path = args.out / (label + "." + stream)
                path.write_bytes(getattr(result, stream)); row[stream + "_sha256"] = sha(path)
            report["commands"].append(row)
            if result.returncode:
                raise RuntimeError(label + " failed")
            if label == "execute" and result.stdout != b"UWVM-CXX26-BOOTSTRAP-OK\n":
                raise RuntimeError("bootstrap execution output differs")
        report.update(status="PASS", probe_sha256=sha(binary))
    except FileNotFoundError as error:
        report.update(status="BLOCKED", reason=str(error))
    except (RuntimeError, OSError, subprocess.TimeoutExpired) as error:
        report["reason"] = str(error)
    finally:
        save(args.out / "report.json", report)
    print("toolchain: " + report["status"])
    return 0 if report["status"] == "PASS" else 77 if report["status"] == "BLOCKED" else 1


if __name__ == "__main__":
    sys.exit(main())
