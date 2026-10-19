"""Injected policy/DATA cases; no official testcase, compiler or ROS executes.

Root supplies all three modules after full current-MF admission. These cases
exercise actual matching/argv/accounting functions, not a copied implementation
model. Real observer fixture and native/Wasm effect cases are separate lanes.
"""
import json
import errno
import unittest


def suite(port,producer,differential):
    def original(universe='unittest-loaded-method',identity=None):
        identity=identity or port.METHOD
        return {'key':{'suite':'actual-rospy-order','universe':universe,'id':identity},
                'native_status':'PASS','native_observation':{'name':'test_subscriber_first','classname':'TestPubSubOrder'}}
    spec={'snapshot':'actual-original','source_test_files':list(port.FILES)}
    lock={'snapshots':[{'id':'actual-original','repository':port.REPOSITORY,'commit':port.COMMIT}]}
    class PortPolicy(unittest.TestCase):
        def test_locked_loaded_method_matches(self):
            self.assertTrue(port.supports_discovered_case(original(),spec,lock))
        def test_actual_junit_identity_keeps_original_raw_file_universe(self):
            self.assertTrue(port.supports_discovered_case(original('junit-file:'+'a'*64,'actual-raw-key'),spec,lock))
        def test_wrong_official_method_refused(self):
            self.assertFalse(port.supports_discovered_case(original(identity='test_pubsub_order.TestPubSubOrder.test_other'),spec,lock))
        def test_same_name_in_another_class_refused(self):
            row=original('junit-file:'+'a'*64);row['native_observation']['classname']='CustomFake'
            self.assertFalse(port.supports_discovered_case(row,spec,lock))
        def test_mutable_branch_is_not_source_lock(self):
            selected={'snapshots':[dict(lock['snapshots'][0],commit='noetic-devel')]}
            self.assertFalse(port.supports_discovered_case(original(),spec,selected))
        def test_unitree_demo_does_not_become_rospy_official_case(self):
            selected={'snapshots':[dict(lock['snapshots'][0],repository='https://github.com/unitreerobotics/unitree_mujoco.git')]}
            self.assertFalse(port.supports_discovered_case(original(),spec,selected))
        def test_all_original_fixture_sources_required(self):
            self.assertFalse(port.supports_discovered_case(original(),dict(spec,source_test_files=list(port.FILES)[:1]),lock))
        def test_positive_complete_actual_authority_parser(self):
            argv=port.policy_arguments('positive',master_uri='http://127.0.0.1:11311/',uid=1000)
            records=differential.authority(argv,1,'positive',port.GRANTS)
            self.assertEqual([v['operands'][1:] for v in records if v['action']=='single-grant'],port.GRANTS)
            self.assertEqual(sum(v['action']=='single-set' for v in records),sum(len(port.grant_limits(v[1])) for v in port.GRANTS))
        def test_denied_policy_adds_only_audit_budgets(self):
            argv=port.policy_arguments('default-denied',master_uri='http://127.0.0.1:11311/')
            records=differential.authority(argv,1,'default-denied',[])
            self.assertFalse(any(v['action']=='single-grant' for v in records))
        def test_string_effect_uses_actual_bytes(self):
            self.assertEqual(port.normalize('worker',b'{"message":"hi [42]"}',{'mode':'positive'}),{'message':'hi [42]'})
            with self.assertRaises(ValueError):port.normalize('worker',b'{"message":"hi [43]"}',{'mode':'positive'})
        def test_denied_quiet_not_accepted_from_worker(self):
            with self.assertRaises(ValueError):port.normalize('worker',b'{"effects":0,"samples":2,"elapsed_ns":2000000000}',{'mode':'default-denied'})
        def test_stalled_single_sample_does_not_qualify_quiet(self):
            with self.assertRaises(ValueError):port.normalize('native_peer',b'{"effects":0,"samples":1,"elapsed_ns":3000000000}',{'mode':'default-denied'})
            self.assertEqual(port.normalize('native_peer',b'{"effects":0,"samples":30,"elapsed_ns":2000000000}',{'mode':'default-denied'})['samples'],30)
        def test_literal_catalog_uses_nested_actual_file_pin(self):
            pin={'path':'/native/sdk-rootfs/lib/libc.so','bytes':42,'sha256':'a'*64,'device':1,'inode':2,'mtime_ns':3,'ctime_ns':4}
            row={'relative':'sdk-rootfs/lib/libc.so','kind':'file','bytes':42,'sha256':'a'*64,'pin':pin,'mode':420,'nlink':1}
            self.assertEqual(differential.catalog_file({'root':'/native','catalog_format':'literal-chroot-tree-v1'},row),pin)
        def test_literal_alias_is_never_host_authority(self):
            catalog={'root':'/native','catalog_format':'literal-chroot-tree-v1'}
            row={'relative':'sdk-rootfs/usr/lib/alias','kind':'symlink','target':'/etc/ssl'}
            self.assertIsNone(differential.catalog_file(catalog,row))
            with self.assertRaises(ValueError):differential.catalog_file(catalog,dict(row,resolved='/etc/ssl'))
        def test_literal_file_cannot_substitute_an_external_pin(self):
            pin={'path':'/host/libc.so','bytes':42,'sha256':'a'*64,'device':1,'inode':2,'mtime_ns':3,'ctime_ns':4}
            row={'relative':'sdk-rootfs/lib/libc.so','kind':'file','bytes':42,'sha256':'a'*64,'pin':pin}
            with self.assertRaises(ValueError):differential.catalog_file({'root':'/native','catalog_format':'literal-chroot-tree-v1'},row)
        def test_only_exact_retired_positive_rpc_enoent_is_exempt(self):
            error=FileNotFoundError(errno.ENOENT,'actual unlink')
            self.assertTrue(differential.retired_case_socket('/owned','/owned/case-000001-positive/rpc',error))
            for path in ('/owned/case-000001-default-denied/rpc','/owned/case-000001-positive/regular',
                         '/owned/case-000001-positive/nested/rpc','/input/case-000001-positive/rpc'):
                self.assertFalse(differential.retired_case_socket('/owned',path,error))
            self.assertFalse(differential.retired_case_socket('/owned','/owned/case-000001-positive/rpc',PermissionError(errno.EACCES,'not unlink')))
        def test_real_compile_recipe_full_md_no_assert_disable(self):
            tools={v:'/selected/'+v for v in ('cc','wasm_ld','libc','builtins','validator')}
            compile_,link,validate=producer.recipes('/frozen/source','/owned/out',tools,'/actual/sysroot','/actual/wasm-ld')
            self.assertIn('-MD',compile_);self.assertIn('-UNDEBUG',compile_);self.assertNotIn('-MMD',compile_)
            self.assertEqual(compile_[compile_.index('-MT')+1],'/owned/out/official-pubsub.o')
            self.assertIn('-Wl,--entry=_start',link);self.assertIn('-Wl,--max-memory=16777216',link)
            self.assertEqual(validate,['/selected/validator','validate','/owned/out/official-pubsub.wasm'])
    return unittest.defaultTestLoader.loadTestsFromTestCase(PortPolicy)
