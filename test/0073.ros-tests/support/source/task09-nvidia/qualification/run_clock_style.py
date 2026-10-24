#!/usr/bin/env python3
"""Real native-AS AST/comment and StyLua AST formatting of fourteen owned files."""
import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import subprocess
import sys
import tarfile


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("source", "as-driver", "lua-driver", "lua-reference"):
        parser.add_argument("--" + name, type=Path, required=True)
        parser.add_argument("--" + name + "-sha256", required=True)
    args = parser.parse_args()
    assert sys.platform == "linux" and os.environ["UWVM_RESOURCE_HEAVY_LOCK_HELD"] == "1"
    run = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    work = run / "work"
    for name in ("source", "as_driver", "lua_driver", "lua_reference"):
        path = getattr(args, name)
        assert path.is_relative_to(run / "inputs") and path.resolve(strict=True) == path
        assert digest(path) == getattr(args, name + "_sha256")
    node = Path("/usr/bin/node")
    assert digest(node) == "893391109bf5d2b34dd50143bfab997b3fcd123dd59cb156b67f01b7a1db02bf"
    base = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
    compiler = base / "04/round03-as-xmake-r1/staged/node_modules/assemblyscript/dist/assemblyscript.js"
    closure = base / "04/round03-as-installed-r6/work/qualification/actual-offline-inputs.json"
    closure_sha = "a451ae4ea990d820904331dc0ec0cd0d9f5842869d1f4a195e9f8d2ca9c6c98f"
    formatter = base / "10/staged-wasi-r1/staged/wasi-sdk/bin/clang-format"
    formatter_sha = "57228f29475519fda704013fbd3be5e8434929673d3021436ff3d1f338fd920a"
    acquisition = base / "04/round05-stylua-wasm-r1/work/formatter/acquisition.json"
    acquisition_sha = "b3f3bee783667dbaa36d8c39c9f5f0932d90ee6dcbe7f763394c77f222f67f77"
    assert digest(compiler) == "b0ea444edfff355fe2981451358c5b8b24996f5e9190e02bceae60c94adeea19"
    assert digest(closure) == closure_sha and digest(formatter) == formatter_sha
    assert digest(acquisition) == acquisition_sha
    selected = {
        "sdk/include/platform/nvidia-isaac-sim/as/clock-control/clock.ts",
        "sdk/example/platform/nvidia-isaac-sim/as/clock-program/main.ts",
        "sdk/example/platform/nvidia-isaac-sim/as/clock-program/wasi.ts",
        "sdk/tools/platform/nvidia/examples/build_backend.lua",
        "sdk/example/platform/nvidia-groot/tinygo/xmake.lua",
        "sdk/example/platform/nvidia-groot/py/xmake.lua",
        "sdk/example/platform/nvidia-isaac-ros/c/xmake.lua",
        "sdk/example/platform/nvidia-isaac-ros/cpp/xmake.lua",
        "sdk/example/platform/nvidia-isaac-sim/cpp/xmake.lua",
        "sdk/example/platform/nvidia-isaac-sim/py/xmake.lua",
        "sdk/tools/platform/nvidia-isaac-sim/clock/clock_build.lua",
        "sdk/example/platform/nvidia-isaac-sim/rs/xmake.lua",
        "sdk/example/platform/nvidia-isaac-sim/as/xmake.lua",
        "sdk/example/platform/nvidia-isaac-sim/tinygo/xmake.lua",
    }
    source = work / "source"
    source.mkdir()
    before, total = {}, 0
    with tarfile.open(args.source, "r:") as archive:
        members = archive.getmembers()
        assert len(members) == len(selected) == 14
        for item in members:
            relative = PurePosixPath(item.name)
            assert item.isfile() and item.name in selected and item.name not in before
            assert not relative.is_absolute() and ".." not in relative.parts
            assert 0 < item.size <= 65536
            total += item.size
            assert total <= 262144
            raw = archive.extractfile(item).read(item.size + 1)
            assert len(raw) == item.size
            path = source / item.name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(raw)
            before[item.name] = {"bytes": len(raw), "sha256": hashlib.sha256(raw).hexdigest()}
    report = {"format": "task09-real-as-lua-format-v1", "status": "RUNNING", "inputs": before,
              "source_archive_sha256": args.source_sha256, "commands": [],
              "application_executions": 0, "guest_executions": 0, "native_sdk_executions": 0,
              "default_authority": []}
    environment = dict(os.environ, UV_THREADPOOL_SIZE="1")
    for name in ("NODE_OPTIONS", "NODE_PATH", "NODE_COMPILE_CACHE"):
        environment.pop(name, None)

    def save():
        (work / "clock-style.json").write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")

    def command(name, arguments):
        row = {"name": name, "argv": [str(value) for value in arguments]}
        report["commands"].append(row)
        save()
        with (work / (name + ".stdout")).open("xb") as stdout, (work / (name + ".stderr")).open("xb") as stderr:
            completed = subprocess.run(row["argv"], env=environment, cwd=work,
                                       stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, timeout=75)
        row["exit_code"] = completed.returncode
        save()
        assert completed.returncode == 0, "actual formatter/AST verifier failed: " + name

    try:
        command("actual-as-ast-comments", [node, "--max-old-space-size=256", args.as_driver,
            source, compiler, closure, closure_sha, formatter, formatter_sha, work / "as-format"])
        command("actual-lua-full-ast", [node, "--max-old-space-size=256", args.lua_driver,
            source, acquisition, acquisition_sha, args.lua_reference, args.lua_reference_sha256,
            work / "lua-format"])
        as_report = json.loads((work / "as-format/results.json").read_bytes())
        lua_report = json.loads((work / "lua-format/results.json").read_bytes())
        assert as_report["status"] == "SCOPED_NATIVE_AS_AST_COMMENT_IDEMPOTENCE_PASS"
        assert lua_report["status"] == "SCOPED_LUA54_FORMAT_AST_IDEMPOTENCE_PASS"
        assert len(as_report["files"]) == 3 and len(lua_report["files"]) == 11
        products = {}
        for result in (as_report, lua_report):
            for row in result["files"]:
                name = row["relative"]
                assert name in selected and name not in products
                assert {key: row["source"][key] for key in ("bytes", "sha256")} == before[name]
                product = Path(row["product"]["path"])
                assert product.resolve(strict=True).is_relative_to(work)
                assert digest(product) == row["product"]["sha256"]
                products[name] = product
        assert set(products) == selected
        candidate = work / "formatted-clock-style.tar"
        with tarfile.open(candidate, "x:", format=tarfile.USTAR_FORMAT) as archive:
            for name, path in sorted(products.items()):
                archive.add(path, arcname=name, recursive=False)
        report.update(status="REAL_AS3_LUA11_AST_IDEMPOTENCE_PASS", outputs={name: {
            "bytes": path.stat().st_size, "sha256": digest(path)} for name, path in products.items()},
            formatted_archive={"bytes": candidate.stat().st_size, "sha256": digest(candidate)},
            actual_as_formatter_ast_comment_checks=3, actual_lua_formatter_full_ast_checks=11,
            codegen_or_link_executions=0, inherited_other_task_acceptance=False)
    except Exception as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
    for name, pin in before.items():
        assert digest(source / name) == pin["sha256"]
    for name in ("source", "as_driver", "lua_driver", "lua_reference"):
        assert digest(getattr(args, name)) == getattr(args, name + "_sha256")
    assert digest(node) == "893391109bf5d2b34dd50143bfab997b3fcd123dd59cb156b67f01b7a1db02bf"
    save()
    print(json.dumps({"status": report["status"], "guest_executions": 0, "native_sdk_executions": 0}))
    return 0 if report["status"] != "FAILED_RETAINED" else 1


if __name__ == "__main__":
    raise SystemExit(main())
