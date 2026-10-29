#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
"""Install an explicitly pinned Linux VM image and optional SDK command config.

Requires Linux and Python 3.11 or newer. Copies one actual ELF into a new
version directory. It does not
execute the image, grant ROS/WASI rights, install a native gateway, or turn a
producer receipt into consumer qualification. The deployment's own admission,
process limits and full-lifetime lease remain the caller's responsibility.

Example (the caller supplies real image/receipt identities and a new prefix):
  python3 tools/runtime/install.py --image /producer/uwvm2-ros \
    --sha256 IMAGE_SHA256 --source-identity sha256:SOURCE_SHA256 \
    --producer-receipt /producer/receipt.json --producer-receipt-sha256 RECEIPT_SHA256 \
    --prefix /home/user/.local/opt/uwvm2-ros

To create uwvm-ros-command-run-v1, also supply --target, --backend,
--distribution and --runtime-arguments-json. That file must contain the exact
argument array. There is no implicit ROS enable flag, grant, or preload.
"""
from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import stat
import sys


MINIMUM_RESERVE = 512 * 1024**2
MAXIMUM_IMAGE = 128 * 1024**2
MAXIMUM_JSON = 2 * 1024**2


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def digest(value: str) -> str:
    require(re.fullmatch(r"[0-9a-f]{64}", value) is not None,
            "Use the complete lowercase SHA-256 digest")
    return value


def regular_input(path: Path, maximum: int):
    descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW)
    try:
        info = os.fstat(descriptor)
        require(stat.S_ISREG(info.st_mode) and 0 < info.st_size <= maximum,
                "Expected a bounded regular input file: " + str(path))
        return os.fdopen(descriptor, "rb"), info
    except BaseException:
        os.close(descriptor)
        raise


def unchanged(before, after) -> bool:
    return (before.st_dev, before.st_ino, before.st_size, before.st_mtime_ns, before.st_ctime_ns) == (
        after.st_dev, after.st_ino, after.st_size, after.st_mtime_ns, after.st_ctime_ns)


def json_input(path: Path) -> tuple[bytes, object]:
    stream, before = regular_input(path, MAXIMUM_JSON)
    with stream:
        data = stream.read(MAXIMUM_JSON + 1)
        require(len(data) == before.st_size and unchanged(before, os.fstat(stream.fileno())),
                "JSON input changed while reading")
    return data, json.loads(data)


def arguments(value: object, label: str) -> list[str]:
    require(isinstance(value, list) and all(isinstance(item, str) and "\0" not in item for item in value),
            label + " must be an explicit array of NUL-free strings")
    return value


def new_json(path: Path, value: object) -> None:
    data = (json.dumps(value, indent=2, sort_keys=True) + "\n").encode()
    with path.open("xb") as stream:
        stream.write(data)
        stream.flush()
        os.fsync(stream.fileno())
    path.chmod(0o644)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--image", type=Path, required=True)
    parser.add_argument("--sha256", required=True)
    parser.add_argument("--source-identity", required=True)
    parser.add_argument("--producer-receipt", type=Path, required=True)
    parser.add_argument("--producer-receipt-sha256", required=True)
    parser.add_argument("--prefix", type=Path, required=True)
    parser.add_argument("--target")
    parser.add_argument("--backend", choices=("ros1", "ros2"))
    parser.add_argument("--distribution")
    parser.add_argument("--runtime-arguments-json", type=Path)
    parser.add_argument("--application-arguments-json", type=Path)
    parser.add_argument("--reserve-bytes", type=int, default=MINIMUM_RESERVE)
    args = parser.parse_args()
    require(sys.platform == "linux", "This installation profile is Linux-only")
    expected = digest(args.sha256)
    require(args.source_identity.startswith("sha256:"), "Expected an explicit sha256 source identity")
    digest(args.source_identity[7:])
    expected_receipt = digest(args.producer_receipt_sha256)
    require(args.reserve_bytes >= MINIMUM_RESERVE, "The installation reserve cannot be below 512 MiB")
    require(args.prefix.is_absolute() and ".." not in args.prefix.parts,
            "Use an absolute installation prefix without parent traversal")
    require(args.prefix.parent.is_dir() and args.prefix.parent.resolve() == args.prefix.parent,
            "The installation parent must already exist without symlink components")
    if args.prefix.exists():
        require(args.prefix.is_dir() and not args.prefix.is_symlink(), "Installation prefix is not a real directory")
    receipt_bytes, receipt = json_input(args.producer_receipt)
    require(hashlib.sha256(receipt_bytes).hexdigest() == expected_receipt, "Producer receipt digest mismatch")
    require(isinstance(receipt, dict), "Producer receipt must be a JSON object")
    artifact = receipt.get("artifact")
    require(isinstance(artifact, dict) and artifact.get("sha256") == expected
            and receipt.get("source_identity") == args.source_identity,
            "Producer receipt does not bind the requested image and source identity")
    config_fields = (args.target, args.backend, args.distribution, args.runtime_arguments_json)
    configured = any(value is not None for value in config_fields)
    runtime_arguments = application_arguments = None
    if configured:
        require(all(value is not None for value in config_fields),
                "Command config requires target, backend, distribution and runtime-arguments-json together")
        require(args.target in ("wasm32-wasip1", "wasm64-wasip1", "wasm32-wasip1-threads", "wasm64-wasip1-threads",
                                "wasm32-wasip2", "wasm64-wasip2", "wasm32-wasip2-threads", "wasm64-wasip2-threads"),
                "Select an explicit SDK compiler triple; declaring it does not qualify its execution support")
        require(re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_.-]{0,63}", args.distribution) is not None,
                "Invalid explicit ROS distribution")
        _, value = json_input(args.runtime_arguments_json)
        runtime_arguments = arguments(value, "Runtime arguments")
        require(not any(item in ("--run", "-r") for item in runtime_arguments),
                "The SDK launcher appends the sole application --run argument")
        application_arguments = []
        if args.application_arguments_json:
            _, value = json_input(args.application_arguments_json)
            application_arguments = arguments(value, "Application arguments")
    else:
        require(args.application_arguments_json is None, "Application arguments require an explicit command config")
    image, before = regular_input(args.image, MAXIMUM_IMAGE)
    with image:
        require(type(artifact.get("bytes")) is int and artifact["bytes"] == before.st_size,
                "Producer receipt image size mismatch")
        require(stat.S_IMODE(before.st_mode) & 0o111, "Source image has no execute bit")
        require(image.read(4) == b"\x7fELF", "Expected the actual Linux ELF image")
        image.seek(0)
        actual = hashlib.file_digest(image, "sha256").hexdigest()
        require(actual == expected and unchanged(before, os.fstat(image.fileno())), "Runtime image digest or identity mismatch")
        image.seek(0)
        needed = args.reserve_bytes + before.st_size + len(receipt_bytes) + 2 * 1024**2
        require(shutil.disk_usage(args.prefix.parent).free >= needed, "Insufficient installation space reserve")
        args.prefix.mkdir(mode=0o755, exist_ok=True)
        versions = args.prefix / "versions"
        versions.mkdir(mode=0o755, exist_ok=True)
        require(not versions.is_symlink() and versions.is_dir(), "Version parent is not a real directory")
        destination = versions / expected
        # Exclusive creation preserves an existing or interrupted installation.
        # An incomplete directory is never automatically reused or removed.
        destination.mkdir(mode=0o700)
        binary_directory = destination / "bin"
        binary_directory.mkdir(mode=0o755)
        executable = binary_directory / "uwvm2-ros"
        copied = hashlib.sha256()
        with executable.open("xb") as output:
            while chunk := image.read(1024**2):
                output.write(chunk)
                copied.update(chunk)
                require(shutil.disk_usage(destination).free >= args.reserve_bytes,
                        "Installation reserve reached while copying; incomplete version preserved")
            output.flush()
            os.fsync(output.fileno())
        require(copied.hexdigest() == expected and unchanged(before, os.fstat(image.fileno())),
                "Source changed during installation; incomplete version preserved")
        executable.chmod(0o755)
    with (destination / "producer-receipt.json").open("xb") as output:
        output.write(receipt_bytes)
        output.flush()
        os.fsync(output.fileno())
    (destination / "producer-receipt.json").chmod(0o644)
    config = None
    if configured:
        config = destination / "command-run.json"
        new_json(config, {"format": "uwvm-ros-command-run-v1", "runtime": str(executable),
            "target": args.target, "backend": args.backend, "distribution": args.distribution,
            "runtime_arguments": runtime_arguments, "application_arguments": application_arguments})
    new_json(destination / "installation.json", {
        "format": "uwvm2-ros-runtime-installation-v1", "utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
        "runtime": "bin/uwvm2-ros", "runtime_sha256": expected, "runtime_bytes": before.st_size,
        "source_identity": args.source_identity, "producer_receipt": "producer-receipt.json",
        "producer_receipt_sha256": expected_receipt, "installer_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        "host_platform": "linux", "command_run_config": config.name if config else None,
        "execution_performed": False, "native_gateway_installed": False, "consumer_qualification_transferred": False})
    destination.chmod(0o755)
    for path in (binary_directory, destination, versions, args.prefix):
        directory = os.open(path, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW)
        try:
            os.fsync(directory)
        finally:
            os.close(directory)
    print(json.dumps({"status": "INSTALLED_PINNED_IMAGE_ONLY", "installation": str(destination),
        "runtime": str(executable), "runtime_sha256": expected, "command_run_config": str(config) if config else None,
        "execution_performed": False, "consumer_qualification_transferred": False}, sort_keys=True))


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, json.JSONDecodeError) as error:
        print("runtime installation refused: " + str(error), file=sys.stderr)
        raise SystemExit(1)
