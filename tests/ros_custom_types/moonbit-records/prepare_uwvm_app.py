#!/usr/bin/env python3
"""Adapt the maintained codec test bodies to a standalone WASIp1 application.

This is a source transformation, not a compiler, runner or resource supervisor.
The caller supplies separately generated records and owns the build directory.
Only test declarations and assert_true calls change; all assertion expressions,
golden bytes and malformed inputs remain in the resulting application.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re

CASES = {
    "fixture": ("records_test.mbt", 5),
    "joint1": ("joint_ros1_test.mbt", 1),
    "joint2": ("joint_ros2_test.mbt", 1),
}


def adapt(raw: bytes, catalog: str, *, mutate_golden: bool = False):
    filename, count = CASES[catalog]
    text = raw.decode("utf-8")
    names = re.findall(r'^test "([^"\r\n]+)" \{$', text, re.M)
    if len(names) != count or len(set(names)) != count:
        raise ValueError("unexpected maintained test declarations")
    assertions = len(re.findall(r"\bassert_true\(", text))
    if assertions == 0 or len(re.findall(r"\bassert_true\b", text)) != assertions:
        raise ValueError("unexpected assertion syntax")
    if "check_bool" in text or "uwvm_codec_case_" in text:
        raise ValueError("harness name collision")
    changed = []

    def declaration(match):
        index = len(changed)
        changed.append(match.group(1))
        return f"fn uwvm_codec_case_{index}() -> Unit {{"

    text = re.sub(r'^test "([^"\r\n]+)" \{$', declaration, text, flags=re.M)
    text = re.sub(r"\bassert_true\(", "check_bool(", text)
    if mutate_golden:
        anchor = 'let golden = b"\\x01\\xff\\x80'
        if catalog != "fixture" or text.count(anchor) != 1:
            raise ValueError("negative control requires the exact independent scalar golden")
        text = text.replace(anchor, 'let golden = b"\\x00\\xff\\x80', 1)
    markers = [f"PASS_CODEC_{catalog}_{index}" for index in range(count)]
    text += '\nfn check_bool(value : Bool) -> Unit {\n  if !value { abort("codec assertion failed") }\n}\n\nfn main {\n'
    for index, marker in enumerate(markers):
        text += f'  uwvm_codec_case_{index}()\n  @stdio.line_ascii("{marker}")\n'
    text += "}\n"
    result = text.encode("utf-8")
    return result, {
        "catalog": catalog,
        "source_filename": filename,
        "source_bytes": len(raw),
        "source_sha256": hashlib.sha256(raw).hexdigest(),
        "application_bytes": len(result),
        "application_sha256": hashlib.sha256(result).hexdigest(),
        "test_names": names,
        "test_count": count,
        "assertion_calls": assertions,
        "mutated_golden": mutate_golden,
        "expected_stdout": "" if mutate_golden else "".join(m + "\n" for m in markers),
        "expected_outcome": "unreachable trap before first completion marker" if mutate_golden else "all methods completed",
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("catalog", choices=CASES)
    parser.add_argument("output", type=Path, help="fresh application source file")
    parser.add_argument("--mutate-golden", action="store_true", help="fixture-only deliberate failure control")
    args = parser.parse_args()
    source = Path(__file__).resolve().parent / CASES[args.catalog][0]
    result, report = adapt(source.read_bytes(), args.catalog, mutate_golden=args.mutate_golden)
    with args.output.open("xb") as dest:
        dest.write(result)
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
