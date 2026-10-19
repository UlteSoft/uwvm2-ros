#!/usr/bin/env python3
"""Final binary import signature gate. Does not execute Wasm or prove authority.

Only the existing bounded core Wasm reader is reused from exact captured bytes.
The raw12 catalog, original ABI and current Host/source bindings remain pinned.
No ordinary import, SourceFileLoader or ambient pyc selects a candidate helper.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types

def sha(b):return hashlib.sha256(b).hexdigest()
def read(path,maximum):
    fd=os.open(path,os.O_RDONLY|os.O_CLOEXEC|os.O_NONBLOCK|os.O_NOFOLLOW)
    try:
        first=os.fstat(fd)
        if not stat.S_ISREG(first.st_mode)or first.st_size>maximum:raise ValueError('bounded ordinary file required')
        parts=[];n=0
        while n<=first.st_size:
            b=os.read(fd,min(65536,first.st_size+1-n))
            if not b:break
            parts.append(b);n+=len(b)
        def key(s):return tuple(getattr(s,'st_'+x)for x in('dev','ino','mode','nlink','size','mtime_ns','ctime_ns'))
        if n!=first.st_size or key(first)!=key(os.fstat(fd))or key(first)!=key(os.lstat(path)):raise ValueError('file changed during admission')
        return b''.join(parts)
    finally:os.close(fd)
def strict(b):
    def pairs(rows):
        d={}
        for k,v in rows:
            if k in d:raise ValueError('duplicate JSON key')
            d[k]=v
        return d
    return json.loads(b,object_pairs_hook=pairs)
def check(root,wasm,helper_sha,external_path):
    helper_path=root/'sdk/tools/check_wasm_imports.py';raw=read(helper_path,1048576)
    if sha(raw)!=helper_sha:raise ValueError('explicit reviewed full Wasm reader SHA required')
    catalog_path=root/'sdk/abi/rmw-service-v1.json';catalog_raw=read(catalog_path,1048576)
    if sha(catalog_raw)!='9e743ad0077cfab6d96bd9ca37d4d23a8804d9c2593af70678b6406222b9ac18':raise ValueError('raw12 catalog differs')
    c=strict(catalog_raw)
    for row in(c['authority_source'],c['implementation_source'],c['raw_header']):
        if sha(read(root/row['path'],1048576))!=row['sha256']:raise ValueError('current Host/raw implementation binding differs')
    original_raw=read(root/'sdk/abi/abi-v1.json',1048576)
    if sha(original_raw)!='0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0':raise ValueError('original81 catalog differs')
    original=strict(original_raw);expected={};required=set()
    for row in original['modules']['uwvm_ros2_v1']['functions']:
        expected['uwvm_ros2_v1',row['name']]=([p['type']for p in row['params']],row['results'])
    for row in c['modules']['uwvm_ros2_rmw_srv_v1']['functions']:
        k=('uwvm_ros2_rmw_srv_v1',row['name']);expected[k]=(row['params'],row['results']);required.add(k)
    external=strict(read(external_path,1048576));allowed={}
    if not isinstance(external,list):raise ValueError('explicit external full-signature rows required')
    for row in external:
        if set(row)!={'module','name','kind','params','results'}or row['module'].startswith('uwvm_ros')or row['kind']!=0:raise ValueError('explicit nonROS function signatures only')
        k=(row['module'],row['name'])
        if k in allowed:raise ValueError('duplicate external signature')
        allowed[k]=(row['params'],row['results'])
    name='_uwvm_csharp_final_binary_reader'
    if name in sys.modules:raise ValueError('private module namespace occupied')
    module=types.ModuleType(name);module.__file__=str(helper_path);sys.modules[name]=module
    data=read(wasm,128*1024*1024)
    try:
        exec(compile(raw,str(helper_path),'exec'),module.__dict__)
        imports=module.inspect(data)
    finally:sys.modules.pop(name,None)
    seen=set();seen_ros=set()
    for row in imports:
        k=(row['module'],row['name'])
        if k in seen or row['kind']!=0:raise ValueError('duplicate/nonfunction import refused by this exact variant')
        seen.add(k);pair=(row['params'],row['results'])
        if row['module'].startswith('uwvm_ros'):
            if k not in expected or pair!=expected[k]:raise ValueError('wrong backend/unselected namespace/full signature')
            seen_ros.add(k)
        elif k not in allowed or pair!=allowed[k]:raise ValueError('external import lacks exact explicit signature')
    if seen_ros&required!=required or len(required)!=12:raise ValueError('complete actual raw12 imports required')
    # A terminal source re-read binds the actual bytes executed above, not pyc.
    if read(helper_path,1048576)!=raw or read(catalog_path,1048576)!=catalog_raw:raise ValueError('checker inputs changed')
    return {'status':'BINARY_SIGNATURES_ONLY','runtime_or_native_credit':False,'wasm_sha256':sha(data),'reader_sha256':sha(raw),'raw12_catalog_sha256':sha(catalog_raw),'imports':imports}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--wasm',type=Path,required=True);p.add_argument('--reader-sha256',required=True);p.add_argument('--external-signatures',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
    try:
        result=check(a.source_root.resolve(strict=True),a.wasm,a.reader_sha256,a.external_signatures)
        with a.report.open('x')as f:f.write(json.dumps(result,indent=2)+'\n')
    except Exception as e:print(str(e),file=sys.stderr);return 2
    return 0
if __name__=='__main__':raise SystemExit(main())
