#!/usr/bin/env python3
"""Execute one closed legacy recipe and bind its raw evidence without mutation.

Legacy drivers retain their own argv/report schemas. This wrapper adds current
source/cgroup/ownership provenance and explicitly selected actual records. It
does not rewrite a prior receipt, infer native behavior from source/documents,
or upgrade an unsupported operation to a working Runtime feature. Record
selectors are data-only, bounded and pinned in the frozen launch recipe.
"""
import argparse
from pathlib import Path
import re
import sys
import traceback

sys.dont_write_bytecode=True
from lane import Lane, add_arguments
from protocol import require, physical_pin, side_effect
from gateway_qualification import architecture_binding
from runtime import require_arm, validate_architecture_mappings
from run_all import validate_fresh_worker_inputs

# The complete driver list is shared by value, not by importing run.py's entry.
# New dedicated drivers also pass through this wrapper when original inventory
# records must be associated with their exact current raw report rows.
DRIVERS=frozenset(("test/0073.ros-tests/support/source/ros_aarch64/build_worker.py","test/0073.ros-tests/support/source/ros_aarch64/rebuild_affected.py","test/0073.ros-tests/support/source/ros_aarch64/run_all.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_native.py","test/0073.ros-tests/support/source/ros_aarch64/run_cli.py","test/0073.ros-tests/support/source/ros_aarch64/run_regression.py","test/0073.ros-tests/support/source/ros_aarch64/run_extended.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_original_regressions.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_unit.py","test/0073.ros-tests/support/source/ros_aarch64/run_ros1_search.py","test/0073.ros-tests/support/source/ros_aarch64/run_resources.py","test/0073.ros-tests/support/source/ros_aarch64/run_action_qos.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_native_adoption.py","test/0073.ros-tests/support/source/ros_aarch64/run_local_parameter_events.py","test/0073.ros-tests/support/source/ros_aarch64/run_g09_guest_enforce.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_revoke_reconnect.py","test/0073.ros-tests/support/source/ros_aarch64/run_custom_topics.py","test/0073.ros-tests/support/source/ros_aarch64/run_custom_guest.py",
    "test/0073.ros-tests/support/source/ros_acceptance/prepare_native.py","test/0073.ros-tests/support/source/ros_acceptance/build_delivery.py","test/0073.ros-tests/support/source/ros_acceptance/run_delivery.py",
    "test/0073.ros-tests/support/source/ros_acceptance/run_recovery.py","test/0073.ros-tests/support/source/ros_acceptance/run_attacks.py","test/0073.ros-tests/support/source/ros_acceptance/run_metrics.py","test/0073.ros-tests/support/source/ros_acceptance/run_bounded_service.py",
    "examples/ros/materials.py","test/0073.ros-tests/support/source/ros_acceptance/run_events.py","test/0073.ros-tests/support/source/ros_acceptance/run_sdk_matrix.py",
    "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/run_custom_bounded.py"))

RUN_ALL_ALIASES = frozenset(("/a64", "/ar6", "/r64", "/z64"))
RPC_ALIASES = frozenset(("/gp64", "/pg64", "/rg64", "/gr64"))


def run_all_witness_log_bindings(lane, raw, argv):
    """Admit only logs captured through the original live workspace mounts.

    The original Runner reports keep their alias paths unchanged. run_all
    captures each opened alias/physical identity before unmounting. A nested
    RPC wrapper contributes its own earlier capture; the parent appends its
    live outer-mount step. This function validates that explicit chain and its
    pinned child report. It never reconstructs a missing pin from a pathname.
    """
    def canonical(value):
        return (type(value) is str and value.startswith("/") and not any(c in value for c in ("\0", "\r", "\n"))
                and str(Path(value)) == value and ".." not in Path(value).parts)

    def workspace(value, aliases):
        require(isinstance(value, dict) and set(value)=={"alias", "origin", "alias_directory", "origin_directory"},
            "closed captured workspace alias association required")
        require(value["alias"] in aliases and canonical(value["origin"]), "captured workspace roots differ")
        if "alias_directory" in value:
            for name, path in (("alias_directory", value["alias"]), ("origin_directory", value["origin"])):
                entry=value[name]
                require(isinstance(entry, dict) and set(entry)=={"path", "device", "inode"}
                    and entry["path"]==path and all(type(entry[key]) is int and entry[key]>0 for key in ("device", "inode")),
                    "captured workspace directory identity differs")
            require(all(value["alias_directory"][key]==value["origin_directory"][key] for key in ("device", "inode")),
                    "captured alias did not name the same physical directory")
        return value

    fields={"path", "device", "inode", "bytes", "mtime_ns", "ctime_ns", "sha256"}
    def full_pin(value, limit=16*1024**2):
        require(isinstance(value, dict) and set(value)==fields and canonical(value["path"])
            and all(type(value[key]) is int and value[key]>0 for key in ("device", "inode"))
            and all(type(value[key]) is int and value[key]>=0 for key in ("bytes", "mtime_ns", "ctime_ns"))
            and value["bytes"]<=limit
            and type(value["sha256"]) is str and re.fullmatch(r"[a-f0-9]{64}", value["sha256"]),
            "complete opened witness log identity required")

    require(raw.get("source_manifest_sha256")==lane.report["source_manifest_sha256"]
        and argv.count("--workspace")==1, "current run_all source/workspace association required")
    index=argv.index("--workspace")
    require(index+1<len(argv) and canonical(argv[index+1]), "explicit absolute run_all workspace required")
    origin=Path(argv[index+1]).resolve(strict=True)
    binding=workspace(raw.get("workspace_alias_binding"), RUN_ALL_ALIASES)
    require(raw.get("workspace_origin")==str(origin)==binding["origin"]
        and origin.is_dir() and not origin.is_symlink() and raw.get("native_witness_logs_preserved") is True,
        "run_all original workspace/log preservation differs")
    if "origin_directory" in binding:
        current=origin.stat()
        require((current.st_dev,current.st_ino)==(binding["origin_directory"]["device"],binding["origin_directory"]["inode"]),
                "run_all original physical workspace identity changed")
    logs=raw.get("native_witness_log_bindings")
    require(isinstance(logs, dict) and 0<len(logs)<=10000, "captured native witness log inventory missing")
    children={}
    for alias_path, entry in logs.items():
        require(canonical(alias_path) and isinstance(entry, dict)
            and set(entry)=={"alias_path", "alias_log", "physical_log", "alias_chain", "producer_reports"}
            and entry["alias_path"]==alias_path, "closed captured witness log association required")
        chain=entry["alias_chain"]
        require(isinstance(chain, list) and len(chain) in (1,2), "captured witness alias chain differs")
        for step in chain:
            require(isinstance(step, dict) and set(step)=={"alias_root", "origin_root", "alias_log", "physical_log"}
                and step["alias_root"] in RUN_ALL_ALIASES|RPC_ALIASES and canonical(step["origin_root"]),
                "closed captured alias-chain step required")
            full_pin(step["alias_log"]);full_pin(step["physical_log"])
            aliased=Path(step["alias_log"]["path"])
            require(aliased.is_relative_to(step["alias_root"]) and aliased!=Path(step["alias_root"]),
                    "witness log escapes its captured alias root")
            relative=aliased.relative_to(step["alias_root"])
            require(Path(step["physical_log"]["path"])==Path(step["origin_root"])/relative
                and all(step["alias_log"][key]==step["physical_log"][key] for key in fields-{"path"}),
                "captured alias/physical witness identities differ")
        require(entry["alias_log"]==chain[0]["alias_log"] and entry["physical_log"]==chain[-1]["physical_log"]
            and entry["alias_log"]["path"]==alias_path and chain[-1]["alias_root"]==binding["alias"]
            and chain[-1]["origin_root"]==str(origin), "flat witness pins do not match the complete captured chain")
        producers=entry["producer_reports"]
        require(isinstance(producers, list) and len(producers)==len(chain)-1, "nested witness producer association missing")
        if len(chain)==1:
            require(chain[0]["alias_root"]==binding["alias"], "direct witness uses another workspace alias")
            continue
        require(chain[0]["alias_root"] in RPC_ALIASES and chain[0]["origin_root"]==binding["alias"]
            and chain[0]["physical_log"]==chain[1]["alias_log"], "nested witness chain is not an exact joined capture")
        producer=producers[0];full_pin(producer, 64*1024**2)
        rpc_rows=[row for row in raw.get("suites", []) if isinstance(row, dict) and row.get("suite")=="rpc"]
        require(len(rpc_rows)==1 and rpc_rows[0].get("status")=="PASS"
            and isinstance(rpc_rows[0].get("evidence"), dict)
            and all(rpc_rows[0]["evidence"].get(key)==producer[key] for key in fields),
            "nested witness producer is not the original selected RPC suite report")
        if producer["path"] not in children:
            child,pin=lane.json(producer["path"])
            require(pin==producer, "nested witness producer report changed")
            children[producer["path"]]=(child,pin)
        child,pin=children[producer["path"]]
        require(pin==producer and child.get("status")=="PASS"
            and child.get("source_manifest_sha256")==lane.report["source_manifest_sha256"]
            and child.get("native_witness_logs_preserved") is True,
            "nested witness producer lacks current successful preservation")
        child_workspace=workspace(child.get("workspace_alias_binding"), RPC_ALIASES)
        require(child.get("workspace_origin")==child_workspace["origin"]==binding["alias"]
            and child_workspace["alias"]==chain[0]["alias_root"], "nested witness producer workspace differs")
        if "origin_directory" in child_workspace and "alias_directory" in binding:
            require(child_workspace["origin_directory"]==binding["alias_directory"], "nested/outer workspace directory captures differ")
        captured=child.get("native_witness_log_bindings", {}).get(alias_path)
        require(isinstance(captured, dict)
            and set(captured)=={"alias_path", "alias_log", "physical_log", "alias_chain", "producer_reports"}
            and captured.get("alias_path")==alias_path
            and captured.get("alias_log")==entry["alias_log"] and captured.get("physical_log")==chain[0]["physical_log"]
            and captured.get("alias_chain")==chain[:1] and captured.get("producer_reports", [])==[],
            "nested witness log is not present in the exact pinned producer")
    return logs


def select(value,path):
    """Select one exact raw row with no eval, wildcard or first-match fallback."""
    require(isinstance(path,list) and len(path)<=32,"bounded data selector required")
    for step in path:
        require(isinstance(step,dict) and len(step) in (1,2),"closed selector step required")
        if set(step)=={"key"}:
            require(isinstance(value,dict) and type(step["key"]) is str and step["key"] in value,"raw object field missing")
            value=value[step["key"]]
        elif set(step)=={"index"}:
            require(isinstance(value,list) and type(step["index"]) is int and 0<=step["index"]<len(value)<=10000,"raw array index differs")
            value=value[step["index"]]
        elif set(step)=={"where","equals"}:
            require(isinstance(value,list) and len(value)<=10000 and type(step["where"]) is str,"raw exact row selector differs")
            matching=[item for item in value if isinstance(item,dict) and step["where"] in item and
                type(item[step["where"]]) is type(step["equals"]) and item[step["where"]]==step["equals"]]
            require(len(matching)==1,"raw selected row missing/ambiguous");value=matching[0]
        else:raise ValueError("unknown raw selector form")
    return value


def fact(actual,rule):
    operation=rule["operator"];expected=rule["expected"]
    if operation=="eq":require(type(actual) is type(expected) and actual==expected,"raw exact fact differs")
    elif operation=="length":require(isinstance(actual,(list,dict,str)) and type(expected) is int and len(actual)==expected,"raw inventory count differs")
    elif operation in ("gte","lte"):
        require(type(actual) is int and type(expected) is int and (actual>=expected if operation=="gte" else actual<=expected),"raw integer budget differs")
    else:raise ValueError("unknown raw fact operation")


def terminal_commands(lane,root,selectors):
    """Require original executed command outcomes plus their actual raw logs.

    A compilation recipe or prepared argv is not a command outcome. The log
    fields are explicitly selected because older drivers use different names.
    Every selected log must be regular, bounded and byte-pinned in that raw row.
    """
    require(isinstance(selectors,list) and selectors,"actual executed command association required")
    result=[]
    for item in selectors:
        if item=={"driver_exit":True}:
            command=next(row for row in lane.report["commands"] if row["name"]=="original-driver")
            require(command["status"]=="PASS" and command["returncode"]==0,"original driver did not actually complete")
            result.append({"scope":"Actual original driver exit and raw logs; unrecorded inner command outcomes are not invented",
                "argv":command["argv"],"returncode":command["returncode"],"logs":[lane.pin(command["stdout"]),lane.pin(command["stderr"])]})
            continue
        command=select(root,item["path"]);require(isinstance(command,dict),"raw command record required")
        code=select(command,item["returncode"])
        expected=item.get("expected_returncode",0)
        require(type(code) is int and type(expected) is int and code==expected,"actual command exit differs")
        argv=select(command,item["argv"]);require(isinstance(argv,list) and argv and all(type(arg) is str for arg in argv),"actual original command argv missing")
        logs=[]
        for path in item["logs"]:
            old=select(command,path);require(isinstance(old,dict) and "path" in old and "sha256" in old,"raw original log pin missing")
            now=lane.pin(old["path"]);require(now["sha256"]==old["sha256"] and ("bytes" not in old or now["bytes"]==old["bytes"]),"original command log changed")
            logs.append(now)
        require(logs,"actual command log binding missing");result.append({"raw_selector":item["path"],"argv":argv,"returncode":code,"logs":logs})
    return result


def record(lane,raw,specification,architecture,witness_log_bindings=None):
    if architecture is not None:validate_architecture_mappings(architecture)
    require(specification["kind"] in ("runtime","native","compiler","bounded-rejection","material","sdk-compiler-model"),"closed evidence kind required")
    evidence=select(raw,specification["path"]);require(isinstance(evidence,dict),"exact original raw record required")
    require(specification["facts"],"empty evidence predicates cannot establish acceptance")
    for item in specification["facts"]:fact(select(evidence,item["path"]),item)
    kind=specification["kind"];result={"id":specification["id"],"kind":kind,"status":"PASS","actual_execution":kind!="material",
        "raw_selector":specification["path"],"raw_record":evidence,"facts":specification["facts"]}
    if "complete_rows" in specification:
        rows=select(evidence,specification["complete_rows"])
        require(isinstance(rows,list) and 0<len(rows)<=10000,"complete current result inventory missing")
        names=[row.get("name") for row in rows]
        require(all(type(name) is str and name for name in names) and len(names)==len(set(names))
            and all(row.get("status")=="PASS" for row in rows),"current result inventory failed/skipped/duplicate")
        result["actual_current_cases"]=len(rows)
    if kind=="material":
        require(specification["materials"],"exact material files required")
        result.update(materials_verified=True,physical_material_pins=[lane.pin(path) for path in specification["materials"]])
    else:
        result["executed_commands"]=terminal_commands(lane,raw,specification["commands"])
        if kind=="sdk-compiler-model":
            require(evidence.get("evidence_scope")=="sdk-compiler-model-supplement" and
                evidence.get("supplemental_scope")=="sdk-complete-matrix" and
                evidence.get("actual_host_sdk_checks")=="EXECUTED" and
                evidence.get("actual_ROS_delivery")=="NOT_EXECUTED" and
                evidence.get("actual_ARM_Runtime_execution")=="NOT_EXECUTED" and architecture is not None,
                "SDK compiler/model scope cannot claim native ROS or ARM Runtime delivery")
            result.update(actual_execution=False,actual_host_execution=True,actual_ROS_delivery="NOT_EXECUTED",
                actual_ARM_Runtime_execution="NOT_EXECUTED",supplemental_scope="sdk-complete-matrix",
                evidence_scope="sdk-compiler-model-supplement")
        elif kind=="compiler":
            products=[select(raw,path) for path in specification["ARM_product_selectors"]]
            require(products,"actual compiler output association required")
            for product in products:require(require_arm(product["path"])==product,"compiler's original ARM product changed")
            result.update(ARM_products=products,actual_Runtime_execution=False)
        else:
            labels=specification.get("mapped_ARM_labels",[])
            if "mapped_launches" in specification:
                launches=select(evidence,specification["mapped_launches"])
                require(isinstance(launches,list) and launches,"actual original launch inventory missing")
                labels=[]
                for launch in launches:
                    selected=launch.get("required",[])
                    require(selected and type(launch.get("pid")) is int and launch["pid"]>0,"original mapped launch identity missing")
                    matched={target for sample in architecture["actual_process_samples"]
                        if launch["pid"] in sample.get("ancestry",[]) and sample.get("live_cgroup_procs")
                        for target in sample.get("mapped_arm_targets",[])}
                    require(set(selected)<=matched,"original launch lacks its actual ARM mapping")
                    labels.extend(selected)
                labels=sorted(set(labels))
            require(isinstance(labels,list) and labels,"explicit actual ARM image association required")
            targets=architecture["arm_targets"];observed={label for sample in architecture["actual_process_samples"]
                if sample.get("live_cgroup_procs") and sample.get("membership")==architecture["cgroup_before"]["membership"]
                for label in sample.get("mapped_arm_targets",[])}
            require(set(labels)<=set(targets) and set(labels)<=observed,"actual selected ARM mapping missing")
            result["mapped_ARM_labels"]=labels
    if "side_effect" in specification:
        measured=select(raw,specification["side_effect"]);side_effect(measured)
        for key in ("expected_errno","actual_errno","output_sentinels_unchanged","native_observer_independent",
                    "native_effects_before","native_effects_after","observer_kind","quiet_samples","quiet_ns"):
            result[key]=measured[key]
    if "native_witnesses" in specification:
        # This is the additive current Runner receipt, not a historical
        # inference from a witness-mode string. Counter observers finish;
        # persistent positive sources retain actual post-Guest liveness.
        witnesses=select(evidence,specification["native_witnesses"])
        require(isinstance(witnesses,list) and witnesses,"actual native witnesses missing")
        bindings=[]
        for witness in witnesses:
            terminated=type(witness.get("returncode")) is int and witness["returncode"]==0
            live=witness.get("returncode") is None and witness.get("observed_alive_after_guest") is True
            require((terminated or live) and type(witness.get("pid")) is int and witness["pid"]>0,"native witness outcome missing")
            old=witness["log"]
            association=None
            if witness_log_bindings is not None and any(Path(old["path"]).is_relative_to(root) for root in RUN_ALL_ALIASES|RPC_ALIASES):
                association=witness_log_bindings.get(old["path"])
                require(isinstance(association, dict) and association["alias_path"]==old["path"]
                    and all(association[name][key]==old[key] for name in ("alias_log", "physical_log") for key in ("sha256", "bytes")),
                    "original alias witness log has no exact captured binding")
                now=lane.pin(association["physical_log"]["path"])
                require(now==association["physical_log"], "captured physical witness log identity changed")
            else:
                now=lane.pin(old["path"])
            require(now["sha256"]==old["sha256"] and now["bytes"]==old["bytes"],"native witness log changed")
            normalized={**witness,"log":now}
            if association is not None:
                normalized.update(original_alias_log=old,original_alias_association=association)
            bindings.append(normalized)
        if specification.get("native_terminal_required") is True:
            require(all(type(row["returncode"]) is int and row["returncode"]==0 for row in bindings),"native absence/count witness must finish")
        result.update(native_witnesses=bindings,independent_native_verified=True)
    if "native_counter_assertions" in specification:
        require(specification["native_counter_assertions"] and architecture is not None,"actual native counter scope missing")
        for assertion in specification["native_counter_assertions"]:fact(select(evidence,assertion["path"]),assertion)
        result.update(independent_native_verified=True,native_counter_assertions=specification["native_counter_assertions"])
    if "security_contract" in specification:
        contract=specification["security_contract"]
        require(contract["kind"] in ("guest-refusal-and-native-assertions","startup-refusal","bounded-allowed-counts",
            "retained-memory-growth","owned-lifetime","native-concurrency") and contract["scope"] and contract["assertions"],
            "closed security assertion scope required")
        for assertion in contract["assertions"]:fact(select(evidence,assertion["path"]),assertion)
        require(kind in ("runtime","native") and architecture is not None,"security needs actual mapped execution")
        result["security_contract"]={**contract,"actual_assertions_verified":True,
            "individual_import_returnvalues":"NOT_RECORDED unless explicitly present in selected raw fields"}
    if kind=="bounded-rejection":
        require(result.get("expected_errno")==95 and result.get("actual_errno")==95 and result.get("native_observer_independent") is True
            and type(specification.get("unsupported_reason")) is str and specification["unsupported_reason"],"actual bounded95 rejection/native effect/reason required")
        result.update(status="QUALIFIED_SUPPORTED_SCOPE",unsupported_reason=specification["unsupported_reason"],full_native_feature_supported=False)
    # Example metadata is copied only from a pinned generated material object;
    # its Guest/native terminal outcomes remain separate required actual facts.
    if "example_material" in specification:
        material,pin=lane.json(specification["example_material"])
        for key in ("guest_source","compile_argv","runtime_argv","grants","limits","expected_output","native_test"):
            require(key in material and material[key],"example seven-part material field missing");result[key]=material[key]
        native_verified=result.get("independent_native_verified")
        if "independent_native_verified" in specification:native_verified=select(raw,specification["independent_native_verified"])
        result.update(example_material_pin=pin,Guest_exit=select(raw,specification["Guest_exit"]),independent_native_verified=native_verified)
        require(type(result["Guest_exit"]) is int and result["Guest_exit"]==0 and result["independent_native_verified"] is True,"actual example execution missing")
    return result


def documents(lane,raw,entries):
    """Load exact current nested reports without modifying legacy schemas.

    A run_all suite has a physical evidence pin, not an architecture object
    embedded in its parent. Resolve that pin explicitly. Unpinned paths are
    allowed only inside this job's fresh raw output and are labelled as a
    present binding; no missing historical SHA is invented. Dependencies must
    precede their references, and every JSON object is bounded by Lane.json.
    """
    values={"raw":raw};bindings={}
    for entry in entries:
        name=entry["id"]
        require(type(name) is str and name not in values and entry["from"] in values,"ordered unique raw documents required")
        parent=values[entry["from"]]
        if "reference" in entry:
            old=select(parent,entry["reference"])
            require(isinstance(old,dict) and set(("path","sha256"))<=set(old),"nested report physical pin missing")
            value,pin=lane.json(old["path"])
            require(pin["sha256"]==old["sha256"] and ("bytes" not in old or pin["bytes"]==old["bytes"]),"nested receipt changed")
            historical="Original parent physical pin"
        else:
            path=Path(entry["path"])
            require(path.is_absolute() and path.is_relative_to(lane.out/"raw"),"unpinned nested path escapes fresh raw output")
            value,pin=lane.json(path);historical="Original parent SHA NOT_RECORDED; fresh owned raw bytes bound now"
        values[name]=value;bindings[name]={"pin":pin,"scope":historical}
    return values,bindings


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser,workers=False)
    parser.add_argument("--recipe",type=Path,required=True);args=parser.parse_args();lane=None;status="FAIL"
    try:
        lane=Lane(args,"acceptance-normalized");recipe,recipe_pin=lane.json(args.recipe)
        require(recipe.get("format_version")==1 and type(recipe.get("format_version")) is int and recipe.get("kind")=="ros-legacy-bound-recipe"
            and recipe.get("source_manifest_sha256")==lane.report["source_manifest_sha256"] and recipe["driver"] in DRIVERS
            and recipe["driver"] in lane.frozen,"closed current frozen recipe required")
        argv=recipe["arguments"];require(isinstance(argv,list) and all(type(arg) is str and "\0" not in arg for arg in argv),"closed original driver argv required")
        require(argv.count("--source")==1 and argv[argv.index("--source")+1]==str(lane.source),"legacy source argv differs")
        # Original helpers without --manifest retain their parser unchanged.
        # The wrapper always binds its full current manifest plus the raw
        # report's actual source-manifest field after that helper runs.
        if "--manifest" in argv:require(argv.count("--manifest")==1 and argv[argv.index("--manifest")+1]==str(args.manifest.resolve()),"legacy explicit manifest differs")
        require(argv.count("--out")==1 and Path(argv[argv.index("--out")+1]).is_relative_to(lane.out/"raw"),"legacy output escapes dedicated raw directory")
        child,command=lane.start("original-driver",["/usr/bin/unshare","-m","--",sys.executable,"-B",str(lane.source/recipe["driver"]),*argv],
            seconds=recipe["timeout_seconds"],rss=recipe["rss_bytes"]);lane.finish(child,command)
        raw,raw_pin=lane.json(recipe["raw_report"])
        fact(select(raw,recipe["raw_success_path"]),{"operator":"eq","expected":recipe["raw_success_value"]})
        association=recipe["source_association"]
        if association["kind"]=="raw-manifest":
            require(select(raw,association["path"])==lane.report["source_manifest_sha256"],"original receipt came from another source freeze")
            source_binding={"scope":"Original explicit manifest field","raw_selector":association["path"]}
        elif association["kind"]=="qualified-worker":
            # Some historical CLI helpers record exact Worker/driver hashes,
            # but have no manifest field. Keep that absence visible and bind
            # their actual Worker to the current full compile dependency report.
            worker,worker_pin=lane.json(association["report"])
            require(worker["status"]=="PASS" and worker["source_manifest_sha256"]==lane.report["source_manifest_sha256"],"current qualified Worker source association missing")
            bindings=validate_fresh_worker_inputs(worker,lane.source,lane.frozen)
            product=select(worker,association["product"])
            require(require_arm(product["path"])==product and select(raw,association["raw_worker_sha256"])==product["sha256"],"original actual Worker differs from current qualification")
            source_binding={"scope":"Original manifest field NOT_RECORDED; current original input Worker byte/full-MD binding reconstructed",
                            "report":worker_pin,"product":product,"full_dependencies":bindings}
        else:raise ValueError("unrecognized original source association")
        witness_log_bindings=None
        if recipe["driver"]=="test/0073.ros-tests/support/source/ros_aarch64/run_all.py":
            require(association["kind"]=="raw-manifest", "run_all alias captures require its original explicit source manifest")
            witness_log_bindings=run_all_witness_log_bindings(lane,raw,argv)
            source_binding["workspace_alias_binding"]=raw["workspace_alias_binding"]
            source_binding["native_witness_logs_preserved"]=True
        roots,document_pins=documents(lane,raw,recipe.get("documents",[]))
        if all(item["kind"] in ("material","compiler") for item in recipe["records"]):
            architecture=None;architecture_pin=None
        else:
            architecture,architecture_pin=architecture_binding(recipe["raw_architecture"],lane.pins)
            require(architecture["cgroup_incarnation_before"]==lane.before,"original driver cgroup/boot differs")
        require(recipe["records"] and len({row["id"] for row in recipe["records"]})==len(recipe["records"]),"closed unique original records required")
        records=[];record_architectures={}
        for specification in recipe["records"]:
            root=roots[specification.get("document","raw")];observed=architecture
            if "architecture_document" in specification:
                name=specification["architecture_document"]
                require(name in document_pins,"selected architecture document missing")
                observed,pin=architecture_binding(document_pins[name]["pin"]["path"],lane.pins)
                require(pin["sha256"]==document_pins[name]["pin"]["sha256"] and
                    observed["cgroup_incarnation_before"]==lane.before,"selected architecture source/incarnation differs")
                record_architectures[name]=pin
            item=record(lane,root,specification,observed,witness_log_bindings)
            if "source_program" in specification:
                program=specification["source_program"];pin=lane.pin(program["path"])
                require(select(roots[program["document"]],program["sha256_selector"])==pin["sha256"],"full current original program bytes differ")
                item["current_full_program"]=pin
            if specification.get("deliver_receipt") is True:
                selected_pin=raw_pin if specification.get("document","raw")=="raw" else document_pins[specification["document"]]["pin"]
                item.update(materials_verified=True,physical_material_pins=[selected_pin])
            records.append(item)
        lane.report["acceptance_records"]=records
        if all(item["kind"]=="sdk-compiler-model" for item in records):
            require(recipe["driver"]=="test/0073.ros-tests/support/source/ros_acceptance/run_sdk_matrix.py","SDK supplemental kind requires the exact closed adapter")
            lane.report.update(evidence_scope="sdk-compiler-model-supplement",supplemental_scope="sdk-complete-matrix",
                actual_ROS_delivery="NOT_EXECUTED",actual_ARM_Runtime_execution="NOT_EXECUTED")
        lane.report.update(recipe=recipe_pin,raw_report=raw_pin,raw_architecture=architecture_pin,raw_argv=argv,source_association=source_binding,
            nested_documents=document_pins,record_architectures=record_architectures,
            original_reports_modified=False,new_tests_executed=True);status="PASS"
    except BaseException as error:
        if lane is not None:lane.report.update(error=repr(error),traceback=traceback.format_exc())
        else:raise
    finally:
        if lane is not None:status=lane.close(status)
    return 0 if status=="PASS" else 1


if __name__=="__main__":raise SystemExit(main())
