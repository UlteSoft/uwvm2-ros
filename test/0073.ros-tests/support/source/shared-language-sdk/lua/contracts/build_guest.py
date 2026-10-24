#!/usr/bin/env python3
"""Build a real frozen Lua 5.4.8 ROS Guest under the shared finite owner.

This SDK consumer recipe changes no language compiler, debugger, installed SDK
or upstream Lua source. Its explicit --download-source input is SHA256 pinned.
The interpreter, complete record package, native bridge and candidate program
are compiled into the same core-Wasm command. All callbacks remain in Wasm.
"""

import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tarfile
import urllib.request

LUA_URL='https://www.lua.org/ftp/lua-5.4.8.tar.gz'
LUA_SHA256='4f18ddae154e793e46eeab727c59ef1c0c0c2b744e7b94219710d76f530629ae'
SOURCES='lapi lcode lctype ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib liolib lmathlib lstrlib ltablib lutf8lib'.split()


def fact(path):
    with path.open('rb') as stream:
        return {'path':str(path),'bytes':path.stat().st_size,'sha256':hashlib.file_digest(stream,'sha256').hexdigest()}


def frozen(modules,program):
    lines=['#include <lua.h>','#include <lauxlib.h>','#include <lualib.h>',
           '#include <stdio.h>','#include <string.h>',
           'int luaopen_roswasm_native(lua_State*);']
    for index,(name,body) in enumerate([*modules.items(),('application',program)]):
        lines+=['static unsigned char const source_'+str(index)+'[]={'+','.join(str(b) for b in body)+'};']
    lines+=['static int frozen_require(lua_State *L){','const char *name=luaL_checkstring(L,1);',
        'luaL_getsubtable(L,LUA_REGISTRYINDEX,LUA_LOADED_TABLE);lua_getfield(L,-1,name);',
        'if(!lua_isnil(L,-1))return 1;lua_pop(L,1);',
        'if(strcmp(name,"roswasm_native")==0){luaL_requiref(L,name,luaopen_roswasm_native,0);return 1;}']
    for index,(name,body) in enumerate(modules.items()):
        lines+=['if(strcmp(name,'+json.dumps(name)+')==0){',
            'if(luaL_loadbufferx(L,(char const*)source_'+str(index)+',sizeof(source_'+str(index)+'),'+json.dumps('@frozen/'+name)+',"t")!=LUA_OK)return lua_error(L);',
            'lua_call(L,0,1);lua_pushvalue(L,-1);lua_setfield(L,-3,name);return 1;}']
    lines+=['return luaL_error(L,"module is outside the exact frozen package");}',
        'int main(void){lua_State *L=luaL_newstate();if(!L)return 2;',
        'luaL_requiref(L,"_G",luaopen_base,1);lua_pop(L,1);',
        'luaL_requiref(L,"string",luaopen_string,1);lua_pop(L,1);',
        'luaL_requiref(L,"table",luaopen_table,1);lua_pop(L,1);',
        'luaL_requiref(L,"math",luaopen_math,1);lua_pop(L,1);',
        'luaL_requiref(L,"utf8",luaopen_utf8,1);lua_pop(L,1);',
        'luaL_requiref(L,"io",luaopen_io,1);lua_pop(L,1);',
        'lua_pushcfunction(L,frozen_require);lua_setglobal(L,"require");',
        'int code=luaL_loadbufferx(L,(char const*)source_'+str(len(modules))+',sizeof(source_'+str(len(modules))+'),"@frozen/application","t");',
        'if(!code)code=lua_pcall(L,0,0,0);',
        'if(code)fprintf(stderr,"Lua Guest failed: %s\\n",lua_tostring(L,-1));',
        'lua_close(L);return code?1:0;}']
    return '\n'.join(lines)+'\n'


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True)
    p.add_argument('--catalog',type=Path,required=True)
    p.add_argument('--wasi-sdk',type=Path,required=True)
    p.add_argument('--validator',type=Path,required=True)
    p.add_argument('--runtime',type=Path,required=True)
    p.add_argument('--contract',type=Path,default=Path(__file__).resolve().with_name('contract.lua'),
        help='Explicit relocated Guest contract; the canonical co-located support source is the default')
    p.add_argument('--toolchain-receipt',type=Path,required=True)
    p.add_argument('--toolchain-receipt-sha256',required=True)
    p.add_argument('--backend',choices=('1','2'),required=True)
    p.add_argument('--exceptions',choices=('on','off'),required=True)
    p.add_argument('--out',type=Path,required=True)
    group=p.add_mutually_exclusive_group(required=True)
    group.add_argument('--archive',type=Path)
    group.add_argument('--download-source',action='store_true')
    args=p.parse_args()
    if os.uname().sysname!='Linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD')!='1':raise ValueError('admitted Linux owner required')
    sdk=args.source.resolve(strict=True)/'sdk/origin';args.out.mkdir(exist_ok=False)
    if args.exceptions!='on':
        raise ValueError('Lua C error recovery needs the selected WASI SDK setjmp/longjmp EH runtime; explicitly select the EH-on profile or refuse this route')
    report={'kind':'real-lua-ros-guest-candidate-v1','backend':int(args.backend),'default_authority':[],
            'native_ros':'NOT_RUN','gateway':'NOT_RUN','upstream_suite':'NOT_RUN','commands':[],
            'profile':{'target':'wasm32-wasip1','core_baseline':'3.0','exceptions':True,'threads':False,'startup':'command'},
            'required_runtime_features':['exceptions'],'setjmp_strategy':'actual WASI libsetjmp plus LLVM wasm SJLJ lowering',
            'exception_encoding':'exnref-try-table'}
    def save(): (args.out/'build.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,argv):
        with (args.out/(name+'.stdout')).open('xb') as stdout,(args.out/(name+'.stderr')).open('xb') as stderr:
            result=subprocess.run(list(map(str,argv)),stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr,cwd=args.out,check=False)
        report['commands'].append({'name':name,'argv':list(map(str,argv)),'exit':result.returncode,
            'stdout':fact(args.out/(name+'.stdout')),'stderr':fact(args.out/(name+'.stderr'))})
        save()
        if result.returncode:raise RuntimeError('failed '+name+'; retained original logs')
    def observe_standard_eh(artifact):
        # Retain all original disassembly bytes losslessly without spending a
        # multi-megabyte plain-text copy of the same final binary.
        argv = [str(args.validator), 'print', str(artifact)]
        output = args.out / 'actual-wasm-text.stdout.gz'
        errors = args.out / 'actual-wasm-text.stderr'
        digest = hashlib.sha256()
        extent, found, tail = 0, False, b''
        with errors.open('xb') as stderr, output.open('xb') as target:
            with gzip.GzipFile(filename='', fileobj=target, mode='wb', mtime=0) as compressed:
                with subprocess.Popen(argv, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                      stderr=stderr, cwd=args.out) as child:
                    while block := child.stdout.read(65536):
                        extent += len(block)
                        if extent > 16 << 20:
                            child.kill()
                            raise ValueError('actual Lua disassembly exceeds the explicit byte bound')
                        found |= b'try_table' in tail + block
                        tail = block[-16:]
                        digest.update(block)
                        compressed.write(block)
                    result = child.wait()
        report['commands'].append(dict(name='actual-wasm-text', argv=argv, exit=result,
            stdout=fact(output), stderr=fact(errors), stdout_encoding='gzip',
            stdout_uncompressed_bytes=extent, stdout_uncompressed_sha256=digest.hexdigest()))
        save()
        if result or not found:
            raise ValueError('the actual standard-EH Lua closure must disassemble with try_table')
    save()
    receipt_raw=args.toolchain_receipt.read_bytes()
    if hashlib.sha256(receipt_raw).hexdigest()!=args.toolchain_receipt_sha256:
        raise ValueError('independently selected staged toolchain receipt differs')
    receipt=json.loads(receipt_raw)
    selected=[row for row in receipt['inputs'] if Path(row['output']).resolve()==args.wasi_sdk.resolve()]
    if len(selected)!=1:raise ValueError('exact staged WASI SDK root required')
    for pin in selected[0]['files']:
        relative=Path(pin['path'])
        if relative.is_absolute() or '..' in relative.parts:raise ValueError('confined toolchain pin required')
        path=args.wasi_sdk/relative
        row=fact(path)
        if row['bytes']!=pin['bytes'] or row['sha256']!=pin['sha256']:raise ValueError('full compiler/sysroot closure changed')
    report['complete_toolchain_receipt']=fact(args.toolchain_receipt)
    report['complete_toolchain_files_checked']=len(selected[0]['files']);save()
    archive=args.archive
    if archive is None:
        archive=args.out/'lua-5.4.8.tar.gz'
        with urllib.request.urlopen(LUA_URL,timeout=30) as source,archive.open('xb') as target:
            raw=source.read(1024*1024+1)
            if len(raw)>1024*1024:raise ValueError('Lua archive byte bound')
            target.write(raw)
    if fact(archive)['sha256']!=LUA_SHA256:raise ValueError('official pinned Lua source SHA256 differs')
    report['upstream']={'version':'5.4.8','url':LUA_URL,'archive':fact(archive),'license':'MIT; original source license retained'}
    unpack=args.out/'upstream';unpack.mkdir()
    with tarfile.open(archive) as package:
        if sum(max(0,m.size) for m in package.getmembers())>8*1024*1024:raise ValueError('Lua source extraction bound')
        package.extractall(unpack,filter='data')
    lua=unpack/'lua-5.4.8/src'
    run('stdlib-profile',[sys.executable,'-I','-S','-B',sdk/'lua/tools/stdlib_profile.py',
        '--source-dir',lua,'--out',args.out/'stdlib-profile'])
    run('records',[sys.executable,'-I','-B',sdk/'lua/tools/records.py',args.catalog,'--out',args.out/'records'])
    run('bridge',[sys.executable,'-I','-B',sdk/'lua/tools/bridge.py','--backend',args.backend,'--out',args.out/'bridge'])
    modules={'roswasm.wire':(args.out/'records/roswasm/wire.lua').read_bytes(),
             'bounded_records':(args.out/'records/bounded_records.lua').read_bytes(),
             'roswasm':(sdk/'lua/include/roswasm/init.lua').read_bytes()}
    args.contract=args.contract.resolve(strict=True)
    main_c=args.out/'frozen.c';main_c.write_text(frozen(modules,args.contract.read_bytes()))
    inputs=[lua/(name+'.c') for name in SOURCES]+[args.out/'bridge/native.c',main_c]
    inputs=[args.out/'stdlib-profile/lmathlib.c' if path.name=='lmathlib.c' else path for path in inputs]
    clang=args.wasi_sdk/'bin/clang';sysroot=args.wasi_sdk/'share/wasi-sysroot'
    report['tools']=[fact(clang),fact(args.validator),fact(args.runtime)]
    config=sdk/'lua/embedding/guest_config.h'
    report['stdlib_profile']=fact(args.out/'stdlib-profile/profile.json')
    report['source_inputs']=[fact(p) for p in sorted(lua.glob('*')) if p.is_file()]+[fact(sdk/'lua/include/roswasm/init.lua'),fact(args.contract),fact(config),fact(args.out/'stdlib-profile/lmathlib.c')]
    run('compiler-version',[clang,'--version'])
    run('validator-help',[args.validator,'validate','--help'])
    objects=[]
    for index,source in enumerate(inputs):
        output=args.out/('object-'+str(index)+'.o');objects.append(output)
        run('compile-'+str(index),[clang,'--target=wasm32-wasip1','--sysroot='+str(sysroot),'-std=c11','-O2','-g0',
            '-mexception-handling','-mllvm','-wasm-enable-sjlj',
            '-mllvm','-wasm-use-legacy-eh=false','-Dl_signalT=int',
            '-include',config,
            '-DROSWASM_BACKEND='+args.backend,'-I'+str(lua),'-I'+str(sdk),'-c',source,'-o',output])
    artifact=args.out/'lua-ros.wasm'
    run('link',[clang,'--target=wasm32-wasip1','--sysroot='+str(sysroot),'-O2','-mexception-handling',
        '-Wl,-mllvm,-wasm-use-legacy-eh=false',*objects,'-lm','-lsetjmp',
        '-Wl,--initial-memory=16777216','-Wl,--max-memory=134217728','-Wl,-Map='+str(args.out/'link.map'),'-o',artifact])
    run('validate',[args.validator,'validate',artifact])
    observe_standard_eh(artifact)
    run('imports',[sys.executable,'-I','-B',sdk/'tools/check_wasm_imports.py',artifact,'--backend',args.backend,
        '--allow-module','wasi_snapshot_preview1','--require-all','--report',args.out/'imports.json'])
    report['artifact']=fact(artifact);report['status']='BUILT_VALIDATED_NOT_EXECUTED';save()
    argv = [args.runtime, '--runtime-int', '-Rct', '0', '--wasm-feature-enable-exceptions',
        '--wasm-set-main-module-name', 'main', '--wasip1-global-disable',
        '--wasip1-global-noinherit-system-environment', '--wasip1-single-create', 'main',
        '--wasip1-single-enable', 'main', '--wasip1-single-hide-host-api', 'main',
        '--wasip1-single-noinherit-system-environment', 'main']
    for right in ('realtime-clock', 'monotonic-clock', 'process-clock', 'thread-clock',
                  'random', 'proc-raise', 'sched-yield', 'stdio-full-rights'):
        argv += ['--wasip1-single-deny', 'main', right]
    run('real-guest', [*argv, '--run', artifact])
    output=(args.out/'real-guest.stdout').read_text()
    if output.strip()!='PASS_LUA_REAL_GUEST_CHECKS=24':raise ValueError('exact actual Guest result/check denominator differs')
    report['status']='REAL_GUEST_LOCAL_VALUES_AND_DEFAULT_DENIAL_PASSED';report['guest_checks']=24;save()
    print(report['status'])


if __name__=='__main__':main()
