#!/usr/bin/env python3
"""Write the concrete same-SourceB selection; do not launch any workload.

The caller supplies physical inputs from the current preparation, including
both composed-provider Gateways, all four Workers, and their full dependency
receipts. This file fixes the original suite names, nested report selectors,
case identities and requirement mappings. It does not copy historical status
or select a first matching report. The output remains PREPARED_NOT_EXECUTED.

Run this only after freeze_source.py has created complete SourceB containing
the fixed-v1 plus bounded-v2 provider and exact official derivation inputs.
The subsequent make_manifest.py and run.py are separate, serial operations.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys

sys.dont_write_bytecode=True
from protocol import require, load_bound
from requirements import inventories


def key(name):return {"key":name}
def where(name):return {"where":"name","equals":name}
def suite(name):return [key("suites"),{"where":"suite","equals":name}]
def eq(path,value):return {"path":path,"operator":"eq","expected":value}
def count(path,value):return {"path":path,"operator":"length","expected":value}
def inp(name):return {"input":name}
def out(name):return {"output":name}
def produced(name,path):return {"product_of":name,"path":path}


COMMON_INPUTS=("sdk-workspace","qemu","binfmt-evidence","container-started-at",
    "worker","worker-report","worker-config","ros1-gateway","ros1-gateway-report",
    "ros2-gateway","ros2-gateway-report","wat2wasm","prepared-sdk","prepared-recovery",
    "prepared-metrics","prepared-observer","delivery-products","node")
LEGACY_INPUTS=("workspace","gateway-build","prepared-resources","prepared-action-qos",
    "prepared-ros1-search","prepared-ros2-local-parameter-events","source-addon","source-addon-manifest",
    "prepared-native-adoption","prepared-enforce","prepared-reconnect","ros1-rootfs","ros2-rootfs",
    "native-products","example-socket-dir","example-worker-pid","worker-uid","gateway-uid","expected-cgroup",
    "prepared-bounded-service","prepared-events","prepared-custom-bounded","custom-products",
    "cross-sdk-manifest","compiler-resource-root","sdk-matrix-tool-config")


def job(name,driver,arguments,records,*,requires=(),report="report.json",architecture="execution/architecture-evidence.json",
        success="PASS",documents=(),seconds=7080):
    return {"id":name,"driver":driver,"arguments":arguments,"requires":sorted(requires),"timeout_seconds":seconds,
        "rss_bytes":8*1024**3,"raw_report":out(report),"raw_architecture":out(architecture) if architecture else None,
        "raw_success_path":[key("status")],"raw_success_value":success,
        "source_association":{"kind":"raw-manifest","path":[key("source_manifest_sha256")]},
        "documents":list(documents),"records":records}


def common():
    return ["--source",inp("source"),"--manifest",inp("manifest"),"--sdk-workspace",inp("sdk-workspace"),
        "--qemu",inp("qemu"),"--binfmt-evidence",inp("binfmt-evidence"),"--container-started-at",inp("container-started-at"),"--out",out(".")]


def products():
    return [item for name in ("worker","worker-report","worker-config","ros1-gateway","ros1-gateway-report",
        "ros2-gateway","ros2-gateway-report","wat2wasm") for item in ("--"+name,inp(name))]


def base_record(name,document,*,path=(),facts=(),labels=("uwvm",),kind="runtime",architecture=None):
    value={"id":name,"kind":kind,"document":document,"path":list(path),
        "facts":[eq([key("status")],"PASS"),*facts],"commands":[{"driver_exit":True}],"mapped_ARM_labels":list(labels)}
    if architecture is not None:value["architecture_document"]=architecture
    return value


def legacy_case(name,document,*,family="network",version=None,extra=(),security=None):
    collection="results" if family in ("network","rpc") else "original_runner_results"
    versions=(1,2) if version is None else (version,)
    labels=["uwvm",*["ros"+str(v)+"/uwvm-ros"+str(v)+"-gateway" for v in versions]]
    value=base_record(name,document,path=[key(collection),where(name)],facts=[eq([key("wasm_exit")],0),*extra],labels=labels,architecture=document)
    if security:
        value["security_contract"]={"kind":security,"scope":"Exact source-bound Guest/native assertions in the selected actual legacy case; no unrecorded numeric errno is invented",
            "assertions":[eq([key("wasm_exit")],0),*extra]}
    return value


NETWORK_CASES=("cross-module-proxy","cross-module-tail-proxy","cross-module-handle","cross-backend-handle",
    "ros1-wasm-publish","ros1-wasm-subscribe","ros2-wasm-publish","ros2-wasm-subscribe",
    "hybrid-ros1-to-ros2","hybrid-ros2-to-ros1",
    "gateway-disconnect-independent","gateway-disconnect-recovery","gateway2-disconnect-independent","gateway2-disconnect-recovery")


def legacy_records():
    """Concrete existing schemas: suite physical pins then exact original names."""
    records=[];documents=[]
    names=("native","cli","multi-memory","network","rpc","five-module","additional-security","ingress-trap",
        "revoke-race","flood","payload-security","performance","current-resources","action-qos","wasip1-default-deny",
        "original-units","ros1-search-contracts","ros1-search","ros2-local-parameter-events",
        "regression-none","regression-ros1","regression-ros2","regression-dual")
    for name in names:
        documents.append({"id":name,"from":"raw","reference":[*suite(name),key("evidence")]})
    # RPC's optional denial-retention wrapper exposes the unchanged adapter
    # architecture through its actual byte pin, not through a guessed timestamp.
    documents.append({"id":"rpc-architecture","from":"rpc","reference":[key("adapter_evidence")]})
    documents.append({"id":"cli-cases","from":"cli","path":out("cli/cli-results.json")})
    for name in names:
        if name in ("cli","rpc","current-resources","action-qos","wasip1-default-deny","original-units",
                    "ros1-search-contracts","ros1-search","ros2-local-parameter-events"):continue
        value=base_record("suite-"+name,name,labels=("uwvm",),architecture=name)
        if name=="native" or name.startswith("regression-"):value["mapped_launches"]=[key("launches")]
        if name=="native":
            value["security_contract"]={"kind":"bounded-allowed-counts",
                "scope":"Actual complete source-bound native probes include twenty real network records per legacy overflow policy and assert exact retained frames; individual numeric counters are not separately recorded by this legacy report",
                "assertions":[eq([key("status")],"PASS")]}
        if name in ("flood","revoke-race"):
            value["security_contract"]={"kind":"native-concurrency" if name=="revoke-race" else "bounded-allowed-counts",
                "scope":"Actual complete source-bound original native flood/race driver; assertions and individual counters stay in retained original rows/logs",
                "assertions":[eq([key("extended_mapping_failures")],[])]}
        value["deliver_receipt"]=True;records.append(value)
    for name in ("current-resources","action-qos","ros1-search","ros2-local-parameter-events"):
        documents.append({"id":name+"-architecture","from":name,"reference":[key("architecture_evidence")]})
        records.append(base_record("suite-"+name,name,labels=("ros2/uwvm-ros2-gateway",) if name=="current-resources" else
            ("ros1-parent-search",) if name=="ros1-search" else ("local-parameter-events-ipc",) if name=="ros2-local-parameter-events" else ("action-qos-native",),architecture=name+"-architecture"))
    # The current run_all front door separately qualifies these receipts and
    # exact suite counts. Select its retained checks rather than calling these
    # C++ unit processes a Wasm execution.
    unit_labels={"wasip1-default-deny":("wasip1-configuration","wasip1-authority32","wasip1-authority64"),
        "original-units":("security","ipc","ipc-client-deadline","sorted-rows","sdk","sorted-rows-cpp17"),
        "ros1-search-contracts":("ros1-search-worker-contract","ros1-search-gateway-contract")}
    for name,labels in unit_labels.items():
        documents.append({"id":name+"-architecture","from":name,"reference":[key("architecture_evidence")]})
        records.append(base_record("suite-"+name,name,facts=[count([key("suites")],17 if name=="wasip1-default-deny" else len(labels))],kind="native",
            labels=labels,architecture=name+"-architecture"))
    cli=base_record("mandatory-cli","cli-cases",architecture="cli",labels=("worker",))
    cli["complete_rows"]=[key("cases")]
    records.append(cli)
    records.append(base_record("suite-rpc","rpc-architecture",labels=("uwvm",),architecture="rpc-architecture"))
    for name in NETWORK_CASES:
        security="owned-lifetime" if "disconnect" in name else "guest-refusal-and-native-assertions" if name.startswith("cross-") else None
        value=legacy_case(name,"network",security=security)
        if name not in ("cross-module-proxy","cross-module-tail-proxy","cross-module-handle","cross-backend-handle"):
            value["native_witnesses"]=[key("native_witnesses")]
        records.append(value)
    for version in (1,2):
        prefix="ros"+str(version)
        for suffix in ("topic_denied","service_denied","graph_denied","action_denied","handle_security","memory64_bounds","memory64_growth","subscribe-denied","cancel-denied"):
            kind="retained-memory-growth" if suffix=="memory64_growth" else "guest-refusal-and-native-assertions"
            records.append(legacy_case(prefix+"-"+suffix,"network",version=version,security=kind))
        for suffix in ("publish-flood","action-flood"):
            records.append(legacy_case(prefix+"-"+suffix,"network",version=version,security="bounded-allowed-counts"))
        for suffix in ("wasm-service-client-trigger","wasm-service-server-trigger","wasm-action-client","wasm-action-server"):
            value=legacy_case(prefix+"-"+suffix,"network",version=version);value["native_witnesses"]=[key("native_witnesses")];records.append(value)
        for suffix in ("parameter_set","tf_lookup","parameter_get","parameter_list","graph_list_nodes","graph_list_topics","log_write"):
            value=legacy_case(prefix+"-"+suffix,"rpc-architecture",family="rpc",version=version)
            if suffix in ("parameter_set","tf_lookup"):value["native_witnesses"]=[key("native_witnesses")]
            records.append(value)
        for suffix in ("parameter_set_denied","tf_broadcast_denied"):
            records.append(legacy_case(prefix+"-"+suffix,"rpc-architecture",family="rpc",version=version,security="guest-refusal-and-native-assertions"))
        for suffix in ("topic_subscribe_denied","parameter_get_denied","tf_lookup_denied","action_cancel_denied"):
            records.append(legacy_case(prefix+"-"+suffix,"additional-security",family="extended",version=version,
                extra=[eq([key("expected_error")],13),eq([key("output_sentinels_unchanged")],True)],security="guest-refusal-and-native-assertions"))
        for suffix in ("normalized-resource","relative-resource","private-resource","namespace-bypass","remapping-resource",
                "overlong-resource","wrong-ros-type","cross-version-type","forged-module-name","multiple-module-groups",
                "wrong-md5" if version==1 else "wrong-typesupport"):
            value=base_record(prefix+"-startup-"+suffix,"additional-security",path=[key("original_runner_results"),where(prefix+"-startup-"+suffix)],
                facts=[eq([key("expected_exit")],126),eq([key("wasm_exit")],126),eq([key("guest_start_entered")],False)],
                labels=("uwvm","ros"+str(version)+"/uwvm-ros"+str(version)+"-native-probe"),architecture="additional-security")
            value["security_contract"]={"kind":"startup-refusal","scope":"Actual pre-start126 plus native absence; Guest entry sentinel201 was not executed",
                "assertions":[eq([key("wasm_exit")],126),eq([key("guest_start_entered")],False)]};records.append(value)
        for suffix,error in (("malformed-length",22),("invalid-utf8",22),("noncanonical-bool",22),("truncated-fields",22),
            ("nan",22),("infinity",22),("explicit-bound",13),("malformed-image-length",22),("oversized",90)):
            value=legacy_case(prefix+"-payload-"+suffix,"payload-security",family="extended",version=version,
                extra=[eq([key("expected_error")],error)],security="guest-refusal-and-native-assertions")
            value["native_witnesses"]=[key("native_witnesses")];value["native_terminal_required"]=True;records.append(value)
        for suffix in ("service","graph","log"):
            records.append(legacy_case(prefix+"-"+suffix+"-flood","flood",family="extended",version=version,
                extra=[eq([key("requests")],100),eq([key("allowed")],2),eq([key("denied")],98),eq([key("expected_error")],11)],security="bounded-allowed-counts"))
        for suffix in ("socket-standard-input","socket-preopen","socket-single-preopen","socket-group-preopen",
                "missing-environment-isolation","directory-preopen","native-plugin-ingress"):
            value=base_record(prefix+"-"+suffix,"ingress-trap",path=[key("original_runner_results"),where(prefix+"-"+suffix)],
                facts=[eq([key("wasm_exit")],126),eq([key("native_ros_publisher_count")],0),eq([key("native_ros_message_count")],0)],
                labels=("uwvm","ros"+str(version)+"/uwvm-ros"+str(version)+"-native-probe"),architecture="ingress-trap")
            value["security_contract"]={"kind":"startup-refusal","scope":"Actual strict-ingress126 with native publisher/message counters zero",
                "assertions":[eq([key("wasm_exit")],126),eq([key("native_ros_message_count")],0)]};records.append(value)
        value=base_record(prefix+"-wasm-trap-resource-cleanup","ingress-trap",path=[key("original_runner_results"),where(prefix+"-wasm-trap-resource-cleanup")],
            facts=[eq([key("wasm_exit")],-4),eq([key("native_after_trap"),key("publishers")],0),eq([key("native_after_trap"),key("subscribers")],0),
                eq([key("native_after_trap"),key("services")],0),eq([key("native_after_trap"),key("consecutive_graph_samples")],10)],
            labels=("uwvm","ros"+str(version)+"/uwvm-ros"+str(version)+"-native-probe"),architecture="ingress-trap")
        value["security_contract"]={"kind":"owned-lifetime","scope":"Actual Trap and ten real native empty-resource samples before test termination",
            "assertions":value["facts"][1:]};records.append(value)
    for name in ("global-group-no-inheritance","backend-group-confusion","wasi-group-confusion"):
        records.append(legacy_case(name,"additional-security",family="extended",extra=[eq([key("expected_error")],13)],security="guest-refusal-and-native-assertions"))
    five=legacy_case("required-five-module-group-single-hybrid","five-module",family="extended",
        extra=[count([key("actual_host_instance_ids")],5),count([key("camera_authenticated_sessions")],2),count([key("actual_native_message_counts")],7),
            eq([key("simultaneous_wasi_ros_groups"),key("wasi")],"basic_io"),eq([key("simultaneous_wasi_ros_groups"),key("ros1")],"legacy_sensors"),
            eq([key("simultaneous_wasi_ros_groups"),key("ros2")],"modern_planners"),eq([key("simultaneous_wasi_ros_groups"),key("wasi_environment_inherited")],False)])
    # The existing native five-module report independently counted every
    # concrete topic/text projection; it is not merely a witness mode string.
    five["native_counter_assertions"]=[eq([key("actual_native_message_counts"),key(topic),key(text),key("count")],2)
        for topic,text in (("/five/camera_a","camera-a"),("/five/camera_b","camera-b"),("/five/controller","controller"),
            ("/five/hybrid1","hybrid-ros1"),("/five/shared_cameras","camera-a"),("/five/shared_cameras","camera-b"),
            ("/five/hybrid2","hybrid-ros2"),("/five/planner","planner"))]
    five["deliver_receipt"]=True;records.append(five)
    for record in records:record["deliver_receipt"]=True
    return records,documents


def original_mapping():
    """Every numbered reference is explicit; missing records are fatal later."""
    legacy=lambda name:{"launch":"original-all","record":name}
    ref=lambda launch,name:{"launch":launch,"record":name}
    mapping={name:{} for name in ("chapters","phases","acceptance","security","examples","outputs")}
    a={1:[ref("workers","worker-none")],2:[ref("workers","worker-ros1")],3:[ref("workers","worker-ros2")],4:[ref("workers","worker-dual")],
        5:[legacy("mandatory-cli")],6:[legacy("mandatory-cli")],7:[legacy("mandatory-cli"),legacy("required-five-module-group-single-hybrid")],
        8:[legacy("mandatory-cli"),legacy("required-five-module-group-single-hybrid")],9:[legacy("mandatory-cli")],10:[legacy("mandatory-cli")],
        11:[legacy("global-group-no-inheritance")],12:[legacy("mandatory-cli")],13:[legacy("mandatory-cli")],
        14:[legacy("cross-module-handle"),legacy("required-five-module-group-single-hybrid")],15:[legacy("cross-backend-handle")],16:[legacy("cross-backend-handle")],
        17:[legacy("cross-module-handle")],18:[legacy("ros1-wasm-publish")],19:[legacy("ros1-wasm-subscribe")],20:[legacy("ros2-wasm-publish")],21:[legacy("ros2-wasm-subscribe")],
        22:[legacy("ros1-wasm-service-client-trigger"),legacy("ros1-wasm-service-server-trigger"),ref("bounded-service","bounded-service")],
        23:[legacy("ros2-wasm-service-client-trigger"),legacy("ros2-wasm-service-server-trigger")],24:[legacy("ros1-parameter_set"),legacy("suite-ros1-search")],
        25:[legacy("ros2-parameter_set"),legacy("suite-current-resources")],26:[legacy("ros1-wasm-action-client"),legacy("ros1-wasm-action-server")],
        27:[legacy("ros2-wasm-action-client"),legacy("ros2-wasm-action-server")],28:[legacy("ros1-tf_lookup")],29:[legacy("ros2-tf_lookup")],
        30:[ref("native-adoption","native-adoption")],31:[legacy("required-five-module-group-single-hybrid")],32:[legacy("hybrid-ros1-to-ros2")],
        33:[legacy("hybrid-ros1-to-ros2"),ref("memory1-dual","memory1-dual")],
        34:[legacy("ros1-topic_denied"),legacy("ros2-topic_denied"),ref("graph-query-flood-ros1","graph-query-flood-ros1"),ref("graph-query-flood-ros2","graph-query-flood-ros2")],
        35:[legacy("ros1-socket-preopen"),legacy("ros2-socket-preopen")],36:[legacy("suite-native"),ref("raw-frame-ros1","raw-frame-ros1"),ref("wrong-uid-ros2","wrong-uid-ros2")],
        37:[ref("tcpros-peer-restart","tcpros-peer-restart"),ref("dds-peer-restart","dds-peer-restart")],38:[legacy("ros1-wasm-trap-resource-cleanup"),legacy("ros2-wasm-trap-resource-cleanup")],
        39:[legacy("suite-native"),legacy("suite-network")],40:[legacy("suite-network")],41:[legacy("suite-network")],42:[legacy("suite-wasip1-default-deny"),legacy("suite-regression-dual")],
        43:[legacy("suite-regression-dual")],44:[legacy("suite-regression-none"),legacy("suite-regression-ros1"),legacy("suite-regression-ros2"),legacy("suite-regression-dual")],
        45:[legacy("suite-network"),legacy("suite-rpc")],46:[ref("materials","documentation")],47:[ref("materials","documentation")],48:[ref("materials","documentation")],
        49:[legacy("required-five-module-group-single-hybrid"),ref("materials","documentation")],50:[legacy("hybrid-ros1-to-ros2"),ref("materials","documentation")]}
    for index,values in a.items():
        mapping["acceptance"][str(index)]=[{**value,**({"evidence_role":"material"} if value["launch"]=="materials" else {})} for value in values]
    # The preserved four Worker-profile runtime regressions stay required.
    # The new aggregate separately reopens every current native chunk and
    # requires all3073 original interpreter configurations exactly once.
    for index in (42,43,44):mapping["acceptance"][str(index)].append(ref("original-regressions-full","original-regressions-full"))
    # Pair both backends for every operation family. Graph/read-rate, kernel
    # identity and raw frames use the new independent native count fixtures.
    attack={1:("network","topic_denied"),2:("additional-security","topic_subscribe_denied"),3:("network","service_denied"),
        4:("additional-security","parameter_get_denied"),5:("rpc","parameter_set_denied"),6:("network","action_denied"),7:("additional-security","action_cancel_denied"),
        8:("additional-security","tf_lookup_denied"),9:("rpc","tf_broadcast_denied"),11:("network","handle_security"),12:("network","handle_security"),
        16:("additional-security","startup-forged-module-name"),17:("additional-security","startup-multiple-module-groups"),21:("additional-security","startup-normalized-resource"),
        22:("additional-security","startup-relative-resource"),23:("additional-security","startup-private-resource"),24:("additional-security","startup-namespace-bypass"),
        25:("additional-security","startup-remapping-resource"),27:("additional-security","startup-wrong-ros-type"),29:("payload","payload-malformed-length"),
        30:("payload","payload-noncanonical-bool"),31:("payload","payload-nan"),32:("payload","payload-infinity"),33:("payload","payload-oversized"),
        34:("additional-security","startup-overlong-resource"),35:("network","memory64_bounds"),36:("network","memory64_bounds"),37:("network","memory64_bounds"),38:("network","memory64_growth"),
        39:("network","publish-flood"),40:("flood","service-flood"),41:("network","action-flood"),43:("flood","log-flood"),48:("ingress-trap","wasm-trap-resource-cleanup"),
        51:("ingress-trap","socket-preopen"),52:("ingress-trap","socket-group-preopen"),53:("ingress-trap","native-plugin-ingress")}
    for index,(_,suffix) in attack.items():mapping["security"][str(index)]=[legacy("ros"+str(v)+"-"+suffix) for v in (1,2)]
    for index,names in {13:("cross-module-handle",),14:("cross-backend-handle",),15:("cross-backend-handle",),18:("backend-group-confusion",),19:("wasi-group-confusion",),
        20:("cross-module-proxy","cross-module-tail-proxy"),26:("ros1-startup-wrong-md5",),28:("ros2-startup-wrong-typesupport",),
        45:("gateway-disconnect-independent","gateway2-disconnect-independent"),46:("gateway-disconnect-recovery","gateway2-disconnect-recovery")}.items():
        mapping["security"][str(index)]=[legacy(name) for name in names]
    mapping["security"]["10"]=[legacy("ros"+str(v)+"-graph_denied") for v in (1,2)]+[
        ref("graph-query-flood-ros"+str(v),"graph-query-flood-ros"+str(v)+"-denial") for v in (1,2)]
    mapping["security"]["44"]=[ref("graph-query-flood-ros"+str(v),"graph-query-flood-ros"+str(v)) for v in (1,2)]
    # The original native topic probes publish twenty real TCPROS/DDS records
    # for each legacy overflow policy and assert the exact retained frames.
    # Queue units supplement this network experiment; they cannot replace it.
    mapping["security"]["42"]=[legacy("suite-native"),legacy("suite-flood"),ref("unified-contracts","unified-contracts")]
    # Callback teardown and lifetime stress also require the original actual
    # native/dual-Guest revoke races, whose independently observed counters are
    # frozen after revocation. Keep the existing flood and queue contracts.
    for index in (49,50):mapping["security"][str(index)]=[legacy("suite-revoke-race"),legacy("suite-flood"),ref("unified-contracts","unified-contracts")]
    mapping["security"]["47"]=[ref("reconnect","reconnect"),legacy("suite-revoke-race")]
    mapping["security"]["54"]=[ref(kind+"-ros"+str(v),kind+"-ros"+str(v)) for kind in ("raw-frame","wrong-uid") for v in (1,2)]
    mapping["security"]["55"]=[legacy("suite-revoke-race"),ref("unified-contracts","unified-contracts")]
    example_names=("ros1-wasm-publish","ros1-wasm-subscribe","ros2-wasm-publish","ros2-wasm-subscribe",
        "ros1-wasm-service-client-trigger","ros1-wasm-service-server-trigger","ros2-wasm-service-client-trigger","ros2-wasm-service-server-trigger",
        "ros1-parameter_set","ros2-parameter_set","ros1-wasm-action-client","ros2-wasm-action-client","ros1-tf_lookup","ros2-tf_lookup",
        "required-five-module-group-single-hybrid","required-five-module-group-single-hybrid","hybrid-ros1-to-ros2","ros1-topic_denied","ros2-topic_denied","cross-module-handle")
    for index,name in enumerate(example_names,1):mapping["examples"][str(index)]=[legacy("example-"+str(index))]
    chapter_rows={1:("mandatory-cli",),2:("suite-network",),3:("suite-regression-dual",),4:("suite-network",),5:("mandatory-cli",),6:("mandatory-cli",),
        7:("required-five-module-group-single-hybrid",),8:("cross-module-handle",),9:("cross-module-proxy",),10:("suite-native",),11:("suite-network",),
        12:("ros1-wasm-publish","ros2-wasm-subscribe"),13:("suite-network",),14:("ros1-wasm-service-client-trigger","ros2-wasm-service-server-trigger"),
        15:("suite-rpc",),16:("ros1-parameter_set","ros2-parameter_set"),17:("ros1-wasm-action-client","ros2-wasm-action-server"),18:("suite-network",),
        19:("suite-network",),20:("suite-rpc",),21:("suite-flood",),22:("suite-ingress-trap",),23:("required-five-module-group-single-hybrid",),24:("suite-regression-dual",),
        25:("suite-payload-security",),26:("suite-network",),27:("suite-network",),28:("suite-additional-security",),29:("suite-performance",),30:("suite-regression-dual",),
        31:("suite-network",),32:("required-five-module-group-single-hybrid",),33:("suite-network",),34:("suite-network",),35:("suite-network",)}
    for index,names in chapter_rows.items():mapping["chapters"][str(index)]=[legacy(name) for name in names]
    for index,names in {1:("mandatory-cli",),2:("cross-module-handle",),3:("suite-native",),4:("suite-network",),5:("suite-rpc",),6:("suite-network",),
        7:("ros1-wasm-action-client","ros2-wasm-action-client"),8:("ros1-tf_lookup","ros2-tf_lookup"),9:("required-five-module-group-single-hybrid",),
        10:("suite-additional-security",),11:("suite-performance",),12:("suite-regression-dual",)}.items():mapping["phases"][str(index)]=[legacy(name) for name in names]
    for index in range(1,24):mapping["outputs"][str(index)]=[ref("materials","output-"+str(index))]
    for index,names in {13:("suite-network","suite-rpc"),14:("suite-network","suite-rpc"),15:("suite-network",),16:("required-five-module-group-single-hybrid",),
        17:("suite-additional-security","suite-payload-security","suite-flood"),18:("suite-network",),19:("suite-network",),20:("suite-network",),
        23:("suite-network",)}.items():mapping["outputs"][str(index)]=[legacy(name) for name in names]
    return mapping,example_names


def selection(source,manifest,inputs,execution):
    require(all(name in inputs and type(inputs[name]) is str and inputs[name] and "\0" not in inputs[name]
        for name in (*COMMON_INPUTS,*LEGACY_INPUTS)),"closed current SourceB physical inputs missing")
    require(all(not value.startswith(("REPLACE_WITH_","REPLACED_BY_")) for name,value in inputs.items()
        if name not in ("worker","worker-report")),"unresolved template literal cannot be executed")
    require((source/"tests/ros_custom_types/deployment-final/generated/manifest.hpp").is_file(),"complete composed provider must already be frozen in SourceB")
    jobs=[];references,example_names=original_mapping();records,documents=legacy_records()
    # This is a mandatory supplementary compiler/mock-model obligation. Its
    # host executables and mathematical Wasm runs never replace one of the65
    # original behavior requirements or any ARM/native ROS crosswalk record.
    # Run it first so missing SDK examples/models fail before four large Worker
    # builds. Every tool path/package is selected by the closed input JSON.
    sdk_record=base_record("sdk-complete-matrix","raw",kind="sdk-compiler-model",labels=(),facts=[
        eq([key("evidence_scope")],"sdk-compiler-model-supplement"),
        eq([key("supplemental_scope")],"sdk-complete-matrix"),
        eq([key("actual_host_sdk_checks")],"EXECUTED"),
        eq([key("actual_ROS_delivery")],"NOT_EXECUTED"),
        eq([key("actual_ARM_Runtime_execution")],"NOT_EXECUTED"),
        eq([key("languages")],["c","cpp","rust","tinygo","assemblyscript","javascript_bridge"]),
        eq([key("backends")],[1,2]),eq([key("wasm_products")],349),eq([key("fresh_full_validations")],349),
        eq([key("complete_example_variants_verified")],True),eq([key("whole_tool_roots_preserved")],True),
        eq([key("pure_models")],10),eq([key("pure_assertions")],446),eq([key("relay_cases")],80),
        eq([key("scheduling_cases")],12),eq([key("bounded_python_tests")],[17,9]),
        eq([key("native_mock_test_counts")],{"rust":20,"go":21}),
        eq([key("bounded_cpp_assertions")],[67,67,27,26]),count([key("bounded_models")],6)])
    jobs.append(job("sdk-complete-matrix","tests/ros_acceptance/run_sdk_matrix.py",
        [*common(),"--tools",inp("sdk-matrix-tool-config")],[sdk_record]))
    # Build all four profiles in this same final run. All subsequent products
    # refer to that exact output; no initial preparation-only Worker is reused.
    inputs=dict(inputs);worker_raw=execution/"workers/raw"
    inputs.update({"worker":str(worker_raw/"dual/uwvm"),"worker-report":str(worker_raw/"report.json")})
    workers=[]
    for profile in ("none","ros1","ros2","dual"):
        workers.append({"id":"worker-"+profile,"kind":"compiler","path":[key("profiles"),{"where":"profile","equals":profile}],
            "facts":[eq([key("status")],"PASS"),count([key("commands")],5)],"commands":[{"driver_exit":True}],
            "ARM_product_selectors":[[key("profiles"),{"where":"profile","equals":profile},key("binary")]]})
    jobs.append(job("workers","tests/ros_aarch64/build_worker.py",["--source",inp("source"),"--manifest",inp("manifest"),"--out",out("."),"--config",inp("worker-config")],workers,architecture=None,requires=("sdk-complete-matrix",)))
    full=["--source",inp("source"),"--out",out("."),"--worker-build",str(worker_raw),"--config",inp("worker-config"),
        "--qemu",inp("qemu"),"--wat2wasm",inp("wat2wasm"),"--binfmt-evidence",inp("binfmt-evidence"),"--container-started-at",inp("container-started-at")]
    for name in ("workspace","sdk-workspace","gateway-build","prepared-resources","prepared-action-qos","prepared-ros1-search",
        "prepared-ros2-local-parameter-events","source-addon","source-addon-manifest","ros1-gateway-report","ros2-gateway-report"):full += ["--"+name,inp(name)]
    full += ["--rpc-denial-gate"]
    material_root=execution/"materials/raw"
    indexed={record["id"]:record for record in records}
    indexed["mandatory-cli"]["source_program"]={"path":str(source/"tests/ros_cli/run.py"),"document":"cli",
        "sha256_selector":[key("original_driver_sha256")]}
    for index,name in enumerate(example_names,1):
        template=json.loads(json.dumps(indexed[name]));template["id"]="example-"+str(index)
        template["example_material"]=str(material_root/(str(index).zfill(2)+"/material.json"))
        template["Guest_exit"]=[*template["path"],key("wasm_exit")]
        if "native_witnesses" not in template and "native_counter_assertions" not in template:
            # A native count projection or a current ordinary witness receipt
            # is mandatory. Empty native-witness vectors cannot qualify.
            template["native_witnesses"]=[key("native_witnesses")]
        template["deliver_receipt"]=True;records.append(template)
    # Material must exist before the actual old examples are normalized.
    materials=[]
    for index in range(1,24):
        materials.append({"id":"output-"+str(index),"kind":"material","path":[],"facts":[eq([key("status")],"PREPARED_NOT_EXECUTED"),count([key("examples")],20)],
            "materials":[inp("source")],"scope":"Physical delivered source/documents; actual result files are appended separately after all current suites"})
    # Directories are never accepted as file pins: each deliverable below names
    # actual regular files. Runtime result deliverables additionally pin the
    # final run report in the final output-packaging stage, not this renderer.
    file_sets={1:("tests/ros_acceptance/README.md",),2:("tests/ros_acceptance/selection_template.py",),3:("sdk/ros1/abi.h","documents/ros/abi.md"),
        4:("sdk/ros2/abi.h","documents/ros/abi.md"),5:("documents/ros/capabilities.md",),6:("documents/ros/capabilities.md",),7:("documents/ros/capabilities.md",),
        8:("documents/ros/security.md",),9:("documents/ros/security.md",),10:("documents/ros/architecture.md",),11:("documents/ros/abi.md",),12:("documents/ros/abi.md",),
        13:("tests/ros_acceptance/SELECTION.md",),14:("tests/ros_acceptance/SELECTION.md",),15:("tests/ros_acceptance/SELECTION.md",),16:("examples/ros/catalog.json",),
        17:("tests/ros_acceptance/SELECTION.md",),18:("tests/ros_acceptance/SELECTION.md",),19:("tests/ros_acceptance/README.md",),20:("tests/ros_acceptance/selection_template.py",),
        21:("tests/ros_acceptance/run.py",),22:("tests/ros_acceptance/README.md","documents/ros/current-limitations.md",
            "documents/ros/current-capabilities.md","sdk/docs/client-library-coverage.md","sdk/custom-types/BOUNDED_V2.md",
            "sdk/docs/unified-events.md"),23:("tests/ros_acceptance/run.py","tests/ros_acceptance/requirements.py")}
    # Historical explanatory files are retained, but delivered interface and
    # policy material must also pin the current additive event/custom profile.
    # These are material-only rows, never substitutes for Runtime evidence.
    current_policy=("documents/ros/current-capabilities.md",)
    current_abi=("sdk/docs/abi-v1.md","sdk/docs/unified-events.md","sdk/custom-types/BOUNDED_V2.md")
    for index in (5,6,7):file_sets[index]+=current_policy
    for index in (3,4,11,12):file_sets[index]+=current_abi
    for record in materials:record["materials"]=[str(source/name) for name in file_sets[int(record["id"].split("-")[1])]]
    materials.append({"id":"documentation","kind":"material","path":[],"facts":[count([key("examples")],20)],
        "materials":[str(source/name) for name in ("documents/ros/capabilities.md","documents/ros/security.md","documents/ros/abi.md","examples/ros/catalog.json",*current_policy,*current_abi)]})
    material_args=["--source",inp("source"),"--manifest",inp("manifest"),"--out",out("."),"--uwvm",inp("worker"),"--wat2wasm",inp("wat2wasm")]
    for name in ("socket-dir","ros1-rootfs","ros2-rootfs","native-products","worker-pid","worker-uid","gateway-uid","ros1-gateway-report","ros2-gateway-report"):
        material_args += ["--"+name,inp({"socket-dir":"example-socket-dir","worker-pid":"example-worker-pid"}.get(name,name))]
    # Full strict Gateway qualification and before/after SDK closure are real
    # read-only work. This material-only bound changes no Guest/ROS deadline.
    jobs.append(job("materials","examples/ros/materials.py",material_args,materials,requires=("workers",),report="materials.json",architecture=None,success="PREPARED_NOT_EXECUTED",seconds=180))
    jobs.append(job("original-all","tests/ros_aarch64/run_all.py",full,records,requires=("materials","workers"),architecture="network/architecture-evidence.json",documents=documents))
    # Run bounded chunks serially; the read-only aggregate qualifies their
    # actual successful mains, retained ARM/QEMU birth samples and full-MD
    # products. It does not launch those targets again. Two explicit LLVM-only
    # harness exclusions remain visible and cannot be counted as test PASS.
    regression_base=["--source",inp("source"),"--manifest",inp("manifest"),"--config",inp("worker-config"),
        "--sdk-manifest",inp("cross-sdk-manifest"),"--compiler-resource-root",inp("compiler-resource-root"),"--out",out(".")]
    chunks=[("original-regressions-portable",["--groups","wasip1-functions","wasip1-hybrid","local-imported","uwvm-int-portable","uwvm-int-validate"],183)]
    for combine in ("none","soft","heavy","extra"):
        for delay in ("none","soft","heavy"):
            chunks.append(("original-regressions-strict-"+combine+"-"+delay,
                ["--groups","uwvm-int-strict","--strict-config",combine+":"+delay],241))
    for label,arguments,expected_rows in chunks:
        row=base_record(label,"raw",facts=[eq([key("mode")],"chunk"),count([key("suites")],expected_rows),
            eq([key("source_inputs_preserved")],True),eq([key("sdk_inputs_preserved")],True),eq([key("tool_inputs_preserved")],True)],kind="native")
        row["mapped_launches"]=[key("launches")]
        jobs.append(job(label,"tests/ros_aarch64/run_original_regressions.py",[*regression_base,*arguments],[row],
            report="results.json",architecture="architecture/architecture-evidence.json",requires=("original-all",)))
    aggregate_args=[*regression_base]
    for label,_,_ in chunks:aggregate_args += ["--aggregate-input",produced(label,"results.json")]
    aggregate_row=base_record("original-regressions-full","raw",facts=[eq([key("mode")],"aggregate"),count([key("suites")],3073),
        eq([key("required_portable_sources")],422),count([key("excluded_backend_sources")],2),
        *[eq([key("groups"),key(group),key("status")],"PASS") for group in ("wasip1-functions","wasip1-hybrid","local-imported","uwvm-int")]],kind="native")
    aggregate_row["mapped_launches"]=[key("launches")]
    jobs.append(job("original-regressions-full","tests/ros_aarch64/run_original_regressions.py",aggregate_args,[aggregate_row],
        report="results.json",architecture="architecture/architecture-evidence.json",requires=tuple(label for label,_,_ in chunks)))
    native_adoption=base_record("native-adoption","raw",facts=[eq([key("actual_native_behavior_probes")],1)],labels=("action-qos-adoption",))
    args=["--source",inp("source"),"--rootfs",inp("ros2-rootfs"),"--out",out("."),"--qemu",inp("qemu"),
        "--binfmt-evidence",inp("binfmt-evidence"),"--container-started-at",inp("container-started-at"),"--mode","execute","--prepared",inp("prepared-native-adoption")]
    jobs.append(job("native-adoption","tests/ros_aarch64/run_native_adoption.py",args,[native_adoption],requires=("original-all",)))
    for label,driver,prepared in (("enforce","run_g09_guest_enforce.py","prepared-enforce"),("reconnect","run_revoke_reconnect.py","prepared-reconnect")):
        args=["--source",inp("source"),"--manifest",inp("manifest"),"--rootfs",inp("ros2-rootfs"),"--out",out("."),"--qemu",inp("qemu"),
            "--binfmt-evidence",inp("binfmt-evidence"),"--container-started-at",inp("container-started-at"),"--mode","execute","--prepared",inp(prepared)]
        for name in ("worker","worker-report","worker-config","wat2wasm"):args += ["--"+name,inp(name)]
        args += ["--gateway",inp("ros2-gateway"),"--gateway-report",inp("ros2-gateway-report"),"--expected-cgroup",inp("expected-cgroup")]
        labels=("g09-worker","g09-gateway","g09-secure-witness") if label=="enforce" else ("revoke-worker","revoke-gateway","revoke-reconnect-witness")
        row=base_record(label,"raw",facts=[eq([key("actual_guest_behavior")],"PASS")],labels=labels)
        row["security_contract"]={"kind":"owned-lifetime","scope":"Actual signed three-role controller's strict combined completion oracle",
            "assertions":[eq([key("actual_guest_behavior")],"PASS"),eq([key("secrets_removed")],True),eq([key("socket_absent")],True)]}
        jobs.append(job(label,"tests/ros_aarch64/"+driver,args,[row],requires=("original-all",)))
    unit_args=["--source",inp("source"),"--manifest",inp("manifest"),"--config",inp("worker-config"),"--out",out("."),"--suite","unified-events",
        "--sdk-manifest",inp("cross-sdk-manifest"),"--compiler-resource-root",inp("compiler-resource-root")]
    unit=base_record("unified-contracts","raw",facts=[count([key("suites")],4)],kind="native",labels=("unified-event-queue","unified-event-async","unified-event-manager","unified-event-server"))
    unit["security_contract"]={"kind":"native-concurrency","scope":"Four actual queue/owned-worker/manager/socketpair units; this does not claim a ROS native seven-kind exchange",
        "assertions":[count([key("suites")],4)]}
    jobs.append(job("unified-contracts","tests/ros_aarch64/run_unit.py",unit_args,[unit],report="results.json",architecture="architecture/architecture-evidence.json",requires=("original-all",)))
    jobs.append(job("bounded-service","tests/ros_acceptance/run_bounded_service.py",[*common(),"--prepared-native",inp("prepared-bounded-service")],
        [base_record("bounded-service","raw",facts=[eq([key("new_tests_executed")],True)],labels=("bounded-service",),kind="native")],requires=("original-all",)))
    for version in (1,2):
        label="events-ros"+str(version)
        row=base_record(label,"raw",facts=[eq([key("seven_event_kinds")],[1,2,3,4,5,6,7]),eq([key("four_additive_imports_executed")],True),
            eq([key("too_small90_retains_all_seven_records")],True),eq([key("memory1_and_memory0_sentinel")],True)],labels=("worker","gateway-"+str(version),"event-peer"))
        jobs.append(job(label,"tests/ros_acceptance/run_events.py",[*common(),*products(),"--version",str(version),"--prepared-native",inp("prepared-events")],[row],requires=("original-all",)))
    coverage={"worker-four-profiles":["workers"],"native-original":["original-all"],"mandatory-cli":["original-all"],"original-regressions":["original-all","original-regressions-full"],
        "original-security":["original-all"],"payload-security":["original-all"],"ingress-trap":["original-all"],"revoke-race":["original-all"],"flood":["original-all"],
        "five-modules":["original-all"],"dual-network":["original-all"],"ros1-search":["original-all"],"ros2-resources":["original-all"],"action-policy":["original-all"],
        "action-native-qos40":["native-adoption"],"seven-events":["events-ros1","events-ros2"],"signed-enforce":["enforce"],"revoke-reconnect":["reconnect"],
        "wasi-regression":["original-all","original-regressions-full"],"trusted-dl-regression":["original-all","original-regressions-full"],"runtime-regression":["original-all","original-regressions-full"],"bounded-service":["bounded-service"],
        "native-versus-wasm-performance":["original-all"],"unified-event-contracts":["unified-contracts"]}
    fixed=base_record("custom-fixed-guest","raw",facts=[count([key("cases")],2)],labels=("worker-1","worker-2","gateway-1","gateway-2"))
    jobs.append(job("custom-fixed-guest","tests/ros_aarch64/run_custom_guest.py",[*common(),*products()],[fixed],requires=("original-all",)))
    provider=source/"tests/ros_custom_types/deployment-final/generated"
    native_args=["--source",inp("source"),"--sdk-workspace",inp("sdk-workspace"),"--out",out("."),"--qemu",inp("qemu"),
        "--binfmt-evidence",inp("binfmt-evidence"),"--container-started-at",inp("container-started-at"),"--mode","all",
        "--custom-registry-header",str(provider/"manifest.hpp"),"--custom-prefix","/opt/uwvm-custom-types",
        "--custom-native-header","1="+str(provider/"native_ros1.hpp"),"--custom-native-header","2="+str(provider/"native_ros2.hpp"),
        "--custom-packages","1=uwvm_custom_ros1_fixture,uwvm_ros_bounded_v2",
        "--custom-packages","2=uwvm_ros_custom_test_msgs,uwvm_ros_bounded_v2"]
    native_fixed=base_record("custom-fixed-native","raw",facts=[eq([key("actual_native_behavior_probes")],2)],kind="native",
        labels=("custom-ros1-native","custom-ros2-native"))
    jobs.append(job("custom-fixed-native","tests/ros_aarch64/run_custom_topics.py",native_args,[native_fixed],requires=("original-all",)))
    bounded=[]
    for version in (1,2):
        for direction in ("topics","denied","rejection","client","server"):
            for language in (("c","cpp","rust","tinygo","assemblyscript","javascript") if direction in ("client","server") else (None,)):
                label="custom-bounded-"+str(version)+"-"+direction+("-"+language if language else "")
                argv=[*common(),*products(),"--prepared-native",inp("prepared-custom-bounded"),"--version",str(version),"--case",direction]
                if language:argv += ["--language",language,"--products",inp("custom-products")]
                if language=="javascript":argv += ["--node",inp("node")]
                row=base_record(label,"raw",facts=[eq([key("actual_guest_behavior")],"PASS"),eq([key("version")],version),eq([key("case")],direction)],
                    labels=("worker","gateway-"+str(version),"bounded-peer-runtime"))
                jobs.append(job(label,"tests/ros_custom_types/bounded-v2/run_custom_bounded.py",argv,[row],requires=("original-all",)))
                bounded.append(label)
    coverage.update({"custom-types":["custom-fixed-native","custom-fixed-guest",*bounded],"custom-guest":["custom-fixed-guest",*bounded]})
    inventory=inventories(source)
    require(all(set(references[name])=={str(row["id"]) for row in rows} for name,rows in inventory.items()),"complete original numeric inventory differs")
    return {"format_version":1,"kind":"ros-final-explicit-selection","status":"PREPARED_NOT_EXECUTED",
        "source_manifest_sha256":hashlib.sha256(manifest.read_bytes()).hexdigest(),"inputs":inputs,"jobs":jobs,"coverage":coverage,
        "supplemental_coverage":{"sdk-complete-matrix":["sdk-complete-matrix"]},
        "gap_prerequisites":sorted(("original-all","workers")),"original_requirements":references,
        "scope":"Concrete current selectors; no historical status reused. Missing native receipts fail closed.",
        "hosted_GitHub_execution_claimed":False}


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ("source","manifest","inputs","out","execution-out"):parser.add_argument("--"+name,type=Path,required=True)
    args=parser.parse_args();source=args.source.resolve(strict=True);inputs,pin=load_bound(args.inputs)
    require(not args.out.exists() and not args.execution_out.exists(),"fresh plan and future execution outputs required")
    value=selection(source,args.manifest.resolve(strict=True),inputs,args.execution_out.resolve())
    value["input_selection_pin"]=pin;args.out.mkdir(mode=0o700,parents=True)
    (args.out/"selection.json").write_text(json.dumps(value,indent=2)+"\n")
    return 0


if __name__=="__main__":raise SystemExit(main())
