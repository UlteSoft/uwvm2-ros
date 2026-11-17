#!/usr/bin/env python3
"""Qualify one exact attachment-only VM build inside the existing resource gate.

Serial compilation, refusal, hostile-peer, multi-VM, debugger and optional
MuJoCo runs produce separate receipts. Completion applies to this suite only;
it is not full provider, vendor, hard-real-time or industrial qualification.
"""
import argparse,hashlib,json,os,resource,shutil,subprocess,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'test/0084.ros-runtime'))
from run_scene_vm import GATE,verify_gate,sha

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--mujoco',type=Path)
    parser.add_argument('--named-modules',action='store_true')
    args=parser.parse_args();verify_gate();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    runtime=args.runtime.resolve();out=args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True,exist_ok=False)
    build_path=runtime.parent/'full-build-results.json';build=json.loads(build_path.read_text())
    assert build['complete'] and build['binary_sha256']==sha(runtime)
    source=Path(build['source_root']);manifest=source/'source-manifest.json'
    report=dict(complete=False,scope=__doc__,runtime_sha256=sha(runtime),build_receipt_sha256=sha(build_path),
        source_root=str(source),manifest_sha256=sha(manifest),cgroup=str(GATE),memory_max=68719476736,swap_max=0,
        events_before=(GATE/'memory.events').read_text(),commands=[],inputs={},fixtures={})
    inputs=[Path(__file__),ROOT/'tools/ros_scene/interface-catalogue.json',ROOT/'tools/ros_scene/operation-contracts.json',
        ROOT/'tools/ros_scene/robot-provider-contracts.json',*sorted((ROOT/'test/0084.ros-runtime').glob('*.py')),
        *sorted((ROOT/'test/0084.ros-runtime').glob('driver*.cpp'))]
    for path in inputs:
        report['inputs'][str(path)]=sha(path);saved=out/'retained-inputs'/path.relative_to(ROOT)
        saved.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,saved);report['fixtures'][str(saved.relative_to(out))]=sha(saved)
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,argv,timeout=600):
        verify_gate();argv=list(map(str,argv));started=time.monotonic()
        with (out/(name+'.log')).open('w') as log:p=subprocess.run(argv,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT,timeout=timeout)
        report['commands'].append(dict(name=name,argv=argv,exit=p.returncode,seconds=time.monotonic()-started));save()
        assert p.returncode==0,(name,(out/(name+'.log')).read_text()[-3000:]);print('PASS '+name,flush=True)
    def suite(name,filename,extra=()):
        run(name,[sys.executable,ROOT/'test/0084.ros-runtime'/filename,'--runtime',runtime,'--output',out/name,*extra])
    try:
        flags=['/usr/bin/clang++-22','-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread',
            '-I'+str(source/'src'),'-isystem',source/'third-parties/fast_io/include']
        for name in ('driver_session','driver_attachment','platform_extension_core'):
            run('compile-'+name,[*flags,'-fsanitize=address,undefined',ROOT/'test/0084.ros-runtime'/(name+'.cpp'),'-o',out/name])
            run(name,[out/name])
        for name in ('driver_allocation','exchange_allocation'):
            wrap=['-Wl,--wrap=malloc','-Wl,--wrap=calloc','-Wl,--wrap=realloc']
            if name=='exchange_allocation':wrap+=['-Wl,--wrap=free']
            run('compile-'+name,[*flags,ROOT/'test/0084.ros-runtime'/(name+'.cpp'),*wrap,'-o',out/name]);run(name,[out/name])
        suite('driver-vm','run_driver_attachment_vm.py')
        suite('driver-cli','run_driver_cli.py')
        suite('shared-vm','run_driver_shared_vm.py')
        suite('module-vm','run_module_security_vm.py')
        suite('all-import-denials','run_scene_all_import_denials.py')
        run('debug',[sys.executable,ROOT/'test/0084.ros-runtime/run_scene_debug.py','--output',out/'debug'])
        run('allocators',[sys.executable,ROOT/'tools/ros_scene/check_owned_allocators.py','--output',out/'allocators.json'])
        run('catalogue',[sys.executable,ROOT/'tools/ros_scene/check_catalogue.py','--corpus',ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-ecosystem','--output',out/'catalogue.json'])
        run('schema',[sys.executable,ROOT/'tools/ros_scene/generate_schema_contract.py','--check'])
        if args.mujoco:
            for name,extra in [('mujoco-ordinary',[]),('mujoco-namespace',['--namespace'])]:
                suite(name,'run_driver_mujoco.py',['--source-root',source,'--mujoco',args.mujoco.resolve(),*extra])
            suite('mujoco-device','run_scene_mujoco.py',['--source-root',source,'--mujoco',args.mujoco.resolve()])
        if args.named_modules:
            # check_modules consumes flags, not a complete compile/link command.
            # Remove the original TU, objects and output without losing ABI,
            # optimization, debug, runtime or library-selection switches.
            def reusable(argv,compile):
                result=[];skip=False
                for token in argv:
                    if skip:skip=False;continue
                    if token=='-o':skip=True;continue
                    if token=='-c' or (not token.startswith('-') and token.endswith(('.cpp','.cc','.o'))):continue
                    result.append(token)
                return result
            compile_flags=out/'module-compile-flags.json';link_flags=out/'module-link-flags.json'
            compile_flags.write_text(json.dumps(reusable(json.loads((runtime.parent/'main.default.cpp.o.argv.json').read_text()),True)))
            link_flags.write_text(json.dumps(reusable(json.loads((runtime.parent/'link.argv.json').read_text()),False)))
            for mode in ('llvm','interpreter'):
                run('modules-'+mode,[sys.executable,ROOT/'tools/ros_scene/check_modules.py',
                    '--source-root',source,'--compile-argv',compile_flags,
                    '--link-argv',link_flags,'--isolated-host','--host-fp-mode',mode,'--output',out/('modules-'+mode)],1200)
        report['complete']=True
    finally:
        report['inputs_unchanged']=all(sha(path)==digest for path,digest in report['inputs'].items())
        report['runtime_unchanged']=sha(runtime)==report['runtime_sha256'] and sha(manifest)==report['manifest_sha256']
        report['complete']=report['complete'] and report['inputs_unchanged'] and report['runtime_unchanged']
        report['events_after']=(GATE/'memory.events').read_text();save()
    assert report['complete']

if __name__=='__main__':main()
