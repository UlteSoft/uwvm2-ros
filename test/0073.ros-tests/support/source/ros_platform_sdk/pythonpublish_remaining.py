#!/usr/bin/env python3
"""Deliver qualified Python value sources into canonical per-language SDK kits.

Keeps source/representation evidence distinct from Guest/native-driver claims.
Updates only exact prior public documents, verifies actual source views and
refusal models, then exercises the inert upgrade stream against a private old
SDK fixture. No compiler, Guest, robot or native middleware runs in this phase.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def facts(data):return dict(bytes=len(data),sha256=hashlib.sha256(data).hexdigest())


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args();root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    prepared=root/'out/pythonprepare-r1';verified=root/'out/pythonverify-r3'
    verification=json.loads((verified/'pythonverify.json').read_bytes())
    assert verification['status']=='TWENTY_SIX_VENDOR_PYTHON_FULL_VALUE_AND_REFUSAL_PROFILES_PASS'
    assert out.parent==root/'out'and not out.exists()
    out.mkdir(mode=0o700)
    report=dict(status='RUNNING',profiles=[],commands=[],default_authority=[],
                execution_scope='Source views and native Python representation; Guest/native driver qualification is separate')
    def save():(out/'pythonpublish.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv):
        with(out/(label+'.stdout')).open('xb')as stdout,(out/(label+'.stderr')).open('xb')as stderr:
            result=subprocess.run(list(map(str,argv)),stdout=stdout,stderr=stderr,timeout=180,check=False)
        report['commands'].append(dict(label=label,argv=list(map(str,argv)),exit=result.returncode));save()
        assert result.returncode==0,(label,(out/(label+'.stderr')).read_text()[-2600:])
    clone=out/'old-source'
    try:
        # Clone ordinary sources by hardlink, but keep view documents independent:
        # those are the exact old bytes that the upgrade must recognize. This
        # temporary source fixture executes no transported SDK/application code.
        def copy(old,new):
            relative=Path(old).relative_to(source/'sdk/origin')
            if relative.parts[0]in('language','platform'):return shutil.copyfile(old,new)
            return os.link(old,new)
        shutil.copytree(source/'sdk/origin',clone/'sdk',symlinks=True,copy_function=copy)
        manifests={}
        for row in verification['profiles']:
            vendor,profile,family=row['vendor'],row['profile'],row['family']
            version=row['ros_version'];key='ros'+str(version)+'/'+family
            prefix=source/'sdk/origin/platform'/vendor/'python'
            selected=next(v for v in json.loads((source/'sdk/origin/robots/platforms'/vendor/'profiles.json').read_bytes())['profiles']if v['id']==profile)
            manifest=manifests.setdefault(vendor,dict(kind='complete-vendor-python-sdk-v1',default_authority=[],
                implicit_permissions=[],native_driver_qualified=False,official_examples_ported=0,profiles=[],files={}))
            manifest['profiles'].append({k:selected[k]for k in('id','family','ros_version','state_type','command_type')})
            inputs_map={
                'include/'+key+'/bounded_records.py':Path(row['output'])/'bounded_records.py',
                'include/'+key+'/generation.python.json':Path(row['output'])/'generation.python.json',
                'test/'+key+'/vectors.json':verified/(vendor+'-'+profile)/'python-vectors.json',
                'test/'+key+'/verification.json':verified/(vendor+'-'+profile)/'verification.json'}
            for name,original in inputs_map.items():
                data=original.read_bytes();destination=prefix/name
                assert destination.parent.resolve().is_relative_to(prefix)
                destination.parent.mkdir(parents=True,exist_ok=True)
                if destination.exists():assert destination.read_bytes()==data
                else:destination.write_bytes(data)
                manifest['files'][name]=facts(data)
            report['profiles'].append(dict(vendor=vendor,profile=profile,family=family,records=row['records']))
        for vendor,manifest in manifests.items():
            path=source/'sdk/origin/platform'/vendor/'python/sdk.python.json'
            data=(json.dumps(manifest,indent=2,sort_keys=True)+'\n').encode()
            if path.exists():assert path.read_bytes()==data
            else:path.write_bytes(data)
        tool=source/'sdk/origin/tools/sdk_layout.py'
        run('reviewed-view-update',[sys.executable,'-I','-S','-B',tool,'create','--previous',prepared/'previous-layout.json'])
        run('exact-view-check',[sys.executable,'-I','-S','-B',tool,'check'])
        run('view-refusal-models',[sys.executable,'-I','-S','-B',source/'test/0073.ros-tests/support/source/ros_platform_sdk/test_sdk_layout.py',
             '--source',source,'--out',out/'view-models'])
        report['status']='PYTHON_VENDOR_SDK_SOURCE_VIEWS_AND_NATIVE_VALUE_ACCEPTANCE_PASS';save()
        io=source/'test/0073.ros-tests/support/source/ros_platform_sdk/io_layout_delivery.py'
        export_argv=[sys.executable,'-I','-S','-B',io,'export','--source',source,'--receipt',out/'export.json']
        receive_argv=[sys.executable,'-I','-S','-B',io,'receive','--source',clone,'--receipt',out/'receive.json']
        with(out/'export.stderr').open('xb')as export_error,(out/'receive.stdout').open('xb')as stdout,(out/'receive.stderr').open('xb')as stderr:
            export=subprocess.Popen(list(map(str,export_argv)),stdout=subprocess.PIPE,stderr=export_error)
            try:
                receive=subprocess.run(list(map(str,receive_argv)),stdin=export.stdout,stdout=stdout,stderr=stderr,timeout=180,check=False)
                export.stdout.close();exit_code=export.wait(timeout=180)
            finally:
                if export.poll()is None:export.kill();export.wait()
        report['commands']+= [dict(label='inert-export',argv=list(map(str,export_argv)),exit=exit_code),
                             dict(label='inert-old-view-upgrade',argv=list(map(str,receive_argv)),exit=receive.returncode)]
        assert exit_code==receive.returncode==0,(out/'receive.stderr').read_text()[-2600:]
        exported,received=(json.loads((out/name).read_bytes())for name in('export.json','receive.json'))
        assert exported['status']==received['status']=='INERT_PUBLIC_SDK_SOURCE_DELIVERY_PASS'
        assert all(exported[k]==received[k]for k in('manifest_sha256','rows','ordinary_files','relative_links','ordinary_bytes'))
        # All outputs are an exact duplicate or retained source revision. Only
        # this closed private fixture is removed; actual SDK and evidence stay.
        shutil.rmtree(clone)
        report.update(inert_upgrade_and_private_fixture_retirement=True,delivered_profile_count=len(report['profiles']))
        assert report['delivered_profile_count']==26
        print(report['status'],26,flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
