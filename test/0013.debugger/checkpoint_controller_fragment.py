#!/usr/bin/env python3
"""Extract the current checkpoint branch for the controller formatter fixture.

This tests control-flow/formatting only; it does not replace native capsule tests.
"""
import argparse
import hashlib
from pathlib import Path

def extract(source):
    marker = "            if(query.operation==wasip1_state::action::checkpoint_save ||"
    begin = source.index(marker)
    opening = source.index("{", begin)
    depth = 0
    for index in range(opening, len(source)):
        if source[index] == "{": depth += 1
        elif source[index] == "}":
            depth -= 1
            if depth == 0: return source[begin:index+1] + "\n"
    raise ValueError("checkpoint branch is not balanced")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--controller", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    raw = args.controller.read_bytes()
    text = extract(raw.decode())
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(text)
    print("controller_sha256=" + hashlib.sha256(raw).hexdigest())
    print("fragment_sha256=" + hashlib.sha256(text.encode()).hexdigest())
