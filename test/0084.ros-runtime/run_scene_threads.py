#!/usr/bin/env python3
"""Real shared-memory WASI child threads calling the independent scene host.

Two child lifetimes overlap at an atomic barrier. The main publishes one scene
capability and guard before spawn, children trigger that same admitted resource,
and the main verifies all three trigger events. Missing grants and denied
interfaces still run both children and finish with a distinct completion exit.
Only the launcher uses separately authorized WASI thread-spawn/proc_exit; no
legacy ROS interface implementation or authority is borrowed by the scene.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import time

sys.dont_write_bytecode=True
from run_scene_vm import verify_gate,GATE,sha

CANARY=0x45464748494a4b4c
COMPLETION=37


def guest(state):
    opened=0 if state in ('allow','zero-budget') else 13
    operation=0 if state=='allow' else 11 if state=='zero-budget' else 13 if state=='deny' else 9
    imports='''
 (import "env" "memory" (memory 1 2 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (import "uwvm_ros_v2" "abi" (func $abi (result i32)))
 (import "uwvm_ros_v2" "capability.open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros_v2" "capability.close" (func $close (param i64) (result i32)))
'''
    for name in ('guard.create','guard.trigger','guard.take','resource.close'):
        imports+=f' (import "uwvm_ros_v2" "{name}" (func ${name} (param i64 i32 i64 i64 i64 i64 i64) (result i32)))\n'
    # Scene call arguments and result storage are separate for main and each
    # child. Only the parent-created cap/guard words are immutable shared input.
    def call(name,cap='(i64.load (i32.const 256))',request=264,size=8,output=272,length=280,capacity=8):
        arg=lambda x:f'(i64.const {x})' if isinstance(x,int) else x
        return f'(call ${name} {cap} (i32.const 0) {arg(request)} {arg(size)} {arg(output)} {arg(capacity)} {arg(length)})'
    def check(expression,value):return f'(call $check {expression} (i32.const {value}))'
    def child_slot(offset):return f'(i32.add (i32.const {512+offset}) (i32.mul (local.get $index) (i32.const 64)))'
    def child_address(offset):return '(i64.extend_i32_u '+child_slot(offset)+')'
    main=f'''
 (func (export "_start") (local $first i32) (local $second i32)
  ;; No active data segment: child world creation cannot reset a live barrier.
  (i64.store (i32.const 0) (i64.const 0x00006c616e676973))
  (i64.store (i32.const 256) (i64.const {CANARY}))
  (i64.store (i32.const 264) (i64.const {CANARY}))
  {check('(call $abi)',13 if state=='deny' else 2)}
  {check('(call $open (i32.const 0) (i64.const 0) (i64.const 6) (i64.const 256))',opened)}
  {check(call('guard.create',request=0,size=0,output=264),operation)}
'''
    if state=='allow':
        main+=check(call('guard.trigger',capacity=0),0)
    else:
        main+=check('(i64.ne (i64.load (i32.const 264)) (i64.const '+str(CANARY)+'))',0)
        if state in ('missing-grant','deny'):
            main+=check('(i64.ne (i64.load (i32.const 256)) (i64.const '+str(CANARY)+'))',0)
    main+='''
  (local.set $first (call $bounded_spawn (i32.const 0)))
  (local.set $second (call $bounded_spawn (i32.const 1)))
  (call $wait_equal (i32.const 1024) (i32.const 2))
  (call $check (i32.eq (i32.load (i32.const 1040)) (local.get $first)) (i32.const 1))
  (call $check (i32.eq (i32.load (i32.const 1044)) (local.get $second)) (i32.const 1))
  (call $check (i32.ne (local.get $first) (local.get $second)) (i32.const 1))
  (i32.atomic.store (i32.const 1028) (i32.const 1))
  (drop (memory.atomic.notify (i32.const 1028) (i32.const 2)))
  (call $wait_equal (i32.const 1032) (i32.const 2))
'''
    if state=='allow':
        main+=check(call('guard.take'),0)
        main+=check('(i64.eq (i64.load (i32.const 272)) (i64.const 3))',1)
        main+=check(call('guard.take'),11)
        main+=check(call('resource.close',capacity=0),0)
    if opened==0:main+=check('(call $close (i64.load (i32.const 256)))',0)
    main+=f'(call $exit (i32.const {COMPLETION})) unreachable)\n'
    child=f'''
 (func (export "wasi_thread_start") (param $tid i32) (param $index i32)
  (call $check (i32.le_u (local.get $index) (i32.const 1)) (i32.const 1))
  (i32.store (i32.add (i32.const 1040) (i32.mul (local.get $index) (i32.const 4))) (local.get $tid))
  (drop (i32.atomic.rmw.add (i32.const 1024) (i32.const 1)))
  (drop (memory.atomic.notify (i32.const 1024) (i32.const 2)))
  (call $wait_equal (i32.const 1028) (i32.const 1))
  (i64.store {child_slot(0)} (i64.const {CANARY}))
  (i64.store {child_slot(8)} (i64.const {CANARY}))
  {check('(call $abi)',13 if state=='deny' else 2)}
  {check(f'(call $open (i32.const 0) (i64.const 0) (i64.const 6) {child_address(0)})',opened)}
'''
    if state=='allow':
        # Child A and B each use the capability minted by main; the group is
        # shared only because the defining module is explicitly scene-bound.
        child+=check(call('guard.trigger',output=child_address(8),length=child_address(16),capacity=0),0)
    else:
        child+=check(call('guard.create',request=0,size=0,output=child_address(8),length=child_address(16)),operation)
        child+=check('(i64.ne (i64.load '+child_slot(8)+') (i64.const '+str(CANARY)+'))',0)
    if opened==0:child+=check('(call $close (i64.load '+child_slot(0)+'))',0)
    else:child+=check('(i64.ne (i64.load '+child_slot(0)+') (i64.const '+str(CANARY)+'))',0)
    child+='''
  (drop (i32.atomic.rmw.add (i32.const 1032) (i32.const 1)))
  (drop (memory.atomic.notify (i32.const 1032) (i32.const 2))))
'''
    helpers='''
 (func $check (param $actual i32) (param $expected i32)
  (if (i32.ne (local.get $actual) (local.get $expected)) (then (call $exit (i32.const 101)) unreachable)))
 (func $bounded_spawn (param $index i32) (result i32) (local $attempts i32) (local $tid i32)
  (local.set $attempts (i32.const 64))
  (loop $retry
   (local.set $tid (call $spawn (local.get $index)))
   (if (i32.gt_s (local.get $tid) (i32.const 0)) (then (return (local.get $tid))))
   (local.set $attempts (i32.sub (local.get $attempts) (i32.const 1)))
   (if (i32.eqz (local.get $attempts)) (then (call $exit (i32.const 102)) unreachable))
   (drop (memory.atomic.wait32 (i32.const 1060) (i32.const 0) (i64.const 1000000)))
   (br $retry)) unreachable)
 (func $wait_equal (param $address i32) (param $wanted i32) (local $got i32) (local $attempts i32)
  (local.set $attempts (i32.const 1000))
  (block $done (loop $wait
   (local.set $got (i32.atomic.load (local.get $address)))
   (br_if $done (i32.eq (local.get $got) (local.get $wanted)))
   (local.set $attempts (i32.sub (local.get $attempts) (i32.const 1)))
   (if (i32.eqz (local.get $attempts)) (then (call $exit (i32.const 103)) unreachable))
   (drop (memory.atomic.wait32 (local.get $address) (local.get $got) (i64.const 10000000)))
   (br $wait))))
'''
    return '(module\n'+imports+'(export "memory" (memory 0))\n'+helpers+main+child+')\n'


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime',required=True,type=Path);parser.add_argument('--output',required=True,type=Path)
    parser.add_argument('--engines',nargs='+',default=['int','aot']);args=parser.parse_args();verify_gate()
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=False);runtime=args.runtime.resolve()
    report=dict(scope='actual shared-memory main + two overlapping WASI child threads calling scene ABI',
                cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max='68719476736',swap_max='0',
                events_before=(GATE/'memory.events').read_text(),runtime_sha256=sha(runtime),source_sha256=sha(__file__),
                cases=[],complete=False)
    def compile_wat(name,text):
        wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(text)
        subprocess.run(['wat2wasm','--enable-threads','--debug-names',str(wat),'-o',str(wasm)],check=True)
        return wasm
    owner=compile_wat('memory','(module (memory (export "memory") 1 2 shared))')
    common=['--runtime-compile-threads','0','--wasm-feature-enable-threads','--wasi-threads','3','3','1048576','16777216','67108864','131072',
            '--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main',
            '--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit',
            '--wasithread-single-create','main','--wasithread-single-enable','main',
            '--wasithread-single-interface','main','thread-spawn','allow',
            '--wasm-preload-library',str(owner),'env','--ros-scene-create','robot','--ros-scene-bind','robot','main']
    for state in ('allow','missing-grant','deny','zero-budget'):
        wasm=compile_wat(state,guest(state));flags=common.copy()
        for action in ('abi','capability.open','capability.close','guard.create','guard.trigger','guard.take','resource.close'):
            flags+=['--ros-scene-interface','robot',action,'deny' if state=='deny' else 'allow']
        if state!='missing-grant':
            flags+=['--ros-scene-grant','robot','signal','guard','signal','local','guard.create,guard.trigger,guard.take,resource.close']
        if state=='zero-budget':
            flags+=['--ros-scene-limit','robot','signal','delivery','may-reject',
                    '--ros-scene-limit','robot','signal','calls-per-window','0']
        for engine in args.engines:
            verify_gate();command=[str(runtime),'--runtime-'+engine,*flags]
            if engine=='aot':command+=['--runtime-llvm-jit-cache-path','disable']
            command+=['--run',str(wasm)];label=state+'-'+engine;begin=time.monotonic()
            # Regular sinks preserve exact VM diagnostics in this launch.
            with (out/(label+'.stdout')).open('wb') as stdout,(out/(label+'.stderr')).open('wb') as stderr:
                result=subprocess.run(command,stdout=stdout,stderr=stderr,timeout=90)
            passed=result.returncode==COMPLETION
            report['cases'].append(dict(state=state,engine=engine,argv=command,returncode=result.returncode,
                                        expected=COMPLETION,passed=passed,seconds=time.monotonic()-begin,guest_sha256=sha(wasm)))
            report['events_after']=(GATE/'memory.events').read_text();(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
            print(label,result.returncode,'PASS' if passed else 'FAIL',flush=True)
            if not passed:raise AssertionError((label,(out/(label+'.stderr')).read_text(errors='replace')[-5000:]))
    assert report['runtime_sha256']==sha(runtime) and report['source_sha256']==sha(__file__)
    report['complete']=True;(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')


if __name__=='__main__':main()
