#!/usr/bin/env python3
"""Compile real Lite3 service sources; run pure values and ungranted Wasm only.

Run as one workload of the common native v2 finite owner. Never create fake
imports/native SDKs or reinterpret value/default-denial cases as positive ROS.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
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
        help='Separate canonical DEEP tests; select archived embedded inputs explicitly')
    parser.add_argument('--review',type=Path,required=True)
    parser.add_argument('--wasi',type=Path,required=True)
    parser.add_argument('--runtime',type=Path,required=True)
    parser.add_argument('--validator',type=Path,required=True)
    args=parser.parse_args()
    run=Path(os.environ['UWVM_RESOURCE_RUN']).resolve(strict=True)
    assert os.environ['UWVM_RESOURCE_HEAVY_LOCK_HELD']=='1'
    assert Path('/proc/self/cgroup').read_text().strip().endswith('/'+Path(os.environ['UWVM_RESOURCE_SCOPE']).name)
    out=run/'work/lite3';out.mkdir()
    reference=args.reference.resolve(strict=True);sdk=reference/'sdk';source=args.overlay.resolve(strict=True)
    tests=(args.test_source_root or Path(__file__).resolve().parent).resolve(strict=True)
    wasi=args.wasi.resolve(strict=True);sysroot=wasi/'share/wasi-sysroot'
    report=dict(status='RUNNING',commands=[],inputs=[],products=[],native_value_runs=0,
        Wasm_value_runs=0,default_denial_runs=0,argument_refusal_runs=0,native_ROS_runs=0,
        end_to_end_runs=0,official_suite_runs=0,QEMU_runs=0,VTune_runs=0,default_authority=[],
        complete_platform_support=False,current_public_SDK_qualified=False,
        scope='Selected Lite3 source over frozen genuine C11/C++20 public SDK; values and actual ungranted Wasm only')
    pins={}
    def pin(path):
        path=Path(path).resolve(strict=True)
        if path not in pins:pins[path]=fact(path);report['inputs'].append(pins[path])
        return path
    def save():(out/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    def command(label,argv,expected_exit=0):
        directory=out/'commands'/label;directory.mkdir(parents=True)
        row=dict(label=label,argv=list(map(str,argv)),exit=None);report['commands'].append(row);save()
        with (directory/'stdout').open('xb') as so,(directory/'stderr').open('xb') as se:
            child=subprocess.run(row['argv'],cwd=out,stdin=subprocess.DEVNULL,stdout=so,stderr=se,timeout=180)
        row.update(exit=child.returncode,stdout=fact(directory/'stdout'),stderr=fact(directory/'stderr'));save()
        if child.returncode!=expected_exit:raise RuntimeError((label,child.returncode,(directory/'stderr').read_text()[-4000:]))
        return (directory/'stdout').read_bytes()
    try:
        pin(__file__);pin(args.review);pin(sdk/'share/distribution-manifest.json');pin(sdk/'share/abi/abi-v1.json')
        pin(sdk/'tools/check_wasm_imports.py')
        manifest={row['path']:row for row in json.loads((sdk/'share/distribution-manifest.json').read_bytes())['entries']}
        for base in (sdk/'include',reference/'fast_io'):
            for path in sorted(base.rglob('*')):
                if not path.is_file():continue
                name=path.relative_to(base).as_posix()
                if name=='license.txt' and base==reference/'fast_io':continue
                actual=fact(path);expected=manifest['include/'+name]
                assert all(actual[key]==expected[key] for key in ('sha256','bytes')),(path,actual,expected)
                pin(path)
        for base in (source/'include',source/'examples',tests):
            for path in sorted(base.rglob('*')):
                if path.is_file():pin(path)
        original=source/'upstream/deep/main/files/src/lite3_transfer/interface/robot/udp_interface.hpp'
        body=pin(original).read_text()
        assert 'command == 0' in body and 'command > 200' in body and '1000 % command != 0' in body
        assert 'response->result = -1' in body and 'response->result = 0' in body
        native_client=source/'upstream/deep/main/files/src/lite3_sdk_service/service/lite3_sdk_service.hpp'
        body=pin(native_client).read_text();stop=body.index('bool SendEmergencyStop()')
        assert 'request->command = 0;' in body[stop:]
        for tool in (Path('/usr/bin/clang-21'),wasi/'bin/clang',wasi/'bin/clang++',args.runtime,args.validator):pin(tool)
        command('native-compiler',['/usr/bin/clang-21','--version'])
        command('wasi-compiler',[wasi/'bin/clang++','--version'])
        includes=['-I'+str(source/'include'),'-I'+str(reference/'fast_io'),'-I'+str(sdk/'include'),
            '-I'+str(sdk/'include/platform/ros2/c'),'-I'+str(sdk/'include/platform/ros/c'),
            '-I'+str(sdk/'include/platform/deep_robotics/c/ros2/deep-robotics-ros2-main-complete')]
        products=[]
        for name,entry,kind in (('native-values',tests/'lite3_service_values.c','native'),
                ('wasm-values',tests/'lite3_service_values.c','wasm'),
                ('wasm-service',source/'examples/lite3_service_once.cpp','wasm')):
            is_cpp=entry.suffix=='.cpp';tool=wasi/'bin/clang++' if is_cpp else wasi/'bin/clang' if kind=='wasm' else Path('/usr/bin/clang-21')
            product=out/(name+('.wasm' if kind=='wasm' else ''));dep=out/(name+'.d')
            target=[] if kind=='native' else ['--target=wasm32-wasip1','--sysroot='+str(sysroot),
                '-Wl,--threads=1,-z,stack-size=1048576,--initial-memory=16777216,--max-memory=67108864']
            extra=['-fno-exceptions','-fno-rtti'] if is_cpp else []
            command('build-'+name,[tool,*target,'-std=c++20' if is_cpp else '-std=c11','-O1','-g0',
                '-UNDEBUG',*extra,*includes,'-MD','-MF',dep,entry,'-o',product])
            for value in shlex.split(dep.read_text().replace('\\\n',' ').partition(':')[2]):pin(value)
            products.append((name,product));report['products'].append(dict(name=name,artifact=fact(product)))
        for name,path in products:
            if not path.suffix=='.wasm':continue
            command('validate-'+name,[args.validator,'validate',path])
            argv=['/usr/bin/python3','-I','-S','-B']
            argv+=([args.review,path,'--checker',sdk/'tools/check_wasm_imports.py'] if name=='wasm-values' else
                [sdk/'tools/check_wasm_imports.py',path,'--backend','2','--allow-module','wasi_snapshot_preview1'])
            command('imports-'+name,[*argv,'--report',out/(name+'-imports.json')])
        expected=b'LITE3_SERVICE_VALUES_OK rates=14 invalid=9 native_result_bits=32\n'
        assert command('run-native-values',[products[0][1]])==expected;report['native_value_runs']=1
        prefix=[args.runtime,'-Rint','--wasip1-global-noinherit-system-environment','--run']
        assert command('run-wasm-values',[*prefix,products[1][1]])==expected;report['Wasm_value_runs']=1
        entry=products[2][1]
        for rate in (0,1,2,4,5,8,10,20,25,40,50,100,125,200):
            assert command('denied-mode-'+str(rate),[*prefix,entry,'sdk-mode','task05.lite3.mode',str(rate)],13)== \
                b'DONE transport=13 native_valid=0 native_result=0 cleanup=0\n'
            report['default_denial_runs']+=1
        assert command('denied-emergency-stop', [*prefix,entry,'emergency-stop','task05.lite3.stop'],13)== \
            b'DONE transport=13 native_valid=0 native_result=0 cleanup=0\n';report['default_denial_runs']+=1
        for value in ('-1','3','7','199','201','250','1000','-2147483648','2147483647','2147483648'):
            assert command('invalid-mode-'+value,[*prefix,entry,'sdk-mode','task05.lite3.mode',value],22)==b''
            report['argument_refusal_runs']+=1
        assert all(fact(path)==row for path,row in pins.items())
        report.update(status='PASS',input_pins_preserved=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
