#!/usr/bin/env python3
"""Mandatory complete SDK compiler/model supplement for final acceptance.

This Lane adapter runs the unchanged frozen sdk/origin/tools/run_local_checks.py in
two fresh, retained /tmp directories: all source/CMake/native/policy/Wasm groups
with six languages, both backends and every conditional example variant, then
scheduling models using that exact successful compiler report. Everything is
serial, with a512-MiB observed command-tree RSS guard and600s per inner command.
Each nested Python coordinator and its process-group samplers has a separate
64-MiB RSS ceiling; its complete tree has the corresponding576-MiB ceiling.
All other actors retain their512-MiB tree ceiling. Coordinator overhead never
increases a compiler/model command's512-MiB allowance.

This is explicitly host/compiler and mathematical-Wasm-model evidence. It
starts no ROS, Gateway, Worker, application Guest or native witness and grants
no capability. Zero ARM products is the correct architectural scope; a host
model/compiled example must never satisfy a ROS/ARM Runtime requirement.

--tools selects a closed JSON object: format_version=1,
kind=sdk-final-tool-selection, tools mapping python,cc,cxx,cmake,cargo,rustc,go,
node,wasi_sdk,tinygo,asc_js,wasm_tools to absolute existing paths, and tool_roots
listing absolute immutable package roots (or explicit standalone tool files).
All tools must be covered. WASI SDK/Rust/Go/Node/TinyGo/AS packages require
directory coverage, not just an executable hash. Native
CC/CXX/CMake/Python may be explicitly pinned standalone host tools; their
system-header/module/library closure remains UNQUALIFIED. Additional explicit
system inventories do not infer that unobserved OS dependencies were covered.
Every selected root is inventoried before and after; no
package manager, download, automatic permission or prior product reuse exists.

The original SDK reports, full raw command logs, Wasm bytes/imports/validation,
copied source/context manifests, pure/relay/scheduling receipts and current
tool package inventories are retained and pinned. No historical receipt is
edited and no compiler-only result is relabelled as ROS delivery.
"""
from __future__ import annotations

import argparse
import ast
import copy
import fnmatch
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import sys
import tempfile
import traceback
from types import SimpleNamespace

sys.dont_write_bytecode = True
from lane import Lane, add_arguments, atomic_json
from protocol import require
from runtime import elf, digest
from gateway_qualification import read_file_binding

LANGUAGES = ("c", "cpp", "rust", "tinygo", "assemblyscript", "javascript_bridge")
TOOLS = ("python", "cc", "cxx", "cmake", "cargo", "rustc", "go", "node", "wasi_sdk", "tinygo", "asc_js", "wasm_tools")
PACKAGE_TOOLS = ("cargo", "rustc", "go", "node", "wasi_sdk", "tinygo", "asc_js", "wasm_tools")
SCOPE = "sdk-compiler-model-supplement"
RSS = 512 * 1024**2
COORDINATOR_RSS = 64 * 1024**2
PROFILE_TESTS = (
    "test_complete_shapes_and_empty_service", "test_ros1_explicit_dynamic_bounds",
    "test_unbounded_string_refused", "test_unbounded_sequence_refused",
    "test_narrower_limits_allowed_but_not_native_expansion", "test_unused_or_wrong_kind_bound_refused",
    "test_unknown_nested_type_refused", "test_recursive_nested_type_refused",
    "test_component_not_a_catalog_topic", "test_duplicate_interface_and_member_refused",
    "test_empty_topic_refused", "test_defaults_constants_aliases_wide_and_idl_refused",
    "test_bound_literal_and_expansion_ceilings", "test_component_official_identities_required_for_native_emission",
    "test_canonical_fingerprint_is_bound_and_version_sensitive",
    "test_six_guest_codecs_and_typed_servers_emit_without_native_registry",
    "test_native_source_outputs_and_composite_tamper_refusal")
FEEDBACK_TESTS = (
    "test_client_terminal_is_released_only_after_actual_feedback",
    "test_server_native_callback_must_precede_guest_terminal", "test_completion_without_feedback_is_refused",
    "test_duplicate_feedback_marker_is_refused_before_hook", "test_pass_budget_remains_exactly_two_hundred",
    "test_retired_guest_before_completion_is_refused", "test_gateway_zero_requires_actual_worker_zero",
    "test_closed_native_marker_inventory_accepts_each_mode",
    "test_missing_reordered_or_excess_native_feedback_is_refused")
AS_CHECKS = {"codec_vectors":175, "adapter_contracts":14, "executor_mutation":9,
    "factory_transactions":75, "resource_roles":25, "unicode_strings":8,
    "highlevel_coverage":None, "local_schedule":47, "server_callbacks":60, "unified_events":33}


class SdkLane(Lane):
    """Account separately for a nested runner and its serial leaf command.

    The frozen runner creates a new session for every command. Its own process
    group contains only that coordinator and its short-lived ``ps`` samplers;
    all other observed owned descendants, including reparented children and
    commands that create another session, share the512-MiB command budget.
    The base Lane also retains its full descendant-tree and orphan RSS checks,
    birth/pidfd ownership, cgroup checks, deadlines, logs and cleanup.
    """

    def coordinator(self, name, argv, seconds):
        child, row = self.start(name, argv, stdin=None, seconds=seconds,
                                rss=RSS+COORDINATOR_RSS)
        row["rss_budget_scope"] = "coordinator process group plus separately bounded serial command descendants"
        row["coordinator_rss_limit"] = COORDINATOR_RSS
        row["command_descendants_rss_limit"] = RSS
        row["peak_coordinator_RSS"] = 0
        row["peak_command_descendants_RSS"] = 0
        self.save()
        return child, row

    def check(self):
        super().check()
        for child, row in self.children:
            if "coordinator_rss_limit" not in row or child.poll() is not None:
                continue
            coordinator, commands = 0, 0
            for owned in self.owned.rows.values():
                if owned["label"] != row["name"] or not self.owned.alive(owned):
                    continue
                pid = owned["identity"]["pid"]
                try:
                    root = Path("/proc")/str(pid)
                    first = (root/"stat").read_text().rsplit(")", 1)[1].split()
                    rss = sum(int(line.split()[1])*1024 for line in (root/"status").read_text().splitlines()
                              if line.startswith("VmRSS:"))
                    last = (root/"stat").read_text().rsplit(")", 1)[1].split()
                    if int(first[19]) != owned["identity"]["starttime_ticks"] or first[19] != last[19]:
                        require(False, "owned SDK RSS sample changed process birth")
                    # A freshly forked Popen child may execute setsid between
                    # these reads. Charge either observed non-coordinator
                    # group to the command budget; a normal session transition
                    # is not a change of the proven process birth.
                    if int(first[2]) == child.pid and int(last[2]) == child.pid:
                        coordinator += rss
                    else:
                        commands += rss
                except (FileNotFoundError, ProcessLookupError):
                    continue
            row["peak_coordinator_RSS"] = max(row["peak_coordinator_RSS"], coordinator)
            row["peak_command_descendants_RSS"] = max(row["peak_command_descendants_RSS"], commands)
            require(coordinator <= COORDINATOR_RSS, "SDK coordinator RSS limit exceeded")
            require(commands <= RSS, "SDK serial command descendant RSS limit exceeded")


def extra_models(lane, tools, temporary):
    """Actually run the separately scoped G01 source/mock fixtures.

    These do not load native ROS or validate official ROS interoperability.
    Every Python named test must execute exactly once without a skip, and every
    C++ executable must emit its exact successful assertion count. System
    headers/libraries remain explicitly outside the complete tool-package
    attestation; retained compiler dependency files do not change that scope.
    """
    directory = temporary/"bounded-models"; directory.mkdir()
    results = []
    for name, relative, expected in (
        ("bounded-profile-python", "sdk/origin/custom-types/tests/bounded_profile_models.py", PROFILE_TESTS),
        ("bounded-feedback-python", "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/feedback_protocol_models.py", FEEDBACK_TESTS)):
        source = lane.source/relative; lane.pin(source)
        tree = ast.parse(source.read_text())
        tests = [node.name for node in ast.walk(tree) if isinstance(node, ast.FunctionDef) and node.name.startswith("test_")]
        require(tests == list(expected), "closed G01 Python named inventory changed")
        argv = [str(tools["python"]), "-B", str(source), "-v"]
        child, row = lane.start(name, argv, stdin=None, seconds=120, rss=RSS); lane.finish(child, row)
        text = lane.read(row)+lane.read(row, True)
        observed = re.findall(r"^(test_[A-Za-z0-9_]+) \([^\r\n]+\) \.\.\. ok$", text, re.MULTILINE)
        require(sorted(observed) == sorted(expected) and len(observed) == len(set(observed))
            and re.search(r"^Ran "+str(len(expected))+r" tests in [0-9.]+s$", text, re.MULTILINE)
            and re.search(r"^OK$", text, re.MULTILINE)
            and not re.search(r"\b(?:skipped|expected failure|unexpected success)\b", text, re.IGNORECASE),
            "G01 Python model omitted/repeated/skipped a required named case")
        results.append({"name":name, "scope":"SOURCE_AND_MOCK_MODEL_ONLY", "tests":list(expected),
            "returncode":row["returncode"], "source":lane.pin(source),
            "stdout":row["stdout_pin"], "stderr":row["stderr_pin"]})
    for name, relative, standard, options, checks, marker in (
        ("bounded-wire-cpp17", "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/wire_contract.cc", "c++17", [], 67, "PASS bounded-wire checks=67 synthetic_only=1\n"),
        ("bounded-wire-cpp20", "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/wire_contract.cc", "c++20", [], 67, "PASS bounded-wire checks=67 synthetic_only=1\n"),
        ("bounded-facade-cpp20", "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/typed_facade_contract.cpp", "c++20", [], 27, "PASS bounded typed facade checks=27\n"),
        ("bounded-facade-cpp20-noexceptions", "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/typed_facade_contract.cpp", "c++20", ["-fno-exceptions"], 26, "PASS bounded typed facade checks=26\n")):
        source = lane.source/relative; output = directory/name; depfile = directory/(name+".d")
        includes = [lane.source, lane.source/"src", lane.source/"third-parties/fast_io/include",
            lane.source/"sdk/origin/include", lane.source/"test/0073.ros-tests/support/source/ros_custom_types/deployment-final/generated/bounded"]
        argv = [str(tools["cxx"]), "-std="+standard, "-O0", "-g0", "-UNDEBUG", *options,
            *[item for path in includes for item in ("-I",str(path))], "-MD", "-MF", str(depfile), str(source), "-o", str(output)]
        child, compilation = lane.start(name+"-compile", argv, stdin=None, seconds=120, rss=RSS); lane.finish(child, compilation)
        artifact = lane.pin(output); dependencies = lane.pin(depfile)
        require(elf(output)["e_machine"] == 62, "G01 mock fixture must be a host product, not an ARM delivery claim")
        child, row = lane.start(name+"-execute", [str(output)], stdin=None, seconds=60, rss=RSS); lane.finish(child, row)
        require(lane.read(row) == marker and lane.read(row, True) == "", "G01 C++ model assertion marker differs")
        require(lane.pin(output) == artifact, "G01 host model product changed during execution")
        results.append({"name":name, "scope":"HOST_MOCK_MODEL_ONLY", "standard":standard,
            "checks":checks, "returncode":row["returncode"], "source":lane.pin(source), "artifact":artifact,
            "compiler_argv":argv, "compiler_stdout":compilation["stdout_pin"], "compiler_stderr":compilation["stderr_pin"],
            "raw_dependency_file":dependencies, "system_dependency_closure":"UNQUALIFIED",
            "stdout":row["stdout_pin"], "stderr":row["stderr_pin"]})
    return results


def root_inventory(root):
    """Stream all declared package bytes; refuse hidden escaping symlinks."""
    require(root.is_absolute() and not root.is_symlink() and root == root.resolve(strict=True),
            "tool root must be an explicit canonical absolute path")
    require(str(root) not in ("/", "/usr", "/usr/bin", "/usr/lib", "/usr/local", "/opt", "/root", "/home", "/tmp", "/var"),
            "select physical tool packages or standalone host binaries, never a whole OS/tree inventory")
    root = root.resolve(strict=True)
    if root.is_file(): return {"kind": "explicit-standalone-file", "file": read_file_binding(root)}
    require(root.is_dir(), "tool package must be a regular file or directory")
    rows = {}
    for path in sorted(root.rglob("*")):
        name = str(path.relative_to(root))
        if path.is_symlink():
            target = path.resolve(strict=True)
            require(target.is_relative_to(root), "tool package symlink escapes selected immutable root")
            rows[name] = {"symlink": os.readlink(path), "target": str(target.relative_to(root))}
        elif path.is_file():
            rows[name] = {"sha256": digest(path), "bytes": path.stat().st_size}
    require(rows, "empty tool package cannot qualify a compiler")
    return {"kind": "whole-explicit-package-directory", "files": rows}


def load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module); return module


def planned_rows(snapshot, output, selected):
    """Replay only the frozen source-only recipe/variant constructor.

    The retained AST stops before the first symbols-file write/copy/compiler.
    This is executed only inside the actual authorized Lane, never as a source
    preparation/model test. All expected row IDs/argv derive independently from
    the complete current copied SDK and selected tools, not its result list.
    """
    script = snapshot/"sdk/origin/tools/run_local_checks.py"
    tree = ast.parse(script.read_text())
    function = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == "wasm_checks")
    def assigned(node, name):
        return isinstance(node, ast.Assign) and any(isinstance(target, ast.Name) and target.id == name for target in node.targets)
    begin = [index for index, node in enumerate(function.body) if assigned(node, "rows")]
    end = [index for index, node in enumerate(function.body) if assigned(node, "schema")]
    require(len(begin) == len(end) == 1 and begin[0] < end[0], "frozen SDK recipe/variant construction boundary differs")
    nodes = copy.deepcopy(function.body[begin[0]:end[0]])
    require(not any(isinstance(node, (ast.Import, ast.ImportFrom)) or
        isinstance(node, ast.Attribute) and node.attr in ("run", "Popen", "write_text", "write_bytes", "mkdir")
        for entry in nodes for node in ast.walk(entry)), "side effect leaked into source-only SDK recipe planner")
    module = load(snapshot/"sdk/origin/tools/build_cgroup_matrix.py", "complete_sdk_recipe_planner")
    namespace = {"module": module, "args": selected, "base": output/"wasm", "Path": Path}
    code = ast.fix_missing_locations(ast.Module(body=nodes, type_ignores=[]))
    exec(compile(code, str(script), "exec"), namespace)
    rows = namespace["rows"]
    require(rows and len({row["id"] for row in rows}) == len(rows), "current complete recipe inventory is empty/ambiguous")
    require({row["language"] for row in rows} == set(LANGUAGES) and
        all({row["backend"] for row in rows if row["language"] == language} >= {"1", "2"} for language in LANGUAGES),
        "complete six-language/both-backend recipes missing")
    return rows


def qualify_import_coverage(plans):
    """Independently require real whole-ABI probes and every typed lesson04 variant.

    The total product inventory follows the complete current frozen recipe
    plan, including newly added lessons, rather than an obsolete fixed total.
    Its result rows are still reconciled one-for-one below. This independent
    inventory prevents a planner from making every application a subset and
    thereby silently deleting full raw ABI coverage in any binding language.
    Typed service/action applications keep their seven real cases; they do not
    become compile-only smoke probes or acquire additional default rights.
    """
    indexed = {row['id']: row for row in plans}
    require(len(indexed) == len(plans), 'complete frozen SDK plan has duplicate row IDs')
    complete, typed = set(), set()
    for backend in ('1', '2'):
        for language in ('c', 'cpp', 'rust', 'tinygo'):
            name = '00_abi_coverage' if language in ('c', 'cpp') else 'coverage'
            identifier = language + '-ros' + backend + '-' + name
            row = indexed.get(identifier)
            require(row is not None and row['require_all_imports'] is True and row['expects_ros_imports'] is True,
                    'dedicated ' + language + '/ROS' + backend + ' whole-ABI probe missing')
            if language in ('c', 'cpp'):
                suffix = '.c' if language == 'c' else '.cpp'
                require(Path(row['source']).name == name + suffix and
                        '-Wl,--export=roswasm_abi_coverage' in row['argv'],
                        'C/C++ full raw ABI selector lacks its dedicated source or retained export')
            complete.add(identifier)
        for name in ('bridge', 'relay'):
            complete.add('javascript_bridge-ros' + backend + '-' + name)
        complete.add('assemblyscript-ros' + backend + '-coverage_ros' + backend)
        for language, source_name in (('c', '04_service_action_resources'), ('cpp', '04_services_actions')):
            for case in range(7):
                identifier = language + '-ros' + backend + '-' + source_name + '-case' + str(case)
                row = indexed.get(identifier)
                definition = '-DROSWASM_EXAMPLE_CASE=' + str(case)
                require(row is not None and row['expects_ros_imports'] is True and row['require_all_imports'] is False and
                        Path(row['source']).stem == source_name and row.get('variant') == [definition] and definition in row['argv'],
                        'typed C/C++ service/action subset case was omitted, changed or misclassified')
                typed.add(identifier)
    actual = {row['id'] for row in plans if row['require_all_imports'] is True}
    require(actual == complete and all(indexed[name]['expects_ros_imports'] is True for name in complete),
            'whole-ABI coverage must retain every dedicated language/backend probe, without relabeling typed subsets')
    return {'expected_products_from_frozen_plan': len(plans), 'complete_abi_probe_ids': sorted(complete),
            'typed_service_action_subset_ids': sorted(typed),
            'scope': 'Exact current frozen recipe IDs plus 14 independent full-ABI probes and 28 typed subset variants; compilation/import/full-validation only'}


def expected_commands(snapshot, plans):
    """Require every source/model group and every compile/validate pair."""
    sdk = snapshot/"sdk"
    result = ["source-"+Path(name).stem for name in ("abi_contract", "check_bindings", "wire_vectors", "test_import_inspector", "artifact_read_models", "check_sources")]
    result += ["js-syntax-"+str(path.relative_to(sdk)).replace("/", "_").removesuffix(".mjs") for path in sorted(sdk.rglob("*.mjs"))]
    result += ["source-complete-verification", "source-custom-codec-models", "js-custom-example-roundtrip"]
    result += ["js-"+name for name in ("test", "factory_transactions", "executor_contracts", "unified_events_models", "run_pure_wasm_options", "javascript_protocol_models", "javascript_api_models")]
    result += ["cmake-configure", "cmake-install", "cmake-relocated-consumer"]
    for language, suffix in (("c", ".c"), ("cpp", ".cpp")):
        for source in sorted((sdk/"tests"/language).glob("*"+suffix)):
            result += [language+"-"+source.stem+"-compile", language+"-"+source.stem+"-execute"]
    result += ["rust-host-models", "go-host-models", "policy-generator-cli", "policy-contract-compile", "policy-contract-execute"]
    for row in plans: result += [row["id"]+"-compile", row["id"]+"-validate"]
    result += ["assemblyscript-local-pure-models", "javascript-local-compiled-relay-models"]
    require(len(result) == len(set(result)), "duplicate expected complete SDK command names")
    return result


def command_rows(lane, rows, names):
    require(type(rows) is list and [row.get("name") for row in rows] == names, "SDK source/model/compiler command inventory/order differs")
    pins = []
    for row in rows:
        require(row.get("status") == "PASS" and type(row.get("returncode")) is int and row["returncode"] == 0
                and not row.get("error") and type(row.get("peak_group_rss_kib")) is int
                and row["peak_group_rss_kib"] <= 512*1024, "SDK raw command failed/blocked/exceeded its RSS guard")
        pin = lane.pin(row["log"])
        require(pin["sha256"] == row["log_sha256"] and pin["bytes"] == row["log_bytes"], "SDK original command log changed")
        pins.append(pin)
    return pins


def main():
    parser = argparse.ArgumentParser(description=__doc__); add_arguments(parser, workers=False)
    parser.add_argument("--tools", type=Path, required=True)
    args = parser.parse_args(); lane = None; status = "FAIL"
    try:
        lane = SdkLane(args, "sdk-complete-compiler-model-matrix")
        require(Path(__file__).resolve() == lane.source/"test/0073.ros-tests/support/source/ros_acceptance/run_sdk_matrix.py", "execute exact frozen SDK adapter")
        lane.report.update(mode="execute", supplemental_scope="sdk-complete-matrix", evidence_scope=SCOPE,
            actual_ROS_delivery="NOT_EXECUTED", actual_ARM_Runtime_execution="NOT_EXECUTED",
            actual_host_sdk_checks="NOT_PERFORMED", models_use_mathematical_imports=True, implicit_permissions=[],
            compile_jobs=1, rss_limit_mib=512, rss_limit_scope="each serial command and its descendants",
            coordinator_rss_limit_mib=64, composite_coordinator_tree_rss_limit_mib=576,
            inner_command_timeout_seconds=600,
            host_system_header_module_library_closure="UNQUALIFIED",
            native_compiler_model_scope="Host numeric/ownership/mock contracts only; no ROS/ARM Runtime/target-system qualification")
        selection, selection_pin = lane.json(args.tools)
        require(type(selection.get("format_version")) is int and selection["format_version"] == 1 and
            selection.get("kind") == "sdk-final-tool-selection" and set(selection) == {"format_version", "kind", "tools", "tool_roots"}
            and type(selection["tools"]) is dict and set(selection["tools"]) == set(TOOLS), "closed explicit SDK tool selection required")
        tools = {}
        for name, value in selection["tools"].items():
            require(type(value) is str and value and Path(value).is_absolute(), "explicit absolute SDK tool path required")
            tools[name] = Path(value).resolve(strict=True)
        require(tools["wasi_sdk"].is_dir(), "complete WASI SDK directory required")
        executables = [tools[name] for name in TOOLS if name not in ("wasi_sdk", "asc_js")]
        executables += [tools["wasi_sdk"]/"bin/clang", tools["wasi_sdk"]/"bin/clang++", tools["rustc"].with_name("rustdoc")]
        for path in executables:
            require(elf(path)["e_machine"] == 62, "SDK supplement selects host x86 executable, not an invented ARM target")
            lane.pin(path)
        lane.pin(tools["asc_js"])
        roots = selection["tool_roots"]
        require(type(roots) is list and roots and all(type(value) is str and Path(value).is_absolute() for value in roots)
                and len(set(roots)) == len(roots), "explicit unique whole SDK tool package roots required")
        before = {value: root_inventory(Path(value)) for value in roots}
        directory_roots = [Path(value).resolve(strict=True) for value in roots if before[value]["kind"] == "whole-explicit-package-directory"]
        for name in PACKAGE_TOOLS:
            require(any(tools[name].is_relative_to(root) for root in directory_roots), "compiler/stdlib package is qualified only by its executable: "+name)
        for path in [*executables, tools["asc_js"]]:
            require(any(path.resolve().is_relative_to(root) for root in directory_roots) or str(path) in before,
                    "explicit selected tool lies outside the declared package/standalone inputs")
        before_path = lane.out/"tool-roots-before.json"; atomic_json(before_path, before)
        lane.report["tool_selection"] = selection_pin; lane.report["whole_tool_roots_before"] = lane.pin(before_path)
        before_digest = digest(before_path)
        # Keep the complete catalog on disk, not resident throughout every
        # compiler. Canonical serialization below compares every recorded byte.
        del before
        # run_local_checks requires /tmp itself, rather than a symlink or a
        # substituted project. Retain both children and every failure there.
        temporary = Path(tempfile.mkdtemp(prefix="uwvm-sdk-complete-", dir="/tmp")); temporary.chmod(0o700)
        matrix, schedule = temporary/"matrix", temporary/"scheduling"
        lane.report["retained_tmp_root"] = str(temporary)
        driver = lane.source/"sdk/origin/tools/run_local_checks.py"; lane.pin(driver)
        argv = [str(tools["python"]), "-B", str(driver), "--out", str(matrix), "--groups", "source", "cmake", "native", "policy", "wasm",
                "--languages", *LANGUAGES, "--backends", "1", "2", "--example-variants", "--execute-pure-models", "--execute-relay-models",
                "--rss-limit-mib", "512", "--timeout-seconds", "600"]
        for name in TOOLS:
            if name != "python": argv += ["--"+name.replace("_", "-"), str(tools[name])]
        # Keep the complete adapter inside its7080s parent row, including
        # scheduling, the six bounded-model cells and final input replay:
        # 5100+660+2*120+4*(120+60)=6720s actor ceilings. The remaining360s
        # in the7080s recipe covers source/package replay and nested cleanup;
        # normalize adds120s while the unchanged final front door caps7200s.
        # Every unchanged SDK inner command retains its own600s ceiling.
        child, outer = lane.coordinator("sdk-complete-matrix", argv, seconds=5100)
        lane.finish(child, outer)
        full, full_pin = lane.json(matrix/"report.json")
        require(full.get("status") == "LOCAL_SDK_CHECKS_PASS" and full.get("actual_ROS_delivery") == "NOT_EXECUTED"
            and full.get("original_task_complete") is False and full.get("compile_jobs") == 1 and full.get("rss_limit_mib") == 512
            and all(full.get(name) is True for name in ("source_inputs_preserved", "installed_inputs_preserved", "only_expected_new_work_files"))
            and full.get("reused_compiler_products") == 0, "current complete SDK compile/model report failed or reused products")
        raw_sdk, raw_sdk_pin = lane.json(matrix/"source-manifest.json")
        require(digest(matrix/"source-manifest.json") == full["sdk_source_manifest_sha256"], "SDK frozen-copy manifest changed")
        for relative, facts in raw_sdk.items():
            expected = lane.frozen.get("sdk/"+relative)
            require(expected == facts, "SDK local snapshot differs from the complete current source manifest")
            copied = lane.pin(matrix/"source/sdk"/relative)
            require(copied["sha256"] == facts["sha256"] and copied["bytes"] == facts["bytes"], "copied SDK source bytes changed")
        require(set(raw_sdk) == {name[4:] for name in lane.frozen if name.startswith("sdk/")}, "SDK local copy omitted a frozen source file")
        require({str(path.relative_to(matrix/"source/sdk")) for path in (matrix/"source/sdk").rglob("*") if path.is_file()} == set(raw_sdk),
                "SDK copied snapshot has unrecorded source files")
        context, context_pin = lane.json(matrix/"context-manifest.json")
        for relative, facts in context.items():
            require(lane.frozen.get(relative) == facts, "SDK retained repository context is not source-bound")
            copied = lane.pin(matrix/"source"/relative)
            require(copied["sha256"] == facts["sha256"] and copied["bytes"] == facts["bytes"], "copied repository context bytes changed")
        selected = SimpleNamespace(**{name: value for name, value in tools.items() if name != "python"},
            languages=list(LANGUAGES), backends=["1", "2"], rust_target="wasm32-wasip1", example_variants=True)
        plans = planned_rows(matrix/"source", matrix, selected)
        coverage_qualification = qualify_import_coverage(plans)
        command_pins = command_rows(lane, full["rows"], expected_commands(matrix/"source", plans))
        # A zero-exit cargo/go process with zero or skipped tests is not a
        # complete native mock suite. Reconcile every declared current named
        # test against the actual retained runner output, without importing or
        # executing the SDK in this controller process.
        native_tests = {}
        for language, count, files, expression, command, output_expression in (
            ("rust",20,(matrix/"source/sdk/origin/rust").rglob("*.rs"),
             r"#\[test\]\s*(?:pub\s+)?fn\s+([A-Za-z0-9_]+)","rust-host-models",r"^test (?:[A-Za-z0-9_]+::)*([A-Za-z0-9_]+) \.\.\. ok$"),
            ("go",21,(matrix/"source/sdk/origin/tinygo").glob("*_test.go"),
             r"^func\s+(Test[A-Za-z0-9_]+)\s*\(","go-host-models",r"^--- PASS: (Test[A-Za-z0-9_]+) \([0-9.]+s\)$")):
            names = [name for path in files for name in re.findall(expression,path.read_text(),re.MULTILINE)]
            require(len(names) == count and len(set(names)) == count,"current "+language+" mock test inventory differs")
            record = next(row for row in full["rows"] if row["name"] == command)
            text = Path(record["log"]).read_bytes().decode("utf-8",errors="strict")
            observed = re.findall(output_expression,text,re.MULTILINE)
            require(sorted(observed) == sorted(names) and not re.search(r"(?:--- SKIP:|\.\.\. ignored|\d+ failed; [1-9]\d* ignored)",text),
                    "native "+language+" mock suite omitted/repeated/skipped a current named test")
            native_tests[language] = sorted(names)
        native_artifacts = []
        for command in full["rows"]:
            if command["name"].endswith("-execute"):
                path = Path(command["argv"][0])
                require(path.is_absolute() and path.is_relative_to(matrix),"native mock executable escapes owned work")
                require(elf(path)["e_machine"] == 62,"native SDK mock product is not the selected host architecture")
                native_artifacts.append(lane.pin(path))
        installed, installed_pin = lane.json(matrix/"installed-source-manifest.json")
        # The unchanged CMake delivery excludes dated verification JSON; those
        # historical receipts still stay in the immutable full source view.
        # Excluding historical evidence from an install is not permission to
        # omit any current SDK header, binding, lesson or generated codec.
        installed_expected = {name:facts for name,facts in raw_sdk.items()
            if not any(fnmatch.fnmatch(part,"verification-*.json") for part in Path(name).parts)}
        require(installed == installed_expected,"relocated installed SDK omitted/changed a current source file")
        for relative, facts in installed.items():
            pin = lane.pin(matrix/"prefix-b/share/roswasm/sdk"/relative)
            require(pin["sha256"] == facts["sha256"] and pin["bytes"] == facts["bytes"],"relocated SDK bytes changed")
        rows = full.get("wasm_compilation_rows")
        require(type(rows) is list and [row.get("id") for row in rows] == [row["id"] for row in plans],
                "complete SDK examples/conditional variants compiler inventory differs")
        planner = load(matrix/"source/sdk/origin/tools/build_cgroup_matrix.py", "complete_sdk_current_import_inspector")
        artifacts = []
        for plan, row in zip(plans, rows):
            require(row.get("status") == "COMPILE_IMPORT_FULL_VALIDATION_PASS" and row.get("current_compiler_execution") == "PERFORMED"
                and row.get("actual_ROS_delivery") == "NOT_EXECUTED" and all(row.get(key) == plan[key] for key in
                ("id", "language", "backend", "source", "source_sha256", "artifact", "cwd", "require_all_imports", "expects_ros_imports")),
                "SDK compiled example/variant source or scope differs")
            actual = list(plan["argv"])
            if plan["language"] == "rust": actual.insert(2, "-j1")
            if plan["language"] in ("c", "cpp", "javascript_bridge"):
                actual = ["-O0" if value == "-O2" else value for value in actual]
                actual.insert(1, "-Wl,--threads=1")
            if plan["language"] == "tinygo": actual[2:2] = ["-p", "1", "-opt=0"]
            compile_command = next(item for item in full["rows"] if item["name"] == row["id"]+"-compile")
            require(row.get("argv") == plan["argv"] and row.get("planned_compile_argv") == actual
                and row.get("actual_argv") == actual and compile_command["argv"] == actual
                and compile_command["cwd"] == plan["cwd"], "SDK fresh serial compiler argv/cwd differs from frozen recipe")
            data, binding = planner.read_artifact(Path(row["artifact"]))
            require(binding == row["artifact_binding"] == row["artifact_before_full_validation"] == row["artifact_after_full_validation"]
                and binding["sha256"] == row["artifact_sha256"], "SDK validated Wasm byte/identity changed")
            if row["expects_ros_imports"]:
                require(planner.INSPECTOR.check(data, row["backend"], ("wasi_snapshot_preview1",), row["require_all_imports"]) == row["import_contract"],
                        "SDK current full import signature contract differs")
            else:
                require(planner.INSPECTOR.inspect(data) == row["import_contract"]["imports"], "SDK pure/scheduling actual imports changed")
            validation = next(item for item in full["rows"] if item["name"] == row["id"]+"-validate")
            require(validation["argv"] == [str(tools["wasm_tools"]), "validate", row["artifact"]], "SDK artifact has no fresh full validator command")
            artifacts.append(lane.pin(row["artifact"]))
        pure, pure_pin = lane.json(matrix/"pure-models.json")
        relay, relay_pin = lane.json(matrix/"compiled-relay-models.json")
        require(pure.get("status") == "LOCAL_IMPORT_FREE_WASM_MODELS_PASS" and pure.get("model_count") == 10
            and pure.get("assertion_count") == 446 and pure.get("actual_ROS_delivery") == "NOT_EXECUTED",
            "complete current ten AssemblyScript pure models/446 assertions missing")
        pure_rows = pure.get("results")
        require(type(pure_rows) is list and len(pure_rows) == len(AS_CHECKS)
            and len({row.get("fixture") for row in pure_rows}) == len(pure_rows)
            and {row.get("fixture") for row in pure_rows} == set(AS_CHECKS), "AssemblyScript named pure model inventory differs")
        for row in pure_rows:
            count = AS_CHECKS[row["fixture"]]
            expected_status = "SEMANTIC_MODEL_EXECUTED" if count is None else "PURE_ASSERTIONS_PASS"
            require(row.get("status") == expected_status and row.get("completed_checks") == count
                and (count is None or type(row.get("completed_checks")) is int), "AssemblyScript actual assertion count differs")
            artifact = next(item for item in rows if item["language"] == "assemblyscript" and item["backend"] == "none"
                and Path(item["source"]).stem == row["fixture"])
            require(artifact["artifact_sha256"] == row.get("artifact_sha256"), "AssemblyScript pure execution used another Wasm product")
        require(pure.get("compile_report_sha256") == digest(matrix/"wasm-results.json"), "AssemblyScript model used another compilation inventory")
        require(relay.get("status") == "PASS_COMPILED_WASM_RELAY_GUEST_LOCAL_MODELS_ONLY" and relay.get("cases") == 80
            and relay.get("actual_Runtime_execution") is False and relay.get("actual_native_ROS_execution") is False,
            "current compiled two-backend relay mathematical80 cases missing")
        relay_rows = relay.get("results")
        require(type(relay_rows) is list and len(relay_rows) == 80 and
            len({(row.get("backend"),row.get("name")) for row in relay_rows}) == 80 and
            all(row.get("status") == "PASS" for row in relay_rows) and
            all(sum(row.get("backend") == version for row in relay_rows) == 40 for version in (1,2)),
            "actual compiled relay80 unique successful cases missing")
        schedule_argv = [str(tools["python"]), "-B", str(driver), "--out", str(schedule), "--groups", "scheduling",
                        "--node", str(tools["node"]), "--compiler-reports", str(matrix/"report.json"),
                        "--rss-limit-mib", "512", "--timeout-seconds", "600"]
        child, outer = lane.coordinator("sdk-current-compiled-scheduling", schedule_argv, seconds=660)
        lane.finish(child, outer)
        scheduling, scheduling_pin = lane.json(schedule/"report.json")
        require(scheduling.get("status") == "LOCAL_SDK_CHECKS_PASS" and scheduling.get("actual_ROS_delivery") == "NOT_EXECUTED",
                "separate current compiled scheduling report failed")
        command_pins += command_rows(lane, scheduling["rows"], ["compiled-scheduling-models"])
        semantic, semantic_pin = lane.json(schedule/"scheduling-models.json")
        require(semantic.get("status") == "COMPILED_LOCAL_SCHEDULING_MODELS_PASS" and semantic.get("cases") == 12
            and semantic.get("actual_ROS_delivery") == "NOT_EXECUTED", "complete compiled mathematical scheduling12 cases missing")
        schedule_rows = semantic.get("results")
        require(type(schedule_rows) is list and len(schedule_rows) == 12
            and len({(row.get("id"),row.get("synthetic_entropy_selected")) for row in schedule_rows}) == 12
            and all(row.get("status") == "PASS" and row.get("ROS_resource_calls") == 0 for row in schedule_rows),
            "compiled scheduling actual twelve unique model executions missing")
        bounded_models = extra_models(lane, tools, temporary)
        for item in (matrix/"wasm-results.json", matrix/"progress.json", schedule/"progress.json",
                     schedule/"scheduling-inputs.json", schedule/"compiler-report-bindings.json",
                     matrix/"complete-source-verification.json",matrix/"custom-codec-models.json",
                     matrix/"javascript_protocol_models.json",matrix/"javascript_api_models.json"):
            lane.pin(item)
        after = {value: root_inventory(Path(value)) for value in roots}
        after_path = lane.out/"tool-roots-after.json"; atomic_json(after_path, after)
        require(digest(before_path) == before_digest == digest(after_path),
                "whole SDK compiler/stdlib/package inputs changed")
        del after
        lane.report.update(raw_matrix_report=full_pin, raw_scheduling_report=scheduling_pin,
            sdk_snapshot_manifest=raw_sdk_pin, repository_context_manifest=context_pin, raw_command_logs=command_pins,
            installed_sdk_manifest=installed_pin, native_mock_test_names=native_tests,native_mock_artifacts=native_artifacts,
            native_mock_test_counts={"rust":20,"go":21},
            artifact_pins=artifacts, current_pure_model_receipt=pure_pin, current_relay_model_receipt=relay_pin,
            current_scheduling_model_receipt=semantic_pin, whole_tool_roots_preserved=True,
            complete_example_variants_verified=True, wasm_products=len(rows), fresh_full_validations=len(rows),
            import_coverage_qualification=coverage_qualification,
            languages=list(LANGUAGES), backends=[1, 2], source_groups=["source", "cmake", "native", "policy", "wasm"],
            pure_models=10, pure_assertions=446, relay_cases=80, scheduling_cases=12,
            bounded_models=bounded_models, bounded_python_tests=[17,9], bounded_cpp_assertions=[67,67,27,26],
            whole_tool_roots_after=lane.pin(after_path),
            actual_host_sdk_checks="EXECUTED", new_tests_executed=True)
        status = "PASS"
    except BaseException as error:
        if lane is None: raise
        lane.report.update(error=repr(error), traceback=traceback.format_exc())
    finally:
        if lane is not None: status = lane.close(status)
    return 0 if status == "PASS" else 1


if __name__ == "__main__": raise SystemExit(main())
