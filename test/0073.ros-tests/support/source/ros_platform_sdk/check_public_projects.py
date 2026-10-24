#!/usr/bin/env python3
"""Build actual ROS source examples from a standalone platform-first SDK.

The preparation and compiler lanes run serially in the original finite Linux
owner. No maintenance origin is present in the distribution used for builds.
Every producer and output is recorded. Default-denial/local-value execution
does not establish native ROS, controller, official-suite or performance credit.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tomllib
import xml.etree.ElementTree as ET


def fact(path):
    path=Path(path)
    with path.open('rb')as f:sha=hashlib.file_digest(f,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=sha)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    p.add_argument('--phase',choices=('prepare','verify','frontends','catalog'),required=True)
    a=p.parse_args();root=a.root.resolve(strict=True);out=a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    toolchains=Path(inputs['toolchains']);wasi=toolchains/'wasi-sdk-33.0-x86_64-linux'
    assert not out.exists();out.mkdir();(out/'commands').mkdir()
    report=dict(status='RUNNING',commands=[],default_authority=[],official_suite_credit=0,
        native_ROS_qualified=False,hardware_performance_measured=False,compiled_products=[],executed_products=[])
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0,cwd=None,extra=None,timeout=180):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),cwd=str(cwd)if cwd else None,exit=None,expected_exit=expected)
        report['commands'].append(row);save()
        env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1',GOMAXPROCS='1',GOFLAGS='-p=1',
            GOTOOLCHAIN='local',GOPROXY='off',GOSUMDB='off',GOWORK='off',CGO_ENABLED='0',
            GOOS='wasip1',GOARCH='wasm',GOCACHE=str(out/'go-cache'),GOMODCACHE=str(out/'go-modcache'),
            CARGO_HOME=str(out/'cargo-home'),CARGO_BUILD_JOBS='1',
            RUSTC=str(toolchains/'rust-1.90.0/bin/rustc'),GOROOT=str(toolchains/'go'),
            PATH=str(toolchains/'go/bin')+':'+str(toolchains/'rust-1.90.0/bin')+':/usr/bin:/bin')
        env.update(extra or {})
        with(directory/'stdout').open('xb')as so,(directory/'stderr').open('xb')as se:
            process=subprocess.run(row['argv'],cwd=cwd,env=env,stdin=subprocess.DEVNULL,stdout=so,stderr=se,
                timeout=timeout,check=False)
        row.update(exit=process.returncode,stdout=fact(directory/'stdout'),stderr=fact(directory/'stderr'));save()
        assert expected is None or process.returncode==expected,(label,process.returncode,(directory/'stderr').read_text()[-3000:])
        return(directory/'stdout').read_bytes(),(directory/'stderr').read_bytes()
    def product(language,backend,name,path):
        run('validate-'+language+'-'+str(backend)+'-'+name,[inputs['validator'],'validate',path])
        row=dict(language=language,backend=backend,name=name,artifact=fact(path));report['compiled_products'].append(row);save()
        return row
    def guest(row,expected,argument=None,marker=None,grants=(),suffix='',scope='default-denial-or-local-value-only'):
        argv=[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment']
        for grant in grants:argv+=['--wasip1-global-grant',grant]
        argv+=['--run',row['artifact']['path']]
        if argument is not None:argv.append(argument)
        so,se=run('execute-'+row['language']+'-'+str(row['backend'])+'-'+row['name']+suffix,argv,expected)
        if marker is not None:assert marker in so+se,(row,so,se)
        report['executed_products'].append(dict(language=row['language'],backend=row['backend'],name=row['name'],
            actual_exit=expected,default_ROS_authority=[],WASI_grants=list(grants),scope=scope));save()
    try:
        if a.phase=='catalog':
            state=json.loads((root/inputs['project_sdk_prepared']/'result.json').read_bytes());sdk=Path(state['sdk'])
            earlier=root/inputs['project_previous_sdk']/'sdk'
            old_manifest=json.loads((earlier/'share/distribution-manifest.json').read_bytes())
            new_manifest=json.loads((sdk/'share/distribution-manifest.json').read_bytes())
            old_files={v['path']:v for v in old_manifest['entries']};new_files={v['path']:v for v in new_manifest['entries']}
            assert not set(old_files)-set(new_files)
            assert set(new_files)-set(old_files)=={'share/platform/ros/docs/standalone-examples.md'}
            changed=[]
            for name,row in new_files.items():
                actual=fact(sdk/name);assert all(actual[k]==row[k]for k in('bytes','sha256'))
                if name in old_files and any(row[k]!=old_files[name][k]for k in('bytes','sha256')):changed.append(name)
            assert changed==['example/platform/ros/as/assemblyscript/06-events-tf-lifecycle.ts'],changed
            report['SDK_product_source_comparison']=dict(previous=str(earlier),current=str(sdk),
                changed=changed,added=sorted(set(new_files)-set(old_files)),
                Rust_Go_TinyGo_source_and_library_bytes_unchanged=True)
            for backend in(1,2):
                artifact=root/'out/sdk-layout-projects-verify-r2'/('rust-'+str(backend))/'wasm32-wasip1/release/raw.wasm'
                run('public-import-checker-'+str(backend),[sys.executable,'-I','-S','-B',sdk/'tools/check_wasm_imports.py',
                    artifact,'--backend',str(backend),'--allow-module','wasi_snapshot_preview1',
                    '--report',out/('public-imports-'+str(backend)+'.json')])
            previous=root/inputs['sdk_test_catalog']/'index.json'
            run('numbering-contracts',[sys.executable,'-I','-S','-B',
                source/'test/0073.ros-tests/support/source/ros_platform_sdk/test_numbered_catalog.py'])
            run('prepare-numbered-catalog',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_test_catalog.py',
                '--source',source/'test/0073.ros-tests/support/source','--out',out/'ros-tests',
                '--previous-index',previous])
            old={v['source']:v for v in json.loads(previous.read_bytes())['cases']}
            current=json.loads((out/'ros-tests/index.json').read_bytes());changes=[]
            for row in current['cases']:
                actual=fact(source/'test/0073.ros-tests'/row['source'])
                assert all(actual[k]==row['source_'+k]for k in('bytes','sha256'))
                if row['source']in old:
                    assert row['id']==old[row['source']]['id']
                    if row!=old[row['source']]:changes.append(dict(before=old[row['source']],after=row))
            report.update(status='NUMBERED_CATALOG_EXTENDED_WITH_STABLE_IDENTIFIERS',
                previous_cases=len(old),cases=current['cases_prepared'],
                added_sources=sorted({v['source']for v in current['cases']}-set(old)),
                changed_source_pins=changes,test_catalog=str(out/'ros-tests'))
        elif a.phase=='prepare':
            run('prepare-public-sdk',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_distribution.py',
                '--origin',source/'sdk/origin','--out',out/'sdk','--wasi-sysroot',wasi/'share/wasi-sysroot',
                '--fast-io',source/'third-parties/fast_io/include'],timeout=600)
            for name in('README.md','xmake.lua','CMakeLists.txt'):shutil.copyfile(source/'sdk'/name,out/'sdk'/name)
            shutil.copytree(source/'sdk/tools',out/'sdk/tools',dirs_exist_ok=True)
            report.update(status='STANDALONE_PROJECT_SOURCES_PREPARED',sdk=str(out/'sdk'),applications_executed=0)
        else:
            accepted=json.loads((root/inputs['project_sdk_prepared']/'result.json').read_bytes())
            assert accepted['status']=='STANDALONE_PROJECT_SOURCES_PREPARED'
            sdk=Path(accepted['sdk']);assert not(sdk/'origin').exists()
            manifest=json.loads((sdk/'share/distribution-manifest.json').read_bytes())
            for row in manifest['entries']:
                actual=fact(sdk/row['path']);assert all(actual[k]==row[k]for k in('bytes','sha256'))
            # Resolve real package/project references before invoking producers.
            rust=sdk/'example/platform/ros/rs/rust'
            cargo=tomllib.loads((rust/'Cargo.toml').read_text())
            assert(rust/cargo['dependencies']['roswasm']['path']).resolve()==sdk/'include/platform/ros/rs'
            modules=list((sdk/'example').rglob('go.mod'));references=0
            for module in modules:
                for line in module.read_text().splitlines():
                    if '=>'not in line:continue
                    local=line.split('=>',1)[1].strip()
                    target=(module.parent/local).resolve(strict=True)
                    assert target.is_relative_to(sdk/'include')and(target/'go.mod').is_file();references+=1
            managed=0
            for extension in('*.csproj','*.fsproj','*.vbproj'):
                for project in(sdk/'example').rglob(extension):
                    for element in ET.parse(project).getroot().iter():
                        if element.tag not in('ProjectReference','Import'):continue
                        spelling=element.get('Include')or element.get('Project')
                        if not spelling or '$('in spelling:continue
                        target=(project.parent/spelling).resolve(strict=True)
                        assert target.is_relative_to(sdk);managed+=1
            report.update(local_Go_module_references=references,managed_project_paths_checked=managed,
                managed_compiler_or_Runtime_qualification=False);save()
            if a.phase=='frontends':
                # Node checks syntax and the pure host-codec lesson only. It
                # supplies no credit for an embedded JavaScript Wasm engine.
                javascript=sorted((sdk/'example').rglob('*.mjs'))
                if inputs.get('project_js_checked'):
                    earlier=root/inputs['project_js_checked'];accepted=json.loads((earlier/'result.json').read_bytes())
                    commands=[v for v in accepted['commands']if v['label'].startswith('js-')]
                    assert len(commands)==len(javascript)+1
                    old_sdk=next(p for p in Path(commands[0]['argv'][-1]).parents if p.name=='sdk')
                    assert old_sdk.is_relative_to(root/'out')
                    assert [Path(v['argv'][-1]).relative_to(old_sdk)for v in commands[:-1]]==[p.relative_to(sdk)for p in javascript]
                    old_manifest=json.loads((old_sdk/'share/distribution-manifest.json').read_bytes())
                    for row in old_manifest['entries']:
                        if Path(row['path']).suffix not in('.js','.mjs'):continue
                        assert all(fact(sdk/row['path'])[k]==row[k]for k in('bytes','sha256'))
                    for command in commands:
                        assert command['exit']==0
                        for stream in('stdout','stderr'):
                            assert fact(command[stream]['path'])==command[stream]
                    so=Path(commands[-1]['stdout']['path']).read_bytes()
                    report['reused_exact_JS_source_checks']=dict(result=fact(earlier/'result.json'),
                        commands=len(commands),reason='All public JS module bytes unchanged, including imports; exact completed logs reopened.')
                else:
                    for index,entry in enumerate(javascript):
                        run('js-syntax-'+str(index),['/usr/bin/node','--max-old-space-size=128','--check',entry])
                    so,se=run('js-host-codecs',['/usr/bin/node','--max-old-space-size=128',
                        sdk/'example/platform/ros/js/javascript/02-codecs.mjs'])
                assert b'Decoded: hello from JavaScript'in so and b'9223372036854775807'in so
                report.update(javascript_source_syntax_checks=len(javascript),javascript_embedded_Wasm_qualified=False);save()
                base=sdk/'example/platform/ros/as/assemblyscript'
                for backend in(1,2):
                    lessons=sorted((base/'ros1'if backend==1 else base).glob('[0-9][0-9]-*.ts'))
                    assert len(lessons)==9
                    for entry in lessons:
                        name=entry.stem;local=name.startswith('07-');expected=0 if local else 13
                        wrapper=out/('as-'+str(backend)+'-'+name+'.ts')
                        module=os.path.relpath(entry.with_suffix(''),out)
                        wrapper.write_text('/** Calls the unchanged actual lesson; no mock imports or authority. */\n'
                            +'import { _start as lesson, lastStatus'+(', completedTicks'if local else'')+' } from '+json.dumps(module)+';\n'
                            +'export { abortGuest } from '+json.dumps(os.path.relpath(sdk/'include/platform/ros/as/api',out))+';\n'
                            +'export function _start():void { lesson(); if(lastStatus()!='+str(expected)+') unreachable();'
                            +(' if(completedTicks()!=1) unreachable();'if local else'')+' }\n')
                        artifact=wrapper.with_suffix('.wasm')
                        run('as-build-'+str(backend)+'-'+name,['/usr/bin/node','--max-old-space-size=256',
                            '--disable-wasm-trap-handler',toolchains/'node_modules/assemblyscript/bin/asc.js',wrapper.name,
                            '--outFile',artifact,'--runtime','incremental','--initialMemory','128','--maximumMemory','512',
                            '--optimizeLevel','0','--shrinkLevel','0','--use',
                            'abort='+os.path.relpath(sdk/'include/platform/ros/as/api',out)+'/abortGuest'],
                            cwd=out,timeout=600)
                        row=product('as',backend,name,artifact)
                        guest(row,0,scope='actual-lesson-diagnostic-checked-by-Wasm-harness')
                report.update(status='STANDALONE_JS_SOURCE_AND_ASSEMBLYSCRIPT_GUEST_CHECKS_PASS',
                    compiler_products=len(report['compiled_products']),actual_Guest_executions=len(report['executed_products']))
                return
            run('rust-version',[toolchains/'rust-1.90.0/bin/rustc','--version'])
            run('go-version',[toolchains/'go/bin/go','version'])
            run('tinygo-version',[toolchains/'tinygo/bin/tinygo','version'])
            for backend in(1,2):
                target=out/('rust-'+str(backend))
                flags='\x1f'.join(['-C','codegen-units=1','-C','debuginfo=0','-C','link-arg=--threads=1',
                    '-C','link-arg='+str(sdk/'lib/wasm32-wasip1/libc.a')])
                run('rust-build-'+str(backend),[toolchains/'rust-1.90.0/bin/cargo','build','--offline','--release','-j1',
                    '--manifest-path',rust/'Cargo.toml','--target','wasm32-wasip1','--target-dir',target,
                    '--no-default-features','--features','ros'+str(backend)+',alloc','--bins'],
                    extra=dict(CARGO_ENCODED_RUSTFLAGS=flags),timeout=600)
                for entry in cargo['bin']:
                    if 'ros2'in entry.get('required-features',[])and backend==1:continue
                    name=entry['name'];row=product('rs',backend,name,target/'wasm32-wasip1/release'/(name+'.wasm'))
                    if name=='coverage':continue
                    if name=='scheduling':guest(row,0,marker=b'one local timer tick')
                    else:guest(row,1,marker=b'13')
            for backend in(1,2):
                for name,base,marker in [('telemetry',sdk/'example/platform/ros/go/go',b'PASS_ROS_DEFAULT_DENIED'),
                        ('resources',sdk/'example/platform/ros/go/go/resources',b'PASS_GO_ROS_RESOURCES_DEFAULT_DENIED')]:
                    artifact=out/('go-'+str(backend)+'-'+name+'.wasm')
                    run('go-build-'+str(backend)+'-'+name,[toolchains/'go/bin/go','build','-mod=readonly','-p=1',
                        '-tags=ros'+str(backend),'-o',artifact,'.'],cwd=base,timeout=600)
                    row=product('go',backend,name,artifact)
                    guest(row,2,'deny',b'clock_time_get failed',suffix='-startup-denied',
                        scope='Go-runtime-startup-refuses-ungranted-monotonic-clock')
                    guest(row,0,'deny',marker,grants=('monotonic-clock',),suffix='-ROS-denied',
                        scope='ROS-default-denial-with-explicit-WASI-runtime-clock')
            base=sdk/'example/platform/ros/tinygo/tinygo'
            names=['raw','typed','node','resources','servers','scheduling','action_parameters','tf_graph_clock',
                'unified_events','native_resources','sdk_codec_debug','codec_benchmark','coverage','events_tf_lifecycle']
            for backend in(1,2):
                for name in names:
                    if name=='events_tf_lifecycle'and backend==1:continue
                    artifact=out/('tinygo-'+str(backend)+'-'+name+'.wasm')
                    run('tinygo-build-'+str(backend)+'-'+name,[toolchains/'tinygo/bin/tinygo','build','-p','1',
                        '-target=wasip1','-scheduler=none','-opt=0','-panic=trap','-tags=ros'+str(backend),
                        '-o',artifact,'./'+name],cwd=base,timeout=600)
                    row=product('tinygo',backend,name,artifact)
                    if name=='native_resources':guest(row,0,'deny',b'PASS_TINYGO_ROS_RESOURCES_DEFAULT_DENIED')
                    if name=='scheduling':guest(row,0,marker=b'one local tick')
                    if name=='sdk_codec_debug':guest(row,0)
            report.update(status='STANDALONE_RUST_GO_TINYGO_PROJECTS_AND_EXPLICIT_AUTHORITY_CHECKS_PASS',
                compiler_products=len(report['compiled_products']),actual_Guest_executions=len(report['executed_products']))
        print(report['status'],flush=True)
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
