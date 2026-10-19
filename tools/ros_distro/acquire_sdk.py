#!/usr/bin/env python3
"""Authenticated APT SDK extraction for an explicitly selected arm64 or amd64 target.

All network/tool commands are guarded by the frozen distribution lane's
pidfd owner. Both native-host and cross-target SDKs use the same signed closure.
No command is invoked merely by importing this file.
An extracted package tree is not a configured OS, an executed ROS SDK, or a
compatibility PASS. No maintainer script, trigger, target binary, compiler or ROS
program is run. Trust roots, repository identities, package seeds and host tool
closure are explicit reviewed inputs; this file supplies no trusted key/hash.
"""
from __future__ import annotations

import argparse
import email.utils
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import posixpath
import pwd
import re
import resource
import select
import shlex
import stat
import sys
import tarfile
import time
import traceback
import types
import urllib.parse

sys.dont_write_bytecode = True
MIB = 1024 ** 2
GIB = 1024 ** 3
SHA = re.compile(r"[0-9a-f]{64}\Z")
TOKEN = re.compile(r"[A-Za-z0-9][A-Za-z0-9.+_-]*\Z")
PKG = re.compile(r"[a-z0-9][a-z0-9+.-]*(?::(?:arm64|amd64))?(?:=[^\s'\"\\;]+)?\Z")
HISTORICAL_SOURCE_ONLY = {"boxturtle", "cturtle", "diamondback", "electric", "fuerte"}
# This is a target selection table, not an assertion that repositories exist.
CODENAMES = {
    "noetic": "focal", "jazzy": "noble",
    "groovy": "precise", "hydro": "precise", "indigo": "trusty", "jade": "trusty",
    "kinetic": "xenial", "lunar": "xenial", "melodic": "bionic",
    "ardent": "xenial", "bouncy": "bionic", "crystal": "bionic", "dashing": "bionic",
    "eloquent": "bionic", "foxy": "focal", "galactic": "focal", "humble": "jammy",
    "iron": "jammy", "kilted": "noble", "lyrical": "resolute", "rolling": None,
}
FIRST_FOUR = {"humble", "jazzy", "kilted", "lyrical", "rolling"}
CG_KEYS = {"memory_max", "memory_swap_max", "expected_boot_id", "expected_leaf",
           "expected_device", "expected_inode", "expected_cpus"}


class Blocked(Exception):
    """A required reviewed input/repository/format is unavailable, not ROS FAIL."""


def need(value, message):
    if not value:
        raise ValueError(message)


def closed(value, names, message):
    need(type(value) is dict and set(value) == set(names), "closed " + message + " required")


def pairs(items):
    result = {}
    for key, value in items:
        need(key not in result, "duplicate JSON key: " + key)
        result[key] = value
    return result


def bounded_body(path, limit=16 * MIB):
    """Bootstrap one small input using opened bytes, before any frozen exec."""
    path = Path(path)
    need(path.is_absolute(), "absolute metadata path required")
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        need(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= limit, "bounded regular metadata required")
        pieces, size = [], 0
        while True:
            part = os.read(fd, min(65536, limit + 1 - size))
            if not part:
                break
            pieces.append(part); size += len(part)
            need(size <= limit, "metadata byte bound exceeded")
        after = os.fstat(fd); named = path.stat(follow_symlinks=False)
        need(size == before.st_size and all(getattr(before, k) == getattr(after, k) == getattr(named, k)
             for k in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")), "metadata changed")
        return b"".join(pieces)
    finally:
        os.close(fd)


def load_common(source, manifest, expected):
    raw = bounded_body(manifest)
    need(SHA.fullmatch(expected) and hashlib.sha256(raw).hexdigest() == expected, "actual frozen manifest SHA differs")
    entries = json.loads(raw, object_pairs_hook=pairs)
    need(type(entries) is dict and entries, "complete frozen manifest required")
    bodies = {}
    for relative in ("tools/ros_distro/common.py", "tests/ros_cgroup/probe.py"):
        body = bounded_body(source / relative, MIB)
        need(entries.get(relative) == {"bytes": len(body), "sha256": hashlib.sha256(body).hexdigest()},
             "frozen ownership/probe source not admitted: " + relative)
        bodies[relative] = body
    module = types.ModuleType("uwvm_authenticated_apt_frozen_common")
    module.__file__ = str(source / "tools/ros_distro/common.py")
    sys.modules[module.__name__] = module
    exec(compile(bodies["tools/ros_distro/common.py"], module.__file__, "exec"), module.__dict__)
    return module


def within(path, roots):
    return any(path == root or root in path.parents for root in roots)


def absolute(value, directory=False):
    need(type(value) is str and Path(value).is_absolute() and not any(c in value for c in "\0\r\n"),
         "explicit absolute input path required")
    path = Path(value).resolve(strict=True)
    need(path.is_dir() if directory else path.is_file(), "input kind differs: " + value)
    return path


def alias_binding(value):
    """Retain selected literal aliases as well as the physical opened bytes."""
    path = Path(value); need(path.is_absolute(), "absolute alias selection required")
    prefix = Path(path.anchor); links = []
    for component in path.parts[1:]:
        prefix /= component
        if not prefix.is_symlink():
            continue
        first = prefix.lstat(); target = os.readlink(prefix); resolved = prefix.resolve(strict=True)
        last = prefix.lstat()
        need(all(getattr(first, k) == getattr(last, k) for k in
             ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode")) and os.readlink(prefix) == target,
             "selected alias changed while observed")
        links.append({"path": str(prefix), "target": target, "resolved": str(resolved), "device": first.st_dev,
            "inode": first.st_ino, "bytes": first.st_size, "mtime_ns": first.st_mtime_ns, "ctime_ns": first.st_ctime_ns})
    return {"selection": str(path), "resolved": str(path.resolve(strict=True)), "links": links}


def safe_url(value):
    need(type(value) is str and not any(c in value for c in "\0\r\n\t '"), "single literal repository URL required")
    parsed = urllib.parse.urlsplit(value)
    need(parsed.scheme in ("https", "http") and parsed.hostname and not parsed.username and not parsed.password
         and not parsed.query and not parsed.fragment and ".." not in PurePosixPath(parsed.path).parts,
         "reviewed HTTP(S) repository URL without credentials/query/traversal required")
    return value.rstrip("/") + "/"


def validate_spec(spec):
    def missing(value, where="spec"):
        if type(value) is str and value.startswith("REPLACE_WITH_"):
            raise Blocked("MISSING_REVIEWED_PREREQUISITE: " + where)
        if type(value) is dict:
            for key, item in value.items():
                missing(item, where + "." + key)
        elif type(value) is list:
            for index, item in enumerate(value):
                missing(item, where + "[%d]" % index)
    missing(spec)
    closed(spec, ("format_version", "kind", "target", "cgroup", "tools", "input_roots", "input_files",
                  "tls_ca", "distribution_lock", "repositories", "seeds", "seed_review", "limits"), "acquisition spec")
    need(type(spec["format_version"]) is int and spec["format_version"] == 1 and
         spec["kind"] == "authenticated-apt-sdk-inputs", "spec kind/version differs")
    closed(spec["target"], ("backend", "distribution", "ubuntu_codename", "architecture", "route"), "target")
    target = spec["target"]; distro = target["distribution"]
    need(target["architecture"] in ("arm64", "amd64") and type(target["backend"]) is int and target["backend"] in (1, 2), "explicit supported target architecture required")
    if distro in HISTORICAL_SOURCE_ONLY:
        raise Blocked("PRE_GROOVY_REQUIRES_SEPARATELY_LOCKED_SOURCE_BACKPORT_NOT_THIS_APT_PRODUCER")
    if distro not in CODENAMES:
        raise Blocked("NOT_A_REMAINING_REVIEWED_APT_TARGET")
    if target["route"] != "authenticated-apt-extraction":
        raise Blocked("SOURCE_OR_UNAVAILABLE_ARCHIVE_REQUIRES_ANOTHER_REVIEWED_PRODUCER")
    need(TOKEN.fullmatch(target["ubuntu_codename"]) and
         (CODENAMES[distro] is None or target["ubuntu_codename"] == CODENAMES[distro]), "distro/OS target differs from reviewed selection")
    need((target["backend"] == 2) == (distro in {"ardent", "bouncy", "crystal", "dashing", "eloquent", "foxy",
         "galactic", "humble", "iron", "jazzy", "kilted", "lyrical", "rolling"}), "backend label differs")
    closed(spec["cgroup"], CG_KEYS, "current original cgroup")
    need(spec["cgroup"]["memory_max"] == 68719476736 and spec["cgroup"]["memory_swap_max"] == 0,
         "current original64GiB/swap0 required")
    need(all(type(spec["cgroup"][key]) is int for key in ("memory_max", "memory_swap_max", "expected_device", "expected_inode")) and
         spec["cgroup"]["expected_device"] > 0 and spec["cgroup"]["expected_inode"] > 0 and
         type(spec["cgroup"]["expected_boot_id"]) is str and type(spec["cgroup"]["expected_leaf"]) is str and
         spec["cgroup"]["expected_leaf"].startswith("/sys/fs/cgroup/") and type(spec["cgroup"]["expected_cpus"]) is str,
         "exact typed current original cgroup incarnation required")
    closed(spec["tools"], ("apt_get", "apt_config", "apt_helper", "apt_methods", "dpkg", "dpkg_deb", "gpgv", "python"), "host tools")
    for name, row in spec["tools"].items():
        if name == "apt_methods":
            absolute(row, directory=True)
        else:
            closed(row, ("path", "sha256"), "host tool pin")
            need(SHA.fullmatch(row["sha256"]), "reviewed tool SHA required")
            need(os.access(absolute(row["path"]), os.X_OK), "host tool not executable")
    need(type(spec["input_roots"]) is list and 1 <= len(spec["input_roots"]) <= 32,
         "explicit complete host APT/Python/OS/TLS tool roots required")
    roots = [absolute(value, True) for value in spec["input_roots"]]
    need(len(set(roots)) == len(roots) and Path("/") not in roots, "distinct non-root inventory roots required")
    for row in spec["input_files"] + [spec["tls_ca"], spec["distribution_lock"], spec["seed_review"]]:
        closed(row, ("path", "sha256"), "selected metadata pin")
        need(SHA.fullmatch(row["sha256"]), "reviewed metadata SHA required")
        absolute(row["path"])
    need(type(spec["input_files"]) is list and len(spec["input_files"]) <= 128, "bounded selected host config file list required")
    need(type(spec["seeds"]) is list and 1 <= len(spec["seeds"]) <= 256 and
         len(set(spec["seeds"])) == len(spec["seeds"]) and all(type(p) is str and PKG.fullmatch(p) for p in spec["seeds"]),
         "reviewed explicit package seed names/optional versions required")
    architecture = target["architecture"]
    for seed in spec["seeds"]:
        name = seed.split("=", 1)[0]
        need(":" not in name or name.rsplit(":", 1)[1] == architecture, "seed architecture differs from explicit target")
    need(type(spec["repositories"]) is list and 2 <= len(spec["repositories"]) <= 16, "Ubuntu and ROS signed repositories required")
    need({row["role"] for row in spec["repositories"]} == {"ubuntu", "ros"}, "both Ubuntu and ROS sources required")
    names = set()
    for repo in spec["repositories"]:
        closed(repo, ("name", "role", "uri", "suite", "components", "keyring", "fingerprints",
                      "expected_inrelease_sha256", "release_fields", "expiry"), "signed repository")
        need(TOKEN.fullmatch(repo["name"]) and repo["name"] not in names and TOKEN.fullmatch(repo["suite"]), "distinct safe repository names/suites required")
        names.add(repo["name"]); safe_url(repo["uri"])
        need(type(repo["components"]) is list and repo["components"] and
             all(TOKEN.fullmatch(x) for x in repo["components"]) and len(set(repo["components"])) == len(repo["components"]), "explicit distinct components required")
        closed(repo["keyring"], ("path", "sha256"), "repository keyring")
        absolute(repo["keyring"]["path"]); need(SHA.fullmatch(repo["keyring"]["sha256"]), "reviewed keyring SHA required")
        need(type(repo["fingerprints"]) is list and repo["fingerprints"] and all(type(x) is str and
             re.fullmatch(r"[0-9A-F]{40}|[0-9A-F]{64}", x) for x in repo["fingerprints"]), "full reviewed primary signing fingerprints required")
        expected = repo["expected_inrelease_sha256"]
        need(expected is None or SHA.fullmatch(expected), "optional immutable signed Release SHA invalid")
        if distro == "rolling":
            need(expected is not None, "Rolling requires independently reviewed exact signed Release SHA for EVERY source")
        elif distro not in FIRST_FOUR:
            need(expected is not None, "historical/EOL sources require independently reviewed exact signed Release SHA for EVERY source")
        fields = repo["release_fields"]
        need(type(fields) is dict and {"Origin", "Suite", "Codename"} <= set(fields) <=
             {"Origin", "Label", "Suite", "Codename", "Version"} and all(type(v) is str and v and
             not any(c in v for c in "\0\r\n") for v in fields.values()), "explicit signed Release identities required")
        closed(repo["expiry"], ("mode", "review"), "Release expiry policy")
        need(repo["expiry"]["mode"] in ("require", "allow-missing", "pinned-expired"), "unknown expiry policy")
        if repo["expiry"]["mode"] == "pinned-expired":
            need(expected is not None and type(repo["expiry"]["review"]) is dict, "expired archive exception needs exact signed Release + reviewed physical reason")
            closed(repo["expiry"]["review"], ("path", "sha256"), "expiry review")
            absolute(repo["expiry"]["review"]["path"])
            need(SHA.fullmatch(repo["expiry"]["review"]["sha256"]), "expiry review SHA required")
        else:
            need(repo["expiry"]["review"] is None, "expiry exception only for exact pinned-expired source")
    closed(spec["limits"], ("command_seconds", "overall_seconds", "rss_bytes", "log_bytes", "min_free_bytes",
          "max_packages", "max_deb_bytes", "max_tar_bytes", "max_total_tar_bytes", "max_index_bytes", "max_total_download_bytes",
          "max_sdk_bytes", "max_entries", "max_file_bytes", "max_pending_hardlinks"), "finite acquisition limits")
    limits = spec["limits"]
    need(all(type(v) is int and v > 0 for v in limits.values()), "positive integer bounds required")
    need(limits["command_seconds"] <= 3600 and limits["overall_seconds"] <= 86400 and
         limits["rss_bytes"] <= 2 * GIB and limits["log_bytes"] <= 32 * MIB and limits["min_free_bytes"] >= 4 * GIB and
         limits["max_packages"] <= 10000 and limits["max_entries"] <= 2000000 and limits["max_pending_hardlinks"] <= 65536 and
         max(limits[k] for k in ("max_deb_bytes", "max_tar_bytes", "max_index_bytes", "max_file_bytes")) <= 2 * GIB,
         "explicit serial/low-memory/file bounds exceeded")
    return roots


def paragraphs(stream, line_limit=MIB, paragraph_limit=8 * MIB):
    """Streaming Debian control format, including continuation lines, no eval."""
    row, total, last = {}, 0, None
    while True:
        raw = stream.readline(line_limit + 1)
        if not raw:
            break
        need(len(raw) <= line_limit, "control line limit exceeded")
        total += len(raw); need(total <= paragraph_limit, "control paragraph limit exceeded")
        line = raw.decode("utf-8").rstrip("\r\n")
        if not line:
            if row:
                yield row
            row, total, last = {}, 0, None
        elif line[0].isspace():
            need(last is not None, "orphan control continuation")
            row[last] += "\n" + line[1:]
        else:
            key, separator, value = line.partition(":")
            need(separator and key not in row and re.fullmatch(r"[A-Za-z0-9-]+", key), "invalid/duplicate control field")
            row[key] = value.lstrip(); last = key
    if row:
        yield row


def file_paragraphs(path):
    with Path(path).open("rb") as stream:
        yield from paragraphs(stream)


def print_uris(path):
    """Read APT's own quoted URL/filename/size plan, never shell-evaluate it."""
    result = []
    for line in Path(path).read_text(encoding="utf-8").splitlines():
        if not line.startswith("'"):
            continue
        values = shlex.split(line)
        need(3 <= len(values) <= 4 and values[2].isdigit(), "unexpected actual APT print-URI form")
        filename = values[1]
        need(filename and Path(filename).name == filename and filename not in (".", ".."), "APT target filename escapes owned directory")
        result.append({"uri": values[0], "filename": filename, "bytes": int(values[2]),
                       "apt_printed_hash": values[3] if len(values) == 4 else None})
    return result


def uri_identity(value):
    """Compare APT escaping with the signed literal Filename, not another host."""
    parsed = urllib.parse.urlsplit(value)
    need(parsed.scheme in ("http", "https") and parsed.hostname and not parsed.username and
         not parsed.password and not parsed.query and not parsed.fragment, "literal package URI required")
    path = urllib.parse.unquote(parsed.path, errors="strict")
    need(".." not in PurePosixPath(path).parts and not any(c in path for c in "\0\r\n"), "unsafe encoded package URI")
    return urllib.parse.urlunsplit((parsed.scheme, parsed.netloc, path, "", ""))


def release_checks(path, repo, architecture):
    rows = list(file_paragraphs(path)); need(len(rows) == 1, "single signed Release body required")
    release = rows[0]
    need(all(release.get(k) == v for k, v in repo["release_fields"].items()), "authenticated Release identity differs")
    need(architecture in ("arm64", "amd64") and architecture in release.get("Architectures", "").split(), "signed source lacks selected architecture")
    now = time.time()
    date = email.utils.parsedate_to_datetime(release["Date"]).timestamp()
    need(date <= now + 300, "signed Release Date is in the future")
    until = release.get("Valid-Until"); policy = repo["expiry"]["mode"]
    if until is None:
        need(policy != "require", "signed Release has no required Valid-Until")
    else:
        need(email.utils.parsedate_to_datetime(until).timestamp() >= now or policy == "pinned-expired",
             "signed Release is expired without explicit hash-bound archive exception")
    hashes = {}
    for line in release.get("SHA256", "").splitlines():
        if not line.strip():
            continue
        fields = line.split(); need(len(fields) == 3 and SHA.fullmatch(fields[0]) and fields[1].isdigit(), "invalid signed SHA256 entry")
        relative = PurePosixPath(fields[2])
        need(not relative.is_absolute() and ".." not in relative.parts and fields[2] not in hashes, "unsafe/duplicate signed index path")
        hashes[fields[2]] = {"sha256": fields[0], "bytes": int(fields[1])}
    need(hashes, "strong signed index hashes required")
    return release, hashes


def apt_text(value):
    need(type(value) is str and not any(c in value for c in '\0\r\n"\\'), "safe explicit APT configuration value required")
    return '"' + value + '"'


def effective_apt_configuration(lines, expected):
    """Verify the owned controls, accepting only APT's actual key/list spelling.

    APT looks up identifiers case-insensitively and apt-config canonicalizes
    list-valued Languages and Architectures before dumping. Values themselves
    remain byte-exact: this does not accept alternate Boolean spellings, paths,
    extra architectures/languages or configured hooks. Other built-in defaults
    are retained in the original opened log, not promoted to user controls.
    See upstream cmdline/apt-config.cc and Configuration::Lookup/Dump.
    """
    need(type(lines) is list and 0 < len(lines) <= 16384, "bounded nonempty APT dump required")
    observed = {}
    for line in lines:
        need(type(line) is str and len(line) <= 65536, "bounded APT dump record required")
        match = re.fullmatch(r'([A-Za-z0-9_.:+-]+) "([^\r\n]*)";', line)
        need(match is not None, "unrecognized effective APT dump record")
        name, value = match.groups()
        # Only ASCII option identifiers are lowercased. Dump values are raw
        # strings in APT's default %v format; no shell/JSON escape evaluation.
        observed.setdefault(name.lower(), []).append(value)
    controls, spellings = {}, {}
    for name, value in expected.items():
        apt_text(value)  # Retain the original explicit-value restrictions.
        key = name.lower()
        need(key not in controls, "duplicate case-equivalent bootstrap control")
        controls[key] = value; spellings[key] = name
    need(controls.get("acquire::languages") == "none", "owned translation policy must be none")
    language_keys = {key for key in observed if key == "acquire::languages" or key.startswith("acquire::languages::")}
    scalar_none = language_keys == {"acquire::languages"} and observed["acquire::languages"] == ["none"]
    list_none = language_keys == {"acquire::languages", "acquire::languages::"} and \
        observed["acquire::languages"] == [""] and observed["acquire::languages::"] == ["none"]
    need(scalar_none or list_none, "effective APT Languages must disable exactly all translations")
    for key, value in controls.items():
        if key == "acquire::languages":
            continue
        actual = observed.get(key, [])
        need(actual == [value], "effective APT scalar differs: " + spellings[key] +
             " expected=" + repr(value) + " observed=" + repr(actual))
    architecture = controls.get("apt::architecture")
    need(architecture in ("arm64", "amd64"), "unsupported effective target architecture")
    architecture_keys = {key for key in observed if key == "apt::architectures" or key.startswith("apt::architectures::")}
    need(architecture_keys == {"apt::architectures", "apt::architectures::"} and
         observed["apt::architectures"] == [""] and observed["apt::architectures::"] == [architecture],
         "effective APT architecture list differs")
    hooks = ("dpkg::pre-invoke", "dpkg::post-invoke", "dpkg::pre-install-pkgs",
             "apt::update::pre-invoke", "apt::update::post-invoke", "apt::update::post-invoke-success")
    for key, values in observed.items():
        if key in hooks:
            need(values == [""], "APT hook scalar must be empty")
        need(not any(key.startswith(hook + "::") for hook in hooks), "APT hook arrays must be empty")
    return {"identifier_comparison": "ASCII_CASE_INSENSITIVE_VALUES_EXACT",
            "languages_form": "EMPTY_PARENT_SINGLE_NONE_CHILD" if list_none else "SINGLE_NONE_SCALAR",
            "controlled_scalars": {spellings[key]: observed[key] for key in sorted(controls)},
            "architecture": [architecture], "hooks": "NO_NONEMPTY_SCALARS_OR_ARRAY_ENTRIES",
            "record_count": len(lines)}


def write_bytes(path, body):
    with Path(path).open("xb") as stream:
        stream.write(body); stream.flush(); os.fsync(stream.fileno())


def emit_stdout(args):
    """Internal same-PID exec: bounded data output, stderr stays the guarded log.

    No shell, grandchild, target code or installation occurs. RLIMIT_FSIZE
    bounds decompressed index/tar bytes independently of the diagnostic log.
    The outer runner retains this Popen/pidfd before exec and tracks the same
    birth. The external program and full argument list remain in commands.
    """
    need(len(args) >= 4 and args[1].isdigit(), "internal data-output arguments invalid")
    output = Path(args[0]); limit = int(args[1]); program = Path(args[2])
    need(output.is_absolute() and program.is_absolute() and 0 < limit <= 2 * GIB, "bounded explicit data-output selection required")
    fd = os.open(output, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, 0o600)
    resource.setrlimit(resource.RLIMIT_FSIZE, (limit, limit))
    os.dup2(fd, 1)
    if fd != 1:
        os.close(fd)
    os.execv(str(program), [str(program), *args[3:]])


def preflight_tar(path, guard):
    """Bound extension metadata before tarfile can allocate any PAX body.

    APT authenticates package bytes, but that does not make a multi-gigabyte
    PAX/long-name allocation appropriate. This finite SDK extractor accepts
    ordinary ustar/GNU names and <=64KiB PAX metadata, not sparse-file extensions
    or PAX size overrides. All file bodies are still extracted as bounded data.
    """
    allowed = {tarfile.REGTYPE, tarfile.AREGTYPE, tarfile.DIRTYPE, tarfile.SYMTYPE, tarfile.LNKTYPE,
               tarfile.XHDTYPE, tarfile.XGLTYPE, tarfile.GNUTYPE_LONGNAME, tarfile.GNUTYPE_LONGLINK}
    with Path(path).open("rb") as stream:
        total = os.fstat(stream.fileno()).st_size
        while True:
            guard(); start = stream.tell(); header = stream.read(512)
            need(len(header) == 512, "complete tar header/end marker required")
            if header == b"\0" * 512:
                need(stream.read(512) == b"\0" * 512, "complete tar end marker required")
                while True:
                    tail = stream.read(MIB)
                    if not tail:
                        return
                    need(not any(tail), "nonzero trailing tar data refused")
            info = tarfile.TarInfo.frombuf(header, "utf-8", "strict")
            need(info.type in allowed and 0 <= info.size <= total - start - 512,
                 "unsupported tar type/length before extraction")
            if info.type in (tarfile.XHDTYPE, tarfile.XGLTYPE, tarfile.GNUTYPE_LONGNAME, tarfile.GNUTYPE_LONGLINK):
                need(info.size <= 65536, "tar extension metadata bound exceeded")
                value = stream.read(info.size)
                if info.type in (tarfile.XHDTYPE, tarfile.XGLTYPE):
                    offset = 0
                    while offset < len(value):
                        space = value.find(b" ", offset)
                        need(space > offset and value[offset:space].isdigit(), "bounded PAX record length required")
                        end = offset + int(value[offset:space]); need(space < end <= len(value), "PAX record escaped metadata")
                        record = value[space + 1:end]
                        key, separator, _ = record.partition(b"=")
                        need(separator and record.endswith(b"\n") and key != b"size" and not key.startswith(b"GNU.sparse"),
                             "sparse/PAX size override refused by finite extraction profile")
                        offset = end
            stream.seek(start + 512 + ((info.size + 511) // 512) * 512)
            need(stream.tell() <= total, "tar entry exceeds retained archive")


class SafeTree:
    """Single-writer, dirfd/NOFOLLOW package overlay confined to a fresh tree.

    Original absolute symlink targets are recorded but represented by relative
    links into this SDK root, never the host '/'. No chmod/chown/postinst script
    from a package is executed. UID/GID/set-id bits are metadata only. Changed
    duplicate files/type replacement and special nodes are refused instead of
    emulating dpkg maintainer scripts/Replaces/alternatives. Such a package set
    cannot qualify under this finite extraction profile.
    """
    def __init__(self, root, ledger, limits, guard):
        self.root, self.limits, self.guard = root, limits, guard
        self.rootfd = os.open(root, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        self.ledger = Path(ledger).open("xb")
        self.entries = self.bytes = self.sequence = 0
        self.pending = []

    def path_parts(self, name):
        need(type(name) is str and "\0" not in name and not name.startswith("/") and len(name.encode()) <= 4096,
             "absolute/NUL/overlong payload member refused")
        parts = [p for p in name.split("/") if p not in ("", ".")]
        need(".." not in parts and len(parts) <= 128 and all(len(p.encode()) <= 255 for p in parts), "payload path traversal/size refused")
        return parts

    def parent(self, parts):
        """Follow only SDK-relative existing aliases, with no host path lookup."""
        todo, stack, seen = list(parts), [], 0
        fd = os.dup(self.rootfd)
        try:
            while todo:
                part = todo.pop(0)
                if part in ("", "."):
                    continue
                if part == "..":
                    need(stack, "payload alias escapes SDK root")
                    stack.pop(); os.close(fd); fd = os.dup(self.rootfd)
                    todo = stack + todo; stack = []; continue
                try:
                    info = os.stat(part, dir_fd=fd, follow_symlinks=False)
                except FileNotFoundError:
                    os.mkdir(part, 0o755, dir_fd=fd)
                    info = os.stat(part, dir_fd=fd, follow_symlinks=False)
                if stat.S_ISLNK(info.st_mode):
                    seen += 1; need(seen <= 40, "SDK symlink loop/depth refused")
                    target = os.readlink(part, dir_fd=fd)
                    need(not target.startswith("/"), "physical absolute SDK alias refused")
                    todo = target.split("/") + todo; continue
                need(stat.S_ISDIR(info.st_mode), "payload parent is not a confined directory")
                other = os.open(part, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=fd)
                os.close(fd); fd = other; stack.append(part)
            return fd, stack
        except BaseException:
            os.close(fd); raise

    def record(self, value):
        self.ledger.write((json.dumps(value, sort_keys=True, separators=(",", ":")) + "\n").encode())

    def extract(self, path, package):
        local = 0
        preflight_tar(path, self.guard)
        with Path(path).open("rb") as body, tarfile.open(fileobj=body, mode="r|") as archive:
            for member in archive:
                self.guard(); self.entries += 1
                need(self.entries <= self.limits["max_entries"], "SDK entry count limit exceeded")
                parts = self.path_parts(member.name)
                if not parts:
                    need(member.isdir(), "only a directory may describe package root"); archive.members.clear(); continue
                row = {"package": package, "member": member.name, "original_mode": member.mode,
                       "uid": member.uid, "gid": member.gid, "mtime": member.mtime,
                       "owner_and_setid_preserved": False}
                fd, actual = self.parent(parts[:-1]); name = parts[-1]
                row["physical_relative"] = "/".join(actual + [name])
                try:
                    try:
                        old = os.stat(name, dir_fd=fd, follow_symlinks=False)
                    except FileNotFoundError:
                        old = None
                    if member.isdir():
                        if old is None:
                            os.mkdir(name, 0o755, dir_fd=fd)
                        elif stat.S_ISLNK(old.st_mode):
                            resolved, _ = self.parent(parts); os.close(resolved)
                        else:
                            need(stat.S_ISDIR(old.st_mode), "directory overlay changes existing type")
                        row["kind"] = "directory"
                    elif member.issym():
                        target = member.linkname
                        need(target and "\0" not in target and len(target.encode()) <= 4096, "bounded symlink target required")
                        logical = posixpath.normpath(target.lstrip("/") if target.startswith("/") else posixpath.join(*parts[:-1], target))
                        need(logical != ".." and not logical.startswith("../"), "logical SDK symlink escapes root")
                        # Compute relative from the actual confined parent: an
                        # earlier usr-merge alias may have redirected it.
                        rewritten = posixpath.relpath(logical, "/".join(actual) or ".")
                        if old is None:
                            os.symlink(rewritten, name, dir_fd=fd)
                        else:
                            need(stat.S_ISLNK(old.st_mode) and os.readlink(name, dir_fd=fd) == rewritten, "conflicting package symlink overlay")
                        row.update(kind="symlink", original_target=target, physical_target=rewritten)
                    elif member.islnk():
                        target = self.path_parts(member.linkname)
                        need(target, "nonempty confined hardlink target required")
                        need(old is None, "hardlink overlay cannot replace an existing path")
                        need(len(self.pending) < self.limits["max_pending_hardlinks"], "pending hardlink memory bound exceeded")
                        self.pending.append((parts, target, dict(row)))
                        row.update(kind="hardlink-pending", original_target=member.linkname)
                    elif member.isfile():
                        need(0 <= member.size <= self.limits["max_file_bytes"], "payload regular file bound exceeded")
                        local += member.size; self.bytes += member.size
                        need(local <= self.limits["max_tar_bytes"] and self.bytes <= self.limits["max_sdk_bytes"], "SDK expanded byte limit exceeded")
                        self.sequence += 1; temporary = ".uwvm-acquire-%016x.pending" % self.sequence
                        sink = os.open(temporary, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, 0o600, dir_fd=fd)
                        digest = hashlib.sha256(); count = 0
                        try:
                            source = archive.extractfile(member); need(source is not None, "regular tar data missing")
                            while True:
                                piece = source.read(MIB)
                                if not piece:
                                    break
                                count += len(piece); need(count <= member.size, "tar member exceeded declared length")
                                digest.update(piece)
                                offset = 0
                                while offset < len(piece):
                                    written = os.write(sink, piece[offset:]); need(written > 0, "short payload write")
                                    offset += written
                            need(count == member.size, "truncated regular tar member")
                            os.fchmod(sink, member.mode & 0o777); os.fsync(sink)
                        finally:
                            os.close(sink)
                        if old is None:
                            os.link(temporary, name, src_dir_fd=fd, dst_dir_fd=fd, follow_symlinks=False)
                        else:
                            need(stat.S_ISREG(old.st_mode), "regular file overlay changes existing type")
                            other = os.open(name, os.O_RDONLY | os.O_NOFOLLOW, dir_fd=fd)
                            try:
                                previous = hashlib.sha256()
                                while True:
                                    piece = os.read(other, MIB)
                                    if not piece:
                                        break
                                    previous.update(piece)
                            finally:
                                os.close(other)
                            need(previous.hexdigest() == digest.hexdigest() and old.st_size == count and
                                 (old.st_mode & 0o777) == (member.mode & 0o777), "conflicting package regular-file overlay")
                        os.unlink(temporary, dir_fd=fd)
                        row.update(kind="file", bytes=count, sha256=digest.hexdigest(), mode=member.mode & 0o777)
                    else:
                        raise Blocked("SPECIAL_TAR_NODE_OR_UNSUPPORTED_PAYLOAD_TYPE: " + member.name)
                    self.record(row)
                finally:
                    os.close(fd)
                    # Stream-mode TarFile on older Python still caches member
                    # objects. We never use its path lookup/extract helpers;
                    # drop the consumed cache rather than retain an SDK-sized
                    # dictionary in the metadata coordinator.
                    archive.members.clear()

    def finish(self):
        for parts, target, row in self.pending:
            self.guard()
            source, _ = self.parent(target[:-1]); dest, _ = self.parent(parts[:-1])
            try:
                info = os.stat(target[-1], dir_fd=source, follow_symlinks=False)
                need(stat.S_ISREG(info.st_mode), "hardlink source must be an actual regular SDK file")
                os.link(target[-1], parts[-1], src_dir_fd=source, dst_dir_fd=dest, follow_symlinks=False)
                self.record({**row, "kind": "hardlink-resolved", "original_target": "/".join(target)})
            finally:
                os.close(source); os.close(dest)
        self.ledger.flush(); os.fsync(self.ledger.fileno())

    def close(self):
        self.ledger.close(); os.close(self.rootfd)


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for field in ("source", "manifest", "spec", "out"):
        parser.add_argument("--" + field, type=Path, required=True)
    parser.add_argument("--manifest-sha256", required=True)
    parser.add_argument("--spec-sha256", required=True)
    args = parser.parse_args()
    need(sys.platform == "linux" and all(p.is_absolute() for p in (args.source, args.manifest, args.spec, args.out)), "explicit Linux absolute selections required")
    source = args.source.resolve(strict=True); manifest = args.manifest.resolve(strict=True)
    common = load_common(source, manifest, args.manifest_sha256)
    common.install_interrupts()
    spec, spec_pin = common.document(args.spec.resolve(strict=True))
    need(SHA.fullmatch(args.spec_sha256) and spec_pin["sha256"] == args.spec_sha256, "reviewed actual spec SHA differs")
    out = args.out
    need(out.parent.resolve(strict=True) == out.parent and not out.exists() and not out.is_symlink(), "fresh canonical output required")
    out.mkdir(mode=0o700)
    report = {"format_version": 1, "kind": "authenticated-apt-sdk-acquisition", "status": "RUNNING",
        "source": str(source), "source_manifest_sha256": args.manifest_sha256, "spec": spec_pin,
        "target": spec.get("target"), "commands": [], "repositories": [], "packages": [], "jobs": 1,
        "scope": "AUTHENTICATED_EXTRACTED_SYSROOT_NO_POSTINST", "installation_executed": False,
        "compiler_executed": False, "target_program_executed": False, "compatibility_qualified": False,
        "Guest_authority": "UNCHANGED_EMPTY", "cleanup": {"status": "NOT_PERFORMED"}}
    runner = tree = before_cg = None
    inputs, products, roots_before = {}, {}, []
    roots = []; source_before = None; aliases_before = {}; started = time.monotonic(); phase = "prerequisites"
    active_data = None
    controls = {}
    empty_control_dirs = []

    def remember(path, expected=None):
        pin = common.opened(Path(path).resolve(strict=True))
        need(expected is None or pin["sha256"] == expected, "reviewed physical input SHA differs: " + pin["path"])
        need(pin["path"] not in inputs or inputs[pin["path"]] == pin, "selected input changed")
        inputs[pin["path"]] = pin
        return pin

    def product(path, limit=2 * GIB):
        pin = common.opened(Path(path), limit)
        need(pin["path"] not in products or products[pin["path"]] == pin, "retained product changed")
        products[pin["path"]] = pin
        return pin

    def control(path):
        pin = product(path, 16 * MIB); controls[pin["path"]] = pin
        return pin

    def save():
        report["input_pins"] = list(inputs.values()); report["product_pins"] = list(products.values())
        if runner is not None:
            report["commands"] = runner.commands
        common.atomic_json(out / "report.json", report)

    def guard():
        limits = spec["limits"]
        need(time.monotonic() - started < limits["overall_seconds"], "overall acquisition deadline exceeded")
        common.cgroup_preserved(before_cg, common.cgroup_state(spec["cgroup"]))
        # The output may be a sparse-image mount below a host filesystem.
        # Its own free-byte count cannot establish backing-store headroom.
        # Check every visible ancestor filesystem as well, including the
        # mounted output's parent. No mount, resize or host cleanup is done.
        for reserve_path in (out, *out.parents):
            space = os.statvfs(reserve_path)
            need(space.f_bavail * space.f_frsize >= limits["min_free_bytes"],
                 "output/ancestor filesystem reserve exhausted: " + str(reserve_path))
        if active_data is not None and active_data.exists():
            need(active_data.lstat().st_size <= max(limits["max_tar_bytes"], limits["max_index_bytes"]), "active decompressed data bound exceeded")

    def run(label, argv, cwd=None):
        need(all(common.opened(Path(p["path"])) == p for p in controls.values()), "isolated APT control/keyring changed")
        need(all(not any(directory.iterdir()) for directory in empty_control_dirs), "isolated APT snippet/source/trust directory is no longer empty")
        try:
            row = runner.run(label, argv, out if cwd is None else cwd, environment)
            # This producer can run thousands of package tools. The shared
            # runner intentionally retains pidfds until final cleanup, which
            # is appropriate for its short build lanes. Here retire ONLY
            # kernel-proven terminal handles after retaining their identities;
            # never signal by numeric PID or silently continue with a live
            # earlier actor. All retained Popen records remain for the final
            # cleanup audit and every live/unproven handle remains owned.
            runner.discover()
            terminal = []; row["terminal_owned_pidfds"] = terminal
            for key, item in list(runner.owned.items()):
                need(bool(select.select([item["fd"]], [], [], 0)[0]), "previous acquisition actor is still live after command exit")
                terminal.append({"identity": item["identity"], "identity_after": item["identity_after"], "pidfd_terminal": True})
                os.close(item["fd"]); del runner.owned[key]
            for key, item in runner.unproven.items():
                now = common.identity(item["pid"])
                need(common.birth(now) != key or now["state"] == "Z", "unproven actor remains live; refuse next package command")
            common.atomic_json(out / "commands.json", runner.commands)
            return row
        finally:
            if runner.commands and "log" in runner.commands[-1]:
                need(product(runner.commands[-1]["log"]["path"], spec["limits"]["log_bytes"]) == runner.commands[-1]["log"], "command log changed")
            save()

    def data_command(label, path, limit, command):
        nonlocal active_data
        active_data = path
        argv = [tools["python"], "-B", str(Path(__file__).resolve()), "--emit-stdout", str(path), str(limit), *command]
        row = run(label, argv)
        row["same_pid_exec_argv"] = command
        row["stdout_data"] = product(path, limit)
        row["stdout_data_limit"] = limit
        common.atomic_json(out / "commands.json", runner.commands)
        active_data = None; save()
        return row["stdout_data"]

    save()
    try:
        roots = validate_spec(spec)
        architecture = spec["target"]["architecture"]
        need(not within(out, [source, *roots]) and not within(source, [out]) and
             all(not within(r, [out]) for r in roots), "output must be disjoint from frozen source and tool roots")
        remember(Path(__file__)); remember(args.spec, args.spec_sha256); remember(manifest, args.manifest_sha256)
        source_before = common.source_state(source, manifest); report["source_before"] = source_before
        before_cg = common.cgroup_state(spec["cgroup"]); report["cgroup_before"] = before_cg
        need(all(a["events"].get(k, 0) == 0 for a in before_cg["ancestors"] for k in ("oom", "oom_kill", "oom_group_kill")), "ancestor OOM counters must initially be zero")
        need(before_cg["membership"] == ["0::/" + str(Path(spec["cgroup"]["expected_leaf"]).relative_to("/sys/fs/cgroup"))], "exact supplied original leaf membership required")
        tools = {}
        for name, selected in spec["tools"].items():
            if name == "apt_methods":
                need(within(absolute(selected, True), roots), "APT method root must be inventoried")
                continue
            real = absolute(selected["path"]); need(within(real, roots), "host tool escapes whole inventory roots")
            remember(real, selected["sha256"])
            tools[name] = str(Path(selected["path"]).parent.resolve(strict=True) / Path(selected["path"]).name)
            aliases_before[selected["path"]] = alias_binding(selected["path"])
        need(Path(tools["python"]).resolve(strict=True) == Path(sys.executable).resolve(strict=True), "actual interpreter must equal reviewed Python tool")
        for selected in spec["input_files"] + [spec["tls_ca"], spec["distribution_lock"], spec["seed_review"]]:
            remember(selected["path"], selected["sha256"])
            aliases_before[selected["path"]] = alias_binding(selected["path"])
        for selection in [*spec["input_roots"], spec["tools"]["apt_methods"], str(args.source), str(args.manifest), str(args.spec)]:
            aliases_before[selection] = alias_binding(selection)
        seed_review, seed_pin = common.document(Path(spec["seed_review"]["path"]).resolve(strict=True))
        need(seed_pin["sha256"] == spec["seed_review"]["sha256"], "seed review changed after admission")
        closed(seed_review, ("format_version", "kind", "target", "seed_groups", "policy"), "explicit SDK seed review")
        need(seed_review["format_version"] == 1 and seed_review["kind"] == "reviewed-apt-sdk-seeds" and
             seed_review["target"] == spec["target"] and seed_review["policy"] == "EMPTY_STATUS_FULL_TRANSITIVE_CLOSURE_NO_POSTINST",
             "actual selected OS/ROS seed review differs")
        closed(seed_review["seed_groups"], ("base_os", "ros_development", "build_tools", "python"), "seed categories")
        groups = seed_review["seed_groups"]
        need(all(type(group) is list and group and all(type(x) is str and PKG.fullmatch(x) for x in group)
                 for group in groups.values()) and set(spec["seeds"]) == {x for group in groups.values() for x in group},
             "all four explicit OS/ROS-development/compiler-CMake/Python seed categories must match solver inputs")
        if spec["target"]["distribution"] in FIRST_FOUR:
            distro = spec["target"]["distribution"]
            requested = {s.split("=", 1)[0].removesuffix(":" + architecture) for s in groups["ros_development"]}
            # This lane's fixtures use more than ros-base. Names select real
            # packages only; their existence/versions are proved by the actual
            # signed solver/download, never by these strings or a profile.
            components = ("ament-cmake", "rclcpp", "rclcpp-action", "action-tutorials-interfaces", "std-msgs",
                "geometry-msgs", "sensor-msgs", "std-srvs", "rcl-interfaces", "rcl-action", "lifecycle-msgs",
                "rclcpp-lifecycle", "tf2", "tf2-ros", "tf2-msgs", "rcl", "rmw",
                "rosidl-typesupport-introspection-cpp", "rosidl-typesupport-cpp")
            need({"ros-" + distro + "-" + c for c in components} <= requested,
                 "current gateway/Action/TF/lifecycle fixture development packages must be explicit modern ROS2 seeds")
            build_requested = {s.split("=", 1)[0].removesuffix(":" + architecture) for s in groups["build_tools"]}
            need({"build-essential", "cmake", "ninja-build", "pkg-config", "libssl-dev"} <= build_requested,
                 "explicit native compiler/CMake/Python SDK preparation cannot use ros-base alone")
            python_requested = {s.split("=", 1)[0].removesuffix(":" + architecture) for s in groups["python"]}
            need({"python3", "python3-dev", "python3-yaml", "python3-empy", "python3-numpy", "python3-setuptools"} <= python_requested,
                 "explicit target Python development/generator seed group incomplete")
        report["seed_review"] = seed_pin
        # Host NSS/DNS inputs remain explicit; no ambient proxy/loader/PYTHONPATH
        # is inherited. Their libc/NSS libraries and Python stdlib belong to the
        # complete selected roots, not merely a version string in this report.
        selected_configs = {str(Path(r["path"]).resolve(strict=True)) for r in spec["input_files"]}
        for required in ("/etc/hosts", "/etc/resolv.conf", "/etc/nsswitch.conf"):
            need(str(Path(required).resolve(strict=True)) in selected_configs, "explicit actual host NSS/DNS config pin missing: " + required)
        for index, root in enumerate(roots):
            guard(); entry = common.catalog(root, out / ("host-before-%02d.jsonl" % index), roots)
            product(entry["catalog"]["path"]); roots_before.append(entry)
        report["input_roots_before"] = roots_before
        apt = out / "apt"; apt.mkdir()
        for relative in ("etc", "etc/parts", "etc/sourceparts", "etc/preferencesparts", "etc/trustedparts", "state", "state/lists", "state/lists/partial", "cache", "cache/archives", "cache/archives/partial", "home", "gnupg", "downloads", "indices", "keys"):
            (apt / relative).mkdir(mode=0o700)
        empty_control_dirs = [apt / relative for relative in ("etc/parts", "etc/sourceparts", "etc/preferencesparts", "etc/trustedparts")]
        for relative in ("etc/main.conf", "etc/preferences", "etc/trusted.gpg", "state/status", "state/extended_states"):
            write_bytes(apt / relative, b"")
            if relative != "state/extended_states":
                control(apt / relative)
        sources, source_paths = [], {}
        for repo in spec["repositories"]:
            key = apt / "keys" / (repo["name"] + ".gpg")
            pin = remember(repo["keyring"]["path"], repo["keyring"]["sha256"])
            aliases_before[repo["keyring"]["path"]] = alias_binding(repo["keyring"]["path"])
            write_bytes(key, bounded_body(Path(pin["path"]), 16 * MIB)); need(control(key)["sha256"] == pin["sha256"], "keyring byte copy differs")
            if repo["expiry"]["review"] is not None:
                remember(repo["expiry"]["review"]["path"], repo["expiry"]["review"]["sha256"])
                aliases_before[repo["expiry"]["review"]["path"]] = alias_binding(repo["expiry"]["review"]["path"])
            # Do not set Trusted=yes OR Trusted=no. Either overrides APT's
            # authenticated-source state; Signed-By must determine that state.
            body = "Types: deb\nURIs: " + safe_url(repo["uri"]) + "\nSuites: " + repo["suite"] + "\nComponents: " + " ".join(repo["components"]) + "\nArchitectures: " + architecture + "\nSigned-By: " + str(key) + "\n"
            if repo["expiry"]["mode"] == "pinned-expired":
                body += "Check-Valid-Until: no\n"
            body += "\n"; sources.append(body)
            path = apt / "etc" / (repo["name"] + ".sources"); write_bytes(path, body.encode()); control(path)
            source_paths[repo["name"]] = path
        all_sources = apt / "etc/all.sources"; write_bytes(all_sources, "".join(sources).encode()); control(all_sources)
        values = {
            "Dir::Etc": str(apt / "etc"), "Dir::Etc::main": str(apt / "etc/main.conf"),
            "Dir::Etc::parts": str(apt / "etc/parts"), "Dir::Etc::sourcelist": str(all_sources),
            "Dir::Etc::sourceparts": str(apt / "etc/sourceparts"), "Dir::Etc::preferences": str(apt / "etc/preferences"),
            "Dir::Etc::preferencesparts": str(apt / "etc/preferencesparts"), "Dir::Etc::trusted": str(apt / "etc/trusted.gpg"),
            "Dir::Etc::trustedparts": str(apt / "etc/trustedparts"), "Dir::State": str(apt / "state"),
            "Dir::State::status": str(apt / "state/status"), "Dir::State::extended_states": str(apt / "state/extended_states"),
            "Dir::State::lists": str(apt / "state/lists"), "Dir::Cache": str(apt / "cache"),
            "Dir::Cache::archives": str(apt / "cache/archives"), "Dir::Cache::pkgcache": str(apt / "cache/pkgcache.bin"),
            "Dir::Cache::srcpkgcache": str(apt / "cache/srcpkgcache.bin"),
            "Dir::Log": str(apt), "Dir::Bin::methods": str(absolute(spec["tools"]["apt_methods"], True)),
            "Dir::Bin::dpkg": tools["dpkg"],
            "APT::Architecture": architecture, "APT::Sandbox::User": pwd.getpwuid(os.getuid()).pw_name,
            "APT::Install-Recommends": "false", "APT::Install-Suggests": "false", "APT::Get::Download-Only": "true",
            "APT::Get::AllowUnauthenticated": "false", "APT::Get::List-Cleanup": "false",
            "Acquire::AllowInsecureRepositories": "false", "Acquire::AllowDowngradeToInsecureRepositories": "false",
            "Acquire::Check-Date": "true", "Acquire::Check-Valid-Until": "true", "Acquire::Max-FutureTime": "300",
            "Acquire::https::Verify-Peer": "true", "Acquire::https::Verify-Host": "true", "Acquire::https::CaInfo": str(absolute(spec["tls_ca"]["path"])),
            # Retry transient transport failures within the existing command
            # deadline. Signed metadata and every package hash are still
            # checked; a final download failure cannot qualify an SDK.
            "Acquire::http::Proxy": "DIRECT", "Acquire::https::Proxy": "DIRECT", "Acquire::Retries": "3",
            "Acquire::Queue-Mode": "access", "Acquire::http::Pipeline-Depth": "0", "Acquire::https::Pipeline-Depth": "0",
            "Acquire::Languages": "none", "Acquire::GzipIndexes": "false", "APT::Color": "false", "Dpkg::Use-Pty": "false",
        }
        config = apt / "bootstrap.conf"
        # APT_CONFIG is loaded before default snippets. Passing only '-c' would
        # be too late to prevent ambient configuration/hook inclusion.
        text = "".join(k + " " + apt_text(v) + ";\n" for k, v in values.items())
        text += '#clear APT::Architectures;\nAPT::Architectures { ' + apt_text(architecture) + '; };\n'
        for hook in ("DPkg::Pre-Invoke", "DPkg::Post-Invoke", "DPkg::Pre-Install-Pkgs", "APT::Update::Pre-Invoke", "APT::Update::Post-Invoke", "APT::Update::Post-Invoke-Success"):
            text += "#clear " + hook + ";\n"
        write_bytes(config, text.encode()); control(config)
        environment = {"APT_CONFIG": str(config), "HOME": str(apt / "home"), "GNUPGHOME": str(apt / "gnupg"),
                       "LANG": "C", "LC_ALL": "C", "PATH": ":".join(dict.fromkeys(str(Path(p).parent) for p in tools.values())),
                       "PYTHONDONTWRITEBYTECODE": "1", "GOMAXPROCS": "1", "MAKEFLAGS": "-j1", "DPKG_DEB_THREADS_MAX": "1"}
        report["environment"] = environment
        runner = common.SerialRunner(out, {k: spec["limits"][k] for k in ("rss_bytes", "log_bytes", "command_seconds")}, guard)
        config_row = run("apt-isolated-config", [tools["apt_config"], "dump"])
        config_lines = Path(config_row["log"]["path"]).read_text().splitlines()
        report["effective_apt_configuration"] = effective_apt_configuration(config_lines, values)
        report["effective_apt_configuration"]["actual_dump"] = config_row["log"]
        save()
        run("apt-host-version", [tools["apt_get"], "--version"])
        run("dpkg-deb-host-version", [tools["dpkg_deb"], "--version"])
        # One reviewed source is updated at a time. APT's own URI formatter
        # supplies the actual retained filename; no lossy URL-name guess.
        phase = "authenticated-index-update"
        for repo in spec["repositories"]:
            option = ["-o", "Dir::Etc::sourcelist=" + str(source_paths[repo["name"]])]
            planned = run(repo["name"] + "-index-uri-plan", [tools["apt_get"], *option, "--print-uris", "update"])
            uri = safe_url(repo["uri"]) + "dists/" + repo["suite"] + "/InRelease"
            matches = [r for r in print_uris(planned["log"]["path"]) if r["uri"] == uri]
            if len(matches) != 1:
                raise Blocked("THIS_FINITE_PRODUCER_REQUIRES_APT_ADVERTISED_InRelease: " + repo["name"])
            try:
                run(repo["name"] + "-authenticated-update", [tools["apt_get"], *option, "-o", "APT::Update::Error-Mode=any", "update"])
            except ValueError as error:
                # Keep the original actual FAIL command/log. An unavailable or
                # unauthenticatable repository is an acquisition prerequisite
                # blocker; it is never a ROS implementation test failure/PASS.
                if not str(error).startswith("command returned unexpected status:"):
                    raise
                raise Blocked("AUTHENTICATED_REPOSITORY_UPDATE_UNAVAILABLE: " + repo["name"] + ": " + str(error)) from error
            inrelease = apt / "state/lists" / matches[0]["filename"]
            signed = product(inrelease, spec["limits"]["max_index_bytes"])
            expected = repo["expected_inrelease_sha256"]
            need(expected is None or signed["sha256"] == expected, "signed Release differs from independently reviewed immutable pin")
            decoded = apt / "indices" / (repo["name"] + "-Release")
            status_file = apt / "indices" / (repo["name"] + "-gpg-status")
            # Machine status has its own stdout artifact. Human stderr (which
            # may mention untrusted key UIDs) must never be parsed as status.
            status_pin = data_command(repo["name"] + "-independent-gpgv", status_file, MIB,
                [tools["gpgv"], "--homedir", str(apt / "gnupg"), "--keyring", str(apt / "keys" / (repo["name"] + ".gpg")), "--status-fd", "1", "--output", str(decoded), str(inrelease)])
            statuses = status_file.read_text().splitlines()
            valid = [s.split() for s in statuses if s.startswith("[GNUPG:] VALIDSIG ")]
            # Primary-key fingerprint is the optional final VALIDSIG field.
            # If omitted, only the actual signing fingerprint itself may match
            # a reviewed primary key; no inferred subkey-to-primary relation.
            need(valid and all(len(s) in (11, 12) and (s[11] if len(s) == 12 else s[2])
                 in repo["fingerprints"] for s in valid), "actual GPG primary signing fingerprint not reviewed")
            need(not any(s.startswith("[GNUPG:] " + bad + " ") for s in statuses for bad in
                         ("BADSIG", "ERRSIG", "NO_PUBKEY", "EXPSIG", "EXPKEYSIG", "REVKEYSIG")), "invalid/expired/revoked GPG signature status")
            product(decoded, spec["limits"]["max_index_bytes"])
            release, hashes = release_checks(decoded, repo, architecture)
            report["repositories"].append({"name": repo["name"], "uri": safe_url(repo["uri"]), "suite": repo["suite"],
                "signed_InRelease": signed, "verified_Release": product(decoded), "gpg_status": status_pin, "validsig": valid,
                "signed_fields": {k: release.get(k) for k in ("Origin", "Label", "Suite", "Codename", "Date", "Valid-Until")},
                "expiry_policy": repo["expiry"], "signed_SHA256": hashes, "indices": []})
            save()
        phase = "solver-plan"
        target_row = run("apt-authenticated-index-targets", [tools["apt_get"], "indextargets"])
        targets = list(file_paragraphs(target_row["log"]["path"]))
        try:
            plan = run("apt-empty-status-complete-solver-plan", [tools["apt_get"], "--yes", "--no-remove", "--no-install-recommends", "--no-install-suggests", "--download-only", "--print-uris", "install", *spec["seeds"]])
        except ValueError as error:
            if not str(error).startswith("command returned unexpected status:"):
                raise
            raise Blocked("SIGNED_SELECTED_PACKAGE_CLOSURE_UNAVAILABLE: " + str(error)) from error
        selections = print_uris(plan["log"]["path"])
        need(0 < len(selections) <= spec["limits"]["max_packages"] and len({uri_identity(r["uri"]) for r in selections}) == len(selections), "complete unique nonempty package download plan required")
        need(sum(r["bytes"] for r in selections) <= spec["limits"]["max_total_download_bytes"], "total solver download bytes exceed reviewed bound")
        wanted = {uri_identity(r["uri"]): r for r in selections}; selected = {}
        for repo, ledger in zip(spec["repositories"], report["repositories"]):
            prefix = safe_url(repo["uri"]) + "dists/" + repo["suite"] + "/"
            for index, target in enumerate(targets):
                key = target.get("MetaKey", "")
                if not key.endswith("/binary-" + architecture + "/Packages") or target.get("URI") != prefix + key:
                    continue
                need(key.split("/", 1)[0] in repo["components"] and key in ledger["signed_SHA256"], "Packages target is not in signed selected component")
                actual = Path(target["Filename"])
                need(actual.is_absolute() and actual.parent == apt / "state/lists", "APT retained index path escapes owned lists")
                compressed_pin = product(actual, spec["limits"]["max_index_bytes"])
                plain = apt / "indices" / (repo["name"] + "-%04d-Packages" % index)
                plain_pin = data_command(repo["name"] + "-%04d-decode-index" % index, plain, spec["limits"]["max_index_bytes"], [tools["apt_helper"], "cat-file", str(actual)])
                need({k: plain_pin[k] for k in ("sha256", "bytes")} == ledger["signed_SHA256"][key], "decoded Packages SHA256/size differs from GPG-signed Release")
                ledger["indices"].append({"MetaKey": key, "apt_retained": compressed_pin, "signed_uncompressed": plain_pin})
                for pkg in file_paragraphs(plain):
                    filename = pkg.get("Filename", "")
                    rel = PurePosixPath(filename)
                    need(filename and not rel.is_absolute() and ".." not in rel.parts and not any(c in filename for c in "\0\r\n"), "unsafe signed package Filename")
                    url = uri_identity(safe_url(repo["uri"]) + filename)
                    if url not in wanted:
                        continue
                    need(pkg.get("Architecture") in (architecture, "all") and SHA.fullmatch(pkg.get("SHA256", "")) and pkg.get("Size", "").isdigit(), "selected signed package lacks selected-architecture/all strong hash")
                    need(int(pkg["Size"]) == wanted[url]["bytes"] and int(pkg["Size"]) <= spec["limits"]["max_deb_bytes"], "signed package size differs from solver/bound")
                    row = {"repository": repo["name"], "index": plain_pin, "uri": url, "package": pkg["Package"], "version": pkg["Version"],
                        "architecture": pkg["Architecture"], "filename": filename, "bytes": int(pkg["Size"]), "sha256": pkg["SHA256"],
                        "Depends": pkg.get("Depends"), "Pre-Depends": pkg.get("Pre-Depends"), "Essential": pkg.get("Essential")}
                    need(url not in selected or all(selected[url][k] == row[k] for k in ("package", "version", "architecture", "bytes", "sha256")), "ambiguous signed package bodies")
                    selected[url] = row
            need(ledger["indices"], "no authenticated selected-architecture Packages index for source: " + repo["name"])
            save()
        need(set(selected) == set(wanted), "solver URI has no retained GPG→SHA256→Packages→deb binding")
        report["solver_plan"] = {"raw_log": plan["log"], "empty_status": product(apt / "state/status"), "seeds": spec["seeds"],
            "packages": len(selections), "download_bytes": sum(r["bytes"] for r in selections), "install_or_postinst": False}
        sdk = out / "sdk"; sdk.mkdir(mode=0o700)
        tree = SafeTree(sdk, out / "payloads.jsonl", spec["limits"], guard)
        phase = "serial-download-extraction"
        total_tar_bytes = 0
        for ordinal, item in enumerate(selections):
            selected_pkg = selected[uri_identity(item["uri"])]
            directory = apt / "downloads" / ("%05d" % ordinal); directory.mkdir(mode=0o700)
            # Download exactly the solver-selected name/version/architecture.
            # APT authenticates this lookup as well; equality to the independently
            # verified signed package SHA is mandatory before any extraction.
            name = selected_pkg["package"] + ":" + selected_pkg["architecture"] + "=" + selected_pkg["version"]
            need(PKG.fullmatch(selected_pkg["package"]) and not any(c in name for c in "\0\r\n"), "selected package identity invalid")
            download = run("deb-%05d-authenticated-download" % ordinal, [tools["apt_get"], "download", name], directory)
            debs = list(directory.iterdir()); need(len(debs) == 1 and debs[0].suffix == ".deb" and not debs[0].is_symlink(), "one complete retained package required")
            deb = product(debs[0], spec["limits"]["max_deb_bytes"])
            need({k: deb[k] for k in ("sha256", "bytes")} == {k: selected_pkg[k] for k in ("sha256", "bytes")}, "downloaded deb not equal to authenticated package")
            metadata = run("deb-%05d-control-fields" % ordinal, [tools["dpkg_deb"], "--field", str(debs[0])])
            deb_control = list(file_paragraphs(metadata["log"]["path"])); need(len(deb_control) == 1, "one actual deb control stanza required")
            need(all(deb_control[0].get(k) == selected_pkg[v] for k, v in (("Package", "package"), ("Version", "version"), ("Architecture", "architecture"))), "actual deb control identity differs from signed Packages")
            tar = directory / "data.tar"
            tar_pin = data_command("deb-%05d-data-tar" % ordinal, tar, spec["limits"]["max_tar_bytes"], [tools["dpkg_deb"], "--fsys-tarfile", str(debs[0])])
            total_tar_bytes += tar_pin["bytes"]
            need(total_tar_bytes <= spec["limits"]["max_total_tar_bytes"], "retained uncompressed tar quota exceeded")
            tree.extract(tar, selected_pkg["package"] + "=" + selected_pkg["version"])
            report["packages"].append({**selected_pkg, "deb": deb, "actual_control_log": metadata["log"],
                                       "data_tar": tar_pin, "authenticated_download_log": download["log"]})
            save()
        tree.finish(); tree.close(); tree = None
        product(out / "payloads.jsonl")
        report["sdk_before_finalization"] = common.catalog(sdk, out / "sdk-catalog-before-finalization.jsonl", [sdk])
        product(out / "sdk-catalog-before-finalization.jsonl")
        report["acquisition_complete"] = True
    except Blocked as error:
        report["status"] = "BLOCKED"; report["reason"] = str(error)
    except FileNotFoundError as error:
        report["status"] = "BLOCKED" if phase == "prerequisites" else "FAILED_ACQUISITION"
        report["reason"] = "MISSING_REQUIRED_PHYSICAL_INPUT" if phase == "prerequisites" else "MISSING_ACTUAL_ACQUISITION_PRODUCT"
        report["error"] = repr(error); report["failed_phase"] = phase
    except BaseException as error:
        report["status"] = "FAILED_ACQUISITION"; report["error"] = repr(error); report["traceback"] = traceback.format_exc()
        report["failed_phase"] = phase
    finally:
        if tree is not None:
            try:
                tree.close()
            except BaseException as error:
                report["extraction_close_error"] = repr(error)
        try:
            report["cleanup"] = runner.close() if runner is not None else {"status": "PASS", "remaining": [], "scope": "NO_ACTOR_STARTED"}
            if runner is not None:
                common.atomic_json(out / "commands.json", runner.commands)
                report["commands_file"] = product(out / "commands.json")
            need(report["cleanup"]["status"] == "PASS" and not report["cleanup"]["remaining"], "owned acquisition cleanup incomplete")
        except BaseException as error:
            report["status"] = "FAILED_ACQUISITION"; report["cleanup_error"] = repr(error)
        try:
            if source_before is not None:
                report["source_after"] = common.source_state(source, manifest)
                need(report["source_after"] == source_before, "frozen source changed")
                report["source_preserved"] = True
            if before_cg is not None:
                report["cgroup_after"] = common.cgroup_state(spec["cgroup"])
                common.cgroup_preserved(before_cg, report["cgroup_after"])
            after = []
            for index, root in enumerate(roots):
                entry = common.catalog(root, out / ("host-after-%02d.jsonl" % index), roots)
                product(entry["catalog"]["path"]); after.append(entry)
            report["input_roots_after"] = after
            need(len(roots_before) == len(after) and all(a["content_sha256"] == b["content_sha256"] for a, b in zip(roots_before, after)), "whole host tool/SDK inputs changed or initial inventory incomplete")
            need(all(common.opened(Path(p["path"])) == p for p in inputs.values()), "selected physical input changed")
            need(all(alias_binding(name) == binding for name, binding in aliases_before.items()), "selected tool/metadata/root alias identity or target changed")
            report["selected_alias_bindings"] = aliases_before
            need(all(not any(directory.iterdir()) for directory in empty_control_dirs), "isolated APT control directory acquired an unexpected snippet/source/key")
            need(all(common.opened(Path(p["path"])) == p for p in products.values()), "retained raw product/log changed")
            report["inputs_preserved"] = True; report["products_preserved"] = True
            if report.get("acquisition_complete") is True:
                final = common.catalog(out / "sdk", out / "sdk-catalog-final.jsonl", [out / "sdk"])
                product(out / "sdk-catalog-final.jsonl")
                need(final["content_sha256"] == report["sdk_before_finalization"]["content_sha256"], "extracted SDK changed during finalization")
                report["sdk"] = final
                need(report["status"] == "RUNNING" and all(r["status"] == "PASS" and r.get("returncode") == 0 for r in report["commands"]), "failed command cannot be promoted by cleanup")
                report["status"] = "PREPARED_AUTHENTICATED_EXTRACTED_SYSROOT"
        except BaseException as error:
            report["status"] = "FAILED_ACQUISITION"; report["preservation_error"] = repr(error)
        report["finished_monotonic_ns"] = time.monotonic_ns(); save()
    print(json.dumps({"status": report["status"], "report": str(out / "report.json"), "packages": len(report["packages"]),
                      "compatibility_qualified": False, "target_program_executed": False}), flush=True)
    return 0 if report["status"] == "PREPARED_AUTHENTICATED_EXTRACTED_SYSROOT" else 2 if report["status"] == "BLOCKED" else 1


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--emit-stdout":
        emit_stdout(sys.argv[2:])
    else:
        raise SystemExit(main())
