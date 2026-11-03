"""Derive recorded ROS copies of shared tests after removal of stack selection.

The original shared/upstream test files stay unchanged. Only the exact removed
argv pairs and their matrix labels change. Production src/dependencies and tools
are read through symlinks to the current admitted source tree.
"""
from __future__ import annotations

import json
from pathlib import Path
import shutil

from qualification_support import require, sha


TRANSFORMS = {
    "test/0017.runtime/run_debug_source_step_cli.py": [
        ("'-Rllvm-call-stack', policy,", "", 1),
        ("for policy in ('instruction', 'unwind'):", "for policy in ('mandatory-native',):", 1),
        ("tools = {name: getattr(args, name).resolve(strict=True) for name in ('wasm_clang', 'wasm_ld', 'rustc', 'wasm_tools', 'llvm_dwarfdump')}",
         "tools = {name: getattr(args, name).absolute() for name in ('wasm_clang', 'wasm_ld', 'rustc', 'wasm_tools', 'llvm_dwarfdump')}\n    for tool in tools.values():\n        tool.resolve(strict=True)", 1),
    ],
    "test/0017.runtime/run_debug_language_experience_cli.py": [
        ("'-Rllvm-call-stack', policy,", "", 1),
        ("args.diagnostic_policy or ('instruction', 'unwind')", "args.diagnostic_policy or ('mandatory-native',)", 3),
    ],
    "test/0018.debugger/run_dap_current_broker.py": [
        ('"-Rllvm-call-stack", policy,', "", 1),
        ('for policy in ("instruction", "unwind"):', 'for policy in ("mandatory-native",):', 1),
    ],
    "test/0018.debugger/run_dap_frame_scope_lifetime.py": [
        ('"-Rllvm-call-stack", args.policy,', "", 1),
        ('choices=("instruction", "unwind")', 'choices=("mandatory-native",)', 1),
    ],
    "test/0018.debugger/run_wasm_blocked_wait_cli.py": [
        ('"-Rllvm-call-stack", policy,', "", 1),
        ('for policy in ("instruction", "unwind"):', 'for policy in ("mandatory-native",):', 1),
    ],
}

# CI's separately frozen current test closure has already migrated these two
# drivers. Retain its ordinary-upstream branch and record only the remaining
# wasm-ld argv[0] correction. Never apply an obsolete blanket string rewrite to
# a current driver or relabel an instruction result as mandatory native.
MIGRATED_MARKERS = {
    "test/0017.runtime/run_debug_source_step_cli.py": (
        "for policy in (('mandatory-native',) if args.ros else ('instruction', 'unwind')):",
        "*([] if args.ros else ['-Rllvm-call-stack', policy])"),
    "test/0017.runtime/run_debug_language_experience_cli.py": (
        "args.diagnostic_policy = product_policies(parser, args.ros, args.diagnostic_policy)",
        "*([] if args.ros else ['-Rllvm-call-stack', policy])"),
}


def prepare(root: Path, out: Path):
    """Linux caller must have admitted its original root before copying."""
    out.mkdir(parents=True, exist_ok=False)
    for directory in ("src", "third-parties", "tools"):
        (out / directory).symlink_to(root / directory, target_is_directory=True)
    (out / "test").mkdir()
    for directory in ("0017.runtime", "0018.debugger"):
        shutil.copytree(root / "test" / directory, out / "test" / directory,
                        ignore=shutil.ignore_patterns("__pycache__", ".DS_Store", "._*"))
    rows = []
    for relative, changes in TRANSFORMS.items():
        source, derived = root / relative, out / relative
        original = source.read_text()
        text = original
        route = "derived legacy ROS driver"
        if relative in MIGRATED_MARKERS and "from mandatory_native_cli import" in text:
            require(all(text.count(marker) == 1 for marker in MIGRATED_MARKERS[relative]),
                    "current mandatory-native driver changed; review exact route", relative)
            changes = TRANSFORMS[relative][2:] if relative.endswith("run_debug_source_step_cli.py") else []
            route = "current CI mandatory-native ROS route; ordinary-upstream branch retained"
        for old, new, count in changes:
            require(text.count(old) == count, "shared harness changed; review ROS derivation before execution", (relative, old, text.count(old), count))
            text = text.replace(old, new)
        if route == "derived legacy ROS driver":
            require("-Rllvm-call-stack" not in text, "removed ROS public stack option remains", relative)
        derived.write_text(text)
        rows.append({"route": route, "original": str(source), "original_sha256": sha(source),
                     "derived": str(derived), "derived_sha256": sha(derived),
                     "exact_substitutions": [{"old": old, "new": new, "count": count} for old, new, count in changes]})
    pins = {str(path): sha(path) for path in sorted((out / "test").rglob("*")) if path.is_file()}
    receipt = {"product": "uwvm2-ros", "public_stack_option": False,
               "runtime_policy": "mandatory checked native unwind and crash stack",
               "production_source_link": str(root / "src"), "rows": rows, "derived_test_inputs": pins}
    (out / "derivation.json").write_text(json.dumps(receipt, indent=2) + "\n")
    return receipt
