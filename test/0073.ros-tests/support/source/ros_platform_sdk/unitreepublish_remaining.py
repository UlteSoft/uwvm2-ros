#!/usr/bin/env python3
"""Publish checked Unitree source inventory and safely refresh SDK views.

Copies only verified ordinary upstream text and complete recursive trees. The
official CMake/source entry review remains distinct from payload SDK coverage.
Existing views must match the independently retained previous source checkpoint
before an upgrade. No original executable, native driver or Guest runs here.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile


def facts(data):return dict(bytes=len(data),sha256=hashlib.sha256(data).hexdigest())


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    a=p.parse_args();root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    assert out.parent==root/'out'and not out.exists();out.mkdir(mode=0o700)
    report=dict(status='RUNNING',default_authority=[],commands=[],official_examples_ported=0,
                native_controller_or_transport_qualified=False)
    def save():(out/'unitreepublish.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv):
        with(out/(label+'.stdout')).open('xb')as stdout,(out/(label+'.stderr')).open('xb')as stderr:
            result=subprocess.run(list(map(str,argv)),stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr,timeout=180,check=False)
        report['commands'].append(dict(label=label,argv=list(map(str,argv)),exit=result.returncode));save()
        assert result.returncode==0,(label,(out/(label+'.stderr')).read_text()[-2600:])
    def write(path,data):
        path.parent.mkdir(parents=True,exist_ok=True)
        assert not path.is_symlink()and path.parent.resolve().is_relative_to(source)
        if path.exists():assert path.is_file()and path.read_bytes()==data,path
        else:
            with path.open('xb')as stream:stream.write(data)
    try:
        # An already closed, independently read-back checkpoint authorizes
        # only an exact old generated document, never a user-edited file.
        evidence=root.parent.parent/'recovery/ros-remaining-r60'
        retained=json.loads((evidence/'complete-python-platform-sdk-source-r1.json').read_bytes())
        client=json.loads((root/'control/client-readbacks/complete-python-platform-sdk-source-r1-client-readback.json').read_bytes())
        assert retained['status']=='TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS'
        assert client['status']=='CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS'
        assert all(client[k]==retained[k]for k in('bytes','sha256','manifest_sha256'))
        path=Path(retained['primary'])
        with path.open('rb')as stream:assert hashlib.file_digest(stream,'sha256').hexdigest()==retained['sha256']
        with tarfile.open(path,'r|gz')as archive:
            first=next(iter(archive));assert first.name=='manifest.json'and first.size<=16<<20
            raw=archive.extractfile(first).read();assert facts(raw)['sha256']==retained['manifest_sha256']
            manifest=json.loads(raw)
        prior=dict(kind='checked-public-sdk-view-source-state-v1',default_authority=[],files={},links={})
        for row in manifest['roles']:
            relative=Path(row['path'])
            if relative.parts[:3]not in (('source','sdk','platform'),('source','sdk','language')):continue
            dst=root/relative;name=relative.relative_to('source/sdk').as_posix()
            if row['type']=='regular':
                assert not dst.is_symlink()and facts(dst.read_bytes())=={k:row[k]for k in('bytes','sha256')}
                prior['files'][name]={k:row[k]for k in('bytes','sha256')}
            elif row['type']=='symlink-metadata':
                assert dst.is_symlink()and os.readlink(dst)==row['target']
                prior['links'][name]=row['target']
        (out/'previous-layout.json').write_text(json.dumps(prior,indent=2,sort_keys=True)+'\n')
        captures=root/inputs['unitree_source_capture']
        capture=json.loads((captures/'unitreecapture.json').read_bytes())
        assert capture['status']=='FOUR_COMPLETE_PINNED_TREES_AND_SELECTED_ORIGINAL_EXAMPLE_SOURCES_CAPTURED'
        value_path=root/'out/unitreesport-r3/unitreesport.json'
        build_path=root/'out/unitreesportbuild-r2/unitreesportbuild.json'
        values=json.loads(value_path.read_bytes());build=json.loads(build_path.read_bytes())
        assert values['status']=='ALL_THIRTY_NINE_SPORT_PAYLOAD_OPERATIONS_C_CPP_AND_REAL_WASM_DEFAULT_REFUSALS_PASS'
        assert build['status']=='DEFAULT_OFF_AND_EXPLICIT_COMPLETE_UNITREE_SPORT_CMAKE_WASI_PRODUCTS_PASS'
        spec=importlib.util.spec_from_file_location('unitree_published_git_objects',source/'sdk/origin/tools/git_source_tree.py')
        git=importlib.util.module_from_spec(spec);spec.loader.exec_module(git)
        kit=source/'sdk/origin/robots/unitree';destination=kit/'upstream/example-review-r1'
        published=[]
        for repo in capture['repositories']:
            short=repo['repository'].split('/')[1];old=captures/short
            raw=(old/'tree.json').read_bytes()
            assert facts(raw)==dict(bytes=repo['tree_bytes'],sha256=repo['tree_sha256'])
            assert git.tree_identity(json.loads(raw))==repo['tree']
            write(destination/short/'tree.json',raw)
            for row in repo['files']:
                body=(old/'files'/row['path']).read_bytes()
                assert facts(body)=={k:row[k]for k in('bytes','sha256')}
                assert hashlib.sha1(b'blob '+str(len(body)).encode()+b'\0'+body).hexdigest()==row['git_blob']
                write(destination/short/'files'/row['path'],body)
            write(destination/short/'source-lock.json',(json.dumps(repo,indent=2)+'\n').encode())
            published.append(dict(repository=repo['repository'],commit=repo['commit'],tree=repo['tree'],
                tree_file=(destination/short/'tree.json').relative_to(source).as_posix(),
                retained_source_files=len(repo['files'])))
        upstream=destination/'unitree_ros2/files/example/src'
        cmake=upstream/'CMakeLists.txt';text=cmake.read_text()
        targets=re.findall(r'(?m)^\s*add_executable\((\w+)\s+([^)]*)\)',text)
        disabled=re.findall(r'(?m)^\s*#add_executable\((\w+)\s+([^)]*)\)',text)
        entries=[]
        for name,body in targets+disabled:
            paths=body.split();assert all((upstream/path).is_file()for path in paths)
            mains=[path for path in paths if re.search(r'\bint\s+main\s*\(',(upstream/path).read_text())]
            assert len(mains)==1
            entries.append(dict(target=name,source_files=paths,entry_source=mains[0],
                source_build_status='COMMENTED_OUT'if(name,body)in disabled else'CONDITIONAL_YAML_CPP_0_6_OR_NEWER'if name=='g1_dual_arm_example'else'ENABLED',
                payload_adapter='sdk/origin/robots/unitree/examples/sport_operations.cpp'if name=='go2_sport_client'else None,
                port_status='PARTIAL_SPORT_PAYLOAD_AND_DEFAULT_REFUSAL_ONLY'if name=='go2_sport_client'else'REVIEWED_ENTRY_FULL_WASM_PORT_PENDING',
                native_driver_simulator_or_hardware_validation='NOT_ESTABLISHED_BY_THIS_LANE'))
        assert len(targets)==23 and len(disabled)==1 and len(entries)==24
        main_candidates=[row['path']for repo in capture['repositories']if repo['repository'].endswith('unitree_ros2')
                         for row in repo['files']if row['entry_signature_candidate']]
        assert {'example/src/'+row['entry_source']for row in entries}==set(main_candidates)
        review=dict(kind='pinned-unitree-source-example-review-v1',default_authority=[],
            repositories=published,ros2_cmake=facts(cmake.read_bytes()),
            reviewed_ros2_source_entry_points=24,enabled_without_yaml_condition=22,
            yaml_conditional_entries=1,commented_out_entries=1,entries=entries,
            implemented_sport_payload_methods=values['operations'],
            delivered_sport_payload_languages=['c','cpp'],
            complete_official_example_ports=0,official_suite_cases_run=0,
            value_and_default_refusal_result=facts(value_path.read_bytes()),
            optional_build_result=facts(build_path.read_bytes()),
            remaining_scope='Other SDK2/ROS1 tree entries are inventory only; full official programs and per-language/native/controller acceptance remain incomplete')
        write(kit/'example-review.json',(json.dumps(review,indent=2,sort_keys=True)+'\n').encode())
        spec=importlib.util.spec_from_file_location('unitree_updated_public_layout',source/'sdk/origin/tools/sdk_layout.py')
        layout=importlib.util.module_from_spec(spec);spec.loader.exec_module(layout)
        plan=layout.make_plan(source/'sdk/origin');plan.apply(prior)
        report['layout']=plan.check()
        run('source-layout-refusals',[sys.executable,'-I','-S','-B',source/'test/0073.ros-tests/support/source/ros_platform_sdk/test_sdk_layout.py',
            '--source',source,'--out',out/'source-models'])
        report.update(status='UNITREE_OFFICIAL_ENTRY_REVIEW_AND_COMPLETE_SPORT_PAYLOAD_SOURCE_VIEWS_PASS',
            reviewed_ros2_entries=len(entries),source_repositories=len(published),
            complete_sport_payload_methods=39,official_programs_completed=0,
            source_files_published=sum(v['retained_source_files']for v in published),
            independently_checked_previous_source_checkpoint=retained['sha256'])
        print(report['status'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
