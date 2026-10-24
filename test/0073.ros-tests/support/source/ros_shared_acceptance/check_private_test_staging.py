#!/usr/bin/env python3
"""Check private source staging and actual native-adapter configuration tests."""
import ast
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

ROOT=next(p for p in Path(__file__).resolve().parents if(p/'sdk/origin').is_dir())
TESTS=ROOT/'test/0073.ros-tests';SUPPORT=TESTS/'support'
sys.path.insert(0,str(SUPPORT))
from resource_contract import validate_resource_owner
from stage_sdk_tests import native_languages_tools,overlay


def require(value,message):
    if not value:raise AssertionError(message)


def sha(path):
    with Path(path).open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    proof=validate_resource_owner();work=Path.cwd();rows=[]
    require(work==Path(proof['run'])/'work','actual owner work required')
    def passed(name,**evidence):rows.append(dict(name=name,status='PASS',**evidence))
    def refused(name,call):
        try:call()
        except ValueError:passed(name)
        else:raise AssertionError('required refusal missing: '+name)
    for source in sorted((SUPPORT/'source/sdk-native-languages/drivers').glob('*.py')):
        ast.parse(source.read_bytes(),filename=str(source))
    passed('all-canonical-native-driver-syntax')
    refused('refuse-original-production-copy',lambda:overlay('go',ROOT/'sdk/origin/go',test_root=TESTS))
    origin=work/'selected/sdk/origin';shutil.copytree(ROOT/'sdk/origin',origin)
    production_before={str(p.relative_to(origin)):sha(p)for p in origin.rglob('*')if p.is_file()}
    staged=native_languages_tools(origin,test_root=TESTS)
    require(staged['mode']=='separate-canonical-tests-in-owned-private-copy'and staged['files'],
        'canonical fixture staging missing')
    helper=origin/'tools/native_languages'
    require((helper/'fixtures/catalog.json').is_file()and(helper/'fixtures/go-values/main.go').is_file(),
        'current validation/generation fixtures missing')
    passed('current-native-fixtures-and-drivers',copied_files=len(staged['files']))
    go=next(r for r in staged['overlays']if r['language']=='go')
    zig=next(r for r in staged['overlays']if r['language']=='zig')
    require(any(Path(r['destination']).name=='client_test.go'for r in go['files']),
        'actual Go client tests were not restored')
    plan=json.loads((SUPPORT/'sdk-inline-test-extraction.json').read_bytes())
    block=next(r for r in plan['blocks']if r['source']=='sdk/origin/zig/client.zig')
    fixture=TESTS/block['destination'][len('test/0073.ros-tests/'):]
    require((origin/'zig/client.zig').read_bytes()==(ROOT/'sdk/origin/zig/client.zig').read_bytes()+fixture.read_bytes(),
        'private Zig test scope differs from exact extracted block')
    require(len(zig['files'])>=1,'Zig staging has no tests')
    passed('go-and-exact-private-zig-checks',go_files=len(go['files']),zig_files=len(zig['files']))
    for language in('rust','tinygo','moonbit'):
        result=overlay(language,origin/language,test_root=TESTS)
        require(result['files'],'staging would silently run zero '+language+' tests')
        if language=='rust':
            expected={r['sha256']for r in plan['blocks']if r['source'].startswith('sdk/origin/rust/')}
            actual={r['sha256']for r in result['files']if r.get('operation')=='append original private test scope'}
            require(actual==expected and len(expected)==5,'all five Rust inline blocks required')
        if language=='moonbit':
            require(sum(r.get('operation')=='append external test linker reservation'for r in result['files'])==5,
                'all five real MoonBit package reservations required')
        passed(language+'-private-module-checks',files=len(result['files']))
    refused('refuse-duplicate-go-overlay',lambda:overlay('go',origin/'go',test_root=TESTS))
    embedded=native_languages_tools(origin,test_root=TESTS)
    require(embedded['mode']=='embedded-source-tests-retained-without-current-overlay'
        and not embedded['overlays'],'embedded legacy checks must not be mixed with a new overlay')
    passed('retain-embedded-archive-checks-without-replacement')
    for name in('validate','validate_subset','validate_go_values'):
        path=SUPPORT/'source/sdk-native-languages/drivers'/(name+'.py')
        spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        require(module.stage_inputs(origin,TESTS)['mode']==embedded['mode'],'driver did not use selected external artifact')
    passed('three-current-driver-artifact-loaders')
    partial=work/'partial/sdk/origin/tools/native_languages';partial.mkdir(parents=True)
    (partial/'test_profile.py').write_text('# partial legacy input\n')
    refused('refuse-partial-embedded-test-mixture',lambda:native_languages_tools(partial.parents[1],test_root=TESTS))
    configuration=[]
    for name in('test_profile.py','test_arguments.py'):
        command=[sys.executable,'-I','-S','-B',str(helper/name)]
        done=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=30)
        log=work/(name+'.log');log.write_bytes(done.stdout)
        require(done.returncode==0,'actual selected configuration checks failed: '+str(log))
        configuration.append(dict(name=name,exit=done.returncode,log_sha256=sha(log)))
    passed('actual-private-production-profile-and-argument-suites',suites=len(configuration))
    require(all(sha(origin/p)==pin for p,pin in production_before.items()
        if not(p.startswith(('rust/','zig/','moonbit/')))),
        'staging changed an unrelated production source')
    source_pins=[dict(path=str(p.relative_to(ROOT)),sha256=sha(p))for p in sorted(ROOT.rglob('*'))
        if p.is_file()and not p.is_symlink()]
    result=dict(kind='canonical-private-test-stage-acceptance-v1',status='PASS',resource=proof,
        checks=rows,check_denominator=12,configuration_suites=configuration,source_pins=source_pins,
        test_staging=staged,numbered_workloads=1,language_compiler_runs=0,guest_cases=0,native_ros_cases=0,
        qemu_cases=0,official_upstream_credit=0,full_sdk_install_qualified=False,default_authority=[])
    require(len(rows)==12,'source-stage check denominator changed')
    (work/'private-test-staging-result.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    print(json.dumps(dict(status='PASS',source_stage_checks=len(rows),configuration_suites=2,
        language_compiler_runs=0,guest_cases=0)),flush=True)


if __name__=='__main__':main()
