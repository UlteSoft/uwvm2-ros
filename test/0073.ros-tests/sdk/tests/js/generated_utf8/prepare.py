#!/usr/bin/env python3
"""Prepare a disposable real-Wasm regression project from an installed SDK.

@brief Exercise both generated string profiles without modifying the SDK.
@details Copy only the declared build-input closure into a fresh directory.
The synthetic record has no native type identity or ROS permission. Generated
modules use the same public namespace and shared QuickJS engine as SDK users.
Run this preparation and all subsequent builds inside the Linux test cgroup.
"""

from pathlib import Path
import argparse
import hashlib
import json
import shutil
import sys
import types

sys.dont_write_bytecode = True


def digest(path):
    """@brief Hash complete source and engine bytes without a partial-file shortcut."""
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def load_module(name, path):
    """@brief Load captured generator bytes without creating Python cache files."""
    source = path.read_bytes()
    module = types.ModuleType(name)
    module.__file__ = str(path)
    sys.modules[name] = module
    exec(compile(source, str(path), "exec", dont_inherit=True), module.__dict__)
    if hashlib.sha256(source).hexdigest() != digest(path):
        raise RuntimeError("Generator changed during preparation")
    return module


def main():
    """@brief Create the project, exact bundle closure and input manifest."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk-source", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    fixture = Path(__file__).resolve().parent
    source = args.sdk_source.resolve(strict=True)
    output = args.output.resolve()
    paths = json.loads((fixture / "inputs.json").read_text())
    if len(paths) != len(set(paths)):
        raise ValueError("Duplicate source input")
    files = []
    for relative in paths:
        member = Path(relative)
        if member.is_absolute() or ".." in member.parts or member.as_posix() != relative:
            raise ValueError("Noncanonical source member")
        file = source / member
        if not file.is_file() or not file.resolve().is_relative_to(source):
            raise ValueError("Missing or escaping source input: " + relative)
        files.append((relative, file, digest(file)))
    if sum(file.stat().st_size for _, file, _ in files) > 512 * 1024**2:
        raise ValueError("The declared thin input closure exceeds 512 MiB")
    output.mkdir(parents=True, exist_ok=False)
    sdk = output / "sdk"
    manifest = []
    for relative, file, expected in files:
        destination = sdk / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(file, destination)
        if digest(destination) != expected or digest(file) != expected:
            raise RuntimeError("Source input changed: " + relative)
        manifest.append({"path": relative, "sha256": expected})
    parent = load_module("_utf8_parent", sdk / "tools/custom_types.py")
    generator = load_module("_utf8_generator", sdk / "tools/custom_types/bounded_profile.py")
    record = generator.Record(
        version=2,
        name="test/msg/Text",
        members=[generator.Field(name="text", type="string", string_bound=32)],
        identity=None,
        source={"fixture": "synthetic-emitter-only"},
        minimum=4,
        maximum=36,
        cartesian_maximum=36,
    )
    records = {(2, record.name): record}
    generated = sdk / "include/platform/kuka/js/ros2/kuka-ros2-complete"
    (generated / "generated_bounded.mjs").write_text(generator.emit_js(parent, records, [record]))
    record.profile = generator.CONTROL_PROFILE
    (generated / "generated_control.mjs").write_text(generator.emit_control_js(parent, records, [record]))
    modules = [
        ("ros/js/wire", sdk / "include/platform/ros/js/wire.mjs"),
        ("kuka/js/ros2/kuka-ros2-complete/generated_bounded.mjs", generated / "generated_bounded.mjs"),
        ("kuka/js/ros2/kuka-ros2-complete/generated_control.mjs", generated / "generated_control.mjs"),
    ]
    bundle = {
        "format": "uwvm-language-bundle-v1",
        "language": "js",
        "backend": "ros2",
        "modules": [
            {"name": name, "file": str(file.relative_to(sdk)), "sha256": digest(file)}
            for name, file in modules
        ],
    }
    bundle_file = sdk / "share/qualification/generated-utf8.json"
    bundle_file.parent.mkdir(parents=True)
    bundle_file.write_text(json.dumps(bundle, indent=2) + "\n")
    project = output / "project"
    project.mkdir()
    for name in ("main.mjs", "xmake.lua"):
        shutil.copy2(fixture / name, project / name)
    (output / "source-manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print("Prepared independent project: " + str(project))


if __name__ == "__main__":
    main()
