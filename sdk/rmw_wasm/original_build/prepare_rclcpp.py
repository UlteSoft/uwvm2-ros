#!/usr/bin/env python3
"""Extract the fixed original Git tree and apply four explicit source ports.

This build-time tool prepares SOURCE only. It does not build or run a module,
install an SDK, choose NodeOptions, create a ROS endpoint, or grant authority.
Use an independently admitted real Git executable and a complete local object
database. Lazy network object acquisition, replacement objects, submodules and
symbolic links are refused. A fresh caller-owned destination is mandatory.
All compiler/Guest/native/import/permission qualification remains independent.
"""
from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import stat
import subprocess

COMMIT = "53fa7aeea86e8ed484f9465fa14059faf13a71f1"
MAX_FILES = 8192
MAX_FILE_BYTES = 64 * 1024 * 1024
MAX_TOTAL_BYTES = 256 * 1024 * 1024
MAX_TREES = 2048
MAX_TREE_BYTES = 16 * 1024 * 1024
MAX_TREE_DEPTH = 64
ORIGINALS = {
    "rclcpp/CMakeLists.txt": "a01f644852207f35f39c906e251e1c7ad0771e4b43e3b4f23eab5ee6593d6c89",
    "rclcpp/src/rclcpp/signal_handler.hpp": "1d68b69acd82a8f70e6a4f0189eade6a7c71dac11acede13a9696432e0128532",
    "rclcpp/src/rclcpp/signal_handler.cpp": "3b914a317f73a75bd9771e55c2735b371fc96d0e7d888392be3d82e1b868bea3",
    "rclcpp/include/rclcpp/service.hpp": "d1c1ecedd27015c8d64f91ba7b7573efc5b09087724db9e59d1400adc0b9e6e4",
    "rclcpp/include/rclcpp/any_service_callback.hpp": "aae11292c4d4f24a231175cd605ee3e4c341c6ee7ce3b6f43100c34c339b786d",
}
PORT_TARGETS = set(ORIGINALS) - {"rclcpp/CMakeLists.txt"}


def digest(data: bytes) -> str:
    """Return a complete body digest, never a filename-derived identity."""
    return hashlib.sha256(data).hexdigest()


def regular_file(path: Path, maximum: int) -> bytes:
    """Read one finite regular source body, refusing a final symlink."""
    with path.open("rb") as stream:
        actual = os.fstat(stream.fileno())
        link = path.lstat()
        if (not stat.S_ISREG(actual.st_mode) or stat.S_ISLNK(link.st_mode)
                or (actual.st_dev, actual.st_ino) != (link.st_dev, link.st_ino)
                or actual.st_size > maximum):
            raise ValueError(f"Expected one bounded regular input: {path}")
        data = stream.read(maximum + 1)
        after = os.fstat(stream.fileno())
        if (len(data) != actual.st_size or len(data) > maximum
                or (actual.st_size, actual.st_mtime_ns, actual.st_ctime_ns)
                != (after.st_size, after.st_mtime_ns, after.st_ctime_ns)):
            raise ValueError(f"Input changed while reading: {path}")
        return data


def relative_path(text: str) -> PurePosixPath:
    """Admit portable source member names without traversal or aliases."""
    name = PurePosixPath(text)
    if (not text or name.is_absolute() or str(name) != text
            or any(part in (".", "..", ".git") for part in name.parts)
            or "\\" in text or any(ord(c) < 32 or ord(c) == 127 for c in text)):
        raise ValueError(f"Noncanonical source member: {text!r}")
    return name


def git_object_id(kind: str, data: bytes) -> str:
    """Verify the genuine Git SHA-1 object representation, including its header."""
    return hashlib.sha1(kind.encode() + b" " + str(len(data)).encode() + b"\0" + data).hexdigest()


class Git:
    """Bounded local reads through the explicitly admitted real Git program.

    No shell, checkout, hook, build, filter or network command is invoked.
    Object sizes are checked before reading bodies. Git commands have separate
    finite deadlines; the external harness also owns the entire process budget.
    """
    def __init__(self, executable: Path, repository: Path, expected: str):
        if not executable.is_absolute() or not repository.is_absolute():
            raise ValueError("Git executable and repository must be absolute")
        body = regular_file(executable, MAX_FILE_BYTES)
        if not re.fullmatch(r"[0-9a-f]{64}", expected) or digest(body) != expected:
            raise ValueError("Actual Git executable body differs from the admitted hash")
        self.binary = executable
        self.repository = repository
        self.environment = {k: v for k, v in os.environ.items()
                            if not k.startswith("GIT_") and not k.startswith("DYLD_")
                            and k not in ("LD_PRELOAD", "LD_LIBRARY_PATH")}
        self.environment.update({"GIT_NO_LAZY_FETCH": "1", "GIT_CONFIG_NOSYSTEM": "1",
                                 "GIT_CONFIG_GLOBAL": os.devnull, "LC_ALL": "C"})
        self.executable_sha256 = expected

    def run(self, args: list[str], maximum: int) -> bytes:
        argv = [str(self.binary), "--no-pager", "--no-replace-objects",
                "-c", "core.fsmonitor=false", "-c", "core.hooksPath=" + os.devnull,
                "-C", str(self.repository), *args]
        result = subprocess.run(argv, stdin=subprocess.DEVNULL, capture_output=True,
                                timeout=30, env=self.environment, check=False)
        if result.returncode or len(result.stdout) > maximum or len(result.stderr) > 65536:
            raise ValueError(f"Bounded local Git operation refused: {args!r}, code={result.returncode}")
        return result.stdout

    def object(self, kind: str, oid: str) -> bytes:
        if not re.fullmatch(r"[0-9a-f]{40}", oid):
            raise ValueError("An exact Git SHA-1 object is required")
        raw_size = self.run(["cat-file", "-s", oid], 32).strip()
        if not re.fullmatch(rb"[0-9]{1,9}", raw_size):
            raise ValueError("Invalid Git object extent")
        size = int(raw_size)
        if size > MAX_FILE_BYTES:
            raise ValueError("Git object exceeds the explicit source quota")
        data = self.run(["cat-file", kind, oid], size)
        if len(data) != size or git_object_id(kind, data) != oid:
            raise ValueError("Git object body/header identity mismatch")
        return data


def verified_inventory(git: Git, root_oid: str):
    """Derive membership only from recursively hash-verified binary trees.

    `ls-tree -r` alone does not verify that an intermediate stored tree's body
    matches its requested OID. Verifying the root and resulting blobs would
    therefore leave directory-to-blob membership unproven. Every traversed tree
    is read and hashed here before any of its entries is used. Quotas count
    expanded directory/file occurrences even when the same tree OID is reused.
    """
    inventory = []
    trees = []
    names = set()
    active = set()
    tree_bytes = 0
    total_bytes = 0

    def visit(oid: str, prefix: str, depth: int):
        nonlocal tree_bytes, total_bytes
        if depth > MAX_TREE_DEPTH or len(trees) >= MAX_TREES or oid in active:
            raise ValueError("Tree depth/count/cycle quota exceeded")
        raw = git.object("tree", oid)
        tree_bytes += len(raw)
        if tree_bytes > MAX_TREE_BYTES:
            raise ValueError("Expanded verified-tree byte quota exceeded")
        trees.append({"path":prefix,"git_tree":oid,"bytes":len(raw),"sha256":digest(raw)})
        active.add(oid)
        at = 0
        while at < len(raw):
            space = raw.find(b" ", at)
            nul = raw.find(b"\0", space + 1) if space >= 0 else -1
            if space <= at or nul <= space + 1 or len(raw) - nul - 1 < 20:
                raise ValueError("Incomplete binary Git tree entry")
            mode = raw[at:space].decode("ascii")
            component = raw[space + 1:nul].decode("utf-8")
            if "/" in component:
                raise ValueError("A binary Git tree component cannot contain a slash")
            name = str(relative_path(prefix + component))
            child = raw[nul + 1:nul + 21].hex()
            at = nul + 21
            if name in names:
                raise ValueError("Duplicate directory/file source member")
            names.add(name)
            if name.startswith(".uwvm-"):
                raise ValueError("Original member conflicts with preparation metadata")
            if mode == "40000":
                visit(child, name + "/", depth + 1)
            elif mode in ("100644", "100755"):
                if len(inventory) >= MAX_FILES:
                    raise ValueError("Original source file-count quota exceeded")
                raw_size = git.run(["cat-file", "-s", child], 32).strip()
                if not re.fullmatch(rb"[0-9]{1,9}", raw_size):
                    raise ValueError("Invalid member extent")
                size = int(raw_size)
                total_bytes += size
                if size > MAX_FILE_BYTES or total_bytes > MAX_TOTAL_BYTES:
                    raise ValueError("Complete original source exceeds the explicit byte quota")
                inventory.append((name, mode, child, size))
            else:
                raise ValueError("No submodules, symlinks or special source members")
        active.remove(oid)

    visit(root_oid, "", 1)
    return inventory, trees, total_bytes


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--git", type=Path, required=True,
                        help="Absolute independently admitted real Git executable")
    parser.add_argument("--git-sha256", required=True,
                        help="Actual complete admitted Git executable SHA-256")
    parser.add_argument("--original-repository", type=Path, required=True,
                        help="Complete local original ros2/rclcpp Git object database")
    parser.add_argument("--source-root", type=Path, required=True,
                        help="Actual reviewed uwvm2-ros source tree containing the four ports")
    parser.add_argument("--ports", type=Path, required=True,
                        help="Actual reviewed ports.json belonging to this source cohort")
    parser.add_argument("--ports-sha256", required=True,
                        help="Independently recorded complete ports.json SHA-256")
    parser.add_argument("--output", type=Path, required=True,
                        help="Fresh absolute caller-owned source directory; never merged/replaced")
    args = parser.parse_args()
    if not all(path.is_absolute() for path in
               [args.source_root, args.ports, args.output, args.original_repository]):
        raise ValueError("All input/output filesystem roles must be absolute")
    ports_bytes = regular_file(args.ports, 65536)
    if digest(ports_bytes) != args.ports_sha256:
        raise ValueError("The actual reviewed port manifest has changed")
    ports = json.loads(ports_bytes)
    if (ports.get("format") != "uwvm-original-rclcpp-four-source-ports-v1"
            or ports.get("original_commit") != COMMIT
            or not isinstance(ports.get("ports"), list) or len(ports["ports"]) != 4):
        raise ValueError("Wrong fixed original port cohort")
    replacements = {}
    for port in ports["ports"]:
        target = str(relative_path(port["original_path"]))
        replacement = relative_path(port["replacement_path"])
        if target not in PORT_TARGETS or target in replacements:
            raise ValueError("The four exact original source members are required")
        if port["before_sha256"] != ORIGINALS[target]:
            raise ValueError("An original preimage cannot be retagged")
        data = regular_file(args.source_root / replacement, MAX_FILE_BYTES)
        if digest(data) != port["after_sha256"] or len(data) != port["after_bytes"]:
            raise ValueError("Actual complete port body differs from its reviewed identity")
        replacements[target] = data
    if set(replacements) != PORT_TARGETS:
        raise ValueError("Incomplete four-member source composition")

    git = Git(args.git, args.original_repository, args.git_sha256)
    commit = git.object("commit", COMMIT)
    first = commit.split(b"\n", 1)[0]
    if not re.fullmatch(rb"tree [0-9a-f]{40}", first):
        raise ValueError("Original commit has no exact root tree")
    tree = first[5:].decode("ascii")
    inventory, verified_trees, total = verified_inventory(git, tree)
    names = {row[0] for row in inventory}
    if not set(ORIGINALS).issubset(names):
        raise ValueError("The complete original package source is missing")
    # Materialize only after all port inputs and the complete tree inventory are
    # admitted. Use exclusive creation; a failed partial output is never reused.
    parent = args.output.parent.resolve(strict=True)
    output = parent / args.output.name
    if (output.is_relative_to(args.source_root.resolve())
            or output.is_relative_to(args.original_repository.resolve())):
        raise ValueError("The original and reviewed source directories cannot contain outputs")
    output.mkdir(mode=0o700)
    records = []
    try:
        for name, mode, oid, size in inventory:
            original = git.object("blob", oid)
            if len(original) != size:
                raise ValueError("Original source extent changed")
            original_sha = digest(original)
            if name in ORIGINALS and original_sha != ORIGINALS[name]:
                raise ValueError("Original member does not match the reviewed preimage")
            data = replacements.get(name, original)
            target = output / name
            target.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
            flags = os.O_WRONLY | os.O_CREAT | os.O_EXCL
            if hasattr(os, "O_NOFOLLOW"):
                flags |= os.O_NOFOLLOW
            fd = os.open(target, flags, 0o755 if mode == "100755" else 0o644)
            with os.fdopen(fd, "wb") as stream:
                stream.write(data)
            records.append({"path":name,"git_mode":mode,"original_git_blob":oid,
                            "original_bytes":size,"original_sha256":original_sha,
                            "after_bytes":len(data),"after_sha256":digest(data),
                            "ported":name in replacements})
        report = {"status":"COMPLETE_ORIGINAL_SOURCE_WITH_FOUR_EXPLICIT_PORTS",
                  "utc":datetime.datetime.now(datetime.timezone.utc).isoformat(),
                  "original_commit":COMMIT,"original_tree":tree,
                  "original_commit_sha256":digest(commit),
                  "verified_tree_objects":verified_trees,
                  "actual_git_sha256":git.executable_sha256,
                  "port_manifest_sha256":digest(ports_bytes),"files":records,
                  "original_file_count":len(inventory),"original_total_bytes":total,
                  "candidate_build_or_execution":0,
                  "qualification":"Source extraction only. Original application sources, NodeOptions and all other members are byte-exact. No Runtime, SDK, native, imports, rights or official-suite acceptance is implied."}
        (output/".uwvm-source.json").write_text(json.dumps(report, indent=2)+"\n")
    except BaseException:
        (output/".uwvm-preparation-incomplete.json").write_text(
            json.dumps({"status":"INCOMPLETE_DO_NOT_BUILD_OR_REUSE","completed_files":len(records)})+"\n")
        raise
    print(json.dumps({"status":report["status"],"output":str(output),
                      "files":len(records),"ported":len(replacements)}))


if __name__ == "__main__":
    main()
