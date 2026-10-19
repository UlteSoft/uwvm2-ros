#!/usr/bin/env python3
"""Compile six real standalone Go2 C/C++/embedded-JS command products.

Root supplies actual tool/archive labels, the complete GO generated directory,
a completed current Typed14 prerequisite and explicit restart mappings. This
producer does not discover tools, fetch packages, execute a Guest or initialize
ROS/MuJoCo. Each main and its entry/engine live in ONE final Wasm. Builds are
serial and owned; full-MD, actual linker traces, validation and binary tables
are retained. A successful build remains separate from the physics runner.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys
import types

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
MIB = 1024**2
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
PREFIX = 'sdk/robots/unitree/examples/'
TRAP = '-D_LIBCPP_VERBOSE_ABORT(...)=__builtin_trap()'
CG_LEAF = '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'


def require(value, message):
    if not value:
        raise ValueError(message)


def bounded(path,maximum):
    """Fully hash-bound bootstrap reads are bounded before body allocation."""
    path=Path(path);require(path.is_absolute() and path.resolve(strict=True)==path,'physical bootstrap file required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
    try:
        before=os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and 0<before.st_size<=maximum,'bounded ordinary bootstrap bytes required')
        chunks=[];size=0
        while True:
            block=os.read(fd,min(65536,maximum-size+1))
            if not block:break
            size+=len(block);require(size<=maximum,'bootstrap bytes grew');chunks.append(block)
        after=os.fstat(fd);named=path.stat(follow_symlinks=False)
        require(size==before.st_size and all(getattr(before,key)==getattr(after,key)==getattr(named,key)
            for key in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),'opened bootstrap file changed')
        return b''.join(chunks)
    finally:os.close(fd)


def bootstrap(args):
    """Hash the first frozen loader before executing any project helper."""
    source = args.source.resolve(strict=True)
    require(source == ROOT and sys.platform == 'linux' and os.geteuid() == 0,
            'Root must invoke the real frozen Linux entry in its admitted cgroup')
    # The maintained first loader has no import-time actors. Its own bounded
    # read is bootstrapped from stable ordinary bytes, then its frozen-module
    # function binds every later helper to this exact complete manifest.
    manifest = args.manifest.resolve(strict=True)
    raw=bounded(manifest,16*MIB)
    require(hashlib.sha256(raw).hexdigest() == args.manifest_sha256, 'actual complete manifest SHA differs')
    def pairs(items):
        result={}
        for key,value in items:
            require(key not in result,'duplicate manifest member');result[key]=value
        return result
    frozen = json.loads(raw,object_pairs_hook=pairs)
    for relative in ('tests/ros_unitree/prepare_direct_guests.py', 'tests/ros_javascript_guest/run.py'):
        path = source / relative;body = bounded(path,MIB)
        require(len(body) <= MIB and frozen.get(relative) ==
                {'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()}, 'first source absent from complete freeze')
        if relative.endswith('ros_javascript_guest/run.py'):
            api = types.ModuleType('unitree_direct_build_frozen_loader');api.__file__ = str(path)
            sys.modules[api.__name__] = api;exec(compile(body,str(path),'exec'),api.__dict__)
    unitree = api.frozen_module(source,frozen,'tests/ros_unitree/common.py','unitree_direct_build_owners')
    return source,frozen,api,unitree


def inside(frame, path, *, owned=False):
    path = Path(path).resolve(strict=True)
    require(path.is_relative_to(frame.source) or any(path.is_relative_to(root) for root in frame.roots) or
            (owned and path.is_relative_to(frame.out)), 'complete source/tool/generated inventory required: '+str(path))
    return path


def source_pin(frame, relative):
    pin = frame.pin(frame.source / relative)
    require(frame.frozen.get(relative) == {key:pin[key] for key in ('bytes','sha256')}, 'actual source differs from complete MF')
    return pin


def selected_cpp_archives(frame, api, sysroot, labels):
    """Qualify the explicit same-profile pair; never select a fallback library.

    The same current manifest supplies the reviewed Legacy layout policy. Its
    flat/noeh/eh result describes the selected nonthread, non-LTO WASIp1
    directory, not the command's C++ exception flags. Each physical archive
    remains an ordinary selected whole-SDK full-seven input. Actual Direct
    links must separately prove that both selected archives were consumed.
    """
    require(type(labels) is dict and set(labels)=={'libcxx','libcxxabi'} and
            all(type(label) is str and label in frame.inputs for label in labels.values()),
            'two explicit current C++ archive input labels required')
    selected={key:frame.inputs[label] for key,label in labels.items()}
    paths={key:Path(pin['path']) for key,pin in selected.items()}
    policy=api.frozen_module(frame.source,frame.frozen,
        'tests/ros_unitree/prepare_legacy_guests.py','direct_explicit_cpp_archive_layout')
    profile=policy.wasi_cpp_archive_profile(sysroot,paths)
    for key,path in paths.items():
        require(inside(frame,path)==path and frame.pin(path)==selected[key],
                'explicit Direct C++ archive must retain its selected whole-SDK full-seven')
    return {'profile':profile,'target':'wasm32-wasip1','archives':selected,
            'selection':'EXPLICIT_CURRENT_INPUT_PAIR_NOT_DISCOVERED_OR_SUBSTITUTED'}


def prerequisite(frame, admission, api, selection):
    """Consume real current qualification, retaining original source labels."""
    evidence = admission.Evidence(api,frame,selection['restart_mappings'])
    packet = evidence.selected(selection['typed_build'])
    admission.clean_build(packet,admission.BUILD_STATUS,
        ('source_preserved','input_pins_preserved','whole_roots_preserved'))
    require(packet['kind'] == 'uwvm-unitree-command14-continuation-builds-v1' and
            packet.get('retained_command_products') == 9 and packet.get('fresh_command_products') == 5 and
            packet.get('granted_rights') == [] and packet.get('failed_batch_promoted_to_success') is False,
            'actual completed9-retained5-fresh binary-only prerequisite required')
    old_source, old_frozen = evidence.source(packet)
    official = packet['official_cpp_object']['qualification']
    evidence.retained(official)
    require(official['policy'] == 'OFFICIAL_LIBCXXABI_BAREMETAL_ABORT_TU_AND_HEADER_VERBOSE_TRAP_V1' and
            official['Guest_compile_flags'] == ['-UNDEBUG',TRAP] and
            official['official']['commit'] == '4434dabb69916856b824f68a64b029c67175e532',
            'actual checked official termination input required; no fabricated C++ success stub')
    obj = evidence.pin(official['object'])
    rows = [row for row in packet['products'] if row['family'] == 'unitree_go']
    require(rows, 'actual complete GO family absent from typed build')
    family = rows[0]['complete_family']
    require(all(row['complete_family'] == family for row in rows) and
            family['current_complete_profile_bytes_verified'] is True and family['current_emitter_bytes_verified'] is True,
            'one actual full native-bound GO family required')
    profile = evidence.pin(family['profile'])
    current = source_pin(frame,'sdk/robots/unitree/profiles/unitree_go.json')
    require(all(profile[key] == current[key] for key in ('bytes','sha256')), 'current complete GO profile differs')
    generation = evidence.document(family['generation'])
    directory = inside(frame,Path(family['generation']['path']).parent)
    require(str(directory) == selection['generated_go'], 'explicit actual full GO generation directory differs')
    # The original generation receipt is retained as recorded. Every selected
    # complete module is independently opened again; no projected header or
    # code-generator stub is accepted in its place.
    required = ('bounded_records.h','bounded_records.hpp','bounded_records.mjs')
    modules = {name:frame.pin(directory/name) for name in required}
    outputs = generation.get('outputs')
    require(type(outputs) is dict and outputs and generation['format']=='uwvm-ros-custom-bounded-v2' and
            generation['status']=='GENERATED_SOURCE_ONLY' and generation['native_registry_emitted'] is True and
            generation['implicit_permissions']==[] and len(generation['records'])==len(generation['types'])==24,
            'actual complete GO24 generated family required')
    regenerate=selection['kind']=='unitree-direct-go2-modular-build-selection-v1'
    if regenerate:
        # The old report's current_* flags describe its own original source.
        # Reopen that complete source and bind the recorded emitter digests to
        # it. Only a real fresh generator process below can establish current
        # output bytes; no historical flag is rewritten as a current fact.
        for relative,key in (('sdk/tools/custom_types.py','generator_sha256'),
                             ('sdk/custom-types/bounded_profile.py','bounded_generator_sha256')):
            original=frame.pin(old_source/relative)
            require(old_frozen.get(relative)=={part:original[part] for part in ('bytes','sha256')} and
                    original['sha256']==generation[key], 'original generation emitter/source provenance differs')
    else:
        require(generation['generator_sha256']==source_pin(frame,'sdk/tools/custom_types.py')['sha256'] and
                generation['bounded_generator_sha256']==source_pin(frame,'sdk/custom-types/bounded_profile.py')['sha256'],
                'exact unchanged complete emitters required; no source-version relabeling')
    profile_document,_=frame.common.document(frame.source/'sdk/robots/unitree/profiles/unitree_go.json')
    wanted={row['type'] for row in profile_document['definitions']}
    native_catalog=evidence.document(family['native_catalog'])
    require(len(wanted)==24 and {row['type'] for row in generation['records']}==
            {row['type'] for row in generation['types']}==wanted and
            {row['type']:row['native_identity'] for row in native_catalog['types']}==
            {row['type']:row['native_identity'] for row in generation['types']},
            'all native-bound GO identities/records must match the complete current profile')
    catalog_pin=evidence.pin(family['native_catalog'])
    require({key:catalog_pin[key] for key in ('bytes','sha256')}==generation['source_inputs']['catalog'],
            'generation is not bound to the actual complete native catalog')
    for definition in generation['source_inputs']['definitions']:
        relative=Path(definition['file'])
        require(not relative.is_absolute() and '..' not in relative.parts,'safe complete definition path required')
        pin=frame.pin(Path(catalog_pin['path']).parent/relative)
        require(all(pin[key]==definition[key] for key in ('bytes','sha256')),'actual generated definition bytes changed')
    for name,expected in outputs.items():
        require(type(name) is str and Path(name).name==name and name not in ('.','..'), 'safe actual generation output name required')
        actual=frame.pin(directory/name)
        require({key:actual[key] for key in ('bytes','sha256')}==expected, 'complete generated output differs from actual receipt')
    require(set(required)<=outputs.keys(), 'complete generated GO modules absent')
    if regenerate:
        frame.direct_regeneration={'catalog':catalog_pin,'historical_generation':evidence.pin(family['generation']),
            'original_generation_body':generation,'original_complete_family':family,
            'profile':current,'original_typed_report':frame.inputs[selection['typed_build']],
            'original_source':str(old_source),'original_manifest_sha256':packet['source_manifest_sha256']}
    frame.report['typed_prerequisite'] = {'report':frame.inputs[selection['typed_build']],
        'original_source':str(old_source),'original_manifest_sha256':packet['source_manifest_sha256'],
        'selected_official_object_current':obj,'complete_GO_generation':evidence.pin(family['generation']),
        'complete_GO_modules':modules,'original_reports_rewritten':False,
        'selected_pin_bridges':evidence.bridges,'scope':'ORIGINAL_NATIVE_BOUND_INPUTS_PENDING_ACTUAL_CURRENT_REGENERATION' if regenerate else
            'CURRENT_INPUT_CONTENT_ONLY_NOT_RUNTIME_OR_PHYSICS'}
    overlay=evidence.document(official['report'])
    require(overlay.get('kind')=='wasi-cpp-official-termination-overlay' and
            overlay.get('status')=='PASS_OFFICIAL_CPP_TERMINATION_OBJECT_PREPARATION_ONLY' and
            evidence.pin(overlay['overlay']['object'])==obj,
            'the retained official object must have its original independent successful producer')
    return obj,directory,evidence,overlay['overlay']


def boundary(frame,api,utilities,schema,path,positive,*,modular=False):
    """Actual binary signatures and local entry, independent of compiler flags."""
    inspector = api.frozen_module(frame.source,frame.frozen,'sdk/tools/check_wasm_imports.py','direct_guest_binary_reader')
    raw = api.bounded(path,64*MIB);pin = frame.pin(path)
    require(hashlib.sha256(raw).hexdigest() == pin['sha256'], 'actual binary changed during inspection')
    table = utilities.wasm_structure(raw,inspector)
    expected = {row['name']:([item['type'] for item in row['params']],row['results'])
                for row in schema['modules']['uwvm_ros2_v1']['functions']}
    wasi = {'fd_read':(['i32']*4,['i32']),'fd_write':(['i32']*4,['i32']),'proc_exit':(['i32'],[])}
    graph = api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/direct_modular.py','direct_actual_modular_binary_contract')
    graph.main_imports(table['imports'], modular and positive)
    seen,ros = set(),set()
    for row in table['imports']:
        key = row['module'],row['name'];require(key not in seen and row['kind'] == 0, 'unique function imports only');seen.add(key)
        signature = row['params'],row['results']
        if row['module'] == 'uwvm_ros2_v1':
            require(expected.get(row['name']) == signature, 'actual ROS2 ABI signature differs');ros.add(row['name'])
        elif row['module'] in graph.NAMES:
            require(modular and positive and graph.MAIN_IMPORTS.get(key)==signature, 'explicit positive pure graph imports only')
        else:
            require(row['module'] == 'wasi_snapshot_preview1' and wasi.get(row['name']) == signature,
                    'only basic stdio/exit WASI is admitted; no env/preload/optional right')
    require('capability_open' in ros and (not positive or {'event_poll','topic_publish'} <= ros), 'real selected ROS imports required')
    require({('wasi_snapshot_preview1','fd_write'),('wasi_snapshot_preview1','proc_exit')} <= seen and
            (not positive or ('wasi_snapshot_preview1','fd_read') in seen), 'actual basic command I/O required')
    require(table['start_function'] is None and len(table['memories']) == 1 and
            table['memories'][0]['imported'] is False and table['memories'][0]['maximum'] == 1024,
            'one owned64MiB bounded memory, no implicit Wasm start required')
    require(table['exports'].get('memory',{}).get('kind')==2 and table['exports']['memory']['index']==0,
            'the actual single owned memory0 must be exported')
    entry = table['exports'].get('_start')
    imports = sum(row['kind'] == 0 for row in table['imports'])
    require(entry and entry['kind'] == 0 and entry['params'] == entry['results'] == [] and entry['index'] >= imports,
            'actual same-module local void standalone entry required')
    return {'artifact':pin,'imports':table['imports'],'memories':table['memories'],
        'entry':entry,'function_import_count':imports,'local_defined_entry_index':entry['index']-imports,
        'implicit_start':False,'default_authority':[],'Runtime_executed':False}


def provider_boundary(frame,api,utilities,path,name):
    """Independently inspect the real pure preload; validation is not execution."""
    inspector=api.frozen_module(frame.source,frame.frozen,'sdk/tools/check_wasm_imports.py','direct_provider_binary_reader')
    graph=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/direct_modular.py','direct_provider_graph_contract')
    raw=api.bounded(path,64*MIB);pin=frame.pin(path)
    require(hashlib.sha256(raw).hexdigest()==pin['sha256'],'actual provider changed during binary inspection')
    return {'artifact':pin,**graph.binary(utilities.wasm_structure(raw,inspector),name)}


def protocol_boundary(frame,api,utilities,path):
    """Inspect the real no-host-import protocol root; never execute it here."""
    inspector=api.frozen_module(frame.source,frame.frozen,'sdk/tools/check_wasm_imports.py','direct_protocol_binary_reader')
    graph=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/direct_modular.py','direct_protocol_graph_contract')
    raw=api.bounded(path,64*MIB);pin=frame.pin(path)
    require(hashlib.sha256(raw).hexdigest()==pin['sha256'],'actual protocol fixture changed')
    table=utilities.wasm_structure(raw,inspector);graph.main_imports(table['imports'],True)
    require(all((row['module'],row['name']) in graph.MAIN_IMPORTS for row in table['imports']) and
            table['start_function'] is None and table['memories']==[{'minimum':2,'maximum':2,'imported':False}] and
            set(table['exports'])=={'composition_protocol','memory'}, 'pure bounded protocol root required')
    entry=table['exports']['composition_protocol'];imports=len(table['imports'])
    require(entry['kind']==0 and entry['params']==entry['results']==[] and entry['index']>=imports and
            table['exports']['memory']=={'kind':2,'index':0}, 'actual local void protocol entry and memory0 required')
    return {'artifact':pin,'imports':table['imports'],'exports':table['exports'],'memories':table['memories'],
            'local_defined_entry_index':entry['index']-imports,'Runtime_executed':False,'default_authority':[]}


def build(frame,api,unitree,selection):
    independent=selection.get('kind') in ('unitree-direct-go2-standalone-independent-build-selection-v2',
        'unitree-direct-go2-modular-independent-build-selection-v2')
    if independent:
        unitree.exact(selection,('format_version','kind','tools','paths','cpp_library','native_family_library','generated_go','restart_mappings'),
            'independent Direct v2 selection')
    else:
        unitree.exact(selection,('format_version','kind','tools','paths','typed_build','generated_go','restart_mappings'), 'direct Guest build selection')
    graph = api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/direct_modular.py','direct_actual_modular_build_contract')
    modular = selection['kind'] in (graph.BUILD_SELECTION,'unitree-direct-go2-modular-independent-build-selection-v2')
    require((independent and selection['format_version']==2) or (not independent and selection['format_version']==1 and
        (modular or selection['kind']=='unitree-direct-go2-standalone-build-selection-v1')), 'closed current build selection required')
    unitree.exact(selection['tools'],('cc','cxx','python','validator','libc','libm','printscan','builtins','libcxx','libcxxabi'), 'actual selected tool/archive labels')
    unitree.exact(selection['paths'],('sysroot','resource_dir','wasm_ld_alias','quickjs'), 'complete selected SDK paths')
    cg = frame.config['cgroup']
    unitree.exact(cg, ('memory_max','memory_swap_max','expected_device','expected_inode',
                      'expected_boot_id','expected_leaf','expected_cpus'), 'explicit current build cgroup')
    require(cg['memory_max'] == 64*1024**3 and cg['memory_swap_max'] == 0 and
            cg['expected_leaf'] == CG_LEAF and cg['expected_cpus'] == '0-31' and
            type(cg['expected_device']) is int and cg['expected_device'] >= 0 and
            type(cg['expected_inode']) is int and cg['expected_inode'] > 0 and
            type(cg['expected_boot_id']) is str and
            re.fullmatch(r'[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}',cg['expected_boot_id']) and
            all(frame.before[key] == cg[selected] for key,selected in
                (('memory_max','memory_max'),('memory_swap_max','memory_swap_max'),('path','expected_leaf'),
                 ('cpus','expected_cpus'),('device','expected_device'),('inode','expected_inode'),('boot_id','expected_boot_id'))),
            'caller-selected actual current64GiB/swap0 original leaf/CPU and exact incarnation required')
    frame.guard()
    require(all(key is None for key in (frame.config['simulation'],frame.config['benchmark'],frame.config['profiler'])) and
            frame.limits['jobs'] == 1 and frame.limits['rss_bytes'] <= 2*1024**3,
            'compiler-only serial low-memory/current64 configuration required')
    tools = {name:str(frame.input(label)) for name,label in selection['tools'].items()}
    paths = {name:inside(frame,path) for name,path in selection['paths'].items()}
    alias = Path(selection['paths']['wasm_ld_alias'])
    require(alias.name.startswith('wasm-ld'), 'actual selected multicall Wasm linker basename required')
    require(alias.parent.resolve(strict=True) == alias.parent and alias.resolve(strict=True) == paths['wasm_ld_alias'], 'actual canonical selected linker alias required')
    utilities = api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/acceptance/typed40_boot2/command14-utilities.py','direct_guest_full_md_and_binary')
    admission = api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/typed_transport_inputs.py','direct_guest_selected_current_inputs')
    if independent:
        independent_api=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/direct_independent_inputs.py','direct_completed_A1_A2_inputs')
        official,generated,evidence,official_proof=independent_api.prerequisite(frame,api,admission,selection)
    else:
        official,generated,evidence,official_proof = prerequisite(frame,admission,api,selection)
    require(official_proof['compiler']['physical']==str(Path(tools['cxx']).resolve(strict=True)) and
            official_proof['sysroot']==str(paths['sysroot']) and
            official_proof['Guest_compile_flags']==['-UNDEBUG',TRAP],
            'actual C++ compiler/sysroot must match the independently qualified official termination input')
    for key,name in (('libc','libc.a'),('libm','libm.a'),('printscan','libc-printscan-long-double.a')):
        require(Path(tools[key])==(paths['sysroot']/'lib/wasm32-wasip1'/name).resolve(strict=True),
                'selected archive must belong to the actual matching WASI sysroot: '+key)
    cpp_archives=selected_cpp_archives(frame,api,paths['sysroot'],
        {key:selection['tools'][key] for key in ('libcxx','libcxxabi')})
    schema,_ = frame.common.document(frame.source/'sdk/abi/abi-v1.json')
    owner = frame.common.SerialRunner(frame.out,frame.limits,frame.guard);frame.actors.append(owner)
    frame.report.update(products=[],providers=[],commands=owner.commands,planned_products=6,planned_provider_products=3 if modular else 0,Guest_languages=['c','cpp','javascript'],
        default_ROS_authority=[],optional_WASI_rights=[],Runtime_executed=False,ROS_DDS_initialized=False,
        simulator_executed=False,performance_qualified=False,selected_cpp_archives=cpp_archives)
    def run(label,argv,cwd):
        row = utilities.run(frame,owner,label,argv,cwd,frame.environment);frame.save();return row
    def command(row,deps,**extra):
        # Relative names in an actual full-MD rule or linker trace must be
        # resolved against the compiler's real working directory later. Do
        # not ask the independent consumer to infer it from a source path.
        return {**utilities.receipt_command(frame,row,deps),'working_directory':row['cwd'],**extra}
    def compile_one(label,source,folder,flags,compiler):
        obj,dep = folder/(label+'.o'),folder/(label+'.d')
        argv = [compiler,*flags,'-MD','-MF',str(dep),'-MT',str(obj),'-c',str(source),'-o',str(obj)]
        row = run(folder.name+'-'+label,argv,folder)
        deps = utilities.full_md(frame,dep,obj,source,folder)
        return obj,command(row,deps,role='compile',source=frame.pin(source),object=frame.pin(obj),dependency_file=frame.pin(dep))
    current_family=None
    if modular or independent:
        previous=frame.direct_regeneration;old=previous['original_generation_body']
        generated=frame.out/'current-go24-generated'
        generator=source_pin(frame,'sdk/tools/custom_types.py')
        bounded_generator=source_pin(frame,'sdk/custom-types/bounded_profile.py')
        argv=[tools['python'],'-B',generator['path'],previous['catalog']['path'],'--out',str(generated)]
        row=run('regenerate-current-native-bound-GO24',argv,frame.out)
        fresh,generation_pin=frame.common.document(generated/'generation.json')
        names=graph.generation_equivalence(old,fresh,generator['sha256'],bounded_generator['sha256'])
        outputs={name:frame.pin(generated/name) for name in sorted(names)}
        for name,pin in outputs.items():
            require({key:pin[key] for key in ('bytes','sha256')}==fresh['outputs'][name], 'fresh current generated output changed')
        dependencies=[generator,bounded_generator,frame.inputs[selection['tools']['python']],previous['catalog'],previous['profile']]
        for definition in fresh['source_inputs']['definitions']:
            relative=Path(definition['file']);require(not relative.is_absolute() and '..' not in relative.parts,'safe retained definition path required')
            pin=frame.pin(Path(previous['catalog']['path']).parent/relative)
            require(all(pin[key]==definition[key] for key in ('bytes','sha256')),'official retained definition changed during fresh generation')
            dependencies.append(pin)
        body={key:value for key,value in previous.items() if key!='original_generation_body'}
        body.update(kind=(independent_api.GENERATION if independent else 'uwvm-unitree-current-go24-generation-admission-v1'),status='CURRENT_SOURCE_GENERATION_IDENTICAL_NATIVE_PROVIDER_BYTES_NOT_EXECUTED',
            source=str(frame.source),source_manifest_sha256=frame.report['source_manifest_sha256'],
            generation=generation_pin,generator=generator,bounded_generator=bounded_generator,outputs=outputs,python=frame.inputs[selection['tools']['python']],
            original_native_identities_preserved=True,compiled_provider_output_bytes_equal=True,old_reports_rewritten=False,
            current_emitter_executed=True,Runtime_executed=False,commands=[command(row,dependencies,role='generate')])
        if independent:
            # C52 compiled the native package/exporter before source emission.
            # Its generated conversion header is source-only here; the owned
            # physics consumer independently requires the Gateway full-MD role.
            body['compiled_provider_output_bytes_equal']=False
            body['original_generated_provider_output_bytes_equal']=True
            body['generated_provider_compilation_qualification']='REQUIRES_SEPARATE_CURRENT_NATIVE_GATEWAY_FULL_MD_ADMISSION'
        path=frame.out/'current-go24-generation-admission.json';frame.common.atomic_json(path,body)
        current_family=frame.pin(path);frame.report['current_GO24_generation_admission']=current_family
    base = ['--target=wasm32-wasip1','--sysroot='+str(paths['sysroot']),'-resource-dir='+str(paths['resource_dir']),
            '-O3','-g0','-UNDEBUG','-ffreestanding','-fno-builtin','-fno-fast-math','-ffp-contract=off','-ffunction-sections','-fdata-sections']
    includes = []
    for path in (frame.source/'sdk/include',frame.source/'sdk/robots/unitree/include',generated,
                 frame.source/'third-parties/fast_io/include',frame.source/'third-parties/bizwen/include',frame.source/'third-parties/boost_unordered/include'):
        includes += ['-I',str(inside(frame,path,owned=True))]
    if modular:includes += ['-I',str(inside(frame,frame.source/'sdk/examples/robotics_modular'))]
    if modular:
        # Fresh provider objects have full-MD evidence and the exact same O3,
        # contraction-off compiler/sysroot as the actual root commands. Their
        # final imports/exports are checked independently of linker options.
        for provider in graph.NAMES:
            folder=frame.out/('provider-'+provider);folder.mkdir(mode=0o700)
            source=frame.source/graph.SOURCES[provider][0];artifact=folder/'provider.wasm'
            obj,compiled=compile_one('provider',source,folder,[*base,*includes,'--driver-mode=gcc','-std=c11'],tools['cc'])
            argv=[tools['cc'],*base[:3],'-fuse-ld='+str(alias),'-nostdlib','-nostartfiles','-Wl,--threads=1',
                  '-Wl,--gc-sections','-Wl,--trace','-Wl,--no-entry','-Wl,--export-memory',
                  '-Wl,--initial-memory=131072','-Wl,--max-memory=131072','-Wl,-z,stack-size=65536',
                  '-Wl,--allow-undefined',str(obj),
                  *([tools[key] for key in ('libm','libc','builtins')] if provider=='motion' else []),'-o',str(artifact)]
            linked=run('provider-'+provider+'-link',argv,folder);links=utilities.trace_inputs(frame,linked,folder)
            require(frame.pin(obj) in links,'actual provider link omitted its fresh object')
            validated=run('provider-'+provider+'-validate',[tools['validator'],'validate',str(artifact)],folder)
            inspected=provider_boundary(frame,api,utilities,artifact,provider)
            proof={'format_version':1,'kind':graph.PROVIDER_KIND,'status':'PASS_BINARY_VALIDATED_NOT_EXECUTED',
                'source':str(frame.source),'source_manifest_sha256':frame.report['source_manifest_sha256'],
                'language':'c','positive':False,'provider':provider,'output':frame.pin(artifact),'definitions':{},
                'producer':source_pin(frame,'tests/ros_unitree/prepare_direct_guests.py'),
                'build_configuration':frame.report['configuration'],
                'tools':{key:frame.inputs[label] for key,label in selection['tools'].items()},
                'source_inputs':[source_pin(frame,path) for path in graph.SOURCES[provider]],
                'commands':[compiled,command(linked,links,role='link'),command(validated,[frame.pin(artifact)],role='validate')],
                'binary_boundary':inspected,'embedded':None,'full_dependencies_verified':True,
                'default_authority':[],'Runtime_executed':False}
            receipt=folder/'build-receipt.json';frame.common.atomic_json(receipt,proof)
            frame.report['providers'].append({'name':provider,'artifact':frame.pin(artifact),
                'receipt':frame.pin(receipt),'binary_boundary':inspected});frame.save()
    if modular:
        folder=frame.out/'preload-protocol';folder.mkdir(mode=0o700);artifact=folder/'protocol.wasm'
        source=frame.source/'sdk/examples/robotics_modular/unitree_composition_protocol.c'
        obj,compiled=compile_one('protocol',source,folder,[*base,*includes,'--driver-mode=gcc','-std=c11'],tools['cc'])
        argv=[tools['cc'],*base[:3],'-fuse-ld='+str(alias),'-nostdlib','-nostartfiles','-Wl,--threads=1',
              '-Wl,--gc-sections','-Wl,--trace','-Wl,--no-entry','-Wl,--export-memory',
              '-Wl,--initial-memory=131072','-Wl,--max-memory=131072','-Wl,-z,stack-size=65536',
              '-Wl,--allow-undefined',str(obj),tools['libc'],tools['builtins'],'-o',str(artifact)]
        linked=run('preload-protocol-link',argv,folder);links=utilities.trace_inputs(frame,linked,folder)
        require(frame.pin(obj) in links,'protocol link omitted fresh object')
        validated=run('preload-protocol-validate',[tools['validator'],'validate',str(artifact)],folder)
        inspected=protocol_boundary(frame,api,utilities,artifact)
        proof={'kind':'unitree-direct-preload-protocol-build-v1','status':'PASS_BINARY_VALIDATED_NOT_EXECUTED',
            'source':str(frame.source),'source_manifest_sha256':frame.report['source_manifest_sha256'],
            'output':frame.pin(artifact),'source_inputs':[source_pin(frame,str(source.relative_to(frame.source))),
                *[source_pin(frame,path) for path in graph.ROOT_SOURCES]],'providers':frame.report['providers'],
            'commands':[compiled,command(linked,links,role='link'),command(validated,[frame.pin(artifact)],role='validate')],
            'binary_boundary':inspected,'Runtime_executed':False,'default_authority':[]}
        path=folder/'build-receipt.json';frame.common.atomic_json(path,proof)
        frame.report['preload_protocol_fixture']={'artifact':frame.pin(artifact),'receipt':frame.pin(path),'binary_boundary':inspected}
    for language in ('c','cpp','javascript'):
        for positive in (False,True):
            name = language+('-positive' if positive else '-denied');folder = frame.out/name;folder.mkdir(mode=0o700)
            artifact = folder/'guest.wasm';records=[];associations=[];embedded=None
            definitions = {'UWVM_UNITREE_DIRECT_ENABLED':int(positive)}
            if language == 'cpp' or modular:definitions['UWVM_UNITREE_MODULAR_MOTION']=int(modular and positive)
            if modular:definitions['UWVM_UNITREE_MODULAR_PERCEPTION']=int(positive)
            if language == 'javascript':definitions['UWVM_JS_UNITREE_CONTROL']=1
            if language != 'javascript':
                flags = [*base,*includes,*['-D'+key+'='+str(value) for key,value in definitions.items()]]
                cpp = ['--driver-mode=g++','-std=c++20','-fno-rtti','-fno-exceptions',TRAP]
                jobs = [(frame.source/(PREFIX+'direct_go2.'+('c' if language == 'c' else 'cpp')),
                         tools['cc'] if language == 'c' else tools['cxx'],
                         [*flags,*(['--driver-mode=gcc','-std=c11'] if language == 'c' else cpp),'-Dmain=uwvm_unitree_direct_main'])]
                if language == 'c':jobs.append((frame.source/(PREFIX+'direct_go2_output.cpp'),tools['cxx'],[*flags,*cpp]))
                jobs.append((frame.source/(PREFIX+'direct_main_entry.cpp'),tools['cxx'],
                             [*flags,*cpp,'-DUWVM_UNITREE_DIRECT_C_MAIN='+str(int(language == 'c'))]))
                objects=[]
                for i,(source,compiler,flags_) in enumerate(jobs):
                    source_pin(frame,str(source.relative_to(frame.source)))
                    obj,record=compile_one('tu'+str(i),source,folder,flags_,compiler);objects.append(obj);records.append(record)
                allow=folder/'allowed-symbols.txt'
                allow.write_text('\n'.join((['uwvm_unitree_motion_'+name for name in ('begin','status','word','end')]+
                    ['uwvm_unitree_perception_'+name for name in ('begin','status','word','end','abort')] if modular and positive else [])+['uwvm_ros2_'+row['name'] for row in schema['modules']['uwvm_ros2_v1']['functions']]+['uwvm_unitree_example_read','uwvm_unitree_example_write','uwvm_unitree_direct_exit'])+'\n')
                argv=[tools['cxx'],'--driver-mode=g++',*base[:3],'-fuse-ld='+str(alias),'-nostdlib','-nostartfiles',
                      '-Wl,--threads=1','-Wl,--gc-sections','-Wl,--trace','-Wl,--no-entry','-Wl,--export=_start',
                      '-Wl,--export-memory','-Wl,--stack-first','-Wl,-z,stack-size=1048576','-Wl,--initial-memory=2097152',
                      '-Wl,--max-memory=67108864','-Wl,--allow-undefined-file='+str(allow),
                      *map(str,objects),official['path'],*[tools[key] for key in ('libcxx','libcxxabi','libm','printscan','libc','builtins')],'-o',str(artifact)]
                linked=run(name+'-link',argv,folder);links=utilities.trace_inputs(frame,linked,folder)
                require(all(frame.pin(obj) in links for obj in objects) and official in links, 'actual link trace omitted owned or qualified official object')
                require(all(pin in links for pin in cpp_archives['archives'].values()),
                        'actual Direct link trace omitted an explicitly selected C++ archive')
                records.append(command(linked,links,role='link'));frame.pin(allow)
                source_inputs=[source_pin(frame,str(path.relative_to(frame.source))) for path,_,_ in jobs]
                source_inputs += [source_pin(frame,'sdk/robots/unitree/include/roswasm_unitree/'+name_) for name_ in ('simulation_control.h','crc.h')]
            else:
                # The real SDK producer records closed8 file observations,
                # including raw st_mode. Load the shared reviewed association
                # from this SAME complete source manifest; loading definitions
                # executes no Legacy build, main, compiler, Guest or actor.
                javascript_observations=api.frozen_module(frame.source,frame.frozen,
                    'tests/ros_unitree/prepare_legacy_guests.py','direct_javascript_prepared_file_observations')
                # Copy exact source-relative SDK/module bytes into owned work;
                # original manifests and the generated GO module stay intact.
                stage=folder/'source';stage.mkdir(mode=0o700)
                def copy(original,relative):
                    pin=frame.pin(original);destination=stage/relative;destination.parent.mkdir(parents=True,exist_ok=True)
                    raw=api.bounded(original,8*MIB);require(hashlib.sha256(raw).hexdigest()==pin['sha256'],'staged source changed')
                    destination.write_bytes(raw);current=frame.pin(destination)
                    require(all(current[key]==pin[key] for key in ('bytes','sha256')),'source copy changed bytes')
                    associations.append({'source':pin,'copy':current})
                prefixes=('sdk/javascript/','sdk/include/','sdk/abi/','sdk/robots/unitree/include/')
                originals=[relative for relative in frame.frozen if relative.startswith(prefixes) and '/tests/' not in relative]
                originals += [PREFIX+'javascript/direct_go2.mjs',PREFIX+'javascript/control_kernel.c']
                if modular:originals += list(graph.ROOT_SOURCES)
                for relative in sorted(set(originals)):copy(frame.source/relative,relative)
                copy(generated/'bounded_records.mjs',PREFIX+'javascript/bounded_records.mjs')
                modules=[{'name':str(path.relative_to(stage)),'source':str(path.relative_to(stage))}
                         for path in sorted(stage.rglob('*.mjs'))]
                manifest={'schema':'uwvm-embedded-javascript-build-v1','mode':'command','backend':2,
                    'entry':PREFIX+'javascript/direct_go2.mjs','modules':modules,'imports':[],'exports':[],
                    'limits':{'heap_bytes':16*MIB,'stack_bytes':MIB,'max_jobs':4096,'interrupt_polls':10000000,
                              'input_bytes':32768,'output_bytes':32768,'print_bytes':4096,'max_text_bytes':32768,
                              'max_modules':128,'max_source_bytes':8*MIB},
                    'random_seed':1,'optimization':'-O3','initial_memory':32*MIB,'maximum_memory':64*MIB}
                manifest_path=folder/'manifest.json';frame.common.atomic_json(manifest_path,manifest)
                prepared=folder/'prepared'
                argv=[tools['python'],'-B',str(stage/'sdk/javascript/guest/prepare.py'),'--source-root',str(stage),
                      '--quickjs',str(paths['quickjs']),'--manifest',str(manifest_path),'--out',str(prepared),
                      '--clang',tools['cc'],'--linker',str(alias),'--sysroot',str(paths['sysroot']),
                      '--resource-dir',str(paths['resource_dir']),*sum((['--'+key,tools[key]] for key in ('libc','libm','printscan','builtins')),[]),
                      '--validator',tools['validator'],'--unitree-direct-enabled',str(int(positive)),
                      *(['--unitree-modular-motion',str(int(positive))] if modular else [])]
                prepared_job=run(name+'-metadata',argv,folder);records.append(command(prepared_job,[frame.pin(manifest_path)],role='metadata'))
                metadata,_=frame.common.document(prepared/'report.json')
                require(metadata['status']=='PREPARED_NOT_COMPILED' and metadata['compiler_executed'] is False and
                        metadata['manifest']==manifest and metadata['implicit_permissions']==[] and
                        metadata['unitree_control']['direct_enabled']==int(positive),'actual closed same-Wasm preparation required')
                objects=[]
                for job in metadata['jobs']:
                    require(job['cwd']==str(prepared),'prepared direct engine job cwd differs')
                    if 'source' in job:
                        source=Path(job['source']['path']);obj=Path(job['artifact']);dep=Path(job['dependencies'])
                        argv=job['argv']
                        require(source.is_relative_to(stage) or source.is_relative_to(prepared),'engine source outside admitted stage/preparation')
                        require(obj.parent==prepared/'objects' and dep==obj.with_suffix('.d') and
                                argv[0]==tools['cc'] and all(flag in argv for flag in
                                ('--target=wasm32-wasip1','-O3','-UNDEBUG','-fno-fast-math','-ffp-contract=off','-MD','-MF','-MT','-c','-o',
                                 '-DUWVM_JS_UNITREE_CONTROL=1','-DUWVM_UNITREE_DIRECT_ENABLED='+str(int(positive)))) and
                                argv[argv.index('-c')+1]==str(source) and argv[argv.index('-o')+1]==str(obj) and
                                argv[argv.index('-MF')+1]==str(dep) and argv[argv.index('-MT')+1]==str(obj),
                                'actual exact same-Wasm engine full-MD/source/object/definition association required')
                        source_qualification=javascript_observations.javascript_prepared_file(frame,job['source'])
                        row=run(name+'-'+job['id'],argv,prepared)
                        deps=utilities.full_md(frame,dep,obj,source,prepared);objects.append(obj)
                        records.append(command(row,deps,role='compile',source=frame.pin(source),object=frame.pin(obj),dependency_file=frame.pin(dep),
                            javascript_prepared_source=source_qualification))
                    elif job['id']=='link':
                        argv=job['argv']
                        require(argv[0]==tools['cc'] and all(str(obj) in argv for obj in objects) and
                                all(tools[key] in argv for key in ('libm','printscan','libc','builtins')) and
                                '-Wl,--threads=1' in argv and '-Wl,--trace' in argv and
                                argv[argv.index('-o')+1]==str(prepared/'guest.wasm'), 'actual selected bounded direct engine link required')
                        row=run(name+'-'+job['id'],argv,prepared)
                        deps=utilities.trace_inputs(frame,row,prepared)
                        require(all(frame.pin(obj) in deps for obj in objects),'actual embedded link omitted an engine/kernel object')
                        records.append(command(row,deps,role='link'))
                    else:
                        require(job['id']=='validate' and job['argv']==[tools['validator'],'validate',str(prepared/'guest.wasm')], 'actual engine validator differs')
                        row=run(name+'-'+job['id'],job['argv'],prepared)
                        records.append(command(row,[frame.pin(prepared/'guest.wasm')],role='validate'))
                artifact=prepared/'guest.wasm';embedded={'manifest':frame.pin(manifest_path),'preparation':frame.pin(prepared/'report.json'),
                    'module_table':frame.pin(prepared/'module_table.c'),'source_associations':associations,
                    'same_Wasm_local_C_kernel':not(modular and positive),'native_bridge':False}
                # Retain the original raw SDK observations separately. Every
                # prepared input (including generated C) is requalified after
                # the actual serial jobs, not projected to a weaker schema.
                embedded['preparation_inputs']=metadata['inputs']
                embedded['preparation_input_qualifications']=[
                    javascript_observations.javascript_prepared_file(frame,pin) for pin in metadata['inputs']]
                embedded['control_source_qualification']=javascript_observations.javascript_prepared_file(
                    frame,metadata['unitree_control']['source'])
                require(embedded['control_source_qualification']['current_file']['path']==
                        str(stage/(PREFIX+'javascript/control_kernel.c')),'actual prepared same-main control source differs')
                if modular:
                    embedded.update(same_Wasm_local_C_adapter=True,
                        mathematical_kernel_location='EXPLICIT_PURE_PRELOADED_MOTION' if positive else 'UNUSED_DEFAULT_DENIED')
                source_inputs=[row['source'] for row in associations if Path(row['source']['path']).is_relative_to(frame.source)]
            if modular and positive:
                source_inputs += [source_pin(frame,relative) for relative in graph.ROOT_SOURCES]
            if language!='javascript':
                validated=run(name+'-validate',[tools['validator'],'validate',str(artifact)],folder)
                records.append(command(validated,[frame.pin(artifact)],role='validate'))
            inspected=boundary(frame,api,utilities,schema,artifact,positive,modular=modular)
            proof={'format_version':1,'kind':graph.MAIN_KIND if modular else 'unitree-direct-standalone-build-v1','status':'PASS_BINARY_VALIDATED_NOT_EXECUTED',
                'source':str(frame.source),'source_manifest_sha256':frame.report['source_manifest_sha256'],
                'language':language,'positive':positive,'output':frame.pin(artifact),'definitions':definitions,
                'producer':source_pin(frame,'tests/ros_unitree/prepare_direct_guests.py'),
                'build_configuration':frame.report['configuration'],
                'tools':{key:frame.inputs[label] for key,label in selection['tools'].items()},
                'source_inputs':source_inputs,'commands':records,'binary_boundary':inspected,'embedded':embedded,
                'full_dependencies_verified':True,'default_authority':[],'Runtime_executed':False}
            if language!='javascript':
                proof['selected_cpp_archives']={**cpp_archives,'actual_link_trace_contains_both':True}
            if modular or independent:
                proof['current_family']=current_family
            if independent:
                proof['independent_library_prerequisites']=frame.report['independent_library_prerequisites']
            if modular:
                proof['composition']={'providers':frame.report['providers'],
                    'observed_input':'REAL_DECODED_LOWSTATE_36_BINARY32_WORDS',
                    'motion_kernel':'EXACT_NATIVE_SIMULATION_CONTROL_HEADER',
                    'callback_protocol':'TWELVE_ORDERED_REAL_WASM_CALLS_AND_CHECKED_RETIREMENT',
                    'image_vision_qualified':False,'main_only_ROS':True}
            receipt=folder/'build-receipt.json';frame.common.atomic_json(receipt,proof)
            frame.report['products'].append({'id':name,'language':language,'positive':positive,'artifact':frame.pin(artifact),
                'receipt':frame.pin(receipt),'binary_boundary':inspected});frame.save()
    require(len(frame.report['products'])==6,'all three real denied/positive standalone mains required')
    frame.report['selected_prerequisite_pin_bridges']=evidence.bridges
    require(len(frame.report['providers'])==(3 if modular else 0),'all selected actual pure provider products required')
    return graph.BUILD_STATUS if modular else 'PASS_SIX_DIRECT_GO2_COMMANDS_COMPILED_VALIDATED_NOT_EXECUTED'


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ('source','manifest','config','out'):parser.add_argument('--'+name,type=Path,required=True)
    for name in ('manifest-sha256','config-sha256','selection-input'):parser.add_argument('--'+name,required=True)
    args=parser.parse_args();frame=None
    try:
        source,frozen,api,unitree=bootstrap(args);frame=unitree.Frame(args,'unitree-direct-go2-standalone-Guest-builds')
        frame.frozen=frozen
        selection,pin=frame.common.document(frame.input(args.selection_input))
        require(pin==frame.inputs[args.selection_input],'actual direct build selection changed')
        if selection.get('kind') in ('unitree-direct-go2-modular-build-selection-v1','unitree-direct-go2-modular-independent-build-selection-v2'):
            frame.report['kind']='unitree-direct-go2-modular-Guest-builds'
        status=build(frame,api,unitree,selection);return frame.finish(status)
    except BaseException as error:
        if frame is not None:return frame.finish('FAIL',repr(error))
        print(repr(error),file=sys.stderr);return 1


if __name__=='__main__':
    raise SystemExit(main())
