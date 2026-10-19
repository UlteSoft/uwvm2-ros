#!/usr/bin/env python3
"""Non-ROS CPython DBG artifact subcheck; not a whole validator or usable DBG.

Only the maintained inspector's exact captured Source is executed, against the
same already captured Wasm bytes. This requires the future keeper's admitted
Linux interpreter/stdlib boundary. No ambient candidate import or PYC. A bound
external validator receipt does not prove its own authority/independence.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import stat
import sys
import types

MIB = 1024 * 1024
INSPECTOR_SHA = 'a457050eb7a94a39ffc9ed1a451a22e3a2808c4cb8dcf5fe6ba03862a7ab1dca'
TYPES = {0x7f: 'i32', 0x7e: 'i64', 0x7d: 'f32', 0x7c: 'f64'}
ORDER = {1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 13: 6, 6: 7, 7: 8, 8: 9, 9: 10, 12: 11, 10: 12, 11: 13}


def need(value, text):
    if not value: raise ValueError(text)


def sha(raw): return hashlib.sha256(raw).hexdigest()
def full_sha(value): return type(value) is str and re.fullmatch('[0-9a-f]{64}', value) is not None


def strict(raw):
    def pairs(rows):
        out = {}
        for key, value in rows:
            need(key not in out, 'duplicate JSON key'); out[key] = value
        return out
    def bad(value): raise ValueError('nonfinite JSON: ' + value)
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=bad)


class Capture:
    def __init__(self): self.rows, self.identities = {}, {}
    def read(self, value, maximum, expected):
        need(full_sha(expected), 'full expected SHA required')
        path = Path(value)
        need(path.is_absolute() and path.resolve(strict=True) == path, 'canonical input path; no symlink aliases')
        fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
        try:
            first = os.fstat(fd); keys = ('st_dev', 'st_ino', 'st_mode', 'st_nlink', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
            need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum, 'bounded regular file')
            identity = first.st_dev, first.st_ino
            need(identity not in self.identities or self.identities[identity] == str(path), 'hardlink input alias')
            parts = []; size = 0
            while True:
                part = os.read(fd, min(65536, maximum - size + 1))
                if not part: break
                size += len(part); need(size <= maximum and size <= first.st_size, 'input grew'); parts.append(part)
            last, named = os.fstat(fd), os.lstat(path)
            need(size == first.st_size and all(getattr(first, k) == getattr(last, k) == getattr(named, k) for k in keys), 'input changed')
            raw = b''.join(parts); need(sha(raw) == expected, 'exact SHA differs')
            row = {'path': str(path), 'bytes': size, 'sha256': expected, 'stat': {k: str(getattr(first, k)) for k in keys}}
            need(str(path) not in self.rows or self.rows[str(path)] == row, 'capture drift')
            self.rows[str(path)] = row; self.identities[identity] = str(path)
            return raw
        finally: os.close(fd)
    def recheck(self):
        for row in tuple(self.rows.values()): self.read(row['path'], row['bytes'], row['sha256'])


class Reader:
    def __init__(self, raw): self.raw, self.position = raw, 0
    def take(self, size):
        need(type(size) is int and 0 <= size <= len(self.raw) - self.position, 'truncated Wasm field')
        value = self.raw[self.position:self.position + size]; self.position += size; return value
    def byte(self): return self.take(1)[0]
    def uint(self):
        value = 0
        for i in range(5):
            b = self.byte(); need(i != 4 or b < 16, 'u32 LEB overflow')
            value |= (b & 127) << (i * 7)
            if b < 128: return value
        raise ValueError('unterminated u32 LEB')
    def count(self, maximum):
        value = self.uint(); need(value <= maximum, 'bounded vector count'); return value
    def name(self): return self.take(self.count(65536)).decode('utf-8', 'strict')
    def end(self): need(self.position == len(self.raw), 'trailing section fields')
    def vector_type(self):
        out = []
        for _ in range(self.count(64)):
            value = self.byte(); need(value in TYPES, 'profile requires actual numeric core types'); out.append(TYPES[value])
        return out


def structure(raw):
    reader = Reader(raw); need(reader.take(8) == b'\0asm\1\0\0\0', 'core module, not component')
    seen = set(); order = 0; signatures = []; imported = []; functions = []; exports = {}; memories = []; custom = {}; code = None
    while reader.position < len(raw):
        tag = reader.byte(); body = Reader(reader.take(reader.uint()))
        if tag == 0:
            name = body.name(); custom.setdefault(name, []).append(len(body.raw) - body.position); continue
        need(tag in ORDER and tag not in seen and ORDER[tag] > order, 'duplicate/unsupported/out-of-order core section')
        seen.add(tag); order = ORDER[tag]
        if tag == 1:
            for _ in range(body.count(1000000)):
                need(body.byte() == 0x60, 'this CPython core profile has no GC/recursive function types')
                signatures.append((body.vector_type(), body.vector_type()))
            body.end()
        elif tag == 2:
            for _ in range(body.count(1024)):
                module, name, kind = body.name(), body.name(), body.byte()
                need(kind == 0 and module == 'wasi_snapshot_preview1', 'all ROS/foreign/nonfunction imports refused')
                index = body.uint(); need(index < len(signatures), 'import type index')
                params, results = signatures[index]
                imported.append({'module': module, 'name': name, 'kind': kind, 'params': params, 'results': results})
            body.end()
        elif tag == 3:
            functions = [body.uint() for _ in range(body.count(1000000))]
            need(all(x < len(signatures) for x in functions), 'defined function type index'); body.end()
        elif tag == 5:
            for _ in range(body.count(1)):
                flags = body.uint(); need(flags == 0, 'one defined unshared memory32, no max, fixed FullOz profile')
                minimum = body.uint(); need(minimum == 640, 'actual original41943040-byte initial memory')
                memories.append({'flags': flags, 'min': minimum, 'max': None})
            body.end()
        elif tag == 7:
            for _ in range(body.count(65536)):
                name, kind, index = body.name(), body.byte(), body.uint()
                need(name not in exports, 'duplicate export name'); exports[name] = (kind, index)
            body.end()
        elif tag == 8: raise ValueError('implicit start forbidden; invoke actual WASI _start exactly once')
        elif tag == 10: code = body.count(1000000)
        # Remaining core/table/global/element/code/DATA instruction validity is
        # deliberately delegated to the mandatory independent whole validator.
    need(len(memories) == 1 and exports.get('memory') == (2, 0), 'exact actual memory0 export')
    need(code == len(functions) and code is not None and code > 0, 'actual code/function count')
    entry = exports.get('_start'); need(entry is not None and entry[0] == 0, 'actual function _start export')
    local = entry[1] - len(imported)
    need(0 <= local < len(functions) and signatures[functions[local]] == ([], []), 'defined []->[] WASI _start')
    for name in ('name', '.debug_info', '.debug_abbrev', '.debug_line'):
        need(name in custom and len(custom[name]) == 1 and custom[name][0] > 0, 'necessary nonempty unique name/DWARF section missing: ' + name)
    need(len({(x['module'], x['name']) for x in imported}) == len(imported), 'duplicate qualified import')
    return {'imports': imported, 'exports': exports, 'memory': memories[0], 'custom': custom, 'defined_function_count': len(functions), 'defined_start_local_index': local}


def captured_inspect(source, raw, filename):
    module = types.ModuleType('_uwvm_debug_inspect_' + secrets.token_hex(16))
    module.__file__ = filename; module.__package__ = ''; module.__cached__ = None; module.__spec__ = None
    exec(compile(source, filename, 'exec', dont_inherit=True), module.__dict__)
    sentinel = 'captured-final-python-debug-wasm'
    class CapturedPath:
        def __init__(self, value): need(value == sentinel, 'inspector attempted an uncaptured path')
        def read_bytes(self): return raw
    module.Path = CapturedPath
    return module.inspect(sentinel)  # Same captured bytes; no second disk read.


def checked(args):
    cap = Capture(); cap.read(Path(__file__).resolve(), 2 * MIB, args.checker_sha256)
    raw = cap.read(args.wasm, 256 * MIB, args.wasm_sha256)
    source = cap.read(args.inspector, 2 * MIB, INSPECTOR_SHA)
    plan_raw = cap.read(args.build_plan, 16 * MIB, args.build_plan_sha256); plan = strict(plan_raw)
    need(type(plan) is dict and plan.get('format') == 'python-debug-additive-build-plan-v1' and
         plan.get('status') == 'CAPTURED_ARGV_NOT_EXECUTED' and full_sha(plan.get('source_closure_sha256')) and
         full_sha(plan.get('build_admission_sha256')), 'real preparation plan identity, not a build PASS receipt')
    startup_raw = cap.read(args.startup_imports, MIB, args.startup_sha256); startup = strict(startup_raw)
    need(type(startup) is dict and set(startup) == {'format', 'imports'} and startup['format'] == 'python-debug-wasi-startup-v1' and
         type(startup['imports']) is list and 1 <= len(startup['imports']) <= 1024, 'external reviewed exact startup signatures')
    expected = {}
    for row in startup['imports']:
        need(type(row) is dict and set(row) == {'module', 'name', 'kind', 'params', 'results'} and
             row['module'] == 'wasi_snapshot_preview1' and type(row['kind']) is int and row['kind'] == 0 and
             type(row['name']) is str and 0 < len(row['name'].encode()) <= 256, 'non-ROS numeric WASI function row only')
        for k in ('params', 'results'):
            need(type(row[k]) is list and len(row[k]) <= 64 and all(type(t) is str and t in TYPES.values() for t in row[k]), 'full numeric signature')
        key = row['module'], row['name']; need(key not in expected, 'duplicate startup signature'); expected[key] = row
    actual = structure(raw)
    need({(x['module'], x['name']): x for x in actual['imports']} == expected, 'missing/extra/wrong actual startup signatures')
    observed = captured_inspect(source, raw, str(args.inspector))
    need([(x['module'], x['name'], x['kind']) for x in observed['imports']] ==
         [(x['module'], x['name'], x['kind']) for x in actual['imports']], 'maintained inspector differs')
    names = observed['functions']
    need(any(type(index) is int and len(actual['imports']) <= index < len(actual['imports']) + actual['defined_function_count'] and name == 'uwvm_python_debug_anchor'
             for index, name in names.items()), 'necessary real named anchor absent; name is not location proof')
    cap.recheck()
    result = {'format': 'python-debug-final-wasm-subcheck-v1', 'wasm_sha256': sha(raw), 'wasm_bytes': len(raw),
              'build_plan_sha256': args.build_plan_sha256, 'startup_sha256': args.startup_sha256,
              'inspector_sha256': INSPECTOR_SHA, 'checker_sha256': args.checker_sha256, 'structure': actual,
              'ROS_imports': 0, 'whole_wasm_validation': 'HOLD_PENDING_INDEPENDENT_EVIDENCE', 'usable_DBG': False,
              'typed_DWARF_root_activation_controller_GC_qualified': False, 'full_memory_safety_pass': False,
              'qualification': 'necessary artifact structure/signatures only; labels/sections/name are not source variable or stop authority',
              'trust_boundary': 'keeper pre-admits this entry/native Python/stdlib and independently reviews validator evidence; external hashes/JSON cannot prove their own authority'}
    if args.validator_receipt is None:
        result['status'] = 'HOLD'; result['input_pins'] = list(cap.rows.values()); return result, 3
    receipt_raw = cap.read(args.validator_receipt, 8 * MIB, args.validator_receipt_sha256)
    evidence = strict(receipt_raw)
    need(type(evidence) is dict and set(evidence) == {'format', 'wasm_sha256', 'wasm_bytes', 'build_plan_sha256', 'checker_sha256', 'inspector_sha256', 'validator', 'argv', 'exit_code', 'evidence'} and
         evidence['format'] == 'independent-python-debug-whole-wasm-validation-v1' and
         evidence['wasm_sha256'] == sha(raw) and type(evidence['wasm_bytes']) is int and evidence['wasm_bytes'] == len(raw) and
         evidence['build_plan_sha256'] == args.build_plan_sha256 and evidence['checker_sha256'] == args.checker_sha256 and
         evidence['inspector_sha256'] == INSPECTOR_SHA and type(evidence['exit_code']) is int and evidence['exit_code'] == 0,
         'independently admitted actual whole-validator receipt must bind exact final bytes and this cohort')
    need(type(evidence['evidence']) is list and 1 <= len(evidence['evidence']) <= 32, 'actual separate validator receipts/logs required')
    rows = [evidence['validator']] + evidence['evidence']; paths = set(); total = 0
    for row in rows:
        need(type(row) is dict and set(row) == {'path', 'bytes', 'sha256'} and row['path'] not in paths and
             type(row['bytes']) is int and 0 <= row['bytes'] <= 128 * MIB, 'unique bounded validator input/evidence')
        paths.add(row['path']); total += row['bytes']; need(total <= 256 * MIB, 'validator evidence total bound')
        body = cap.read(row['path'], row['bytes'], row['sha256']); need(len(body) == row['bytes'], 'validator evidence extent')
    argv = evidence['argv']
    need(type(argv) is list and 2 <= len(argv) <= 256 and all(type(x) is str and x and len(x) <= 65536 and '\0' not in x for x in argv) and
         argv[0] == evidence['validator']['path'] and argv.count(str(args.wasm)) == 1, 'exact independently reviewed whole-validator argv')
    cap.recheck()
    result.update(status='SUBCHECK_AND_EXTERNAL_EVIDENCE_BOUND_NOT_RUNTIME_OR_DBG_PASS',
                  whole_wasm_validation='EXTERNAL_RECEIPT_BYTES_BOUND_AUTHORITY_REMAINS_KEEPER',
                  validator_receipt_sha256=args.validator_receipt_sha256, input_pins=list(cap.rows.values()))
    return result, 0


def main():
    need(sys.platform == 'linux' and sys.flags.isolated and sys.flags.no_site and sys.flags.safe_path, 'pre-admitted Linux Python -I -S only')
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('wasm', 'inspector', 'build-plan', 'startup-imports'): parser.add_argument('--' + name, type=Path, required=True)
    for name in ('wasm', 'checker', 'build-plan', 'startup'): parser.add_argument('--' + name + '-sha256', required=True)
    parser.add_argument('--validator-receipt', type=Path); parser.add_argument('--validator-receipt-sha256')
    args = parser.parse_args()
    need((args.validator_receipt is None) == (args.validator_receipt_sha256 is None), 'validator receipt/SHA pair')
    result, code = checked(args); print(json.dumps(result, indent=2)); return code


if __name__ == '__main__':
    try: raise SystemExit(main())
    except (ValueError, KeyError, TypeError, IndexError, OSError) as failure:
        print(json.dumps({'status': 'HOLD', 'error_type': type(failure).__name__, 'error': str(failure), 'usable_DBG': False}), file=sys.stderr)
        raise SystemExit(3)
