#!/usr/bin/env python3
"""Actual native-value/Wasm payload checks and roundtrip Guest default refusal.

Use the shared native v2 owner. Borrow an immutable selected public header
closure and actual staged WASI SDK. This does not test current C++ macro edits,
positive Gateway/native ROS transport, firmware, hardware, QEMU or VTune.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import struct
import subprocess


def fact(path):
    path=Path(path)
    with path.open('rb') as stream:digest=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=digest)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--reference',type=Path,required=True)
    parser.add_argument('--overlay',type=Path,required=True)
    parser.add_argument('--test-source-root',type=Path,
        help='Separate canonical Unitree tests; an archived embedded test folder must be selected explicitly')
    parser.add_argument('--wasi',type=Path,required=True)
    parser.add_argument('--runtime',type=Path,required=True)
    parser.add_argument('--validator',type=Path,required=True)
    parser.add_argument('--cpp-profile',type=Path,required=True,
        help='Exact current task-01 source profile header; isolated platform-facade overlay only')
    parser.add_argument('--cpp-assertion-candidate',type=Path,required=True,
        help='Read-only task-01 bounded_profile.py candidate which supplies the exact legacy assertion spelling')
    args=parser.parse_args()
    run=Path(os.environ['UWVM_RESOURCE_RUN']).resolve(strict=True)
    assert os.environ['UWVM_RESOURCE_HEAVY_LOCK_HELD']=='1'
    assert Path('/proc/self/cgroup').read_text().strip().endswith('/'+Path(os.environ['UWVM_RESOURCE_SCOPE']).name)
    out=run/'work/payload';out.mkdir()
    reference=args.reference.resolve(strict=True);sdk=reference/'sdk';overlay=args.overlay.resolve(strict=True)
    tests=(args.test_source_root or Path(__file__).resolve().parent).resolve(strict=True)
    wasi=args.wasi.resolve(strict=True);sysroot=wasi/'share/wasi-sysroot'
    expected={row['path']:row for row in json.loads((sdk/'share/distribution-manifest.json').read_bytes())['entries']}
    report=dict(status='RUNNING',commands=[],inputs=[],products=[],native_value_runs=0,guest_runs=0,host_python_runs=0,
        default_authority=[],native_ROS_runs=0,end_to_end_runs=0,official_upstream_runs=0,QEMU_runs=0,VTune_runs=0,
        current_public_SDK_qualified=False,current_Runtime_source_rebuilt=False,
        Cpp_macro_revisions_qualified=False,positive_transport_qualified=False,
        scope='current platform source over exact frozen public C11/C++20 headers; local payload and actual ungranted Guest only')
    pins={}
    def pin(path):
        path=Path(path).resolve(strict=True)
        if path not in pins:pins[path]=fact(path);report['inputs'].append(pins[path])
        return path
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def command(label,argv,expected_exit=0,cwd=out):
        directory=out/'commands'/label;directory.mkdir(parents=True)
        row=dict(label=label,argv=list(map(str,argv)),cwd=str(cwd),exit=None);report['commands'].append(row);save()
        with (directory/'stdout').open('xb') as so,(directory/'stderr').open('xb') as se:
            child=subprocess.run(row['argv'],cwd=cwd,stdin=subprocess.DEVNULL,stdout=so,stderr=se,timeout=180)
        row.update(exit=child.returncode,stdout=fact(directory/'stdout'),stderr=fact(directory/'stderr'));save()
        if child.returncode!=expected_exit:raise RuntimeError((label,child.returncode,(directory/'stderr').read_text()[-5000:]))
        return (directory/'stdout').read_bytes()
    try:
        pin(__file__);pin(sdk/'share/distribution-manifest.json')
        pin(sdk/'share/abi/abi-v1.json');pin(sdk/'tools/check_wasm_imports.py')
        profile=pin(args.cpp_profile)
        profile_include=out/'profile/platform/ros/cpp/roswasm';profile_include.mkdir(parents=True)
        (profile_include/'cpp_profile.hpp').write_bytes(profile.read_bytes())
        for header in sorted((overlay/'include/roswasm_unitree').iterdir()):
            if header.suffix not in ('.h','.hpp','.inc') or not header.is_file():continue
            language='cpp' if header.suffix=='.hpp' else 'c'
            target=out/'profile/platform/unitree'/language/'roswasm_unitree'/header.name
            target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(pin(header),target)
        report['Cpp_profile_header_overlay']=fact(profile)
        report['Cpp_profile_scope']='Only this platform facade consumes the current selector header; full shared C++ SDK remains unqualified'
        candidate=pin(args.cpp_assertion_candidate)
        original_c=sdk/'include/platform/unitree/c/ros2/unitree-api-complete/bounded_records.h'
        before=pin(original_c).read_bytes()
        old=b'#if defined(__cplusplus)\n#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")'
        new=b'#if defined(__cplusplus) && __cplusplus >= 201103L\n#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")\n#elif defined(__cplusplus)\n#define UWVM_BOUNDED_ASSERT(x) typedef char uwvm_bounded_representation_required[(x)?1:-1]'
        assert before.count(old)==1
        candidate_text=candidate.read_text()
        assert all(repr(line) in candidate_text for line in new.decode().splitlines())
        compatible=out/'profile/bounded_records.h';compatible.write_bytes(before.replace(old,new))
        report['Cpp_C_record_compat_candidate']=dict(source_generator=fact(candidate),before=fact(original_c),
            after=fact(compatible),scope='Exact task-01 C++ assertion spelling only; original fields, bounds, codec and C branch remain unchanged')
        for folder in (sdk/'include',reference/'fast_io'):
            for path in sorted(folder.rglob('*')):
                if not path.is_file():continue
                relative=path.relative_to(folder).as_posix()
                key='include/'+relative
                if relative=='license.txt' and folder==reference/'fast_io':continue
                row=expected[key];actual=fact(path)
                assert all(actual[key]==row[key] for key in ('bytes','sha256')),(str(path),row,actual)
                pin(path)
        for path in sorted(overlay.rglob('*')):
            if path.is_file():pin(path)
        for path in sorted(tests.rglob('*')):
            if path.is_file():pin(path)
        for path in (wasi/'bin/clang',wasi/'bin/clang++',Path('/usr/bin/clang-21'),Path('/usr/bin/clang++-21'),
                Path('/usr/bin/python3'),args.runtime,args.validator):pin(path)
        command('host-python-version',['/usr/bin/python3','-I','-S','-B','--version'])
        for label,tool in (('native-compiler','/usr/bin/clang++-21'),('wasi-compiler',wasi/'bin/clang++')):
            command(label,[tool,'--version'])
        includes=['-I'+str(out/'profile'),'-I'+str(reference/'fast_io'),'-I'+str(sdk/'include')]
        for language in ('c','cpp'):
            includes+=['-I'+str(sdk/'include/platform/ros2'/language),'-I'+str(sdk/'include/platform/ros'/language),
                '-I'+str(sdk/'include/platform/unitree'/language),
                '-I'+str(sdk/'include/platform/unitree'/language/'ros2/unitree-api-complete')]
        native=[];wasm=[]
        modes=[(98,'c++98'),(3,'c++03'),(11,'c++11'),(14,'c++14'),(17,'c++17'),(20,'c++20'),(23,'c++23'),(26,'c++2c')]
        for kind in ('native','wasm'):
            destination=out/kind;destination.mkdir()
            cxx=Path('/usr/bin/clang++-21') if kind=='native' else wasi/'bin/clang++'
            cc=Path('/usr/bin/clang-21') if kind=='native' else wasi/'bin/clang'
            target=[] if kind=='native' else ['--target=wasm32-wasip1','--sysroot='+str(sysroot)]
            link=[] if kind=='native' else ['-Wl,--threads=1,-z,stack-size=1048576,--initial-memory=16777216,--max-memory=67108864']
            common=[*target,'-O1','-g0',*includes]
            sources=[('b2',overlay/'b2_sport.cpp',True),('c-values',tests/'b2_sport_values.c',False),
                ('cpp-values',tests/'b2_sport_values.cpp',True)]
            if kind=='wasm':sources += [('go2',overlay/'sport.cpp',True),('roundtrip',overlay/'examples/sport_roundtrip.cpp',True)]
            objects={}
            for name,source,is_cpp in sources:
                obj=destination/(name+'.o');dep=destination/(name+'.d')
                argv=[cxx if is_cpp else cc,*common,'-std=c++20' if is_cpp else '-std=c11']
                if is_cpp:argv+=['-fno-exceptions','-fno-rtti']
                argv+=['-MD','-MF',dep,'-c',source,'-o',obj]
                command(kind+'-compile-'+name,argv);objects[name]=obj
                for value in shlex.split(dep.read_text().replace('\\\n',' ').partition(':')[2]):pin(value)
            for name in ('c-values','cpp-values'):
                product=destination/(name+('.wasm' if kind=='wasm' else ''))
                command(kind+'-link-'+name,[cxx,*target,*link,'-fno-exceptions','-fno-rtti',objects[name],objects['b2'],'-o',product])
                (wasm if kind=='wasm' else native).append((name,product))
                report['products'].append(dict(layer=kind,role=name,artifact=fact(product)))
            if kind=='wasm':
                product=destination/'roundtrip.wasm'
                command('wasm-link-roundtrip',[cxx,*target,*link,'-fno-exceptions','-fno-rtti',objects['roundtrip'],objects['go2'],'-o',product])
                wasm.append(('roundtrip',product));report['products'].append(dict(layer='wasm',role='roundtrip',artifact=fact(product)))
            for standard,mode in modes:
                name='cpp-profile-'+str(standard);product=destination/(name+('.wasm' if kind=='wasm' else ''));dep=destination/(name+'.d')
                command(kind+'-build-'+name,[cxx,*common,*link,'-std='+mode,'-pedantic-errors',
                    '-DROSWASM_CPP_STANDARD='+str(standard),'-fno-exceptions','-fno-rtti','-MD','-MF',dep,
                    tests/'b2_sport_cpp_profiles.cpp',objects['b2'],'-o',product])
                for value in shlex.split(dep.read_text().replace('\\\n',' ').partition(':')[2]):pin(value)
                (wasm if kind=='wasm' else native).append((name,product))
                report['products'].append(dict(layer=kind,role=name,compiler_mode=mode,
                    selected_standard=standard,draft=standard==26,artifact=fact(product)))
        for name,path in wasm:
            command('validate-'+name,[args.validator,'validate',path])
            if name=='roundtrip':
                argv=['/usr/bin/python3','-I','-S','-B',sdk/'tools/check_wasm_imports.py',path,
                    '--backend','2','--allow-module','wasi_snapshot_preview1']
            else:
                argv=['/usr/bin/python3','-I','-S','-B',tests/'review_local_imports.py',path,
                    '--checker',sdk/'tools/check_wasm_imports.py']
            command('imports-'+name,[*argv,'--report',out/(name+'-imports.json')])
        # Independent Request oracle retains every original API field, not just
        # the Sport parameter projection. Native and actual Wasm must agree.
        def decode(raw):
            assert len(raw)>=37
            id,api,lease,priority,noreply=struct.unpack_from('<qqqiB',raw)
            at=29;count=struct.unpack_from('<I',raw,at)[0];at+=4;parameter=raw[at:at+count].decode();at+=count
            count=struct.unpack_from('<I',raw,at)[0];at+=4;binary=raw[at:at+count];at+=count
            assert at==len(raw) and (id,lease,priority,noreply,binary)==(9007199254740993,-9007199254740993,-101,1,b'\0\xff\1\x80')
            return api,parameter
        table=(overlay/'include/roswasm_unitree/b2_sport_operations.inc').read_text()
        inventory={name:(int(api),kind) for name,api,kind in re.findall(r'^UWVM_UNITREE_B2\((\w+),(\d+),(\w+)\)',table,re.M)}
        assert len(inventory)==21
        original=overlay/'upstream/example-review-r1/unitree_ros2/files/example/src'
        header=(original/'include/common/ros2_b2_sport_client.h').read_text()
        implementation=(original/'src/common/ros2_b2_sport_client.cpp').read_text()
        constants={name:int(number) for name,number in re.findall(r'const int32_t\s+(\w+)\s*=\s*(\d+)\s*;',header)}
        functions=list(re.finditer(r'(?:^|\n)\s*void SportClient::(\w+)\(([^)]*)\)\s*\{',implementation))
        original_methods={}
        for index,match in enumerate(functions):
            body=implementation[match.end():functions[index+1].start() if index+1<len(functions) else len(implementation)]
            assignments=re.findall(r'req\.header\.identity\.api_id\s*=\s*(\w+)\s*;',body)
            assert len(assignments)==1
            original_methods[match[1]]=dict(api=constants[assignments[0]],keys=set(re.findall(r'\bjs\["([^"]+)"\]',body)))
        assert set(original_methods)==set(inventory)
        assert all(original_methods[name]['api']==api for name,(api,kind) in inventory.items())
        def vectors(body):
            rows={}
            for line in body.splitlines():
                if b'\t' not in line:continue
                label,hexadecimal=line.split(b'\t');name=label.decode();assert name not in rows
                raw=bytes.fromhex(hexadecimal.decode());api,parameter=decode(raw);wanted,kind=inventory[name];assert api==wanted
                if kind=='none':assert parameter=='keep'
                else:
                    value=json.loads(parameter,parse_int=lambda text:-0.0 if text=='-0' else int(text))
                    if kind!='trajectory':assert set(value)==original_methods[name]['keys']
                    if kind=='xyz':
                        assert value==dict(x=1.25,y=-0.0,z=0.5) and struct.pack('<f',value['y'])==struct.pack('<f',-0.0)
                    elif kind=='position':
                        assert value==dict(x=1.25,y=-0.0,yaw=0.5) and struct.pack('<f',value['y'])==struct.pack('<f',-0.0)
                    elif kind=='integer':assert value==dict(data=-2147483648)
                    elif kind=='height':assert value==dict(data=-0.125)
                    elif kind=='boolean':assert value==dict(data=True) and type(value['data']) is bool
                    else:
                        assert kind=='trajectory' and type(value) is list and len(value)==30
                        for i,point in enumerate(value):
                            assert point==dict(t_from_start=i,x=0,y=-0.0,yaw=0,vx=0,vy=0,vyaw=0)
                            assert struct.pack('<f',point['y'])==struct.pack('<f',-0.0)
                rows[name]=raw
            assert set(rows)==set(inventory) and b'B2_SPORT_LOCAL_OK operations=21 negative_cases=5\n' in body
            return rows
        def semantic(rows):
            result={}
            for name,raw in rows.items():
                api,parameter=decode(raw)
                result[name]=(api,parameter if inventory[name][1]=='none' else
                    json.loads(parameter,parse_int=lambda text:-0.0 if text=='-0' else int(text)))
            return result
        reference_vectors=None
        for kind,products in (('native',native),('wasm',wasm)):
            for name,path in products:
                if name=='roundtrip':continue
                argv=[path] if kind=='native' else [args.runtime,'-Rint','--wasip1-global-noinherit-system-environment','--run',path]
                body=command(kind+'-run-'+name,argv)
                if name=='c-values':
                    actual=vectors(body)
                    if reference_vectors is None:reference_vectors=actual
                    else:assert actual==reference_vectors
                elif name=='cpp-values':assert body==b'B2_CPP_NAMES_OK operations=21\n'
                else:assert body==('B2_CPP_PROFILE_OK standard='+name.removeprefix('cpp-profile-')+' operations=21\n').encode()
                report['native_value_runs' if kind=='native' else 'guest_runs']+=1
        body=command('host-python-b2',['/usr/bin/python3','-I','-S','-B',tests/'b2_sport_python.py',sdk,overlay])
        assert semantic(vectors(body))==semantic(reference_vectors)
        report['host_python_runs']=1
        roundtrip=out/'wasm/roundtrip.wasm'
        go2=(overlay/'include/roswasm_unitree/sport_operations.inc').read_text()
        operations=re.findall(r'^UWVM_UNITREE_SPORT\((\w+),(\d+),(\w+)\)',go2,re.M);assert len(operations)==39
        for name,api,kind in operations:
            values=['0.125','-0.25','0.5'] if kind=='xyz' else ['1'] if kind in ('integer','boolean') else []
            body=command('denied-'+name,[args.runtime,'-Rint','--wasip1-global-noinherit-system-environment','--run',roundtrip,
                name,'task05.request','task05.response','0','10','2',*values],expected_exit=13)
            assert body==b'DONE primary=13 subscription=0 publisher=0\n';report['guest_runs']+=1
        assert all(fact(path)==row for path,row in pins.items())
        report.update(status='PASS',input_pins_preserved=True,B2_method_vectors=21,Go2_default_denials=39,
            Wasm_products=len(wasm),native_products=len(native),
            qualified_platform_Cpp_facade_modes=[dict(standard=standard,compiler_mode=mode,draft=standard==26) for standard,mode in modes])
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
