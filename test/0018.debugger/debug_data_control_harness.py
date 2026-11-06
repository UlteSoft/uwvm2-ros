"""Common environment admission for actual debugger data-control acceptance."""
import argparse
import os
from pathlib import Path
import subprocess

def configuration(case):
    parser = argparse.ArgumentParser(description="Real broker/DAP debugger acceptance: " + case)
    parser.add_argument("--vm", type=Path, required=True, help="Built product or existing QEMU launcher")
    parser.add_argument("--sdk", type=Path, required=True, help="Existing uwvm2-ros-sdk checkout")
    parser.add_argument("--out", type=Path, required=True, help="New directory in repository .tmp")
    parser.add_argument("--arch", choices=("x86_64", "i686", "aarch64", "arm", "armhf"), default="x86_64")
    parser.add_argument("--wat2wasm", default="wat2wasm")
    parser.add_argument("--wasm-tools", default="wasm-tools")
    parser.add_argument("--wasm-clang", default="clang")
    parser.add_argument("--wasm-ld", default="wasm-ld")
    parser.add_argument("--source-wasm", type=Path, help="Optional existing O0/DWARF5 SDK algorithm fixture")
    args = parser.parse_args()
    args.source_root = Path(__file__).resolve().parents[2]
    common = subprocess.check_output(["git", "-C", str(args.source_root), "rev-parse", "--path-format=absolute", "--git-common-dir"], text=True)
    args.repo = Path(common.strip()).parent
    args.vm = args.vm.absolute()
    args.vm.resolve(strict=True)
    args.sdk = args.sdk.resolve(strict=True)
    args.out = args.out.resolve()
    if not args.out.is_relative_to(args.repo / ".tmp"):
        parser.error("all generated output must stay under this repository's .tmp")
    args.out.parent.mkdir(parents=True, exist_ok=True)
    # The renewed task cgroup has a CPU quota chosen by its owner. This
    # correctness runner requires the requested memory isolation, not the
    # unrelated benchmark topology of the full wasm3 CI harness.
    memberships = [line[3:] for line in Path("/proc/self/cgroup").read_text().splitlines() if line.startswith("0::")]
    if len(memberships) != 1 or ".." in Path(memberships[0]).parts:
        parser.error("one actual Linux cgroup v2 membership is required")
    candidates = [Path("/sys/fs/cgroup") / memberships[0].lstrip("/"), Path("/sys/fs/cgroup")]
    admitted = False
    for cg in candidates:
        try:
            admitted = (str(os.getpid()) in (cg / "cgroup.procs").read_text().split()
                        and (cg / "memory.max").read_text().strip() == "68719476736"
                        and (cg / "memory.swap.max").read_text().strip() == "0")
        except OSError:
            continue
        if admitted:
            break
    if not admitted:
        parser.error("this process must actually belong to the 64 GiB, zero-swap cgroup")
    args.harness = args.source_root / "test/0018.debugger/run_dap_current_broker.py"
    return args

def compile_source_fixture(args):
    """Compile the real SDK C algorithm; no host-language execution substitute."""
    if args.source_wasm:
        return args.source_wasm.resolve(strict=True)
    directory = args.sdk / "example/language/c/static_pointer_library"
    entry = args.out / "entry.c"
    entry.write_text('#include "algorithm.h"\nstatic uint32_t values[16];\n'
                     'void _start(void) { for (;;) (void)transform_and_sum(values, 16); }\n')
    module = args.out / "sdk-source.wasm"
    command = [args.wasm_clang, "--target=wasm32-unknown-unknown", "-ffreestanding", "-nostdlib",
               "-O0", "-g", "-gdwarf-5", "-fuse-ld=" + args.wasm_ld, "-Wl,--no-entry",
               "-Wl,--export=_start", "-I" + str(directory), str(directory / "algorithm.c"),
               str(entry), "-o", str(module)]
    with (args.out / "compile.log").open("w") as log:
        subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True)
    return module
