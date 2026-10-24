#!/usr/bin/env python3
"""Qualify default-OFF Unitree CMake options and their actual WASI products."""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    a=p.parse_args();root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    assert out.parent==root/'out'and not out.exists();out.mkdir(mode=0o700);(out/'commands').mkdir()
    report=dict(status='RUNNING',commands=[],artifacts=[],default_authority=[],runtime_rebuilt=False,
                official_suite_cases_run=0,native_controller_or_transport_qualified=False)
    def pin(path):
        path=Path(path);return dict(path=str(path),bytes=path.stat().st_size,sha256=hashlib.sha256(path.read_bytes()).hexdigest())
    def save():(out/'unitreesportbuild.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0,contains=None):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),expected_exit=expected,exit=None)
        report['commands'].append(row);save()
        with(directory/'stdout').open('xb')as stdout,(directory/'stderr').open('xb')as stderr:
            process=subprocess.run(row['argv'],cwd=source,env=dict(os.environ,MAKEFLAGS='-j1',CMAKE_BUILD_PARALLEL_LEVEL='1'),
                stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr,timeout=240,check=False)
        row.update(exit=process.returncode,stdout=pin(directory/'stdout'),stderr=pin(directory/'stderr'));save()
        text=(directory/'stdout').read_bytes();error=(directory/'stderr').read_bytes()
        assert (process.returncode==0 if expected==0 else process.returncode!=0),(label,error[-2800:])
        if contains is not None:assert contains.encode()in text+error,(label,text[-1000:],error[-1000:])
        return text
    try:
        kit=source/'sdk/origin/robots/unitree';family=source/'sdk/origin/custom-types/unitree-api-complete'
        wasi=Path(inputs['toolchains'])/'wasi-sdk-33.0-x86_64-linux'
        fastio=Path(inputs['runtime_source_root'])/'third-parties/fast_io/include'
        native=['-DCMAKE_C_COMPILER=/usr/bin/clang-21','-DCMAKE_CXX_COMPILER=/usr/bin/clang++-21']
        base=['/usr/bin/cmake','-S',kit]
        run('default-off-configure',[*base,'-B',out/'default',*native])
        help_=run('default-off-targets',['/usr/bin/cmake','--build',out/'default','--target','help','--parallel','1'])
        assert b'unitree_sport'not in help_
        for label,flags,reason in[
            ('example-without-library',['-DUWVM_UNITREE_BUILD_SPORT_EXAMPLE=ON'],'Explicit UWVM_UNITREE_BUILD_SPORT=ON'),
            ('contracts-without-library',['-DUWVM_UNITREE_BUILD_SPORT_CONTRACTS=ON'],'Explicit UWVM_UNITREE_BUILD_SPORT=ON'),
            ('library-without-inputs',['-DUWVM_UNITREE_BUILD_SPORT=ON'],'Explicit absolute source')]:
            run(label,[*base,'-B',out/label,*native,*flags],1,reason)
        selected=['-DUWVM_UNITREE_BUILD_SPORT=ON','-DUWVM_UNITREE_SOURCE_ROOT='+str(source),
            '-DUWVM_UNITREE_FAST_IO='+str(fastio)]
        run('foreign-family',[*base,'-B',out/'foreign',*native,*selected,
            '-DUWVM_UNITREE_API_GENERATED='+str(source/'sdk/origin/custom-types/unitree-go-complete')],1,
            'Exact complete Unitree ROS2 API source association required')
        run('native-example-refused',[*base,'-B',out/'native-example',*native,*selected,
            '-DUWVM_UNITREE_API_GENERATED='+str(family),'-DUWVM_UNITREE_BUILD_SPORT_EXAMPLE=ON'],1,
            'actual WASI wasm32 toolchain is required')
        # A single explicit family must be intact even when only the library is
        # requested. A self-consistent foreign family is not a substitute.
        build=out/'wasi'
        run('wasi-configure',[*base,'-B',build,*selected,
            '-DCMAKE_BUILD_TYPE=MinSizeRel','-DCMAKE_TOOLCHAIN_FILE='+str(wasi/'share/cmake/wasi-sdk.cmake'),
            '-DCMAKE_EXE_LINKER_FLAGS=-Wl,--threads=1,-z,stack-size=1048576,--initial-memory=16777216,--max-memory=67108864',
            '-DUWVM_UNITREE_API_GENERATED='+str(family),'-DUWVM_UNITREE_BUILD_SPORT_EXAMPLE=ON',
            '-DUWVM_UNITREE_BUILD_SPORT_CONTRACTS=ON'])
        run('wasi-build',['/usr/bin/cmake','--build',build,'--parallel','1'])
        spec=importlib.util.spec_from_file_location('unitree_original_sport_oracle',Path(__file__).with_name('unitreesport_remaining.py'))
        oracle=importlib.util.module_from_spec(spec);spec.loader.exec_module(oracle)
        original=root/inputs['unitree_source_capture']/'unitree_ros2/files/example/src'
        operations=oracle.original_operations((original/'include/common/ros2_sport_client.h').read_text(),
            (original/'src/common/ros2_sport_client.cpp').read_text())
        table=__import__('re').findall(r'^UWVM_UNITREE_SPORT\((\w+),(\d+),(\w+)\)$',
            (kit/'include/roswasm_unitree/sport_operations.inc').read_text(),__import__('re').M)
        ordered=[(name,operations[name])for name,_,_ in table]
        prefix=[inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run']
        for name in('unitree_sport_values_c','unitree_sport_values_cpp','unitree_sport_operations'):
            product=build/(name+'.wasm');assert product.is_file()
            run(name+'-validate',[inputs['validator'],'validate',product])
            report['artifacts'].append(pin(product))
            if name!='unitree_sport_operations':
                raw=run(name+'-values',prefix+[product]);oracle.check_frames(raw,ordered)
            else:
                run(name+'-imports',[sys.executable,'-I','-S','-B',source/'sdk/origin/tools/check_wasm_imports.py',product,
                    '--backend','2','--allow-module','wasi_snapshot_preview1','--report',out/'imports.json'])
                for method,args in[('Damp',[]),('Move',['0.25','-0.125','0.0625']),('SpeedLevel',['-7']),
                                   ('Pose',['1']),('AutoRecoveryGet',[])]:
                    raw=run('default-denied-'+method,prefix+[product,'send',method,'unitree.sport.request','41',*args],1)
                    last=report['commands'][-1]
                    assert last['exit']==13 and not raw and last['stderr']['bytes']==0
        report['artifacts'].append(pin(build/'libroswasm_unitree_sport.a'))
        report.update(status='DEFAULT_OFF_AND_EXPLICIT_COMPLETE_UNITREE_SPORT_CMAKE_WASI_PRODUCTS_PASS',
            all_sdk_switches_default_off=True,negative_configuration_cases=5,
            complete_values_checked=156,actual_default_denials=5,
            make_jobs=1,linker_threads=1)
        print(report['status'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
