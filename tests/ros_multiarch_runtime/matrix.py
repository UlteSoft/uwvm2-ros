#!/usr/bin/env python3
"""Explicit complete8-architecture x2-backend serial Runtime/ROS matrix.

Unavailable inputs are BLOCKED, never skipped into PASS. Each profile produces
fresh build and execution receipts; compiler-only and Native SDK-only evidence
cannot satisfy actual Runtime communication. No external installation, SSH,
container launch, binfmt mutation or implicit capability is performed.
"""
import argparse
import json
from pathlib import Path
import sys
import traceback

sys.dont_write_bytecode=True
from profile import ARCHES, validate, require, exact, available
from engine import helpers, environment


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ("source","manifest","selection","out"):
        parser.add_argument("--"+name,type=Path,required=True)
    args=parser.parse_args();source=args.source.resolve(strict=True);common=helpers(source,args.manifest)
    selection,selection_pin=common.document(args.selection.resolve(strict=True))
    exact(selection,("schema_version","cgroup","profiles"),"matrix")
    require(selection["schema_version"]==1 and type(selection["profiles"]) is list,"closed matrix schema1 required")
    expected={(architecture,backend) for architecture in ARCHES for backend in (1,2)}
    require(len(selection["profiles"])==16,"all8architectures x2backends must be explicitly selected")
    actual=set()
    for row in selection["profiles"]:
        exact(row,("architecture","backend","profile"),"matrix cell")
        require(Path(row["profile"]).is_absolute(),"explicit profile path required")
        key=(row["architecture"],row["backend"]);require(key in expected and key not in actual,"unknown/duplicate architecture-backend cell")
        actual.add(key)
    require(actual==expected,"complete16-cell matrix required")
    out=args.out.resolve();require(not out.exists() and out!=source and source not in out.parents and out not in source.parents,"fresh output outside frozen source required");out.mkdir(mode=0o700,parents=True)
    common.install_interrupts();source_before=common.source_state(source,args.manifest)
    before=common.cgroup_state(selection["cgroup"])
    report={"kind":"multiarch-runtime-matrix","status":"RUNNING","source_manifest_sha256":source_before["manifest"]["sha256"],
            "selection":selection_pin,"rows":[],"scope":"Runtime architecture and native ROS Gateway architecture are independent; compiler/models never count as Runtime behavior"}
    common.atomic_json(out/"report.json",report)
    python=Path(sys.executable).resolve(strict=True);python_pin=common.opened(python)
    runner=None
    try:
        for index,cell in enumerate(selection["profiles"]):
            row={**cell,"status":"RUNNING"};report["rows"].append(row)
            try:
                profile,pin=common.document(Path(cell["profile"]).resolve(strict=True));validate(profile)
                require(profile["runtime"]["architecture"]==cell["architecture"] and profile["backend"]==cell["backend"] and
                        profile["cgroup"]==selection["cgroup"],"profile identity differs from explicit matrix cell")
                available(profile)
                directory=out/("%02d-"%index+profile["id"]);directory.mkdir(mode=0o700)
                # Coordinator overhead is a separate explicit256MiB allowance;
                # each child still enforces its original compiler/actor bounds.
                bound=max(profile["limits"]["compiler_rss"],profile["limits"]["worker_rss"]+4*profile["limits"]["native_rss"])+256*1024**2
                limits={"rss_bytes":bound,"log_bytes":16*1024**2,"command_seconds":36000}
                runner=common.SerialRunner(directory,limits,lambda:common.cgroup_preserved(before,common.cgroup_state(selection["cgroup"])))
                prepared=directory/"prepare";executed=directory/"execute"
                base=["--source",str(source),"--manifest",str(args.manifest.resolve(strict=True)),"--profile",str(Path(cell["profile"]).resolve(strict=True))]
                runner.run("prepare",[str(python),"-B",str(source/"tests/ros_multiarch_runtime/prepare.py"),*base,"--out",str(prepared)],directory,environment())
                product,product_pin=common.document(prepared/"report.json")
                require(product["status"]=="PREPARED" and product["source_manifest_sha256"]==report["source_manifest_sha256"],"fresh build products incomplete")
                runner.run("execute",[str(python),"-B",str(source/"tests/ros_multiarch_runtime/run.py"),*base,"--prepared",str(prepared/"report.json"),"--out",str(executed)],directory,environment())
                result,result_pin=common.document(executed/"report.json")
                require(result["status"]=="PASS" and result["source_manifest_sha256"]==report["source_manifest_sha256"] and result["profile"]==profile and
                        [r["case"] for r in result["cases"]]==profile["cases"] and all(r["status"]=="PASS" for r in result["cases"]),"actual complete13-case Runtime result required")
                require(all(result[k] is True for k in ("source_preserved","inputs_preserved","whole_roots_preserved","new_tests_executed")),"actual preservation flags incomplete")
                for p in result["input_pins"]:
                    require(common.opened(Path(p["path"]))==p,"actual matrix evidence changed")
                common.cgroup_preserved(before,result["cgroup_before"]);common.cgroup_preserved(before,result["cgroup_after"])
                require(all(c["status"]=="PASS" for c in result["process_cleanup"]),"actual actor cleanup incomplete")
                row.update(status="PASS",profile_pin=pin,preparation=product_pin,execution=result_pin,
                           runtime_architecture=profile["runtime"]["architecture"],native_ros_architecture=profile["native"]["architecture"])
            except FileNotFoundError as error:
                row.update(status="BLOCKED",error="Explicit SDK/tool/profile/product unavailable: "+str(error))
            except BaseException as error:
                row.update(status="FAIL",error=repr(error),traceback=traceback.format_exc())
                if isinstance(error,(KeyboardInterrupt,InterruptedError,SystemExit)):
                    raise
            finally:
                if runner:
                    row["commands"]=runner.commands;row["process_cleanup"]=runner.close();runner=None
                    if row["process_cleanup"]["status"]!="PASS":
                        row["status"]="FAIL"
                common.atomic_json(out/"report.json",report)
    finally:
        report["source_preserved"]=common.source_state(source,args.manifest)==source_before
        common.cgroup_preserved(before,common.cgroup_state(selection["cgroup"]))
        report["inputs_preserved"]=common.opened(python)==python_pin and common.opened(args.selection.resolve(strict=True))==selection_pin
        report["status"]="PASS" if len(report["rows"])==16 and all(r["status"]=="PASS" for r in report["rows"]) and report["source_preserved"] and report["inputs_preserved"] else "INCOMPLETE"
        report["counts"]={status:sum(r["status"]==status for r in report["rows"]) for status in ("PASS","FAIL","BLOCKED","RUNNING")}
        report["authoritative_Runtime_architectures"]=list(ARCHES) if report["status"]=="PASS" else []
        common.atomic_json(out/"report.json",report)
    return 0 if report["status"]=="PASS" else 1


if __name__=="__main__":
    raise SystemExit(main())
