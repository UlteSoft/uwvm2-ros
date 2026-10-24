"""Run actual Unitree Python payloads/contracts under an admitted Linux owner."""
import argparse
import importlib.util
import json
from pathlib import Path
import sys


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--generated', type=Path, required=True)
    p.add_argument('--mode', choices=('contract', 'values'), required=True)
    a = p.parse_args()
    sys.path[:0] = [str(a.source/'sdk/origin/python'), str(a.generated)]
    kit = a.source/'sdk/origin/robots/unitree'
    module = load('roswasm.unitree_sport', kit/'python/sport.py')
    contract = load('roswasm.unitree_sport_contract', kit/'tests/sport_python_contract.py')
    upstream = kit/'upstream/example-review-r1/unitree_ros2/files/example/src'
    oracle = load('unitree_original_sport_oracle', Path(__file__).with_name('unitreesport_remaining.py'))
    original = oracle.original_operations((upstream/'include/common/ros2_sport_client.h').read_text(),
                                           (upstream/'src/common/ros2_sport_client.cpp').read_text())
    assert {n:dict(id=i, kind=k) for n,(i,k) in module.OPERATIONS.items()} == original
    if a.mode == 'contract':
        print('PASS_UNITREE_SPORT_PYTHON_CONTRACT', contract.check())
    else:
        contract.values(sys.stdout.buffer)


if __name__ == '__main__': main()
