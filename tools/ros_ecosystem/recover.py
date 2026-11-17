#!/usr/bin/env python3
"""Retry observed transient source failures in a separate evidence tree.

No successful primary snapshots are downloaded again or overwritten. This
command does not execute fetched code and can run beside primary acquisition.
"""
from concurrent.futures import ThreadPoolExecutor, as_completed
import argparse
from collections import Counter
import json
from pathlib import Path
from acquire import acquire, save
from reconcile import primary_receipts, reconcile, transient_failure


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--jobs', type=int, default=4)
    args = parser.parse_args()
    root = args.root.resolve()
    rows = [r['source'] for r in primary_receipts(root).values()
            if r['status'] == 'FAILED' and transient_failure(r.get('error', ''))]
    save(root / 'recovery/catalog.json', dict(sources=rows, selection='observed-transient-primary-failures'))
    print('TRANSIENT_RECOVERY', len(rows), flush=True)
    results = []
    with ThreadPoolExecutor(max_workers=max(1, min(args.jobs, 8))) as pool:
        pending = [pool.submit(acquire, root / 'recovery', row) for row in rows]
        for future in as_completed(pending):
            results.append(future.result())
            if len(results) % 25 == 0 or len(results) == len(rows):
                status = dict(finished=len(results), total=len(rows), statuses=dict(Counter(r['status'] for r in results)))
                save(root / 'recovery/progress.json', status)
                print(json.dumps(status), flush=True)
    print(json.dumps(reconcile(root), indent=2), flush=True)


if __name__ == '__main__':
    main()
