#!/usr/bin/env python3
"""Future Linux local binary DATA models, not Guest/native execution results.
External keeper first admits this entry, its stdlib and bounded process lifetime.
The checker/inspector and all real catalog sources are explicit SHA-bound.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import sys
import stat
import types

def require(ok):
    if not ok:raise AssertionError()
def capture(path,expected):
    fd=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
    try:
        first=os.fstat(fd);require(stat.S_ISREG(first.st_mode)and 0<=first.st_size<=2*1024*1024)
        parts=[];count=0
        while True:
            part=os.read(fd,min(65536,2*1024*1024-count+1))
            if not part:break
            count+=len(part);require(count<=2*1024*1024);parts.append(part)
        raw=b''.join(parts);last=os.fstat(fd);named=os.lstat(path);keys=('st_dev','st_ino','st_mode','st_size','st_mtime_ns','st_ctime_ns')
        require(count==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k)for k in keys)and hashlib.sha256(raw).hexdigest()==expected)
        return raw
    finally:os.close(fd)

def leb(n):
    out=bytearray()
    while n>=128:out.append((n&127)|128);n>>=7
    out.append(n);return bytes(out)
def text(s):
    b=s.encode();return leb(len(b))+b
def binary(rows):
    types_body=leb(len(rows));imports_body=leb(len(rows));values={'i32':b'\x7f','i64':b'\x7e','f32':b'\x7d','f64':b'\x7c'}
    for index,row in enumerate(rows):
        types_body+=b'\x60'+leb(len(row['params']))+b''.join(values[t]for t in row['params'])+leb(len(row['results']))+b''.join(values[t]for t in row['results'])
        imports_body+=text(row['module'])+text(row['name'])+b'\0'+leb(index)
    return b'\0asm\1\0\0\0'+b'\x01'+leb(len(types_body))+types_body+b'\x02'+leb(len(imports_body))+imports_body

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    for k in ('checker','inspector','catalog'):p.add_argument('--'+k+'-sha256',required=True)
    a=p.parse_args();require(sys.platform=='linux'and a.root.is_absolute()and a.out.is_absolute()and not a.out.exists());a.out.mkdir(parents=True)
    path=a.root/'sdk/java/owned-service/check_imports.py';raw=capture(path,a.checker_sha256)
    name='_uwvm_java_owned_model_'+os.urandom(16).hex();require(name not in sys.modules);m=types.ModuleType(name);m.__file__=str(path);m.__package__='';m.__loader__=m.__spec__=m.__cached__=None;sys.modules[name]=m
    try:
        exec(compile(raw,str(path),'exec',dont_inherit=True),m.__dict__)
        catalog=a.root/'sdk/abi/rmw-service-v1.json';doc=m.strict(m.capture(catalog,2*1024*1024,a.catalog_sha256)[0]);functions=m.rmw_catalog_contract(doc)
        wanted=[{'module':m.RMW_MODULE,'name':f['name'],'kind':0,'params':f['params'],'results':f['results']}for f in functions]
        policy=a.out/'policy.json';policy_raw=(json.dumps({'kind':'java-linear-wasi-exact-imports-v1','imports':wanted})+'\n').encode();policy.write_bytes(policy_raw);policy_sha=hashlib.sha256(policy_raw).hexdigest()
        def exercise(rows,accepted):
            wasm=a.out/('model-'+str(exercise.count)+'.wasm');exercise.count+=1;wasm.write_bytes(binary(rows));passed=False
            try:result=m.check(a.root,wasm,policy,policy_sha,catalog,a.catalog_sha256,a.inspector_sha256);passed=result['status']=='BINARY_IMPORT_CONTRACT_PASS'
            except ValueError:pass
            require(passed==accepted)
        exercise.count=0
        exercise(wanted,True)
        for index in range(12):
            mutated=json.loads(json.dumps(wanted));mutated[index]['name']+='wrong';exercise(mutated,False)
            mutated=json.loads(json.dumps(wanted));mutated[index]['results']=['i64'];exercise(mutated,False)
        exercise(wanted[:-1],False);exercise(wanted+[wanted[0]],False)
        mutated=json.loads(json.dumps(wanted));mutated[1]['params'][0]='i32';exercise(mutated,False)
        mutated=json.loads(json.dumps(wanted));mutated[1]['module']='uwvm_ros1_rmw_srv_v1';exercise(mutated,False)
        exercise(wanted+[{'module':'wasi_snapshot_preview1','name':'proc_exit','kind':0,'params':['i32'],'results':[]}],False)
        exercise(wanted+[{'module':'uwvm_ros2_v1','name':'capability_open','kind':0,'params':['i32'],'results':['i32']}],False)
        # Explicitly no Runtime/ROS/native/function-body validation claim.
        (a.out/'models.json').write_text(json.dumps({'cases':exercise.count,'kind':'LOCAL_BINARY_MODEL_ONLY','Guest_native_execution':False})+'\n')
    finally:
        if sys.modules.get(name)is m:del sys.modules[name]
if __name__=='__main__':main()
