#!/usr/bin/env python3
"""Small production CLI boundaries for preload order and unique dependency edges."""
import argparse
from pathlib import Path
import subprocess
import tempfile


HEADER = b"\0asm\x01\0\0\0"


def section(kind, payload):
    # All fixtures fit the one-byte canonical unsigned LEB encoding.
    assert len(payload) < 128
    return bytes([kind, len(payload)]) + payload


def name(value):
    return bytes([len(value)]) + value.encode("ascii")


def module(import_names, export_names):
    body = HEADER + section(1, b"\x01\x60\x00\x00")
    if import_names:
        body += section(2, bytes([len(import_names)]) +
                        b"".join(name(owner) + name(field) + b"\0\0" for owner, field in import_names))
    body += section(3, b"\x01\x00")
    body += section(7, bytes([len(export_names)]) +
                    b"".join(name(field) + bytes([0, len(import_names)]) for field in export_names))
    return body + section(10, b"\x01\x02\x00\x0b")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--work-dir", required=True, type=Path)
    args = parser.parse_args()
    runtime = args.runtime.resolve()
    args.work_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(dir=args.work_dir) as directory:
        root = Path(directory)
        main_wasm = root / "main.wasm"
        provider = root / "provider.wasm"
        empty = root / "empty.wasm"
        empty.write_bytes(HEADER)
        main_wasm.write_bytes(module([("app.provider", "a"), ("app.provider", "b")], ["a", "_start"]))
        provider.write_bytes(module([], ["a", "b"]))

        def check(description, options, expected, diagnostic=""):
            result = subprocess.run([runtime, *options], cwd=root, capture_output=True, timeout=20)
            output = (result.stdout + result.stderr).decode(errors="replace")
            if result.returncode != expected or diagnostic not in output:
                raise AssertionError(f"{description}: status {result.returncode}, expected {expected}\n{output}")

        preload = ["--wasm-preload-library", provider, "app.provider"]
        main_run = ["--wasm-set-main-module-name", "app.main", "--run", main_wasm]
        count = ["--wasm-process-initializer-limit", "runtime_modules", "1"]
        byte_limit = ["--wasm-set-resource-limit", "parser", "input-bytes", str(len(HEADER))]
        for before in (True, False):
            ordered = lambda option: [*option, *preload] if before else [*preload, *option]
            check("preload count", [*ordered(count), *main_run], 126, "runtime_modules")
            check("preload image bytes", [*ordered(byte_limit), "--mode", "section-details", "--run", empty], 127)
        edge = ["--wasm-process-initializer-limit", "dependency_edges"]
        check("duplicate imports use one edge", [*edge, "1", *preload, *main_run], 0)
        check("one edge over zero", [*edge, "0", *preload, *main_run], 127, "dependency_edges")
        check("last host budget wins", [*edge, "0", *preload, *edge, "1", *main_run], 0)
        check("guest argv keeps host option text", [*preload, *main_run, *edge, "0"], 0)

        provider.write_bytes(module([("app.main", "a")], ["a", "b"]))
        paths = ["--wasm-process-initializer-limit", "cycle_path_entries"]
        check("two modules form a three-entry cycle", [*paths, "3", *preload, *main_run], 0)
        check("closing name exceeds path quota", [*paths, "2", *preload, *main_run], 127, "cycle_path_entries")


if __name__ == "__main__":
    main()
