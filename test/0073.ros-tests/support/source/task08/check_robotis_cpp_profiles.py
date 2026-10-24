"""Genuine format/eight-standard WASI frontend for the portable Robotis C++ view.

This4MiB job copies pinned genuine current C foundation source and invokes the
unaltered shared public C record transform before compiling the actual complete
telemetry command without C++ STL. It never links/runs a Guest or ROS peer.
One cpp language has eight source-standard profiles, never eight languages.
"""

import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import zipfile


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
WASI = BASE / "10/staged-wasi-r1/staged/wasi-sdk"
FORMAT = Path("/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang-format")
PARENT_SDK = BASE / "09/installed-c-current-r36/work/relocated-sdk"
FAMILY = "include/platform/robotis/c/ros2/robotis-turtlebot3-humble-complete"
STANDARDS = ((98, "c++98"), (3, "c++03"), (11, "c++11"), (14, "c++14"),
             (17, "c++17"), (20, "c++20"), (23, "c++23"), (26, "c++2c"))


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def pins(root):
    rows = []
    for path in sorted(root.rglob("*")):
        assert not path.is_symlink()
        if path.is_file():
            rows.append(dict(path=str(path.relative_to(root)), bytes=path.stat().st_size, sha256=sha(path)))
    return rows


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original Linux finite common owner required")
    packet = Path(sys.argv[1])
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    source, formatted = work / "source", work / "formatted"
    source.mkdir()
    formatted.mkdir()
    with zipfile.ZipFile(packet) as archive:
        members = archive.infolist()
        assert len(members) <= 20 and sum(row.file_size for row in members) < 512 << 10
        assert len({row.filename for row in members}) == len(members)
        for row in members:
            relative = Path(row.filename)
            assert not relative.is_absolute() and ".." not in relative.parts and not row.is_dir()
            target = source / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(archive.read(row))
    manifest = json.loads((source / "manifest.json").read_bytes())
    before = pins(source)
    parent = json.loads((source / "parent-installed-c-r36.json").read_bytes())
    assert parent["status"] == "PASS" and parent["installed_sdk"] == str(PARENT_SDK)
    parent_before = pins(PARENT_SDK)
    actual = {row["path"]: row for row in parent_before}
    for row in parent["source_pins"]:
        assert actual[row["path"]] == row
    sdk = work / "C-foundation"
    sdk.mkdir()
    for row in parent["source_pins"]:
        if "/nvidia-isaac-ros/" in row["path"]:
            continue
        target = sdk / row["path"]
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(PARENT_SDK / row["path"], target)
        assert sha(target) == row["sha256"]
    module_path = source / "shared/c_profiles.py"
    specification = importlib.util.spec_from_file_location("task08_actual_public_C_transform", module_path)
    public_C = importlib.util.module_from_spec(specification)
    specification.loader.exec_module(public_C)
    products = []
    for name in ("bounded_records.h", "bounded_interfaces.h"):
        original = source / "generated" / name
        relative = FAMILY + "/" + name
        body, transformation = public_C.transform(original, relative, original.read_bytes())
        assert transformation is not None and transformation["fields_removed"] == 0
        target = sdk / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(body)
        products.append(dict(path=relative, bytes=len(body), sha256=sha(target),
                             shared_transform=transformation))
    sdk_before = pins(sdk)
    compiler = WASI / "bin/clang++"
    environment = dict(os.environ, LD_LIBRARY_PATH=(
        "/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu"
        ":/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu"))
    runner_sha = sha(Path(__file__))
    tools = [dict(path=str(path), sha256=sha(path)) for path in (compiler, FORMAT)]
    commands, outputs, profiles = [], [], []
    report = dict(schema="uwvm-task08-robotis-eight-CPP-source-profiles-v1", status="STARTED",
        input_packet_sha256=sha(packet), runner_sha256=runner_sha, source_pins_before=before, C_foundation=str(sdk),
        C_foundation_pins_before=sdk_before, tools=tools, commands=commands, format_outputs=outputs,
        source_standard_profiles=profiles, normalized_languages=["cpp"],
        C_bindings_scope="ACTUAL_UNALTERED_SHARED_PUBLIC_C_TRANSFORM_OF_PINNED_WHOLE_RECORDS",
        parent_C_source=str(PARENT_SDK), parent_C_source_pins_before=parent_before,
        shared_C_exporter_sha256=sha(module_path), actual_C_export_products=products,
        whole_public_distribution_assembled=False,
        Cpp_route="GENUINE_RAW_FREESTANDING_NO_CPP_STANDARD_LIBRARY",
        installed_xmake_builds=0, final_links=0, Guest_runs=0, native_ROS_runs=0,
        official_parity_accepted=False, default_authority=[])

    def save():
        (work / "robotis-CPP-profiles.json").write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")

    def run(name, argv, body=None):
        argv = list(map(str, argv))
        kwargs = dict(stdin=subprocess.DEVNULL) if body is None else dict(input=body)
        result = subprocess.run(argv, cwd=work, env=environment, capture_output=True, timeout=30, **kwargs)
        assert len(result.stdout) + len(result.stderr) < 64 << 10
        for kind in ("stdout", "stderr"):
            (work / (name + "." + kind)).write_bytes(getattr(result, kind))
        commands.append(dict(name=name, argv=argv, exit=result.returncode,
            stdin_sha256=hashlib.sha256(body).hexdigest() if body is not None else None,
            stdout_sha256=sha(work / (name + ".stdout")), stderr_sha256=sha(work / (name + ".stderr"))))
        save()
        return result

    try:
        for index, tool in enumerate((compiler, FORMAT)):
            assert run("tool-version-" + str(index), [tool, "--version"]).returncode == 0
        for index, name in enumerate(manifest["format"]):
            target = formatted / name
            target.parent.mkdir(parents=True, exist_ok=True)
            original = source / name
            argv = [FORMAT, "--style={BasedOnStyle: LLVM, Standard: Cpp03, IndentWidth: 4, ColumnLimit: 88, SortIncludes: Never, ReflowComments: false}", "--assume-filename=" + name]
            first = run("format-" + str(index) + "-first", argv, original.read_bytes())
            assert first.returncode == 0
            second = run("format-" + str(index) + "-second", argv, first.stdout)
            assert second.returncode == 0 and second.stdout == first.stdout
            target.write_bytes(first.stdout)
            outputs.append(dict(path=name, before_sha256=sha(original), after_sha256=sha(target), bytes=target.stat().st_size, idempotent=True))
        entry = formatted / manifest["entry"]
        include = Path(manifest["include_directory"])
        assert not include.is_absolute() and ".." not in include.parts
        for number, standard in STANDARDS:
            result = run("WASI-CPP-profile-" + str(number), [compiler, "--target=wasm32-wasip1",
                "--sysroot=" + str(WASI / "share/wasi-sysroot"), "-std=" + standard,
                "-DROSWASM_CPP_STANDARD=" + str(number), "-DROSWASM_CPP_FREESTANDING=1",
                "-DUWVM_TURTLEBOT3_PROFILE=2", "-fno-exceptions", "-nostdinc++", "-pedantic-errors", "-ferror-limit=3", "-fsyntax-only",
                "-I" + str(formatted / include), "-I" + str(source / include),
                "-I" + str(source / "profile-include"), "-I" + str(sdk / "include"),
                "-I" + str(sdk / FAMILY), "-I" + str(source / manifest["entry"]).rsplit("/", 1)[0], entry])
            profiles.append(dict(selector=number, flag=standard, status="PASS" if result.returncode == 0 else "FAILED_RETAINED"))
        report["status"] = "PASS_EIGHT_REAL_WASI_CPP_FRONTENDS_FORMAT_ONLY" if all(row["status"] == "PASS" for row in profiles) else "FAILED_CPP_FRONTENDS_RETAINED"
        if report["status"] != "PASS_EIGHT_REAL_WASI_CPP_FRONTENDS_FORMAT_ONLY":
            raise RuntimeError("one or more genuine C++ frontend modes failed")
    except BaseException as error:
        if report["status"] == "STARTED":
            report["status"] = "FAILED_RETAINED"
        report["error"] = repr(error)
        raise
    finally:
        report.update(source_unchanged=pins(source) == before, C_foundation_unchanged=pins(sdk) == sdk_before,
            parent_C_source_unchanged=pins(PARENT_SDK) == parent_before,
            runner_unchanged=sha(Path(__file__)) == runner_sha,
            tools_unchanged=all(sha(Path(row["path"])) == row["sha256"] for row in tools))
        if not all(report[key] for key in ("source_unchanged", "C_foundation_unchanged", "parent_C_source_unchanged", "tools_unchanged", "runner_unchanged")):
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        save()


if __name__ == "__main__":
    main()
