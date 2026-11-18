#!/usr/bin/env python3
"""Verify actual native DDS run receipts and rejected evidence substitutions."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'tools/ros_scene'))
from rmw_native_wire import verify


def refused(path):
    try:
        verify(path)
    except ValueError:
        return
    raise AssertionError('mismatched native wire evidence accepted')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--passed', type=Path, action='append', default=[])
    parser.add_argument('--failed', type=Path, action='append', default=[])
    args = parser.parse_args()
    if not args.passed and not args.failed:
        parser.error('supply at least one actual fixture receipt')
    for path in args.failed:
        assert verify(path)['status'] == 'failed'
    for path in args.passed:
        checked = verify(path)
        assert checked['status'] == 'passed' and checked['response'] == 1 + checked['challenge']
        source = json.loads(path.read_bytes())
        with tempfile.TemporaryDirectory() as temp:
            out = Path(temp)
            for name in ('ros-client', 'cyclone-echo', 'ros-echo', 'Int32.c', 'Int32.h', 'ros.log', 'cyclone.log'):
                if name in source['outputs']:
                    (out/name).symlink_to((path.parent/name).resolve(strict=True))
            exchange = json.loads((path.parent/'exchange.json').read_bytes())
            exchange['expected_response'] += 1
            data = json.dumps(exchange).encode()
            (out/'exchange.json').write_bytes(data)
            receipt = out/'results.json'
            receipt.write_text(json.dumps({**source, 'outputs': {**source['outputs'], 'exchange.json': hashlib.sha256(data).hexdigest()}}))
            refused(receipt)
            (out/'exchange.json').write_bytes((path.parent/'exchange.json').read_bytes())
            receipt.write_text(json.dumps({**source, 'reply_peer': {**source['reply_peer'], 'rmw': 'rmw_fastrtps_cpp'}}))
            refused(receipt)
            receipt.write_text(json.dumps({**source, 'steps': source['steps'][:-1]}))
            refused(receipt)
    print('PASS native wire receipt checks')


if __name__ == '__main__':
    main()
