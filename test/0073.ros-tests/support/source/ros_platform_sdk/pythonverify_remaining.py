#!/usr/bin/env python3
"""Run all prepared vendor Python values against native/Wasm-checked vectors.

This is native Python representation acceptance. It deliberately reports no
CPython/Wasm, native ROS, driver, model, hardware or official-suite credit.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import sys


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args()
    root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes())
    source=Path(inputs['source_root'])
    prepared=root/'out'/inputs.get('vendor_python_preparation','pythonprepare-r1')
    preparation=json.loads((prepared/'pythonprepare.json').read_bytes())
    assert preparation['status']=='TWENTY_SIX_FULL_PYTHON_VENDOR_PROFILE_SOURCES_PREPARED'
    assert not out.exists() and out.parent==root/'out'
    out.mkdir(mode=0o700)
    report=dict(status='RUNNING',commands=[],profiles=[],default_authority=[],
                execution_mode='NATIVE_PYTHON_REPRESENTATION_ONLY',actual_wasm_native_ros_or_official_cases_run=0)
    def save():(out/'pythonverify.json').write_text(json.dumps(report,indent=2)+'\n')
    try:
        spec=importlib.util.spec_from_file_location('independent_vendor_field_vectors',source/'test/0073.ros-tests/support/source/ros_platform_sdk/managed_vectors_remaining.py')
        vectors=importlib.util.module_from_spec(spec)
        spec.loader.exec_module(vectors)
        for row in preparation['profiles']:
            vendor,profile=row['vendor'],row['profile']
            name=vendor+'-'+profile
            directory=out/name
            directory.mkdir()
            direct='deep'if vendor=='deep_robotics'else vendor.replace('_','-')
            revision='r2'if vendor=='agibot'else'r1'
            direct_report=root/'out'/('qualify-'+direct+'-'+revision)
            accepted=json.loads((direct_report/'qualification.json').read_bytes())
            assert accepted['status']=='SELECTED_VENDOR_C_CPP_COMPLETE_DATA_AND_DEFAULT_REFUSAL_PASS'
            gold=direct_report/('golden-vectors-'+profile)/'vectors.json'
            cases=[]
            def collect(name,wire,value):
                cases.append(dict(type=name,wire=wire.hex(),value=value))
            independent=vectors.build(source,vendor,profile,gold,directory/'original-field-sources',on_case=collect)
            vector_path=directory/'python-vectors.json'
            vector_path.write_text(json.dumps(dict(cases=cases),indent=2)+'\n')
            argv=[sys.executable,'-I','-S','-B',source/'test/0073.ros-tests/support/source/ros_platform_sdk/check_python_vendor.py',
                  '--sdk-python',source/'sdk/origin/python','--generated',row['output'],
                  '--vectors',vector_path,'--out',directory/'verification.json']
            with (directory/'stdout').open('xb')as stdout,(directory/'stderr').open('xb')as stderr:
                process=subprocess.run(list(map(str,argv)),stdout=stdout,stderr=stderr,timeout=120,check=False)
            report['commands'].append(dict(argv=list(map(str,argv)),exit=process.returncode))
            save()
            assert process.returncode==0,(name,(directory/'stderr').read_text()[-2600:])
            result=json.loads((directory/'verification.json').read_bytes())
            assert result['records']==row['records']==independent['records']
            assert result['populated_scalar_or_string_instances']==independent['scalar_or_string_instances']
            report['profiles'].append(dict(row,result=result))
            save()
        report.update(status='TWENTY_SIX_VENDOR_PYTHON_FULL_VALUE_AND_REFUSAL_PROFILES_PASS',
            records=sum(v['records']for v in report['profiles']),
            populated_scalar_or_string_instances=sum(v['result']['populated_scalar_or_string_instances']for v in report['profiles']),
            completed_codec_methods=sum(v['result']['completed_codec_methods']for v in report['profiles']))
        assert len(report['profiles'])==26 and report['records']==1729
        print(report['status'],report['records'],report['populated_scalar_or_string_instances'])
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error))
        raise
    finally:save()


if __name__=='__main__':main()
