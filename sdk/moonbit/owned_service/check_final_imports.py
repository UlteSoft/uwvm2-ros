#!/usr/bin/env python3
"""Conservative final MoonBit owned-service import/static-memory subcheck.

SOURCE IMPLEMENTATION ONLY: not parsed, imported or executed by its author.
Future execution belongs to the externally admitted Linux keeper, Python
interpreter/startup/stdlib and existing bounded execution envelope. Checking
this file's current bytes cannot authenticate the bytes used at process start.

Requires exactly old ROS2 business43 plus raw12. All other imports, including
exception tags, need an external SHA-pinned exact full-signature table. There
is no module-wide allowance. The maintained inspector is executed from the
same captured, hard-pinned bytes, never an ambient module or pyc.

This only checks import signatures, actual memory declarations/exports and
conservatively understood active DATA segments. It is NOT a whole Wasm
validator, code/allocator verifier, sandbox proof or runtime/native test.
In particular, the heap lower bound is NEVER proved by Source constants, a
custom section, an exported global or a TLSF first-instruction pattern here.
Absent separately reviewed actual compiler/link admission => HOLD, exit3.
An externally pinned receipt is retained as an external attestation; its JSON
does not mint keeper authority and never changes full_memory_safety_pass=False.

Receipt format (all keys exact, no duplicate JSON keys/nonfinite numbers):
  format: 'independent-moonbit-compiler-link-memory-admission-v1'
  target: 'wasm' or 'wasm-gc'; wasm_sha256, checker_sha256,
  inspector_sha256, external_signatures_sha256: full lowercase SHA256
  memory: {index:0, minimum_pages:256, maximum_pages:1024,
           heap_start_lower_bound:<actual independently established >=8519680>}
  source_inputs: [{path:<canonical absolute>, bytes:<finite>, sha256:<full>}]
    Includes every pinned adjacent Source below AND the actual checker, plus
    at least one actual selected application moon.pkg and a .mbt body. Those
    extra sources are evidence bindings, NOT inferred allocator semantics.
  toolchain: [{role:'compiler'|'linker', path, bytes, sha256}]
    Exactly the two actual compiler/linker roles; one binary may serve both.
  evidence: [{role:'compiler-link-layout'|'allocator-codegen', path, bytes,
              sha256}]
    At least one of EACH role, referring to actual independent evidence.
The operator supplies the receipt's independently reviewed SHA on the command
line. This checker verifies identity/binding and finite ranges, not the truth
or independence of evidence. Self-created assertion JSON is not qualification.
"""
import argparse
import hashlib
import importlib.machinery
import json
import os
from pathlib import Path
import re
import secrets
import stat
import sys
import types


MIB = 1024 * 1024
PAGE_BYTES = 65536
EXPECTED_PAGES = (256, 1024)
RESERVED = ((0, 289), (65536, 8454160))
HEAP_MINIMUM = 8519680
RECEIPT_FORMAT = 'independent-moonbit-compiler-link-memory-admission-v1'
NUMERIC = {'i32', 'i64', 'f32', 'f64', 'v128'}
SOURCE_BINDINGS = {
    'sdk/tools/check_wasm_imports.py': '327fccbd6bdcfae0ae8da593014f4fb4451747b1ee1c9991996242b798c9bfa4',
    'sdk/abi/abi-v1.json': '0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0',
    'sdk/abi/rmw-service-v1.json': '9e743ad0077cfab6d96bd9ca37d4d23a8804d9c2593af70678b6406222b9ac18',
    'sdk/include/roswasm/rmw_service_v1.h': '6cbda155ccc897772f64094dd1b7b53577bf72432df50e469999c9227d3637ec',
    'src/uwvm2/uwvm/imported/ros/host.h': 'af98d37878e4e34e7e2e0ea8a5b099faccbd05bd51cfe066c4083dcf0c14959b',
    'src/uwvm2/uwvm/imported/ros/rmw_service_host.h': 'e795fb57537f416e107f4bfe58f2867944cbb34a7e0a01fefa2e9cf04f9dc6d5',
    'sdk/moonbit/linear/linear.mbt': 'b2e1889387e03eb494d8ff606dbec950cea7664b4be4fbc1f3a683e31be81fa9',
    'sdk/moonbit/linear/moon.pkg': '86dcc35cdbb4a80692dfa1126c2bf90e3747d97c7d2c52cb851337b1309bd7c3',
    'sdk/moonbit/moon.mod': 'ab318357192f4833565830137e9f10e971c6700cb0bd4b97cf1e98861e7a2182',
    'sdk/moonbit/producer-contract.json': '4e922fdf74b6c19581bf1afaa640732a6c005e56f4efa7d06fd042009e93b526',
}
RAW_MODULE = 'uwvm_ros2_rmw_srv_v1'
RAW_SIGNATURES = (
    ('abi_version', []),
    ('client_activate', ['i64', 'i64', 'i32', 'i64']),
    ('service_activate', ['i64', 'i64', 'i32', 'i64']),
    ('endpoint_inspect', ['i64', 'i64', 'i32', 'i64']),
    ('client_submit', ['i64', 'i64', 'i32', 'i64', 'i64', 'i64']),
    ('client_abandon', ['i64', 'i64', 'i64']),
    ('service_respond', ['i64', 'i64', 'i64', 'i64', 'i32', 'i64', 'i64']),
    ('endpoint_peek', ['i64', 'i64', 'i32', 'i32', 'i64', 'i64']),
    ('endpoint_commit', ['i64', 'i64', 'i64', 'i64']),
    ('endpoint_cancel', ['i64', 'i64', 'i64', 'i64']),
    ('client_available', ['i64', 'i64', 'i64', 'i64', 'i32', 'i64']),
    ('endpoint_retire', ['i64', 'i64']),
)
# Tag section precedes globals; DATA_COUNT precedes code despite numeric ID12.
SECTION_ORDER = {1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 13: 6, 6: 7,
                 7: 8, 8: 9, 9: 10, 12: 11, 10: 12, 11: 13}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def full_sha(value):
    return type(value) is str and re.fullmatch('[0-9a-f]{64}', value) is not None


def integer(value, lower, upper):
    return type(value) is int and lower <= value <= upper


def exact_keys(value, keys, label):
    require(type(value) is dict and set(value) == set(keys), 'exact keys required: ' + label)


def strict_json(data):
    def pairs(rows):
        result = {}
        for key, value in rows:
            require(key not in result, 'duplicate JSON key')
            result[key] = value
        return result

    def constant(value):
        raise ValueError('nonfinite JSON number: ' + value)

    return json.loads(data, object_pairs_hook=pairs, parse_constant=constant)


class Captures:
    """Stable same-FD DATA identities; no captured candidate is run here implicitly."""
    def __init__(self):
        self.rows = {}

    def read(self, path, maximum, expected=None, keep=True):
        path = Path(path)
        require(path.is_absolute() and path.resolve(strict=True) == path,
                'canonical absolute input required: ' + str(path))
        require(expected is None or full_sha(expected), 'full expected SHA256 required')
        selected = path.stat(follow_symlinks=False)
        flags = os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK
        fd = os.open(path, flags)
        try:
            first = os.fstat(fd)
            require(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum,
                    'bounded regular input required')
            fields = ('st_dev', 'st_ino', 'st_mode', 'st_nlink', 'st_uid', 'st_gid',
                      'st_size', 'st_mtime_ns', 'st_ctime_ns')
            key = lambda s: tuple(getattr(s, name) for name in fields)
            require(key(selected) == key(first), 'input changed at descriptor admission')
            parts, size, hashing = [], 0, hashlib.sha256()
            while True:
                part = os.read(fd, min(65536, maximum + 1 - size))
                if not part:
                    break
                size += len(part)
                require(size <= maximum and size <= first.st_size, 'input grew during capture')
                hashing.update(part)
                if keep:
                    parts.append(part)
            last = os.fstat(fd)
            final = path.stat(follow_symlinks=False)
            require(size == first.st_size and key(first) == key(last) == key(final),
                    'input changed during capture')
            sha = hashing.hexdigest()
            require(expected is None or expected == sha, 'input SHA256 differs: ' + str(path))
            row = {'path': str(path), 'bytes': size, 'sha256': sha,
                   'stat': {name.removeprefix('st_'): str(getattr(first, name)) for name in fields}}
            previous = self.rows.get(str(path))
            require(previous is None or previous == row, 'one input changed between captures')
            self.rows[str(path)] = row
            return b''.join(parts) if keep else None
        finally:
            os.close(fd)

    def recheck(self):
        for row in tuple(self.rows.values()):
            self.read(Path(row['path']), row['bytes'], row['sha256'], keep=False)


def expected_ros(captured, root):
    original = strict_json(captured[str(root / 'sdk/abi/abi-v1.json')])
    raw = strict_json(captured[str(root / 'sdk/abi/rmw-service-v1.json')])
    require(raw['kind'] == 'optional-owned-rmw-service-v1' and
            raw['schema_version'] == raw['extension_version'] == 1,
            'raw12 catalog version/kind')
    for field, relative in (('authority_source', 'src/uwvm2/uwvm/imported/ros/host.h'),
                            ('implementation_source', 'src/uwvm2/uwvm/imported/ros/rmw_service_host.h'),
                            ('raw_header', 'sdk/include/roswasm/rmw_service_v1.h')):
        require(raw[field] == {'path': relative, 'sha256': SOURCE_BINDINGS[relative]},
                'actual raw12 Source binding differs')
    rows = original['modules']['uwvm_ros2_v1']['functions']
    require(len(rows) == 43, 'complete original business43 catalog required')
    expected = {}
    for row in rows:
        key = ('uwvm_ros2_v1', row['name'])
        params, results = [p['type'] for p in row['params']], row['results']
        require(key not in expected and all(x in NUMERIC for x in params + results),
                'duplicate/nonnumeric original business signature')
        expected[key] = {'module': key[0], 'name': key[1], 'kind': 0,
                         'params': params, 'results': results}
    module = raw['modules'][RAW_MODULE]
    require(set(raw['modules']) == {RAW_MODULE} and module['backend'] == 2 and
            module['function_count'] == 12 and len(module['functions']) == 12,
            'closed backend2 raw12 catalog required')
    wanted = [{'name': name, 'params': params, 'results': ['i32']}
              for name, params in RAW_SIGNATURES]
    require(module['functions'] == wanted, 'full raw12 signature catalog differs')
    for row in wanted:
        expected[RAW_MODULE, row['name']] = {'module': RAW_MODULE, 'name': row['name'],
                                           'kind': 0, 'params': row['params'], 'results': row['results']}
    require(len(expected) == 55, 'exact mandatory business43 plus raw12 required')
    return expected


def external_signatures(data):
    rows = strict_json(data)
    require(type(rows) is list and len(rows) <= 256, 'finite external full-signature table required')
    result = {}
    for row in rows:
        exact_keys(row, ('module', 'name', 'kind', 'params', 'results'), 'external import')
        require(all(type(row[k]) is str and 0 < len(row[k].encode('utf-8')) <= 256
                    for k in ('module', 'name')), 'bounded external names required')
        require(not row['module'].startswith('uwvm_ros') and
                type(row['kind']) is int and row['kind'] in (0, 4),
                'external exact function/tag only; ROS namespace is closed')
        for field in ('params', 'results'):
            require(type(row[field]) is list and len(row[field]) <= 64 and
                    all(type(x) is str and 0 < len(x) <= 80 for x in row[field]),
                    'bounded exact external value-type spellings required')
        require(row['kind'] != 4 or row['results'] == [], 'tag results must be empty')
        key = (row['module'], row['name'])
        require(key not in result, 'duplicate external import row')
        result[key] = row
    return result


def inspect_layout(data, reader):
    """Own conservative DATA walk; Reader is the exact captured maintained reader.

    No code-body decoding/allocator proof. Unknown sections or DATA expressions
    refuse, while unparsed known code/table/global/element payloads still need
    the independent whole Wasm validator. Passive DATA is intentionally refused.
    """
    source = reader(data)
    require(bytes(source.take(8)) == b'\0asm\1\0\0\0', 'core Wasm v1 binary required')
    seen, last_order, sections = set(), 0, 0
    signatures, imports, functions, memories, exports, segments, starts = [], [], [], [], {}, [], []
    imported_functions, total_data, declared_data_count = 0, 0, None
    while source.position < len(source.data):
        sections += 1
        require(sections <= 4096, 'section count work bound')
        section = source.byte()
        body = reader(source.take(source.uint()))
        if section == 0:
            # Custom sections confer no layout/heap authority, including names.
            continue
        require(section in SECTION_ORDER and section not in seen and
                SECTION_ORDER[section] > last_order, 'unknown/duplicate/out-of-order standard section')
        seen.add(section)
        last_order = SECTION_ORDER[section]
        if section == 1:
            signatures = body.types()
            body.end()
        elif section == 2:
            for _ in range(body.count()):
                module, name, kind = body.name(), body.name(), body.byte()
                require(kind in (0, 4), 'imported memory/table/global refused')
                if kind == 4:
                    require(body.byte() == 0, 'tag attribute')
                index = body.uint()
                require(index < len(signatures) and signatures[index] is not None,
                        'import type must be an actual available function type')
                params, results = signatures[index]
                require(kind != 4 or not results, 'tag cannot have results')
                imports.append({'module': module, 'name': name, 'kind': kind,
                                'params': params, 'results': results})
                if kind == 0:
                    functions.append(index)
                    imported_functions += 1
            body.end()
        elif section == 3:
            for _ in range(body.count()):
                index = body.uint()
                require(index < len(signatures) and signatures[index] is not None,
                        'defined function type index')
                functions.append(index)
            body.end()
        elif section == 5:
            count = body.count()
            require(count == 1, 'exactly one defined memory0 required')
            # Only has-max flag1: rejects shared2, memory64=4 and custom pages.
            require(body.uint() == 1, 'plain bounded wasm32 nonshared memory required')
            minimum, maximum = body.uint(), body.uint()
            require((minimum, maximum) == EXPECTED_PAGES, 'actual memory must be256/1024 pages')
            memories.append({'index': 0, 'minimum_pages': minimum, 'maximum_pages': maximum,
                             'shared': False, 'memory64': False, 'imported': False})
            body.end()
        elif section == 7:
            for _ in range(body.count()):
                name, kind, index = body.name(), body.byte(), body.uint()
                require(name not in exports and kind in (0, 1, 2, 3, 4), 'duplicate/unknown export')
                if kind == 2:
                    require(name == 'memory' and index == 0, 'only actual memory0 export named memory allowed')
                if kind == 0:
                    require(index < len(functions), 'function export index outside actual function inventory')
                exports[name] = {'kind': kind, 'index': index}
            body.end()
        elif section == 8:
            index = body.uint()
            require(index < len(functions) and signatures[functions[index]] == ([], []),
                    'start function must have actual empty signature')
            starts.append(index)
            body.end()
        elif section == 12:
            declared_data_count = body.uint()
            require(declared_data_count <= 100000, 'finite DATA_COUNT required')
            body.end()
        elif section == 11:
            count = body.count()
            for ordinal in range(count):
                mode = body.uint()
                require(mode in (0, 2), 'passive/unknown DATA mode requires separate reviewed variant')
                memory = 0 if mode == 0 else body.uint()
                require(memory == 0 and body.byte() == 0x41, 'DATA offset must be known i32.const memory0')
                offset = body.sint(32) & 0xffffffff
                require(body.byte() == 0x0b, 'extended/uncertain DATA constant expression refused')
                size = body.uint()
                require(size <= 64 * MIB - total_data, 'cumulative DATA work/byte bound')
                total_data += size
                body.take(size)
                require(offset <= EXPECTED_PAGES[0] * PAGE_BYTES and
                        size <= EXPECTED_PAGES[0] * PAGE_BYTES - offset,
                        'active DATA outside initial memory')
                end = offset + size
                # Empty segments write no bytes, even when offset is reserved.
                require(size == 0 or all(end <= first or offset >= last for first, last in RESERVED),
                        'active DATA overlaps stdout scratch or reserved ROS arena')
                segments.append({'ordinal': ordinal, 'mode': mode, 'memory': memory,
                                 'offset': offset, 'bytes': size, 'end_exclusive': end})
            body.end()
        elif section == 13:
            for _ in range(body.count()):
                require(body.byte() == 0, 'defined tag attribute')
                index = body.uint()
                require(index < len(signatures) and signatures[index] is not None and
                        signatures[index][1] == [], 'defined tag actual function type')
            body.end()
        # Types/imports/functions/memory/exports/start/DATA/tags are parsed here.
        # Known table/global/element/code content is opaque to THIS subcheck.
    require(len(memories) == 1 and exports.get('memory') == {'kind': 2, 'index': 0},
            'actual single defined memory0 and its memory export required')
    require(declared_data_count is None or declared_data_count == len(segments),
            'DATA_COUNT differs from actual accepted DATA inventory')
    require(len(imports) <= 100000 and len(functions) <= 100000, 'import/function total bound')
    return {'imports': imports, 'memories': memories, 'exports': exports,
            'start': starts, 'active_data': segments,
            'active_data_high_water': max((row['end_exclusive'] for row in segments if row['bytes']), default=0),
            'reserved_half_open_ranges': [list(row) for row in RESERVED],
            'whole_wasm_validator_required': True, 'heap_start_proved_by_binary': False,
            'full_memory_safety_pass': False}


def resource_row(row, captures, maximum, label):
    exact_keys(row, ('path', 'bytes', 'sha256'), label)
    require(type(row['path']) is str and 0 < len(row['path']) <= 4096 and
            integer(row['bytes'], 0, maximum) and full_sha(row['sha256']),
            'finite path/bytes/full SHA required: ' + label)
    captures.read(Path(row['path']), maximum, row['sha256'], keep=False)
    require(captures.rows[row['path']]['bytes'] == row['bytes'], 'admission exact byte count differs')
    return row


def compiler_link_admission(data, captures, target, wasm_sha, entry_sha, external_sha,
                            required_sources, active_end):
    receipt = strict_json(data)
    exact_keys(receipt, ('format', 'target', 'wasm_sha256', 'checker_sha256', 'inspector_sha256',
                         'external_signatures_sha256', 'memory', 'source_inputs', 'toolchain', 'evidence'),
               'independent compiler/link receipt')
    require(receipt['format'] == RECEIPT_FORMAT and receipt['target'] == target and
            receipt['wasm_sha256'] == wasm_sha and receipt['checker_sha256'] == entry_sha and
            receipt['inspector_sha256'] == SOURCE_BINDINGS['sdk/tools/check_wasm_imports.py'] and
            receipt['external_signatures_sha256'] == external_sha,
            'receipt must bind actual artifact, target, entry, inspector and external signature bytes')
    memory = receipt['memory']
    exact_keys(memory, ('index', 'minimum_pages', 'maximum_pages', 'heap_start_lower_bound'), 'receipt memory')
    require(type(memory['index']) is int and memory['index'] == 0 and
            type(memory['minimum_pages']) is int and memory['minimum_pages'] == 256 and
            type(memory['maximum_pages']) is int and memory['maximum_pages'] == 1024 and
            integer(memory['heap_start_lower_bound'], max(HEAP_MINIMUM, active_end), 256 * PAGE_BYTES),
            'independent actual compiler-owned heap bound must follow reservation and DATA')
    sources = receipt['source_inputs']
    require(type(sources) is list and 1 <= len(sources) <= 256, 'finite actual source-input list required')
    actual_sources, total, app_manifest, app_body = {}, 0, False, False
    for row in sources:
        selected = resource_row(row, captures, 16 * MIB, 'source input')
        require(selected['path'] not in actual_sources, 'duplicate source-input path')
        total += selected['bytes']
        require(total <= 32 * MIB, 'cumulative source-input bytes bound')
        actual_sources[selected['path']] = selected['sha256']
        # At least one producer-selected main package and body must be bound;
        # no parser or Source constant is used to attest that they set the heap.
        if selected['path'] not in required_sources:
            app_manifest |= Path(selected['path']).name == 'moon.pkg'
            app_body |= Path(selected['path']).suffix == '.mbt'
    require(all(actual_sources.get(path) == sha for path, sha in required_sources.items()) and
            app_manifest and app_body, 'receipt omits actual selected application or mandatory Source identities')
    tools = receipt['toolchain']
    require(type(tools) is list and len(tools) == 2, 'actual compiler and linker identities required')
    roles = set()
    for row in tools:
        exact_keys(row, ('role', 'path', 'bytes', 'sha256'), 'toolchain input')
        require(row['role'] in ('compiler', 'linker') and row['role'] not in roles,
                'one exact compiler and linker role required')
        roles.add(row['role'])
        resource_row({key: row[key] for key in ('path', 'bytes', 'sha256')}, captures,
                     512 * MIB, 'actual compiler/linker')
        require(row['bytes'] > 0, 'empty toolchain cannot attest actual build')
    evidence = receipt['evidence']
    require(type(evidence) is list and 2 <= len(evidence) <= 32, 'bounded independent layout/codegen evidence required')
    evidence_paths, evidence_roles, total = set(), set(), 0
    for row in evidence:
        exact_keys(row, ('role', 'path', 'bytes', 'sha256'), 'layout evidence')
        require(row['role'] in ('compiler-link-layout', 'allocator-codegen') and
                row['path'] not in evidence_paths, 'independent distinct evidence roles/paths required')
        evidence_paths.add(row['path'])
        evidence_roles.add(row['role'])
        resource_row({key: row[key] for key in ('path', 'bytes', 'sha256')}, captures,
                     32 * MIB, 'independent actual evidence')
        total += row['bytes']
        require(row['bytes'] > 0 and total <= 64 * MIB, 'finite nonempty cumulative evidence bound')
    require(evidence_roles == {'compiler-link-layout', 'allocator-codegen'},
            'both actual compiler/link layout and allocator codegen evidence required')
    return {'receipt_format': RECEIPT_FORMAT, 'external_receipt_sha256': digest(data),
            'heap_start_lower_bound_externally_attested': memory['heap_start_lower_bound'],
            'heap_start_proved_by_this_checker': False,
            'keeper_authority_or_evidence_truth_verified': False,
            'independent_operator_review_required': True,
            'source_inputs': sources, 'toolchain': tools, 'evidence': evidence}


def check(root, wasm, external_path, external_sha, entry_path, entry_sha, target,
          admission_path=None, admission_sha=None):
    captures = Captures()
    captured = {}
    for relative, expected in SOURCE_BINDINGS.items():
        path = root / relative
        captured[str(path)] = captures.read(path, 2 * MIB, expected)
    entry = captures.read(entry_path, 2 * MIB, entry_sha)
    require(digest(entry) == entry_sha, 'explicit checker Source SHA required')
    external_raw = captures.read(external_path, MIB, external_sha)
    allowed = external_signatures(external_raw)
    expected = expected_ros(captured, root)
    data = captures.read(wasm, 128 * MIB)
    require((admission_path is None) == (admission_sha is None), 'receipt path and reviewed SHA must be supplied together')
    admission_raw = None if admission_path is None else captures.read(admission_path, 2 * MIB, admission_sha)
    helper_path = root / 'sdk/tools/check_wasm_imports.py'
    helper_bytes = captured[str(helper_path)]
    # Fresh object, no ordinary alias import/pyc/sys.modules adoption. This
    # inspector has only pinned stdlib imports; startup/stdlib is keeper trust.
    name = '_uwvm_moonbit_captured_inspector_' + secrets.token_hex(16)
    require(name not in sys.modules, 'private inspector namespace collision')
    module = types.ModuleType(name)
    module.__file__ = str(helper_path)
    module.__package__ = ''
    module.__cached__ = None
    module.__spec__ = importlib.machinery.ModuleSpec(name, loader=None)
    exec(compile(helper_bytes, str(helper_path), 'exec'), module.__dict__)
    maintained_rows = module.inspect(data)
    layout = inspect_layout(data, module.Reader)
    rows = layout['imports']
    require(len(rows) == len(maintained_rows), 'maintained/own actual import counts differ')
    for actual, maintained in zip(rows, maintained_rows):
        require(all(actual[key] == value for key, value in maintained.items()),
                'maintained/own import parsing disagrees')
    found_ros, found_external, seen = set(), set(), set()
    for row in rows:
        key = (row['module'], row['name'])
        require(key not in seen, 'duplicate backend-qualified import')
        seen.add(key)
        if row['module'].startswith('uwvm_ros'):
            require(key in expected and row == expected[key], 'unknown/wrong-backend ROS namespace or full numeric signature')
            found_ros.add(key)
        else:
            require(key in allowed and row == allowed[key], 'external import lacks exact explicitly admitted full signature')
            found_external.add(key)
    require(found_ros == set(expected) and len(found_ros) == 55, 'mandatory actual old43 plus raw12 missing')
    require(found_external == set(allowed), 'external table is exact, not unused module/name allowances')
    result = {'status': 'HOLD_EXTERNAL_COMPILER_LINK_ADMISSION',
              'scope': 'Import signatures and conservative static DATA/memory declarations only',
              'wasm_sha256': digest(data), 'target': target, 'ROS_functions_checked': 55,
              'imports': rows, 'memory_subcheck': layout,
              'heap_start_proved_by_binary': False, 'full_memory_safety_pass': False,
              'whole_wasm_validator_required': True, 'runtime_or_native_credit': False,
              'startup_entry_interpreter_stdlib_keeper_trust': 'EXTERNAL_NOT_AUTHENTICATED_BY_SELF_READ',
              'external_signature_sha256': digest(external_raw),
              'compiler_link_admission': None}
    if admission_raw is not None:
        sources = {str(root / relative): sha for relative, sha in SOURCE_BINDINGS.items()}
        sources[str(entry_path)] = entry_sha
        result['compiler_link_admission'] = compiler_link_admission(
            admission_raw, captures, target, digest(data), entry_sha, digest(external_raw),
            sources, layout['active_data_high_water'])
        result['status'] = 'SIGNATURE_STATIC_LAYOUT_SUBCHECK_WITH_EXTERNAL_ATTESTATION'
    captures.recheck()
    result['input_identities'] = list(captures.rows.values())
    result['exit_code'] = 3 if admission_raw is None else 0
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--wasm', type=Path, required=True)
    parser.add_argument('--target', choices=('wasm', 'wasm-gc'), required=True)
    parser.add_argument('--entry-sha256', required=True)
    parser.add_argument('--external-signatures', type=Path, required=True)
    parser.add_argument('--external-signatures-sha256', required=True)
    parser.add_argument('--compiler-link-admission', type=Path)
    parser.add_argument('--compiler-link-admission-sha256')
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    try:
        require(sys.platform == 'linux' and sys.flags.isolated and sys.flags.no_site and sys.flags.safe_path,
                'externally admitted Linux Python -I -S execution required')
        root = args.source_root.resolve(strict=True)
        require(root.is_dir() and full_sha(args.entry_sha256) and
                full_sha(args.external_signatures_sha256) and
                (args.compiler_link_admission_sha256 is None or full_sha(args.compiler_link_admission_sha256)),
                'source root and full independently selected input SHAs required')
        result = check(root, args.wasm, args.external_signatures, args.external_signatures_sha256,
                       Path(__file__), args.entry_sha256, args.target,
                       args.compiler_link_admission, args.compiler_link_admission_sha256)
        # Fresh report only. This is a future subcheck receipt, not canonical
        # SDK/security state and not an execution or Native acceptance claim.
        with args.report.open('x', encoding='utf-8') as stream:
            stream.write(json.dumps(result, indent=2, allow_nan=False) + '\n')
        print(result['status'])
        return result['exit_code']
    except Exception as error:
        print(str(error), file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
