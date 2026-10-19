#!/usr/bin/env python3
"""Exercise the actual bounded reader against synthetic descriptors only.

The source is parsed with AST and only ArtifactMutation/read_artifact are loaded.
Every filesystem operation below is a fake, including open/read/fstat/close.
No FIFO, device, compiler, native test, Wasm, cgroup or ROS process is opened or
executed. The cases focus on admission before reads, bounded short/growth reads,
same-descriptor identity, and cleanup after every post-open exception.
"""
import argparse
import ast
import hashlib
import json
from pathlib import Path
import stat
import sys
import types

sys.dont_write_bytecode = True
SOURCE = Path(__file__).resolve().parents[1] / 'tools/build_cgroup_matrix.py'
LIMIT = 64 * 1024 * 1024


def metadata(**changes):
    """Return independent synthetic fstat facts, with a regular baseline."""
    values = dict(st_dev=29, st_ino=1001, st_size=4, st_mtime_ns=2,
                  st_ctime_ns=3, st_mode=stat.S_IFREG | 0o600)
    values.update(changes)
    return types.SimpleNamespace(**values)


class Filesystem:
    """Expose only fake descriptor operations; any pathname stat is forbidden."""
    O_RDONLY = 0
    O_NONBLOCK = 2048

    def __init__(self, data=b'\x00abc', before=None, after=None, chunk=None, fail=None):
        self.data, self.before = data, before or metadata(st_size=len(data))
        self.after = after or self.before
        self.chunk, self.fail = chunk, fail
        self.opened, self.closed, self.reads, self.fstats = [], [], [], []
        self.offset = 0

    def fspath(self, value):
        return str(value)

    def open(self, value, flags):
        self.opened.append((value, flags))
        if self.fail == 'open':
            raise OSError('synthetic open failure')
        assert flags & self.O_NONBLOCK, 'Open must be nonblocking before admission'
        return 71

    def fstat(self, descriptor):
        assert descriptor == 71
        self.fstats.append(descriptor)
        if self.fail == ('first-fstat' if len(self.fstats) == 1 else 'last-fstat'):
            raise OSError('synthetic fstat failure')
        return self.before if len(self.fstats) == 1 else self.after

    def read(self, descriptor, count):
        assert descriptor == 71 and 0 < count <= 1024 * 1024
        self.reads.append((descriptor, count))
        if self.fail == 'read':
            raise OSError('synthetic read failure')
        if self.chunk:
            count = min(count, self.chunk)
        result = self.data[self.offset:self.offset + count]
        self.offset += len(result)
        return result

    def close(self, descriptor):
        assert descriptor == 71
        self.closed.append(descriptor)


def main():
    """Run immutable source/mock cases and optionally save a fresh evidence file."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path, help='New report only; an existing file is refused')
    args = parser.parse_args()
    if args.report and args.report.exists():
        parser.error('report already exists')
    source_bytes = SOURCE.read_bytes()
    tree = ast.parse(source_bytes)
    nodes = [node for node in tree.body if
             isinstance(node, ast.ClassDef) and node.name == 'ArtifactMutation' or
             isinstance(node, ast.FunctionDef) and node.name == 'read_artifact']
    assert len(nodes) == 2, 'The actual reader and its mutation exception must each exist once'
    executable = compile(ast.Module(body=nodes, type_ignores=[]), str(SOURCE) + '<pure-descriptor-model>', 'exec')
    rows = []

    def case(name, fs, error=None, no_reads=False, no_open=False):
        scope = {'os': fs, 'stat': stat, 'hashlib': hashlib}
        exec(executable, scope)
        result = None
        observed_error = None
        try:
            result = scope['read_artifact']('/synthetic/input')
        except (OSError, ValueError, RuntimeError) as caught:
            observed_error = caught
        if error:
            expected_type = scope['ArtifactMutation'] if error == 'mutation' else error
            assert isinstance(observed_error, expected_type), (name, repr(observed_error))
        else:
            assert observed_error is None, (name, repr(observed_error))
            data, binding = result
            assert data == fs.data
            assert binding == dict(device=fs.before.st_dev, inode=fs.before.st_ino,
                                   size=len(fs.data), mtime_ns=fs.before.st_mtime_ns,
                                   ctime_ns=fs.before.st_ctime_ns,
                                   sha256=hashlib.sha256(fs.data).hexdigest())
        if no_reads:
            assert not fs.reads, name
        if no_open:
            assert not fs.opened and not fs.closed, name
        elif fs.fail == 'open':
            assert len(fs.opened) == 1 and not fs.closed, name
        else:
            assert fs.opened == [('/synthetic/input', fs.O_RDONLY | fs.O_NONBLOCK)], name
            assert fs.closed == [71], name
        rows.append({'name': name, 'status': 'PASS', 'reads': len(fs.reads),
                     'descriptor_closed': fs.closed == [71],
                     'exception': type(observed_error).__name__ if observed_error else None})

    case('regular binary bytes receive one exact identity/hash binding', Filesystem())
    case('empty regular file is bounded and admitted', Filesystem(data=b''))
    case('short individual reads accumulate without assuming a full read', Filesystem(chunk=1))
    for label, kind in [('FIFO', stat.S_IFIFO), ('character-device', stat.S_IFCHR),
                        ('directory', stat.S_IFDIR), ('socket', stat.S_IFSOCK)]:
        case(label + ' rejected before read', Filesystem(before=metadata(st_mode=kind | 0o600)),
             ValueError, no_reads=True)
    case('declared over64MiB rejected before read', Filesystem(before=metadata(st_size=LIMIT + 1)), ValueError, no_reads=True)
    case('negative declared size rejected before read', Filesystem(before=metadata(st_size=-1)), ValueError, no_reads=True)
    # The exact boundary is admitted for reading, but these four fake bytes do
    # not satisfy that declared length. No64MiB allocation is made by the model.
    case('64MiB boundary still requires exact byte length', Filesystem(before=metadata(st_size=LIMIT)), ValueError)
    fs = Filesystem(); fs.O_NONBLOCK = None
    case('missing O_NONBLOCK fails before open', fs, ValueError, no_reads=True, no_open=True)
    fs = Filesystem(); fs.O_NONBLOCK = 0
    case('unusable O_NONBLOCK fails before open', fs, ValueError, no_reads=True, no_open=True)
    case('truncated data rejected', Filesystem(data=b'abc', before=metadata(st_size=4)), ValueError)
    case('one extra growth byte rejected', Filesystem(data=b'abcde', before=metadata(st_size=4)), ValueError)
    for field in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns'):
        before = metadata()
        case(field + ' mutation rejected', Filesystem(before=before, after=metadata(**{field: getattr(before, field) + 1})), 'mutation')
    case('post-read file-kind mutation rejected', Filesystem(after=metadata(st_mode=stat.S_IFIFO | 0o600)), 'mutation')
    for failure in ('open', 'first-fstat', 'read', 'last-fstat'):
        case(failure + ' error preserves descriptor ownership', Filesystem(fail=failure), OSError,
             no_reads=failure in ('open', 'first-fstat'))
    assert SOURCE.read_bytes() == source_bytes, 'Reader source changed during models'
    report = {'status': 'SOURCE_ARTIFACT_READ_MODEL_PASS', 'checks': len(rows), 'rows': rows,
              'source': str(SOURCE), 'source_sha256': hashlib.sha256(source_bytes).hexdigest(),
              'scope': 'Actual extracted reader with wholly synthetic descriptor operations',
              'filesystem_artifact_execution': 'NOT_PERFORMED', 'native_compilation': 'NOT_PERFORMED',
              'WebAssembly_execution': 'NOT_PERFORMED', 'cgroup_execution': 'NOT_PERFORMED',
              'ROS_execution': 'NOT_PERFORMED'}
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        with args.report.open('x') as output:
            output.write(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report if args.report else {key: value for key, value in report.items() if key != 'rows'}))
    return 0


if __name__ == '__main__':
    sys.exit(main())
