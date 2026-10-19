#!/usr/bin/env python3
"""Measure real native Graph calls and zero effects for six closed gap cases.

This adds evidence to the original55 attacks; it never replaces that inventory.
Graph flood uses real Wasm imports and the authenticated fixed-PID Worker. Raw
frame and wrong-UID rows are explicitly adversarial host/socket tests. Each has
a positive native Graph control on the same selected Gateway/observer before
the denial window, plus an independent native zero-resource/message witness.
Audit output is retained but never substituted for the native query ledger.
"""
import argparse
import os
from pathlib import Path
import signal
import stat
import struct
import sys
import time
import traceback

sys.dont_write_bytecode=True
from lane import Lane, add_arguments
from protocol import require
from fixtures import graph_guest
from prepare_native import qualify_peer
from run_delivery import policy, worker_argv, gateway_config, translate, start_master, native_markers
from run_action_qos import process_identity

METHODS={1:frozenset(("getSystemState","getTopicTypes","getPublishedTopics","lookupNode","lookupService","getParamNames","searchParam")),
         2:frozenset(("topics","nodes","nodes-enclaves","services","clients-by-node","services-by-node","publishers-by-node","subscriptions-by-node","publisher-info","subscription-info"))}


def selection(version):
    connection,_=policy(version,True)
    if version==1:connection["master-uri"]="http://127.0.0.1:11386"
    common={"max-graph-result-bytes":"128","allowed-name-prefix":"/uwvm_sdk_relay","timeout-ms":"2000","max-inflight":"1",
        "max-bytes-per-second":"65536","audit":"allow-and-deny","audit-max-rate-hz":"1000","audit-burst":"256",
        "audit-max-records":"10000","audit-max-bytes":"1048576"}
    grants=[]
    for alias,rate,burst in (("ready","5","1"),("cap","0.001","2")):
        grants.append({"alias":alias,"operation":"graph.inspect-node","resource":"/uwvm_sdk_relay/sdk_peer",
            "type":"builtin/graph-list-v1","identity":"builtin:graph-list-v1","limits":{**common,"max-rate-hz":rate,"burst":burst}})
    return connection,grants


def manifest(version,selected):
    """Exact public UWRP v1 serialization for the explicit native control.

    Its acceptance is also checked by the real Gateway byte equality. This is
    not an alternate Guest trust binding: the host control has only the exact
    configured actor PID/UID and its own explicitly selected metadata grants.
    """
    connection,grants=selected;out=bytearray(b"UWRP\x01"+bytes((version,1)))
    def integer(value):out.extend(struct.pack("<I",value))
    def text(value):body=value.encode();integer(len(body));out.extend(body)
    integer(len(connection))
    for key,value in sorted(connection.items()):text(key);text(value)
    integer(len(grants))
    for cap in sorted(grants,key=lambda item:item["alias"]):
        text(cap["alias"])
        for key in ("alias","operation","resource","type","identity"):text(cap[key])
        integer(len(cap["limits"]))
        for key,value in sorted(cap["limits"].items()):text(key);text(value)
    return bytes(out)


def ledger(path,version,identity):
    """Read the bounded unchanged observer format without filling missing rows."""
    state=path.lstat();require((state.st_dev,state.st_ino)==identity and stat.S_ISREG(state.st_mode) and
        state.st_uid==os.getuid() and state.st_mode&0o777==0o600 and state.st_size<=1024*1024,"native ledger replaced/overgrown")
    data=path.read_bytes()
    if not data:return {"active":False,"rows":[],"counts":{}}
    text=data.decode("ascii");require(text.endswith("\n") and "\r" not in text,"closed native ledger bytes required")
    lines=text.splitlines();prefix="MASTER_GRAPH" if version==1 else "RMW_GRAPH"
    require(lines[0]==prefix+"_LEDGER "+str(identity[0])+" "+str(identity[1]),"native ledger FD identity differs")
    rows=[];counts={}
    for expected,line in enumerate(lines[1:],1):
        tokens=line.split(" ");require(len(tokens)==4 and tokens[0]==prefix and tokens[1]==str(expected) and
            tokens[2] in METHODS[version] and tokens[3].lstrip("-").isdigit(),"closed native ledger row differs")
        require(int(tokens[3])==(1 if version==1 else 0),"real native Graph returned an error")
        rows.append({"sequence":expected,"method":tokens[2],"native_return":int(tokens[3])});counts[tokens[2]]=counts.get(tokens[2],0)+1
    require(len(rows)<=4096,"native ledger admitted excess records");return {"active":True,"rows":rows,"counts":counts}


def delta(before,after):
    require(after["rows"][:len(before["rows"])]==before["rows"],"native ledger previous prefix changed")
    result={key:after["counts"].get(key,0)-before["counts"].get(key,0) for key in set(before["counts"])|set(after["counts"])}
    require(all(value>=0 for value in result.values()),"native query counter regressed");return {key:value for key,value in result.items() if value}


def quiet(lane,path,version,identity,previous):
    begin=time.monotonic_ns();samples=0
    while time.monotonic_ns()-begin<2_000_000_000:
        lane.check();require(ledger(path,version,identity)==previous,"native query occurred in controlled denial quiet window")
        samples+=1;time.sleep(.02)
    elapsed=time.monotonic_ns()-begin;require(samples>=30,"independent quiet sample count missing")
    return samples,elapsed


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser)
    parser.add_argument("--version",type=int,choices=(1,2),required=True);parser.add_argument("--case",choices=("graph-query-flood","wrong-uid","raw-frame"),required=True)
    parser.add_argument("--prepared-native",type=Path,required=True);parser.add_argument("--prepared-observer",type=Path)
    parser.add_argument("--wat2wasm",type=Path,required=True);args=parser.parse_args()
    require(args.version!=2 or args.prepared_observer,"qualified separate instrumented ROS2 Gateway required")
    lane=None;status="FAIL"
    try:
        lane=Lane(args,"native-side-effect-gap");version=args.version;selected=selection(version)
        lane.report.update(case=args.case+"-ros"+str(version),actual_execution_scope="real Wasm authenticated Graph flood" if args.case=="graph-query-flood" else "owned adversarial native host/socket; not a Wasm authentication claim")
        lane.qualify_workers();lane.qualify_gateway(version)
        if version==1:start_master(lane)
        else:lane.lock("domain-205")
        origin=qualify_peer(lane,args.prepared_native,version);lane.snapshot(origin,lane.out/("ros"+str(version))/"tmp/peer","peer-"+str(version))
        peer,peer_row=lane.start("native-peer",lane.native(version,["/tmp/peer","absent"],witness=True),lane.environment(version),seconds=150)
        peer_proof=lane.stopped(peer,peer_row,"peer-"+str(version),cooperative=True);lane.send(peer,peer_row,b"I","current independent native ARM map")
        lane.wait_marker(peer,peer_row,"READY_SDK_PEER")
        file=lane.short/"graph.ledger";file.touch(mode=0o600);state=file.stat();identity=(state.st_dev,state.st_ino)
        lane.report["ledger_identity"]={"device":identity[0],"inode":identity[1],"mode":"0600","initial_bytes":0}
        if version==1:
            lane.lock("master-11386")
            proxy,proxy_row=lane.start("master-proxy",[sys.executable,"-B",str(lane.source/"tests/ros_acceptance/master_observer.py"),"--ledger",str(file)],seconds=150,rss=512*1024**2)
            lane.stopped_host(proxy,proxy_row,sys.executable);lane.send(proxy,proxy_row,b"G","actual stopped owned host proxy map")
            lane.wait_marker(proxy,proxy_row,"READY_MASTER_GRAPH_PROXY")
            gateway_path="/tmp/gateway";gateway_label="gateway-1"
        else:
            origin=qualify_peer(lane,args.prepared_observer,2,"graph-gateway")
            lane.snapshot(origin,lane.out/"ros2/tmp/observed-gateway","observed-gateway-2")
            gateway_path="/tmp/observed-gateway";gateway_label="observed-gateway-2"
        if args.case=="graph-query-flood":
            wasm=translate(lane,args.wat2wasm,"graph-quota",graph_guest(version))
            argv=worker_argv(lane,(version,),wasm,multi=True,selections={version:selected})
            child,row=lane.start("worker",["/bin/bash","-c",'kill -STOP $$; exec "$@"',"graph-worker",*lane.config["qemu_argv"],*argv],seconds=150,rss=8*1024**3)
            until=time.monotonic()+5
            while time.monotonic()<until and child.poll() is None and process_identity(child.pid)["state"] not in ("T","t"):lane.check();time.sleep(.001)
            require(child.poll() is None and process_identity(child.pid)["state"] in ("T","t"),"fixed Worker PID did not stop")
            lane.report["worker_argv"]=argv
        else:
            binding=lane.out/"native-control-manifest.bin";binding.write_bytes(manifest(version,selected));lane.pin(binding)
            child,row=lane.start("native-attack",[sys.executable,"-B",str(lane.source/"tests/ros_acceptance/raw_client.py"),"--socket",str(lane.short/("ros"+str(version)+".sock")),
                "--version",str(version),"--mode",args.case,"--manifest",str(binding)],seconds=150,rss=512*1024**2)
            lane.stopped_host(child,row,sys.executable)
        config=lane.short/("ros"+str(version)+".conf");config.write_text(gateway_config(version,child.pid,False,selected).replace("timeout-ms 10000\n","timeout-ms 60000\n"));config.chmod(0o600)
        environment=lane.environment(version,gateway=True)
        if version==1:environment["ROS_MASTER_URI"]="http://127.0.0.1:11386"
        else:environment.update(UWVM_ROS_TEST_GRAPH_LEDGER="/guest/graph.ledger",UWVM_ROS_TEST_GRAPH_NODE="sdk_gateway")
        gateway,gateway_row=lane.start("gateway",lane.native(version,[gateway_path,"/guest/"+config.name]),environment,seconds=150)
        lane.stopped(gateway,gateway_row,gateway_label);socket_path=lane.short/("ros"+str(version)+".sock");until=time.monotonic()+30
        while not socket_path.exists() and time.monotonic()<until:lane.check();require(gateway.poll() is None,"Gateway retired before readiness");time.sleep(.01)
        require(socket_path.exists() and stat.S_ISSOCK(socket_path.lstat().st_mode),"owned attack socket missing")
        if args.case=="wrong-uid":
            # Let the owned dropped-UID child reach SO_PEERCRED. Config files
            # retain0600; no other task/socket or native right is modified.
            lane.short.chmod(0o711);socket_path.chmod(0o666)
        lane.report["gateway_config"]={"pin":lane.pin(config),"text":config.read_text()}
        before=ledger(file,version,identity)
        if args.case=="graph-query-flood":
            owner=next(item for item in lane.owned.rows.values() if item["identity"]["pid"]==child.pid)
            lane.owned.send(owner,signal.SIGCONT);lane.stopped(child,row,"worker");lane.send(child,row,b"G","all native/Gateway/Worker maps and independent zero-effect witness ready")
            for attempt in range(200):
                until=time.monotonic()+3
                while time.monotonic()<until:
                    lane.check();text=lane.read(row)
                    if "GRAPH_READY_DONE\n" in text:break
                    if text.splitlines().count("GRAPH_READY_AGAIN")>attempt:break
                    require(child.poll() is None,"Guest discovery retired");time.sleep(.005)
                else:raise TimeoutError("bounded Graph readiness round did not finish")
                if "GRAPH_READY_DONE\n" in text:break
                time.sleep(.25);lane.send(child,row,b"R","bounded readiness-only native query window")
            else:raise TimeoutError("Graph discovery quota exhausted")
            warm=ledger(file,version,identity);require(warm["active"] and len(warm["rows"])>len(before["rows"]),"real native observer positive readiness control missing")
            lane.send(child,row,b"P","exact real native node bytes/discovery completed")
            lane.wait_marker(child,row,"GRAPH_DENIED13_DONE");denied=ledger(file,version,identity)
            require(delta(warm,denied)=={},"operation-substitution13 reached native Graph")
            samples,elapsed=quiet(lane,file,version,identity,denied)
            lane.report["graph_denial"]={"expected_errno":13,"actual_errno":13,"output_sentinels_unchanged":True,
                "native_observer_independent":True,"native_effects_before":warm["counts"],"native_effects_after":denied["counts"],
                "observer_kind":"master-graph-query" if version==1 else "native-graph-query-hook","quiet_samples":samples,"quiet_ns":elapsed}
            lane.send(child,row,b"D","independent Graph operation-denial13 zero-effect interval")
            lane.wait_marker(child,row,"GRAPH_CONTROL_DONE");control=ledger(file,version,identity)
            expected={"getSystemState" if version==1 else "nodes":1};require(delta(warm,control)==expected,"native allowed Graph control count differs")
            lane.send(child,row,b"B","actual native positive-control query count1")
            lane.wait_marker(child,row,"GRAPH_ALLOWED_TWO");allowed=ledger(file,version,identity)
            require(delta(control,allowed)==expected,"second allowed native Graph query count differs")
            lane.send(child,row,b"E","two actual allowed native Graph calls observed before denied98 window")
            lane.wait_marker(child,row,"GRAPH_FLOOD_DONE");after=ledger(file,version,identity)
            require(delta(allowed,after)=={},"98 EAGAIN refusals caused extra native Graph queries")
            samples,elapsed=quiet(lane,file,version,identity,after);lane.send(child,row,b"Q","2s independently unchanged native ledger")
            lane.wait_marker(child,row,"PASS_GRAPH_NATIVE_QUOTA");lane.finish(child,row)
            lane.report["native_queries"]={"initialization":before,"readiness":warm,"control":control,"allowed_two":allowed,"flood":after,"flood_requests":100,"allowed":2,"denied11":98,"denied_native_queries":0,"quiet_samples":samples,"quiet_ns":elapsed}
            lane.report["graph_flood_denial"]={"expected_errno":11,"actual_errno":11,"output_sentinels_unchanged":True,
                "native_observer_independent":True,"native_effects_before":allowed["counts"],"native_effects_after":after["counts"],
                "observer_kind":"master-graph-query" if version==1 else "native-graph-query-hook","quiet_samples":samples,"quiet_ns":elapsed}
        else:
            lane.send(child,row,b"G","actual stopped host/Gateway/native witness maps")
            lane.wait_marker(child,row,"RAW_GRAPH_CONTROL_DONE");control=ledger(file,version,identity)
            require(control["active"] and len(control["rows"])>len(before["rows"]),"native observer was not exercised by positive control")
            lane.send(child,row,b"A","actual native observer positive control completed and old control socket closed")
            lane.wait_marker(child,row,"PASS_RAW_CREDENTIAL" if args.case=="wrong-uid" else "PASS_RAW_FRAME");lane.finish(child,row)
            after=ledger(file,version,identity);require(delta(control,after)=={},"raw/credential refusals reached native Graph")
            lane.report["native_queries"]={"before_attack":control,"after_attack":after,"denied_native_queries":0,
                "refusal_scope":"SO_PEERCRED rejected hello before response; no Guest errno claim" if args.case=="wrong-uid" else "13 malformed/truncated header refusals plus real unbound errno13"}
        lane.send(peer,peer_row,b"Q","actual attack actor exit0 and native query ledger bound")
        lane.wait_marker(peer,peer_row,"PASS_SDK_PEER");lane.finish(peer,peer_row)
        lane.report["independent_native"]=native_markers(lane.read(peer_row),version,"absent",peer_proof["actual_stopped_ARM"]["pid"])
        require(gateway.poll() is None,"Gateway died during attack refusal")
        # Read the ledger before stopping the Gateway; teardown native methods
        # are outside the controlled denial interval and retained separately.
        lane.report["native_ledger"]=lane.pin(file);lane.report.update(new_tests_executed=True,native_zero_resource_effects=True)
        if version==1:lane.stop_role(proxy,proxy_row)
        status="PASS"
    except BaseException as error:
        if lane is not None:lane.report.update(error=repr(error),traceback=traceback.format_exc())
        else:raise
    finally:
        if lane is not None:
            # A live observer may still append teardown records. Retain its
            # final full ledger after cleanup instead of claiming an immutable
            # pre-teardown log pin. Controlled-window rows remain exact copies.
            if "native_ledger" in lane.report:lane.pins=[pin for pin in lane.pins if pin["path"]!=str(file)]
            status=lane.close(status)
    return 0 if status=="PASS" else 1


if __name__=="__main__":raise SystemExit(main())
