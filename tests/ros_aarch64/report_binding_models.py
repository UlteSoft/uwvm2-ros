#!/usr/bin/env python3
"""Pure descriptor models for the shared immutable JSON report reader.

Only read_json_binding is extracted from the current runtime.py source. Every
open/read/fstat/close below is mocked; no report artifact, pipe, device, live
cgroup, native program, QEMU, compiler, Wasm or ROS process is opened/executed.
The independent expected JSON/hash and descriptor facts test one-read binding,
pre-read admission, exact length, mutation refusal and cleanup after errors.
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
SOURCE = Path(__file__).with_name('runtime.py')
RAW = b'{"status":"PASS","value":7}'


def metadata(**changes):
    """Independent opened-descriptor facts; the baseline is a regular file."""
    values = dict(st_mode=stat.S_IFREG | 0o600, st_dev=29, st_ino=1234,
                  st_size=len(RAW), st_mtime_ns=10, st_ctime_ns=11)
    values.update(changes)
    return types.SimpleNamespace(**values)


class Filesystem:
    """Offer only fake descriptor operations, recording identity and read budget."""
    O_RDONLY = 0
    O_NONBLOCK = 2048

    def __init__(self, data=RAW, before=None, after=None, chunk=None, fail=None):
        self.data, self.before = data, before or metadata(st_size=len(data))
        self.after, self.chunk, self.fail = after or self.before, chunk, fail
        self.opened, self.closed, self.reads, self.fstats, self.offset = [], [], [], [], 0

    def open(self, path, flags):
        self.opened.append((str(path), flags))
        if self.fail == 'open': raise OSError('synthetic open failure')
        assert flags & self.O_NONBLOCK, 'Nonblocking open must precede kind admission'
        return 71

    def fstat(self, fd):
        assert fd == 71
        self.fstats.append(fd)
        if self.fail == ('first-fstat' if len(self.fstats) == 1 else 'last-fstat'):
            raise OSError('synthetic fstat failure')
        return self.before if len(self.fstats) == 1 else self.after

    def read(self, fd, count):
        assert fd == 71 and 0 < count <= 1024*1024
        self.reads.append(count)
        if self.fail == 'read': raise OSError('synthetic read failure')
        length = min(count, self.chunk) if self.chunk else count
        piece = self.data[self.offset:self.offset+length]
        self.offset += len(piece)
        return piece

    def close(self, fd):
        assert fd == 71
        self.closed.append(fd)
        if self.fail == 'close': raise OSError('synthetic close failure')


def main():
    """Run actual source with fake descriptors and optionally save a new report."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    if args.report and args.report.exists(): parser.error('report already exists')
    source_bytes = SOURCE.read_bytes()
    nodes = [node for node in ast.parse(source_bytes).body
             if isinstance(node, ast.FunctionDef) and node.name == 'read_json_binding']
    assert len(nodes) == 1
    executable = compile(ast.Module(body=nodes, type_ignores=[]), str(SOURCE)+'<pure-report-model>', 'exec')
    rows = []

    def case(name, model=None, error=None, limit=None, no_read=False, no_open=False):
        model = model or Filesystem()
        scope = {'Path': Path, 'os': model, 'stat': stat, 'hashlib': hashlib, 'json': json}
        exec(executable, scope)
        output, caught = None, None
        try:
            output = (scope['read_json_binding']('/synthetic/report.json') if limit is None else
                      scope['read_json_binding']('/synthetic/report.json', limit))
        except (OSError, ValueError, RuntimeError) as exception:
            caught = exception
        if error:
            assert isinstance(caught, error), (name, repr(caught))
        else:
            assert caught is None, (name, repr(caught))
            parsed, bound = output
            assert parsed == json.loads(model.data), name
            assert bound == {'path':'/synthetic/report.json', 'device':model.before.st_dev,
                'inode':model.before.st_ino, 'bytes':len(model.data),
                'mtime_ns':model.before.st_mtime_ns, 'ctime_ns':model.before.st_ctime_ns,
                'sha256':hashlib.sha256(model.data).hexdigest()}, name
        if no_read: assert not model.reads, name
        if no_open: assert not model.opened and not model.closed, name
        else:
            assert model.opened == [('/synthetic/report.json', model.O_RDONLY | model.O_NONBLOCK)], name
            assert model.closed == ([] if model.fail == 'open' else [71]), name
        rows.append({'name':name, 'status':'PASS', 'read_calls':len(model.reads),
                     'close_calls':len(model.closed), 'exception':type(caught).__name__ if caught else None})
        return output

    case('parsed JSON and hash derive from the same exact bytes')
    case('partial descriptor reads preserve the exact JSON/hash', Filesystem(chunk=1))
    case('literal Unicode JSON hashes its original UTF-8 bytes', Filesystem(data='{"text":"🌍"}'.encode()))
    first = case('first frozen inode gets its own binding')
    second = case('later same-byte replacement gets a distinct binding', Filesystem(before=metadata(st_ino=1235)))
    assert first[0] == second[0] and first[1]['sha256'] == second[1]['sha256'] and first[1] != second[1]
    for label, kind in [('FIFO',stat.S_IFIFO), ('device',stat.S_IFCHR),
                        ('directory',stat.S_IFDIR), ('socket',stat.S_IFSOCK)]:
        case(label+' refused before read', Filesystem(before=metadata(st_mode=kind|0o600)), RuntimeError, no_read=True)
    case('default 16 MiB declared overflow refused before read', Filesystem(before=metadata(st_size=16*1024*1024+1)), RuntimeError, no_read=True)
    case('negative declared size refused before read', Filesystem(before=metadata(st_size=-1)), RuntimeError, no_read=True)
    case('caller smaller budget remains bounded', error=RuntimeError, limit=len(RAW)-1, no_read=True)
    case('caller exact byte budget admits its file', limit=len(RAW))
    case('exact 16 MiB boundary still requires actual bytes', Filesystem(before=metadata(st_size=16*1024*1024)), RuntimeError)
    for value in (None,0,-1):
        model = Filesystem(); model.O_NONBLOCK = value
        case('unsupported nonblocking flag '+str(value)+' refused before open', model, RuntimeError, no_read=True, no_open=True)
    case('truncated descriptor contents refused', Filesystem(data=RAW[:-1], before=metadata()), RuntimeError)
    case('growth beyond declared length refused', Filesystem(data=RAW+b' ', before=metadata()), RuntimeError)
    for key in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns'):
        first_stat = metadata()
        case(key+' mutation refused', Filesystem(after=metadata(**{key:getattr(first_stat,key)+1})), RuntimeError)
    case('post-read file-kind mutation refused', Filesystem(after=metadata(st_mode=stat.S_IFIFO)), RuntimeError)
    case('invalid JSON still closes the descriptor', Filesystem(data=b'{broken'), json.JSONDecodeError)
    case('malformed UTF-8 still closes the descriptor', Filesystem(data=b'{"x":"\xff"}'), UnicodeDecodeError)
    case('empty regular input still needs valid JSON', Filesystem(data=b''), json.JSONDecodeError)
    for fail in ('open','first-fstat','read','last-fstat','close'):
        case(fail+' failure preserves owned cleanup', Filesystem(fail=fail), OSError,
             no_read=fail in ('open','first-fstat'))
    assert SOURCE.read_bytes() == source_bytes
    report = {'status':'SOURCE_REPORT_BINDING_MODEL_PASS', 'checks':len(rows), 'rows':rows,
        'source':str(SOURCE), 'source_sha256':hashlib.sha256(source_bytes).hexdigest(),
        'scope':'Actual extracted JSON reader with wholly fake descriptor operations',
        'actual_artifact_or_pipe_IO':'NOT_PERFORMED', 'actual_compiler_native_QEMU_Wasm_cgroup_ROS_execution':'NOT_PERFORMED'}
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        with args.report.open('x') as output: output.write(json.dumps(report,indent=2)+'\n')
    print(json.dumps({key:value for key,value in report.items() if key!='rows'}))
    return 0


if __name__ == '__main__':
    sys.exit(main())
