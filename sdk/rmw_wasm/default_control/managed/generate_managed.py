#!/usr/bin/env python3
"""Emit complete original control Guest records for four languages.

This explicit operator consumes captured, SHA-paired trusted current IR source
and the separate genuine C/C++ canonical source revision. It never compiles,
executes a generated product, obtains Native identities or grants authority.
The historical custom_types.py --guest-only control command remains unchanged.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import stat
import sys
import types

PROFILE='uwvm-ros-original-node-control-c-cpp-v1'
LIMIT=8*1024*1024
MAX_OUTPUT=32*1024*1024
LANGUAGES=['c','cpp','rust','javascript','assemblyscript','tinygo']
# Exact selected original family, matching production original_control_record_name.
EXPECTED_RECORDS=set(['builtin_interfaces/msg/Time', 'rcl_interfaces/msg/FloatingPointRange', 'rcl_interfaces/msg/IntegerRange', 'rcl_interfaces/msg/ListParametersResult', 'rcl_interfaces/msg/Log', 'rcl_interfaces/msg/Parameter', 'rcl_interfaces/msg/ParameterDescriptor', 'rcl_interfaces/msg/ParameterEvent', 'rcl_interfaces/msg/ParameterType', 'rcl_interfaces/msg/ParameterValue', 'rcl_interfaces/msg/SetParametersResult', 'rcl_interfaces/srv/DescribeParameters_Request', 'rcl_interfaces/srv/DescribeParameters_Response', 'rcl_interfaces/srv/GetParameterTypes_Request', 'rcl_interfaces/srv/GetParameterTypes_Response', 'rcl_interfaces/srv/GetParameters_Request', 'rcl_interfaces/srv/GetParameters_Response', 'rcl_interfaces/srv/ListParameters_Request', 'rcl_interfaces/srv/ListParameters_Response', 'rcl_interfaces/srv/SetParameters_Request', 'rcl_interfaces/srv/SetParameters_Response', 'rcl_interfaces/srv/SetParametersAtomically_Request', 'rcl_interfaces/srv/SetParametersAtomically_Response', 'service_msgs/msg/ServiceEventInfo', 'type_description_interfaces/msg/Field', 'type_description_interfaces/msg/FieldType', 'type_description_interfaces/msg/IndividualTypeDescription', 'type_description_interfaces/msg/KeyValue', 'type_description_interfaces/msg/TypeDescription', 'type_description_interfaces/msg/TypeSource', 'type_description_interfaces/srv/GetTypeDescription_Request', 'type_description_interfaces/srv/GetTypeDescription_Response'])
CANONICAL_NAMES={'bounded_records.h','bounded_records.hpp','bounded_interfaces.h','bounded_interfaces.hpp','control_wire.hpp','control_manifest.hpp','control-schema.json'}

def read(path,limit=LIMIT):
    """Admit one ordinary FD before reading; FIFO/symlink never blocks this open."""
    fd=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
    try:
        first=os.fstat(fd)
        if not stat.S_ISREG(first.st_mode)or first.st_size>limit:
            raise ValueError('bounded ordinary source required')
        chunks=[];count=0
        while True:
            part=os.read(fd,min(1024*1024,limit-count+1))
            if not part:break
            count+=len(part)
            if count>limit:raise ValueError('source grew beyond explicit bound')
            chunks.append(part)
        final=os.fstat(fd);named=os.lstat(path)
        def key(s):return(s.st_dev,s.st_ino,s.st_size,s.st_mode,s.st_mtime_ns,s.st_ctime_ns,s.st_nlink)
        if key(first)!=key(final)or key(first)!=key(named)or count!=first.st_size:
            raise ValueError('source FD/name/extent changed')
        return b''.join(chunks)
    finally:os.close(fd)

def strict(raw):
    def pairs(rows):
        out={}
        for key,value in rows:
            if key in out:raise ValueError('duplicate JSON key')
            out[key]=value
        return out
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:(_ for _ in()).throw(ValueError('nonfinite JSON')))

def sha(raw):return hashlib.sha256(raw).hexdigest()

def expected_sha(value):
    if not isinstance(value,str)or len(value)!=64 or any(c not in '0123456789abcdef' for c in value):
        raise ValueError('explicit lowercase SHA256 required')
    return value

def paired_module(name,path,raw):
    # Only bytes already captured and explicitly SHA-paired by the caller are
    # loaded. The catalog never supplies a module name, plugin or executable.
    module=types.ModuleType(name);module.__file__=str(path);sys.modules[name]=module
    exec(compile(raw,str(path),'exec'),module.__dict__)
    return module

def run(source_root,catalog,canonical,output,parent_sha,bounded_sha):
    if not all(p.is_absolute()for p in(source_root,catalog,canonical,output)):
        raise ValueError('all four source/output paths must be explicit absolute paths')
    if output.exists()or output==canonical or output.is_relative_to(canonical)or canonical.is_relative_to(output):
        raise ValueError('managed output must be fresh and separate from canonical source')
    source_root=source_root.resolve();catalog=catalog.absolute();canonical=canonical.resolve();output=output.resolve()
    if output.exists()or output.is_relative_to(canonical)or canonical.is_relative_to(output):
        raise ValueError('resolved output must remain fresh and separate')
    operator_raw=read(Path(__file__))
    parent_path=source_root/'sdk/tools/custom_types.py';bounded_path=source_root/'sdk/custom-types/bounded_profile.py'
    parent_raw=read(parent_path);bounded_raw=read(bounded_path)
    if sha(parent_raw)!=expected_sha(parent_sha)or sha(bounded_raw)!=expected_sha(bounded_sha):
        raise ValueError('actual captured generator source differs from explicit reviewed SHA')
    catalog_raw=read(catalog);declared=strict(catalog_raw)
    if not isinstance(declared,dict)or set(declared)!={'format','types'}or declared['format']!=PROFILE or not isinstance(declared['types'],list):
        raise ValueError('exact original control catalog required')
    captured={};base=catalog.resolve().parent
    for entry in declared['types']:
        if not isinstance(entry,dict)or not isinstance(entry.get('file'),str):raise ValueError('complete source row required')
        p=(base/entry['file']).resolve()
        if not p.is_relative_to(base):raise ValueError('definition outside original catalog root')
        captured[p]=read(p)
    templates={}
    parent=paired_module('_uwvm_managed_control_parent',parent_path,parent_raw)
    bounded=paired_module('_uwvm_managed_control_bounded',bounded_path,bounded_raw)
    records,ordered,interfaces,pins=bounded.load_catalog(parent,catalog,False)
    if pins.get('catalog')!={'bytes':len(catalog_raw),'sha256':sha(catalog_raw)}:
        raise ValueError('resolved IR catalog differs from the captured admitted catalog')
    if len(ordered)!=32 or {r.name for r in ordered}!=EXPECTED_RECORDS or any(r.profile!=PROFILE for r in ordered):
        raise ValueError('complete original32-record control closure required')
    registered=[i for i in interfaces if i.registered]
    if {i.name for i in registered}!=bounded.control_registered_names() or sum(i.kind=='message'for i in registered)!=2 or sum(i.kind=='service'for i in registered)!=7:
        raise ValueError('complete two Topic/seven Service root family required')
    generation_raw=read(canonical/'generation.json');generation=strict(generation_raw)
    if generation.get('format')!=PROFILE or generation.get('implemented_record_outputs')!=['c','cpp']or generation.get('native_registry_emitted')is not False or generation.get('source_inputs')!=pins:
        raise ValueError('separate actual same-source authoritative C/C++ revision required')
    if generation.get('generator_sha256')!=parent_sha or generation.get('bounded_generator_sha256')!=bounded_sha:
        raise ValueError('canonical revision requires this exact admitted IR source')
    outputs=generation.get('outputs')
    if not isinstance(outputs,dict)or set(outputs)!=CANONICAL_NAMES:
        raise ValueError('complete historical seven-product/eighth-proof canonical source required')
    canonical_raw={}
    for name,pin in outputs.items():
        raw=read(canonical/name)
        if not isinstance(pin,dict)or type(pin.get('bytes'))is not int or pin['bytes']!=len(raw)or expected_sha(pin.get('sha256'))!=sha(raw):
            raise ValueError('canonical product extent/hash differs')
        canonical_raw[canonical/name]=raw
    products,proof=bounded.control_managed_products(parent,records,ordered,interfaces,pins)
    if proof.get('generator_sha256')!=sha(parent_raw)or proof.get('bounded_generator_sha256')!=sha(bounded_raw):
        raise ValueError('managed proof generator hashes differ from actual captured executed sources')
    proof.update(generator_sha256=sha(parent_raw),bounded_generator_sha256=sha(bounded_raw),source_language_inventory=LANGUAGES,
        c_cpp_products='Separate authoritative unchanged generation.json/source command; not duplicated here',
        canonical_generation={'bytes':len(generation_raw),'sha256':sha(generation_raw)},
        canonical_outputs=outputs,
        operator_sha256=sha(operator_raw),
        input_generator_pair={'parent':parent_sha,'bounded':bounded_sha},
        execution_status='SOURCE_ONLY_GENERATED_PRODUCTS_NOT_COMPILED_OR_EXECUTED')
    products['generation.control-managed.json']=json.dumps(proof,indent=2)+'\n'
    if set(products)!={'bounded_records.'+e for e in('rs','mjs','go','ts')}|{'generation.control-managed.json'}:
        raise ValueError('exact four record modules/proof required')
    total=0
    for name,body in products.items():
        if '/'in name or '\\'in name or name in('.','..')or not isinstance(body,str):raise ValueError('fixed source basenames only')
        extent=len(body.encode());total+=extent
        if extent>LIMIT or total>MAX_OUTPUT:raise ValueError('generated source revision exceeds bound')
    # Admission and emission are followed by terminal exact input reopens. This
    # does not turn a path/name/version tuple into Native proof or a permission.
    for p,raw in {**captured,**templates,**canonical_raw}.items():
        if read(p)!=raw:raise ValueError('admitted source changed before commit')
    if read(parent_path)!=parent_raw or read(bounded_path)!=bounded_raw or read(catalog)!=catalog_raw or read(canonical/'generation.json')!=generation_raw or read(Path(__file__))!=operator_raw:
        raise ValueError('generator/catalog/canonical proof changed before commit')
    for row in pins['definitions']:
        if sha(captured[(base/row['file']).resolve()])!=row['sha256']or len(captured[(base/row['file']).resolve()])!=row['bytes']:
            raise ValueError('resolved IR/source pairing differs')
    if output.exists():raise ValueError('output appeared before commit')
    output.mkdir(parents=True)
    def write_new(name,body):
        raw=body.encode('utf-8');fd=os.open(output/name,os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o644)
        try:
            at=0
            while at<len(raw):
                wrote=os.write(fd,raw[at:])
                if wrote<=0:raise OSError('source output made no progress')
                at+=wrote
        finally:os.close(fd)
    for name,body in products.items():
        if name!='generation.control-managed.json':write_new(name,body)
    # Proof is last. A failed filesystem write leaves an incomplete fresh source
    # revision; no overwrite, directory cleanup, executable or grant is attempted.
    write_new('generation.control-managed.json',products['generation.control-managed.json'])
    return proof

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for key in('source-root','catalog','canonical','out'):parser.add_argument('--'+key,type=Path,required=True)
    parser.add_argument('--parent-sha256',required=True);parser.add_argument('--bounded-sha256',required=True)
    parser.add_argument('--records-only',action='store_true',required=True,help='Explicitly select complete records; owned-RMW Service source is a separate in-progress layer')
    args=parser.parse_args()
    try:print(json.dumps(run(args.source_root,args.catalog,args.canonical,args.out,args.parent_sha256,args.bounded_sha256),indent=2))
    except(OSError,ValueError,TypeError,KeyError,RecursionError)as error:
        print(str(error),file=sys.stderr);return 2
    return 0
if __name__=='__main__':raise SystemExit(main())
