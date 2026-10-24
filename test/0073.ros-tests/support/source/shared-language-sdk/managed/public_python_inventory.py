#!/usr/bin/env python3
"""@brief Inventory authored Python public declarations using the actual parser.

@details Parses sources without importing SDK/application code. Every entry binds
its source SHA, AST location, parameters and attached docstring/attribute comment.
Explicit reexports and returned private types are visible. Missing text, unknown
dynamic exports and unresolved references remain gaps; this is never a semantic,
formatter, compiler-artifact or Guest qualification by itself. The original
admitted Linux finite owner is required.
@see ../../../../../../sdk/origin/language/managed/EXAMPLES.md
"""

import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import sys


def _pin(path):
    raw = path.read_bytes()
    return dict(path=str(path.resolve(strict=True)), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def _name(node):
    if isinstance(node, ast.Name):
        return node.id
    if isinstance(node, ast.Attribute):
        return _name(node.value) + "." + node.attr
    if isinstance(node, ast.Call):
        return _name(node.func)
    return ""


def _documentation(text):
    tags = {}
    for line in (text or "").splitlines():
        line = line.strip()
        if line.startswith("@"):
            tag, _, value = line[1:].partition(" ")
            tags.setdefault(tag, []).append(value)
    return dict(text=text, tags=tags, missing=[name for name in ("brief", "details", "see") if name not in tags],
                semantic_review="PENDING: source-specific human review; tag presence is not qualification")


def _main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, required=True)
    parser.add_argument("--module", required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--returned-type", action="append", default=[])
    parser.add_argument("--require-complete", action="store_true")
    args = parser.parse_args()
    if os.uname().sysname != "Linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("the original admitted Linux finite owner is required")
    root = args.root.resolve(strict=True)
    args.out.mkdir(parents=True, exist_ok=False)
    entries, modules, exports, unresolved, private = [], [], [], [], []

    def declaration(module, path, pin, node, name, kind, text, **extra):
        item = dict(id=module + "." + name if name else module, kind=kind, source=pin,
                    location=dict(line=node.lineno, column=node.col_offset, end_line=node.end_lineno,
                                  end_column=node.end_col_offset), documentation=_documentation(text), **extra)
        entries.append(item)
        return item

    def attribute_doc(body, index):
        following = body[index + 1] if index + 1 < len(body) else None
        return following.value.value if (isinstance(following, ast.Expr) and
            isinstance(following.value, ast.Constant) and isinstance(following.value.value, str)) else None

    for path in sorted(root.rglob("*.py")):
        relative = path.relative_to(root)
        parts = list(relative.with_suffix("").parts)
        if parts[-1] == "__init__":
            parts.pop()
        module = ".".join([args.module, *parts])
        if any(part.startswith("_") and part != "__init__" for part in relative.with_suffix("").parts):
            private.append(dict(source=_pin(path), reason="private module; public reexports are recorded separately"))
            continue
        raw = path.read_bytes()
        if len(raw) > 8 << 20:
            raise ValueError("bounded ordinary Python API source required")
        tree = ast.parse(raw, filename=str(path), type_comments=True)
        pin = _pin(path)
        modules.append(dict(id=module, source=pin, module_documentation=_documentation(ast.get_docstring(tree)),
                            parser="CPython ast.parse; no application import/execution"))
        explicit = None
        for node in tree.body:
            if isinstance(node, ast.Assign) and any(isinstance(target, ast.Name) and target.id == "__all__" for target in node.targets):
                try:
                    value = ast.literal_eval(node.value)
                except (ValueError, TypeError):
                    value = None
                if not isinstance(value, (list, tuple)) or not all(isinstance(name, str) for name in value):
                    unresolved.append(dict(module=module, line=node.lineno, reason="dynamic __all__; not evaluated"))
                else:
                    explicit = set(value)
        for node in tree.body:
            if isinstance(node, ast.ImportFrom):
                prefix = "." * node.level + (node.module or "")
                for alias in node.names:
                    name = alias.asname or alias.name
                    if alias.name == "*":
                        unresolved.append(dict(module=module, line=node.lineno, reason="wildcard reexport requires source graph resolution"))
                    elif explicit is not None and name in explicit:
                        exports.append(dict(id=module + "." + name, from_module=prefix,
                                            imported_name=alias.name, source=pin, line=node.lineno))

        def visible(name):
            return name in explicit if explicit is not None else not name.startswith("_")

        def function(node, qualified, kind):
            positional = [*node.args.posonlyargs, *node.args.args]
            defaults = [None] * (len(positional) - len(node.args.defaults)) + list(node.args.defaults)
            params = [dict(name=arg.arg, annotation=ast.unparse(arg.annotation) if arg.annotation else None,
                           default=ast.unparse(default) if default is not None else None,
                           kind="positional-only" if index < len(node.args.posonlyargs) else "positional-or-keyword")
                      for index, (arg, default) in enumerate(zip(positional, defaults))]
            params += [dict(name=arg.arg, annotation=ast.unparse(arg.annotation) if arg.annotation else None,
                            default=ast.unparse(default) if default is not None else None, kind="keyword-only")
                       for arg, default in zip(node.args.kwonlyargs, node.args.kw_defaults)]
            for arg, mode in ((node.args.vararg, "variadic-positional"), (node.args.kwarg, "variadic-keyword")):
                if arg:
                    params.append(dict(name=arg.arg, annotation=ast.unparse(arg.annotation) if arg.annotation else None, kind=mode))
            return declaration(module, path, pin, node, qualified, kind, ast.get_docstring(node),
                               parameters=params, returns=ast.unparse(node.returns) if node.returns else None,
                               decorators=[ast.unparse(item) for item in node.decorator_list],
                               asynchronous=isinstance(node, ast.AsyncFunctionDef))

        def class_body(node, qualified, exposure):
            declaration(module, path, pin, node, qualified, "type", ast.get_docstring(node), exposure=exposure,
                        bases=[ast.unparse(base) for base in node.bases], decorators=[ast.unparse(item) for item in node.decorator_list])
            for index, child in enumerate(node.body):
                if isinstance(child, (ast.FunctionDef, ast.AsyncFunctionDef)):
                    if not child.name.startswith("_") or child.name in ("__init__", "__enter__", "__exit__", "__iter__", "__len__", "__getitem__", "__setitem__"):
                        function(child, qualified + "." + child.name, "property" if any(_name(dec) == "property" for dec in child.decorator_list) else "method")
                    if child.name == "__init__":
                        seen = set()
                        attached = {}
                        # PEP258 instance-attribute strings belong to the immediately
                        # preceding assignment in its own statement suite. A shared
                        # tuple assignment documents each of its declared attributes.
                        # Do not borrow a comment across an if/try/loop boundary.
                        for suite_owner in ast.walk(child):
                            for _, suite in ast.iter_fields(suite_owner):
                                if isinstance(suite, list) and all(isinstance(item, ast.stmt) for item in suite):
                                    for at, statement in enumerate(suite):
                                        if isinstance(statement, (ast.Assign, ast.AnnAssign)):
                                            attached[id(statement)] = attribute_doc(suite, at)
                        for assignment in ast.walk(child):
                            targets = assignment.targets if isinstance(assignment, ast.Assign) else [assignment.target] if isinstance(assignment, ast.AnnAssign) else []
                            for target in targets:
                                for attr in ast.walk(target):
                                    if (isinstance(attr, ast.Attribute) and isinstance(attr.value, ast.Name) and attr.value.id == "self"
                                            and not attr.attr.startswith("_") and attr.attr not in seen):
                                        seen.add(attr.attr)
                                        declaration(module, path, pin, attr, qualified + "." + attr.attr,
                                                    "instance-attribute", attached.get(id(assignment)),
                                                    assignment_location=dict(line=assignment.lineno, end_line=assignment.end_lineno),
                                                    classification="publicly named state; semantic API/internal classification pending")
                elif isinstance(child, ast.AnnAssign) and isinstance(child.target, ast.Name) and not child.target.id.startswith("_"):
                    declaration(module, path, pin, child, qualified + "." + child.target.id, "field", attribute_doc(node.body, index),
                                annotation=ast.unparse(child.annotation), default=ast.unparse(child.value) if child.value else None)

        for index, node in enumerate(tree.body):
            if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef)) and visible(node.name):
                function(node, node.name, "function")
                returned = {child.value.id for child in ast.walk(node) if isinstance(child, ast.Return) and isinstance(child.value, ast.Name)}
                for child in node.body:
                    if isinstance(child, ast.ClassDef) and child.name in returned:
                        class_body(child, node.name + "." + child.name, "factory-returned type")
            elif isinstance(node, ast.ClassDef) and (visible(node.name) or module + "." + node.name in args.returned_type):
                class_body(node, node.name, "declared public type" if visible(node.name) else "explicit returned private type")
            elif isinstance(node, (ast.Assign, ast.AnnAssign)):
                targets = node.targets if isinstance(node, ast.Assign) else [node.target]
                for target in targets:
                    if isinstance(target, ast.Name) and visible(target.id) and target.id != "__all__":
                        declaration(module, path, pin, node, target.id, "constant" if target.id.isupper() else "module-value",
                                    attribute_doc(tree.body, index), annotation=ast.unparse(node.annotation) if isinstance(node, ast.AnnAssign) else None)
    known = {entry["id"] for entry in entries} | {entry["id"] for entry in exports}
    for entry in entries:
        module = next(item["id"] for item in modules if item["source"] == entry["source"])
        references = []
        for target in entry["documentation"]["tags"].get("see", []):
            name = target.split()[0].strip("`") if target.split() else ""
            candidates = [name, module + "." + name, entry["id"].rsplit(".", 1)[0] + "." + name]
            matched = next((candidate for candidate in candidates if candidate in known), None)
            references.append(dict(target=name, resolved=matched, status="RESOLVED_DECLARATION" if matched else "UNRESOLVED"))
        entry["see_references"] = references
    missing = [dict(id=entry["id"], fields=entry["documentation"]["missing"]) for entry in entries if entry["documentation"]["missing"]]
    result = dict(format="task04-public-python-parser-inventory-v1", status="INVENTORIED_REVIEW_REQUIRED",
                  language="py", parser=dict(name="CPython ast", version=sys.version), source_root=str(root),
                  modules=modules, entries=entries, explicit_reexports=exports, private_modules=private,
                  unresolved_exports=unresolved, missing_documentation=missing,
                  unresolved_see=[dict(id=entry["id"], reference=ref) for entry in entries for ref in entry["see_references"] if ref["resolved"] is None],
                  source_hashes_rechecked=all(_pin(Path(item["source"]["path"])) == item["source"] for item in modules),
                  limitations=["Only this explicit package root is inventoried; add actual generated/compatibility package roots separately.",
                      "Dynamic assignments, implicit generated members and private-helper return reachability require manual classification.",
                      "Imported implementation dependencies are not counted as authored SDK declarations; explicit reexports are recorded."],
                  semantic_review="PENDING", complete_public_api_coverage=False, formatter_qualified=False,
                  compiler_artifact_credit=0, guest_credit=0, default_authority=[])
    (args.out / "inventory.json").write_text(json.dumps(result, indent=2) + "\n")
    if not result["source_hashes_rechecked"] or (args.require_complete and (missing or unresolved or result["unresolved_see"])):
        raise ValueError("source drift or incomplete documentation/export/reference binding")


if __name__ == "__main__":
    _main()
