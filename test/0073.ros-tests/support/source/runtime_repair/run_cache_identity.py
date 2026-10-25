#!/usr/bin/env python3
"""@brief Qualify signed-cache identity and inherited descriptor admission.

@details Execute the actual Linux product in fresh processes. Ordinary output
files, paths and timestamps must not invalidate a compatible object. Changed
instructions and executable bytes must invalidate it; changed data must be
initialized from the current guest. A corrupted object must be rejected before
use. The harmless procfs input probe verifies that allowing
ordinary files did not allow pseudo-files. This is a bounded regression suite,
not a claim that every host configuration has been qualified.
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time


def main():
    """Preserve every command, object digest, rejection and process result."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--uwvm", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    assert os.uname().sysname == "Linux"
    args.out.mkdir(parents=True, exist_ok=False)
    args.uwvm = args.uwvm.resolve(strict=True)
    report = {"passed": False, "commands": [], "runtime_sha256": hashlib.sha256(args.uwvm.read_bytes()).hexdigest()}
    specification = importlib.util.spec_from_file_location("basic_test", Path(__file__).resolve().parents[3] / "basic_test.py")
    basic = importlib.util.module_from_spec(specification)
    specification.loader.exec_module(basic)
    guest, _, message = basic.fixtures()
    wasm = args.out / "identity.wasm"
    wasm.write_bytes(guest)
    cache = args.out / "cache"
    flags = ["-Raot", "-Rct", "0", "-Rllvm-policy", "max", "--log-verbose", "--runtime-compiler-log", "err",
             "--runtime-llvm-jit-cache-seal-process", "-Rllvm-cache-path", "path", str(cache),
             "--wasip1-global-noinherit-system-environment",
             "--wasm-set-main-module-name", "cache-identity-fixture"]

    def snapshot():
        """Hash only this suite's cache files, never unrelated user directories."""
        return {str(p.relative_to(cache)): {"sha256": hashlib.sha256(p.read_bytes()).hexdigest(), "mtime_ns": p.stat().st_mtime_ns}
                for p in cache.rglob("*") if p.is_file()}

    def run(label, *, runtime=None, source=None, files=False, input_path=None, revoked=False,
            argument=None, expected=None, hit=False, store=False):
        command = [str(runtime or args.uwvm), *flags]
        if not revoked:
            command += ["--wasip1-global-grant", "monotonic-clock"]
        selected = argument if argument is not None else (2 if revoked else 0)
        command += ["-Wstart", "2", str(selected), "--run", str(source or wasm)]
        stdout = args.out / (label + ".stdout")
        stderr = args.out / (label + ".stderr")
        started = time.monotonic()
        with open(input_path or os.devnull, "rb") as input_stream:
            if files:
                with stdout.open("wb") as output_stream, stderr.open("wb") as error_stream:
                    process = subprocess.run(command, stdin=input_stream, stdout=output_stream, stderr=error_stream, timeout=60)
            else:
                process = subprocess.run(command, stdin=input_stream, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=60)
                stdout.write_bytes(process.stdout)
                stderr.write_bytes(process.stderr)
        text = basic.ANSI.sub(b"", stderr.read_bytes())
        row = {"name": label, "argv": command, "returncode": process.returncode,
               "seconds": time.monotonic() - started, "regular_output_files": files,
               "stdout_sha256": hashlib.sha256(stdout.read_bytes()).hexdigest(),
               "stderr_sha256": hashlib.sha256(stderr.read_bytes()).hexdigest(),
               "cache_hits": len(re.findall(rb"object-cache-hit [^\n]*signature_verified=1\b", text))}
        report["commands"].append(row)
        assert process.returncode == 0 and stdout.read_bytes() == (message if expected is None else expected), row
        if hit:
            assert row["cache_hits"] > 0 and b"object-cache-store-enqueue " not in text, label
        if store:
            assert not row["cache_hits"] and re.search(rb"object-cache-store-complete [^\n]*status=ok\b", text), label
        return text

    try:
        run("cold", store=True)
        original = snapshot()
        assert len(original) == 1, original
        run("pipe-hit", hit=True)
        run("regular-file-hit", files=True, hit=True)
        run("revoked-grant-hit", files=True, revoked=True, hit=True)
        relocated = args.out / "relocated.wasm"
        relocated.write_bytes(guest)
        os.utime(relocated, (1000000000, 1000000000))
        run("relocated-source-hit", source=relocated, files=True, hit=True)
        identical = args.out / "identical-runtime"
        shutil.copyfile(args.uwvm, identical)
        identical.chmod(0o500)
        run("identical-executable-hit", runtime=identical, files=True, hit=True)
        assert snapshot() == original, "unchanged executions rewrote their cache"

        for path in cache.rglob("*"):
            path.chmod(0o500 if path.is_dir() else 0o400)
        cache.chmod(0o500)
        run("readonly-directory-hit", files=True, hit=True)
        assert snapshot() == original
        cache.chmod(0o700)
        for path in cache.rglob("*"):
            path.chmod(0o700 if path.is_dir() else 0o600)

        before = snapshot()
        text = run("pseudo-file-refused", input_path="/proc/version", files=True)
        assert b"reason=pseudo-or-unreviewed-filesystem standard-descriptor=0" in text
        assert b"object-cache-hit " not in text and snapshot() == before

        changed = guest.replace(message, message.replace(b"BASIC", b"ALTER", 1))
        assert changed != guest and len(changed) == len(guest)
        wasm.write_bytes(changed)
        # Initial memory is instantiated from the current validated module.
        # Data-only changes may reuse code when they do not change emitted IR.
        run("changed-data-fresh", files=True, expected=message.replace(b"BASIC", b"ALTER", 1))
        run("changed-data-hit", files=True, expected=message.replace(b"BASIC", b"ALTER", 1), hit=True)
        wasm.write_bytes(guest)
        run("restored-data-hit", files=True, hit=True)
        old_trap = b"\x20\x00\x41\x01\x46\x04\x40\x00\x0b"
        new_trap = b"\x20\x00\x41\x03\x46\x04\x40\x00\x0b"
        assert guest.count(old_trap) == 1
        wasm.write_bytes(guest.replace(old_trap, new_trap))
        # Argument one traps in the old object and returns in the changed one.
        run("changed-code-miss", files=True, argument=1, store=True)
        run("changed-code-hit", files=True, argument=1, hit=True)
        wasm.write_bytes(guest)
        run("restored-code-hit", files=True, hit=True)

        altered_runtime = args.out / "altered-runtime"
        # An appended, nonloaded byte changes the complete ELF identity without
        # modifying any executable instruction or manufacturing an unsafe guest.
        with args.uwvm.open("rb") as src, altered_runtime.open("wb") as dst:
            shutil.copyfileobj(src, dst)
            dst.write(b"\0")
        altered_runtime.chmod(0o500)
        report["altered_runtime_sha256"] = hashlib.sha256(altered_runtime.read_bytes()).hexdigest()
        assert report["altered_runtime_sha256"] != report["runtime_sha256"]
        run("changed-executable-miss", runtime=altered_runtime, files=True, store=True)
        run("changed-executable-hit", runtime=altered_runtime, files=True, hit=True)
        run("original-executable-hit", files=True, hit=True)

        cache_file = cache / next(iter(original))
        authenticated = cache_file.read_bytes()
        damaged = bytearray(authenticated)
        damaged[-1] ^= 1
        cache_file.write_bytes(damaged)
        text = run("damaged-object-refused", files=True, store=True)
        assert b"status=signature-mismatch" in text, "a corrupted native object was not rejected by signature verification"
        run("recompiled-object-hit", files=True, hit=True)
        report["final_cache"] = snapshot()
        report["passed"] = True
    except BaseException as error:
        report["failure"] = repr(error)
        raise
    finally:
        (args.out / "result.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
