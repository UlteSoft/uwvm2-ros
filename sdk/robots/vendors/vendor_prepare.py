"""Prepare real model schemas, a native identity exporter and six Guest codecs.

This administrative source preparer never launches a compiler, ROS generator,
Gateway, robot or Runtime. The caller supplies current-MF load(relative, name)
for the fixed repository modules (three original, four in explicit legacy mode)
and read(role, full7) for every admitted
ordinary file. Native identities are copied only by the existing bounded-v2
binder from a separately executed official exporter; no MD5/RIHS is fabricated.
Output is fresh Root-owned source, not a registration, grant or test result.
"""
from pathlib import Path, PurePosixPath
import hashlib
import json
import os
import re
import stat

PIN = {'path','bytes','sha256','device','inode','mtime_ns','ctime_ns'}
PROFILE = 'uwvm-ros-custom-bounded-v2'
STANDARD_PROFILE = 'uwvm-ros-official-standard-bounded-v1'
SUPPORT = (
    'sdk/tools/custom_types.py',
    'sdk/custom-types/bounded_profile.py',
    'tests/ros_custom_types/bounded-v2/bind_catalog.py',
    'tests/ros_custom_types/bounded-v2/native_identity.cc',
    'tests/ros_custom_types/bounded-v2/native_protocol.h',
    'gateways/common/native_io.h',
    'sdk/custom-types/interface_templates/bounded_interfaces.rs.in',
    'sdk/custom-types/interface_templates/bounded_interfaces.mjs.in',
    'sdk/custom-types/interface_templates/bounded_interfaces.go.in',
    'sdk/custom-types/interface_templates/bounded_interfaces.ts.in',
)
# These are current source roles, not SDK headers or permissions. The native
# owner still proves the actual complete compiler/MD/link/SDK closure separately.
LEGACY_EMITTER = 'sdk/robots/vendors/legacy_ros2_exporter.py'
LEGACY_SUPPORT = (
    LEGACY_EMITTER,
    'gateways/ros2/compat/metadata_export.h',
    'gateways/ros2/compat/rihs.h',
    'gateways/ros2/compat/identity.h',
    'gateways/ros2/compat/features.h',
    'src/uwvm2/imported/ros/ipc/custom_type.h',
    'src/uwvm2/imported/ros/ipc/custom_wire.h',
    'src/uwvm2/imported/ros/common/types.h',
)
ORIGINAL_IDENTITY_MODE = 'original-native-metadata'
LEGACY_IDENTITY_MODE = 'legacy-introspection'
LEGACY_PROVENANCE = 'LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01'
LANGUAGES = ('c','cpp','rust','javascript','assemblyscript','tinygo')
MAX_FILE = 16 * 1024 * 1024
MAX_TOTAL = 64 * 1024 * 1024


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


def need(condition, message):
    if not condition:
        raise ValueError(message)


def strict(raw):
    def pairs(rows):
        result = {}
        for key, value in rows:
            need(key not in result, 'duplicate JSON object key')
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs,
        parse_constant=lambda _: need(False, 'nonfinite JSON is refused'))


def canonical(value):
    return (json.dumps(value, indent=2, sort_keys=True, ensure_ascii=True,
        allow_nan=False) + '\n').encode()


def ordinary_pin(value):
    need(type(value) is dict and set(value) == PIN, 'exact ordinary full7 required')
    need(type(value['path']) is str and value['path'].startswith('/') and
        type(value['sha256']) is str and re.fullmatch('[0-9a-f]{64}',value['sha256']),
        'absolute selected ordinary path and exact SHA256 required')
    need(all(type(value[k]) is int and value[k] >= 0 for k in PIN-{'path','sha256'}) and
        value['inode'] > 0, 'typed ordinary full7 required')


def admitted(read, role, pin, maximum=MAX_FILE):
    ordinary_pin(pin)
    need(pin['bytes'] <= maximum, 'bounded role exceeds maximum: ' + role)
    raw = read(role, pin)
    need(type(raw) is bytes and len(raw) == pin['bytes'] and
        hashlib.sha256(raw).hexdigest() == pin['sha256'], 'admitted bytes differ: ' + role)
    return raw


def relative(name):
    need(type(name) is str and name and len(name) <= 512 and
        not PurePosixPath(name).is_absolute() and
        all(part not in ('','.','..') for part in name.split('/')) and
        '\\' not in name and '\x00' not in name, 'closed relative source name required')
    return name


def fact(path):
    """Pin only an actually produced ordinary file, through its own descriptor.

    The caller still supplies and guards the output authority. This helper does
    not adopt a pre-existing tree, follow final aliases or call a native tool.
    """
    path = Path(path)
    need(path.resolve(strict=True) == path, 'physical output path required')
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        before = os.fstat(fd)
        need(stat.S_ISREG(before.st_mode) and before.st_size <= MAX_FILE,
            'bounded ordinary output required')
        digest = hashlib.sha256(); count = 0
        while count <= before.st_size:
            block = os.read(fd, min(65536, before.st_size + 1 - count))
            if not block:
                break
            count += len(block); digest.update(block)
        after = os.fstat(fd); named = os.stat(path, follow_symlinks=False)
        key = lambda s: (s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns,s.st_mode,s.st_nlink)
        need(key(before) == key(after) == key(named) and count == before.st_size,
            'output changed during descriptor pin')
        return {'path':str(path),'bytes':count,'sha256':digest.hexdigest(),
            'device':before.st_dev,'inode':before.st_ino,
            'mtime_ns':before.st_mtime_ns,'ctime_ns':before.st_ctime_ns}
    finally:
        os.close(fd)


def fresh(path):
    path = Path(path)
    need(path.is_absolute() and not path.exists() and not path.is_symlink(),
        'select a fresh caller-owned source output')
    parent = path.parent.resolve(strict=True)
    need(parent == path.parent, 'canonical existing output parent required')
    path.mkdir(mode=0o700)
    return path


def write(root, name, raw):
    name = relative(name)
    need(type(raw) is bytes and len(raw) <= MAX_FILE, 'bounded source asset required')
    path = root/name
    path.parent.mkdir(parents=True, exist_ok=True)
    need(path.parent.resolve(strict=True).is_relative_to(root), 'output parent escaped')
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o600)
    try:
        with os.fdopen(fd,'wb') as stream:
            fd = None; stream.write(raw)
    finally:
        if fd is not None:
            os.close(fd)
    return fact(path)


def identity_mode(request):
    """Omission retains the original exporter; fallback is never automatic."""
    mode = request.get('identity_mode', ORIGINAL_IDENTITY_MODE)
    need(type(mode) is str and mode in (ORIGINAL_IDENTITY_MODE, LEGACY_IDENTITY_MODE), 'one exact native identity preparation mode required')
    need(mode != LEGACY_IDENTITY_MODE or (type(request.get('ros_version')) is int and request['ros_version'] == 2),
        'explicit legacy introspection is ROS2 Message/Service/Action only')
    return mode


def source_modules(support, *, read, load, mode=ORIGINAL_IDENTITY_MODE):
    """Load only actual current-MF definitions; no ambient module search.

    The original three modules and optional fourth pure legacy emitter use only
    stdlib imports. Native compatibility/declaration headers are admitted as
    complete current source evidence, not executed Python or SDK proof.
    """
    need(type(mode) is str and mode in (ORIGINAL_IDENTITY_MODE, LEGACY_IDENTITY_MODE), 'unknown native identity mode')
    roles = SUPPORT + (LEGACY_SUPPORT if mode == LEGACY_IDENTITY_MODE else ())
    need(type(support) is dict and set(support) == set(roles), 'all fixed current generator support roles required')
    originals = {name:admitted(read,'current-source:'+name,support[name]) for name in roles}
    modules = []
    python_roles = SUPPORT[:3] + ((LEGACY_EMITTER,) if mode == LEGACY_IDENTITY_MODE else ())
    for index, name in enumerate(python_roles):
        module = load(name, '_uwvm_vendor_preparer_' + str(index))
        need(Path(module.__file__).resolve(strict=True) == Path(support[name]['path']),
            'current-MF loaded module path differs')
        modules.append(module)
    for name in roles:
        need(admitted(read,'current-source-reopen:'+name,support[name]) == originals[name],
            'current support changed after loading')
    return modules, originals


def legacy_schemas(parent, bounded, records, interfaces, emitter=None):
    """Describe actual parsed native declarations; do not calculate a type hash.

    Capacities are the original native declaration values, never canonical
    buffer quotas. The real C++ helper independently compares actual installed
    introspection and derives RIHS only in that separately owned native run.
    """
    need(type(records) is dict and all(type(record) is bounded.Record and type(record.members) is list and
        all(type(field) is bounded.Field and getattr(field,'native_default',None) is None for field in record.members)
        for record in records.values()), 'legacy source declarations cannot carry native defaults')
    if type(interfaces) is list and any(getattr(item,'kind',None)!='message' for item in interfaces):
        need(emitter is not None, 'actual current-MF legacy emitter required for whole Service declarations')
        values=emitter.declarations(parent,bounded,records,interfaces)
        scalar={'int8':2,'uint8':3,'int16':4,'uint16':5,'int32':6,'uint32':7,'int64':8,'uint64':9,
            'float32':10,'float64':11,'char':13,'bool':15,'byte':16,'string':17}
        def graph(rows):
            types={}
            for name,fields in rows:
                native=[]
                for name_,type_,size,array,string_bound,sequence,sequence_bound in fields:
                    kind=scalar.get(type_,1)
                    if type_=='string' and string_bound:kind=21
                    if array:kind+=48
                    elif sequence:kind+=96 if sequence_bound else 144
                    native.append({'name':name_,'type_id':kind,'capacity':size if array else sequence_bound if sequence else 0,
                        'string_capacity':string_bound,'nested_type_name':type_ if type_ not in scalar else '', 'default_value':''})
                need(name not in types, 'duplicate complete native declaration')
                types[name]={'type':name,'fields':native}
            return {'root':types[rows[0][0]],'references':[types[name] for name in sorted(types) if name!=rows[0][0]]}
        result={name:{'kind':value['kind'],'official':graph(value['official']),
            'components':{role:graph(rows) for role,rows in value['components'].items()},
            'standard_protocol':value['standard_protocol'],
            **({'protocol_associations':{role:{'kind':row['kind'],'graph':graph(row['graph'])}
                for role,row in value['protocol_associations'].items()}} if value['kind']=='action' else {})} for name,value in values.items()}
        document={'format_version':2,'kind':'complete-original-native-interface-declarations-not-identities','types':result}
        need(len(canonical(document))<=MAX_FILE, 'finite complete Service native declaration sidecar required')
        return document
    need(type(records) is dict and type(interfaces) is list and 1 <= len(interfaces) <= 128 and
        all(type(i) is bounded.Interface and i.version == 2 and i.kind == 'message' and i.identity is None for i in interfaces),
        'complete unbound ROS2 Message-only source inventory required')
    need(set(records) == {(2, i.name) for i in interfaces} and len({i.name for i in interfaces}) == len(interfaces),
        'native Message and interface inventories differ')
    # Bound repeated per-root declaration graphs while constructing them.
    # This deliberately conservative accounting caps Python intermediates; the
    # later actual serialized file is also checked against its exact byte cap.
    declaration_budget = [1024]
    def charge(amount):
        need(type(amount) is int and 0 <= amount <= MAX_FILE-declaration_budget[0],
            'complete native declaration source exceeds finite preparation budget')
        declaration_budget[0] += amount
    scalar = {'int8':2,'uint8':3,'int16':4,'uint16':5,'int32':6,'uint32':7,'int64':8,'uint64':9,
        'float32':10,'float64':11,'char':13,'bool':15,'byte':16,'string':17}
    def declaration(record):
        need(type(record) is bounded.Record and record.version == 2 and record.kind == 'message' and record.identity is None and
            1 <= len(record.members) <= 4096, 'complete nonempty original Message declaration required')
        charge(len(record.name.encode('utf-8'))+1024)
        fields=[]
        for field in record.members:
            need(type(field) is bounded.Field and field.array in ('','array','sequence'), 'actual parsed field required')
            kind=scalar.get(field.type,1)
            if field.type == 'string' and field.native_string_bound:kind=21
            if field.array == 'array':kind+=48
            elif field.array == 'sequence':kind+=96 if field.native_sequence_bound else 144
            row={'name':field.name,'type_id':kind,
                'capacity':field.count if field.array=='array' else field.native_sequence_bound if field.array=='sequence' else 0,
                'string_capacity':field.native_string_bound,'nested_type_name':field.type if field.type not in scalar else '',
                'default_value':''}
            charge(len(canonical(row))+512);fields.append(row)
        return {'type':record.name,'fields':fields}
    result={}
    for interface in interfaces:
        charge(len(interface.name.encode('utf-8'))+1024)
        active=set();seen=set();rows={}
        def visit(name,depth):
            need(depth <= 16 and name not in active, 'finite acyclic native declaration closure required')
            if name in seen:return
            record=records.get((2,name));need(record is not None, 'native dependency missing')
            active.add(name);rows[name]=declaration(record);need(len(rows)<=256, 'native closure exceeds bound')
            for field in record.members:
                if field.type not in scalar:visit(field.type,depth+1)
            active.remove(name);seen.add(name)
        visit(interface.name,1)
        result[interface.name]={'root':rows[interface.name],
            'references':[rows[name] for name in sorted(rows) if name!=interface.name]}
    return {'format_version':1,'kind':'complete-original-native-message-declarations-not-identities','types':result}


def original_metadata_provenance(metadata, version):
    """A normal ROS2 exporter cannot silently accept a legacy-derived receipt."""
    if version != 2:return
    need(type(metadata) is dict and set(metadata)=={'format','ros_version','types'} and
        metadata['format']=='uwvm-ros-official-bounded-native-v2' and type(metadata['ros_version']) is int and metadata['ros_version']==2 and
        type(metadata['types']) is dict and 1<=len(metadata['types'])<=128, 'closed actual original ROS2 metadata required')
    for row in metadata['types'].values():
        need(type(row) is dict and set(row)=={'official','components'} and type(row['components']) is dict and row['components'],
            'closed original native metadata roles required')
        for value in [row['official'],*row['components'].values()]:
            need(type(value) is dict and set(value)=={'native_identity','root','references'},
                'original native exporter metadata required; no legacy-derived fallback')


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


def source_inputs(inventory, request, *, read):
    required = {'format_version','vendor','model','sdk_revision','ros_version','upstream_lock','native_headers','support'}
    need(type(request) is dict and required <= set(request) <= required | {'identity_mode'}, 'closed preparer request required')
    mode = identity_mode(request)
    need(type(request['format_version']) is int and request['format_version'] == 1 and
        type(request['ros_version']) is int and request['ros_version'] in (1,2), 'explicit format/backend required')
    need(reviewed_inventory(inventory), 'the reviewed vendor19/vendor28 inventory is DATA, not acceptance')
    vendors = [r for r in inventory['vendors'] if r['id'] == request['vendor']]
    need(len(vendors) == 1, 'one reviewed vendor required')
    vendor = vendors[0]
    need(not vendor['ros'+str(request['ros_version'])].startswith(('NOT_ESTABLISHED','EXPLICITLY_NOT_SUPPORTED')),
        'reviewed sources do not establish this vendor backend')
    for key in ('model','sdk_revision'):
        need(type(request[key]) is str and 1 <= len(request[key]) <= 128 and '\x00' not in request[key],
            'actual separate model and SDK major/revision required')
    lock_raw = admitted(read,'upstream-lock',request['upstream_lock'])
    lock = strict(lock_raw)
    need(type(lock) is dict and set(lock) == {'format_version','repository','commit','tree_sha256','model',
        'sdk_revision','definitions','catalog','license','source_review'}, 'closed reviewed Git-source lock required')
    need(type(lock['format_version']) is int and lock['format_version'] == 1 and
        lock['source_review'] == 'CLOSED_SOURCE_REVIEW_NOT_EXECUTION' and
        lock['repository'] in vendor['official_primary_sources'] and
        re.fullmatch('[0-9a-f]{40}',str(lock['commit'])) and
        re.fullmatch('[0-9a-f]{64}',str(lock['tree_sha256'])) and
        lock['model'] == request['model'] and lock['sdk_revision'] == request['sdk_revision'],
        'source lock must bind actual vendor/model/revision; moving URLs are insufficient')
    catalog_raw = admitted(read,'source-catalog',lock['catalog'],2*1024*1024)
    catalog = strict(catalog_raw)
    need(type(catalog) is dict and set(catalog) == {'format','types'} and catalog['format'] in (PROFILE,STANDARD_PROFILE) and
        type(catalog['types']) is list and 1 <= len(catalog['types']) <= 128, 'complete unbound bounded-v2 template required')
    need(type(lock['definitions']) is list and 1 <= len(lock['definitions']) <= 128,
        'finite complete definition inventory required')
    files = {}; seen = set()
    for row in lock['definitions']:
        need(type(row) is dict and set(row) == {'relative','generation_file','pin'}, 'closed official definition source row')
        relative(row['relative']); relative(row['generation_file'])
        need(row['relative'] not in seen and row['generation_file'] not in files, 'duplicate complete definition role')
        seen.add(row['relative'])
        files[row['generation_file']] = admitted(read,'definition:'+row['relative'],row['pin'],1024*1024)
    need(type(lock['definitions']) is list and 1 <= len(files) <= 128, 'finite complete dependency closure required')
    license_raw = admitted(read,'official-license',lock['license'])
    names = []
    for row in catalog['types']:
        need(type(row) is dict and set(row) <= ({'ros_version','type','file','bounds','registered'} if catalog['format']==STANDARD_PROFILE else {'ros_version','type','file','bounds'}) and
            {'ros_version','type','file'} <= set(row) and row['ros_version'] == request['ros_version'],
            'native identities may not be prefilled into the preparation template')
        relative(row['file']); need(row['file'] in files, 'catalog definition absent from original closure')
        names.append(row['file'])
    need(len(set(names)) == len(names) and set(names) == set(files),
        'whole catalog/definition closure must match, no undeclared extra or omitted input')
    need(sum(map(len,files.values())) + len(catalog_raw) + len(lock_raw) + len(license_raw) <= MAX_TOTAL,
        'source preparation aggregate bound exceeded')
    if mode == LEGACY_IDENTITY_MODE:
        need(all(row['file'].endswith(('.msg','.srv','.action')) for row in catalog['types']),
            'legacy introspection requires complete Message/Service/Action roots and native protocol proof')
    headers = request['native_headers']
    need(type(headers) is list and len(headers) == len(catalog['types']), 'every actual installed native type header required')
    by_type = {}
    for row in headers:
        need(type(row) is dict and set(row) == {'type','include','pin'} and row['type'] not in by_type,
            'closed unique original generated native header role')
        include = relative(row['include'])
        need(re.fullmatch('[A-Za-z0-9_]+/(?:[a-z_]+/)?[A-Za-z0-9_]+\\.(?:h|hpp)',include),
            'one ordinary native include spelling, never compiler options or source code')
        admitted(read,'native-header:'+row['type'],row['pin'])
        by_type[row['type']] = row
    need(set(by_type) == {row['type'] for row in catalog['types']}, 'native header inventory differs from whole catalog')
    return lock, lock_raw, catalog, catalog_raw, files, license_raw, by_type


def exporter(template, catalog, headers, version):
    """Keep the existing native metadata implementation unchanged.

    Only the finite original include/type-call inventory is generated. C++ type
    spellings follow actual ROS namespaces, and include names are separately
    reviewed installed-header inputs. No byte offsets, dynamic provider or
    invented type hash is emitted. ROS2 without real description/hash support
    is refused by the retained official-metadata implementation at execution.
    """
    text = template.decode('utf-8')
    begin = text.index('namespace\n{\n')
    end = text.index('\nint main()\n')
    helpers = text[begin:end]
    need(helpers.count('namespace\n{\n') == 1 and '#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1' in helpers,
        'expected current exporter helper source shape')
    prefix = ['/** Generated model-native identity inventory, source only. No ROS Node or grant. */',
        '#include "tests/ros_custom_types/bounded-v2/native_protocol.h"',
        '#include <array>','#include <string_view>','#include <stdexcept>','#include <csignal>',
        '#if UWVM_CUSTOM_NATIVE_ROS_VERSION != '+str(version),
        '#error "Compile against exactly the reviewed native ROS major"','#endif']
    calls = []
    for index, row in enumerate(catalog['types']):
        bits = row['type'].split('/')
        need(re.fullmatch('[a-z][a-z0-9_]{0,62}',bits[0]) and
            re.fullmatch('[A-Z][A-Za-z0-9]*',bits[-1]), 'exact native package/type grammar')
        extension = PurePosixPath(row['file']).suffix
        kind = {'.msg':'message','.srv':'service','.action':'action'}[extension]
        need(len(bits) == (2 if version == 1 else 3) and
            (version == 1 or bits[1] == {'message':'msg','service':'srv','action':'action'}[kind]),
            'exact native interface category')
        header = headers[row['type']]['include']
        if version == 1:
            need(header == bits[0]+'/'+bits[-1]+('Action' if kind == 'action' else '')+'.h',
                'ROS1 generated native header spelling differs')
        else:
            need(re.fullmatch(re.escape(bits[0]+'/'+bits[1]+'/')+'[a-z][a-z0-9_]*\\.hpp',header),
                'ROS2 reviewed generated native header category differs')
        prefix.append('#include <'+header+'>')
        cpp = '::'+'::'.join(bits)
        if index:
            calls.append('        bounded_native_io::print(output(),\',\');')
        calls.append('        quoted('+json.dumps(row['type'])+');')
        if version == 1:
            trait = '::ros::service_traits' if kind == 'service' else '::ros::message_traits'
            root = cpp+('Action' if kind == 'action' else '')
            calls += ['        bounded_native_io::print(output(),":{\\"native_identity\\":");',
                '        quoted('+trait+'::MD5Sum<'+root+'>::value());',
                '        bounded_native_io::print(output(),",\\"components\\":{");']
        else:
            handle = {'message':'message','service':'service','action':'action'}[kind]
            calls += ['        bounded_native_io::print(output(),":{\\"official\\":");',
                '        metadata(rosidl_typesupport_cpp::get_'+handle+'_type_support_handle<'+cpp+'>());',
                '        bounded_native_io::print(output(),",\\"components\\":{");']
        roles = {'message':['message'],'service':['request','response'],'action':['goal','result','feedback']}[kind]
        for number, role in enumerate(roles):
            part = cpp if kind == 'message' else cpp+('::'+role.title() if version == 2 or kind == 'service' else role.title())
            calls.append('        component<'+part+'>('+json.dumps(role)+','+('true' if number else 'false')+');')
        calls.append('        bounded_native_io::print(output(),"}}");')
    if version == 1:
        prefix += ['#include <ros/message_traits.h>','#include <ros/service_traits.h>']
    else:
        prefix += ['#include <rosidl_typesupport_cpp/message_type_support.hpp>',
            '#include <rosidl_typesupport_cpp/service_type_support.hpp>',
            '#include <rosidl_typesupport_cpp/action_type_support.hpp>']
    main = '\n'.join(['int main() {','    try {',
        '        if(::raise(SIGSTOP)!=0) {throw std::runtime_error{"exporter mapping stop failed"};}',
        "        bounded_native_io::gate('I');",
        '        bounded_native_io::print(output(),"{\\"format\\":\\"uwvm-ros-official-bounded-native-v2\\",\\"ros_version\\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\\"types\\":{");',
        *calls,'        bounded_native_io::print(output(),"}}\\n");return 0;',
        '    } catch(std::exception const&) {',
        '        bounded_native_io::print(STDERR_FILENO,"native identity export refused\\n");return 1;','    }','}'])
    raw = ('\n'.join(prefix)+'\n'+helpers+'\n'+main+'\n').encode()
    need(len(raw) <= MAX_FILE, 'finite exporter source bound')
    return raw, hashlib.sha256(helpers.encode()).hexdigest()


def prepare(inventory, request, *, read, load, out):
    """Write fresh complete guest-only codecs and a real native exporter source.

    Root must first admit every external definition/header/license and current
    source role. It later compiles/runs the exporter under its actual owned
    architecture/cgroup/SDK process lane. This function does not run that lane.
    A refusal may leave an unaccepted fresh source directory for forensic review;
    it never overwrites a prior output or silently rolls back evidence.
    """
    lock, lock_raw, catalog, catalog_raw, files, license_raw, headers = source_inputs(inventory,request,read=read)
    mode = identity_mode(request)
    modules, support_raw = source_modules(request['support'],read=read,load=load,mode=mode)
    parent,bounded,binder = modules[:3]
    if catalog['format']==STANDARD_PROFILE:
        need(getattr(bounded,'STANDARD_PROFILE',None)==STANDARD_PROFILE,
            'the actual current-MF bounded generator must implement the reviewed explicit standard mode')
    output = fresh(out)
    assets = {'input/catalog.json':write(output,'input/catalog.json',catalog_raw),
        'source-lock.json':write(output,'source-lock.json',lock_raw),
        'UPSTREAM-LICENSE':write(output,'UPSTREAM-LICENSE',license_raw)}
    for name, raw in files.items():
        assets['input/'+name] = write(output,'input/'+name,raw)
    # Parse through the real current source. The explicit standard profile
    # preserves its exact official dependency family; legacy mode cannot relax
    # category, field, complete-closure or builtin-shadow restrictions.
    records,_,interfaces,_ = bounded.load_catalog(parent,output/'input/catalog.json',False)
    if mode == LEGACY_IDENTITY_MODE:
        schemas = legacy_schemas(parent,bounded,records,interfaces,modules[3])
        for item in interfaces:
            expected=item.name.split('/')[0]+'/'+{'message':'msg','service':'srv','action':'action'}[item.kind]+'/'+parent.ros2_header(item.name.split('/')[-1])+'.hpp'
            need(headers[item.name]['include']==expected, 'actual legacy native header spelling differs')
        code = modules[3].emit(parent,bounded,records,interfaces)
        need(type(code) is bytes and len(code)<=MAX_FILE, 'bounded actual legacy emitter bytes required')
        helper_hash = None  # No claim that the original modern namespace ran.
        assets['native-declaration-schemas.json'] = write(output,'native-declaration-schemas.json',canonical(schemas))
    else:
        code, helper_hash = exporter(support_raw[SUPPORT[3]],catalog,headers,request['ros_version'])
    assets['native_identity.cc'] = write(output,'native_identity.cc',code)
    generation = bounded.generate(parent,output/'input/catalog.json',output/'guest-only',guest_only=True)
    for name, expected in generation['outputs'].items():
        pin = fact(output/'guest-only'/relative(name))
        need({key:pin[key] for key in ('bytes','sha256')} == expected, 'actual guest output body differs')
        assets['guest-only/'+name] = pin
    assets['guest-only/generation.json'] = fact(output/'guest-only/generation.json')
    # Reopen original input roles and current code after all source work. The
    # caller's read repeats its real held-FD/mode/nlink/root checks each time.
    source_inputs(inventory,request,read=read)
    for name, raw in support_raw.items():
        need(admitted(read,'current-source-final:'+name,request['support'][name]) == raw,
            'source implementation changed during preparation')
    receipt = {'format_version':1,'kind':'vendor-complete-schema-source-preparation-v1',
        'status':'PREPARED_GUEST_SOURCE_AND_NATIVE_EXPORTER_NOT_NATIVE_QUALIFIED',
        'vendor':request['vendor'],'model':request['model'],'sdk_revision':request['sdk_revision'],
        'ros_version':request['ros_version'],'catalog_format':catalog['format'],'request':request,'files':assets,
        'retained_official_metadata_helpers_sha256':helper_hash,
        'native_identity_mode':mode,
        'legacy_source_roles':{name:request['support'][name] for name in LEGACY_SUPPORT} if mode==LEGACY_IDENTITY_MODE else None,
        'guest_languages':list(LANGUAGES),'default_authority':[],
        'native_identity':'NOT_ACQUIRED','native_registry_emitted':False,
        'compiler_executed':False,'Runtime_executed':False,'native_delivery_qualified':False,
        'hardware_qualified':False,'official_cases_run':0,
        'next_required':['actual official generator/install source and native library closure',
            'compile exporter with C++17, -UNDEBUG and full system-inclusive MD',
            'actual native ELF, kernel map and original pidfd-owned stopped exporter proof',
            'single actual I gate then finite raw official metadata output',
            'independent native metadata/schema/traits and current SDK qualification before any grant']}
    write(output,'preparation.json',canonical(receipt))
    return receipt


def bind_and_generate(preparation_pin, native_metadata, native_owner_evidence, *, read, load, out):
    """Copy actual exported identities through the real binder, then emit six codecs.

    The owner evidence is retained ordinary DATA, never interpreted as a magic
    true field establishing an execution. Root's actual qualification remains
    mandatory. Emitted native registry source still gives no Runtime authority.
    """
    original = strict(admitted(read,'source-preparation',preparation_pin))
    need(type(original) is dict and original.get('kind') == 'vendor-complete-schema-source-preparation-v1' and
        original.get('status') == 'PREPARED_GUEST_SOURCE_AND_NATIVE_EXPORTER_NOT_NATIVE_QUALIFIED' and
        original.get('default_authority') == [], 'one actual source preparation receipt required')
    request = original['request']
    mode = identity_mode(request)
    modules, support_raw = source_modules(request['support'],read=read,load=load,mode=mode)
    parent,bounded,binder = modules[:3]
    need(original.get('native_identity_mode',ORIGINAL_IDENTITY_MODE)==mode, 'source preparation identity mode differs')
    files = original['files']; input_files = {}; prepared_bodies = {}
    for name, pin in files.items():
        relative(name)
        raw = admitted(read,'prepared:'+name,pin); prepared_bodies[name]=raw
        if name.startswith('input/'):
            input_files[name[6:]] = raw
    need('catalog.json' in input_files, 'whole prepared source catalog required')
    metadata_raw = admitted(read,'actual-official-native-metadata',native_metadata)
    evidence_raw = admitted(read,'actual-native-owner-evidence',native_owner_evidence)
    output = fresh(out)
    for name, raw in input_files.items():
        write(output,'input/'+name,raw)
    legacy_provenance = None; schema_pin = None
    if mode == LEGACY_IDENTITY_MODE:
        records,_,interfaces,_ = bounded.load_catalog(parent,output/'input/catalog.json',False)
        schemas = legacy_schemas(parent,bounded,records,interfaces,modules[3])
        need(prepared_bodies.get('native-declaration-schemas.json')==canonical(schemas),
            'retained preparation declarations differ from actual reparsed source')
        legacy_provenance = legacy_metadata_provenance(strict(metadata_raw),schemas)
        schema_pin = write(output,'legacy-declaration-schemas.json',canonical(schemas))
    else:
        original_metadata_provenance(strict(metadata_raw),request['ros_version'])
    metadata_pin = write(output,'actual-native-metadata.json',metadata_raw)
    owner_copy = write(output,'actual-native-owner-evidence.json',evidence_raw)
    binding = binder.bind(output/'input/catalog.json',output/'actual-native-metadata.json',output/'bound/catalog.json')
    generated = bounded.generate(parent,output/'bound/catalog.json',output/'generated',guest_only=False)
    produced = {}
    for name, expected in generated['outputs'].items():
        pin = fact(output/'generated'/relative(name))
        need({key:pin[key] for key in ('bytes','sha256')} == expected, 'actual native-bound output differs')
        produced[name] = pin
    need(sum(pin['bytes'] for pin in produced.values()) <= MAX_TOTAL, 'whole generated output budget exceeded')
    generation_pin = fact(output/'generated/generation.json')
    bound_catalog = fact(output/'bound/catalog.json')
    bound_definitions = {row['file']:fact(output/'bound'/row['file'])
        for row in strict(input_files['catalog.json'])['types']}
    for name, pin in files.items():
        admitted(read,'prepared-final:'+name,pin)
    admitted(read,'actual-official-native-metadata-final',native_metadata)
    admitted(read,'actual-native-owner-evidence-final',native_owner_evidence)
    for name, raw in support_raw.items():
        need(admitted(read,'current-source-final:'+name,request['support'][name]) == raw, 'generator changed')
    receipt = {'format_version':1,'kind':'vendor-native-bound-six-language-source-v1',
        'status':'PREPARED_WITH_CALLER_EXPORTED_IDENTITIES_NOT_NATIVE_QUALIFIED',
        'vendor':original['vendor'],'model':original['model'],'sdk_revision':original['sdk_revision'],
        'ros_version':original['ros_version'],'preparation':preparation_pin,
        'native_metadata_original':native_metadata,'native_owner_evidence_original':native_owner_evidence,
        'native_metadata_copy':metadata_pin,'native_owner_evidence_copy':owner_copy,
        'native_identity_mode':mode,'legacy_identity_provenance':legacy_provenance,
        'legacy_declaration_schemas':schema_pin,
        'binding':binding,'bound_catalog':bound_catalog,'bound_definitions':bound_definitions,
        'generation':generation_pin,'generated_files':produced,
        'guest_languages':list(LANGUAGES),'default_authority':[],
        'native_registry_emitted':True,'compiler_executed':False,'Runtime_executed':False,
        'native_delivery_qualified':False,'hardware_qualified':False,'official_cases_run':0}
    write(output,'source-binding.json',canonical(receipt))
    return receipt
