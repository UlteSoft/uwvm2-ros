"""Generic Linux actor evidence, independent of the old AArch64-only Lane.

An executable's architecture is parsed from its actual bytes. A declared QEMU
must itself be ELF62 and must be /proc/PID/exe; the target's stopped maps must
bind an opened target map_files descriptor. Native execution requires the target itself
as /proc/PID/exe. No binfmt registration or process-group signal is used.
"""
from pathlib import Path
import importlib.util
import hashlib
import json
import os
import select
import signal
import stat
import struct
import subprocess
import sys
import time
import types

from profile import ARCHES, require

ROOT = Path(__file__).resolve().parents[5]


def load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def json_input(path):
    """Parse the same bounded stable descriptor whose physical pin is retained."""
    path=Path(path).resolve(strict=True);fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(fd);require(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=16*1024**2,"bounded regular JSON required")
        pieces=[];count=0
        while True:
            part=os.read(fd,65536)
            if not part:
                break
            count+=len(part);require(count<=16*1024**2,"JSON grew beyond bound");pieces.append(part)
        last=os.fstat(fd);named=path.stat(follow_symlinks=False)
        require(count==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in
            ("st_dev","st_ino","st_size","st_mtime_ns","st_ctime_ns")),"JSON changed while read")
        raw=b"".join(pieces)
        def pairs(items):
            value={}
            for key,item in items:
                require(key not in value,"duplicate JSON field");value[key]=item
            return value
        return json.loads(raw,object_pairs_hook=pairs),{"path":str(path),"device":first.st_dev,"inode":first.st_ino,
            "bytes":count,"mtime_ns":first.st_mtime_ns,"ctime_ns":first.st_ctime_ns,"sha256":hashlib.sha256(raw).hexdigest()}
    finally:
        os.close(fd)


def helpers(source,manifest):
    require(source == ROOT, "invoke the actual entry from its complete frozen source")
    frozen,_=json_input(manifest);path=source/"tools/ros_distro/common.py"
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(fd);require(stat.S_ISREG(first.st_mode) and 0<first.st_size<=1024**2,"bounded regular frozen helper required")
        raw=b""
        while True:
            part=os.read(fd,65536)
            if not part:
                break
            raw+=part;require(len(raw)<=1024**2,"helper grew beyond bound")
        last=os.fstat(fd);named=path.stat(follow_symlinks=False)
        require(len(raw)==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in
            ("st_dev","st_ino","st_size","st_mtime_ns","st_ctime_ns")),"helper changed while opened")
    finally:
        os.close(fd)
    require(frozen.get("tools/ros_distro/common.py")=={"bytes":len(raw),"sha256":hashlib.sha256(raw).hexdigest()},
            "bind shared helper to actual frozen manifest before import")
    module=types.ModuleType("uwvm_multiarch_primitives");module.__file__=str(path)
    sys.modules[module.__name__]=module
    exec(compile(raw,str(path),"exec"),module.__dict__)  # Execute only the opened, pinned bytes on a future actual invocation.
    return module


def elf(path, common):
    pin = common.opened(Path(path).resolve(strict=True))
    with Path(pin["path"]).open("rb") as stream:
        data = stream.read(64)
        require(len(data) >= 52 and data[:4] == b"\x7fELF" and data[4] in (1, 2) and data[5] in (1, 2), "actual ELF required")
        order = "<" if data[5] == 1 else ">"
        machine, bits = struct.unpack_from(order+"H", data, 18)[0], 32 if data[4] == 1 else 64
        kind = struct.unpack_from(order+"H", data, 16)[0]
        offset = struct.unpack_from(order+("I" if bits == 32 else "Q"), data, 28 if bits == 32 else 32)[0]
        size, count = struct.unpack_from(order+"HH", data, 42 if bits == 32 else 54)
        require(count <= 512 and (not count or size >= (32 if bits == 32 else 56)), "bounded ELF program headers required")
        interpreter = None
        for i in range(count):
            require(offset+(i+1)*size <= pin["bytes"], "program header outside file")
            stream.seek(offset+i*size);header = stream.read(size)
            if struct.unpack_from(order+"I", header)[0] == 3:
                require(interpreter is None, "duplicate ELF interpreter")
                start = struct.unpack_from(order+("I" if bits == 32 else "Q"), header, 4 if bits == 32 else 8)[0]
                length = struct.unpack_from(order+("I" if bits == 32 else "Q"), header, 16 if bits == 32 else 32)[0]
                require(1 < length <= 4096 and start+length <= pin["bytes"], "bounded complete PT_INTERP required")
                stream.seek(start);raw = stream.read(length)
                require(raw.endswith(b"\0") and b"\0" not in raw[:-1], "canonical PT_INTERP required")
                interpreter = raw[:-1].decode("ascii")
    require(common.opened(Path(pin["path"])) == pin, "ELF changed while parsing")
    return {"input": pin, "machine": machine, "bits": bits, "endian": "little" if data[5] == 1 else "big",
            "type": kind, "interpreter": interpreter}


def executable(path, descriptor, common):
    value = elf(path, common)
    require((value["machine"], value["bits"], value["endian"]) == ARCHES[descriptor["architecture"]]
            and value["type"] in (2, 3), "selected target architecture differs from executable ELF")
    execution = descriptor["execution"]
    if value["interpreter"] is not None:
        require(execution["loader"] is not None, "explicit target loader required for dynamic ELF")
        loader = Path(execution["loader"]).resolve(strict=True)
        expected = Path(execution["sysroot"])/value["interpreter"].lstrip("/") if execution["sysroot"] else Path(value["interpreter"])
        require(expected.resolve(strict=True) == loader, "PT_INTERP differs from the explicit physical loader")
        target_loader = elf(loader, common)
        require((target_loader["machine"], target_loader["bits"], target_loader["endian"]) == ARCHES[descriptor["architecture"]], "loader has another architecture")
        value["loader"] = target_loader
    if execution["mode"] == "qemu":
        emulator = elf(execution["qemu"], common)
        require((emulator["machine"], emulator["bits"], emulator["endian"]) == ARCHES["x86_64"], "user-QEMU must be actual host ELF62")
        value["emulator"] = emulator
    return value


def argv(descriptor, binary, tail=()):
    selected = descriptor["execution"]
    prefix = [] if selected["mode"] == "native" else [selected["qemu"], "-L", selected["sysroot"]]
    return [*prefix, str(binary), *map(str, tail)]


def environment(descriptor=None):
    result = {"PATH": "/usr/bin:/bin", "HOME": "/nonexistent", "LANG": "C.UTF-8", "LC_ALL": "C.UTF-8",
              "PYTHONDONTWRITEBYTECODE": "1", "GOMAXPROCS": "1", "MAKEFLAGS": "-j1", "CMAKE_BUILD_PARALLEL_LEVEL": "1"}
    if descriptor:
        result.update(descriptor["execution"]["environment"])
    return result


class Frame:
    """Complete source/tool/SDK before-after binding, with a current64 guard."""
    def __init__(self, source, manifest, profile_path, profile, out, profile_pin=None):
        require(sys.platform == "linux" and hasattr(os, "pidfd_open"), "BLOCKED: Linux pidfds and procfs required")
        self.source, self.args = Path(source).resolve(strict=True), None
        self.common = helpers(self.source,manifest);self.profile = profile
        self.out = Path(out).resolve();require(not self.out.exists() and self.out!=self.source and
            self.source not in self.out.parents and self.out not in self.source.parents, "fresh output outside frozen source required")
        roots = [Path(x).resolve(strict=True) for x in profile["roots"]]
        require(all(x != Path("/") and x != self.out and x not in self.out.parents for x in roots), "output cannot mutate a selected root")
        self.out.mkdir(mode=0o700, parents=True)
        self.pins = [self.common.opened(Path(profile_path).resolve(strict=True)), self.common.opened(Path(manifest).resolve(strict=True)),
                     self.common.opened(Path(profile["source_lock"]).resolve(strict=True))]
        require(profile_pin is None or profile_pin==self.pins[0],"parsed profile changed before admission")
        self.manifest = Path(manifest).resolve(strict=True)
        self.source_before = self.common.source_state(self.source, self.manifest)
        self.frozen,manifest_pin = self.common.document(self.manifest)
        require(manifest_pin==self.pins[1]==self.source_before["manifest"],"manifest changed during admission")
        mapping_path = self.source/"test/0073.ros-tests/support/source/ros_aarch64/runtime.py"
        mapping_raw,mapping_pin = self.common.body_pin(mapping_path,limit=1024**2)
        require(self.frozen.get("test/0073.ros-tests/support/source/ros_aarch64/runtime.py") ==
                {"bytes":len(mapping_raw),"sha256":mapping_pin["sha256"]},
                "opened mapping helper must match the actual frozen manifest before execution")
        self.pins.append(mapping_pin)
        self.mapping = types.ModuleType("uwvm_multiarch_opened_mappings");self.mapping.__file__ = str(mapping_path)
        exec(compile(mapping_raw,str(mapping_path),"exec"),self.mapping.__dict__)
        require(all("test/0073.ros-tests/support/source/ros_multiarch_runtime/"+name in self.frozen for name in
                    ("profile.py", "engine.py", "prepare.py", "run.py", "stop.h", "CMakeLists.txt")), "freeze must include new generic lane")
        self.before = self.common.cgroup_state(profile["cgroup"])
        self.roots = roots;self.inventories = []
        for i, root in enumerate(roots):
            self.inventories.append(self.common.catalog(root, self.out/("inputs-before-%02d.jsonl" % i), roots))
        self.report = {"kind": "multiarch-runtime", "status": "RUNNING", "source": str(self.source),
                       "source_manifest_sha256": self.pins[1]["sha256"], "profile": profile, "cgroup_before": self.before,
                       "runtime_architecture": profile["runtime"]["architecture"], "native_ros_architecture": profile["native"]["architecture"],
                       "native_DDS_scope": "Only the separately recorded native ROS architecture; never the emulated Runtime architecture",
                       "cases": [], "commands": [], "process_cleanup": [], "guest_authority": "Empty until explicit per-case grant selection"}
        self.runner = None;self.save()
        self.context_locks=[]
        self.socket_bindings=[]

    def save(self):
        self.common.atomic_json(self.out/"report.json", self.report)

    def guard(self):
        self.common.cgroup_preserved(self.before, self.common.cgroup_state(self.profile["cgroup"]))

    def pin(self, path):
        value = self.common.opened(Path(path).resolve(strict=True));self.pins.append(value);return value

    def json(self, path):
        value, pin = self.common.document(Path(path).resolve(strict=True));self.pins.append(pin);return value, pin

    def image(self,path,descriptor):
        value=executable(path,descriptor,self.common)
        self.pins.append(value["input"])
        for key in ("loader","emulator"):
            if key in value:
                self.pins.append(value[key]["input"])
        return value

    def runner_for(self, out, compiler=False):
        out.mkdir(mode=0o700, parents=True, exist_ok=False)
        limit = self.profile["limits"]
        self.runner = Actors(self, out, {"command_seconds": limit["compile_seconds"] if compiler else limit["actor_seconds"],
            "rss_bytes": limit["compiler_rss"] if compiler else limit["worker_rss"]+4*limit["native_rss"], "log_bytes": limit["log_bytes"]})
        return self.runner

    def finish(self, status, error=None):
        try:
            if self.runner:
                self.report["process_cleanup"].append(self.runner.close())
                self.report["commands"] += self.runner.commands
                self.runner = None
            self.cleanup_contexts()
            self.report["inputs_preserved"] = all(self.common.opened(Path(p["path"])) == p for p in self.pins)
            self.report["products_preserved"] = self.report["inputs_preserved"]
            after = [self.common.catalog(root, self.out/("inputs-after-%02d.jsonl" % i), self.roots) for i, root in enumerate(self.roots)]
            self.report["whole_roots_preserved"] = all({k:v for k,v in a.items() if k != "catalog"} ==
                {k:v for k,v in b.items() if k != "catalog"} for a,b in zip(self.inventories,after))
            self.report["source_preserved"] = self.common.source_state(self.source,self.manifest) == self.source_before
            self.guard();self.report["cgroup_after"] = self.common.cgroup_state(self.profile["cgroup"])
            require(all(self.report[k] for k in ("inputs_preserved", "whole_roots_preserved", "source_preserved")) and
                    all(r["status"] == "PASS" for r in self.report["process_cleanup"]), "preservation or owned cleanup incomplete")
        except BaseException as caught:
            status = "FAIL";self.report["finalization_error"] = repr(caught)
        self.report.update(status=status, error=error, input_pins=self.pins, inventories_before=self.inventories)
        self.save();return 0 if status in ("PASS", "PREPARED") else 1

    def cleanup_contexts(self):
        """Retire only captured private socket inodes after owned actors died."""
        require(self.runner is None,"cannot release context while actors remain owned")
        require(all(x["status"]=="PASS" for x in self.report["process_cleanup"]),"cannot release context after incomplete actor cleanup")
        for item in self.socket_bindings:
            path=Path(item["path"])
            if path.exists():
                state=path.lstat()
                require(stat.S_ISSOCK(state.st_mode) and (state.st_dev,state.st_ino,state.st_uid)==
                        (item["device"],item["inode"],item["uid"]),"owned socket was replaced")
                path.unlink()
        for fd in self.context_locks:
            os.close(fd)
        self.context_locks.clear()
        self.report["remaining_owned_sockets"]=[str(p) for name in self.report.get("socket_directories",[]) for p in Path(name).iterdir() if p.is_socket()]
        require(not self.report["remaining_owned_sockets"],"unproven socket remains in the exclusive owner directory")


class Actors:
    """Asynchronous gates on top of the reviewed common pidfd ownership tracker."""
    def __init__(self, frame, out, limits):
        self.frame, self.out, self.limits = frame, out, limits
        self.owner = frame.common.SerialRunner(out, limits, frame.guard)
        self.commands = self.owner.commands;self.rows = []

    def start(self, name, command, env, descriptor=None, target=None, stdout_pipe=False, pass_fds=()):
        require(name not in {r[1]["name"] for r in self.rows}, "unique actor label required")
        self.frame.guard();out, err = self.out/(name+".stdout"),self.out/(name+".stderr")
        row = {"name": name, "argv": command, "environment": env, "stdout": str(out), "stderr": str(err),
               "status": "RUNNING", "deadline": time.monotonic()+self.limits["command_seconds"], "peak_owned_rss_bytes": 0,
               "rss_limit":self.frame.profile["limits"]["worker_rss" if name=="worker" else "native_rss"]}
        self.commands.append(row)
        with out.open("xb") as output, err.open("xb") as errors:
            child = subprocess.Popen(command, env=env, cwd=self.out, stdin=subprocess.PIPE,
                stdout=subprocess.PIPE if stdout_pipe else output, stderr=errors, pass_fds=pass_fds)
            self.owner.children.append(child);self.rows.append((child,row))  # Retain before procfs/pidfd can fail.
            first = self.frame.common.identity(child.pid);self.owner.roots[child.pid] = first;row["first_identity"] = first
            self.owner.acquire(child.pid,os.getpid())
            require(any(k[0] == child.pid for k in self.owner.owned), "live root birth admission missing")
        row["pid"] = child.pid
        if descriptor is not None:
            row["executable"] = executable(target,descriptor,self.frame.common)
        self.frame.save();return child,row

    def check(self):
        self.frame.guard();self.owner.discover()
        rss = self.owner.rss()
        require(rss <= self.limits["rss_bytes"], "combined owned actor RSS bound exhausted")
        for child,row in self.rows:
            owned={child.pid};items=list(self.owner.owned.values())
            for _ in range(len(items)):
                prior=len(owned)
                owned.update(item["identity"]["pid"] for item in items if item["identity"]["parent_pid"] in owned)
                if len(owned)==prior:
                    break
            actual=0
            for item in items:
                original=item["identity"]
                if original["pid"] not in owned or self.frame.common.birth(self.frame.common.identity(original["pid"]))!=self.frame.common.birth(original):
                    continue
                try:
                    for line in (Path("/proc")/str(original["pid"])/"status").read_text().splitlines():
                        if line.startswith("VmRSS:"):
                            actual+=int(line.split()[1])*1024;break
                except (FileNotFoundError, ProcessLookupError):
                    pass
            row["peak_owned_rss_bytes"] = max(row["peak_owned_rss_bytes"],actual)
            require(actual<=row["rss_limit"],"per-actor subtree RSS bound exhausted")
            if child.poll() is None:
                require(time.monotonic() < row["deadline"], "finite actor deadline exhausted")
            require(all(Path(row[k]).stat().st_size <= self.limits["log_bytes"] for k in ("stdout", "stderr")), "actor log budget exhausted")

    def record(self, child):
        return next(item for key,item in self.owner.owned.items() if key[0] == child.pid)

    def signal(self, child, sig):
        self.check();record = self.record(child)
        require(self.frame.common.birth(self.frame.common.identity(child.pid)) == self.frame.common.birth(record["identity"]), "signal birth changed")
        signal.pidfd_send_signal(record["fd"],sig)

    def stopped(self, child, row, descriptor, target):
        image = executable(target,descriptor,self.frame.common)
        principal = image.get("emulator",image)["input"]
        wanted = dict(image["input"], elf_class=2 if image["bits"]==64 else 1,
                      elf_data=1 if image["endian"]=="little" else 2, e_machine=image["machine"])
        until = time.monotonic()+20
        while time.monotonic() < until:
            self.check();require(child.poll() is None,"actor exited before mapping barrier")
            subtree={child.pid}
            for _ in range(len(self.owner.owned)):
                subtree.update(item["identity"]["pid"] for item in self.owner.owned.values()
                               if item["identity"]["parent_pid"] in subtree)
            candidates=[]
            for item in self.owner.owned.values():
                original=item["identity"];pid=original["pid"]
                now=self.frame.common.identity(pid)
                if (pid not in subtree or self.frame.common.birth(now)!=self.frame.common.birth(original)
                        or now["state"] not in ("T","t") or self.frame.common.pidfd_terminal(item["fd"])):continue
                host=(Path("/proc")/str(pid)/"exe").stat()
                if self.frame.mapping.mapping_file_facts(host)!={key:principal[key] for key in self.frame.mapping._MAP_FACTS}:continue
                candidates.append((item,now))
            require(len(candidates)<=1,"multiple owned stopped executable candidates are ambiguous")
            if not candidates:
                time.sleep(.002);continue
            selected,now=candidates[0];original=selected["identity"];pid=now["pid"]
            def guard():
                current=self.frame.common.identity(pid)
                require(self.frame.common.birth(current)==self.frame.common.birth(original)
                        and current["state"] in ("T","t") and not self.frame.common.pidfd_terminal(selected["fd"]),
                        "selected owned stopped birth changed")
                membership=(Path("/proc")/str(pid)/"cgroup").read_text().splitlines()
                require(membership==self.frame.before["membership"] and str(pid) in
                        (Path(self.frame.before["path"])/"cgroup.procs").read_text().split(),"target left original live cgroup")
                host=self.frame.mapping.mapping_file_facts((Path("/proc")/str(pid)/"exe").stat())
                require(host=={key:principal[key] for key in self.frame.mapping._MAP_FACTS},"target/QEMU executable changed")
                return dict(current,membership=membership,live_cgroup_procs=True,host_identity=host)
            receipt=self.frame.mapping.opened_target_mappings(pid,{"target":wanted},guard)
            require(receipt["targets"]["target"],"opened stopped target mapping absent")
            exe=self.frame.common.opened((Path("/proc")/str(pid)/"exe").resolve(strict=True))
            require(exe==principal,"actual /proc/exe differs from exact target/QEMU bytes")
            proof={"birth":original,"stopped_identity":now,"image":image,"actual_exe":exe,
                   "mapped_target":receipt["raw_maps"],"opened_target_mappings":receipt,
                   "membership":Path("/proc/self/cgroup").read_text(),
                   "scope":"Actual Runtime/native architecture separately recorded"}
            row["stopped_mapping"]=proof
            self.frame.save();self.frame.guard();guard()
            signal.pidfd_send_signal(selected["fd"],signal.SIGCONT)
            return proof
        raise TimeoutError("actual pre-main SIGSTOP not observed")

    def text(self, row, stderr=False):
        return Path(row["stderr" if stderr else "stdout"]).read_text()

    def wait(self, child, row, marker, seconds=30):
        until = time.monotonic()+seconds
        while time.monotonic() < until:
            self.check();text = self.text(row)
            require(not any(x.startswith("FAIL") for x in text.splitlines()), "native/Guest failure marker")
            if any(line == marker or line.startswith(marker+" ") for line in text.splitlines()):
                return
            require(child.poll() is None,"actor exited before required marker");time.sleep(.005)
        raise TimeoutError("required marker deadline: "+marker)

    def send(self, child, byte):
        self.check();require(child.poll() is None and len(byte)==1,"one live checked gate required")
        child.stdin.write(byte);child.stdin.flush()

    def finish(self, child, row):
        while child.poll() is None:
            self.check();time.sleep(.005)
        row["returncode"]=child.returncode;row["status"]="PASS" if type(child.returncode) is int and child.returncode==0 else "FAIL"
        for key in ("stdout","stderr"):
            row[key+"_pin"] = self.frame.pin(row[key])
        require(row["status"]=="PASS","actual actor returned nonzero")

    def close(self):
        acquired=[{"first":item["identity"],"after_pidfd":item["identity_after"]} for item in self.owner.owned.values()]
        result = self.owner.close()
        result["acquired_process_births"]=acquired
        for child,row in self.rows:
            row["final_returncode"] = child.poll()
            for field in ("stdout","stderr"):
                row[field+"_pin"] = self.frame.pin(row[field])
            if child.stdin:
                child.stdin.close()
            if child.stdout:
                child.stdout.close()
        return result
