#!/usr/bin/env python3
"""Build four fresh native interpreter profiles without executing a Worker or ROS.

Execute this maintained producer from a complete frozen source and an explicitly
pinned configuration. It uses the source-bound cgroup/pidfd/catalog primitives
and debugger builder's ELF/archive/full-MD readers without invoking their main
routines or selecting an LLVM Runtime provider. Every profile compiles the real
three Runtime translation units; custom registry headers must occur in actual
full-MD output. This is compilation evidence, never ROS transport acceptance.

The original private producer is preserved separately as historical evidence.
This maintained successor requires its own manifest entry and live current
cgroup before importing project helpers. It neither discovers SDKs nor grants
Guest authority. See INTERPRETER_WORKERS.md for the closed input contract.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import resource
import stat
import sys
import time
import traceback
import types

sys.dont_write_bytecode=True
MIB=1024**2;GIB=1024**3
PROFILES=('none','ros1','ros2','dual')
BUILD_ORDER=('dual','none','ros1','ros2')
MAIN_PASS_WORKAROUND=('-mllvm','-disable-machine-dce')
COMPILER_WORKAROUND_PROVENANCE={'scope':'EXPLICIT_MAIN_TU_COMPILER_PASS_WORKAROUND_ONLY_NOT_RUNTIME_SOURCE_OR_PERMISSION_CHANGE',
    'affected_translation_unit':'src/uwvm2/uwvm/main.default.cpp','argv':list(MAIN_PASS_WORKAROUND),
    'reported_clang_commit':'4c4c1db7c69a6fda6cfa6bc6066bb09a433edc89',
    'official_definition_url':'https://github.com/llvm/llvm-project/blob/4c4c1db7c69a6fda6cfa6bc6066bb09a433edc89/llvm/lib/CodeGen/TargetPassConfig.cpp',
    'Root_reported_definition_lines':[73,314],
    'historical_failed_inner_report_sha256':'391e7bd238af36043388f5b4d6011ce350ddf164dd532363df684e0d2b8e00f0',
    'historical_failure_packet_sha256':'1de2ea54c9cfe71f4ce8723be1c62799da0bab0da116965128091232c64a29d6',
    'historical_scope':'Root-supplied failed compile/official option references only; not current input full pins or a successful option/build result',
    'runtime_and_host_api_machine_passes':'UNCHANGED','O1_and_UNDEBUG':'UNCHANGED'}
UNITS=(('runtime','src/uwvm2/runtime/lib/uwvm_runtime.default.cpp'),
       ('main','src/uwvm2/uwvm/main.default.cpp'),('host_api','src/uwvm2/uwvm/host_api.default.cpp'))
LIMITS={'jobs':1,'rss_bytes':5*GIB,'command_seconds':600,'log_bytes':32*MIB}
PIN_KEYS={'path','device','inode','bytes','mtime_ns','ctime_ns','sha256'}


def need(value,message):
    if not value:raise ValueError(message)


def pairs(items):
    result={}
    for key,value in items:
        need(key not in result,'duplicate JSON key: '+key);result[key]=value
    return result


def read(path,limit):
    path=Path(path);need(path.is_absolute() and path.resolve(strict=True)==path,'physical absolute control required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=limit,'bounded regular control required')
        raw=bytearray()
        while True:
            data=os.read(fd,min(65536,limit+1-len(raw)))
            if not data:break
            raw.extend(data);need(len(raw)<=limit,'control grew beyond its bound')
        last=os.fstat(fd);named=path.stat(follow_symlinks=False)
        fields=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')
        need(len(raw)==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in fields),
             'opened control changed')
        return bytes(raw),{'path':str(path),'device':first.st_dev,'inode':first.st_ino,'bytes':len(raw),
            'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns,'sha256':hashlib.sha256(raw).hexdigest()}
    finally:os.close(fd)


def document(path,limit=16*MIB):
    raw,pin=read(path,limit)
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite JSON')),pin


def module(source,entries,relative,name):
    raw,pin=read(source/relative,512*1024)
    need(entries.get(relative)=={key:pin[key] for key in ('bytes','sha256')},'helper absent from actual frozen MF: '+relative)
    value=types.ModuleType(name);value.__file__=str(source/relative);sys.modules[name]=value
    exec(compile(raw,value.__file__,'exec'),value.__dict__)
    return value,pin


def closed(value,keys,label):
    need(type(value) is dict and set(value)==set(keys),'closed '+label+' schema required')


def inside(path,roots):
    return any(path==root or path.is_relative_to(root) for root in roots)


def aliases(path):
    """Keep invocation basenames; Clang/LLD multicall names are significant."""
    path=Path(path);need(path.is_absolute() and path.parent.resolve(strict=True)==path.parent,'canonical invocation parent required')
    state=path.lstat()
    return {'path':str(path),'device':state.st_dev,'inode':state.st_ino,'mode':state.st_mode,
        'mtime_ns':state.st_mtime_ns,'ctime_ns':state.st_ctime_ns,
        'target':os.readlink(path) if path.is_symlink() else None,'physical':str(path.resolve(strict=True))}


def configuration(value,source,builder):
    """Consume Root's actual native tool selection, never an LLVM package.

    The same eight-tool metadata configuration used by the dependency preparer
    is accepted. source_root is only a catalog input, not an include/library or
    compiler source. No package label substitutes for a fresh native compile.
    """
    keys={'format_version','source_root','tools','target','stdlib','cflags','cxxflags','link_flags',
          'sysroot','input_roots','environment','cgroup','build_seconds'}
    need(type(value) is dict and set(value) in (keys,keys|{'profile_compile_flags'}),'closed native configuration required')
    need(type(value['format_version']) is int and value['format_version']==1,'configuration version1 required')
    closed(value['tools'],('cc','cxx','cmake','ninja','ar','ranlib','linker','python'),'tools')
    closed(value['target'],('triple','machine','bits','endian','host_machine'),'target')
    target=value['target']
    need(platform.machine().lower() in ('x86_64','amd64') and target['host_machine'] in ('x86_64','amd64') and
         (target['machine'],target['bits'],target['endian'])==(62,64,'little') and
         target['triple'] in ('x86_64-unknown-linux-gnu','x86_64-linux-gnu'),
         'actual native Linux ELF62 target required; no cross/QEMU relabel')
    roots=value['input_roots']
    need(type(roots) is list and 1<=len(roots)<=32 and all(type(x) is str for x in roots),'explicit complete roots required')
    roots=[Path(x) for x in roots]
    need(len(set(roots))==len(roots) and all(x.is_absolute() and x.resolve(strict=True)==x and x.is_dir() and x!=Path('/')
         and not inside(x,[source]) and not inside(source,[x]) for x in roots),'canonical disjoint whole input roots required')
    tools={}
    for name,token in value['tools'].items():
        need(type(token) is str and '\0' not in token,'explicit tool path required')
        path=Path(token);alias=aliases(path);physical=Path(alias['physical'])
        need(physical.is_file() and os.access(path,os.X_OK) and inside(physical,roots),'tool outside complete roots')
        tools[name]=str(path)
    gcc=[]
    for name in ('cflags','cxxflags','link_flags'):
        flags=value[name];need(type(flags) is list and all(type(x) is str and x and not any(c in x for c in '\0\n\r') for x in flags),
            'explicit native flags required')
        selected=[x for x in flags if x.startswith('--gcc-install-dir=')]
        need(len(selected)==1 and len(flags)==(1 if name=='cflags' else 2) and
             (name=='cflags' or flags.count('-stdlib=libstdc++')==1),'only explicit GCC15/stdlib flags admitted')
        gcc.append(selected[0]);builder.no_extra_llvm(flags,name)
    need(len(set(gcc))==1 and value['stdlib']=='libstdc++','paired native GCC15/libstdc++ selection required')
    gcc_path=Path(gcc[0].split('=',1)[1])
    need(gcc_path.is_absolute() and gcc_path.resolve(strict=True)==gcc_path and gcc_path.is_dir() and
         gcc_path.name=='15' and inside(gcc_path,roots),'actual selected whole GCC15 installation required')
    sysroot=value['sysroot']
    if sysroot is not None:
        need(type(sysroot) is str,'explicit sysroot path or null required');path=Path(sysroot)
        need(path.is_absolute() and path.resolve(strict=True)==path and path.is_dir() and inside(path,roots),'sysroot outside complete inputs')
    extra=Path(value['source_root'])
    need(extra.resolve(strict=True)==extra and extra.is_dir() and inside(extra,roots),'metadata source_root outside selected catalogs')
    environment=value['environment']
    need(type(environment) is dict and {'PATH','LANG','LC_ALL'}<=set(environment)<= {'PATH','LANG','LC_ALL','LD_LIBRARY_PATH'} and
         all(type(x) is str and x and not any(c in x for c in '\0\n\r') for x in environment.values()),'closed compiler-only environment required')
    for key in ('PATH','LD_LIBRARY_PATH'):
        for token in environment.get(key,'').split(':') if key in environment else []:
            path=Path(token);need(token and path.is_absolute() and path.resolve(strict=True)==path and path.is_dir() and inside(path,roots),
                'environment directories must have complete physical catalogs')
    closed(value['cgroup'],('memory_max','memory_swap_max','expected_leaf','expected_device','expected_inode','expected_boot_id','expected_cpus'),'cgroup')
    need(value['cgroup']['memory_max']==64*GIB and type(value['cgroup']['memory_max']) is int and
         value['cgroup']['memory_swap_max']==0 and type(value['cgroup']['memory_swap_max']) is int,
         'original64GiB/swap0 cgroup required')
    need(type(value['build_seconds']) is int and 600<=value['build_seconds']<=86400,'bounded configuration duration required')
    selections=value.get('profile_compile_flags',{key:[] for key in PROFILES});closed(selections,PROFILES,'profile header selection')
    headers={}
    for profile,tokens in selections.items():
        need(type(tokens) is list and len(tokens)<=1 and (profile!='none' or not tokens),'none has no custom ROS header selection')
        selected={}
        for token in tokens:
            need(type(token) is str,'literal custom header definition required')
            match=re.fullmatch(r'-D(UWVM_ROS_CUSTOM_TOPIC_MANIFEST)="([^"\0\r\n]+)"',token)
            need(match is not None and match[1] not in selected,'only unique literal custom-header macros allowed')
            path=Path(match[2]);need(path.is_absolute() and path.resolve(strict=True)==path and path.is_file() and
                inside(path,[source,*roots]),'custom header outside frozen source/whole roots')
            selected[match[1]]=path
        headers[profile]=list(selected.values())
    return roots,tools,selections,headers,gcc_path


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ('source','manifest','config','out'):parser.add_argument('--'+name,type=Path,required=True)
    for name in ('manifest-sha256','config-sha256'):parser.add_argument('--'+name,required=True)
    args=parser.parse_args()
    need(sys.platform=='linux' and hasattr(os,'pidfd_open'),'actual Linux pidfd controller required')
    need(all(getattr(args,k).is_absolute() for k in ('source','manifest','config','out')),'absolute CLI paths required')
    need(all(re.fullmatch('[0-9a-f]{64}',getattr(args,k)) for k in ('manifest_sha256','config_sha256')),'explicit actual control hashes required')
    source=args.source.resolve(strict=True);entries,mf_pin=document(args.manifest.resolve(strict=True))
    need(mf_pin['sha256']==args.manifest_sha256 and type(entries) is dict and entries,'actual complete frozen source MF required')
    own=source/'test/0073.ros-tests/support/source/ros_unitree/build_interpreter_workers.py'
    need(Path(__file__).absolute()==own,'execute the maintained producer from the selected complete source')
    own_raw,own_pin=read(own,512*1024)
    need(entries.get('test/0073.ros-tests/support/source/ros_unitree/build_interpreter_workers.py')=={key:own_pin[key] for key in ('bytes','sha256')},
         'maintained producer absent from complete current manifest')
    config,config_pin=document(args.config.resolve(strict=True));need(config_pin['sha256']==args.config_sha256,'actual configuration differs')
    # Prove the operator-selected current leaf using only stdlib before any
    # project module executes. The source-bound probe then checks all ancestors.
    cg=config['cgroup'];closed(cg,('memory_max','memory_swap_max','expected_leaf','expected_device','expected_inode','expected_boot_id','expected_cpus'),'current cgroup')
    need(type(cg['memory_max']) is int and cg['memory_max']==64*GIB and
         type(cg['memory_swap_max']) is int and cg['memory_swap_max']==0,'hard64GiB/zero-swap selection required')
    leaf=Path(cg['expected_leaf']);need(leaf.is_absolute() and leaf.resolve(strict=True)==leaf,'physical current cgroup leaf required')
    info=leaf.stat()
    need((info.st_dev,info.st_ino)==(cg['expected_device'],cg['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip()==cg['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines()==['0::'+str(leaf).removeprefix('/sys/fs/cgroup')] and
         (leaf/'memory.max').read_text().strip()==str(64*GIB) and
         (leaf/'memory.swap.max').read_text().strip()=='0' and
         (leaf/'cpuset.cpus.effective').read_text().strip()==cg['expected_cpus'],
         'actual current process must remain in the selected hard64GiB cgroup')
    probe,probe_pin=module(source,entries,'test/0073.ros-tests/support/source/ros_cgroup/probe.py','native_four_current_probe')
    common,common_pin=module(source,entries,'tools/ros_distro/common.py','native_four_common')
    common._CGROUP_PROBE=probe
    builder,builder_pin=module(source,entries,'test/0073.ros-tests/support/source/ros_debugger/build_runtime.py','native_four_binary_readers')
    cancelled=common.install_interrupts()
    roots,tools,selections,headers,gcc=configuration(config,source,builder)
    out=args.out;need(out.parent.resolve(strict=True)==out.parent and not out.exists() and not out.is_symlink() and
        all(not inside(out,[root]) and not inside(root,[out]) for root in [source,*roots]),'fresh independent output required')
    out.mkdir(mode=0o700);(out/'home').mkdir();(out/'tmp').mkdir()
    report={'format_version':1,'kind':'native-interpreter-four-profile-build','status':'RUNNING','source':str(source),
        'source_manifest_sha256':mf_pin['sha256'],'configuration':config_pin,'profiles':[], 'commands':[],
        'limits':LIMITS,'parallelism':1,'runtime_backend':'uwvm-interpreter','no_reuse':True,'reused_products':0,
        'actual_profile_build_order':list(BUILD_ORDER),'canonical_profile_result_order':list(PROFILES),
        'compiler_pass_workaround':COMPILER_WORKAROUND_PROVENANCE,
        'Worker_executed':False,'native_ROS_executed':False,'Guest_executed':False,'Guest_permissions_granted':False,
        'scope':'FRESH_NATIVE_INTERPRETER_BUILD_ONLY_NOT_RUNTIME_ROS_DEFAULT_DENIAL_OR_UNITREE_ACCEPTANCE',
        'source_root_scope':'UNCHANGED_SELECTED_CATALOG_INPUT_ONLY_NOT_LLVM_RUNTIME_PROVIDER'}
    before=None;source_before=None;before_roots=[];inputs={};products={};runner=None;finalizing=False;start=time.monotonic()
    active_seconds=min(config['build_seconds'],14400);next_output_scan=0.0
    alias_before={name:aliases(path) for name,path in tools.items()}
    def remember(path,expected=None):
        path=Path(path).resolve(strict=True);pin=common.opened(path)
        need(expected is None or pin==expected,'initial parsed/control input changed')
        need(pin['path'] not in inputs or inputs[pin['path']]==pin,'physical input changed')
        inputs[pin['path']]=pin;return pin
    def product(path,limit=512*MIB):
        pin=common.opened(Path(path),limit);need(pin['path'] not in products or products[pin['path']]==pin,'product changed')
        products[pin['path']]=pin;return pin
    def guard():
        nonlocal next_output_scan
        need(finalizing or not cancelled['cancelled'],'cooperative cancellation requested')
        need(time.monotonic()-start<active_seconds+(600 if finalizing else 0),'finite build/finalization deadline exceeded')
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024<=GIB,'coordinator metadata RSS exceeds1GiB')
        current=common.cgroup_state(config['cgroup'])
        if before is not None:common.cgroup_preserved(before,current)
        need(all(aliases(tools[name])==old for name,old in alias_before.items()),'selected invocation alias changed')
        # Exhausted active storage must not prevent child-first cleanup. A
        # separate fresh terminal storage check is required before success.
        if not finalizing:
            storage()
            if time.monotonic()>=next_output_scan:
                output_budget();next_output_scan=time.monotonic()+0.5
        return current
    def storage():
        for path in (out,*out.parents):
            state=os.statvfs(path);need(state.f_bavail*state.f_frsize>=8*GIB,'output/backing filesystem reserve below8GiB')
    def output_budget():
        need(sum(x.stat().st_size for x in out.rglob('*') if x.is_file() and not x.is_symlink())<=16*GIB,
             'finite owned output budget exceeded')
    def save():
        if runner is not None:report['commands']=runner.commands
        report['input_pins']=list(inputs.values());report['product_pins']=list(products.values())
        common.atomic_json(out/'report.json',report)
    def run(label,argv,cwd):
        try:
            row=runner.run(label,argv,cwd,environment)
            # A driver exit is insufficient if an already observed compiler
            # descendant remains live. Retire only proven terminal pidfds;
            # retained Popen/root and unproven ledgers remain for final audit.
            runner.discover();terminal=[]
            for key,item in runner.owned.items():
                need(common.pidfd_terminal(item['fd']),'owned compiler descendant remains after command')
                terminal.append({'birth':item['identity'],'after':common.identity(key[0]),'pidfd_terminal':True})
            for key,first in runner.unproven.items():
                current=common.identity(key[0])
                need(common.birth(current)!=key or current['state'] in ('Z','X'),
                     'unproven live compiler birth remains before the next command')
            for key,item in list(runner.owned.items()):
                os.close(item['fd']);runner.owned.pop(key)
            row['owned_terminal_barrier']={'observations':terminal,'descriptor_retired':True,
                'retained_Popen_and_roots_for_final_audit':True}
            return row
        finally:
            if runner.commands:
                row=runner.commands[-1];row['effective_limits']=dict(LIMITS)
                if 'log' in row:need(product(row['log']['path'],LIMITS['log_bytes'])==row['log'],'original command log changed')
                common.atomic_json(out/'commands.json',runner.commands)
            save()
    environment={'HOME':str(out/'home'),'TMPDIR':str(out/'tmp'),'PYTHONDONTWRITEBYTECODE':'1','GOMAXPROCS':'1',
        'MAKEFLAGS':'-j1','CMAKE_BUILD_PARALLEL_LEVEL':'1',**config['environment']}
    try:
        before=guard();report['cgroup_before']=before
        need(int((Path(config['cgroup']['expected_leaf'])/'memory.current').read_text())+LIMITS['rss_bytes']+2*GIB<60*GIB,
             'actual cgroup must reserve compiler RSS and parent headroom below60GiB')
        need(all(row['events'].get(key,0)==0 for row in before['ancestors'] for key in ('oom','oom_kill','oom_group_kill')),'initial OOM counters nonzero')
        source_before=common.source_state(source,args.manifest);need(source_before['manifest']==mf_pin,'initial MF binding changed')
        report['source_before']=source_before
        for pin in (mf_pin,config_pin,own_pin,probe_pin,common_pin,builder_pin):remember(pin['path'],pin)
        remember(Path(__file__).resolve())
        for name,path in tools.items():remember(path);builder.elf(common,Path(path).resolve(strict=True),config['target'])
        report['tools']={name:remember(path) for name,path in tools.items()};report['selected_aliases_before']=alias_before
        report['gcc_install_dir']=str(gcc)
        for index,root in enumerate(roots):
            guard();row=common.catalog(root,out/('inputs-before-%02d.jsonl'%index),roots);before_roots.append(row);product(row['catalog']['path'])
            report['input_roots_before']=before_roots;save()
        runner=common.SerialRunner(out,LIMITS,guard)
        version_row=run('compiler-version',[tools['cxx'],'--driver-mode=g++','--version'],out)
        version_raw,_=common.body_pin(Path(version_row['log']['path']),LIMITS['log_bytes'])
        need(re.search(rb'\bclang version 23(?:\.|\s)',version_raw) is not None,'actual selected compiler is not Clang23')
        run('linker-version',[tools['linker'],'--version'],out)
        base=['--driver-mode=g++','--target='+config['target']['triple'],'-std=c++26','-O1','-g0','-UNDEBUG','-pthread',
            '-Wno-undefined-inline','-fconstexpr-steps=8388608','-DUWVM=2','-DUWVM_USE_UWVM_INT','-DUWVM_DISABLE_JIT',
            '-DUWVM_USE_THREAD_LOCAL','-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64','-DUWVM_ENABLE_WASIP1_SOCKET',
            '-DUWVM_VERSION_X=2','-DUWVM_VERSION_Y=0','-DUWVM_VERSION_Z=3','-DUWVM_VERSION_S=0']
        if config['sysroot']:base.append('--sysroot='+config['sysroot'])
        for relative in ('src','third-parties/fast_io/include','third-parties/bizwen/include','third-parties/boost_unordered/include'):
            base+=['-I',str(source/relative)]
        report['compile_recipe_authority']='Same interpreter base as frozen A64 Worker, with native target, -UNDEBUG and full MD/MT; no test macro or injected stop'
        for profile in BUILD_ORDER:
            destination=out/profile;destination.mkdir(mode=0o700)
            row={'profile':profile,'status':'RUNNING','objects':[],'commands':[],'full_dependencies_verified':False}
            report['profiles'].append(row);save()
            flags=[*base,*config['cxxflags'],*selections[profile]]
            for version in ((1,) if profile=='ros1' else (2,) if profile=='ros2' else (1,2) if profile=='dual' else ()):
                flags.append('-DUWVM_IMPORT_ROS'+str(version))
            selected_headers=[remember(path) for path in headers[profile]];observed=set()
            for unit,relative in UNITS:
                obj=destination/(unit+'.o');dep=destination/(unit+'.d')
                need(not obj.exists() and not dep.exists(),'fresh object/MD paths required')
                # The observed Clang failure was in main's MachineDCE pass.
                # Preserve runtime/host_api O1 code generation and all link
                # flags; this fixed pair is applied only to the real main TU.
                pass_workaround=list(MAIN_PASS_WORKAROUND) if unit=='main' else []
                command=[tools['cxx'],*flags,*pass_workaround,'-MD','-MF',str(dep),'-MT',str(obj),'-c',str(source/relative),'-o',str(obj)]
                job=run(profile+'-'+unit,command,destination);row['commands'].append(len(runner.commands)-1)
                dep_pin,deps=builder.dependency_inputs(common,dep,obj,source,roots)
                need(str(source/relative) in {p['path'] for p in deps},'full MD omitted the actual translation unit')
                for pin in deps:remember(pin['path'],pin);observed.add(pin['path'])
                object_pin=builder.elf(common,obj,config['target'],(1,));product(obj);product(dep)
                row['objects'].append({'unit':unit,'source':remember(source/relative),'object':object_pin,
                    'dependency_file':dep_pin,'dependencies':deps,'command_index':len(runner.commands)-1,'cwd':str(destination),
                    'compiler_pass_workaround_argv':pass_workaround})
                save()
            need({pin['path'] for pin in selected_headers}<=observed,'selected custom headers absent from actual full MD')
            by_unit={item['unit']:item['object'] for item in row['objects']}
            binary=destination/'uwvm2-ros';link_objects=[by_unit[unit] for unit in ('main','runtime','host_api')]
            objects=[item['path'] for item in link_objects]
            command=[tools['cxx'],'--driver-mode=g++','--target='+config['target']['triple'],*config['link_flags'],
                '-fuse-ld='+tools['linker'],'-pthread','-Wl,--threads=1','-Wl,--trace','-Wl,--no-undefined']
            if config['sysroot']:command.append('--sysroot='+config['sysroot'])
            command += [*objects,'-ldl','-o',str(binary)]
            job=run(profile+'-link',command,destination);row['commands'].append(len(runner.commands)-1)
            raw,_=common.body_pin(Path(job['log']['path']),LIMITS['log_bytes']);linked={};archive_members={}
            for line in raw.decode('utf-8','strict').splitlines():
                if not line.strip():continue
                operand=line.split('(',1)[0];path=Path(operand)
                if not path.is_absolute():path=destination/path
                need(path.exists(),'link trace contains an unqualified/nonfile record: '+line)
                path=path.resolve(strict=True)
                need(path.is_file() and inside(path,[destination,*roots]),'actual link input outside complete closures')
                need(not path.name.lower().startswith(('libllvm','llvm')),'LLVM Runtime provider appeared in interpreter link')
                if path.suffix=='.a':
                    _archive_pin,count=builder.archive(common,path,config['target']);archive_members[str(path)]=count
                pin=product(path) if inside(path,[destination]) else remember(path);linked[pin['path']]=pin
            need(set(objects)<=set(linked),'actual link trace omitted an owned object')
            need(any(Path(path).name.startswith('libstdc++') for path in linked),'actual selected standard library link input missing')
            binary_pin=builder.elf(common,binary,config['target']);product(binary)
            need(os.access(binary,os.X_OK),'fresh native Worker is not executable')
            row.update(status='BUILT_NOT_EXECUTED',binary=binary_pin,
                elf={'machine':62,'bits':64,'endian':'little'},link_inputs=list(linked.values()),
                native_archive_members=archive_members,
                link_command_index=len(runner.commands)-1,custom_header_inputs=selected_headers,
                link_objects=link_objects,
                custom_header_scope='ACTUAL_OPENED_HEADER_BYTES_AND_MD_ONLY_NO_NATIVE_PREPARATION_SOURCE_PROMOTION',
                full_dependencies_verified=True,no_reuse=True,reused_products=0)
            save()
        # Preserve chronological commands/object indices, but expose complete
        # results in the same canonical profile order as every consumer.
        report['profiles'].sort(key=lambda item:PROFILES.index(item['profile']))
        need(tuple(row['profile'] for row in report['profiles'])==PROFILES and
             all(row['status']=='BUILT_NOT_EXECUTED' for row in report['profiles']) and len(runner.commands)==18,
             'exact four independent three-TU/link profiles plus two tool-version commands required')
        report['build_complete']=True
    except BaseException as error:
        report.update(status='FAIL',error=repr(error),traceback=traceback.format_exc())
    finally:
        finalizing=True;errors=[]
        def finish(label,action):
            try:action()
            except BaseException as error:errors.append({'stage':label,'error':repr(error)});report['status']='FAIL'
        def cleanup():
            report['cleanup']=runner.close() if runner else {'status':'PASS','remaining':[],'scope':'NO_ACTOR_STARTED'}
            need(report['cleanup']['status']=='PASS' and not report['cleanup']['remaining'],'owned compiler cleanup incomplete')
        finish('owned-cleanup',cleanup)
        if runner:
            for job in runner.commands:
                if 'log' in job:finish('retain-command-log',lambda job=job:product(job['log']['path'],LIMITS['log_bytes']))
            if (out/'commands.json').exists():finish('retain-command-ledger',lambda:product(out/'commands.json',128*MIB))
        def preserve():
            need(source_before is not None and len(before_roots)==len(roots),'complete initial input proof unavailable')
            report['source_after']=common.source_state(source,args.manifest)
            need(report['source_after']==source_before,'complete frozen source changed')
            after=[]
            for index,root in enumerate(roots):
                guard();item=common.catalog(root,out/('inputs-after-%02d.jsonl'%index),roots);after.append(item);product(item['catalog']['path'])
                need({k:v for k,v in item.items() if k!='catalog'}=={k:v for k,v in before_roots[index].items() if k!='catalog'},
                    'whole selected input root changed')
            report['input_roots_after']=after
            need(all(common.opened(Path(pin['path']))==pin for pin in [*inputs.values(),*products.values()]),'physical input/product changed')
            need(alias_before=={name:aliases(path) for name,path in tools.items()},'selected invocation aliases changed')
            report['selected_aliases_after']=alias_before;report['cgroup_after']=guard()
            report.update(source_preserved=True,inputs_preserved=True,products_preserved=True)
        finish('source-whole-inputs-products-preservation',preserve)
        finish('fresh-terminal-output-budget',output_budget)
        finish('fresh-terminal-backing-reserve',storage)
        report['finalization_errors']=errors
        if report.get('build_complete') and report['status']=='RUNNING' and not errors:
            report['status']='PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED'
        save()
    return 0 if report['status']=='PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED' else 1


if __name__=='__main__':raise SystemExit(main())
