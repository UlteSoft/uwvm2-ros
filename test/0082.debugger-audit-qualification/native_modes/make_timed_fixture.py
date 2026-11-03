"""Linux-only deterministic workload/oracle producer, executed through the gate.

The independent Python recurrence computes the final checksum. Every mode then
uses exactly one compiled Wasm; no debugger-specific Wasm transformation exists.
"""
from pathlib import Path
import argparse
import json
import sys


def main():
    if sys.platform != 'linux':
        raise RuntimeError('Fixture production is Linux only')
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--template', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    state = 1
    memory = [0] * 1024
    for index in range(1 << 20):
        state ^= (state << 13) & 0xffffffff
        state ^= state >> 17
        state ^= (state << 5) & 0xffffffff
        if state == 0:
            raise RuntimeError("Nonzero xorshift invariant failed")
        memory[index & 1023] = state
    checksum = sum(memory) & 0xffffffff
    template = args.template.read_text()
    if template.count('@CHECKSUM@') != 1:
        raise RuntimeError('Exactly one checksum substitution is required')
    with args.out.open('x') as output:
        output.write(template.replace('@CHECKSUM@', str(checksum)))
    print(json.dumps({'algorithm': 'xorshift32(13,17,5), seed 1, 1048576 updates, 1024-cell final unsigned sum',
                      'updates': 1 << 20, 'checksum': checksum, 'final_state': state}))


if __name__ == '__main__':
    main()
