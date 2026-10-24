"""Injected pure-policy regressions; no compiler, ROS entity or native proof.

Root must load the reviewed external preparer bytes itself and pass the actual
MF-bound native exporter source. Fixture names below are synthetic DATA used to
check safe source generation; they are never vendor official cases or identities.
This file has not been imported or run by its author.
"""
import unittest


def suite(api, *, inventory, native_exporter_source):
    class Policy(unittest.TestCase):
        def catalog(self, version, kind='message'):
            category={'message':'msg','service':'srv','action':'action'}[kind]
            suffix={'message':'msg','service':'srv','action':'action'}[kind]
            name='policy_fixture/'+(category+'/' if version==2 else '')+'Reading'
            return {'format':'uwvm-ros-custom-bounded-v2','types':[{'ros_version':version,
                'type':name,'file':'policy_fixture/'+category+'/Reading.'+suffix}]}
        def headers(self, version, kind='message'):
            row=self.catalog(version,kind)['types'][0]
            header='policy_fixture/'+({'message':'msg','service':'srv','action':'action'}[kind]+'/reading.hpp' if version==2 else
                'Reading'+('Action' if kind=='action' else '')+'.h')
            return {row['type']:{'type':row['type'],'include':header,'pin':None}}
        def test_no_traversal_or_control_include_role(self):
            for value in ('../source.msg','/source.msg','a//b.msg','a/./b.msg','a\\b.msg','a\x00.msg'):
                with self.subTest(value=value),self.assertRaises(ValueError):api.relative(value)
        def test_boolean_is_not_an_observed_numeric_pin(self):
            pin={'path':'/actual/file','bytes':False,'sha256':'0'*64,
                'device':1,'inode':1,'mtime_ns':1,'ctime_ns':1}
            with self.assertRaises(ValueError):api.ordinary_pin(pin)
        def test_support_inventory_cannot_name_a_provider(self):
            with self.assertRaises(ValueError):
                api.source_modules({'provider.py':None},read=lambda *_:self.fail('unexpected read'),
                    load=lambda *_:self.fail('unexpected load'))
        def test_unknown_vendor_refused_before_any_file_read(self):
            request={'format_version':1,'vendor':'not_a_vendor','model':'fixture','sdk_revision':'fixture',
                'ros_version':2,'upstream_lock':None,'native_headers':None,'support':{}}
            with self.assertRaises(ValueError):
                api.source_inputs(inventory,request,read=lambda *_:self.fail('unexpected read'))
        def test_closed_request_refuses_an_extra_runtime_setting(self):
            with self.assertRaises(ValueError):
                api.source_inputs(inventory,{'runtime_update':True},read=lambda *_:self.fail('unexpected read'))
        def test_ros1_native_header_is_exact(self):
            catalog=self.catalog(1);headers=self.headers(1);headers[catalog['types'][0]['type']]['include']='other/Reading.h'
            with self.assertRaises(ValueError):api.exporter(native_exporter_source,catalog,headers,1)
        def test_ros2_native_header_category_is_exact(self):
            catalog=self.catalog(2);headers=self.headers(2);headers[catalog['types'][0]['type']]['include']='policy_fixture/srv/reading.hpp'
            with self.assertRaises(ValueError):api.exporter(native_exporter_source,catalog,headers,2)
        def test_official_metadata_helper_namespace_is_unchanged(self):
            raw,digest=api.exporter(native_exporter_source,self.catalog(2),self.headers(2),2)
            text=native_exporter_source.decode();helper=text[text.index('namespace\n{\n'):text.index('\nint main()\n')]
            self.assertIn(helper.encode(),raw)
            self.assertEqual(digest,api.hashlib.sha256(helper.encode()).hexdigest())
            self.assertIn(b'raise(SIGSTOP)',raw);self.assertIn(b"gate('I')",raw)
        def test_ros1_action_uses_actual_wrapper_and_all_three_components(self):
            raw,_=api.exporter(native_exporter_source,self.catalog(1,'action'),self.headers(1,'action'),1)
            for spelling in (b'MD5Sum<::policy_fixture::ReadingAction>',
                b'component<::policy_fixture::ReadingGoal>("goal",false)',
                b'component<::policy_fixture::ReadingResult>("result",true)',
                b'component<::policy_fixture::ReadingFeedback>("feedback",true)'):
                self.assertIn(spelling,raw)
        def test_service_first_inventory_has_no_invented_message_dependency(self):
            raw,_=api.exporter(native_exporter_source,self.catalog(2,'service'),self.headers(2,'service'),2)
            main=raw[raw.index(b'\nint main() {'):]
            self.assertIn(b'get_service_type_support_handle<::policy_fixture::srv::Reading>()',main)
            self.assertIn(b'component<::policy_fixture::srv::Reading::Request>',main)
            self.assertNotIn(b"print(output(),',');",main)
        def legacy_data(self):
            # Syntactic sentinel used only by negative source-policy cases;
            # never exported, bound, accepted or labelled a native identity.
            shape={'root':{'type':'policy_fixture/msg/Reading','fields':[{'name':'value','type_id':6,'capacity':0,'string_capacity':0,'nested_type_name':'','default_value':''}]},'references':[]}
            schemas={'format_version':1,'kind':'complete-original-native-message-declarations-not-identities','types':{'policy_fixture/msg/Reading':shape}}
            official={'native_identity':'RIHS01_'+'a'*64,'identity_provenance':api.LEGACY_PROVENANCE,'native_baked_hash_available':False,**shape}
            return schemas,{'format':'uwvm-ros-official-bounded-native-v2','ros_version':2,'types':{'policy_fixture/msg/Reading':{'official':official,'components':{'message':dict(official)}}}}
        def test_omitted_identity_mode_keeps_original_exporter(self):
            self.assertEqual(api.identity_mode({'ros_version':2}),api.ORIGINAL_IDENTITY_MODE)
        def test_unknown_identity_mode_is_refused(self):
            with self.assertRaises(ValueError):api.identity_mode({'ros_version':2,'identity_mode':'automatic-fallback'})
        def test_legacy_mode_is_not_ros1(self):
            for version in (1,True,2.0):
                with self.subTest(version=version),self.assertRaises(ValueError):
                    api.identity_mode({'ros_version':version,'identity_mode':'legacy-introspection'})
        def test_legacy_mode_needs_every_fixed_current_source_role(self):
            with self.assertRaises(ValueError):
                api.source_modules({name:None for name in api.SUPPORT},mode=api.LEGACY_IDENTITY_MODE,
                    read=lambda *_:self.fail('unexpected read'),load=lambda *_:self.fail('unexpected module load'))
        def test_legacy_metadata_requires_explicit_derived_provenance(self):
            schemas,metadata=self.legacy_data();metadata['types']['policy_fixture/msg/Reading']['official'].pop('identity_provenance')
            with self.assertRaises(ValueError):api.legacy_metadata_provenance(metadata,schemas)
        def test_original_mode_cannot_accept_legacy_metadata(self):
            _,metadata=self.legacy_data()
            with self.assertRaises(ValueError):api.original_metadata_provenance(metadata,2)
        def test_modern_baked_metadata_cannot_downgrade(self):
            schemas,metadata=self.legacy_data();metadata['types']['policy_fixture/msg/Reading']['official']['native_baked_hash_available']=True
            with self.assertRaises(ValueError):api.legacy_metadata_provenance(metadata,schemas)
        def test_legacy_root_component_identity_must_match(self):
            schemas,metadata=self.legacy_data();metadata['types']['policy_fixture/msg/Reading']['components']['message']['native_identity']='RIHS01_'+'b'*64
            with self.assertRaises(ValueError):api.legacy_metadata_provenance(metadata,schemas)
        def test_legacy_metadata_cannot_narrow_or_project_native_declarations(self):
            import copy
            schemas,metadata=self.legacy_data();row=metadata['types']['policy_fixture/msg/Reading'];row['official']['root']=copy.deepcopy(row['official']['root'])
            row['official']['root']['fields'][0]['type_id']=7;row['components']['message']=copy.deepcopy(row['official'])
            with self.assertRaises(ValueError):api.legacy_metadata_provenance(metadata,schemas)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policy)
