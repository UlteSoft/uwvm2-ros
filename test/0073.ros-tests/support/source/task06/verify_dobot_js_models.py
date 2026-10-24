#!/usr/bin/env python3
"""Check actual dual-version DOBOT local models with original complete inputs.

This new1MiB scope reads all four retained alarm tables externally and writes
small source/model receipts. It does not copy an SDK, invoke a compiler, create
a ROS/native peer or reduce the unchanged96MiB SDK-build plan.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def pin(path):
    raw = path.read_bytes()
    return dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('source', 'alarms', 'originals', 'model', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same finite Linux owner/common lock required')
    args.out.mkdir()
    pins = []
    for path in [Path(__file__), args.model, Path('/usr/bin/node'),
                 Path('/usr/share/nodejs/acorn/dist/acorn.mjs'),
                 *sorted(path for path in args.source.rglob('*') if path.is_file())]:
        pins.append(pin(path))
    for version, wanted in ((3, '63f01dd6315ba988db1e76036d8a16baf472ef7ab97954ce96bdbdcffb069a56'),
                            (4, 'aa54d664b468ac24d26e94fe59d9222c26ec7e73f8221b233873059640e09252')):
        path = args.source / ('include/ros1/rosdemo-v' + str(version) + '/bounded_records.mjs')
        if pin(path)['sha256'] != wanted:
            raise RuntimeError('genuine complete original generated codec pin changed')
    expected = {
        'v3/alarm_controller.json': '28ae41285187582c57a2647b3cfada6adb87217a04b9eae01443bc7211b5fb8b',
        'v3/alarm_servo.json': 'd1a7152a0a42e467690886615869c291588d1ddc2127c1f657864feb2ea7014d',
        'v4/alarm_controller.json': 'f7b369a64bf1f00ec8320d23ca8333a298e12867476bc1fe19cd3c47ce97a310',
        'v4/alarm_servo.json': '19e577366d8568d5285ef8f7f1a6b262b3f8b53d302d5328916f5377d0bc01d8'
    }
    for relative, wanted in expected.items():
        row = pin(args.alarms / relative)
        if row['sha256'] != wanted:
            raise RuntimeError('complete original alarm file changed: ' + relative)
        pins.append(row)
    original_hashes = {
        'v3/main.cpp': '58262d143c024d53358ebfc3c87624c93895e7affbd69ce311f9a885e9070485',
        'v3/rosDemoCRV3.cpp': 'fef1123d12b43e8cf3d60049f77918b1df38b85e3773491dbf93ee9298fac40e',
        'v4/main.cpp': '7e951fe8f27bf45d449f210163bfe110a6511f387245f65f68b28948e5ac4b8a',
        'v4/rosDemoCRV4.cpp': '04f1b2f8fcf85792993eaf4d2b573852b36483811d9392f1dd69d85e4f81fbd9'
    }
    requirements = {
        'v3/main.cpp': ['serviceHandler.movePoint(pointA)', 'serviceHandler.finishPoint(pointB)', 'threadMove.detach()'],
        'v3/rosDemoCRV3.cpp': ['feedbackData.ErrorStatus && feedbackData.EnableStatus', 'if (0)',
                             '(!feedbackData.RunQueuedCmd)', 'std::abs(value1 - value2) < 0.001', 'srvJointMovJ.request.j6 = point[5]'],
        'v4/main.cpp': ['serviceHandler.movePoint(pointA, currentCommandID)', 'serviceHandler.finishPoint(currentCommandID)'],
        'v4/rosDemoCRV4.cpp': ['errorIdNew == errorId', 'feedbackData.CurrentCommandID > currentCommandID',
                             '(feedbackData.RobotMode == 5)', 'srvMovJ.request.mode = 1']
    }
    for relative, tokens in requirements.items():
        path = args.originals / relative
        row = pin(path)
        if row['sha256'] != original_hashes[relative]:
            raise RuntimeError('exact original main/body pin changed: ' + relative)
        pins.append(row)
        source = path.read_text()
        if any(token not in source for token in tokens):
            raise RuntimeError('reviewed original active workflow changed: ' + relative)
    ast.parse(Path(__file__).read_bytes())
    json.loads((args.source / 'example/official/demo.recipe.json').read_bytes())
    commands = [['/usr/bin/node', '--version'],
                ['/usr/bin/node', '--check', str(args.source / 'include/demo_cycle.mjs')],
                ['/usr/bin/node', '--check', str(args.source / 'example/official/demo_cycle.mjs')],
                ['/usr/bin/node', args.model, '--source', args.source, '--alarms', args.alarms,
                 '--out', args.out / 'model-result.json']]
    receipts = []
    for index, command in enumerate(commands):
        argv = list(map(str, command))
        result = subprocess.run(argv, capture_output=True, timeout=120, check=False)
        (args.out / ('command-' + str(index) + '.stdout')).write_bytes(result.stdout)
        (args.out / ('command-' + str(index) + '.stderr')).write_bytes(result.stderr)
        receipts.append(dict(argv=argv, exit=result.returncode))
        (args.out / 'commands.json').write_text(json.dumps(receipts, indent=2) + '\n')
        if result.returncode:
            raise RuntimeError('actual source/model command failed: ' + str(index))
    for row in pins:
        if pin(Path(row['path'])) != row:
            raise RuntimeError('immutable source/tool/original input changed')
    model = json.loads((args.out / 'model-result.json').read_bytes())
    summary = dict(status=model['status'], cases=len(model['cases']),
                   public_api_items=len(model['public_api_inventory']),
                   sdk_builds=0, wasm_validation=0, guest=0, native_ros=0,
                   official_original_process_runs=0, shared_producer_publication=False)
    (args.out / 'source-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
