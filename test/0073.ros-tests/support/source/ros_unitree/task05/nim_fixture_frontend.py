#!/usr/bin/env python3
"""Format and type-check the new Nim value fixture, without code generation."""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess
import tarfile


def pin(path):
    body = path.read_bytes()
    return {"bytes": len(body), "sha256": hashlib.sha256(body).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("fixture", "unitree", "records", "foundation", "nim-root"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    work = Path.cwd()
    sources = {"nim_sport_values.nim": args.fixture,
               "unitree_sport.nim": args.unitree / "language/nim/unitree_sport.nim",
               "unitree_api_records.nim": args.records}
    captured = {str(path): pin(path) for path in sources.values()}
    nim, pretty = args.nim_root / "bin/nim", args.nim_root / "bin/nimpretty"
    captured.update({str(path): pin(path) for path in (nim, pretty)})
    for path in sorted((args.foundation / "src").rglob("*.nim")):
        captured[str(path)] = pin(path)
    source = work / "source"
    source.mkdir()
    for name, origin in sources.items():
        shutil.copyfile(origin, source / name)
    fixture = source / "nim_sport_values.nim"
    commands = []

    def run(name, argv):
        with (work / (name + ".stdout")).open("xb") as out, (work / (name + ".stderr")).open("xb") as err:
            result = subprocess.run([str(value) for value in argv], stdout=out, stderr=err, check=False)
        commands.append({"name": name, "argv": [str(value) for value in argv], "exit": result.returncode})
        if result.returncode:
            raise ValueError(name + " failed; actual Nim diagnostics retained")

    run("pretty-first", [pretty, "--indent:2", "--maxLineLen:100", fixture])
    formatted = pin(fixture)
    run("pretty-second", [pretty, "--indent:2", "--maxLineLen:100", fixture])
    if pin(fixture) != formatted:
        raise ValueError("actual Nim formatting is not idempotent")
    run("check-fixture", [nim, "check", "--mm:arc", "--threads:off", "--exceptions:goto",
        "--cpu:wasm32", "--os:any", "-d:wasm", "-d:useMalloc",
        "--path:" + str(args.foundation / "src"), "--path:" + str(source),
        "--nimcache:" + str(work / "nimcache"), fixture])
    for path, before in captured.items():
        if pin(Path(path)) != before:
            raise ValueError("original fixture/foundation/provider input changed")
    with tarfile.open(work / "source-candidate.tar", "w") as archive:
        archive.add(fixture, arcname="nim_sport_values.nim", recursive=False)
    result = {"status": "PASS_NEW_NIM_FIXTURE_FORMAT_AND_FRONTEND_ONLY",
              "source_and_tool_inputs": captured, "commands": commands, "formatted": formatted,
              "C_compiler_runs": 0, "Guest_runs": 0, "native_ROS_runs": 0, "default_authority": []}
    (work / "result.json").write_text(json.dumps(result, indent=2)+"\n")
    print(result["status"])


if __name__ == "__main__":
    main()
