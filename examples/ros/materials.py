#!/usr/bin/env python3
"""Render all twenty complete original examples without executing anything.

Each emitted document contains physical Guest source, compiler argv, complete
uwvm argv, mirrored Gateway config, explicit permissions/limits, expected
output and an independent native test recipe. Caller paths and UID/PID are
required; no symbolic template, Level, Role or permission preset is passed to
uwvm. Output status remains PREPARED_NOT_EXECUTED. A trusted supervisor must
reserve ports/domains, hold the exact Worker PID across exec and qualify/map
all products before using these commands.

Each backend's Gateway must have a fresh strict compilation report. The shared
qualifier binds its complete frozen source, SDK and custom-provider selection;
the loader helper may then return one closed native library directory. This is
read-only product qualification, not an execution or an additional Guest right.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shlex
import sys

sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[2]


def load(name,path):
    spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module);return module


def pin(path):
    path=Path(path).resolve(strict=True);return {"path":str(path),"sha256":hashlib.sha256(path.read_bytes()).hexdigest(),"bytes":path.stat().st_size}


def prepare_gateways(args,products,frozen,input_pins):
    """Qualify each selected physical Gateway once, before rendering lessons.

    Use the same compiler/SDK validator and closed loader selector as the actual
    acceptance lanes. A raw report's PASS string, a caller-supplied library path
    or an ambient setup prefix cannot substitute for either operation. The SDK
    snapshot below has just been compared to the complete current SDK by the
    qualifier, including root aliases; finish_gateways checks it again after
    all material writes. No SDK path is written or activated by this renderer.
    """
    from gateway_qualification import qualify_gateway,read_file_binding
    from native_runtime_environment import gateway_library_environment
    from runtime import read_json_binding

    expected_families={"gateway","topic","resources","action","tf","five"}
    if not isinstance(products,dict) or set(products)!={"1","2"}:
        raise ValueError("native products require exactly both backend mappings")
    states={}
    for version in (1,2):
        entries=products[str(version)]
        if not isinstance(entries,dict) or set(entries)!=expected_families:
            raise ValueError("native products require exactly six product families per backend")
        product_pins={}
        for family,selected in entries.items():
            if (not isinstance(selected,dict) or set(selected)!={"origin","guest_path"} or
                    not all(isinstance(selected[key],str) and selected[key] and "\0" not in selected[key]
                            and Path(selected[key]).is_absolute() for key in ("origin","guest_path")) or
                    selected["guest_path"].startswith("//") or
                    os.path.normpath(selected["guest_path"])!=selected["guest_path"] or
                    ".." in Path(selected["guest_path"]).parts):
                raise ValueError("product requires exact physical origin and canonical absolute guest_path")
            product_pins[family]=read_file_binding(Path(selected["origin"]).resolve(strict=True))
            input_pins.append(product_pins[family])
        # Resolve each selected origin once. Later material pins use these same
        # opened physical products rather than following a mutable view alias.
        origin=Path(product_pins["gateway"]["path"])
        sdk=(args.ros1_rootfs if version==1 else args.ros2_rootfs).resolve(strict=True)
        report=args.ros1_gateway_report if version==1 else args.ros2_gateway_report
        qualification=qualify_gateway(origin,report,args.manifest,args.source,sdk,version)
        binary=qualification["binary"];origin_pin=read_file_binding(origin)
        if any(binary.get(key)!=origin_pin[key] for key in ("path","sha256","bytes","device","inode")):
            raise RuntimeError("qualified Gateway differs from selected native-products origin")
        input_pins.extend(qualification["input_pins"])
        inventory,inventory_pin=read_json_binding(qualification["sdk_static_input_binding"]["path"],limit=256*1024*1024)
        if inventory_pin!=qualification["sdk_static_input_binding"]:
            raise RuntimeError("qualified complete SDK snapshot changed before material rendering")
        loader=gateway_library_environment(qualification,sdk,inventory)
        input_pins.extend(loader["input_pins"])
        before=args.out/f"gateway-ros{version}-sdk-before.json"
        before.write_text(json.dumps({"inventory":inventory,"aliases":qualification["sdk_root_aliases"]},sort_keys=True,separators=(",",":"))+"\n")
        before.chmod(0o600);before_pin=read_file_binding(before);input_pins.append(before_pin)
        states[version]={"sdk":sdk,"inventory":inventory,"aliases":qualification["sdk_root_aliases"],
                         "qualification":qualification,"loader":loader,"before":before_pin,"origin":origin_pin,
                         "products":product_pins}
    # Bind the actual Python helper bytes selected by this process, as well as
    # the archived helper/control inputs already returned by strict qualification.
    for name in ("gateway_qualification","native_runtime_environment","rebuild_affected","runtime"):
        path=Path(sys.modules[name].__file__).resolve(strict=True)
        relative=f"tests/ros_aarch64/{name}.py";binding=read_file_binding(path)
        if path!=args.source/relative or frozen.get(relative,{}).get("sha256")!=binding["sha256"]:
            raise RuntimeError("material qualification helper is not the exact frozen source")
        input_pins.append(binding)
    return states


def finish_gateways(args,states,frozen,input_pins):
    """Retain full before/after SDK and alias evidence without claiming a run."""
    from gateway_qualification import read_file_binding,pins_preserved
    from rebuild_affected import sdk_inventory,sdk_root_aliases,source_state

    rows={}
    for version,state in states.items():
        after_inventory=sdk_inventory(state["sdk"]);after_aliases=sdk_root_aliases(state["sdk"])
        if after_inventory!=state["inventory"] or after_aliases!=state["aliases"]:
            raise RuntimeError("material generation changed the complete SDK or its root aliases")
        after=args.out/f"gateway-ros{version}-sdk-after.json"
        after.write_text(json.dumps({"inventory":after_inventory,"aliases":after_aliases},sort_keys=True,separators=(",",":"))+"\n")
        after.chmod(0o600);after_pin=read_file_binding(after);input_pins.append(after_pin)
        rows[str(version)]={"selected_origin":state["origin"],"qualification":state["qualification"],
            "loader":state["loader"],"sdk_root":str(state["sdk"]),"sdk_inventory_before":state["before"],
            "sdk_inventory_after":after_pin,"sdk_root_aliases_before":state["aliases"],
            "sdk_root_aliases_after":after_aliases,"sdk_preserved":True,"sdk_root_aliases_preserved":True}
    source_state(args.source,frozen)
    if not pins_preserved(input_pins):
        raise RuntimeError("material source/product/qualification/control input changed during rendering")
    return rows


def serialize(args,version,scope,target,members,connection,grants,socket_name):
    """Serialize every explicitly chosen field to both sides of exact binding."""
    prefix=f"--ros{version}-{scope}-";cli=[prefix+"create",target]
    if scope=="group":
        for member in members:cli += [prefix+"add-module",target,member]
    cli += [prefix+"enable",target]
    selected={**connection,"gateway-socket":str(args.socket_dir/socket_name),"gateway-uid":str(args.gateway_uid),
              "connect-timeout-ms":"10000","audit-deny-max-rate-hz":"100","audit-deny-burst":"20",
              "audit-deny-max-records":"10000","audit-deny-max-bytes":"1048576"}
    for key,value in selected.items():cli += [prefix+key,target,value]
    for grant in grants:
        cli += [prefix+"grant",target,*[grant[key] for key in ("alias","operation","resource","type","identity")]]
        for key,value in grant["limits"].items():cli += [prefix+"set",target,grant["alias"],key,value]
    config=[f"backend ros{version}",f"worker-uid {args.worker_uid}",f"worker-pid {args.worker_pid}",
            f"socket {args.socket_dir/socket_name}","timeout-ms 10000","max-sessions 8",
            "audit-max-records-per-second 100","audit-max-records 10000","audit-max-bytes 1048576"]
    config += ["connection "+key+" "+value for key,value in connection.items()]
    for cap in grants:
        config += ["grant "+" ".join(cap[key] for key in ("alias","operation","resource","type","identity"))]
        config += ["set "+cap["alias"]+" "+key+" "+value for key,value in cap["limits"].items()]
    return cli,"\n".join(config)+"\n"


def five_targets(network):
    """The original five real module identities, including explicit quota grant."""
    targets=[]
    records=((1,"group","legacy_sensors",["camera_a","camera_b"],[("camera_a","/five/camera_a"),("camera_b","/five/camera_b")]),
             (1,"single","controller",[],[("control","/five/controller")]),
             (2,"group","modern_planners",["planner"],[("plan","/five/planner")]),
             (1,"single","hybrid",[],[("hybrid1","/five/hybrid1")]),
             (2,"single","hybrid",[],[("hybrid2","/five/hybrid2")]))
    for version,scope,name,members,aliases in records:
        connection,_=network.policy(version,[]);connection.update({"node-name":"uwvm_five_"+name,"namespace":"/five"})
        connection["master-uri" if version==1 else "domain-id"]="http://127.0.0.1:11379" if version==1 else "79"
        grants=[]
        for alias,resource in aliases:
            _,selected=network.policy(version,["publish"]);cap=selected[0];cap.update(alias=alias,resource=resource)
            cap["limits"].update({"max-message-bytes":"128","max-rate-hz":"20","burst":"4","max-queue-bytes":"512"})
            cap["limits"].update({"latch":"true"} if version==1 else {"qos.durability":"transient-local"});grants.append(cap)
        if name=="legacy_sensors":
            _,selected=network.policy(1,["publish"]);cap=selected[0];cap.update(alias="quota",resource="/five/shared_cameras")
            cap["limits"].update({"max-message-bytes":"128","max-rate-hz":"0.001","burst":"2","max-queue-bytes":"512","latch":"true"});grants.append(cap)
        targets.append((version,scope,name,members,connection,grants))
    return targets


def material(args,row,network,rpc,products,gateways):
    versions=(1,2) if row["version"]=="dual" else (row["version"],)
    sources=[args.source/row["source"]];targets=[];extra=[];main="main";native=[]
    if row["role"]=="five":
        sources=[args.source/f"examples/ros_multimodule/{name}.wat" for name in ("camera_a","camera_b","controller","planner","hybrid")]
        targets=five_targets(network);main="hybrid"
        for name in ("camera_a","camera_b","controller","planner"):extra += ["--wasm-preload-library",str(args.out/f"{row['id']:02d}"/(name+".wasm")),name]
    elif row["role"]=="hybrid":
        for version,direction in ((1,"subscribe"),(2,"publish")):
            connection,grants=network.policy(version,[direction]);targets.append((version,"single","main",[],connection,grants))
            native.append({"version":version,"family":"topic","arguments":["--publish" if version==1 else "--observe-count2","/uwvm_ros_test/input" if version==1 else "/uwvm_ros_test/output","hello"]})
    elif row["role"]=="foreign-handle":
        sources.insert(0,args.source/"examples/ros_dual/handle_provider.wat");connection,grants=network.policy(1,["publish"])
        targets=[(1,"single","provider",[],connection,grants),(1,"single","main",[],connection,[])]
        # One backend may have several subjects. Each needs its own config and
        # socket so different authoritative manifests are not conflated.
        extra=["--wasm-preload-library",str(args.out/f"{row['id']:02d}"/"handle_provider.wasm"),"provider"]
    else:
        version=versions[0];role=row["role"]
        if role in ("param.set","tf.lookup"):
            connection,_=network.policy(version,[]);grants=[rpc.grant(version,role)]
        else:connection,grants=network.policy(version,["subscribe" if role=="denied" else role])
        targets=[(version,"single","main",[],connection,grants)]
    if row["role"]!="hybrid" and row["role"]!="five":
        native=[{"version":versions[0],"family":row["native_family"],"arguments":row["native_arguments"]}]
    if row["role"]=="five":
        native=[{"version":1,"family":"five","arguments":["/five/camera_a","camera-a","/five/camera_b","camera-b","/five/controller","controller","/five/hybrid1","hybrid-ros1","/five/shared_cameras","camera-a","/five/shared_cameras","camera-b"]},
                {"version":2,"family":"five","arguments":["/five/planner","planner","/five/hybrid2","hybrid-ros2"]}]
    directory=args.out/f"{row['id']:02d}";directory.mkdir();argv=[str(args.uwvm)];configs=[]
    for index,(version,scope,target,members,connection,grants) in enumerate(targets):
        # Provider and main are separate subjects on ROS1; main has zero grants
        # and never opens its socket. Configs still describe both explicitly.
        socket_name=f"example{row['id']}-ros{version}-{index}.sock"
        cli,config=serialize(args,version,scope,target,members,connection,grants,socket_name);argv+=cli
        path=directory/(f"gateway-{index}.conf");path.write_text(config);path.chmod(0o600)
        selected=products[str(version)]["gateway"]
        if not isinstance(selected,dict) or set(selected)!={"origin","guest_path"} or not Path(selected["guest_path"]).is_absolute() or ".." in Path(selected["guest_path"]).parts:
            raise ValueError("gateway product requires exact physical origin and canonical guest_path")
        root=args.ros1_rootfs if version==1 else args.ros2_rootfs;distro="noetic" if version==1 else "jazzy"
        environment={"ROS_MASTER_URI":connection["master-uri"],"ROS_IP":connection["advertise-host"]} if version==1 else {
            "ROS_DOMAIN_ID":connection["domain-id"],"RMW_IMPLEMENTATION":connection["rmw-implementation"],"ROS_SECURITY_ENABLE":"false"}
        # Only a strictly qualified Gateway receives this explicit native loader
        # entry. Original native probes below receive no custom loader setting.
        environment.update(gateways[version]["loader"]["environment"])
        command=["/usr/bin/env","-i","PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin","HOME=/root","LANG=C.UTF-8",
            *[key+"="+value for key,value in environment.items()],"/usr/bin/chroot",str(root),"/bin/bash","-lc",
            f'source /opt/ros/{distro}/setup.bash && exec "$@"',"original-example-gateway",selected["guest_path"],str(path)]
        configs.append({"version":version,"scope":scope,"target":target,"members":members,"connection":connection,"grants":grants,"config":pin(path),"text":config,
                        "product":{key:gateways[version]["origin"][key] for key in ("path","sha256","bytes")},
                        "guest_path":selected["guest_path"],"command":command,"environment":environment,
                        "gateway_qualification_report":gateways[version]["qualification"]["report"],
                        "gateway_loader_scope":gateways[version]["loader"]["scope"]})
    modules=["camera_a","camera_b","controller","planner","hybrid"] if row["role"]=="five" else ["main","provider"] if row["role"]=="foreign-handle" else ["main"]
    # One complete WASI Group accompanies the separate ROS1/ROS2 Groups in the
    # five-module lesson. Basic stdio remains the only implicit WASI authority.
    if row["role"]=="five":
        argv += ["--wasip1-group-create","basic_io"]
        for name in modules:argv += ["--wasip1-group-add-module","basic_io",name]
        argv += ["--wasip1-group-enable","basic_io","--wasip1-group-noinherit-system-environment","basic_io","--wasip1-group-grant","basic_io","monotonic-clock"]
    else:
        for name in modules:argv += ["--wasip1-single-create",name,"--wasip1-single-enable",name,"--wasip1-single-noinherit-system-environment",name]
        if any('"clock_time_get"' in source.read_text() for source in sources):argv += ["--wasip1-single-grant",main,"monotonic-clock"]
    argv += ["--wasm-set-main-module-name",main,*extra,"--run",str(directory/(sources[-1].stem+".wasm"))]
    compiles=[[str(args.wat2wasm),"--enable-all",str(source),"-o",str(directory/(source.stem+".wasm"))] for source in sources]
    for recipe in native:
        version=recipe["version"];selected=products[str(version)][recipe["family"]]
        if not isinstance(selected,dict) or set(selected)!={"origin","guest_path"}:
            raise ValueError("native product requires exact physical origin and guest_path")
        root=args.ros1_rootfs if version==1 else args.ros2_rootfs;distro="noetic" if version==1 else "jazzy"
        recipe["product"]={key:gateways[version]["products"][recipe["family"]][key] for key in ("path","sha256","bytes")}
        recipe["guest_path"]=selected["guest_path"]
        if not Path(selected["guest_path"]).is_absolute() or ".." in Path(selected["guest_path"]).parts:
            raise ValueError("canonical absolute native guest path required")
        recipe["environment"]={"ROS_MASTER_URI":"http://127.0.0.1:11379" if row["role"]=="five" else "http://127.0.0.1:11339","ROS_IP":"127.0.0.1"} if version==1 else {"ROS_DOMAIN_ID":"79" if row["role"]=="five" else "73","RMW_IMPLEMENTATION":"rmw_fastrtps_cpp","ROS_SECURITY_ENABLE":"false"}
        recipe["command"]=["/usr/bin/env","-i","PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin","HOME=/root","LANG=C.UTF-8",
            *[key+"="+value for key,value in recipe["environment"].items()],"/usr/bin/chroot",str(root),"/bin/bash","-lc",
            f'source /opt/ros/{distro}/setup.bash && exec "$@"',"original-example-native",selected["guest_path"],*recipe["arguments"]]
    result={**row,"status":"PREPARED_NOT_EXECUTED","Guest_sources":[pin(path) for path in sources],"compile_argv":compiles,
        "runtime_argv":argv,"Gateway_configs":configs,"permissions":[{"version":c["version"],"target":c["target"],"grants":c["grants"]} for c in configs],
        "resource_limits":[{"version":c["version"],"target":c["target"],"aliases":{cap["alias"]:cap["limits"] for cap in c["grants"]}} for c in configs],
        "expected_output":row["expected"],"native_test":native,
        "actual_test_method":"Qualify frozen compile/link/SDK products; stage native and Gateway snapshots read-only; reserve owned Master/domain; hold exact WorkerPID before exec; map all live ARM actors; start independent peer and wait actual READY; release Guest; check actual full payload/effect count, exit0, denial sentinels and zero unauthorized endpoint/message effects; retain raw logs/cgroup/OOM and owned cleanup. No current run claimed by this renderer."}
    # Canonical seven-part names used by the final raw-evidence normalizer.
    # Metadata remains PREPARED_NOT_EXECUTED until independent actual rows are
    # associated; presence of these fields never supplies a Runtime PASS.
    result.update(guest_source=result["Guest_sources"],grants=result["permissions"],limits=result["resource_limits"])
    (directory/"material.json").write_text(json.dumps(result,indent=2)+"\n")
    text=f"# {row['id']}. {row['name']}\n\nPrepared source material; no compilation or native execution is claimed.\n\n"
    text+="Guest source: "+", ".join(str(source) for source in sources)+"\n\nCompile each actual source:\n\n```sh\n"+"\n".join(shlex.join(command) for command in compiles)+"\n```\n\nComplete uwvm argv:\n\n```sh\n"+shlex.join(argv)+"\n```\n\n"
    text+="The principal/socket/PID values are explicit caller inputs. Preserve the stopped Worker PID across exec; regenerate configs if its birth changes. Bind the owned material output and socket directory at these same absolute paths inside each private rootfs; stage immutable product copies at the selected guest paths. No operation grants another operation. These clean environments inherit no ambient ROS discovery, security, namespace or loader override. Only Gateways may receive the shared qualifier's explicit closed custom-library path; ordinary native probes do not. Full source, SDK, aliases and qualification inputs must remain unchanged after material generation and through execution.\n\nComplete Gateway commands after qualification/staging:\n\n```sh\n"+"\n".join(shlex.join(config["command"]) for config in configs)+"\n```\n\n"
    text+="Permissions and every resource limit (same bytes are mirrored to Gateway):\n\n```json\n"+json.dumps(result["permissions"],indent=2)+"\n```\n\nExpected output: "+row["expected"]+"\n\nIndependent native test commands (products must first be staged at these absolute paths in the selected rootfs):\n\n```sh\n"+"\n".join(shlex.join(recipe["command"]) for recipe in native)+"\n```\n\n"+result["actual_test_method"]+"\n"
    (directory/"README.md").write_text(text);return result


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ("source","manifest","out","uwvm","wat2wasm","socket-dir","ros1-rootfs","ros2-rootfs","native-products","ros1-gateway-report","ros2-gateway-report"):
        p.add_argument("--"+name,type=Path,required=True)
    for name in ("worker-pid","worker-uid","gateway-uid"):p.add_argument("--"+name,type=int,required=True)
    args=p.parse_args();args.source=args.source.resolve(strict=True);args.out=args.out.resolve()
    if args.source!=ROOT or args.worker_pid<=0 or min(args.worker_uid,args.gateway_uid)<0:p.error("exact source and explicit valid principal required")
    if not all(path.is_absolute() for path in (args.manifest,args.uwvm,args.wat2wasm,args.socket_dir,args.ros1_rootfs,args.ros2_rootfs,args.native_products,args.ros1_gateway_report,args.ros2_gateway_report)):p.error("all caller paths must be absolute")
    args.manifest=args.manifest.resolve(strict=True)
    args.ros1_rootfs=args.ros1_rootfs.resolve(strict=True);args.ros2_rootfs=args.ros2_rootfs.resolve(strict=True)
    args.out.mkdir(mode=0o700,parents=True,exist_ok=False);sys.path.insert(0,str(args.source/"tests/ros"));sys.path.insert(0,str(args.source/"tests/ros_aarch64"))
    from runtime import read_json_binding
    from gateway_qualification import read_file_binding
    frozen,manifest_pin=read_json_binding(args.manifest)
    catalog,catalog_pin=read_json_binding(ROOT/"examples/ros/catalog.json")
    products,products_pin=read_json_binding(args.native_products.resolve(strict=True))
    renderer_pin=read_file_binding(Path(__file__).resolve(strict=True))
    input_pins=[manifest_pin,catalog_pin,products_pin,renderer_pin]
    network=load("original_material_network",args.source/"tests/ros/run_network.py");rpc=load("original_material_rpc",args.source/"tests/ros/run_rpc.py")
    input_pins.extend(read_file_binding(args.source/name) for name in ("tests/ros/run_network.py","tests/ros/run_rpc.py"))
    if [row["id"] for row in catalog["examples"]]!=list(range(1,21)):p.error("exact original twenty required")
    gateways=prepare_gateways(args,products,frozen,input_pins)
    rows=[material(args,row,network,rpc,products,gateways) for row in catalog["examples"]]
    qualified_gateways=finish_gateways(args,gateways,frozen,input_pins)
    report={"format_version":1,"kind":"original-twenty-example-materials","status":"PREPARED_NOT_EXECUTED","examples":rows,
            "source_manifest_sha256":manifest_pin["sha256"],
            "catalog":catalog_pin,"renderer":renderer_pin,"native_products_manifest":products_pin,
            "gateway_qualifications":qualified_gateways,"input_pins":input_pins,
            "source_preserved":True,"sdk_preserved":True,"sdk_root_aliases_preserved":True,"inputs_preserved":True,
            "combined_groups_examples":[15,16],"implicit_ROS_rights":[],"actual_acceptance":"NOT_EXECUTED"}
    (args.out/"materials.json").write_text(json.dumps(report,indent=2)+"\n")
    return 0
if __name__=="__main__":raise SystemExit(main())
