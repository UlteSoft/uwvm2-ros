#!/usr/bin/env python3
"""Verify every retained Leju checkpoint pair and independent client receipt.

Run inside the original hard64GiB/swap0 cgroup using its explicitly saved input
identity. This is inert file/receipt verification. It neither extracts archives
nor executes their SDKs, changes limits, removes evidence, or runs robot code.
An optional cleanup receipt additionally proves the owned tmpfs was retired.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def read(path):
    return json.loads(path.read_bytes())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inputs', type=Path, required=True)
    parser.add_argument('--primary', type=Path, required=True)
    parser.add_argument('--replica', type=Path, required=True)
    parser.add_argument('--cleanup', type=Path)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_AS, (256 << 20, 256 << 20))
    resource.setrlimit(resource.RLIMIT_CPU, (300, 300))
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    inputs = read(args.inputs)
    group = Path(inputs['cgroup']['path'])
    events = (group / 'memory.events').read_text()

    def guard():
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
        info = group.stat()
        assert [info.st_dev, info.st_ino] == inputs['cgroup']['identity']
        assert (group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (group / 'memory.swap.max').read_text().strip() == '0'
        after = dict(line.split() for line in (group / 'memory.events').read_text().splitlines())
        assert all(after[k] == '0' for k in ('oom', 'oom_kill', 'oom_group_kill'))

    guard()
    assert not args.out.exists() and args.out.is_absolute()
    checkpoints = []
    for path in sorted(args.primary.glob('*.json')):
        proof = read(path)
        if proof.get('status') != 'TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS':
            continue
        label = path.stem
        assert (args.replica / path.name).read_bytes() == path.read_bytes()
        assert proof['default_authority'] == []
        for parent, role in ((args.primary, 'primary'), (args.replica, 'replica')):
            archive = parent / (label + '.tar.gz')
            assert str(archive) == proof[role]
            assert archive.stat().st_size == proof['bytes'] and sha(archive) == proof['sha256']
            client_path = parent / 'client-readbacks' / (label + '-client-readback.json')
            client = read(client_path)
            assert client['status'] == 'CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS'
            assert client['sha256'] == proof['sha256'] and client['bytes'] == proof['bytes']
            assert client['manifest_sha256'] == proof['manifest_sha256']
            assert client['objects'] == proof['objects'] and client['roles'] == proof['roles']
            assert client['compiler_or_ros_tests_run'] == 0
            assert not client['archive_objects_extracted_or_executed']
        assert (args.primary / 'client-readbacks' / (label + '-client-readback.json')).read_bytes() == (
            args.replica / 'client-readbacks' / (label + '-client-readback.json')).read_bytes()
        checkpoints.append(dict(label=label, bytes=proof['bytes'], sha256=proof['sha256']))
        guard()
    assert checkpoints
    expected = {row['label'] + '.tar.gz' for row in checkpoints}
    for parent in (args.primary, args.replica):
        assert {p.name for p in parent.glob('*.tar.gz')} == expected
    cleanup = None
    if args.cleanup:
        cleanup = read(args.cleanup)
        assert cleanup['status'] == 'OWNED_LEJU_TMPFS_NORMALLY_UNMOUNTED_AFTER_FULL_RETAINED_CLIENT_READBACK'
        root = Path(inputs['source_root']).parent
        assert cleanup['original_root'] == str(root) and not cleanup['root_exists'] and not root.exists()
        assert cleanup['checkpoint_sha256'] in {row['sha256'] for row in checkpoints}
        assert cleanup['persistent_sdk_inputs_preserved'] and not cleanup['other_roots_or_limits_modified']
    guard()
    report = dict(status='ALL_LEJU_CHECKPOINT_PAIRS_AND_CLIENT_READBACKS_VERIFIED' if cleanup is None else
                  'FINAL_LEJU_CHECKPOINT_PAIRS_CLIENT_READBACKS_AND_NORMAL_CLEANUP_VERIFIED',
                  checkpoints=checkpoints, count=len(checkpoints),
                  retained_archive_bytes_per_copy=sum(row['bytes'] for row in checkpoints),
                  cleanup=cleanup, default_authority=[], events_before=events,
                  events_after=(group / 'memory.events').read_text(),
                  archive_content_extracted_or_executed=False)
    args.out.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(status=report['status'], count=report['count'],
                          retained_archive_bytes_per_copy=report['retained_archive_bytes_per_copy'])), flush=True)


if __name__ == '__main__':
    main()
