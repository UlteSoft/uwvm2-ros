#!/usr/bin/env python3
"""Generate explicit complete legacy ROS2 protocol proof SOURCE only.

The original catalog, actual installed IDL export and exact source generators
form one domain. A separate admitted native compiler/exporter owner must prove
all real typed fields, defaults, constants and Service/Action associations.
Neither this generator nor a matching public digest establishes execution.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import types

FORMAT = 'uwvm-ros2-installed-protocol-source-proof-v1'
NATIVE_FORMAT = 'uwvm-ros-industrial-native-pre-rihs-protocol-v1'
PROVENANCE = 'PRE_RIHS_INSTALLED_IDL_AND_TYPED_NATIVE_PROTOCOL_DERIVED_RIHS01'
TYPE = r'[a-z][a-z0-9_]{0,62}/(?:msg|srv|action)/[A-Z][A-Za-z0-9_]*'
FIELDS = ('name','type','array_size','is_array','string_bound','is_sequence',
          'sequence_bound','native_default','has_declared_default')


def fact(raw):
    return dict(bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def helpers(api):
    read = api.env['read_source']; base = Path(__file__).parent
    names = ('source_proof.py', 'idl_export.py', 'protocol_idl_export.py')
    raw = {name:read(base/name) for name in names}
    module = types.ModuleType('_legacy_protocol_source_literals')
    module.__file__ = str(base/'source_proof.py')
    exec(compile(raw['source_proof.py'], module.__file__, 'exec', dont_inherit=True), module.__dict__)
    return module, {name:fact(body) for name,body in raw.items()}


def source_interfaces(interfaces):
    return {i.name:dict(kind=i.kind, components={role:r.name for role,r in i.components.items()})
            for i in interfaces}


def snapshot(api, facts):
    """Validate typed facts and derive only their exact reachable legacy graphs."""
    need = api.env['need']; canonical = api.env['canonical']
    need(type(facts) is dict and set(facts) == {'format','scope','request','parser_sources',
         'exporter_sources','roots','interfaces','records','default_authority'} and
         facts['format'] == 'uwvm-ros2-installed-protocol-idl-source-v1' and facts['default_authority'] == [],
         'closed complete installed legacy protocol facts required')
    def pin(value, *, path=False):
        need(type(value) is dict and set(value) == ({'path','bytes','sha256'} if path else {'bytes','sha256'}) and
             type(value['bytes']) is int and 0 <= value['bytes'] <= 16*1024*1024 and
             type(value['sha256']) is str and re.fullmatch('[0-9a-f]{64}',value['sha256']), 'exact bounded source fact required')
        if path:
            need(type(value['path']) is str and Path(value['path']).is_absolute(), 'absolute original installed IDL required')
    pin(facts['request'])
    need(type(facts['parser_sources']) is dict and set(facts['parser_sources']) ==
         {'__init__.py','definition.py','grammar.lark','parser.py'}, 'whole actual SDK parser sources required')
    for value in facts['parser_sources'].values():pin(value)
    need(type(facts['records']) is list and 1 <= len(facts['records']) <= 256 and
         type(facts['interfaces']) is list and 1 <= len(facts['interfaces']) <= 256 and
         type(facts['roots']) is dict and 1 <= len(facts['roots']) <= 128, 'bounded complete native inventory required')
    messages = {}; interfaces = {}; declarations = {}
    scalars = set(api.parent.SCALARS)|{'string','byte','char'}
    for row in facts['records']:
        need(type(row) is dict and set(row) == {'type','fields','constants','annotations','owner','installed_idl'} and
             type(row['type']) is str and re.fullmatch(TYPE,row['type']) and row['type'] not in messages and
             type(row['fields']) is list and 1 <= len(row['fields']) <= 4096 and
             type(row['constants']) is list and len(row['constants']) <= 4096 and
             type(row['annotations']) is list and len(row['annotations']) <= 8193 and
             type(row['owner']) is str and row['owner'] in facts['roots'], 'unique full original native Message required')
        pin(row['installed_idl'], path=True); names = set()
        for field in row['fields']:
            need(type(field) is dict and set(field) == set(FIELDS) and type(field['name']) is str and
                 re.fullmatch('[a-zA-Z_][a-zA-Z0-9_]*',field['name']) and field['name'] not in names and
                 type(field['type']) is str and (field['type'] in scalars or re.fullmatch(TYPE,field['type'])) and
                 all(type(field[k]) is int and 0 <= field[k] <= 32768 for k in ('array_size','string_bound','sequence_bound')) and
                 all(type(field[k]) is bool for k in ('is_array','is_sequence','has_declared_default')) and
                 type(field['native_default']) is str and '\0' not in field['native_default'] and
                 len(field['native_default'].encode('utf-8')) <= 32768 and
                 not(field['is_array'] and field['is_sequence']) and
                 (field['array_size'] > 0 if field['is_array'] else field['array_size'] == 0) and
                 (field['is_sequence'] or field['sequence_bound'] == 0) and
                 (field['type'] == 'string' or field['string_bound'] == 0) and
                 (field['has_declared_default'] or field['native_default'] == ''), 'exact typed native field facts required')
            names.add(field['name'])
        constants = set()
        for value in row['constants']:
            need(type(value) is dict and set(value) == {'name','type','value'} and type(value['name']) is str and
                 re.fullmatch('[A-Z][A-Z0-9_]*',value['name']) and value['name'] not in constants and
                 type(value['type']) is str and value['type'] in scalars and type(value['value']) is str and '\0' not in value['value'] and
                 len(value['value'].encode('utf-8')) <= 32768, 'complete typed native constants required')
            constants.add(value['name'])
        messages[row['type']] = row
        declarations[row['type']] = [tuple(field[k] for k in FIELDS) for field in row['fields']]

    def scalar(name, typ):
        return (name,typ,0,False,0,False,0,'',False)
    for row in facts['interfaces']:
        need(type(row) is dict and row.get('kind') in ('message','service','action') and
             set(row) == ({'type','kind','owner','installed_idl','components'} |
                         ({'send_goal_service','get_result_service','feedback_message'} if row['kind'] == 'action' else set())) and
             type(row['type']) is str and re.fullmatch(TYPE,row['type']) and row['type'] not in interfaces and
             type(row['owner']) is str and row['owner'] in facts['roots'] and type(row['components']) is dict,
             'unique complete actual native interface required')
        pin(row['installed_idl'], path=True); typ = row['type']; parts = row['components']
        roles = {'message':('message',),'service':('request','response'),'action':('goal','result','feedback')}[row['kind']]
        need(set(parts) == set(roles) and all(type(v) is str and v in messages for v in parts.values()),
             'whole exact native interface payload set required')
        need(all(value == (typ if role == 'message' else typ+'_'+role.title()) for role,value in parts.items()),
             'actual native payload role/name association differs')
        if row['kind'] == 'message':
            need(typ in messages, 'actual Message interface missing its record')
        elif row['kind'] == 'service':
            declarations[typ] = [scalar('request_message',parts['request']),scalar('response_message',parts['response'])]
        else:
            need(row['send_goal_service'] == typ+'_SendGoal' and row['get_result_service'] == typ+'_GetResult' and
                 row['feedback_message'] == typ+'_FeedbackMessage', 'actual derived Action role names differ')
            declarations[typ] = [scalar(role,parts[role]) for role in ('goal','result','feedback')]+[
                scalar('send_goal_service',row['send_goal_service']),scalar('get_result_service',row['get_result_service']),
                scalar('feedback_message',row['feedback_message'])]
        interfaces[typ] = row
    for typ, row in interfaces.items():
        if row['kind'] == 'action':
            need(all(row[k] in interfaces and interfaces[row[k]]['kind'] == 'service' for k in ('send_goal_service','get_result_service')) and
                 row['feedback_message'] in messages and 'action_msgs/srv/CancelGoal' in interfaces and
                 'action_msgs/msg/GoalStatusArray' in messages, 'actual complete Action protocol closure required')
    graphs = {}
    for root in declarations:
        selected = []; colors = {}
        def visit(typ, depth):
            need(depth <= 16 and colors.get(typ) != 1 and typ in declarations, 'missing/cyclic/deep native protocol graph')
            if colors.get(typ) == 2:return
            colors[typ] = 1; selected.append((typ,declarations[typ]))
            for field in declarations[typ]:
                if field[1] not in scalars:visit(field[1], depth+1)
            colors[typ] = 2
        visit(root,1); graphs[root] = selected
    # Every original IDL role must own its complete interfaces and declarations.
    for typ, row in {**messages,**interfaces}.items():
        need(row['owner'] in interfaces and interfaces[row['owner']]['owner'] == row['owner'], 'original native IDL owner differs')
        need(canonical(row['installed_idl']) == canonical(interfaces[row['owner']]['installed_idl']), 'payload/original IDL source differs')
    return messages, interfaces, graphs


def verify(api, records, ordered, original, pins, raw, *, require_outputs=True):
    need = api.env['need']; canonical = api.env['canonical']
    need(type(raw) is bytes and len(raw) <= 16*1024*1024, 'bounded original protocol proof DATA required')
    proof = api.env['strict_json'](raw)
    need(type(proof) is dict and set(proof) == {'format','scope','domain','source_proof_sha256','outputs','default_authority'} and
         proof['format'] == FORMAT and proof['default_authority'] == [], 'closed explicit legacy protocol proof required')
    domain = proof['domain']; need(type(domain) is dict and set(domain) == {'profile','catalog_inputs','source_generators',
         'proof_generator','source_helpers','source_records','source_interfaces','installed_idl_facts'}, 'closed complete protocol source domain required')
    _, source_helpers = helpers(api)
    expected = dict(profile=api.env['PROFILE'], catalog_inputs=pins, source_generators=api.sources,
        proof_generator=fact(api.env['read_source'](Path(__file__).resolve())), source_helpers=source_helpers,
        source_records=api.native_payload_source_facts(ordered), source_interfaces=source_interfaces(original))
    need(all(canonical(domain[k]) == canonical(value) for k,value in expected.items()) and
         proof['source_proof_sha256'] == hashlib.sha256(canonical(domain)).hexdigest(), 'original complete protocol/source/generator domain differs')
    facts = domain['installed_idl_facts']; messages, interfaces, graphs = snapshot(api, facts)
    need(canonical(facts['exporter_sources']) == canonical(dict(protocol=source_helpers['protocol_idl_export.py'],message=source_helpers['idl_export.py'])),
         'actual installed protocol exporters differ')
    for typ,row in expected['source_records'].items():
        need(typ in messages and canonical({k:messages[typ][k] for k in ('type','fields','constants')}) == canonical(row),
             'actual installed source fields/default presence/constants differ: '+typ)
    selected = set(expected['source_interfaces'])
    need(all(typ in interfaces and canonical({k:interfaces[typ][k] for k in ('kind','components')}) == canonical(row)
             for typ,row in expected['source_interfaces'].items()), 'original whole interface roles differ')
    if any(i.kind == 'action' for i in original):
        selected.update(('action_msgs/srv/CancelGoal','action_msgs/msg/GoalStatusArray'))
    reached = {typ for root in selected for typ,_ in graphs[root]}
    need(reached == set(graphs) and set(facts['roots']) == {row['owner'] for row in [*messages.values(),*interfaces.values()]},
         'unrelated or missing installed protocol source inventory')
    outputs = proof['outputs']
    need(type(outputs) is dict and set(outputs) == ({'native_source.hpp','native_source_identity.cc','native_source_provider.hpp','native_source_types.h'}
         if require_outputs else set()), 'whole emitted protocol proof source set required')
    for pin in outputs.values():
        need(type(pin) is dict and set(pin) == {'bytes','sha256'} and type(pin['bytes']) is int and
             0 < pin['bytes'] <= 16*1024*1024 and type(pin['sha256']) is str and re.fullmatch('[0-9a-f]{64}',pin['sha256']),
             'exact emitted protocol source fact required')
    return proof


def make(api, catalog, installed_raw):
    need = api.env['need']; canonical = api.env['canonical']
    records, ordered, original, pins = api.load_catalog(catalog,False)
    need(all(r.version == 2 and r.identity is None for r in ordered) and any(i.kind != 'message' for i in original),
         'explicit unbound complete ROS2 Service/Action catalog required')
    h, source_helpers = helpers(api); facts = api.env['strict_json'](installed_raw)
    domain = dict(profile=api.env['PROFILE'], catalog_inputs=pins, source_generators=api.sources,
        proof_generator=fact(api.env['read_source'](Path(__file__).resolve())), source_helpers=source_helpers,
        source_records=api.native_payload_source_facts(ordered), source_interfaces=source_interfaces(original), installed_idl_facts=facts)
    digest = hashlib.sha256(canonical(domain)).hexdigest()
    proof = dict(format=FORMAT, scope='Complete legacy protocol proof SOURCE; not native execution, identity authentication, endpoint or authority.',
                 domain=domain, source_proof_sha256=digest, outputs={}, default_authority=[])
    verify(api,records,ordered,original,pins,canonical(proof),require_outputs=False)
    messages, interfaces, graphs = snapshot(api,facts); names = {}; order = []
    def visit(typ):
        if typ in names:return
        for field in messages[typ]['fields']:
            if field['type'] in messages:visit(field['type'])
        names[typ] = 'm'+str(len(order)); order.append(typ)
    for typ in sorted(messages):visit(typ)
    ns = 'uwvm_installed_protocol_'+digest
    lines = ['/** Complete actual installed legacy protocol proof. No Node or grant. */','#pragma once',
        '#include "gateways/ros2/compat/installed_protocol.h"','#include <cstdint>','#include <cstddef>',
        '#include <string>','#include <string_view>','#include <type_traits>']
    lines += ['#include <'+typ.rsplit('/',1)[0]+'/'+api.parent.ros2_header(typ.rsplit('/',1)[1])+'.hpp>' for typ in sorted(facts['roots'])]
    lines += ['namespace '+ns+' {','namespace source=::uwvm2::ros_gateway::ros2_compat::installed_source;',
              'namespace ipc=::uwvm2::imported::ros::ipc;']
    for typ in order:
        row = messages[typ]; sym = names[typ]; native = h.native_type(typ)
        lines += ['using '+sym+'='+native+';','inline bool value_'+sym+'('+sym+' const& value){']
        for index,field in enumerate(row['fields']):
            kind = field['type']; access = 'value.'+field['name']; shape = 1 if field['is_array'] else 2 if field['is_sequence'] else 0
            bound = field['array_size'] if shape == 1 else field['sequence_bound'] if shape == 2 else 0
            raw = 'field_'+str(index)
            lines += ['using '+raw+'=std::remove_cv_t<std::remove_reference_t<decltype('+access+')>>;',
                'static_assert(source::field_type<'+raw+','+h.native_type(kind)+','+str(shape)+','+str(bound)+'>(),"actual protocol field type differs");']
            if shape == 2:
                lines.append('if(!'+access+'.empty())return false;');continue
            if shape == 1:lines.append('for(auto const& item:'+access+'){');access = 'item'
            if kind in messages:lines.append('if(!value_'+names[kind]+'('+access+'))return false;')
            elif kind == 'string':
                text = field['native_default'];lines.append('if(std::string_view{'+access+'}!=std::string_view{'+h.cpp_text(text)+','+str(len(text.encode('utf-8')))+'})return false;')
            else:
                value = field['native_default'] if field['has_declared_default'] else 'False' if kind == 'bool' else '0'
                lines.append('if(!source::same_value('+access+','+h.native_type(kind)+'('+h.literal(kind,value)+')))return false;')
            if shape == 1:lines.append('}')
        for constant in row['constants']:
            kind = constant['type']; access = native+'::'+constant['name']
            if kind == 'string':
                text = constant['value'];lines.append('if(std::string_view{'+access+'}!=std::string_view{'+h.cpp_text(text)+','+str(len(text.encode('utf-8')))+'})return false;')
            else:lines += ['static_assert(std::is_same_v<std::remove_cv_t<decltype('+access+')>,'+h.native_type(kind)+'>,"actual protocol constant type differs");',
                'if(!source::same_value('+access+','+h.native_type(kind)+'('+h.literal(kind,constant['value'])+')))return false;']
        lines += ['return true;}','inline bool typed_'+sym+'(){return value_'+sym+'('+sym+'{});}']
    graph_names = {}
    for index,(typ,graph) in enumerate(graphs.items()):
        graph_names[typ] = 'g'+str(index)
        for node,(name,fields) in enumerate(graph):
            rows = ['{'+','.join(json.dumps(v) if isinstance(v,str) else str(v).lower() if type(v) is bool else str(v) for v in field)+'}' for field in fields]
            lines.append('inline constexpr ipc::custom_native_field '+graph_names[typ]+'_f'+str(node)+'[]={'+','.join(rows)+'};')
        lines.append('inline constexpr ipc::custom_native_record '+graph_names[typ]+'[]={'+','.join(
            '{'+json.dumps(name)+','+graph_names[typ]+'_f'+str(node)+','+str(len(fields))+'}' for node,(name,fields) in enumerate(graph))+'};')
    for typ in order:
        sym = names[typ]
        lines.append('inline constexpr source::field_layout layout_'+sym+'[]={'+','.join(
            '{offsetof('+sym+','+f['name']+'),sizeof(std::declval<'+sym+'>().'+f['name']+')}' for f in messages[typ]['fields'])+'};')
    lines.append('inline source::witness const witnesses[]={'+','.join('{'+json.dumps(typ)+','+json.dumps(messages[typ]['installed_idl']['sha256'])+
        ',&::uwvm2::ros_gateway::ros2_compat::message_members_for<'+names[typ]+'>,&typed_'+names[typ]+',sizeof('+names[typ]+'),layout_'+names[typ]+','+str(len(messages[typ]['fields']))+'}' for typ in order)+'};')
    lines += ['}','namespace uwvm2::ros_gateway::ros2_compat::installed_source {']
    for typ in order:
        graph = ns+'::'+graph_names[typ]
        lines += ['template<>struct traits<'+h.native_type(typ)+'>{static constexpr bool enabled=true;',
            'inline static constexpr std::string_view type='+json.dumps(typ)+',source_proof_sha256="'+digest+'";',
            'static constexpr ipc::custom_native_schema schema()noexcept{return {'+graph+',sizeof('+graph+')/sizeof(ipc::custom_native_record)};}',
            'static bool prove(){return records(schema(),'+ns+'::witnesses,sizeof('+ns+'::witnesses)/sizeof(witness));}};']
    for typ,row in interfaces.items():
        if row['kind'] == 'message':continue
        graph = ns+'::'+graph_names[typ]; native = h.native_type(typ)
        lines += ['template<>struct interface_traits<'+native+'>{static constexpr bool enabled=true;static constexpr unsigned kind='+('1' if row['kind'] == 'service' else '2')+';',
            'inline static constexpr std::string_view type='+json.dumps(typ)+',source_proof_sha256="'+digest+'";',
            'static constexpr ipc::custom_native_schema schema()noexcept{return {'+graph+',sizeof('+graph+')/sizeof(ipc::custom_native_record)};}',
            'static auto support(){return ::rosidl_typesupport_cpp::get_'+row['kind']+'_type_support_handle<'+native+'>();}};']
    lines += ['}']
    exporter = ['/** Actual typed legacy protocol export; original native owner required. */','#include "native_source.hpp"',
        '#include "tests/ros_custom_types/bounded-v2/native_protocol.h"','#include <csignal>','#include <cstring>','#include <stdexcept>',
        'int main(int argc,char**argv){try{if(argc!=2||std::strcmp(argv[1],"--mapped-stop"))return 64;',
        'namespace source=::uwvm2::ros_gateway::ros2_compat::installed_source;namespace compat=::uwvm2::ros_gateway::ros2_compat;compat::rihs_json output;']
    prefix = dict(format=NATIVE_FORMAT,ros_version=2,source_profile=api.env['PROFILE'],
        constant_schema_sha256=hashlib.sha256(canonical(api.constant_facts(ordered))).hexdigest(),source_proof_sha256=digest)
    exporter.append('output.add('+h.cpp_text(json.dumps(prefix,separators=(',',':'))[:-1]+',"types":{')+');')
    def emit(typ, kind):
        graph = api._modern_graph(graphs[typ],typ);graph['references'] = [graph['references'][n] for n in sorted(graph['references'])]
        graph.update(identity_provenance=PROVENANCE,native_baked_hash_available=False,source_proof_sha256=digest)
        identity = 'source::identity<'+h.native_type(typ)+'>()' if kind == 'message' else 'source::protocol_identity<'+h.native_type(typ)+'>(source::interface_traits<'+h.native_type(typ)+'>::support())'
        return ['{auto identity='+identity+';if(identity.empty())throw std::runtime_error{'+h.cpp_text('actual complete native protocol refused: '+typ)+'};',
            'output.add('+h.cpp_text('{"native_identity":')+');output.name(identity);output.add(",");output.add('+h.cpp_text(json.dumps(graph,separators=(',',':'))[1:])+');}']
    for index,interface in enumerate(original):
        exporter.append('output.add('+h.cpp_text((',' if index else '')+json.dumps(interface.name)+':{"official":')+');')
        exporter += emit(interface.name,interface.kind)
        exporter.append('output.add('+h.cpp_text(',"components":{')+');')
        for n,(role,record) in enumerate(interface.components.items()):
            exporter.append('output.add('+h.cpp_text((',' if n else '')+json.dumps(role)+':')+');');exporter += emit(record.name,'message')
        exporter.append('output.add("}}");')
    exporter += ['output.add('+h.cpp_text('}}\n')+');if(!output.valid())throw std::length_error{"whole protocol export bound"};',
        'if(::raise(SIGSTOP)!=0)throw std::runtime_error{"original native mapping checkpoint failed"};',
        'bounded_native_io::gate(\'I\');bounded_native_io::emit(STDOUT_FILENO,output.body());return 0;',
        '}catch(std::exception const& e){bounded_native_io::print(STDERR_FILENO,std::string_view{e.what()},"\\n");return 1;}}']
    products = {'native_source.hpp':'\n'.join(lines)+'\n','native_source_identity.cc':'\n'.join(exporter)+'\n',
        'native_source_provider.hpp':'/** Explicit matching native protocol provider. No authority. */\n#pragma once\n#include "native_source.hpp"\n#include "native_ros2.hpp"\n',
        'native_source_types.h':'/** Full actual proof inventory, including protocol-only dependencies. */\n#pragma once\n#define UWVM_ROS_INSTALLED_SOURCE_MESSAGES(X) \\\n'+' \\\n'.join('X('+h.native_type(typ)+','+json.dumps(typ)+')' for typ in order)+'\n#define UWVM_ROS_INSTALLED_SOURCE_INTERFACES(X) \\\n'+' \\\n'.join('X('+h.native_type(typ)+','+json.dumps(typ)+')' for typ,row in interfaces.items() if row['kind'] != 'message')+'\n'}
    proof['outputs'] = {name:fact(text.encode()) for name,text in products.items()}
    products['native_source_proof.json'] = canonical(proof).decode()
    need(all(len(text.encode()) <= 16*1024*1024 for text in products.values()), 'bounded complete protocol source outputs required')
    return products, proof


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('source-root','catalog','installed-idl','out'):p.add_argument('--'+name,required=True,type=Path)
    for name in ('parent','bounded','adapter'):p.add_argument('--'+name+'-sha256',required=True)
    args = p.parse_args(); root = args.source_root; path = root/'sdk/robots/industrial/vendor_profile.py'; raw = path.read_bytes()
    if hashlib.sha256(raw).hexdigest() != args.adapter_sha256:raise ValueError('actual reviewed adapter SHA differs')
    m = types.ModuleType('_complete_legacy_protocol_adapter');m.__file__ = str(path)
    exec(compile(raw,str(path),'exec',dont_inherit=True),m.__dict__)
    parent = root/'sdk/tools/custom_types.py';bounded = root/'sdk/custom-types/bounded_profile.py'
    api = m.API(m.read_source(parent),m.read_source(bounded),parent_path=parent,bounded_path=bounded,
        parent_sha256=args.parent_sha256,bounded_sha256=args.bounded_sha256,adapter_raw=raw,adapter_sha256=args.adapter_sha256,
        templates={f'interface_templates/bounded_interfaces.{ext}.in':m.read_source(bounded.parent/f'interface_templates/bounded_interfaces.{ext}.in') for ext in ('rs','mjs','go','ts')})
    installed = m.read_source(args.installed_idl);products,_ = make(api,args.catalog,installed)
    m.need(args.out.is_absolute() and args.out.parent.resolve(strict=True) == args.out.parent and not args.out.exists() and
           not args.out.is_symlink() and m.read_source(path) == raw and m.read_source(args.installed_idl) == installed,
           'fresh physical output and unchanged original inputs required')
    args.out.mkdir(mode=0o700)
    for name,text in products.items():
        fd = os.open(args.out/name,os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o600)
        with os.fdopen(fd,'wb') as stream:stream.write(text.encode())


if __name__ == '__main__':
    main()
