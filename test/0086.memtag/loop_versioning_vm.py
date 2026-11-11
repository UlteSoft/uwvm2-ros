#!/usr/bin/env python3
"""Current packed-Wasm correctness checks for opt-in private loop versioning.

The same actual AOT binary executes every fixture with the experiment disabled
and explicitly enabled. Guest assertions check the complete successful memory
footprint. Fault oracles require controlled exits and exact tag evidence; a
signal, timeout, old wire format, or elapsed time is never a passing witness.

--prepare-observer writes a separate, cold-only VFS overlay and optional build
plan. It does not edit or compile the production source. --require-ir is for a
separately identified binary built with that overlay, never a timing cohort.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

import vm_matrix as m
from vm_diagnostics import fault_matches


BASE, STRIDE, COUNT, TAG = 256, 32, 8, 3
AUX, COMPLETED = 1024, 37
CASES = ("valid", "interior-tag", "unvisited-tag", "preflight-order",
         "bounds-late", "tag-before-bounds", "call", "retag", "atomic", "shared", "async-control")
VERSIONED_CASES = {"valid", "interior-tag", "unvisited-tag", "preflight-order"}
EXPERIMENT = "UWVM_STUDY_MEMTAG_LOOP_VERSIONING"
OBSERVER = "UWVM_MTE_STUDY_IR_DIR"


def digest(path):
    h = hashlib.sha256()
    with Path(path).open("rb") as stream:
        while block := stream.read(1 << 20):
            h.update(block)
    return h.hexdigest()


def local(index):
    return b"\x20" + m.u(index)


def put(index):
    return b"\x21" + m.u(index)


def oracle(iterations, called=False):
    words = [i * 17 + 1 for i in range(COUNT)]
    checksum = 0x13579BDF
    for i in range(iterations):
        index = (i >> 12) & (COUNT - 1)
        value = words[index]
        mix = 2 * i + 1 if called else i
        checksum = (((checksum << 5) | (checksum >> 27)) & 0xFFFFFFFF) ^ ((value + mix) & 0xFFFFFFFF)
        words[index] = (value + 3) & 0xFFFFFFFF
    image = bytearray(COUNT * STRIDE)
    for i, value in enumerate(words):
        image[i * STRIDE:i * STRIDE + 4] = value.to_bytes(4, "little")
    return checksum, bytes(image)


def fixture(bits, case, iterations):
    p = m.Program(bits == 64)
    wide = bits == 64
    base = 65536 - 128 if case in ("bounds-late", "tag-before-bounds") else BASE
    span = 128 if base != BASE else COUNT * STRIDE
    bad = base + (2 if case == "tag-before-bounds" else 4) * STRIDE
    if case == "retag":
        bad = base
    hole = base + 16
    p.add(p.tags(base, span, TAG))
    for i in range(span // STRIDE):
        p.add(p.tagged(base + i * STRIDE, TAG) + m.c(i * 17 + 1) + m.mem(0x36))
    if case in ("interior-tag", "preflight-order", "tag-before-bounds"):
        p.add(p.tags(bad, 16, 4))
    elif case == "unvisited-tag":
        p.add(p.tags(hole, 16, 4))
    if case == "preflight-order":
        # An active-data value loaded before the loop is not an LLVM constant.
        # A literal `i == 1` can reduce the counted loop to two iterations;
        # an in-loop volatile condition load also changes loop simplification.
        p.add(p.n(AUX) + m.mem(0x28) + put(4))
    # Local 3 is a constant-tag base. The selected leaf loop has an independently
    # bounded scalar address set and a known counted latch of >=32768 iterations.
    p.add(p.tagged(base, TAG) + put(3) + m.c(0) + put(0) + m.c(0x13579BDF) + put(1))
    p.add(b"\x03\x40")
    if case == "preflight-order":
        # A future bad granule must only select the original loop. Its earlier
        # unreachable at iteration 1 must win over that later tag mismatch.
        p.add(local(0) + local(4) + b"\x46\x04\x40\x00\x0b")
    pointer = (local(3) + local(0) + m.c(12) + b"\x76" + m.c(COUNT - 1) + b"\x71" +
               m.c(5) + b"\x74" + (b"\xad\x7c" if wide else b"\x6a"))
    if case == "retag":
        # Mutation in the loop invalidates a preflight proof. First iteration
        # changes tag 3 to 4 before the ordinary tag-3 access: exact 122 required.
        p.add(p.tags(base, 16, 4))
    load = b"\xfe\x10\x02\0" if case == "atomic" else m.mem(0x28)
    p.add(pointer + load + put(2))
    mix = local(0) + b"\x10\x02" if case == "call" else local(0)
    p.add(local(1) + m.c(5) + b"\x77" + local(2) + mix + b"\x6a\x73" + put(1))
    p.add(pointer + local(2) + m.c(3) + b"\x6a" + m.mem(0x36))
    p.add(local(0) + m.c(1) + b"\x6a\x22\0" + m.c(iterations) + b"\x49\x0d\0\x0b")
    checksum, image = oracle(iterations, case == "call")
    p.add(m.assertion(local(1), checksum))
    if base == BASE:
        # All 256 bytes, including unused words/holes and both outside canaries,
        # are checked. In the hole case use the actual allocation tag for the
        # inspection itself, so successful fallback is not confused with a fault.
        for i, byte in enumerate(image):
            tag = 4 if case == "unvisited-tag" and hole <= base + i < hole + 16 else TAG
            p.add(m.assertion(p.tagged(base + i, tag) + m.mem(0x2D), byte))
        for i in range(16):
            for address in (base - 16 + i, base + span + i):
                p.add(m.assertion(p.n(address) + m.mem(0x2D), 0))
    if case == "call":
        p.add(m.assertion(p.n(AUX) + m.mem(0x28), iterations))
        for address in (AUX - 4, AUX + 4):
            p.add(m.assertion(p.n(address) + m.mem(0x28), 0))
    # A nonzero completion sentinel establishes that the selected entry reached
    # every assertion. There are no host calls inside an eligible leaf loop.
    p.add(m.c(COMPLETED) + b"\x10\0\x00")
    locals_ = b"\x03\x03\x7f\x01\x7e\x01\x7f" if wide else b"\x01\x05\x7f"
    bodies = [locals_ + p.code + b"\x0b"]
    signatures = b"\x03\x60\0\0\x60\x01\x7f\0\x60\x01\x7f\x01\x7f"
    functions = [0]
    if case == "call":
        # This genuine Wasm callee has an observable counter update. The call
        # cannot be dropped as a pure unused identity by the ordinary optimizer.
        helper = (p.n(AUX) + p.n(AUX) + m.mem(0x28) + m.c(1) + b"\x6a" + m.mem(0x36) +
                  local(0) + p.n(AUX) + m.mem(0x28) + b"\x6a")
        bodies.append(b"\0" + helper + b"\x0b")
        functions.append(2)
    flags = 1 + (4 if wide else 0) + (2 if case == "shared" else 0)
    imports = b"\x01\x16wasi_snapshot_preview1\x09proc_exit\0\x01"
    binary = (b"\0asm\x01\0\0\0" + m.sec(1, signatures) + m.sec(2, imports) +
              m.sec(3, m.u(len(functions)) + b"".join(m.u(x) for x in functions)) +
              m.sec(5, b"\x01" + m.u(flags) + b"\x01\x02") +
              m.sec(7, b"\x01\x06_start\0\x01") +
              m.sec(10, m.u(len(bodies)) + b"".join(m.u(len(body)) + body for body in bodies)))
    if case == "preflight-order":
        binary += m.sec(11, b"\x01\x00" + p.n(AUX) + b"\x0b\x04\x01\0\0\0")
    status = (120 if case == "preflight-order" else 121 if case == "bounds-late" else
              122 if case in ("interior-tag", "tag-before-bounds", "retag") else COMPLETED)
    return binary, dict(bits=bits, case=case, expected_exit=status,
                        expected_pointer=(TAG << (56 if wide else 28)) | bad,
                        expected_granule=bad, expected_tag=TAG, expected_allocation_tag=4,
                        expected_checksum=checksum, successful_footprint_bytes=(300 if case == "call" else 288) if base == BASE else 0,
                        ir_versioned_when_enabled=case in VERSIONED_CASES,
                        delivery="software-async" if case == "async-control" else "sync")


def tag_evidence(output, expected):
    for label, value in (("Memory index", 0), ("Pointer", expected["expected_pointer"]),
                         ("Granule", expected["expected_granule"]), ("Pointer tag", TAG),
                         ("Allocation tag", 4), ("Tag evidence mask", 255)):
        actual = re.findall(r"^  " + re.escape(label) + r": ([0-9]+)$", output, re.MULTILINE)
        if actual != [str(value)]:
            return False
    return True


def ir_evidence(directory):
    rows = []
    for path in sorted(directory.glob("*.ll")):
        text = path.read_text(errors="replace")
        fast_blocks = []
        for block in re.split(r"(?=^[a-zA-Z$._][\w$.-]*:)", text, flags=re.MULTILINE):
            header = block.splitlines()[0] if block else ""
            if ".memtag.proved:" in header:
                fast_blocks.append(block.split("\n}", 1)[0])
        rows.append(dict(file=str(path), sha256=digest(path), bytes=path.stat().st_size,
                         optimized=path.name.endswith("-optimized.ll"),
                         version_attribute='"uwvm.memtag.loop.proved.v1"' in text,
                         preflight="memtag.preflight.scan" in text,
                         fast_blocks=len(fast_blocks),
                         fast_i8_loads=sum(len(re.findall(r"\bload(?: volatile)? i8\b", block)) for block in fast_blocks)))
    return rows


OBSERVER_HEADER = r'''// TEST-ONLY cold compiler observation. Never install in release source.
#pragma once
#include <atomic>
#include <cstdlib>
#include <string>
#include <system_error>
#include <llvm/IR/Module.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
namespace uwvm_loop_validation {
inline void record_ir(::llvm::Module const& module, char const* stage) noexcept {
    auto const* directory=::std::getenv("UWVM_MTE_STUDY_IR_DIR");
    if(directory==nullptr || *directory=='\0') return;
    static ::std::atomic<unsigned long long> serial{};
    auto const number=serial.fetch_add(1,::std::memory_order_relaxed);
    auto const path=::std::string(directory)+"/module-"+::std::to_string(number)+"-"+stage+".ll";
    ::std::error_code error;
    ::llvm::raw_fd_ostream output(path,error,::llvm::sys::fs::OF_Text);
    if(error) return;
    module.print(output,nullptr);
}
}
'''


def prepare_observer(source_root, out, build_receipt):
    source_root = source_root.resolve()
    source = source_root / "src/uwvm2/runtime/lib/uwvm_runtime.default.cpp"
    text = source.read_text()
    before = digest(source)
    include = '#  include "uwvm_runtime_llvm_memtag_loop_versioning.h"\n'
    start = '            // Verify before/after optimization and run the selected full-module pipeline.\n'
    end = ('            return true;\n        }\n\n'
           '        // Successful fused emission is necessary but not sufficient: only\n')
    for anchor in (include, start, end):
        if text.count(anchor) != 1:
            raise ValueError("observer anchor must match exactly once: " + repr(anchor))
    header = out / "uwvm_loop_ir_observer.h"
    header.write_text(OBSERVER_HEADER)
    text = text.replace(include, include + "#  include " + json.dumps(str(header.resolve())) + "\n")
    text = text.replace(start, '            ::uwvm_loop_validation::record_ir(module, "input");\n' + start)
    text = text.replace(end, '            ::uwvm_loop_validation::record_ir(module, "optimized");\n' + end)
    rewritten = out / "uwvm_runtime.observed.cpp"
    rewritten.write_text(text)
    overlay = out / "overlay.json"
    overlay.write_text(json.dumps({"version": 0, "use-external-names": False, "roots": [
        {"type": "file", "name": str(source), "external-contents": str(rewritten.resolve())}]}, indent=2) + "\n")
    receipt = dict(scope="Test-only immutable-source VFS overlay: one include and two cold IR snapshots; no production writes or compilation.",
                   source=str(source), source_before_sha256=before, source_after_sha256=digest(source),
                   observed_source_sha256=digest(rewritten), observer_sha256=digest(header),
                   overlay_sha256=digest(overlay), hooks=["input", "optimized"], production_unchanged=before == digest(source))
    if build_receipt:
        build = json.loads(build_receipt.read_text())
        stages = build.get("stages", [])
        compile_stage = next(x for x in stages if "-c" in x.get("command", []) and
                             any(y.endswith("/uwvm_runtime.default.cpp") for y in x["command"]))
        compile_cmd = compile_stage["command"]
        compile_cwd = Path(compile_stage.get("cwd", str(source_root))).resolve()
        input_path = Path(next(y for y in compile_cmd if y.endswith("/uwvm_runtime.default.cpp")))
        if not input_path.is_absolute():
            input_path = compile_cwd / input_path
        if input_path.resolve() != source:
            raise ValueError("build command is for a different source snapshot")
        old_object = compile_cmd[compile_cmd.index("-o") + 1]
        observed_object = str((out / "uwvm_runtime.observed.o").resolve())
        rewritten_compile = list(compile_cmd)
        rewritten_compile[rewritten_compile.index("-o") + 1] = observed_object
        rewritten_compile[1:1] = ["-ivfsoverlay", str(overlay.resolve())]
        link_stage = next(x for x in stages if "-c" not in x.get("command", []) and old_object in x.get("command", []))
        link = link_stage["command"]
        rewritten_link = [observed_object if x == old_object else x for x in link]
        rewritten_link[rewritten_link.index("-o") + 1] = str((out / "uwvm2-ros-loop-observer").resolve())
        receipt.update(build_receipt=str(build_receipt.resolve()), build_receipt_sha256=digest(build_receipt),
                       compile_command=rewritten_compile, compile_cwd=str(compile_cwd),
                       link_command=rewritten_link, link_cwd=link_stage.get("cwd", str(source_root)),
                       reused_objects_sha256={x: digest(x) for x in link if x.endswith(".o") and x != old_object})
    (out / "observer-plan.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps(receipt, indent=2))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--candidate", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    # Leave enough counted latches after the ordinary pipeline's observed
    # four-way unrolling: 262144 / 4 still exceeds the 32768 admission floor.
    parser.add_argument("--iterations", type=int, default=262144)
    parser.add_argument("--cases", default=",".join(CASES))
    parser.add_argument("--bits", default="32,64")
    parser.add_argument("--cpu", type=int)
    parser.add_argument("--timeout", type=float, default=60)
    parser.add_argument("--require-ir", action="store_true")
    parser.add_argument("--generate-only", action="store_true")
    parser.add_argument("--prepare-observer", type=Path, metavar="SOURCE_ROOT")
    parser.add_argument("--build-receipt", type=Path)
    args = parser.parse_args()
    if args.cpu is not None:
        os.sched_setaffinity(0, {args.cpu})
    if not 32768 <= args.iterations < 2**31 or not 0 < args.timeout < float("inf"):
        parser.error("iterations must be >=32768 and <2^31; timeout must be finite and positive")
    cases, widths = args.cases.split(","), [int(x) for x in args.bits.split(",")]
    if not cases or not set(cases) <= set(CASES) or not widths or not set(widths) <= {32, 64}:
        parser.error("unknown case or address width")
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    if args.prepare_observer:
        prepare_observer(args.prepare_observer, out, args.build_receipt)
        return 0
    if not args.generate_only and args.candidate is None:
        parser.error("--candidate is required for execution")
    fixtures = []
    for bits in widths:
        for case in cases:
            binary, row = fixture(bits, case, args.iterations)
            file = out / f"loop-{bits}-{case}.wasm"
            file.write_bytes(binary)
            fixtures.append(dict(row, file=str(file), sha256=digest(file), encoding="packed",
                                 iterations=args.iterations))
    (out / "manifest.json").write_text(json.dumps(fixtures, indent=2) + "\n")
    if args.generate_only:
        print("GENERATED", len(fixtures), "current packed fixtures; no execution claim")
        return 0
    candidate = args.candidate.resolve()
    report = dict(binary=str(candidate), binary_sha256=digest(candidate), source_sha256=digest(__file__),
                  generator_sha256=digest(Path(m.__file__)), iterations=args.iterations, require_ir=args.require_ir,
                  scope="Actual single-candidate AOT correctness, opt-in off/on, no elapsed-time inference; separate optional instrumented IR witness.",
                  host_precondition="This test exclusively executes private owners. Shared case is a rejection control; no production lease is claimed to enforce exclusivity.",
                  rows=[], complete=False)
    for case in fixtures:
        for enabled in (False, True):
            name = f"loop-{case['bits']}-{case['case']}-{'enabled' if enabled else 'disabled'}"
            directory = out / name
            directory.mkdir()
            env = dict(os.environ)
            env.pop(EXPERIMENT, None)
            env.pop(OBSERVER, None)
            if enabled:
                env[EXPERIMENT] = "exclusive-private"
            ir_dir = directory / "ir"
            if args.require_ir:
                ir_dir.mkdir()
                env[OBSERVER] = str(ir_dir)
            command = [str(candidate), "--runtime-aot", "--runtime-compile-threads", "0",
                       "--runtime-llvm-jit-cache-path", "disable", "--wasm-feature-enable-memory64",
                       "--wasm-feature-enable-threads", *m.mode_flags(case["delivery"]),
                       "--wasm-set-main-module-name", "main", "--wasip1-single-create", "main",
                       "--wasip1-single-enable", "main", "--wasip1-single-interface", "main", "proc_exit", "allow",
                       "--wasip1-single-grant", "main", "proc-exit", "--wasm-set-start-func", "0", "--", case["file"]]
            try:
                done = subprocess.run(command, env=env, capture_output=True, timeout=args.timeout)
                code, stdout, stderr = done.returncode, done.stdout, done.stderr
            except subprocess.TimeoutExpired as error:
                code, stdout, stderr = "timeout", error.stdout or b"", error.stderr or b""
            (directory / "stdout.log").write_bytes(stdout)
            (directory / "stderr.log").write_bytes(stderr)
            output = (stdout + stderr).decode(errors="replace")
            expected = case["expected_exit"]
            semantic = code == COMPLETED if expected == COMPLETED else fault_matches(
                code, output, expected, {120: "unreachable", 121: "out of bounds", 122: "memory tag mismatch"}[expected])
            if expected == 122:
                semantic = semantic and tag_evidence(output, case)
            snapshots = ir_evidence(ir_dir) if args.require_ir else []
            optimized = [x for x in snapshots if x["optimized"]]
            want_versioned = enabled and case["ir_versioned_when_enabled"]
            witness = None
            if args.require_ir:
                witness = bool(optimized) and all(x["version_attribute"] == want_versioned for x in optimized)
                if want_versioned:
                    witness = witness and any(x["preflight"] and x["fast_blocks"] and x["fast_i8_loads"] == 0 for x in optimized)
            row = dict(name=name, case=case["case"], bits=case["bits"], enabled=enabled,
                       command=command, environment={EXPERIMENT: env.get(EXPERIMENT), OBSERVER: env.get(OBSERVER)},
                       fixture_sha256=case["sha256"], exit=code, expected_exit=expected,
                       semantic_pass=bool(semantic), expected_versioned=want_versioned,
                       ir_witness_pass=witness, snapshots=snapshots,
                       passed=bool(semantic) and (witness is True if args.require_ir else True))
            report["rows"].append(row)
            (out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
            print(name, "exit", code, "semantic", bool(semantic), "IR", witness, flush=True)
    report["binary_unchanged"] = digest(candidate) == report["binary_sha256"]
    report["semantic_passed"] = sum(x["semantic_pass"] for x in report["rows"])
    report["ir_witness_passed"] = sum(x["ir_witness_pass"] is True for x in report["rows"])
    report["complete"] = report["binary_unchanged"] and all(x["passed"] for x in report["rows"])
    (out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    return 0 if report["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
