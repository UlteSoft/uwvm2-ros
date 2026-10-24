"""Explicit Direct v2 admission for separately completed A1/A2 library inputs.

The v1 Typed14 route remains unchanged. An A1/A2 preparer's original wide root
observations do not become Direct input roots. This consumer reopens only real
finished same-source/current64 library outputs and actual tool/sysroot/MD input
closure. Native execution/physics still requires its independent full Native
and Worker receipts; no source-provider copy authorizes execution or ROS rights.
"""
from __future__ import annotations
import copy
import os
from pathlib import Path

STANDALONE='unitree-direct-go2-standalone-independent-build-selection-v2'
MODULAR='unitree-direct-go2-modular-independent-build-selection-v2'
KINDS=(STANDALONE,MODULAR)
GENERATION='uwvm-unitree-independent-go24-current-generation-admission-v2'


def need(value,message):
    if not value:raise ValueError(message)


def prerequisite(frame,api,admission,selection):
    library=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/library_inputs.py','direct_A1_consumed_library_contract')
    native=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/native_family_inputs.py','direct_A2_consumed_library_contract')
    library.closed(selection,('format_version','kind','tools','paths','cpp_library','native_family_library','generated_go','restart_mappings'),
        'independent Direct selection')
    need(selection['format_version']==2 and selection['kind'] in KINDS and selection['restart_mappings']==[],
        'explicit v2 uses completed current library leaves, never historical root projection')
    compiler=frame.inputs[selection['tools']['cxx']]
    cpp=library.consume_cpp(frame,api,selection['cpp_library'],compiler,Path(selection['paths']['sysroot']))
    family=native.consume(frame,api,selection['native_family_library'],library)
    directory=Path(family['historical_generation']['path']).parent
    need(str(directory)==selection['generated_go'],'explicit selected actual copied GO generation differs')
    frame.direct_regeneration=family
    frame.report['independent_library_prerequisites']={'kind':'uwvm-direct-independent-library-prerequisites-v2',
        'cpp':cpp,'native_family':{key:value for key,value in family.items() if key!='original_generation_body'},
        'Typed14_required':False,'old_root_observations_current_here':False,'original_reports_rewritten':False,
        'Runtime_executed':False,'default_authority':[]}
    evidence=admission.Evidence(api,frame,[])
    return cpp['object'],directory,evidence,cpp


def verify_cpp(frame,api,prerequisite,tools):
    """Reopen the actually consumed A1 object and finite MD/official closure."""
    library=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/library_inputs.py','direct_actual_A1_leaf_consumer')
    need(prerequisite['kind']=='uwvm-direct-independent-library-prerequisites-v2' and
        prerequisite['Typed14_required'] is prerequisite['old_root_observations_current_here'] is
        prerequisite['original_reports_rewritten'] is prerequisite['Runtime_executed'] is False and
        prerequisite['default_authority']==[],'explicit binary-only A1/A2 prerequisite scope required')
    selected=prerequisite['cpp'];pin=selected['prepared_library_report'];library.current(frame,pin)
    class SelectedView:
        def __init__(self,parent):self.parent=parent;self.inputs={'actual-A1-library':pin}
        def __getattr__(self,name):return getattr(self.parent,name)
    actual=library.consume_cpp(SelectedView(frame),api,'actual-A1-library',tools['cxx'],Path(selected['sysroot']))
    need(actual==selected,'actual independently copied CPP object/MD/official source association differs')
    return actual


def verify_generation(frame,api,body):
    """Re-admit actual A2 leaves for a real fresh current-emitter receipt."""
    library=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/library_inputs.py','physics_A1A2_finite_leaf_contract')
    native=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/native_family_inputs.py','physics_A2_finite_provider_contract')
    need(body['kind']==GENERATION,'explicit independent generation v2 required')
    actual=native.consume(frame,api,None,library,selected_pin=body['native_family_library_report'])
    need(all(body[key]==actual[key] for key in ('catalog','historical_generation','original_generation',
        'profile','native_family_library_report','original_native_report','original_source','original_manifest_sha256')),
        'actual completed A2 copied provider lineage differs')
    need(body['original_complete_family']==actual['original_complete_family'],
        'original C52 family row must remain unchanged historical data')
    return actual


def configuration(frame,api,base_config_input,base_physics_input,build_input):
    """Publish actual unexecuted Direct metadata after B really completed.

    The regular CP310 run/profile routes consume the published same schemas.
    No namespace, actor or simulator is started here. A modular B uses the
    existing three-provider adapter; a standalone B retains libraries=[] and
    the original three same-main controllers. Only B's actual minimal consumed
    whole roots are added, never A1/A2's historical preparation-root lists.
    """
    graph=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/direct_modular.py','independent_direct_metadata_graph')
    def doc(label):
        body,pin=frame.common.document(frame.input(label));need(pin==frame.inputs[label],'selected current metadata changed');return body,pin
    base,base_pin=doc(base_config_input);physics,physics_pin=doc(base_physics_input);build,build_pin=doc(build_input)
    if build.get('kind')==graph.BUILD_KIND:
        return graph.configuration(frame,api,base_config_input,base_physics_input,build_input)
    need(build.get('kind')=='unitree-direct-go2-standalone-Guest-builds' and
        build.get('status')=='PASS_SIX_DIRECT_GO2_COMMANDS_COMPILED_VALIDATED_NOT_EXECUTED' and
        build.get('source')==str(frame.source) and build.get('source_manifest_sha256')==frame.report['source_manifest_sha256'] and
        all(build.get(key) is True for key in ('source_preserved','input_pins_preserved','whole_roots_preserved')) and
        not build.get('error') and not build.get('finalization_errors') and
        len(build.get('products',[]))==6 and build.get('providers')==[] and
        build.get('Runtime_executed') is build.get('ROS_DDS_initialized') is build.get('simulator_executed') is False and
        build.get('independent_library_prerequisites',{}).get('kind')=='uwvm-direct-independent-library-prerequisites-v2',
        'actual completed independent six-main standalone B build required')
    need(physics.get('kind')=='unitree-go2-three-direct-physics-v1' and physics.get('expected_network_namespace') is None and
        'libraries' not in physics and base['cgroup']==frame.config['cgroup'] and
        base['benchmark'] is base['profiler'] is None and base['simulation']['controllers'] is
        base['simulation']['direct_runtime'] is None and 'physics_selection' not in base['inputs'],
        'actual unexecuted baseline standalone metadata required')
    cleanup=build.get('cleanup',[]);cleanup=[cleanup] if type(cleanup) is dict else cleanup
    need(cleanup and all(row.get('status')=='PASS' and not row.get('errors') and not row.get('remaining') for row in cleanup),
        'B cleanup must really have completed')
    consumer=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/direct_guest_receipts.py','independent_standalone_receipt_admission')
    config,selected=copy.deepcopy(base),copy.deepcopy(physics);labels={};selected['reports']['guests']='direct-independent-products'
    labels['direct-independent-products']=build_pin;selected['guests']={}
    shared='sdk/origin/robots/unitree/include/roswasm_unitree/simulation_control.h'
    required={'c':['sdk/origin/robots/unitree/examples/direct_go2.c','sdk/origin/robots/unitree/examples/direct_go2_output.cpp',shared],
        'cpp':['sdk/origin/robots/unitree/examples/direct_go2.cpp',shared],
        'javascript':['sdk/origin/robots/unitree/examples/javascript/direct_go2.mjs','sdk/origin/robots/unitree/examples/javascript/control_kernel.c',shared]}
    for language in ('c','cpp','javascript'):
        chosen={}
        for positive,mode in ((False,'denied'),(True,'positive')):
            matches=[row for row in build['products'] if row['language']==language and row['positive'] is positive]
            need(len(matches)==1,'one actual independent language/mode B product required');row=matches[0]
            artifact,receipt=row['artifact'],row['receipt'];need(frame.pin(artifact['path'])==artifact and frame.pin(receipt['path'])==receipt,
                'actual completed B artifact/receipt changed')
            body,actual=frame.common.document(Path(receipt['path']));need(actual==receipt,'B receipt changed while parsing')
            current=consumer.current_family(frame,api,body['current_family'])
            original=base['inputs'][physics['families']['unitree_go']['generation']]
            need({k:current['original_generation'][k] for k in ('path','bytes','sha256')}==original,
                'independent C52 generation differs from selected native physics family')
            definitions={'UWVM_UNITREE_DIRECT_ENABLED':int(positive)}
            if language=='cpp':definitions['UWVM_UNITREE_MODULAR_MOTION']=0
            if language=='javascript':definitions['UWVM_JS_UNITREE_CONTROL']=1
            consumer.guest(frame,api,body,build_input,build_input,required[language],definitions,
                selection_pins={'artifact':artifact,'receipt':receipt})
            label='direct-independent-'+language+'-'+mode
            labels[label]=artifact;labels[label+'-receipt']=receipt;chosen[mode]=label;chosen[mode+'_receipt']=label+'-receipt'
        selected['guests'][language]=chosen
    need(not(set(labels)&set(config['inputs'])),'actual independent labels must not overwrite baseline roles')
    for label,pin in labels.items():config['inputs'][label]={k:pin[k] for k in ('path','bytes','sha256')}
    builder_config,pin=frame.common.document(Path(build['configuration']['path']))
    need(pin==build['configuration'] and builder_config['cgroup']==frame.config['cgroup'],
        'actual completed B configuration/current64 association differs')
    roots=[Path(p) for p in builder_config['input_roots']];root=Path(build_pin['path']).parent
    need(root in frame.roots and all(p in frame.roots for p in roots),'every actual consumed B root must be whole selected current metadata input')
    config['input_roots']=list(dict.fromkeys([*config['input_roots'],*[str(r) for r in roots],str(root)]))
    need(len(config['input_roots'])<=63,'leave one exact CP310 runtime-control root')
    config_path=frame.out/'independent-baseline-config.json';selection_path=frame.out/'independent-physics-unforecast.json'
    frame.common.atomic_json(config_path,config);frame.common.atomic_json(selection_path,selected)
    result={'configuration':frame.pin(config_path),'physics_template':frame.pin(selection_path),'base_configuration':base_pin,
        'base_physics_template':physics_pin,'actual_independent_build':build_pin,'namespace_forecast':False,
        'actors_started':0,'Runtime_executed':False,'image_vision_qualified':False}
    frame.report['independent_metadata_adaptation']=result;frame.save();return result


def stage_configuration(frame,api,admission,request):
    """Real metadata-only B constructor; Root supplies actual completed A1/A2.

    The input Frame already selects only the actual B-consumed complete roots.
    All ten tool/archive labels and four SDK paths are explicit. No discovery
    from an old preparation config and no expansion of its historical roots is
    performed. Normal Frame.finish is required before using this constructor's
    output report. The published argv is literal and remains unexecuted.
    """
    library=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/library_inputs.py','independent_metadata_finite_contract')
    library.closed(request,('format_version','kind','composition','tools','paths','cpp_library','native_family_library','build_out'),
        'independent Direct configuration request')
    need(request['format_version']==2 and request['kind']=='uwvm-direct-independent-build-configuration-request-v2' and
        request['composition'] in ('standalone','modular') and len(frame.roots)<=63 and
        frame.limits['jobs']==1 and frame.limits['rss_bytes']<=2*1024**3 and frame.limits['command_seconds']<=600 and
        frame.config['simulation'] is frame.config['benchmark'] is frame.config['profiler'] is None,
        'current compiler-free independent constructor with one controls-root slot required')
    library.closed(request['tools'],('cc','cxx','python','validator','libc','libm','printscan','builtins','libcxx','libcxxabi'),'explicit ten actual tool/archive labels')
    library.closed(request['paths'],('sysroot','resource_dir','wasm_ld_alias','quickjs'),'explicit complete SDK paths')
    for label in request['tools'].values():library.current(frame,frame.inputs[label])
    for name in ('sysroot','resource_dir','quickjs'):
        path=Path(request['paths'][name]);need(path.is_absolute() and path.resolve(strict=True)==path and path.is_dir() and
            any(path==r or path.is_relative_to(r) for r in frame.roots),'whole actual selected SDK directory required')
    alias=Path(request['paths']['wasm_ld_alias']);need(alias.is_absolute() and alias.name.startswith('wasm-ld') and
        alias.parent.resolve(strict=True)==alias.parent,'explicit physical Wasm linker spelling required')
    library.selected(frame,alias.resolve(strict=True))
    raw,pin=frame.common.document(frame.input(request['native_family_library']))
    need(pin==frame.inputs[request['native_family_library']],'selected A2 report changed')
    # This spelling is read only as a prospective argument. prerequisite() below
    # independently proves its actual terminal A2 and all finite copied leaves.
    generated=str(Path(raw['library']['generation']['path']).parent)
    selection={'format_version':2,'kind':MODULAR if request['composition']=='modular' else STANDALONE,
        'tools':request['tools'],'paths':request['paths'],'cpp_library':request['cpp_library'],
        'native_family_library':request['native_family_library'],'generated_go':generated,'restart_mappings':[]}
    obj,directory,evidence,cpp=prerequisite(frame,api,admission,selection)
    need(str(directory)==generated and cpp['compiler']['physical']==frame.inputs[request['tools']['cxx']]['path'] and
        cpp['sysroot']==request['paths']['sysroot'],'actual copied official object/compiler/sysroot association differs')
    for key,name in (('libc','libc.a'),('libm','libm.a'),('printscan','libc-printscan-long-double.a')):
        need(Path(frame.inputs[request['tools'][key]]['path'])==
            (Path(request['paths']['sysroot'])/'lib/wasm32-wasip1'/name).resolve(strict=True),
            'actual matching WASI SDK archive required: '+key)
    preparer=api.frozen_module(frame.source,frame.frozen,
        'test/0073.ros-tests/support/source/ros_unitree/prepare_direct_guests.py','independent_direct_explicit_archive_inputs')
    cpp_archives=preparer.selected_cpp_archives(frame,api,Path(request['paths']['sysroot']),
        {key:request['tools'][key] for key in ('libcxx','libcxxabi')})
    output=Path(request['build_out'])
    need(output.is_absolute() and not output.exists() and not output.is_symlink() and output.parent.resolve(strict=True)==output.parent and
        all(not(output==p or output.is_relative_to(p) or p.is_relative_to(output)) for p in [frame.source,frame.out,*frame.roots]),
        'fresh disjoint actual owned B build output required')
    controls=frame.out/'build-controls';controls.mkdir(mode=0o700)
    selection_path=controls/'selection.json';frame.common.atomic_json(selection_path,selection)
    selection_path.chmod(0o400);selection_pin=frame.pin(selection_path)
    inputs=dict(frame.config['inputs']);need('independent-direct-selection' not in inputs,'fresh selection input label required')
    inputs['independent-direct-selection']={k:selection_pin[k] for k in ('path','bytes','sha256')}
    config={**frame.config,'input_roots':[*[str(r) for r in frame.roots],str(frame.out)],'inputs':inputs}
    need(len(config['input_roots'])<=64,'actual selected B roots must fit unchanged64-root limit')
    config_path=controls/'frame-config.json';frame.common.atomic_json(config_path,config);config_path.chmod(0o400)
    config_pin=frame.pin(config_path)
    argv=[frame.inputs[request['tools']['python']]['path'],'-B',str(frame.source/'test/0073.ros-tests/support/source/ros_unitree/prepare_direct_guests.py'),
        '--source',str(frame.source),'--manifest',str(frame.manifest),'--manifest-sha256',frame.report['source_manifest_sha256'],
        '--config',config_pin['path'],'--config-sha256',config_pin['sha256'],'--selection-input','independent-direct-selection','--out',str(output)]
    invocation_path=controls/'invocation.json';frame.common.atomic_json(invocation_path,{'argv':argv,'execution_performed':False})
    invocation_path.chmod(0o400);invocation=frame.pin(invocation_path)
    descriptor=os.open(controls,os.O_RDONLY|os.O_DIRECTORY|os.O_NOFOLLOW)
    try:
        os.fsync(descriptor);os.fchmod(descriptor,0o500);os.fsync(descriptor)
    finally:os.close(descriptor)
    need(not frame.actors and not output.exists(),'metadata constructor cannot start a compiler or create build output')
    frame.report.update(derived_selection=selection_pin,derived_configuration=config_pin,future_invocation=invocation,
        observed_cpp_object=obj,observed_actual_A2_generation=frame.pin(directory/'generation.json'),
        observed_selected_cpp_archives=cpp_archives,
        metadata_only=True,actors_started=0,compilation_performed=False,Runtime_executed=False,default_authority=[],
        original_reports_rewritten=False,original_preparation_roots_projected=False,cleanup=[])
    return 'PASS_DIRECT_INDEPENDENT_BUILD_CONFIGURATION_REAL_A1_A2_ADMITTED_NOT_COMPILED'
