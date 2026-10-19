"""Closed source planning over real generated bounded types; never executes code.

The trusted caller supplies read(role, full7)->bytes after its actual selected-root
same-FD admission. This module additionally checks those exact bytes. It neither
acquires upstream source nor validates a Git signature. Its result is an
unexecuted plan, never a native identity/transport/hardware qualification.
"""
from pathlib import PurePosixPath
import hashlib
import json
import re

LANGUAGES = ('c','cpp','rust','javascript','assemblyscript','tinygo')
PIN = {'path','bytes','sha256','device','inode','mtime_ns','ctime_ns'}
REQUIRED_OUTPUTS = {'bounded_records.h','bounded_records.hpp','bounded_records.rs',
    'bounded_records.mjs','bounded_records.ts','bounded_records.go','manifest.hpp'}

# Explicit additive source planning only. Historical LANGUAGES/default behavior
# remains the original six; no managed emitter is invoked by this planner.
MANAGED_LANGUAGES = ('python','java','csharp')
MANAGED_OUTPUTS = {'python': 'bounded_records.py',
    'java': 'org/uwvm/roswasm/generated/BoundedRecords.java',
    'csharp': 'csharp/BoundedRecords.cs'}
MANAGED_FOUNDATIONS = {
    'optional_generator': 'sdk/custom-types/managed_profile.py',
    'schema_generator': 'sdk/tools/custom_types.py',
    'bounded_generator': 'sdk/custom-types/bounded_profile.py'}
MANAGED_LANGUAGE_FOUNDATIONS = {
    'python': {'python_codecs': 'sdk/python/roswasm/codecs.py'},
    'java': {'java_codec_interface': 'sdk/java/src/main/java/org/uwvm/roswasm/Codec.java',
             'java_codecs': 'sdk/java/src/main/java/org/uwvm/roswasm/Codecs.java'},
    'csharp': {'csharp_emitter': 'sdk/custom-types/managed_csharp.py',
               'csharp_codec_interface': 'sdk/csharp/RosWasm/Result.cs',
               'csharp_codecs': 'sdk/csharp/RosWasm/Codecs.cs'}}
_LESSON = 'sdk/robots/vendors/examples/joint_state/'
MANAGED_LESSON_SOURCES = {
    'python': ('lesson.py','mode_default.py','mode_positive.py'),
    'java': ('java/JointStateLesson.java','java/SelectedRos1.java','java/SelectedRos2.java',
             'java/ModeDefault.java','java/ModePositive.java','java/pom.xml'),
    'csharp': ('csharp/Program.cs','csharp/JointStateLesson.csproj',
               'csharp/Directory.Build.props','csharp/global.json')}
MANAGED_SDK_SOURCES = {
    'python': tuple('sdk/python/roswasm/'+name for name in
        ('__init__.py','_registry.py','raw.py','client.py','codecs.py','events.py','_native.c')) +
        ('sdk/python/tools/freeze.py','sdk/python/embedding/main.c',
         'sdk/ros1/abi.h','sdk/ros2/abi.h','sdk/common/abi.h') +
        tuple('sdk/python/compat/rclpy/'+name for name in
            ('rclpy/__init__.py','rclpy/_dispatch.py','rclpy/_service_support.py',
             'rclpy/client.py','rclpy/clock.py','rclpy/context.py','rclpy/node.py',
             'rclpy/qos.py','rclpy/service.py','rclpy/task.py','rclpy/time.py','rclpy/timer.py',
             'std_msgs/__init__.py','std_msgs/msg/__init__.py',
             'std_srvs/__init__.py','std_srvs/srv/__init__.py',
             'example_interfaces/__init__.py','example_interfaces/srv/__init__.py',
             'example_interfaces/srv/_add_two_ints.py',
             'examples_rclpy_minimal_service/__init__.py',
             'examples_rclpy_minimal_service/service_member_function.py')),
    'java': tuple('sdk/java/src/main/java/org/uwvm/roswasm/'+name+'.java' for name in
        ('Capability','Codec','Codecs','LinearMemory','Node','OpenCleanup','Result','RetirementAttempt',
         'RetirementTransport','Ros1Raw','Ros1Transport','Ros2Raw','Ros2Transport','RosException','Session','Transport')),
    'csharp': tuple('sdk/csharp/RosWasm/'+name+'.cs' for name in
        ('Actions','Codecs','Events','NativeTransport','Node','Result','Retirement','SelectedRaw','Session')) +
        ('sdk/csharp/Uwvm.RosWasm.csproj','sdk/csharp/Directory.Build.props',
         'sdk/csharp/NativeBridge.targets','sdk/csharp/roswasm_bridge.c',
         'sdk/ros1/abi.h','sdk/ros2/abi.h','sdk/common/abi.h')}


def reviewed_inventory(inventory):
    """Admit legacy19 or the additive28 Source DATA; never grant permissions.

    Model/backend/revision/source/native checks still run in the actual caller.
    Inventory admission cannot load code, choose a native provider or launch a
    robot. NVIDIA ecosystem and unpublished products cannot enter as a brand.
    """
    if type(inventory) is not dict:
        return False
    if (inventory.get('vendor_count') == 19 and
            inventory.get('kind') == 'official-primary-vendor19-research-inventory-not-acceptance'):
        return True  # Preserve the original nineteen-vendor Source route.
    if (inventory.get('format_version') != 2 or type(inventory.get('format_version')) is not int or
            type(inventory.get('vendor_count')) is not int or inventory['vendor_count'] != 28 or
            inventory.get('kind') != 'official-primary-vendor28-research-inventory-not-acceptance' or
            inventory.get('default_authority') != []):
        return False
    predecessor = inventory.get('predecessor')
    if predecessor != {'path': 'sdk/robots/vendors/inventory.json', 'bytes': 26281, 'sha256': '97384f3e5926aa5e821a49c934677b9c49f58aedaa7a2440cf89e28c03eb680a', 'original_nineteen_rows_preserved': True}:
        return False
    rows = inventory.get('vendors')
    if type(rows) is not list or len(rows) != 28 or any(type(row) is not dict for row in rows):
        return False
    actual = [row.get('id') for row in rows]
    if any(type(value) is not str for value in actual) or sorted(actual) != ['abb', 'agibot', 'agilex', 'booster', 'boston_dynamics', 'clearpath', 'deep_robotics', 'dobot', 'doosan', 'elephant', 'fanuc', 'flexiv', 'fourier', 'franka', 'hello_robot', 'kinova', 'kuka', 'leju', 'limx', 'pal', 'realman', 'robotis', 'trossen', 'ubtech', 'ufactory', 'unitree', 'universal_robots', 'yaskawa']:
        return False
    # Exact reviewed DATA, including all preserved original rows and new primary sources.
    try:
        raw = json.dumps(inventory,sort_keys=True,separators=(',',':'),ensure_ascii=True,allow_nan=False).encode()
    except (TypeError,ValueError):
        return False
    if hashlib.sha256(raw).hexdigest() != '4f3fffd9de0e3f05cc9fde5180f0ffe2858371acc0e18b3406a1c57baaed4939':
        return False
    return all(row.get('default_authority') == [] and
        type(row.get('ros1')) is str and type(row.get('ros2')) is str and
        type(row.get('official_primary_sources')) is list and row['official_primary_sources'] and
        all(type(url) is str and url.startswith('https://') for url in row['official_primary_sources'])
        for row in rows)


def need(ok, why):
    if not ok:
        raise ValueError(why)


def unique_json(raw):
    def pairs(rows):
        value={}
        for key,item in rows:
            need(key not in value,'duplicate JSON key')
            value[key]=item
        return value
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite JSON'))


def full7(value):
    need(type(value) is dict and set(value)==PIN,'closed ordinary full7 required')
    need(type(value['path']) is str and value['path'].startswith('/') and
        type(value['sha256']) is str and re.fullmatch('[0-9a-f]{64}',value['sha256']) is not None,
        'absolute actual file role and SHA required')
    need(all(type(value[key]) is int and value[key]>=0 for key in PIN-{'path','sha256'}) and value['inode']>0,
        'typed actual full7 fields required')


def body(read,role,pin,limit=16*1024*1024):
    full7(pin)
    need(pin['bytes']<=limit,'bounded selected input role')
    raw=read(role,pin)
    need(type(raw) is bytes and len(raw)==pin['bytes'] and hashlib.sha256(raw).hexdigest()==pin['sha256'],
        'actual selected role bytes differ')
    return raw


def relative(value):
    need(type(value) is str and value and not PurePosixPath(value).is_absolute() and
        all(part not in ('','.','..') for part in value.split('/')),'closed relative source role')


def legacy_metadata_provenance(metadata, schemas):
    """Reconcile complete raw Message/Service/Action evidence, never calculate RIHS.

    V1 Message-only receipts remain accepted unchanged. V2 explicitly separates
    actual installed payloads from standard protocol declarations and retains
    per-interface/component identities; no native run is proved by JSON alone.
    """
    need(type(schemas) is dict and set(schemas)=={'format_version','kind','types'} and
        type(schemas['format_version']) is int and
        ((schemas['format_version']==1 and schemas['kind']=='complete-original-native-message-declarations-not-identities') or
         (schemas['format_version']==2 and schemas['kind']=='complete-original-native-interface-declarations-not-identities')) and
        type(schemas['types']) is dict and 1<=len(schemas['types'])<=128, 'complete retained native declarations required')
    need(type(metadata) is dict and set(metadata)=={'format','ros_version','types'} and
        metadata['format']=='uwvm-ros-official-bounded-native-v2' and type(metadata['ros_version']) is int and metadata['ros_version']==2 and
        type(metadata['types']) is dict and set(metadata['types'])==set(schemas['types']), 'exact complete legacy native inventory required')
    def graph(value,root):
        need(type(value) is dict and set(value)=={'root','references'} and type(value['root']) is dict and
            value['root'].get('type')==root and type(value['references']) is list, 'closed complete native graph required')
        return value
    def message(value,expected,root):
        graph(expected,root)
        need(type(value) is dict and set(value)=={'native_identity','identity_provenance','native_baked_hash_available','root','references'} and
            value['identity_provenance']=='LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01' and value['native_baked_hash_available'] is False,
            'actual legacy Message provenance required; no modern downgrade')
        identity(value)
        need(value['root']==expected['root'] and value['references']==expected['references'],
            'complete actual Message native declarations differ')
    def identity(value):
        need(type(value.get('native_identity')) is str and re.fullmatch('RIHS01_[0-9a-f]{64}',value['native_identity']) is not None,
            'separately exported actual identity required')
    def protocol_service(value,expected,root):
        graph(expected,root)
        need(type(value) is dict and set(value)=={'native_identity','identity_provenance','native_baked_hash_available',
            'standard_protocol','protocol_event_native_introspection','root','references'} and
            value['identity_provenance']=='LEGACY_NATIVE_SERVICE_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01' and
            value['native_baked_hash_available'] is False and value['standard_protocol']=='ROS2_SERVICE_EVENT_V1' and
            type(value['protocol_event_native_introspection']) is bool,
            'complete actual Action protocol Service provenance required')
        identity(value)
        need(value['root']==expected['root'] and value['references']==expected['references'],
            'actual Action protocol Service declarations differ')
    def action_protocols(value,expected,root):
        need(type(value) is dict and set(value)=={'native_identity','identity_provenance','native_baked_hash_available',
            'standard_protocol','action_type_support_associations_verified','protocol_associations','root','references'} and
            value['identity_provenance']=='LEGACY_NATIVE_ACTION_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01' and
            value['native_baked_hash_available'] is False and value['standard_protocol']=='ROS2_ACTION_EVENT_V1' and
            value['action_type_support_associations_verified'] is True,
            'complete actual Action plus standard protocol provenance required')
        identity(value)
        need(value['root']==expected['official']['root'] and value['references']==expected['official']['references'],
            'whole actual Action root/protocol declaration differs')
        roles={'send_goal':('service',root+'_SendGoal'),'get_result':('service',root+'_GetResult'),
            'feedback_message':('message',root+'_FeedbackMessage'),'cancel_goal':('service','action_msgs/srv/CancelGoal'),
            'status':('message','action_msgs/msg/GoalStatusArray'),'uuid':('message','unique_identifier_msgs/msg/UUID'),
            'time':('message','builtin_interfaces/msg/Time')}
        need(type(expected['protocol_associations']) is dict and type(value['protocol_associations']) is dict and
            set(expected['protocol_associations'])==set(value['protocol_associations'])==set(roles),
            'all five ActionSupport links plus UUID/Time native associations required')
        proof={}
        for role,(kind,name) in roles.items():
            declaration=expected['protocol_associations'][role];actual=value['protocol_associations'][role]
            need(type(declaration) is dict and set(declaration)=={'kind','graph'} and declaration['kind']==kind,
                'closed source Action protocol declaration role required')
            if kind=='service':protocol_service(actual,declaration['graph'],name)
            else:message(actual,declaration['graph'],name)
            proof[role]={'kind':kind,'native_identity':actual['native_identity'],'identity_provenance':actual['identity_provenance']}
            if kind=='service':proof[role]['protocol_event_native_introspection']=actual['protocol_event_native_introspection']
        return proof
    message_ids={};types={}
    for name,expected in schemas['types'].items():
        row=metadata['types'][name]
        need(type(row) is dict and set(row)=={'official','components'} and type(row['components']) is dict,
            'closed native root/component roles required')
        if schemas['format_version']==1:
            kind='message';official=graph(expected,name);components={'message':official}
        else:
            need(type(expected) is dict and set(expected)==({'kind','official','components','standard_protocol','protocol_associations'} if expected.get('kind')=='action' else {'kind','official','components','standard_protocol'}) and
                expected['kind'] in ('message','service','action') and type(expected['components']) is dict,
                'complete Message/Service/Action declaration envelope required')
            kind=expected['kind'];official=graph(expected['official'],name);components=expected['components']
            need(expected['standard_protocol']=={'message':'NONE','service':'ROS2_SERVICE_EVENT_V1','action':'ROS2_ACTION_EVENT_V1'}[kind],
                'exact reviewed Service protocol declaration grammar required')
        roles={'message':('message',),'service':('request','response'),'action':('goal','result','feedback')}[kind]
        need(set(row['components'])==set(roles) and set(components)==set(roles), 'whole native component inventory required')
        for role in roles:
            component=name if kind=='message' else name+'_'+role.title()
            message(row['components'][role],components[role],component)
        value=row['official']
        if kind=='message':
            message(value,official,name)
            need(value==row['components']['message'], 'actual Message root/component identity differs')
            message_ids[name]=value['native_identity']
        elif kind=='service':
            need(type(value) is dict and set(value)=={'native_identity','identity_provenance','native_baked_hash_available',
                'standard_protocol','protocol_event_native_introspection','root','references'} and
                value['identity_provenance']=='LEGACY_NATIVE_SERVICE_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01' and
                value['native_baked_hash_available'] is False and value['standard_protocol']=='ROS2_SERVICE_EVENT_V1' and
                type(value['protocol_event_native_introspection']) is bool,
                'explicit native Service plus standard protocol provenance required')
            identity(value)
            need(value['root']==official['root'] and value['references']==official['references'],
                'whole Service protocol/payload declarations differ')
        else:
            protocol_proof=action_protocols(value,expected,name)
        types[name]={'kind':kind,'native_identity':value['native_identity'],
            'component_identities':{role:row['components'][role]['native_identity'] for role in roles},
            'identity_provenance':value['identity_provenance']}
        if kind=='service':types[name]['protocol_event_native_introspection']=value['protocol_event_native_introspection']
        if kind=='action':types[name]['protocol_associations']=protocol_proof
    if schemas['format_version']==1:
        return {'identity_mode':'legacy-introspection','identity_provenance':'LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01',
            'native_baked_hash_available':False,'message_identities':message_ids}
    return {'identity_mode':'legacy-introspection','native_baked_hash_available':False,'types':types}


def legacy_provenance(read, preparation, binding):
    """Reopen both copies of complete actual legacy metadata and owner DATA."""
    metadata_raw=body(read,'legacy-native-metadata-original',binding['native_metadata_original'])
    need(body(read,'legacy-native-metadata-copy',binding['native_metadata_copy'])==metadata_raw, 'actual metadata copies differ')
    schema_raw=body(read,'legacy-prepared-native-declarations',preparation['files']['native-declaration-schemas.json'])
    need(body(read,'legacy-bound-native-declarations',binding['legacy_declaration_schemas'])==schema_raw, 'actual native schema copies differ')
    proof=legacy_metadata_provenance(unique_json(metadata_raw),unique_json(schema_raw))
    need(binding.get('legacy_identity_provenance')==proof, 'retained legacy provenance differs')
    owner=body(read,'legacy-native-owner-evidence-original',binding['native_owner_evidence_original'])
    need(body(read,'legacy-native-owner-evidence-copy',binding['native_owner_evidence_copy'])==owner, 'actual owner evidence copies differ')
    return proof


def native_default_contract(catalog, definitions, metadata):
    """Bind complete scalar defaults to independently exported modern metadata.

    This pure DATA validator is duplicated byte-exact in the injected planner.
    It executes no ROS generator/library and computes no native hash. Whole
    payload and Service/Action protocol graphs are required, including actual
    defaults, field order, native capacities and every nested dependency. The
    previous Quaternion-only receipt domain is retained for that exact family.
    """
    import math
    import struct
    from pathlib import PurePosixPath
    def require(ok,why):
        if not ok:raise ValueError('native default contract: '+why)
    rows=catalog.get('types',[])
    require(type(rows) is list and 1<=len(rows)<=128,'finite exact interface inventory required')
    sources={};requested=False;default_records=set()
    for row in rows:
        require(type(row) is dict and type(row.get('file')) is str,'explicit original definition required')
        raw=definitions.get(row['file'])
        require(type(raw) is bytes and len(raw)<=1024*1024,'whole bounded original source required')
        lines=[line.split('#',1)[0].strip() for line in raw.decode('utf-8').splitlines()]
        lines=[line for line in lines if line]
        name=row.get('type');require(type(name) is str and name not in sources,'unique original interface required')
        sources[name]=lines
        if any('=' in line.replace('<=','') or len(line.split())>2 for line in lines):
            requested=True;default_records.add(name)
        requested|=row.get('ros_version')==2 and name=='geometry_msgs/msg/Quaternion'
    # A source that erased defaults must not avoid the proof merely because
    # RIHS01 stayed unchanged. Independently exported actual defaults also
    # trigger full comparison. Only a genuinely no-default source+metadata
    # keeps the old None receipt domain. This traversal is bounded by the
    # original input reader; it follows only the known description roles.
    if (type(metadata) is dict and metadata.get('format')=='uwvm-ros-official-bounded-native-v2' and
        type(metadata.get('ros_version')) is int and metadata['ros_version']==2 and type(metadata.get('types')) is dict):
        for actual_row in metadata['types'].values():
            if type(actual_row) is not dict:continue
            descriptions=[actual_row.get('official')]
            if type(actual_row.get('components')) is dict:
                descriptions.extend(actual_row['components'].values())
            for description in descriptions:
                if type(description) is not dict:continue
                records=[description.get('root')]
                if type(description.get('references')) is list:records.extend(description['references'])
                for record in records:
                    if type(record) is not dict or type(record.get('fields')) is not list:continue
                    for actual_field in record['fields']:
                        if type(actual_field) is not dict or 'default_value' not in actual_field:
                            requested=True
                        else:
                            default=actual_field['default_value']
                            requested|=type(default) is not str or default!=''
    if not requested:return None
    if (catalog.get('format')=='uwvm-ros-official-standard-bounded-v1' and
        all(row.get('ros_version')==2 and row['file'].endswith('.msg') for row in rows) and
        'geometry_msgs/msg/Quaternion' in sources and
        default_records<= {'geometry_msgs/msg/Quaternion'}):
        return quaternion_native_default_contract(catalog,definitions,metadata)
    require(catalog.get('format') in ('uwvm-ros-custom-bounded-v2','uwvm-ros-official-standard-bounded-v1'),
        'actual bounded source format required')
    require(all(type(row.get('ros_version')) is int and row['ros_version']==2 for row in rows),
        'declared defaults require the actual modern ROS2 path')
    require(type(metadata) is dict and set(metadata)=={'format','ros_version','types'} and
        metadata['format']=='uwvm-ros-official-bounded-native-v2' and type(metadata['ros_version']) is int and
        metadata['ros_version']==2 and type(metadata['types']) is dict and set(metadata['types'])==set(sources),
        'exact original modern exported inventory required')
    scalar_ids={'int8':2,'uint8':3,'int16':4,'uint16':5,'int32':6,'uint32':7,
        'int64':8,'uint64':9,'float32':10,'float64':11,'bool':15,'string':17}
    def normalize(type_,literal):
        require(type(literal) is str and 0<len(literal)<=128,'bounded default literal required')
        if type_=='bool':
            require(literal.lower() in ('true','false','0','1'),'original bool default required')
            return 'True' if literal.lower() in ('true','1') else 'False'
        if re.fullmatch(r'u?int(?:8|16|32|64)',type_):
            require(re.fullmatch(r'[+-]?(?:[0-9]+|0[xX][0-9a-fA-F]+|0[oO][0-7]+|0[bB][01]+)',literal) is not None,
                'exact integer default required')
            try:value=int(literal,10)
            except ValueError:value=int(literal,0)
            unsigned=type_.startswith('u');bits=int(type_[4 if unsigned else 3:])
            require((0 if unsigned else -(1<<(bits-1)))<=value<=(1<<(bits if unsigned else bits-1))-1,
                'default exceeds original integer width')
            return str(value)
        if type_ in ('float32','float64'):
            require(re.fullmatch(r'[+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eE][+-]?[0-9]+)?',literal) is not None,
                'finite decimal floating default required')
            value=float(literal);require(math.isfinite(value),'nonfinite scalar default refused')
            if type_=='float32':
                try:rounded=struct.unpack('<f',struct.pack('<f',value))[0]
                except (OverflowError,struct.error):raise ValueError('native default contract: float32 overflow') from None
                require(math.isfinite(rounded),'float32 overflow')
            return str(value)
        raise ValueError('native default contract: array/string/nested default grammar required')
    declared={};interfaces={};defaults={}
    def field(name,type_,capacity=0,sequence=False):
        scalar=scalar_ids.get(type_,1)
        if capacity:scalar+=96 if sequence else 48
        return {'name':name,'type_id':scalar,'capacity':capacity,'string_capacity':0,
            'nested_type_name':'' if type_ in scalar_ids else type_,'default_value':''}
    def install(name,fields):
        value={'type':name,'fields':fields}
        require(name not in declared or declared[name]==value,'original/protocol declarations conflict')
        declared[name]=value
    def parse_component(name,lines,package):
        fields=[];values={}
        for line in lines:
            match=re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:<=([0-9]+))?(?:\[([0-9]*|<=[0-9]+)\])?\s+([A-Za-z][A-Za-z0-9_]*)(?:\s+([^\s]+))?',line)
            require(match is not None,'unknown field/default/constant syntax refused')
            typ,text_bound,array,label,literal=match.groups()
            require(typ not in ('byte','char','wstring','time','duration','Header'),'ambiguous/wide primitive refused')
            native=typ if typ in scalar_ids else typ if typ.count('/')==2 else typ.replace('/','/msg/') if '/' in typ else package+'/msg/'+typ
            f=field(label,native)
            if text_bound is not None:
                require(typ=='string' and len(text_bound)<=5 and 1<=int(text_bound)<=32764,'original string bound required')
                f['type_id']=21;f['string_capacity']=int(text_bound)
            if array is not None:
                digits=array[2:] if array.startswith('<=') else array
                require(not digits or (len(digits)<=4 and 1<=int(digits)<=4096),'original array bound required')
                f['type_id']+=144 if array=='' else 96 if array.startswith('<=') else 48
                f['capacity']=int(digits) if digits else 0
            if literal is not None:
                require(array is None and text_bound is None,'only exact scalar defaults implemented')
                f['default_value']=normalize(typ,literal);values[label]=f['default_value']
            fields.append(f)
        require(len(fields)<=4096 and len({f['name'] for f in fields})==len(fields),'whole unique field inventory required')
        if not fields:fields=[field('structure_needs_at_least_one_member','uint8')]
        install(name,fields)
        if values:defaults[name]=values
    for row in rows:
        name=row['type'];suffix=PurePosixPath(row['file']).suffix
        kind={'.msg':'msg','.srv':'srv','.action':'action'}.get(suffix)
        require(kind is not None and re.fullmatch(r'[a-z][a-z0-9_]*/'+kind+r'/[A-Z][A-Za-z0-9_]*',name) is not None,
            'original ROS2 interface name/category required')
        parts=[[]]
        for line in sources[name]:
            if line=='---':parts.append([])
            else:parts[-1].append(line)
        roles={'msg':('message',),'srv':('request','response'),'action':('goal','result','feedback')}[kind]
        require(len(parts)==len(roles),'exact original component separator inventory required')
        components={role:name if role=='message' else name+'_'+role.title() for role in roles}
        for role,lines in zip(roles,parts):parse_component(components[role],lines,name.split('/')[0])
        interfaces[name]=(kind,components)
    def service(name,request,response):
        install(name,[field('request_message',request),field('response_message',response),field('event_message',name+'_Event')])
        install(name+'_Event',[field('info','service_msgs/msg/ServiceEventInfo'),field('request',request,1,True),field('response',response,1,True)])
    for name,(kind,c) in interfaces.items():
        if kind=='msg':continue
        install('builtin_interfaces/msg/Time',[field('sec','int32'),field('nanosec','uint32')])
        install('service_msgs/msg/ServiceEventInfo',[field('event_type','uint8'),field('stamp','builtin_interfaces/msg/Time'),
            field('client_gid','uint8',16),field('sequence_number','int64')])
        if kind=='srv':service(name,c['request'],c['response'])
        else:
            install('unique_identifier_msgs/msg/UUID',[field('uuid','uint8',16)])
            send,get=name+'_SendGoal',name+'_GetResult'
            install(send+'_Request',[field('goal_id','unique_identifier_msgs/msg/UUID'),field('goal',c['goal'])])
            install(send+'_Response',[field('accepted','bool'),field('stamp','builtin_interfaces/msg/Time')])
            install(get+'_Request',[field('goal_id','unique_identifier_msgs/msg/UUID')])
            install(get+'_Response',[field('status','int8'),field('result',c['result'])])
            service(send,send+'_Request',send+'_Response');service(get,get+'_Request',get+'_Response')
            install(name+'_FeedbackMessage',[field('goal_id','unique_identifier_msgs/msg/UUID'),field('feedback',c['feedback'])])
            install(name,[field('goal',c['goal']),field('result',c['result']),field('feedback',c['feedback']),
                field('send_goal_service',send),field('get_result_service',get),field('feedback_message',name+'_FeedbackMessage')])
    # Preserve the actual supported official Quaternion variant even when it is
    # nested inside a new default-bearing Service/Action, rather than erasing it.
    quat='geometry_msgs/msg/Quaternion'
    if quat in declared:
        require(declared[quat]['fields']==[
            dict(field(label,'float64'),default_value=value)
            for label,value in (('x','0.0'),('y','0.0'),('z','0.0'),('w','1.0'))],
            'complete selected original Quaternion defaults required')
    proofs={}
    def verify(actual,name):
        require(type(actual) is dict and set(actual)=={'native_identity','root','references'} and
            type(actual['native_identity']) is str and re.fullmatch(r'RIHS01_[0-9a-f]{64}',actual['native_identity']) is not None,
            'actual modern complete metadata required; legacy fallback refused')
        reachable=set();active=set();heights={}
        def visit(current,depth=1):
            require(depth<=16 and current not in active and current in declared,'whole acyclic original dependency required')
            if current in reachable:
                require(depth+heights[current]-1<=16,'cached graph exceeds depth16');return heights[current]
            active.add(current);height=1
            for f in declared[current]['fields']:
                if f['nested_type_name']:height=max(height,visit(f['nested_type_name'],depth+1)+1)
            require(depth+height-1<=16,'complete graph exceeds depth16')
            active.remove(current);reachable.add(current);heights[current]=height;return height
        visit(name)
        require(len(reachable)<=256 and type(actual['references']) is list and len(actual['references'])==len(reachable)-1,
            'complete reference inventory required')
        refs={}
        for record in [actual['root'],*actual['references']]:
            require(type(record) is dict and set(record)=={'type','fields'} and type(record['type']) is str and
                type(record['fields']) is list and record['type'] not in refs,'typed unique native record required')
            refs[record['type']]=record
            for f in record['fields']:
                require(type(f) is dict and set(f)=={'name','type_id','capacity','string_capacity','nested_type_name','default_value'} and
                    all(type(f[k]) is int for k in ('type_id','capacity','string_capacity')) and
                    all(type(f[k]) is str for k in ('name','nested_type_name','default_value')),'closed exact typed native field required')
        require(type(actual['root']) is dict and actual['root']['type']==name and set(refs)==reachable and
            all(refs[key]==declared[key] for key in reachable),'native full field/order/capacity/default graph differs')
        normalized={'root':refs[name],'references':[refs[key] for key in sorted(reachable-{name})]}
        return {'native_identity':actual['native_identity'],
            'complete_description_sha256':hashlib.sha256(json.dumps(normalized,sort_keys=True,separators=(',',':')).encode()).hexdigest()}
    for name,(kind,c) in interfaces.items():
        row=metadata['types'][name]
        require(type(row) is dict and set(row)=={'official','components'} and type(row['components']) is dict and set(row['components'])==set(c),
            'exact original interface/component roles required')
        root=verify(row['official'],name);components={role:verify(row['components'][role],component) for role,component in c.items()}
        if kind=='msg':require(row['official']==row['components']['message'],'actual Message root/component disagree')
        proofs[name]={'kind':kind,'official':root,'components':components}
    return {'format_version':1,'kind':'complete-modern-scalar-default-contract-v1','declared_defaults':defaults,
        'complete_interfaces':proofs,'native_RIHS_binds_defaults':False,'native_execution_qualified':False,'implicit_permissions':[]}


def quaternion_native_default_contract(catalog, definitions, metadata):
    """Match the finite declared-default Message closure to actual metadata.

    This is DATA validation, not a native exporter or a RIHS calculation. Keep
    this pure body identical in the direct binder and injected source planner.
    No-default catalogs keep the old receipt domain. The first default family
    is exactly original ROS2 Quaternion (x/y/z0,w1); defaults in other types,
    Services/Actions or legacy derived metadata are deliberately refused.
    """
    def require(ok, why):
        if not ok:raise ValueError('native default contract: '+why)
    rows=catalog.get('types',[])
    if not isinstance(rows,list):raise ValueError('native default catalog inventory required')
    source={}
    requested=False
    for row in rows:
        require(isinstance(row,dict) and isinstance(row.get('file'),str),'explicit definition role required')
        raw=definitions.get(row['file'])
        require(type(raw) is bytes and len(raw)<=1024*1024,'complete bounded original source required')
        text=raw.decode('utf-8')
        lines=[line.split('#',1)[0].strip() for line in text.splitlines()]
        lines=[line for line in lines if line]
        source[row.get('type')]=lines
        requested|=row.get('ros_version')==2 and row.get('type')=='geometry_msgs/msg/Quaternion'
        requested|=any('=' in line.replace('<=','') or len(line.split())>2 for line in lines)
    if not requested:return None
    require(catalog.get('format')=='uwvm-ros-official-standard-bounded-v1','explicit standard mode required')
    require(1<=len(rows)<=128 and len(source)==len(rows),'unique finite Message inventory required')
    require(all(row.get('ros_version')==2 and row['file'].endswith('.msg') for row in rows),
        'first default family requires a complete single-backend Message catalog')
    require(type(metadata) is dict and set(metadata)=={'format','ros_version','types'} and
        metadata['format']=='uwvm-ros-official-bounded-native-v2' and type(metadata['ros_version']) is int and
        metadata['ros_version']==2 and type(metadata['types']) is dict and set(metadata['types'])==set(source),
        'exact modern original metadata inventory required')
    # Official rosidl_generator_type_description FIELD_TYPE_* IDs. These are
    # native scalar/capacity IDs, never the application's canonical quotas.
    scalars={'int8':2,'uint8':3,'int16':4,'uint16':5,'int32':6,'uint32':7,
        'int64':8,'uint64':9,'float32':10,'float64':11,'bool':15,'string':17}
    quat='geometry_msgs/msg/Quaternion'
    declared={};dependencies={}
    for name,lines in source.items():
        require(type(name) is str and re.fullmatch(r'[a-z][a-z0-9_]*/msg/[A-Z][A-Za-z0-9_]*',name) is not None,
            'actual ROS2 Message name required')
        fields=[];nested=[]
        for line in lines:
            match=re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:<=([0-9]+))?(?:\[([0-9]*|<=[0-9]+)\])?\s+([A-Za-z][A-Za-z0-9_]*)(?:\s+([01](?:\.0)?))?',line)
            require(match is not None,'unknown default/constant/field syntax refused')
            typ,text_bound,array,field,literal=match.groups()
            require(typ not in ('byte','char','wstring','time','duration','Header'),'ambiguous primitive/alias refused')
            string_capacity=0
            if text_bound is not None:
                require(typ=='string' and len(text_bound)<=5 and 1<=int(text_bound)<=32764,'finite native string bound required')
                string_capacity=int(text_bound)
            native_type='' if typ in scalars else (typ if typ.count('/')==2 else typ.replace('/','/msg/') if '/' in typ else name.split('/')[0]+'/msg/'+typ)
            if native_type:
                require(native_type in source,'complete declared nested dependency required');nested.append(native_type)
            type_id=(21 if string_capacity else scalars.get(typ,1));capacity=0
            if array is not None:
                digits=array[2:] if array.startswith('<=') else array
                require(not digits or (len(digits)<=4 and 1<=int(digits)<=4096),'finite native array bound required')
                if array=='':type_id+=144
                elif array.startswith('<='):type_id+=96;capacity=int(digits)
                else:type_id+=48;capacity=int(digits)
            default=''
            if literal is not None:
                expected={'x':'0.0','y':'0.0','z':'0.0','w':'1.0'}
                require(name==quat and typ=='float64' and text_bound is None and array is None and
                    field in expected and literal in (expected[field],expected[field][0]),'only complete official Quaternion defaults supported')
                default=expected[field]
            fields.append({'name':field,'type_id':type_id,'capacity':capacity,'string_capacity':string_capacity,
                'nested_type_name':native_type,'default_value':default})
        require(0<len(fields)<=4096 and len({f['name'] for f in fields})==len(fields),'whole nonempty field inventory required')
        declared[name]={'type':name,'fields':fields};dependencies[name]=nested
    require(quat in declared and declared[quat]['fields']==[
        {'name':field,'type_id':11,'capacity':0,'string_capacity':0,'nested_type_name':'','default_value':default}
        for field,default in (('x','0.0'),('y','0.0'),('z','0.0'),('w','1.0'))],
        'complete original Quaternion variant required; erased/partial defaults refused')
    proofs={}
    for name in source:
        reachable=set();active=set();heights={}
        def visit(current,depth=1):
            require(depth<=16 and current not in active,'recursive/excess-depth native graph refused')
            if current in reachable:
                require(depth+heights[current]-1<=16,'cached native dependency exceeds depth16');return heights[current]
            active.add(current);height=1
            for child in dependencies[current]:height=max(height,visit(child,depth+1)+1)
            require(depth+height-1<=16,'complete native dependency exceeds depth16')
            active.remove(current);reachable.add(current);heights[current]=height;return height
        visit(name)
        expected={'root':declared[name],'references':[declared[key] for key in sorted(reachable-{name})]}
        row=metadata['types'][name]
        require(type(row) is dict and set(row)=={'official','components'} and type(row['components']) is dict and
            set(row['components'])=={'message'},'actual complete Message roles required')
        actual=row['official']
        require(type(actual) is dict and set(actual)=={'native_identity','root','references'} and
            type(actual['native_identity']) is str and re.fullmatch(r'RIHS01_[0-9a-f]{64}',actual['native_identity']) is not None,
            'modern full TypeDescription required; legacy-derived metadata refused')
        require(actual==row['components']['message'],'actual root/component full metadata differs')
        require(type(actual['references']) is list and len(actual['references'])<=127 and
            all(type(item) is dict and type(item.get('type')) is str for item in actual['references']),
            'complete actual referenced type descriptions required')
        refs={item['type']:item for item in actual['references']}
        require(len(refs)==len(actual['references']) and set(refs)==reachable-{name},'exact reachable metadata closure required')
        # Python bool compares equal to int: explicitly reject it (and unknown
        # keys) before the complete exact field/value/default comparison.
        for record in [actual['root'],*actual['references']]:
            require(type(record) is dict and set(record)=={'type','fields'} and type(record['fields']) is list,
                'closed native record required')
            for field in record['fields']:
                require(type(field) is dict and set(field)=={'name','type_id','capacity','string_capacity','nested_type_name','default_value'} and
                    all(type(field[key]) is int for key in ('type_id','capacity','string_capacity')) and
                    all(type(field[key]) is str for key in ('name','nested_type_name','default_value')),
                    'typed complete native field required')
        normalized={'root':actual['root'],'references':[refs[key] for key in sorted(refs)]}
        require(normalized==expected,'complete native field/order/capacity/default text differs')
        proofs[name]={'native_identity':actual['native_identity'],
            'complete_description_sha256':hashlib.sha256(json.dumps(normalized,sort_keys=True,separators=(',',':')).encode()).hexdigest()}
    return {'format_version':1,'kind':'complete-modern-quaternion-default-contract-v1',
        'declared_defaults':{quat:{'x':'0.0','y':'0.0','z':'0.0','w':'1.0'}},
        'complete_messages':proofs,'native_RIHS_binds_defaults':False,'native_execution_qualified':False,'implicit_permissions':[]}

def action_client_roles(request):
    """Name actual caller-selected aliases; never infer or open a capability.

    Four independent roles are mandatory. Cancel-any is optional and remains
    stronger than cancel-own: absence never aliases/falls back to another role.
    Sending an Action may actuate a robot; source planning grants no authority
    and does not establish model/controller operational safety.
    """
    aliases=request['action_aliases'];roles={'send_goal':'action.send-goal','cancel_own':'action.cancel-own-goal',
        'feedback':'action.receive-feedback','result':'action.receive-result'}
    need(type(aliases) is dict and set(roles)<=set(aliases)<=set(roles)|{'cancel_any'},
        'four explicit independent Action client aliases required')
    need(all(type(value) is str and re.fullmatch('[A-Za-z0-9][A-Za-z0-9_.-]{0,63}',value) is not None for value in aliases.values()) and
        len(set(aliases.values()))==len(aliases) and request['alias']==aliases['send_goal'],
        'distinct Action aliases; primary alias must be selected send-goal role')
    if 'cancel_any' in aliases:roles['cancel_any']='action.cancel-any-goal'
    return dict(aliases),roles

def managed_source_plan(read, selection, request, native_generation, native_record):
    """Bind actual optional outputs and dependency SOURCE bytes, never run them.

    The separate native generation/binding is still mandatory in plan(). The
    optional producer explicitly emits NO native registry. Its syntax/receipts
    cannot promote native identities, a compiler, a runtime or delivery to PASS.
    """
    need(type(selection) is dict and set(selection)=={
        'languages','generation','generated_files','source_files','producer_inputs'},
        'closed explicit managed source selection required')
    languages=selection['languages']
    need(type(languages) is list and 1<=len(languages)<=3 and
        all(type(value) is str and value in MANAGED_LANGUAGES for value in languages) and
        len(set(languages))==len(languages), 'unique implemented managed languages; MoonBit remains pending')
    need(request['mode']=='telemetry' and request['alias']=='joint-state' and
        request['type']==('sensor_msgs/JointState' if request['ros_version']==1 else 'sensor_msgs/msg/JointState') and
        request['poll_limit']<=64, 'this managed lesson covers only complete JointState telemetry with64 credits')
    generated=unique_json(body(read,'managed-generation',selection['generation']))
    need(type(generated) is dict and generated.get('kind')=='optional-managed-bounded-record-sources-v1' and
        generated.get('format_version')==1 and generated.get('status')=='GENERATED_OPTIONAL_RECORD_SOURCE_ONLY' and
        generated.get('languages')==languages and generated.get('native_registry_emitted') is False and
        generated.get('implicit_permissions')==[] and generated.get('RPC_facades_emitted') is False and
        generated.get('historical_six_record_emitters_called') is False and
        generated.get('native_identity_syntax_required') is True and
        generated.get('native_qualification')=='NOT_PERFORMED' and
        generated.get('compiler_execution')=='NOT_RUN' and generated.get('native_execution')=='NOT_RUN',
        'actual optional emitter source receipt required, not a compiler/native qualification')
    need(generated.get('source_inputs')==native_generation.get('source_inputs'),
        'managed and native products must use the SAME actual bound catalog/complete definitions')
    actual_records=generated.get('records');native_records=native_generation.get('records')
    need(type(actual_records) is list and type(native_records) is list and
        len(actual_records)==len(native_records) and
        all(type(row) is dict for row in actual_records), 'whole optional record closure required')
    fields=('ros_version','type','native_identity','adapter_codec_identity',
            'minimum_wire_bytes','maximum_wire_bytes','expanded_nodes')
    index={(row.get('ros_version'),row.get('type')):row for row in actual_records}
    need(len(index)==len(actual_records), 'duplicate managed record')
    for row in native_records:
        actual=index.get((row.get('ros_version'),row.get('type')))
        need(actual is not None and all(actual.get(key)==row.get(key) for key in fields),
            'whole managed/native record identity and complete bound differ')
    selected=index[(request['ros_version'],request['type'])]
    symbol='Ros1Sensor_msgsJointState' if request['ros_version']==1 else 'Ros2Sensor_msgsMsgJointState'
    need(selected.get('symbol')==symbol and
        selected.get('complete_value_semantics')=='joint-state-optional-equal-length-arrays' and
        selected.get('maximum_wire_bytes')==native_record['maximum_wire_bytes'],
        'actual complete generated JointState symbol/semantics required')
    if 'csharp' in languages:
        need(generated.get('csharp_source_types',{}).get(symbol)=={
            'value':'_'+symbol,'codec':symbol+'Codec',
            'namespace':'Uwvm.RosWasm.Generated.BoundedRecords'}, 'actual C# source API association differs')
    outputs=generated.get('outputs');files=selection['generated_files']
    need(type(files) is dict and type(outputs) is dict and
        set(files)==set(outputs)=={MANAGED_OUTPUTS[lang] for lang in languages},
        'exact whole optional output bundle required')
    for name,pin in files.items():
        relative(name);raw=body(read,'managed-output:'+name,pin)
        need(outputs[name]=={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()},
            'actual optional output bytes differ from its source receipt')
    sources=selection['source_files'];expected=dict(MANAGED_FOUNDATIONS)
    required={'sdk/robots/vendors/adapter_plan.py',_LESSON+'observer-fixture.json',
              _LESSON+'managed-recipe.json',_LESSON+'README_MANAGED.md'}
    for language in languages:
        expected.update(MANAGED_LANGUAGE_FOUNDATIONS[language])
        required.update(MANAGED_SDK_SOURCES[language])
        required.update(_LESSON+name for name in MANAGED_LESSON_SOURCES[language])
    required.update(expected.values())
    need(type(sources) is dict and set(sources)==required, 'exact actual managed SDK/lesson/producer source closure required')
    for name,pin in sources.items():
        relative(name);body(read,'managed-source:'+name,pin)
    foundations=generated.get('generator_and_foundation_sources')
    need(type(foundations) is dict and set(foundations)==set(expected), 'whole selected generator/foundation closure required')
    for role,name in expected.items():
        pin=sources[name]
        need(foundations[role]=={key:pin[key] for key in ('path','bytes','sha256')},
            'optional emitter foundation must be the actual selected SDK source bytes')
    producer_inputs=selection['producer_inputs']
    need(type(producer_inputs) is dict and set(producer_inputs)==set(languages),
        'separate actual producer dependencies per selected managed language required')
    for language,inputs in producer_inputs.items():
        need(type(inputs) is dict and set(inputs)=={'producer_source','toolchain_receipt','dependency_inventory'},
            'actual producer source/receipt/dependency inventory roles required')
        for role,pin in inputs.items():
            raw=body(read,'managed-producer:'+language+':'+role,pin)
            if role!='producer_source':
                need(type(unique_json(raw)) is dict, 'actual bounded producer dependency document required')
    return {'status':'SOURCE_ONLY_UNEXECUTED','languages':languages,
        'generation':selection['generation'],'whole_generated_files':files,
        'source_files':sources,'producer_inputs':producer_inputs,
        'source_dependency_scope':'Selected source and receipt/inventory bytes only; actual compiler tool/library closure, linkage and execution remain unqualified',
        'actual_compiles':0,'actual_Guest_runs':0,'actual_native_deliveries':0,
        'default_authority':[],'poll_limit':request['poll_limit'],'moonbit_emitter':'PENDING_NOT_IMPLEMENTED'}


def moonbit_source_plan(read, selection, request, native_generation, native_record):
    """Bind the separate real MoonBit producer; never run it or a Guest."""
    need(type(selection) is dict and set(selection)=={
        'generation','generated_files','source_files','producer_inputs'}, 'closed explicit MoonBit selection required')
    need(request['mode']=='telemetry' and request['alias']=='joint-state' and
        request['type']==('sensor_msgs/JointState' if request['ros_version']==1 else 'sensor_msgs/msg/JointState') and
        request['poll_limit']<=64, 'MoonBit lesson is full JointState telemetry with64 credits')
    proof=unique_json(body(read,'moonbit-generation',selection['generation']))
    need(type(proof) is dict and proof.get('kind')=='optional-moonbit-bounded-record-sources-v1' and
        proof.get('status')=='GENERATED_OPTIONAL_RECORD_SOURCE_ONLY' and proof.get('language')=='moonbit' and
        proof.get('native_registry_emitted') is False and proof.get('native_qualification')=='NOT_PERFORMED' and
        proof.get('implicit_permissions')==[] and proof.get('RPC_facades_emitted') is False and
        proof.get('historical_emitters_called') is False and proof.get('native_identity_syntax_required') is True and
        proof.get('compiler_execution')=='NOT_RUN' and proof.get('native_execution')=='NOT_RUN',
        'actual explicit MoonBit source emission receipt required, no native/compiler qualification')
    need(proof.get('source_inputs')==native_generation.get('source_inputs'),
        'same actual bound catalog and complete definition bytes required')
    records=proof.get('records');native_records=native_generation.get('records')
    need(type(records) is list and type(native_records) is list and len(records)==len(native_records) and
        all(type(row) is dict for row in records), 'whole MoonBit record closure required')
    by_name={(row.get('ros_version'),row.get('type')):row for row in records}
    need(len(by_name)==len(records), 'duplicate MoonBit record')
    keys=('ros_version','type','native_identity','adapter_codec_identity','minimum_wire_bytes','maximum_wire_bytes','expanded_nodes')
    for row in native_records:
        emitted=by_name.get((row.get('ros_version'),row.get('type')))
        need(emitted is not None and all(emitted.get(key)==row.get(key) for key in keys),
            'actual complete MoonBit/native record identity or bound differs')
    selected=by_name[(request['ros_version'],request['type'])]
    symbol='Ros1Sensor_msgsJointState' if request['ros_version']==1 else 'Ros2Sensor_msgsMsgJointState'
    need(selected.get('symbol')==symbol and
        selected.get('complete_value_semantics')=='joint-state-optional-equal-length-arrays' and
        selected.get('maximum_wire_bytes')==native_record['maximum_wire_bytes'] and
        selected.get('moonbit_source_types')=={'value':symbol,'codec':symbol+'Codec',
            'constructor':'new','metadata':'max_wire_bytes/min_wire_bytes/native_identity/codec_identity/ros_version/native_type'},
        'actual complete MoonBit typed Codec API association required')
    files=selection['generated_files'];outputs=proof.get('outputs')
    need(type(files) is dict and type(outputs) is dict and
        set(files)==set(outputs)=={'moonbit/bounded_records.mbt','moonbit/moon.pkg'},
        'whole independent MoonBit output set required')
    for name,pin in files.items():
        raw=body(read,'moonbit-output:'+name,pin)
        need(outputs[name]=={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}, 'MoonBit output bytes differ')
    expected={'moonbit_generator':'sdk/custom-types/moonbit_profile.py',
        'moonbit_runtime':'sdk/custom-types/moonbit_runtime.mbt.in',
        'schema_generator':'sdk/tools/custom_types.py','bounded_generator':'sdk/custom-types/bounded_profile.py',
        'moonbit_wire':'sdk/moonbit/wire/wire.mbt','moonbit_wire_package':'sdk/moonbit/wire/moon.pkg'}
    required=set(expected.values())|{'sdk/robots/vendors/adapter_plan.py',
        _LESSON+'observer-fixture.json',_LESSON+'moonbit-recipe.json',_LESSON+'README_MOONBIT.md',
        'sdk/moonbit/moon.mod','sdk/moonbit/producer-contract.json',
        'sdk/moonbit/linear/linear.mbt','sdk/moonbit/linear/moon.pkg',
        'sdk/examples/moonbit/stdio/write.mbt','sdk/examples/moonbit/stdio/moon.pkg'}
    required.update('sdk/moonbit/ros'+str(ros)+'/'+name for ros in (1,2)
        for name in ('client.mbt','raw.mbt','coverage.mbt','moon.pkg'))
    required.update(_LESSON+'moonbit/'+name for name in (
        'main.mbt','observe_ros1.mbt','observe_ros2.mbt','mode_default.mbt','mode_positive.mbt',
        'read.mbt','moon.mod','moon.work','guest_ros1.pkg','guest_ros2.pkg'))
    sources=selection['source_files']
    need(type(sources) is dict and set(sources)==required, 'exact MoonBit producer/SDK/lesson source dependency roles required')
    for name,pin in sources.items():
        relative(name);body(read,'moonbit-source:'+name,pin)
    foundations=proof.get('generator_and_foundation_sources')
    need(type(foundations) is dict and set(foundations)==set(expected), 'exact MoonBit emitter foundations required')
    for role,name in expected.items():
        need(foundations[role]=={key:sources[name][key] for key in ('path','bytes','sha256')},
            'MoonBit actual source/foundation association differs')
    inputs=selection['producer_inputs']
    need(type(inputs) is dict and set(inputs)=={
        'producer_source','toolchain_receipt','dependency_inventory','core_archive'},
        'actual MoonBit producer/toolchain/dependency/core roles required')
    for role,pin in inputs.items():
        raw=body(read,'moonbit-producer:'+role,pin)
        if role in ('toolchain_receipt','dependency_inventory'):
            need(type(unique_json(raw)) is dict, 'actual bounded MoonBit producer document required')
    contract=unique_json(body(read,'moonbit-producer-contract',sources['sdk/moonbit/producer-contract.json']))
    need(inputs['core_archive']['sha256']==contract.get('compiler',{}).get('core_sha256'),
        'actual selected core archive must match the source-pinned SDK contract')
    return {'status':'SOURCE_ONLY_UNEXECUTED','language':'moonbit','generation':selection['generation'],
        'whole_generated_files':files,'source_files':sources,'producer_inputs':inputs,
        'source_dependency_scope':'Actual source/output/producer-input bytes only; real final compiler/layout/import/native delivery and performance remain unqualified',
        'default_authority':[],'poll_limit':request['poll_limit'],
        'actual_compiles':0,'actual_Guest_runs':0,'actual_native_deliveries':0}


def plan(inventory,request,*,read,managed=None,moonbit=None):
    """Return finite DATA requirements; no aliases are opened or granted.

    Native traits, upstream revision, firmware and ROS major are separate input
    facts. A valid generated source record alone cannot qualify native transport.
    Even a returned plan carries all actual execution booleans False.
    """
    need(reviewed_inventory(inventory), 'reviewed vendor19 or additive vendor28 inventory required')
    need(type(request) is dict and set(request)=={'format_version','vendor','model','ros_version',
        'sdk_revision','upstream_lock','source_binding','generation','generated_files','type','resource','alias',
        'native_identity','codec_identity','default_authority','poll_limit','mode','command_binding'} |
        ({'action_aliases'} if request.get('mode')=='action-client' else set()),
        'closed explicit adapter request required')
    need(type(request['format_version']) is int and request['format_version']==1,'adapter format1')
    vendors=[row for row in inventory['vendors'] if row['id']==request['vendor']]
    need(len(vendors)==1,'one known vendor, never a guessed fallback')
    vendor=vendors[0]
    need(type(request['model']) is str and 1<=len(request['model'])<=128 and '\x00' not in request['model'],
        'explicit model required, independent of vendor')
    need(type(request['ros_version']) is int and request['ros_version'] in (1,2),'one explicit ROS major')
    backend=vendor['ros'+str(request['ros_version'])]
    need(not backend.startswith(('NOT_ESTABLISHED','EXPLICITLY_NOT_SUPPORTED')),
        'this reviewed primary-source inventory does not establish the selected backend')
    need(type(request['sdk_revision']) is str and 1<=len(request['sdk_revision'])<=128,
        'actual controller/RDK/FCI/SDK revision is separate from ROS major')
    need(request['default_authority']==[],'no implicit vendor or topic authority')
    need(type(request['alias']) is str and re.fullmatch('[A-Za-z0-9][A-Za-z0-9_.-]{0,63}',request['alias']) is not None,
        'one bounded alias; it is not a module principal')
    need(type(request['resource']) is str and request['resource'].startswith('/') and
        1<len(request['resource'])<=255 and '\x00' not in request['resource'],
        'actual selected resource is required; no vendor-wide wildcard')
    need(type(request['poll_limit']) is int and 1<=request['poll_limit']<=4096,'finite caller-driven poll count')
    need(request['mode'] in ('telemetry','simulator-hold-stop','service-call','action-client'),'explicit example mode')
    lock=unique_json(body(read,'upstream-lock',request['upstream_lock']))
    need(type(lock) is dict and set(lock)=={'format_version','repository','commit','tree_sha256','model','sdk_revision',
        'definitions','catalog','license','source_review'},'closed separately reviewed source lock')
    need(lock['format_version']==1 and lock['source_review']=='CLOSED_SOURCE_REVIEW_NOT_EXECUTION' and
        lock['repository'] in vendor['official_primary_sources'] and
        re.fullmatch('[0-9a-f]{40}',str(lock['commit'])) is not None and
        re.fullmatch('[0-9a-f]{64}',str(lock['tree_sha256'])) is not None and
        lock['model']==request['model'] and lock['sdk_revision']==request['sdk_revision'],
        'actual reviewed source-lock association, not a moving default branch')
    need(type(lock['definitions']) is list and lock['definitions'],'complete source definition/dependency closure required')
    seen=set();definition_bodies={};definition_sources={}
    for row in lock['definitions']:
        need(type(row) is dict and set(row)=={'relative','generation_file','pin'},'closed upstream definition role')
        relative(row['relative']);need(row['relative'] not in seen,'duplicate definition role')
        seen.add(row['relative']);relative(row['generation_file'])
        need(row['generation_file'] not in definition_bodies,'duplicate generation source role')
        raw=body(read,'definition:'+row['relative'],row['pin'])
        definition_bodies[row['generation_file']]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
        definition_sources[row['generation_file']]=raw
    body(read,'upstream-license',lock['license'])
    catalog_body=body(read,'upstream-catalog',lock['catalog'])
    binding=unique_json(body(read,'source-binding',request['source_binding']))
    need(type(binding) is dict and binding.get('kind')=='vendor-native-bound-six-language-source-v1' and
        binding.get('status')=='PREPARED_WITH_CALLER_EXPORTED_IDENTITIES_NOT_NATIVE_QUALIFIED' and
        binding.get('vendor')==request['vendor'] and binding.get('model')==request['model'] and
        binding.get('sdk_revision')==request['sdk_revision'] and binding.get('ros_version')==request['ros_version'] and
        binding.get('default_authority')==[] and binding.get('generation')==request['generation'] and
        binding.get('generated_files')==request['generated_files'], 'actual source-binding/generation association differs')
    preparation=unique_json(body(read,'source-preparation',binding['preparation']))
    need(preparation.get('kind')=='vendor-complete-schema-source-preparation-v1' and
        preparation.get('status')=='PREPARED_GUEST_SOURCE_AND_NATIVE_EXPORTER_NOT_NATIVE_QUALIFIED' and
        preparation['request']['upstream_lock']==request['upstream_lock'],
        'source preparation must retain the actual original source lock')
    identity_mode=preparation['request'].get('identity_mode','original-native-metadata')
    need(identity_mode in ('original-native-metadata','legacy-introspection') and
        preparation.get('native_identity_mode','original-native-metadata')==identity_mode and
        binding.get('native_identity_mode','original-native-metadata')==identity_mode,
        'preparation/binding native identity mode differs')
    native_provenance=None;default_contract=None
    if identity_mode=='legacy-introspection':
        need(request['ros_version']==2, 'legacy native identity mode is ROS2 Message/Service/Action only')
        native_provenance=legacy_provenance(read,preparation,binding)
    elif request['ros_version']==2:
        native_raw=body(read,'original-native-metadata',binding['native_metadata_original'])
        need(body(read,'original-native-metadata-copy',binding['native_metadata_copy'])==native_raw,
            'original native metadata retained copy differs')
        native_document=unique_json(native_raw)
        need(type(native_document) is dict and set(native_document)=={'format','ros_version','types'} and
            native_document['format']=='uwvm-ros-official-bounded-native-v2' and type(native_document['ros_version']) is int and
            native_document['ros_version']==2 and type(native_document['types']) is dict and 1<=len(native_document['types'])<=128,
            'closed original native exporter document required')
        for row in native_document['types'].values():
            need(type(row) is dict and set(row)=={'official','components'} and type(row['components']) is dict and row['components'],
                'closed original native metadata roles required')
            need(all(type(value) is dict and set(value)=={'native_identity','root','references'}
                for value in [row['official'],*row['components'].values()]),
                'original exporter mode cannot use a derived legacy receipt')
    if identity_mode=='original-native-metadata' and request['ros_version']==2:
        default_contract=native_default_contract(unique_json(catalog_body),definition_sources,native_document)
    need(binding.get('binding',{}).get('native_default_contract')==default_contract,
        'retained complete native-default proof differs or is absent')
    bound_catalog=body(read,'actual-bound-catalog',binding['bound_catalog'])
    need({k:binding['bound_catalog'][k] for k in ('bytes','sha256')}==
        {k:binding['binding']['output'][k] for k in ('bytes','sha256')} and
        {k:preparation['files']['input/catalog.json'][k] for k in ('bytes','sha256')}==
        {'bytes':len(catalog_body),'sha256':hashlib.sha256(catalog_body).hexdigest()} and
        {k:binding['binding']['template'][k] for k in ('bytes','sha256')}==
        {'bytes':len(catalog_body),'sha256':hashlib.sha256(catalog_body).hexdigest()},
        'original unbound catalog and separately derived native-bound catalog must remain distinct')
    bound_document=unique_json(bound_catalog)
    need(type(bound_document) is dict and set(bound_document)=={'format','types'} and
        bound_document['format'] in ('uwvm-ros-custom-bounded-v2','uwvm-ros-official-standard-bounded-v1'),
        'actual generated catalog mode required')
    if identity_mode=='legacy-introspection':
        rows=bound_document['types']
        need(type(rows) is list, 'whole bound catalog inventory required')
        if 'message_identities' in native_provenance:
            identities={name:{'kind':'message','native_identity':value,'component_identities':{}}
                for name,value in native_provenance['message_identities'].items()}
        else:identities=native_provenance['types']
        need(len(rows)==len(identities) and len({row['type'] for row in rows})==len(rows), 'whole bound interface inventory differs')
        for row in rows:
            need(type(row) is dict and row.get('ros_version')==2 and row.get('type') in identities,
                'actual complete bound interface row required')
            value=identities[row['type']]
            need(type(row.get('file')) is str and row['file'].endswith({'message':'.msg','service':'.srv','action':'.action'}[value['kind']]) and
                row.get('native_identity')==value['native_identity'], 'bound root identity must equal actual raw exporter')
            if value['kind']!='message':
                need(row.get('component_identities')==value['component_identities'], 'actual bound Service component identities differ')
    service_call=request['mode']=='service-call'
    action_client=request['mode']=='action-client'
    interface_call=service_call or action_client
    aliases,operations=action_client_roles(request) if action_client else (None,None)
    specs=[row for row in bound_document['types'] if row.get('ros_version')==request['ros_version'] and
        row.get('type')==request['type'] and type(row.get('file')) is str and
        row['file'].endswith('.action' if action_client else '.srv' if service_call else '.msg') and row.get('registered',True) is True]
    need(len(specs)==1, 'alias must name an actual registered whole Message/Service/Action, never a component/dependency')
    generation=unique_json(body(read,'generation',request['generation']))
    need(type(generation) is dict and generation.get('native_registry_emitted') is True and
        generation.get('implicit_permissions')==[],'actual bounded native-registry generation required')
    source_inputs=generation.get('source_inputs',{})
    need(source_inputs.get('catalog')=={'bytes':len(bound_catalog),'sha256':hashlib.sha256(bound_catalog).hexdigest()},
        'actual bound profile catalog generation source differs')
    definitions=source_inputs.get('definitions')
    need(type(definitions) is list and all(type(row) is dict and set(row)=={'file','bytes','sha256'} for row in definitions) and
        len({row['file'] for row in definitions})==len(definitions) and
        {row['file']:{k:row[k] for k in ('bytes','sha256')} for row in definitions}==definition_bodies,
        'whole actual generated input closure must equal admitted upstream definitions')
    need(type(binding['bound_definitions']) is dict and set(binding['bound_definitions'])==set(definition_bodies) and
        binding['binding']['definition_copies']==definition_bodies, 'whole copied native-bound definition ledger differs')
    for name,pin in binding['bound_definitions'].items():
        raw=body(read,'actual-bound-definition:'+name,pin)
        need({'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}==definition_bodies[name],
            'bound definition differs from original official byte closure')
    records=[row for row in generation.get('types' if interface_call else 'records',[]) if row.get('ros_version')==request['ros_version'] and row.get('type')==request['type']]
    need(len(records)==1, 'one complete actual generated typed interface required')
    record=records[0]
    need(record['native_identity']==request['native_identity'] and record['adapter_codec_identity']==request['codec_identity'],
        'actual native identity and canonical codec identity must independently match')
    need(re.fullmatch('[0-9a-f]{32}' if request['ros_version']==1 else 'RIHS01_[0-9a-f]{64}',request['native_identity']) is not None,
        'actual separately exported native identity required')
    need(re.fullmatch('UWVMROS1_[0-9a-f]{64}',request['codec_identity']) is not None, 'actual canonical codec identity required')
    components=None
    if interface_call:
        kind='action' if action_client else 'service'
        expected_roles={'goal','result','feedback'} if action_client else {'request','response'}
        need(record.get('kind')==kind and type(record.get('components')) is dict and
            set(record['components'])==expected_roles, 'complete generated native interface components required')
        components=record['components']
        for role,value in components.items():
            need(type(value) is dict and value.get('type')==request['type']+'_'+role.title() and
                type(value.get('minimum_wire_bytes')) is int and type(value.get('maximum_wire_bytes')) is int and
                0<=value['minimum_wire_bytes']<=value['maximum_wire_bytes']<=32768,
                'complete Service component canonical bounds required; zero-byte request is valid')
            component_records=[row for row in generation.get('records',[]) if row.get('ros_version')==request['ros_version'] and row.get('type')==value['type']]
            need(len(component_records)==1 and all(component_records[0].get(key)==value.get(key) for key in
                ('native_identity','adapter_codec_identity','minimum_wire_bytes','maximum_wire_bytes','expanded_nodes')),
                'whole interface/component generated records differ')
            source_identity=specs[0].get('component_identities',{}).get(role)
            need(value.get('native_identity')==source_identity, 'actual bound Service component identity differs')
        maximum=max(value['maximum_wire_bytes'] for value in components.values())
    else:
        maximum=record['maximum_wire_bytes']
        need(type(maximum) is int and 0<maximum<=32768, 'complete bounded telemetry record required')
    outputs=generation.get('outputs');files=request['generated_files']
    need(type(outputs) is dict and type(files) is dict and set(outputs)==set(files) and
        (REQUIRED_OUTPUTS | ({'bounded_interfaces.'+suffix for suffix in ('h','hpp','rs','mjs','ts','go')} if interface_call else set()))<=set(files),'whole generated file set and six language records required')
    need(('native_ros1.hpp' if request['ros_version']==1 else 'native_ros2.hpp') in files,
        'selected actual native adapter source required')
    for name,pin in files.items():
        relative(name);raw=body(read,'generated:'+name,pin)
        need(outputs[name]=={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()},
            'generation output body association differs')
    command=request['command_binding']
    if request['mode'] in ('telemetry','service-call','action-client'):
        need(command is None,'telemetry grants no actuation or native update route')
    else:
        need(type(command) is dict and set(command)=={'environment','model','sdk_revision','binding_source','native_simulator_witness'} and
            command['environment']=='SIMULATOR_ONLY' and command['model']==request['model'] and
            command['sdk_revision']==request['sdk_revision'],'exact simulator-only model command binding required')
        body(read,'simulator-command-binding',command['binding_source'])
        body(read,'native-simulator-witness',command['native_simulator_witness'])
        # This admits source roles for review only. It does not parse a generic
        # JSON true as proof that a simulator or a physical robot really ran.
    result={'status':'UNEXECUTED_SOURCE_PLAN_NO_AUTHORITY','vendor':request['vendor'],'model':request['model'],
        'ros_version':request['ros_version'],'sdk_revision':request['sdk_revision'],'backend_source_status':backend,
        'type':request['type'],'alias':request['alias'],'resource':request['resource'],
        'maximum_wire_bytes':maximum,'native_identity':request['native_identity'],'codec_identity':request['codec_identity'],
        'native_identity_mode':identity_mode,'legacy_identity_provenance':native_provenance,
        'guest_languages':list(LANGUAGES),'default_authority':[],'poll_limit':request['poll_limit'],
        'required_operation':None if action_client else 'service.call' if service_call else 'topic.subscribe','command_binding':command,
        'service_components':components if service_call else None,
        'whole_generated_files':files,'upstream_lock':request['upstream_lock'],
        'source_binding':request['source_binding'],'generation':request['generation'],
        'compiler_executed':False,'Runtime_executed':False,'native_delivery_qualified':False,'hardware_qualified':False,
        'official_cases_run':0,'remaining_validation':['independent actual native traits/version/source lock','six actual compiles and full dependency/link validation',
            'fresh default-denied endpoint/queue witness','actual native complete Action goal/result/feedback/protocol oracle' if action_client else 'actual native Service Request/Response complete-value oracle' if service_call else 'native simulator telemetry and exact canonical-value oracle',
            'close/revoke/disconnect/cancellation evidence',
            'separate real hardware and vendor official tests where explicitly available']}

    if default_contract is not None:result['native_default_contract']=default_contract

    if action_client:
        result.update(action_components=components,action_aliases=aliases,required_operations=operations,
            action_send_may_actuate=True,action_protocol_native_proof=native_provenance,
            required_extra_validation=['separate exact model/controller Action operation authority',
                'actual native SendGoal/GetResult/Feedback/cancel/status and unknown-outcome no-replay witness'])
    if managed is not None:
        result['managed_source_plan']=managed_source_plan(read,managed,request,generation,record)
        result['guest_languages']=list(LANGUAGES)+list(managed['languages'])
        result['remaining_validation'].append('each selected managed actual generated source/producer/native bridge/Wasm/full value and cleanup qualification')
    if moonbit is not None:
        result['moonbit_source_plan']=moonbit_source_plan(read,moonbit,request,generation,record)
        result['guest_languages'].append('moonbit')
        if managed is not None:
            result['managed_source_plan']['moonbit_emitter']='UNCHANGED_MANAGED_REGISTRY_HAS_NONE; EXPLICIT_INDEPENDENT_SOURCE_ROUTE_SELECTED'
        result['remaining_validation'].append('actual independent MoonBit emitter + wasm/wasm-gc compiler/core/arena/import/Guest/native full-value and cleanup qualification')
    return result
