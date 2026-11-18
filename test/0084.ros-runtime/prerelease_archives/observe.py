"""Compare genuine release chat messages on isolated loopback with an optional Jazzy peer."""
import argparse
import errno
import hashlib
import json
import os
from pathlib import Path
import pty
import re
import secrets
import signal
import socket
import subprocess
import threading
import time

from extract import ARCHIVES, member_digest


HELLO = re.compile(r"Hello World: \d+")
MW = re.compile(r"lib(?:rmw[^/]*|rclcpp|fastrtps|fastdds|fastcdr|dcpssacpp|ddskernel)\.so(?:\..*)?$")


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


class Node:
    def __init__(self, command, environment, cwd, path):
        self.log = path
        self.mapped = set()
        master, slave = pty.openpty()
        try:
            self.proc = subprocess.Popen(command, cwd=cwd, env=environment, start_new_session=True,
                                         stdin=subprocess.DEVNULL, stdout=slave, stderr=slave)
        except BaseException:
            os.close(master)
            raise
        finally:
            os.close(slave)
        self.capture = threading.Thread(target=self.read, args=(master,), daemon=True)
        self.capture.start()
        self.identity = {"command": list(map(str, command)), "sha256": sha(Path(command[0]))}

    def read(self, master):
        with os.fdopen(master, "rb", buffering=0) as terminal, self.log.open("wb") as out:
            while True:
                try:
                    data = terminal.read(8192)
                except OSError as error:
                    if error.errno != errno.EIO:
                        raise
                    return
                if not data or out.tell() >= 1024 * 1024:
                    return
                out.write(data.replace(b"\r\n", b"\n"))

    def observe(self):
        try:
            mappings = Path(f"/proc/{self.proc.pid}/maps").read_text().splitlines()
        except FileNotFoundError:
            return
        for line in mappings:
            parts = line.split(None, 5)
            if len(parts) == 6 and "x" in parts[1] and parts[-1].startswith("/"):
                path = Path(parts[-1])
                if MW.fullmatch(path.name):
                    self.mapped.add(path)

    def finish(self):
        self.identity["stopped_by_harness"] = self.proc.poll() is None
        for sig in (signal.SIGINT, signal.SIGTERM, signal.SIGKILL):
            if self.proc.poll() is not None:
                break
            os.killpg(self.proc.pid, sig)
            try:
                self.proc.wait(timeout=1)
            except subprocess.TimeoutExpired:
                continue
        self.capture.join(2)
        self.identity.update(exit=self.proc.returncode, log_sha256=sha(self.log),
                             mapped={str(p): sha(p) for p in sorted(self.mapped)})


def matches(mode, nonce, listener, talker):
    if mode == "jazzy-to-legacy":
        return [nonce] if nonce in listener and "sent 10" in talker else []
    emitted = set(HELLO.findall(talker))
    if mode == "native":
        return sorted(emitted & set(HELLO.findall(listener)))
    received = [bytes.fromhex(h).decode(errors="replace") for h in re.findall(r"^received ([0-9a-f]+)$", listener, re.M)]
    return sorted(emitted.intersection(received))


def run_case(mode, args, environments, release):
    directory = args.output / mode
    directory.mkdir()
    nonce = "wros_" + secrets.token_hex(16)
    sub = ([args.jazzy_witness, "receive", nonce], "jazzy") if mode == "legacy-to-jazzy" else ([args.closure / "listener"], "legacy")
    pub = ([args.jazzy_witness, "send", nonce], "jazzy") if mode == "jazzy-to-legacy" else ([args.closure / "talker"], "legacy")
    nodes = {}
    try:
        for role, (command, kind) in (("listener", sub), ("talker", pub)):
            nodes[role] = Node(command, environments[kind], directory, directory / (role + ".log"))
            if role == "listener":
                time.sleep(0.4)
        deadline = time.monotonic() + 10
        while time.monotonic() < deadline:
            for node in nodes.values():
                node.observe()
            if any(node.proc.poll() is not None for node in nodes.values()):
                break
            time.sleep(0.05)
    finally:
        for node in nodes.values():
            node.finish()
    seen = matches(mode, nonce, nodes["listener"].log.read_text(errors="replace"), nodes["talker"].log.read_text(errors="replace"))
    expected_name = "librmw_opensplice_cpp.so" if release in tuple("alpha" + str(i) for i in range(1, 7)) else "librmw_fastrtps_cpp.so"
    identities = {}
    for role, (_, kind) in (("listener", sub), ("talker", pub)):
        paths = nodes[role].mapped
        library = args.closure / expected_name if kind == "legacy" else args.jazzy_prefix / "lib/librmw_fastrtps_cpp.so"
        identities[role] = any(path.resolve() == library.resolve() for path in paths)
    return {"mode": mode, "nonce": nonce, "payloads": seen, "origin_matches": identities,
            "passed": all(identities.values()) and len(seen) >= (1 if mode == "jazzy-to-legacy" else 3),
            "nodes": {r: n.identity for r, n in nodes.items()}}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--closure", type=Path, required=True)
    p.add_argument("--output", type=Path, required=True)
    p.add_argument("--legacy-lib", type=Path, action="append", default=[])
    p.add_argument("--opensplice-xml", type=Path)
    p.add_argument("--opensplice-home", type=Path)
    p.add_argument("--jazzy-witness", type=Path)
    p.add_argument("--jazzy-prefix", type=Path)
    p.add_argument("--jazzy-lib", type=Path, action="append", default=[])
    p.add_argument("--fastdds-xml", type=Path)
    args = p.parse_args()
    if socket.if_nameindex() != [(1, "lo")]:
        p.error("run inside a private network namespace or VM with only loopback")
    if bool(args.jazzy_prefix) != bool(args.jazzy_witness):
        p.error("supply both the Jazzy prefix and witness for cross-release observations")
    for name in ("closure", "jazzy_witness", "jazzy_prefix", "fastdds_xml", "opensplice_xml", "opensplice_home"):
        value = getattr(args, name)
        if value:
            setattr(args, name, value.resolve(strict=True))
    args.output = args.output.resolve()
    args.output.mkdir(exist_ok=False, parents=True)
    receipt = json.loads((args.closure / "closure.json").read_bytes())
    release = receipt["release"]
    if receipt["asset"] != ARCHIVES[release] or not {"listener", "talker"} <= set(receipt["members"]):
        raise ValueError("expected the recorded upstream release")
    if member_digest(receipt["members"]) != ARCHIVES[release]["linked_elf_sha256"]:
        raise ValueError("release member identities differ from the recorded extraction")
    if {path.name for path in args.closure.iterdir()} != set(receipt["members"]) | {"closure.json"}:
        raise ValueError("unexpected file in release closure")
    for name, member in receipt["members"].items():
        if Path(name).name != name or (args.closure / name).is_symlink() or sha(args.closure / name) != member["sha256"]:
            raise ValueError("changed extracted release member")
    common = {"PATH": "/usr/bin:/bin", "LC_ALL": "C", "HOME": str(args.output), "ROS_HOME": str(args.output),
              "ROS_LOG_DIR": str(args.output), "ROS_DOMAIN_ID": "0"}
    legacy = {**common, "LD_LIBRARY_PATH": os.pathsep.join(map(str, [args.closure, *[p.resolve() for p in args.legacy_lib]]))}
    if args.opensplice_xml:
        legacy["OSPL_URI"] = args.opensplice_xml.as_uri()
    if args.opensplice_home:
        legacy["OSPL_HOME"] = str(args.opensplice_home)
    environments = {"legacy": legacy}
    modes = ["native"]
    if args.jazzy_witness:
        paths = [args.jazzy_prefix / "lib", args.jazzy_prefix / "lib/x86_64-linux-gnu", *[p.resolve() for p in args.jazzy_lib]]
        jazzy = {**common, "AMENT_PREFIX_PATH": str(args.jazzy_prefix), "RMW_IMPLEMENTATION": "rmw_fastrtps_cpp",
                 "LD_LIBRARY_PATH": os.pathsep.join(map(str, paths))}
        if args.fastdds_xml:
            jazzy.update(FASTRTPS_DEFAULT_PROFILES_FILE=str(args.fastdds_xml), FASTDDS_DEFAULT_PROFILES_FILE=str(args.fastdds_xml))
        environments["jazzy"] = jazzy
        modes.extend(["legacy-to-jazzy", "jazzy-to-legacy"])
    results = [run_case(mode, args, environments, release) for mode in modes]
    (args.output / "observations.json").write_text(json.dumps({"release": release, "cases": results}, indent=2) + "\n")
    return 0 if all(row["passed"] for row in results) else 1


if __name__ == "__main__":
    raise SystemExit(main())
