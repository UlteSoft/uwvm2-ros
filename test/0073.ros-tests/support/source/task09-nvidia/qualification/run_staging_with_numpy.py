"""Acquire one genuine pinned NumPy wheel and run host staging contracts.

Only the shared finite Linux owner may run this. The wheel stays in this run's
work directory. This CPython3.14/NumPy2.4 host transport check is NOT the locked
GR00T Python3.12/NumPy1.26.4 environment, model inference or a Python Wasm Guest.
"""

import argparse
import hashlib
import io
import json
import os
from pathlib import Path, PurePosixPath
import stat
import subprocess
import sys
import urllib.parse
import urllib.request
import zipfile


def fetch(url, maximum):
    with urllib.request.urlopen(
        urllib.request.Request(
            url, headers={"User-Agent": "uwvm-task09-host-contract/1"}
        ),
        timeout=30,
    ) as response:
        raw = response.read(maximum + 1)
    if len(raw) > maximum:
        raise ValueError("complete download exceeds finite budget")
    return raw


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--source", type=Path, required=True)
    p.add_argument(
        "--tests",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="Canonical task09-nvidia test source root",
    )
    p.add_argument("--wasm-artifact", type=Path, required=True)
    a = p.parse_args()
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1" or sys.version_info[
        :2
    ] != (3, 14):
        raise ValueError("actual finite CPython3.14 host owner required")
    root = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    out = root / "work" / "numpy-staging"
    out.mkdir()
    filename = "numpy-2.4.2-cp314-cp314-manylinux_2_27_x86_64.manylinux_2_28_x86_64.whl"
    metadata = fetch("https://pypi.org/pypi/numpy/2.4.2/json", 2 << 20)
    (out / "pypi-metadata.json").write_bytes(metadata)
    rows = [row for row in json.loads(metadata)["urls"] if row["filename"] == filename]
    if len(rows) != 1:
        raise ValueError("one exact official CPython3.14 NumPy wheel required")
    selected = rows[0]
    if urllib.parse.urlparse(selected["url"]).hostname != "files.pythonhosted.org":
        raise ValueError("official Python package host required")
    raw = fetch(selected["url"], 32 << 20)
    digest = hashlib.sha256(raw).hexdigest()
    if digest != selected["digests"]["sha256"] or len(raw) != selected["size"]:
        raise ValueError("whole pinned wheel size/SHA differs from official metadata")
    (out / filename).write_bytes(raw)
    site = out / "site"
    site.mkdir()
    with zipfile.ZipFile(io.BytesIO(raw)) as archive:
        members = archive.infolist()
        if len(members) > 4000 or sum(item.file_size for item in members) > 128 << 20:
            raise ValueError("complete wheel extraction budget exceeded")
        for item in members:
            path = PurePosixPath(item.filename)
            if (
                path.is_absolute()
                or ".." in path.parts
                or "\\" in item.filename
                or stat.S_ISLNK(item.external_attr >> 16)
            ):
                raise ValueError("nonordinary wheel member refused")
            target = site / item.filename
            if item.is_dir():
                target.mkdir(parents=True, exist_ok=True)
            else:
                target.parent.mkdir(parents=True, exist_ok=True)
                with archive.open(item) as source, target.open("xb") as destination:
                    total = 0
                    while True:
                        block = source.read(65536)
                        if not block:
                            break
                        total += len(block)
                        if total > item.file_size:
                            raise ValueError("wheel member extent drift")
                        destination.write(block)
                if total != item.file_size:
                    raise ValueError("incomplete wheel member")
    env = os.environ.copy()
    env["PYTHONPATH"] = os.pathsep.join((str(site), str(a.source / "groot")))
    result = subprocess.run(
        [
            sys.executable,
            "-B",
            str(a.tests / "groot/tests/staged_video_contract.py"),
            "-v",
        ],
        capture_output=True,
        timeout=60,
        env=env,
    )
    (out / "staging.stdout").write_bytes(result.stdout)
    (out / "staging.stderr").write_bytes(result.stderr)
    validation = subprocess.run(
        ["/usr/bin/wasm-validate", str(a.wasm_artifact)],
        capture_output=True,
        timeout=20,
    )
    (out / "wasm-validate.stdout").write_bytes(validation.stdout)
    (out / "wasm-validate.stderr").write_bytes(validation.stderr)
    receipt = {
        "format": "uwvm-groot-host-staging-with-real-numpy-v1",
        "numpy_version": "2.4.2",
        "python": sys.version,
        "wheel": filename,
        "wheel_sha256": digest,
        "metadata_sha256": hashlib.sha256(metadata).hexdigest(),
        "wheel_bytes": len(raw),
        "staging_exit_code": result.returncode,
        "wasm_validation_exit_code": validation.returncode,
        "wasm_sha256": hashlib.sha256(a.wasm_artifact.read_bytes()).hexdigest(),
        "gr00t_sdk_environment_verified": False,
        "model_calls": 0,
        "native_ros_exchanges": 0,
        "wasm_guest_executions": 0,
        "upstream_suite_cases": 0,
        "default_authority": [],
    }
    (out / "receipt.json").write_text(
        json.dumps(receipt, indent=2, sort_keys=True) + "\n"
    )
    print(result.stderr.decode("utf-8", "replace"), end="")
    print(json.dumps(receipt, sort_keys=True))
    return 0 if result.returncode == validation.returncode == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
