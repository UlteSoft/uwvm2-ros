#!/usr/bin/env python3
"""Compile the actual original Rust scalar reactor and execute its zero-right graph.

This inner-artifact qualification uses real retained tools and production binary
inspection/probe generation. It does not manufacture an installed SDK or claim
public xmake/installation acceptance. Run only under the original finite owner.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import tarfile


def digest(path):
    with path.open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--sha256',required=True)
    args=parser.parse_args()
    if digest(args.source)!=args.sha256:raise ValueError('Captured current reactor source changed')
    work=Path.cwd();source=work/'source';source.mkdir()
    with tarfile.open(args.source) as archive:
        members=archive.getmembers()
        if sum(row.size for row in members)>256*1024 or any(
                not row.isfile() or Path(row.name).is_absolute() or '..' in Path(row.name).parts for row in members):
            raise ValueError('Bounded ordinary current source capsule required')
        archive.extractall(source,filter='data')
    tools=source/'sdk/origin/tools'
    for path in tools.rglob('*.py'):compile(path.read_bytes(),str(path),'exec',dont_inherit=True)
    sys.path.insert(0,str(tools))
    import check_wasm_imports
    from native_languages import reactors
    agents=Path('/home/macromodel/Documents/uwvm-test-261018/agents')
    rust=agents/'03/20261021-toolchains-r1/work/toolchains/rust'
    wasi=agents/'10/staged-wasi-r1/staged/wasi-sdk'
    programs={'rustc':rust/'bin/rustc','linker':wasi/'bin/wasm-ld',
              'validator':Path('/usr/bin/wasm-validate'),'wat2wasm':Path('/usr/bin/wat2wasm'),
              'runtime':agents/'10/runtime-denial-r2/work/uwvm-dual'}
    if digest(programs['runtime'])!='0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556':
        raise ValueError('Retained actual Runtime changed')
    target=rust/'lib/rustlib/wasm32-unknown-unknown/lib'
    for stem in ('libcore-','libcompiler_builtins-'):
        if len(list(target.glob(stem+'*.rlib')))!=1:raise ValueError('Actual matching original Rust target libraries required')
    inputs=[path for path in source.rglob('*') if path.is_file()]
    inputs+=list(programs.values())+[path for path in target.rglob('*') if path.is_file()]
    inputs+=[path for path in (rust/'lib').glob('*.so*') if path.is_file()]
    inputs+=[path for path in (wasi/'lib').glob('*.so*') if path.is_file()]
    pins={str(path.resolve(strict=True)):{'bytes':path.stat().st_size,'sha256':digest(path)} for path in inputs}
    rows=[]
    report={'kind':'task03-original-Rust-scalar-reactor-inner-artifact-v1','status':'RUNNING',
        'source_capsule_sha256':args.sha256,'source_and_tool_pins':pins,'commands':rows,
        'public_target_families':['wasm32-wasip1','wasm64-wasip1'],
        'internal_compiler_target':'wasm32-unknown-unknown','artifact_WASI_import_ABI':'none',
        'public_SDK_assembly':False,'installed_SDK_builds':0,'xmake_builds':0,
        'native_ROS_runs':0,'default_authority':[],'actual_binary_observations':{}}
    def save():
        (work/'rust-reactor.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,layer,command,success=True):
        log=work/(name+'.log')
        row={'name':name,'layer':layer,'argv':[str(value) for value in command],'log':log.name}
        rows.append(row);save()
        with log.open('wb') as stream:
            result=subprocess.run(row['argv'],cwd=work,stdout=stream,stderr=subprocess.STDOUT,timeout=180)
        row.update(exit=result.returncode,log_sha256=digest(log),
                   status='PASS' if (result.returncode==0)==success else 'FAIL')
        save()
        if row['status']!='PASS':raise ValueError('Retained actual '+name+' failure')
        return log.read_text(errors='replace')
    try:
        for name,path in programs.items():
            # Native loader dependency observations are recorded separately;
            # they are neither target-library nor Guest execution credit.
            output=work/('loader-'+name+'.log')
            with output.open('wb') as stream:
                result=subprocess.run(['/usr/bin/ldd',str(path)],stdout=stream,stderr=subprocess.STDOUT,timeout=30)
            text=output.read_text(errors='replace')
            if result.returncode and not any(word in text for word in ('not a dynamic executable','statically linked')):
                raise ValueError('Actual tool loader dependency observation failed')
            rows.append({'name':'loader-'+name,'layer':'native_tool_dependency_observation',
                         'argv':['/usr/bin/ldd',str(path)],'exit':result.returncode,
                         'log':output.name,'log_sha256':digest(output),'status':'PASS_OBSERVATION'})
            for spelling in re.findall(r'(/[^\s()]+)\s+\(0x',text):
                dependency=Path(spelling).resolve(strict=True)
                pins[str(dependency)]={'bytes':dependency.stat().st_size,'sha256':digest(dependency)}
        version=run('rustc-version','actual_compiler_version',[programs['rustc'],'--version','--verbose'])
        if not version.startswith('rustc 1.85.0 '):raise ValueError('Actual Rust1.85.0 required')
        selected=run('target-library','actual_compiler_target_library_query',[programs['rustc'],
            '--sysroot',rust,'--target=wasm32-unknown-unknown','--print','target-libdir']).strip()
        if Path(selected).resolve(strict=True)!=target:raise ValueError('Actual target library fallback detected')
        artifact=work/'vision-rust.wasm'
        entry=source/'sdk/origin/examples/robotics_modular/vision.rs'
        run('rust-vision','actual_Rust_compiler_and_linker',[programs['rustc'],'--edition=2021',
            '--crate-type=cdylib','--target=wasm32-unknown-unknown','--sysroot',rust,
            '-C','panic=abort','-C','opt-level=1','-C','codegen-units=1',
            '-C','linker='+str(programs['linker']),'-C','link-arg=--no-entry',
            '-C','link-arg=--initial-memory=131072','-C','link-arg=--max-memory=131072',
            '-C','link-arg=-z','-C','link-arg=stack-size=65536',entry,'-o',artifact])
        run('vision-validate','actual_independent_full_Wasm_validator',[programs['validator'],'--enable-all',artifact])
        report['actual_binary_observations']['vision']=reactors.inspect_scalar(
            artifact.read_bytes(),'rust-primary',check_wasm_imports.Reader)
        wat,count=reactors.probe_source(False)
        caller=work/'probe.wat';caller.write_text(wat)
        probe=work/'probe.wasm'
        run('probe-compile','actual_WAT_to_Wasm_compiler',[programs['wat2wasm'],caller,'-o',probe])
        run('probe-validate','actual_independent_full_Wasm_validator',[programs['validator'],'--enable-all',probe])
        report['actual_binary_observations']['probe']=reactors.inspect_scalar(
            probe.read_bytes(),'probe',check_wasm_imports.Reader)
        runtime=programs['runtime']
        def graph(main):
            return [runtime,'--wasm-preload-library',artifact,'vision',
                    '--wasm-set-main-module-name','native-language-vision-probe','--run',main]
        run('scalar-Guest','actual_empty_authority_composed_Guest',graph(probe))
        prefix,suffix=wat.rsplit('i32.const '+str(reactors.INVALID),1)
        wrong=work/'wrong-last-value.wat';wrong.write_text(prefix+'i32.const 0'+suffix)
        negative=work/'wrong-last-value.wasm'
        run('negative-probe-compile','actual_WAT_to_Wasm_compiler',[programs['wat2wasm'],wrong,'-o',negative])
        run('negative-probe-validate','actual_independent_full_Wasm_validator',[programs['validator'],'--enable-all',negative])
        report['actual_binary_observations']['negative_probe']=reactors.inspect_scalar(
            negative.read_bytes(),'probe',check_wasm_imports.Reader)
        trapped=run('wrong-value-Guest','actual_Guest_last_assertion_trap_control',graph(negative),success=False)
        if 'unreachable' not in trapped.lower():
            raise ValueError('Negative Guest must report the actual assertion unreachable trap, not an unrelated launch failure')
        for spelling,pin in pins.items():
            path=Path(spelling)
            if path.stat().st_size!=pin['bytes'] or digest(path)!=pin['sha256']:
                raise ValueError('Current source/tool/target dependency changed')
        report.update(status='PASS_ACTUAL_RUST_INNER_REACTOR_AND_ZERO_RIGHT_SCALAR_GRAPH',
            scalar_vectors=count,Rust_compilations=1,full_Wasm_validations=3,
            positive_Guest_runs=1,negative_Guest_trap_controls=1,
            inputs_unchanged=True,artifact_pins={path.name:{'bytes':path.stat().st_size,'sha256':digest(path)}
                for path in (artifact,probe,negative)},installed_public_SDK_acceptance=False)
        save();return 0
    except Exception:
        report['status']='FAILED_RETAINED';save();raise


if __name__=='__main__':raise SystemExit(main())
