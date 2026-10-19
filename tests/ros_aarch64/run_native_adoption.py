#!/usr/bin/env python3
"""Prepare/execute the separate real RMW Action QoS adoption witness.

Use the same required frozen source, current rootfs, QEMU/binfmt lease,
container start identity and private mount namespace arguments documented by
run_action_qos.py. This wrapper selects exactly one native executable; its
client and server each verify forty explicitly configured fields through
sixteen official actual-profile queries, then exchange one real Action goal,
feedback and result through independent RCL contexts. It does not mutate QoS,
prove Wasm admission, reuse historical products, or create a cgroup.

Preparation and execution are separate actual scopes. An unexecuted build is
never a ROS behavior PASS. All strict dependency, target mapping, current
64 GiB/OOM, immutable input and owned cleanup checks remain in the shared
driver. Both this wrapper and that driver's bytes are archived and pinned.
"""
import sys
sys.dont_write_bytecode = True
import run_action_qos


def main():
    arguments = sys.argv[1:]
    if any(value == '--probe-set' or value.startswith('--probe-set=') for value in arguments):
        raise SystemExit('The adoption wrapper fixes its independent probe set; use the general driver to select another suite.')
    return run_action_qos.main(['--probe-set','native-adoption',*arguments])


if __name__ == '__main__':
    raise SystemExit(main())
