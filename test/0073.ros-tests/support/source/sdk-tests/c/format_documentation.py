#!/usr/bin/env python3
"""Stage and verify owned C formatting/documentation without publishing files."""

import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile


FORMATTER = Path("/home/macromodel/Documents/uwvm-test-261018/agents/02/c-formatter-tools-r1/work/formatter/usr/lib/llvm-22/bin/clang-format")
FORMATTER_SHA256 = "0a8a34e84b31ce046ff93e191cffa89801a07defd8e1500b438e142cf21bab4e"
MODES = ("c89", "c90", "iso9899:199409", "c99", "c11", "c17", "c23", "c2x", "c2y")


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs = run / "inputs"
    root = run / "work/source"
    root.mkdir()
    with tarfile.open(inputs / "source.tar") as archive:
        archive.extractall(root, filter="data")
    originals = {relative: (root / relative).read_bytes()
                 for relative in (inputs / "c-format-files.txt").read_text().splitlines()}
    if digest(FORMATTER) != FORMATTER_SHA256:
        raise RuntimeError("exact syntax-aware formatter changed")
    commands = []

    def execute(label, argv, data=None):
        result = subprocess.run([str(arg) for arg in argv], input=data,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                timeout=90, check=False)
        commands.append({"label": label, "argv": [str(arg) for arg in argv],
                         "exit_code": result.returncode,
                         "stdout_sha256": hashlib.sha256(result.stdout).hexdigest(),
                         "stderr_sha256": hashlib.sha256(result.stderr).hexdigest()})
        if result.returncode:
            (run / "work/failure.stderr").write_bytes(result.stderr)
            raise RuntimeError("failed " + label)
        return result.stdout

    version = execute("formatter-version", [FORMATTER, "--version"]).decode().strip()
    documented = run / "work/documented"
    execute("clang-documentation-inventory", [sys.executable, "-I", "-S", "-B",
        root / "tools/c_api_documentation.py", "--root", root, "--output", documented,
        "--files", inputs / "c-format-files.txt", "--clang",
        "/home/macromodel/Documents/uwvm-test-261018/agents/10/staged-wasi-r1/staged/wasi-sdk/bin/clang",
        "--target", "wasm32-wasip1", "--sysroot",
        "/home/macromodel/Documents/uwvm-test-261018/agents/10/staged-wasi-r1/staged/wasi-sdk/share/wasi-sysroot"])
    inventory = json.loads((documented / "documentation-inventory.json").read_bytes())
    for row in inventory["files"]:
        if digest(root / row["path"]) != row["before_sha256"]:
            raise RuntimeError("source changed after declaration inventory")
        shutil.copyfile(documented / row["path"], root / row["path"])
    # The final source tree and inventory retain every candidate. Retire this
    # duplicate pre-format scratch before compiler checks and the final receipt.
    shutil.rmtree(documented)
    spec = importlib.util.spec_from_file_location("owned_c_tokens", root / "tools/c_profiles.py")
    tokens = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(tokens)

    def significant(data):
        # C translation phase2 removes escaped newlines before tokenization.
        # clang-format can wrap a macro without changing any preprocessing token.
        logical = re.sub(r"\\\r?\n", "", data.decode())
        return [token[0] for token in tokens.TOKENS.finditer(logical)
                if not token[0].startswith(("//", "/*"))]

    rows = []
    for relative in (inputs / "c-format-files.txt").read_text().splitlines():
        path = root / relative
        original = path.read_bytes()
        argv = [FORMATTER, "--style=file:" + str(root / "c/.clang-format"),
                "--assume-filename=" + str(path)]
        formatted = execute("format-" + relative, argv, original)
        passes = 1
        stable = False
        while passes <= 3:
            repeated = execute("idempotence-" + str(passes) + "-" + relative, argv, formatted)
            if repeated == formatted:
                stable = True
                break
            formatted = repeated
            passes += 1
        if significant(originals[relative]) != significant(formatted):
            (run / "work/token-failure-before.c").write_bytes(original)
            (run / "work/token-failure-after.c").write_bytes(formatted)
            raise RuntimeError("formatter changed a noncomment C token: " + relative)
        if not stable:
            (run / "work/idempotence-failure.c").write_bytes(formatted)
            raise RuntimeError("formatter is not byte-idempotent: " + relative)
        path.write_bytes(formatted)
        rows.append({"path": relative, "input_sha256": hashlib.sha256(original).hexdigest(),
                     "source_input_sha256": hashlib.sha256(originals[relative]).hexdigest(),
                     "output_sha256": digest(path), "significant_tokens_unchanged": True,
                     "documentation_and_formatter_tokens_unchanged": True,
                     "formatter_idempotent": True, "normalization_passes": passes})
    fixture = inputs / "portable_headers.c"
    for compiler in ("/usr/bin/gcc-15", "/usr/bin/clang-22"):
        for mode in MODES:
            execute("headers-" + Path(compiler).name + "-" + mode,
                    [compiler, "-std=" + mode, "-pedantic-errors", "-fsyntax-only",
                     "-I", root / "include", fixture])
    report = {"format": "task02-owned-c-format-and-documentation-v1", "formatter": str(FORMATTER),
              "formatter_sha256": FORMATTER_SHA256, "formatter_version": version,
              "configuration_sha256": digest(root / "c/.clang-format"),
              "files": rows, "commands": commands, "public_sdk_published": False,
              "header_syntax_checks": 18, "documentation_inventory": inventory,
              "documentation_semantic_review": "REQUIRED_BEFORE_APPLYING_CANDIDATES",
              "generated_public_families": "SEPARATE_TEMPLATE_AND_PUBLICATION_STAGE_REQUIRED",
              "status": "PASS_FORMAT_IDEMPOTENCE_TOKENS_AND_NATIVE_HEADER_SYNTAX"}
    (run / "work/format.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({"status": report["status"], "formatted_files": len(rows),
                      "documented_declarations": sum(len(row["declarations"]) for row in inventory["files"])}), flush=True)


if __name__ == "__main__":
    main()
