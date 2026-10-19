"""Injected source/DATA policy models, not native SDK or exporter evidence.

Root supplies actual current-MF preparer/planner modules and admitted C++/Python
source bytes. The synthetic syntax-only IDs below are never installed native
identities or acceptance facts. Author has not imported or run this file.
"""
import copy
import unittest


def suite(preparer, planner, *, emitter_source, helper_source):
    class Models(unittest.TestCase):
        def data(self):
            def f(name,type_id,nested='',capacity=0):
                return {'name':name,'type_id':type_id,'capacity':capacity,'string_capacity':0,
                    'nested_type_name':nested,'default_value':''}
            name='source_policy/srv/Measure';request=name+'_Request';response=name+'_Response';event=name+'_Event'
            rows={request:{'type':request,'fields':[f('value',6)]},response:{'type':response,'fields':[f('value',7)]},
                'builtin_interfaces/msg/Time':{'type':'builtin_interfaces/msg/Time','fields':[f('sec',6),f('nanosec',7)]},
                'service_msgs/msg/ServiceEventInfo':{'type':'service_msgs/msg/ServiceEventInfo','fields':[
                    f('event_type',3),f('stamp',1,'builtin_interfaces/msg/Time'),f('client_gid',51,capacity=16),f('sequence_number',8)]},
                event:{'type':event,'fields':[f('info',1,'service_msgs/msg/ServiceEventInfo'),f('request',97,request,1),f('response',97,response,1)]},
                name:{'type':name,'fields':[f('request_message',1,request),f('response_message',1,response),f('event_message',1,event)]}}
            def graph(root,refs=()):return {'root':copy.deepcopy(rows[root]),'references':[copy.deepcopy(rows[k]) for k in sorted(refs)]}
            components={'request':graph(request),'response':graph(response)};official=graph(name,set(rows)-{name})
            schemas={'format_version':2,'kind':'complete-original-native-interface-declarations-not-identities',
                'types':{name:{'kind':'service','official':official,'components':components,'standard_protocol':'ROS2_SERVICE_EVENT_V1'}}}
            def message(graph_,code):return {'native_identity':'RIHS01_'+code*64,
                'identity_provenance':'LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01','native_baked_hash_available':False,**graph_}
            value={'native_identity':'RIHS01_'+'a'*64,'identity_provenance':'LEGACY_NATIVE_SERVICE_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01',
                'native_baked_hash_available':False,'standard_protocol':'ROS2_SERVICE_EVENT_V1','protocol_event_native_introspection':False,**official}
            metadata={'format':'uwvm-ros-official-bounded-native-v2','ros_version':2,'types':{name:{'official':value,
                'components':{'request':message(components['request'],'b'),'response':message(components['response'],'c')}}}}
            # Model independent serialized source-sidecar/raw-export documents.
            return name,copy.deepcopy(schemas),copy.deepcopy(metadata)
        def refuse(self,schemas,metadata):
            for api in (preparer,planner):
                with self.subTest(api=api.__name__),self.assertRaises(ValueError):api.legacy_metadata_provenance(metadata,schemas)
        def test_separate_root_and_component_source_data_associations(self):
            name,schemas,metadata=self.data()
            a=preparer.legacy_metadata_provenance(metadata,schemas);b=planner.legacy_metadata_provenance(metadata,schemas)
            self.assertEqual(a,b);self.assertEqual(a['types'][name]['component_identities']['request'],'RIHS01_'+'b'*64)
            self.assertFalse(a['types'][name]['protocol_event_native_introspection'])
        def test_service_cannot_impersonate_message_provenance(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['identity_provenance']=preparer.LEGACY_PROVENANCE
            self.refuse(schemas,metadata)
        def test_modern_service_cannot_downgrade(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['native_baked_hash_available']=True
            self.refuse(schemas,metadata)
        def test_component_modern_downgrade_is_independently_refused(self):
            name,schemas,metadata=self.data();metadata['types'][name]['components']['request']['native_baked_hash_available']=True
            self.refuse(schemas,metadata)
        def test_missing_real_response_is_refused(self):
            name,schemas,metadata=self.data();del metadata['types'][name]['components']['response']
            self.refuse(schemas,metadata)
        def test_no_false_installed_event_status(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['protocol_event_native_introspection']='false'
            self.refuse(schemas,metadata)
        def test_no_native_capacity_projection(self):
            name,schemas,metadata=self.data();metadata['types'][name]['components']['request']['root']['fields'][0]['capacity']=1
            self.refuse(schemas,metadata)
        def test_whole_protocol_event_graph_must_match(self):
            name,schemas,metadata=self.data();metadata['types'][name]['official']['references'].pop()
            self.refuse(schemas,metadata)
        def test_no_unknown_protocol_or_action_envelope(self):
            name,schemas,metadata=self.data();schemas['types'][name]['standard_protocol']='guessed-legacy-event'
            self.refuse(schemas,metadata)
            name,schemas,metadata=self.data();schemas['types'][name]['kind']='action';self.refuse(schemas,metadata)
        def test_complete_metadata_materializes_before_stop_and_gate(self):
            text=emitter_source.decode();self.assertLess(text.index('auto metadata=document();'),text.index('::raise(SIGSTOP)'))
            self.assertLess(text.index('::raise(SIGSTOP)'),text.index("gate('I')"))
            self.assertIn("bounded.ros2_interface_schema(parent,records,item)",text)
            self.assertIn("item.kind in ('message','service')",text)
        def test_real_service_members_and_typed_associations_are_mandatory(self):
            text=helper_source.decode()
            for value in ('service_introspection.hpp','get_service_type_support_handle<Native>()',
                'actual->request_members_!=request_members','actual->response_members_!=response_members',
                'has_baked_hash<std::remove_pointer_t<decltype(support)>>',
                'protocol_event_native_introspection','matches_schema(message_members_for<::builtin_interfaces::msg::Time>()'):
                self.assertIn(value,text)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Models)
