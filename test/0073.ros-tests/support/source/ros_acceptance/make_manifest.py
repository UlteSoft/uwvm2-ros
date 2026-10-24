#!/usr/bin/env python3
"""Materialize a closed same-freeze serial manifest, without running any test.

The selection JSON contains the exact reviewed legacy argv/record selectors and
all explicit physical inputs. This renderer expands data placeholders, writes
immutable per-job normalization recipes, adds the39 gap invocations and checks
the complete original35/12/50/55/20/23 references. Missing selectors/products
are errors, never substituted with historical PASS, a permission preset or a
documentation-only Runtime result. Use SourceB after SourceA custom generation
and trusted old+new catalog composition have been frozen into that same source.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import sys

sys.dont_write_bytecode=True
from protocol import CASES, LANGUAGES, require, load_bound
from requirements import inventories
from normalize import DRIVERS

# normalize imports Lane, whose legacy dependencies prepend ros_aarch64 to
# sys.path. Load this sibling by its physical path so the unrelated legacy
# run.py cannot supply these acceptance constants. The private module name
# never invokes the runner's guarded main entry or creates a Lane.
_runner_spec = importlib.util.spec_from_file_location(
    "uwvm_ros_acceptance_manifest_runner_constants", Path(__file__).resolve().with_name("run.py"))
require(_runner_spec is not None and _runner_spec.loader is not None,
        "sibling acceptance runner module required")
_runner_constants = importlib.util.module_from_spec(_runner_spec)
_runner_spec.loader.exec_module(_runner_constants)
BASE_SUITES = _runner_constants.BASE_SUITES
SUPPLEMENTAL_SCOPES = _runner_constants.SUPPLEMENTAL_SCOPES
SDK_SCOPE = _runner_constants.SDK_SCOPE


def expand(value,inputs,output,products):
    """Resolve explicit data references; never interpolate shell command text."""
    if isinstance(value,dict):
        if set(value)=={"input"}:
            require(value["input"] in inputs,"explicit input missing");return inputs[value["input"]]
        if set(value)=={"output"}:
            relative=Path(value["output"]);require(not relative.is_absolute() and ".." not in relative.parts,"output reference escapes row")
            return str(output/relative)
        if set(value)=={"product_of","path"}:
            require(value["product_of"] in products,"future product prerequisite not earlier in serial manifest")
            relative=Path(value["path"]);require(not relative.is_absolute() and ".." not in relative.parts,"future product path escapes row")
            return str(products[value["product_of"]]/relative)
        return {key:expand(item,inputs,output,products) for key,item in value.items()}
    if isinstance(value,list):return [expand(item,inputs,output,products) for item in value]
    require(type(value) in (str,int,bool) or value is None,"unsupported manifest data type");return value


def raw_fact(key,expected):return {"path":[{"key":key}],"operator":"eq","expected":expected}


def gap_record(case,version=None,language=None,mode=None):
    """Bind a dedicated driver's actual contract, not a neighbouring result."""
    facts=[raw_fact("status","PASS"),raw_fact("new_tests_executed",True)]
    for key in ("source_preserved","sdk_preserved","inputs_preserved","products_preserved"):facts.append(raw_fact(key,True))
    labels=[]
    if case.startswith("sdk-") or case.startswith("memory1-"):
        facts+=[raw_fact("case","sdk" if case.startswith("sdk-") else "memory1")]
        versions=[1,2] if case.endswith("dual") else [version]
        labels=["worker",*["gateway-"+str(v) for v in versions],*["peer-"+str(v) for v in versions]]
        if language is not None:facts+=[raw_fact("languages",[language]),raw_fact("versions",[version]),raw_fact("sdk_mode",mode)]
    elif case in ("master-restart","tcpros-peer-restart","dds-peer-restart"):
        facts+=[raw_fact("case",case),raw_fact("actual_ROS_delivery","VERIFIED"),raw_fact("retained_survivor_process",True),raw_fact("retained_survivor_Guest_handles",True)]
        labels=["worker","gateway-1","gateway-2","recovery-peer-1","recovery-peer-2"]
    elif "graph-query" in case or case.startswith(("wrong-uid-","raw-frame-")):
        name="graph-query-flood" if case.startswith("graph-query") else "wrong-uid" if case.startswith("wrong-uid") else "raw-frame"
        facts+=[raw_fact("case",name+"-ros"+str(version)),raw_fact("native_zero_resource_effects",True)]
        labels=["peer-"+str(version),"gateway-1" if version==1 else "observed-gateway-2"]
        if name=="graph-query-flood":labels.append("worker")
    else:
        # Metrics has a native actual queue/IPC contract with independent
        # checked counters. It is not relabelled as authenticated Wasm ROS.
        labels=["metrics"]
    result={"id":case,"kind":"native" if case.startswith("queue-") or case=="isolated-ipc" else "runtime" if case.startswith(("sdk-","memory1-","graph-query")) or case in ("master-restart","tcpros-peer-restart","dds-peer-restart") else "native",
        "path":[],"facts":facts,"commands":[{"driver_exit":True}],"mapped_ARM_labels":labels}
    if case.startswith("graph-query"):
        result["side_effect"]=[{"key":"graph_flood_denial"}]
    elif case.startswith(("raw-frame-","wrong-uid-")):
        result["security_contract"]={"kind":"startup-refusal","scope":"Actual hostile host/socket metadata refusal plus independent native Graph/resource observation; no Guest errno is invented",
            "assertions":[raw_fact("native_zero_resource_effects",True),raw_fact("new_tests_executed",True)]}
    return result


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ("source","manifest","selection","out","execution-out"):parser.add_argument("--"+name,type=Path,required=True)
    args=parser.parse_args();source=args.source.resolve(strict=True);selection,selection_pin=load_bound(args.selection)
    manifest_digest=hashlib.sha256(args.manifest.read_bytes()).hexdigest();out=args.out.resolve();execution=args.execution_out.resolve()
    require(not out.exists() and not execution.exists() and out!=execution,"fresh separate plan/execution outputs required")
    require(selection.get("format_version")==1 and type(selection.get("format_version")) is int and selection.get("kind")=="ros-final-explicit-selection"
        and selection["source_manifest_sha256"]==manifest_digest,"current SourceB selection required")
    inputs=dict(selection["inputs"]);inputs.update(source=str(source),manifest=str(args.manifest.resolve()))
    for key,value in inputs.items():require(type(value) is str and "\0" not in value,"literal explicit input strings required")
    required=("sdk-workspace","qemu","binfmt-evidence","container-started-at","worker","worker-report","worker-config",
        "ros1-gateway","ros1-gateway-report","ros2-gateway","ros2-gateway-report","wat2wasm","prepared-sdk","prepared-recovery",
        "prepared-metrics","prepared-observer","delivery-products","node")
    require(set(required)<=set(inputs),"explicit qualified current products/preparations missing")
    out.mkdir(mode=0o700,parents=True);recipes=out/"recipes";recipes.mkdir();launches=[];products={};coverage=dict(selection["coverage"])
    common=["--source",str(source),"--manifest",str(args.manifest.resolve()),"--sdk-workspace",inputs["sdk-workspace"],"--qemu",inputs["qemu"],
        "--binfmt-evidence",inputs["binfmt-evidence"],"--container-started-at",inputs["container-started-at"]]
    def write_job(job):
        label=job["id"];require(label not in products and label and all(c.isalnum() or c=="-" for c in label),"closed unique job label required")
        target=execution/label;raw=target/"raw";value=expand(job,inputs,raw,products)
        require(value["driver"] in DRIVERS,"unapproved selected driver")
        arguments=value["arguments"]
        require(arguments.count("--source")==1 and arguments[arguments.index("--source")+1]==str(source),"legacy exact source required")
        require(arguments.count("--out")==1 and Path(arguments[arguments.index("--out")+1]).is_relative_to(raw),"legacy dedicated raw output required")
        requires=value["requires"];require(requires==sorted(set(requires)) and set(requires)<=set(products),"serial prerequisites not earlier")
        recipe={"format_version":1,"kind":"ros-legacy-bound-recipe","source_manifest_sha256":manifest_digest,
            "documents":value.get("documents",[]),
            **{key:value[key] for key in ("driver","arguments","timeout_seconds","rss_bytes","raw_report","raw_success_path","raw_success_value","source_association","raw_architecture","records")}}
        path=recipes/(label+".json");path.write_text(json.dumps(recipe,indent=2)+"\n")
        launches.append({"id":label,"driver":"test/0073.ros-tests/support/source/ros_acceptance/normalize.py","arguments":[*common,"--out",str(target),"--recipe",str(path)],
            "requires":requires,"timeout_seconds":value["timeout_seconds"]+120,"rss_bytes":value["rss_bytes"],"report":str(target/"report.json")})
        if value["driver"]=="test/0073.ros-tests/support/source/ros_acceptance/run_sdk_matrix.py":
            require(value["records"] and all(row["kind"]=="sdk-compiler-model" for row in value["records"]),
                    "SDK adapter requires only scoped supplemental records")
            launches[-1]["evidence_scope"]=SDK_SCOPE
        products[label]=raw
    # Reviewed old argv/selectors are explicit inputs because their schemas
    # differ. The renderer never invents original facts from filenames.
    for job in selection["jobs"]:write_job(job)
    require(selection["gap_prerequisites"]==sorted(set(selection["gap_prerequisites"])) and set(selection["gap_prerequisites"])<=set(products),"gap preparation dependency list differs")
    worker=[]
    for name in ("worker","worker-report","worker-config","ros1-gateway","ros1-gateway-report","ros2-gateway","ros2-gateway-report"):
        worker += ["--"+name,inputs[name]]
    for case in CASES:
        if case.startswith("sdk-"):
            _,language,backend,mode=case.split("-");version=int(backend.removeprefix("ros"))
            driver="test/0073.ros-tests/support/source/ros_acceptance/run_delivery.py";extra=["--case","sdk","--language",language,"--version",str(version),"--mode",mode,"--products",inputs["delivery-products"],"--prepared-native",inputs["prepared-sdk"]]
            if language=="javascript":extra += ["--node",inputs["node"]]
        elif case.startswith("memory1-"):
            backend=case.removeprefix("memory1-");version=None if backend=="dual" else int(backend.removeprefix("ros"));language=None;mode=None
            driver="test/0073.ros-tests/support/source/ros_acceptance/run_delivery.py";extra=["--case","memory1","--version",backend.removeprefix("ros"),"--prepared-native",inputs["prepared-sdk"]]
        elif case in ("master-restart","tcpros-peer-restart","dds-peer-restart"):
            version=None;language=None;mode=None;driver="test/0073.ros-tests/support/source/ros_acceptance/run_recovery.py";extra=["--case",case,"--prepared-native",inputs["prepared-recovery"]]
        elif case in ("queue-reject","queue-drop-oldest","isolated-ipc"):
            version=None;language=None;mode=None;driver="test/0073.ros-tests/support/source/ros_acceptance/run_metrics.py";extra=["--prepared-native",inputs["prepared-metrics"]]
        else:
            version=int(case[-1]);language=None;mode=None;driver="test/0073.ros-tests/support/source/ros_acceptance/run_attacks.py"
            kind="graph-query-flood" if case.startswith("graph-query") else "wrong-uid" if case.startswith("wrong-uid") else "raw-frame"
            extra=["--case",kind,"--version",str(version),"--prepared-native",inputs["prepared-sdk"]]
            if version==2:extra += ["--prepared-observer",inputs["prepared-observer"]]
        # run_metrics owns a native-only recipe and accepts no Worker/wat flags;
        # the other dedicated drivers preserve all explicit product arguments.
        raw_args=[*common,"--out",{"output":"."},*([] if driver.endswith("run_metrics.py") else [*worker,"--wat2wasm",inputs["wat2wasm"]]),*extra]
        records=[gap_record(case,version,language,mode)]
        if case.startswith("graph-query"):
            denial=json.loads(json.dumps(records[0]));denial["id"]=case+"-denial";denial["side_effect"]=[{"key":"graph_denial"}];records.append(denial)
        write_job({"id":case,"driver":driver,"arguments":raw_args,"requires":selection["gap_prerequisites"],"timeout_seconds":300,"rss_bytes":8*1024**3,
            "raw_report":{"output":"report.json"},"raw_architecture":{"output":"execution/architecture-evidence.json"},"raw_success_path":[{"key":"status"}],"raw_success_value":"PASS",
            "source_association":{"kind":"raw-manifest","path":[{"key":"source_manifest_sha256"}]},"records":records})
        coverage[case]=[case]
    inventory=inventories(source);references=selection["original_requirements"]
    record_ids={row["id"]:{record["id"] for record in json.loads((recipes/(row["id"]+".json")).read_text())["records"]} for row in launches}
    for category,rows in inventory.items():
        require(set(references[category])=={str(row["id"]) for row in rows},"complete original inventory references missing: "+category)
        for item in references[category].values():
            require(item,"empty original evidence mapping")
            for reference in item:require(reference["launch"] in record_ids and reference["record"] in record_ids[reference["launch"]],"original evidence record missing")
    require(set(coverage)==set(BASE_SUITES)|set(CASES),"complete mandatory base plus39 coverage differs")
    for labels in coverage.values():require(labels and set(labels)<=set(products),"unbound mandatory coverage job")
    supplement=selection.get("supplemental_coverage",{})
    require(set(supplement)==SUPPLEMENTAL_SCOPES,"complete separate SDK supplemental scope missing")
    for labels in supplement.values():
        require(labels and len(labels)==len(set(labels)) and set(labels)<=set(products) and
            all(next(row for row in launches if row["id"]==label).get("evidence_scope")==SDK_SCOPE for label in labels),
            "SDK supplemental coverage selects another evidence kind")
    result={"format_version":1,"kind":"ros-final-serial-launches","status":"PREPARED_NOT_EXECUTED","source_manifest_sha256":manifest_digest,
        "selection":selection_pin,"launches":launches,"base_suites":sorted(BASE_SUITES),"gap_cases":list(CASES),"coverage":coverage,
        "original_requirements":references,"inventory_counts":{key:len(value) for key,value in inventory.items()},
        "supplemental_scopes":sorted(SUPPLEMENTAL_SCOPES),"supplemental_coverage":supplement,
        "source_derivation":"SourceA official bounded-v2 build/identity export -> trusted fixed+bounded bundle -> full immutable SourceB -> all final products and actual suites",
        "actual_results_reused":False,"hosted_GitHub_execution_claimed":False}
    (out/"launches.json").write_text(json.dumps(result,indent=2)+"\n");return 0


if __name__=="__main__":raise SystemExit(main())
