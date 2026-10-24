#!/usr/bin/env python3
"""Independently consume standalone Guest and Native74 Go2 build provenance.

This module starts no actor. It preserves the real original producer/source
labels, reopens current physical bytes and full MD/link inputs, and checks the
same C kernel before the separately owned physics protocol may begin. It never
turns binary validation into a DDS, simulator or performance result.
"""
from __future__ import annotations

import hashlib
from pathlib import Path

MIB=1024**2
PIN={'path','bytes','sha256','device','inode','mtime_ns','ctime_ns'}


def require(value,message):
    if not value:raise ValueError(message)


def native_controller(frame,api,admission,evidence,body,label,output_label,required,definitions,*,prepared=None):
    """Read actual Source39 compilation without rewriting a failed producer.

    Native6 completed all translation units and links, then its active-view
    catalog failed on a mounted device placeholder. Only a separately opened
    successful at-rest admission may select those retained products. The raw
    original report remains FAIL; its compilation is neither re-executed nor
    promoted into a Runtime, DDS, physics or performance success.
    """
    admission_receipt=body
    if body.get('schema')=='uwvm-native74-retained-products-at-rest-v1':
        require(prepared is not None and prepared['preparation_mode']=='retained74-at-rest' and
                prepared['admission_receipt']==body and prepared['admission_pin']==frame.inputs[label],
                'actual separately admitted retained74 at-rest producer required')
        admission.clean_build(body,'PASS_RETAINED_NATIVE74_BUILD_PRODUCTS_FRESH_AT_REST_CATALOG_NOT_EXECUTED',
            ('source_preserved','input_pins_preserved','whole_roots_preserved'))
        require(body['retained_compilation']['translation_units']==74 and body['execution_performed'] is False,
                'fresh at-rest admission must retain all74 real compilation units without execution')
        raw_pin=evidence.pin(body['original_raw_inner'])
        body=evidence.document(body['original_raw_inner'])
        require(body==prepared['report'] and body.get('status')=='FAIL',
                'actual original failed Native6 report must remain unchanged and failed')
    else:
        admission.clean_build(body,'PREPARED_NATIVE_PRODUCTS_NOT_EXECUTED')
        raw_pin=frame.inputs[label]
    require(body['kind']=='humble-three-gateways-and-portable-peer' and
            body['qualified_source_translation_units']==74 and body['physics_controller_executed'] is False and
            body['physics_controller_DDS_initialized'] is False and body['matched_physics_baseline_qualified'] is False,
            'actual Native74 source/build completion required; no prior physics PASS is assumed')
    source,frozen=evidence.source(body)
    output=evidence.pin(body['physics_controller']['input'])
    require(output==frame.inputs[output_label] and definitions=={'UWVM_UNITREE_NATIVE_HOLD':0,'UWVM_UNITREE_NATIVE_HG':0},
            'exact actual Go2 stand controller and its native policy required')
    selected=body['physics_controller_contract']
    require(selected['family']=='unitree_go' and selected['mode']=='go2-stand' and selected['cxx_standard']==20 and
            selected['protocol']=='UCK1/R/Q','native configured policy must match actual ordinary compile')
    evidence.retained(selected)
    records=admission.compile_records(evidence,body['physics_controller_compile'])
    require(len(records)==1,'one real native ROS2 controller translation unit required')
    record=records[0];argv=record['actual_compile']['argv']
    require(record['source']['path']==str(source/'test/0073.ros-tests/support/source/ros_unitree/native_ros2_controller.cpp') and
            all(flag in argv for flag in ('-O3','-UNDEBUG','-fno-fast-math','-ffp-contract=off','-std=c++20')) and
            not any(flag in argv for flag in ('-Ofast','-ffast-math','-DNDEBUG')),
            'real matched optimized C++20 native controller flags required')
    for key,value in definitions.items():
        require([flag for flag in argv if flag.startswith('-D'+key+'=')]==['-D'+key+'='+str(value)],
                'native controller compile definition differs')
    dependencies={pin['path']:pin for pin in record['dependencies']}
    relations=[]
    for relative in required:
        require(str(source/relative) in dependencies,'shared native source/kernel absent from full MD')
        relations.append(evidence.source_counterpart(dependencies[str(source/relative)],source,frozen,mandatory=True))
    # Actual verbose link traces, not a configured command alone, bind every
    # input and the produced binary. Loaded mappings remain a live-case check.
    links=body['physics_controller_compile']['resolved_link_inputs']
    require(links and record['object']['input'] in links,'native link trace omitted its real fresh object')
    evidence.retained(links)
    frame.report.setdefault('direct_native_controller_builds',[]).append({'selected_admission':frame.inputs[label],
        'original_raw_producer':raw_pin,'original_raw_status':body['status'],
        'preparation_mode':'retained74-at-rest' if admission_receipt is not body else 'original-success',
        'original_source':str(source),'original_manifest_sha256':body['source_manifest_sha256'],
        'selected_current_binary':output,'current_source_relations':relations,
        'raw_producer_rewritten':False,'physics_execution_qualified':False})
    return body


def current_family(frame,api,selected):
    """Verify real current regeneration while keeping original native roles.

    All eleven message-only output bodies must equal the actual previously
    compiled native-provider generation byte for byte. This is explicit source
    regeneration, not a native recompile, process execution or report rewrite.
    Native export/full-MD/registry association remains the outer admission's
    independent responsibility and is cross-bound there to historical_generation.
    """
    def pin(value):
        require(type(value) is dict and set(value)==PIN and frame.pin(value['path'])==value,
                'actual current-family metadata/input/output changed')
        return value
    def doc(value):
        pin(value);body,actual=frame.common.document(Path(value['path']))
        require(actual==value,'current-family document changed while parsing');return body
    body=doc(selected)
    independent=body.get('kind')=='uwvm-unitree-independent-go24-current-generation-admission-v2'
    require(body.get('kind') in ('uwvm-unitree-current-go24-generation-admission-v1',
            'uwvm-unitree-independent-go24-current-generation-admission-v2') and
            body.get('status')=='CURRENT_SOURCE_GENERATION_IDENTICAL_NATIVE_PROVIDER_BYTES_NOT_EXECUTED' and
            body.get('source')==str(frame.source) and body.get('source_manifest_sha256')==frame.report['source_manifest_sha256'] and
            body.get('original_native_identities_preserved') is body.get('current_emitter_executed') is True and
            ((not independent and body.get('compiled_provider_output_bytes_equal') is True) or
             (independent and body.get('compiled_provider_output_bytes_equal') is False and
              body.get('original_generated_provider_output_bytes_equal') is True and
              body.get('generated_provider_compilation_qualification')=='REQUIRES_SEPARATE_CURRENT_NATIVE_GATEWAY_FULL_MD_ADMISSION')) and body.get('old_reports_rewritten') is body.get('Runtime_executed') is False,
            'actual current source-only family admission required')
    fresh,old=doc(body['generation']),doc(body['historical_generation'])
    catalog,profile=doc(body['catalog']),doc(body['profile'])
    if independent:
        library_api=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/direct_independent_inputs.py','direct_actual_A2_generation_consumer')
        library_api.verify_generation(frame,api,body)
    else:pin(body['original_typed_report'])
    for key,relative in (('generator','sdk/origin/tools/custom_types.py'),('bounded_generator','sdk/origin/custom-types/bounded_profile.py')):
        value=pin(body[key]);require(value['path']==str(frame.source/relative) and
            frame.frozen.get(relative)=={part:value[part] for part in ('bytes','sha256')}, 'actual current emitter differs from complete MF')
    require(fresh['generator_sha256']==body['generator']['sha256'] and fresh['bounded_generator_sha256']==body['bounded_generator']['sha256'] and
            fresh['status']=='GENERATED_SOURCE_ONLY' and fresh['native_registry_emitted'] is True and fresh['implicit_permissions']==[] and
            len(fresh['records'])==len(fresh['types'])==24 and fresh['records']==old['records'] and fresh['types']==old['types'] and
            fresh['outputs']==old['outputs'] and fresh['source_inputs']==old['source_inputs'] and
            fresh['source_inputs']['catalog']=={key:body['catalog'][key] for key in ('bytes','sha256')} and
            {row['type']:row['native_identity'] for row in catalog['types']}=={row['type']:row['native_identity'] for row in fresh['types']} and
            {row['type'] for row in profile['definitions']}=={row['type'] for row in fresh['types']},
            'all24 native IDs/codec contracts and original compiled output bytes must survive actual regeneration')
    graph=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/direct_modular.py','direct_actual_current_generation_contract')
    names=graph.generation_equivalence(old,fresh,body['generator']['sha256'],body['bounded_generator']['sha256'])
    require(set(body['outputs'])==set(fresh['outputs'])==names,'all11 current output files required')
    directory=Path(body['generation']['path']).parent
    for name,value in body['outputs'].items():
        pin(value);require(value['path']==str(directory/name) and
            fresh['outputs'][name]=={key:value[key] for key in ('bytes','sha256')},'actual fresh output/source association differs')
    commands=body['commands'];require(type(commands) is list and len(commands)==1,'one actual current generator command required')
    row=commands[0];require(row['role']=='generate' and row['returncode']==0 and
        row['argv']==[body['python']['path'],'-B',body['generator']['path'],body['catalog']['path'],'--out',str(directory)] and
        Path(row['argv'][0]).is_absolute(), 'actual explicit current generator argv required')
    pin(body['python']);pin(row['stdout']);pin(row['stderr'])
    dependencies=[pin(value) for value in row['dependencies']]
    require(all(body[key] in dependencies for key in ('generator','bounded_generator','catalog','profile')),
            'actual generator command omitted emitter/catalog/profile input pins')
    for definition in fresh['source_inputs']['definitions']:
        relative=Path(definition['file']);require(not relative.is_absolute() and '..' not in relative.parts,'safe original definition path required')
        value=frame.pin(Path(body['catalog']['path']).parent/relative)
        require(value in dependencies and all(value[key]==definition[key] for key in ('bytes','sha256')),
                'actual current generator definition closure differs')
    return body


def guest(frame,api,body,label,output_label,required,definitions,*,provider=None,selection_pins=None):
    """Reopen actual compile/trace/validator and exact JS source embedding."""
    graph=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/direct_modular.py','direct_actual_graph_receipt_contract')
    modular=body.get('kind')==graph.MAIN_KIND
    require((provider in graph.NAMES and body.get('kind')==graph.PROVIDER_KIND and body.get('provider')==provider) or
            (provider is None and body.get('kind') in ('unitree-direct-standalone-build-v1',graph.MAIN_KIND)),
            'actual closed main/provider receipt kind required')
    require(body['format_version']==1 and body['status']=='PASS_BINARY_VALIDATED_NOT_EXECUTED' and
            body['source']==str(frame.source) and body['source_manifest_sha256']==frame.report['source_manifest_sha256'] and
            body['definitions']==definitions and body['full_dependencies_verified'] is True and
            body['default_authority']==[] and body['Runtime_executed'] is False,
            'actual same complete source/definition standalone build required')
    def pin(old):
        require(type(old) is dict and set(old)==PIN and frame.pin(old['path'])==old,'current standalone build input changed')
        return old
    if selection_pins is not None:
        require(type(selection_pins) is dict and set(selection_pins)=={'artifact','receipt'},'two finite metadata-only selected pins required')
        selected_output,selected_receipt=pin(selection_pins['artifact']),pin(selection_pins['receipt'])
        actual_body,actual_receipt=frame.common.document(Path(selected_receipt['path']))
        require(actual_receipt==selected_receipt and actual_body==body,'metadata-only receipt bytes differ')
    else:selected_output,selected_receipt=frame.inputs[output_label],frame.inputs[label]
    pin(body['output']);require(body['output']==selected_output,'actual selected command differs from its build')
    pin(body['producer']);pin(body['build_configuration'])
    require(body['producer']['path']==str(frame.source/'test/0073.ros-tests/support/source/ros_unitree/prepare_direct_guests.py'),
            'maintained actual standalone producer required')
    tools=body['tools'];require(type(tools) is dict and tools,'complete selected standalone tool closure required')
    for value in tools.values():pin(value)
    if 'independent_library_prerequisites' in body:
        inputs_api=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/direct_independent_inputs.py','direct_actual_A1_A2_prerequisite_consumer')
        inputs_api.verify_cpp(frame,api,body['independent_library_prerequisites'],tools)
    generated=current_family(frame,api,body['current_family']) if modular or 'independent_library_prerequisites' in body else None
    sources={value['path']:pin(value) for value in body['source_inputs']}
    for relative in required:
        require(str(frame.source/relative) in sources,'required real Guest/kernel source absent')
    for path,value in sources.items():
        relative=str(Path(path).relative_to(frame.source))
        require(frame.frozen.get(relative)=={key:value[key] for key in ('bytes','sha256')},'Guest source differs from complete MF')
    utilities=api.frozen_module(frame.source,frame.frozen,
        'test/0073.ros-tests/support/source/ros_unitree/acceptance/typed40_boot2/command14-utilities.py','direct_receipt_full_md_consumer')
    compiler_paths={tools[key]['path'] for key in ('cc','cxx')}
    objects=[];compile_dependencies=set();links=[];validators=[];kernel_compiled=False
    for row in body['commands']:
        argv=row['argv'];require(row['returncode']==0 and argv and Path(argv[0]).is_absolute(),'successful actual direct command required')
        cwd=Path(row['working_directory'])
        expected_cwd=Path(body['output']['path']).parent
        if row['role']=='metadata':expected_cwd=expected_cwd.parent
        require(cwd.is_absolute() and cwd.resolve(strict=True)==cwd and cwd==expected_cwd,
                'actual standalone command working directory differs from its product')
        pin(row['stdout']);pin(row['stderr'])
        for value in row['dependencies']:pin(value)
        if row['role']=='compile':
            require(argv[0] in compiler_paths and all(flag in argv for flag in ('-O3','-UNDEBUG','-fno-fast-math','-ffp-contract=off','-MD','-MF','-MT','-c','-o')) and
                    not any(flag in argv for flag in ('-Ofast','-ffast-math','-DNDEBUG','-MMD','-MP')),
                    'actual strict optimized full-MD compile required')
            for key,value in definitions.items():
                require('-D'+key+'='+str(value) in argv,'actual per-TU direct policy macro absent')
            for value in (row['source'],row['object'],row['dependency_file']):pin(value)
            source,obj,dep=(Path(row[key]['path']) for key in ('source','object','dependency_file'))
            require(argv[argv.index('-c')+1]==str(source) and argv[argv.index('-o')+1]==str(obj) and
                    argv[argv.index('-MF')+1]==str(dep) and argv[argv.index('-MT')+1]==str(obj),
                    'actual source/object/depfile association differs')
            actual=utilities.full_md(frame,dep,obj,source,cwd)
            require(actual==row['dependencies'],'actual standalone full MD differs from retained receipt')
            objects.append(row['object']);compile_dependencies.update(value['path'] for value in actual)
            kernel_compiled |= any(Path(value['path']).name=='simulation_control.h' for value in actual)
        elif row['role']=='link':
            require(argv[0] in compiler_paths and '-Wl,--trace' in argv and '-Wl,--threads=1' in argv,
                    'actual bounded Wasm link trace required')
            # The producer records merged logs honestly as one physical file.
            require(row['stdout']==row['stderr'],'actual merged linker capture must remain explicitly merged')
            observed=utilities.trace_inputs(frame,{'log':row['stdout']},cwd)
            require(observed==row['dependencies'] and all(obj in observed for obj in objects),
                    'actual link trace differs or omitted a fresh object')
            links.append(row)
        elif row['role']=='validate':
            require(argv==[tools['validator']['path'],'validate',body['output']['path']], 'actual selected validator required')
            validators.append(row)
        else:
            require(row['role']=='metadata' and body['language']=='javascript' and argv[0]==tools['python']['path'],
                    'only actual embedded-JS source preparation may be metadata')
    require(objects and len(links)==len(validators)==1 and (kernel_compiled or provider in ('callbacks','perception')),
            'actual fresh objects/shared kernel, one final link and full validator required')
    if generated:
        expected=generated['outputs']['bounded_records.'+('hpp' if body['language']=='cpp' else 'h')]
        if body['language']!='javascript':require(expected['path'] in compile_dependencies,'actual main full MD must consume its current regenerated codec')
    embedded=body['embedded']
    if body['language']=='javascript':
        require(embedded and embedded['same_Wasm_local_C_kernel'] is (not(modular and body['positive'])) and embedded['native_bridge'] is False and
                (not modular or (embedded.get('same_Wasm_local_C_adapter') is True and
                 embedded.get('mathematical_kernel_location')==('EXPLICIT_PURE_PRELOADED_MOTION' if body['positive'] else 'UNUSED_DEFAULT_DENIED'))),
                'actual same-Wasm Guest C computation module required')
        for key in ('manifest','preparation','module_table'):pin(embedded[key])
        manifest,_=frame.common.document(Path(embedded['manifest']['path']))
        prepared,_=frame.common.document(Path(embedded['preparation']['path']))
        require(manifest==prepared['manifest'] and manifest['mode']=='command' and manifest['backend']==2 and
                manifest['imports']==manifest['exports']==[] and
                prepared['unitree_control']['direct_enabled']==int(body['positive']) and
                prepared['compiler_executed'] is False and prepared['implicit_permissions']==[],
                'original actual source preparation must remain metadata only')
        associations={}
        for row in embedded['source_associations']:
            source,copy=pin(row['source']),pin(row['copy'])
            require(all(source[key]==copy[key] for key in ('bytes','sha256')) and copy['path'] not in associations,
                    'all staged SDK/generated JS bytes must be exact unique copies')
            associations[copy['path']]=source
        if generated:
            require(generated['outputs']['bounded_records.mjs'] in associations.values(),
                    'actual JS source embedding must use the current regenerated codec')
        entry=Path(body['output']['path']).parents[1]/'source'
        modules=[]
        for row in manifest['modules']:
            path=entry/row['source'];require(str(path) in associations,'embedded module outside actual source-copy ledger')
            raw=api.bounded(path,8*MIB);require(hashlib.sha256(raw).hexdigest()==associations[str(path)]['sha256'],'embedded source copy changed')
            modules.append((row['name'],raw))
        prepare=api.frozen_module(frame.source,frame.frozen,'sdk/origin/javascript/guest/prepare.py','direct_receipt_source_table_generator')
        expected=prepare.module_table(modules,manifest['entry']).encode()
        require(api.bounded(Path(embedded['module_table']['path']),32*MIB)==expected,
                'actual compiled source table does not contain the complete exact JS source bytes')
        for relative in required:
            if relative.endswith('.mjs'):
                require(any(value['path']==str(frame.source/relative) for value in associations.values()),'actual JS main not embedded')
            else:
                copies=[name for name,value in associations.items() if value['path']==str(frame.source/relative)]
                require(copies and any(name in compile_dependencies for name in copies),'actual C kernel/header not consumed by engine compile')
    else:
        require(embedded is None and all(str(frame.source/relative) in compile_dependencies for relative in required),
                'real C/C++ main/kernel sources must occur in actual full MD')
    producer=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/prepare_direct_guests.py','direct_receipt_independent_binary_gate')
    schema,_=frame.common.document(frame.source/'sdk/origin/abi/abi-v1.json')
    actual=(producer.provider_boundary(frame,api,utilities,Path(body['output']['path']),provider) if provider is not None else
            producer.boundary(frame,api,utilities,schema,Path(body['output']['path']),body['positive'],modular=modular))
    require(actual==body['binary_boundary'],'actual standalone binary table changed')
    frame.report.setdefault('direct_Guest_builds',[]).append({'receipt':selected_receipt,
        'language':body['language'],'positive':body['positive'],'provider':provider,'output':body['output'],
        'source_manifest_sha256':body['source_manifest_sha256'],'binary_boundary':actual,'Runtime_executed':False})
    return body


def receipt(frame,api,admission,evidence,label,output_label,required,definitions,*,prepared=None):
    """Hook installed only by the explicit controlled-physics Root runner."""
    body=evidence.selected(label)
    if body.get('kind') in ('unitree-direct-standalone-build-v1','unitree-direct-modular-build-v1'):
        return guest(frame,api,body,label,output_label,required,definitions)
    if body.get('kind')=='unitree-direct-pure-provider-build-v1':
        return guest(frame,api,body,label,output_label,required,definitions,provider=body.get('provider'))
    if body.get('kind')=='humble-three-gateways-and-portable-peer' or body.get('schema')=='uwvm-native74-retained-products-at-rest-v1':
        return native_controller(frame,api,admission,evidence,body,label,output_label,required,definitions,prepared=prepared)
    return None
