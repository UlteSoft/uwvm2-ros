#!/usr/bin/env python3
"""Author a pinned post-reboot continuation without rewriting old evidence.

The selected immutable sources are verified before a finite reviewed set of
metadata-comparison functions is derived. Compiler recipes, language bodies,
Runtime authority and process supervision are retained. This tool only writes
fresh source; Root separately stages, admits and executes it in the real cgroup.
"""
from __future__ import annotations

import argparse
import ast
import hashlib
import json
from pathlib import Path


BUILDER_SHA = "0693fb2246605e0676ec0c165da2fd7a0972e9123f25a129f401a9b48a6af60e"
QUALIFIER_SHA = "91420bd98a6bf3e16dfafaa6d3f247822f5e25ea22f5cbbdbb3a169199cc1b69"
OLD_BOOT = "af121220-8c6e-4302-9853-f08bf4c50dbb"
NEW_BOOT = "ec0d5f0f-3060-4220-bee2-2a7791ed4e72"


def need(condition, reason):
    if not condition:
        raise ValueError(reason)


def sha(body):
    return hashlib.sha256(body).hexdigest()


class ObservationComparisons(ast.NodeTransformer):
    """Replace equality only within named historical-reference consumers.

    The comparison service leaves both operands untouched and returns false for
    any non-pin difference. It can bridge only a preselected historical file
    to a separately opened current full-seven observation.
    """
    def visit_Compare(self, node):
        self.generic_visit(node)
        if not all(isinstance(operator, ast.Eq) for operator in node.ops):
            return node
        values = [node.left, *node.comparators]
        calls = [ast.Call(func=ast.Attribute(value=ast.Name(id="POSTBOOT", ctx=ast.Load()),
                    attr="same", ctx=ast.Load()), args=[left, right], keywords=[])
                 for left, right in zip(values, values[1:])]
        result = calls[0] if len(calls) == 1 else ast.BoolOp(op=ast.And(), values=calls)
        return ast.copy_location(result, node)


def replace(text, before, after):
    need(text.count(before) == 1, "one exact reviewed source selection required: " + before[:110])
    return text.replace(before, after, 1)


def comparisons(text, names):
    tree = ast.parse(text)
    lines = text.splitlines(keepends=True)
    edits = []
    for name in names:
        nodes = [node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == name]
        need(len(nodes) == 1, "exact original historical consumer required: " + name)
        node = ObservationComparisons().visit(nodes[0])
        ast.fix_missing_locations(node)
        edits.append((nodes[0].lineno - 1, nodes[0].end_lineno,
                      "# Explicit post-reboot reference comparisons; raw documents remain unchanged.\n" + ast.unparse(node) + "\n"))
    for begin, end, body in sorted(edits, reverse=True):
        lines[begin:end] = [body]
    return "".join(lines)


def derive_qualifier(text):
    text = replace(text, "config[\"cgroup\"] == frame.config[\"cgroup\"] and",
        "config[\"cgroup\"] == POSTBOOT.namespace['HISTORICAL_CG'] and frame.config[\"cgroup\"] == POSTBOOT.namespace['CG'] and")
    text = replace(text,
        "        require({key: value for key, value in actual.items() if key != \"catalog\"} ==\n"
        "                {key: value for key, value in row.items() if key != \"catalog\"}, \"overlay original whole input root changed\")",
        "        require(POSTBOOT.tree_matches(row, actual), \"overlay original whole input root changed\")")
    text = replace(text,
        'return {"report": report_pin, "producer": report["producer"], "object": obj, "policy": POLICY,',
        'return {"report": report_pin, "producer": POSTBOOT.pin(report["producer"]), "object": POSTBOOT.pin(obj), "policy": POLICY,')
    return comparisons(text, ("reopen_pin", "official_sources", "qualify_overlay"))


def derive_builder(text, qualifier_sha, adapter_sha):
    text = replace(text, "CG={'memory_max':68719476736,", "HISTORICAL_CG={'memory_max':68719476736,")
    end = "'expected_cpus':'0-31'}\nEXPECTED="
    text = replace(text, end,
        "'expected_cpus':'0-31'}\nCG={**HISTORICAL_CG,'expected_inode':18315,'expected_boot_id':'" + NEW_BOOT + "'}\nEXPECTED=")
    text = replace(text, "CPP_QUALIFIER='" + QUALIFIER_SHA + "'", "CPP_QUALIFIER='" + qualifier_sha + "'")
    text = replace(text,
        "    exec(compile(raw,str(path),'exec'),module.__dict__);return module,pin",
        "    exec(compile(raw,str(path),'exec'),module.__dict__)\n"
        "    if frame is not None and hasattr(frame,'postboot'):module.POSTBOOT=frame.postboot\n"
        "    return module,pin")
    text = replace(text,
        "current[0]['content_sha256']==original[0]['content_sha256']",
        "POSTBOOT.tree_matches(original[0],current[0])")
    text = replace(text,
        "catalogs[0]['content_sha256']==nr['derived_tree']['content_sha256']",
        "POSTBOOT.tree_matches(nr['derived_tree'],catalogs[0])")
    text = replace(text, "old_profile in selection['outputs']", "POSTBOOT.member(old_profile,selection['outputs'])")
    text = replace(text,
        "      old_config['limits']==frame.limits and all(Path(root) in frame.roots for root in old_config['input_roots']),",
        "      old_config['limits']==frame.limits and all(Path(root) in frame.roots for root in old_config['input_roots']),")
    text = replace(text, "old_config['cgroup']==frame.config['cgroup'] and",
        "old_config['cgroup']==HISTORICAL_CG and frame.config['cgroup']==CG and")
    text = replace(text,
        "        need(len(matches)==1 and {k:v for k,v in matches[0].items() if k!='catalog'}==\n"
        "          {k:v for k,v in prior.items() if k!='catalog'},'whole original failed-batch input package changed')",
        "        need(len(matches)==1 and POSTBOOT.tree_matches(prior,matches[0]),'whole original failed-batch input package changed')")
    text = replace(text,
        "persistent[pin['path']]=pin", "persistent[pin['path']]=current")
    text = replace(text,
        "proof['entry_object'] in persistent.values()", "POSTBOOT.member(proof['entry_object'],list(persistent.values()))")
    text = replace(text, "js_config['cgroup']==CG and", "js_config['cgroup']==HISTORICAL_CG and")
    # The retained visitor becomes an explicit accessor, not a historical-pin
    # replacement. Frame.pin is unchanged and all new commands use current pins.
    tree = ast.parse(text)
    visitor = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == "retained_input")
    lines = text.splitlines(keepends=True)
    lines[visitor.lineno - 1:visitor.end_lineno] = [
        'def retained_input(frame,frozen,value):\n'
        '    """Reopen selected historical persistent references through the explicit bridge."""\n'
        '    frame.postboot.retained_input(value)\n']
    text = "".join(lines)
    text = comparisons(text, ("admit_rust", "admit_families", "qualify_rust_linker",
        "retained_batch", "retained_official_object", "build"))
    # Add a current physical sidecar while keeping every original receipt and
    # historical compilation observation exactly as recorded in the failed run.
    text = replace(text,
        "            frame.report['products'].append(product)",
        "            product['current_file_qualification']={'artifact':POSTBOOT.pin(product['artifact']),"
        "'receipt':POSTBOOT.pin(product['receipt']),'source':POSTBOOT.pin(product['source']),"
        "'original_receipt_unchanged':True,'Runtime_executed':False}\n"
        "            frame.report['products'].append(product)")
    text = replace(text,
        "        status=build(frame,frozen,args,utilities);frame.guard()",
        "        postboot_module,_=load(Path(__file__).with_name('typed-command-postboot.py'),'typed40_current_postboot'," +
        repr(adapter_sha) + ",frame)\n"
        "        frame.postboot=postboot_module.attach(frame,frozen,globals(),utilities)\n"
        "        status=build(frame,frozen,args,utilities);frame.guard()")
    text = replace(text,
        "            state['finalizing']=True\n            result=frame.finish(status,error)",
        "            state['finalizing']=True\n"
        "            if hasattr(frame,'postboot'):\n"
        "                try:frame.postboot.preserve()\n"
        "                except BaseException as caught:status='FAIL';error=repr(caught)\n"
        "            result=frame.finish(status,error)")
    return text


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    historical = Path(__file__).resolve().parent / "historical"
    parser.add_argument("--builder", type=Path,
        default=historical / "command14-schema-fixed-0693fb.py.txt")
    parser.add_argument("--qualifier", type=Path,
        default=historical / "official-cpp-qualifier-91420b.py.txt")
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    raw = args.builder.read_bytes()
    qualifier = args.qualifier.read_bytes()
    need(sha(raw) == BUILDER_SHA and sha(qualifier) == QUALIFIER_SHA,
         "exact schema-fixed builder and immutable original qualifier required")
    adapter_path = Path(__file__).resolve().with_name("typed_command_postboot.py")
    adapter = adapter_path.read_bytes()
    new_qualifier = derive_qualifier(qualifier.decode("utf-8", "strict")).encode()
    builder = derive_builder(raw.decode("utf-8", "strict"), sha(new_qualifier), sha(adapter)).encode()
    ast.parse(new_qualifier)
    ast.parse(builder)
    out = args.out.resolve()
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    outputs = {"command14-builder.py": builder, "official-cpp-qualifier.py": new_qualifier,
               "typed-command-postboot.py": adapter}
    records = []
    for name, body in outputs.items():
        path = out / name
        path.write_bytes(body)
        path.chmod(0o700)
        records.append({"path": str(path), "bytes": len(body), "sha256": sha(body)})
    receipt = {"kind": "typed40-postboot-source-derivation", "status": "SOURCE_NOT_EXECUTED",
        "original_builder_sha256": BUILDER_SHA, "original_qualifier_sha256": QUALIFIER_SHA,
        "outputs": records, "old_JSON_rewritten": False, "Frame_pin_returns_current": True,
        "current_boot": NEW_BOOT, "current_cgroup_inode": 18315,
        "compilers_started": False, "tests_executed": False, "Runtime_executed": False}
    (out / "source-derivation.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps(receipt, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
