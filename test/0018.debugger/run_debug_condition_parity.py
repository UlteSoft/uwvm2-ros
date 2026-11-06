#!/usr/bin/env python3
"""Live source and Wasm conditional-breakpoint checks on an SDK C example.

Supply the O0/DWARF5 Wasm compiled from static_pointer_library/algorithm.c and
an entry that repeatedly calls transform_and_sum with length 16.
All output, including broker private directories, must be under repo .tmp.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys
import tempfile

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("vm", "wasm", "source", "out"):
        parser.add_argument("--"+name, type=Path, required=True)
    parser.add_argument("--harness", type=Path, default=Path(__file__).with_name("run_dap_current_broker.py"))
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[2]
    args.out = args.out.resolve()
    if not args.out.is_relative_to(root / ".tmp"):
        parser.error("all generated artifacts must be in the repository .tmp")
    args.out.mkdir(parents=True)
    spec = importlib.util.spec_from_file_location("condition_live", args.harness.resolve())
    live = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = live
    sys.path.insert(0, str(args.harness.resolve().parent))
    spec.loader.exec_module(live)
    mk = tempfile.mkdtemp
    def local(*a, **kw):
        kw["dir"] = str(root / ".tmp")
        return mk(*a, **kw)
    live.tempfile.mkdtemp = local
    result = {"passed": False, "checks": [], "transcript": [],
              "wasmSha256": hashlib.sha256(args.wasm.read_bytes()).hexdigest(),
              "sdkSourceSha256": hashlib.sha256(args.source.read_bytes()).hexdigest()}
    server = client = None
    def check(name, value):
        assert value, name
        result["checks"].append(name)
    def cmd(text):
        answer = client.evaluate(text).decode()
        result["transcript"].append([text, answer])
        return answer
    def stop():
        for _ in range(64):
            answer = cmd("wait")
            if answer.startswith("stopped:"):
                return answer
        raise AssertionError("no actual stopped state")
    def register(text):
        answer = cmd(text)
        match = re.search(r"breakpoint (\d+)", answer)
        assert match, answer
        return int(match[1]), answer
    def run():
        cmd("continue")
        return stop()
    def value(name, expected):
        location = live.actual_location(client)
        frame = client.request("stackTrace", {"threadId": location["thread"]})["body"]["stackFrames"][0]
        answer = client.request("evaluate", {"expression": name, "context": "watch", "frameId": frame["id"]})["body"]
        check(f"{name}={expected}", answer["result"] == str(expected))
        return location

    try:
        server = live.BrokerSession(args.vm.resolve(), root/"tools/debug/secure_server.py",
            root/"tools/debug/dap_adapter.py",
            ["-Raot", "-Rct", "1", "-Rllvm-cache-path", "disable",
             "--wasm-set-main-module-name", "main", "--wasip1-single-create", "main",
             "--wasip1-single-enable", "main", "--run", str(args.wasm.resolve())], args.out)
        client = server.connect("source")
        cmd("pause"); stop()
        target = f"bs 0 {args.source.resolve()}:6"
        witness = f"bs 0 {args.source.resolve()}:9"
        point, registration = register(target + " if i == 2")
        function, offset = map(int, re.search(r"function=(\d+) byte-offset=(\d+)", registration).groups())
        answer = run()
        check("source true condition stops without evaluation error", "breakpoint-condition " not in answer)
        value("i", 2)
        cmd(f"condition {point} false")
        sentinel, _ = register(witness)
        answer = run()
        check("source false condition reaches return witness", f"{args.source.resolve()}:9:" in answer)
        check("false condition evaluated on actual hits", int(re.search(rf"breakpoint {point} .*hits=(\d+)", cmd("info breakpoints"))[1]) > 1)
        cmd(f"delete {sentinel}")
        cmd(f"condition {point} i == 2")
        cmd(f"ignore {point} 3")
        answer = run()
        check("source updated predicate with ignore works", "breakpoint-condition " not in answer)
        value("i", 2)
        cmd(f"disable {point}")
        sentinel, _ = register(witness)
        answer = run()
        check("disabled source breakpoint stays disabled", f"{args.source.resolve()}:9:" in answer)
        cmd(f"delete {sentinel}"); cmd(f"enable {point}")
        cmd(f"condition {point} missing_condition_variable == 0")
        answer = run()
        check("unavailable source predicate retains real stop", "unavailable code=" in answer and "actual stop retained" in answer)
        prior = live.actual_location(client)
        for rejected in (f"condition {point} system()", f"condition {point} i = 0", "watch i"):
            client.request("evaluate", {"expression": rejected, "context": "repl"}, success=False)
            check("denied without continuing: "+rejected, live.actual_location(client)["stop"] == prior["stop"])
        cmd(f"condition {point}")
        answer = run()
        check("removed source predicate is unconditional", "breakpoint-condition " not in answer)
        cmd(f"delete {point}")

        point, _ = register(f"bw 0 {function} {offset} ignore 3 if i == 2")
        answer = run()
        check("Wasm condition and ignore stop at requested opcode", f"byte-offset={offset} " in answer and "breakpoint-condition " not in answer)
        value("i", 2)
        check("Wasm ignored actual hits before checking predicate", int(re.search(rf"breakpoint {point} .*hits=(\d+)", cmd("info breakpoints"))[1]) >= 4)
        cmd(f"condition {point} false")
        sentinel, _ = register(witness)
        answer = run()
        check("false Wasm condition reaches witness", f"{args.source.resolve()}:9:" in answer)
        cmd(f"delete {sentinel}")
        cmd(f"condition {point} missing_condition_variable")
        answer = run()
        check("unavailable Wasm condition retains stop", "unavailable code=" in answer and "actual stop retained" in answer)
        cmd(f"condition {point}")
        answer = run()
        check("Wasm condition removal", "breakpoint-condition " not in answer)
        cmd(f"disable {point}"); cmd(f"enable {point}"); cmd(f"delete {point}")
        check("breakpoint deletion", f"breakpoint {point} " not in cmd("info breakpoints"))
        result["passed"] = True
    except BaseException as error:
        result["error"] = repr(error)
    finally:
        if client:
            try: client.close()
            except BaseException as error: result.update(passed=False, clientCleanupError=repr(error))
        if server:
            try: result["cleanup"] = server.close()
            except BaseException as error: result.update(passed=False, cleanupError=repr(error))
        (args.out/"results.json").write_text(json.dumps(result, indent=2)+"\n")
        print(json.dumps({"passed": result["passed"], "checks": len(result["checks"]), "error": result.get("error")}), flush=True)
    return int(not result["passed"])

if __name__ == "__main__":
    raise SystemExit(main())
