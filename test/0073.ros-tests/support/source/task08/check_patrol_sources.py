"""Genuine Linux formatter/frontend checks for selected current Patrol sources.

This does not execute an application with a host-language runtime, produce an
installed xmake receipt, validate Wasm, or confer Guest/native integration credit.
"""

import ast
import atexit
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import urllib.parse
import urllib.request
import zipfile


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents/03")
TOOLS = {
    "clang-format": Path(
        "/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang-format"
    ),
    "rustfmt": BASE / "20261021-round03-rustfmt-r1/work/tools/bin/rustfmt",
    "gofmt": BASE / "20261021-toolchains-r1/work/toolchains/go-input/go/bin/gofmt",
    "nimpretty": BASE
    / "20261021-additional-tools-r2/work/toolchains/nim/nim-2.2.4/bin/nimpretty",
    "nim": BASE / "20261021-additional-tools-r2/work/toolchains/nim/nim-2.2.4/bin/nim",
    "odin": BASE
    / "20261021-additional-tools-r2/work/toolchains/odin/odin-linux-amd64-nightly+2025-10-05/odin",
    "zig": BASE
    / "20261021-toolchains-r1/work/toolchains/zig-input/zig-x86_64-linux-0.14.1/zig",
}
ROOT = Path("sdk/origin/robots/mobile_bases/robotis/turtlebot3/examples/patrol")
STYLE = "{BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 88, SortIncludes: Never, ReflowComments: false}"
FORMAT_PACKAGES = {
    "black": "25.1.0",
    "click": "8.1.8",
    "mypy-extensions": "1.0.0",
    "packaging": "24.2",
    "pathspec": "0.12.1",
    "platformdirs": "4.3.7",
}


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def source_pins(root):
    result = []
    for path in sorted(root.rglob("*")):
        assert not path.is_symlink(), "ordinary confined source input required"
        if path.is_file():
            result.append(
                dict(
                    path=str(path.relative_to(root)),
                    bytes=path.stat().st_size,
                    sha256=sha(path),
                )
            )
    return result


def main():
    if (
        sys.platform != "linux"
        or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1"
    ):
        raise RuntimeError("original common finite owner required")
    source = Path(sys.argv[1]).resolve(strict=True)
    manifest_path = (
        Path(sys.argv[2]).resolve(strict=True) if len(sys.argv) > 2 else None
    )
    manifest = json.loads(manifest_path.read_bytes()) if manifest_path else None
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    logs = work / "logs"
    logs.mkdir()
    formatted = work / "formatted"
    environment = dict(
        os.environ,
        HOME=str(work),
        TMPDIR=str(work),
        PYTHONDONTWRITEBYTECODE="1",
        LD_LIBRARY_PATH=(
            "/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-toolchains-r1/work/toolchains/rust/lib"
            ":/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu"
            ":/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu"
        ),
    )
    rows, commands, packages = [], [], []
    before_pins = source_pins(source)
    report = dict(
        schema="uwvm-task08-patrol-source-check-v2",
        status="STARTED",
        source_root=str(source),
        source_input_pins_before=before_pins,
        runner_sha256=sha(Path(__file__)),
        manifest_sha256=sha(manifest_path) if manifest_path else None,
        inputs=rows,
        commands=commands,
        formatters={},
        formatter_packages=packages,
        actual_xmake_builds=0,
        wasm_validations=0,
        Guest_runs=0,
        Gateway_runs=0,
        native_ROS_runs=0,
        default_authority=[],
    )

    def save():
        (work / "source-check.json").write_text(
            json.dumps(report, indent=2, sort_keys=True) + "\n"
        )

    def finalize():
        after = source_pins(source)
        report.update(
            source_input_pins_after=after, source_inputs_unchanged=before_pins == after
        )
        if "black" in report["formatters"]:
            report["formatter_inputs_unchanged"] = (
                source_pins(python_tools)
                == report["formatters"]["black"]["installed_file_pins"]
                and sha(black_python) == report["formatters"]["black"]["python_sha256"]
            )
            if not report["formatter_inputs_unchanged"]:
                report["status"] = "FAILED_RETAINED"
        if report["status"] == "STARTED" or before_pins != after:
            report["status"] = "FAILED_RETAINED"
        save()

    atexit.register(finalize)

    def run(label, argv, input_bytes=None):
        actual = list(map(str, argv))
        standard_input = (
            dict(stdin=subprocess.DEVNULL)
            if input_bytes is None
            else dict(input=input_bytes)
        )
        result = subprocess.run(
            actual,
            env=environment,
            cwd=work,
            capture_output=True,
            timeout=60,
            **standard_input,
        )
        assert len(result.stdout) + len(result.stderr) <= 1 << 20
        (logs / (label + ".stdout")).write_bytes(result.stdout)
        (logs / (label + ".stderr")).write_bytes(result.stderr)
        commands.append(
            dict(
                label=label,
                argv=actual,
                exit=result.returncode,
                stdin_sha256=(
                    hashlib.sha256(input_bytes).hexdigest()
                    if input_bytes is not None
                    else None
                ),
                stdout_sha256=sha(logs / (label + ".stdout")),
                stderr_sha256=sha(logs / (label + ".stderr")),
            )
        )
        save()
        return result

    versions = {}
    report["formatters"] = versions
    only_python = bool(manifest and manifest.get("python_only"))
    for name, tool in [] if only_python else TOOLS.items():
        assert tool.is_file()
        flag = "version" if name in ("odin", "zig") else "--version"
        # gofmt has no version flag: the actual Go distribution VERSION file
        # supplies its provenance, alongside the exact formatter byte pin.
        if name == "gofmt":
            versions[name] = dict(
                path=str(tool),
                sha256=sha(tool),
                version=(tool.parent.parent / "VERSION").read_text().strip(),
            )
        else:
            result = run(name + "-version", [tool, flag])
            versions[name] = dict(
                path=str(tool),
                sha256=sha(tool),
                exit=result.returncode,
                output=(result.stdout + result.stderr).decode(errors="replace"),
            )

    groups = {
        "clang-format": ["cpp/application.hpp", "cpp/main.cpp", "d/main.c"],
        "rustfmt": ["rust/lib.rs", "rust/main.rs"],
        "gofmt": [
            "go/main.go",
            "go/patrol/client.go",
            "go/patrol/transport_go.go",
            "go/patrol/transport_tinygo.go",
        ],
        "nimpretty": ["nim/Patrol.nim"],
        "zig": ["zig/main.zig"],
    }
    if only_python:
        groups = {}
    if manifest:
        if not only_python:
            groups["clang-format"] = list(manifest["cpp"])
    else:
        groups["clang-format"] = [str(ROOT / name) for name in groups["clang-format"]]
    for family, names in groups.items():
        for index, name in enumerate(names):
            relative = Path(name) if family == "clang-format" else ROOT / name
            assert not relative.is_absolute() and ".." not in relative.parts
            before = source / relative
            output = formatted / relative
            output.parent.mkdir(parents=True, exist_ok=True)
            row = dict(path=str(relative), formatter=family, before_sha256=sha(before))
            tool = TOOLS[family]
            if family == "clang-format":
                arguments = [tool, "--style=" + STYLE]
            elif family == "gofmt":
                arguments = [tool]
            else:
                shutil.copy2(before, output)
                arguments = (
                    [tool, "--edition", "2021", "--config", "skip_children=true"]
                    if family == "rustfmt"
                    else (
                        [tool, "fmt"]
                        if family == "zig"
                        else [tool, "--indent:2", "--maxLineLen:88"]
                    )
                )
            first_input = (
                output if family in ("rustfmt", "nimpretty", "zig") else before
            )
            first = run(family + "-" + str(index) + "-first", [*arguments, first_input])
            if first.returncode == 0:
                if family in ("clang-format", "gofmt"):
                    output.write_bytes(first.stdout)
                first_bytes = output.read_bytes()
                second = run(
                    family + "-" + str(index) + "-second", [*arguments, output]
                )
                row["idempotent"] = second.returncode == 0 and (
                    second.stdout == first_bytes
                    if family in ("clang-format", "gofmt")
                    else output.read_bytes() == first_bytes
                )
                row.update(
                    after_sha256=sha(output),
                    bytes=output.stat().st_size,
                    status="PASS" if row["idempotent"] else "FAILED_IDEMPOTENCE",
                )
            else:
                row.update(status="FAILED_RETAINED", exit=first.returncode)
            rows.append(row)

    python_names = (
        manifest["python"] if manifest else [str(ROOT / "python/patrol_client.py")]
    )
    if manifest and len(sys.argv) > 3:
        parent_report_path = Path(sys.argv[3]).resolve(strict=True)
        parent_report = json.loads(parent_report_path.read_bytes())
        python_tools = parent_report_path.parent / "formatter-python"
        black_python = Path("/usr/bin/python3.13")
        parent_version = parent_report["formatters"]["black"]
        assert parent_version["python"] == str(black_python)
        assert parent_version["python_sha256"] == sha(black_python)
        assert source_pins(python_tools) == parent_version["installed_file_pins"]
        assert {
            row["name"]: row["version"] for row in parent_report["formatter_packages"]
        } == FORMAT_PACKAGES
        packages.extend(parent_report["formatter_packages"])
        report["formatter_parent_receipt"] = dict(
            path=str(parent_report_path), sha256=sha(parent_report_path)
        )
    elif manifest:
        wheels = work / "formatter-wheels"
        python_tools = work / "formatter-python"
        wheels.mkdir()
        python_tools.mkdir()
        downloaded, expanded = 0, 0
        for name, version in FORMAT_PACKAGES.items():
            url = "https://pypi.org/pypi/" + name + "/" + version + "/json"
            with urllib.request.urlopen(url, timeout=20) as response:
                metadata_body = response.read((1 << 20) + 1)
            assert len(metadata_body) <= 1 << 20
            metadata = json.loads(metadata_body)
            matches = [
                row
                for row in metadata["urls"]
                if row["filename"].endswith("-py3-none-any.whl")
            ]
            assert len(matches) == 1
            selected = matches[0]
            assert (
                urllib.parse.urlparse(selected["url"]).hostname
                == "files.pythonhosted.org"
            )
            with urllib.request.urlopen(selected["url"], timeout=20) as response:
                body = response.read((4 << 20) + 1)
            downloaded += len(body)
            assert len(body) == selected["size"] and downloaded <= 12 << 20
            wheel = wheels / selected["filename"]
            wheel.write_bytes(body)
            assert sha(wheel) == selected["digests"]["sha256"]
            (wheels / (name + ".json")).write_bytes(metadata_body)
            with zipfile.ZipFile(wheel) as archive:
                for member in archive.infolist():
                    relative = Path(member.filename)
                    assert not relative.is_absolute() and ".." not in relative.parts
                    assert (member.external_attr >> 16) & 0o170000 != 0o120000
                    expanded += member.file_size
                    assert expanded <= 16 << 20
                    destination = python_tools / relative
                    if member.is_dir():
                        destination.mkdir(parents=True, exist_ok=True)
                    else:
                        destination.parent.mkdir(parents=True, exist_ok=True)
                        assert not destination.exists()
                        destination.write_bytes(archive.read(member))
            packages.append(
                dict(
                    name=name,
                    version=version,
                    filename=selected["filename"],
                    bytes=len(body),
                    sha256=sha(wheel),
                    url=selected["url"],
                )
            )
        # The actual pure-Python release runs with its supported Python3.13;
        # wheel extraction installs no scripts and imports nothing on this path.
        black_python = Path("/usr/bin/python3.13")
        assert black_python.is_file()
    if manifest:
        black = [
            black_python,
            "-I",
            "-B",
            "-c",
            "import runpy,sys; sys.path.insert(0,sys.argv.pop(1)); "
            "sys.argv[0]='black'; runpy.run_module('black',run_name='__main__')",
            python_tools,
        ]
        version = run("black-version", [*black, "--version"])
        assert version.returncode == 0
        versions["black"] = dict(
            python=str(black_python),
            python_sha256=sha(black_python),
            version=(version.stdout + version.stderr).decode(errors="replace"),
            installed_file_pins=source_pins(python_tools),
        )
    for index, name in enumerate(python_names):
        relative = Path(name)
        assert not relative.is_absolute() and ".." not in relative.parts
        python = source / relative
        ast.parse(python.read_bytes(), filename=str(python))
        row = dict(
            path=str(relative),
            layer="Python_AST_syntax",
            before_sha256=sha(python),
            status="PASS",
            imported=False,
        )
        if manifest:
            output = formatted / relative
            output.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(python, output)
            arguments = [
                *black,
                "--target-version",
                "py310",
                "--line-length",
                "88",
                "--stdin-filename",
                relative,
                "-",
            ]
            first = run(
                "black-" + str(index) + "-first", arguments, python.read_bytes()
            )
            first_bytes = first.stdout
            second = run("black-" + str(index) + "-second", arguments, first_bytes)
            output.write_bytes(first_bytes)
            row.update(
                formatter="black",
                after_sha256=sha(output),
                bytes=output.stat().st_size,
                idempotent=first.returncode == second.returncode == 0
                and second.stdout == first_bytes,
            )
            if not row["idempotent"]:
                row["status"] = "FAILED_FORMAT_RETAINED"
            ast.parse(output.read_bytes(), filename=str(output))
        rows.append(row)
    if only_python:
        report.update(
            status=(
                "PASS_SCOPED_SOURCE_CHECKS"
                if all(r["status"] == "PASS" for r in rows)
                else "FAILED_RETAINED"
            )
        )
        save()
        print(
            json.dumps(
                dict(
                    status=report["status"],
                    checks=len(rows),
                    failed=[r for r in rows if r["status"] != "PASS"],
                )
            ),
            flush=True,
        )
        if report["status"] != "PASS_SCOPED_SOURCE_CHECKS":
            raise SystemExit(1)
        return
    nim_entry = formatted / ROOT / "nim/Patrol.nim"
    check = run(
        "nim-frontend",
        [
            TOOLS["nim"],
            "check",
            "--hints:off",
            "--warnings:off",
            "--mm:arc",
            "--threads:off",
            "--exceptions:goto",
            "--cpu:wasm32",
            "--os:any",
            "-d:wasm",
            "-d:useMalloc",
            "-d:noSignalHandler",
            "--nimcache:" + str(work / "nim-cache"),
            "--path:" + str(source / "sdk/origin/nim/src"),
            nim_entry,
        ],
    )
    rows.append(
        dict(
            path=str(ROOT / "nim/Patrol.nim"),
            layer="genuine_Nim_target_frontend",
            status="PASS" if check.returncode == 0 else "FAILED_RETAINED",
            exit=check.returncode,
            input_sha256=sha(nim_entry),
        )
    )
    check = run(
        "odin-frontend",
        [
            TOOLS["odin"],
            "check",
            source / ROOT / "odin/main.odin",
            "-file",
            "-target:wasi_wasm32",
            "-thread-count:1",
            "-collection:roswasm=" + str(source / "sdk/origin/odin"),
        ],
    )
    rows.append(
        dict(
            path=str(ROOT / "odin/main.odin"),
            layer="genuine_Odin_target_frontend",
            status="PASS" if check.returncode == 0 else "FAILED_RETAINED",
            exit=check.returncode,
            input_sha256=sha(source / ROOT / "odin/main.odin"),
        )
    )
    after_pins = source_pins(source)
    report.update(
        source_input_pins_after=after_pins,
        source_inputs_unchanged=before_pins == after_pins,
        status=(
            "PASS_SCOPED_SOURCE_CHECKS"
            if all(r["status"] == "PASS" for r in rows) and before_pins == after_pins
            else "FAILED_RETAINED"
        ),
    )
    save()
    print(
        json.dumps(
            dict(
                status=report["status"],
                checks=len(rows),
                failed=[r for r in rows if r["status"] != "PASS"],
            )
        ),
        flush=True,
    )
    if report["status"] != "PASS_SCOPED_SOURCE_CHECKS":
        raise SystemExit(1)


if __name__ == "__main__":
    main()
