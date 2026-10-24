"""Shared serial Linux lane for remaining acceptance fixtures.

Nothing executes on import. The caller supplies every product/report/SDK path,
the current parent binfmt lease and current container StartedAt. The class
retains exact births through Linux pidfds; it never signals a process group or
changes the original SDK, cgroup, global network or binfmt registration.
"""
from __future__ import annotations

import fcntl
import importlib.util
import json
import os
from pathlib import Path
import shutil
import signal
import stat
import subprocess
import sys
import tempfile
import time

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "ros_aarch64"))
from runtime import Evidence, cgroup_incarnation, digest, require_arm, read_json_binding, elf
from rebuild_affected import sdk_inventory, sdk_root_aliases, source_state, rooted
from gateway_qualification import qualify_gateway, read_file_binding, pins_preserved, architecture_binding
from native_runtime_environment import gateway_library_environment
from run_ros1_search import OwnedProcesses
from run_resources import bind_frozen_work
from run_action_qos import process_identity
from run_all import validate_fresh_worker_inputs
from run_unit import process_tree_rss
from protocol import MEMORY_MAX, require


def atomic_json(path, value):
    """Publish progress atomically inside the new task-owned output directory."""
    path = Path(path); temporary = path.with_name(path.name + ".new")
    with temporary.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2); stream.write("\n"); stream.flush(); os.fsync(stream.fileno())
    os.replace(temporary, path)


def add_arguments(parser, workers=True):
    for name in ("source", "manifest", "sdk-workspace", "out", "qemu", "binfmt-evidence"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--container-started-at", required=True)
    if workers:
        for name in ("worker", "worker-report", "worker-config", "ros1-gateway", "ros1-gateway-report",
                     "ros2-gateway", "ros2-gateway-report"):
            parser.add_argument("--" + name, type=Path, required=True)


def recover_popen_owners(owned, children):
    """Recover only retained direct Popen roots before failure cleanup.

    A child is ledgered before its first fallible procfs/pidfd/evidence call.
    This recovery never signals a numeric PID or assumes an argv match proves
    ownership: it requires a current direct-child relationship, an unchanged
    recorded birth when available, and stable birth/parent around pidfd_open.
    Registration precedes cgroup checking so an owned escape remains cleanup's
    responsibility. An unavailable proof is retained as metadata; the caller
    must report CLEANUP_INCOMPLETE if any retained Popen root remains live.
    """
    recovery = []
    for child, row in children:
        if child.poll() is not None:
            continue
        fd = None
        try:
            first = process_identity(child.pid)
            row["startup_cleanup_first_observation"] = first
            birth = row.get("birth")
            expected = birth.get("starttime_ticks") if isinstance(birth, dict) else row.get("starttime_ticks")
            if expected is not None and first["starttime_ticks"] != expected:
                raise RuntimeError("retained Popen birth differs before cleanup admission")
            key = (child.pid, first["starttime_ticks"])
            if key in owned.rows:
                continue
            if first["parent_pid"] != os.getpid():
                raise RuntimeError("retained live Popen root has no current caller-parent proof")
            fd = os.pidfd_open(child.pid)
            second = process_identity(child.pid)
            if (any(first[field] != second[field] for field in ("pid", "starttime_ticks"))
                    or second["parent_pid"] != os.getpid()):
                raise RuntimeError("retained direct Popen birth/parent changed around cleanup pidfd")
            record = {"identity": first, "pidfd": fd, "label": row["name"], "ancestry": [child.pid],
                      "acquisition": {"first": first, "after_pidfd": second,
                                      "parent_changed": False, "cleanup_recovery": True}}
            owned.rows[key] = record
            fd = None  # Ownership moves once to the shared tracker's cleanup.
            if hasattr(owned, "unproven"):
                owned.unproven.pop(key, None)
            row["startup_cleanup_ownership"] = {"identity": first, "after_pidfd": second,
                                                 "method": "retained direct Popen birth and pidfd"}
            owned.check(record)
        except BaseException as error:
            row["startup_cleanup_admission_error"] = repr(error)
            recovery.append({"pid": child.pid, "name": row["name"], "error": repr(error)})
        finally:
            if fd is not None:
                try:
                    os.close(fd)
                except BaseException as error:
                    row["startup_cleanup_descriptor_error"] = repr(error)
                    recovery.append({"pid": child.pid, "name": row["name"], "descriptor_error": repr(error)})
    return recovery


def retained_popen_cleanup(owned, children):
    """Audit every retained root, including one never admitted by the tracker."""
    recovery = recover_popen_owners(owned, children)
    try:
        result = owned.stop([child for child, _ in children])
    except BaseException as error:
        # A failed tracker cleanup is never evidence that its births retired.
        # Still audit the retained Popen handles below and preserve the failure.
        result = {"status": "CLEANUP_INCOMPLETE", "remaining_owned_processes": [],
                  "owned_cleanup_unverified": True, "errors": [repr(error)]}
    remaining = [{"pid": child.pid, "name": row["name"],
                  "recorded_birth": row.get("birth", row.get("starttime_ticks")),
                  "admission_error": row.get("startup_cleanup_admission_error")}
                 for child, row in children if child.poll() is None]
    result["startup_ownership_recovery"] = recovery
    result["remaining_popen_roots"] = remaining
    if remaining:
        result["status"] = "CLEANUP_INCOMPLETE"
    elif any("descriptor_error" in item for item in recovery):
        result["status"] = "FAIL"
    return result


class Lane:
    """Own immutable copies, private mounts, finite commands and evidence.

    Readiness is never inferred from an executable's pathname. Before a Guest
    input gate, ``stopped`` matches a current /proc mapping device/inode to the
    selected ARM bytes and matches /proc/exe to the pinned x86 QEMU image. A
    shell is stopped before exec solely to retain the SO_PEERCRED Worker PID.
    """
    def __init__(self, args, kind):
        require(sys.platform == "linux" and os.geteuid() == 0, "BLOCKED verified Linux root lane required")
        require(os.readlink("/proc/self/ns/mnt") != os.readlink(f"/proc/{os.getppid()}/ns/mnt"),
                "BLOCKED launch with unshare -m inside the parent lease")
        self.args = args; self.source = args.source.resolve(strict=True)
        self.out = args.out.resolve(); self.out.mkdir(mode=0o700, parents=True, exist_ok=False)
        self.pins = []; self.products = {}; self.children = []; self.mounts = []; self.locks = []
        self.roots = {}; self.sdks = {}; self.sdk_states = {}; self.gateway_bindings = {}; self.deadlines = {}
        self.report = {"format_version": 1, "kind": kind, "mode": "execute", "status": "RUNNING",
                       "source": str(self.source), "container_started_at": args.container_started_at,
                       "commands": [], "roles": [], "cases": [], "new_tests_executed": False}
        self.frozen, manifest = self.json(args.manifest)
        self.report["source_manifest_sha256"] = manifest["sha256"]
        self.initial = source_state(self.source, self.frozen)
        # Bind all local Python imports through the full frozen source view.
        for folder in (self.source/"test/0073.ros-tests/support/source/ros_aarch64", self.source/"test/0073.ros-tests/support/source/ros_acceptance"):
            for path in folder.glob("*.py"):
                name = str(path.relative_to(self.source))
                require(self.frozen.get(name, {}).get("sha256") == digest(path), "helper missing from frozen source")
        spec = importlib.util.spec_from_file_location("remaining_cgroup", self.source/"test/0073.ros-tests/support/source/ros_cgroup/probe.py")
        probe = importlib.util.module_from_spec(spec); spec.loader.exec_module(probe)
        self.evidence = Evidence(self.out/"execution", probe.probe, interval=.001)
        self.evidence.emulator(args.qemu); self.before = cgroup_incarnation(self.evidence.leaf)
        require((self.evidence.leaf/"memory.max").read_text().strip() == str(MEMORY_MAX)
                and (self.evidence.leaf/"memory.swap.max").read_text().strip() == "0", "current64GiB/swap0 required")
        self.owned = OwnedProcesses(self.evidence)
        lease, lease_pin = self.json(args.binfmt_evidence)
        entry = (Path("/proc/sys/fs/binfmt_misc")/lease["name"]).read_text()
        require(entry == lease["entry"] and lease["qemu_sha256"] == digest(args.qemu)
                and lease["name"] == os.environ.get("UWVM_ROS_ARM_BINFMT_ENTRY"), "current parent QEMU lease differs")
        self.report["binfmt"] = {**lease_pin, "entry": entry}; self.pin(args.qemu)
        # An explicit minimal environment replaces inherited ROS/loader values.
        self.clean = {"PATH": "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
                      "HOME": "/root", "LANG": "C.UTF-8", "LC_ALL": "C.UTF-8",
                      "PYTHONDONTWRITEBYTECODE": "1", "UWVM_ROS_ARM_BINFMT_ENTRY": lease["name"]}
        self.checked(["mount", "--make-rprivate", "/"])
        self.short = Path(tempfile.mkdtemp(prefix="uwvm-remaining-", dir="/tmp")); self.short.chmod(0o700)
        self.report["socket_directory"] = str(self.short); self.save()

    def save(self):
        atomic_json(self.out/"report.json", self.report)

    def json(self, path):
        value, pin = read_json_binding(path, limit=64*1024**2); self.pins.append(pin); return value, pin

    def pin(self, path):
        pin = read_file_binding(path); self.pins.append(pin); return pin

    @staticmethod
    def checked(argv):
        subprocess.run(list(map(str, argv)), check=True, capture_output=True, timeout=30)

    def lock(self, name):
        require(name and all(c.isalnum() or c == "-" for c in name), "closed lease label required")
        path = Path("/tmp")/("uwvm-ros-remaining-" + name + ".lock")
        fd = os.open(path, os.O_RDWR|os.O_CREAT|os.O_CLOEXEC|os.O_NOFOLLOW, 0o600)
        state = os.fstat(fd); require(stat.S_ISREG(state.st_mode) and state.st_uid == os.geteuid(), "task lease file invalid")
        fcntl.flock(fd, fcntl.LOCK_EX|fcntl.LOCK_NB)
        self.locks.append((fd, path, state.st_dev, state.st_ino))

    def bind(self, origin, destination, readonly=False):
        destination = Path(destination); destination.mkdir(parents=True, exist_ok=True)
        self.checked(["mount", "--rbind", origin, destination]); self.mounts.append(destination)
        self.checked(["mount", "--make-rprivate", destination])
        if readonly: self.checked(["mount", "-o", "remount,bind,ro", destination])

    def root(self, version):
        if version in self.roots: return self.roots[version]
        sdk = (self.args.sdk_workspace.resolve()/"gateway"/("noetic-rootfs" if version == 1 else "jazzy-rootfs")).resolve(strict=True)
        directory = self.out/("ros"+str(version)); directory.mkdir()
        root = directory/"rootfs"; root.mkdir(); temporary = directory/"tmp"; temporary.mkdir()
        self.sdks[version] = sdk; self.sdk_states[version] = (sdk_inventory(sdk), sdk_root_aliases(sdk))
        inventory_path = self.out/("ros"+str(version)+"-sdk.json")
        atomic_json(inventory_path, {"inventory": self.sdk_states[version][0], "aliases": self.sdk_states[version][1]})
        self.report.setdefault("sdk_before", {})[str(version)] = read_file_binding(inventory_path)
        self.bind(sdk, root)
        self.report.setdefault("source_mounts", {})[str(version)] = bind_frozen_work(root, self.source, sdk, directory, self.checked)
        self.mounts.append(root/"work"); self.bind(temporary, root/"tmp")
        home = directory/"home"; home.mkdir(); self.bind(home, root/"root")
        for relative in ("usr", "opt", "etc", "var/lib/dpkg"): self.bind(sdk/relative, root/relative, True)
        for relative in ("proc", "sys", "dev"): self.bind(Path("/")/relative, root/relative)
        self.bind(self.short, root/"guest")
        self.roots[version] = root; return root

    def native(self, version, argv, witness=False):
        distro = "noetic" if version == 1 else "jazzy"
        script = "source /opt/ros/" + distro + "/setup.bash && "
        if witness and version == 2:
            # Actual Jazzy setup sets SUBNET. Normalize only the test witness;
            # the strict C++ absence predicate stays unchanged.
            script += "unset ROS_AUTOMATIC_DISCOVERY_RANGE && [[ ! -v ROS_AUTOMATIC_DISCOVERY_RANGE ]] && "
        script += 'exec "$@"'
        return ["/usr/bin/chroot", str(self.root(version)), "/bin/bash", "-lc", script,
                "remaining-native", *map(str, argv)]

    def environment(self, version, master_port=11384, domain=205, gateway=False):
        env = dict(self.clean); env["ROS_LOG_DIR"] = "/tmp/ros-logs"
        if version == 1: env.update(ROS_MASTER_URI=f"http://127.0.0.1:{master_port}", ROS_IP="127.0.0.1")
        else: env.update(ROS_DOMAIN_ID=str(domain), RMW_IMPLEMENTATION="rmw_fastrtps_cpp", ROS_SECURITY_ENABLE="false")
        if gateway:
            env.update(self.report["gateway_loader"][str(version)]["environment"])
        return env

    def snapshot(self, path, destination, label):
        origin = require_arm(path); self.pin(path); destination = Path(destination)
        require(not destination.exists(), "owned immutable snapshot already exists")
        shutil.copy2(path, destination); destination.chmod(0o500); copy = require_arm(destination)
        require((origin["sha256"], origin["bytes"]) == (copy["sha256"], copy["bytes"])
                and (origin["device"], origin["inode"]) != (copy["device"], copy["inode"]), "separate exact product copy required")
        self.products[label] = copy; self.evidence.target(label, destination)
        return {"origin": origin, "snapshot": copy}

    def qualify_workers(self):
        report, report_pin = self.json(self.args.worker_report); config, config_pin = self.json(self.args.worker_config)
        require(report.get("status") == "PASS" and report.get("source") == str(self.source)
                and report.get("source_manifest_sha256") == self.report["source_manifest_sha256"]
                and report.get("config_sha256") == config_pin["sha256"], "fresh current Worker/config required")
        binding = validate_fresh_worker_inputs(report, self.source, self.frozen)
        matches = [row for row in report["profiles"] if row["profile"] == "dual"]
        require(len(matches) == 1 and matches[0]["status"] == "PASS" and
                matches[0]["binary"] == require_arm(self.args.worker), "exact qualified dual Worker required")
        for item in binding.values():
            for record in item["raw_compile_dependencies"].values():
                pin = self.pin(record["path"]); require(pin["sha256"] == record["sha256"], "Worker depfile changed")
                self.pin(record["object"]["path"])
        self.pins.extend((report["compiler"]["input"], report["qemu"]["input"]))
        require(config["qemu_argv"][0] == str(self.args.qemu.resolve()), "Worker selected another QEMU")
        self.config = config; self.worker = self.out/"worker"
        self.report["worker_product"] = self.snapshot(self.args.worker, self.worker, "worker")
        self.report["worker_qualification"] = {"report": report_pin, "config": config_pin, "bindings": binding}

    def qualify_gateway(self, version):
        root = self.root(version); sdk = self.sdks[version]
        result = qualify_gateway(getattr(self.args, f"ros{version}_gateway"),
                                 getattr(self.args, f"ros{version}_gateway_report"), self.args.manifest,
                                 self.source, sdk, version)
        self.pins.extend(result["input_pins"]); self.gateway_bindings[version] = result
        loader = gateway_library_environment(result, sdk, self.sdk_states[version][0]); self.pins.extend(loader["input_pins"])
        self.report.setdefault("gateway_qualification", {})[str(version)] = result
        self.report.setdefault("gateway_loader", {})[str(version)] = loader
        self.report.setdefault("gateway_products", {})[str(version)] = self.snapshot(
            getattr(self.args, f"ros{version}_gateway"), self.out/("ros"+str(version))/"tmp/gateway", "gateway-"+str(version))

    def check(self):
        self.owned.sample()
        require(cgroup_incarnation(self.evidence.leaf) == self.before
                and (self.evidence.leaf/"memory.max").read_text().strip() == str(MEMORY_MAX)
                and (self.evidence.leaf/"memory.swap.max").read_text().strip() == "0", "cgroup birth/budget changed")
        require(int((self.evidence.leaf/"memory.current").read_text()) < 60*1024**3, "64GiB reserve exhausted")
        now = time.monotonic()
        for child, row in self.children:
            if child.poll() is None:
                require(now < self.deadlines[child.pid], "bounded actor deadline exhausted")
                rss = max(process_tree_rss(child.pid), self.owned.rss(row["name"]))
                row["peak_observed_RSS"] = max(row.get("peak_observed_RSS", 0), rss)
                require(rss <= row["rss_limit"] and Path(row["stdout"]).stat().st_size <= 16*1024**2
                        and Path(row["stderr"]).stat().st_size <= 16*1024**2, "actor RSS/log bound exhausted")

    def start(self, name, argv, env=None, stdin=subprocess.PIPE, seconds=120, rss=2*1024**3,
              stdout=None, pass_fds=()):
        require(name not in {row["name"] for _, row in self.children}, "unique actor label required")
        out, err = self.out/(name+".stdout"), self.out/(name+".stderr")
        # Binary relay stdout is a private pipe. Audits always use another fd.
        with out.open("xb") as output, err.open("xb") as error:
            child = subprocess.Popen(list(map(str, argv)), stdin=stdin, stdout=output if stdout is None else stdout,
                                     stderr=error, env=self.clean if env is None else env,
                                     pass_fds=pass_fds, start_new_session=True)
        row = {"name": name, "argv": list(map(str, argv)), "pid": child.pid,
               "birth": None, "status": "RUNNING", "stdout": str(out), "stderr": str(err),
               "rss_limit": rss, "deadline_seconds": seconds, "startup_status": "POPEN_RETAINED"}
        # Keep the trusted Popen handle before any fallible identity, pidfd or
        # evidence acquisition, so failure cleanup cannot omit a live root.
        self.children.append((child, row)); self.report["commands"].append(row)
        self.save()
        row["birth"] = process_identity(child.pid)
        self.owned.acquire(child.pid, name, [child.pid]); row["startup_status"] = "OWNED_BIRTH_RECORDED"
        self.evidence.start(child, name+"/", list(map(str, argv))); row["startup_status"] = "EVIDENCE_REGISTERED"
        self.deadlines[child.pid] = time.monotonic()+seconds; self.save(); return child, row

    def read(self, row, stderr=False):
        path = Path(row["stderr" if stderr else "stdout"])
        require(path.stat().st_size <= 16*1024**2, "bounded actor log required")
        return path.read_bytes().decode("utf-8", errors="strict")

    def wait_marker(self, child, row, marker, seconds=30):
        until = time.monotonic()+seconds
        while time.monotonic() < until:
            self.check(); text = self.read(row)
            require("FAIL_" not in text and "FAIL_" not in self.read(row, True), "actor emitted fixed refusal")
            lines = text.splitlines()
            if any(line == marker or line.startswith(marker+" ") for line in lines): return text
            require(child.poll() is None, "actor retired before required marker")
            time.sleep(.005)
        raise TimeoutError("bounded marker deadline: "+marker)

    def finish(self, child, row, expected=0):
        while child.poll() is None: self.check(); time.sleep(.005)
        row.update(returncode=child.returncode, stdout_pin=read_file_binding(row["stdout"]),
                   stderr_pin=read_file_binding(row["stderr"]))
        require(child.returncode == expected, "actor return code differs")
        row["status"] = "PASS"; self.save()

    def send(self, child, row, byte, prior):
        require(type(byte) is bytes and len(byte) == 1 and child.poll() is None, "single live actor gate required")
        self.check(); child.stdin.write(byte); child.stdin.flush()
        self.report.setdefault("gates", []).append({"role": row["name"], "pid": child.pid,
            "starttime_ticks": row["birth"]["starttime_ticks"], "byte": byte.decode("ascii"), "after": prior})
        self.save()

    def stopped(self, child, row, label, seconds=20, expected_pid=None, cooperative=False):
        """Prove the current ARM image before resuming one stopped birth.

        A probe with its own pre-initialization SIGSTOP barrier uses cooperative
        mode. Wait until that exact mapped image is already stopped; sending an
        earlier STOP could otherwise pause it before its own barrier and cause
        a second, unobserved stop after our resume. Ordinary Gateway/Worker,
        Master and retained running peers keep the explicit-stop default.
        """
        require(type(cooperative) is bool,"explicit cooperative barrier boolean required")
        target = self.evidence.targets[label]; until = time.monotonic()+seconds
        def mapped(pid):
            return self.evidence.mapped(pid, target)
        while child.poll() is None and time.monotonic() < until:
            self.check()
            for owned in list(self.owned.rows.values()):
                if owned["label"] != row["name"] or not self.owned.alive(owned) or not self.owned.check(owned): continue
                pid = owned["identity"]["pid"]
                if expected_pid is not None and pid != expected_pid: continue
                try:
                    if not mapped(pid): continue
                    if cooperative:
                        if process_identity(pid)["state"] not in ("T", "t"):continue
                    else:
                        self.owned.send(owned, signal.SIGSTOP); deadline = time.monotonic()+3
                        while self.owned.alive(owned) and process_identity(pid)["state"] not in ("T", "t") and time.monotonic() < deadline: time.sleep(.001)
                    actual = process_identity(pid); require(actual["state"] in ("T", "t"), "actual ARM did not stop")
                    self.evidence.observe(child.pid, row["name"]+"/")
                    with self.evidence._lock:
                        matches = [item for item in self.evidence.samples.values() if item['opened_target_mappings']['identity_after']['state'] in ('T','t') and item["name"] == row["name"]+"/"
                            and item["pid"] == pid and item["starttime_ticks"] == actual["starttime_ticks"]
                            and item["ancestry"] == owned["ancestry"] and item["live_cgroup_procs"]
                            and item["membership"] == self.evidence.before["membership"] and label in item["mapped_arm_targets"]
                            and item["kernel_exe_elf"]["e_machine"] == 62 and item["kernel_exe_elf"]["sha256"] in self.evidence.emulator_shas]
                    require(matches and mapped(pid) and self.owned.check(owned) and process_identity(pid) == actual,
                            "current stopped ARM/birth/map proof differs")
                    sample = matches[-1]; proof = {"actual_stopped_ARM": actual, "target_proof": sample,
                        "current_mapping_check": True, "ancestry": owned["ancestry"], "live_cgroup_membership": self.evidence.before["membership"],
                        "cooperative_barrier":cooperative}
                    self.report["roles"].append({"role": row["name"], "label": label, "pid": pid,
                        "starttime_ticks": actual["starttime_ticks"], "proof": proof})
                    require(self.owned.check(owned), "owned stopped membership changed before CONT")
                    self.evidence.before_resume(pid, actual)
                    self.owned.send(owned, signal.SIGCONT); self.save(); return proof
                except (FileNotFoundError, ProcessLookupError): continue
            time.sleep(.002)
        raise RuntimeError("stopped actual ARM mapping unavailable: "+label)

    def stop_role(self, child, row, signum=signal.SIGTERM):
        """Retire only this actor's proven descendants, retaining all other roles."""
        self.check(); selected = [item for item in self.owned.rows.values() if item["label"] == row["name"]]
        require(selected, "owned role birth missing")
        for item in sorted(selected, key=lambda item: len(item["ancestry"]), reverse=True):
            self.owned.send(item, signal.SIGCONT); self.owned.send(item, signum)
        deadline = time.monotonic()+5
        while any(self.owned.alive(item) for item in selected) and time.monotonic() < deadline:
            self.check(); time.sleep(.005)
        for item in selected:
            if self.owned.alive(item): self.owned.send(item, signal.SIGKILL)
        child.wait(timeout=5); require(not any(self.owned.alive(item) for item in selected), "owned fault role did not retire")
        row.update(status="OWNED_RETIRED", returncode=child.returncode,
                   stdout_pin=read_file_binding(row["stdout"]), stderr_pin=read_file_binding(row["stderr"]))
        self.save()

    def stopped_host(self, child, row, executable, seconds=20):
        """Bind an actual stopped host interpreter without claiming ARM code.

        Only private proxy/attack/Node actors use this path. It proves the
        selected kernel executable, retained birth and current cgroup leaf;
        its receipt stays distinct from the ARM/QEMU/native ROS role proof.
        """
        selected=elf(Path(executable).resolve(strict=True)); self.pin(selected["path"])
        require(selected["e_machine"]==62, "explicit x86 host executable required")
        until=time.monotonic()+seconds
        while child.poll() is None and time.monotonic()<until:
            self.check(); current=process_identity(child.pid)
            if current["state"] not in ("T","t"): time.sleep(.002); continue
            owner=next(item for item in self.owned.rows.values() if item["identity"]["pid"]==child.pid)
            require(self.owned.check(owner) and current["starttime_ticks"]==row["birth"]["starttime_ticks"], "host actor birth/leaf differs")
            actual=elf(f"/proc/{child.pid}/exe")
            require(all(actual[key]==selected[key] for key in ("device","inode","bytes","sha256","e_machine")), "host executable mapping differs")
            proof={"pid":child.pid,"starttime_ticks":current["starttime_ticks"],"membership":self.evidence.before["membership"],
                   "kernel_executable":selected,"actual_stopped_host":True,"ARM_execution_claimed":False}
            self.report.setdefault("host_roles",[]).append({"role":row["name"],"proof":proof})
            self.owned.send(owner,signal.SIGCONT); self.save();return proof
        raise RuntimeError("stopped actual host birth/executable unavailable")

    def close(self, status):
        """Run all cleanup even after an assertion and keep failure evidence."""
        errors = []
        try:
            cleanup = retained_popen_cleanup(self.owned, self.children); self.report["process_cleanup"] = cleanup
            if cleanup["status"] != "PASS": status = "FAIL"
            for child, row in self.children:
                row["final_returncode"] = child.poll()
                row["stdout_pin"] = read_file_binding(row["stdout"]); row["stderr_pin"] = read_file_binding(row["stderr"])
        except BaseException as error: errors.append(repr(error)); status = "FAIL"
        try:
            self.report["source_preserved"] = source_state(self.source, self.frozen) == self.initial
            self.report["sdk_preserved"] = all((sdk_inventory(self.sdks[v]), sdk_root_aliases(self.sdks[v])) == state for v, state in self.sdk_states.items())
            self.report["inputs_preserved"] = pins_preserved(self.pins)
            self.report["products_preserved"] = all(require_arm(item["path"]) == item for item in self.products.values())
            if not all(self.report[name] for name in ("source_preserved", "sdk_preserved", "inputs_preserved", "products_preserved")): status = "FAIL"
            actual = self.evidence.finish(status, {"scope": self.report["kind"], "container_started_at": self.args.container_started_at})
            status = actual["status"]; self.report["architecture_evidence"] = read_file_binding(self.out/"execution/architecture-evidence.json")
        except BaseException as error: errors.append(repr(error)); status = "FAIL"
        for mount in reversed(self.mounts):
            try:
                result = subprocess.run(["umount", "-R", str(mount)], capture_output=True, timeout=30)
                self.report.setdefault("mount_cleanup", []).append({"path": str(mount), "returncode": result.returncode})
                if result.returncode: status = "FAIL"
            except BaseException as error: errors.append(repr(error)); status = "FAIL"
        self.report["remaining_owned_mounts"] = [line for line in Path("/proc/self/mountinfo").read_text().splitlines() if str(self.out) in line]
        if self.report["remaining_owned_mounts"]: status = "FAIL"
        for fd, path, device, inode in self.locks:
            try:
                value = path.lstat(); require((value.st_dev, value.st_ino) == (device, inode), "lease file replaced")
                path.unlink(); os.close(fd)
            except BaseException as error: errors.append(repr(error)); status = "FAIL"
        try:
            # Retain configs as evidence, then remove only the owned short dir.
            archive = self.out/"socket-configs"; archive.mkdir()
            for path in self.short.iterdir():
                if path.is_file() and not path.is_symlink():
                    before=read_file_binding(path);shutil.copy2(path, archive/path.name);copy=read_file_binding(archive/path.name)
                    require((before["bytes"],before["sha256"])==(copy["bytes"],copy["sha256"]),"short owned evidence copy changed")
                    self.report.setdefault("socket_archives",{})[path.name]={"origin_at_cleanup":before,"snapshot":copy}
                path.unlink()
            self.short.rmdir()
        except BaseException as error: errors.append(repr(error)); status = "FAIL"
        self.report["input_pins"] = self.pins; self.report["cleanup_errors"] = errors
        self.report["status"] = status; self.save(); return status
