#!/usr/bin/env python3
"""Pack explicitly selected, frozen ROS evidence; never discover SDK/rootfs inputs.

Example:
  python3 pack_ros2_next.py --input source=/absolute/frozen/source \
    --input reports=/absolute/reports \
    --file history/policy.tgz=/absolute/verified-policy.tgz \
    --out /absolute/delivery.tgz --index /absolute/delivery-index.json

--input accepts a regular text file or a directory of allowed UTF-8 files.
--file is an explicit caller attestation that a .tgz is a previously verified
complete historical archive. Its structure and bytes are checked again here;
this program cannot infer the historical test outcome from an archive name.
Nested archives may contain binary payloads, and the index says so explicitly.
"""

import argparse
import codecs
import gzip
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import stat
import sys
import tarfile
import tempfile
import zlib


CHUNK = 1024 * 1024
EXTENSIONS = frozenset({
    ".json", ".log", ".txt", ".md", ".py", ".cc", ".cpp", ".h", ".hpp",
    ".d", ".make", ".cmake", ".wat", ".sh", ".lua", ".tpp", ".ipp",
    ".in", ".hh", ".c", ".def", ".toml", ".yaml", ".yml", ".policy",
    ".cfg", ".out",
})
SPECIAL_NAMES = frozenset({"flags.make", "link.txt", "depend.make"})
PRIVATE_HEADER = re.compile(
    rb"(?m)^[ \t]*(?:-----BEGIN (?:[A-Z0-9 ]* )?PRIVATE KEY-----|"
    rb"---- BEGIN SSH2 ENCRYPTED PRIVATE KEY ----)[ \t]*\r?$"
)


class PackError(Exception):
    pass


def safe_name(value):
    """Use one canonical POSIX spelling so aliases cannot hide collisions."""
    if (not value or value.startswith("/") or "\\" in value
            or re.match(r"^[A-Za-z]:", value)
            or any(ord(c) < 32 or ord(c) == 127 for c in value)):
        raise PackError("unsafe archive name: " + repr(value))
    parts = value.split("/")
    if any(p in ("", ".", "..") for p in parts):
        raise PackError("unsafe archive name: " + repr(value))
    return "/".join(parts)


def canonical_path(value):
    path = Path(value)
    if not path.is_absolute():
        raise PackError("source and output paths must be absolute: " + value)
    # Parent aliases such as macOS /tmp are legitimate. The selected leaf itself
    # is checked with lstat; resolving it would conceal an explicit symlink.
    return Path(os.path.normpath(path.parent.resolve(strict=True) / path.name))


def mapping(value):
    name, sep, source = value.partition("=")
    if not sep:
        raise PackError("expected name=/absolute/path: " + repr(value))
    return safe_name(name), canonical_path(source)


def private_path(name):
    for part in PurePosixPath(name).parts:
        low = part.lower()
        if (low.endswith((".pem", ".key")) or low == "key"
                or any(x in low for x in ("private_key", "private-key", "privatekey"))):
            return True
    return False


def fingerprint(info):
    return (info.st_dev, info.st_ino, info.st_mode, info.st_size,
            info.st_mtime_ns, info.st_ctime_ns)


class PrivateKeyDetector:
    def __init__(self):
        self.tail = b""
        self.found = False

    def feed(self, block):
        joined = self.tail + block
        last = joined.rfind(b"\n")
        if last >= 0:
            if PRIVATE_HEADER.search(joined[:last + 1]):
                self.found = True
            self.tail = joined[last + 1:]
        else:
            self.tail = joined
        # A key header is short. A truncated long line gets a non-whitespace
        # sentinel, so a header-looking substring cannot become a false line start.
        if len(self.tail) > 256:
            self.tail = b"x" + self.tail[-256:]

    def finish(self):
        return self.found or bool(PRIVATE_HEADER.search(self.tail))


def open_regular(path, expected=None):
    flags = os.O_RDONLY | getattr(os, "O_NOFOLLOW", 0) | getattr(os, "O_NONBLOCK", 0)
    fd = os.open(path, flags)
    info = os.fstat(fd)
    if not stat.S_ISREG(info.st_mode):
        os.close(fd)
        raise PackError("source is no longer a regular file: " + str(path))
    if expected is not None and fingerprint(info) != expected:
        os.close(fd)
        raise PackError("source metadata changed after freeze: " + str(path))
    return os.fdopen(fd, "rb"), info


def freeze(path):
    """Read every byte once, recording identity, digest, UTF-8 and key evidence."""
    stream, info = open_regular(path)
    expected = fingerprint(info)
    digest = hashlib.sha256()
    decoder = codecs.getincrementaldecoder("utf-8")("strict")
    utf8 = True
    private = PrivateKeyDetector()
    first = b""
    count = 0
    with stream:
        while True:
            block = stream.read(CHUNK)
            if not block:
                break
            if not count:
                first = block[:4]
            count += len(block)
            digest.update(block)
            private.feed(block)
            if utf8:
                try:
                    decoder.decode(block)
                except UnicodeDecodeError:
                    utf8 = False
        if utf8:
            try:
                decoder.decode(b"", final=True)
            except UnicodeDecodeError:
                utf8 = False
        if fingerprint(os.fstat(stream.fileno())) != expected:
            raise PackError("source changed while freezing: " + str(path))
    if fingerprint(path.lstat()) != expected or count != info.st_size:
        raise PackError("source changed while freezing: " + str(path))
    return {"sha256": digest.hexdigest(), "bytes": count, "isutf8": utf8,
            "elf": first == b"\x7fELF", "private": private.finish(), "_fingerprint": expected}


def check_nested_archive(path, expected):
    """Validate full gzip data, tar members and absence of private key material."""
    stream, _ = open_regular(path, expected)
    with stream, gzip.GzipFile(fileobj=stream, mode="rb") as zipped:
        while zipped.read(CHUNK):
            pass  # Also validate the trailer, beyond tar's end-of-archive blocks.
        if fingerprint(os.fstat(stream.fileno())) != expected:
            raise PackError("nested archive changed during gzip validation")
    stream, _ = open_regular(path, expected)
    seen = set()
    regular = set()
    members = 0
    with stream, tarfile.open(fileobj=stream, mode="r:gz") as archive:
        for member in archive:
            name = safe_name(member.name[:-1] if member.isdir() and member.name.endswith("/") else member.name)
            if name in seen:
                raise PackError("duplicate nested archive member: " + name)
            seen.add(name)
            if private_path(name):
                raise PackError("private key path in nested archive: " + name)
            if member.isdir():
                continue
            if not member.isfile():
                raise PackError("nested archive contains a link or special member: " + name)
            regular.add(name)
            data = archive.extractfile(member)
            if data is None:
                raise PackError("unreadable nested archive member: " + name)
            count = 0
            private = PrivateKeyDetector()
            with data:
                while True:
                    block = data.read(CHUNK)
                    if not block:
                        break
                    count += len(block)
                    private.feed(block)
            if private.finish():
                raise PackError("private key content in nested archive: " + name)
            if count != member.size:
                raise PackError("truncated nested archive member: " + name)
            members += 1
        if fingerprint(os.fstat(stream.fileno())) != expected:
            raise PackError("nested archive changed during tar validation")
    for name in seen:
        if any(str(p) in regular for p in PurePosixPath(name).parents if str(p) != "."):
            raise PackError("nested regular file is another member's parent: " + name)
    if fingerprint(path.lstat()) != expected:
        raise PackError("nested archive changed after validation")
    return members


def inside(path, root):
    return path == root or root in path.parents


def collect(inputs, explicit_files):
    payloads, skipped, roots = [], [], []
    names = set()
    directories = []

    def reserve(name):
        safe_name(name)
        if name in names:
            raise PackError("duplicate archive name: " + name)
        names.add(name)

    def skip(name, path, reason, info=None):
        record = {"arcname": name, "source": str(path), "reason": reason}
        if info is not None:
            record["bytes"] = info.st_size
        skipped.append(record)

    def select(name, path, info, binary=False):
        reserve(name)
        if stat.S_ISLNK(info.st_mode):
            skip(name, path, "symlink-file", info)
            return
        if not stat.S_ISREG(info.st_mode):
            if binary:
                raise PackError("--file must refer to a regular .tgz file: " + str(path))
            skip(name, path, "not-regular", info)
            return
        if private_path(name) or private_path(path.name):
            if binary:
                raise PackError("explicit private key path: " + str(path))
            skip(name, path, "private-key-path", info)
            return
        if binary and path.suffix.lower() != ".tgz":
            raise PackError("--file only accepts explicit historical .tgz files: " + str(path))
        if not binary and path.suffix.lower() not in EXTENSIONS and path.name not in SPECIAL_NAMES:
            stream, checked = open_regular(path, fingerprint(info))
            with stream:
                elf = stream.read(4) == b"\x7fELF"
                if fingerprint(os.fstat(stream.fileno())) != fingerprint(checked):
                    raise PackError("source changed during magic check: " + str(path))
            skip(name, path, "binary-elf" if elf else "extension-not-allowed", info)
            return
        frozen = freeze(path)
        if fingerprint(info) != frozen["_fingerprint"]:
            raise PackError("source changed between inventory and freeze: " + str(path))
        reason = ("binary-elf" if frozen["elf"] else
                  "private-key-content" if frozen["private"] else
                  "invalid-utf8" if not frozen["isutf8"] else None)
        if not binary and reason:
            skip(name, path, reason, info)
            return
        record = {"arcname": name, "source": str(path),
                  "kind": "binary-nested-archive" if binary else "utf8-text",
                  "sha256": frozen["sha256"], "bytes": frozen["bytes"],
                  "isutf8": frozen["isutf8"], "_fingerprint": frozen["_fingerprint"]}
        if binary:
            if frozen["elf"] or frozen["private"]:
                raise PackError("explicit .tgz is ELF or private key material: " + str(path))
            record["nested_regular_members"] = check_nested_archive(path, frozen["_fingerprint"])
            record["historical_verified_archive_attested_by_caller"] = True
            record["nested_archive_validation"] = "full-gzip-trailer-and-safe-regular-tar-members"
        payloads.append(record)

    def walk(name, path, info):
        directories.append((path, fingerprint(info)))
        for child in sorted(path.iterdir(), key=lambda p: p.name):
            child_name = safe_name(name + "/" + child.name)
            child_info = child.lstat()
            if stat.S_ISLNK(child_info.st_mode):
                reserve(child_name)
                skip(child_name, child, "symlink-not-followed", child_info)
            elif stat.S_ISDIR(child_info.st_mode):
                if private_path(child_name):
                    reserve(child_name)
                    skip(child_name, child, "private-key-directory", child_info)
                else:
                    walk(child_name, child, child_info)
            else:
                select(child_name, child, child_info)
        if fingerprint(path.lstat()) != fingerprint(info):
            raise PackError("input directory changed while freezing: " + str(path))

    for binary, mappings in ((False, inputs), (True, explicit_files)):
        for value in mappings:
            name, path = mapping(value)
            info = path.lstat()
            if stat.S_ISLNK(info.st_mode):
                raise PackError("explicit input must not be a symlink: " + str(path))
            roots.append({"name": name, "source": str(path),
                          "selection": "explicit-historical-archive" if binary else "utf8-input",
                          "is_directory": stat.S_ISDIR(info.st_mode)})
            if stat.S_ISDIR(info.st_mode):
                if binary:
                    raise PackError("--file must refer to a regular .tgz file")
                if private_path(name) or private_path(path.name):
                    reserve(name)
                    skip(name, path, "private-key-directory", info)
                else:
                    walk(name, path, info)
            else:
                select(name, path, info, binary)
    payloads.sort(key=lambda p: p["arcname"])
    selected = {p["arcname"] for p in payloads}
    for name in selected:
        if any(str(p) in selected for p in PurePosixPath(name).parents if str(p) != "."):
            raise PackError("regular payload is another payload's parent: " + name)
    return payloads, sorted(skipped, key=lambda p: p["arcname"]), roots, directories


class CheckedReader:
    def __init__(self, stream):
        self.stream = stream
        self.digest = hashlib.sha256()
        self.count = 0

    def read(self, amount=-1):
        block = self.stream.read(amount)
        self.digest.update(block)
        self.count += len(block)
        return block


def unchanged(payloads, directories):
    for payload in payloads:
        if fingerprint(Path(payload["source"]).lstat()) != payload["_fingerprint"]:
            raise PackError("frozen source changed: " + payload["source"])
    for path, expected in directories:
        if fingerprint(path.lstat()) != expected:
            raise PackError("frozen directory changed: " + str(path))


def write_archive(path, payloads, directories):
    unchanged(payloads, directories)
    with path.open("wb") as destination:
        with gzip.GzipFile(filename="", mode="wb", fileobj=destination, mtime=0) as zipped:
            with tarfile.open(fileobj=zipped, mode="w|", format=tarfile.PAX_FORMAT) as archive:
                for payload in payloads:
                    stream, _ = open_regular(Path(payload["source"]), payload["_fingerprint"])
                    with stream:
                        reader = CheckedReader(stream)
                        info = tarfile.TarInfo(payload["arcname"])
                        info.size = payload["bytes"]
                        info.uid = info.gid = info.mtime = 0
                        info.uname = info.gname = ""
                        info.mode = 0o644
                        archive.addfile(info, reader)
                        if (reader.count != payload["bytes"]
                                or reader.digest.hexdigest() != payload["sha256"]
                                or stream.read(1)
                                or fingerprint(os.fstat(stream.fileno())) != payload["_fingerprint"]):
                            raise PackError("source content changed during archive write: " + payload["source"])
        destination.flush()
        os.fsync(destination.fileno())
    unchanged(payloads, directories)


def verify_archive(path, payloads):
    expected = {p["arcname"]: p for p in payloads}
    seen = set()
    with tarfile.open(path, "r:gz") as archive:
        for member in archive:
            if member.name in seen or member.name not in expected or not member.isfile():
                raise PackError("unexpected, duplicate or nonregular output member: " + member.name)
            seen.add(member.name)
            payload = expected[member.name]
            if (member.size != payload["bytes"] or member.uid or member.gid or member.mtime
                    or member.uname or member.gname or member.mode != 0o644):
                raise PackError("output metadata mismatch: " + member.name)
            stream = archive.extractfile(member)
            if stream is None:
                raise PackError("output member cannot be read: " + member.name)
            with stream:
                reader = CheckedReader(stream)
                while reader.read(CHUNK):
                    pass
            if reader.count != payload["bytes"] or reader.digest.hexdigest() != payload["sha256"]:
                raise PackError("output member digest mismatch: " + member.name)
    if seen != set(expected):
        raise PackError("output member set differs from index")
    # Consume all compressed bytes and check the final gzip trailer as well.
    with gzip.open(path, "rb") as zipped:
        while zipped.read(CHUNK):
            pass
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        while True:
            block = stream.read(CHUNK)
            if not block:
                break
            digest.update(block)
    return {"sha256": digest.hexdigest(), "bytes": path.stat().st_size,
            "verified_members": len(seen), "exact_index_match": True,
            "all_member_sha256_matched": True}


def ensure_outputs(out, index, mappings):
    if out == index:
        raise PackError("archive and index must be different paths")
    for target in (out, index):
        if os.path.lexists(target):
            raise PackError("refusing to overwrite existing output: " + str(target))
        for value in mappings:
            _, source = mapping(value)
            info = source.lstat()
            if target == source or (stat.S_ISDIR(info.st_mode) and inside(target, source)):
                raise PackError("output/index is inside an input tree: " + str(target))


def pack(args):
    if not args.input and not args.file:
        raise PackError("at least one explicit --input or --file mapping is required")
    out = canonical_path(args.out)
    index = canonical_path(args.index)
    mappings = args.input + args.file
    ensure_outputs(out, index, mappings)
    payloads, skipped, roots, directories = collect(args.input, args.file)
    archive_fd, archive_name = tempfile.mkstemp(prefix=".ros-next-pack-", suffix=".tgz", dir=out.parent)
    os.close(archive_fd)
    archive_temp = Path(archive_name)
    index_temp = None
    published_out = False
    try:
        write_archive(archive_temp, payloads, directories)
        verification = verify_archive(archive_temp, payloads)
        document = {
            "schema": "uwvm-ros-next-evidence-index-v1", "status": "PASS",
            "archive": {"path": str(out), **verification},
            "selection": {
                "explicit_inputs_only": True, "directory_symlinks_followed": False,
                "allowed_extensions": sorted(EXTENSIONS), "special_names": sorted(SPECIAL_NAMES),
                "private_keys_excluded": True,
                "binary_nested_archives_require_explicit_caller_attestation": True,
                "default_payloads_require_utf8": True,
            },
            "summary": {
                "payload_files": len(payloads), "payload_bytes": sum(p["bytes"] for p in payloads),
                "utf8_text_files": sum(p["kind"] == "utf8-text" for p in payloads),
                "binary_nested_archives": sum(p["kind"] == "binary-nested-archive" for p in payloads),
                "all_payloads_utf8": all(p["isutf8"] for p in payloads),
                "skipped_entries": len(skipped),
            },
            "inputs": roots,
            "payloads": [{k: v for k, v in p.items() if not k.startswith("_")} for p in payloads],
            "skipped": skipped,
            "verification": {
                "frozen_source_stream_hash_checked": True,
                "read_only_second_pass": True, "exact_member_set": True,
                "tar_metadata": {"uid": 0, "gid": 0, "mtime": 0, "mode": "0644"},
            },
        }
        fd, name = tempfile.mkstemp(prefix=".ros-next-index-", suffix=".json", dir=index.parent)
        index_temp = Path(name)
        with os.fdopen(fd, "w", encoding="utf-8", newline="\n") as stream:
            json.dump(document, stream, ensure_ascii=False, indent=2, sort_keys=True)
            stream.write("\n")
            stream.flush()
            os.fsync(stream.fileno())
        # Re-read the exact written index, rather than trusting only the in-memory object.
        with index_temp.open(encoding="utf-8") as stream:
            reread = json.load(stream)
        if reread != document:
            raise PackError("written index does not match the verified payload index")
        unchanged(payloads, directories)
        ensure_outputs(out, index, mappings)
        # link() publishes without replacing another process's output. Both temporary
        # files are siblings of their destinations, so no cross-filesystem rename.
        os.link(archive_temp, out)
        published_out = True
        os.link(index_temp, index)
        return document
    except BaseException:
        # If index publication failed, remove only the archive inode we created.
        if published_out and os.path.lexists(out) and os.path.samefile(out, archive_temp):
            out.unlink()
        raise
    finally:
        archive_temp.unlink(missing_ok=True)
        if index_temp is not None:
            index_temp.unlink(missing_ok=True)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--input", action="append", default=[], metavar="NAME=/ABSOLUTE/PATH")
    parser.add_argument("--file", action="append", default=[], metavar="NAME=/ABSOLUTE/VERIFIED.tgz")
    parser.add_argument("--out", required=True)
    parser.add_argument("--index", required=True)
    args = parser.parse_args(argv)
    try:
        document = pack(args)
    except (PackError, OSError, ValueError, EOFError, tarfile.TarError, zlib.error) as error:
        print("FAIL: " + str(error), file=sys.stderr)
        return 1
    print(json.dumps({"status": "PASS", "archive": document["archive"],
                      "summary": document["summary"]}, sort_keys=True))
    return 0


if __name__ == "__main__":
    sys.exit(main())
