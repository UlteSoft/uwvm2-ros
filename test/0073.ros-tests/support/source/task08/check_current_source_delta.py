"""Format current owned C/C++ changes and use genuine target/API frontends.

No executable is linked or run. Current-source syntax is separate from the
earlier final-link/contract receipts, installed builds, Guests and native ROS.
"""

import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import zipfile


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
NATIVE = Path("/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm")
WASI = BASE / "10/staged-wasi-r1/staged/wasi-sdk"
ORIGINAL = BASE / "08/20261021-kortex-acquire-r1/work/kortex-2.8.0"
PARENT = BASE / "08/20261022-installed-patrol-nim-r3/work"


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def pins(root):
    result = []
    for path in sorted(root.rglob("*")):
        assert not path.is_symlink()
        if path.is_file():
            result.append(dict(path=str(path.relative_to(root)), bytes=path.stat().st_size, sha256=sha(path)))
    return result


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original Linux common finite owner required")
    packet = Path(sys.argv[1])
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    source, formatted = work / "source", work / "formatted"
    source.mkdir()
    formatted.mkdir()
    with zipfile.ZipFile(packet) as archive:
        rows = archive.infolist()
        assert len(rows) < 32 and sum(row.file_size for row in rows) < 512 << 10
        assert len({row.filename for row in rows}) == len(rows)
        for row in rows:
            relative = Path(row.filename)
            assert not relative.is_absolute() and ".." not in relative.parts and not row.is_dir()
            target = source / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(archive.read(row))
    manifest = json.loads((source / "manifest.json").read_bytes())
    before = pins(source)
    local_c_include = None
    if manifest.get("C_external_header_report"):
        reference = Path(manifest["C_external_header_report"]).resolve(strict=True)
        assert reference.is_relative_to(BASE / "08")
        sealed = json.loads(reference.read_bytes())
        assert sealed["status"] == "PASS_SCOPED_GENUINE_FORMAT_AND_FRONTENDS_NO_LINK_OR_EXECUTION"
        assert sealed["installed_C_foundation_unchanged"] and sealed["source_inputs_unchanged"]
        sdk = Path(sealed["C_foundation_root"]).resolve(strict=True)
        assert sdk.is_relative_to(BASE / "08")
        sdk_include = sdk
        sdk_before = pins(sdk)
        assert sdk_before == sealed["installed_C_foundation_pins_before"]
        relative = Path(manifest["C_include_root"])
        assert not relative.is_absolute() and ".." not in relative.parts
        local_c_include = source / relative
        foundation_kind = "REUSED_SEALED_CANONICAL_C_HEADERS_NOT_INSTALLED_BUILD"
    elif manifest.get("C_include_root"):
        relative = Path(manifest["C_include_root"])
        assert not relative.is_absolute() and ".." not in relative.parts
        sdk_include = source / relative
        sdk = sdk_include
        foundation_kind = "SEALED_CANONICAL_PUBLIC_C_HEADERS_NOT_INSTALLED_BUILD"
        sdk_before = pins(sdk)
    else:
        parent = json.loads((PARENT / "installed-patrol-nim.json").read_bytes())
        assert parent["status"] == "PASS_PRIVATE_INSTALLED_NIM_PATROL_BUILD_VALIDATION_AND_EMPTY_AUTHORITY_GUESTS"
        sdk = PARENT / "relocated-sdk"
        sdk_before = pins(sdk)
        assert sdk_before == parent["SDK_pins_before"]
        sdk_include = sdk / "include"
        foundation_kind = "SEALED_PRIVATE_INSTALLED_NIM_FOUNDATION"
    lock = json.loads((source / "original-sdk-lock.json").read_bytes()) if manifest["cpp_units"] else None

    def original_unchanged():
        if lock is None:
            return True
        return sha(ORIGINAL / "original.zip") == lock["archive_sha256"] and all(
            (ORIGINAL / "original" / row["path"]).stat().st_size == row["bytes"]
            and sha(ORIGINAL / "original" / row["path"]) == row["sha256"] for row in lock["files"])

    assert original_unchanged()
    tools = [NATIVE / "bin/clang-format", NATIVE / "bin/clang++", WASI / "bin/clang"]
    tool_pins = [dict(path=str(path), sha256=sha(path)) for path in tools]
    environment = dict(os.environ, LD_LIBRARY_PATH=(str(NATIVE / "lib/x86_64-unknown-linux-gnu")
        + ":/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu"))
    commands, outputs = [], []
    report = dict(schema="uwvm-task08-current-source-delta-frontend-v1", status="STARTED",
        input_packet_sha256=sha(packet), input_pins_before=before, format_outputs=outputs,
        tools=tool_pins, commands=commands, original_expanded_files_checked=len(lock["files"]) if lock else 0,
        original_archive_sha256=lock["archive_sha256"] if lock else None, C_foundation_root=str(sdk),
        C_foundation_kind=foundation_kind,
        installed_C_foundation_pins_before=sdk_before, actual_final_links=0, contract_runs=0,
        actual_installed_xmake_builds=0, Guest_runs=0, native_ROS_runs=0, default_authority=[])

    def save():
        (work / "source-delta.json").write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")

    def run(name, argv, body=None):
        arguments = list(map(str, argv))
        kwargs = dict(stdin=subprocess.DEVNULL) if body is None else dict(input=body)
        result = subprocess.run(arguments, env=environment, cwd=work, capture_output=True, timeout=40, **kwargs)
        assert len(result.stdout) + len(result.stderr) < 512 << 10
        (work / (name + ".stdout")).write_bytes(result.stdout)
        (work / (name + ".stderr")).write_bytes(result.stderr)
        commands.append(dict(name=name, argv=arguments, exit=result.returncode,
            stdin_sha256=hashlib.sha256(body).hexdigest() if body is not None else None,
            stdout_sha256=sha(work / (name + ".stdout")), stderr_sha256=sha(work / (name + ".stderr"))))
        save()
        assert result.returncode == 0, name
        return result.stdout

    try:
        for index, tool in enumerate(tools):
            run("tool-version-" + str(index), [tool, "--version"])
        for index, name in enumerate(manifest["format"]):
            relative = Path(name)
            assert not relative.is_absolute() and ".." not in relative.parts
            body = (source / relative).read_bytes()
            argv = [tools[0], "--style=" + manifest["style"], "--assume-filename=" + name]
            first = run("format-" + str(index) + "-first", argv, body)
            second = run("format-" + str(index) + "-second", argv, first)
            assert first == second
            target = formatted / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(first)
            outputs.append(dict(path=name, before_sha256=sha(source / relative),
                after_sha256=sha(target), bytes=len(first), idempotent=True))
        includes = [ORIGINAL / "original/include" / part for part in ("", "client", "common", "messages", "client_stubs")]
        native_header = formatted / manifest["native_header"]
        notification_header = formatted / manifest["notification_header"]
        for index, name in enumerate(manifest["cpp_units"]):
            entry = formatted / name if (formatted / name).is_file() else source / name
            run("genuine-original-SDK-cxx11-syntax-" + str(index), [tools[1], "-std=c++11", "-D_OS_UNIX",
                "-stdlib=libstdc++", "--gcc-toolchain=/usr", "-fsyntax-only", "-UNDEBUG",
                "-I" + str(native_header.parent), "-I" + str(notification_header.parent),
                *["-I" + str(path) for path in includes], entry])
        binding = source / manifest["C_binding"]
        entry = formatted / manifest["C_unit"]
        if not entry.is_file():
            entry = source / manifest["C_unit"]
        definitions = manifest.get("C_definitions", ["UWVM_TURTLEBOT3_PROFILE=2"])
        assert isinstance(definitions, list) and len(definitions) <= 4
        assert all(isinstance(value, str) and re.fullmatch(r"[A-Z][A-Z0-9_]*=[0-9]+", value) for value in definitions)
        name = manifest.get("C_check_name", "genuine-WASI33-C11-obstacle-syntax")
        assert re.fullmatch(r"[a-zA-Z0-9-]{1,80}", name)
        run(name, [tools[2], "--target=wasm32-wasip1",
            "--sysroot=" + str(WASI / "share/wasi-sysroot"), "-std=c11", "-fsyntax-only", "-UNDEBUG",
            *["-D" + value for value in definitions],
            *(["-I" + str(local_c_include)] if local_c_include else []), "-I" + str(sdk_include),
            "-I" + str((source / manifest["C_unit"]).parent), "-I" + str(binding.parent), entry])
        report.update(status="PASS_SCOPED_GENUINE_FORMAT_AND_FRONTENDS_NO_LINK_OR_EXECUTION",
            actual_Cxx11_translation_units=len(manifest["cpp_units"]), actual_WASI_C11_translation_units=1,
            C_translation_unit=manifest["C_unit"], C_definitions=definitions)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        report.update(input_pins_after=pins(source), source_inputs_unchanged=pins(source) == before,
            original_inputs_unchanged=original_unchanged(), installed_C_foundation_unchanged=pins(sdk) == sdk_before,
            tools_unchanged=all(sha(Path(row["path"])) == row["sha256"] for row in tool_pins))
        if not all(report[key] for key in ("source_inputs_unchanged", "original_inputs_unchanged",
                                          "installed_C_foundation_unchanged", "tools_unchanged")):
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        save()
        if report["status"] == "FAILED_INPUT_IMMUTABILITY_RETAINED":
            raise RuntimeError("source/provider/tool bytes changed")


if __name__ == "__main__":
    main()
