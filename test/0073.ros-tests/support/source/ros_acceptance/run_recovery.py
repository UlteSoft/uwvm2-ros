#!/usr/bin/env python3
"""Run bounded dual-backend recovery with independent native delivery.

The three closed cases distinguish an owned Master outage from retirement of
one owned TCPROS/DDS peer. No firewall, foreign namespace, non-owned process,
implicit authority or failed application publish replay is used. The same
survivor process and Guest Instance/handles span all three exact byte stages.
Every PASS below is reachable only during a future actual guarded execution.
"""
import argparse
import os
from pathlib import Path
import signal
import stat
import sys
import time
import traceback
import xmlrpc.client

sys.dont_write_bytecode = True
from lane import Lane, add_arguments
from protocol import require, marker_lines, GID
from prepare_native import qualify_peer
from fixtures import recovery_guest
from run_delivery import translate, worker_argv, gateway_config, start_master, STRING_MD5
from run_action_qos import process_identity
from topic_gate import ShortTransport

CASES = ("master-restart", "tcpros-peer-restart", "dds-peer-restart")


def native_records(text, version, mode, pid):
    """Bind the complete native transcript; partial discovery is not success."""
    common = ("backend", "mode", "pid", "node", "gateway_node")
    common += ("transport", "md5") if version == 1 else ("domain", "rmw", "own_pub_gid", "own_sub_gid", "gateway_pub_gid", "gateway_sub_gid")
    schemas = {"READY_RECOVERY_PEER": common, "MATCHED_RECOVERY_PEER": common+("publishers", "subscribers"),
        "RECOVERY_NATIVE_DELIVERY": common+("stage", "received", "sent"),
        "RETIRED_RECOVERY_PEER": common+("before", "during", "after", "excess", "graceful"),
        "PASS_RECOVERY_PEER": common+("before", "during", "after", "excess", "quiet_samples", "quiet_ns")}
    rows = marker_lines(text, "", schemas)
    stages = ["before"] if mode == "before" else ["after"] if mode == "after" else ["before", "during", "after"]
    require([name for name, _ in rows] == ["READY_RECOVERY_PEER", "MATCHED_RECOVERY_PEER", *["RECOVERY_NATIVE_DELIVERY"]*len(stages),
        "RETIRED_RECOVERY_PEER" if mode == "before" else "PASS_RECOVERY_PEER"], "native recovery transcript cardinality differs")
    stable = None
    for index, (name, value) in enumerate(rows):
        require(value["backend"] == str(version) and value["mode"] == mode and value["pid"] == str(pid)
            and value["node"] == "/uwvm_sdk_relay/sdk_peer" and value["gateway_node"] == "/uwvm_sdk_relay/sdk_gateway", "native recovery origin differs")
        if version == 1:
            require(value["transport"] == "tcpros" and value["md5"] == STRING_MD5, "official TCPROS identity differs")
        else:
            require(value["domain"] == "205" and value["rmw"] == "rmw_fastrtps_cpp", "native DDS context differs")
            current = tuple(value[key] for key in ("own_pub_gid", "own_sub_gid", "gateway_pub_gid", "gateway_sub_gid"))
            require(all(GID.fullmatch(gid) for gid in current), "strict16-byte native GID width differs")
            if name == "READY_RECOVERY_PEER": require(current[0] != "0"*32 and current[1:] == ("0"*32,)*3, "prematch GID scope differs")
            else:
                require("0"*32 not in current and len(set(current)) == 4, "native matched GIDs differ")
                if stable is None: stable = current
                require(current == stable, "matched native endpoints changed within one role")
        if name == "MATCHED_RECOVERY_PEER": require(value["publishers"] == value["subscribers"] == "1", "final exact native connection count differs")
        if name == "RECOVERY_NATIVE_DELIVERY":
            require(value["stage"] == stages[index-2] and value["received"] == value["sent"] == "1", "native byte-stage effect differs")
        if name in ("RETIRED_RECOVERY_PEER", "PASS_RECOVERY_PEER"):
            expected = ("1", "0", "0") if mode == "before" else ("0", "0", "1") if mode == "after" else ("1", "1", "1")
            require(tuple(value[key] for key in ("before", "during", "after")) == expected and value["excess"] == "0", "native final delivery counts differ")
            if mode == "before": require(value["graceful"] == "1", "actual peer endpoint retirement missing")
            else: require(value["quiet_samples"].isdigit() and 30 <= int(value["quiet_samples"]) <= 10000 and
                value["quiet_ns"].isdigit() and 2_000_000_000 <= int(value["quiet_ns"]) <= 10_000_000_000, "bounded quiet observation missing")
    return rows


def delivery(lane, actor, stage):
    child, row, version, pid = actor
    until = time.monotonic()+30
    while time.monotonic() < until:
        lane.check(); text = lane.read(row)
        require("FAIL_" not in text and child.poll() is None, "native recovery actor failed before exact delivery")
        matching = [line for line in text.splitlines() if line.startswith("RECOVERY_NATIVE_DELIVERY ") and " stage="+stage+" " in line]
        if matching:
            require(len(matching) == 1 and " received=1 sent=1" in matching[0], "native stage duplicated or incomplete")
            return matching[0]
        time.sleep(.005)
    raise TimeoutError("native recovery stage deadline: "+stage)


def exchange(lane, worker, row, peers, stage):
    """Poll only after each independent native reply was actually published."""
    lane.wait_marker(worker, row, "RECOVERY_"+stage.upper()+"_SENT")
    proofs = [delivery(lane, actor, stage) for actor in peers]
    final = "RECOVERY_"+stage.upper()+"_COMPLETE"
    for count in range(200):
        before = lane.read(row).splitlines().count("SDK_POLL_AGAIN")
        lane.send(worker, row, b"R", "all independent native exact "+stage+" delivery/reply records")
        until = time.monotonic()+3
        while time.monotonic() < until:
            lane.check(); text = lane.read(row); lines = text.splitlines()
            require(worker.poll() is None and "FAIL_" not in text, "Guest retired before recovery response")
            if final in lines:
                lane.report.setdefault("stages", []).append({"stage": stage, "native": proofs, "Guest_marker": final, "poll_gates": count+1}); return
            if lines.count("SDK_POLL_AGAIN") > before: break
            time.sleep(.005)
        else: raise TimeoutError("one bounded Guest recovery poll did not complete")
        time.sleep(.01)
    raise TimeoutError("bounded Guest recovery polling exhausted")


def master_peer_absent(lane):
    """Independent Master registration observation, never a Guest graph query."""
    deadline = time.monotonic()+10
    while time.monotonic() < deadline:
        lane.check()
        with xmlrpc.client.ServerProxy("http://127.0.0.1:11384", transport=ShortTransport()) as master:
            result = master.getSystemState("/remaining_acceptance")
        require(result[0] == 1 and len(result[2]) == 3, "bounded owned Master graph observation failed")
        counts = [sum(nodes.count("/uwvm_sdk_relay/sdk_peer") for resource, nodes in result[2][kind]
                  if resource in ("/uwvm_sdk_relay/pub", "/uwvm_sdk_relay/sub")) for kind in (0, 1)]
        if counts == [0, 0]: return {"method": "getSystemState", "native_peer_publishers": 0, "native_peer_subscribers": 0,
                                   "scope": "owned controller metadata query, outside Guest effects"}
        time.sleep(.02)
    raise TimeoutError("retired TCPROS peer registration remained visible")


def main():
    parser = argparse.ArgumentParser(description=__doc__); add_arguments(parser)
    parser.add_argument("--prepared-native", type=Path, required=True); parser.add_argument("--wat2wasm", type=Path, required=True)
    parser.add_argument("--case", choices=CASES, required=True); args = parser.parse_args()
    lane = None; status = "FAIL"
    try:
        lane = Lane(args, "dual-backend-native-recovery"); fault = 2 if args.case == "dds-peer-restart" else 1; survivor = 3-fault
        lane.report.update(case=args.case, fault_version=fault, survivor_version=survivor, actual_ROS_delivery="NOT_EXECUTED",
            fault_scope="owned Master outage" if args.case == "master-restart" else "owned native peer endpoint/transport retirement; no global network partition")
        lane.qualify_workers(); [lane.qualify_gateway(version) for version in (1, 2)]
        master, master_row = start_master(lane); lane.lock("domain-205")
        origins = {}
        for version in (1, 2):
            origins[version] = qualify_peer(lane, args.prepared_native, version, "recovery")
            lane.snapshot(origins[version], lane.out/("ros"+str(version))/"tmp/recovery-peer", "recovery-peer-"+str(version))
        def peer(version, mode, old=()):
            child, row = lane.start("peer-ros"+str(version)+"-"+mode, lane.native(version, ["/tmp/recovery-peer", mode, *old], witness=True),
                lane.environment(version), seconds=210)
            proof = lane.stopped(child, row, "recovery-peer-"+str(version), cooperative=True); lane.send(child, row, b"I", "current stopped native ARM map")
            lane.wait_marker(child, row, "READY_RECOVERY_PEER"); return child, row, version, proof["actual_stopped_ARM"]["pid"]
        previous = peer(fault, "before"); retained = peer(survivor, "survivor")
        wasm = translate(lane, args.wat2wasm, "recovery", recovery_guest(fault)); argv = worker_argv(lane, (1, 2), wasm, multi=True)
        worker, worker_row = lane.start("worker", ["/bin/bash", "-c", 'kill -STOP $$; exec "$@"', "recovery-worker", *lane.config["qemu_argv"], *argv], seconds=210, rss=8*1024**3)
        until = time.monotonic()+5
        while time.monotonic() < until and worker.poll() is None and process_identity(worker.pid)["state"] not in ("T", "t"): lane.check(); time.sleep(.001)
        require(worker.poll() is None and process_identity(worker.pid)["state"] in ("T", "t"), "PID-preserving Worker stop missing")
        gateways = []
        for version in (1, 2):
            config = lane.short/("ros"+str(version)+".conf")
            config.write_text(gateway_config(version, worker.pid, False).replace("timeout-ms 10000\n", "timeout-ms 60000\n")); config.chmod(0o600)
            child, row = lane.start("gateway-ros"+str(version), lane.native(version, ["/tmp/gateway", "/guest/"+config.name]), lane.environment(version, gateway=True), seconds=210)
            lane.stopped(child, row, "gateway-"+str(version)); target = lane.short/("ros"+str(version)+".sock"); until = time.monotonic()+30
            while not target.exists() and time.monotonic() < until: lane.check(); require(child.poll() is None, "Gateway retired before readiness"); time.sleep(.01)
            require(target.exists() and stat.S_ISSOCK(target.lstat().st_mode) and target.lstat().st_uid == os.getuid(), "owned Gateway socket missing")
            lane.report.setdefault("gateway_configs", {})[str(version)] = {"pin": lane.pin(config), "text": config.read_text()}; gateways.append((child, row))
        original_check=lane.check
        def check_recovery_roles():
            original_check()
            for gateway,_ in gateways:
                code=gateway.poll()
                require(code is None or type(code) is int and code==0 and worker.poll()==0,
                    "Gateway failed or retired during retained Worker recovery")
        lane.check=check_recovery_roles
        owner = next(item for item in lane.owned.rows.values() if item["identity"]["pid"] == worker.pid)
        lane.owned.send(owner, signal.SIGCONT); lane.stopped(worker, worker_row, "worker"); lane.send(worker, worker_row, b"G", "both native peers/Gateways and current Worker mapped")
        lane.report["worker_argv"] = argv; lane.wait_marker(worker, worker_row, "RECOVERY_ENDPOINTS_CREATED")
        for actor in (previous, retained): lane.wait_marker(actor[0], actor[1], "MATCHED_RECOVERY_PEER")
        matched = next(line for line in lane.read(previous[1]).splitlines() if line.startswith("MATCHED_RECOVERY_PEER "))
        old = tuple(dict(field.split("=", 1) for field in matched.split()[1:])[name] for name in
                    ("own_pub_gid", "own_sub_gid", "gateway_pub_gid", "gateway_sub_gid")) if fault == 2 else ()
        survivor_birth = dict(retained[1]["birth"])
        lane.send(worker, worker_row, b"B", "both independent native exact connection snapshots"); exchange(lane, worker, worker_row, (previous, retained), "before")
        if args.case == "master-restart":
            lane.stop_role(master, master_row, signal.SIGKILL)
            try:
                import socket
                with socket.create_connection(("127.0.0.1", 11384), .2): raise ValueError("owned Master still reachable")
            except ConnectionRefusedError: pass
            lane.report["master_outage"] = {"owned_reaped": True, "numeric_endpoint_unreachable": True, "TCPROS_connection_loss_inferred": False}
        else:
            lane.send(previous[0], previous[1], b"X", "exact before delivery observed by Guest/native")
            lane.wait_marker(previous[0], previous[1], "RETIRED_RECOVERY_PEER"); lane.finish(previous[0], previous[1])
            if fault == 1: lane.report["native_retirement"] = master_peer_absent(lane)
            else: lane.report["native_retirement"] = {"graceful_native_endpoint_shutdown": True, "owned_process_reaped": True,
                "scope": "later fresh peer independently requires old DDS endpoint GIDs gone; no partition claim"}
        lane.send(worker, worker_row, b"X", "selected actual owned fault; other backend role remains live")
        exchange(lane, worker, worker_row, (retained,), "during")
        require(retained[0].poll() is None and retained[1]["birth"] == survivor_birth, "survivor process changed during fault")
        lane.stopped(retained[0], retained[1], "recovery-peer-"+str(survivor), expected_pid=retained[3])
        if args.case == "master-restart":
            master, master_row = start_master(lane, name="master-restored", acquire_lease=False)
            lane.send(previous[0], previous[1], b"X", "owned Master restored after survivor during delivery")
            lane.wait_marker(previous[0], previous[1], "RETIRED_RECOVERY_PEER"); lane.finish(previous[0], previous[1]); lane.report["native_retirement"] = master_peer_absent(lane)
        fresh = peer(fault, "after", old)
        lane.send(worker, worker_row, b"Y", "fresh mapped native peer ready; only fault handles will be replaced")
        lane.wait_marker(worker, worker_row, "RECOVERY_ENDPOINTS_RECREATED"); lane.wait_marker(fresh[0], fresh[1], "MATCHED_RECOVERY_PEER")
        lane.send(worker, worker_row, b"A", "fresh independent native exact endpoint/GID binding")
        exchange(lane, worker, worker_row, (fresh, retained), "after")
        lane.wait_marker(worker, worker_row, "PASS_BACKEND_RECOVERY"); lane.finish(worker, worker_row)
        for actor in (retained, fresh):
            lane.send(actor[0], actor[1], b"Q", "actual Guest PASS and exit0 after all replies")
            lane.wait_marker(actor[0], actor[1], "PASS_RECOVERY_PEER"); lane.finish(actor[0], actor[1])
        lane.report["native_transcripts"] = [native_records(lane.read(actor[1]), actor[2], mode, actor[3])
            for actor, mode in ((previous, "before"), (retained, "survivor"), (fresh, "after"))]
        require(worker.returncode==0 and all(child.poll() in (None,0) for child,_ in gateways),
            "Gateway terminal status differs after actual recovery/native completion")
        lane.report["gateway_retirement"]=[{"pid":child.pid,"returncode_at_completion":child.poll(),
            "scope":"Alive throughout fault/recovery while Worker live; normal0 after verified Guest/native completion"} for child,_ in gateways]
        lane.report.update(actual_ROS_delivery="VERIFIED", retained_survivor_process=True, retained_survivor_Guest_handles=True,
            original_Instance_retained=True, memory0_sentinel_verified=True, new_tests_executed=True); status = "PASS"
    except BaseException as error:
        if lane is not None: lane.report.update(error=repr(error), traceback=traceback.format_exc())
        else: raise
    finally:
        if lane is not None: status = lane.close(status)
    return 0 if status == "PASS" else 1


if __name__ == "__main__": raise SystemExit(main())
