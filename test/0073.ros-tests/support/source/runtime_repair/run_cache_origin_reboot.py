#!/usr/bin/env python3
"""@brief Verify authenticated cache reuse across two actual Linux guest boots.

@details Run as root in an isolated disposable guest. Only /state is retained
between boots. The source VM and helper are identical in both initramfs images.
The VM runs as a dedicated unprivileged UID. No secret bytes enter the receipt.
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import resource
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--uwvm", type=Path, required=True)
    parser.add_argument("--helper", type=Path, required=True)
    parser.add_argument("--basic-test", type=Path, required=True)
    args = parser.parse_args()
    assert os.getuid() == 0 and os.geteuid() == 0
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    uid = gid = 41874
    state = Path("/state")
    user = state / "user"
    user.mkdir(mode=0o700, exist_ok=True)
    os.chown(user, uid, gid)
    secret = Path(f"/var/lib/uwvm2-ros/cache-origin/uid-{uid}.secret")
    receipt = {"passed": False, "boot_id": Path("/proc/sys/kernel/random/boot_id").read_text().strip(),
               "runtime_sha256": hashlib.sha256(args.uwvm.read_bytes()).hexdigest(),
               "helper_sha256": hashlib.sha256(args.helper.read_bytes()).hexdigest(), "checks": []}
    prior_path = state / "first-boot.json"
    first_boot = not prior_path.exists()

    def child_credentials():
        os.setgroups([])
        os.setgid(gid)
        os.setuid(uid)

    def helper(label, accepted=True, unprivileged=False):
        result = subprocess.run([str(args.helper), "--uid", str(uid)], stdin=subprocess.DEVNULL,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=20,
                                preexec_fn=child_credentials if unprivileged else None)
        assert (result.returncode == 0) == accepted, (label, result.returncode, result.stderr.decode())
        receipt["checks"].append({"name": label, "returncode": result.returncode,
                                  "diagnostic": result.stderr.decode()})

    def cache_manifest():
        return {str(p.relative_to(user / "cache")): hashlib.sha256(p.read_bytes()).hexdigest()
                for p in sorted((user / "cache").rglob("*")) if p.is_file()}

    def execute(label, expect_hit):
        command = [str(args.uwvm), "-Raot", "-Rct", "0", "-Rllvm-policy", "max", "--log-verbose",
                   "--runtime-compiler-log", "err", "--wasip1-global-noinherit-system-environment",
                   "--runtime-llvm-jit-cache-seal-process", "-Rllvm-cache-path", "path", str(user / "cache"),
                   "--wasip1-global-grant", "monotonic-clock", "-Wstart", "2", "0", "--run", str(user / "basic.wasm")]
        result = subprocess.run(command, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, timeout=60, preexec_fn=child_credentials)
        text = re.sub(rb"\x1b\[[0-9;]*m", b"", result.stderr)
        assert result.returncode == 0 and result.stdout == b"BASIC_WASI_ROS_OK\n", (label, result.returncode, text.decode())
        assert b"call_stack=unwind " in text and b"unwind_check=live" in text
        if expect_hit:
            assert re.search(rb"object-cache-hit [^\n]*signature_verified=1\b", text), text.decode()
            assert b"object-cache-store-enqueue " not in text, text.decode()
        else:
            assert re.search(rb"object-cache-store-complete [^\n]*status=ok\b", text), text.decode()
        (state / (label + ".stderr")).write_bytes(result.stderr)
        receipt["checks"].append({"name": label, "returncode": 0, "signed_hit": expect_hit,
                                  "stderr_sha256": hashlib.sha256(result.stderr).hexdigest()})

    try:
        helper("restore-before-vm")
        helper("idempotent-restoration")
        assert secret.stat().st_uid == 0 and secret.stat().st_gid == 0
        assert secret.stat().st_mode & 0o7777 == 0o600
        if first_boot:
            specification = importlib.util.spec_from_file_location("product_probe", args.basic_test)
            module = importlib.util.module_from_spec(specification)
            specification.loader.exec_module(module)
            guest, _, _ = module.fixtures()
            (user / "basic.wasm").write_bytes(guest)
            os.chown(user / "basic.wasm", uid, gid)
            execute("first-cold", False)
            execute("first-warm", True)
            receipt["cache"] = cache_manifest()
            assert receipt["cache"]
            receipt["wasm_sha256"] = hashlib.sha256(guest).hexdigest()
        else:
            previous = json.loads(prior_path.read_text())
            assert previous["passed"] and previous["boot_id"] != receipt["boot_id"]
            assert previous["runtime_sha256"] == receipt["runtime_sha256"]
            assert previous["helper_sha256"] == receipt["helper_sha256"]
            assert previous["wasm_sha256"] == hashlib.sha256((user / "basic.wasm").read_bytes()).hexdigest()
            assert cache_manifest() == previous["cache"]
            execute("second-boot-hit", True)
            assert cache_manifest() == previous["cache"]
            helper("unprivileged-helper-refused", False, True)
            os.chmod(secret, 0o640)
            try:
                helper("group-readable-secret-refused", False)
            finally:
                os.chmod(secret, 0o600)
            saved = secret.with_suffix(".saved")
            secret.rename(saved)
            try:
                secret.symlink_to(saved)
                helper("symlink-secret-refused", False)
            finally:
                secret.unlink()
                saved.rename(secret)
            other_link = secret.with_suffix(".link")
            os.link(secret, other_link)
            try:
                helper("hard-linked-secret-refused", False)
            finally:
                other_link.unlink()
            secret.rename(saved)
            try:
                secret.write_bytes(b"malformed")
                os.chmod(secret, 0o600)
                helper("truncated-secret-refused", False)
                secret.write_bytes(os.getrandom(32))
                helper("different-kernel-origin-refused", False)
            finally:
                secret.unlink()
                saved.rename(secret)
            os.chmod(secret.parent, 0o777)
            try:
                helper("writable-store-refused", False)
            finally:
                os.chmod(secret.parent, 0o700)
            inaccessible = subprocess.run(["/bin/busybox", "cat", str(secret)], stdin=subprocess.DEVNULL,
                                          stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                          preexec_fn=child_credentials, timeout=10)
            assert inaccessible.returncode != 0 and not inaccessible.stdout
            receipt["checks"].append({"name": "guest-account-cannot-read-root-store", "returncode": inaccessible.returncode})
            helper("original-key-still-live")
            execute("after-refusals-hit", True)
            assert cache_manifest() == previous["cache"]
        receipt["passed"] = True
    finally:
        path = state / ("first-boot.json" if first_boot else "second-boot.json")
        path.write_text(json.dumps(receipt, indent=2) + "\n")
        print("REBOOT_CACHE_RECEIPT=" + json.dumps(receipt, sort_keys=True), flush=True)


if __name__ == "__main__":
    main()
