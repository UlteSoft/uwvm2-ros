#!/usr/bin/env python3
"""Prepare an explicit complete Message native-source proof; execute no ROS code.

Pair an original unbound complete catalog with independently captured installed
IDL facts. Outputs are native C++ source only. Actual selected-SDK compilation,
source/MD/link/process ownership, metadata export and Gateway qualification are
separate mandatory stages. No registry, identity or permission is invented here.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import struct
import types

FORMAT = 'uwvm-ros2-installed-message-source-proof-v1'


def fact(raw):
    return dict(bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def cpp_text(text):
    """An exact counted UTF-8 C++ literal, independent of source encoding."""
    return '"' + ''.join('\\%03o' % b for b in text.encode('utf-8')) + '"'


def native_type(kind):
    if '/' in kind:
        return '::' + kind.replace('/', '::')
    if kind == 'string':
        return 'std::string'
    if kind in ('byte', 'char'):
        return 'std::uint8_t' if kind == 'byte' else 'unsigned char'
    return dict(bool='bool', float32='float', float64='double').get(kind, 'std::'+kind+'_t')


def literal(kind, text):
    if kind == 'bool':
        return 'true' if text == 'True' else 'false'
    if kind in ('float32', 'float64'):
        value = float(text)
        if kind == 'float32':
            value = struct.unpack('<f', struct.pack('<f', value))[0]
        if not math.isfinite(value):
            raise ValueError('finite representable typed native default required')
        return value.hex() + ('f' if kind == 'float32' else '')
    value = int(text)
    if value < 0:
        return '(-INT64_C(' + str(-value-1) + ')-1)'
    return 'UINT64_C(' + str(value) + ')'


def make(api, catalog, installed_raw):
    """Return one coherent default-denied source domain in memory."""
    need = api.env['need']; canonical = api.env['canonical']
    records, ordered, interfaces, pins = api.load_catalog(catalog, False)
    need(all(r.identity is None for r in ordered) and
         all(i.version == 2 and i.kind == 'message' for i in interfaces),
         'explicit unbound ROS2 Message-only complete catalog required')
    facts = api.env['strict_json'](installed_raw)
    own = api.env['read_source'](Path(__file__).resolve())
    domain = dict(profile=api.env['PROFILE'], catalog_inputs=pins, source_generators=api.sources,
        proof_generator=fact(own), records=api.native_message_source_facts(ordered), installed_idl_facts=facts)
    digest = hashlib.sha256(canonical(domain)).hexdigest()
    proof = dict(format=FORMAT, scope='Generated installed-IDL/typed native source proof only; no execution, native identity, registry or authority.',
        domain=domain, source_proof_sha256=digest, outputs={}, default_authority=[])
    api.verify_native_message_source_proof(records, ordered, pins, canonical(proof), require_outputs=False)
    ns = 'uwvm_installed_source_' + digest
    lines = ['/** Complete actual-IDL/native C++ proof. No Node, endpoint or grant. */',
        '#pragma once', '#include "gateways/ros2/compat/installed_source.h"',
        '#include <cstdint>', '#include <cstddef>', '#include <string>', '#include <string_view>', '#include <type_traits>']
    lines += ['#include <'+r.name.rsplit('/', 1)[0]+'/'+api.parent.ros2_header(r.name.rsplit('/', 1)[1])+'.hpp>' for r in ordered]
    lines += ['namespace '+ns+' {', 'namespace source=::uwvm2::ros_gateway::ros2_compat::installed_source;',
        'namespace ipc=::uwvm2::imported::ros::ipc;']
    names = {r.name:'r'+str(index) for index, r in enumerate(ordered)}
    idls = {row['type']:row['installed_idl'] for row in facts['records']}
    for record in ordered:
        symbol = names[record.name]; typ = native_type(record.name)
        fields = domain['records'][record.name]['fields']
        lines += ['using '+symbol+'='+typ+';', 'inline bool value_'+symbol+'('+symbol+' const& value) {']
        for index, field in enumerate(fields):
            name = field['name']; kind = field['type']; access = 'value.'+name
            raw = 'field_'+str(index)
            shape = 1 if field['is_array'] else 2 if field['is_sequence'] else 0
            bound = field['array_size'] if shape == 1 else field['sequence_bound'] if shape == 2 else 0
            lines += ['using '+raw+'=std::remove_cv_t<std::remove_reference_t<decltype('+access+')>>;',
                'static_assert(source::field_type<'+raw+','+native_type(kind)+','+str(shape)+','+str(bound)+'>(),"actual native field type/shape differs");']
            if shape == 2:
                lines.append('if(!'+access+'.empty())return false;')
                continue
            if shape == 1:
                lines += ['for(auto const& item:'+access+'){']; access = 'item'
            if '/' in kind:
                lines.append('if(!value_'+names[kind]+'('+access+'))return false;')
            elif kind == 'string':
                text = field['native_default']
                lines.append('if(std::string_view{'+access+'}!=std::string_view{'+cpp_text(text)+','+str(len(text.encode('utf-8')))+'})return false;')
            else:
                expected = field['native_default'] if field['has_declared_default'] else 'False' if kind == 'bool' else '0'
                lines.append('if(!source::same_value('+access+','+native_type(kind)+'('+literal(kind, expected)+')))return false;')
            if shape == 1:
                lines.append('}')
        for constant in domain['records'][record.name]['constants']:
            kind = constant['type']; access = typ+'::'+constant['name']
            if kind == 'string':
                text = constant['value']
                lines.append('if(std::string_view{'+access+'}!=std::string_view{'+cpp_text(text)+','+str(len(text.encode('utf-8')))+'})return false;')
            else:
                lines += ['static_assert(std::is_same_v<std::remove_cv_t<decltype('+access+')>,'+native_type(kind)+'>,"actual native constant type differs");',
                    'if(!source::same_value('+access+','+native_type(kind)+'('+literal(kind, constant['value'])+')))return false;']
        lines += ['return true;}', 'inline bool typed_'+symbol+'(){return value_'+symbol+'('+symbol+'{});}']
        field_rows = ['{'+','.join(json.dumps(x) if isinstance(x, str) else str(x).lower() if isinstance(x, bool) else str(x)
            for x in row)+'}' for row in api._fields(record)]
        lines += ['inline constexpr ipc::custom_native_field fields_'+symbol+'[]={'+','.join(field_rows)+'};',
            'inline constexpr source::field_layout layout_'+symbol+'[]={'+','.join(
                '{offsetof('+symbol+','+f['name']+'),sizeof(std::declval<'+symbol+'>().'+f['name']+')}' for f in fields)+'};']
    lines += ['inline source::witness const witnesses[]={']
    for record in ordered:
        symbol = names[record.name]
        lines.append('{'+json.dumps(record.name)+','+json.dumps(idls[record.name]['sha256'])+
            ',&::uwvm2::ros_gateway::ros2_compat::message_members_for<'+symbol+'>,&typed_'+symbol+',sizeof('+symbol+'),layout_'+symbol+','+str(len(api._fields(record)))+'},')
    lines += ['};']
    for record in ordered:
        symbol = names[record.name]
        payload = next(i for i in interfaces if i.name == record.name)
        graph = api.declarations(records, payload)
        lines += ['inline constexpr ipc::custom_native_record graph_'+symbol+'[]={'+','.join(
            '{'+json.dumps(name)+',fields_'+names[name]+','+str(len(fields))+'}' for name, fields in graph)+'};']
    lines += ['}', 'namespace uwvm2::ros_gateway::ros2_compat::installed_source {']
    for record in ordered:
        symbol = names[record.name]
        lines += ['template<>struct traits<'+native_type(record.name)+'> {', 'static constexpr bool enabled=true;',
            'inline static constexpr std::string_view type='+json.dumps(record.name)+',source_proof_sha256="'+digest+'";',
            'static constexpr ipc::custom_native_schema schema() noexcept{return {'+ns+'::graph_'+symbol+',sizeof('+ns+'::graph_'+symbol+')/sizeof(ipc::custom_native_record)};}',
            'static bool prove(){return records(schema(),'+ns+'::witnesses,sizeof('+ns+'::witnesses)/sizeof(witness));}', '};']
    lines += ['}']
    header = '\n'.join(lines)+'\n'
    exporter = ['/** Genuine pre-RIHS Message export; original owner must admit every input/process. */',
        '#include "native_source.hpp"', '#include "tests/ros_custom_types/bounded-v2/native_protocol.h"',
        '#include <csignal>', '#include <cstring>', '#include <stdexcept>',
        'int main(int argc,char** argv){try{', 'if(argc!=2||std::strcmp(argv[1],"--mapped-stop"))return 64;',
        'namespace compat=::uwvm2::ros_gateway::ros2_compat;', 'compat::rihs_json output;']
    prefix = dict(format='uwvm-ros-industrial-native-pre-rihs-source-v1', ros_version=2,
        source_profile=api.env['PROFILE'], constant_schema_sha256=hashlib.sha256(canonical(api.constant_facts(ordered))).hexdigest(),
        source_proof_sha256=digest)
    prefix_json = json.dumps(prefix, separators=(',', ':'))[:-1]+',"types":{'
    exporter.append('output.add('+cpp_text(prefix_json)+');')
    for index, record in enumerate(ordered):
        interface = next(i for i in interfaces if i.name == record.name)
        metadata = api._modern_graph(api.declarations(records, interface), record.name)
        metadata['references'] = [metadata['references'][name] for name in sorted(metadata['references'])]
        metadata.update(identity_provenance='PRE_RIHS_INSTALLED_IDL_AND_TYPED_NATIVE_INTROSPECTION_DERIVED_RIHS01',
            native_baked_hash_available=False, source_proof_sha256=digest)
        tail = json.dumps(metadata, separators=(',', ':'), ensure_ascii=True)[1:]
        exporter += ['{auto identity=compat::installed_source::identity<'+native_type(record.name)+'>();',
            'if(identity.empty())throw std::runtime_error{"complete installed native proof refused"};',
            'compat::rihs_json value;value.add("{\\\"native_identity\\\":");value.name(identity);value.add(",");',
            'value.add('+cpp_text(tail)+');if(!value.valid())throw std::length_error{"native metadata bound"};',
            'output.add('+cpp_text((',' if index else '')+json.dumps(record.name)+':{"official":')+');',
            'output.add(value.body());output.add(",\\\"components\\\":{\\\"message\\\":");output.add(value.body());output.add("}}");}']
    exporter += ['output.add("}}\\n");if(!output.valid())throw std::length_error{"whole native export bound"};',
        'if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};',
        'bounded_native_io::gate(\'I\');bounded_native_io::emit(STDOUT_FILENO,output.body());return 0;',
        '}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native installed-source metadata refused\\n");return 1;}}']
    products = {'native_source.hpp':header, 'native_source_identity.cc':'\n'.join(exporter)+'\n',
        'native_source_provider.hpp':'/** Explicit trusted native registry wrapper; creates no authority. */\n#pragma once\n#include "native_source.hpp"\n#include "native_ros2.hpp"\n',
        'native_source_types.h':'/** Explicit complete actual Message test inventory; no registry or grant. */\n#pragma once\n#define UWVM_ROS_INSTALLED_SOURCE_MESSAGES(X) \\\n'+
            ' \\\n'.join('X('+native_type(r.name)+','+json.dumps(r.name)+')' for r in ordered)+'\n'}
    proof['outputs'] = {name:fact(text.encode()) for name, text in products.items()}
    products['native_source_proof.json'] = canonical(proof).decode()
    need(all(len(text.encode()) <= 16*1024*1024 for text in products.values()), 'bounded complete native proof source required')
    return products, proof


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('source-root', 'catalog', 'installed-idl', 'out'):
        p.add_argument('--'+name, required=True, type=Path)
    for name in ('parent', 'bounded', 'adapter'):
        p.add_argument('--'+name+'-sha256', required=True)
    args = p.parse_args(); root = args.source_root
    # Fixed source entry only. No plugin path or ambient project import.
    source = root/'sdk/robots/industrial/vendor_profile.py'
    raw = source.read_bytes()
    if hashlib.sha256(raw).hexdigest() != args.adapter_sha256:
        raise ValueError('explicit reviewed actual adapter SHA differs')
    module = types.ModuleType('_installed_message_source_adapter'); module.__file__ = str(source)
    exec(compile(raw, str(source), 'exec', dont_inherit=True), module.__dict__)
    parent = root/'sdk/tools/custom_types.py'; bounded = root/'sdk/custom-types/bounded_profile.py'
    api = module.API(module.read_source(parent), module.read_source(bounded),
        parent_path=parent, bounded_path=bounded, parent_sha256=args.parent_sha256,
        bounded_sha256=args.bounded_sha256, adapter_raw=raw, adapter_sha256=args.adapter_sha256,
        templates={f'interface_templates/bounded_interfaces.{ext}.in':module.read_source(bounded.parent/f'interface_templates/bounded_interfaces.{ext}.in')
                   for ext in ('rs','mjs','go','ts')})
    installed = module.read_source(args.installed_idl)
    products, proof = make(api, args.catalog, installed)
    module.need(args.out.is_absolute() and args.out.parent.resolve(strict=True) == args.out.parent and
                not args.out.exists() and not args.out.is_symlink(), 'fresh physical caller-owned output required')
    module.need(module.read_source(source) == raw and module.read_source(args.installed_idl) == installed,
                'actual source/input drift before publication')
    args.out.mkdir(mode=0o700)
    for name, text in products.items():
        fd = os.open(args.out/name, os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC, 0o600)
        with os.fdopen(fd, 'wb') as stream:
            stream.write(text.encode('utf-8'))


if __name__ == '__main__':
    main()
