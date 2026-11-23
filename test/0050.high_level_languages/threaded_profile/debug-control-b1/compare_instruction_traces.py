#!/usr/bin/env python3
"""Compare the actual periodic opcode traces of the B1 loop fixtures.

This checks executed Wasm coordinates, not native addresses, instruction labels
or merely the set of positions that appeared. It is specific to these periodic
fixtures; it is not a validator for arbitrary programs.
"""
import argparse
import json
import re
from pathlib import Path


def cycle(path):
    matches = re.findall(
        r"^B1 ADVANCE step=(\d+) function=(\d+) offset=(\d+) instruction=\d+$",
        path.read_text(),
        re.MULTILINE,
    )
    if len(matches) < 128 or [int(row[0]) for row in matches] != list(range(len(matches))):
        raise ValueError(f"{path}: incomplete or duplicated native trace")
    trace = [(int(row[1]), int(row[2])) for row in matches]
    # A genuine early pause can precede entry into the steady loop. Require
    # three complete repetitions after a small explicitly reported prefix.
    for skipped in range(min(64, len(trace) // 4) + 1):
        remaining = trace[skipped:]
        for length in range(1, len(remaining) // 3 + 1):
            if all(value == remaining[index % length] for index, value in enumerate(remaining)):
                period = remaining[:length]
                canonical = min(period[index:] + period[:index] for index in range(length))
                return {"samples": len(trace), "initial_prefix": skipped, "period": canonical}
    raise ValueError(f"{path}: no complete periodic instruction trace")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("interpreter", type=Path)
    parser.add_argument("llvm", type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    interpreted, compiled = cycle(args.interpreter), cycle(args.llvm)
    result = {"interpreter": interpreted, "llvm": compiled, "equal": interpreted["period"] == compiled["period"]}
    args.output.write_text(json.dumps(result, indent=2) + "\n")
    print(f"executed opcode cycles: interpreter={len(interpreted['period'])} llvm={len(compiled['period'])} equal={result['equal']}")
    return 0 if result["equal"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
