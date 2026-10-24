"""Scoped native Spot tests and installed import."""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import time


def main():
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original common finite owner required")
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--dependencies", type=Path, required=True)
    parser.add_argument(
        "--test-root",
        type=Path,
        required=True,
        help="Canonical task08/platform/boston_dynamics/tests/python directory",
    )
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    dependencies = args.dependencies.resolve(strict=True)
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    tests = source / "test/0073.ros-tests/support/source/task08"
    test_root = args.test_root.resolve(strict=True)
    rows = []
    environment = dict(
        os.environ, OPENBLAS_NUM_THREADS="1", OMP_NUM_THREADS="1", MKL_NUM_THREADS="1"
    )
    for name, test in (
        ("sessions", test_root / "native_sessions_contract.py"),
        ("sensors", test_root / "native_sensors_loopback.py"),
        ("leases", tests / "spot_native_lease_loopback.py"),
    ):
        command = [
            sys.executable,
            "-I",
            "-S",
            "-B",
            str(test),
            "--source",
            str(source),
            "--dependencies",
            str(dependencies),
            "--output",
            str(work / (name + ".json")),
        ]
        with (
            (work / (name + ".stdout")).open("xb") as out,
            (work / (name + ".stderr")).open("xb") as err,
        ):
            result = subprocess.run(
                command,
                env=environment,
                stdin=subprocess.DEVNULL,
                stdout=out,
                stderr=err,
                timeout=45,
                check=False,
            )
        rows.append(dict(name=name, argv=command, exit_code=result.returncode))
        print(json.dumps(rows[-1]), flush=True)
    sys.path[:0] = [
        str(dependencies),
        str(source / "sdk/lib/platform/boston_dynamics/py/native"),
    ]
    file = source / "sdk/example/platform/boston_dynamics/py/controlled_sequence.py"
    spec = importlib.util.spec_from_file_location("installed_spot_sequence", file)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    from uwvm_mobile_manipulation.common.native_gate import NativeGate, SERVICES
    from uwvm_mobile_manipulation.spot.native_provider import SpotProvider
    from uwvm_mobile_manipulation.spot.native_sessions import SpotNativeSessions

    native = SpotProvider(None, None, None, gate=NativeGate())
    owner, robot = object(), object()
    sessions = SpotNativeSessions()
    binding = sessions.attach_native_only(
        owner, robot, native, expires_at_ns=time.monotonic_ns() + 10_000_000_000
    )
    aliases = {
        name: SERVICES[operation]
        for name, operation in (
            ("state", "state"),
            ("acquire", "lease.acquire"),
            ("retain", "lease.retain"),
            ("stand", "command.stand"),
            ("velocity", "command.velocity"),
            ("feedback", "command.feedback"),
        )
    }
    report = module.run_authenticated(
        sessions, binding, owner, aliases, 101, 1001, (0.0, 0.0, 0.0, 1)
    )
    assert isinstance(report["primary"], PermissionError)
    assert report["cleanup"] is None and not binding.body_exclusion
    assert native._body_state == "none" and native.diagnostics()[0].state == "reserved"
    rows.append(
        dict(
            name="actual installed native package/component import and empty-policy denial",
            exit_code=0,
            native_sdk_rpcs=0,
        )
    )
    hashes = []
    for pattern in (
        "sdk/origin/robots/mobile_manipulation/spot/**/*.py",
        "sdk/lib/platform/boston_dynamics/py/native/**/*.py",
        "sdk/example/platform/boston_dynamics/py/**/*.py",
        "test/0073.ros-tests/support/source/task08/*spot*.py",
    ):
        for file in sorted(source.glob(pattern)):
            body = file.read_bytes()
            compile(body, str(file), "exec")
            hashes.append(
                dict(
                    path=str(file.relative_to(source)),
                    bytes=len(body),
                    sha256=hashlib.sha256(body).hexdigest(),
                )
            )
    for file in sorted(test_root.glob("*.py")):
        body = file.read_bytes()
        compile(body, str(file), "exec")
        hashes.append(
            dict(
                path=str(file),
                bytes=len(body),
                sha256=hashlib.sha256(body).hexdigest(),
                role="canonical-test-source",
            )
        )
    (work / "spot-followup02.json").write_text(
        json.dumps(
            dict(
                schema="uwvm-task08-spot-followup02-v1",
                rows=rows,
                input_hashes=hashes,
                robot_cases=0,
                ros_cases=0,
                gateway_cases=0,
                wasm_guest_cases=0,
                default_authority=[],
            ),
            indent=2,
            sort_keys=True,
        )
        + "\n"
    )
    if any(row["exit_code"] for row in rows):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
