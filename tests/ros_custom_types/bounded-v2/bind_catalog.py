#!/usr/bin/env python3
"""Bind an exact finite catalog to independently exported official native identities.

This administrative preparation tool never invokes a ROS generator, loads a
library, computes an MD5/RIHS value, grants authority or creates a native entity.
The unified controller must qualify the exporter, original package, full compiler
inputs and linked native support before using the new catalog. Gateway admission
then independently compares official root/component identities and every field.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import stat


def read(path,maximum):
    """Read a bounded regular file without following a final symlink or blocking FIFO."""
    fd=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW)
    try:
        before=os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or before.st_size>maximum:
            raise ValueError('expected bounded regular input')
        body=bytearray()
        while len(body)<=before.st_size:
            piece=os.read(fd,min(65536,before.st_size+1-len(body)))
            if not piece:break
            body.extend(piece)
        after=os.fstat(fd)
        identity=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns)
        if identity(before)!=identity(after) or len(body)!=before.st_size:
            raise ValueError('input mutated while reading')
        return bytes(body)
    finally:os.close(fd)


def strict(body):
    def pairs(items):
        result={}
        for key,value in items:
            if key in result:raise ValueError('duplicate JSON key')
            result[key]=value
        return result
    return json.loads(body,object_pairs_hook=pairs)


def native_identity(row,version):
    """Copy only a syntactically qualified exported identity; never derive one."""
    if not isinstance(row,dict):raise ValueError('native metadata object required')
    value=(row.get('official',row)).get('native_identity')
    pattern=r'[0-9a-f]{32}' if version==1 else r'RIHS01_[0-9a-f]{64}'
    if not isinstance(value,str) or not re.fullmatch(pattern,value):
        raise ValueError('official native identity missing or malformed')
    return value


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

def bind(template_path,metadata_path,output_path):
    template_bytes=read(template_path,2*1024*1024)
    metadata_bytes=read(metadata_path,16*1024*1024)
    template,metadata=strict(template_bytes),strict(metadata_bytes)
    if not isinstance(template,dict) or set(template)!={'format','types'} or template['format'] not in ('uwvm-ros-custom-bounded-v2','uwvm-ros-official-standard-bounded-v1'):
        raise ValueError('closed bounded-v2 template required')
    if not isinstance(metadata,dict) or set(metadata)!={'format','ros_version','types'} or metadata['format']!='uwvm-ros-official-bounded-native-v2' or type(metadata['ros_version']) is not int or metadata['ros_version'] not in (1,2):
        raise ValueError('official exporter format/backend mismatch')
    # The explicit standard mode retains dependency-registration markers; it
    # does not acquire identities or turn Header/Time into Topic authority.
    # The same exact official root/component inventory remains mandatory.
    version=metadata['ros_version'];entries=template['types']
    if not isinstance(entries,list) or not 1<=len(entries)<=128 or not isinstance(metadata['types'],dict):
        raise ValueError('bounded interface inventory required')
    names=[entry.get('type') for entry in entries if isinstance(entry,dict)]
    if len(names)!=len(entries) or len(set(names))!=len(entries) or set(names)!=set(metadata['types']):
        raise ValueError('exact original official interface inventory required')
    definition_copies={}
    for entry in entries:
        if entry.get('ros_version')!=version or 'native_identity' in entry or 'component_identities' in entry:
            raise ValueError('unbound single-backend preparation template required')
        extension=Path(entry['file']).suffix
        roles={'.msg':('message',),'.srv':('request','response'),'.action':('goal','result','feedback')}.get(extension)
        native=metadata['types'][entry['type']]
        if roles is None or not isinstance(native,dict) or not isinstance(native.get('components'),dict) or set(native['components'])!=set(roles):
            raise ValueError('complete official component inventory required')
        entry['native_identity']=native_identity(native,version)
        if extension!='.msg':entry['component_identities']={role:native_identity(native['components'][role],version) for role in roles}
        else:
            if native_identity(native['components']['message'],version)!=entry['native_identity']:
                raise ValueError('official Message root/component identity differs')
        # Snapshot each complete official source definition into the fresh
        # derived directory. The original SourceA tree remains immutable.
        definition=(template_path.resolve().parent/entry['file']).resolve()
        if not definition.is_relative_to(template_path.resolve().parent):
            raise ValueError('definition escapes original template root')
        relative=Path(entry['file'])
        if relative.is_absolute() or '..' in relative.parts or '.' in relative.parts:
            raise ValueError('closed relative definition path required')
        definition_copies[str(relative)]=read(definition,1024*1024)
    default_contract=native_default_contract(template,definition_copies,metadata)
    if output_path.exists() or output_path.parent.exists():raise ValueError('select a fresh derived catalog directory, preserving immutable SourceA')
    body=(json.dumps(template,indent=2)+'\n').encode()
    output_path.parent.mkdir(parents=True,exist_ok=True)
    for name,definition in definition_copies.items():
        target=output_path.parent/name
        target.parent.mkdir(parents=True,exist_ok=True)
        if target.exists():
            if read(target,1024*1024)!=definition:raise ValueError('existing definition copy differs')
        else:
            with target.open('xb') as stream:stream.write(definition)
    with output_path.open('xb') as stream:stream.write(body)
    receipt={'status':'NATIVE_IDENTITY_BINDING_SOURCE_ONLY','template':{'path':str(template_path.resolve()),'sha256':hashlib.sha256(template_bytes).hexdigest(),'bytes':len(template_bytes)},'official_export':{'path':str(metadata_path.resolve()),'sha256':hashlib.sha256(metadata_bytes).hexdigest(),'bytes':len(metadata_bytes)},'output':{'path':str(output_path.resolve()),'sha256':hashlib.sha256(body).hexdigest(),'bytes':len(body)},'definition_copies':{name:{'sha256':hashlib.sha256(value).hexdigest(),'bytes':len(value)} for name,value in definition_copies.items()},'native_registration':'NOT_ESTABLISHED','implicit_permissions':[]}
    if default_contract is not None:receipt['native_default_contract']=default_contract
    return receipt


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--template',type=Path,required=True)
    parser.add_argument('--native-metadata',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    print(json.dumps(bind(args.template,args.native_metadata,args.output),indent=2))

if __name__=='__main__':main()
