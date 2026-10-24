#!/usr/bin/env python3
"""Root-only actual Source40 SAME-MAIN typed command14 compiler/binary adapter.

No author has executed this candidate. Nine individually successful Source40
commands are retained from the exact preserved failed batch; five commands
(TinyGo3, C API and C++ API) compile fresh with whole native-bound modules. This worker starts compilers/validators only when Root explicitly invokes
it after review. It NEVER starts a Guest/Runtime/Gateway/native ROS peer,
initializes a reactor, grants an alias, or labels compilation as delivery.
The independently successful original40 official C++ termination object is
requalified as a retained library input; the failed batch remains FAIL. Old definitions/native IDs retain their
actual original producer/source identity and are byte-bridged, not relabelled.
"""
from __future__ import annotations
import argparse,copy,csv,hashlib,io,json,os,re,resource,shlex,stat,sys,time,traceback,types
from pathlib import Path,PurePosixPath
sys.dont_write_bytecode=True
MIB=1024**2
EXPECTED_SOURCE40_MANIFEST=None  # Actual future Source40 MF must be supplied by Root after publication; never invented.
SOURCE35='eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
SOURCE37='c8a2bde2c12fab767ca31eba68a08c10093dc5844ef76ea2dec8f87ae9738ee7'
SOURCE33='0ff8c2bc605c0acd24b4b8c641c2ceb503a53966e12054c6f2bc61b77b5f7a99'
NATIVE='c52bc5ed98e52904f52e1f8dac7031cd65a53f94d5128375d8de2eea23ae4d73'
UTILITIES='a7e89266486dba5ada416a31841b12646482001089b45765b3e05ec7b24c050f'
JS_DRIVER='a5581c6219732eacec8c1c7be23f610e0940523bd16f8ea893439316220b3d3c'
CPP_PRODUCER='31340e874df8848a9e7710d824483ae4588fbb9c320fe79ca12f056a33407981'
CPP_QUALIFIER='cb62255f68b42095f0322b66aa40d8691a7980289b0796a63055dd1a830767e4'
RUST_WRAPPER='472a89990970da1ad9956fffa9306e37f76561a68c2e759211cd61819d1a7be0'
RUST_SELECTION='63e78e5c58ffd7dd6b7f8cd80f949556dd05312ae57ef7819d1a997067ef390b'
RUST_REPORT='18023f1eba5d7c07555bd922d6c71e2a1e425654c5ca3d846b97224770a9e04a'
RECIPE='sdk/robots/unitree/recipes/closedtyped-example-recipes.json'
COMMAND_ROOT='tests/ros_unitree/typed_command'
COMMAND_RECIPE=COMMAND_ROOT+'/recipes.json'
EXAMPLES='sdk/robots/unitree/examples/'
HISTORICAL_CG={'memory_max':68719476736,'memory_swap_max':0,'expected_device':29,'expected_inode':18151,
 'expected_boot_id':'af121220-8c6e-4302-9853-f08bf4c50dbb',
 'expected_leaf':'/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope','expected_cpus':'0-31'}
CG={**HISTORICAL_CG,'expected_inode':18315,'expected_boot_id':'ec0d5f0f-3060-4220-bee2-2a7791ed4e72'}
EXPECTED=tuple((lang+'-'+kind+('-api' if kind=='topic-rpc' else ''),lang,kind,
              'unitree_api' if kind=='topic-rpc' else 'unitree_'+kind.rsplit('-',1)[1])
 for lang in ('rust','javascript','assemblyscript','tinygo')
 for kind in ('lowstate-go','lowstate-hg','topic-rpc'))+(('c-topic-rpc-api','c','topic-rpc','unitree_api'),('cpp-topic-rpc-api','cpp','topic-rpc','unitree_api'))

def need(value,reason):
    if not value:raise ValueError(reason)

def read(path,maximum=64*MIB):
    path=Path(path);need(path.is_absolute(),'absolute input required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=maximum,'bounded regular input required')
        body=bytearray()
        while True:
            part=os.read(fd,min(MIB,maximum+1-len(body)))
            if not part:break
            body.extend(part);need(len(body)<=maximum,'input grew')
        last,named=os.fstat(fd),path.stat(follow_symlinks=False)
        fields=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')
        need(len(body)==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in fields),'opened input changed')
        raw=bytes(body);return raw,{'path':str(path),'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),'device':first.st_dev,'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
    finally:os.close(fd)

def pairs(rows):
    result={}
    for key,value in rows:need(key not in result,'duplicate JSON member');result[key]=value
    return result

def doc(path):
    raw,pin=read(path);return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite JSON')),pin

def load(path,name,expected,frame=None):
    raw,pin=read(path,2*MIB);need(pin['sha256']==expected,'selected private/frozen helper changed')
    if frame:need(frame.pin(path)==pin,'helper full pin changed')
    module=types.ModuleType(name);module.__file__=str(path);sys.modules[name]=module
    exec(compile(raw,str(path),'exec'),module.__dict__)
    if frame is not None and hasattr(frame,'postboot'):module.POSTBOOT=frame.postboot
    return module,pin


def source_pin(frame,frozen,path):
    """Bind an original source to this complete current freeze, not a label."""
    path=Path(path).resolve(strict=True)
    need(path.is_relative_to(frame.source),'actual current source required')
    pin=frame.pin(path)
    need(frozen.get(str(path.relative_to(frame.source)))=={k:pin[k] for k in ('bytes','sha256')},'source body absent from actual Source40 MF')
    return pin


def put(frame,path,body):
    path=Path(path);need(path.is_relative_to(frame.out) and not path.exists() and not path.is_symlink(),'fresh owned regular product required')
    path.parent.mkdir(mode=0o700,parents=True,exist_ok=True)
    with path.open('xb') as stream:stream.write(body if isinstance(body,bytes) else body.encode())
    return frame.pin(path)


def copy_file(frame,old,new,frozen=None):
    old=Path(old).resolve(strict=True)
    before=source_pin(frame,frozen,old) if frozen is not None else frame.pin(old)
    raw,pin=read(old,max(before['bytes'],1));need(pin==before,'opened source changed during independent copy')
    current=put(frame,new,raw)
    need(all(current[k]==before[k] for k in ('bytes','sha256')) and
         (current['device'],current['inode'])!=(before['device'],before['inode']),'independent exact owned source bytes required')
    return {'source':before,'copy':current}


def copy_tree(frame,frozen,relative,stage,associations):
    root=frame.source/relative
    need(root.is_dir(),'complete actual SDK source tree missing')
    for old in sorted(root.rglob('*')):
        if old.is_dir():continue
        need(old.is_file() and not old.is_symlink(),'only MF-bound ordinary staged SDK sources admitted')
        associations.append(copy_file(frame,old,stage/old.relative_to(frame.source),frozen))


def expand(token,values):
    need(type(token) is str and '\0' not in token and '\n' not in token,'clean literal recipe token required')
    for key,value in values.items():token=token.replace('{'+key+'}',str(value))
    need('{' not in token and '}' not in token,'unresolved closed recipe placeholder: '+token)
    return token


# Explicit post-reboot reference comparisons; raw documents remain unchanged.
def admit_rust(frame, values):
    """Reopen the actual prepared unknown target's unchanged selection proof."""
    wrapper, wp = doc(frame.input('typed_rust_metadata'))
    report, rp = doc(frame.input('typed_rust_selection_report'))
    need(POSTBOOT.same(wp['sha256'], RUST_WRAPPER) and POSTBOOT.same(rp['sha256'], RUST_REPORT), 'exact actual new Rust metadata/provenance required')
    need(POSTBOOT.same(set(wrapper), {'path', 'bytes', 'sha256', 'data'}), 'closed actual tool wrapper required')
    selected, sp = doc(Path(wrapper['path']))
    need(POSTBOOT.same(sp['sha256'], RUST_SELECTION) and POSTBOOT.same({k: sp[k] for k in ('path', 'bytes', 'sha256')}, {k: wrapper[k] for k in ('path', 'bytes', 'sha256')}) and POSTBOOT.same(selected, wrapper['data']), 'actual raw new selection and wrapper differ')
    need(POSTBOOT.same(frame.pin(wp['path']), wp) and POSTBOOT.same(frame.pin(rp['path']), rp) and POSTBOOT.same(frame.pin(sp['path']), sp), 'first actual Rust selection pins changed')
    need(POSTBOOT.same(report['status'], 'PREPARED_CURRENT_RUST_TOOL_SELECTION_METADATA_ONLY') and POSTBOOT.same(report['new_tool_selection'], sp) and POSTBOOT.same(report['new_tool_selection_wrapper'], wp) and all((report.get(k) is True for k in ('source_preserved', 'inputs_preserved', 'complete_tool_roots_preserved'))) and POSTBOOT.same(report['cleanup']['status'], 'PASS') and (not report['cleanup']['remaining']) and (not report.get('finalization_errors')) and (report['compiler_executed'] is False) and (report['compilation_compatibility_qualified'] is False), 'prepared Rust input is not itself a compiler compatibility result')
    root = Path(values['RUST_SYSROOT'])
    need(root in frame.roots and (POSTBOOT.same(Path(values['RUSTC']).resolve(strict=True), Path(selected['tools']['rustc']).resolve(strict=True)) and POSTBOOT.same(Path(selected['tools']['rustc']).resolve(strict=True), root / 'bin/rustc')) and (root / 'lib/rustlib/wasm32-unknown-unknown/lib').is_dir(), 'actual complete prepared unknown Rust SDK required; no WASIp1 substitution')
    current = [row for row in frame.catalogs if POSTBOOT.same(row['root'], str(root))]
    original = [row for row in report['tool_roots_after'] if POSTBOOT.same(row['root'], str(root))]
    need((POSTBOOT.same(len(current), len(original)) and POSTBOOT.same(len(original), 1)) and POSTBOOT.tree_matches(original[0], current[0]), 'complete current prepared Rust SDK changed')
    return {'wrapper': wp, 'selection': sp, 'actual_preparation': rp, 'complete_current_sdk': current[0], 'scope': 'Actual prepared unknown-target bytes; these fourteen fresh builds still require their own results'}


# Explicit post-reboot reference comparisons; raw documents remain unchanged.
def admit_families(frame, frozen, inventory):
    """Admit complete actual native identities and unchanged generation bodies.

    The original Source33 inventory and c52 producer keep their real identities.
    This only associates unchanged emitters/profiles with their old definition
    products; new entry/SDK translation units compile from actual Source40.
    """
    need(POSTBOOT.same(inventory['kind'], 'unitree-three-family-native-bound-source-inventory') and POSTBOOT.same(inventory['source'], '/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r33/source') and POSTBOOT.same(inventory['source_manifest_sha256'], SOURCE33) and POSTBOOT.same(inventory['type_count'], 43) and POSTBOOT.same(set(inventory['families']), {'go', 'hg', 'api'}) and POSTBOOT.same(inventory['native_producer']['sha256'], NATIVE), 'exact actual complete three native families43-definition inventory required')
    native, np = doc(Path(inventory['native_producer']['path']))
    need((POSTBOOT.same(np, inventory['native_producer']) and POSTBOOT.same(inventory['native_producer'], frame.pin(np['path']))) and POSTBOOT.same(native['status'], 'PREPARED_NATIVE_INTERFACES_IDENTITIES_AND_SOURCE_PROVIDERS') and POSTBOOT.same(native['cleanup']['status'], 'PASS') and (not native['cleanup']['remaining']) and (not native['remaining_mounts']) and (not native.get('finalization_errors')) and all((native.get(k) is True for k in ('source_preserved', 'inputs_preserved', 'products_preserved', 'execution_sdk_preserved', 'derived_products_preserved'))), 'actual preserved native3 producer required; no guessed native identities')
    rows = {row['family']: row for row in native['families']}
    need(POSTBOOT.same(len(native['families']), 3) and POSTBOOT.same(set(rows), {'unitree_go', 'unitree_hg', 'unitree_api'}), 'all actual native families required')
    result = {}
    for family, count in (('go', 24), ('hg', 11), ('api', 8)):
        key = 'unitree_' + family
        selected = inventory['families'][family]
        nr = rows[key]
        directory = Path(selected['directory'])
        need(POSTBOOT.same(directory.resolve(strict=True), directory) and directory.is_dir(), 'physical complete generation directory required')
        generation, gp = doc(frame.input(selected['generation_input']))
        need((POSTBOOT.same(gp, selected['generation']) and POSTBOOT.same(selected['generation'], nr['generation']) and POSTBOOT.same(nr['generation'], frame.pin(gp['path']))) and POSTBOOT.same(Path(gp['path']), directory / 'generation.json'), 'actual native generation physical pin differs')
        need(POSTBOOT.same(generation['format'], 'uwvm-ros-custom-bounded-v2') and POSTBOOT.same(generation['status'], 'GENERATED_SOURCE_ONLY') and (generation['native_registry_emitted'] is True) and POSTBOOT.same(generation['implicit_permissions'], []) and POSTBOOT.same(generation['generator_sha256'], source_pin(frame, frozen, frame.source / 'sdk/tools/custom_types.py')['sha256']) and POSTBOOT.same(generation['bounded_generator_sha256'], source_pin(frame, frozen, frame.source / 'sdk/custom-types/bounded_profile.py')['sha256']), 'old complete generation must match the actual current unchanged emitters')
        profile, pp = doc(frame.source / 'sdk/robots/unitree/profiles' / (key + '.json'))
        source_pin(frame, frozen, Path(pp['path']))
        wanted = {row['type'] for row in profile['definitions']}
        need(POSTBOOT.same(len(wanted), count) and POSTBOOT.same(generation['records'], selected['records']) and (POSTBOOT.same({row['type'] for row in generation['records']}, {row['type'] for row in generation['types']}) and POSTBOOT.same({row['type'] for row in generation['types']}, wanted)) and (POSTBOOT.same(len(generation['records']), len(generation['types'])) and POSTBOOT.same(len(generation['types']), count)) and all((POSTBOOT.same(row['ros_version'], 2) and re.fullmatch('RIHS01_[0-9a-f]{64}', row['native_identity']) for row in generation['types'])), 'complete official profile/native identity inventory cannot be projected')
        derived = Path(nr['derived_tree']['root'])
        need(derived in frame.roots, 'every actual native-derived complete family root must be explicit')
        catalogs = [row for row in frame.catalogs if POSTBOOT.same(row['root'], str(derived))]
        need(POSTBOOT.same(len(catalogs), 1) and POSTBOOT.tree_matches(nr['derived_tree'], catalogs[0]), 'whole actual native-derived family changed')
        selection, selection_pin = doc(Path(nr['source_selection']['path']))
        old_profile = frame.pin(derived / 'selection/profile.json')
        need((POSTBOOT.same(selection_pin, nr['source_selection']) and POSTBOOT.same(nr['source_selection'], frame.pin(selection_pin['path']))) and POSTBOOT.same(selection['status'], 'PREPARED_SOURCE_ONLY') and POSTBOOT.same(selection['family'], key) and POSTBOOT.same(selection['source'], native['source']) and POSTBOOT.same(selection['source_manifest_sha256'], native['source_manifest_sha256']) and (selection['inputs_preserved'] is True) and POSTBOOT.member(old_profile, selection['outputs']) and all((POSTBOOT.same(old_profile[k], pp[k]) for k in ('bytes', 'sha256'))), 'current complete profile bytes must equal the actual native producer selection output')
        bound = derived / 'bound/catalog.json'
        catalog, cp = doc(bound)
        need((POSTBOOT.same(cp, selected['bound_catalog']) and POSTBOOT.same(selected['bound_catalog'], frame.pin(bound))) and POSTBOOT.same({k: cp[k] for k in ('bytes', 'sha256')}, generation['source_inputs']['catalog']) and POSTBOOT.same({r['type']: r['native_identity'] for r in catalog['types']}, {r['type']: r['native_identity'] for r in generation['types']}), 'actual native catalog and complete generation identities differ')
        for definition in generation['source_inputs']['definitions']:
            path = (bound.parent / definition['file']).resolve(strict=True)
            need(path.is_relative_to(bound.parent), 'definition escaped actual native bound root')
            pin = frame.pin(path)
            need(all((POSTBOOT.same(pin[k], definition[k]) for k in ('bytes', 'sha256'))), 'actual native definition bytes changed')
        for name, expected in generation['outputs'].items():
            need(POSTBOOT.same(Path(name).name, name) and name not in ('.', '..'), 'flat generated output name required')
            pin = frame.pin(directory / name)
            need(all((POSTBOOT.same(pin[k], expected[k]) for k in ('bytes', 'sha256'))), 'complete actual generated output changed')
        result[key] = {'directory': directory, 'generation': gp, 'profile': pp, 'native_catalog': cp, 'records': generation['records'], 'original_native_producer': np, 'original_native_source_selection': selection_pin, 'original_native_profile_copy': old_profile, 'current_complete_profile_bytes_verified': True, 'original_native_source': native['source'], 'original_native_manifest': native['source_manifest_sha256'], 'current_emitter_bytes_verified': True, 'scope': 'Unchanged actual native-bound definitions only; new Guest compilation belongs to Source40'}
    return result


def derive_tinygo_target(frame,values,cell):
    original=Path(values['TINYGO_ROOT'])/'targets/wasm-unknown.json';raw,pin=read(original,MIB)
    need(frame.pin(original)==pin and len(raw)==790 and pin['sha256']=='63531e39ab6c1a3bcd0c1a78d7a68bdae917a530c0b64fe80541b163a1b037f3','exact installed official pure reactor target required')
    target=json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite target'))
    need(target['llvm-target']=='wasm32-unknown-unknown' and target['buildmode']=='c-shared' and target['scheduler']=='none' and
         target['libc']=='wasmbuiltins' and target['gc']=='leaking' and not target.get('inherits') and
         target['ldflags']==['--stack-first','--no-demangle','--no-entry'],'self-contained actual pure target required')
    target['buildmode']='default'
    target['ldflags']=[*target['ldflags'],'--max-memory=67108864']
    path=cell/'bounded-wasm-unknown.json';owned=put(frame,path,json.dumps(target,indent=2)+'\n')
    return path,{'original':pin,'owned':owned,'only_semantic_changes':{'buildmode':'default','ldflags_append':['--max-memory=67108864']},'SDK_modified':False}




def compile_command_entry(frame,frozen,row,cell,run,values,utilities,javascript_symbols=None):
    """Compile one real C entry object; direct functions resolve at same link."""
    if row['language'] not in ('c','cpp','rust','javascript'):return None
    source=frame.source/COMMAND_ROOT/'entry.c';sp=source_pin(frame,frozen,source)
    obj,dep=cell/'command-entry.o',cell/'command-entry.d'
    family={'unitree_go':1,'unitree_hg':2,'unitree_api':3}[row['family']]
    aliases=[]
    if row['language']=='javascript':
        need(javascript_symbols=={name:'exported_'+str(index) for index,name in enumerate(row['exports'])},
             'exact ordered actual scalar trampoline C-symbol mapping required')
        aliases=['-D'+name+'='+symbol for name,symbol in javascript_symbols.items()]
    else:need(javascript_symbols is None,'C/C++/Rust use their actual named extern C symbols')
    argv=[values['WASI_CLANG'],'--driver-mode=gcc','--target=wasm32-unknown-unknown',
      '--sysroot='+values['WASI_SYSROOT'],'-std=c11','-O1','-g0','-UNDEBUG','-ffreestanding','-fno-builtin',
      '-fno-fast-math','-ffp-contract=off','-DUWVM_TYPED_FAMILY='+str(family),
      '-DUWVM_TYPED_JAVASCRIPT='+str(int(row['language']=='javascript')),*aliases,
      '-MD','-MF',str(dep),'-MT',str(obj),'-c',str(source),'-o',str(obj)]
    job=run(row['id']+'-command-entry-compile',argv,cell)
    dependencies=utilities.full_md(frame,dep,obj,source,cell)
    raw,pin=read(obj,16*MIB);need(raw[:8]==b'\x00asm\x01\x00\x00\x00' and frame.pin(obj)==pin,'actual wasm32 entry object required')
    return {'source':sp,'object':pin,'dependency_file':frame.pin(dep),'full_MD':dependencies,'command':job,
      'definitions':{'UWVM_TYPED_FAMILY':family,'UWVM_TYPED_JAVASCRIPT':int(row['language']=='javascript')},
      'entry_only_C_symbol_aliases':javascript_symbols,'scope':'Fresh real C object linked into SAME module; no separately loaded ROS bridge'}


def stage_command_sources(frame,frozen,row,stage,associations):
    """Stage exact new helper sources and small explicit AS import selectors."""
    language=row['language'];family={'unitree_go':1,'unitree_hg':2,'unitree_api':3}[row['family']]
    result={'kind':'SAME_MAIN_COMPILED_COMMAND','language':language,'family':family,
      'entry_export':'typed_command_entry' if language=='assemblyscript' else '_start',
      'language_initialization':'Actual AS exported initializer or TinyGo default _start; C entry explicitly initializes embedded JS' if language in ('assemblyscript','tinygo','javascript') else 'Actual command function in same module',
      'reactor_results_relabelled':False,'preloaded_application':False}
    if language=='assemblyscript':
        relative=COMMAND_ROOT+'/assemblyscript_'+('lowstate.ts' if family!=3 else 'topic_rpc.ts')
        for rel in (relative,COMMAND_ROOT+'/protocol.ts'):
            associations.append(copy_file(frame,frame.source/rel,stage/rel,frozen))
        root=stage/COMMAND_ROOT
        names=','.join(row['exports'])
        selected=put(frame,root/'selected_case.ts',"// Actual selected source re-export; no replacement implementation.\nexport {"+names+"} from '../../../"+row['source'].removesuffix('.ts')+"';\n")
        result.update(staged_entry=str(stage/relative),selected_case=selected,
           selected_actual_source=source_pin(frame,frozen,frame.source/row['source']))
        if family!=3:result['selected_family']=put(frame,root/'selected_family.ts','export const SELECTED_FAMILY:u32='+str(family)+';\n')
    elif language=='tinygo':
        destination=stage/EXAMPLES/'tinygo'
        for name,target in (('protocol.go','typed_command_protocol.go'),
             ('command_lowstate.go' if family!=3 else 'command_topic_rpc.go','typed_command_main.go')):
            associations.append(copy_file(frame,frame.source/COMMAND_ROOT/name,destination/target,frozen))
        result['entry_source']=source_pin(frame,frozen,frame.source/COMMAND_ROOT/('command_lowstate.go' if family!=3 else 'command_topic_rpc.go'))
    return result


def inspect_command_binary(frame,utilities,inspector,schema,row,artifact,validation):
    pin=frame.pin(artifact);raw,current=read(artifact,64*MIB);need(current==pin,'actual command binary changed')
    actual=utilities.wasm_structure(raw,inspector);frame.report.setdefault('observed_binary_tables',{})[row['id']+'-command']=actual;frame.save()
    need(actual['start_function'] is None and not actual['imported_globals'] and len(actual['memories'])==1 and
       not actual['memories'][0]['imported'] and actual['memories'][0]['maximum']==1024 and
       0<=actual['memories'][0]['minimum']<=1024,'actual bounded64MiB command without implicit start/imported memory/global required')
    expected={r['name']:([p['type'] for p in r['params']],r['results']) for r in schema['modules']['uwvm_ros2_v1']['functions']}
    seen=set();ros=set();stdio=set()
    for item in actual['imports']:
        key=item['module'],item['name'];need(item['kind']==0 and key not in seen,'unique actual command function imports only');seen.add(key)
        if key[0]=='uwvm_ros2_v1':
            need((item['params'],item['results'])==expected.get(key[1]),'actual ROS2 signature differs');ros.add(key[1])
        else:
            shape=(['i32'],[]) if key[1]=='proc_exit' else (['i32']*4,['i32'])
            need(key[0]=='wasi_snapshot_preview1' and key[1] in ('fd_read','fd_write','proc_exit') and
                 (item['params'],item['results'])==shape,'unexpected optional WASI or foreign bridge import');stdio.add(key[1])
    need('capability_open' in ros and stdio=={'fd_read','fd_write','proc_exit'},'actual typed SDK and complete basic command IO imports required')
    core=lambda value:{'u32':'i32','u64':'i64'}.get(value,value)
    for name,signature in row['exports'].items():
        found=actual['exports'].get(name);need(found and found['kind']==0 and found.get('params')==[core(v) for v in signature['parameters']] and
           found.get('results')==[core(signature['result'])],'actual unchanged typed scalar ABI must remain exported')
    name='typed_command_entry' if row['language']=='assemblyscript' else '_start';entry=actual['exports'].get(name)
    need(entry and entry['kind']==0 and entry.get('params')==[] and entry.get('results')==[],'actual same-main void command entry required')
    return {'artifact':pin,'actual_binary':actual,'validation':validation,'status':'COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED',
      'entry_export':name,'runtime_initialization':'NOT_EXECUTED','default_denial':'NOT_EXECUTED','native_transport':'NOT_EXECUTED','granted_rights':[],
      'same_main_source_control':True,'preloaded_application':False,'cross_module_guard_modified':False}


def prepare_cpp(frame,args,run,values):
    """Make one real isolated Source40 official object, separate from Guests."""
    producer=frame.input('typed_cpp_producer');qualifier=frame.input('typed_cpp_qualifier')
    need(frame.pin(producer)['sha256']==CPP_PRODUCER and frame.pin(qualifier)['sha256']==CPP_QUALIFIER,
         'unchanged official producer and distinct reviewed consumer required')
    folder=frame.out/'official-cpp-object'
    command=run('typed-official-cpp-object',[values['PYTHON'],'-B',str(producer),'--source',str(frame.source),
        '--manifest',str(frame.manifest),'--manifest-sha256',frame.report['source_manifest_sha256'],'--config',str(args.config.resolve(strict=True)),
        '--config-sha256',args.config_sha256,'--out',str(folder),'--runner-sha256',CPP_PRODUCER,
        '--selection-input','typed_cpp_selection'],frame.out,cpp_supervisor=True)
    report,rp=doc(folder/'report.json')
    need(report['status']=='PASS_OFFICIAL_CPP_TERMINATION_OBJECT_PREPARATION_ONLY' and report['source']==str(frame.source) and
         report['source_manifest_sha256']==frame.report['source_manifest_sha256'],'fresh Source40 object provenance required; never relabel aa500')
    # This newly completed child output is now a derived read-only input. Its
    # complete tree is inventoried after production, before consumption, and at
    # finalization. Catalog files live outside it, so there is no hash cycle.
    need(folder not in frame.roots and len(frame.roots)<64,'bounded derived product inventory required')
    frame.roots.append(folder)
    frame.catalogs.append(frame.common.catalog(folder,frame.out/'official-cpp-derived-before.jsonl',frame.roots))
    module,_=load(qualifier,'typed_current_cpp_object_qualifier',CPP_QUALIFIER,frame)
    value=module.qualify_overlay(frame,folder/'report.json',rp['sha256'],values)
    need(value['report']==rp and value['producer']==frame.pin(producer) and value['consumer_qualifier']==frame.pin(qualifier),
         'actual fresh object/current distinct qualifier association changed')
    frame.report['official_cpp_object']={'command':command,'qualification':value,'separate_object_compiles':1,
        'Guest_products':0,'scope':'Official isolated abort TU only; no Guest/current-source interoperability by this object'}
    frame.save();return value


def cpp_link_evidence(frame,utilities,linked,why,overlay,links):
    need(overlay['object'] in links and frame.pin(overlay['object']['path'])==overlay['object'],'actual trace omitted selected official object')
    raw,pin=read(why,16*MIB);rows=list(csv.reader(io.StringIO(raw.decode('utf-8','strict')),delimiter='\t'))
    need(rows and rows[0]==['reference','extracted','symbol'] and all(len(r)==3 for r in rows),'actual LLVM22 extraction table required')
    def default_handler(value):
        if not value.endswith(')') or '(' not in value:return False
        archive,member=value.rsplit('(',1)
        return Path(archive).name in ('libc++.a','libc++abi.a') and member[:-1] in (
            'verbose_abort.cpp.o','abort_message.cpp.o','verbose_abort.cpp.obj','abort_message.cpp.obj')
    log,_=read(Path(linked['log']['path']),16*MIB)
    need(not any(default_handler(r[1]) for r in rows[1:]) and
         not any(default_handler(line.strip()) for line in log.decode('utf-8','strict').splitlines()),
         'default diagnostic stderr termination member still selected')
    return {'why_extract':frame.pin(why),'archive_extractions':[dict(zip(rows[0],r)) for r in rows[1:]],
            'default_verbose_and_abort_members_absent':True,'Guest_assertions_disabled':False,'stdio_or_WASI_stubs':False}


def stage_product(frame,frozen,row,family,stage):
    """Preserve complete generated modules and real relative SDK resolution."""
    associations=[];language=row['language'];directory=family['directory']
    if language=='rust':
        relative=EXAMPLES+'rust'
        copy_tree(frame,frozen,relative,stage,associations)
        associations.append(copy_file(frame,directory/'bounded_records.rs',stage/relative/'bounded_records.rs'))
    elif language=='tinygo':
        copy_tree(frame,frozen,'sdk/tinygo',stage,associations)
        root=frame.source/EXAMPLES/'tinygo'
        for original in sorted(root.rglob('*')):
            if original.is_dir():continue
            need(original.is_file() and not original.is_symlink(),'ordinary actual TinyGo source required')
            destination=stage/original.relative_to(frame.source)
            if original==frame.source/row['source']:
                before=source_pin(frame,frozen,original);raw,actual=read(original,MIB)
                needle=b'func main() {}'
                need(actual==before and raw.count(needle)==1,'only exact original empty TinyGo main may be removed')
                result=put(frame,destination,raw.replace(needle,b'// Empty reactor main removed; actual separate command main supplies entry.',1))
                associations.append({'source':before,'copy':result,'derivation':'ONLY exact empty func main() {} replaced by a comment; all actual typed function bodies unchanged','original_retained':True})
            else:associations.append(copy_file(frame,original,destination,frozen))
        associations.append(copy_file(frame,directory/'bounded_records.go',stage/EXAMPLES/'tinygo/customtypes/bounded_records.go'))
    elif language=='assemblyscript':
        copy_tree(frame,frozen,'sdk/assemblyscript',stage,associations)
        copy_tree(frame,frozen,EXAMPLES+'assemblyscript',stage,associations)
        associations.append(copy_file(frame,directory/'bounded_records.ts',stage/'sdk/custom-types/typed-family/bounded_records.ts'))
    elif language=='javascript':
        copy_tree(frame,frozen,'sdk/javascript',stage,associations)
        copy_tree(frame,frozen,EXAMPLES+'javascript',stage,associations)
        for relative in ('sdk/ros2/abi.h','sdk/common/abi.h','sdk/abi/abi-v1.json'):
            associations.append(copy_file(frame,frame.source/relative,stage/relative,frozen))
        associations.append(copy_file(frame,directory/'bounded_records.mjs',stage/EXAMPLES/'javascript/bounded_records.mjs'))
    else:
        source_pin(frame,frozen,frame.source/row['source'])
    return associations


def compile_javascript(frame,frozen,utilities,run,js_helper,js_config,catalog,row,stage,cell,schema,values):
    """Actual frozen prepare -> twelve C TUs -> one closed link -> full validator."""
    manifest=copy.deepcopy(catalog['javascript_manifest']);manifest['entry']=row['source']
    manifest['modules']=[{key:(row['source'] if value=='{SELECTED_SOURCE_RELATIVE}' else value)
                         for key,value in item.items()} for item in manifest['modules']]
    manifest['exports']=[{'name':name,'function':name,**signature,'failure':22,'initialize':True}
                         for name,signature in row['exports'].items()]
    path=cell/'manifest.json';put(frame,path,json.dumps(manifest,indent=2)+'\n')
    jsi,jsp=js_config['inputs'],js_config['paths'];out=cell/'prepared'
    argv=[str(frame.input('python')),'-B',str(stage/'sdk/javascript/guest/prepare.py'),'--source-root',str(stage),
          '--quickjs',jsp['quickjs'],'--manifest',str(path),'--out',str(out),'--clang',jsi['clang']['path'],
          '--linker',jsp['linker_alias'],'--sysroot',jsp['sysroot'],'--resource-dir',jsp['resource_dir'],
          '--libc',jsi['libc']['path'],'--libm',jsi['libm']['path'],'--printscan',jsi['printscan']['path'],
          '--builtins',jsi['builtins']['path'],'--validator',jsi['validator']['path']]
    jobs=[run(row['id']+'-prepare',argv,cell)]
    prepared,pp=doc(out/'report.json')
    need(prepared['schema']=='uwvm-embedded-javascript-preparation-v1' and prepared['status']=='PREPARED_NOT_COMPILED' and
         prepared['manifest']==manifest and prepared['compiler_executed'] is False and prepared['implicit_permissions']==[] and
         prepared['artifact']==str(out/'guest.wasm'),'actual current frozen preparation/manifest required')
    frame.pin(pp['path'])
    for pin in prepared['inputs']:
        need(all(frame.pin(pin['path'])[k]==pin[k] for k in ('path','bytes','sha256')),'actual staged JS compiler input changed')
    sources=[out/'quickjs'/name for name in ('quickjs.c','dtoa.c','libregexp.c','libunicode.c','cutils.c')]+[
        stage/'sdk/javascript/guest'/name for name in ('engine.c','scalars.c','utf8.c','native_ros.c','stdio.c')]+[
        out/'module_table.c',out/'scalar_trampolines.c']
    need([job['id'] for job in prepared['jobs']]==['compile-'+str(i) for i in range(12)]+['link','validate'],
         'actual backend2 reactor must have exact twelve C TUs then link/validator')
    allow=out/'allowed-symbols.txt';allowed,allow_pin=read(allow,MIB)
    expected='\n'.join(['uwvm_ros2_'+r['name'] for r in schema['modules']['uwvm_ros2_v1']['functions']]+['fd_read','fd_write'])+'\n'
    need(allowed==expected.encode(),'closed current ROS2/stdio-only undefined allowlist required; no scalar/command symbols')
    frame.pin(allow);objects=[];closures=[];link_inputs=[];validation=None
    # export_name selects the final Wasm label, not the C link symbol. The
    # frozen real generator emits exported_N in this closed manifest order.
    # Bind/check every actual declaration before compiling this entry-only
    # translation unit with literal name aliases; trampoline bodies stay exact.
    scalar_raw,scalar_pin=read(out/'scalar_trampolines.c',MIB)
    need(frame.pin(scalar_pin['path'])==scalar_pin,'actual generated scalar trampoline changed')
    scalar_text=scalar_raw.decode('utf-8','strict');symbols={}
    for index,descriptor in enumerate(manifest['exports']):
        name=descriptor['name'];signature=row['exports'][name]
        need(descriptor['function']==name and signature['result']=='u32' and
             all(kind=='u32' for kind in signature['parameters']),'actual unsigned scalar ABI required')
        parameters=','.join('uint32_t a'+str(n) for n in range(len(signature['parameters']))) or 'void'
        declaration='__attribute__((export_name('+json.dumps(name)+'))) uint32_t exported_'+str(index)+'('+parameters+'){'
        need(scalar_text.count(declaration)==1,'actual generated C symbol/signature/export label differs')
        symbols[name]='exported_'+str(index)
    observed=re.findall(r'__attribute__\(\(export_name\("([A-Za-z_][A-Za-z0-9_]*)"\)\)\) uint32_t (exported_[0-9]+)\(',scalar_text)
    need(observed==list(symbols.items()),'actual ordered scalar C-symbol inventory differs')
    entry=compile_command_entry(frame,frozen,row,cell,run,values,utilities,symbols)
    command_allow=cell/'command-allowed-symbols.txt';command_allow_pin=put(frame,command_allow,allowed+b'exit_process\n')
    common=[jsi['clang']['path'],'--target=wasm32-wasip1','--sysroot='+jsp['sysroot'],'-resource-dir='+jsp['resource_dir'],
            '-std=gnu11',manifest['optimization'],'-UNDEBUG','-fwrapv','-funsigned-char','-fno-fast-math','-ffp-contract=off',
            '-ffunction-sections','-fdata-sections','-DUWVM_JS_SANDBOX=1','-DCONFIG_VERSION="2026-06-04"','-D_GNU_SOURCE',
            '-DUWVM_JS_BACKEND=2','-I',str(stage/'sdk/javascript/guest'),'-I',str(out/'quickjs')]
    common+=['-DUWVM_JS_'+key.upper()+'='+str(value) for key,value in manifest['limits'].items()]
    for index,job in enumerate(prepared['jobs']):
        need(job['cwd']==str(out),'actual JS job cwd differs')
        if index<12:
            obj=out/'objects'/(str(index)+'.o');dep=obj.with_suffix('.d');source=sources[index]
            actual=[*common,'-MD','-MF',str(dep),'-MT',str(obj),'-c',str(source),'-o',str(obj)]
            need(job['argv']==actual and job['artifact']==str(obj) and job['dependencies']==str(dep) and
                 job['source']['path']==str(source),'actual closed full-MD JS C argv differs')
            jobs.append(run(row['id']+'-'+job['id'],actual,out));objects.append(obj)
            closures.append(utilities.full_md(frame,dep,obj,source,out));frame.pin(dep);frame.pin(obj)
        elif job['id']=='link':
            actual=[jsi['clang']['path'],'--target=wasm32-wasip1','--sysroot='+jsp['sysroot'],'-resource-dir='+jsp['resource_dir'],
                '-fuse-ld='+jsp['linker_alias'],'-nostdlib','-nostartfiles','-Wl,--threads=1','-Wl,--gc-sections','-Wl,--trace',
                '-Wl,--export-memory','-Wl,--stack-first','-Wl,-z,stack-size='+str(manifest['limits']['stack_bytes']),
                '-Wl,--initial-memory='+str(manifest['initial_memory']),'-Wl,--max-memory='+str(manifest['maximum_memory']),
                '-Wl,--allow-undefined-file='+str(allow),'-Wl,--no-entry',*[str(p) for p in objects],
                *[jsi[key]['path'] for key in ('libm','printscan','libc','builtins')],'-o',str(out/'guest.wasm')]
            need(job['argv']==actual and job['artifact']==str(out/'guest.wasm') and job['requires_retained_link_trace'] is True,
                 'actual exact serial JS Wasm link required')
            original_link=list(actual)
            actual[actual.index('-Wl,--allow-undefined-file='+str(allow))]='-Wl,--allow-undefined-file='+str(command_allow)
            actual.insert(actual.index(jsi['libm']['path']),entry['object']['path'])
            actual.insert(actual.index('-o'),'-Wl,--export=_start')
            linked=run(row['id']+'-same-main-command-link',actual,out);jobs.append(linked)
            link_inputs=js_helper.trace_closure(types.SimpleNamespace(common=frame.common,limits=frame.limits,pin=frame.pin,inputs=jsi),linked,[*objects,Path(entry['object']['path'])])
            need(entry['object'] in link_inputs,'actual JS command link trace omitted entry object')
            linked['same_main_link_derivation']={'original_preparation_argv':original_link,'actual_argv':actual,
                'additional_fresh_entry_object':entry['object'],'separate_allowlist':command_allow_pin,'only_added_undefined_symbol':'exit_process'}
        else:
            need(job['id']=='validate' and job['argv']==[jsi['validator']['path'],'validate',str(out/'guest.wasm')],'actual final JS validator argv required')
            validation=run(row['id']+'-full-validate',job['argv'],out);jobs.append(validation)
    need(frame.pin(allow)==allow_pin,'actual allowlist changed during link')
    artifact=out/'guest.wasm';raw,bp=read(artifact,64*MIB);need(frame.pin(artifact)==bp,'actual JS artifact changed')
    need(frame.pin(command_allow)==command_allow_pin,'derived command allowlist changed')
    need(frame.pin(scalar_pin['path'])==scalar_pin,'actual generated scalar symbols changed after link')
    return artifact,validation,{'commands':jobs,'manifest':frame.pin(path),'preparation':frame.pin(pp['path']),
        'entry_compile':entry,'actual_scalar_C_symbol_binding':{'source':scalar_pin,'ordered_export_to_C_symbol':symbols,
            'entry_only_preprocessor_aliases':True,'trampoline_bodies_modified':False},
        'full_MD':closures,'link_inputs':link_inputs,'original_undefined_symbols':allow_pin,'command_undefined_symbols':command_allow_pin,
        'actual_embedded_boundaries':{'current_full_manifest':manifest,'original_preparation_preserved':True,'strict_command_binary_gate_required':True},
        'closure':'Actual12C engine TUs plus one fresh same-main C entry, fullMD/link/validator; no reactor result relabelled'}



def prepare_rust_linker(frame,cell,values,entry,utilities):
    """Capture successful stable-Rust linker stdout outside Rustc's pipe.

    The wrapper is a fresh owned native Python script, not a Guest or a fake
    linker. It checks the same selected physical tools and invocation alias,
    records actual linker argv/cwd, redirects only stdout to a new trace, and
    execs the already opened genuine ELF with the original wasm-ld argv[0].
    The original Rust diagnostics remain in the ordinary compiler job log.
    """
    python=frame.pin(Path(values['PYTHON']).resolve(strict=True))
    need(python['path']=='/usr/bin/python3.14','selected actual Python3.14 required for owned linker wrapper')
    alias=utilities.alias(values['WASM_LD']);linker=frame.pin(alias['physical'])
    wrapper=cell/'rust-linker/wasm-ld';trace=cell/'rust-wasm-ld.trace.log';receipt=cell/'rust-wasm-ld.argv.json'
    config={'python':python,'linker':linker,'alias':alias,'cwd':str(cell),'entry_object':entry['object'],
            'artifact':str(cell/'guest.wasm'),'trace':str(trace),'receipt':str(receipt)}
    body='#!/usr/bin/python3.14\n'+r'''"""Owned one-link stdout capture; never a substitute linker implementation."""
import hashlib,json,os,resource,stat,sys
from pathlib import Path
sys.dont_write_bytecode=True
CONFIG=__CONFIG__
FIELDS=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')
def need(value,reason):
    if not value:raise ValueError(reason)
def opened(path):
    path=Path(path);need(path.is_absolute() and path.resolve(strict=True)==path,'physical regular input required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
    try:
        first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<first.st_size<=256*1024**2,'bounded physical tool required')
        digest=hashlib.sha256();count=0
        while True:
            block=os.read(fd,1024**2)
            if not block:break
            digest.update(block);count+=len(block);need(count<=first.st_size,'input grew')
        last,named=os.fstat(fd),path.stat(follow_symlinks=False)
        need(count==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in FIELDS),'opened input changed')
        pin=dict(path=str(path),bytes=count,sha256=digest.hexdigest(),device=first.st_dev,inode=first.st_ino,mtime_ns=first.st_mtime_ns,ctime_ns=first.st_ctime_ns)
        os.lseek(fd,0,os.SEEK_SET);return fd,pin
    except BaseException:os.close(fd);raise
def alias(path):
    path=Path(path);need(path.is_absolute() and path.parent.resolve(strict=True)==path.parent,'canonical invocation parent required')
    first=path.lstat();target=os.readlink(path) if stat.S_ISLNK(first.st_mode) else None
    need(stat.S_ISREG(first.st_mode) or stat.S_ISLNK(first.st_mode),'ordinary selected alias required')
    physical=path.resolve(strict=True);last=path.lstat()
    need(all(getattr(first,k)==getattr(last,k) for k in (*FIELDS,'st_mode')) and (target is None or os.readlink(path)==target),'tool alias changed')
    return dict(path=str(path),physical=str(physical),target=target,device=first.st_dev,inode=first.st_ino,mode=first.st_mode,bytes=first.st_size,mtime_ns=first.st_mtime_ns,ctime_ns=first.st_ctime_ns)
need(sys.platform=='linux' and os.geteuid()==0 and os.execve in os.supports_fd,'Linux Root opened-ELF exec required')
need(os.getcwd()==CONFIG['cwd'],'actual linker cwd differs from owned compiler cell')
need(len(sys.argv)<=4096 and sum(len(x.encode())+1 for x in sys.argv)<=1024**2 and all('\0' not in x and '\n' not in x for x in sys.argv),'bounded actual linker argv required')
args=sys.argv[1:]
need(args.count('--trace')==1 and args.count('--threads=1')==1 and args.count('--max-memory=67108864')==1 and args.count('--export=_start')==1 and args.count(CONFIG['entry_object']['path'])==1,'actual unchanged strict linker flags/entry required')
need(args.count('-o')==1 and args[args.index('-o')+1]==CONFIG['artifact'],'actual owned output required')
need(not any(x.startswith('@') for x in args),'unopened response-file indirection is outside this closed linker receipt')
fd,linker=opened(CONFIG['linker']['path']);need(linker==CONFIG['linker'] and alias(CONFIG['alias']['path'])==CONFIG['alias'],'selected real linker/alias differs')
for key,path in (('python',Path(sys.executable).resolve(strict=True)),('entry_object',Path(CONFIG['entry_object']['path']))):
    handle,pin=opened(path);os.close(handle);need(pin==CONFIG[key],'selected actual tool/entry differs')
handle,own=opened(Path(__file__).resolve(strict=True));os.close(handle)
# Rustc can remove its own temporary objects after a successful link. Capture
# every actual ordinary file argument at this genuine pre-exec barrier, before
# those files can disappear. This is historical opened-byte provenance, not a
# promise that a temporary still exists after Rustc returns.
file_arguments=[]
for index,token in enumerate(args):
    if token.startswith('-'):continue
    candidate=Path(token);candidate=candidate if candidate.is_absolute() else Path.cwd()/candidate
    if candidate.is_file():
        handle,pin=opened(candidate.resolve(strict=True));os.close(handle)
        file_arguments.append(dict(argument_index=index+1,spelling=token,pin=pin))
need(any(row['pin']==CONFIG['entry_object'] for row in file_arguments),'pre-exec actual entry object absent')
record=dict(kind='uwvm-owned-rust-linker-exec-v1',cwd=os.getcwd(),pid=os.getpid(),argv=[CONFIG['alias']['path'],*args],wrapper=own,python=CONFIG['python'],linker=linker,invocation_alias=CONFIG['alias'],trace=CONFIG['trace'],file_arguments=file_arguments,execution='same-PID opened physical ELF; original invocation argv0 preserved')
raw=(json.dumps(record,indent=2)+'\n').encode();need(len(raw)<=1024**2,'bounded linker receipt required')
out=os.open(CONFIG['receipt'],os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o600)
with os.fdopen(out,'wb') as stream:stream.write(raw)
# Bound every linker-created file independently of the parent's finite RSS,
# wall-time, whole-output and disk guards. The trace is rechecked at16MiB.
resource.setrlimit(resource.RLIMIT_FSIZE,(64*1024**2,64*1024**2))
stdout=os.open(CONFIG['trace'],os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o600)
os.dup2(stdout,1,inheritable=True);os.close(stdout)
os.execve(fd,[CONFIG['alias']['path'],*args],dict(os.environ))
'''
    body=body.replace('__CONFIG__',repr(config))
    # Final permissions must precede the FIRST stored physical pin: chmod
    # changes ctime even when the bytes are identical.
    need(wrapper.is_relative_to(frame.out) and not wrapper.exists() and not wrapper.is_symlink(),'fresh owned linker wrapper required')
    wrapper.parent.mkdir(mode=0o700,parents=True,exist_ok=True)
    with wrapper.open('xb') as stream:stream.write(body.encode())
    os.chmod(wrapper,0o700);pin=frame.pin(wrapper)
    return {'wrapper':pin,'python':python,'linker':linker,'alias':alias,'trace_path':str(trace),'receipt_path':str(receipt),
            'cell':str(cell),'entry_object':entry['object'],'artifact':str(cell/'guest.wasm')}


# Explicit post-reboot reference comparisons; raw documents remain unchanged.
def qualify_rust_linker(frame, proof, utilities):
    """Require genuine successful-link trace and actual recorded invocation."""
    receipt, rp = doc(Path(proof['receipt_path']))
    trace = frame.pin(proof['trace_path'])
    need(trace['bytes'] <= 16 * MIB and POSTBOOT.same(rp, frame.pin(rp['path'])), 'bounded actual linker receipt/trace required')
    need(POSTBOOT.same(set(receipt), {'kind', 'cwd', 'pid', 'argv', 'wrapper', 'python', 'linker', 'invocation_alias', 'trace', 'file_arguments', 'execution'}) and POSTBOOT.same(receipt['kind'], 'uwvm-owned-rust-linker-exec-v1') and POSTBOOT.same(receipt['cwd'], proof['cell']) and (type(receipt['pid']) is int) and (receipt['pid'] > 0) and POSTBOOT.same(receipt['wrapper'], proof['wrapper']) and POSTBOOT.same(receipt['python'], proof['python']) and POSTBOOT.same(receipt['linker'], proof['linker']) and POSTBOOT.same(receipt['invocation_alias'], proof['alias']) and POSTBOOT.same(receipt['trace'], trace['path']) and POSTBOOT.same(receipt['execution'], 'same-PID opened physical ELF; original invocation argv0 preserved'), 'actual owned-linker execution receipt differs')
    need(POSTBOOT.same(frame.pin(proof['wrapper']['path']), proof['wrapper']) and POSTBOOT.same(frame.pin(proof['python']['path']), proof['python']) and POSTBOOT.same(frame.pin(proof['linker']['path']), proof['linker']) and POSTBOOT.same(utilities.alias(proof['alias']['path']), proof['alias']), 'selected wrapper/tool bytes or alias changed')
    argv = receipt['argv']
    need(type(argv) is list and POSTBOOT.same(argv[0], proof['alias']['path']) and all((type(x) is str for x in argv)) and (POSTBOOT.same(argv.count('--trace'), argv.count('--threads=1')) and POSTBOOT.same(argv.count('--threads=1'), argv.count('--max-memory=67108864')) and POSTBOOT.same(argv.count('--max-memory=67108864'), argv.count('--export=_start')) and POSTBOOT.same(argv.count('--export=_start'), 1)) and POSTBOOT.same(argv.count(proof['entry_object']['path']), 1) and POSTBOOT.same(argv.count('-o'), 1) and POSTBOOT.same(argv[argv.index('-o') + 1], proof['artifact']), 'actual strict linker flags/output/entry evidence required')
    captured = {}
    temporary = []
    persistent = {}
    need(type(receipt['file_arguments']) is list and 1 <= len(receipt['file_arguments']) <= 4096, 'bounded genuine pre-exec file argument ledger required')
    for row in receipt['file_arguments']:
        need(type(row) is dict and POSTBOOT.same(set(row), {'argument_index', 'spelling', 'pin'}) and (type(row['argument_index']) is int) and (0 < row['argument_index'] < len(argv)) and POSTBOOT.same(argv[row['argument_index']], row['spelling']), 'pre-exec file argument/actual argv association differs')
        pin = row['pin']
        need(type(pin) is dict and POSTBOOT.same(set(pin), {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}), 'actual pre-exec full7 required')
        path = Path(pin['path'])
        spelled = Path(row['spelling'])
        spelled = spelled if spelled.is_absolute() else Path(proof['cell']) / spelled
        need(POSTBOOT.same(spelled.resolve(strict=False), path) and path.is_absolute(), 'actual file argument physical spelling differs')
        need(pin['path'] not in captured or POSTBOOT.same(captured[pin['path']], pin), 'duplicate pre-exec path has inconsistent identity')
        captured[pin['path']] = pin
        if path.exists() or path.is_symlink():
            current = frame.pin(path)
            need(POSTBOOT.same(current, pin), 'persistent actual Rust linker input changed')
            persistent[pin['path']] = current
        else:
            compiler_temp = path.is_relative_to(frame.out / 'tmp') or (POSTBOOT.same(path.parent, Path(proof['cell'])) and re.fullmatch('[A-Za-z0-9_.-]+\\.rcgu\\.o', path.name))
            need(compiler_temp and POSTBOOT.same(path.suffix, '.o') and (pin != proof['entry_object']), 'unproved disappeared linker input refused')
            temporary.append({'pre_exec_full7': pin, 'post_rustc_exists': False, 'scope': 'Actual compiler-owned temporary object hashed before linker exec; not a current retained input pin'})

    def historical_facts(pin):
        return {'physical_path': pin['path'], **{k: v for k, v in pin.items() if k != 'path'}}
    raw, tp = read(Path(trace['path']), 16 * MIB)
    need(POSTBOOT.same(tp, trace), 'actual trace changed during complete parsing')
    trace_entries = []
    seen = set()
    for line in raw.decode('utf-8', 'strict').splitlines():
        value = line.strip()
        if not value:
            continue
        candidate = value.split('(', 1)[0] if value.endswith(')') else value
        need(candidate and (not any((ch in candidate for ch in '\x00\t'))), 'invalid actual trace file syntax')
        path = Path(candidate)
        path = path if path.is_absolute() else Path(proof['cell']) / path
        path = path.resolve(strict=False)
        name = str(path)
        seen.add(name)
        if path.exists() or path.is_symlink():
            pin = frame.pin(path)
            if name in captured:
                need(POSTBOOT.same(pin, captured[name]), 'trace input differs from genuine pre-exec bytes')
            persistent[name] = pin
            trace_entries.append({'raw': line, 'current_full7': pin})
        else:
            matches = [row for row in temporary if POSTBOOT.same(row['pre_exec_full7']['path'], name)]
            need(POSTBOOT.same(len(matches), 1), 'actual trace names an uncaptured or unsupported missing file')
            trace_entries.append({'raw': line, 'pre_exec_temporary_opened_facts': historical_facts(matches[0]['pre_exec_full7']), 'current_file_present': False, 'scope': 'Historical actual pre-exec compiler temporary only'})
    need(trace_entries and proof['entry_object']['path'] in seen and POSTBOOT.member(proof['entry_object'], list(persistent.values())), 'actual retained Rust linker trace omitted direct persistent entry object')
    need(all((row['pre_exec_full7']['path'] in seen for row in temporary)), 'retired compiler object was not witnessed in actual linker trace')
    links = utilities.trace_inputs(frame, {'log': trace}, Path(proof['cell']))
    need(all((pin in persistent.values() for pin in links)), 'ordinary utility trace closure differs')
    return {'wrapper': proof['wrapper'], 'python': proof['python'], 'selected_linker': proof['linker'], 'selected_invocation_alias': proof['alias'], 'actual_linker_receipt': rp, 'actual_linker_argv': argv, 'actual_cwd': receipt['cwd'], 'actual_linker_pid': receipt['pid'], 'retained_stdout_trace': trace, 'actual_link_inputs': list(persistent.values()), 'actual_trace_entries': trace_entries, 'pre_exec_file_arguments_historical_facts': [{'argument_index': r['argument_index'], 'spelling': r['spelling'], 'opened_at_linker_barrier': historical_facts(r['pin'])} for r in receipt['file_arguments']], 'retired_compiler_temporaries_historical_only': [{'opened_at_linker_barrier': historical_facts(r['pre_exec_full7']), 'post_rustc_exists': False, 'scope': r['scope']} for r in temporary], 'rust_diagnostics_remain_in_original_compiler_log': True, 'same_PID_real_linker_exec': True}


FAILED_BATCH_PIN={'path':'/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source40-unitree-command14-build-r1/report.json',
 'bytes':3911428,'sha256':'b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747',
 'device':81,'inode':682208,'mtime_ns':1792120051801163862,'ctime_ns':1792120051808163972}
COMPLETED_RETAINED_IDS={language+'-'+kind+('-api' if kind=='topic-rpc' else '')
 for language in ('rust','javascript','assemblyscript') for kind in ('lowstate-go','lowstate-hg','topic-rpc')}
FRESH_CONTINUATION_IDS={'tinygo-lowstate-go','tinygo-lowstate-hg','tinygo-topic-rpc-api','c-topic-rpc-api','cpp-topic-rpc-api'}

def retained_input(frame,frozen,value):
    """Reopen selected historical persistent references through the explicit bridge."""
    frame.postboot.retained_input(value)


# Maintained exact historical-schema predicate; source provenance follows.
# {"bytes": 10856, "path": "/Users/liyinan/Documents/MacroModel/src/uwvm2-ros/tests/ros_unitree/acceptance/typed_command_retention.py", "sha256": "bf8ca6bcd0cd54d6b22fefed71e92f6ca2f8d86037939ee69dd8987ca8257ac5"}
REPORT_SHA256 = "b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747"

REPORT_BYTES = 3911428

SOURCE = "/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r40/source"

MANIFEST_SHA256 = "a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd"

MANIFEST_BYTES = 3404861

FULL_PIN_KEYS = frozenset(("path", "device", "inode", "bytes", "mtime_ns", "ctime_ns", "sha256"))

COMPLETE = "COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED"

COMPLETED_IDS = frozenset(language + "-" + role for language in
    ("rust", "javascript", "assemblyscript") for role in
    ("lowstate-go", "lowstate-hg", "topic-rpc-api"))

MIB = 1024 ** 2

def _need(condition, message):
    if not condition:
        raise ValueError(message)

def _pin(value, label, nonempty=True):
    """Validate an immutable historical pin's shape, never assert it is current."""
    _need(type(value) is dict and set(value) == FULL_PIN_KEYS,
          label + ": exact historical full-seven pin required")
    path = value["path"]
    _need(type(path) is str and path.startswith("/") and
          not any(item in path for item in ("\0", "\n", "\r")) and
          ".." not in PurePosixPath(path).parts, label + ": clean absolute path required")
    _need(all(type(value[key]) is int and value[key] >= 0 for key in
          ("device", "inode", "bytes", "mtime_ns", "ctime_ns")) and value["inode"] > 0,
          label + ": actual integer physical facts required")
    _need((not nonempty or value["bytes"] > 0) and type(value["sha256"]) is str and
          re.fullmatch("[0-9a-f]{64}", value["sha256"]) is not None,
          label + ": complete content identity required")
    return value

def _successful(command, label):
    """Require a genuinely completed original command, including its exit code."""
    _need(type(command) is dict and command.get("status") == "PASS" and
          type(command.get("returncode")) is int and command["returncode"] == 0 and
          type(command.get("argv")) is list and command["argv"] and
          type(command["argv"][0]) is str and command["argv"][0].startswith("/"),
          label + ": original successful command and recorded returncode0 required")

def admit_batch(report, opened_report_pin):
    """Select nine completed command records from the exact failed real batch.

    ``opened_report_pin`` is the caller's independently opened current report
    pin.  Only its actual path, size and complete SHA identify this immutable
    raw report; embedded historical device facts are retained unchanged.
    This predicate cannot qualify original files or promote the failed batch.
    """
    _pin(opened_report_pin, "current raw report")
    expected_report = str(PurePosixPath(SOURCE).parents[1] /
        "acceptance-storage-r1/source40-unitree-command14-build-r1/report.json")
    _need(opened_report_pin["path"] == expected_report and
          opened_report_pin["bytes"] == REPORT_BYTES and
          opened_report_pin["sha256"] == REPORT_SHA256,
          "the complete exact original failed Source40 report is required")
    _need(type(report) is dict and report.get("kind") == "uwvm-unitree-command14-guest-builds-v1" and
          report.get("status") == "FAIL" and report.get("format_version") == 1 and
          report.get("source") == SOURCE and report.get("source_manifest_sha256") == MANIFEST_SHA256 and
          report.get("error") == "ValueError('owned command RSS limit exceeded')" and
          "source_before" not in report and not report.get("finalization_errors") and
          all(report.get(key) is True for key in
              ("source_preserved", "input_pins_preserved", "whole_roots_preserved")),
          "the original preserved RSS-failed Frame report schema is required")
    _need(report.get("granted_rights") == [] and all(report.get(key) == "NOT_EXECUTED" for key in
          ("Wasm_initialization", "default_denial_cases", "positive_native_transport",
           "native_ROS", "simulator", "performance")),
          "retained compilation must not be relabelled as authority or Runtime acceptance")
    cleanup = report.get("cleanup")
    _need(type(cleanup) is list and cleanup and all(type(row) is dict and
          row.get("status") == "PASS" and not row.get("remaining") and not row.get("errors")
          for row in cleanup), "the original producer must have completed owned cleanup")
    pins = report.get("input_pins")
    _need(type(pins) is list and pins, "the original input pin ledger is required")
    manifest_path = str(PurePosixPath(SOURCE).parent / "source-manifest.json")
    manifest_pins = [row for row in pins if type(row) is dict and row.get("path") == manifest_path]
    _need(manifest_pins, "the original complete source manifest pin is required")
    historical_manifest = _pin(manifest_pins[0], "original full manifest")
    _need(all(row == historical_manifest for row in manifest_pins) and
          historical_manifest["bytes"] == MANIFEST_BYTES and
          historical_manifest["sha256"] == MANIFEST_SHA256,
          "all repeated original manifest observations must be identical")
    commands = report.get("commands")
    _need(type(commands) is list and len(commands) == 68,
          "all68 original compiler/helper/validator command records are required")
    failed = [row for row in commands if type(row) is dict and row.get("status") != "PASS"]
    _need(len(failed) == 1, "exactly one original command may have failed")
    failure = failed[0]
    _need(failure.get("label") == "tinygo-lowstate-go-same-main-command" and
          failure.get("status") == "FAIL" and
          failure.get("error") == "ValueError('owned command RSS limit exceeded')" and
          "returncode" not in failure and
          type(failure.get("peak_owned_rss_bytes")) is int and
          512 * MIB < failure["peak_owned_rss_bytes"] <= 768 * MIB,
          "retain the original RSS interruption; no process exit status was recorded")
    for command in commands:
        if command is not failure:
            _successful(command, "original command")
    products = report.get("products")
    _need(type(products) is list and len(products) == 10 and
          all(type(row) is dict for row in products),
          "the original inventory has nine completed products and one incomplete row")
    completed = [row for row in products if row.get("status") == COMPLETE]
    incomplete = [row for row in products if row.get("status") != COMPLETE]
    _need(len(completed) == 9 and {row.get("reactor_id") for row in completed} == COMPLETED_IDS and
          len(incomplete) == 1 and incomplete[0].get("reactor_id") == "tinygo-lowstate-go" and
          incomplete[0].get("status") == "RUNNING" and
          "artifact" not in incomplete[0] and "receipt" not in incomplete[0],
          "only the exact nine complete original products are eligible")
    for product in completed:
        identifier = product["reactor_id"]
        _need(product.get("id") == identifier + "-command" and
              product.get("language") == identifier.split("-", 1)[0] and
              product.get("granted_rights") == [] and
              product.get("future_transport_cases_executed") == 0 and
              product.get("cross_module_guard_modified") is False and
              product.get("preloaded_application") is False,
              "original product and language association differs")
        for field in ("source", "artifact", "receipt"):
            _pin(product.get(field), identifier + " " + field)
        _successful(product.get("validation"), identifier + " full validator")
        _need(type(product.get("commands")) is list and product["commands"],
              identifier + ": original compiler commands are required")
        for command in product["commands"]:
            _successful(command, identifier)
        entry = product.get("entry_compile")
        if entry is not None:
            _need(type(entry) is dict, identifier + ": original entry compile association required")
            _successful(entry.get("command"), identifier + " entry")
    _pin(report.get("configuration"), "original configuration")
    return {"status": "SCHEMA_ONLY_PERSISTENT_INPUTS_AND_BINARIES_REQUIRE_REOPEN",
            "raw_batch_status": "FAIL", "historical_manifest": historical_manifest,
            "configuration": report["configuration"],
            "completed": {row["reactor_id"]: row for row in completed},
            "incomplete_product_consumed": False, "batch_promoted_to_success": False,
            "missing_returncode_interpreted_as_exit_status": False}

def bind_current_source(report_schema, current_source_state):
    """Bind a complete current source_state result to the original manifest bytes.

    The caller must run the real complete ``common.source_state`` check first.
    A post-reboot physical transition must additionally be admitted by the
    separate restart bridge; matching content never asserts old device facts.
    """
    _need(report_schema.get("status") == "SCHEMA_ONLY_PERSISTENT_INPUTS_AND_BINARIES_REQUIRE_REOPEN" and
          type(current_source_state) is dict and current_source_state.get("root") == SOURCE and
          current_source_state.get("files") == 17928,
          "the actual complete current Source40 state is required")
    current = _pin(current_source_state.get("manifest"), "current complete manifest")
    historical = report_schema["historical_manifest"]
    _need(all(current[key] == historical[key] for key in ("path", "bytes", "sha256")),
          "complete current source must match the original manifest content")
    return {"status": "CURRENT_COMPLETE_SOURCE_BYTES_MATCH_ORIGINAL_MANIFEST",
            "historical_manifest": historical, "current_manifest": current,
            "files": current_source_state["files"], "historical_report_rewritten": False,
            "persistent_inputs_requalified": False, "runtime_execution_qualified": False}


# Explicit post-reboot reference comparisons; raw documents remain unchanged.
def retained_batch(frame, frozen, catalog, families, utilities, inspector, schema, js_helper, js_config, values):
    """Qualify exactly nine successful products from one preserved failed batch.

    The failed batch and its partial TinyGo row are historical failure evidence.
    Only nine independently complete command receipts are eligible. Reusing a
    complete artifact never changes its original producer, commands or receipt.
    This function starts no compiler, validator, Guest or native process.
    """
    selected = frame.input('typed_retained_failed_batch')
    failed, pin = doc(selected)
    need(POSTBOOT.same(pin, FAILED_BATCH_PIN) and POSTBOOT.same(FAILED_BATCH_PIN, frame.pin(selected)), 'the exact immutable actual failed Source40 batch is mandatory')
    schema_observation = admit_batch(failed, pin)
    source_observation = bind_current_source(schema_observation, frame.source_before)
    frame.report['retained_original_source_schema'] = source_observation
    old_root = Path(pin['path']).parent
    need(old_root in frame.roots and POSTBOOT.same(failed['kind'], 'uwvm-unitree-command14-guest-builds-v1') and POSTBOOT.same(failed['status'], 'FAIL') and POSTBOOT.same(failed['source'], str(frame.source)) and POSTBOOT.same(failed['source_manifest_sha256'], frame.report['source_manifest_sha256']) and POSTBOOT.same(source_observation['status'], 'CURRENT_COMPLETE_SOURCE_BYTES_MATCH_ORIGINAL_MANIFEST') and POSTBOOT.same(failed['error'], "ValueError('owned command RSS limit exceeded')") and (not failed.get('finalization_errors')) and all((failed.get(k) is True for k in ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved'))), 'closed actual RSS-only failed batch with preserved full source/inputs required')
    clean = failed.get('cleanup')
    clean = [clean] if type(clean) is dict else clean
    need(type(clean) is list and clean and all((POSTBOOT.same(r['status'], 'PASS') and (not r.get('remaining')) and (not r.get('errors')) for r in clean)), 'failed producer must have completed all owned cleanup')
    bad = [r for r in failed['commands'] if r['status'] != 'PASS']
    need(POSTBOOT.same(len(bad), 1) and POSTBOOT.same(bad[0]['label'], 'tinygo-lowstate-go-same-main-command') and ('returncode' not in bad[0]) and (512 * MIB < bad[0]['peak_owned_rss_bytes'] <= 768 * MIB), 'only the observed bounded TinyGo RSS failure may be continued')
    old_config, config_pin = doc(Path(failed['configuration']['path']))
    need((POSTBOOT.same(config_pin, failed['configuration']) and POSTBOOT.same(failed['configuration'], frame.pin(config_pin['path']))) and POSTBOOT.same(old_config['cgroup'], HISTORICAL_CG) and POSTBOOT.same(frame.config['cgroup'], CG) and POSTBOOT.same(old_config['limits'], frame.limits) and all((Path(root) in frame.roots for root in old_config['input_roots'])), 'the original configuration/full7 and every original complete root must be retained verbatim')
    need(any((POSTBOOT.same(p['sha256'], 'eaf7a375b7d69aa7883a2ea0e24373b4c90910733f8255ea8f338d57fe6c0f54') and POSTBOOT.same(p['bytes'], 74024) for p in failed['input_pins'])), 'failed products must retain the exact previously reviewed producer body')
    need(POSTBOOT.same(failed['source_recipes'], frame.pin(frame.source / RECIPE)) and POSTBOOT.same(failed['command_recipes'], frame.pin(frame.source / COMMAND_RECIPE)), 'the same actual current40 command and example recipes are required')
    for prior in failed['inventories_after']:
        matches = [r for r in frame.catalogs if POSTBOOT.same(r['root'], prior['root'])]
        if not matches:
            need(POSTBOOT.same(Path(prior['root']), old_root / 'official-cpp-object'), 'only the original producer-owned official object may have a derived subcatalog')
            current = frame.common.catalog(Path(prior['root']), frame.out / 'retained-official-cpp-current.jsonl', frame.roots)
            frame.pin(current['catalog']['path'])
            matches = [current]
        need(POSTBOOT.same(len(matches), 1) and POSTBOOT.tree_matches(prior, matches[0]), 'whole original failed-batch input package changed')
    for original in failed['input_pins']:
        retained_input(frame, frozen, original)
    need(POSTBOOT.same(len(failed['products']), 10), 'actual failed product inventory must be nine completed and one partial row')
    completed = {r['reactor_id']: r for r in failed['products'] if POSTBOOT.same(r['status'], 'COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED')}
    partial = [r for r in failed['products'] if r['status'] != 'COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED']
    need(POSTBOOT.same(set(completed), COMPLETED_RETAINED_IDS) and POSTBOOT.same(len(completed), 9) and POSTBOOT.same(len(partial), 1) and POSTBOOT.same(partial[0]['reactor_id'], 'tinygo-lowstate-go') and POSTBOOT.same(partial[0]['status'], 'RUNNING') and ('artifact' not in partial[0]) and ('receipt' not in partial[0]), 'partial TinyGo is never an eligible artifact; exactly nine genuine prior products are allowed')
    rows = {r['id']: r for r in catalog['products']}
    observations = []
    for identifier, product in completed.items():
        row = rows[identifier]
        family = families[row['family']]
        rp = frame.pin(product['receipt']['path'])
        receipt, actual_receipt = doc(Path(rp['path']))
        expected = {k: v for k, v in product.items() if k != 'receipt'}
        expected.update(format='uwvm-unitree-same-main-command-guest-build-receipt-v1', source_manifest_sha256=frame.report['source_manifest_sha256'], current_source=str(frame.source), observed_execution='NOT_EXECUTED')
        need((POSTBOOT.same(rp, actual_receipt) and POSTBOOT.same(actual_receipt, product['receipt'])) and POSTBOOT.same(receipt, expected) and POSTBOOT.same(product['source'], source_pin(frame, frozen, frame.source / row['source'])) and POSTBOOT.same(product['complete_family'], {k: v for k, v in family.items() if k not in ('directory', 'records')}) and POSTBOOT.same(product['language'], row['language']) and POSTBOOT.same(product['family'], row['family']) and POSTBOOT.same(product['complete_record_count'], row['complete_record_count']), 'complete original receipt/source/family/identity/codec association differs')
        artifact = Path(product['artifact']['path'])
        cell = old_root / identifier
        need(artifact.is_relative_to(cell) and POSTBOOT.same(Path(rp['path']), cell / 'build-receipt.json'), 'only actual original closed product locations are eligible')
        retained_input(frame, frozen, receipt)
        for association in product['owned_source_associations']:
            need('derivation' not in association and all((POSTBOOT.same(association['source'][key], association['copy'][key]) for key in ('bytes', 'sha256'))), 'retained SDK/generated-module stages must remain exact byte copies')
        for command in [*product['commands'], *([product['entry_compile']['command']] if product.get('entry_compile') else [])]:
            need(POSTBOOT.same(command['status'], 'PASS') and POSTBOOT.same(command['returncode'], 0) and command['argv'] and Path(command['argv'][0]).is_absolute(), 'every retained actual compiler/link/helper command must have succeeded')
        validation = product['validation']
        need(POSTBOOT.same(validation['status'], 'PASS') and POSTBOOT.same(validation['returncode'], 0) and POSTBOOT.same(validation['argv'], [values['WASM_TOOLS'], 'validate', str(artifact)]), 'the original genuine selected full validator invocation is required; no new invocation is invented')
        if product.get('entry_compile'):
            entry = product['entry_compile']
            command = entry['command']
            argv = command['argv']
            need(all((x in argv for x in ('-MD', '-MF', '-MT', '-c', '-o'))) and POSTBOOT.same(argv[argv.index('-c') + 1], entry['source']['path']) and POSTBOOT.same(argv[argv.index('-o') + 1], entry['object']['path']), 'retained direct C-entry compile/source/object association differs')
            deps = utilities.full_md(frame, Path(entry['dependency_file']['path']), Path(entry['object']['path']), Path(entry['source']['path']), Path(command['cwd']))
            need(POSTBOOT.same(deps, entry['full_MD']), 'retained direct entry actual complete MD rule changed')
        if POSTBOOT.same(row['language'], 'rust'):
            original = product['actual_rust_link']
            entry = product['entry_compile']
            proof = {'receipt_path': original['actual_linker_receipt']['path'], 'trace_path': original['retained_stdout_trace']['path'], 'cell': original['actual_cwd'], 'wrapper': original['wrapper'], 'python': original['python'], 'linker': original['selected_linker'], 'alias': original['selected_invocation_alias'], 'entry_object': entry['object'], 'artifact': str(artifact)}
            need(POSTBOOT.same(Path(proof['cell']), cell), 'actual original Rust compiler cell required')
            proxy_values = dict(vars(frame))
            proxy_values.update(out=old_root, pin=frame.pin)
            proxy = types.SimpleNamespace(**proxy_values)
            observed = qualify_rust_linker(proxy, proof, utilities)
            need(POSTBOOT.same(observed, original), 'retained actual Rust trace/pre-exec transient/persistent closure changed')
            for dep in product['dependency_files']:
                raw, dp = read(Path(dep['path']), 16 * MIB)
                need((POSTBOOT.same(dp, dep) and POSTBOOT.same(dep, frame.pin(dp['path']))) and raw, 'retained actual Rust dependency file changed')
        elif POSTBOOT.same(row['language'], 'javascript'):
            compiles = [r for r in product['commands'] if '-MD' in r['argv'] and '-MF' in r['argv']]
            need(POSTBOOT.same(len(compiles), len(product['full_MD'])) and POSTBOOT.same(len(product['full_MD']), 12), 'all twelve original actual JS engine translation units required')
            objects = []
            for command, declared in zip(compiles, product['full_MD']):
                argv = command['argv']
                need(all((x in argv for x in ('-MD', '-MF', '-MT', '-c', '-o'))) and '-MMD' not in argv, 'actual retained JS full-MD argv required')
                obj = Path(argv[argv.index('-o') + 1])
                objects.append(obj)
                observed = utilities.full_md(frame, Path(argv[argv.index('-MF') + 1]), obj, Path(argv[argv.index('-c') + 1]), Path(command['cwd']))
                need(POSTBOOT.same(observed, declared), 'retained actual JS MD closure changed')
            links = [r for r in product['commands'] if '-Wl,--trace' in r['argv']]
            need(POSTBOOT.same(len(links), 1), 'one original JS linker trace required')
            observed = js_helper.trace_closure(types.SimpleNamespace(common=frame.common, limits=frame.limits, pin=frame.pin, inputs=js_config['inputs']), links[0], [*objects, Path(product['entry_compile']['object']['path'])])
            need(POSTBOOT.same(observed, product['link_inputs']), 'retained real JS object/archive trace closure changed')
        else:
            need(POSTBOOT.same(row['language'], 'assemblyscript') and POSTBOOT.same(len(product['commands']), 1), 'only the three real AS whole-package source/validator closures remain in the retained set')
        observed = inspect_command_binary(frame, utilities, inspector, schema, row, artifact, validation)
        need(all((POSTBOOT.same(observed[key], product[key]) for key in observed)), 'current reopened retained binary/import/entry/memory differs from its original observed table')
        observations.append({'reactor_id': identifier, 'original_receipt': rp, 'actual_artifact': product['artifact'], 'original_validator': validation, 'full_MD_or_language_closure_reopened': True, 'actual_binary_reparsed': True, 'new_validator_or_compiler_executed': False})
    frame.report.update(retained_failure=pin, retained_original_configuration=config_pin, retained_nine_products_qualification=observations, retained_command_products=9, failed_batch_promoted_to_success=False, failed_partial_TinyGo_consumed_as_product=False, retained_whole_output_root=str(old_root))
    frame.save()
    return {'failure': pin, 'document': failed, 'products': completed, 'old_root': old_root}

# Explicit post-reboot reference comparisons; raw documents remain unchanged.
def retained_official_object(frame, values, retained):
    """Consume the independent actual40 official object without recompilation."""
    original = retained['document']['official_cpp_object']
    expected = original['qualification']
    need(POSTBOOT.same(original['separate_object_compiles'], 1) and POSTBOOT.same(expected['report']['path'], str(retained['old_root'] / 'official-cpp-object/report.json')), 'original independent official object producer association differs')
    qualifier = frame.input('typed_cpp_qualifier')
    module, _ = load(qualifier, 'typed40_retained_official_cpp_qualifier', CPP_QUALIFIER, frame)
    value = module.qualify_overlay(frame, Path(expected['report']['path']), expected['report']['sha256'], values)
    need(POSTBOOT.same(value['report'], expected['report']) and POSTBOOT.same(value['object'], expected['object']) and (POSTBOOT.same(value['producer'], expected['producer']) and POSTBOOT.same(expected['producer'], frame.pin(expected['producer']['path']))) and POSTBOOT.same(value['consumer_qualifier'], frame.pin(qualifier)), 'actual retained original official object/current consumer differs')
    frame.report['official_cpp_object'] = {'qualification': value, 'separate_object_compiles': 0, 'retained_objects': 1, 'Guest_products': 0, 'original_independent_command': original['command'], 'original_independent_report': expected['report'], 'scope': 'Independent completed Source40 official termination object retained as library input; original failed Guest batch remains FAIL'}
    frame.save()
    return value


# Explicit post-reboot reference comparisons; raw documents remain unchanged.
def build(frame, frozen, args, utilities):
    need(POSTBOOT.same(frame.config['cgroup'], CG) and POSTBOOT.same(frame.limits, {'jobs': 1, 'command_seconds': 600, 'rss_bytes': 512 * MIB, 'log_bytes': 16 * MIB, 'output_bytes': 4 * 1024 ** 3}) and (not frame.config['upstreams']) and all((frame.config[name] is None for name in ('simulation', 'benchmark', 'profiler'))), 'unchanged512MiB/600s/j1 compiler-only current64 configuration required')
    need(set(frame.config['environment']) <= {'PATH', 'LD_LIBRARY_PATH'}, 'compiler-only explicit PATH/loader environment required')
    plan, _ = doc(frame.input('example_plan'))
    values, aliases = utilities.select_inputs(frame, plan)
    rust = admit_rust(frame, values)
    inventory, inventory_pin = doc(frame.input('native_source_inventory'))
    families = admit_families(frame, frozen, inventory)
    catalog, catalog_pin = doc(frame.source / RECIPE)
    source_pin(frame, frozen, Path(catalog_pin['path']))
    need(POSTBOOT.same(catalog['format'], 'uwvm-unitree-typed-example-recipes-v1') and POSTBOOT.same(catalog['status'], 'SOURCE_RECIPES_NOT_EXECUTED') and POSTBOOT.same(catalog['default_authority'], []) and POSTBOOT.same(len(catalog['products']), 14), 'actual frozen closed14 source recipe inventory required')
    for row, (identifier, language, kind, family) in zip(catalog['products'], EXPECTED):
        need(POSTBOOT.same((row['id'], row['language'], row['family']), (identifier, language, family)) and POSTBOOT.same(row['role'], 'lowstate' if kind.startswith('lowstate') else 'topic-rpc') and POSTBOOT.same(row['backend'], 2) and POSTBOOT.same(row['complete_record_count'], {'unitree_go': 24, 'unitree_hg': 11, 'unitree_api': 8}[family]) and POSTBOOT.same(row['poll_limit'], 4096), 'all and only fourteen fixed current typed products required')
    commands, command_pin = doc(frame.source / COMMAND_RECIPE)
    source_pin(frame, frozen, Path(command_pin['path']))
    need(POSTBOOT.same(commands['kind'], 'uwvm-unitree-same-main-command-recipes-v1') and POSTBOOT.same(commands['status'], 'SOURCE_RECIPES_NOT_EXECUTED') and POSTBOOT.same(commands['default_authority'], []) and POSTBOOT.same(commands['command_products'], 14) and POSTBOOT.same(len(commands['products']), 14), 'exact fourteen current command harness recipes required')
    need(POSTBOOT.same([(r['reactor_id'], r['language'], r['family'], r['actual_example']) for r in commands['products']], [(r['id'], r['language'], r['family'], r['source']) for r in catalog['products']]), 'command entries must retain exact actual fourteen example/source/family associations')
    inspector = utilities.admitted(frame, frozen, 'sdk/tools/check_wasm_imports.py', 'command14_actual_wasm_inspector')
    schema, _ = doc(frame.source / 'sdk/abi/abi-v1.json')
    source_pin(frame, frozen, frame.source / 'sdk/abi/abi-v1.json')
    js_config, jcp = doc(frame.input('embedded_js_inputs'))
    jp, jpp = doc(frame.input('typed_js_preparation'))
    need(POSTBOOT.same(js_config['schema'], 'uwvm-embedded-js-build-inputs-v1') and POSTBOOT.same(js_config['cgroup'], HISTORICAL_CG) and POSTBOOT.same(jp['schema'], 'uwvm-embedded-js-build-input-preparation-v1') and POSTBOOT.same(jp['status'], 'PREPARED_JS_BUILD_INPUTS_ONLY') and POSTBOOT.same(jp['source_manifest']['sha256'], SOURCE37) and POSTBOOT.same(Path(jp['source_manifest']['path']), Path('/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r37/source-manifest.json')) and POSTBOOT.same(jp['config'], jcp) and all((jp.get(k) is True for k in ('physical_pins_preserved', 'source_preserved', 'whole_inputs_preserved'))) and (not jp.get('finalization_errors')) and (jp['compilers_started'] is False) and (jp['runtime_executed'] is False), 'exact actual Source37 compiler-input metadata baseline required; never relabel its source')
    previous_manifest = frame.pin(jp['source_manifest']['path'])
    need(POSTBOOT.same(previous_manifest, jp['source_manifest']), 'actual historical Source37 full manifest pin changed')
    for root in js_config['input_roots']:
        need(Path(root).resolve(strict=True) in frame.roots, 'every current JS compiler/source root must remain complete')
    for pin in js_config['inputs'].values():
        need(all((POSTBOOT.same(frame.pin(pin['path'])[k], pin[k]) for k in ('path', 'bytes', 'sha256'))), 'actual selected JS tool/source changed')
    js_helper, _ = load(Path(js_config['inputs']['driver']['path']), 'typed14_real_embedded_binary_helpers', JS_DRIVER, frame)
    retained = retained_batch(frame, frozen, catalog, families, utilities, inspector, schema, js_helper, js_config, values)
    owner = frame.common.SerialRunner(frame.out, frame.limits, frame.guard)
    frame.actors.append(owner)
    environment = dict(frame.environment)
    environment.update(CARGO_NET_OFFLINE='true', RUSTUP_AUTO_INSTALL='0', GOTOOLCHAIN='local', GOPROXY='off', GOSUMDB='off', GOENV='off', GOWORK='off', GOFLAGS='-mod=readonly', GOMAXPROCS='1', GOMEMLIMIT='192MiB', GOROOT=values['GO_ROOT'], TINYGOROOT=values['TINYGO_ROOT'], BINARYEN_CORES='1', RAYON_NUM_THREADS='1', MAKEFLAGS='-j1')
    selected_paths = list(dict.fromkeys((str(Path(values[key]).parent) for key in ('GO', 'TINYGO', 'RUSTC', 'WASM_LD', 'WASI_CLANG', 'NODE'))))
    environment['PATH'] = ':'.join(selected_paths + environment['PATH'].split(':'))
    for name in ('PATH', 'LD_LIBRARY_PATH'):
        if name not in environment:
            continue
        for directory in environment[name].split(':'):
            path = Path(directory)
            need(directory and path.is_absolute() and path.is_dir() and any((path.resolve(strict=True).is_relative_to(r) or POSTBOOT.same(path.resolve(strict=True), r) for r in frame.roots)), 'explicit actual complete-package-covered ' + name + ' only; no implicit CWD or loader discovery')
    for name in ('go-cache', 'go-path', 'go-tmp'):
        (frame.out / name).mkdir(mode=448)
    environment.update(GOCACHE=str(frame.out / 'go-cache'), GOPATH=str(frame.out / 'go-path'), GOTMPDIR=str(frame.out / 'go-tmp'))
    frame.report.update(commands=owner.commands, products=[], scope='Actual Source40 nine individually complete retained commands plus five fresh commands; original failed batch remains FAIL; binary-only qualification', source_recipes=catalog_pin, command_recipes=command_pin, native_source_inventory=inventory_pin, rust_prepared_inputs=rust, historical_js_compiler_metadata=jpp, historical_js_compiler_source_manifest=previous_manifest, historical_js_metadata_scope='Source37 tool acquisition only; fresh Source40 prepare and compilation required', original_native_producer=inventory['native_producer'], native_definition_records=43, legacy_native_compatibility='UNQUALIFIED', source33_native_definition_bridge=True, source40_fresh_Guest_compilation=False, source40_all_products_current=True, granted_rights=[], default_denial_cases='NOT_EXECUTED', positive_native_transport='NOT_EXECUTED', Wasm_initialization='NOT_EXECUTED', native_ROS='NOT_EXECUTED', simulator='NOT_EXECUTED', performance='NOT_EXECUTED', planned_Guest_products=14)

    def run(label, argv, cwd, cpp_supervisor=False):
        tinygo_job = label in {identifier + '-same-main-command' for identifier in FRESH_CONTINUATION_IDS if identifier.startswith('tinygo-')}
        need(not cpp_supervisor, 'continuation retains the independent official object; no fresh nested producer may start')
        selected = {**frame.limits, 'rss_bytes': 768 * MIB} if tinygo_job else dict(frame.limits)
        selected_environment = {**environment, **({'GOMEMLIMIT': '128MiB'} if tinygo_job else {})}
        if cpp_supervisor:
            need(POSTBOOT.same(label, 'typed-official-cpp-object'), 'only the single full producer may select a supervisor budget')
            frame.report['official_cpp_supervision'] = {'owned_subtree_rss_bytes': 768 * MIB, 'whole_producer_seconds': 1800, 'inner_compiler_rss_bytes': 512 * MIB, 'inner_compiler_seconds': 600, 'required_root_outer_owned_tree_rss_bytes': 1536 * MIB}
            frame.save()
        owner.limits = selected
        try:
            result = utilities.run(frame, owner, label, argv, cwd, selected_environment)
            result['effective_supervision_limits'] = selected
            frame.save()
            return result
        finally:
            owner.limits = frame.limits
    overlay = retained_official_object(frame, values, retained)
    for row in catalog['products']:
        if row['id'] in retained['products']:
            product = copy.deepcopy(retained['products'][row['id']])
            product['compilation_provenance'] = {'mode': 'RETAINED_COMPLETED_PRODUCT_FROM_FAILED_BATCH', 'original_report': retained['failure'], 'original_receipt': product['receipt']}
            product['current_file_qualification']={'artifact':POSTBOOT.pin(product['artifact']),'receipt':POSTBOOT.pin(product['receipt']),'source':POSTBOOT.pin(product['source']),'original_receipt_unchanged':True,'Runtime_executed':False}
            frame.report['products'].append(product)
            frame.save()
            continue
        need(row['id'] in FRESH_CONTINUATION_IDS, 'only three TinyGo and C/C++ API commands compile in this continuation')
        frame.guard()
        cell = frame.out / row['id']
        cell.mkdir(mode=448)
        stage = cell / 'source'
        family = families[row['family']]
        product = {'id': row['id'] + '-command', 'reactor_id': row['id'], 'language': row['language'], 'family': row['family'], 'status': 'RUNNING', 'source': source_pin(frame, frozen, frame.source / row['source']), 'complete_family': {k: v for k, v in family.items() if k not in ('directory', 'records')}, 'complete_record_count': row['complete_record_count'], 'commands': [], 'future_modes': ['default-denied', 'explicit-positive'], 'future_transport_cases_executed': 0}
        frame.report['products'].append(product)
        frame.save()
        associations = stage_product(frame, frozen, row, family, stage)
        command_entry = stage_command_sources(frame, frozen, row, stage, associations)
        entry = None if POSTBOOT.same(row['language'], 'javascript') else compile_command_entry(frame, frozen, row, cell, run, values, utilities)
        product.update(command_entry=command_entry, entry_compile=entry)
        mapping = {**values, 'WASM_LD_ALIAS': values['WASM_LD'], 'SOURCE': str(frame.source), 'OUT': str(cell), 'STAGE': str(stage), 'ACTUAL_COMPLETE_API_GENERATION': str(families['unitree_api']['directory']), 'QUALIFIED_OFFICIAL_CPP_TERMINATION_OBJECT': overlay['object']['path']}
        language = row['language']
        artifact = cell / 'guest.wasm'
        validation = None
        if language in ('c', 'cpp'):
            jobs = [{**job, 'argv': [expand(x, mapping) for x in job['argv']]} for job in row['commands']]
            need(POSTBOOT.same([j['id'] for j in jobs], ['compile', 'link']), 'separate actual C/C++ compile/link required')
            compiled = run(row['id'] + '-compile', jobs[0]['argv'], cell)
            obj, dep = (cell / 'reactor.o', cell / 'reactor.d')
            dependencies = utilities.full_md(frame, dep, obj, frame.source / row['source'], cell)
            required = family['directory'] / ('bounded_records.hpp' if POSTBOOT.same(language, 'cpp') else 'bounded_records.h')
            need(str(required) in {p['path'] for p in dependencies}, 'actual C/C++ fullMD omitted complete API module')
            frame.pin(obj)
            frame.pin(dep)
            link = list(jobs[1]['argv'])
            need(entry is not None, 'actual C/C++ entry object required')
            link.insert(link.index('-o'), entry['object']['path'])
            link.insert(link.index('-o'), '-Wl,--export=_start')
            why = cell / 'why-extract.tsv'
            if POSTBOOT.same(language, 'cpp'):
                link.append('-Wl,--why-extract=' + str(why))
            linked = run(row['id'] + '-link', link, cell)
            links = utilities.trace_inputs(frame, linked, cell)
            need(frame.pin(obj) in links and entry['object'] in links, 'actual link trace omitted fresh example or entry object')
            product.update(commands=[compiled, linked], full_MD=dependencies, dependency_file=frame.pin(dep), object=frame.pin(obj), link_inputs=links)
            if POSTBOOT.same(language, 'cpp'):
                product['official_termination'] = cpp_link_evidence(frame, utilities, linked, why, overlay, links)
        elif POSTBOOT.same(language, 'rust'):
            jobs = []
            for command in row['commands']:
                argv = [expand(x, mapping) for x in command['argv']]
                argv[1:1] = ['--sysroot', values['RUST_SYSROOT']]
                if POSTBOOT.same(command['id'], 'reactor'):
                    need(entry is not None, 'actual Rust same-module C entry required')
                    argv[1:1] = ['-C', 'link-arg=' + entry['object']['path'], '-C', 'link-arg=--export=_start']
                    rust_linker = prepare_rust_linker(frame, cell, values, entry, utilities)
                    original = 'linker=' + values['WASM_LD']
                    need(POSTBOOT.same(argv.count(original), 1), 'exact original selected Rust linker required')
                    argv[argv.index(original)] = 'linker=' + rust_linker['wrapper']['path']
                job = run(row['id'] + '-' + command['id'], argv, cell)
                if POSTBOOT.same(command['id'], 'reactor'):
                    product['actual_rust_link'] = qualify_rust_linker(frame, rust_linker, utilities)
                jobs.append(job)
            product.update(commands=jobs, sdk_rlib=frame.pin(cell / 'libroswasm.rlib'), dependency_files=[frame.pin(cell / 'sdk.d'), frame.pin(cell / 'reactor.d')], recipe_adaptation={'SDK_sysroot_tokens': ['--sysroot', values['RUST_SYSROOT']], 'command_link_tokens': ['-C', 'link-arg=' + entry['object']['path'], '-C', 'link-arg=--export=_start'], 'owned_linker_wrapper': rust_linker['wrapper'], 'real_linker_invocation_preserved': rust_linker['alias']}, closure='Actual Rust depfiles plus complete SDK/sysroot/current-source inventories; not a fabricated Clang MD')
        elif POSTBOOT.same(language, 'tinygo'):
            target, target_proof = derive_tinygo_target(frame, values, cell)
            mapping['OWNED_PINNED_BOUNDED_WASM_UNKNOWN_JSON'] = str(target)
            command = row['commands'][0]
            argv = [expand(x, mapping) for x in command['argv']]
            need(POSTBOOT.same(argv.count('-buildmode=c-shared'), 1), 'exact original reactor buildmode expected')
            argv[argv.index('-buildmode=c-shared')] = '-buildmode=default'
            cwd = Path(expand(command['cwd'], mapping))
            job = run(row['id'] + '-same-main-command', argv, cwd)
            need(POSTBOOT.same(frame.pin(target), target_proof['owned']) and POSTBOOT.same(frame.pin(target_proof['original']['path']), target_proof['original']), 'actual bounded target changed')
            product.update(commands=[job], derived_target=target_proof, closure='Actual complete Go/TinyGo packages, source module graph and owned target; no invented Clang MD or startup result')
        elif POSTBOOT.same(language, 'assemblyscript'):
            command = row['commands'][0]
            argv = [expand(x, mapping) for x in command['argv']]
            need(POSTBOOT.same(argv.count(str(stage / row['source'])), 1) and POSTBOOT.same(argv.count('--exportStart'), 1) and POSTBOOT.same(argv[argv.index('--exportStart') + 1], '_initialize'), 'exact original AS reactor source and initializer recipe required')
            argv[argv.index(str(stage / row['source']))] = command_entry['staged_entry']
            argv[argv.index('--exportStart') + 1] = 'typed_command_entry'
            product.update(commands=[run(row['id'] + '-same-main-command', argv, stage)], closure='Actual complete Node/AssemblyScript packages and unchanged staged full SDK/generated modules; no invented Clang MD')
        else:
            artifact, validation, details = compile_javascript(frame, frozen, utilities, run, js_helper, js_config, catalog, row, stage, cell, schema, values)
            product.update(details)
        if validation is None:
            argv = [values['WASM_TOOLS'], 'validate', str(artifact)]
            need(POSTBOOT.same([expand(x, mapping) for x in row['validation']], argv), 'exact selected full binary validator required')
            validation = run(row['id'] + '-full-validate', argv, cell)
        product.update(owned_source_associations=associations, **inspect_command_binary(frame, utilities, inspector, schema, row, artifact, validation))
        receipt = {k: v for k, v in product.items() if k != 'receipt'}
        receipt.update(format='uwvm-unitree-same-main-command-guest-build-receipt-v1', source_manifest_sha256=frame.report['source_manifest_sha256'], current_source=str(frame.source), observed_execution='NOT_EXECUTED')
        rp = put(frame, cell / 'build-receipt.json', json.dumps(receipt, indent=2, default=str) + '\n')
        product['receipt'] = rp
        product['compilation_provenance'] = {'mode': 'FRESH_IN_CONTINUATION'}
        frame.save()
    need(POSTBOOT.same(len(frame.report['products']), 14) and all((POSTBOOT.same(row['status'], 'COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED') for row in frame.report['products'])) and POSTBOOT.same(sum((POSTBOOT.same(row['compilation_provenance']['mode'], 'RETAINED_COMPLETED_PRODUCT_FROM_FAILED_BATCH') for row in frame.report['products'])), 9) and POSTBOOT.same(sum((POSTBOOT.same(row['compilation_provenance']['mode'], 'FRESH_IN_CONTINUATION') for row in frame.report['products'])), 5) and POSTBOOT.same(len(owner.commands), 14) and all((POSTBOOT.same(row['status'], 'PASS') and POSTBOOT.same(row['returncode'], 0) for row in owner.commands)), 'all fourteen qualified commands require exactly nine retained, five fresh and fourteen new real tool jobs')
    need(all((POSTBOOT.same(utilities.alias(alias['path']), alias) for alias in aliases)), 'selected invocation aliases changed')
    frame.report.update(compiled_command_products=14, compiled_Guest_products=14, retained_command_products=9, fresh_command_products=5, separate_official_object_compiles=0, retained_official_object_count=1, actual_native_definition_records=43, future_independent_Runtime_cases=28, actual_Runtime_cases_executed=0, tool_invocation_aliases=aliases, scope='Source40 nine independently complete retained commands plus five fresh command builds; original failure and raw receipts preserved; no initializer/body/default-denial/positive native delivery/robot control/performance result')
    return 'PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY'


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ('source','manifest','config','out','utilities'):parser.add_argument('--'+name,type=Path,required=True)
    for name in ('manifest-sha256','config-sha256','runner-sha256'):parser.add_argument('--'+name,required=True)
    args=parser.parse_args();frame=None;status='FAIL';error=None;state={'finalizing':False};started=time.monotonic()
    try:
        need(sys.platform=='linux' and os.geteuid()==0 and all(getattr(args,n).is_absolute() for n in ('source','manifest','config','out','utilities')) and
             re.fullmatch('[0-9a-f]{64}',args.manifest_sha256) and
             (EXPECTED_SOURCE40_MANIFEST is None or args.manifest_sha256==EXPECTED_SOURCE40_MANIFEST) and re.fullmatch('[0-9a-f]{64}',args.config_sha256) and re.fullmatch('[0-9a-f]{64}',args.runner_sha256),
             'Root-only exact current Source40/explicit full config and worker SHA required')
        own,own_pin=read(Path(__file__).resolve(),2*MIB);need(own_pin['sha256']==args.runner_sha256,'reviewed typed producer differs')
        frozen,mf_pin=doc(args.manifest.resolve(strict=True));need(mf_pin['sha256']==args.manifest_sha256,'initial actual published Source40 MF changed')
        config,config_pin=doc(args.config.resolve(strict=True));need(config_pin['sha256']==args.config_sha256,'initial actual config changed')
        source=args.source.resolve(strict=True);need(source==Path('/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r40/source'),'do not relabel old source as Source40')
        helper=source/'tests/ros_unitree/common.py';raw,hp=read(helper,MIB)
        need(frozen.get('tests/ros_unitree/common.py')=={k:hp[k] for k in ('bytes','sha256')},'actual Frame absent from current full MF')
        module=types.ModuleType('typed14_current_frozen_frame');module.__file__=str(helper);sys.modules[module.__name__]=module
        exec(compile(raw,str(helper),'exec'),module.__dict__)
        class TypedFrame(module.Frame):
            def pin(self,path):
                physical=Path(path).resolve(strict=True)
                value=self.common.opened(physical)
                current_manifest=physical==self.manifest and value['sha256']==args.manifest_sha256
                historical_js_manifest=((physical==Path('/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r35/source-manifest.json') and value['sha256']==SOURCE35) or
                                        (physical==Path('/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r37/source-manifest.json') and value['sha256']==SOURCE37))
                need(physical.is_relative_to(self.source) or physical.is_relative_to(self.out) or
                     any(physical.is_relative_to(root) for root in self.roots) or current_manifest or historical_js_manifest,
                     'opened physical pin lacks complete source/owned/input-root coverage: '+str(physical))
                self.pins.append(value);return value
            def guard(self):
                super().guard()
                need(state['finalizing'] or not self.cancelled['cancelled'],'cooperative cancellation requested')
                need(state['finalizing'] or resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024<=512*MIB,'coordinator512MiB peak bound')
                need(state['finalizing'] or time.monotonic()-started<=25200,'finite7hour whole typed build bound')
        frame=TypedFrame(args,'uwvm-unitree-command14-continuation-builds-v1')
        frame.report['source40_manifest_binding']={'expected_at_authoring':EXPECTED_SOURCE40_MANIFEST,
            'actual_explicit_cli_sha256':args.manifest_sha256,'full_published_manifest_required':True}
        for pin in (own_pin,mf_pin,config_pin,hp):need(frame.pin(pin['path'])==pin,'initial full7 admission changed')
        need(any(Path(own_pin['path']).is_relative_to(root) for root in frame.roots),'new private worker must have a complete explicit control root')
        utilities,_=load(args.utilities.resolve(strict=True),'typed14_reviewed_serial_and_binary_utilities',UTILITIES,frame)
        need(any(args.utilities.resolve(strict=True).is_relative_to(root) for root in frame.roots),'actual reviewed helper needs full control-root inventory')
        postboot_module,_=load(Path(__file__).with_name('typed-command-postboot.py'),'typed40_current_postboot','d66fc6b078336dc5239d19e60f92d5d3b73a0221a9b1baaed9d59ba80f6ed7aa',frame)
        frame.postboot=postboot_module.attach(frame,frozen,globals(),utilities)
        status=build(frame,frozen,args,utilities);frame.guard()
    except BaseException as caught:
        status='FAIL';error=repr(caught)
        if frame is not None:frame.report['traceback']=traceback.format_exc()
    finally:
        if frame is not None:
            state['finalizing']=True
            if hasattr(frame,'postboot'):
                try:frame.postboot.preserve()
                except BaseException as caught:status='FAIL';error=repr(caught)
            result=frame.finish(status,error)
    if frame is None:raise RuntimeError(error)
    return result


if __name__=='__main__':raise SystemExit(main())
