"""Check the exact Python entry-selection delta with the archived real Ruff.

No CPython 3.12 package build, native dependency import or Guest is simulated.
The binary lives in sealed anonymous memory charged to the same finite owner.
"""

import argparse
import ast
import fcntl
import hashlib
import io
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile
import zipfile


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--expected-sources", type=int, default=3)
    args = parser.parse_args()
    assert 1 <= args.expected_sources <= 16
    assert os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") == "1"
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    work = run / "work"
    candidate = work / "candidate"
    candidate.mkdir()
    with tarfile.open(run / "inputs/source.tar") as archive:
        archive.extractall(candidate, filter="data")
    files = sorted(candidate.rglob("*.py"))
    assert len(files) == args.expected_sources
    before = {path.relative_to(candidate).as_posix(): digest(path) for path in files}
    trees = {
        path: ast.dump(ast.parse(path.read_bytes()), include_attributes=False)
        for path in files
    }
    cache = run.parent / "format-r18/resource-archives/evidence.tar"
    assert (
        digest(cache)
        == "ede0921ab080fcbe526958994c5eed34dfa1e2f2489f8e5407d83f2faa143784"
    )
    wheel_name = "ruff-0.14.14-py3-none-manylinux_2_17_x86_64.manylinux2014_x86_64.whl"
    with tarfile.open(cache) as archive:
        metadata = json.load(archive.extractfile("work/packages/ruff-metadata.json"))
        pin = next(row for row in metadata["urls"] if row["filename"] == wheel_name)
        raw = archive.extractfile("work/packages/" + wheel_name).read()
    assert (
        len(raw) == pin["size"]
        and hashlib.sha256(raw).hexdigest() == pin["digests"]["sha256"]
    )
    with zipfile.ZipFile(io.BytesIO(raw)) as archive:
        binary = archive.read("ruff-0.14.14.data/scripts/ruff")
    descriptor = os.memfd_create(
        "task09-ruff-delta",
        os.MFD_CLOEXEC | os.MFD_ALLOW_SEALING | getattr(os, "MFD_EXEC", 0),
    )
    with os.fdopen(os.dup(descriptor), "wb") as stream:
        stream.write(binary)
    os.fchmod(descriptor, 0o500)
    fcntl.fcntl(
        descriptor,
        fcntl.F_ADD_SEALS,
        fcntl.F_SEAL_WRITE
        | fcntl.F_SEAL_GROW
        | fcntl.F_SEAL_SHRINK
        | fcntl.F_SEAL_SEAL,
    )
    commands = []
    try:
        for name, arguments in (
            ("version", ["--version"]),
            (
                "format",
                ["format", "--isolated", "--target-version", "py312", *map(str, files)],
            ),
            (
                "check",
                [
                    "format",
                    "--isolated",
                    "--target-version",
                    "py312",
                    "--check",
                    *map(str, files),
                ],
            ),
        ):
            argv = ["/proc/self/fd/" + str(descriptor), *arguments]
            process = subprocess.run(
                argv, capture_output=True, timeout=60, pass_fds=(descriptor,)
            )
            (work / (name + ".stdout")).write_bytes(process.stdout)
            (work / (name + ".stderr")).write_bytes(process.stderr)
            commands.append(
                {
                    "argv": argv,
                    "exit_code": process.returncode,
                    "stdout_sha256": hashlib.sha256(process.stdout).hexdigest(),
                    "stderr_sha256": hashlib.sha256(process.stderr).hexdigest(),
                }
            )
            assert process.returncode == 0
        for path in files:
            assert (
                ast.dump(ast.parse(path.read_bytes()), include_attributes=False)
                == trees[path]
            )
            compile(path.read_bytes(), str(path), "exec", dont_inherit=True)
        guards, inventory = [], []
        for path in files:
            if path.name not in {"native_clock.py", "native_camera.py"}:
                continue

            def public(nodes, prefix=""):
                for node in nodes:
                    if not isinstance(node, (ast.FunctionDef, ast.ClassDef)):
                        continue
                    if node.name.startswith("_") and node.name != "__init__":
                        continue
                    doc = ast.get_docstring(node) or ""
                    missing = [
                        tag for tag in ("@brief", "@details", "@see") if tag not in doc
                    ]
                    if isinstance(node, ast.FunctionDef):
                        for parameter in node.args.args + node.args.kwonlyargs:
                            if parameter.arg not in {"self", "cls"} and not re.search(
                                r"@param\[[^\]]+\]\s+"
                                + re.escape(parameter.arg)
                                + r"\b",
                                doc,
                            ):
                                missing.append("@param " + parameter.arg)
                    inventory.append(
                        {
                            "source": path.relative_to(candidate).as_posix(),
                            "name": prefix + node.name,
                            "line": node.lineno,
                            "doc": doc,
                            "missing": missing,
                        }
                    )
                    if isinstance(node, ast.ClassDef):
                        public(node.body, prefix + node.name + ".")

            public(ast.parse(path.read_bytes()).body)
            if path.name == "native_clock.py":
                selections = [
                    (
                        "clock",
                        [
                            "--sdk-root",
                            "/absent",
                            "--domain",
                            "231",
                            "--automatic-topic",
                            "/uwvm_clock/automatic",
                            "--manual-topic",
                            "/uwvm_clock/manual",
                            "--lifetime-ms",
                            "1",
                            "--maximum-events",
                            "1",
                            "--evidence",
                            "/absent",
                        ],
                    )
                ]
                flags = ["--enable-native-simulation", "--enable-sim-commands"]
            else:
                selections = [
                    (
                        "camera-" + program,
                        [
                            "--program",
                            program,
                            "--sdk-root",
                            "/absent",
                            "--warehouse-usd",
                            "/absent",
                            "--warehouse-sha256",
                            "0" * 64,
                            "--domain",
                            "231",
                            "--rgb-topic",
                            "/uwvm_camera/rgb",
                            "--depth-topic",
                            "/uwvm_camera/depth",
                            "--info-topic",
                            "/uwvm_camera/info",
                            "--frames",
                            "1",
                            "--lifetime-ms",
                            "1",
                            "--evidence",
                            "/absent",
                        ],
                    )
                    for program in ("manual", "periodic")
                ]
                flags = ["--enable-native-simulation", "--enable-native-sensors"]
            for label, arguments in selections:
                modes = [
                    ("disabled", [], 13),
                    ("first-only", flags[:1], 13),
                    ("second-only", flags[1:], 13),
                ]
                if sys.implementation.name != "cpython" or sys.version_info[:2] != (
                    3,
                    12,
                ):
                    modes.append(("wrong-interpreter", flags, 95))
                for mode, enables, expected in modes:
                    argv = [sys.executable, "-B", str(path), *arguments, *enables]
                    process = subprocess.run(argv, capture_output=True, timeout=10)
                    name = label + "-" + mode
                    (work / (name + ".stdout")).write_bytes(process.stdout)
                    (work / (name + ".stderr")).write_bytes(process.stderr)
                    guards.append(
                        {
                            "case": name,
                            "argv": argv,
                            "exit_code": process.returncode,
                            "expected_exit_code": expected,
                            "stdout_sha256": hashlib.sha256(process.stdout).hexdigest(),
                            "stderr_sha256": hashlib.sha256(process.stderr).hexdigest(),
                        }
                    )
                    assert process.returncode == expected
        assert not [row for row in inventory if row["missing"]]
        (work / "public-python-delta-api.json").write_text(
            json.dumps(
                {
                    "scope": "Exact selected new clock/camera public declarations and constructors only",
                    "entries": inventory,
                    "native_sdk_or_generated_documentation_acceptance": False,
                },
                indent=2,
            )
            + "\n"
        )
        outputs = {
            path.relative_to(candidate).as_posix(): digest(path) for path in files
        }
        with tarfile.open(work / "formatted-delta.tar", "w") as archive:
            for path in files:
                archive.add(path, arcname=path.relative_to(candidate), recursive=False)
        result = {
            "schema": "task09-python-entry-format-delta-v1",
            "status": "PASS",
            "inputs": before,
            "outputs": outputs,
            "commands": commands,
            "formatter_binary_sha256": hashlib.sha256(binary).hexdigest(),
            "formatter_package_sha256": pin["digests"]["sha256"],
            "candidate_tar_sha256": digest(work / "formatted-delta.tar"),
            "interpreter": sys.version,
            "python_312_build_credit": False,
            "syntax_only": not guards,
            "admission_guard_checks": guards,
            "public_python_delta_api_entries": len(inventory),
            "ast_equivalence": True,
            "idempotent": True,
            "native_ros_exchanges": 0,
            "guest_executions": 0,
            "default_authority": [],
        }
        (work / "delta-result.json").write_text(json.dumps(result, indent=2) + "\n")
        print(
            json.dumps(
                {
                    "status": "PASS",
                    "source_files": len(files),
                    "python_312_build_credit": False,
                }
            )
        )
    finally:
        os.close(descriptor)


if __name__ == "__main__":
    main()
