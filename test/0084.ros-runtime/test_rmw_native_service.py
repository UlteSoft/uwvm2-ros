"""Verify a real service receipt and reject a swapped, re-hashed pair of results."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT/'tools/ros_scene'))
from rmw_native_wire import verify_service


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--passed', required=True, type=Path)
    parser.add_argument('--failed', type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--wire-remap', action='append', nargs=2, default=[], type=Path)
    args = parser.parse_args()
    passed = args.passed.resolve(strict=True)
    row = verify_service(passed, args.wire_remap)
    if row['status'] != 'passed':
        raise ValueError('expected a completed native service exchange')
    if args.failed and verify_service(args.failed, args.wire_remap)['status'] != 'failed':
        raise ValueError('expected a failed service observation')
    output = args.output.resolve()
    if '.tmp' not in output.parts:
        raise ValueError('choose an output inside the project .tmp')
    output.mkdir(parents=True, exist_ok=False)
    with tempfile.TemporaryDirectory(dir=output) as directory:
        copy = Path(directory)
        report = json.loads(passed.read_bytes())
        for name in ('ros-client','ros-echo','ros.log','cyclone.log','exchange.json'):
            shutil.copyfile(passed.parent/name, copy/name)
        exchange = copy/'exchange.json'
        values = json.loads(exchange.read_bytes())
        values['response_values'].reverse()
        exchange.write_text(json.dumps(values))
        report['outputs']['exchange.json'] = hashlib.sha256(exchange.read_bytes()).hexdigest()
        altered = copy/'results.json'
        altered.write_text(json.dumps(report))
        try:
            verify_service(altered, args.wire_remap)
        except ValueError as exc:
            if 'request/reply correlation differs' not in str(exc):
                raise
        else:
            raise AssertionError('interchanged service replies passed review')
    (output/'results.json').write_text(json.dumps(dict(complete=True, passed=row['receiptSha256'],
        failedObserved=bool(args.failed), swappedRejected=True),indent=2)+'\n')


if __name__ == '__main__':
    main()
