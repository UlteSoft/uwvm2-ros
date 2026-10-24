#!/usr/bin/env python3
"""Rerun actual original package cases with a pinned offline ROS XML catalog.

The independently bounded Linux owner must admit this phase. Earlier failed
JUnit, CTest and per-case results are preserved before any reusable build-tree
test output is refreshed. The original source, test registration, binaries and
read-only ROS SDK are unchanged. This never substitutes authored SDK contracts
for official package cases, starts a robot or grants a Guest capability.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET


def pin(path):
    with path.open('rb') as stream:
        digest = hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=digest)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a = p.parse_args()
    root,out = a.root.resolve(strict=True),a.out
    inputs = json.loads((root/'inputs.json').read_bytes())
    prior = root/'out'/inputs['original_native_run']
    old = json.loads((prior/'native.json').read_bytes())
    assert old['status'] in ('ORIGINAL_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS','ORIGINAL_NATIVE_INTERFACE_BUILD_PASS_UPSTREAM_TEST_FAILURES_RETAINED')
    assert old['distribution']=='humble' and old['original_test_cases_discovered']==3
    assert out.is_absolute() and out.parent==root/'out' and not out.exists()
    assert inputs['offline_ros_schema'] and os.environ['XML_CATALOG_FILES']=='/tmp/tools/ros-schema-xsd/catalog.xml'
    out.mkdir(mode=0o700)
    (out/'commands').mkdir()
    report = dict(status='RUNNING',profile=old['profile'],original_native_report=pin(prior/'native.json'),
        default_authority=[],commands=[],preserved_prior_results=[],
        scope='Actual original package tests with an admitted offline official schema; no native robot or Wasm positive delivery')
    for name in ('build/Testing','build/test_results','upstream-junit.xml'):
        path = prior/name
        if not path.exists():
            continue
        dest = out/'prior-original-data'/name
        dest.parent.mkdir(parents=True,exist_ok=True)
        if path.is_dir():
            assert not any(p.is_symlink()for p in path.rglob('*'))
            shutil.copytree(path,dest)
            files = [p for p in path.rglob('*')if p.is_file()]
        else:
            assert not path.is_symlink()
            shutil.copyfile(path,dest)
            files = [path]
        for file in files:
            copied = out/'prior-original-data'/file.relative_to(prior)
            assert copied.read_bytes()==file.read_bytes()
            report['preserved_prior_results'].append(dict(original=pin(file),saved=pin(copied)))
    unchanged = old['original_files']+old['sdk_tools']+old.get('sdk_identity_files',[])+old['installed_products']
    for row in unchanged:
        assert pin(Path(row['path']))==row
    schema = root/'tools/ros-schema-xsd'
    report['offline_schema'] = [pin(p)for p in sorted(schema.rglob('*'))if p.is_file()]
    assert report['offline_schema']
    def save():
        (out/'retest.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,args):
        directory = out/'commands'/label
        directory.mkdir()
        inside = '/tmp/'+str(prior.relative_to(root))
        sdk = old['native_sdk_root']
        argv = ['/usr/bin/unshare','--mount','--net','--propagation','private',sys.executable,
            '-I','-S','-B',prior/'chroot-exec.py',sdk,root,'bash','--noprofile','--norc','-c',
            'set -e; source /opt/ros/humble/setup.sh; exec "$@"','flexiv-original-retest',
            'cmake','-E','chdir',inside+'/build','ctest',*args]
        with (directory/'stdout').open('xb')as stdout,(directory/'stderr').open('xb')as stderr:
            result = subprocess.run(list(map(str,argv)),env=dict(os.environ,CATKIN_SHELL='sh',
                PYTHONDONTWRITEBYTECODE='1',HOME='/tmp/out/home',TMPDIR='/tmp/out/tmp'),
                stdout=stdout,stderr=stderr,timeout=240,check=False)
        report['commands'].append(dict(label=label,argv=list(map(str,argv)),exit=result.returncode,
            stdout=pin(directory/'stdout'),stderr=pin(directory/'stderr')))
        save()
        assert result.returncode==0,(label,(directory/'stdout').read_text()[-2500:],(directory/'stderr').read_text()[-1000:])
    try:
        run('list-original-tests',['--show-only=json-v1'])
        listed = json.loads((out/'commands/list-original-tests/stdout').read_bytes())
        assert len(listed['tests'])==old['original_test_cases_discovered']
        run('run-original-tests',['-j1','--timeout','60','--output-on-failure',
            '--output-junit','/tmp/'+str((out/'upstream-junit.xml').relative_to(root))])
        cases = ET.fromstring((out/'upstream-junit.xml').read_bytes()).findall('.//testcase')
        assert len(cases)==3 and all(c.find('failure')is None and c.find('error')is None and c.find('skipped')is None for c in cases)
        for row in unchanged:
            assert pin(Path(row['path']))==row
        for row in report['preserved_prior_results']:
            assert pin(Path(row['saved']['path']))==row['saved']
        report.update(status='ORIGINAL_PACKAGE_THREE_CASES_PASS_WITH_PINNED_OFFLINE_SCHEMA',
            original_cases_run=3,original_case_failures=0,original_case_skips=0,
            source_sdk_and_installed_products_unchanged=True)
        print(report['status'],report['profile'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error))
        raise
    finally:
        save()


if __name__=='__main__':
    main()
