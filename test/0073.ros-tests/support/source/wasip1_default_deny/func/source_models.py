#!/usr/bin/env python3
"""Review authority source placement and independent masks without compiling.

This script only reads UTF-8 source and runs Python text/mask models. It neither
executes a host API nor proves C++ compilation, syscall isolation, guest
execution, ROS behavior, or ARM behavior. Run authority.cc in the qualified
Linux cgroup to obtain those native API results separately.
"""
from __future__ import annotations

import pathlib
import re
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[6]
FUNC = ROOT / "src/uwvm2/imported/wasi/wasip1/func"
NS = "::uwvm2::imported::wasi::wasip1::"


def clock_contract(source: str, native_operation: str) -> bool:
    """Require validation before authorization and authorization before effects."""
    invalid = source.index("::einval;")
    check = source.index("env.permits(" + NS + "environment::clock_permission(clock_id))")
    denial = source.index("::enotcapable;", check)
    native = source.index("::fast_io::" + native_operation)
    store = source.index("store_basic_wasm_type_to_memory_")
    return invalid < check < denial < native < store


def simple_contract(source: str, permission: str, first_effect: str) -> bool:
    check = source.index("env.permits(" + NS + "environment::permission_t::" + permission + ")")
    denial = source.index("::enotcapable;", check)
    return check < denial < source.index(first_effect)


def poll_contract(source: str, width: int) -> bool:
    """Check the all-subscription preflight and each separate platform branch."""
    event = "eventtype_wasm64_t" if width == 64 else "eventtype_t"
    rights = "rights_wasm64_t" if width == 64 else "rights_t"
    start = source.index("// subscriptions.size() == nsubscriptions")
    preflight = source.index("for(auto const& sub : subscriptions)", start)
    permission = source.index("environment::clock_permission(sub.u.u.clock.id)", preflight)
    invalid = source.index("::einval;", permission)
    check = source.index("if(!env.permits(requested))", invalid)
    denial = source.index("::enotcapable;", check)
    single_clock = source.index("if(nsubscriptions == 1u", start)
    if not start < preflight < permission < invalid < check < denial < single_clock:
        return False
    # The decoder finishes before preflight. Native facilities occur only after
    # the complete preflight; tracing has a separate operator-selected target.
    native_markers = ("::fast_io::this_thread::sleep_for(", "::fast_io::posix_clock_gettime(",
                      "::fast_io::system_call<", "::uwvm2::imported::wasi::wasip1::func::posix::poll(",
                      "::uwvm2::imported::wasi::wasip1::func::posix::select(")
    for marker in native_markers:
        if marker in source and source.index(marker) < denial:
            return False
    blocks = re.findall(r"auto const required_rights\{(.*?)\};", source, re.S)
    if len(blocks) != 6:
        return False
    expected = (
        NS + "abi::" + rights + "::right_poll_fd_readwrite |",
        "sub.u.tag == " + NS + "abi::" + event + "::eventtype_fd_read",
        "? " + NS + "abi::" + rights + "::right_fd_read",
        ": " + NS + "abi::" + rights + "::right_fd_write",
    )
    for block in blocks:
        normalized = " ".join(block.split())
        if not all(item in normalized for item in expected):
            return False
    return source.count("if((curr_fd.rights_base & required_rights) != required_rights)") == 6


class SourceModels(unittest.TestCase):
    def test_clock_ordering_and_gate_mutations(self) -> None:
        for width in (32, 64):
            suffix = "_wasm64" if width == 64 else ""
            for name, operation in (("clock_res_get", "posix_clock_getres("),
                                    ("clock_time_get", "posix_clock_gettime(")):
                source = (FUNC / (name + suffix + ".h")).read_text()
                self.assertTrue(clock_contract(source, operation))
                with self.assertRaises(ValueError):
                    clock_contract(source.replace("env.permits(", "env.permission_bypass(", 1), operation)

    def test_random_and_shared_callback_gates(self) -> None:
        for suffix in ("", "_wasm64"):
            source = (FUNC / ("random_get" + suffix + ".h")).read_text()
            self.assertTrue(simple_contract(source, "random", "::fast_io::native_white_hole{}"))
        for name in ("proc_raise", "sched_yield"):
            source = (FUNC / (name + ".h")).read_text()
            self.assertTrue(simple_contract(source, name, "if(env.wasip1_" + name + "_func_ptr"))
            with self.assertRaises(ValueError):
                simple_contract(source.replace("env.permits(", "env.permission_bypass(", 1), name,
                                "if(env.wasip1_" + name + "_func_ptr")

    def test_poll_array_and_all_platform_paths(self) -> None:
        for width in (32, 64):
            suffix = "_wasm64" if width == 64 else ""
            source = (FUNC / ("poll_oneoff" + suffix + ".h")).read_text()
            self.assertTrue(poll_contract(source, width))
            with self.assertRaises(ValueError):
                poll_contract(source.replace("if(!env.permits(requested))", "if(false)", 1), width)
            # Each path is attacked independently. Five correct branches cannot
            # hide one branch that omits or swaps the subscribed direction.
            for index in range(6):
                blocks = list(re.finditer(r"auto const required_rights\{(.*?)\};", source, re.S))
                start, end = blocks[index].span()
                bad = source[:start] + source[start:end].replace("::right_fd_read", "::right_fd_write", 1) + source[end:]
                self.assertFalse(poll_contract(bad, width))
                bad = source[:start] + source[start:end].replace("::right_poll_fd_readwrite |", "::right_poll_fd_readwrite &", 1) + source[end:]
                self.assertFalse(poll_contract(bad, width))

    def test_independent_authority_mask(self) -> None:
        # These numbers are intentionally declared here, not obtained from the
        # production enum: this is an independent truth-table expectation.
        expected = {"clock_realtime": 1, "clock_monotonic": 2,
                    "clock_process_cputime": 4, "clock_thread_cputime": 8,
                    "random": 16, "proc_raise": 32, "sched_yield": 64,
                    "stdio_full_rights": 128}
        source = (ROOT / "src/uwvm2/imported/wasi/wasip1/environment/environment.h").read_text()
        for name, bit in expected.items():
            self.assertRegex(source, rf"\b{re.escape(name)}\s*=\s*{bit}u?\b")
        for mask in range(256):
            for request in range(256):
                permitted = request != 0 and (mask & request) == request
                missing = [bit for bit in expected.values() if request & bit and not mask & bit]
                self.assertEqual(permitted, bool(request) and not missing)


if __name__ == "__main__":
    unittest.main()
