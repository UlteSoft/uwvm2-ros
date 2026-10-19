#!/usr/bin/env python3
"""Execute one source-bound SDK or multi-memory real-native delivery row.

Every invocation is serial and output-owned. The parent prepares native peers,
fresh Workers/Gateways and Guest products first. This driver grants only the
explicit pub/sub aliases in positive mode; absent mode has an empty ROS grant
table. A tiny real Wasm main gates the preloaded SDK's _start, so the retained
Worker can be mapped before any application ROS call without editing a SDK.
"""
import argparse
import json
import os
from pathlib import Path
import signal
import socket
import stat
import subprocess
import sys
import time
import traceback
import xmlrpc.client

sys.dont_write_bytecode = True
from lane import Lane, add_arguments
from protocol import LANGUAGES, require, GID, marker_lines
from prepare_native import qualify_peer
from fixtures import memory_guest, sdk_gate
from runtime import digest, elf
from rebuild_affected import rooted
from topic_gate import ShortTransport

STRING_MD5 = "992ce8a1687cec8c8bd883ec73ca41d1"
STRING_HASH = "RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18"


def policy(version, absent=False):
    """All limits are literal, caller-selected and mirrored to native config."""
    connection = {"node-name": "sdk_gateway", "namespace": "/uwvm_sdk_relay"}
    if version == 1:
        connection.update({"master-uri": "http://127.0.0.1:11384", "advertise-host": "127.0.0.1", "transport": "tcpros"})
    else:
        connection.update({"domain-id": "205", "rmw-implementation": "rmw_fastrtps_cpp", "security-mode": "disabled-test",
                           "keystore": "not-applicable", "enclave": "not-applicable"})
    grants = []
    if absent: return connection, grants
    for alias, operation, topic in (("pub", "topic.publish", "pub"), ("sub", "topic.subscribe", "sub")):
        limits = {"max-message-bytes": "64", "max-rate-hz": "100", "burst": "4", "max-bytes-per-second": "65536",
                  "max-queue-bytes": "256", "overflow-policy": "reject", "validator": "none", "audit": "allow-and-deny",
                  "audit-max-rate-hz": "100", "audit-burst": "16", "audit-max-records": "10000", "audit-max-bytes": "1048576",
                  "max-publishers" if alias == "pub" else "max-subscribers": "1"}
        if version == 1:
            limits.update({"queue-depth": "4", "transport": "tcpros"})
            if alias == "pub": limits["latch"] = "false"
        else:
            limits.update({"max-inflight": "1", "qos.reliability": "reliable", "qos.durability": "volatile",
                           "qos.history": "keep-last", "qos.depth": "4", "qos.deadline": "infinite", "qos.lifespan": "infinite",
                           "qos.liveliness": "automatic", "qos.liveliness-lease-duration": "infinite"})
        grants.append({"alias": alias, "operation": operation, "resource": "/uwvm_sdk_relay/"+topic,
                       "type": "std_msgs/String" if version == 1 else "std_msgs/msg/String",
                       "identity": STRING_MD5 if version == 1 else STRING_HASH, "limits": limits})
    return connection, grants


def worker_argv(lane, versions, wasm, sdk_wasm=None, rights=(), absent=False, multi=False, selections=None):
    """No ambient environment or automatic clock/random/socket right is granted."""
    module = "sdk_guest" if sdk_wasm is not None else "main"
    argv = [str(lane.worker)]
    for version in versions:
        prefix = "--ros"+str(version)+"-single-"; connection, grants = selections[version] if selections is not None else policy(version, absent)
        argv += [prefix+"create", module, prefix+"enable", module]
        selected = {**connection, "gateway-socket": str(lane.short/("ros"+str(version)+".sock")),
                    "gateway-uid": str(os.getuid()), "connect-timeout-ms": "10000", "audit-deny-max-rate-hz": "100",
                    "audit-deny-burst": "16", "audit-deny-max-records": "10000", "audit-deny-max-bytes": "1048576"}
        for key, value in selected.items(): argv += [prefix+key, module, value]
        for cap in grants:
            argv += [prefix+"grant", module, *[cap[key] for key in ("alias", "operation", "resource", "type", "identity")]]
            for key, value in cap["limits"].items(): argv += [prefix+"set", module, cap["alias"], key, value]
    for name in (("main", "sdk_guest") if sdk_wasm is not None else ("main",)):
        argv += ["--wasip1-single-create", name, "--wasip1-single-enable", name,
                 "--wasip1-single-noinherit-system-environment", name]
        # Additional startup rights must be explicitly present in the qualified
        # artifact manifest, never inferred because a language happens to fail.
        if name == module:
            for right in rights: argv += ["--wasip1-single-grant", name, right]
    if multi: argv += ["--wasm-feature-enable-multi-memory"]
    argv += ["--wasm-set-main-module-name", "main"]
    if sdk_wasm is not None: argv += ["--wasm-preload-library", str(sdk_wasm), "sdk_guest"]
    return [*argv, "--run", str(wasm)]


def gateway_config(version, worker_pid, absent, selection=None):
    connection, grants = selection if selection is not None else policy(version, absent)
    lines = ["backend ros"+str(version), "worker-uid "+str(os.getuid()), "worker-pid "+str(worker_pid),
             "socket /guest/ros"+str(version)+".sock", "timeout-ms 10000", "max-sessions 2",
             "audit-max-records-per-second 100", "audit-max-records 10000", "audit-max-bytes 1048576"]
    lines += ["connection "+key+" "+value for key, value in connection.items()]
    for cap in grants:
        lines += ["grant "+" ".join(cap[key] for key in ("alias", "operation", "resource", "type", "identity"))]
        lines += ["set "+cap["alias"]+" "+key+" "+value for key, value in cap["limits"].items()]
    return "\n".join(lines)+"\n"


def native_markers(text, version, mode, pid):
    """Check native PID/context/exact effect counts and the whole fixed schema."""
    common = ("backend", "mode", "pid", "node", "gateway_node", "type", "out_topic", "in_topic")
    common += ("master", "transport", "md5") if version == 1 else ("domain", "rmw", "hash", "own_pub_gid", "own_sub_gid")
    gids = () if version == 1 else ("gateway_pub_gid", "gateway_sub_gid")
    count = ("gateway_publishers", "gateway_subscribers")
    effects = ("warmup", "final_out", "reply_send", "excess")
    expected = {"READY_SDK_PEER": common, "MATCHED_SDK_PEER": common+gids+count,
                "DELIVERED_SDK_PEER": common+gids+count+effects,
                "PASS_SDK_PEER": common+(gids if mode == "positive" else ())+count+effects+("quiet_samples", "quiet_ns")}
    rows = marker_lines(text, "", expected)
    names = [name for name, _ in rows]
    require(names == (["READY_SDK_PEER", "MATCHED_SDK_PEER", "DELIVERED_SDK_PEER", "PASS_SDK_PEER"]
                     if mode == "positive" else ["READY_SDK_PEER", "PASS_SDK_PEER"]), "native marker order/cardinality differs")
    stable = None
    for name, fields in rows:
        require(fields["backend"] == str(version) and fields["mode"] == mode and fields["pid"] == str(pid)
                and fields["node"] == "/uwvm_sdk_relay/sdk_peer" and fields["gateway_node"] == "/uwvm_sdk_relay/sdk_gateway"
                and fields["out_topic"] == "/uwvm_sdk_relay/pub" and fields["in_topic"] == "/uwvm_sdk_relay/sub", "native role/context differs")
        if version == 1:
            require(fields["type"] == "std_msgs/String" and fields["md5"] == STRING_MD5 and fields["transport"] == "tcpros"
                    and fields["master"] == "http://127.0.0.1:11384", "TCPROS identity differs")
        else:
            require(fields["type"] == "std_msgs/msg/String" and fields["hash"] == STRING_HASH and fields["domain"] == "205"
                    and fields["rmw"] == "rmw_fastrtps_cpp", "DDS identity differs")
            values = [fields[key] for key in ("own_pub_gid", "own_sub_gid", *gids) if key in fields]
            require(all(GID.fullmatch(value) and value != "0"*32 for value in values) and len(values) == len(set(values)), "native GID width/distinctness differs")
            own = tuple(fields[key] for key in ("own_pub_gid", "own_sub_gid"))
            if stable is None: stable = own
            require(own == stable, "native endpoint changed during SDK case")
        if name != "READY_SDK_PEER":
            require(all(fields[key] == ("1" if mode == "positive" else "0") for key in count), "native matched snapshot differs")
        if name in ("DELIVERED_SDK_PEER", "PASS_SDK_PEER"):
            require(fields["warmup"] in ("0", "1") and fields["final_out"] == ("1" if mode == "positive" else "0")
                    and fields["reply_send"] == ("1" if mode == "positive" else "0") and fields["excess"] == "0", "native effect counts differ")
        if name == "PASS_SDK_PEER":
            require(fields["quiet_samples"].isdigit() and 30 <= int(fields["quiet_samples"]) <= 10000 and
                    fields["quiet_ns"].isdigit() and 2_000_000_000 <= int(fields["quiet_ns"]) <= 10_000_000_000, "native quiet budget differs")
    return rows


def guest_markers(text, mode):
    lines = text.splitlines(); require("\r" not in text and text.endswith("\n"), "canonical Guest log required")
    if mode == "absent":
        require(lines in (["PASS_SDK_DENIED"], ["PASS_SDK_DEFAULT_DENIED"]), "default-deny SDK marker differs"); return lines
    require(len(lines) >= 4 and lines[:2] == ["SDK_ENDPOINTS_CREATED", "SDK_FINAL_SENT"]
            and lines[-2:] == ["SDK_DELIVERED", "PASS_SDK_DELIVERY"]
            and all(line == "SDK_POLL_AGAIN" for line in lines[2:-2]) and len(lines[2:-2]) < 200, "SDK marker protocol differs")
    return lines


def start_master(lane, port=11384, name="master", acquire_lease=True):
    """Reserve a private owned Master; never stop an already existing service."""
    if acquire_lease: lane.lock("master-"+str(port))
    try:
        with socket.create_connection(("127.0.0.1", port), .2): raise ValueError("BLOCKED reserved Master port already occupied")
    except ConnectionRefusedError: pass
    sdk = lane.sdks[1]; lane.evidence.target("master-python", rooted(sdk, "/usr/bin/python3"))
    master, row = lane.start(name, lane.native(1, ["/opt/ros/noetic/bin/roscore", "-p", str(port)]), lane.environment(1,master_port=port), seconds=240)
    until = time.monotonic()+30
    while time.monotonic() < until:
        lane.check(); require(master.poll() is None, "owned Master retired before readiness")
        try:
            with xmlrpc.client.ServerProxy("http://127.0.0.1:"+str(port), transport=ShortTransport()) as client:
                uri = client.getUri("/remaining_acceptance"); pid = client.getPid("/remaining_acceptance")
            if uri[0] == 1 and uri[2].rstrip("/") == "http://127.0.0.1:"+str(port) and pid[0] == 1:
                proof = lane.stopped(master, row, "master-python", expected_pid=pid[2])
                lane.report["master_readiness"] = {"uri": uri, "pid": pid, "mapping": proof}; return master, row
        except (OSError, xmlrpc.client.Error): pass
        time.sleep(.02)
    raise TimeoutError("owned Master readiness deadline")


def compiled_guest(lane, path, language, version, mode):
    """Admit a fresh real product; validation-only or prior mathematical rows do not suffice."""
    report, pin = lane.json(path)
    require(report.get("kind") == "sdk-native-delivery-products" and report.get("status") == "PREPARED"
            and report.get("source_manifest_sha256") == lane.report["source_manifest_sha256"], "fresh delivery product manifest required")
    rows = [row for row in report["products"] if (row["language"], row["version"], row["mode"]) == (language, version, mode)]
    require(len(rows) == 1, "exact one selected delivery product required"); row = rows[0]
    require(row.get("signature_status") == "PASS" and row.get("full_validation_returncode") == 0
            and row.get("compiler_returncode") == 0 and row.get("source_inputs_preserved") is True,
            "actual compilation/signature/full validation missing")
    selected = lane.pin(row["artifact"]["path"]); require(selected == row["artifact"], "Guest product changed")
    require(Path(selected["path"]).read_bytes()[:8] == b"\0asm\x01\0\0\0", "actual Wasm v1 required")
    for control in row["input_pins"]: require(lane.pin(control["path"]) == control, "Guest compiler/SDK input changed")
    packages,package_pin=lane.json(report["toolchain_packages"]["path"])
    require(package_pin==report["toolchain_packages"],"Guest tool/stdlib inventory changed")
    from build_delivery import closure
    require({root:closure(Path(root)) for root in report["toolchain_root_selections"]}==packages,
            "Guest compiler/standard-library package bytes changed after preparation")
    rights = row.get("explicit_wasi_startup_rights", [])
    require(isinstance(rights, list) and len(rights) == len(set(rights)) and
            all(right in ("random", "monotonic-clock") for right in rights), "closed explicit startup-right selection required")
    # Such a right is never guessed from a runtime failure. The manifest must
    # associate the exact startup import with the explicit approved CLI choice.
    if rights: require(row.get("startup_rights_reviewed") is True, "explicit startup authority review missing")
    destination = lane.out/"sdk-guest.wasm"; destination.write_bytes(Path(selected["path"]).read_bytes())
    require(digest(destination) == selected["sha256"], "Guest immutable copy differs")
    lane.report["sdk_product"] = {"manifest": pin, "row": row, "snapshot": lane.pin(destination)}
    return destination, rights


def translate(lane, wat2wasm, name, source):
    tool = lane.pin(wat2wasm); wat = lane.out/(name+".wat"); wat.write_text(source); wasm = lane.out/(name+".wasm")
    child, row = lane.start(name+"-translate", [str(wat2wasm), "--enable-multi-memory", str(wat), "-o", str(wasm)], seconds=30)
    lane.finish(child, row); require(wasm.read_bytes()[:8] == b"\0asm\x01\0\0\0", "translation did not produce Wasm")
    lane.report.setdefault("translations", []).append({"tool": tool, "source": lane.pin(wat), "product": lane.pin(wasm), "command": row})
    return wasm


def main():
    parser = argparse.ArgumentParser(description=__doc__); add_arguments(parser)
    parser.add_argument("--prepared-native", type=Path, required=True); parser.add_argument("--wat2wasm", type=Path, required=True)
    parser.add_argument("--case", choices=("sdk", "memory1"), required=True)
    parser.add_argument("--version", choices=("1", "2", "dual"), required=True)
    parser.add_argument("--language", choices=LANGUAGES); parser.add_argument("--mode", choices=("positive", "absent"), default="positive")
    parser.add_argument("--products", type=Path); parser.add_argument("--node", type=Path); args = parser.parse_args()
    require(args.case != "sdk" or args.language and args.products and args.version != "dual", "SDK needs language/products/single backend")
    require(args.case != "memory1" or args.mode == "positive" and not args.language, "memory1 exact positive scope required")
    require(args.language != "javascript" or args.node, "explicit Node required for JS host facade")
    versions = (1, 2) if args.version == "dual" else (int(args.version),)
    lane = None; status = "FAIL"
    try:
        lane = Lane(args, "sdk-native-delivery" if args.case == "sdk" else "multi-memory-native-delivery")
        lane.report.update(case=args.case, languages=[args.language] if args.language else [], versions=list(versions),
                           sdk_mode=args.mode,
                           explicit_scope="Real Wasm imports + authenticated fixed-PID IPC + independent native exact String; JS host remains outside Wasm")
        lane.qualify_workers()
        for version in versions: lane.qualify_gateway(version)
        # Complete expensive product/package admission before any native peer's
        # finite protocol lifetime starts; setup cannot consume its quiet budget.
        sdk_wasm = None; rights = []
        if args.case == "sdk":
            sdk_wasm, rights = compiled_guest(lane, args.products, args.language, versions[0], args.mode)
            wasm = translate(lane, args.wat2wasm, "sdk-gate", sdk_gate())
        else: wasm = translate(lane, args.wat2wasm, "memory1", memory_guest(versions))
        if 1 in versions: start_master(lane)
        if 2 in versions: lane.lock("domain-205")
        peers = []
        for version in versions:
            origin = qualify_peer(lane, args.prepared_native, version)
            destination = lane.out/("ros"+str(version))/"tmp/peer"
            lane.snapshot(origin, destination, "peer-"+str(version))
            child, row = lane.start("peer-ros"+str(version), lane.native(version, ["/tmp/peer", args.mode], witness=True), lane.environment(version), seconds=120)
            proof = lane.stopped(child, row, "peer-"+str(version), cooperative=True); lane.send(child, row, b"I", "actual stopped native map")
            lane.wait_marker(child, row, "READY_SDK_PEER"); peers.append((version, child, row, proof["actual_stopped_ARM"]["pid"]))
        argv = worker_argv(lane, versions, wasm, sdk_wasm, rights, args.mode == "absent", args.case == "memory1")
        binary = args.language == "javascript"
        worker, worker_row = lane.start("worker", ["/bin/bash", "-c", 'kill -STOP $$; exec "$@"', "remaining-worker",
            *lane.config["qemu_argv"], *argv], stdout=subprocess.PIPE if binary else None, seconds=120, rss=8*1024**3)
        until = time.monotonic()+5
        while time.monotonic() < until and worker.poll() is None and process_state(worker.pid) not in ("T", "t"): lane.check(); time.sleep(.001)
        require(worker.poll() is None and process_state(worker.pid) in ("T", "t"), "Worker PID-preserving shell did not stop")
        lane.report["worker_argv"] = argv
        gateways = []
        for version in versions:
            config = lane.short/("ros"+str(version)+".conf"); config.write_text(gateway_config(version, worker.pid, args.mode == "absent")); config.chmod(0o600)
            child, row = lane.start("gateway-ros"+str(version), lane.native(version, ["/tmp/gateway", "/guest/"+config.name]), lane.environment(version, gateway=True), seconds=120)
            lane.stopped(child, row, "gateway-"+str(version)); until = time.monotonic()+30
            target = lane.short/("ros"+str(version)+".sock")
            while not target.exists() and time.monotonic() < until:
                lane.check(); require(child.poll() is None, "Gateway retired before socket readiness"); time.sleep(.01)
            require(target.exists() and stat.S_ISSOCK(target.lstat().st_mode) and target.lstat().st_uid == os.getuid(), "actual owned Gateway socket absent")
            lane.report.setdefault("gateway_configs", {})[str(version)] = {"pin": lane.pin(config), "text": config.read_text()}
            gateways.append((child, row))
        owner = next(item for item in lane.owned.rows.values() if item["identity"]["pid"] == worker.pid)
        lane.owned.send(owner, signal.SIGCONT); lane.stopped(worker, worker_row, "worker")
        lane.send(worker, worker_row, b"G", "all mapped native roles/Gateway sockets")
        sdk, sdk_row = worker, worker_row
        if binary:
            node_pin = lane.pin(args.node); require(elf(args.node)["e_machine"] == 62, "explicit native host Node required")
            # Preserve descriptor numbers without preexec_fn in a threaded
            # sampler. The small shell merely duplicates inherited private
            # pipes and execs the pinned host Node; it opens no IPC/socket.
            command = ["/bin/bash", "-c", 'exec 3<&"$1" 4>&"$2"; shift 2; exec "$@"', "sdk-private-pipes",
                       str(worker.stdout.fileno()), str(worker.stdin.fileno()), str(args.node),
                       str(lane.source/"sdk/examples/javascript/05-runtime-relay.mjs"), str(versions[0]), args.mode]
            sdk, sdk_row = lane.start("js-host", command, seconds=120, rss=96*1024**2,
                pass_fds=(worker.stdout.fileno(), worker.stdin.fileno()))
            lane.report["js_host"] = {"node": node_pin, "scope": "Host copied-result facade; retained real Wasm relay Instance owns ROS authority"}
        original_check=lane.check
        def check_delivery_roles():
            original_check()
            for gateway,_ in gateways:
                code=gateway.poll()
                # A Gateway may normally observe the retained Worker's pidfd
                # retirement and exit0. During the actual exchange it must
                # remain alive. Final Guest/native transcript checks below
                # still decide acceptance; Worker0 alone never supplies PASS.
                require(code is None or type(code) is int and code==0 and worker.poll()==0,
                    "Gateway failed or retired while its retained Worker was live")
        lane.check=check_delivery_roles
        if args.mode == "positive":
            lane.wait_marker(sdk, sdk_row, "SDK_ENDPOINTS_CREATED")
            for _, child, row, _ in peers: lane.wait_marker(child, row, "MATCHED_SDK_PEER")
            lane.send(sdk, sdk_row, b"P", "all independent native MATCHED_SDK_PEER")
            lane.wait_marker(sdk, sdk_row, "SDK_FINAL_SENT")
            for _, child, row, _ in peers: lane.wait_marker(child, row, "DELIVERED_SDK_PEER")
            for _ in range(200):
                before_count = lane.read(sdk_row).count("SDK_POLL_AGAIN\n")
                lane.send(sdk, sdk_row, b"R", "native actual reply sent; one explicit Guest poll")
                until = time.monotonic()+10
                while time.monotonic() < until:
                    lane.check(); text = lane.read(sdk_row)
                    if "SDK_DELIVERED\n" in text or text.count("SDK_POLL_AGAIN\n") > before_count: break
                    require(sdk.poll() is None, "SDK retired before poll completion"); time.sleep(.005)
                else: raise TimeoutError("one bounded Guest poll reply missing")
                if "SDK_DELIVERED\n" in lane.read(sdk_row): break
            lane.wait_marker(sdk, sdk_row, "PASS_SDK_DELIVERY")
        lane.finish(sdk, sdk_row)
        if sdk is not worker: lane.finish(worker, worker_row)
        guest = guest_markers(lane.read(sdk_row), args.mode); lane.report["guest_markers"] = guest
        for version, child, row, pid in peers:
            lane.send(child, row, b"Q", "SDK exact markers + actual retained Worker exit0")
            lane.finish(child, row); parsed = native_markers(lane.read(row), version, args.mode, pid)
            lane.report["cases"].append({"version": version, "mode": args.mode, "language": args.language,
                "status": "PASS", "native_markers": parsed, "native_pid": pid, "Guest_proc_exit": worker.returncode,
                "denial13_and_sentinels": True, "stale9": args.mode == "positive",
                "memory1_complete_reply_and_memory0_untouched": args.case == "memory1"})
        require(worker.returncode==0 and all(child.poll() in (None,0) for child,_ in gateways),
            "Gateway terminal status differs after actual Guest/native completion")
        lane.report["gateway_retirement"]=[{"pid":child.pid,"returncode_at_completion":child.poll(),
            "scope":"Alive during retained Worker exchange; normal0 allowed only after Worker0 and final Guest/native assertions"} for child,_ in gateways]
        lane.report["new_tests_executed"] = True; status = "PASS"
    except BaseException as error:
        if lane is not None: lane.report.update(error=repr(error), traceback=traceback.format_exc())
        else: raise
    finally:
        if lane is not None: status = lane.close(status)
    return 0 if status == "PASS" else 1


def process_state(pid):
    from run_action_qos import process_identity
    return process_identity(pid)["state"]


if __name__ == "__main__": raise SystemExit(main())
