"""Format an explicit owned C/C++ source list using the genuine Linux tool."""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("common finite owner required")
    source, manifest_path = map(Path, sys.argv[1:])
    manifest = json.loads(manifest_path.read_text())
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    tool = Path(
        "/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang-format"
    )
    environment = dict(
        os.environ,
        LD_LIBRARY_PATH=(
            "/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu"
            ":/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu"
        ),
    )
    version = subprocess.run(
        [tool, "--version"],
        env=environment,
        stdin=subprocess.DEVNULL,
        capture_output=True,
        timeout=10,
        check=True,
    )
    (work / "formatter-version.stdout").write_bytes(version.stdout)
    (work / "formatter-version.stderr").write_bytes(version.stderr)
    rows = []
    for relative in manifest["cpp"]:
        original = source / relative
        assert original.is_file() and not original.is_symlink()
        output = work / "formatted" / relative
        output.parent.mkdir(parents=True, exist_ok=True)
        first_argv = [
            str(tool),
            "--style=" + manifest["clang_format_style"],
            str(original),
        ]
        first = subprocess.run(
            first_argv,
            env=environment,
            stdin=subprocess.DEVNULL,
            capture_output=True,
            timeout=10,
            check=True,
        )
        assert len(first.stdout) < 524288 and not first.stderr
        output.write_bytes(first.stdout)
        second_argv = [
            str(tool),
            "--style=" + manifest["clang_format_style"],
            str(output),
        ]
        second = subprocess.run(
            second_argv,
            env=environment,
            stdin=subprocess.DEVNULL,
            capture_output=True,
            timeout=10,
            check=True,
        )
        assert second.stdout == first.stdout and not second.stderr
        rows.append(
            dict(
                path=relative,
                before_sha256=sha(original),
                after_sha256=sha(output),
                bytes=output.stat().st_size,
                idempotent=True,
                first_argv=first_argv,
                second_argv=second_argv,
            )
        )
    report = dict(
        schema="uwvm-task08-round03-owned-cpp-format-v1",
        status="PASS_SCOPED_FORMAT",
        formatter=str(tool),
        formatter_sha256=sha(tool),
        version=version.stdout.decode().strip(),
        style=manifest["clang_format_style"],
        manifest_sha256=sha(manifest_path),
        inputs=rows,
        whole_sdk_formatting_complete=False,
        actual_build_cases=0,
        default_authority=[],
        private_candidate_only=True,
    )
    (work / "format-report.json").write_text(
        json.dumps(report, indent=2, sort_keys=True) + "\n"
    )
    print(
        json.dumps(
            dict(status=report["status"], formatted=len(rows), idempotent=len(rows))
        ),
        flush=True,
    )


if __name__ == "__main__":
    main()
