#!/usr/bin/env python3
"""Make independent canonical-wire scenarios from whole locked Unitree .msg files.

No production schema parser or codec emitter is imported. This deliberately
narrow reader accepts the original Unitree field grammar only, failing on any
unhandled declaration. It constructs expected little-endian canonical bytes
with Python struct, populates every original field, and writes a C11/C++20 test
entry. This is a local SDK oracle, not ROS1 serialization, CDR or native proof.
The caller must compile and run the produced entry separately in its cgroup.
For Wasm, explicitly budget the two32769-byte buffers and three complete records:
the qualified recipe selects -Wl,-z,stack-size=1048576,
-Wl,--initial-memory=2097152 and -Wl,--max-memory=33554432. The default64KiB
stack is insufficient for this deliberately large independent test entry.
This documentation does not change the emitted test Source or its semantics.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct


SCALARS = dict(bool='B', int8='b', uint8='B', int16='h', uint16='H',
               int32='i', uint32='I', int64='q', uint64='Q', float32='f', float64='d')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--materialized', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--native-ros2', action='store_true', help=(
        'Additionally compile against genuine ROS2 Message headers and rclcpp '
        'serialization; no Node, endpoint, Gateway registration or grant.'))
    parser.add_argument('--native-converters', action='store_true', help=(
        'With --native-ros2, also check the actual selected production Native '
        'binding and generated converters against independent values/bytes.'))
    args = parser.parse_args()
    if args.native_converters and not args.native_ros2:
        raise ValueError('production Native converters require explicit native-ROS2 mode')
    root = args.materialized.resolve(strict=True)
    catalog = json.loads((root / 'catalog.preparation.json').read_text())
    preparation = json.loads((root / 'report.json').read_text())
    if catalog['format'] != 'uwvm-ros-vendor-complete-bounded-v1' or (
            preparation['status'] != 'PREPARED_NOT_EXECUTED' or
            preparation['runtime_permissions'] != []):
        raise ValueError('explicit locked, unbound complete-family preparation required')
    expected_inputs = {(p['bytes'], p['sha256']) for p in preparation['inputs']}
    records = {}
    inputs = []
    for row in catalog['types']:
        raw = (root / row['file']).read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        if (len(raw), digest) not in expected_inputs:
            raise ValueError('original full definition differs from locked preparation')
        fields = []
        for line in raw.decode('utf-8').splitlines():
            line = line.split('#', 1)[0].strip()
            if not line:
                continue
            match = re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:\[([0-9]*)\])?\s+([A-Za-z_][A-Za-z0-9_]*)', line)
            if not match:
                raise ValueError('unsupported original declaration; no projection allowed: ' + line)
            fields.append(match.groups())
        if row['type'] in records:
            raise ValueError('duplicate original type')
        records[row['type']] = (row, fields)
        inputs.append(dict(file=row['file'], bytes=len(raw), sha256=digest))
    if not 1 <= len(records) <= 128:
        raise ValueError('finite complete family required')
    out = args.out.absolute()
    out.mkdir(mode=0o700)
    lines = ['/** Generated independent whole-field canonical scenarios; no ROS imports. */',
        '#ifdef __cplusplus', '#include "bounded_records.hpp"', '#else',
        '#include "bounded_records.h"', '#endif', '#include <string.h>', '#include <stdio.h>',
        '#define CHECK(x) do { if(!(x)){fprintf(stderr,"contract check failed at line %u\\n",(unsigned)__LINE__);return 1;} } while(0)']
    if args.native_ros2:
        if any(row['ros_version'] != 2 for row, fields in records.values()):
            raise ValueError('native ROS2 witnesses require only actual ROS2 messages')
        lines[0] = '/** Independent canonical/actual ROS2 CDR value witness; no Node or authority. */'
        lines += ['#ifndef __cplusplus', '#error "Native ROS2 witnesses require C++20"',
            '#endif', '#include <rclcpp/serialization.hpp>',
            '#include <rclcpp/serialized_message.hpp>', '#include <string>',
            '#include <csignal>', '#include <unistd.h>']
        for name in sorted(records):
            package, category, symbol = name.split('/')
            if category != 'msg':
                raise ValueError('actual ROS2 Message roots required')
            header = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', symbol)
            header = re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', header).lower()
            lines.append('#include <' + package + '/msg/' + header + '.hpp>')
        if args.native_converters:
            lines += ['#include "gateways/ros2/custom_topics.h"']
    scenarios = []

    def native_copy(name, portable, native, statements, *, from_native, depth=0):
        """Independent field spelling/shape bridge, never a Gateway provider.

        Only the original full declarations select actual generated C++ members.
        No offset, native pointer or CDR byte stream enters a Guest. This bridge
        is intentionally separate from the production Native converter under
        test elsewhere; success here alone cannot qualify that converter.
        """
        if depth > 16:
            raise ValueError('finite native value dependency graph required')
        row, fields = records[name]
        for kind, shape, member in fields:
            source = portable + '.' + member
            target = native + '.' + member
            if shape is not None:
                if not shape:
                    limit = row['bounds'][member]['sequence']
                    if from_native:
                        statements += ['CHECK(' + target + '.size()<=' + str(limit) + ');',
                            source + '.size=(uint32_t)' + target + '.size();']
                    else:
                        statements.append(target + '.resize(' + source + '.size);')
                else:
                    statements.append('CHECK(' + target + '.size()==' + shape + ');')
                iterator = 'n' + str(depth)
                count = source + '.size' if not shape else shape
                statements.append('for(size_t ' + iterator + '=0;' + iterator + '<' + count + ';++' + iterator + '){')
                source += ('.data' if not shape else '') + '[' + iterator + ']'
                target += '[' + iterator + ']'
            if kind in SCALARS:
                statements.append((source + '=' + target if from_native else target + '=' + source) + ';')
            elif kind == 'string':
                if from_native:
                    statements += ['CHECK(' + target + '.size()<=' + str(row['bounds'][member]['string']) + ');',
                        source + '.size=(uint32_t)' + target + '.size();',
                        'if(' + source + '.size)memcpy(' + source + '.data,' + target + '.data(),' + source + '.size);']
                else:
                    statements.append(target + '.assign(reinterpret_cast<char const*>(' + source + '.data),' + source + '.size);')
            else:
                nested = kind if '/' in kind else name.split('/')[0] + '/msg/' + kind
                native_copy(nested, source, target, statements, from_native=from_native, depth=depth + 1)
            if shape is not None:
                statements.append('}')

    def scalar(kind, seed):
        if kind == 'bool': value = seed % 2
        elif kind.startswith('float'): value = (-1 if seed % 2 else 1) * (seed % 31 + 0.25)
        elif kind == 'uint64': value = 0xfedcba9876543210 - seed
        elif kind == 'int64': value = -0x123456789abcde + seed
        elif kind.startswith('uint'):
            bits = int(kind[4:]); value = (0xa1b2c3d4 ^ seed) & ((1 << bits) - 1)
        else:
            bits = int(kind[3:]); value = seed % (1 << (bits - 1)) - (1 << (bits - 2))
        literal = repr(value)
        if kind == 'uint64': literal = 'UINT64_C(' + str(value) + ')'
        elif kind == 'int64': literal = '(-INT64_C(' + str(-value) + '))'
        elif kind == 'uint32': literal = 'UINT32_C(' + str(value) + ')'
        return struct.pack('<' + SCALARS[kind], value), literal

    def fill(name, path, mode, wire, assignments, guards, depth=0):
        if depth > 16:
            raise ValueError('finite acyclic original definition required')
        row, fields = records[name]
        package = name.split('/')[0]
        for kind, shape, member in fields:
            target = path + '.' + member
            bounds = row['bounds'].get(member, {})
            if shape is None:
                count = 1
            elif shape:
                count = int(shape)
            else:
                limit = bounds['sequence']
                count = (0, min(1, limit), limit)[mode]
                assignments.append(target + '.size=' + str(count) + ';')
                guards.append((target + '.size', str(limit + 1)))
                wire.extend(struct.pack('<I', count))
            for index in range(count):
                field = target if shape is None else target + ('.data' if not shape else '') + '[' + str(index) + ']'
                # Vary scalar/fixed-array values as well as dynamic prefixes.
                # Otherwise fixed-only records would repeat one payload in all
                # three modes, concealing a constant-value encoder defect.
                seed = 1 + int.from_bytes(hashlib.sha256(field.encode()).digest()[:2], 'little') + mode * 65537
                if kind in SCALARS:
                    packed, literal = scalar(kind, seed)
                    wire.extend(packed); assignments.append(field + '=' + literal + ';')
                    if kind == 'bool': guards.append((field, '2'))
                elif kind == 'string':
                    limit = bounds['string']
                    sample = b'\xce\xbcX' if limit >= 3 else b'X'[:limit]
                    text = (b'', sample, (b'Unitree' * ((limit + 6) // 7))[:limit])[mode]
                    wire.extend(struct.pack('<I', len(text))); wire.extend(text)
                    assignments.append(field + '.size=' + str(len(text)) + ';')
                    if text:
                        assignments.append('{static unsigned char const text[]={' + ','.join(map(str, text)) + '};memcpy(' + field + '.data,text,sizeof text);}')
                    guards.append((field + '.size', str(limit + 1)))
                else:
                    nested = kind if '/' in kind else package + ('/msg/' if row['ros_version'] == 2 else '/') + kind
                    if nested not in records:
                        raise ValueError('missing complete original nested type: ' + nested)
                    fill(nested, field, mode, wire, assignments, guards, depth + 1)

    for number, (name, (row, fields)) in enumerate(records.items()):
        cs = 'roswasmc_bounded_' + str(row['ros_version']) + '_' + name.replace('/', '_')
        cpp = 'Ros' + str(row['ros_version']) + ''.join(s[:1].upper() + s[1:] for s in name.split('/'))
        for mode in range(3):
            wire = bytearray(); assignments = []; guards = []
            fill(name, 'value', mode, wire, assignments, guards)
            if not 0 < len(wire) <= 32768:
                raise ValueError('complete canonical wire bound required')
            function = 'case_' + str(number) + '_' + str(mode)
            lines += ['static int ' + function + '(void) {',
                'static unsigned char const expected[]={' + ','.join(map(str, wire)) + '};',
                cs + ' value; ' + cs + ' decoded; ' + cs + ' before;',
                'unsigned char output[32769],snapshot[32769];size_t used=123;',
                'memset(&value,0,sizeof value);memset(&decoded,0x5a,sizeof decoded);',
                *assignments, 'memset(output,0x5a,sizeof output);memcpy(snapshot,output,sizeof output);',
                'CHECK(' + cs + '_encode(&value,output,sizeof expected-1,&used)==90);',
                'CHECK(used==123&&memcmp(output,snapshot,sizeof output)==0);',
                'CHECK(' + cs + '_encode(&value,output,sizeof output,&used)==0);',
                'CHECK(used==sizeof expected&&memcmp(output,expected,used)==0);',
                'memcpy(&before,&decoded,sizeof decoded);',
                'CHECK(' + cs + '_decode(expected,sizeof expected-1,&decoded)==22);',
                'CHECK(memcmp(&before,&decoded,sizeof decoded)==0);',
                'output[used]=0;', 'CHECK(' + cs + '_decode(output,used+1,&decoded)==22);',
                'CHECK(memcmp(&before,&decoded,sizeof decoded)==0);',
                'CHECK(' + cs + '_decode(expected,sizeof expected,&decoded)==0);',
                'CHECK(' + cs + '_encode(&decoded,output,sizeof output,&used)==0);',
                'CHECK(used==sizeof expected&&memcmp(output,expected,used)==0);']
            if args.native_ros2:
                native = '::' + name.replace('/', '::')
                lines += ['{', native + ' original; ' + native + ' restored;',
                    'rclcpp::Serialization<' + native + '> serialization;',
                    'rclcpp::SerializedMessage serialized;']
                native_copy(name, 'value', 'original', lines, from_native=False)
                if args.native_converters:
                    codec = '::uwvm_custom_bounded_native::' + cpp + 'Codec'
                    lines += ['{using converter=' + codec + ';',
                        'CHECK((::uwvm2::ros_gateway::ros2_custom_topics::binding<' + native + ',converter>::native_contract()));',
                        native + ' converted;', 'std::vector<std::uint8_t> canonical;',
                        'CHECK(converter::decode(expected,sizeof expected,converted));',
                        'CHECK(converted==original);',
                        'CHECK(converter::encode(converted,canonical));',
                        'CHECK(canonical.size()==sizeof expected&&memcmp(canonical.data(),expected,sizeof expected)==0);', '}']
                lines += ['serialization.serialize_message(&original,&serialized);',
                    'CHECK(serialized.size()>=4&&serialized.size()<=16777216);',
                    'serialization.deserialize_message(&serialized,&restored);',
                    'CHECK(original==restored);',
                    'memset(&decoded,0,sizeof decoded);']
                native_copy(name, 'decoded', 'restored', lines, from_native=True)
                lines += ['CHECK(' + cs + '_encode(&decoded,output,sizeof output,&used)==0);',
                    'CHECK(used==sizeof expected&&memcmp(output,expected,used)==0);', '}']
            # Independently challenge every dynamic length / boolean field.
            # These are malformed input values, not an alternative serializer.
            for target, invalid in guards:
                lines += ['{uint32_t saved=(uint32_t)' + target + ';' + target + '=' + invalid + ';',
                    'memset(output,0x5a,sizeof output);memcpy(snapshot,output,sizeof output);used=123;',
                    'CHECK(' + cs + '_encode(&value,output,sizeof output,&used)==22);',
                    'CHECK(used==123&&memcmp(output,snapshot,sizeof output)==0);',
                    target + '=saved;}']
            lines += ['#ifdef __cplusplus',
                '{roswasmcpp::wire::bytes bytes;uwvm_custom::' + cpp + ' next{};',
                'CHECK(roswasmcpp::Codec<uwvm_custom::' + cpp + '>::encode(value,bytes));',
                'CHECK(bytes.size()==sizeof expected&&memcmp(bytes.data(),expected,bytes.size())==0);',
                'CHECK(roswasmcpp::Codec<uwvm_custom::' + cpp + '>::decode(bytes,next));',
                'CHECK(roswasmcpp::Codec<uwvm_custom::' + cpp + '>::encode(next,bytes));',
                'CHECK(bytes.size()==sizeof expected&&memcmp(bytes.data(),expected,bytes.size())==0);}',
                '#endif', 'return 0;', '}']
            scenarios.append(dict(function=function, type=name, dynamic_mode=mode,
                wire_bytes=len(wire), wire_sha256=hashlib.sha256(wire).hexdigest(),
                malformed_value_cases=len(guards)))
    for name in records:
        if len({case['wire_sha256'] for case in scenarios if case['type'] == name}) != 3:
            raise ValueError('three distinct whole-field payloads required for ' + name)
    lines += (['int main(int argc,char** argv) {int result;',
        'if(argc!=2||strcmp(argv[1],"--mapped-stop"))return 64;']
        if args.native_ros2 else ['int main(void) {int result;'])
    for scenario in scenarios:
        lines.append('result=' + scenario['function'] + '();if(result)return result;')
    if args.native_ros2:
        # All actual serializer calls and their late-loaded libraries precede
        # this owned checkpoint. The original parent must observe the stopped
        # child, concrete mappings and cgroup before providing the one-byte gate.
        lines += ['if(::raise(SIGSTOP)!=0)return 1;',
            'char gate=0;if(::read(STDIN_FILENO,&gate,1)!=1||gate!=\'I\')return 1;']
    lines += ['return 0;', '}']
    raw = ('\n'.join(lines) + '\n').encode()
    if len(raw) > 8 * 1024**2:
        raise ValueError('finite independent contract source bound required')
    (out / 'contract.c').write_bytes(raw)
    (out / 'source-oracle.json').write_text(json.dumps(dict(
        scope=('Independent original-field canonical, real ROS2 CDR and production Native binding/converter witness Source; '
               'not executed by this generator; no Node, endpoint, Guest, ROS network, robot or official-suite acceptance.'
               if args.native_converters else
               'Independent original-field canonical and actual ROS2 CDR value witness Source; '
               'not compiled or executed by this generator; no production Native-converter, '
               'Gateway, transport, identity, SDK, robot or official-suite acceptance.'
               if args.native_ros2 else
               'Independent original-field canonical oracle Source; not compiled or executed by this generator.'),
        native_ros2_value_witness=args.native_ros2,
        production_native_converters=args.native_converters,
        original_definitions=inputs, records=len(records), scenarios=scenarios,
        contract=dict(bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())), indent=2) + '\n')


if __name__ == '__main__':
    main()
