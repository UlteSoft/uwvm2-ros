#!/usr/bin/env python3
"""Prepare a Source-only additive CPython/WASI recipe; never run its commands.

Run this operator only through the admitted Linux keeper. An externally pinned
Source closure lists every staged SDK .py/C/header and generated control product.
The exact retained original link argv supplies actual libpython/static libraries;
this adapter does not guess libraries, rebuild CPython or change old artifacts.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys

CATALOG_SHA = '9e743ad0077cfab6d96bd9ca37d4d23a8804d9c2593af70678b6406222b9ac18'
LEGACY_SHA = '0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0'


def read(path, maximum=8*1024*1024, expected=None):
    path = Path(path)
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise ValueError('resolved absolute admitted file required')
    fd = os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
    try:
        first=os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= maximum:
            raise ValueError('bounded ordinary input required')
        out=bytearray()
        while len(out) <= first.st_size:
            part=os.read(fd,min(65536,first.st_size+1-len(out)))
            if not part:break
            out.extend(part)
        last,named=os.fstat(fd),os.lstat(path)
        keys=('st_dev','st_ino','st_mode','st_size','st_mtime_ns','st_ctime_ns','st_nlink')
        if len(out)!=first.st_size or any(getattr(first,k)!=getattr(last,k) or getattr(first,k)!=getattr(named,k) for k in keys):
            raise ValueError('input changed during admission')
        raw=bytes(out)
        if expected is not None and hashlib.sha256(raw).hexdigest()!=expected:
            raise ValueError('admitted source SHA differs')
        return raw
    finally:os.close(fd)


def strict(raw):
    def pairs(rows):
        out={}
        for key,value in rows:
            if key in out:raise ValueError('duplicate JSON key')
            out[key]=value
        return out
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:(_ for _ in ()).throw(ValueError('nonfinite JSON')))


def prepare(root, stage, closure_path, closure_sha, original_path, original_sha,
            entry, entry_sha, startup, startup_sha, backend, enabled):
    if sys.platform!='linux':raise ValueError('all preparation/build/qualification belongs to the Linux keeper')
    if backend not in (1,2) or enabled not in (0,1) or enabled and backend!=2:
        raise ValueError('owned Service is explicitly default-off and ROS2-only')
    if not stage.is_absolute() or stage.resolve() != stage or stage.exists() or stage.is_relative_to(root) or root.is_relative_to(stage):
        raise ValueError('fresh separate stage required')
    closure_raw=read(closure_path,expected=closure_sha);closure=strict(closure_raw)
    if type(closure) is not dict or set(closure)!={'format','sources'} or closure['format']!='python-owned-service-source-closure-v1' or type(closure['sources']) is not list:
        raise ValueError('externally reviewed complete source closure required')
    sources={}
    for row in closure['sources']:
        if type(row) is not dict or set(row)!={'source','target','bytes','sha256'}:
            raise ValueError('exact complete source row required')
        target=Path(row['target'])
        if target.is_absolute() or '..' in target.parts or target.as_posix() in sources or not target.parts or target.parts[0]!='sdk':
            raise ValueError('unique staged SDK-relative target required')
        body=read(Path(row['source']),expected=row['sha256'])
        if type(row['bytes']) is not int or len(body)!=row['bytes']:
            raise ValueError('source extent differs')
        sources[target.as_posix()]=body
    required={'sdk/python/embedding/main.c','sdk/python/roswasm/_native.c','sdk/python/roswasm/_registry.py',
              'sdk/python/roswasm/raw.py','sdk/python/roswasm/client.py','sdk/python/tools/freeze.py',
              'sdk/ros1/abi.h','sdk/ros2/abi.h','sdk/common/abi.h','sdk/python/roswasm/__init__.py','sdk/python/roswasm/codecs.py','sdk/python/roswasm/events.py','sdk/python/tools/check_owned_service_imports.py',
              'sdk/tools/check_wasm_imports.py','sdk/abi/abi-v1.json'}
    if enabled:
        required|={'sdk/python/roswasm/_owned_service.c','sdk/python/roswasm/_open_witness.h','sdk/include/roswasm/rmw_service_v1.h',
                   'sdk/abi/rmw-service-v1.json','src/uwvm2/uwvm/imported/ros/host.h'}
        # Host support is read separately below and is not staged into SDK.
        required.remove('src/uwvm2/uwvm/imported/ros/host.h')
        required|={'sdk/python/roswasm/owned_service/'+name for name in ('__init__.py','bridge.py','wire.py','owned.py','control.py','applications.py','drive.py')}
        required|={'sdk/python/roswasm/generated_control/__init__.py',
                   'sdk/python/roswasm/generated_control/bounded_records.py',
                   'sdk/python/roswasm/generated_control/control_services.py',
                   'sdk/python/roswasm/generated_control/generation.python.json'}
    if not required <= sources.keys():raise ValueError('incomplete actual SDK/frozen/control source closure')
    if hashlib.sha256(sources['sdk/abi/abi-v1.json']).hexdigest()!=LEGACY_SHA:
        raise ValueError('legacy business81 catalog changed')
    if enabled:
        if hashlib.sha256(sources['sdk/abi/rmw-service-v1.json']).hexdigest()!=CATALOG_SHA:
            raise ValueError('complete raw12 catalog SHA differs')
        catalog=strict(sources['sdk/abi/rmw-service-v1.json'])
        for key in ('authority_source','implementation_source','raw_header'):
            pin=catalog[key];read(root/pin['path'],expected=pin['sha256'])
        proof=strict(sources['sdk/python/roswasm/generated_control/generation.python.json'])
        if len(proof.get('complete_records',[]))!=32 or len(proof.get('services',[]))!=7 or proof.get('native_registry_emitted')is not False:
            raise ValueError('complete generated control Source proof required')
        for name,pin in proof['outputs'].items():
            body=sources.get('sdk/python/roswasm/generated_control/'+name)
            if body is None or pin!={'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()}:
                raise ValueError('generated full record product identity differs')
    entry_raw=read(entry,expected=entry_sha);read(startup,65536,startup_sha)
    original_raw=read(original_path,64*1024*1024,original_sha);original=strict(original_raw)
    if original.get('passed')is not True:
        raise ValueError('retained successful CPython build recipe required')
    native=Path(original['native_python']['path']);read(native,512*1024*1024,original['native_python']['sha256'])
    cpython=Path(original['source']);wasi=Path(original['wasi_build'])
    clang=Path(original['sdk_clang']['path']);read(clang,512*1024*1024,original['sdk_clang']['sha256'])
    selected=[row for row in original['commands'] if row.get('passed')is True and row.get('retired')is True and type(row.get('argv'))is list
              and row['argv'][-1]==original['artifacts']['semantics-wasi']['path']]
    if len(selected)!=1:raise ValueError('one exact physical-retired original WASI link argv required')
    old=selected[0];argv=list(old['argv']);old_main=[i for i,value in enumerate(argv) if value.endswith('/embedded_main.c')]
    old_frozen='-I'+str(Path(original['artifacts']['semantics-wasi']['frozen_assets']).parent)
    if len(old_main)!=1 or argv.count(old_frozen)!=1 or argv[-2]!='-o' or any('HLL_EXTENSION_HEADER'in value for value in argv):
        raise ValueError('original source/link shape differs; do not guess a replacement')
    argv[old_main[0]]=str(stage/'sdk/python/embedding/main.c')
    argv[argv.index(old_frozen)]='-I'+str(stage/'frozen')
    flags=['-DROSWASM_BACKEND='+str(backend),'-DROSWASM_OWNED_SERVICE='+str(enabled)]
    objects=[stage/'native.o']+([stage/'owned-service.o']if enabled else[])
    argv[-2:-2]=flags+[str(path)for path in objects]
    argv[-1]=str(stage/'python-owned-service.wasm')
    common=[str(clang),'-O2','-g','-I'+str(wasi),'-I'+str(cpython/'Include'),
            '-I'+str(stage/'sdk'),'-I'+str(stage/'sdk/include')]+flags
    commands=[{'name':'freeze','argv':[str(native),'-I','-S',str(stage/'sdk/python/tools/freeze.py'),
               '--stdlib',str(cpython/'Lib'),'--sdk-python',str(stage/'sdk/python'),
               '--entry',str(stage/'entry.py'),'--out',str(stage/'frozen')],'cwd':str(wasi)}]
    commands.append({'name':'compile-legacy','argv':common+['-c',str(stage/'sdk/python/roswasm/_native.c'),'-o',str(objects[0])],'cwd':str(wasi)})
    if enabled:commands.append({'name':'compile-owned','argv':common+['-c',str(stage/'sdk/python/roswasm/_owned_service.c'),'-o',str(objects[1])],'cwd':str(wasi)})
    commands.append({'name':'link','argv':argv,'cwd':old['cwd']})
    commands.append({'name':'check-full-imports','argv':[str(native),'-I','-S',str(stage/'sdk/python/tools/check_owned_service_imports.py'),
        '--source-root',str(root),'--wasm',str(stage/'python-owned-service.wasm'),'--backend',str(backend),
        '--owned-service',str(enabled),'--startup-imports',str(startup),'--startup-sha256',startup_sha], 'cwd':str(wasi)})
    # Reopen every admitted Source BEFORE the first fresh-stage write. Native
    # executables/std C extension/startup and command execution stay keeper-owned.
    for row in closure['sources']:read(Path(row['source']),expected=row['sha256'])
    read(entry,expected=entry_sha);read(original_path,64*1024*1024,original_sha);read(closure_path,expected=closure_sha)
    stage.mkdir(parents=True,exist_ok=False)
    for target,body in sources.items():
        destination=stage/target;destination.parent.mkdir(parents=True,exist_ok=True)
        with destination.open('xb')as stream:stream.write(body)
    with(stage/'entry.py').open('xb')as stream:stream.write(entry_raw)
    report={'status':'PREPARED_SOURCE_COMMANDS_NOT_EXECUTED','owned_service':bool(enabled),'backend':backend,
        'closure_sha256':closure_sha,'original_build_receipt_sha256':original_sha,'entry_sha256':entry_sha,
        'commands':commands,'actual_command_executions':0,'require_existing_linux_64GiB_cgroup':True,
        'external_admission':'keeper owns exact entry/interpreter/compiler/std C library, finite deadlines, RSS, PID/cgroup retirement, full Wasm validation and all executions',
        'debug':'-g C DWARF is CPython/C, not Python source-locals; actual UWVM DBG and Python tracing require separate observations'}
    with(stage/'prepare.json').open('x')as stream:stream.write(json.dumps(report,indent=2)+'\n')
    return report


def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in('source-root','stage','closure','original-receipt','entry','startup-imports'):
        p.add_argument('--'+name,type=Path,required=True)
    for name in('closure','original-receipt','entry','startup'):
        p.add_argument('--'+name+'-sha256',required=True)
    p.add_argument('--backend',type=int,choices=(1,2),required=True)
    p.add_argument('--owned-service',type=int,choices=(0,1),default=0)
    a=p.parse_args()
    print(json.dumps(prepare(a.source_root,a.stage,a.closure,a.closure_sha256,a.original_receipt,a.original_receipt_sha256,
        a.entry,a.entry_sha256,a.startup_imports,a.startup_sha256,a.backend,a.owned_service),indent=2))


if __name__=='__main__':main()
