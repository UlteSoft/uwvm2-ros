#!/usr/bin/env python3
"""Prepare all24 bounded custom client/server Guest selections, serially.

Run only after SourceB, including the complete composite registry, has been
frozen. SourceB's deployment-final/generated/bounded output is copied byte for
byte into a task-owned generated-bounded-v2 alias beside the handwritten SDK
examples. Nothing writes the frozen source, downloaded packages or SDK roots.

The six languages, two backends and two roles are a closed inventory. Twenty
ordinary programs and two real persistent JavaScript relay Guests are compiled;
the two JavaScript roles deliberately share each backend's relay bytes but have
distinct copied host entries/role selections. Every artifact receives actual
binary signature inspection and fresh full validation. No Guest, JavaScript
application, native ROS peer, Gateway or cryptographic tool executes here.

Existing build_delivery/Lane guards provide the original64GiB, swap0, current
binfmt/cgroup incarnation, owned compiler births, deadlines and cleanup. All
compiler/stdlib/package roots are explicit and replayed. Their inventory is not
a claim of exhaustive instruction tracing or physical ARM compiler execution.
"""
from __future__ import annotations
import argparse
import json
import os
from pathlib import Path
import shutil
import sys
import traceback
from types import SimpleNamespace

sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(HERE.parents[1]/"ros_acceptance"))
from lane import Lane,add_arguments,atomic_json
from protocol import LANGUAGES,require
from runtime import digest
from build_delivery import closure,load
from gateway_qualification import architecture_binding

DRIVER="tests/ros_custom_types/bounded-v2/build_guests.py"
GENERATED="tests/ros_custom_types/deployment-final/generated/bounded"
EXAMPLES="sdk/custom-types/bounded-examples"
HOST="tests/ros_custom_types/bounded-v2/run_custom_bounded_host.mjs"
ROLES=("client","server")
SELECTIONS=tuple((language,version,role) for language in LANGUAGES for version in (1,2) for role in ROLES)
GENERATED_REQUIRED=frozenset(("bounded_interfaces.h","bounded_interfaces.hpp","bounded_records.h","bounded_records.hpp",
    "bounded_records.rs","bounded_records.go","bounded_records.mjs","bounded_records.ts","manifest.hpp","native_ros1.hpp","native_ros2.hpp","generation.json"))
REQUIRED={"client":frozenset(("capability_open","capability_close","service_call","action_send_goal","action_receive_feedback","action_receive_result")),
          "server":frozenset(("capability_open","capability_close","service_advertise","service_respond","action_advertise","action_respond","event_poll"))}


def regular_files(root):
    """Return only ordinary copied inputs; never follow an arbitrary source link."""
    root=Path(root);require(root.is_dir() and not root.is_symlink(),"ordinary owned/frozen directory required")
    result=[]
    for path in sorted(root.rglob("*")):
        require(not path.is_symlink(),"copied Guest/SDK source contains a symlink")
        if path.is_file():result.append(path)
        else:require(path.is_dir(),"unexpected Guest input kind")
    return result


def frozen_pin(lane,path):
    """Bind a selected file to its exact SourceB key, rather than a workspace alias."""
    path=Path(path).resolve(strict=True);require(path.is_relative_to(lane.source),"input escapes frozen SourceB")
    relative=path.relative_to(lane.source).as_posix();expected=lane.frozen.get(relative)
    require(isinstance(expected,dict),"selected input absent from frozen SourceB")
    pin=lane.pin(path);require(pin["sha256"]==expected["sha256"] and pin["bytes"]==expected["bytes"],"frozen selected input bytes differ")
    return pin


def copy_source_tree(lane,origin,destination):
    """Copy ordinary exact source inputs and retain source/copy associations."""
    origin=Path(origin);files=regular_files(origin);require(files,"nonempty source directory required")
    require(not destination.exists(),"disposable input destination already exists");destination.mkdir(parents=True)
    rows=[]
    for path in files:
        relative=path.relative_to(origin);source=frozen_pin(lane,path);target=destination/relative;target.parent.mkdir(parents=True,exist_ok=True)
        shutil.copy2(path,target);copy=lane.pin(target)
        require((copy["sha256"],copy["bytes"])==(source["sha256"],source["bytes"]),"copied Guest input differs")
        rows.append({"source":source,"copy":copy})
    return rows


def same_content_inventory(root):
    """Bound the disposable source namespace; caches/products live elsewhere."""
    return {str(path):{"sha256":digest(path),"bytes":path.stat().st_size} for path in regular_files(root)}


def copied_inputs(lane):
    """Create one exact SDK/generated/example view plus separate Go role modules."""
    work=lane.out/"work";work.mkdir();associations=[]
    # Public include/roswasm forwarding headers retain relative references to
    # sdk/ros1, sdk/ros2 and sdk/common. Preserve these complete ABI roots in the
    # isolated build view, including the same dependencies used by bridge.c.
    # Every additional copied file follows the existing source/copy pin checks.
    for name in ("common","ros1","ros2","include","rust","tinygo","assemblyscript","javascript"):
        associations.extend(copy_source_tree(lane,lane.source/"sdk"/name,work/"sdk"/name))
    license_source=frozen_pin(lane,lane.source/"sdk/LICENSE");shutil.copy2(lane.source/"sdk/LICENSE",work/"sdk/LICENSE")
    associations.append({"source":license_source,"copy":lane.pin(work/"sdk/LICENSE")})
    associations.extend(copy_source_tree(lane,lane.source/EXAMPLES,work/EXAMPLES))
    generated=lane.source/GENERATED;names={p.name for p in regular_files(generated)}
    require(GENERATED_REQUIRED<=names,"complete frozen bounded output missing")
    associations.extend(copy_source_tree(lane,generated,work/"sdk/custom-types/generated-bounded-v2"))
    host_source=frozen_pin(lane,lane.source/HOST);host=work/HOST;host.parent.mkdir(parents=True);shutil.copy2(lane.source/HOST,host)
    associations.append({"source":host_source,"copy":lane.pin(host)})
    # Cargo compiles real std-WASI main programs. Both binary sources remain in
    # the same copied layout, preserving their generated-code relative imports.
    rust=work/EXAMPLES;manifest=rust/"Cargo.toml";require(not manifest.exists(),"handwritten temporary Cargo manifest collides")
    manifest.write_text('[package]\nname="roswasm-bounded-custom-guest"\nversion="0.1.0"\nedition="2021"\npublish=false\n'
        '[dependencies]\nroswasm={path="../../rust",default-features=false,features=["alloc"]}\n'
        '[features]\ndefault=[]\nros1=["roswasm/ros1"]\nros2=["roswasm/ros2"]\n'
        '[[bin]]\nname="client"\npath="client.rs"\n[[bin]]\nname="server"\npath="server.rs"\n')
    lane.pin(manifest)
    for role in ROLES:
        module=work/"go"/role;module.mkdir(parents=True)
        sdk_go=(work/"sdk/tinygo").resolve();require('"' not in str(sdk_go) and "\n" not in str(sdk_go),"Go replacement path contains syntax")
        (module/"go.mod").write_text('module example.local/bounded\n\ngo 1.23\nrequire uwvm2.local/roswasm v0.0.0\nreplace uwvm2.local/roswasm => '+json.dumps(str(sdk_go))+'\n')
        for name,target in ((role+".go","main.go"),("aliases_ros1.go","aliases_ros1.go"),("aliases_ros2.go","aliases_ros2.go")):
            origin=work/EXAMPLES/name;require(origin.is_file(),"complete selected Go role missing")
            shutil.copy2(origin,module/target);lane.pin(module/target)
        (module/"customtypes").mkdir();shutil.copy2(work/"sdk/custom-types/generated-bounded-v2/bounded_records.go",module/"customtypes/bounded_records.go")
        lane.pin(module/"go.mod");lane.pin(module/"customtypes/bounded_records.go")
    return work,associations,same_content_inventory(work)


def recipe(lane,args,work,language,version,role,symbols,guards):
    """Closed serial recipes, with CRT main and freestanding relay kept distinct."""
    rowdir=lane.out/(language+"-ros"+str(version)+"-"+role);rowdir.mkdir();artifact=rowdir/"guest.wasm";copied=None;cwd=work;archive=None;rust_flags=None
    env=dict(lane.clean);env.update(HOME=str(lane.out/"home"),XDG_CACHE_HOME=str(lane.out/"cache"),
        CARGO_HOME=str(lane.out/"cargo-home"),CARGO_NET_OFFLINE="true",RUSTUP_AUTO_INSTALL="0",CARGO_BUILD_JOBS="1",
        RUSTC_WRAPPER="",RUSTC_WORKSPACE_WRAPPER="",CARGO_BUILD_RUSTC_WRAPPER="",CARGO_BUILD_RUSTC_WORKSPACE_WRAPPER="",
        GOCACHE=str(lane.out/"go-cache"),GOMODCACHE=str(lane.out/"go-mod-cache"),GOPROXY="off",GOSUMDB="off",GOENV="off",GOTOOLCHAIN="local",GOMAXPROCS="1",GOFLAGS="")
    base=work/EXAMPLES
    if language in ("c","cpp"):
        source=base/(role+(".c" if language=="c" else ".cpp"));tool=args.wasi_sdk/("bin/clang" if language=="c" else "bin/clang++")
        argv=[str(tool),"--target=wasm32-wasip1","--sysroot="+str(args.wasi_sdk/"share/wasi-sysroot"),
            "-std="+("c11" if language=="c" else "c++20"),"-O0","-g0","-Wl,--threads=1","-Wl,--export-dynamic",
            "-Wl,--allow-undefined-file="+str(symbols),"-I",str(work/"sdk/include"),"-DUWVM_BOUNDED_ROS_VERSION="+str(version),str(source),"-o",str(artifact)]
        if language=="cpp":argv[4:4]=["-stdlib=libc++","-fno-exceptions","-fno-rtti"]
    elif language=="rust":
        source=base/(role+".rs");target=rowdir/"cargo-target";copied=target/"wasm32-wasip1/release"/(role+".wasm")
        rust_flags,archive=guards.rust_wasi_link_inputs(args.wasi_sdk,args.rust_sysroot)
        env.update(RUSTC=str(args.rustc),RUSTFLAGS="",CARGO_ENCODED_RUSTFLAGS="\x1f".join(rust_flags))
        argv=[str(args.cargo),"build","--offline","-j1","--release","--manifest-path",str(base/"Cargo.toml"),"--target","wasm32-wasip1",
            "--target-dir",str(target),"--no-default-features","--features","ros"+str(version),"--bin",role]
    elif language=="tinygo":
        # Go's heap budget does not include native LLVM memory; preserve the
        # separate aggregate compiler RSS guard and the existing -p1/-opt0.
        cwd=work/"go"/role;source=cwd/"main.go";env.update(PATH=str(args.go.parent)+os.pathsep+os.defpath,GOROOT=str(args.go_root),TINYGOROOT=str(args.tinygo.resolve().parent.parent),GOWORK="off",GOFLAGS="-mod=readonly",GOMEMLIMIT="192MiB")
        argv=[str(args.tinygo),"build","-p","1","-opt=0","-target=wasi","-scheduler=none","-tags=ros"+str(version),"-o",str(artifact),"."]
    elif language=="assemblyscript":
        source=base/(role+"_ros"+str(version)+".ts")
        argv=[str(args.node),str(args.asc_js),str(source),"--runtime","incremental","--exportRuntime","--use","abort=sdk/assemblyscript/api/abortGuest","--outFile",str(artifact)]
    else:
        source=work/"sdk/javascript/relay.c"
        argv=[str(args.wasi_sdk/"bin/clang"),"--target=wasm32","-std=c11","-O0","-g0","-nostdlib","-ffreestanding","-fno-builtin",
            "-DROSWASM_BRIDGE_ROS_VERSION="+str(version),"-Wl,--entry=_start","-Wl,--export=_start","-Wl,--export-memory","-Wl,--threads=1",
            "-Wl,--initial-memory=131072","-Wl,--max-memory=131072","-Wl,-z,stack-size=65536","-Wl,--allow-undefined-file="+str(symbols),str(source),"-o",str(artifact)]
    require(source.is_file(),"selected copied Guest source missing")
    return dict(source=source,argv=argv,cwd=cwd,environment=env,rowdir=rowdir,artifact=artifact,copied=copied,
        rust_target_libc=archive,explicit_rustc_tokens=rust_flags)


def role_imports(contract,language,version,role):
    """Require actual selected-role imports, in addition to every exact signature."""
    module="uwvm_ros"+str(version)+"_v1";rows=contract["imports"]
    require(all(row["kind"]==0 and row["module"] in (module,"wasi_snapshot_preview1") for row in rows),"non-function/foreign Guest import refused")
    found={row["name"] for row in rows if row["module"]==module};required=REQUIRED[role]
    if language!="tinygo":required=required|{"abi_version"}
    require(required<=found,"selected role's real imports are missing")
    return sorted(required)


def scope_matches(command,before):
    """Check exact current incarnation and both command OOM/cleanup endpoints."""
    if not isinstance(before.get("path"),str) or not before["path"] or not before.get("boot_id") or any(type(before.get(key)) is not int or before[key]<=0 for key in ("device","inode")):return False
    if type(command.get("returncode")) is not int or command["returncode"]!=0 or command.get("guard_error") is not None:return False
    tool=command.get("tool");held=command.get("held_launch")
    if not isinstance(tool,dict) or not tool.get("sha256") or not isinstance(held,dict) or any(type(held.get(key)) is not int or held[key]<=0 for key in ("pid","birth")):return False
    if command.get("cleanup",{}).get("status")!="PASS" or tool!=command.get("tool_after"):return False
    for side in ("before","after"):
        state=command.get(side,{})
        if state.get("leaf")!=before.get("path") or any(state.get(key)!=before.get(key) for key in ("device","inode","boot_id")):return False
        if state.get("leaf_memory_max")!=68719476736 or state.get("swap_max")!="0":return False
    delta=command.get("memory_events_delta",{})
    return isinstance(delta,dict) and all(type(value) is int and value>=0 for value in delta.values()) and not any(delta.get(key,0) for key in ("oom","oom_kill","oom_group_kill"))


def preserved_pins(lane,pins):
    """Replay each exact opened input once; conflicting repeated paths fail.

    Every selected product uses the same large compilers and copied SDK tree.
    Deduplication avoids redundant tool reads without treating two different
    recorded identities of a single path as an interchangeable input.
    """
    unique={}
    for pin in pins:
        require(isinstance(pin,dict) and isinstance(pin.get("path"),str),"ordinary recorded input pin required")
        previous=unique.setdefault(pin["path"],pin);require(previous==pin,"one input path has conflicting recorded identities")
    for pin in unique.values():require(lane.pin(pin["path"])==pin,"prepared compiler/source/log/product input changed")


def qualify_product(lane,report_path,language,version,role):
    """Read-only admission of a prepared actual product; returns path and rights.

    The execute controller must still copy/map the Worker and real Native roles,
    authenticate policy, prove independent exact effects and bounded cleanup.
    This helper never turns compiler evidence into behavior acceptance.
    """
    require(type(version) is int and (language,version,role) in SELECTIONS,"closed positive selection required")
    report,pin=lane.json(report_path)
    require(report.get("kind")=="bounded-custom-guest-products" and report.get("status")=="PREPARED" and
        report.get("source")==str(lane.source) and report.get("source_manifest_sha256")==lane.report["source_manifest_sha256"],"fresh exact SourceB product report required")
    require(all(report.get(key) is True for key in ("source_preserved","sdk_preserved","inputs_preserved","products_preserved","all_compilation_serial")),"preparation preservation/serial proof missing")
    require(report.get("remaining_owned_mounts")==[] and report.get("cleanup_errors")==[],"preparation mount/cleanup proof missing")
    require(type(report.get("compiler_invocations")) is int and report["compiler_invocations"]==22 and type(report.get("full_validation_invocations")) is int and report["full_validation_invocations"]==24 and type(report.get("prepared_selections")) is int and report["prepared_selections"]==24,"exact actual build/validation inventory required")
    architecture,architecture_pin=architecture_binding(report["architecture_evidence"]["path"],lane.pins)
    require(architecture_pin==report["architecture_evidence"] and architecture["cgroup_incarnation_before"]==lane.before,"preparation architecture/current incarnation differs")
    rows=report.get("products",[]);require(len(rows)==24 and {(r.get("language"),r.get("version"),r.get("role")) for r in rows}==set(SELECTIONS) and all(type(r.get("version")) is int and r.get("status")=="PREPARED" for r in rows),"complete closed24 preparation required")
    require(sum(r.get("shared_relay_compile") is True for r in rows)==2 and all(r.get("shared_relay_compile") is True for r in rows if r["language"]=="javascript" and r["role"]=="server"),"exactly two JavaScript server roles share genuine client relay builds")
    require(len({(r["compile"]["held_launch"]["pid"],r["compile"]["held_launch"]["birth"]) for r in rows})==22 and
        len({(r["validation"]["held_launch"]["pid"],r["validation"]["held_launch"]["birth"]) for r in rows})==24,"actual original compiler/validator birth inventory differs")
    selected=[r for r in rows if (r["language"],r["version"],r["role"])==(language,version,role)];require(len(selected)==1,"one exact role selection required");row=selected[0]
    controls=list(report["input_pins"])
    # Recompute the complete24-row claimed inventory, not just a selected
    # product's PREPARED label. Raw command/log and shared-build associations
    # remain explicit; only the two JavaScript server rows share real builds.
    for prepared in rows:
        require(prepared.get("source_inputs_preserved") is True and prepared.get("signature_status")=="PASS" and
            all(type(prepared.get(key)) is int and prepared[key]==0 for key in ("compiler_returncode","full_validation_returncode")) and
            scope_matches(prepared["compile"],lane.before) and scope_matches(prepared["validation"],lane.before),"current actual compile/validation/cleanup proof missing")
        require(prepared["compile"]["log"]==prepared["compile_log"]["path"] and prepared["validation"]["log"]==prepared["validation_log"]["path"],"raw command/log association differs")
        require(prepared["validation"]["argv"][1:]==["validate",prepared["artifact"]["path"]],"fresh full validator did not select the exact artifact")
        controls.extend(prepared["input_pins"]+[prepared[key] for key in ("compile_log","validation_log","artifact","source")])
        if "shared_relay_compile" not in prepared:
            require(prepared["compile"]["argv"]==prepared["recipe_argv"] and prepared["compile"]["cwd"]==prepared["working_directory"],"original successful source recipe differs")
        else:
            require(prepared["language"]=="javascript" and prepared["role"]=="server" and prepared["shared_relay_compile"] is True and prepared.get("compile_source_role")=="client","only the server JavaScript selection may share a compile")
            original=next(r for r in rows if (r["language"],r["version"],r["role"])==("javascript",prepared["version"],"client"))
            origin=prepared["compiled_artifact_origin"]
            require(origin==original["artifact"] and prepared["compile"]==original["compile"] and prepared["compile_log"]==original["compile_log"] and
                (origin["sha256"],origin["bytes"])==(prepared["artifact"]["sha256"],prepared["artifact"]["bytes"]),"shared real relay origin differs")
            require(prepared["working_directory"]==original["working_directory"] and prepared["recipe_argv"]==
                [prepared["artifact"]["path"] if token==origin["path"] else token for token in original["recipe_argv"]],"shared relay source recipe differs")
        if prepared["language"]=="javascript":
            syntax=prepared.get("js_syntax",[]);require(len(syntax)==2 and prepared.get("js_host_execution")=="NOT_EXECUTED","exact syntax-only host/application preparation required")
            require(prepared["js_host_argv"][1:]==[prepared["js_host_entry"]["path"],str(prepared["version"]),prepared["role"]] and
                all(type(prepared["js_control_fds"].get(key)) is int and prepared["js_control_fds"][key]==value for key,value in (("read",3),("write",4))),"actual copied host selection/control descriptors differ")
            for check,source in zip(syntax,(prepared["js_host_entry"],prepared["js_application"])):
                require(scope_matches(check["command"],lane.before) and check["command"]["argv"]==[prepared["js_host_argv"][0],"--check",source["path"]] and
                    check["command"]["log"]==check["log"]["path"],"actual selected host/application syntax source differs")
                controls.extend((check["log"],source))
    preserved_pins(lane,controls)
    for association in report["copied_input_associations"]:
        require(frozen_pin(lane,association["source"]["path"])==association["source"] and
            (association["source"]["sha256"],association["source"]["bytes"])==(association["copy"]["sha256"],association["copy"]["bytes"]),"copied input has no exact SourceB association")
    final=report["guest_work_inputs_final"];work=Path(report_path).resolve(strict=True).parent/"work"
    require(same_content_inventory(work)=={path:{"sha256":pin["sha256"],"bytes":pin["bytes"]} for path,pin in final.items()},"disposable copied input namespace changed")
    packages,package_pin=lane.json(report["toolchain_packages"]["path"]);require(package_pin==report["toolchain_packages"],"whole tool inventory changed")
    require({root:closure(Path(root)) for root in report["toolchain_root_selections"]}==packages,"compiler/package/stdlib bytes changed")
    inspector=load(lane.source,"sdk/tools/check_wasm_imports.py","bounded_qualify_imports")
    guards=load(lane.source,"sdk/tools/build_cgroup_matrix.py","bounded_qualify_guards")
    data,descriptor=guards.read_artifact(Path(row["artifact"]["path"]));require(descriptor["sha256"]==row["artifact"]["sha256"],"opened product bytes changed")
    contract=inspector.check(data,str(version),("wasi_snapshot_preview1",),language=="javascript")
    require(contract==row["import_contract"] and role_imports(contract,language,version,role)==row["required_role_imports"],"replayed exact import contract differs")
    if language=="javascript":guards.check_relay_imports(contract);require(lane.pin(row["js_host_entry"]["path"])==row["js_host_entry"],"copied actual JavaScript host changed")
    rights=row.get("explicit_wasi_startup_rights")
    require(isinstance(rights,list) and len(rights)==len(set(rights)) and all(right in ("random","monotonic-clock") for right in rights) and
        (language=="tinygo" or not rights) and (not rights or row.get("startup_rights_reviewed") is True),"explicit startup-right selection differs")
    lane.report.setdefault("bounded_guest_qualifications",[]).append({"manifest":pin,"selection":row,"behavior":"NOT_EXECUTED_BY_QUALIFIER"});lane.save()
    return Path(row["artifact"]["path"]),rights


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser,workers=False)
    for name in ("wasi-sdk","cargo","rustc","rust-sysroot","tinygo","go","go-root","node","asc-js","wasm-tools"):parser.add_argument("--"+name,type=Path,required=True)
    parser.add_argument("--toolchain-root",type=Path,action="append",required=True,help="Complete immutable compiler/package/stdlib roots; no escaping links")
    parser.add_argument("--tinygo-startup-right",choices=("random","monotonic-clock"),action="append",default=[],help="Explicit independently reviewed right, never inferred or applied by preparation")
    parser.add_argument("--rss-limit-mib",type=int,default=512,help="Serial owned compiler aggregate RSS guard, 128..2048MiB")
    parser.add_argument("--timeout-seconds",type=int,default=600,help="Bounded compile/validation deadline, 30..1800seconds")
    args=parser.parse_args();require(128<=args.rss_limit_mib<=2048 and 30<=args.timeout_seconds<=1800,"explicit finite tool budgets required")
    require(len(args.tinygo_startup_right)==len(set(args.tinygo_startup_right)),"duplicate explicit startup rights")
    lane=None;status="FAIL";current=None
    try:
        lane=Lane(args,"bounded-custom-guest-products");lane.report.update(mode="prepare",actual_ROS_delivery="NOT_EXECUTED",compiler_and_validation_scope="Guest product preparation only")
        require(Path(__file__).resolve()==lane.source/DRIVER,"controller must execute from selected frozen SourceB")
        driver=frozen_pin(lane,lane.source/DRIVER)
        for name in ("wasi_sdk","cargo","rustc","rust_sysroot","tinygo","go","go_root","node","asc_js","wasm_tools"):
            raw=getattr(args,name);require(raw.is_absolute(),"absolute selected tool/package path required");setattr(args,name,raw.resolve(strict=True))
        roots=[path.resolve(strict=True) for path in args.toolchain_root];require(len(roots)==len(set(roots)),"duplicate selected package roots")
        packages={str(path):closure(path) for path in roots};package_path=lane.out/"toolchain-inputs.json";atomic_json(package_path,packages)
        lane.report.update(toolchain_packages=lane.pin(package_path),toolchain_root_selections=[str(path) for path in roots],driver_input=driver)
        tools=[args.wasi_sdk/"bin/clang",args.wasi_sdk/"bin/clang++",args.cargo,args.rustc,args.tinygo,args.go,args.node,args.asc_js,args.wasm_tools]
        for path in [args.wasi_sdk,args.rust_sysroot,args.go_root,*tools]:require(any(path.resolve(strict=True).is_relative_to(root) for root in roots),"tool or stdlib lacks complete package inventory")
        tool_pins=[lane.pin(path.resolve(strict=True)) for path in tools]
        compiler=load(lane.source,"sdk/tools/build_cgroup_matrix.py","bounded_guest_guards");inspector=load(lane.source,"sdk/tools/check_wasm_imports.py","bounded_guest_imports")
        controls=SimpleNamespace(expected_memory_max=68719476736,expected_swap_max="0",expected_leaf=lane.evidence.leaf,
            expected_device=lane.before["device"],expected_inode=lane.before["inode"],expected_boot=lane.before["boot_id"],
            expected_cpus=(lane.evidence.leaf/"cpuset.cpus.effective").read_text().strip(),rss_limit_mib=args.rss_limit_mib,cgroup_current_limit_mib=61440,timeout_seconds=args.timeout_seconds)
        work,associations,baseline=copied_inputs(lane);input_path=lane.out/"guest-inputs.json"
        atomic_json(input_path,{"source_associations":associations,"temporary_input_inventory":baseline,"generated_source":GENERATED,"owned_generated_alias":"sdk/custom-types/generated-bounded-v2"})
        lane.report["guest_inputs"]=lane.pin(input_path);lane.report["copied_input_associations"]=associations
        schema_path=lane.source/"sdk/abi/abi-v1.json";schema_pin=frozen_pin(lane,schema_path);schema=json.loads(schema_path.read_text());symbols={}
        for version in (1,2):
            path=lane.out/("allowed-ros"+str(version)+".txt");path.write_text("\n".join("uwvm_ros"+str(version)+"_"+r["name"] for r in schema["modules"]["uwvm_ros"+str(version)+"_v1"]["functions"])+"\n");symbols[version]=path;lane.pin(path)
        products=[];lane.report["products"]=products;relays={};compiler_count=0
        for language,version,role in SELECTIONS:
            current={"language":language,"version":version,"role":role,"status":"RUNNING","actual_ROS_delivery":"NOT_EXECUTED"};products.append(current);lane.save()
            item=recipe(lane,args,work,language,version,role,symbols[version],compiler);artifact=item["artifact"]
            current.update(source=lane.pin(item["source"]),recipe_argv=item["argv"],working_directory=str(item["cwd"]))
            if language=="rust":
                require(lane.pin(item["rust_target_libc"]["path"])==item["rust_target_libc"],"selected Rust target libc changed")
                current.update(rust_target_libc=item["rust_target_libc"],explicit_rustc_tokens=item["explicit_rustc_tokens"])
            if language=="javascript" and version in relays:
                previous=relays[version];shutil.copy2(previous["artifact"]["path"],artifact);actual=previous["compile"]
                current.update(shared_relay_compile=True,compiled_artifact_origin=previous["artifact"],compile_source_role="client")
            else:
                actual=compiler.guarded_command(item["argv"],item["cwd"],item["environment"],item["rowdir"],"compile",controls);compiler_count+=1
                current["compile"]=actual;current["compile_log"]=lane.pin(actual["log"]);lane.save()
                require(scope_matches(actual,lane.before),"actual serial Guest compilation or guard failed")
                if item["copied"]:
                    require(item["copied"].is_file() and not item["copied"].is_symlink(),"Cargo output is not an ordinary product");shutil.copy2(item["copied"],artifact)
            current["compile"]=actual;current["compile_log"]=lane.pin(actual["log"])
            require(not artifact.is_symlink(),"produced Guest must be ordinary");data,opened=compiler.read_artifact(artifact);before=lane.pin(artifact);require(opened["sha256"]==before["sha256"],"opened compiler artifact differs")
            contract=inspector.check(data,str(version),("wasi_snapshot_preview1",),language=="javascript");required=role_imports(contract,language,version,role)
            if language=="javascript":compiler.check_relay_imports(contract)
            validation=compiler.guarded_command([str(args.wasm_tools),"validate",str(artifact)],item["rowdir"],item["environment"],item["rowdir"],"validate",controls)
            current["validation"]=validation;current["validation_log"]=lane.pin(validation["log"]);lane.save()
            require(scope_matches(validation,lane.before) and lane.pin(artifact)==before,"fresh full validation or exact product binding failed")
            if language=="rust":
                _,current["rust_target_libc_after"]=compiler.rust_wasi_link_inputs(args.wasi_sdk)
                require(current["rust_target_libc_after"]==current["rust_target_libc"],"Rust target libc changed around compilation/validation")
            now=same_content_inventory(work);added=set(now)-set(baseline);lock=str(work/EXAMPLES/"Cargo.lock")
            require(added<={lock} and all(now.get(path)==content for path,content in baseline.items()),"copied SDK/generated/Guest inputs changed")
            if lock in added:lane.pin(lock)
            rights=list(args.tinygo_startup_right) if language=="tinygo" else []
            current.update(status="PREPARED",artifact=before,compiler_returncode=0,full_validation_returncode=0,signature_status="PASS",import_contract=contract,
                required_role_imports=required,source_inputs_preserved=True,input_pins=[schema_pin,driver,*tool_pins,lane.report["toolchain_packages"],lane.report["guest_inputs"],*([lane.pin(lock)] if lock in added else []),
                    *([current["rust_target_libc"]] if language=="rust" else [])],
                explicit_wasi_startup_rights=rights,startup_rights_reviewed=bool(rights),toolchain_sdk_closure="Whole selected package/stdlib inventories pinned; tool-child map coverage remains sampled")
            if language=="javascript":
                current.update(js_host_entry=lane.pin(work/HOST),js_application=lane.pin(work/EXAMPLES/(role+".mjs")),
                    js_host_argv=[str(args.node),str(work/HOST),str(version),role],js_control_fds={"read":3,"write":4},js_host_execution="NOT_EXECUTED")
                current["js_syntax"]=[]
                for label,source in (("host",work/HOST),("application",work/EXAMPLES/(role+".mjs"))):
                    command=compiler.guarded_command([str(args.node),"--check",str(source)],item["rowdir"],item["environment"],item["rowdir"],"syntax-"+label,controls)
                    current["js_syntax"].append({"command":command,"log":lane.pin(command["log"])});lane.save()
                    require(scope_matches(command,lane.before),"actual selected JavaScript syntax check failed")
                relays.setdefault(version,current)
            lane.save();current=None
        require(len(products)==24 and all(r["status"]=="PREPARED" for r in products) and compiler_count==22,"closed24 role preparation incomplete")
        final=same_content_inventory(work);require(all(final.get(path)==content for path,content in baseline.items()) and set(final)-set(baseline)<={str(work/EXAMPLES/"Cargo.lock")},"final copied namespace changed")
        require({str(root):closure(root) for root in roots}==packages,"compiler/package/stdlib inputs changed during preparation")
        lane.report.update(compiler_invocations=compiler_count,full_validation_invocations=24,prepared_selections=24,all_compilation_serial=True,
            source_inputs_preserved=True,guest_work_inputs_final={path:lane.pin(path) for path in final},actual_ROS_delivery="NOT_EXECUTED");status="PREPARED"
    except BaseException as error:
        if lane is None:raise
        if current is not None:
            current["status"]="FAIL"
            if hasattr(error,"capsule"):current["fatal_command_guard"]=error.capsule
        lane.report.update(error=repr(error),traceback=traceback.format_exc())
    finally:
        if lane is not None:
            result=lane.close("PASS" if status=="PREPARED" else "FAIL");status="PREPARED" if status=="PREPARED" and result=="PASS" else "FAIL"
            lane.report["status"]=status;lane.save()
    return 0 if status=="PREPARED" else 1


if __name__=="__main__":raise SystemExit(main())
