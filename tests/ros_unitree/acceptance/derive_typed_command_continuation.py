#!/usr/bin/env python3
"""Derive the two real schema repairs from the immutable reviewed continuation.

This is a source authoring tool, not a build/test launcher. It embeds the
maintained metadata predicate into a fresh copy of the exact recovered Source40
continuation. Root still has to admit the current boot/cgroup, every physical
input and the complete control closure. In particular, this tool deliberately
does not turn old full-seven pins or old cgroup facts into current identities.
The old report, builder and unsuccessful output directories are never edited.
"""
from __future__ import annotations

import argparse
import ast
import hashlib
import json
from pathlib import Path


ORIGINAL_SHA256 = "baa93383374811af5ff76bae8be1fd356b32c1c1fb9512a9a382cc03286b2fa9"
ORIGINAL_BYTES = 90333


def need(condition, reason):
    if not condition:
        raise ValueError(reason)


def pin(path, body):
    return {"path": str(path), "bytes": len(body), "sha256": hashlib.sha256(body).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument("--original", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    original = args.original.resolve(strict=True)
    output = args.out.absolute()
    need(output.resolve() == output and not output.exists() and not output.is_symlink(),
         "a fresh absolute output spelling is required")
    raw = original.read_bytes()
    need(len(raw) == ORIGINAL_BYTES and hashlib.sha256(raw).hexdigest() == ORIGINAL_SHA256,
         "the exact immutable recovered continuation is required")
    helper_path = Path(__file__).resolve().with_name("typed_command_retention.py")
    helper = helper_path.read_bytes()
    helper_text = helper.decode("utf-8", "strict")
    tree = ast.parse(helper_text, str(helper_path))
    lines = helper_text.splitlines(keepends=True)
    # This embeds only reviewed pure constants/functions. The generated source
    # needs no sibling import, ambient sys.path lookup or unpinned executable.
    included = []
    for node in tree.body:
        if isinstance(node, (ast.Import, ast.ImportFrom)) or (
                isinstance(node, ast.Expr) and isinstance(node.value, ast.Constant) and
                isinstance(node.value.value, str)):
            continue
        need(isinstance(node, (ast.Assign, ast.AnnAssign, ast.FunctionDef)),
             "the maintained predicate must contain only pure declarations")
        included.append("".join(lines[node.lineno - 1:node.end_lineno]))
    embedded = ("\n# Maintained exact historical-schema predicate; source provenance follows.\n" +
        "# " + json.dumps(pin(helper_path, helper), sort_keys=True) + "\n" +
        "\n".join(included) + "\n\n")
    text = raw.decode("utf-8", "strict")
    replacements = (
        ("from pathlib import Path\nsys.dont_write_bytecode=True\nMIB=1024**2\n",
         "from pathlib import Path,PurePosixPath\nsys.dont_write_bytecode=True\nMIB=1024**2\n"),
        ("def retained_batch(frame,frozen,catalog,families,utilities,inspector,schema,js_helper,js_config,values):\n",
         embedded + "def retained_batch(frame,frozen,catalog,families,utilities,inspector,schema,js_helper,js_config,values):\n"),
        ("    old_root=Path(pin['path']).parent\n    need(old_root in frame.roots and failed['kind']",
         "    schema_observation=admit_batch(failed,pin)\n"
         "    source_observation=bind_current_source(schema_observation,frame.source_before)\n"
         "    frame.report['retained_original_source_schema']=source_observation\n"
         "    old_root=Path(pin['path']).parent\n    need(old_root in frame.roots and failed['kind']"),
        ("      failed['source_before']==frame.source_before and failed['error']==",
         "      source_observation['status']=='CURRENT_COMPLETE_SOURCE_BYTES_MATCH_ORIGINAL_MANIFEST' and failed['error']=="),
        ("         bad[0]['returncode'] is None and 512*MIB<bad[0]['peak_owned_rss_bytes']<=768*MIB,",
         "         'returncode' not in bad[0] and 512*MIB<bad[0]['peak_owned_rss_bytes']<=768*MIB,"),
    )
    changes = []
    for before, after in replacements:
        need(text.count(before) == 1, "exact unique reviewed source replacement required: " + before[:90])
        text = text.replace(before, after, 1)
        changes.append({"before": before, "after": after,
                        "scope": "pure metadata schema repair; original runtime/physical gates retained"})
    body = text.encode("utf-8")
    ast.parse(text, str(output))
    output.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    with output.open("xb") as stream:
        stream.write(body)
    output.chmod(0o700)
    receipt = {"kind": "source-only-typed40-continuation-schema-derivation", "status": "SOURCE_NOT_EXECUTED",
        "original": pin(original, raw), "maintained_predicate": pin(helper_path, helper),
        "derived": pin(output, body), "changes": changes,
        "historical_reports_edited": False, "compiler_or_test_started": False,
        "postboot_physical_input_requalification": "STILL_REQUIRED_SEPARATELY",
        "current_cgroup_rebinding": "STILL_REQUIRED_SEPARATELY"}
    receipt_path = output.with_name(output.name + ".derivation.json")
    with receipt_path.open("x", encoding="utf-8") as stream:
        json.dump(receipt, stream, indent=2)
        stream.write("\n")
    print(json.dumps({"derived": receipt["derived"], "receipt": str(receipt_path)}, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
