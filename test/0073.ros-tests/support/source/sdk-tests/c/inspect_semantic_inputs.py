#!/usr/bin/env python3
"""Find captured native schema/IDL provenance for generated-C semantic review.

This inventories immutable inputs only. File existence, JSON keys and comments
do not qualify field meaning; the selected source bytes still require review.
"""

import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import sys
import tarfile


ARCHIVE_SHA256 = "9c4844cac8f47b3d98b0629f51d8fd72d3d85bab294c7d2353486c144d6acbab"


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    archive_path = run.parent / "c-matrix-r6/resource-archives/evidence.tar"
    digest = hashlib.sha256()
    with archive_path.open("rb") as stream:
        while block := stream.read(1 << 20):
            digest.update(block)
    if digest.hexdigest() != ARCHIVE_SHA256:
        raise RuntimeError("pinned captured source archive changed")
    families = set()
    for line in (run / "inputs/generated-headers-current.sha256").read_text().splitlines():
        _, spelling = line.split("  ", 1)
        families.add(PurePosixPath(spelling).parent.as_posix())
    rows = []
    samples = []
    with tarfile.open(archive_path) as outer:
        with tarfile.open(fileobj=outer.extractfile("inputs/source.tar")) as source:
            for member in source:
                path = PurePosixPath(member.name.removeprefix("./"))
                if not member.isfile() or path.parent.as_posix() not in families:
                    continue
                if path.suffix not in (".json", ".msg", ".srv", ".action", ".idl"):
                    continue
                if member.size > 16 * 1024 * 1024:
                    raise RuntimeError("captured schema exceeds bounded inspection scope")
                body = source.extractfile(member).read()
                row = {"path": path.as_posix(), "bytes": len(body),
                       "sha256": hashlib.sha256(body).hexdigest()}
                if path.suffix == ".json":
                    parsed = json.loads(body)
                    row["top_level_keys"] = list(parsed)[:40] if isinstance(parsed, dict) else []
                    if len(samples) < 3:
                        samples.append({"path": path.as_posix(), "utf8_prefix": body[:12000].decode()})
                rows.append(row)
    report = {"status": "SOURCE_METADATA_INVENTORY_ONLY", "source_archive_sha256": ARCHIVE_SHA256,
              "captured_header_families": len(families), "candidate_files": rows,
              "samples_for_human_review": samples, "physical_units_or_field_semantics_qualified": False}
    (run / "work/semantic-inputs.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({"status": report["status"], "families": len(families),
                      "candidate_files": len(rows)}), flush=True)


if __name__ == "__main__":
    main()
