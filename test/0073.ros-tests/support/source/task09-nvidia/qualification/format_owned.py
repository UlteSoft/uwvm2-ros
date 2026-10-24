"""Format only current Task09 source in a finite Linux owner.

Immutable upstream/source-definition/license captures and prior evidence are
excluded. Exact official package metadata, package hashes, formatter versions,
input/output inventories, Python AST equivalence and idempotence are retained.
The script prepares a candidate tar; it never publishes into another checkout.
"""

import argparse
import ast
import fcntl
import hashlib
import inspect
import io
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile
import urllib.request
import zipfile


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def fetch(url, maximum):
    request = urllib.request.Request(
        url, headers={"User-Agent": "uwvm-task09-format-audit/1"}
    )
    with urllib.request.urlopen(request, timeout=30) as response:
        raw = response.read(maximum + 1)
    if len(raw) > maximum:
        raise ValueError("bounded official download exceeded its declared extent")
    return raw


def normalized_python(raw):
    tree = ast.parse(raw)
    for node in ast.walk(tree):
        if isinstance(
            node, (ast.Module, ast.ClassDef, ast.FunctionDef, ast.AsyncFunctionDef)
        ):
            if node.body and isinstance(node.body[0], ast.Expr):
                value = node.body[0].value
                if isinstance(value, ast.Constant) and isinstance(value.value, str):
                    value.value = inspect.cleandoc(value.value)
    return ast.dump(tree, include_attributes=False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--package-cache", type=Path)
    parser.add_argument("--archive-cache", type=Path)
    parser.add_argument(
        "--memory-tools",
        action="store_true",
        help="Use sealed anonymous memory for exact formatter binaries; charged to the same finite owner",
    )
    parser.add_argument("--qualify-current", action="store_true")
    parser.add_argument(
        "--skip-lua",
        action="store_true",
        help="Retain Lua as an explicit formatter gap after recorded acquisition failure",
    )
    args = parser.parse_args()
    assert not (args.package_cache and args.archive_cache)
    assert os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") == "1"
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    work = run / "work"
    source = work / "candidate"
    source.mkdir()
    with tarfile.open(run / "inputs/source.tar") as archive:
        members = [
            item
            for item in archive
            if not any(part.startswith("._") for part in Path(item.name).parts)
        ]
        archive.extractall(source, members=members, filter="data")
    roots = [
        source / "sdk/origin/robots/nvidia",
        source / "test/0073.ros-tests/support/source/task09-nvidia",
    ]
    excluded = {"upstream", "source-definitions", "licenses", "__pycache__"}
    suffixes = {".c", ".cpp", ".cc", ".h", ".hpp", ".py", ".go", ".lua"}
    selected = sorted(
        path
        for root in roots
        for path in root.rglob("*")
        if path.is_file()
        and path.suffix in suffixes
        and not excluded.intersection(path.relative_to(root).parts)
    )
    before = {path.relative_to(source).as_posix(): digest(path) for path in selected}
    python_ast = {
        path: normalized_python(path.read_bytes())
        for path in selected
        if path.suffix == ".py"
    }
    packages = work / "packages"
    packages.mkdir()
    tools = work / "formatter-tools"
    tools.mkdir()
    acquisition = []
    commands = []
    tool_descriptors = []
    if args.archive_cache:
        assert (
            digest(args.archive_cache)
            == "ede0921ab080fcbe526958994c5eed34dfa1e2f2489f8e5407d83f2faa143784"
        )
    result = {
        "schema": "task09-owned-format-v1",
        "status": "RUNNING",
        "exclusions": sorted(excluded),
        "inputs": before,
        "acquisition": acquisition,
        "commands": commands,
        "default_authority": [],
        "anonymous_tool_memory_charged_to_same_owner": args.memory_tools,
        "memory_disk_reserve_or_mount_changes": False,
    }

    def save():
        (work / "format-result.json").write_text(json.dumps(result, indent=2) + "\n")

    def command(label, argv):
        argv = [str(value) for value in argv]
        process = subprocess.run(
            argv,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            stdin=subprocess.DEVNULL,
            timeout=120,
            pass_fds=tuple(tool_descriptors),
        )
        (work / (label + ".stdout")).write_bytes(process.stdout)
        (work / (label + ".stderr")).write_bytes(process.stderr)
        commands.append(
            {
                "label": label,
                "argv": argv,
                "exit_code": process.returncode,
                "stdout_sha256": hashlib.sha256(process.stdout).hexdigest(),
                "stderr_sha256": hashlib.sha256(process.stderr).hexdigest(),
            }
        )
        save()
        if process.returncode:
            raise ValueError("actual formatter command failed: " + label)
        return process.stdout

    def archived_package(name):
        with tarfile.open(args.archive_cache) as archive:
            member = archive.getmember("work/packages/" + name)
            assert member.isfile() and member.size <= 20 << 20
            return archive.extractfile(member).read()

    def executable(name, raw):
        if args.memory_tools:
            flags = os.MFD_CLOEXEC | os.MFD_ALLOW_SEALING | getattr(os, "MFD_EXEC", 0)
            descriptor = os.memfd_create("task09-" + name, flags)
            with os.fdopen(os.dup(descriptor), "wb") as stream:
                stream.write(raw)
            os.fchmod(descriptor, 0o500)
            fcntl.fcntl(
                descriptor,
                fcntl.F_ADD_SEALS,
                fcntl.F_SEAL_WRITE
                | fcntl.F_SEAL_GROW
                | fcntl.F_SEAL_SHRINK
                | fcntl.F_SEAL_SEAL,
            )
            tool_descriptors.append(descriptor)
            return Path("/proc/self/fd") / str(descriptor)
        binary = tools / name
        binary.write_bytes(raw)
        binary.chmod(0o700)
        return binary

    def wheel(project, version, executable_suffix):
        url = "https://pypi.org/pypi/" + project + "/" + version + "/json"
        metadata = (
            archived_package(project + "-metadata.json")
            if args.archive_cache
            else fetch(url, 2 << 20)
        )
        (packages / (project + "-metadata.json")).write_bytes(metadata)
        choices = [
            item
            for item in json.loads(metadata)["urls"]
            if item["filename"].endswith(".whl")
            and "x86_64" in item["filename"]
            and "manylinux" in item["filename"]
            and "musllinux" not in item["filename"]
        ]
        # Prefer an ordinary manylinux wheel with no interpreter-specific ABI.
        choices = [item for item in choices if "py3-none" in item["filename"]]
        assert len(choices) == 1, (project, [item["filename"] for item in choices])
        item = choices[0]
        assert item["size"] <= 20 << 20
        assert item["url"].startswith("https://files.pythonhosted.org/")
        cached = args.package_cache / item["filename"] if args.package_cache else None
        raw = (
            archived_package(item["filename"])
            if args.archive_cache
            else (
                cached.read_bytes()
                if cached and cached.is_file()
                else fetch(item["url"], 20 << 20)
            )
        )
        assert (
            len(raw) == item["size"]
            and hashlib.sha256(raw).hexdigest() == item["digests"]["sha256"]
        )
        package_path = (
            cached if cached and cached.is_file() else packages / item["filename"]
        )
        if package_path.parent == packages and not args.archive_cache:
            package_path.write_bytes(raw)
        with zipfile.ZipFile(io.BytesIO(raw)) as archive:
            matches = [
                name for name in archive.namelist() if name.endswith(executable_suffix)
            ]
            assert len(matches) == 1
            member = archive.getinfo(matches[0])
            assert member.file_size <= 48 << 20
            binary = executable(project, archive.read(member))
        acquisition.append(
            {
                "project": project,
                "version": version,
                "metadata_url": url,
                "metadata_sha256": hashlib.sha256(metadata).hexdigest(),
                "package_url": item["url"],
                "package_bytes": len(raw),
                "package_sha256": item["digests"]["sha256"],
                "retained_package": (
                    str(args.archive_cache) + "!work/packages/" + item["filename"]
                    if args.archive_cache
                    else str(package_path)
                ),
                "retained_archive": str(args.archive_cache)
                if args.archive_cache
                else None,
                "binary_sha256": digest(binary),
            }
        )
        save()
        return binary

    try:
        clang = wheel("clang-format", "21.1.8", "/bin/clang-format")
        ruff = wheel("ruff", "0.14.14", "/scripts/ruff")
        lua_metadata_url = (
            "https://api.github.com/repos/JohnnyMorganz/StyLua/releases/tags/v2.3.1"
        )
        lua = None
        if not args.skip_lua:
            metadata = fetch(lua_metadata_url, 2 << 20)
            (packages / "stylua-metadata.json").write_bytes(metadata)
            assets = [
                item
                for item in json.loads(metadata)["assets"]
                if item["name"] == "stylua-linux-x86_64.zip"
            ]
            assert len(assets) == 1
            asset = assets[0]
            assert asset["size"] <= 16 << 20
            raw = fetch(asset["browser_download_url"], 16 << 20)
            assert len(raw) == asset["size"]
            assert asset["digest"] == "sha256:" + hashlib.sha256(raw).hexdigest()
            (packages / asset["name"]).write_bytes(raw)
            with zipfile.ZipFile(io.BytesIO(raw)) as archive:
                assert archive.namelist() == ["stylua"]
                assert archive.getinfo("stylua").file_size <= 24 << 20
                lua = executable("stylua", archive.read("stylua"))
            acquisition.append(
                {
                    "project": "StyLua",
                    "version": "2.3.1",
                    "metadata_url": lua_metadata_url,
                    "metadata_sha256": hashlib.sha256(metadata).hexdigest(),
                    "package_url": asset["browser_download_url"],
                    "package_bytes": len(raw),
                    "package_sha256": hashlib.sha256(raw).hexdigest(),
                    "binary_sha256": digest(lua),
                }
            )
        else:
            result["lua_formatter_status"] = (
                "NOT_RUN: actual v2.3.1 GitHub release download timed out in retained format-r20"
            )
        go_root = Path(
            "/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-toolchains-r1/work/toolchains/go-input/go"
        )
        gofmt = go_root / "bin/gofmt"
        acquisition.append(
            {
                "project": "gofmt",
                "version": "go1.24.2",
                "binary": str(gofmt),
                "binary_sha256": digest(gofmt),
                "read_only_shared_input": True,
            }
        )
        command("clang-format-version", [clang, "--version"])
        command("ruff-version", [ruff, "--version"])
        if lua:
            command("stylua-version", [lua, "--version"])
        command("gofmt-version", [go_root / "bin/go", "version", gofmt])
        c_files = [
            path
            for path in selected
            if path.suffix in {".c", ".cc", ".cpp", ".h", ".hpp"}
        ]
        py_files = [path for path in selected if path.suffix == ".py"]
        lua_files = [path for path in selected if path.suffix == ".lua"]
        go_files = [path for path in selected if path.suffix == ".go"]
        style = "{BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 100, SortIncludes: Never, ReflowComments: false, BreakStringLiterals: false, AllowShortFunctionsOnASingleLine: None, AllowShortIfStatementsOnASingleLine: Never}"
        command("clang-format", [clang, "--style=" + style, "-i", *c_files])
        command(
            "ruff-format",
            [ruff, "format", "--isolated", "--target-version", "py312", *py_files],
        )
        if lua:
            command(
                "stylua-format",
                [
                    lua,
                    "--verify",
                    "--indent-type",
                    "Spaces",
                    "--indent-width",
                    "4",
                    *lua_files,
                ],
            )
        command("gofmt-format", [gofmt, "-w", *go_files])
        for path, original in python_ast.items():
            assert normalized_python(path.read_bytes()) == original, str(path)
        # Formatting changes source-bound adapter bytes. Rebind only these four
        # own reviewed sibling sources, preserving the upstream SDK inventory.
        entry = roots[0] / "groot/run_captured.py"
        text = entry.read_text()
        tree = ast.parse(text)
        values = next(
            node.value
            for node in tree.body
            if isinstance(node, ast.Assign)
            and any(
                isinstance(target, ast.Name) and target.id == "EXPECTED"
                for target in node.targets
            )
        )
        for name, value in zip(values.keys, values.values):
            assert isinstance(name, ast.Constant) and isinstance(value, ast.Constant)
            text = text.replace(
                value.value, digest(entry.parent / (name.value + ".py"))
            )
        entry.write_text(text)
        command(
            "ruff-source-pin-format",
            [ruff, "format", "--isolated", "--target-version", "py312", entry],
        )
        outputs = {
            path.relative_to(source).as_posix(): digest(path) for path in selected
        }
        command(
            "clang-format-check",
            [clang, "--style=" + style, "--dry-run", "--Werror", *c_files],
        )
        command(
            "ruff-format-check",
            [
                ruff,
                "format",
                "--isolated",
                "--target-version",
                "py312",
                "--check",
                *py_files,
            ],
        )
        if lua:
            command(
                "stylua-format-check",
                [
                    lua,
                    "--check",
                    "--verify",
                    "--indent-type",
                    "Spaces",
                    "--indent-width",
                    "4",
                    *lua_files,
                ],
            )
        assert not command("gofmt-format-check", [gofmt, "-l", *go_files])
        assert outputs == {
            path.relative_to(source).as_posix(): digest(path) for path in selected
        }
        public_modules = [
            "groot/native_conversion.py",
            "groot/staged_video.py",
            "groot/native_owner.py",
            "groot/owned_sdk_source.py",
            "groot/run_captured.py",
            "isaac_sim/sensor_graph.py",
            "isaac_sim/native_graph.py",
            "isaac_sim/native_observer.py",
            "isaac_sim/native_cube_subscriber.py",
            "isaac_sim/native_clock.py",
            "isaac_sim/native_camera.py",
            "isaac_sim/native_carter.py",
            "jetson/admit_platform.py",
            "jetson/package_contract.py",
        ]
        inventory = []
        for relative in public_modules:
            tree = ast.parse((roots[0] / relative).read_bytes())

            def public(nodes, prefix=""):
                for node in nodes:
                    if not isinstance(node, (ast.FunctionDef, ast.ClassDef)):
                        continue
                    if node.name.startswith("_") and node.name != "__init__":
                        continue
                    doc = ast.get_docstring(node) or ""
                    missing = [
                        field
                        for field in ("@brief", "@details", "@see")
                        if field not in doc
                    ]
                    parameters = []
                    if isinstance(node, ast.FunctionDef):
                        parameters = [
                            arg.arg
                            for arg in node.args.args + node.args.kwonlyargs
                            if arg.arg not in {"self", "cls"}
                        ]
                        missing.extend(
                            "@param " + name
                            for name in parameters
                            if not re.search(
                                r"@param\[[^\]]+\]\s+" + re.escape(name) + r"\b", doc
                            )
                        )
                    inventory.append(
                        {
                            "module": relative,
                            "name": prefix + node.name,
                            "line": node.lineno,
                            "parameters": parameters,
                            "missing": missing,
                            "doc": doc,
                        }
                    )
                    if isinstance(node, ast.ClassDef):
                        public(node.body, prefix + node.name + ".")

            public(tree.body)
        (work / "public-python-api-inventory.json").write_text(
            json.dumps(
                {
                    "scope": "All authored top-level public classes/functions and public class methods in fourteen selected native application modules; constructors included",
                    "entries": inventory,
                    "semantic_review": "Owner compared lifetime, complete fields, quotas, actual permissions, thread rules, failure branches and version constraints to source; tag coverage alone is not acceptance",
                    "generated_public_api_docs": "PENDING shared generator owners and final publication",
                    "native_cpp_api_semantic_compile": "NOT_RUN: genuine installed modern ROS Buffer dependencies absent",
                },
                indent=2,
            )
            + "\n"
        )
        assert not [row for row in inventory if row["missing"]], (
            "Public Python documentation fields incomplete"
        )
        if args.qualify_current:
            for path in py_files:
                compile(path.read_bytes(), str(path), "exec", dont_inherit=True)
            result["syntax_interpreter"] = sys.version
            result["python_312_build_credit"] = False
            denied = [
                (
                    "groot",
                    [
                        roots[0] / "groot/run_captured.py",
                        "--sdk-root",
                        "/absent",
                        "--sdk-source-inventory",
                        "/absent",
                        "--model-root",
                        "/absent",
                        "--embodiment",
                        "absent",
                        "--device",
                        "cuda:0",
                        "--service",
                        "/infer",
                        "--domain",
                        "231",
                        "--lifetime-ms",
                        "1",
                        "--call-ms",
                        "1",
                        "--maximum-requests",
                        "1",
                    ],
                ),
                (
                    "cube",
                    [
                        roots[0] / "isaac_sim/native_cube_subscriber.py",
                        "--sdk-root",
                        "/absent",
                        "--domain",
                        "231",
                        "--topic",
                        "/uwvm_cube/denied",
                        "--frames",
                        "1",
                        "--lifetime-ms",
                        "1",
                        "--maximum-events",
                        "1",
                        "--evidence",
                        "/absent",
                    ],
                ),
            ]
            denied.append(
                (
                    "clock",
                    [
                        roots[0] / "isaac_sim/native_clock.py",
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
            )
            for program in ("manual", "periodic"):
                denied.append(
                    (
                        "camera-" + program,
                        [
                            roots[0] / "isaac_sim/native_camera.py",
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
                )
            for program, environment in (
                ("navigation", "hospital"),
                ("navigation", "office"),
                ("stereo", None),
            ):
                name = "carter-" + program + ("-" + environment if environment else "")
                arguments = [
                    roots[0] / "isaac_sim/native_carter.py",
                    "--program",
                    program,
                    "--sdk-root",
                    "/absent",
                    "--scenario-usd",
                    "/absent",
                    "--scenario-sha256",
                    "0" * 64,
                    "--domain",
                    "231",
                    "--frames",
                    "1",
                    "--lifetime-ms",
                    "1",
                    "--evidence",
                    "/absent",
                ]
                if environment:
                    arguments += ["--environment", environment]
                denied.append((name, arguments))
            checks = [(name, arguments, 13) for name, arguments in denied]
            for name, arguments in denied:
                if name.startswith("carter-"):
                    flags = [
                        "--enable-native-simulation",
                        "--enable-sim-commands",
                        "--enable-native-sensors",
                    ]
                    for index, flag in enumerate(flags):
                        checks.append(
                            (
                                name + "-single-enable-" + str(index),
                                arguments + [flag],
                                13,
                            )
                        )
                        checks.append(
                            (
                                name + "-missing-enable-" + str(index),
                                arguments + [value for value in flags if value != flag],
                                13,
                            )
                        )
                    if sys.implementation.name != "cpython" or sys.version_info[:2] != (
                        3,
                        12,
                    ):
                        checks.append(
                            (name + "-wrong-interpreter", arguments + flags, 95)
                        )
                elif name in ("clock", "cube") or name.startswith("camera-"):
                    flags = [
                        "--enable-native-simulation",
                        "--enable-sim-commands"
                        if name in ("clock", "cube")
                        else "--enable-native-sensors",
                    ]
                    for index, flag in enumerate(flags):
                        checks.append(
                            (
                                name + "-single-enable-" + str(index),
                                arguments + [flag],
                                13,
                            )
                        )
                    if sys.implementation.name != "cpython" or sys.version_info[:2] != (
                        3,
                        12,
                    ):
                        checks.append(
                            (name + "-wrong-interpreter", arguments + flags, 95)
                        )
            for name, arguments, expected in checks:
                argv = [sys.executable, "-B", *map(str, arguments)]
                process = subprocess.run(argv, capture_output=True, timeout=10)
                (work / (name + "-denied.stdout")).write_bytes(process.stdout)
                (work / (name + "-denied.stderr")).write_bytes(process.stderr)
                commands.append(
                    {
                        "label": name
                        + (
                            "-default-denial"
                            if expected == 13
                            else "-interpreter-refusal"
                        ),
                        "argv": argv,
                        "exit_code": process.returncode,
                        "expected_exit_code": expected,
                    }
                )
                assert process.returncode == expected
            test = roots[1] / "isaac_ros/qualification/cpu_sensor_contract.c"
            executable_path = work / "cpu-contract"
            command(
                "canonical-c99-build",
                [
                    "/usr/bin/clang-21",
                    "-std=c99",
                    "-O2",
                    "-Wall",
                    "-Wextra",
                    "-Werror",
                    "-pedantic-errors",
                    "-I",
                    roots[0] / "isaac_ros/include",
                    test,
                    "-o",
                    executable_path,
                ],
            )
            command("canonical-c99-run", [executable_path])
            result["qualification_scope"] = (
                "Exact host Python syntax, native admission/interpreter refusals and canonical native C99 CPU contract only; no CPython3.12 build, ROS/Sim/model or Guest credit"
            )
        result.update(
            status="PASS",
            outputs=outputs,
            input_count=len(selected),
            python_ast_equivalence=True,
            source_bound_adapter_pins_refreshed=True,
            idempotent=True,
            syntax_counts={
                "c_cpp": len(c_files),
                "python": len(py_files),
                "lua": len(lua_files) if lua else 0,
                "lua_pending": 0 if lua else len(lua_files),
                "go": len(go_files),
            },
        )
        with tarfile.open(work / "formatted-owned.tar", "w") as archive:
            for path in selected:
                archive.add(path, arcname=path.relative_to(source), recursive=False)
        result["candidate_tar_sha256"] = digest(work / "formatted-owned.tar")
        save()
    finally:
        # Exact fetched packages and metadata remain sufficient to reproduce
        # binaries; avoid duplicating their extracted copies in both archives.
        shutil.rmtree(tools)
        for descriptor in tool_descriptors:
            os.close(descriptor)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
