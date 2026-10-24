#!/usr/bin/env python3
"""Qualify all26 Python vendor profiles in two actual multi-profile Wasms.

The public SDK builder supplies the real frozen Python/interpreter/ROS bridge.
Every selected family executes original-field values, an intentionally corrupt
reference, all three default-denied modes and invalid-argument cases. No native
ROS driver, physical robot or upstream official example is claimed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys


def sha(path):
    with path.open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args();root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    published=json.loads((root/'out'/inputs.get('vendor_python_publication','pythonpublish-r1')/'pythonpublish.json').read_bytes())
    assert published['status']=='PYTHON_VENDOR_SDK_SOURCE_VIEWS_AND_NATIVE_VALUE_ACCEPTANCE_PASS'
    verification=json.loads((root/'out/pythonverify-r3/pythonverify.json').read_bytes())
    assert verification['status']=='TWENTY_SIX_VENDOR_PYTHON_FULL_VALUE_AND_REFUSAL_PROFILES_PASS'
    assert out.parent==root/'out'and not out.exists()
    out.mkdir(mode=0o700);(out/'commands').mkdir()
    producer=root/'out/pythonbuild-r2/pythonbuild.json'
    builder=source/'sdk/origin/robots/vendors/build_python_guest.py'
    report=dict(status='RUNNING',commands=[],products=[],profiles=[],default_authority=[],
        execution_scope='Actual Python/interpreter/bridge in two UWVM Wasms; no native driver/official-suite qualification')
    def save():(out/'pythonmulti.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0,timeout=900):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),expected_exit=expected,exit=None)
        report['commands'].append(row);save()
        with(directory/'stdout').open('xb')as stdout,(directory/'stderr').open('xb')as stderr:
            result=subprocess.run(row['argv'],stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr,timeout=timeout,check=False)
        row.update(exit=result.returncode,stdout_sha256=sha(directory/'stdout'),stderr_sha256=sha(directory/'stderr'));save()
        assert result.returncode==expected,(label,(directory/'stderr').read_text()[-2600:])
        return(directory/'stdout').read_bytes(),(directory/'stderr').read_bytes()
    try:
        base=[sys.executable,'-I','-S','-B',builder,'--producer',producer,
              '--producer-sha256',sha(producer),'--validator',inputs['validator']]
        for version in(1,2):
            rows=[v for v in verification['profiles']if v['ros_version']==version]
            argv=list(base)+['--out',out/('ros'+str(version))]
            for row in rows:
                prefix=source/'sdk/origin/platform'/row['vendor']/'python'
                argv+=['--example',prefix/'example'/('ros'+str(version))/row['family'],
                       '--vectors',prefix/'test'/('ros'+str(version))/row['family']/'vectors.json']
            run('ros'+str(version)+'-public-build',argv)
            product=json.loads((out/('ros'+str(version))/'build.json').read_bytes())
            assert product['status']=='REAL_MULTI_PROFILE_CPYTHON_VENDOR_WASM_BUILT_NOT_EXECUTED'
            assert len(product['profiles'])==len(rows)and product['backend']==version
            artifact=Path(product['artifact']['path']);assert sha(artifact)==product['artifact']['sha256']
            prefix=[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact]
            all_values,_=run('ros'+str(version)+'-all-values',prefix+['all-values'])
            assert all_values==('PASS_ALL_VENDOR_PYTHON_VALUES %d %d\n'%
                (sum(v['records']for v in rows),sum(v['result']['populated_scalar_or_string_instances']for v in rows))).encode()
            for row in rows:
                family=row['family'];expected=row['result']
                output,_=run(family+'-values',prefix+[family,'values'])
                assert output==('PASS_COMPLETE_VENDOR_PYTHON_VALUES %d %d\n'%
                    (row['records'],expected['populated_scalar_or_string_instances'])).encode()
                output,error=run(family+'-corrupt-control',prefix+[family,'values-corrupt'],expected=1)
                assert not output and b'/roswasm/vendor_check.py' in error
                assert any(name in error for name in(b'AssertionError',b'ValueError',b'UnicodeDecodeError'))
                for mode in('raw','state','command'):
                    output,error=run(family+'-denied-'+mode,prefix+[family,mode,row['vendor']+'.observation'],expected=13)
                    assert not output and not error
                for label,args in[('missing',[]),('mode',['invalid','alias']),('alias',['state',''])]:
                    output,error=run(family+'-arguments-'+label,prefix+[family,*args],expected=22)
                    assert not output and not error
                report['profiles'].append(dict(vendor=row['vendor'],profile=row['profile'],family=family,
                    ros_version=version,records=row['records'],complete_values=expected,
                    real_guest_value_pass=True,corrupt_control_rejected=True,default_denied_modes=3))
                save()
            output,error=run('ros'+str(version)+'-unknown-family',prefix+['../outside','state','alias'],expected=22)
            assert not output and not error
            report['products'].append(product['artifact']);save()
        example1=next(v for v in verification['profiles']if v['ros_version']==1)
        example2=next(v for v in verification['profiles']if v['ros_version']==2)
        def entry(row):return source/'sdk/origin/platform'/row['vendor']/'python/example'/('ros'+str(row['ros_version']))/row['family']
        mixed=out/'refusal-mixed-backend'
        _,error=run('mixed-backend-builder-refusal',base+['--example',entry(example1),'--example',entry(example2),'--out',mixed],expected=1)
        assert b'all profiles must match the one compiled ROS backend' in error
        assert json.loads((mixed/'build.json').read_bytes())['commands']==[]
        wrong=out/'refusal-wrong-producer'
        wrong_args=[sys.executable,'-I','-S','-B',builder,'--producer',producer,'--producer-sha256','0'*64,
                    '--validator',inputs['validator'],'--example',entry(example1),'--out',wrong]
        _,error=run('wrong-producer-builder-refusal',wrong_args,expected=2)
        assert b'explicit producer receipt SHA256 differs' in error and not wrong.exists()
        report.update(status='ALL_TWENTY_SIX_PYTHON_PROFILES_IN_TWO_REAL_WASMS_PASS',
            records=sum(v['records']for v in report['profiles']),
            populated_scalar_or_string_instances=sum(v['complete_values']['populated_scalar_or_string_instances']for v in report['profiles']),
            corrupt_controls=sum(v['corrupt_control_rejected']for v in report['profiles']),
            default_denied_cases=sum(v['default_denied_modes']for v in report['profiles']))
        assert len(report['profiles'])==26 and report['records']==1729
        print(report['status'],report['records'],report['default_denied_cases'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
