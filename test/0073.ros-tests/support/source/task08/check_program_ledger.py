"""Seal reviewed logical-program inventory and import genuine owned xmake rules.

Consumes an inert ZIP of exact source/evidence bytes. Execute only inside the
original Linux owner. Catalog imports confer zero compiler/build/Guest credit.
"""

import hashlib
import gzip
import json
import os
from pathlib import Path
import subprocess
import sys
import zipfile


ROOT = "sdk/origin/"
PATROL = ROOT + "robots/mobile_bases/robotis/turtlebot3/examples/patrol/"
ROUTES = {
    "c": ROOT + "robots/mobile_bases/robotis/turtlebot3/examples/patrol_client.c",
    "cpp": PATROL + "cpp/main.cpp",
    "rust": PATROL + "rust/main.rs",
    "go": PATROL + "go/main.go",
    "tinygo": PATROL + "go/main.go",
    "zig": PATROL + "zig/main.zig",
    "javascript": PATROL + "js/patrol_humble.mjs",
    "assemblyscript": PATROL + "as/patrol_humble.ts",
    "java": PATROL + "java/PatrolClient.java",
    "kotlin": PATROL + "kotlin/PatrolClient.kt",
    "python": PATROL + "python/patrol_client.py",
    "csharp": PATROL + "csharp/PatrolClient.cs",
    "fsharp": PATROL + "fsharp/Program.fs",
    "vbnet": PATROL + "vbnet/Program.vb",
    "moonbit": PATROL + "moonbit/app/main.mbt",
    "swift": PATROL + "swift/Patrol.swift",
    "haskell": PATROL + "haskell/Main.hs",
    "nim": PATROL + "nim/Patrol.nim",
    "d": PATROL + "d/Patrol.d",
    "odin": PATROL + "odin/main.odin",
    "lua": PATROL + "lua/patrol.lua",
}
LANGUAGE_DIR = dict(rust="rs", python="py", javascript="js", assemblyscript="as")


def digest(body):
    return hashlib.sha256(body).hexdigest()


def main():
    if (
        os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1"
        or sys.platform != "linux"
    ):
        raise RuntimeError("original Linux common finite owner required")
    packet = Path(sys.argv[1])
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    with zipfile.ZipFile(packet) as archive:
        infos = archive.infolist()
        assert len(infos) < 300 and sum(row.file_size for row in infos) < 16 << 20
        assert len({row.filename for row in infos}) == len(infos)
        bodies = {}
        for row in infos:
            path = Path(row.filename)
            assert (
                not path.is_absolute() and ".." not in path.parts and not row.is_dir()
            )
            bodies[row.filename] = archive.read(row)
    review_name = (
        ROOT
        + "coordination/20261021-director/handoffs/08/followup-03/official-program-review.json"
    )
    review = json.loads(bodies[review_name])
    languages = review["languages"]
    assert len(languages) == len(set(languages)) == 27
    audits = {
        name: json.loads(
            bodies[ROOT + "platform/" + name + "/application-entry-audit.current.json"]
        )
        for name in review["platforms"]
    }
    programs = []
    for selected in review["programs"]:
        program = dict(selected)
        originals = []
        if "source" in selected:
            body = bodies[ROOT + selected["source"]]
            originals.append(
                dict(
                    source=selected["source"],
                    repository=selected["repository"],
                    commit=selected["commit"],
                    version=selected["version"],
                    bytes=len(body),
                    sha256=digest(body),
                )
            )
        else:
            matches = selected.get("match_any", [selected.get("match")])
            rows = [
                r
                for r in audits[selected["platform"]]["candidates"]
                if r["source_path"] in matches
            ]
            assert rows and set(r["source_path"] for r in rows) == set(matches)
            for row in rows:
                tree = Path(row["tree_path"]).parent
                name = ROOT + str(tree / "files" / row["source_path"])
                body = bodies[name]
                assert (
                    len(body) == row["source_bytes"]
                    and digest(body) == row["source_sha256"]
                )
                git_blob = hashlib.sha1(
                    b"blob " + str(len(body)).encode() + b"\0" + body
                ).hexdigest()
                assert git_blob == row["git_blob"]
                capture = json.loads(bodies[ROOT + str(tree / "capture.json")])
                commit = json.loads(bodies[ROOT + str(tree / "commit.json")])
                assert commit["sha"] == capture["commit"]
                originals.append(
                    dict(
                        source=name[len(ROOT) :],
                        original_path=row["source_path"],
                        repository=capture.get("repository", capture["url"]),
                        commit=commit["sha"],
                        selected_source_profile=tree.name,
                        reconstructed_tree=row["tree_sha"],
                        api_response_treeish=row["api_response_treeish"],
                        git_blob=git_blob,
                        bytes=len(body),
                        sha256=digest(body),
                    )
                )
        program["originals"] = originals
        program["duplicate_version_bodies"] = (
            len(originals) > 1 and len({r["sha256"] for r in originals}) == 1
        )
        cells = {}
        for language in languages:
            source = (
                ROUTES.get(language)
                if selected.get("authored_source_routes") == "patrol"
                else None
            )
            cell = dict(
                SOURCE="AUTHORED_UNQUALIFIED" if source else "NOT_IMPLEMENTED",
                BUILT=False,
                VALIDATED=False,
                GUEST=False,
                NATIVE_INTEGRATION=False,
                official_parity_accepted=False,
                receipts=[],
            )
            if source:
                files = [
                    name
                    for name in bodies
                    if name.startswith(
                        PATROL + source[len(PATROL) :].split("/")[0] + "/"
                    )
                ]
                if language == "c":
                    files = [source]
                if language in ("java", "kotlin", "csharp", "fsharp", "vbnet"):
                    files.append(PATROL + "managed-build.py")
                if language in ("csharp", "fsharp", "vbnet"):
                    files.extend(
                        [PATROL + "Directory.Build.props", PATROL + "global.json"]
                    )
                files = sorted(set(files))
                assert source in bodies and files
                cell["source_pins"] = [
                    dict(
                        path=name, bytes=len(bodies[name]), sha256=digest(bodies[name])
                    )
                    for name in files
                ]
                cell["entry"] = source
                cell["public_language_id"] = LANGUAGE_DIR.get(language, language)
                cell["remaining"] = (
                    "Current installed xmake/compiler/validator/Guest/native action receipts and explicit original input-domain parity remain required"
                )
                if language in ("java", "kotlin", "csharp", "fsharp", "vbnet"):
                    cell["remaining"] = (
                        "Owned actual Patrol entry/project producer is authored; syntax, full installed offline target-runtime closure, genuine compiler/validator/Guest and native action receipts remain required"
                    )
                if language == "assemblyscript":
                    cell["source_profiles"] = ["humble"]
            cells[language] = cell
        program["languages"] = cells
        programs.append(program)

    # A real build receipt qualifies only its exact installed Humble command.
    # Preserve the original program/input-domain and positive-action gaps.
    qualification = None
    receipt_name = "qualification/installed-patrol-nim-r3.json"
    if receipt_name in bodies:
        qualification = json.loads(bodies[receipt_name])
        owner_name = "qualification/installed-patrol-nim-r3-owner.json"
        copies_name = "qualification/installed-patrol-nim-r3-copies.json"
        owner = json.loads(bodies[owner_name])
        copies = json.loads(bodies[copies_name])
        assert owner["status"] == "PASS" and owner["default_authority"] == []
        assert copies["copies"] == 2 and copies["result_sha256"] == digest(bodies[owner_name])
        assert qualification["status"] == "PASS_PRIVATE_INSTALLED_NIM_PATROL_BUILD_VALIDATION_AND_EMPTY_AUTHORITY_GUESTS"
        assert qualification["SDK_unchanged_during_build"] and qualification["parent_SDK_unchanged"]
        assert qualification["actual_installed_xmake_builds"] == qualification["wasm_validations"] == 1
        assert qualification["actual_Guest_cases"] == 8 and qualification["positive_ROS_Guest_runs"] == 0
        assert qualification["Gateway_runs"] == qualification["native_ROS_runs"] == qualification["physical_robot_runs"] == 0
        assert qualification["default_authority"] == [] and not qualification["official_parity_accepted"]
        assert qualification["build_receipt"]["selection"] == dict(example="patrol-client.humble", language="nim", platform="robotis")
        dependencies = {}
        for row in qualification["explicit_source_overlays"]:
            assert row["source"] in bodies and digest(bodies[row["source"]]) == row["after_sha256"]
            dependencies[row["source"]] = row["after_sha256"]
        assert len(dependencies) == 9 and len(qualification["explicit_source_overlays"]) == 13
        artifact_name = "qualification/installed-patrol-nim-r3.wasm"
        artifact = bodies[artifact_name]
        assert artifact[:8] == b"\0asm\1\0\0\0"
        assert len(artifact) == qualification["artifact"]["bytes"] and digest(artifact) == qualification["artifact"]["sha256"]
        matching = [p for p in programs if p.get("authored_source_routes") == "patrol"]
        assert len(matching) == 1
        cell = matching[0]["languages"]["nim"]
        cell.update(
            BUILT=True, VALIDATED=True, GUEST=True,
            qualification_scope="One private installed Humble wasm32-wasip1 bounded command; aggregate flags mean this exact scoped receipt exists",
            profile_results={"humble": dict(BUILT=True, VALIDATED=True, GUEST=True, NATIVE_INTEGRATION=False),
                             "jazzy": dict(BUILT=False, VALIDATED=False, GUEST=False, NATIVE_INTEGRATION=False),
                             "main": dict(BUILT=False, VALIDATED=False, GUEST=False, NATIVE_INTEGRATION=False)},
            Guest_scope=dict(pure_goal=3, quit=1, default_denial=1, invalid_input=3, positive_action=0),
            build_dependency_pins=dependencies,
            receipts=[dict(path=name, bytes=len(bodies[name]), sha256=digest(bodies[name]))
                      for name in (receipt_name, owner_name, copies_name, artifact_name)],
            remaining="Jazzy/main builds, positive Guest/Gateway/native action integration and full original input-domain/interactive behavior parity remain required",
        )

    # Materialize only the small installed rule tree for genuine xmake imports.
    rules = work / "installed-rules"
    rules.mkdir()
    prefix = ROOT + "platform/robotis/example-rules/"
    for name, body in bodies.items():
        if name.startswith(prefix) and name.endswith(".lua"):
            target = rules / name[len(prefix) :]
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(body)
    script = work / "catalog-check.lua"
    script.write_text(
        """function main(root,output)
  local json=import("core.base.json")
  local rows={}
  for _,language in ipairs({"c","cpp","rs","go","tinygo","zig","py","js","as","java","kotlin","csharp","fsharp","vbnet","moonbit","swift","haskell","nim","d","odin","lua"}) do
    local module=import("xmake",{rootdir=path.join(root,language),anonymous=true})
    assert(type(module.build)=="function" and type(module.prepare)=="function" and type(module.inspect)=="function")
    local catalog=module.catalog()
    assert(catalog.format=="uwvm-sdk-examples-v1")
    local entries={};local ids={};local patrol=0
    for _,entry in ipairs(catalog.examples) do
      assert(not ids[entry.id]);ids[entry.id]=true
      if entry.id:sub(1,14)=="patrol-client." then
        patrol=patrol+1
        if language=="java" or language=="kotlin" or language=="csharp" or language=="fsharp" or language=="vbnet" then
          assert(entry.managed_local and not entry.unavailable)
          assert(entry.managed_builder==((language=="java" or language=="kotlin") and language or "dotnet"))
          if entry.managed_builder=="dotnet" then assert(type(entry.project)=="string" and #entry.project>0) end
        end
      end
      entries[#entries+1]={id=entry.id,source=entry.source,unavailable=entry.unavailable,
          managed_builder=entry.managed_builder,managed_local=entry.managed_local,project=entry.project}
    end
    assert(patrol==(language=="as" and 1 or 3))
    json.mark_as_array(entries)
    rows[#rows+1]={language=language,entries=entries,patrol_recipes=patrol}
  end
  json.mark_as_array(rows)
  json.savefile(output,{format="uwvm-task08-patrol-catalog-import-v1",rows=rows,actual_builds=0,Guest_runs=0})
end
"""
    )
    tool = Path("/home/macromodel/.local/bin/xmake")
    environment = dict(
        os.environ,
        XMAKE_STATS="false",
        XMAKE_SKIP_HISTORY="y",
        XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake",
    )
    argv = [str(tool), "lua", str(script), str(rules), str(work / "catalogs.json")]
    with (
        (work / "xmake.stdout").open("xb") as stdout,
        (work / "xmake.stderr").open("xb") as stderr,
    ):
        result = subprocess.run(
            argv,
            env=environment,
            cwd=work,
            stdin=subprocess.DEVNULL,
            stdout=stdout,
            stderr=stderr,
            timeout=45,
        )
    tool_report = dict(
        argv=argv,
        tool_sha256=digest(tool.read_bytes()),
        exit=result.returncode,
        stdout_sha256=digest((work / "xmake.stdout").read_bytes()),
        stderr_sha256=digest((work / "xmake.stderr").read_bytes()),
    )
    report = dict(
        schema="uwvm-task08-logical-program-language-ledger-v1",
        review_sha256=digest(bodies[review_name]),
        input_packet_sha256=digest(packet.read_bytes()),
        input_pins=[
            dict(path=name, bytes=len(body), sha256=digest(body))
            for name, body in sorted(bodies.items())
        ],
        platforms=review["platforms"],
        programs=programs,
        excluded_candidates=review["excluded_candidates"],
        requested_languages=languages,
        reviewed_logical_programs=len(programs),
        whole_vendor_denominators_complete=False,
        actual_program_xmake_builds=1 if qualification else 0,
        current_program_Wasm_validations=1 if qualification else 0,
        current_program_Guest_runs=8 if qualification else 0,
        current_program_positive_action_Guest_runs=0,
        current_program_native_integrations=0,
        official_program_parity_acceptances=0,
        xmake_catalog_import=tool_report,
        default_authority=[],
        complete_platform_support=False,
        status=(
            ("SOURCE_INVENTORY_CATALOGS_AND_ONE_SCOPED_INSTALLED_NIM_PROOF"
             if qualification else "SOURCE_INVENTORY_AND_CATALOGS_ONLY")
            if result.returncode == 0
            else "FAILED_CATALOG_IMPORT_RETAINED"
        ),
    )
    encoded = (json.dumps(report, indent=2, sort_keys=True) + "\n").encode()
    compressed = gzip.compress(encoded, compresslevel=9, mtime=0)
    assert len(compressed) <= 262144
    (work / "program-language-ledger.json.gz").write_bytes(compressed)
    print(
        json.dumps(
            dict(
                status=report["status"],
                logical_programs=len(programs),
                languages=len(languages),
                source_routes=len(ROUTES),
                catalog_exit=result.returncode,
            )
        ),
        flush=True,
    )
    if result.returncode:
        raise SystemExit(result.returncode)


if __name__ == "__main__":
    main()
