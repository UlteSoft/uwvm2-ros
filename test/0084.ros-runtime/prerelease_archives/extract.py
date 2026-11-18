"""Verify and extract the linked talker and listener from one historical ROS 2 Linux release."""
import argparse
import bz2
import collections
import hashlib
import io
import json
from pathlib import Path, PurePosixPath
import re
import subprocess
import tarfile
import tempfile
import urllib.request


HERE = Path(__file__).resolve().parent
ARCHIVES = json.loads((HERE / "archives.json").read_text())
MIB = 1024 * 1024


def member_digest(members):
    fingerprints = {name: member["sha256"] for name, member in members.items()}
    return hashlib.sha256(json.dumps(fingerprints, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def extract(release, source, stage):
    pin = ARCHIVES[release]
    if not 0 < pin["bytes"] <= 96 * MIB:
        raise ValueError("invalid release size")
    with source as stream:
        data = stream.read(pin["bytes"] + 1)
    if len(data) != pin["bytes"] or hashlib.sha256(data).hexdigest() != pin["sha256"]:
        raise ValueError("release archive differs from its recorded bytes")
    # Random access within the tar avoids decompressing the same archive for each dependency.
    with bz2.BZ2File(io.BytesIO(data)) as packed:
        payload = packed.read(896 * MIB + 1)
    if len(payload) > 896 * MIB:
        raise ValueError("tar expansion exceeds 896 MiB")
    with tarfile.open(fileobj=io.BytesIO(payload), mode="r:") as archive:
        return closure(archive, release, stage)


def closure(archive, release, root):
    by_name = collections.defaultdict(list)
    for index, member in enumerate(archive):
        if index >= 65536:
            raise ValueError("too many tar members")
        if member.isfile() or member.issym() or member.islnk():
            by_name[PurePosixPath(member.name).name].append(member)
    def resolve(name):
        matches = by_name.get(name, [])
        if len(matches) != 1:
            raise ValueError(f"expected one archive member named {name}, found {len(matches)}")
        return matches[0]

    todo = ["talker", "listener"]
    members = {}
    missing = set()
    total = 0
    while todo:
        name = todo.pop()
        if name in members:
            continue
        if not re.fullmatch(r"[A-Za-z0-9_+.\-]{1,192}", name):
            raise ValueError("invalid ELF dependency name")
        member = resolve(name)
        # Tarfile resolves links internally. Only bytes are copied to fixed basenames.
        with archive.extractfile(member) as stream:
            body = stream.read(64 * MIB + 1)
        total += len(body)
        if len(body) > 64 * MIB or total > 112 * MIB or len(members) >= 256:
            raise ValueError("selected executable closure exceeds the bound")
        if not body.startswith(b"\x7fELF\x02\x01"):
            raise ValueError("expected a little-endian ELF64 member")
        path = root / name
        path.write_bytes(body)
        path.chmod(0o755)
        table = subprocess.check_output(["/usr/bin/readelf", "-d", path], env={"LC_ALL": "C"}, text=True)
        needed = re.findall(r"\(NEEDED\)\s+Shared library: \[([^\]]+)\]", table)
        members[name] = {"path": member.name, "bytes": len(body), "sha256": hashlib.sha256(body).hexdigest(), "needed": needed}
        for library in needed:
            if library in by_name:
                todo.append(library)
            else:
                missing.add(library)
    if member_digest(members) != ARCHIVES[release]["linked_elf_sha256"]:
        raise ValueError("selected ELF members differ from the recorded upstream release")
    return {"release": release, "asset": ARCHIVES[release], "bytes": total, "members": members,
            "external_sonames": sorted(missing)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("release", choices=ARCHIVES)
    parser.add_argument("--output", type=Path, required=True, help="new directory for this release")
    parser.add_argument("--archive", type=Path, help="use the local bytes of the pinned HTTPS asset")
    args = parser.parse_args()
    if args.output.exists():
        parser.error("select a fresh output directory")
    args.output.parent.mkdir(exist_ok=True, parents=True)
    source = args.archive.open("rb") if args.archive else urllib.request.urlopen(ARCHIVES[args.release]["url"], timeout=60)
    with tempfile.TemporaryDirectory(dir=args.output.parent) as temporary:
        stage = Path(temporary) / "closure"
        stage.mkdir()
        receipt = extract(args.release, source, stage)
        (stage / "closure.json").write_text(json.dumps(receipt, indent=2) + "\n")
        stage.rename(args.output)


if __name__ == "__main__":
    main()
