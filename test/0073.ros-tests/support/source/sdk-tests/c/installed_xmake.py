#!/usr/bin/env python3
"""Install, update, relocate and actually xmake-build the owned C examples.

The base is a retained ordinary installed SDK, read-only. Current owned C files
are rebound through its explicit source export map, installed by a separate
CMake overlay and pinned before qualification. No repository, sdk tree or
test fixture becomes a distributed consumer dependency.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import shutil
import subprocess
import sys
import tarfile


BASE_ARCHIVE_SHA256 = "f19d4b7ef3d40ce522e1e40dfaafebb89e54b95591bce2f062495b7a53b4fb0c"


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--formatted-source", type=Path, required=True)
    parser.add_argument("--catalog-mode", choices=("full", "recipes", "selected"), default="full")
    parser.add_argument("--standard", action="append", default=[])
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        parser.error("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs = run / "inputs"
    work = run / "work"
    source = args.formatted_source.resolve(strict=True)
    base = run.parent / "c-xmake-r11/work/relocated-sdk"
    base_archive = run.parent / "c-xmake-r11/resource-archives/evidence.tar"
    if base.is_dir():
        base_manifest_bytes = (base / "share/distribution-manifest.json").read_bytes()
    else:
        if digest(base_archive) != BASE_ARCHIVE_SHA256:
            raise RuntimeError("immutable installed base archive changed")
        with tarfile.open(base_archive) as archive:
            base_manifest_bytes = archive.extractfile("work/relocated-sdk/share/distribution-manifest.json").read()
    base_manifest = json.loads(base_manifest_bytes)
    owned = inputs / "owned"
    overlay = work / "overlay"
    overlay.mkdir()
    prefix = work / "prefix-a"
    commands = []
    os.environ["XMAKE_STATS"] = "false"
    os.environ["XMAKE_SKIP_HISTORY"] = "y"

    def command(label, argv, timeout=90):
        result = subprocess.run([str(item) for item in argv], stdin=subprocess.DEVNULL,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                timeout=timeout, check=False)
        (work / (label + ".stdout")).write_bytes(result.stdout)
        (work / (label + ".stderr")).write_bytes(result.stderr)
        commands.append({"label": label, "argv": [str(item) for item in argv],
                         "exit_code": result.returncode,
                         "stdout_sha256": hashlib.sha256(result.stdout).hexdigest(),
                         "stderr_sha256": hashlib.sha256(result.stderr).hexdigest()})
        if result.returncode:
            raise RuntimeError("failed " + label)

    exports = {}
    for row in base_manifest["entries"]:
        relative = row.get("original", {}).get("path")
        if relative:
            exports.setdefault(relative, []).append(row["path"])
    selected = {}
    for relative, destinations in exports.items():
        current = owned / relative
        if not current.is_file():
            current = source / relative
        if current.is_file() and current.suffix in (".c", ".h", ".lua", ".md"):
            selected[relative] = (current, destinations)
    for relative, destination in {
        "docs/c-api-contracts.md": "share/platform/ros/docs/c-api-contracts.md",
    }.items():
        if (owned / relative).is_file():
            selected[relative] = (owned / relative, [destination])
    expected = set((inputs / "c-format-files.txt").read_text().splitlines())
    if not expected <= set(selected):
        raise RuntimeError("owned installed source exports are missing: " + repr(sorted(expected - set(selected))))
    header_destinations = {}
    for relative, (_, destinations) in selected.items():
        if relative.endswith(".h"):
            choices = [value for value in destinations if value.startswith("include/platform/")]
            if not choices:
                continue
            if len(choices) != 1:
                raise RuntimeError("one canonical public C destination required: " + relative)
            header_destinations[relative] = choices[0]

    def rebind(relative, destination, data):
        def include(match):
            spelling = match[2]
            candidates = [posixpath.normpath(posixpath.join(posixpath.dirname(relative), spelling)),
                          posixpath.normpath(spelling), posixpath.normpath("include/" + spelling)]
            chosen = None
            for candidate in candidates:
                if candidate in header_destinations:
                    chosen = header_destinations[candidate]
                    break
                if candidate in selected:
                    destinations = selected[candidate][1]
                    if len(destinations) == 1:
                        chosen = destinations[0]
                        break
            if chosen and chosen.startswith("include/"):
                return "#include <" + chosen.removeprefix("include/") + ">"
            if chosen:
                local = posixpath.relpath(chosen, posixpath.dirname(destination))
                return '#include "' + local + '"'
            if match[1] == '"' or spelling.startswith(("roswasm/", "roswasm_unitree/")):
                raise RuntimeError("unexported local C dependency: " + relative + " -> " + spelling)
            return match[0]
        if Path(relative).suffix not in (".c", ".h"):
            return data
        return re.sub(r'#include\s*(["<])([^"<>\n]+)[">]', include, data.decode()).encode()

    updates = []
    for relative, (current, destinations) in sorted(selected.items()):
        raw = current.read_bytes()
        for destination in destinations:
            path = overlay / destination
            path.parent.mkdir(parents=True, exist_ok=True)
            body = rebind(relative, destination, raw)
            path.write_bytes(body)
            updates.append({"maintenance_path": relative, "source_sha256": digest(current),
                            "installed_path": destination, "installed_sha256": digest(path)})
    root_entry = inputs / "xmake.lua"
    if not root_entry.is_file():
        raise RuntimeError("explicit current Task01 root plus narrow C options hook required")
    shutil.copyfile(root_entry, overlay / "xmake.lua")
    updates.append({"maintenance_path": "Task01-root-with-C-options-hook", "source_sha256": digest(root_entry),
                    "installed_path": "xmake.lua", "installed_sha256": digest(root_entry)})
    (work / "overlay-source-map.json").write_text(json.dumps(updates, indent=2) + "\n")
    (work / "overlay-project").mkdir()
    (work / "overlay-project/CMakeLists.txt").write_text(
        'cmake_minimum_required(VERSION 3.20)\n'
        'project(task02_current_C_overlay LANGUAGES NONE)\n'
        'install(DIRECTORY "' + str(overlay) + '/" DESTINATION .)\n')
    if base.is_dir():
        command("install-configure", ["/usr/bin/cmake", "-S", base, "-B", work / "install-build",
                "-DCMAKE_INSTALL_PREFIX=" + str(prefix), "-DROSWASM_INSTALL_EXAMPLES=ON"])
        command("install-build", ["/usr/bin/cmake", "--build", work / "install-build", "--parallel", "1"])
        command("install-base", ["/usr/bin/cmake", "--install", work / "install-build"])
    else:
        # Restore the exact previously CMake-installed product, rather than a
        # second full source/SDK working copy. Current files still pass through
        # the explicit CMake install overlay below before prefix relocation.
        prefix.mkdir()
        with tarfile.open(base_archive) as archive:
            for member in archive:
                if not member.name.startswith("work/relocated-sdk/"):
                    continue
                relative = Path(member.name).relative_to("work/relocated-sdk")
                if any(part in (".xmake", "__pycache__") for part in relative.parts):
                    continue
                if relative.parts[:1] == ("build",) or relative.parts[:2] == ("example", "build"):
                    continue
                if relative.is_absolute() or '..' in relative.parts or not (member.isdir() or member.isfile()):
                    raise RuntimeError("ordinary confined installed base members required")
                target = prefix / relative
                if member.isdir():
                    target.mkdir(parents=True, exist_ok=True)
                else:
                    target.parent.mkdir(parents=True, exist_ok=True)
                    with archive.extractfile(member) as incoming, target.open('xb') as outgoing:
                        shutil.copyfileobj(incoming, outgoing, 1 << 20)
                    target.chmod(member.mode & 0o777)
        commands.append({"label": "restore-actual-CMake-installed-base", "archive": str(base_archive),
                         "archive_sha256": BASE_ARCHIVE_SHA256, "member_root": "work/relocated-sdk",
                         "original_install_recipe": "work/install.json in the same pinned archive"})
    # The restored base has fresh filesystem timestamps. CMake's directory
    # installer may otherwise classify those older bytes as up to date. Retire
    # only the exact mapped private-prefix preimages before the real install.
    retired_preimages = []
    for relative in sorted({row["installed_path"] for row in updates}):
        path = prefix / relative
        if path.is_symlink() or not path.is_relative_to(prefix):
            raise RuntimeError("ordinary confined private overlay path required")
        if path.exists():
            if not path.is_file():
                raise RuntimeError("mapped overlay destination is not an ordinary file")
            retired_preimages.append({"path": relative, "sha256": digest(path)})
            path.unlink()
    command("overlay-configure", ["/usr/bin/cmake", "-S", work / "overlay-project", "-B", work / "overlay-build",
            "-DCMAKE_INSTALL_PREFIX=" + str(prefix)])
    command("install-current-C", ["/usr/bin/cmake", "--install", work / "overlay-build"])
    for row in updates:
        if digest(prefix / row["installed_path"]) != row["installed_sha256"]:
            raise RuntimeError("installed current C source changed: " + row["installed_path"])
    removed_aliases = []
    for name in ("roswasm", "roswasm_unitree"):
        tree = prefix / "include" / name
        if not tree.exists():
            continue
        for path in tree.rglob("*"):
            if path.is_symlink():
                raise RuntimeError("ordinary forwarding files required")
            if path.is_file():
                if not re.fullmatch(rb"\s*(?:/\*.*?\*/\s*)?#include\s+<platform/[^>\n]+>\s*", path.read_bytes(), re.S):
                    raise RuntimeError("refusing to retire a distinct public implementation")
                removed_aliases.append({"path": path.relative_to(prefix).as_posix(), "sha256": digest(path)})
        shutil.rmtree(tree)
    sdk = work / "relocated-sdk"
    prefix.rename(sdk)
    if any((sdk / name).exists() for name in ("sdk", "test", "tests")):
        raise RuntimeError("standalone SDK must contain no sdk/test/tests dependency")
    source_overlay = {"format": "task02-current-C-installed-overlay-v1", "base_sdk": str(base),
                      "base_distribution_sha256": hashlib.sha256(base_manifest_bytes).hexdigest(),
                      "base_archive_sha256": BASE_ARCHIVE_SHA256 if not base.is_dir() else None,
                      "current_owned_updates": updates, "retired_forwarding_aliases": removed_aliases,
                      "retired_private_overlay_preimages": retired_preimages,
                      "installed_with_cmake": True, "relocated_from": str(prefix), "relocated_to": str(sdk),
                      "original_prefix_absent": not prefix.exists(), "commands": commands,
                      "global_publication": False, "default_authority": []}
    (work / "installation.json").write_text(json.dumps(source_overlay, indent=2) + "\n")
    (sdk / "share/task02-current-C-overlay.json").write_text(json.dumps(source_overlay, indent=2) + "\n")
    # The old distribution manifest is explicitly identified as the base. Fresh
    # qualification pins every current C/tool source and every produced artifact.
    shutil.rmtree(overlay)
    toolchain = run.parent.parent / "10/staged-wasi-r1/staged/wasi-sdk"
    runtime = run.parent / "c-focused-r1/work/uwvm2-ros"
    argv = [sys.executable, "-I", "-S", "-B", inputs / "qualify_xmake.py", "--sdk", sdk,
            "--output", work / "qualification", "--toolchain", toolchain,
            "--xmake", "/home/macromodel/.local/bin/xmake", "--archiver", toolchain / "bin/llvm-ar",
            "--runtime", runtime, "--catalog-mode", args.catalog_mode, "--compact"]
    for standard in args.standard:
        argv += ["--standard", standard]
    command("qualification", argv, timeout=900)


if __name__ == "__main__":
    main()
