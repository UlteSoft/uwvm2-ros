#!/usr/bin/env python3
"""Qualify canonical Rust SDK sources through actual installation and xmake.

Use the original finite owner. No source/resolver overlay, generated-record
substitute, host-language executable, implicit capability or old result is used.
The producer's build/validation-pending receipts remain unchanged; this driver
records its actual subsequent validators and Guest executions separately.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import os
import re
import shutil
import subprocess
import sys
import tarfile


COMMANDS=('raw','typed','coverage','unified_events','node','resources','servers',
          'scheduling','action_parameters','tf_graph_clock','events_tf_lifecycle',
          'portable_telemetry','owned_control_client','owned_control_server')
CONTROLS=('owned_control_client','owned_control_server')
DEFAULT={'target':'wasm32-wasip1','address_bits':32,'wasi':'wasip1',
         'exceptions':False,'threads':False,'startup':'command','core_baseline':'3.0'}


def digest(path):
    with path.open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def command_profile(data,reader):
    """Observe actual command startup and own unshared memory32 after validation."""
    source=reader(data)
    if bytes(source.take(8))!=b'\0asm\1\0\0\0':raise ValueError('Actual Wasm v1 command required')
    types=[];functions=[];memories=[];exports={}
    while source.position<len(source.data):
        number=source.byte();body=reader(source.take(source.uint()))
        if number==1:types=body.types();body.end()
        elif number==2:
            for _ in range(body.count()):
                body.name();body.name()
                if body.byte()!=0:raise ValueError('Ordinary memory-zero Rust command cannot import memory/table/global/tag')
                functions.append(body.uint())
            body.end()
        elif number==3:
            functions.extend(body.uint() for _ in range(body.count()));body.end()
        elif number==5:
            for _ in range(body.count()):
                flags=body.uint()
                if flags not in (0,1):raise ValueError('Actual unshared memory32 required; no threads/memory64 downgrade')
                minimum=body.uint();maximum=body.uint() if flags else None
                if not 0<minimum<=65536 or maximum is not None and not minimum<=maximum<=65536:
                    raise ValueError('Actual Rust command memory range differs from memory32')
                memories.append({'minimum_pages':minimum,'maximum_pages':maximum,'shared':False,'address_bits':32})
            body.end()
        elif number==7:
            for _ in range(body.count()):
                name=body.name()
                if name in exports:raise ValueError('Duplicate actual command export')
                exports[name]=(body.byte(),body.uint())
            body.end()
        elif number==8:raise ValueError('Command must use explicit _start, without an implicit Wasm start section')
        elif number==13:raise ValueError('EH-off command cannot declare exception tags')
    if len(memories)!=1 or exports.get('memory')!=(2,0) or '_start' not in exports:
        raise ValueError('Actual own memory0 and explicit command export required')
    kind,index=exports['_start']
    if kind!=0 or index>=len(functions) or functions[index]>=len(types) or types[functions[index]]!=([],[]):
        raise ValueError('Actual _start must be a zero-argument/zero-result function')
    return {'startup':'command','entry':'_start','entry_parameters':[],'entry_results':[],
        'implicit_start':False,'memories':memories,'exception_tag_section':False,
        'scope':'Bounded actual startup/memory/type observation; independent full validation is separately required'}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--sha256',required=True)
    parser.add_argument('--scope',choices=('reactor','control','all-recipes'),required=True)
    args=parser.parse_args()
    work=Path.cwd();source=work/'source';source.mkdir()
    if digest(args.source)!=args.sha256:raise ValueError('Current public source capsule changed')
    with tarfile.open(args.source) as archive:
        members=archive.getmembers()
        if sum(row.size for row in members)>128*1024**2 or any(not row.isfile()
                or Path(row.name).is_absolute() or '..' in Path(row.name).parts for row in members):
            raise ValueError('Finite ordinary complete public source capsule required')
        archive.extractall(source,filter='data')
    sdk_source=source/'sdk'
    if (sdk_source/'origin').exists():raise ValueError('Canonical source SDK cannot depend on an origin tree')
    before={str(path.relative_to(source)):digest(path) for path in source.rglob('*') if path.is_file()}
    rows=[];recipes=[]
    report={'kind':'task03-canonical-installed-Rust-xmake-v1','status':'RUNNING',
        'scope':args.scope,'source_capsule_sha256':args.sha256,'source_pins':before,
        'executed_driver':{'path':str(Path(__file__).resolve()),'sha256':digest(Path(__file__))},
        'uses_private_shared_candidate':False,'source_regeneration_before_prepare':False,
        'global_publication':False,'commands':rows,'recipes':recipes,
        'public_target_families':['wasm32-wasip1','wasm64-wasip1'],
        'default_authority':[],'actual_Guest_passes':0,'native_ROS_runs':0,
        'Gateway_runs':0,'vendor_credit':0,'all_public_API_documentation_accepted':False}
    def save():
        (work/'installed-rust.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,layer,command,*,cwd=work,env=None,expected=0,nonzero=False,marker=None,timeout=300):
        log=work/(name+'.log');row={'name':name,'layer':layer,'argv':[str(value) for value in command],
                                  'cwd':str(cwd),'log':log.name}
        rows.append(row);save()
        with log.open('wb') as stream:
            result=subprocess.run(row['argv'],cwd=cwd,env=env,stdout=stream,
                                  stderr=subprocess.STDOUT,timeout=timeout)
        text=log.read_text(errors='replace')
        passed=result.returncode!=0 if nonzero else result.returncode==expected
        if marker is not None:passed=passed and marker in text
        row.update(exit=result.returncode,log_sha256=digest(log),status='PASS' if passed else 'FAIL')
        save()
        if not passed:raise ValueError('Retained actual '+name+' failure: '+str(log))
        return text
    try:
        for path in source.rglob('*.py'):compile(path.read_bytes(),str(path),'exec',dont_inherit=True)
        agents=Path('/home/macromodel/Documents/uwvm-test-261018/agents')
        rust=agents/'03/20261021-toolchains-r1/work/toolchains/rust'
        wasi=agents/'10/staged-wasi-r1/staged/wasi-sdk'
        rustc=rust/'bin/rustc';cargo=rust/'bin/cargo';linker=wasi/'bin/wasm-ld'
        validator=Path('/usr/bin/wasm-validate');wat2wasm=Path('/usr/bin/wat2wasm')
        xmake=Path('/home/macromodel/.local/bin/xmake')
        runtime=agents/'10/runtime-denial-r2/work/uwvm-dual'
        if digest(xmake)!='d24ec7daa11ab5007f33221ba0a883f9c7479afae2a47370773f209047152a1e':
            raise ValueError('Actual xmake changed')
        if digest(runtime)!='0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556':
            raise ValueError('Actual retained Runtime changed; select and qualify its successor explicitly')
        if not run('rustc-version','actual_compiler_version',[rustc,'--version','--verbose']).startswith('rustc 1.85.0 '):
            raise ValueError('Exact matching Rust1.85 required')
        if not run('cargo-version','actual_build_tool_version',[cargo,'--version']).startswith('cargo 1.85.0 '):
            raise ValueError('Exact matching Cargo1.85 required')
        if Path(run('rust-sysroot','actual_compiler_sysroot_query',[rustc,'--print','sysroot']).strip()).resolve(strict=True)!=rust:
            raise ValueError('Actual compiler sysroot fell back outside the selected Rust root')
        toolroot=Path('/');tool_pins={}
        def pin(path):
            path=Path(path)
            if not path.is_file():raise ValueError('Actual selected tool/runtime file absent: '+str(path))
            name=path.relative_to(toolroot).as_posix()
            value={'path':name,'bytes':path.stat().st_size,'sha256':digest(path)}
            tool_pins[name]=value;return value
        for target in ('wasm32-wasip1','wasm32-unknown-unknown'):
            directory=rust/'lib/rustlib'/target/'lib'
            actual=Path(run('target-'+target,'actual_target_library_query',
                [rustc,'--target='+target,'--print','target-libdir']).strip()).resolve(strict=True)
            if actual!=directory:raise ValueError('Actual target library directory mismatch')
            for path in sorted(directory.rglob('*')):
                if path.is_file():pin(path)
        for directory in (rust/'lib',wasi/'lib'):
            for path in sorted(directory.glob('*.so*')):
                if path.is_file():pin(path)
        libc=wasi/'share/wasi-sysroot/lib/wasm32-wasip1/libc.a';pin(libc)
        programs={'rustc':rustc,'cargo':cargo,'linker':linker,'validator':validator,'wat2wasm':wat2wasm}
        host_libraries=set()
        for name,path in {**programs,'runtime':runtime}.items():
            pin(path)
            text=run('loader-'+name,'actual_native_loader_dependencies',['/usr/bin/ldd',path])
            if 'not found' in text:raise ValueError('Actual selected tool shared-library dependency missing')
            for spelling in re.findall(r'(/[^\s()]+)\s+\(0x',text):
                dependency=Path(spelling).resolve(strict=True);pin(dependency);host_libraries.add(str(dependency))
        if len(tool_pins)>1024:raise ValueError('Actual Rust executable/target closure exceeds the shared finite provider limit')
        captured_tools=dict(tool_pins)
        prepared=work/'prepared-sdk'
        # A bounded private installation input copy of the one current source
        # SDK, with no producer, generated maintenance tree or resolver overlay.
        shutil.copytree(sdk_source,prepared)
        report['canonical_source_copy_only']=True
        expected_files=('include/platform/ros/rs/Cargo.toml',
            'include/platform/ros/rs/owned_service/Cargo.toml',
            'example/platform/ros/rs/robotics_modular/vision.rs',
            'example/platform/ros/rs/owned_rmw_service/control_client.rs',
            'example/platform/ros/rs/owned_rmw_service/control_server.rs',
            'example/platform/ros/rs/owned_rmw_service/control_bindings.rs',
            'include/platform/ros/rs/owned_service/src/records.rs',
            'share/platform/ros/rs/owned_control_records.json',
            'tools/rmw_service_contract.py')
        if args.scope!='reactor':expected_files+=('share/abi/rmw-service-v1.json',)
        for name in expected_files:
            if not (prepared/name).is_file():raise ValueError('Genuine installed Rust source dependency absent: '+name)
        files=[{'path':str(path.relative_to(prepared)),'bytes':path.stat().st_size,'sha256':digest(path)}
            for directory in (prepared/'include',prepared/'lib') for path in sorted(directory.rglob('*')) if path.is_file()]
        def provider(reactor):
            primary=pin(rustc);secondary=pin(linker);build=pin(cargo)
            excluded={primary['path'],secondary['path'],build['path']}
            return {'toolchain_id':'actual-rust185-wasi33-native_languages','version':'1.85.0',
                'compiler':primary,'linker':secondary,'build_tools':{'cargo':build},
                'tool_dependencies':[value for name,value in sorted(tool_pins.items()) if name not in excluded],
                'compiler_argv':[] if reactor else ['-C','codegen-units=1'],
                'linker_argv':[] if reactor else ['-C','linker='+str(linker),'-C','link-arg=--threads=1',
                    '-C','link-arg='+str(libc)],'library_modes':['core'] if reactor else ['core','alloc','std'],
                'source_ready':True,'compiled':False,'runtime_tested':False,
                'host_runtime_dependency_names':sorted(host_libraries)}
        profiles=[]
        for reactor in (False,True):
            contract={**DEFAULT,'startup':'reactor' if reactor else 'command'}
            abi={'address_bits':32,'wasi_pointer_abi':'wasip1-memory32','exceptions':False,'threads':False,
                'exception_encoding':'none','thread_abi':'none','shared_memory':False,
                'required_files':['include/platform/ros/rs/Cargo.toml']}
            if reactor:
                abi['internal_scalar_reactors']={'rust-vision-core-v1':{
                    'format':'uwvm-internal-scalar-reactor-v1','compiler_target':'wasm32-unknown-unknown',
                    'artifact_wasi_import_abi':'none','artifact_address_bits':32,'implicit_start':False,
                    'initializer':{'name':'vision_initialize','params':[],'results':['i32']},
                    'maximum_memory_pages':2,'sysroot':str(rust.relative_to(toolroot)),
                    'validation_tools':{'wat2wasm':str(wat2wasm.relative_to(toolroot)),
                                        'wasm_validate':str(validator.relative_to(toolroot))}}}
            profiles.append({'id':'actual-rust185-'+contract['startup'],'profile':contract,
                'toolchain_id':'actual-rust185-wasi33-native_languages','sysroot':'.','resource_dir':None,
                'library_abi':abi,'files':files,'languages':{'rust':provider(reactor)},'runtime_requirements':[]})
        manifest=prepared/'share/native_languages-rust-profile.json'
        manifest.write_text(json.dumps({'format':'uwvm-sdk-wasm-profiles-v1','defaults':DEFAULT,
            'profiles':profiles,'default_authority':[]},indent=2)+'\n')
        report['selected_profile_manifest_sha256']=digest(manifest)
        installed=work/'installed-sdk'
        run('cmake-configure','actual_CMake_configuration',['/usr/bin/cmake','-S',prepared,
            '-B',work/'cmake-build','-DCMAKE_INSTALL_PREFIX='+str(installed),'-DROSWASM_INSTALL_EXAMPLES=ON'])
        run('cmake-install','actual_CMake_installation',['/usr/bin/cmake','--install',work/'cmake-build'])
        prepared_pins={str(path.relative_to(prepared)):digest(path) for path in prepared.rglob('*') if path.is_file()}
        installed_pins={str(path.relative_to(installed)):digest(path) for path in installed.rglob('*') if path.is_file()}
        extras=set(installed_pins)-set(prepared_pins)
        allowed={'lib/cmake/roswasm/roswasmTargets.cmake','lib/cmake/roswasm/roswasmTargets-noconfig.cmake',
                 'lib/cmake/roswasm/roswasmConfig.cmake'}
        if any(installed_pins.get(name)!=value for name,value in prepared_pins.items()) or not extras<=allowed:
            raise ValueError('Actual installation omitted/changed canonical sources or added an undeclared product')
        report['prepared_install_comparison']={'status':'PASS','files':len(prepared_pins),
            'pins':prepared_pins,'actual_CMake_generated_exports':sorted(extras)}
        shutil.rmtree(prepared)
        relocated=work/'relocated-sdk';installed.rename(relocated)
        if any((relocated/name).exists() for name in ('origin','test','tests')) or any(path.is_symlink() for path in relocated.rglob('*')):
            raise ValueError('Actual SDK has a maintenance/test/linked-file dependency')
        sdk_before={str(path.relative_to(relocated)):digest(path) for path in relocated.rglob('*') if path.is_file()}
        inspector_spec=importlib.util.spec_from_file_location('_actual_installed_Rust_imports',
            relocated/'tools/check_wasm_imports.py')
        inspector=importlib.util.module_from_spec(inspector_spec);inspector_spec.loader.exec_module(inspector)
        environment=dict(os.environ);environment.update(RUSTC=str(rustc),RUSTFLAGS='-Cpanic=abort',
            CARGO_BUILD_JOBS='1',CARGO_NET_OFFLINE='true',RUSTC_WRAPPER='',RUSTC_WORKSPACE_WRAPPER='')
        environment.pop('CARGO_ENCODED_RUSTFLAGS',None)
        run('current-crate-front-end','actual_installed_wasm32_Rust_crate_semantics',
            [cargo,'check','--frozen','--offline','--jobs','1','--manifest-path',
             relocated/'include/platform/ros/rs/Cargo.toml','--target=wasm32-wasip1',
             '--target-dir',work/'crate-check','--no-default-features','--features','ros2,alloc'],env=environment)
        report['actual_current_Rust_crate_frontend_runs']=1
        project=work/'xmake-project';project.mkdir()
        def xmake_command(action,*arguments):
            return [xmake,action,'--project='+str(project),'--file='+str(relocated/'xmake.lua'),*arguments]
        selected=(['vision_library'] if args.scope=='reactor' else list(CONTROLS)
                  if args.scope=='control' else [*COMMANDS,'vision_library'])
        for identifier in selected:
            reactor=identifier=='vision_library';output=work/('example-'+identifier)
            recipe={'id':identifier,'state':'BUILDING','actual_Guest_state':'NOT_EXECUTED'}
            recipes.append(recipe);save()
            run(identifier+'-configure','actual_installed_xmake_configuration',xmake_command('f','--yes',
                '--ros-sdk-platforms=ros','--ros-sdk-languages=rs','--sdk-example-platform=ros',
                '--sdk-example-language=rs','--sdk-example='+identifier,'--sdk-ros-backend=ros2',
                '--sdk-rust-version=1.85.0','--sdk-rust-edition=2021','--sdk-rust-panic=abort',
                '--sdk-rust-library='+('core' if reactor else 'std'),'--sdk-target=wasm32-wasip1',
                '--sdk-startup='+('reactor' if reactor else 'command'),'--sdk-exceptions=n','--sdk-threads=n',
                '--sdk-offline=y','--sdk-toolchain=/','--sdk-python=/usr/bin/python3',
                '--sdk-profile-manifest='+str(relocated/'share/native_languages-rust-profile.json')),cwd=project)
            run(identifier+'-build','actual_installed_xmake_build',xmake_command('sdk-example-build',
                '--output='+str(output)),cwd=project,timeout=600)
            outer=json.loads((output/'build-receipt.json').read_bytes())
            inner=json.loads((output/'native_languages/work/build-receipt.json').read_bytes())
            expected='WASM_BUILT_AND_VALIDATED_GUEST_INITIALIZATION_NOT_RUN' if reactor else 'WASM_BUILT_UNQUALIFIED_UNTIL_VALIDATOR_AND_GUEST'
            if outer['status']!='BUILT_NOT_EXECUTED' or inner['status']!=expected:
                raise ValueError('Actual build receipt must preserve its exact not-executed qualification state')
            recipe.update(state='BUILT_NOT_EXECUTED',outer_receipt_sha256=digest(output/'build-receipt.json'),
                inner_receipt_sha256=digest(output/'native_languages/work/build-receipt.json'))
            for number,artifact in enumerate(outer['artifacts']):
                path=Path(artifact['path']).resolve(strict=True)
                if not path.is_relative_to(output) or digest(path)!=artifact['sha256']:
                    raise ValueError('Actual output differs from its closed artifact receipt')
                run(identifier+'-validate-'+str(number),'independent_actual_full_Wasm_validator',
                    [validator,*(['--enable-all'] if reactor else []),path])
                if not reactor:recipe['actual_command_profile']=command_profile(path.read_bytes(),inspector.Reader)
            if not reactor:
                if len(outer['artifacts'])!=1:raise ValueError('One actual command artifact required')
                artifact=Path(outer['artifacts'][0]['path'])
                if identifier=='scheduling':
                    imports=inspector.inspect(artifact.read_bytes())
                    if any(row['module']!='wasi_snapshot_preview1' for row in imports):
                        raise ValueError('Pure caller-time scheduling has an unexpected non-WASI provider import')
                    recipe['actual_pure_imports']=imports
                    run(identifier+'-Guest','actual_installed_xmake_Guest_empty_authority',
                        xmake_command('sdk-example-run','--receipt='+str(output/'build-receipt.json'),
                            '--runtime='+str(runtime)),cwd=project,marker='deferred result 42',timeout=90)
                    recipe['actual_Guest_state']='PASS_EXPLICIT_CURRENT_GUEST';report['actual_Guest_passes']+=1
                    recipe['state']='VALIDATED_WITH_SEPARATE_EXECUTION_STATE';save();continue
                command=[sys.executable,'-I','-S','-B',relocated/'tools/check_wasm_imports.py',artifact,
                    '--backend','2','--allow-module','wasi_snapshot_preview1','--report',work/(identifier+'-imports.json')]
                if identifier in CONTROLS:command+=['--ros2-rmw-service']
                if identifier=='coverage':command+=['--require-all']
                run(identifier+'-imports','actual_installed_binary_import_contract',command)
                imports=json.loads((work/(identifier+'-imports.json')).read_bytes())
                recipe['actual_imports']=imports
                if identifier in CONTROLS:
                    if imports.get('optional_functions_checked',0)==0:raise ValueError('Actual selected raw12 imports required')
                    proof=inner['original_control_closure']
                    if len(proof['record_names'])!=32 or len(proof['services'])!=7:
                        raise ValueError('Complete original control records and seven Service roots required')
                    recipe['original_control_closure']=proof
                    run(identifier+'-policy-refusal','actual_dispatcher_explicit_policy_refusal',
                        xmake_command('sdk-example-run','--receipt='+str(output/'build-receipt.json'),
                            '--runtime='+str(runtime)),cwd=project,nonzero=True,marker='explicit trusted Host policy')
                    arguments=(['ungranted-control-call','ungranted-control-observer','-',
                        'type_description_interfaces/srv/GetTypeDescription','','1']
                        if identifier=='owned_control_client' else ['ungranted-control-advertise','ungranted-control-respond','1'])
                    # The client type-hash argument is deliberately empty and
                    # cannot be used: the real first ungranted owner must refuse
                    # before a request is encoded/submitted. No Native hash is invented.
                    run(identifier+'-Guest-default-deny','actual_current_raw12_Guest_default_denial',
                        [runtime,'--run',artifact,*arguments],expected=1,
                        marker='owned '+('client' if identifier.endswith('client') else 'server')+' status 13',timeout=90)
                    recipe['actual_Guest_state']='PASS_ACTUAL_DEFAULT_DENIAL_13_NO_POSITIVE_NATIVE_SERVICE_CREDIT'
                    recipe['Native_type_hash_supplied']=False;report['actual_Guest_passes']+=1
            if reactor or identifier=='portable_telemetry':
                run(identifier+'-Guest','actual_installed_xmake_Guest_empty_authority',
                    xmake_command('sdk-example-run','--receipt='+str(output/'build-receipt.json'),
                        '--runtime='+str(runtime)),cwd=project,
                    marker='PASS_ROS_DEFAULT_DENIED' if not reactor else None,timeout=90)
                recipe['actual_Guest_state']='PASS_EXPLICIT_CURRENT_GUEST';report['actual_Guest_passes']+=1
            recipe['state']='VALIDATED_WITH_SEPARATE_EXECUTION_STATE';save()
        if any(digest(Path('/'+name))!=pin['sha256'] or Path('/'+name).stat().st_size!=pin['bytes']
                for name,pin in captured_tools.items()):raise ValueError('Actual selected tool/target input changed')
        if any(digest(source/name)!=pin for name,pin in before.items()):raise ValueError('Actual captured public source changed')
        sdk_after={str(path.relative_to(relocated)):digest(path) for path in relocated.rglob('*') if path.is_file()}
        if sdk_after!=sdk_before:raise ValueError('Actual relocated SDK changed during public builds or Guest work')
        report.update(status='PASS_CURRENT_PUBLIC_INSTALLED_RUST_'+args.scope.upper().replace('-','_'),
            SDK_unchanged=True,selected_tools_unchanged=True,public_sources_unchanged=True,
            SDK_product_pins=sdk_before,tool_pins=captured_tools,
            positive_native_control_interop_qualified=False,all41_recipe_closures_accepted=False)
        save();return 0
    except Exception:
        report['status']='FAILED_RETAINED';save();raise


if __name__=='__main__':raise SystemExit(main())
