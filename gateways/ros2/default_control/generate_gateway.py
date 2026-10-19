#!/usr/bin/env python3
"""Prepare independent genuine C++ Gateway control adapters as SOURCE only.

Explicit SHA-paired trusted current IR modules are read through retained bounded
regular descriptors before loading. This operator never runs a compiler, native
accessor, entity, probe or subprocess. Independent actual native hashes remain
mandatory later bootstrap input; this operator does not calculate native IDs.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import stat
import sys
import types

PROFILE = 'uwvm-ros-original-node-control-c-cpp-v1'
LIMIT = 8 * 1024 * 1024

def read(path, limit=LIMIT):
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or first.st_size > limit:
            raise ValueError('bounded regular source required')
        parts, count = [], 0
        while True:
            part = os.read(fd, min(1024 * 1024, limit - count + 1))
            if not part:
                break
            count += len(part)
            if count > limit:
                raise ValueError('source grew beyond bound')
            parts.append(part)
        after, named = os.fstat(fd), os.lstat(path)
        def key(s):
            return s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns, s.st_mode, s.st_nlink
        if key(first) != key(after) or key(first) != key(named) or count != first.st_size:
            raise ValueError('source identity or extent drift')
        return b''.join(parts)
    finally:
        os.close(fd)

def strict(raw):
    def pairs(rows):
        result = {}
        for key, value in rows:
            if key in result:
                raise ValueError('duplicate JSON key')
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: (_ for _ in ()).throw(ValueError('nonfinite JSON')))

def literal(value):
    return json.dumps(value, ensure_ascii=True)

def binary_view(value):
    return 'std::string_view{"' + ''.join('\\x' + format(b, '02x') for b in value) + '",' + str(len(value)) + '}'

def native_source_bytes(raw):
    text = raw.decode('utf-8')
    if '\r' in text:
        raise ValueError('selected original LF source required')
    # Exact pinned full__description.c.em: emitted C storage is splitlines()
    # joined by LF, then the C literal terminator; static_seq still declares
    # len(ORIGINAL TEXT), not normalized length. All fixed actual control files
    # are ASCII+terminal LF, so that declared view contains the final NUL. This
    # is metadata evidence, not permission to erase it or relax business UTF-8.
    storage = '\n'.join(text.splitlines()).encode('utf-8') + b'\0'
    extent = len(text)
    if extent > len(storage):
        raise ValueError('original SDK source getter would exceed emitted storage')
    return storage[:extent]

def native(name, language):
    return '::' + ('__' if language == 'c' else '::').join(name.split('/'))

def header(parent, name, language):
    package, category, local = name.split('/')
    for suffix in ('_Request', '_Response', '_Event'):
        if local.endswith(suffix):
            local = local[:-len(suffix)]
            break
    return package + '/' + category + '/' + parent.ros2_header(local) + ('.h' if language == 'c' else '.hpp')

def field(member):
    return dict(name=member.name, type=member.type, native_scalar=member.native_scalar,
                array=member.array, count=member.count, string=member.string_bound,
                native_string=member.native_string_bound, native_sequence=member.native_sequence_bound,
                default=member.native_default, string_encoding=member.string_encoding)

def build(parent, bounded, catalog):
    if set(bounded.Field.__dataclass_fields__) != {'name', 'type', 'array', 'count', 'string_bound', 'native_string_bound', 'native_sequence_bound', 'native_default', 'native_scalar', 'string_encoding'}:
        raise ValueError('unreviewed IR field/domain extension requires a source successor')
    records, ordered, interfaces, pins = bounded.load_catalog(parent, catalog, False)
    if not ordered or any(r.profile != PROFILE for r in ordered):
        raise ValueError('actual reviewed control profile required')
    roots = {i.name: i for i in interfaces}
    messages = {r.name: dict(name=r.name, fields=[field(f) for f in r.members], record=r,
                            source_file=r.source['file'], implicit=bool(r.component)) for r in ordered}
    for interface in interfaces:
        if interface.kind != 'service':
            continue
        request, response = [interface.components[r].name for r in ('request', 'response')]
        messages[interface.name + '_Event'] = dict(name=interface.name + '_Event', fields=[
            dict(name='info', type='service_msgs/msg/ServiceEventInfo'),
            dict(name='request', type=request, array='sequence', count=1, native_sequence=1),
            dict(name='response', type=response, array='sequence', count=1, native_sequence=1)],
            record=None, source_file=interface.components['request'].source['file'], implicit=True)
    nodes, lookup = [], {}
    def visit(name, language, kind='message'):
        key = language, name
        if key in lookup:
            return lookup[key]
        index = len(nodes)
        lookup[key] = index
        if kind == 'service':
            interface = roots[name]
            fields = [dict(name=role + '_message', type=interface.components[role].name) for role in ('request', 'response')]
            fields.append(dict(name='event_message', type=name + '_Event'))
            item = dict(name=name, language=language, kind=kind, fields=fields, record=None,
                        source_file=interface.components['request'].source['file'], implicit=False)
        else:
            item = dict(messages[name], language=language, kind=kind)
        nodes.append(item)
        for member in item['fields']:
            if '/' in member['type']:
                visit(member['type'], language)
        return index
    selected = [('rcl_interfaces/msg/Log', 'cpp'), ('rcl_interfaces/msg/ParameterEvent', 'cpp')]
    selected += [(i.name, 'cpp')
                 for i in interfaces if i.kind == 'service' and i.registered]
    for name, language in selected:
        visit(name, language, 'service' if name in roots and roots[name].kind == 'service' else 'message')
    visit('rcl_interfaces/msg/ParameterType', 'cpp')
    for node in nodes:
        closure = []
        def descend(name):
            at = lookup[node['language'], name]
            if at in closure:
                return
            closure.append(at)
            for member in nodes[at]['fields']:
                if '/' in member['type']:
                    descend(member['type'])
        for member in node['fields']:
            if '/' in member['type']:
                descend(member['type'])
        node['references'] = closure
    identities = sorted({n['name'] for n in nodes})
    codecs, codec_map = [], {}
    def add(at):
        if at in codec_map:
            return codec_map[at]
        slot = len(codecs)
        codec_map[at] = slot
        node = nodes[at]
        result = dict(node=at, request=256, response=256)
        codecs.append(result)
        if node['kind'] == 'message':
            result['maximum'] = node['record'].maximum
            result['minimum'] = node['record'].minimum
            result['canonical'] = node['record'].codec_identity
        else:
            result['request'], result['response'] = [add(lookup[node['language'], f['type']]) for f in node['fields'][:2]]
            result['maximum'] = max(codecs[result[k]]['maximum'] for k in ('request', 'response'))
            result['minimum'] = 0
            result['canonical'] = roots[node['name']].codec_identity
        return slot
    for name, language in selected:
        add(lookup[language, name])
    if len(nodes) > 256 or len(identities) > 128 or len(codecs) > 64:
        raise ValueError('complete native closure exceeds fixed state bounds')
    return records, ordered, interfaces, pins, nodes, lookup, identities, codecs

def scalar(parent, member, language):
    name = member['type']
    if name == 'bool':
        return 'bool'
    if name == 'string':
        return '::rosidl_runtime_c__String' if language == 'c' else '::std::string'
    return parent.SCALARS[name][2] if name in parent.SCALARS else native(name, language)

def preamble(parent, node):
    return ['// Generated SOURCE over original objects; no native hashes.',
            '#include "registry.hpp"', '#include "conversion.hpp"', '#include "bounded_records.h"',
            '#include <' + header(parent, node['name'], node['language']) + '>',
            '#include <rosidl_runtime_c/string_functions.h>', '#include <rosidl_runtime_c/primitives_sequence_functions.h>',
            '#include <rosidl_typesupport_cpp/message_type_support.hpp>', '#include <rosidl_typesupport_cpp/service_type_support.hpp>',
            '#include <rosidl_typesupport_introspection_c/identifier.h>', '#include <rosidl_typesupport_introspection_cpp/identifier.hpp>',
            '#include <type_traits>', '#include <tuple>', '#include <utility>', '#include <new>',
            'namespace uwvm_default_control_gateway_detail {']

def typed_fields(parent, node):
    lines = []
    for member in node['fields']:
        raw = 'std::remove_cvref_t<decltype(std::declval<T&>().' + member['name'] + ')>'
        array = member.get('array', '')
        if array == 'array':
            if node['language'] == 'c':
                lines.append('static_assert(std::extent_v<' + raw + '> == ' + str(member['count']) + ');')
                element = 'std::remove_extent_t<' + raw + '>'
            else:
                lines.append('static_assert(std::tuple_size_v<' + raw + '> == ' + str(member['count']) + ');')
                element = 'typename ' + raw + '::value_type'
        elif array == 'sequence':
            if node['language'] == 'c':
                element = 'std::remove_cvref_t<decltype(std::declval<T&>().' + member['name'] + '.data[0])>'
            else:
                element = 'typename ' + raw + '::value_type'
                lines.append('static_assert(native_sequence_bound<' + raw + '>::value == ' + str(member.get('native_sequence', 0)) + ');')
        else:
            element = raw
        lines.append('static_assert(std::is_same_v<' + element + ',' + scalar(parent, member, node['language']) + '>,"original native field type differs");')
    return lines

def getters(node, ordinal):
    name, language, kind = node['name'], node['language'], node['kind']
    package, category, local = name.split('/')
    support = 'rosidl_service_type_support_t' if kind == 'service' else 'rosidl_message_type_support_t'
    if language == 'c':
        expression = ('ROSIDL_GET_SRV_TYPE_SUPPORT' if kind == 'service' else 'ROSIDL_GET_MSG_TYPE_SUPPORT') + '(' + ','.join((package, category, local)) + ')'
    else:
        expression = '::rosidl_typesupport_cpp::get_' + kind + '_type_support_handle<T>()'
    identifier = 'rosidl_typesupport_introspection_c__identifier' if language == 'c' else '::rosidl_typesupport_introspection_cpp::typesupport_identifier'
    return [support + ' const* generic_' + str(ordinal) + '(){return ' + expression + ';}',
            support + ' const* introspection_' + str(ordinal) + '(){auto p=generic_' + str(ordinal) + '();return p&&p->func?p->func(p,' + identifier + '):nullptr;}']

def layout_and_defaults(parent, bounded, node, ordinal):
    T, record = native(node['name'], node['language']), node['record']
    fields = node['fields']
    offsets = ','.join('offsetof(T,' + f['name'] + ')' for f in fields) if fields else 'offsetof(T,structure_needs_at_least_one_member)'
    lines = ['Layout layout_' + str(ordinal) + '() noexcept {static constexpr std::array<std::size_t,' + str(max(1, len(fields))) + '> offsets{' + offsets + '};return {sizeof(T),offsets};}',
             'std::uint32_t defaults_' + str(ordinal) + '() noexcept {try{T value{};']
    if node['language'] == 'c':
        lines += ['if(!' + T + '__init(&value))return 12;struct Guard{T*p;~Guard(){' + T + '__fini(p);}}guard{&value};']
    if record:
        for member in record.members:
            if member.native_default is not None:
                lines.append('if(value.' + member.name + '!=' + bounded.scalar_default_expression(member.type, member.native_default, 'c') + ')return 22;')
        if not record.members:
            lines.append('if(value.structure_needs_at_least_one_member)return 22;')
    lines += ['return 0;}catch(std::bad_alloc const&){return 12;}catch(...){return 22;}}']
    return lines

def converter(parent, bounded, node, ordinal, records):
    language, record = node['language'], node['record']
    T, cs = native(node['name'], language), record.c_symbol
    prefix = 'c_' if language == 'c' else 'cpp_'
    tag = prefix + record.symbol
    lines = preamble(parent, node) + ['namespace {using T=' + T + ';using Record=' + cs + ';}']
    for child in dict.fromkeys(f['type'] for f in node['fields'] if '/' in f['type']):
        r = records[2, child]
        lines += ['std::uint32_t copy_' + prefix + r.symbol + '(' + native(child, language) + ' const&,' + r.c_symbol + '&);',
                  'std::uint32_t fill_' + prefix + r.symbol + '(' + r.c_symbol + ' const&,' + native(child, language) + '&);']
    lines += typed_fields(parent, node)
    for constant in record.constants:
        name = T + '__' + constant['name'] if language == 'c' else T + '::' + constant['name']
        lines.append('static_assert(' + name + '==' + bounded.scalar_default_expression(constant['type'], constant['value'], 'c') + ',"original source constant differs");')
    lines += ['std::uint32_t copy_' + tag + '(T const&value,Record&output){Record next{};']
    if not record.members:
        lines.append('if(value.structure_needs_at_least_one_member)return 22;')
    for member in node['fields']:
        a, b = 'value.' + member['name'], 'next.' + member['name']
        arr = member.get('array', '')
        if arr:
            count = a + ('.size' if language == 'c' else '.size()') if arr == 'sequence' else str(member['count'])
            if language == 'c' and arr == 'sequence':
                lines.append('if(' + a + '.size>' + a + '.capacity||(' + a + '.size&&!' + a + '.data))return 22;')
            lines.append('if(' + count + '>' + str(member['count']) + 'u)return 90;')
            if arr == 'sequence':
                lines.append(b + '.size=static_cast<std::uint32_t>(' + count + ');')
            lines.append('for(std::size_t i{};i<' + count + ';++i){')
            a += '.data[i]' if language == 'c' and arr == 'sequence' else '[i]'
            b += '.data[i]' if arr == 'sequence' else '[i]'
        if member['type'] == 'string':
            lines.append('{auto e=copy_text(' + a + ',' + b + '.data,' + str(member['string']) + ',' + b + '.size);if(e)return e;}')
        elif member['type'] in parent.SCALARS:
            lines.append(b + '=' + a + ';')
        else:
            lines.append('{auto e=copy_' + prefix + records[2, member['type']].symbol + '(' + a + ',' + b + ');if(e)return e;}')
        if arr:
            lines.append('}')
    valid = ('std::size_t measured=SIZE_MAX;auto check=' + cs + '_measure(&next,&measured);if(check)return check;' if record.aggregate_wire_bytes else 'if(!' + cs + '_valid(&next))return 22;')
    lines += [valid + 'output=next;return 0;}', 'std::uint32_t fill_' + tag + '(Record const&value,T&next){']
    for member in node['fields']:
        a, b, arr = 'value.' + member['name'], 'next.' + member['name'], member.get('array', '')
        if arr == 'sequence':
            if language == 'c':
                if member['type'] == 'string':
                    seq = '::rosidl_runtime_c__String'
                elif member['type'] in parent.SCALARS:
                    alias = member.get('native_scalar') or member['type']
                    seq = '::rosidl_runtime_c__' + {'bool': 'boolean', 'float32': 'float', 'float64': 'double', 'byte': 'octet', 'char': 'uint8'}.get(alias, alias)
                else:
                    seq = native(member['type'], language)
                lines += [seq + '__Sequence__fini(&' + b + ');' + b + '={};', 'if(!' + seq + '__Sequence__init(&' + b + ',' + a + '.size))return 12;']
            else:
                lines.append(b + '.resize(' + a + '.size);')
        if arr:
            count = a + '.size' if arr == 'sequence' else str(member['count'])
            lines.append('for(std::size_t i{};i<' + count + ';++i){')
            a += '.data[i]' if arr == 'sequence' else '[i]'
            b += '.data[i]' if language == 'c' and arr == 'sequence' else '[i]'
        if member['type'] == 'string':
            if language == 'c':
                lines.append('if(!rosidl_runtime_c__String__assignn(&' + b + ',reinterpret_cast<char const*>(' + a + '.data),' + a + '.size))return 12;')
            else:
                lines.append(b + '.assign(reinterpret_cast<char const*>(' + a + '.data),' + a + '.size);')
        elif member['type'] in parent.SCALARS:
            lines.append(b + '=' + a + ';')
        else:
            lines.append('{auto e=fill_' + prefix + records[2, member['type']].symbol + '(' + a + ',' + b + ');if(e)return e;}')
        if arr:
            lines.append('}')
    lines += ['return 0;}', 'std::uint32_t encode_' + str(ordinal) + '(void const*value,void*bytes,std::size_t capacity,std::size_t*length)noexcept {',
              'if(!value||!length||(!bytes&&capacity))return 22;try{Record next{};auto e=copy_' + tag + '(*static_cast<T const*>(value),next);if(e)return e;return ' + cs + '_encode(&next,bytes,capacity,length);}catch(std::bad_alloc const&){return 12;}catch(...){return 22;}}',
              'std::uint32_t decode_' + str(ordinal) + '(void const*bytes,std::size_t size,void*value)noexcept {if(!value||(size&&!bytes))return 22;if(size>' + str(record.maximum) + 'u)return 90;',
              'try{Record record{};auto e=' + cs + '_decode(bytes,size,&record);if(e)return e;T next{};']
    if language == 'c':
        lines += ['if(!' + T + '__init(&next))return 12;struct Guard{T*p;~Guard(){' + T + '__fini(p);}}guard{&next};',
                  'if((e=fill_' + tag + '(record,next)))return e;static_assert(std::is_trivially_copyable_v<T>);auto old=*static_cast<T*>(value);*static_cast<T*>(value)=next;next=old;return 0;}']
    else:
        lines += ['if((e=fill_' + tag + '(record,next)))return e;static_assert(std::is_nothrow_move_assignable_v<T>,"atomic whole native commit required");*static_cast<T*>(value)=std::move(next);return 0;}']
    lines += ['catch(std::bad_alloc const&){return 12;}catch(...){return 22;}}']
    lines += ['std::uint32_t body_' + str(ordinal) + '(void const*bytes,std::size_t size)noexcept{if(!bytes&&size)return 22;if(size>' + str(record.maximum) + 'u)return 90;try{Record value{};return ' + cs + '_decode(bytes,size,&value);}catch(std::bad_alloc const&){return 12;}catch(...){return 22;}}']
    lines += layout_and_defaults(parent, bounded, node, ordinal) + getters(node, ordinal) + ['}']
    return '\n'.join(lines) + '\n'

def metadata_only(parent, bounded, node, ordinal):
    language, kind = node['language'], node['kind']
    lines = preamble(parent, node)
    if kind == 'message' or language == 'cpp':
        lines.append('namespace {using T=' + native(node['name'], language) + ';}')
    if kind == 'message':
        lines += typed_fields(parent, node) + layout_and_defaults(parent, bounded, node, ordinal)
    elif language == 'cpp':
        for role in ('Request', 'Response', 'Event'):
            lines.append('static_assert(std::is_same_v<typename T::' + role + ',' + native(node['name'] + '_' + role, language) + '>);')
    lines += getters(node, ordinal) + ['}']
    return '\n'.join(lines) + '\n'

def table(catalog, nodes, lookup, identities, codecs):
    lines = ['// Complete immutable selected graph. Actual native hashes are external.', '#include "registry.hpp"',
             '#include <rosidl_typesupport_introspection_c/field_types.h>', '#include <rosidl_typesupport_introspection_cpp/field_types.hpp>',
             '#include <rosidl_runtime_c/type_description/field_type__struct.h>', 'namespace uwvm_default_control_gateway_detail {']
    for i, node in enumerate(nodes):
        support = 'rosidl_service_type_support_t' if node['kind'] == 'service' else 'rosidl_message_type_support_t'
        lines += [support + ' const* generic_' + str(i) + '();', support + ' const* introspection_' + str(i) + '();']
        if node['kind'] == 'message':
            lines += ['Layout layout_' + str(i) + '()noexcept;', 'std::uint32_t defaults_' + str(i) + '()noexcept;']
            if node['record'] is not None:
                lines += ['std::uint32_t encode_' + str(i) + '(void const*,void*,std::size_t,std::size_t*)noexcept;', 'std::uint32_t decode_' + str(i) + '(void const*,std::size_t,void*)noexcept;', 'std::uint32_t body_' + str(i) + '(void const*,std::size_t)noexcept;']
        lines.append('static constexpr std::array<std::size_t,' + str(len(node['references'])) + '> refs_' + str(i) + '{' + ','.join(map(str, node['references'])) + '};')
        lines.append('static constexpr Field fields_' + str(i) + '[]={')
        for member in node['fields'] or [dict(name='structure_needs_at_least_one_member', type='uint8')]:
            typ = member['type']; alias = member.get('native_scalar') or typ
            bound = member.get('native_string', 0); arr = member.get('array', '')
            capacity = member.get('count', 0) if arr == 'array' else member.get('native_sequence', 0) if arr == 'sequence' else 0
            base = {'bool': 'BOOLEAN', 'int8': 'INT8', 'uint8': 'UINT8', 'int16': 'INT16', 'uint16': 'UINT16', 'int32': 'INT32', 'uint32': 'UINT32', 'int64': 'INT64', 'uint64': 'UINT64', 'float32': 'FLOAT', 'float64': 'DOUBLE', 'byte': 'BYTE', 'char': 'UINT8', 'string': 'BOUNDED_STRING' if bound else 'STRING'}.get(alias, 'NESTED_TYPE')
            td = 'rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_' + base
            if arr:
                td += '_ARRAY' if arr == 'array' else '_BOUNDED_SEQUENCE' if capacity else '_UNBOUNDED_SEQUENCE'
            it = {'bool': 'BOOLEAN', 'int8': 'INT8', 'uint8': 'UINT8', 'int16': 'INT16', 'uint16': 'UINT16', 'int32': 'INT32', 'uint32': 'UINT32', 'int64': 'INT64', 'uint64': 'UINT64', 'float32': 'FLOAT', 'float64': 'DOUBLE', 'byte': 'OCTET', 'char': 'UINT8', 'string': 'STRING'}.get(alias, 'MESSAGE')
            it = ('rosidl_typesupport_introspection_c__ROS_TYPE_' if node['language'] == 'c' else '::rosidl_typesupport_introspection_cpp::ROS_TYPE_') + it
            values = [literal(member['name']), literal(typ if '/' in typ else ''), literal(member.get('default') or ''), td, it, str(capacity), str(bound), str(bool(arr)).lower(), str(arr == 'sequence' and bool(capacity)).lower(), str(node['language'] == 'cpp' and typ == 'bool' and arr == 'sequence').lower()]
            lines.append('{' + ','.join(values) + '},')
        lines.append('};')
    lines += ['static constexpr std::array<std::string_view,' + str(len(identities)) + '> identities{' + ','.join(literal(n) for n in identities) + '};', 'static Node const nodes[]={']
    for i, node in enumerate(nodes):
        language = node['language']; package, category, local = node['name'].split('/')
        source = read(catalog.parent / node['source_file'])
        encoding = 'implicit' if node['implicit'] else Path(node['source_file']).suffix[1:]
        raw_text = b'' if node['implicit'] else native_source_bytes(source)
        service = node['kind'] == 'service'
        components = [lookup[language, f['type']] for f in node['fields']] if service else [256] * 3
        callbacks = ('nullptr,nullptr,nullptr,nullptr,&generic_' + str(i) + ',&introspection_' + str(i)) if service else ('&defaults_' + str(i) + ',&layout_' + str(i) + ',&generic_' + str(i) + ',&introspection_' + str(i) + ',nullptr,nullptr')
        values = [literal(node['name']), literal(package + ('__' if language == 'c' else '::') + category), literal(local), literal(encoding), binary_view(raw_text), literal(hashlib.sha256(source).hexdigest())]
        lines.append('{Language::' + language + ',Kind::' + node['kind'] + ',' + str(identities.index(node['name'])) + ',' + ','.join(values) + ',fields_' + str(i) + ',refs_' + str(i) + ',' + callbacks + ',{' + ','.join(map(str, components)) + '}},')
    lines += ['};', 'static constexpr CodecSpec codecs[]={']
    for codec in codecs:
        ordinal = codec['node']
        callbacks = 'nullptr,nullptr' if nodes[ordinal]['kind'] == 'service' else '&encode_' + str(ordinal) + ',&decode_' + str(ordinal)
        lines.append('{' + ','.join(str(codec[k]) for k in ('node', 'maximum', 'request', 'response', 'minimum')) + ',' + literal(codec['canonical']) + ',' + ('nullptr' if nodes[ordinal]['kind'] == 'service' else '&body_' + str(ordinal)) + ',' + callbacks + '},')
    lines += ['};', 'Graph const& generated_graph()noexcept {static Graph const g{identities,nodes,codecs};return g;}', '}']
    return '\n'.join(lines) + '\n'

def endpoint_products(parent, nodes, codecs):
    selected = [n for n in nodes if n['name'] in ('rcl_interfaces/msg/Log','rcl_interfaces/msg/ParameterEvent') or n['kind']=='service']
    selected.sort(key=lambda n:n['name'])
    if len(selected)!=9 or any(n['language']!='cpp' for n in selected):
        raise ValueError('exact complete nine-root all-C++ family required')
    result = {}
    service_args='std::shared_ptr<profile const> const&p,std::shared_ptr<rclcpp::Node> const&node,std::shared_ptr<uwvm_rmw_cpp::Registry const> const&old,ros::capability_spec const&cap,wire::binding const&o,std::uint64_t id,std::size_t history,std::shared_ptr<native_rcl_service::report> const&report,std::shared_ptr<native_rcl_service_registry::endpoint>&out,wire::packet&packet'
    pub_args='std::shared_ptr<profile const> const&p,std::shared_ptr<rclcpp::Node> const&node,ros::capability_spec const&cap,wire::binding const&o,std::uint64_t id,std::shared_ptr<native_rmw_publisher::endpoint>&out,wire::packet&packet'
    sub_args=pub_args.replace('native_rmw_publisher','native_rmw_subscription')
    pre=['// Light addresses/routing only; no typed Native messages or old full-table templates.','#include "default_control_gateway.h"','#include "profile_internal.hpp"','#include "../common/config.h"','namespace uwvm2::ros_gateway::default_control_gateway {','namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;']
    dispatch=list(pre)
    for i,n in enumerate(selected):
        service=n['kind']=='service'
        args=service_args if service else pub_args
        dispatch.append('std::uint32_t '+('service_' if service else 'publisher_')+str(i)+'('+args+')noexcept;')
        if not service:
            dispatch.append('std::uint32_t subscription_'+str(i)+'('+sub_args+')noexcept;')
        lines=['// One actual native root per TU; no old whole-table template.','#include <'+header(parent,n['name'],'cpp')+'>']
        lines+=['#include "service_endpoint.hpp"'] if service else ['#include "publisher_endpoint.hpp"','#include "subscription_endpoint.hpp"']
        lines+=['namespace uwvm2::ros_gateway::default_control_gateway {','namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;']+['namespace {using Native='+native(n['name'],'cpp')+';}']
        if service:
            lines+=['std::uint32_t service_'+str(i)+'('+args+')noexcept{try{',
                    'auto proof=std::make_shared<detail::ServiceProof<Native> const>(p,'+literal(n['name'])+');',
                    'if(cap.operation=="service.call")return detail::client<Native>::create(node,old,proof,cap,o,id,history,report,out,packet);',
                    'if(cap.operation=="service.advertise")return detail::server<Native>::create(node,old,proof,cap,o,id,history,report,out,packet);return 13;',
                    '}catch(std::bad_alloc const&){return 12;}catch(...){return 5;}}']
        else:
            for prefix,arguments,namespace in [('publisher_',pub_args,'publisher_detail'),('subscription_',sub_args,'subscription_detail')]:
                lines+=['std::uint32_t '+prefix+str(i)+'('+arguments+')noexcept{try{',
                        'auto proof=std::make_shared<detail::TopicProof<Native> const>(p,'+literal(n['name'])+');',
                        'return '+namespace+'::typed_endpoint<Native>::create(proof,node,cap,o,id,out,packet);',
                        '}catch(std::bad_alloc const&){return 12;}catch(...){return 5;}}']
        lines+=['}'];result['endpoints/root_'+str(i)+'.cpp']='\n'.join(lines)+'\n'
    for name,args,call in [('create_service',service_args,'node,old,cap,o,id,history,report,out,packet'),('create_publisher',pub_args,'node,cap,o,id,out,packet'),('create_subscription',sub_args,'node,cap,o,id,out,packet')]:
        dispatch+=['std::uint32_t '+name+'('+args+')noexcept{','if(!p)return 95;uwvm_rmw_codec const*root{};auto selected=uwvm_default_control_gateway_detail::select(*p,cap,root);if(selected)return selected;']
        for i,n in enumerate(selected):
            if (name=='create_service') != (n['kind']=='service'):
                continue
            dispatch.append('if(cap.type=='+literal(n['name'])+')return '+name.replace('create_','')+'_'+str(i)+'(p,'+call+');')
        dispatch+=['return 95;}']
    dispatch+=['}'];result['endpoints/dispatch.cpp']='\n'.join(dispatch)+'\n'
    return result

def run(source_root, catalog, canonical, output, parent_sha, bounded_sha):
    if not all(p.is_absolute() for p in (source_root, catalog, canonical, output)):
        raise ValueError('explicit absolute paths required')
    parent_path, bounded_path = source_root / 'sdk/tools/custom_types.py', source_root / 'sdk/custom-types/bounded_profile.py'
    parent_raw, bounded_raw = read(parent_path), read(bounded_path)
    if hashlib.sha256(parent_raw).hexdigest() != parent_sha or hashlib.sha256(bounded_raw).hexdigest() != bounded_sha:
        raise ValueError('explicit independently reviewed generator pins differ')
    def module(name, path, raw):
        result = types.ModuleType(name); result.__file__ = str(path); sys.modules[name] = result
        exec(compile(raw, str(path), 'exec'), result.__dict__)
        return result
    parent, bounded = module('_uwvm_control_parent', parent_path, parent_raw), module('_uwvm_control_bounded', bounded_path, bounded_raw)
    records, ordered, interfaces, pins, nodes, lookup, identities, codecs = build(parent, bounded, catalog)
    generation_raw = read(canonical / 'generation.json'); generation = strict(generation_raw)
    if generation.get('format') != PROFILE or generation.get('native_registry_emitted') is not False or generation.get('implemented_record_outputs') != ['c', 'cpp'] or generation.get('source_inputs') != pins:
        raise ValueError('actual same-source canonical revision required')
    if generation.get('generator_sha256') != parent_sha or generation.get('bounded_generator_sha256') != bounded_sha:
        raise ValueError('canonical generator source differs')
    canonical_pins = generation['outputs']
    if not {'bounded_records.h', 'control-schema.json'} <= set(canonical_pins):
        raise ValueError('complete actual canonical output required')
    def reopen_canonical():
        for name, pin in canonical_pins.items():
            if not isinstance(name, str) or '/' in name or '\\' in name or name in ('.', '..'):
                raise ValueError('canonical name must be a basename')
            raw = read(canonical / name)
            if len(raw) != pin['bytes'] or hashlib.sha256(raw).hexdigest() != pin['sha256']:
                raise ValueError('canonical output drift')
    reopen_canonical()
    products = {'nodes/node_' + str(i) + '.cpp': converter(parent, bounded, node, i, records) if node['kind'] == 'message' and node['record'] is not None else metadata_only(parent, bounded, node, i) for i, node in enumerate(nodes)}
    products['nodes/graph.cpp'] = table(catalog, nodes, lookup, identities, codecs)
    products.update(endpoint_products(parent, nodes, codecs))
    selected_roots = [dict(type=n['name'], kind=n['kind'], adapter_codec_identity=c['canonical'], minimum_wire_bytes=c['minimum'], maximum_wire_bytes=c['maximum'], request_maximum_wire_bytes=(codecs[c['request']]['maximum'] if n['kind']=='service' else None), response_maximum_wire_bytes=(codecs[c['response']]['maximum'] if n['kind']=='service' else None)) for c in codecs for n in [nodes[c['node']]] if n['name'] in ('rcl_interfaces/msg/Log','rcl_interfaces/msg/ParameterEvent') or n['kind']=='service']
    products['gateway-control-selection.json'] = json.dumps(dict(format=PROFILE, status='CANONICAL_COMPILED_SELECTION_NOT_NATIVE_ID_OR_PERMISSION', roots=selected_roots),indent=2)+'\n'
    products['native-identities.required.json'] = json.dumps(dict(status='REQUIRED_INDEPENDENT_ACTUAL_EXPORT', native_types=identities, native_hash_values_generated=0, C_and_CPP='Gateway uses only separately proved genuine C++ objects; Guest C adapters remain independent'), indent=2) + '\n'
    products['generated-sources.cmake'] = 'set(UWVM_DEFAULT_CONTROL_GATEWAY_GENERATED_SOURCES\n' + ''.join('  "${CMAKE_CURRENT_LIST_DIR}/' + name + '"\n' for name in products if name.endswith('.cpp')) + ')\n'
    proof = dict(format_version=1, status='GATEWAY_CPP_ADAPTER_SOURCE_ONLY_NOT_QUALIFIED', format=PROFILE, source_inputs=pins, canonical_outputs=canonical_pins,
                 canonical_generation=dict(bytes=len(generation_raw), sha256=hashlib.sha256(generation_raw).hexdigest()),
                 generator_sha256=hashlib.sha256(read(Path(__file__))).hexdigest(), parent_generator_sha256=parent_sha, bounded_generator_sha256=bounded_sha,
                 native_nodes=len(nodes), native_identity_names=identities, codecs=len(codecs), native_hash_values_generated=0, native_objects_created=0,
                 outputs={name: dict(bytes=len(body.encode()), sha256=hashlib.sha256(body.encode()).hexdigest()) for name, body in products.items()})
    reopen_canonical()
    for pin in pins['definitions']:
        raw = read(catalog.parent / pin['file'])
        if len(raw) != pin['bytes'] or hashlib.sha256(raw).hexdigest() != pin['sha256']:
            raise ValueError('original definition terminal drift')
    raw = read(catalog)
    if len(raw) != pins['catalog']['bytes'] or hashlib.sha256(raw).hexdigest() != pins['catalog']['sha256'] or read(parent_path) != parent_raw or read(bounded_path) != bounded_raw or read(canonical / 'generation.json') != generation_raw:
        raise ValueError('input terminal drift')
    if output.exists():
        raise ValueError('output must be a fresh source revision')
    output.mkdir(parents=True)
    for name, body in products.items():
        path = output / name; path.parent.mkdir(parents=True, exist_ok=True); path.write_text(body, encoding='utf-8')
    (output / 'generation.gateway-control.json').write_text(json.dumps(proof, indent=2) + '\n', encoding='utf-8')
    return proof

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source-root', 'catalog', 'canonical', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--parent-sha256', required=True); parser.add_argument('--bounded-sha256', required=True)
    args = parser.parse_args()
    try:
        print(json.dumps(run(args.source_root, args.catalog, args.canonical, args.out, args.parent_sha256, args.bounded_sha256), indent=2))
    except (OSError, ValueError, TypeError, KeyError, RecursionError) as error:
        print(str(error), file=sys.stderr); return 2
    return 0

if __name__ == '__main__':
    raise SystemExit(main())
