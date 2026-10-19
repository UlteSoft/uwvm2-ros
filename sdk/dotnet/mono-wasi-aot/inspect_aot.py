"""Actual artifact inspection, unexecuted author Source. No synthetic PASS.

Called with the actual admitted producer/owner. It parses emitted DATA, not
managed assemblies as executable code. LLVM and whole-validator are real owned
commands. AOT evidence is never a promise that every managed method is compiled.
"""
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys
import types

LEGACY_SHA='0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0'
RAW12_SHA='9e743ad0077cfab6d96bd9ca37d4d23a8804d9c2593af70678b6406222b9ac18'

PRIMARY={
 'WasiApp.targets':'f33bd602481884f53be365cbbca36d2133c88988cb79ba76a5da40d8d805d1bf',
 'WasmApp.Common.targets':'0efdfb2b818b20acdaf8a51d362c834b71ee6d77703af197538f4ef7c8c6c77d',
 'main.c':'46f172b1b0981f587463866dbeec306d520384517ba75610a18771da0b0cb943',
 'driver.c':'d2974daabd0b9110351d125a2dec5e8b510030477e28d0bdad3aa62891f8cde8',
 'runtime.c':'3d6d98fa1c5819b4ab89920be85fa3429b3f0bc32e035cc4f100791dbef3d00b',
 'MonoAOTCompiler.cs':'a1926c1271edbe3e306513a0c3cb7820f71b4f5c292af5f1097ff054f2ec9c42'}

def sha(b):return hashlib.sha256(b).hexdigest()
def data(path,maximum=128<<20):
    p=Path(path)
    fd=os.open(p,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
    try:
        first=os.fstat(fd)
        if not stat.S_ISREG(first.st_mode)or not 0<=first.st_size<=maximum:raise ValueError('actual artifact must be a bounded regular file')
        pieces=[];n=0
        while n<=first.st_size:
            part=os.read(fd,min(65536,first.st_size+1-n))
            if not part:break
            pieces.append(part);n+=len(part)
        keys=('st_dev','st_ino','st_mode','st_nlink','st_size','st_mtime_ns','st_ctime_ns')
        last,named=os.fstat(fd),os.lstat(p)
        if n!=first.st_size or any(getattr(first,k)!=getattr(last,k)or getattr(first,k)!=getattr(named,k)for k in keys):raise ValueError('artifact extent/identity changed')
        return b''.join(pieces)
    finally:os.close(fd)
def text(path):return data(path,64<<20).decode('utf-8-sig','strict')
def tsv(path,width):
    result=[]
    for line in text(path).splitlines():
        fields=line.split('|')
        if len(fields)!=width or not fields[0]:raise ValueError('actual item snapshot shape differs')
        result.append(fields)
    if not result:raise ValueError('empty actual MSBuild item evidence')
    return result
def bounded(path,stage):
    p=Path(path)
    if not p.is_absolute()or p.resolve(strict=True)!=p or not p.is_relative_to(stage):raise ValueError('actual output escaped fresh producer stage')
    return p
def pin(path):
    b=data(path);return {'path':str(path),'bytes':len(b),'sha256':sha(b)}

class Reader:
    def __init__(self,raw):self.b=raw;self.at=0
    def take(self,n):
        if n<0 or self.at+n>len(self.b):raise ValueError('bounded Wasm DATA extent')
        out=self.b[self.at:self.at+n];self.at+=n;return out
    def u(self):
        value=0
        for i in range(5):
            b=self.take(1)[0]
            if i==4 and b&0xf0:raise ValueError('u32 overflow')
            value|=(b&127)<<(7*i)
            if not b&128:return value
        raise ValueError('unterminated LEB')
    def string(self):return self.take(self.u()).decode('utf-8','strict')
    def end(self):
        if self.at!=len(self.b):raise ValueError('trailing actual section data')

def wasm_data(raw,require_code=True):
    r=Reader(raw)
    if r.take(8)!=b'\0asm\x01\0\0\0':raise ValueError('actual core Wasm v1 required, not component/P2 container')
    custom={};names={};body_count=0;nontrivial=0
    while r.at<len(raw):
        kind=r.take(1)[0];section=Reader(r.take(r.u()))
        if kind==0:
            name=section.string();payload=section.take(len(section.b)-section.at)
            custom.setdefault(name,[]).append(len(payload))
            if name=='name':
                sub=Reader(payload)
                while sub.at<len(sub.b):
                    sid=sub.take(1)[0];part=Reader(sub.take(sub.u()))
                    if sid==1:
                        count=part.u()
                        if count>1000000:raise ValueError('actual function name budget')
                        for _ in range(count):
                            index=part.u();label=part.string()
                            if index in names:raise ValueError('duplicate actual function name')
                            names[index]=label
                        part.end()
        elif kind==10:
            body_count=section.u()
            if body_count>1000000:raise ValueError('actual code body budget')
            for _ in range(body_count):
                body=Reader(section.take(section.u()));locals_count=body.u()
                if locals_count>1000000:raise ValueError('actual local count budget')
                for _ in range(locals_count):body.u();body.take(1)
                code=body.take(len(body.b)-body.at)
                if not code or code[-1]!=0x0b:raise ValueError('empty/unclosed actual body')
                if len(code)>1:nontrivial+=1
            section.end()
    if require_code and(not body_count or not nontrivial):raise ValueError('no actual emitted instruction bodies')
    return {'core_version':1,'code_bodies':body_count,'nontrivial_bodies':nontrivial,'custom_sections':custom,'actual_function_names':names}

def inspect(a,runner,root,build,evidence,env,language,ros,mode,case='application'):
    v=a.value;stage=build.parent;tools=v['tools'];actual_primary={}
    if set(v['primary_map'])!=set(PRIMARY):raise ValueError('complete actual selected primary Source map required')
    for role,expected in PRIMARY.items():
        path=Path(v['primary_map'][role]);b=data(path,8<<20)
        if str(path)not in a.rows or sha(b)!=expected or a.rows[str(path)]['sha256']!=expected:raise ValueError('actual target/task/runtime Source differs from selected commit')
        actual_primary[role]=pin(path)
    selected=dict(tsv(evidence/'selected-tools.tsv',2))
    for key,tool in [('cross','crosscompiler'),('clang','clang')]:
        if Path(selected[key]).resolve(strict=True)!=Path(tools[tool]):raise ValueError('actual MSBuild tool differs from admitted '+key)
    if (Path(selected['pack']).resolve(strict=True)!=Path(v['runtime_pack_root'])or selected['mode']!=mode or selected['runtime']!='9.0.0-preview.6.24327.7'or selected['should-aot']!='true'or selected['single-file']!='true'):
        raise ValueError('actual target/runtime/mode differs or AOT skipped')
    assemblies=tsv(evidence/'aot-assemblies.tsv',3);objects=tsv(evidence/'bitcode-objects.tsv',2)
    driver=text(evidence/'driver-gen.c');pinvoke=text(evidence/'pinvoke-table.h');link=text(evidence/'clang-link.rsp')
    compile_rsp=text(evidence/'wasi-compile.rsp')
    if any(flag not in compile_rsp for flag in('-DENABLE_AOT=1','-DDRIVER_GEN=1','-DWASM_SINGLE_FILE=1')):raise ValueError('actual runtime object compilation did not select real AOT registration/single-file flags')
    if ('#define EE_MODE_LLVMONLY_INTERP' in driver)!=(mode=='LLVMOnlyInterp'):raise ValueError('actual registration runtime mode differs')
    if mode=='LLVMOnly'and not re.search(r'^#define EE_MODE_LLVMONLY\s+1\s*$',driver,re.M):raise ValueError('actual full-runtime-mode definition absent')
    registrations=re.findall(r'mono_aot_register_module\s*\(\s*(mono_aot_module_[A-Za-z0-9_]+_info)\s*\)',driver)
    if not registrations or len(set(registrations))!=len(registrations):raise ValueError('actual unique module registration calls absent')
    linkmap=text(evidence/'native-link.map')
    if any(symbol not in linkmap for symbol in registrations):raise ValueError('registered actual module metadata not retained by the real final link map')
    linked=[line.strip()for line in text(evidence/'linked-items.tsv').splitlines()if line.strip()]
    source_objects=tsv(evidence/'runtime-source-objects.tsv',2)
    for role in('main.c','driver.c','runtime.c'):
        actual=[row for row in source_objects if Path(row[0]).name==role]
        if len(actual)!=1 or sha(data(actual[0][0],8<<20))!=PRIMARY[role]:raise ValueError('actual compiled runtime driver/main Source differs')
        obj=bounded(actual[0][1],stage);wasm_data(data(obj));
        if str(obj)not in link:raise ValueError('actual runtime object not present in real link response')
    if ros:
        for name in('roswasm_bridge','roswasm_owned_bridge'):
            path=build/(name+'.o')
            if linked.count(str(path))!=1 or str(path)not in link or name not in pinvoke:raise ValueError('actual bridge must be scanned and linked exactly once')
            wasm_data(data(path))
        sdk_assembly='Uwvm.RosWasm.Aot.dll'if case=='application'else'uwvm-aot-owned-faults.dll'
        if sdk_assembly not in {Path(row[0]).name for row in assemblies}:raise ValueError('actual full SDK assembly absent from AOT inputs')
    app_names={'csharp':'uwvm-aot-csharp','fsharp':'uwvm-aot-fsharp','visualbasic':'uwvm-aot-visualbasic'}
    app=app_names[language]+(''if ros else'-nonros')+'.dll'
    if case=='guest-faults':app='uwvm-aot-owned-faults.dll'
    by_bc={str(bounded(bc,stage)):(Path(assembly),forced)for assembly,bc,forced in assemblies if bc}
    without_bitcode=[{'assembly':assembly,'forced_interpret':forced,'method_credit':False}for assembly,bc,forced in assemblies if not bc]
    if language=='fsharp'and 'FSharp.Core.dll'not in {p.name for p,_ in by_bc.values()}:raise ValueError('actual FSharp.Core classlib has no compiler-produced bitcode')
    if app not in {p.name for p,_ in by_bc.values()}:raise ValueError('actual application assembly has no compiler-produced bitcode')
    if any(forced.lower()=='true'for _,_,forced in assemblies):raise ValueError('forced-interpret assembly cannot be silently presented as this qualified full input set')
    seen=set();defined=set();compiled=[]
    for i,(bc_name,obj_name)in enumerate(objects):
        bc=bounded(bc_name,stage);obj=bounded(obj_name,stage)
        if str(bc)in seen or str(bc)not in by_bc or str(obj)not in linked or str(obj)not in link:raise ValueError('unassociated/repeated actual AOT bitcode/object/link input')
        seen.add(str(bc));b=data(bc)
        if b[:4]not in(b'BC\xc0\xde',b'\xde\xc0\x17\x0b'):raise ValueError('actual LLVM bitcode magic absent')
        assembly,_=by_bc[str(bc)]
        info=wasm_data(data(obj),require_code=assembly.name in(app,'Uwvm.RosWasm.Aot.dll'))
        runner.run('object-symbols-'+str(i),[tools['llvm-nm'],'--defined-only','--format=posix',obj],root,env,300)
        symbols=text(stage/'commands'/('object-symbols-'+str(i))/'stdout.log')
        object_defined=set(re.findall(r'^(\S+)\s+[A-Za-z]\s+',symbols,re.M));defined.update(object_defined)
        if not object_defined:raise ValueError('empty actual object symbol inspection')
        row={'assembly':pin(assembly),'llvm_bitcode':pin(bc),'object':pin(obj),'defined_symbol_count':len(object_defined),'defined_symbols':sorted(object_defined),'code_bodies':info['code_bodies'],
            'metadata_only_object':info['code_bodies']==0,'metadata_only_credit':'No method coverage inferred for a legitimate forwarding/metadata-only assembly; real app and SDK require instruction bodies.'}
        # Concrete IR method evidence for the real app and complete SDK. Other
        # assemblies retain actual object symbol coverage, never "all methods".
        if assembly.name in(app,'Uwvm.RosWasm.Aot.dll'):
            ir=evidence/('assembly-'+str(i)+'.ll')
            runner.run('decode-real-bitcode-'+str(i),[tools['llvm-dis'],bc,'-o',ir],root,env,300)
            source=text(ir)
            functions=re.findall(r'^define\s+.*?@([^\s(]+)\(',source,re.M)
            if not functions:raise ValueError('no actual LLVM method definitions')
            expected='CoverageSentinel'if assembly.name=='Uwvm.RosWasm.Aot.dll'or language=='csharp'and not ros else 'frameThree'if language=='fsharp'and not ros else 'FrameThree'if not ros else 'Main'
            if not any(expected.lower()in name.lower()for name in functions):raise ValueError('actual expected live method absent from emitted LLVM IR')
            row['IR']=pin(ir);row['actual_IR_function_definitions']=functions
        compiled.append(row)
    if set(by_bc)!=seen or not set(registrations)<=defined:raise ValueError('actual compiled object set/module definitions do not cover real registration table')
    final=evidence/'final.wasm';final_info=wasm_data(data(final))
    if any(not final_info['custom_sections'].get(section)or not any(final_info['custom_sections'][section])for section in('.debug_info','.debug_line')):raise ValueError('native debug data absent; this is not a source-line/locals PASS')
    runner.run('final-sections',[tools['llvm-objdump'],'--section-headers',final],root,env,300)
    runner.run('whole-core-wasm-validation',[tools['validator'],'validate',final],root,env,300)
    # Bind final imports to the actual captured existing bounded reader; no PYC.
    helper=root/'sdk/tools/check_wasm_imports.py';raw=a.source['sdk/tools/check_wasm_imports.py'][0]
    name='_uwvm_mono_aot_final_core_reader'
    if name in sys.modules:raise ValueError('private reader namespace occupied')
    module=types.ModuleType(name);module.__file__=str(helper)
    module.__package__='';module.__cached__=module.__spec__=module.__loader__=None
    sys.modules[name]=module
    try:exec(compile(raw,str(helper),'exec',dont_inherit=True),module.__dict__);imports=module.inspect(data(final))
    finally:sys.modules.pop(name,None)
    startup=v['p1_startup_signatures'];allowed={}
    for row in startup:
        if set(row)!={'module','name','kind','params','results'}or row['module']!='wasi_snapshot_preview1'or row['kind']!=0:raise ValueError('only exact actual P1 startup function signatures, no JS/env/browser/component host')
        key=(row['module'],row['name'])
        if key in allowed:raise ValueError('duplicate admitted startup signature')
        allowed[key]=(row['params'],row['results'])
    expected=dict(allowed)
    if ros:
        original_raw=data(root/'sdk/abi/abi-v1.json',1<<20)
        if sha(original_raw)!=LEGACY_SHA:raise ValueError('original complete81 ABI catalog differs')
        original=json.loads(original_raw);catalog_raw=data(root/'sdk/abi/rmw-service-v1.json',1<<20)
        if sha(catalog_raw)!=RAW12_SHA or v['raw12_catalog_sha256']!=RAW12_SHA:raise ValueError('explicit admitted exact raw12 catalog differs')
        catalog=json.loads(catalog_raw)
        for key in('authority_source','implementation_source','raw_header'):
            row=catalog[key]
            if sha(data(root/row['path'],1<<20))!=row['sha256']:raise ValueError('catalog must match actual staged authority/implementation/header; no stale ledger bypass')
        for f in original['modules']['uwvm_ros2_v1']['functions']:expected['uwvm_ros2_v1',f['name']]=([p['type']for p in f['params']],f['results'])
        for f in catalog['modules']['uwvm_ros2_rmw_srv_v1']['functions']:expected['uwvm_ros2_rmw_srv_v1',f['name']]=(f['params'],f['results'])
    actual={}
    for row in imports:
        key=(row['module'],row['name'])
        if key in actual or row['kind']!=0:raise ValueError('duplicate/nonfunction final import')
        actual[key]=(row['params'],row['results'])
    if actual!=expected:raise ValueError('final exact full P1+selected ROS import set differs; no namespace wildcard')
    debug_files=[pin(p)for p in sorted(build.rglob('*.pdb'))]
    if not debug_files:raise ValueError('real portable managed PDB provenance absent')
    return {'status':'ACTUAL_BUILD_ARTIFACTS_ONLY_NOT_GUEST_NATIVE_OR_DEBUG_QUALIFICATION','requested_mode':mode,
        'actual_primary_source_bindings':actual_primary,'actual_selected_tools':selected,'compiled_assemblies':compiled,
        'input_assemblies_without_bitcode_no_method_credit':without_bitcode,
        'registered_module_info_symbols':registrations,'final':pin(final),'final_module_data':final_info,'final_imports':imports,
        'actual_PDB_files':debug_files,'actual_link_map':pin(evidence/'native-link.map'),
        'method_coverage_statement':'Actual object symbols and selected real LLVM function definitions only. LLVMOnlyInterp permits interpreter fallback. LLVMOnly is an actual requested/registered full runtime mode, not evidence every managed/generic/dynamic method compiled or ran.',
        'debug_statement':'PDB/DWARF/source/object provenance retained. Real UWVM source breakpoints/stepping, typed managed locals/argv/frames and GC unwind still require implementation and actual admitted Linux debugger cases; section existence is not a debugging PASS.',
        'external_tcb_statement':'Binary compiler/runtime/classlib/loader authenticity and source-build provenance require independently admitted real artifacts. This producer output/self-manifest is not that trust root.'}
