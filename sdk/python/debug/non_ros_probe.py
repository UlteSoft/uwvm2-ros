"""Freeze as roswasm_app; actual C trace provider, argument and DBG probe.

Unexecuted Source. The Linux debugger must observe actual stopped snapshots;
normal stdout/assertions alone cannot qualify frame/typed-value debugging.
"""
import gc
import sys


class NoInspectionCalls:
    calls = 0

    @property
    def danger(self):
        type(self).calls += 1
        raise AssertionError("debugger invoked a property")

    def __repr__(self):
        type(self).calls += 1
        raise AssertionError("debugger invoked repr")

    def __iter__(self):
        type(self).calls += 1
        raise AssertionError("debugger invoked iteration")


def recursive(depth, seed):
    scalar = seed + depth
    unsigned = 0xFEDCBA9800000007
    negative_zero = -0.0
    text = "python\x00\u03a9\U0001f916"
    binary = b"\x00\xff\x01"
    structured = {"actual": [scalar, unsigned, None, False], 7: (text, binary)}
    cycle = []
    cycle.append(cycle)
    unsupported = NoInspectionCalls()
    # A real line breakpoint here must expose this frame and its callers.
    observed = (scalar, unsigned, negative_zero, text, binary, structured, cycle, unsupported)
    if depth:
        result = recursive(depth - 1, seed)
    else:
        gc.collect()  # The next trace snapshot must describe actual post-GC values.
        result = scalar + 7
    assert observed[0] == seed + depth
    assert observed[1] == unsigned
    assert structured["actual"][0] == scalar
    assert cycle[0] is cycle
    return result + depth


def main():
    if len(sys.argv) != 2:
        raise ValueError("pass exactly one integer application argument")
    seed = int(sys.argv[1])
    result = recursive(2, seed)
    assert result == seed + 10
    assert NoInspectionCalls.calls == 0
    print("PYTHON_DEBUG_COMPLETED", result)
    return 0
