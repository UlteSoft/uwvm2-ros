#!/usr/bin/env python3
"""Exercise numbered navigation without claiming robot or Guest acceptance.

Small temporary sources below are fixtures for the catalog's identity and
confinement rules. They are never compiled, imported as a robot application or
credited as vendor examples. Run only in the admitted Linux test owner.
"""
import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import tarfile
import io
import unittest


ROOT=Path(__file__).resolve().parents[5]
spec=importlib.util.spec_from_file_location('numbered_catalog',ROOT/'sdk/tools/prepare_test_catalog.py')
catalog=importlib.util.module_from_spec(spec);spec.loader.exec_module(catalog)


class NumberedCatalogContracts(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory(prefix='numbered-ros-catalog-')
        self.addCleanup(self.temp.cleanup)
        self.base=Path(self.temp.name);self.source=self.base/'source';self.source.mkdir()

    def write(self,name,body):
        path=self.source/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_text(body)
        return path

    def test_separate_platform_language_sequences_and_exact_source_pins(self):
        bodies={'ros_unitree/check_a.py':'if __name__=="__main__": assert True\n',
                'ros_unitree/check_b.py':'if __name__=="__main__": assert True\n',
                'ros_unitree/check.c':'int main(void){return 0;}\n',
                'ros2_contract/check.c':'int main(void){return 0;}\n'}
        for name,body in bodies.items():self.write(name,body)
        out=self.base/'catalog';result=catalog.prepare(self.source,out)
        self.assertEqual(result['cases_prepared'],4)
        groups={}
        for row in result['cases']:
            key=(row['platform'],row['language']);groups[key]=groups.get(key,0)+1
            folder=out/row['id'];self.assertTrue(folder.name.startswith('%04d.'%groups[key]))
            self.assertEqual(row['source_sha256'],hashlib.sha256(bodies[row['source'][15:]].encode()).hexdigest())
            self.assertEqual(row['default_authority'],[])
            self.assertEqual(row['official_upstream_suite_credit'],0)
            self.assertFalse(row['guest_language_qualification'])
            self.assertEqual(json.loads((folder/'case.json').read_bytes()),row)
        self.assertEqual(groups,{('unitree','py'):2,('unitree','c'):1,('ros2','c'):1})
        self.assertEqual(result['actual_tests_executed'],0)

    def test_build_and_acquisition_tools_are_support_not_test_cases(self):
        self.write('ros/build_product.py','"""unittest in prose is not a test import."""\nif __name__=="__main__": pass\n')
        self.write('ros/pack_ros2_contracts.py','if __name__=="__main__": pass\n')
        result=catalog.prepare(self.source,self.base/'catalog')
        self.assertEqual(result['cases'],[])
        self.assertEqual(len(result['support_entrypoints']),2)

    def test_symlinks_generated_outputs_and_archives_do_not_add_cases(self):
        path=self.write('ros/test_one.py','if __name__=="__main__": assert True\n')
        (self.source/'ros/test_link.py').symlink_to(path)
        for folder in ('generated','records','results','acceptance','fixtures'):
            self.write('ros/'+folder+'/test_hidden.py',path.read_text())
        result=catalog.prepare(self.source,self.base/'catalog')
        self.assertEqual(result['cases_prepared'],1)

    def test_existing_output_and_output_beneath_sources_refuse(self):
        out=self.base/'exists';out.mkdir()
        with self.assertRaises(ValueError):catalog.prepare(self.source,out)
        with self.assertRaises(ValueError):catalog.prepare(self.source,self.source/'nested')

    def test_library_helpers_without_entry_points_are_dependencies(self):
        self.write('ros/helper.py','def test_helper(): return True\n')
        result=catalog.prepare(self.source,self.base/'catalog')
        self.assertEqual(result['cases_prepared'],0)

    def test_new_source_sorts_before_existing_without_renumbering(self):
        self.write('ros/check_z.py','if __name__=="__main__": assert True\n')
        old=catalog.prepare(self.source,self.base/'old')
        self.write('ros/check_a.py','if __name__=="__main__": assert True\n')
        new=catalog.prepare(self.source,self.base/'new',self.base/'old/index.json')
        by_source={row['source']:row for row in new['cases']}
        self.assertEqual(by_source[old['cases'][0]['source']]['id'],old['cases'][0]['id'])
        self.assertTrue(by_source['support/source/ros/check_a.py']['id'].split('/')[-1].startswith('0002.'))

    def test_missing_or_duplicate_previous_case_refuses(self):
        source=self.write('ros/check_a.py','if __name__=="__main__": assert True\n')
        old=catalog.prepare(self.source,self.base/'old');source.unlink()
        with self.assertRaises(ValueError):catalog.prepare(self.source,self.base/'missing',self.base/'old/index.json')
        old['cases']*=2
        bad=self.base/'bad.json';bad.write_text(json.dumps(old))
        with self.assertRaises(ValueError):catalog.prepare(self.source,self.base/'duplicate',bad)

    def test_private_language_tests_are_numbered_without_fixture_or_execution_credit(self):
        for name,body in {
                'native/rust/vector.rs':'#[test]\nfn values() { assert_eq!(1,1); }\n',
                'native/tinygo/vector_test.go':'package sdk\nfunc TestValues(t *testing.T) {}\n',
                'native/zig/vector.zig':'test "values" { try expect(true); }\n',
                'native/moonbit/vector_wbtest.mbt':'test "values" { assert_true(true) }\n',
                'native/inline/vector.rs.block1':'#[test]\nfn values() {}\n'}.items():
            self.write(name,body)
        result=catalog.prepare(self.source,self.base/'languages')
        self.assertEqual({row['language']for row in result['cases']},{'rust','tinygo','zig','moonbit'})
        self.assertEqual(result['cases_prepared'],4)
        self.assertEqual(result['actual_tests_executed'],0)
        self.assertTrue(all(not row['guest_language_qualification']for row in result['cases']))

    def test_historical_rust_label_and_identity_survive_language_classifier_update(self):
        self.write('native/rust/vector.rs','fn main() {}\n')
        old=catalog.prepare(self.source,self.base/'original')
        row=old['cases'][0];row['language']='rs'
        row['id']='platform/ros/language/rs/0001.original-rust'
        baseline=self.base/'historical.json';baseline.write_text(json.dumps(old))
        new=catalog.prepare(self.source,self.base/'rebased',baseline)
        self.assertEqual(new['cases'][0]['id'],row['id'])
        self.assertEqual(new['cases'][0]['language'],'rs')

    def test_pinned_archive_matches_directory_catalog_and_refuses_changed_pin(self):
        self.write('ros/check_a.py','if __name__=="__main__": assert True\n')
        self.write('ros/check_b.c','int main(void){return 0;}\n')
        archive=self.base/'sources.tar.gz'
        with tarfile.open(archive,'w:gz')as output:
            for path in sorted(self.source.rglob('*')):
                if path.is_file():output.add(path,arcname='test/0073.ros-tests/support/source/'+path.relative_to(self.source).as_posix())
        pin=hashlib.sha256(archive.read_bytes()).hexdigest()
        plain=catalog.prepare(self.source,self.base/'plain')
        packed=catalog.prepare(self.source,self.base/'packed',source_archive=archive,source_archive_sha256=pin)
        self.assertEqual(plain['cases'],packed['cases'])
        self.assertFalse(packed['source_archive']['extracted'])
        with self.assertRaises(ValueError):catalog.prepare(self.source,self.base/'changed',source_archive=archive,source_archive_sha256='0'*64)

    def test_archive_traversal_is_rejected_without_extraction(self):
        archive=self.base/'invalid.tar'
        with tarfile.open(archive,'w')as output:
            member=tarfile.TarInfo('test/0073.ros-tests/support/source/../escape.py')
            raw=b'if __name__=="__main__": assert True\n';member.size=len(raw);output.addfile(member,io.BytesIO(raw))
        with self.assertRaises(ValueError):catalog.prepare(self.source,self.base/'refused',source_archive=archive,
            source_archive_sha256=hashlib.sha256(archive.read_bytes()).hexdigest())

    def test_relocated_nvidia_prefixes_keep_the_selected_platform(self):
        for name in ('groot','isaac_ros','isaac_sim'):
            self.write('task09-nvidia/'+name+'/test_codec.py','if __name__=="__main__": assert True\n')
        result=catalog.prepare(self.source,self.base/'nvidia')
        self.assertEqual({row['platform']for row in result['cases']},{'nvidia-groot','nvidia-isaac-ros','nvidia-isaac-sim'})


if __name__=='__main__':unittest.main(verbosity=2)
