#!/usr/bin/env python3
"""Emit four-language complete-value checks from original declaration vectors.

No production codec generator is imported. Original fields are parsed and filled
with an independently packed nonzero value. The byte SHA must agree with the
reference vector already exercised by native/Wasm C and C++. Public spelling
maps select each language's escaped field names; every original scalar/string
value and array extent is then checked after decode, before exact re-encoding.
Truncation and trailing data must also be rejected. Synthetic values establish
representation only, never physical command validity or ROS authorization.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import zlib

SCALARS = {'bool', 'byte', 'char', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32',
           'int64', 'uint64', 'float32', 'float64', 'string'}


def sections(body, version, kind):
    if version == 1 and kind in ('srv', 'action'):
        pieces = [[]]
        for line in body.split('\n'):
            if line.startswith('---'):
                if kind == 'action' or len(pieces) == 1:
                    pieces.append([])
            else:
                pieces[-1].append(line + '\n')
        if kind == 'srv' and len(pieces) == 1:
            pieces.append([])
        return [''.join(v) for v in pieces]
    return re.split(r'^\s*---\s*(?:#[^\n]*)?$', body, flags=re.MULTILINE)


def build(source, vendor, profile, vectors, out, *, on_case=None):
    source = Path(source).resolve(strict=True)
    kit = source / 'sdk/origin/robots/platforms' / vendor
    root = kit / 'catalogs' / profile
    configuration = next(v for v in json.loads((kit / 'profiles.json').read_bytes())['profiles'] if v['id'] == profile)
    version = configuration['ros_version']
    family = source / 'sdk/origin/custom-types' / configuration['family']
    generation = json.loads((family / 'generation.vendor.json').read_bytes())
    reference = json.loads(Path(vectors).read_bytes())
    reference_cases = {v['type']: v for v in reference['cases']}
    records, pins = {}, {}
    def resolve(name, package):
        if name in SCALARS or version == 1 and name in ('time', 'duration'):
            return name
        if name == 'Header':
            name = 'std_msgs/Header'
        pieces = name.split('/')
        if len(pieces) == 1:
            return package + ('/msg/' if version == 2 else '/') + name
        return pieces[0] + '/msg/' + pieces[1] if version == 2 and len(pieces) == 2 else name
    for row in json.loads((root / 'catalog.json').read_bytes())['types']:
        path = root / row['file']
        raw = path.read_bytes()
        pins[row['file']] = hashlib.sha256(raw).hexdigest()
        kind = path.suffix[1:]
        roles = {'msg': ['message'], 'srv': ['request', 'response'], 'action': ['goal', 'result', 'feedback']}[kind]
        parts = sections(raw.decode(), version, kind)
        assert len(parts) == len(roles)
        for role, part in zip(roles, parts):
            name = row['type'] if role == 'message' else row['type'] + ('_' if version == 2 else '') + role.title()
            fields = []
            for line in part.splitlines():
                line = line.split('#', 1)[0].strip()
                if not line or re.match(r'^\w+\s+\w+\s*=', line):
                    continue
                match = re.fullmatch(r'([A-Za-z0-9_/]+)(?:<=([0-9]+))?(?:\[([0-9]*|<=[0-9]+)\])?\s+([A-Za-z0-9_]+)(?:\s+.*)?', line)
                assert match, (name, line)
                native, native_string, array, member = match.groups()
                quota = row['bounds'].get(('' if role == 'message' else role + '.') + member, {})
                fields.append(dict(type=resolve(native, name.split('/')[0]), array=array, name=member,
                                   string=quota.get('string', int(native_string) if native_string else 0)))
            assert name not in records
            records[name] = fields
    if version == 1:
        for name in {f['type'] for fields in list(records.values()) for f in fields if f['type'] in ('time', 'duration')}:
            records[name] = [dict(type='uint32' if name == 'time' else 'int32', name=n, array=None, string=0) for n in ('sec', 'nsec')]
    assert set(records) == set(reference_cases) == {v['type'] for v in generation['records']}
    instances = 0
    def scalar(typ, path, member):
        nonlocal instances
        instances += 1
        seed = zlib.crc32(path.encode()) & 0xffffffff
        if typ == 'string':
            raise AssertionError('String requires the explicit field bound')
        if typ in ('byte', 'char'):
            typ = 'int8' if typ == 'byte' and version == 1 else 'uint8'
        if typ == 'bool':
            value, wire = bool(seed & 1), bytes([seed & 1])
        elif typ.startswith(('int', 'uint')):
            signed = typ.startswith('int')
            bits = int(typ[3:] if signed else typ[4:])
            value = ((seed << 32) | (seed ^ 0x9e3779b9)) if bits == 64 else seed & ((1 << bits) - 1)
            if signed and value >= 1 << (bits - 1):
                value -= 1 << bits
            if member in ('nsec', 'nanosec'):
                value = seed % 1000000000
            wire = value.to_bytes(bits // 8, 'little', signed=signed)
        else:
            assert typ in ('float32', 'float64')
            value = ((seed % 4093) - 2046) / 8.0
            wire = struct.pack('<f' if typ == 'float32' else '<d', value)
        return wire, dict(scalar=typ, value=value, wire=wire.hex())
    def fill(name, path):
        nonlocal instances
        wire, members = bytearray(), {}
        for field in records[name]:
            member, typ, array = field['name'], field['type'], field['array']
            field_path = path + '.' + member
            count = 1 if array is None or not array or array.startswith('<=') else int(array)
            if array is not None and (not array or array.startswith('<=')):
                wire.extend(struct.pack('<I', count))
            values = []
            for index in range(count):
                item_path = field_path + ('[' + str(index) + ']' if array is not None else '')
                if typ == 'string':
                    seed = zlib.crc32(item_path.encode()) & 0xffffffff
                    body = b'v' + format(seed, '08x')[:4].encode() + b'\xce\xb2'
                    if len(body) > field['string']:
                        body = format(seed, '08x').encode()[:field['string']]
                    assert body and len(body) <= field['string']
                    wire.extend(struct.pack('<I', len(body)) + body)
                    value = dict(scalar='string', value=body.decode())
                    instances += 1
                elif typ in SCALARS:
                    body, value = scalar(typ, item_path, member)
                    wire.extend(body)
                else:
                    body, value = fill(typ, item_path)
                    wire.extend(body)
                values.append(value)
            members[member] = values[0] if array is None else dict(array=values)
        return bytes(wire), dict(record=name, members=members)
    def symbol(name):
        return 'Ros' + str(version) + ''.join(v[:1].upper() + v[1:] for v in name.split('/'))
    def expected(node, language):
        typ, value = node['scalar'], node['value']
        if typ == 'string':
            return json.dumps(value, ensure_ascii=False)
        if typ == 'bool':
            return 'true' if value else 'false'
        if language == 'javascript' and typ in ('int64', 'uint64'):
            return str(value) + 'n'
        if language == 'assemblyscript':
            native = {'int8':'i8', 'uint8':'u8', 'int16':'i16', 'uint16':'u16',
                      'int32':'i32', 'uint32':'u32', 'int64':'i64', 'uint64':'u64',
                      'float32':'f32', 'float64':'f64'}[typ]
            if typ in ('int64', 'uint64'):
                return '<' + native + '><u64>0x' + bytes.fromhex(node['wire'])[::-1].hex()
            return '<' + native + '>(' + str(value) + ')'
        if language == 'rust':
            return str(value) + {'int8':'i8','uint8':'u8','int16':'i16','uint16':'u16','int32':'i32','uint32':'u32',
                                 'int64':'i64','uint64':'u64','float32':'f32','float64':'f64'}[typ]
        return str(value)
    def checks(node, expression, language):
        lines = []
        def check(condition):
            if language == 'rust':
                lines.append('assert!(' + condition + ');')
            elif language == 'tinygo':
                lines.append('if !(' + condition + '){panic("original field mismatch")}')
            elif language == 'assemblyscript':
                lines.append('assert(' + condition + ');')
            else:
                lines.append('check(' + condition + ');')
        if 'record' in node:
            mapping = generation['member_map'][node['record']][language]
            assert set(mapping) == set(node['members'])
            for member, value in node['members'].items():
                lines.extend(checks(value, expression + '.' + mapping[member], language))
        elif 'array' in node:
            extent = ('len(' + expression + ')' if language == 'tinygo' else expression + ('.len()' if language == 'rust' else '.length'))
            check(extent + ' == ' + str(len(node['array'])))
            for index, value in enumerate(node['array']):
                lines.extend(checks(value, expression + '[' + str(index) + ']', language))
        else:
            value = expression + '.as_str()' if language == 'rust' and node['scalar'] == 'string' else expression
            check(value + (' === ' if language == 'javascript' else ' == ') + expected(node, language))
        return lines
    out = Path(out)
    assert out.is_absolute() and not out.exists()
    out.mkdir(mode=0o700)
    rust, go, assemblyscript, javascript, cases = [], [], [], [], []
    for index, name in enumerate(sorted(records)):
        wire, value = fill(name, name + '#nonzero1')
        assert len(wire) == reference_cases[name]['wire_bytes'] and hashlib.sha256(wire).hexdigest() == reference_cases[name]['wire_sha256']
        if on_case is not None:
            on_case(name, wire, value)
        sym, literal = symbol(name), ','.join('0x%02x' % v for v in wire)
        rust.append('fn test_' + str(index) + '(){let input:&[u8]=&[' + literal + '];let value=r::' + sym + '::decode(input).unwrap();\n' +
            '\n'.join(checks(value, 'value', 'rust')) + '\nassert_eq!(value.encode().unwrap().as_slice(),input);\n' +
            'if !input.is_empty(){assert!(r::' + sym + '::decode(&input[..input.len()-1]).is_err());}\n' +
            'let mut extra=input.to_vec();extra.push(255);assert!(r::' + sym + '::decode(&extra).is_err());}\n')
        go.append('func test_' + str(index) + '(){input:=bytes(' + json.dumps(wire.hex()) + ');value,err:=r.Decode' + sym + '(input);if err!=nil{panic(err)}\n' +
            '\n'.join(checks(value, 'value', 'tinygo')) + '\nencoded,err:=value.Encode();if err!=nil||!equal(encoded,input){panic("wire mismatch")}\n' +
            'if len(input)>0{if _,e:=r.Decode' + sym + '(input[:len(input)-1]);e==nil{panic("truncation accepted")}}\n' +
            'extra:=append(append([]byte{},input...),255);if _,e:=r.Decode' + sym + '(extra);e==nil{panic("trailing byte accepted")}}\n')
        assemblyscript.append('function test_' + str(index) + '():void{const input=bytes(' + json.dumps(wire.hex()) + ');const codec=new r.' + sym + 'Codec();const decoded=codec.decodeChecked(input);assert(decoded.status==0);const value=decoded.value;assert(value!==null);if(value!==null){\n' +
            '\n'.join(checks(value, 'value', 'assemblyscript')) + '\nconst encoded=codec.encodeChecked(value);assert(encoded.status==0&&equal(encoded.bytes,input));}\n' +
            'if(input.length){const bad=codec.decodeChecked(input.subarray(0,input.length-1));assert(bad.status!=0&&bad.value===null);}\n' +
            'const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;const bad=codec.decodeChecked(extra);assert(bad.status!=0&&bad.value===null);}\n')
        javascript.append('function test_' + str(index) + '(){const input=bytes(' + json.dumps(wire.hex()) + '),codec=r.' + sym + 'Codec,value=codec.decode(input);\n' +
            '\n'.join(checks(value, 'value', 'javascript')) + '\ncheck(equal(codec.encode(value),input));\n' +
            'if(input.length)reject(()=>codec.decode(input.subarray(0,input.length-1)));\n' +
            'const extra=new Uint8Array(input.length+1);extra.set(input);extra[input.length]=255;reject(()=>codec.decode(extra));}\n')
        cases.append(dict(type=name, wire_bytes=len(wire), wire_sha256=hashlib.sha256(wire).hexdigest()))
    assert instances == reference['scalar_or_string_field_instances']
    calls = '\n'.join('test_' + str(i) + '();' for i in range(len(cases)))
    heading = '/* Authored original-field checks; no ROS calls or physical command validity. */\n'
    (out / 'main.rs').write_text('// Original-field Rust Wasm value assertions.\nuse roswasm::typed::Canonical;\n#[path=' + json.dumps(str(family / 'bounded_records.rs')) + '] mod r;\n' +
        '\n'.join(rust) + '\nfn main(){\n' + calls + '\n}\n')
    (out / 'main.go').write_text('//go:build tinygo && (wasm || tinygo.wasm) && ros' + str(version) + ' && !ros' + str(3-version) +
        '\n\n// Complete decoded values, exact wire bytes and malformed input checks.\npackage main\nimport("encoding/hex";r "uwvm2.local/managed-values/records")\n' +
        'func bytes(s string)[]byte{v,e:=hex.DecodeString(s);if e!=nil{panic(e)};return v}\n' +
        'func equal(a,b []byte)bool{if len(a)!=len(b){return false};for i:=range a{if a[i]!=b[i]{return false}};return true}\n' +
        '\n'.join(go) + '\nfunc main(){\n' + calls + '\n}\n')
    ts_base = kit / 'tests/managed_values'
    (out / 'main.ts').write_text(heading + 'import * as r from ' + json.dumps(os.path.relpath(family / 'bounded_records', ts_base)) + ';\n' +
        'import {abortGuest} from ' + json.dumps(os.path.relpath(source / 'sdk/origin/assemblyscript/api', ts_base)) + ';\nexport {abortGuest};\n' +
        'function bytes(s:string):Uint8Array{const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i){let a=s.charCodeAt(i*2),b=s.charCodeAt(i*2+1);out[i]=<u8>(((a<58?a-48:a-87)<<4)|(b<58?b-48:b-87));}return out;}\n' +
        'function equal(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}\n' +
        '\n'.join(assemblyscript) + '\nexport function _start():void{\n' + calls + '\n}\n')
    (out / 'main.mjs').write_text(heading + 'import * as r from ' + json.dumps(str((family / 'bounded_records.mjs').relative_to(source))) + ';\n' +
        'function check(v){if(!v)throw new Error("original field or wire mismatch");}\nfunction reject(fn){let failed=false;try{fn();}catch(e){failed=true;}check(failed);}\n' +
        'function bytes(s){const out=new Uint8Array(s.length/2);for(let i=0;i<out.length;++i)out[i]=parseInt(s.slice(i*2,i*2+2),16);return out;}\n' +
        'function equal(a,b){if(a.length!==b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!==b[i])return false;return true;}\n' +
        '\n'.join(javascript) + '\nexport function main(){\n' + calls + '\nreturn 0;\n}\n')
    report = dict(status='FOUR_MANAGED_VALUE_CONTRACT_SOURCES_MATCH_INDEPENDENT_C_CPP_REFERENCE_VECTORS',
        vendor=vendor, profile=profile, records=len(cases), scalar_or_string_instances=instances,
        original_source_sha256=pins, cases=cases, source_only=True, default_authority=[])
    (out / 'managed-vectors.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--vendor', required=True)
    p.add_argument('--profile', required=True)
    p.add_argument('--vectors', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    result = build(a.source, a.vendor, a.profile, a.vectors, a.out)
    print(json.dumps({k: result[k] for k in ('vendor', 'profile', 'records', 'scalar_or_string_instances')}))
