"""Injected policy DATA models, not native SDK, generator or ROS evidence.

Root injects actual MF-loaded preparer/planner/emitter/parent/bounded modules.
Syntax-only RIHS values below are never installed native identities or accepted
execution proofs. This file has not been imported, generated or run by author.
"""
import copy
import unittest


def suite(preparer,planner,emitter,parent,bounded,*,emitter_source,helper_source):
    class Models(unittest.TestCase):
        def data(self):
            name='source_policy/action/Move';goal=name+'_Goal';result=name+'_Result';feedback=name+'_Feedback'
            send=name+'_SendGoal';get=name+'_GetResult';wrapped=name+'_FeedbackMessage'
            def f(name_,kind,nested='',capacity=0):return {'name':name_,'type_id':kind,'capacity':capacity,
                'string_capacity':0,'nested_type_name':nested,'default_value':''}
            def record(name_,fields):return {'type':name_,'fields':fields}
            rows={goal:record(goal,[f('value',6)]),result:record(result,[f('value',6)]),feedback:record(feedback,[f('value',6)]),
                'builtin_interfaces/msg/Time':record('builtin_interfaces/msg/Time',[f('sec',6),f('nanosec',7)]),
                'unique_identifier_msgs/msg/UUID':record('unique_identifier_msgs/msg/UUID',[f('uuid',51,capacity=16)]),
                'service_msgs/msg/ServiceEventInfo':record('service_msgs/msg/ServiceEventInfo',[f('event_type',3),
                    f('stamp',1,'builtin_interfaces/msg/Time'),f('client_gid',51,capacity=16),f('sequence_number',8)]),
                send+'_Request':record(send+'_Request',[f('goal_id',1,'unique_identifier_msgs/msg/UUID'),f('goal',1,goal)]),
                send+'_Response':record(send+'_Response',[f('accepted',15),f('stamp',1,'builtin_interfaces/msg/Time')]),
                get+'_Request':record(get+'_Request',[f('goal_id',1,'unique_identifier_msgs/msg/UUID')]),
                get+'_Response':record(get+'_Response',[f('status',2),f('result',1,result)]),
                wrapped:record(wrapped,[f('goal_id',1,'unique_identifier_msgs/msg/UUID'),f('feedback',1,feedback)])}
            for svc in (send,get):
                rows[svc]=record(svc,[f('request_message',1,svc+'_Request'),f('response_message',1,svc+'_Response'),f('event_message',1,svc+'_Event')])
                rows[svc+'_Event']=record(svc+'_Event',[f('info',1,'service_msgs/msg/ServiceEventInfo'),f('request',97,svc+'_Request',1),f('response',97,svc+'_Response',1)])
            rows[name]=record(name,[f('goal',1,goal),f('result',1,result),f('feedback',1,feedback),
                f('send_goal_service',1,send),f('get_result_service',1,get),f('feedback_message',1,wrapped)])
            # The fixed external runtime protocols are actual source declarations,
            # supplied by the real emitter, not a replacement native SDK/provider.
            runtime={n:record(n,[{'name':a,'type_id':({'uint8':3,'int8':2,'uint32':7,'int32':6,'int64':8}.get(t,1))+
                (48 if array else 96 if sequence and bound else 144 if sequence else 0),'capacity':count if array else bound if sequence else 0,
                'string_capacity':string,'nested_type_name':t if '/' in t else '','default_value':''}
                for a,t,count,array,string,sequence,bound in fields]) for n,fields in emitter.action_runtime_declarations()}
            def graph(source,root):
                seen=set()
                def visit(current):
                    if current in seen:return
                    seen.add(current)
                    for field in source[current]['fields']:
                        if field['nested_type_name']:visit(field['nested_type_name'])
                visit(root)
                return {'root':copy.deepcopy(source[root]),'references':[copy.deepcopy(source[n]) for n in sorted(seen-{root})]}
            protocol_roles={'send_goal':('service',rows,send),'get_result':('service',rows,get),
                'feedback_message':('message',rows,wrapped),'cancel_goal':('service',runtime,'action_msgs/srv/CancelGoal'),
                'status':('message',runtime,'action_msgs/msg/GoalStatusArray'),'uuid':('message',rows,'unique_identifier_msgs/msg/UUID'),
                'time':('message',rows,'builtin_interfaces/msg/Time')}
            protocols={role:{'kind':kind,'graph':graph(source,root)} for role,(kind,source,root) in protocol_roles.items()}
            components={role:graph(rows,root) for role,root in (('goal',goal),('result',result),('feedback',feedback))}
            official=graph(rows,name)
            expected={'kind':'action','official':official,'components':components,'standard_protocol':'ROS2_ACTION_EVENT_V1','protocol_associations':protocols}
            def native(value,number,kind='message'):
                doc={'native_identity':'RIHS01_'+format(number,'064x'),'native_baked_hash_available':False,**copy.deepcopy(value)}
                if kind=='message':doc['identity_provenance']='LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01'
                else:doc.update(identity_provenance='LEGACY_NATIVE_SERVICE_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01',
                    standard_protocol='ROS2_SERVICE_EVENT_V1',protocol_event_native_introspection=False)
                return doc
            actual={'native_identity':'RIHS01_'+'a'*64,'identity_provenance':'LEGACY_NATIVE_ACTION_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01',
                'native_baked_hash_available':False,'standard_protocol':'ROS2_ACTION_EVENT_V1','action_type_support_associations_verified':True,
                **copy.deepcopy(official),'protocol_associations':{role:native(value['graph'],i+10,value['kind']) for i,(role,value) in enumerate(protocols.items())}}
            schemas={'format_version':2,'kind':'complete-original-native-interface-declarations-not-identities','types':{name:expected}}
            metadata={'format':'uwvm-ros-official-bounded-native-v2','ros_version':2,'types':{name:{'official':actual,
                'components':{role:native(value,i+1) for i,(role,value) in enumerate(components.items())}}}}
            return name,schemas,metadata
        def refuse(self,schemas,metadata):
            for api in (preparer,planner):
                with self.subTest(api=api.__name__),self.assertRaises(ValueError):api.legacy_metadata_provenance(metadata,schemas)
        def test_complete_source_root_components_and_protocols_associate(self):
            name,schemas,metadata=self.data();a=preparer.legacy_metadata_provenance(metadata,schemas)
            self.assertEqual(a,planner.legacy_metadata_provenance(metadata,schemas));self.assertEqual(a['types'][name]['kind'],'action')
            self.assertEqual(set(a['types'][name]['protocol_associations']),{'send_goal','get_result','feedback_message','cancel_goal','status','uuid','time'})
        def test_payload_only_service_provenance_cannot_impersonate_action(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['identity_provenance']='LEGACY_NATIVE_SERVICE_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01'
            self.refuse(schemas,metadata)
        def test_missing_cancel_or_status_native_proof_refused(self):
            for role in ('cancel_goal','status'):
                name,schemas,metadata=self.data();del metadata['types'][name]['official']['protocol_associations'][role];self.refuse(schemas,metadata)
        def test_missing_actual_feedback_component_refused(self):
            name,schemas,metadata=self.data();del metadata['types'][name]['components']['feedback'];self.refuse(schemas,metadata)
        def test_modern_root_or_payload_cannot_downgrade(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['native_baked_hash_available']=True;self.refuse(schemas,metadata)
            name,schemas,metadata=self.data();metadata['types'][name]['components']['goal']['native_baked_hash_available']=True;self.refuse(schemas,metadata)
        def test_modern_protocol_service_cannot_downgrade(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['protocol_associations']['send_goal']['native_baked_hash_available']=True
            self.refuse(schemas,metadata)
        def test_native_cancel_capacity_cannot_be_projected(self):
            name,schemas,metadata=self.data();value=metadata['types'][name]['official']['protocol_associations']['cancel_goal']
            row=next(r for r in value['references'] if r['type']=='action_msgs/srv/CancelGoal_Response');row['fields'][1]['capacity']=64
            self.refuse(schemas,metadata)
        def test_incomplete_root_event_closure_refused(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['references'].pop();self.refuse(schemas,metadata)
        def test_association_flag_is_exact_but_not_native_execution_credit(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['action_type_support_associations_verified']=1
            self.refuse(schemas,metadata)
        def test_present_event_requires_typed_boolean(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['protocol_associations']['get_result']['protocol_event_native_introspection']='false'
            self.refuse(schemas,metadata)
        def test_four_separate_aliases_are_required(self):
            with self.assertRaises(ValueError):planner.action_client_roles({'alias':'send','action_aliases':{'send_goal':'send'}})
        def test_cancel_any_is_optional_and_never_inferred(self):
            request={'alias':'send','action_aliases':{'send_goal':'send','cancel_own':'own','feedback':'feedback','result':'result'}}
            aliases,operations=planner.action_client_roles(request);self.assertNotIn('cancel_any',aliases)
            self.assertEqual(operations['cancel_own'],'action.cancel-own-goal');request['action_aliases']['cancel_any']='any'
            self.assertEqual(planner.action_client_roles(request)[1]['cancel_any'],'action.cancel-any-goal')
        def test_alias_collision_or_wrong_primary_refused(self):
            request={'alias':'other','action_aliases':{'send_goal':'send','cancel_own':'own','feedback':'feedback','result':'result'}}
            with self.assertRaises(ValueError):planner.action_client_roles(request)
            request['alias']='send';request['action_aliases']['result']='send'
            with self.assertRaises(ValueError):planner.action_client_roles(request)
        def test_declared_defaults_refused_before_legacy_sidecar_or_source(self):
            value=bounded.Field('value','int32');value.native_default='0.0'
            record=bounded.Record(2,'source_policy/msg/Reading',[value],None,{})
            interface=bounded.Interface(2,record.name,'message',None,components={'message':record})
            records={(2,record.name):record}
            with self.assertRaises(ValueError):preparer.legacy_schemas(parent,bounded,records,[interface],emitter)
            with self.assertRaises(parent.GenerationError):emitter.declarations(parent,bounded,records,[interface])
        def test_document_warms_before_original_stop_and_gate(self):
            text=emitter_source.decode();self.assertLess(text.index('auto metadata=document();'),text.index('::raise(SIGSTOP)'))
            self.assertLess(text.index('::raise(SIGSTOP)'),text.index("gate('I')"))
            source=helper_source.decode()
            for token in ('goal_service_type_support!=send_support','cancel_service_type_support!=cancel_support',
                'status_message_type_support!=status_support','get_action_type_support_handle<Native>()','require_action_runtime(runtime)',
                'has_baked_hash<std::remove_pointer_t<decltype(support)>>'):
                self.assertIn(token,source)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Models)
