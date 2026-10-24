#!/usr/bin/env python3
"""Verify original RealMan package results and a separate format-only rebuild.

Original upstream failures remain failures. A downstream formatting fix is
checked byte-for-byte, has no message/test changes, and receives its own result.
This checker executes no compiler, driver, Guest or controller.
"""
import argparse
import hashlib
import json
from pathlib import Path
import xml.etree.ElementTree as ET


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    args=p.parse_args();root=args.root.resolve(strict=True);out=args.out
    assert out.parent==root/'out'and not out.exists()
    def read(path):return json.loads(path.read_bytes())
    def sha(path):
        with path.open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
    def pin(row):
        path=Path(row['path']);assert path.stat().st_size==row['bytes']and sha(path)==row['sha256']
    def check(attempt,upstream_failure=False):
        base=root/'out'/('native-'+attempt);report=read(base/'native.json')
        owner=read(root/'out'/('realman-native-'+attempt+'.json'))
        assert owner['status']=='PASS'and owner['all_admitted_source_and_tool_pins_preserved']
        assert all(c['exit']==0 and c['forced_signal']is None for c in owner['commands'])
        for name in ('events_before','events_after'):
            events=dict(line.split()for line in owner[name].splitlines())
            assert all(events[n]=='0'for n in ('oom','oom_kill','oom_group_kill'))
        for row in report['original_files']+report.get('captured_original_files',[])+report['sdk_tools']+report['sdk_identity_files']+report['installed_products']:pin(row)
        assert report['installed_products']
        for row in report['commands']:
            allowed=8 if upstream_failure and row['label']=='run-original-tests'else 0
            assert row['exit']==allowed
            assert row['expected_exit']==0 or (row['label']=='run-original-tests'and row['expected_exit']is None)
            pin(row['stdout']);pin(row['stderr'])
        listed=read(base/'commands/list-original-tests/stdout')['tests']
        assert report['original_test_cases_discovered']==report['original_cases_run']==len(listed)
        assert report['original_case_skips']==0
        cases=ET.fromstring((base/'upstream-junit.xml').read_bytes()).findall('.//testcase')if listed else []
        assert len(cases)==len(listed)and {r['name']for r in listed}=={r.get('name')for r in cases}
        failures=[r.get('name')for r in cases if r.find('failure')is not None or r.find('error')is not None]
        assert not any(r.find('skipped')is not None for r in cases)
        assert report['original_case_failures']==len(failures)
        if upstream_failure:
            assert report['status']=='ORIGINAL_NATIVE_INTERFACE_BUILD_PASS_UPSTREAM_TEST_FAILURES_RETAINED'
            assert {r['name']for r in listed}=={'lint_cmake','xmllint'}and failures==['lint_cmake']
            assert b'CMakeLists.txt:84: Line ends in whitespace'in(base/'commands/run-original-tests/stdout').read_bytes()
        else:assert not failures
        return report,max(c['max_sampled_owned_rss']for c in owner['commands'])
    inputs=read(root/'inputs.json');results=[]
    assert set(inputs['native_attempts'])=={'ros1_develop','ros1_main','ros2_humble'}
    for profile,selection in inputs['native_attempts'].items():
        original,peak=check(selection['build'],profile=='ros2_humble')
        assert original['profile']==profile
        prefix=Path(inputs['source_root'])/'sdk/origin/robots/arms/realman/upstream'/profile/'files'
        assert all(Path(r['path']).is_relative_to(prefix)for r in original['original_files'])
        row=dict(profile=profile,distribution=original['distribution'],installed_regular_products=len(original['installed_products']),
            original_cases=original['original_cases_run'],original_failures=original['original_case_failures'],sampled_owned_peak_rss=peak)
        if profile!='ros2_humble':
            assert original['status']=='ORIGINAL_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS'
            assert original['original_cases_run']==0
        else:
            fixed,peak=check(selection['format_rebuild'])
            assert fixed['status']=='DOWNSTREAM_FORMAT_ONLY_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS'
            assert fixed['source_variant']=='EXPLICIT_DOWNSTREAM_FORMAT_ONLY_OVERLAY'
            assert fixed['sdk_tools']==original['sdk_tools']and fixed['sdk_identity_files']==original['sdk_identity_files']
            assert fixed['native_sdk_root']==original['native_sdk_root']and fixed['original_cases_run']==2
            proof=read(root/'out/nativeformat-r1/overlay.json')
            before=Path(proof['original']);after=Path(proof['overlay'])
            assert proof['changed_line']==84 and proof['removed_ascii_space_bytes']==2
            for entry in proof['files']:
                old=(before/entry['path']).read_bytes();new=(after/entry['path']).read_bytes()
                assert sha(before/entry['path'])==entry['before_sha256']and sha(after/entry['path'])==entry['after_sha256']
                if entry['path']=='CMakeLists.txt':
                    assert old.splitlines(keepends=True)[83]==b'  msg/Mastername.msg  \n'
                    assert new==old.replace(b'  msg/Mastername.msg  \n',b'  msg/Mastername.msg\n',1)
                else:assert old==new and not entry['changed']
            prior={str(Path(r['path']).relative_to(root/'out'/('native-'+selection['build'])/'install')):r for r in original['installed_products']}
            headers=0
            for entry in fixed['installed_products']:
                relative=Path(entry['path']).relative_to(root/'out'/('native-'+selection['format_rebuild'])/'install')
                if relative.parts[0]=='include'or relative.suffix in ('.msg','.idl'):
                    assert str(relative)in prior and entry['sha256']==prior[str(relative)]['sha256'],relative
                    headers+=1
            assert headers>=77
            row.update(downstream_cases=2,downstream_failures=0,downstream_installed_regular_products=len(fixed['installed_products']),
                native_headers_and_declarations_byte_identical=headers,sampled_owned_peak_rss=max(row['sampled_owned_peak_rss'],peak))
        results.append(row)
    result=dict(status='INDEPENDENT_REALMAN_ORIGINAL_FAILURE_AND_FORMAT_ONLY_REBUILD_VERIFIED',profiles=results,
        original_cases=2,original_failures_retained=1,downstream_cases=2,downstream_failures=0,
        original_full_driver_suites_run=0,native_robot_delivery=False,default_authority=[],
        scope='Selected original native interface builds and retained original CMake lint failure; separate two-space formatting fix and full registered suite rebuild. No whole-driver, positive robot, other-installation or performance acceptance.')
    out.mkdir(mode=0o700);(out/'native-verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],flush=True)


if __name__=='__main__':main()
