#!/usr/bin/env python3
"""Real cross-module thread-spawn caller/entry identity, not a mocked admission."""
import argparse
import hashlib
import json
import subprocess
import time
from pathlib import Path
from run_scene_vm import verify_gate, GATE

ROOT=Path(__file__).resolve().parents[2]
BASE=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--runtime',type=Path,required=True)
    ap.add_argument('--output',type=Path,required=True)
    ap.add_argument('--engines',nargs='+',default=['int','aot'])
    a=ap.parse_args();verify_gate();out=a.output.resolve()
    assert out.is_relative_to(BASE)
    out.mkdir(parents=True,exist_ok=False);runtime=a.runtime.resolve()
    def wat(name,text):
        source=out/(name+'.wat');binary=out/(name+'.wasm');source.write_text(text)
        subprocess.run(['wat2wasm','--enable-threads','--debug-names',str(source),'-o',str(binary)],check=True)
        return binary
    memory=wat('memory','(module (memory (export "memory") 1 2 shared))')
    side=wat('side','''(module
      (import "env" "memory" (memory 1 2 shared))
      (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
      (export "raw" (func $spawn))
      (func (export "launch") (param i32) (result i32) (call $spawn (local.get 0)))
      (func (export "wasi_thread_start") (param i32 i32)
        (i32.atomic.store (i32.const 0) (i32.const 77))
        (drop (memory.atomic.notify (i32.const 0) (i32.const 1)))))''')
    def main_wat(import_name,wanted,denied=False):
        check="""(if (i32.ne (local.get $tid) (i32.const -1)) (then (call $exit (i32.const 92))))
          (call $exit (i32.const 70))""" if denied else f"""(if (i32.le_s (local.get $tid) (i32.const 0)) (then (call $exit (i32.const 91))))
          (loop $wait
            (local.set $got (i32.atomic.load (i32.const 0)))
            (if (i32.ne (local.get $got) (i32.const 0))
              (then (if (i32.ne (local.get $got) (i32.const {wanted})) (then (call $exit (i32.const 93))))
                    (call $exit (i32.const 70))))
            (local.set $attempt (i32.add (local.get $attempt) (i32.const 1)))
            (if (i32.eq (local.get $attempt) (i32.const 100)) (then (call $exit (i32.const 94))))
            (drop (memory.atomic.wait32 (i32.const 0) (i32.const 0) (i64.const 10000000)))
            (br $wait))"""
        return f'''(module
          (import "env" "memory" (memory 1 2 shared))
          (import "side" "{import_name}" (func $spawn (param i32) (result i32)))
          (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
          (func (export "_start") (local $tid i32) (local $got i32) (local $attempt i32)
            (local.set $tid (call $spawn (i32.const 0))) {check})
          (func (export "wasi_thread_start") (param i32 i32)
            (i32.atomic.store (i32.const 0) (i32.const 11))
            (drop (memory.atomic.notify (i32.const 0) (i32.const 1)))))'''
    def grant(module,state):
        return ['--wasithread-single-create',module,'--wasithread-single-enable',module,
                '--wasithread-single-interface',module,'thread-spawn',state]
    common=['--runtime-compile-threads','0','--wasi-threads','unlimited','unlimited','1048576','16777216','67108864','131072',
            '--wasm-set-main-module-name','main','--wasm-preload-library',str(memory),'env',
            '--wasm-preload-library',str(side),'side',
            '--wasip1-single-create','main','--wasip1-single-enable','main',
            '--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit']
    report=dict(runtime_sha256=sha(runtime),source_sha256=sha(__file__),cgroup=str(GATE),cases=[],complete=False)
    for label,import_name,wanted,permission,denied in [
        ('defined-side-entry','launch',77,grant('side','allow'),False),
        ('raw-main-entry','raw',11,grant('side','deny')+grant('main','allow'),False),
        ('raw-denied-main','raw',0,grant('side','allow')+grant('main','deny'),True)]:
        guest=wat(label,main_wat(import_name,wanted,denied))
        for engine in a.engines:
            verify_gate();name=engine+'-'+label
            cmd=[str(runtime),'--runtime-'+engine,*common,*permission]
            if engine=='aot':cmd+=['--runtime-llvm-jit-cache-path','disable']
            cmd+=['--run',str(guest)];started=time.monotonic()
            with (out/(name+'.stdout')).open('wb') as stdout,(out/(name+'.stderr')).open('wb') as stderr:
                p=subprocess.run(cmd,stdout=stdout,stderr=stderr,timeout=90)
            report['cases'].append(dict(name=name,argv=cmd,returncode=p.returncode,expected=70,
                passed=p.returncode==70,seconds=time.monotonic()-started,wasm_sha256=sha(guest)))
            (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
            print(name,p.returncode,'PASS' if p.returncode==70 else 'FAIL',flush=True)
            if p.returncode!=70:raise AssertionError((name,(out/(name+'.stderr')).read_text(errors='replace')[-5000:]))
    assert report['runtime_sha256']==sha(runtime)
    report['complete']=True;(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
