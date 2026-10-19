#!/usr/bin/env python3
"""Replay actual retained Unitree selection membership under the live guard.

This read-only regression suite consumes the genuine failed b7 report and its
three preserved native profile-selection documents. It never starts a compiler,
Runtime, ROS node or simulator. Current file identities come from complete opened
reads; the only historical/current device mappings are the selected artifact
mount's81 to68 transition. Altered historical metadata in a negative test is
explicitly an invalid candidate, never an invented current observation.
"""
from __future__ import annotations
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys
import types
import unittest

MIB=1024**2
ROOT=Path('/var/tmp/uwvm-ros-complete-20261015-r1')
ARTIFACTS=ROOT/'acceptance-storage-r1'
OLD_REPORT=ARTIFACTS/'source40-unitree-command14-build-r1/report.json'
OLD_SHA='b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747'
CG={'memory_max':64*1024**3,'memory_swap_max':0,'expected_device':29,'expected_inode':18315,
    'expected_boot_id':'ec0d5f0f-3060-4220-bee2-2a7791ed4e72',
    'expected_leaf':'/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
    'expected_cpus':'0-31'}
HELPER=ADAPTER=GUARD=None
CASES=[]
PRESERVE=[]


def load(path,wanted,name):
    raw=path.read_bytes()
    if not path.is_absolute() or path.resolve(strict=True)!=path or len(raw)>MIB or hashlib.sha256(raw).hexdigest()!=wanted:
        raise ValueError('exact reviewed physical test helper required')
    module=types.ModuleType(name);module.__file__=str(path);sys.modules[name]=module
    exec(compile(raw,str(path),'exec'),module.__dict__)
    return module


class ActualSelectionMembership(unittest.TestCase):
    def setUp(self):
        GUARD()
    def tearDown(self):
        GUARD()
    def closure(self,case):
        value=ADAPTER.RetainedClosure.__new__(ADAPTER.RetainedClosure)
        value.pins=HELPER.PersistentPinSet([case['historical']],
            [{'root':str(ARTIFACTS),'historical_device':81,'current_device':68}],GUARD)
        return value
    def test_real_three_family_selection_lists_find_the_matching_profile(self):
        for case in CASES:
            with self.subTest(family=case['family']):
                value=self.closure(case)
                self.assertTrue(value.member(case['current'],case['outputs']))
                self.assertEqual(len(value.pins.evidence),1)
                self.assertEqual(value.pins.evidence[0]['current_pin'],case['current'])
                value.pins.preserve(require_complete=True)
    def test_unrelated_actual_names_are_false_without_admission(self):
        for case in CASES:
            with self.subTest(family=case['family']):
                value=self.closure(case)
                for candidate in case['outputs']:
                    if candidate['path']!=case['current']['path']:
                        self.assertFalse(value.same(case['current'],candidate))
                        self.assertFalse(value.same(candidate,case['current']))
                self.assertEqual(value.pins.evidence,[])
    def test_same_selected_name_is_symmetric_and_preserved(self):
        for case in CASES:
            with self.subTest(family=case['family']):
                value=self.closure(case)
                self.assertTrue(value.same(case['historical'],case['current']))
                self.assertTrue(value.same(case['current'],case['historical']))
                value.pins.preserve(require_complete=True)
    def test_same_name_unknown_historical_hash_stays_rejected(self):
        for case in CASES:
            with self.subTest(family=case['family']):
                value=self.closure(case)
                invalid={**case['historical'],'sha256':'0'*64}
                with self.assertRaisesRegex(ValueError,'preselected finite'):
                    value.same(case['current'],invalid)
                self.assertEqual(value.pins.evidence,[])
    def test_equal_profile_bytes_do_not_alias_distinct_actual_paths(self):
        for case in CASES:
            with self.subTest(family=case['family']):
                self.assertNotEqual(case['current_source']['path'],case['historical']['path'])
                self.assertEqual(case['current_source']['sha256'],case['historical']['sha256'])
                value=self.closure(case)
                self.assertFalse(value.same(case['current_source'],case['historical']))
                self.assertFalse(value.same(case['historical'],case['current_source']))
                self.assertEqual(value.pins.evidence,[])
    def test_membership_keeps_all_original_output_rows_unchanged(self):
        for case in CASES:
            with self.subTest(family=case['family']):
                original=copy.deepcopy(case['outputs'])
                value=self.closure(case)
                self.assertTrue(value.member(case['current'],case['outputs']))
                self.assertEqual(case['outputs'],original)
                self.assertTrue(all(row['device']==81 for row in case['outputs']))
                value.pins.preserve(require_complete=True)


def main():
    global HELPER,ADAPTER,GUARD,CASES
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    parser.add_argument('--helper',type=Path,required=True)
    parser.add_argument('--helper-sha256',required=True)
    parser.add_argument('--adapter',type=Path,required=True)
    parser.add_argument('--adapter-sha256',required=True)
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args()
    if sys.platform!='linux':raise ValueError('actual guarded Linux regression required')
    HELPER=load(args.helper,args.helper_sha256,'typed_membership_restart_helper')
    source=ROOT/'inputs-r39/source';mf=source.parent/'source-manifest.json';raw=mf.read_bytes()
    if hashlib.sha256(raw).hexdigest()!='54d6c79978e36bd13fa37272d8dd5f2cc94a9d2e62ac437b51dfc4aa4f6ae5b9':
        raise ValueError('actual complete original probe manifest required')
    GUARD=HELPER.CurrentGuard(str(source),json.loads(raw),CG);GUARD.maximum_seconds=180
    before=GUARD.force()
    ADAPTER=load(args.adapter,args.adapter_sha256,'typed_membership_adapter')
    for path in (args.helper,args.adapter):PRESERVE.append(HELPER.opened_file(str(path),MIB,GUARD))
    pin,raw=HELPER.opened_file(str(OLD_REPORT),8*MIB,GUARD,collect=True)
    if pin['bytes']!=3911428 or pin['sha256']!=OLD_SHA:raise ValueError('original full b7 evidence changed')
    PRESERVE.append(pin);old=json.loads(raw)
    if old['status']!='FAIL':raise ValueError('the genuine failed batch must remain failed')
    pins={row['path']:row for row in old['input_pins']}
    for family in ('unitree_go','unitree_hg','unitree_api'):
        path=ARTIFACTS/'unitree-native-humble-build-export-20261016-r6/work'/family/'selection/profile.json'
        historical=pins[str(path)];selection=path.with_name('preparation.json');expected=pins[str(selection)]
        receipt=HELPER.requalify_persistent_pin(expected,str(selection),GUARD,historical_device=81,current_device=68)
        selected=receipt['current_pin'];PRESERVE.append(selected)
        opened,raw=HELPER.opened_file(str(selection),MIB,GUARD,collect=True)
        if opened!=selected:raise ValueError('actual native selection changed')
        outputs=json.loads(raw)['outputs']
        if len(outputs)<2 or not all(type(row) is dict and set(row)==ADAPTER.PIN for row in outputs):
            raise ValueError('complete actual native full-seven output list required')
        if [row for row in outputs if row['path']==str(path)]!=[historical]:
            raise ValueError('b7 profile observation differs from actual original native selection')
        current=HELPER.requalify_persistent_pin(historical,str(path),GUARD,historical_device=81,current_device=68)['current_pin']
        current_source=HELPER.opened_file(str(ROOT/'inputs-r40/source/sdk/robots/unitree/profiles'/(family+'.json')),MIB,GUARD)
        PRESERVE.extend([current,current_source]);CASES.append({'family':family,'historical':historical,'current':current,
            'outputs':outputs,'current_source':current_source})
    if not args.out.is_absolute() or args.out.parent.resolve(strict=True)!=args.out.parent or args.out.exists():
        raise ValueError('fresh absolute regression output required')
    args.out.mkdir(mode=0o700)
    result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(ActualSelectionMembership))
    if not all(HELPER.opened_file(pin['path'],pin['bytes'],GUARD)==pin for pin in PRESERVE):
        raise ValueError('regression input changed')
    report={'format_version':1,'kind':'actual-retained-native-selection-membership-regressions',
        'status':'PASS' if result.wasSuccessful() and result.testsRun==6 and not result.skipped else 'FAIL',
        'tests_run':result.testsRun,'actual_families':[case['family'] for case in CASES],
        'input_pins':PRESERVE,'source_and_inputs_preserved':True,'cgroup_before':before,'cgroup_after':GUARD.force(),
        'scope':'ACTUAL_READ_ONLY_METADATA_MEMBERSHIP_ONLY_NO_COMPILER_RUNTIME_DDS_OR_PHYSICS_QUALIFICATION',
        'historical_failed_report_promoted':False,'commands':[]}
    HELPER._write_report(args.out,report)
    print(json.dumps({'status':report['status'],'tests_run':report['tests_run'],'report':str(args.out/'report.json')}),flush=True)
    return 0 if report['status']=='PASS' else 1


if __name__=='__main__':raise SystemExit(main())
