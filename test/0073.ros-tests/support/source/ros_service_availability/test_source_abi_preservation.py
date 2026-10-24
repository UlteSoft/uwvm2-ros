"""Source-only optional/legacy catalog policies; Root executes after review.

Inject the actual MF-admitted abi_contract helper. Cases copy only actual source
metadata into private scratch and never import/compile/call Host/native code.
Synthetic source mutations are negative consistency data, not native proofs.
"""
import hashlib
import json
from pathlib import Path
import unittest


def suite(helper, *, scratch, guard):
    scratch=Path(scratch)
    class Policies(unittest.TestCase):
        def setUp(self):
            guard();self.root=scratch/self._testMethodName;self.root.mkdir()
            for rel in (helper.HOST,'sdk/origin/abi/abi-v1.json','sdk/origin/abi/service-client-v1.json'):
                target=self.root/rel;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes((helper.ROOT/rel).read_bytes())
            self.original=(self.root/'sdk/origin/abi/abi-v1.json').read_bytes()
        def tearDown(self):guard()
        def optional(self):return json.loads((self.root/'sdk/origin/abi/service-client-v1.json').read_text())
        def save(self, value):
            (self.root/'sdk/origin/abi/service-client-v1.json').write_text(json.dumps(value,indent=2)+'\n')
        def mutate_host(self, old, new, rebind_source=False):
            path=self.root/helper.HOST;text=path.read_text();self.assertIn(old,text);path.write_text(text.replace(old,new,1))
            if rebind_source:
                value=self.optional();value['authority_source']['sha256']=hashlib.sha256(path.read_bytes()).hexdigest();self.save(value)
        def test_exact_current_host_preserves_full_legacy_catalog(self):
            value=helper.extract(self.root)
            self.assertEqual(value,json.loads(self.original));self.assertEqual([value['modules'][f'uwvm_ros{n}_v1']['function_count'] for n in (1,2)],[38,43])
            self.assertEqual((self.root/'sdk/origin/abi/abi-v1.json').read_bytes(),self.original)
        def test_new_host_without_optional_association_is_drift(self):
            (self.root/'sdk/origin/abi/service-client-v1.json').unlink()
            self.assertNotEqual(helper.extract(self.root),json.loads(self.original))
        def test_unbound_host_comment_change_is_refused(self):
            path=self.root/helper.HOST;path.write_bytes(path.read_bytes()+b'\n// unbound negative-policy mutation\n')
            with self.assertRaises(ValueError):helper.extract(self.root)
        def test_optional_numeric_signature_cannot_be_retagged_as_same(self):
            self.mutate_host('client_features(::std::uint64_t a','client_features(::std::uint32_t a',True)
            with self.assertRaises(ValueError):helper.extract(self.root)
        def test_optional_namespace_must_be_the_actual_registered_name(self):
            self.mutate_host('u8"uwvm_ros1_client_v1"','u8"uwvm_ros1_client_v2"',True)
            with self.assertRaises(ValueError):helper.extract(self.root)
        def test_legacy_declaration_digest_is_not_optional(self):
            value=self.optional();value['legacy_preservation']['original_module_declaration_sha256']['uwvm_ros1_v1']='0'*64;self.save(value)
            with self.assertRaises(ValueError):helper.extract(self.root)
        def test_changed_legacy_description_is_not_just_hash_rebinding(self):
            path=self.root/'sdk/origin/abi/abi-v1.json';value=json.loads(path.read_text());value['authority_contract']='unreviewed';path.write_text(json.dumps(value,indent=2)+'\n')
            option=self.optional();option['legacy_preservation']['catalog_sha256']=hashlib.sha256(path.read_bytes()).hexdigest();self.save(option)
            with self.assertRaises(ValueError):helper.extract(self.root)
        def test_extra_optional_function_and_bad_backend_are_refused(self):
            for change in ('extra','backend'):
                value=self.optional()
                if change=='extra':value['modules']['uwvm_ros1_client_v1']['functions'].append({'name':'grant','params':[],'results':['i32']})
                else:value['modules']['uwvm_ros1_client_v1']['backend']=2
                self.save(value)
                with self.assertRaises(ValueError):helper.extract(self.root)
                self.save(json.loads((helper.ROOT/'sdk/origin/abi/service-client-v1.json').read_text()))
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policies)
