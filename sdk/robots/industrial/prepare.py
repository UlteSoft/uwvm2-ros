#!/usr/bin/env python3
"""Prepare one complete industrial source family; never run native/Guest code.

Explicit source SHA pairing and fresh caller-owned output are mandatory. This
entry is separate from the old vendor19 operator. It cannot add a vendor to the
Runtime, register a type, install a driver, grant an alias or qualify a robot.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import stat
import sys
import types
import math

LIMIT = 16 * 1024 * 1024


def need(ok, why):
    if not ok: raise ValueError(why)


def read(path, limit=LIMIT):
    path = Path(path)
    need(path.is_absolute(), 'absolute source path required')
    fd = os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
    try:
        a = os.fstat(fd)
        need(stat.S_ISREG(a.st_mode) and a.st_nlink == 1 and 0 <= a.st_size <= limit,
             'bounded ordinary single-link source required')
        chunks=[];count=0
        while True:
            b=os.read(fd,min(65536,limit+1-count))
            if not b:break
            count+=len(b);need(count<=limit and count<=a.st_size,'source growth beyond admission');chunks.append(b)
        z=os.fstat(fd);n=os.stat(path,follow_symlinks=False)
        key=lambda s:(s.st_dev,s.st_ino,s.st_mode,s.st_nlink,s.st_size,s.st_mtime_ns,s.st_ctime_ns)
        need(key(a)==key(z)==key(n) and count==a.st_size,'source changed during one-FD read')
        return b''.join(chunks)
    finally:os.close(fd)


def fact(raw):return {'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}


def run(args):
    source=Path(args.source_root);catalog=Path(args.catalog);out=Path(args.out)
    need(source.is_absolute() and source.resolve(strict=True)==source,'physical source root required')
    need(catalog.is_absolute() and catalog.resolve(strict=True)==catalog,'physical original catalog required')
    need(out.is_absolute() and out.parent.resolve(strict=True)==out.parent and
         not out.exists() and not out.is_symlink() and not out.is_relative_to(source),
         'fresh output outside the source SDK required')
    roles={'parent':source/'sdk/tools/custom_types.py',
           'bounded':source/'sdk/custom-types/bounded_profile.py',
           'adapter':source/'sdk/robots/industrial/vendor_profile.py',
           'exporter':source/'tests/ros_custom_types/bounded-v2/native_identity.cc'}
    original={name:read(path) for name,path in roles.items()}
    for name in ('parent','bounded','adapter'):
        need(fact(original[name])['sha256']==getattr(args,name+'_sha256'),
             'reviewed exact source SHA differs: '+name)
    module=types.ModuleType('_uwvm_industrial_selected_adapter')
    module.__file__=str(roles['adapter'])
    # No importlib/PYC/plugin path. This is the exact explicitly paired source.
    exec(compile(original['adapter'],module.__file__,'exec',dont_inherit=True),module.__dict__)
    templates={n:read(roles['bounded'].parent/n) for n in
               ('interface_templates/bounded_interfaces.'+e+'.in' for e in ('rs','mjs','go','ts'))}
    api=module.API(original['parent'],original['bounded'],parent_path=roles['parent'],
        bounded_path=roles['bounded'],parent_sha256=args.parent_sha256,
        bounded_sha256=args.bounded_sha256,templates=templates,
        adapter_raw=original['adapter'],adapter_sha256=args.adapter_sha256)
    catalog_raw=read(catalog,2*1024*1024);document=module.strict_json(catalog_raw)
    need(type(document)is dict and set(document)=={'format','types'} and document['format']==module.PROFILE and
         type(document['types'])is list and 1<=len(document['types'])<=module.MAX_TYPES,'complete industrial catalog required')
    captures={};total=len(catalog_raw)
    for row in document['types']:
        need(type(row)is dict and type(row.get('file'))is str,'explicit definition source role required')
        name=row['file'];p=Path(name)
        need(not p.is_absolute() and all(part not in ('','.','..') for part in name.split('/')) and
             '\\'not in name and '\0'not in name and name not in captures,'closed unique relative definition required')
        physical=(catalog.parent/p).resolve(strict=True)
        need(physical.is_relative_to(catalog.parent),'definition escaped original source root')
        raw=read(physical,1024*1024);captures[name]=(physical,raw);total+=len(raw)
    need(total<=64*1024*1024,'whole original source closure exceeds64MiB')
    # Full parse/resolution before the first output effect, including all source
    # constants/defaults/member maps/expanded nodes and independent codec domain.
    api.load_catalog(catalog,False)
    metadata=owner=source_proof=None
    source_proof_path=getattr(args,'installed_source_proof',None)
    if args.mode=='bind':
        need(args.native_metadata and args.native_owner_report,'bind requires original metadata and independent original owner DATA')
        metadata=read(Path(args.native_metadata));owner=read(Path(args.native_owner_report))
        if source_proof_path:
            source_proof=read(Path(source_proof_path))
        document,binding=api.bind(catalog,metadata,installed_source_proof=source_proof)
    else:
        need(not args.native_metadata and not args.native_owner_report and not source_proof_path,
             'prepare cannot adopt native evidence implicitly')
        binding=None
    out.mkdir(mode=0o700);inputs=out/'input';inputs.mkdir(mode=0o700)
    def write(path,raw):
        path.parent.mkdir(parents=True,exist_ok=True)
        fd=os.open(path,os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o600)
        try:
            with os.fdopen(fd,'wb')as f:fd=None;f.write(raw)
        finally:
            if fd is not None:os.close(fd)
    for name,(path,raw)in captures.items():write(inputs/name,raw)
    write(inputs/'catalog.json',module.canonical(document))
    if metadata is not None:
        write(out/'original-native-metadata.json',metadata);write(out/'original-native-owner-report.json',owner)
    if source_proof is not None:
        write(out/'original-installed-source-proof.json',source_proof)
    proof=api.generate(inputs/'catalog.json',out/'generated',guest_only=args.mode!='bind',helper_source=original['exporter'])
    for name,path in roles.items():need(read(path)==original[name],'current source drift before final receipt: '+name)
    need(read(catalog)==catalog_raw,'original catalog drift before final receipt')
    for path,raw in captures.values():need(read(path)==raw,'original definition drift before final receipt')
    for name,raw in templates.items():need(read(roles['bounded'].parent/name)==raw,'actual RPC template drift')
    receipt={'kind':'industrial-complete-source-preparation-not-acceptance','mode':args.mode,
             'default_authority':[],'native_delivery_qualified':False,'hardware_qualified':False,
             'source_preparation_executed':True,'compiler_executed':False,'Guest_executed':False,
             'original_source_inputs':{n:fact(b)for n,b in original.items()},
             'original_catalog':dict(path=str(catalog),**fact(catalog_raw)),
             'original_definitions':{n:dict(path=str(p),**fact(b))for n,(p,b)in captures.items()},
             'binding':binding,'generation':proof,
             'native_metadata':fact(metadata)if metadata is not None else None,
             'native_owner_report':fact(owner)if owner is not None else None}
    if source_proof is not None:
        receipt['installed_source_proof']=fact(source_proof)
    write(out/'preparation.vendor.json',module.canonical(receipt))
    return receipt


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source-root',required=True);p.add_argument('--catalog',required=True)
    p.add_argument('--out',required=True);p.add_argument('--mode',choices=['prepare','bind'],default='prepare')
    for name in ('parent','bounded','adapter'):p.add_argument('--'+name+'-sha256',required=True)
    p.add_argument('--native-metadata');p.add_argument('--native-owner-report')
    p.add_argument('--installed-source-proof',help='Explicit original pre-RIHS Message or complete Service/Action protocol source proof; never a modern metadata fallback')
    run(p.parse_args())

if __name__=='__main__':main()
