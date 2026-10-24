#!/usr/bin/env python3
"""Closed Direct3 module graph predicates; importing this module runs no actor.

The graph composes main -> perception and main -> motion -> callbacks. This
module only checks DATA and real binary tables supplied by an admitted caller.
It never grants a right, guesses an artifact, changes a historical report or
claims compilation/execution. The original standalone selection remains valid.
"""
NAMES = ('callbacks', 'motion', 'perception')
BUILD_SELECTION = 'unitree-direct-go2-modular-build-selection-v1'
PHYSICS_SELECTION = 'unitree-go2-three-modular-direct-physics-v1'
BUILD_KIND = 'unitree-direct-go2-modular-Guest-builds'
BUILD_STATUS = 'PASS_SIX_DIRECT_GO2_MODULAR_COMMANDS_AND_THREE_PRELOADS_COMPILED_VALIDATED_NOT_EXECUTED'
PHYSICS_KIND = 'unitree-three-modular-direct-Guest-controlled-physics'
PHYSICS_STATUS = 'PASS_THREE_ACTUAL_MODULAR_GUEST_NATIVE_DDS_MUJOCO_CONTROL_CASES'
MAIN_KIND = 'unitree-direct-modular-build-v1'
PROVIDER_KIND = 'unitree-direct-pure-provider-build-v1'
ROOT_SOURCES = ('sdk/origin/examples/robotics_modular/unitree_modular_control.h',
                'sdk/origin/examples/robotics_modular/unitree_motion_abi.h',
                'sdk/origin/examples/robotics_modular/unitree_perception_abi.h')
SOURCES = {
    'callbacks': ('sdk/origin/examples/robotics_modular/unitree_callback.c', 'sdk/origin/examples/robotics_modular/unitree_motion_abi.h'),
    'motion': ('sdk/origin/examples/robotics_modular/unitree_motion.c', 'sdk/origin/examples/robotics_modular/unitree_motion_abi.h',
               'sdk/origin/robots/unitree/include/roswasm_unitree/simulation_control.h'),
    'perception': ('sdk/origin/examples/robotics_modular/unitree_perception.c',
                   'sdk/origin/examples/robotics_modular/unitree_perception_abi.h', 'sdk/origin/examples/robotics_modular/unitree_motion_abi.h')}
def sig(count): return (['i32'] * count, ['i32'])
EXPORTS = {
    'callbacks': {'joint_begin': sig(3), 'on_joint': sig(10), 'joint_end': sig(5), 'joint_abort': sig(2)},
    'motion': {'motion_begin': sig(2), 'motion_status': sig(0), 'motion_word': sig(3), 'motion_end': sig(1)},
    'perception': {'perception_begin': sig(1), 'perception_status': sig(0), 'perception_word': sig(3),
                   'perception_end': sig(2), 'perception_abort': sig(1)}}
IMPORTS = {name: {} for name in NAMES}
IMPORTS['motion'] = {('callbacks', name): value for name, value in EXPORTS['callbacks'].items()}
MAIN_IMPORTS = {(module, name): value for module in ('motion', 'perception') for name, value in EXPORTS[module].items()}

def require(value, message):
    if not value: raise ValueError(message)

def libraries(value):
    """Accept only the exact ordered named graph and six distinct input labels."""
    require(type(value) is list and len(value) == len(NAMES), 'exact three-provider graph required')
    labels = []
    for row, name in zip(value, NAMES):
        require(type(row) is dict and set(row) == {'name', 'guest_input', 'build_input'} and row['name'] == name,
                'provider order/name/shape differs')
        for key in ('guest_input', 'build_input'):
            label = row[key]
            require(type(label) is str and 0 < len(label) <= 128 and
                    all(c.isascii() and (c.isalnum() or c in '._-') for c in label), 'explicit safe provider input label required')
            labels.append(label)
    require(len(set(labels)) == len(labels), 'provider artifact/receipt labels must be distinct')
    return value

def binary(table, name):
    """Require actual pure scalar ABI and one independent two-page memory.

    An imported table/memory/global, start section, unlisted function, wrong
    scalar signature or extra exported function fails; no WASI/ROS import is
    admitted. Memory is private, wasm32 and nonshared, even for callbacks.
    """
    require(name in NAMES, 'closed provider name required')
    got = {}
    for row in table['imports']:
        key = row['module'], row['name']
        require(row['kind'] == 0 and key not in got, 'unique scalar function imports only')
        got[key] = row['params'], row['results']
    require(got == IMPORTS[name], 'actual provider import graph/signatures differ')
    require(table['start_function'] is None and len(table['memories']) == 1, 'one memory and no implicit provider start required')
    memory = table['memories'][0]
    require(memory == {'imported': False, 'minimum': 2, 'maximum': 2}, 'private bounded two-page wasm32 memory required')
    actual = {key: (row.get('params'), row.get('results')) for key, row in table['exports'].items() if row['kind'] == 0}
    require(actual == EXPORTS[name], 'actual provider export/signature set differs')
    require(set(table['exports']) == set(EXPORTS[name]) | {'memory'} and
            table['exports']['memory']['kind'] == 2 and table['exports']['memory']['index'] == 0,
            'provider exports only its scalar functions and owned memory0')
    return {'name': name, 'imports': table['imports'], 'exports': table['exports'], 'memories': table['memories'],
            'implicit_start': False, 'default_authority': [], 'Runtime_executed': False}

def main_imports(rows, enabled):
    """Return only graph imports after exact scalar-signature admission.

    Denied mains have no provider imports and therefore cannot even initialize
    or call a provider while verifying denial. Main's ROS/WASI checks remain in
    the ordinary independent command boundary validator.
    """
    actual = {}
    for row in rows:
        if row['module'] in NAMES:
            key = row['module'], row['name']
            require(row['kind'] == 0 and key not in actual, 'unique scalar main graph imports required')
            actual[key] = row['params'], row['results']
    require(actual == (MAIN_IMPORTS if enabled else {}), 'actual main composition imports differ')
    return actual


def generation_equivalence(old, fresh, generator_sha256, bounded_generator_sha256):
    """Admit an actual regenerated message-only GO24 source revision.

    Changed emitter hashes are explicit, while ALL eleven produced body
    digests, native identities, codec identities and original definition/catalog
    pins must equal the previously compiled native provider generation. The
    caller must actually run the current emitters and reopen every output.
    This DATA predicate neither performs nor claims that execution itself.
    """
    names={'bounded_records.'+suffix for suffix in ('h','hpp','rs','go','ts','mjs')} | {
        'bounded_interfaces.h','bounded_interfaces.hpp','manifest.hpp','native_ros1.hpp','native_ros2.hpp'}
    require(old.get('status')==fresh.get('status')=='GENERATED_SOURCE_ONLY' and
            fresh.get('format')==old.get('format')=='uwvm-ros-custom-bounded-v2' and
            fresh.get('native_registry_emitted') is old.get('native_registry_emitted') is True and
            fresh.get('implicit_permissions')==old.get('implicit_permissions')==[] and
            fresh.get('generator_sha256')==generator_sha256 and fresh.get('bounded_generator_sha256')==bounded_generator_sha256 and
            len(fresh['records'])==len(fresh['types'])==24 and set(fresh['outputs'])==names==set(old['outputs']) and
            fresh['records']==old['records'] and fresh['types']==old['types'] and fresh['source_inputs']==old['source_inputs'] and
            fresh['outputs']==old['outputs'],
            'actual current generation must preserve every native GO24 identity/codec and all11 compiled output bodies')
    return names


def configuration(frame, api, base_config_input, base_physics_input, build_input):
    """Publish fresh unexecuted modular metadata from actual current inputs.

    Root first creates an admitted metadata-only Frame with the base config,
    its unforecast physics template, and a completed real modular build report.
    Every actual command/receipt/provider must fit that Frame's selected whole
    roots. This routine starts no actor or namespace. It returns two real
    published full7 pins for the unchanged CP310 stage --phase run protocol.
    Root supplies fresh owned paths through Frame.out, never future identities.
    """
    from pathlib import Path
    import copy
    def document(label):
        body,pin=frame.common.document(frame.input(label))
        require(pin==frame.inputs[label], 'selected current modular metadata input changed')
        return body,pin
    base,base_pin=document(base_config_input);physics,physics_pin=document(base_physics_input)
    build,build_pin=document(build_input)
    require(physics.get('kind')=='unitree-go2-three-direct-physics-v1' and
            physics.get('expected_network_namespace') is None and 'libraries' not in physics and
            base['cgroup']==frame.config['cgroup'] and base['benchmark'] is base['profiler'] is None and
            base['simulation']['controllers'] is base['simulation']['direct_runtime'] is None and
            'physics_selection' not in base['inputs'], 'unexecuted standalone baseline metadata required')
    require(build.get('kind')==BUILD_KIND and build.get('status')==BUILD_STATUS and
            build.get('source')==str(frame.source) and build.get('source_manifest_sha256')==frame.report['source_manifest_sha256'] and
            all(build.get(key) is True for key in ('source_preserved','input_pins_preserved','whole_roots_preserved')) and
            not build.get('error') and not build.get('errors') and not build.get('finalization_errors') and
            build.get('Runtime_executed') is build.get('ROS_DDS_initialized') is build.get('simulator_executed') is False and
            len(build.get('products',[]))==6 and len(build.get('providers',[]))==3,
            'actual six-command three-provider binary-only completed build required')
    def observe(pin):
        require(type(pin) is dict and set(pin)=={'path','bytes','sha256','device','inode','mtime_ns','ctime_ns'} and
                frame.pin(pin['path'])==pin, 'current actual build artifact changed')
        return pin
    cleanup=build.get('cleanup',[]);cleanup=[cleanup] if type(cleanup) is dict else cleanup
    require(cleanup and all(row.get('status')=='PASS' and not row.get('errors') and not row.get('remaining') for row in cleanup),
            'actual producer cleanup required before metadata adaptation')
    config,selected=copy.deepcopy(base),copy.deepcopy(physics);selected['kind']=PHYSICS_SELECTION
    consumer=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/direct_guest_receipts.py',
                               'direct_modular_metadata_real_receipt_admission')
    labels={};selected['reports']['guests']='direct-modular-products';labels['direct-modular-products']=build_pin
    selected['guests']={}
    for language in ('c','cpp','javascript'):
        chosen={}
        for positive,mode in ((False,'denied'),(True,'positive')):
            matches=[row for row in build['products'] if row.get('language')==language and row.get('positive') is positive]
            require(len(matches)==1, 'exact actual modular language/mode product required')
            row=matches[0];artifact,receipt=observe(row['artifact']),observe(row['receipt'])
            body,actual=frame.common.document(Path(receipt['path']));require(actual==receipt and body.get('kind')==MAIN_KIND and
                body.get('output')==artifact and body.get('positive') is positive and body.get('language')==language and
                body.get('source')==str(frame.source) and body.get('source_manifest_sha256')==frame.report['source_manifest_sha256'] and
                body.get('composition',{}).get('providers')==build['providers'], 'actual modular command receipt association differs')
            current=consumer.current_family(frame,api,body['current_family'])
            original=base['inputs'][physics['families']['unitree_go']['generation']]
            require({key:current.get('original_generation',current['historical_generation'])[key] for key in ('path','bytes','sha256')}==original,
                    'current generation differs from the selected original native family')
            shared='sdk/origin/robots/unitree/include/roswasm_unitree/simulation_control.h'
            required=({'c':['sdk/origin/robots/unitree/examples/direct_go2.c','sdk/origin/robots/unitree/examples/direct_go2_output.cpp',shared],
                'cpp':['sdk/origin/robots/unitree/examples/direct_go2.cpp',shared],
                'javascript':['sdk/origin/robots/unitree/examples/javascript/direct_go2.mjs',
                              'sdk/origin/robots/unitree/examples/javascript/control_kernel.c',shared]}[language])
            if positive:required=[*required,*ROOT_SOURCES]
            definitions={'UWVM_UNITREE_DIRECT_ENABLED':int(positive),'UWVM_UNITREE_MODULAR_MOTION':int(positive),
                         'UWVM_UNITREE_MODULAR_PERCEPTION':int(positive)}
            if language=='javascript':definitions['UWVM_JS_UNITREE_CONTROL']=1
            # The metadata-only consumer accepts these exact already opened
            # pins. It neither mutates Frame.inputs nor creates a Runtime label
            # or widens complete-root containment.
            consumer.guest(frame,api,body,build_input,build_input,required,definitions,
                           selection_pins={'artifact':artifact,'receipt':receipt})
            label='direct-modular-'+language+'-'+mode;chosen[mode]=label;chosen[mode+'_receipt']=label+'-receipt'
            labels[label]=artifact;labels[label+'-receipt']=receipt
        selected['guests'][language]=chosen
    selected['libraries']=[]
    for name,row in zip(NAMES,build['providers']):
        require(row.get('name')==name, 'exact actual provider order required')
        artifact,receipt=observe(row['artifact']),observe(row['receipt'])
        body,actual=frame.common.document(Path(receipt['path']))
        require(actual==receipt and body.get('kind')==PROVIDER_KIND and body.get('provider')==name and
                body.get('output')==artifact, 'actual pure provider receipt association differs')
        consumer.guest(frame,api,body,build_input,build_input,SOURCES[name],{},provider=name,
                       selection_pins={'artifact':artifact,'receipt':receipt})
        label='direct-modular-'+name;labels[label]=artifact;labels[label+'-receipt']=receipt
        selected['libraries'].append({'name':name,'guest_input':label,'build_input':label+'-receipt'})
    libraries(selected['libraries'])
    require(not set(labels)&set(config['inputs']), 'modular metadata cannot overwrite an existing input label')
    for label,pin in labels.items():config['inputs'][label]={key:pin[key] for key in ('path','bytes','sha256')}
    # Retain complete old scopes and the explicitly admitted complete producer
    # root. Never derive a wider parent from an individual product pathname.
    root=Path(build_pin['path']).parent
    require(root in frame.roots, 'complete actual modular producer root must be explicitly selected')
    consumed=[]
    if build.get('independent_library_prerequisites',{}).get('kind')=='uwvm-direct-independent-library-prerequisites-v2':
        build_config,actual=frame.common.document(Path(build['configuration']['path']))
        require(actual==build['configuration'] and build_config['cgroup']==frame.config['cgroup'],
            'actual completed independent build configuration/current64 association differs')
        # Only actual v2 B roots are consumed here. A1/A2 historical preparation
        # roots are not B roots and are never projected into this configuration.
        consumed=[Path(value) for value in build_config['input_roots']]
        require(all(value in frame.roots for value in consumed), 'actual consumed v2 B roots must remain whole selected inputs')
    config['input_roots']=list(dict.fromkeys([*config['input_roots'],*[str(value) for value in consumed],str(root)]))
    require(len(config['input_roots'])<=63, 'leave one exact CP310 runtime-control root')
    configuration=frame.out/'modular-baseline-config.json';selection=frame.out/'modular-physics-unforecast.json'
    frame.common.atomic_json(configuration,config);frame.common.atomic_json(selection,selected)
    result={'configuration':frame.pin(configuration),'physics_template':frame.pin(selection),
            'base_configuration':base_pin,'base_physics_template':physics_pin,'actual_modular_build':build_pin,
            'namespace_forecast':False,'actors_started':0,'Runtime_executed':False,'image_vision_qualified':False}
    frame.report['modular_metadata_adaptation']=result;frame.save()
    return result
