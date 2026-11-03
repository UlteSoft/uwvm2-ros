#!/usr/bin/env python3
"""Generate an untested acceptance ledger from public source inventories."""
import argparse
import json
from pathlib import Path
import re

from receipt import DIMENSIONS, save, sha

FAMILIES = {
    "parser": ["binary-sections", "leb-and-lengths", "malformed-and-truncated", "feature-admission", "quotas-and-allocation"],
    "initializer": ["imports-and-cycles", "constants-and-start", "active-passive-data", "elements-and-tables", "failure-rollback"],
    "wasm1": ["numeric-and-fp", "control-and-calls", "memory-and-tables", "validation-and-traps"],
    "wasm1p1": ["sign-extension", "nontrapping-conversion", "clang-cpp-producer-profile"],
    "wasm2": ["multi-value", "bulk-memory", "reference-types", "simd"],
    "wasm3-and-implemented-proposals": ["typed-function-references", "gc-and-subtyping", "tail-calls", "exceptions",
        "extended-constants", "multi-memory", "memory64", "table64", "relaxed-simd", "shared-memory-and-atomics"],
    "llvm": ["ir-verifier", "native-abi", "floating-environment", "memory-and-table-bounds", "exceptions-and-tail-unwind"],
    "cache": ["cold-write-two-fresh-hits", "source-and-elf-identity", "signature-and-corruption", "relocation-and-data",
        "permissions-and-keyring", "reboot-origin", "no-restored-authority"],
    "debugger": ["native-disassembly-and-step", "crash-wasm-frames", "stale-identity", "language-metadata", "broker-authority"],
    "audit": ["allow-deny-records", "bounded-io", "lost-peer-and-timeouts", "sensitive-field-handling"],
    "hotupdate": ["checkpoint-source-binding", "native-safe-point", "ownership-and-revocation", "atomic-commit-rollback",
        "thread-and-queue-quiescence", "incompatible-version-refusal"],
    "platform": ["exact-elf-abi", "native-execution", "qemu-user", "qemu-full-system", "native-ros-peer", "native-performance"],
}


def row(name, category, **facts):
    return {"id": name, "category": category, **facts,
            "dimensions": {name: {"status": "UNTESTED", "evidence": []} for name in DIMENSIONS}}


def generate(source, sdk=None):
    rows = [row(f"{family}/{name}", family) for family, names in FAMILIES.items() for name in names]
    ros_path = source / "src/uwvm2/uwvm/imported/ros/host.h"
    ros = ros_path.read_text()
    names = dict(re.findall(r'char8_t\s+(\w+)\[\]\s*=\s*u8"([^"\n]+)"', ros))
    modules = re.findall(r'module_name\s*\{u8"(uwvm_ros[^"\n]+)"\};(.*?)\n\s*};', ros, re.S)
    if len(modules) != 7:
        raise ValueError("public ROS namespace inventory changed; review inventory extraction")
    for module, body in modules:
        exports = re.findall(r'wrapper::(\w+_name)', body)
        if not exports or any(name not in names for name in exports):
            raise ValueError("public ROS export extraction incomplete: " + module)
        for name in exports:
            rows.append(row("ros/" + module + "/" + names[name], "ros-import", namespace=module, export=names[name],
                            source=str(ros_path.relative_to(source)), source_sha256=sha(ros_path),
                            required_cases=["positive-native-witness", "denied", "malformed", "stale-or-revoked", "deadline-and-concurrency"]))
    wasi_path = source / "src/uwvm2/uwvm/imported/wasi/wasip1/local_imported/define.h"
    wasi = wasi_path.read_text()
    aliases = dict(re.findall(r'char8_t\s+name_(\w+)\[\]\s*=\s*u8"([^"\n]+)"', wasi))
    wasi_modules = re.findall(r'module_name\s*\{u8"([^"\n]+)"\};(.*?)\n\s*};', wasi, re.S)
    if not wasi_modules:
        raise ValueError("public WASI namespace extraction empty")
    for module, body in wasi_modules:
        exports = re.findall(r'details::(\w+)', body)
        if not exports or any(name not in aliases for name in exports):
            raise ValueError("public WASI export extraction incomplete: " + module)
        for name in exports:
            rows.append(row("wasi/" + module + "/" + aliases[name], "wasi-import", namespace=module, export=aliases[name],
                            source=str(wasi_path.relative_to(source)), source_sha256=sha(wasi_path),
                            required_cases=["positive", "denied", "pointer-and-length-boundaries", "rights-and-lifetime", "host-error-mapping"]))
    # Preview 2/3 paths or platform names alone cannot establish a component implementation.
    for preview in ("preview2", "preview3"):
        rows.append(row("wasi/" + preview + "/implementation-audit", "wasi-component",
                        required_cases=["component-parser", "canonical-abi", "interface-inventory", "real-component-execution"],
                        note="Audit implemented scope first; unsupported interfaces remain NOT_IMPLEMENTED, not skipped passes."))
    if sdk is not None:
        adapter = sdk / "tools/xmake/language_adapter.lua"
        text = adapter.read_text()
        languages = {"c", "cpp"} | set(re.findall(r'\b(\w+)\s*=\s*"tools/language/', text)) | set(re.findall(r'\b(\w+)\s*=\s*true', text))
        for language in sorted(languages):
            for width in (32, 64):
                for artifact in ("binary", "static", "shared"):
                    for backend in (1, 2):
                        rows.append(row(f"sdk/{language}/wasm{width}/{artifact}/ros{backend}", "sdk-language",
                            language=language, width=width, artifact_kind=artifact, backend=backend,
                            adapter_source_sha256=sha(adapter),
                            required_cases=["real-language-source", "producer-identity", "binary-structure", "link-and-load", "guest-run", "native-ros", "deny-and-revoke"],
                            note="A declared adapter is a candidate. Unsupported artifact kinds require explicit NOT_IMPLEMENTED evidence."))
    return rows


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--sdk-root", type=Path)
    parser.add_argument("--platform", required=True, help="Exact target tuple, including OS/architecture/ABI/endian/libc")
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    if args.out.exists():
        parser.error("fresh ledger output required")
    source = args.source.resolve(strict=True)
    sdk = args.sdk_root.resolve(strict=True) if args.sdk_root else None
    rows = generate(source, sdk)
    save(args.out, {"schema_version": 1, "status": "UNTESTED", "platform": args.platform,
                    "source": str(source), "sdk": str(sdk) if sdk else None,
                    "scope": "Planning inventory only. No compiler, VM, ROS process or QEMU was executed.", "cases": rows})
    print(json.dumps({"status": "UNTESTED", "cases": len(rows), "out": str(args.out)}))


if __name__ == "__main__":
    main()
