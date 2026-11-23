#!/usr/bin/env python3
"""A rejected in-flight capture must not claim a stopped guest or idle journal."""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time
from run_dump_launch_policy import VM, require


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary",type=Path,required=True)
    parser.add_argument("--out",type=Path,required=True)
    parser.add_argument("--runner-json",type=Path)
    parser.add_argument("--dump-refusal",action="store_true")
    args=parser.parse_args();out=args.out.resolve();out.mkdir(parents=True,exist_ok=False)
    require("/docker-" in Path("/proc/self/cgroup").read_text(),"resource cgroup required")
    runner=json.loads(args.runner_json.read_text()) if args.runner_json else []
    wat=out/"pending.wat";wasm=out/"pending.wasm"
    wat.write_text(r'''(module
      (import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32) (result i32)))
      (memory 1) (data (i32.const 0) "\20\00\00\00\01\00\00\00")
      (func (export "_start") (loop $again
        i32.const 0 i32.const 0 i32.const 1 i32.const 16 call $read drop
        br $again)))''')
    subprocess.run(["/usr/bin/wat2wasm","--debug-names",str(wat),"-o",str(wasm)],check=True)
    read,write=os.pipe();vm=None;result={"passed":False}
    try:
        vm=VM([str(args.binary.resolve()),"-Raot","-Rdbg","-Rct","0","-Rllvm-cache-path","disable",
            "--runtime-disable-memtag","--wasip1-global-enable","--wasip1-global-interface","fd_read","allow",
            "--wasip1-global-grant","stdin.read","--run",str(wasm)],out,runner,stdin=read)
        os.close(read);read=-1
        def stopped():
            until=time.monotonic()+120
            while True:
                status=vm.request("status");matched=re.search(r"stop-id ([1-9][0-9]*)",status)
                if matched and re.search(r"^thread [1-9]",status,re.M):return matched[1],status
                require(time.monotonic()<until,status);time.sleep(.01)
        require("breakpoint" in vm.request("breakwasm 0 1 2"),"loop stop")
        vm.request("continue");stop,_=stopped()
        start=vm.request(f"dump record start {stop}");require(start.startswith("snapshot-record mode=1 "),start)
        vm.request("continue")
        until=time.monotonic()+30
        while True:
            state=vm.request("dump record status")
            matched=re.search(r"events=(\d+)",state)
            if matched and int(matched[1])>=6:break
            require(time.monotonic()<until,state);time.sleep(.01)
        paused=vm.request("pause");require("no complete stop" in paused,paused)
        require(vm.request("status")=="running\n","blocked provider has no complete stop")
        pending=out/"pending.uwsnap";reply=vm.request(f"dump record save {stop} {pending}")
        require(reply.startswith("error: snapshot-record rejected mode=1 ") and
            "; guest running\n" in reply and "stop retained" not in reply and not pending.exists(),reply)
        result["pending_refusal"]=reply
        if args.dump_refusal:
            rejected=out/"pending-dump.uwsnap";reply=vm.request(f"dump wasm full {stop} {rejected}")
            require(reply.startswith("error: wasm-dump rejected") and "; guest running\n" in reply and
                "stop retained" not in reply and not rejected.exists(),reply)
            result["pending_dump_refusal"]=reply
        os.write(write,b"Q");stop,_=stopped()
        saved=out/"returned.uwsnap";reply=vm.request(f"dump record save {stop} {saved}")
        require(reply.startswith("snapshot-record mode=2 ") and "saved=1" in reply,reply)
        memory=vm.request("memory 0 0 16 20");require("51 00 00 00" in memory,memory)
        for _ in range(2):
            reply=vm.request(f"dump replay {stop} {saved}");require(reply.startswith("snapshot-restored"),reply)
            stop,_=stopped();vm.request("continue");stop,_=stopped()
            require(vm.request("memory 0 0 16 20")==memory,"recorded external byte changed")
            require("mode=4 " in vm.request("dump record status"),"actual replay endpoint")
        result["passed"]=True
    finally:
        if vm:result["commands"]=vm.commands;vm.close()
        if read>=0:os.close(read)
        os.close(write);(out/"results.json").write_text(json.dumps(result,indent=2))
    print("pending recording diagnostic PASS; after-return replay PASS",flush=True)


if __name__=="__main__":main()
