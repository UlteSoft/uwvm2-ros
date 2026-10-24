#!/usr/bin/env python3
"""@file check_backend_layout.py
@brief Check an explicit SDK checkout for canonical ROS backend paths.
@details This source-layout regression checks directories, public references and
frozen Python package parents. It does not execute SDK programs or qualify a VM.
"""

import argparse
import json
from pathlib import Path
import re

LANGUAGES = (
    "as", "c", "c_ffi_bridge", "cpp", "csharp", "d", "fsharp", "go",
    "haskell", "java", "js", "kotlin", "lua", "moonbit", "nim", "objectc",
    "odin", "php", "py", "r", "rs", "ruby", "swift", "tinygo", "vbnet",
    "zig", "fortran", "wat", "typescript",
)


def check(root):
    """@brief Inspect actual public source paths without rewriting the checkout.
    @param root Explicit SDK repository root or a complete source snapshot.
    @return A report containing each violation and the inspected scope.
    @exception OSError An input cannot be read completely.
    """
    root = Path(root).resolve(strict=True)
    platforms = sorted(path.name for path in (root / "include/platform").iterdir() if path.is_dir())
    violations = []
    directory_count = 0
    for section in ("include", "src", "lib", "tools"):
        base = root / section / "platform"
        for platform in platforms:
            for language in LANGUAGES:
                folder = base / platform / language
                if not folder.is_dir():
                    continue
                for path in folder.iterdir():
                    if not path.is_dir():
                        continue
                    directory_count += 1
                    if re.fullmatch(r"ros[_-]?[12][_-].+", path.name, re.IGNORECASE):
                        violations.append({"kind": "backend-directory", "path": str(path.relative_to(root))})

    vendor_pattern = "|".join(map(re.escape, platforms))
    language_pattern = "|".join(LANGUAGES)
    old_reference = re.compile(
        rf"(?<![A-Za-z0-9_])(?:{vendor_pattern})([/\.])(?:{language_pattern})\1ros[12]_[A-Za-z0-9_]+"
    )
    source_count = 0
    sections = ("include/platform", "src/platform", "tools", "share/platform", "example/platform", "document")
    for section in sections:
        for path in (root / section).rglob("*"):
            if not path.is_file() or path.suffix in (".wasm", ".xmz", ".a", ".o", ".so", ".png", ".pdf"):
                continue
            raw = path.read_bytes()
            if b"\0" in raw:
                continue
            try:
                text = raw.decode("utf-8")
            except UnicodeDecodeError:
                continue
            source_count += 1
            if "ros1_" not in text and "ros2_" not in text:
                continue
            match = old_reference.search(text)
            if match:
                violations.append({
                    "kind": "public-reference",
                    "path": str(path.relative_to(root)),
                    "reference": match.group(),
                })

    python_modules = 0

    def packages(value, manifest):
        """@brief Check explicit package parents in each independently frozen graph."""
        nonlocal python_modules
        if isinstance(value, dict):
            modules = value.get("py")
            if isinstance(modules, list) and all(isinstance(item, dict) for item in modules):
                indexed = {item.get("name"): item for item in modules}
                for name, item in indexed.items():
                    if not isinstance(name, str) or not re.match(rf"^(?:{vendor_pattern})\.py\.ros[12]\.", name):
                        continue
                    python_modules += 1
                    parent = ".".join(name.split(".")[:3])
                    declaration = indexed.get(parent)
                    if not declaration or declaration.get("package") is not True:
                        violations.append({"kind": "python-parent", "path": manifest, "module": parent})
                    elif not (root / declaration["path"]).is_file():
                        violations.append({"kind": "python-parent-source", "path": manifest, "module": parent})
                    source = item.get("path")
                    if source and not (root / source).is_file():
                        violations.append({"kind": "python-module-source", "path": manifest, "module": name})
            for item in value.values():
                packages(item, manifest)
        elif isinstance(value, list):
            for item in value:
                packages(item, manifest)

    for path in (root / "share/platform").glob("*/provider.json"):
        packages(json.loads(path.read_text()), str(path.relative_to(root)))
    return {
        "schema": "sdk-backend-layout-check-v1",
        "platforms": len(platforms),
        "directories": directory_count,
        "source_files": source_count,
        "frozen_python_modules": python_modules,
        "violations": violations,
        "passed": not violations,
        "language_execution_qualified": False,
    }


def main():
    """@brief Report violations for one selected source tree and return its status."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk-root", type=Path, required=True)
    args = parser.parse_args()
    report = check(args.sdk_root)
    print(json.dumps(report, indent=2))
    return 0 if report["passed"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
