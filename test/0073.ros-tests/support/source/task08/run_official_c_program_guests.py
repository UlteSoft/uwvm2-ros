"""Link/validate/run the two sealed complete C programs with genuine WASI tools.

This small4MiB job reuses exact read-only frontend closures. It intentionally
earns zero installed-xmake, positive ROS, native or official-parity credit.
Requires the original owner's finite32768MiB address-space envelope. The old
8GiB receipts remain failed; no Runtime code or memory protection is changed.
"""

import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
WASI = BASE / "10/staged-wasi-r1/staged/wasi-sdk"
RUNTIME = BASE / "10/runtime-denial-r2/work/uwvm-dual"


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
    manifest_path = Path(sys.argv[1])
    manifest = json.loads(manifest_path.read_bytes())
    assert len(sys.argv) in (2, 3)
    resume_robotis = len(sys.argv) == 3 and sys.argv[2] in (
        "--resume-robotis-Guests", "--resume-robotis-Guests-runtime04"
    )
    franka_only = len(sys.argv) == 3 and sys.argv[2] == "--franka-only"
    assert len(sys.argv) == 2 or resume_robotis or franka_only
    runtime04 = len(sys.argv) == 3 and sys.argv[2] == "--resume-robotis-Guests-runtime04"
    runtime = BASE / "04/lua-guest-ros2-r2/input/runtime/uwvm2-ros" if runtime04 else RUNTIME
    runtime_sha = ("719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19"
        if runtime04 else "0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556")
    helper = Path(__file__).with_name("run_installed_official_c_programs.py")
    helper_sha = sha(helper)
    specification = importlib.util.spec_from_file_location("task08_official_C_vectors", helper)
    vectors = importlib.util.module_from_spec(specification)
    specification.loader.exec_module(vectors)
    guest_limits = vectors.require_guest_address_space()
    assert set(manifest["programs"]) == {"robotis", "franka"}
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    rows, artifacts, sources = [], [], []
    environment = dict(os.environ)
    checker = Path(__file__).parent / "tools/check_wasm_imports.py"
    abi = Path(__file__).parent / "share/abi/abi-v1.json"
    tools = (WASI / "bin/clang", WASI / "bin/wasm-ld", Path("/usr/bin/wasm-validate"), Path("/usr/bin/wasm-objdump"), runtime, checker, abi)
    tool_pins = [dict(path=str(path), bytes=path.stat().st_size, sha256=sha(path)) for path in tools]
    assert sha(runtime) == runtime_sha
    report = dict(schema="uwvm-task08-two-official-C-direct-Guest-v1", status="STARTED",
        manifest_sha256=sha(manifest_path), vector_helper_sha256=helper_sha, source_closures=sources,
        tools=tool_pins, commands=rows, artifacts=artifacts, final_links=0, wasm_validations=0,
        Guest_cases=0, installed_xmake_builds=0, positive_ROS_Guest_runs=0, Gateway_runs=0,
        native_ROS_runs=0, physical_robot_runs=0, official_parity_accepted=False,
        whole_sdk_assembly=False, selected_runtime=dict(path=str(runtime),sha256=runtime_sha),
        Guest_address_envelope=guest_limits,
        reused_linked_validated_artifacts=[], default_authority=[])

    def save():
        (work / "official-C-Guests.json").write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")

    def run(name, argv, body=None, expected_exit=0, expected_output=None):
        argv = list(map(str, argv))
        kwargs = dict(stdin=subprocess.DEVNULL) if body is None else dict(input=body)
        value = subprocess.run(argv, env=environment, cwd=work, capture_output=True, timeout=45, **kwargs)
        assert len(value.stdout) + len(value.stderr) <= 512 << 10
        for kind in ("stdout", "stderr"):
            (work / (name + "." + kind)).write_bytes(getattr(value, kind))
        rows.append(dict(name=name, argv=argv, exit=value.returncode, expected_exit=expected_exit,
            stdin_sha256=hashlib.sha256(body).hexdigest() if body is not None else None,
            stdout_sha256=sha(work / (name + ".stdout")), stderr_sha256=sha(work / (name + ".stderr"))))
        save()
        assert value.returncode == expected_exit, name
        if expected_output is not None:
            assert value.stdout == expected_output, name + " complete output"
        return value.stdout

    def guest(name, artifact, arguments, **kwargs):
        run(name, [runtime, "-Rint", "--wasip1-global-noinherit-system-environment", "--run", artifact, *arguments], **kwargs)
        report["Guest_cases"] += 1

    try:
        if not resume_robotis:
            run("actual-WASI33-version", [tools[0], "--version"])
        for platform in (("robotis",) if resume_robotis else (("franka",) if franka_only else ("robotis", "franka"))):
            selected = manifest["programs"][platform]
            qualification = Path(selected["frontend_report"]).resolve(strict=True)
            assert qualification.is_relative_to(BASE / "08") and sha(qualification) == selected["frontend_report_sha256"]
            q = json.loads(qualification.read_bytes())
            assert q["status"] == "PASS_SCOPED_GENUINE_FORMAT_AND_FRONTENDS_NO_LINK_OR_EXECUTION"
            assert q["source_inputs_unchanged"] and q["installed_C_foundation_unchanged"] and q["tools_unchanged"]
            root = qualification.parent
            source = root / "source"
            before = pins(source)
            assert before == q["input_pins_before"]
            formatted = root / "formatted"
            unit_name = selected["unit"]
            unit = formatted / unit_name if (formatted / unit_name).is_file() else source / unit_name
            for relative, expected in selected["current_source_pins"].items():
                path = formatted / relative if (formatted / relative).is_file() else source / relative
                assert sha(path) == expected
            format_before = pins(formatted)
            foundation = Path(q["C_foundation_root"]).resolve(strict=True)
            foundation_before = pins(foundation)
            assert foundation_before == q["installed_C_foundation_pins_before"]
            sources.append(dict(platform=platform, frontend_report=str(qualification), frontend_report_sha256=sha(qualification),
                source_root=str(source), source_pins_before=before, formatted_root=str(formatted), formatted_pins_before=format_before,
                foundation_root=str(foundation), foundation_pins_before=foundation_before,
                current_source_pins=selected["current_source_pins"]))
            if resume_robotis:
                previous = BASE / "08/20261022-official-c-guests-r1/work/official-C-Guests.json"
                sealed = json.loads(previous.read_bytes())
                assert sealed["status"] == "FAILED_RETAINED" and sealed["manifest_sha256"] == sha(manifest_path)
                assert sealed["all_selected_source_inputs_unchanged"] and sealed["tools_unchanged"]
                assert sealed["final_links"] == sealed["wasm_validations"] == 1
                assert len(sealed["artifacts"]) == 1 and sealed["artifacts"][0]["platform"] == "robotis"
                item = sealed["artifacts"][0]
                artifact = Path(item["path"])
                assert artifact.stat().st_size == item["bytes"] and sha(artifact) == item["sha256"]
                assert any(row["name"] == "robotis-real-wasm-validator" and row["exit"] == 0 for row in sealed["commands"])
                artifacts.append(item)
                report["reused_linked_validated_artifacts"].append(dict(parent_report=str(previous),
                    parent_report_sha256=sha(previous), artifact=item,
                    scope="Prior real link/validator passed; prior runner failed its subsequent wrong textual module-name assertion before Guest execution"))
                run(platform + "-canonical-ABI-import-signatures", [sys.executable, "-I", "-S", "-B", checker, artifact,
                    "--backend", "2", "--allow-module", "wasi_snapshot_preview1", "--report", work / (platform + "-import-contract.json")])
                vectors.exercise_program(platform, artifact, guest)
                continue
            artifact = work / (platform + "-complete-program.wasm")
            local_includes = [formatted / "sdk/include", source / "sdk/include", foundation,
                (formatted / unit_name).parent, (source / unit_name).parent,
                source / selected["binding_directory"]]
            run(platform + "-genuine-WASI33-C11-final-link", [tools[0], "--target=wasm32-wasip1",
                "--sysroot=" + str(WASI / "share/wasi-sysroot"), "-std=c11", "-O1", "-UNDEBUG",
                "-D" + selected["definition"], *["-I" + str(path) for path in local_includes], unit,
                "-lm", "-Wl,--threads=1", "-Wl,-z,stack-size=8388608", "-Wl,--max-memory=67108864", "-o", artifact])
            artifacts.append(dict(platform=platform, path=str(artifact), bytes=artifact.stat().st_size, sha256=sha(artifact)))
            report["final_links"] += 1
            run(platform + "-real-wasm-validator", [tools[2], artifact])
            report["wasm_validations"] += 1
            imports = run(platform + "-actual-imports", [tools[3], "-x", artifact])
            assert b'uwvm_ros1_v1.' not in imports
            run(platform + "-canonical-ABI-import-signatures", [sys.executable, "-I", "-S", "-B", checker, artifact,
                "--backend", "2", "--allow-module", "wasi_snapshot_preview1", "--report", work / (platform + "-import-contract.json")])
            vectors.exercise_program(platform, artifact, guest)
        report["status"] = ("PASS_PREVIOUSLY_LINKED_VALIDATED_ROBOTIS_PROGRAM_SCOPED_GUESTS_NO_NEW_LINK_OR_INSTALL"
            if resume_robotis else ("PASS_FRANKA_COMPLETE_C_PROGRAM_LINK_VALIDATION_AND_SCOPED_GUESTS_NOT_INSTALLED_OR_POSITIVE_ROS"
            if franka_only else "PASS_TWO_COMPLETE_C_PROGRAM_LINKS_VALIDATIONS_AND_SCOPED_GUESTS_NOT_INSTALLED_OR_POSITIVE_ROS"))
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        unchanged = True
        for source in sources:
            for kind in ("source", "formatted", "foundation"):
                source[kind + "_unchanged"] = pins(Path(source[kind + "_root"])) == source[kind + "_pins_before"]
                unchanged = unchanged and source[kind + "_unchanged"]
        report.update(all_selected_source_inputs_unchanged=unchanged, vector_helper_unchanged=sha(helper) == helper_sha,
            tools_unchanged=all(sha(Path(row["path"])) == row["sha256"] for row in tool_pins))
        if not unchanged or not report["vector_helper_unchanged"] or not report["tools_unchanged"]:
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        save()
        if report["status"] == "FAILED_INPUT_IMMUTABILITY_RETAINED":
            raise RuntimeError("selected program/foundation/tool inputs changed")


if __name__ == "__main__":
    main()
