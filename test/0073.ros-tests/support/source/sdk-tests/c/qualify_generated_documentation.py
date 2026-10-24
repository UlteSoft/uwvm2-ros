#!/usr/bin/env python3
"""Check generated C documentation against all pinned raw headers, without compiling.

This is a bounded declaration/token/idempotence check. It does not qualify the
physical meaning of unknown native fields, a public SDK, or compiler behavior.
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import sys
import tarfile


ARCHIVE = Path("/home/macromodel/Documents/uwvm-test-261018/agents/02/c-matrix-r6/resource-archives/evidence.tar")
ARCHIVE_SHA256 = "9c4844cac8f47b3d98b0629f51d8fd72d3d85bab294c7d2353486c144d6acbab"


def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        while block := stream.read(1 << 20):
            value.update(block)
    return value.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--extra-root", type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs = run / "inputs"
    if digest(ARCHIVE) != ARCHIVE_SHA256:
        raise RuntimeError("immutable source archive changed")
    spec = importlib.util.spec_from_file_location("task02_c_profiles", inputs / "c_profiles.py")
    adapter = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(adapter)
    pins = []
    for line in (inputs / "generated-headers-current.sha256").read_text().splitlines():
        sha256, spelling = line.split("  ", 1)
        relative = Path(spelling)
        if relative.is_absolute() or ".." in relative.parts:
            raise RuntimeError("confined input path required")
        pins.append((relative, sha256))
    if len(pins) != 235:
        raise RuntimeError("235-header captured denominator changed")
    rows = []
    with tarfile.open(ARCHIVE) as outer:
        with tarfile.open(fileobj=outer.extractfile("inputs/source.tar"), mode="r:") as source:
            for relative, sha256 in pins:
                extra = args.extra_root / relative
                if extra.is_file():
                    raw = extra.read_bytes()
                else:
                    try:
                        member = source.getmember(relative.as_posix())
                    except KeyError:
                        member = source.getmember("./" + relative.as_posix())
                    if not member.isfile() or member.size > 16 * 1024 * 1024:
                        raise RuntimeError("ordinary bounded source header required")
                    raw = source.extractfile(member).read()
                if hashlib.sha256(raw).hexdigest() != sha256:
                    raise RuntimeError("captured source changed: " + relative.as_posix())
                row = {"path": relative.as_posix(), "input_sha256": sha256}
                try:
                    output, _ = adapter.transform(relative, "include/platform/candidate/c/" + relative.as_posix(), raw)
                    repeated = adapter._DOCUMENTATION.document_generated(output.decode()).encode()
                    if repeated != output:
                        raise ValueError("documentation byte idempotence failed")
                    row.update(status="PASS", output_sha256=hashlib.sha256(output).hexdigest(),
                               output_bytes=len(output), documentation_token_preservation=True,
                               documentation_byte_idempotence=True,
                               value_contracts=output.count(b"@par Generated C value contract"))
                except (ValueError, RuntimeError) as error:
                    row.update(status="FAIL", error=str(error))
                rows.append(row)
    failed = sum(row["status"] == "FAIL" for row in rows)
    report = {
        "format": "task02-generated-c-documentation-check-v1",
        "status": "FAIL" if failed else "PASS_DECLARATION_SCOPE",
        "counts": {"PASS": len(rows) - failed, "FAIL": failed},
        "headers": len(rows), "rows": rows,
        "source_archive_sha256": ARCHIVE_SHA256,
        "adapter_sha256": digest(inputs / "c_profiles.py"),
        "documentation_helper_sha256": digest(inputs / "c_api_documentation.py"),
        "native_field_semantics_reviewed_types": list(adapter._DOCUMENTATION.GENERATED_FIELD_SEMANTICS),
        "other_native_physical_meanings_qualified": False,
        "compiler_runs": 0, "runtime_runs": 0, "public_sdk_published": False,
    }
    (run / "work/generated-documentation.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({key: report[key] for key in ("status", "counts", "headers")}), flush=True)
    return bool(failed)


if __name__ == "__main__":
    sys.exit(main())
