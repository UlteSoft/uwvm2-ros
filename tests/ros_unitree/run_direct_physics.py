#!/usr/bin/env python3
"""Own three real Go2 Guest/native DDS/MuJoCo control comparisons.

The private namespace/COPY owner stages every actual input before invoking
this host-Python coordinator. Only simulator and Python state witnesses enter
the admitted CP310 chroot. The host-linked interpreter, Gateway and ordinary
native C++ controller retain their actual executable/loader/library identities.
No compiler, downloader, physical robot endpoint or permission service is
started here. Default Guest ROS/WASI authority is empty except basic stdio.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import time
import types

MIB=1024**2
LANGUAGES=('c','cpp','javascript')
sys.dont_write_bytecode = True


def require(value,message):
    if not value:raise ValueError(message)


def bootstrap(args):
    """Bind the real sibling loader to the manifest before its code executes."""
    root = Path(__file__).resolve().parents[2]
    require(args.source.resolve(strict=True) == root, 'invoke the actual complete frozen physics source')
    def bounded(path, maximum):
        path=Path(path)
        require(path.is_absolute() and path.resolve(strict=True)==path, 'physical absolute first-loader input required')
        fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
        try:
            first=os.fstat(fd)
            require(stat.S_ISREG(first.st_mode) and 0<first.st_size<=maximum, 'bounded ordinary first-loader input required')
            body=bytearray()
            while True:
                block=os.read(fd,min(65536,maximum+1-len(body)))
                if not block:break
                body.extend(block);require(len(body)<=maximum, 'first-loader input grew')
            last,named=os.fstat(fd),path.stat(follow_symlinks=False)
            require(len(body)==first.st_size and all(getattr(first,key)==getattr(last,key)==getattr(named,key)
                    for key in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),
                    'first-loader input changed during bounded read')
            return bytes(body)
        finally:os.close(fd)
    raw=bounded(args.manifest,16*MIB)
    require(hashlib.sha256(raw).hexdigest()==args.manifest_sha256, 'explicit complete source manifest hash differs')
    def pairs(items):
        result={}
        for key,value in items:
            require(key not in result, 'duplicate first-loader manifest key');result[key]=value
        return result
    frozen=json.loads(raw,object_pairs_hook=pairs)
    sibling=None
    for relative in ('tests/ros_unitree/run_direct_physics.py','tests/ros_unitree/prepare_direct_guests.py'):
        body=bounded(root/relative,MIB)
        require(frozen.get(relative)=={'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()},
                'actual coordinator/first sibling absent from complete frozen manifest')
        if relative.endswith('prepare_direct_guests.py'):sibling=body
    module=types.ModuleType('direct_physics_manifest_bound_first_loader')
    module.__file__=str(root/'tests/ros_unitree/prepare_direct_guests.py')
    sys.modules[module.__name__]=module
    exec(compile(sibling,module.__file__,'exec'),module.__dict__)
    return module.bootstrap(args)


def policy(chosen,positive,records):
    """Exactly two independently native-bound simulator aliases, no wildcard."""
    connection={'node-name':'unitree_direct_gateway','namespace':'/','domain-id':'1',
        'rmw-implementation':'rmw_cyclonedds_cpp','security-mode':'disabled-test',
        'keystore':'not-applicable','enclave':'not-applicable'}
    if not positive:return connection,[]
    grants=[]
    for alias,operation,resource,name in (
            ('unitree-lowcmd','topic.publish','/lowcmd','unitree_go/msg/LowCmd'),
            ('unitree-lowstate','topic.subscribe','/lowstate','unitree_go/msg/LowState')):
        capacity=records[name]['maximum_wire_bytes']
        require(type(capacity) is int and 0<capacity<=32768,'actual bounded native GO command/state capacity required')
        codec=chosen['codec_identities'][name]
        require(records[name]['native_identity']==chosen['identities'][name] and
                records[name]['adapter_codec_identity']==codec,
                'actual native GO record and compiled adapter codec association differs')
        # The complete32-step caller-paced fixture is bounded even if the host
        # schedules several gates closely. This is an explicit test grant,
        # never an ambient/default capability or a physical control deadline.
        limits={'adapter-codec-identity':codec,'max-message-bytes':str(capacity),'max-rate-hz':'10000','burst':'32',
            'max-bytes-per-second':str(capacity*10000),'max-queue-bytes':str(capacity),'max-inflight':'1',
            'overflow-policy':'reject','validator':'none','audit':'allow-and-deny','audit-max-rate-hz':'100',
            'audit-burst':'16','audit-max-records':'10000','audit-max-bytes':'1048576',
            'max-publishers' if operation=='topic.publish' else 'max-subscribers':'1',
            'qos.reliability':'reliable','qos.durability':'volatile','qos.history':'keep-last','qos.depth':'1',
            'qos.deadline':'infinite','qos.lifespan':'infinite','qos.liveliness':'automatic',
            'qos.liveliness-lease-duration':'infinite'}
        grants.append({'alias':alias,'operation':operation,'resource':resource,'type':name,
                       'identity':chosen['identities'][name],'limits':limits})
    return connection,grants


def prepare(frame,api,unitree,selection,*,profiling=False):
    """Close actual file/source/namespace contracts before any ROS actor."""
    graph=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/direct_modular.py','direct_physics_actual_module_graph')
    modular=selection.get('kind')==graph.PHYSICS_SELECTION
    unitree.exact(selection,('format_version','kind','reports','workers','families','native_peer','exec_gate',
        'worker_catalog_roots','restart_mappings','cyclonedds_xml','expected_network_namespace','guests',
        'native_controller','mappings','cp310','measurement',*(['libraries'] if modular else [])),'controlled direct-physics selection')
    require(selection['format_version']==1 and (modular or selection['kind']=='unitree-go2-three-direct-physics-v1'),
            'actual closed three-language physical control selection required')
    unitree.exact(selection['reports'],('workers','native_products','native_interfaces','guests'),'four actual producer reports')
    unitree.exact(selection['guests'],LANGUAGES,'all three real direct Guest languages')
    unitree.exact(selection['mappings'],('worker','gateway','gateway_context','native_controller',
        'native_controller_context','physics_python','witness_python'),'actual selected live mapping roles')
    unitree.exact(selection['cp310'],('root','python','chroot','catalog_roots','environment','view_receipt'),'actual prepared CP310 selection')
    unitree.exact(selection['measurement'],('steps','warmup_steps','slowdown_trigger'),'explicit matched control count/timing choice')
    measure=selection['measurement']
    require(type(measure['steps']) is int and 1<=measure['steps']<=32 and
            type(measure['warmup_steps']) is int and 0<=measure['warmup_steps']<=2 and
            measure['warmup_steps']<measure['steps'] and type(measure['slowdown_trigger']) in (int,float) and
            1<=measure['slowdown_trigger']<=10,'finite caller-selected control measurements required')
    # Baseline remains profiler=None. Only the explicit source-bound Direct3
    # hardware entry may opt in; it separately binds a terminal measured case.
    require(type(profiling) is bool and frame.config['benchmark'] is None and
            ((not profiling and frame.config['profiler'] is None) or
             (profiling and frame.report['kind']=='unitree-direct3-vtune-hardware-profiles' and
              type(frame.config['profiler']) is dict and 'direct3' in frame.config['profiler'])) and
            frame.config['simulation']['controllers'] is frame.config['simulation']['direct_runtime'] is None,
            'immutable baseline model map and explicit profile-only opt-in required')
    admission=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/typed_transport_inputs.py','direct_physics_original_producer_admission')
    native=api.frozen_module(frame.source,frame.frozen,'tests/ros_javascript_guest/native.py','direct_physics_live_map_and_gate')
    session=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/typed_transport_session.py','direct_physics_logged_lines_and_Runtime_policy')
    receipt=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/direct_guest_receipts.py','direct_physics_build_receipts')
    # run_simulation uses ordinary sibling imports. Make only this admitted
    # original Unitree helper visible under its expected name, and bind all
    # siblings before evaluating it. No module import starts an actor.
    for relative,name in (('tests/ros_unitree/common.py','common'),('tests/ros_unitree/messages.py','messages'),
                          ('tests/ros_unitree/benchmark.py','benchmark')):
        api.frozen_module(frame.source,frame.frozen,relative,name)
    simulation=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/run_simulation.py','direct_physics_actual_callback_and_step_protocol')
    evidence=admission.Evidence(api,frame,selection['restart_mappings'])
    namespace=admission.environment(frame,api,selection)
    workers=admission.runtime_profiles(frame,evidence,selection)
    prepared=admission.native_products(frame,api,native,evidence,selection)
    families=admission.family_bindings(frame,api,evidence,selection,workers,prepared)
    chosen=families['unitree_go']
    require(chosen['worker_profile']=='ros2' and chosen['native_profile']=='go-api',
            'actual GO+API registry Worker/Gateway selection required')
    worker_files,worker_elfs=admission.host_catalog(frame,api,selection['worker_catalog_roots'])
    cp=selection['cp310'];view_root=Path(cp['root'])
    require(view_root.is_absolute() and view_root.resolve(strict=True)==view_root and view_root.is_dir(),
            'actual independently prepared CP310 view root required')
    python=frame.input(cp['python']);chroot=frame.input(cp['chroot'])
    require(python==view_root/'usr/bin/python3.10' and frame.config['simulation']['python_input']==cp['python'],
            'actual CP310 COPY interpreter must match the selected simulator input')
    view=evidence.selected(cp['view_receipt'])
    require(view.get('kind')=='unitree-cp310-controlled-physics-view' and
            view.get('status')=='PREPARED_CONTENT_ONLY_NOT_EXECUTED' and view.get('root')==str(view_root) and
            view.get('python')==frame.inputs[cp['python']] and view.get('source_preserved') is True and
            view.get('inputs_preserved') is True and view.get('products_preserved') is True and
            not view.get('error') and not view.get('errors'),'actual fresh COPY preparation receipt required')
    cp_files,cp_elfs=admission.host_catalog(frame,api,cp['catalog_roots'])
    environment=cp['environment']
    allowed_environment={'PATH','LD_LIBRARY_PATH','PYTHONPATH','AMENT_PREFIX_PATH','CMAKE_PREFIX_PATH',
        'ROS_DOMAIN_ID','RMW_IMPLEMENTATION','CYCLONEDDS_URI','ROS_LOCALHOST_ONLY','ROS_SECURITY_ENABLE',
        'MUJOCO_GL','LANG','LC_ALL','HOME','TMPDIR','XDG_CACHE_HOME','XDG_CONFIG_HOME','PYTHONDONTWRITEBYTECODE','PYTHONNOUSERSITE',
        'OMP_NUM_THREADS','OPENBLAS_NUM_THREADS','MKL_NUM_THREADS','NUMEXPR_NUM_THREADS'}
    require(type(environment) is dict and environment and environment.get('ROS_DOMAIN_ID')=='1' and
            set(environment)<=allowed_environment and
            environment.get('RMW_IMPLEMENTATION')=='rmw_cyclonedds_cpp' and
            environment.get('CYCLONEDDS_URI')=='file://'+str(frame.input(selection['cyclonedds_xml'])) and
            environment.get('PYTHONDONTWRITEBYTECODE')=='1' and environment.get('MUJOCO_GL')=='osmesa',
            'actual explicit CP310 ROS2/Cyclone/OSMesa environment required')
    require(all(type(key) is str and type(value) is str and not any(ch in value for ch in '\0\r\n')
                for key,value in environment.items()),'clean explicit CP310 environment required')
    require(environment.get('ROS_SECURITY_ENABLE','false')=='false' and
            all(environment.get(key,'1')=='1' for key in ('OMP_NUM_THREADS','OPENBLAS_NUM_THREADS','MKL_NUM_THREADS','NUMEXPR_NUM_THREADS')),
            'explicit nonphysical security-disabled test and serial native math libraries required')
    for key in ('HOME','TMPDIR','XDG_CACHE_HOME','XDG_CONFIG_HOME'):
        require(key in environment and Path(environment[key]).is_absolute() and
                Path(environment[key]).is_relative_to(frame.out), 'CP310 mutable paths must stay in the actual owned run output')
    products=evidence.selected(selection['reports']['guests'])
    admission.clean_build(products,graph.BUILD_STATUS if modular else 'PASS_SIX_DIRECT_GO2_COMMANDS_COMPILED_VALIDATED_NOT_EXECUTED',
        ('source_preserved','input_pins_preserved','whole_roots_preserved'))
    require(products['kind']==(graph.BUILD_KIND if modular else 'unitree-direct-go2-standalone-Guest-builds') and len(products['products'])==6 and
            products['Runtime_executed'] is False and products['ROS_DDS_initialized'] is False,
            'actual six standalone binary-only products required')
    source,frozen=evidence.source(products)
    require(source==frame.source and products['source_manifest_sha256']==frame.report['source_manifest_sha256'],
            'direct Guests require their actual complete current source, never an old relabeled build')
    binaries={}
    for language in LANGUAGES:
        pick=selection['guests'][language]
        unitree.exact(pick,('positive','positive_receipt','denied','denied_receipt'),'actual distinct command/receipt labels')
        for positive,key in ((False,'denied'),(True,'positive')):
            rows=[row for row in products['products'] if row['language']==language and row['positive'] is positive]
            require(len(rows)==1 and evidence.pin(rows[0]['artifact'])==frame.inputs[pick[key]] and
                    evidence.pin(rows[0]['receipt'])==frame.inputs[pick[key+'_receipt']], 'actual standalone product association differs')
            body=evidence.selected(pick[key+'_receipt'])
            require(body['language']==language and body['positive'] is positive,'actual compiled language/mode differs')
            if modular:
                current=receipt.current_family(frame,api,body['current_family'])
                require(current['historical_generation']==frame.inputs[selection['families']['unitree_go']['generation']],
                        'fresh current emitters must retain this exact independently admitted native GO generation')
                require(body.get('composition',{}).get('providers')==products.get('providers') and
                        body['composition'].get('image_vision_qualified') is False and body['composition'].get('main_only_ROS') is True,
                        'actual main receipt must bind this exact complete provider build batch')
            binaries[language,key]={'artifact':frame.inputs[pick[key]],'boundary':{'local_entry':body['binary_boundary']['local_defined_entry_index']}}
            # mmap of a selected Wasm is an ordinary file mapping. Add exactly
            # these six admitted artifacts, never arbitrary map-display paths.
            if frame.inputs[pick[key]] not in worker_files:worker_files.append(frame.inputs[pick[key]])
    libraries=graph.libraries(selection['libraries']) if modular else []
    if modular:
        require(len(products.get('providers',[]))==3,'three actual newly compiled pure providers required')
        for item,row in zip(libraries,products['providers']):
            name=item['name']
            require(row['name']==name and evidence.pin(row['artifact'])==frame.inputs[item['guest_input']] and
                    evidence.pin(row['receipt'])==frame.inputs[item['build_input']], 'actual pure provider product association differs')
            receipt.guest(frame,api,evidence.selected(item['build_input']),item['build_input'],item['guest_input'],
                          graph.SOURCES[name],{},provider=name)
            if frame.inputs[item['guest_input']] not in worker_files:worker_files.append(frame.inputs[item['guest_input']])
        # run_simulation is also used by older generic callers. Only this
        # source-bound owner installs the exact three-provider admission hook.
        def bind_selected(value):
            require(value['modular'] is True and value['libraries']==libraries,
                    'actual derived preload graph changed after admission')
            graph.libraries(value['libraries'])
        frame.direct_modular_bind=bind_selected
    # The actual native controller and complete one-TU proof are consumed by
    # direct_runtime through this hook, keeping Native39 labels separate.
    frame.direct_build_receipt=lambda *args:receipt.receipt(frame,api,admission,evidence,*args,prepared=prepared)
    original=prepared['report']['physics_controller']['input']
    require(evidence.pin(original)==frame.inputs[selection['native_controller']], 'actual Native74 stand controller differs')
    generation=evidence.selected(selection['families']['unitree_go']['generation'])
    records={row['type']:row for row in generation['records']}
    frame.report['controlled_physics_admission']={'network':namespace,'workers':workers,'native_products':frame.inputs[selection['reports']['native_products']],
        'Guest_products':frame.inputs[selection['reports']['guests']],'CP310_view':frame.inputs[cp['view_receipt']],
        'native_preparation_mode':prepared['preparation_mode'],'native_admission_receipt':prepared['admission_pin'],
        'composition':'PERCEPTION_MOTION_WASM_CALLBACKS' if modular else 'STANDALONE','pure_provider_count':len(libraries),
        'image_vision_qualified':False,'actual_language_count':len(LANGUAGES),'measurement':measure,'historical_reports_rewritten':False,
        'physical_robot_access':False,'default_ROS_authority':[],'optional_WASI_rights':[]}
    return {'api':api,'admission':admission,'native':native,'session':session,'simulation':simulation,'evidence':evidence,
        'prepared':prepared,'chosen':chosen,'selection':selection,'worker_file_catalog':worker_files,'worker_elf_catalog':worker_elfs,
        'CP310_file_catalog':cp_files,'CP310_elf_catalog':cp_elfs,'binaries':binaries,'records':records,
        'chroot':chroot,'view_root':view_root,'python':python,'exec_gate':prepared['exec_gate'],
        'graph':graph,'modular':modular,'libraries':libraries}


def runtime_argv(admitted,frame,product,positive,connection,grants,socket_path):
    """Add only admitted pure preloads; never transfer main's ROS authority.

    The ordinary policy helper creates each provider with explicit disabled
    WASI and ROS2 contexts. Denied mains run without preloads at all. Library
    paths are actual selected artifacts already binary/source/MD/link checked.
    """
    libraries=tuple((row['name'],frame.input(row['guest_input'])) for row in admitted['libraries']) if positive else ()
    argv=admitted['session'].runtime_argv(admitted['api'],frame,product,'ros2',positive,connection,grants,socket_path,
                                           libraries=libraries)
    actual=[(argv[i+2],argv[i+1]) for i,token in enumerate(argv) if token=='--wasm-preload-library']
    require(actual==[(name,str(path)) for name,path in libraries], 'actual preloads differ from exact admitted graph')
    for name,_ in libraries:
        for backend in ('wasip1','ros2'):
            require(any(argv[i:i+2]==['--'+backend+'-single-disable',name] for i in range(len(argv)-1)),
                    'pure provider must have explicit disabled WASI/ROS authority')
            require(not any(argv[i:i+2]==['--'+backend+'-single-'+operation,name]
                    for operation in ('enable','grant','mount','add-environment') for i in range(len(argv)-1)),
                    'pure provider cannot inherit main or host authority')
    return argv


class Hooks:
    """Hold/map real processes; never manufacture a callback or physics state."""
    def __init__(self,frame,admitted,language,*,label_prefix=''):
        require(label_prefix in ('','hotspots-','uarch-exploration-'), 'closed collection label prefix required')
        self.frame,self.a,self.language=frame,admitted,language
        self.label_prefix=label_prefix
        self.gateways={};self.processes=[];self.lines={};self.bindings={}

    def python_command(self,script,tail):
        frame=self.frame;a=self.a
        require(script in ('mujoco_driver.py','native_peer.py'),'only maintained actual simulator/witness scripts allowed')
        logical='/'+str(a['python'].relative_to(a['view_root']))
        return [str(a['chroot']),str(a['view_root']),logical,'-B','-s',str(frame.source/'tests/ros_unitree'/script),
                *frame.unitree.actor_control_arguments(frame),*tail]

    def python_ready(self,owner,child,label,script,ready):
        a=self.a;key='physics_python' if script=='mujoco_driver.py' else 'witness_python'
        proof=a['native'].mappings(a['api'],self.frame,owner,child.pid,a['python'],
            a['selection']['mappings'][key],a['CP310_file_catalog'],a['CP310_elf_catalog'])
        self.processes.append({'role':label,'python_script':script,'actual_readiness':ready,'live_mapping':proof})

    def start(self,owner,label,job,selected,role):
        frame=self.frame;a=self.a;native=a['native'];api=a['api'];chosen=a['chosen'];mapping=a['selection']['mappings']
        if role=='native':
            require(job['executable']==a['selection']['native_controller'] and job['arguments']==[], 'direct ordinary native executable required')
            child,row=owner.start(label,frame.command(job),environment=a['prepared']['environment'])
            # The real native main initially blocks on its12-byte setup. Its
            # executable/DT_NEEDED mappings can be inspected before any state.
            self.processes.append({'role':'native','live_before_setup':native.mappings(api,frame,owner,child.pid,
                frame.input(job['executable']),mapping['native_controller'],a['prepared']['file_catalog'],a['prepared']['elf_catalog'])})
            return child,row
        require(role in ('wasm','denied') and job['arguments']==[],'closed derived standalone Runtime job required')
        positive=role=='wasm';key='positive' if positive else 'denied';product=a['binaries'][self.language,key]
        connection,grants=policy(chosen,positive,a['records'])
        socket_path=frame.out/('s'+str(len(frame.report['execution_bindings'])))
        require(len(os.fsencode(socket_path))<104 and not socket_path.exists(),'fresh bounded owned Gateway socket required')
        argv=runtime_argv(a,frame,product,positive,connection,grants,socket_path)
        # Interpreter DSOs belong to host catalogs; a CP310/native LD_LIBRARY_PATH
        # must never silently redirect this host-linked executable to older libc.
        old_environment=frame.environment
        host_environment={key:value for key,value in old_environment.items() if key not in
            ('LD_LIBRARY_PATH','PYTHONPATH','AMENT_PREFIX_PATH','CMAKE_PREFIX_PATH','RMW_IMPLEMENTATION','CYCLONEDDS_URI','ROS_DOMAIN_ID','MUJOCO_GL')}
        try:
            frame.environment=host_environment
            child,binding,release=native.gated_worker(api,frame,owner,label,a,argv)
        finally:frame.environment=old_environment
        gateway=gateway_row=None
        if positive:
            config=frame.out/(self.label_prefix+self.language+'-gateway.conf')
            with config.open('x',encoding='utf-8') as stream:stream.write(native.configuration(2,child.pid,connection,grants,socket_path))
            config.chmod(0o600);config_pin=frame.pin(config)
            gateway,gateway_row=owner.start(self.label_prefix+self.language+'-gateway',[chosen['gateway']['path'],str(config)],
                                           json_output=False,environment=a['prepared']['environment'])
            until=time.monotonic()+20
            while not socket_path.exists():
                owner.check();require(gateway.poll() is None and time.monotonic()<until,'actual direct Gateway readiness failed');time.sleep(.01)
            facts=socket_path.lstat();require(stat.S_ISSOCK(facts.st_mode) and facts.st_uid==os.getuid(),'actual owned socket required')
            binding['Gateway_config']=config_pin
            binding['Gateway_before_context']=native.mappings(api,frame,owner,gateway.pid,Path(chosen['gateway']['path']),
                mapping['gateway'],a['prepared']['file_catalog'],a['prepared']['elf_catalog'])
            self.gateways[child.pid]=(gateway,gateway_row,binding)
        release()
        # The Guest-local entry holds before C++ constructors, JS initialization
        # or the actual user main. Consume the test exec gate separately; then
        # record the real Runtime image and complete map_files proof before G.
        require(native.line(frame,owner,child)==b'READY_UNITREE_MAIN\n','actual standalone main-entry hold required')
        self.lines[child.pid]=a['session'].Lines(owner,child)
        self.bindings[child.pid]=binding
        binding['live_Runtime_before_main']=native.mappings(api,frame,owner,child.pid,frame.workers['ros2'],
            mapping['worker'],a['worker_file_catalog'],a['worker_elf_catalog'])
        binding.update(role=role,language=self.language,actual_command=argv,grants=grants,default_authority=[],optional_WASI_rights=[])
        frame.report['execution_bindings'].append(binding);frame.save();owner.write(child,b'G')
        return child,next(row for actor,row in owner.rows if actor is child)

    def line(self,owner,child):
        return self.lines[child.pid].read() if child.pid in self.lines else owner.receive_line(child)

    def ready(self,owner,child,role):
        a=self.a;frame=self.frame
        if role=='native':
            self.processes.append({'role':'native','after_real_ROS_context':a['native'].mappings(a['api'],frame,owner,child.pid,
                frame.input(a['selection']['native_controller']),a['selection']['mappings']['native_controller_context'],
                a['prepared']['file_catalog'],a['prepared']['elf_catalog'])})
        elif role=='wasm':
            gateway,_,binding=self.gateways[child.pid]
            binding['Gateway_after_real_subscription']=a['native'].mappings(a['api'],frame,owner,gateway.pid,
                Path(a['chosen']['gateway']['path']),a['selection']['mappings']['gateway_context'],
                a['prepared']['file_catalog'],a['prepared']['elf_catalog'])

    def finished(self,owner,child,role):
        if child.pid in self.lines:
            binding=self.bindings[child.pid]
            binding['complete_actual_transcript']=self.lines[child.pid].finish(
                binding['gate_stdout'].encode('ascii')+b'READY_UNITREE_MAIN\n')
        else:
            require(not owner.lines[child.pid] and not owner.buffers[child.pid],
                    'ordinary native controller left extra or truncated output')
            row=next(row for actor,row in owner.rows if actor is child)
            require(not Path(row['stderr']).read_bytes(),'ordinary native controller emitted stderr')
        if child.pid not in self.gateways:return
        gateway,row,binding=self.gateways.pop(child.pid);until=time.monotonic()+10
        while gateway.poll() is None and time.monotonic()<until:owner.check();time.sleep(.01)
        require(gateway.poll()==0,'actual Gateway must retire normally after Worker closure')
        owner.finish(gateway)
        binding['Gateway_audit']=self.a['native'].gateway_audit(self.a['api'],self.frame,row,2,child.pid)


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ('source','manifest','config','out'):parser.add_argument('--'+name,type=Path,required=True)
    for name in ('manifest-sha256','config-sha256','selection-input'):parser.add_argument('--'+name,required=True)
    args=parser.parse_args();frame=None
    try:
        source,frozen,api,unitree=bootstrap(args)
        own=api.bounded(source/'tests/ros_unitree/run_direct_physics.py',MIB)
        require(frozen['tests/ros_unitree/run_direct_physics.py']=={'bytes':len(own),'sha256':hashlib.sha256(own).hexdigest()},'actual frozen physics coordinator required')
        frame=unitree.Frame(args,'unitree-three-direct-Guest-controlled-physics')
        frame.unitree,frame.frozen,frame.workers,frame.native_aliases=unitree,frozen,{},{}
        selection,pin=frame.common.document(frame.input(args.selection_input));require(pin==frame.inputs[args.selection_input],'actual physics selection changed')
        frame.report.update(cases=[],execution_bindings=[],physical_robot_access=False,default_ROS_authority=[],optional_WASI_rights=[])
        admitted=prepare(frame,api,unitree,selection);simulation=admitted['simulation'];measure=selection['measurement']
        if admitted['modular']:frame.report['kind']=admitted['graph'].PHYSICS_KIND
        spec=simulation.validate(frame)
        require(spec['robot']=='go2' and spec['family']=='go' and spec['domain']==1 and spec['peer_transport']=='ros2' and
                measure['steps']<=spec['steps'],'exact admitted Go2/domain1 actual-model selection required')
        baseline=spec['direct_runtime']
        for language in LANGUAGES:
            chosen=selection['guests'][language]
            direct={'language':language,'job':{'executable':selection['workers']['ros2'],'arguments':[]},
                'denied_job':{'executable':selection['workers']['ros2'],'arguments':[]},'runtime_input':selection['workers']['ros2'],
                'guest_input':chosen['positive'],'denied_guest_input':chosen['denied'],
                'guest_build_input':chosen['positive_receipt'],'denied_build_input':chosen['denied_receipt'],
                'native_job':{'executable':selection['native_controller'],'arguments':[]},
                'native_binary_input':selection['native_controller'],'native_build_input':selection['reports']['native_products'],
                'steps':measure['steps'],'warmup_steps':measure['warmup_steps'],'slowdown_trigger':measure['slowdown_trigger'],
                'controller_mode':'go2-stand','hold':None,'modular':admitted['modular'],'libraries':admitted['libraries']}
            # This explicitly recorded derived selection does not rewrite the
            # SHA-bound immutable config read by the Python physical actors.
            spec['direct_runtime']=direct;hooks=Hooks(frame,admitted,language)
            frame.simulation_python_command=hooks.python_command
            frame.simulation_python_environment=selection['cp310']['environment']
            frame.simulation_actor_ready=hooks.python_ready;frame.direct_start=hooks.start
            frame.direct_ready=hooks.ready;frame.direct_finished=hooks.finished;frame.direct_line=hooks.line
            owner=unitree.Actors(frame)
            result=simulation.direct_runtime(owner,frame,spec)
            api.terminal(frame,owner);cleanup=owner.close();require(cleanup['status']=='PASS','actual whole control-case cleanup required')
            frame.actors.remove(owner)
            require(not hooks.gateways,'owned Gateway remains after the actual matched control case')
            frame.report['cases'].append({'language':language,'status':'PASS_ACTUAL_GUEST_NATIVE_DDS_MUJOCO',
                'derived_selection':direct,'actual_control':result,'actual_processes':hooks.processes,'cleanup':cleanup})
            frame.save()
        spec['direct_runtime']=baseline
        require(len(frame.report['cases'])==len(LANGUAGES),'all three actual language comparisons required')
        actual_roles=sum(len(row['actual_control']['actual_matched_controllers']) for row in frame.report['cases'])
        actual_steps=sum(len(role['actual_physics_states']) for row in frame.report['cases']
                         for role in row['actual_control']['actual_matched_controllers'].values())
        require(actual_roles==2*len(LANGUAGES) and actual_steps==actual_roles*measure['steps'],
                'every counted physics step must have an actual native/Guest state observation')
        frame.report.update(actual_matched_roles=actual_roles,actual_controlled_physics_steps=actual_steps,
            actual_default_denied_witnesses=sum('default_denial_witness' in row['actual_control'] for row in frame.report['cases']),
            selected_original_pin_bridges=admitted['evidence'].bridges,
            performance_qualified=all(row['actual_control']['performance_sample_qualification']=='AT_LEAST_30_PER_ROLE'
                and not row['actual_control']['profiling_required'] for row in frame.report['cases']),
            profiling_required=any(row['actual_control']['profiling_required'] for row in frame.report['cases']))
        return frame.finish(admitted['graph'].PHYSICS_STATUS if admitted['modular'] else
                            'PASS_THREE_ACTUAL_STANDALONE_GUEST_NATIVE_DDS_MUJOCO_CONTROL_CASES')
    except BaseException as error:
        if frame is not None:return frame.finish('FAIL',repr(error))
        print(repr(error),file=sys.stderr);return 1


if __name__=='__main__':raise SystemExit(main())
