#!/usr/bin/env python3
"""Acquire the upstream-documented exact StyLua npm/Wasm package on Linux.

Requires the previously acquired official registry metadata pin. Checks the
publisher's SHA512 integrity and SHA1 record before extracting only ordinary
confined files. This does not format sources or execute package scripts.
"""
import argparse
import base64
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import tarfile
import urllib.request


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--metadata", type=Path, required=True)
    parser.add_argument("--metadata-sha256", required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    if os.uname().sysname != "Linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("original admitted Linux finite owner required")
    raw_meta = args.metadata.read_bytes()
    if hashlib.sha256(raw_meta).hexdigest() != args.metadata_sha256:
        raise ValueError("previously inspected official registry metadata changed")
    meta = json.loads(raw_meta)
    url = "https://registry.npmjs.org/@johnnymorganz/stylua/-/stylua-2.6.0.tgz"
    if (meta["name"] != "@johnnymorganz/stylua" or meta["version"] != "2.6.0" or
            meta["repository"]["url"] != "git+https://github.com/JohnnyMorganz/StyLua.git" or
            meta["dist"]["tarball"] != url or meta.get("dependencies") or meta.get("optionalDependencies")):
        raise ValueError("review this exact dependency-free official package before use")
    scheme, digest = meta["dist"]["integrity"].split("-", 1)
    if scheme != "sha512":
        raise ValueError("one actual SHA512 package integrity pin required")
    with urllib.request.urlopen(url, timeout=25) as response:
        raw = response.read((4 << 20) + 1)
    if (len(raw) > 4 << 20 or hashlib.sha512(raw).digest() != base64.b64decode(digest, validate=True)
            or hashlib.sha1(raw).hexdigest() != meta["dist"]["shasum"]):
        raise ValueError("the original upstream package archive differs from official integrity")
    args.out.mkdir(parents=True, exist_ok=False)
    archive = args.out / "stylua-2.6.0.tgz"
    archive.write_bytes(raw)
    package = args.out / "package"
    package.mkdir()
    pins, total = [], 0
    with tarfile.open(archive) as stream:
        for item in stream.getmembers():
            relative = PurePosixPath(item.name)
            if relative.is_absolute() or ".." in relative.parts or relative.parts[0] != "package":
                raise ValueError("confined official npm archive required")
            if item.isdir():
                continue
            if not item.isfile() or item.size > 4 << 20:
                raise ValueError("ordinary bounded formatter package files required")
            total += item.size
            if total > 4 << 20:
                raise ValueError("bounded complete formatter package required")
            body = stream.extractfile(item).read()
            path = args.out.joinpath(*relative.parts)
            path.parent.mkdir(parents=True, exist_ok=True)
            with path.open("xb") as target:
                target.write(body)
            pins.append(dict(path=relative.as_posix(), bytes=len(body), sha256=hashlib.sha256(body).hexdigest()))
    if total != meta["dist"]["unpackedSize"] or len(pins) != meta["dist"]["fileCount"]:
        raise ValueError("actual complete package differs from registry extent/file count")
    if json.loads((package / "package.json").read_bytes())["version"] != "2.6.0":
        raise ValueError("actual formatter package version differs")
    report = dict(format="task04-official-stylua-wasm-acquisition-v1", status="ACQUIRED_NOT_FORMAT_QUALIFIED",
        version="2.6.0", registry_metadata_sha256=args.metadata_sha256, official_url=url,
        archive_bytes=len(raw), archive_sha256=hashlib.sha256(raw).hexdigest(), archive_integrity=meta["dist"]["integrity"],
        files=pins, unpacked_bytes=total, package_scripts_executed=False, formatting_runs=0, default_authority=[])
    (args.out / "acquisition.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
