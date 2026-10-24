#!/usr/bin/env python3
"""Build and execute selected complete Python vendor applications inside Wasm.

Use the freshly rebuilt exact-version interpreter, actual frozen language SDK,
complete generated values and numeric bridge. No native Python application,
Host relay or fake extension supplies the Guest outcomes. These finite cases
do not qualify a native robot, driver, transport or official upstream example.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys


def pin(path):
    with path.open('rb')as stream:h=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=h)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args();root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    producer=json.loads((root/'out'/inputs.get('vendor_python_build','pythonbuild-r2')/'pythonbuild.json').read_bytes())
    assert producer['status']=='FRESH_CPYTHON_3148_NATIVE_AND_STATIC_WASI_PRODUCER_PASS'
    prepared=root/'out/pythonprepare-r1'
    preparation=json.loads((prepared/'pythonprepare.json').read_bytes())
    verified=root/'out/pythonverify-r3'
    verification=json.loads((verified/'pythonverify.json').read_bytes())
    assert verification['status']=='TWENTY_SIX_VENDOR_PYTHON_FULL_VALUE_AND_REFUSAL_PROFILES_PASS'
    target,native,sdk,cpython=map(Path,(producer['target'],producer['native_python'],producer['sdk'],producer['source']))
    assert out.parent==root/'out'and not out.exists()
    out.mkdir(mode=0o700);(out/'commands').mkdir()
    report=dict(status='RUNNING',commands=[],profiles=[],default_authority=[],
        execution_mode='ACTUAL_CPYTHON_AND_PYTHON_LOGIC_INSIDE_UWVM_WASM',
        native_driver_or_official_cases_run=0,make_jobs=1)
    def save():(out/'pythonwasm.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,*,expected=0,cwd=None,timeout=240):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),exit=None,expected_exit=expected,cwd=str(cwd or source))
        report['commands'].append(row);save()
        with(directory/'stdout').open('xb')as stdout,(directory/'stderr').open('xb')as stderr:
            process=subprocess.run(row['argv'],cwd=cwd or source,env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1'),
                stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr,timeout=timeout,check=False)
        row.update(exit=process.returncode,stdout=pin(directory/'stdout'),stderr=pin(directory/'stderr'));save()
        assert process.returncode==expected,(label,(directory/'stderr').read_text()[-2600:])
        return(directory/'stdout').read_bytes()
    try:
        for row in producer['files']:
            assert pin(Path(row['path']))==row
        selected=[('ubtech','ros2_main'),('fourier','ros1_aurora_bridge_v1')]
        for vendor,profile in selected:
            selected_profile=next(v for v in preparation['profiles']if(v['vendor'],v['profile'])==(vendor,profile))
            version=selected_profile['ros_version'];name=vendor+'-'+profile
            directory=out/name;directory.mkdir()
            stage=directory/'sdk-python';(stage/'roswasm').mkdir(parents=True)
            base=source/'sdk/origin/python'
            for start in [base/'roswasm',*[base/'compat/rclpy'/v for v in('rclpy','std_msgs','std_srvs','example_interfaces','examples_rclpy_minimal_service')]]:
                for path in sorted(start.rglob('*.py')):
                    destination=stage/path.relative_to(base)
                    destination.parent.mkdir(parents=True,exist_ok=True)
                    shutil.copyfile(path,destination)
            generated=Path(selected_profile['output'])
            proof=json.loads((generated/'generation.python.json').read_bytes())
            shutil.copyfile(generated/'bounded_records.py',stage/'roswasm/vendor_records.py')
            shutil.copyfile(source/'sdk/origin/robots/vendors/python_example.py',stage/'roswasm/vendor_entry.py')
            shutil.copyfile(source/'test/0073.ros-tests/support/source/ros_platform_sdk/check_python_vendor.py',stage/'roswasm/vendor_check.py')
            configuration=next(v for v in json.loads((source/'sdk/origin/robots/platforms'/vendor/'profiles.json').read_bytes())['profiles']if v['id']==profile)
            symbols={r['type']:r['symbol']for r in proof['records']}
            (stage/'roswasm/vendor_selected.py').write_text(
                '"""Compile-selected complete source profile; no grant."""\n'
                'from .vendor_records import '+symbols[configuration['state_type']]+' as State\n'
                'from .vendor_records import '+symbols[configuration['command_type']]+' as Command\n'
                'ROS_VERSION = '+str(version)+'\n')
            vectors=json.loads((verified/name/'python-vectors.json').read_bytes())
            (stage/'roswasm/vendor_fixture.py').write_text(
                '"""Independent original-source complete values; representation only."""\n'
                'PROOF = '+repr(dict(records=proof['records'],member_map=proof['member_map']))+'\n'
                'CASES = '+repr(vectors['cases'])+'\n')
            entry=directory/'main.py'
            entry.write_text('''"""One closed Python Guest: values or the actual vendor application."""
import sys
from roswasm import vendor_records, vendor_selected
# Actual frozen modules receive explicit local aliases; no substitute codec,
# Host object or import hook is installed.
sys.modules['bounded_records'] = vendor_records
sys.modules['selected_vendor'] = vendor_selected
def main():
    if sys.argv[1:] == ['values']:
        from roswasm.vendor_check import check
        from roswasm.vendor_fixture import PROOF, CASES
        result = check(vendor_records, PROOF, CASES)
        sys.stdout.write('PASS_COMPLETE_VENDOR_PYTHON_VALUES %d %d\\n' %
                         (result['records'], result['populated_scalar_or_string_instances']))
        return 0
    from roswasm.vendor_entry import main as application
    return application()
''')
            frozen=directory/'frozen'
            run(name+'-freeze',[native,'-I','-S','-B',source/'sdk/origin/python/tools/freeze.py',
                '--stdlib',cpython/'Lib','--sdk-python',stage,'--entry',entry,'--out',frozen],cwd=cpython)
            raw_recipe=run(name+'-link-recipe',[sys.executable,'-I','-S','-B',
                source/'test/0073.ros-tests/support/source/ros_custom_types/managed-records/python/link_recipe.py','--makefile',target/'Makefile'])
            recipe=json.loads(raw_recipe);(directory/'actual-link-recipe.json').write_bytes(raw_recipe)
            flags=[sdk/'bin/clang','--target=wasm32-wasip1','--sysroot='+str(sdk/'share/wasi-sysroot'),
                '-Os','-g0','-std=c11','-Werror','-DUWVM_DEBUG_LANGUAGE_PYTHON=0','-DROSWASM_OWNED_SERVICE=0',
                '-DROSWASM_BACKEND='+str(version),'-I'+str(target),'-I'+str(cpython/'Include'),
                '-I'+str(frozen),'-I'+str(source/'sdk/origin')]
            objects=[]
            for role,path in[('entry',source/'sdk/origin/python/embedding/main.c'),('bridge',source/'sdk/origin/python/roswasm/_native.c')]:
                obj=directory/(role+'.o');objects.append(obj)
                run(name+'-compile-'+role,[*flags,'-MD','-MF',directory/(role+'.d'),'-c',path,'-o',obj])
            libraries=[str((target/v).resolve(strict=True))if v.endswith('.a')and not Path(v).is_absolute()else v
                       for v in recipe['library_arguments']]
            artifact=directory/'vendor-python.wasm'
            run(name+'-link',[sdk/'bin/clang','--target=wasm32-wasip1','--sysroot='+str(sdk/'share/wasi-sysroot'),
                '-Os','-g0',*shlex.split(recipe['variables']['PY_CORE_LDFLAGS']),'-Wl,--threads=1',
                '-Wl,--max-memory=268435456',*objects,target/'libpython3.14.a',*libraries,'-o',artifact],cwd=target)
            run(name+'-validate',[inputs['validator'],'validate',artifact])
            run(name+'-imports',[sys.executable,'-I','-S','-B',source/'sdk/origin/tools/check_wasm_imports.py',
                artifact,'--backend',str(version),'--require-all','--allow-module','wasi_snapshot_preview1',
                '--report',directory/'imports.json'])
            expected=next(v['result']for v in verification['profiles']if(v['vendor'],v['profile'])==(vendor,profile))
            prefix=[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact]
            observed=run(name+'-values',[*prefix,'values'])
            assert observed==('PASS_COMPLETE_VENDOR_PYTHON_VALUES %d %d\n'%
                (expected['records'],expected['populated_scalar_or_string_instances'])).encode()
            for mode in('raw','state','command'):
                assert run(name+'-denied-'+mode,[*prefix,mode,vendor+'.observation'],expected=13)==b''
            for label,args in [('empty',[]),('bad-mode',['invalid','alias']),('empty-alias',['state',''])]:
                assert run(name+'-arguments-'+label,[*prefix,*args],expected=22)==b''
            report['profiles'].append(dict(selected_profile,artifact=pin(artifact),
                complete_value_result=expected,actual_guest_value_run=True,actual_default_denial_modes=3))
            save()
        report.update(status='TWO_REAL_CPYTHON_VENDOR_WASMS_FULL_VALUES_AND_ZERO_AUTHORITY_PASS')
        print(report['status'],len(report['commands']),flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
