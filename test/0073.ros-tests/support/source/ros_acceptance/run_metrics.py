#!/usr/bin/env python3
"""Run real queue/isolated-IPC probes; do not substitute them for ROS latency."""
import argparse
import json
from pathlib import Path
import sys
import traceback
from lane import Lane, add_arguments
from prepare_native import qualify_peer
from protocol import require, queue_samples


def check_queue(text):
    """Read numeric native snapshots, not configured capacities or PASS alone."""
    lines = text.splitlines(); require(lines and text.endswith("\n") and "\r" not in text, "complete queue output required")
    require(lines[-1] == "PASS_NATIVE_QUEUE_METRICS operations=82 samples=35 configured_depth=4 configured_bytes=512 reserved_control=2 pending_max=1 actual_occupancy=1",
            "exact bounded queue workload marker required")
    samples = [json.loads(line) for line in lines[:-1]]; require(len(samples) == 35, "all actual queue snapshots required")
    require(all(row["kind"] == "actual-native-queue" for row in samples), "queue snapshot kind differs")
    # Each source has an independently initialized counter lifetime. Do not
    # combine a legacy body's bytes with a shared64-byte envelope's bytes.
    groups = []; active = []
    for row in samples:
        if row["conservation_scope"] != "queued-topic-before-retirement":
            if active: groups.append(active); active=[]
            continue
        if row["step"] in ("empty", "reject", "drop-oldest", "drop-newest", "backpressure") and active:
            groups.append(active); active=[]
        active.append(row)
    if active: groups.append(active)
    require(len(groups) == 8, "four observer/four legacy queue lifetimes required")
    for group in groups:
        source = group[0]["queue"] == "legacy-source"
        queue_samples(group, 3 if source else 4, 48 if source else 640)
    require(any(row["pending_depth"] == 1 and row["backpressured"] > 0 for row in samples)
            and any(row["dropped_oldest"] == 1 for row in samples)
            and any(row["dropped_newest"] == 1 for row in samples)
            and any(row["reserved_depth"] == 1 and row["completed"] == 1 for row in samples),
            "actual overflow/pending/reserved-credit observations missing")
    return samples


def check_ipc(text):
    lines = text.splitlines(); require(text.endswith("\n") and len(lines) == 3 and lines[-1] ==
        "PASS_ISOLATED_IPC records=2000 payload_sizes=64,4096 concurrency=1 authentication=not-measured", "closed IPC output differs")
    rows = [json.loads(line) for line in lines[:2]]
    require([row["payload_bytes"] for row in rows] == [64, 4096], "IPC sizes differ")
    for row in rows:
        require(row["kind"] == "isolated-socketpair-frame-ipc" and row["simulation"] == "qemu-aarch64"
            and row["completed"] == 1000 and row["concurrency"] == 1 and row["header_bytes"] == 48
            and type(row["elapsed_ns"]) is int and row["elapsed_ns"] > 0
            and 0 < row["latency_ns_p50"] <= row["latency_ns_p95"] <= row["latency_ns_max"]
            and all(row[key] is False for key in ("gateway_authentication_measured", "native_ros_measured", "permission_check_measured")),
            "actual isolated IPC metric scope differs")
    return rows


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser,workers=False)
    parser.add_argument("--prepared-native",type=Path,required=True);args=parser.parse_args();lane=None;status="FAIL"
    try:
        lane=Lane(args,"remaining-queue-ipc-metrics");lane.root(2);origin=qualify_peer(lane,args.prepared_native,2,"metrics")
        destination=lane.out/"ros2/tmp/metrics";lane.snapshot(origin,destination,"metrics")
        for mode in ("queue","ipc"):
            child,row=lane.start("metrics-"+mode,lane.native(2,["/tmp/metrics",mode]),seconds=120)
            lane.stopped(child,row,"metrics",cooperative=True);lane.send(child,row,b"I","actual native stopped ARM map")
            lane.finish(child,row);text=lane.read(row);measurements=check_queue(text) if mode=="queue" else check_ipc(text)
            lane.report["cases"].append({"status":"PASS","workload":mode,"measurements":measurements,
                "scope":"Actual native queue implementation" if mode=="queue" else "Socketpair framing only, no Gateway/ROS/auth cost"});lane.save()
        lane.report["new_tests_executed"]=True;status="PASS"
    except BaseException as error:
        if lane is None:raise
        lane.report.update(error=repr(error),traceback=traceback.format_exc())
    finally:
        if lane is not None:status=lane.close(status)
    return 0 if status=="PASS" else 1
if __name__=="__main__":raise SystemExit(main())
