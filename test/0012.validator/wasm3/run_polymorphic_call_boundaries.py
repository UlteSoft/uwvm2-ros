#!/usr/bin/env python3
"""Black-box frame-boundary regressions for unreachable Wasm calls.

Use --vm for a real VM or a platform runner wrapper. Invalid cases must be
rejected before the start function can run. wasm-tools validates every fixture
independently; --wasmtime additionally executes the standard valid controls.
"""
from pathlib import Path
import argparse
import json
import subprocess


def cases():
    frames = {
        "block": lambda body: f"block br 0 {body} end",
        "loop": lambda body: f"block loop br 1 {body} end end",
        "if": lambda body: f"i32.const 0 if unreachable {body} end",
        "try_table": lambda body: f"try_table br 0 {body} end",
    }
    outers = {
        "i64": ("i64.const 42", "i64.const 42 i64.ne if unreachable end"),
        "v128": ("v128.const i32x4 7 8 9 10", "i32x4.extract_lane 2 i32.const 9 i32.ne if unreachable end"),
        "i31": ("i32.const 29 ref.i31", "i31.get_s i32.const 29 i32.ne if unreachable end"),
    }
    calls = {
        "direct": "call $callee",
        "indirect": "i32.const 0 call_indirect (type $sig)",
        "reference": "ref.func $callee call_ref $sig",
    }
    prefix = """(module
      (type $sig (func (param i32 i32) (result f64 i64)))
      (table 1 funcref) (elem (i32.const 0) $callee)
      (func $callee (type $sig) f64.const 1 i64.const 2)
    """
    for call, instruction in calls.items():
        for frame, wrap in frames.items():
            for outer, (push, check) in outers.items():
                for supplied in [0, 1]:
                    body = ("i32.const 99 " if supplied else "") + instruction + " drop drop"
                    yield (f"{call}-{frame}-{outer}-partial{supplied}", True,
                           prefix + f'(func (export "_start") {push} {wrap(body)} {check}))')
        # Explicit operands remain type-checked even in a polymorphic region.
        for kind, body, cleanup in [
            ("wrong-concrete", "f64.const 0 " + instruction + " drop drop", "drop"),
            ("extra-outer-result", instruction + " drop drop", ""),
        ]:
            yield (f"invalid-{call}-{kind}", False,
                   prefix + f'(func (export "_start") i64.const 42 block br 0 {body} end {cleanup}))')
        yield (f"invalid-{call}-reachable-underflow", False,
               prefix + f'(func (export "_start") i64.const 42 block {instruction} drop drop end drop))')
        # Ordinary reachable calls still consume their two parameters/results.
        yield (f"{call}-reachable", True,
               prefix + f'(func (export "_start") i32.const 4 i32.const 5 {instruction} i64.const 2 i64.ne if unreachable end f64.const 1 f64.ne if unreachable end))')
    # Tail calls must likewise leave enclosing frames intact during validation.
    for name, instruction in {
        "direct": "return_call $callee", "indirect": "i32.const 0 return_call_indirect (type $sig)",
        "reference": "ref.func $callee return_call_ref $sig",
    }.items():
        head = '(module (type $sig (func (param i32))) (table 1 funcref) (elem (i32.const 0) $callee) (func $callee (type $sig)) '
        for frame, wrap in frames.items():
            yield (f"tail-{name}-{frame}", True,
                   head + f'(func (export "_start") i64.const 42 {wrap(instruction)} i64.const 42 i64.ne if unreachable end))')
        yield (f"invalid-tail-{name}-concrete", False,
               head + f'(func (export "_start") block br 0 f64.const 0 {instruction} end))')


def run(argv, timeout):
    try:
        p = subprocess.run(list(map(str, argv)), capture_output=True, text=True, timeout=timeout)
        return dict(argv=list(map(str, argv)), exit=p.returncode, stdout=p.stdout, stderr=p.stderr)
    except subprocess.TimeoutExpired:
        return dict(argv=list(map(str, argv)), exit="timeout")


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--vm", type=Path, required=True)
    p.add_argument("--wasm-tools", type=Path, required=True)
    p.add_argument("--output", type=Path, required=True)
    p.add_argument("--wasmtime", type=Path)
    p.add_argument("--timeout", type=float, default=60)
    p.add_argument("--engines", default="int,aot")
    args = p.parse_args()
    if any(engine not in ("int", "aot") for engine in args.engines.split(",")):
        p.error("--engines accepts int,aot only; interactive debug requires an authenticated control session")
    out = args.output.resolve();out.mkdir(parents=True, exist_ok=True)
    rows = []
    for name, valid, wat in cases():
        source = out / (name + ".wat");binary = source.with_suffix(".wasm");source.write_text(wat + "\n")
        parsed = run([args.wasm_tools, "parse", source, "-o", binary], args.timeout)
        if parsed['exit'] != 0:
            rows.append(dict(name=name, stage="assemble", passed=False, **parsed));continue
        validation = run([args.wasm_tools, "validate", binary], args.timeout)
        accepted = validation['exit'] == 0
        rows.append(dict(name=name, stage="independent-validation", passed=accepted == valid, **validation))
        if accepted != valid:continue
        for engine in args.engines.split(','):
            command = [args.vm, "-R" + engine,
                       "-Rct", "0", "-Rllvm-cache-path", "disable", "--debug-fault-action", "exit", "--run", binary]
            result = run(command, args.timeout)
            expected = 0 if valid else 127
            rows.append(dict(name=name, stage=engine, expected=expected, passed=result['exit'] == expected, **result))
        if args.wasmtime:
            result = run([args.wasmtime, "run", "-C", "cache=n", binary], args.timeout)
            good = result['exit'] == 0 if valid else result['exit'] not in (0, "timeout") and "failed to compile" in result.get('stderr', '')
            rows.append(dict(name=name, stage="wasmtime", passed=good, **result))
    result = dict(complete=True, fixtures=len(list(cases())), rows=rows, failed=sum(not r['passed'] for r in rows))
    (out / "results.json").write_text(json.dumps(result, indent=2) + "\n")
    print(f"{len(rows)} checks, {result['failed']} failures")
    return bool(result['failed'])


if __name__ == "__main__":
    raise SystemExit(main())
