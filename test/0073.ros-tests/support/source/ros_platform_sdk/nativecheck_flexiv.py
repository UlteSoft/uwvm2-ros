#!/usr/bin/env python3
"""Independently check actual native package builds, unchanged products and cases.

Read closed raw evidence under the Linux owner. Neither a build nor three
interface-package linters qualify a native robot driver, controller or positive
Wasm delivery. Earlier offline failures and their complete saved results remain
failures; final distinct original cases are counted separately from reruns.
"""
import argparse
import hashlib
import json
from pathlib import Path
import xml.etree.ElementTree as ET


def sha(path):
    with path.open('rb')as stream:
        return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args()
    root,out=a.root.resolve(strict=True),a.out
    assert out.is_absolute()and out.parent==root/'out'and not out.exists()
    def read(path):return json.loads(path.read_bytes())
    def pin(row):
        path=Path(row['path'])
        assert path.stat().st_size==row['bytes']and sha(path)==row['sha256'],path
    def commands(report):
        for row in report['commands']:
            pin(row['stdout']);pin(row['stderr'])
    def owner(phase,attempt):
        report=read(root/'out'/('flexiv-'+phase+'-'+attempt+'.json'))
        assert report['status']=='PASS'and report['all_admitted_source_and_tool_pins_preserved']
        for name in ('events_before','events_after'):
            e=dict(line.split()for line in report[name].splitlines())
            assert all(e[k]=='0'for k in ('oom','oom_kill','oom_group_kill'))
        assert all(r['exit']==0 and r['forced_signal']is None for r in report['commands'])
        return max(r['max_sampled_owned_rss']for r in report['commands'])
    results=[]
    inputs=read(root/'inputs.json')
    attempts=inputs['native_attempts']
    expected={'ros1_noetic','ros2_humble_v1','ros2_humble_v2','ros2_humble_v2_2'}
    assert set(attempts)==expected
    initial_failed_executions=0
    for profile,selection in attempts.items():
        attempt,retest=selection['build'],selection.get('retest')
        base=root/'out'/('native-'+attempt)
        report=read(base/'native.json')
        assert report['profile']==profile
        assert report['status']in('ORIGINAL_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS','ORIGINAL_NATIVE_INTERFACE_BUILD_PASS_UPSTREAM_TEST_FAILURES_RETAINED')
        peak=owner('native',attempt)
        commands(report)
        assert [r['exit']for r in report['commands'][:4]]==[0,0,0,0]
        for row in report['original_files']+report['sdk_tools']+report.get('sdk_identity_files',[])+report['installed_products']:
            pin(row)
        assert len(report['installed_products'])>0
        final=report
        data=base
        if retest:
            data=root/'out'/('retest-'+retest)
            final=read(data/'retest.json')
            assert final['status']=='ORIGINAL_PACKAGE_THREE_CASES_PASS_WITH_PINNED_OFFLINE_SCHEMA'
            assert final['source_sdk_and_installed_products_unchanged']
            peak=max(peak,owner('retest',retest))
            commands(final)
            assert all(r['exit']==0 for r in final['commands'])
            assert final['preserved_prior_results']
            for row in final['preserved_prior_results']:pin(row['saved'])
            assert report['original_cases_run']==3 and report['original_case_failures']==1
            initial_failed_executions+=report['original_cases_run']
        else:
            assert report['status']=='ORIGINAL_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS'
            assert all(r['exit']==0 for r in report['commands'])
        if profile=='ros1_noetic':
            assert final['original_cases_run']==report['original_test_cases_discovered']==0
        else:
            assert final['original_cases_run']==3 and final['original_case_failures']==final['original_case_skips']==0
            cases=ET.fromstring((data/'upstream-junit.xml').read_bytes()).findall('.//testcase')
            assert {c.get('name')for c in cases}=={'copyright','lint_cmake','xmllint'}and len(cases)==3
            assert all(c.find('failure')is None and c.find('error')is None and c.find('skipped')is None for c in cases)
        results.append(dict(profile=profile,installed_products=len(report['installed_products']),
            final_original_cases=final['original_cases_run'],sampled_owned_peak_rss=peak,
            original_source_sdk_and_installed_products_unchanged=True))
    result=dict(status='INDEPENDENT_ORIGINAL_FLEXIV_NATIVE_PACKAGES_AND_NINE_FINAL_CASES_PASS',
        profiles=results,final_distinct_original_cases=9,additional_initial_failed_case_executions=initial_failed_executions,
        original_full_driver_suites_run=0,native_robot_delivery=False,default_authority=[],
        scope='Four selected original native interface package builds, nine distinct final package lint cases; no RDK/controller, positive Wasm/native delivery, all releases or full ROS/driver suite claim')
    out.mkdir(mode=0o700)
    (out/'native-verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],flush=True)


if __name__=='__main__':main()
