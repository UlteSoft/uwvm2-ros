#!/usr/bin/env python3
"""Qualify the explicit local scheduling FP preservation contract.

Core mode varies host controls and tests allocation/cancellation failures using
Clang ASan/UBSan and container GCC. Wasm mode appends dynamic-memory arithmetic
assertions immediately after each host call in the same Wasm function. It
covers all sixteen scheduling imports on interpreter and optimized LLVM, with
memory32/64 and memory index zero/one. No WASI or native ROS grants are installed.
Every workload must inherit the existing 64 GiB, zero-swap cgroup.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import time


HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('scene_vm_fixture', HERE/'run_scene_vm.py')
base = importlib.util.module_from_spec(spec)
spec.loader.exec_module(base)
ROOT, GATE = base.ROOT, base.GATE


class FPGuest(base.Guest):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.fp_sites = 0
        floats = [1, 2**-24, -1, -(2**-24), 2**-126, .5, 2**-149, 2, 0]
        doubles = [1, 2**-53, -1, -(2**-53), 2**-1022, .5, 2**-1074, 2, 0]
        payload = struct.pack('<9f', *floats).ljust(64, b'\0') + struct.pack('<9d', *doubles)
        encoded = ''.join('\\'+format(byte, '02x') for byte in payload)
        self.data.append(f'(data (memory $work) (i{self.width}.const 8192) "{encoded}")')

    def check(self, expression, expected=0):
        super().check(expression, expected)
        if ' call $' not in ' '+expression:
            return
        self.fp_sites += 1
        checks = []
        for bits, offset, expected_values in [
            (32, 8192, [0x3f800000, 0xbf800000, 0x00400000, 2, 0x7f800000]),
            (64, 8256, [0x3ff0000000000000, 0xbff0000000000000, 0x0008000000000000, 2, 0x7ff0000000000000])]:
            def load(index):
                return f'i{self.width}.const {offset+index*(bits//8)} f{bits}.load $work'
            for left, right, operation, result in zip([0,2,4,6,0], [1,3,5,7,8],
                                                       ['add','add','mul','mul','div'], expected_values):
                checks.append(f'{load(left)} {load(right)} f{bits}.{operation} i{bits}.reinterpret_f{bits} '
                              f'i{bits}.const {result} i{bits}.ne if unreachable end')
            # Negative square-root must produce NaN without an unmasked exception.
            checks.append(f'{load(2)} f{bits}.sqrt {load(2)} f{bits}.sqrt f{bits}.eq if unreachable end')
        self.checks[-1] = self.checks[-1][:-1]+'\n'+'\n'.join(checks)+')'


def configured_policy(operations):
    flags = base.policy(operations)
    for alias, purpose in [('signal','guard'),('other','guard'),('zero','guard'),('rate','guard'),('bytes','guard'),
                           ('no-observe','guard'),('no-read','clock.ros'),('time','clock.ros'),('steady','clock.steady'),
                           ('system','clock.system'),('timer','timer'),('wait','wait')]:
        actions = [row['name'] for row in operations if row['name'].startswith(purpose.split('.')[0]+'.')]
        actions += ['resource.close']
        if alias != 'no-observe': actions += ['resource.observe']
        if alias == 'no-read': actions.remove('clock.read')
        if purpose in ('clock.steady','clock.system'): actions.remove('clock.update')
        flags += base.grant(alias, purpose, actions)
    for alias, key, value in [('signal','max-handles','1'),('signal','max-resources','1'),('zero','max-resources','0'),
                              ('rate','delivery','may-reject'),('rate','calls-per-window','1'),
                              ('rate','window-ns','3600000000000'),('bytes','delivery','may-reject'),('bytes','bytes-per-window','0')]:
        flags += ['--ros-scene-limit','robot',alias,key,value]
    return flags


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode', choices=['core','prepare','wasm','all'], default='all')
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--source-root', type=Path, default=ROOT,
                        help='Production source snapshot; test fixtures stay in the checkout')
    parser.add_argument('--container', default='uwvm-sdk-resource-gate-20261024')
    args = parser.parse_args()
    base.verify_gate()
    if args.mode in ('wasm','all') and args.runtime is None:
        parser.error('--runtime is required for Wasm execution')
    production=args.source_root.resolve()
    def source(name): return (production if name.startswith('src/') else ROOT)/name
    out = args.output.resolve();out.mkdir(parents=True, exist_ok=True)
    paths = ['test/0084.ros-runtime/fp_control.cc', 'test/0084.ros-runtime/run_scene_fp.py',
             'test/0084.ros-runtime/run_scene_vm.py', 'test/0008.imported/wasi/wasip1/func/fp_control_probe.h',
             'src/uwvm2/imported/ros/runtime/contract.h','src/uwvm2/imported/ros/runtime/resources.h',
             'src/uwvm2/imported/ros/scene/authority.h','src/uwvm2/imported/ros/scene/broker.h',
             'src/uwvm2/imported/ros/scene/artifact_contract.h','src/uwvm2/imported/ros/scene/artifacts.h',
             'src/uwvm2/imported/ros/scene/storage.h','src/uwvm2/imported/ros/scene/output_contract.h',
             'src/uwvm2/imported/ros/scene/outputs.h','src/uwvm2/imported/ros/scene/byte_buffer.h',
             'src/uwvm2/imported/ros/scene/managed_lifecycle.h','src/uwvm2/imported/ros/scene/managed_lifecycle_contract.h',
             'src/uwvm2/uwvm/imported/scene/host.h']
    def hashes(): return {name: base.sha(source(name)) for name in paths}
    report = dict(scope='local scheduling FP controls and actual Wasm arithmetic; no native provider qualification',
                  mode=args.mode, cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino, memory_max=68719476736,
                  swap_max=0, memory_events_before=(GATE/'memory.events').read_text(), sources=hashes(),
                  source_root=str(production),runtime_sha256=base.sha(args.runtime) if args.runtime else None, runs=[], guests=[], complete=False)
    def save(): (out/(args.mode+'.json')).write_text(json.dumps(report,indent=2)+'\n')
    def run(name, command, data=None):
        base.verify_gate();start=time.monotonic()
        result=subprocess.run(command,cwd=ROOT,input=data,capture_output=True,timeout=180)
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        report['runs'].append(dict(name=name,argv=command,exit=result.returncode,seconds=time.monotonic()-start));save()
        assert result.returncode == 0, (name,result.returncode,result.stderr.decode(errors='replace')[-4000:])
        print('PASS',name,flush=True)
    if args.mode in ('core','all'):
        compiler=shutil.which('clang++');assert compiler
        binary=out/'fp-control-asan'
        run('core-clang-compile',[compiler,'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror',
            '-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread','-I'+str(production/'src'),str(HERE/'fp_control.cc'),'-o',str(binary)])
        run('core-clang',[str(binary)])
        # Docker exec enters the very same pre-existing container budget.
        script='''import pathlib,json,sys,subprocess,os,tempfile
assert pathlib.Path('/sys/fs/cgroup/memory.max').read_text().strip()=='68719476736'
assert pathlib.Path('/sys/fs/cgroup/memory.swap.max').read_text().strip()=='0'
scratch=tempfile.TemporaryDirectory(prefix='uwvm-scene-fp-',dir='/dev')
root=pathlib.Path(scratch.name)
for name,text in json.load(sys.stdin).items():
 p=root/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(text)
env=dict(os.environ,TMPDIR='/dev/shm')
print(subprocess.check_output(['g++','--version'],text=True),flush=True)
command=['g++','-std=c++23','-O2','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(root/'src'),str(root/'test/0084.ros-runtime/fp_control.cc'),'-o',str(root/'fp-control')]
subprocess.run(command,env=env,check=True,timeout=120)
subprocess.run([str(root/'fp-control')],check=True,timeout=30)
'''
        inputs=[name for name in paths if name.endswith(('.h','.cc')) and '/uwvm/imported/' not in name]
        run('core-gcc',['docker','exec','-i',args.container,'taskset','-c','4-7','python3','-c',script],
            json.dumps({name:source(name).read_text() for name in inputs}).encode())
    if args.mode in ('prepare','wasm','all'):
        operations=[dict(name=n,words=int(w),bytes=int(b)) for n,w,b in
                    re.findall(r' X\(\w+,"([^"]+)","[^"]+",(\d+),(-?\d+)\)',
                               source('src/uwvm2/imported/ros/runtime/contract.h').read_text())]
        assert len(operations)==16
        original=base.Guest;base.Guest=FPGuest
        try:
            for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
                for mode in ['success','denied','no-grants']:
                    name=f'fp-{mode}-{width}-memory{memory}'
                    if mode=='success': guest=base.success_guest(operations,width,memory)
                    else:
                        guest=FPGuest(operations,width,memory)
                        for operation in operations: guest.call(operation['name'],1,expected=13 if mode=='denied' else 9)
                    wat,wasm=out/(name+'.wat'),out/(name+'.wasm');wat.write_text(guest.render())
                    run(name+'-compile',['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)])
                    report['guests'].append(dict(name=name,sha256=base.sha(wasm),fp_call_sites=guest.fp_sites,
                                                arithmetic_checks_per_site=12))
                    if args.mode!='prepare':
                        flags=configured_policy(operations) if mode=='success' else base.policy(operations,'deny' if mode=='denied' else 'allow')
                        for engine in ['int','aot']:
                            engine_flags=['--runtime-'+engine]
                            if engine=='aot': engine_flags+=['--wasm-policy-global', 'runtime.llvm.policy','max','--runtime-llvm-jit-cache-path','disable']
                            run(name+'-'+engine,[str(args.runtime.resolve()),'--wasm-set-main-module-name','main',
                                *engine_flags,*flags,'--run',str(wasm)])
        finally: base.Guest=original
    base.verify_gate()
    report['sources_after']=hashes();assert report['sources_after']==report['sources']
    if args.runtime: assert base.sha(args.runtime)==report['runtime_sha256']
    report['memory_events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()


if __name__=='__main__': main()
