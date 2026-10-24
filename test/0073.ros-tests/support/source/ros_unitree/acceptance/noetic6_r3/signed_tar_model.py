#!/usr/bin/env python3
"""Read-only signed-tar virtual model shared with the independently reviewed r5.

Only the class below is retained literally from noetic_partial_resume.py
41661B ae69eef2d2a9fc86b338360aff272ba50b9c4d140dcd777db53d9b6dff18c4b9.
No historical report, ledger or partial package count supplies authority here.
The r6 caller passes a fresh generated ledger sink to audit(), after verifying
every tar against newly authenticated package bytes. No module import runs it.
"""
from __future__ import annotations
import copy
import hashlib
import json
import os
from pathlib import Path
import posixpath
import stat
import tarfile
MIB=1024**2
def need(value,message):
    if not value:raise ValueError(message)
def facts(info):
    return (info.st_dev,info.st_ino,info.st_mode,info.st_size,info.st_mtime_ns,info.st_ctime_ns,info.st_nlink)
def directory_pin(path,info):
    return {"path":str(path),"device":info.st_dev,"inode":info.st_ino,"mode":info.st_mode,"mtime_ns":info.st_mtime_ns,"ctime_ns":info.st_ctime_ns}

class VirtualTree:
    """Read-only tar replay with the maintained SafeTree alias/overlay rules.

    Nodes are physical SDK-relative names, including implicitly created parent
    directories. Deferred hardlinks do not become files until SafeTree.finish.
    Directory permissions are not supplied by package metadata in SafeTree;
    physical verification records them and rejects writeable/set-id variants.
    Regular-file modes and bytes are derived exactly from authenticated tar.
    """
    def __init__(self, maintained, limits, guard):
        self.maintained, self.limits, self.guard = maintained, limits, guard
        self.nodes = {"": {"kind": "directory"}}
        self.entries = self.bytes = self.sequence = 0
        self.pending = []

    def parent(self, parts):
        todo, stack, seen = list(parts), [], 0
        while todo:
            self.guard()
            part = todo.pop(0)
            if part in ("", "."):
                continue
            if part == "..":
                need(stack, "replayed SDK alias escapes root")
                stack.pop(); todo = stack + todo; stack = []; continue
            path = "/".join(stack + [part])
            node = self.nodes.get(path)
            if node is None:
                node = {"kind": "directory"}; self.nodes[path] = node
            if node["kind"] == "symlink":
                seen += 1; need(seen <= 40, "replayed SDK alias loop")
                target = node["physical_target"]
                need(not target.startswith("/"), "physical absolute replay alias refused")
                todo = target.split("/") + todo; continue
            need(node["kind"] == "directory", "replayed parent changes existing type")
            stack.append(part)
        return stack

    def checkpoint(self, ledger):
        return {"ledger_end": ledger.offset, "ledger_rows": ledger.rows,
                "entries": self.entries, "bytes": self.bytes, "sequence": self.sequence,
                "pending": copy.deepcopy(self.pending)}

    def audit(self, path, package, ledger, *, partial=False):
        self.maintained.preflight_tar(path, self.guard)
        local = matched = 0
        with Path(path).open("rb") as body, tarfile.open(fileobj=body, mode="r|") as archive:
            for member in archive:
                self.guard()
                # The failed extraction's guard ran before changing a member.
                # An EOF ledger suffix therefore authorizes no following node.
                if partial and ledger.next_row is None:
                    archive.members.clear(); return matched
                self.entries += 1
                need(self.entries <= self.limits["max_entries"], "replayed SDK entry bound exceeded")
                parts = self.maintained.SafeTree.path_parts(self, member.name)
                if not parts:
                    need(member.isdir(), "only directory may describe replay root")
                    archive.members.clear(); continue
                row = {"package": package, "member": member.name, "original_mode": member.mode,
                       "uid": member.uid, "gid": member.gid, "mtime": member.mtime,
                       "owner_and_setid_preserved": False}
                actual = self.parent(parts[:-1]); name = parts[-1]
                physical = "/".join(actual + [name]); row["physical_relative"] = physical
                old = self.nodes.get(physical)
                if member.isdir():
                    if old is None:
                        self.nodes[physical] = {"kind": "directory"}
                    elif old["kind"] == "symlink":
                        self.parent(parts)
                    else:
                        need(old["kind"] == "directory", "directory replay changes existing type")
                    row["kind"] = "directory"
                elif member.issym():
                    target = member.linkname
                    need(target and "\0" not in target and len(target.encode()) <= 4096, "bounded replay symlink required")
                    logical = posixpath.normpath(target.lstrip("/") if target.startswith("/") else posixpath.join(*parts[:-1], target))
                    need(logical != ".." and not logical.startswith("../"), "logical replay alias escapes root")
                    rewritten = posixpath.relpath(logical, "/".join(actual) or ".")
                    node = {"kind": "symlink", "physical_target": rewritten}
                    need(old is None or old == node, "conflicting replay symlink overlay")
                    self.nodes[physical] = node
                    row.update(kind="symlink", original_target=target, physical_target=rewritten)
                elif member.islnk():
                    target = self.maintained.SafeTree.path_parts(self, member.linkname)
                    need(target and old is None, "replayed hardlink must not replace a node")
                    need(len(self.pending) < self.limits["max_pending_hardlinks"], "replayed hardlink bound exceeded")
                    self.pending.append((parts, target, dict(row)))
                    row.update(kind="hardlink-pending", original_target=member.linkname)
                elif member.isfile():
                    need(0 <= member.size <= self.limits["max_file_bytes"], "replayed file bound exceeded")
                    local += member.size; self.bytes += member.size; self.sequence += 1
                    need(local <= self.limits["max_tar_bytes"] and self.bytes <= self.limits["max_sdk_bytes"],
                         "replayed expanded byte limit exceeded")
                    source = archive.extractfile(member); need(source is not None, "replayed file body missing")
                    digest = hashlib.sha256(); count = 0
                    while True:
                        self.guard(); block = source.read(MIB)
                        if not block:
                            break
                        count += len(block); need(count <= member.size, "replayed member grew")
                        digest.update(block)
                    need(count == member.size, "truncated authenticated replay member")
                    node = {"kind": "file", "bytes": count, "sha256": digest.hexdigest(), "mode": member.mode & 0o777}
                    need(old is None or old == node, "conflicting replay regular-file overlay")
                    self.nodes[physical] = node
                    row.update(node)
                else:
                    need(False, "special authenticated tar node refused in replay")
                ledger.match(row); matched += 1; archive.members.clear()
        need(not partial or ledger.next_row is None, "retained partial ledger exceeds authenticated package tar")
        return matched

    def verify_physical(self, rootfd, output, guard):
        """Hash the entire actual tree through retained directory descriptors.

        No symlink target is followed, no hardlink is admitted before finish,
        and the complete name set must equal the independently replayed nodes.
        A second complete pass at the transfer boundary catches intervening
        name, inode, link, permission or byte changes. The root's path itself
        may change only during the separately recorded atomic transfer.
        """
        root_first = os.fstat(rootfd); device = root_first.st_dev
        seen = set(); content = hashlib.sha256(); identities = hashlib.sha256()
        files = total = directories = links = 0
        with Path(output).open("xb") as stream:
            def walk(fd, relative):
                nonlocal files, total, directories, links
                guard(); before = os.fstat(fd)
                need(stat.S_ISDIR(before.st_mode) and before.st_dev == device and
                     not before.st_mode & 0o7022, "unsafe or cross-filesystem SDK directory")
                names = sorted(os.listdir(fd))
                need(len(names) <= self.limits["max_entries"], "SDK directory entry bound exceeded")
                for name in names:
                    guard(); path = relative + "/" + name if relative else name
                    need(path in self.nodes and path not in seen, "unexpected actual SDK name: " + path)
                    need(not name.startswith(".uwvm-acquire-") and not name.endswith(".pending"), "uncommitted SDK temporary refused")
                    seen.add(path); expected = self.nodes[path]
                    first = os.stat(name, dir_fd=fd, follow_symlinks=False)
                    need(first.st_dev == device, "SDK crossed a mounted filesystem")
                    row = {"relative": path, "kind": expected["kind"]}
                    if expected["kind"] == "directory":
                        need(stat.S_ISDIR(first.st_mode), "actual replay directory kind differs")
                        other = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW, dir_fd=fd)
                        try:
                            need(facts(first) == facts(os.fstat(other)), "SDK directory changed while opening")
                            walk(other, path)
                        finally:
                            os.close(other)
                        directories += 1; row["mode"] = stat.S_IMODE(first.st_mode)
                    elif expected["kind"] == "symlink":
                        need(stat.S_ISLNK(first.st_mode) and os.readlink(name, dir_fd=fd) == expected["physical_target"],
                             "actual authenticated SDK link differs")
                        row["physical_target"] = expected["physical_target"]; links += 1
                    else:
                        need(stat.S_ISREG(first.st_mode) and first.st_nlink == 1 and
                             first.st_size == expected["bytes"] and stat.S_IMODE(first.st_mode) == expected["mode"],
                             "actual SDK file metadata differs")
                        opened = os.open(name, os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK | os.O_NOFOLLOW, dir_fd=fd)
                        try:
                            need(facts(first) == facts(os.fstat(opened)), "SDK file changed while opening")
                            digest = hashlib.sha256(); count = 0
                            while True:
                                guard(); block = os.read(opened, MIB)
                                if not block:
                                    break
                                count += len(block); need(count <= expected["bytes"], "actual SDK file grew")
                                digest.update(block)
                            need(count == expected["bytes"] and digest.hexdigest() == expected["sha256"] and
                                 facts(first) == facts(os.fstat(opened)), "actual SDK file bytes changed or differ")
                        finally:
                            os.close(opened)
                        row.update({k: expected[k] for k in ("bytes", "sha256", "mode")}); files += 1; total += count
                    need(facts(first) == facts(os.stat(name, dir_fd=fd, follow_symlinks=False)), "actual SDK name changed during read")
                    encoded = (json.dumps(row, sort_keys=True, separators=(",", ":")) + "\n").encode()
                    stream.write(encoded); content.update(encoded)
                    identities.update(json.dumps([path, *facts(first)], separators=(",", ":")).encode() + b"\n")
                need(names == sorted(os.listdir(fd)) and facts(before) == facts(os.fstat(fd)), "actual SDK directory changed during traversal")
            walk(rootfd, "")
            need(seen == set(self.nodes) - {""}, "actual SDK tree is missing replayed names")
            stream.flush(); os.fsync(stream.fileno())
        need(facts(root_first) == facts(os.fstat(rootfd)), "held SDK root changed during reconciliation")
        return {"content_sha256": content.hexdigest(), "physical_identity_sha256": identities.hexdigest(),
                "files": files, "bytes": total, "directories": directories, "symlinks": links,
                "root": directory_pin("HELD_DIRECTORY_DESCRIPTOR", root_first), "entries": len(seen),
                "scope": "ALL_CURRENT_PHYSICAL_NAMES_AND_BYTES_EQUAL_AUTHENTICATED_TAR_REPLAY; NO_EXECUTION"}

