"""Run the same linked owner-page regression with an interpreter or AOT CLI."""
import argparse
import shlex
import subprocess
from pathlib import Path


def compile_wat(directory, wat2wasm, name, source):
    wat = directory / (name + ".wat")
    wasm = wat.with_suffix(".wasm")
    wat.write_text(source)
    subprocess.run([wat2wasm, "--enable-multi-memory", wat, "-o", wasm], check=True)
    return str(wasm)


def expect_value(expression, value):
    return f"(if (i32.ne {expression} (i32.const {value})) (then unreachable))"


def check_memory(name, minimum, maximum):
    # Touch the last old and new bytes, grow through this alias, and retain the
    # old byte after a refused grow. All four operands refer to different owners.
    old = f"(i32.const {minimum * 65536 - 1})"
    new = f"(i32.const {maximum * 65536 - 1})"
    return "\n".join([
        expect_value(f"(memory.size ${name})", minimum),
        f"(i32.store8 ${name} {old} (i32.const 42))",
        expect_value(f"(memory.grow ${name} (i32.const {maximum - minimum}))", minimum),
        expect_value(f"(memory.size ${name})", maximum),
        expect_value(f"(i32.load8_u ${name} {new})", 0),
        f"(i32.store8 ${name} {new} (i32.const 91))",
        expect_value(f"(memory.grow ${name} (i32.const 1))", -1),
        expect_value(f"(i32.load8_u ${name} {old})", 42),
        expect_value(f"(i32.load8_u ${name} {new})", 91),
    ])


def fixtures(directory, wat2wasm):
    owner = '(module (memory (export "mem") 1 8))'
    files = {name: compile_wat(directory, wat2wasm, name, owner) for name in ("base", "camera")}
    files["lidar"] = compile_wat(directory, wat2wasm, "lidar", """(module
        (import "base" "mem" (memory $input 0))
        (export "relay" (memory $input))
        (memory (export "first") 1 8)
        (memory (export "second") 1 8))""")
    for label, camera_maximum in (("app", 6), ("open", 8)):
        body = "\n".join(check_memory(name, low, high) for name, low, high in (
            ("relay", 1, 2), ("first", 2, 4), ("second", 4, 5), ("camera", 3, camera_maximum)))
        files[label] = compile_wat(directory, wat2wasm, label, f"""(module
            (import "lidar" "relay" (memory $relay 0))
            (import "lidar" "first" (memory $first 0))
            (import "lidar" "second" (memory $second 0))
            (import "camera" "mem" (memory $camera 0))
            (global $value (mut i32) (i32.const 7))
            (func (export "_start")
                {expect_value("(global.get $value)", 7)}
                (global.set $value (i32.const 11))
                {expect_value("(global.get $value)", 11)}
                {body}))""")
    return files


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--engine", required=True, choices=("int", "aot"))
    parser.add_argument("--runner", default="")
    parser.add_argument("--wat2wasm", default="wat2wasm")
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=False)
    files = fixtures(args.out.resolve(), args.wat2wasm)
    base = shlex.split(args.runner) + [str(args.vm.resolve()), "--runtime-" + args.engine, "--runtime-compile-threads", "0",
                                      "--wasm-set-main-module-name", "app", "-Wstart", "0"]
    if args.engine == "aot":
        base += ["--runtime-llvm-jit-cache-path", "disable"]
    key = "runtime.memory-pages"
    preloads = [v for name in ("base", "lidar", "camera") for v in ("--wasm-preload-library", files[name], name)]
    groups = ["--wasm-policy-global", key, "1:2", "--wasm-policy-group-add-module", "sensors", "lidar",
              "--wasm-policy-group", "sensors", key, "2:4", "--wasm-policy-group-add-module", "sensors", "camera",
              "--wasm-policy-single", "app", key, "0:0"]
    single = ["--wasm-policy-single", "camera", key, "3:6"]
    indexed = ["--wasm-set-memory-limit", "lidar", "1", "4", "5"]
    legacy = ["--wasm-set-memory-limit", "camera", "all", "3", "6"]

    def run(label, flags, fixture="app", status=0, message=""):
        result = subprocess.run(base + flags + ["--run", files[fixture]], capture_output=True, text=True, timeout=30)
        if result.returncode != status or message not in result.stderr:
            raise RuntimeError(f"{label}: exit {result.returncode}, expected {status}\n{result.stderr}")
        print("PASS", label)

    run("global/group/single/index through aliases", preloads + groups + single + indexed)
    run("reordered settings and preloads", indexed + single + groups + preloads)
    run("legacy all above group", indexed + legacy + groups + preloads)
    run("minimum alone replaces the complete pair", groups + single[:-1] + ["3"] + indexed + preloads, "open")
    run("an importer has no local index for its aliases", groups + single + indexed + preloads +
        ["--wasm-set-memory-limit", "app", "0", "0", "0"], status=126, message="Unknown local memory definition")
    for label, flags in (("all then single", legacy + single), ("single then all", single + legacy),
                         ("repeated index", indexed + indexed), ("repeated all", legacy + legacy)):
        run(label, flags, status=126, message="Duplicate memory page policy")
    for invalid in ("2:1", ":3", "2:", "1:2:3", "18446744073709551616"):
        run("invalid " + invalid, ["--wasm-policy-global", key, invalid], status=126, message="Invalid or duplicate Wasm policy")


if __name__ == "__main__":
    main()
