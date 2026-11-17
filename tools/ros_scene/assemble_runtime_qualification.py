#!/usr/bin/env python3
"""Join completed safety receipts for one exact VM without widening their scope.

Case totals include exposure/refusal tests and narrow simulation fixtures. They
do not establish provider semantics for every registered operation, vendor
qualification, other native architectures, or physical safety certification.
Supporting core receipts retain their own source identities; they are never
counted as executions of the selected VM.
"""
import argparse
import json
from pathlib import Path

from performance_build_policy import vm_policy
from module_exchange_performance import GATE, ROOT, sha, verify_gate


def events(receipt):
    """Accept documented historical field spellings, but never absent evidence."""
    for prefix in ('events_', 'memory_events_'):
        if prefix+'before' in receipt and prefix+'after' in receipt:
            before, after = (receipt[prefix+suffix] for suffix in ('before', 'after'))
            assert isinstance(before, str) and isinstance(after, str)
            parse = lambda text: {key: int(value) for key, value in
                                  (line.split() for line in text.splitlines())}
            previous, current = parse(before), parse(after)
            assert previous.keys() == current.keys()
            assert all(current[key] >= previous[key] for key in previous)
            assert all(current[key] == previous[key] for key in ('oom', 'oom_kill', 'oom_group_kill'))
            return {key: current[key]-previous[key] for key in previous}
    raise AssertionError('Missing before/after cgroup event evidence')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--receipt', type=Path, action='append', required=True)
    parser.add_argument('--core', type=Path, action='append', default=[])
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    runtime = args.runtime.resolve()
    runtime_digest = sha(runtime)
    build_path = runtime.parent/'full-build-results.json'
    build = json.loads(build_path.read_text())
    assert build['complete'] and build['sources_unchanged']
    assert build['binary_sha256'] == runtime_digest
    source = Path(build['source_root'])
    manifest_path = source/'source-manifest.json'
    manifest = json.loads(manifest_path.read_text())
    assert all(sha(source/name) == digest for name, digest in manifest.items())
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    assert not out.exists()
    report = dict(complete=False, scope=__doc__, runtime=str(runtime), runtime_sha256=runtime_digest,
                  vm_compile_policy=vm_policy(runtime), build_receipt_sha256=sha(build_path),
                  source_manifest_sha256=sha(manifest_path), entries=[], core=[], cases=0)
    unique = set()
    for kind, paths in (('entries', args.receipt), ('core', args.core)):
        for original in paths:
            path = original.resolve()
            assert path not in unique, 'Duplicate receipt'
            unique.add(path)
            value = json.loads(path.read_text())
            assert value['complete'] is True, str(path)
            assert value['cgroup'] == str(GATE)
            assert value['memory_max'] == 68719476736 and value['swap_max'] == 0
            entry = dict(path=str(path), sha256=sha(path), scope=value.get('scope'),
                         memory_event_delta=events(value))
            if kind == 'entries':
                binding = value.get('runtime_sha256')
                if binding is None:
                    # Older DAP receipts pin the executable among their inputs.
                    binding = value.get('inputs', {}).get(str(runtime))
                assert binding == runtime_digest, ('Different or unpinned VM', str(path))
                cases = value['cases']
                assert isinstance(cases, list) and cases
                assert all(case.get('passed', case.get('complete')) is True for case in cases), str(path)
                entry['cases'] = len(cases)
                report['cases'] += len(cases)
            else:
                entry['source_root'] = value.get('source_root')
                entry['source_roots'] = value.get('source_roots')
                entry['vm_execution_evidence'] = False
            report[kind].append(entry)
    verify_gate()
    report['complete'] = True
    out.parent.mkdir(parents=True, exist_ok=True)
    with out.open('x') as stream:
        json.dump(report, stream, indent=2)
        stream.write('\n')
    print(f'{report["cases"]} exact-VM cases; {len(report["core"])} separate core receipts: {out}')


if __name__ == '__main__':
    main()
