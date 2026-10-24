"""Build the actual Lua Patrol client through a separate installed xmake SDK.

Reuses Task04's exact genuine Lua5.4.8 source/provider closure read-only. Copies
its complete45-file installed source product, overlays the explicit owned
Robotis module and current application bytes, then invokes real xmake for
Humble/Jazzy/main. Standard-EH validators and complete ROS2 import signatures are
independent gates. Guest execution is a separate phase using an actual qualified
finite address-space envelope; this build runner never selects a Runtime.
this producer deliberately performs builds and validation only.
"""

import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import zipfile


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
PARENT = BASE / "04/round06-lua-installed-records-r1/work/qualification"
PARENT_SDK = PARENT / "SDK Lua relocated with Unicode-λ"
INPUTS = PARENT / "actual-lua-inputs.json"
INPUTS_SHA256 = "c387217df43ae8abb3861f5d0104b9c66b0f93a2c5c488fe1521c65df314686c"
PREFIX = "sdk/origin/"
RULES = PREFIX + "platform/robotis/example-rules/"
PATROL = PREFIX + "robots/mobile_bases/robotis/turtlebot3/examples/patrol/lua/patrol.lua"
OVERLAY = {
    **{RULES + name: "example/platform/robotis/" + name
       for name in ("backend.lua", "profiles.lua", "patrol-module.lua", "xmake.lua", "lua/xmake.lua")},
    PATROL: "example/platform/robotis/lua/patrol/lua/patrol.lua",
}


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def pin(path):
    return dict(path=str(path.resolve(strict=True)), bytes=path.stat().st_size, sha256=sha(path))


def pins(root):
    result = []
    for path in sorted(root.rglob("*")):
        assert not path.is_symlink(), "ordinary sealed SDK/source files required"
        if path.is_file():
            result.append(dict(path=str(path.relative_to(root)), bytes=path.stat().st_size, sha256=sha(path)))
    return result


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original Linux finite common owner required")
    packet = Path(sys.argv[1])
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    work.mkdir(exist_ok=True)
    with zipfile.ZipFile(packet) as archive:
        rows = archive.infolist()
        assert len(rows) <= 16 and len({row.filename for row in rows}) == len(rows)
        assert sum(row.file_size for row in rows) < 1 << 20
        bodies = {}
        for row in rows:
            relative = Path(row.filename)
            assert not relative.is_absolute() and ".." not in relative.parts and not row.is_dir()
            assert (row.external_attr >> 16) & 0o170000 != 0o120000
            bodies[row.filename] = archive.read(row)
    assert set(bodies) == set(OVERLAY) | {"parent-installed-lua.json"}
    parent = json.loads(bodies["parent-installed-lua.json"])
    assert parent["status"] == "ACTUAL_INSTALLED_LUA_XMAKE_VALIDATE_GUEST_PASS"
    assert parent["sdk"] == str(PARENT_SDK)
    manifest = PARENT_SDK / "share/wasm-profiles.json"
    assert pin(manifest) == parent["manifest"]
    document = json.loads(manifest.read_bytes())
    assert len(document["profiles"]) == 1
    profile = document["profiles"][0]
    assert profile["id"] == "task04-lua548-standard-eh-source-provider"
    expected = {row["path"]: {key: row[key] for key in ("path", "bytes", "sha256")}
                for row in profile["files"]}
    expected["share/wasm-profiles.json"] = dict(path="share/wasm-profiles.json",
                                               bytes=manifest.stat().st_size, sha256=sha(manifest))
    actual = {row["path"]: row for row in pins(PARENT_SDK)}
    assert len(actual) == len(expected) == 45 and actual == expected
    assert sha(INPUTS) == INPUTS_SHA256
    sdk = work / "relocated-sdk"
    shutil.copytree(PARENT_SDK, sdk)
    assert pins(sdk) == list(actual.values())
    assert not any((sdk / name).exists() for name in ("origin", "test", "tests", "sdk"))
    changes = []
    for source, destination in OVERLAY.items():
        path = sdk / destination
        before = sha(path) if path.exists() else None
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(bodies[source])
        changes.append(dict(source=source, path=destination, before_sha256=before,
                            after_sha256=sha(path), bytes=path.stat().st_size))
    # This is an explicit private platform extension, not a rewrite of any
    # foundation/provider bytes or a relabeling of the parent's Guest receipt.
    assert all(row["before_sha256"] is None for row in changes)
    assert "--platform" in (sdk / "tools/xmake/examples/managed_backend.py").read_text()
    before = pins(sdk)
    inputs = json.loads(INPUTS.read_bytes())
    validator = Path(inputs["paths"]["validator"])
    xmake = Path("/home/macromodel/.local/bin/xmake")
    tools = [pin(path) for path in (xmake, validator, Path(sys.executable), INPUTS, Path(__file__))]
    report = dict(schema="task08-current-installed-Lua-patrol-v1", status="STARTED",
                  parent_sdk=str(PARENT_SDK), parent_sdk_before=list(actual.values()),
                  parent_receipt_sha256=hashlib.sha256(bodies["parent-installed-lua.json"]).hexdigest(),
                  input_packet=pin(packet), sdk_root=str(sdk), SDK_pins_before=before,
                  private_platform_extension=changes, tools=tools, commands=[], artifacts=[],
                  actual_xmake_builds=0, validated_artifacts=0, import_contracts=0,
                  Guest_runs=0, native_ROS_runs=0, official_parity_accepted=False,
                  Guest_execution="NOT_REQUESTED_BUILD_VALIDATION_PHASE_ONLY", default_authority=[])
    environment = dict(os.environ, XMAKE_STATS="false", XMAKE_SKIP_HISTORY="y",
                       XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake",
                       OMP_NUM_THREADS="1", OPENBLAS_NUM_THREADS="1")
    logs = work / "logs"
    logs.mkdir()

    def save():
        (work / "installed-lua-patrol.json").write_text(json.dumps(report, indent=2) + "\n")

    def run(label, argv, cwd, timeout=150):
        argv = list(map(str, argv))
        folder = logs / label
        folder.mkdir()
        with (folder / "stdout").open("xb") as stdout, (folder / "stderr").open("xb") as stderr:
            result = subprocess.run(argv, cwd=cwd, env=environment, stdin=subprocess.DEVNULL,
                                    stdout=stdout, stderr=stderr, timeout=timeout, check=False)
        report["commands"].append(dict(label=label, argv=argv, cwd=str(cwd), exit=result.returncode,
                                       stdout=pin(folder / "stdout"), stderr=pin(folder / "stderr")))
        save()
        assert result.returncode == 0, "actual installed Lua step failed: " + label

    save()
    try:
        run("xmake-version", [xmake, "--version"], work)
        run("validator-version", [validator, "--version"], work)
        for name in ("humble", "jazzy", "main"):
            project = work / (name + "-project")
            project.mkdir()
            entry = "patrol-client." + name

            def command(task):
                return [xmake, task, "-P", project, "-F", sdk / "example/xmake.lua"]

            run(name + "-configure", [*command("f"), "-c", "--yes", "--ros-sdk-platforms=robotis",
                "--ros-sdk-languages=lua", "--sdk-example-platform=robotis", "--sdk-example-language=lua",
                "--sdk-example=" + entry, "--sdk-target=wasm32-wasip1", "--sdk-startup=command",
                "--sdk-exceptions=y", "--sdk-threads=n", "--sdk-offline=y",
                "--sdk-python=" + str(Path(sys.executable).resolve()), "--sdk-toolchain=/usr",
                "--sdk-profile-manifest=" + str(sdk / "share/wasm-profiles.json"),
                "--ros-sdk-lua-runtime=lua54-wasi-eh", "--ros-sdk-managed-backend=2",
                "--ros-sdk-managed-inputs=" + str(INPUTS),
                "--ros-sdk-managed-inputs-sha256=" + INPUTS_SHA256], project)
            run(name + "-list", command("sdk-example-list"), project)
            product = work / (name + "-product")
            run(name + "-build", [*command("sdk-example-build"), "--output=" + str(product)], project)
            receipt = product / "build-receipt.json"
            built = json.loads(receipt.read_bytes())
            assert built["status"] == "BUILT_NOT_EXECUTED" and len(built["artifacts"]) == 1
            artifact = Path(built["artifacts"][0]["path"])
            selected = built["artifacts"][0]
            assert artifact.is_relative_to(product) and selected["kind"] == "wasm"
            assert selected["bytes"] == artifact.stat().st_size and selected["sha256"] == sha(artifact)
            report["actual_xmake_builds"] += 1
            run(name + "-validate", [validator, "validate", artifact], project)
            report["validated_artifacts"] += 1
            imports = work / (name + "-imports.json")
            run(name + "-import-signatures", [sys.executable, "-I", "-S", "-B",
                sdk / "tools/check_wasm_imports.py", artifact, "--backend", "2",
                "--allow-module", "wasi_snapshot_preview1", "--require-all", "--report", imports], project)
            report["import_contracts"] += 1
            report["artifacts"].append(dict(profile=name, entry=entry, artifact=pin(artifact),
                                              build_receipt=pin(receipt), import_contract=pin(imports)))
            save()
        report["status"] = "PASS_THREE_CURRENT_INSTALLED_LUA_XMAKE_BUILDS_VALIDATORS_IMPORTS_ONLY"
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        report.update(SDK_unchanged=pins(sdk) == before,
                      parent_SDK_unchanged=pins(PARENT_SDK) == list(actual.values()),
                      tools_unchanged=all(pin(Path(row["path"])) == row for row in tools))
        if not all(report[key] for key in ("SDK_unchanged", "parent_SDK_unchanged", "tools_unchanged")):
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        save()


if __name__ == "__main__":
    main()
