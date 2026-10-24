#!/usr/bin/env python3
"""Run real six-language ROS delivery on an independently selected Runtime ELF.

Each backend/profile runs twelve String cases and one seven-event exchange.
There is no fake Runtime/Instance/Host or native RPC substitute. Only the
existing finite production policies and independent native effect oracles are
reused. AArch64 Gateway/native code is never described as LoongArch native DDS.
"""
import argparse
import fcntl
import json
import os
from pathlib import Path
import re
import signal
import socket
import stat
import subprocess
import sys
import tempfile
import time
import traceback
import xmlrpc.client

sys.dont_write_bytecode=True
from profile import validate, require, SDK_CASES, available
from engine import Frame, environment, executable, argv, elf, load, json_input


class Adapter:
    """Data-only API expected by existing Guest admission and policy helpers."""
    def __init__(self, frame, out, short, worker):
        self.frame,self.source,self.out,self.short,self.worker=frame,frame.source,out,short,worker
        self.report=frame.report

    def pin(self,path):
        return self.frame.pin(path)

    def json(self,path):
        return self.frame.json(path)


def translate(frame,runner,out,name,text):
    wat=out/(name+".wat");wasm=out/(name+".wasm");wat.write_text(text)
    tool=Path(frame.profile["wat2wasm"]).resolve(strict=True);validator=Path(frame.profile["wasm_validate"]).resolve(strict=True)
    frame.pin(tool);frame.pin(validator)
    prior=runner.owner.limits
    runner.owner.limits={**prior,"rss_bytes":frame.profile["limits"]["compiler_rss"]}
    try:
        runner.owner.run(name+"-translate",[str(tool),"--enable-multi-memory",str(wat),"-o",str(wasm)],out,environment())
        runner.owner.run(name+"-validate",[str(validator),"--enable-multi-memory",str(wasm)],out,environment())
    finally:
        runner.owner.limits=prior
    frame.pin(wat);frame.pin(wasm);require(wasm.read_bytes()[:8]==b"\0asm\1\0\0\0","real validated Wasm v1 required")
    return wasm


def native_env(frame,out,events=False):
    value=environment(frame.profile["native"]);value["ROS_LOG_DIR"]=str(out/"ros-logs")
    if frame.profile["backend"]==1:
        value.update(ROS_MASTER_URI="http://127.0.0.1:"+("11386" if events else "11384"),ROS_IP="127.0.0.1")
    else:
        value.update(ROS_DOMAIN_ID="206" if events else "205",RMW_IMPLEMENTATION="rmw_fastrtps_cpp",ROS_SECURITY_ENABLE="false")
        value.pop("ROS_AUTOMATIC_DISCOVERY_RANGE",None)
    return value


def root_lock(frame,events):
    label=("master-"+("11386" if events else "11384")) if frame.profile["backend"]==1 else "domain-"+("206" if events else "205")
    # Share the existing acceptance context lease, not merely a new label for
    # the same global Master port or DDS domain.
    path=Path("/tmp")/("uwvm-ros-remaining-"+label+".lock")
    fd=os.open(path,os.O_RDWR|os.O_CREAT|os.O_CLOEXEC|os.O_NOFOLLOW,0o600)
    try:
        info=os.fstat(fd);require(stat.S_ISREG(info.st_mode) and info.st_uid==os.geteuid(),"private context lease invalid")
        fcntl.flock(fd,fcntl.LOCK_EX|fcntl.LOCK_NB);return fd
    except BaseException:
        os.close(fd);raise


class MasterTransport(xmlrpc.client.Transport):
    def make_connection(self,host):
        connection=super().make_connection(host);connection.timeout=.25;return connection


def master(frame,runner,out,events):
    """Own the exact private Master, never connect a case to a foreign daemon."""
    if frame.profile["backend"]!=1:
        return
    port=11386 if events else 11384
    try:
        with socket.create_connection(("127.0.0.1",port),.2):
            raise ValueError("BLOCKED: selected private Master port is already occupied")
    except ConnectionRefusedError:
        pass
    selected=frame.profile["master"];python=Path(selected["python"]).resolve(strict=True)
    frame.pin(python);frame.pin(selected["script"])
    image=executable(python,frame.profile["native"],frame.common)
    child,row=runner.start("master",argv(frame.profile["native"],python,[selected["script"],"-p",str(port)]),native_env(frame,out,events))
    until=time.monotonic()+30
    while time.monotonic()<until:
        runner.check();require(child.poll() is None,"owned Master retired")
        try:
            with xmlrpc.client.ServerProxy("http://127.0.0.1:"+str(port),transport=MasterTransport()) as client:
                result=client.getPid("/uwvm_multiarch_controller")
            if result[0]==1:
                runner.owner.discover();records=[x for x in runner.owner.owned.values() if x["identity"]["pid"]==result[2]]
                require(len(records)==1,"XMLRPC Master PID is not an owned descendant")
                current=frame.common.identity(result[2]);require(frame.common.birth(current)==frame.common.birth(records[0]["identity"]),"Master birth differs")
                actual=frame.common.opened((Path("/proc")/str(result[2])/"exe").resolve(strict=True))
                require(actual==image.get("emulator",image)["input"],"actual owned Master interpreter differs")
                row["readiness"]={"pid_result":result,"actual_exe":actual,"selected_target":image,
                    "scope":"Owned auxiliary Master; not a Runtime/native endpoint pre-main barrier"};return
        except (OSError,xmlrpc.client.Error):
            pass
        time.sleep(.02)
    raise TimeoutError("owned Master readiness deadline")


def product(frame,prepared,name,descriptor):
    row=prepared["worker"] if name=="worker" else prepared["native_products"][name]
    image=frame.image(row["image"]["input"]["path"],descriptor)
    require(image==row["image"],"prepared product bytes/ELF/loader/QEMU changed")
    return Path(image["input"]["path"])


def start_case(frame,prepared,runner,out,delivery,fixtures,events_module,case):
    version=frame.profile["backend"];events=case=="seven-events"
    short=Path(tempfile.mkdtemp(prefix="uwvm-ma-",dir="/tmp"));short.chmod(0o700)
    frame.report.setdefault("socket_directories",[]).append(str(short))
    lock=None
    try:
        worker_binary=product(frame,prepared,"worker",frame.profile["runtime"])
        gateway_binary=product(frame,prepared,"gateway",frame.profile["native"])
        peer_binary=product(frame,prepared,"events-peer" if events else "sdk-peer",frame.profile["native"])
        lane=Adapter(frame,out,short,worker_binary)
        language,mode=(None,"positive") if events else tuple(case[4:].rsplit("-",1))
        sdk_wasm=None;rights=[]
        if events:
            selected=events_module.selection(version)
            event_wat=load(frame.source/"test/0073.ros-tests/support/source/ros_acceptance/event_fixtures.py","uwvm_multiarch_event_wat")
            wasm=translate(frame,runner,out,"events",event_wat.seven_event_guest(version))
        else:
            selected=delivery.policy(version,mode=="absent")
            sdk_wasm,rights=delivery.compiled_guest(lane,Path(frame.profile["guests"]),language,version,mode)
            wasm=translate(frame,runner,out,"sdk-gate",fixtures.sdk_gate())
        lock=root_lock(frame,events);frame.context_locks.append(lock)
        master(frame,runner,out,events)
        peer,peer_row=runner.start("peer",argv(frame.profile["native"],peer_binary,[] if events else [mode]),native_env(frame,out,events))
        proof=runner.stopped(peer,peer_row,frame.profile["native"],peer_binary);native_pid=proof["birth"]["pid"]
        runner.send(peer,b"I");runner.wait(peer,peer_row,"READY_NATIVE_SEVEN_EVENTS" if events else "READY_SDK_PEER")
        command=delivery.worker_argv(lane,(version,),wasm,sdk_wasm,rights,mode=="absent",events,selections={version:selected})
        worker_command=command[:]
        worker,worker_row=runner.start("worker",argv(frame.profile["runtime"],worker_binary,command[1:]),environment(frame.profile["runtime"]),stdout_pipe=language=="javascript")
        # Exact target image is already mapped by its real pre-main barrier.
        # Keep it stopped until the Gateway has captured this actual principal.
        until=time.monotonic()+20
        while time.monotonic()<until:
            runner.check();state=frame.common.identity(worker.pid)
            if state and state["state"] in ("T","t"):
                break
            require(worker.poll() is None,"Worker retired before pre-main stop");time.sleep(.002)
        else:
            raise TimeoutError("Worker pre-main stop missing")
        config=short/("ros"+str(version)+".conf")
        original=delivery.gateway_config(version,worker.pid,mode=="absent",selected)
        text=original.replace("socket /guest/ros"+str(version)+".sock", "socket "+str(short/("ros"+str(version)+".sock")))
        require(text!=original,"one explicit physical socket substitution required")
        config.write_text(text);config.chmod(0o600)
        gateway,gateway_row=runner.start("gateway",argv(frame.profile["native"],gateway_binary,[str(config)]),native_env(frame,out,events))
        runner.stopped(gateway,gateway_row,frame.profile["native"],gateway_binary)
        target=short/("ros"+str(version)+".sock");until=time.monotonic()+30
        while not target.exists() and time.monotonic()<until:
            runner.check();require(gateway.poll() is None,"Gateway retired before owned socket");time.sleep(.01)
        require(target.exists() and stat.S_ISSOCK(target.lstat().st_mode) and target.lstat().st_uid==os.geteuid(),"owned Gateway socket absent")
        info=target.lstat();frame.socket_bindings.append({"path":str(target),"device":info.st_dev,"inode":info.st_ino,"uid":info.st_uid})
        config_pin=frame.pin(config)
        runner.stopped(worker,worker_row,frame.profile["runtime"],worker_binary)
        runner.send(worker,b"G")
        sdk,srow=worker,worker_row
        if language=="javascript":
            node=Path(frame.profile["node"]).resolve(strict=True);frame.pin(node)
            require(elf(node,frame.common)["machine"]==62,"JS host is separately recorded actual host Node")
            host=frame.source/"sdk/origin/examples/javascript/05-runtime-relay.mjs";frame.pin(host)
            shell=Path("/bin/bash").resolve(strict=True);frame.pin(shell)
            command=[str(shell),"--noprofile","--norc","-c",'exec 3<&"$1" 4>&"$2"; shift 2; exec "$@"',"multiarch-js-pipes",
                     str(worker.stdout.fileno()),str(worker.stdin.fileno()),str(node),str(host),str(version),mode]
            sdk,srow=runner.start("javascript-host",command,environment(),pass_fds=(worker.stdout.fileno(),worker.stdin.fileno()))
            frame.report.setdefault("js_scope",[]).append("Host Node copied-result facade; actual target Wasm relay Instance retains all ROS authority")
        base_check=runner.check
        def live():
            base_check()
            require(gateway.poll() is None or (gateway.poll()==0 and worker.poll()==0),"Gateway retired while real Guest exchange live")
        runner.check=live
        if events:
            event_exchange(frame,runner,worker,worker_row,peer,peer_row,gateway,native_pid,version)
            record={"case":case,"status":"PASS","event_kinds":[1,2,3,4,5,6,7],"actual_native_effects":{"topic":1,"request":1,"response":1,"goal":1,"feedback":1,"result":1,"absent_native_requests":0}}
        else:
            if mode=="positive":
                runner.wait(sdk,srow,"SDK_ENDPOINTS_CREATED");runner.wait(peer,peer_row,"MATCHED_SDK_PEER")
                runner.send(sdk,b"P");runner.wait(sdk,srow,"SDK_FINAL_SENT");runner.wait(peer,peer_row,"DELIVERED_SDK_PEER")
                advance(runner,sdk,srow,"SDK_DELIVERED","SDK_POLL_AGAIN",gateway)
                runner.wait(sdk,srow,"PASS_SDK_DELIVERY")
            runner.finish(sdk,srow)
            if sdk is not worker:
                runner.finish(worker,worker_row)
            guest=delivery.guest_markers(runner.text(srow),mode)
            runner.send(peer,b"Q");runner.finish(peer,peer_row)
            native=delivery.native_markers(runner.text(peer_row),version,mode,native_pid)
            record={"case":case,"status":"PASS","language":language,"mode":mode,"Guest_markers":guest,"native_markers":native,
                    "explicit_startup_rights":rights,"default_denied_no_native_endpoint":mode=="absent","actual_worker_returncode":worker.returncode,
                    "guest_product":frame.report["sdk_product"]}
        record.update(runtime_architecture=frame.profile["runtime"]["architecture"],native_ros_architecture=frame.profile["native"]["architecture"],
                      gateway_config=config_pin,worker_argv=worker_command,actual_native_pid=native_pid)
        frame.report["cases"].append(record)
        # Config is a consumed input, preserved through all actual protocol
        # work. The owned socket directory is audited only after pidfd cleanup.
        frame.pin(config);return short,config
    finally:
        pass  # Context lease belongs to Frame until checked owned actor cleanup.


def advance(runner,child,row,marker,again,gateway):
    for _ in range(200):
        previous=runner.text(row).count(again+"\n");runner.send(child,b"R");until=time.monotonic()+12
        while time.monotonic()<until:
            runner.check();text=runner.text(row)
            if marker+"\n" in text:
                return
            require(gateway.poll() is None and child.poll() is None,"real role retired during live exchange")
            if text.count(again+"\n")>previous:
                break
            time.sleep(.005)
        else:
            raise TimeoutError("finite Guest poll response missing")
    raise TimeoutError("finite200-poll limit exhausted")


def event_exchange(frame,runner,worker,row,peer,peer_row,gateway,pid,version):
    runner.wait(worker,row,"EVENTS_READY")
    runner.send(peer,b"T");runner.wait(peer,peer_row,"TOPIC_NATIVE_SENT")
    advance(runner,worker,row,"EVENT_TOPIC_DONE","EVENT_POLL_AGAIN",gateway)
    runner.send(peer,b"S");runner.wait(peer,peer_row,"SERVICE_NATIVE_STARTED")
    for marker in ("EVENT_REQUEST_DONE","EVENT_RESPONSE_DONE","EVENT_ERROR_DONE"):
        advance(runner,worker,row,marker,"EVENT_POLL_AGAIN",gateway)
    runner.wait(worker,row,"EVENT_GOAL_WATCHED");runner.send(peer,b"F");runner.wait(peer,peer_row,"FEEDBACK_NATIVE_SENT")
    advance(runner,worker,row,"EVENT_FEEDBACK_DONE","EVENT_POLL_AGAIN",gateway)
    runner.send(peer,b"E");runner.wait(peer,peer_row,"ACTION_NATIVE_SENT")
    for marker in ("EVENT_RESULT_DONE","EVENT_CLOSED_DONE"):
        advance(runner,worker,row,marker,"EVENT_POLL_AGAIN",gateway)
    runner.wait(worker,row,"PASS_WASM_SEVEN_EVENTS");runner.finish(worker,row)
    expected=["EVENTS_READY","EVENT_TOPIC_DONE","EVENT_REQUEST_DONE","EVENT_RESPONSE_DONE","EVENT_ERROR_DONE",
              "EVENT_GOAL_WATCHED","EVENT_FEEDBACK_DONE","EVENT_RESULT_DONE","EVENT_CLOSED_DONE","PASS_WASM_SEVEN_EVENTS"]
    require([x for x in runner.text(row).splitlines() if x!="EVENT_POLL_AGAIN"]==expected,"exact actual seven-event Guest transcript differs")
    runner.send(peer,b"Q");runner.finish(peer,peer_row);native=runner.text(peer_row).splitlines()
    require(native[:5]==[f"{name} backend={version} pid={pid}" for name in ("READY_NATIVE_SEVEN_EVENTS","TOPIC_NATIVE_SENT",
            "SERVICE_NATIVE_STARTED","FEEDBACK_NATIVE_SENT","ACTION_NATIVE_SENT")] and len(native)==6,"exact native seven-event protocol differs")
    match=re.fullmatch(r"PASS_NATIVE_SEVEN_EVENTS backend="+str(version)+r" pid="+str(pid)+
        r" topic_sent=1 service_calls=1 guest_responses=1 goals=1 feedback=1 results=1 absent_native_requests=0 quiet_samples=([0-9]+) quiet_ns=([0-9]+)",native[-1])
    require(match and 30<=int(match[1])<=10000 and 2_000_000_000<=int(match[2])<=10_000_000_000,"real native exact effects/quiet proof absent")


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ("source","manifest","profile","prepared","out"):
        parser.add_argument("--"+name,type=Path,required=True)
    args=parser.parse_args();frame=None;status="FAIL";error=None
    try:
        profile,profile_pin=json_input(args.profile);validate(profile);available(profile)
        frame=Frame(args.source,args.manifest,args.profile,profile,args.out,profile_pin)
        frame.common.install_interrupts();prepared,prepared_pin=frame.json(args.prepared)
        require(prepared.get("kind")=="multiarch-runtime-products" and prepared.get("status")=="PREPARED" and
                prepared.get("source")==str(frame.source) and prepared.get("source_manifest_sha256")==frame.report["source_manifest_sha256"] and
                prepared.get("profile")==profile and all(prepared.get(k) is True for k in ("inputs_preserved","whole_roots_preserved","source_preserved","products_preserved")) and
                all(x["status"]=="PASS" for x in prepared["process_cleanup"]),"fresh current same-profile real products required")
        frame.common.cgroup_preserved(prepared["cgroup_before"],prepared["cgroup_after"])
        frame.common.cgroup_preserved(prepared["cgroup_before"],frame.before)
        for pin in prepared["input_pins"]:
            require(frame.pin(pin["path"])==pin,"prepared tool/SDK/source/MD/log input changed")
        frame.report["prepared_report"]=prepared_pin
        sys.path.insert(0,str(frame.source/"test/0073.ros-tests/support/source/ros_aarch64"));sys.path.insert(0,str(frame.source/"test/0073.ros-tests/support/source/ros_acceptance"))
        delivery=load(frame.source/"test/0073.ros-tests/support/source/ros_acceptance/run_delivery.py","uwvm_multiarch_delivery")
        fixtures=load(frame.source/"test/0073.ros-tests/support/source/ros_acceptance/fixtures.py","uwvm_multiarch_fixture_sources")
        events=load(frame.source/"test/0073.ros-tests/support/source/ros_acceptance/run_events.py","uwvm_multiarch_event_policy")
        for case in profile["cases"]:
            out=frame.out/case;runner=frame.runner_for(out)
            short=config=None
            try:
                short,config=start_case(frame,prepared,runner,out,delivery,fixtures,events,case)
            finally:
                cleanup=runner.close();frame.report["process_cleanup"].append(cleanup);frame.report["commands"]+=runner.commands;frame.runner=None
                require(cleanup["status"]=="PASS","owned exact-birth cleanup incomplete")
                frame.cleanup_contexts()
                frame.save()
        require([r["case"] for r in frame.report["cases"]]==profile["cases"] and all(r["status"]=="PASS" for r in frame.report["cases"]),"closed actual case inventory incomplete")
        frame.report.update(new_tests_executed=True,scope="Selected Runtime arch actual Wasm/IPC against independent native ROS arch; no all-distro or same-arch DDS claim")
        status="PASS"
    except FileNotFoundError as caught:
        status="BLOCKED";error="Explicit SDK/tool/product missing: "+str(caught)
    except BaseException as caught:
        error=repr(caught)
        if frame:
            frame.report["traceback"]=traceback.format_exc()
    if frame:
        return frame.finish(status,error)
    if status=="BLOCKED":
        args.out.mkdir(mode=0o700,parents=True,exist_ok=False)
        with (args.out/"report.json").open("x") as stream:
            json.dump({"kind":"multiarch-runtime","status":"BLOCKED","error":error,"Runtime_or_ROS_behavior":"NOT_EXECUTED"},stream)
        return 1
    raise RuntimeError(error)


if __name__=="__main__":
    raise SystemExit(main())
