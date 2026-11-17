#!/usr/bin/env python3
"""Actual I1T admission and the same pthread Wasm's explicit fallback, in the existing cgroup."""
import argparse
import hashlib
import json
import subprocess
import time
from pathlib import Path
from run_scene_vm import verify_gate, GATE

ROOT=Path(__file__).resolve().parents[2]
def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--runtime',type=Path,required=True)
    ap.add_argument('--pthread',type=Path,required=True)
    ap.add_argument('--output',type=Path,required=True)
    ap.add_argument('--engines',nargs='+',default=['int','aot'])
    a=ap.parse_args();verify_gate();out=a.output.resolve()
    assert out.is_relative_to(ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security')
    out.mkdir(parents=True,exist_ok=False)
    runtime=a.runtime.resolve();pthread=a.pthread.resolve()
    report=dict(runtime_sha256=sha(runtime),pthread_sha256=sha(pthread),source_sha256=sha(__file__),
                cgroup=str(GATE),memory_max=(GATE/'memory.max').read_text().strip(),
                swap_max=(GATE/'memory.swap.max').read_text().strip(),cases=[],complete=False)
    def run(label,flags,wasm,expected,require='',absent=''):
        verify_gate();argv=[str(runtime),*flags,'--run',str(wasm)];start=time.monotonic()
        with (out/(label+'.stdout')).open('wb') as stdout,(out/(label+'.stderr')).open('wb') as stderr:
            p=subprocess.run(argv,stdout=stdout,stderr=stderr,timeout=60)
        text=(out/(label+'.stderr')).read_text(errors='replace')
        okay=p.returncode==expected and (not require or require in text) and (not absent or absent not in text)
        report['cases'].append(dict(name=label,argv=argv,code=p.returncode,expected=expected,passed=okay,seconds=time.monotonic()-start,wasm_sha256=sha(wasm)))
        (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
        print(label,p.returncode,'PASS' if okay else 'FAIL',flush=True)
        if not okay:raise AssertionError((label,text[-5000:]))
    common=['--runtime-compile-threads','0','--wasi-threads','unlimited','unlimited','1048576','16777216','67108864','16777216',
            '--wasm-set-main-module-name','main']
    wasi=['--wasip1-single-create','main','--wasip1-single-enable','main']
    for interface in ['clock_time_get','proc_exit','sched_yield']:
        wasi+=['--wasip1-single-interface','main',interface,'allow']
    for permission in ['monotonic-clock','realtime-clock','proc-exit','sched-yield']:
        wasi+=['--wasip1-single-grant','main',permission]
    single=['--wasithread-single-create','main','--wasithread-single-enable','main','--wasithread-single-interface','main','thread-spawn','allow']
    # No WASIp1 imports: exposing I1T does not need Preview 1 at all.
    pure=out/'pure.wasm';wat=out/'pure.wat'
    wat.write_text('''(module
      (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
      (memory (export "memory") 1 2 shared)
      (func (export "_start")
        (if (i32.ne (call $spawn (i32.const 0)) (i32.const -1)) (then unreachable)))
      (func (export "wasi_thread_start") (param i32 i32) unreachable))''')
    subprocess.run(['wat2wasm','--enable-threads',str(wat),'-o',str(pure)],check=True)
    # WASI Threads re-instantiates the caller against the SAME imported shared
    # memory. Defined shared memory is a different instance, not a thread ABI.
    owner=out/'memory.wasm';owner_wat=out/'memory.wat'
    owner_wat.write_text('(module (memory (export "memory") 2 256 shared))')
    subprocess.run(['wat2wasm','--enable-threads',str(owner_wat),'-o',str(owner)],check=True)
    # The explicit private execution profile admits imported shared memory.
    # Keep the no-profile exposure fixture independent, and use the real thread
    # ABI memory shape for permission refusal inside that execution profile.
    threaded_pure=out/'pure-threaded.wasm';threaded_wat=out/'pure-threaded.wat'
    threaded_wat.write_text(wat.read_text().replace(
        '(memory (export "memory") 1 2 shared)',
        '(import "env" "memory" (memory 2 256 shared)) (export "memory" (memory 0))'))
    subprocess.run(['wat2wasm','--enable-threads',str(threaded_wat),'-o',str(threaded_pure)],check=True)
    for engine in a.engines:
        prefix=['--runtime-'+engine,*common,'--wasm-preload-library',str(owner),'env']
        if engine=='aot':prefix+=['--runtime-llvm-jit-cache-path','disable']
        ordinary=['--runtime-'+engine,'--runtime-compile-threads','0','--wasm-set-main-module-name','main']
        if engine=='aot':ordinary+=['--runtime-llvm-jit-cache-path','disable']
        run(engine+'-expose-no-profile',ordinary+single,pure,0,require='no private threaded execution profile')
        run(engine+'-default-hidden',ordinary,pure,127,require='wasithread')
        run(engine+'-pure-zero',prefix+single+['--wasithread-single-limit','main','max-extra-threads','0'],threaded_pure,0)
        run(engine+'-pthread-single-zero',prefix+wasi+single+['--wasithread-single-limit','main','max-extra-threads','0'],pthread,61)
        run(engine+'-pthread-single-allow',prefix+wasi+single,pthread,62)
        denied=['--wasithread-single-create','main','--wasithread-single-enable','main','--wasithread-single-interface','main','thread-spawn','deny']
        run(engine+'-pthread-deny',prefix+wasi+denied,pthread,61,require='wasithread')
        run(engine+'-global-cap',prefix+wasi+single+['--wasithread-global-limit','max-threads','1'],pthread,61)
        global_=['--wasithread-global-enable','--wasithread-global-interface','thread-spawn','allow']
        run(engine+'-implicit-global',prefix+wasi+global_,pthread,62)
        group=['--wasithread-group-create','work','--wasithread-group-add-module','work','main',
               '--wasithread-group-enable','work','--wasithread-group-interface','work','thread-spawn','allow']
        run(engine+'-group-zero',prefix+wasi+group+['--wasithread-group-limit','work','max-extra-threads','0'],pthread,61)
        run(engine+'-group-allow',prefix+wasi+group,pthread,62)
        run(engine+'-ratio-zero',prefix+wasi+single+['--wasithread-single-limit-ratio','main','max-extra-threads','0'],pthread,61,require='admitted-limit=0')
    assert report['runtime_sha256']==sha(runtime) and report['pthread_sha256']==sha(pthread)
    report['complete']=True;report['events_after']=(GATE/'memory.events').read_text()
    (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
