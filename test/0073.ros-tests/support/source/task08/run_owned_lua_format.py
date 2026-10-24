"""Admitted tiny-job wrapper for actual owned Lua54 AST/format verification.

Reuses the unchanged genuine formatter package read-only. Extracts only a small
sealed source packet and runs Node as formatter tooling, never an SDK application.
One honest1MiB output allowance covers source, formatted files, report and logs.
"""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import zipfile


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
ACQUISITION = BASE / "04/round05-stylua-wasm-r1/work/formatter/acquisition.json"
ACQUISITION_SHA256 = "b3f3bee783667dbaa36d8c39c9f5f0932d90ee6dcbe7f763394c77f222f67f77"


def pin(path):
    with path.open("rb") as stream:
        return dict(path=str(path.resolve(strict=True)), bytes=path.stat().st_size,
                    sha256=hashlib.file_digest(stream, "sha256").hexdigest())


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original Linux finite common owner required")
    packet, formatter = map(Path, sys.argv[1:])
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    work.mkdir(exist_ok=True)
    source = work / "source"
    source.mkdir()
    with zipfile.ZipFile(packet) as archive:
        rows = archive.infolist()
        assert len(rows) <= 25 and len({row.filename for row in rows}) == len(rows)
        assert sum(row.file_size for row in rows) <= 390 << 10
        for row in rows:
            relative = Path(row.filename)
            assert not relative.is_absolute() and ".." not in relative.parts and not row.is_dir()
            assert (row.external_attr >> 16) & 0o170000 != 0o120000
            target = source / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(archive.read(row))
    sealed = [pin(path) for path in (packet, formatter, Path(__file__), ACQUISITION, Path("/usr/bin/node"))]
    report = dict(format="task08-real-owned-lua-format-supervisor-v1", status="STARTED",
                  sealed_inputs=sealed, commands=[], default_authority=[])

    def save():
        (work / "lua-format-supervisor.json").write_text(json.dumps(report, indent=2) + "\n")

    save()
    try:
        argv = ["/usr/bin/node", "--max-old-space-size=128", str(formatter), str(source),
                str(ACQUISITION), ACQUISITION_SHA256, str(work / "format-product")]
        completed = subprocess.run(argv, cwd=work, stdin=subprocess.DEVNULL,
                                   capture_output=True, timeout=75)
        assert len(completed.stdout) + len(completed.stderr) < 32 << 10
        for kind in ("stdout", "stderr"):
            (work / ("formatter." + kind)).write_bytes(getattr(completed, kind))
        report["commands"].append(dict(argv=argv, exit=completed.returncode,
                                       stdout=pin(work / "formatter.stdout"),
                                       stderr=pin(work / "formatter.stderr")))
        assert completed.returncode == 0, "genuine formatter failed; retained actual logs"
        result = json.loads((work / "format-product/lua-format.json").read_bytes())
        assert result["status"] == "PASS_SCOPED_ACTUAL_LUA54_AST_FORMAT_IDEMPOTENCE_ONLY"
        assert result["source_unchanged"] and result["tools_unchanged"]
        report["formatter_receipt"] = pin(work / "format-product/lua-format.json")
        report["status"] = result["status"]
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        report["sealed_inputs_unchanged"] = all(pin(Path(row["path"])) == row for row in sealed)
        if not report["sealed_inputs_unchanged"]:
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        save()


if __name__ == "__main__":
    main()
