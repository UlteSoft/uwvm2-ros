"""Admission and byte provenance for the independent native preflight actors.

These helpers do not install dependencies, start a service, initialize DDS, or
choose a model. The Root coordinator owns complete input inventories, a fresh
read-only simulation view, deadlines/RSS limits and pidfd cleanup. Each actor
also reopens the exact frozen source, selected files and current cgroup before
loading native packages. A source-only review of this file is not a probe run.
"""
import hashlib
import os
from pathlib import Path
import stat
import sys
import types

from common import ROOT, child_guard, exact, load_common, read_json, require


def file_pin(path, maximum=256 * 1024**2):
    """Hash one stable regular descriptor with a bounded one-MiB buffer."""
    path = Path(path).resolve(strict=True)
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum,
                "preflight file exceeds its explicit regular-file byte bound")
        digest, count = hashlib.sha256(), 0
        while True:
            block = os.read(fd, min(1024**2, maximum - count + 1))
            if not block:
                break
            count += len(block)
            require(count <= maximum, "preflight file grew beyond its bound")
            digest.update(block)
        after, named = os.fstat(fd), path.stat(follow_symlinks=False)
        fields = ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")
        require(count == before.st_size and all(getattr(before, key) == getattr(after, key) ==
                getattr(named, key) for key in fields), "preflight file changed while opened")
        return {"path": str(path), "bytes": count, "sha256": digest.hexdigest(),
                "device": before.st_dev, "inode": before.st_ino,
                "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
    finally:
        os.close(fd)


def admit(args, script):
    """Reopen current controls and selected files before importing native code.

    This is an actor check, not a replacement for the coordinator's complete
    before/after catalogs or its owned-process ledger. Paths inside a chroot
    must be explicitly selected by the Root view and retain their source lock.
    """
    config, guard = child_guard(args)
    frozen, manifest_pin = read_json(args.control_manifest)
    require(manifest_pin["sha256"] == args.control_manifest_sha256,
            "preflight manifest changed after child admission")
    for name in ("tests/ros_unitree/" + script, "tests/ros_unitree/preflight_support.py"):
        pin = file_pin(ROOT / name, 1024**2)
        require(frozen.get(name) == {"bytes": pin["bytes"], "sha256": pin["sha256"]},
                "preflight source absent from the actual complete freeze: " + name)
    common = load_common(ROOT, frozen)
    roots = [Path(value).resolve(strict=True) for value in config["input_roots"]]
    require(roots and len(roots) <= 64 and len(set(roots)) == len(roots) and
            all(root.is_dir() and root != Path("/") for root in roots),
            "complete explicit non-root input directories required")
    pins = {}
    for label, selected in config["inputs"].items():
        exact(selected, ("path", "bytes", "sha256"), "preflight selected input")
        path = Path(selected["path"]).resolve(strict=True)
        require(any(path.is_relative_to(root) for root in [ROOT, *roots]),
                "preflight selected input is outside admitted source/input roots")
        pin = common.opened(path)
        require(pin["bytes"] == selected["bytes"] and pin["sha256"] == selected["sha256"],
                "preflight selected input bytes differ: " + label)
        pins[label] = pin
        guard()
    return config, guard, pins, roots


def preserve(pins, guard):
    """Compare all admitted selected files again before a terminal success."""
    for initial in pins.values():
        current = file_pin(initial["path"], max(initial["bytes"], 1))
        require(all(current[key] == initial[key] for key in current),
                "preflight selected file changed during native observation")
        guard()


def official_file(config, pins, upstream_label, path):
    """Bind an actual selected upstream file to the original acquisition proof.

    Root has already reopened the complete upstream tree/catalog. This actor
    additionally checks the exact original blob entry for the file it loads;
    it never infers a revision from a package name or a model directory.
    """
    require(upstream_label in config["upstreams"], "explicit official upstream selection required")
    selected = config["upstreams"][upstream_label]
    exact(selected, ("root", "repository", "commit", "lock_input"), "preflight upstream selection")
    root = Path(selected["root"]).resolve(strict=True)
    path = Path(path).resolve(strict=True)
    require(path.is_relative_to(root), "file is outside its selected official source tree")
    lock_input = selected["lock_input"]
    require(lock_input in pins, "actual upstream acquisition proof must be a pinned input")
    proof, pin = read_json(pins[lock_input]["path"])
    require(all(pin[key] == pins[lock_input][key] for key in pin), "upstream proof changed at actor admission")
    if upstream_label in proof and "status" not in proof:
        proof = proof[upstream_label]
    repository = selected["repository"].removeprefix("https://github.com/").removesuffix(".git")
    require(repository.startswith("unitreerobotics/") and proof.get("repository") == repository and
            proof.get("commit") == selected["commit"] and proof.get("root") == str(root) and
            proof.get("status") == "OFFICIAL_SNAPSHOT_GIT_TREE_AND_ALL_BLOBS_VERIFIED" and
            proof.get("code_executed") is False and type(proof.get("files")) is list,
            "actual official Git-tree/all-blob acquisition proof required")
    relative = str(path.relative_to(root))
    rows = [row for row in proof["files"] if row.get("path") == relative]
    require(len(rows) == 1 and rows[0].get("mode") in ("100644", "100755"),
            "one actual ordinary upstream Git blob required")
    physical = file_pin(path)
    require(all(rows[0].get(key) == physical[key] for key in ("bytes", "sha256")),
            "selected official file differs from its actual upstream blob")
    return {"upstream": upstream_label, "repository": repository, "commit": selected["commit"],
            "relative": relative, "git_blob": rows[0]["git_blob"], "file": physical}


def mapped_native_file(args, selected, guard):
    """Use the frozen ROS mapping helper on this actor's own loaded native DSO.

    The raw maps device is display-only. Actual kernel map_files FDs must match
    the selected physical file, its complete bytes and ELF header. No STOP or
    CONT is sent. This is a self-observation in an already Root-owned actor;
    the coordinator retains the process pidfd and its full cleanup ledger.
    """
    frozen, pin = read_json(args.control_manifest)
    require(pin["sha256"] == args.control_manifest_sha256, "mapping source manifest changed")
    configuration, configuration_pin = read_json(args.control_config)
    require(configuration_pin["sha256"] == args.control_config_sha256, "mapping control configuration changed")
    leaf = Path(configuration["cgroup"]["expected_leaf"])
    path = ROOT / "tests/ros_aarch64/runtime.py"
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(fd)
        require(stat.S_ISREG(first.st_mode) and 0 < first.st_size <= 1024**2, "bounded actual mapping source required")
        raw = os.read(fd, 1024**2 + 1)
        last, named = os.fstat(fd), path.stat(follow_symlinks=False)
        require(len(raw) == first.st_size and len(raw) <= 1024**2 and all(
                getattr(first, key) == getattr(last, key) == getattr(named, key) for key in
                ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")), "mapping helper changed while opened")
    finally:
        os.close(fd)
    require(frozen.get("tests/ros_aarch64/runtime.py") ==
            {"bytes": len(raw), "sha256": hashlib.sha256(raw).hexdigest()}, "mapping helper absent from actual freeze")
    helper = types.ModuleType("uwvm_unitree_preflight_opened_mapping")
    helper.__file__ = str(path)
    exec(compile(raw, str(path), "exec"), helper.__dict__)
    # The native view deliberately uses CPython3.10. Do not call the older
    # helper's elf() convenience routine, which uses Python3.11 file_digest.
    # Our selected file was already fully streamed and pinned before loading;
    # bind its actual first64 ELF bytes with the same descriptor metadata.
    target_fd = os.open(selected["path"], os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(target_fd)
        require(helper.mapping_file_facts(first) == {key: selected[key] for key in helper._MAP_FACTS},
                "native CRC selected file identity changed before ELF observation")
        header = os.read(target_fd, 64)
        require(helper.mapping_file_facts(os.fstat(target_fd)) == helper.mapping_file_facts(first) and
                helper.mapping_file_facts(Path(selected["path"]).stat(follow_symlinks=False)) == helper.mapping_file_facts(first),
                "native CRC file changed during actual ELF observation")
    finally:
        os.close(target_fd)
    require(len(header) >= 20 and header[:4] == b"\x7fELF" and header[4] in (1, 2) and header[5] in (1, 2),
            "actual selected native CRC ELF header required")
    wanted = dict(selected, elf_class=header[4], elf_data=header[5],
                  e_machine=int.from_bytes(header[18:20], "little" if header[5] == 1 else "big"))
    pid = os.getpid()
    def coherent():
        guard()
        proc = Path("/proc") / str(pid)
        first = (proc / "stat").read_text().rsplit(")", 1)[1].split()
        host = helper.mapping_file_facts((proc / "exe").stat())
        membership = (proc / "cgroup").read_text().splitlines()
        require(str(pid) in (leaf / "cgroup.procs").read_text().split(),
                "preflight own actor is absent from the selected original cgroup")
        last = (proc / "stat").read_text().rsplit(")", 1)[1].split()
        require(first[1] == last[1] and first[19] == last[19], "preflight own process birth changed")
        return {"pid": pid, "parent_pid": int(first[1]), "starttime_ticks": int(first[19]),
                "state": last[0], "membership": membership, "live_cgroup_procs": True,
                "host_identity": host}
    receipt = helper.opened_target_mappings(pid, {"native-crc": wanted}, coherent)
    require(receipt["targets"]["native-crc"], "selected official CRC DSO was not actually mapped")
    return receipt


def package_files(prefixes, roots):
    """Record actual imported package/extension files, not just version labels.

    Each physical module file must belong to a completely inventoried input
    root. The CRC actor separately proves its selected actual native mapping.
    This list itself does not claim that Python module names prove DSO mapping
    identity or that the outer view implements a complete mapping collector.
    """
    selected = {}
    for name, module in list(sys.modules.items()):
        if not any(name == prefix or name.startswith(prefix + ".") for prefix in prefixes):
            continue
        path = getattr(module, "__file__", None)
        if path is None:
            continue
        path = Path(path).resolve(strict=True)
        require(any(path.is_relative_to(root) for root in roots),
                "native preflight package loaded outside inventoried roots: " + name)
        selected.setdefault(str(path), []).append(name)
    require(len(selected) <= 512, "bounded actual package file inventory required")
    result, total = [], 0
    for path, names in sorted(selected.items()):
        pin = file_pin(path)
        total += pin["bytes"]
        require(total <= 512 * 1024**2, "preflight package provenance exceeds total bound")
        result.append({"modules": sorted(names), "file": pin})
    return result
