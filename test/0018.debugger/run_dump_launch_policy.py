#!/usr/bin/env python3
"""Real VM launch rejection and stopped dump tests. Run in the resource cgroup.

No mock broker, stdout scraping as authority, or guest-provided snapshot inputs.
All output paths are below --out. Only subprocesses launched here are retired.
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
import sys
import time

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
import snapshot


def require(value, message):
    if not value: raise AssertionError(message)


class VM:
    def __init__(self, argv, folder, prefix=(), *, stdin=None, start_paused=False):
        self.channel, child = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        child.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        self.channel.settimeout(120)
        self.log = (folder / "vm.log").open("wb")
        self.commands = []
        command = [*prefix, argv[0], "--debug-jit-control-fd", str(child.fileno()), *argv[1:]]
        if start_paused:
            command = ["/bin/sh", "-c", 'kill -STOP $$; exec "$@"', "authenticated-test-launch", *command]
        self.process = subprocess.Popen(command,
                                        pass_fds=(child.fileno(),), stdin=stdin, stdout=self.log, stderr=subprocess.STDOUT)
        child.close()
        self.pidfd = os.pidfd_open(self.process.pid)

    def request(self, command):
        self.channel.sendall(command.encode())
        value = self.channel.recv(65536).decode("utf-8", "replace")
        self.commands.append({"command": command, "reply": value})
        require(value, "VM closed its management channel")
        return value

    def close(self):
        self.channel.close()
        if self.process.poll() is None:
            signal.pidfd_send_signal(self.pidfd, signal.SIGKILL)
        self.process.wait(timeout=10)
        self.log.close();os.close(self.pidfd)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--wat2wasm", default="/usr/bin/wat2wasm")
    parser.add_argument("--launch-only", action="store_true")
    parser.add_argument("--runner-json", type=Path)
    args = parser.parse_args();args.out.mkdir(parents=True, exist_ok=False)
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "test must run in the resource cgroup")
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    wasm = args.out / "snapshot.wasm"
    wat = wasm.with_suffix(".wat")
    wat.write_text('''(module (memory 1) (global $g (mut i64) (i64.const 13))
      (func (export "_start") (local $i i32)
        (loop $fill
          local.get $i local.get $i i32.const 5 i32.mul i32.const 33 i32.add i32.store8
          local.get $i i32.const 1 i32.add local.tee $i i32.const 32 i32.lt_u br_if $fill)
        global.get $g i64.const 1337 i64.mul global.set $g
        unreachable))\n''')
    subprocess.run([args.wat2wasm, str(wat), "-o", str(wasm)], check=True)
    marker = bytes((i * 5 + 33) & 255 for i in range(32))
    require(marker not in wasm.read_bytes(), "dynamic marker must not occur in original code")
    base = [*prefix, str(args.binary)]
    rows = []
    failures = [[], ["--debug-dump-max"], ["--debug-dump-max", "none"], ["--debug-dump-force", "bad"],
                ["--debug-dump-max", "full", "--debug-dump-force", "none"],
                ["--debug-dump-force", "none", "--debug-dump-max", "diagnostic"],
                ["--debug-dump-max", "full", "--debug-dump-max", "full"],
                ["--debug-dump-force", "diagnostic", "--debug-dump-force", "diagnostic"],
                ["--debug-permissions", "limited", "--debug-dump-force", "full"],
                ["--debug-permissions", "none", "--debug-dump-force", "diagnostic"]]
    # An empty argument list is not an error case: use a deliberately misspelled level.
    failures[0] = ["--debug-dump-max", "FULL"]
    for i, flags in enumerate(failures):
        result = subprocess.run([*base, *flags, "--run", str(wasm)], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=120)
        (args.out / f"reject-{i}.log").write_bytes(result.stdout)
        row = {"test": "launch-rejection", "flags": flags, "exit": result.returncode, "passed": result.returncode == 126}
        rows.append(row)
    help_result = subprocess.run([*base, "--help", "debug"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=120)
    rows.append({"test": "debug-help", "passed": help_result.returncode == 0 and all(x in help_result.stdout for x in (b"--debug-dump-max", b"--debug-dump-force"))})
    if not args.launch_only:
        cases = [("full-default", "full", [], "max=full effective=full", (None, "diagnostic", "full")),
                 ("full-ceiling", "full", ["--debug-dump-max", "diagnostic"], "max=diagnostic effective=diagnostic", (None, "diagnostic")),
                 ("full-forced", "full", ["--debug-dump-force", "diagnostic"], "force=diagnostic effective=diagnostic", (None, "diagnostic")),
                 ("full-forced-full", "full", ["--debug-dump-force", "full"], "force=full effective=full", (None, "full")),
                 ("full-disabled", "full", ["--debug-dump-force", "none"], "force=none effective=none", ()),
                 ("read-only", "read-only", [], "max=full effective=full", (None, "diagnostic", "full")),
                 ("limited", "limited", [], "max=full effective=diagnostic", (None, "diagnostic"))]
        for name, permission, flags, expected, allowed in cases:
            folder = args.out / name;folder.mkdir();vm = None
            row = {"test": name, "passed": False, "exports": []}
            try:
                vm = VM([str(args.binary), "--debug-permissions", permission, "--debug-fault-action", "pause",
                         "-Rct", "0", "-Rllvm-cache-path", "disable", *flags, "--run", str(wasm)], folder, prefix)
                policy = vm.request("dump policy");require(policy == "dump-policy " + expected + "\n", policy)
                deadline = time.monotonic() + 60
                while True:
                    status = vm.request("status")
                    match = re.search(r"stop-id ([1-9][0-9]*)\b", status)
                    if match:break
                    require(time.monotonic() < deadline, status);time.sleep(.05)
                stop = match[1]
                for i, request in enumerate((None, "diagnostic", "full")):
                    path = folder / f"{i}.uwsnap"
                    reply = vm.request("dump wasm " + (request + " " if request else "") + stop + " " + str(path))
                    if request in allowed:
                        require(reply.startswith("wasm-dump saved"), reply)
                        image = snapshot.decode(path.read_bytes())
                        sources = [r for r in image.records if r.kind == 3]
                        require(len(sources) == 1 and sources[0].payload == wasm.read_bytes(), "original code roundtrip")
                        require(image.describe()["diagnostic"]["frames"], "missing fault coordinates")
                        require(path.stat().st_mode & 0o777 == 0o600, "private output mode")
                        if image.disclosure == "diagnostic":
                            require(marker not in image.data and all(r.kind in (1, 3, 4, 5) for r in image.records), "diagnostic leaked dynamic state")
                        else:
                            require(marker in image.data, "full inspection failed to capture actual computed memory bytes")
                        digest = hashlib.sha256(image.data).hexdigest()
                        refusal = vm.request("dump wasm " + stop + " " + str(path))
                        require(not refusal.startswith("wasm-dump saved") and hashlib.sha256(path.read_bytes()).hexdigest() == digest, "existing output overwritten")
                        row["exports"].append({"requested": request, "disclosure": image.disclosure, "sha256": digest, "bytes": len(image.data)})
                    else:
                        require(reply.startswith("error:") and not path.exists(), "denied export created output")
                stale = folder / "stale.uwsnap"
                reply = vm.request("dump wasm " + str(int(stop) + 1) + " " + str(stale))
                require(reply.startswith("error:") and not stale.exists(), "stale stop exported")
                target = folder / "untouched.txt";target.write_text("never replace")
                link = folder / "symlink.uwsnap";link.symlink_to(target.name)
                reply = vm.request("dump wasm " + stop + " " + str(link))
                require(reply.startswith("error:") and link.is_symlink() and target.read_text() == "never replace", "symlink output was followed")
                require(vm.request("dump policy") == policy, "export changed launch policy")
                require(not list(folder.glob(".uwvm-core-*")), "temporary output leaked")
                row["passed"] = True
            except Exception as error:
                row["error"] = repr(error)
            finally:
                if vm:
                    row["commands"] = vm.commands;vm.close()
                rows.append(row)
                (args.out / "results.json").write_text(json.dumps(rows, indent=2))
                print(json.dumps({k:v for k,v in row.items() if k != "commands"}), flush=True)
    (args.out / "results.json").write_text(json.dumps(rows, indent=2))
    require(all(row["passed"] for row in rows), "dump policy matrix has failures; inspect results.json")
    print("PASS actual VM dump launch policy", len(rows), "cases")


if __name__ == "__main__": main()
