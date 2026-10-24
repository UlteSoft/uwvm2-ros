#!/usr/bin/env python3
"""@file accept.py
@brief Execute manifest-selected Wasm cases with genuine native ROS witnesses.
@details The manifest supplies exact user-authorized commands and permissions.
This runner creates no grants, substitutes no language and interprets no guest
source. It records ARM runtime execution independently of ARM source production.
The same runner accepts machine=62 for a separate x86_64 acceptance cohort.
"""

import argparse
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import time

from identity import cgroup, elf, mapped_files, require_native_elf, sha256, wasm_memories


def events(path):
    """@brief Read durable JSONL native observations, ignoring only an unfinished tail.
    @param path Owned native peer log, not a guest-created success receipt.
    @return Complete observations in original order.
    @exception ValueError An already completed observation is malformed.
    """
    if not path.exists():
        return []
    data = path.read_text()
    return [json.loads(line) for line in data.splitlines(keepends=True)
            if line.endswith("\n") and line.strip()]


def matches(value, expected):
    """@brief Match an explicitly selected subset without discarding full evidence."""
    return all(key in value and value[key] == item for key, item in expected.items())


def admit_preloads(entry, command, expand):
    """@brief Freeze every declared preload's bytes, memory and explicit module name.
    @param entry Case metadata with optional ordered preload_modules and main_module_name.
    @param command Exact manually supplied runtime argv, including --run.
    @param expand Manifest placeholder expansion supplied by the caller.
    @return Complete identity observations; no import binding or permission is inferred.
    @exception ValueError A preload is unrecorded, altered, misnamed or has wrong metadata.
    @details The actual CLI takes a plain optional rename argument, without literal
    parentheses. This runner requires explicit names for reproducible Single policy
    binding. Duplicate names remain executable negative-test inputs; their rejection
    must come from the real VM. Different declared library widths are likewise kept
    for real mismatch-refusal cases, without claiming their interoperation works.
    """
    declarations = entry.get("preload_modules", [])
    runtime_argv = command[:command.index("--run")] if "--run" in command else command
    if "-Wpre" in runtime_argv or "-Wname" in runtime_argv:
        raise ValueError("Use full explicit preload/main-name CLI options in acceptance")
    indices = [index for index, value in enumerate(runtime_argv) if value == "--wasm-preload-library"]
    if not declarations:
        if indices:
            raise ValueError("Every actual preload requires an immutable module declaration")
        return []
    if "--run" not in command or not isinstance(declarations, list):
        raise ValueError("Preload cases require explicit ordered declarations and --run argv")
    main_name = entry.get("main_module_name")
    name_indices = [index for index, value in enumerate(runtime_argv) if value == "--wasm-set-main-module-name"]
    if not isinstance(main_name, str) or not main_name or len(name_indices) != 1:
        raise ValueError("Preload cases require one explicit main module identity")
    index = name_indices[0]
    if index + 1 >= len(runtime_argv) or runtime_argv[index + 1] != main_name:
        raise ValueError("The main module identity differs from the manual CLI")
    if len(indices) != len(declarations):
        raise ValueError("The actual and declared preload counts differ")
    result = []
    for index, item in zip(indices, declarations):
        path = Path(expand(item["wasm"])).resolve(strict=True)
        name = item["name"]
        if not isinstance(name, str) or not name or "\x00" in name:
            raise ValueError("A preload needs an exact nonempty module identity")
        if runtime_argv[index + 1:index + 3] != [str(path), name]:
            raise ValueError("Preload order/path/plain rename differs from the manual CLI")
        if sha256(path) != item["sha256"]:
            raise ValueError("Frozen preload Wasm changed: " + name)
        memories = wasm_memories(path, require_memory=False)
        bits = item["memory_bits"]
        if (memories and (bits not in (32, 64) or any(memory["bits"] != bits for memory in memories))) or (
                not memories and bits is not None):
            raise ValueError("The declared preload memory width differs from actual bytes")
        result.append({"name":name,"path":str(path),"sha256":item["sha256"],
                       "declared_memory_bits":bits,"memories":memories})
    return result


def stop(process):
    """@brief Reap only an owned child's process group and report cleanup.
    @param process Popen instance created by this runner with start_new_session.
    @return Exit status and whether forced termination was necessary.
    """
    forced = False
    if process.poll() is None:
        try:
            os.killpg(process.pid, signal.SIGTERM)
            process.wait(timeout=5)
        except ProcessLookupError:
            process.wait()
        except subprocess.TimeoutExpired:
            forced = True
            os.killpg(process.pid, signal.SIGKILL)
            process.wait(timeout=5)
    return {"exit_code": process.returncode, "forced": forced, "reaped": process.poll() is not None}


def main():
    """@brief Admit exact native assets, start peers, run cases and seal receipts.
    @return Zero only when all cases, native witnesses and cleanup checks pass.
    @exception OSError Input, process or log creation failed; raw logs survive.
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    manifest = json.loads(args.manifest.read_text())
    if manifest["format"] != "uwvm-native-acceptance-v1":
        parser.error("Unknown acceptance manifest format")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    report = {"format": "uwvm-native-acceptance-receipt-v1", "status": "admitting",
              "manifest_sha256": sha256(args.manifest), "cgroup": cgroup(),
              "target_machine": manifest["target_machine"], "assets": {}, "peers": [], "cases": [],
              "arm_compiler_host_qualified": False, "physical_hardware_qualified": False}
    assets, children, streams = {}, [], []

    def save():
        """@brief Persist the complete accumulated receipt before each next effect."""
        (output / "acceptance.json").write_text(json.dumps(report, indent=2) + "\n")

    def expand(text):
        """@brief Expand only declared asset and output placeholders in exact arguments."""
        text = text.replace("${output}", str(output))
        for name, path in assets.items():
            text = text.replace("${asset:" + name + "}", str(path))
        if "${" in text:
            raise ValueError("Unresolved manifest placeholder: " + text)
        return text

    def environment(overrides):
        """@brief Apply explicit manifest environment without inventing ROS permissions."""
        result = os.environ.copy()
        result.pop("LD_PRELOAD", None)
        result.update({key: expand(value) for key, value in overrides.items()})
        return result

    def fixture_files(files):
        """@brief Prepare explicitly requested peer mode files below this result root."""
        for name, contents in files.items():
            path = Path(expand(name)).resolve()
            if not path.is_relative_to(output):
                raise ValueError("Fixture data must stay below the owned result directory")
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(contents)

    save()
    try:
        roles = set()
        for name, entry in manifest["assets"].items():
            path = Path(entry["path"])
            if not path.is_absolute():
                path = args.manifest.resolve().parent / path
            path = path.resolve(strict=True)
            digest = sha256(path)
            if digest != entry["sha256"]:
                raise ValueError("Asset hash mismatch: " + name)
            metadata = elf(path)
            if "machine" in entry and (metadata is None or metadata["machine"] != entry["machine"] or metadata["bits"] != 64):
                raise ValueError("Asset ELF machine/class mismatch: " + name)
            role = entry["role"]
            if role in ("runtime", "gateway", "native_peer", "native_interpreter"):
                metadata = require_native_elf(path, manifest["target_machine"])
            assets[name] = path
            roles.add(role)
            report["assets"][name] = {**entry, "path": str(path), "elf": metadata}
        if not {"runtime", "gateway"} <= roles:
            raise ValueError("A runtime and gateway ELF must be independently admitted")
        if manifest["target_machine"] == 183 and "qemu" not in roles:
            raise ValueError("Explicit AArch64 QEMU executable required")
        for name, entry in manifest["assets"].items():
            if entry["role"] == "qemu":
                version = subprocess.run([str(assets[name]), "--version"], capture_output=True, text=True, timeout=10)
                if version.returncode:
                    raise RuntimeError("QEMU version command failed")
                report["assets"][name]["version_stdout"] = version.stdout
                report["assets"][name]["version_stderr"] = version.stderr
        fixture_files(manifest.get("initial_files", {}))
        for entry in manifest["peers"]:
            name = entry["id"]
            if not re.fullmatch(r"[a-zA-Z0-9_-]+", name):
                raise ValueError("Unsafe peer receipt identifier")
            stdout = (output / (name + ".stdout")).open("wb")
            stderr = (output / (name + ".stderr")).open("wb")
            streams += [stdout, stderr]
            command = [expand(item) for item in entry["argv"]]
            env = environment({**manifest.get("environment", {}), **entry.get("environment", {})})
            process = subprocess.Popen(command, env=env, stdout=stdout, stderr=stderr, start_new_session=True)
            record = {"id": name, "argv": command, "pid": process.pid, "ready": False}
            children.append((process, record))
            report["peers"].append(record)
            save()
            ready = entry["ready"]
            deadline = time.monotonic() + entry.get("timeout_seconds", 45)
            while True:
                path = Path(expand(ready["path"]))
                observed = events(path) if ready["kind"] == "jsonl" else []
                if any(matches(row, ready["match"]) for row in observed):
                    record.update(ready=True, initial_native_events=observed)
                    break
                if ready["kind"] == "text" and path.exists() and ready["contains"] in path.read_text():
                    record.update(ready=True)
                    break
                if process.poll() is not None or time.monotonic() >= deadline:
                    raise RuntimeError("Native participant failed readiness: " + name)
                time.sleep(0.05)
            record["mapped_files"] = mapped_files(process.pid, output / (name + ".maps"))
            save()
        for entry in manifest["cases"]:
            if not re.fullmatch(r"[a-zA-Z0-9_-]+", entry["id"]):
                raise ValueError("Unsafe case receipt identifier")
            directory = output / entry["id"]
            directory.mkdir()
            fixture_files(entry.get("fixture_files", {}))
            wasm = Path(expand(entry["wasm"]))
            if sha256(wasm) != entry["sha256"]:
                raise ValueError("Frozen Wasm changed: " + entry["id"])
            memories = wasm_memories(wasm)
            if any(memory["bits"] != entry["memory_bits"] for memory in memories):
                raise ValueError("Actual Wasm memory width differs from the selected target")
            witness = entry["native_witness"]
            event_path = Path(expand(witness["path"]))
            before = len(events(event_path))
            command = [expand(item) for item in entry["argv"]]
            if str(wasm) not in command:
                raise ValueError("The exact admitted Wasm must occur in the case argv")
            runtime_paths = {str(assets[name]) for name, asset in manifest["assets"].items()
                             if asset["role"] == "runtime"}
            launcher_paths = {str(assets[name]) for name, asset in manifest["assets"].items()
                              if asset["role"] == "launcher"
                              and asset.get("runtime_asset") in manifest["assets"]
                              and manifest["assets"][asset["runtime_asset"]]["role"] == "runtime"}
            if not (runtime_paths | launcher_paths).intersection(command):
                raise ValueError("A case must execute the admitted runtime or its hash-paired launcher")
            preloads = admit_preloads(entry, command, expand)
            row = {"id": entry["id"], "argv": command, "wasm_sha256": entry["sha256"],
                   "memories": memories, "preload_modules":preloads,
                   "main_module_name":entry.get("main_module_name"), "status": "running"}
            report["cases"].append(row)
            save()
            started = time.monotonic()
            with (directory / "run.stdout").open("wb") as out, (directory / "run.stderr").open("wb") as err:
                process = subprocess.Popen(command, env=environment(manifest.get("environment", {})),
                                           stdout=out, stderr=err, start_new_session=True)
                try:
                    code = process.wait(timeout=entry.get("timeout_seconds", 180))
                except subprocess.TimeoutExpired:
                    row.update(status="timeout", cleanup=stop(process))
                    save()
                    raise
            row.update(exit_code=code, elapsed_seconds=time.monotonic() - started)
            if code != entry["expected_exit_code"]:
                raise RuntimeError("Unexpected guest exit: " + entry["id"])
            stdout = (directory / "run.stdout").read_text()
            stderr = (directory / "run.stderr").read_text()
            for text in entry.get("stdout_contains", []):
                if text not in stdout:
                    raise ValueError("Missing complete guest observation: " + text)
            for text in entry.get("stderr_contains", []):
                if text not in stderr:
                    raise ValueError("Missing exact runtime refusal diagnostic: " + text)
            quiet = witness.get("quiet_seconds", 0.25)
            if not isinstance(quiet, (int, float)) or not 0.1 <= quiet <= 5:
                raise ValueError("Denied-effect observation window must be 0.1..5 seconds")
            deadline = time.monotonic() + (quiet if "absent" in witness else 10)
            while True:
                observed = events(event_path)
                candidates = observed[before:] if witness.get("after_case_start", True) else observed
                selected = [value for value in candidates if matches(value, witness.get("match", witness.get("absent", {})))]
                if "absent" in witness and (selected or time.monotonic() >= deadline):
                    break
                if "absent" in witness:
                    time.sleep(0.01)
                    continue
                if selected or time.monotonic() >= deadline:
                    break
                time.sleep(0.05)
            if "absent" in witness and selected:
                raise ValueError("A denied operation caused an independently observed native effect")
            if "absent" not in witness and not selected:
                raise ValueError("No independently observed native event for " + entry["id"])
            row["native_witnesses"] = selected
            if "wire_stdout_regex" in witness:
                found = re.search(witness["wire_stdout_regex"], stdout)
                if not found or not any(found.group(1) == value.get(witness.get("wire_field", "wire_hex")) for value in selected):
                    raise ValueError("Complete guest/native wire equality failed")
            row["status"] = "passed"
            save()
        if not report["cases"]:
            raise ValueError("An empty cohort cannot qualify runtime execution")
        if any(process.poll() is not None for process, _ in children):
            raise RuntimeError("A required native peer exited during acceptance")
        report["status"] = "passed"
    except BaseException as error:
        report.update(status="failed", error={"type": type(error).__name__, "message": str(error)})
    finally:
        for process, row in reversed(children):
            row["cleanup"] = stop(process)
        for stream in streams:
            stream.close()
        report["cleanup_passed"] = all(row["cleanup"]["reaped"] and not row["cleanup"]["forced"]
                                        for _, row in children)
        save()
        hashes = [{"path": str(path.relative_to(output)), "sha256": sha256(path), "bytes": path.stat().st_size}
                  for path in sorted(output.rglob("*")) if path.is_file() and path.name != "files.sha256.json"]
        (output / "files.sha256.json").write_text(json.dumps(hashes, indent=2) + "\n")
    return 0 if report["status"] == "passed" and report["cleanup_passed"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
