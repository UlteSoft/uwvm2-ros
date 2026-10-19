"""Executable port of ONE exact original rospy subscriber-first testcase.

This module has no candidate imports or effects on import. It matches only the
Git-locked original module/class/method and launch+echo source, then replays real
SDK, Worker and Guest compilation inputs before starting any role. The original
native official result, this C API adaptation, and default-denied negative are
three distinct observations. No prior custom PASS substitutes for this run.

The first supported provider is native Noetic/Focal x86_64. Other distros/arches
remain missing port prerequisites until their real source/provider ABI is added;
the26-target catalog is not a claim that this one port covers every target.
"""
import hashlib
import http.client
import json
import os
from pathlib import Path
import re
import stat
import time
import xmlrpc.client

ADAPTER_ABI='official-api-port-v2'
SUPPORTED_OFFICIAL_CASES=[]
REPOSITORY='https://github.com/ros/ros_comm.git'
COMMIT='30483a9f218f1545eec16d3934bf3cb042e2cb5b'
FILES={
 'test/test_rospy/test/rostest/test_pubsub_order.py':(3786,'c0f44510807ba74fafb1e3a64c57b1b3ca8d93eccabc1cca5b77ca1a49c9027e'),
 'test/test_rospy/test/rostest/pubsub-order.test':(298,'20109aa4ebc61becbd5cb3600e735245cf5134be8f43a78879cfffda0a39a7d6'),
 'test/test_rospy/nodes/listenerpublisher.py':(2500,'5a8bd018338f0290a33de9a1cc7accc3a6335a13c0bbea3753d8dd3679ba22be')}
GUEST='tools/ros_distro/official_suites/ports/rospy_pubsub_order_guest.c'
PEER='tools/ros_distro/official_suites/ports/rospy_pubsub_order_peer.py'
METHOD='test_pubsub_order.TestPubSubOrder.test_subscriber_first'
MD5='992ce8a1687cec8c8bd883ec73ca41d1'
GRANTS=[['pub','topic.publish','/chatter','std_msgs/String',MD5],
        ['sub','topic.subscribe','/listenerpublisher','std_msgs/String',MD5]]


def need(value,reason):
    if not value:raise ValueError(reason)


def grant_limits(operation):
    """Literal current production topic schema; no wildcard or implicit bound."""
    result={'max-message-bytes':'64','max-rate-hz':'100','burst':'16','max-bytes-per-second':'65536',
        'max-queue-bytes':'1024','overflow-policy':'reject','validator':'none','audit':'allow-and-deny',
        'audit-max-rate-hz':'100','audit-burst':'16','audit-max-records':'10000','audit-max-bytes':'1048576',
        'max-publishers' if operation=='topic.publish' else 'max-subscribers':'1',
        'queue-depth':'16','transport':'tcpros'}
    if operation=='topic.publish':result['latch']='false'
    return result


def policy_arguments(mode,*,master_uri,gateway_socket='{out}/rpc',uid=None):
    """Build ONLY the explicit ROS policy suffix, never Runtime/tool argv.

    Root appends this to its independently admitted interpreter/main/Wasm argv.
    Both global defaults stay disabled. Denied mode adds audit budgets only.
    Positive enables exactly main's two String aliases, with every literal
    schema field repeated identically in the independent Gateway config.
    """
    need(mode in ('positive','default-denied'),'explicit port authority mode required')
    result=['--ros1-global-disable','--ros1-single-create','main',
            '--ros1-single-enable' if mode=='positive' else '--ros1-single-disable','main']
    settings={'audit-deny-max-rate-hz':'100','audit-deny-burst':'16','audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'}
    if mode=='positive':
        need(type(uid) is int and uid>=0,'actual selected Gateway uid required')
        settings.update({'gateway-socket':gateway_socket,'gateway-uid':str(uid),'connect-timeout-ms':'10000',
            'master-uri':master_uri,'advertise-host':'127.0.0.1','transport':'tcpros','node-name':'official_pubsub_gateway','namespace':'/'})
    for name,value in settings.items():result+=['--ros1-single-'+name,'main',value]
    if mode=='positive':
        for grant in GRANTS:
            result+=['--ros1-single-grant','main',*grant]
            for name,value in grant_limits(grant[1]).items():result+=['--ros1-single-set','main',grant[0],name,value]
    return result


def supports_discovered_case(row,spec,lock):
    """Match raw discovered identities, never invent IDs from a desired count.

    JUnit file universes retain their actual file SHA and raw classname/ordinal.
    Those hashes are necessarily different for each actual run. Only this
    exact locked original class/method is accepted within such a real file.
    """
    selected=[v for v in lock['snapshots'] if v['id']==spec['snapshot']]
    if len(selected)!=1 or selected[0]['repository']!=REPOSITORY or selected[0]['commit']!=COMMIT:return False
    if not set(FILES)<=set(spec['source_test_files']):return False
    key=row['key'];value=row['native_observation']
    if key['universe']=='unittest-loaded-method':return key['id']==METHOD
    return key['universe'].startswith('junit-file:') and value.get('name')=='test_subscriber_first' and \
        value.get('classname') in ('TestPubSubOrder','__main__.TestPubSubOrder','test_pubsub_order.TestPubSubOrder')


def _source(scope,relative):
    actual=scope.pin(scope.source/relative)
    need(scope.frozen.get(relative)=={name:actual[name] for name in ('bytes','sha256')},'actual current complete-MF source required')
    return actual


def _command(scope,row):
    need(row.get('status')=='PASS' and row.get('returncode')==0 and row.get('terminal_owned_births'),
         'real completed owned tool command with terminal births required')
    scope.pin(scope.admit_pin(row['log'])['path'])
    return row['argv']


def _worker(scope,report,builder,worker_api):
    """Replay original real three-TU ROS1 Worker MD and actual native link trace."""
    need(report.get('kind')=='native-interpreter-four-profile-build' and
        report.get('status')=='PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED' and
        report['source']==str(scope.source) and report['source_manifest_sha256']==scope.manifest_current['sha256'] and
        all(report.get(name) is True for name in ('source_preserved','inputs_preserved','products_preserved')) and
        not report['finalization_errors'] and report['cleanup']['status']=='PASS' and not report['cleanup']['remaining'],
        'actual complete preserved current-source Worker producer required')
    config=scope.document(report['configuration'])
    roots,tools,_,_,_=worker_api.configuration(config,scope.source,builder)
    need(all(any(root==value or root.is_relative_to(value) for value in scope.roots) for root in roots),
         'Worker complete compiler/provider roots lack actual case closure')
    selected=[value for value in report['profiles'] if value['profile']=='ros1']
    need(len(selected)==1 and selected[0]['binary']==scope.config['roles']['worker']['image'],
         'actual selected ROS1 interpreter Worker required')
    row=selected[0];need(row['status']=='BUILT_NOT_EXECUTED' and len(row['objects'])==3 and row['full_dependencies_verified'],
        'all three fresh Worker translation units required')
    source_names=dict(worker_api.UNITS)
    need({item['unit'] for item in row['objects']}==set(source_names),'exact Runtime/main/host_api units required')
    for item in row['objects']:
        scope.guard();source=_source(scope,source_names[item['unit']]);need(item['source']==source,'compiled primary differs from actual source')
        job=report['commands'][item['command_index']];argv=job['argv']
        need(job['status']=='PASS' and job['returncode']==0 and '-MD' in argv and '-MMD' not in argv and
             '-DUWVM_IMPORT_ROS1' in argv and '-DUWVM_IMPORT_ROS2' not in argv and
             argv[argv.index('-c')+1]==source['path'] and argv[argv.index('-o')+1]==item['object']['path'] and
             argv[argv.index('-MF')+1]==item['dependency_file']['path'] and
             argv[argv.index('-MT')+1]==item['object']['path'] and argv[0]==tools['cxx'], 'actual ROS1 full-MD Worker command differs')
        scope.pin(job['log']['path'])
        dep,deps=builder.dependency_inputs(scope.common,Path(item['dependency_file']['path']),Path(item['object']['path']),scope.source,roots)
        need(dep==item['dependency_file'] and deps==item['dependencies'] and source in deps,'actual full-MD Worker inputs changed')
        for value in deps:scope.pin(scope.admit_pin(value)['path'])
        need(builder.elf(scope.common,Path(item['object']['path']),config['target'],(1,))==scope.pin(item['object']['path']),
             'actual compiled native Worker object changed')
    link=report['commands'][row['link_command_index']]
    need(link['status']=='PASS' and link['returncode']==0 and '-Wl,--trace' in link['argv'],'real completed traced Worker link required')
    raw,actual=scope.common.body_pin(Path(link['log']['path']),32*1024**2);scope.pin(actual['path'])
    names={}
    for line in raw.decode('utf-8','strict').splitlines():
        if not line.strip():continue
        path=Path(line.split('(',1)[0]);need(path.is_absolute(),'original absolute Worker trace required')
        value=scope.pin(path.resolve(strict=True));names[value['path']]=value
        if path.suffix=='.a':builder.archive(scope.common,path.resolve(strict=True),config['target'])
    need(list(names.values())==row['link_inputs'] and all(item['object'] in names.values() for item in row['objects']),
         'real Worker link trace object/provider closure differs')
    need(builder.elf(scope.common,Path(row['binary']['path']),config['target'])==scope.pin(row['binary']['path']),
         'actual selected Worker ELF differs')
    return report


def _sdk(scope,report,support):
    """Reopen the real retained native producer's complete SDK/work and3 MDs."""
    need(report.get('schema')=='uwvm-unitree-noetic-native-products-v1' and
        report.get('status')=='PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED' and
        report['source']==str(scope.source) and report['source_manifest_sha256']==scope.manifest_current['sha256'] and
        report['qualified_primary_translation_units']==3 and not report['native_finalization_errors'] and
        all(report.get(name) is True for name in ('source_preserved','input_pins_preserved','whole_roots_preserved',
          'original_sdk_preserved','original_official_upstream_preserved','official_source_copy_unmodified',
          'execution_view_preserved','acquisition_source_preserved')) and report['cleanup'] and
        all(value['status']=='PASS' and not value.get('remaining') for value in report['cleanup']),
        'actual completed preserved Noetic SDK/native producer required')
    current={}
    for name,selected in report['execution_view_after'].items():
        root=Path(selected['root']);need(any(root==value or root.is_relative_to(value) for value in scope.roots),'complete native SDK/work root missing')
        scope.pin(selected['catalog']['path'])
        actual=support.tree(scope.common,root,scope.out/('native-%s-current.jsonl'%name),scope.guard)
        scope.product(actual['catalog']['path'],512*1024**2)
        need({key:value for key,value in actual.items() if key!='catalog'}==
             {key:value for key,value in selected.items() if key!='catalog'},'actual whole SDK/work changed')
        current[name]=root
    def elf(common,path,kind=None):return support.elf(common,path,kind,guard=scope.guard)
    need(set(report['products'])=={'gateway','legacy_peer','identity_exporter'},'exact three native build roles required')
    for role,selected in report['products'].items():
        needed={'gateway':'gateways/ros1/main.cc','legacy_peer':'tests/ros_unitree/native_legacy_ros1_peer.cpp',
                'identity_exporter':'sdk/integrations/unitree/native_identity.cpp'}
        original=_source(scope,needed[role]);source=Path(selected['primary_source']['path'])
        if role=='identity_exporter':
            need(selected['original_frozen_source']==original and source==current['work']/'identity-selection/native_identity.cpp' and
                 all(selected['primary_source'][name]==original[name] for name in ('bytes','sha256')),
                 'actual staged exporter must preserve the exact original complete-MF body')
        else:need(selected['primary_source']==original,'actual native primary source differs')
        actual=support.build_proof(scope,current['sdk'],current['work'],Path(selected['compile_database']['path']).parent,
            {'stdout':selected['verbose_stdout']['path'],'stderr':selected['verbose_stderr']['path']},source,
            Path(selected['binary']['path']),elf)
        need(all(selected.get(key)==value for key,value in actual.items()),'original native full-MD/link/ELF proof changed')
    if scope.mode=='positive':need(scope.config['roles']['gateway']['image']==report['products']['gateway']['binary'],'selected Gateway differs from qualified actual native product')
    return report


def _guest(scope,report,utilities,inspector,producer):
    need(report.get('kind')=='official-rospy-pubsub-guest-build-v1' and report.get('status')==
        'PASS_PREPARED_OFFICIAL_ROSPY_PUBSUB_GUEST_VALIDATED_NOT_EXECUTED' and
        report['source']==str(scope.source) and report['source_manifest_sha256']==scope.manifest_current['sha256'] and
        report['guest']==scope.config['wasm'] and report['Runtime_executed'] is False and report['default_authority']==[] and
        all(report.get(name) is True for name in ('source_preserved','input_pins_preserved','whole_roots_preserved','full_dependencies_verified')) and
        not report.get('finalization_errors') and report['cleanup'] and
        all(value['status']=='PASS' and not value.get('remaining') for value in report['cleanup']),
        'actual preserved standalone Guest compilation/validation report required')
    need(len(report['commands'])==3 and [v['role'] for v in report['commands']]==['compile','link','validate'],
         'one actual compile/link/full-validator required')
    compiled,linked,validated=report['commands'];argv=_command(scope,compiled['command'])
    root=Path(report['sysroot']);need(root.resolve(strict=True)==root and root.is_dir() and
        any(root==value or root.is_relative_to(value) for value in scope.roots),'selected full WASI sysroot missing')
    alias=utilities.alias(report['linker_alias']['path'])
    need(alias==report['linker_alias'] and alias['physical']==report['tool_pins']['wasm_ld']['path'],'actual selected linker alias changed')
    recipes=producer.recipes(scope.source,Path(compiled['object']['path']).parent,
        {name:value['path'] for name,value in report['tool_pins'].items()},root,alias['path'])
    need([value['command']['argv'] for value in report['commands']]==list(recipes),'actual compiler/link/validator recipe differs from the closed source-bound producer')
    source=_source(scope,GUEST);need(compiled['source']==source and '-MD' in argv and '-MMD' not in argv and
        argv[argv.index('-c')+1]==source['path'] and argv[argv.index('-o')+1]==compiled['object']['path'] and
        argv[argv.index('-MF')+1]==compiled['dependency_file']['path'] and
        argv[argv.index('-MT')+1]==compiled['object']['path'],'actual standalone source/full-MD command required')
    deps=utilities.full_md(scope,Path(compiled['dependency_file']['path']),Path(compiled['object']['path']),Path(source['path']),Path(compiled['object']['path']).parent)
    need(deps==compiled['dependencies'],'actual Guest full-MD dependencies changed')
    for value in deps:scope.pin(scope.admit_pin(value)['path'])
    argv=_command(scope,linked['command']);need('-Wl,--trace' in argv and str(compiled['object']['path']) in argv,
        'actual Guest object/traced link required')
    links=utilities.trace_inputs(scope,linked['command'],Path(compiled['object']['path']).parent)
    need(links==linked['inputs'] and compiled['object'] in links,'actual Guest link inputs differ')
    argv=_command(scope,validated['command']);need(argv==[report['tool_pins']['validator']['path'],'validate',scope.config['wasm']['path']],
        'actual full code-body validator/artifact association required')
    for value in report['tool_pins'].values():scope.pin(scope.admit_pin(value)['path'])
    raw,current=scope.common.body_pin(Path(scope.config['wasm']['path']),64*1024**2)
    need(current==scope.config['wasm'],'actual Wasm bytes changed')
    inspected=producer.boundary(raw,utilities,inspector)
    need(inspected==report['binary_boundary'],'actual Guest entry/memory structure changed')
    argv=scope.config['roles']['worker']['arguments']
    positions=[i for i,v in enumerate(argv) if v=='--wasm-set-start-func']
    need(len(positions)==1 and positions[0]+1<len(argv) and argv[positions[0]+1]==str(inspected['local_entry']),
         'actual Runtime must invoke the same-module admitted standalone local entry')
    abi=scope.document(scope.pin(scope.source/'sdk/abi/abi-v1.json'))
    functions={row['name']:([p['type'] for p in row['params']],row['results']) for row in abi['modules']['uwvm_ros1_v1']['functions']}
    seen=set()
    for value in inspector.inspect(raw):
        name=(value['module'],value['name']);need(value['kind']==0 and name not in seen,'unique actual function imports required');seen.add(name)
        shape=(value['params'],value['results'])
        if name[0]=='uwvm_ros1_v1':need(shape==functions.get(name[1]),'actual ROS1 import ABI differs')
        else:
            expected=(['i32'],[]) if name[1]=='proc_exit' else (['i32']*4,['i32'])
            need(name[0]=='wasi_snapshot_preview1' and name[1] in ('fd_read','fd_write','proc_exit') and shape==expected,
                 'Guest requires authority beyond basic stdin/stdout/exit')
    need({('wasi_snapshot_preview1',name) for name in ('fd_read','fd_write','proc_exit')}<=seen and
         ('uwvm_ros1_v1','capability_open') in seen,'real basic IO and capability imports required')


def verify_prerequisites(scope,documents):
    need(scope.profile['backend']==1 and scope.profile['distro']=='noetic' and
        scope.profile['target']['architecture']=='x86_64' and scope.profile['runtime']['mode']=='native',
        'this exact source port currently requires an actual native Noetic/Focal SDK')
    need(supports_discovered_case(scope.native_row,scope.spec,scope.source_lock),'official source lock/test identity differs')
    snapshot=scope.snapshot_roots[scope.spec['snapshot']];fixture_by_path={v['path']:v for v in scope.config['fixture_inputs']}
    for relative,(size,digest) in FILES.items():
        actual=scope.pin(snapshot/relative)
        need(actual['bytes']==size and actual['sha256']==digest and fixture_by_path.get(actual['path'])==actual,
             'all three exact independently verified upstream fixture bodies required')
    for relative in (GUEST,PEER):
        need(_source(scope,relative) in scope.config['source_inputs'],'actual port primary source missing from selected source inputs')
    need(set(scope.config['roles'])==({'master','native_peer','worker','gateway'} if scope.mode=='positive' else {'master','native_peer','worker'}),
         'one owned native Master, original echo witness, Worker and optional positive Gateway required')
    expected=GRANTS if scope.mode=='positive' else []
    need(scope.config['rights']['grants']==expected,'only the exact original chatter publisher and echo subscription are permitted')
    # Load only the caller's admitted current frozen definitions. None of these
    # main routines run, and there is no ambient Python module substitution.
    builder=scope.load_source('tests/ros_debugger/build_runtime.py','official_pubsub_worker_binary_reader')
    workers=scope.load_source('tests/ros_unitree/build_interpreter_workers.py','official_pubsub_worker_build_reader')
    support=scope.load_source('tests/ros_unitree/noetic_native_support.py','official_pubsub_native_evidence')
    utilities=scope.load_source('tests/ros_unitree/command_build_support.py','official_pubsub_wasm_evidence')
    inspector=scope.load_source('sdk/tools/check_wasm_imports.py','official_pubsub_import_reader')
    producer=scope.load_source('tools/ros_distro/official_suites/ports/build_rospy_pubsub_guest.py','official_pubsub_recipe_reader')
    need(len(documents['sdk_evidence'])==2 and len(documents['worker_evidence'])==1,'exact native+Guest build and Worker prerequisites required')
    native=_sdk(scope,documents['sdk_evidence'][0],support)
    _guest(scope,documents['sdk_evidence'][1],utilities,inspector,producer)
    _worker(scope,documents['worker_evidence'][0],builder,workers)
    roles=scope.config['roles'];master=roles['master'];peer=roles['native_peer']
    launch=native['launch']['master'];port=roles['master']['environment']['ROS_MASTER_URI']
    match=re.fullmatch(r'http://127\.0\.0\.1:([1-9][0-9]{3,4})/',port)
    need(match and 1024<=int(match[1])<=65535,'one literal owned loopback Master port required')
    authority=scope.load_source('tools/ros_distro/official_suites/differential.py','official_pubsub_complete_authority_reader')
    expected_policy=policy_arguments(scope.mode,master_uri=port,uid=os.getuid())
    need(authority.authority(roles['worker']['arguments'],1,scope.mode,expected)==
         authority.authority(expected_policy,1,scope.mode,expected),'actual Worker policy differs from the exact port/Gateway limits')
    args=[value.replace('{port}',match[1]) for value in launch['arguments']]
    need(master['launcher']==master['host']==master['loader']==launch['executable'] and
        master['image']==launch['python'] and master['arguments']==args and
        peer['launcher']==peer['host']==peer['loader']==launch['executable'] and peer['image']==launch['python'] and
        peer['arguments']==['--library-path',native['environment']['LD_LIBRARY_PATH'],launch['python']['path'],
            str(scope.source/PEER),'--fixture',str(snapshot/'test/test_rospy/nodes/listenerpublisher.py'),'--mode',scope.mode],
        'actual original SDK loader/Python/master and exact peer protocol required')
    scope.pin(launch['script']['path'])
    for role in ('master','native_peer')+ (('gateway',) if scope.mode=='positive' else ()):
        env=roles[role]['environment'];need(env.get('ROS_IP')=='127.0.0.1' and env.get('ROS_MASTER_URI')==port and
            all(env.get(name)==value for name,value in native['environment'].items()),'native roles must use the actual qualified separate SDK environment')
    scope.port_native=native;scope.port_master_port=int(match[1]);scope.port_fixture=snapshot


def normalize(role,raw,configuration):
    value=json.loads(raw)
    if configuration['mode']=='default-denied':
        need(role=='native_peer' and type(value) is dict and set(value)=={'effects','samples','elapsed_ns'} and
             type(value['samples']) is int and value['samples']>=30 and type(value['elapsed_ns']) is int and
             value['elapsed_ns']>=2000000000,'actual native quiet record requires both two seconds and thirty samples')
    else:need(type(value) is dict and value=={'message':'hi [42]'},'exact original callback String required')
    return value


def _record(scope,role):return json.loads(scope.observations[scope.read(role)]['raw'])


def _expect(scope,role,text):
    token=scope.read(role);need(scope.observations[token]['raw']==text,'actual cooperative port gate differs');return token


def _master_ready(scope):
    """Bound XMLRPC body/timeout and require PID of the actual owned Master."""
    child,_=scope.children['master'];until=time.monotonic()+20;last=None
    while time.monotonic()<until:
        scope.owner.check();need(child.poll() is None,'owned Master retired before readiness')
        request=xmlrpc.client.dumps(('/uwvm_official_owner',),methodname='getPid').encode()
        connection=http.client.HTTPConnection('127.0.0.1',scope.port_master_port,timeout=.25)
        try:
            connection.request('POST','/',request,{'Content-Type':'text/xml'})
            response=connection.getresponse();length=response.getheader('Content-Length')
            need(response.status==200 and response.getheader('Content-Encoding','identity')=='identity' and
                 response.getheader('Transfer-Encoding') is None and length is not None and length.isdecimal() and
                 0<int(length)<=65536,'ordinary bounded actual Master XMLRPC response required')
            raw=response.read(65537);need(len(raw)==int(length),'Master XMLRPC body length differs')
            values,_=xmlrpc.client.loads(raw);reply=values[0]
            need(type(reply) in (tuple,list) and len(reply)==3 and reply[0]==1 and reply[2]==child.pid,
                 'Master readiness names another process')
            scope.prove('master');scope.report['master_getPid']={'response':reply,'raw_sha256':hashlib.sha256(raw).hexdigest()};return
        except (OSError,http.client.HTTPException) as error:last=repr(error)
        finally:connection.close()
        scope.tick()
    raise ValueError('actual owned Master readiness exhausted: '+str(last))


def execute(scope):
    scope.start('master');_master_ready(scope)
    scope.start('native_peer');native_gate=_record(scope,'native_peer')
    need(native_gate=={'event':'native-gate','pid':scope.children['native_peer'][0].pid},'native witness birth gate differs')
    scope.prove('native_peer');scope.setup_write('native_peer',b'G')
    need(_record(scope,'native_peer')=={'event':'native-subscribed','publishers':0},'native subscriber-first graph assertion failed')
    scope.prove('native_peer')
    scope.start('worker');_expect(scope,'worker',b'PORT_WORKER_GATE');scope.prove('worker')
    if scope.mode=='positive':
        native_api=scope.load_source('tests/ros_javascript_guest/native.py','official_pubsub_gateway_config')
        socket_path=scope.out/'rpc';need(len(os.fsencode(socket_path))<=107,'explicit owned Unix socket exceeds ABI limit')
        grants=[dict(zip(('alias','operation','resource','type','identity'),row),limits=grant_limits(row[1])) for row in GRANTS]
        connection={'master-uri':scope.config['roles']['master']['environment']['ROS_MASTER_URI'],
                    'advertise-host':'127.0.0.1','transport':'tcpros','node-name':'official_pubsub_gateway','namespace':'/'}
        text=native_api.configuration(1,scope.children['worker'][0].pid,connection,grants,socket_path)
        path=scope.out/'gateway.conf'
        with path.open('x') as stream:stream.write(text);stream.flush();os.fsync(stream.fileno())
        scope.report['gateway_configuration']=scope.product(path)
        need(scope.config['roles']['gateway']['arguments']==['{out}/gateway.conf'],'exact fresh owned Gateway configuration argv required')
        scope.start('gateway');until=time.monotonic()+20
        while not socket_path.exists():
            need(scope.children['gateway'][0].poll() is None and time.monotonic()<until,'Gateway readiness exhausted');scope.tick()
        info=socket_path.lstat();need(stat.S_ISSOCK(info.st_mode) and info.st_uid==os.getuid(),'actual owned IPC socket required')
    scope.release();scope.write('worker',b'G+' if scope.mode=='positive' else b'G-')
    if scope.mode=='positive':
        _expect(scope,'worker',b'PORT_WORKER_READY');scope.write('native_peer',b'M')
        need(_record(scope,'native_peer')=={'event':'native-matched','publishers':1},'actual new chatter publisher did not match')
        scope.prove('gateway');scope.prove('worker')
        for index in range(10):
            scope.write('worker',b'P');_expect(scope,'worker',b'PORT_WORKER_SENT');scope.tick(.1)
        scope.write('native_peer',b'E');native=scope.read('native_peer');wasm=None
        for attempt in range(2000):
            scope.write('worker',b'R');token=scope.read('worker')
            if scope.observations[token]['raw']!=b'PORT_WORKER_AGAIN':wasm=token;break
            scope.tick()
        need(wasm is not None,'actual original echo callback was not delivered')
        scope.effect('locked-rospy-subscriber-first-exact-echo',native,wasm,{'message':'hi [42]'})
        scope.report['original_assertions']={'subscriber_created_before_publisher':True,'actual_native_zero_publisher_gate':True,
            'published_messages':10,'owner_interval_seconds':.1,'selected_original_random_domain_value':42,
            'original_random_domain_exhausted':False,'callback_data_equal':True}
    else:
        _expect(scope,'worker',b'PORT_WORKER_DENIED_13_13');scope.write('native_peer',b'Q')
        scope.quiet(scope.read('native_peer'),minimum_ns=2000000000)
    scope.write('worker',b'F');scope.finish_role('worker')
    if scope.mode=='positive':scope.finish_role('gateway')
    scope.write('native_peer',b'F');scope.finish_role('native_peer');scope.interrupt_master()
