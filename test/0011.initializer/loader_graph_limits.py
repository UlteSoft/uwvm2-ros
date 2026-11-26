#!/usr/bin/env python3
"""Production CLI boundaries for preload order, dependencies and instance allocations."""
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


def module(import_names, export_names, *, memory_import=None, table_size=None):
    body = HEADER + section(1, b"\x01\x60\x00\x00")
    imports = [name(owner) + name(field) + b"\0\0" for owner, field in import_names]
    if memory_import:
        imports.append(name(memory_import) + name("m") + b"\x02\x00\x01")
    if imports:
        body += section(2, bytes([len(imports)]) + b"".join(imports))
    body += section(3, b"\x01\x00")
    if table_size is not None:
        body += section(4, b"\x01\x70\x00" + bytes([table_size]))
    body += section(7, bytes([len(export_names)]) +
                    b"".join(name(field) + bytes([0, len(import_names)]) for field in export_names))
    return body + section(10, b"\x01\x02\x00\x0b")


def reference_budget_cases(root, runtime, check):
    main = root / "reference-main.wasm"
    owner = root / "reference-owner.wasm"
    flags = ["-Rint", "--wasm-policy-global", "parser.gc", "on",
             "--wasm-policy-global", "parser.reference-types", "on",
             "--wasm-policy-global", "parser.function-references", "on",
             "--wasm-policy-global", "parser.bulk-memory", "on",
             "--wasm-policy-global", "parser.extended-const", "on",
             "--wasm-policy-global", "parser.table-initializer", "on"]

    def program(types, function_type, declarations, imports=b"", elements=b""):
        return (HEADER + section(1, types) + imports + section(3, bytes((1, function_type))) +
                declarations + section(7, b"\x01" + name("_start") + b"\x00\x00") + elements +
                section(10, b"\x01\x02\x00\x0b"))

    # (array i32) and (func). A constant expression creates a four-element array.
    array = section(6, bytes((1, 0x64, 0, 0, 0x41, 4, 0xfb, 7, 0, 0x0b)))
    main.write_bytes(program(bytes((2, 0x5e, 0x7f, 0, 0x60, 0, 0)), 1, array))
    check("GC constant with heap space", [*flags, "--run", main], 0)
    check("GC constant without heap space",
          [*flags, "--wasm-set-resource-limit", "runtime", "gc-single-bytes", "0", "--run", main], 127, "runtime gc-single-bytes")

    # The provider exports extern.convert_any(ref.i31(37)).
    bridge = bytes((0x41, 37, 0xfb, 28, 0xfb, 27, 0x0b))
    bridge_global = section(6, bytes((1, 0x6f, 0)) + bridge)
    owner.write_bytes(HEADER + bridge_global +
                      section(7, b"\x01" + name("bridge") + b"\x03\x00"))
    imports = section(2, b"\x01" + name("provider") + name("bridge") + bytes((3, 0x6f, 0)))
    signature = b"\x01\x60\x00\x00"
    preload = ["--wasm-preload-library", owner, "provider", "--run", main]
    limit = ["--wasm-set-resource-limit", "runtime", "auxiliary-live-bytes"]
    null = bytes((0xd0, 0x6f, 0x0b))
    # Measure the exact startup footprint of each control on this host ABI.
    def minimum_auxiliary(label):
        low, high = 0, 65536
        check(label, [*flags, *limit, str(high), *preload], 0)
        while low < high:
            probe = (low + high) // 2
            run = subprocess.run([runtime, *flags, *limit, str(probe), *preload], cwd=root, capture_output=True, timeout=20)
            if run.returncode == 0:
                high = probe
            elif run.returncode == 127 and b"auxiliary-" in run.stderr:
                low = probe + 1
            else:
                raise AssertionError(f"{label} at {probe}: status {run.returncode}, {run.stderr!r}")
        return str(low)

    main.write_bytes(program(signature, 0, section(6, bytes((1, 0x6f, 0)) + null), imports))
    cap = minimum_auxiliary("null global control")
    global_get = bytes((0x23, 0, 0x0b))
    cases = {
        "global": (section(6, bytes((1, 0x6f, 0)) + global_get), b""),
        "table initializer": (section(4, bytes((1, 0x40, 0, 0x6f, 0, 1)) + global_get), b""),
        "passive element": (b"", section(9, bytes((1, 5, 0x6f, 1)) + global_get)),
        "active element": (section(4, bytes((1, 0x6f, 0, 1))),
                           section(9, bytes((1, 6, 0, 0x41, 0, 0x0b, 0x6f, 1)) + global_get)),
    }
    for label, (declaration, elements) in cases.items():
        main.write_bytes(program(signature, 0, declaration, imports, elements))
        check(label + " retains a foreign bridge", [*flags, *preload], 0)
        check(label + " refuses the extra lease", [*flags, *limit, cap, *preload], 127, "runtime auxiliary-single-bytes")

    # The segment issuer owns this bridge. Only the destination table needs a
    # foreign lease when the active segment writes through the imported table.
    table = bytes((0x6f, 0, 1))
    owner.write_bytes(HEADER + section(4, b"\x01" + table) + section(7, b"\x01" + name("table") + b"\x01\x00"))
    imports = section(2, b"\x01" + name("provider") + name("table") + b"\x01" + table)
    element = bytes((1, 6, 0, 0x41, 0, 0x0b, 0x6f, 1))
    main.write_bytes(program(signature, 0, bridge_global, imports, section(9, element + null)))
    cap = minimum_auxiliary("null imported table write")
    main.write_bytes(program(signature, 0, bridge_global, imports, section(9, element + bridge)))
    check("active write to imported table", [*flags, *preload], 0)
    check("imported table rejects the destination lease", [*flags, *limit, cap, *preload], 127, "runtime auxiliary-single-bytes")


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

        provider.write_bytes(HEADER + section(5, b"\x01\x00\x01") +
                             section(7, b"\x01" + name("m") + b"\x02\x00"))
        main_wasm.write_bytes(module([], ["_start"], memory_import="app.provider"))
        global_zero = ["--wasm-policy-global", "init.allocation-bytes", "0"]
        group = ["--wasm-policy-group", "owners", "init.allocation-bytes", "65536",
                 "--wasm-policy-group-add-module", "owners", "app.provider"]
        check("imported alias borrows the admitted owner", [*global_zero, *group, *preload, *main_run], 0)
        check("single owner is one byte over",
              [*global_zero, *group, "--wasm-policy-single", "app.provider", "init.allocation-bytes", "65535",
               *preload, *main_run], 127, "init.allocation-bytes")
        check("process ceiling still applies",
              [*group, "--wasm-set-resource-limit", "initializer", "allocation-bytes", "65535",
               *preload, *main_run], 127, "byte budget")
        for slots in (0, 1):
            main_wasm.write_bytes(module([], ["_start"], table_size=slots))
            check(f"table slots at zero byte limit: {slots}", [*global_zero, *main_run], 0 if slots == 0 else 127)

        reference_budget_cases(root, runtime, check)


if __name__ == "__main__":
    main()
