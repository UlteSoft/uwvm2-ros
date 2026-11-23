#!/usr/bin/env python3
"""Real stable ROS v1 capability open -> snapshot -> two restores -> close.

Uses the authenticated management socket and genuine builtin policy gates.
Uses an existing native ROS gateway for launch attestation. --publish additionally
checks a real isolated topic publication before and after restore with the same
native session. This does not qualify external IPC replay or subscriber delivery.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import socket
import subprocess
import struct
import sys
import tempfile
import time
from run_dump_launch_policy import VM, require
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import snapshot


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--execution-replay",action="store_true",help="Record and replay the real WASI/ROS calls without repeating native effects")
    p.add_argument("--binary", type=Path, required=True)
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--runner-json", type=Path)
    p.add_argument("--nested-wasi", action="store_true", help="Restore two Wasm frames, then exercise both WASI and ROS")
    p.add_argument("--recorded", action="store_true", help="Restore local ROS data using independently validated current owners")
    p.add_argument("--closed-events", action="store_true", help="Capture a real retired watch and its undelivered local control records")
    p.add_argument("--group-preload", action="store_true", help="Share a real ROS handle between main and a preloaded Wasm module")
    p.add_argument("--publish", action="store_true", help="With --nested-wasi, preserve a real native publishing session")
    p.add_argument("--post-restore-capture", action="store_true", help="Save and restore again after the restored worker makes actual WASI/ROS calls")
    p.add_argument("--retire-before-restore", action="store_true", help="Refuse restoring recorded handles after the live guest closes them")
    p.add_argument("--indirect-host-calls", action="store_true", help="Resolve the builtin host calls through real Wasm table entries")
    p.add_argument("--forward-host-imports", action="store_true", help="Import WASI via the preloaded module's re-export")
    p.add_argument("--quota-preservation", action="store_true", help="Verify consumed publishing credits cannot be reset by repeated restoration")
    p.add_argument("--wasip1-single", action="store_true", help="Give the calling and forwarding modules different isolated WASI arguments")
    p.add_argument("--deny-forwarded-wasi", action="store_true", help="A permitted provider import must not bypass the calling single policy")
    p.add_argument("--receiver-mutator", type=Path, help="Native typed-codec fixture that forges a different receiver with a valid checksum")
    p.add_argument("--native-root", type=Path, required=True, help="Existing qualified native-ros1/2 receipts and tools")
    args = p.parse_args()
    args.binary = args.binary.resolve()
    args.out = args.out.resolve()
    args.native_root = args.native_root.resolve()
    require(not args.execution_replay or (args.post_restore_capture and not args.quota_preservation),
            "--execution-replay requires --post-restore-capture and no quota exhaustion")
    require(not args.publish or args.nested_wasi, "--publish requires --nested-wasi")
    require(not args.group_preload or args.publish, "--group-preload requires --publish")
    require(not args.closed_events or (args.nested_wasi and not args.publish and not args.group_preload), "--closed-events requires --nested-wasi without --publish")
    require(not args.post_restore_capture or (args.recorded and args.publish and not args.retire_before_restore), "--post-restore-capture requires --recorded --publish without --retire-before-restore")
    require(not args.retire_before_restore or (args.recorded and args.nested_wasi and not args.closed_events), "--retire-before-restore requires --recorded --nested-wasi without --closed-events")
    require(not args.forward_host_imports or args.group_preload, "--forward-host-imports requires --group-preload")
    require(not args.indirect_host_calls or args.nested_wasi, "--indirect-host-calls requires --nested-wasi")
    require(not args.quota_preservation or args.post_restore_capture, "--quota-preservation requires --post-restore-capture")
    require(not args.wasip1_single or args.forward_host_imports, "--wasip1-single requires --forward-host-imports")
    require(not args.deny_forwarded_wasi or args.wasip1_single, "--deny-forwarded-wasi requires --wasip1-single")
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "resource cgroup required")
    require([name for _,name in socket.if_nameindex()]==["lo"], "isolated network namespace required")
    args.out.mkdir(parents=True, exist_ok=False)
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    results = []
    for version in (1, 2):
        folder = args.out / f"ros{version}"; folder.mkdir()
        receipt=json.loads((args.native_root/f"native-ros{version}/receipt.json").read_text())
        gateway=args.native_root/f"native-ros{version}/build/gateway/uwvm-ros{version}-gateway"
        require(hashlib.sha256(gateway.read_bytes()).hexdigest()==receipt["binaries"][str(gateway)], "native gateway identity")
        native_env={**os.environ, **receipt["env"], "PATH":str(args.native_root/"tools")+":"+os.environ["PATH"],
                    "ROS_DOMAIN_ID":"193", "ROS_LOCALHOST_ONLY":"1", "RMW_IMPLEMENTATION":"rmw_fastrtps_cpp",
                    "ROS_MASTER_URI":"http://127.0.0.1:11393", "ROS_IP":"127.0.0.1",
                    "ROS_HOME":str(folder/"ros-home"), "ROS_LOG_DIR":str(folder/"ros-logs")}
        socket_dir=tempfile.TemporaryDirectory(prefix="sr-",dir=Path(__file__).resolve().parents[2]/".tmp")
        socket_path=Path(socket_dir.name)/"g.sock"
        wat = folder / "current.wat"; wasm = wat.with_suffix(".wasm")
        wasi_import = '(import "wasi_snapshot_preview1" "args_sizes_get" (func $args (param i32 i32) (result i32)))' if args.nested_wasi else ""
        publish_import = f'(import "uwvm_ros{version}_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))' if args.publish else ""
        publish = 'i32.const 16 i64.load i32.const 0 i64.const 128 i64.const 12 call $publish if unreachable end' if args.publish else ""
        close = 'i32.const 16 i64.load call $close if unreachable end'
        event_imports = ""
        if args.closed_events:
            event_imports = f'''(import "uwvm_ros{version}_v1" "event_stream_watch" (func $watch (param i64 i64 i64) (result i32)))
              (import "uwvm_ros{version}_v1" "event_stream_poll" (func $poll (param i64 i32 i64 i64 i64) (result i32)))'''
            close = '''i32.const 24 i64.load i32.const 0 i64.const 256 i64.const 128 i64.const 80 call $poll
              if unreachable end i32.const 80 i64.load i64.const 64 i64.ne if unreachable end
              i32.const 256 i32.load i32.const 0x45525755 i32.ne if unreachable end
              i32.const 24 i64.load call $close if unreachable end'''

        body = f'''(func (export "_start")
            i32.const 0 i64.const 0 i64.const 4 i64.const 16 call $open
            if unreachable end
            nop {close})'''
        function, offset = 2, 14
        if args.nested_wasi:
            body = f'''(func (export "_start")
                i32.const 0 i64.const 0 i64.const 4 i64.const 16 call $open
                if unreachable end {publish} call $finish)
              (func $finish
                nop
                i32.const 64 i32.const 68 call $args if unreachable end
                i32.const 64 i32.load i32.const 2 i32.ne if unreachable end
                {publish}
                {close})'''
            function, offset = 4 + int(args.publish), 0
        if args.closed_events:
            body = body.replace("call $finish", '''i32.const 0 i64.const 8 i64.const 6 i64.const 24 call $open if unreachable end
              i32.const 24 i64.load i32.const 16 i64.load i64.const 0 call $watch if unreachable end
              i32.const 16 i64.load call $close if unreachable end call $finish''')
            function += 2
        preload_flags = []
        preload_import = ""
        if args.group_preload:
            library = folder/"control.wat"
            library_wasi = wasi_import+' (export "forward_args" (func $args))' if args.forward_host_imports else ""
            memory_witness = '(func (export "memory_witness") (result i32) i32.const 64 i32.load i32.const 68 i32.load i32.or)' if args.forward_host_imports else ""
            library.write_text(f'''(module
              (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
              {library_wasi}
              (memory 1) (data (i32.const 0) "talk")
              (func (export "acquire") (result i64)
                i32.const 0 i64.const 0 i64.const 4 i64.const 16 call $open
                if unreachable end i32.const 16 i64.load) {memory_witness})''')
            subprocess.run(["/usr/bin/wat2wasm", "--debug-names", str(library), "-o", str(library.with_suffix(".wasm"))], check=True)
            preload_flags = ["--wasm-preload-library", str(library.with_suffix(".wasm")), "control"]
            preload_import = '(import "control" "acquire" (func $acquire (result i64)))'
            body = body.replace("i32.const 0 i64.const 0 i64.const 4 i64.const 16 call $open\n                if unreachable end", "i32.const 16 call $acquire i64.store")
            function += 1
            if args.forward_host_imports:
                wasi_import = '(import "control" "forward_args" (func $args (param i32 i32) (result i32)))'
                preload_import += '(import "control" "memory_witness" (func $memory_witness (result i32)))'
                body = body.replace("i32.const 64 i32.load i32.const 2 i32.ne if unreachable end",
                    "i32.const 64 i32.load i32.const 2 i32.ne if unreachable end call $memory_witness if unreachable end")
                function += 1
        if args.deny_forwarded_wasi:
            body=body.replace("call $args if unreachable end\n                i32.const 64 i32.load i32.const 2 i32.ne if unreachable end",
                "call $args i32.const 76 i32.ne if unreachable end\n                i32.const 64 i32.load if unreachable end")
        if args.recorded and args.closed_events:
            tail = "i32.const 24 i64.load call $close if unreachable end"
            require(tail in body, "closed event fixture tail")
            body = body.replace(tail, "call $after_poll")
            body += f"(func $after_poll nop {tail})"
        if args.post_restore_capture:
            require(body.count(close)==1, "post-call fixture tail")
            body = body.replace(close, "call $after_calls")
            refused_publish = publish.replace("if unreachable end", "i32.const 11 i32.ne if unreachable end") if args.quota_preservation else ""
            body += f"(func $after_calls nop {refused_publish} {close})"
        if args.retire_before_restore:
            require(body.count(close)==1, "closed handle fixture tail")
            body = body.replace(close, close+" call $after_close")
            body += "(func $after_close nop)"
        host_types = host_table = ""
        if args.indirect_host_calls:
            types = {"open":"(param i32 i64 i64 i64) (result i32)", "close":"(param i64) (result i32)"}
            if args.nested_wasi:types["args"]="(param i32 i32) (result i32)"
            if args.publish:types["publish"]="(param i64 i32 i64 i64) (result i32)"
            if args.closed_events:types.update({"watch":"(param i64 i64 i64) (result i32)","poll":"(param i64 i32 i64 i64 i64) (result i32)"})
            host_types=" ".join(f"(type ${name}_t (func {kind}))" for name,kind in types.items())
            host_table=f'(table {len(types)} funcref) (elem (i32.const 0) '+" ".join("$"+name for name in types)+")"
            for n,name in enumerate(types):body=body.replace("call $"+name, f"i32.const {n} call_indirect (type ${name}_t)")
        wat.write_text(f'''(module
          {host_types}
          (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
          (import "uwvm_ros{version}_v1" "capability_close" (func $close (param i64) (result i32)))
          {wasi_import}
          {publish_import}
          {preload_import}
          {event_imports}
          {host_table}
          (memory 1) (data (i32.const 0) "talk") (data (i32.const 8) "events")
          (data (i32.const 128) "\\08\\00\\00\\00restored")
          {body})\n''')
        subprocess.run(["/usr/bin/wat2wasm", "--debug-names", str(wat), "-o", str(wasm)], check=True)
        base = f"--ros{version}-global-"
        flags = [base+"enable", base+"gateway-socket", str(socket_path),
                 base+"gateway-uid", str(os.getuid()), base+"node-name", "checkpoint", base+"namespace", "/"]
        for name in ("capability_open", "capability_close"):
            flags += [base+"interface", f"uwvm_ros{version}_v1.{name}", "allow"]
        if args.publish: flags += [base+"interface", f"uwvm_ros{version}_v1.topic_publish", "allow"]
        if version == 1:
            flags += [base+"master-uri", native_env["ROS_MASTER_URI"], base+"advertise-host", "127.0.0.1", base+"transport", "tcpros"]
            datatype, identity = "std_msgs/String", "992ce8a1687cec8c8bd883ec73ca41d1"
            settings = {"queue-depth":"4", "transport":"tcpros"}
            if not args.closed_events: settings["latch"] = "false"
        else:
            flags += [base+"domain-id", "193", base+"rmw-implementation", "rmw_fastrtps_cpp",
                      base+"security-mode", "disabled-test", base+"keystore", "not-applicable", base+"enclave", "not-applicable"]
            datatype, identity = "std_msgs/msg/String", "RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18"
            settings = {"qos.reliability":"reliable", "qos.durability":"volatile", "qos.history":"keep-last",
                        "qos.depth":"4", "qos.deadline":"infinite", "qos.lifespan":"infinite",
                        "qos.liveliness":"automatic", "qos.liveliness-lease-duration":"infinite"}
        source_operation = "topic.subscribe" if args.closed_events else "topic.publish"
        flags += [base+"grant", "talk", source_operation, "/test/data", datatype, identity]
        settings.update({"overflow-policy":"reject", "validator":"none",
                         "audit":"allow-and-deny" if args.publish else "deny-only"})
        if args.quota_preservation:settings.update({"max-rate-hz":"0.000001", "burst":"2", "max-rate-hz.on-exhausted":"reject"})
        for key,value in settings.items(): flags += [base+"set", "talk", key, value]
        observer_settings = {"event-source-alias":"talk", "event-kinds":"topic-message,resource-error,resource-closed", "overflow-policy":"reject", "audit":"deny-only"}
        if args.closed_events:
            for name in ("event_stream_watch", "event_stream_poll"):
                flags += [base+"interface", f"uwvm_ros{version}_v1.{name}", "allow"]
            flags += [base+"grant", "events", "event.observe", "/test/data", "builtin/event-v1", "builtin:ros-event-envelope-le64-v1"]
            for key,value in observer_settings.items():flags += [base+"set", "events", key, value]
        if args.group_preload:
            grouped = [f"--ros{version}-group-create", "robots",
                       f"--ros{version}-group-add-module", "robots", "robot",
                       f"--ros{version}-group-add-module", "robots", "control"]
            for token in flags:
                if token.startswith(base):
                    grouped += [token.replace("-global-", "-group-"), "robots"]
                else: grouped.append(token)
            flags = grouped
        if args.nested_wasi:
            if args.wasip1_single:
                for name,arguments in [("robot",["robot","joint"]),("control",["control","private","arguments"])]:
                    access="deny" if name=="robot" and args.deny_forwarded_wasi else "allow"
                    flags += ["--wasip1-single-create",name,"--wasip1-single-enable",name,"--wasip1-single-interface",name,"args_sizes_get",access,
                              "--wasip1-single-grant",name,"args","--wasip1-single-force-args",name,str(len(arguments)),*arguments]
            else:
                flags += ["--wasip1-global-enable", "--wasip1-global-interface", "args_sizes_get", "allow",
                          "--wasip1-global-grant", "args",
                          "--wasip1-global-force-args", "2", "robot", "joint"]
        vm = None; actors=[]; logs=[]
        row = {"backend":version, "abi":"v1", "nestedWasi":args.nested_wasi,
               "nativePublish":args.publish, "groupPreload":args.group_preload, "closedEvents":args.closed_events, "recorded":args.recorded,
               "postRestoreCapture":args.post_restore_capture, "retireBeforeRestore":args.retire_before_restore, "passed":False, "restores":[]}
        row.update(indirectHostCalls=args.indirect_host_calls,forwardHostImports=args.forward_host_imports,quotaPreservation=args.quota_preservation,
                   wasip1Single=args.wasip1_single,denyForwardedWasi=args.deny_forwarded_wasi)
        try:
            def launch(name, command):
                log=(folder/(name+".log")).open("wb");logs.append(log)
                process=subprocess.Popen(command,env=native_env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
                actors.append(process);return process
            if version==1:
                master=launch("master",[str(args.native_root/"tools/roscore"),"-p","11393"])
                until=time.monotonic()+30
                while True:
                    with socket.socket() as probe:
                        if probe.connect_ex(("127.0.0.1",11393))==0:break
                    require(master.poll() is None and time.monotonic()<until,"master startup");time.sleep(.01)
            vm = VM([str(args.binary), "-Raot", "-Rdbg", "-Rct", "0", "-Wname", "robot",
                     "--debug-fault-action", "exit", "--runtime-disable-memtag", "-Rllvm-cache-path", "disable", *preload_flags, *flags, "--run", str(wasm)], folder,
                    ["/bin/bash","-c",'kill -STOP $$; exec "$@"',"snapshot-worker",*prefix])
            until=time.monotonic()+20
            while "\nState:\tT" not in Path(f"/proc/{vm.process.pid}/status").read_text():
                require(vm.process.poll() is None and time.monotonic()<until,"worker launch gate");time.sleep(.005)
            fields={"node-name":"checkpoint","namespace":"/"}
            if version==1:fields.update({"master-uri":native_env["ROS_MASTER_URI"],"advertise-host":"127.0.0.1","transport":"tcpros"})
            else:fields.update({"domain-id":"193","rmw-implementation":"rmw_fastrtps_cpp","security-mode":"disabled-test","keystore":"not-applicable","enclave":"not-applicable"})
            contract=folder/"gateway.conf"
            lines=[f"backend ros{version}",f"worker-uid {os.getuid()}",f"worker-pid {vm.process.pid}",f"socket {socket_path}"]
            lines += [f"connection {k} {v}" for k,v in fields.items()]
            lines += [f"grant talk {source_operation} /test/data {datatype} {identity}"]
            lines += [f"set talk {k} {v}" for k,v in settings.items()]
            if args.closed_events:
                lines += ["grant events event.observe /test/data builtin/event-v1 builtin:ros-event-envelope-le64-v1"]
                lines += [f"set events {k} {v}" for k,v in observer_settings.items()]

            contract.write_text("\n".join(lines)+"\n");contract.chmod(0o600)
            peer=launch("gateway",[str(gateway),str(contract)])
            while not socket_path.is_socket():
                require(peer.poll() is None and time.monotonic()<until,"gateway startup");time.sleep(.005)
            os.kill(vm.process.pid,signal.SIGCONT)
            modules = vm.request("info modules")
            module_ids = {name: int(number) for number, name in re.findall(r'^\s*(\d+) = [^\n]+\n\s*name="([^"]+)"', modules, re.MULTILINE)}
            require("robot" in module_ids, "main module mapping unavailable")
            module_id = module_ids["robot"]
            require(not vm.request(f"breakwasm {module_id} {function} {offset}").startswith("error:"), "after-open breakpoint: "+modules)
            vm.request("continue")
            until = time.monotonic()+90
            while True:
                state = vm.request("status")
                if f"function={function} byte-offset={offset}" in state: break
                require(time.monotonic()<until, state); time.sleep(.01)
            stop = re.search(r"stop-id (\d+)", state)[1]
            prior_bindings = (folder/"gateway.log").read_text().count("operation=session.bind ")
            archive = folder/"full.uwsnap"
            reply = vm.request(f"dump wasm full {stop} {archive}")
            require("wasm-dump saved" in reply, reply)
            saved = snapshot.decode(archive.read_bytes()).describe()
            ros_rows = saved.get("ros", [])
            require(len(ros_rows)==1 and ros_rows[0]["backend"]==version, "missing same-stop ROS environment")
            groups = ros_rows[0]["groups"]
            require(len(groups)==1 and groups[0]["scope"]==(2 if args.group_preload else 0) and groups[0]["registered"], "wrong native ROS environment")
            expected_members = [{"module":str(number), "name":name, "registered":True, "retired":False} for name, number in sorted(module_ids.items(), key=lambda x:x[1])]
            require(groups[0]["members"]==expected_members, "ROS member census")
            require(len(groups[0]["handles"])==(2 if args.closed_events else 1), "missing ROS handles")
            require(any(h["active"] for h in groups[0]["handles"]), "missing active ROS handle")
            if args.closed_events:
                watches=groups[0]["observations"]
                require(len(watches)==1 and watches[0]["retired"], "missing retired ROS watch")
                require(watches[0]["local"]["events"] and any(e["kind"]==7 for e in watches[0]["local"]["events"]), "missing unconsumed ROS close event")
                require(any(not h["active"] for h in groups[0]["handles"]), "source retirement not captured")
            require(any(c["alias"]=="talk" for c in groups[0]["capabilities"]), "missing ROS capability data")
            if args.quota_preservation:
                initial_account=next(c for c in groups[0]["capabilities"] if c["alias"]=="talk")["accounting"]
                require(int(initial_account["tokens"])==1, "initial publish must consume exactly one rate token")
            require(not ros_rows[0]["externalEnvironmentCaptured"] and not ros_rows[0]["grantsRestoreAuthority"], "false external completeness")
            row["rosSnapshot"] = ros_rows[0]
            # A recomputed outer checksum must not turn a contradictory ROS
            # payload into executable authority or retire the current workers.
            original = archive.read_bytes()
            cursor = 64
            ros_offset = None
            for _ in range(struct.unpack_from("<Q", original, 56)[0]):
                kind = struct.unpack_from("<H", original, cursor)[0]
                size = struct.unpack_from("<Q", original, cursor+24)[0]
                if kind == version+6: ros_offset = cursor
                cursor += 32+size
            require(ros_offset is not None, "ROS outer record missing")
            payload = ros_offset+32
            mutations = [("completeness", ros_offset+2, 0),
                         ("backend", payload+32, 3-version),
                         ("foreign-recording", payload+16, original[payload+16]^0x80),
                         ("unknown-schema", ros_offset+16, 3)]
            row["rejectedRosArchives"] = []
            for name, offset, value in mutations:
                changed = bytearray(original); changed[offset] = value
                changed[-32:] = hashlib.sha256(changed[:-32]).digest()
                rejected_path = folder/("bad-ros-"+name+".uwsnap")
                rejected_path.write_bytes(changed)
                refused = vm.request(f"dump restore current {stop} {rejected_path}")
                require("error:" in refused, "accepted contradictory ROS archive: "+name)
                require(f"stop-id {stop}\n" in vm.request("status"), "ROS rejection changed execution")
                row["rejectedRosArchives"].append(name)


            if args.recorded:
                # A valid checksum cannot move a saved environment to another
                # current policy group or module, even with matching code.
                fields = [("foreign-member", b"robot", b"other")]
                if args.group_preload: fields.append(("foreign-group", b"robots", b"others"))
                for name, before, after in fields:
                    changed = bytearray(original)
                    target = struct.pack("<Q",len(before))+before
                    index = changed.find(target,payload)
                    require(index>=payload, "missing authority field")
                    changed[index+8:index+8+len(before)] = after
                    changed[-32:] = hashlib.sha256(changed[:-32]).digest()
                    bad = folder/(name+".uwsnap");bad.write_bytes(changed)
                    refused = vm.request(f"dump restore recorded {stop} - {bad}")
                    require("error:" in refused, "recorded environment crossed current authority: "+name)
                    require(f"stop-id {stop}\n" in vm.request("status"), "recorded refusal changed stop")
                    row["rejectedRosArchives"].append(name)
                if args.receiver_mutator and args.forward_host_imports:
                    bad=folder/"foreign-builtin-receiver.uwsnap"
                    subprocess.run([str(args.receiver_mutator.resolve()),str(archive),str(bad)],check=True)
                    refused=vm.request(f"dump restore recorded {stop} - {bad}")
                    require("error:" in refused and "pending" not in refused, "forged builtin receiver accepted: "+refused)
                    require(f"stop-id {stop}\n" in vm.request("status"), "builtin receiver refusal changed execution")
                    row["rejectedRosArchives"].append("foreign-builtin-receiver")
                    bad=folder/"unhandled-replay-history.uwsnap"
                    subprocess.run([str(args.receiver_mutator.resolve()),str(archive),str(bad),"--event"],check=True)
                    refused=vm.request(f"dump restore recorded {stop} - {bad}")
                    require("error:" in refused and "pending" not in refused, "replay history silently ignored: "+refused)
                    require(f"stop-id {stop}\n" in vm.request("status"), "unhandled replay changed execution")
                    row["rejectedRosArchives"].append("unhandled-replay-history")
            if args.recorded and args.closed_events:
                require(not vm.request(f"tbreak bw {module_id} {function+1} 0").startswith("error:"), "after-poll breakpoint")
                require("running" in vm.request("continue"), "consume original local event")
                until=time.monotonic()+90
                while True:
                    state=vm.request("status")
                    if f"function={function+1} byte-offset=0" in state:break
                    require(time.monotonic()<until,state);time.sleep(.01)
                stop=re.search(r"stop-id (\d+)",state)[1]
                consumed=folder/"consumed.uwsnap"
                require("wasm-dump saved" in vm.request(f"dump wasm full {stop} {consumed}"), "capture consumed queue")
                current=snapshot.decode(consumed.read_bytes()).describe()["ros"][0]["groups"][0]["observations"][0]
                require(not current["local"]["events"], "event was not consumed before recorded restoration")
                row["restoresConsumedLocalEvent"] = True
            if args.retire_before_restore:
                require(not vm.request(f"tbreak bw {module_id} {function+1} 0").startswith("error:"), "after-close breakpoint")
                require("running" in vm.request("continue"), "close current handle")
                until=time.monotonic()+90
                while True:
                    state=vm.request("status")
                    if f"function={function+1} byte-offset=0" in state:break
                    require(time.monotonic()<until,state);time.sleep(.01)
                stop=re.search(r"stop-id (\d+)",state)[1]
                refused=vm.request(f"dump restore recorded {stop} - {archive}")
                require("error:" in refused and "pending" not in refused, "recorded handle resurrected after close: "+refused)
                require(f"stop-id {stop}\n" in vm.request("status"), "stale handle rejection changed execution")
                row["closedHandleRefusal"] = refused
            for _ in range(0 if args.retire_before_restore else 2):
                selection = "recorded" if args.recorded else "current"
                bindings = "- " if args.recorded else ""
                reply = vm.request(f"dump restore {selection} {stop} {bindings}{archive}")
                for _ in range(12):
                    if "snapshot-restored" in reply or "pending" not in reply: break
                    reply = vm.request("dump restore continue")
                require("snapshot-restored" in reply, reply)
                stop = re.search(r"stop=(\d+)", reply)[1]
                row["restores"].append(reply)
                refreshed = vm.request("info modules")
                refreshed_ids = {name: int(number) for number, name in re.findall(r'^\s*(\d+) = [^\n]+\n\s*name="([^"]+)"', refreshed, re.MULTILINE)}
                require(refreshed_ids == module_ids, "restore changed logical module IDs / breakpoint ownership")
                module_id = refreshed_ids["robot"]
                points = vm.request("info breakpoints")
                require(f"module={module_id} function={function}" in points, "restore discarded or retargeted the Wasm breakpoint: "+points)
                thread = re.search(r"thread ([1-9][0-9]*)", vm.request("status"))[1]
                backtrace = vm.request(f"bt {thread}")
                require(f"#0 module={module_id} function={function}" in backtrace, backtrace)
                if args.nested_wasi: require(f"#1 module={module_id} function={function-1}" in backtrace, backtrace)
            if args.post_restore_capture:
                if args.execution_replay:
                    started=vm.request(f"dump record start {stop}")
                    require(started.startswith("snapshot-record mode=1 "),started)
                require(not vm.request(f"tbreak bw {module_id} {function+1} 0").startswith("error:"), "post-call breakpoint")
                require("running" in vm.request("continue"), "run restored WASI/ROS calls")
                until=time.monotonic()+90
                while True:
                    state=vm.request("status")
                    if f"function={function+1} byte-offset=0" in state:break
                    require(time.monotonic()<until,state);time.sleep(.01)
                stop=re.search(r"stop-id (\d+)",state)[1]
                latest=folder/"after-restored-calls.uwsnap"
                reply=vm.request(f"dump wasm full {stop} {latest}")
                require("wasm-dump saved" in reply, reply)
                if args.quota_preservation:
                    latest_state=snapshot.decode(latest.read_bytes()).describe()["ros"][0]["groups"][0]
                    account=next(c for c in latest_state["capabilities"] if c["alias"]=="talk")["accounting"]
                    require(int(account["tokens"])==0, "restoration reset consumed capability credits")
                    row["remainingPublishTokens"]=account["tokens"]
                if args.execution_replay:
                    journal=folder/"recorded-calls.uwsnap"
                    saved_journal=vm.request(f"dump record save {stop} {journal}")
                    require("saved=1" in saved_journal and not saved_journal.startswith("error:"),saved_journal)
                    audit=(folder/"gateway.log").read_text()
                    baseline=(audit.count("operation=topic.publish "),audit.count("operation=session.bind "))
                    require(baseline[0]==2 and baseline[1]>0,"native original publish not observed")
                    row["replays"]=[]
                    for replay_round in range(2):
                        restored=vm.request(f"dump replay {stop} {journal}")
                        for _ in range(12):
                            if "pending" not in restored:break
                            restored=vm.request("dump restore continue")
                        require(restored.startswith("snapshot-restored"),restored)
                        continued=vm.request("continue")
                        require(continued.startswith(("running", "stopped:")),continued)
                        until=time.monotonic()+90
                        while True:
                            state=vm.request("status")
                            if f"function={function+1} byte-offset=0" in state:break
                            require(time.monotonic()<until,state);time.sleep(.01)
                        stop=re.search(r"stop-id (\d+)",state)[1]
                        replay_state=vm.request("dump record status")
                        require("mode=4 " in replay_state,replay_state)
                        audit=(folder/"gateway.log").read_text()
                        require((audit.count("operation=topic.publish "),audit.count("operation=session.bind "))==baseline,
                                "replay repeated an external ROS publish or reconnect")
                        require(vm.request("continue").startswith("error:"),"replay endpoint escaped to live I/O")
                        row["replays"].append({"state":replay_state,"nativePublishes":baseline[0],"nativeBindings":baseline[1]})
                reply=vm.request(f"dump restore recorded {stop} - {latest}")
                for _ in range(12):
                    if "snapshot-restored" in reply or "pending" not in reply:break
                    reply=vm.request("dump restore continue")
                require("snapshot-restored" in reply, "fresh capture after restored host calls could not restore: "+reply)
                stop=re.search(r"stop=(\d+)",reply)[1]
                row["restores"].append(reply)
            # Continue skips the current breakpoint once. Close must accept the
            # SAME handle stored by the old incarnation, then return normally.
            require("running" in vm.request("continue"), "continue restored execution")
            code = vm.process.wait(timeout=90)
            require(code==0, f"restored capability_close failed: exit {code}")
            if args.publish:
                audit = (folder/"gateway.log").read_text()
                require(prior_bindings > 0 and audit.count("operation=session.bind ")==prior_bindings, "restoration reconnected the native session")
                require(audit.count("operation=topic.publish ")==2, "both native publish acknowledgements required")
                row["nativeSessionBindings"] = prior_bindings
            row["passed"] = True
        finally:
            if vm:
                row["commands"] = vm.commands
                vm.close()
            for process in reversed(actors):
                if process.poll() is None:os.killpg(process.pid,signal.SIGTERM)
                try:process.wait(timeout=5)
                except subprocess.TimeoutExpired:os.killpg(process.pid,signal.SIGKILL);process.wait()
            for log in logs:log.close()
            socket_dir.cleanup()
            results.append(row)
            (args.out/"results.json").write_text(json.dumps(results, indent=2))
    print(json.dumps({"passed":True, "backends":2, "restores":sum(len(row["restores"]) for row in results)}))


if __name__ == "__main__": main()
