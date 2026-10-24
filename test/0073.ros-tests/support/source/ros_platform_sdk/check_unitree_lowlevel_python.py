"""Run the actual offline full-record GO/HG Python adapter on Linux.

No native ROS bridge or replacement backend is installed. These mathematical
representation checks do not open endpoints, execute robot programs or qualify
any original middleware, firmware, controller or hardware behavior.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import sys


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    sys.modules[name] = result
    spec.loader.exec_module(result)
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--generated', type=Path, required=True)
    p.add_argument('--type', required=True)
    p.add_argument('--mode', choices=('values', 'contract', 'legacy-refusal'), required=True)
    a = p.parse_args()
    sys.path.insert(0, str(a.source/'sdk/origin/python'))
    kit = a.source/'sdk/origin/robots/unitree'
    helper = load('roswasm.unitree_lowlevel', kit/'python/lowlevel.py')
    contract = load('roswasm.unitree_lowlevel_contract', kit/'tests/lowlevel_python_contract.py')
    records = load('_actual_complete_unitree_lowlevel_records', a.generated/'bounded_records.py')
    proof = json.loads((a.generated/'generation.python.json').read_bytes())
    codec = getattr(records, next(r['symbol'] for r in proof['records'] if r['type'] == a.type))
    if a.mode == 'legacy-refusal':
        try: helper.LowLevelCommands(codec)
        except ValueError: print('PASS_UNITREE_LEGACY_LOWLEVEL_LAYOUT_REFUSED')
        else: raise AssertionError('legacy ROS1 admitted to a ROS2 checksum layout')
    elif a.mode == 'values': contract.values(codec, sys.stdout.buffer)
    else: print('PASS_UNITREE_LOWLEVEL_PYTHON_CONTRACT', contract.check(codec))


if __name__ == '__main__': main()
