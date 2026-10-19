#!/usr/bin/env python3
"""Deliver all seven event kinds through real Wasm and independent native ROS.

Run serially under the parent64GiB/swap0 cgroup/binfmt lease and a new private
mount namespace. Every selected Worker/Gateway/peer is qualified by current
source hashes, full compiler dependencies and stopped ARM/QEMU mapping before
its input gate. Each R causes at most one new readiness/small-buffer/full-copy
attempt. An absent-service timeout is observed separately from actual native
successful service calls. No raw IPC or mathematical model supplies events.
"""
import argparse
import os
from pathlib import Path
import re
import signal
import stat
import time
import traceback

from lane import Lane,add_arguments
from protocol import require
from prepare_native import qualify_peer
from run_delivery import (gateway_config,policy as topic_policy,process_state,
                          start_master,translate,worker_argv)
from event_fixtures import seven_event_guest

EVENTS=("EVENT_TOPIC_DONE","EVENT_REQUEST_DONE","EVENT_RESPONSE_DONE","EVENT_ERROR_DONE",
        "EVENT_FEEDBACK_DONE","EVENT_RESULT_DONE","EVENT_CLOSED_DONE")
AUDIT={"audit":"allow-and-deny","audit-max-rate-hz":"100","audit-burst":"100",
       "audit-max-records":"10000","audit-max-bytes":"1048576"}
QOS={"reliability":"reliable","durability":"volatile","history":"keep-last","depth":"16",
     "deadline":"infinite","lifespan":"infinite","liveliness":"automatic","liveliness-lease-duration":"infinite"}


def selection(version):
    """A literal complete grant inventory; no ambient/native-derived grants."""
    connection,topics=topic_policy(version)
    connection.update({"node-name":"events_gateway","namespace":"/uwvm_events"})
    connection["master-uri" if version==1 else "domain-id"]="http://127.0.0.1:11386" if version==1 else "206"
    topic=next(cap for cap in topics if cap["alias"]=="sub")
    topic.update(alias="topic",resource="/uwvm_events/topic")
    topic["limits"].update({**AUDIT,"burst":"4"})
    caps=[topic]
    def grant(alias,operation,resource,type_name,identity,limits):
        cap={"alias":alias,"operation":operation,"resource":"/uwvm_events/"+resource,
             "type":type_name,"identity":identity,"limits":{**limits,**AUDIT}}
        caps.append(cap);return cap
    service_type="std_srvs/SetBool" if version==1 else "std_srvs/srv/SetBool"
    service_id="09fb03525b03e7ea1fd3992bafd87e16" if version==1 else "RIHS01_abe9e4bb6b41b40e6789712c00ec8871923e089af3f667a79992a428cff2da0a"
    for alias,operation,resource in (("advertise","service.advertise","guest"),("respond","service.respond","guest"),
                                     ("call","service.call","native"),("missing","service.call","absent")):
        limits={"timeout-ms":"1000" if alias=="missing" else "8000","max-inflight":"2",
                "max-request-bytes":"64","max-response-bytes":"64","max-rate-hz":"100",
                "burst":"16","max-bytes-per-second":"65536","validator":"none"}
        if operation=="service.advertise":limits.update({"max-services":"1","max-event-queue-depth":"16",
                                                         "max-event-queue-bytes":"8192","overflow-policy":"reject"})
        if version==2:limits.update({"qos."+key:value for key,value in QOS.items()})
        grant(alias,operation,resource,service_type,service_id,limits)
    action_type="actionlib_tutorials/Fibonacci" if version==1 else "action_tutorials_interfaces/action/Fibonacci"
    action_id="f59df5767bf7634684781c92598b2406" if version==1 else "RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0"
    for alias,operation in (("goal","action.send-goal"),("feedback","action.receive-feedback"),("result","action.receive-result")):
        limits={"fibonacci.max-order":"5","max-active-goals":"1","max-actions":"1","timeout-ms":"8000",
                "max-goal-bytes":"4","max-feedback-bytes":"64","max-result-bytes":"64","cancel-scope":"not-applicable",
                "max-rate-hz":"100","burst":"16","max-bytes-per-second":"65536","max-event-queue-depth":"16",
                "max-event-queue-bytes":"8192","overflow-policy":"reject","validator":"none"}
        if version==2:
            for channel in ("goal-service","cancel-service","result-service","feedback","status"):
                limits.update({"qos."+channel+"."+key:value for key,value in QOS.items()})
        grant(alias,operation,"action",action_type,action_id,limits)
    for alias,source,kinds in (("topic-events","topic","topic-message,resource-closed"),
                              ("request-events","advertise","service-request"),
                              ("response-events","call","service-response,resource-error"),
                              ("error-events","missing","service-response,resource-error"),
                              ("feedback-events","feedback","action-feedback"),
                              ("result-events","result","action-result")):
        source_cap=next(cap for cap in caps if cap["alias"]==source)
        limits={"event-source-alias":source,"event-kinds":kinds,"max-event-bytes":"128",
                "max-event-queue-depth":"16","max-event-queue-bytes":"8192","max-inflight":"2",
                "max-rate-hz":"100","burst":"16","max-bytes-per-second":"65536","overflow-policy":"reject"}
        cap=grant(alias,"event.observe",source_cap["resource"].split("/uwvm_events/",1)[1],
                  "builtin/event-v1","builtin:ros-event-envelope-le64-v1",limits)
    return connection,caps


def advance(lane,worker,row,marker,gateway):
    """One finite Guest attempt at a time; never resend submit/goal operations."""
    for _ in range(200):
        prior=lane.read(row).count("EVENT_POLL_AGAIN\n")
        lane.send(worker,row,b"R","independent native phase ready; one bounded event poll")
        deadline=time.monotonic()+12
        while time.monotonic()<deadline:
            lane.check();text=lane.read(row)
            if marker+"\n" in text:return
            require(gateway.poll() is None,"native Gateway retired during live Guest event exchange")
            if text.count("EVENT_POLL_AGAIN\n")>prior:break
            require(worker.poll() is None,"real Guest retired before complete event validation")
            time.sleep(.005)
        else:raise TimeoutError("one finite unified-event poll did not complete")
        time.sleep(.02)
    raise TimeoutError("finite200-attempt event budget exhausted")


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser)
    parser.add_argument("--prepared-native",type=Path,required=True)
    parser.add_argument("--wat2wasm",type=Path,required=True)
    parser.add_argument("--version",type=int,choices=(1,2),required=True);args=parser.parse_args()
    lane=None;status="FAIL"
    try:
        version=args.version;selected=selection(version)
        lane=Lane(args,"seven-event-real-native-delivery");lane.qualify_workers();lane.qualify_gateway(version)
        wasm=translate(lane,args.wat2wasm,"seven-events",seven_event_guest(version))
        if version==1:start_master(lane,11386)
        else:lane.lock("domain-206")
        peer_source=qualify_peer(lane,args.prepared_native,version,"events")
        lane.snapshot(peer_source,lane.out/("ros"+str(version))/"tmp/event-peer","event-peer")
        peer,peer_row=lane.start("event-peer",lane.native(version,["/tmp/event-peer"]),lane.environment(version,11386,206),seconds=180)
        proof=lane.stopped(peer,peer_row,"event-peer",cooperative=True);pid=proof["actual_stopped_ARM"]["pid"]
        lane.send(peer,peer_row,b"I","actual current stopped ARM peer and owned ROS context")
        lane.wait_marker(peer,peer_row,"READY_NATIVE_SEVEN_EVENTS")
        argv=worker_argv(lane,(version,),wasm,multi=True,selections={version:selected})
        worker,row=lane.start("event-worker",["/bin/bash","-c",'kill -STOP $$; exec "$@"',"event-worker",
            *lane.config["qemu_argv"],*argv],seconds=180,rss=8*1024**3)
        deadline=time.monotonic()+5
        while worker.poll() is None and process_state(worker.pid) not in ("T","t") and time.monotonic()<deadline:
            lane.check();time.sleep(.001)
        require(worker.poll() is None and process_state(worker.pid) in ("T","t"),"PID preserving Worker shell did not stop")
        config=lane.short/("ros"+str(version)+".conf")
        config.write_text(gateway_config(version,worker.pid,False,selected));config.chmod(0o600)
        gateway,gateway_row=lane.start("event-gateway",lane.native(version,["/tmp/gateway","/guest/"+config.name]),
                                     lane.environment(version,11386,206,gateway=True),seconds=180)
        lane.stopped(gateway,gateway_row,"gateway-"+str(version))
        socket=lane.short/("ros"+str(version)+".sock");deadline=time.monotonic()+30
        while not socket.exists() and time.monotonic()<deadline:
            lane.check();require(gateway.poll() is None,"native Gateway retired before socket");time.sleep(.01)
        require(socket.exists() and stat.S_ISSOCK(socket.lstat().st_mode) and socket.lstat().st_uid==os.getuid(),"owned Gateway socket missing")
        owner=next(item for item in lane.owned.rows.values() if item["identity"]["pid"]==worker.pid)
        lane.owned.send(owner,signal.SIGCONT);lane.stopped(worker,row,"worker")
        lane.send(worker,row,b"G","all native/Worker mappings and explicit complete policy")
        lane.wait_marker(worker,row,"EVENTS_READY")
        lane.send(peer,peer_row,b"T","actual Guest topic observer activated")
        lane.wait_marker(peer,peer_row,"TOPIC_NATIVE_SENT");advance(lane,worker,row,"EVENT_TOPIC_DONE",gateway)
        lane.send(peer,peer_row,b"S","actual Guest service observer activated")
        lane.wait_marker(peer,peer_row,"SERVICE_NATIVE_STARTED");advance(lane,worker,row,"EVENT_REQUEST_DONE",gateway)
        advance(lane,worker,row,"EVENT_RESPONSE_DONE",gateway);advance(lane,worker,row,"EVENT_ERROR_DONE",gateway)
        lane.wait_marker(worker,row,"EVENT_GOAL_WATCHED")
        lane.send(peer,peer_row,b"F","actual accepted owned goal and both data watches installed")
        lane.wait_marker(peer,peer_row,"FEEDBACK_NATIVE_SENT")
        advance(lane,worker,row,"EVENT_FEEDBACK_DONE",gateway)
        lane.send(peer,peer_row,b"E","real Guest confirmed feedback before independent native Result channel")
        lane.wait_marker(peer,peer_row,"ACTION_NATIVE_SENT")
        for marker in EVENTS[5:]:advance(lane,worker,row,marker,gateway)
        lane.wait_marker(worker,row,"PASS_WASM_SEVEN_EVENTS");lane.finish(worker,row)
        lines=lane.read(row).splitlines();require([line for line in lines if line!="EVENT_POLL_AGAIN"]==
            ["EVENTS_READY",*EVENTS[:4],"EVENT_GOAL_WATCHED",*EVENTS[4:],"PASS_WASM_SEVEN_EVENTS"],"exact real Wasm event sequence differs")
        lane.send(peer,peer_row,b"Q","real Guest all7 header/payload checks and actual exit0")
        lane.finish(peer,peer_row);native=lane.read(peer_row).splitlines()
        expected=[f"{name} backend={version} pid={pid}" for name in
                  ("READY_NATIVE_SEVEN_EVENTS","TOPIC_NATIVE_SENT","SERVICE_NATIVE_STARTED","FEEDBACK_NATIVE_SENT","ACTION_NATIVE_SENT")]
        require(native[:5]==expected and len(native)==6,"closed independent native protocol differs")
        match=re.fullmatch(r"PASS_NATIVE_SEVEN_EVENTS backend="+str(version)+r" pid="+str(pid)+
            r" topic_sent=1 service_calls=1 guest_responses=1 goals=1 feedback=1 results=1 absent_native_requests=0 quiet_samples=([0-9]+) quiet_ns=([0-9]+)",native[-1])
        require(match and int(match[1])>=30 and int(match[2])>=2_000_000_000,"native exact effects/quiet proof differs")
        gateway_exit=gateway.poll()
        # The principal-bound production listener normally retires after the
        # exact Worker pidfd becomes readable. That successful retirement must
        # not be mislabeled as a communication fault after Guest/native proof.
        require(gateway_exit in (None,0) and worker.returncode==0,"Gateway failed after verified Worker retirement")
        lane.report['gateway_after_worker']={"returncode":gateway_exit,"principal_retirement_allowed":True}
        lane.report.update(version=version,worker_argv=argv,gateway_config={"text":config.read_text(),"pin":lane.pin(config)},
            guest_markers=lines,native_markers=native,seven_event_kinds=[1,2,3,4,5,6,7],
            native_effects={"topic_sent":1,"service_calls":1,"guest_responses":1,"goals":1,"feedback":1,"results":1,"absent_native_requests":0},
            four_additive_imports_executed=True,too_small90_retains_all_seven_records=True,memory1_and_memory0_sentinel=True,
            async_success_and_timeout_owned_correlations=True,resource_closed_stops_later_delivery=True,new_tests_executed=True)
        status="PASS"
    except BaseException as error:
        if lane is None:raise
        lane.report.update(error=repr(error),traceback=traceback.format_exc())
    finally:
        if lane is not None:status=lane.close(status)
    return 0 if status=="PASS" else 1
if __name__=="__main__":raise SystemExit(main())
