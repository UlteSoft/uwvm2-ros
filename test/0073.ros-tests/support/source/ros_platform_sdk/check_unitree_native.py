#!/usr/bin/env python3
"""Reproduce the public SDK and rebuild pinned original Unitree ROS2 messages.

Run through the original finite Linux cgroup owner. Original package files are
checked against the saved complete Git tree before configuration. Native SDK
mounts are read-only and build outputs stay in this task's owned scratch tree.
Package tests, native DDS and Wasm/Gateway acceptance are distinct claims.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import shlex
import stat
import subprocess
import sys
import xml.etree.ElementTree as ET


def fact(path):
    path=Path(path)
    with path.open('rb')as stream:h=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(bytes=path.stat().st_size,sha256=h)


def load(name,path):
    spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec)
    sys.modules[name]=module;spec.loader.exec_module(module);return module


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    p.add_argument('--phase',choices=('reproduce','package','peer','session','audit','capture','catalog','proof'),required=True)
    a=p.parse_args();root=a.root.resolve(strict=True);out=a.out;out.mkdir();(out/'commands').mkdir()
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    report=dict(status='RUNNING',commands=[],default_authority=[],official_full_driver_suite_credit=0,
        native_robot_qualified=False,Wasm_Gateway_transport_qualified=False,QEMU_qualified=False,VTune_qualified=False)
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0,env=None,timeout=600):
        d=out/'commands'/label;d.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),exit=None);report['commands'].append(row);save()
        with(d/'stdout').open('xb')as so,(d/'stderr').open('xb')as se:
            child=subprocess.run(row['argv'],env=env,stdin=subprocess.DEVNULL,stdout=so,stderr=se,timeout=timeout)
        row.update(exit=child.returncode,stdout=fact(d/'stdout'),stderr=fact(d/'stderr'));save()
        if expected is not None:assert child.returncode==expected,(label,child.returncode,(d/'stderr').read_text()[-3000:])
        return child.returncode
    try:
        if a.phase=='reproduce':
            sdk=root/'out/sdk-layout-sport-prepare-r3/sdk';sys.path.insert(0,str(source/'sdk/tools'))
            generator=load('current_platform_sdk_generator',source/'sdk/tools/prepare_distribution.py')
            checked=[]
            presence={'lib/wasm32-wasip1/libc.a','lib/wasm32-wasip1/crt1.o','include/wasm32-wasip1/stdio.h'}
            class Compare(generator.Distribution):
                def emit(self,relative,body,*,source=None,transform=None):
                    name=Path(relative).as_posix();actual=sdk/name
                    assert actual.is_file()and not actual.is_symlink(),name
                    assert actual.read_bytes()==body,('fresh assembly differs',name)
                    if name in self.files:return
                    if name in presence:
                        target=self.out/name;target.parent.mkdir(parents=True,exist_ok=True);os.link(actual,target)
                    row=dict(path=name,**generator.fact(body))
                    if source is not None:
                        row['original']=dict(path=source.relative_to(self.origin).as_posix(),**generator.fact(source.read_bytes()))
                        if transform:row['transformation']=transform
                    self.files[name]=row;checked.append(dict(path=name,**generator.fact(body)))
            generator.Distribution=Compare
            # The normal assembler checks three conventional paths exist. The
            # comparison sink emits exact input hardlinks for those three,
            # never placeholder headers/libraries or substitute compiler input.
            probe=out/'fresh-assembly'
            manifest=generator.prepare(source/'sdk/origin',probe,
                Path(inputs['toolchains'])/'wasi-sdk-33.0-x86_64-linux/share/wasi-sysroot',[],[],source/'third-parties/fast_io/include')
            assert len(checked)==8315 and len(manifest['entries'])==8313
            report.update(status='FRESH_ASSEMBLER_BYTES_MATCH_ALL_PUBLIC_FILES',files_compared=len(checked),
                manifest_entries=len(manifest['entries']),comparison_sink=True,source_generation_executed=True,
                official_sysroot_read=True,temporary_exact_hardlinks=3,SDK_bodies_copied=0)
            (out/'compared-files.json').write_text(json.dumps(checked,indent=2)+'\n');shutil.rmtree(probe)
        elif a.phase=='capture':
            product=json.loads((root/'out/sdk-layout-native-peer-r6/native-product.json').read_bytes())
            session=json.loads((root/'out/sdk-layout-native-session-r6/session/result.json').read_bytes())
            sdk=Path('/home/macromodel/.local/share/uwvm-ros-extension/platform-sdk-inputs-20261020-r56-r2/humble')
            selected={Path(path):pin for path,pin in product['dependencies'].items()}
            for row in product['native_sdk_tools']:selected[Path(row['path'])]={k:row[k]for k in('bytes','sha256')}
            for pair in session['pairs']:
                for role in pair['actual_loaded_files'].values():
                    for name,row in role['files'].items():
                        p=Path(name);path=root/p.relative_to('/tmp')if p.is_relative_to('/tmp')else sdk/p.relative_to('/')
                        selected[path]={k:row[k]for k in('bytes','sha256')}
            objects=out/'objects';objects.mkdir();rows=[];seen={}
            for path,pin in sorted(selected.items()):
                resolved=path.resolve(strict=True)
                assert resolved.is_relative_to(root)or resolved.is_relative_to(sdk),str(resolved)
                before=resolved.stat();assert stat.S_ISREG(before.st_mode)and fact(resolved)==pin
                key=pin['sha256']+'-'+oct(stat.S_IMODE(before.st_mode))[2:]
                if key not in seen:
                    with resolved.open('rb')as src,(objects/key).open('xb')as dst:shutil.copyfileobj(src,dst,1<<20)
                    (objects/key).chmod(stat.S_IMODE(before.st_mode));assert fact(objects/key)==pin
                    seen[key]=pin
                after=resolved.stat();assert before.st_dev==after.st_dev and before.st_ino==after.st_ino and before.st_mtime_ns==after.st_mtime_ns and before.st_ctime_ns==after.st_ctime_ns
                rows.append(dict(path=str(path),resolved=str(resolved),object='objects/'+key,mode=stat.S_IMODE(before.st_mode),**pin))
            (out/'closure.json').write_text(json.dumps(dict(kind='native-compiler-dependency-and-loaded-file-bodies-v1',entries=rows),indent=2)+'\n')
            report.update(status='EXACT_NATIVE_USED_FILE_BODIES_CAPTURED',roles=len(rows),objects=len(seen),
                bytes=sum(v['bytes']for v in seen.values()),complete_native_SDK_toolchain_claim=False)
        elif a.phase=='audit':
            def read(path):return json.loads(path.read_bytes())
            def verify_pin(path,pin):assert fact(path)=={k:pin[k]for k in('bytes','sha256')},str(path)
            peaks={}
            for name,status in(('native-reproduce-r2','FRESH_ASSEMBLER_BYTES_MATCH_ALL_PUBLIC_FILES'),
                    ('native-package-r1','ORIGINAL_UNITREE_API_BUILD_AND_TESTS_PASS'),
                    ('native-session-r6','UNITREE_COMPLETE_API_NATIVE_DDS_CARRIERS_PASS')):
                base=root/('out/sdk-layout-'+name);value=read(base/'result.json');owner=read(base.with_suffix('.json'))
                assert value['status']==status and owner['status']=='PASS'
                assert all(c['exit']==0 and c['forced_signal']is None for c in owner['commands'])
                for key in('events_before','events_after'):
                    counters=dict(line.split()for line in owner[key].splitlines());assert all(counters[k]=='0'for k in('oom','oom_kill','oom_group_kill'))
                peaks[name]=max(c['max_sampled_owned_rss']for c in owner['commands'])
            package=root/'out/sdk-layout-native-package-r1';native=read(package/'result.json')
            inventory=source/'sdk/origin/robots/unitree/upstream/example-review-r1/unitree_ros2'
            lock=read(inventory/'source-lock.json');verify_pin(inventory/'tree.json',dict(bytes=lock['tree_bytes'],sha256=lock['tree_sha256']))
            tree=read(inventory/'tree.json');prefix='cyclonedds_ws/src/unitree/unitree_api/'
            complete={x['path'][len(prefix):]for x in tree['tree']if x['type']=='blob'and x['path'].startswith(prefix)}
            assert complete=={x['original']for x in native['original_source_files']}and len(complete)==10
            for row in native['original_source_files']:verify_pin(Path(native['package'])/row['original'],row)
            for row in native['installed_products']:verify_pin(root/row['path'],row)
            cases=ET.fromstring((package/'upstream-junit.xml').read_bytes()).findall('.//testcase')
            assert len(cases)==3 and {c.get('name')for c in cases}=={'copyright','lint_cmake','xmllint'}
            assert all(c.find('failure')is None and c.find('error')is None and c.find('skipped')is None for c in cases)
            product=read(root/'out/sdk-layout-native-peer-r6/native-product.json')
            for row in(product['artifact'],product['cpp_source'],product['cmake_source'],*product['native_sdk_tools']):verify_pin(row['path'],row)
            for path,pin in product['dependencies'].items():verify_pin(path,pin)
            elf=Path(product['artifact']['path']).read_bytes();assert elf[:5]==b'\x7fELF\x02'and int.from_bytes(elf[18:20],'little')==62
            session_root=root/'out/sdk-layout-native-session-r6/session';session=read(session_root/'result.json')
            assert session['network_proof']['interfaces']==['lo']and session['network_proof']['namespace']!=session['network_proof']['parent_namespace']
            assert len(session['pairs'])==8 and session['complete_requests_received']==336
            sdk=Path('/home/macromodel/.local/share/uwvm-ros-extension/platform-sdk-inputs-20261020-r56-r2/humble')
            combinations=set()
            for row in session['pairs']:
                key=(row['rmw'],row['language']);assert key not in combinations;combinations.add(key)
                assert row['status']=='FULL_GENERATED_REQUEST_DDS_DELIVERY_PASS'and row['received']==42
                directory=session_root/(row['rmw']+'-'+row['language']);verify_pin(directory/'transport.xml',row['transport_profile'])
                assert len(row['delivery_offsets_ns'])==42 and row['delivery_offsets_ns']==sorted(row['delivery_offsets_ns'])
                for role,verb in(('writer','PUBLISHED'),('reader','DELIVERED')):
                    expected=['READY '+role,'MATCHED '+role,*[verb+' '+str(i)for i in range(1,43)],'PASS '+role]
                    assert(directory/(role+'.stdout')).read_text().splitlines()==expected
                    for stream in('stdout','stderr'):verify_pin(directory/(role+'.'+stream),row['outputs'][role][stream])
                    loaded=row['actual_loaded_files'][role]['files'];assert any('/librmw_'+row['rmw'].removeprefix('rmw_')+'.so'in p for p in loaded)
                    for path,pin in loaded.items():
                        p=Path(path);actual=root/p.relative_to('/tmp')if p.is_relative_to('/tmp')else sdk/p.relative_to('/')
                        verify_pin(actual,pin)
            assert combinations=={(rmw,lang)for rmw in('rmw_fastrtps_cpp','rmw_cyclonedds_cpp')for lang in('rs','as','tinygo','js')}
            report.update(status='INDEPENDENT_UNITREE_NATIVE_PACKAGE_AND_DDS_EVIDENCE_PASS',official_interface_package_cases=3,
                complete_original_package_files=10,installed_products=len(native['installed_products']),native_requests=336,
                Wasm_produced_native_requests=252,host_JS_produced_native_requests=84,compiled_native_architecture='x86_64',
                native_distribution='Humble',compiler_dependency_files=len(product['dependencies']),sampled_peak_rss=peaks)
        elif a.phase=='catalog':
            previous=root/'out/sdk-layout-sport-catalog-r1/ros-tests/index.json'
            run('catalog',[sys.executable,'-I','-S','-B',source/'sdk/tools/prepare_test_catalog.py','--source',source/'test/0073.ros-tests/support/source',
                '--out',out/'ros-tests','--previous-index',previous])
            old={r['source']:r for r in json.loads(previous.read_bytes())['cases']}
            value=json.loads((out/'ros-tests/index.json').read_bytes())
            for row in value['cases']:
                if row['source']in old:assert row==old[row['source']]
                actual=fact(source/'test/0073.ros-tests'/row['source']);assert all(row['source_'+k]==actual[k]for k in('bytes','sha256'))
            added=[r for r in value['cases']if r['source']not in old]
            assert len(old)==417 and len(added)==2
            assert {Path(r['source']).name for r in added}=={'check_unitree_native.py','unitree_native_api_peer.cpp'}
            report.update(status='UNITREE_NATIVE_SOURCE_CATALOG_PASS',cases=value['cases_prepared'],added=added)
        else:
            sdk=Path('/home/macromodel/.local/share/uwvm-ros-extension/platform-sdk-inputs-20261020-r56-r2/humble')
            assert(sdk/'opt/ros/humble/setup.sh').is_file()
            identity=[dict(path=str(sdk/n),**fact(sdk/n))for n in('etc/os-release','usr/bin/g++','usr/bin/cmake','usr/bin/python3','opt/ros/humble/setup.sh')]
            mount=out/'chroot-exec.py'
            mount.write_text('''import os,pathlib,stat,subprocess,sys
sdk,root=sys.argv[1:3]
def run(argv):subprocess.run(argv,check=True,timeout=10)
assert pathlib.Path(root).is_mount()
run(['/bin/mount','--bind',sdk,sdk]);run(['/bin/mount','-o','remount,bind,ro',sdk])
run(['/bin/mount','--bind',root,sdk+'/tmp'])
run(['/bin/mount','--bind','/proc',sdk+'/proc']);run(['/bin/mount','-o','remount,bind,ro',sdk+'/proc'])
run(['/bin/mount','-t','tmpfs','-o','size=256k,nr_inodes=16,mode=755,nosuid,noexec','unitree-native-null',sdk+'/dev'])
os.mknod(sdk+'/dev/null',stat.S_IFCHR|0o666,os.makedev(1,3))
os.mknod(sdk+'/dev/urandom',stat.S_IFCHR|0o666,os.makedev(1,9))
os.mknod(sdk+'/dev/random',stat.S_IFCHR|0o666,os.makedev(1,8))
os.mkdir(sdk+'/dev/shm',0o1777)
run(['/bin/mount','-t','tmpfs','-o','size=64M,nr_inodes=128,mode=1777,nosuid,nodev,noexec','unitree-native-shm',sdk+'/dev/shm'])
run(['/usr/bin/ip','link','set','lo','up'])
os.chroot(sdk);os.chdir('/tmp');os.execvpe(sys.argv[3],sys.argv[3:],os.environ)
''')
            inside=lambda path:'/tmp/'+str(path.relative_to(root))
            env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1',HOME=inside(out),TMPDIR=inside(out),
                CMAKE_BUILD_PARALLEL_LEVEL='1',MAKEFLAGS='-j1',OMP_NUM_THREADS='1',OPENBLAS_NUM_THREADS='1',
                ROS_LOCALHOST_ONLY='1',ROS_DOMAIN_ID='209',ROS_SECURITY_ENABLE='false')
            def native(label,argv,expected=0,timeout=600):
                shell=['bash','--noprofile','--norc','-c','set -e; source /opt/ros/humble/setup.sh; exec "$@"','unitree-native',*map(str,argv)]
                return run(label,['/usr/bin/unshare','--mount','--net','--propagation','private',sys.executable,'-I','-S','-B',mount,sdk,root,*shell],expected,env,timeout)
            if a.phase=='package':
                inventory=source/'sdk/origin/robots/unitree/upstream/example-review-r1/unitree_ros2'
                tree=json.loads((inventory/'tree.json').read_bytes());assert tree['sha']=='668d1ec5a05d1c38d3306bdca7d59f2ba3581a88'and not tree['truncated']
                blobs={r['path']:r for r in tree['tree']if r['type']=='blob'}
                original='cyclonedds_ws/src/unitree/unitree_api/'
                package=out/'original-unitree-api';package.mkdir();pins=[]
                files=[(Path(__file__).with_name('fixtures')/'unitree_api'/name,name)for name in('CMakeLists.txt','package.xml')]
                messages=source/'sdk/origin/robots/unitree/upstream/python-api-r1/unitree_ros2/files'/original/'msg'
                files +=[(p,'msg/'+p.name)for p in sorted(messages.glob('*.msg'))]
                assert len(files)==10
                for path,name in files:
                    body=path.read_bytes();entry=blobs[original+name]
                    assert len(body)==entry['size']and hashlib.sha1(b'blob '+str(len(body)).encode()+b'\0'+body).hexdigest()==entry['sha'],name
                    dest=package/name;dest.parent.mkdir(exist_ok=True);dest.write_bytes(body);pins.append(dict(original=name,git_blob=entry['sha'],**fact(dest)))
                report.update(original_source_files=pins,source_commit=tree['sha'],native_sdk_tools_before=identity);save()
                native('configure',['cmake','-S',inside(package),'-B',inside(out/'build'),'-DCMAKE_BUILD_TYPE=Release',
                    '-DCMAKE_C_COMPILER=/usr/bin/gcc','-DCMAKE_CXX_COMPILER=/usr/bin/g++','-DCMAKE_INSTALL_PREFIX='+inside(out/'install'),
                    '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON','-DBUILD_TESTING=ON','-DPython3_EXECUTABLE=/usr/bin/python3'])
                native('build',['cmake','--build',inside(out/'build'),'--parallel','1'],timeout=900)
                native('install',['cmake','--install',inside(out/'build')])
                native('discover-tests',['ctest','--test-dir',inside(out/'build'),'-N'])
                result=native('upstream-tests',['ctest','--test-dir',inside(out/'build'),'--output-on-failure','--no-compress-output',
                    '--output-junit',inside(out/'upstream-junit.xml')],expected=None)
                cases=ET.fromstring((out/'upstream-junit.xml').read_bytes()).findall('.//testcase')
                failures=sum(c.find('failure')is not None or c.find('error')is not None for c in cases)
                products=[dict(path=str(p.relative_to(root)),**fact(p))for p in sorted((out/'install').rglob('*'))if p.is_file()and not p.is_symlink()]
                assert products and all(fact(package/r['original'])=={k:r[k]for k in('bytes','sha256')}for r in pins)
                report.update(status='ORIGINAL_UNITREE_API_BUILD_AND_TESTS_PASS'if result==0 else'ORIGINAL_UNITREE_API_BUILD_PASS_UPSTREAM_FAILURES_RETAINED',
                    original_package_cases_run=len(cases),original_package_case_failures=failures,original_package_case_skips=sum(c.find('skipped')is not None for c in cases),
                    installed_products=products,package=str(package),install=str(out/'install'))
            else:
                package=root/'out/sdk-layout-native-package-r1'
                package_report=json.loads((package/'result.json').read_bytes())
                assert package_report['status']in('ORIGINAL_UNITREE_API_BUILD_AND_TESTS_PASS','ORIGINAL_UNITREE_API_BUILD_PASS_UPSTREAM_FAILURES_RETAINED')
                for row in package_report['installed_products']:
                    assert fact(root/row['path'])=={k:row[k]for k in('bytes','sha256')}
                install=Path(package_report['install'])
                if a.phase=='proof':
                    # The installed dispatcher dlopens introspection by its
                    # original soname; executable RUNPATH alone does not make
                    # this private package visible to that dispatcher.
                    env['LD_LIBRARY_PATH']=inside(install/'lib')+':/opt/ros/humble/lib:/opt/ros/humble/lib/x86_64-linux-gnu'
                    # Re-materialize the entire locked family. Do not use the
                    # historical generated codec as a source/identity oracle.
                    unitree=source/'sdk/origin/robots/unitree'
                    materialized=out/'original-catalog'
                    run('materialize-complete',[sys.executable,'-I','-S','-B',unitree/'materialize_catalog.py',
                        '--upstream-root',unitree/'upstream/python-api-r1/unitree_ros2/files',
                        '--profile',unitree/'profiles/unitree_api.json','--lock',unitree/'upstream-lock.json',
                        '--out',materialized,'--catalog-format','uwvm-ros-vendor-complete-bounded-v1'])
                    catalog=materialized/'catalog.preparation.json';document=json.loads(catalog.read_bytes())
                    assert len(document['types'])==8
                    request=out/'installed-request.json'
                    request.write_text(json.dumps(dict(format='uwvm-ros2-installed-message-idl-source-v1',
                        records=[dict(type=row['type'],idl=inside(install/'share'/row['type'].split('/')[0]/'msg'/
                            (row['type'].split('/')[-1]+'.idl')))for row in document['types']]),indent=2)+'\n')
                    proof_tools=source/'sdk/origin/robots/industrial/pre_rihs'
                    parser_root=Path('/opt/ros/humble/local/lib/python3.10/dist-packages')
                    parser_hash=lambda name:fact(sdk/parser_root.relative_to('/')/'rosidl_parser'/name)['sha256']
                    # The genuine SDK interpreter resolves its own Lark and
                    # parser dependencies. Bytecode writes remain disabled.
                    native('capture-installed-idl',['/usr/bin/python3','-I','-B',inside(proof_tools/'idl_export.py'),
                        '--request',inside(request),'--out',inside(out/'installed-idl.json'),
                        '--parser-root',str(parser_root),'--parser-sha256',parser_hash('parser.py'),
                        '--definition-sha256',parser_hash('definition.py')])
                    pairing=[]
                    for name,path in(('parent',source/'sdk/origin/tools/custom_types.py'),
                            ('bounded',source/'sdk/origin/custom-types/bounded_profile.py'),
                            ('adapter',source/'sdk/origin/robots/industrial/vendor_profile.py')):
                        pairing+=['--'+name+'-sha256',fact(path)['sha256']]
                    proof=out/'proof-source'
                    run('generate-typed-proof',[sys.executable,'-I','-S','-B',proof_tools/'source_proof.py',
                        '--source-root',source,'--catalog',catalog,'--installed-idl',out/'installed-idl.json',
                        '--out',proof,*pairing])
                    configure=['cmake','-S',inside(proof_tools),'-B',inside(out/'proof-build'),
                        '-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_COMPILER=/usr/bin/g++',
                        '-DUWVM_ROS2_BUILD_INSTALLED_SOURCE=ON','-DUWVM_SOURCE='+inside(source),
                        '-DUWVM_SOURCE_GENERATION='+inside(proof),'-DUWVM_NATIVE_PACKAGE=unitree_api',
                        '-DCMAKE_PREFIX_PATH='+inside(install)+';/opt/ros/humble']
                    native('configure-typed-proof',configure)
                    native('build-typed-proof',['cmake','--build',inside(out/'proof-build'),'--parallel','1',
                        '--target','native-source-identities','native-source-contract'],timeout=900)
                    native('actual-native-source-contract',[inside(out/'proof-build/native-source-contract')])
                    native('owned-native-source-export',['/usr/bin/python3','-I','-S','-B',
                        inside(Path(__file__).with_name('unitree_native_source_export.py')),
                        '--exporter',inside(out/'proof-build/native-source-identities'),
                        '--install',inside(install),'--out',inside(out/'export')])
                    export=json.loads((out/'export/owner.json').read_bytes())
                    assert export['status']=='GENUINE_STOPPED_UNITREE_INSTALLED_SOURCE_EXPORT_PASS'
                    # Binding must challenge the independently executed export
                    # and original installed source proof, including refusals.
                    run('source-proof-negative-contract',[sys.executable,'-I','-S','-B',
                        source/'test/0073.ros-tests/support/source/ros2_compat/installed_source_proof_contract.py',
                        '--source-root',source,'--catalog',catalog,'--metadata',out/'export/metadata.json',
                        '--proof',proof/'native_source_proof.json',*pairing])
                    bound=out/'bound-source'
                    run('bind-original-complete',[sys.executable,'-I','-S','-B',
                        source/'sdk/origin/robots/industrial/prepare.py','--source-root',source,'--catalog',catalog,
                        '--out',bound,'--mode','bind','--native-metadata',out/'export/metadata.json',
                        '--native-owner-report',out/'export/owner.json',
                        '--installed-source-proof',proof/'native_source_proof.json',*pairing])
                    native('configure-bound-contracts',configure+['-DUWVM_BOUND_GENERATION='+inside(bound/'generated')])
                    native('build-bound-contracts',['cmake','--build',inside(out/'proof-build'),'--parallel','1',
                        '--target','native-source-binding','native-source-unproved-binding'],timeout=900)
                    for target in('native-source-binding','native-source-unproved-binding'):
                        native('actual-'+target,[inside(out/'proof-build'/target)])
                    dependencies={}
                    for path in sorted((out/'proof-build').rglob('*.o.d')):
                        for name in shlex.split(path.read_text().replace('\\\n',' ').partition(':')[2]):
                            p=Path(name);actual=root/p.relative_to('/tmp')if p.is_relative_to('/tmp')else sdk/p.relative_to('/')
                            dependencies[str(actual)]=fact(actual)
                    (out/'compiler-dependencies.json').write_text(json.dumps(dependencies,indent=2)+'\n')
                    report.update(status='UNITREE_HUMBLE_COMPLETE_INSTALLED_SOURCE_BINDING_PASS',
                        native_messages=8,native_source_contract_executed=True,
                        production_binding_with_proof=True,production_binding_without_proof_refused=True,
                        original_installed_idl_files=8,compiler_dependencies=len(dependencies),
                        native_source_proof=fact(proof/'native_source_proof.json'),
                        native_metadata=fact(out/'export/metadata.json'),
                        compiled_products=[dict(path=str(p.relative_to(root)),**fact(p))for p in
                            (out/'proof-build'/n for n in('native-source-identities','native-source-contract',
                                'native-source-binding','native-source-unproved-binding'))])
                    assert [dict(path=x['path'],**fact(x['path']))for x in identity]==identity
                    report['native_sdk_tools_after']=identity;save()
                    return
                if a.phase=='peer':
                    original=out/'native-source';original.mkdir()
                    shutil.copyfile(Path(__file__).with_name('unitree_native_api_CMakeLists.txt'),original/'CMakeLists.txt')
                    native('configure-peer',['cmake','-S',inside(original),'-B',inside(out/'build'),
                        '-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_COMPILER=/usr/bin/g++','-DUWVM_SOURCE='+inside(source),
                        '-DCMAKE_PREFIX_PATH='+inside(install)+';/opt/ros/humble'])
                    native('build-peer',['cmake','--build',inside(out/'build'),'--parallel','1'],timeout=900)
                    peer=out/'build/unitree-native-api'
                    dependencies={}
                    for path in sorted((out/'build').rglob('*.o.d')):
                        names=shlex.split(path.read_text().replace('\\\n',' ').partition(':')[2])
                        for name in names:
                            p=Path(name)
                            actual=root/p.relative_to('/tmp')if p.is_relative_to('/tmp')else sdk/p.relative_to('/')
                            assert actual.is_file();dependencies[str(actual)]=fact(actual)
                    assert len(dependencies)>100
                    product=dict(status='ACTUAL_NATIVE_PEER_COMPILE_PASS_NOT_TRANSPORT',artifact=dict(path=str(peer),**fact(peer)),
                        cpp_source=dict(path=str(Path(__file__).with_name('unitree_native_api_peer.cpp')),**fact(Path(__file__).with_name('unitree_native_api_peer.cpp'))),
                        cmake_source=dict(path=str(original/'CMakeLists.txt'),**fact(original/'CMakeLists.txt')),
                        compile_commands=fact(out/'build/compile_commands.json'),dependencies=dependencies,native_sdk_tools=identity)
                    (out/'native-product.json').write_text(json.dumps(product,indent=2)+'\n')
                else:
                    receipt=root/inputs['native_peer_prebuilt'];assert receipt.resolve(strict=True).is_relative_to(root/'out')
                    product=json.loads(receipt.read_bytes());assert product['status']=='ACTUAL_NATIVE_PEER_COMPILE_PASS_NOT_TRANSPORT'
                    peer=Path(product['artifact']['path']);assert fact(peer)=={k:product['artifact'][k]for k in('bytes','sha256')}
                    assert fact(Path(__file__).with_name('unitree_native_api_peer.cpp'))=={k:product['cpp_source'][k]for k in('bytes','sha256')}
                    assert fact(Path(__file__).with_name('unitree_native_api_CMakeLists.txt'))=={k:product['cmake_source'][k]for k in('bytes','sha256')}
                    for path,pin in product['dependencies'].items():assert fact(path)==pin
                    for pin in product['native_sdk_tools']:assert fact(pin['path'])=={k:pin[k]for k in('bytes','sha256')}
                    report['native_compile_reused_from_closed_attempt']=dict(artifact=fact(peer),product_receipt=dict(path=str(receipt),**fact(receipt)),
                        reason='Python-only fixture fix; full compiler dependency closure and artifact are unchanged.')
                vectors=out/'vectors';vectors.mkdir()
                accepted=root/'out/sdk-layout-sport-verify-r6'
                assert json.loads((accepted/'result.json').read_bytes())['status']=='SPORT_REAL_WASM_BUILDERS_AND_DEFAULT_DENIAL_PASS'
                vector_sources=[]
                for language in('rs','as','tinygo','js'):
                    path=accepted/'commands'/('host-js-local'if language=='js'else'Guest-'+language)/'stdout'
                    raw=path.read_bytes();lines=[line for line in raw.splitlines()if b'\t'in line];assert len(lines)==42
                    dest=vectors/(language+'.txt');dest.write_bytes(b'\n'.join(lines)+b'\n')
                    vector_sources.append(dict(language=language,mode='host-only'if language=='js'else'actual-Wasm',original=dict(path=str(path),**fact(path)),staged=fact(dest)))
                report['vectors_from_prior_passing_source_runs']=vector_sources;save()
                native('two-rmw-native-carriers',['/usr/bin/python3','-I','-S','-B',inside(Path(__file__).with_name('unitree_native_api_session.py')),
                    '--out',inside(out/'session'),'--peer',inside(peer),'--install',inside(install),'--vectors',inside(vectors),
                    '--parent-netns',os.readlink('/proc/self/ns/net')],timeout=600)
                session=json.loads((out/'session/result.json').read_bytes())
                assert session['status']=='TWO_RMW_FOUR_SOURCE_NATIVE_CARRIERS_PASS'and session['complete_requests_received']==336
                if a.phase=='session':assert fact(peer)==report['native_compile_reused_from_closed_attempt']['artifact']
                for row in vector_sources:assert fact(row['original']['path'])=={k:row['original'][k]for k in('bytes','sha256')}
                report.update(status='UNITREE_COMPLETE_API_NATIVE_DDS_CARRIERS_PASS',native_requests=336,
                    native_pairs=len(session['pairs']),rmw=['rmw_fastrtps_cpp','rmw_cyclonedds_cpp'],native_peer=dict(path=str(peer),**fact(peer)),
                    session_result=fact(out/'session/result.json'))
            assert [dict(path=x['path'],**fact(x['path']))for x in identity]==identity
            report['native_sdk_tools_after']=identity
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()
    print(report['status'],flush=True)


if __name__=='__main__':main()
