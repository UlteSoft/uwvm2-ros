#!/usr/bin/env python3
"""Extract conservative ROS IDL shapes through a pinned upstream rosidl grammar.

IDL and include locators are data only: this tool never follows an include,
preprocesses code, imports a type support library or evaluates a source-level
expression as Python. The upstream parser and Lark dependency are explicit,
retained inputs. File parsing is byte/time bounded inside the existing cgroup.
Only plain ROS structures in the owning package namespace are translated.
Unknown annotations, enums, forward declarations, ambiguous aliases, unsupported
scalars and complex arrays fail closed. Constants/defaults remain pinned source
metadata; this is structural qualification, not native or package semantics.
"""
import argparse
from collections import Counter
import copy
import json
from pathlib import Path
import shutil
import signal
import sys
import time
from capability_inventory import ROOT,GATE,guard,sha

class Refused(Exception):pass

def parse(text,package):
    from rosidl_parser import parser as p
    from rosidl_parser import definition as d
    from lark import Tree
    tree=p.get_ast_from_idl_string(text)
    for feature in ('enum_dcl','struct_forward_dcl'):
        if next(tree.find_data(feature),None) is not None:raise Refused('UNSUPPORTED_'+feature.upper())
    # An extension can change layout/meaning even when the parser accepts it.
    # Inspect all applications, including module-level inherited annotations.
    for annotation in tree.find_data('annotation_appl'):
        name=p.get_first_identifier_value(annotation)
        if name not in ('verbatim','default','final'):raise Refused('ANNOTATION_REQUIRES_REVIEW:'+name)
    typedefs={}
    for declaration in tree.find_data('typedef_dcl'):
        if len(declaration.children)!=1:raise Refused('TYPEDEF_REQUIRES_REVIEW')
        declarator=declaration.children[0]
        if not isinstance(declarator,Tree) or declarator.data!='type_declarator' or len(declarator.children)!=2:raise Refused('TYPEDEF_REQUIRES_REVIEW')
        abstract=p.get_abstract_type(declarator.children[0]);names=declarator.children[1]
        if not isinstance(names,Tree) or names.data!='any_declarators' or len(names.children)!=1:raise Refused('TYPEDEF_REQUIRES_REVIEW')
        name=p.get_first_identifier_value(names.children[0]);value=p.get_abstract_type_optionally_as_array(abstract,names.children[0])
        if name in typedefs:raise Refused('AMBIGUOUS_TYPEDEF_SCOPE')
        typedefs[name]=value
    basic={'boolean':'bool','octet':'uint8','char':'uint8','float':'float32','double':'float64',
           'short':'int16','unsigned short':'uint16','long':'int32','unsigned long':'uint32',
           'long long':'int64','unsigned long long':'uint64',**{name:name for name in
           ('int8','uint8','int16','uint16','int32','uint32','int64','uint64')}}
    def field(member,namespace):
        value=member.type;collection='scalar';array_bound=None
        if isinstance(value,d.Array):collection='fixed_array';array_bound=value.size;value=value.value_type
        elif isinstance(value,d.BoundedSequence):collection='bounded_sequence';array_bound=value.maximum_size;value=value.value_type
        elif isinstance(value,d.UnboundedSequence):collection='sequence';value=value.value_type
        if array_bound is not None and (type(array_bound)is not int or not 0<array_bound<=0xffffffff):raise Refused('ARRAY_BOUND_REQUIRES_REVIEW')
        string_bound=None
        if isinstance(value,d.BasicType):
            if value.typename not in basic:raise Refused('SCALAR_REQUIRES_REVIEW:'+value.typename)
            kind=basic[value.typename]
        elif isinstance(value,(d.AbstractString,d.AbstractWString)):
            kind='wstring' if isinstance(value,d.AbstractWString) else 'string';string_bound=getattr(value,'maximum_size',None)
            if string_bound is not None and (type(string_bound)is not int or not 0<string_bound<=0xffffffff):raise Refused('STRING_BOUND_REQUIRES_REVIEW')
        elif isinstance(value,d.NamespacedType):
            if len(value.namespaces)!=2 or value.namespaces[1] not in ('msg','srv','action'):raise Refused('TYPE_NAMESPACE_REQUIRES_REVIEW')
            kind='/'.join([*value.namespaces,value.name])
        elif isinstance(value,d.NamedType):
            if value.name in typedefs:raise Refused('UNRESOLVED_TYPEDEF')
            kind='/'.join([*namespace,value.name])
        else:raise Refused('NESTED_TYPE_REQUIRES_REVIEW')
        return dict(line=0,section=1,name=member.name,type=kind,primitive=kind in basic.values()or kind in ('string','wstring'),
                    string_bound=string_bound,array=collection,array_bound=array_bound,has_default=member.has_annotation('default'))
    results=[];names=set();structures=list(tree.find_data('struct_def'))
    local_names={tuple([*p.get_module_identifier_values(tree,s),p.get_child_identifier_value(s)])for s in structures}
    for structure in structures:
        namespace=p.get_module_identifier_values(tree,structure);name=p.get_child_identifier_value(structure)
        if len(namespace)!=2 or namespace[0]!=package or namespace[1]not in ('msg','srv','action'):raise Refused('OWNER_NAMESPACE_REQUIRES_REVIEW')
        identity='/'.join([*namespace,name])
        if identity in names:raise Refused('DUPLICATE_STRUCTURE')
        names.add(identity);message=d.Message(d.Structure(d.NamespacedType(namespace,name)))
        p.add_message_members(message,structure)
        for member in message.structure.members:
            value=member.type.value_type if isinstance(member.type,d.AbstractNestedType) else member.type
            if isinstance(value,d.NamedType)and tuple([*namespace,value.name])in local_names:
                if value.name in typedefs:raise Refused('AMBIGUOUS_TYPEDEF_SCOPE')
                qualified=d.NamespacedType(namespace,value.name)
                if isinstance(member.type,d.AbstractNestedType):member.type.value_type=qualified
                else:member.type=qualified
        p.resolve_typedefed_names(message.structure,copy.deepcopy(typedefs))
        fields=[field(m,namespace)for m in message.structure.members]
        # The generated empty-structure marker is an actual octet in XCDR1.
        # Keep it explicit; ordinary no-member records have the same wire shape.
        results.append(dict(type_name=name,type_namespace=namespace[1],type_identity=identity,fields=fields))
    if not results:raise Refused('NO_STRUCTURE')
    return results

def checks():
    def source(body):return 'module test_msgs { module msg { '+body+' }; };'
    good=parse(source('typedef sequence<double, 4> Samples; struct Item { Samples values; string<12> name; unsigned long counter; };'),'test_msgs')
    assert good[0]['fields'][0]['type']=='float64'and good[0]['fields'][0]['array_bound']==4
    assert good[0]['fields'][1]['string_bound']==12 and good[0]['fields'][2]['type']=='uint32'
    assert len(parse(source('struct A { octet data; }; struct B { A previous; };'),'test_msgs'))==2
    assert parse('#include "/must-not-be-opened/unsafe.idl"\n'+source('struct A { long value; };'),'test_msgs')[0]['fields'][0]['type']=='int32'
    cases=['@mutable struct A { long value; };','struct A { @optional long value; };',
           'enum Choice { X, Y }; struct A { Choice value; };','struct A; struct B { A value; };',
           'struct A { long double value; };','struct A { long values[2][3]; };',
           'typedef long Alias; typedef long Alias; struct A { Alias value; };',
           'struct A { long value; }; struct A { short value; };']
    for body in cases:
        try:parse(source(body),'test_msgs')
        except Exception:pass
        else:raise AssertionError('Unsupported construct accepted: '+body)
    try:parse(source('struct A { long value; };'),'foreign_package')
    except Refused:pass
    else:raise AssertionError('Foreign ownership accepted')
    return len(cases)+4

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--inventory',type=Path,required=True)
    parser.add_argument('--parser-root',type=Path,required=True);parser.add_argument('--lark-wheel',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);args=parser.parse_args();guard()
    out=args.output.resolve();temporary=(ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve();assert out.is_relative_to(temporary);out.mkdir(parents=True,exist_ok=False)
    upstream=args.parser_root.resolve();wheel=args.lark_wheel.resolve();assert upstream.is_relative_to(temporary)and wheel.is_relative_to(temporary)
    inventory=args.inventory.resolve();proof=json.loads((inventory/'results.json').read_text());assert proof['complete']
    pins={str(path):sha(path)for path in [inventory/'results.json',inventory/'definitions.json',wheel,Path(__file__).resolve()]}
    snapshot=out/'parser/rosidl_parser';snapshot.mkdir(parents=True)
    for path in sorted((upstream/'rosidl_parser').iterdir()):
        if path.suffix in ('.py','.lark'):
            assert path.is_file()and not path.is_symlink();shutil.copyfile(path,snapshot/path.name);pins[str(path)]=sha(path);pins[str(snapshot/path.name)]=sha(snapshot/path.name)
    # Zip import keeps the explicitly hashed wheel intact and avoids a global
    # package install. Upstream grammar includes remain package resources only.
    sys.dont_write_bytecode=True;sys.path[:0]=[str(wheel),str(snapshot.parent)]
    negative_checks=checks();records=json.loads((inventory/'definitions.json').read_text());original_count=len(records)
    outcomes=[];totals=Counter();parse_cache={};started=time.monotonic()
    def timeout(_signal,_frame):raise Refused('PARSE_TIME_BOUND')
    old=signal.signal(signal.SIGALRM,timeout)
    for index in range(original_count):
        record=records[index]
        if record['kind']!='idl':continue
        guard();path=Path(record['file']);assert sha(path)==record['sha256'];pins[str(path)]=record['sha256'];row=dict(index=index,file=str(path))
        try:
            if record['bytes']>1<<20:raise Refused('PARSE_BYTE_BOUND')
            key=(record['sha256'],record['package'])
            if key not in parse_cache:
                signal.setitimer(signal.ITIMER_REAL,3)
                try:parse_cache[key]=parse(path.read_text(encoding='utf-8-sig'),record['package'])
                finally:signal.setitimer(signal.ITIMER_REAL,0)
            values=parse_cache[key];indices=[]
            for value in values:
                translated={**record,**copy.deepcopy(value), 'kind':'msg','original_kind':'idl','original_index':index,
                    'status':'SHAPES_EXTRACTED','issues':[],'sections':1}
                indices.append(len(records));records.append(translated)
            row.update(status='IDL_SHAPES_EXTRACTED',structures=indices)
            records[index]={**record,'status':'IDL_SHAPES_EXTRACTED','structures':indices}
        except Refused as failure:row.update(status='IDL_REFUSED',reason=str(failure))
        except Exception as failure:row.update(status='IDL_PARSE_REVIEW',reason=type(failure).__name__+': '+str(failure)[:512])
        finally:signal.setitimer(signal.ITIMER_REAL,0)
        if row['status']!='IDL_SHAPES_EXTRACTED':records[index]={**record,'status':row['status'],'reason':row['reason']}
        outcomes.append(row);totals[row['status']]+=1
    signal.signal(signal.SIGALRM,old)
    report=dict(complete=all(sha(p)==h for p,h in pins.items()),scope=__doc__,inputs=pins,counts=dict(totals),
        original_records=original_count,added_structures=len(records)-original_count,parser_checks=negative_checks,
        seconds=time.monotonic()-started,cgroup=str(GATE),memory_max=68719476736,swap_max=0,semantic_audit=False)
    for name,value in [('definitions.json',records),('outcomes.json',outcomes),('results.json',report)]:
        (out/name).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps({key:report[key]for key in ('complete','counts','added_structures','parser_checks','seconds')}),flush=True);assert report['complete']

if __name__=='__main__':main()
