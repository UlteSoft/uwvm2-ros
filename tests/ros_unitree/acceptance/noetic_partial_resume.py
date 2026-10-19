#!/usr/bin/env python3
"""Reconcile a failed Noetic extraction before moving its SDK directory.

This module is an explicit input to the r5 acquisition producer. Importing it
starts no process and grants no authority. The producer must first obtain fresh
verified Release/Packages metadata and solve an empty-status APT closure. This
module then independently decodes authenticated DEBs, replays their tar members
against the retained ledger, and compares the resulting physical SDK tree.

Historical reports and extraction counters remain historical. Only freshly
opened bytes and a newly reconstructed checkpoint may initialize the resumed
SafeTree. The old SDK is transferred by one same-filesystem, no-replace rename;
reports, DEBs, logs and the original ledger remain at their original names.
An intent without a matching confirmation deliberately requires a separately
reviewed recovery decision. It is never automatically treated as a completed
transfer or permission to delete an apparent duplicate.
"""
from __future__ import annotations

import copy
import ctypes
import hashlib
import json
import os
from pathlib import Path
import posixpath
import stat
import tarfile

MIB = 1024 ** 2
PIN = {"path", "bytes", "sha256", "device", "inode", "mtime_ns", "ctime_ns"}
DIRECTORY = {"path", "device", "inode", "mode", "mtime_ns", "ctime_ns"}
IDENTITY = ("package", "version", "architecture", "filename", "bytes", "sha256", "uri")


def need(value, message):
    if not value:
        raise ValueError(message)


def pairs(rows):
    result = {}
    for key, value in rows:
        need(key not in result, "duplicate continuation JSON field")
        result[key] = value
    return result


def parse(body):
    return json.loads(body, object_pairs_hook=pairs,
                      parse_constant=lambda _: need(False, "nonfinite continuation JSON"))


def facts(info):
    return (info.st_dev, info.st_ino, info.st_mode, info.st_size,
            info.st_mtime_ns, info.st_ctime_ns, info.st_nlink)


def directory_pin(path, info):
    return {"path": str(path), "device": info.st_dev, "inode": info.st_ino,
            "mode": info.st_mode, "mtime_ns": info.st_mtime_ns, "ctime_ns": info.st_ctime_ns}


def directory_fd(path):
    """Open every absolute ancestor without following a symlink."""
    path = Path(path)
    need(path.is_absolute() and str(path) == str(path.absolute()) and
         all(part not in (".", "..") for part in path.parts), "literal physical directory required")
    fd = os.open(path.anchor, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC)
    try:
        for part in path.parts[1:]:
            other = os.open(part, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW, dir_fd=fd)
            os.close(fd); fd = other
        return fd
    except BaseException:
        os.close(fd); raise


class Ledger:
    """Bounded sequential comparison against one admitted, stable old ledger."""
    def __init__(self, path, expected, guard):
        self.path, self.expected, self.guard = Path(path), expected, guard
        self.fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
        self.first = os.fstat(self.fd)
        need(stat.S_ISREG(self.first.st_mode) and self.first.st_nlink == 1 and
             tuple(expected[k] for k in ("device", "inode", "bytes", "mtime_ns", "ctime_ns")) ==
             (self.first.st_dev, self.first.st_ino, self.first.st_size, self.first.st_mtime_ns, self.first.st_ctime_ns),
             "admitted old ledger identity changed")
        self.stream = os.fdopen(os.dup(self.fd), "rb")
        self.offset = self.rows = 0
        self.next_row = None
        self._read()

    def _read(self):
        self.guard()
        raw = self.stream.readline(65537)
        need(len(raw) <= 65536 and (not raw or raw.endswith(b"\n")), "bounded complete ledger line required")
        self.next_row = None if not raw else parse(raw)
        if raw:
            need(type(self.next_row) is dict, "ledger object required")
        self.next_raw = raw

    def match(self, row):
        need(self.next_row is not None and row == self.next_row, "authenticated tar member differs from retained ledger")
        self.offset += len(self.next_raw); self.rows += 1
        self._read()

    def close(self):
        try:
            last = os.fstat(self.fd)
            named = self.path.stat(follow_symlinks=False)
            need(facts(self.first) == facts(last) == facts(named), "old ledger changed during reconciliation")
        finally:
            self.stream.close(); os.close(self.fd)


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


class ResumeSession:
    """Finite r4 checkpoint reconciliation owned by one fresh r5 producer."""
    def __init__(self, *, control, restart, maintained, common, guard, remember, product,
                 report, run, decode, tools, limits, out, inputs, products):
        self.control, self.restart, self.maintained, self.common = control, restart, maintained, common
        self.guard, self.remember, self.product = guard, remember, product
        self.report, self.run, self.decode = report, run, decode
        self.tools, self.limits, self.out = tools, limits, Path(out)
        self.inputs, self.products = inputs, products
        self.fds = []
        self.original = None
        self.current_ledger = None
        self.audit_dir = self.out / "resume-audit"
        self.audit_dir.mkdir(mode=0o700)
        self.audit_rows = []
        self.control_logs = {}

    def document(self, expected, limit=16 * MIB):
        need(type(expected) is dict and set(expected) == PIN, "explicit current continuation document full7 required")
        pin, body = self.restart.opened_file(expected["path"], limit, self.guard, collect=True)
        need(pin == expected and self.remember(pin["path"], pin["sha256"]) == pin, "current continuation document changed")
        return parse(body)

    def historical(self, expected, limit=16 * MIB):
        need(type(expected) is dict and set(expected) == PIN and expected["device"] == 81,
             "exact original artifact-device historical pin required")
        path = Path(expected["path"])
        need(path.parent == Path(self.control["old_report"]["path"]).parent and
             path.name.startswith("deb-") and path.name.endswith("-checked-tar-retirement.json"),
             "only schema-selected original retirement documents may be bridged")
        current = self.common.opened(Path(expected["path"]), limit)
        relation = self.restart.requalify_persistent_pin(expected, expected["path"], self.guard,
            historical_device=81, current_device=self.control["current_artifact_device"], observed_current=current)
        need(self.remember(current["path"], current["sha256"]) == current, "requalified historical file changed")
        self.audit_rows.append(relation)
        _, body = self.restart.opened_file(current["path"], limit, self.guard, collect=True)
        return parse(body), current

    def prerequisites(self):
        control = self.control
        required = {"format_version", "kind", "old_report", "old_outer_report", "old_ledger", "old_sdk",
                    "last_retirement", "committed_packages", "complete_packages", "checkpoint",
                    "source_manifest_sha256", "current_artifact_device", "scope"}
        need(type(control) is dict and set(control) == required and control["format_version"] == 1 and
             control["kind"] == "noetic-r4-partial-sdk-r5-reconciliation" and
             control["scope"] == "FRESH_SIGNED_METADATA_FIRST; CONTENT_RECONCILIATION_THEN_SDK_DIRECTORY_ONLY_ATOMIC_TRANSFER" and
             control["committed_packages"] == 255 and control["complete_packages"] == 513 and
             control["current_artifact_device"] == 68, "closed exact Noetic continuation scope required")
        old = self.document(control["old_report"]); outer = self.document(control["old_outer_report"])
        need(control["old_report"]["bytes"] == 3615127 and control["old_report"]["sha256"] ==
             "f37195059b17d68573edd9e1b072dc32c933dc3a1a9fc7fed8ec4a9f9f738bc3" and
             control["old_outer_report"]["bytes"] == 57193 and control["old_outer_report"]["sha256"] ==
             "49272f388877f7fb118c9adf0f2ef32dd2e6fbee6654e5e86eafa051cbd01b0b",
             "exact retained raw failed reports required")
        need(old["status"] == "FAILED_ACQUISITION" and outer["status"] == "FAIL" and
             old["source_manifest_sha256"] == control["source_manifest_sha256"] and
             old["failed_phase"] == "serial-download-extraction" and
             old["error"].startswith("ValueError('output/ancestor filesystem reserve exhausted:") and
             old["cleanup"]["status"] == "PASS" and not old["cleanup"]["remaining"] and
             len(old["packages"]) == 255 and not old.get("acquisition_complete", False),
             "only genuine failed reserve-bound partial SDK may be reconciled")
        need(all(row["status"] == "PASS" and row.get("returncode") == 0 for row in old["commands"]),
             "historical package commands must be genuinely terminal successes, not fabricated extraction success")
        self.original = old
        self.current_ledger = control["old_ledger"]
        need(set(self.current_ledger) == PIN and self.current_ledger["bytes"] == 7719810 and
             self.current_ledger["sha256"] == "6f05b3f2b70e8a332de261d48ad715a3ec86c579dad45235d93b81b77d4adb24" and
             self.current_ledger["device"] == 68 and self.current_ledger["inode"] == 721975,
             "exact independently observed partial ledger required")
        need(self.remember(self.current_ledger["path"], self.current_ledger["sha256"]) == self.current_ledger,
             "current old ledger differs from explicit observation")
        sdk = control["old_sdk"]
        need(set(sdk) == DIRECTORY and sdk["device"] == 68 and sdk["inode"] == 721974 and
             Path(sdk["path"]) == Path(control["old_report"]["path"]).parent / "sdk" and
             Path(self.current_ledger["path"]) == Path(sdk["path"]).parent / "payloads.jsonl",
             "exact current partial SDK directory and adjacent ledger required")
        self.source_parent = directory_fd(Path(sdk["path"]).parent); self.fds.append(self.source_parent)
        self.destination_parent = directory_fd(self.out); self.fds.append(self.destination_parent)
        self.sdkfd = os.open("sdk", os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=self.source_parent)
        self.fds.append(self.sdkfd)
        need(directory_pin(sdk["path"], os.fstat(self.sdkfd)) == sdk and
             directory_pin(sdk["path"], os.stat("sdk", dir_fd=self.source_parent, follow_symlinks=False)) == sdk and
             os.fstat(self.destination_parent).st_dev == sdk["device"], "held SDK identity/same-filesystem check failed")
        need(control["checkpoint"] == {"ledger_end": 6784738, "entries": 19690, "bytes": 414777798},
             "exact independently observed committed checkpoint required")
        last = self.document(control["last_retirement"])
        need(control["last_retirement"]["bytes"] == 3035 and control["last_retirement"]["sha256"] ==
             "b00548fbc513f739b511d0d2c0dfccc6440a54f09ed8ea5a8fca7e6b1ab31961" and
             last["payload_ledger_end_after_extract"] == 6784738 and
             last["SDK_payload_entries_seen"] == 19690 and last["SDK_payload_bytes_seen"] == 414777798,
             "last durable extraction checkpoint differs")
        self.report["continuation"] = {"kind": control["kind"], "old_report": control["old_report"],
            "old_outer_report": control["old_outer_report"], "historical_inner_status": old["status"],
            "historical_outer_status": outer["status"], "historical_reports_promoted": False,
            "original_sdk_status": "UNQUALIFIED_FAILED_PARTIAL_TREE", "current_ledger": self.current_ledger,
            "current_sdk_before_transfer": sdk, "fresh_signed_authority_required": True,
            "target_program_executed": False, "compatibility_qualified": False}

    def validate_cache_controls(self, cache_index, selected_packages):
        """Every retained complete DEB must match fresh signed bytes and fields.

        Equal full content may share one fresh dpkg-deb control observation;
        each physical file still has its own guarded full-stream current pin.
        No stale index or historical control log supplies package authority.
        """
        signed = {}
        for row in selected_packages:
            signed.setdefault((row["bytes"], row["sha256"]), []).append(row)
        observations = []; sequence = 0
        for key, pins in sorted(cache_index.items()):
            self.guard(); need(key in signed and pins, "retained DEB is outside the fresh signed solver closure")
            chosen = sorted(pins, key=lambda item: item["path"])[0]
            command = self.run("resume-cache-%05d-current-control" % sequence,
                               [self.tools["dpkg_deb"], "--field", chosen["path"]])
            fields = list(self.maintained.file_paragraphs(command["log"]["path"]))
            need(len(fields) == 1 and all(all(fields[0].get(k) == row[v] for k, v in
                 (("Package", "package"), ("Version", "version"), ("Architecture", "architecture")))
                 for row in signed[key]), "retained actual DEB control differs from fresh signed package")
            observations.append({"current_physical_debs": pins, "fresh_actual_control_log": command["log"],
                "fresh_signed_packages": signed[key], "scope": "CONTENT_EQUAL_DEBS_SHARE_ONLY_THIS_NEW_CONTROL_OBSERVATION"})
            self.control_logs[key] = command["log"]
            sequence += 1
        path = self.out / "retained-DEBs-fresh-signed-control-validation.json"
        self.common.atomic_json(path, {"kind": "fresh-signed-complete-retained-DEB-revalidation", "observations": observations,
            "historical_controls_used_as_authority": False, "executed_package_scripts": False})
        self.report["continuation"]["retained_DEBs"] = self.product(path, 16 * MIB)

    def retire_audit_tar(self, path, pin, ordinal, checkpoint):
        """Retire only a fresh transient read-only audit tar, with honest proof."""
        need(path == self.audit_dir / ("%05d.data.tar" % ordinal) and
             self.common.opened(path, self.limits["max_tar_bytes"]) == pin,
             "only unchanged fresh owned audit tar may retire")
        row = self.report["commands"][-1]
        need(row.get("stdout_data") == pin and row["status"] == "PASS" and row["returncode"] == 0,
             "fresh terminal audit decoder receipt required")
        proof_path = self.audit_dir / ("%05d-tar-audit.json" % ordinal)
        self.common.atomic_json(proof_path, {"kind": "authenticated-tar-read-only-ledger-reconciliation",
            "fresh_decoded_tar": pin, "actual_decoder_argv": row["argv"], "actual_same_pid_exec_argv": row["same_pid_exec_argv"],
            "actual_decoder_log": row["log"], "actual_decoder_returncode": 0,
            "checkpoint_after_audit": checkpoint, "SafeTree_extract_executed": False,
            "scope": "READ_ONLY_REPLAY_AND_FULL_PHYSICAL_TREE_CHECK_REQUIRED_SEPARATELY"})
        proof = self.product(proof_path, 16 * MIB)
        parent = directory_fd(self.audit_dir)
        try:
            info = os.stat(path.name, dir_fd=parent, follow_symlinks=False)
            need((info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns) ==
                 tuple(pin[k] for k in ("device", "inode", "bytes", "mtime_ns", "ctime_ns")), "audit tar name changed")
            os.unlink(path.name, dir_fd=parent); os.fsync(parent)
        finally:
            os.close(parent)
        need(self.products.pop(pin["path"], None) == pin, "audit tar product registry mismatch")
        del row["stdout_data"]
        row["stdout_data_historical"] = {"physical_path": pin["path"], **{k: v for k, v in pin.items() if k != "path"},
            "disposition": "RETIRED_OWNED_AFTER_READ_ONLY_AUTHENTICATED_LEDGER_AUDIT", "retained_proof": proof}
        row["stdout_data_retired_after_read_only_ledger_audit"] = True
        self.common.atomic_json(self.out / "commands.json", self.report["commands"])
        return proof

    def reconcile(self, ordered, cache_index):
        need(len(ordered) == 513 and self.original is not None, "fresh complete 513-package closure required before replay")
        self.validate_cache_controls(cache_index, ordered)
        virtual = VirtualTree(self.maintained, self.limits, self.guard)
        ledger = Ledger(self.current_ledger["path"], self.current_ledger, self.guard)
        prefix_rows = []; total_tar = 0; committed = None
        try:
            for ordinal in range(256):
                self.guard(); selected = ordered[ordinal]
                if ordinal < 255:
                    original = self.original["packages"][ordinal]
                    need(all(original[k] == selected[k] for k in IDENTITY), "fresh solver order/identity differs from committed prefix")
                    history = original["data_tar"]
                    need(history["disposition"] == "RETIRED_OWNED_AFTER_CHECKED_EXTRACT", "original prefix tar was not honestly retired")
                    need(Path(history["retained_proof"]["path"]) == Path(self.control["old_report"]["path"]).parent /
                         ("deb-%05d-checked-tar-retirement.json" % ordinal), "original retirement name differs from package ordinal")
                    proof, current_proof = self.historical(history["retained_proof"])
                    need(proof["kind"] == "checked-owned-decoded-tar-retirement" and
                         proof["checked_extract_returned"] is True and proof["actual_decoder_returncode"] == 0 and
                         all(proof["fresh_signed_package"][k] == selected[k] for k in IDENTITY), "genuine original checked extraction proof required")
                    expected_tar = proof["decoded_tar_before_retirement"]
                else:
                    # The original failed report contains actual successful
                    # ordinal-255 decoder/control commands, but no committed
                    # package row. Their tar is historical content evidence.
                    matching = [row for row in self.original["commands"] if row.get("label") == "deb-00255-data-tar"]
                    need(len(matching) == 1 and "stdout_data" in matching[0], "original partial decoder observation required")
                    expected_tar = matching[0]["stdout_data"]
                    current_proof = None
                candidates = cache_index.get((selected["bytes"], selected["sha256"]), [])
                need(candidates, "authenticated retained DEB missing for replay; refuse guessed partial contents")
                deb = sorted(candidates, key=lambda item: item["path"])[0]
                tar = self.audit_dir / ("%05d.data.tar" % ordinal)
                pin = self.decode("resume-%05d-authenticated-tar" % ordinal, tar, self.limits["max_tar_bytes"],
                                  [self.tools["dpkg_deb"], "--fsys-tarfile", deb["path"]])
                need((pin["bytes"], pin["sha256"]) == (expected_tar["bytes"], expected_tar["sha256"]),
                     "fresh signed DEB decoding differs from original tar content")
                # Include the partial-package audit too. Its later fresh
                # extraction will be counted again by the caller, so the
                # cumulative quota covers actual decoded output, not only
                # one logical tar per package.
                total_tar += pin["bytes"]
                need(total_tar <= self.limits["max_total_tar_bytes"], "replayed tar cumulative quota exceeded")
                virtual.audit(tar, selected["package"] + "=" + selected["version"], ledger, partial=ordinal == 255)
                checkpoint = virtual.checkpoint(ledger)
                if ordinal < 255:
                    need(proof["payload_ledger_end_after_extract"] == checkpoint["ledger_end"] and
                         proof["SDK_payload_entries_seen"] == checkpoint["entries"] and
                         proof["SDK_payload_bytes_seen"] == checkpoint["bytes"], "original retirement checkpoint differs from independent tar replay")
                    audit_proof = self.retire_audit_tar(tar, pin, ordinal, checkpoint)
                    prefix_rows.append({**selected, "deb": deb,
                        "actual_control_log": self.control_logs[(selected["bytes"], selected["sha256"])],
                        "data_tar": {"physical_path": pin["path"], **{k: v for k, v in pin.items() if k != "path"},
                            "disposition": "RETIRED_OWNED_AFTER_READ_ONLY_AUTHENTICATED_LEDGER_AUDIT", "retained_proof": audit_proof},
                        "authenticated_download_log": None, "byte_source": "CURRENT_RETAINED_DEB_FRESH_SIGNED_AND_CONTROL_VALIDATED",
                        "current_cache_revalidation": self.report["continuation"]["retained_DEBs"],
                        "extraction": "ORIGINAL_COMMITTED_PREFIX_INDEPENDENTLY_RECONCILED_NOT_REEXECUTED",
                        "original_compilation_or_extraction_provenance": {"original_package_record": original,
                            "original_retirement_current_pin": current_proof}})
                    if ordinal == 254:
                        committed = checkpoint
                        need({k: checkpoint[v] for k, v in (("ledger_end", "ledger_end"), ("entries", "entries"), ("bytes", "bytes"))} ==
                             self.control["checkpoint"], "independent final committed checkpoint differs from Root observation")
                else:
                    self.retire_audit_tar(tar, pin, ordinal, checkpoint)
                    need(ledger.next_row is None and ledger.offset == self.current_ledger["bytes"], "unaccounted old ledger suffix refused")
        finally:
            ledger.close()
        need(committed is not None, "committed checkpoint missing")
        first = virtual.verify_physical(self.sdkfd, self.out / "partial-SDK-authenticated-catalog-before.jsonl", self.guard)
        self.product(self.out / "partial-SDK-authenticated-catalog-before.jsonl", 64 * MIB)
        # Reopen the source ledger completely before committing any directory
        # transfer. New extraction will use only its committed byte prefix.
        need(self.remember(self.current_ledger["path"], self.current_ledger["sha256"]) == self.current_ledger,
             "old ledger changed before transfer")
        tree = self.prepare_tree(committed)
        try:
            second = virtual.verify_physical(self.sdkfd, self.out / "partial-SDK-authenticated-catalog-transfer.jsonl", self.guard)
            self.product(self.out / "partial-SDK-authenticated-catalog-transfer.jsonl", 64 * MIB)
            need(first == second, "partial SDK changed before atomic transfer")
            receipt = self.transfer(first, committed)
        except BaseException:
            tree.close(); raise
        self.report["continuation"].update({"authenticated_partial_tree": first, "checkpoint": committed,
            "partial_ordinal_255_state_carried_into_checkpoint": False, "committed_prefix_packages": 255,
            "fresh_reextract_ordinals": [255, 512], "requalification_evidence": self.audit_rows,
            "read_only_audit_decoded_tar_bytes": total_tar,
            "transfer_receipt": receipt, "old_ledger_preserved": True,
            "original_extract_status_promoted": False, "SDK_whole_tree_copied": False})
        return tree, prefix_rows, total_tar

    def prepare_tree(self, checkpoint):
        """Initialize only checkpoint state; do not alter maintained methods."""
        tree = self.maintained.SafeTree.__new__(self.maintained.SafeTree)
        tree.root, tree.limits, tree.guard = self.out / "sdk", self.limits, self.guard
        tree.rootfd = os.dup(self.sdkfd)
        path = self.out / "payloads.jsonl"
        tree.ledger = path.open("xb")
        try:
            source = os.open(self.current_ledger["path"], os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC)
            try:
                remaining = checkpoint["ledger_end"]; digest = hashlib.sha256()
                while remaining:
                    self.guard(); block = os.read(source, min(MIB, remaining))
                    need(block, "old committed ledger prefix truncated")
                    tree.ledger.write(block); digest.update(block); remaining -= len(block)
            finally:
                os.close(source)
            tree.ledger.flush(); os.fsync(tree.ledger.fileno())
            tree.entries, tree.bytes, tree.sequence = checkpoint["entries"], checkpoint["bytes"], checkpoint["sequence"]
            tree.pending = copy.deepcopy(checkpoint["pending"])
            self.report["continuation"]["new_ledger_initial_prefix"] = {
                "bytes": checkpoint["ledger_end"], "sha256": digest.hexdigest(),
                "scope": "ONLY_OLD_COMMITTED_LEDGER_BYTES_COPIED; OLD_LEDGER_UNCHANGED"}
            return tree
        except BaseException:
            tree.close(); raise

    def transfer(self, tree_proof, checkpoint):
        self.guard(); old = self.control["old_sdk"]
        def parents_still_named():
            for path, retained in ((Path(old["path"]).parent, self.source_parent), (self.out, self.destination_parent)):
                current = directory_fd(path)
                try:
                    first, second = os.fstat(retained), os.fstat(current)
                    need((first.st_dev, first.st_ino, first.st_mode) == (second.st_dev, second.st_ino, second.st_mode),
                         "held transfer parent no longer has its explicit canonical name")
                finally:
                    os.close(current)
        parents_still_named()
        need(directory_pin(old["path"], os.fstat(self.sdkfd)) == old and
             directory_pin(old["path"], os.stat("sdk", dir_fd=self.source_parent, follow_symlinks=False)) == old,
             "old SDK directory identity changed before transfer")
        try:
            os.stat("sdk", dir_fd=self.destination_parent, follow_symlinks=False)
        except FileNotFoundError:
            pass
        else:
            need(False, "fresh destination SDK name already exists")
        intent_path = self.out / "SDK-directory-transfer-intent.json"
        intent = {"kind": "partial-SDK-directory-only-transfer-intent", "source": old,
            "destination": str(self.out / "sdk"), "source_parent_identity": directory_pin(Path(old["path"]).parent, os.fstat(self.source_parent)),
            "destination_parent_identity": directory_pin(self.out, os.fstat(self.destination_parent)),
            "authenticated_tree": tree_proof, "committed_checkpoint": checkpoint,
            "rename": "renameat2_RENAME_NOREPLACE_SAME_FILESYSTEM", "automatic_unconfirmed_transfer_recovery": False,
            "old_reports_DEBs_logs_and_ledger_moved": False}
        self.common.atomic_json(intent_path, intent); os.fsync(self.destination_parent)
        intent_pin = self.product(intent_path, 16 * MIB)
        parents_still_named()
        # ctypes is the selected inventoried Python's stdlib interface to its
        # already loaded libc. No compiler, command, alternate library search
        # path or architecture-specific syscall number is introduced.
        libc = ctypes.CDLL(None, use_errno=True)
        rename = getattr(libc, "renameat2", None)
        need(rename is not None, "libc renameat2 required; no unsafe replace fallback")
        rename.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_uint]
        rename.restype = ctypes.c_int
        result = rename(self.source_parent, b"sdk", self.destination_parent, b"sdk", 1)
        if result:
            code = ctypes.get_errno()
            raise OSError(code, os.strerror(code), str(self.out / "sdk"))
        after = os.fstat(self.sdkfd); named = os.stat("sdk", dir_fd=self.destination_parent, follow_symlinks=False)
        need(facts(after) == facts(named) and (after.st_dev, after.st_ino, after.st_mode, after.st_mtime_ns) ==
             tuple(old[k] for k in ("device", "inode", "mode", "mtime_ns")), "atomic destination differs from held SDK")
        try:
            os.stat("sdk", dir_fd=self.source_parent, follow_symlinks=False)
        except FileNotFoundError:
            pass
        else:
            need(False, "source SDK still named after atomic transfer")
        os.fsync(self.source_parent); os.fsync(self.destination_parent)
        parents_still_named()
        receipt_path = self.out / "SDK-directory-transfer-confirmed.json"
        self.common.atomic_json(receipt_path, {"kind": "partial-SDK-directory-only-transfer-confirmed",
            "intent": intent_pin, "destination_actual": directory_pin(self.out / "sdk", after),
            "held_directory_identity_confirmed": True, "source_name_absent": True,
            "both_parent_directories_fsynced": True, "SDK_whole_tree_copied": False,
            "scope": "DIRECTORY_TRANSFER_ONLY; COMPLETE_R5_EXTRACTION_AND_FINAL_PRESERVATION_STILL_REQUIRED"})
        os.fsync(self.destination_parent)
        return self.product(receipt_path, 16 * MIB)

    def close(self):
        while self.fds:
            os.close(self.fds.pop())
