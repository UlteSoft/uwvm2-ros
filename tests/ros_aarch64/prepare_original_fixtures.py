#!/usr/bin/env python3
"""Prepare real Wasm/argv for eight unchanged original wasm3 native tests.

Run only after source freeze, serially in the current inherited 64 GiB/swap0
Linux lease and a separate mount namespace. This preparer assembles and fully
validates inputs; it never compiles or executes a native test or Wasm entry.
The resulting report is selected explicitly by the original-regression runner's
configuration field ``original_regression_fixtures_report``.

Memory WAT is generated from the original scripts' exact source-writing AST
parts. Their historical 64 GiB/CPU guards, argument parsing, compiler/reference
launches and main functions are not executed. The current shared Lane proves
64 GiB, process ownership, finite tool RSS/deadlines, preservation and ancestor
OOM counters. Authored Core 3 WAT and original JSON expectations are retained
unchanged. Invalid cells must assemble successfully and then receive exactly
the independent validator's type-mismatch refusal; a tool failure is no proof.

Required arguments are the common Lane source/manifest/SDK/out/QEMU/lease/
StartedAt arguments, plus absolute ``--wat2wasm`` and ``--wasm-tools`` ELF paths.
The SDK and QEMU are lease context only: no native SDK root is mounted or run.
"""
from __future__ import annotations

import argparse
import ast
import copy
import hashlib
from pathlib import Path
import subprocess
import sys
import traceback
from types import SimpleNamespace

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "ros_acceptance"))
from lane import Lane, add_arguments, atomic_json
from protocol import require
from runtime import elf

ORIGINAL = "test/0013.uwvm_int/wasm3/"
FIXTURES = "test/0017.runtime/fixtures/"
CASES = ("memory_performance", "memory_layout_probe", "memory_store_atomicity",
         "dead_codegen_model", "ref_join_type_preservation", "ref_branch_control_flow",
         "gc_two_phase_policy", "gc_two_phase_policy_declarations")


def sha(data):
    return hashlib.sha256(data).hexdigest()


def source_input(lane, relative):
    """Open one maintained original input through the complete frozen manifest."""
    require(relative in lane.frozen, "original fixture input absent from frozen source: " + relative)
    path = lane.source / relative
    opened = lane.pin(path)
    expected = lane.frozen[relative]
    require(isinstance(expected, dict) and expected.get("sha256") == opened["sha256"]
            and expected.get("bytes") == opened["bytes"], "original fixture source differs: " + relative)
    lane.report["source_inputs"][relative] = {"sha256": opened["sha256"], "bytes": opened["bytes"]}
    return path


def assignment(node, name):
    return (isinstance(node, ast.Assign) and any(isinstance(t, ast.Name) and t.id == name
                                              for t in node.targets))


class WritingParts(ast.NodeTransformer):
    """Remove only the explicitly known original compiler/reference launches.

    This is an extraction of source-generation parts, not a monkeypatch of the
    old test's guards or subprocess API. The original script is never imported.
    All retained AST nodes are copied, then recorded with their exact digest.
    """
    def visit_If(self, node):
        if any(isinstance(n, ast.Attribute) and n.attr == "wasmtime" for n in ast.walk(node.test)):
            return None
        return self.generic_visit(node)

    def visit_Expr(self, node):
        call = node.value
        if (isinstance(call, ast.Call) and isinstance(call.func, ast.Attribute)
                and isinstance(call.func.value, ast.Name) and call.func.value.id == "subprocess"
                and call.func.attr == "run"):
            return None
        return self.generic_visit(node)


def execute_writing_ast(nodes, namespace, filename):
    """Execute only a frozen, closed source-writing AST in this preparer process."""
    tree = ast.fix_missing_locations(ast.Module(body=nodes, type_ignores=[]))
    require(not any(isinstance(n, (ast.Import, ast.ImportFrom)) for n in ast.walk(tree)),
            "imports are not source-writing parts")
    require(not any(isinstance(n, ast.Name) and n.id in
                    {"subprocess", "os", "sys", "argparse", "exec", "eval", "__import__"}
                    for n in ast.walk(tree)), "launch/guard/evaluation body leaked into generation AST")
    encoded = ast.dump(tree, include_attributes=False).encode("utf-8")
    exec(compile(tree, filename, "exec"), namespace)
    return sha(encoded)


def memory_sources(lane):
    """Generate all original scalar/SIMD, independent-read and atomicity shapes."""
    result = {"performance": [], "atomicity": []}
    path = source_input(lane, ORIGINAL + "memory_performance_cases.py")
    tree = ast.parse(path.read_text(encoding="utf-8"), filename=str(path))
    main = [n for n in tree.body if isinstance(n, ast.FunctionDef) and n.name == "main"]
    pattern = [n for n in tree.body if isinstance(n, ast.FunctionDef) and n.name == "pattern_byte"]
    require(len(main) == len(pattern) == 1, "original memory generator function inventory differs")
    body = main[0].body
    begin = [i for i, n in enumerate(body) if assignment(n, "kernels")]
    end = [i for i, n in enumerate(body) if isinstance(n, ast.For)
           and isinstance(n.target, ast.Name) and n.target.id == "kernel"]
    require(len(begin) == len(end) == 1 and begin[0] < end[0], "original memory write region differs")
    nodes = [copy.deepcopy(pattern[0]), *copy.deepcopy(body[begin[0]:end[0] + 1])]
    nodes = [WritingParts().visit(n) for n in nodes]
    directory = lane.out / "memory-performance"
    directory.mkdir()
    ast_sha = execute_writing_ast(nodes, {"args": SimpleNamespace(
        output=directory, include_independent_loads=True, wasmtime=None)}, str(path))
    paths = sorted(directory.glob("*.wat"))
    wanted = {kernel + "-" + syntax + ".wat" for kernel in
              ("scalar-aligned", "scalar-unaligned", "simd-aligned", "load-scalar-aligned",
               "load-scalar-unaligned", "load-simd-aligned") for syntax in ("legacy", "indexed")}
    require({p.name for p in paths} == wanted, "complete twelve original memory kernels required")
    result["performance"] = paths
    lane.report["source_derivations"].append({"source": str(path), "selected_ast_sha256": ast_sha,
        "method": "original in-process source-writing AST only; historical main/guard never executed",
        "explicit_options": {"include_independent_loads": True}, "wat_inputs": [lane.pin(p) for p in paths]})

    path = source_input(lane, ORIGINAL + "memory_store_atomicity_cases.py")
    tree = ast.parse(path.read_text(encoding="utf-8"), filename=str(path))
    begin = [i for i, n in enumerate(tree.body) if assignment(n, "cases")]
    end = [i for i, n in enumerate(tree.body) if isinstance(n, ast.For)
           and isinstance(n.target, ast.Tuple) and any(isinstance(t, ast.Name) and t.id == "op"
                                                      for t in n.target.elts)]
    require(len(begin) == len(end) == 1 and begin[0] < end[0], "original atomicity write region differs")
    nodes = [WritingParts().visit(copy.deepcopy(n)) for n in tree.body[begin[0]:end[0] + 1]]
    for memory64 in (False, True):
        directory = lane.out / ("memory-atomicity64" if memory64 else "memory-atomicity32")
        directory.mkdir()
        namespace = {"a": SimpleNamespace(output=directory, include_fused_inputs=True,
                                           memory64=memory64, wat2wasm=lane.args.wat2wasm)}
        ast_sha = execute_writing_ast(nodes, namespace, str(path))
        rows = namespace.get("rows")
        require(isinstance(rows, list) and len(rows) == 26
                and len({r["wasm"] for r in rows}) == 26, "all26 original fused/store shapes required")
        for row in rows:
            wat = Path(row["wasm"]).with_suffix(".wat")
            require(wat.parent == directory and wat.is_file() and row["width"] in (2, 4, 8, 16)
                    and row["memory64"] is memory64, "atomicity shape/path/width differs")
            result["atomicity"].append({**row, "wat": wat})
        lane.report["source_derivations"].append({"source": str(path), "selected_ast_sha256": ast_sha,
            "method": "original in-process source-writing AST only; historical main/guard never executed",
            "explicit_options": {"include_fused_inputs": True, "memory64": memory64},
            "wat_inputs": [lane.pin(Path(r["wasm"]).with_suffix(".wat")) for r in rows]})
    return result


def command(lane, label, argv, expected=0, diagnostic=None):
    """Keep exact actual host-tool argv/birth/logs, including required rejection."""
    child, row = lane.start(label, argv, stdin=subprocess.DEVNULL, seconds=120, rss=512 * 1024**2)
    row.update(role="fixture-validation" if "validate" in label else "fixture-assembly",
               expected_returncode=expected, diagnostic_contains=list(diagnostic or []),
               working_directory=str(Path.cwd()))
    lane.finish(child, row, expected=expected)
    if expected:
        text = lane.read(row) + lane.read(row, True)
        require(bool(text.strip()) and diagnostic and all(token in text for token in diagnostic),
                "independent invalid fixture diagnostic differs")
        row["status"] = "EXPECTED_REJECTION"
    lane.pins.extend((row["stdout_pin"], row["stderr_pin"]))
    lane.save()
    return row


def assemble(lane, wat, label, family="core3", valid=True):
    """Materialize a real core module, then run independent full validation.

    Core3 parse is intentionally separate from validation so original invalid
    control/type cells remain genuine bytes. No validation disable flag is
    accepted as a successful full-validation record.
    """
    wat = wat.resolve(strict=True)
    binary = wat.with_suffix(".wasm")
    require(not binary.exists(), "fixture output must be new")
    source_pin = lane.pin(wat)
    if family == "memory":
        argv = [str(lane.args.wat2wasm), "--enable-all", str(wat), "-o", str(binary)]
    else:
        argv = [str(lane.args.wasm_tools), "parse", str(wat), "-o", str(binary)]
    built = command(lane, label + "-assemble", argv)
    with binary.open("rb") as stream:
        header = stream.read(8)
    require(header == b"\0asm\x01\0\0\0", "real core Wasm binary header required")
    product = lane.pin(binary)
    validated = command(lane, label + "-validate",
                        [str(lane.args.wasm_tools), "validate", "--features", "all", str(binary)],
                        expected=0 if valid else 1, diagnostic=None if valid else ["type mismatch"])
    lane.report["fixtures"].append({"name": label, "wat": source_pin, "wasm": product,
        "assembly": built["name"], "validation": validated["name"],
        "full_validation_expectation": "VALID" if valid else "INVALID_TYPE_MISMATCH"})
    return binary


def copied_wat(lane, relative, directory):
    original = source_input(lane, relative)
    destination = directory / original.name
    require(not destination.exists(), "unique original authored fixture name required")
    destination.write_bytes(original.read_bytes())
    copy_pin, original_pin = lane.pin(destination), lane.pin(original)
    require(all(copy_pin[k] == original_pin[k] for k in ("sha256", "bytes")), "authored WAT copy differs")
    return destination


def run_spec(arguments, fixture_paths):
    return {"arguments": list(map(str, arguments)), "fixture_paths": list(map(str, fixture_paths))}


def first_type_prefix(binary):
    """Observe the actual first encoded type prefix after full validation.

    This small bounded framing reader is not a WebAssembly validator or a GC
    classifier. It prevents a parser/encoder which removes an explicit rec/sub
    wrapper from silently inheriting that authored cell's metadata expectation.
    Native tests still inspect their own full parsed declaration metadata.
    """
    data = binary.read_bytes()
    require(8 <= len(data) <= 16 * 1024**2, "bounded core module required")
    def u32(position, limit):
        value = 0
        for index in range(5):
            require(position < limit, "truncated section/type count")
            byte = data[position]
            position += 1
            require(index != 4 or byte <= 15, "section/type count exceeds u32")
            value |= (byte & 127) << (7 * index)
            if not byte & 128:
                return value, position
        raise ValueError("unterminated section/type count")
    position = 8
    while position < len(data):
        section = data[position]
        size, start = u32(position + 1, len(data))
        finish = start + size
        require(finish <= len(data), "section extends outside core module")
        if section == 1:
            count, cursor = u32(start, finish)
            require(not count or cursor < finish, "missing first encoded type")
            return None if not count else data[cursor]
        position = finish
    return None


def authored_sources(lane):
    """Use original closed JSON plans and embedded literal WAT expectations."""
    for key, plan, count in (("dead_codegen_model", "dead_codegen_model_matrix.json", 9),
                             ("ref_join_type_preservation", "ref_join_combine_delay_matrix.json", 11)):
        path = source_input(lane, ORIGINAL + plan)
        proof, _ = lane.json(path)
        rows = proof.get("fixtures")
        require(isinstance(rows, list) and len(rows) == count
                and len({r["file"] for r in rows}) == count, "original reference plan inventory differs")
        directory = lane.out / key
        directory.mkdir()
        argv, products = [], []
        for i, row in enumerate(rows):
            require(row.get("expectation") in ("valid", "invalid")
                    and row["file"].startswith(FIXTURES), "original normative expectation differs")
            wat = copied_wat(lane, row["file"], directory)
            binary = assemble(lane, wat, key + "-" + str(i), valid=row["expectation"] == "valid")
            argv += [row["expectation"], str(binary)]
            products.append(binary)
        lane.report["cases"][key] = {"runs": [run_spec(argv, products)],
            "scope": "all original fixture expectations; compiler macro-vector expansion is the native runner's separate scope"}

    path = source_input(lane, "test/0014.llvm_jit/run_ref_branches_cli.py")
    tree = ast.parse(path.read_text(encoding="utf-8"), filename=str(path))
    candidates = [n for n in tree.body if isinstance(n, ast.Assign)
                  and any(isinstance(t, ast.Attribute) and t.attr == "CASES" for t in n.targets)]
    require(len(candidates) == 1, "original embedded reference-branch dictionary required")
    rows = ast.literal_eval(candidates[0].value)
    require(isinstance(rows, dict) and len(rows) == 20, "all20 original branch source cells required")
    directory = lane.out / "ref_branch_control_flow"
    directory.mkdir()
    products = []
    for name, cell in rows.items():
        require(isinstance(cell, tuple) and len(cell) == 2 and cell[0] in ("success", "validation")
                and isinstance(cell[1], str), "closed embedded branch literal schema required")
        if cell[0] != "success":
            continue  # This native cc accepts only valid, no-result _start inputs.
        require(name and all(c.isalnum() or c == "-" for c in name), "closed branch fixture name required")
        wat = directory / (name + ".wat")
        wat.write_text(cell[1], encoding="utf-8")
        products.append(assemble(lane, wat, "ref-branch-" + name))
    require(len(products) == 11, "all eleven native-executable branch cells required")
    lane.report["cases"]["ref_branch_control_flow"] = {"runs": [run_spec(products, products)],
        "scope": "all11 original valid embedded _start cells; nine invalid CLI cells are not native cc inputs"}

    path = source_input(lane, ORIGINAL + "gc_two_phase_policy_plan.json")
    plan, _ = lane.json(path)
    rows = plan.get("fixtures")
    require(isinstance(rows, list) and len(rows) == 14
            and len({r["wat"] for r in rows}) == 14, "all fourteen original GC policy cells required")
    directory = lane.out / "gc-two-phase"
    directory.mkdir()
    argv, products = [], []
    for i, row in enumerate(rows):
        require(row["validation_gc_off"] in ("accept", "reject")
                and type(row["encoded_type_section_requires_gc"]) is bool
                and row["fully_enabled_oracle_expected"] == "valid", "original GC normative schema differs")
        wat = copied_wat(lane, row["wat"], directory)
        binary = assemble(lane, wat, "gc-policy-" + str(i))
        prefix = first_type_prefix(binary)
        if row["name"] == "recursive_group":
            require(prefix == 0x4e, "official encoder did not preserve authored explicit rec wrapper")
        elif row["name"] == "subtype_wrapper":
            require(prefix == 0x50, "official encoder did not preserve authored nonfinal subtype wrapper")
        lane.report["fixtures"][-1]["encoded_first_type_prefix"] = prefix
        argv += [row["validation_gc_off"], "type-gc" if row["encoded_type_section_requires_gc"] else "type-no-gc", str(binary)]
        products.append(binary)
    lane.report["cases"]["gc_two_phase_policy"] = {"runs": [run_spec(argv, products)],
        "scope": "original14 fully enabled valid cells; narrowed GC normative mismatches must fail actual native tests"}
    declarations_argv, declarations_products = list(argv), list(products)
    # These additional authored sources correspond exactly to the declaration
    # runner's retained signature/local/import checks. The native runner observes
    # encoded metadata; these expectations never grant a before-fix mismatch PASS.
    extra = (("signature_any", "reject", "type-gc"),
             ("signature_exn", "accept", "type-no-gc"),
             ("nondefaultable_any", "reject", "type-no-gc"))
    for name, expected, type_expected in extra:
        wat = copied_wat(lane, FIXTURES + "gc_two_phase_" + name + ".wat", directory)
        binary = assemble(lane, wat, "gc-declarations-" + name)
        declarations_argv += [expected, type_expected, str(binary)]
        declarations_products.append(binary)
    provider = assemble(lane, copied_wat(lane, FIXTURES + "gc_two_phase_import_provider.wat", directory), "gc-provider")
    imported_argv, imported_products = ["--provider", str(provider)], [provider]
    for name in ("import_global_any", "import_table_any"):
        wat = copied_wat(lane, FIXTURES + "gc_two_phase_" + name + ".wat", directory)
        binary = assemble(lane, wat, "gc-declarations-" + name)
        imported_argv += ["reject", "type-no-gc", str(binary)]
        imported_products.append(binary)
    lane.report["cases"]["gc_two_phase_policy_declarations"] = {
        "runs": [run_spec(declarations_argv, declarations_products), run_spec(imported_argv, imported_products)],
        "scope": "original14 plus3 authored declaration cells and2 imported declaration cells using actual provider bytes"}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    add_arguments(parser, workers=False)
    parser.add_argument("--wat2wasm", type=Path, required=True)
    parser.add_argument("--wasm-tools", type=Path, required=True)
    args = parser.parse_args()
    lane, status = None, "FAIL"
    try:
        lane = Lane(args, "original-regression-fixtures")
        require(Path(__file__).resolve() == lane.source / "tests/ros_aarch64/prepare_original_fixtures.py",
                "execute exact frozen original-fixture preparer")
        lane.report.update(mode="prepare", source_inputs={}, source_derivations=[], fixtures=[], cases={},
            actual_native_execution="NOT_PERFORMED", actual_Wasm_execution="NOT_PERFORMED",
            toolchain_input_closure="UNQUALIFIED", sdk_preservation_scope="NOT_USED_NO_SDK_ROOT_MOUNTED",
            implicit_permissions=[], parallelism=1)
        for label in ("wat2wasm", "wasm_tools"):
            path = getattr(args, label)
            require(path.is_absolute(), "explicit absolute tool path required")
            path = path.resolve(strict=True)
            require(elf(path)["e_machine"] == 62, "native x86_64 host tool required")
            setattr(args, label, path)
            lane.report.setdefault("tools", {})[label] = lane.pin(path)
        source_input(lane, "tests/ros_aarch64/prepare_original_fixtures.py")
        for key in CASES:
            source_input(lane, ORIGINAL + key + ".cc")
        source_input(lane, ORIGINAL + "memory_performance_reference.h")
        sources = memory_sources(lane)
        performance = []
        for wat in sources["performance"]:
            binary = assemble(lane, wat, "memory-" + wat.stem, family="memory")
            # The original independent reference selects these two counts,
            # crossing Wasm pages and wrapping the working set. This is checksum
            # regression input, not a native throughput/performance acceptance.
            performance += [run_spec([binary, count, 1], [binary]) for count in (1003, 20003)]
        lane.report["cases"]["memory_performance"] = {"runs": performance,
            "scope": "all12 original kernels at two original oracle counts, samples1; no throughput claim"}
        layout = lane.out / "memory-performance/scalar-unaligned-indexed.wasm"
        lane.report["cases"]["memory_layout_probe"] = {"runs": [run_spec([layout], [layout])],
            "scope": "one original page-crossing indexed diagnostic; immutable native main still performs336 fixed million-iteration samples; no performance acceptance"}
        atomicity = []
        for row in sources["atomicity"]:
            binary = assemble(lane, row["wat"], "atomicity-" + row["wat"].stem + ("-32" if not row["memory64"] else "-64"), family="memory")
            for growth in (False, True):
                argv = [binary, row["width"], *(["growth"] if growth else []), *(["memory64"] if row["memory64"] else [])]
                atomicity.append(run_spec(argv, [binary]))
        lane.report["cases"]["memory_store_atomicity"] = {"runs": atomicity,
            "scope": "all26 original fused/store shapes × memory32/64 × growth0/1; native checks remain original"}
        authored_sources(lane)
        run_counts = {"memory_performance": 24, "memory_layout_probe": 1,
                      "memory_store_atomicity": 104, "dead_codegen_model": 1,
                      "ref_join_type_preservation": 1, "ref_branch_control_flow": 1,
                      "gc_two_phase_policy": 1, "gc_two_phase_policy_declarations": 2}
        require(set(lane.report["cases"]) == set(CASES)
                and all(len(lane.report["cases"][key]["runs"]) == count
                        for key, count in run_counts.items()), "closed eight/full135 actual fixture-fed entries required")
        require(len(lane.report["fixtures"]) == 115 and len(lane.report["commands"]) == 230
                and sum(row["status"] == "EXPECTED_REJECTION" for row in lane.report["commands"]) == 5,
                "all115 real Wasm/230 tool commands/five independent invalid type refusals required")
        lane.report["fixture_inventory"] = {"closed_native_sources": list(CASES),
            "native_argument_runs": run_counts, "total_native_argument_runs": 135,
            "prepared_Wasm_products": 115, "actual_assembly_commands": 115,
            "actual_full_validation_commands": 115, "expected_type_rejections": 5}
        derivation = lane.out / "source-derivations.json"
        atomic_json(derivation, {"status": "PREPARED", "method": "source-writing AST/unchanged authored WAT",
                                "source_derivations": lane.report["source_derivations"]})
        lane.report["source_derivation_receipt"] = lane.pin(derivation)
        # This owned fragment names the eventual finalized report. The trusted
        # parent merges it into a NEW native-runner config and pins that config;
        # this preparer never rewrites a prior Worker/configuration receipt.
        configuration = lane.out / "configuration.json"
        atomic_json(configuration, {"original_regression_fixtures_report": str(lane.out / "report.json")})
        lane.report["configuration_fragment"] = lane.pin(configuration)
        lane.report["new_tests_executed"] = False
        status = "PREPARED"
    except BaseException as error:
        if lane is None:
            raise
        lane.report.update(error=repr(error), traceback=traceback.format_exc())
    finally:
        if lane is not None:
            actual = lane.close("PASS" if status == "PREPARED" else "FAIL")
            status = "PREPARED" if status == "PREPARED" and actual == "PASS" else "FAIL"
            lane.report["status"] = status
            lane.save()
    return 0 if status == "PREPARED" else 1


if __name__ == "__main__":
    raise SystemExit(main())
