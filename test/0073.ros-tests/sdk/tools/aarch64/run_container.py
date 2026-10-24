#!/usr/bin/env python3
"""@file run_container.py
@brief Hold the shared execution lease for one isolated Docker test batch.
@details Run this host orchestrator on Linux after transferring the complete
owned work folder. It never compiles or executes a guest on the host. Only the
owned work folder is writable. Explicit dependency mounts are read-only.
"""

import argparse
import fcntl
import hashlib
import json
import os
from pathlib import Path
import signal
import subprocess
import time
import uuid


def main():
    """@brief Launch one bounded batch and preserve its original exit and logs.
    @return Zero only when Docker reports that the exact child command passed.
    @details The flock is held until the container has exited and been removed.
    No existing container or unrelated process is stopped. Waiting for the lease
    is bounded by --lease-timeout-seconds; that timeout is a blocked test.
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True)
    parser.add_argument("--image", required=True)
    parser.add_argument("--label", required=True)
    parser.add_argument("--lock", type=Path, required=True)
    parser.add_argument("--read-only", action="append", default=[], metavar="HOST:CONTAINER")
    parser.add_argument("--lease-timeout-seconds", type=int, default=7200)
    parser.add_argument("--network", choices=("none", "bridge"), default="none")
    parser.add_argument("--tmpfs-mib", type=int, default=512)
    parser.add_argument("command", nargs=argparse.REMAINDER)
    args = parser.parse_args()
    if not 512 <= args.tmpfs_mib <= 8192:
        parser.error("Use a tmpfs budget between512 and8192MiB inside the unchanged64GiB cgroup")
    work = args.work.resolve(strict=True)
    if not args.command or args.command[0] != "--":
        parser.error("Separate the exact container command with --")
    if not args.label.replace("-", "").replace("_", "").isalnum():
        parser.error("Use a simple unique receipt label")
    receipt = work / "evidence" / args.label
    receipt.mkdir(parents=True, exist_ok=False)
    args.lock.parent.mkdir(parents=True, exist_ok=True)
    started = time.monotonic()
    report = {"format": "uwvm-acceptance-container-v1", "command": args.command[1:],
              "tmpfs_mib": args.tmpfs_mib, "lease": str(args.lock), "orchestrator_pid":os.getpid(), "status": "waiting-for-lease"}

    def save():
        """@brief Persist the current orchestration state before further effects."""
        pending = receipt / "container.next"
        pending.write_text(json.dumps(report, indent=2) + "\n")
        pending.replace(receipt / "container.json")

    save()
    with args.lock.open("a") as lease:
        def lease_timeout(signum, frame):
            """@brief Bound a kernel-blocking lease wait without polling contenders."""
            raise TimeoutError("The shared execution lease wait expired")

        previous = signal.signal(signal.SIGALRM, lease_timeout)
        signal.alarm(args.lease_timeout_seconds)
        try:
            fcntl.flock(lease, fcntl.LOCK_EX)
        except TimeoutError:
            report.update(status="lease-timeout", exit_code=75)
            save()
            return 75
        except KeyboardInterrupt:
            report.update(status="cancelled-before-admission", exit_code=130)
            save()
            return 130
        finally:
            signal.alarm(0)
            signal.signal(signal.SIGALRM, previous)
        image = json.loads(subprocess.check_output(["docker", "image", "inspect", args.image]))[0]
        name = "uwvm-acceptance-" + uuid.uuid4().hex[:16]
        nonce = uuid.uuid4().hex
        admission = receipt / "admission.json"
        container_admission = "/work/evidence/" + args.label + "/admission.json"
        command = ["docker", "run", "--name", name, "--memory=64g", "--memory-swap=64g",
                   "--network=" + args.network, "--ipc=private", "--pids-limit=1024",
                   "--cap-drop=ALL", "--security-opt=no-new-privileges", "--read-only",
                   "--user", str(os.getuid()) + ":" + str(os.getgid()),
                   "--mount", "type=bind,src=" + str(work) + ",dst=/work",
                   "--tmpfs", "/tmp:rw,nosuid,nodev,exec,size=" + str(args.tmpfs_mib * 1024 * 1024) + ",mode=1777",
                   "--env", "HOME=/work/home", "--env", "PYTHONDONTWRITEBYTECODE=1",
                   "--env", "UWVM_ACCEPTANCE_ADMISSION=" + container_admission,
                   "--env", "UWVM_ACCEPTANCE_NONCE=" + nonce,
                   "--workdir=/work"]
        for item in args.read_only:
            source, target = item.split(":", 1)
            source = str(Path(source).resolve(strict=True))
            installed_native = target == "/native-sysroot/usr" and source == "/usr"
            if not (target.startswith("/deps/") or installed_native) or "," in source + target:
                parser.error("Use /deps/ dependencies or the exact read-only /usr native closure")
            command += ["--mount", "type=bind,src=" + source + ",dst=" + target + ",readonly"]
        command += [image["Id"], *args.command[1:]]
        report.update(status="running", image_id=image["Id"], image_architecture=image["Architecture"],
                      image_entrypoint=image["Config"].get("Entrypoint"),
                      source_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                      admission_nonce=nonce,
                      docker_argv=command, waited_seconds=time.monotonic() - started)
        save()
        try:
            with (receipt / "batch.stdout").open("wb") as out, (receipt / "batch.stderr").open("wb") as err:
                process = subprocess.Popen(command, stdout=out, stderr=err)
                report["docker_client_pid"] = process.pid
                save()
                # Inspect the actual kernel cgroup while this batch owns the lease.
                for _ in range(100):
                    inspect = subprocess.run(["docker", "inspect", name], capture_output=True, text=True)
                    if inspect.returncode == 0:
                        state = json.loads(inspect.stdout)[0]
                        pid = state["State"]["Pid"]
                        if pid:
                            membership = Path("/proc", str(pid), "cgroup").read_text()
                            leaf = next(line[3:] for line in membership.splitlines() if line.startswith("0::"))
                            base = Path("/sys/fs/cgroup") / leaf.lstrip("/")
                            measured = {key: (base / key).read_text().strip()
                                        for key in ("memory.max", "memory.swap.max")}
                            report["actual_cgroup"] = {"membership": membership, **measured}
                            save()
                            if measured != {"memory.max": "68719476736", "memory.swap.max": "0"}:
                                raise RuntimeError("Actual container limits differ from required limits")
                            gate = {"nonce": nonce, "status": "verified", "pid": pid,
                                    "memory_max": measured["memory.max"],
                                    "memory_swap_max": measured["memory.swap.max"],
                                    "membership": membership}
                            temporary = admission.with_suffix(".next")
                            temporary.write_text(json.dumps(gate, indent=2) + "\n")
                            temporary.replace(admission)
                            report["admission_sha256"] = hashlib.sha256(admission.read_bytes()).hexdigest()
                            save()
                            break
                    if process.poll() is not None:
                        break
                    time.sleep(0.05)
                code = process.wait()
                if "actual_cgroup" not in report:
                    raise RuntimeError("The batch ended before its actual cgroup was recorded")
                report.update(status="passed" if code == 0 else "failed", exit_code=code,
                              elapsed_seconds=time.monotonic() - started)
                save()
                return code
        finally:
            # Removal is restricted to the unique container created above.
            removed = subprocess.run(["docker", "rm", "-f", name], capture_output=True, text=True)
            report["cleanup"] = {"exit_code": removed.returncode,
                                 "stdout": removed.stdout, "stderr": removed.stderr}
            save()


if __name__ == "__main__":
    raise SystemExit(main())
