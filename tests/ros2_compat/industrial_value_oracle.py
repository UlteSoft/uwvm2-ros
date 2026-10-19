#!/usr/bin/env python3
"""Emit independent native value witnesses from captured installed ROS2 IDL.

This tool deliberately does not import the production schema parser, portable
record generator or codec emitter. Original installed IDL facts select native
members; the separately reviewed source catalog supplies finite admission
bounds. Python struct computes canonical expected bytes independently. The
generated program must be built against the actual same-source native package
overlay and production converters, then run by the admitted cgroup owner.
Neither generating nor passing this local contract qualifies a robot driver,
Guest transport, physical controller, upstream test suite or performance.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import re
import stat
import struct


SCALARS = dict(bool='B', byte='B', char='B', int8='b', uint8='B', int16='h',
               uint16='H', int32='i', uint32='I', int64='q', uint64='Q',
               float32='f', float64='d')
TYPE = r'[a-z][a-z0-9_]{0,62}/(?:msg|srv|action)/[A-Z][A-Za-z0-9_]*'
FIELDS = {'name', 'type', 'array_size', 'is_array', 'string_bound',
          'is_sequence', 'sequence_bound', 'native_default', 'has_declared_default'}
LIMIT = 16 * 1024**2


def need(ok, reason):
    if not ok:
        raise ValueError(reason)


def read(path):
    need(path.is_absolute() and path.resolve(strict=True) == path,
         'physical absolute original input required')
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        before = os.fstat(fd)
        need(stat.S_ISREG(before.st_mode) and before.st_nlink == 1 and
             0 <= before.st_size <= LIMIT, 'bounded ordinary single-link input required')
        chunks = []; size = 0
        while raw := os.read(fd, min(65536, LIMIT + 1 - size)):
            size += len(raw); chunks.append(raw)
            need(size <= before.st_size and size <= LIMIT, 'original input grew')
        key = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns, s.st_nlink)
        need(size == before.st_size and key(before) == key(os.fstat(fd)) == key(path.lstat()),
             'original input changed during one-FD capture')
        return b''.join(chunks)
    finally:
        os.close(fd)


def strict(raw):
    def pairs(rows):
        result = {}
        for key, value in rows:
            need(key not in result, 'duplicate JSON member')
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda _: need(False, 'nonfinite JSON token'))


def pin(raw):
    return dict(bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def generate(catalog_raw, installed_raw):
    catalog = strict(catalog_raw); installed = strict(installed_raw)
    need(type(catalog) is dict and set(catalog) == {'format', 'types'} and
         catalog['format'] == 'uwvm-ros-vendor-complete-bounded-v1' and
         type(catalog['types']) is list and 1 <= len(catalog['types']) <= 128,
         'complete unbound source catalog required')
    need(type(installed) is dict and installed.get('format') ==
         'uwvm-ros2-installed-protocol-idl-source-v1' and installed.get('default_authority') == [],
         'explicit installed protocol IDL source facts required')
    need(type(installed.get('records')) is list and 1 <= len(installed['records']) <= 256 and
         type(installed.get('interfaces')) is list, 'finite installed declarations required')
    records = {}; interfaces = {}; selections = {}; roots = []
    for row in installed['records']:
        name = row.get('type') if type(row) is dict else None
        need(type(name) is str and re.fullmatch(TYPE, name) and name not in records and
             type(row.get('fields')) is list and 1 <= len(row['fields']) <= 4096,
             'unique complete native record required')
        names = set()
        for field in row['fields']:
            need(type(field) is dict and set(field) == FIELDS and
                 type(field['name']) is str and re.fullmatch(r'[a-z][a-z0-9_]*', field['name']) and
                 field['name'] not in names, 'exact unique native field required')
            names.add(field['name'])
            kind = field['type']
            need(type(kind) is str and (kind in SCALARS or kind == 'string' or re.fullmatch(TYPE, kind)),
                 'unsupported native field type')
            need(all(type(field[k]) is bool for k in ('is_array', 'is_sequence', 'has_declared_default')) and
                 not (field['is_array'] and field['is_sequence']) and
                 all(type(field[k]) is int and 0 <= field[k] <= 32768
                     for k in ('array_size', 'string_bound', 'sequence_bound')) and
                 (field['array_size'] > 0) == field['is_array'] and
                 (not field['sequence_bound'] or field['is_sequence']) and
                 (not field['string_bound'] or kind == 'string'), 'invalid native field shape')
        records[name] = row
    for row in installed['interfaces']:
        name = row.get('type') if type(row) is dict else None
        need(type(name) is str and re.fullmatch(TYPE, name) and name not in interfaces and
             type(row.get('components')) is dict, 'unique native interface required')
        roles = {'message': ('message',), 'service': ('request', 'response'),
                 'action': ('goal', 'result', 'feedback')}.get(row.get('kind'))
        need(roles and set(row['components']) == set(roles), 'complete native component roles required')
        interfaces[name] = row
    for row in catalog['types']:
        need(type(row) is dict and row.get('ros_version') == 2 and type(row.get('type')) is str and
             row['type'] in interfaces and row['type'] not in selections and
             type(row.get('registered')) is bool and type(row.get('bounds')) is dict,
             'unique explicit ROS2 source selection required')
        name = row['type']; interface = interfaces[name]
        selections[name] = row
        for role, component in interface['components'].items():
            need(component in records and component not in roots, 'one actual native payload per role required')
            roots.append(component)
    # Every selected dependency has its own reviewed bounds. Protocol transport
    # envelopes are retained as source facts but are not public value aliases.
    by_payload = {}
    for name, row in selections.items():
        for role, component in interfaces[name]['components'].items():
            prefix = '' if role == 'message' else role + '.'
            bounds = {key[len(prefix):]: value for key, value in row['bounds'].items()
                      if key.startswith(prefix)}
            by_payload[component] = bounds
    for name in roots:
        for field in records[name]['fields']:
            need('/' not in field['type'] or field['type'] in by_payload,
                 'missing selected original Message dependency: ' + name)

    def capacity(field, bounds, key):
        explicit = bounds.get(field['name'], {}).get(key)
        declared = field['sequence_bound' if key == 'sequence' else 'string_bound']
        result = explicit if explicit is not None else declared
        need(type(result) is int and 1 <= result <= 32768 and
             (not declared or result <= declared), 'missing/rejected finite native ' + key + ' bound')
        return result

    def scalar(kind, seed, mode):
        if kind == 'bool':
            value = bool(seed & 1); literal = 'true' if value else 'false'
        elif kind.startswith('float'):
            value = -0.0 if mode == 0 else (-1 if seed & 1 else 1) * (seed % 31 + .25)
            literal = repr(value)
        else:
            fmt = SCALARS[kind]; bits = struct.calcsize(fmt) * 8
            signed = fmt.islower()
            value = (-0x123456789abcde + seed if bits == 64 and signed else
                     0xfedcba9876543210 - seed if bits == 64 else
                     seed % (1 << (bits - 1)) - (1 << (bits - 2)) if signed else
                     (0xa1b2c3d4 ^ seed) & ((1 << bits) - 1))
            literal = ('(-INT64_C(' + str(-value) + '))' if signed and bits == 64 and value < 0 else
                       ('INT64_C' if signed else 'UINT64_C') + '(' + str(value) + ')')
        need(not isinstance(value, float) or math.isfinite(value), 'finite independent scalar required')
        return struct.pack('<' + SCALARS[kind], value), literal

    def fill(name, expression, mode, wire, assignments, malformed, depth=0):
        need(depth <= 16, 'finite acyclic native graph required')
        row = records[name]; bounds = by_payload[name]
        if len(row['fields']) == 1 and row['fields'][0]['name'] == 'structure_needs_at_least_one_member':
            need(row['fields'][0]['type'] == 'uint8' and not row['fields'][0]['is_array'] and
                 not row['fields'][0]['is_sequence'], 'exact SDK empty-message placeholder required')
            return
        for field in row['fields']:
            path = expression + '.' + field['name']; kind = field['type']
            if field['is_sequence']:
                bound = capacity(field, bounds, 'sequence'); count = (0, 1, min(2, bound))[mode]
                malformed.append(dict(offset=len(wire), bytes=[255] * 4, reason='sequence-length'))
                wire.extend(struct.pack('<I', count)); assignments.append(path + '.resize(' + str(count) + ');')
            else:
                count = field['array_size'] if field['is_array'] else 1
            for index in range(count):
                target = path + '[' + str(index) + ']' if field['is_sequence'] or field['is_array'] else path
                seed = 1 + int.from_bytes(hashlib.sha256(target.encode()).digest()[:2], 'little') + mode * 65537
                if kind in SCALARS:
                    if name in ('builtin_interfaces/msg/Time', 'builtin_interfaces/msg/Duration') and field['name'] == 'nanosec':
                        value = (0, 123456789, 999999999)[mode]
                        raw, literal = struct.pack('<I', value), str(value)
                    else:
                        raw, literal = scalar(kind, seed, mode)
                    if kind == 'bool':
                        malformed.append(dict(offset=len(wire), bytes=[2], reason='boolean'))
                    wire.extend(raw); assignments.append(target + '=' + literal + ';')
                elif kind == 'string':
                    bound = capacity(field, bounds, 'string')
                    text = (b'', b'A', b'\xce\xbcX' if bound >= 3 else b'B')[mode]
                    malformed.append(dict(offset=len(wire), bytes=[255] * 4, reason='string-length'))
                    wire.extend(struct.pack('<I', len(text)))
                    if text:
                        malformed.append(dict(offset=len(wire), bytes=[255], reason='utf8'))
                    wire.extend(text)
                    assignments.append(target + '.assign({' + ','.join('char(' + str(b) + ')' for b in text) + '});')
                else:
                    fill(kind, target, mode, wire, assignments, malformed, depth + 1)
                need(len(wire) <= 32768 and len(assignments) <= 16384, 'finite canonical/native scenario required')

    lines = ['/** Independent installed-IDL native value witness. No Node or grant. */',
             '#include "gateways/ros2/custom_services.h"', '#include "gateways/ros2/custom_action_contract.h"',
             '#include <rclcpp/serialization.hpp>', '#include <rclcpp/serialized_message.hpp>',
             '#include <fast_io.h>', '#include <cassert>', '#include <csignal>', '#include <cstring>',
             '#include <cstdint>', '#include <vector>', '#include <unistd.h>',
             'template<class Native>struct oracle;']
    scenarios = []
    for name in roots:
        native = '::' + name.replace('/', '::')
        lines += ['template<>struct oracle<' + native + '>{using Native=' + native + ';',
                  'static Native value(unsigned mode){Native value{};switch(mode){']
        variants = []
        for mode in range(3):
            wire = bytearray(); assignments = []; malformed = []
            fill(name, 'value', mode, wire, assignments, malformed)
            variants.append((bytes(wire), malformed))
            lines += ['case ' + str(mode) + ':{', *assignments, 'return value;}']
            scenarios.append(dict(type=name, mode=mode, canonical=pin(wire), malformed_fields=len(malformed)))
        lines += ['default:std::abort();}}', 'static std::vector<std::uint8_t> bytes(unsigned mode){switch(mode){']
        for mode, (wire, _) in enumerate(variants):
            lines.append('case ' + str(mode) + ':return {' + ','.join(map(str, wire)) + '};')
        lines += ['default:std::abort();}}',
                  'static void mutate(unsigned mode,std::vector<std::vector<std::uint8_t>>& output){auto original=bytes(mode);switch(mode){']
        for mode, (_, malformed) in enumerate(variants):
            lines.append('case ' + str(mode) + ':')
            for mutation in malformed:
                lines.append('{auto bad=original;')
                for index, value in enumerate(mutation['bytes']):
                    lines.append('bad[' + str(mutation['offset'] + index) + ']=' + str(value) + ';')
                lines.append('output.push_back(std::move(bad));}')
            lines.append('return;')
        lines += ['default:std::abort();}}};']
    lines += ['unsigned payloads{},cases{},refusals{};',
              'template<class Codec>void check(){using Native=typename Codec::Native;++payloads;',
              'for(unsigned mode=0;mode!=3;++mode){++cases;auto original=oracle<Native>::value(mode);auto expected=oracle<Native>::bytes(mode);',
              'assert((::uwvm2::ros_gateway::ros2_custom_topics::binding<Native,Codec>::native_contract()));',
              'assert(Codec::valid(original));std::vector<std::uint8_t> encoded;assert(Codec::encode(original,encoded)&&encoded==expected);',
              'Native decoded{};assert(Codec::decode(expected.data(),expected.size(),decoded));assert(decoded==original);',
              'rclcpp::Serialization<Native> serialization;rclcpp::SerializedMessage cdr;Native restored{};',
              'serialization.serialize_message(&original,&cdr);serialization.deserialize_message(&cdr,&restored);assert(restored==original);',
              'assert(Codec::encode(restored,encoded)&&encoded==expected);',
              'auto sentinel=oracle<Native>::value((mode+1)%3);auto before=sentinel;',
              'std::vector<std::vector<std::uint8_t>> malformed;oracle<Native>::mutate(mode,malformed);',
              'auto trailing=expected;trailing.push_back(0);malformed.push_back(std::move(trailing));',
              'if(!expected.empty()){auto short_value=expected;short_value.pop_back();malformed.push_back(std::move(short_value));}',
              'for(auto const& bad:malformed){++refusals;assert(!Codec::decode(bad.data(),bad.size(),sentinel));assert(sentinel==before);}}}',
              'int main(int argc,char**argv){if(argc!=2||std::strcmp(argv[1],"--mapped-stop"))return 64;',
              '#define CHECK_MESSAGE(Native,Name,Identity,Canonical,Bytes,Codec) check<Codec>();',
              'UWVM_ROS_CUSTOM_ROS2_TYPES(CHECK_MESSAGE)', '#undef CHECK_MESSAGE',
              '#define CHECK_SERVICE(Native,Name,Identity,Canonical,Q,S) check<Q>();check<S>();',
              'UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(CHECK_SERVICE)', '#undef CHECK_SERVICE',
              '#define CHECK_ACTION(Native,Name,Identity,Canonical,G,R,F) check<G>();check<R>();check<F>();',
              'UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES(CHECK_ACTION)', '#undef CHECK_ACTION',
              'fast_io::io::println("PASS installed industrial native payloads=",payloads," cases=",cases," refusals=",refusals);',
              'if(::raise(SIGSTOP)!=0)return 1;char gate=0;if(::read(STDIN_FILENO,&gate,1)!=1||gate!=\'I\')return 1;return 0;}']
    raw = ('\n'.join(lines) + '\n').encode()
    need(len(raw) <= 8 * 1024**2, 'bounded complete generated witness required')
    return raw, dict(scope='Independent installed-IDL canonical/native/CDR witness SOURCE, not execution, robot, Guest or official acceptance.',
                     original_catalog=pin(catalog_raw), original_installed_idl=pin(installed_raw),
                     records=len(roots), public_payloads=sum(len(interfaces[n]['components'])
                         for n, row in selections.items() if row['registered']),
                     scenarios=scenarios, contract=pin(raw), default_authority=[])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('catalog', 'installed-idl', 'out'):
        parser.add_argument('--' + name, required=True, type=Path)
    args = parser.parse_args()
    inputs = {name: read(getattr(args, name)) for name in ('catalog', 'installed_idl')}
    raw, report = generate(inputs['catalog'], inputs['installed_idl'])
    need(args.out.is_absolute() and args.out.parent.resolve(strict=True) == args.out.parent and
         not args.out.exists() and not args.out.is_symlink(), 'fresh physical owned output required')
    for name, captured in inputs.items():
        need(read(getattr(args, name)) == captured, 'original input drift before output')
    args.out.mkdir(mode=0o700)
    for name, value in (('contract.cc', raw), ('source-oracle.json',
                        (json.dumps(report, sort_keys=True, indent=2) + '\n').encode())):
        fd = os.open(args.out / name, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o600)
        with os.fdopen(fd, 'wb') as stream:
            stream.write(value)


if __name__ == '__main__':
    main()
