#!/usr/bin/env python3
"""Run actual bounded ROS1 TCPROS transport against an owned numeric Master."""
import argparse
from pathlib import Path
import re
import sys
import traceback
from lane import Lane, add_arguments
from prepare_native import qualify_peer
from run_delivery import start_master
from protocol import require

CASES={"a":0,"e":0,"b":0,"m":13,"h":13,"u":22,"n":22,"r":22,"l":90,"t":110,"c":107}


def check(text,expected_pid):
    lines=text.splitlines();require(text.endswith("\n") and len(lines)==13 and "\r" not in text,"closed bounded-service log required")
    require(re.fullmatch(r"READY_BOUNDED_SERVICE pid="+str(expected_pid)+r" peer_pid=[1-9][0-9]* master=11385 transport=tcpros",lines[0]),"actual bounded-service role metadata differs")
    rows=[]
    for line,(mode,error) in zip(lines[1:-1],CASES.items()):
        request=0 if mode in ("m","h") else 1
        require(line==f"BOUNDED_SERVICE_CASE mode={mode} errno={error} accepted=1 requests={request} closed=1 output_preserved=1",
                "native exact error/request-once/FD-close evidence differs")
        rows.append({"mode":mode,"actual_errno":error,"requests":request,"closed":1,"native_output_preserved":True})
    require(lines[-1]=="PASS_BOUNDED_SERVICE checks=14 positive=3 malformed22=3 quota90=1 header13=2 local13=1 timeout110=1 closed107=1 dns95=1 numeric_profile=1",
            "bounded-service complete workload marker differs")
    return rows


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser,workers=False)
    parser.add_argument("--prepared-native",type=Path,required=True);args=parser.parse_args();lane=None;status="FAIL"
    try:
        lane=Lane(args,"bounded-service-transport-arm");lane.root(1);start_master(lane,port=11385)
        origin=qualify_peer(lane,args.prepared_native,1,"bounded-service");lane.snapshot(origin,lane.out/"ros1/tmp/service","bounded-service")
        child,row=lane.start("bounded-service",lane.native(1,["/tmp/service"]),lane.environment(1,master_port=11385),seconds=120)
        proof=lane.stopped(child,row,"bounded-service",cooperative=True);lane.send(child,row,b"I","actual native ARM/owned numeric Master readiness")
        lane.finish(child,row);rows=check(lane.read(row),proof["actual_stopped_ARM"]["pid"])
        lane.report.update(cases=rows,new_tests_executed=True,
            scope="Actual native bounded official Service transport/fault peer; no Guest authorization or remote rollback claim")
        status="PASS"
    except BaseException as error:
        if lane is None:raise
        lane.report.update(error=repr(error),traceback=traceback.format_exc())
    finally:
        if lane is not None:status=lane.close(status)
    return 0 if status=="PASS" else 1
if __name__=="__main__":raise SystemExit(main())
