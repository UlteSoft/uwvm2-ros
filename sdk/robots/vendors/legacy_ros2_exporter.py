"""Pure source emitter for explicit pre-RIHS ROS2 Message/Service/Action preparation.

Actual current-MF parser modules and exact unbound parsed objects are required.
No filesystem, import, process, native hash or ROS endpoint operation occurs.
Installed ServiceMembers associations and every payload field are verified by
real C++ against the selected SDK. Absent old-SDK Event types are labelled as
standard protocol declarations, never installed native introspection.
"""
import json

MAX_SOURCE = 16 * 1024 * 1024
STANDARD_PROTOCOL = 'ROS2_SERVICE_EVENT_V1'
ACTION_PROTOCOL = 'ROS2_ACTION_EVENT_V1'


def action_runtime_declarations():
    """Complete source-only shared Action protocols, not a canonical registry.

    Original CancelGoal/GoalStatus constants do not become serialized fields.
    This table preserves their complete native fields and unbounded capacities;
    the actual installed typed metadata is still independently mandatory.
    """
    def f(name,type_,count=0,array=False,sequence=False,bound=0):
        return (name,type_,count,array,0,sequence,bound)
    return [
        ('builtin_interfaces/msg/Time',[f('sec','int32'),f('nanosec','uint32')]),
        ('unique_identifier_msgs/msg/UUID',[f('uuid','uint8',16,True)]),
        ('action_msgs/msg/GoalInfo',[f('goal_id','unique_identifier_msgs/msg/UUID'),f('stamp','builtin_interfaces/msg/Time')]),
        ('action_msgs/msg/GoalStatus',[f('goal_info','action_msgs/msg/GoalInfo'),f('status','int8')]),
        ('action_msgs/msg/GoalStatusArray',[f('status_list','action_msgs/msg/GoalStatus',sequence=True)]),
        ('action_msgs/srv/CancelGoal_Request',[f('goal_info','action_msgs/msg/GoalInfo')]),
        ('action_msgs/srv/CancelGoal_Response',[f('return_code','int8'),f('goals_canceling','action_msgs/msg/GoalInfo',sequence=True)]),
        ('service_msgs/msg/ServiceEventInfo',[f('event_type','uint8'),f('stamp','builtin_interfaces/msg/Time'),f('client_gid','uint8',16,True),f('sequence_number','int64')]),
        ('action_msgs/srv/CancelGoal_Event',[f('info','service_msgs/msg/ServiceEventInfo'),f('request','action_msgs/srv/CancelGoal_Request',sequence=True,bound=1),f('response','action_msgs/srv/CancelGoal_Response',sequence=True,bound=1)]),
        ('action_msgs/srv/CancelGoal',[f('request_message','action_msgs/srv/CancelGoal_Request'),f('response_message','action_msgs/srv/CancelGoal_Response'),f('event_message','action_msgs/srv/CancelGoal_Event')])]


def selected_declarations(parent, rows, root):
    """One finite reachable source view; disconnected raw rows are not exported."""
    values=dict(rows)
    if len(values)!=len(rows):raise parent.GenerationError('duplicate protocol declaration')
    result=[];active=set();done=set()
    def visit(name,depth):
        if depth>16 or name in active or name not in values:raise parent.GenerationError('finite complete protocol closure required')
        if name in done:return
        active.add(name);result.append((name,values[name]))
        if len(result)>256:raise parent.GenerationError('protocol graph exceeds bound')
        for field in values[name]:
            if field[1] not in parent.SCALARS and field[1]!='string':visit(field[1],depth+1)
        active.remove(name);done.add(name)
    visit(root,1);return result


def declarations(parent, bounded, records, interfaces):
    """Return complete original-native graphs, distinct from canonical quotas.

    Empty Service components retain the official representation-only uint8.
    The real parser's protocol constructor supplies the full standard root/Event
    grammar; this module never renames fields or calculates a native identity.
    """
    def need(ok, message):
        if not ok:
            raise parent.GenerationError(message)
    need(type(records) is dict and type(interfaces) is list and 1 <= len(interfaces) <= 128,
         'complete actual parsed interface inventory required')
    need(all(type(item) is bounded.Interface and item.version == 2 and
        item.kind in ('message','service','action') and item.identity is None for item in interfaces),
        'legacy mode requires unbound ROS2 Message/Service/Action with complete native protocol proof')
    need(len({item.name for item in interfaces}) == len(interfaces), 'duplicate native interface name')
    profiles={item.profile for item in interfaces}
    need(len(profiles)==1 and next(iter(profiles)) in (bounded.PROFILE,getattr(bounded,'STANDARD_PROFILE',None)),
         'one actual parsed catalog profile required')
    expected={}
    for item in interfaces:
        bounded.native_name(parent,2,item.name,item.kind,item.profile==getattr(bounded,'STANDARD_PROFILE',None))
        roles={'message':('message',),'service':('request','response'),'action':('goal','result','feedback')}[item.kind]
        need(type(item.components) is dict and set(item.components)==set(roles), 'complete interface component roles required')
        for role in roles:
            value=item.components[role]
            need(type(value) is bounded.Record and value.version==2 and value.identity is None and
                value.kind==item.kind and (value.component==role and value.owner==item.name and
                value.name==item.name+'_'+role.title() if item.kind!='message' else not value.component and value.name==item.name),
                'actual unbound native component record required')
            need((2,value.name) not in expected, 'duplicate native component')
            expected[(2,value.name)]=value
    need(set(records)==set(expected) and all(records[key] is value for key,value in expected.items()),
         'exact original parsed record/interface association required')
    for key,value in records.items():
        need(type(value.members) is list and len(value.members)<=4096, 'finite complete native fields required')
        for field in value.members:
            need(type(field) is bounded.Field and field.array in ('','array','sequence'), 'actual parsed field shape required')
            parent.field_name(field.name,2,bounded=True)
            need(getattr(field,'native_default',None) is None, 'legacy declarations cannot carry native defaults')
            need(all(type(n) is int and 0<=n<=4294967295 for n in
                (field.count,field.native_string_bound,field.native_sequence_bound)), 'native capacity types required')
            need(not field.array or 1<=field.count<=4096, 'finite canonical capacity required independently of native bound')
    def fields(record):
        values=[(f.name,f.type,f.count if f.array=='array' else 0,f.array=='array',
            f.native_string_bound,f.array=='sequence',f.native_sequence_bound) for f in record.members]
        if not values:
            need(record.kind=='service', 'zero-field Topic still needs an independent implementation')
            values=[('structure_needs_at_least_one_member','uint8',0,False,0,False,0)]
        return values
    def closure(name):
        rows=[];active=set();done=set()
        def visit(current,depth):
            need(depth<=16 and current not in active, 'finite acyclic native declaration closure required')
            if current in done:return
            value=records.get((2,current));need(value is not None, 'complete native dependency missing')
            active.add(current);rows.append((current,fields(value)))
            need(len(rows)<=256, 'finite native graph exceeds bound')
            for f in value.members:
                if f.type not in parent.SCALARS and f.type!='string':visit(f.type,depth+1)
            active.remove(current);done.add(current)
        visit(name,1)
        return rows
    result={};budget=1024
    for item in interfaces:
        components={role:closure(value.name) for role,value in item.components.items()}
        graph=components['message'] if item.kind=='message' else bounded.ros2_interface_schema(parent,records,item)
        need(type(graph) is list and 1<=len(graph)<=256, 'complete finite interface root graph required')
        result[item.name]={'kind':item.kind,'official':graph,'components':components,
            'standard_protocol':'NONE' if item.kind=='message' else STANDARD_PROTOCOL if item.kind=='service' else ACTION_PROTOCOL}
        if item.kind=='action':
            runtime=action_runtime_declarations()
            result[item.name]['runtime']=runtime
            result[item.name]['protocol_associations']={role:{'kind':kind,'graph':selected_declarations(parent,rows,root)}
                for role,kind,rows,root in (
                    ('send_goal','service',graph,item.name+'_SendGoal'),
                    ('get_result','service',graph,item.name+'_GetResult'),
                    ('feedback_message','message',graph,item.name+'_FeedbackMessage'),
                    ('cancel_goal','service',runtime,'action_msgs/srv/CancelGoal'),
                    ('status','message',runtime,'action_msgs/msg/GoalStatusArray'),
                    ('uuid','message',graph,'unique_identifier_msgs/msg/UUID'),
                    ('time','message',graph,'builtin_interfaces/msg/Time'))}
        # Conservative finite charge before keeping another per-root graph.
        budget+=len(json.dumps(result[item.name],ensure_ascii=True,allow_nan=False).encode())+4096
        need(budget<=MAX_SOURCE, 'complete native graphs exceed finite source budget')
    return result


def emit(parent, bounded, records, interfaces):
    """Emit complete actual-native metadata before SIGSTOP, then one I-gated JSON.

    Every interface/dependency remains in the exact binder inventory. Service
    root identity and actual Request/Response identities are separate exports.
    No Node, entity, robot command, provider or permission is constructed here.
    """
    graphs=declarations(parent,bounded,records,interfaces)
    def literal(value):return json.dumps(value,ensure_ascii=True,allow_nan=False)
    class SourceLines:
        def __init__(self):self.rows=[];self.bytes=0
        def append(self,line):
            size=len(line.encode())+1
            if size>MAX_SOURCE-self.bytes:raise parent.GenerationError('finite native exporter source ceiling')
            self.rows.append(line);self.bytes+=size
        def __iadd__(self,values):
            for line in values:self.append(line)
            return self
    lines=SourceLines()
    lines += ['/** Actual pre-RIHS ROS2 Message/Service/Action source; no execution or grant.',
        ' * Verify real installed SDK/MD/link/ELF/maps and owned original I gate.',
        ' * All introspection and complete JSON materialize BEFORE SIGSTOP.',
        ' * Old-SDK Event declarations are explicitly standard protocol, not',
        ' * invented installed-native Event type support. */',
        '#include "tests/ros_custom_types/bounded-v2/native_protocol.h"',
        '#include "gateways/ros2/compat/metadata_export.h"',
        '#include <cstddef>','#include <csignal>','#include <stdexcept>',
        '#include <string>','#include <string_view>',
        '#if UWVM_CUSTOM_NATIVE_ROS_VERSION != 2',
        '#error "Explicitly select actual ROS2 SDK"','#endif']
    lines += ['#include <'+item.name.split('/')[0]+'/'+{'message':'msg','service':'srv','action':'action'}[item.kind]+'/'+
        parent.ros2_header(item.name.split('/')[-1])+'.hpp>' for item in interfaces]
    lines += ['namespace {','namespace ipc=::uwvm2::imported::ros::ipc;',
        'namespace legacy=::uwvm2::ros_gateway::ros2_compat::legacy_metadata_export;',
        'constexpr std::size_t maximum_document=16u*1024u*1024u;',
        'void append(std::string& out,std::string_view value) {',
        ' if(value.size()>maximum_document-out.size())throw std::length_error("native metadata document ceiling");',
        ' out.append(value);','}']
    def function(label,graph,native,method,root,runtime=None):
        nonlocal lines
        lines.append('std::string '+label+'() {')
        for index,(_,values) in enumerate(graph):
            rows=['{'+','.join(literal(v) if type(v) is str else str(v).lower() if type(v) is bool else str(v) for v in f)+'}' for f in values]
            lines.append(' static constexpr ipc::custom_native_field fields_'+str(index)+'[]={'+','.join(rows)+'};')
        lines.append(' static constexpr ipc::custom_native_record graph[]={')
        for index,(name,values) in enumerate(graph):lines.append(' {'+literal(name)+',fields_'+str(index)+','+str(len(values))+'},')
        lines.append(' };')
        if runtime is not None:
            for index,(_,values) in enumerate(runtime):
                fields=['{'+','.join(literal(v) if type(v) is str else str(v).lower() if type(v) is bool else str(v) for v in f)+'}' for f in values]
                lines.append(' static constexpr ipc::custom_native_field runtime_fields_'+str(index)+'[]={'+','.join(fields)+'};')
            lines.append(' static constexpr ipc::custom_native_record runtime_graph[]={')
            for index,(name,values) in enumerate(runtime):lines.append(' {'+literal(name)+',runtime_fields_'+str(index)+','+str(len(values))+'},')
            lines.append(' };')
        extra=',{runtime_graph,sizeof(runtime_graph)/sizeof(runtime_graph[0])}' if runtime is not None else ''
        lines += [' return legacy::'+method+'<'+native+'>({graph,sizeof(graph)/sizeof(graph[0])},'+literal(root)+extra+');','}']
    # Keep the accumulator local; nested helper writes only finite source lines.
    for number,item in enumerate(interfaces):
        value=graphs[item.name];native='::'+item.name.replace('/','::')
        function('official_'+str(number),value['official'],native,item.kind,item.name,value.get('runtime'))
        if item.kind!='message':
            for role,graph in value['components'].items():
                component=item.components[role]
                function(role+'_'+str(number),graph,component.native,'message',component.name)
    lines += ['std::string document() {',' std::string out;',
        ' append(out,'+literal('{"format":"uwvm-ros-official-bounded-native-v2","ros_version":2,"types":{')+');']
    for number,item in enumerate(interfaces):
        prefix=(',' if number else '')+literal(item.name)+':{"official":'
        lines += [' { auto official=official_'+str(number)+'();',' append(out,'+literal(prefix)+');append(out,official);',
            ' append(out,'+literal(',"components":{')+');']
        for index,role in enumerate(graphs[item.name]['components']):
            lines.append(' append(out,'+literal((',' if index else '')+literal(role)+':')+');')
            lines.append(' append(out,'+('official' if item.kind=='message' else role+'_'+str(number)+'()')+');')
        lines += [' append(out,'+literal('}}')+'); }']
    lines += [' append(out,'+literal('}}\n')+');return out;','}','}',
        'int main() {',' try {','  auto metadata=document();',
        '  if(::raise(SIGSTOP)!=0)throw std::runtime_error("exporter mapping stop failed");',
        "  bounded_native_io::gate('I');",'  bounded_native_io::emit(STDOUT_FILENO,metadata);return 0;',
        ' } catch(std::exception const&) {',
        '  bounded_native_io::print(STDERR_FILENO,"native legacy identity export refused\\n");return 1;',' }','}','']
    return '\n'.join(lines.rows).encode()
